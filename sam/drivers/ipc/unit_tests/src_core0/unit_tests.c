/**
 * \file
 *
 * \brief Unit tests for IPC driver.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#include <asf.h>
#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <sysclk.h>
#include <ipc.h>
#include <string.h>
#include <unit_test/suite.h>
#include <stdio_serial.h>
#include <conf_test.h>
#include <conf_board.h>

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the IPC driver.
 * It consists of test cases for the following functionality:
 * - Load the binary for core 1
 * - Deal with the signal from core 1
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_usart_serial.h
 *
 * \section device_info Device Info
 * All SAM devices with IPC can be used.
 * This example has been tested with the following setup:
 * - sam4c16c_sam4c_ek
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://support.atmel.no/
 */

static bool is_core1_signal = false;

/**
 *  \brief Handler for IPC interrupt request.
 */
static void ipc_core1_signal_handler(Ipc *p, enum ipc_interrupt_source mask)
{
	is_core1_signal = true;
	ipc_clear_command(p, mask);
}

static void fill_sram1_memory(void)
{
	extern char SRAM1_BIN;
	memcpy((char*)IRAM1_ADDR, (char*)&SRAM1_BIN,IRAM1_SIZE);
}

/**
 *  \brief Configure the Console UART.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_TEST_BAUDRATE,
		.paritytype = CONF_TEST_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_TEST_USART, &uart_serial_options);
}

/**
 *  \brief Configure the IPC.
 */
static void configure_ipc(void)
{
	ipc_enable(IPC0);
	ipc_enable(IPC1);

	ipc_set_handler(IPC0, IPC_INTERRUPT_SRC_IRQ0, ipc_core1_signal_handler);
	ipc_enable_interrupt(IPC0, IPC_INTERRUPT_SRC_IRQ0);
	NVIC_EnableIRQ(IPC0_IRQn);
}

/**
 * \brief Test load core 1 binary.
 *
 * \param test Current test case.
 */
static void ipc_load_core1(const struct test_case *test)
{
	is_core1_signal = false;
	rstc_deassert_reset_of_coprocessor(RSTC, RSTC_CPMR_CPROCEN);
	delay_ms(1);
	test_assert_true(test, is_core1_signal, "Load Core 1 failed");
}

/**
 * \brief Test the interrupt handler on both cores.
 *
 * \param test Current test case.
 */
static void ipc_core1_request(const struct test_case *test)
{
	is_core1_signal = false;
	ipc_set_command(IPC1, IPC_INTERRUPT_SRC_IRQ0);
	delay_ms(1);
	test_assert_true(test, is_core1_signal, "No response from Core 1");
}

/**
 * \brief Run IPC driver unit tests
 */
int main(void)
{
	sysclk_init();
	board_init();
	configure_ipc();
	configure_console();

	/* Define all the test cases */
	DEFINE_TEST_CASE(test_ipc_load_core1, NULL,
			ipc_load_core1, NULL,
			"IPC Load Core 1 test");
	DEFINE_TEST_CASE(test_ipc_core1_request, NULL,
			ipc_core1_request, NULL,
			"IPC Core 1 Request test");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(ipc_test_array) = {
		&test_ipc_load_core1,
		&test_ipc_core1_request
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(ipc_test_suite, ipc_test_array,
			"IPC driver test suite");

	fill_sram1_memory();

	/* Run all tests in the test suite */
	test_suite_run(&ipc_test_suite);

	while (1) {
		/* Busy-wait forever */
	}
}
