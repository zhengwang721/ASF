/**
 * \file
 *
 * \brief SAMD20 Peripheral Analog-to-Digital Converter Driver
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
 * mux2 [label="", shape=polygon, sides=4, distortion=0.6, orientation=90, style=filled, fillcolor=black, height=0.9, width=0.2];
 *
 *
 * int1v [label="INT1V", shape=none];
 * intvcc [label="INTVCC", shape=none];
 * arefa [label="AEFA", shape=none];
 * arefb [label="AREFB", shape=none];
 *
 * mux3 [label="", shape=polygon, sides=4, distortion=0.6, orientation=90, style=filled, fillcolor=black, height=0.9, width=0.2];
 *
 * adc0top -> mux1;
 * dotstop -> mux1;
 * adcntop -> mux1;
 * sigtop -> mux1;
 *
 * adc0bot -> mux2:nw;
 * dotsbot -> mux2:w;
 * adcnbot -> mux2:w;
 * sigbot -> mux2:sw;
 *
 * int1v -> mux3;
 * intvcc -> mux3;
 * arefa -> mux3;
 * arefb -> mux3;
 *
 *
 * adc [label="ADC", shape=polygon, sides=5, orientation=90, distortion=-0.6, style=filled, fillcolor=darkolivegreen1, height=1, width=1];
 * prescaler [label="PRESCALER", shape=box];
 *
 * mux1 -> adc;
 * mux2 -> adc;
 * mux3 -> adc:sw;
 * prescaler -> adc;
 *
 * postproc [label="Post processing", shape=box];
 * result [label="RESULT", shape=box];
 *
 * adc:e -> postproc:w;
 * postproc:e -> result:w;
 *
 * {rank=same; adc0top dotstop adcntop sigtop adc0bot dotsbot adcnbot sigbot }
 * {rank=same; mux1 mux2 int1v intvcc arefa arefb}
 * {rank=same; prescaler adc}
 *
 * }
 * \enddot
 *
 * \dot
 * digraph overview {
 * splines = false;
 * rankdir=LR;
 *
 * mux1 [label="Positive input", shape=box];
 * mux2 [label="Negative input", shape=box];
 *
 *
 * mux3 [label="Reference", shape=box];
 *
 * adc [label="ADC", shape=polygon, sides=5, orientation=90, distortion=-0.6, style=filled, fillcolor=darkolivegreen1, height=1, width=1];
 * prescaler [label="PRESCALER", shape=box, style=filled, fillcolor=lightblue];
 *
 * mux1 -> adc;
 * mux2 -> adc;
 * mux3 -> adc:sw;
 * prescaler -> adc;
 *
 * postproc [label="Post processing", shape=box];
 * result [label="RESULT", shape=box, style=filled, fillcolor=lightblue];
 *
 * adc:e -> postproc:w;
 * postproc:e -> result:w;
 *
 * {rank=same; mux1 mux2}
 * {rank=same; prescaler adc}
 *
 * }
 * \enddot
 *
 * \subsection prescaler Prescaler
 * The ADC features a prescaler which enables conversion at lower clock rates
 * than the specified GCLK settings. The prescaler can be configured in the
 * \ref adc_conf struct.
 *
 * \subsection resolution ADC Resolution
 * The ADC supports 8-bit, 10-bit or 12-bit resolution. The resolution can be
 * configured in the \ref adc_conf struct.
 *
 * \subsubsection oversampling Oversampling and Decimation
 * Oversampling and decimation can be configured with the
 * \ref adc_oversampling_and_decimation in the \ref adc_conf struct.
 * In oversampling and decimation mode the ADC resolution is increased from
 * 12-bits to programmed 13, 14, 15 or 16-bits.
 *
 *
 * \subsection conversion Conversion
 * An ADC conversion can be started with the \ref adc_start_conversion
 * function. It is also possible configure the ADC in free-running mode where
 * new conversions are started as soon as a conversion is finished, or
 * configure a number of input pins to scan. See \ref pin_scan for more
 * information about the latter.
 *
 * The result of a conversion can be retrieved with the \ref adc_read
 * function.
 *
 * \subsubsection diff_mode Differential and Single-Ended Conversion
 * The ADC has two conversion modes; differential and single-ended. When
 * measuring signals where the positive input always is at a higher voltage
 * than the negative input, the single-ended conversion mode should be used in
 * order to have full 12-bit resolution in the conversion mode, which has only
 * positive values.
 * If however the positive input may go below the negative input, creating
 * some negative results, the differential mode should be used
 * in order to get correct results. The configuration of the conversion mode
 * is done in the \ref adc_conf struct.
 *
 * \subsubsection sample_time Sample Time
 * Sample time is configurable with \ref adc_conf.sample_length. This value
 * (0-63) controls the ADC sampling time in number of half-ADC prescaled clock
 * cycles (depending on the prescaler value), thus controlling the ADC input
 * impedance.
 *
 * The resulting sampling time is given by the following equation:
 * \f[
 * t_{SAMPLE} = (sample\_length+1) \times \frac{ADC_{CLK}} {2}
 * \f]
 *
 * \subsubsection averaging Averaging
 * The ADC can be configured to trade conversion speed for accuracy by
 * averaging multiple samples. This feature is suitable when operating in noisy
 * conditions. The numbers of samples to be averaged is specified with the
 * \ref adc_average_samples enum in the \ref adc_conf struct.
 * When reading the ADC result, this will be the output.
 * The effective ADC sample rate will be reduced with:
 * <table>
 *   <tr>
 *     <th>Number of Samples</th>
 *     <th>Final Result</th>
 *   </tr>
 *   <tr>
 *     <td>1</td>
 *     <td>12-bits</td>
 *  </tr>
 *  <tr>
 *     <td>2</td>
 *     <td>13-bits</td>
 *  </tr>
 *  <tr>
 *     <td>4</td>
 *     <td>14-bits</td>
 *  </tr>
 *  <tr>
 *     <td>8</td>
 *     <td>15-bits</td>
 *  </tr>
 *  <tr>
 *     <td>16</td>
 *     <td>16-bits</td>
 *  </tr>
 *  <tr>
 *     <td>32</td>
 *     <td>16-bits</td>
 *  </tr>
 *  <tr>
 *     <td>64</td>
 *     <td>16-bits</td>
 *  </tr>
 *  <tr>
 *     <td>128</td>
 *     <td>16-bits</td>
 *  </tr>
 *  <tr>
 *     <td>256</td>
 *     <td>16-bits</td>
 *  </tr>
 *  <tr>
 *     <td>512</td>
 *     <td>16-bits</td>
 *  </tr>
 *  <tr>
 *     <td>1024</td>
 *     <td>16-bits</td>
 *  </tr>
 * </table>
 *
 *
 * \subsubsection offset_corr Offset and Gain Correction
 * Inherent gain and offset errors affect the absolute accuracy of the ADC. The
 * offset error is defined as the deviation of the ADC’s actual transfer
 * function from ideal straight line at zero input voltage.
 * The gain error is defined as the deviation of the last output step's
 * midpoint from the ideal straight line, after compensating for offset error.
 * The offset correction value is subtracted from the converted data before the
 * result is ready. The gain correction value is multiplied with the offset
 * corrected value.
 *
 * The equation for both offset and gain error compensation is shown below:
 * \f[
 * ADC_{RESULT} = (VALUE_{CONV} + CORR_{OFFSET}) \times CORR_{GAIN}
 * \f]
 *
 * Offset and gain correction is enabled with the
 * \ref adc_conf.correction_enable bool, and the correction values are
 * written to the \ref adc_conf.gain_correction and
 * \ref adc_conf.offset_correction values.
 *
 * In single conversion, a latency of 13 GCLK_ADC is added for the final
 * RESULT availability. Since the correction time is always less than
 * propagation delay, in free running mode this latency appears only during the
 * first conversion. All other conversion results are available at the defined
 * sampling rate.

 * \subsubsection pin_scan Pin Scan
 * In pin scan mode, the first conversion will start at the configured positive
 * input (\ref adc_conf.positive_input) plus a start offset
 * (\ref adc_conf.offset_start_scan). When a conversion is done, the next
 * conversion will start at the next input and so on, until the configured
 * number of input pins to scan (\ref adc_conf.inputs_to_scan) conversions
 * are done.
 *
 * Pin scan mode can be configured in the \ref adc_conf struct before
 * initializing the ADC, or run-time with the \ref adc_set_pin_scan_mode and
 * \ref adc_disable_pin_scan_mode functions.
 *
 * \subsection window_monitor Window Monitor
 * The window monitor allows comparing the conversion result to predefined
 * threshold values.
 * The threshold values are evaluated differently, depending on whether
 * differential or single-ended mode is selected.
 * The upper and lower thresholds are evaluated as signed values in
 * differential mode, otherwise they are evaluated as unsigned values.
 *
 * The significant bits of the lower window monitor threshold and upper window
 * monitor threshold are given by the precision selected by the resolution
 * option in the \ref adc_conf struct. That means that only the eight lower
 * bits will be considered in 8-bit mode. In addition, if using differential
 * mode, the 8th bit will be considered as the sign bit even if bit 9 is a
 * zero.

 * The window mode can be configured with the configuration struct before
 * initializing the ADC, or run-time with the \ref adc_set_window_mode function.
 * The window monitor result can be polled with the
 * \ref adc_is_interrupt_flag_set function. If this flag is set, it must be
 * cleared with the \ref adc_clear_interrupt_flag function before checking for
 * a new match.
 *
 * \subsection events Events
 * Event generation and event actions are configurable in the ADC.
 *
 * The ADC has two actions that can be triggered upon event reception:
 * \li Start conversion
 * \li Flush pipeline and start conversion
 *
 * The ADC can generate two events:
 * \li Window monitor
 * \li Result ready
 *
 * If the event actions are enabled in the configuration, any incoming event
 * will trigger the action.
 * \n
 * If the window monitor event is enabled, an event will be generated
 * when the configured window condition is detected.
 * If the result ready event is enabled, an event will be generated when a
 * conversion is done.
 *
 * \subsection module_clk_sources Clock Sources
 * The clock for the ADC interface (CLK_ADC) is generated by
 * the Power Manager. This clock is turned on by default, and can be enabled
 * and disabled in the Power Manager.
 *\n\n
 * Additionally, an asynchronous clock source (GCLK_ADC) is required.
 * These clocks are normally disabled by default. The selected clock source
 * must be enabled in the Power Manager before it can be used by the ADC.
 * The ADC core operates asynchronously from the user interface and
 * peripheral bus. As a consequence, the ADC needs two clock cycles of both
 * CLK_ADC and GCLK_ADC to synchronize the values written to some of the
 * control and data registers.
 * The oscillator source for the GCLK_ADC clock is selected in the System
 * Control Interface (SCIF).
 *
 * \section dependencies Dependencies
 * The ADC driver has the following dependencies:
 * \li \ref system_group "System" - The ADC can only do conversions in active or
 * idle mode. The clock for the ADC interface (CLK_ADC) is generated by
 * the Power Manager. This clock is turned on by default, and can be enabled
 * and disabled in the Power Manager.
 * Additionally, an asynchronous clock source (GCLK_ADC) is required.
 * These clocks are normally disabled by default. The selected clock source
 * must be enabled in the Power Manager before it can be used by the ADC.
 * \li \ref sam0_events_group "Events" - Event generation and event actions are
 * configurable in the ADC.
 *
 * \section special_cons Special Considerations
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
 * \brief ADC interrupt flags
 *
 * Enum for the possible ADC interrupt flags
 *
 */
