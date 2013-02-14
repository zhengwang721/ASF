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
#include "eeprom.h"
#include <string.h>
#include <nvm.h>

/* This macro will wait until the command return anything else than STATUS_BUSY
 * Expecting err to be defined as "enum status_code err" */
#define wait_for_function(function) \
	do { \
		err = function;	\
	} \
	while (err == STATUS_BUSY); \


/* Magic key is the ASCII codes for "AtEEPROMEmu.", which is much faster to
 * check as uint32_t values than a string compare. But this numbers will show
 * up as the string in studio memory view, which makes the data easy to identify
 * */
#define EEPROM_MAGIC_KEY_0                 0x41744545
#define EEPROM_MAGIC_KEY_1                 0x50524f4d
#define EEPROM_MAGIC_KEY_2                 0x456d752e
#define EEPROM_MAGIC_KEY_COUNT             3

#define EEPROM_MAJOR_VERSION               1
#define EEPROM_MINOR_VERSION               0
#define EEPROM_REVISION                    0
#define EEPROM_EMULATOR_ID                 1
#define EEPROM_MASTER_PAGE_NUMBER          123

#define EEPROM_HEADER_SIZE                 4
#define EEPROM_STATUS_BYTE                 0
#define EEPROM_PAGE_NUMBER_BYTE            3
#define EEPROM_INVALID_PAGE_NUMBER         0xff

#define FLASH_MAX_PAGES                    256

/**
 * \brief Wrapper struct for the user data pages
 *
 */
struct _eeprom_data {
	/* Header information */
	uint8_t header[EEPROM_HEADER_SIZE];
	/* Data content */
	uint8_t data[EEPROM_DATA_SIZE];
};

/**
 * \brief This struct is used when scanning rows for pages
 *
 */
struct _eeprom_page_translater {
	/* Logical page number */
	uint8_t lpage;
	/* Physical page number */
	uint8_t ppage;
};

/**
 * \brief Wrapper struct for the eeprom emulation master page
 *
 */
struct _eeprom_master_page {
	/* Magic key which in ASCII will show as AtEEPROMEmu. */
	uint32_t magic_key[3];

	/* Versioning information */
	uint8_t major_version;
	uint8_t minor_version;
	uint8_t revision;
	uint8_t emulator_id;

	/* Unused bytes in the page */
	uint8_t reserved[45];
};

/**
 * \brief Internal device instance struct
 *
 */
struct _eeprom_emulator_device_struct {
	bool initialized;
	uint8_t *flash;
	uint32_t flash_start_page;

	uint16_t physical_pages;
	uint8_t logical_pages;

	uint8_t page_map[FLASH_MAX_PAGES];

	uint8_t spare_row;

	uint8_t cache_buffer[NVMCTRL_PAGE_SIZE];
	bool cache_active;
	uint8_t cached_page;
};

static struct _eeprom_emulator_device_struct _eeprom_emulator_device = {
	.initialized = false,
};

/**
 * \brief Function to initialize memory to initial state
 *
 */
static void _eeprom_emulator_create_memory(void)
{
	struct _eeprom_data data;
	uint16_t ppage;
	uint16_t lpage = 0;
	enum status_code err = STATUS_OK;

	for (ppage = 0; ppage < (_eeprom_emulator_device.physical_pages  - 4);
			++ppage) {
		/* Is this a new row? */
		if ((ppage % 4) == 0) {
			nvm_erase_row(ppage / 4);
		}

		/* Is this the first or second page of a row? */
		if (((ppage % 4) == 0) | ((ppage % 4) == 1)) {
			data.header[EEPROM_STATUS_BYTE] = 0x40;
			data.header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

			wait_for_function(nvm_write_page(_eeprom_emulator_device
					.
					flash_start_page + ppage,
					(uint32_t *)&data.data));

			lpage++;
		}
	}

	/* Set last row as spare row */
	_eeprom_emulator_device.spare_row
		= (_eeprom_emulator_device.physical_pages / 4) - 1;
}

