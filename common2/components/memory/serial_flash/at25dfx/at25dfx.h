/**
 * \file
 *
 * \brief AT25DFx SerialFlash driver interface.
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

#ifndef AT25DFX_H
#define AT25DFX_H

/**
 * \defgroup asfdoc_common2_at25dfx_group AT25DFx SerialFlash Driver
 *
 * This driver provides an interface for basic usage of SerialFlash devices,
 * i.e., read/write, global and local protect/unprotect of sectors, and
 * sleep/wake.
 *
 * It supports multiple instances, and the instances can be connected to either
 * common or individual SPI interfaces. For a list of the supported SerialFlash
 * types, see \ref at25dfx_type.
 *
 * The following peripherals are used by this module:
 * - \ref SERCOM SPI (data transfer)
 * - \ref PORT (chip selection)
 *
 * The outline of this documentation is as follows:
 * - \ref asfdoc_common2_at25dfx_prerequisites
 * - \ref asfdoc_common2_at25dfx_module_overview
 * - \ref asfdoc_common2_at25dfx_special_considerations
 * - \ref asfdoc_common2_at25dfx_extra_info
 * - \ref asfdoc_common2_at25dfx_examples
 * - \ref asfdoc_common2_at25dfx_api_overview
 *
 *
 * \section asfdoc_common2_at25dfx_prerequisites Prerequisites
 *
 * This driver requires that the SPI interfaces are instantiated, initialized
 * and enabled by the user, and that the instances are persistent for as long as
 * the instances of this driver exist.
 *
 * However, the configuration to use is supplied by this driver with
 * \ref at25dfx_spi_get_config_defaults(). To ensure correct operation, the user
 * should not change anything but the baud rate, the SERCOM MUX and the pin MUX
 * settings in the supplied configuration.
 *
 * Persistence of the SPI instances can be ensured by, e.g., defining them in
 * the same scope as the instances of this driver, or by simply making them
 * global.
 *
 *
 * \section asfdoc_common2_at25dfx_module_overview Module Overview
 *
 * This driver enables the user to do basic operations on a SerialFlash device,
 * such as checking its presence, reading from and writing to it, putting it to
 * sleep and waking it back up when needed.
 *
 * Functionality for permanently locking and securing the SerialFlash is not
 * implemented in this driver. Only basic security is implemented, i.e.,
 * sector protection to prevent accidental erase and write.
 *
 * Further, this driver is built upon the APIs of the SERCOM SPI and PORT
 * drivers. The API is blocking, meaning the functions do not return until the
 * requested operation is done. It is also thread-safe since it uses the SPI
 * instance lock mechanism to ensure that no two operations can interfere with
 * each other.
 *
 *
 * \section asfdoc_common2_at25dfx_special_considerations Special Considerations
 *
 * The available erase block sizes, and the availability and granularity of
 * protection sectors may differ between devices.
 *
 * For example, with AT25F512B (\ref AT25DFX_512B), issuing a 64 kB block erase
 * command will cause a 32 kB erase, and it does not support setting protection
 * on individual sectors, only globally.
 *
 * Refer to the SerialFlash device's datasheet for details about which
 * operations are supported and what limitations apply to them.
 *
 * The user should also be aware of some details of how flash memory generally
 * functions:
 * -# Erasing a flash memory block means that all bits in the block are reset to
 * a high level (logical "1"), i.e., all bytes will read as \c 0xFF afterwards.
 * -# Writing to a flash memory location will only set relevant bits to a low
 * level (logical "0").
 * -# It is possible to write several times to a flash memory block without
 * erasing, but there may be limitations on the number of times it can be done.
 *
 *
 * \section asfdoc_common2_at25dfx_extra_info Extra Information
 *
 * For extra information, see
 * \subpage asfdoc_common2_at25dfx_extra. This includes:
 * - \ref asfdoc_common2_at25dfx_extra_acronyms
 * - \ref asfdoc_common2_at25dfx_extra_dependencies
 * - \ref asfdoc_common2_at25dfx_extra_errata
 * - \ref asfdoc_common2_at25dfx_extra_history
 *
 * \section asfdoc_common2_at25dfx_examples Examples
 *
 * For a list of examples related to this driver, see
 * \subpage asfdoc_common2_at25dfx_exqsg.
 *
 * \section asfdoc_common2_at25dfx_api_overview API Overview
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

//! Size of block to erase.
enum at25dfx_block_size {
	//! 4 kiloByte block size.
	AT25DFX_BLOCK_SIZE_4KB,
	//! 32 kiloByte block size.
	AT25DFX_BLOCK_SIZE_32KB,
	//! 64 kiloByte block size.
	AT25DFX_BLOCK_SIZE_64KB,
};

//! SerialFlash internal address.
typedef uint32_t at25dfx_address_t;

//! Length of data package to read/write.
typedef uint16_t at25dfx_datalen_t;

//! SerialFlash chip driver instance.
struct at25dfx_chip_module {
	//! SPI module to use.
	at25dfx_spi_module_t *spi;
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

//! \name Initialization
//@{

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
		at25dfx_spi_module_t *const spi_module,
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

//@}

#ifdef __cplusplus
extern "C" {
#endif

//! \name Presence
//@{
enum status_code at25dfx_chip_check_presence(struct at25dfx_chip_module *chip);
//@}

//! \name Read/Write
//@{
enum status_code at25dfx_chip_read_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, void *data, at25dfx_datalen_t length);
enum status_code at25dfx_chip_write_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, const void *data, at25dfx_datalen_t length);
//@}

//! \name Erase
//@{
enum status_code at25dfx_chip_erase(struct at25dfx_chip_module *chip);
enum status_code at25dfx_chip_erase_block(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, enum at25dfx_block_size block_size);
//@}

//! \name Sector Protect
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

/** @} */