enum adc_interrupt_flag {
	/** ADC result ready */
	ADC_INTERRUPT_RESULT_READY = ADC_INTFLAG_RESRDY,
	/** Window monitor match */
	ADC_INTERRUPT_WINDOW       = ADC_INTFLAG_WINMON,
	/** ADC result overwritten before read */
	ADC_INTERRUPT_OVERRUN      = ADC_INTFLAG_MASK,
};

/**
 * \brief ADC reference voltage enum
 *
 * Enum for the possible reference voltages for the ADC.
 *
 */
enum adc_reference {
	/** 1.0V voltage reference */
	ADC_REFERENCE_INT1V   = ADC_REFCTRL_REFSEL_INT1V,
	/** 1/1.48 VCC reference */
	ADC_REFERENCE_INTVCC0 = ADC_REFCTRL_REFSEL_INTVCC0,
	/** 1/2 VCC (only for internal Vcc > 2.1v) */
	ADC_REFERENCE_INTVCC1 = ADC_REFCTRL_REFSEL_INTVCC1,
	/** External reference A */
	ADC_REFERENCE_AREFA   = ADC_REFCTRL_REFSEL_AREFA,
	/** External reference B */
	ADC_REFERENCE_AREFB   = ADC_REFCTRL_REFSEL_AREFB,
};