/**
 * \brief This function create a map in SRAM to translate logical page
 *  numbers to physical page numbers
 *
 */
static void _eeprom_emulator_scan_memory(void)
{
	uint16_t c;
	uint8_t logical_page;
	struct _eeprom_data *data_ptr
		= (struct _eeprom_data *)_eeprom_emulator_device.flash;

	for (c = 0; c < (_eeprom_emulator_device.physical_pages - 4); ++c) {
		logical_page = data_ptr[c].header[EEPROM_PAGE_NUMBER_BYTE];

		if (logical_page != EEPROM_INVALID_PAGE_NUMBER) {
			_eeprom_emulator_device.page_map[logical_page] = c;
		}
	}
}

/**
 * \brief This function will cleanup the memory
 *
 * \note I have not been able make any working testcase for this function yet
 *
 */
static void _eeprom_emulator_clean_memory(void)
{
	uint16_t page_counter;
	uint16_t page_counter2;
	uint8_t current_row;
	uint8_t last_row;
	uint8_t last_page;
	struct _eeprom_data *data_ptr
		= (struct _eeprom_data *)_eeprom_emulator_device.flash;
	enum status_code err = STATUS_OK;

	for (page_counter = 0;
			page_counter < _eeprom_emulator_device.logical_pages;
			++page_counter) {
		last_row = 0xff;
		current_row = 0xff;

		for (page_counter2 =
				0;
				page_counter2 <
				_eeprom_emulator_device.physical_pages;
				++page_counter2) {
			current_row = page_counter2 / 4;

			if (data_ptr[page_counter2].header[1] ==
					_eeprom_emulator_device.page_map[
						page_counter]) {
				if (last_row == 0xff) {
					last_row = current_row;
					last_page = page_counter2;
				} else {
					if (last_row != current_row) {
						if ((page_counter2 % 4) > 1) {
							wait_for_function(nvm_erase_row(
									current_row));

							_eeprom_emulator_device.
							page_map
							[
								page_counter
							]
								= last_page;
						} else {
							wait_for_function(nvm_erase_row(
									last_row));

							_eeprom_emulator_device.
							page_map
							[
								page_counter
							]
								= page_counter2;
						}
					}
				}
			}
		}
	}
}

/**
 * \brief Find new free page
 *
 */
static bool _eeprom_emulator_is_page_free_on_row(uint8_t current_page,
		uint8_t *new_page)
{
	uint8_t c;
	uint8_t row = current_page / 4;
	uint8_t rest_pages = 4 - (current_page % 4);

	struct _eeprom_data *ee_data
		= (struct _eeprom_data *)_eeprom_emulator_device.flash;

	/* Is this the first page in a row? */
	if ((current_page % 4) == 0) {
		rest_pages = 4;
	}

	if (rest_pages > 0) {
		for (c = (4 - rest_pages); c < 4; ++c) {
			uint8_t page = (row * 4) + c;

			if (ee_data[page].header[EEPROM_PAGE_NUMBER_BYTE] ==
					EEPROM_INVALID_PAGE_NUMBER) {
				*new_page = page;
				return true;
			}
		}
	}

	return false;
}

/**
 * \brief Build a map of the newest pages in a row
 *
 */
static void _eeprom_emulator_scan_row(uint8_t row,
		struct _eeprom_page_translater *page_trans)
{
	struct _eeprom_data *row_data
		= (struct _eeprom_data *)&_eeprom_emulator_device.flash[row *
			NVMCTRL_ROW_SIZE];
	uint8_t c;
	uint8_t c2 = 0;

	/* We assume that there are some content on the first two pages */
	page_trans[0].lpage = row_data[0].header[EEPROM_PAGE_NUMBER_BYTE];
	page_trans[0].ppage = row * 4;

	page_trans[1].lpage = row_data[1].header[EEPROM_PAGE_NUMBER_BYTE];
	page_trans[1].ppage = (row * 4) + 1;

