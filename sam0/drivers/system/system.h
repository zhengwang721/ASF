/**
 * \file
 *
 * \brief SAMD20 System related functionality
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
#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <compiler.h>

/**
 * \defgroup asfdoc_samd20_system_group SAMD20 System Driver (SYSTEM)
 *
 * This driver for SAMD20 devices provides an interface for the configuration
 * and management of the device's system relation functionality, necessary for
 * the basic device operation. This is not limited to a single peripheral, but
 * extends across multiple hardware peripherals,
 *
 * The following peripherals are used by this module:
 *
 * - SYSCTRL (System Control) for Clock control
 * - PM (Power Manager) for Reset Cause determination, Peripheral Bus Control and Sleep
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_samd20_system_prerequisites
 *  - \ref asfdoc_samd20_system_module_overview
 *  - \ref asfdoc_samd20_system_special_considerations
 *  - \ref asfdoc_samd20_system_extra_info
 *  - \ref asfdoc_samd20_system_examples
 *  - \ref asfdoc_samd20_system_api_overview
 *
 *
 * \section asfdoc_samd20_system_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samd20_system_module_overview Module Overview
 *
 * The System driver provides a collection of interfaces between the user
 * application logic, and the core device functionality (such as clocks, reset
 * cause determination, etc.) that is required for all applications. It contains
 * a number of sub-modules that control one specific aspect of the device.
 *
 * \section asfdoc_samd20_system_module_reset_cause System Reset Cause
 * In some application there might be a need to perform a different program
 * flow based on how the device was reset. For example, if the cause of reset
 * was the Watchdog timer (WDT), this might indicate an error in the application
 * and a form of error handling or error logging might be needed.
 *
 * \section asfdoc_samd20_system_module_sleep_mode Sleep Modes
 * The SAMD20 devices have several sleep modes, where the sleep mode controls
 * which clock systems on the device will remain enabled or disabled when the
 * device enters a low power sleep mode. The table below lists the clock
 * settings of the different sleep modes.
 *
 * <table>
 * 	<tr>
 * 		<th>Sleep mode</th>
 * 		<th>IDLE</th>
 * 		<th>CPU clock</th>
 * 		<th>AHB clock</th>
 * 		<th>APB clocks</th>
 * 		<th>Clock sources</th>
 * 		<th>System clock</th>
 * 		<th>32kHz</th>
 * 		<th>Regulator mode</th>
 * 		<th>RAM mode</th>
 * 	</tr>
 * 	<tr>
 * 		<td rowspan="4">IDLE</td>
 * 		<td>0</td>
 * 		<td>Stop</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td rowspan="4">Normal Mode (LDO)</td>
 * 		<td rowspan="4">Normal Mode</td>
 * 	</tr>
 * 	<tr>
 * 		<td>1</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 *	</tr>
 * 	<tr>
 * 		<td>2</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 *	</tr>
 * 	<tr>
 * 		<td>3</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Run</td>
 * 		<td>Run</td>
 *	</tr>
 * 	<tr>
 * 		<td>STANDBY</td>
 * 		<td> </td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Stop</td>
 * 		<td>Low Power</td>
 * 		<td>Source/Drain biasing</td>
 * 	</tr>
 * </table>
 *
 *
 * \section asfdoc_samd20_system_special_considerations Special Considerations
 *
 * TODO
 *
 * \section asfdoc_samd20_system_extra_info Extra Information for SYSTEM
 *
 * For extra information see \ref asfdoc_samd20_system_extra. This includes:
 *  - \ref asfdoc_samd20_system_extra_acronyms
 *  - \ref asfdoc_samd20_system_extra_dependencies
 *  - \ref asfdoc_samd20_system_extra_errata
 *  - \ref asfdoc_samd20_system_extra_history
 *
 *
 * \section asfdoc_samd20_system_examples Examples
 *
 * The following Quick Start guides and application examples are available for this driver:
 * - \ref asfdoc_samd20_system_basic_use_case
 *
 *
 * \section asfdoc_samd20_system_api_overview API Overview

 */

#if !defined(__DOXYGEN__)
/* Weak init function used in system_init */
static inline void system_dummy_init(void)
{
	return;
}

#  ifdef __GNUC__
void system_board_init ( void ) WEAK __attribute__((alias("system_dummy_init")));
void system_clock_init ( void ) WEAK __attribute__((alias("system_dummy_init")));
#  endif
#  ifdef __ICCARM__
static inline void system_board_init(void);
static inline void system_clock_init(void);
#    pragma weak system_board_init=system_dummy_init
#    pragma weak system_clock_init=system_dummy_init
#  endif
#endif

/**
 * \brief Voltage references within the device.
 *
 * List of available voltage references (VREF) that may be used within the
 * device.
 */
enum system_voltage_reference {
	/** Temperature sensor voltage reference */
	SYSTEM_VOLTAGE_REFERENCE_TEMPSENSE,
	/** Bandgap voltage reference */
	SYSTEM_VOLTAGE_REFERENCE_BANDGAP,
};

/**
 * \brief Device sleep modes.
 *
 * List of available sleep modes in the device. A table of clocks available in
 * different sleep mode can be found in \ref asfdoc_samd20_system_module_sleep_mode.
 */
