/**
 * \file
 *
 * \brief I2C Common Driver for SAMB
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
#ifndef I2C_COMMON_H_INCLUDED
#define I2C_COMMON_H_INCLUDED

#include <compiler.h>
#include <system_sam_b.h>
#include <gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \if (I2C_MASTER_MODE && I2C_SLAVE_MODE)
 *   \defgroup asfdoc_samb_i2c_group SAM I2C Driver (I2C)
 * \elseif I2C_MASTER_MODE
 *   \defgroup asfdoc_samb_i2c_group SAM I2C Master Mode Driver (I2C)
 * \elseif I2C_SLAVE_MODE
 *   \defgroup asfdoc_samb_i2c_group SAM I2C Slave Mode Driver (I2C)
 * \endif
 *
 * This driver for Atmel&reg; | SMART SAM devices provides an interface for the configuration
 * and management of the device's I<SUP>2</SUP>C module, for the transfer
 * of data via an I<SUP>2</SUP>C bus. The following driver API modes are covered
 * by this manual:
 *
 * \if I2C_MASTER_MODE
 * - Master Mode Polled APIs
 * \endif
 * \if I2C_MASTER_CALLBACK_MODE
 * - Master Mode Callback APIs
 * \endif
 * \if I2C_SLAVE_MODE
 * - Slave Mode Polled APIs
 * \endif
 * \if I2C_SLAVE_CALLBACK_MODE
 * - Slave Mode Callback APIs
 * \endif
 *
 * The following peripheral is used by this module:
 * - Serial Communication Interface
 *
 * The following devices can use this module:
 *  - Atmel | SMART SAM B11
 *
 * The outline of this documentation is as follows:
 * - \ref asfdoc_samb_i2c_prerequisites
 * - \ref asfdoc_samb_i2c_overview
 * - \ref asfdoc_samb_i2c_special_considerations
 * - \ref asfdoc_samb_i2c_extra
 * - \ref asfdoc_samb_i2c_examples
 * - \ref asfdoc_samb_i2c_api_overview
 *
 * \section asfdoc_samb_i2c_prerequisites Prerequisites
 * There are no prerequisites.
 *
 * \section asfdoc_samb_i2c_overview Module Overview
 *
 * \section asfdoc_samb_i2c_special_considerations Special Considerations
 *
 *
 * \section asfdoc_samb_i2c_extra Extra Information
 * For extra information, see \ref asfdoc_samb_i2c_extra_info_page.
 * This includes:
 *  - \ref asfdoc_samb_i2c_acronyms
 *  - \ref asfdoc_samb_i2c_extra_dependencies
 *  - \ref asfdoc_samb_i2c_extra_errata
 *  - \ref asfdoc_samb_i2c_extra_history
 *
 * \section asfdoc_samb_i2c_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_samb_i2c_exqsg.
 *
 * \section asfdoc_samb_i2c_api_overview API Overview
 * @{
 */


/** \brief Transfer direction
 *
 * For master: transfer direction or setting direction bit in address.
 * For slave: direction of request from master.
 */
enum i2c_transfer_direction {
	/** Master write operation is in progress */
	I2C_TRANSFER_WRITE = 0,
	/** Master read operation is in progress */
	I2C_TRANSFER_READ  = 1,
};

/**
 * \brief I2C module clock input 
 *
 * I2C module clock.
 *
 */
enum i2c_clock_input {
	/** source from clock input 0: 26MHz */
	I2C_CLK_INPUT_0 = 0,
	/** source from clock input 1: 13MHz */
	I2C_CLK_INPUT_1,
	/** source from clock input 2: 6.5MHz */
	I2C_CLK_INPUT_2,
	/** source from clock input 3: 3MHz */
	I2C_CLK_INPUT_3,
};

/**
 * \brief Wait for idle
 *
 * Wait for hardware module to sync
 *
 * \param[in]  i2c_module  Pointer to software module structure
 */
static inline void i2c_wait_for_idle(I2C *const i2c_module)
{
	while (i2c_module->I2C_STATUS.bit.I2C_ACTIVE) {
		/* Wait for I2C module to sync. */
	}
}

/**
 * \brief Enable driver instance
 *
 * This function enable driver instance
 *
 * \param[in,out] module Pointer to the driver instance to enable
 *
 */
static inline void i2c_enable(I2C *const i2c_module)
{
	/* Wait for module to sync. */
	i2c_wait_for_idle(i2c_module);
	/* Enable module. */
	i2c_module->I2C_MODULE_ENABLE.reg = (1 << I2C_I2C_MODULE_ENABLE_ENABLE_Pos);
}

