/**
 * \file
 *
 * \brief SAM0+ System related functionality
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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
#ifndef SYSTEM_H
# define SYSTEM_H

#include <compiler.h>


/**
 * \defgroup system_group System control
 *
 *
 * \section intro Introduction
 *
 * This driver provides control of all the system related functionality
 * of the SAM0+ devices. This is not limited to a single peripheral, but
 * extends across multiple hardware peripherals:
 * - SYSCTRL (BOD12, BOD33)
 * - PM (reset cause)
* \dot
digraph overview {
	node [shape=record];
		SYSCTRL
		[label =<<TABLE BORDER="0" CELLBORDER="1" CELLPADDING="4" CELLSPACING="0">
				<TR><TD PORT="f0"><B>SYSCTRL</B></TD></TR>
				<TR><TD PORT="f1">BOD12</TD></TR>
				<TR><TD PORT="f2">BOD33</TD></TR>
			</TABLE>>
		shape = "none"];

		PM
		[label =<<TABLE BORDER="0" CELLBORDER="1" CELLPADDING="4" CELLSPACING="0">
				<TR><TD PORT="f0"><B>PM</B></TD></TR>
				<TR><TD PORT="f1">Reset Cause</TD></TR>
			</TABLE>>
		shape = "none"];
	}
 * \enddot
 *
 * \section bod BOD (Brown-Out Detector)
 *  The BOD monitors the supply voltage for any dips that go below the set BOD threshold.
 *  In case of a BOD detect the BOD will either reset the system or raise an interrupt.
 *
 *
 * \par BOD Configuration
 * The BOD configuration is done by using the \ref system_bod_set_config function.
 * This function uses a system_bod_config struct containing configuration
 * options to set either the BOD33 or the BOD12 to the selected settings.
 * To fill the system_bod_config struct with the system defaults you can
 * run the \ref system_bod_get_config_defaults passing you configuration struct
 * as the parameter. The default settings are listed \ref system_bod_get_config_defaults "here"
 *
 * \section reset_cause Reset Cause
 * In some application there might be a need to do a different program
 * flow based on how the controller was reset. Say if the cause of reset
 * was the watchdog timer (WDT), this might indicate an error in the application
 * and some sort of error handling or error logging might be needed.
 * The function system_get_reset_cause() returns the reset cause of latest reset.
 * The possible reset causes are enumerated in the enum \ref system_reset_cause.
 *
 * \section sleep_mode Sleep Modes
 * The SAM0+ have several sleep modes, where the sleep mode controls which clock
 * systems on the device are enabled/disabled when entering sleep. The table below
 * lists the clock settings of the different sleep modes
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
 *</table>
 *
 * \section etra_info Extra Information
 * For extra information see \ref system_extra_info
 *
* @{
 */

/**
 * BOD controller
*/
enum system_bod {
	/** BOD12 Internal core voltage*/
	SYSTEM_BOD12,
	/** BOD33 External IO voltage*/
	SYSTEM_BOD33,
};

/**
 * BOD Action on a BOD detect
 */
enum system_bod_action {
	/** A BOD detect will reset the device */
	SYSTEM_BOD_ACTION_RESET     = SYSCTRL_ACTION_RESET_gc, // 1
	/** A BOD detect will fire an interrupt */
	SYSTEM_BOD_ACTION_INTERRUPT = SYSCTRL_ACTION_INTERRUPT_gc, // 2
};

/**
 * BOD sampling mode */
enum system_bod_mode {
	/** BOD will sample the supply line continuously */
	SYSTEM_BOD_MODE_CONTINIOUS,
	/** BOD will use the BOD sampling clock (1kHz) to sample the supply line */
	SYSTEM_BOD_MODE_SAMPLED,
};

/**
 * Available voltage references
 */
enum system_voltage_reference {
	/** Temperature sensor voltage reference */
	SYSTEM_VREF_TEMPSENSE,
	/** Bandgap voltage reference */
	SYSTEM_VREF_BANDGAP,
};

/**
 * Available sleep modes in the device. A table of clocks available in different
 * sleep mode is available here: \ref sleep_mode
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
 * System reset cause
 */
enum system_reset_cause {
	/** The system was reset by the watchdog timer */
	SYSTEM_RESET_CAUSE_WDT,
	/** The system was reset because the external reset line was pulled low */
	SYSTEM_RESET_CAUSE_EXTERNAL_RESET,
	/** The system was reset by the BOD33 */
	SYSTEM_RESET_CAUSE_BOD33,
	/** The system was reset by the BOD12 */
	SYSTEM_RESET_CAUSE_BOD12,
	/** The system was reset by the POR (Power on reset). */
	SYSTEM_RESET_CAUSE_POR,
};



