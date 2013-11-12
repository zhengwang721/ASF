/**
 * \file
 *
 * \brief AT25DFx SerialFlash driver implementation.
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

#include "at25dfx.h"

enum at25dfx_command_opcode {
	AT25DFX_COMMAND_PROGRAM_PAGE         = 0x02,
	AT25DFX_COMMAND_READ_STATUS          = 0x05,
	AT25DFX_COMMAND_READ_ARRAY           = 0x0b,
	AT25DFX_COMMAND_READ_DEVICE_ID       = 0x9f,
	AT25DFX_COMMAND_WRITE_ENABLE         = 0x06,
	AT25DFX_COMMAND_WRITE_DISABLE        = 0x04,
	AT25DFX_COMMAND_ERASE_CHIP           = 0xc7,
	AT25DFX_COMMAND_ERASE_BLOCK_4KB      = 0x20,
	AT25DFX_COMMAND_ERASE_BLOCK_32KB     = 0x52,
	AT25DFX_COMMAND_ERASE_BLOCK_64KB     = 0xd8,
	AT25DFX_COMMAND_WRITE_STATUS         = 0x01,
	AT25DFX_COMMAND_PROTECT_SECTOR       = 0x36,
	AT25DFX_COMMAND_UNPROTECT_SECTOR     = 0x39,
	AT25DFX_COMMAND_READ_PROTECT_SECTOR  = 0x3c,
	AT25DFX_COMMAND_SLEEP                = 0xb9,
	AT25DFX_COMMAND_WAKE                 = 0xab,
};

#define AT25DFX_PAGE_SIZE         256
#define AT25DFX_COMMAND_MAX_SIZE  (1 + 3 + 2)

enum at25dfx_status_field {
	// These two are read-fields
	AT25DFX_STATUS_BUSY            = (1 << 0),
	AT25DFX_STATUS_ERROR           = (1 << 5),
	// This is a write-field
	AT25DFX_STATUS_GLOBAL_PROTECT  = (0x0f << 2),
};

struct at25dfx_command {
	// Command byte opcode
	enum at25dfx_command_opcode opcode;
	// Size of command: command byte (1) + address bytes (3) + dummy bytes (N)
	uint8_t command_size;
	// SerialFlash internal address
	at25dfx_address_t address;
	// Data buffer to be read from/write to
	union {
		const uint8_t *tx;
		uint8_t *rx;
	} data;
	// Number of bytes to read/write
	at25dfx_datalen_t length;
};

//! \name SerialFlash chip info helpers
//@{

/**
 * \brief Get the device ID of a specific SerialFlash type.
 *
 * \param type the type or model of SerialFlash.
 *
 * \return the SerialFlash device ID.
 */
static inline uint32_t _at25dfx_get_device_id(enum at25dfx_type type)
{
	switch (type) {
	case AT25DFX_512B:
		return 0x00651f;

	case AT25DFX_021:
		return 0x00431f;

	case AT25DFX_041A:
		return 0x01441f;

	case AT25DFX_081:
		return 0x02451f;

	case AT25DFX_081A:
		return 0x01451f;

	case AT25DFX_161:
		return 0x02461f;

	case AT25DFX_L161:
		return 0x03461f;

	case AT25DFX_Q161:
		return 0x00861f;

	case AT25DFX_321A:
		return 0x01471f;

	case AT25DFX_641:
		return 0x00481f;

	default:
		Assert(false);
		return 0;
	}
}

/**
 * \brief Get the storage size of a specific SerialFlash type.
 *
 * \param type the type or model of SerialFlash.
 *
 * \return the SerialFlash storage size.
 */
static inline uint32_t _at25dfx_get_device_size(enum at25dfx_type type)
{
	switch (type) {
	case AT25DFX_512B:
		return 64 * 1024UL;

	case AT25DFX_021:
		return 256 * 1024UL;

	case AT25DFX_041A:
		return 512 * 1024UL;

	case AT25DFX_081:
	case AT25DFX_081A:
		return 1024 * 1024UL;

	case AT25DFX_161:
	case AT25DFX_L161:
	case AT25DFX_Q161:
		return 2048 * 1024UL;

	case AT25DFX_321A:
		return 4096 * 1024UL;

	case AT25DFX_641:
		return 8192 * 1024UL;

	default:
		Assert(false);
		return 0;
	}
}

