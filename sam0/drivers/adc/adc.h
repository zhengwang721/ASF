/**
 * \file
 *
 * \brief SAMD20 Peripheral Analog-to-Digital Converter Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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

#ifndef _ADC_H_INCLUDED_
#define _ADC_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>
#include <system.h>

/**
 * \defgroup sam0_adc_group SAMD20 Analog-to-Digital Converter Driver (ADC)
 *
 * Driver for the SAMD20 architecture devices. This driver provides an
 * interface for converting analog signals to digital values.
 * This driver encompasses the following module within the SAMD20 devices:
 * \li \b ADC \b (Analog-to-Digital Converter)
 *
 * \section module_introduction Introduction
 *
 * \subsection module_overview ADC Overview
 * The analog-to-digital converter converts an analog signal to a digital value.
 * The ADC has up to 12-bit resolution, and is capable of converting up to 500k
 * samples per second (ksps).
 * The input selection is flexible, and both single-ended and differential
 * measurements can be done. For differential measurements, an optional gain
 * stage is available to increase the dynamic range. In addition, several
 * internal signal inputs are available. The ADC can provide both signed and
 * unsigned results.
 *
 * The ADC measurements can either be started by application software or an
 * incoming event from another peripheral in the device.
 * Both internal and external reference voltages can be used. An integrated
 * temperature sensor is available for use with the ADC. The bandgap voltage, as
 * well as the scaled IO and core voltages can also be measured by the ADC.
 *
 * The ADC has a compare function for accurate monitoring of user defined
 * thresholds with minimum software intervention required.
 * The ADC may be configured for 8-, 10- or 12-bit result, reducing the
 * conversion time from 2.0μs for 12-bit to 1.4μs for 8-bit result. ADC
 * conversion results are provided left or right adjusted which eases
 * calculation when the result is represented as a signed integer.
 * \n\n
 * A simplified block diagram of the ADC can be seen in the following figure:
 *
 * \dot
 * digraph overview {
 * splines = false;
 * rankdir=LR;
 *
 * adc0top [label="ADC0", shape=none];
 * dotstop [label="...", shape=none];
 * adcntop [label="ADCn", shape=none];
 * sigtop [label="Int. Sig", shape=none];
 *
 * adc0bot [label="ADC0", shape=none];
 * dotsbot [label="...", shape=none];
 * adcnbot [label="ADCn", shape=none];
 * sigbot [label="Int. Sig", shape=none];
 *
 *
 * mux1 [label="", shape=polygon, sides=4, distortion=0.6, orientation=90, style=filled, fillcolor=black, height=0.9, width=0.2];
 * inputctrl [label="INPUTCTRL", shape=box, style=filled, fillcolor=lightblue];
 * mux2 [label="", shape=polygon, sides=4, distortion=0.6, orientation=90, style=filled, fillcolor=black, height=0.9, width=0.2];
 *
 * adc0top -> mux1;
 * dotstop -> mux1;
 * adcntop -> mux1;
 * sigtop -> mux1;
 *
 * adc0bot -> mux2:nw;
 * dotsbot -> mux2:cw;
 * adcnbot -> mux2:cw;
 * sigbot -> mux2:sw;
 *
 * inputctrl -> mux1;
 * inputctrl:se -> mux2;
 * 
 * adc [label="ADC", shape=polygon, sides=5, orientation=90, distortion=-0.6, style=filled, fillcolor=darkolivegreen1, height=1, width=1];
 *
 * {rank=same; adc0top dotstop adcntop sigtop adc0bot dotsbot adcnbot sigbot }
 * {rank=same; mux1 inputctrl mux2 }
 * {rank=same; adc }
 *
 * }
 * \enddot
 *
 *
 * \subsection extra_info Extra Information
 * For extra information see \ref adc_extra_info.
 *
 * \section module_examples Examples
 * - \ref quickstart
 *
 * \section api_overview API Overview
 * @{
 */

/**
 * \brief ADC status flags.
 *
 * Enum for the possible status flags
 *
 */
enum adc_interrupt_flag {
	ADC_INTERRUPT_RESULT_READY = ADC_INTFLAG_RESRDY,
	ADC_INTERRUPT_WINDOW       = ADC_INTFLAG_WINMON,
	ADC_INTERRUPT_OVERRUN      = ADC_INTFLAG_MASK,
};