/**
 * \brief ADC clock prescaler enum
 *
 * Enum for the possible clock prescaler values for the ADC.
 *
 */
enum adc_clock_prescaler {
	/** ADC clock division factor 4 */
	ADC_CLOCK_PRESCALER_DIV4   = ADC_CTRLB_PRESCALER_DIV4,
	/** ADC clock division factor 8 */
	ADC_CLOCK_PRESCALER_DIV8   = ADC_CTRLB_PRESCALER_DIV8,
	/** ADC clock division factor 16 */
	ADC_CLOCK_PRESCALER_DIV16  = ADC_CTRLB_PRESCALER_DIV16,
	/** ADC clock division factor 32 */
	ADC_CLOCK_PRESCALER_DIV32  = ADC_CTRLB_PRESCALER_DIV32,
	/** ADC clock division factor 64 */
	ADC_CLOCK_PRESCALER_DIV64  = ADC_CTRLB_PRESCALER_DIV64,
	/** ADC clock division factor 128 */
	ADC_CLOCK_PRESCALER_DIV128 = ADC_CTRLB_PRESCALER_DIV128,
	/** ADC clock division factor 256 */
	ADC_CLOCK_PRESCALER_DIV256 = ADC_CTRLB_PRESCALER_DIV256,
	/** ADC clock division factor 512 */
	ADC_CLOCK_PRESCALER_DIV512 = ADC_CTRLB_PRESCALER_DIV512,
};

/**
 * \brief ADC resolution enum
 *
 * Enum for the possible resolution values for the ADC.
 *
 */
enum adc_resolution {
	/** ADC 12-bit resolution */
	ADC_RESOLUTION_12BIT = ADC_CTRLB_RESSEL_12BIT,
	/** ADC 16-bit resolution, for averaging mode output */
	ADC_RESOLUTION_16BIT = ADC_CTRLB_RESSEL_16BIT,
	/** ADC 10-bit resolution */
	ADC_RESOLUTION_10BIT = ADC_CTRLB_RESSEL_10BIT,
	/** ADC 8-bit resolution */
	ADC_RESOLUTION_8BIT = ADC_CTRLB_RESSEL_8BIT,
};

/**
 * \brief ADC window monitor mode enum
 *
 * Enum for the possible window monitor modes for the ADC.
 *
 */
enum adc_window_mode {
	/** No window mode */
	ADC_WINDOW_MODE_DISABLE          = ADC_WINCTRL_WINMODE_DISABLE,
	/** RESULT > WINLT */
	ADC_WINDOW_MODE_ABOVE_LOWER      = ADC_WINCTRL_WINMODE_MODE1,
	/** RESULT < WINUT */
	ADC_WINDOW_MODE_BELOW_UPPER      = ADC_WINCTRL_WINMODE_MODE2,
	/** WINLT < RESULT < WINUT */
	ADC_WINDOW_MODE_BETWEEN          = ADC_WINCTRL_WINMODE_MODE3,
	/** !(WINLT < RESULT < WINUT) */
	ADC_WINDOW_MODE_BETWEEN_INVERTED = ADC_WINCTRL_WINMODE_MODE4,
};

/**
 * \brief ADC gain factor selection enum
 *
 * Enum for the possible gain factor values for the ADC.
 *
 */
