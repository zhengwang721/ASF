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
 * This driver for SAMD20 devices provides a en emulated EEPROM memory space,
 * for the storage and retrieval of configuration data into and out of
 * non-volatile memory.
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
 * This module provides an EEPROM emulation layer on top of the device's raw
 * NVM controller, to provide a standard interface for the reading and writing
 * of non-volatile configuration data.
 *
 * A basic wear-leveling algorithm is implemented to automatically handle the
 * transferral of data across flash rows to ensure that repeated writes to
 * locations in the same logical EEPROM page do not exhaust the physical memory
 * write cycle lifespan.
 *
 * \subsection asfdoc_samd20_eeprom_module_overview_implementation Implementation Details
 * The following information is relevant for EEPROM emulator scheme 1, version
 * 1.0.0 as implemented by this module. Other revisions or emulation schemes may
 * vary in their implementation details.
 *
 * The SAMD20 non-volatile FLASH is divided into a number of rows, each
 * containing four identically sized flash pages. Pages may be read or written
 * to individually, however pages must be erased before being reprogrammed and
 * the smallest granularity available for erasure is one single row.
 *
 * This discrepancy results in the need for a emulator scheme that is able to
 * handle the versioning and moving of page data to different physical rows as
 * needed, erasing old rows ready for re-use by future page write operations.
 *
 * \subsubsection asfdoc_samd20_eeprom_module_overview_implementation_mp Master Page
 * One row, at the end of the emulated EEPROM memory space, is reserved for use
 * by the emulator to store configuration data. This includes a magic identifier
 * to indicated an initialized emulated EEPROM memory, as well as version
 * information and other relevant data. The master page is not user-accessible,
 * and is reserved solely for internal use by the emulator.
 *
 * \subsubsection asfdoc_samd20_eeprom_module_overview_implementation_sr Spare Row
 * As data needs to be preserved between row erasures, a spare row is tracked in
 * the NVM memory space to act as a destination for copied data when a write
 * request is made to a full row. When the write request is made any logical
 * pages in the full row that needs to be preserved is written to the spare row
 * along with the new (updated) logical page data, before the old row is erased
 * and marked as the new spare.
 *
 * \subsubsection asfdoc_samd20_eeprom_module_overview_implementation_rc Row Contents
 * Each physical row contains two logical EEPROM memory pages. This halves the
 * available storage space for the emulated EEPROM but reduces the overall
 * number of row erases that are required, by reserving two pages within each
 * row for updated versions of the logical page contents. As logical pages
 * within a physical row are updated, the new data is filled into the remaining
 * unused pages in the row. Once the entire row is full, a new write request
 * will copy the logical page not being written to in the current row to the
 * spare page with the new (updated) logical page data, before the old row is
 * erased.
 *
 * This system allow for the same logical page to be updated up to three times
 * into physical memory before a row erasure procedure is needed. In the case of
 * multiple versions of the same logical page being stored in the same physical
 * row, the right-most (highest physical page address) version is considered the
 * most current.
 *
 * \subsubsection asfdoc_samd20_eeprom_module_overview_implementation_wc Write Cache
 * As a typical EEPROM use case is to write to multiple sections of the same
 * EEPROM page sequentially, the emulator is optimized with a single logical
 * EEPROM page write cache to buffer writes before they are written to the
 * physical backing memory store. The cache is automatically flushed when a new
 * write request to a different logical EEPROM memory page is requested, or when
 * the user manually flushes the write cache.
 *
 * Without the write cache, each write request to a EEPROM memory page would
 * require a full page write, reducing the system performance and significantly
 * reducing the lifespan of the non-volatile memory.
 *
 * \section asfdoc_samd20_eeprom_special_considerations Special Considerations
 *
 * \subsection asfdoc_samd20_eeprom_special_considerations_wearlevel Wear Leveling Algorithm
 * The wear leveling algorithm is tuned to achieve best performance (and minimal
 * physical flash writes) when data is written to locations within the same
 * logical EEPROM page. The user application should ensure that wherever
 * possible subsequent data writes are restricted to the same logical page
 * address to prevent premature flash write cycle exhaustion.
 *
 * \subsection asfdoc_samd20_eeprom_special_considerations_pagesize Logical EEPROM Page Size
 * Due to the requirement of a header before the contents of each logical EEPROM
 * page, the available data in each EEPROM page is less than the total size of
 * a single NVM memory page by several bytes.
 *
 * \subsection asfdoc_samd20_eeprom_special_considerations_flushing Flushing of the Write Cache
 * A single-page write cache is used internally to buffer data written to pages
 * in order to reduce the number of physical writes required to store the user
 * data, and to preserve the physical memory lifespan. As a result, it is
 * important that the write cache is flushed to physical memory <b>as soon as
 * possible after a BOD low power condition</b>, to ensure that enough power is
 * available to guarantee a completed write so that no data is lost.
 *
 * The write cache must also be manually flushed to physical memory if the user
 * application is to perform any NVM operations using the NVM controller
 * directly.
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
#  define EEPROM_MASTER_PAGE_NUMBER   (EEPROM_MAX_PAGES - 1)
#  define EEPROM_INVALID_PAGE_NUMBER  0xFF
#  define EEPROM_HEADER_SIZE          4
#endif


/** \name EEPROM emulator information
 * @{
 */

/** Emulator scheme ID, identifying the scheme used to emulated EEPROM storage. */
#define EEPROM_EMULATOR_ID          1

/** Emulator major version number, identifying the emulator major version. */
#define EEPROM_MAJOR_VERSION        1

/** Emulator minor version number, identifying the emulator minor version. */
#define EEPROM_MINOR_VERSION        0

/** Emulator revision version number, identifying the emulator revision. */
#define EEPROM_REVISION             0

/** @} */


/** \name Emulated EEPROM size information
 * @{
 */

/** Size of each logical EEPROM page, in bytes. */
#define EEPROM_DATA_SIZE            (NVMCTRL_PAGE_SIZE - EEPROM_HEADER_SIZE)

/** Maximum number of physical EEPROM pages that can be reserved. */
#define EEPROM_MAX_PAGES            (64 * NVMCTRL_ROW_PAGES)

/** @} */


/** \name Configuration and initialization
 * @{
 */

enum status_code eeprom_emulator_init(void);

void eeprom_emulator_erase_memory(void);

/** @} */


/** \name Logical EEPROM page reading/writing
 * @{
 */

enum status_code eeprom_emulator_flush_page_buffer(void);

enum status_code eeprom_emulator_write_page(
		uint8_t logical_page,
		uint8_t *data);


enum status_code eeprom_emulator_read_page(
		uint8_t logical_page,
		uint8_t *data);

/** @} */

/** \name Buffer EEPROM reading/writing
 * @{
 */

enum status_code eeprom_emulator_write_buffer(
		uint16_t offset,
		uint8_t *data,
		uint16_t length);

enum status_code eeprom_emulator_read_buffer(
		uint16_t offset,
		uint8_t *data,
		uint16_t length);

/** @} */

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
