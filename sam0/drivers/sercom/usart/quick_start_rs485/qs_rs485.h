/**
 * \file
 *
 * \brief SAM USART RS485 Quick Start
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

/**
 * \page asfdoc_sam0_sercom_usart_rs485_use_case Quick Start Guide for SERCOM USART RS485
 *
 * The supported board list:
 *    - SAMC21 Xplained Pro
 *
 * This quick start will set up RS485 transmission.
 *
 * \section asfdoc_sam0_sercom_usart_rs485_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_usart_rs485_use_case_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_sam0_usart_rs485_use_case_setup_code Code
 * Add to the main application source file, outside of any functions:
 * \snippet qs_rs485.c module_var
 *
 * Copy-paste the following setup code to your user application:
 * \snippet qs_rs485.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_rs485.c setup_init
 *
 * \subsection asfdoc_sam0_usart_rs485_use_case_setup_flow Workflow
 * -# Create USART CDC and RS485 module software instance structure for the USART module to store
 *    the USART driver state while it is in use.
 *    \snippet qs_rs485.c module_inst
 *
 * -# Define RS485 RX/TX buffer.
 *    \snippet qs_rs485.c rs485_buffer
 *
 * -# Configure the USART CDC for output message.
 *     \snippet qs_rs485.c CDC_setup
 *
 * -# Configure the USART RS485 module.
 *     \snippet qs_rs485.c rs485_setup
 *
 * \section asfdoc_sam0_usart_rs485_use_case_main Use Case
 *
 * \subsection asfdoc_sam0_usart_rs485_use_case_main_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_rs485.c main_setup
 *
 * \subsection asfdoc_sam0_usart_rs485_use_case_main_flow Workflow
 * -# Configure USART RS485 module.
 *     \snippet qs_rs485.c configure_rs485
 * -# Sending RS485 message every \c RS485_SENDING_MSG_INTERVAL .
 *     \snippet qs_rs485.c rs485_msg
 */