/**
 * \brief ADC reference voltage enum.
 *
 * Enum for the possible reference voltages for the ADC
 *
 */
enum adc_reference {
	/** 1.0V voltage reference */
	ADC_REFERENCE_INT1V   = ADC_REF_INT1V_bm,
	/** 1/1.48 VCC reference */
	ADC_REFERENCE_INTVCC0 = ADC_REF_INTVCC0_bm,
	/** 1/2 VCC (only for internal Vcc > 2.1v) */
	ADC_REFERENCE_INTVCC1 = ADC_REF_INTVCC1_bm,
	/** External reference */
	ADC_REFERENCE_AREFA   = ADC_REF_AREFA_bm,
	/** External reference */
	ADC_REFERENCE_AREFB   = ADC_REF_AREFB_bm,
};

/**
 * \brief ADC clock prescaler enum.
 *
 * Enum for the possible clock prescaler values for the ADC
 *
 */
enum adc_clock_prescaler {
	/** ADC clock division factor 4 */
	ADC_CLOCK_PRESCALER_DIV4   = ADC_PRESCALER_DIV4_bm,
	/** ADC clock division factor 8 */
	ADC_CLOCK_PRESCALER_DIV8   = ADC_PRESCALER_DIV8_bm,
	/** ADC clock division factor 16 */
	ADC_CLOCK_PRESCALER_DIV16  = ADC_PRESCALER_DIV16_bm,
	/** ADC clock division factor 32 */
	ADC_CLOCK_PRESCALER_DIV32  = ADC_PRESCALER_DIV32_bm,
	/** ADC clock division factor 64 */
	ADC_CLOCK_PRESCALER_DIV64  = ADC_PRESCALER_DIV64_bm,
	/** ADC clock division factor 128 */
	ADC_CLOCK_PRESCALER_DIV128 = ADC_PRESCALER_DIV128_bm,
	/** ADC clock division factor 256 */
	ADC_CLOCK_PRESCALER_DIV256 = ADC_PRESCALER_DIV256_bm,
	/** ADC clock division factor 512 */
	ADC_CLOCK_PRESCALER_DIV512 = ADC_PRESCALER_DIV512_bm,
};

/**
 * \brief ADC resolution enum.
 *
 * Enum for the possible resolution values for the ADC
 *
 */
enum adc_resolution {
	/** ADC 12-bit resolution */
	ADC_RESOLUTION_12BIT = ADC_RESOLUTION_12BIT_bm,
	/** TODO: I assume this will be renamed in the datasheet. Supposed to be "averaging mode output" */
	ADC_RESOLUTION_16BIT = ADC_RESOLUTION_16BIT_bm,
	/** ADC 10-bit resolution */
	ADC_RESOLUTION_10BIT = ADC_RESOLUTION_10BIT_bm,
	/** ADC 8-bit resolution */
	ADC_RESOLUTION_8BIT = ADC_RESOLUTION_8BIT_bm,
};

/**
 * \brief ADC window monitor mode enum.
 *
 * Enum for the possible window monitor modes for the ADC
 *
 */
enum adc_window_mode {
	/** No window mode */
	ADC_WINDOW_MODE_DISABLE          = ADC_WINMODE_DISABLE_bm,
	/** RESULT > WINLT */
	ADC_WINDOW_MODE_ABOVE_LOWER      = ADC_WINMODE1_bm,
	/** RESULT < WINUT */
	ADC_WINDOW_MODE_BELOW_UPPER      = ADC_WINMODE2_bm,
	/** WINLT < RESULT < WINUT */
	ADC_WINDOW_MODE_BETWEEN          = ADC_WINMODE3_bm,
	/** !( WINLT < RESULT < WINUT ) */
	ADC_WINDOW_MODE_BETWEEN_INVERTED = ADC_WINMODE4_bm,
};

/**
 * \brief ADC gain factor selection enum.
 *
 * Enum for the possible gain factor values for the ADC
 *
 */
enum adc_gain_factor {
	/** 1x gain */
	ADC_GAIN_FACTOR_1X   = ADC_GAIN_1X_bm,
	/** 2x gain */
	ADC_GAIN_FACTOR_2X   = ADC_GAIN_2X_bm,
	/** 4x gain */
	ADC_GAIN_FACTOR_4X   = ADC_GAIN_4X_bm,
	/** 8x gain */
	ADC_GAIN_FACTOR_8X   = ADC_GAIN_8X_bm,
	/** 16x gain */
	ADC_GAIN_FACTOR_16X  = ADC_GAIN_16X_bm,
	/** 1/2x gain */
	ADC_GAIN_FACTOR_DIV2 = ADC_GAIN_DIV2_bm,
};

