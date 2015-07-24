/**
 *
 * \file
 *
 * \brief
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "asf.h"
#include <stdio.h>
#include <string.h>
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "lwip/inet.h"
#include "netif/etharp.h"
#include <string.h>
#include "os/include/net_init.h"
#include "driver/include/m2m_wifi.h"
#include "driver/include/m2m_types.h"
#include "tinyservices.h"
#include "conf_tinyservices.h"

extern void app_network_up(uint8_t *ipv4_addr, uint8_t *ipv4_mask, uint8_t *ipv6_addr);
extern void app_network_down(void);
extern void socket_in_tcpip_task(void);
extern err_t winc_netif_init(struct netif *netif);
extern struct netif winc_netif_sta;
extern struct netif winc_netif_c_mode;

static xTaskHandle net_tcpip_task;
static int net_started;
static xSemaphoreHandle net_start_sem;
#define NET_S_NET_UP			0x1
#define NET_S_DHCP_RUNNING		0x2
#define NET_S_AUTOIP_RUNNING	0x4
#define NET_S_DHCP_SVR_RUNNING	0x8
static uint32_t net_state_sta;
static uint32_t net_mode_sta;
static uint32_t net_state_c;
static uint32_t net_mode_c;

int random_number(void)
{
	// BUGBUG we must call srand() at some point with something that is a bit
	// random but not quite sure what at this stage.
	return rand();
}

extern tpfAppWifiCb gpfAppWifiCb;
static void status_callback(struct netif *netif)
{
	if (netif_is_up(netif)) {
		tstrM2MIPConfig2 strIpConfig;
		strIpConfig.u8StaticIP = (uint8_t *)&netif->ip_addr;
		strIpConfig.u8SubnetMask = (uint8_t *)&netif->netmask;
		strIpConfig.u8StaticIPv6 = (uint8_t *)netif->ip6_addr;
						
		if (gpfAppWifiCb && (netif == &winc_netif_sta)) {
			gpfAppWifiCb(M2M_WIFI_REQ_DHCP_CONF, &strIpConfig);		
		}
	}
}

void net_add_winc_netif(void)
{
	struct ip_addr ip_addr;
	ip_addr.addr = 0;

	/* Add WILC1000 STA interface. */
	netif_add(&winc_netif_sta, &ip_addr, &ip_addr, &ip_addr, 0, winc_netif_init, tcpip_input);
	netif_set_default(&winc_netif_sta);
	netif_create_ip6_linklocal_address(&winc_netif_sta, 1); /* Needed to sent data on the internet, not matching local netmask. */
	netif_set_status_callback(&winc_netif_sta, status_callback);
	
	/* Add WILC1000 AP/WD interface. */
	struct ip_addr mask = SN_MASK_IP;
	struct ip_addr gateway = GW_ADDR_IP;
	ip_addr = AP_ADDR_IP;
	netif_add(&winc_netif_c_mode, &ip_addr, &mask, &gateway, 0, winc_netif_init, tcpip_input);
	net_set_mode(NET_IF_C, NET_MODE_AP | NET_MODE_USE_DHCP_SVR);	
}

int net_in_tcpip_task(void)
{
	return (net_tcpip_task == xTaskGetCurrentTaskHandle());	
}

static void tcpip_init_done(void *arg)
{
	net_tcpip_task = xTaskGetCurrentTaskHandle();
	xSemaphoreGive(net_start_sem);
}

