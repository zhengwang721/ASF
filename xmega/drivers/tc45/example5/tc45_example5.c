/**
 * \file
 *
 * \brief AVR XMEGA TC45 Driver Example 5
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
 * This simple example shows how to use the \ref tc45_group for generation of
 * pulsed width modulation signals and to switch them on different outputs
 * with the use of Output Matrix function of WeX.
 *
 * \section files Main files:
 * - tc45.c timer/counter driver implementation
 * - tc45.h timer/counter driver definitions
 * - tc45_example5.c: tc driver example application
 *
 * \section apiinfo tc driver API
 * The tc4 driver API can be found \ref tc45_group "here".
 *
 * \section deviceinfo Device Info
 * AVR XMEGA-E devices with tc45 and WeX modules can be used.
 *
 * \section example_description Description of the example
 * The example will configure the Timer C4 (TCC4) and Timer 5(TCC5) in waveform
 * generator mode.
 * Wex is used to output PWM signals from Compare modules CCA and CCB.
 * This example is dimming the duty-cycle of the two signals in an
 * opposite way.
 * In addition the example configures the CCA and CCB interrupts to update the
 * duty-cycle
 * In default configuration these complementary signals are output on:
 * TCC4 CCA/CCB : Ports C0/C1.
 * TCC5 CCA/CCB : Ports C4/C5
 * PortD pin0 is used to switch the Output Matrix modes
 * If PortD pinà is connected to a switch, each time the switch is pressed
 * the OTMX mode will be incremented and results will be as descroibed below:
 * In OTMX 0 mode, the Outputs will be :
 *	  PC0/PC1 : TC4 CCA/CCB
 *        PC4/PC5 : TC5 CCA/CCB
 * In OTMX 1 mode, the Outputs will be :
 *        PC0/PC1 : TC4 CCA/CCB
 *        PC4/PC5 : TC5 CCA/CCB
 *        PC6/PC7 : TC5 CCA/CCB
 * In OTMX2 mode, the Outputs will be :
 *        PC0/PC1 : TC4 CCA/CCB
 *        PC2/PC3 : TC5 CCA/CCB
 *        PC4/PC5 : TC4 CCA/CCB
 *        PC6/PC7 : TC5 CCA/CCB
 * In OTMX3  mode, the Outputs will be :
 *        PC0 to PC7 : TC4 CCA
 * In OTMX4  mode, the Outputs will be :
 *        PC0 to PC6 : TC4 CCA
 *        PC7 : TC4 CCB
 *
 * Summary of Timer Counter configuration:
 *  - TCC4 and TCC5 : Timers with Timer TCC5 2 times faster than TCC4
 *  - Compare/Capture modules A/B : Compare and waveform generation in Dual
 *  slope mode
 *  - WeXC : OTMX modes
 *
 * This example uses the on-board STK600 LEDs to provide user feedback,
 * the output from the LEDs are as follows:
 * Default configuration:
 *  - LED0: fades out
 *  - LED1: fades in
 *  - LED4: fades out two times faster than LED0
 *  - LED5: fades in two times faster than LED1
 * Then versus the OTMX configurations, the LEDS will fade according to the
 * PortC outputs routing.
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

uint16_t cca_tcc4_pwm_index = (TIMER_EXAMPLE1_PERIOD / 2);
uint16_t ccb_tcc4_pwm_index = (TIMER_EXAMPLE1_PERIOD / 2);
uint16_t cca_tcc5_pwm_index = (TIMER_EXAMPLE2_PERIOD / 2);
uint16_t ccb_tcc5_pwm_index = (TIMER_EXAMPLE2_PERIOD / 2);
uint8_t otmx_mode_index = 0;
PORT_t *port;

/**
 * \brief Compare/Capture detection interrupt callback function
 *
 * This function is called when an interrupt has occurred on a Compare A
 *channel.
 * It increments the CC detection level and thus modify the duty-cycle.
 */
