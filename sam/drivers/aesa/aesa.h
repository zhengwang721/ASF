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

#ifndef AESA_H_INCLUDED
#define AESA_H_INCLUDED

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
enum aesa_encrypt_mode {
	AESA_DECRYPTION = 0,
	AESA_ENCRYPTION,
};
/** AESA Cryptographic key size */
enum aesa_key_size {
	AESA_KEY_SIZE_128 = 0,
	AESA_KEY_SIZE_192,
	AESA_KEY_SIZE_256,
};

/** AESA DMA mode */
enum aesa_dma_mode {
	AESA_MANUAL_MODE = 0,
	AESA_DMA_MODE,
};

/** AESA Confidentiality mode */
enum aesa_opmode {
	AESA_ECB_MODE = 0,
	AESA_CBC_MODE,
	AESA_OFB_MODE,
	AESA_CFB_MODE,
	AESA_CTR_MODE,
};

/** AESA CFB size */
enum aesa_cfb_size {
	AESA_CFB_SIZE_128 = 0,
	AESA_CFB_SIZE_64,
	AESA_CFB_SIZE_32,
	AESA_CFB_SIZE_16,
	AESA_CFB_SIZE_8,
};

/** AESA CounterMeasure type */
enum aesa_countermeature_type {
	AESA_COUNTERMEASURE_TYPE_1 = 0x01,
	AESA_COUNTERMEASURE_TYPE_2 = 0x02,
	AESA_COUNTERMEASURE_TYPE_3 = 0x04,
	AESA_COUNTERMEASURE_TYPE_4 = 0x08,
	AESA_COUNTERMEASURE_TYPE_ALL = 0x0F,
};

typedef enum aesa_interrupt_source {
	AESA_INTERRUPT_INPUT_BUFFER_READY = AESA_IER_ODATARDY,
	AESA_INTERRUPT_OUTPUT_DATA_READY = AESA_IER_IBUFRDY,
} aesa_interrupt_source_t;

/**
 * \brief Interrupt callback function type for AESA.
 *
 * The interrupt handler can be configured to do a function callback,
 * the callback function must match the aesa_callback_t type.
 *
 */
typedef void (*aesa_callback_t)(void);

/** AESA Configuration structure. */
struct aesa_config {
	/* 0=decryption data, 1=encryption data */
	enum aesa_encrypt_mode encrypt_mode;
	/* 0 = 128bits, 1 = 192bits, 2 = 256bits */
	enum aesa_key_size key_size;
	/* 0=Non-DMA mode, 1=DMA mode */
	enum aesa_dma_mode dma_mode;
	/* 0 = ECB, 1 = CBC, 2 = OFB, 3 = CFB, 4 = CTR */
	enum aesa_opmode opmode;
	/* 0 = 128bits, 1 = 64bits, 2 = 32bits, 3 = 16bits, 4 = 8bits */
	enum aesa_cfb_size cfb_size;
	/* [0,15], bit=0 means CounterMeasure is disabled. */
	enum aesa_countermeature_type countermeasure_mask;
};

/**
 * \brief AESA driver software instance structure.
 *
 * Device instance structure for a AESA driver instance. This
 * structure should be initialized by the \ref aesa_init() function to
 * associate the instance with a particular hardware module of the device.
 */
struct aesa_dev_inst {
	/** Base address of the AESA module. */
	Aesa *hw_dev;
	/** Pointer to AESA configuration structure. */
	struct aesa_config  *aesa_cfg;
};

void aesa_get_config_defaults(struct aesa_config *const cfg);

bool aesa_init(struct aesa_dev_inst *const dev_inst, Aesa *const aesa,
		struct aesa_config *const cfg);

/**
 * \brief Perform a software reset of the AESA.
 *
 * \param dev_inst Device structure pointer.
 *
 */
static inline void aesa_reset(struct aesa_dev_inst *const dev_inst)
{
	dev_inst->hw_dev->AESA_CTRL |= AESA_CTRL_SWRST;
}

/**
 * \brief Notifies the module that the next input data block
 * is the beginning of a new message.
 *
 * \param dev_inst Device structure pointer.
 *
 */
static inline void aesa_set_new_message(struct aesa_dev_inst *const dev_inst)
{
	dev_inst->hw_dev->AESA_CTRL |= AESA_CTRL_NEWMSG;
}

