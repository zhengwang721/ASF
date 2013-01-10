/**
 *
 * \file
 *
 * \brief IISC driver for SAM.
 *
 * This file defines a useful set of functions for the IISC on SAM devices.
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

#ifndef _IISC_H_INCLUDED_
#define _IISC_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup group_sam_drivers_iisc IISC - Inter-IC Sound Controller
 *
 * Driver for the IISC - Inter-IC Sound Controller module.
 * IISC provides a 5-wire, bidirectional, synchronous, digital audio
 * link with off-chip audio devices: ISDI, ISDO, IWS, ISCK and IMCK pins.
 *
 * \{
 */

#include "compiler.h"
#include "status_codes.h"

#define IISC_RETRY_VALUE   10000

/** Supported number of channels */
enum iisc_channel_number {
	IISC_CHANNEL_NONE = 0,
	IISC_CHANNEL_MONO,
	IISC_CHANNEL_STEREO,
};

/** Supported sample rate */
enum iisc_sample_rate {
	IISC_SAMPLE_RATE_32000 = 0,
	IISC_SAMPLE_RATE_48000,
	IISC_SAMPLE_RATE_96000,
	IISC_SAMPLE_RATE_192000,
};

/** Supported Master Clock to Sample Frequency (fs) Ratio */
enum iisc_fs_rate {
	IISC_FS_RATE_16  = 0,
	IISC_FS_RATE_32  = 1,
	IISC_FS_RATE_48  = 2,
	IISC_FS_RATE_64  = 3,
	IISC_FS_RATE_96  = 5,
	IISC_FS_RATE_128 = 7,
	IISC_FS_RATE_192 = 11,
	IISC_FS_RATE_256 = 15,
	IISC_FS_RATE_384 = 23,
	IISC_FS_RATE_512 = 31,
	IISC_FS_RATE_768 = 47,
	IISC_FS_RATE_1024 = 63,
};

/** Data format of the data register value */
enum iisc_data_format {
	IISC_DATE_32BIT = 0,
	IISC_DATE_24BIT,
	IISC_DATE_20BIT,
	IISC_DATE_18BIT,
	IISC_DATE_16BIT,
	IISC_DATE_16BIT_COMPACT,
	IISC_DATE_8BIT,
	IISC_DATE_8BIT_COMPACT,
};

/** Configuration setting structure */
struct iisc_config {
	/* Number of bits per sample. */
	enum iisc_data_format data_word_format;

	/* Number of bits of the slot: 32, 24, 16 or 8 */
	uint32_t slot_length;

	/* Master Clock to Sample Frequency (fs) Ratio. */
	enum iisc_fs_rate fs_ratio;

	/* Number of channels for Tx (0: no channels, 1 for Mono, 2 for stereo */
	enum iisc_channel_number num_tx_channels;

	/* Number of channels for Rx (0: no channels, 1 for Mono, 2 for stereo */
	enum iisc_channel_number num_rx_channels;

	/* 1 for loopback, 0 for normal. */
	bool loopback;

	/* 1 for master, 0 for slave. */
	bool master;
};

struct iisc_device {
	/* Base Address of the IISC module */
	Iisc *hw_dev;

	/* Pointer to IISC configuration structure. */
	struct iisc_config  *iisc_cfg;
};

/**
 * \brief Initialize and configure the IISC module.
 *
 * \param dev_inst    Device structure pointer.
 * \param iisc         Base address of the IISC instance.
 * \param cfg         Pointer to IISC configuration.
 *
 * \return status
 */
status_code_t iis_init(struct iisc_device *const dev_inst,
		Iisc *iisc, struct iisc_config *const iisc_conf);

/**
 * \brief Resets the IISC module
 *
 *  \param dev_inst    Device structure pointer.
 */
static inline void iis_reset(struct iisc_device *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_SWRST;
}

/**
 *  \brief Enable the IISC module in transmission
 *
 *  \param dev_inst    Device structure pointer.
 */
static inline void iis_enable_transmission(struct iisc_device *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_TXEN;
}

/**
 *  \brief Disable the IISC module in transmission
 *
 *  \param dev_inst    Device structure pointer.
 */
static inline void iis_disable_transmission(struct iisc_device *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_TXDIS;
}

/**
 *  \brief Enable the IISC module in reception
 *
 *  \param dev_inst    Device structure pointer.
 */
static inline void iis_enable_reception(struct iisc_device *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_RXEN;
}

/**
 *  \brief Disable the IISC module in reception
 *
 *  \param dev_inst    Device structure pointer.
 */
static inline void iis_disable_reception(struct iisc_device *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_RXDIS;
}

/**
 * \brief Enable the clocks for the IISC module
 *
 *  \param dev_inst    Device structure pointer.
 */
static inline void iis_enable_clocks(struct iisc_device *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_CKEN;
}

/**
 *  \brief Disable the clocks for the IISC module
 *
 *  \param dev_inst    Device structure pointer.
 */
static inline void iis_disable_clocks(struct iisc_device *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_CKDIS;
}

