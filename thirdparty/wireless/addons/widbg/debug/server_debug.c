/**
 * \file server_debug.c
 *
 * \brief Server debug implementation
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
 
#include "stddef.h"
#include "string.h"
#include "stdint.h"
#include "server_debug.h"
#include "widbg_debug.h"
#include "widbg_parser.h"

debug_widbg_state_t curr_debug_widbg_state;

void widbg_debug_init(void)
{

}

void widbg_debug_rcvd_frame(uint8_t addr_mode, uint8_t *src_addr, uint8_t length, uint8_t *payload, uint8_t lqi)
{
	uint8_t msg_code = *payload;
	switch (msg_code)
	{
		case OTA_READ_MEMORY_INDICATION:
		{
			send_server_data(DOMAIN_WIDBG_DEBUG, addr_mode, src_addr, READ_MEMORY_INDICATION, payload + 1, length - 1);
			break;
		}
		case OTA_WRITE_MEMORY_INDICATION:
		{
			send_server_data(DOMAIN_WIDBG_DEBUG, addr_mode, src_addr, WRITE_MEMORY_INDICATION, payload + 1, length - 1);
			break;
		}
		case OTA_LOG_INDICATION:
		{
			send_server_data(DOMAIN_WIDBG_DEBUG, addr_mode, src_addr, LOG_INDICATION, payload + 1, length - 1);
			break;
		}
		case OTA_TRACE_INDICATION:
		{
			send_server_data(DOMAIN_WIDBG_DEBUG, addr_mode, src_addr, TRACE_INDICATION, payload + 1, length - 1);
			break;
		}
	}
}

void widbg_debug_sent_frame(uint8_t addr_mode, uint8_t *addr, uint8_t status)
{
	switch (curr_debug_widbg_state)
	{
		case READ_MEMORY_REQUEST_SENT:
		send_server_data(DOMAIN_WIDBG_DEBUG, addr_mode, addr, READ_MEMORY_CONFIRM, &status, 1);
		break;
		case WRITE_MEMORY_REQUEST_SENT:
		send_server_data(DOMAIN_WIDBG_DEBUG, addr_mode, addr, WRITE_MEMORY_CONFIRM, &status, 1);
		break;
		case LOG_REQUEST_SENT:
		send_server_data(DOMAIN_WIDBG_DEBUG, addr_mode, addr, LOG_CONFIRM, &status, 1);
		break;
		case TRACE_REQUEST_SENT:
		send_server_data(DOMAIN_WIDBG_DEBUG, addr_mode, addr, TRACE_CONFIRM, &status, 1);
		break;
		default:
		break;
		/*  */
	}
	curr_debug_widbg_state = DEBUG_OTA_IDLE;
}

/**
 * \brief Parses the Received Data in the Buffer and Process the Commands
 *accordingly.
 */
void handle_debug_widbg_msg(uint16_t len, uint8_t *msg)
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
		addr = NULL;
		msg++;
	}

	if(DEBUG_OTA_IDLE != curr_debug_widbg_state)
	{
		temp = MSG_IN_PROGRESS;
		send_pc_data(DOMAIN_WIDBG_DEBUG, DEBUG_ERROR_MSG, &temp, 1);
	}
	else
	{
		switch (msg_id)
		{ /* message type */
			case READ_MEMORY_REQUEST:
			{
				read_memory_req_t read_mem_req;
				read_mem_req.msg_id = OTA_READ_MEMORY_REQUEST;
				memcpy(&read_mem_req.mem_type, msg, sizeof(read_memory_req_t) - 1);
				widbg_mgr_data_req(DEBUG, addr_mode, addr, &read_mem_req, sizeof(read_memory_req_t));
				curr_debug_widbg_state = READ_MEMORY_REQUEST_SENT;
				break;
			}
			case WRITE_MEMORY_REQUEST:
			{
				write_memory_req_t write_mem_req;
				write_mem_req.msg_id = OTA_WRITE_MEMORY_REQUEST;
				memcpy(&write_mem_req.mem_type, msg, sizeof(write_memory_req_t) - 1);
				widbg_mgr_data_req(DEBUG, addr_mode, addr, &write_mem_req, sizeof(write_memory_req_t) - (100 - write_mem_req.mem_len));
				curr_debug_widbg_state = WRITE_MEMORY_REQUEST_SENT;
				break;
			}
			case LOG_REQUEST:
			{
				log_request_t log_req;
				log_req.msg_id = OTA_LOG_REQUEST;
				log_req.log_bitmap = *msg;
				widbg_mgr_data_req(DEBUG, addr_mode, addr, &log_req, sizeof(log_request_t));
				curr_debug_widbg_state = LOG_REQUEST_SENT;
				break;
			}
			case TRACE_REQUEST:
			{
				uint8_t trace_req;
				trace_req = OTA_TRACE_REQUEST;
				widbg_mgr_data_req(DEBUG, addr_mode, addr, &trace_req, sizeof(trace_req));
				curr_debug_widbg_state = TRACE_REQUEST_SENT;
				break;
			}
			default:
			{
				/* Do nothing...*/
			}
		}
	}
}


void widbg_debug_timer_handler(void)
{

}


void widbg_log(uint8_t log_type, module_id_t module_id, error_code_t error_code, uint8_t len, uint8_t* user_log)
{
	uint32_t temp = 10;
	temp++;
	temp--;
}

void widbg_trace(trace_type_t trace_type)
{


}