//@}

//! \name Chip-level functions
//@{

static inline void _at25dfx_chip_select(struct at25dfx_chip_module *chip)
{
	port_pin_set_output_level(chip->cs_pin, false);
}

static inline void _at25dfx_chip_deselect(struct at25dfx_chip_module *chip)
{
	port_pin_set_output_level(chip->cs_pin, true);
}

#include <at25dfx_priv_hal.h>

static inline void _at25dfx_chip_enable_write(struct at25dfx_chip_module *chip)
{
	struct at25dfx_command cmd;

	cmd.opcode = AT25DFX_COMMAND_WRITE_ENABLE;
	cmd.command_size = 1;
	cmd.length = 0;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);
}

static inline void _at25dfx_chip_disable_write(struct at25dfx_chip_module *chip)
{
	struct at25dfx_command cmd;

	cmd.opcode = AT25DFX_COMMAND_WRITE_DISABLE;
	cmd.command_size = 1;
	cmd.length = 0;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);
}

//@}

/** PUBLIC FUNCTIONS BELOW HERE **/

enum status_code at25dfx_chip_check_presence(struct at25dfx_chip_module *chip)
{
	enum status_code status;
	struct at25dfx_command cmd;
	uint32_t id = 0;

	Assert(chip);

	// Reserve the SPI for us
	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	cmd.opcode = AT25DFX_COMMAND_READ_DEVICE_ID;
	cmd.command_size = 1;
	cmd.data.rx = (uint8_t *)&id;
	cmd.length = 3;
	_at25dfx_chip_issue_read_command_wait(chip, cmd);

	spi_unlock(chip->spi);

	if (id == _at25dfx_get_device_id(chip->type)) {
		return STATUS_OK;
	} else {
		return STATUS_ERR_NOT_FOUND;
	}
}

enum status_code at25dfx_chip_read_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, void *data, at25dfx_datalen_t length)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);
	Assert(data);
	Assert(length);

	// Address out of range?
	if ((address + length) > _at25dfx_get_device_size(chip->type)) {
		return STATUS_ERR_INVALID_ARG;
	}

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	cmd.opcode = AT25DFX_COMMAND_READ_ARRAY;
	cmd.command_size = 5;
	cmd.address = address;
	cmd.data.rx = (uint8_t *)data;
	cmd.length = length;
	_at25dfx_chip_issue_read_command_wait(chip, cmd);

	spi_unlock(chip->spi);

	return STATUS_OK;
}

enum status_code at25dfx_chip_write_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, const void *data, at25dfx_datalen_t length)
{
	at25dfx_datalen_t page_bytes;
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);
	Assert(data);
	Assert(length);

	if ((address + length) > _at25dfx_get_device_size(chip->type)) {
		return STATUS_ERR_INVALID_ARG;
	}

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	_at25dfx_chip_enable_write(chip);

	cmd.opcode = AT25DFX_COMMAND_PROGRAM_PAGE;
	cmd.command_size = 4;
	cmd.address = address;
	cmd.data.tx = (uint8_t *)data;
	page_bytes = AT25DFX_PAGE_SIZE - (address % AT25DFX_PAGE_SIZE);
	cmd.length = min(page_bytes, length);
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	status = _at25dfx_chip_get_nonbusy_status(chip);

	length -= cmd.length;

	while (length && (status == STATUS_OK)) {
		_at25dfx_chip_enable_write(chip);

		cmd.address += cmd.length;
		cmd.data.tx += cmd.length;
		cmd.length = min(AT25DFX_PAGE_SIZE, length);

		_at25dfx_chip_issue_write_command_wait(chip, cmd);

		status = _at25dfx_chip_get_nonbusy_status(chip);

		length -= cmd.length;
	}

	spi_unlock(chip->spi);

	return status;
}

