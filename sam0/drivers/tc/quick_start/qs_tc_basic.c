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

static void configure_led(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

	pin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_0_PIN, &pin_conf);
}


int main(void)
{
	//! [main]

	//! [system_init]
	system_init();
	//! [system_init]


	configure_led();
	port_pin_set_output_level(LED_0_PIN, false);
	/* Structures for config and software device instance */
	//! [config]
	struct tc_config config;
	//! [config]
	//! [dev_inst]
	struct tc_module module_inst;
	//! [dev_inst]

	//! [tc_get_config_defaults]
	tc_get_config_defaults(&config);
	//! [tc_get_config_defaults]

	//! [pwm_channel_0]
	config.channel_pwm_out_enabled[0] = true;
	config.channel_pwm_out_pin[0] = PWM_OUT_PIN;
	config.channel_pwm_out_mux[0] = PWM_OUT_PIN_MUX;
	//! [pwm_channel_0]

	//! [setup]
	config.counter_size = TC_COUNTER_SIZE_16BIT;
	config.wave_generation = TC_WAVE_GENERATION_NORMAL_FREQ;
	config.size_specific.size_16_bit.compare_capture_channel[0] = 0x7FFF;
	//! [setup]

	//! [tc_init]
	tc_init(&module_inst, PWM_MODULE, &config);
	//! [tc_init]

	//! [tc_enable]
	tc_enable(&module_inst);
	//! [tc_enable]

	

	//! [inf_loop]
	while (1) {
	}
	//! [inf_loop]
	//! [main]
}
