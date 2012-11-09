/**
 * \file
 *
 * \brief SAM0+ SPI Quick Start
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \license
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
 * \page spi_basic_use_case Quick Start Guide for the SPI module - Basic Use Case
 *
 * In this use case, the SPI will be used  The SPI will be set up as follows:
 * - Master Mode enabled
 * - MSB of the data is transmitted first
 * - Transfer mode 0
 * - \ref mux_setting_d
 * - Character size 8 bit
 * - Not enabled in sleep mode
 * - Receiver disabled
 * - Baudrate 9600
 *
 *
 * \section spi_basic_use_case_setup Quick Start
 *
 * \subsection spi_basic_use_case_prereq Prerequisites
 *
 * \subsection spi_basic_use_case_setup_code Code
 * The following must be added to the user application:
 *
 * A sample buffer to send via SPI:
 * \snippet qs_spi_basic.c buffer
 * Number of entries in the sample buffer:
 * \snippet qs_spi_basic.c buf_length
 * GPIO pin to use as Slave Select:
 * \snippet qs_spi_basic.c slave_select_pin
 *
 * Add to user application %main():
 * \snippet qs_spi_basic.c main
 *
 * \section spi_basic_use_case_workflow Workflow
 * -# Create configuration struct.
 *  - \snippet qs_spi_basic.c config
 * -# Create software device instance struct.
 *  - \snippet qs_spi_basic.c dev_inst
 * -# Create peripheral slave configuration struct.
 *  - \snippet qs_spi_basic.c slave_config
 * -# Create peripheral slave software device instance struct.
 *  - \snippet qs_spi_basic.c slave_dev_inst
 * -# Get default peripheral slave configuration.
 *  - \snippet qs_spi_basic.c slave_conf_defaults
 * -# Set Slave Select pin.
 *  - \snippet qs_spi_basic.c ss_pin
  * -# Initialize peripheral slave software instance with configuration.
 *  - \snippet qs_spi_basic.c slave_init
 * -# Get default configuration.
 *  - \snippet qs_spi_basic.c conf_defaults
 * -# Initialize SPI module with default configuration.
 *  - \snippet qs_spi_basic.c init
 * -# Edit configuration to disable receiver.
 *  - \snippet qs_spi_basic.c conf_rec
 * -# Enable SPI module.
 *  - \snippet qs_spi_basic.c config
 * -# Enable slave select pin.
 *  - \snippet qs_spi_basic.c slave_select
 * -# Write buffer to SPI slave.
 *  - \snippet qs_spi_basic.c write_buffer
 * -# Disable slave select pin.
 *  - \snippet qs_spi_basic.c slave_unselect
 * -# Infinite loop.
 *  - \snippet qs_spi_basic.c inf_loop
 */