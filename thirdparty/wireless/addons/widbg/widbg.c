/**
 * \file widbg.c
 *
 * \brief WiDBG implementation
 *
 * Copyright (C) 2014, Atmel Corporation. All rights reserved.
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

#include "stddef.h"
#include "stdint.h"
#include "widbg.h"
#include "widbg_mgr.h"
#ifdef WIDBG_SERVER
#include "widbg_parser.h"
#endif
#include "widbg_common.h"
#include "widbg_upgrade.h"
//#include "widbg_debug.h"

void widbg_init(void)
{
	widbg_mgr_init();
#ifdef WIDBG_SERVER
	widbg_parser_init();
#endif
}

void widbg_task(void)
{
	widbg_mgr_task();
}

void handle_widbg_msg(uint16_t length, uint8_t *msg)
{
#ifdef WIDBG_SERVER
	if(DOMAIN_WIDBG_COMMON == *msg)
	{
		handle_common_widbg_msg(length - 1, msg + 1);
	}
	else if(DOMAIN_WIDBG_UPGRADE == *msg)
	{
		handle_upgrade_widbg_msg(length - 1, msg + 1);
	}
#if (WIDBG_DEBUG_SUPPORT == 1)	
	else if(DOMAIN_WIDBG_DEBUG == *msg)
	{
		handle_debug_widbg_msg(length - 1, msg + 1);
	}
#endif
#endif
}

void reverse_memcpy(uint8_t *dst, uint8_t *src, uint8_t len)
{
	uint8_t i;
	for (i=0; i < len; ++i)
	{
		dst[len-1-i] = src[i];
	}
}

