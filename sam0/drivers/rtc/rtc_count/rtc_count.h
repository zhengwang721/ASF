/**
 * \file
 *
 * \brief SAM0+ RTC Driver for count mode
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#ifndef RTC_COUNT_H_INCLUDED
#define RTC_COUNT_H_INCLUDED

#include <conf_clocks.h>

#if CONF_CLOCK_GCLK_2_RTC == false
# error "conf_clocks.h must be set up for the RTC."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_rtc_count_group SAM0+ Real Time Counter (RTC), Counter Mode
 * Driver for the SAM0+ architecture devices. This driver provides a
 * interface for setting up and utilizing the RTC in counter mode. This module
 * encompasses the following modules within the SAM0+ devices:
 * - \b RTC (Real Time Counter)
 *
 * \section rtc_count_intro Introduction
 * The RTC is a 32-bit counter with a 10-bit programmable prescaler that
 * typically runs continuously, including in low-power sleep modes, to track
 * time. The RTC can be used as a source to wake up the system using the \ref
 * rtc_count_compares "Compares, Overflows" or \ref rtc_count_periodic triggers.
 *
 * For this driver the RTC is operated in count mode. This allows for an
 * easy integration of a counter to your project that will operate in most sleep
 * modes as long as the RTC_GCLK is enabled. Alternatively the RTC can be
 * operated in a convenient \ref sam0_rtc_calendar_group "Calendar" mode if
 * a convenient clock integration is needed.
 *
 * For the count mode, the RTC features the following for the two operation
 * modes.
 * <table>
 *   <tr>
 *     <th>Mode</th><th>Clear On Match</th>
 *     <th>Selectable Period</th><th>Compares</th>
 *   </tr>
 *   <tr>
 *     <td>16 bit</td><td>-</td><td>x</td><td>6</td>
 *   </tr>
 *   <tr>
 *      <td>32 bit</td><td>x</td><td>-</td><td>4</td>
 *    </tr>
 * </table>
 *
 * \subsection rtc_count_compares Compare and Overflow
 * The RTC can be used with up to 4/6 compare (depending on selected operation
 * mode). These alarms will be will trigger on match with the current RTC
 * counter value, and can be set up to trigger an interrupt, an event, or both.
 * The RTC can also be configured to clear the counter value on compare match
 * in 32 bit mode, meaning the counter value of the RTC will be set to 0.
 *
 * If the RTC is operated without the \ref rtc_count_conf "clear on match"
 * option enabled, or in 16 bit mode, the RTC counter value will instead be
 * cleared on overflow once the maximum count value has been reached
 * (\f$ 2^{32}-1 \f$ for 32 bit, and \f$ 2^{16}-1 \f$ for 16 bit). Below is a
 * figure showing the concept of the two operation modes.
 *
 * \dot
 * digraph compare_match_off {
 *     splines = false;
 *     rankdir=LR;
 *     subgraph cluster_a {
 *         node [shape=none];
 *         style="invis";
 *         overflow1 [label=""];
 *         match1 [label=""];
 *         start1 [label="Start"];
 *         overflow1:e -> match1 [style="invis", constraint=false];
 *         match1 -> start1 [style="invis", constraint=false];
 *         start1:n -> overflow1:n [constraint=false, headlabel=
 *         "counter              ", fontsize=20];
 *         {rank="same"; overflow1 match1 start1}
 *     }
 *     subgraph cluster_b {
 *         node [shape=none];
 *         style="invis";
 *         overflow2 [label=""];
 *         match2 [label="Match"];
 *         start2 [label=""];
 *         overflow2 -> match2 [style="invis", constraint=false];
 *         match2 -> start2 [style="invis", constraint=false];
 *         {rank="same"; overflow2 match2 start2 }
 *     }
 *     subgraph cluster_c {
 *         node [shape=none];
 *         style="invis";
 *         overflow3 [label="Overflow"];
 *         match3 [label=""];
 *         start3 [label="Start"];
 *         time [fontsize=20, labelloc="t"];
 *         overflow3 -> match3 [style="invis", constraint=false];
 *         match3 -> start3 [style="invis", constraint=false];
 *         overflow3-> start3 [constraint=false, weight=1];
 *         {rank="same"; overflow3 start3 }
 *     }
 *     subgraph cluster_d {
 *         node [shape=none];
 *         style="invis";
 *         overflow4 [label=""];
 *         match4 [label="Match"];
 *         start4 [label=""];
 *         overflow4 -> match4 [style="invis", constraint=false];
 *         match4 -> start4 [style="invis", constraint=false];
 *         {rank="same"; overflow4 match4 start4 }
 *     }
 *     subgraph cluster_e {
 *         node [shape=none];
 *         style="invis";
 *         overflow5 [label="Overflow"];
 *         match5 [label=""];
 *         start5 [label="Start"];
 *         zeroline5 [label=""];
 *         overflow5 -> match5 [style="invis", constraint=false];
 *         match5 -> start5 [style="invis", constraint=false];
 *         overflow5-> start5 [constraint=false, weight=1];
 *         {rank="same"; overflow5 start5 }
 *     }
 *     start1:n -> match2 [weight=0, constraint=false];
 *     match2 -> overflow3:s [weight=0, constraint=false];
 *     start3:n -> match4 [weight=0, constraint=false];
 *     match4 -> overflow5:s [weight=0, constraint=false];
 *     match1:w -> match5:e [style="dotted", constraint=false, dir="none"];
 *     start1:n -> start5:ne [constraint=false];
 *
 *     start1 -> start2 [style="invis"];
 *     start2 -> start3 [style="invis"];
 *     start3 -> start4 [style="invis"];
 *     start4 -> start5 [style="invis"];
 *
 *     label = "Counter with clear on match deactivated.";
 * }
 * \enddot
 *
 * \dot
 * digraph compare_match_on {
 *     splines = false;
 *     rankdir=LR;
 *     subgraph cluster_a {
 *         node [shape=none];
 *         style="invis";
 *         overflow1 [label=""];
 *         match1 [label=""];
 *         start1 [label="Start "];
 *         match1 -> start1 [style="invis", constraint=false];
 *         start1:n -> overflow1:n [constraint=false, headlabel=
 *         "counter              ", fontsize=20];
 *         {rank="same"; match1 start1}
 *     }
 *     subgraph cluster_b {
 *         node [shape=none];
 *         style="invis";
 *         match2 [label="Match"];
 *         start2 [label="Start "];
 *         match2 -> start2 [constraint=false];
 *         {rank="same"; match2 start2 }
 *     }
 *     subgraph cluster_c {
 *         node [shape=none];
 *         style="invis";
 *         match3 [label="Match    "];
 *         start3 [label="Start "];
 *         time [fontsize=20, labelloc="t"];
 *         start3 -> match3 [dir="back", constraint=false];
 *         {rank="same"; start3 }
 *     }
 *     subgraph cluster_d {
 *         node [shape=none];
 *         style="invis";
 *         match4 [label="Match"];
 *         start4 [label="Start "];
 *         match4 -> start4 [constraint=false];
 *         {rank="same"; match4 start4 }
 *     }
 *     subgraph cluster_e {
 *         node [shape=none];
 *         style="invis";
 *         match5 [label="Match"];
 *         start5 [label="Start "];
 *         zeroline5 [label=""];
 *         match5 -> start5 [constraint=false];
 *         {rank="same"; start5 }
 *     }
 *     start1:n -> match2 [weight=0, constraint=false];
 *     start2:n -> match3 [weight=0, constraint=false];
 *     start3:n -> match4 [weight=0, constraint=false];
 *     start4:n -> match5 [weight=0, constraint=false];
 *
 *     match1:w -> match5:e [style="dotted", constraint=false, dir="none"];
 *     start1:n -> start5:ne [constraint=false];
 *
 *     start1 -> start2 [style="invis"];
 *     start2 -> start3 [style="invis"];
 *     start3 -> start4 [style="invis"];
 *     start4 -> start5 [style="invis"];
 *
 *     label = "Counter with clear on match activated.";
 * }
 *
 * \enddot
 *
 * In 16 bit mode, the overflow value is selectable with a period value.
 * The overflow will then occur when the counter value reaches the value
 * specified for the \ref rtc_count_set_period "period".
 *
 * \subsection rtc_count_periodic Periodic Events
 * The RTC can generate events at periodic intervals, allowing for flexible
 * system ticks. The periodic events can be generated on the upper 8 bits of the
 * RTC prescaler, and will be generated on the rising edge transition of the
 * specified bit. The resulting periodic frequency can be calculated by the
 * following formula:
 * \f[
 * f_{PERIODIC}=\frac{f_{ASY}}{2^{n+3}}
 * \f]
 * Where \f$f_{ASY}\f$ refers to the <EM>asynchronous</EM>
 * \ref rtc_count_clock_conf "clock" set up in the \ref sam0_gclk_group "GCLK"
 * configurations. By default, the GCLK_RTC will run at 1024 Hz. The parameter
 * <EM>n</EM> is the periodic event source specified and is available as an
 * enum in the \ref rtc_count_events "API". If the \f$f_{ASY}\f$ is
 * operated at the default 1024 Hz, the formula gives you:
 * <table>
 * <tr>
 * <th>n</th><th>Periodic event</th>
 * </tr>
 * <tr>
 * <td>7</td><td>1 Hz</td>
 * </tr>
 * <tr>
 * <td>6</td><td>2 Hz</td>
 * </tr>
 * <tr>
 * <td>5</td><td>4 Hz</td>
 * </tr>
 * <tr>
 * <td>4</td><td>8 Hz</td>
 * </tr>
 * <tr>
 * <td>3</td><td>16 Hz</td>
 * </tr>
 * <tr>
 * <td>2</td><td>32 Hz</td>
 * </tr>
 * <tr>
 * <td>1</td><td>64 Hz</td>
 * </tr>
 * <tr>
 * <td>0</td><td>128 Hz</td>
 * </tr>
 * </table>
 *
 * \subsection rtc_count_clock_conf Clock Setup
 * The RTC is typically clocked by a specialized GCLK generator that has a
 * smaller prescaler than the others. To make it easier for the implementer,
 * the clock is handled by setting the desired end-frequency in the
 * \ref sam0_clock_group "clocks configuration file". By default the RTC clock
 * is on, selected to use the internal 32 kHz RC-oscillator with a prescaler of
 * 32, giving a clock frequency of 1024 Hz to the RTC. The RTC prescaler is set
 * to 1024 which yields an end-frequency of 1 Hz.
 *
 * The implementer also has the option to set other end-frequencies. The table
 * below lists the available RTC frequencies for each possible GCLK and RTC
 * input prescaler options.
 * <table>
 *   <tr>
 *     <th>End-frequency</th>
 *     <th>GCLK prescaler</th>
 *     <th>RTC Prescaler</th>
 *   </tr>
 *   <tr>
 *     <td>32kHz</td>
 *     <td>1</td>
 *     <td>1</td>
 *   </tr>
 *   <tr>
 *     <td>1kHz</td>
 *     <td>32</td>
 *     <td>1</td>
 *   </tr>
 *   <tr>
 *     <td>1Hz</td>
 *     <td>32</td>
 *     <td>1024</td>
 *   </tr>
 * </table>
 *
 * The overall RTC module clocking scheme is shown below.
 * \dot
 * digraph clocking_scheme {
 *     rankdir=LR;
 *     GCLK [shape="record", label="<f0> GCLK | <f1> RTC_GCLK",
 *         bgcolor="lightgray", style="filled"];
 *     RTCPRE [shape="record" label="<f0> RTC | <f1> RTC PRESCALER"];
 *     RTC [shape="record", label="<f0> RTC | <f1> RTC CLOCK"];
 *
 *     GCLK:f1 -> RTCPRE:f1;
 *     RTCPRE:f1 -> RTC:f1;
 * }
 * \enddot
 *
 * \subsection rtc_count_correction Digital Frequency Correction
 * The RTC can employ counter corrections to compensate for inaccurate
 * oscillators. The correction scheme requires that at least two bits
 * in the RTC prescaler are used. Because of this, the correction will only be
 * available in 1Hz mode.
 *
 * The correction procedure is implemented by subtracting or adding a single
 * cycle from the RTC prescaler every 1024 RTC_GCLK cycles. The adjustment is
 * applied the specified number of times (max 127 in either direction) over 976
 * of these periods. The corresponding correction in PPM will be given by:
 * \f[
 * Correction(PPM) = \frac{VALUE}{999424}10^6
 * \f]
 * To specify if the correction should be applied to make the a faster or slower
 * clock, the implementer must provide a positive number up to 127 to make the
 * clock go faster, or a negative number down to -127 for a slower clock.
 *
 * \section rtc_count_dependencies Dependencies
 * The RTC Count has the following dependencies:
 * - \b GCLK (Generic Clock Management)
 * - \b SYSCTRL (Clock source control)
 *
 * \section rtc_count_special_considerations Special Considerations
 * There are no special considerations.
 *
 * \subsection rtc_count_extra_info Extra Information
 * See \ref rtc_count_extra_info_page
 *
 * \section rtc_count_examples Examples
 * - \ref rtc_count_quickstart
 *
 * \section rtc_count_api_overview API Overview
 * @{
 */

