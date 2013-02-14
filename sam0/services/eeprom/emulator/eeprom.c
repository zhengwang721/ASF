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

/* Magic key is the ASCII codes for "AtEEPROMEmu.", which is much faster to
 * check as uint32_t values than a string compare. But this numbers will show
 * up as the string in studio memory view, which makes the data easy to identify
 * */
#define EEPROM_MAGIC_KEY                   {0x41744545, 0x50524f4d, 0x456d752e}
#define EEPROM_MAGIC_KEY_COUNT             3

/**
 * \internal
 * \brief Structure describing emulated pages of EEPROM data
 */
struct _eeprom_page {
	/** Header information of the EEPROM page. */
	uint8_t header[EEPROM_HEADER_SIZE];
	/** Data content of the EEPROM page. */
	uint8_t data[EEPROM_DATA_SIZE];
};

/**
 * \internal
 * \brief Convenience struct for mapping between logical and physical pages
 */
struct _eeprom_page_translater {
	/** Logical page number. */
	uint8_t lpage;
	/** Physical page number. */
	uint8_t ppage;
};

/**
 * \internal
 * \brief Structure describing the EEPROM Emulation master page
 */
struct _eeprom_master_page {
	/** Magic key which in ASCII will show as AtEEPROMEmu. */
	uint32_t magic_key[EEPROM_MAGIC_KEY_COUNT];

	/** Emulator major version information. */
	uint8_t major_version;
	/** Emulator minor version information. */
	uint8_t minor_version;
	/** Emulator revision version information. */
	uint8_t revision;

	/** Emulator identification value (to distinguish between different emulator
	 *  schemes that carry the same version numbers). */
	uint8_t emulator_id;

	/** Unused reserved bytes in the master page. */
	uint8_t reserved[45];
};

/**
 * \internal
 * \brief Internal device instance struct
 */
struct _eeprom_module {
	/** Initialization state of the EEPROM emulator. */
	bool     initialized;
	/** Absolute byte pointer to the first byte of FLASH where the emulated
	 *  EEPROM is stored. */
	uint8_t *flash;
	/** Physical FLASH page number where the emulated EEPROM is stored. */
	uint32_t flash_start_page;

	/** Number of physical FLASH pages occupied by the EEPROM emulator. */
	uint16_t physical_pages;
	/** Number of logical FLASH pages occupied by the EEPROM emulator. */
	uint8_t  logical_pages;

	/** Mapping array from logical EEPROM pages to physical FLASH pages. */
	uint8_t  page_map[EEPROM_MAX_PAGES];

	/** Row number for the spare row (used by next write). */
	uint8_t  spare_row;

	/** Buffer to hold the currently cached page. */
	uint8_t  cache_buffer[NVMCTRL_PAGE_SIZE];
	/** Indicates if the cache is currently full. */
	bool     cache_active;
	/** Logical EEPROM page number of the currently cached page. */
	uint8_t  cached_page;
};

/**
 * \internal
 * \brief Internal EEPROM emulator instance
 */
static struct _eeprom_module _eeprom_module_inst = {
	.initialized = false,
};


/**
 * \brief Initializes the emulated EEPROM memory, destroying the current contents.
 */
static void _eeprom_emulator_format_memory(void)
{
	enum status_code err = STATUS_OK;
	struct _eeprom_page data;
	uint16_t ppage;
	uint16_t lpage = 0;

	for (ppage = 0; ppage < (_eeprom_module_inst.physical_pages - NVMCTRL_ROW_PAGES); ppage++) {
		/* Is this a new row? */
		if ((ppage % NVMCTRL_ROW_PAGES) == 0) {
			nvm_erase_row(ppage / NVMCTRL_ROW_PAGES);
		}

		/* Is this the first or second page of a row? */
		if ((ppage % NVMCTRL_ROW_PAGES) < 2) {
			data.header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

			do {
				err = nvm_write_buffer(
						_eeprom_module_inst.flash_start_page + ppage,
						(uint8_t *)&data.data,
						NVMCTRL_PAGE_SIZE);
			} while (err == STATUS_BUSY);

			lpage++;
		}
	}

	/* Set last row as spare row */
	_eeprom_module_inst.spare_row
		= (_eeprom_module_inst.physical_pages / NVMCTRL_ROW_PAGES) - 1;
}

