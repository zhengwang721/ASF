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

/**
 * \brief Edge detect enum
 *
 * Event channel edge detect setting 
 *
 */
enum events_edge_detect {
	/** No event output */
	EVENTS_NO_OUTPUT,
	/** Event on rising edge */
	EVENTS_RISING_EDGE,
	/** Event on falling edge */
	EVENTS_FALLING_EDGE,
	/** Event on both edges */
	EVENTS_BOTH_EDGES,
};

/**
 * \brief Path selection enum
 *
 * Event channel path selection
 *
 */
enum events_path_selection {
	/** Select the synchronous path for this event channel */
	EVENTS_PATH_SYNCHRONOUS,
	/** Select the resynchronizer path for this event channel */
	EVENTS_PATH_RESYNCHRONIZED,
	/** Select the asynchronous path for this event channel */
	EVENTS_PATH_ASYNCHRONOUS,
};

/**
 * \brief Events configuration struct
 *
 * This events configuration struct is used to configure each of the channels
 *
 */
struct events_config {
	/** Select edge detection mode */
	enum events_edge_detect    edge_detect;
	/** Select events channel path */
	enum events_path_selection path;
	/** Set event channel user peripheral */
	uint8_t                    user;
	/** Set event generator for the channel */
	uint8_t                    generator;
};

/**
 * \brief Event channel descriptor
 *
 */
struct events_descriptor {
	uint8_t channel;
};

/**
 * \brief Initializes an event configurations struct to defaults
 *
 * Initailize an event configuration struct to predefined default settings.
 *
 * \param[in] config Pointer to an instance of \c struct events_config
 *
 */
void events_get_config_defaults(struct events_config *config);

/**
 * \brief Allocate a event channel and set configuration
 *
 * Allocates a event channel from the even channel pool and sets
 * the channel configuration.
 *
 * \param[out] descriptor Pointer to a events_descriptor struct instance
 * \param[in]  config     Pointer to a events_config struct
 *
 * \return Status of the configuration procedure
 * \retval STATUS_OK            Allocation and configuration went successful
 * \retval STATUS_ERR_NOT_FOUND No free event channel found
 *
 */
void events_allocate(struct events_descriptor *descriptor, struct events_config *config);


/**
 * DOCUMENTATION DOES NOT MAKE SENSE
 * \brief Check if a channel is busy
 *
 * Check if a channel is busy 
 *
 * \param[in] descriptor Pointer to a events_descriptor struct instance
 *
 * \return Status of the channels busy state
 * \retval true  The event channel is busy
 * \retval false The event channel is not busy
 */
bool events_is_busy(struct events_descriptor *descriptor);

/**
 * \brief Trigger software event
 *
 * Trigger an event by software
 *
 * \param[in] descriptor Pointer to a events_descriptor struct
 *
 */
enum status_code events_trigger(struct events_descriptor *descriptor);

/**
 * \brief ????, documentation does not make sense
 */
bool events_is_user_ready(struct events_descriptor *descriptor);


/**
 * \brief Release allocated channel back the the resource pool
 *
 * Release an allocated channel back to the resource pool to make it availble for other purposes.
 *
 */
void events_release(struct events_descriptor *descriptor);

#endif /* EVENTS_H_INCLUDED */
