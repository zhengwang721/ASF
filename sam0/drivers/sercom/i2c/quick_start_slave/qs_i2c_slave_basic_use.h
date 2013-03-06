/**
 * \file
 *
 * \brief SAM D20 I2C Slave Quick Start Guide with Callbacks
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
 * \page asfdoc_samd20_i2c_slave_basic_use_case Quick Start Guide for the I2C Slave module - Callback Use Case
 *
 * In this use case, the I2C will be used as follows.
 *  - Slave mode
 *
 * \section i2c_slave_basic_use_case_setup Quick Start
 *
 * \subsection i2c_slave_basic_use_case_prereq Prerequisites
 *
 * \section i2c_slave_basic_use_case_setup_code Setup
 * To set up the I2C with desired configurations, add the following:
 *
 * \subsection i2c_slave_basic_use_setup_code Code
 * Add this globally accessible, i.e at the start of your code outside any functions.
 * \snippet qs_i2c_slave_basic_use.c module
 * \snippet qs_i2c_slave_basic_use.c packet
 *
 * Function for setting up module.
 * \snippet qs_i2c_slave_basic_use.c initialize_i2c
 * Callback functions for read request:
 * \snippet qs_i2c_slave_basic_use.c read_request
 * Function for write request from a master:
 * \snippet qs_i2c_slave_basic_use.c write_request
 * Function for setting up the callback functionality of the driver.
 * \snippet qs_i2c_slave_basic_use.c setup_i2c_callback
 * Add the following to the start of main():
 * \snippet qs_i2c_slave_basic_use.c run_initialize_i2c
 *
 * \subsection i2c_slave_basic_use_workflow Workflow
 * When setting up the device, the correct procedure will be to:
 * -# Initialize configuration structure.
 *  - \snippet qs_i2c_slave_basic_use.c init_conf
 * -# Change settings in the configuration.
 *  - \snippet qs_i2c_slave_basic_use.c conf_changes
 * -# Initialize the module with the set configurations.
 *  - \snippet qs_i2c_slave_basic_use.c init_module
 * -# Enable the module.
 *  - \snippet qs_i2c_slave_basic_use.c enable_module
 * -# Register and enable callback functions.
 *  - \snippet qs_i2c_slave_basic_use.c reg_en_i2c_callback
 *
 * \section i2c_slave_basic_use_implementation Implementation
 * To use the I2C, you must make a packet that can be used for the reading:
 * \snippet qs_i2c_slave_basic_use.c packet_read
 * and writing:
 * \snippet qs_i2c_slave_basic_use.c packet_write
 * Where the values are defined and initialized as:
 * \snippet qs_i2c_slave_basic_use.c packet_data
 * Then the packets can be sent by using the i2c_slave_write_packet function from the read request
 * callback:
 * \snippet qs_i2c_slave_basic_use.c write_packet
 * and received by using the i2c_slave_read_packet function from the write request callback.
 * \snippet qs_i2c_slave_basic_use.c read_packet
 */

#include <asf.h>
#include <conf_clocks.h>

