/**
 * \file
 *
 * \brief SAMD20 Non-Volatile Memory driver
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
#ifndef NVM_H_INCLUDED
#define NVM_H_INCLUDED

/**
 * \defgroup sam0_nvm_group SAMD20 Non-Volatile Memory Driver (NVM)
 *
 * Driver for the SAMD20 Non-volatile Memory controller. Provides a unified
 * interface for the erasing, reading and writing of data to and from the
 * various non-volatile memories of the device, including executable flash
 * memory and user configuration pages. This driver encompasses the following
 * module within the SAMD20 devices:
 *
 * \li \b NVM (Non-Volatile Memory)
 *
 * \section nvm_introduction Introduction
 *
 * \subsection memory_region NVM Memory Layout
 *
 * The NVM memory is divided into two sections: Main Array and Auxiliary space,
 * where the Main Array can be configured to have both a EEPROM and boot loader
 * section. The memory layout with EEPROM and bootloader is shown in the figure
 * below.
 *
 * \dot
 * digraph memory_layout {
 *  size="5,5"
 *  node [shape=plaintext]
 *  memory [label=<
 *   <table border="0" cellborder="1" cellspacing="0" >
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
 *     <td height="300" align="right" border="0"> </td>
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
 * The Main Array is divided into rows and pages, where each row contains four
 * pages. The size of each page may vary from 8-1024 bytes dependent of the
 * device. Parameters like page size and total number of pages in the NVM memory
 * are available by using \ref nvm_get_parameters().
 *
 * The equations for a page's number and address is given by the equations
 * below.
 *
 * \f[ PageNum = (RowNum \times 4) + PagePosInRow \f]
 * \f[ PageAddr = PageNum \times PageSize \f]
 *
 * The figure below gives an example to which page numbers and addresses row 7
 * in the NVM memory has.
 *
 * \dot
 * digraph row_layout {
 *  size="4,4"
 *  node [shape=plaintext]
 *  row [label=<
 *   <table border="0" cellborder="1" cellspacing="0">
 *    <tr>
 *     <td align="right" border ="0"> Row 0x07 </td>
 *     <td > Page 0x1F </td>
 *     <td > Page 0x1E </td>
 *     <td > Page 0x1D </td>
 *     <td > Page 0x1C </td>
 *    </tr>
 *    <tr>
 *     <td align="right" border ="0"> Address </td>
 *     <td border="0"> 0x7C0 </td>
 *     <td border="0"> 0x780 </td>
 *     <td border="0"> 0x740 </td>
 *     <td border="0"> 0x700 </td>
 *    </tr>
 *   </table>
 *  >]
 * }
 * \enddot
 *
 * The Auxiliary space contains the User Row which is organized as shown in
 * table below:
 *
 *  <table>
 *   <tr>
 *    <th> Bit position </th>
 *    <th> Name </th>
 *    <th> Usage</th>
 *   </tr>
 *   <tr>
 *    <td> 2:0 </td>
 *    <td> BOOTPROT </td>
 *    <td> Used to set one of eight bootloader sizes.
 *         \note Can not be changed when security bit is set.
 *         \note Rows included in bootloader section can only be erased
 *         by a chip erase.</td>
 *   </tr>
 *   <tr>
 *    <td> 3 </td>
 *    <td> Reserved </td>
 *    <td> </td>
 *   </tr>
 *   <tr>
 *    <td> 6:4 </td>
 *    <td> EEPROM </td>
 *    <td> Used to set one of eight EEPROM sizes
 *           \note Can not be changed when security bit is set. </td>
 *   </tr>
 *   <tr>
 *    <td> 7 </td>
 *    <td> Reserved </td>
 *    <td> </td>
 *   </tr>
 *   <tr>
 *    <td> 47:8 </td>
 *    <td> Reserved </td>
 *    <td>  </td>
 *   </tr>
 *   <tr>
 *    <td> 63:48 </td>
 *    <td> LOCK </td>
 *    <td> Used to lock different regions of the NVM. See
 *         \ref nvm_locking_regions
 *   </td>
 *   </tr>
 *  </table>
 *
 * The table below shows the number of NVM memory rows that are reserved for
 * use by the bootloader. The size must be configured via the \ref nvm_config
 * structure instance.
 *
 * <table>
 *  <tr>
 *   <th> BOOTPROT[2:0] </th>
 *   <th> Rows Protected by BOOTPROT </th>
 *  </tr>
 *  <tr>
 *   <td align="center"> 7 </td>
 *   <td align="center"> None </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 6 </td>
 *   <td align="center"> 2 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 5 </td>
 *   <td align="center"> 4 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 4 </td>
 *   <td align="center"> 8 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 3 </td>
 *   <td align="center"> 16 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 2 </td>
 *   <td align="center"> 32 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 1 </td>
 *   <td align="center"> 64 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 0 </td>
 *   <td align="center"> 128 </td>
 *  </tr>
 * </table>
 *
 * The table below shows the number of NVM memory rows that are reserved for
 * use by the emulated EEPROM. The size must be configured via the
 * \ref nvm_config structure instance.
 *
 * <table>
 *  <tr>
 *   <th> EEPROM[2:0] </th>
 *   <th> Rows Allocated by EEPROM </th>
 *  </tr>
 *  <tr>
 *   <td align="center"> 7 </td>
 *   <td align="center"> None </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 6 </td>
 *   <td align="center"> 1 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 5 </td>
 *   <td align="center"> 2 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 4 </td>
 *   <td align="center"> 4 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 3 </td>
 *   <td align="center"> 8 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 2 </td>
 *   <td align="center"> 16 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 1 </td>
 *   <td align="center"> 32 </td>
 *  </tr>
 *  <tr>
 *   <td align="center"> 0 </td>
 *   <td align="center"> 64 </td>
 *  </tr>
 * </table>
 *
 * \subsection nvm_locking_regions Region Lock Bits
 * As mentioned in \ref memory_region, the main block of the NVM memory is
 * divided into pages. These pages are grouped into 16 equal sized regions,
 * where each region can be locked separately issuing an
 * \ref NVM_COMMAND_LOCK_REGION command or by writing the LOCK bits in the User
 * Row. Rows reserved for the EEPROM section is not affected by the locking.
 *
 * \note By using the \ref NVM_COMMAND_LOCK_REGION/\ref NVM_COMMAND_UNLOCK_REGION commands
 * the settings will stay in effect until the next reset. By changing the
 * default lock setting for the regions, the auxiliary space have to be written.
 * These settings will not come into action before a reset is issued.
 *
 * \note If the \ref nvm_security_bit is set, the auxiliary space cannot be written.
 * Clearing the \ref nvm_security_bit can only be done by a chip erase.
 *
 * \subsection nvm_sub_rw Read/Write
 * Reading from the NVM memory can be done by directly addressing it, or by
 * using the \ref nvm_write_page(). Writing to the NVM memory requires some
 * special considerations. If a whole page is to be written the
 * \ref nvm_write_page() function can be used after erasing the row where the
 * page is located, this can be done by \ref nvm_erase_row(). However, if a
 * buffer of a different size or location than a specific page is to be written,
 * it can be done by the following procedure:
 * -# Erase the location in the NVM memory of where the buffer is to be written.
 * -# Write to the page buffer, which has the size of one page, by addressing
 *  the NVM memory location directly.
 * -# Issue an \ref NVM_COMMAND_WRITE_PAGE
 *  -# If the manual_page_write in the \ref nvm_config is disabled, and the last
 *  byte of the page buffer is addressed, this will happen automatically.
 *  -# If not, use the \ref nvm_execute_command() with the
 *  \ref NVM_COMMAND_WRITE_PAGE.
 *
 * \note The granularity of an erase is per row while the granularity of a
 *       write is per page. Thus, modifying only one page of a row will require
 *       to buffer the three remaining pages, as an erase is mandatory before
 *       writing to a page.
 *
 * \section module_dependencies Dependencies
 * The NVM driver has the following dependencies:
 *
 * \li \b None
 *
 * \section nvm_special_considerations Special Considerations
 *
 * \subsection nvm_clocks Clocks
 * The user must ensure that the driver is configured with a proper number of
 * wait states when the CPU is running at high frequencies.
 *
 * \subsection nvm pac Peripheral Access Control (PAC)
 * The user must ensure that the Peripheral Access Controller is configured to
 * enable write protection when using this driver.
 *
 * \subsection nvm_security_bit Security Bit
 * The User Row in the Auxiliary Space Cannot be read or written when
 * the Security Bit is set. The Security Bit can be set by using passing
 * \ref NVM_COMMAND_SET_SECURITY_BIT to the \ref nvm_execute_command() function,
 * or it will be be set if one tries to access a locked region. See \ref
 * nvm_locking_regions.
 *
 * The Security Bit can only be cleared by performing a chip erase.
 *
 * \subsection nvm_sub_extra_info Extra Info
 * For extra information see \ref nvm_extra_info.
 *
 * \section module_examples Examples
 * - \ref nvm_quickstart
 *
 * \section api_overview API Overview
 * @{
 */

