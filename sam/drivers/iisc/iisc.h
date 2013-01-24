/**
 *
 * \file
 *
 * \brief IIS driver for SAM.
 *
 * This file defines a useful set of functions for the IIS on SAM devices.
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

#ifndef IIS_H_INCLUDED
#define IIS_H_INCLUDED

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

#define IIS_RETRY_VALUE   10000

/** Supported number of channels */
enum iis_number_of_channels {
	IIS_CHANNEL_MONO = 0,
	IIS_CHANNEL_STEREO
};

/** Supported Master Clock to Sample Frequency (fs) Ratio */
enum iis_fs_rate {
	IIS_FS_RATE_16  = 0,
	IIS_FS_RATE_32  = 1,
	IIS_FS_RATE_48  = 2,
	IIS_FS_RATE_64  = 3,
	IIS_FS_RATE_96  = 5,
	IIS_FS_RATE_128 = 7,
	IIS_FS_RATE_192 = 11,
	IIS_FS_RATE_256 = 15,
	IIS_FS_RATE_384 = 23,
	IIS_FS_RATE_512 = 31,
	IIS_FS_RATE_768 = 47,
	IIS_FS_RATE_1024 = 63,
};

/** Slot length */
enum iis_slot_length {
	IIS_SLOT_LENGTH_8BIT  = 8,
	IIS_SLOT_LENGTH_16BIT = 16,
	IIS_SLOT_LENGTH_24BIT = 24,
	IIS_SLOT_LENGTH_32BIT = 32,
};

/** Data format */
enum iis_data_format {
	IIS_DATE_32BIT = 0,
	IIS_DATE_24BIT,
	IIS_DATE_20BIT,
	IIS_DATE_18BIT,
	IIS_DATE_16BIT,
	IIS_DATE_16BIT_COMPACT,
	IIS_DATE_8BIT,
	IIS_DATE_8BIT_COMPACT,
};

/** DMA channel usage for IIS transfer */
enum iis_dma_channel {
	IIS_ONE_DMA_CHANNEL_FOR_BOTH_CHANNELS = 0,
	IIS_ONE_DMA_CHANNEL_FOR_ONE_CHANNEL,
};

/** Configuration setting structure */
struct iis_config {
	/* Number of bits per sample. */
	enum iis_data_format iis_data_format;

	/* Number of bits of the slot */
	enum iis_slot_length iis_slot_length;

	/* Master Clock to Sample Frequency (fs) Ratio. */
	enum iis_fs_rate iis_fs_ratio;

	/* Number of channels for Tx */
	enum iis_number_of_channels iis_tx_channels;

	/* Number of channels for Rx */
	enum iis_number_of_channels iis_rx_channels;

	/* DMA channel usage for IIS transmission */
	enum iis_dma_channel iis_tx_dma;

	/* DMA channel usage for IIS reception */
	enum iis_dma_channel iis_rx_dma;

	/* 1 for loopback, 0 for normal. */
	bool iis_loopback;

	/* 1 for master, 0 for slave. */
	bool iis_master;
};

struct iis_dev_inst {
	/* Base Address of the IISC module */
	Iisc *hw_dev;

	/* Pointer to IIS configuration structure. */
	struct iis_config *cfg;
};

typedef void (*iis_callback_t)(void);

/**
 * \brief Get the default IIS module configuration:
 * Data format: 32 bits
 * Slot length: 32 bits
 * Sample frequecny ratio: 1024
 * Tx channel: Stereo
 * Rx channel: Stereo
 * DMA for Tx: 1 DMA channel for 1 IIS Tx channel
 * DMA for Rx: 1 DMA channel for 1 IIS Rx channel
 * Loopback: No
 * Master: Yes
 *
 * \param cfg         Pointer to IIS configuration.
 */
static inline void iis_get_config_defaults(struct iis_config *const cfg)
{
	/* Sanity check arguments */
	Assert(cfg);

	cfg->iis_data_format = IIS_DATE_32BIT;
	cfg->iis_slot_length = IIS_SLOT_LENGTH_32BIT;
	cfg->iis_fs_ratio = IIS_FS_RATE_1024;
	cfg->iis_tx_channels = IIS_CHANNEL_STEREO;
	cfg->iis_rx_channels = IIS_CHANNEL_STEREO;
	cfg->iis_tx_dma = IIS_ONE_DMA_CHANNEL_FOR_ONE_CHANNEL;
	cfg->iis_rx_dma = IIS_ONE_DMA_CHANNEL_FOR_ONE_CHANNEL;
	cfg->iis_loopback = false;
	cfg->iis_master = true;
}

enum status_code iis_init(struct iis_dev_inst *const dev_inst, Iisc *iisc,
		struct iis_config *const cfg);

