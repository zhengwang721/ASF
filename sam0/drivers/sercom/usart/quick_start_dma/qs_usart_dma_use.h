/**
 * \file
 *
 * \brief SAM D2x Usart driver example with DMA
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
 * \page asfdoc_sam0_sercom_usart_dma_use_case Quick Start Guide for SERCOM USART - DMA
 *
 * This quick start will transmit a string to the terminal through DMA. In this
 * use case the USART will be configured with the following settings:
 * - Asynchronous mode
 * - 9600 Baudrate
 * - 8-bits, No Parity and 1 Stop Bit
 * - TX and RX enabled and connected to the Xplained PRO Embedded Debugger virtual COM port
 *
 * \section asfdoc_sam0_sercom_usart_dma_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_usart_dma_use_case_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_sam0_usart_dma_use_case_setup_code Code
 * Add to the main application source file, outside of any functions:
 * \snippet qs_usart_dma_use.c module_inst
 * \snippet qs_usart_dma_use.c transfer_done_flag
 * \snippet qs_usart_dma_use.c usart_string
 * \snippet qs_usart_dma_use.c transfer_descriptor
 *
 * Copy-paste the following setup code to your user application:
 * \snippet qs_usart_dma_use.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_usart_dma_use.c setup_init
 *
 * \subsection asfdoc_sam0_usart_dma_use_case_setup_flow Workflow
 * -# Create a module software instance structure for the USART module to store
 *    the USART driver state while it is in use.
 *    \note This should never go out of scope as long as the module is in use.
 *          In most cases, this should be global.
 *
 *    \snippet qs_usart_dma_use.c module_inst
 * -# Configure the USART module.
 * -# Create a module software instance structure for DMA resource to store
 *    the DMA resource state while it is in use.
 *    \note This should never go out of scope as long as the module is in use.
 *          In most cases, this should be global.
 *
 *    \snippet qs_tc_dma.c dma_resource
 * -# Create a transfer done flag to indication DMA transfer done
 *    \snippet qs_tc_dma.c transfer_done_flag
 * -# Create an array to store the data to be transferred
 *    \snippet qs_tc_dma.c string
 *  -# Create a USART module configuration struct, which can be filled out to
 *     adjust the configuration of a physical USART peripheral.
 *     \snippet qs_usart_dma_use.c setup_config
 *  -# Initialize the USART configuration struct with the module's default values.
 *     \note This should always be performed before using the configuration
 *           struct to ensure that all values are initialized to known default
 *           settings.
 *
 *     \snippet qs_usart_dma_use.c setup_config_defaults
 *  -# Alter the USART settings to configure the physical pinout, baud rate and
 *     other relevant parameters.
 *     \snippet qs_usart_dma_use.c setup_change_config
 *  -# Configure the USART module with the desired settings, retrying while the
 *     driver is busy until the configuration is stressfully set.
 *     \snippet qs_usart_dma_use.c setup_set_config
 *  -# Enable the USART module.
 *     \snippet qs_usart_dma_use.c setup_enable
 *
 * -# Create a DMA resource configuration structure, which can be filled out to
 *    adjust the configuration of a single DMA transfer.
 *  \snippet qs_dma_basic.c dma_setup_1
 *
 * -# Initialize the DMA resource configuration struct with the module's
 *    default values.
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 *  \snippet qs_dma_basic.c dma_setup_2
 *
 * -# Set extra configurations for the DMA resource. It is using peripheral trigger,
 * SERCOM Tx empty trigger and trigger causes a beat transfer in this example.
 *  \snippet qs_dma_basic.c dma_setup_3
 *
 * -# Allocate a DMA resource with the configurations.
 *  \snippet qs_dma_basic.c dma_setup_4
 *
 * -# Create a DMA transfer descriptor configuration structure, which can be
 * filled out to adjust the configuration of a single DMA transfer.
 *  \snippet qs_dma_basic.c dma_setup_5
 *
 * -# Initialize the DMA transfer descriptor configuration struct with the module's
 *    default values.
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 *  \snippet qs_dma_basic.c dma_setup_6
 *
 * -# Set the specific parameters for a DMA transfer with transfer size, source
 *    address, destination address.
 *  \snippet qs_dma_basic.c dma_setup_7
 *
 * -# Create the DMA transfer descriptor.
 *  \snippet qs_dma_basic.c dma_setup_8
 *
 * \section asfdoc_sam0_usart_dma_use_case_main Use Case
 *
 * \subsection asfdoc_sam0_usart_dma_use_case_main_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_usart_dma_use.c main
 *
 * \subsection asfdoc_sam0_usart_dma_use_case_main_flow Workflow
 * -# Start the transfer job.
 *  \snippet qs_tc_dma.c main_1
 *
 * -# Wait for transfer done.
 *  \snippet qs_tc_dma.c main_2
 *
 * -# enter endless loop
 *  \snippet qs_tc_dma.c endless_loop
 */
