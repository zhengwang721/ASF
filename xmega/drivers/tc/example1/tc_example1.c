/**
 * \file
 *
 * \brief AVR XMEGA TC Driver Example 1
 *
 * Copyright (c) 2010 - 2011 Atmel Corporation. All rights reserved.
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
 * \mainpage
 *
 * \section intro Introduction
 * This simple example shows how to use the \ref tc_group to toggle LEDs.
 *
 * \section files Main files:
 *  - tc.c Timer XMEGA Timer Counter driver implementation
 *  - tc.h Timer XMEGA Timer Counter driver definitions
 *  - tc_example1.c example application
 *  - conf_example.h: configuration of the example
 *
 * \section driverinfo TC Driver
 * The XMEGA TC driver can be found \ref tc_group "here".
 *
 * \section deviceinfo Device Info
 * All AVR XMEGA devices with an tc can be used.
 *
 * \section exampledescription Description of the example
 * The example will configure one TC module (\ref TIMER_EXAMPLE) in normal mode and
 * use three TC interrupts (overflow, Capture/Compare Channels A and B).
 *
 * The TC is setup to use a 31250Hz resolution clock and a 31250 period value
 * resulting in a 1Hz TC overflow frequency.
 * Compare Channels A and B will be setup at 1/4 and 1/2 of the TC period.

 * Each callback interrupts functions are setup to toggle a LED.
 *  - LED0: Toggles on TC overflow interrupt
 *  - LED1: Toggle on Compare Channel A
 * In the case of A1 Xplain:
 *  - LED2: Toggle on Compare Channel B interrupts
 * In the case of A3BU Xplained:
 *  - Status LED: Toggles red on Compare Channel B interrupts
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 */
#include <conf_example.h>
#include <string.h>
#include <asf.h>
#include "common_sw_timer.h"

uint32_t time_out1 = 20000;
uint32_t time_out2 = 10000;
uint32_t time_out3 = 40000;
uint32_t time_out4 = 50000;

static void timeout1_cb(void *parameter);
static void timeout2_cb(void *parameter);
static void timeout3_cb(void *parameter);
static void timeout4_cb(void *parameter);

int main(void)
{
	pmic_init();
	sysclk_init();
	board_init();
	sleepmgr_init();

	sw_timer_init();

	cpu_irq_enable();

	ioport_configure_pin(J2_PIN0, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(J2_PIN1, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(J2_PIN2, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(J2_PIN3, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);

	while(STATUS_OK != sw_timer_start(0, time_out1, TIMEOUT_RELATIVE, (FUNC_PTR)timeout1_cb, NULL));
	while(STATUS_OK != sw_timer_start(1, time_out2, TIMEOUT_RELATIVE, (FUNC_PTR)timeout2_cb, NULL));
//	while(STATUS_OK != sw_timer_start(2, time_out3++, TIMEOUT_RELATIVE, (FUNC_PTR)timeout3_cb, NULL));
//	while(STATUS_OK != sw_timer_start(3, time_out4++, TIMEOUT_RELATIVE, (FUNC_PTR)timeout4_cb, NULL));

	do {
		sw_timer_service();
	} while (1);
}

static void timeout1_cb(void *parameter)
{
	ioport_toggle_pin(J2_PIN0);

	while(STATUS_OK != sw_timer_start(0, time_out1, TIMEOUT_RELATIVE, (FUNC_PTR)timeout1_cb, NULL));
}

static void timeout2_cb(void *parameter)
{
	ioport_toggle_pin(J2_PIN1);
	sw_timer_stop(0);
	//while(STATUS_OK != sw_timer_start(1, time_out2, TIMEOUT_RELATIVE, (FUNC_PTR)timeout2_cb, NULL));
}

static void timeout3_cb(void *parameter)
{
	ioport_toggle_pin(J2_PIN2);

	while(STATUS_OK != sw_timer_start(2, time_out3, TIMEOUT_RELATIVE, (FUNC_PTR)timeout3_cb, NULL));
}

static void timeout4_cb(void *parameter)
{
	ioport_toggle_pin(J2_PIN3);

	while(STATUS_OK != sw_timer_start(3, time_out4, TIMEOUT_RELATIVE, (FUNC_PTR)timeout4_cb, NULL));
}
