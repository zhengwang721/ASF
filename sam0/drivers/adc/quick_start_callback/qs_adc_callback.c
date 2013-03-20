/**
 * \file
 *
 * \brief SAMD20 ADC Quick Start
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
#include <asf.h>
#define ADC_SAMPLES 128



//! [result_buffer]
uint16_t adc_result_buffer[ADC_SAMPLES];
//! [result_buffer]

//! [job_complete_callback]
volatile bool done = false;
static void adc_complete_callback(const struct adc_module *const module) {
	done = true;
}
//! [job_complete_callback]



int main(void)
{
//! [main]
//! [variable]
	struct adc_module module_inst;
	struct adc_config config;
//! [variable]
//! [system_init]
	system_init();
//! [system_init]
	/* Setup ADC module*/
//! [get_conf]
	adc_get_config_defaults(&config);
//! [get_conf]

//! [modify_conf]
	config.gain_factor = ADC_GAIN_FACTOR_DIV2;
	config.clock_prescaler = ADC_CLOCK_PRESCALER_DIV8;
	config.reference = ADC_REFERENCE_INTVCC1;
	config.positive_input = ADC_POSITIVE_INPUT_PIN4;
	config.resolution =  ADC_RESOLUTION_12BIT;
//! [modify_conf]

//! [init_adc]
	adc_init(&module_inst, ADC, &config);
//! [init_adc]
//! [enable]
	adc_enable(&module_inst);
//! [enable]

//! [setup_callback]
	adc_register_callback(&module_inst, adc_complete_callback, ADC_CALLBACK_READ_BUFFER);
	adc_enable_callback(&module_inst, ADC_CALLBACK_READ_BUFFER);
//! [setup_callback]


//! [start_adc_job]
	cpu_irq_enable();
	adc_read_buffer_job(&module_inst, ADC_SAMPLES, adc_result_buffer);
//! [start_adc_job]

//! [job_complete_poll]
	while(done != true);
//! [job_complete_poll]

//! [inf_loop]
	while (1) {
		/* Infinite loop */
	}
//! [inf_loop]
//! [main]
}