static void net_interface_up_imp(void *p)
{
	uint32_t net_if = (uint32_t)p;

	if (net_if == NET_IF_STA) {
		/* Bring up interface in lwIP. */
		netif_set_link_up(&winc_netif_sta);
		net_state_sta |= NET_S_NET_UP;
		
		/* Interface 1 (STA). */
		if (net_mode_sta & NET_MODE_USE_DHCP) {
			dhcp_start(&winc_netif_sta);
			net_state_sta |= NET_S_DHCP_RUNNING;
		} else if (net_mode_sta & NET_MODE_USE_LINK_LOCAL) {
			autoip_start(&winc_netif_sta);
			net_state_sta |= NET_S_AUTOIP_RUNNING;
		} else if (net_mode_sta & NET_MODE_USE_STATIC) {
			struct ip_addr addr;
			IP4_ADDR(&addr, CONF_IP_ADDR0, CONF_IP_ADDR1,
			CONF_IP_ADDR2, CONF_IP_ADDR3);
			netif_set_ipaddr(&winc_netif_sta, &addr);
			IP4_ADDR(&addr, CONF_NET_MASK0, CONF_NET_MASK1,
			CONF_NET_MASK2, CONF_NET_MASK3);
			netif_set_netmask(&winc_netif_sta, &addr);
			IP4_ADDR(&addr, CONF_GATEWAY_ADDR0, CONF_GATEWAY_ADDR1,
			CONF_GATEWAY_ADDR2, CONF_GATEWAY_ADDR3);
			netif_set_gw(&winc_netif_sta, &addr);
			netif_set_up(&winc_netif_sta);
		}
	}
	else {
		/* Bring up interface in lwIP. */
		netif_set_link_up(&winc_netif_c_mode);
		net_state_c |= NET_S_NET_UP;
		
		/* Interface 2 (Concurrent mode). */
		if (net_mode_c & NET_MODE_USE_DHCP_SVR) {
			netif_set_up(&winc_netif_c_mode);
			lwip_tiny_dhcpserver_start();
			net_state_c |= NET_S_DHCP_SVR_RUNNING;
		}
	}
}

static void net_interface_down_imp(void *p)
{
	uint32_t net_if = (uint32_t)p;

	if (net_if == NET_IF_STA) {
		netif_set_link_down(&winc_netif_sta);
		net_state_sta &= ~NET_S_NET_UP;
		net_mode_sta &= ~(NET_MODE_USE_DHCP | NET_MODE_AP);
		if (net_state_sta & NET_S_DHCP_RUNNING) {
			dhcp_stop(&winc_netif_sta);
			net_state_sta &= ~NET_S_DHCP_RUNNING;
		}
		if (net_state_sta & NET_S_AUTOIP_RUNNING) {
			autoip_stop(&winc_netif_sta);
			net_state_sta &= ~NET_S_AUTOIP_RUNNING;
		}
		netif_set_down(&winc_netif_sta);
	}
	else {
		netif_set_link_down(&winc_netif_c_mode);
		net_state_c &= ~NET_S_NET_UP;
		net_mode_c &= ~NET_MODE_AP;
		if (net_state_c & NET_S_DHCP_SVR_RUNNING) {
			lwip_tiny_dhcpserver_stop();
			net_state_c &= ~NET_S_DHCP_SVR_RUNNING;
		}
		netif_set_down(&winc_netif_c_mode);
	}
}

void net_interface_up(uint32_t net_if)
{
	tcpip_callback_with_block((tcpip_callback_fn)net_interface_up_imp, (void *)net_if, 0);
}

void net_interface_down(uint32_t net_if)
{
	tcpip_callback_with_block((tcpip_callback_fn)net_interface_down_imp, (void *)net_if, 0);
}

void net_set_mode(uint32_t net_if, uint32_t mode)
{
	if (net_if == NET_IF_STA) {
		net_mode_sta = mode;
		
		if ((mode & NET_MODE_USE_DHCP)) {
			if ((net_state_sta & (NET_S_DHCP_RUNNING | NET_S_NET_UP)) == NET_S_NET_UP) {
				dhcp_start(&winc_netif_sta);
				net_state_sta |= NET_S_DHCP_RUNNING;
			}
			} else {
			if (net_state_sta & NET_S_DHCP_RUNNING) {
				dhcp_stop(&winc_netif_sta);
				net_state_sta &= ~NET_S_DHCP_RUNNING;
			}
		}
	}
	else {
		net_mode_c = mode;
	}
}

void net_init(void)
{
	if (!net_started) {
		net_started = 1;
		vSemaphoreCreateBinary(net_start_sem);
		xSemaphoreTake(net_start_sem, portMAX_DELAY);
		tcpip_init(tcpip_init_done, 0);
		xSemaphoreTake(net_start_sem, portMAX_DELAY);
		vSemaphoreDelete(net_start_sem);
	}
}
