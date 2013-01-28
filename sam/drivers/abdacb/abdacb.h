/**
 * \file
 *
 * \brief ABDACB driver for SAM.
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#ifndef _ABDACB_H_INCLUDED
#define _ABDACB_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup group_sam_drivers_abdacb ABDACB - Audio Bitstream DAC
 *
 * Audio Bitstream DAC (Digital to Analog Converter) provides functions to
 * convert a 16-bit sample value to a digital bitstream.
 *
 * \{
 */

#include "compiler.h"

/** Supported sample rate */
enum abdacb_sample_rate {
	ABDACB_SAMPLE_RATE_8000 = 0,
	ABDACB_SAMPLE_RATE_11025,
	ABDACB_SAMPLE_RATE_12000,
	ABDACB_SAMPLE_RATE_16000,
	ABDACB_SAMPLE_RATE_22050,
	ABDACB_SAMPLE_RATE_24000,
	ABDACB_SAMPLE_RATE_32000,
	ABDACB_SAMPLE_RATE_44100,
	ABDACB_SAMPLE_RATE_48000,
};

/** Data format of the SDR register value */
enum abdacb_data_format {
	ABDACB_DATE_32BIT = 0,
	ABDACB_DATE_20BIT,
	ABDACB_DATE_18BIT,
	ABDACB_DATE_16BIT,
	ABDACB_DATE_16BIT_COMPACT,
	ABDACB_DATE_8BIT,
	ABDACB_DATE_8BIT_COMPACT,
};

/** Configuration setting structure */
struct abdacb_config {
	/* Sample rate. */
	enum abdacb_sample_rate sample_rate_hz;
	/* Number of bit per sample. */
	enum abdacb_data_format data_word_format;
	/* 1 for Mono, 0 for stereo. */
	uint32_t mono;
	/* 1 for enable, 0 for disable. */
	uint32_t cmoc;
};

/** Interrupt sources */
typedef enum abdacb_interrupt_source {
	ABDACB_INTERRUPT_TXRDY = 0,
	ABDACB_INTERRUPT_TXUR,
} abdacb_interrupt_source_t;

/**
 * \brief ABDACB driver software instance structure.
 *
 * Device instance structure for a ABDACB driver instance. This
 * structure should be initialized by the \ref abdacb_init() function to
 * associate the instance with a particular hardware module of the device.
 */
struct abdacb_dev_inst {
	/** Base address of the ABDACB module. */
	Abdacb *hw_dev;
	/** Pointer to ABDACB configuration structure. */
	struct abdacb_config  *abdacb_cfg;
};

/**
 * \brief Interrupt callback function type for ABDACB.
 *
 * The interrupt handler can be configured to do a function callback,
 * the callback function must match the abdacb_callback_t type.
 *
 */
typedef void (*abdacb_callback_t)(void);

/**
 * \brief Initializes a ABDACB configuration structure to defaults.
 *
 *  Initializes a given ABDACB configuration structure to a set of
 *  known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *  - sample rate with 8000hz.
 *  - 16 bit data format.
 *  - stereo mode.
 *  - Common mode adjustment is disabled.
 *
 *  \param cfg    Configuration structure to initialize to default values.
 */
void abdacb_get_config_defaults(struct abdacb_config *const cfg);

/**
 * \brief Initialize the ABDACB module.
 *
 * \param dev_inst    Device structure pointer.
 * \param abdacb         Base address of the ABDACB instance.
 * \param cfg         Pointer to ABDACB configuration.
 *
 * \retval true if the initialization was successful.
 * \retval false if initialization failed.
 */
bool abdacb_init(struct abdacb_dev_inst *const dev_inst, Abdacb *const abdacb,
		struct abdacb_config *const cfg);

/**
 * \brief Check the busy status of ABDACB.
 *
 * \param dev_inst Device structure pointer..
 *
 * \return 1 if ABDACB is busy, else it will return 0.
 */
static inline bool abdacb_is_busy(struct abdacb_dev_inst *const dev_inst)
{
	return (dev_inst->hw_dev->ABDACB_SR & ABDACB_SR_BUSY) != 0;
}

/**
 * \brief Check the transmit ready status of ABDACB.
 *
 * \param dev_inst Device structure pointer..
 *
 * \return 1 if ABDACB is ready to receive a new data in SDR,
 * else it will return 0.
 */
static inline bool abdacb_is_tx_ready(struct abdacb_dev_inst *const dev_inst)
{
	return (dev_inst->hw_dev->ABDACB_SR & ABDACB_SR_TXRDY) != 0;
}

/**
 * \brief Check the transmit underrun status of ABDACB.
 *
 * \param dev_inst Device structure pointer..
 *
 * \return 1 if at least one underrun has occurred since the last time
 * this bit was cleared, else it will return 0.
 */
static inline bool abdacb_is_tx_underrun(
		struct abdacb_dev_inst *const dev_inst)
{
	return (dev_inst->hw_dev->ABDACB_SR & ABDACB_SR_TXUR) != 0;
}

/**
 * \brief Return the ABDACB interrupts mask value.
 *
 * \param dev_inst Device structure pointer..
 *
 * \return Interrupt mask value
 */
static inline uint32_t abdacb_read_interrupt_mask(
		struct abdacb_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->ABDACB_IMR;
}

/**
 * \brief Enable the ABDACB module.
 *
 * \param dev_inst Device structure pointer..
 */
void abdacb_enable(struct abdacb_dev_inst *const dev_inst);

/**
 * \brief Disable the ABDACB module.
 *
 * \param dev_inst Device structure pointer..
 */
void abdacb_disable(struct abdacb_dev_inst *const dev_inst);

