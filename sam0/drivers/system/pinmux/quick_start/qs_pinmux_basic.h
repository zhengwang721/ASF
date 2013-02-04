/**
 * \file
 *
 * \brief SAMD20 PINMUX Driver Quick Start
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
 * \page pinmux_basic_use_case Quick Start Guide for the PINMUX module - Basic Use Case
 *
 * In this use case, the PINMUX module is configured for:
 *  \li One pin in input mode, with pull-up enabled, connected to the GPIO
 *      module
 *  \li Sampling mode of the pin changed to sample on demand
 *
 * This use case sets up the PINMUX to configure a physical I/O pin set as
 * an input with pullup. and changes the sampling mode of the pin to reduce
 * power by only sampling the physical pin state when the user application
 * attempts to read it.
 *
 * \section pinmux_basic_use_case_setup Setup
 *
 * \subsection pinmux_basic_use_case_setup_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection pinmux_basic_use_case_setup_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_pinmux_basic.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_pinmux_basic.c setup_init
 *
 * \subsection pinmux_basic_use_case_setup_flow Workflow
 * -# Create a PINMUX module pin configuration struct, which can be filled out
 *    to adjust the configuration of a single port pin.
 *  - \snippet qs_pinmux_basic.c setup_1
 * -# Initialize the pin configuration struct with the module's default values.
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 *  - \snippet qs_pinmux_basic.c setup_2
 * -# Adjust the configuration struct to request an input pin with pullup
 *    connected to the GPIO peripheral.
 *  - \snippet qs_pinmux_basic.c setup_3
 * -# Configure GPIO10 with the initialized pin configuration struct, to enable
 *    the input sampler on the pin.
 *  - \snippet qs_pinmux_basic.c setup_4
 * -# Adjust the configuration of the pin to enable on-demand sampling mode.
 *  - \snippet qs_pinmux_basic.c setup_5
 *
 * \section pinmux_basic_use_case Use Case
 *
 * \subsection pinmux_basic_use_case_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_pinmux_basic.c main
 *
 * \subsection pinmux_basic_use_case_flow Workflow
 * -# None
 */
