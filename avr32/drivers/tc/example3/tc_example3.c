/**
 * \file
 *
 * \brief Timer/Counter example 3.
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
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
 * \section intro Introduction
 * This is the documentation for the data structures, functions, variables,
 * defines, enums, and typedefs for the TC driver.\n It also gives an example
 * of the usage of the TC module.
 *
 * This example will start a timer/counter and generates "tick" interrupts.
 *
 * The selected timer input clock is the internal clock labelled TC3
 * referred to as TIMER_CLOCK3 in the datasheet. TIMER_CLOCK3 is connected to
 * fPBA / 8 (see datasheet).
 *
 * The 16-bit timer/counter channel will cycle from 0x0000 to RC. RC is
 * initialized to (fPBA / 8) / 100, so that an interrupt will be triggered
 * every 1 ms. Upon interrupt, a LED0 is toggled thus producing a square
 * signal of frequency 500 Hz. Check the toggled GPIO pin connected to LED0 on
 *  an oscilloscope and see the square signal at frequency 500 Hz.
 *
 * Also, a message is displayed on USART and the 'Timer:' value is updated
 * every second.
 * The displayed message is:\n
 * <code> ATMEL \n AVR32 UC3 - TC example \n Timer: 1 s </code>
 *
 * \section files Main Files
 * - tc.c: TC driver
 * - tc.h: TC driver header file
 * - tc_example3.c: TC example 3
 * - conf_example.h: TC Configuration
 * - conf_clock.h: Clock configuration for example
 * - conf_board.h: Simulator configuration
 *
 * \section compinfo Compilation Info
 * This software was written for GCC for AVR32 and IAR Embedded Workbench
 * for AVR32. Other compilers may or may not work.
 *
 * \section Configuration Information
 * This example has been tested with the following configuration:
 * - EVK1100, EVK1101, EVK1104, EVK1105, UC3L_EK, UC3C_EK, UC3-A3 XPLAINED,
 *   UC3-L0 XPLAINED, STK600 + RCUC3D routing board boards.
 * - CPU clock: 16MHz in UC3C_EK and 12 MHz in all other boards.
 * - PC terminal settings:
 *   - 57600 bps,
 *   - 8 data bits,
 *   - no parity bit,
 *   - 1 stop bit,
 *   - no flow control.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/uc3">Atmel AVR UC3</A>.\n
*/
#include <stdint.h>
#include <stdbool.h>

#include "sysclk.h"
#include "board.h"
#include "print_funcs.h"
#include "tc.h"
#include "gpio.h"
#if defined (__GNUC__)
#  include "intc.h"
#endif
#include "common_hw_timer.h"


//! \name Example configuration
//!@{
/**
 * \def CONFIG_SYSCLK_SOURCE
 * \brief Clock source to use
 */
/**
 * \def EXAMPLE_TC
 * \brief Base address of TC module to use.
 */
/**
 * \def EXAMPLE_TC_CHANNEL
 * \brief Channel to use with TC module.
 */
/**
 * \def EXAMPLE_TC_IRQ
 * \brief IRQ Line number for TC module
 */
 /**
 * \def EXAMPLE_TC_IRQ_PRIORITY
 * \brief IRQ Group number for TC module
 */
//!@}

#define delay	(10000)

static void hw_overflow_cb(void)
{
	LED_Toggle(LED0);
}

static void hw_expiry_cb(void)
{
	gpio_toggle_pin(AVR32_PIN_PB00);
	
	common_tc_delay(delay);
}

/*! \brief Main function:
 *  - Configure the CPU to run at 12MHz
 *  - Configure the USART
 *  - Register the TC interrupt (GCC only)
 *  - Configure, enable the CPCS (RC compare match) interrupt, and start a
 *    TC channel in waveform mode
 *  - In an infinite loop, update the USART message every second.
 */
int main(void)
{
	/**
	 * \note the call to sysclk_init() will disable all non-vital
	 * peripheral clocks, except for the peripheral clocks explicitly
	 * enabled in conf_clock.h.
	 */
	sysclk_init();
	// Enable the clock to the selected example Timer/counter peripheral module.
//	sysclk_enable_peripheral_clock(EXAMPLE_TC);
	// Initialize the USART module for trace messages
//	init_dbg_rs232(sysclk_get_pba_hz());
	// Disable the interrupts
	cpu_irq_disable();

#if defined (__GNUC__)
	// Initialize interrupt vectors.
	INTC_init_interrupts();
	// Register the RTC interrupt handler to the interrupt controller.
	INTC_register_interrupt(&tc_irq, EXAMPLE_TC_IRQ, EXAMPLE_TC_IRQ_PRIORITY);
#endif
	// Enable the interrupts
	cpu_irq_enable();
	// Initialize the timer module
//	tc_init(tc);

	gpio_configure_pin(AVR32_PIN_PB00,GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);

	set_common_tc_overflow_callback(hw_overflow_cb);
	set_common_tc_expiry_callback(hw_expiry_cb);
	common_tc_init();
	common_tc_delay(delay);
	while (1) {
		}
}