/**
 * \brief Resets the IIS module
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void iis_reset(struct iis_dev_inst *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_SWRST;
}

/**
 * \brief Enable the IIS module in transmission
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void iis_enable_transmission(struct iis_dev_inst *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_TXEN;
}

/**
 * \brief Disable the IIS module in transmission
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void iis_disable_transmission(struct iis_dev_inst *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_TXDIS;
}

/**
 * \brief Enable the IIS module in reception
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void iis_enable_reception(struct iis_dev_inst *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_RXEN;
}

/**
 * \brief Disable the IIS module in reception
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void iis_disable_reception(struct iis_dev_inst *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_RXDIS;
}

/**
 * \brief Enable the clocks for the IIS module
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void iis_enable_clocks(struct iis_dev_inst *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_CKEN;
}

/**
 * \brief Disable the clocks for the IIS module
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void iis_disable_clocks(struct iis_dev_inst *dev_inst)
{
	dev_inst->hw_dev->IISC_CR = IISC_CR_CKDIS;
}

/**
 * \brief Get the IIS status value.
 *
 * \param dev_inst    Device structure pointer.
 *
 * \return Status value
 */
static inline uint32_t iis_get_status(struct iis_dev_inst *dev_inst)
{
	return dev_inst->hw_dev->IISC_SR;
}

typedef enum iis_interrupt_source {
	IIS_INTERRUPT_RXRDY = 0,
	IIS_INTERRUPT_RXOR,
	IIS_INTERRUPT_TXRDY,
	IIS_INTERRUPT_TXUR,
	_IIS_INTERRUPT_SOURCE_NUM,
} iis_interrupt_source_t;

void iis_set_callback(struct iis_dev_inst *const dev_inst,
		iis_interrupt_source_t source, iis_callback_t callback,
		uint8_t irq_level);

void iis_clear_status(struct iis_dev_inst *const dev_inst,
		iis_interrupt_source_t source);
void iis_enable_interrupt(struct iis_dev_inst *const dev_inst,
		iis_interrupt_source_t source);
void iis_disable_interrupt(struct iis_dev_inst *const dev_inst,
		iis_interrupt_source_t source);

/**
 * \brief Get the IIS interrupts mask value.
 *
 * \param dev_inst    Device structure pointer.
 *
 * \return Interrupt mask value
 */
static inline uint32_t iis_get_interrupt_mask(
		struct iis_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->IISC_IMR;
}

void iis_enable(struct iis_dev_inst *const dev_inst);
void iis_disable(struct iis_dev_inst *const dev_inst);

enum status_code iis_write(struct iis_dev_inst *dev_inst, uint32_t data);
enum status_code iis_read(struct iis_dev_inst *dev_inst, uint32_t *data);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif

/**
 * \page sam_iis_quick_start Quick Start Guide for the IIS driver
 *
 * This is the quick start guide for the \ref group_sam_drivers_iisc, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case.The code examples can be copied into e.g the main
 * application loop or any other function that will need to control the
 * IIS module.
 *
 * \section iis_qs_use_cases Use cases
 * - \ref iis_basic
 *
 * \section iis_basic IIS basic usage
 *
 * This use case will demonstrate how to initialize the IIS module to
 * master in loopback mode.
 *
 * \section iis_basic_setup Setup steps
 *
 * \subsection iis_basic_prereq Prerequisites
 *
 * This module requires the following service
 * - \ref clk_group
 *
 * \subsection iis_basic_setup_workflow
 *
 * -# Enable the IIS module
 * -# Set the configuration
 *
 *  - \note The driver not cover all the configuration for
 * the clock setting complexity.
 *
 * \subsection iis_basic_setup_code
 *
 * Add this to the main loop or a setup function:
 * \code
 *  struct iis_config config;
 *  struct iis_dev_inst dev_inst;
 *  config.ac_data_format = IIS_DATE_16BIT_COMPACT;
 *  config.ac_slot_length = IIS_SLOT_LENGTH_16BIT;
 *  config.ac_fs_ratio = IIS_FS_RATE_256;
 *  config.ac_num_tx_channels = IIS_CHANNEL_STEREO;
 *  config.ac_num_rx_channels = IIS_CHANNEL_STEREO;
 *  config.ac_loopback = true;
 *  config.ac_master = true;
 *  iis_init(&dev_inst, IISC, &config);
 *  iis_enable(&dev_inst);
 * \endcode
 *
 *
 * \section iis_basic_usage Usage steps
 *
 * \subsection iis_basic_usage_code
 *
 * We can enable the transfer by
 * \code
 *  iis_enable_transmission(&dev_inst);
 *  iis_enable_clocks(&dev_inst);
 * \endcode
 * Or we can enable the receive by
 * \code
 *  iis_enable_reception(&dev_inst);
 * \endcode
 *
 * If we not use the PDCA, we can access the data by
 * \code
 *  iis_write(&dev_inst, data);
 *  iis_read(&dev_inst, &data);
 * \endcode
 *
 * And we can set the interrupt by
 * \code
 *  iis_enable_interrupt(&dev_inst, interrupt_source);
 * \endcode
 */

#endif  /* IIS_H_INCLUDED */
