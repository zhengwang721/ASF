/**
 * \file
 *
 * \brief AVR XMEGA TC45 Driver Example 2
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

/**
 * \mainpage
 *
 * \section intro Introduction
 * This simple example shows how to use the \ref tc45_group to generate
 * single slope PWM.
 *
 * \section files Main files:
 *  - tc45.c Timer XMEGA Timer Counter driver implementation
 *  - tc45.h Timer XMEGA Timer Counter driver definitions
 *  - tc45_example2.c example application
 *  - conf_example.h: configuration of the example
 *
 * \section driverinfo TC45 Driver
 * The XMEGA TC45 driver can be found \ref tc45_group "here".
 *
 * \section deviceinfo Device Info
 * All AVR XMEGA devices with an tc45 can be used.
 *
 * \section exampledescription Description of the example
 * The example uses and configures two TC modules.
 *  - \ref TIMER_PORT_LED is configured to generate single slope PWM on LED0
 *  - \ref TIMER_EXAMPLE is used in generate a 50ms overflow interrupt
 *
 * Each time the 50ms overflow interrupt of \ref TIMER_EXAMPLE is executed
 * the \ref TIMER_PORT_LED single slope PWM duty cycle is changed so that it
 * result visible on LEDO light intensity.
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

/**
 * \brief Timer Counter Overflow interrupt callback function
 *
 * This function is called when an overflow interrupt has occurred (50ms) on
 * TIMER_PORT_LED and change PWM duty single generation on TIMER_PORT_LED CCA.
 */
static void example_overflow_interrupt_callback(void)
{
	tc45_write_cc(&TIMER_PORT_LED, TC45_CCA,
			tc45_read_cc(&TIMER_PORT_LED, TC45_CCA) + 0x2ff);
	tc45_clear_overflow(&TIMER_EXAMPLE);
}

int main(void)
{
	pmic_init();
	board_init();
	sysclk_init();
	sleepmgr_init();
	cpu_irq_enable();
        
        /* configures PortD Pin4 in output mode */
        ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 4), IOPORT_DIR_OUTPUT);
        
	/*
	 * Configure TIMER_EXAMPLE to generate 50ms overflow interrupt
	 * using 500kHz (2us) resolution clock (50ms=25000*2us)
	 */
	/* Unmask clock for TIMER_EXAMPLE */
	tc45_enable(&TIMER_EXAMPLE);
	/* Enable overflow interrupt */
	tc45_set_overflow_interrupt_level(&TIMER_EXAMPLE, TC45_INT_LVL_LO);
	/* Configure TC in normal mode */
	tc45_set_wgm(&TIMER_EXAMPLE, TC45_WG_NORMAL);
	/* Configure call back interrupt */
	tc45_set_overflow_interrupt_callback(&TIMER_EXAMPLE,
			example_overflow_interrupt_callback);
	/* Configure TC period and and resolution */
	tc45_write_period(&TIMER_EXAMPLE, 25000);
	tc45_set_resolution(&TIMER_EXAMPLE, 500000);

	/*
	 * Configure TIMER_PORT_LED to generate 2ms Single Slope PWM
	 */
	/* Unmask clock for TIMER_PORT_LED */
	tc45_enable(&TIMER_PORT_LED);
	/* Configure TC in PWM Single Slope PWM */
	tc45_set_wgm(&TIMER_PORT_LED, TC45_WG_SS);
	/* Initialize and enable Channel A */
	tc45_write_cc(&TIMER_PORT_LED, TC45_CCA, 0);
	tc45_enable_cc_channels(&TIMER_PORT_LED, TC45_CCACOMP);
	/* Run TC at 2MHz clock resolution */
	tc45_set_resolution(&TIMER_PORT_LED, 20000000);

	do {
		/* Go to sleep, everything is handled by interrupts. */
		sleepmgr_enter_sleep();
	} while (1);
}
