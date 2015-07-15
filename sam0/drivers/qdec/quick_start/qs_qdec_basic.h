/**
 * \file
 *
 * \brief SAM QUAD DECODER Driver Quick Start for SAMB11
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

/**
 * \page asfdoc_sam0_qdec_use_case Quick Start Guide for QUAD DECODER - Basic
 *
 * In this use case, the QUAD DECODER module is configured for quad decoder.
 *
 * \section asfdoc_sam0_qdec_basic_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_qdec_basic_use_case_setup_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_sam0_qdec_basic_use_case_setup_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_qdec_basic.c setup
 *
 * \subsection asfdoc_sam0_qdec_use_case_main_code Add to Main
 * Add the following to \c main().
 * \snippet qs_qdec_basic.c add_main
 *
 * \subsection asfdoc_sam0_qdec_basic_use_case_setup_flow Workflow
 * -# Make configuration structure.
 *    \snippet qs_qdec_basic.c set_conf
 *
 * -# Fill the configuration structure with the default driver configuration.
 *    \snippet qs_qdec_basic.c get_def
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Initialize module.
 *    \snippet qs_qdec_basic.c init_qdec1
 *
 * -# Change configurations as desired.
 *    \snippet qs_qdec_basic.c set_config1
 *
 * -# Initialize module.
 *    \snippet qs_qdec_basic.c init_qdec2
 *
 * -# Change configurations as desired.
 *    \snippet qs_qdec_basic.c set_config2
 *
 * -# Initialize module.
 *    \snippet qs_qdec_basic.c init_qdec3
 *
 * -# Enable clock.
 *    \snippet qs_qdec_basic.c enable_clk
 *
 * -# Enable module.
 *    \snippet qs_qdec_basic.c enable_qdec
 *
 * \section asfdoc_sam0_qdec_basic_use_case_basic_implement Implementation
 * Add the following to \c main().
 * \snippet qs_qdec_basic.c main_imp
 *
 * \subsection asfdoc_sam0_qdec_basic_use_case_basic_workflow Workflow
 * -# Start an infinite loop, to continuously get quad decoder counter.
 *    \snippet qs_qdec_basic.c main_loop
 *
 * -# Get the irq status.
 *    \snippet qs_qdec_basic.c get_irq
 *
 * -# Check to see if irq occurred.
 *    \snippet qs_qdec_basic.c ckeck_status
 * -# Once an irq occurs, clear the irq status.
 *    \snippet qs_qdec_basic.c clear_status
 * -# Reset the module, so that reset counter to 0
 *    \snippet qs_qdec_basic.c reset
 *
 * -# Get the x,y,z counter
 *    \snippet qs_qdec_basic.c get_counter
 */