/**
 * \file
 *
 * \brief SAMD20 Peripheral Digital to Analog Converter Driver
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
#ifndef DAC_H_INCLUDED
#define DAC_H_INCLUDED

/**
 * \defgroup asfdoc_samd20_dac_group SAMD20 Digital-to-Analog Driver (DAC)
 *
 * This driver for SAMD20 devices provides an interface for the conversion of
 * digital values to analog voltage.
 *
 * The following peripherals are used by this module:
 *
 *  - DAC (Digital to Analog Converter)
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_samd20_dac_prerequisites
 *  - \ref asfdoc_samd20_dac_module_overview
 *  - \ref asfdoc_samd20_dac_special_considerations
 *  - \ref asfdoc_samd20_dac_extra_info
 *  - \ref asfdoc_samd20_dac_examples
 *  - \ref asfdoc_samd20_dac_api_overview
 *
 *
 * \section asfdoc_samd20_dac_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samd20_dac_module_overview Module Overview
 *
 * The Digital-to-Analog converter converts a digital value to analog voltage.
 * The SAMD20 DAC module has one channel with 10-bit resolution, and is capable
 * of converting up to 350k samples per second (ksps).
 *
 * A common use of DAC is to generate audio signals by connecting the DAC
 * output to a speaker, or to generate a reference voltage; either for an
 * external circuit or an internal peripheral such as the Analog Comparator.
 *
 * After being set up, the DAC will convert the digital
 * value written to the conversion data register (DATA) to an analog value
 * on either the VOUT pin or internally available for use as input to the AC
 * or ADC.
 * Writing the DATA register will start a new conversion. It is also possible
 * to trigger the conversion from the event system.
 *
 * A simplified block diagram of the DAC can be seen in the following figure:
 *
 * \dot
 * digraph overview {
 *  splines = false;
 *  nodesep = .2;
 *  rankdir=LR;
 *  data [label="DATA", shape=box, style=filled, fillcolor=lightblue];
 *  empty1 [label="", style=invisible];
 *  empty2 [label="", style=invisible];
 *  databuf [label="DATABUF", shape=box, style=filled, fillcolor=lightblue];
 *  evctrl [label="EVCTRL", shape=box, style=filled, fillcolor=lightblue];
 *  eventcontrol [label="EVENT CONTROL", shape=box, style=filled, fillcolor=darkolivegreen1];
 *  dac [label="DAC", shape=polygon, sides=5, orientation=-90, distortion=-0.6, style=filled, fillcolor=darkolivegreen1, height=1, width=1];
 *  outputdriver [label="output\n driver", shape=polygon, sides=3, orientation=-90, style=filled, fillcolor=darkolivegreen1];
 *  vout [label="VOUT", shape=box];
 *  aref [label="AREF", shape=box];
 *  avcc [label="AVCC", shape=none];
 *  int1v [label="INT1V", shape=none];
 *  acadc [label="AC/ADC", shape=none];
 *  mux [label="", shape=polygon, sides=4, distortion=0.6, orientation=-90, style=filled, fillcolor=black, height=0.9, width=0.2];
 *
 *  edge [dir="back"];
 *  eventcontrol -> empty2 [label="START"]
 *  edge [dir="forward"];
 *  evctrl -> eventcontrol;
 *  eventcontrol -> empty1 [label="EMPTY"]
 *  databuf -> data;
 *  data -> dac;
 *  dac -> outputdriver;
 *  dac -> acadc;
 *  outputdriver -> vout;
 *  eventcontrol -> dac [constraint=true, style="invis"];
 *  dac -> mux [constraint=true, style="invis"];
 *  mux:w -> dac:s;
 *  edge [dir="both"];
 *  evctrl -> databuf;
 *  edge [dir="back"];
 *  mux:se -> aref;
 *  mux:ec -> avcc;
 *  mux:ne -> int1v;
 *  {rank=same; evctrl databuf data }
 *  {rank=same; vout aref }
 *  {rank=same; eventcontrol dac mux }
 * }
 *
 * \enddot
 *
 * \subsection asfdoc_samd20_dac_conversion_range Conversion Range
 * The conversion range is between GND and the selected voltage reference.
 * Available voltage references are:
 * \li AVCC voltage reference
 * \li Internal 1 V reference (INT1V)
 * \li External voltage reference (AREF)
 *
 * The output voltage from a DAC channel is given as:
 * \f[
 *    V_{OUT} = \frac{DATA}{0x3FF} \times VREF
 * \f]
 *
 * \subsection asfdoc_samd20_dac_conversion Conversion
 * The digital value written to the conversion data register (DATA) will be
 * converted to an analog value.
 * Writing the DATA register will start a new conversion.
 * It is also possible to write the conversion data to the DATABUF register,
 * the writing of the DATA register can then be triggered from the event
 * system, which will load the value from DATABUF to DATA.
 *
 * \subsection asfdoc_samd20_dac_analog_output Analog Output
 * The analog output value can be output to either the VOUT pin or internally,
 * but not both at the same time.
 *
 * \subsubsection asfdoc_samd20_dac_analog_output_external External Output
 * The output buffer must be enabled in order to drive the DAC output to the
 * VOUT pin. Due to the output buffer, the DAC has high drive strength, and is
 * capable of driving both resistive and capacitive loads, as well as loads
 * which combine both.
 *
 * \subsubsection asfdoc_samd20_dac_analog_output_internal Internal Output
 * The analog value can be internally available for use as input to the
 * AC or ADC modules.
 *
 * \subsection asfdoc_samd20_dac_events Events
 * Events generation and event actions are configurable in the DAC.
 * The DAC has one event line input and one event output: Start Conversion
 * and Data Buffer Empty.
 *
 * If the start conversion event is enabled in the
 * configuration, an incoming event will load data from the data buffer to
 * the data register and start a new conversion.
 * This method synchronizes conversions with external events and ensures
 * regular and fixed conversion intervals.
 *
 * If the data buffer empty event is enabled in the
 * configuration, an event is generated when the DATABUF becomes empty and
 * new data can be loaded to the buffer.
 *
 * \subsection asfdoc_samd20_dac_data_adjust Left and Right Adjusted Values
 * The 10-bit input value to the DAC is contained in a 16-bit register. This
 * can be configured to be either left or right adjusted. In the figure below
 * both options are shown, and the position of the most (MSB) and the least
 * (LSB) significant bit are shown. The unused bits should always be written to
 * zero.
 *
 * \dot
 * digraph {
 *   subgraph cluster_right {
 *      msbl [label="MSB", shape=none, group="msbl"];
 *      lsbl [label="LSB", shape=none];
 *      node [shape=none];
 *      color="white";
 *      reg_left [label=<
 *        <table cellspacing="0" cellpadding="2" width="100%">
 *          <tr>
 *            <td port="msb">15</td>
 *            <td>14</td>
 *            <td>13</td>
 *            <td>12</td>
 *            <td>11</td>
 *            <td>10</td>
 *            <td>9</td>
 *            <td>8</td>
 *            <td>7</td>
 *            <td port="lsb">6</td>
 *            <td>5</td>
 *            <td>4</td>
 *            <td>3</td>
 *            <td>2</td>
 *            <td>1</td>
 *            <td>0</td>
 *          </tr>
 *          <tr>
 *            <td COLSPAN="10"> DATA[9:0] </td>
 *            <td BGCOLOR="lightgray"> </td>
 *            <td BGCOLOR="lightgray"> </td>
 *            <td BGCOLOR="lightgray"> </td>
 *            <td BGCOLOR="lightgray"> </td>
 *            <td BGCOLOR="lightgray"> </td>
 *            <td BGCOLOR="lightgray"> </td>
 *          </tr>
 *        </table>
 *      >];
 *      msbl -> reg_left:msb:nc;
 *      lsbl -> reg_left:lsb;
 *      label ="Left adjusted.\n";
 *   }
 *   subgraph cluster_left {
 *      rankdir=TB;
 *      msb [label="MSB", shape=none];
 *      lsb [label="LSB", shape=none];
 *      color="white";
 *      node [shape=none];
 *      reg_right [label=<
 *        <table cellspacing="0" cellpadding="2">
 *          <tr>
 *            <td>15</td>
 *            <td>14</td>
 *            <td>13</td>
 *            <td>12</td>
 *            <td>11</td>
 *            <td>10</td>
 *            <td port="msb">9</td>
 *            <td>8</td>
 *            <td>7</td>
 *            <td>6</td>
 *            <td>5</td>
 *            <td>4</td>
 *            <td>3</td>
 *            <td>2</td>
 *            <td>1</td>
 *            <td port="lsb">0</td>
 *          </tr>
 *          <tr>
 *            <td BGCOLOR="lightgray"></td>
 *            <td BGCOLOR="lightgray"></td>
 *            <td BGCOLOR="lightgray"></td>
 *            <td BGCOLOR="lightgray"></td>
 *            <td BGCOLOR="lightgray"></td>
 *            <td BGCOLOR="lightgray"></td>
 *            <td COLSPAN="10"> DATA[9:0] </td>
 *          </tr>
 *        </table>
 *      >];
 *      msb -> reg_right:msg;
 *      lsb -> reg_right:lsb:nc;
 *      label = "Right adjusted.\n";
 *      graph [shape=none];
 *  }
 * }
 * \enddot
 *
 * \subsection asfdoc_samd20_dac_clk_sources Clock Sources
 * The clock for the DAC interface (CLK_DAC) is generated by
 * the Power Manager. This clock is turned on by default, and can be enabled
 * and disabled in the Power Manager.
 *
 * Additionally, an asynchronous clock source (GCLK_DAC) is required.
 * These clocks are normally disabled by default. The selected clock source
 * must be enabled in the Power Manager before it can be used by the DAC.
 * The DAC core operates asynchronously from the user interface and
 * peripheral bus. As a consequence, the DAC needs two clock cycles of both
 * CLK_DAC and GCLK_DAC to synchronize the values written to some of the
 * control and data registers.
 * The oscillator source for the GCLK_DAC clock is selected in the System
 * Control Interface (SCIF).
 *
 * \section asfdoc_samd20_dac_special_considerations Special Considerations
 *
 * \subsection asfdoc_samd20_dac_special_considerations_output_buffer Output Driver
 * The DAC can only do conversions in active or idle mode. However, if the
 * output buffer is enabled it will draw current even if the system is in
 * sleep mode. Therefore, always make sure that the output buffer is not
 * enabled when it is not needed, to ensure minimum power consumption.
 *
 * \subsection asfdoc_samd20_dac_special_considerations_conversion_time Conversion Time
 * DAC conversion time is approximately 2.85us. The user must ensure that new
 * data is not written to the DAC before the last conversion is complete.
 * Conversions should be triggered by a periodic event from a Timer/Counter or
 * another peripheral.
 *
 *
 * \section asfdoc_samd20_dac_extra_info Extra Information for DAC
 *
 * For extra information see \ref asfdoc_samd20_dac_extra. This includes:
 *  - \ref asfdoc_samd20_dac_extra_acronyms
 *  - \ref asfdoc_samd20_dac_extra_dependencies
 *  - \ref asfdoc_samd20_dac_extra_errata
 *  - \ref asfdoc_samd20_dac_extra_history
 *
 *
 * \section asfdoc_samd20_dac_examples Examples
 *
 * The following Quick Start guides and application examples are available for this driver:
 * - \ref asfdoc_samd20_dac_basic_use_case
 *
 *
 * \section asfdoc_samd20_dac_api_overview API Overview
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>
#include <clock.h>
#include <gclk.h>

/**
 * \brief DAC reference voltage enum.
 *
 * Enum for the possible reference voltages for the DAC
 *
 */
