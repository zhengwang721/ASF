/**
 * \file
 *
 * \brief SAMD20 RTC Driver for calendar mode
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
#include "rtc_calendar.h"

/**
 * \internal Device structure
 */
struct _rtc_device {
	/** If clock mode 24h. */
	bool clock_24h;
	/** If continuously update clock register. */
	bool continuously_update;
	/** Initial year for counter value 0. */
	uint16_t year_init_value;
};

static struct _rtc_device _rtc_dev;

/**
 * \internal Reset the RTC module.
 */
static inline void _rtc_calendar_reset(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Disable module before reset. */
	rtc_calendar_disable();

	/* Sync. */
	_rtc_calendar_wait_for_sync();

	/* Initiate software reset. */
	rtc_module->CTRL |= RTC_SWRST_bm;
}

/**
 * \internal Convert time structure to register_value.
 */

static uint32_t _rtc_calendar_time_to_register_value(
		const struct rtc_calendar_time *const time)
{
	/* Initialize return value. */
	uint32_t register_value;

	/* Set year value into register_value minus initial year. */
	register_value = (time->year - _rtc_dev.year_init_value) <<
			RTC_CLOCK_YEAR_bp;

	/* Set month value into register_value. */
	register_value |= (time->month) << RTC_CLOCK_MONTH_bp;

	/* Set day value into register_value. */
	register_value |= (time->day) << RTC_CLOCK_DAY_bp;

	/* Set 24 hour value into register_value. */
	register_value |= (time->hour) << RTC_CLOCK_HOUR_bp;

	/* Check if 24 h clock and set pm flag. */
	if (!(_rtc_dev.clock_24h)) {
		/* Set pm flag. */
		register_value |= (time->pm) << RTC_CLOCK_PM_bp;
	}

	/* Set minute value into register_value. */
	register_value |= (time->minute) << RTC_CLOCK_MINUTE_bp;

	/* Set second value into register_value. */
	register_value |= (time->second) << RTC_CLOCK_SECOND_bp;

	return register_value;
}

/**
 * \internal Convert register_value to time structure.
 */
static void _rtc_calendar_register_value_to_time(
		uint32_t register_value,
		struct rtc_calendar_time *const time)
{
	/* Set year plus value of initial year. */
	time->year = ((register_value & RTC_CLOCK_YEAR_bm) >> RTC_CLOCK_YEAR_bp)
			 + _rtc_dev.year_init_value;

	/* Set month value into time struct. */
	time->month = ((register_value & RTC_CLOCK_MONTH_bm)
			>> RTC_CLOCK_MONTH_bp);

	/* Set day value into time struct. */
	time->day = ((register_value & RTC_CLOCK_DAY_bm) >> RTC_CLOCK_DAY_bp);

	if (_rtc_dev.clock_24h) {
		/* Set hour in 24h mode. */
		time->hour = ((register_value & RTC_CLOCK_24HOUR_bm)
				>> RTC_CLOCK_24HOUR_bp);
	} else {
		/* Set hour in 12h mode. */
		time->hour = ((register_value & RTC_CLOCK_12HOUR_bm)
				>> RTC_CLOCK_12HOUR_bp);
		/* Set pm flag */
		time->pm = ((register_value & RTC_CLOCK_PM_bm)
				>> RTC_CLOCK_PM_bp);
	}

	/* Set minute value into time struct. */
	time->minute = ((register_value & RTC_CLOCK_MINUTE_bm)
			>> RTC_CLOCK_MINUTE_bp);

	/* Set second value into time struct. */
	time->second = ((register_value & RTC_CLOCK_SECOND_bm)
			>> RTC_CLOCK_SECOND_bp);
}

/**
 * \internal Applies the given configuration.
 *
 * Set the configurations given from the configuration structure to the
 * hardware module.
 * \param[in] config Pointer to the configuration structure.
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK RTC configurations where set successfully.
 * \retval STATUS_ERR_INVALID_ARG If invalid argument(s) where given.
 */
