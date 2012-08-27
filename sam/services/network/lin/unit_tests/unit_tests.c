/**
 * \file
 *
 * \brief Unit tests for LIN service.
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 #include "asf.h"
#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <sysclk.h>
#include <lin.h>
#include <string.h>
#include <unit_test/suite.h>
#include <stdio_serial.h>
#include <conf_test.h>
#include <conf_board.h>

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the LIN service.
 * It consists of test cases for the following functionality:
 * - LIN transmit
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_usart_serial.h
 * - \ref conf_lin.h 
 *
 * \section device_info Device Info
 * SAM3X-EK devices can be used.
 * This example has been tested with the following setup:
 * - sam3x8h_sam3x_ek
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
 * \def CONF_TEST_LIN
 * \brief Init LIN and start a transmit.
 */
//@}

/** LIN frame number */
#define LIN_FRAME_ID_12     0x12

/** First byte for test */
#define LIN_FIRST_BYTE      0x55

/** Local Buffer for emission */
uint8_t lin_data_node[8];

/** LIN master node number */
#define LIN_MASTER_NODE_NUM 0

/** Timer counter channel used */
#define TC_CHANNEL          0

/** Timer counter frequency in Hz */
#define TC_FREQ             20

/** LIN interrupt received flag */
static uint32_t ul_LIN_int_flag=0;

/** Systick Counter */
static volatile uint32_t gs_ul_ms_ticks = 0U;

/**
 * \brief Interrupt handler for USART. Echo the bytes received and start the
 * next receive.
 */
void USART0_Handler(void)
{
	uint32_t status;
	status = usart_get_status(USART0);

	/* Check the LIN transmit complete flag */
	if (status & US_IMR_LINTC) {
		usart_disable_interrupt(USART0, US_IDR_LINTC);
		ul_LIN_int_flag = 1;
	}
}

/**
 * \brief SysTick handler.
 */
void SysTick_Handler(void)
{
	/* Increment counter necessary in delay(). */
	gs_ul_ms_ticks++;
}

/**
 * \brief Delay number of tick Systicks (happens every 1 ms).
 */
static void delay_ms(uint32_t ul_dly_ticks)
{
	uint32_t ul_cur_ticks;

	ul_cur_ticks = gs_ul_ms_ticks;
	while ((gs_ul_ms_ticks - ul_cur_ticks) < ul_dly_ticks) {
	}
}

/**
 * \brief lin_master_task_ID12.
 *
 */
static void  lin_master_task_ID12(void)
{
	lin_data_node[0] = LIN_FIRST_BYTE;
	lin_send_cmd(LIN_MASTER_NODE_NUM, LIN_FRAME_ID_12, sizeof(lin_data_node));
}

/**
 * \brief Interrupt handler for TC0. Record the number of bytes received,
 * and then restart a read transfer on the USART if the transfer was stopped.
 */
void TC0_Handler(void)
{
	/* Read TC0 Status. */
	tc_get_status(TC0, 0);

	lin_master_task_ID12();
}

/**
 * \brief Configure Timer Counter 0 (TC0) to generate an interrupt every 200ms.
 * This interrupt will be used to flush USART input and echo back.
 */
static void configure_tc(void)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	static uint32_t ul_sysclk;

	/* Get system clock. */
	ul_sysclk = sysclk_get_cpu_hz();

	/* Configure PMC. */
	pmc_enable_periph_clk(ID_TC0);

	/* Configure TC for a 50Hz frequency and trigger on RC compare. */
	tc_find_mck_divisor(TC_FREQ, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC0, 0, (ul_sysclk / ul_div) / TC_FREQ);

	/* Configure and enable interrupt on RC compare. */
	NVIC_EnableIRQ((IRQn_Type)ID_TC0);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
}

/**
 * \brief Test LIN transmit.
 *
 * This test initializes the LIN and start a transmit.
 *
 * \param test Current test case.
 */
static void run_lin_test(const struct test_case *test)
{
	st_lin_message lin_desc;

	/* Enable the peripheral clock in the PMC. */
	pmc_enable_periph_clk(ID_USART0);

	/* Enable LIN transceiver */
	gpio_set_pin_high(PIN_USART0_CTS_IDX);
	gpio_set_pin_high(PIN_USART0_RTS_IDX);

	/* Node 0:  LIN_MASTER_MODE */
	lin_init(USART0, true, LIN_MASTER_NODE_NUM, 9600, sysclk_get_cpu_hz());

	/* Configure lin_descriptor */
	/* Init LIN data Node 0 */
	/* Object 0 */
	lin_desc.uc_id = LIN_FRAME_ID_12;
	lin_desc.uc_dlc = sizeof(lin_data_node);
	lin_desc.lin_cmd = PUBLISH;
	lin_desc.uc_status = 0;
	lin_desc.uc_pt_data = lin_data_node;
	lin_register_descriptor(LIN_MASTER_NODE_NUM, 0, &lin_desc);

	configure_tc();
	/* In case of Master Mode, the timing transmission starts... */
	tc_start(TC0, 0);

	/* Configure and enable interrupt of USART */
	NVIC_EnableIRQ(USART0_IRQn);
	usart_enable_interrupt(USART0, US_IER_LINTC);

	delay_ms(200);
	test_assert_true(test, ul_LIN_int_flag, "LIN transmit failed!");
}

/**
 * \brief Run LIN service unit tests.
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

	/* Set up SysTick Timer for 1 msec interrupts. */
	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		/* Capture error. */
		while (1) {
		}
	}

#if defined(__GNUC__)
	setbuf(stdout, NULL);
#endif

	/* Define all the test cases */
	DEFINE_TEST_CASE(lin_test, NULL, run_lin_test, NULL,
		"LIN transmit test");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(lin_tests) = {
		&lin_test,
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(lin_suite, lin_tests, "LIN service test suite");

	/* Run all tests in the test suite */
	test_suite_run(&lin_suite);

	while (1) {
		/* Busy-wait forever. */
	}
}