enum adc_gain_factor {
	/** 1x gain */
	ADC_GAIN_FACTOR_1X   = ADC_INPUTCTRL_GAIN_1X,
	/** 2x gain */
	ADC_GAIN_FACTOR_2X   = ADC_INPUTCTRL_GAIN_2X,
	/** 4x gain */
	ADC_GAIN_FACTOR_4X   = ADC_INPUTCTRL_GAIN_4X,
	/** 8x gain */
	ADC_GAIN_FACTOR_8X   = ADC_INPUTCTRL_GAIN_8X,
	/** 16x gain */
	ADC_GAIN_FACTOR_16X  = ADC_INPUTCTRL_GAIN_16X,
	/** 1/2x gain */
	ADC_GAIN_FACTOR_DIV2 = ADC_INPUTCTRL_GAIN_DIV2,
};

/**
 * \brief ADC event action enum
 *
 * Enum for the possible actions to take on an incoming event.
 *
 */
enum adc_event_action {
	/** Event action disabled */
	ADC_EVENT_ACTION_DISABLED = 0,
	/** Flush ADC and start conversion */
	ADC_EVENT_ACTION_FLUSH_START_CONV   = ADC_EVCTRL_SYNCEI,
	/** Start conversion */
	ADC_EVENT_ACTION_START_CONV   = ADC_EVCTRL_STARTEI,
};


/**
 * \brief ADC positive MUX input selection enum
 *
 * Enum for the possible positive MUX input selections for the ADC.
 *
 */
enum adc_positive_input {
	/** ADC0 pin */
	ADC_POSITIVE_INPUT_PIN0          = ADC_INPUTCTRL_MUXPOS_PIN0,
	/** ADC1 pin */
	ADC_POSITIVE_INPUT_PIN1          = ADC_INPUTCTRL_MUXPOS_PIN1,
	/** ADC2 pin */
	ADC_POSITIVE_INPUT_PIN2          = ADC_INPUTCTRL_MUXPOS_PIN2,
	/** ADC3 pin */
	ADC_POSITIVE_INPUT_PIN3          = ADC_INPUTCTRL_MUXPOS_PIN3,
	/** ADC4 pin */
	ADC_POSITIVE_INPUT_PIN4          = ADC_INPUTCTRL_MUXPOS_PIN4,
	/** ADC5 pin */
	ADC_POSITIVE_INPUT_PIN5          = ADC_INPUTCTRL_MUXPOS_PIN5,
	/** ADC6 pin */
	ADC_POSITIVE_INPUT_PIN6          = ADC_INPUTCTRL_MUXPOS_PIN6,
	/** ADC7 pin */
	ADC_POSITIVE_INPUT_PIN7          = ADC_INPUTCTRL_MUXPOS_PIN7,
	/** ADC8 pin */
	ADC_POSITIVE_INPUT_PIN8          = ADC_INPUTCTRL_MUXPOS_PIN8,
	/** ADC9 pin */
	ADC_POSITIVE_INPUT_PIN9          = ADC_INPUTCTRL_MUXPOS_PIN9,
	/** ADC10 pin */
	ADC_POSITIVE_INPUT_PIN10         = ADC_INPUTCTRL_MUXPOS_PIN10,
	/** ADC11 pin */
	ADC_POSITIVE_INPUT_PIN11         = ADC_INPUTCTRL_MUXPOS_PIN11,
	/** ADC12 pin */
	ADC_POSITIVE_INPUT_PIN12         = ADC_INPUTCTRL_MUXPOS_PIN12,
	/** ADC13 pin */
	ADC_POSITIVE_INPUT_PIN13         = ADC_INPUTCTRL_MUXPOS_PIN13,
	/** ADC14 pin */
	ADC_POSITIVE_INPUT_PIN14         = ADC_INPUTCTRL_MUXPOS_PIN14,
	/** ADC15 pin */
	ADC_POSITIVE_INPUT_PIN15         = ADC_INPUTCTRL_MUXPOS_PIN15,
	/** ADC16 pin */
	ADC_POSITIVE_INPUT_PIN16         = ADC_INPUTCTRL_MUXPOS_PIN16,
	/** ADC17 pin */
	ADC_POSITIVE_INPUT_PIN17         = ADC_INPUTCTRL_MUXPOS_PIN17,
	/** ADC18 pin */
	ADC_POSITIVE_INPUT_PIN18         = ADC_INPUTCTRL_MUXPOS_PIN18,
	/** ADC19 pin */
	ADC_POSITIVE_INPUT_PIN19         = ADC_INPUTCTRL_MUXPOS_PIN19,
	/** ADC20 pin */
	ADC_POSITIVE_INPUT_PIN20         = ADC_INPUTCTRL_MUXPOS_PIN20,
	/** ADC21 pin */
	ADC_POSITIVE_INPUT_PIN21         = ADC_INPUTCTRL_MUXPOS_PIN21,
	/** ADC22 pin */
	ADC_POSITIVE_INPUT_PIN22         = ADC_INPUTCTRL_MUXPOS_PIN22,
	/** ADC23 pin */
	ADC_POSITIVE_INPUT_PIN23         = ADC_INPUTCTRL_MUXPOS_PIN23,
	/** Temperature reference */
	ADC_POSITIVE_INPUT_TEMP          = ADC_INPUTCTRL_MUXPOS_TEMP,
	/** Bandgap voltage */
	ADC_POSITIVE_INPUT_BANDGAP       = ADC_INPUTCTRL_MUXPOS_BANDGAP,
	/** 1/4 scaled core supply */
	ADC_POSITIVE_INPUT_SCALEDCOREVCC = ADC_INPUTCTRL_MUXPOS_SCALEDCOREVCC,
	/** 1/4 scaled I/O supply */
	ADC_POSITIVE_INPUT_SCALEDIOVCC   = ADC_INPUTCTRL_MUXPOS_SCALEDIOVCC,
	/** DAC input */
	ADC_POSITIVE_INPUT_DAC           = ADC_INPUTCTRL_MUXPOS_DAC,
};