/**
 * \brief ADC positive MUX input selection enum.
 *
 * Enum for the possible positive MUC input selections for the ADC
 *
 */
enum adc_positive_input {
	/** ADC0 pin */
	ADC_POSITIVE_INPUT_PIN0          = ADC_POSPIN0_bm,
	/** ADC1 pin */
	ADC_POSITIVE_INPUT_PIN1          = ADC_POSPIN1_bm,
	/** ADC2 pin */
	ADC_POSITIVE_INPUT_PIN2          = ADC_POSPIN2_bm,
	/** ADC3 pin */
	ADC_POSITIVE_INPUT_PIN3          = ADC_POSPIN3_bm,
	/** ADC4 pin */
	ADC_POSITIVE_INPUT_PIN4          = ADC_POSPIN4_bm,
	/** ADC5 pin */
	ADC_POSITIVE_INPUT_PIN5          = ADC_POSPIN5_bm,
	/** ADC6 pin */
	ADC_POSITIVE_INPUT_PIN6          = ADC_POSPIN6_bm,
	/** ADC7 pin */
	ADC_POSITIVE_INPUT_PIN7          = ADC_POSPIN7_bm,
	/** ADC8 pin */
	ADC_POSITIVE_INPUT_PIN8          = ADC_POSPIN8_bm,
	/** ADC9 pin */
	ADC_POSITIVE_INPUT_PIN9          = ADC_POSPIN9_bm,
	/** ADC10 pin */
	ADC_POSITIVE_INPUT_PIN10         = ADC_POSPIN10_bm,
	/** ADC11 pin */
	ADC_POSITIVE_INPUT_PIN11         = ADC_POSPIN11_bm,
	/** ADC12 pin */
	ADC_POSITIVE_INPUT_PIN12         = ADC_POSPIN12_bm,
	/** ADC13 pin */
	ADC_POSITIVE_INPUT_PIN13         = ADC_POSPIN13_bm,
	/** ADC14 pin */
	ADC_POSITIVE_INPUT_PIN14         = ADC_POSPIN14_bm,
	/** ADC15 pin */
	ADC_POSITIVE_INPUT_PIN15         = ADC_POSPIN15_bm,
	/** ADC16 pin */
	ADC_POSITIVE_INPUT_PIN16         = ADC_POSPIN16_bm,
	/** ADC17 pin */
	ADC_POSITIVE_INPUT_PIN17         = ADC_POSPIN17_bm,
	/** ADC18 pin */
	ADC_POSITIVE_INPUT_PIN18         = ADC_POSPIN18_bm,
	/** ADC19 pin */
	ADC_POSITIVE_INPUT_PIN19         = ADC_POSPIN19_bm,
	/** ADC20 pin */
	ADC_POSITIVE_INPUT_PIN20         = ADC_POSPIN20_bm,
	/** ADC21 pin */
	ADC_POSITIVE_INPUT_PIN21         = ADC_POSPIN21_bm,
	/** ADC22 pin */
	ADC_POSITIVE_INPUT_PIN22         = ADC_POSPIN22_bm,
	/** ADC23 pin */
	ADC_POSITIVE_INPUT_PIN23         = ADC_POSPIN23_bm,
	/** Temperature reference */
	ADC_POSITIVE_INPUT_TEMP          = ADC_POSPIN_TEMP_bm,
	/** Bandgap voltage */
	ADC_POSITIVE_INPUT_BANDGAP       = ADC_POSPIN_BANDGAP_bm,
	/** 1/4 scaled core supply */
	ADC_POSITIVE_INPUT_SCALEDCOREVCC = ADC_POSPIN_SCALEDCOREVCC_bm,
	/** 1/4 scaled I/O supply */
	ADC_POSITIVE_INPUT_SCALEDIOVCC   = ADC_POSPIN_SCALEDIOVCC_bm,
	/** AUX input */
	ADC_POSITIVE_INPUT_AUX           = ADC_POSPIN_AUX_bm,
};

