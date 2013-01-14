/**
 * \file
 *
 * \brief SAMD20 Pin Multiplexer Driver
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
#include <pinmux.h>

/** \brief Writes a Port pin configuration to the hardware module.
 *
 *  Writes out a given configuration of a Port pin configuration to the hardware
 *  module.
 *
 *  \note If the pin output buffer is enabled, the pull-up/pull-down input
 *        buffer configuration setting is ignored.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to configure
 *  \param[in] config    Configuration settings for the pin
 */
void system_pinmux_pin_set_config(
		const uint8_t gpio_pin,
		const struct system_pinmux_pin_conf *const config)
{
	PortGroup *const port_base = system_pinmux_get_port_from_gpio_pin(gpio_pin);
	uint32_t pin_mask = (1UL << (gpio_pin % 32));

	/* Track the configuration bits into a temporary variable before writing */
	uint32_t pin_cfg = 0;

	/* Check the IO port pin configuration type */
	bool is_output = (config->direction == SYSTEM_PINMUX_PIN_DIR_OUTPUT) |
			(config->direction == SYSTEM_PINMUX_PIN_DIR_BOTH);

	bool is_input = (config->direction == SYSTEM_PINMUX_PIN_DIR_INPUT) |
			(config->direction == SYSTEM_PINMUX_PIN_DIR_BOTH);

	/* Check if the user has requested that the input buffer be enabled */
	if (is_input) {
		/* Enable input buffer flag */
		pin_cfg |= PORT_PINCFG_INEN;

		/* Enable pull-up/pull-down control flag if requested */
		if (config->input.pull != SYSTEM_PINMUX_PIN_PULL_NONE) {
			pin_cfg |= PORT_PINCFG_PULLEN;
		}

		/* Set the requested sampling mode of the input buffer */
		if (config->input.sampling_mode ==
				SYSTEM_PINMUX_PIN_SAMPLING_CONTINUOUS) {
			port_base->CTRL.reg |= pin_mask;
		} else {
			port_base->CTRL.reg &= ~pin_mask;
		}

		/* Clear the port DIR bits to disable the output buffer */
		port_base->DIRCLR.reg = pin_mask;
	}

	/* Check if the user has requested that the output buffer be enabled */
	if (is_output) {
		/* Cannot use a pullup if the output driver is enabled,
		 * if requested the input buffer can only sample the current
		 * output state */
		pin_cfg &= ~PORT_PINCFG_PULLEN;

		/* Set the pin drive strength mode */
		if (config->output.strength == SYSTEM_PINMUX_PIN_STRENGTH_HIGH) {
			pin_cfg |= PORT_PINCFG_DRVSTR;
		}

		/* Set the pin slew rate limiter mode */
		if (config->output.slew_rate == SYSTEM_PINMUX_PIN_SLEW_RATE_LIMITED) {
			pin_cfg |= PORT_PINCFG_SLEWLIM;
		}

		/* Set the pin drive mode */
		if (config->output.drive == SYSTEM_PINMUX_PIN_DRIVE_OPEN_DRAIN) {
			pin_cfg |= PORT_PINCFG_ODRAIN;
		}

		/* Set the port DIR bits to enable the output buffer */
		port_base->DIRSET.reg = pin_mask;
	}

	if (config->mux_position != SYSTEM_PINMUX_GPIO) {
		/* Enable the pin peripheral mux flag - pin mux will be written
		 * later */
		pin_cfg |= PORT_PINCFG_PMUXEN;
	}

	/* The Write Configuration register (WRCONFIG) requires the
	 * pins to to grouped into two 16-bit half-words - split them out here */
	uint32_t lower_pin_mask = (pin_mask & 0xFFFF);
	uint32_t upper_pin_mask = (pin_mask >> 16);

	/* Configure the lower 16-bits of the port to the desired configuration,
	 * including the pin peripheral multiplexer just in case it is enabled */
	port_base->WRCONFIG.reg
		= (lower_pin_mask << PORT_WRCONFIG_PINMASK_Pos) |
			/* Shift to upper 16 bits. */
			(pin_cfg << 16) |
			(config->mux_position << PORT_WRCONFIG_PMUX_Pos) |
			PORT_WRCONFIG_WRPMUX | PORT_WRCONFIG_WRPINCFG;

	/* Configure the upper 16-bits of the port to the desired configuration,
	 * including the pin peripheral multiplexer just in case it is enabled */
	port_base->WRCONFIG.reg
		= (upper_pin_mask << PORT_WRCONFIG_PINMASK_Pos) |
			/* Shift to upper 16 bits. */
			(pin_cfg << 16) |
			(config->mux_position << PORT_WRCONFIG_PMUX_Pos) |
			PORT_WRCONFIG_WRPMUX | PORT_WRCONFIG_WRPINCFG |
			PORT_WRCONFIG_HWSEL;

	/* Set the pull-up state once the port pins are configured if one was
	 * requested and it does not violate the valid set of port
	 * configurations */
	if (pin_cfg & PORT_PINCFG_PULLEN) {
		/* Set the OUT register bits to enable the pullup if requested,
		 * clear to enable pull-down */
		if (config->input.pull == SYSTEM_PINMUX_PIN_PULL_UP) {
			port_base->OUTSET.reg = pin_mask;
		} else {
			port_base->OUTCLR.reg = pin_mask;
		}
	}
}
