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
void port_pin_set_config(
		const uint8_t gpio_pin,
		const struct port_pin_conf *const config)
{
	struct system_pinmux_pin_conf pinmux_config;
	system_pinmux_pin_get_config_defaults(&pinmux_config);

	pinmux_config.mux_position        = SYSTEM_PINMUX_GPIO;
	pinmux_config.direction           = config->direction;
	pinmux_config.input.pull          = config->input.pull;
	pinmux_config.input.sampling_mode = config->input.sampling_mode;
	pinmux_config.output.strength     = config->output.strength;
	pinmux_config.output.slew_rate    = config->output.slew_rate;
	pinmux_config.output.drive        = config->output.drive;

	system_pinmux_pin_set_config(gpio_pin, &pinmux_config);
}
