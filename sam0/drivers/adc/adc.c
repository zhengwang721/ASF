#include "adc.h"
#include <status_codes.h>


status_code_t _adc_set_config (ADC_t *const hw_dev,
		struct adc_conf *const config)
{
	/* Configure CTRLA */
	hw_dev->CTRLA = (config->sleep_enable << ADC_SLEEPEN_bp);

	/* Configure REFCTRL */
	hw_dev->REFCTRL =
			(config->reference_compenstation_enable << ADC_REFCOMP_bp) |
			(config->adc_reference                  << ADC_REFSEL_bp);

	/* Configure AVGCTRL */
	switch (config->oversampling_and_decimation) {

	case ADC_OVERSAMPLING_AND_DECIMATION_DISABLE:
		adjres = 0;
		average = config->average_samples;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_1BIT:
		adjres = 0x01;
		average = ADC_AVERAGE_SAMPLES_4;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_2BIT:
		adjres = 0x02;
		average = ADC_AVERAGE_SAMPLES_16;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_3BIT:
		adjres = 0x01;
		average = ADC_AVERAGE_SAMPLES_64;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_4BIT:
		adjres = 0x00;
		average = ADC_AVERAGE_SAMPLES_256;
		break;

	default:
		/* Unknown. Abort. */
		return STATUS_ERR_INVALID_ARG;
	}

	/* Configure SAMPCTRL */
	hw_dev->SAMPCTRL = (config->sample_length << ADC_SAMPLEN_bp);

	/* Configure CTRLB */
	_adc_wait_for_sync(hw_dev);
	hw_dev->CTRLB =
			(config->clock_prescaler   << ADC_PRESCALER_bp) |
			(config->resolution        << ADC_RESSEL_bp)    |
			(config->correction_enable << ADC_CORREN_bp)    |
			(config->freerunning       << ADC_FREERUN_bp)   |
			(config->left_adjust       << ADC_LEFTADJ_bp)   |
			(config->differential_mode << ADC_DIFFMODE_bp);

	/* Configure WINCTRL/WINLT/WINUT */
	_adc_wait_for_sync(hw_dev);
	hw_dev->WINCTRL = config->window_mode        << ADC_WINMODE_bp;
	_adc_wait_for_sync(hw_dev);
	/* this will NOT work for negative values, please fix! */
	hw_dev->WINLT   = config->window_lower_value << ADC_WINLT_bp;
	_adc_wait_for_sync(hw_dev);
	/* this will NOT work for negative values, please fix! */
	hw_dev->WINUT   = config->window_upper_value << ADC_WINUT_bp;

	/* Configure INPUTCTRL */
	_adc_wait_for_sync(hw_dev);
	hw_dev->INPUTCTRL =
			(config->gain_factor       << ADC_GAIN_bp)        |
			(config->offset_start_scan << ADC_INPUTOFFSET_bp) |
			(config->inputs_to_scan    << ADC_INPUTSCAN_bp)   |
			(config->negative_input    << ADC_MUXNEG_bp)      |
			(config->positive_input    << ADC_MUXPOS_bp);

	/* Configure EVCTRL */
	hw_dev->EVCTRL =
			(config->generate_event_on_window_monitor  << ADC_WINMONEO_bp) |
			(config->generate_event_on_conversion_done << ADC_RESRDYEO_bp) |
			(config->flush_adc_on_event                << ADC_SYNCEI_bp)   |
			(config->start_conversion_on_event         << ADC_STARTEI_bp);

	/* Disable all interrupts */
	hw_dev->INTENCLR =
			(1 << ADC_READY_bp)   | (1 << ADC_WINMON_bp) |
			(1 << ADC_OVERRUN_bp) | (1 << ADC_RESRDY_bp);

	/* Configure GAINCORR/OFFSETCORR */
	hw_dev->GAINCORR   = config->gain_correction   << ADC_GAINCORR_bp;
	hw_dev->OFFSETCORR = config->offset_correction << ADC_OFFSETCORR_bp;

	return STATUS_OK;
}


status_code_t adc_init(struct adc_dev_inst *const dev_inst, ADC_t *hw_dev,
		struct adc_config *config)
{
	uint8_t adjres;
	enum adc_average_samples average;

	dev_inst->hw_dev = hw_dev;

	if (hw_dev->CTRLA & ADC_SWRST_bm) {
		/* We are in the middle of a reset. Abort. */
		return STATUS_ERR_BUSY;
	}

	_adc_wait_for_sync(hw_dev);
	if (hw_dev->CTRLA & ADC_ENABLE_bm) {
		/* Module must be disabled before initialization. Abort. */
		return STATUS_ERR_DENIED;
	}

	/* TODO: Set up GCLK_ADC using sysclock driver*/


	/* Write configuration to module */
	_adc_set_config(hw_dev, config);

	return STATUS_OK;
}