	for (c = 0; c < 2; ++c) {
		for (c2 = 2; c2 < 4; ++c2) {
			if (page_trans[c].lpage ==
					row_data[c2].header[
						EEPROM_PAGE_NUMBER_BYTE
					]) {
				page_trans[c].ppage = (row * 4) + c2;
			}
		}
	}
}

/**
 * \brief Move data to spare row
 *
 */
static enum status_code _eeprom_emulator_move_data_to_spare(uint8_t row,
		uint8_t lpage,
		uint8_t *data)
{
	struct _eeprom_page_translater page_trans[2];
	uint8_t header[4];
	uint8_t c;
	uint32_t new_page;
	enum status_code err = STATUS_OK;

	/* Scan row for content to be copied to spare row */
	_eeprom_emulator_scan_row(row, page_trans);

	for (c = 0; c < 2; ++c) {
		new_page = ((_eeprom_emulator_device.spare_row * 4) + c);

		if (lpage == page_trans[c].lpage) {
			header[EEPROM_STATUS_BYTE] = 0x40;
			header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

			/* Flush page buffer (ie. cache) */
			eeprom_emulator_flush_page_buffer();

			/* Write header to SRAM cache */
			memcpy((void *)_eeprom_emulator_device.cache_buffer,
					(void *)&header, EEPROM_HEADER_SIZE);

			/* Write data to SRAM cache */
			memcpy((void *)&_eeprom_emulator_device.cache_buffer[4],
					(void *)data, EEPROM_DATA_SIZE);

			/* Write data to page buffer */
			wait_for_function(nvm_write_page(new_page +
					_eeprom_emulator_device.flash_start_page,
					(uint32_t *)_eeprom_emulator_device.
					cache_buffer));

			_eeprom_emulator_device.page_map[page_trans[c].lpage]
				= new_page;
			_eeprom_emulator_device.cached_page
				= page_trans[c].lpage;
			_eeprom_emulator_device.cache_active = true;
		} else {
			/* Flush page buffer (ie. cache) */
			eeprom_emulator_flush_page_buffer();

			/* Copy data buffer to cache buffer */
			memcpy((void *)_eeprom_emulator_device.cache_buffer,
					(void *)_eeprom_emulator_device.flash[
						page_trans[c].ppage *
						NVMCTRL_PAGE_SIZE],
					NVMCTRL_PAGE_SIZE);

			wait_for_function(nvm_write_page(new_page +
					_eeprom_emulator_device.flash_start_page,
					(uint32_t *)_eeprom_emulator_device.
					cache_buffer));

			_eeprom_emulator_device.page_map[page_trans[c].lpage]
				= new_page;
			_eeprom_emulator_device.cached_page
				= page_trans[c].lpage;
			_eeprom_emulator_device.cache_active = true;
		}
	}

	/* Erase row and set as spare row */
	wait_for_function(nvm_erase_row(row +
			(_eeprom_emulator_device.flash_start_page /
			NVMCTRL_ROW_PAGES)));

	/* Set new spare row */
	_eeprom_emulator_device.spare_row = row;

	return err;
}

/**
 * \brief Create master block
 *
 */
static void _eeprom_emulator_create_master_page(void)
{
	uint32_t magic_key[]
		= {EEPROM_MAGIC_KEY_0, EEPROM_MAGIC_KEY_1,
		   EEPROM_MAGIC_KEY_2};
	uint8_t c;
	struct _eeprom_master_page master_page;

	for (c = 0; c < EEPROM_MAGIC_KEY_COUNT; ++c) {
		master_page.magic_key[c] = magic_key[c];
	}

	master_page.major_version = EEPROM_MAJOR_VERSION;
	master_page.minor_version = EEPROM_MINOR_VERSION;
	master_page.revision      = EEPROM_REVISION;
	master_page.emulator_id   = EEPROM_EMULATOR_ID;

	eeprom_emulator_write_page(EEPROM_MASTER_PAGE_NUMBER,
			(uint8_t *)&master_page);
}

enum status_code eeprom_emulator_init(void)
{
	struct nvm_parameters parm;
	struct _eeprom_master_page master_page;

