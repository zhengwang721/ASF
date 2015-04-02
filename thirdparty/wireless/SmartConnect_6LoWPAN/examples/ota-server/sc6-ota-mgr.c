/*
 * Copyright (c) 2011, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
 /**
 * \mainpage
 * \section preface Preface
 * Over the air module manager
 */
#include "contiki.h"
#include "contiki-conf.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "simple-udp.h"
#include "servreg-hack.h"
#include "net/rpl/rpl.h"
#include "compiler.h"
#include "stdbool.h"
#include "widbg.h"
#include "widbg_mgr.h"
#include "widbg_common.h"
#include "widbg_upgrade.h"
#include "widbg_debug.h"
#ifdef WIDBG_SERVER
#include "widbg_mgr_server.h"
#include "widbg_parser.h"
#else
#include "widbg_mgr_client.h"
#endif
#ifndef WIDBG_SERVER
#include "widbg_nvm.h"
#endif
#include <stdio.h>
#include <string.h>
#include "leds.h"
#include "port.h"
#include "delay.h"
#define UDP_PORT 5321
#define SERVICE_ID 190

#define SEND_INTERVAL		(60 * CLOCK_SECOND)

#define PRINTF(...) 
/* Currently UPGRADE and COMMON module is added for SC6LoWPAN */
/* TODO: Inlcude the macro definition in project symbols */
#define WIDBG_COMMON_SUPPORT   1
#define WIDBG_UPGRADE_SUPPORT  1

#define EXTND_ADDR_SIZE        8

#define TIMER_NONE             0
#define TIMER_INTERVAL         1
#define TIMER_PERIODIC         2

#define TIMER_STARTED          1

uint16_t global_shortaddr;
uint64_t global_extndaddr;
uint16_t pan_id = IEEE802154_CONF_PANID;
/*---------------------------------------------------------------------------*/
PROCESS(otau_server_process, "Over-The-Air  client process");
PROCESS(otau_server_common_process, "Over-The-Air client common module process");
PROCESS(otau_server_upgrade_process, "Over-The-Air client Upgrade  module process");

//PROCESS(otau_client_debug_process, "Over-The-Air client Debug  module process");
AUTOSTART_PROCESSES(&otau_server_process,&otau_server_common_process,
	&otau_server_upgrade_process/*,&otau_client_debug_process*/);

static struct simple_udp_connection unicast_connection;
static uint32_t last_received; 
uint8_t last_received_updated;
#if (WIDBG_COMMON_SUPPORT == 1)
static struct ctimer ota_common_tmr;
uint8_t ota_common_tmr_mode;
uint8_t ota_common_data[128];
uint8_t ota_common_recv_data[128];
#endif
#if (WIDBG_UPGRADE_SUPPORT == 1)
static struct ctimer ota_upgrade_tmr;
uint8_t ota_upgrade_tmr_mode;
uint8_t ota_upgrade_data[128];
uint8_t ota_upgrade_recv_data[128];
#endif
#if (WIDBG_DEBUG_SUPPORT == 1)
static struct ctimer ota_debug_tmr;
uint8_t ota_debug_tmr_mode;
uint8_t ota_debug_data[128];
uint8_t ota_debug_recv_data[128];
#endif

static void widbg_data_conf(module_code_t msg_code,uint8_t *addr,uint8_t status);
node_info_t node_info = {
	.image_start = 0x00002000,
	.image_size = 0x00002D7F,
	.new_image = 0,
	.supported_mode = 0x03,
	.firmware = "SC6lowpan ",
	.firmware_version = "1.0.0",
	.board = "SAMR21XPro",
	.board_version = "1.0.0"
};
/*---------------------------------------------------------------------------*/


