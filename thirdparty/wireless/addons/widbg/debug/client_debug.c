/**
 * \file client_debug.c
 *
 * \brief OTA Client implementation
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
 
#include "stddef.h"
#include "string.h"
#include "stdint.h"
#include "client_debug.h"
#include "widbg_nvm.h"
#include "widbg_mgr_client.h"
#include "widbg_debug.h"


static void write_log_memory(uint8_t len, uint8_t *content);
static uint8_t read_log_memory(uint8_t len, uint8_t *content);

static void write_trace_memory(uint8_t len, uint8_t *content);
static uint8_t read_trace_memory(uint8_t len, uint8_t *content);

uint8_t req_log_addr_mode = NATIVE_ADDR_MODE;
uint8_t log_bitmap = 0x00;
uint8_t trace_enable = 0;

debug_timer_state_t curr_debug_timer_state = DEBUG_TIMER_IDLE;

#define DEBUG_LOG_INTERVAL     (3000)
#define DEBUG_TRACE_INTERVAL   (200)
uint32_t curr_log_read_addr = LOG_MEMORY_START_ADDRESS;
uint32_t curr_log_write_addr = LOG_MEMORY_START_ADDRESS;
uint32_t curr_trace_read_addr = TRACE_MEMORY_START_ADDRESS;
uint32_t curr_trace_write_addr = TRACE_MEMORY_START_ADDRESS;

uint8_t buff[10];

uint8_t status;

uint8_t device_pid1 = 0;
uint8_t device_pid2 = 0;
uint8_t *pid1 = 0x41003FE4;
uint8_t *pid2 = 0x41003FE8;

uint8_t debug_confirm_wait = 0;

void widbg_debug_init(void)
{
	device_pid1 = *pid1;
	device_pid2 = *pid2;

	curr_debug_timer_state = DEBUG_TIMER_IDLE;
	curr_log_read_addr = LOG_MEMORY_START_ADDRESS;
	curr_log_write_addr = LOG_MEMORY_START_ADDRESS;
	curr_trace_read_addr = TRACE_MEMORY_START_ADDRESS;
	curr_trace_write_addr = TRACE_MEMORY_START_ADDRESS;
}

void widbg_debug_rcvd_frame(uint8_t addr_mode, uint8_t *src_addr, uint8_t length, uint8_t *payload, uint8_t lqi)
{
	uint8_t msg_code = *payload;
	switch (msg_code)
	{
		case OTA_READ_MEMORY_REQUEST:
		{
			if(!debug_confirm_wait)
			{
				read_mem_indication_t read_mem_ind;
				read_memory_req_t *read_mem_req = (read_memory_req_t *)payload;
				read_mem_ind.msg_id = OTA_READ_MEMORY_INDICATION;
				read_mem_ind.mem_type = read_mem_req->mem_type;
				read_mem_ind.mem_len = read_mem_req->mem_len;
				memcpy(&read_mem_ind.mem_addr, &read_mem_req->mem_addr, sizeof(uint32_t));

				//if((FLASH_ADDR + FLASH_SIZE) <= (read_mem_req->mem_addr + read_mem_req->mem_len) || (HMCRAMC0_ADDR + HMCRAMC0_SIZE) <= (read_mem_req->mem_addr + read_mem_req->mem_len))
				{
					read_mem_ind.status = SUCCESS;
					memcpy((uint8_t *)&read_mem_ind.mem_addr, (uint8_t *)&read_mem_req->mem_addr, sizeof(uint32_t));
					memcpy(read_mem_ind.mem_content, (uint8_t *)read_mem_req->mem_addr, read_mem_req->mem_len);
				}
				widbg_mgr_data_req(DEBUG, addr_mode, src_addr, &read_mem_ind.msg_id, sizeof(read_mem_indication_t) - (100 - read_mem_req->mem_len));
				debug_confirm_wait = 1;
			}
			break;
		}
		case OTA_WRITE_MEMORY_REQUEST:
		{
			if(!debug_confirm_wait)
			{
				write_mem_indication_t write_mem_ind;
				write_memory_req_t *write_mem_req = (write_memory_req_t *)payload;
				uint32_t addr;
				write_mem_ind.msg_id = OTA_WRITE_MEMORY_INDICATION;
				write_mem_ind.status = SUCCESS;
				write_mem_ind.mem_type = write_mem_req->mem_type;
				write_mem_ind.mem_len = write_mem_req->mem_len;
				memcpy((uint8_t *)&write_mem_ind.mem_addr, (uint8_t *)&write_mem_req->mem_addr, sizeof(uint32_t));
				memcpy((uint8_t *)&addr, (uint8_t *)&write_mem_req->mem_addr, sizeof(uint32_t));
				if(0x2000000 <= addr)
				{
					memcpy((uint8_t *)addr, (uint8_t *)&write_mem_req->mem_content, write_mem_req->mem_len);
				}
				else
				{
					widbg_nvm_write(MEMORY_ABSOLUTE_ADDRESS, addr, write_mem_req->mem_len, (uint8_t *)&write_mem_req->mem_content);
				}
				widbg_mgr_data_req(DEBUG, addr_mode, src_addr, &write_mem_ind.msg_id, sizeof(write_mem_indication_t));
				debug_confirm_wait = 1;
			}
			break;
		}
		case OTA_LOG_REQUEST:
		{
			req_log_addr_mode = addr_mode;
			log_bitmap = *(payload + 1);
			log_bitmap &= LOG_MASK;
			if(log_bitmap)
			{
				curr_debug_timer_state = DEBUG_LOG_STATE;
				widbg_mgr_timer_start(DEBUG, DEBUG_LOG_INTERVAL, TIMER_MODE_PERIODIC, widbg_debug_timer_handler);
			}
			break;
		}
		case OTA_TRACE_REQUEST:
		{
			//trace_enable = 1;
			curr_debug_timer_state = DEBUG_TRACE_STATE;
			widbg_mgr_timer_start(DEBUG, DEBUG_TRACE_INTERVAL, TIMER_MODE_PERIODIC, widbg_debug_timer_handler);
			break;
		}
		default:
		{
			break;
		}
	}
}

void widbg_debug_sent_frame(uint8_t addr_mode, uint8_t *addr, uint8_t status)
{
	debug_confirm_wait = 0;
}

void widbg_debug_timer_handler(void)
{
	switch (curr_debug_timer_state)
	{
		case DEBUG_LOG_STATE:
		{
			uint8_t temp_len;
			uint8_t *dst_addr = NULL;
			log_indication_t log_ind;
			log_ind.msg_id = OTA_LOG_INDICATION;
			temp_len = read_log_memory(100, (uint8_t *)&log_ind.log_content);
			if(temp_len && !debug_confirm_wait)
			{
				get_debug_server_details(NATIVE_ADDR_MODE, dst_addr);
				widbg_mgr_data_req(DEBUG, NATIVE_ADDR_MODE, dst_addr, &log_ind.msg_id, temp_len + 1);
				debug_confirm_wait = 1;
			}
			break;
		}
		case DEBUG_TRACE_STATE:
		{
			uint8_t temp_len;
			uint8_t *dst_addr = NULL;
			trace_indication_t trace_ind;
			trace_ind.msg_id = OTA_TRACE_INDICATION;
			temp_len = read_trace_memory(99, (uint8_t *)&trace_ind.trace_content);
			if (temp_len < 99)
			{
				trace_ind.trace_end = 1;
				if(log_bitmap)
				{
					curr_debug_timer_state = DEBUG_LOG_STATE;
					widbg_mgr_timer_start(DEBUG, DEBUG_LOG_INTERVAL, TIMER_MODE_PERIODIC, widbg_debug_timer_handler);
				}
				else
				{
					widbg_mgr_timer_stop(DEBUG);
				}
			}
			else
			{
				trace_ind.trace_end = 0;
			}
			if(temp_len && !debug_confirm_wait)
			{
				get_debug_server_details(NATIVE_ADDR_MODE, dst_addr);
				widbg_mgr_data_req(DEBUG, NATIVE_ADDR_MODE, dst_addr, &trace_ind.msg_id, temp_len + 2);
				debug_confirm_wait = 1;
			}
			break;
		}
		default:
		break;
	}
}

register int counter asm("lr");

void widbg_log(uint8_t log_type, module_id_t module_id, error_code_t error_code, uint8_t len, uint8_t* user_log)
{
	if(log_type & log_bitmap)
	{
		uint32_t temp = counter;
		uint8_t eof = 0xFF;
		log_header_t log_header;
		log_header.sof = 0xF0;
		log_header.log_len = 7 + len;
		log_header.log_type = log_type;
		log_header.module_id = module_id;
		memcpy(&log_header.addr, &temp, 4);
		log_header.error_code = error_code;
		write_log_memory(sizeof(log_header_t), (uint8_t *)&log_header);
		if (0 != len && NULL != user_log)
		{
			write_log_memory(len, user_log);
		} 
		write_log_memory(1, &eof);
	}
}

void widbg_trace(trace_type_t trace_type)
{
	//if(trace_enable)
	//{
		trace_t trace;
		uint32_t temp = counter;
		trace.sof = 0xF0;
		trace.length = 0x05;
		trace.addr = temp;
		trace.trace_type = trace_type;
		trace.eof = 0xFF;
		write_trace_memory(sizeof(trace_t), (uint8_t *)&trace);
	//}
}

static void write_log_memory(uint8_t len, uint8_t *content)
{
	if((LOG_MEMORY_START_ADDRESS + LOG_MEMORY_SIZE) > (curr_log_write_addr + len))
	{
		memcpy((uint8_t *)curr_log_write_addr, (uint8_t *)content, len);
		curr_log_write_addr += len;
	}
	else
	{
		uint8_t temp_len = (LOG_MEMORY_START_ADDRESS + LOG_MEMORY_SIZE) - curr_log_write_addr;
		memcpy((uint8_t *)curr_log_write_addr, (uint8_t *)content, temp_len);
		curr_log_write_addr = LOG_MEMORY_START_ADDRESS;
		content = content + temp_len;
		len = len - temp_len;
		memcpy((uint8_t *)curr_log_write_addr, (uint8_t *)content, len);
	}
}

static uint8_t read_log_memory(uint8_t len, uint8_t *content)
{
	uint8_t temp_len;
	if(curr_log_read_addr < curr_log_write_addr)
	{
		temp_len = curr_log_write_addr - curr_log_read_addr;
		if (temp_len > len)
		{
			temp_len = len;
		}
		memcpy((uint8_t *)content, (uint8_t *)curr_log_read_addr, temp_len);
	}
	else if (curr_log_write_addr < curr_log_read_addr)
	{
		temp_len = (LOG_MEMORY_START_ADDRESS + LOG_MEMORY_SIZE) - curr_log_read_addr;
		if (temp_len > len)
		{
			temp_len = len;
			memcpy((uint8_t *)content, (uint8_t *)curr_log_read_addr, temp_len);
		}
		else
		{
			memcpy((uint8_t *)content, (uint8_t *)curr_log_read_addr, temp_len);
			curr_log_read_addr = LOG_MEMORY_START_ADDRESS;
			if(curr_log_read_addr != curr_log_write_addr)
			{
				uint8_t temp1_len;
				temp1_len = curr_log_write_addr - curr_log_read_addr;
				if ((temp_len + temp1_len) > len)
				{
					temp1_len = len - temp_len;
				}
				memcpy((uint8_t *)(content+ temp_len), (uint8_t *)curr_log_read_addr, temp1_len);
				temp_len += temp1_len;
			}
		}
	}
	else
	{
		temp_len = 0;
	}
	curr_log_read_addr += temp_len;
	return temp_len;
}

static void write_trace_memory(uint8_t len, uint8_t *content)
{
	if((TRACE_MEMORY_START_ADDRESS + TRACE_MEMORY_SIZE) > (curr_trace_write_addr + len))
	{
		memcpy((uint8_t *)curr_trace_write_addr, (uint8_t *)content, len);
		curr_trace_write_addr += len;
	}
	else
	{
		uint8_t temp_len = (TRACE_MEMORY_START_ADDRESS + TRACE_MEMORY_SIZE) - curr_trace_write_addr;
		memcpy((uint8_t *)curr_trace_write_addr, (uint8_t *)content, temp_len);
		curr_trace_write_addr = TRACE_MEMORY_START_ADDRESS;
		content = content + temp_len;
		len = len - temp_len;
		memcpy((uint8_t *)curr_trace_write_addr, (uint8_t *)content, len);
	}
}

static uint8_t read_trace_memory(uint8_t len, uint8_t *content)
{
	uint8_t temp_len;
	if(curr_trace_read_addr < curr_trace_write_addr)
	{
		temp_len = curr_trace_write_addr - curr_trace_read_addr;
		if (temp_len > len)
		{
			temp_len = len;
		}
		memcpy((uint8_t *)content, (uint8_t *)curr_trace_read_addr, temp_len);
	}
	else if (curr_trace_write_addr < curr_trace_read_addr)
	{
		temp_len = (TRACE_MEMORY_START_ADDRESS + TRACE_MEMORY_SIZE) - curr_trace_read_addr;
		if (temp_len > len)
		{
			temp_len = len;
			memcpy((uint8_t *)content, (uint8_t *)curr_trace_read_addr, temp_len);
		}
		else
		{
			memcpy((uint8_t *)content, (uint8_t *)curr_trace_read_addr, temp_len);
			curr_trace_read_addr = TRACE_MEMORY_START_ADDRESS;
			if(curr_trace_read_addr != curr_trace_write_addr)
			{
				uint8_t temp1_len;
				temp1_len = curr_trace_write_addr - curr_trace_read_addr;
				if ((temp_len + temp1_len) > len)
				{
					temp1_len = len - temp_len;
				}
				memcpy((uint8_t *)(content+ temp_len), (uint8_t *)curr_trace_read_addr, temp1_len);
				temp_len += temp1_len;
			}
		}
	}
	else
	{
		temp_len = 0;
	}
	curr_trace_read_addr += temp_len;
	return temp_len;
}