	struct nvm_config config;

	uint32_t magic_key[]
		= {EEPROM_MAGIC_KEY_0, EEPROM_MAGIC_KEY_1,
		   EEPROM_MAGIC_KEY_2};
	uint8_t c;
	enum status_code err = STATUS_OK;

	/* The device datasheet is not ready yet, so I do not have these numbers
	 **/
	/* TODO: Find a function get the start of the EEPROM Emulation section
	 * in flash */
	/* _eeprom_emulator_device.flash =
	 * _eeprom_emulator_get_start_of_eeprom_memory_ptr(); */
	_eeprom_emulator_device.flash_start_page
		= (uint32_t)_eeprom_emulator_device.flash / NVMCTRL_PAGE_SIZE;

	nvm_get_config_defaults(&config);
	config.manual_page_write = true;
	wait_for_function(nvm_set_config(&config));

	nvm_get_parameters(&parm);

	/* This number is not correct, we have to wait for the device sheet. */
	_eeprom_emulator_device.physical_pages = parm.nvm_number_of_pages;
	_eeprom_emulator_device.logical_pages
		= (parm.nvm_number_of_pages / 2) - NVMCTRL_ROW_PAGES;

	_eeprom_emulator_device.cache_active = false;
	_eeprom_emulator_device.cached_page = EEPROM_INVALID_PAGE_NUMBER;

	_eeprom_emulator_scan_memory();

	eeprom_emulator_read_page(EEPROM_MASTER_PAGE_NUMBER,
			(uint8_t *)&master_page);

	for (c = 0; c < EEPROM_MAGIC_KEY_COUNT; ++c) {
		if (master_page.magic_key[c] != magic_key[c]) {
			return STATUS_ERR_BAD_FORMAT;
		}
	}

	if (master_page.emulator_id != EEPROM_EMULATOR_ID) {
		return STATUS_ERR_BAD_FORMAT;
	}

	_eeprom_emulator_clean_memory();

	_eeprom_emulator_device.initialized = true;

	return err;
}

enum status_code eeprom_emulator_write_page(uint8_t lpage, uint8_t *data)
{
	uint8_t eeprom_header[EEPROM_HEADER_SIZE];
	uint8_t new_page = 0;
	enum status_code err = STATUS_OK;

	if (!_eeprom_emulator_device.initialized) {
		return STATUS_ERR_NOT_INITALIZATED;
	}

