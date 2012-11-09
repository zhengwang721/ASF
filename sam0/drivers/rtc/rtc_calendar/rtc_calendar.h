/**
 * \file
 *
 * \brief SAM0+ RTC Driver for calendar mode
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

#ifndef RTC_CALENDAR_H_INCLUDED
#define RTC_CALENDAR_H_INCLUDED

#include "./config/conf_clocks.h"

#if CONF_CLOCK_GCLK_2_RTC == false
# error "The GCLK_RTC must be set to use a 32kHz source with a prescaler of 32."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_rtc_cal_group SAM0+ Real Time Counter (RTC), Calendar Mode
 * Driver for the SAM0+ architecture devices. This driver provides a
 * interface for setting up and utilizing the RTC in calendar mode. This module
 * encompasses the following modules within the SAM0+ devices:
 * - \b RTC (Real Time Counter)
 *
 * \section rtc_calendar_intro Introduction
 * The RTC is a 32-bit counter with a 10-bit programmable prescaler that
 * typically runs continuously, including in low-power sleep modes, to track
 * time. The RTC can be used as a source to wake up the system using the \ref
 * rtc_cal_alarms "Alarms, Overflows" or \ref rtc_cal_periodic triggers.
 *
 * For this driver, the RTC is operated in calendar mode. This allows for an
 * easy integration of a clock to projects, as the time value is handled by
 * a \ref rtc_calendar_time "Struct" that provides a easy human readable time
 * stamp. Alternatively the RTC can be operated as a regular
 * \ref sam0_rtc_count_group "counter" if the specialized calendar mode is not
 * needed.
 *
 * For the calendar mode, the RTC features are as follows:
 *  - Time in seconds, minutes and hours.
 *   - 12 hour or 24 hour mode.
 *  - Date in day, month and year.
 *   - Month 1 is January.
 *   - Month 2 is February.
 *   - ...
 *  - Leap year correction.
 *
 * \subsection rtc_cal_alarms Alarms and Overflow
 * The RTC has 4 alarm that can be set up independently.
 * These alarms will be will trigger on match with the current clock
 * value, and can be set up to trigger an interrupt, an event, or both. The RTC
 * can also be configured to clear the clock value on alarm match, meaning
 * the clock will be set to the original start time.
 *
 * If the RTC is operated without the \ref rtc_calendar_conf option enabled, the
 * RTC counter value will instead be cleared on overflow once the maximum count
 * value has been reached (\f$2^{32}-1\f$). When the RTC is operated in the
 * calendar mode and run at the required 1 Hz, a register overflow will happen
 * after 64 years. Below is a figural representation of the two operation modes.
 * \dot
 *     digraph text_box1{
 *     fontsize=20;
 *     label = "Counter with clear on match deactivated.";
 *     }
 * \enddot
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
 * }
 * \enddot
 * \dot
 *     digraph text_box2{
 *     fontsize=20;
 *     label = "Counter with clear on match activated.";
 *     }
 * \enddot
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
 * }
 *
 * \enddot
 *
 * \subsection rtc_cal_periodic Periodic Events
 * The RTC can generate events at periodic intervals, allowing for flexible
 * system ticks. The periodic events can be generated on the upper 8 bits of the
 * RTC prescaler, and will be generated on the rising edge transition of the
 * specified bit. The resulting periodic frequency can be calculated by the
 * following formula:
 * \f[
 * f_{PERIODIC}=\frac{f_{ASY}}{2^{n+3}}
 * \f]
 * Where \f$f_{ASY}\f$ refers to the <EM>asynchronous</EM>
 * \ref rtc_cal_clock_conf "clock" set up in the \ref sam0_gclk_group "GCLK"
 * configurations. For the RTC to operate correctly in calendar mode, this
 * frequency will need to be 1024 Hz. The <EM>n</EM> parameter is the event
 * source given by the implementer and is available as an enum in the
 * \ref rtc_calendar_events "API". If the \f$f_{ASY}\f$ is operated at the
 * recommended 1024 Hz, the formula gives you:
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
 * \subsection rtc_cal_clock_conf Clock Setup
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
 * input prescaler options:
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
 * \note For the calendar to operate correctly, the default setting of 1Hz must
 * be used.
 *
 * \subsection rtc_cal_correction Digital Frequency Correction
 * The RTC can employ counter corrections to compensate inaccurate oscillators.
 * The correction scheme requires that at least two bits
 * in the RTC prescaler are used. Because of this the correction will only be
 * available in 1 Hz mode.
 *
 * The correction procedure is implemented by subtracting or adding a single
 * cycle from the RTC prescaler every 1024 RTC_GCLK cycles. The adjustment is
 * applied the specified number of time (max 127) over 976 of these periods. The
 * corresponding correction in PPM will be given by:
 * \f[
 * Correction(PPM) = \frac{VALUE}{999424}10^6
 * \f]
 * To clock will go faster if provided with a positive value, and slower when
 * given a negative one.
 *
 * \section rtc_cal_dependencies Dependencies
 * The RTC calendar has the following dependencies:
 * - \b GCLK (Generic Clock Management)
 * - \b SYSCTRL (Clock source control)
 *
 * \section rtc_cal_special_considerations Special Considerations
 * \subsection rtc_cal_special_limit_year Year limit.
 * If a year other than between 2000 and 2063 is needed, the initial year in
 * the \ref rtc_calendar_conf will have to be altered. The years available will
 * be between value of the initial year and 63 years forward.
 *
 * \subsection rtc_cal_extra_info Extra Information
 * See \ref rtc_cal_extra_info_page
 *
 * \section rtc_cal_examples Examples
 * - \ref rtc_cal_quickstart
 *
 * \section rtc_cal_api_overview API Overview
 * @{
 */
