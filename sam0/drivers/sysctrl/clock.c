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
#include "clock.h"
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
uint32_t clock_source_get_hz(enum clock_source clk_source)
{
	uint32_t prescaler = 0;
	switch (clk_source) {

		case CLOCK_SOURCE_XOSC:
			return xosc_frequency;

		case CLOCK_SOURCE_RC8MHZ:
			prescaler = (SYSCTRL.OSC8M & SYSCTRL_PRESC_gm) >> SYSCTRL_PRESC_gp;
			if (prescaler) {
				return 8000000 / SYSCTRL.OSC;
			} else {
				return 8000000;
			}

		case CLOCK_SOURCE_RC32KHZ:
			/* Fall trough */
		case CLOCK_SOURCE_ULP32KHZ:
			/* Fall trough */
		case CLOCK_SOURCE_XOSC32K:
			return 32000000UL;

		case DFLL:
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
 * this must be done by the \ref clock_source_enable function.
 *
 * This function will disable the clock source before applying the configuration
 *
 * \param[in] conf Clock source configuration
 * \param[in] clk_source Clock source to configure
 *
 * \retval STATUS_OK The operation completed successfully
 * \retval STATUS_ERR_UNAVAILABLE The selected clock source is not available
 */
enum status_codes clock_source_set_config(struct clock_source_config
		*conf, enum clock_source clk_source)
{
	Assert(conf);

	uint16_t temp_register = 0;

	/* Disable the clock source if running */
	clock_source_disable(clk_source);

	switch (clk_source) {
		case CLOCK_SOURCE_RC8MHZ:
			SYSCTRL.OSC8M = conf->rc8mhz.prescaler << SYSCTRL_PRESC_gp;
			break;

		case CLOCK_SOURCE_RC32KHZ:
			/* Need to enable before we can configure */
			sysctrl_clock_source_enable(CLKSOURCE_RC32KHZ);
			/* TODO: verify that this is always enabled */
			break;

		case CLOCK_SOURCE_XOSC:
			temp_register = conf->external.startup_time;
			if (conf->ext.external_clock) {
				temp_register |= SYSCTRL_XTALEN_bm;
				if (conf->ext.auto_gain_control) {
					temp_register |= SYSCTRL_AMPGC_bm;
				}
			}

			SYSCTRL.XOSC = temp_register;
			break;

		case CLOCK_SOURCE_XOSC32K:
			temp_register = conf->startup_time;

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
					conf->chill_cycle;

			sysctrl_dfll_wait_for_sync();
			SYSCTRL.DFLLCTRL = temp_register;

			sysctrl_dfll_wait_for_sync();
			SYSCTRL.DFLLVAL = conf->dfll.coarse_value |
					(conf->dfll.fine_value << SYSCTRL_FIN_gp);

			if (conf->dfll.loop = SYSCTRL_DFLL_CLOSED_LOOP) {
				sysctrl_dfll_wait_for_sync();
				SYSCTRL.DFLLSTEP = conf->dfll.coarse_max_step |
						(conf->dfll.fine_max_step << SYSCTRL_FSTEP_gp);
			}
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
		}

}

/* @} */

