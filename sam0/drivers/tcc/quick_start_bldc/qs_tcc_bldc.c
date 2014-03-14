/**
 * \file
 *
 * \brief SAM D21 TCC - Timer Counter for Control Applications Callback Driver Quick Start
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
 */

#include <asf.h>
#include <conf_quick_start_bldc.h>

//! [setup_config_values]
#define PERIOD_VALUE 4800
#define MATCH_1      (PERIOD_VALUE*1/3)
#define MATCH_2      (PERIOD_VALUE*2/3)
//! [setup_config_values]

static void configure_tcc(void);
static void configure_tcc_callbacks(void);
static void tcc_callback_to_change_pattern(
		struct tcc_module *const module_inst);

//! [module_inst]
struct tcc_module tcc_instance;
//! [module_inst]

//! [callback_funcs]
static void tcc_callback_to_change_pattern(
		struct tcc_module *const module_inst)
{
	const enum tcc_output_pattern patterns[2] = {
		TCC_OUTPUT_PATTERN_DISABLE,
		TCC_OUTPUT_PATTERN_0
	};
	tcc_force_double_buffer_update(module_inst);

	static uint8_t patt_sel_0 = 1, patt_sel_1 = 0;
	tcc_set_pattern(module_inst, CONF_PWM_OUTPUT0, patterns[patt_sel_0]);
	tcc_set_pattern(module_inst, CONF_PWM_OUTPUT1, patterns[patt_sel_1]);
	tcc_set_pattern(module_inst, CONF_PWM_OUTPUT2, patterns[patt_sel_1]);
	tcc_set_pattern(module_inst, CONF_PWM_OUTPUT3, patterns[patt_sel_0]);
	patt_sel_0 = !patt_sel_0;
	patt_sel_1 = !patt_sel_1;
}
//! [callback_funcs]

//! [setup]
static void configure_tcc(void)
{
	//! [setup_config]
	struct tcc_config config_tcc;
	//! [setup_config]
	//! [setup_config_defaults]
	tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_tcc.counter.direction = TCC_COUNT_DIRECTION_UP;
	config_tcc.double_buffering  = true;
	config_tcc.counter.period = PERIOD_VALUE - 1;
	config_tcc.wave.wave_generation = TCC_WAVE_GENERATION_DOUBLE_SLOPE_BOTH;

	config_tcc.wave.match[CONF_PWM_CHANNEL0] = MATCH_1 - 1;
	config_tcc.wave.wave_polarity[CONF_PWM_CHANNEL0] = TCC_WAVE_POLARITY_0;

	config_tcc.wave.match[CONF_PWM_CHANNEL1] = MATCH_2 - 1;
	config_tcc.wave.wave_polarity[CONF_PWM_CHANNEL1] = TCC_WAVE_POLARITY_1;

	config_tcc.wave.match[CONF_PWM_CHANNEL2] = MATCH_1 - 1;
	config_tcc.wave.wave_polarity[CONF_PWM_CHANNEL2] = TCC_WAVE_POLARITY_0;

	config_tcc.wave.match[CONF_PWM_CHANNEL3] = MATCH_2 - 1;
	config_tcc.wave.wave_polarity[CONF_PWM_CHANNEL3] = TCC_WAVE_POLARITY_1;

	config_tcc.wave_ext.invert[CONF_PWM_OUTPUT0] = false;
	config_tcc.wave_ext.invert[CONF_PWM_OUTPUT1] = false;
	config_tcc.wave_ext.invert[CONF_PWM_OUTPUT2] = false;
	config_tcc.wave_ext.invert[CONF_PWM_OUTPUT3] = false;
	config_tcc.wave_ext.invert[CONF_PWM_OUTPUT4] = true;
	config_tcc.wave_ext.invert[CONF_PWM_OUTPUT5] = true;
	//! [setup_change_config]

	//! [setup_change_config_pwm]
	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT0] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT0]        = CONF_PWM_OUT0_PIN;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT0]    = CONF_PWM_OUT0_MUX;

	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT1] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT1]        = CONF_PWM_OUT1_PIN;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT1]    = CONF_PWM_OUT1_MUX;

	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT2] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT2]        = CONF_PWM_OUT2_PIN;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT2]    = CONF_PWM_OUT2_MUX;

	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT3] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT3]        = CONF_PWM_OUT3_PIN;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT3]    = CONF_PWM_OUT3_MUX;

	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT4] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT4]        = CONF_PWM_OUT4_PIN;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT4]    = CONF_PWM_OUT4_MUX;

	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT5] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT5]        = CONF_PWM_OUT5_PIN;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT5]    = CONF_PWM_OUT5_MUX;
	//! [setup_change_config_pwm]

	//! [setup_set_config]
	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	//! [setup_set_config]

	//! [setup_enable]
	tcc_enable(&tcc_instance);
	//! [setup_enable]
}

static void configure_tcc_callbacks(void)
{
	//! [setup_register_callback]
	tcc_register_callback(&tcc_instance, tcc_callback_to_change_pattern,
			TCC_CALLBACK_OVERFLOW);
	//! [setup_register_callback]

	//! [setup_enable_callback]
	tcc_enable_callback(&tcc_instance, TCC_CALLBACK_OVERFLOW);
	//! [setup_enable_callback]
}
//! [setup]

int main(void)
{
	system_init();

//! [setup_init]
	configure_tcc();
	configure_tcc_callbacks();
//! [setup_init]

//! [main]
	//! [enable_global_interrupts]
	system_interrupt_enable_global();
	//! [enable_global_interrupts]

	//! [main_loop]
	while (true) {
	}
	//! [main_loop]
//! [main]
}
