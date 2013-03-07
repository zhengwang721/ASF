/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
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
 * \page asfdoc_samd20_i2c_master_callback_use_case Quick Start Guide for the I2C Master Module - Callback Use Case
 *
 * In this use case, the I2C&trade; will be used as follows.
 *  - Master mode
 *  - 100 kHz operation speed
 *  - Not operational in standby
 *  - 10000 packet timeout value
 *  - 65535 unknown bus state timeout value
 *
 * \section i2c_master_basic_use_case_setup Quick Start
 *
 * \subsection i2c_master_basic_use_case_prereq Prerequisites
 * The quick start will need the system drivers to operate correctly.
 *
 * \section i2c_master_basic_use_case_setup_code Setup
 * To set up the I2C&trade; with desired configurations, add the following:
 *
 * \subsection i2c_master_basic_use_setup_code Code
 * Add this globally accessible, i.e at the start of your code outside any functions.
 * \snippet qs_i2c_master_callback.c dev_inst
 *
 * Function for setting up module.
 * \snippet qs_i2c_master_callback.c initialize_i2c
 *
 * and the following to the start of main():
 * \snippet qs_i2c_master_callback.c run_initialize_i2c
 *
 * \subsection i2c_master_basic_use_workflow Workflow
 * When setting up the device, the correct procedure will be to:
 * -# Initialize configuration structure.
 *  - \snippet qs_i2c_master_callback.c init_conf
 * -# Change settings in the configuration.
 *  - \snippet qs_i2c_master_callback.c conf_change
 * -# Initialize the module with the set configurations.
 *  - \snippet qs_i2c_master_callback.c init_module
 * -# Configure callback funtionality.
 *  - \snippet qs_i2c_master_callback.c setup_callback
 * -# Enable the module.
 *  - \snippet qs_i2c_master_callback.c enable_module
 *
 * \section i2c_master_basic_use_implementation Implementation
 * To use the I2C&trade;, you must make a packet that can be used for the transfer.
 * \snippet qs_i2c_master_callback.c packet
 * Where the 3 values are defined and initialized as:
 * \snippet qs_i2c_master_callback.c packet_data
 * Then the first packet can be sent using i2c_master_async_write_packet function:
 * \snippet qs_i2c_master_callback.c write_packet
 *
 * When the first packet is sent, it will enter the callback function, and continue sending
 * the same data, reverted.
 *
 */

#include <asf.h>
#include <conf_clocks.h>

