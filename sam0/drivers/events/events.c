/*
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

#define EVENTS_START_OFFSET_BUSY_BITS		8
#define EVENTS_START_OFFSET_USER_READY_BIT	0

struct _events_module {
	volatile uint32_t allocated_channels;
	uint8_t           free_channels;
};

struct _events_module _events_inst = {
		.allocated_channels = 0,
		.free_channels      = EVSYS_CHANNELS,
};

static inline uint8_t _events_find_bit_position(uint8_t channel, uint8_t start_ofset)
{
	uint8_t byte_ofset = channel >> 3;
	uint32_t pos;

	pos = (((channel % 8) + 1) << start_ofset) * ((0xffff * byte_ofset) + 1);

	return pos;
}

static uint8_t _events_find_first_free_channel_and_allocate(void)
{
	uint8_t count;
	uint32_t tmp;
	bool allocated = false;

	system_interrupt_enter_critical_section();

	tmp = _events_inst.allocated_channels;

	for(count = 0; count < EVSYS_CHANNELS; ++count) {

		if(!(tmp & 0x00000001)) {
			/* If free channel found, set as allocated and return number */

			_events_inst.allocated_channels |= 1 << count;
			_events_inst.free_channels--;
			allocated = true;

			break;

		}

		tmp = tmp >> 1;
	}

	system_interrupt_leave_critical_section();

	if(!allocated) {
		return EVENTS_INVALID_CHANNEL;
	} else {
		return count;
	}
}

static void _events_release_channel(uint8_t channel)
{
	system_interrupt_enter_critical_section();

	_events_inst.allocated_channels &= ~(1 << channel);
	_events_inst.free_channels++;

	system_interrupt_leave_critical_section();
}

void events_get_config_defaults(struct events_config *config)
{
	/* Check that config is something other than NULL */
	Assert(config);

	config->edge_detect = EVENTS_EDGE_DETECT_NONE;
	config->path        = EVENTS_PATH_ASYNCHRONOUS;
	config->generator   = EVSYS_ID_GEN_NONE;
}

enum status_code events_allocate(
		struct events_descriptor *descriptor,
		struct events_config *config)
{
	uint8_t new_channel;

	Assert(descriptor);

	new_channel = _events_find_first_free_channel_and_allocate();

	if(new_channel == EVENTS_INVALID_CHANNEL) {
		return STATUS_ERR_NOT_FOUND;
	}

	descriptor->channel = new_channel;

	EVSYS->CHANNEL.reg = EVSYS_CHANNEL_CHANNEL(new_channel)       |
			     EVSYS_CHANNEL_EVGEN(config->generator)   |
			     EVSYS_CHANNEL_PATH(config->path)         |
			     EVSYS_CHANNEL_EDGSEL(config->edge_detect);

	return STATUS_OK;
}


enum status_code events_release(struct events_descriptor *descriptor)
{
	Assert(descriptor);

	/* Check if channel is busy */
	if(events_is_busy(descriptor)) {
		return STATUS_BUSY;
	}

	if (!(_events_inst.allocated_channels & (1<<descriptor->channel))) {
		return STATUS_ERR_NOT_INITIALIZED;
	}

	_events_release_channel(descriptor->channel);

	return STATUS_OK;
}

enum status_code events_trigger(struct events_descriptor *descriptor)
{
	Assert(descriptor);

	((uint8_t*)&EVSYS->CHANNEL)[1] = (uint8_t)EVSYS_CHANNEL_CHANNEL(descriptor->channel);

	EVSYS->CHANNEL.reg |= EVSYS_CHANNEL_CHANNEL(descriptor->channel) |
			      EVSYS_CHANNEL_SWEVT;

	return STATUS_OK;
}

bool events_is_busy(struct events_descriptor *descriptor)
{
	Assert(descriptor);

	return EVSYS->CHSTATUS.reg & (_events_find_bit_position(descriptor->channel, EVENTS_START_OFFSET_BUSY_BITS));
}

bool events_is_users_ready(struct events_descriptor *descriptor)
{
	Assert(descriptor);

	return EVSYS->CHSTATUS.reg & (_events_find_bit_position(descriptor->channel, EVENTS_START_OFFSET_USER_READY_BIT));
}

enum status_code events_attach_user(struct events_descriptor *descriptor, uint8_t user_id)
{
	Assert(descriptor);

	/* Channel number is n + 1 */
	EVSYS->USER.reg = EVSYS_USER_CHANNEL(descriptor->channel + 1) |
			  EVSYS_USER_USER(user_id);

	return STATUS_OK;
}

enum status_code events_deattach_user(struct events_descriptor *descriptor, uint8_t user_id)
{

	Assert(descriptor);

	/* Write 0 to the channel bit field to select no input */
	EVSYS->USER.reg = EVSYS_USER_USER(user_id);

	return STATUS_OK;
}

uint8_t events_get_free_channels()
{
	return _events_inst.free_channels;
}
