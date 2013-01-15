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
 * \internal Write an ADC configuration to the hardware module.
 *
 * This function will write out a given configuration to the hardware module.
 * Used by \ref adc_init.
 */
enum status_code _adc_set_config (Adc *const hw_dev,
		struct adc_conf *const config)
{
	uint8_t adjres;
	enum adc_average_samples average;

	/* Configure CTRLA */
	hw_dev->CTRLA.reg = (config->run_in_standby << ADC_CTRLA_RUNSTDBY_Pos);

	/* Configure REFCTRL */
	hw_dev->REFCTRL.reg =
			(config->reference_compensation_enable << ADC_REFCTRL_REFCOMP_Pos) |
			(config->reference);

	/* Configure AVGCTRL */
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

	/* Configure SAMPCTRL */
	hw_dev->SAMPCTRL.reg = (config->sample_length << ADC_SAMPCTRL_SAMPLEN_Pos);

	/* Configure CTRLB */
	_adc_wait_for_sync(hw_dev);
	hw_dev->CTRLB.reg =
			config->clock_prescaler |
			config->resolution |
			(config->correction_enable << ADC_CTRLB_CORREN_Pos) |
			(config->freerunning << ADC_CTRLB_FREERUN_Pos) |
			(config->left_adjust << ADC_CTRLB_LEFTADJ_Pos) |
			(config->differential_mode << ADC_CTRLB_DIFFMODE_Pos);

	/* Configure WINCTRL/WINLT/WINUT */
	_adc_wait_for_sync(hw_dev);
	hw_dev->WINCTRL.reg = config->window_mode;
	_adc_wait_for_sync(hw_dev);
	/* TODO: this will NOT work for negative values, please fix! */
	hw_dev->WINLT.reg   = config->window_lower_value << ADC_WINLT_WINLT_Pos;
	_adc_wait_for_sync(hw_dev);
	/* TODO: this will NOT work for negative values, please fix! */
	hw_dev->WINUT.reg   = config->window_upper_value << ADC_WINUT_WINUT_Pos;

	/* TODO: check size of inputs */
	/* Configure INPUTCTRL */
	_adc_wait_for_sync(hw_dev);
	hw_dev->INPUTCTRL.reg =
			config->gain_factor |
			(config->offset_start_scan << ADC_INPUTCTRL_INPUTOFFSET_Pos) |
			(config->inputs_to_scan << ADC_INPUTCTRL_INPUTSCAN_Pos) |
			config->negative_input |
			config->positive_input;

	/* Configure EVCTRL */
	hw_dev->EVCTRL.reg =
			(config->generate_event_on_window_monitor  << ADC_EVCTRL_WINMONEO_Pos) |
			(config->generate_event_on_conversion_done << ADC_EVCTRL_RESRDYEO_Pos) |
			(config->flush_adc_on_event                << ADC_EVCTRL_SYNCEI_Pos)   |
			(config->start_conversion_on_event         << ADC_EVCTRL_STARTEI_Pos);

	/* TODO: Necessary? */
	/* Disable all interrupts */
	hw_dev->INTENCLR.reg =
			(1 << ADC_INTENCLR_READY_Pos)   | (1 << ADC_INTENCLR_WINMON_Pos) |
			(1 << ADC_INTENCLR_OVERRUN_Pos) | (1 << ADC_INTENCLR_RESRDY_Pos);

	/* TODO: check size */
	/* Configure GAINCORR/OFFSETCORR */
	hw_dev->GAINCORR.reg   = config->gain_correction   << ADC_GAINCORR_GAINCORR_Pos;
	hw_dev->OFFSETCORR.reg = config->offset_correction << ADC_OFFSETCORR_OFFSETCORR_Pos;

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

	/* TODO: Set up GCLK_ADC using sysclock driver*/


	/* Write configuration to module */
	_adc_set_config(hw_dev, config);

	return STATUS_OK;
}
