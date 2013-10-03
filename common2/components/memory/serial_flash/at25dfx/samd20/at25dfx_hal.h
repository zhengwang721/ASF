/**
 * \file
 *
 * \brief Connection of the AT25DFx SerialFlash to SPI interface driver.
 *
 * This file manages the connection of the AT25DFx SerialFlash driver to an
 * SPI service (could be spi master service or usart in spi mode). The SPI
 * service selection depends on  AT25DFX_USES_SPI_MASTER_SERVICE or
 * AT25DFX_USES_USART_SPI_SERVICE in conf_at25dfx.h.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#ifndef AT25DFX_HAL_H
#define AT25DFX_HAL_H

#include <spi.h>

struct at25dfx_spi_module {
	volatile bool locked;
	struct spi_module spi_module;
};

struct at25dfx_chip_module {
	struct at25dfx_spi_module *spi;
	enum at25dfx_type type;
	uint8_t cs_pin;
};

struct at25dfx_spi_config {
	/** GCLK generator to use as clock source. */
	enum gclk_generator generator_source;
	/** Baud rate */
	uint32_t baudrate;
	/** Mux setting */
	enum spi_signal_mux_setting mux_setting;
	/** PAD0 pinmux */
	uint32_t pinmux_pad0;
	/** PAD1 pinmux */
	uint32_t pinmux_pad1;
	/** PAD2 pinmux */
	uint32_t pinmux_pad2;
	/** PAD3 pinmux */
	uint32_t pinmux_pad3;
};

struct at25dfx_chip_config {
	enum at25dfx_type type;
	uint8_t cs_pin;
};

static inline void at25dfx_spi_get_config_defaults(
		struct at25dfx_spi_config *const config)
{
	// Defaults copied from spi_get_config_defaults
	config->generator_source = GCLK_GENERATOR_0;
	config->baudrate = 100000;
	config->mux_setting = SPI_SIGNAL_MUX_SETTING_D;
	config->pinmux_pad0 = PINMUX_DEFAULT;
	config->pinmux_pad1 = PINMUX_DEFAULT;
	config->pinmux_pad2 = PINMUX_DEFAULT;
	config->pinmux_pad3 = PINMUX_DEFAULT;
}

static inline enum status_code at25dfx_spi_init(
		struct at25dfx_spi_module *const module,
		Sercom *const hw,
		const struct at25dfx_spi_config *const config)
{
	struct spi_config spi_config;

	spi_get_config_defaults(&spi_config);

	spi_config.mode = SPI_MODE_MASTER;

	// Now get user config from at25dfx_spi_config
	spi_config.generator_source = config->generator_source;
	spi_config.mode_specific.master.baudrate = config->baudrate;
	spi_config.pinmux_pad0 = config->pinmux_pad0;
	spi_config.pinmux_pad1 = config->pinmux_pad1;
	spi_config.pinmux_pad2 = config->pinmux_pad2;
	spi_config.pinmux_pad3 = config->pinmux_pad3;
	spi_config.mux_setting = config->mux_setting;

	return spi_init(&(module->spi_module), hw, &spi_config);
}

static inline enum status_code at25dfx_chip_init(
		struct at25dfx_chip_module *const module,
		struct at25dfx_spi_module *const spi_module,
		const struct at25dfx_chip_config *const config)
{
	module->type = config->type;
	module->cs_pin = config->cs_pin;
	module->spi = spi_module;

	return STATUS_OK;
}

#endif // AT25DFX_HAL_H