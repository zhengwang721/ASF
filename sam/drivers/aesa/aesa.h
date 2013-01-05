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

#ifndef _AESA_H_INCLUDED
#define _AESA_H_INCLUDED

/**
 * \defgroup group_sam_drivers_aesa AESA - Advanced Encryption Standard module
 *
 * Driver for the AESA (Advanced Encryption Standard) module.
 * Provides functions for configuring and initiating ciphering/deciphering
 * with AES algorithms.
 * Several modes(ECB, CBC, OFB, CFB or CTR) and
 * key sizes(128-, 192- or 256-bit) are supported,
 * in addition to manual or DMA-based data input to the AESA module.
 *
 * \{
 */

#include "compiler.h"


/** AESA Processing mode */
#define AESA_DECRYPTION 0
#define AESA_ENCRYPTION 1

/** AESA Cryptographic key size */
#define AESA_KEY_SIZE_128 0
#define AESA_KEY_SIZE_192 1
#define AESA_KEY_SIZE_256 2

/** AESA DMA mode */
#define AESA_MANUAL_MODE 0
#define AESA_DMA_MODE 1

/** AESA Confidentiality mode */
#define AESA_ECB_MODE 0
#define AESA_CBC_MODE 1
#define AESA_OFB_MODE 2
#define AESA_CFB_MODE 3
#define AESA_CTR_MODE 4

/** AESA CFB size */
#define AESA_CFB_SIZE_128 0
#define AESA_CFB_SIZE_64 1
#define AESA_CFB_SIZE_32 2
#define AESA_CFB_SIZE_16 3
#define AESA_CFB_SIZE_8 4

/** AESA Configuration structure. */
typedef struct {
	/* 0=decryption data, 1=encryption data */
	uint8_t encrypt_mode;
	/* 0 = 128bits, 1 = 192bits, 2 = 256bits */
	uint8_t key_size;
	/* 0=Non-DMA mode, 1=DMA mode */
	uint8_t dma_mode;
	/* 0 = ECB, 1 = CBC, 2 = OFB, 3 = CFB, 4 = CTR */
	uint8_t opmode;
	/* 0 = 128bits, 1 = 64bits, 2 = 32bits, 3 = 16bits, 4 = 8bits */
	uint8_t cfb_size;
	/* [0,15], bit=0 means CounterMeasure is disabled. */
	uint8_t countermeasure_mask;
} aesa_config_t;

typedef enum aesa_interrupt_source {
	AESA_INTERRUPT_INPUT_BUFFER_READY = AESA_IER_ODATARDY,
	AESA_INTERRUPT_OUTPUT_DATA_READY = AESA_IER_IBUFRDY,
} aesa_interrupt_source_t;

/**
 * \brief Perform a software reset of the AESA.
 *
 * \param *aesa Base address of the AESA.
 *
 */
static inline void aesa_reset(Aesa *aesa)
{
	aesa->AESA_CTRL |= AESA_CTRL_SWRST;
}

/**
 * \brief Notifies the module that the next input data block
 * is the beginning of a new message.
 *
 * \param *aesa Base address of the AESA.
 *
 */
static inline void aesa_set_new_message(Aesa *aesa)
{
	aesa->AESA_CTRL |= AESA_CTRL_NEWMSG;
}

/**
 * \brief Starts the computation of the last Nk words of the expanded key.
 *
 * \param *aesa Base address of the AESA.
 *
 */
static inline void aesa_decryption_key_generate(Aesa *aesa)
{
	aesa->AESA_CTRL |= AESA_CTRL_DKEYGEN;
}

void aesa_enable(Aesa *aesa);

void aesa_disable(Aesa *aesa);

/**
 * \brief Configure the AESA.
 *
 * \param  *aesa Base address of the AESA.
 * \param  *p_aesa_config  Parameters for the AESA configuration.
 *
 * \note See the aes_config_t structure definition for the meaning of
 * the parameters.
 *
 */
static inline void aesa_set_config(Aesa *aesa,
		const aesa_config_t *p_aesa_config)
{
	aesa->AESA_MODE = p_aesa_config->encrypt_mode |
			AESA_MODE_KEYSIZE(p_aesa_config->key_size) |
			(p_aesa_config->dma_mode ? AESA_MODE_DMA : 0) |
			AESA_MODE_OPMODE(p_aesa_config->opmode) |
			AESA_MODE_CFBS(p_aesa_config->cfb_size) |
			AESA_MODE_CTYPE(p_aesa_config->countermeasure_mask);
}

/**
 * \brief Write the input buffer pointer position.
 *
 * \param  *aesa Base address of the AESA
 * \param  ul_in_position Input buffer pointer position.
 *
 */
static inline void aesa_write_input_buffer_pointer(Aesa *aesa,
		uint32_t ul_in_position)
{
	aesa->AESA_DATABUFPTR |= AESA_DATABUFPTR_IDATAW(ul_in_position);
}

/**
 * \brief Write the output buffer pointer position.
 *
 * \param  *aesa Base address of the AESA
 * \param  ul_out_position Output buffer pointer position.
 *
 */
static inline void aesa_write_output_buffer_pointer(Aesa *aesa,
		uint32_t ul_out_position)
{
	aesa->AESA_DATABUFPTR |= AESA_DATABUFPTR_ODATAW(ul_out_position);
}

/**
 * \brief Read the input buffer pointer position.
 *
 * \param  *aesa Base address of the AESA
 *
 * \return the input buffer pointer position.
 *
 */
