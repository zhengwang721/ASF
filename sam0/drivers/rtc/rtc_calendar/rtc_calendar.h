/**
 * \file
 *
 * \brief SAMD20 RTC Driver for calendar mode
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
#ifndef RTC_CALENDAR_H_INCLUDED
#define RTC_CALENDAR_H_INCLUDED

/**
 * \defgroup asfdoc_samd20_rtc_calendar_group SAMD20 RTC Calendar Driver (RTC CAL)
 *
 * This driver for SAMD20 devices provides an interface for the configuration
 * and management of the device's Real Time Clock functionality in Calendar
 * operating mode, for the configuration and retrieval of the current time and
 * date as maintained by the RTC module.
 *
 * The following peripherals are used by this module:
 *
 *  - RTC (Real Time Clock)
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_samd20_rtc_calendar_prerequisites
 *  - \ref asfdoc_samd20_rtc_calendar_module_overview
 *  - \ref asfdoc_samd20_rtc_calendar_special_considerations
 *  - \ref asfdoc_samd20_rtc_calendar_extra_info
 *  - \ref asfdoc_samd20_rtc_calendar_examples
 *  - \ref asfdoc_samd20_rtc_calendar_api_overview
 *
 *
 * \section asfdoc_samd20_rtc_calendar_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samd20_rtc_calendar_module_overview Module Overview
 *
 * The RTC module in the SAMD20 devices is a 32-bit counter, with a 10-bit
 * programmable prescaler. Typically, the RTC clock is run continuously,
 * including in the device's low-power sleep modes, to track the current time
 * and date information. The RTC can be used as a source to wake up the system
 * at a scheduled time or periodically using the alarm functions.
 *
 * In this driver, the RTC is operated in Calendar mode. This allows for an
 * easy integration of a real time clock and calendar into a user application
 * to track the passing or time and/or perform scheduled tasks.
 *
 * Whilst operating in Calendar mode, the RTC features:
 *  - Time tracking in seconds, minutes and hours
 *   - 12 hour or 24 hour mode
 *  - Date tracking in day, month and year
 *   - Automatic leap year correction
 *
 * \subsection asfdoc_samd20_rtc_calendar_module_overview_alarms Alarms and Overflow
 * The RTC has 4 independent hardware alarms that can be configured by the user
 * application. These alarms will be will trigger on match with the current
 * clock value, and can be set up to trigger an interrupt, event, or both. The
 * RTC can also be configured to clear the clock value on alarm match, resetting
 * the clock to the original start time.
 *
 * If the RTC is operated in clock-only mode (i.e. with calendar disabled), the
 * RTC counter value will instead be cleared on overflow once the maximum count
 * value has been reached:
 *
 * \f$COUNT_{MAX} = 2^{32}-1\f$
 *
 * When the RTC is operated with the calendar enabled and run at the required
 * 1 Hz input clock frequency, a register overflow will occur after 64 years.
 *
 * \subsection asfdoc_samd20_rtc_calendar_module_overview_periodic Periodic Events
 * The RTC can generate events at periodic intervals, allowing for direct
 * peripheral actions without CPU intervention. The periodic events can be
 * generated on the upper 8 bits of the RTC prescaler, and will be generated on
 * the rising edge transition of the specified bit. The resulting periodic
 * frequency can be calculated by the following formula:
 *
 * \f[
 * f_{PERIODIC}=\frac{f_{ASY}}{2^{n+3}}
 * \f]
 *
 * Where \f$f_{ASY}\f$ refers to the \e asynchronous clock set up in the RTC
 * module configuration. For the RTC to operate correctly in calendar mode, this
 * frequency must be 1024 Hz. The \b n parameter is the event source generator
 * index of the RTC module. If the asynchronous clock is operated at the
 * recommended 1024 Hz, the formula results in the following output:
 *
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
 * \subsection asfdoc_samd20_rtc_calendar_module_overview_correction Digital Frequency Correction
 * The RTC module contains Digital Frequency Correction logic to compensate for
 * inaccurate source clock frequencies which would otherwise result in skewed
 * time measurements. The correction scheme requires that at least two bits
 * in the RTC module prescaler are reserved by the correction logic. As a
 * result of this implementation, frequency correction is only available when
 * the RTC is running from a 1 Hz reference clock.
 *
 * The correction procedure is implemented by subtracting or adding a single
 * cycle from the RTC prescaler every 1024 RTC GCLK cycles. The adjustment is
 * applied the specified number of time (max 127) over 976 of these periods. The
 * corresponding correction in PPM will be given by:
 *
 * \f[
 * Correction(PPM) = \frac{VALUE}{999424}10^6
 * \f]
 *
 * The RTC clock will tick faster if provided with a positive correction value,
 * and slower when given a negative correction value.
 *
 *
 * \section asfdoc_samd20_rtc_calendar_special_considerations Special Considerations
 *
 * \subsection asfdoc_samd20_rtc_calendar_special_considerations_year Year limit
 * The RTC module has a year range of 63 years from the starting year configured
 * when the module is initialized. Dates outside the start to end year range
 * described below will need software adjustment:
 *
 * \f[[YEAR_{START}, YEAR_{START}+64)\f]
 *
 * \subsection asfdoc_samd20_rtc_calendar_special_considerations_clock Clock Setup
 * The RTC is typically clocked by a specialized GCLK generator that has a
 * smaller prescaler than the others. By default the RTC clock is on, selected
 * to use the internal 32 kHz RC-oscillator with a prescaler of 32, giving a
 * resulting clock frequency of 1024 Hz to the RTC. The internal RTC prescaler
 * is set to 1024 which yields an end-frequency of 1 Hz.
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
 *       be used.
 *
 *
 * \section asfdoc_samd20_rtc_calendar_extra_info Extra Information for RTC CAL
 *
 * For extra information see \ref asfdoc_samd20_rtc_calendar_extra. This includes:
 *  - \ref asfdoc_samd20_rtc_calendar_extra_acronyms
 *  - \ref asfdoc_samd20_rtc_calendar_extra_dependencies
 *  - \ref asfdoc_samd20_rtc_calendar_extra_errata
 *  - \ref asfdoc_samd20_rtc_calendar_extra_history
 *
 *
 * \section asfdoc_samd20_rtc_calendar_examples Examples
 *
 * The following Quick Start guides and application examples are available for this driver:
 * - \ref asfdoc_samd20_rtc_calendar_basic_use_case
 *
 *
 * \section asfdoc_samd20_rtc_calendar_api_overview API Overview
 * @{
 */

