/**
 * \file client_common.c
 *
 * \brief Client common implementation
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 *
 */

/*
 * Copyright (c) 2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the Over-the-Air Client Application
 * //TODO
 */
#include <stdio.h>
#include "stddef.h"
#include "string.h"
#include "stdint.h"
#include "client_common.h"
#include "ota_common.h"
#include "ota_mgr.h"
#include "ota_mgr_client.h"
#include "ota_nvm.h"

static void send_ota_client_notify(void);

client_info_indication_t client_info = {
	.msg_id = OTA_CLIENT_INFO
	};

common_timer_state_t timer_state;
uint8_t led_toggle_count;
uint32_t client_notify_interval_ms;
uint8_t common_confirm_wait = 0;

void ota_common_init(void)
{
	timer_state = CLIENT_NOTIFY_STATE;
	client_notify_interval_ms = CLIENT_NOTIFY_INTERVAL_MS;
	ota_mgr_timer_start(COMMON, client_notify_interval_ms, TIMER_MODE_SINGLE, ota_common_timer_handler);
}

void ota_common_rcvd_frame(uint8_t addr_mode, uint8_t *src_addr, uint8_t length, const uint8_t *payload, uint8_t lqi)
{
	uint8_t msg_code = *payload;
	switch (msg_code)
	{
		case OTA_CLIENT_DISCOVERY:
		{
			uint16_t interval_sec;
			memcpy((uint8_t *)&interval_sec, payload + 1, sizeof(interval_sec));
			if (interval_sec)
			{
				client_notify_interval_ms = interval_sec * 1000;
				if (CLIENT_NOTIFY_INTERVAL_MS > client_notify_interval_ms)
				{
					client_notify_interval_ms = CLIENT_NOTIFY_INTERVAL_MS;
				}
				ota_mgr_timer_start(COMMON, client_notify_interval_ms, TIMER_MODE_SINGLE, ota_common_timer_handler);
			}
			else
			{
				ota_mgr_timer_stop(COMMON);
			}
			break;
		}
		case OTA_CLIENT_INFO_REQ:
		{
			if(!common_confirm_wait)
			{
				ota_nvm_read(MEMORY_NODE_INFO_ADDRESS, 9, 31, &client_info.supported_mode);
				common_confirm_wait = 1;
				printf("\r\n Client info request");
				ota_mgr_data_req(COMMON, addr_mode, src_addr, &client_info, sizeof(client_info_indication_t));
			}
			break;
		}
		case OTA_IDENTIFY_REQ:
		{
			ota_mgr_timer_start(COMMON, LED_TOGGLE_INTERVAL_MS, TIMER_MODE_SINGLE, ota_common_timer_handler);
			timer_state = IDENTIFY_STATE;
			led_toggle_count = LED_TOGGLE_COUNT;
		}
		break;
		case OTA_RESET_REQ:
		{
			ota_mgr_reset_device();
			break;
		}
	}
}

void ota_common_sent_frame(uint8_t addr_mode, uint8_t *addr, uint8_t status)
{    
	common_confirm_wait = 0;
}

void ota_common_timer_handler(void)
{
	if(CLIENT_NOTIFY_STATE == timer_state)
	{
		if (!common_confirm_wait)
			send_ota_client_notify();
		ota_mgr_timer_start(COMMON, client_notify_interval_ms, TIMER_MODE_SINGLE, ota_common_timer_handler);
	}
	else if (IDENTIFY_STATE == timer_state)
	{
		ota_mgr_led(OTA_LED_TOGGLE);
		--led_toggle_count;
		ota_mgr_timer_start(COMMON, LED_TOGGLE_INTERVAL_MS, TIMER_MODE_SINGLE, ota_common_timer_handler);
		if(!led_toggle_count)
		{
			ota_mgr_timer_start(COMMON, client_notify_interval_ms - (LED_TOGGLE_INTERVAL_MS * LED_TOGGLE_COUNT), TIMER_MODE_SINGLE, ota_common_timer_handler);
			timer_state = CLIENT_NOTIFY_STATE;
		}
	}
}

static void send_ota_client_notify(void)
{
	client_notify_indication_t client_notify;
	client_notify.msg_id = OTA_CLIENT_NOTIFY;
	client_notify.dev_type = ROUTER;
	client_notify.ieee_addr_mode = EXTENDED_ADDR_MODE;
	memcpy((uint8_t *)&client_notify.ieee_addr, get_node_address(EXTENDED_ADDR_MODE), EXTENDED_ADDR_SIZE);
	memcpy((uint8_t *)&client_notify.pan_id, get_pan_id(), 2);
	client_notify.parent_addr_mode = NATIVE_ADDR_MODE;
	memcpy(&client_notify.parent_short_addr, get_parent_native_addr(),NATIVE_ADDR_SIZE);
	common_confirm_wait = 1;
	ota_mgr_data_req(COMMON, NATIVE_ADDR_MODE,NULL, &client_notify, sizeof(client_notify_indication_t));
	
}