/**
 * Configuration struct for the BOD12 and BOD33
 */
struct system_bod_config {
	/** Sampled or continuous mode */
	enum system_bod_mode mode;
	/** Action on detect; reset or interrupt */
	enum system_bod_action action;
	/** BOD level */
	uint8_t level; /* TODO: document this more */
	/** Enable hysteresis */
	bool hysteresis;
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
static inline void system_vref_enable(enum system_voltage_reference vref) {
	switch(vref) {
	case SYSTEM_VREF_TEMPSENSE:
		SYSCTRL.VREFCTRLA |= SYSCTRL_VREFCTRLA_TSEN;
		break;
	case SYSTEM_VREF_BANDGAP:
		SYSCTRL.VREFCTRLA |= SYSCTRL_VREFCTRLA_BGOUTEN;
		break;
	default:
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
static inline void system_vref_disable(enum system_voltage_reference vref) {
	switch(vref) {
	case SYSTEM_VREF_TEMPSENSE:
		SYSCTRL.VREFCTRLA &= ~SYSCTRL_VREFCTRLA_TSEN;
		break;
	case SYSTEM_VREF_BANDGAP:
		SYSCTRL.VREFCTRLA &= ~SYSCTRL_VREFCTRLA_BGOUTEN;
		break;
	default:
		return;
	}
}

/**
 * \name BOD configuration
 * @{
 */

/**
 * \brief Get default BOD configuration
 *
 * The default BOD configuration is:
 * - Continuous mode
 * - Reset on BOD detect
 * - Hysteresis enabled
 * - BOD level 0x12
 *
 * \param[out] conf BOD configuration struct to set to default settings
 */
static inline void system_bod_get_config_defaults(
		struct system_bod_config *conf)
{
	Assert(conf);

	conf->mode = SYSTEM_BOD_MODE_CONTINIOUS;
	conf->action = SYSTEM_BOD_ACTION_RESET;
	conf->level = 0x12; /* TODO: Need to revisit this level */
	conf->hysteresis = true;
}

enum status_code system_bod_set_config(struct system_bod_config *conf,
		enum system_bod bod);

/**
 * @}
 */

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
static inline enum status_code system_set_sleepmode(enum system_sleepmode sleepmode)
{

	switch (sleepmode) {
		case SYSTEM_SLEEPMODE_IDLE_0:
		case SYSTEM_SLEEPMODE_IDLE_1:
		case SYSTEM_SLEEPMODE_IDLE_2:
		case SYSTEM_SLEEPMODE_IDLE_3:
			//TODO: is the sleepdeep bit in the CPU ?
			//CPU.SCR &= ~SCR_SLEEPDEEP_bm;
			PM.SLEEP = sleepmode;
			break;
		case SYSTEM_SLEEPMODE_STANDBY:
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
	asm volatile ("wfi\n");
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
 * \retval SYSTEM_RESET_CAUSE_WDT The last system reset was from the Watchdog timer
 * \retval SYSTEM_RESET_CAUSE_EXTERNAL_RESET Last system reset was from the
 * external reset pin
 * \retval SYSTEM_RESET_CAUSE_BOD33 Last system reset was from the BOD33
 * \retval SYSTEM_RESET_CAUSE_BOD12 Last system reset was from the BOD12
 * \retval SYSTEM_RESET_CAUSE POR Last system reset was from the power-on-reset
 * (power up)
 */
static inline enum system_reset_cause system_get_reset_cause(void)
{
	return PM.RCAUSE;
}

/**
 * @}
 */

/* @} */

void system_bod_init(void);

#endif /* SYSTEM_H */

/**
 * \page system_extra_info Extra Information (system)
 * Below is a listing of the acronyms used in this module:
 *
 * \section acronyms Acronyms
 * List of acronyms used in the module.
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *      <td>BOD</td>
 *      <td>Brownout detector</td>
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
 * \section fixed_erratas Erratas fixed by driver
 * No errata workarounds in driver
 *
 * \section sys_module_history Module History
 * <table>
 * 	<tr>
 * 		<th>Changelog</th>
 * 	</tr>
 * 	<tr>
 * 		<td>Initial version</td>
 * 	</tr>
 * </table>
 *
 */
