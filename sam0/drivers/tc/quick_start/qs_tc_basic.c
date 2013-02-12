/**
 * \file
 *
 * \brief SAM0+ Serial Peripheral Interface Driver
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
#include <conf_quick_start.h>
#include <asf.h>

int main(void)
{
	//! [main]

	//! [system_init]
	system_init();
	//! [system_init]
	struct port_conf pin;
	port_get_config_defaults(&pin);
	pin.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_0_PIN, &pin);

	port_pin_set_output_level(LED_0_PIN, 0);

	/* Structures for config and software device instance */
	//! [config]
	struct tc_conf config;
	//! [config]
	//! [dev_inst]
	struct tc_module dev_inst;
	//! [dev_inst]

	//! [tc_get_config_defaults]
	tc_get_config_defaults(&config);
	//! [tc_get_config_defaults]

	//! [setup]
	config.counter_size = TC_COUNTER_SIZE_32BIT;
	config.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
	config.onshot = true;
	config.size_specific.size_32_bit.compare_capture_channel[0] = 6000000;
	//! [setup]

	//! [tc_init]
	tc_init(&dev_inst, TC0, &config);
	//! [tc_init]

	//! [tc_enable]
	tc_enable(&dev_inst);
	//! [tc_enable]

	//! [inf_loop]
	while (1) {
		if(tc_is_interrupt_flag_set(&dev_inst, TC_INTERRUPT_FLAG_OVERFLOW)) {
			port_pin_toggle_output_level(LED_0_PIN);
			tc_clear_interrupt_flag(&dev_inst, TC_INTERRUPT_FLAG_OVERFLOW);
		}
	}
	//! [inf_loop]
	//! [main]
}
