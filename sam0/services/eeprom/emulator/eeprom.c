#include "asf.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

/* This macro will wait until the command return anything else than STATUS_ERR_BUSY
   Expecting err to be defined as "enum status_code err" */
#define wait_for_function(function) \
		do {                    \
			err = #function; \
		} while (err == STATUS_ERR_BUSY);

/* Magic key is the ASCII codes for "AtEEPROMEmu.", which is much faster to
   check as uint32_t values than a string compare. But this numbers will show
   up as the string in studio memory view, which makes the data easy to identify */
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
#define EEPROM_DATA_SIZE                   60
#define EEPROM_MAX_PAGES                   124
#define EEPROM_INVIALID_PAGE_NUMBER        0xff

#define FLASH_MAX_PAGES                    256
#define FLASH_ROW_SIZE                     256
#define FLASH_PAGE_SIZE                    64
#define FLASH_PAGES_PR_ROW                 4

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
	uint8_t  major_version;
	uint8_t  minor_version;
	uint8_t  revision;
	uint8_t  emualtor_id;

	/* Unused bytes in the page */
	uint8_t  reserved[45];
};

/**
 * \brief Internal device instance struct
 *
 */
struct _eeprom_emulator_device_struct {
	bool      initialized;
	uint8_t   *flash;
	uint32_t  flash_start_page;

	uint16_t  physical_pages;
	uint8_t   logical_pages;

	uint8_t   page_map[FLASH_MAX_PAGES];

	uint8_t   spare_row;

	uint8_t   cache_buffer[FLASH_PAGE_SIZE];
	bool      cache_active;
	uint8_t   cached_page;
};

static struct _eeprom_emulator_device_struct _eemprom_emulator_device = {
			initialized = false,
		};

/**
 * \brief Function to initialize memory to initial state
 *
 */