/**
 * \brief Available operation modes for the RTC.
 */
enum rtc_count_mode {
	/** To operate in 16 bit mode. */
	RTC_COUNT_MODE_16BIT = 0,
	/** To operate in 32 bit mode. */
	RTC_COUNT_MODE_32BIT = 1,
};

/**
 * \brief Available compare registers.
 */
enum rtc_count_compare {
	/** Compare register 0. */
	RTC_COUNT_COMPARE_0 = 0,
	/** Compare register 1. */
	RTC_COUNT_COMPARE_1 = 1,
	/** Compare register 2. */
	RTC_COUNT_COMPARE_2 = 2,
	/** Compare register 3. */
	RTC_COUNT_COMPARE_3 = 3,
	/** Compare register 4. Only available in 16 bit mode */
	RTC_COUNT_COMPARE_4 = 4,
	/** Compare register 5. Only available in 16 bit mode */
	RTC_COUNT_COMPARE_5 = 5,
};

/**
 * \brief Values used to enable and disable events.
 *
 * Use these values when disabling or enabling the RTC events.
 */
enum rtc_count_events {
	/** To set event off. */
	RTC_COUNT_EVENT_OFF        = 0,
	/** Overflow event. */
	RTC_COUNT_EVENT_OVF        = RTC_EVENT_OVF_bm,
	/** Compare 0 match event. */
	RTC_COUNT_EVENT_CMP_0       = RTC_EVENT_CMP_0_bm,
	/** Compare 1 match event. */
	RTC_COUNT_EVENT_CMP_1       = RTC_EVENT_CMP_1_bm,
	/** Compare 2 match event. */
	RTC_COUNT_EVENT_CMP_2       = RTC_EVENT_CMP_2_bm,
	/** Compare 3 match event. */
	RTC_COUNT_EVENT_CMP_3       = RTC_EVENT_CMP_3_bm,
	/** Compare 4 match event. Only available in 16 bit mode. */
	RTC_COUNT_EVENT_CMP_4       = RTC_EVENT_CMP_4_bm,
	/** Compare 5 match event. Only available in 16 bit mode. */
	RTC_COUNT_EVENT_CMP_5       = RTC_EVENT_CMP_5_bm,
	/** To set periodic event 0. */
	RTC_COUNT_EVENT_PERIODIC_0 = RTC_EVENT_PER_0_bm,
	/** Periodic event 1. */
	RTC_COUNT_EVENT_PERIODIC_1 = RTC_EVENT_PER_1_bm,
	/** Periodic event 2. */
	RTC_COUNT_EVENT_PERIODIC_2 = RTC_EVENT_PER_2_bm,
	/** Periodic event 3. */
	RTC_COUNT_EVENT_PERIODIC_3 = RTC_EVENT_PER_3_bm,
	/** Periodic event 4. */
	RTC_COUNT_EVENT_PERIODIC_4 = RTC_EVENT_PER_4_bm,
	/** Periodic event 5. */
	RTC_COUNT_EVENT_PERIODIC_5 = RTC_EVENT_PER_5_bm,
	/** Periodic event 6. */
	RTC_COUNT_EVENT_PERIODIC_6 = RTC_EVENT_PER_6_bm,
	/** Periodic event 7. */
	RTC_COUNT_EVENT_PERIODIC_7 = RTC_EVENT_PER_7_bm,
};

