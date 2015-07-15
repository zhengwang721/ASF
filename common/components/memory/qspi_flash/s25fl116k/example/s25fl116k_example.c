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

/* Chip select. */
#define QSPI_NOT_RELOADED         0
#define QSPI_LASTXFER             1
#define QSPI_SYSTEMATICALLY       2

/* Clock polarity. */
#define QSPI_CLK_POLARITY         0

/* Clock phase. */
#define QSPI_CLK_PHASE            0

/* Delay before SPCK. */
#define QSPI_DLYBS                0x40

/* Delay between consecutive transfers. */
#define QSPI_DLYBCT               0x10

/* QSPI clock setting (Hz). */
static uint32_t gs_ul_spi_clock = 1000000;

qspid_t g_qspid = {
	QSPI,
	mem_mode,
	0,
	0,
	0
};

unsigned char p_buffercode[] = {
	0x50, 0x10, 0x44, 0x20, 0xB5, 0x19, 0x00, 0x80, 0x09, 0x15, 0x00, 0x80,
	0x61, 0x15, 0x00, 0x80, 0x71, 0x15, 0x00, 0x80
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
 *  \brief Handler for System Tick interrupt.
 */
void SysTick_Handler(void)
{
	systick_count ++;
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

	uint8_t *p_memory;

	/* Initialize the system */
	sysclk_init();
	board_init();

	/* Configure the console uart */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Systick configuration. */
	puts("Configure systick to get 1ms tick period.\r");
	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		puts("-F- Systick configuration error\r");
	}
	systick_configured = 1;

	/* Enable SMC peripheral clock */
	pmc_enable_periph_clk(ID_QSPI);

	/* QSPI memory mode configure */
	qspi_memory_mode_initialize();
	puts("QSPI drivers initialized\n\r");

	/* enable quad mode */
	s25fl1d_set_quad_mode(&g_qspid, 1);

	/* Unlock block protection */
	s25fl1d_unprotect(&g_qspid);
	s25fl1d_data_unprotect(&g_qspid);

	/* erase entire chip  */
	s25fl1d_erase_chip(&g_qspid);

	/* Flash the code to QSPI flash */
	puts("Writing to Memory\n");

	s25fl1d_write(&g_qspid, (uint32_t *)p_buffercode, sizeof(p_buffercode), 0, 0);

	/* Lock block protection */
	s25fl1d_protect(&g_qspid);

	printf("\rExample code written 0x%x bytes to Memory\r", sizeof(p_buffercode));

	puts("Verifying \r");
	/* Start continuous read mode to enter in XIP mode*/
	s25fl1d_read_quad_io(&g_qspid, buffer, sizeof(buffer), 0, 1, 0);

	p_memory = (uint8_t *)QSPIMEM_ADDR;
	for(idx = 0; idx < sizeof(p_buffercode); idx++) {
		if(*p_memory == p_buffercode[idx]) {
			p_memory++;
		} else {
			mem_verified = 1;
			printf("\nData does not match at 0x%x \r", p_memory);
			break;
		}
	}
	if(!mem_verified) {
		puts("Everything is OK \r");
		/* set PC and SP */
		__start_new = (uint32_t(*) (void) ) buffer[1];
		__start_sp = buffer[0];

		puts("\n\r Starting getting started example from QSPI flash \n\r");
		puts("========================================================= \n\r");

		__set_MSP(__start_sp);

		__start_new();
	}

	while(1);
}
