/**
 * \file
 *
 * \brief SAM D20 EEPROM Emulator Service Unit test
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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
 * \mainpage EEPROM EMULATOR UNIT TEST
 *
 * \section intro Introduction
 * This unit test carries out tests for the EEPROM emulator service.
 * It consists of test cases for the following functionalities:
 *      - Test for EEPROM emulator initialization.
 *      - Test for EEPROM emulator buffer read/write functionality.
 *      - Test for EEPROM emulator page read/write functionality.
 *
 * The following kit is required for carrying out the test:
 *      - SAM D20 Xplained Pro board
 *
 * \section Setup
 *
 * The NVMCTRL_FUSES_EEPROM_SIZE has to be set to 0x00 in the fuse setting
 * of the device to run this test.
 *
 * Once the connection is made the following has to be done:
 *      - Connect the SAM D20 Xplained Pro board to the computer using
 *        a micro USB cable.
 *      - Open the virtual COM port in a terminal application.
 * \note  The USB composite firmware running on the Embedded Debugger (EDBG)
 *        will enumerate as debugger, virtual COM port and EDBG data
 *        gateway.
 *      - Build the project, program the target and run the application.
 *        The terminal shows the results of the unit test.
 *
 * \section Description
 *
 *      - The unit test first initializes the EEPROM emulator and formats the
 *        memory if not previously done.
 *      - The test writes a buffer of data to arbitrary offset and reads back
 *        and compares
 *      - The test then writes a page of data to a page and reads back and
 *        compares
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://support.atmel.no/
 */

#include <asf.h>
#include <stdio_serial.h>
#include <string.h>

#define TEST_EEPROM_PAGE    1
#define TEST_BUFFER_SIZE    19
#define TEST_BUFFER_OFFSET  123

/* Structure for UART module connected to EDBG (used for unit test output) */
struct usart_module cdc_uart_module;
/* Structure to store EEPROM emulator parameters */
struct eeprom_emulator_parameters ee_params;

/* Data buffers used in test */
uint8_t test_buffer[TEST_BUFFER_SIZE];
uint8_t verify_buffer[TEST_BUFFER_SIZE];
uint8_t test_page[EEPROM_PAGE_SIZE];
uint8_t verify_page[EEPROM_PAGE_SIZE];
uint16_t i;

/* Flag used to check successful initialization */
volatile bool init_success = true;

/**
 * \brief Initialize the USART for unit test
 *
 * Initializes the SERCOM USART (SERCOM4) used for sending the
 * unit test status to the computer via the EDBG CDC gateway.
 */
static void cdc_uart_init(void)
{
	struct usart_config cdc_uart_config;

	/* Configure USART for unit test output */
	usart_get_config_defaults(&cdc_uart_config);
	cdc_uart_config.mux_setting     = USART_RX_3_TX_2_XCK_3;
	cdc_uart_config.pinmux_pad3      = EDBG_UART_RX_PINMUX;
	cdc_uart_config.pinmux_pad2      = EDBG_UART_TX_PINMUX;
	cdc_uart_config.baudrate         = 115200;
	stdio_serial_init(&cdc_uart_module, EDBG_CDC_MODULE,
			&cdc_uart_config);
	/* Enable transceivers */
	usart_enable_transceiver(&cdc_uart_module, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&cdc_uart_module, USART_TRANSCEIVER_RX);
}

/**
 * \internal
 * \brief Test for EEPROM emulator initialization.
 *
 * This test initializes the EEPROM emulator. If no memory was set
 * for EEPROM in the fuses then all test will be skipped.
 * During initialization it will format the EEPROM memory if it was
 * not done previously.
 *
 * It also checks the get parameter functionality.
 *
 * \param test Current test case.
 */
static void run_eeprom_init_test(const struct test_case *test)
{
	enum status_code status = STATUS_ERR_IO;
	init_success = true;

	/* Initialize the EEPROM emulator */
	status = eeprom_emulator_init();
	if (status == STATUS_ERR_NO_MEMORY) {
		init_success = false;
	}

	test_assert_true(test, init_success,
			"No EEPROM memory section has been set in the device's fuses");

	/* Format memory if needed */
	if (status != STATUS_OK) {
		eeprom_emulator_erase_memory();
		status = eeprom_emulator_init();
		if (status != STATUS_OK) {
			init_success = false;
		}
	}

	test_assert_true(test, status == STATUS_OK,
			"EEPROM emulator initialization failed");

	/* Get parameter check */
	status = STATUS_ERR_IO;
	status = eeprom_emulator_get_parameters(&ee_params);
	if (status != STATUS_OK) {
		init_success = false;
	}

	test_assert_true(test, status == STATUS_OK,
			"EEPROM parameter retrieve failed");
	test_assert_true(test, ee_params.page_size == EEPROM_PAGE_SIZE,
			"Get parameter returned wrong page size"
			"Expected %d - Returned %d", EEPROM_PAGE_SIZE,
			ee_params.page_size);
}