/**
 * \brief Disable driver instance
 *
 * This function disable driver instance
 *
 * \param[in,out] i2c_module Pointer to the driver instance to disable
 *
 */
static inline void i2c_disable(I2C *const i2c_module)
{
	i2c_wait_for_idle(i2c_module);
	i2c_module->I2C_MODULE_ENABLE.reg = 0;
}

/**
 * \brief Flush the contents of both the Tx and Rx FIFOs
 *
 * This function flushes the contents of both the Tx and Rx FIFOs.
 *
 * \param[in,out] i2c_module Pointer to the driver instance
 *
 */
static inline void i2c_slave_flush_fifo(I2C *const i2c_module)
{
	i2c_wait_for_idle(i2c_module);
	i2c_module->I2C_FLUSH.reg = 1;
}

/**
 * \brief Enable or Disable slave rx interrupt
 *
 * This function enables or disables slave rx interrupt.
 *
 * \param[in,out] i2c_module Pointer to the driver
 * \param[in]     enable     Enable or Disable
 *
 */
static inline void i2c_slave_rx_interrupt(I2C *const i2c_module, bool enable)
{
	if (enable) {
		i2c_module->RX_INTERRUPT_MASK.bit.RX_FIFO_NOT_EMPTY_MASK = 1;
	} else {
		i2c_module->RX_INTERRUPT_MASK.bit.RX_FIFO_NOT_EMPTY_MASK = 0;
	}
}

/**
 * \brief Enable or Disable slave tx interrupt
 *
 * This function enables or disables slave tx interrupt.
 *
 * \param[in,out] i2c_module Pointer to the driver
 * \param[in]     enable     Enable or Disable
 *
 */
static inline void i2c_slave_tx_interrupt(I2C *const i2c_module, bool enable)
{
	if (enable) {
		i2c_module->TX_INTERRUPT_MASK.bit.TX_FIFO_NOT_FULL_MASK = 1;
		i2c_module->RX_INTERRUPT_MASK.bit.NAK_MASK = 1;
	} else {
		i2c_module->TX_INTERRUPT_MASK.bit.TX_FIFO_NOT_FULL_MASK = 0;
		i2c_module->RX_INTERRUPT_MASK.bit.NAK_MASK = 0;
	}
}
/** @} */

#ifdef __cplusplus
}
#endif

/**
 * \page asfdoc_samb_i2c_extra_info_page  Extra Information for I2C Driver
 *
 * \section asfdoc_samb_i2c_acronyms Acronyms
 * \ref asfdoc_samb_i2c_acronyms_table "Below" is a table listing the acronyms
 * used in this module, along with their intended meanings.
 *
 * \anchor asfdoc_samb_i2c_acronyms_table
 * <table>
 *  <caption>Acronyms</caption>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>SDA</td>
 *		<td>Serial Data Line</td>
 *	</tr>
 *	<tr>
 *		<td>SCL</td>
 *		<td>Serial Clock Line</td>
 *	</tr>
 * </table>
 *
 * \section asfdoc_samb_i2c_extra_dependencies Dependencies
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samb_i2c_extra_errata Errata
 * There are no errata related to this driver.
 *
 * \section asfdoc_samb_i2c_extra_history Module History
 * \ref asfdoc_samb_i2c_extra_history_table "Below" is an overview of the
 * module history, detailing enhancements and fixes made to the module since
 * its first release. The current version of this corresponds to the newest
 * version listed in
 * \ref asfdoc_samb_i2c_extra_history_table "the table below".
 *
 * \anchor asfdoc_samb_i2c_extra_history_table
 * <table>
 *  <caption>Module History</caption>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_samb_i2c_exqsg Examples for I2C Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samb_i2c_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 * \if I2C_MASTER_MODE
 * - \subpage asfdoc_samb_i2c_master_basic_use_case "Quick Start Guide for the I2C Master module - Basic Use Case"
 * \endif
 * \if I2C_MASTER_CALLBACK_MODE
 * - \subpage asfdoc_samb_i2c_master_callback_use_case "Quick Start Guide for the I2C Master module - Callback Use Case"
 * \endif
 * \if I2C_SLAVE_MODE
 * - \subpage asfdoc_samb_i2c_slave_basic_use_case "Quick Start Guide for the I2C Slave module - Basic Use Case"
 * \endif
 * \if I2C_SLAVE_CALLBACK_MODE
 * - \subpage asfdoc_samb_i2c_slave_callback_use_case "Quick Start Guide for the I2C Slave module - Callback Use Case"
 * \endif
 *
 * \page asfdoc_samb_i2c_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>09/2015</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#endif /* I2C_COMMON_H_INCLUDED */
