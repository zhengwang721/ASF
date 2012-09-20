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

#include "compiler.h"
#include "bscif.h"
#include "sleepmgr.h"



/*! \brief Sets the mode of the 32-kHz oscillator.
 *
 * \param bscif Base address of the BSCIF
 * \param mode 32-kHz oscillator mode 
 */
void bscif_set_osc32_params(volatile Bscif* bscif, 
					 uint8_t mode, 
					 uint8_t current, 
					 uint8_t startup,
					 bool en_32k,
					 bool en_1k)
{
	uint32_t tmp;
	tmp = BSCIF_OSCCTRL32_SELCURR(current)
			| BSCIF_OSCCTRL32_STARTUP(startup)
			| BSCIF_OSCCTRL32_MODE(mode)
			| BSCIF_OSCCTRL32_EN32K
			| BSCIF_OSCCTRL32_OSC32EN;
	if(en_32k)
			tmp|=BSCIF_OSCCTRL32_EN32K;
	if(en_1k)
			tmp|=BSCIF_OSCCTRL32_EN1K;
	
	// Unlock OSCCTRL32 register
	bscif->BSCIF_UNLOCK = BSCIF_UNLOCK_KEY(0xAAUL)
		| BSCIF_UNLOCK_ADDR((uint32_t)&bscif->BSCIF_OSCCTRL32 - (uint32_t)bscif);
	//Write value
	bscif->BSCIF_OSCCTRL32 = tmp;
}

void bscif_wait_for_osc32_ready(volatile Bscif* bscif)
{
	while((bscif->BSCIF_PCLKSR & BSCIF_PCLKSR_OSC32RDY)== 0);
}

void bscif_wait_for_rc32k_ready(volatile Bscif* bscif)
{
	while((bscif->BSCIF_PCLKSR & BSCIF_PCLKSR_RC32KRDY)== 0);
}

void bscif_enable_osc32_ext_clock(volatile Bscif* bscif)
{
	bscif_set_osc32_params(bscif,
				 BSCIF_OSC32_MODE_EXTERNALCLOCK,
				 BSCIF_OSC32_DEFAULTCURRENT,
				 BSCIF_OSC32_STARTUPTIME_0_S,true,false);
}

void bscif_enable_osc32_crystal_no_wait(volatile Bscif* bscif,
								 bool en_32k,
								 bool en_1k)
{
	bscif_set_osc32_params(bscif,
				 BSCIF_OSC32_MODE_CRYSTAL,
				 BSCIF_OSC32_DEFAULTCURRENT,
				 BSCIF_OSC32_STARTUPTIME_1_1_S,en_32k,en_1k);
}

void bscif_enable_osc32_crystal(volatile Bscif* bscif,
								 bool en_32k,
								 bool en_1k)
{
	bscif_enable_osc32_crystal_no_wait(bscif,en_32k,en_1k);
	bscif_wait_for_osc32_ready(bscif);
}

/*! \brief Sets the mode of the 32-kHz oscillator.
 *
 * \param bscif Base address of the BSCIF
 * \param mode 32-kHz oscillator mode 
 */
static void bscif_set_rc32k_params(volatile Bscif* bscif,uint8_t params) 
{
	// Unlock OSCCTRL32 register
	bscif->BSCIF_UNLOCK = BSCIF_UNLOCK_KEY(0xAAUL)
		| BSCIF_UNLOCK_ADDR((uint32_t)&bscif->BSCIF_OSCCTRL32 - (uint32_t)bscif);
	//Write value
	bscif->BSCIF_RC32KCR = params;
}

void bscif_enable_rc32k_no_wait(volatile Bscif* bscif)
{
	bscif_set_rc32k_params(bscif,BSCIF_RC32KCR_EN|BSCIF_RC32KCR_EN32K);
}

void bscif_enable_rc32k(volatile Bscif* bscif)
{
	bscif_enable_rc32k_no_wait(bscif);
	bscif_wait_for_rc32k_ready(bscif);
}

																	 
																	 
																	 

