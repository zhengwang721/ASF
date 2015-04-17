/**
 * \file server_common.c
 *
 * \brief Server common implementation
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
#include "server_common.h"
#include "ota_common.h"
#include "ota_parser.h"

common_ota_state_t curr_common_ota_state;

server_info_resp_t server_info = {
	.ieee_addr_mode = EXTENDED_ADDR_MODE,
	.short_addr_mode = NATIVE_ADDR_MODE,
	.max_app_payload = APP_MAX_PAYLOAD_SIZE,
	.max_phy_payload = PHY_MAX_PAYLOAD_SIZE,
	.supported_modes = SUPPORTED_MODE,
	.firmware = "SC6LoWPAN ",
	.firmware_version = "1.0.0",
	.board = "SAMR21XPro",
	.board_version = "1.0.0"
};

void ota_common_init(void)
{
	curr_common_ota_state = COMMON_OTA_IDLE;
	memcpy(&server_info.ieee_addr, get_node_address(EXTENDED_ADDR_MODE),EXTENDED_ADDR_SIZE);
	memcpy(&server_info.pan_id, get_pan_id(), 2);
	memcpy(&server_info.short_addr, get_node_address(NATIVE_ADDR_MODE), NATIVE_ADDR_SIZE);
}

void ota_common_rcvd_frame(uint8_t addr_mode, uint8_t *src_addr, uint8_t length, const uint8_t *payload, uint8_t lqi)
{
	uint8_t msg_code = *payload;
	switch (msg_code)
	{
	case OTA_CLIENT_NOTIFY:
	{
		client_notify_indication_t *client_notify = (client_notify_indication_t *)payload;
		client_notify->lqi = lqi;
		send_server_data(DOMAIN_OTA_COMMON, addr_mode, src_addr, CLIENT_NOTIFY_INDICATION, payload + 1, length - 1);
		break;
	}
	case OTA_CLIENT_INFO:
		send_server_data(DOMAIN_OTA_COMMON, addr_mode, src_addr, CLIENT_INFO_INDICATION, payload + 1, length - 1);
		break;
	}
}

void ota_common_sent_frame(uint8_t addr_mode, uint8_t *addr, uint8_t status)
{
	switch (curr_common_ota_state)
	{
		case CLIENT_DISCOVERY_REQUEST_SENT:
			send_pc_data(DOMAIN_OTA_COMMON, CLIENT_DISCOVERY_CONFIRM, &status, 1);
			break;
		case CLIENT_INFO_REQUEST_SENT:
			send_server_data(DOMAIN_OTA_COMMON, addr_mode, addr, CLIENT_INFO_CONFIRM, &status, 1);
			break;
		case CLIENT_IDENTIFY_REQUEST_SENT:
			send_server_data(DOMAIN_OTA_COMMON, addr_mode, addr, CLIENT_IDENTIFY_CONFIRM, &status, 1);
			break;
		case CLIENT_RESET_REQUEST_SENT:
			send_server_data(DOMAIN_OTA_COMMON, addr_mode, addr, CLIENT_RESET_CONFIRM, &status, 1);
			break;
		default:
			break;
			/*  */
	}
	curr_common_ota_state = COMMON_OTA_IDLE;
}


/**
 * \brief Parses the Received Data in the Buffer and Process the Commands
 *accordingly.
 */
void handle_common_ota_msg(uint16_t len, uint8_t *msg)
{
	/* *msg is the mode specification for phy/app */
	uint8_t msg_id = *(uint8_t *)(msg + 1);
	uint8_t temp;
	msg += 2;

	uint8_t addr_mode = *msg;
	uint8_t *addr = msg + 1;

	if (!(msg_id & 0x80))
	{
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
			addr = NULL;
			msg++;
		}
	}
	if(COMMON_OTA_IDLE != curr_common_ota_state)
	{
		temp = MSG_IN_PROGRESS;
		send_pc_data(DOMAIN_OTA_COMMON, COMMON_ERROR_MSG, &temp, 1);
	}
	else
	{
		switch (msg_id)
		{ /* message type */
			case SERVER_INFO_REQUEST:
			{
				uint8_t channel = (uint8_t) *msg;
				ota_mgr_set_channel(channel);
				send_pc_data(DOMAIN_OTA_COMMON, SERVER_INFO_RESPONSE, (uint8_t *)&server_info, sizeof(server_info_resp_t));
				break;
			}
			case CLIENT_DISCOVERY_REQUEST:
			{
				client_discovery_req_t client_disc_req;
				client_disc_req.msg_id = OTA_CLIENT_DISCOVERY;
				memcpy((uint8_t *)&client_disc_req.interval, msg, sizeof(client_disc_req.interval));
				curr_common_ota_state = CLIENT_DISCOVERY_REQUEST_SENT;
				ota_mgr_data_req(COMMON, addr_mode, addr, &client_disc_req.msg_id, sizeof(client_discovery_req_t));
				break;
			}
			case CLIENT_INFO_REQUEST:
			{
				temp = OTA_CLIENT_INFO_REQ;
				curr_common_ota_state = CLIENT_INFO_REQUEST_SENT;
				ota_mgr_data_req(COMMON, addr_mode, addr, &temp, 1);
				break;
			}
			case CLIENT_IDENTIFY_REQUEST:
			{
				temp = OTA_IDENTIFY_REQ;
				curr_common_ota_state = CLIENT_IDENTIFY_REQUEST_SENT;
				ota_mgr_data_req(COMMON, addr_mode, addr, &temp, 1);
				break;
			}
			case CLIENT_RESET_REQUEST:
			{
				temp = OTA_RESET_REQ;
				curr_common_ota_state = CLIENT_RESET_REQUEST_SENT;
				ota_mgr_data_req(COMMON, addr_mode, addr, &temp, 1);
				break;
			}
			default:
			{
				/* Do nothing...*/
			}
		}
	}
}

void ota_common_timer_handler(void)
{

}