/**
 * \brief Starts the computation of the last Nk words of the expanded key.
 *
 * \param dev_inst Device structure pointer.
 *
 */
static inline void aesa_decryption_key_generate(
		struct aesa_dev_inst *const dev_inst)
{
	dev_inst->hw_dev->AESA_CTRL |= AESA_CTRL_DKEYGEN;
}

void aesa_enable(struct aesa_dev_inst *const dev_inst);

void aesa_disable(struct aesa_dev_inst *const dev_inst);

/**
 * \brief Configure the AESA.
 *
 * \param  dev_inst Device structure pointer.
 *
 */
static inline void aesa_set_config(struct aesa_dev_inst *const dev_inst)
{
	dev_inst->hw_dev->AESA_MODE = dev_inst->aesa_cfg->encrypt_mode |
			AESA_MODE_KEYSIZE(dev_inst->aesa_cfg->key_size) |
			(dev_inst->aesa_cfg->dma_mode ? AESA_MODE_DMA : 0) |
			AESA_MODE_OPMODE(dev_inst->aesa_cfg->opmode) |
			AESA_MODE_CFBS(dev_inst->aesa_cfg->cfb_size) |
			AESA_MODE_CTYPE(dev_inst->aesa_cfg->countermeasure_mask);
}

/**
 * \brief Write the input buffer pointer position.
 *
 * \param  dev_inst Device structure pointer.
 * \param  ul_in_position Input buffer pointer position.
 *
 */
static inline void aesa_write_input_buffer_pointer(
		struct aesa_dev_inst *const dev_inst, uint32_t ul_in_position)
{
	dev_inst->hw_dev->AESA_DATABUFPTR |=
			AESA_DATABUFPTR_IDATAW(ul_in_position);
}

/**
 * \brief Write the output buffer pointer position.
 *
 * \param  dev_inst Device structure pointer.
 * \param  ul_out_position Output buffer pointer position.
 *
 */
static inline void aesa_write_output_buffer_pointer(
		struct aesa_dev_inst *const dev_inst, uint32_t ul_out_position)
{
	dev_inst->hw_dev->AESA_DATABUFPTR |=
			AESA_DATABUFPTR_ODATAW(ul_out_position);
}

/**
 * \brief Read the input buffer pointer position.
 *
 * \param  dev_inst Device structure pointer.
 *
 * \return the input buffer pointer position.
 *
 */
static inline uint32_t aesa_read_input_buffer_pointer(
		struct aesa_dev_inst *const dev_inst)
{
	return ((dev_inst->hw_dev->AESA_DATABUFPTR & AESA_DATABUFPTR_IDATAW_Msk)
			>> AESA_DATABUFPTR_IDATAW_Pos);
}

/**
 * \brief Read the output buffer pointer position.
 *
 * \param  dev_inst Device structure pointer.
 *
 * \return the output buffer pointer position.
 *
 */
static inline uint32_t aesa_read_output_buffer_pointer(
		struct aesa_dev_inst *const dev_inst)
{
	return ((dev_inst->hw_dev->AESA_DATABUFPTR & AESA_DATABUFPTR_ODATAW_Msk)
			>> AESA_DATABUFPTR_ODATAW_Pos);
}

/**
 * \brief Get the AESA status.
 *
 * \param  dev_inst Device structure pointer.
 *
 * \return the content of the status register.
 *
 */
static inline uint32_t aesa_read_status(struct aesa_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->AESA_SR;
}

void aesa_set_callback(struct aesa_dev_inst *const dev_inst,
		aesa_interrupt_source_t source, aesa_callback_t callback,
		uint8_t irq_level);

/**
 * \brief Enable the AESA interrupt.
 *
 * \param  dev_inst Device structure pointer.
 * \param  source Interrupt source.
 *
 */
static inline void aesa_enable_interrupt(struct aesa_dev_inst *const dev_inst,
		aesa_interrupt_source_t source)
{
	dev_inst->hw_dev->AESA_IER = (uint32_t)source;
}

/**
 * \brief Disable the AESA interrupt.
 *
 * \param  dev_inst Device structure pointer.
 * \param  source Interrupt source.
 *
 */
