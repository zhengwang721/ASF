/**
 * \file
 *
 * \brief Unit tests for FREQM driver.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#include <board.h>
#include <sysclk.h>
#include "freqm.h"
#include "delay.h"
#include <string.h>
#include <unit_test/suite.h>
#include <stdio_serial.h>
#include <conf_test.h>
#include <conf_board.h>

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the FREQM driver.
 * It consists of test cases for the following functionality:
 * - Enable FREQM and start measurement, when measurement done will generate interrupt
 *
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_usart_serial.h
 * - \ref conf_sleepmgr.h
 *
 * \section device_info Device Info
 * The SAM4L devices can be used.
 * This example has been tested with the following setup:
 * - sam4lc4c_sam4l_ek
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://support.atmel.no/
 */

//! \name Unit test configuration
//@{
/**
 * \def CONF_TEST_FREQM
 * \brief Test the functions provided by the FREQM driver.
 */
//@}

volatile uint8_t intflag = 0;

/**
 * \brief Interrupt handler for FREQM interrupt.
 */
static void set_int_flag(void)
{
	/* Check if FREQM test line interrupt line is pending */
	if (FREQM_line_interrupt_is_pending(FREQM, GPIO_UNIT_TEST_FREQM_LINE)) {
		FREQM_line_disable_interrupt(FREQM, GPIO_UNIT_TEST_FREQM_LINE);
		intflag = 1;
	}
}

/**
 * \brief Test FREQM
 *
 * \param test Current test case.
 */
static void run_FREQM_test(const struct test_case *test)
{
	struct FREQM_line_config FREQM_line_conf;

	FREQM_line_conf.FREQM_mode = FREQM_MODE_EDGE_TRIGGERED;
	FREQM_line_conf.FREQM_edge = FREQM_EDGE_FALLING_EDGE;
	FREQM_line_conf.FREQM_level = FREQM_LEVEL_HIGH_LEVEL;
	FREQM_line_conf.FREQM_filter = FREQM_FILTER_DISABLED;
	FREQM_line_conf.FREQM_async = FREQM_ASYNCH_MODE;

	FREQM_enable(FREQM);
	FREQM_line_set_config(FREQM, GPIO_UNIT_TEST_FREQM_LINE, 
		&FREQM_line_conf);
	FREQM_line_set_callback(FREQM, GPIO_UNIT_TEST_FREQM_LINE, set_int_flag, 
		FREQM_1_IRQn, 1);
	FREQM_line_enable(FREQM, GPIO_UNIT_TEST_FREQM_LINE);

	ioport_set_pin_dir(GPIO_FREQM_TRIG_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(GPIO_FREQM_TRIG_PIN, IOPORT_PIN_LEVEL_HIGH);
	delay_ms(1000);
	ioport_set_pin_level(GPIO_FREQM_TRIG_PIN, IOPORT_PIN_LEVEL_LOW);

	delay_ms(1000);

	test_assert_true(test, intflag == 1, "FREQM test failed");


}

/**
 * \brief Run PDCA driver unit tests.
 */
int main(void)
{
	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS
	};

	/* Initialize the system clock and board */
	sysclk_init();
	board_init();

	/* Enable the debug uart */
	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

#if defined(__GNUC__)
	setbuf(stdout, NULL);
#endif

	/* Define all the test cases */
	DEFINE_TEST_CASE(FREQM_test, NULL, run_FREQM_test, NULL,
			"FREQM test");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(FREQM_tests) = {
		&FREQM_test
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(FREQM_suite, FREQM_tests,
			"SAM FREQM driver test suite");

	/* Run all tests in the test suite */
	test_suite_run(&FREQM_suite);

	while (1) {
		/* Busy-wait forever. */
	}
}