static inline uint32_t aesa_read_input_buffer_pointer(Aesa *aesa)
{
	return ((aesa->AESA_DATABUFPTR & AESA_DATABUFPTR_IDATAW_Msk) >>
			AESA_DATABUFPTR_IDATAW_Pos);
}

/**
 * \brief Read the output buffer pointer position.
 *
 * \param  *aesa Base address of the AESA
 *
 * \return the output buffer pointer position.
 *
 */
static inline uint32_t aesa_read_output_buffer_pointer(Aesa *aesa)
{
	return ((aesa->AESA_DATABUFPTR & AESA_DATABUFPTR_ODATAW_Msk) >>
			AESA_DATABUFPTR_ODATAW_Pos);
}

/**
 * \brief Get the AESA status.
 *
 * \param  *aesa Base address of the AESA.
 *
 * \return the content of the status register.
 *
 */
static inline uint32_t aesa_read_status(Aesa *aesa)
{
	return aesa->AESA_SR;
}

/**
 * \brief Enable the AESA interrupt.
 *
 * \param  *aesa Base address of the AESA.
 * \param  source Interrupt source.
 *
 */
static inline void aesa_enable_interrupt(Aesa *aesa,
		aesa_interrupt_source_t source)
{
	aesa->AESA_IER = (uint32_t)source;
}

/**
 * \brief Disable the AESA interrupt.
 *
 * \param  *aesa Base address of the AESA.
 * \param  source Interrupt source.
 *
 */
static inline void aesa_disable_interrupt(Aesa *aesa,
		aesa_interrupt_source_t source)
{
	aesa->AESA_IDR = (uint32_t)source;
}

/**
 * \brief Get the AESA interrupt mask status.
 *
 * \param  *aesa Base address of the AESA.
 *
 * \return the content of the interrupt mask register.
 *
 */
static inline uint32_t aesa_read_interrupt_mask(Aesa *aesa)
{
	return aesa->AESA_IMR;
}

void aesa_write_key(Aesa *aesa, const uint32_t *p_key);

void aesa_write_initvector(Aesa *aesa, const uint32_t *p_vector);

/**
 * \brief Write the input data.
 *
 * \param  *aesa Base address of the AESA.
 * \param  ul_data Input data.
 *
 */
static inline void aesa_write_input_data(Aesa *aesa, uint32_t ul_data)
{
	aesa->AESA_IDATA = ul_data;
}

/**
 * \brief Read the output data.
 *
 * \param  *aesa Base address of the AESA.
 *
 * \return  the output data.
 *
 */
static inline uint32_t aesa_read_output_data(Aesa *aesa)
{
	return aesa->AESA_ODATA;
}

/**
 * \brief Write the DRNG seed.
 *
 * \param  *aesa Base address of the AESA.
 * \param  ul_drng_seed DRNG seed.
 *
 */
static inline void aesa_write_drng_seed(Aesa *aesa, uint32_t ul_drng_seed)
{
	aesa->AESA_DRNGSEED = ul_drng_seed;
}

/**
 * \}
 */

/**
 * \page sam_aesa_quick_start Quick Start Guide for the AESA driver
 *
 * This is the quick start guide for the \ref group_sam_drivers_aesa, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case.The code examples can be copied into e.g the main
 * application loop or any other function that will need to control the
 * AESA module.
 *
 * \section aesa_qs_use_cases Use cases
 * - \ref aesa_basic
 *
 * \section aesa_basic AESA basic usage
 *
 * This use case will demonstrate how to initialize the AESA module to
 * encryption or decryption data.
 *
 *
 * \section aesa_basic_setup Setup steps
 *
 * \subsection aesa_basic_prereq Prerequisites
 *
 * This module requires the following service
 * - \ref clk_group
 *
 * \subsection aesa_basic_setup_code
 *
 * Add this to the main loop or a setup function:
 * \code
 * aesa_enable(AESA);
 * \endcode
 *
 * \subsection aesa_basic_setup_workflow
 *
 * -# Enable the AESA module
 *  - \code aesa_enable(AESA); \endcode
 * -# Initialize the AESA to ECB cipher mode
 *  - \code
 * aesa_config_t aesa_config;
 * aesa_config.encrypt_mode = AESA_ENCRYPTION;
 * aesa_config.key_size = AESA_KEY_SIZE_128;
 * aesa_config.dma_mode = AESA_MANUAL_MODE;
 * aesa_config.opmode = AESA_ECB_MODE;
 * aesa_config.cfb_size = AESA_CFB_SIZE_128;
 * aesa_config.countermeasure_mask = 0xF;
 * aesa_set_config(AESA, &aesa_config);
 * \endcode
 *
 * \section aesa_basic_usage Usage steps
 *
 * \subsection aesa_basic_usage_code
 *
 * We can then encrypte the plain text by
 * \code
 * aesa_set_new_message(AESA);
 * aesa_write_key(AESA, key128);
 * aesa_write_input_data(AESA, ref_plain_text[0]);
 * aesa_write_input_data(AESA, ref_plain_text[1]);
 * aesa_write_input_data(AESA, ref_plain_text[2]);
 * aesa_write_input_data(AESA, ref_plain_text[3]);
 * \endcode
 *
 * We can get the cipher text after it's ready by
 * \code
 * output_data[0] = aesa_read_output_data(AESA);
 * output_data[1] = aesa_read_output_data(AESA);
 * output_data[2] = aesa_read_output_data(AESA);
 * output_data[3] = aesa_read_output_data(AESA);
 * \endcode
 */

#endif  /* _AESA_H_INCLUDED */
