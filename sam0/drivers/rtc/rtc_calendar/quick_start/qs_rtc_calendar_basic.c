/**
 * \file
 *
 * \brief SAM0+ RTC Basic Usage Example
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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
 */

#define USE_FRAMEWORK

#if defined(USE_FRAMEWORK)
# include "../rtc_calendar.h"

#endif

void init_rtc_calendar(void)
{
	/* Initialize RTC in calendar mode. */
	struct rtc_calendar_conf config;

	rtc_calendar_get_config_defaults(&config);

	config.clock_24h = true;

	rtc_calendar_init(&config);

	rtc_calendar_enable();
}

int main(void)
{
	struct rtc_calendar_time time;
	time.year 	= 2012;
	time.month 	= 12;
	time.day 	= 12;
	time.hour 	= 13;
	time.minute 	= 37;

	//system_init();

	init_rtc_calendar();

	rtc_calendar_set_time(&time);

	rtc_calendar_swap_time_mode();

	while(1){
		/* Inf loop. */
	}
	return 0;
}
