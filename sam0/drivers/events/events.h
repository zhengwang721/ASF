/**
 * \file
 *
 * \brief SAM0+ Event System Controller Driver
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
#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

/**
 * \defgroup sam0_events_group SAM0+ Event System Driver
 *
 * Driver for the SAM0+ architecture devices. This driver provides a unified
 * interface for the configuration and management of the peripheral event
 * channels and users within the device, including the enabling, disabling,
 * peripheral source selection and synchronization of clock domains between
 * various modules within the device. This driver encompasses the following
 * module within the SAM0 devices:
 *
 * \li \b EVSYS (Event System Management)
 *
 * Physically, the modules are interconnected within the device as shown in the
 * following diagram:
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="Source\nPeripherals" shape=ellipse style=filled fillcolor=lightgray] src_peripheral;
 *
 *   subgraph driver {
 *     node [label="<f0> EVSYS | <f1> Event Channels" style=outline shape=record] events_chan;
 *     node [label="<f0> EVSYS | <f1> Event Users" shape=record] events_user;
 *   }
 *
 *   node [label="Destination\nPeripherals" shape=ellipse style=filled fillcolor=lightgray] dst_peripheral;
 *
 *   src_peripheral -> events_chan:f1 [label="Source\nMUXs"];
 *   events_chan:f1  -> events_user:f1 [label="Channel\nMUXs"];
 *   events_user:f1  -> dst_peripheral;
 * }
 * \enddot
 *
 * \section module_introduction Introduction
 *
 * \par Event System Overview
 * Peripherals within the SAM0 devices are capable of generating two types of
 * actions in response to given stimulus; they can set a register flag for later
 * intervention by the CPU (using interrupt or polling methods), or they can
 * generate event signals which can be internally routed directly to other
 * peripherals within the device. The use of events allows for direct actions
 * to be performed in one peripheral in response to a stimulus in another
 * without CPU intervention. This can lower the overall power consumption of the
 * system if the CPU is able to remain in sleep modes for longer periods, and
 * lowers the latency of the system response.
 * \n\n
 * The event system is comprised of a number of freely configurable Event
 * Channels, plus a number of fixed Event Users. Each Event Channel can be
 * configured to select the input peripheral that will generate the events on
 * the channel, as well as the synchronisation path and edge detection mode.
 * The fixed-function Event Users, connected to peripherals within the device,
 * can then subscribe to an Event Channel in a one-to-many relationship in order
 * to receive events as they are generated.
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="Source\nPeripheral" shape=ellipse style=filled fillcolor=lightgray] src_peripheral;
 *   node [label="Event\nChannel a" shape=square style=outline] event_gen0;
 *   node [label="Event\nUser x" shape=square] event_user0;
 *   node [label="Event\nUser y" shape=square] event_user1;
 *   node [label="Destination\nPeripheral" shape=ellipse style=filled fillcolor=lightgray] dst_peripheral0;
 *   node [label="Destination\nPeripheral" shape=ellipse style=filled fillcolor=lightgray] dst_peripheral1;
 *
 *   src_peripheral -> event_gen0;
 *   event_gen0     -> event_user0;
 *   event_gen0     -> event_user1;
 *   event_user0    -> dst_peripheral0;
 *   event_user1    -> dst_peripheral1;
 * }
 * \enddot
 *
 * \par ""
 * There are many different events that can be routed in the device, which can
 * then trigger many different actions. For example, an Analog Comparator module
 * could be configured to generate an event when the input signal rises about
 * the compare threshold, which then triggers a Timer module to capture the
 * current count value for later use.
 *
 * \par Event Channels
 * The Event module in each device consists of several channels, which can be
 * freely linked to an event generator (i.e. a peripheral within the device
 * that is capable of generating events). Each channel can be individually
 * configured to select the generator peripheral, signal path and edge detection
 * applied to the input event signal, before being passed to any event user(s).
 * \n\n
 * Event channels can support multiple users within the device in a standardized
 * manner; when an Event User is linked to an Event Channel, the channel will
 * automatically handshake with all attached users to ensure that all modules
 * correctly receive and acknowledge the event.
 *
 * \par Event Users
 * Event Users are able to subscribe to an Event Channel, once it has been
 * configured. Each Event User consists of a fixed connection to one of the
 * peripherals within the device (for example, an ADC module or Timer module)
 * and is capable of being connected to a single Event Channel.
 *
 * \par Edge Detection
 * For asynchronous events, edge detection on the event input is not possible,
 * and the event signal must be passed directly between the event generator and
 * event user. For synchronous and re-synchronous events, the input signal from
 * the event generator must pass through an edge detection unit, so that only
 * the rising, falling or both edges of the event signal triggers an action in
 * the event user.
 *
 * \par Path Selection
 * The event system in the SAM0 devices supports three signal path types from
 * the event generator to event users: asynchronous, synchronous and
 * re-synchronous events.
 * \n\n
 * <b>Asynchronous event</b> paths allow for an asynchronous connection between
 * the event generator and event user(s), when the source and destination
 * peripherals share the same \ref sam0_gclk_group "generic clock" channel. In
 * this mode the event is propagated between the source and destination directly
 * to reduce the event latency, thus no edge detection is possible.
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="Source\nPeripheral" shape=ellipse style=filled fillcolor=lightgray] src_peripheral;
 *   node [label="<f0> EVSYS | <f1> Event\nChannel/User" style="outline, dashed" shape=record] events_chan;
 *   node [label="Destination\nPeripheral" shape=ellipse style=filled fillcolor=lightgray] dst_peripheral;
 *
 *   src_peripheral -> events_chan;
 *   events_chan     -> dst_peripheral;
 *
 *   label="(Note: identical shape borders indicate a shared generic clock channel)" [labelloc=b];
 * }
 * \enddot
 *
 * \par ""
 * <b>Synchronous event</b> paths can be used when the source and destination
 * peripherals, as well as the generic clock to the event system itself, use
 * different \ref sam0_gclk_group "generic clock" channels. This case introduces
 * additional latency in the event propagation due to the addition of a
 * synchronizer and edge detector on the input event signal, however this allows
 * modules of different clocks to communicate events to one-another.
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="Source\nPeripheral" shape=ellipse style="filled, dashed" fillcolor=lightgray] src_peripheral;
 *   node [label="<f0> EVSYS | <f1> Event\nChannel/User" style=outline shape=record] events_chan;
 *   node [label="Destination\nPeripheral" shape=ellipse style="filled, dotted" fillcolor=lightgray] dst_peripheral;
 *
 *   src_peripheral -> events_chan;
 *   events_chan     -> dst_peripheral;
 *
 *   label="(Note: identical shape borders indicate a shared generic clock channel)" [labelloc=b];
 * }
 * \enddot
 *
 * \par ""
 * <b>Re-synchronous event</b> paths are a special form of synchronous events,
 * where the event users share the same \ref sam0_gclk_group "generic clock"
 * channel as the event system module itself, but the event generator does not.
 * This reduces latency by performing the synchronisation acros the event source
 * and event user clock domains once within the event channel itself, rather
 * than in each event user.
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="Source\nPeripheral" shape=ellipse style="filled, dashed" fillcolor=lightgray] src_peripheral;
 *   node [label="<f0> EVSYS | <f1> Event\nChannel/User" style=outline shape=record] events_chan;
 *   node [label="Destination\nPeripheral" shape=ellipse style=filled fillcolor=lightgray] dst_peripheral;
 *
 *   src_peripheral -> events_chan;
 *   events_chan     -> dst_peripheral;
 *
 *   label="(Note: identical shape borders indicate a shared generic clock channel)" [labelloc=b];
 * }
 * \enddot
 *
 * \section module_dependencies Dependencies
 * The event system driver has the following dependencies.
 *
 * \li None
 *
 * \section special_considerations Special Considerations
 *
 * For extra information see \ref events_extra_info.
 *
 * \section module_examples Examples
 * - \ref events_quickstart
 *
 * \section api_overview API Overview
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Event System synchronous channel edge detection configurations.
 *
 *  Enum containing the possible event channel edge detection configurations,
 *  to select when the synchronous event triggers according to a particular
 *  trigger edge.
 *
 *  \note For asynchronous events, edge detection is not possible and selection
 *        of any value other than \ref EVENT_EDGE_NONE will have no effect. For
 *        synchronous events, a valid edge detection mode other than
 *        \ref EVENT_EDGE_NONE must be set for events to be generated.
 */