/**
 * \brief Configurate the ABDACB module.
 *
 * \param dev_inst Device structure pointer..
 */
void abdacb_set_config(struct abdacb_dev_inst *const dev_inst);

/**
 * \brief Software rest the ABDACB module.
 *
 * \param dev_inst Device structure pointer..
 */
void abdacb_sw_reset(struct abdacb_dev_inst *const dev_inst);

/**
 * \brief Swap the ABDACB channal output.
 *
 * \param dev_inst Device structure pointer..
 */
void abdacb_swap_channels(struct abdacb_dev_inst *const dev_inst);

/**
 * \brief Writes the data to SDR0.
 *
 * \param dev_inst Device structure pointer..
 * \param data Data value to write to SDR0.
 */
void abdacb_write_data0(struct abdacb_dev_inst *const dev_inst,
		uint32_t data);

/**
 * \brief Writes the data to SDR1.
 *
 * \param dev_inst Device structure pointer..
 * \param data Data value to write to SDR1.
 */
void abdacb_write_data1(struct abdacb_dev_inst *const dev_inst,
		uint32_t data);

/**
 * \brief Set the volume of channal 0.
 *
 * \param dev_inst Device structure pointer..
 * \param mute Flag if set the channal mute
 * \param volume Value of volume
 */
void abdacb_set_volume0(struct abdacb_dev_inst *const dev_inst, bool mute,
		uint32_t volume);

/**
 * \brief Set the volume of channal 1.
 *
 * \param dev_inst Device structure pointer..
 * \param mute Flag if set the channal mute
 * \param volume Value of volume
 */
void abdacb_set_volume1(struct abdacb_dev_inst *const dev_inst, bool mute,
		uint32_t volume);

/**
 * \brief Enable the interrupt.
 *
 * \param dev_inst Device structure pointer..
 * \param source Interrupt to be enabled
 */
void abdacb_enable_interrupt(struct abdacb_dev_inst *const dev_inst,
		abdacb_interrupt_source_t source);

/**
 * \brief Disable the interrupt.
 *
 * \param dev_inst Device structure pointer..
 * \param source Interrupt to be disabled
 */
void abdacb_disable_interrupt(struct abdacb_dev_inst *const dev_inst,
		abdacb_interrupt_source_t source);

/**
 * \brief Clear the interrupt status.
 *
 * \param dev_inst Device structure pointer..
 * \param source Interrupt status to be cleared
 */
void abdacb_clear_interrupt_flag(struct abdacb_dev_inst *const dev_inst,
		abdacb_interrupt_source_t source);

/**
 * \brief Set callback for ABDACB
 *
 * \param dev_inst Device structure pointer.
 * \param source Interrupt source.
 * \param callback callback function pointer.
 * \param irq_level interrupt level.
 */
void abdacb_set_callback(struct abdacb_dev_inst *const dev_inst,
		abdacb_interrupt_source_t source, abdacb_callback_t callback,
		uint8_t irq_level);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif

/**
 * \page sam_abdacb_quick_start Quick Start Guide for the ABDACB driver
 *
 * This is the quick start guide for the \ref group_sam_drivers_abdacb, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case.The code examples can be copied into e.g the main
 * application loop or any other function that will need to control the
 * ABDACB module.
 *
 * \section abdacb_qs_use_cases Use cases
 * - \ref abdacb_basic
 *
 * \section abdacb_basic ABDACB basic usage
 *
 * This use case will demonstrate how to initialize the ABDACB module to
 * output audio data.
 *
 *
 * \section abdacb_basic_setup Setup steps
 *
 * \subsection abdacb_basic_prereq Prerequisites
 *
 * This module requires the following service
 * - \ref clk_group
 *
 * \subsection abdacb_basic_setup_code
 *
 * Add this to the main loop or a setup function:
 * \code
 * struct abdacb_dev_inst g_abdacb_inst;
 * struct abdacb_config   g_abdacb_cfg;
 * abdacb_get_config_defaults(&g_abdacb_cfg);
 * g_abdacb_cfg.sample_rate_hz = ABDACB_SAMPLE_RATE_11025;
 * g_abdacb_cfg.data_word_format = ABDACB_DATE_16BIT;
 * g_abdacb_cfg.mono = false;
 * g_abdacb_cfg.cmoc = false;
 * abdacb_init(&g_abdacb_inst, ABDACB, &g_abdacb_cfg);
 * abdacb_enable(&g_abdacb_inst);
 * \endcode
 *
 * \subsection abdacb_basic_setup_workflow
 *
 * -# Enable the ABDACB module
 * -# Configurate the ABDACB mode
 *
 *  - \note The syste clock may changed after setting the ABDACB module.
 *
 *
 * \section abdacb_basic_usage Usage steps
 *
 * \subsection abdacb_basic_usage_code
 *
 * We can set the volume by
 * \code
 * abdacb_set_volume0(&g_abdacb_inst, false, 0x7FFF);
 * abdacb_set_volume1(&g_abdacb_inst, false, 0x7FFF);
 * \endcode
 * Or we can mute the volume by
 * \code
 * abdacb_set_volume0(&g_abdacb_inst, true, 0x7FFF);
 * abdacb_set_volume1(&g_abdacb_inst, true, 0x7FFF);
 * \endcode
 *
 * We can output the data without PDC by
 * \code
 * abdacb_write_data0(&g_abdacb_inst, data);
 * abdacb_write_data0(&g_abdacb_inst, data);
 * \endcode
 *
 * And we can set the interrupt by
 * \code
 * abdacb_set_callback(&g_abdacb_inst, ABDACB_INTERRUPT_TXRDY, callback, 3)
 * \endcode
 */

#endif  /* _ABDACB_H_INCLUDED */

