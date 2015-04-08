/**
 * \file server_upgrade.c
 *
 * \brief Server upgrade implementation
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
 
#include "stddef.h"
#include "string.h"
#include "stdint.h"
#include "server_upgrade.h"
#include "widbg_upgrade.h"
#include "widbg_parser.h"

upgrade_widbg_state_t curr_upgrade_widbg_state;

void widbg_upgrade_init(void)
{

}

void widbg_upgrade_rcvd_frame(uint8_t addr_mode, uint8_t *src_addr, uint8_t length, uint8_t *payload, uint8_t lqi)
{
	uint8_t msg_code = *payload;
	switch (msg_code)
	{
		case OTA_SERVER_DISCOVERY:
		{
			send_server_data(DOMAIN_WIDBG_UPGRADE, addr_mode, src_addr, SERVER_DISCOVERY_REQUEST, payload + 1, length - 1);
			break;
		}
		case OTA_QUERY_IMAGE:
		{
			send_server_data(DOMAIN_WIDBG_UPGRADE, addr_mode, src_addr, QUERY_IMAGE_REQUEST, payload + 1, length - 1);
			break;
		}
		case OTA_IMAGE_REQUEST:
		{
			send_server_data(DOMAIN_WIDBG_UPGRADE, addr_mode, src_addr, IMAGE_REQUEST, payload + 1, length - 1);
			break;
		}
		case OTA_SWITCH_REQUEST:
		{
			send_server_data(DOMAIN_WIDBG_UPGRADE, addr_mode, src_addr, SWITCH_IMAGE_REQUEST, payload + 1, length - 1);
			break;
		}
		break;
	}
}

void widbg_upgrade_sent_frame(uint8_t addr_mode, uint8_t *addr, uint8_t status)
{
	switch (curr_upgrade_widbg_state)
	{
		case SERVER_NOTIFY_SENT:
		send_server_data(DOMAIN_WIDBG_UPGRADE, addr_mode, addr, SERVER_NOTIFY_CONFIRM, &status, 1);
		break;
		case IMAGE_NOTIFY_SENT:
		send_server_data(DOMAIN_WIDBG_UPGRADE, addr_mode, addr, IMAGE_NOTIFY_CONFIRM, &status, 1);
		break;
		case IMAGE_RESPONSE_SENT:
		send_server_data(DOMAIN_WIDBG_UPGRADE, addr_mode, addr, IMAGE_CONFIRM, &status, 1);
		break;
		case SWITCH_RESPONSE_SENT:
		send_server_data(DOMAIN_WIDBG_UPGRADE, addr_mode, addr, SWITCH_IMAGE_CONFIRM, &status, 1);
		break;
		default:
		break;
		/*  */
	}
	curr_upgrade_widbg_state = UPGRADE_OTA_IDLE;
}



/**
 * \brief Parses the Received Data in the Buffer and Process the Commands
 *accordingly.
 */
void handle_upgrade_widbg_msg(uint16_t len, uint8_t *msg)
{
//	uint8_t op_mode = *msg;

	/* *msg is the mode specification for phy/app */
	uint8_t msg_id = *(uint8_t *)(msg + 1);
	uint8_t temp;
	msg += 2;

	uint8_t addr_mode = *msg;
	uint8_t *addr = msg + 1;
	if(NATIVE_ADDR_MODE == addr_mode)
	{
		msg += (1 + NATIVE_ADDR_SIZE);
	}
	else if(EXTENDED_ADDR_MODE == addr_mode)
	{
		msg += (1 + EXTENDED_ADDR_SIZE);
	}
	else
	{
		msg++;
	}

	if(UPGRADE_OTA_IDLE != curr_upgrade_widbg_state)
	{
		temp = MSG_IN_PROGRESS;
		send_pc_data(DOMAIN_WIDBG_UPGRADE, UPGRADE_ERROR_MSG, &temp, 1);
	}
	else
	{
		switch (msg_id)
		{ /* message type */
			case SERVER_NOTIFY_INDICATION:
			{
				server_notify_indication_t server_notify;
				server_notify.msg_id = OTA_SERVER_NOTIFY;
				memcpy(&server_notify.ieee_addr, get_node_address(EXTENDED_ADDR_MODE), EXTENDED_ADDR_SIZE);
				memcpy(&server_notify.short_addr, get_node_address(NATIVE_ADDR_MODE), NATIVE_ADDR_SIZE);
				curr_upgrade_widbg_state = SERVER_NOTIFY_SENT;				
				widbg_mgr_data_req(UPGRADE, addr_mode, addr, &server_notify, sizeof(server_notify_indication_t));
				break;
			}
			case IMAGE_NOTIFY_REQUEST:
			{
				image_notify_request_t image_notify;
				image_notify.msg_id = OTA_IMAGE_NOTIFY;
				memcpy(&image_notify.image_type, msg, sizeof(image_notify_request_t) - 1);
				curr_upgrade_widbg_state = IMAGE_NOTIFY_SENT;
				widbg_mgr_data_req(UPGRADE, addr_mode, addr, &image_notify, sizeof(image_notify_request_t));
				break;
			}
			case IMAGE_RESPONSE:
			{
				image_response_t image_response;
				temp = sizeof(image_response.image_resp_type)
						+ sizeof(image_response.block_start) + sizeof(image_response.block_size);
				image_response.msg_id = OTA_IMAGE_RESPONSE;
				memcpy(&image_response.image_resp_type, msg, temp);
				msg += temp;
				memcpy(&image_response.block, msg, image_response.block_size);
				curr_upgrade_widbg_state = IMAGE_RESPONSE_SENT;				
				widbg_mgr_data_req(UPGRADE, addr_mode, addr, &image_response, temp + 1 + image_response.block_size);
				break;
			}
			case SWITCH_IMAGE_RESPONSE:
			{
				switch_image_response_t switch_image_resp;
				switch_image_resp.msg_id = OTA_SWITCH_RESPONSE;
				memcpy(&switch_image_resp.switch_time, msg, sizeof(switch_image_resp.switch_time));
				curr_upgrade_widbg_state = SWITCH_RESPONSE_SENT;				
				widbg_mgr_data_req(UPGRADE, addr_mode, addr, &switch_image_resp, sizeof(switch_image_response_t));
				break;
			}
			default:
			{
				/* Do nothing...*/
			}
		}
	}
}


void widbg_upgrade_timer_handler(void)
{
	//	send_pc_data(DOMAIN_WIDBG_UPGRADE, CLIENT_NOTIFY_INDICATION, &server_notify, sizeof(server_notify_indication_t));
}