/**
 * \brief ADC negative MUX input selection enum
 *
 * Enum for the possible negative MUX input selections for the ADC.
 *
 */
enum adc_negative_input {
	/** ADC0 pin */
	ADC_NEGATIVE_INPUT_PIN0          = ADC_INPUTCTRL_MUXNEG_PIN0,
	/** ADC1 pin */
	ADC_NEGATIVE_INPUT_PIN1          = ADC_INPUTCTRL_MUXNEG_PIN1,
	/** ADC2 pin */
	ADC_NEGATIVE_INPUT_PIN2          = ADC_INPUTCTRL_MUXNEG_PIN2,
	/** ADC3 pin */
	ADC_NEGATIVE_INPUT_PIN3          = ADC_INPUTCTRL_MUXNEG_PIN3,
	/** ADC4 pin */
	ADC_NEGATIVE_INPUT_PIN4          = ADC_INPUTCTRL_MUXNEG_PIN4,
	/** ADC5 pin */
	ADC_NEGATIVE_INPUT_PIN5          = ADC_INPUTCTRL_MUXNEG_PIN5,
	/** ADC6 pin */
	ADC_NEGATIVE_INPUT_PIN6          = ADC_INPUTCTRL_MUXNEG_PIN6,
	/** ADC7 pin */
	ADC_NEGATIVE_INPUT_PIN7          = ADC_INPUTCTRL_MUXNEG_PIN7,
	/** ADC8 pin */
	ADC_NEGATIVE_INPUT_PIN8          = ADC_INPUTCTRL_MUXNEG_PIN8,
	/** ADC9 pin */
	ADC_NEGATIVE_INPUT_PIN9          = ADC_INPUTCTRL_MUXNEG_PIN9,
	/** ADC10 pin */
	ADC_NEGATIVE_INPUT_PIN10         = ADC_INPUTCTRL_MUXNEG_PIN10,
	/** ADC11 pin */
	ADC_NEGATIVE_INPUT_PIN11         = ADC_INPUTCTRL_MUXNEG_PIN11,
	/** ADC12 pin */
	ADC_NEGATIVE_INPUT_PIN12         = ADC_INPUTCTRL_MUXNEG_PIN12,
	/** ADC13 pin */
	ADC_NEGATIVE_INPUT_PIN13         = ADC_INPUTCTRL_MUXNEG_PIN13,
	/** ADC14 pin */
	ADC_NEGATIVE_INPUT_PIN14         = ADC_INPUTCTRL_MUXNEG_PIN14,
	/** ADC15 pin */
	ADC_NEGATIVE_INPUT_PIN15         = ADC_INPUTCTRL_MUXNEG_PIN15,
	/** ADC16 pin */
	ADC_NEGATIVE_INPUT_PIN16         = ADC_INPUTCTRL_MUXNEG_PIN16,
	/** ADC17 pin */
	ADC_NEGATIVE_INPUT_PIN17         = ADC_INPUTCTRL_MUXNEG_PIN17,
	/** ADC18 pin */
	ADC_NEGATIVE_INPUT_PIN18         = ADC_INPUTCTRL_MUXNEG_PIN18,
	/** ADC19 pin */
	ADC_NEGATIVE_INPUT_PIN19         = ADC_INPUTCTRL_MUXNEG_PIN19,
	/** ADC20 pin */
	ADC_NEGATIVE_INPUT_PIN20         = ADC_INPUTCTRL_MUXNEG_PIN20,
	/** ADC21 pin */
	ADC_NEGATIVE_INPUT_PIN21         = ADC_INPUTCTRL_MUXNEG_PIN21,
	/** ADC22 pin */
	ADC_NEGATIVE_INPUT_PIN22         = ADC_INPUTCTRL_MUXNEG_PIN22,
	/** ADC23 pin */
	ADC_NEGATIVE_INPUT_PIN23         = ADC_INPUTCTRL_MUXNEG_PIN23,
	/** Internal ground */
	ADC_NEGATIVE_INPUT_GND           = ADC_INPUTCTRL_MUXNEG_GND,
	/** I/O ground */
	ADC_NEGATIVE_INPUT_IOGND         = ADC_INPUTCTRL_MUXNEG_IOGND,
};

/**
 * \brief ADC averaging selection enum
 *
 * Enum for the possible numbers of ADC samples to average.
 *
 */