enum dac_reference {
	/** 1V from internal bandgap reference.*/
	/* TODO: This define needs to be updated once it is available in the header */
	DAC_REF_INT1V = 0,
	/** Analog VCC as reference. */
	/* TODO: This define needs to be updated once it is available in the header */
	DAC_REF_AVCC  = 1,
	/** External reference on AREF. */
	/* TODO: This define needs to be updated once it is available in the header */
	DAC_REF_AREF  = 2,
};

/**
 * \brief DAC output selection enum.
 *
 * Enum for the DAC output selection.
 */
enum dac_output {
	/** DAC output to VOUT pin */
	DAC_OUTPUT_EXTERNAL = DAC_CTRLB_EOEN,
	/** DAC output as internal reference */
	DAC_OUTPUT_INTERNAL = DAC_CTRLB_IOEN,
	/** No output*/
	DAC_OUTPUT_NONE = 0,
};

/**
 * \brief DAC channel selection enum.
 *
 * Enum for the DAC channel selection.
 */
enum dac_channel {
	/** Only one channel*/
	DAC_CHANNEL_0,
};

/**
 * \brief DAC device instance structure.
 * DAC software instance structure.
 *
 */
struct dac_dev_inst {
	/** DAC hardware module */
	Dac *hw_dev;
	/** DAC output selection */
	enum dac_output output;
};

