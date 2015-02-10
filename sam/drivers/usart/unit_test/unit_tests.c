/**
 * \file
 *
 * \brief Unit tests for USART driver.
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


#include <stdio_serial.h>
#include <conf_test.h>
#include "asf.h"

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the USART driver.
 * It consists of test cases for the following functionalities:
 * - USART async mode test
 * - USART sync mode test
 * - USART Write Protection
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 *
 * \section device_info Device Info
 * All SAM devices can be used.
 * This example has been tested with the following setup:
 * - samg55_xplained_pro
 *
 * To run the test:
 *  - Connect the supported Xplained Pro board to the computer using a
 *    micro USB cable.
 *  - Open the virtual COM port in a terminal application.
 *    \note The USB composite firmware running on the Embedded Debugger (EDBG)
 *          will enumerate as debugger, virtual COM port and EDBG data
 *          gateway.
 *  - Build the project, program the target and run the application.
 *  - Conect usart as follows:
 *  EXT1_PIN14(PA10) -- EXT4_PIN12(PB11)
 *  EXT3_PIN14(PB00) -- EXT3_PIN08(PB13)
 *  EXT1_PIN06(PA25) -- EXT3_PIN06(PB15)
 *
 *    The terminal shows the results of the unit test.
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 /* For further information, visit
 * <a href="http://www.atmel.com">http://www.atmel.com</a>.
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

/* structures for transmit and receive*/
uint32_t tx_data, rx_data;

/**
 *  \brief transfer data between 2 usarts.
 */
static void data_transfer(void)
{
	/* Enable transmit*/
	usart_enable_tx(BOARD_USART1);
	/* Enable receive*/
	usart_enable_rx(BOARD_USART2);
	/* Send out data*/
	while(!(usart_is_tx_ready(BOARD_USART1))){};
	usart_write(BOARD_USART1, tx_data);
	/* receive data*/
	while(!(usart_is_rx_ready(BOARD_USART2))){};
	usart_read(BOARD_USART2, &rx_data);
}

/**
 * \brief This test tests USART async mode test .
 *
 * include 8 bits transfer test, 9 bits transfer test,
 * parity error detect and hardware handshaking test.
 *
 * \param test Current test case.
 */
