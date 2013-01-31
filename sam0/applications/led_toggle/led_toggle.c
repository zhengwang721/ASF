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

/*
	USE_INTERRUPTS    USE_EIC  Result
	--------------    -------  ---------------------------------------------
	false             false    Polled via PORT driver
	false             true     Polled via EIC driver
	true              false    Polled via PORT driver, using SysTick handler
	true              true     Polled via EIC driver, using EIC handler
 */

#define USE_INTERRUPTS   true
#define USE_EIC          true

static void board_extint_handler(uint32_t channel)
{
	bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
	port_pin_set_output_level(LED_0_PIN, !pin_state);
}

static void configure_led(void)
{
	struct port_conf pin_conf;
	port_get_config_defaults(&pin_conf);

	pin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_0_PIN, &pin_conf);
}

static void configure_button(void)
{
#if USE_EIC == false
	struct port_conf pin_conf;
	port_get_config_defaults(&pin_conf);

	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(BUTTON_0_PIN, &pin_conf);
#else
	struct extint_ch_conf eint_ch_conf;
	extint_ch_get_config_defaults(&eint_ch_conf);

	eint_ch_conf.gpio_pin     = BUTTON_0_EIC_PIN;
	eint_ch_conf.gpio_pin_mux = BUTTON_0_EIC_PIN_MUX;
	eint_ch_conf.detect       = EXTINT_DETECT_BOTH;
	extint_ch_set_config(BUTTON_0_EIC_LINE, &eint_ch_conf);

	extint_enable();

#  if USE_INTERRUPTS == true
	extint_async_register_callback(board_extint_handler,
			EXTINT_ASYNC_TYPE_DETECT);
	extint_async_ch_enable_callback(BUTTON_0_EIC_LINE,
			EXTINT_ASYNC_TYPE_DETECT);
#  endif
#endif
}

#if USE_EIC == false && USE_INTERRUPTS == true
void SysTick_Handler(void)
{
	board_extint_handler(BUTTON_0_EIC_LINE);
}
#endif

int main(void)
{
	system_init();

	configure_led();
	configure_button();

#if USE_INTERRUPTS == true
#  if USE_EIC == false
	SysTick->CTRL = 0;
	SysTick->LOAD = 999;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
#  endif

	cpu_irq_enable();

	while (true) {
		/* Do nothing - use interrupts */
	}
#else
#  if USE_EIC == false
	while (true) {
		board_extint_handler(BUTTON_0_EIC_LINE);
	}
#  else
	while (true) {
		if (extint_ch_is_detected(BUTTON_0_EIC_LINE)) {
			extint_ch_clear_detected(BUTTON_0_EIC_LINE);

			board_extint_handler(BUTTON_0_EIC_LINE);
		}
	}
#  endif
#endif
}
