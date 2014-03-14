/**
 * \file
 *
 * \brief SAM D21 TCC - Timer Counter for Control Applications Callback Driver Quick Start
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
 * \page asfdoc_sam0_tcc_bldc_use_case Quick Start Guide for TCC - BLDC
 *
 * The supported board list:
 *    - SAM D21 Xplained Pro
 *
 * In this use case, the TCC will be used to generate 6 PWM signals, for BLDC
 * drive. To see the waveform, you may need an oscilloscope.
 *
 * The TCC module will be set up as follows:
 * - GCLK generator 0 (GCLK main) clock source
 * - No dithering on the counter or compare
 * - No prescaler
 * - Double Slope PWM wave generation with interrupt on both bottom and top
 * - GCLK reload action
 * - Don't run in standby
 * - No faults or waveform extensions
 * - No inversion of waveform output
 * - No capture enabled
 * - Count upward
 * - Don't perform one-shot operations
 * - No event input enabled
 * - No event action
 * - No event generation enabled
 * - Counter starts on 0
 * - Period value is set to 4800
 * - Compare match channel 0 and 2 values are set to 1/3 period
 * - Compare match channel 1 and 3 values are set to 2/3 period
 * - Wave generation polarity on compare match channel 0 and 2 are normal
 * - Wave generation polarity on compare match channel 1 and 3 are inverted
 * - The wave output 0 ~ 5 are enabled
 * - The wave output 4 and 5 are inverted
 *
 * Then in callback handler the patterns of wave output 0 ~ 3 are changed, to
 * generate the right waveform.
 *
 * \section asfdoc_sam0_tcc_bldc_use_case_setup Quick Start
 *
 * \subsection asfdoc_sam0_tcc_bldc_use_case_prereq Prerequisites
 * There are no prerequisites for this use case.
 *
 * \subsection asfdoc_sam0_tcc_bldc_use_case_setup_code Code
 *
 * Add to the main application source file, before any functions:
 * \snippet conf_quick_start_bldc.h definition_pwm
 * \snippet qs_tcc_bldc.c setup_config_values
 *
 * Add to the main application source file, outside of any functions:
 * \snippet qs_tcc_bldc.c module_inst
 *
 * Copy-paste the following callback function code to your user application:
 * \snippet qs_tcc_bldc.c callback_funcs
 *
 * Copy-paste the following setup code to your user application:
 * \snippet qs_tcc_bldc.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_tcc_bldc.c setup_init
 *
 * \subsection asfdoc_sam0_tcc_bldc_use_case_setup_flow Workflow
 * -# Create a module software instance structure for the TCC module to store
 *    the TCC driver state while it is in use.
 *    \snippet qs_tcc_bldc.c module_inst
 *    \note This should never go out of scope as long as the module is in use.
 *          In most cases, this should be global.
 *
 * -# Configure the TCC module.
 *  -# Create a TCC module configuration struct, which can be filled out to
 *     adjust the configuration of a physical TCC peripheral.
 *     \snippet qs_tcc_bldc.c setup_config
 *  -# Initialize the TCC configuration struct with the module's default values.
 *     \snippet qs_tcc_bldc.c setup_config_defaults
 *     \note This should always be performed before using the configuration
 *           struct to ensure that all values are initialized to known default
 *           settings.
 *
 *  -# Alter the TCC settings to configure the double buffering, period, wave
  *    generation mode, polarity, the compare channel 0 value and output
  *    invertion.
 *     \snippet qs_tcc_bldc.c setup_change_config
 *  -# Alter the TCC settings to configure the PWM outputs on physical device
 *     pins.
 *     \snippet qs_tcc_bldc.c setup_change_config_pwm
 *  -# Configure the TCC module with the desired settings.
 *     \snippet qs_tcc_bldc.c setup_set_config
 *  -# Enable the TCC module to start the timer and begin PWM signal generation.
 *     \snippet qs_tcc_bldc.c setup_enable
 * -# Configure the TCC callbacks.
 *  -# Register the Counter Overflow callback functions with the driver.
 *     \snippet qs_tcc_bldc.c setup_register_callback
 *  -# Enable the Counter Overflow callback so that it will be called by
 *     the driver when appropriate.
 *     \snippet qs_tcc_bldc.c setup_enable_callback
 *
 *
 * \section asfdoc_sam0_tcc_bldc_use_case_main Use Case
 *
 * \subsection asfdoc_sam0_tcc_bldc_use_case_main_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_tcc_bldc.c main
 *
 * \subsection asfdoc_sam0_tcc_bldc_use_case_main_flow Workflow
 * -# Enter an infinite loop while the PWM waves are generated via the
 *    TCC module.
 *  \snippet qs_tcc_bldc.c main_loop
 */


#include <asf.h>
#include <conf_clocks.h>