#include <compiler.h>
#include <status_codes.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Mask for the error flags in the status register
 */
#define NVM_ERRORS_MASK (NVMCTRL_STATUS_PROGE | NVMCTRL_STATUS_LOCKE | NVMCTRL_STATUS_NVME)

/**
 * \brief NVM errors
 */
enum nvm_errors {
	/** No errors */
	NVM_ERROR_NONE  = 0x00,
	/** Lock error, a locked region was attempted accessed */
	NVM_ERROR_LOCK  = 0x18,
	/** Program error, invalid command was executed */
	NVM_ERROR_PROG  = 0x14,
};

/**
 * \brief NVM controller commands.
 */
enum nvm_command {
	/** Erases the addressed memory row.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>Address of the memory row to erase</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_ERASE_ROW         = NVMCTRL_CTRLA_CMD_ER,

	/** Write the contents of the page buffer to the addressed memory page.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>Address of the memory row to write to</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_WRITE_PAGE        = NVMCTRL_CTRLA_CMD_WP,

	/** Erases the addressed auxiliary memory row.
	 *
	 *  \note This command can only be given when the security bit is not set.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>Address of the auxiliary memory row to erase</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_ERASE_AUX_ROW     = NVMCTRL_CTRLA_CMD_EAR,

	/** Write the contents of the page buffer to the addressed auxiliary memory
	 *  row.
	 *
	 *  \note This command can only be given when the security bit is not set.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>Address of the auxiliary memory row to erase</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_WRITE_AUX_ROW     = NVMCTRL_CTRLA_CMD_WAP,

	/** Locks the addressed memory region, preventing further modifications
	 *  until the region is unlocked or the device is erased.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>Address of the memory region to lock</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_LOCK_REGION       = NVMCTRL_CTRLA_CMD_LR,

	/** Unlocks the addressed memory region, allowing the region contents to be
	 *  modified.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>Address of the memory region to unlock</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_UNLOCK_REGION     = NVMCTRL_CTRLA_CMD_UR,

	/** Clears the page buffer of the NVM controller, resetting the contents to
	 *  all zero values.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_PAGE_BUFFER_CLEAR = NVMCTRL_CTRLA_CMD_PBC,

	/** Sets the device security bit, disallowing the changing of lock bits and
	 *  auxiliary row data until a chip erase has been performed.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_SET_SECURITY_BIT  = NVMCTRL_CTRLA_CMD_SSB,

	/** Enter power reduction mode in the NVM controller to reduce the power
	 *  consumption of the system. When in low power mode, all commands other
	 *  than \ref NVM_COMMAND_CLEAR_POWER_REDUCTION_MODE will fail.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_SET_POWER_REDUCTION_MODE = NVMCTRL_CTRLA_CMD_SPRM,

	/** Exit power reduction mode in the NVM controller to allow other NVM
	 *  commands to be issued.
	 *
	 *  <table>
	 *   <tr>
	 *    <td>\b Address:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *   <tr>
	 *    <td>\b Parameter:</td>
	 *    <td>\e Unused</td>
	 *   </tr>
	 *  </table>
	 */
	NVM_COMMAND_CLEAR_POWER_REDUCTION_MODE = NVMCTRL_CTRLA_CMD_CPRM,
};

