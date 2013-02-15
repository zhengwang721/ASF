/**
 * \file
 *
 * \brief SAMD20 Generic Clock Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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
#ifndef SYSTEM_CLOCK_GCLK_H_INCLUDED
#define SYSTEM_CLOCK_GCLK_H_INCLUDED

/**
 * \addtogroup asfdoc_samd20_system_clock_group
 *
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Generic Clock Generator configuration structure.
 *
 * Configuration structure for a Generic Clock Generator channel. This
 * structure should be initialized by the
 * \ref system_gclk_gen_get_config_defaults() function before being modified by
 * the user application.
 */
struct system_gclk_gen_conf {
	/** Source clock input channel index. */
	uint8_t source_clock;
	/** If \c true, the generator output level is high when disabled. */
	bool high_when_disabled;
	/** Integer division factor of the clock output compared to the input. */
	uint32_t division_factor;
	/** If \c true the clock is kept enabled during device standby mode. */
	bool run_in_standby;
	/** If \c true, enables GCLK generator clock output to a GPIO pin. */
	bool output_enable;
};

/**
 * \brief Generic Clock configuration structure.
 *
 * Configuration structure for a Generic Clock channel. This structure
 * should be initialized by the \ref system_gclk_chan_get_config_defaults()
 * function before being modified by the user application.
 */
struct system_gclk_chan_conf {
	/** Generic Clock Generator source channel. */
	uint8_t source_generator;
	/** If \c true, the clock is kept enabled during device standby mode. */
	bool run_in_standby;
};


/** \name Generic Clock configuration and initialization
 * @{
 */

/**
 * \brief Determines if the hardware module(s) are currently synchronizing to the bus.
 *
 * Checks to see if the underlying hardware peripheral module(s) are currently
 * synchronizing across multiple clock domains to the hardware bus, This
 * function can be used to delay further operations on a module until such time
 * that it is ready, to prevent blocking delays for synchronization in the
 * user application.
 *
 * \return Synchronization status of the underlying hardware module(s).
 *
 * \retval true if the module has completed synchronization
 * \retval false if the module synchronization is ongoing
 */
static inline bool system_gclk_is_syncing(void)
{
	if (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {
		return true;
	}

	return false;
}

void system_gclk_init(void);

/** @} */


/**
 * \name Generic Clock configuration and initialization (Generators)
 * @{
 */

/**
 * \brief Initializes a Generic Clock Generator configuration structure to defaults.
 *
 * Initializes a given Generic Clock Generator configuration structure to
 * a set of known default values. This function should be called on all
 * new instances of these configuration structures before being modified
 * by the user application.
 *
 * The default configuration is as follows:
 *  \li Clock is generated undivided from the source frequency
 *  \li Clock generator output is low when the generator is disabled
 *  \li The input clock is sourced from input clock channel 0
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void system_gclk_gen_get_config_defaults(
		struct system_gclk_gen_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->division_factor    = 1;
	config->high_when_disabled = false;
	config->source_clock       = 0;
	config->run_in_standby     = false;
	config->output_enable      = false;
}

void system_gclk_gen_set_config(
		const uint8_t generator,
		struct system_gclk_gen_conf *const config);

void system_gclk_gen_enable(
		const uint8_t generator);

void system_gclk_gen_disable(
		const uint8_t generator);

/** @} */


/**
 * \name Generic Clock configuration and initialization (Channels)
 * @{
 */

/**
 * \brief Initializes a Generic Clock configuration structure to defaults.
 *
 * Initializes a given Generic Clock configuration structure to a set of
 * known default values. This function should be called on all new
 * instances of these configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 *  \li Clock is sourced from the Generic Clock Generator channel 0
 *  \li Clock is disabled during device sleep
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void system_gclk_chan_get_config_defaults(
		struct system_gclk_chan_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->source_generator = 0;

	#if !defined (REVB)
	config->run_in_standby = false;
	#endif
}

void system_gclk_chan_set_config(
		const uint8_t channel,
		struct system_gclk_chan_conf *const config);

void system_gclk_chan_enable(
		const uint8_t channel);

void system_gclk_chan_disable(
		const uint8_t channel);

/** @} */


/**
 * \name Generic Clock frequency retrieval
 * @{
 */

uint32_t system_gclk_gen_get_hz(
		const uint8_t generator);

uint32_t system_gclk_chan_get_hz(
		const uint8_t channel);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

#endif