enum events_edge {
	/** Event channel disabled (or direct passthrough for asynchronous
	 *  events). */
	EVENT_EDGE_NONE    = 0,
	/** Event channel triggers on rising edges. */
	EVENT_EDGE_RISING  = 1,
	/** Event channel triggers on falling edges. */
	EVENT_EDGE_FALLING = 2,
	/** Event channel triggers on both edges. */
	EVENT_EDGE_BOTH    = 3,
};

/** \brief Event System channel path type configurations.
 *
 *  Enum containing the possible event channel paths, to select between
 *  digital clock sychronisation settings for each channel.
 */
enum events_path {
	/** Event is synchronised to the digital clock. */
	EVENT_PATH_SYNCHRONOUS   = 0,
	/** Event is re-synchronised between the source and destination digital
	 *  clock domains. */
	EVENT_PATH_RESYNCHRONOUS = 1,
	/** Event is asynchronous to the digital clock. */
	EVENT_PATH_ASYNCHRONOUS  = 2,
};

/** \brief Event System Channel configuration structure.
 *
 *  Configuration structure for an Event System channel. This structure
 *  should be initialized by the \ref events_ch_get_config_defaults() function
 *  before being modified by the user application.
 */
struct events_ch_conf {
	/** Edge detection for synchronous event channels, from \ref events_edge. */
	enum events_edge edge_detection;
	/** Path of the event system, from \ref events_path. */
	enum events_path path;
	/** Event generator module that should be attached to the event channel */
	uint8_t generator_id;
};

/** \brief Event System user MUX configuration structure.
 *
 *  Configuration structure for an Event System subscriber multiplexer
 *  channel. This structure should be initialized by the
 *  \ref events_user_get_config_defaults() function before being modified by the
 *  user application.
 */
