/**
 * \file
 *
 * \brief SAMD20 PAC Quick Start
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
 * \page asfdoc_samd20_pac_basic_use_case Quick Start Guide for the PAC module - Basic Use Case
 *
 * In this use case, the peripheral-lock will be used to lock and unlock the
 * PORT peripheral access, and show how to implement the PAC module when the
 * PORT registers needs to be altered. The PORT will be set up as follows:
 * - One pin in input mode, with pull-up and falling edge-detect.
 * - One pin in output mode.
 *
 * \section asfdoc_samd20_pac_basic_use_case_setup Quick Start
 *
 * \subsection asfdoc_samd20_pac_basic_use_case_prereq Prerequisites
 * Needs to include and set up the PORT driver. A suggested set up for the PORT
 * in the quick start, but the drivers for the PORT will need to be included.
 *
 * \subsection asfdoc_samd20_pac_basic_use_case_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_pac_basic.c pin_setup
 * Add to user application %main():
 * \snippet qs_pac_basic.c main
 *
 * \section asfdoc_samd20_pac_basic_use_case_main Workflow
 * -# Make function to set up PORT module.
 *  - \snippet qs_pac_basic.c pin_setup
 * -# Initialize the system peripherals and set up some GPIO port pins.
 *  - \snippet qs_pac_basic.c init
 * -# Lock peripheral access for PORT
 *  - \snippet qs_pac_basic.c init_lock
 * -# Enable global interrupts.
 *  - \snippet qs_pac_basic.c enable_interrupts
 * -# Loop to wait for edge detect.
 *  - \snippet qs_pac_basic.c button_press
 * -# Disable interrupts globally
 *  - \snippet qs_pac_basic.c disable_interrupts
 * -# Unlock the peripheral registers.
 *  - \snippet qs_pac_basic.c unlock_perph
 * -# Toggle pin 11, and clear edge detect flag.
 *  - \snippet qs_pac_basic.c alter_config
 * -# Lock the peripheral registers.
 *  - \snippet qs_pac_basic.c lock_perph
 * -# Re-enable global interrupts.
 *  - \snippet qs_pac_basic.c enable_interrupts_2
 * -# Start inf wait loop.
 *  - \snippet qs_pac_basic.c inf_loop
 */










