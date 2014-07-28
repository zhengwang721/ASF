/**
 * \file
 *
 * \brief Unit tests for SSC driver.
 *
 * Copyright (c) 2011-2013 Atmel Corporation. All rights reserved.
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

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <sysclk.h>
#include <ssc.h>
#include <string.h>
#include <unit_test/suite.h>
#include <stdio_serial.h>
#include <conf_test.h>
#include <conf_board.h>

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the SSC driver.
 * It tests SSC module in the loop mode, in this case, RD is connected to TD,
 * RF isconnected to TF and RK is connected to TK.
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_uart_serial.h
 *
 * \section device_info Device Info
 * This example has been tested with the following setup:
 * - sam3s4c_sam3s_ek
 * - sam3sd8c_sam3s_ek2
 * - sam3u4e_sam3u_ek
 * - sam3x8h_sam3x_ek
 * - sam4s16c_sam4s_ek
 * - sam4s16c_sam4s_xplained
 * - sam4sd32c_sam4s_ek2
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://www.atmel.com/design-support/
 */

//! \name Unit test configuration
//@{
/**
 * \def CONF_TEST_SSC
 * \brief Check the SSC receive and transmit function.
 */
//@}

/** Transmitter and receiver buffer size. */
#define BUFFER_SIZE            10

/** SSC work bit rate. */
#define BIT_LEN_PER_CHANNEL    8
#define SAMPLE_RATE            8000
#define SSC_BIT_RATE           (BIT_LEN_PER_CHANNEL * SAMPLE_RATE)

/** The SSC interrupt IRQ priority. */
#define SSC_IRQ_PRIO           4

/** Transmit buffer content. */
uint8_t g_uc_tx_buff[BUFFER_SIZE] =
	{ 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89 };

/** Receiver buffer content. */
uint8_t g_uc_rx_buff[BUFFER_SIZE];

/** Receive done flag. */
volatile uint8_t g_uc_rx_done = 0;

/** Receive index. */
uint8_t g_uc_rx_index = 0;

/** Transmit index. */
uint8_t g_uc_tx_index = 0;

/**
 * \brief Synchronous Serial Controller Handler.
 *
 */
void SSC_Handler(void)
{
	uint32_t ul_data;
	ssc_get_status(SSC);

	ssc_read(SSC, &ul_data);
	g_uc_rx_buff[g_uc_rx_index++] = ul_data;

	if (BUFFER_SIZE == g_uc_rx_index) {
		g_uc_rx_done = 1;
		ssc_disable_interrupt(SSC, SSC_IDR_RXRDY);
	}
}

/**
 * \brief Test Synchronous Serial Controller.
 *  This test sets SSC module in the loop mode and check the receiver data
 *  whether it's the same as the transmit data.
 *
 * \param test Current test case.
 */
