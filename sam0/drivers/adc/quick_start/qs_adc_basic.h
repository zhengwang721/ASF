/**
 * \file
 *
 * \brief SAM D20 ADC Quick Start
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
 * \page asfdoc_samd20_adc_basic_use_case Quick Start Guide for ADC - Basic
 *
 * In this use case, the ADC will be used for..
 * The ADC will be set up as follows:
 * - 1V from internal bandgap reference
 * - Div 4 clock prescaler
 * - 12 bit resolution
 * - Window monitor disabled
 * - No gain
 * - Positive input on ADC PIN 0
 * - Negative input on ADC PIN 1
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
 * \section asfdoc_samd20_adc_basic_use_case_setup Setup
 *
 * \subsection asfdoc_samd20_adc_basic_use_case_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_samd20_adc_basic_use_case_code Code
 * Add to user application %main():
 * \snippet qs_adc_basic.c main_setup
 *
 * \section asfdoc_samd20_adc_basic_use_case_workflow Workflow
 * -# Create variables for device software instance, config and channel config.
 *  \snippet qs_adc_basic.c variable
 * -# Create variable for storing the ADC result.
 *  \snippet qs_adc_basic.c res_variable
 * -# Get ADC config defaults.
 *  \snippet qs_adc_basic.c get_conf
 * -# Initialize software instance with hardware module and write configuration
 * to the module.
 *  \snippet qs_adc_basic.c init_adc
 * -# Enable the ADC.
 *  \snippet qs_adc_basic.c enable
 
  * \section asfdoc_samd20_adc_basic_use_case_use Use Case
 * \subsection asfdoc_samd20_adc_basic_use_case_use_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_adc_basic.c main_use_case
 * \subsection asfdoc_samd20_adc_basic_use_case_use_workflow Workflow
  * -# Start conversion.
 *  \snippet qs_adc_basic.c start_conv
 * -# Wait until conversion is done and read result.
 *  \snippet qs_adc_basic.c get_res
 * -# Start infinite loop.
 *  \snippet qs_adc_basic.c inf_loop

 */
 */
