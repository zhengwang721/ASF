/**
 * \file
 *
 * \brief SAM0+ GPIO Port Driver Quick Start
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
 */
#include <asf.h>

void configure_clock_sources(void);

void configure_clock_sources(void)
{
	struct clock_source_config cs_conf;
	clock_source_get_default_config(&cs_conf);

	/* 8MHz RC oscillator */
	cs_conf.rc8mhz.prescaler = 4;

	clock_source_set_config(&cs_conf, CLOCK_SOURCE_RC8MHZ);

	/* XOSC */
	cs_conf.ext.external_clock = CLOCK_EXTERNAL_CRYSTAL;

	clock_source_set_config(&cs_conf, CLOCK_SOURCE_XOSC);

	/* DFLL */
	cs_conf.dfll.coarse_value = 42;
	cs_conf.dfll.fine_value = 42;

	clock_source_set_config(&cs_conf, CLOCK_SOURCE_DFLL);
}

int main(void)
{
	/** [setup_init] */
	configure_clock_sources();
	/** [setup_init] */


	/** [main1] */
	clock_main_set_source(CLOCK_MAIN_DFLL);
	/** [main1] */

}
