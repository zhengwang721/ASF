/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>


#define AT25DFX_TEST_DATA_SIZE   (1024)


static uint8_t ram_buff[AT25DFX_TEST_DATA_SIZE];

struct at25dfx_spi_module at25dfx_spi;
struct at25dfx_chip_module at25dfx_chip_1;
struct at25dfx_chip_module at25dfx_chip_2;


int main(void)
{
	bool is_protected;
	enum status_code status;
	struct at25dfx_chip_config at25dfx_chip_config;
	struct at25dfx_spi_config at25dfx_spi_config;

	UNUSED(status);

	system_init();

	// Set up the SPI to use for the two serialflash chips
	at25dfx_spi_get_config_defaults(&at25dfx_spi_config);
	at25dfx_spi_config.baudrate = 1000000;
	at25dfx_spi_config.mux_setting = EXT1_SPI_SERCOM_MUX_SETTING;
	at25dfx_spi_config.pinmux_pad0 = EXT1_SPI_SERCOM_PINMUX_PAD0;
	at25dfx_spi_config.pinmux_pad1 = EXT1_SPI_SERCOM_PINMUX_PAD1;
	at25dfx_spi_config.pinmux_pad2 = EXT1_SPI_SERCOM_PINMUX_PAD2;
	at25dfx_spi_config.pinmux_pad3 = EXT1_SPI_SERCOM_PINMUX_PAD3;

	at25dfx_spi_init(&at25dfx_spi, EXT1_SPI_MODULE, &at25dfx_spi_config);

	// Now configure and associate the two chips with the SPI
	at25dfx_chip_config.type = AT25DFX_081A;
	at25dfx_chip_config.cs_pin = EXT1_PIN_SPI_SS_0;

	at25dfx_chip_init(&at25dfx_chip_1, &at25dfx_spi, &at25dfx_chip_config);

	at25dfx_chip_config.type = AT25DFX_041A;
	at25dfx_chip_config.cs_pin = EXT1_PIN_SPI_SS_1;

	at25dfx_chip_init(&at25dfx_chip_2, &at25dfx_spi, &at25dfx_chip_config);


	// Check that the devices are actually there
	if (at25dfx_chip_check_presence(&at25dfx_chip_1) == STATUS_OK) {
		// Help! Something disastrous has happened!
		Assert(false);
	}

	if (at25dfx_chip_check_presence(&at25dfx_chip_2) == STATUS_OK) {
		// Help! Something disastrous has happened!
		Assert(false);
	}

	// Read 10 bytes from location 0x1234 in SerialFlash
	at25dfx_chip_read_buffer(&at25dfx_chip_1, 0x1234, ram_buff, 10);

	// Try to write to a sector -- check first if it is protected
	
	at25dfx_chip_get_sector_protect(&at25dfx_chip_2, 0x2345, &is_protected);
	if (is_protected) {
		// Disable sector (64 kB size) protection
		at25dfx_chip_set_sector_protect(&at25dfx_chip_2, 0x2345, false);
	}
	// Write a bunch of data..
	status = at25dfx_chip_write_buffer(&at25dfx_chip_2, 0x2345, ram_buff,
			AT25DFX_TEST_DATA_SIZE);
	// If status is not OK, the write failed in the chip somehow.
	Assert(status == STATUS_OK);

	// Erase chip 1, but unprotect it first
	status = at25dfx_chip_set_global_sector_protect(&at25dfx_chip_1, false);
	Assert(status == STATUS_OK);
	status = at25dfx_chip_erase(&at25dfx_chip_1);
	Assert(status == STATUS_OK);

	// Now erase the 4 kB block that we started writing into in chip 2
	status = at25dfx_chip_block_erase(&at25dfx_chip_2, 0x2345, AT25DFX_BLOCK_SIZE_4KB);
	Assert(status == STATUS_OK);

	// And protect the 64 kB sector again.
	status = at25dfx_chip_set_sector_protect(&at25dfx_chip_2, 0x2345, true);
	Assert(status == STATUS_OK);
}