static void run_usart_async_test(const struct test_case *test)
{
	
	uint32_t status;

#if (SAMG55)
	/* Enable the peripheral and set USART mode. */
	flexcom_enable(BOARD_FLEXCOM1);
	flexcom_set_opmode(BOARD_FLEXCOM1, FLEXCOM_USART);
	flexcom_enable(BOARD_FLEXCOM2);
	flexcom_set_opmode(BOARD_FLEXCOM2, FLEXCOM_USART);

#else
	/* Enable the peripheral clock in the PMC. */
	sysclk_enable_peripheral_clock(BOARD_USART1);
	sysclk_enable_peripheral_clock(BOARD_USART2);
#endif

	/*usart async mode 8 bits transfer test */
	sam_usart_opt_t usart_asynchronous_settings = {
		BOARD_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Configure USART in async mode, 8 bits data, no parity, 1 stop bit */
	usart_init_rs232(BOARD_USART1, &usart_asynchronous_settings, sysclk_get_cpu_hz());
	usart_init_rs232(BOARD_USART2, &usart_asynchronous_settings, sysclk_get_cpu_hz());
	tx_data = 0x3A;
	/* Transfer data between 2 usarts */
	data_transfer();
	test_assert_true(test, tx_data == rx_data,
		"usart async 8 bits transfer test failed");

	/* Usart async mode 9 bits transfer test*/
	/* Configure USART in async mode, 9 bits data, no parity, 1 stop bit */
	usart_asynchronous_settings.char_length = US_MR_MODE9;
	usart_init_rs232(BOARD_USART1, &usart_asynchronous_settings, sysclk_get_cpu_hz());
	usart_init_rs232(BOARD_USART2, &usart_asynchronous_settings, sysclk_get_cpu_hz());
	tx_data = 0x1A3;
	/* Transfer data between 2 usarts */
	data_transfer();
	test_assert_true(test, tx_data == rx_data,
		"usart async 9 bits transfer test failed");

	/* Async mode parity error dected test*/
	/* Configure transmiter parity bit as 0 but receiver parity bit as 1 */
	usart_asynchronous_settings.parity_type = US_MR_PAR_SPACE;
	usart_init_rs232(BOARD_USART1, &usart_asynchronous_settings, sysclk_get_cpu_hz());
	usart_asynchronous_settings.parity_type = US_MR_PAR_MARK;
	usart_init_rs232(BOARD_USART2, &usart_asynchronous_settings, sysclk_get_cpu_hz());
	/* Transfer data between 2 usarts */
	data_transfer();
	status = usart_get_status(BOARD_USART2);
	test_assert_true(test, (status & US_CSR_PARE) ,
		"usart async parity error detected test failed");

	/* hardware handshaking test*/
	/* Enable transmiter and receiver hardware handshaking feature */
	usart_init_hw_handshaking(BOARD_USART1, &usart_asynchronous_settings, sysclk_get_cpu_hz());
	usart_init_hw_handshaking(BOARD_USART2, &usart_asynchronous_settings, sysclk_get_cpu_hz());
	/* enable USART1 transmit, USART2 receive doesn't enable, so USART2 will produce high RTS signal */
	usart_enable_tx(BOARD_USART1);
	/* USART1 should get high CTS siganl */
	status = usart_get_status(BOARD_USART1);
	test_assert_true(test, status & US_CSR_CTS,
		"usart async handshaking test failed");

}

/**
 * \brief This test tests USART sync mode test .
 *
 * include 8 bits transfer test, 9 bits transfer test and
 * parity error detect test.
 * 
 * \param test Current test case.
 */
static void run_usart_sync_test(const struct test_case *test)
{
	uint32_t status;

	/*usart sync mode 8 bits transfer test*/
	sam_usart_opt_t usart_synchronous_settings = {
		BOARD_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};
	/* Configure USART in sync mode, 8 bits data, no parity, 1 stop bit */
	usart_init_sync_master(BOARD_USART1, &usart_synchronous_settings, sysclk_get_cpu_hz());
	usart_init_sync_slave(BOARD_USART2, &usart_synchronous_settings);
	tx_data = 0x0A3;
	/* Transfer data between 2 usarts */
	data_transfer();
	test_assert_true(test, tx_data == rx_data,
		"usart sync 8 bits transfer test failed");

	/*usart async mode 9 bits transfer test*/
	/* Configure USART in sync mode, 9 bits data, no parity, 1 stop bit */
	usart_synchronous_settings.char_length = US_MR_MODE9;
	usart_init_sync_master(BOARD_USART1, &usart_synchronous_settings, sysclk_get_cpu_hz());
	usart_init_sync_slave(BOARD_USART2, &usart_synchronous_settings);
	tx_data = 0x13A;
	/* Transfer data between 2 usarts */
	data_transfer();
	test_assert_true(test, tx_data == rx_data,
		"usart sync 9 bits transfer test failed");

	/* Parity error dected test*/
	/* Configure transmiter parity bit as 0 but receiver parity bit as 1 */
	usart_synchronous_settings.parity_type = US_MR_PAR_SPACE;
	usart_init_sync_master(BOARD_USART1, &usart_synchronous_settings, sysclk_get_cpu_hz());
	usart_synchronous_settings.parity_type = US_MR_PAR_MARK;
	usart_init_sync_slave(BOARD_USART2, &usart_synchronous_settings);
	/* Transfer data between 2 usarts */
	data_transfer();
	status = usart_get_status(BOARD_USART2);
	test_assert_true(test, (status & US_CSR_PARE),
		"usart sync parity error detected test failed");
}

/**
 * \brief Test USART write protection.
 * \param test Current test case.
 */
static void run_usart_writeprotect_test(const struct test_case *test)
{
	uint32_t wp_status;
	uint32_t reg_backup;

	/* Enable write protect */
	usart_enable_writeprotect(BOARD_USART1);

	/* Access _MR to generate violation */
	reg_backup = BOARD_USART1->US_MR;
	BOARD_USART1->US_MR = 0xFF;
	
	wp_status = usart_get_writeprotect_status(BOARD_USART1);

	/* If status is 0, no write violation detected */
	test_assert_true(test, wp_status != 0,
		"WriteProtection on _MR not detected");

	/* Write violation source should be _MR */
	test_assert_true(test, wp_status == 0x4,
		"WriteProtection on _MR src error");

	/* Write violation should be blocked */
	test_assert_true(test, reg_backup == BOARD_USART1->US_MR,
		"_MR write not blocked");

	/* Disable write protect */
	usart_disable_writeprotect(BOARD_USART1);
}

/**
 * \brief Run USART driver unit test.
 */
int main(void)
{
	
	/* Configure UART for debug message output. */
	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS
	};

	sysclk_init();
	board_init();

	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	/* Define all the test cases */
	DEFINE_TEST_CASE(usart_async_test, NULL, run_usart_async_test, NULL,
		"USART async mode transfer test");
	DEFINE_TEST_CASE(usart_sync_test, NULL, run_usart_sync_test, NULL,
		"USART sync mode transfer test");
	DEFINE_TEST_CASE(usart_writeprotect_test, NULL, run_usart_writeprotect_test, NULL,
		"USART write protection");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(usart_tests) = {
		&usart_async_test,
		&usart_sync_test,
		&usart_writeprotect_test
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(usart_suite, usart_tests, "SAM USART driver test suite");

	/* Run all tests in the test suite */
	test_suite_run(&usart_suite);

	while (1) {
		/* Busy-wait forever */
	}
}