#include <conf_clocks.h>

#if CONF_CLOCK_GCLK_2_RTC == false
#  error "Application conf_clocks.h configuration header has invalid settings for the RTC module."
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief Available alarm registers.
 *
 * Available alarm registers.
 * \note Not all alarm registers are available on all devices.
 */
enum rtc_calendar_alarm {
	/** Alarm register 0. */
	RTC_CALENDAR_ALARM_0 = 0,
#if (RTC_NUM_OF_COMP16 > 1) || defined(__DOXYGEN__)
	/** Alarm register 1. */
	RTC_CALENDAR_ALARM_1 = 1,
#endif
#if (RTC_NUM_OF_COMP16 > 2) || defined(__DOXYGEN__)
	/** Alarm register 2. */
	RTC_CALENDAR_ALARM_2 = 2,
#endif
#if (RTC_NUM_OF_COMP16 > 3) || defined(__DOXYGEN__)
	/** Alarm register 3. */
	RTC_CALENDAR_ALARM_3 = 3,
#endif
};

/**
 * \brief Available mask options for alarms.
 *
 * Available mask options for alarms.
 */
enum rtc_calendar_alarm_mask {
	/** Alarm disabled */
	RTC_CALENDAR_ALARM_MASK_DISABLED = 0,
	/** Alarm match on second */
	RTC_CALENDAR_ALARM_MASK_SEC      = 1,
	/** Alarm match on second and minute */
	RTC_CALENDAR_ALARM_MASK_MIN      = 2,
	/** Alarm match on second, minute and hour */
	RTC_CALENDAR_ALARM_MASK_HOUR     = 3,
	/** Alarm match on second, minutes hour and day */
	RTC_CALENDAR_ALARM_MASK_DAY      = 4,
	/** Alarm match on second, minute, hour, day and month */
	RTC_CALENDAR_ALARM_MASK_MONTH    = 5,
	/** Alarm match on second, minute, hour, day, month and year */
	RTC_CALENDAR_ALARM_MASK_YEAR     = 6,
};