/**
 * \brief Get the IISC status value.
 *
 * \param dev_inst    Device structure pointer.
 *
 * \return Status value
 */
static inline uint32_t iis_get_status(struct iisc_device *dev_inst)
{
	return dev_inst->hw_dev->IISC_SR;
}

/**
 * \brief Clear the IISC status value.
 *
 * \param dev_inst    Device structure pointer.
 * \param status_mask Status mask value
 */
static inline void iis_clear_status(struct iisc_device *dev_inst,
		uint32_t status_mask)
{
	dev_inst->hw_dev->IISC_SCR = status_mask;
}

/**
 * \brief Set the IISC status value.
 *
 * \param dev_inst    Device structure pointer.
 * \param status_mask Status mask value
 */
static inline void iis_set_status(struct iisc_device *dev_inst,
		uint32_t status_mask)
{
	dev_inst->hw_dev->IISC_SSR = status_mask;
}

/**
 * \brief Enable the IISC interrupts.
 *
 * \param dev_inst    Device structure pointer.
 * \param int_mask interrupt mask value
 */
static inline void iis_enable_interrupts(struct iisc_device *dev_inst,
		uint32_t int_mask)
{
	dev_inst->hw_dev->IISC_IER = int_mask;
}

/**
 * \brief Disable the IISC interrupts.
 *
 * \param dev_inst    Device structure pointer.
 * \param int_mask interrupt mask value
 */
static inline void iis_disable_interrupts(struct iisc_device *dev_inst,
		uint32_t int_mask)
{
	dev_inst->hw_dev->IISC_IDR = int_mask;
}


/**
 * \brief Read the IISC interrupts mask value.
 *
 * \param dev_inst    Device structure pointer.
 *
 * \return Interrupt mask value
 */
static inline uint32_t iis_read_interrupt_mask(struct iisc_device *dev_inst)
{
	return dev_inst->hw_dev->IISC_IMR;
}

/**
 * \brief Enable the IISC module.
 *
 * \param dev_inst    Device structure pointer.
 *
 */
void iis_enable(struct iisc_device *dev_inst);

/**
 * \brief Disable the IISC module.
 *
 * \param dev_inst    Device structure pointer.
 *
 */
void iis_disable(struct iisc_device *dev_inst);

/**
 * \brief Write a single message of data.
 *
 * \param dev_inst    Device structure pointer.
 * \param data The data to write
 *
 * \return status
 */
status_code_t iis_write(struct iisc_device *dev_inst, uint32_t data);

/**
 * \brief Read a single message of data.
 *
 * \param dev_inst    Device structure pointer.
 * \param *data Pointer for receive data
 *
 * \return status
 */
status_code_t iis_read(struct iisc_device *dev_inst, uint32_t *data);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif

/**
 * \page sam_iisc_quick_start Quick Start Guide for the IISC driver
 *
 * This is the quick start guide for the \ref group_sam_drivers_iisc, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case.The code examples can be copied into e.g the main
 * application loop or any other function that will need to control the
 * IISC module.
 *
 * \section iisc_qs_use_cases Use cases
 * - \ref iisc_basic
 *
 * \section iisc_basic IISC basic usage
 *
 * This use case will demonstrate how to initialize the IISC module to
 * master or slave mode.
 *
 * \section iisc_basic_setup Setup steps
 *
 * \subsection iisc_basic_prereq Prerequisites
 *
 * This module requires the following service
 * - \ref clk_group
 *
 * \subsection iisc_basic_setup_workflow
 *
 * -# Enable the IISC module
 * -# Set the configuration
 *
 *  - \note The driver not cover all the configuration for
 * the clock setting complexity.
 *
 * \subsection iisc_basic_setup_code
 *
 * Add this to the main loop or a setup function:
 * \code
 * struct iisc_config config;
 * struct iisc_device dev_inst;
 * config.data_word_format = IISC_DATE_16BIT_COMPACT;
 * config.slot_length = 16;
 * config.fs_ratio = IISC_FS_RATE_256;
 * config.num_tx_channels = IISC_CHANNEL_STEREO;
 * config.num_rx_channels = IISC_CHANNEL_STEREO;
 * config.loopback = true;
 * config.master = true;
 * iis_init(&dev_inst, IISC, &config);
 * iisc_enable(&dev_inst);
 * \endcode
 *
 *
 * \section iisc_basic_usage Usage steps
 *
 * \subsection iisc_basic_usage_code
 *
 * We can enable the transfer by
 * \code
 * iisc_enable_transmission(&dev_inst);
 * iisc_enable_clocks(&dev_inst);
 * \endcode
 * Or we can enable the receive by
 * \code
 * iisc_enable_reception(&dev_inst);
 * \endcode
 *
 * If we not use the PDCA, we can access the data by
 * \code
 * iisc_write(&dev_inst, data);
 * iisc_read(&dev_inst, &data);
 * \endcode
 *
 * And we can set the interrupt by
 * \code
 * iisc_enable_interrupts(&dev_inst, int_mask);
 * \endcode
 */

#endif  /* _IISC_H_INCLUDED_ */
