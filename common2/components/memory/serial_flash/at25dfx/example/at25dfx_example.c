/**
 * \file
 *
 * \brief AT25DFx SerialFlash Component Example.
 *
 * Copyright (c) 2012-2013 Atmel Corporation. All rights reserved.
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

/**
 * \mainpage AT25 SerialFlash example
 *
 * \section Purpose
 *
 * This example demonstrates how to use the Atmel SerialFlash AT25DFx component.
 * The SerialFlash requires an SPI bus as a low level communication interface. This
 * interface can be implemented using SPI or USART in SPI mode for SAM devices.
 *
 * \section Requirements
 *
 * This package can be used with SAM3N, SAM4L, SAM4E and SAM4C evaluation kits.
 *
 * \section Description
 *
 * This example will do the following operations.
 *   - Send "Read Status" command to the SerialFlash.
 *   - Read back the status of the SerialFlash.
 *   - Write two sectors to the SerialFlash.
 *   - Read back these sectors and check correct content.
 *   - If all operations are correct, \ref DATA_FLASH_LED_EXAMPLE_0 is 'on' and
 *     \ref DATA_FLASH_LED_EXAMPLE_1 is 'on'(If have).
 *     The other states of  LEDs mean that the SerialFlash access fails somewhere.
 *
 * \section Usage
 *
 * -# Build the program and download it into the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on the solutions that users choose.
 * -# Some texts, images and basic shapes should be displayed on the LCD.
 *
 */

#include <asf.h>
#include "conf_board.h"

/** Test size */
#define AT25DFX_TEST_DATA_SIZE   (1024)

/** Test block start address */
#define AT25DFX_TEST_BLOCK_ADDR  (0)


/** RAM buffer used in this example */
static uint8_t ram_buff[AT25DFX_TEST_DATA_SIZE];


/**
 * \brief Entry point when test is failed.
 */
void test_ko(void);


struct at25dfx_spi_module at25dfx_spi;
struct at25dfx_chip_module at25dfx_chip_1;
struct at25dfx_chip_module at25dfx_chip_2;


int main(void)
{
	bool is_protected;
	enum status_code status;
	struct at25dfx_chip_config at25dfx_chip_config;
	struct at25dfx_spi_config at25dfx_spi_config;

	system_init();

	// Set up the SPI to use for the two serialflash chips
	at25dfx_get_config_defaults(&at25dfx_spi_config);
	at25dfx_spi_config.baudrate = 1000000;
	at25dfx_spi_config.mux_setting = EXT1_SPI_SERCOM_MUX_SETTING;
	at25dfx_spi_config.pinmux_pad0 = EXT1_SPI_SERCOM_PINMUX_PAD0;
	at25dfx_spi_config.pinmux_pad1 = EXT1_SPI_SERCOM_PINMUX_PAD1;
	at25dfx_spi_config.pinmux_pad2 = EXT1_SPI_SERCOM_PINMUX_PAD2;
	at25dfx_spi_config.pinmux_pad3 = EXT1_SPI_SERCOM_PINMUX_PAD3;

	at25dfx_init(&at25dfx_spi, EXT1_SPI_MODULE, &at25dfx_spi_config);

	// Now configure and associate the two chips with the SPI
	at25dfx_chip_get_config_defaults(&at25dfx_chip_config);
	at25dfx_chip_config.type = AT25DFX_081A;
	at25dfx_chip_config.ss_pin = EXT1_PIN_SPI_SS_0;

	at25dfx_chip_init(&at25dfx_chip_1, &at25dfx_spi, &at25dfx_chip_config);

	at25dfx_chip_config.type = AT25DFX_041A;
	at25dfx_chip_config.ss_pin = EXT1_PIN_SPI_SS_1;

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
	// Write a bunch of data -- nevermind that we read past end of ram_buf
	status = at25dfx_chip_write_buffer(&at25dfx_chip_2, 0x2345, ram_buff,
			2 * AT25DFX_PAGE_SIZE);
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