/**
 * \file
 *
 * \brief SAM DUALTIMER Driver Quick Start for SAMB11
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
 * \page asfdoc_sam0_dualtimer_basic_use_case Quick Start Guide for DUALTIMER - Basic
 *
 * In this use case, the DUALTIMER module is configured for two general downward timer.
 *
 * \section asfdoc_sam0_dualtimer_basic_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_dualtimer_basic_use_case_setup_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_sam0_dualtimer_basic_use_case_setup_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_dualtimer_basic.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_dualtimer_basic.c setup_init
 *
 * \subsection asfdoc_sam0_dualtimer_basic_use_case_setup_flow Workflow
 * -# Create a new function \c configure_dualtimer(), which will be used to configure
 *    the DUALTIMER peripheral.
 *    \snippet qs_dualtimer_basic.c setup_dualtimer_init
 *
 * -# Create a DUALTIMER module configuration struct, which can be filled out to
 *    adjust the configuration of a physical DUALTIMER peripheral.
 *    \snippet qs_dualtimer_basic.c setup_dualtimer_1
 * -# Initialize the DUALTIMER configuration struct with the module's default values.
 *    \snippet qs_dualtimer_basic.c setup_dualtimer_2
 *
 * -# Configure the DUALTIMER module with the desired settings.
 *    \snippet qs_dualtimer_basic.c setup_timer_3
 *
 * -# Set the DUALTIMER module timer1 enable.
 *    \snippet qs_dualtimer_basic.c setup_dualtimer1_enable
 *
 * -# Set the DUALTIMER module timer2 enable.
 *    \snippet qs_dualtimer_basic.c setup_dualtimer2_enable
 *
 *
 * \section asfdoc_sam0_dualtimer_basic_use_case_basic_implement Implementation
 * Add the following to \c main().
 * \snippet qs_dualtimer_basic.c main_imp
 *
 * \subsection asfdoc_sam0_dualtimer_basic_use_case_basic_workflow Workflow
 * -# Start an infinite loop, to get timer1/timer2 counter.
 *    \snippet qs_dualtimer_basic.c main_loop
 *
 * -# Get the dualtimer1 raw interrupt status.
 *    \snippet qs_dualtimer_basic.c timer1_interrupt
 * -# Clear the dualtimer1 interrupt status.
 *    \snippet qs_dualtimer_basic.c timer1_interrupt_clr
 * -# Plus the dualtimer_counter1.
 *    \snippet qs_dualtimer_basic.c counter1
 *
 * -# Get the dualtimer2 raw interrupt status.
 *    \snippet qs_dualtimer_basic.c timer2_interrupt
 * -# Clear the dualtimer1 interrupt status.
 *    \snippet qs_dualtimer_basic.c timer2_interrupt_clr
 * -# Plus the dualtimer_counter2.
 *    \snippet qs_dualtimer_basic.c counter1
 */