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
};

#define AT25DFX_PAGE_SIZE  256
#define AT25DFX_COMMAND_MAX_SIZE (1 + 3 + 2)

enum at25dfx_status_field {
	// These two are read-fields
	AT25DFX_STATUS_BUSY            = (1 << 0),
	AT25DFX_STATUS_ERROR           = (1 << 5),
	// This is a write-field
	AT25DFX_STATUS_GLOBAL_PROTECT  = (0x0f << 2),
};

//! SerialFlash command container
struct at25dfx_command {
	//! Command byte opcode
	enum at25dfx_command_opcode opcode;
	//! Size of command: command byte (1) + address bytes (3) + dummy bytes (N)
	uint8_t command_size;
	//! SerialFlash internal address
	at25dfx_address_t address;
	//! Data buffer to be read from/write to
	uint8_t *data;
	//! Number of bytes to read/write
	at25dfx_datalen_t length;
};

#include <at25dfx_priv_hal.h>

//! \name SerialFlash meta-data helpers
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
	case AT25DFX_041A:
		return 0x0001441F;

	case AT25DFX_161:
		return 0x0002461F;

	case AT25DFX_081A:
		return 0x0001451F;

	case AT25DFX_0161:
		return 0x0000461F;

	case AT25DFX_161A:
		return 0x0001461F;

	case AT25DFX_321:
		return 0x0000471F;

	case AT25DFX_321A:
		return 0x0001471F;

	case AT25DFX_021:
		return 0x0000431F;

	case AT25DFX_641A:
		return 0x0000481F;

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
	case AT25DFX_021:
		return 256 * 1024;

	case AT25DFX_041A:
		return 512 * 1024;

	case AT25DFX_081A:
		return 1 * 1024 * 1024;

	case AT25DFX_161:
	case AT25DFX_0161:
	case AT25DFX_161A:
		return 2 * 1024 * 1024;

	case AT25DFX_321:
	case AT25DFX_321A:
		return 4 * 1024 * 1024;

	case AT25DFX_641A:
		return 8 * 1024 * 1024;

	default:
		Assert(false);
		return 0;
	}
}

//@}

//! \name SPI interface lock/unlock
//@{

static inline enum status_code _at25dfx_lock_spi(struct at25dfx_spi_module *spi)
{
	system_interrupt_enter_critical_section();
	if (spi->locked) {
		system_interrupt_leave_critical_section();
		return STATUS_BUSY;
	} else {
		spi->locked = true;
		system_interrupt_leave_critical_section();
		return STATUS_OK;
	}
}

static inline void _at25dfx_unlock_spi(struct at25dfx_spi_module *spi)
{
	spi->locked = false;
}

//@}

//! \name Chip-level functions
//@{

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
	uint32_t id;

	Assert(chip);

	// Reserve the SPI for us
	status = _at25dfx_lock_spi(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	cmd.opcode = AT25DFX_COMMAND_READ_DEVICE_ID;
	cmd.command_size = 1;
	cmd.data = (uint8_t *)&id;
	cmd.length = 4;
	_at25dfx_chip_issue_read_command_wait(chip, cmd);

	_at25dfx_unlock_spi(chip->spi);

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

	status = _at25dfx_lock_spi(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	cmd.opcode = AT25DFX_COMMAND_READ_ARRAY;
	cmd.command_size = 5;
	cmd.address = address;
	cmd.data = (uint8_t *)&data;
	cmd.length = length;
	_at25dfx_chip_issue_read_command_wait(chip, cmd);

	_at25dfx_unlock_spi(chip->spi);

	return STATUS_OK;
}

enum status_code at25dfx_chip_write_buffer(struct at25dfx_chip_module *chip,
		at25dfx_address_t address, void *data, at25dfx_datalen_t length)
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

	status = _at25dfx_lock_spi(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	_at25dfx_chip_enable_write(chip);

	cmd.opcode = AT25DFX_COMMAND_PROGRAM_PAGE;
	cmd.command_size = 4;
	cmd.address = address;
	cmd.data = (uint8_t *)&data;
	page_bytes = AT25DFX_PAGE_SIZE - (address % AT25DFX_PAGE_SIZE);
	cmd.length = min(page_bytes, length);
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	status = _at25dfx_chip_get_nonbusy_status(chip);

	length -= cmd.length;

	while (length && (status == STATUS_OK)) {
		cmd.address += cmd.length;
		cmd.data += cmd.length;
		cmd.length = min(AT25DFX_PAGE_SIZE, length);

		_at25dfx_chip_issue_write_command_wait(chip, cmd);

		status = _at25dfx_chip_get_nonbusy_status(chip);

		length -= cmd.length;
	}

	_at25dfx_chip_disable_write(chip);

	_at25dfx_unlock_spi(chip->spi);

	return status;
}

enum status_code at25dfx_chip_erase(struct at25dfx_chip_module *chip)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);

	status = _at25dfx_lock_spi(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	_at25dfx_chip_enable_write(chip);

	cmd.opcode = AT25DFX_COMMAND_ERASE_CHIP;
	cmd.command_size = 1;
	cmd.length = 0;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);
	
	status = _at25dfx_chip_get_nonbusy_status(chip);

	_at25dfx_chip_disable_write(chip);

	_at25dfx_unlock_spi(chip->spi);

	return status;
}

enum status_code at25dfx_chip_block_erase(struct at25dfx_chip_module *chip,
		uint32_t address, enum at25dfx_block_size block_size)
{
	enum status_code status;
	struct at25dfx_command cmd;

	Assert(chip);

	if (address >= _at25dfx_get_device_size(chip->type)) {
		return STATUS_ERR_INVALID_ARG;
	}

	status = _at25dfx_lock_spi(chip->spi);
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

	_at25dfx_chip_disable_write(chip);

	_at25dfx_unlock_spi(chip->spi);

	return status;
}

enum status_code at25dfx_chip_set_global_sector_protect(
		struct at25dfx_chip_module *chip, bool protect)
{
	enum status_code status;
	struct at25dfx_command cmd;
	uint8_t temp_data;

	Assert(chip);

	status = _at25dfx_lock_spi(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	_at25dfx_chip_enable_write(chip);

	temp_data = protect ? AT25DFX_STATUS_GLOBAL_PROTECT : 0;
	cmd.opcode = AT25DFX_COMMAND_WRITE_STATUS;
	cmd.command_size = 4;
	cmd.length = 1;
	cmd.data = &temp_data;
	_at25dfx_chip_issue_write_command_wait(chip, cmd);

	_at25dfx_chip_disable_write(chip);

	_at25dfx_unlock_spi(chip->spi);

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

	status = _at25dfx_lock_spi(chip->spi);
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

	_at25dfx_chip_disable_write(chip);

	_at25dfx_unlock_spi(chip->spi);

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

	status = _at25dfx_lock_spi(chip->spi);
	if (status == STATUS_BUSY) {
		return status;
	}

	cmd.opcode = AT25DFX_COMMAND_READ_PROTECT_SECTOR;
	cmd.command_size = 4;
	cmd.address = address;
	cmd.length = 1;
	cmd.data = (uint8_t *)&protect;
	_at25dfx_chip_issue_read_command_wait(chip, cmd);

	_at25dfx_unlock_spi(chip->spi);

	return STATUS_OK;
}