void widbg_mgr_data_req(module_code_t msg_code, addr_mode_t addr_mode, uint8_t *addr, void *payload, uint8_t len)
{
    uip_ipaddr_t client_addr;
	uint64_t client_global_prefix = 0x000000000000aaaa;
	if (EXTENDED_ADDR_MODE != addr_mode)
	{
		//srv_addr = servreg_hack_lookup(SERVICE_ID);
		if(addr != NULL) {
			memcpy(&(client_addr.u16[4]),addr,PREFIX_ADDR_SIZE);
			memcpy(&(client_addr.u16[0]),&client_global_prefix,NATIVE_ADDR_SIZE);
			PRINTF("\r\nOTA CLIENT ADDRESS: ");
			uip_debug_ipaddr_print(&client_addr);
		} 
		else {
			uip_create_linklocal_allnodes_mcast(&client_addr);
		}
#if (WIDBG_COMMON_SUPPORT == 1)		
			if(COMMON == msg_code)
			{
				memcpy(ota_common_data, &msg_code, sizeof(msg_code));
		        memcpy(ota_common_data+1, payload, len);
				simple_udp_sendto(&unicast_connection, ota_common_data, len + sizeof(msg_code), &client_addr);
				widbg_data_conf(msg_code,addr,SUCCESS);
			}
			else
#endif		
#if (WIDBG_UPGRADE_SUPPORT == 1)			
			if(UPGRADE == msg_code)
			{
				memcpy(ota_upgrade_data, &msg_code, sizeof(msg_code));
		        memcpy(ota_upgrade_data+1, payload, len);
				simple_udp_sendto(&unicast_connection, ota_upgrade_data, len + sizeof(msg_code), &client_addr);
				memset(ota_upgrade_recv_data,0,128);
				widbg_data_conf(msg_code,addr,SUCCESS);

			}
			else
#endif
#if (WIDBG_DEBUG_SUPPORT == 1)				
			if(DEBUG == msg_code)
			{
				memcpy(ota_debug_data, &msg_code, sizeof(msg_code));
		        memcpy(ota_debug_data+1, payload, len);
				simple_udp_sendto(&unicast_connection, ota_debug_data, len + sizeof(msg_code), &client_addr);
				widbg_data_conf(msg_code,addr,SUCCESS);
			}
#endif			
			{
			}
		  
	}

}
/*****************************************************************************
*****************************************************************************/

void ota_mgr_init(void)
{
#ifndef WIDBG_SERVER	
	uint32_t* nvm_addr_loc = (uint32_t *)NVM_APP_SHORT_ADDR;
	widbg_nvm_init();

	nvm_addr_loc = (uint32_t *) NVM_DEVICE_INFO_LOCATION;
	if(0xFFFFFFFF == *nvm_addr_loc)
	{
		widbg_nvm_write(MEMORY_ABSOLUTE_ADDRESS, (uint32_t)nvm_addr_loc, sizeof(node_info_t),(uint8_t *)&node_info);
	}
	else
	{
		widbg_nvm_read(MEMORY_ABSOLUTE_ADDRESS, (uint32_t)nvm_addr_loc, sizeof(node_info_t),(uint8_t *)&node_info);
	}
#endif	
	widbg_common_init();
	widbg_upgrade_init();
	//widbg_debug_init();
}
/**/
/*---------------------------------------------------------------------------*/
static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
    if((datalen > 0) && (NULL != data))
	{
#if (WIDBG_COMMON_SUPPORT == 1)
		if(COMMON == *data)
		{
			widbg_common_rcvd_frame(NATIVE_ADDR_MODE, (uint16_t *)(&sender_addr->u16[4]), datalen-1, data+1,255);
		}
		else 
#endif		
#if (WIDBG_UPGRADE_SUPPORT == 1)
		if (UPGRADE == *data)
		{
			memcpy(ota_upgrade_recv_data,data+1,datalen-1);
			if(0x01 == ota_upgrade_recv_data[0] && 0x04 == ota_upgrade_recv_data[1])
			{
				if(last_received_updated && memcmp(&last_received,ota_upgrade_recv_data[3],4))
				{
					LED_On(LED0_PIN);
					delay_ms(500);
					LED_Off(LED0_PIN);
				}
				if(last_received == 0)
				{
					last_received_updated = 1;
				}
				memcpy(&last_received,ota_upgrade_recv_data[3],4);
				
			}
			widbg_upgrade_rcvd_frame(NATIVE_ADDR_MODE, (uint16_t *)(&sender_addr->u16[4]), datalen-1, ota_upgrade_recv_data,255);
		}
		else 
#endif
#if (WIDBG_DEBUG_SUPPORT == 1)		
		if (DEBUG == *data)
		{
			widbg_debug_rcvd_frame(NATIVE_ADDR_MODE, (uint16_t *)(&sender_addr->u16[4]), datalen-1, data+1,255);
		}
		else
#endif		
		{
			PRINTF("\r\nERROR: Unknown data type");
		}
	}
}

