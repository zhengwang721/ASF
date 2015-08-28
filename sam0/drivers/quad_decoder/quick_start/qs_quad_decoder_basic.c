/**
 * \file
 *
 * \brief SAM QUAD DECODER Driver Quick Start for SAMB11
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

volatile int16_t	qdec_axis_x,
					qdec_axis_y,
					qdec_axis_z;

//! [setup]
static void configure_quad_decoder(void)
{
//! [set_conf]
	struct quad_decoder_config config_quad_decoder;
//! [set_conf]
//! [get_def]
	quad_decoder_get_config_defaults(&config_quad_decoder);
//! [get_def]
//! [set_config1]
	config_quad_decoder.pinmux_pad[0] = CONF_QUAD_DECODER_MUX_X_A;
	config_quad_decoder.pinmux_pad[1] = CONF_QUAD_DECODER_MUX_X_B;
//! [set_config1]
//! [init_qdec1]
	quad_decoder_init(QDEC_AXIS_X, &config_quad_decoder);
//! [init_qdec1]

//! [set_config2]
	config_quad_decoder.pinmux_pad[0] = CONF_QUAD_DECODER_MUX_Y_A;
	config_quad_decoder.pinmux_pad[1] = CONF_QUAD_DECODER_MUX_Y_B;
//! [set_config2]
//! [init_qdec2]
	quad_decoder_init(QDEC_AXIS_Y, &config_quad_decoder);
//! [init_qdec2]

//! [set_config3]
	config_quad_decoder.pinmux_pad[0] = CONF_QUAD_DECODER_MUX_Z_A;
	config_quad_decoder.pinmux_pad[1] = CONF_QUAD_DECODER_MUX_Z_B;
//! [set_config3]
//! [init_qdec3]
	quad_decoder_init(QDEC_AXIS_Z, &config_quad_decoder);
//! [init_qdec3]

//! [enable_qdec]
	quad_decoder_enable(QDEC_AXIS_X);
	quad_decoder_enable(QDEC_AXIS_Y);
	quad_decoder_enable(QDEC_AXIS_Z);
//! [enable_qdec]
}
//! [setup]

int main(void)
{
//! [add_main]
	uint8_t status;
	//system_init();

	configure_quad_decoder();

//! [add_main]

//! [main_imp]
//! [main_loop]
	while (true) {
//! [main_loop]

		/* Check irq status, if trigger irq, clear irq status and
		 * reset the quad decoder to reset counter */
//! [get_irq]
		status = quad_decoder_get_irq_status();
//! [get_irq]
//! [ckeck_status]
		if (status & LPMCU_MISC_REGS_QUAD_DEC_IRQS_QUAD_DEC_1_IRQ) {
//! [ckeck_status]
//! [clear_status]
			quad_decoder_clear_irq_status(QDEC_AXIS_X);
//! [clear_status]
		}
		if (status & LPMCU_MISC_REGS_QUAD_DEC_IRQS_QUAD_DEC_2_IRQ) {
			quad_decoder_clear_irq_status(QDEC_AXIS_Y);
		}
		if (status & LPMCU_MISC_REGS_QUAD_DEC_IRQS_QUAD_DEC_3_IRQ) {
			quad_decoder_clear_irq_status(QDEC_AXIS_Z);
		}

//! [get_counter]
		/* Get x,y,z axis current counter */
		qdec_axis_x = quad_decoder_get_counter(QDEC_AXIS_X);
		qdec_axis_y = quad_decoder_get_counter(QDEC_AXIS_Y);
		qdec_axis_z = quad_decoder_get_counter(QDEC_AXIS_Z);
//! [get_counter]
	}
//! [main_imp]
}
