/**
 * \file
 *
 * \brief BSCIF driver
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#ifndef _BSCIF_H_
#define _BSCIF_H_

/**
 * \defgroup group_sam_drivers_bscif BSCIF - Backup System Control Interface
 *
 * Driver for the BSCIF (Backup System Control Interface).
 * Provides functions for configuring the Backup Power Event.
 *
 * \{
 */

#include "compiler.h"

#define BSCIF_UNLOCK(reg) do {BSCIF->UNLOCK = BSCIF_UNLOCK_KEY(0xAAUL)|BSCIF_UNLOCK_ADDR((uint32_t)&reg - (uint32_t)BSCIF);); } while(0)
		

//! \name MODE OSC32 Oscillator Mode
//@{
//! External Clock Mode
#define BSCIF_OSC32_MODE_EXTERNALCLOCK  0
//! Crystal Clock Mode
#define BSCIF_OSC32_MODE_CRYSTAL			  1
//@}
	 
//! \name MODE OSC32 recommended current value
//@{
//! OSC32 recommended current value
#define BSCIF_OSC32_DEFAULTCURRENT  		10
//! OSC32 startup time 0s
#define BSCIF_OSC32_STARTUPTIME_0_S		0
//! OSC32 startup time 1.1s
#define BSCIF_OSC32_STARTUPTIME_1_1_S		5
//@}
	 
/*!
 * \brief This function will wait until the Osc32 clock is ready.
 * \param bscif Base address of the BSCIF
 */
extern void bscif_set_osc32_params(volatile Bscif* bscif, 
					 uint8_t mode, 
					 uint8_t current, 
					 uint8_t startup,
					 bool en_32k,
					 bool en_1k);
		 
/*!
 * \brief This function will wait until the Osc32 clock is ready.
 * \param bscif Base address of the BSCIF
 */
extern void bscif_wait_for_osc32_ready(volatile Bscif* bscif);

/*!
 * \brief This function will wait until the RC32 clock is ready.
 * \param bscif Base address of the BSCIF
 */
extern void bscif_wait_for_rc32k_ready(volatile Bscif* bscif);

/*!
 * \brief This function will enable the external clock mode of the 32-kHz oscillator.
 * \param bscif Base address of the BSCIF
 */
extern void bscif_enable_osc32_ext_clock(volatile Bscif* bscif);

/*!
 * \brief This function will enable the oscillator 32 to be used with no startup time.
 * \param bscif Base address of the BSCIF
 */
extern void bscif_enable_osc32_crystal_no_wait(volatile Bscif* bscif,bool en_32k,bool en_1k);

/*!
 * \brief This function will enable the crystal mode of the 32-kHz oscillator.
 * \param bscif Base address of the BSCIF
 */
extern void bscif_enable_osc32_crystal(volatile Bscif* bscif,bool en_32k,bool en_1k);

/*!
 * \brief This function will enable the RC 32-kHz oscillator.
 * \param bscif Base address of the BSCIF
 */
extern void bscif_enable_rc32k(volatile Bscif* bscif);

/*!
 * \brief This function will enable the RC 32-kHz oscillator to be used with no startup time.
 * \param bscif Base address of the BSCIF
 */
extern void bscif_enable_rc32k_no_wait(volatile Bscif* bscif);


#endif /* __BSCIF_H__ */