uint8_t *get_node_address(addr_mode_t addr_mode)
{

	if (NATIVE_ADDR_MODE == addr_mode)
	{
		return (uint8_t *)&global_extndaddr;
	}
	else
	{
		return (uint8_t *)&global_extndaddr;
	}
}


void widbg_mgr_timer_start(module_code_t msg_code, uint32_t interval, widbg_timer_mode_t mode, void *handler)
{
	switch (msg_code)
	{
#if (WIDBG_COMMON_SUPPORT == 1)	
	case COMMON:
	{	
		if(ota_common_tmr_mode != TIMER_NONE)
		{
			ctimer_stop(&ota_common_tmr);
		}
	    ctimer_set(&ota_common_tmr,interval,handler,NULL);
		if(TIMER_MODE_SINGLE == mode)
			ota_common_tmr_mode = TIMER_INTERVAL;
		else
		{
			ota_common_tmr_mode = TIMER_PERIODIC;
			process_post(&otau_server_common_process,TIMER_STARTED,NULL);
		}
	}
	break;
#endif		
#if (WIDBG_UPGRADE_SUPPORT == 1)		
	case UPGRADE:
	{
		if(ota_upgrade_tmr_mode != TIMER_NONE)
		{
			ctimer_stop(&ota_common_tmr);
		}	
		ctimer_set(&ota_upgrade_tmr,interval,handler,NULL);
		if(TIMER_MODE_SINGLE == mode)
			ota_upgrade_tmr_mode = TIMER_INTERVAL;
		else
		{
			ota_upgrade_tmr_mode = TIMER_PERIODIC;		
			process_post(&otau_server_upgrade_process,TIMER_STARTED,NULL);
		}
	}
	break;
#endif
#if (WIDBG_DEBUG_SUPPORT == 1)		
	case DEBUG:
		if(ota_debug_tmr_mode != TIMER_NONE)
		{
			ctimer_stop(&ota_common_tmr);
		}	
		ctimer_set(&ota_debug_tmr,interval,handler,NULL);
		if(TIMER_MODE_SINGLE == mode)
			ota_debug_tmr_mode = TIMER_INTERVAL;
		else
		{
			ota_debug_tmr_mode = TIMER_PERIODIC;
			process_post(&otau_server_debug_process,TIMER_STARTED,NULL);
		}
		break;
#endif		
	default:
		break;
	}
}
static void widbg_data_conf(module_code_t msg_code,uint8_t *addr,uint8_t status)
{
#if (WIDBG_COMMON_SUPPORT == 1)	
    if(COMMON == msg_code)
    {
	    widbg_common_sent_frame(NATIVE_ADDR_MODE, addr, status);
    }
    else
#endif		
#if (WIDBG_UPGRADE_SUPPORT == 1)	
	if(UPGRADE == msg_code)
    {
	    widbg_upgrade_sent_frame(NATIVE_ADDR_MODE, addr, status);
    }
    else 
#endif
#if (WIDBG_DEBUG_SUPPORT == 1)		
	if(DEBUG == msg_code)
    {
	    widbg_debug_sent_frame(NATIVE_ADDR_MODE, addr, status);
    }
	else
#endif		
	{
		/* Log error */
	}
}
	
void widbg_mgr_timer_stop(module_code_t msg_code)
{
	switch (msg_code)
	{
#if (WIDBG_COMMON_SUPPORT == 1)	
		case COMMON:
			ctimer_stop(&ota_common_tmr);
			ota_common_tmr_mode = TIMER_NONE;
			break;
#endif		
#if (WIDBG_UPGRADE_SUPPORT == 1)			
		case UPGRADE:
			ctimer_stop(&ota_upgrade_tmr);
			ota_upgrade_tmr_mode = TIMER_NONE;
			break;
#endif
#if (WIDBG_DEBUG_SUPPORT == 1)			
		case DEBUG:
			ctimer_stop(&ota_debug_tmr);
			ota_debug_tmr_mode = TIMER_NONE;
			break;
#endif			
		default:
			break;
	}
}