/**
 * \brief DAC configuration structure.
 *
 * Configuration structure for a DAC instance. This structure should be
 * initialized by the \ref dac_get_config_defaults()
 * function before being modified by the user application.
 */
struct dac_conf {
	/** Reference voltage */
	enum dac_reference reference;
	/** Select DAC output */
	enum dac_output output;
	/** Left adjusted data */
	bool left_adjust;
	/** GCLK generator used to clock the peripheral */
	enum gclk_generator clock_source;
	/**
	 * The DAC behaves as in normal mode when the chip enters STANDBY sleep
	 * mode
	 */
	bool run_in_standby;
};

/**
 * \brief DAC channel configuration structure
 *
 * Configuration for a DAC channel. This structure should be initialized by the
 * \ref dac_ch_get_config_defaults() function before being modified by the
 * user application.
 */
struct dac_ch_conf {
	/** Enable Start Conversion Event Input */
	bool enable_start_on_event;
	/** Enable Data Buffer Empty Event Output */
	bool enable_empty_event;
};

/**
 * \name Driver initialization and configuration
 * @{
 */

void dac_ch_set_config(
		struct dac_dev_inst *const dev_inst,
		const enum dac_channel channel,
		struct dac_ch_conf *const config);

void dac_init(
		struct dac_dev_inst *const dev_inst,
		Dac *const module,
		struct dac_conf *const config);

