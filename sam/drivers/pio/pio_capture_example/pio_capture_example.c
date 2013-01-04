/**
 * \file
 *
 * \brief PIO Capture Example.
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
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
 *  \mainpage PIO Parallel Capture Example
 *
 *  \par Purpose
 *
 *  This example demonstrates how to use PIO Parallel Capture function of
 *  the PIO Controller.
 *
 *  \par Requirements
 *
 *  This package can only be used with SAM3/4 MCU featuring PIO Parallel 
 *  Capture function.
 *
 *  \par Description
 *
 *  The PIO Controller integrates an interface that is able to read data from a CMOS
 *  digital image sensor, a high-speed parallel ADC, a DSP synchronous port in
 *  synchronous mode, etc.
 *
 *  The application is composed of 2 softwares:
 *  - one for using the PIO Parallel Capture in send mode.
 *  - one for using the PIO Parallel Capture in receive mode.
 *
 *  Two boards are required to use. Connect one to the other, and put one board
 *  in send mode and the second in receive mode.
 *  Different choices can be selected to use the data enable pins or not, and
 *  to sample all the data or only one out of two.
 *  Pins to be connected between the 2 boards:<br />
 *    PA15 PIODCEN1<br />
 *    PA16 PIODCEN2<br />
 *    PA23 PIODCCLK<br />
 *    PA24 PIODC0<br />
 *    PA25 PIODC1<br />
 *    PA26 PIODC2<br />
 *    PA27 PIODC3<br />
 *    PA28 PIODC4<br />
 *    PA29 PIODC5<br />
 *    PA30 PIODC6<br />
 *    PA31 PIODC7<br />
 *    And, of course: GND<br />
 *
 *  \par Usage
 *
 *  -# Build the program and download it into the evaluation board.
 *  -# On the computer, open and configure a terminal application
 *     (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *    - 115200 bauds
 *    - 8 bits of data
 *    - No parity
 *    - 1 stop bit
 *    - No flow control
 *  -# Connect the first board to the second board by connecting:
 *     PIODCCLK, PIODC[7:0], PIODCEN1, PIODCEN2 and GND.
 *  -# Start the application of the first board.
 *  -# Put the software in send mode.
 *  -# In the terminal window, the
 *     following text should appear (values depend on the board and the chip used):
 *     \code
 *      -- PIO Parallel Capture example --
 *      -- xxxx-xx --
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *      Frequency: xxx MHz.
 *      Press r to Receive data on PPIO Parallel Capture.
 *      Press s to Send data on PIO Parallel Capture.
 *      ** SEND mode **
 *      This is for debug purpose only !
 *      Frequency of PIO controller clock must be strictly superior to 2 times
 *      the frequency of the clock of the device which generates the parallel data.
 *
 *      Please, connect the second board, and put it in receive mode.
 *      Press y to indicate that receiver samples all data.
 *      Press n to indicate that receiver samples data with an even index.
 *      Receiver samples all data, or Receiver samples data with an even index.
 *      Press y to send data without enabling the data enable pins.
 *      Press n to send data with data enable pins enabled.
 *      Send data without enabling the data enable pins, or Send data with both data enable pins enabled.
 *      Send data without enabling the data enable pins.
 *      Press a key
 *      or
 *      Send data with both data enable pins enabled.
 *      Press a key
 *     \endcode
 *  -# Start the application of the second board.
 *  -# Put the software in receive mode.
 *  -# In the terminal window, the
 *     following text should appear (values depend on the board and the chip used):
 *     \code
 *      -- PIO Parallel Capture example --
 *      -- xxxxxx-xx --
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *      Frequency: xxx MHz
 *      Press r to Receive data on PIO Parallel Capture.
 *      Press s to Send data on PIO Parallel Capture.
 *      ** RECEIVE mode **
 *      Press y to sample the data when both data enable pins are enabled.
 *      Press n to sample the data, don't care the status of the data enable pins.
 *      Receive data, don't care the status of the data enable pins, or Receive data when both data enable pins are enabled.
 *      Press y to sample all the data.
 *      Press n to sample the data only one out of two.
 *      Only one out of two data is sampled, with an even index, or All data are sampled.
 *      Waiting...
 *     \endcode
 *
 */

#include <asf.h>
#include "pio_handler_with_capture.h"

/** Buffer size. */
#define SIZE_BUFF_RECEPT               64

/** Configure the PIO capture pins that send out data simulating as a sensor. */
#define PIO_CAPTURE_CCLK_IDX           PIO_PA23_IDX
#define PIO_CAPTURE_EN1_IDX            PIO_PA15_IDX
#define PIO_CAPTURE_EN2_IDX            PIO_PA16_IDX
#define PIO_CAPTURE_PIN_FLAGS          (PIO_OUTPUT_0 | PIO_DEFAULT)
#define PIO_CAPTURE_DATA_PINS_MASK     0xFF000000
#define PIO_CAPTURE_ALL_PIN_MSK        ((1 << 15) | (1 << 16) | (1 << 23) | \
										PIO_CAPTURE_DATA_PINS_MASK)

