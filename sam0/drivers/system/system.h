/**
 * \file
 *
 * \brief SAM System related functionality
 *
 * Copyright (C) 2012-2014 Atmel Corporation. All rights reserved.
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
#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <compiler.h>
#include <clock.h>
#include <gclk.h>
#include <pinmux.h>
#include <power.h>
#include <reset.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup asfdoc_sam0_system_group SAM System Driver (SYSTEM)
 *
 * This driver for SAM devices provides an interface for the configuration
 * and management of the device's system relation functionality, necessary for
 * the basic device operation. This is not limited to a single peripheral, but
 * extends across multiple hardware peripherals,
 *
 * The following peripherals are used by this module:
 * \if DEVICE_SAML21_SUPPORT
 *	- PM (Power Manager)
 *  - RSTC(Reset Controller)
 *  - SUPC(Supply Controller)
 *	\else
 * - SYSCTRL (System Control)
 * - PM (Power Manager)
 * \endif 
 *
 * The following devices can use this module:
 *  - SAM D20/D21
 *  - SAM R21
 *  - SAM D10/D11
 *  - SAM L21
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam0_system_prerequisites
 *  - \ref asfdoc_sam0_system_module_overview
 *  - \ref asfdoc_sam0_system_special_considerations
 *  - \ref asfdoc_sam0_system_extra_info
 *  - \ref asfdoc_sam0_system_examples
 *  - \ref asfdoc_sam0_system_api_overview
 *
 *
 * \section asfdoc_sam0_system_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam0_system_module_overview Module Overview
 *
 * The System driver provides a collection of interfaces between the user
 * application logic, and the core device functionality (such as clocks, reset
 * cause determination, etc.) that is required for all applications. It contains
 * a number of sub-modules that control one specific aspect of the device:
 *
 * - System Core (this module)
 * - \ref asfdoc_sam0_system_clock_group "System Clock Control" (sub-module)
 * - \ref asfdoc_sam0_system_interrupt_group "System Interrupt Control" (sub-module)
 * - \ref asfdoc_sam0_system_pinmux_group "System Pin Multiplexer Control" (sub-module)
 *
 *
 * \if DEVICE_SAML21_SUPPORT
 * \subsection asfdoc_sam0_system_module_overview_vreg Voltage Regulator
 * The SAML21 device controls the voltage regulators for the core (VDDCORE) and
 * backup (VDDBU) domains. It sets the voltage regulators according to the sleep modes, 
 * the performance level, or the user configuration. 
 *
 * In active mode, the voltage regulator can be chosen on the fly between a LDO or a 
 * Buck converter.In standby mode, the low power voltage regulator is used to supply 
 * VDDCORE.
 *
 * \subsection asfdoc_sam0_system_module_overview_bbps Battery Backup Power Switch
 * The SAML21 device supports connection of a battery backup to the VBAT power pin. 
 * It includes functionality that enables automatic power switching between main power 
 * and battery backup power. This will ensure power to the backup domain, when the main 
 * battery or power source is unavailable.
 * \endif 
 * 
 * \subsection asfdoc_sam0_system_module_overview_vref Voltage References
 * The various analog modules within the SAM devices (such as AC, ADC and
 * DAC) require a voltage reference to be configured to act as a reference point
 * for comparisons and conversions.
 *
 * The SAM devices contain multiple references, including an internal
 * temperature sensor, and a fixed band-gap voltage source. When enabled, the
 * associated voltage reference can be selected within the desired peripheral
 * where applicable.
 *
 * \subsection asfdoc_sam0_system_module_overview_reset_cause System Reset Cause
 * In some application there may be a need to execute a different program
 * flow based on how the device was reset. For example, if the cause of reset
 * was the Watchdog timer (WDT), this might indicate an error in the application
 * and a form of error handling or error logging might be needed.
 *
 * For this reason, an API is provided to retrieve the cause of the last system
 * reset, so that appropriate action can be taken.
 *
 * \if DEVICE_SAML21_SUPPORT
 * For SAM L21,There are three groups of reset sources:
 *
 * Power supply reset: Resets caused by an electrical issue. It covers POR and BODs reset.
 *
 * User reset: Resets caused by the application. It covers external reset, system reset 
 * request and watchdog reset.
 *
 * Backup reset: Resets caused by a backup mode exit condition.
 *
 * \subsection asfdoc_sam0_system_module_overview_performance_level Performance Level
 * Performance level allows use to adjust the regulator output voltage to reduce power consumption. 
 * The user can select on the fly the performance level configuration which best suits its application.
 *
 * The SAM L21 device embeds up to three performance level (PL0, PL1 and PL2).Each performance level
 * defines a maximum frequency and a corresponding consumption in μA/MHz,when the application selects a new
 * performance level, the voltage applied on the full logic area moves from a value to another, it can reduce
 * the active consumption while decreasing the maximum frequency of the device 
 *
 * Performance level transition is possible only when the device is in active mode,after a reset, the device starts
 * in the lowest performance level (lowest power consumption and lowest max frequency). The application can then 
 * switch to another performance level at anytime without any stop in the code execution. As shown in
 * \ref asfdoc_sam0_system_performance_level_transition_figure 
 *
 * \anchor asfdoc_sam0_system_performance_level_transition_figure
 * \image html performance_level_transition.gif "The performance level  transition"
 
 * \subsection asfdoc_sam0_system_module_overview_power_domain Power Domain
 * Power domain gating  can  turn on or off power domain voltage to save power while keeping other
 * domain powered up. It can be used in standby sleep mode,in standby mode, when power-gated, the internal
 * state of the logic can be retained  allowing the application context to be kept.
 *
 * The SAM L21 device has five power domains.
 *
 * Switchable power domains (PD0, PD1 and PD2): these power domains uses the power domain gating
 * technique and can then be set to either active, retention or off state. 
 *
 * Always-on power domain(PDTOP): this power domain is either in active or off state
 *
 * Backup power domain(PDBACKUP): this power domain is always in active state (except in off sleep mode).
 * There are four cases to consider in standby mode:
 *
 * Case 1:PD0, PD1 and PD2  are running.
 *
 * Case 2:PD0 and PD1  are running while PD2 is in retention.
 *
 * Case 3:PD0 is running while PD1 and PD2 are in retention state.
 *
 * Case 4:PD0, PD1 and PD2  are in retention state
 *
 * Here is an overview of sleep mode versus power domain state.As shown in
 * \ref asfdoc_sam0_system_power_domain_overview_table
 *
 * \anchor asfdoc_sam0_system_power_domain_overview_table
 * <table>
 *  <caption>Sleep Mode versus Power Domain State Overview</caption>
 * 	<tr>
 * 		<th>Sleep mode</th>
 * 		<th></th>
 * 		<th>PD0</th>
 * 		<th>PD1</th>
 * 		<th>PD2</th>
 * 		<th>PDTOP</th>
 * 		<th>PDBACKUP</th>
 * 	</tr>
 * 	<tr>
 * 		<td>IDLE</td>
 * 		<td></td>
 * 		<td>active</td>
 * 		<td>active</td>
 * 		<td>active</td>
 * 		<td>active</td>
 * 		<td>active</td>
 * 	</tr>
 * 	<tr>
 * 		<td>Standby</td>
 * 		<td>Case 1</td>
 * 		<td>active</td>
 * 		<td>active</td>
 * 		<td>active</td>
 * 		<td>active</td>
 * 		<td>active</td>
 *	</tr>
 * 	<tr>
 * 		<td>Standby</td>
 * 		<td>Case 2</td>
 * 		<td>active</td>
 * 		<td>active</td>
 * 		<td>retention</td>
 * 		<td>active</td>
 * 		<td>active</td>
 *	</tr>
 * 	<tr>
 * 		<td>Standby</td>
 * 		<td>Case 3</td>
 * 		<td>active</td>
 * 		<td>retention</td>
 * 		<td>retention</td>
 * 		<td>active</td>
 * 		<td>active</td>
 *	</tr>
 * 	<tr>
 * 		<td>Standby</td>
 * 		<td>Case 4</td>
 * 		<td>retention</td>
 * 		<td>retention</td>
 * 		<td>retention</td>
 * 		<td>active</td>
 * 		<td>active</td>
 *	</tr>
 * 	<tr>
 * 		<td>Backup</td>
 * 		<td></td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>active</td>
 *	</tr>
 * 	<tr>
 * 		<td>Off</td>
 * 		<td></td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 	</tr>
 * </table>
 *
 * \endif 
 *
 * \subsection asfdoc_sam0_system_module_overview_sleep_mode Sleep Modes
 * The SAM devices have several sleep modes, where the sleep mode controls
 * which clock systems on the device will remain enabled or disabled when the
 * device enters a low power sleep mode.
 * \ref asfdoc_sam0_system_module_sleep_mode_table "The table below" lists the
 * clock settings of the different sleep modes.
 *
 * \anchor asfdoc_sam0_system_module_sleep_mode_table
 * <table>
 *  <caption>SAM Device Sleep Modes</caption>
 * \if DEVICE_SAML21_SUPPORT
 * 	<tr>
 * 		<th>Sleep mode</th>
 * 		<th>System clock</th>
 * 		<th>CPU clock</th>
 * 		<th>AHB/AHB clock</th>
 * 		<th>GCLK clocks</th>
 * 		<th>Oscillators (ONDEMAND  = 0)</th>
 * 		<th>Oscillators (ONDEMAND  = 1)</th>
 * 		<th>Regulator mode</th>
 * 		<th>RAM mode</th>
 * 	</tr>
 * 	<tr>
 * 		<td>IDLE</td>
 * 		<td>Run</td>
 * 		<td>Stop</td>
 * 		<td>Run if requested</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run if requested</td>
 * 		<td>Normal</td>
 * 		<td>Normal</td>
 * 	</tr>
 * 	<tr>
 * 		<td>Standby</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Run if requested</td>
 * 		<td>Run if requested</td>
 * 		<td>Run if requested or RUNSTDBY  = 1</td>
 * 		<td>Run if requested</td>
 * 		<td>Low pwer</td>
 * 		<td>Low pwer</td>
 *	</tr>
 * 	<tr>
 * 		<td>Backup</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Backup</td>
 * 		<td>OFF</td>
 *	</tr>
 * 	<tr>
 * 		<td>Off</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 		<td>OFF</td>
 * 	</tr>
 *	\else
 * 	<tr>
 * 		<th>Sleep mode</th>
 * 		<th>CPU clock</th>
 * 		<th>AHB clock</th>
 * 		<th>APB clocks</th>
 * 		<th>Clock sources</th>
 * 		<th>System clock</th>
 * 		<th>32KHz</th>
 * 		<th>Reg mode</th>
 * 		<th>RAM mode</th>
 * 	</tr>
 * 	<tr>
 * 		<td>IDLE 0</td>
 * 		<td>Stop</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Normal</td>
 * 		<td>Normal</td>
 * 	</tr>
 * 	<tr>
 * 		<td>IDLE 1</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Normal</td>
 * 		<td>Normal</td>
 *	</tr>
 * 	<tr>
 * 		<td>IDLE 2</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Normal</td>
 * 		<td>Normal</td>
 *	</tr>
 * 	<tr>
 * 		<td>STANDBY</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Low Power</td>
 * 		<td>Source/Drain biasing</td>
 * 	</tr>
 * \endif 
 * </table>
 *
 * To enter device sleep, one of the available sleep modes must be set, and the
 * function to enter sleep called. The device will automatically wake up in
 * response to an interrupt being generated or other device event.
 *
 * Some peripheral clocks will remain enabled during sleep, depending on their
 * configuration; if desired, modules can remain clocked during sleep to allow
 * them to continue to operate while other parts of the system are powered down
 * to save power.
 *
 *
 * \section asfdoc_sam0_system_special_considerations Special Considerations
 *
 * Most of the functions in this driver have device specific restrictions and
 * caveats; refer to your device datasheet.
 *
 *
 * \section asfdoc_sam0_system_extra_info Extra Information
 *
 * For extra information see \ref asfdoc_sam0_system_extra. This includes:
 *  - \ref asfdoc_sam0_system_extra_acronyms
 *  - \ref asfdoc_sam0_system_extra_dependencies
 *  - \ref asfdoc_sam0_system_extra_errata
 *  - \ref asfdoc_sam0_system_extra_history
 *
 *
 * \section asfdoc_sam0_system_examples Examples
 *
 * For SYSTEM module related examples, please refer to the sub-modules listed in
 * the \ref asfdoc_sam0_system_module_overview "system module overview".
 *
 *
 * \section asfdoc_sam0_system_api_overview API Overview
 * @{
 */