/**
 * \brief NVM controller power reduction mode configurations.
 */
enum nvm_sleep_power_mode {
	/** NVM controller exits low power mode on first access after sleep. */
	NVM_AUTO_WAKE_MODE_WAKEONACCESS     = NVMCTRL_CTRLB_SLEEPPRM_WAKEONACCESS,
	/** NVM controller exits low power mode when the device exits sleep mode. */
	NVM_AUTO_WAKE_MODE_WAKEUPINSTANT    = NVMCTRL_CTRLB_SLEEPPRM_WAKEUPINSTANT,
	/** Power reduction mode in the NVM controller disabled. */
	NVM_AUTO_WAKE_MODE_ALWAYS_AWAKE  = NVMCTRL_CTRLB_SLEEPPRM_DISABLED,
};

/**
 * \brief NVM controller configuration structure.
 *
 * Configuration structure for the NVM controller within the device.
 */
struct nvm_config {
	/** Power reduction mode during device sleep. */
	enum nvm_sleep_power_mode sleep_power_mode;
	/** Manual write mode; if enabled, pages loaded into the NVM buffer will
	 *  not be written until a separate write command is issued. If disabled,
	 *  writing to the last byte in the NVM page buffer will trigger an automatic
	 *  write.
	 *
	 *  \note If a partial page is to be written, a manual write command must be
	 *        executed in either mode.
	 */
	bool manual_page_write;
	/** Number of wait states to insert when reading from flash, to prevent
	 *  invalid data from being read at high clock frequencies.
	 */
	uint8_t wait_states;
	/** Size of the emulated EEPROM memory section configured in the NVM
	 *  auxiliary memory space, in bytes. */
	uint32_t eeprom_size;
	/** Size of the Bootloader memory section configured in the NVM auxiliary
	 *  memory space, in bytes. */
	uint32_t bootloader_size;
};

