/**
 * \file
 *
 * \brief SAM0+ Clock related functionality
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

#include <clock.h>

/* \ingroup clock_group
 * @{
 */

/** Internal variable to keep external oscillator frequency */
static uint32_t xosc_frequency = 0;

/**
 * \brief Get clock source frequency
 *
 * This function will return the frequency of the given clock source
 *
 *
 * \param[in] clk_source Clock source to get frequency of
 *
 * \returns Frequency of the given clock source
 */
uint32_t system_clock_source_get_hz(enum system_clock_source clk_source)
{
	uint32_t prescaler = 0;
	switch (clk_source) {

		case CLOCK_SOURCE_XOSC:
			return xosc_frequency;

		case CLOCK_SOURCE_RC8MHZ:
			prescaler = (SYSCTRL.OSC8M & SYSCTRL_PRESC_gm) >> SYSCTRL_PRESC_gp;
			if (prescaler) {
				return 8000000 / prescaler;
			} else {
				return 8000000;
			}

		case CLOCK_SOURCE_RC32KHZ:
			/* Fall trough */
		case CLOCK_SOURCE_ULP32KHZ:
			/* Fall trough */
		case CLOCK_SOURCE_XOSC32K:
			return 32000000UL;

		case CLOCK_SOURCE_DFLL:
			/* get_generator_hz * dfll_multiply_factor */
			return 48000000;

		default:
			return 0;

	}

}






/**
 * \brief Apply configuration for the selected clock source
 *
 * This function will apply the settings in the config struct to the
 * clock source selected. Only fields that are relevant to the selected
 * clock source will be modified, and unrelated settings in the config
 * struct will be ignored. This function will not enable the clock source,
 * this must be done by the \ref system_clock_source_enable function.
 *
 * This function will disable the clock source before applying the configuration
 *
 * \param[in] conf Clock source configuration
 * \param[in] clk_source Clock source to configure
 *
 * \retval STATUS_OK The operation completed successfully
 * \retval STATUS_ERR_UNAVAILABLE The selected clock source is not available
 */
enum status_code system_clock_source_set_config(struct system_clock_source_config
		*conf, enum system_clock_source clk_source)
{
	Assert(conf);

	uint16_t temp_register = 0;

	/* Disable the clock source if running */
	system_clock_source_disable(clk_source);

	switch (clk_source) {
		case CLOCK_SOURCE_RC8MHZ:
			SYSCTRL.OSC8M = conf->rc8mhz.prescaler << SYSCTRL_PRESC_gp;
			break;

		case CLOCK_SOURCE_RC32KHZ:
			/* Need to enable before we can configure */
			system_clock_source_enable(CLOCK_SOURCE_RC32KHZ, true);
			/* TODO: verify that this is always enabled */
			break;

		case CLOCK_SOURCE_XOSC:
			temp_register = conf->ext.startup_time;
			if (conf->ext.external_clock) {
				temp_register |= SYSCTRL_XTALEN_bm;
				if (conf->ext.auto_gain_control) {
					temp_register |= SYSCTRL_AMPGC_bm;
				}
			}

			SYSCTRL.XOSC = temp_register;
			break;

		case CLOCK_SOURCE_XOSC32K:
			temp_register = conf->ext.startup_time;

			if (conf->ext.external_clock) {
				temp_register |= SYSCTRL_XTALEN_bm;
				if (conf->ext.auto_gain_control) {
					/* Automatic Amplitude control */
					temp_register |= SYSCTRL_AAMPEN_bm;
				}
			}

			if (conf->osc32k.enable_1khz_output) {
				temp_register |= SYSCTRL_EN1K_bm;
			}

			if (conf->osc32k.enable_32khz_output) {
				temp_register |= SYSCTRL_EN32K_bm;
}

			SYSCTRL.XOSC32K = temp_register;
			break;

		case CLOCK_SOURCE_DFLL:
			temp_register |= conf->dfll.loop | conf->dfll.wakeup_lock |
					conf->dfll.stable_tracking | conf->dfll.quick_lock |
					conf->dfll.chill_cycle;

			clock_dfll_wait_for_sync();
			SYSCTRL.DFLLCTRL = temp_register;

			clock_dfll_wait_for_sync();
			SYSCTRL.DFLLVAL = conf->dfll.coarse_value |
					(conf->dfll.fine_value << SYSCTRL_FIN_gp);

			if (conf->dfll.loop == SYSCTRL_DFLL_CLOSED_LOOP) {
				clock_dfll_wait_for_sync();
				SYSCTRL.DFLLSTEP = conf->dfll.coarse_max_step |
						(conf->dfll.fine_max_step << SYSCTRL_FSTEP_gp);
			}
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
		}
	return STATUS_OK;
}