uint8_t *get_pan_id(void)
{
	return &pan_id;
}

void widbg_mgr_set_channel(uint8_t channel)
{
	rf_set_channel(channel);
}

void widbg_mgr_led(widbg_led_t led_state)
{
	if (WIDBG_LED_ON == led_state)
	{
		leds_on(LED0);
	}
	else if (WIDBG_LED_OFF == led_state)
	{
		leds_off(LED0);
	} 
	else
	{
		leds_toggle(LED0);
	}
}

void widbg_mgr_reset_device(void)
{
	NVIC_SystemReset();
}

/*---------------------------------------------------------------------------*/
#if (WIDBG_COMMON_SUPPORT == 1)	
PROCESS_THREAD(otau_server_common_process, ev, data)
{

  PROCESS_BEGIN();
  while(1) {
	PROCESS_WAIT_EVENT();
	if(ev == TIMER_STARTED)
	{	
		while(TIMER_PERIODIC == ota_common_tmr_mode)
		{
			PROCESS_WAIT_EVENT_UNTIL(ctimer_expired(&ota_common_tmr));
			ctimer_reset(&ota_common_tmr);
		}
	}
  }	
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
#endif		
#if (WIDBG_UPGRADE_SUPPORT == 1)	
PROCESS_THREAD(otau_server_upgrade_process, ev, data)
{

  PROCESS_BEGIN();
  while(1) {
	PROCESS_WAIT_EVENT();
	if(ev == TIMER_STARTED)
	{	
		while(TIMER_PERIODIC == ota_upgrade_tmr_mode)
		{
			PROCESS_WAIT_EVENT_UNTIL(ctimer_expired(&ota_upgrade_tmr));
			ctimer_reset(&ota_upgrade_tmr);
		}
	}
  }	
  PROCESS_END();
}
#endif
/*---------------------------------------------------------------------------*/
#if (WIDBG_DEBUG_SUPPORT == 1)	
PROCESS_THREAD(otau_server_debug_process, ev, data)
{

  PROCESS_BEGIN();
  while(1) {
	PROCESS_WAIT_EVENT();
	if(ev == TIMER_STARTED)
	{	
		while(TIMER_PERIODIC == ota_debug_tmr_mode)
		{
			PROCESS_WAIT_EVENT_UNTIL(ctimer_expired(&ota_debug_tmr));
			ctimer_reset(&ota_debug_tmr);
		}
	}
  }	
  PROCESS_END();
}
#endif


/*---------------------------------------------------------------------------*/
static uip_ipaddr_t *
set_global_address(void)
{
  static uip_ipaddr_t ipaddr;
  int i;
  uint8_t state;

  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
  memcpy(((uint8_t *)&global_extndaddr),&(ipaddr.u8[8]),NATIVE_ADDR_SIZE);
  PRINTF("IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
    }
  }

  return &ipaddr;
}
/*---------------------------------------------------------------------------*/
static void
create_rpl_dag(uip_ipaddr_t *ipaddr)
{
  struct uip_ds6_addr *root_if;

  root_if = uip_ds6_addr_lookup(ipaddr);
  if(root_if != NULL) {
    rpl_dag_t *dag;
    uip_ipaddr_t prefix;
    
    rpl_set_root(RPL_DEFAULT_INSTANCE, ipaddr);
    dag = rpl_get_any_dag();
    uip_ip6addr(&prefix, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
    rpl_set_prefix(dag, &prefix, 64);
    PRINTF("created a new RPL dag\n");
  } else {
    PRINTF("failed to create a new RPL DAG\n");
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(otau_server_process, ev, data)
{
  uip_ipaddr_t *ipaddr;

  PROCESS_BEGIN();

  //servreg_hack_init();

  ipaddr = set_global_address();

  create_rpl_dag(ipaddr);
  ota_mgr_init();
  //servreg_hack_register(SERVICE_ID, ipaddr);

  simple_udp_register(&unicast_connection, UDP_PORT,
                      NULL, UDP_PORT, receiver);

  while(1) {
    PROCESS_WAIT_EVENT();
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/




