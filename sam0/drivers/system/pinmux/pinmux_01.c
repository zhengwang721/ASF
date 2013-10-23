/**
 * \file
 *
 * \brief SAM D2x System Interrupt Driver
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#include "pinmux_01.h"

/**
 * \brief Configures the output slew rate mode for a group of pins.
 *
 * Configures the output slew rate mode for a group of pins, to
 * control the speed at which the physical output pin can react to
 * logical changes of the I/O pin value.
 *
 * \param[in] port     Base of the PORT module to configure.
 * \param[in] mask     Mask of the port pin(s) to configure.
 * \param[in] mode     New pin slew rate mode to configure.
 */
void system_pinmux_group_set_output_slew_rate(
		PortGroup *const port,
		const uint32_t mask,
		const enum system_pinmux_pin_slew_rate mode)
{
	Assert(port);

	for (int i = 0; i < 32; i++) {
		if (mask & (1UL << i)) {
			if (mode == SYSTEM_PINMUX_PIN_SLEW_RATE_LIMITED) {
				port->PINCFG[i].reg |=  PORT_PINCFG_SLEWLIM;
			}
			else {
				port->PINCFG[i].reg &= ~PORT_PINCFG_SLEWLIM;
			}
		}
	}
}


/**
 * \brief Configures the output driver strength mode for a group of pins.
 *
 * Configures the output drive strength for a group of pins, to
 * control the amount of current the pad is able to sink/source.
 *
 * \param[in] port     Base of the PORT module to configure.
 * \param[in] mask     Mask of the port pin(s) to configure.
 * \param[in] mode     New output driver strength mode to configure.
 */
void system_pinmux_group_set_output_strength(
		PortGroup *const port,
		const uint32_t mask,
		const enum system_pinmux_pin_strength mode)
{
	Assert(port);

	for (int i = 0; i < 32; i++) {
		if (mask & (1UL << i)) {
			if (mode == SYSTEM_PINMUX_PIN_STRENGTH_HIGH) {
				port->PINCFG[i].reg |=  PORT_PINCFG_DRVSTR;
			}
			else {
				port->PINCFG[i].reg &= ~PORT_PINCFG_DRVSTR;
			}
		}
	}
}

/**
 * \brief Configures the output driver mode for a group of pins.
 *
 * Configures the output driver mode for a group of pins, to
 * control the pad behavior.
 *
 * \param[in] port Base of the PORT module to configure.
 * \param[in] mask Mask of the port pin(s) to configure.
 * \param[in] mode New pad output driver mode to configure.
 */
void system_pinmux_group_set_output_drive(
		PortGroup *const port,
		const uint32_t mask,
		const enum system_pinmux_pin_drive mode)
{
	Assert(port);

	for (int i = 0; i < 32; i++) {
		if (mask & (1UL << i)) {
			if (mode == SYSTEM_PINMUX_PIN_DRIVE_OPEN_DRAIN) {
				port->PINCFG[i].reg |= PORT_PINCFG_ODRAIN;
			} else {
				port->PINCFG[i].reg &= ~PORT_PINCFG_ODRAIN;
			}
		}
	}
}


