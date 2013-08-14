/**
 * \file
 *
 * \brief SLCDC example for SAM.
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
 * \section intro Introduction
 * This example demonstrates how to use SLCDC driver to address an external
 * LCD segment (C42364A) and LCD still display in steady state after core
 * entering backup mode.
 *
 * \section files Main Files
 *  - slcdc.c SLCDC driver implementation
 *  - slcdc.h SLCDC driver definitions
 *  - slcdc_example.c example application
 *  - conf_example.h: configuration of the example
 *
 * \section compilinfo Compilation Information
 * This software is written for GNU GCC and IAR Embedded Workbench
 * for Atmel. Other compilers may or may not work.
 *
 * \section deviceinfo Device Information
 * All SAM devices with a SLCDC and a USART module can be used.
 *
 * \section exampledescription Description of the example
 * After an initialization phase, the display memory in SLCDC will be set.
 * Then the core will enter backup mode and LCD still display in steady sate mode.
 *
 * \section configinfo Configuration Information
 * - PC terminal settings:
 *   - 115200 bps,
 *   - 8 data bits,
 *   - no parity bit,
 *   - 1 stop bit,
 *   - no flow control.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com">Atmel</A>.\n
 * Support and FAQ: http://support.atmel.com/
 */

#include <asf.h>

#define  SLCDC_FRAME_RATE     64

/**
 *  Configure serial console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Application entry point for SLCDC example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/* SLCDC configuration */
	struct slcdc_config slcdc_cfg;
	status_code_t status;

	/* Initialize the SAM system */
	sysclk_init();
	board_init();
	sleepmgr_init();

	/* Initialize the console uart */
	configure_console();

	/* Output example information */
	printf("-- SLCDC Example --\r\n");
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	/* Turn on LCD back light */
	ioport_set_pin_level(LCD_BL_GPIO, IOPORT_PIN_LEVEL_LOW);

	/* SLCDC Reset */
	slcdc_reset(SLCDC);
	/*
	 * SLCDC initialization
	 * - Clock,
	 * - Display mode: Normal
	 * - Frame Rate:  64Hz
	 * - Power mode: Internal supply
	 */
	slcdc_cfg.buf_time = SLCDC_BUFTIME_X64_SCLK;
	slcdc_cfg.frame_rate= SLCDC_FRAME_RATE;
	slcdc_cfg.disp_mode = SLCDC_DISPMODE_NORMAL;
	slcdc_cfg.power_mode = SLCDC_POWER_MODE_LCDON_INVR;

	status = slcdc_init(SLCDC, &slcdc_cfg);
	if (status != STATUS_OK) {
		printf("-- Initialization fails! --\r\n");
		while (1) {
		}
	}
	/*LCD seg 17, 20~22, and 24 ~49 mapped on SEGx I/O pin */
	slcdc_set_segmap0(SLCDC,0xff720000);
	slcdc_set_segmap1(SLCDC,0x3ffff);

	/* Enable SLCDC */
	slcdc_enable(SLCDC);
	while(!slcdc_get_status(SLCDC));
	
	/* Display all SLCDC memory*/
	slcdc_set_display_memory(SLCDC);

	/* Enter in sleep mode */
	sleepmgr_enter_sleep();

	while (1) {
	}
}