struct events_user_conf {
	/** Event channel ID that should be attached to the user MUX */
	uint8_t event_channel_id;
};


/** \name Configuration and initialization
 * @{
 */

void events_init(void);

/** @} */


/** \name Configuration and initialization (Event Channel)
 * @{
 */

/** \brief Initializes an Event System configuration structure to defaults.
 *
 *  Initializes a given Event System channel configuration structure to a
 *  set of known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Event channel fires on rising edge of trigger
 *   \li Event channel is synchronised between the source and destination
 *       event system digital clocks
 *   \li Event channel is not connected to an Event Generator
 *
 *  \param config    Configuration structure to initialize to default values
 */
static inline void events_ch_get_config_defaults(
		struct events_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->edge_detection = EVENT_EDGE_RISING;
	config->path           = EVENT_PATH_SYNCHRONOUS;
	config->generator_id   = 0;
}

void events_ch_set_config(const uint8_t channel,
		struct events_ch_conf *const config);

/** @} */


/** \name Configuration and initialization (Event User)
 * @{
 */

/** \brief Initializes an Event System user MUX configuration structure to defaults.
 *
 *  Initializes a given Event System user MUX configuration structure to a
 *  set of known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li User MUX input event is not connected to any source channel
 *
 *  \param config  Configuration structure to initialize to default values
 */
static inline void events_user_get_config_defaults(
		struct events_user_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->event_channel_id = 0;
}

void events_user_set_config(
		const uint8_t user,
		struct events_user_conf *const config);

/** @} */


/** \name Channel Control and Management
 * @{
 */

/** \brief Retrieves the busy status of an Event channel.
 *
 *  Reads the status of the requested Event channel, to determine if the
 *  channel is currently busy.
 *
 *  \pre The specified event channel must be configured and enabled.
 *
 *  \param[in] channel  Event channel to check
 *
 *  \return Status of the specified event channel.
 *
 *  \retval true  If the channel is ready to be used
 *  \retval false If the channel is currently busy
 */
static inline bool events_ch_is_ready(
		const uint8_t channel)
{
	/* Event channel busy/user busy flags are interleaved, 8 channels to a
	 * 16-bit word */
	uint8_t status_halfword = channel / 8;
	uint8_t status_bitindex = channel % 8;

	/* Determine if the specified channel is currently busy */
	if (EVSYS.CHSTATUS_UINT16_T[status_halfword] &
			(EVSYS_CHBUSY0_bp << status_bitindex)) {
		return false;
	}

	return true;
}

/** \brief Retrieves the channel status of the users subscribed to an Event channel.
 *
 *  Reads the status of the requested Event channel users, to determine if the
 *  users of the event channel are currently busy.
 *
 *  \pre The specified event channel must be configured and enabled.
 *
 *  \param[in] channel  Event channel to check
 *
 *  \return Status of the specified event channel subscribers.
 *
 *  \retval true  If all channel subscribers are ready
 *  \retval false If one or more channel subscribers are currently busy
 */
static inline bool events_user_is_ready(
		const uint8_t channel)
{
	/* Event channel busy/user busy flags are interleaved, 8 channels to a
	 * 16-bit word */
	uint8_t status_halfword = channel / 8;
	uint8_t status_bitindex = channel % 8;

	/* Determine if the specified channel users(s) are currently ready */
	if (EVSYS.CHSTATUS_UINT16_T[status_halfword] &
			(EVSYS_USRREADY0_bp << status_bitindex)) {
		return true;
	}

	return false;
}

/** \brief Software triggers an event channel.
 *
 *  Triggers an event channel via software, seting an event notification to
 *  the channel subscriber module(s) of the channel.
 *
 *  \pre The specified event channel must be configured and enabled.
 *
 *  \param[in] channel  Event channel to trigger
 */
static inline void events_ch_software_trigger(
		const uint8_t channel)
{
	/* Trigger the appropriate event channel - must be performed as a single
	 * 8-bit write as mandated in the datasheet for the event system module */
	EVSYS.CHCTRL = (channel << EVSYS_CHSEL_gp) | EVSYS_STROBE_bm;
}

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page events_extra_info Extra Information
 *
 * \section acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>CPU</td>
 *		<td>Central Processing Unit</td>
 *	</tr>
 *	<tr>
 *		<td>MUX</td>
 *		<td>Multiplexer</td>
 *	</tr>
 * </table>
 *
 * \section fixed_errata Erratas fixed by driver
 * No errata workarounds in driver.
 *
 * \section module_history Module History
 * Below is an overview of the module history, detailing enhancements and fixes
 * made to the module since its first release. The current version of this
 * corresponds to the newest version listed in the table below.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page events_quickstart Quick Start Guides for the Event System module
 *
 * This is the quick start guide list for the \ref sam0_events_group module, with
 * step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function of the
 * user application and run at system startup, while the steps for usage can be
 * copied into the normal user application program flow.
 *
 * \see General list of module \ref module_examples "examples".
 *
 * \section events_use_cases Event System module use cases
 * - \subpage events_basic_use_case
 */

#endif