/**
 * \brief Available alarm registers.
 */
enum rtc_calendar_alarm {
	/** Alarm register 0. */
	RTC_CALENDAR_ALARM_0 = 0,
	/** Alarm register 1. */
	RTC_CALENDAR_ALARM_1 = 1,
	/** Alarm register 2. */
	RTC_CALENDAR_ALARM_2 = 2,
	/** Alarm register 3. */
	RTC_CALENDAR_ALARM_3 = 3,
};

/**
 * \brief Values used to enable and disable events.
 *
 * Use these values when disabling or enabling the RTC events.
 */
enum rtc_calendar_events {
	/** To set event off. */
	RTC_CALENDAR_EVENT_OFF        = 0,
	/** Overflow event. */
	RTC_CALENDAR_EVENT_OVF        = RTC_EVENT_OVF_bm,
	/** Alarm 0 match event. */
	RTC_CALENDAR_EVENT_ALARM_0    = RTC_EVENT_ALARM_0_bm,
	/** Alarm 1 match event. */
	RTC_CALENDAR_EVENT_ALARM_1    = RTC_EVENT_ALARM_1_bm,
	/** Alarm 2 match event. */
	RTC_CALENDAR_EVENT_ALARM_2    = RTC_EVENT_ALARM_2_bm,
	/** Alarm 3 match event. */
	RTC_CALENDAR_EVENT_ALARM_3    = RTC_EVENT_ALARM_3_bm,
	/** Periodic event 0. */
	RTC_CALENDAR_EVENT_PERIODIC_0 = RTC_EVENT_PER_0_bm,
	/** Periodic event 1. */
	RTC_CALENDAR_EVENT_PERIODIC_1 = RTC_EVENT_PER_1_bm,
	/** Periodic event 2. */
	RTC_CALENDAR_EVENT_PERIODIC_2 = RTC_EVENT_PER_2_bm,
	/** Periodic event 3. */
	RTC_CALENDAR_EVENT_PERIODIC_3 = RTC_EVENT_PER_3_bm,
	/** Periodic event 4. */
	RTC_CALENDAR_EVENT_PERIODIC_4 = RTC_EVENT_PER_4_bm,
	/** Periodic event 5. */
	RTC_CALENDAR_EVENT_PERIODIC_5 = RTC_EVENT_PER_5_bm,
	/** Periodic event 6. */
	RTC_CALENDAR_EVENT_PERIODIC_6 = RTC_EVENT_PER_6_bm,
	/** Periodic event 7. */
	RTC_CALENDAR_EVENT_PERIODIC_7 = RTC_EVENT_PER_7_bm,
};

