/**
 * \file
 *
 * \brief SAMD20 Non Volatile Memory driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#include "nvm.h"

/**
 * \brief Number of pages per row in the NVM controller.
 *
 * Number of pages per row in the NVM controller. An NVM memory row consists of
 * \ref NVM_PAGES_PER_ROW * \ref nvm_parameters.page_size bytes.
 */
#define NVM_PAGES_PER_ROW  4

/**
 * \internal NVM data
 *
 *  Union of different data lengths
 */
union _nvm_data {
	uint32_t data32;
	uint16_t data16[2];
	uint8_t  data8[4];
};

/**
 * \internal Internal device instance struct
 *
 * This struct contains information about the NVM module which is
 * often used by the different functions. The information is loaded
 * into the struct in the nvm_init() function.
 */
struct _nvm_device {
	/** Number of bytes contained per page */
	uint16_t page_size;
	/** Total number of pages in the NVM memory */
	uint16_t number_of_pages;
	/** If man_write_page returns false, a page write command will
	 * be issued automatically when the page buffer is full */
	bool man_page_write;
};

/**
 * \internal Instance of the internal device struct
 */
static struct _nvm_device _nvm_dev;

/**
 * \internal Pointer to the NVM MEMORY region
 */
#define NVM_MEMORY ((union _nvm_data *) 0)

/**
 * \brief Sets the up the NVM hardware module based on the configuration.
 *
 * Writes a given configuration of a NVM controller configuration to the
 * hardware module, and initializes the internal device struct
 *
 * \param[in] config    Configuration settings for the NVM controller
 *
 * \note The security bit must be cleared in order successfully use this
 *       function. This can only be done by a chip erase.
 *
 * \return Status of the configuration procedure.
 *
 * \retval STATUS_OK                If the initialization was a success
 * \retval STATUS_ERR_BUSY          If the module was already busy
 * \retval STATUS_ERR_IO            If the security bit was set, the function
 *                                  could not set the bootloader and eeprom
 *                                  size
 */
enum status_code nvm_set_config(
		const struct nvm_config *const config)
{
	/* Sanity check argument */
	Assert(config);

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_ERR_BUSY;
	}

	/* Writing configuration to the CTRLB register */
	nvm_module->CTRLB.reg =
			(config->sleep_power_mode  << NVMCTRL_CTRLB_SLEEPPRM_Pos) |
			(config->manual_page_write << NVMCTRL_CTRLB_MANW_Pos) |
			(config->wait_states       << NVMCTRL_CTRLB_RWS_Pos);

	/* Initialize the internal device struct */
	_nvm_dev.page_size =
			8 * (2 << ((nvm_module->PARAM.reg & NVMCTRL_PARAM_PSZ_Msk) >>
			NVMCTRL_PARAM_PSZ_Pos));
	_nvm_dev.number_of_pages = (nvm_module->PARAM.reg & NVMCTRL_PARAM_NVMP_Msk);
	_nvm_dev.man_page_write = config->manual_page_write;

	/* If the security bit is set, the auxiliary space cannot be written */
	if (nvm_module->STATUS.reg & NVMCTRL_STATUS_SB) {
		return STATUS_ERR_IO;
	}

	/* Create pointer to the auxiliary space */
	uint32_t *aux_row = (uint32_t*)NVMCTRL_AUX0_ADDRESS;

	/* Writing bootloader and eeprom size to the auxiliary space */
	*aux_row =
			(config->bootloader_size << NVMCTRL_AUX_BOOTPROT_Pos) |
			(config->eeprom_size     << NVMCTRL_AUX_EEPROM_Pos);

	/* Issue the write auxiliary space command */
	nvm_module->CTRLA.reg = NVM_COMMAND_WRITE_AUX_ROW;

	return STATUS_OK;
}

/**
 * \brief Executes a command on the NVM controller.
 *
 * Executes an asynchronous command on the NVM controller, to perform a requested
 * action such as a NVM page read or write operation.
 *
 * \note The function will return before the execution of the given command is
 *       done.
 *
 * \param[in] command    Command to issue to the NVM controller
 * \param[in] address    Address to pass to the NVM controller
 * \param[in] parameter  Parameter to pass to the NVM controller, not used
 *                       for this driver
 *
 * \return Status of the attempt to execute a command.
 *
 * \retval STATUS_OK               If the command was accepted and execution
 *                                 is now in progress
 * \retval STATUS_ERR_BUSY         If the NVM controller was already busy
 *                                 executing a command when the new command
 *                                 was issued
 * \retval STATUS_ERR_IO           If the command was invalid due to memory or
 *                                 security locking
 * \retval STATUS_ERR_INVALID_ARG  If the given command was invalid or
 *                                 unsupported
 * \retval STATUS_ERR_BAD_ADDRESS  If the given address was invalid
 */
