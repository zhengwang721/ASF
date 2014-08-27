/**
 * \file
 *
 * \brief Clock system example 3.
 *
 * Copyright (c) 2011 - 2014 Atmel Corporation. All rights reserved.
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
 * This example shows how to switch between various system clock sources
 * and prescalers at run time.
 *
  * \section files Main files:
 * - clock_example3_sam.c: clock system example application
 * - conf_board.h: board initialization configuration
 * - conf_clock.h: system clock configuration
 * - conf_example3.h: chip- or board-specific example configuration
 *
 * \section deviceinfo Device Info
 * All SAM devices can be used.
 *
 * \section exampledescription Description of the example
 * The clock system is reconfigured every time the user presses a button
 * to use the next clock source and prescaler settings
 * shown in the state diagram below.
 *
 * The system clock is output on pin PCK0.
 * \note For SAM4C_EK, PCK2 is used.
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for SAM.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 * Support and FAQ: http://www.atmel.com/design-support/
 *
 */

#include "asf.h"
#include "genclk.h"
#include "conf_board.h"
#include "conf_example3.h"

/* Global ul_ms_ticks in milliseconds since start of application */
volatile uint32_t ul_ms_ticks = 0;

/** Programmable Clock ID for the example by default */
#ifndef GCLK_ID
#define GCLK_ID         GENCLK_PCK_0
#endif

/**
 * \brief Wait for the given number of milliseconds (using the ul_ms_ticks
 * generated by the SAM microcontroller system tick).
 *
 * \param ul_dly_ticks  Delay to wait for, in milliseconds.
 */
static void mdelay(uint32_t ul_dly_ticks)
{
	uint32_t ul_cur_ticks;

	ul_cur_ticks = ul_ms_ticks;
	while ((ul_ms_ticks - ul_cur_ticks) < ul_dly_ticks) {
	}
}

/**
 *  Wait for user push the button.
 */
static void wait_for_switches(void)
{
	do {
	} while (ioport_get_pin_level(GPIO_PUSH_BUTTON_1));
	mdelay(1);
	do {
	} while (!ioport_get_pin_level(GPIO_PUSH_BUTTON_1));
	mdelay(1);
}

/**
 * \brief Handler for System Tick interrupt.
 *
 * Process System Tick Event
 * Increments the ul_ms_ticks counter.
 */
void SysTick_Handler(void)
{
	ul_ms_ticks++;
}

/**
 * \brief Switch between various system clock sources and prescalers at
 * run time.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	struct genclk_config gcfg;

	sysclk_init();
	board_init();

	/* Setup SysTick Timer for 1 msec interrupts */
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}

	/* Enable PIO module related clock */
	sysclk_enable_peripheral_clock(PIN_PUSHBUTTON_1_ID);

	/* Configure specific CLKOUT pin */
	ioport_set_pin_mode(GCLK_PIN, GCLK_PIN_MUX);
	ioport_disable_pin(GCLK_PIN);

	/* Configure the output clock source and frequency */
	genclk_config_defaults(&gcfg, GCLK_ID);
	genclk_config_set_source(&gcfg, GENCLK_PCK_SRC_PLLACK);
	genclk_config_set_divider(&gcfg, GENCLK_PCK_PRES_1);
	genclk_enable(&gcfg, GCLK_ID);

	while (1) {
		/*
		 * Initial state.
		 */
		wait_for_switches();

		/*
		 * Divide MCK frequency by 2.
		 */
		sysclk_set_prescalers(SYSCLK_PRES_2);
		genclk_config_set_divider(&gcfg, GENCLK_PCK_PRES_2);
		genclk_enable(&gcfg, GCLK_ID);
		wait_for_switches();

#ifdef BOARD_NO_32K_XTAL
		/*
		 * Switch to the slow clock with all prescalers disabled.
		 */
		sysclk_set_source(SYSCLK_SRC_SLCK_RC);
		sysclk_set_prescalers(SYSCLK_PRES_1);
		genclk_config_set_source(&gcfg, GENCLK_PCK_SRC_SLCK_RC);
		genclk_config_set_divider(&gcfg, GENCLK_PCK_PRES_1);
		genclk_enable(&gcfg, GCLK_ID);
		osc_disable(OSC_MAINCK_XTAL);
		wait_for_switches();
#endif

		/*
		 * Switch to internal 8 MHz RC.
		 */
		/* Switch to slow clock before switch main clock */
		sysclk_set_source(SYSCLK_SRC_SLCK_RC);
		osc_enable(OSC_MAINCK_8M_RC);
		osc_wait_ready(OSC_MAINCK_8M_RC);
		sysclk_set_source(SYSCLK_SRC_MAINCK_8M_RC);
		genclk_config_set_source(&gcfg, GENCLK_PCK_SRC_MAINCK_8M_RC);
		genclk_enable(&gcfg, GCLK_ID);
		wait_for_switches();

#if BOARD_FREQ_MAINCK_XTAL
		/*
		 * Switch to external crystal (8MHz or 12MHz, depend on the board).
		 */
		osc_enable(OSC_MAINCK_XTAL);
		osc_wait_ready(OSC_MAINCK_XTAL);
		sysclk_set_source(SYSCLK_SRC_MAINCK_XTAL);
		genclk_config_set_source(&gcfg, GENCLK_PCK_SRC_MAINCK_XTAL);
		genclk_enable(&gcfg, GCLK_ID);
		osc_disable(OSC_MAINCK_8M_RC);
		wait_for_switches();
#endif

		/*
		 * Go back to the initial state and start over.
		 */
		sysclk_init();
		genclk_config_set_source(&gcfg, GENCLK_PCK_SRC_PLLACK);
		genclk_config_set_divider(&gcfg, GENCLK_PCK_PRES_1);
		genclk_enable(&gcfg, GCLK_ID);
	}
}