/**
 * \brief Time structure.
 *
 * Time structure containing the time given by or set to the RTC calendar.
 * The structure uses seven values to give second, minute, hour, pm/am, day,
 * month and year. It should be initiated with the
 * \ref rtc_calendar_get_time_default.
 *
 *
 */
struct rtc_calendar_time {
	/** The second of the time */
	uint8_t second;
	/** The minute of the time */
	uint8_t minute;
	/** The hour of the time */
	uint8_t hour;
	/** pm/am value. 1 for pm, or 0 for am. */
	uint8_t pm;
	/** The day of the time. Day 1 will be the first day of the month. */
	uint8_t day;
	/** The month of the time. Month 1 will be January. */
	uint8_t month;
	/** The year of the time */
	uint16_t year;
};

/**
 * \brief RTC configuration structure
 *
 * Configuration structure for the RTC instance. This structure should
 * be initialized using the \ref rtc_calendar_get_config_defaults() before any
 * user configurations are set.
 */
struct rtc_calendar_conf {
	/** If true, clears the clock on alarm match. */
	bool clear_on_match;
	/** Continuously update the clock so no synchronization is
	 * needed for read. */
	bool continuously_update;
	/** 24 hour clock. */
	bool clock_24h;
	/** Initial year for counter value 0. */
	uint16_t year_init_value;
	/** Set bitmask of events to enable. */
	uint16_t event_generators;
	/** Alarm values. */
	struct rtc_calendar_time alarm[4];
};

/**
 * \name Configuration and initialization
 * @{
 */

#if !defined(__DOXYGEN__)
/**
 * \internal Function to wait for sync busy flag to clear.
 */
static inline void _rtc_calendar_wait_for_sync(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	while(rtc_module->STATUS & RTC_SYNC_BUSY_bm){
		/* Wait for RTC to sync. */
	}
}
#endif

/**
 * \brief Initialize time structure.
 *
 * This will initialize a given time structure to the time 00:00:00 (hh:mm:ss)
 * and date 2000-01-01 (YYYY-MM-DD).
 *
 * \param[out] time Time structure to initialize.
 */
static void rtc_calendar_get_time_defaults(struct rtc_calendar_time *const time)
{
	time->second 	= 0;
	time->minute 	= 0;
	time->hour 	= 0;
	time->pm 	= 0;
	time->day 	= 1;
	time->month 	= 1;
	time->year 	= 2000;
}

/**
 * \brief Gets the RTC default settings.
 *
 * Initializes the configuration structure to the known default values. This
 * function should be called at the start of any RTC initiation.
 *
 * The default configuration is as follows:
 * - Clear on alarm match off.
 * - Continuously sync clock off.
 * - 12 hour calendar.
 * - Start year 2000 (Year 0 in the counter will be year 2000).
 * - Events off.
 * - All alarms set to January 1. 2000.
 *
 *  \param[out] config Configuration structure to be initialized to default
 *  values.
 */
static inline void rtc_calendar_get_config_defaults(
		struct rtc_calendar_conf *const config)
{
	/* Sanity check argument */
	Assert(config);

	/* Initialize and set time structure to default. */
	struct rtc_calendar_time time;
	rtc_calendar_get_time_default(&time);

	/* Set default into configuration structure */
	config->clear_on_match = false;
	config->continuously_update = false;
	config->clock_24h = false;
	config->year_init_value = 2000;
	config->event_generators = RTC_CALENDAR_EVENT_OFF;
	config->alarm[0] = time;
	config->alarm[1] = time;
	config->alarm[2] = time;
	config->alarm[3] = time;
}

/**
 * \brief Enables the RTC module.
 *
 * This will enable the RTC module. Most configurations cannot be altered
 * while the module is enabled. \ref rtc_calendar_disable "Disable" module to
 * change configurations.
 */
static inline void rtc_calendar_enable(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sync. */
	_rtc_calendar_wait_for_sync();

	/* Enable RTC module. */
	rtc_module->CTRL |= RTC_ENABLE_bm;
}

/**
 * \brief Disables the RTC module.
 *
 * This will disable the RTC module.
 */
