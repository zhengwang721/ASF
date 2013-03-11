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
 * \page asfdoc_samd20_i2c_slave_basic_use_case Quick Start Guide for the I2C Slave Module - Basic Use Case
 *
 * In this use case, the I<SUP>2</SUP>C will used and set up as follows:
 *  - Slave mode
 *
 * \section asfdoc_samd20_i2c_slave_basic_use_case_prereq Prerequisites
 * The device must be connected to an I<SUP>2</SUP>C master.
 *
 * \section asfdoc_samd20_i2c_slave_basic_use_case_setup_code Setup
 *
 * \subsection asfdoc_samd20_i2c_slave_basic_use_setup_code Code
 * The following must be added to the user application:
 *
 *
 * \subsection asfdoc_samd20_i2c_slave_basic_use_setup_workflow Workflow
 * -# Initialize system.
 *  - \snippet qs_i2c_slave_basic_use.c system_init
 *
 * \section asfdoc_samd20_i2c_slave_basic_use_implementation Implementation
 * \subsection asfdoc_samd20_i2c_slave_basic_use_implementation_code Code
 * Add to user application main:
 * \snippet 
 * \subsection i2c_slave_basic_use_implementation_workflow Workflow 
 * -# 
 *  - \snippet 
 */

#include <asf.h>
#include <conf_clocks.h>

