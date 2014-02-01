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
#include "conf_sleepmgr.h"
#include "rtc_count.h"
#include "system.h"
#include "rtc_count_interrupt.h"
#include "samd20_xplained_pro.h"
#include "port.h"
/**
 * @brief Configuring RTC Callback Funtion on Overflow
 *
 * @param void
 */
void configure_rtc_callbacks(void);

/**
 * @brief Callback Function indicating RTC Overflow
 *
 * @param void
 */
void rtc_overflow_callback(void);

/* Residual time returned to MAC for Synchronization after wakeup,When Switched to Low precision clock during sleep*/
static uint32_t res;
/* This variable denotes the sleep duration returned from MAC*/
uint32_t sleep_time =0;
/* Sleep State of the Application and MAC*/
bool sys_sleep = false;

/**
 * \brief This function Initializes the Sleep functions 
*/
void sm_init(void)
{
	struct rtc_count_config config_rtc_count;
	rtc_count_get_config_defaults(&config_rtc_count);
	config_rtc_count.prescaler           = RTC_COUNT_PRESCALER_DIV_1;
	config_rtc_count.mode                = RTC_COUNT_MODE_16BIT;
	/** Continuously update the counter value so no synchronization is
	 *  needed for reading. */
	config_rtc_count.continuously_update = true;
	rtc_count_init(&config_rtc_count);	
	configure_rtc_callbacks();
}

/**
 * \brief This function puts the transceiver and device to sleep
*/
void sm_sleep(unsigned int interval)
{

   /* Set timeout period for rtc*/
	//res = interval % 1000;
	interval = interval*1000;	
	rtc_count_set_period(interval);
	rtc_count_enable();
	/*put the MCU in standby mode with RTC as wakeup source*/
	system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
	system_sleep();
}

void configure_rtc_callbacks(void)
{   
	/*Register rtc callback*/
	rtc_count_register_callback(
	rtc_overflow_callback, RTC_COUNT_CALLBACK_OVERFLOW);
	rtc_count_enable_callback(RTC_COUNT_CALLBACK_OVERFLOW);
}
void rtc_overflow_callback(void)
{
	/* Do something on RTC overflow here */
	rtc_count_disable();
	
}
