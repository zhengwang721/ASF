/**
 * \file
 *
 * \brief TC Capture Waveform Example for SAM.
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
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
 * \mainpage TC Capture Waveform Example
 *
 * \section Purpose
 *
 * This example indicates how to use TC in capture mode to measure the pulse frequency
 * and count the total pulse number of an external signal injected on TIOA pin.
 *
 * \section Requirements
 *
 * This package can be used with SAM evaluation kits.
 * It generates a waveform on TC0 channel 1 TIOA1, and it captures wave
 * from channel 2 TIOA2. These 2 pins could be found on board extension
 * header. To measure the waveform on TIOA1, connect PIN_TC0_TIOA1 to PIN_TC0_TIOA2,
 * and configure PIN_TC0_TIOA1 as output pin and PIN_TC0_TIOA2 as input pin.
 *
 * \section Descriptions
 *
 * This example shows how to configure TC in waveform and capture mode.
 * In capture mode, pulse signal (from TIOA1) is set as an input to TIOA2, and RA and RB will be
 * loaded when programmed event occurs. When TC interrupt happens, we could read RA and RB
 * value for calculating pulse frequency and increased pulse number. The current pulse frequency
 * and total pulse number are output on UART.
 *
 * The code can be roughly broken down as follows:
 * <ul>
 * <li>Select pre-defined waveform frequency and duty cycle to be generated.
 * <li>Configure TC channel 1 as waveform output.
 * <li>Configure TC channel 2 as capture input.
 * <li>Configure capture Register A loaded when rising edge of TIOA occurs.
 * <li>Configure capture Register B loaded when falling edge of TIOA occurs.
 * <li>Configure an interrupt for TC and enable the RB load interrupt.
 * <li> 'c' starts capture.
 * <li> 's' stops capture, and dumps the information that has been captured.
 * </ul>
 *
 * \section Usage
 *
 * -# Compile the application.
 * -# Connect the UART port of the evaluation board to the computer and open
 * it in a terminal.
 *    - Settings: 115200 bauds, 8 bits, 1 stop bit, no parity, no flow control.
 * -# Download the program into the evaluation board and run it. Please refer to
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on the solutions that users choose.
 * -# Upon startup, the application will output the following line on the UART:
 *    \code
 *     -- TC capture waveform example  xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 * -# Choose the item in the following menu to test.
 *    \code
 * Configure TC0 channel 1 as waveform operating mode
 * Start waveform: Frequency = 375 Hz,Duty Cycle = 50%
 * Configure TC0 channel 2 as capture operating mode
 *
 *     Menu :
 *     ------
 *       Output waveform property:
 *       0: Set Frequency =  375 Hz, Duty Cycle = 50%
 *       1: Set Frequency =  800 Hz, Duty Cycle = 75%
 *       2: Set Frequency = 1000 Hz, Duty Cycle = 80%
 *       3: Set Frequency = 4000 Hz, Duty Cycle = 55%
 *       -------------------------------------------
 *       c: Capture waveform from TC 0 channel 2
 *       s: Stop capture and display informations what have been captured
 *       h: Display menu
 *     ------
 *    \endcode
 *
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#define TC_CHANNEL_1 1 /* TC Channel number 1 */
#define TC_CHANNEL_2 2 /* TC Channel number 2 */

#define STRING_EOL    "\r"
#define STRING_HEADER "--TC capture waveform Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

struct waveconfig_t {
    /** Internal clock signals selection. */
	uint32_t ul_intclock;
    /** Waveform frequency (in Hz). */
	uint16_t us_frequency;
    /** Duty cycle in percent (positive).*/
	uint16_t us_dutycycle;
};

/** TC waveform configurations */
static const struct waveconfig_t gc_waveconfig[] = {
#if !SAM4S
	{TC_CMR_TCCLKS_TIMER_CLOCK4, 178, 30},
#endif	
	{TC_CMR_TCCLKS_TIMER_CLOCK3, 375, 50},
	{TC_CMR_TCCLKS_TIMER_CLOCK3, 800, 75},
	{TC_CMR_TCCLKS_TIMER_CLOCK2, 1000, 80},
	{TC_CMR_TCCLKS_TIMER_CLOCK1, 4000, 55}
};

/** Current wave configuration*/
static uint8_t gs_uc_configuration = 0;

/** Number of available wave configurations */
const uint8_t gc_uc_nbconfig = sizeof(gc_waveconfig)
		/ sizeof(struct waveconfig_t);

/** Capture status*/
static uint32_t gs_ul_captured_pulses;
static uint32_t gs_ul_captured_ra;
static uint32_t gs_ul_captured_rb;

/**
 * \brief Display the user menu on the UART.
 */
static void display_menu(void)
{
	uint8_t i;
	puts("\n\rMenu :\n\r"
			"------\n\r"
			"  Output waveform property:\r");
	for (i = 0; i < gc_uc_nbconfig; i++) {
		printf("  %d: Set Frequency = %4u Hz, Duty Cycle = %2u%%\n\r",
				i,
				(unsigned int)gc_waveconfig[i].
				us_frequency,
				(unsigned int)gc_waveconfig[i].
				us_dutycycle);
	}
	puts("  -------------------------------------------\n\r"
			"  c: Capture waveform from TC 0 channel 2\n\r"
			"  s: Stop capture and display informations what have been captured \n\r"
			"  h: Display menu \n\r"
			"------\n\r\r");
}

/**
 * \brief Configure TC0 channel 1 in waveform operating mode.
 */
