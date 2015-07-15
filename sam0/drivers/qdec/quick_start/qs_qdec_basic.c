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

void configure_qdec(void);

//! [setup]
void configure_qdec(void)
{
//! [set_conf]
	struct qdec_config config_qdec;
//! [set_conf]
//! [get_def]
	qdec_get_config_defaults(&config_qdec);
//! [get_def]

//! [init_qdec1]
	qdec_init(QDEC_AXIS_X, &config_qdec);
//! [init_qdec1]

//! [set_config1]
	config_qdec.pinmux_pad[0] = 
			PINMUX_MEGAMUX_SEL31_QDEC_Y_A_PIN(PIN_LP_GPIO_17);
	config_qdec.pinmux_pad[1] = 
			PINMUX_MEGAMUX_SEL32_QDEC_Y_B_PIN(PIN_LP_GPIO_18);
//! [set_config1]
//! [init_qdec2]
	qdec_init(QDEC_AXIS_Y, &config_qdec);
//! [init_qdec2]

//! [set_config2]
	config_qdec.pinmux_pad[0] = 
			PINMUX_MEGAMUX_SEL33_QDEC_Z_A_PIN(PIN_LP_GPIO_19);
	config_qdec.pinmux_pad[1] = 
			PINMUX_MEGAMUX_SEL34_QDEC_Z_B_PIN(PIN_LP_GPIO_20);
//! [set_config2]
//! [init_qdec3]
	qdec_init(QDEC_AXIS_Y, &config_qdec);
//! [init_qdec3]

//! [enable_clk]
	qdec_clock_enable(QDEC_AXIS_X);
	qdec_clock_enable(QDEC_AXIS_Y);
	qdec_clock_enable(QDEC_AXIS_Z);
//! [enable_clk]
//! [enable_qdec]
	qdec_enable(QDEC_AXIS_X);
	qdec_enable(QDEC_AXIS_Y);
	qdec_enable(QDEC_AXIS_Z);
//! [enable_qdec]
}
//! [setup]

int main(void)
{
//! [add_main]
	//system_init();

	configure_qdec();

//! [add_main]

//! [main_imp]
//! [main_loop]
	while (true) {
//! [main_loop]

		/* Check irq status, if trigger irq, clear irq status and 
		 * reset the quad decoder to reset counter */
//! [get_irq]
		uint8_t staus = qdec_get_irq_status();
//! [get_irq]
//! [ckeck_status]
		if (staus & LPMCU_MISC_REGS_QUAD_DEC_IRQS_QUAD_DEC_1_IRQ) {
//! [ckeck_status]
//! [clear_status]
			qdec_clear_irq_status(QDEC_AXIS_X);
//! [clear_status]
//! [reset]
			qdec_reset(QDEC_AXIS_X);
//! [reset]
		} else if (staus & LPMCU_MISC_REGS_QUAD_DEC_IRQS_QUAD_DEC_2_IRQ) {
			qdec_clear_irq_status(QDEC_AXIS_Y);
			qdec_reset(QDEC_AXIS_Y);
		} else if (staus & LPMCU_MISC_REGS_QUAD_DEC_IRQS_QUAD_DEC_3_IRQ) {
			qdec_clear_irq_status(QDEC_AXIS_Z);
			qdec_reset(QDEC_AXIS_Z);
		}
//! [get_counter]
		/* Get x,y,z axis current counter */
		qdec_axis_x = qdec_get_counter(QDEC_AXIS_X);
		qdec_axis_y = qdec_get_counter(QDEC_AXIS_Y);
		qdec_axis_z = qdec_get_counter(QDEC_AXIS_Z);
//! [get_counter]
	}
//! [main_imp]
}
