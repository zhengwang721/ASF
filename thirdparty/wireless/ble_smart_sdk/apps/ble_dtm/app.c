/**
 * \file
 *
 * \brief DTM Application
 *
 * Copyright (c) 2014-2016 Atmel Corporation. All rights reserved.
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

 /**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the iBeacon Application
 */
/*- Includes ---------------------------------------------------------------*/

#include <asf.h>
#include "at_ble_api.h"
#include "platform.h"

#define APP_STACK_SIZE	(1024)
volatile unsigned char app_stack_patch[APP_STACK_SIZE];


/** Enum for the possible callback types for the timer module. */
enum timer_callback_type
{
	/** Callback for timer expiry*/
	TIMER_EXPIRED_CALLBACK_TYPE_DETECT = 1,
};

#define AON_SLEEP_TIMER_EXPIRY_CALLBACK 0x42

void (* stop_AON_sleep_timer)(void);

int main(void)
{

	uint16_t plf_event_type;
	uint8_t plf_event_data[16];
	uint16_t plf_event_data_len;
	uint32_t sleep_count = 0;
	uint32_t clk_enables;
	at_ble_status_t status = AT_BLE_SUCCESS;
	stop_AON_sleep_timer	= (void (*)(void))(*(unsigned int *)0x10041FCC);
	#if 1
	platform_driver_init();
	clk_enables = *((volatile unsigned int *)0x4000b00c);
	clk_enables |= 0x1E000;
	*((volatile unsigned int *)0x4000b00c) = clk_enables;
	release_sleep_lock();
	while(platform_event_get(&plf_event_type,plf_event_data,&plf_event_data_len))
	{
		//This is  added just not to optimize it out.
		acquire_sleep_lock();
		if(plf_event_type == ((TIMER_EXPIRED_CALLBACK_TYPE_DETECT<< 8)| AON_SLEEP_TIMER_EXPIRY_CALLBACK ))
		{
			sleep_count++;
			if((sleep_count%100) == 0) {
				stop_AON_sleep_timer();
			}
		}
		release_sleep_lock();
	}
	#else
	while(1)
	{
		//WFI();
	}
	#endif
	
	return status;
}
