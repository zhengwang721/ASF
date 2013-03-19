/**
 * \file
 *
 * \brief SAMD20 TC Callbac Driver
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
#include <conf_quick_start_callback.h>
#include <asf.h>



static void qs_tc_callback_to_change_duty_cycle(
		struct tc_module *const module_inst)
{
	static uint32_t pases = 0;
	pases = pases + 1;
	if (pases == 2) {
		while (tc_is_syncing(module_inst)) {
			/* Wait for sync */
		}
		module_inst->hw->COUNT16.CC[0].reg = 0x5555;
	}
	if (pases == 3) {
		pases = 0;
		while (tc_is_syncing(module_inst)) {
			/* Wait for sync */
		}
		module_inst->hw->COUNT16.CC[0].reg = 0x7FFF;
	}
}

int main(void)
{
	//! [main]

	//! [system_init]
	system_init();
	//! [system_init]

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
	config.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
	config.size_specific.size_16_bit.compare_capture_channel[0] = 0x7FFF;
	//! [setup]

	//! [tc_init]
	tc_init(&module_inst, PWM_MODULE, &config);
	//! [tc_init]

	//! [register_callback]
	tc_register_callback(
			&module_inst,
			(tc_callback_t)qs_tc_callback_to_change_duty_cycle,
			TC_CALLBACK_CC_CHANNEL0);
	//! [register_callback]

	//! [enable_callback
	tc_enable_callback(&module_inst, TC_CALLBACK_CC_CHANNEL0);
	//! [enable_callback]

	//! [enable_global_interrupts]
	cpu_irq_enable();
	//! [enable_global_interrupts]

	//! [tc_enable]
	tc_enable(&module_inst);
	//! [tc_enable]

	//! [inf_loop]
	while (1) {
	}
	//! [inf_loop]
	//! [main]
}
