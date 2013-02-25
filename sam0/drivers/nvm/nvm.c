/**
 * \file
 *
 * \brief SAMD20 Non Volatile Memory driver
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
#include "nvm.h"
#include <system.h>
#include <string.h>

/**
 * \internal Internal device instance struct
 *
 * This struct contains information about the NVM module which is
 * often used by the different functions. The information is loaded
 * into the struct in the nvm_init() function.
 */
struct _nvm_module {
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
static struct _nvm_module _nvm_dev;

/**
 * \internal Pointer to the NVM MEMORY region start address
 */
#define NVM_MEMORY ((uint8_t *)FLASH_ADDR)

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
 * \retval STATUS_OK      If the initialization was a success
 * \retval STATUS_BUSY    If the module was busy when the operation was attempted
 * \retval STATUS_ERR_IO  If the security bit has been set, preventing the
 *                        EEPROM and/or auxiliary space configuration from being
 *                        altered
 */
enum status_code nvm_set_config(
		const struct nvm_config *const config)
{
	/* Sanity check argument */
	Assert(config);

	/* Configure the generic clock for the module */
	struct system_gclk_chan_config gclk_chan_conf;
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	gclk_chan_conf.source_generator = GCLK_GENERATOR_0;
	gclk_chan_conf.run_in_standby   = false;
	system_gclk_chan_set_config(NVMCTRL_GCLK_ID, &gclk_chan_conf);
	system_gclk_chan_enable(NVMCTRL_GCLK_ID);

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_BUSY;
	}

	/* Writing configuration to the CTRLB register */
	nvm_module->CTRLB.reg =
			(config->sleep_power_mode  << NVMCTRL_CTRLB_SLEEPPRM_Pos) |
			(config->manual_page_write << NVMCTRL_CTRLB_MANW_Pos) |
			(config->wait_states       << NVMCTRL_CTRLB_RWS_Pos);

	/* Initialize the internal device struct */
	_nvm_dev.page_size       = (8 << nvm_module->PARAM.bit.PSZ);
	_nvm_dev.number_of_pages = nvm_module->PARAM.bit.NVMP;
	_nvm_dev.man_page_write  = config->manual_page_write;

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
 *       completed.
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
 * \retval STATUS_BUSY         If the NVM controller was already busy
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
		return STATUS_BUSY;
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
					NVMCTRL_CTRLA_CMDEX_KEY;
			break;

		/* Commands requiring address */
		case NVM_COMMAND_ERASE_ROW:
		case NVM_COMMAND_WRITE_PAGE:
		case NVM_COMMAND_LOCK_REGION:
		case NVM_COMMAND_UNLOCK_REGION:

			/* Set address and command */
			nvm_module->ADDR.reg  = address;
			nvm_module->CTRLA.reg = (command << NVMCTRL_CTRLA_CMD_Pos) |
					NVMCTRL_CTRLA_CMDEX_KEY;
			break;

		/* Commands not requiring address */
		case NVM_COMMAND_PAGE_BUFFER_CLEAR:
		case NVM_COMMAND_SET_SECURITY_BIT:
		case NVM_COMMAND_SET_POWER_REDUCTION_MODE:
		case NVM_COMMAND_CLEAR_POWER_REDUCTION_MODE:

