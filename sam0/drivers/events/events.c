/**
 * \file
 *
 * \brief SAMD20 Event System Controller Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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

#include <events.h>

/** \brief Initializes the event driver.
 *
 *  Initializes the event driver ready for use. This resets the underlying
 *  hardware modules, clearing any existing event channel configuration(s).
 */
void events_init(void)
{
	/* Software reset the module to ensure it is re-initialized correctly */
	EVSYS->CTRL.reg = EVSYS_CTRL_SWRST;

	while (EVSYS->CTRL.reg & EVSYS_CTRL_SWRST) {
	}
}

/** \brief Writes an Event System channel configuration to the hardware module.
 *
 *  Writes out a given configuration of a Event System channel configuration to
 *  the hardware module.
 *
 *  \param[in] channel  Event channel to configure
 *  \param[in] config   Configuration settings for the event channel
 */
void events_ch_set_config(
		const uint8_t channel,
		struct events_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Select and configure the event channel (must be done in one
	 * word-access write as specified in the module datasheet */
	EVSYS->CHANNEL.reg = (channel << EVSYS_CHANNEL_CHANNEL_Pos) |
			(config->generator_id << EVSYS_CHANNEL_EVGEN_Pos) |
			(config->edge_detection << EVSYS_CHANNEL_EDGSEL_Pos) |
			(config->path << EVSYS_CHANNEL_PATH_Pos);
}

/** \brief Writes an Event System user MUX configuration to the hardware module.
 *
 *  Writes out a given configuration of a Event System user MUX configuration to
 *  the hardware module.
 *
 *  \param[in] user    Event User MUX index to configure
 *  \param[in] config  Configuration settings for the event user MUX
 */
void events_user_set_config(
		const uint8_t user,
		struct events_user_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Select and configure the user MUX channel (must be done in one
	 * word-access write as specified in the module datasheet */
	EVSYS->USERMUX.reg = (user << EVSYS_USERMUX_UMUXSEL_Pos) |
			(config->event_channel_id << EVSYS_USERMUX_CHANNELEVENT_Pos);
}
