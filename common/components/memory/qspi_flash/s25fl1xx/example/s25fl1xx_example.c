/**
 * \file
 *
 * \brief SDRAMC on QSPI example for SAMV71.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
 *
 * \mainpage QSPI serial memory Example for S25FL116K
 *
 * \section Purpose
 *
 * This example shows how to configure the S25FL116K.
 * It will perform write and read operations to check the configuration.
 *
 * \section Description
 *
 * An accurate one-to-one comparison is processed after the complete SDRAM
 * configuration.
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR EWARM.
 * Other compilers may or may not work.
 *
 * \section Usage
 *
 * -# Build the program and download it into the evaluation board.
 * -# On the computer, open and configure a terminal application
 *    (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear
 *    (values depend on the board and the chip used):
 *    \code
	-- QSPI Example --
	-- xxxxxx-xx
	-- Compiled: xxx xx xxxx xx:xx:xx --
	...
\endcode
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "asf.h"
#include "conf_board.h"
#include "s25fl1xx.h"

#define STRING_EOL    "\r"
#define STRING_HEADER "--QSpi Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/* Clock polarity. */
#define QSPI_CLK_POLARITY         0

/* Clock phase. */
#define QSPI_CLK_PHASE            0

/* Delay before SPCK. */
#define QSPI_DLYBS                0x40

/* Delay between consecutive transfers. */
#define QSPI_DLYBCT               0x10

struct qspid_t g_qspid = {QSPI, 0, 0, 0};

struct qspi_config_t mode_config = {0, false, false, 0, 0, 0, 0, 0, 0, 0, 0, false, false, 0};

uint8_t buffercode[] = {
	#include "getting_started_hex.h"
};

/**
 *  \brief Configure the console uart.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Application entry point for sdramc_example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t mem_verified = 0;
	uint32_t __start_sp, idx;
	uint32_t (*__start_new)(void);
	uint32_t buffer[4];

	uint8_t *memory;
	enum status_code status = STATUS_OK;

	/* Initialize the system */
	sysclk_init();
	board_init();

	/* Configure the console uart */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Enable SMC peripheral clock */
	pmc_enable_periph_clk(ID_QSPI);

	/* QSPI memory mode configure */
	status = s25fl1xx_initialize(g_qspid.qspi_hw, &mode_config, 1);
	if (status == STATUS_OK) {
		puts("QSPI drivers initialized\n\r");
	} else {
		puts("QSPI drivers initialize failed\n\r");
		while (1) {
			/* Capture error */
		}
	}


	/* enable quad mode */
	s25fl1xx_set_quad_mode(&g_qspid, 1);

	/* Unlock block protection */
	s25fl1xx_unprotect(&g_qspid);
	s25fl1xx_data_unprotect(&g_qspid);

	/* erase entire chip  */
	s25fl1xx_erase_chip(&g_qspid);

	/* Flash the code to QSPI flash */
	puts("Writing to Memory\n");

	s25fl1xx_write(&g_qspid, (uint32_t *)buffercode, sizeof(buffercode), 0, 0);

	/* Lock block protection */
	s25fl1xx_protect(&g_qspid);

	printf("\rExample code written 0x%x bytes to Memory\r", sizeof(buffercode));

	puts("Verifying \r");
	/* Start continuous read mode to enter in XIP mode*/
	s25fl1xx_read_quad_io(&g_qspid, buffer, sizeof(buffer), 0, 1, 0);

	memory = (uint8_t *)QSPIMEM_ADDR;
	for (idx = 0; idx < sizeof(buffercode); idx++) {
		if(*memory == buffercode[idx]) {
			memory++;
		} else {
			mem_verified = 1;
			printf("\nData does not match at 0x%x \r", (int)memory);
			break;
		}
	}
	if (!mem_verified) {
		puts("Everything is OK \r");
		/* set PC and SP */
		__start_new = (uint32_t(*) (void)) buffer[1];
		__start_sp = buffer[0];

		puts("\n\r Starting getting started example from QSPI flash \n\r");
		puts("========================================================= \n\r");

		__set_MSP(__start_sp);

		__start_new();
	}

	while(1);
}