enum system_sleepmode {
	/** IDLE 0 */
	SYSTEM_SLEEPMODE_IDLE_0,
	/** IDLE 1 */
	SYSTEM_SLEEPMODE_IDLE_1,
	/** IDLE 2 */
	SYSTEM_SLEEPMODE_IDLE_2,
	/** IDLE 3 */
	SYSTEM_SLEEPMODE_IDLE_3,
	/** Standby */
	SYSTEM_SLEEPMODE_STANDBY,
};

/**
 * \brief Reset causes of the system.
 *
 * List of possible reset causes of the system.
 */
enum system_reset_cause {
	/** The system was reset by the watchdog timer. */
	SYSTEM_RESET_CAUSE_WDT            = PM_RCAUSE_WDT,
	/** The system was reset because the external reset line was pulled low. */
	SYSTEM_RESET_CAUSE_EXTERNAL_RESET = PM_RCAUSE_EXT,
	/** The system was reset by the BOD33. */
	SYSTEM_RESET_CAUSE_BOD33          = PM_RCAUSE_BOD33,
	/** The system was reset by the BOD12. */
	SYSTEM_RESET_CAUSE_BOD12          = PM_RCAUSE_BOD12,
	/** The system was reset by the POR (Power on reset). */
	SYSTEM_RESET_CAUSE_POR            = PM_RCAUSE_POR,
};

/**
 * \brief Enable the selected voltage reference
 *
 * This function will enable the selected voltage reference, making the voltage
 * reference available on a pin as well as an input source to the analog
 * peripherals.
 *
 * \param[in] vref Voltage reference to enable
 */
static inline void system_vref_enable(
		const enum system_voltage_reference vref)
{
	switch (vref) {
		case SYSTEM_VOLTAGE_REFERENCE_TEMPSENSE:
			SYSCTRL->VREF.reg |= SYSCTRL_VREF_TSEN;
			break;
		case SYSTEM_VOLTAGE_REFERENCE_BANDGAP:
			SYSCTRL->VREF.reg |= SYSCTRL_VREF_BGOUTEN;
			break;
		default:
			Assert(false);
			return;
	}
}

/**
 * \brief Disable the selected voltage reference
 *
 * This function will disable the selected voltage reference
 *
 * \param[in] vref Voltage reference to disable
 */
static inline void system_vref_disable(
		const enum system_voltage_reference vref)
{
	switch (vref) {
		case SYSTEM_VOLTAGE_REFERENCE_TEMPSENSE:
			SYSCTRL->VREF.reg &= ~SYSCTRL_VREF_TSEN;
			break;
		case SYSTEM_VOLTAGE_REFERENCE_BANDGAP:
			SYSCTRL->VREF.reg &= ~SYSCTRL_VREF_BGOUTEN;
			break;
		default:
			Assert(false);
			return;
	}
}

/**
 * \name Device sleep
 * @{
 */

/**
 * \brief Set the sleep mode of the device
 *
 * This function will set the sleep mode of the device. The possible sleep modes
 * are:
 *
 * - SYSTEM_SLEEPMODE_IDLE_0
 * - SYSTEM_SLEEPMODE_IDLE_1
 * - SYSTEM_SLEEPMODE_IDLE_2
 * - SYSTEM_SLEEPMODE_IDLE_3
 * - SYSTEM_SLEEPMODE_STANDBY
 *
 * For an overview of what are being disabled in sleep for the different sleep
 * modes, see \ref system_sleepmode documentation
 *
 * \param[in] sleepmode Sleepmode to set
 *
 * \retval STATUS_OK Operation performed successfully
 * \retval STATUS_ERR_INVALID_ARG The supplied sleep mode is not available
 */
static inline enum status_code system_set_sleepmode(
	const enum system_sleepmode sleepmode)
{
	switch (sleepmode) {
		case SYSTEM_SLEEPMODE_IDLE_0:
		case SYSTEM_SLEEPMODE_IDLE_1:
		case SYSTEM_SLEEPMODE_IDLE_2:
		case SYSTEM_SLEEPMODE_IDLE_3:
			//TODO: is the sleepdeep bit in the CPU ?
			//CPU.SCR &= ~SCR_SLEEPDEEP_bm;
			PM->SLEEP.reg = sleepmode;
			break;
		case SYSTEM_SLEEPMODE_STANDBY:
			/* TODO: Find core register for this */
			//CPU.SCR |= SCR_SLEEPDEEP_bm;
			break;
		default:
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/**
 * \brief Put the system to sleep waiting for interrupt
 *
 * This will execute the WFI (wait for interrupt) instruction,
 * putting the device into the sleep mode specified by \ref system_set_sleepmode
 * and wait for an interrupt to wake up
 */
static inline void system_sleep(void)
{
	__WFI();
}

/**
 * @}
 */

/**
 * \name Reset cause
 * @{
 */

/**
 * \brief Return the reset cause
 *
 * This function will return the cause of a system reset
 *
 * \return A enum value indicating the cause of the last system reset.
 */
static inline enum system_reset_cause system_get_reset_cause(void)
{
	return (enum system_reset_cause)PM->RCAUSE.reg;
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
 * \page asfdoc_samd20_system_extra Extra Information
 *
 * \section asfdoc_samd20_system_extra_acronyms Acronyms
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
 * \section asfdoc_samd20_system_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_samd20_system_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samd20_system_extra_history Module History
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
 * \page asfdoc_samd20_system_exqsg Examples for SYSTEM Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samd20_system_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_samd20_system_basic_use_case
 */

#endif /* SYSTEM_H_INCLUDED */