/**
 * \brief ADC negative MUX input selection enum.
 *
 * Enum for the possible negative MUC input selections for the ADC
 *
 */
enum adc_negative_input {
	/** ADC0 pin */
	ADC_NEGATIVE_INPUT_PIN0          = ADC_NEGPIN0_bm,
	/** ADC1 pin */
	ADC_NEGATIVE_INPUT_PIN1          = ADC_NEGPIN1_bm,
	/** ADC2 pin */
	ADC_NEGATIVE_INPUT_PIN2          = ADC_NEGPIN2_bm,
	/** ADC3 pin */
	ADC_NEGATIVE_INPUT_PIN3          = ADC_NEGPIN3_bm,
	/** ADC4 pin */
	ADC_NEGATIVE_INPUT_PIN4          = ADC_NEGPIN4_bm,
	/** ADC5 pin */
	ADC_NEGATIVE_INPUT_PIN5          = ADC_NEGPIN5_bm,
	/** ADC6 pin */
	ADC_NEGATIVE_INPUT_PIN6          = ADC_NEGPIN6_bm,
	/** ADC7 pin */
	ADC_NEGATIVE_INPUT_PIN7          = ADC_NEGPIN7_bm,
	/** ADC8 pin */
	ADC_NEGATIVE_INPUT_PIN8          = ADC_NEGPIN8_bm,
	/** ADC9 pin */
	ADC_NEGATIVE_INPUT_PIN9          = ADC_NEGPIN9_bm,
	/** ADC10 pin */
	ADC_NEGATIVE_INPUT_PIN10         = ADC_NEGPIN10_bm,
	/** ADC11 pin */
	ADC_NEGATIVE_INPUT_PIN11         = ADC_NEGPIN11_bm,
	/** ADC12 pin */
	ADC_NEGATIVE_INPUT_PIN12         = ADC_NEGPIN12_bm,
	/** ADC13 pin */
	ADC_NEGATIVE_INPUT_PIN13         = ADC_NEGPIN13_bm,
	/** ADC14 pin */
	ADC_NEGATIVE_INPUT_PIN14         = ADC_NEGPIN14_bm,
	/** ADC15 pin */
	ADC_NEGATIVE_INPUT_PIN15         = ADC_NEGPIN15_bm,
	/** ADC16 pin */
	ADC_NEGATIVE_INPUT_PIN16         = ADC_NEGPIN16_bm,
	/** ADC17 pin */
	ADC_NEGATIVE_INPUT_PIN17         = ADC_NEGPIN17_bm,
	/** ADC18 pin */
	ADC_NEGATIVE_INPUT_PIN18         = ADC_NEGPIN18_bm,
	/** ADC19 pin */
	ADC_NEGATIVE_INPUT_PIN19         = ADC_NEGPIN19_bm,
	/** ADC20 pin */
	ADC_NEGATIVE_INPUT_PIN20         = ADC_NEGPIN20_bm,
	/** ADC21 pin */
	ADC_NEGATIVE_INPUT_PIN21         = ADC_NEGPIN21_bm,
	/** ADC22 pin */
	ADC_NEGATIVE_INPUT_PIN22         = ADC_NEGPIN22_bm,
	/** ADC23 pin */
	ADC_NEGATIVE_INPUT_PIN23         = ADC_NEGPIN23_bm,
	/** Internal ground */
	ADC_NEGATIVE_INPUT_GND           = ADC_NEGPIN_GND_bm,
	/** I/O ground */
	ADC_NEGATIVE_INPUT_IOGND         = ADC_NEGPIN_IOGND_bm,
};

/**
 * \brief ADC averaging selection enum.
 *
 * Enum for the possible numbers of ADC samples to average
 *
 */
