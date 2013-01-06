/**
 * \file
 *
 * \brief Unit tests for ACIFC driver.
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
#include "acifc.h"
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
 * This is the unit test application for the ACIFC driver.
 * It consists of test cases for the following functionality:
 * - Configure ACIFC channel to generate interrupt
 *
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_usart_serial.h
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
 * \def CONF_TEST_ACIFC
 * \brief Test the functions provided by the ACIFC driver.
 */
//@}

/** Analog comparator channel number */
#define EXAMPLE_ACIFC_CHANNEL         0

volatile uint8_t intflag = 0;

/**
 * \brief Interrupt handler for ACIFC interrupt.
 */
static void set_int_flag(void)
{
	uint32_t ul_int_status;

	ul_int_status = acifc_get_interrupt_status(ACIFC);

	/* Compare Output Interrupt */
	if ((ul_int_status & ACIFC_ISR_ACINT0) == ACIFC_ISR_ACINT0) {
		acifc_clear_interrupt_status(ACIFC, ACIFC_ICR_ACINT0);
		intflag = 1;
	}
}

/**
 * \brief Test ACIFC
 *
 * \param test Current test case.
 */
static void run_acifc_test(const struct test_case *test)
{
	/* ACIFC Configuration */
	const acifc_cfg_t acifc_opt = {
		.actest = false,
		.eventen = true
	};
	/* ACIFC Channel Configuration */
	const acifc_channel_cfg_t acifc_channel_opt = {
		/* Hysteresis value */
		.hysteresis_value = ACIFC_HYS_0,
		/* Always on enable */
		.alwayson = true,
		/* Fast mode enable */
		.fast = true,
		/* Output event when ACOUT is zero? */
		.event_negative = false,
		/* Output event when ACOUT is one? */
		.event_positive = false,
		/* Set the negative input */
		.negative_input = NI_ACN,
		/* Set the comparator mode */
		.mode = MODE_USER_TRIGGERED,
		/* Interrupt settings */
		.interrupt_settings = IS_COMP_DONE,
	};

	acifc_enable(ACIFC);
	acifc_configure(ACIFC, &acifc_opt);
	acifc_channel_configure(ACIFC, &acifc_channel_opt, EXAMPLE_ACIFC_CHANNEL);
	acifc_set_callback(ACIFC, set_int_flag, ACIFC_IRQn, 1, ACIFC_IER_ACINT0);
	acifc_user_trigger_single_comparison(ACIFC);

	delay_ms(1000);

	test_assert_true(test, intflag == 1, "ACIFC test failed");
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
	DEFINE_TEST_CASE(acifc_test, NULL, run_acifc_test, NULL,
			"ACIFC test");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(acifc_tests) = {
		&acifc_test
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(acifc_suite, acifc_tests,
			"SAM ACIFC driver test suite");

	/* Run all tests in the test suite */
	test_suite_run(&acifc_suite);

	while (1) {
		/* Busy-wait forever. */
	}
}