/**
 * \brief RTC configuration structure
 *
 * Configuration structure for the RTC instance. This structure should
 * be initialized using the \ref rtc_count_get_config_defaults() before any
 * user configurations are set.
 */
struct rtc_count_conf {
	/** Select the operation mode of the RTC.*/
	enum rtc_count_mode mode;
	/** If true, clears the counter value on compare match. Only
	 * available in 32 bit mode. */
	bool clear_on_match;
	/** Continuously update the counter value so no synchronization is
	 * needed for reading. */
	bool continuously_update;
	/** Set event output generators. */
	enum rtc_count_events event_generators;
	/** Array of Compare values. Compare register 4 and 5 are only
	 * available in 16 bit mode. */
	uint32_t compare_values[6];
};

/**
 * \name Configuration and initialization
 * @{
 */

#if !defined(__DOXYGEN__)
/**
 * \internal Function to wait for sync busy flag to clear.
 */
static inline void _rtc_count_wait_for_sync(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	while(rtc_module->STATUS & RTC_SYNC_BUSY_bm){
		/* Wait for RTC to sync. */
	}
}
#endif

/**
 *  \brief Gets the RTC default configurations.
 *
 *  Initializes the configuration structure to default values. This
 *  function should be called at the start of any RTC initialization.
 *
 *  The default configuration is as follows:
 *  - RTC in 32 bit mode.
 *  - Clear on compare match off.
 *  - Continuously sync count register off.
 *  - No event source on.
 *  - All compare values equal 0.
 *
 *  \param[out] config Configuration structure to be initialized to default
 *  values.
 */