/** Data offset position. */
#define PIO_CAPTURE_DATA_POS           24

/** PIOA interrupt priority. */
#define PIO_IRQ_PRI                    4

#define STRING_EOL    "\r"
#define STRING_HEADER "-- PIO Parallel Capture example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/** PIO receive buffer. */
uint8_t pio_rx_buffer[SIZE_BUFF_RECEPT];

/** Test if a callback is received. */
volatile uint8_t g_uc_cbk_received = 0;

/** PDC data packet. */
pdc_packet_t packet_t;

/** Pointer to PDC register base. */
Pdc *p_pdc;

/**
 *  \brief Handler for capture function interrupt.
 *
 */
void capture_handler(void)
{
	uint8_t uc_i;

	pio_capture_disable_interrupt(PIOA, (PIO_PCIDR_ENDRX | PIO_PCIDR_RXBUFF));

	puts("End of receive.\r\n");
	for (uc_i = 0; uc_i < SIZE_BUFF_RECEPT; uc_i++) {
		printf("0x%X ", pio_rx_buffer[uc_i]);
	}
	puts("\r\n");
	g_uc_cbk_received = 1;
}

/**
 *  \brief Wait for some time.
 *
 */
static void wait(uint32_t ul_time)
{
	while (ul_time--) {
	}
}

/**
 *  Configure UART for debug message output.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief pio_capture Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 *
 */