enum adc_average_samples {
	/** No averaging */
	ADC_AVERAGE_DISABLE      = ADC_SAMPLENUM1_bm,
	/** Average 2 samples */
	ADC_AVERAGE_SAMPLES_2    = ADC_SAMPLENUM2_bm,
	/** Average 4 samples */
	ADC_AVERAGE_SAMPLES_4    = ADC_SAMPLENUM4_bm,
	/** Average 8 samples */
	ADC_AVERAGE_SAMPLES_8    = ADC_SAMPLENUM8_bm,
	/** Average 16 samples */
	ADC_AVERAGE_SAMPLES_16   = ADC_SAMPLENUM16_bm,
	/** Average 32 samples */
	ADC_AVERAGE_SAMPLES_32   = ADC_SAMPLENUM32_bm,
	/** Average 64 samples */
	ADC_AVERAGE_SAMPLES_64   = ADC_SAMPLENUM64_bm,
	/** Average 128 samples */
	ADC_AVERAGE_SAMPLES_128  = ADC_SAMPLENUM128_bm,
	/** Average 265 samples */
	ADC_AVERAGE_SAMPLES_265  = ADC_SAMPLENUM265_bm,
	/** Average 512 samples */
	ADC_AVERAGE_SAMPLES_512  = ADC_SAMPLENUM512_bm,
	/** Average 1024 samples */
	ADC_AVERAGE_SAMPLES_1024 = ADC_SAMPLENUM1024_bm,
};

/**
 * \brief ADC oversampling and decimation enum.
 *
 * Enum for the possible numbers of bits you can increase resolution of
 * by using oversampling and decimation mode.
 *
 */
enum adc_oversampling_and_decimation {
	/** Don't use oversampling and decimation mode */
	ADC_OVERSAMPLING_AND_DECIMATION_DISABLE = 0,
	/** 1 bit resolution increase */
	ADC_OVERSAMPLING_AND_DECIMATION_1BIT,
	/** 2 bits resolution increase */
	ADC_OVERSAMPLING_AND_DECIMATION_2BIT,
	/** 3 bits resolution increase */
	ADC_OVERSAMPLING_AND_DECIMATION_3BIT,
	/** 4 bits resolution increase */
	ADC_OVERSAMPLING_AND_DECIMATION_4BIT
};

/**
 * \brief ADC configuration structure.
 *
 * Configuration structure for an ADC instance. This structure should be
 * initialized by the \ref adc_get_config_defaults()
 * function before being modified by the user application.
 */
struct adc_config {
	/** TODO */
	enum gclock_source       clock_source;
	/** Voltage reference */
	enum adc_reference       reference;
	/** Clock prescaler */
	enum adc_clock_prescaler clock_prescaler;
	/** Result resolution */
	enum adc_resolution      resolution;
	/** Selected window mode */
	enum adc_window_mode     window_mode;
	/** Lower window value */
	int32_t                  window_lower_value;
	/** Upper window value */
	int32_t                  window_upper_value;
	/** Gain factor */
	enum adc_gain_factor     gain_factor;
	/** Positive MUX input */
	enum adc_positive_input  positive_input;
	/** Negative MUX input */
	enum adc_negative_input  negative_input;
	/** Average sampling mode */
	enum adc_average_samples average_samples;
	/** Oversampling and decimation mode */
	enum adc_oversampling_and_decimation oversampling_and_decimation;
	/** Left adjusted result */
	bool left_adjust;
	/** Differential mode */
	bool differential_mode;
	/** Free running mode */
	bool freerunning;
	/** Enable event input to trigger conversion start */
	bool start_conversion_on_event;
	/** Enable event input to trigger flush ADC module */
	bool flush_adc_on_event;
	/** Enable event generation on conversion done */
	bool generate_event_on_conversion_done;
	/** Enable event generation on window monitor */
	bool generate_event_on_window_monitor;
	/** ADC enabled in standby */
	bool run_in_standby;
	/**
	Reference buffer offset compensation enable.
	Set this to true to enable the reference buffer compensation to
	increase the accuracy of the gain stage. But decrease the input
	impedance; therefore increase the startup time of the reference.
	*/
	bool reference_compensation_enable;
	/**
	Correct for gain and offset based on values of gain_correction and
	offset_correction.
	*/
	bool correction_enable;
	/**
	If correction_enable is set to true, this value define how the ADC
	conversion result is compensated for gain error before written to
	the result register. This is a fractional value, 1-bit integer +
	11-bits fraction, therefore 1/2 <= gain_correction < 2.
	gain_correction values ranges from 0.10000000000 to 1.11111111111.
	*/
	uint16_t gain_correction;
	/**
	If correction_enable is set to true, these bits define how the ADC
	conversion result is compensated for offset error before written to
	the result register. This value is in two’s complement format.
	*/
	uint16_t offset_correction;
	/**
	This value (0-63) control the ADC sampling time in number of half ADC
	prescaled clock cycles (depends of ADC_PRESCALER value), thus
	controlling the ADC input impedance.
	Sampling time is set according to the formula:
	Sample time = (sample_length+1) * (ADCclk / 2)
	*/
	uint8_t sample_length;
	/**
	Offset (relative to selected (positive) input) of first input pin to be
	used in pin scan mode.
	*/
	uint8_t offset_start_scan;
	/**
	Number of input pins to scan in pin scan mode.
	Set to 0 do disable pin scan mode.
	*/
	uint8_t inputs_to_scan;
};

