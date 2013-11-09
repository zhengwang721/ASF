/**
 * \file
 *
 * \brief AT25DFx SerialFlash driver interface.
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

#ifndef AT25DFX_H
#define AT25DFX_H

/**
 * \defgroup asfdoc_common2_at25dfx_group AT25DFx SerialFlash Driver
 *
 * This driver provides an interface for basic use of SerialFlash devices.
 *
 * @{
 */

#include <compiler.h>

//! SerialFlash type.
enum at25dfx_type {
	//! AT25F512B, compatible with AT25BCM512B
	AT25DFX_512B,
	//! AT25DF021
	AT25DFX_021,
	//! AT25DF041A
	AT25DFX_041A,
	//! AT25DF081, compatible with AT25DL081
	AT25DFX_081,
	//! AT25DF081A, compatible with AT26DF081A
	AT25DFX_081A,
	//! AT25DF161
	AT25DFX_161,
	//! AT25DL161
	AT25DFX_L161,
	//! AT25DQ161
	AT25DFX_Q161,
	//! AT25DF321A
	AT25DFX_321A,
	//! AT25DF641, compatible with AT25DF641A
	AT25DFX_641,
};

#include <at25dfx_hal.h>

/**
 * Size of block to erase.
 *
 * \note A 64kB block erase results in a 32kB erase on AT25DFX_512B.
 */
enum at25dfx_block_size {
	//! 4 kiloByte block size.
	AT25DFX_BLOCK_SIZE_4KB,
	//! 32 kiloByte block size.
	AT25DFX_BLOCK_SIZE_32KB,
	//! 64 kiloByte block size.
	AT25DFX_BLOCK_SIZE_64KB,
};

//! SerialFlash internal address
typedef uint32_t at25dfx_address_t;

//! Length of data package to read/write
typedef uint16_t at25dfx_datalen_t;

//! SerialFlash chip driver instance.
struct at25dfx_chip_module {
	//! SPI module to use.
	struct spi_module *spi;
	//! Type of SerialFlash
	enum at25dfx_type type;
	//! Chip Select (CS) pin.
	uint8_t cs_pin;
};

//! SerialFlash chip configuration.
struct at25dfx_chip_config {
	//! Type of SerialFlash
	enum at25dfx_type type;
	//! Chip Select (CS) pin.
	uint8_t cs_pin;
};

/**
 * \brief Initialize chip driver instance.
 *
 * This function initializes a chip instance and associates it with a specified
 * SPI instance.
 *
 * \param[out] module Pointer to the chip instance to initialize.
 * \param[in] spi_module Pointer to the SPI instance to associate with.
 * \param[in] config Pointer to the configuration for the chip.
 */
static inline enum status_code at25dfx_chip_init(
		struct at25dfx_chip_module *const module,
		struct spi_module *const spi_module,
		const struct at25dfx_chip_config *const config)
{
	struct port_config port_config;

	module->type = config->type;
	module->cs_pin = config->cs_pin;
	module->spi = spi_module;

	// Configure CS pin as output, high
	port_get_config_defaults(&port_config);
	port_config.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(module->cs_pin, &port_config);
	port_pin_set_output_level(module->cs_pin, true);

	return STATUS_OK;
}

#ifdef __cplusplus
extern "C" {
#endif

enum status_code at25dfx_chip_check_presence(struct at25dfx_chip_module *chip);

//! \name Read/Write
//@{
enum status_code at25dfx_chip_read_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, void *data, at25dfx_datalen_t length);
enum status_code at25dfx_chip_write_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, void *data, at25dfx_datalen_t length);
//@}

//! \name Erase
//@{
enum status_code at25dfx_chip_erase(struct at25dfx_chip_module *chip);
enum status_code at25dfx_chip_block_erase(struct at25dfx_chip_module *chip,
		uint32_t address, enum at25dfx_block_size block_size);
//@}

/**
 * \name Sector Protect
 *
 * \note See the datasheet for details about the availability and granularity
 * of sector-level protection. Global-level protection is always available.
 */
//@{
enum status_code at25dfx_chip_set_global_sector_protect(
		struct at25dfx_chip_module *chip, bool protect);
enum status_code at25dfx_chip_set_sector_protect(
		struct at25dfx_chip_module *chip, at25dfx_address_t address,
		bool protect);
enum status_code at25dfx_chip_get_sector_protect(
		struct at25dfx_chip_module *chip, at25dfx_address_t address,
		bool *protect);
//@}

//! \name Sleep/Wake
//@{
enum status_code at25dfx_chip_sleep(struct at25dfx_chip_module *chip);
enum status_code at25dfx_chip_wake(struct at25dfx_chip_module *chip);
//@}

#ifdef __cplusplus
}
#endif

/** @{ */

#endif // AT25DFX_H