/**
 * \file
 *
 * \brief API driver for AT25DFx SerialFlash component.
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

#include <compiler.h>

enum at25dfx_type {
	AT25DFX_0161,
	AT25DFX_021,
	AT25DFX_041A,
	AT25DFX_081A,
	AT25DFX_161,
	AT25DFX_161A,
	AT25DFX_321,
	AT25DFX_321A,
	AT25DFX_512B,
	AT25DFX_641A,
/* Adesto's devices:
	AT25DF_041A,
	AT25DF_081A,
	AT25DF_321A,
	AT25DF_641,
	AT25DF_641A,
	AT25DL_081,
	AT25F_512B,
*/
};

#include <at25dfx_hal.h>

/**
 * Size of block to erase.
 *
 * \note Not all chips support 64KB block erase.
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
	struct spi_module *spi;
	enum at25dfx_type type;
	uint8_t cs_pin;
};

//! SerialFlash chip configuration.
struct at25dfx_chip_config {
	enum at25dfx_type type;
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
	module->type = config->type;
	module->cs_pin = config->cs_pin;
	module->spi = spi_module;

	return STATUS_OK;
}

#ifdef __cplusplus
extern "C" {
#endif

enum status_code at25dfx_chip_check_presence(struct at25dfx_chip_module *chip);
enum status_code at25dfx_chip_read_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, void *data, at25dfx_datalen_t length);
enum status_code at25dfx_chip_write_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, void *data, at25dfx_datalen_t length);
enum status_code at25dfx_chip_erase(struct at25dfx_chip_module *chip);
enum status_code at25dfx_chip_block_erase(struct at25dfx_chip_module *chip,
		uint32_t address, enum at25dfx_block_size block_size);
enum status_code at25dfx_chip_set_global_sector_protect(
		struct at25dfx_chip_module *chip, bool protect);
enum status_code at25dfx_chip_set_sector_protect(
		struct at25dfx_chip_module *chip, at25dfx_address_t address,
		bool protect);
enum status_code at25dfx_chip_get_sector_protect(
		struct at25dfx_chip_module *chip, at25dfx_address_t address,
		bool *protect);

#ifdef __cplusplus
}
#endif

#endif // AT25DFX_H