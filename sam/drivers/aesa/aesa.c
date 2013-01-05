/**
 *
 * \file
 *
 * \brief AESA software driver for SAM.
 *
 * This file defines a useful set of functions for the AESA on SAM devices.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#include "aesa.h"
#include "sysclk.h"
#include "sleepmgr.h"
#include "conf_aesa.h"

/**
 * \brief Enable the AESA.
 *
 * \param *aesa Base address of the AESA.
 *
 */
void aesa_enable(Aesa *aesa)
{
	struct genclk_config gencfg;

	sysclk_enable_peripheral_clock(aesa);
	sleepmgr_lock_mode(SLEEPMGR_SLEEP_0);
	aesa->AESA_CTRL = AESA_CTRL_ENABLE;
	genclk_config_defaults(&gencfg, AESA_GCLK_NUM);
	genclk_enable_source(CONFIG_GENERIC_SRC);
	genclk_config_set_source(&gencfg, CONFIG_GENERIC_SRC);
	genclk_config_set_divider(&gencfg, CONFIG_GENERIC_DIV);
	genclk_enable(&gencfg, AESA_GCLK_NUM);
}

/**
 * \brief Disable the AESA.
 *
 * \param *aesa Base address of the AESA.
 *
 */
void aesa_disable(Aesa *aesa)
{
	aesa->AESA_CTRL &= ~AESA_CTRL_ENABLE;
	sysclk_disable_peripheral_clock(aesa);
	sleepmgr_unlock_mode(SLEEPMGR_SLEEP_0);
}

/**
 * \brief Write the 128/192/256bit cryptographic key.
 *
 * \param  *aesa Base address of the AESA
 * \param  *p_key Pointer on 4/6/8 contiguous 32bit words.
 *
 * \note The key size depends on the AES configuration.
 *
 */
void aesa_write_key(Aesa *aesa, const uint32_t *p_key)
{
	uint32_t i, key_length = 0;

	switch ((aesa->AESA_MODE & AESA_MODE_KEYSIZE_Msk) >>
			AESA_MODE_KEYSIZE_Pos) {
	case 0: /* 128bit cryptographic key */
		key_length = 4;
		break;

	case 1: /* 192bit cryptographic key */
		key_length = 6;
		break;

	case 2: /* 256bit cryptographic key */
		key_length = 8;
		break;

	default:
		break;
	}

	for (i = 0; i < key_length; i++) {
		aesa->AESA_KEY[i].AESA_KEY = *p_key;
		p_key++;
	}
}

/**
 * \brief Write the initialization vector (for the CBC, CFB, OFB & CTR
 * cipher modes)
 *
 * \param  *aesa Base address of the AESA
 * \param  *p_vector Pointer on 4 contiguous 32bit words.
 *
 */
void aesa_write_initvector(Aesa *aesa, const uint32_t *p_vector)
{
	uint32_t i;

	for (i = 0; i < 4; i++) {
		aesa->AESA_INITVECT[i].AESA_INITVECT = *p_vector;
		p_vector++;
	}
}

