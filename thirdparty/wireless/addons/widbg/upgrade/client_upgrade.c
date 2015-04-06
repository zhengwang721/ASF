/**
 * \file client_upgrade.c
 *
 * \brief OTA Client implementation
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
#include "client_upgrade.h"
#include "widbg_upgrade.h"
#include "widbg_nvm.h"
#include "widbg_mgr.h"
#include "widbg_mgr_client.h"
#include "port.h"
#define DEFAULT_IMAGE_REQ_INTERVAL  (1000)
#define DEFAULT_IMAGE_RESP_INTERVAL (5000)
#define DEFAULT_SWITCH_REQ_INTERVAL (3000)
#define DEFAULT_SWITCH_INTERVAL     (1000)
#define DEFAULT_IMAGE_RETRY_COUNT   (8)

static uint8_t string_cmp(uint8_t *str1, uint8_t *str2, uint8_t len);
static void send_image_req(uint32_t index);
static void send_switch_req(void);

uint32_t image_start;
uint32_t image_size;
uint32_t image_end;
uint8_t image_crc;
uint16_t block_size;
uint32_t image_index = 0;

uint16_t image_req_interval_ms = DEFAULT_IMAGE_REQ_INTERVAL;
uint8_t  image_request_start=0;
node_info_t curr_image;

upgrade_state_t curr_upgrade_state = STATE_IDLE;
upgrade_widbg_state_t curr_widbg_state = UPGRADE_OTA_IDLE;

uint16_t block[APP_MAX_PAYLOAD_SIZE / 2];

uint8_t upgrade_confirm_wait = 0;
node_info_t upgrading_node_info;

uint8_t image_req_retry = 0;

void widbg_upgrade_init(void)
{
	curr_upgrade_state = STATE_IDLE;
	curr_widbg_state = UPGRADE_OTA_IDLE;
	widbg_nvm_read(MEMORY_NODE_INFO_ADDRESS, 0, sizeof(node_info_t), (uint8_t *)&curr_image);
	upgrade_confirm_wait = 0;
	image_req_retry = 0;
	image_req_interval_ms = DEFAULT_IMAGE_REQ_INTERVAL;
}

void widbg_upgrade_rcvd_frame(uint8_t addr_mode, uint8_t *src_addr, uint8_t length, uint8_t *payload, uint8_t lqi)
{
	uint8_t msg_code = *payload;
	switch (msg_code)
	{
		case OTA_SERVER_NOTIFY:
		{
			configure_upgrade_server_details(EXTENDED_ADDR_MODE, payload + 1);
			configure_upgrade_server_details(NATIVE_ADDR_MODE, payload + 1 + EXTENDED_ADDR_SIZE);
			break;
		}
		case OTA_IMAGE_NOTIFY:
		{
			if(STATE_IDLE == curr_upgrade_state || STATE_WAITING_FOR_SWITCH == curr_upgrade_state)
			{
				image_notify_request_t *image_notify = (image_notify_request_t *)payload;
				if (string_cmp((uint8_t *)&(image_notify->firmware_version), (uint8_t *)&curr_image.firmware_version, sizeof(curr_image.firmware_version)))
				{
					block_size = APP_MAX_PAYLOAD_SIZE;
					memcpy(&upgrading_node_info.image_start, &image_notify->image_start, sizeof(uint32_t));
					memcpy(&upgrading_node_info.image_size, &image_notify->image_size, sizeof(uint32_t));
					memcpy(&upgrading_node_info.firmware, &image_notify->firmware, sizeof(upgrading_node_info.firmware));
					memcpy(&upgrading_node_info.firmware_version, &image_notify->firmware_version, sizeof(upgrading_node_info.firmware_version));
					memcpy(&upgrading_node_info.board, &image_notify->board, sizeof(upgrading_node_info.board));
					memcpy(&upgrading_node_info.board_version, &image_notify->board_version, sizeof(upgrading_node_info.board_version));

					memcpy(&image_start, &image_notify->image_start, sizeof(uint32_t));
					memcpy(&image_size, &image_notify->image_size, sizeof(uint32_t));
					image_crc = image_notify->image_crc;
					memcpy(&image_req_interval_ms, &image_notify->req_interval, sizeof(image_req_interval_ms));
					if (image_req_interval_ms < DEFAULT_IMAGE_REQ_INTERVAL)
					{
						image_req_interval_ms = DEFAULT_IMAGE_REQ_INTERVAL;
					}

					image_index = image_start;
					image_end = image_start + image_size;
					curr_upgrade_state = STATE_START_DOWNLOAD;
					widbg_mgr_timer_start(UPGRADE, image_req_interval_ms, TIMER_MODE_SINGLE, widbg_upgrade_timer_handler);
				}
			}
			break;
		}
		case OTA_IMAGE_RESPONSE:
		{
			if(STATE_IMAGE_REQUESTED == curr_upgrade_state)
			{
				image_response_t *image_resp = (image_response_t *)payload;
				block_size = APP_MAX_PAYLOAD_SIZE;
				if(image_index == image_resp->block_start)
				{
					image_req_retry = 0;
					memcpy(&block, &image_resp->block, image_resp->block_size);
					printf("\r\n ImageResp 0x%x",image_resp->block_start);
					widbg_nvm_write(MEMORY_OFFSET_ADDRESS, image_resp->block_start - image_start, image_resp->block_size, (uint8_t *)&block);
					image_index += image_resp->block_size;
					if(image_index < image_end)
					{
						widbg_mgr_timer_start(UPGRADE, image_req_interval_ms, TIMER_MODE_SINGLE, widbg_upgrade_timer_handler);
					}
					else
					{
						uint8_t read_block[64];
						uint32_t row_index = 0;
						uint32_t index;
						uint8_t crc = 0;
						while(row_index < image_size)
						{
							widbg_nvm_read(MEMORY_OFFSET_ADDRESS, row_index, 64, read_block);
							for(index = 0; index < 64; index++)
							{
								crc ^= read_block[index];
							}
							if ((row_index + 64) > image_size)
							{
								row_index += image_size - row_index;
							}
							else
							{
								row_index += 64;
							}
						}
						if(crc == image_crc)
						{
							widbg_mgr_timer_stop(UPGRADE);
							send_switch_req();
						}
						else
						{
							image_index = image_start;
							curr_upgrade_state = STATE_START_DOWNLOAD;
							widbg_mgr_timer_start(UPGRADE, image_req_interval_ms, TIMER_MODE_SINGLE, widbg_upgrade_timer_handler);
						}
					}
				}
				else
				{
					//while(1);
				}
			}
			break;
		}
		case OTA_SWITCH_RESPONSE:
		{
			if(STATE_WAITING_FOR_SWITCH == curr_upgrade_state)
			{
				switch_image_response_t *switch_resp = (switch_image_response_t *)payload;
				if (0 == switch_resp->switch_time)
				{
					curr_upgrade_state = STATE_SWITCH_SUCCESS;
					widbg_mgr_timer_start(UPGRADE, DEFAULT_SWITCH_INTERVAL, TIMER_MODE_SINGLE, widbg_upgrade_timer_handler);
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

void widbg_upgrade_sent_frame(uint8_t addr_mode, uint8_t *addr, uint8_t status)
{
	upgrade_confirm_wait = 0;
	if (curr_widbg_state == IMAGE_REQUEST_SENT)
	{
		widbg_mgr_timer_start(UPGRADE, image_req_interval_ms, TIMER_MODE_SINGLE, widbg_upgrade_timer_handler);
	}
	if (SUCCESS != status && curr_widbg_state == SWITCH_REQUEST_SENT)
	{
		widbg_mgr_timer_start(UPGRADE, DEFAULT_SWITCH_REQ_INTERVAL, TIMER_MODE_SINGLE, widbg_upgrade_timer_handler);
	}
	else
	{
		curr_widbg_state = UPGRADE_OTA_IDLE;
	}
}

void widbg_upgrade_timer_handler(void)
{
	port_pin_toggle_output_level(PIN_PA23);
	if (STATE_IMAGE_REQUESTED == curr_upgrade_state || STATE_START_DOWNLOAD == curr_upgrade_state || STATE_IMAGE_RESPONDED == curr_upgrade_state)
	{
		if (upgrade_confirm_wait)
		{
			widbg_mgr_timer_start(UPGRADE, image_req_interval_ms, TIMER_MODE_SINGLE, widbg_upgrade_timer_handler);
		}
		else
		{
			send_image_req(image_index);
		}
	}
	else if(STATE_WAITING_FOR_SWITCH == curr_upgrade_state)
	{
		send_switch_req();
	}
	else if (STATE_SWITCH_SUCCESS == curr_upgrade_state)
	{
		widbg_mgr_timer_stop(UPGRADE);
		curr_upgrade_state = STATE_IDLE;
		upgrading_node_info.supported_mode = 0x03;
		upgrading_node_info.new_image = 1;
		widbg_nvm_write(MEMORY_NODE_INFO_ADDRESS, 0, sizeof(upgrading_node_info), (uint8_t *)&upgrading_node_info);
		widbg_mgr_reset_device();
	}
	else
	{

	}
}

static uint8_t string_cmp(uint8_t *str1, uint8_t *str2, uint8_t len)
{
	uint8_t index;
	for(index = 0; index < len; index++)
	{
		if(*(str1 + index) != *(str2 + index))
		{
			return 1;
		}
	}
	return 0;
}

static void send_image_req(uint32_t index)
{
	image_request_t image_request;
	uint8_t *addr = NULL;
	get_upgrade_server_details(NATIVE_ADDR_MODE, addr);

	image_req_retry++;
	if (DEFAULT_IMAGE_RETRY_COUNT == image_req_retry)
	{
		widbg_mgr_timer_stop(UPGRADE);
		widbg_upgrade_init();
	}
	else
	{
		if ((index + block_size) > image_end)
		{
			block_size = image_end - index;
		}
		else
		{
			block_size = APP_MAX_PAYLOAD_SIZE;
		}
		image_request.msg_id = OTA_IMAGE_REQUEST;
		image_request.req_type = 0x00;
		image_request.addr = index;
		image_request.size = block_size;
		curr_widbg_state = IMAGE_REQUEST_SENT;
		upgrade_confirm_wait = 1;
		curr_upgrade_state = STATE_IMAGE_REQUESTED;
		printf("\r\n ImageRequest 0x%x",index);		
		widbg_mgr_data_req(UPGRADE, NATIVE_ADDR_MODE, addr, &image_request.msg_id, sizeof(image_request_t));
		image_request_start = 1;
		widbg_mgr_timer_start(UPGRADE, DEFAULT_IMAGE_RESP_INTERVAL, TIMER_MODE_SINGLE, widbg_upgrade_timer_handler);
	}
}


static void send_switch_req(void)
{
	uint8_t *addr = NULL;
	get_upgrade_server_details(NATIVE_ADDR_MODE, addr);
	uint8_t temp = OTA_SWITCH_REQUEST;
	curr_widbg_state = SWITCH_REQUEST_SENT;
	upgrade_confirm_wait = 1;
	curr_upgrade_state = STATE_WAITING_FOR_SWITCH;	
	widbg_mgr_data_req(UPGRADE, NATIVE_ADDR_MODE, addr, &temp, 1);

}


