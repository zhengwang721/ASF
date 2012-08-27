/**
 * \file
 *
 * \brief ADCIFA driver example
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
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

/** \mainpage
 * \section intro Introduction
 * Example use of the ASF ADCIFA driver for UC3 devices, giving a demonstration
 * of how to set up the module and perform conversions.
 *
 * \section files Main Files
 * - adcifa_example.c : ADCIFA code example
 * - conf_example.h : Example configuration for the chosen board hardware
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC for AVR32 and IAR Systems compiler
 * for AVR UC3. Other compilers may or may not work.
 *
 * \section deviceinfo Device Info
 * All AVR UC3 devices with a ADCIFA module can be used. This example has been
 * tested with the following setup:
 * - UC3C_EK evaluation kit
 *
 * \section setupinfo Setup Information
 * CPU speed: <i>16 MHz</i>
 * - [on UC3C_EK only] Connect a PC USB cable to the USB VCP plug (the USB plug
 * on the right) of the UC3C_EK. The PC is used as a power source. The UC3C0512C
 * USART2 is connected to the UC3B USART1. The UC3B holds a firmware that acts
 * as a USART to USB gateway. It implements a USB CDC class: when connected to a
 * PC, it will enumerate as a Virtual Com Port. Once the UC3B USB is correctly
 * installed on Windows, to communicate on this port, open a HyperTerminal
 * configured with the following settings: 57600 bps, 8 data bits, no parity
 * bit, 1 stop bit, no flow control.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <a href="http://www.atmel.com/avr">Atmel AVR</a>.\n
 */

#include <asf.h>
#include "conf_example.h"

/* GPIO pin/adc-function map. */
const gpio_map_t ADCIFA_GPIO_MAP = {
	{AVR32_ADCREF0_PIN, AVR32_ADCREF0_FUNCTION},
	{AVR32_ADCREFP_PIN, AVR32_ADCREFP_FUNCTION},
	{AVR32_ADCREFN_PIN, AVR32_ADCREFN_FUNCTION},
	{EXAMPLE_ADC_POTENTIOMETER_PIN, EXAMPLE_ADC_POTENTIOMETER_FUNCTION},
	{EXAMPLE_ADC_MIC_PIN, EXAMPLE_ADC_MIC_FUNCTION}
};

/* ADC Configuration */
adcifa_opt_t adc_config_t = {
	.frequency                = 1000000,
	.reference_source         = ADCIFA_ADCREF0,
	.sample_and_hold_disable  = false,
	.single_sequencer_mode    = false,
	.free_running_mode_enable = false,
	.sleep_mode_enable        = false
};

/* Sequencer Configuration */
adcifa_sequencer_opt_t adcifa_sequence_opt = {
	.convnb               = EXAMPLE_ADCIFA_NUMBER_OF_SEQUENCE,
	.resolution           = ADCIFA_SRES_12B,
	.trigger_selection    = ADCIFA_TRGSEL_SOFT,
	.start_of_conversion  = ADCIFA_SOCB_ALLSEQ,
	.sh_mode              = ADCIFA_SH_MODE_OVERSAMP,
	.half_word_adjustment = ADCIFA_HWLA_NOADJ,
	.software_acknowledge = ADCIFA_SA_NO_EOS_SOFTACK
};

/* Conversions in the Sequencer Configuration */
adcifa_sequencer_conversion_opt_t
	adcifa_sequence_conversion_opt[EXAMPLE_ADCIFA_NUMBER_OF_SEQUENCE]
		= {
			{
				.channel_p = EXAMPLE_ADC_POTENTIOMETER_INP,
				.channel_n = EXAMPLE_ADC_POTENTIOMETER_INN,
				.gain      = ADCIFA_SHG_1
			},
			{
				.channel_p = EXAMPLE_ADC_MIC_INP,
				.channel_n = EXAMPLE_ADC_MIC_INN,
				.gain      = ADCIFA_SHG_8
			}
		};


/** \brief Main function to initialize the system and loop to display ADC values */
int main( void )
{
	int16_t adc_values[EXAMPLE_ADCIFA_NUMBER_OF_SEQUENCE];

	/* Init system clocks */
	sysclk_init();

	/* Init debug serial line */
	init_dbg_rs232(sysclk_get_cpu_hz());

	/* Assign and enable GPIO pins to the ADC function. */
	gpio_enable_module(ADCIFA_GPIO_MAP, sizeof(ADCIFA_GPIO_MAP) /
			sizeof(ADCIFA_GPIO_MAP[0]));

	/* Get ADCIFA Factory Configuration */
	adcifa_get_calibration_data(&AVR32_ADCIFA, &adc_config_t);

	/* Configure ADCIFA core */
	adcifa_configure(&AVR32_ADCIFA, &adc_config_t, sysclk_get_cpu_hz());

	/* Configure ADCIFA sequencer 0 */
	adcifa_configure_sequencer(&AVR32_ADCIFA, 0, &adcifa_sequence_opt,
			adcifa_sequence_conversion_opt);

	/* Display a header to user */
	print_dbg("\x1B[2J\x1B[H\r\nADCIFA Example\r\n");

	while (true) {
		/* Start ADCIFA sequencer 0 */
		adcifa_start_sequencer(&AVR32_ADCIFA, 0);

		/* Get Values from sequencer 0 */
		if (adcifa_get_values_from_sequencer(&AVR32_ADCIFA, 0,
				&adcifa_sequence_opt, adc_values) == ADCIFA_STATUS_COMPLETED) {
			/* Display values to user */
			print_dbg("HEX Value for Channel potentiometer: 0x");
			print_dbg_hex(adc_values[0]);
			print_dbg("\r\n");
			print_dbg("HEX Value for Channel microphone: 0x");
			print_dbg_hex(~adc_values[1]);
			print_dbg("\r\n");

			/* Clear end-of-sequence for sequencer 0, ready for next conversion */
			ADCIFA_clear_eos_sequencer_0();
		}
		
		/* Slow down display of converted values */
		delay_ms(100);
	}
}