/**
 * \brief Parameter struct
 *
 * Struct holding the parameters for the NVM module.
 *
 */
struct nvm_parameters {
	/** Number of bytes per page */
	uint8_t page_size;
	/** Number of pages in the main array */
	uint16_t nvm_number_of_pages;
};

/**
 * \name Configuration and Initialization
 * @{
 */

/**
 * \brief Initializes an NVM controller configuration structure to defaults.
 *
 * Initializes a given NVM controller configuration structure to a set of
 * known default values. This function should be called on all new
 * instances of these configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 *  \li Power reduction mode enabled after sleep until first NVM access
 *  \li Automatic page commit when full pages are written to
 *  \li Zero wait states when reading flash memory
 *  \li No reserved memory for the EEPROM
 *  \li No protected bootloader section
 *
 * \param[out] config  Configuration structure to initialize to default values
 *
 */
static inline void nvm_get_config_defaults(
		struct nvm_config *const config)
{
	/* Sanity check the parameters */
	Assert(config);

	/* Write the default configuration for the */
	config->sleep_power_mode  = NVM_AUTO_WAKE_MODE_WAKEONACCESS;
	config->manual_page_write = false;
	config->wait_states       = 0;
	config->eeprom_size       = 0;
	config->bootloader_size   = 0;
}

enum status_code nvm_set_config(
		const struct nvm_config *const config);

/**
 * \brief Checks if the NVM controller is ready
 *
 * Retrieves information on whether the NVM controller is ready
 * to be used or if it is busy.
 *
 * \return    Returns the state of the NVM hardware module
 *
 * \retval true     If the hardware module is ready
 * \retval false    If the hardware module is busy
 *
 */