enum adc_average_samples {
	/** No averaging */
	ADC_AVERAGE_DISABLE      = ADC_AVGCTRL_SAMPLENUM_1,
	/** Average 2 samples */
	ADC_AVERAGE_SAMPLES_2    = ADC_AVGCTRL_SAMPLENUM_2,
	/** Average 4 samples */
	ADC_AVERAGE_SAMPLES_4    = ADC_AVGCTRL_SAMPLENUM_4,
	/** Average 8 samples */
	ADC_AVERAGE_SAMPLES_8    = ADC_AVGCTRL_SAMPLENUM_8,
	/** Average 16 samples */
	ADC_AVERAGE_SAMPLES_16   = ADC_AVGCTRL_SAMPLENUM_16,
	/** Average 32 samples */
	ADC_AVERAGE_SAMPLES_32   = ADC_AVGCTRL_SAMPLENUM_32,
	/** Average 64 samples */
	ADC_AVERAGE_SAMPLES_64   = ADC_AVGCTRL_SAMPLENUM_64,
	/** Average 128 samples */
	ADC_AVERAGE_SAMPLES_128  = ADC_AVGCTRL_SAMPLENUM_128,
	/** Average 265 samples */
	ADC_AVERAGE_SAMPLES_265  = ADC_AVGCTRL_SAMPLENUM_256,
	/** Average 512 samples */
	ADC_AVERAGE_SAMPLES_512  = ADC_AVGCTRL_SAMPLENUM_512,
	/** Average 1024 samples */
	ADC_AVERAGE_SAMPLES_1024 = ADC_AVGCTRL_SAMPLENUM_1024,
};

/**
 * \brief ADC oversampling and decimation enum
 *
 * Enum for the possible numbers of bits resolution can be increased by when
 * using oversampling and decimation.
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
 * \brief Window monitor configuration structure
 *
 * Window monitor configuration structure. Part of the \ref adc_conf struct
 * and will be initialized by \ref adc_get_config_defaults .
 */
struct adc_window_conf {
	/** Selected window mode */
	enum adc_window_mode     window_mode;
	/** Lower window value */
	int32_t                  window_lower_value;
	/** Upper window value */
		int32_t                  window_upper_value;
};

/**
 * \brief Event configuration structure
 *
 * Event configuration structure. Part of the \ref adc_conf struct and will
 * be initialized by \ref adc_get_config_defaults .
 */
struct adc_event_conf {
	/** Event action to take on incoming event */
	enum adc_event_action event_action;
	/** Enable event generation on conversion done */
	bool generate_event_on_conversion_done;
	/** Enable event generation on window monitor */
	bool generate_event_on_window_monitor;
};

/**
 * \brief Gain and offsett correction configuration structure
 *
 * Gain and offsett correction configuration structure.
 * Part of the \ref adc_conf struct and will  be initialized by
 * \ref adc_get_config_defaults .
 */
struct adc_correction_conf {
	/**
	Enables correction for gain and offset based on values of gain_correction and
	offset_correction if set to true.
	*/
	bool correction_enable;
	/**
	* This value defines how the ADC conversion result is compensated for gain
	* error before written to the result register. This is a fractional value,
	* 1-bit integer plus 11-bits fraction, therefore
	* 1/2 <= gain_correction < 2.
	* gain_correction values ranges from 0b010000000000 to 0b111111111111.
	*/
	uint16_t gain_correction;
	/**
	* This value defines how the ADC conversion result is compensated for
	* offset error before written to the result register. This is a 12-bit
	* value in two’s complement format.
	*/
	int16_t offset_correction;
};

/**
 * \brief Pin scan configuration structure
 *
 * Pin scan configuration structure. Part of the \ref adc_conf struct and will
 * be initialized by \ref adc_get_config_defaults .
 */
struct adc_pin_scan_conf {
	/**
	 * Offset (relative to selected positive input) of first input pin to be
	 * used in pin scan mode.
	 */
	uint8_t offset_start_scan;
	/**
	 * Number of input pins to scan in pin scan mode.
	 * inputs_to_scan < 2 will disable pin scan mode.
	 */
	uint8_t inputs_to_scan;
};

/**
 * \brief ADC configuration structure
 *
 * Configuration structure for an ADC instance. This structure should be
 * initialized by the \ref adc_get_config_defaults()
 * function before being modified by the user application.
 */
struct adc_conf {
	/** GCLK generator used to clock the peripheral */
	enum gclk_generator       clock_source;
	/** Voltage reference */
	enum adc_reference       reference;
	/** Clock prescaler */
	enum adc_clock_prescaler clock_prescaler;
	/** Result resolution */
	enum adc_resolution      resolution;
	/** Gain factor */
	enum adc_gain_factor     gain_factor;
	/** Positive MUX input */
	enum adc_positive_input  positive_input;
	/** Negative MUX input */
	enum adc_negative_input  negative_input;
	/** Number of samples to average when using 16-bit resolution */
	enum adc_average_samples average_samples;
	/** Oversampling and decimation mode */
	enum adc_oversampling_and_decimation oversampling_and_decimation;
	/** Left adjusted result */
	bool left_adjust;
	/** Enables differential mode if true */
	bool differential_mode;
	/** Enables free running mode if true */
	bool freerunning;
	/** Enables ADC in standby sleep mode if true */
	bool run_in_standby;
	/**
	Enables reference buffer offset compensation if true.
	This will increase the accuracy of the gain stage, but decreasee the input
	impedance; therefore the startup time of the reference must be increased.
	*/
	bool reference_compensation_enable;
	/**
	This value (0-63) control the ADC sampling time in number of half ADC
	prescaled clock cycles (depends of ADC_PRESCALER value), thus
	controlling the ADC input impedance.
	Sampling time is set according to the formula:
	Sample time = (sample_length+1) * (ADCclk / 2)
	*/
	uint8_t sample_length;
	/** Window monitor configuration structure */
	struct adc_window_conf window;
	/** Gain and offsett correction configuration structure */
	struct adc_correction_conf correction;
	/** Event configuration structure */
	struct adc_event_conf event;
	/** Pin scan configuration structure */
	struct adc_pin_scan_conf pin_scan;
};