static void example_cca_tcc4_interrupt_callback(void)
{
	cca_tcc4_pwm_index += 0x10;
	if (cca_tcc4_pwm_index >= (TIMER_EXAMPLE1_PERIOD - 17)) {
		cca_tcc4_pwm_index = 17;
	}

	tc45_write_cc_buffer(&TIMER_EXAMPLE1, TC45_CCA, cca_tcc4_pwm_index);
}

static void example_cca_tcc5_interrupt_callback(void)
{
	cca_tcc5_pwm_index += 0x10;
	if (cca_tcc5_pwm_index >= (TIMER_EXAMPLE2_PERIOD - 17)) {
		cca_tcc5_pwm_index = 17;
	}

	tc45_write_cc_buffer(&TIMER_EXAMPLE2, TC45_CCA, cca_tcc5_pwm_index);
}

/**
 * \brief Compare/Capture detection interrupt callback function
 *
 * This function is called when an interrupt has occurred on a Compare B
 *channel.
 * It decrements the CC detection level and thus modify the duty-cycle.
 */
static void example_ccb_tcc4_interrupt_callback(void)
{
	ccb_tcc4_pwm_index -= 0x10;
	if (ccb_tcc4_pwm_index <= 17) {
		ccb_tcc4_pwm_index = (TIMER_EXAMPLE1_PERIOD - 17);
	}

	tc45_write_cc_buffer(&TIMER_EXAMPLE1, TC45_CCB, ccb_tcc4_pwm_index);
}

static void example_ccb_tcc5_interrupt_callback(void)
{
	ccb_tcc5_pwm_index -= 0x10;
	if (ccb_tcc5_pwm_index <= 17) {
		ccb_tcc5_pwm_index = (TIMER_EXAMPLE2_PERIOD - 17);
	}

	tc45_write_cc_buffer(&TIMER_EXAMPLE2, TC45_CCB, ccb_tcc5_pwm_index);
}

/*! \brief Interrupt on PORTD pin0 "pin change" from switch to switch OTMX mode
 *
 * Note: otmx_mode_index is the index for Output Matrix mode
 */
ISR(PORTD_INT_vect)
{
	PORT_t *port;
	port = ioport_pin_to_port(GPIO_PUSH_BUTTON_0);
	delay_ms(100);
	otmx_mode_index++;
	delay_ms(100);
	port->INTFLAGS = 0x01; /* Ack interrupt */
	if (otmx_mode_index == 5) {
		otmx_mode_index = 0;
	}
}