static void run_ssc_test(const struct test_case *test)
{
	uint32_t ul_mck;
	clock_opt_t tx_clk_option;
	clock_opt_t rx_clk_option;
	data_frame_opt_t rx_data_frame_option;
	data_frame_opt_t tx_data_frame_option;

	/* Initialize the local variable. */
	ul_mck = 0;
	memset((uint8_t *)&rx_clk_option, 0, sizeof(clock_opt_t));
	memset((uint8_t *)&rx_data_frame_option, 0, sizeof(data_frame_opt_t));
	memset((uint8_t *)&tx_clk_option, 0, sizeof(clock_opt_t));
	memset((uint8_t *)&tx_data_frame_option, 0, sizeof(data_frame_opt_t));

	/* Initialize the SSC module and work in loop mode. */
	pmc_enable_periph_clk(ID_SSC);
	ssc_reset(SSC);
	ul_mck = sysclk_get_cpu_hz();
	ssc_set_clock_divider(SSC, SSC_BIT_RATE, ul_mck);

	/* Transmitter clock mode configuration. */
	tx_clk_option.ul_cks = SSC_TCMR_CKS_MCK;
	tx_clk_option.ul_cko = SSC_TCMR_CKO_CONTINUOUS;
	tx_clk_option.ul_cki = 0;
	tx_clk_option.ul_ckg = 0;
	tx_clk_option.ul_start_sel = SSC_TCMR_START_CONTINUOUS;
	tx_clk_option.ul_sttdly = 0;
	tx_clk_option.ul_period = 0;
	/* Transmitter frame mode configuration. */
	tx_data_frame_option.ul_datlen = BIT_LEN_PER_CHANNEL - 1;
	tx_data_frame_option.ul_msbf = SSC_TFMR_MSBF;
	tx_data_frame_option.ul_datnb = 0;
	tx_data_frame_option.ul_fslen = 0;
	tx_data_frame_option.ul_fslen_ext = 0;
	tx_data_frame_option.ul_fsos = SSC_TFMR_FSOS_TOGGLING;
	tx_data_frame_option.ul_fsedge = SSC_TFMR_FSEDGE_POSITIVE;
	/* Configure the SSC transmitter. */
	ssc_set_transmitter(SSC, &tx_clk_option, &tx_data_frame_option);

	/* Receiver clock mode configuration. */
	rx_clk_option.ul_cks = SSC_RCMR_CKS_TK;
	rx_clk_option.ul_cko = SSC_RCMR_CKO_NONE;
	rx_clk_option.ul_cki = 0;
	rx_clk_option.ul_ckg = 0;
	rx_clk_option.ul_start_sel = SSC_RCMR_START_RF_EDGE;
	rx_clk_option.ul_sttdly = 0;
	rx_clk_option.ul_period = 0;
	/* Receiver frame mode configuration. */
	rx_data_frame_option.ul_datlen = BIT_LEN_PER_CHANNEL - 1;
	rx_data_frame_option.ul_msbf = SSC_TFMR_MSBF;
	rx_data_frame_option.ul_datnb = 0;
	rx_data_frame_option.ul_fslen = 0;
	rx_data_frame_option.ul_fslen_ext = 0;
	rx_data_frame_option.ul_fsos = SSC_TFMR_FSOS_NONE;
	rx_data_frame_option.ul_fsedge = SSC_TFMR_FSEDGE_POSITIVE;
	/* Configure the SSC receiver. */
	ssc_set_receiver(SSC, &rx_clk_option, &rx_data_frame_option);

	/* Enable the loop mode. */
	ssc_set_loop_mode(SSC);

	/* Enable the tx and rx function. */
	ssc_enable_rx(SSC);
	ssc_enable_tx(SSC);

	/* Configure the RX interrupt. */
	ssc_enable_interrupt(SSC, SSC_IER_RXRDY);

	/* Enable SSC interrupt line from the core */
	NVIC_DisableIRQ(SSC_IRQn);
	NVIC_ClearPendingIRQ(SSC_IRQn);
	NVIC_SetPriority(SSC_IRQn, SSC_IRQ_PRIO);
	NVIC_EnableIRQ(SSC_IRQn);

	for (g_uc_tx_index = 0; g_uc_tx_index < BUFFER_SIZE; g_uc_tx_index++) {
		ssc_write(SSC, g_uc_tx_buff[g_uc_tx_index]);
	}

	while (!g_uc_rx_done) {
	}

	test_assert_true(test, (memcmp(g_uc_rx_buff, g_uc_tx_buff, BUFFER_SIZE) == 0),
			"Test: SSC received data is not the same as the transmit data!");
}

/**
 * \brief Run SSC driver unit tests.
 */
int main(void)
{
	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_TEST_BAUDRATE,
		.paritytype = CONF_TEST_PARITY
	};

	sysclk_init();
	board_init();

	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	/* Define all the test cases. */
	DEFINE_TEST_CASE(ssc_test, NULL, run_ssc_test, NULL,
		"Init SSC in loop mode and check the received data with transmit data");

	/* Put test case addresses in an array. */
	DEFINE_TEST_ARRAY(ssc_tests) = {
		&ssc_test,
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(ssc_suite, ssc_tests, "SAM SSC driver test suite");

	/* Run all tests in the test suite. */
	test_suite_run(&ssc_suite);

	while (1) {
		/* Busy-wait forever. */
	}
}
