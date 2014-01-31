/**
 * @file sleep_mgr.c
 *
 * @brief 
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
#include "sleep_mgr.h"
#include "wireless_api.h"
#include "sleepmgr.h"
#include "conf_sleepmgr.h"

ISR( rtc_irq_handler, AVR32_RTC_IRQ_GROUP, 3)
{

	// clear the interrupt flag
	rtc_clear_interrupt(&AVR32_RTC);
	// Disable the RTC
	rtc_disable(&AVR32_RTC);
	
}

/**
 * \brief This function Initializes the Sleep functions 
*/
void sm_init(void)
{
	// Set the sleep mode to initially lock.
	enum sleepmgr_mode mode = SLEEPMGR_DEEPSTOP;

	irq_register_handler(rtc_irq_handler, AVR32_RTC_IRQ, 3);

	// Initialize the RTC
	rtc_init(&AVR32_RTC, RTC_OSC_32KHZ, RTC_PSEL_32KHZ_1HZ);
	/* Moving set top value and enable interrupt to sm_sleep*/

	// Initialize the sleep manager, lock initial mode.
	sleepmgr_init();
	sleepmgr_lock_mode(mode);
}

//TODO (Project Wizard) - Call this function to make device and transceiver into sleep
/**
 * \brief This function puts the transceiver and device to sleep
*/
void sm_sleep(uint32_t interval)
{
	// Set top value to 0 to generate an interrupt every second.
	rtc_set_top_value(&AVR32_RTC, (interval-1)); // 0 1sec
	// Enable the interrupts
	rtc_enable_interrupt(&AVR32_RTC);	
	// Enable the RTC
	rtc_enable(&AVR32_RTC);

	sleepmgr_enter_sleep();
}