int main(void)
{
	pmic_init();
	board_init();
	sysclk_init();
	sleepmgr_init();
	cpu_irq_enable();

	/* Configure PORTD.0 pin change interrupt for OTMX modes change */
	ioport_configure_pin(GPIO_PUSH_BUTTON_0,
			IOPORT_DIR_INPUT | IOPORT_PULL_UP);

	port = ioport_pin_to_port(GPIO_PUSH_BUTTON_0);
	port->INTMASK = 0x01;
	port->INTCTRL = PORT_INTLVL_LO_gc;
	/* Enable trigger on falling edge */
	port->PIN0CTRL = PORT_ISC_FALLING_gc;

	/* Enables the Timers defined in conf_example.h : TCC4 (1) and TCC5 (2)
	 *in this example */
	tc45_enable(&TIMER_EXAMPLE1);
	tc45_enable(&TIMER_EXAMPLE2);

	/* Configures the interrupt level of CCA and CCB modules of the 2
	 *Timers: low */
	tc45_set_cca_interrupt_level(&TIMER_EXAMPLE1, TC45_INT_LVL_LO);
	tc45_set_ccb_interrupt_level(&TIMER_EXAMPLE1, TC45_INT_LVL_LO);
	tc45_set_cca_interrupt_level(&TIMER_EXAMPLE2, TC45_INT_LVL_LO);
	tc45_set_ccb_interrupt_level(&TIMER_EXAMPLE2, TC45_INT_LVL_LO);

	/* Declares the interrupt functions which will be called when CCA and
	 * CCB
	 * interrupts will occur */
	tc45_set_cca_interrupt_callback(&TIMER_EXAMPLE1,
			example_cca_tcc4_interrupt_callback);
	tc45_set_ccb_interrupt_callback(&TIMER_EXAMPLE1,
			example_ccb_tcc4_interrupt_callback);
	tc45_set_cca_interrupt_callback(&TIMER_EXAMPLE2,
			example_cca_tcc5_interrupt_callback);
	tc45_set_ccb_interrupt_callback(&TIMER_EXAMPLE2,
			example_ccb_tcc5_interrupt_callback);

	/* Configures the Timer periods*/
	tc45_write_period(&TIMER_EXAMPLE1, TIMER_EXAMPLE1_PERIOD);
	tc45_write_period(&TIMER_EXAMPLE2, TIMER_EXAMPLE2_PERIOD);

	/* Configures the CCA and CCB levels*/
	tc45_write_cc(&TIMER_EXAMPLE1, TC45_CCA, TIMER_EXAMPLE1_PERIOD / 2);
	tc45_write_cc(&TIMER_EXAMPLE1, TC45_CCB, TIMER_EXAMPLE1_PERIOD / 2);
	tc45_write_cc(&TIMER_EXAMPLE2, TC45_CCA, TIMER_EXAMPLE2_PERIOD / 4);
	tc45_write_cc(&TIMER_EXAMPLE2, TC45_CCB, TIMER_EXAMPLE2_PERIOD / 4);

	/* Enables the CCA and CCB channels*/
	tc45_enable_cc_channels(&TIMER_EXAMPLE1, TC45_CCACOMP);
	tc45_enable_cc_channels(&TIMER_EXAMPLE1, TC45_CCBCOMP);
	tc45_enable_cc_channels(&TIMER_EXAMPLE2, TC45_CCACOMP);
	tc45_enable_cc_channels(&TIMER_EXAMPLE2, TC45_CCBCOMP);

	/* Configures the waveform genertaor in Dual Slope mode and Top*/
	tc45_set_wgm(&TIMER_EXAMPLE1, TC45_WG_DS_T);
	tc45_set_wgm(&TIMER_EXAMPLE2, TC45_WG_DS_T);

	tc45_set_resolution(&TIMER_EXAMPLE1, 10000000);
	tc45_set_resolution(&TIMER_EXAMPLE2, 10000000);

	/* Enables the control of Port output from the Timer */
	tc45_fault_set_portcontrol(&FAULTC4);
	tc45_fault_set_portcontrol(&FAULTC5);

	do {
		/* Go to sleep, everything is handled by interrupts. */
		/*	sleepmgr_enter_sleep(); */

		/* Configures the Output Matrix mode
		 * In OTMX 0 mode, the Outputs will be :
		 * PC0/PC1 : TC4 CCA/CCB
		 * PC4/PC5 : TC5 CCA/CCB
		 * In OTMX 1 mode, the Outputs will be :
		 * PC0/PC1 : TC4 CCA/CCB
		 * PC4/PC5 : TC5 CCA/CCB
		 * PC6/PC7 : TC5 CCA/CCB
		 *  In OTMX2 mode, the Outputs will be :
		 * PC0/PC1 : TC4 CCA/CCB
		 * PC2/PC3 : TC5 CCA/CCB
		 * PC4/PC5 : TC4 CCA/CCB
		 * PC6/PC7 : TC5 CCA/CCB
		 *  In OTMX3  mode, the Outputs will be :
		 * PC0 to PC7 : TC4 CCA
		 *  In OTMX4  mode, the Outputs will be :
		 * PC0 to PC6 : TC4 CCA
		 * PC7 : TC4 CCB
		 */
		switch (otmx_mode_index) {
		case 0:
			tc45_WEX_set_otmx(&WEXC, WEX_OTMX_DEFAULT);
			break;

		case 1:
			tc45_WEX_set_otmx(&WEXC, WEX_OTMX_1);
			break;

		case 2:
			tc45_WEX_set_otmx(&WEXC, WEX_OTMX_2);
			break;

		case 3:
			tc45_WEX_set_otmx(&WEXC, WEX_OTMX_3);
			break;

		case 4:
			tc45_WEX_set_otmx(&WEXC, WEX_OTMX_4);
			break;

		default:
			break;
		}
	} while (1);
}