/**
 * \brief Write oscillator calibration value
 *
 * This function will write an oscillator calibration value to the oscillator
 * control register. The ranges are:
 * - OSC32K
 *  - 7 bits (max value 128)
 * - OSC8MHZ
 *  - 8 bits (Max value 255)
 * - OSCULP
 *  - 5 bits (Max value 32)
 *
 * \note Frequency range only applies when configuring the 8MHz oscillator
 * and will be ignored for the other oscillators.
 *
 * \param[in] clock_source Clock source to calibrate
 * \param[in] calibration_value Calibration value to write
 * \param[in] freq_range Frequency range (Only applicable to the 8MHz oscillator)
 *
 * \retval STATUS_ERR_INVALID_ARG The selected clock source is not available
 */
enum status_code clock_source_write_calibration(
		enum clock_source clock_source,
		uint16_t calibration_value, uint8_t freq_range)
{

	switch (clock_source) {
		case CLOCK_SOURCE_RC8MHZ:

			if (calibration_value > 255 || freq_range > 4) {
				return STATUS_ERR_INVALID_ARG;
			}

			SYSCTRL.OSC8M |= calibration_value << SYSCTRL_CALIB_gp |
					freq_range << SYSCTRL_FRANGE_gp;
			break;

		case CLOCK_SOURCE_RC32KHZ:

			if (calibration_value > 128) {
				return STATUS_ERR_INVALID_ARG;
			}

			clock_osc32k_wait_for_sync();
			SYSCTRL.OSC32K |= calibration_value;
			break;

		case CLOCK_SOURCE_ULP32KHZ:

			if (calibration_value > 32) {
				return STATUS_ERR_INVALID_ARG;
			}
			SYSCTRL.OSCULP32K |= calibration_value;
			break;

		default:
			Assert(!"Invalid clock source provided");
			return STATUS_ERR_INVALID_ARG;
			break;
	}

	return STATUS_OK;
}

/**
 * \brief Enable a clock source
 *
 * This function will enable the selected clock source
 *
 *  \param[in] block_until_ready block until the clock source has been enabled.
 *  \param[in] clock_source Clock source to enable
 *
 *  \retval STATUS_OK Clock source was enabled successfully and is ready
 *  \retval STATUS_ERR_INVALID_ARG The clock source is not available on this device
 *  \retval STATUS_TIMEOUT The clock source did not start (timeout)
 */

enum status_code clock_source_enable(enum clock_source clock_source, bool block_until_ready)
{
	/* Default value is 0xFFFFFFFF for timeout*/
	uint32_t timeout;
	uint32_t waitmask;

