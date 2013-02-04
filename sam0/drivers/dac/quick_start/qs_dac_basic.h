/**
 * \file
 *
 * \brief SAMD20 DAC Quick Start
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
 * \page dac_basic_use_case Quick Start Guide for the DAC module - Basic Use Case
 *
 * In this use case, the DAC will be used  The DAC will be set up as follows:
 * - Analog VCC as reference
 * - Internal output disabled
 * - Drive the DAC output to the VOUT pin
 * - Right adjust data
 * - The output buffer is disabled when the chip enters STANDBY sleep mode
 *
 * \section dac_basic_use_case_setup Quick Start
 *
 * \subsection dac_basic_use_case_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection dac_basic_use_case_code Code
 * Add to user application %main():
 * \snippet qs_dac_basic.c main
 *
 * \section dac_basic_use_case_workflow Workflow
 * -# Create variables for device software instance, config and channel config.
 *  - \snippet qs_dac_basic.c variable
 * -# Get DAC config defaults.
 *  - \snippet qs_dac_basic.c get_conf
 * -# Initialize software instance with hardware module and write configuration
 * to the module.
 *  - \snippet qs_dac_basic.c init_dac
 * -# Enable the DAC.
 *  - \snippet qs_dac_basic.c enable
 * -# Get DAC channel config defaults.
 *  - \snippet qs_dac_basic.c get_ch_conf
 * -# Disable event triggered conversion.
 *  - \snippet qs_dac_basic.c edit_ch_conf
 * -# Write the channel configuration to the DAC.
 *  - \snippet qs_dac_basic.c set_ch_conf
  * -# Enable the DAC channel.
 *  - \snippet qs_dac_basic.c enable_ch
 * -# Write and convert a value.
 *  - \snippet qs_dac_basic.c write
 * -# Disable the DAC
 *  - \snippet qs_dac_basic.c disable
 * -# Start infinite loop.
 *  - \snippet qs_dac_basic.c inf_loop
 */
