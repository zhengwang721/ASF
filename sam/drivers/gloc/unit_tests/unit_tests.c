/**
 * \file
 *
 * \brief Unit tests for GLOC driver.
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
#include <conf_test.h>

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the GLOC driver.
 * It contains one test case for the GLOC module:
 * - Test 4 inputs XOR.
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_uart_serial.h
 * - \ref conf_gloc.h
 *
 * \section device_info Device Info
 * Only SAM devices with GLOC can be used.
 * This example has been tested with the following setup:
 * - sam4lc4c_sam4l_ek_gloc
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://support.atmel.no/
 */

struct gloc_lut_config config[GLOC_LUTS];
struct gloc_lut_config *p_config[GLOC_LUTS];
struct gloc_dev_inst dev_inst;

/**
 * \brief Test GLOC Logic module with 4 Inputs XOR.
 *
 * This tests check the capabilty of the Glue Logic driver to handle a 4
 * inputs XOR operation.
 *
 * \param test Current test case.
 */
static void run_gloc_4inputs_xor_test(const struct test_case *test)
{
	bool out;

	ioport_set_pin_dir(in0, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(in1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(in2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(in3, IOPORT_DIR_OUTPUT);

	dev_inst.gloc_lut_cfg[0]->truth_table_value = 0x6996;
	gloc_lut_set_config(&dev_inst);

	ioport_set_pin_level(in0, LOW);
	ioport_set_pin_level(in1, LOW);
	ioport_set_pin_level(in2, LOW);
	ioport_set_pin_level(in3, LOW);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == false, "4 inputs XOR failure");

	ioport_set_pin_level(in0, HIGH);
	ioport_set_pin_level(in1, LOW);
	ioport_set_pin_level(in2, LOW);
	ioport_set_pin_level(in3, LOW);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == true, "4 inputs XOR failure");

	ioport_set_pin_level(in0, LOW);
	ioport_set_pin_level(in1, HIGH);
	ioport_set_pin_level(in2, LOW);
	ioport_set_pin_level(in3, LOW);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == true, "4 inputs XOR failure");

	ioport_set_pin_level(in0, HIGH);
	ioport_set_pin_level(in1, HIGH);
	ioport_set_pin_level(in2, LOW);
	ioport_set_pin_level(in3, LOW);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == false, "4 inputs XOR failure");

	ioport_set_pin_level(in0, LOW);
	ioport_set_pin_level(in1, LOW);
	ioport_set_pin_level(in2, HIGH);
	ioport_set_pin_level(in3, LOW);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == true, "4 inputs XOR failure");

	ioport_set_pin_level(in0, HIGH);
	ioport_set_pin_level(in1, LOW);
	ioport_set_pin_level(in2, HIGH);
	ioport_set_pin_level(in3, LOW);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == false, "4 inputs XOR failure");

	ioport_set_pin_level(in0, LOW);
	ioport_set_pin_level(in1, HIGH);
	ioport_set_pin_level(in2, HIGH);
	ioport_set_pin_level(in3, LOW);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == false, "4 inputs XOR failure");

	ioport_set_pin_level(in0, HIGH);
	ioport_set_pin_level(in1, HIGH);
	ioport_set_pin_level(in2, HIGH);
	ioport_set_pin_level(in3, LOW);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == true, "4 inputs XOR failure");

	ioport_set_pin_level(in0, LOW);
	ioport_set_pin_level(in1, LOW);
	ioport_set_pin_level(in2, LOW);
	ioport_set_pin_level(in3, HIGH);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == true, "4 inputs XOR failure");

	ioport_set_pin_level(in0, HIGH);
	ioport_set_pin_level(in1, LOW);
	ioport_set_pin_level(in2, LOW);
	ioport_set_pin_level(in3, HIGH);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == false, "4 inputs XOR failure");

	ioport_set_pin_level(in0, LOW);
	ioport_set_pin_level(in1, HIGH);
	ioport_set_pin_level(in2, LOW);
	ioport_set_pin_level(in3, HIGH);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == false, "4 inputs XOR failure");

	ioport_set_pin_level(in0, HIGH);
	ioport_set_pin_level(in1, HIGH);
	ioport_set_pin_level(in2, LOW);
	ioport_set_pin_level(in3, HIGH);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == true, "4 inputs XOR failure");

	ioport_set_pin_level(in0, LOW);
	ioport_set_pin_level(in1, LOW);
	ioport_set_pin_level(in2, HIGH);
	ioport_set_pin_level(in3, HIGH);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == false, "4 inputs XOR failure");

	ioport_set_pin_level(in0, HIGH);
	ioport_set_pin_level(in1, LOW);
	ioport_set_pin_level(in2, HIGH);
	ioport_set_pin_level(in3, HIGH);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == true, "4 inputs XOR failure");

	ioport_set_pin_level(in0, LOW);
	ioport_set_pin_level(in1, HIGH);
	ioport_set_pin_level(in2, HIGH);
	ioport_set_pin_level(in3, HIGH);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == true, "4 inputs XOR failure");

	ioport_set_pin_level(in0, HIGH);
	ioport_set_pin_level(in1, HIGH);
	ioport_set_pin_level(in2, HIGH);
	ioport_set_pin_level(in3, HIGH);
	delay_us(50);
	out = ioport_get_pin_level(out0);
	test_assert_true(test, out == false, "4 inputs XOR failure");

}

/**
 * \brief Run GLOC driver unit tests.
 */
int main(void)
{
	sysclk_init();
	board_init();

	const usart_serial_options_t usart_serial_options = {
		.baudrate = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits = CONF_TEST_STOPBITS
	};

	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	/* Set the default configuration */
	gloc_lut_get_config_defaults(config);
	gloc_init(&dev_inst, GLOC, config);
	/* Enable the GLOC module. */
	gloc_enable(&dev_inst);

	/* Define all the test cases. */
	DEFINE_TEST_CASE(gloc_4inputs_xor_test, NULL, run_gloc_4inputs_xor_test,
			NULL, "SAM GLOC LUT 4 Inputs XOR Test");

	/* Put test case addresses in an array. */
	DEFINE_TEST_ARRAY(gloc_tests) = {
		&gloc_4inputs_xor_test,
	};

	/* Define the test suite. */
	DEFINE_TEST_SUITE(gloc_suite, gloc_tests, "SAM GLOC driver test suite");

	/* Run all tests in the test suite. */
	test_suite_run(&gloc_suite);

	while (1) {
		/* Busy-wait forever. */
	}
}