static void tc_waveform_initialize(void)
{
	const uint32_t divisors[5] = { 2, 8, 32, 128, BOARD_MCK / 32768 };
	uint32_t ra, rc;

	/* Configure the PMC to enable the Timer Counter clock for TC0 channel 1. */
	pmc_enable_periph_clk(ID_TC1);
	
	/* Init TC clock. */
	tc_init(TC0, TC_CHANNEL_1, gc_waveconfig[gs_uc_configuration].ul_intclock /* Waveform Clock Selection */
			| TC_CMR_WAVE       /* Waveform mode is enabled */
			| TC_CMR_ACPA_SET	/* RA Compare Effect: set */
			| TC_CMR_ACPC_CLEAR /* RC Compare Effect: clear */
			| TC_CMR_CPCTRG     /* UP mode with automatic trigger on RC Compare */
	);
	
	/* Configure waveform frequency and duty cycle. */
    rc = (sysclk_get_cpu_hz() / divisors[gc_waveconfig[gs_uc_configuration].ul_intclock]) / 
		gc_waveconfig[gs_uc_configuration].us_frequency;
    tc_write_rc(TC0, TC_CHANNEL_1, rc);
    ra = (100 - gc_waveconfig[gs_uc_configuration].us_dutycycle) * rc / 100;
    tc_write_ra(TC0, TC_CHANNEL_1, ra);

	/* Enable TC0 channel 1. */
	tc_start(TC0, TC_CHANNEL_1);
	printf("Start waveform: Frequency = %d Hz,Duty Cycle = %2d%%\n\r",
			gc_waveconfig[gs_uc_configuration].us_frequency,
			gc_waveconfig[gs_uc_configuration].us_dutycycle);
}

/**
 * \brief Configure TC0 channel 2 in capture operating mode.
 */
static void tc_capture_initialize(void)
{
	/* Configure the PMC to enable the Timer Counter clock TC0 channel 2. */
	pmc_enable_periph_clk(ID_TC2);
	
	/* Init TC clock. */
	tc_init(TC0, TC_CHANNEL_2, TC_CMR_TCCLKS_TIMER_CLOCK2 /* Clock Selection */
			| TC_CMR_LDRA_RISING     /* RA Loading Selection: rising edge of TIOA */
			| TC_CMR_LDRB_FALLING    /* RB Loading Selection: falling edge of TIOA */
			| TC_CMR_ABETRG          /* External Trigger Selection: TIOA */
			| TC_CMR_ETRGEDG_FALLING /* External Trigger Edge Selection: Falling edge */
	);
}

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Interrupt handler for the TC0 channel 2.
 */
void TC2_Handler(void)
{
	if ((tc_get_status(TC0, TC_CHANNEL_2) & TC_SR_LDRBS) == TC_SR_LDRBS) {
		gs_ul_captured_pulses++;
		gs_ul_captured_ra = tc_read_ra(TC0, TC_CHANNEL_2);
		gs_ul_captured_rb = tc_read_rb(TC0, TC_CHANNEL_2);
	}
}

/**
 * \brief Application entry point for tc_capture_waveform example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t key;
	uint16_t frequence, dutycycle;

	/* Initialize the SAM system */
	sysclk_init();
	board_init();	

	/* Initialize the console uart */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Configure PIO Pins for TC0 */
	gpio_configure_pin(PIN_TC0_TIOA1, PIN_TC0_TIOA1_FLAGS);
	gpio_configure_pin(PIN_TC0_TIOA2, PIN_TC0_TIOA2_FLAGS);

	/* Configure TC0 channel 1 as waveform operating mode */
	puts("Configure TC0 channel 1 as waveform operating mode \r");
	tc_waveform_initialize();
	/* Configure TC0 channel 2 as capture operating mode */
	puts("Configure TC0 channel 2 as capture operating mode \r");
	tc_capture_initialize();

	/* Configure TC interrupts for TC0 channel 2 only */
	NVIC_DisableIRQ(TC2_IRQn);
	NVIC_ClearPendingIRQ(TC2_IRQn);
	NVIC_SetPriority(TC2_IRQn, 0);
	NVIC_EnableIRQ(TC2_IRQn);

	/* Display menu */
	display_menu();

	while (1) {
		while (uart_read(CONSOLE_UART, &key));

		switch (key) {
		case 'h':
			display_menu();
			break;

		case 's':
			if (gs_ul_captured_pulses) {
				tc_disable_interrupt(TC0, TC_CHANNEL_2, TC_IDR_LDRBS);
				printf("Captured %u pulses from TC0 channel 2, RA = %u, RB = %u \n\r", (unsigned int)gs_ul_captured_pulses, (unsigned int)gs_ul_captured_ra, (unsigned int)gs_ul_captured_rb);

				frequence = (sysclk_get_cpu_hz() / 8) / gs_ul_captured_rb;
				dutycycle = (gs_ul_captured_rb - gs_ul_captured_ra) * 100 / gs_ul_captured_rb;
				printf("Captured wave frequency = %d Hz, Duty cycle = %d%% \n\r", frequence, dutycycle);

				gs_ul_captured_pulses = 0;
				gs_ul_captured_ra = 0;
				gs_ul_captured_rb = 0;
			} else {
				puts("No waveform has been captured\r");
			}
			puts("\n\rPress 'h' to display menu\r");
			break;

		case 'c':
			puts("Start capture, press 's' to stop \r");
			tc_enable_interrupt(TC0, TC_CHANNEL_2, TC_IER_LDRBS);
			/* Start the timer counter on TC0 channel 2 */
			tc_start(TC0, TC_CHANNEL_2);
			break;
		default:
			/* Set waveform configuration #n */
			if ((key >= '0') && (key <= ('0' + gc_uc_nbconfig - 1))) {
				if (!gs_ul_captured_pulses) {
					gs_uc_configuration = key - '0';
					tc_waveform_initialize();
				} else {
					puts("Capturing ... , press 's' to stop capture first \r");
				}
			}
			break;
		}
	}
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
