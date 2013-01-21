/**
 * \file
 *
 * \brief SAMD20 Peripheral Analog-to-Digital Converter Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#include "adc.h"

/**
 * \internal Writes an ADC configuration to the hardware module
 *
 * This function will write out a given configuration to the hardware module.
 * Used by \ref adc_init.
 *
 * \param[out] hw_dev Pointer to the ADC software instance struct 
 * \param[in] config  Pointer to configuration struct
 *
 * \return Status of the configuration procedure
 * \retval STATUS_OK                The configuration was successful
 * \retval STATUS_ERR_INVALID_ARG   Invalid argument(s) were provided
 */
static enum status_code _adc_set_config (Adc *const hw_dev,
		struct adc_conf *const config)
{
	uint8_t adjres;
	enum adc_average_samples average;
	struct system_gclk_ch_conf gclk_ch_conf;


	/* Configure GCLK channel and enable clock */
	gclk_ch_conf.source_generator = config->clock_source;

	#if defined (REVB)
	/* Set the GCLK channel to run in standby mode */
	gclk_ch_conf.run_in_standby = config->run_in_standby;
	#else
	/* Set the GCLK channel sleep enable mode */
	gclk_ch_conf.enable_during_sleep = config->run_in_standby;
	#endif

	/* Apply configuration and enable the GCLK channel */
	system_gclk_ch_set_config(ADC_GCLK_ID, &gclk_ch_conf);
	system_gclk_ch_enable(ADC_GCLK_ID);

	/* Configure run in standby */
	hw_dev->CTRLA.reg = (config->run_in_standby << ADC_CTRLA_RUNSTDBY_Pos);

	/* Configure reference */
	hw_dev->REFCTRL.reg =
			(config->reference_compensation_enable << ADC_REFCTRL_REFCOMP_Pos) |
			(config->reference);

	/* Set adjusting result and number of samples */
	switch (config->oversampling_and_decimation) {

	case ADC_OVERSAMPLING_AND_DECIMATION_DISABLE:
		adjres = 0x00;
		average = config->average_samples;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_1BIT:
		adjres = 0x01;
		average = ADC_AVGCTRL_SAMPLENUM_4;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_2BIT:
		adjres = 0x02;
		average = ADC_AVGCTRL_SAMPLENUM_16;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_3BIT:
		adjres = 0x01;
		average = ADC_AVGCTRL_SAMPLENUM_64;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_4BIT:
		adjres = 0x00;
		average = ADC_AVGCTRL_SAMPLENUM_256;
		break;

	default:
		/* Unknown. Abort. */
		return STATUS_ERR_INVALID_ARG;
	}
	hw_dev->AVGCTRL.reg = ADC_AVGCTRL_ADJRES(adjres) | average;

	/* Check validity of sample length value */
	if (config->sample_length > 63) {
		return STATUS_ERR_INVALID_ARG;
	} else {
		/* Configure sample length */
		hw_dev->SAMPCTRL.reg = (config->sample_length << ADC_SAMPCTRL_SAMPLEN_Pos);
	}

	/* Configure CTRLB */
	_adc_wait_for_sync(hw_dev);
	hw_dev->CTRLB.reg =
			config->clock_prescaler |
			config->resolution |
			(config->correction.correction_enable << ADC_CTRLB_CORREN_Pos) |
			(config->freerunning << ADC_CTRLB_FREERUN_Pos) |
			(config->left_adjust << ADC_CTRLB_LEFTADJ_Pos) |
			(config->differential_mode << ADC_CTRLB_DIFFMODE_Pos);

	/* Check validity of window thresholds */
	if (config->window.window_mode != ADC_WINDOW_MODE_DISABLE) {
		switch (config->resolution) {
		case ADC_RESOLUTION_8BIT:
			if (config->differential_mode &&
					(config->window.window_lower_value > 127 ||
					config->window.window_lower_value < -128 ||
					config->window.window_upper_value > 127 ||
					config->window.window_upper_value < -128)) {
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			} else if (config->window.window_lower_value > 255 ||
					config->window.window_upper_value > 255){
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			}
			break;
		case ADC_RESOLUTION_10BIT:
			if (config->differential_mode &&
					(config->window.window_lower_value > 511 ||
					config->window.window_lower_value < -512 ||
					config->window.window_upper_value > 511 ||
					config->window.window_upper_value > -512)) {
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			} else if (config->window.window_lower_value > 1023 ||
					config->window.window_upper_value > 1023){
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			}
			break;
		case ADC_RESOLUTION_12BIT:
			if (config->differential_mode &&
					(config->window.window_lower_value > 2047 ||
					config->window.window_lower_value < -2048 ||
					config->window.window_upper_value > 2047 ||
					config->window.window_upper_value < -2048)) {
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			} else if (config->window.window_lower_value > 4095 ||
					config->window.window_upper_value > 4095){
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			}
			break;
		case ADC_RESOLUTION_16BIT:
			if (config->differential_mode && (config->window.window_lower_value > 32767 ||
					config->window.window_lower_value < -32768 ||
					config->window.window_upper_value > 32767 ||
					config->window.window_upper_value < -32768)) {
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			} else if (config->window.window_lower_value > 65535 ||
					config->window.window_upper_value > 65535){
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			}
			break;
		}
	}
	/* Wait for synchronization */
	_adc_wait_for_sync(hw_dev);
	/* Configure window mode */
	hw_dev->WINCTRL.reg = config->window.window_mode;
	
