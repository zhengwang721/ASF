/**
 * \file
 *
 * \brief SAM4S Xplained PRO board IO driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 *    AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 */

#ifndef BOARD_IO_H
#define BOARD_IO_H

#include <board.h>
#include <compiler.h>
#include <ioport.h>

static inline void board_toggle_led(uint8_t led_id)
{
	switch (led_id) {
	case 0:
		ioport_toggle_pin_level(LED_0_PIN);
		break;

	default:
		break;
	}
}

static inline void board_turn_led_on(uint8_t led_id)
{
	switch (led_id) {
	case 0:
		ioport_set_pin_level(LED_0_PIN, false);
		break;

	default:
		break;
	}
}

static inline void board_turn_led_off(uint8_t led_id)
{
	switch (led_id) {
	case 0:
		ioport_set_pin_level(LED_0_PIN, true);
		break;

	default:
		break;
	}
}

static inline bool board_button_is_pressed(uint8_t button_id)
{
	bool state;

	switch (button_id) {
	case 0:
		state = !ioport_get_pin_level(BUTTON_0_PIN);
		break;

	default:
		state = false;
		break;
	}

	return state;
}

#endif /* BOARD_IO_H */
