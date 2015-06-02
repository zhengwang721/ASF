/**
 * \file
 *
 * \brief SAM Temperature Sensor (TSENS) Driver
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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

#include "tsens.h"

#define WINDOW_MIN_VALUE    -40
#define WINDOW_MAX_VALUE    105

/**
 * \internal Writes an TSENS configuration to the hardware module
 *
 * Writes out a given TSENS module configuration to the hardware module.
 *
 * \param[in]  config       Pointer to configuration struct
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK               The configuration was successful
 * \retval STATUS_ERR_INVALID_ARG  Invalid argument(s) were provided
 */
static enum status_code _tsens_set_config(struct tsens_config *const config)
{
	/* Configure GCLK channel and enable clock */
	struct system_gclk_chan_config gclk_chan_conf;
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	gclk_chan_conf.source_generator = config->clock_source;
	system_gclk_chan_set_config(TSENS_GCLK_ID, &gclk_chan_conf);
	system_gclk_chan_enable(TSENS_GCLK_ID);

	/* Configure run in standby */
	TSENS->CTRLA.reg = (config->run_in_standby << TSENS_CTRLA_RUNSTDBY_Pos);

	/* Check validity of window thresholds */
	if (config->window.window_mode != TSENS_WINDOW_MODE_DISABLE) {
		if((config->window.window_lower_value < WINDOW_MIN_VALUE) || \
			(config->window.window_upper_value > WINDOW_MAX_VALUE)) {
				return STATUS_ERR_INVALID_ARG;
			}
	}

	/* Configure CTRLC */
	TSENS->CTRLC.reg =
			(config->free_running << TSENS_CTRLC_FREERUN_Pos) | \
			(config->window.window_mode);

#if ERRATA_14476
	/* Configure lower threshold */
	TSENS->WINLT.reg = \
			config->window.window_upper_value << TSENS_WINLT_WINLT_Pos;

	/* Configure upper threshold */
	TSENS->WINUT.reg = \
			config->window.window_lower_value << TSENS_WINUT_WINUT_Pos;
#else
	/* Configure lower threshold */
	TSENS->WINLT.reg = \
			config->window.window_lower_value << TSENS_WINLT_WINLT_Pos;

	/* Configure upper threshold */
	TSENS->WINUT.reg = \
			config->window.window_upper_value << TSENS_WINUT_WINUT_Pos;
#endif

	/* Configure events */
	TSENS->EVCTRL.reg = config->event_action;

	/* Disable all interrupts */
	TSENS->INTENCLR.reg =
			(1 << TSENS_INTENCLR_OVF_Pos) | (1 << TSENS_INTENCLR_WINMON_Pos) | \
			(1 << TSENS_INTENCLR_OVERRUN_Pos) | (1 << TSENS_INTENCLR_RESRDY_Pos);

	/* Read calibration from NVM */
	uint32_t tsens_bits = *((uint32_t *)NVMCTRL_TEMP_LOG);
	uint32_t tsens_tcal = \
				((tsens_bits & TSENS_FUSES_TCAL_Msk) >> TSENS_FUSES_TCAL_Pos);
	uint32_t tsens_fcal = \
				((tsens_bits & TSENS_FUSES_FCAL_Msk) >> TSENS_FUSES_FCAL_Pos);

	TSENS->CAL.reg = TSENS_CAL_TCAL(tsens_tcal) | TSENS_CAL_FCAL(tsens_fcal);
	TSENS->GAIN.reg = TSENS_GAIN_GAIN(config->calibration.gain);
	TSENS->OFFSET.reg = TSENS_OFFSET_OFFSETC(config->calibration.offset);

	return STATUS_OK;
}

/**
 * \brief Initializes the TSENS.
 *
 * Initializes the TSENS device struct and the hardware module based on the
 * given configuration struct values.
 *
 * \param[in]  config      Pointer to the configuration struct
 *
 * \return Status of the initialization procedure.
 * \retval STATUS_OK                The initialization was successful
 * \retval STATUS_ERR_INVALID_ARG   Invalid argument(s) were provided
 * \retval STATUS_BUSY              The module is busy with a reset operation
 * \retval STATUS_ERR_DENIED        The module is enabled
 */
enum status_code tsens_init(struct tsens_config *config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Turn on the digital interface clock */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBA, MCLK_APBAMASK_TSENS);

	if (TSENS->CTRLA.reg & TSENS_CTRLA_SWRST) {
		/* We are in the middle of a reset. Abort. */
		return STATUS_BUSY;
	}

	if (TSENS->CTRLA.reg & TSENS_CTRLA_ENABLE) {
		/* Module must be disabled before initialization. Abort. */
		return STATUS_ERR_DENIED;
	}

	/* Write configuration to module */
	return _tsens_set_config(config);
}
