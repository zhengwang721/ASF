/**
 * \file
 *
 * \brief SAMD20 External Interrupt Driver Quick Start
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
 *
 */
#include <asf.h>

#define BOARD_LED                    PIN_PB08
#define BOARD_BUTTON                 PIN_PB09
#define BOARD_BUTTON_EXTINT_MUX      PINMUX_PB09A_EIC_EXTINT5
#define BOARD_BUTTON_EXTINT_CHANNEL  5

#define USE_INTERRUPTS               false
#define USE_EIC                      true

#if (USE_INTERRUPTS && !USE_EIC)
#  error Invalid application configuration.
#endif

static void board_extint_handler(uint32_t channel)
{
	bool pin_state = port_pin_get_input_level(BOARD_BUTTON);
	port_pin_set_output_level(BOARD_LED, !pin_state);
}

static void configure_led(void)
{
	struct port_conf pin_conf;
	port_get_config_defaults(&pin_conf);

	pin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(BOARD_LED, &pin_conf);
}

static void configure_button(void)
{
#if USE_EIC == false
	struct port_conf pin_conf;
	port_get_config_defaults(&pin_conf);

	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(BOARD_BUTTON, &pin_conf);
#else
	extint_enable();

	struct extint_ch_conf eint_ch_conf;
	extint_ch_get_config_defaults(&eint_ch_conf);

	eint_ch_conf.pinmux_position = BOARD_BUTTON_EXTINT_MUX;
	eint_ch_conf.detect          = EXTINT_DETECT_BOTH;
	extint_ch_set_config(BOARD_BUTTON_EXTINT_CHANNEL, &eint_ch_conf);

#  if USE_INTERRUPTS == true
	extint_async_register_callback(board_extint_handler,
			EXTINT_ASYNC_TYPE_DETECT);
	extint_async_ch_enable_callback(BOARD_BUTTON_EXTINT_CHANNEL,
			EXTINT_ASYNC_TYPE_DETECT);
#  endif
#endif
}

int main(void)
{
	configure_led();
	configure_button();

#if USE_EIC == true
	system_gclk_init();

	struct system_gclk_gen_conf gclock_gen_conf;
	system_gclk_gen_get_config_defaults(&gclock_gen_conf);
	gclock_gen_conf.source_clock    = 0;
	gclock_gen_conf.division_factor = 128;
	system_gclk_gen_set_config(1, &gclock_gen_conf);
	system_gclk_gen_enable(1);

	struct system_gclk_ch_conf gclock_ch_conf;
	system_gclk_ch_get_config_defaults(&gclock_ch_conf);
	gclock_ch_conf.source_generator    = 1;
	gclock_ch_conf.enable_during_sleep = false;
	system_gclk_ch_set_config(EIC_GCLK_ID, &gclock_ch_conf);
	system_gclk_ch_enable(EIC_GCLK_ID);
#endif

#if USE_INTERRUPTS == true
	cpu_irq_enable();

	while (true) {
		/* Do nothing - use interrupts */
	}
#else
#  if USE_EIC == false
	while (true) {
		board_extint_handler(BOARD_BUTTON_EXTINT_CHANNEL);
	}
#  else
	while (true) {
		//if (extint_ch_is_detected(BOARD_BUTTON_EXTINT_CHANNEL)) {
			extint_ch_clear_detected(BOARD_BUTTON_EXTINT_CHANNEL);

			board_extint_handler(BOARD_BUTTON_EXTINT_CHANNEL);
		//}
	}
#  endif
#endif
}
