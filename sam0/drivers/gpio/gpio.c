/**
 * \file
 *
 * \brief SAM GPIO GPIO Driver for SAMB11
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
#include <gpio.h>

/**
 *  \brief Initializes a gpio pin/group configuration structure to defaults.
 *
 *  Initializes a given gpio pin/group configuration structure to a set of
 *  known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Input mode with internal pullup enabled
 *
 *  \param[out] config  Configuration structure to initialize to default values.
 */
void gpio_get_config_defaults(struct gpio_config *const config)
{
	/* Default configuration values */
	config->direction  = GPIO_PIN_DIR_INPUT;
	config->input_pull = GPIO_PIN_PULL_UP;
	config->powersave  = false;
}

/**
 *  \brief Writes a gpio pin configuration to the hardware module.
 *
 *  Writes out a given configuration of a gpio pin configuration to the hardware
 *  module. If the configuration is NULL then it releases the gpio pin.
 *
 *  \note If the pin direction is set as an output, the pull-up/pull-down input
 *        configuration setting is ignored. Also certain gpio pin is used by
 *        FW and not available for user application. Please \ref gpio_pin
 *        for list of gpio_pin available.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to configure.
 *  \param[in] config    Configuration settings for the pin.
 *
 * 	\return Status of initialization.
 * 	\retval STATUS_OK                       gpio configured correctly
 * 	\retval STATUS_ERR_INVALID_ARG          Invalid gpio number, Certain gpios
 *                                          are used by FW and not allowed to change.
 * 	\retval STATUS_RESOURCE_NOT_AVAILABLE   Requested gpio is already in use.
 * 	\retval STATUS_ERR_INVALID_ARG          Invalid pull-up/pull-down configuration.
 *
 */
enum status_code gpio_pin_set_config(const uint8_t gpio_pin,
		const struct gpio_config *config)
{
	enum status_code status = STATUS_OK;

	/* Following GPIO's should never be modified by user.
	* GPIO_0 & GPIO_1 are used for SWD
	* GPIO_14 is used by firmware for coex.
	*/
	if ((gpio_pin == LPGPIO_0) || \
		(gpio_pin == LPGPIO_1) || \
		(gpio_pin == LPGPIO_14) ||
		(gpio_pin > 24))
	{
		status = STATUS_ERR_INVALID_ARG;
	} else {
		if (gpio_pin <= 7) {
			LPMCU_MISC_REGS0->PINMUX_SEL_0.reg &= ~(7 << ((gpio_pin % 8) * 4));
		} else if (gpio_pin <= 15) {
			LPMCU_MISC_REGS0->PINMUX_SEL_1.reg &= ~(7 << ((gpio_pin % 8) * 4));
		} else if (gpio_pin <= 23) {
			LPMCU_MISC_REGS0->PINMUX_SEL_2.reg &= ~(7 << ((gpio_pin % 8) * 4));
		}

		if(config->direction == GPIO_PIN_DIR_INPUT) {
			if(gpio_pin <= 7) {
				GPIO0->OUTENCLR.reg |= (1 << gpio_pin);
			} else if (gpio_pin <= 15) {
				GPIO1->OUTENCLR.reg |= (1 << (gpio_pin % 8));
			} else if (gpio_pin <= 23) {
				GPIO2->OUTENCLR.reg |= (1 << (gpio_pin % 16));
			}
			/* pull_enable. */
			switch(config->input_pull) {
				case GPIO_PIN_PULL_NONE:
					LPMCU_MISC_REGS0->PULL_ENABLE.reg |= (1 << gpio_pin);
					break;
				case GPIO_PIN_PULL_UP:
					LPMCU_MISC_REGS0->PULL_ENABLE.reg &= ~(1 << gpio_pin);
					break;
#ifdef CHIPVERSION_B0
				case GPIO_PIN_PULL_DOWN:
					/* Set R-Type */
					LPMCU_MISC_REGS0->RTYPE_PAD_0.reg |= (1 << gpio_pin);
					/* Sete REN */
					LPMCU_MISC_REGS0->PULL_ENABLE.reg &= ~(1 << gpio_pin);
					break;
#endif	//CHIPVERSION_B0
				default:
					status = STATUS_ERR_INVALID_ARG;
					break;
			}
		} else if(config->direction == GPIO_PIN_DIR_OUTPUT) {
			if (gpio_pin <= 7) {
				GPIO0->OUTENSET.reg |= (1 << gpio_pin);
			} else if (gpio_pin <= 15) {
				GPIO1->OUTENSET.reg |= (1 << (gpio_pin % 8));
			} else if (gpio_pin <= 23) {
				GPIO2->OUTENSET.reg |= (1 << (gpio_pin % 16));
			}
		}
	}
	return status;
}

