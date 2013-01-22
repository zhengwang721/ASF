/**
 * \file
 *
 * \brief SAMD20 GPIO Port Driver
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
#include <port.h>

/** \brief Writes a Port pin edge detection configuration to the hardware module.
 *
 *  Writes out a given configuration of a Port pin edge detection configuration
 *  to the hardware module.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to configure
 *  \param[in] config    Configuration settings for the pin edge detection mode
 */
static inline void _port_pin_set_eic_config(
		const uint8_t gpio_pin,
		const struct port_pin_edge_conf *const config)
{
	Eic*   eic_base       = port_get_eic_from_gpio_pin(gpio_pin);
	uint32_t pin_mask       = (1UL << (gpio_pin % 32));
	uint8_t  eic_config_pos = (4 * (gpio_pin % 8));
	uint32_t eic_new_config;

	/* Enable the EIC module if needed in case it is not already enabled */
	if (config->mode != PORT_EDGE_DETECT_NONE) {
		eic_base->CTRL.reg |= EIC_CTRL_ENABLE;
	}

	/* Set the channel's new wake up mode setting */
	if (config->wake_if_sleeping) {
		eic_base->WAKEUP.reg |=  pin_mask;
	} else {
		eic_base->WAKEUP.reg &= ~pin_mask;
	}

	/* Determine the channel's new edge detection configuration */
	eic_new_config = config->mode;

	/* Enable the hardware signal filter if requested in the config */
	if (config->filter_input_signal) {
		// TODO: Used NMIFILTEN as CONFIG_FILTEN is not in header. */
		eic_new_config |= EIC_NMICTRL_NMIFILTEN;
	}

	/* Clear the existing and set the new channel configuration */
	eic_base->CONFIG[gpio_pin / 8].reg
		= (eic_base->CONFIG[gpio_pin / 8].reg & ~(0xF << eic_config_pos)) |
			(eic_new_config << eic_config_pos);
}

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
 *
 *  \return Status of the configuration procedure.
 *  \retval STATUS_OK     If the channel was configured correctly
 *  \retval STATUS_ERR_INVALID_ARG   If invalid argument(s) were supplied
 */
enum status_code port_pin_set_config(
		const uint8_t gpio_pin,
		const struct port_pin_conf *const config)
{
	PortGroup*  port_base = port_get_port_from_gpio_pin(gpio_pin);
	uint32_t pin_mask  = (1UL << (gpio_pin % 32));

	/* Track the configuration bits into a temporary variable before writing */
	uint32_t pin_cfg = 0;

	/* Check the IO port pin configuration type */
	if (config->type == PORT_PIN_TYPE_GPIO) {
		/* Check if the user has requested that the input buffer be enabled */
		if (config->input.enabled) {
			/* Enable input buffer flag */
			pin_cfg |= PORT_PINCFG_INEN;

			/* Enable pull-up/pull-down control flag if requested */
			if (config->input.pull != PORT_PIN_PULL_NONE) {
				pin_cfg |= PORT_PINCFG_PULLEN;
			}

			/* Set the requested sampling mode of the input buffer */
			if (config->input.sampling_mode == PORT_PIN_SAMPLING_CONTINUOUS) {
				port_base->CTRL.reg |= pin_mask;
			} else {
				port_base->CTRL.reg &= ~pin_mask;
			}

			/* Clear the port DIR bits to disable the output buffer */
			port_base->DIRCLR.reg = pin_mask;

			/* Configure pin edge detection if one is requested */
			_port_pin_set_eic_config(gpio_pin, &config->input.edge_detection);
		}

		/* Check if the user has requested that the output buffer be enabled */
		if (config->output.enabled) {
			/* Cannot use a pullup if the output driver is enabled,
			 * if requested the input buffer can only sample the current
			 * output state */
			pin_cfg &= ~PORT_PINCFG_PULLEN;

			/* Set the pin drive strength mode */
			if (config->output.drive_strength == PORT_PIN_STRENGTH_HIGH) {
				pin_cfg |= PORT_PINCFG_DRVSTR;
			}

			/* Set the pin slew rate limiter mode */
			if (config->output.slew_rate == PORT_PIN_SLEW_RATE_LIMITED) {
				pin_cfg |= PORT_PINCFG_SLEWLIM;
			}

			/* Set the pin drive mode */
			if (config->output.drive == PORT_PIN_DRIVE_OPEN_DRAIN) {
				pin_cfg |= PORT_PINCFG_ODRAIN;
			}

			/* Set the port DIR bits to enable the output buffer */
			port_base->DIRSET.reg = pin_mask;
		}
	} else if (config->type == PORT_PIN_TYPE_PERIPHERAL) {
		/* Enable the pin peripheral mux flag - pin mux will be written
		 * later */
		pin_cfg |= PORT_PINCFG_PMUXEN;
	} else {
		/* Unknown pin mode, return an error */
		return STATUS_ERR_INVALID_ARG;
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
			(config->peripheral_index << PORT_WRCONFIG_PMUX_Pos) |
			PORT_WRCONFIG_WRPMUX | PORT_WRCONFIG_WRPINCFG;

	/* Configure the upper 16-bits of the port to the desired configuration,
	 * including the pin peripheral multiplexer just in case it is enabled */
	port_base->WRCONFIG.reg
		= (upper_pin_mask << PORT_WRCONFIG_PINMASK_Pos) |
			/* Shift to upper 16 bits. */
			(pin_cfg << 16) |
			(config->peripheral_index << PORT_WRCONFIG_PMUX_Pos) |
			PORT_WRCONFIG_WRPMUX | PORT_WRCONFIG_WRPINCFG |
			PORT_WRCONFIG_HWSEL;

	/* Set the pull-up state once the port pins are configured if one was
	 * requested and it does not violate the valid set of port
	 * configurations */
	if (pin_cfg & PORT_PINCFG_PULLEN) {
		/* Set the OUT register bits to enable the pullup if requested,
		 * clear to enable pull-down */
		if (config->input.pull == PORT_PIN_PULL_UP) {
			port_base->OUTSET.reg = pin_mask;
		} else {
			port_base->OUTCLR.reg = pin_mask;
		}
	}

	return STATUS_OK;
}
