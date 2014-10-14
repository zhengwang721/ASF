/**
 * \file
 *
 * \brief SAM CCL Driver Quick Start
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
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

void configure_ccl(void);
void configure_ccl_lut0(void);

//! [setup]
void configure_ccl(void)
{
	/** Creates a new configuration structure for the CCL. */
	//! [setup_1]
	struct ccl_config conf;
	//! [setup_1]

	/** Settings and fill with the default settings. */
	//! [setup_2]
	ccl_get_config_defaults(&conf);
	//! [setup_2]

	/** Set the CCL. */
	//! [setup_3]
	conf.run_in_standby = true;
	//! [setup_3]

	/** Initialize the CCL with the user settings. */
	//! [setup_4]
	ccl_init(&conf);
	//! [setup_4]
}

void configure_ccl_lut0(void)
{
	/** Creates a new configuration structure for the LUT0. */
	//! [setup_5]
	struct ccl_lut_config conf;
	//! [setup_5]

	/** Settings and fill with the default settings. */
	//! [setup_6]
	ccl_lut_get_config_defaults(&conf);
	//! [setup_6]

	/** Set the LUT0. */
	//! [setup_7]
	conf.input0_src_sel = CCL_LUT_INPUT_SRC_IO;
	conf.input1_src_sel = CCL_LUT_INPUT_SRC_IO;
	conf.input2_src_sel = CCL_LUT_INPUT_SRC_IO;
	conf.edge_selection_enable = true;
	conf.filter_sel = CCL_LUTCTRL_FILTSEL_FILTER;
	//! [setup_7]

	/** Set up LUT0 input and output pin. */
	//! [setup_8]
	struct system_pinmux_config lut0_input_pin0_conf, lut0_input_pin1_conf, lut0_input_pin2_conf;
	system_pinmux_get_config_defaults(&lut0_input_pin0_conf);
	system_pinmux_get_config_defaults(&lut0_input_pin1_conf);
	system_pinmux_get_config_defaults(&lut0_input_pin2_conf);
	lut0_input_pin0_conf.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	lut0_input_pin0_conf.mux_position = MUX_PA04I_CCL_IN0;
	lut0_input_pin1_conf.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	lut0_input_pin1_conf.mux_position = MUX_PA05I_CCL_IN1;
	lut0_input_pin2_conf.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	lut0_input_pin2_conf.mux_position = MUX_PA06I_CCL_IN2;
	system_pinmux_pin_set_config(PIN_PA04I_CCL_IN0, &lut0_input_pin0_conf);
	system_pinmux_pin_set_config(PIN_PA05I_CCL_IN1, &lut0_input_pin1_conf);
	system_pinmux_pin_set_config(PIN_PA06I_CCL_IN2, &lut0_input_pin2_conf);
	struct system_pinmux_config lut0_out_pin_conf;
	system_pinmux_get_config_defaults(&lut0_out_pin_conf);
	lut0_out_pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	lut0_out_pin_conf.mux_position = MUX_PA07I_CCL_OUT0;
	system_pinmux_pin_set_config(PIN_PA07I_CCL_OUT0, &lut0_out_pin_conf);
	//! [setup_8]

	/** Initialize and enable the LUT0 with the user settings. */
	//! [setup_9]
	ccl_lut_set_config(CCL_LUT_0, &conf);
	//! [setup_9]
	//! [setup_10]
	ccl_lut_enable(CCL_LUT_0);
	//! [setup_10]

	//! [setup_11]
	/** Enable CCL module. */
	ccl_module_enable();
	//! [setup_11]
}
//! [setup]

int main(void)
{
	system_init();

	//! [setup_init]
	configure_ccl();
	configure_ccl_lut0();
	//! [setup_init]

	//! [main]
	while (true) {
		/* Do nothing */
	}
	//! [main]
}
