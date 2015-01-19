/**
 * \file
 *
 * \brief SAM BOD Callback Quick Start
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <stdio_serial.h>
#include <string.h>
#include "conf_test.h"

struct usart_module cdc_uart_module;

static void cdc_uart_init(void)
{
	struct usart_config usart_conf;

	/* Configure USART for unit test output */
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART, &usart_conf);
	usart_enable(&cdc_uart_module);
}


/* bod init test*/
static void run_bod_init_test(const struct test_case *test)
{
	enum status_code status;
	struct bod_config config_bod;

	bod_get_config_defaults(&config_bod);
	config_bod.action = BOD_ACTION_INTERRUPT;
	config_bod.level = 0x3f;	
	status = bod_set_config(BOD_BOD33, &config_bod);
	test_assert_true(test, status == STATUS_OK,
			"BOD init failed");
	status = bod_enable(BOD_BOD33);
	test_assert_true(test, status == STATUS_OK,
			"BOD enable failed");
			
}

/* bod detect test*/
static void run_bod_test(const struct test_case *test)
{
	bool bod_status;
	enum status_code status;

	/* change the cpu voltage to 2.5V*/
	bod_status = bod_is_detected(BOD_BOD33);
	test_assert_true(test, bod_status == true,
			"bod detect failed");

	bod_clear_detected(BOD_BOD33);
	status = bod_disable(BOD_BOD33);
	test_assert_true(test, status == STATUS_OK,
			"BOD disable failed");
}

int main(void)
{

	system_init();
	cdc_uart_init();

	DEFINE_TEST_CASE(bod_init_test,NULL,
			run_bod_init_test,NULL,
			"bod init test");
	DEFINE_TEST_CASE(bod_test,NULL,
			run_bod_test,NULL,
			"bod detect test");


	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(bod_tests) = {
		&bod_init_test,
		&bod_test,
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(bod_test_suite, bod_tests,
			"SAM0 BOD driver test suite");

	/* Run all tests in the suite*/
	test_suite_run(&bod_test_suite);

	while (true) {
		/* Intentionally left empty */
	}
}

