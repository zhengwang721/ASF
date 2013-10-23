/**
 * \file
 *
 * \brief SAM D2x Event System Driver
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
#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

enum event_edge_detect {
	EVENT_NO_OUTPUT,
	EVENT_RISING_EDGE,
	EVENT_FALLING_EDGE,
	EVENT_BOTH_EDGES,
};

enum event_path_selection {
	EVENT_PATH_SYNCHRONOUS,
	EVENT_PATH_RESYNCHRONIZED,
	EVENT_PATH_ASYNCHRONOUS,
};

struct event_config {
	enum event_edge_detect    edge_detect;
	enum event_path_selection path;
	uint8_t                   user;
	uint8_t                   generator;
};

struct event_descriptor {
	uint8_t channel;
};

void event_get_config_defaults(struct event_config *config);
void event_allocate(struct event_descriptor *descriptor, struct event_config *config);

void event_trigger(struct event_descriptor *descriptor);

void event_is_busy(struct event_descriptor *descriptor);
void event_is_user_ready(struct event_descriptor *descriptor);

void event_deallocate(struct event_descriptor *descriptor);

#endif /* EVENTS_H_INCLUDED */