void _rtc_calendar_set_config(
		const struct rtc_calendar_conf *const config)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;


	/* Set up temporary register value. */
	uint16_t tmp_reg;

	/* Set to calendar mode. */
	tmp_reg = RTC_MODE_CLOCK_bm;

	/* Check clock mode. */
	if (!(config->clock_24h)) {
		/* Set clock mode 12h. */
		tmp_reg |= RTC_CLKREP_bm;
	}

	/* Check for clear on compare match. */
	if (config->clear_on_match) {
		/* Set clear on compare match. */
		tmp_reg |= RTC_MATCHCLR_bm;
	}

	/* Set temporary value to register. */
	rtc_module->CTRL = tmp_reg;

	/* Check to set continuously clock read update mode. */
	if (config->continuously_update) {
		/* Set continuously mode. */
		rtc_module->READREQ |= (1 << RTC_RCONT_bp);
	}

	/* Set alarm time registers. */
	/* Sync. */
	_rtc_calendar_wait_for_sync();
	rtc_calendar_set_alarm(&(config->alarm[0]), RTC_CALENDAR_ALARM_0);
	/* Sync. */
	_rtc_calendar_wait_for_sync();
	rtc_calendar_set_alarm(&(config->alarm[1]), RTC_CALENDAR_ALARM_1);
	/* Sync. */
	_rtc_calendar_wait_for_sync();
	rtc_calendar_set_alarm(&(config->alarm[2]), RTC_CALENDAR_ALARM_2);
	/* Sync. */
	_rtc_calendar_wait_for_sync();
	rtc_calendar_set_alarm(&(config->alarm[3]), RTC_CALENDAR_ALARM_3);

	/* Set event source. */
	rtc_calendar_enable_events(config->event_generators);
}

/**
 * \brief Initializes the RTC module with given configurations.
 *
 * This initializes the module, setting up all given configurations to provide
 * the desired functionality of the RTC. \note \ref conf_clocks.h should be set
 * up correctly before using this function.
 *
 * \param[in] config Pointer to the configuration structure.
 *
 * \return Status of the initialization procedure.
 * \retval STATUS_OK If the initialization was run successfully.
 * \retval STATUS_ERR_INVALID_ARG If invalid argument(s) were given.
 */
void rtc_calendar_init(const struct rtc_calendar_conf *const config)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;


	/* Sanity check. */
	Assert(config);

	/* Reset module to hardware defaults. */
	_rtc_calendar_reset();

	//TODO: Do some magic to set up clock!
	/* Set the prescaler to get 1 Hz. */
	rtc_module->CTRL = RTC_PRESCALER_DIV1024_bm;

	/* Save conf_struct internally for continued use. */
	_rtc_dev.clock_24h = config->clock_24h;
	_rtc_dev.continuously_update = config->continuously_update;
	_rtc_dev.year_init_value = config->year_init_value;

	/* Set config. */
	_rtc_calendar_set_config(config);
}

/**
 * \brief Swap between 12h and 24h clock.
 *
 * This will swap the time mode.
 * - If currently in 12h mode, it will swap to 24h.
 * - If currently in 24h mode, it will swap to 12h.
 *
 * \note This will not change setting in user's configuration structure.
 */
void rtc_calendar_swap_time_mode(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Initialize time structure. */
	struct rtc_calendar_time *time;

	/* Get current time. */
	rtc_calendar_get_time(time);

	/* Check current mode. */
	if (_rtc_dev.clock_24h) {
		/* Set pm flag. */
		time->pm = (uint8_t)(time->hour / 12);

		/* Set 12h clock hour value. */
		time->hour = time->hour % 12;

		/* Change value in configuration structure. */
		_rtc_dev.clock_24h = false;
	} else {
		/* Set hour value based on pm flag. */
		if (time->pm == 1) {
			time->hour = time->hour + 12;
			time->pm = 0;
		}

		/* Change value in configuration structure. */
		_rtc_dev.clock_24h = true;
	}

	/* Disable RTC so new configuration can be set. */
	rtc_calendar_disable();

	/* Toggle mode. */
	rtc_module->CTRL ^= RTC_CLKREP_bm;

	/* Enable RTC. */
	rtc_calendar_enable();

	/* Set new time format in CLOCK register. */
	rtc_calendar_set_time(time);
}

/**
 * \brief Set the current calendar time to desired time.
 *
 * This will set the time provided to the clock.
 *
 * \param[in] time The time to set in the clock.
 */
void rtc_calendar_set_time(
		const struct rtc_calendar_time *const time)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	uint32_t register_value = _rtc_calendar_time_to_register_value(time);

	/* Sync. */
	_rtc_calendar_wait_for_sync();

	/* Write value to register. */
	rtc_module->CALENDAR.CLOCK = register_value;
}

