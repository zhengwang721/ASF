/**
 *
 * \file
 *
 * \brief AESA software driver for SAM.
 *
 * This file defines a useful set of functions for the AESA on SAM devices.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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
 * \internal
 * \brief AESA callback function pointer
 */
aesa_callback_t aesa_callback_pointer;

/**
 * \brief Initializes a AESA configuration structure to defaults.
 *
 *  Initializes a given AESA configuration structure to a set of
 *  known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *  - Encryption.
 *  - 128 key size.
 *  - Manual mode.
 *  - ECB mode.
 *  - Countermeasure.
 *
 *  \param cfg    Configuration structure to initialize to default values.
 */
void aesa_get_config_defaults(struct aesa_config *const cfg)
{
	/* Sanity check arguments */
	Assert(cfg);

	/* Default configuration values */
	cfg->encrypt_mode = AESA_ENCRYPTION;
	cfg->key_size = AESA_KEY_SIZE_128;
	cfg->dma_mode = AESA_MANUAL_MODE;
	cfg->opmode = AESA_ECB_MODE;
	cfg->cfb_size = AESA_CFB_SIZE_128;
	cfg->countermeasure_mask = 0xF;
}

/**
 * \brief Initialize the AESA module.
 *
 * \param dev_inst    Device structure pointer.
 * \param aesa         Base address of the AESA instance.
 * \param cfg         Pointer to AESA configuration.
 *
 * \retval true if the initialization was successful.
 * \retval false if initialization failed.
 */
bool aesa_init(struct aesa_dev_inst *const dev_inst, Aesa *const aesa,
		struct aesa_config *const cfg)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(aesa);
	Assert(cfg);

	dev_inst->hw_dev = aesa;
	dev_inst->aesa_cfg = cfg;

	/* Enable APB clock for AESA */
	sysclk_enable_peripheral_clock(aesa);

	/* Initialize the AESA with new configurations */
	aesa_set_config(dev_inst);

	/* Disable APB clock for AESA */
	sysclk_disable_peripheral_clock(aesa);

	return true;
}

/**
 * \brief Enable the AESA.
 *
 * \param dev_inst Device structure pointer.
 *
 */
void aesa_enable(struct aesa_dev_inst *const dev_inst)
{
	struct genclk_config gencfg;

	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
	sleepmgr_lock_mode(SLEEPMGR_SLEEP_0);
	dev_inst->hw_dev->AESA_CTRL = AESA_CTRL_ENABLE;
	genclk_config_defaults(&gencfg, AESA_GCLK_NUM);
	genclk_enable_source(CONFIG_AESA_GENERIC_SRC);
	genclk_config_set_source(&gencfg, CONFIG_AESA_GENERIC_SRC);
	genclk_config_set_divider(&gencfg, CONFIG_AESA_GENERIC_DIV);
	genclk_enable(&gencfg, AESA_GCLK_NUM);
}

/**
 * \brief Disable the AESA.
 *
 * \param dev_inst Device structure pointer.
 *
 */
void aesa_disable(struct aesa_dev_inst *const dev_inst)
{
	dev_inst->hw_dev->AESA_CTRL &= ~AESA_CTRL_ENABLE;
	sysclk_disable_peripheral_clock(dev_inst->hw_dev);
	sleepmgr_unlock_mode(SLEEPMGR_SLEEP_0);
}

/**
 * \brief Write the 128/192/256bit cryptographic key.
 *
 * \param  dev_inst Device structure pointer.
 * \param  *p_key Pointer on 4/6/8 contiguous 32bit words.
 *
 * \note The key size depends on the AES configuration.
 *
 */
void aesa_write_key(struct aesa_dev_inst *const dev_inst,
		const uint32_t *p_key)
{
	uint32_t i, key_length = 0;

	switch ((dev_inst->hw_dev->AESA_MODE & AESA_MODE_KEYSIZE_Msk) >>
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
		dev_inst->hw_dev->AESA_KEY[i].AESA_KEY = *p_key;
		p_key++;
	}
}

/**
 * \brief Write the initialization vector (for the CBC, CFB, OFB & CTR
 * cipher modes)
 *
 * \param  dev_inst Device structure pointer.
 * \param  *p_vector Pointer on 4 contiguous 32bit words.
 *
 */
void aesa_write_initvector(struct aesa_dev_inst *const dev_inst,
		const uint32_t *p_vector)
{
	uint32_t i;

	for (i = 0; i < 4; i++) {
		dev_inst->hw_dev->AESA_INITVECT[i].AESA_INITVECT = *p_vector;
		p_vector++;
	}
}

/**
 * \brief Set callback for AESA
 *
 * \param dev_inst Device structure pointer.
 * \param source Interrupt source.
 * \param callback callback function pointer.
 * \param irq_level interrupt level.
 */
void aesa_set_callback(struct aesa_dev_inst *const dev_inst,
		aesa_interrupt_source_t source, aesa_callback_t callback,
		uint8_t irq_level)
{
	aesa_callback_pointer = callback;
	irq_register_handler((IRQn_Type)AESA_IRQn, irq_level);
	aesa_enable_interrupt(dev_inst, source);
}

/**
 * \brief The AESA interrupt handler.
 */
void AESA_Handler(void)
{
	if (aesa_callback_pointer) {
		aesa_callback_pointer();
	} else {
		Assert(false); /* Catch unexpected interrupt */
	}
}