/**
 *  \brief Retrieves the state of a gpio pin that is configured as an input.
 *
 *  Reads the current logic level of a gpio pin and returns the current
 *  level as a boolean value.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to read.
 *
 *  \return Status of the gpio pin's input buffer.
 */
bool gpio_pin_get_input_level(const uint8_t gpio_pin)
{
	uint32_t regval;

	if (gpio_pin <= 7) {
		regval = GPIO0->DATA.reg;
		return (regval & (1 << gpio_pin));
	} else if (gpio_pin <= 15) {
		regval = GPIO1->DATA.reg;
		return (regval & (1 << (gpio_pin % 8)));
	} else if (gpio_pin <= 23) {
		regval = GPIO2->DATA.reg;
		return (regval & (1 << (gpio_pin % 16)));
	}
}

/**
 *  \brief Retrieves the state of a gpio pin that is configured as an output.
 *
 *  Reads the current logical output level of a gpio pin and returns the current
 *  level as a boolean value.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to read.
 *
 *  \return Status of the gpio pin's output buffer.
 */
bool gpio_pin_get_output_level(const uint8_t gpio_pin)
{
	uint32_t regval;

	if (gpio_pin <= 7) {
		regval = GPIO0->DATAOUT.reg;
		return (regval & (1 << gpio_pin));
	} else if (gpio_pin <= 15) {
		regval = GPIO1->DATAOUT.reg;
		return (regval & (1 << (gpio_pin % 8)));
	} else if (gpio_pin <= 23) {
		regval = GPIO2->DATAOUT.reg;
		return (regval & (1 << (gpio_pin % 16)));
	}
}

/**
 *  \brief Sets the state of a gpio pin that is configured as an output.
 *
 *  Sets the current output level of a gpio pin to a given logic level.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to write to.
 *  \param[in] level     Logical level to set the given pin to.
 */
void gpio_pin_set_output_level(const uint8_t gpio_pin, const bool level)
{
	if(gpio_pin <= 7) {
		if(level) {
			GPIO0->DATAOUT.reg |= (1 << gpio_pin);
		} else {
			GPIO0->DATAOUT.reg &= ~(1 << gpio_pin);
		}
	} else if (gpio_pin <= 15) {
		if(level) {
			GPIO1->DATAOUT.reg |= (1 << (gpio_pin % 8));
		} else {
			GPIO1->DATAOUT.reg &= ~(1 << (gpio_pin % 8));
		}
	} else if (gpio_pin <= 23) {
		if(level) {
			GPIO2->DATAOUT.reg |= (1 << (gpio_pin % 16));
		} else {
			GPIO2->DATAOUT.reg &= ~(1 << (gpio_pin % 16));
		}
	}
}

/**
 *  \brief Toggles the state of a gpio pin that is configured as an output.
 *
 *  Toggles the current output level of a gpio pin.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to toggle.
 */
void gpio_pin_toggle_output_level(const uint8_t gpio_pin)
{
	if (gpio_pin <= 7) {
		GPIO0->DATAOUT.reg ^= (1 << gpio_pin);
	} else if (gpio_pin <= 15) {
		GPIO1->DATAOUT.reg ^= (1 << (gpio_pin % 8));
	} else if (gpio_pin <= 23) {
		GPIO2->DATAOUT.reg ^= (1 << (gpio_pin % 16));
	}
}

/**
 *  \brief Writes a GPIO pin configuration to the hardware module.
 *
 *  Writes out a given configuration of a GPIO pin configuration to the hardware
 *  module.
 *
 *  \param[in] gpio_pin   Index of the GPIO pin to toggle.
 *  \param[in] pinmux_sel PINMUX selection.
 */
void gpio_pinmux_cofiguration(const uint8_t gpio_pin, enum gpio_pinmux_sel pinmux_sel)
{
	if (gpio_pin <= 7) {
		LPMCU_MISC_REGS0->PINMUX_SEL_0.reg &= ~(7 << ((gpio_pin % 8) * 4));
		LPMCU_MISC_REGS0->PINMUX_SEL_0.reg |= (pinmux_sel << ((gpio_pin % 8)*4));
	} else if (gpio_pin <= 15) {
		LPMCU_MISC_REGS0->PINMUX_SEL_1.reg &= ~(7 << ((gpio_pin % 8) * 4));
		LPMCU_MISC_REGS0->PINMUX_SEL_1.reg |= (pinmux_sel << ((gpio_pin % 8)*4));
	} else if (gpio_pin <= 23) {
		LPMCU_MISC_REGS0->PINMUX_SEL_2.reg &= ~(7 << ((gpio_pin % 8) * 4));
		LPMCU_MISC_REGS0->PINMUX_SEL_2.reg |= (pinmux_sel << ((gpio_pin % 8)*4));
	}
}