	if (lpage > (_eeprom_emulator_device.logical_pages - 1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	if (lpage == EEPROM_MASTER_PAGE_NUMBER) {
		return STATUS_ERR_DENIED;
	}

	eeprom_header[EEPROM_STATUS_BYTE] = 0x40;
	eeprom_header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

	if (_eeprom_emulator_device.cache_active) {
		if (_eeprom_emulator_device.cached_page != lpage) {
			eeprom_emulator_flush_page_buffer();

			if (!_eeprom_emulator_is_page_free_on_row(
					_eeprom_emulator_device.page_map[lpage],
					&new_page)) {
				_eeprom_emulator_move_data_to_spare(
						_eeprom_emulator_device.page_map[
							lpage]  / 4, lpage,
						data);

				return STATUS_OK;
			}

			_eeprom_emulator_device.cache_active = false;
		}
	}

	memcpy((void *)_eeprom_emulator_device.cache_buffer,
			(void *)eeprom_header, EEPROM_HEADER_SIZE);

	memcpy(
			(void *)&_eeprom_emulator_device.cache_buffer[
				EEPROM_HEADER_SIZE],
			(void *)data, EEPROM_DATA_SIZE);

	wait_for_function(nvm_write_page(new_page +
			_eeprom_emulator_device.flash_start_page,
			(uint32_t *)_eeprom_emulator_device.cache_buffer));

	_eeprom_emulator_device.page_map[lpage] = new_page;
	_eeprom_emulator_device.cached_page = lpage;
	_eeprom_emulator_device.cache_active = true;

	return err;
}

enum status_code eeprom_emulator_read_page(uint8_t lpage, uint8_t *data)
{
	if (!_eeprom_emulator_device.initialized) {
		return STATUS_ERR_NOT_INITALIZATED;
	}

	if (lpage > (_eeprom_emulator_device.logical_pages - 1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	if (lpage == EEPROM_MASTER_PAGE_NUMBER) {
		return STATUS_ERR_DENIED;
	}

	if (_eeprom_emulator_device.cache_active) {
		if (_eeprom_emulator_device.cached_page == lpage) {
			memcpy((void *)data,
					(void *)&_eeprom_emulator_device.cache_buffer[
						EEPROM_HEADER_SIZE],
					EEPROM_DATA_SIZE);
		}
	} else {
		uint32_t flash_addr = (_eeprom_emulator_device.page_map[lpage] *
				NVMCTRL_PAGE_SIZE) + EEPROM_HEADER_SIZE;
		uint32_t *flash_ptr
			= (uint32_t *)&_eeprom_emulator_device.flash[
			flash_addr];

		memcpy((void *)data, (void *)flash_ptr, EEPROM_DATA_SIZE);
	}

	return STATUS_OK;
}

void eeprom_emulator_erase_memory(void)
{
	/* Create new EEPROM memory block in EEPROM emulation section */
	_eeprom_emulator_create_memory();

	/* Map the newly created EEPROM memory block */
	_eeprom_emulator_scan_memory();

	/* Write eeprom emulation master block */
	_eeprom_emulator_create_master_page();
}

enum status_code eeprom_emulator_write_buffer(uint16_t offset, uint8_t *data,
		uint16_t length)
{
	uint8_t current_page = offset / EEPROM_DATA_SIZE;
	uint16_t c;
	uint8_t buffer[EEPROM_DATA_SIZE];
	enum status_code err = STATUS_OK;

	if (offset % EEPROM_DATA_SIZE) {
		err = eeprom_emulator_read_page(current_page, buffer);

		if (err != STATUS_OK) {
			return err;
		}
	}

	for (c = offset; c < (length + offset); ++c) {
		if (!(c % EEPROM_DATA_SIZE)) {
			err = eeprom_emulator_write_page(current_page, buffer);

			if (err != STATUS_OK) {
				break;
			}

			current_page = c / EEPROM_DATA_SIZE;
			err = eeprom_emulator_read_page(current_page, buffer);

			if (err != STATUS_OK) {
				break;
			}
		}

		buffer[c % EEPROM_DATA_SIZE] = data[c - offset];
	}

	return err;
}

enum status_code eeprom_emulator_read_buffer(uint16_t offset, uint8_t *data,
		uint16_t length)
{
	uint8_t current_page = offset / EEPROM_DATA_SIZE;
	uint16_t c;
	uint8_t buffer[EEPROM_DATA_SIZE];
	enum status_code err = STATUS_OK;

	if (offset % EEPROM_DATA_SIZE) {
		err = eeprom_emulator_read_page(current_page, buffer);

		if (err != STATUS_OK) {
			return err;
		}
	}

	for (c = offset; c < (length + offset); ++c) {
		if (!(c % EEPROM_DATA_SIZE)) {
			current_page = c / EEPROM_DATA_SIZE;
			err = eeprom_emulator_read_page(current_page, buffer);

			if (err != STATUS_OK) {
				break;
			}
		}

		data[c - offset] = buffer[c % EEPROM_DATA_SIZE];
	}

	return err;
}

enum status_code eeprom_emulator_flush_page_buffer(void)
{
	uint32_t addr;
	enum status_code err = STATUS_OK;

	addr
		= _eeprom_emulator_device.page_map[_eeprom_emulator_device.
			cached_page
			];
	addr += _eeprom_emulator_device.flash_start_page;
	addr *= NVMCTRL_PAGE_SIZE;

	wait_for_function(nvm_execute_command(NVM_COMMAND_WRITE_PAGE, addr, 0));

	return err;
}
