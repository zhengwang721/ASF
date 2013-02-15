/**
 * \file
 *
 * \brief SAMD20 EEPROM Emulator
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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
#ifndef EEPROM_EMULATOR_H_INCLUDED
#define EEPROM_EMULATOR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>

/**
 * \defgroup asfdoc_samd20_eeprom_group SAMD20 EEPROM Emulator Service (EEPROM)
 *
 * This driver for SAMD20 devices provides a TODO.
 *
 *
 * The following peripherals are used by this module:
 *
 *  - NVM (Non-Volatile Memory Controller)
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_samd20_eeprom_prerequisites
 *  - \ref asfdoc_samd20_eeprom_module_overview
 *  - \ref asfdoc_samd20_eeprom_special_considerations
 *  - \ref asfdoc_samd20_eeprom_extra_info
 *  - \ref asfdoc_samd20_eeprom_examples
 *  - \ref asfdoc_samd20_eeprom_api_overview
 *
 *
 * \section asfdoc_samd20_eeprom_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samd20_eeprom_module_overview Module Overview
 *
 * TODO
 *
 *
 * \section asfdoc_samd20_eeprom_special_considerations Special Considerations
 *
 * TODO
 *
 *
 * \section asfdoc_samd20_eeprom_extra_info Extra Information for EEPROM Emulator
 *
 * For extra information see \ref asfdoc_samd20_eeprom_extra. This includes:
 *  - \ref asfdoc_samd20_eeprom_extra_acronyms
 *  - \ref asfdoc_samd20_eeprom_extra_dependencies
 *  - \ref asfdoc_samd20_eeprom_extra_errata
 *  - \ref asfdoc_samd20_eeprom_extra_history
 *
 *
 * \section asfdoc_samd20_eeprom_examples Examples
 *
 * The following Quick Start guides and application examples are available for this driver:
 * - \ref asfdoc_samd20_eeprom_basic_use_case
 *
 *
 * \section asfdoc_samd20_eeprom_api_overview API Overview
 * @{
 */

#if !defined(__DOXYGEN__)
#  define EEPROM_MAJOR_VERSION        1
#  define EEPROM_MINOR_VERSION        0
#  define EEPROM_REVISION             0
#  define EEPROM_EMULATOR_ID          1
#  define EEPROM_MASTER_PAGE_NUMBER   (EEPROM_MAX_PAGES - 1)

#  define EEPROM_PAGE_NUMBER_BYTE     0
#  define EEPROM_INVALID_PAGE_NUMBER  0xff
#  define EEPROM_HEADER_SIZE          2
#endif

/** Size of each logical EEPROM page, in bytes. */
#define EEPROM_DATA_SIZE              (NVMCTRL_PAGE_SIZE - EEPROM_HEADER_SIZE)

/** Maximum number of physical EEPROM pages that can be reserved. */
#define EEPROM_MAX_PAGES              (64 * NVMCTRL_ROW_PAGES)

enum status_code eeprom_emulator_init(void);

enum status_code eeprom_emulator_write_page(
		uint8_t lpage,
		uint8_t *data);


enum status_code eeprom_emulator_read_page(
		uint8_t lpage,
		uint8_t *data);

enum status_code eeprom_emulator_flush_page_buffer(void);

void eeprom_emulator_erase_memory(void);

enum status_code eeprom_emulator_write_buffer(
		uint16_t offset,
		uint8_t *data,
		uint16_t lenght);

enum status_code eeprom_emulator_read_buffer(
		uint16_t offset,
		uint8_t *data,
		uint16_t length);

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page asfdoc_samd20_eeprom_extra Extra Information
 *
 * \section asfdoc_samd20_eeprom_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>EEPROM</td>
 *		<td>Electronically Erasable Read-Only Memory</td>
 *	</tr>
 *	<tr>
 *		<td>NVM</td>
 *		<td>Non-Volatile Memory</td>
 *	</tr>
 * </table>
 *
 *
 * \section asfdoc_samd20_eeprom_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - \ref asfdoc_samd20_nvm_group "Non-Volatile Memory Controller Driver"
 *
 *
 * \section asfdoc_samd20_eeprom_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samd20_eeprom_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_samd20_eeprom_exqsg Examples for Emulated EEPROM service
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samd20_eeprom_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_samd20_eeprom_basic_use_case
 */

#endif