/**
 * \internal
 * \brief Setup function for EEPROM emulator buffer read/write test.
 *
 * This function initializes the test buffer and verify buffer with
 * appropriate values.
 *
 * \param test Current test case.
 */
static void setup_eeprom_buffer_read_write_test(const struct test_case *test)
{
	/* Initialize buffers */
	for (i = 0; i < TEST_BUFFER_SIZE; i++) {
		test_buffer[i] = i + 1;
		verify_buffer[i] = 0;
	}
}

/**
 * \internal
 * \brief Test for EEPROM emulator buffer read/write function.
 *
 * This test writes a buffer of data of a given length to a given
 * offset. It then reads back the data from that offset and verifies.
 *
 * \param test Current test case.
 */
static void run_eeprom_buffer_read_write_test(const struct test_case *test)
{
	volatile enum status_code status = STATUS_ERR_IO;

	/* Skip test if initialization failed */
	test_assert_true(test, init_success,
			"Skipping test due to failed initialization");

	/* Write buffer test */
	status = eeprom_emulator_write_buffer(TEST_BUFFER_OFFSET,
			test_buffer, TEST_BUFFER_SIZE);
	test_assert_true(test, status == STATUS_OK,
			"EEPROM write buffer failed");

	/* Read buffer test */
	status = STATUS_ERR_IO;
	status = eeprom_emulator_read_buffer(TEST_BUFFER_OFFSET,
			verify_buffer, TEST_BUFFER_SIZE);
	test_assert_true(test, status == STATUS_OK,
			"EEPROM read buffer failed");

	/* Verify buffer test */
	for (i = 0; i < TEST_BUFFER_SIZE; i++) {
		test_assert_true(test, test_buffer[i] == verify_buffer[i],
				"Buffer data incorrect at index %d: "
				"Expected %d - Read %d", i, test_buffer[i],
				verify_buffer[i]);
	}
}

/**
 * \internal
 * \brief Setup function for EEPROM emulator page read/write test.
 *
 * This function initializes the test_page and verify_page data arrays
 * with appropriate values.
 *
 * \param test Current test case.
 */
static void setup_eeprom_page_read_write_test(const struct test_case *test)
{
	/* Initialize data arrays */
	for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
		test_page[i] = i + 1;
		verify_page[i] = 0;
	}
}

/**
 * \internal
 * \brief Test for EEPROM emulator page read/write function.
 *
 * This test writes a page of data to a given page number.
 * It then reads back the page and verifies.
 *
 * \param test Current test case.
 */
static void run_eeprom_page_read_write_test(const struct test_case *test)
{
	volatile enum status_code status = STATUS_ERR_IO;

	/* Skip test if initialization failed */
	test_assert_true(test, init_success,
			"Skipping test due to failed initialization");

	/* Write page test */
	status = eeprom_emulator_write_page(TEST_EEPROM_PAGE, test_page);
	test_assert_true(test, status == STATUS_OK,
			"EEPROM write page failed");

	/* Read page test */
	status = STATUS_ERR_IO;
	status = eeprom_emulator_read_page(TEST_EEPROM_PAGE, verify_page);
	test_assert_true(test, status == STATUS_OK,
			"EEPROM read page failed");

	/* Verify page test */
	for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
		test_assert_true(test, test_page[i] == verify_page[i],
				"Page data incorrect at index %d: "
				"Expected %d - Read %d", i, test_page[i],
				verify_page[i]);
	}
}

/**
 * \brief Run EEPROM emulator unit tests
 *
 * Initializes the system and serial output, then sets up the
 * EEPROM emulator unit test suite and runs it.
 */
int main(void)
{
	system_init();
	cdc_uart_init();

	/* Define Test Cases */
	DEFINE_TEST_CASE(eeprom_init_test, NULL,
			run_eeprom_init_test, NULL,
			"Testing EEPROM emulator initialization");

	DEFINE_TEST_CASE(eeprom_buffer_read_write_test,
			setup_eeprom_buffer_read_write_test,
			run_eeprom_buffer_read_write_test, NULL,
			"Testing EEPROM buffer read/write functionality");

	DEFINE_TEST_CASE(eeprom_page_read_write_test,
			setup_eeprom_page_read_write_test,
			run_eeprom_page_read_write_test, NULL,
			"Testing EEPROM page read/write functionality");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(eeprom_tests) = {
		&eeprom_init_test,
		&eeprom_buffer_read_write_test,
		&eeprom_page_read_write_test,
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(eeprom_test_suite, eeprom_tests,
			"SAM D20 EEPROM emulator service test suite");

	/* Run all tests in the suite*/
	test_suite_run(&eeprom_test_suite);

	while (true) {
		/* Intentionally left empty */
	}
}