/**
 * \brief ADC device instance structure.
 *
 * ADC software instance structure.
 *
 */
struct adc_dev_inst {
	/** ADC hardware module */
	Adc *hw_dev;
};

/**
 * \brief Wait for synchronization to finish
 *
 * Blocks in a busy wait while module synchronizes.
 *
 * \param[in] hw_dev Pointer to hardware module
 */
static inline void _adc_wait_for_sync(Adc *const hw_dev)
{
	while (hw_dev->STATUS.reg & ADC_STATUS_SYNCBUSY) {
	}
}

enum status_code adc_init(struct adc_dev_inst *const dev_inst, Adc *hw_dev,
		struct adc_config *config);

/**
 * \brief Enable the ADC module
 *
 * This function will enables the ADC module.
 *
 * \param dev_inst[in]    Pointer to the device struct
 */
static inline void adc_enable(struct adc_dev_inst *const dev_inst)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	_adc_wait_for_sync(adc_module);
	adc_module->CTRLA.reg |= ADC_CTRLA_ENABLE;
}

/**
 * \brief Disable the ADC module
 *
 * This function will disable the ADC module.
 *
 * \param dev_inst[in]    pointer to the device struct
 */
static inline void adc_disable(struct adc_dev_inst *const dev_inst)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	_adc_wait_for_sync(adc_module);
	adc_module->CTRLA.reg &= ~ADC_CTRLA_ENABLE;
}

/**
 * \brief Reset the ADC module
 *
 * This function will reset the ADC module.
 *
 * \param[in] dev_inst Pointer to the ADC software instance struct
 *
 * \todo flush pipeline?
 */
static inline void adc_reset(struct adc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	/* Always disable before reset */
	adc_disable(dev_inst);

	/* Software reset the module */
	_adc_wait_for_sync(adc_module);
	adc_module->CTRLA.reg |= ADC_CTRLA_SWRST;
}

/**
 * \brief Initialize an ADC configuration structure to defaults
 *
 *  This function will initialize a given ADC configuration struct to a set of
 *  known default values. This function should be called on any new
 *  instance of the configuration structs before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li 1V from internal bandgap reference
 *   \li Div 4 clock prescaler
 *   \li 12 bit resolution
 *   \li Window monitor disabled
 *   \li No gain
 *   \li Averaging disabled
 *   \li Oversampling disabled
 *   \li Right adjust data
 *   \li Single-ended mode
 *   \li Free running disabled
 *   \li All events (input and generation) disabled
 *   \li Sleep operation disabled
 *   \li No reference compensation
 *   \li No gain/offset correction
 *   \li No added sampling time
 *   \li Pin scan mode disabled
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void adc_get_config_defaults(struct adc_config *const config)
{
	Assert(config);

	config->reference = ADC_REFERENCE_INT1V;
	config->clock_prescaler = ADC_CLOCK_PRESCALER_DIV4;
	config->resolution = ADC_RESOLUTION_12BIT;
	config->window_mode = ADC_WINDOW_MODE_DISABLE;
	config->gain_factor = ADC_GAIN_FACTOR_1X;
	config->average_samples = ADC_AVERAGE_DISABLE;
	config->oversampling_and_decimation =
			ADC_OVERSAMPLING_AND_DECIMATION_DISABLE;
	config->window_lower_value = 0;
	config->window_upper_value = 0;
	config->left_adjust = false;
	config->differential_mode = false;
	config->freerunning = false;
	config->start_conversion_on_event = false;
	config->flush_adc_on_event = false;
	config->generate_event_on_conversion_done = false;
	config->generate_event_on_window_monitor = false;
	config->run_in_standby = false;
	config->reference_compensation_enable = false;
	config->correction_enable = false;
	config->gain_correction = 0;
	config->offset_correction = 0;
	config->sample_length = 0;
	config->offset_start_scan = 0;
	config->inputs_to_scan = 0;
}

/**
 * \brief Flush the ADC pipeline
 *
 * This function will flush the pipeline and restart the ADC clock on the next 
 * peripheral clock edge.
 * All conversions in progress will be lost.
 * When flush is complete, the module will resume where it left off.
 *
 * \param dev_inst[in]    Pointer to the device struct
 */