	/* TODO: Check _bm naming; this is bit 1 for all ENABLE bits */
	switch (clock_source) {
		case CLOCK_SOURCE_RC8MHZ:
			SYSCTRL.OSC8M |= SYSCTRL_RC8MHZ_ENABLE_bm;
			/* Not possible to wait for ready, so we return */
			return STATUS_OK;

		case CLOCK_SOURCE_RC32KHZ:
			SYSCTRL.OSC32K |= SYSCTRL_OSC32K_ENABLE_bm;
			waitmask = SYSCTRL_OSC32KRDY_bm;
			break;

		case CLOCK_SOURCE_XOSC:
			SYSCTRL.XOSC |= SYSCTRL_XOSC_ENABLE_bm;
			waitmask = SYSCTRL_XOSCRDY_bm;
			break;

		case CLOCK_SOURCE_XOSC32K:
			SYSCTRL.XOSC32K |= SYSCTRL_XOSC32K_ENABLE_bm;
			waitmask = SYSCTRL_XOSC32KRDY_bm;
			break;

		case CLOCK_SOURCE_DFLL:
			SYSCTRL.DFLLCTRL |= SYSCTRL_DFLL_ENABLE_bm;
			waitmask = SYSCTRL_DFLLRDY_bm;
			break;
		case CLOCK_SOURCE_ULP32KHZ:
			/* Always enabled */
			return STATUS_OK;
		default:
			Assert(!"Invalid clock source supplied");
			return STATUS_ERR_INVALID_ARG;
	}

	if (block_until_ready == true) {
		/* Wait for the clock source to be ready or timeout */
		for (timeout = 0; timeout < CONF_CLOCK_TIMEOUT; timeout++) {
			if(SYSCTRL.PCLKSR & waitmask) {
				return STATUS_OK;
			}
		}
		return STATUS_ERR_TIMEOUT;

	} else {
		return STATUS_OK;
	}
}

/**
 * \brief Disable a clock source
 *
 * This function will disable the selected clock source
 *
 *  \param[in] clk_source clock source to disable
 *
 *  \retval STATUS_OK Clock source was disabled successfully
 *  \retval STATUS_ERR_INVALID_ARG the clock source is not available on this device
 */
enum status_code system_clock_source_disable(enum system_clock_source clk_source)
{
	switch (clk_source) {
		case CLOCK_SOURCE_RC8MHZ:
			SYSCTRL.OSC8M &= ~SYSCTRL_RC8MHZ_ENABLE_bm;
			break;
		case CLOCK_SOURCE_RC32KHZ:
			SYSCTRL.OSC32K &= ~SYSCTRL_OSC32K_ENABLE_bm;
			break;
		case CLOCK_SOURCE_XOSC:
			SYSCTRL.XOSC &= ~SYSCTRL_XOSC_ENABLE_bm;
			break;
		case CLOCK_SOURCE_XOSC32K:
			SYSCTRL.XOSC32K &= ~SYSCTRL_XOSC32K_ENABLE_bm;
			break;
		case CLOCK_SOURCE_DFLL:
			SYSCTRL.DFLLCTRL &= ~SYSCTRL_DFLL_ENABLE_bm;
			break;
		case CLOCK_SOURCE_ULP32KHZ:
			/* Not possible to disable */
		default:
			return STATUS_ERR_INVALID_ARG;
	}
	return STATUS_OK;
}

/**
 * \brief Check if a clock source is ready
 *
 * This function will return if the selected clock source is ready to use.
 *
 *	\param[in] clk_source Clock source to check if ready
 *
 *  \retval true Clock source is enabled and ready
 *  \retval false Clock source is either disabled or not yet ready
 */
bool system_clock_source_is_ready(enum system_clock_source clk_source)
{
	uint32_t mask;
	switch (clk_source) {
		case CLOCK_SOURCE_RC8MHZ:
			/* TODO: verify that this cannot be disabled */
			return true;
		case CLOCK_SOURCE_RC32KHZ:
			mask = SYSCTRL_OSC32KRDY_bm;
			break;
		case CLOCK_SOURCE_XOSC:
			mask = SYSCTRL_XOSCRDY_bm;
			break;
		case CLOCK_SOURCE_XOSC32K:
			mask = SYSCTRL_XOSC32KRDY_bm;
			break;
		case CLOCK_SOURCE_DFLL:
			mask = SYSCTRL_DFLLRDY_bm;
			break;
		case CLOCK_SOURCE_ULP32KHZ:
			/* Not possible to disable */
		default:
			return false;
		}

	if(SYSCTRL.PCLKSR & mask) {
		return true;
	} else {
		return false;
	}
}
/* @} */

