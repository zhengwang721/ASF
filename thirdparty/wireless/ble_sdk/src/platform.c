/**
 * \file
 *
 * \brief Platform Abstraction layer for BLE applications
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "platform.h"
#include "conf_serialdrv.h"
#include "serial_drv.h"
#include "serial_fifo.h"

static volatile uint32_t cmd_cmpl_flag = 0;
static volatile uint32_t event_flag = 0;

extern ser_fifo_desc_t ble_usart_tx_fifo;
extern ser_fifo_desc_t ble_usart_rx_fifo;

typedef struct ble_event_frame{
	uint8_t start_byte;
	uint16_t msg_id;
	uint16_t dest_task_id;
	uint16_t src_task_id;
	uint16_t payload_len;
	uint8_t payload[BLE_MAX_TX_PAYLOAD_SIZE];
}ble_event_frame_t;

//#define BLE_DBG_ENABLE
#define DBG_LOG_BLE		printf

#ifdef BLE_DBG_ENABLE
uint8_t rx_buf[256];
uint16_t rx_buf_idx;
#endif

static uint32_t timer_done(void);

at_ble_status_t platform_init(void* platform_params)
{	
	ble_configure_control_pin();	
	configure_serial_drv();
        /* Keep compiler happy */
        platform_params = platform_params;
	return AT_BLE_SUCCESS;
}

void platform_interface_send(uint8_t* data, uint32_t len)
{
#ifdef BLE_DBG_ENABLE
	uint32_t i;

	DBG_LOG_BLE("Rx:%d: ", rx_buf_idx);
	for (i = 0; i < rx_buf_idx; i++)
	{
		DBG_LOG_BLE("0x%X, ", rx_buf[i]);
	}
	rx_buf_idx = 0;
	DBG_LOG_BLE("TxLen:%d: ", len);
	for (i = 0; i < len; i++)
	{
		DBG_LOG_BLE("0x%X, ", data[i]);
		
	}
#endif
	serial_drv_send(data, len);	
}

void platform_cmd_cmpl_signal()
{
	cmd_cmpl_flag = 1;
}

uint32_t ticks = 0;
uint32_t ctr8 = 190; // 1 ms @ 8 MHz
uint32_t ctr48 = 615; // 1ms @ 48 MHz


/* Timeout in ms */
void start_timer(uint32_t timeout);
void start_timer(uint32_t timeout)
{

#if SAMG55	
	uint32_t main_clk  = sysclk_get_cpu_hz();
#endif

#if SAMD21
	uint32_t main_clk  = system_gclk_gen_get_hz(0);
#endif
	double ctr = 1.0;
	if (main_clk == 8000000)
		ticks = ctr8*timeout;
	else if (main_clk == 48000000)
		ticks = ctr48*timeout;
	else
	{
		ctr = 615.0 * (double)((double)main_clk/48000000);
		ticks = (uint32_t)ctr * timeout;
	}
	ticks *= 2;
}


static uint32_t timer_done(void)
{
	return --ticks; 
}

void platform_cmd_cmpl_wait(bool* timeout)
{
	uint32_t t_rx_data;
	
	start_timer(4000);
	do 
	{
		if(ser_fifo_pull_uint8(&ble_usart_rx_fifo, (uint8_t *)&t_rx_data) == SER_FIFO_OK)
		{
			platform_interface_callback((uint8_t *)&t_rx_data, 1);
		}
	}while((cmd_cmpl_flag != 1) && (timer_done()>0));

	if (cmd_cmpl_flag == 1)
	{
	#ifdef BLE_DBG_ENABLE
		DBG_LOG_BLE("AS\n");
	#endif
		cmd_cmpl_flag = 0;
	}
	else
	{
		*timeout = true;
	#ifdef BLE_DBG_ENABLE
		DBG_LOG_BLE("AF\n");
	#endif
	}
}

void platform_event_signal()
{
	event_flag = 1;
}

uint8_t platform_event_wait(uint32_t timeout)
{
	uint8_t status = AT_BLE_SUCCESS;
	uint32_t t_rx_data;
	
	start_timer(timeout);
	do 
	{
		if(ser_fifo_pull_uint8(&ble_usart_rx_fifo, (uint8_t *)&t_rx_data) == SER_FIFO_OK)
		{
			platform_interface_callback((uint8_t *)&t_rx_data, 1);
		}
	}while((event_flag != 1) && (timer_done()>0));
	if (event_flag == 1)
	{
		event_flag = 0;
	#ifdef BLE_DBG_ENABLE
		DBG_LOG_BLE("SS\n");
	#endif
	}	
	else
	{
		status = AT_BLE_TIMEOUT;
	#ifdef BLE_DBG_ENABLE
		DBG_LOG_BLE("SF\n");
	#endif
	}
	return status;
}

void serial_rx_callback(void) {}

void serial_tx_callback(void){}



