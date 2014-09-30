/**
 * \file
 *
 * \brief SAM L21 Write While Read EEPROM Emulator
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
#ifndef WWR_EEPROM_H_INCLUDED
#define WWR_EEPROM_H_INCLUDED

/**
 * \defgroup asfdoc_sam0_wwr_eeprom_group SAM L21 Write While Read EEPROM (WWR EEPROM) Emulator Service
 *
 * This driver for Atmel® | SMART™ SAM L21 provides a WWR emulated EEPROM
 * memory area, for the storage and retrieval of user-application
 * configuration data into and out of non-volatile memory. The main array
 * can therefore run code while eeprom data is written.
 *
 * The following peripherals are used by this module:
 *  - NVM (Non-Volatile Memory Controller)
 *
 * The following devices can use this module:
 *  - SAM L21
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam0_wwr_eeprom_prerequisites
 *  - \ref asfdoc_sam0_wwr_eeprom_module_overview
 *  - \ref asfdoc_sam0_wwr_eeprom_special_considerations
 *  - \ref asfdoc_sam0_wwr_eeprom_extra_info
 *  - \ref asfdoc_sam0_wwr_eeprom_examples
 *  - \ref asfdoc_sam0_wwr_eeprom_api_overview
 *
 *
 * \section asfdoc_sam0_wwr_eeprom_prerequisites Prerequisites
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam0_wwr_eeprom_module_overview Module Overview
 *
 * SAM L21 embedds a separate write while read EEPROM emulation (WWREE) array
 * that can be programmed while the main array is not blocked.
 * To use WWREE memory, data must be written as a number of physical memory pages
 * (of several bytes each) rather than being individually byte addressable, and
 * entire rows of WWREE must be erased before new data may be stored.
 * To help abstract these characteristics away from the user application an
 * emulation scheme is implemented to present a more user-friendly API for
 * data storage and retrieval.
 *
 * This module provides a WWR EEPROM emulation layer on top of the device's
 * internal NVM controller, to provide a standard interface for the reading and
 * writing of non-volatile configuration data. This data is placed into the
 * WWR EEPROM emulated section. Emulated EEPROM is exempt from the usual device
 * NVM region lock bits, so that it may be read from or written to at any point
 * in the user application.
 *
 * There are many different algorithms that may be employed for EEPROM emulation,
 * to tune the write and read latencies, RAM usage, wear
 * levelling and other characteristics. As a result, multiple different emulator
 * schemes may be implemented, so that the most appropriate scheme for a
 * specific application's requirements may be used.
 *
 * \subsection asfdoc_sam0_wwr_eeprom_module_overview_implementation Implementation Details
 * The following information is relevant for <b>WWR EEPROM Emulator scheme 1,
 * version 1.0.0</b>, as implemented by this module. Other revisions or
 * emulation schemes may vary in their implementation details and may have
 * different wear-leveling, latency and other characteristics.
 *
 * \subsubsection asfdoc_sam0_wwr_eeprom_module_overview_implementation_ec Emulator Characteristics
 * This emulator is designed for <b>best reliability, with a good balance of
 * available storage and write-cycle limits</b>. It is designed to ensure that
 * page data is atomically updated so that in the event of a failed update the
 * previous data is not lost (when used correctly). With the exception of a
 * system reset with data cached to the internal write-cache buffer, at most
 * only the latest write to physical non-volatile memory will be lost in the
 * event of a failed write.
 *
 * This emulator scheme is tuned to give best write-cycle longevity when writes
 * are confined to the same logical WWR EEPROM page (where possible) and when writes
 * across multiple logical WWR EEPROM pages are made in a linear fashion through the
 * entire emulated WWR EEPROM space.
 *
 * \subsubsection asfdoc_sam0_wwr_eeprom_module_overview_implementation_pf Physical Memory
 * The SAM L21 WWR EEPROM is divided into a number of physical rows, each
 * containing four identically sized pages. Pages may be read or written
 * to individually, however pages must be erased before being reprogrammed and
 * the smallest granularity available for erasure is one single row.
 *
 * This discrepancy results in the need for an emulator scheme that is able to
 * handle the versioning and moving of page data to different physical rows as
 * needed, erasing old rows ready for re-use by future page write operations.
 *
 * Physically, the emulated WWR EEPROM segment is a dedicated space that are memory
 * mapped, as shown in
 * \ref asfdoc_sam0_wwr_eeprom_module_mem_layout "the figure below".
 *
 * \anchor asfdoc_sam0_wwr_eeprom_module_mem_layout
 * \dot
 * digraph memory_layout {
 *  size="5,5"
 *  node [shape=plaintext]
 *  memory [label=<
 *   <table border="0" cellborder="1" cellspacing="0" >
 *    <tr>
 *     <td align="right" border="0"> End of WWR EEPROM Memory </td>
 *     <td rowspan="3" align="center">WWR EEPROM </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> Start of WWR EEPROM Memory</td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> </td>
 *     <td rowspan="3" align="center"> Reserved </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"></td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> End of NVM Memory </td>
 *     <td rowspan="3" align="center"> Reserved EEPROM Section </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> Start of EEPROM Memory </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> End of Application Memory </td>
 *     <td rowspan="3" align="center"> Application Section </td>
 *    </tr>
 *    <tr>
 *     <td height="200" align="right" border="0"> </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> Start of Application Memory </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> End of Bootloader Memory </td>
 *     <td rowspan="3" align="center"> BOOT Section </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border="0"> Start of NVM Memory</td>
 *    </tr>
 *   </table>
 *  >]
 * }
 * \enddot
 *
 * \subsubsection asfdoc_sam0_wwr_eeprom_module_overview_implementation_mp Master Row
 * One physical row at the end of the emulated WWR EEPROM memory space is
 * reserved for use by the emulator to store configuration data. The master row
 * is not user-accessible, and is reserved solely for internal use by the
 * emulator.
 *
 * \subsubsection asfdoc_sam0_wwr_eeprom_module_overview_implementation_sr Spare Row
 * As data needs to be preserved between row erasures, a single row is kept
 * unused to act as destination for copied data when a write request is made to
 * an already full row. When the write request is made, any logical pages of
 * data in the full row that need to be preserved are written to the spare row
 * along with the new (updated) logical page data, before the old row is erased
 * and marked as the new spare.
 *
 * \subsubsection asfdoc_sam0_wwr_eeprom_module_overview_implementation_rc Row Contents
 * Each physical row initially stores the contents of one or two logical WWR EEPROM
 * memory pages(It depends on application configuration file). This quartered or 
 * halves the available storage space for the emulated WWR EEPROM
 * but reduces the overall number of row erases that are required, by reserving
 * one or two pages within each row for updated versions of the logical page contents.
 * See \ref asfdoc_sam0_wwr_eeprom_init_layout "here" for a visual layout of the WWR
 * EEPROM Emulator physical memory.
 *
 * As logical pages within a physical row are updated, the new data is filled
 * into the remaining unused pages in the row. Once the entire row is full, a
 * new write request will copy the logical page not being written to in the
 * current row to the spare row with the new (updated) logical page data, before
 * the old row is erased.
 *
 * When it's configured each physical row stores the contents of one logical WWR EEPROM
 * memory page, this system will allows for the same logical page to be updated up
 * to four times into physical memory before a row erasure procedure is needed. 
 * In the case of multiple versions of the same logical WWR EEPROM page being stored in
 * the same physical row, the right-most (highest physical memory page address)
 * version is considered to be the most current.
 *
 * \subsubsection asfdoc_sam0_wwr_eeprom_module_overview_implementation_wc Write Cache
 * As a typical EEPROM use case is to write to multiple sections of the same
 * EEPROM page sequentially, the emulator is optimized with a single logical
 * WWR EEPROM page write cache to buffer writes before they are written to the
 * physical backing memory store. The cache is automatically committed when a
 * new write request to a different logical WWR EEPROM memory page is requested, or
 * when the user manually commits the write cache.
 *
 * Without the write cache, each write request to an EEPROM memory page would
 * require a full page write, reducing the system performance and significantly
 * reducing the lifespan of the non-volatile memory.
 *
 * \subsection asfdoc_sam0_wwr_eeprom_special_considerations_memlayout Memory Layout
 * A single logical WWR EEPROM page is physically stored as the page contents and a
 * header inside a single physical page, as shown in
 * \ref asfdoc_sam0_wwr_eeprom_page_layout "the following figure".
 *
 * \anchor asfdoc_sam0_wwr_eeprom_page_layout
 * \image html page_layout.svg "Internal Layout of An Emulated WWR EEPROM age"
 *
 * \note In the following memory layout example, each physical row stores the 
 * contents of one logical WWR EEPROM page, you can refer to "AT03265: SAM EEPROM 
 * Emulator Service (EEPROM)" for the example of two logical EEPROM pages in one row.
 *
 * Within the WWR EEPROM memory reservation section at the top of the NVM memory
 * space, this emulator will produce the layout as shown in
 * \ref asfdoc_sam0_wwr_eeprom_init_layout "the figure below" when initialized for
 * the first time.
 *
 * \anchor asfdoc_sam0_wwr_eeprom_init_layout
 * \image html init_layout.svg "Initial Physical Layout of The Emulated WWR EEPROM Memory"
 *
 * When a WWR EEPROM page needs to be committed to physical memory, the next free
 * page in the same row will be chosen - this makes recovery simple, as the
 * right-most version of a logical page in a row is considered the most current.
 * With four pages to a physical NVM row, this allows for up to four updates to
 * the same logical page to be made before an erase is needed.
 * \ref asfdoc_sam0_wwr_eeprom_page_write1 "The figure below" shows the result of
 * the user writing an updated version of logical EEPROM page <tt>N-1</tt> to
 * the physical memory.
 *
 * \anchor asfdoc_sam0_wwr_eeprom_page_write1
 * \image html nm1_page_write.svg "First Write to Logical WWR EEPROM Page N-1"
 *
 * A second write of the same logical WWR EEPROM page results in the layout shown
 * in \ref asfdoc_sam0_wwr_eeprom_page_write2 "the figure below".
 *
 * \anchor asfdoc_sam0_wwr_eeprom_page_write2
 * \image html nm1_page_write2.svg "Second Write To Logical WWR EEPROM Page N-1"
 *
 * A third write of the same logical WWR EEPROM page results in the layout shown
 * in \ref asfdoc_sam0_wwr_eeprom_page_write3 "the figure below".
 *
 * \anchor asfdoc_sam0_wwr_eeprom_page_write3
 * \image html nm1_page_write3.svg "Third Write To Logical WWR EEPROM Page N-1"
 *
 * A fourth write of the same logical page requires that the WWR EEPROM emulator
 * erase the row, as it has become full. Prior to this, the contents of the
 * unmodified page in the same row as the page being updated will be copied into
 * the spare row, along with the new version of the page being updated. The old
 * (full) row is then erased, resulting in the layout shown in
 * \ref asfdoc_sam0_wwr_eeprom_page_write4 "the figure below".
 *
 * \anchor asfdoc_sam0_wwr_eeprom_page_write4
 * \image html nm1_page_write4.svg "Third Write To Logical WWR EEPROM Page N-1"
 *
 *
 * \section asfdoc_sam0_wwr_eeprom_special_considerations Special Considerations
 *
 * \subsection asfdoc_sam0_wwr_eeprom_special_considerations_nvm_config NVM Controller Configuration
 * The WWR EEPROM Emulator service will initialize the NVM controller as part of its
 * own initialization routine; the NVM controller will be placed in Manual Write
 * mode, so that explicit write commands must be sent to the controller to
 * commit a buffered page to physical memory. The manual write command must thus
 * be issued to the NVM controller whenever the user application wishes to write
 * to a NVM page for its own purposes.
 *
 * \subsection asfdoc_sam0_wwr_eeprom_special_considerations_pagesize Logical WWR EEPROM Page Size
 * As a small amount of information needs to be stored in a header before the
 * contents of a logical EEPROM page in memory (for use by the emulation
 * service), the available data in each WWR EEPROM page is less than the total size
 * of a single NVM memory page by several bytes.
 *
 * \subsection asfdoc_sam0_wwr_eeprom_special_considerations_committing Committing of the Write Cache
 * A single-page write cache is used internally to buffer data written to pages
 * in order to reduce the number of physical writes required to store the user
 * data, and to preserve the physical memory lifespan. As a result, it is
 * important that the write cache is committed to physical memory <b>as soon as
 * possible after a BOD low power condition</b>, to ensure that enough power is
 * available to guarantee a completed write so that no data is lost.
 *
 * The write cache must also be manually committed to physical memory if the
 * user application is to perform any NVM operations using the NVM controller
 * directly.
 *
 * \subsection asfdoc_sam0_wwr_eeprom_special_considerations_checksum WWR EEPROM Page Checksum
 * For each page, a checksum function is used to verify the integrity of 
 * the page data. After reading the page data using \ref wwr_eeprom_emulator_read_page() ,
 * When its checksum is not correct, and error can be detected.
 * This function can be enabled or disabled through configuration file.
 *
 * \section asfdoc_sam0_wwr_eeprom_extra_info Extra Information
 *
 * For extra information see \ref asfdoc_sam0_wwr_eeprom_extra. This includes:
 *  - \ref asfdoc_sam0_wwr_eeprom_extra_acronyms
 *  - \ref asfdoc_sam0_wwr_eeprom_extra_dependencies
 *  - \ref asfdoc_sam0_wwr_eeprom_extra_errata
 *  - \ref asfdoc_sam0_wwr_eeprom_extra_history
 *
 *
 * \section asfdoc_sam0_wwr_eeprom_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam0_wwr_eeprom_exqsg.
 *
 *
 * \section asfdoc_sam0_wwr_eeprom_api_overview API Overview
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>

#if !defined(__DOXYGEN__)
#  define WWR_EEPROM_MAX_PAGES            NVMCTRL_WWREE_PAGES
#  define WWR_EEPROM_MASTER_PAGE_NUMBER   (_eeprom_instance.physical_pages - 1)
#  define WWR_EEPROM_INVALID_PAGE_NUMBER  0xFF
#  define WWR_EEPROM_INVALID_ROW_NUMBER   (WWR_EEPROM_INVALID_PAGE_NUMBER / NVMCTRL_ROW_PAGES)
#  define WWR_EEPROM_HEADER_SIZE          4
#endif


/**
 * \brief WWR EEPROM Logical Page in Each Row.
 *
 * Enum for the possible logical pages that are stored in each physical row .
 *
 */
