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

#include "../rtc_header.h"
#include "../Assert.h"

//#if CONF_CLOCK_GCLK_2_RTC == false
//# error "The GCLK_RTC must be set to use a 32kHz source with a prescaler of 32."
//#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_rtc_calendar_group SAM0+ Real Time Counter (RTC), Calendar Mode
 * Driver for the SAM0+ architecture devices. This driver provides a
 * interface for setting up and utilizing the RTC in calendar mode. This module
 * encompasses the following modules within the SAM0+ devices:
 * - \b RTC (Real Time Counter)
 *
 * \section rtc_calendar_intro Introduction
 * The RTC is a 32-bit counter with a 10-bit programmable prescaler that
 * typically continuously, including in low-power sleep modes, to track time.
 * The RTC can be used as a source to wake up the system using the \ref
 * rtc_cal_alarms , \ref rtc_cal_overflow or \ref rtc_cal_periodic triggers.
 *
 *
 *
 * \subsection rtc_cal_alarms Alarms
 *
 * \subsection rtc_cal_overflow Overflow
 *
 * \subsection rtc_cal_periodic Periodic
 *
 * \section rtc_cal_api_overview API Overview
 * @{
 */
/**
 * \brief Available alarm registers.
 */
enum rtc_calendar_alarm {
	/** Compare register 0. */
	RTC_CALENDAR_ALARM_0 = 0,
	/** Compare register 1. */
	RTC_CALENDAR_ALARM_1 = 1,
	/** Compare register 2. */
	RTC_CALENDAR_ALARM_2 = 2,
	/** Compare register 3. */
	RTC_CALENDAR_ALARM_3 = 3,
};

/**
 * \brief Values for frequency correction to be faster or slower.
 */
enum rtc_calendar_frequency_correction {
	/** Frequency correction faster. */
	RTC_CALENDAR_FREQUENCY_CORRECTION_FASTER = 0,
	/** Frequency correction slower. */
	RTC_CALENDAR_FREQUENCY_CORRECTION_SLOWER = 1,
};

/**
 * \brief Values used to enable and disable events.
 *
 * Use these values when disabling or enabling the RTC events.
 */
enum rtc_calendar_events {
	/** To set event off. */
	RTC_CALENDAR_EVENT_OFF        = 0,
	/** Value for overflow event. */
	RTC_CALENDAR_EVENT_OVF        = RTC_EVENT_OVF_bm,
	/** Value for alarm 0 match event. */
	RTC_CALENDAR_EVENT_ALARM_0    = RTC_EVENT_ALARM0_bm,
	/** Value for alarm 1 match event. */
	RTC_CALENDAR_EVENT_ALARM_1    = RTC_EVENT_ALARM1_bm,
	/** Value for alarm 2 match event. */
	RTC_CALENDAR_EVENT_ALARM_2    = RTC_EVENT_ALARM2_bm,
	/** Value for alarm 3 match event. */
	RTC_CALENDAR_EVENT_ALARM_3    = RTC_EVENT_ALARM3_bm,
	/** To set periodic event 0. */
	RTC_CALENDAR_EVENT_PERIODIC_0 = RTC_EVENT_PER0_bm,
	/** To set periodic event 1. */
	RTC_CALENDAR_EVENT_PERIODIC_1 = RTC_EVENT_PER1_bm,
	/** To set periodic event 2. */
	RTC_CALENDAR_EVENT_PERIODIC_2 = RTC_EVENT_PER2_bm,
	/** To set periodic event 3. */
	RTC_CALENDAR_EVENT_PERIODIC_3 = RTC_EVENT_PER3_bm,
	/** To set periodic event 4. */
	RTC_CALENDAR_EVENT_PERIODIC_4 = RTC_EVENT_PER4_bm,
	/** To set periodic event 5. */
	RTC_CALENDAR_EVENT_PERIODIC_5 = RTC_EVENT_PER5_bm,
	/** To set periodic event 6. */
	RTC_CALENDAR_EVENT_PERIODIC_6 = RTC_EVENT_PER6_bm,
	/** To set periodic event 7. */
	RTC_CALENDAR_EVENT_PERIODIC_7 = RTC_EVENT_PER7_bm,
};

/**
 * \brief Time structure.
 *
 * Time structure containing the time given by or set to the RTC calendar.
 * The structure uses 6 values to give second, minute, hour, day, month and
 * year. Can be initiated with the rtc_calendar_get_time_default.
 *
 * \note The year value in the CLOCK register has a limited value of 63. Year 0
 * will therefore have the value specified in the rtc_calendar_conf as its
 * stating point.
 *
 */
struct rtc_calendar_time {
	/** The second of the time */
	uint8_t second;
	/** The minute of the time */
	uint8_t minute;
	/** The hour of the time */
	uint8_t hour;
	/** Pm value. 1 or 0. */
	uint8_t pm;
	/** The day of the time */
	uint8_t day;
	/** The month of the time */
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
	/** If true, clears the CLOCK register on alarm match. */
	bool clear_on_match;
	/** Continuously update the CLOCK register so no synchronization is
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
static void rtc_calendar_get_time_default(struct rtc_calendar_time *time)
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
 * - Alarm registers all 0 year 2000.
 *
 *  \param[out] config Configuration structure to be initialized to default
 *  values.
 */
static inline void rtc_calendar_get_config_defaults(
		struct rtc_calendar_conf *config)
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
		struct rtc_calendar_time *alarm,
		enum rtc_calendar_alarm alarm_index);

/** @} */

/** \name Set, get and clear int flags.
 * @{
 */

/**
 * \brief Check if RTC overflow has occurred.
 *
 * Checks the OVF flag in the RTC INTFLAG register. The flag is set when there
 * is an overflow in the CLOCK register.
 *
 * \return The current status of the overflow flag.
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
 * Clear the OVF flag in the RTC INTFLAG register.
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
 * Check the ALARM flag in the RTC INTFLAG register. The flag is set when there
 * is an compare match on the ALARM.
 *
 * \param[in] alarm_index The index of the alarm to check.
 *
 * \return Status of given alarm match.
 */
//TODO: Some major stuff!
#warning "Not correct return!"
static inline bool rtc_calendar_is_alarm_match(enum rtc_calendar_alarm alarm_index)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sanity check. */
	if ((uint32_t)alarm_index > RTC_CALENDAR_ALARM_3) {
		return false;
	}

	/* Return int flag status. */
	return (rtc_module->INTFLAG & (1 << alarm_index));
}

/**
 * \brief Clears the RTC alarm flag.
 *
 * Clear the requested ALARM flag in the RTC INTFLAG register.
 *
 * \param[in] alarm_index The index of the alarm to clear.
 *
 * \return status_code of clearing the alarm match flag.
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

enum status_code rtc_calendar_frequency_correction(
		enum rtc_calendar_frequency_correction dir, uint8_t value);

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

#endif /* RTC_CALENDAR_H_INCLUDED */

