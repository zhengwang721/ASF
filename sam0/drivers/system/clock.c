/**
 * \file
 *
 * \brief SAMD20 Clock related functionality
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
#include <conf_clocks.h>

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

		case SYSTEM_CLOCK_SOURCE_XOSC:
			return xosc_frequency;

		case SYSTEM_CLOCK_SOURCE_RC8MHZ:
			prescaler = (SYSCTRL->OSC8M.reg & SYSCTRL_OSC8M_PRESC_Msk) >> SYSCTRL_OSC8M_PRESC_Pos;
			if (prescaler) {
				return 8000000 / prescaler;
			} else {
				return 8000000;
			}

		case SYSTEM_CLOCK_SOURCE_OSC32K:
			/* Fall trough */
		case SYSTEM_CLOCK_SOURCE_ULP32KHZ:
			/* Fall trough */
		case SYSTEM_CLOCK_SOURCE_XOSC32K:
			return 32000000UL;

		case SYSTEM_CLOCK_SOURCE_DFLL:
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
		case SYSTEM_CLOCK_SOURCE_RC8MHZ:
			SYSCTRL->OSC8M.bit.PRESC = conf->rc8mhz.prescaler;
			break;

		case SYSTEM_CLOCK_SOURCE_OSC32K:
			/* Need to enable before we can configure */
			system_clock_source_enable(SYSTEM_CLOCK_SOURCE_OSC32K, true);
			/* TODO: verify that this is always enabled */
			break;

		case SYSTEM_CLOCK_SOURCE_XOSC:
			temp_register = conf->ext.startup_time;
			if (conf->ext.external_clock) {
				temp_register |= SYSCTRL_XOSC_XTALEN;
				if (conf->ext.auto_gain_control) {
					temp_register |= SYSCTRL_XOSC_AMPGC;
				}
			}

			SYSCTRL->XOSC.reg = temp_register;
			break;

		case SYSTEM_CLOCK_SOURCE_XOSC32K:
			temp_register = conf->ext.startup_time;

			if (conf->ext.external_clock) {
				temp_register |= SYSCTRL_XOSC32K_XTALEN;
				if (conf->ext.auto_gain_control) {
					/* Automatic Amplitude control */
					temp_register |= SYSCTRL_XOSC32K_AAMPEN;
				}
			}

			if (conf->osc32k.enable_1khz_output) {
				temp_register |= SYSCTRL_XOSC32K_EN1K;
			}

			if (conf->osc32k.enable_32khz_output) {
				temp_register |= SYSCTRL_XOSC32K_EN32K;
}

			SYSCTRL->XOSC32K.reg = temp_register;
			break;

		case SYSTEM_CLOCK_SOURCE_DFLL:
			temp_register |= conf->dfll.loop | conf->dfll.wakeup_lock |
					conf->dfll.stable_tracking | conf->dfll.quick_lock |
					conf->dfll.chill_cycle;

			_system_dfll_wait_for_sync();
			SYSCTRL->DFLLCTRL.reg = temp_register;

			_system_dfll_wait_for_sync();
			SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(conf->dfll.coarse_value)
					| SYSCTRL_DFLLVAL_FINE(conf->dfll.fine_value);

			if (conf->dfll.loop == SYSTEM_CLOCK_DFLL_CLOSED_LOOP) {
				_system_dfll_wait_for_sync();
				SYSCTRL->DFLLMUL.reg =
						SYSCTRL_DFLLMUL_CSTEP(conf->dfll.coarse_max_step) |
						SYSCTRL_DFLLMUL_FSTEP(conf->dfll.fine_max_step);
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
 * \param[in] clock_src Clock source to calibrate
 * \param[in] calibration_value Calibration value to write
 * \param[in] freq_range Frequency range (Only applicable to the 8MHz oscillator)
 *
 * \retval STATUS_ERR_INVALID_ARG The selected clock source is not available
 */
enum status_code system_clock_source_write_calibration(
		enum system_clock_source clock_src,
		uint16_t calibration_value, uint8_t freq_range)
{

	switch (clock_src) {
		case SYSTEM_CLOCK_SOURCE_RC8MHZ:

			if (calibration_value > 255 || freq_range > 4) {
				return STATUS_ERR_INVALID_ARG;
			}

			SYSCTRL->OSC8M.bit.CALIB = calibration_value;
			SYSCTRL->OSC8M.bit.FRANGE = freq_range;
			break;

		case SYSTEM_CLOCK_SOURCE_OSC32K:

			if (calibration_value > 128) {
				return STATUS_ERR_INVALID_ARG;
			}

			_system_osc32k_wait_for_sync();
			SYSCTRL->OSC32K.bit.CALIB = calibration_value;
			break;

		case SYSTEM_CLOCK_SOURCE_ULP32KHZ:

			if (calibration_value > 32) {
				return STATUS_ERR_INVALID_ARG;
			}
			SYSCTRL->OSCULP32K.bit.CALIB = calibration_value;
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
 *  \param[in] clock_src Clock source to enable
 *
 *  \retval STATUS_OK Clock source was enabled successfully and is ready
 *  \retval STATUS_ERR_INVALID_ARG The clock source is not available on this device
 *  \retval STATUS_TIMEOUT The clock source did not start (timeout)
 */

enum status_code system_clock_source_enable(enum system_clock_source clock_src, bool block_until_ready)
{
	/* Default value is 0xFFFFFFFF for timeout*/
	uint32_t timeout;
	uint32_t waitmask;

	/* TODO: Check _bm naming; this is bit 1 for all ENABLE bits */
	switch (clock_src) {
		case SYSTEM_CLOCK_SOURCE_RC8MHZ:
			SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ENABLE;
			/* Not possible to wait for ready, so we return */
			return STATUS_OK;

		case SYSTEM_CLOCK_SOURCE_OSC32K:
			SYSCTRL->OSC32K.reg |= SYSCTRL_OSC32K_ENABLE;
			waitmask = SYSCTRL_PCLKSR_OSC32KRDY;
			break;

		case SYSTEM_CLOCK_SOURCE_XOSC:
			SYSCTRL->XOSC.reg |= SYSCTRL_XOSC_ENABLE;
			waitmask = SYSCTRL_PCLKSR_XOSCRDY;
			break;

		case SYSTEM_CLOCK_SOURCE_XOSC32K:
			SYSCTRL->XOSC32K.reg |= SYSCTRL_XOSC32K_ENABLE;
			waitmask = SYSCTRL_PCLKSR_XOSC32KRDY;
			break;

		case SYSTEM_CLOCK_SOURCE_DFLL:
			SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE;
			waitmask = SYSCTRL_PCLKSR_DFLLRDY;
			break;
		case SYSTEM_CLOCK_SOURCE_ULP32KHZ:
			/* Always enabled */
			return STATUS_OK;
		default:
			Assert(!"Invalid clock source supplied");
			return STATUS_ERR_INVALID_ARG;
	}

	if (block_until_ready == true) {
		/* Wait for the clock source to be ready or timeout */
		for (timeout = 0; timeout < CONF_CLOCK_TIMEOUT; timeout++) {
			if(SYSCTRL->PCLKSR.reg & waitmask) {
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
		case SYSTEM_CLOCK_SOURCE_RC8MHZ:
			SYSCTRL->OSC8M.reg &= ~SYSCTRL_OSC8M_ENABLE;
			break;
		case SYSTEM_CLOCK_SOURCE_OSC32K:
			SYSCTRL->OSC32K.reg &= ~SYSCTRL_OSC32K_ENABLE;
			break;
		case SYSTEM_CLOCK_SOURCE_XOSC:
			SYSCTRL->XOSC.reg &= ~SYSCTRL_XOSC_ENABLE;
			break;
		case SYSTEM_CLOCK_SOURCE_XOSC32K:
			SYSCTRL->XOSC32K.reg &= ~SYSCTRL_XOSC32K_ENABLE;
			break;
		case SYSTEM_CLOCK_SOURCE_DFLL:
			SYSCTRL->DFLLCTRL.reg &= ~SYSCTRL_DFLLCTRL_ENABLE;
			break;
		case SYSTEM_CLOCK_SOURCE_ULP32KHZ:
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
		case SYSTEM_CLOCK_SOURCE_RC8MHZ:
			/* TODO: verify that this cannot be disabled */
			return true;
		case SYSTEM_CLOCK_SOURCE_OSC32K:
			mask = SYSCTRL_PCLKSR_OSC32KRDY;
			break;
		case SYSTEM_CLOCK_SOURCE_XOSC:
			mask = SYSCTRL_PCLKSR_XOSCRDY;
			break;
		case SYSTEM_CLOCK_SOURCE_XOSC32K:
			mask = SYSCTRL_PCLKSR_XOSC32KRDY;
			break;
		case SYSTEM_CLOCK_SOURCE_DFLL:
			mask = SYSCTRL_PCLKSR_DFLLRDY;
			break;
		case SYSTEM_CLOCK_SOURCE_ULP32KHZ:
			/* Not possible to disable */
		default:
			return false;
		}

	if(SYSCTRL->PCLKSR.reg & mask) {
		return true;
	} else {
		return false;
	}
}
/* @} */

/**
 * \brief Initialize clock system based on the configuration in conf_clocks.h
 *
 * This function will apply the settings in conf_clock.h when run from the user
 * application. All clock sources and GCLKs are up and running when this function
 * returns.
 */
void system_clock_init(void)
{
	struct system_clock_source_config conf;
	struct system_gclk_gen_conf gclk_generator_conf;

	/* Initialize config struct to defaults */
	system_clock_source_get_default_config(&conf);

	/* XOSC */
	#if CONF_CLOCK_XOSC_ENABLE == true
	conf.ext.external_clock = CONF_CLOCK_XOSC_EXTERNAL_CRYSTAL;
	conf.ext.startup_time = CONF_CLOCK_XOSC_STARTUP_TIME;
	conf.ext.auto_gain_control = CONF_CLOCK_XOSC_AUTO_GAIN_CONTROL;

	system_clock_source_set_config(&conf, SYSTEM_CLOCK_SOURCE_XOSC);
	#endif /* CONF_CLOCK_XOSC_ENABLE */

	/* XOSC32K */
	#if CONF_CLOCK_XOSC32K_ENABLE == true
	conf.ext.external_clock = CONF_CLOCK_XOSC32K_EXTERNAL_CRYSTAL;
	conf.ext.startup_time = CONF_CLOCK_XOSC32K_STARTUP_TIME;
	conf.ext.auto_gain_control = CONF_CLOCK_XOSC32K_AUTO_AMPLITUDE_CONTROL;
	conf.osc32k.enable_1khz_output = CONF_CLOCK_XOSC32K_ENABLE_1KHZ_OUPUT;
	conf.osc32k.enable_32khz_output = CONF_CLOCK_XOSC32K_ENABLE_32KHZ_OUTPUT;

	system_clock_source_set_config(&conf, SYSTEM_CLOCK_SOURCE_XOSC32K);
	#endif /* CONF_CLOCK_XOSC32K_ENABLE */

	/* OSC32K */
	#if CONF_CLOCK_OSC32K_ENABLE == true
	conf.osc32k.enable_1khz_output = CONF_CLOCK_OSC32K_ENABLE_1KHZ_OUTPUT;
	conf.osc32k.enable_32khz_output = CONF_CLOCK_OSC32K_ENABLE_32KHZ_OUTPUT;

	system_clock_source_set_config(&conf, SYSTEM_CLOCK_SOURCE_OSC32K);
	#endif /* CONF_CLOCK_OSC32K_ENABLE */

	/* DFLL */
	#if CONF_CLOCK_DFLL_ENABLE == true
	conf.dfll.loop = CONF_CLOCK_DFLL_LOOP_MODE;

	#if (CONF_CLOCK_DFLL_MODE == SYSTEM_CLOCK_DFLL_OPEN_LOOP)
		// TODO: Add 48MHz calibration value here
	#endif

		#if CONF_CLOCK_DFLL_QUICK_LOCK == true
		conf.dfll.quick_lock = SYSTEM_CLOCK_DFLL_QUICK_LOCK_ENABLE;
		#else
		conf.dfll.quick_lock = SYSTEM_CLOCK_DFLL_QUICK_LOCK_DISABLE;
		#endif

		#if CONF_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK == true
		conf.dfll.stable_tracking = SYSTEM_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK;
		#else
		conf.dfll.stable_tracking = SYSTEM_CLOCK_DFLL_FIX_AFTER_FINE_LOCK;
		#endif

		#if CONF_CLOCK_DFLL_KEEP_LOCK_ON_WAKEUP == true
		conf.dfll.wakeup_lock = SYSTEM_CLOCK_DFLL_KEEP_LOCK_AFTER_WAKE;
		#else
		conf.dfll_wakeup_lock = SYSTEM_CLOCK_DFLL_LOSE_LOCK_AFTER_WAKE;
		#endif

		#if CONF_CLOCK_DFLL_ENABLE_CHILL_CYCLE == true
		conf.dfll.chill_cycle = SYSTEM_CLOCK_DFLL_CHILL_CYCLE_ENABLE;
		#else
		conf.dfll.chill_cycle = SYSTEM_CLOCK_DFLL_CHILL_CYCLE_DISABLE;
		#endif

	conf.dfll.coarse_max_step = CONF_CLOCK_DFLL_MAX_COARSE_STEP_SIZE;
	conf.dfll.fine_max_step = CONF_CLOCK_DFLL_MAX_FINE_STEP_SIZE;

	system_clock_source_set_config(&conf, SYSTEM_CLOCK_SOURCE_DFLL);
	#endif /* CONF_CLOCK_DFLL_ENABLE */

	conf.rc8mhz.prescaler = CONF_CLOCK_RC8MHZ_PRESCALER;
	system_clock_source_set_config(&conf, SYSTEM_CLOCK_SOURCE_RC8MHZ);


	#if CONF_CLOCK_CONFIGURE_GCLK == true


	#if CONF_CLOCK_GCLK_0_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_0_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_0_PRESCALER;
	system_gclk_gen_set_config(0, &gclk_generator_conf);
	system_gclk_gen_enable(0);
	#endif

	#if CONF_CLOCK_GCLK_1_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_1_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_1_PRESCALER;
	system_gclk_gen_set_config(1, &gclk_generator_conf);
	system_gclk_gen_enable(1);
	#endif

	#if CONF_CLOCK_GCLK_2_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_2_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_2_PRESCALER;
	system_gclk_gen_set_config(2, &gclk_generator_conf);
	system_gclk_gen_enable(2);
	#endif

	#if CONF_CLOCK_GCLK_3_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_3_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_3_PRESCALER;
	system_gclk_gen_set_config(3, &gclk_generator_conf);
	system_gclk_gen_enable(3);
	#endif

	#if CONF_CLOCK_GCLK_4_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_4_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_4_PRESCALER;
	system_gclk_gen_set_config(4, &gclk_generator_conf);
	system_gclk_gen_enable(4);
	#endif

	#if CONF_CLOCK_GCLK_5_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_5_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_5_PRESCALER;
	system_gclk_gen_set_config(5, &gclk_generator_conf);
	system_gclk_gen_enable(5);
	#endif

	#if CONF_CLOCK_GCLK_6_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_6_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_6_PRESCALER;
	system_gclk_gen_set_config(6, &gclk_generator_conf);
	system_gclk_gen_enable(6);
	#endif

	#if CONF_CLOCK_GCLK_7_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_7_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_7_PRESCALER;
	system_gclk_gen_set_config(7, &gclk_generator_conf);
	system_gclk_gen_enable(7);
	#endif

	#endif /* Configure GCLK */


	/* CPU and BUS clocks */
	system_main_clock_set_source(CONF_CLOCK_CPU_CLOCK_SOURCE);
	system_cpu_clock_set_divider(CONF_CLOCK_CPU_DIVIDER);
	#if CONF_CLOCK_ENABLE_CPU_CLOCK_FAILURE_DETECT == true
	system_main_clock_set_failure_detect(true);
	#else
	system_main_clock_set_failure_detect(false);
	#endif

	system_apb_clock_set_divider(SYSTEM_CLOCK_APB_APBA, CONF_CLOCK_APBA_DIVIDER);
	system_apb_clock_set_divider(SYSTEM_CLOCK_APB_APBB, CONF_CLOCK_APBB_DIVIDER);
}

