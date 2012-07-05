/**
 * \file
 *
 * \brief ILI9225 configuration.
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
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

//! Configuration of the ILI9225 LCD display driver

#ifndef CONF_ILI9225_H_INCLUDED
#define CONF_ILI9225_H_INCLUDED

#include "compiler.h"
#include "board.h"

/*! \brief Interface configuration
 *
 * Currently only EBI mode supported
 */
#if BOARD == SAM3N_EK

	/*! \brief reset pin
	 */
	#define ILI9225_SPI_INTERFACE     (SPI)

	/*! \brief address used to read/write from/to registers or RAM
	 */
	#define ILI9225_SPI_IRQ_NUM       (SPI_IRQn)
	
	/*! \brief reset pin
	 */
	#define ILI9225_LCD_RSTN          (PIN_LCD_RSTN_GPIO)

	/*! \brief address used to read/write from/to registers or RAM
	 */
	#define ILI9225_LCD_RS            (PIN_LCD_RS_GPIO)
	
	
	

#else

	#warning The ILI9225 setup configuration to use in the driver is missing. Default configuration is used.

	#define ILI9225_SPI_INTERFACE     0
	#define ILI9225_SPI_IRQ_NUM       0
	#define ILI9225_LCD_RSTN          0
	#define ILI9225_LCD_RS            0

#endif

#endif /* CONF_ILI9225_H_INCLUDED */
