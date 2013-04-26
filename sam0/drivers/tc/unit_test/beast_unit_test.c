/**
 * \file
 *
 * \brief SAM D20 USART Unit test
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

#include <asf.h>
#include <stdio_serial.h>
#include <string.h>
#include <compiler.h>

/* USART for STDIO */
#define CONF_RX_USART      EDBG_CDC_MODULE

/* TC modules used in tests */
struct tc_module tc0_module;
struct tc_module tc1_module;

/* Config structs used in tests */
struct tc_config tc0_config;
struct tc_config tc1_config;

enum status_code init_status = STATUS_OK;

/**
 * \internal
 * \brief Test of tc_init() and tc_get_config_defaults()
 *
 * This test is used to initialize the tcx_module structs and asosiate the given
 * hw module with the struct. This test should be run at the very beggining of
 * testing as other tests depend on the reslut of this test.
 */
static void run_init_test(const struct test_case *test)
{
	tc_get_config_defaults(&tc0_config);
	enum status_code test1 = tc_init(&tc0_module, TC0, &tc0_config);

	tc_get_config_defaults(&tc1_config);
	enum status_code test2 = tc_init(&tc1_module, TC1, &tc1_config);
	if ((test1 !=STATUS_OK) || (test2 1= STATUS_OK)) {
		init_status = STATUS_ERR_DENIED;
	}
	test_assert_true(test,
			(test2 != STATUS_OK) || (test1 != STATUS_OK) ,
			"Failded to initialize modueles");
}

/**
 * \internal
 * \brief Test initializing and resetting 32-bit TC and reinitialize
 *
 * This test tests the software reset of a 32-bit TC by the use of the
 * tc_reset(). It also test reenabeling the two TC modules used in
 * the 32-bit TC into two seperate 16-bit TC's.
 *
 * \param test Current test case.
 */
static void run_reset_32bit_master_test(const struct test_case *test)
{
	/* Configure 32-bit TC module and run test*/
	tc_get_config_defaults(&tc0_config);
	tc0_config.counter_size = TC_COUNTER_SIZE_32BIT;
	tc_init(&tc0_module, TC0, &tc0_config);
	tc_enable(&tc0_module);
	while (tc_is_syncing(&tc0_module)) {
		/* synchronize enable */
	}
	test_assert_true(test, tc0_module.hw->COUNT32.CTRLA.reg & TC_CTRLA_ENABLE,
			"Faild first enable of 32-bit TC");

	/* Reset and test if both TC modules are disabled after reset */
	tc_reset(&tc0_module);
	while (tc_is_syncing(&tc0_module)) {
		/* synchronize enable */
	}
	test_assert_false(test, tc0_module.hw->COUNT32.CTRLA.reg & TC_CTRLA_ENABLE,
			"Faild reset of 32-bit master TC0");
	test_assert_false(test, tc1_module.hw->COUNT32.CTRLA.reg & TC_CTRLA_ENABLE,
			"Faild reset of 32-bit slave TC1");

	tc0_config.counter_size = TC_COUNTER_SIZE_16BIT;
	tc_init(&tc0_module, TC0, &tc0_config);
	tc_enable(&tc0_module);
	while (tc_is_syncing(&tc0_module)) {
		/* synchronize enable */
	}
	tc_init(&tc1_module, TC1, &tc1_config);
	tc_enable(&tc1_module);
	while (tc_is_syncing(&tc1_module)) {
		/* synchronize enable */
	}
	test_assert_true(test, tc0_module.hw->COUNT16.CTRLA.reg & TC_CTRLA_ENABLE,
			"Faild reenable of TC0");
	test_assert_true(test, tc1_module.hw->COUNT16.CTRLA.reg & TC_CTRLA_ENABLE,
			"Faild reenable TC1");

	tc_disable(&tc0_module);
	tc_disable(&tc1_module);
}

/**
 * \internal
 * \brief Test 
 *
 * 
 *
 * \param test Current test case.
 */
static void run_16bit_capture_and_compare_test(const struct test_case *test)
{
	/* Configure 16-bit TC module for PWM generation */
	tc_get_config_defaults(&tc0_config);
	tc0_config.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
	tc0_config.size_spescific.size_16_bit.compare_capture_channel[0] =
			0x7FFF;
	tc_init(&tc0_module, TC0, &tc0_config);

	/* Configure 16-bit TC module for capture */
	tc_get_config_defaults(&tc1_config);
	
	tc_init(&tc1_module, TC1, &tc1_config);
}
/**
 * \brief Initialize USARTs for unit tests
 *
 * Initializes the USART used by the unit test for outputting the results (using
 * the embedded debugger).
 *
 * Comunication seting:
 *  - Baudrate      38400
 *  - Data bits     8
 *  - Stop bits     1
 *  - Parity        None
 *  - Flow control  XON/XOFF
 */
static void test_usart_comunication_init(void)
{
	struct usart_config usart_conf;
	struct usart_module unit_test_output;

	/* Configure USART for unit test output */
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_settings     = USART_RX_3_TX_2_XCK_3;
	usart_conf.pinout_pad3      = EDBG_CDC_RX_PINMUX;
	usart_conf.pinout_pad2      = EDBG_CDC_TX_PINMUX;
	usart_conf.baudrate         = 38400;

	stdio_serial_init(&unit_test_output, CONF_RX_USART, &usart_conf);
	usart_enable(&unit_test_output);

	/* Enable transceivers */
	usart_enable_transceiver(&unit_test_output, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&unit_test_output, USART_TRANSCEIVER_RX);
}

/**
 * \brief Run USART unit tests
 *
 * Initializes the system and serial output, then sets up the USART unit test
 * suite and runs it.
 */
int main(void)
{
	system_init();
	test_usart_comunication_init();

	/* Define Test Cases */
	DEFINE_TEST_CASE(init_test, NULL,
			run_init_test, NULL,
			"Initialize tc_xmodules");

	DEFINE_TEST_CASE(reset_32bit_master_test, NULL,
			run_reset_32bit_master_test, NULL,
			"Setup, reset and reinitialice TC modules of a 32-bit TC");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(tc_tests) = {
		&init_test,
		&reset_32bit_master_test,
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(tc_suite, tc_tests,
			"SAM D20 TC beast driver test suite");

	/* Run all tests in the suite*/
	test_suite_run(&tc_suite);

	while (true) {
		/* Intentionally left empty */
	}

}