static inline void aesa_disable_interrupt(
		struct aesa_dev_inst *const dev_inst, aesa_interrupt_source_t source)
{
	dev_inst->hw_dev->AESA_IDR = (uint32_t)source;
}

/**
 * \brief Get the AESA interrupt mask status.
 *
 * \param  dev_inst Device structure pointer.
 *
 * \return the content of the interrupt mask register.
 *
 */
static inline uint32_t aesa_read_interrupt_mask(
		struct aesa_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->AESA_IMR;
}

void aesa_write_key(struct aesa_dev_inst *const dev_inst,
		const uint32_t *p_key);

void aesa_write_initvector(struct aesa_dev_inst *const dev_inst,
		const uint32_t *p_vector);

/**
 * \brief Write the input data.
 *
 * \param  dev_inst Device structure pointer.
 * \param  ul_data Input data.
 *
 */
static inline void aesa_write_input_data(
		struct aesa_dev_inst *const dev_inst, uint32_t ul_data)
{
	dev_inst->hw_dev->AESA_IDATA = ul_data;
}

/**
 * \brief Read the output data.
 *
 * \param  dev_inst Device structure pointer.
 *
 * \return  the output data.
 *
 */
static inline uint32_t aesa_read_output_data(
		struct aesa_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->AESA_ODATA;
}

/**
 * \brief Write the DRNG seed.
 *
 * \param  dev_inst Device structure pointer.
 * \param  ul_drng_seed DRNG seed.
 *
 */
static inline void aesa_write_drng_seed(struct aesa_dev_inst *const dev_inst,
		uint32_t ul_drng_seed)
{
	dev_inst->hw_dev->AESA_DRNGSEED = ul_drng_seed;
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
 * struct aesa_dev_inst g_aesa_inst;
 * struct aesa_config   g_aesa_cfg;
 * aesa_get_config_defaults(&g_aesa_cfg);
 * aesa_init(&g_aesa_inst, AESA, &g_aesa_cfg);
 * aesa_enable(&g_aesa_inst);
 * \endcode
 *
 * \subsection aesa_basic_setup_workflow
 *
 * -# Enable the AESA module
 *  - \code aesa_enable(&g_aesa_inst); \endcode
 *
 * -# Set the AESA interrupt and callback
 * \code
 * aesa_set_callback(&g_aesa_inst, AESA_INTERRUPT_INPUT_BUFFER_READY,
 *		aesa_callback, 1);
 * \endcode
 *
 * -# Initialize the AESA to ECB cipher mode
 * \code
 * g_aesa_inst.aesa_cfg->encrypt_mode = AESA_ENCRYPTION;
 * g_aesa_inst.aesa_cfg->key_size = AESA_KEY_SIZE_128;
 * g_aesa_inst.aesa_cfg->dma_mode = AESA_MANUAL_MODE;
 * g_aesa_inst.aesa_cfg->opmode = AESA_ECB_MODE;
 * g_aesa_inst.aesa_cfg->cfb_size = AESA_CFB_SIZE_128;
 * g_aesa_inst.aesa_cfg->countermeasure_mask = 0xF;
 * aesa_set_config(&g_aesa_inst);
 * \endcode
 *
 * \section aesa_basic_usage Usage steps
 *
 * \subsection aesa_basic_usage_code
 *
 * We can then encrypte the plain text by
 * \code
 * aesa_set_new_message(&g_aesa_inst);
 * aesa_write_key(&g_aesa_inst, key128);
 * aesa_write_input_data(&g_aesa_inst, ref_plain_text[0]);
 * aesa_write_input_data(&g_aesa_inst, ref_plain_text[1]);
 * aesa_write_input_data(&g_aesa_inst, ref_plain_text[2]);
 * aesa_write_input_data(&g_aesa_inst, ref_plain_text[3]);
 * \endcode
 *
 * We can get the cipher text after it's ready by
 * \code
 * output_data[0] = aesa_read_output_data(&g_aesa_inst);
 * output_data[1] = aesa_read_output_data(&g_aesa_inst);
 * output_data[2] = aesa_read_output_data(&g_aesa_inst);
 * output_data[3] = aesa_read_output_data(&g_aesa_inst);
 * \endcode
 */

#endif  /* AESA_H_INCLUDED */
