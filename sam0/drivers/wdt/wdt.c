/**
 * \file
 *
 * \brief SAM0+ Watchdog Driver
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
#include <wdt.h>

/** \internal
 *  Key used to reset the Watchdog elapsed count when written to the
 *  CLEAR register to reset the count value.
 */
#define WDT_CLEAR_KEY  0xA5

/**
 * \internal Internal device structure.
 */
struct _wdt_device {
	/** If \c true, the Watchdog should be locked on when enabled. */
	bool always_on;
};

/** \internal
 *
 *  Internal Watchdog device state, used to track instance specific information
 *  for the Watchdog peripheral within the device.
 */
static struct _wdt_device _wdt_dev;

/**
 *  \internal
 *  \brief Waits for a Watchdog module synchronisation to complete.
 *
 *  Spinloops until the Watchdog module has completed any pending writes to the
 *  control registers across the system and watchdog digital clock boundary.
 */
static inline void _wdt_wait_for_sync(void)
{
	WDT_t *const WDT_module = &WDT;

	/* Poll the SYNCBUSY flag until it signals the module is ready */
	while (WDT_module->STATUS & WDT_SYNCBUSY_bm) {
		/* Wait until WDT is synced */
	}
}

/** \brief Initializes a software instance of the Watchdog driver.
 *
 *  Initializes the Watchdog driver, resetting the hardware module and
 *  configuring it to the user supplied configuration parameters, ready for
 *  use. This function should be called before enabling the watchdog.
 *
 *  \note Once called the watchdog will not be running; to start the watchdog,
 *        call \ref wdt_enable() after configuring the module.
 *
 *  \param config  Configuration settings for the watchdog
 *
 *  \return Status of the configuration procedure.
 *  \retval STATUS_OK     If the module was configured correctly
 *  \retval STATUS_ERR_INVALID_ARG   If invalid argument(s) were supplied
 *  \retval STATUS_ERR_IO  If the Watchdog module is locked to be always on
 */
enum status_code wdt_init(
		const struct wdt_conf *const config)
{
	WDT_t *const WDT_module = &WDT;

	/* Sanity check arguments */
	Assert(config);

	/* Check of the watchdog has been locked to be always on, if so, abort */
	if (wdt_is_locked()) {
		return STATUS_ERR_IO;
	}

	/* Check for an invalid timeout period, abort if found */
	if (config->timeout_period == WDT_PERIOD_NONE) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Make sure the Window and Early Warning periods are not more than the
	 * reset period, abort if either is invalid */
	if ((config->timeout_period < config->window_period) ||
			(config->timeout_period < config->early_warning_period)) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Update the timeout period value with the requested period */
	_wdt_wait_for_sync();
	WDT_module->CTRL = (config->timeout_period - 1) << WDT_PER_gp;

	_wdt_wait_for_sync();

	/* Check if the user has requested a reset window period */
	if (config->window_period != WDT_PERIOD_NONE) {
		/* Update and enable the timeout period value */
		WDT_module->WINCTRL = WDT_WEN_bm |
				(config->window_period - 1) << WDT_WINDOW_gp;
	} else {
		/* Ensure the window enable control flag is cleared */
		WDT_module->WINCTRL &= ~WDT_WEN_bm;
	}

	/* Check if the user has requested an early warning period */
	if (config->early_warning_period != WDT_PERIOD_NONE) {
		/* Set the Early Warning period */
		_wdt_wait_for_sync();
		WDT_module->EWCTRL
			= (config->early_warning_period - 1) << WDT_EWOFFSET_gp;
	}

	/* Save the requested Watchdog lock state for when the WDT is enabled */
	_wdt_dev.always_on = config->always_on;

	return STATUS_OK;
}

/** \brief Enables the Watchdog Timer that was previously configured.
 *
 *  Enables and starts the Watchdog Timer that was previously configured via a
 *  call to \ref wdt_init().
 *
 *  \return Status of the enable procedure.
 *  \retval STATUS_OK      If the module was enabled correctly
 *  \retval STATUS_ERR_IO  If the Watchdog module is locked to be always on
 */
enum status_code wdt_enable(void)
{
	WDT_t *const WDT_module = &WDT;

	/* Check of the watchdog has been locked to be always on, if so, abort */
	if (wdt_is_locked()) {
		return STATUS_ERR_IO;
	}

	_wdt_wait_for_sync();

	/* Either enable or lock-enable the watchdog timer depending on the user
	 * settings */
	if (_wdt_dev.always_on) {
		WDT_module->CTRL |= WDT_ALWAYSON_bm;
	} else {
		WDT_module->CTRL |= WDT_ENABLE_bm;
	}

	return STATUS_OK;
}

/** \brief Disables the Watchdog Timer that was previously enabled.
 *
 *  Stops the Watchdog Timer that was previously started via a call to
 *  \ref wdt_enable().
 *
 *  \return Status of the disable procedure.
 *  \retval STATUS_OK     If the module was disabled correctly
 *  \retval STATUS_ERR_IO If the Watchdog module is locked to be always on
 */
enum status_code wdt_disable(void)
{
	WDT_t *const WDT_module = &WDT;

	/* Check of the watchdog has been locked to be always on, if so, abort */
	if (wdt_is_locked()) {
		return STATUS_ERR_IO;
	}

	/* Disable the Watchdog module */
	_wdt_wait_for_sync();
	WDT_module->CTRL &= ~WDT_ENABLE_bm;

	return STATUS_OK;
}

/** \brief Resets the count of the running Watchdog Timer that was previously enabled.
 *
 *  Resets the current count of the Watchdog Timer, restarting the timeout
 *  period count elapsed. This function should be called after the window
 *  period (if one was set in the module configuration) but before the timeout
 *  period to prevent a reset of the system.
 */
void wdt_reset_count(void)
{
	WDT_t *const WDT_module = &WDT;

	/* Disable the Watchdog module */
	_wdt_wait_for_sync();
	WDT_module->CLEAR = WDT_CLEAR_KEY;
}
