/**
 * \file
 *
 * \brief XDMAC (XDMA Controller) driver example for SAM.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#ifndef XDMAC_EXAMPLE_H_INCLUDED
#define XDMAC_EXAMPLE_H_INCLUDED

/**
 * \page asfdoc_sam_drivers_xdmac_example SAM Extensible Direct Memory Access Controller - Example memory to memory transfer
 *
 * \section asfdoc_sam_drivers_xdmac_example_purpose Purpose
 * This example demonstrates how to configure and use XDMA controller
 * for memory to memory data buffer transfers.
 *
 * \section asfdoc_sam_drivers_xdmac_example_requirements Requirements
 * This example can be used with SAM evaluation kits that have a XDMA
 * Controller.
 *
 * \section asfdoc_sam_drivers_xdmac_example_description Description
 * This example transfers data in one RAM buffer to another RAM buffer.
 * It uses single block with multiple microblock transfer with interrupt mode.
 * After transfer is done, the transferred data will be verified.
 *
 * \section asfdoc_sam_drivers_xdmac_example_files Main Files
 * - xdmac.c: Direct Memory Access Controller driver
 * - xdmac.h: Direct Memory Access Controller driver header file
 * - xdmac_example.c: Direct Memory Access Controller example application
 *
 * \section asfdoc_sam_drivers_xdmac_example_compilinfo Compilation Information
 * This software is written for GNU GCC and IAR Embedded Workbench&reg;
 * for Atmel&reg;. Other compilers may or may not work.
 *
 * \section asfdoc_sam_drivers_xdmac_example_usage Usage
 * -# Build the program and download it into the evaluation board.
 * -# On the computer, open and configure a terminal application
 *    (e.g., HyperTerminal on Microsoft&reg; Windows&reg;) with these settings:
 *   - 115200 baud
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear.
 *    (values depend on the board and the chip used):
 *    \code
 *     -- XDMAC Example --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *
 *     > Test OK. \endcode
 */
 
 #endif /* XDMAC_EXAMPLE_H_INCLUDED */