static inline bool nvm_is_ready(void)
{
	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	return nvm_module->INTFLAG.reg & NVMCTRL_INTFLAG_READY;
}
/** @} */

/**
 * \name NVM Access Management
 * @{
 */

/**
 * \brief Gets the parameters of the NVM controller
 *
 * Retrieves the page size and number of pages in the NVM memory region.
 *
 * \param[out] parameters    Parameter structure, which holds page size and
 *                           number of pages in the NVM memory
 */
static inline void nvm_get_parameters(
		struct nvm_parameters *const parameters)
{
	/* Sanity check parameters */
	Assert(parameters);

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Read out from the PARAM register */
	uint32_t param_reg;
	param_reg = nvm_module->PARAM.reg;

	/* Mask out page size and number of pages */
	parameters->page_size  =
			(param_reg & NVMCTRL_PARAM_PSZ_Msk)  >> NVMCTRL_PARAM_PSZ_Pos;
	parameters->nvm_number_of_pages =
			(param_reg & NVMCTRL_PARAM_NVMP_Msk) >> NVMCTRL_PARAM_NVMP_Pos;
}

enum status_code nvm_write_page(
		const uint16_t dst_page_nr,
		const uint32_t *buf);

enum status_code nvm_read_page(
		const uint16_t src_page_nr,
		uint32_t *buf);

enum status_code nvm_erase_row(
		const uint16_t row_nr);

enum status_code nvm_erase_block(
		const uint16_t row_nr,
		const uint16_t rows);

enum status_code nvm_execute_command(
		const enum nvm_command command,
		const uint32_t address,
		const uint32_t parameter);

/**
 * \brief Retrieves, if any, error from the last NVM operation.
 *
 * Retrieves, if any, error from the last executed NVM operation. Once
 * retrieved, any error state flags in the controller are cleared.
 *
 * \note \ref nvm_is_ready() is an exception. Thus, errors retrieved after
 * running this function should be valid for the function executed before
 * nvm_is_ready().
 *
 * \return Error caused by the last NVM operation.
 *
 * \retval NVM_ERROR_NONE          No error did occur in the last NVM operation
 *
 * \retval NVM_ERROR_LOCK          It was attempted to access a locked region
 *                                 in the last NVM operation
 * \retval NVM_ERROR_PROG          An invalid command has been issued
 */
static inline enum nvm_errors nvm_get_error(void)
{
	enum nvm_errors ret_val;

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Mask out non-error bits */
	ret_val =  ((nvm_module->STATUS.reg) & 0x1C);

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Return error*/
	return ret_val;
}

/** @} */

#ifdef __cplusplus
}
#endif
/** @} */

/**
 * \page nvm_extra_info Extra Information
 *
 * \section Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *   <th>Acronym</th>
 *   <th>Description</th>
 *  </tr>
 *  <tr>
 *   <td>NVM</td>
 *   <td>Non-Volatile Memory</td>
 *  </tr>
 *  <tr>
 *   <td>EEPROM</td>
 *   <td>Electrically Erasable Programmable Read-Only Memory</td>
 *  </tr>
 * </table>
 *
 * \section nvm_workaround Workarounds in the driver
 * There are no workarounds in this driver.
 *
 * \section module_history Module History
 * Below is an overview of the module history, detailing enhancements and fixes
 * made to the module since its first release. The current version of this
 * corresponds to the newest version listed in the table below.
 *
 * <table>
 *  <tr>
 *   <th>Changelog</th>
 *  </tr>
 *  <tr>
 *   <td>Initial Release</td>
 *  </tr>
 * </table>
 */

/**
 * \page nvm_quickstart Quick Start Guides for the NVM module
 *
 * This is the quick start guide list for the \ref sam0_nvm_group module, with
 * step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function of the
 * user application and run at system startup, while the steps for usage can be
 * copied into the normal user application program flow.
 *
 * \see General list of module \ref module_examples "examples".
 *
 * \section nvm_use_cases NVM module use cases
 * - \subpage nvm_basic_use_case
 */

#endif /* NVM_H_INCLUDED */
