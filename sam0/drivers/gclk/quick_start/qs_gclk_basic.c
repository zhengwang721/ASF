/**
 * \file
 *
 * \brief SAM0+ Generic Clock Driver Quick Start
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

#include <port.h>

//! [setup]
#define EXAMPLE_GCLOCK_GENERATOR    0
#define EXAMPLE_GCLOCK_CHANNEL      0

void configure_gclock_generator(void)
{
//! [setup_1]
	struct clock_gclk_gen_conf gclock_gen_conf;
//! [setup_1]
//! [setup_2]
	clock_gclk_gen_get_config_defaults(&gclock_gen_conf);
//! [setup_2]

//! [setup_3]
	gclock_gen_conf.source_channel  = 0;
	gclock_gen_conf.division_factor = 128;
//! [setup_3]
//! [setup_4]
	clock_gclk_gen_set_config(EXAMPLE_GCLOCK_GENERATOR, &gclock_gen_conf);
//! [setup_4]

//! [setup_5]
	clock_gclk_gen_enable(EXAMPLE_GCLOCK_GENERATOR);
//! [setup_5]
}

void configure_gclock_channel(void)
{
//! [setup_6]
	struct clock_gclk_ch_conf gclock_ch_conf;
//! [setup_6]
//! [setup_7]
	clock_gclk_ch_get_config_defaults(&gclock_ch_conf);
//! [setup_7]

//! [setup_8]
	gclock_ch_conf.source_generator    = EXAMPLE_GCLOCK_GENERATOR;
	gclock_ch_conf.enable_during_sleep = false;
//! [setup_8]
//! [setup_9]
	clock_gclk_ch_set_config(EXAMPLE_GCLOCK_CHANNEL, &gclock_ch_conf);
//! [setup_9]

//! [setup_10]
	clock_gclk_ch_enable(EXAMPLE_GCLOCK_CHANNEL);
//! [setup_10]
}
//! [setup]

int main(void)
{
	//! [setup_init]
	configure_gclock_generator();
	configure_gclock_channel();
	//! [setup_init]

	//! [main]
	while (true) {
		/* Nothing to do */
	}
	//! [main]
}