/**
 * \brief Values used to enable and disable events.
 *
 * Values used to enable and disable events.
 *
 * \note Not all alarm events are available on all devices.
 */
enum rtc_calendar_event {
	/** To set event off. */
	RTC_CALENDAR_EVENT_OFF        = 0,
	/** Overflow event. */
	RTC_CALENDAR_EVENT_OVF        = RTC_MODE2_EVCTRL_OVFEO,
	/** Alarm 0 match event. */
	RTC_CALENDAR_EVENT_ALARM_0    = RTC_MODE2_EVCTRL_ALARMEO(1 << 0),
#if (RTC_NUM_OF_COMP16 > 1) || defined(__DOXYGEN__)
	/** Alarm 1 match event. */
	RTC_CALENDAR_EVENT_ALARM_1    = RTC_MODE2_EVCTRL_ALARMEO(1 << 1),
#endif
#if (RTC_NUM_OF_COMP16 > 2) || defined(__DOXYGEN__)
	/** Alarm 2 match event. */
	RTC_CALENDAR_EVENT_ALARM_2    = RTC_MODE2_EVCTRL_ALARMEO(1 << 2),
#endif
#if (RTC_NUM_OF_COMP16 > 3) || defined(__DOXYGEN__)
	/** Alarm 3 match event. */
	RTC_CALENDAR_EVENT_ALARM_3    = RTC_MODE2_EVCTRL_ALARMEO(1 << 3),
#endif
	/** Periodic event 0. */
	RTC_CALENDAR_EVENT_PERIODIC_0 = RTC_MODE2_EVCTRL_PEREO(1 << 0),
	/** Periodic event 1. */
	RTC_CALENDAR_EVENT_PERIODIC_1 = RTC_MODE2_EVCTRL_PEREO(1 << 1),
	/** Periodic event 2. */
	RTC_CALENDAR_EVENT_PERIODIC_2 = RTC_MODE2_EVCTRL_PEREO(1 << 2),
	/** Periodic event 3. */
	RTC_CALENDAR_EVENT_PERIODIC_3 = RTC_MODE2_EVCTRL_PEREO(1 << 3),
	/** Periodic event 4. */
	RTC_CALENDAR_EVENT_PERIODIC_4 = RTC_MODE2_EVCTRL_PEREO(1 << 4),
	/** Periodic event 5. */
	RTC_CALENDAR_EVENT_PERIODIC_5 = RTC_MODE2_EVCTRL_PEREO(1 << 5),
	/** Periodic event 6. */
	RTC_CALENDAR_EVENT_PERIODIC_6 = RTC_MODE2_EVCTRL_PEREO(1 << 6),
	/** Periodic event 7. */
	RTC_CALENDAR_EVENT_PERIODIC_7 = RTC_MODE2_EVCTRL_PEREO(1 << 7),
};

/**
 * \brief Time structure.
 *
 * Time structure containing the time given by or set to the RTC calendar.
 * The structure uses seven values to give second, minute, hour, PM/AM, day,
 * month and year. It should be initialized via the
 * \ref rtc_calendar_get_time_defaults() function before use.
 */
