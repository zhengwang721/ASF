/**
 * \file
 *
 * \brief SAMD20 GPIO Port Driver Quick Start
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

void configure_extosc32k(void);
void configure_dfll_open_loop(void);

volatile uint32_t frequency;

void configure_extosc32k(void)
{
	struct system_clock_source_xosc32k_config ext32k_conf;
	system_clock_source_xosc32k_get_default_config(&ext32k_conf);
	ext32k_conf.startup_time = SYSTEM_XOSC32K_STARTUP_4096;
	system_clock_source_xosc32k_set_config(&ext32k_conf);
}

void configure_dfll_open_loop(void)
{
	struct system_clock_source_dfll_config dfll_conf;
	system_clock_source_dfll_get_default_config(&dfll_conf);
	system_clock_source_dfll_set_config(&dfll_conf);
}

int main(void)
{
	enum status_code retval;


	/* Configure the external 32K oscillator */	
	configure_extosc32k();

	/* Enable the external 32k oscillator */
	retval = system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC32K, false);

	if (retval != STATUS_OK) {
		/* Error enabling the clock source */
	}

	/* Configure the DFLL in open loop mode using default values */
	configure_dfll_open_loop();

	/* Change system clock to DFLL */
	system_main_clock_set_source(SYSTEM_CLOCK_SOURCE_DFLL);


	while (true) {

	}

}
