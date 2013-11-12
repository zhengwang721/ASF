/**
 * \file
 *
 * \brief AT25DFX test application
 *
 */

/**
 * \mainpage AT25DFX test application
 *
 * \par AT25DFX test application
 *
 * This is a simple test application for serial flash AT25DFX.
 *
 * The application connects to the serial flash and performs
 * tests to determine if the driver works as expected.
 *
 * In order to use the application you need:
 * #- Asserts needs to be enabled by adding symbol: _ASSERT_ENABLE_
 * #- Run the code using a debugger
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>


#define AT25DFX_TEST_DATA_SIZE   (10)

static uint8_t example_data[AT25DFX_TEST_DATA_SIZE]= {1, 3, 3, 7, 0, 1, 3, 3, 7, 0};
static uint8_t protect_data[AT25DFX_TEST_DATA_SIZE]= {2, 4, 2, 4, 2, 4, 2, 4, 2, 4};
static uint8_t ram_buff[AT25DFX_TEST_DATA_SIZE];

struct spi_module at25dfx_spi;
struct at25dfx_chip_module at25dfx_chip_1;

static bool test_write(struct at25dfx_chip_module *at25_chip) {
	bool success = true;
	enum status_code status;
	UNUSED(status);

	// Erase block
	status = at25dfx_chip_set_sector_protect(at25_chip, 0x2345, false);
	Assert(status == STATUS_OK);
	status = at25dfx_chip_erase_block(at25_chip, 0x2345, AT25DFX_BLOCK_SIZE_4KB);
	Assert(status == STATUS_OK);

	// Write a bunch of data..
	status = at25dfx_chip_write_buffer(at25_chip, 0x2345, example_data, AT25DFX_TEST_DATA_SIZE);
	Assert(status == STATUS_OK);

	// Verify data written correctly by reading back to ram_buff and comparing
	status = at25dfx_chip_read_buffer(at25_chip, 0x2345, ram_buff, AT25DFX_TEST_DATA_SIZE);
	Assert(status == STATUS_OK);

	for (int i=0; i<AT25DFX_TEST_DATA_SIZE; i++) {
		// Verify that data read back is the same
		if (example_data[i] != ram_buff[i]) {
			Assert(false);
			success = false;
		}
	}
	return success;
}

static bool test_protect(struct at25dfx_chip_module *at25_chip) {
	bool success = true;
	enum status_code status;
	UNUSED(status);

	// Set up sector with some data
	status = at25dfx_chip_set_sector_protect(at25_chip, 0x2345, false);
	Assert(status == STATUS_OK);
	status = at25dfx_chip_erase_block(at25_chip, 0x2345, AT25DFX_BLOCK_SIZE_4KB);
	Assert(status == STATUS_OK);
	status = at25dfx_chip_write_buffer(at25_chip, 0x2345, example_data, AT25DFX_TEST_DATA_SIZE);
	Assert(status == STATUS_OK);

	// Protect sector
	status = at25dfx_chip_set_sector_protect(at25_chip, 0x2345, true);
	Assert(status == STATUS_OK);

	// Writing different data to sector should now be ignored
	status = at25dfx_chip_write_buffer(at25_chip, 0x2345, protect_data, AT25DFX_TEST_DATA_SIZE);
	Assert(status == STATUS_OK);

	// Verify by reading back to ram_buff and check that it is different
	status = at25dfx_chip_read_buffer(at25_chip, 0x2345, ram_buff, AT25DFX_TEST_DATA_SIZE);
	Assert(status == STATUS_OK);

	for (int i=0; i<AT25DFX_TEST_DATA_SIZE; i++) {
		// Verify that data read back is the different
		if (protect_data[i] == ram_buff[i]) {
			Assert(false);
			success = false;
		}
	}
	return success;
}

int main(void)
{
	enum status_code status;
	struct at25dfx_chip_config at25dfx_chip_config;
	struct spi_config at25dfx_spi_config;

	UNUSED(status);

	system_init();

	// Set up the SPI to use for the two serialflash chips
	at25dfx_spi_get_config_defaults(&at25dfx_spi_config);
	at25dfx_spi_config.mode_specific.master.baudrate = 1000000;
	at25dfx_spi_config.mux_setting = EXT1_SPI_SERCOM_MUX_SETTING;
	at25dfx_spi_config.pinmux_pad0 = EXT1_SPI_SERCOM_PINMUX_PAD0;
	at25dfx_spi_config.pinmux_pad1 = PINMUX_UNUSED; // EXT1_SPI_SERCOM_PINMUX_PAD1;
	at25dfx_spi_config.pinmux_pad2 = EXT1_SPI_SERCOM_PINMUX_PAD2;
	at25dfx_spi_config.pinmux_pad3 = EXT1_SPI_SERCOM_PINMUX_PAD3;

	spi_init(&at25dfx_spi, EXT1_SPI_MODULE, &at25dfx_spi_config);
	spi_enable(&at25dfx_spi);

	// Now configure and associate the two chips with the SPI
	at25dfx_chip_config.type = AT25DFX_081A;
	at25dfx_chip_config.cs_pin = EXT1_PIN_SPI_SS_0;

	at25dfx_chip_init(&at25dfx_chip_1, &at25dfx_spi, &at25dfx_chip_config);

	// Check that the devices are actually there
	if (at25dfx_chip_check_presence(&at25dfx_chip_1) != STATUS_OK) {
		// Help! Chip is not connected correctly.
		Assert(false);
	}

	if (!test_write(&at25dfx_chip_1)) {
		// Test failure
		asm("BKPT #0");
	}
	if (!test_protect(&at25dfx_chip_1)) {
		// Test failure
		asm("BKPT #0");
	}

	// Test success
	asm("BKPT #0");
}
