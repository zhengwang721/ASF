/**
 * \file
 *
 * \brief Analog Comparator Interfacer (ACIFC) example for SAM.
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

/**
 * \mainpage acifc_irq ACIFC IRQ Example
 *
 * \section Purpose
 *
 * The acifc_irq example demonstrates how to use the ACIFC peripheral to
 * detect comparison event on the input pair.
 *
 * \section Requirements
 *
 * This example can be used with any SAM MCU featuring Analog Comparator
 * Interface.
 *
 * \section Description
 *
 * The acifc_irq is aimed to demonstrate the usage of ACIFC peripheral with
 * interrupt support. The PA06 and PA07 are selected as two inputs. Connect
 * PA06 and ADC_SENSOR output voltage, PA07 and GND or 3.3V voltage.
 *
 * The comparison event would be generated if the voltage of one input is
 * changed across the voltage of the other input. Both bigger and less events
 * could be triggered by default.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/6421B.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a>
 *    application note or to the
 *    <a href="http://www.iar.com/website1/1.0.1.0/78/1/">
 *    IAR EWARM User and reference guides</a>,
 *    depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# In the terminal window, the
 *    following text should appear (values depend on the board and chip used):
 *    \code
 *     -- ACIFC IRQ Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

/** Analog comparator channel number */
#define EXAMPLE_AC_CHANNEL         0

#define STRING_EOL    "\r"
#define STRING_HEADER "-- ACIFC IRQ Example  --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/**
 * Interrupt handler for the ACIFC.
 */
static void compare_result_output(struct ac_dev_inst *const dev_inst)
{
	uint32_t ul_int_status, ul_comp_status;

	ul_int_status = ac_get_interrupt_status(dev_inst);
	ul_comp_status = ac_get_status(dev_inst);

	/* Compare Output Interrupt */
	if ((ul_int_status & ACIFC_ISR_ACINT0) == ACIFC_ISR_ACINT0) {
		if ((ul_comp_status & ACIFC_SR_ACCS0) == ACIFC_SR_ACCS0) {
			puts("-ISR- Voltage Comparison Result: ACAP0 > ACAN0\r");
		} else {
			puts("-ISR- Voltage Comparison Result: ACAP0 < ACAN0\r");
		}
	}
	ac_clear_interrupt_status(dev_inst, ACIFC_ICR_ACINT0);
}

/**
 *  Configure UART console.
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

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

struct ac_dev_inst ac_device;

/**
 *  \brief ACIFC example application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/* Initialize the system */
	sysclk_init();
	board_init();

	/* Initialize debug console */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* AC instance configuration */
	struct ac_config module_cfg;
	ac_get_config_defaults(&module_cfg);
	ac_init(&ac_device, ACIFC, &module_cfg);

	/* AC channel configuration */
	struct ac_ch_config ch_cfg;
	ac_ch_get_config_defaults(&ch_cfg);
	ch_cfg.always_on = true;
	ch_cfg.fast_mode = true;
	ac_ch_set_config(&ac_device, EXAMPLE_AC_CHANNEL, &ch_cfg);

	ac_set_callback(&ac_device, compare_result_output, 1, ACIFC_IER_ACINT0);

	ac_enable(&ac_device);

	/* Start the comparison */
	ac_user_trigger_single_comparison(&ac_device);

	while (1) {
	}
}
