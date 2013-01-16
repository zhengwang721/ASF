/**
 * \file
 *
 * \brief External Interrupt example for megaRF device
 *
 * Copyright (c) 2012-2013 Atmel Corporation. All rights reserved.
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

/**
 * \mainpage
 *
 * \section intro Introduction
 * This simple example shows how to use the \ref megarf_interrupt_group for
 * external interrupt
 *
 * \section files Main files:
 * - ext_int_example1.c: This example application
 * - conf_interrupt.h: configuration of the megarf driver
 *
 * \section driverinfo Interrupt Driver
 * The interrupt driver can be found \ref megarf_interrupt_group "here".
 *
 * \section deviceinfo Device Info
 * AVR MEGARF device can be used.
 *
 * \section setup Setup
 * user defined External interrupt INTx pin and PCINTx is enabled
 * SW1 and SW2 are the switches connected
 * to the interrupt pins
 *
 * \section exampledescription Description of the example
 * when switch SW1 is pressed, due to pull-up the
 * interrupt INTx will be triggered when SW1 is released and LED1 blinks once.
 * When switch SW2 is pressed the interrupt PCINTx is triggered and LED2 blinks
 * once and when SW2 is released the interrupt PCINTx is triggered once again 
 * and LED2 blinks once again.So a single switch action (press and release)
 * on SW2 produces two blinks on LED2.
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 */

#include <asf.h>
#include <conf_interrupt.h>

#ifndef CONFIG_INTERRUPT_SOURCE
# define CONFIG_INTERRUPT_SOURCE    EXT_INT0
#endif /* CONFIG_INTERRUPT_SOURCE */

#ifndef CONFIG_INTERRUPT_MODE
# define CONFIG_INTERRUPT_MODE      IOPORT_SENSE_FALLING
#endif /* CONFIG_INTERRUPT_SOURCE */

/**
 *  \brief delay routine for LED to be ON for some time
 *  \param none
 */

static inline void ext_int_delay(void)
{
	for (volatile unsigned int i = 0; i < 10000; i++) {
	}
}

/**
 * \brief Interrupt callback for External interrupt
 *  Switch ON LED1(PORTE2) for some time and then switches OFF
 *  when the interrupt is triggered
 *  \param none
 */
static void ext_int_callback(void)
{
	/* ! Switch ON LED1 */
	LED_On(LED_GREEN_GPIO);

	ext_int_delay();

	/* Switch OFF LED1 */
	LED_Off(LED_GREEN_GPIO);
}

/**
 * \brief Interrupt PC callback for External interrupt
 *  Switch ON LED2(PORTE3) for some time and then switches OFF
 *  when the interrupt is triggered
 *  \param none
 */
static void ext_pcint_callback(void)
{
	/* ! Switch ON LED2 * / */
	LED_On(LED_YELLOW_GPIO);

	/* ! wait for some delay */
	ext_int_delay();

	/* ! Turn off LED2 */
	LED_Off(LED_YELLOW_GPIO);
	
}

/** \brief Main function.
 */
int main(void)
{
	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	sysclk_init();
	board_init();

	/* Disable Global interrupt */
	cpu_irq_disable();

    /* Set Interrupt CallBack Function **/
    ext_int_set_interrupt_callback(CONFIG_INTERRUPT_SOURCE,ext_int_callback);
	
	/* Enable the Ext Int */
	ext_int_init(CONFIG_INTERRUPT_SOURCE, CONFIG_INTERRUPT_MODE);

	/* Set Interrupt CallBack Function **/
	ext_int_set_interrupt_callback(CONFIG_PC_INT_SOURCE,ext_pcint_callback);
	
	/* Enable the Ext PC INT */
	ext_int_pcint_init(CONFIG_PC_INT_SOURCE);

	/* Enable Global interrupt */
	cpu_irq_enable();

	/* Infinite loop and waiting for the external interrupt to occur */
	while (1) {
	}
}
