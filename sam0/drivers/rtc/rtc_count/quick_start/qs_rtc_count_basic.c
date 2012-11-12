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
#include <asf.h>

void config_rtc_count(void);

//! [initiate]
void config_rtc_count(void)
{
	/* Initialize RTC in 16 bit count mode. */
//! [set_conf]
	struct rtc_count_conf config;
//! [set_conf]

//! [get_default]
	rtc_count_get_config_defaults(&config);
//! [get_default]

//! [set_config]
	config.mode = RTC_COUNT_MODE_16BIT;
	config.continuously_update = true;
	config.compare_values[0] = 2000;
//! [set_config]

//! [init_rtc]
	rtc_count_init(&config);
//! [init_rtc]

//! [enable]
	rtc_count_enable();
//! [enable]
}
//! [initiate]

int main(void)
{
	/* Initialize system. Must configure conf_clocks.h first. */
	//system_init();
//! [add_main]
	config_rtc_count();

	rtc_count_set_count(1000);
//! [add_main]

//! [implementation_code]
	while(1) {
//! [poll]
		if (rtc_count_is_compare_match(RTC_COUNT_COMPARE_0)) {
			/* Do something. */
		}
//! [poll]
	}
//! [implementation_code]
	return 0;
}
