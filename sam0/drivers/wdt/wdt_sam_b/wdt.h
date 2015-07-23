/**
 * \file
 *
 * \brief SAM Watchdog Driver for SAMB
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#ifndef WDT_H_INCLUDED
#define WDT_H_INCLUDED

/**
 * \defgroup asfdoc_samb_wdt_group SAM Watchdog Driver (WDT)
 *
 * This driver for Atmel&reg; | SMART SAM devices provides an interface for the configuration
 * and management of the device's Watchdog Timer module, including the enabling,
 * disabling, and kicking within the device. The following driver API modes are
 * covered by this manual:
 *
 *  - Polled APIs
 * \if WDT_CALLBACK_MODE
 *  - Callback APIs
 * \endif
 *
 * The following peripherals are used by this module:
 *  - WDT (Watchdog Timer)
 *
 * The following devices can use this module:
 *  - Atmel | SMART SAM B11
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_samb_wdt_prerequisites
 *  - \ref asfdoc_samb_wdt_module_overview
 *  - \ref asfdoc_samb_wdt_special_considerations
 *  - \ref asfdoc_samb_wdt_extra_info
 *  - \ref asfdoc_samb_wdt_examples
 *  - \ref asfdoc_samb_wdt_api_overview
 *
 *
 * \section asfdoc_samb_wdt_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samb_wdt_module_overview Module Overview
 *
 *
 * \section asfdoc_samb_wdt_special_considerations Special Considerations
 *
 *
 * \section asfdoc_samb_wdt_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_samb_wdt_extra. This includes:
 *  - \ref asfdoc_samb_wdt_extra_acronyms
 *  - \ref asfdoc_samb_wdt_extra_dependencies
 *  - \ref asfdoc_samb_wdt_extra_errata
 *  - \ref asfdoc_samb_wdt_extra_history
 *
 *
 * \section asfdoc_samb_wdt_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_samb_wdt_exqsg.
 *
 * \section asfdoc_samb_wdt_api_overview API Overview
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WDT_WRITE_ACCESS_KEY    0x1ACCE551

/**
 * \brief Watchdog Timer write access enum.
 *
 * Enum for the possible write access settings of the Watchdog timer module.
 */
enum wdt_write_access {
	/** Write access to all other registers is enabled */
	WDT_WRITE_ACCESS_ENABLE = 0,
	/** Write access to all other registers is disabled */
	WDT_WRITE_ACCESS_DISABLE,
};

/**
 * \brief Watchdog Timer configuration structure.
 *
 *  Configuration structure for a Watchdog Timer instance. This
 *  structure should be initialized by the \ref wdt_get_config_defaults()
 *  function before being modified by the user application.
 */
struct wdt_config {
	/** Watchdog load value */
	uint32_t load_value;
	/** Enable reset output */
	bool enable_reset;
	/** Enable interrupt output */
	bool enable_interrupt;
	/** Enable write access */
	enum wdt_write_access write_access;
};

/**
 * \brief WDT driver software device instance structure.
 *
 * WDT driver software instance structure, used to retain software
 * state information of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct wdt_module {
#if !defined(__DOXYGEN__)
	/** Pointer to the hardware instance. */
	Wdt *hw;
#endif
};

/**
 * \name Configuration and Initialization
 * @{
 */
void wdt_get_config_defaults(struct wdt_config *const config);
enum status_code wdt_set_config(struct wdt_module *const module, Wdt * const hw, \
			const struct wdt_config *const config);
/** @} */

/**
 * \name Disable and reset
 * @{
 */
void wdt_disable_clock(struct wdt_module *const module);
void wdt_disable_reset_output(struct wdt_module *const module);
void wdt_disable_interrupt_output(struct wdt_module *const module);
void wdt_reset(struct wdt_module *const module);
/** @} */

/**
 * \name Get and Clear status
 * @{
 */
uint8_t wdt_get_status(struct wdt_module *const module);
uint8_t wdt_get_status_raw(struct wdt_module *const module);
void wdt_clear_status(struct wdt_module *const module);
/** @} */

/**
 * \name Judge lock and Reload count value
 * @{
 */
bool wdt_is_locked(struct wdt_module *const module);
enum status_code wdt_reload_count(struct wdt_module *const module, \
			uint32_t load_value);
void wdt_get_current_count(struct wdt_module *const module, \
			uint32_t * count_value);
/** @} */

/**
 * \name Intergration Test Opearation
 * @{
 */
void wdt_integration_test_enable(void);
void wdt_integration_test_disable(void);
void wdt_integration_test_set(bool interrupt_output, bool reset_output);
/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page asfdoc_samb_wdt_extra Extra Information for WDT Driver
 *
 * \section asfdoc_samb_wdt_extra_acronyms Acronyms
 * The table below presents the acronyms used in this module:
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
 *
 * \section asfdoc_samb_wdt_extra_dependencies Dependencies
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samb_wdt_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samb_wdt_extra_history Module History
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
 * \page asfdoc_samb_wdt_exqsg Examples for WDT Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samb_wdt_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_samb_wdt_basic_use_case
 *
 * \page asfdoc_samb_wdt_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>07/2015</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#endif /* WDT_H_INCLUDED */
