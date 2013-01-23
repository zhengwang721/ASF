/**
 * \file
 *
 * \brief Peripheral Event Controller Driver for SAM.
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

#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup group_sam_drivers_events Peripheral Event Controller Driver for SAM
 *
 * This driver provides a unified interface for the configuration and
 * management of the event channels within the device.
 *
 * The peripheral event generators and users are interconnected by a network
 * known as the Peripheral Event System.
 *
 * The Peripheral Event System allows low latency peripheral-to-peripheral
 * signalling without CPU intervention, and without consuming system resources
 * such as bus or RAM bandwidth. This offloads the CPU and system resources
 * compared to a traditional interrupt-based software driven system.
 *
 * \note The clock of PEVC must be enabled before using this module.
 * We may enable the clock in system initialization.
 *
 * @{
 */

/** Maximum number for event channels (users) */
#define EVENT_CHANNEL_N      PEVC_TRIGOUT_BITS

/** Maximum number for event generator */
#define EVENT_GENERATOR_N    PEVC_EVIN_BITS

/**
 * \brief Input Glitch Filter divider configurations.
 *
 * Enumerate for the possible division ratio of Input Glitch Filter.
 */
enum events_igf_divider {
	EVENT_IGF_DIVIDER_1       = 0,
	EVENT_IGF_DIVIDER_2       = 1,
	EVENT_IGF_DIVIDER_4       = 2,
	EVENT_IGF_DIVIDER_8       = 3,
	EVENT_IGF_DIVIDER_16      = 4,
	EVENT_IGF_DIVIDER_32      = 5,
	EVENT_IGF_DIVIDER_64      = 6,
	EVENT_IGF_DIVIDER_128     = 7,
	EVENT_IGF_DIVIDER_256     = 8,
	EVENT_IGF_DIVIDER_512     = 9,
	EVENT_IGF_DIVIDER_1024    = 10,
	EVENT_IGF_DIVIDER_2048    = 11,
	EVENT_IGF_DIVIDER_4096    = 12,
	EVENT_IGF_DIVIDER_8192    = 13,
	EVENT_IGF_DIVIDER_16384   = 14,
	EVENT_IGF_DIVIDER_32768   = 15
};

/**
 * \brief Event Input Glitch Filter edge detection configurations.
 */
enum events_igf_edge {
	/** Input Glitch Filter is disabled */
	EVENT_IGF_EDGE_NONE    = 0,
	/** Event detection through Input Glitch Fiilter on rising edge. */
	EVENT_IGF_EDGE_RISING  = 1,
	/** Event detection through Input Glitch Fiilter on falling edge. */
	EVENT_IGF_EDGE_FALLING = 2,
	/** Event detection through Input Glitch Fiilter on both edges. */
	EVENT_IGF_EDGE_BOTH    = 3
};

/**
 * \brief Event channel configuration structure.
 *
 * Configuration structure for an event channel.
 */
struct events_ch_conf {
	/** Channel to configure (user) */
	uint32_t channel_id;
	/** Event generator to connect to the channel */
	uint32_t generator_id;
	/** Enable event sharper or not */
	bool sharper_enable;
	/** Edge detection for event channels */
	enum events_igf_edge   igf_edge;
};

/**
 * \brief Set a value for the Input Glitch Filter Divider.
 *
 * \param  divider      Input Glitch Filter divider.
 *
 * \note As stated in the datasheet, there is one divider value for
 * all EVS instance.
 */
static inline void events_set_igf_divider(enum events_igf_divider divider)
{
	PEVC->PEVC_IGFDR = PEVC_IGFDR_IGFDR(divider);
}

/**
 * \brief Initialize an event channel configuration structure to defaults.
 *
 *  The default configuration is as follows:
 *  - Channel ID is initialized to invalid number.
 *  - Generator ID is initialized to invalid number.
 *  - Event shaper is disabled.
 *  - Event Input Glitch Filter is disabled.
 *
 *  \param config    Configuration structure to initialize to default values.
 */
void events_ch_get_config_defaults(struct events_ch_conf *const config);

/**
 * \brief Configure an event channel.
 *
 * \param config      Configuration settings for the event channel.
 */
void events_ch_configure(struct events_ch_conf *const config);

/**
 * \brief Enable an event channel.
 *
 * \param channel_id  Channel ID.
 */
static inline void events_ch_enable(uint32_t channel_id)
{
	PEVC->PEVC_CHER = PEVC_CHER_CHE(PEVC_CHER_CHE_1 << channel_id);
}

/**
 * \brief Disable an event channel.
 *
 * \param channel_id  Channel ID.
 */
static inline void events_ch_disable(uint32_t channel_id)
{
	PEVC->PEVC_CHDR = PEVC_CHDR_CHD(PEVC_CHDR_CHD_1 << channel_id);
}

/**
 * \brief Get status (enabled or disabled) of a channel.
 *
 * \param channel_id  Channel ID.
 *
 * \retval true  channel is enabled.
 * \retval false channel is disabled.
 */
static inline bool events_ch_is_enabled(uint32_t channel_id)
{
	if (PEVC->PEVC_CHSR & PEVC_CHSR_CHS(PEVC_CHSR_CHS_1 << channel_id)) {
		return true;
	} else {
		return false;
	}
}

/**
 * \brief Get the busy status of an event channel.
 *
 * \param channel_id  Channel ID.
 *
 *  \retval true  If the channel is ready to be used
 *  \retval false If the channel is currently busy
 */
static inline bool events_ch_is_ready(uint32_t channel_id)
{
	if (PEVC->PEVC_BUSY & PEVC_BUSY_BUSY(PEVC_BUSY_BUSY_1 << channel_id)) {
		return false;
	} else {
		return true;
	}
}

/**
 * \brief Enable software trigger for a channel.
 *
 * \param channel_id  Channel ID.
 */
static inline void events_ch_enable_software_trigger(uint32_t channel_id)
{
	PEVC->PEVC_CHMX[channel_id].PEVC_CHMX |= PEVC_CHMX_SMX;
}

/**
 * \brief Disable software trigger for a channel.
 *
 * \param channel_id  Channel ID.
 */
static inline void events_ch_disable_software_trigger(uint32_t channel_id)
{
	PEVC->PEVC_CHMX[channel_id].PEVC_CHMX &= (~PEVC_CHMX_SMX);
}

/**
 * \brief Trigger a Software Event for the corresponding channel.
 *
 * \param channel_id  Channel ID.
 */
static inline void events_ch_software_trigger(uint32_t channel_id)
{
	PEVC->PEVC_SEV = PEVC_SEV_SEV(PEVC_SEV_SEV_1 << channel_id);
}

/** @} */

#ifdef __cplusplus
}
#endif

/**
 * \page sam_events_quick_start Quick Start Guide for the Event System Driver
 *
 * TBD
 */

#endif /* EVENTS_H_INCLUDED */