static void _eeprom_emulator_create_memory(void)
{
	struct _eeprom_data data;
	uint16_t ppage;
	uint16_t lpage=0;
	enum status_code err;

	for (ppage = 0; ppage < (_eeprom_emulator_device.physical_pages  - 4); ++ppage) {
		/* Is this a new row? */
		if (ppage%4 == 0) {
			nvm_erase_row(ppage / 4);
		}

		/* Is this the first or second page of a row? */
		if ((ppage%4 == 0) | (ppage%4 == 1)) {
			data.header[EEPROM_STATUS_BYTE] = 0x40;
			data.header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

			wait_for_function(nvm_write_page(_eeprom_emulator_device.
					flash_page_ofset+ ppage, (uint32_t*)&data));

			lpage++;
		}
	}

	/* Set last row as spare row */
	_eeprom_emulator_device.spare_row = (_eeprom_emulator_device.physical_pages / 4) - 1;
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
	struct _eeprom_data *data_ptr = (struct _eeprom_data*)_eeprom_emulation_device.flash;

	for (c = 0; c < (_eeprom_emulator_device.physical_pages - 4); ++c) {

		logical_page = data_ptr[c].header[EEPROM_PAGE_NUMBER_BYTE];

		if (logical_page != EEPROM_INVIALID_PAGE_NUMBER) {
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
	uint16_t c,c2;
	uint8_t current_row;
	uint8_t last_row;
	uint8_t last_page;
	struct _eeprom_data *data_ptr = (struct _eeprom_data*)_eeprom_emulation_device.flash;
	enum status_code err;

	for (c = 0; c < _eeprom_emulator_device.logical_pages; ++c) {

		last_row = 0xff;
		current_row = 0xff;

		for (c2 = 0; c2 < _eeprom_emulator_device.physical_pages; ++c2) {

			current_row = c2 / 4;

			if (data_ptr[c2].header[1] == _eeprom_emulator_device.page_map[c]) {
				if (last_row == 0xff) {

					last_row = current_row;
					last_page = c2;

				} else {
					if (last_row != current_row) {

						if ((c2 % 4) > 1) {
							wait_for_function(nvm_erase_row(current_row));

							_eeprom_emulator_device.page_map[c] = last_page;
						} else {
							wait_for_function(nvm_erase_row(last_row));

							_eeprom_emulator_device.page_map[c] = c2;
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
static bool _eeprom_emulator_is_page_free_on_row(uint8_t current_page, uint8_t *new_page)
{
	uint8_t c;
	uint8_t row = current_page / 4;
	uint8_t rest_pages = 4 - (current_page % 4);

	struct _eeprom_data *ee_data = (struct _eeprom_data*)_eeprom_emulation_device.flash;

	/* Is this the first page in a row? */
	if ((current_page % 4) == 0)
		rest_pages = 4;

	if (rest_pages > 0) {
		for (c = (4 - rest_pages); c < 4; ++c) {
			uint8_t page = (row*4) + c;

			if (ee_data[page].header[EEPROM_PAGE_NUMBER_BYTE] == EEPROM_INVIALID_PAGE_NUMBER) {

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
static void _eeprom_emulator_scan_row(uint8_t row, struct _eeprom_page_translater *page_trans)
{
	struct _eeprom_data *row_data =
			(struct _eeprom_data*)&_eeprom_emulation_device.flash[row * FLASH_ROW_SIZE];
	uint8_t c;
	uint8_t c2=0;
	uint8_t c3=0;

	/* We assume that there are some content on the first tow pages */
	page_trans[0].lpage = row_data[0].header[EEPROM_PAGE_NUMBER_BYTE];
	page_trans[0].ppage = row*4;

	page_trans[1].lpage = row_data[1].header[EEPROM_PAGE_NUMBER_BYTE];
	page_trans[1].ppage = (row*4) + 1;

	for (c = 0; c < 2; ++c) {

		for (c2 = 2;c2 < 4; ++c2) {
			if (page_trans[c].lpage == row_data[c2].header[EEPROM_PAGE_NUMBER_BYTE]) {
				page_trans[c].ppage = (row * 4) + c2;
			}
		}
	}
}

/**
 * \brief Move data to spare row
 *
 */
static enum status_code _eeprom_emulator_move_data_to_spare(uint8_t row, uint8_t lpage, uint8_t *data)
{
	struct page_translater page_trans[2];
	uint8_t header[4];
	uint8_t c;
	uint32_t new_page;
	enum status_code err = STATUS_OK;

	/* Scan row for content to be copied to spare row */
	_eeprom_emulation_scan_row(row, page_trans);

	for (c = 0; c < 2; ++c) {

		new_page = ((_eeprom_emulator_device.spare_row * 4) + c);

		if (lpage == page_trans[c].lpage) {
			header[EEPROM_STATUS_BYTE] = 0x40;
			header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

			/* Flush page buffer (ie. cache) */
			eeprom_flush_page_buffer();

			/* Write header to SRAM cache */
			memcpy((void*)_eeprom_emulator_device.cache_buffer,
					(void*)&header, EEPROM_HEADER_SIZE);

			/* Write data to SRAM cache */
			memcpy((void*)&_eeprom_emulator_device.cache_buffer[4],
					(void*)data, EEPROM_DATA_SIZE);

			/* Write data to page buffer */
			wait_for_function(nvm_write_page(new_page + eeprom_emulator_device.flash_start_page,
					(uint32_t*)_eeprom_emulator_device.cache_buffer));

			_eeprom_emulator_device.page_map[page_trans[c].lpage] = new_page;
			_eeprom_emulator_device.cached_page = page_trans[c].lpage;
			_eeprom_emulator_device.cache_active = true;
		} else {
			/* Flush page buffer (ie. cache) */
			eeprom_flush_page_buffer();

			/* Copy data buffer to cache buffer */
			memcpy((void*)_eeprom_emulator_device.cache_buffer,
					(void*)_eeprom_emulator_device.flash[page_trans[c].ppage *
					FLASH_PAGE_SIZE], FLASH_PAGE_SIZE);

			wait_for_function(nvm_write_page(new_page + eeprom_emulator_device.flash_start_page,
					(uint32_t*) _eeprom_emulator_device.cache_buffer));

			_eeprom_emulator_device.page_map[page_trans[c].lpage] = new_page;
			_eeprom_emulator_device.cached_page = page_trans[c].lpage;
			_eeprom_emulator_device.cache_active = true;
		}
	}

	/* Erase row and set as spare row */
	wait_for_function(nvm_erase_row(row + (eeprom_emulator_device.flash_start_page /
			FLASH_PAGES_PR_ROW)));

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
	uint32_t magic_key[] = {MAGIC_KEY_0, MAGIC_KEY_1, MAGIC_KEY_2};
	uint8_t c;
	struct eeprom_master_page master_page;

	for (c = 0; c < EEPROM_MAGIC_KEY_COUNT; ++c) {
		master_page.magic_key = magic_key[c];
	}

	master_page.major_version = EEPROM_MAJOR_VERSION;
	master_page.minor_version = EEPROM_MINOR_VERSION;
	master_page.revision      = EEPROM_REVISION;
	master_page.emualtor_id   = EEPROM_EMULATOR_ID;

	eeprom_emulator_write_page(EEPROM_MASTER_PAGE_NUMBER, (uint32_t*)&master_page);
}


enum status_code eeprom_emulator_init(void)
{
	struct nvm_parameters parm;
	struct eeprom_master_page master_page;

	struct nvm_config config;

	uint32_t magic_key[] = {MAGIC_KEY_0, MAGIC_KEY_1, MAGIC_KEY_2};
	struct _eeprom_mem_header  = page;
	uint8_t c;
	enum status_code err = STATUS_OK;

	/* The device datasheet is not ready yet, so I do not have these numbers */
	_eeprom_emulator_device.flash = _eeprom_emulator_get_start_of_eeprom_memory_ptr();
	_eeprom_emulator_device.flash_page_ofset =
			(uint32_t)_eeprom_emulator_device.flash / FLASH_PAGE_SIZE;

	nvm_get_config_defaults(&config);
	config.manual_page_writes = true;
	wait_for_function(nvm_init(&config));

	nvn_get_parameters(&parm);

	// This number is not correct, we have to wait for the device sheet. 
	_eeprom_emulator_device.physical_pages = parm.number_of_pages;
	_eeprom_emulator_device.logical_pages = (parm.number_of_pages / 2) - FLASH_PAGES_PR_ROW;

	_eeprom_emulator_device.cache_active = false;
	_eeprom_emulator_device.cached_page = EEPROM_INVIALID_PAGE_NUMBER;

	_eeprom_emulator_scan_memory();

	eeprom_emulator_read_page(EEPROM_MASTER_PAGE_NUMBER, &master_page);

	for(c = 0; c < EEPROM_MAGIC_KEY_COUNT; ++c) {

		if (master_page.magic_key[c] != magic_key[c]) {
			return STATUS_ERR_BAD_FORMAT;
		}

	}

	if(master_page.emulator_id != EEPROM_EMULATOR_ID)
		return STATUS_ERR_BAD_FORMAT;

	_eeprom_emulator_clean_memory();

	_eeprom_emulator_device.initialized = true;

	return err;
}


void eeprom_emulator_write_page(uint8_t lpage, uint8_t *data)
{
	uint8_t eeprom_header[EEPROM_HEADER_SIZE];
	uint8_t new_page;
	enum status_code err = STATUS_OK;

	if (!_eeprom_emulator_device.initialized) {
		return STATUS_ERR_NOT_INITALIZATED;
	}

	if (lpage > (_eeprom_emulator_device.logical_pages-1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	if (lpage == EEPROM_MASTER_PAGE_NUMBER) {
		return STATUS_ERR_DENIED;
	}

	eeprom_header[EEPROM_STATUS_BYTE] = 0x40;
	eeprom_header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

	if (_eeprom_emulator_device.cache_active) {
		if (_eeprom_emulator_device.cached_page != lpage) {

			eeprom_flush_page_buffer();

			if (!_eeprom_emulator_is_page_free_on_row(_eeprom_emulator_device.page_map[lpage], &new_page)) {

				_eeprom_emulator_move_data_to_spare(
						_eeprom_emulator_device.page_map[lpage]  / 4, lpage , data);

				return STATUS_OK;

			}

			_eeprom_emulator_device.cache_active = false;
		}
	}

	memcpy((void*)_eeprom_emulator_device.cache_buffer,
			(void*)eeprom_header, EEPROM_HEADER_SIZE);

	memcpy((void*)&_eeprom_emulator_device.cache_buffer[EEPROM_HEADER_SIZE],
			(void*)data, EEPROM_DATA_SIZE);

	wait_for_function(nvm_write_page(new_page + eeprom_emulation_device.flash_start_page,
			(uint32_t*)_eeprom_emulator_device.cache_buffer));

	_eeprom_emulator_device.page_map[lpage] = new_page;
	_eeprom_emulator_device.cached_page = lpage;
	_eeprom_emulator_device.cache_active = true;

	return err;
}

enum status_code eeprom_emulator_read_page(uint8_t page, uint8_t *data)
{

	if (!_eeprom_emulator_device.initialized) {
		return STATUS_ERR_NOT_INITALIZATED;
	}

	if (lpage > (_eeprom_emulator_device.logical_pages-1)) {
		return STATUS_ERR_BAD_ADDRESS;
	}

	if (lpage == EEPROM_MASTER_PAGE_NUMBER) {
		return STATUS_ERR_DENIED;
	}

	if (_eeprom_emulator_device.cache_active) {
		if (_eeprom_emulator_device.cached_page == lpage) {

			memcpy((void*)data, (void*)&_eeprom_emulator_device.cache_buffer[EEPROM_HEADER_SIZE],
					EEPROM_DATA_SIZE);

		}
	} else {
		uint32_t flash_addr = (_eeprom_emulator_device.page_map[lpage] *
				FLASH_PAGE_SIZE) + EEPROM_HEADER_SIZE;
		uint32_t *flash_ptr = (uint32_t*)&_eeprom_emulator_device.flash[addr];

		memcpy((void*)data, (void*)flash_ptr, EEPROM_DATA_SIZE);
	}

	return STATUS_OK;

}

void eeprom_erase_memory()
{
	/* Create new EEPROM memory block in EEPROM emulation section */
	_eeprom_emulator_create_memory();

	/* Map the newly created EEPROM memory block */
	_eeprom_emulator_scan_memory();

	/* Write eeprom emulation master block */
	_eeprom_emulator_create_master_page();
}

enum status_code eeprom_flush_page_buffer()
{
	uint32_t addr;
	enum status_code err = STATUS_OK;

	addr = _eeprom_emulator_device.page_map[_eeprom_emulator_device.cached_page];
	addr += _eeprom_emulator_device.start_page;
	addr *= FLASH_PAGE_SIZE;

	wait_for_function(nvm_execute_command(NVM_COMMAND_WRITE_PAGE, addr, 0));

	return err;
}

enum status_code eeprom_write_buffer(uint16_t offset, uint8_t *data, uint16_t length)
{
	uint8_t current_page = offset / EEPROM_PAGE_SIZE;
	uint16_t c;
	uint8_t buffer[EEPROM_PAGE_SIZE];
	enum status_code err;

	if (offset % EEPROM_PAGE_SIZE) {
		err = eeprom_emulator_read_page(current_page, buffer);

		if(err != STATUS_OK) {
			return err;
		}

	}

	for (c = offset; c < (length+offset); ++c) {

		if (!(c % EEPROM_PAGE_SIZE)) {

			err = eeprom_emulator_write_page(current_page, buffer);

			if(err != STATUS_OK) {
				break;
			}

			current_page = c / EEPROM_PAGE_SIZE;
			err = eeprom_emulator_read_page(current_page, buffer);

			if(err != STATUS_OK) {
				break;
			}

		}

		buffer[c % EEPROM_PAGE_SIZE] = data[c - offset];
	}
}

enum status_code eeprom_read_buffer(uint16_t offset, uint8_t *data, uint16_t length)
{
	uint8_t current_page = offset / EEPROM_PAGE_SIZE;
	uint16_t c;
	uint8_t buffer[EEPROM_PAGE_SIZE];
	enum status_code err

	if (offset % EEPROM_PAGE_SIZE) {
		err = eeprom_emulator_read_page(current_page, buffer);

		if(err != STATUS_OK) {
			return err;
		}

	}

	for (c = offset; c < (length+offset); ++c) {

		if (!(c%EEPROM_PAGE_SIZE)) {

			current_page = c / EEPROM_PAGE_SIZE;
			err = eeprom_emulator_read_page(current_page, buffer);

			if(err != STATUS_OK) {
				break;
			}

		}

		data[c - offset] = buffer[c % EEPROM_PAGE_SIZE];
	}

	return err;
}