static inline void rtc_calendar_disable(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sync: */
	_rtc_calendar_wait_for_sync();

	/* Disable RTC module. */
	rtc_module->CTRL &= ~RTC_ENABLE_bm;
}

void rtc_calendar_init(const struct rtc_calendar_conf *const config);

void rtc_calendar_swap_time_mode(void);

/** @} */

/** \name Set calendar and alarm values
 * @{
 */

void rtc_calendar_set_time(const struct rtc_calendar_time *const time);

void rtc_calendar_get_time(struct rtc_calendar_time *const time);

enum status_code rtc_calendar_set_alarm(
		const struct rtc_calendar_time *const alarm,
	        enum rtc_calendar_alarm alarm_index);

enum status_code rtc_calendar_get_alarm(
		struct rtc_calendar_time *const alarm,
		enum rtc_calendar_alarm alarm_index);

/** @} */

/** \name Set, get and clear int flags.
 * @{
 */

/**
 * \brief Check if RTC overflow has occurred.
 *
 * Checks the overflow flag in the RTC. The flag is set when there
 * is an overflow in the clock.
 */
static inline bool rtc_calendar_is_overflow(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Return status of flag. */
	return (rtc_module->INTFLAG & RTC_INTFLAG_OVF_bm);
}

/**
 * \brief Clear the RTC overflow flag.
 *
 * Clear the overflow flag.
 */
static inline void rtc_calendar_clear_overflow(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Clear flag. */
	rtc_module->INTFLAG = RTC_INTFLAG_OVF_bm;
}

/**
 * \brief Check the RTC alarm flag.
 *
 * Check if the  specified alarm flag is set. The flag is set when there
 * is an compare match between the alarm value and the clock.
 *
 * \param[in] alarm_index Index of the alarm to check.
 */
static inline bool rtc_calendar_is_alarm_match(
		num rtc_calendar_alarm alarm_index)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sanity check. */
	if ((uint32_t)alarm_index > RTC_CALENDAR_ALARM_3) {
		Assert(false);
		return false;
	}

	/* Return int flag status. */
	return (rtc_module->INTFLAG & (1 << alarm_index));
}

/**
 * \brief Clears the RTC alarm flag.
 *
 * Clear the requested alarm flag.
 *
 * \param[in] alarm_index The index of the alarm to clear.
 *
 * \return Status of clearing the alarm is match flag.
 * \retval STATUS_OK If flag was cleared correctly.
 * \retval STATUS_ERR_INVALID_ARG If invalid argument(s) were provided.
 */
static enum status_code rtc_calendar_clear_alarm_match(
		enum rtc_calendar_alarm alarm_index)
{

	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sanity check. */
	if ((uint32_t)alarm_index > RTC_CALENDAR_ALARM_3) {
		Assert(false);
		return STATUS_ERR_INVALID_ARG;
	}

	/* Clear flag. */
	rtc_module->INTFLAG = (1 << alarm_index);

	return STATUS_OK;
}

/** @} */

/**
 * \name Misc
 * @{
 */

enum status_code rtc_calendar_frequency_correction(int8_t value);

/**
 * \brief Enables the given event in the module.
 *
 * This will enable the given event so it can be used by the event system.
 *
 * \param[in] event Bitmask containing events to enable.
 */
static inline void rtc_calendar_enable_events(uint16_t events)
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
 * \param[in] event Bitmask to the events to disable.
 */
static inline void rtc_calendar_disable_events(uint16_t events)
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
 * \page rtc_cal_extra_info_page Extra Information
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
 * \page quickstart Quick Start Guide for the RTC calendar
 *
 * This is the quick start guide for the \ref sam0_rtc_cal_group module, with
 * step-by-step instructions on how to implement the module.
 *
 * The use case contain some code segments. The code fragments in the
 * guide can be compiled as is in the separate file, or the
 * user can copy fragments into the users application.
 *
 * \see General list of module \ref rtc_cal_examples "examples".

 * \section rtc_cal_use_cases RTC calendar use cases:
 * - \subpage rtc_cal_basic_use_case
 */

#endif /* RTC_CALENDAR_H_INCLUDED */