/**
 * \brief ADC software instance structure
 *
 * ADC software instance structure.
 *
 */
struct adc_dev_inst {
	/** Pointer to ADC hardware module */
	Adc *hw_dev;
};

/**
 * \internal Wait for synchronization to finish
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

/**
 * \name Driver initialization and configuration
 * @{
 */
enum status_code adc_init(struct adc_dev_inst *const dev_inst, Adc *hw_dev,
		struct adc_conf *config);

/**
 * \brief Initializes an ADC configuration structure to defaults
 *
 *  This function will initialize a given ADC configuration struct to a set of
 *  known default values. This function should be called on any new
 *  instance of the configuration struct before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
  *   \li GCLK generator 0 (GCLK main) clock source
 *   \li 1V from internal bandgap reference
 *   \li Div 4 clock prescaler
 *   \li 12 bit resolution
 *   \li Window monitor disabled
 *   \li No gain
 *   \li Positive input on ADC PIN 0
 *   \li Negative input on ADC PIN 1
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
 * \param[out] config  Pointer to configuration struct to initialize to
 *                     default values
 */
static inline void adc_get_config_defaults(struct adc_conf *const config)
{
	Assert(config);
	config->clock_source = GCLK_GENERATOR_0;
	config->reference = ADC_REFERENCE_INT1V;
	config->clock_prescaler = ADC_CLOCK_PRESCALER_DIV4;
	config->resolution = ADC_RESOLUTION_12BIT;
	config->window.window_mode = ADC_WINDOW_MODE_DISABLE;
	config->window.window_upper_value = 0;
	config->window.window_lower_value = 0;
	config->gain_factor = ADC_GAIN_FACTOR_1X;
	config->positive_input = ADC_POSITIVE_INPUT_PIN0 ;
	config->negative_input = ADC_NEGATIVE_INPUT_PIN1 ;
	config->average_samples = ADC_AVERAGE_DISABLE;
	config->oversampling_and_decimation =
			ADC_OVERSAMPLING_AND_DECIMATION_DISABLE;
	config->left_adjust = false;
	config->differential_mode = false;
	config->freerunning = false;
	config->event.event_action = ADC_EVENT_ACTION_DISABLED;
	config->event.generate_event_on_conversion_done = false;
	config->event.generate_event_on_window_monitor = false;
	config->run_in_standby = false;
	config->reference_compensation_enable = false;
	config->correction.correction_enable = false;
	config->correction.gain_correction = 0;
	config->correction.offset_correction = 0;
	config->sample_length = 0;
	config->pin_scan.offset_start_scan = 0;
	config->pin_scan.inputs_to_scan = 0;
}

/** @} */

/**
 * \name Enable, disable and reset ADC module, start conversion and read result
 * @{
 */

/**
 * \brief Enables the ADC module
 *
 * This function will enable the ADC module.
 *
 * \param[in] dev_inst    Pointer to the ADC software instance struct
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
 * \brief Disables the ADC module
 *
 * This function will disable the ADC module.
 *
 * \param[in] dev_inst Pointer to the ADC software instance struct
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
 * \brief Resets the ADC module
 *
 * This function will reset the ADC module.
 *
 * \param[in] dev_inst Pointer to the ADC software instance struct
 */
static inline void adc_reset(struct adc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	/* Disable to make sure the pipeline is flushed before reset */
	adc_disable(dev_inst);

	/* Software reset the module */
	_adc_wait_for_sync(adc_module);
	adc_module->CTRLA.reg |= ADC_CTRLA_SWRST;
}

/**
 * \brief Starts an ADC conversion
 *
 * This function will start an ADC conversion
 *
 * \param[in] dev_inst      Pointer to the ADC software instance struct
 */
static inline void adc_start_conversion(struct adc_dev_inst *const dev_inst)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	_adc_wait_for_sync(adc_module);
	adc_module->SWTRIG.reg |= ADC_SWTRIG_START;
}

/* Prototype for adc_is_interrupt_flag_set */
static inline bool adc_is_interrupt_flag_set(struct adc_dev_inst *const dev_inst,
		enum adc_interrupt_flag interrupt_flag);
/* Prototype for adc_clear_interrupt_flag */
static inline void adc_clear_interrupt_flag(struct adc_dev_inst *const dev_inst,
		enum adc_interrupt_flag interrupt_flag);
/**
 * \brief Reads the ADC result
 *
 * This function will read the result from an ADC conversion. It will clear
 * the ADC_INTERRUPT_RESULT_READY flag after reading.
 *
 * \param[in] dev_inst      Pointer to the ADC software instance struct
 * \param[out] result       Pointer to store the result value in
 *
 * \return Status of the procedure
 * \retval STATUS_OK                The result was retrieved successfully
 * \retval STATUS_BUSY          The result is not ready
 */
static inline enum status_code adc_read(
		struct adc_dev_inst *const dev_inst, uint16_t *result)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(result);

	if (!adc_is_interrupt_flag_set(dev_inst, ADC_INTERRUPT_RESULT_READY)) {
		/* Result not ready */
		return STATUS_BUSY;
	}

	Adc *const adc_module = dev_inst->hw_dev;

	_adc_wait_for_sync(adc_module);
	/* Get ADC result */
	*result = adc_module->RESULT.reg;
	/* Reset ready flag */
	adc_clear_interrupt_flag(dev_inst, ADC_INTERRUPT_RESULT_READY);

	return STATUS_OK;
}

/** @} */

