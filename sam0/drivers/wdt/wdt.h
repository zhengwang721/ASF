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
#ifndef WDT_H_INCLUDED
#define WDT_H_INCLUDED

/**
 * \defgroup sam0_wdt_group SAM0+ Watchdog Driver (WDT)
 *
 * Driver for the SAM0+ architecture devices. This driver provides a unified
 * interface for the configuration and management of the Watchdog Timer module
 * within the device, including the enabling, disabling and kicking within the
 * device. This driver encompasses the following module within the SAM0 devices:
 *
 * \li \b WDT (Watchdog Timer)
 *
 * Physically, the modules are interconnected within the device as shown in the
 * following diagram:
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="GCLK\nGeneric Clock" shape=square] wdt_clock;
 *
 *   subgraph driver {
 *     node [label="<f0> WDT | <f1> Watchdog Counter" shape=record] wdt_module;
 *     node [label="System Reset Logic" shape=ellipse style=filled fillcolor=lightgray] sys_reset;
 *   }
 *
 *   wdt_clock     -> wdt_module:f1;
 *   wdt_module:f1 -> sys_reset;
 * }
 * \enddot
 *
 * \section module_introduction Introduction
 * The Watchdog module in the SAM0+ devices is designed to give an added level
 * of safety in critical systems, to ensure a system reset is triggered in the
 * case of a deadlock or other software malfunction.
 *
 * \subsection module_overview Overview of the Watchdog
 * At a basic level, the Watchdog is a system timer with a fixed period; once
 * enabled, it will continue to count ticks of its asynchronous clock until
 * it is periodically reset, or the timeout period is reached. In the event of a
 * Watchdog timeout, the module will trigger a system reset identical to a pulse
 * of the device's reset pin, resetting all peripherals to their power-on
 * default states and restarting the application software from the reset vector.
 *
 * In many systems, there is an obvious upper bound to the amount of time each
 * iteration of the main application loop can be expected to run, before a
 * malfunction can be assumed (either due to a deadlock waiting on hardware or
 * software, or due to other means). When the Watchdog is configured with a
 * timeout period equal to this upper bound, a malfunction in the system will
 * force a full system reset to allow for a graceful recovery.
 *
 * \subsection locked_mode Locked Mode
 * The Watchdog configuration can be set in the device fuses and locked in
 * hardware, so that no software changes can be made to the Watchdog
 * configuration. Additionally, the Watchdog can be locked on in software if it
 * is not already locked, so that the module configuration cannot be modified
 * until a power on reset of the device.
 *
 * The locked configuration can be used to ensure that faulty software does not
 * cause the Watchdog configuration to be changed, preserving the level of
 * safety given by the module.
 *
 * \subsection window_mode Window Mode
 * Just as there is a reasonable upper bound to the time the main program loop
 * should take for each iteration, there is also in many applications a lower
 * bound, i.e. a \a minimum time for which each loop iteration should run for
 * under normal circumstances. To guard against a system failure resetting the
 * Watchdog in a tight loop (or a failure in the system application causing the
 * main loop to run faster than expected) a "Window" mode can be enabled to
 * disallow resetting of the Watchdog counter before a certain period of time.
 * If the Watchdog is not reset \a after the window opens but not \a before the
 * Watchdog expires, the system will reset.
 *
 * \subsection early_warning Early Warning
 * In some cases it is desirable to receive an early warning that the Watchdog is
 * about to expire, so that some system action (such as saving any system
 * configuration data for failure analysis purposes) can be performed before the
 * system reset occurs. The Early Warning feature of the Watchdog module allows
 * such a notification to be requested; after the configured early warning time
 * (but before the expiry of the Watchdog counter) the Early Warning flag will
 * become set, so that the user application can take an appropriate action.
 * \note It is important to note that the purpose of the Early Warning feature
 *       is \a not to allow the user application to reset the Watchdog; doing
 *       so will defeat the safety the module gives to the user application.
 *       Instead, this feature should be used purely to perform any tasks that
 *       need to be undertaken before the system reset occurs.
 *
 * \section module_dependencies Dependencies
 * The Watchdog driver has the following dependencies.
 *
 * \li \ref gclk_group "\b GCLK" (Generic Clock Management)
 *
 * \section special_considerations Special Considerations
 *
 * On some devices the Watchdog configuration can be fused to be always on in
 * a particular configuration; if this mode is enabled the Watchdog is not
 * software configurable and can have its count reset and early warning state
 * checked/cleared only.
 *
 * \section module_extra_info Extra Information
 * For extra information see \ref wdt_extra_info.
 *
 * \section module_examples Examples
 * - \ref wdt_quickstart
 *
 * \section api_overview API Overview
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Watchdog Timer period configuration enum.
 *
 * Enum for the possible period settings of the Watchdog timer module, for
 * values requiring a period as a number of Watchdog timer clock ticks.
 */
