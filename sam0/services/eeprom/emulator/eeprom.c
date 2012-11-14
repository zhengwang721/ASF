#include "asf.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

/* Magic key is the ASCII codes for "AtEEPROMEmu.", which is much faster to check as uint32_t values  
   than a string compare. But this numbers will show up as the string in studio memory view, 
   which makes the data easy to identify */
#define EEPROM_MAGIC_KEY_0                 0x41744545
#define EEPROM_MAGIC_KEY_1                 0x50524f4d
#define EEPROM_MAGIC_KEY_2                 0x456d752e
#define EEPROM_MAGIC_KEY_COUNT             3

#define EEPROM_MAJOR_VERSION               1
#define EEPROM_MINOR_VERSION               0
#define EEPROM_REVISION                    0
#define EEPROM_MASTER_PAGE_NUMBER          0

#define EEPROM_HEADER_SIZE                 4
#define EEPROM_STATUS_BYTE                 0
#define EEPROM_PAGE_NUMBER_BYTE            3
#define EEPROM_DATA_SIZE                   60
#define EEPROM_MAX_PAGES                   124

#define EMULATOR_MAX_PAGES                 256
#define EMULATOR_ROW_SIZE                  256
#define EMULATOR_PAGE_SIZE                 64
#define EMULATOR_PAGES_PR_ROW              4

/**
 * \brief Wrapper struct for the user data pages */
 *
 */
struct eeprom_data {
	/* Header information */
	uint8_t header[EEPROM_HEADER_SIZE];
	/* Data content */
	uint8_t data[EEPROM_DATA_SIZE];
};

/**
 * \brief This struct is used when scaning rows for pages
 *
 */
struct eeprom_page_translater {
	/* Logical page number */
	uint8_t lpage;
	/* Physical page number */
	uint8_t ppage;
};

/**
 * \brief Wrapper struct for the eeprom emulation master page 
 *
 */ 
struct eeprom_master_page {
	/* Magic key which in ASCII will show as AtEEPROMEmu. */
        uint32_t magic_key[3];
	/* Versioning information */
        uint8_t  major_version;
        uint8_t  minor_version;
        uint8_t  revision;

	/* Unused bytes in the page */
	uint8_t  reserved[45];
}

/**
 * \brief Internal device instance struct 
 *
 */

struct {
	bool initalized;
	uint8_t *flash;
	uint32_t flash_start_page;

	uint16_t physical_pages;
	uint8_t logical_pages;

	uint8_t page_map[EMULATOR_MAX_PAGES];

	uint8_t spare_row;

	uint8_t cache_buffer[EMULATOR_PAGE_SIZE];
	bool    cache_active;
	uint8_t cached_page;
} _eeprom_emulator_device;


/**
 * \brief Memory copy function that copy data in 32-bit chunks 
 *
 */
void _eeprom_emulator_copy_memory(uint32_t *dst, uint32_t *src, uint8_t size)
{
	uint8_t c;

	for(c=0;c<size;++c) {
		dst[c] = src[c];
	}
}

/**
 * \brief Function to initialize memory to initial state
 *
 */
void _eeprom_emulator_create_memory(void)
{
	struct eeprom_data data;
	uint16_t ppage, lpage=0;

	for(ppage=0;ppage<(_eeprom_emulator_device.physical_pages  - 4);++ppage)
	{
		// Is this a new row?
		if(ppage%4 == 0) {
			nvm_erase_row(ppage / 4);	
		}

		// Is this the first or second page of a row?
		if((ppage%4 == 0) | (ppage%4 == 1)) {
			data.header[EEPROM_STATUS_BYTE] = 0x40;
			data.header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

			nvm_write_page(_eeprom_emulator_device.flash_page_ofset+ ppage, (uint32_t*)&data);

			lpage++;
		}
	}

	// Set last row as spare row
	_eeprom_emulator_device.spare_row = (_eeprom_emulator_device.physical_pages / 4) - 1;
}

/**
 * \brief This function create a map in SRAM to translate logical page numbers to physical page numbers
 *
 */