/**
 * \brief Initializes a DAC configuration structure to defaults
 *
 *  Initializes a given DAC configuration structure to a set of
 *  known default values. This function should be called on any new
 *  instance of the configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li 1V from internal bandgap reference
 *   \li Drive the DAC output to the VOUT pin
 *   \li Right adjust data
 *   \li GCLK generator 0 (GCLK main) clock source
 *   \li The output buffer is disabled when the chip enters STANDBY sleep
 *       mode
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void dac_get_config_defaults(
		struct dac_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->reference =            DAC_REF_INT1V;
	config->output =               DAC_OUTPUT_EXTERNAL;
	config->left_adjust =          false;
	config->clock_source =         GCLK_GENERATOR_0;
	config->run_in_standby =       false;
};

/**
 * \brief Initializes a DAC channel configuration structure to defaults
 *
 *  Initializes a given DAC channel configuration structure to a set of
 *  known default values. This function should be called on any new
 *  instance of the configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Start Conversion Event Input enabled
 *   \li Start Data Buffer Empty Event Output disabled
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void dac_ch_get_config_defaults(
		struct dac_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->enable_start_on_event = true;
	config->enable_empty_event = false;
};

/** @} */

/**
 * \name Enable and disable DAC module, channels and output buffer,
 * DAC conversion
 * @{
 */

void dac_enable(
		struct dac_dev_inst *const dev_inst);

void dac_ch_enable(
		struct dac_dev_inst *const dev_inst,
		enum dac_channel channel);

void dac_disable(
		struct dac_dev_inst *const dev_inst);

void dac_ch_disable(
		struct dac_dev_inst *const dev_inst,
		enum dac_channel channel);

void dac_enable_output_buffer(
		struct dac_dev_inst *const dev_inst);

void dac_disable_output_buffer(
		struct dac_dev_inst
		*const dev_inst);

void dac_write(
		struct dac_dev_inst *const dev_inst,
		enum dac_channel channel,
		const uint16_t data,
		bool event_triggered);

void dac_reset(
		struct dac_dev_inst *const dev_inst);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page asfdoc_samd20_dac_extra Extra Information for DAC Driver
 *
 * \section asfdoc_samd20_dac_extra_acronyms Acronyms
 * The table below presents the acronyms used in this module:
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>ADC</td>
 *		<td>Analog-to-Digital Converter</td>
 *	</tr>
 *	<tr>
 *		<td>AC</td>
 *		<td>Analog Comparator</td>
 *	</tr>
 *	<tr>
 *		<td>DAC</td>
 *		<td>Digital-to-Analog Converter</td>
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
 *
 * \section asfdoc_samd20_dac_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - \ref asfdoc_samd20_pinmux_group "System Pin Multiplexer Driver"
 *
 *
 * \section asfdoc_samd20_dac_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samd20_dac_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
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
 * \page asfdoc_samd20_dac_exqsg Examples for DAC Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samd20_dac_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_samd20_dac_basic_use_case
 */

#endif /* DAC_H_INCLUDED */