enum status_code at25dfx_chip_erase(struct at25dfx_chip_module *chip)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	_at25dfx_chip_enable_write(chip);

	cmd.opcode = AT25DFX_COMMAND_ERASE_CHIP;
	cmd.command_size = 1;
	cmd.length = 0;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	status = _at25dfx_chip_get_nonbusy_status(chip);

	spi_unlock(chip->spi);

	return status;
}

enum status_code at25dfx_chip_erase_block(struct at25dfx_chip_module *chip,
		uint32_t address, enum at25dfx_block_size block_size)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);

	if (address >= _at25dfx_get_device_size(chip->type)) {
		return STATUS_ERR_INVALID_ARG;
	}

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	_at25dfx_chip_enable_write(chip);

	switch (block_size) {
	case AT25DFX_BLOCK_SIZE_4KB:
		cmd.opcode = AT25DFX_COMMAND_ERASE_BLOCK_4KB;
		break;

	case AT25DFX_BLOCK_SIZE_32KB:
		cmd.opcode = AT25DFX_COMMAND_ERASE_BLOCK_32KB;
		break;

	case AT25DFX_BLOCK_SIZE_64KB:
		cmd.opcode = AT25DFX_COMMAND_ERASE_BLOCK_64KB;
		break;

	default:
		Assert(false);
		cmd.opcode = 0;
	}
	cmd.command_size = 4;
	cmd.address = address;
	cmd.length = 0;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	status = _at25dfx_chip_get_nonbusy_status(chip);

	spi_unlock(chip->spi);

	return status;
}

enum status_code at25dfx_chip_set_global_sector_protect(
		struct at25dfx_chip_module *chip, bool protect)
{
	enum status_code status;
	struct at25dfx_command cmd;
	uint8_t temp_data;

	Assert(chip);

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	_at25dfx_chip_enable_write(chip);

	temp_data = protect ? AT25DFX_STATUS_GLOBAL_PROTECT : 0;
	cmd.opcode = AT25DFX_COMMAND_WRITE_STATUS;
	cmd.command_size = 1;
	cmd.length = 1;
	cmd.data.tx = &temp_data;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	spi_unlock(chip->spi);

	return STATUS_OK;
}

enum status_code at25dfx_chip_set_sector_protect(
		struct at25dfx_chip_module *chip, at25dfx_address_t address,
		bool protect)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);

	if ((address) >= _at25dfx_get_device_size(chip->type)) {
		return STATUS_ERR_INVALID_ARG;
	}

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	_at25dfx_chip_enable_write(chip);

	cmd.opcode = protect ?
			AT25DFX_COMMAND_PROTECT_SECTOR : AT25DFX_COMMAND_UNPROTECT_SECTOR;
	cmd.command_size = 4;
	cmd.address = address;
	cmd.length = 0;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	spi_unlock(chip->spi);

	return STATUS_OK;
}

enum status_code at25dfx_chip_get_sector_protect(
		struct at25dfx_chip_module *chip, at25dfx_address_t address,
		bool *protect)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);

	if ((address) >= _at25dfx_get_device_size(chip->type)) {
		return STATUS_ERR_INVALID_ARG;
	}

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	cmd.opcode = AT25DFX_COMMAND_READ_PROTECT_SECTOR;
	cmd.command_size = 4;
	cmd.address = address;
	cmd.length = 1;
	cmd.data.rx = (uint8_t *)protect;
	_at25dfx_chip_issue_read_command_wait(chip, cmd);

	spi_unlock(chip->spi);

	return STATUS_OK;
}

enum status_code at25dfx_chip_sleep(struct at25dfx_chip_module *chip)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	cmd.opcode = AT25DFX_COMMAND_SLEEP;
	cmd.command_size = 1;
	cmd.length = 0;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	spi_unlock(chip->spi);

	return STATUS_OK;
}

enum status_code at25dfx_chip_wake(struct at25dfx_chip_module *chip)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);

	status = spi_lock(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	cmd.opcode = AT25DFX_COMMAND_WAKE;
	cmd.command_size = 1;
	cmd.length = 0;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	spi_unlock(chip->spi);

	return STATUS_OK;
}