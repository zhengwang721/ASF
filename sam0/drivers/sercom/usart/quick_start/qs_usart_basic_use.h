/**
 * \file
 *
 * \brief SAMD20 USART Interface Driver
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
 * \page asfdoc_samd20_sercom_usart_basic_use_case Quick Start Guide for the USART module - Basic Use Case
 *
 * In this use case the USART will be used and set up as follows:
 * - Asynchronous mode
 * - 9600 Baudrate
 * - 8-bits, no parity and 1 stopbit
 * - TX and RX connected to Xplained PRO embedded debugger virtual COM port
 *
 * \section asfdoc_samd20_sercom_usart_basic_use_case_setup Quick Start
 *
 * \subsection asfdoc_samd20_sercom_usart_basic_use_case_prereq Prerequisites
 *
 * \subsection asfdoc_samd20_sercom_usart_basic_use_case_setup_code Code
 * The following must be added to the user application:
 *
 * Add to user application %main():
 * \snippet qs_usart_basic_use.c main 
 *
 * \section asfdoc_samd20_sercom_usart_basic_use_case_workflow Workflow
 * -# Initialize system
 *  - \snippet qs_usart_basic_use.c system_init
 * -# Create configuration struct
 *  - \snippet qs_usart_basic_use.c config
 * -# Create software device module instance
 *  - \snippet qs_usart_basic_use.c module_inst
 * -# Get default configuration values
 *  - \snippet qs_usart_basic_use.c conf_defaults
 * -# Modify confiuration defaults for Embedded Debugger (EDBG) pins
 *  - \snippet qs_usart_basic_use.c conf_modify
 * -# Initialize USART with given configuration
 *  - \snippet qs_usart_basic_use.c init
 * -# Enable USART
 *  - \snippet qs_usart_basic_use.c enable
 * -# Enable Tx and RX
 *  - \snippet qs_usart_basic_use.c enable_transceivers
 * -# Echo every received character back
 *  - \snippet qs_usart_basic_use.c echo_characters
 *
 */

#include <asf.h>
#include <conf_clocks.h>