static inline void rtc_count_get_config_defaults(
		struct rtc_count_conf *const config)
{
	/* Sanity check argument */
	Assert(config);

	/* Set default into configuration structure */
	config->mode = RTC_COUNT_MODE_32BIT;
	config->clear_on_match = false;
	config->continuously_update = false;
	config->event_generators = RTC_COUNT_EVENT_OFF;
	config->compare_values[0] = 0;
	config->compare_values[1] = 0;
	config->compare_values[2] = 0;
	config->compare_values[3] = 0;
	config->compare_values[4] = 0;
	config->compare_values[5] = 0;
}

/**
 * \brief Enables the RTC module.
 *
 * This will enable the RTC module. Most configurations cannot be altered
 * while the module is enabled. \ref rtc_count_disable "Disable" module to
 * change configurations.
 */
static inline void rtc_count_enable(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sync. */
	_rtc_count_wait_for_sync();

	/* Enable RTC module. */
	rtc_module->CTRL |= RTC_ENABLE_bm;
}

/**
 * \brief Disables the RTC module.
 *
 * This will disable the RTC module.
 */
static inline void rtc_count_disable(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sync. */
	_rtc_count_wait_for_sync();

	/* Disable RTC module. */
	rtc_module->CTRL &= ~RTC_ENABLE_bm;
}

