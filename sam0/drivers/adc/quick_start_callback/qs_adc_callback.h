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

/**
 * \page asfdoc_samd20_adc_basic_use_case_callback Quick Start Guide for the ADC module with callback support - Basic Use Case
 *
 * In this use case, the ADC will be convert 128 samples using interrupt driven conversion.
 * When all samples have been sampled, a callback will be called that signals the main
 * application that conversion is compete.
 *
 * The ADC will be set up as follows:
 * - VCC / 2 as reference
 * - Div 8 clock prescaler
 * - 12 bit resolution
 * - Window monitor disabled
 * - 1/2 gain
 * - Positive input on ADC PIN 0
 * - Negative input to GND (Single ended)
 * - Averaging disabled
 * - Oversampling disabled
 * - Right adjust data
 * - Single-ended mode
 * - Free running disabled
 * - All events (input and generation) disabled
 * - Sleep operation disabled
 * - No reference compensation
 * - No gain/offset correction
 * - No added sampling time
 * - Pin scan mode disabled
 *
 * \section asfdoc_samd20_adc_callback_basic_use_case_callback_setup Quick Start
 *
 * \subsection asfdoc_samd20_adc_callback_basic_use_case_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_samd20_adc_callback_basic_use_case_callback_code Code
 * Add to user application %main():
 * \snippet qs_adc_callback.c main
 * Add in same file as your %main():
 * \snippet qs_adc_callback.c result_buffer
 * \snippet qs_adc_callback.c job_complete_callback
 *
 *
 * \section asfdoc_samd20_adc_basic_use_case_callback_workflow Workflow
 * -# Create variables for device software instance, config and channel config.
 *  - \snippet qs_adc_callback.c variable
 * -# Initialize system (clock system)
 *  - \snippet qs_adc_callback.c system_init
 * -# Get ADC config defaults.
 *  - \snippet qs_adc_callback.c get_conf
 * -# Modify ADC default configuration.
 *  - \snippet qs_adc_callback.c modify_conf
 * -# Initialize software instance with hardware module and write configuration
 * to the module.
 *  - \snippet qs_adc_callback.c init_adc
 * -# Enable the ADC.
 *  - \snippet qs_adc_callback.c enable
 * -# Configure and enable a conversion complete callback
 *  - \snippet qs_adc_callback.c setup_callback
 * -# Start an ADC conversion job of ADC_SAMPLES number of samples
 *  - \snippet qs_adc_callback.c start_adc_job
 * -# Wait until job is complete
 *  - \snippet qs_adc_callback.c job_complete_poll
 * -# Hang in infinite loop.
 *  - \snippet qs_adc_callback.c inf_loop
 */