enum status_code nvm_execute_command(
		const enum nvm_command command,
		const uint32_t address,
		const uint32_t parameter)
{
	/* Check that the address given is valid  */
	if (address > (uint32_t)(_nvm_dev.page_size * _nvm_dev.number_of_pages)){
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_ERR_BUSY;
	}

	switch (command) {
		/* Commands requiring address */
		case NVM_COMMAND_ERASE_AUX_ROW:
		case NVM_COMMAND_WRITE_AUX_ROW:

			/* Auxiliary space cannot be accessed if the security
			 * is set */
			if(nvm_module->STATUS.reg & NVMCTRL_STATUS_SB) {
				return STATUS_ERR_IO;
			}

			/* Set address and command */
			nvm_module->ADDR.reg  = address;
			nvm_module->CTRLA.reg = (command << NVMCTRL_CTRLA_CMD_Pos) |
					(NVMCTRL_CMDEX_EXECUTION_KEY << NVMCTRL_CTRLA_CMDEX_Pos);
			break;

		/* Commands requiring address */
		case NVM_COMMAND_ERASE_ROW:
		case NVM_COMMAND_WRITE_PAGE:
		case NVM_COMMAND_LOCK_REGION:
		case NVM_COMMAND_UNLOCK_REGION:

			/* Set address and command */
			nvm_module->ADDR.reg  = address;
			nvm_module->CTRLA.reg = (command << NVMCTRL_CTRLA_CMD_Pos) |
					(NVMCTRL_CMDEX_EXECUTION_KEY << NVMCTRL_CTRLA_CMDEX_Pos);
			break;

		/* Commands not requiring address */
		case NVM_COMMAND_PAGE_BUFFER_CLEAR:
		case NVM_COMMAND_SET_SECURITY_BIT:
		case NVM_COMMAND_SET_POWER_REDUCTION_MODE:
		case NVM_COMMAND_CLEAR_POWER_REDUCTION_MODE:

			/* Set command */
			nvm_module->CTRLA.reg = (command << NVMCTRL_CTRLA_CMD_Pos) |
					(NVMCTRL_CMDEX_EXECUTION_KEY << NVMCTRL_CTRLA_CMDEX_Pos);
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/**
 * \brief Writes to page in the NVM memory region
 *
 * Writes from a buffer to a given page number in the NVM memory.
 *
 * \param[in] dst_page_nr      Number of the page to write to
 * \param[in] buf              Pointer to buffer to write from into the
 *                             NVM memory
 *
 * \note The nvm_is_ready() should be used in advance to make sure that the
 *       NVM controller is ready.
 * \note The user have to perform an \ref nvm_erase_row() before this command
 *       is used.
 *
 * \return Status of the attempt to write a page.
 *
 * \retval STATUS_OK               If the reading of the NVM memory was
 *                                 successful
 * \retval STATUS_ERR_BUSY         If the NVM controller was already busy
 * \retval STATUS_ERR_BAD_ADDRESS  If dst_page_nr is larger than the number of
 *                                 pages available in the NVM memory region
 */
enum status_code nvm_write_page(
		const uint16_t dst_page_nr,
		const uint32_t *buf)
{
	uint32_t i;
	uint32_t nvm_addr;

	/* Sanity check arguments */
	if (dst_page_nr > (_nvm_dev.number_of_pages - 1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_ERR_BUSY;
	}

	/* 32 bit addressing the NVM */
	nvm_addr = dst_page_nr * (_nvm_dev.page_size / 4);

	/* Write to the NVM memory 4 bytes at a time */
	for (i = 0; i < (_nvm_dev.page_size / 4); i++) {
		NVM_MEMORY[nvm_addr++].data32 = buf[i];
	}

	return STATUS_OK;
}

/**
 * \brief Reads from page in the NVM memory region
 *
 * Reads from a given page number in the NVM memory into a buffer.
 *
 * \param[in]  src_page_nr      Number of the page to read from
 * \param[out] buf              Pointer to buffer where the content of the
 *                               page will be stored
 *
 * \note The nvm_is_ready() should be used in advance to make sure that the
 *       NVM controller is ready.
 *
 * \return Status of the attempt to read a page.
 *
 * \retval STATUS_OK               If the reading of the NVM memory was
 *                                 successful
 * \retval STATUS_ERR_BUSY         If the NVM controller was already busy
 * \retval STATUS_ERR_BAD_ADDRESS  If dst_page_nr is larger than the number of
 *                                 pages available in the NVM memory region
 */
enum status_code nvm_read_page(
		const uint16_t src_page_nr,
		uint32_t *buf)
{
	uint32_t i;
	uint32_t nvm_addr;

	/* Sanity check arguments */
	if (src_page_nr > (_nvm_dev.number_of_pages - 1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_ERR_BUSY;
	}

	/* 32-bit addressing the NVM memory */
	nvm_addr = src_page_nr * (_nvm_dev.page_size / 4);

	/* Read out from NVM memory 4 bytes at a time */
	for (i = 0; i < (_nvm_dev.page_size / 4); i++) {
		buf[i] = NVM_MEMORY[nvm_addr++].data32;
	}

	return STATUS_OK;
}

/**
 * \brief Erases a row in the NVM memory
 *
 * Erases a given row in the NVM memory region.
 *
 * \param[in] row_nr      Number of the row to erase
 *
 * \note The nvm_is_ready() should be used in advance to make sure that the
 *       NVM controller is ready.
 *
 * \return Status of the attempt to erase a row.
 *
 * \retval STATUS_OK               If issuing the erase was successful
 * \retval STATUS_ERR_BUSY         If the NVM controller was already busy
 * \retval STATUS_ERR_BAD_ADDRESS  If row_nr was larger than the number of
 *                                 rows available in the NVM memory region
 */
enum status_code nvm_erase_row(const uint16_t row_nr)
{
	uint16_t row_addr;

	/* Check if the row_nr is valid */
	if (row_nr > ((_nvm_dev.number_of_pages / NVM_PAGES_PER_ROW) - 1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_ERR_BUSY;
	}

	/* Address to row */
	row_addr = row_nr * (_nvm_dev.page_size * NVM_PAGES_PER_ROW);

	/* Set address and command */
	nvm_module->ADDR.reg = row_addr;
	nvm_module->CTRLA.reg = (NVM_COMMAND_ERASE_ROW << NVMCTRL_CTRLA_CMD_Pos) |
			(NVMCTRL_CMDEX_EXECUTION_KEY << NVMCTRL_CTRLA_CMDEX_Pos);

	return STATUS_OK;
}

/**
 * \brief Erases a block in the NVM memory
 *
 * Erases a given number of rows in the NVM memory, starting at a
 * given row number.
 *
 * \param[in] row_nr      Number of the first row to erase
 * \param[in] rows        Number of rows to erase
 *
 * \note The nvm_is_ready() should be used in advance to make sure that the
 *       NVM controller is ready.
 *
 * \return Status of the attempt to erase a block.
 *
 * \retval STATUS_OK               If issuing the erase was successful
 * \retval STATUS_ERR_BUSY         If the NVM controller was already busy
 * \retval STATUS_ERR_BAD_ADDRESS  If row_nr and number of rows was larger than
 *                                 the number of rows available in the
 *                                 NVM memory region
 */
enum status_code nvm_erase_block(uint16_t row_nr, const uint16_t rows)
{
	uint16_t i;
	uint16_t row_addr;
	uint16_t row_size;
	uint32_t block_size;

	/* Byte sizes and address */
	row_size = _nvm_dev.page_size * NVM_PAGES_PER_ROW;
	row_addr = row_nr * row_size;
	block_size = rows * row_size;

	/* Sanity check of row and block size */
	if ((row_addr + block_size) >
			(uint32_t)(_nvm_dev.page_size * _nvm_dev.number_of_pages)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_ERR_BUSY;
	}

	/* Set address and command */
	for (i = 0; i > rows; i++) {
		nvm_module->ADDR.reg = row_addr ;
		nvm_module->CTRLA.reg = (NVM_COMMAND_ERASE_ROW << NVMCTRL_CTRLA_CMD_Pos) |
				(NVMCTRL_CMDEX_EXECUTION_KEY << NVMCTRL_CTRLA_CMDEX_Pos);

		/* Increment the row address */
		row_addr += row_size;
	}

	return STATUS_OK;
}