/**
 * \brief Creates a map in SRAM to translate logical EEPROM pages to physical FLASH pages
 */
static void _eeprom_emulator_scan_memory(void)
{
	uint8_t logical_page;
	struct _eeprom_page *data_ptr
		= (struct _eeprom_page *)_eeprom_module_inst.flash;

	for (uint16_t c = 0; c < (_eeprom_module_inst.physical_pages - NVMCTRL_ROW_PAGES); c++) {
		logical_page = data_ptr[c].header[EEPROM_PAGE_NUMBER_BYTE];

		if (logical_page != EEPROM_INVALID_PAGE_NUMBER) {
			_eeprom_module_inst.page_map[logical_page] = c;
		}
	}
}

/**
 * \brief This function will cleanup the memory
 *
 * TODO: NOT TESTED YET
 */
static void _eeprom_emulator_clean_memory(void)
{
	enum status_code err = STATUS_OK;
	uint8_t current_row;
	uint8_t last_row;
	uint8_t last_page;
	struct _eeprom_page *data_ptr
		= (struct _eeprom_page *)_eeprom_module_inst.flash;

	for (uint16_t page_counter = 0;
			page_counter < _eeprom_module_inst.logical_pages; page_counter++) {
		last_row = 0xff;
		current_row = 0xff;

		for (uint16_t page_counter2 = 0;
				page_counter2 < _eeprom_module_inst.physical_pages; page_counter2++) {
			current_row = page_counter2 / NVMCTRL_ROW_PAGES;

			if (data_ptr[page_counter2].header[1] !=
					_eeprom_module_inst.page_map[page_counter]) {
				continue;
			}

			if (last_row == 0xff) {
				last_row = current_row;
				last_page = page_counter2;
			} else if (last_row != current_row) {
				if ((page_counter2 % NVMCTRL_ROW_PAGES) > 1) {
					do {
						err = nvm_erase_row(current_row);
					} while (err == STATUS_BUSY);

					_eeprom_module_inst.page_map[page_counter] = last_page;
				} else {
					do {
						err = nvm_erase_row(last_row);
					} while (err == STATUS_BUSY);

					_eeprom_module_inst.page_map[page_counter] = page_counter2;
				}
			}
		}
	}
}

/**
 * \brief Finds the next free page in the given row if one is available
 *
 * \param[in]  current_page  Physical FLASH page index of the row to examine
 * \param[out] new_page      Index of the physical FLASH page that is free
 *
 * \return Whether a free page was found in the specified row.
 *
 * \retval \c true   If a free page was found
 * \retval \c false  If the specified row was full and needs an erase
 */