static inline void adc_flush(struct adc_dev_inst *const dev_inst)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	_adc_wait_for_sync(adc_module);
	adc_module->SWTRIG.reg |= ADC_SWTRIG_FLUSH;
}

/**
 * \brief Start ADC conversion
 *
 * Starts an ADC conversion
 *
 * \param dev_inst      pointer to the device struct
 */
static inline void adc_start_conversion(struct adc_dev_inst *const dev_inst)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	_adc_wait_for_sync(adc_module);
	adc_module->SWTRIG.reg |= ADC_SWTRIG_START;
}

/**
 * \brief Read ADC result
 *
 * Read result from ADC conversion. Clears INTFLAG register after reading.
 *
 * \param dev_inst       pointer to device struct
 * \param result         pointer to store result
 */
static inline enum status_code adc_get_result(
		struct adc_dev_inst *const dev_inst, uint16_t *result)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(result);

	if (!adc_is_interrupt_flag_set(dev_inst, ADC_INTERRUPT_RESULT_READY)) {
		/* Result not ready. Abort. */
		return STATUS_ERR_BUSY;
	}

	Adc *const adc_module = dev_inst->hw_dev;

	_adc_wait_for_sync(adc_module);
	/* Get ADC result */
	*result = adc_module->RESULT.reg;
	/* Reset ready flag */
	adc_clear_interrupt_flag(dev_inst, ADC_INTERRUPT_RESULT_READY);

	return STATUS_OK;
}

/**
 * \brief Change ADC window mode
 *
 * This function will set the ADC window mode.
 *
 * \param dev_inst[in]           Pointer to the device struct
 * \param adc_window_mode[in]    Window monitor mode to set
 * \param window_lower_value[in] Lower window monitor threshold value
 * \param window_upper_value[in] Upper window monitor threshold value
  */
static inline void adc_set_window_mode(struct adc_dev_inst *const dev_inst,
		enum adc_window_mode window_mode,
		int16_t              window_lower_value,
		int16_t              window_upper_value)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	/* Wait for synchronization to finish */
	_adc_wait_for_sync(adc_module);
	/* Set window mode */
	adc_module->WINCTRL.reg = window_mode << ADC_WINCTRL_WINMODE_Pos;
	
	/* Wait for synchronization to finish */
	_adc_wait_for_sync(adc_module);
	/* Set lower window monitor threshold value */
	adc_module->WINLT.reg = window_lower_value << ADC_WINLT_WINLT_Pos;
	
	/* Wait for synchronization to finish */
	_adc_wait_for_sync(adc_module);
	/* Set upper window monitor threshold value */
	adc_module->WINUT.reg = window_upper_value << ADC_WINUT_WINUT_Pos;
}


/**
 * \brief Set ADC gain factor
 *
 * This function will set the ADC gain factor.
 *
 * \param dev_inst[in]       Pointer to the device struct
 * \param gain_factor[in]    Gain factor value to set
 */
static inline void adc_set_gain(struct adc_dev_inst *const dev_inst,
		enum adc_gain_factor gain_factor)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	/* Wait for synchronization to finish */
	_adc_wait_for_sync(adc_module);

	/* Set gain factor */
	adc_module->INPUTCTRL.reg =
			(adc_module->INPUTCTRL.reg & ~ADC_INPUTCTRL_GAIN_Msk) |
			(gain_factor << ADC_INPUTCTRL_GAIN_Pos);
}


/**
 * \brief Check if a given interrupt flag is set
 *
 * This function will check if a given interrupt flag is set.
 *
 * \param dev_inst[in]         Pointer to the device struct
 * \param interrupt_flag[in]   Interrupt flag to check
 *
 * \return
 * \retval true   The flag is set
 * \retval false  The flag is not set
 */
static inline bool adc_is_interrupt_flag_set(struct adc_dev_inst *const dev_inst,
		enum adc_status_flag interrupt_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;
	return adc_module->INTFLAG.reg & interrupt_flag;
}