	/* Wait for synchronization */
	_adc_wait_for_sync(hw_dev);
	/* Configure lower threshold */
	hw_dev->WINLT.reg = config->window.window_lower_value << ADC_WINLT_WINLT_Pos;
	
	/* Wait for synchronization */
	_adc_wait_for_sync(hw_dev);
	/* Configure lower threshold */
	hw_dev->WINUT.reg = config->window.window_upper_value << ADC_WINUT_WINUT_Pos;
	
	uint8_t inputs_to_scan = config->pin_scan.inputs_to_scan;
	if (inputs_to_scan > 0) {
		/*
		* Number of input sources included is the value written to INPUTSCAN
		* plus 1.
		*/
		inputs_to_scan--;
	}
	if (inputs_to_scan > ADC_INPUTCTRL_INPUTSCAN_Msk ||
			config->pin_scan.offset_start_scan > ADC_INPUTCTRL_INPUTOFFSET_Msk) {
		/* Invalid number of input pins or input offset */
		return STATUS_ERR_INVALID_ARG;
	}
	/* Wait for synchronization */
	_adc_wait_for_sync(hw_dev);
	/* Configure pin scan mode and positive and negative input pins */
	hw_dev->INPUTCTRL.reg =
			config->gain_factor |
			(config->pin_scan.offset_start_scan << ADC_INPUTCTRL_INPUTOFFSET_Pos) |
			(inputs_to_scan << ADC_INPUTCTRL_INPUTSCAN_Pos) |
			config->negative_input |
			config->positive_input;

	/* Configure events */
	hw_dev->EVCTRL.reg =
			config->event.event_action |
			(config->event.generate_event_on_window_monitor  << ADC_EVCTRL_WINMONEO_Pos) |
			(config->event.generate_event_on_conversion_done << ADC_EVCTRL_RESRDYEO_Pos);
			

	/* Disable all interrupts */
	hw_dev->INTENCLR.reg =
			(1 << ADC_INTENCLR_READY_Pos)   | (1 << ADC_INTENCLR_WINMON_Pos) |
			(1 << ADC_INTENCLR_OVERRUN_Pos) | (1 << ADC_INTENCLR_RESRDY_Pos);

	if (config->correction.correction_enable){
		/* Make sure gain_correction value is valid */
		if (config->correction.gain_correction > ADC_GAINCORR_GAINCORR_Msk) {
			return STATUS_ERR_INVALID_ARG;
		} else {
			/* Set gain correction value */
			hw_dev->GAINCORR.reg = config->correction.gain_correction <<
					ADC_GAINCORR_GAINCORR_Pos;
		}
		
		/* Make sure offset correction value is valid */
		if (config->correction.offset_correction > 2047 ||
				config->correction.offset_correction < -2048) {
			return STATUS_ERR_INVALID_ARG;
		} else {
			/* Set offset correction value */
			hw_dev->OFFSETCORR.reg = config->correction.offset_correction <<
					ADC_OFFSETCORR_OFFSETCORR_Pos;
		}
	}
	return STATUS_OK;
}

/**
 * \brief Initialize the ADC
 *
 * This function will initialize the ADC device struct and the hardware module
 * based on the values of the configuration struct.
 *
 * \param[out] dev_inst Pointer to the ADC software instance struct
 * \param[in] hw_dev    Pointer to the ADC module instance
 * \param[in] config    Pointer to the configuration struct
 *
 * \return Status of the initialization procedure
 * \retval STATUS_OK                The initialization was successful
 * \retval STATUS_ERR_INVALID_ARG   Invalid argument(s) were provided
 * \retval STATUS_ERR_BUSY          The module is busy with a reset operation
 * \retval STATUS_ERR_DENIED        The module is enabled
 */
enum status_code adc_init(struct adc_dev_inst *const dev_inst, Adc *hw_dev,
		struct adc_conf *config)
{

	dev_inst->hw_dev = hw_dev;

	if (hw_dev->CTRLA.reg & ADC_CTRLA_SWRST) {
		/* We are in the middle of a reset. Abort. */
		return STATUS_ERR_BUSY;
	}

	if (hw_dev->CTRLA.reg & ADC_CTRLA_ENABLE) {
		/* Module must be disabled before initialization. Abort. */
		return STATUS_ERR_DENIED;
	}

	/* Write configuration to module */
	return _adc_set_config(hw_dev, config);;
}