			/* Set command */
			nvm_module->CTRLA.reg = (command << NVMCTRL_CTRLA_CMD_Pos) |
					NVMCTRL_CTRLA_CMDEX_KEY;
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/**
 * \brief Writes a number of bytes to a page in the NVM memory region
 *
 * Writes from a buffer to a given page number in the NVM memory.
 *
 * \param[in]  destination_page  Destination page index to write to
 * \param[in]  buffer            Pointer to buffer where the data to write is
 *                               stored
 * \param[in]  length            Number of bytes in the page to write
 *
 * \note If writing to a page that has previously been written to, the page's
 *       row should be erased (via \ref nvm_erase_row()) before attempting to
 *       write new data to the page.
 *
 * \return Status of the attempt to write a page.
 *
 * \retval STATUS_OK               Requested NVM memory page was successfully
 *                                 read
 * \retval STATUS_BUSY             NVM controller was busy when the operation
 *                                 was attempted
 * \retval STATUS_ERR_BAD_ADDRESS  The requested page number was outside the
 *                                 acceptable range of the NVM memory region
 * \retval STATUS_ERR_INVALID_ARG  Write length was more than the NVM page size
 */
enum status_code nvm_write_buffer(
		const uint16_t destination_page,
		const uint8_t *buffer,
		uint16_t length)
{
	/* Sanity check arguments */
	if (destination_page > (_nvm_dev.number_of_pages - 1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Ensure the user isn't trying to copy more than a single page worth of
	 * data at once */
	if (length > _nvm_dev.page_size) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_BUSY;
	}

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Read out from NVM memory space */
	memcpy(&NVM_MEMORY[destination_page * _nvm_dev.page_size], buffer, length);

	return STATUS_OK;
}

/**
 * \brief Reads a number of bytes from a page in the NVM memory region
 *
 * Reads a given number of bytes from a given page number in the NVM memory
 * space into a buffer.
 *
 * \param[in]  source_page  Source page index to read from
 * \param[out] buffer       Pointer to a buffer where the content of the read
 *                          page will be stored
 * \param[in]  length       Number of bytes in the page to read
 *
 * \return Status of the page read attempt.
 *
 * \retval STATUS_OK               Requested NVM memory page was successfully
 *                                 read
 * \retval STATUS_BUSY             NVM controller was busy when the operation
 *                                 was attempted
 * \retval STATUS_ERR_BAD_ADDRESS  The requested page number was outside the
 *                                 acceptable range of the NVM memory region
 * \retval STATUS_ERR_INVALID_ARG  Read length was more than the NVM page size
 */
enum status_code nvm_read_buffer(
		const uint16_t source_page,
		uint8_t *const buffer,
		uint16_t length)
{
	/* Sanity check arguments */
	if (source_page > (_nvm_dev.number_of_pages - 1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Ensure the user isn't trying to copy more than a single page worth of
	 * data at once */
	if (length > _nvm_dev.page_size) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_BUSY;
	}

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Copy into NVM memory space */
	memcpy(buffer, &NVM_MEMORY[source_page * _nvm_dev.page_size], length);

	return STATUS_OK;
}

/**
 * \brief Erases a row in the NVM memory space
 *
 * Erases a given row in the NVM memory region.
 *
 * \param[in] row_number  Index of the row to erase
 *
 * \return Status of the NVM row erase attempt.
 *
 * \retval STATUS_OK               Requested NVM memory row was successfully
 *                                 erased
 * \retval STATUS_BUSY             NVM controller was busy when the operation
 *                                 was attempted
 * \retval STATUS_ERR_BAD_ADDRESS  The requested row number was outside the
 *                                 acceptable range of the NVM memory region
 */
enum status_code nvm_erase_row(
		const uint16_t row_number)
{
	/* Check if the row_number is valid */
	if (row_number > ((_nvm_dev.number_of_pages / NVMCTRL_ROW_PAGES) - 1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Get a pointer to the module hardware instance */
	Nvmctrl *const nvm_module = NVMCTRL;

	/* Clear error flags */
	nvm_module->STATUS.reg &= ~NVMCTRL_STATUS_MASK;

	/* Check if the module is busy */
	if (!nvm_is_ready()) {
		return STATUS_BUSY;
	}

	/* Convert row index to a address within NVM memory space */
	uint16_t row_addr = row_number * (_nvm_dev.page_size * NVMCTRL_ROW_PAGES);

	/* Set address and command */
	nvm_module->ADDR.reg  = row_addr;
	nvm_module->CTRLA.reg = NVM_COMMAND_ERASE_ROW | NVMCTRL_CTRLA_CMDEX_KEY;

	return STATUS_OK;
}
