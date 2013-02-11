/**
 * \file
 *
 * \brief SAMD20 Brown Out Detector Driver
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
#ifndef BOD_H_INCLUDED
#define BOD_H_INCLUDED

#include <compiler.h>

/**
 * \brief Brown Out Detector hardware instance IDs.
 *
 * List of possible BOD controllers within the device.
 */
enum bod {
	/** BOD12 Internal core voltage. */
	BOD_BOD12,
	/** BOD33 External I/O voltage, */
	BOD_BOD33,
};

/**
 * \breif Brown Out Detector detection actions.
 *
 * List of possible BOD actions when a BOD module detects a brown-out condition.
 */
enum bod_action {
	/** A BOD detect will reset the device. */
	BOD_ACTION_RESET     = SYSCTRL_BOD33_ACTION(1),
	/** A BOD detect will fire an interrupt. */
	BOD_ACTION_INTERRUPT = SYSCTRL_BOD33_ACTION(2),
};

/**
 * \breif Brown Out Detector sampling modes.
 *
 * List of possible BOD module voltage sampling modes.
 */
enum bod_mode {
	/** BOD will sample the supply line continuously. */
	BOD_MODE_CONTINIOUS  = 0,
	/** BOD will use the BOD sampling clock (1kHz) to sample the supply line. */
	BOD_MODE_SAMPLED     = SYSCTRL_BOD33_MODE,
};

/**
 * Configuration struct for the BOD12 and BOD33
 */
struct bod_config {
	/** Sampled or continuous mode */
	enum bod_mode mode;
	/** Action on detect; reset or interrupt */
	enum bod_action action;
	/** BOD level */
	uint8_t level; /* TODO: document this more */
	/** Enable hysteresis */
	bool hysteresis;
};


/**
 * \name BOD configuration
 * @{
 */

/**
 * \brief Get default BOD configuration
 *
 * The default BOD configuration is:
 * - Continuous mode
 * - Reset on BOD detect
 * - Hysteresis enabled
 * - BOD level 0x12
 *
 * \param[out] conf BOD configuration struct to set to default settings
 */
static inline void bod_get_config_defaults(
		struct bod_config *const conf)
{
	/* Sanity check arguments */
	Assert(conf);

	conf->mode       = BOD_MODE_CONTINIOUS;
	conf->action     = BOD_ACTION_RESET;
	conf->level      = 0x12; /* TODO: Need to revisit this level */
	conf->hysteresis = true;
}

enum status_code bod_set_config(
		const enum bod bod,
		struct bod_config *const conf);

/**
 * @}
 */

#endif /* BOD_H_INCLUDED */
