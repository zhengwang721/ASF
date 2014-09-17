/**
 * \file
 *
 * \brief SAM0 OPAMP Driver Quick Start
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

void configure_inverting_pga_opamp0(void);

//! [setup]
void configure_inverting_pga_opamp0(void)
{
	/** Create a new configuration structure for the OPAMP0 */
	//! [setup_1]
	struct opamp0_config conf;
	//! [setup_1

	/** Reset OPAMP module. */
	//! [setup_2]
	opamp_module_reset();
	//! [setup_2]

	/** Enable OPAMP module. */
	//! [setup_3]
	opamp_module_enable();
	//! [setup_3]

	/** settings and fill with the default settings. */
	//! [setup_4]
	opamp0_get_config_defaults(&conf);
	//! [setup_4]

	/* Set the the OPAMP0 as "Inverting PGA" mode. */
	//! [setup_5]
	conf.negative_input = OPAMP0_NEG_MUX_TAP0;
	conf.positive_input = OPAMP0_POS_MUX_GND;
	conf.r1_connection = OPAMP0_RES1_MUX_NEG_PIN0;
	conf.config_common.r1_enable = true;
	conf.config_common.r2_out = true;
	//! [setup_5]

	/* Set up OA0NEG pin and OA0OUT pin. */
	//! [setup_6]
	struct system_pinmux_config opamp0_neg_pin_conf;
	system_pinmux_get_config_defaults(&opamp0_neg_pin_conf);
	opamp0_neg_pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	opamp0_neg_pin_conf.mux_position = MUX_PA02B_OPAMP_OANEG0;
	system_pinmux_pin_set_config(PIN_PA02B_OPAMP_OANEG0, &opamp0_neg_pin_conf);
	struct system_pinmux_config opamp0_out_pin_conf;
	system_pinmux_get_config_defaults(&opamp0_out_pin_conf);
	opamp0_out_pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	opamp0_out_pin_conf.mux_position = MUX_PA07B_OPAMP_OAOUT0;
	system_pinmux_pin_set_config(PIN_PA07B_OPAMP_OAOUT0, &opamp0_out_pin_conf);
	//! [setup_6]

	/** Initialize and enable the OPAMP0 with the user settings. */
	//! [setup_7]
	opamp0_set_config(&conf);
	//! [setup_7]
	//! [setup_8]
	opamp_enable(OPAMP_0);
	//! [setup_8]

	/* Wait for the output ready. */
	//! [setup_9]
	while(!opamp_is_ready(OPAMP_0));
	//! [setup_9]
}
//! [setup]

int main(void)
{
	system_init();

	//! [setup_init]
	configure_inverting_pga_opamp0();
	//! [setup_init]

	//! [main]
	while (true) {
		/* Do nothing */
	}
	//! [main]
}
