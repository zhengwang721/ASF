/**
 * \file
 *
 * \brief SAM4L Xplained PRO board initialization
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
 *
 */
#include "compiler.h"
#include "sam4l_xplained_pro.h"
#include "conf_board.h"
#include "ioport.h"
#include "board.h"
/**
 * \addtogroup  sam4l_xplained_pro_group
 * @{
 */

/**
 * \brief Set peripheral mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_pin_peripheral_mode(pin, mode) \
	do {\
		ioport_set_pin_mode(pin, mode);\
		ioport_disable_pin(pin);\
	} while (0)

void board_init(void)
{
	// Initialize IOPORTs
	ioport_init();

	/* Configure the pins connected to LEDs as output and set their
	 * default initial state to high (LEDs off).
	 */
	ioport_set_pin_dir(LED0, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED0, IOPORT_PIN_LEVEL_HIGH);

#ifdef  CONF_BOARD_EIC
	// Set push button as external interrupt pin
	ioport_set_pin_peripheral_mode(GPIO_PUSH_BUTTON_EIC_PIN,
			GPIO_PUSH_BUTTON_EIC_PIN_MUX);
	ioport_set_pin_peripheral_mode(GPIO_UNIT_TEST_EIC_PIN,
			GPIO_UNIT_TEST_EIC_PIN_MUX);
#else
	// Push button as input: already done, it's the default pin state
#endif

#if defined (CONF_BOARD_COM_PORT)
	ioport_set_pin_peripheral_mode(EXT2_PIN_UART_RX, EXT2_UART_RX_MUX);
	ioport_set_pin_peripheral_mode(EXT2_PIN_UART_TX, EXT2_UART_TX_MUX);
#endif

#ifdef CONF_BOARD_USART0
	ioport_set_pin_peripheral_mode(EXT4_PIN_UART_RX, EXT4_UART_RX_MUX);
	ioport_set_pin_peripheral_mode(EXT4_PIN_UART_TX, EXT4_UART_TX_MUX);
#endif
}
/**
 * @}
 */