/**
 * \brief Clear a given interrupt flag
 * 
 * This function will clear a given interrupt flag.
 *
 * \param dev_inst[in]          Pointer to the device struct
 * \param interrupt_flag[in]    Interrupt flag to clear
 */
static inline void adc_clear_interrupt_flag(struct adc_dev_inst *const dev_inst,
		enum adc_status_flag interrupt_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;
	
	/* Clear interrupt flag */
	adc_module->INTFLAG.reg = interrupt_flag;
}


/**
 * \brief Set up pin scan mode
 *
 * In pin scan mode, the first conversion will start at the configured
 * positive input + start_offset. When conversion is done, it will skip to
 * next input until inputs_to_scan conversions are made.
 *
 * \param dev_inst[in]           Pointer to the device struct
 * \param inputs_to_scan[in]     Number of input pins to do ADC on
 * \param start_offset[in]       Offset of first pin to scan (relative to
 *                               configured positive input)
 */
static inline void adc_set_pin_scan_mode(struct adc_dev_inst *const dev_inst,
		uint8_t inputs_to_scan, uint8_t start_offset)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	/* Wait for synchronization to finish */
	_adc_wait_for_sync(adc_module);

	adc_module->INPUTCTRL.reg =
			(adc_module->INPUTCTRL.reg &
			 ~(ADC_INPUTCTRL_INPUTSCAN_Msk | ADC_INPUTCTRL_INPUTOFFSET_Msk)) |
			(start_offset   << ADC_INPUTCTRL_INPUTOFFSET_Pos) |
			(inputs_to_scan << ADC_INPUTCTRL_INPUTSCAN_Pos);
}


/**
 * \brief Disable pin scan mode
 *
  * \param dev_inst[in]     Pointer to the device struct
 */
static inline void adc_disable_pin_scan_mode(struct adc_dev_inst *const dev_inst)
{
	/* Disable pin scan mode */
	adc_set_scan_mode(dev_inst, 0, 0);
}


/**
 * \brief Set positive ADC input pin
 *
 * \param dev_inst[in]        Pointer to the device struct
 * \param positive_input[in]  Positive input pin
 */
static inline void adc_set_positive_input(struct adc_dev_inst *const dev_inst,
		enum adc_positive_input positive_input)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	/* Wait for synchronization to finish */
	_adc_wait_for_sync(adc_module);

	/* Set positive input pin */
	adc_module->INPUTCTRL.reg =
			(adc_module->INPUTCTRL.reg & ~ADC_INPUTCTRL_MUXPOS_Msk) |
			(positive_input << ADC_INPUTCTRL_MUXPOS_Pos);
}


/**
 * \brief Set negative ADC input pin for differential mode
 *
 * \param dev_inst[in]        Pointer to the device struct
 * \param negative_input[in]  Negative input pin
 * \param negative_input[in]  Negative input pin
 */
static inline void adc_set_negative_input(struct adc_dev_inst *const dev_inst,
		enum adc_negative_input negative_input)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	/* Wait for synchronization to finish */
	_adc_wait_for_sync(adc_module);

	/* Set negative input pin */
	adc_module->INPUTCTRL.reg =
			(adc_module->INPUTCTRL.reg & ~ADC_INPUTCTRL_MUXNEG_Msk) |
			(negative_input << ADC_INPUTCTRL_MUXNEG_Pos);
}

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page dac_extra_info Extra Information
 *
 * \section acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>LSB</td>
 *		<td>Least Significant Bit</td>
 *	</tr>
 *	<tr>
 *		<td>MSB</td>
 *		<td>Most Significant Bit</td>
 *	</tr>
 * </table>
 *
 * \section workarounds Workarounds Implemented by Driver
 * No workarounds in driver.
 *
 * \section module_history Module History
 * Below is an overview of the module history, detailing enhancements and fixes
 * made to the module since its first release. The current version of this
 * corresponds to the newest version listed in the table below.
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
 * \page quickstart Quick Start Guides for the ADC Module
 *
 * This is the quick start guide list for the \ref sam0_adc_group module, with
 * step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function of the
 * user application and run at system startup, while the steps for usage can be
 * copied into the normal user application program flow.
 *
 * \see General list of module \ref module_examples "examples".
 *
 * \section adc_use_cases ADC driver use cases
 * - \subpage adc_basic_use_case
 */

#endif /* _ADC_H_INCLUDED_ */