/**
 * \name System debugger
 * @{
 */

/**
 * \brief Check if debugger is present
 *
 * Check if debugger is connected to the onboard debug system (DAP)
 *
 * \return A bool identifying if a debugger is present
 *
 * \retval true  Debugger is connected to the system
 * \retval false Debugger is not connected to the system
 *
 */
static inline bool system_is_debugger_present(void)
{
	return DSU->STATUSB.reg & DSU_STATUSB_DBGPRES;
}

/**
 * @}
 */
 
/**
 * \name System identification
 * @{
 */

/**
 * \brief Retrieve the device identification signature
 *
 * Retrieves the signature of the current device.
 *
 * \return Device ID signature as a 32-bit integer.
 */
static inline uint32_t system_get_device_id(void)
{
	return DSU->DID.reg;
}

/**
 * @}
 */
 
/**
 * \name System initialization
 * @{
 */

void system_init(void);

/**
 * @}
 */


/**
 * @}
 */

/**
 * \page asfdoc_sam0_system_extra Extra Information for SYSTEM Driver
 *
 * \section asfdoc_sam0_system_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *		<td>PM</td>
 *		<td>Power Manager</td>
 *  </tr>
 *  <tr>
 *		<td>SYSCTRL</td>
 *		<td>System control interface</td>
 *  </tr>
 * </table>
 *
 *
 * \section asfdoc_sam0_system_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_sam0_system_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam0_system_extra_history Module History
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
 *		<td>Added low power features and  support for SAML21</td>
 *	</tr>
 *	<tr>
 *		<td>Added support for SAMD21</td>
 *	</tr>
 *	<tr>
 *		<td>Added new \c system_reset() to reset the complete MCU with some exceptions</td>
 *	</tr>
 *	<tr>
 *		<td>Added new \c system_get_device_id() function to retrieved the device
 *          ID.</td>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 *
 * \page asfdoc_sam0_system_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>E</td>
 *		<td>04/2014</td>
 *		<td>Added support for SAMD10/D11.</td>
 *	</tr>
 *	<tr>
 *		<td>D</td>
 *		<td>02/2014</td>
 *		<td>Added support for SAMR21.</td>
 *	</tr>
 *	<tr>
 *		<td>C</td>
 *		<td>01/2014</td>
 *		<td>Added support for SAMD21.</td>
 *	</tr>
 *	<tr>
 *		<td>B</td>
 *		<td>06/2013</td>
 *		<td>Corrected documentation typos.</td>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>06/2013</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_H_INCLUDED */
