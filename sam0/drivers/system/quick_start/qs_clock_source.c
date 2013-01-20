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
#include "debug.h"

#define GCLK_GEN 6
#define GCLK_DFLL_GEN 5
#define DFLL_GCLK_CH 0
#define GCLKIO_PIN 26
#define GCLKIO_PIN_MUX MUX_PA26G_GCLK_IO6

#define OUTPUT_DFLL_OPEN
//#define OUTPUT_XOSC32K

//Sysctrl *sysctrl_dbg = (Sysctrl  *)0x40000800U;


void configure_system_clock_sources(void);
void configure_extosc32k(void);
void configure_gclk_generator(void);
void configure_dfll_open_loop(void);



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

void configure_dfll_closed_loop(void)
{
	struct system_gclk_ch_conf gclock_ch_conf;

	system_gclk_ch_get_config_defaults(&gclock_ch_conf);
	gclock_ch_conf.source_generator = GCLK_GEN;


}


void configure_gclk_generator(void)
{
	struct system_gclk_gen_conf gclock_gen_conf;
	struct port_pin_conf pin_conf;
	
	pin_conf.input.enabled  = false;
	pin_conf.output.enabled = true;

	pin_conf.type = PORT_PIN_TYPE_PERIPHERAL;
	pin_conf.peripheral_index = GCLKIO_PIN_MUX;

	port_pin_set_config(GCLKIO_PIN, &pin_conf);

	system_gclk_gen_get_config_defaults(&gclock_gen_conf);

#ifdef OUTPUT_XOSC32K
	gclock_gen_conf.source_clock    = SYSTEM_CLOCK_SOURCE_XOSC32K;
	gclock_gen_conf.division_factor = 10;
	gclock_gen_conf.output_enable   = true;
#endif
#ifdef OUTPUT_DFLL_OPEN

	gclock_gen_conf.source_clock    = SYSTEM_CLOCK_SOURCE_DFLL;
	gclock_gen_conf.division_factor = 2;
	gclock_gen_conf.output_enable   = true;

#endif
	system_gclk_gen_set_config(GCLK_GEN, &gclock_gen_conf);

	system_gclk_gen_enable(GCLK_GEN);

	
}
	
void configure_gclk_channel(void)
{
	struct system_gclk_ch_conf gclock_ch_conf;

	system_gclk_ch_get_config_defaults(&gclock_ch_conf);
	gclock_ch_conf.source_generator = GCLK_GEN;


}

int main(void)
{
	enum status_code retval;
	volatile uint32_t temp = 0;
        volatile uint32_t bob;
	
	init_debug_pins();
	

	


#ifdef OUTPUT_XOSC32K
	configure_extosc32k();
	debug_set_val(0x1);
	retval = system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC32K, false);
	debug_set_val(0x2);
	if (retval != STATUS_OK) {
		debug_set_val(0xa);	
	} else {
		debug_set_val(0xf);
		configure_gclk_generator();
		temp = system_gclk_gen_get_hz(GCLK_GEN);
		bob = temp;
		debug_set_val(temp);
                
	}
#endif
#ifdef OUTPUT_DFLL_OPEN
	configure_dfll_open_loop();
	//system_clock_source_enable(SYSTEM_CLOCK_SOURCE_DFLL, true);
	debug_set_val(0xb);
	configure_gclk_generator();
	//debug_set_val(0xf);
#endif





	while(true);


}