void _eeprom_emulator_scan_memory(void)
{
	uint16_t c;
	uint8_t logical_page;
	struct eeprom_data *data_ptr = (struct eeprom_data*)_eeprom_emulation_device.flash;

	for(c=0;c<(_eeprom_emulator_device.physical_pages - 4);++c) {

		logical_page = data_ptr[c].header[EEPROM_PAGE_NUMBER_BYTE];

		if(logical_page != 0xff) {
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
void _eeprom_emulator_clean_memory()
{
	uint16_t c,c2;
	uint8_t current_row, last_row, last_page;
	struct eeprom_data *data_ptr = (struct eeprom_data*)_eeprom_emulation_device.flash;

	for(c=0;c<_eeprom_emulator_device.logical_pages;++c) {

		last_row = 0xff;
		current_row = 0xff;

		for(c2=0;c2<_eeprom_emulator_device.physical_pages;++c2) {
			current_row = c2 / 4;

			if(data_ptr[c2].header[1] == _eeprom_emulator_device.page_map[c]) {
				if(last_row == 0xff) {
					last_row = current_row;
					last_page = c2;
				} else {
					if(last_row != current_row) {

						if(c2%4>1) {
							nvm_erase_row(current_row);
							_eeprom_emulator_device.page_map[c] = last_page;
						} else {
							nvm_erase_row(last_row);
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
bool _eeprom_emulator_find_free_page(uint8_t current_page, uint8_t *new_page)
{
	uint8_t c;
	uint8_t row = current_page / 4;
	uint8_t rest_pages = 4 - (current_page % 4);

	struct eeprom_data *ee_data = (struct eeprom_data*)_eeprom_emulation_device.flash;

	// Is this the first page in a row?
	if((current_page%4) == 0)
		rest_pages = 4;

	if(rest_pages>0) {
		for(c=4-rest_pages;c<4;++c) {
			uint8_t page = (row*4) + c;

			if(ee_data[page].header[EEPROM_PAGE_NUMBER_BYTE] == 0xff) {
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
void _eeprom_emulator_scan_row(uint8_t row, struct page_translater *page_trans)
{
	struct eeprom_data *row_data = (struct eeprom_data*)&_eeprom_emulation_device.flash[row * EMULATOR_ROW_SIZE];
	uint8_t c, c2=0, c3=0;

	// We assume that there are some content on the first tow pages
	page_trans[0].lpage = row_data[0].header[EEPROM_PAGE_NUMBER_BYTE];
	page_trans[0].ppage = row*4;

	page_trans[1].lpage = row_data[1].header[EEPROM_PAGE_NUMBER_BYTE];
	page_trans[1].ppage = (row*4) + 1;

	for(c=0;c<2;++c) {

		for(c2=2;c2<4;++c2) {
			if(page_trans[c].lpage == row_data[c2].header[EEPROM_PAGE_NUMBER_BYTE]) {
				page_trans[c].ppage = (row * 4) + c2;
			}
		}

	}
}

/**
 * \brief Move data to spare row
 *
 */
void _eeprom_emulator_move_data_to_spare(uint8_t row, uint8_t lpage, uint8_t *data)
{
	struct page_translater page_trans[2];
	uint8_t header[4];
	uint8_t c;
	uint32_t new_page;


	// Scan row for content to be copied to spare row
	_eeprom_emulation_scan_row(row, page_trans);

	for(c=0;c<2;++c) {

		new_page = ((_eeprom_emulator_device.spare_row * 4) + c);

		if(lpage == page_trans[c].lpage) {
			header[EEPROM_STATUS_BYTE] = 0x40;
			header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

			/* Flush page buffer (ie. cache) */
			eeprom_flush_page_buffer();

			/* Write header to SRAM cache */
			_eeprom_emulator_copy_memory((uint32_t*)_eeprom_emulator_device.cache_buffer, (uint32_t*)&header, 1);

			/* Write data to SRAM cache */
			_eeprom_emulator_copy_memory((uint32_t*)&_eeprom_emulator_device.cache_buffer[4], (uint32_t*)data, EEPROM_DATA_SIZE / 4);

			/* Write data to page buffer */
			nvm_write_page(new_page + eeprom_emulator_device.flash_start_page, (uint32_t*)_eeprom_emulator_device.cache_buffer);

			_eeprom_emulator_device.page_map[page_trans[c].lpage] = new_page;
			_eeprom_emulator_device.cached_page = page_trans[c].lpage;
			_eeprom_emulator_device.cache_active = true;
		} else {
			/* Flush page buffer (ie. cache) */
			eeprom_flush_page_buffer();

			/* Copy data buffer to cache buffer */
			_eeprom_emulator_copy_memory((uint32_t*)_eeprom_emulator_device.cache_buffer,
					(uint32_t*)_eeprom_emulator_device.flash[page_trans[c].ppage * EMULATOR_PAGE_SIZE], EMULATOR_PAGE_SIZE / 4);

			/* Write data to page buffer */
			nvm_write_page(new_page + eeprom_emulator_device.flash_start_page, (uint32_t*) _eeprom_emulator_device.cache_buffer);

			_eeprom_emulator_device.page_map[page_trans[c].lpage] = new_page;
			_eeprom_emulator_device.cached_page = page_trans[c].lpage;
			_eeprom_emulator_device.cache_active = true; 
		}
	}

	// Erase row and set as spare row
	nvm_erase_row(row + (eeprom_emulator_device.flash_start_page / EMULATOR_PAGES_PR_ROW));

	// Set new spare row
	_eeprom_emulator_device.spare_row = row;

}

/**
 * \brief Create master block
 *
 */
void _eeprom_emulator_create_master_block(void)
{
        uint32_t magic_key[] = {MAGIC_KEY_0, MAGIC_KEY_1, MAGIC_KEY_2};
        uint8_t c;
	struct eeprom_master_page master_page;

	for(c=0;c<EEPROM_MAGIC_KEY_COUNT;++c) {
		master_page.magic_key = magic_key[c];
	}

	master_page.major_version = EEPROM_MAJOR_VERSION;
	master_page.minor_version = EEPROM_MINOR_VERSION;
	master_page.revision      = EEPROM_REVISION;

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

	// The device datasheet is not ready yet, so I do not have these numbers
	_eeprom_emulator_device.flash = _eeprom_emulator_get_start_of_eeprom_memory_ptr();
	_eeprom_emulator_device.flash_page_ofset = (uint32_t)_eeprom_emulator_device.flash / EMULATOR_PAGE_SIZE;

	nvm_get_config_defaults(&config);
	config.manual_page_writes = true;
	nvm_init(&config);

	nvn_get_parameters(&parm);

        _eeprom_emulator_device.physical_pages = parm.number_of_pages;
        _eeprom_emulator_device.logical_pages = (parm.number_of_pages / 2) - EMULATOR_PAGES_PR_ROW;
                
        _eeprom_emulator_device.cache_active = false;
        _eeprom_emulator_device.cached_page = 0xff;

	_eeprom_emulator_scan_memory();

	eeprom_emulator_read_page(EEPROM_MASTER_PAGE_NUMBER, &master_page);

	for(c=0;c<EEPROM_MAGIC_KEY_COUNT;++c) {
		if(master_page.magic_key[c] != magic_key[c]
			return STATUS_ERR_BAD_FORMAT;
	}

}


void eeprom_emulator_write_page(uint8_t lpage, uint8_t *data)
{
	uint8_t eeprom_header[EEPROM_HEADER_SIZE];
	uint8_t new_page;
	enum status_code err;

	eeprom_header[EEPROM_STATUS_BYTE] = 0x40;
	eeprom_header[EEPROM_PAGE_NUMBER_BYTE] = lpage;

	if(_eeprom_emulator_device.cache_active) {
		if(_eeprom_emulator_device.cached_page != lpage) {

			if(err = eeprom_flush_page_buffer() != STATUS_OK)
				return err;

			if(!find_free_page(_eeprom_emulator_device.page_map[lpage], &new_page)) {
				move_data_to_spare(_eeprom_emulator_device.page_map[lpage]  / 4, lpage , data);
				return STATUS_OK;
			}

			_eeprom_emulator_device.cache_active = false;
		}
	}

	_eeprom_emulator_copy_memory((uint32_t*)_eeprom_emulator_device.cache_buffer, (uint32_t*)eeprom_header, EEPROM_HEADER_SIZE / 4);
	_eeprom_emulator_copy_memory((uint32_t*)&_eeprom_emulator_device.cache_buffer[EEPROM_HEADER_SIZE], (uint32_t*)data, EEPROM_DATA_SIZE / 4);

	nvm_write_page(new_page + eeprom_emulation_device.flash_start_page, (uint32_t*)_eeprom_emulator_device.cache_buffer);

	_eeprom_emulator_device.page_map[lpage] = new_page;

	_eeprom_emulator_device.cached_page = lpage;
	_eeprom_emulator_device.cache_active = true;
}

enum status_code eeprom_emulator_read_page(uint8_t page, uint8_t *data)
{
	if(_eeprom_emulator_device.cache_active) {
		if(_eeprom_emulator_device.cached_page == page) {
			_eeprom_emulator_copy_memory((uint32_t*)data, (uint32_t*)&_eeprom_emulator_device.cache_buffer[EEPROM_HEADER_SIZE], EEPROM_DATA_SIZE); 
			return;
		}
	}

	{
		uint32_t flash_addr = (_eeprom_emulator_device.page_map[page] * EMULATOR_PAGE_SIZE) + EEPROM_HEADER_SIZE;
		uint32_t *flash_ptr = (uint32_t*)&_eeprom_emulator_device.flash[addr];

		_eeprom_emulator_copy_memory((uint32_t*)data, flash_ptr, EEPROM_DATA_SIZE);
	}

}

enum status_code eeprom_erase_memory()
{
	//Create new EEPROM memory block in EEPROM emulation section
	_eeprom_emulator_create_memory();

	//Map the newly created EEPROM memory block
	_eeprom_emulator_scan_memory();

	//Write eeprom emulation master block
	_eeprom_emulator_create_master_block();
}

enum status_code eeprom_flush_page_buffer()
{
	uint32_t addr;

	addr = _eeprom_emulator_device.page_map[_eeprom_emulator_device.cached_page];
	addr += _eeprom_emulator_device.start_page;
	addr *= EMULATOR_PAGE_SIZE;

	nvm_execute_command(NVM_COMMAND_WRITE_PAGE, addr, 0);
}