struct rtc_calendar_time {
	/** The second of the time */
	uint8_t second;
	/** The minute of the time */
	uint8_t minute;
	/** The hour of the time */
	uint8_t hour;
	/** PM/AM value. 1 for pm, or 0 for am. */
	uint8_t pm;
	/** The day of the time. Day 1 will be the first day of the month. */
	uint8_t day;
	/** The month of the time. Month 1 will be January. */
	uint8_t month;
	/** The year of the time */
	uint16_t year;
};

/**
 * \brief Alarm structure.
 *
 * Alarm structure containing time of the alarm and a mask to determine when
 * the alarm will trigger.
 */
struct rtc_calendar_alarm {
	/** Alarm time. */
	struct rtc_calendar_time time;
	/** Alarm mask. */
	enum rtc_calendar_alarm_mask mask;
};

/**
 * \brief RTC configuration structure
 *
 * Configuration structure for the RTC instance. This structure should
 * be initialized using the \ref rtc_calendar_get_config_defaults() before any
 * user configurations are set.
 */
struct rtc_calendar_config {
	/** If \c true, clears the clock on alarm match. */
	bool clear_on_match;
	/** If \c true, the digital counter registers will be continuously updated
	 *  so that internal synchronization is not needed when reading the current
	 *  count. */
	bool continuously_update;
	/** If \true, time is represented in 24 hour mode. */
	bool clock_24h;
	/** Initial year for counter value 0. */
	uint16_t year_init_value;
	/** Set bitmask of events to enable. */
	uint16_t event_generators;
	/** Alarm values. */
	struct rtc_calendar_alarm alarm[RTC_NUM_OF_ALARMS];
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
	while (RTC->MODE2.STATUS.reg & RTC_STATUS_SYNCBUSY){
		/* Wait for RTC to sync. */
	}
}
#endif

/**
 * \brief Initialize a \c time structure.
 *
 * This will initialize a given time structure to the time 00:00:00 (hh:mm:ss)
 * and date 2000-01-01 (YYYY-MM-DD).
 *
 * \param[out] time Time structure to initialize.
 */
static inline void rtc_calendar_get_time_defaults(
		struct rtc_calendar_time *const time)
{
	time->second = 0;
	time->minute = 0;
	time->hour   = 0;
	time->pm     = 0;
	time->day 	 = 1;
	time->month  = 1;
	time->year   = 2000;
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
 * - Alarms set to January 1. 2000, 00:00:00.
 * - Alarm will match on second, minute, hour, day, month and year .
 *
 *  \param[out] config Configuration structure to be initialized to default
 *  values.
 */
static inline void rtc_calendar_get_config_defaults(
		struct rtc_calendar_config *const config)
{
	/* Sanity check argument */
	Assert(config);

	/* Initialize and set time structure to default. */
	struct rtc_calendar_time time;
	rtc_calendar_get_time_defaults(&time);

	/* Set default into configuration structure */
	config->clear_on_match = false;
	config->continuously_update = false;
	config->clock_24h = false;
	config->year_init_value = 2000;
	config->event_generators = RTC_CALENDAR_EVENT_OFF;
	for (uint8_t i = 0; i < RTC_NUM_OF_ALARMS; i++) {
		config->alarm[i].time = time;
		config->alarm[i].mask = RTC_CALENDAR_ALARM_MASK_YEAR;
	}
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
	Rtc *const rtc_module = RTC;

	/* Sync. */
	_rtc_calendar_wait_for_sync();