/**
 * \name Runtime changes of ADC module
 * @{
 */

/**
 * \brief Flushes the ADC pipeline
 *
 * This function will flush the pipeline and restart the ADC clock on the next
 * peripheral clock edge.
 * All conversions in progress will be lost.
 * When flush is complete, the module will resume where it left off.
 *
 * \param[in] dev_inst    Pointer to the ADC software instance struct
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
 * \brief Sets the ADC window mode
 *
 * This function will set the ADC window mode.
 *
 * \param[in] dev_inst           Pointer to the ADC software instance struct
 * \param[in] adc_window_mode    Window monitor mode to set
 * \param[in] window_lower_value Lower window monitor threshold value
 * \param[in] window_upper_value Upper window monitor threshold value
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
 * \brief Sets ADC gain factor
 *
 * This function will set the ADC gain factor.
 *
 * \param[in] dev_inst       Pointer to the ADC software instance struct
 * \param[in] gain_factor    Gain factor value to set
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
 * \brief Sets pin scan mode
 *
 * This function will set up the pin scan mode.
 * In pin scan mode, the first conversion will start at the configured
 * positive input + start_offset. When a conversion is done, a conversion will
 * start on the next input, until inputs_to_scan conversions are made.
 *
 * \param[in] dev_inst          Pointer to the ADC software instance struct
 * \param[in] inputs_to_scan    Number of input pins to do conversion on
 *                              (must be two or more)
 * \param[in] start_offset      Offset of first pin to scan (relative to
 *                               configured positive input)
 *
 * \return Status of the procedure
 * \retval STATUS_OK              Pin scan mode has been set successfully
 * \retval STATUS_ERR_INVALID_ARG Number of input pins to scan or offset
 *                                has an invalid value
 */
static inline enum status_code adc_set_pin_scan_mode(struct adc_dev_inst *const dev_inst,
		uint8_t inputs_to_scan, uint8_t start_offset)

{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	if (inputs_to_scan > 0) {
		/*
		* Number of input sources included is the value written to INPUTSCAN
		* plus 1.
		*/
		inputs_to_scan--;
	}
	if (inputs_to_scan > ADC_INPUTCTRL_INPUTSCAN_Msk ||
			start_offset > ADC_INPUTCTRL_INPUTOFFSET_Msk) {
		/* Invalid number of input pins */
		return STATUS_ERR_INVALID_ARG;
	}

	/* Wait for synchronization to finish */
	_adc_wait_for_sync(adc_module);

	/* Set pin scan mode */
	adc_module->INPUTCTRL.reg =
			(adc_module->INPUTCTRL.reg &
			 ~(ADC_INPUTCTRL_INPUTSCAN_Msk | ADC_INPUTCTRL_INPUTOFFSET_Msk)) |
			(start_offset   << ADC_INPUTCTRL_INPUTOFFSET_Pos) |
			(inputs_to_scan << ADC_INPUTCTRL_INPUTSCAN_Pos);

	return STATUS_OK;
}

/**
 * \brief Disables pin scan mode
 *
 * This function will disable pin scan mode. Next conversion will be done
 * on only one pin.
 *
 * \param[in] dev_inst     Pointer to the ADC software instance struct
 */
static inline void adc_disable_pin_scan_mode(struct adc_dev_inst *const dev_inst)
{
	/* Disable pin scan mode */
	adc_set_pin_scan_mode(dev_inst, 0, 0);
}


/**
 * \brief Sets positive ADC input pin
 *
 * This function will set the positive ADC input pin.
 *
 * \param[in] dev_inst        Pointer to the ADC software instance struct
 * \param[in] positive_input  Positive input pin
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
 * \brief Sets negative ADC input pin for differential mode
 *
 * This function will set the negative ADC input pin to be used in
 * differential mode.
 *
 * \param[in] dev_inst        Pointer to the ADC software instance struct
 * \param[in] negative_input  Negative input pin
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

/** @} */

/**
 * \name Checking and clearing of interrupt flags
 * @{
 */
/**
 * \brief Checks if a given interrupt flag is set
 *
 * This function will check if a given interrupt flag is set.
 *
 * \param[in] dev_inst         Pointer to the ADC software instance struct
 * \param[in] interrupt_flag   Interrupt flag to check
 *
 * \return Boolean value to indicate if the interrupt flag is set or not.
 * \retval true   The flag is set
 * \retval false  The flag is not set
 */
static inline bool adc_is_interrupt_flag_set(struct adc_dev_inst *const dev_inst,
		enum adc_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;
	return adc_module->INTFLAG.reg & interrupt_flag;
}

/**
 * \brief Clears a given interrupt flag
 *
 * This function will clear a given interrupt flag.
 *
 * \param[in] dev_inst         Pointer to the ADC software instance struct
 * \param[in] interrupt_flag   Interrupt flag to clear
 */
static inline void adc_clear_interrupt_flag(struct adc_dev_inst *const dev_inst,
		enum adc_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Adc *const adc_module = dev_inst->hw_dev;

	/* Clear interrupt flag */
	adc_module->INTFLAG.reg = interrupt_flag;
}
/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page adc_extra_info Extra Information
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
 *  <tr>
 *		<td>ADC</td>
 *		<td>Analog-to-Digital Converter</td>
 *	</tr>
 *  <tr>
 *		<td>DAC</td>
 *		<td>Digital-to-Analog Converter</td>
 *	</tr>
 *  <tr>
 *		<td>SCIF</td>
 *		<td>System Control Interface</td>
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
