/**
 * \file
 *
 * \brief SAM SPI Flash Driver for SAMB11
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
#ifndef SPI_FLASH_H_INCLUDED
#define SPI_FLASH_H_INCLUDED

/**
 * \defgroup asfdoc_sam0_spi_flash_group SAM SPI Flash Driver (SPI Flash)
 *
 * This driver for Atmel&reg; | SMART SAM devices provides an interface for the
 * configuration and management of the device's SPI Flash functionality. The
 * following driver API modes are covered by this manual:
 *
 * The following peripherals are used by this module:
 *  - SPI Flash (SPI Flash Management)
 *
 * The following devices can use this module:
 *  - Atmel | SMART SAM B11
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam0_spi_flash_prerequisites
 *  - \ref asfdoc_sam0_spi_flash_module_overview
 *  - \ref asfdoc_sam0_spi_flash_special_considerations
 *  - \ref asfdoc_sam0_spi_flash_extra_info
 *  - \ref asfdoc_sam0_spi_flash_examples
 *  - \ref asfdoc_sam0_spi_flash_api_overview
 *
 *
 * \section asfdoc_sam0_spi_flash_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam0_spi_flash_module_overview Module Overview
 * The AHB SPI-Flash Controller is used to access the internal stacked FLASH
 * memory to access various instruction/data code needed for storing
 * application code, code patches, and OTA images.
 *
 *
 * \section asfdoc_sam0_spi_flash_special_considerations Special Considerations
 * There are no prerequisites for this module.
 *
 * \section asfdoc_sam0_spi_flash_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam0_spi_flash_extra. This includes:
 *  - \ref asfdoc_sam0_spi_flash_extra_acronyms
 *  - \ref asfdoc_sam0_spi_flash_extra_dependencies
 *  - \ref asfdoc_sam0_spi_flash_extra_errata
 *  - \ref asfdoc_sam0_spi_flash_extra_history
 *
 *
 * \section asfdoc_sam0_spi_flash_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam0_spi_flash_exqsg.
 *
 *
 * \section asfdoc_sam0_spi_flash_api_overview API Overview
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Flash memory size: 265 KBytes */
#define FLASH_MEMORY_SIZE       0x40000

/** Flash memory sector size 4 KBytes */
#define FLASH_SECTOR_SIZE       0x1000

/** Flash memory page size: 256 Bytes */
#define FLASH_PAGE_SIZE         0x100

/** Flash sector Mask */
#define FLASH_SECT_MASK         0xFFF000

/** Flash page mask */
#define FLASH_PAGE_MASK         0xFFFF00

#define AHB_DATA_MEM_BASE       0x10000000
#define GET_AHB_ADDRESS(x)      ((AHB_DATA_MEM_BASE + (unsigned long)x))

/**
@defgroup spi-flash-drv SPI FLASH Driver API

@{
*/

void spi_flash_init(void) ;
uint32_t spi_flash_rdid(void);
void spi_flash_read(void *read_buf, uint32_t flash_addr, uint32_t size);
int8_t spi_flash_write(void *write_buf, uint32_t flash_addr, uint32_t size);
void spi_flash_sector_erase(uint32_t flash_addr);
uint8_t spi_flash_erase(uint32_t start_offset, uint32_t size);
void spi_flash_enter_low_power_mode(void);
void spi_flash_leave_low_power_mode(void);

/** @} */

/** @}*/

#ifdef __cplusplus
}
#endif


/**
 * \page asfdoc_sam0_spi_flash_extra Extra Information for SPI Flash Driver
 *
 * \section asfdoc_sam0_spi_flash_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>SPI Flash</td>
 *		<td>General Purpose Input/Output</td>
 *	</tr>
 * </table>
 *
 *
 * \section asfdoc_sam0_spi_flash_extra_dependencies Dependencies
 * There are no dependencies related to this driver.
 *
 *
 * \section asfdoc_sam0_spi_flash_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam0_spi_flash_extra_history Module History
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
 * \page asfdoc_sam0_spi_flash_exqsg Examples for SPI Flash Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_sam0_spi_flash_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam0_spi_flash_use_case
 *
 * \page asfdoc_sam0_spi_flash_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>06/2013</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#endif
