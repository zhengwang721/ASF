/**
 * \file
 *
 * \brief SAM D2x Event System Controller Driver
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

#include <events.h>
#include <system.h>
#include <system_interrupt.h>
#include <status_codes.h>

#define EVENTS_INVALID_CHANNEL			0xff

#define EVENTS_START_OFSET_BUSY_BITS		8
#define EVENTS_START_OFSET_USER_READY_BIT	0

volatile uint32_t _events_allocated_channels = 0;

static inline uint8_t _events_find_bit_position(uint8_t channel, uint8_t start_ofset)
{
	uint8_t byte_ofset = channel >> 3;
	
	pos = (((channel % 8) + 1) << start_ofset) * ((0xffff * byte_ofset) + 1)

	return pos;
}

static uint8_t _events_find_first_free_channel_and_allocate()
{
	uint8_t count;
	uint32_t tmp;
	bool allocated = false;

	system_interrupt_enter_critcal_section();

	tmp = _events_allocated_channels;

	for(count = 0; count < EVSYS_CHANNELS; ++count) {

		if(!(tmp & 0x00000001)) {
			/* If free channel found, set as allocated and return number */

			_events_allocated_channels |= 1 << count;
			allocated = true;

			break;

		}

		tmp = tmp >> 1;
	}

	system_interrupt_leave_critcal_section();

	if(!allocated) {
		return EVENTS_INVALID_CHANNEL;
	} else {
		return count;
	}
}

static void _events_release_channel(uint8_t channel)
{
	system_interrupt_enter_critical_section();

	_events_allocated_channles &= ~(1 << channel);

	system_interrupt_leave_critical_section();
}

void events_get_config_defaults(struct events_config *config)
{
	/* Check that config is something other than NULL */
	Assert(config);

	config->edge_detect = EVENTS_RISING_EDGE;
	config->path        = EVENTS_PATH_ASYNCHRONOUS;
	config->user        = EVSYS_ID_USER_ADC_START;
	config->generator   = EVSYS_ID_GEN_RTC_OVF;  
}

enum status_code events_allocate(
		struct events_descriptor *descriptor,
		struct events_config *config)
{
	Assert(descriptor);

	new_channel = _events_find_first_free_channel_and_allocate();

	if(new_channel == EVENTS_INVALID_CHANNEL) {
		return STATUS_ERR_NOT_FOUND;
	}

	descriptor->channel = new_channel;

	EVSYS.CHANNEL = EVSYS_CHANNEL_CHANNEL(new_channel)       |
					EVSYS_CHANNEL_EVGEN(config->generator)   |
					EVSYS_CHANNEL_PATH(config->path)         |
					EVSYS_CHANNEL_EDGSEL(config->edge_detect);

	EVSYS.USER    = EVSYS_USER_CHANNEL(new_channel) | 
					EVSYS_USER_USER(user);

	return STATUS_OK
}


enum status_code events_release(struct events_descriptor *descriptor)
{
	Assert(descriptor);

	/* Check if channel is busy */
	if(EVSYS.CHSTATUS & (1 << _events_find_bit_position(descriptor->channel, EVENTS_START_OFSET_BUSY_BITS))) {
		return STATUS_BUSY;
	}

	_events_release_channel(descriptor->channel);

	return STATUS_OK;
}

void events_trigger(struct events_descriptor *descriptor)
{
	Assert(descriptor);

	(uint8_t)EVSYS.CHANNEL = (uint8_t)EVSYS_CHANNEL_CHANNEL(descriptor->channel);

	EVSYS.CHANNEL |= EVSYS_CHANNEL_CHANNEL(descriptor->channel) |
					 EVSYS_CHANNEL_SWEVT;

	return STATUS_OK;
}

bool events_is_busy(struct events_descriptor *descriptor)
{
	Assert(descriptor);

	return EVSYS.CHSTATUS & (1 << _events_find_bit_position(descriptor->channel, EVENTS_START_OFSET_BUSY_BITS));
}

bool events_is_user_ready(struct events_descriptor *descriptor)
{
	Assert(descriptor);

	return EVSYS.CHSTATUS & (1 << _events_find_bit_position(descriptor->channel, EVENTS_START_OFSET_USER_READY_BITS));
}
