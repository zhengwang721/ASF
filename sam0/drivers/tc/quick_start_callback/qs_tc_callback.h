/**
 * \file
 *
 * \brief SAMD20 USART Interface Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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
 * \page tc_callback Quick Start Guide for the TC module - Callback
 *
 * In this use case, one TC instance will be used to generate a PWM
 * signal. The duty cycle of the PWM will be changed during runtime by
 * a callback function. The duty cycle will be 0.5 in one cycle and
 * 0.3 in the next and continue in this manner. The TC module
 * will be set up as follows:
 *
 * - GCLK generator 0 (GCLK main) clock source
 * - 16 bit resolution on the counter
 * - No prescaler
 * - Normal frequency wave generation
 * - GCLK reload action
 * - Don't run in standby
 * - No inversion of waveform output
 * - No capture enabled
 * - Count upward
 * - Don't perform oneshot operations
 * - No event input enabled
 * - No event action
 * - No event generation enabled
 * - Counter starts on 0
 * - Capture compare channel 0 set to 0x7FFF
 *
 * \section asfdoc_samd20_tc_basic_use_case_setup Quick Start
 *
 * \subsection asfdoc_samd20_tc_basic_use_case_prereq Prerequisites
 *
 * \subsection asfdoc_samd20_tc_basic_use_case_setup_code Code
 * The following must be added to the user application:
 *
 * Add to user application %main():
 * \snippet qs_tc_callback.c main
 *
 * -# Initialize system.
 *  - \snippet qs_tc_callback.c system_init
 * -# Create configuration struct.
 *  - \snippet qs_tc_callback.c config
 * -# Create software device instance struct.
 *  - \snippet qs_tc_callback.c dev_inst
 * -# Get default configuration values.
 *  - \snippet qs_tc_callback.c tc_get_config_defaults
 * -# Set up PWM output on channel 0.
 *  - \snippet qs_tc_callback.c pwm_channel_0
 * -# Set counter size, wave generation mode and compare capture value.
 *  - \snippet qs_tc_callback.c setup
 * -# Initialize the TC module based on given configuration values.
 *  - \snippet qs_tc_callback.c tc_init
 * -# Register the callback function to be used when a compare match occur.
 *  - \snippet qs_tc_callback.c register_callback
 * -# Enable callbacks for the given interrupt.
 *  - \snippet qs_tc_callback.c enable_callback
 * -# Enable global interrupts.
 *  - \snippet qs_tc_callback.c enable global interrupts
 * -# Enable and start the TC module.
 *  - \snippet qs_tc_callback.c tc_enable
 * -# Loop infinitely. Let the module generate PWM signal.
 *  - \snippet qs_tc_callback.c inf_loop
 * \section asfdoc_samd20_tc_basic_use_case_workflow Workflow
 */


#include <asf.h>
#include <conf_clocks.h>
