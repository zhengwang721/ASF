/**
 * \file client_debug.h
 *
 * \brief Over-the-air client module implementation
 *
 * Copyright (C) 2015, Atmel Corporation. All rights reserved.
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


#ifndef CLIENT_DEBUG_H
#define CLIENT_DEBUG_H

#include "widbg.h"
#include "samr.h"

#define RAM_END_BUFFER_SIZE         (1024ul)
#define RAM_END_ADDRESS             ((HMCRAMC0_ADDR + HMCRAMC0_SIZE) - RAM_END_BUFFER_SIZE)


#define TRACE_MEMORY_SIZE           (1024ul)
#define TRACE_MEMORY_START_ADDRESS  (RAM_END_ADDRESS - (TRACE_MEMORY_SIZE))

#define LOG_MEMORY_SIZE             (1024ul)
#define LOG_MEMORY_START_ADDRESS    (TRACE_MEMORY_START_ADDRESS - LOG_MEMORY_SIZE)


typedef enum {
	DEBUG_TIMER_IDLE,
	DEBUG_LOG_STATE,
	DEBUG_TRACE_STATE
}debug_timer_state_t;

typedef struct PACK {
	uint8_t sof;
	uint8_t log_len;
	log_type_t log_type;
	module_id_t module_id;
	uint32_t addr;
	error_code_t error_code;
}log_header_t;

typedef struct PACK {
	uint8_t sof;
	uint8_t length;
	uint32_t addr;
	trace_type_t trace_type;
	uint8_t eof;
}trace_t;

typedef struct PACK {
	uint8_t msg_id;
	uint8_t log_content[100];
}log_indication_t;

typedef struct PACK {
	uint8_t msg_id;
	uint8_t trace_end;
	uint8_t trace_content[99];
}trace_indication_t;

#endif /* CLIENT_DEBUG_H */