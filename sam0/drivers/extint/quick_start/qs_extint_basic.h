/**
 * \file
 *
 * \brief SAMD20 External Interrupt Driver Quick Start
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 * \page asfdoc_samd20_extint_basic_use_case Quick Start - Basic Use Case
 *
 * In this use case, the EXTINT module is configured for:
 *  - Channel 1 routed to physical pin PA01
 *  - Channel 1 configured to automatically wake up the device from sleep
 *      upon detection
 *  - Channel 1 configured to detect falling input edges
 *
 * This use case configures a physical I/O pin of the device so that it is
 * routed to a logical External Interrupt Controller channel to detect falling
 * edges of the incoming signal.
 *
 *
 * \section asfdoc_samd20_extint_basic_use_case_setup Setup
 *
 * \subsection asfdoc_samd20_extint_basic_use_case_setup_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_samd20_extint_basic_use_case_setup_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_extint_basic.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_extint_basic.c setup_init
 *
 * \subsection asfdoc_samd20_extint_basic_use_case_setup_flow Workflow
 * -# Create an EXTINT module channel configuration struct, which can be filled
 *    out to adjust the configuration of a single external interrupt channel.
 *  - \snippet qs_extint_basic.c setup_1
 * -# Initialize the channel configuration struct with the module's default
 *    values.
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 *  - \snippet qs_extint_basic.c setup_2
 * -# Adjust the configuration struct to configure the pin MUX (to route the
 *    desired physical pin to the logical channel) and to configure the channel
 *    into auto-wake, unfiltered mode with falling edge detection.
 *  - \snippet qs_extint_basic.c setup_3
 * -# Configure external interrupt channel 1 with the desired configuration
 *    settings.
 *  - \snippet qs_extint_basic.c setup_4
 *
 *
 * \section asfdoc_samd20_extint_basic_use_case_main Use Case
 *
 * \subsection asfdoc_samd20_extint_basic_use_case_main_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_extint_basic.c main
 *
 * \subsection asfdoc_samd20_extint_basic_use_case_main_flow Workflow
 * -# Read in the current external interrupt channel 1 state to see if a falling
 *    edge has been detected.
 *  - \snippet qs_extint_basic.c main_1
 * -# Clear the detection state of the external interrupt channel 1 so that it
 *    is ready to detect a future falling edge.
 *  - \snippet qs_extint_basic.c main_2
 */
