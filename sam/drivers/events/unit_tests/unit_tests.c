/**
 * \file
 *
 * \brief Unit tests for Events driver (PEVC).
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
 * This is the unit test application for Events driver (PEVC).
 * It consists of test cases for the following functionality:
 * - Using the AST as an event source and the PDCA as a triggered event.
 * - Using the software trigger as an event source and the PDCA as a triggered
 *   event.
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_uart_serial.h
 * - \ref conf_pdca.h
 * - \ref conf_ast.h
 * - \ref conf_sleepmgr.h
 *
 * \section device_info Device Info
 * SAM4L devices can be used.
 * This example has been tested with the following setup:
 * - sam4l4c_sam4l_ek
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://support.atmel.com/
 */

/** \name Unit test configuration */
/* @{ */
/**
 * \def CONF_TEST_USART
 * \brief USART to redirect STDIO to
 */

/**
 * \def CONF_TEST_BAUDRATE
 * \brief Baudrate of USART
 */

/**
 * \def CONF_TEST_CHARLENGTH
 * \brief Character length (bits) of USART
 */

/**
 * \def CONF_TEST_PARITY
 * \brief Parity mode of USART
 */

/**
 * \def CONF_TEST_STOPBITS
 * \brief Stopbit configuration of USART
 */
/* @} */

enum unit_test_step {
	TEST_STEP_AST_TRIGGER = 0,
	TEST_STEP_SOFTWARE_TRIGGER,
};

enum unit_test_step test_step;
uint32_t test_flag = 0;

uint8_t event_string[] = "..";

/**
 * PDCA transfer interrupt callback.
 */
static void pdca_tranfer_done(enum pdca_channel_status status)
{
	/* Get PDCA channel status and check if PDCA transfer is completed */
	if (status == PDCA_CH_TRANSFER_COMPLETED) {
		/* Reload PDCA data transfer */
		pdca_channel_write_reload(PEVC_ID_USER_PDCA_0, (void *)event_string,
				sizeof( event_string));
		/* Set test_flag */
		if (test_step == TEST_STEP_AST_TRIGGER) {
			test_flag |= (0x01u << TEST_STEP_AST_TRIGGER);
			ast_disable(AST);
		} else if (test_step == TEST_STEP_SOFTWARE_TRIGGER) {
			test_flag |= (0x01u << TEST_STEP_SOFTWARE_TRIGGER);
		}
	}
}

/**
 * Initialize the AST as event generator.
 */
static void init_ast(void)
{
	struct ast_config ast_conf;

	/* Enable osc32 oscillator */
	if (!osc_is_ready(OSC_ID_OSC32)) {
		osc_enable(OSC_ID_OSC32);
		osc_wait_ready(OSC_ID_OSC32);
	}

	/* Enable the AST */
	ast_enable(AST);

	/* Configure the AST with counter mode and set counter to 0 */
	ast_conf.mode = AST_COUNTER_MODE;
	ast_conf.osc_type = AST_OSC_32KHZ;
	ast_conf.psel = AST_PSEL_32KHZ_1HZ;
	ast_conf.counter = 0;

	ast_set_config(AST, &ast_conf);

	/* Enable period enent of AST */
	ast_write_periodic0_value(AST, AST_PSEL_32KHZ_1HZ);
	ast_enable_event(AST, AST_EVENT_PER);
}

/**
 * Initialize the PDCA transfer for the example.
 */
static void init_pdca(void)
{
	/* PDCA channel options */
	static const pdca_channel_config_t pdca_tx_configs = {
		.addr = (void *)event_string,
		.pid = CONF_TEST_PDCA_PID,
		.size = sizeof(event_string),
		.r_addr = 0,
		.r_size = 0,
		.ring = false,
		.etrig = true,
		.transfer_size = PDCA_MR_SIZE_BYTE
	};

	/* Enable PDCA module */
	pdca_enable(PDCA);

	/* Init PDCA channel with the pdca_options.*/
	pdca_channel_set_config(CONF_TEST_USER_ID, &pdca_tx_configs);

	/* Set callback for PDCA channel */
	pdca_channel_set_callback(CONF_TEST_USER_ID, pdca_tranfer_done,
			PDCA_0_IRQn, 1, PDCA_IER_TRC);

	/* Enable PDCA channel */
	pdca_channel_enable(CONF_TEST_USER_ID);
}

/**
 * \brief Test events driver with AST trigger.
 *
 * \param test Current test case.
 */
static void run_events_ast_test(const struct test_case *test)
{
	uint32_t i;
	struct events_chan_conf config;

	test_step = TEST_STEP_AST_TRIGGER;

	init_ast();
	init_pdca();

	/*
	 * Configure an event channel
	 * - AST periodic event 0 --- Generator
	 * - PDCA channel 0       --- User
	 */
	events_chan_get_config_defaults(&config);
	config.channel_id = CONF_TEST_USER_ID;
	config.generator_id = CONF_TEST_GEN_ID;
	config.sharper_enable = true;
	config.igf_edge = EVENT_IGF_EDGE_NONE;
	events_chan_configure(&config);

	/* Enable the channel */
	events_chan_enable(CONF_TEST_USER_ID);

	/* Wait for AST event trigger */
	for (i = 0; i < (sizeof(event_string) * 2); i++) {
		if (test_flag & (0x01u << TEST_STEP_AST_TRIGGER)) {
			break;
		}
		delay_ms(1000);
	}

	/* Disable the AST */
	ast_disable(AST);

	test_assert_true(test, (test_flag & (0x01u << TEST_STEP_AST_TRIGGER)),
		"AST event not triggered!");
}

/**
 * \brief Test events driver with Software trigger.
 *
 * \param test Current test case.
 */
static void run_events_software_test(const struct test_case *test)
{
	uint32_t i;

	test_step = TEST_STEP_SOFTWARE_TRIGGER;

	/* Enable software trigger */
	events_chan_enable_software_trigger(CONF_TEST_USER_ID);

	/* Software event trigger */
	for (i = 0; i < sizeof(event_string) * 2; i++) {
		events_chan_software_trigger(CONF_TEST_USER_ID);
		delay_ms(100);
		if (test_flag & (0x01u << TEST_STEP_SOFTWARE_TRIGGER)) {
			break;
		}
	}

	test_assert_true(test, (test_flag & (0x01u << TEST_STEP_SOFTWARE_TRIGGER)),
		"Software event not triggered!");
}

/**
 * \brief Run Event driver unit tests.
 */
int main(void)
{
	const usart_serial_options_t usart_serial_options = {
		.baudrate = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits = CONF_TEST_STOPBITS
	};

	sysclk_init();
	board_init();
	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	/* Define all the test cases */
	DEFINE_TEST_CASE(events_ast_test, NULL, run_events_ast_test, NULL,
			"Test Event driver with AST trigger.");
	DEFINE_TEST_CASE(events_software_test, NULL, run_events_software_test, NULL,
			"Test Event driver with software trigger.");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(events_tests) = {
		&events_ast_test,
		&events_software_test
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(events_suite, events_tests, "Events driver test suite");

	/* Enable clock for PEVC module */
	sysclk_enable_peripheral_clock(PEVC);

	/* Run all tests in the test suite */
	test_suite_run(&events_suite);

	while (1) {
		/* Busy-wait forever. */
	}
}