int main(void)
{
	uint8_t uc_i;
	uint8_t uc_flag;
	uint32_t ul_mode;
	uint8_t uc_key;
	static uint8_t uc_rx_even_only;
	static uint8_t uc_tx_without_en;

	/* Initialize the SAM system. */
	sysclk_init();
	board_init();

	/* Configure UART for debug message output. */
	configure_console();

	/* Configure PIOA clock. */
	pmc_enable_periph_clk(ID_PIOA);

	/* Output example information. */
	puts(STRING_HEADER);

	printf("Frequency: %d MHz.\r\n",
			(uint8_t) (sysclk_get_cpu_hz() / 1000000));

	puts("Press r to Receive data on PIO Parallel Capture.\r\n");
	puts("Press s to Send data on PIO Parallel Capture.\r\n");
	uc_key = 0;
	while ((uc_key != 'r') && (uc_key != 's')) {
		uart_read(CONSOLE_UART, &uc_key);
	}
	if (uc_key == 'r') {
		puts("** RECEIVE mode **\r\n");

		/* Set up PDC receive buffer, waiting for 64 bytes. */
		packet_t.ul_addr = (uint32_t) pio_rx_buffer;
		packet_t.ul_size = SIZE_BUFF_RECEPT;
		p_pdc = pio_capture_get_pdc_base(PIOA);
		pdc_rx_init(p_pdc, &packet_t, NULL);

		/* Disable all PIOA I/O line interrupt. */
		pio_disable_interrupt(PIOA, 0xFFFFFFFF);

		/* Configure and enable interrupt of PIO. */
		NVIC_DisableIRQ(PIOA_IRQn);
		NVIC_ClearPendingIRQ(PIOA_IRQn);
		NVIC_SetPriority(PIOA_IRQn, PIO_IRQ_PRI);
		NVIC_EnableIRQ(PIOA_IRQn);

		/* Configure the PIO capture interrupt mask. */
		pio_capture_enable_interrupt(PIOA,
				(PIO_PCIER_ENDRX | PIO_PCIER_RXBUFF));

		/* Enable PDC transfer. */
		pdc_enable_transfer(p_pdc, PERIPH_PTCR_RXTEN);

		/* Clear Receive buffer. */
		for (uc_i = 0; uc_i < SIZE_BUFF_RECEPT; uc_i++) {
			pio_rx_buffer[uc_i] = 0;
		}
		/* Initialize PIO capture mode value. */
		ul_mode = 0;
		/* Set up the parallel capture mode data size as 8 bits. */
		ul_mode |= 0 << PIO_PCMR_DSIZE_Pos;

		puts("Press y to sample the data when both data enable pins are enabled.\r\n");
		puts("Press n to sample the data, don't care the status of the data enable pins.\r\n");
		uc_key = 0;
		while ((uc_key != 'y') && (uc_key != 'n')) {
			uart_read(CONSOLE_UART, &uc_key);
		}
		if (uc_key == 'y') {
			/* Sample the data when both data enable pins are enabled. */
			ul_mode &= ~PIO_PCMR_ALWYS;
			puts("Receive data when both data enable pins are enabled.\r\n");
		} else {
			/* Sample the data, don't care the status of the data enable pins. */
			ul_mode |= PIO_PCMR_ALWYS;
			puts("Receive data, don't care the status of the data enable pins.\r\n");
		}
		puts("Press y to sample all the data\r\n");
		puts("Press n to sample the data only one out of two.\r\n");
		uc_key = 0;
		while ((uc_key != 'y') && (uc_key != 'n')) {
			uart_read(CONSOLE_UART, &uc_key);
		}
		if (uc_key == 'y') {
			/* Sample all the data. */
			ul_mode &= ~PIO_PCMR_HALFS;
			puts("All data are sampled.\r\n");
		} else {
			/* Sample the data only one out of two. */
			ul_mode |= PIO_PCMR_HALFS;
			/* Only if half-Sampling is set, data with an even index are sampled. */
			ul_mode &= ~PIO_PCMR_FRSTS;
			puts("Only one out of two data is sampled, with an even index.\r\n");
		}

		while (1) {
			/* Initialize PIO Parallel Capture function. */
			g_uc_cbk_received = 0;
			pio_capture_set_mode(PIOA, ul_mode);
			pio_capture_enable(PIOA);
			puts("Waiting...\r\n");
			while (g_uc_cbk_received == 0) {
			}
		}
	} else if (uc_key == 's') {
		puts("** SEND mode **\r\n");
		puts("This is for debug purpose only !\r\n");
		puts("Frequency of PIO controller clock must be strictly superior");
		puts("to 2 times the frequency of the clock of the device which");
		puts(" generates the parallel data.\r\n");
		puts("\r\nPlease connect the second board, ");
		puts("and put it in receive mode.\r\n");

		/* Configure PIO Parallel Capture pins which simulate as a sensor. */
		pio_configure_pin_group(PIOA, PIO_CAPTURE_ALL_PIN_MSK,
			PIO_CAPTURE_PIN_FLAGS);

		/* Enable sync. output data. */
		pio_enable_output_write(PIOA, PIO_CAPTURE_DATA_PINS_MASK);

		/* Initialize the capture data line. */
		pio_sync_output_write(PIOA, 0);

		puts("Press y to send data with data enable pins.\r\n");
		puts("Press n to send data without data enable pins.\r\n");

		uc_key = 0;
		while ((uc_key != 'y') && (uc_key != 'n')) {
			uart_read(CONSOLE_UART, &uc_key);
		}
		if (uc_key == 'y') {
			uc_tx_without_en = 0;
			puts("Send data with both data enable pins enabled.\r\n");
		} else {
			uc_tx_without_en = 1;
			puts("Send data without enabling the data enable pins.\r\n");
		}

		puts("Press y to indicate that receiver samples all data.\r\n");
		puts("Press n to indicate that receiver samples data with an even index.\r\n");
		uc_key = 0;
		while ((uc_key != 'y') && (uc_key != 'n')) {
			uart_read(CONSOLE_UART, &uc_key);
		}
		if (uc_key == 'y') {
			uc_rx_even_only = 0;
			puts("Receiver samples all data.\r\n");
		} else {
			uc_rx_even_only = 1;
			puts("Receiver samples data with an even index.\r\n");
		}

		uc_flag = 0;
		while (1) {
			if (uc_tx_without_en) {
				puts("\r\nSend data without enabling the data enable pins.\r\n");
			} else {
				puts("\r\nSend data with both data enable pins enabled.\r\n");
			}
			if (!uc_tx_without_en) {
				/* Set enable pins. */
				pio_set_pin_high(PIO_CAPTURE_EN1_IDX);
				pio_set_pin_high(PIO_CAPTURE_EN2_IDX);
			}
			for (uc_i = 0; uc_i < SIZE_BUFF_RECEPT;) {
				/* Send data. */
				pio_sync_output_write(PIOA,
						(uc_i << PIO_CAPTURE_DATA_POS));
				/* Set clock. */
				pio_set_pin_high(PIO_CAPTURE_CCLK_IDX);
				wait(50);
				/* Clear clock. */
				pio_set_pin_low(PIO_CAPTURE_CCLK_IDX);
				wait(50);
				if (uc_rx_even_only) {
					if (!uc_flag) {
						uc_flag = 1;
					} else {
						uc_i++;
						uc_flag = 0;
					}
				} else {
					uc_i++;
				}
			}
			if (!uc_tx_without_en) {
				/* Clear enable pins. */
				pio_set_pin_low(PIO_CAPTURE_EN1_IDX);
				pio_set_pin_low(PIO_CAPTURE_EN2_IDX);
			}
			puts("Press a key.\r\n");
			while (uart_read(CONSOLE_UART, &uc_key)) {
			}
		}
	}

	return 0;
}
