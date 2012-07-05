/**
 * \file
 *
 * \brief Configuration for the ADS7843 component.
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
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

//! Configuration of the ADS7843 component

#ifndef CONF_ADS7843_H_INCLUDED
#define CONF_ADS7843_H_INCLUDED

#include "compiler.h"
#include "board.h"

/*! \brief Interface configuration
 *
 * Currently only EBI mode supported
 */
#if BOARD == SAM3S_EK

	/*! \brief pointer to the SPI instance connected to the ADS7843 controller
	 */
	#define ADS7843_SPI_INTERFACE     (SPI)

#elif BOARD == SAM3S_EK2

	/*! \brief pointer to the SPI instance connected to the ADS7843 controller
	 */
	#define ADS7843_SPI_INTERFACE     (SPI)
	
#elif BOARD == SAM3U_EK

	/*! \brief pointer to the SPI instance connected to the ADS7843 controller
	 */
	#define ADS7843_SPI_INTERFACE     (SPI)
	
#elif BOARD == SAM3X_EK

	/*! \brief pointer to the SPI instance connected to the ADS7843 controller
	 */
	#define ADS7843_SPI_INTERFACE     (SPI0)

#else

	#warning The ADS7843 setup configuration to use in the driver is missing. Default configuration is used.

	#define ADS7843_SPI_INTERFACE     0

#endif

#endif /* CONF_ADS7843_H_INCLUDED */