/**
 * \brief Get the current calendar value.
 *
 * Returns the current time of the clock
 *
 * \param[out] time Pointer to value that will be filled with current time.
 */
void rtc_calendar_get_time(struct rtc_calendar_time *const time)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Change of read method based on value of continuously_update value in
	 * the configuration structure. */
	if (!(_rtc_dev.continuously_update)) {
		/* Request read on CLOCK register. */
		rtc_module->READREQ = RTC_RREQ_bm;

		/* Sync. */
		_rtc_calendar_wait_for_sync();
	}

	/* Read value. */
	uint32_t register_value = rtc_module->CALENDAR.CLOCK;

	/* Convert value to time structure. */
	_rtc_calendar_register_value_to_time(register_value, time);
}

/**
 * \brief Set the alarm time for the specified alarm.
 *
 * This set the time specified to the requested alarm.
 *
 * \param[in] alarm The time to set the alarm.
 * \param[in] alarm_index The index of the alarm to set.
 *
 * \return Status of setting alarm.
 * \retval STATUS_OK If alarm was set correctly.
 * \retval STATUS_ERR_INVALID_ARG If invalid argument(s) were provided.
 */
enum status_code rtc_calendar_set_alarm(
		const struct rtc_calendar_time *const alarm,
		enum rtc_calendar_alarm alarm_index)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sanity check. */
	if ((uint32_t)alarm_index > RTC_CALENDAR_ALARM_3) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Get register_value from time. */
	uint32_t register_value = _rtc_calendar_time_to_register_value(alarm);

	/* Sync. */
	_rtc_calendar_wait_for_sync();

	/* Set alarm value. */
	rtc_module->CALENDAR.ALARM[alarm_index] = register_value;

	return STATUS_OK;
}

/**
 * \brief Get the current alarm time of specified alarm.
 *
 * This will provide the current alarm time for the alarm specified.
 *
 * \param[out] alarm Pointer to the value that will be filled with alarm
 * time of the specified alarm.
 * \param[in] alarm_index Index of alarm to get alarm time from.
 *
 * \return Status of getting alarm.
 * \retval STATUS_OK If alarm was read correctly.
 * \retval STATUS_ERR_INVALID_ARG If invalid argument(s) were provided.
 */
enum status_code rtc_calendar_get_alarm(struct rtc_calendar_time *const alarm,
		enum rtc_calendar_alarm alarm_index)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sanity check. */
	if ((uint32_t)alarm_index > RTC_CALENDAR_ALARM_3) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Read alarm value. */
	uint32_t register_value = rtc_module->CALENDAR.ALARM[alarm_index];

	/* Convert to time structure and return. */
	_rtc_calendar_register_value_to_time(register_value, alarm);

	return STATUS_OK;
}

/**
 * \brief Calibrate for too-slow or too-fast oscillator.
 *
 * When used, the RTC will compensate for an inaccurate oscillator. The
 * RTC module will add or subtract cycles from the RTC prescaler to adjust the
 * frequency in approximately 1 PPM steps. The provided correction value should
 * be between -127 and 127, allowing for a maximum 127 PPM correction in either
 * direction.
 *
 * If no correction is needed, set value to zero.
 *
 * \note Can only be used when the RTC is operated at 1Hz.
 *
 * \param[in] value Between -127 and 127 used for the correction.
 *
 * \return Status of the calibration procedure.
 * \retval STATUS_OK If calibration was done correctly.
 * \retval STATUS_ERR_INVALID_ARG If invalid argument(s) were provided.
 */
enum status_code rtc_calendar_frequency_correction(int8_t value)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	bool slower;

	/* Convert to positive value. */
	if (value < 0) {
		slower = true;
		value = -value;
	}

	/* Check if valid argument. */
	if (abs(value) > 0x7f) {
		/* Value bigger than allowed, return invalid argument. */
		return STATUS_ERR_INVALID_ARG;
	}

	/* Sync. */
	_rtc_calendar_wait_for_sync();

	/* Set direction. */
	if (slower) {
		rtc_module->FREQCORR = (1 << RTC_FREQCORR_SIGN_bp);
	}
	else {
		rtc_module->FREQCORR = (0 << RTC_FREQCORR_SIGN_bp);
	}

	/* Set value. */
	rtc_module->FREQCORR |= value;

	return STATUS_OK;
}