enum status_code rtc_count_init(const struct rtc_count_conf *const config);

/** @} */

/** \name Set count and compare values
 * @{
 */
void rtc_count_set_count(uint32_t count_value);

uint32_t rtc_count_get_count(void);

enum status_code rtc_count_set_compare(uint32_t comp_value,
		enum rtc_count_compare comp_index);

enum status_code rtc_count_get_compare(uint32_t *const comp_value,
		enum rtc_count_compare comp_index);

enum status_code rtc_count_set_period(uint16_t period_value);

enum status_code rtc_count_get_period(uint16_t *const period_value);

/** @} */

/** \name Get and clear flags
 * @{
 */

/**
 * \brief Check if RTC overflow has occurred.
 *
 * Checks the overflow flag. The flag is set when there
 * is an overflow in the counter.
 */
static inline bool rtc_count_is_overflow(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Return status of flag */
	return (rtc_module->INTFLAG & RTC_INTFLAG_OVF_bm);
}

/**
 * \brief Clear the RTC overflow flag.
 *
 * Clear the overflow flag. The flag is set when
 * there is an overflow in the counter.
 */
static inline void rtc_count_clear_overflow(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Clear OVF flag */
	rtc_module->INTFLAG = RTC_INTFLAG_OVF_bm;
}

bool rtc_count_is_compare_match(enum rtc_count_compare comp_index);

