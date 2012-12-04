/**
 *
 * \file
 *
 * \brief IISC driver for SAM.
 *
 * This file defines a useful set of functions for the IISC on SAM devices.
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

#ifndef _IISC_H_INCLUDED
#define _IISC_H_INCLUDED

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

/** Supported sample rate */
enum iisc_sample_rate {
	IISC_SAMPLE_RATE_32000 = 0,
	IISC_SAMPLE_RATE_48000,
	IISC_SAMPLE_RATE_96000,
	IISC_SAMPLE_RATE_192000,
};

/** Supported Master Clock to Sample Frequency (fs) Ratio */
enum iisc_fs_rate {
	IISC_FS_RATE_16  = 16,
	IISC_FS_RATE_32  = 32,
	IISC_FS_RATE_64  = 64,
	IISC_FS_RATE_128 = 128,
	IISC_FS_RATE_256 = 256,
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
	/* Sample rate. */
	enum iisc_sample_rate sample_rate_hz;
	/* Number of bit per sample. */
	enum iisc_data_format data_word_format;
	/* Master Clock to Sample Frequency (fs) Ratio. */
	enum iisc_fs_rate fs_ratio;
	/* 1 for Mono, 0 for stereo. */
	bool tx_mono;
	/* 1 for Mono, 0 for stereo. */
	bool rx_mono;
	/* 1 for loop back, 0 for normal. */
	bool loop;
	/* 1 for master, 0 for slave. */
	bool mode;
};

/**
 * \brief Resets the IISC module
 *
 *  \param iisc Base address of the IISC
 */
static inline void iisc_reset(Iisc *iisc)
{
	iisc->IISC_CR = IISC_CR_SWRST;
}

/**
 *  \brief Enable the IISC module in transmission
 *
 *  \param iisc Base address of the IISC
 */
static inline void iisc_enable_transmission(Iisc *iisc)
{
	iisc->IISC_CR = IISC_CR_TXEN;
}

/**
 *  \brief Disable the IISC module in transmission
 *
 *  \param iisc Base address of the IISC
 */
static inline void iisc_disable_transmission(Iisc *iisc)
{
	iisc->IISC_CR = IISC_CR_TXDIS;
}

/**
 *  \brief Enable the IISC module in reception
 *
 *  \param iisc Base address of the IISC
 */
static inline void iisc_enable_reception(Iisc *iisc)
{
	iisc->IISC_CR = IISC_CR_RXEN;
}

/**
 *  \brief Disable the IISC module in reception
 *
 *  \param iisc Base address of the IISC
 */
static inline void iisc_disable_reception(Iisc *iisc)
{
	iisc->IISC_CR = IISC_CR_RXDIS;
}

/**
 * \brief Enable the clocks for the IISC module
 *
 *  \param iisc Base address of the IISC
 */
static inline void iisc_enable_clocks(Iisc *iisc)
{
	iisc->IISC_CR = IISC_CR_CKEN;
}

/**
 *  \brief Disable the clocks for the IISC module
 *
 *  \param iisc Base address of the IISC
 */
static inline void iisc_disable_clocks(Iisc *iisc)
{
	iisc->IISC_CR = IISC_CR_CKDIS;
}

/**
 * \brief Get the IISC status value.
 *
 * \param iisc Base address of the IISC
 *
 * \return Status value
 */
static inline uint32_t iisc_get_status(Iisc *iisc)
{
	return iisc->IISC_SR;
}

/**
 * \brief Clear the IISC status value.
 *
 * \param iisc Base address of the IISC
 * \param status_mask Status mask value
 */
static inline void iisc_clear_status(Iisc *iisc, uint32_t status_mask)
{
	iisc->IISC_SCR = status_mask;
}

/**
 * \brief Set the IISC status value.
 *
 * \param iisc Base address of the IISC
 * \param status_mask Status mask value
 */
static inline void iisc_set_status(Iisc *iisc, uint32_t status_mask)
{
	iisc->IISC_SSR = status_mask;
}

/**
 * \brief Enable the IISC interrupts.
 *
 * \param iisc Base address of the IISC
 * \param int_mask interrupt mask value
 */
static inline void iisc_enable_interrupts(Iisc *iisc, uint32_t int_mask)
{
	iisc->IISC_IER = int_mask;
}

/**
 * \brief Disable the IISC interrupts.
 *
 * \param iisc Base address of the IISC
 * \param int_mask interrupt mask value
 */
static inline void iisc_disable_interrupts(Iisc *iisc, uint32_t int_mask)
{
	iisc->IISC_IDR = int_mask;
}


/**
 * \brief Read the IISC interrupts mask value.
 *
 * \param iisc Base address of the IISC
 *
 * \return Interrupt mask value
 */
static inline uint32_t iisc_read_interrupt_mask(Iisc *iisc)
{
	return iisc->IISC_IMR;
}

/**
 * \brief Enable the IISC module.
 *
 * \param iisc Base address of the IISC
 *
 */
void iisc_enable(Iisc *iisc);

/**
 * \brief Disable the IISC module.
 *
 * \param iisc Base address of the IISC
 *
 */
void iisc_disable(Iisc *iisc);

/**
 * \brief Configurate the IISC module.
 *
 * \param iisc Base address of the IISC
 * \param iisc_conf IISC configuration
 *
 * \return status
 */
status_code_t iisc_set_config(Iisc *iisc, struct iisc_config *iisc_conf);

/**
 * \brief Write a single message of data.
 *
 * \param iisc Base address of the IISC
 * \param data The data to write
 *
 * \return status
 */
status_code_t iisc_write(Iisc *iisc, uint32_t data);

/**
 * \brief Read a single message of data.
 *
 * \param iisc Base address of the IISC
 * \param *data Pointer for receive data
 *
 * \return status
 */
status_code_t iisc_read(Iisc *iisc, uint32_t *data);

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
 * iisc_enable(IISC);
 * struct iisc_config config;
 * config.sample_rate_hz = IISC_SAMPLE_RATE_48000;
 * config.fs_ratio = IISC_FS_RATE_256;
 * config.data_word_format = IISC_DATE_16BIT_COMPACT;
 * config.loop = false;
 * config.mode = true;
 * iisc_set_config(IISC, &config);
 * \endcode
 *
 *
 * \section iisc_basic_usage Usage steps
 *
 * \subsection iisc_basic_usage_code
 *
 * We can enable the transfer by
 * \code
 * iisc_enable_transmission(IISC);
 * iisc_enable_clocks(IISC);
 * \endcode
 * Or we can enable the receive by
 * \code
 * iisc_enable_reception(IISC);
 * \endcode
 *
 * If we not use the PDCA, we can access the data by
 * \code
 * iisc_write(IISC, data);
 * iisc_read(IISC, &data);
 * \endcode
 *
 * And we can set the interrupt by
 * \code
 * iisc_enable_interrupts(IISC, int_mask);
 * \endcode
 */

#endif  /* _IISC_H_INCLUDED */