/**
 * \page asfdoc_common2_at25dfx_extra Extra Information for AT25DFx SerialFlash Driver
 *
 *
 * \section asfdoc_common2_at25dfx_extra_acronyms Acronyms
 *
 * The table below presents the acronyms used in this module.
 *
 * <table>
 *   <tr>
 *     <th>Acronym</td>
 *     <th>Description</td>
 *   </tr>
 *   <tr>
 *     <td>SPI</td>
 *     <td>Serial Peripheral Interface</td>
 *   </tr>
 *   <tr>
 *     <td>CS</td>
 *     <td>Chip Select</td>
 *   </tr>
 * </table>
 *
 *
 * \section asfdoc_common2_at25dfx_extra_dependencies Dependencies
 *
 * This driver has the following dependencies:
 * - \ref asfdoc_samd20_sercom_spi_group "SERCOM SPI driver"
 * - \ref asfdoc_samd20_port_group "PORT driver"
 * - \ref group_sam0_utils "Compiler driver"
 *
 *
 * \section asfdoc_common2_at25dfx_extra_errata Errata
 *
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_common2_at25dfx_extra_history Module History
 *
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in the
 * table.
 *
 * <table>
 *   <tr>
 *     <th>Changelog</th>
 *   </tr>
 *   <tr>
 *     <td>Initial Release</td>
 *   </tr>
 * </table>
 */

/**
 * \page asfdoc_common2_at25dfx_exqsg Examples for AT25DFx SerialFlash Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_common2_at25dfx_group. QSGs are
 * simple examples with step-by-step instructions to configure and use this
 * driver in a selection of use cases. Note that QSGs can be compiled as a
 * standalone application or be added to the user application.
 *
 * - \subpage asfdoc_common2_at25dfx_basic_use
 */

/**
 * \page asfdoc_common2_at25dfx_document_revision_history Document Revision History
 *
 * <table>
 * <tr>
 * <th>Doc. Rev.</td>
 * <th>Date</td>
 * <th>Comments</td>
 * </tr>
 * <tr>
 * <td>A</td>
 * <td>XX/2013</td>
 * <td>Initial release</td>
 * </tr>
 * </table>
 */

#endif // AT25DFX_H