enum wwree_logical_page_num_in_row {
	/** One logical page stored in a physical row. */
	WWREE_LOGICAL_PAGE_NUM_1 = 1,
	/** Two logical pages stored in a physical row. */
	WWREE_LOGICAL_PAGE_NUM_2 = 2,
};


/** \name WWR EEPROM emulator information
 * @{
 */

/** Emulator scheme ID, identifying the scheme used to emulated EEPROM storage. */
#define WWR_EEPROM_EMULATOR_ID          1

/** Emulator major version number, identifying the emulator major version. */
#define WWR_EEPROM_MAJOR_VERSION        1

/** Emulator minor version number, identifying the emulator minor version. */
#define WWR_EEPROM_MINOR_VERSION        0

/** Emulator revision version number, identifying the emulator revision. */
#define WWR_EEPROM_REVISION             0

/** Size of the user data portion of each logical EEPROM page, in bytes. */
#define WWR_EEPROM_PAGE_SIZE            (NVMCTRL_PAGE_SIZE - WWR_EEPROM_HEADER_SIZE)

/**
 * \brief WWR EEPROM memory parameter structure.
 *
 * Structure containing the memory layout parameters of the EEPROM emulator module.
 */
struct wwr_eeprom_emulator_parameters {
	/** Number of bytes per emulated EEPROM page. */
	uint8_t  page_size;
	/** Number of emulated pages of EEPROM. */
	uint16_t eeprom_number_of_pages;
};