enum wdt_period {
	/** No Watchdog period. This value can only be used when setting the
	 *  Window and Early Warning periods; its use as the Watchdog Reset
	 *  Period is invalid. */
	WDT_PERIOD_NONE     = 0,
	/** Watchdog period of 8 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_8CLK     = 1,
	/** Watchdog period of 16 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_16CLK    = 2,
	/** Watchdog period of 32 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_32CLK    = 3,
	/** Watchdog period of 64 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_64CLK    = 4,
	/** Watchdog period of 128 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_128CLK   = 5,
	/** Watchdog period of 256 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_256CLK   = 6,
	/** Watchdog period of 512 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_512CLK   = 7,
	/** Watchdog period of 1024 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_1024CLK  = 8,
	/** Watchdog period of 2048 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_2048CLK  = 9,
	/** Watchdog period of 4096 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_4096CLK  = 10,
	/** Watchdog period of 8192 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_8192CLK  = 11,
	/** Watchdog period of 16384 clocks of the Watchdog Timer Generic Clock. */
	WDT_PERIOD_16384CLK = 12,
};

/** \brief Watchdog Timer configuration structure.
 *
 *  Configuration structure for a Watchdog Timer instance. This
 *  structure should be initialized by the \ref wdt_get_config_defaults()
 *  function before being modified by the user application.
 */
struct wdt_conf {
	/** If \c true, the Watchdog will be locked to the current configuration
	 *  settings when the Watchdog is enabled. */
	bool always_on;
	/** Number of Watchdog timer clock ticks until the Watchdog expires. */
	enum wdt_period timeout_period;
	/** Number of Watchdog timer clock ticks until the reset window opens. */
	enum wdt_period window_period;
	/** Number of Watchdog timer clock ticks until the early warning flag is
	 *  set. */
	enum wdt_period early_warning_period;
};

/** \name Configuration and initialization
 * @{
 */

/** \brief Initializes a Watchdog Timer configuration structure to defaults.
 *
 *  Initializes a given Watchdog Timer configuration structure to a set of
 *  known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Not locked, to allow for further (re-)configuration
 *   \li A timeout period of 16384 clocks of the Watchdog module clock
 *   \li No window period, so that the Watchdog count can be reset at any time
 *   \li No early warning period to indicate the Watchdog will soon expire
 *
 *  \param[out] config  Configuration structure to initialize to default values
 */
static inline void wdt_get_config_defaults(
		struct wdt_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->always_on            = false;
	config->timeout_period       = WDT_PERIOD_16384CLK;
	config->window_period        = WDT_PERIOD_NONE;
	config->early_warning_period = WDT_PERIOD_NONE;
}

enum status_code wdt_init(
		const struct wdt_conf *const config);

enum status_code wdt_enable(void);

enum status_code wdt_disable(void);

/** \brief Determines if the Watchdog timer is currently locked in an enabled state.
 *
 *  Determines if the Watchdog timer is currently enabled and locked, so that
 *  it cannot be disabled or otherwise reconfigured.
 *
 *  \return Current Watchdog lock state.
 */
static inline bool wdt_is_locked(void)
{
	WDT_t *const WDT_module = &WDT;

	return (WDT_module->CTRL & WDT_ALWAYSON_bm);
}

/** @} */

/** \name Timeout and Early Warning Management
 * @{
 */

/** \brief Clears the Watchdog timer Early Warning period elapsed flag.
 *
 *  Clears the Watchdog timer Early Warning period elapsed flag, so that a new
 *  early warning period can be detected.
 */
static inline void wdt_clear_early_warning(void)
{
	WDT_t *const WDT_module = &WDT;

	WDT_module->INTFLAG = WDT_EW_bm;
}

/** \brief Determines if the Watchdog timer Early Warning period has elapsed.
 *
 *  Determines if the Watchdog timer Early Warning period has elapsed.
 *
 *  \note If no early warning period was configured, the value returned by this
 *        function is invalid.
 *
 *  \return Current Watchdog Early Warning state.
 */
static inline bool wdt_is_early_warning(void)
{
	WDT_t *const WDT_module = &WDT;

	return (WDT_module->INTFLAG & WDT_EW_bm);
}

void wdt_reset_count(void);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page wdt_extra_info Extra Information
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
 *		<td>WDT</td>
 *		<td>Watchdog Timer</td>
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
 * \page wdt_quickstart Quick Start Guides for the WDT module
 *
 * This is the quick start guide list for the \ref sam0_wdt_group module, with
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
 * \section wdt_use_cases WDT module use cases
 * - \subpage wdt_basic_use_case
 */

#endif /* WDT_H_INCLUDED */