	/* Enable RTC module. */
	rtc_module->MODE2.CTRL.reg |= RTC_MODE2_CTRL_ENABLE;
}

/**
 * \brief Disables the RTC module.
 *
 * This will disable the RTC module.
 */
static inline void rtc_calendar_disable(void)
{
	/* Initialize module pointer. */
	Rtc *const rtc_module = RTC;

	/* Sync: */
	_rtc_calendar_wait_for_sync();

	/* Disable RTC module. */
	rtc_module->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_ENABLE;
}

void rtc_calendar_init(
		const struct rtc_calendar_config *const config);

void rtc_calendar_swap_time_mode(void);

enum status_code rtc_calendar_frequency_correction(
		int8_t value);

/** @} */

/** \name Time and alarm management
 * @{
 */

void rtc_calendar_set_time(
		const struct rtc_calendar_time *const time);

void rtc_calendar_get_time(
		struct rtc_calendar_time *const time);

enum status_code rtc_calendar_set_alarm(
		const struct rtc_calendar_alarm *const alarm,
		const enum rtc_calendar_alarm alarm_index);

enum status_code rtc_calendar_get_alarm(
		struct rtc_calendar_alarm *const alarm,
		const enum rtc_calendar_alarm alarm_index);

/** @} */

/** \name Status flag management
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
	Rtc *const rtc_module = RTC;

	/* Return status of flag. */
	return (rtc_module->MODE2.INTFLAG.reg & RTC_MODE2_INTFLAG_OVF);
}

/**
 * \brief Clear the RTC overflow flag.
 *
 * Clear the overflow flag.
 */
static inline void rtc_calendar_clear_overflow(void)
{
	/* Initialize module pointer. */
	Rtc *const rtc_module = RTC;

	/* Clear flag. */
	rtc_module->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_OVF;
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
		const enum rtc_calendar_alarm alarm_index)
{
	/* Initialize module pointer. */
	Rtc *const rtc_module = RTC;

	/* Sanity check. */
	if ((uint32_t)alarm_index > RTC_NUM_OF_ALARMS) {
		Assert(false);
		return false;
	}

	/* Return int flag status. */
	return (rtc_module->MODE2.INTFLAG.reg & (1 << alarm_index));
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
static inline enum status_code rtc_calendar_clear_alarm_match(
		const enum rtc_calendar_alarm alarm_index)
{
	/* Initialize module pointer. */
	Rtc *const rtc_module = RTC;

	/* Sanity check. */
	if ((uint32_t)alarm_index > RTC_NUM_OF_ALARMS) {
		Assert(false);
		return STATUS_ERR_INVALID_ARG;
	}

	/* Clear flag. */
	rtc_module->MODE2.INTFLAG.reg = (1 << alarm_index);

	return STATUS_OK;
}

/** @} */

/**
 * \name Event management
 * @{
 */

/**
 * \brief Enables the given event in the module.
 *
 * This will enable the given event so it can be used by the event system.
 *
 * \param[in] events Bitmask containing events to enable.
 */
static inline void rtc_calendar_enable_events(
		const uint16_t events)
{
	/* Initialize module pointer. */
	Rtc *const rtc_module = RTC;

	/* Enable given event. */
	rtc_module->MODE2.EVCTRL.reg |= events;
}

/**
 * \brief Disables the given event in the module.
 *
 * This will disable the given event so it cannot be used by the event system.
 *
 * \param[in] events Bitmask to the events to disable.
 */
static inline void rtc_calendar_disable_events(
		const uint16_t events)
{
	/* Initialize module pointer. */
	Rtc *const rtc_module = RTC;

	/* Disable given events. */
	rtc_module->MODE2.EVCTRL.reg &= ~events;
}

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif


/**
 * \page asfdoc_samd20_rtc_calendar_extra Extra Information
 *
 * \section asfdoc_samd20_rtc_calendar_extra_acronyms Acronyms
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
 *
 * \section asfdoc_samd20_rtc_calendar_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_samd20_rtc_calendar_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samd20_rtc_calendar_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
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
 * \page asfdoc_samd20_rtc_calendar_exqsg Examples for RTC Calendar Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samd20_rtc_calendar_group. QSGs are simple
 * examples with step-by-step instructions to configure and use this driver in a
 * selection of use cases. Note that QSGs can be compiled as a standalone
 * application or be added to the user application.
 *
 *  - \subpage asfdoc_samd20_rtc_calendar_basic_use_case
 */

#endif /* RTC_CALENDAR_H_INCLUDED */