/** @} */

/** \name Configuration and initialization
 * @{
 */

enum status_code wwr_eeprom_emulator_init(void);

void wwr_eeprom_emulator_erase_memory(void);

enum status_code wwr_eeprom_emulator_get_parameters(
		struct wwr_eeprom_emulator_parameters *const parameters);

/** @} */


/** \name Logical WWR EEPROM page reading/writing
 * @{
 */

enum status_code wwr_eeprom_emulator_commit_page_buffer(void);

enum status_code wwr_eeprom_emulator_write_page(
		const uint8_t logical_page,
		const uint8_t *const data);

enum status_code wwr_eeprom_emulator_read_page(
		const uint8_t logical_page,
		uint8_t *const data);

/** @} */

/** \name Buffer WWR EEPROM reading/writing
 * @{
 */

enum status_code wwr_eeprom_emulator_write_buffer(
		const uint16_t offset,
		const uint8_t *const data,
		const uint16_t length);

enum status_code wwr_eeprom_emulator_read_buffer(
		const uint16_t offset,
		uint8_t *const data,
		const uint16_t length);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page asfdoc_sam0_wwr_eeprom_extra Extra Information
 *
 * \section asfdoc_sam0_wwr_eeprom_extra_acronyms Acronyms
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
 *		<td>WWREE</td>
 *		<td>Write While Read EEPROM</td>
 *	</tr>
  *	<tr>
 *		<td>WWR</td>
 *		<td>Write While Read</td>
 *	</tr>
 *	<tr>
 *		<td>NVM</td>
 *		<td>Non-Volatile Memory</td>
 *	</tr>
 * </table>
 *
 *
 * \section asfdoc_sam0_wwr_eeprom_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - \ref asfdoc_sam0_nvm_group "Non-Volatile Memory Controller Driver"
 *
 *
 * \section asfdoc_sam0_wwr_eeprom_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam0_wwr_eeprom_extra_history Module History
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
 * \page asfdoc_sam0_wwr_eeprom_exqsg Examples for Emulated WWR EEPROM service
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_sam0_wwr_eeprom_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam0_wwr_eeprom_basic_use_case
 *
 * \page asfdoc_sam0_wwr_eeprom_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</th>
 *		<th>Date</th>
 *		<th>Comments</th>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>10/2014</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#endif /* EEPROM_H_INCLUDED */