enum status_code rtc_count_clear_compare_match(
		enum rtc_count_compare comp_index);

/** @} */

/**
 * \name Misc
 * @{
 */

enum status_code rtc_count_frequency_correction(int8_t value);

/**
 * \brief Enables the periodic event.
 *
 * This enables the given periodic event
 *
 * \param[in] events Bitmask to the events to enable. See \ref rtc_count_events .
 */
static inline void rtc_count_enable_events(uint16_t events)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Enable given event. */
	rtc_module->EVCTRL |= events;
}

/**
 * \brief Disables the given event in the module.
 *
 * This will disable the given event so it cannot be used by the event system.
 *
 * \param[in] event Bitmask to the events to disable. See \ref rtc_count_events
 */
static inline void rtc_count_disable_events(uint16_t events)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Disable given events. */
	rtc_module->EVCTRL &= ~events;
}

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

/**
 * \page rtc_count_extra_info_page Extra Information
 *
 *\section acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</td>
 *		<th>Description</td>
 *	</tr>
 *	<tr>
 *		<td>RTC</td>
 *		<td>Real Time Counter</td>
 *	</tr>
 *	<tr>
 *		<td>PPM</td>
 *		<td>Part Per Million</td>
 *	</tr>
 *	<tr>
 *		<td>RC</td>
 *		<td>Resistor/Capacitor</td>
 *	</tr>
 * </table>
 *
 * \section fixed_erratas Erratas fixed by the driver
 * No erratas are registered for the device.
 *
 * \section module_history Module History
 *
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
 * \page quickstart Quick Start Guide for the RTC count
 *
 * This is the quick start guide for the \ref sam0_rtc_count_group module, with
 * step-by-step instructions on how to implement the module.
 *
 * The use case contain some code segments. The code fragments in the
 * guide can be compiled as is in the separate file, or the
 * user can copy fragments into the users application.
 *
 * \see General list of module \ref rtc_count_examples "examples".

 * \section rtc_count_use_cases RTC count use cases:
 * - \subpage rtc_count_basic_use_case
 */

#endif /* RTC_COUNT_H_INCLUDED */