static bool _eeprom_emulator_is_page_free_on_row(
		uint8_t current_page,
		uint8_t *new_page)
{
	uint8_t row        = (current_page / NVMCTRL_ROW_PAGES);
	uint8_t rest_pages = (current_page % NVMCTRL_ROW_PAGES);

	struct _eeprom_page *ee_data
		= (struct _eeprom_page *)_eeprom_module_inst.flash;

	if (rest_pages > 0) {
		for (uint8_t c = rest_pages; c < NVMCTRL_ROW_PAGES; c++) {
			uint8_t page = (row * NVMCTRL_ROW_PAGES) + c;

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
 * \brief Builds a map of the newest pages in a row
 *
 * Creates a translation map between a logical EEPROM page and physical FLASH
 * page within a row.
 *
 * \param[in]  row         Index of the physical row to examine
 * \param[out] page_trans  Translation map for the given row
 */
static void _eeprom_emulator_scan_row(
		uint8_t row,
		struct _eeprom_page_translater *page_trans)
{
	struct _eeprom_page *row_data
		= (struct _eeprom_page *)&_eeprom_module_inst.flash[row * NVMCTRL_ROW_SIZE];

	/* We assume that there are some content on the first two pages */
	page_trans[0].lpage = row_data[0].header[EEPROM_PAGE_NUMBER_BYTE];
	page_trans[0].ppage = row * NVMCTRL_ROW_PAGES;

	page_trans[1].lpage = row_data[1].header[EEPROM_PAGE_NUMBER_BYTE];
	page_trans[1].ppage = (row * NVMCTRL_ROW_PAGES) + 1;

	for (uint8_t c = 0; c < 2; c++) {
		for (uint8_t c2 = 2; c2 < NVMCTRL_ROW_PAGES; c2++) {
			if (page_trans[c].lpage ==
					row_data[c2].header[EEPROM_PAGE_NUMBER_BYTE]) {
				page_trans[c].ppage = (row * NVMCTRL_ROW_PAGES) + c2;
			}
		}
	}
}

/**
 * \brief Moves data from the specified logical page to the spare row
 *
 * Moves the contents of the specified row into the spare row, so that the
 * original row can be erased and re-used. The contents of the given logical
 * page is replaced with a new buffer of data.
 *
 * \param[in] row    Physical row to examine
 * \param[in] lpage  Logical EEPROM page number stored in the row to update
 * \param[in] data   New data to replace the old in the specified logical page
 *
 * \return Status code indicating the status of the operation.
 */
static enum status_code _eeprom_emulator_move_data_to_spare(
		uint8_t row,
		uint8_t lpage,
		uint8_t *data)
{
	enum status_code err = STATUS_OK;
	struct _eeprom_page_translater page_trans[2];
	uint8_t  header[EEPROM_HEADER_SIZE];
	uint32_t new_page;

	/* Scan row for content to be copied to spare row */
	_eeprom_emulator_scan_row(row, page_trans);

	for (uint8_t c = 0; c < 2; c++) {
		new_page = ((_eeprom_module_inst.spare_row * NVMCTRL_ROW_PAGES) + c);

		if (lpage == page_trans[c].lpage) {
			header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

			/* Flush page buffer (ie. cache) */
			eeprom_emulator_flush_page_buffer();

			/* Write header to SRAM cache */
			memcpy(_eeprom_module_inst.cache_buffer,
					&header, EEPROM_HEADER_SIZE);

			/* Write data to SRAM cache */
			memcpy(&_eeprom_module_inst.cache_buffer[4],
					data, EEPROM_DATA_SIZE);

			/* Write data to page buffer */
			do {
				err = nvm_write_buffer(
						new_page + _eeprom_module_inst.flash_start_page,
						_eeprom_module_inst.cache_buffer,
						NVMCTRL_PAGE_SIZE);
			} while (err == STATUS_BUSY);

			_eeprom_module_inst.page_map[page_trans[c].lpage] = new_page;
			_eeprom_module_inst.cached_page  = page_trans[c].lpage;
			_eeprom_module_inst.cache_active = true;
		} else {
			/* Flush page buffer (ie. cache) */
			eeprom_emulator_flush_page_buffer();

			/* Copy data buffer to cache buffer */
			memcpy(_eeprom_module_inst.cache_buffer,
					&_eeprom_module_inst.flash[page_trans[c].ppage * NVMCTRL_PAGE_SIZE],
					NVMCTRL_PAGE_SIZE);

			do {
				err = nvm_write_buffer(
						new_page + _eeprom_module_inst.flash_start_page,
						_eeprom_module_inst.cache_buffer,
						NVMCTRL_PAGE_SIZE);
			} while (err == STATUS_BUSY);

			_eeprom_module_inst.page_map[page_trans[c].lpage] = new_page;
			_eeprom_module_inst.cached_page  = page_trans[c].lpage;
			_eeprom_module_inst.cache_active = true;
		}
	}

	/* Erase row and set as spare row */
	do {
		err = nvm_erase_row(
				row + (_eeprom_module_inst.flash_start_page / NVMCTRL_ROW_PAGES));
	} while (err == STATUS_BUSY);

	/* Set new spare row */
	_eeprom_module_inst.spare_row = row;

	return err;
}

/**
 * \brief Create master emulated EEPROM management page
 */
static void _eeprom_emulator_create_master_page(void)
{
	const uint32_t magic_key[] = EEPROM_MAGIC_KEY;
	struct _eeprom_master_page master_page;

	for (uint8_t c = 0; c < EEPROM_MAGIC_KEY_COUNT; c++) {
		master_page.magic_key[c] = magic_key[c];
	}

	master_page.major_version = EEPROM_MAJOR_VERSION;
	master_page.minor_version = EEPROM_MINOR_VERSION;
	master_page.revision      = EEPROM_REVISION;
	master_page.emulator_id   = EEPROM_EMULATOR_ID;

	eeprom_emulator_write_page(EEPROM_MASTER_PAGE_NUMBER,
			(uint8_t *)&master_page);
}

/**
 * \brief Initializes the EEPROM Emulator service
 *
 * Initializes the emulated EEPROM memory space; if the emulated EEPROM memory
 * has not been previously initialized, it will need to be explicitly formatted
 * via \ref eeprom_emulator_erase_memory(). The EEPROM memory space will \b not
 * be automatically erased by the initialization function, so that partial data
 * may be recovered by the user application manually if the service is unable to
 * initialize successfully.
 *
 * \return Status code indicating the status of the operation.
 *
 * \retval STATUS_OK              EEPROM emulation service was successfully
 *                                initialized
 * \retval STATUS_ERR_BAD_FORMAT  Emulated EEPROM memory is corrupt or not
 *                                formatted
 */
enum status_code eeprom_emulator_init(void)
{
	const uint32_t magic_key[] = EEPROM_MAGIC_KEY;
	enum status_code err = STATUS_OK;
	struct _eeprom_master_page master_page;
	struct nvm_parameters parm;
	struct nvm_config config;

	/* TODO: Find a function get the start of the EEPROM Emulation section
	 * in flash */
	/* _eeprom_module_inst.flash =
	 * _eeprom_emulator_get_start_of_eeprom_memory_ptr(); */
	_eeprom_module_inst.flash_start_page
		= (uint32_t)_eeprom_module_inst.flash / NVMCTRL_PAGE_SIZE;

	nvm_get_config_defaults(&config);
	config.manual_page_write = true;

	do {
		err = nvm_set_config(&config);
	} while (err == STATUS_BUSY);

	nvm_get_parameters(&parm);

	/* This number is not correct, we have to wait for the device sheet. */
	_eeprom_module_inst.physical_pages = parm.nvm_number_of_pages;
	_eeprom_module_inst.logical_pages  =
			(parm.nvm_number_of_pages / 2) - NVMCTRL_ROW_PAGES;

	_eeprom_module_inst.cache_active = false;
	_eeprom_module_inst.cached_page  = EEPROM_INVALID_PAGE_NUMBER;

	_eeprom_emulator_scan_memory();

	eeprom_emulator_read_page(EEPROM_MASTER_PAGE_NUMBER,
			(uint8_t *)&master_page);

	for (uint8_t c = 0; c < EEPROM_MAGIC_KEY_COUNT; c++) {
		if (master_page.magic_key[c] != magic_key[c]) {
			return STATUS_ERR_BAD_FORMAT;
		}
	}

	if (master_page.emulator_id != EEPROM_EMULATOR_ID) {
		return STATUS_ERR_BAD_FORMAT;
	}

	_eeprom_emulator_clean_memory();

	_eeprom_module_inst.initialized = true;

	return err;
}

/**
 * \brief Writes a page of data to an emulated EEPROM memory page
 *
 * Writes an emulated EEPROM page of data to the emulated EEPROM memory space.
 *
 * \note Data stored in pages may be cached in volatile RAM memory; to commit
 *       any cached data to physical non-volatile memory, the
 *       \ref eeprom_emulator_flush_page_buffer() function should be called.
 *
 * \param[in] page  Logical EEPROM page number to write to
 * \param[in] data  Pointer to the data buffer containing source data to write
 *
 * \return Status code indicating the status of the operation.
 *
 * \retval STATUS_OK                    If the page was successfully read
 * \retval STATUS_ERR_NOT_INITALIZATED  If the EEPROM emulator is not initialized
 * \retval STATUS_ERR_BAD_ADDRESS       If an address outside the valid emulated
 *                                      EEPROM memory space was supplied
 * \retval STATUS_ERR_DENIED            If an attempt was made to write to the
 *                                      maser emulated EEPROM control page
 */
enum status_code eeprom_emulator_write_page(
		uint8_t lpage,
		uint8_t *data)
{
	enum status_code err = STATUS_OK;

	/* Ensure the emulated EEPROM has been initialized first */
	if (_eeprom_module_inst.initialized == false) {
		return STATUS_ERR_NOT_INITALIZATED;
	}

	/* Make sure the write address is within the allowable address space */
	if (lpage >= _eeprom_module_inst.logical_pages) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Make sure the write address is not the master EEPROM page number */
#if 0
	// TODO: Enable once solution found for allowing the service (only) to read
	// and write the master page
	if (lpage == EEPROM_MASTER_PAGE_NUMBER) {
		return STATUS_ERR_DENIED;
	}
#endif

	uint8_t eeprom_header[EEPROM_HEADER_SIZE];
	uint8_t new_page = 0;

	/* Store the logical page number in the physical page header */
	eeprom_header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

	/* Check if the cache is active and the currently cached page is not the
	 * page that is being written (if not, we need to flush and cache the new
	 * page) */
	if ((_eeprom_module_inst.cache_active == true) &&
			(_eeprom_module_inst.cached_page != lpage)) {
		/* Flush the currently cached data buffer to non-volatile memory */
		eeprom_emulator_flush_page_buffer();

		if (_eeprom_emulator_is_page_free_on_row(
				_eeprom_module_inst.page_map[lpage],
				&new_page) == false) {
			_eeprom_emulator_move_data_to_spare(
					_eeprom_module_inst.page_map[lpage] / NVMCTRL_ROW_PAGES,
					lpage,
					data);

			return STATUS_OK;
		}

		_eeprom_module_inst.cache_active = false;
	}

	memcpy(&_eeprom_module_inst.cache_buffer[0],
			eeprom_header,
			EEPROM_HEADER_SIZE);

	memcpy(&_eeprom_module_inst.cache_buffer[EEPROM_HEADER_SIZE],
			data,
			EEPROM_DATA_SIZE);

	do {
		err = nvm_write_buffer(
				new_page + _eeprom_module_inst.flash_start_page,
				_eeprom_module_inst.cache_buffer,
				NVMCTRL_PAGE_SIZE);
	} while (err == STATUS_BUSY);

	_eeprom_module_inst.page_map[lpage] = new_page;
	_eeprom_module_inst.cached_page     = lpage;
	_eeprom_module_inst.cache_active    = true;

	return err;
}

/**
 * \brief Reads a page of data from an emulated EEPROM memory page
 *
 * Reads an emulated EEPROM page of data from the emulated EEPROM memory space.
 *
 * \param[in]  page  Logical EEPROM page number to read from
 * \param[out] data  Pointer to the destination data buffer to fill
 *
 * \return Status code indicating the status of the operation.
 *
 * \retval STATUS_OK                    If the page was successfully read
 * \retval STATUS_ERR_NOT_INITALIZATED  If the EEPROM emulator is not initialized
 * \retval STATUS_ERR_BAD_ADDRESS       If an address outside the valid emulated
 *                                      EEPROM memory space was supplied
 * \retval STATUS_ERR_DENIED            If an attempt was made to read from the
 *                                      maser emulated EEPROM control page
 */
enum status_code eeprom_emulator_read_page(
		uint8_t lpage,
		uint8_t *data)
{
	/* Ensure the emulated EEPROM has been initialized first */
	if (_eeprom_module_inst.initialized == false) {
		return STATUS_ERR_NOT_INITALIZATED;
	}

	/* Make sure the read address is within the allowable address space */
	if (lpage >= _eeprom_module_inst.logical_pages) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	/* Make sure the read address is not the master EEPROM page number */
#if 0
	// TODO: Enable once solution found for allowing the service (only) to read
	// and write the master page
	if (lpage == EEPROM_MASTER_PAGE_NUMBER) {
		return STATUS_ERR_DENIED;
	}
#endif

	/* Check if the page to read is currently cached (and potentially out of
	 * sync/newer than the physical memory) */
	if ((_eeprom_module_inst.cache_active == true) &&
		 (_eeprom_module_inst.cached_page == lpage)) {
		/* Copy the potentially newer cached data into the user buffer */
		memcpy(data,
				&_eeprom_module_inst.cache_buffer[EEPROM_HEADER_SIZE],
				EEPROM_DATA_SIZE);
	} else {
		/* Calculate the absolute FLASH address of the page to read */
		uintptr_t flash_addr =
				(_eeprom_module_inst.page_map[lpage] * NVMCTRL_PAGE_SIZE) +
				EEPROM_HEADER_SIZE;

		/* Copy the data from non-volatile memory into the user buffer */
		memcpy(data,
				&_eeprom_module_inst.flash[flash_addr],
				EEPROM_DATA_SIZE);
	}

	return STATUS_OK;
}

/**
 * \brief Erases the entire emulated EEPROM memory space
 *
 * Erases and re-initializes the emulated EEPROM memory space, destroying any
 * existing data.
 */
void eeprom_emulator_erase_memory(void)
{
	/* Create new EEPROM memory block in EEPROM emulation section */
	_eeprom_emulator_format_memory();

	/* Map the newly created EEPROM memory block */
	_eeprom_emulator_scan_memory();

	/* Write EEPROM emulation master block */
	_eeprom_emulator_create_master_page();
}

/**
 * \brief Writes a buffer of data to the emulated EEPROM memory space
 *
 * Writes a buffer of data to a section of emulated EEPROM memory space. The
 * source buffer may be of any size, and the destination may lie outside of a
 * emulated EEPROM page boundary.
 *
 * \note Data stored in pages may be cached in volatile RAM memory; to commit
 *       any cached data to physical non-volatile memory, the
 *       \ref eeprom_emulator_flush_page_buffer() function should be called.
 *
 * \param[in] offset  Starting byte offset to write to, in emulated EEPROM
 *                    memory space
 * \param[in] data    Pointer to the data buffer containing source data to write
 * \param[in] length  Length of the data to write, in bytes
 *
 * \return Status code indicating the status of the operation.
 */
enum status_code eeprom_emulator_write_buffer(
		uint16_t offset,
		uint8_t *data,
		uint16_t length)
{
	enum status_code err = STATUS_OK;
	uint8_t buffer[EEPROM_DATA_SIZE];
	uint8_t current_page = offset / EEPROM_DATA_SIZE;

	/** Perform the initial page read if the starting offset is not aligned  */
	if (offset % EEPROM_DATA_SIZE) {
		err = eeprom_emulator_read_page(current_page, buffer);

		if (err != STATUS_OK) {
			return err;
		}
	}

	/* Write the specified data to the emulated EEPROM memory space */
	for (uint16_t c = offset; c < (length + offset); c++) {
		/* Copy the next byte of data from the user's buffer to the temporary
		 * buffer */
		buffer[c % EEPROM_DATA_SIZE] = data[c - offset];

		/* Check if we have written up to a new EEPROM page boundary */
		if ((c % EEPROM_DATA_SIZE) == 0) {
			/* Write the current page to non-volatile memory from the temporary
			 * buffer */
			err = eeprom_emulator_write_page(current_page, buffer);

			if (err != STATUS_OK) {
				break;
			}

			/* Read the next page from non-volatile memory into the temporary
			 * buffer in case of a partial page write */
			current_page++;
			err = eeprom_emulator_read_page(current_page, buffer);

			if (err != STATUS_OK) {
				return err;
			}
		}
	}

	return err;
}

/**
 * \brief Reads a buffer of data from the emulated EEPROM memory space
 *
 * Reads a buffer of data from a section of emulated EEPROM memory space. The
 * destination buffer may be of any size, and the source may lie outside of a
 * emulated EEPROM page boundary.
 *
 * \param[in]  offset  Starting byte offset to read from, in emulated EEPROM
 *                     memory space
 * \param[out] data    Pointer to the data buffer containing source data to read
 * \param[in]  length  Length of the data to read, in bytes
 *
 * \return Status code indicating the status of the operation.
 */
enum status_code eeprom_emulator_read_buffer(
		uint16_t offset,
		uint8_t *data,
		uint16_t length)
{
	enum status_code err = STATUS_OK;

	uint8_t buffer[EEPROM_DATA_SIZE];
	uint8_t current_page = offset / EEPROM_DATA_SIZE;

	/** Perform the initial page read if the starting offset is not aligned  */
	if (offset % EEPROM_DATA_SIZE) {
		err = eeprom_emulator_read_page(current_page, buffer);

		if (err != STATUS_OK) {
			return err;
		}
	}

	/* Read in the specified data from the emulated EEPROM memory space */
	for (uint16_t c = offset; c < (length + offset); c++) {
		/* Check if we have read up to a new EEPROM page boundary */
		if ((c % EEPROM_DATA_SIZE) == 0) {
			/* Read the next page from non-volatile memory into the temporary
			 * buffer */
			current_page++;
			err = eeprom_emulator_read_page(current_page, buffer);

			if (err != STATUS_OK) {
				return err;
			}
		}

		/* Copy the next byte of data from the temporary buffer to the user's
		 * buffer */
		data[c - offset] = buffer[c % EEPROM_DATA_SIZE];
	}

	return err;
}

/**
 * \brief Flushes any cached data to physical non-volatile memory
 *
 * Flushes the internal SRAM caches to physical non-volatile memory, to ensure
 * that any outstanding cached data is preserved. This function should be called
 * prior to a system reset or shutdown to prevent data loss.
 *
 * \note This should be the first function executed in a BOD33 Early Warning
 *       callback to ensure that any outstanding cache data is fully written to
 *       prevent data loss.
 *
 *
 * \note This function should also be called before using the NVM controller
 *       directly for any other purposes.
 *
 * \return Status code indicating the status of the operation.
 */
enum status_code eeprom_emulator_flush_page_buffer(void)
{
	enum status_code err = STATUS_OK;

	/* Convert the currently cached memory page to an absolute byte address */
	uintptr_t addr;
	addr  = _eeprom_module_inst.page_map[_eeprom_module_inst.cached_page];
	addr += _eeprom_module_inst.flash_start_page;
	addr *= NVMCTRL_PAGE_SIZE;

	/* Perform the page write to commit the NVM page buffer to FLASH */
	do {
		err = nvm_execute_command(NVM_COMMAND_WRITE_PAGE, addr, 0);
	} while (err == STATUS_BUSY);

	return err;
}
