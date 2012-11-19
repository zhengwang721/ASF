/**
 * \file
 *
 * \brief SAM0+ Analog Comparator Driver
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
#ifndef AC_H_INCLUDED
#define AC_H_INCLUDED

/**
 * \defgroup sam0_ac_group SAM0+ Analog Comparator Driver (AC)
 *
 * Driver for the SAM0+ Analog Comparator. Provides a unified interface
 * for the configuration and management of the Analog Comparator
 * module, including the channel comparator input selections, window
 * modes and event generation within the device. This driver encompasses the
 * following module within the SAM0 devices:
 *
 * \li \b AC (Analog Comparator)
 *
 * Physically, the modules are interconnected within the device as shown in the
 * following diagram:
 *
 * \dot
 * digraph overview {
 *  rankdir = LR;
 *  splines = false;
 *
 *  pos_src1_1 [label="GPIO Pins", shape=none, height=0];
 *  neg_src1_1 [label="GPIO Pins", shape=none, height=0];
 *  neg_src1_2 [label="Internal DAC", shape=none, height=0];
 *  neg_src1_3 [label="Internal Refs", shape=none, height=0];
 *  pos_src2_1 [label="GPIO Pins", shape=none, height=0];
 *  neg_src2_1 [label="GPIO Pins", shape=none, height=0];
 *  neg_src2_2 [label="Internal DAC", shape=none, height=0];
 *  neg_src2_3 [label="Internal Refs", shape=none, height=0];
 *  res_out1 [label="", style=invisible];
 *  res_out2 [label="", style=invisible];
 *  res_window [label="", style=invisible];
 *
 *  mux_pos1 [label="", shape=polygon, sides=4, distortion=0.6, orientation=90, style=filled, fillcolor=black, height=0.9, width=0.2];
 *  mux_neg1 [label="", shape=polygon, sides=4, distortion=0.6, orientation=90, style=filled, fillcolor=black, height=0.9, width=0.2];
 *  mux_neg2 [label="", shape=polygon, sides=4, distortion=0.6, orientation=90, style=filled, fillcolor=black, height=0.9, width=0.2];
 *  mux_pos2 [label="", shape=polygon, sides=4, distortion=0.6, orientation=90, style=filled, fillcolor=black, height=0.9, width=0.2];
 *  ac1 [label="AC 1", shape=triangle, orientation=-90, style=filled, fillcolor=darkolivegreen1, height=1, width=1];
 *  ac2 [label="AC 2", shape=triangle, orientation=-90, style=filled, fillcolor=darkolivegreen1, height=1, width=1];
 *
 *  window_comp [label="Window\nLogic", shape=rectangle style=filled fillcolor=lightgray];
 *
 *  edge [dir="forward"];
 *
 *  pos_src1_1:e -> mux_pos1:w;
 *  mux_pos1:e -> ac1:nw [label="+"];
 *  neg_src1_1:e -> mux_neg1:nw;
 *  neg_src1_2:e -> mux_neg1:w;
 *  neg_src1_3:e -> mux_neg1:sw;
 *  mux_neg1:e -> ac1:sw [label="-"];
 *  ac1:e -> res_out1 [label="Comparator 1 Result"];
 *
 *  pos_src2_1:e -> mux_pos2:w;
 *  mux_pos2:e -> ac2:sw [label="+"];
 *  neg_src2_1:e -> mux_neg2:nw;
 *  neg_src2_2:e -> mux_neg2:w;
 *  neg_src2_3:e -> mux_neg2:sw;
 *  mux_neg2:e -> ac2:nw [label="-"];
 *  ac2:e -> res_out2 [label="Comparator 2 Result"];
 *
 *  ac1:e -> window_comp:nw;
 *  ac2:e -> window_comp:sw;
 *  window_comp:e -> res_window:w [label="Window Result"];
 *
 *  {rank=same; pos_src1_1 neg_src1_1 neg_src1_2 neg_src1_3 pos_src2_1 neg_src2_1 neg_src2_2 neg_src2_3 }
 *  {rank=same; mux_pos1 mux_neg1 mux_pos2 mux_neg2 }
 *  {rank=same; ac1 ac2 }
 *  {rank=same; res_out1 res_out2 res_window }
 * }
 * \enddot
 *
 * \section module_introduction Introduction
 *
 * \subsection module_overview Overview of the Analog Comparator
 * The Analog Comparator module compares a pair of analog input voltages, to
 * determine which one of the two is higher. This module can be used in
 * applications where a binary output indicating a higher/lower input voltage is
 * required. The comparison result can be used internally in the user
 * application, output to a GPIO pin of the device, or both.
 *
 * Each comparator requires two analog input voltages, a positive and negative
 * channel input. The result of the comparison is a binary \c true if the
 * comparator's positive channel input is higher than the comparator's negative
 * input channel, and \c false if otherwise.
 *
 * \subsection pairs_and_window_comps Comparator Pairs and Window Comparators
 * Each comparator module contains one or more comparator pairs, a set of two
 * distinct comparators which can be used independently or linked together for
 * Window Comparator mode. In this latter mode, the two comparator units in a
 * comparator pair are linked together to allow the module to detect if an input
 * voltage is below, inside, above or outside a window set by the upper and
 * lower threshold voltages set by the two comparators. If not required, window
 * comparison mode can be turned off and the two comparator units can be
 * configured and used separately.
 *
 * \subsection pos_neg_comp_mux Positive and Negative Input MUXs
 * Each comparator unit requires two input voltages, a positive and negative
 * channel (note that these names refer to the logical operation that the unit
 * performs, and both voltages should be above GND) which are then compared with
 * one another. Both the positive and negative channel inputs are connected to
 * a MUX, which allows one of several possible inputs to be selected for each
 * comparator channel.
 *
 * The exact channels available for each comparator differ for the positive and
 * negative inputs, but the same MUX choices are available for all comparator
 * units (i.e. all positive MUXes are identical, all negative MUXes are
 * identical). This allows the user application to select which voltages are
 * compared to one-another.
 *
 * When used in window mode, both comparators in the window pair should have
 * their positive channel input MUXs configured to the same input channel, with
 * the negative channel input MUXs used to set the lower and upper window
 * bounds.
 *
 * \subsection output_filtering Output Filtering
 * The output of each comparator unit can either be used directly with no
 * filtering (giving a lower latency signal, with potentially more noise around
 * the comparison threshold) or it can be passed through a multiple stage
 * digital majority filter. Several filter lengths are available, with the
 * longer stages producing a more stable result, at the expense of a higher
 * latency.
 *
 * When used in single shot mode, a single trigger of the comparator will
 * automatically perform the required number of samples to produce a correctly
 * filtered result.
 *
 * \subsection input_hysteresis Input Hysteresis
 * To prevent unwanted noise around the threshold where the comparator unit's
 * positive and negative input channels are close in voltage to one another, an
 * optional hysteresis can be used to widen the point at which the output result
 * flips. This mode will prevent a change in the comparison output unless the
 * inputs cross one-another beyond the hysteresis gap introduces by this mode.
 *
 * \subsection one_shot_cont_sampling One Shot and Continuous Sampling Modes
 * Comparators can be configured to run in either One Shot or Continuous
 * sampling modes; when in One Shot mode, the comparator will only perform a
 * comparison (and any resulting filtering, see \ref output_filtering) when
 * triggered via a software or event trigger. This mode improves the power
 * efficiency of the system by only performing comparisons when actually
 * required by the application.
 *
 * For systems requiring a lower latency or more frequent comparisons,
 * continuous mode will place the comparator into continuous sampling mode,
 * which increases the module power consumption but decreases the latency
 * between each comparison result by automatically performing a comparison on
 * every cycle of the module's clock.
 *
 * \subsection input_output_comp_events Input and Output Events
 * Each comparator unit is capable of being triggered by a both software and
 * hardware triggers. Hardware input events allow for other peripherals to
 * automatically trigger a comparison on demand - for example, a timer output
 * event could be used to trigger comparisons at a desired regular interval.
 *
 * The module's output events can similarly be used to trigger other hardware
 * modules each time a new comparison result is available. This scheme allows
 * for reduced levels of CPU usage in an application and lowers the overall
 * system response latency by directly triggering hardware peripherals from one
 * another without requiring software intervention.
 *
 * \section module_dependencies Dependencies
 * The Analog Comparator driver has the following dependencies.
 *
 * \li \ref gclk_group "GCLK" (Generic Clock Management)
 *
 * \section special_considerations Special Considerations
 *
 * The number of comparator pairs (and, thus, window comparators) within a
 * single hardware instance of the Analog Comparator module is device-specific.
 * Some devices will contain a single comparator pair, while others may have two
 * pairs; refer to your device specific datasheet for details.
 *
 * \section module_extra_info Extra Information
 * For extra information see \ref ac_extra_info.
 *
 * \section module_examples Examples
 * - \ref ac_quickstart
 *
 * \section api_overview API Overview
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief AC channel input sampling mode configuration enum.
 *
 * Enum for the possible channel sampling modes of an Analog Comparator channel.
 */
enum ac_ch_sample_mode {
	/** Continuous sampling mode; when the channel is enabled the comparator
	 *  output is available for reading at any time. */
	AC_CH_MODE_CONTINUOUS    = 0,
	/** Single shot mode; when used the comparator channel must be triggered to
	 *  perform a comparison before reading the result. */
	AC_CH_MODE_SINGLE_SHOT   = AC_SINGLE_bm,
};

/** \brief AC channel positive comparator pin input configuration enum.
 *
 * Enum for the possible channel positive pin input of an Analog Comparator
 * channel.
 */
enum ac_ch_pos_mux {
	/** Positive comparator input is connected to physical AC input pin 0. */
	AC_CH_POS_MUX_PIN0       = AC_MUXPOS_PIN0_gc,
	/** Positive comparator input is connected to physical AC input pin 1. */
	AC_CH_POS_MUX_PIN1       = AC_MUXPOS_PIN1_gc,
	/** Positive comparator input is connected to physical AC input pin 2. */
	AC_CH_POS_MUX_PIN2       = AC_MUXPOS_PIN2_gc,
	/** Positive comparator input is connected to physical AC input pin 3. */
	AC_CH_POS_MUX_PIN3       = AC_MUXPOS_PIN3_gc,
};

/** \brief AC channel negative comparator pin input configuration enum.
 *
 * Enum for the possible channel negative pin input of an Analog Comparator
 * channel.
 */
enum ac_ch_neg_mux {
	/** Negative comparator input is connected to physical AC input pin 0. */
	AC_CH_NEG_MUX_PIN0       = AC_MUXNEG_PIN0_gc,
	/** Negative comparator input is connected to physical AC input pin 1. */
	AC_CH_NEG_MUX_PIN1       = AC_MUXNEG_PIN1_gc,
	/** Negative comparator input is connected to physical AC input pin 2. */
	AC_CH_NEG_MUX_PIN2       = AC_MUXNEG_PIN2_gc,
	/** Negative comparator input is connected to physical AC input pin 3. */
	AC_CH_NEG_MUX_PIN3       = AC_MUXNEG_PIN3_gc,
	/** Negative comparator input is connected to the internal ground plane. */
	AC_CH_NEG_MUX_GND        = AC_MUXNEG_GND_gc,
	/** Negative comparator input is connected to the channel's internal VCC
	 *  plane voltage scalar. */
	AC_CH_NEG_MUX_SCALED_VCC = AC_MUXNEG_VSCALE_gc,
	/** Negative comparator input is connected to the internal band gap voltage
	 *  reference. */
	AC_CH_NEG_MUX_BANDGAP    = AC_MUXNEG_BANDGAP_gc,
	/** Negative comparator input is connected to the channel's internal DAC
	 *  channel 0 output. */
	AC_CH_NEG_MUX_DAC0       = AC_MUXNEG_DAC_gc,
};

/** \brief AC channel output filtering configuration enum.
 *
 * Enum for the possible channel output filtering configurations of an Analog
 * Comparator channel.
 */
enum ac_ch_filter {
	/** No output filtering is performed on the comparator channel. */
	AC_CH_FILTER_NONE        = AC_FLEN_OFF_gc,
	/** Comparator channel output is passed through a Majority-of-Three
	 *  filter. */
	AC_CH_FILTER_MAJORITY_3  = AC_FLEN_MAJ3_gc,
	/** Comparator channel output is passed through a Majority-of-Five
	 *  filter. */
	AC_CH_FILTER_MAJORITY_5  = AC_FLEN_MAJ5_gc,
};

/** \brief AC channel GPIO output routing configuration enum.
 *
 * Enum for the possible channel GPIO output routing configurations of an Analog
 * Comparator channel.
 */
enum ac_ch_output {
	/** Comparator channel output is not routed to a physical GPIO pin, and is
	 *  used internally only. */
	AC_CH_OUTPUT_INTERNAL    = AC_OUT_OFF_gc,
	/** Comparator channel output is routed to it's matching physical GPIO pin,
	 *  via an asynchronous path. */
	AC_CH_OUTPUT_ASYNCRONOUS = AC_OUT_ASYNC_gc,
	/** Comparator channel output is routed to it's matching physical GPIO pin,
	 *  via a synchronous path. */
	AC_CH_OUTPUT_SYNCHRONOUS = AC_OUT_SYNC_gc,
};

/** \brief AC channel output state enum.
 *
 * Enum for the possible output states of an Analog Comparator channel.
 */
enum ac_ch_state {
	/** Unknown output state; the comparator channel was not ready. */
	AC_CH_STATE_UNKNOWN,
	/** Comparator's negative input pin is higher in voltage than the positive
	 *  input pin. */
	AC_CH_STATE_NEG_ABOVE_POS,
	/** Comparator's positive input pin is higher in voltage than the negative
	 *  input pin. */
	AC_CH_STATE_POS_ABOVE_NEG,
};

/** \brief AC window channel detection mode configuration enum.
 *
 * Enum for the possible detection modes of an Analog Comparator window channel.
 */
enum ac_win_detect {
	/** Window Comparator should detect an input above the upper threshold. */
	AC_WIN_DETECT_ABOVE,
	/** Window Comparator should detect an input between the lower and upper
	 *  thresholds. */
	AC_WIN_DETECT_INSIDE,
	/** Window Comparator should detect an input below the lower threshold. */
	AC_WIN_DETECT_BELOW,
	/** Window Comparator should detect an input above the upper threshold or
	 *  below the lower threshold. */
	AC_WIN_DETECT_OUTSIDE,
};

/** \brief AC window channel output state enum.
 *
 * Enum for the possible output states of an Analog Comparator window channel.
 */
enum ac_win_state {
	/** Unknown output state; the comparator window channel was not ready. */
	AC_WIN_STATE_UNKNOWN,
	/** Window Comparator's input voltage is above the upper window
	 *  threshold. */
	AC_WIN_STATE_ABOVE,
	/** Window Comparator's input voltage is between the lower and upper window
	 *  thresholds. */
	AC_WIN_STATE_INSIDE,
	/** Window Comparator's input voltage is below the lower window
	 *  threshold. */
	AC_WIN_STATE_BELOW,
};

/** \name Analog Comparator Event Masks
 *  \anchor ac_event_masks
 *
 * Masks for the various Analog Comparator module event input and outputs, which
 * can be controlled via \ref ac_enable_events() and \ref ac_disable_events().
 *
 * @{
 */
/** AC input event to trigger a conversion on comparator channel 0. */
#define AC_EVENT_COMPARATOR0_INPUT     AC_COMPEI0_bm
/** AC input event to trigger a conversion on comparator channel 1. */
#define AC_EVENT_COMPARATOR1_INPUT     AC_COMPEI1_bm
/** AC input event to trigger a conversion on comparator channel 2. */
#define AC_EVENT_COMPARATOR2_INPUT     AC_COMPEI2_bm
/** AC input event to trigger a conversion on comparator channel 3. */
#define AC_EVENT_COMPARATOR3_INPUT     AC_COMPEI3_bm
/** AC output event, triggered by comparator channel 0. */
#define AC_EVENT_COMPARATOR0_OUTPUT    AC_COMPEO0_bm
/** AC output event, triggered by comparator channel 1. */
#define AC_EVENT_COMPARATOR1_OUTPUT    AC_COMPEO1_bm
/** AC output event, triggered by comparator channel 2. */
#define AC_EVENT_COMPARATOR2_OUTPUT    AC_COMPEO2_bm
/** AC output event, triggered by comparator channel 3. */
#define AC_EVENT_COMPARATOR3_OUTPUT    AC_COMPEO3_bm
/** AC output event, triggered by comparator window channel 0. */
#define AC_EVENT_WINDOW0_OUTPUT        AC_WINEO0_bm
/** AC output event, triggered by comparator window channel 1. */
#define AC_EVENT_WINDOW1_OUTPUT        AC_WINEO1_bm
/** @} */

/** \brief AC device instance structure.
 *
 * AC software instance structure, used to retain software state information
 * of an associated hardware module instance.
 */
struct ac_dev_inst {
	/** Hardware module point of the associated Analog Comparator peripheral. */
	AC_t *hw_dev;
};

/** \brief Analog Comparator module configuration structure.
 *
 *  Configuration structure for a Comparator channel, to configure the input and
 *  output settings of the comparator.
 */
struct ac_conf {
	/** If \c true, the comparator pair will continue to sample during sleep
	 *  mode when triggered. */
	bool enable_pair_during_sleep[2];
	/** Events to enable in the module when configured. */
	uint8_t enabled_events;
};

/** \brief Analog Comparator module Comparator configuration structure.
 *
 *  Configuration structure for a Comparator channel, to configure the input and
 *  output settings of the comparator.
 */
struct ac_ch_conf {
	/** Sampling mode of the comparator channel. */
	enum ac_ch_sample_mode sample_mode;
	/** Filtering mode for the comparator output, when the comparator is used
	 *  in a supported mode. */
	enum ac_ch_filter filter;
	/** When \c true, hysteresis mode is enabled on the comparator inputs. */
	bool enable_hysteresis;
	/** Output mode of the comparator, whether it should be available for
	 *  internal use, or asynchronously/synchronously linked to a GPIO pin. */
	enum ac_ch_output output_mode;
	/** Input multiplexer selection for the comparator's positive input pin. */
	enum ac_ch_pos_mux positive;
	/** Input multiplexer selection for the comparator's negative input pin. */
	enum ac_ch_neg_mux negative;
	/** Scaled \f$\frac{VCC\times\mbox{n}}{64}\f$ VCC voltage division factor for the channel, when a comparator
	 *  pin is connected to the VCC voltage scalar input.
	 *
	 *  \note If the VCC voltage scalar is not selected as a comparator channel
	 *        pin's input, this value will be ignored. */
	uint8_t vcc_scale_factor;
};

/** \brief Analog Comparator module Window Comparator configuration structure.
 *
 *  Configuration structure for a Window Comparator channel, to configure the
 *  detection characteristics of the window.
 */
struct ac_win_conf {
	/** Window detection criteria that should be used to determine the state
	 *  of the window detection flag. */
	enum ac_win_detect window_detection;
};

/** \name Configuration and Initialization
 * @{
 */

void ac_init(
		struct ac_dev_inst *const dev_inst,
		AC_t *const module,
		struct ac_conf *const config);

/** \brief Initializes an Analog Comparator configuration structure to defaults.
 *
 *  Initializes a given Analog Comparator configuration structure to a set of
 *  known default values. This function should be called on all new instances
 *  of these configuration structures before being modified by the user
 *  application.
 *
 *  The default configuration is as follows:
 *   \li All comparator pairs disabled during sleep mode
 *   \li No events enabled by default
 *
 *  \param[out] config  Configuration structure to initialize to default values
 */
static inline void ac_get_config_defaults(
		struct ac_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->enable_pair_during_sleep[0] = false;
	config->enable_pair_during_sleep[1] = false;
	config->enabled_events              = 0;
}

void ac_enable(
		struct ac_dev_inst *const dev_inst);

void ac_disable(
		struct ac_dev_inst *const dev_inst);

void ac_enable_events(
		struct ac_dev_inst *const dev_inst,
		const uint8_t events);

void ac_disable_events(
		struct ac_dev_inst *const dev_inst,
		const uint8_t events);

/** @} */


/** \name Channel Configuration and Initialization
 * @{
 */

/** \brief Initializes an Analog Comparator channel configuration structure to defaults.
 *
 *  Initializes a given Analog Comparator channel configuration structure to a
 *  set of known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Continuous sampling mode
 *   \li Majority of 5 sample output filter
 *   \li Hysteresis enabled on the input pins
 *   \li Internal comparator output mode
 *   \li Comparator pin multiplexer 0 selected as the positive input
 *   \li Scaled VCC voltage selected as the negative input
 *   \li VCC voltage scaler set for a division factor of 2 (\f$\frac{VCC\times32}{64}\f$)
 *
 *   \param[out] config  Channel configuration structure to initialize to
 *                       default values
 */
static inline void ac_ch_get_config_defaults(
		struct ac_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->sample_mode       = AC_CH_MODE_CONTINUOUS;
	config->filter            = AC_CH_FILTER_MAJORITY_5;
	config->enable_hysteresis = true;
	config->output_mode       = AC_CH_OUTPUT_INTERNAL;
	config->positive          = AC_CH_POS_MUX_PIN0;
	config->negative          = AC_CH_NEG_MUX_SCALED_VCC;
	config->vcc_scale_factor  = 32;
}

void ac_ch_set_config(
		struct ac_dev_inst *const dev_inst,
		const uint8_t channel,
		struct ac_ch_conf *const config);

/** \brief Enables an Analog Comparator channel that was previously configured.
 *
 *  Enables and starts an Analog Comparator channel that was previously
 *  configured via a call to \ref ac_ch_set_config().
 *
 *  \param[in] dev_inst  Software instance for the Analog Comparator peripheral
 *  \param[in] channel   Comparator channel to enable
 */
static inline void ac_ch_enable(
		struct ac_dev_inst *const dev_inst,
		const uint8_t channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	AC_t *const ac_module = dev_inst->hw_dev;

	/* Write the new comparator module control configuration */
	ac_module->COMPCTRL[channel] |= AC_CH_ENABLE_bm;
}

/** \brief Disables an Analog Comparator channel that was previously enabled.
 *
 *  Stops an Analog Comparator channel that was previously started via a call to
 *  \ref ac_ch_enable().
 *
 *  \param[in] dev_inst  Software instance for the Analog Comparator peripheral
 *  \param[in] channel   Comparator channel channel to disable
 */
static inline void ac_ch_disable(
		struct ac_dev_inst *const dev_inst,
		const uint8_t channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	AC_t *const ac_module = dev_inst->hw_dev;

	/* Write the new comparator module control configuration */
	ac_module->COMPCTRL[channel] &= ~AC_CH_ENABLE_bm;
}

/** @} */


/** \name Channel Control
 * @{
 */

/** \brief Trigger a comparison on a comparator that is configured in single shot mode.
 *
 *  Triggers a single conversion on a comparator configured to compare on demand
 *  (one shot mode) rather than continuously.
 *
 *  \param[in] dev_inst  Software instance for the Analog Comparator peripheral
 *  \param[in] channel   Comparator channel channel to trigger
 */
static inline void ac_ch_trigger_single_shot(
		struct ac_dev_inst *const dev_inst,
		const uint8_t channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	AC_t *const ac_module = dev_inst->hw_dev;

	/* Write the new comparator module control configuration */
	ac_module->CTRLB |= (AC_START0_bm << channel);
}

/** \brief Determines if a given comparator channel is ready for comparisons.
 *
 *  Checks a comparator channel to see if the comparator is currently ready to
 *  begin comparisons.
 *
 *  \param[in] dev_inst  Software instance for the Analog Comparator peripheral
 *  \param[in] channel   Comparator channel channel to test
 *
 *  \return Comparator channel readiness state.
 */
static inline bool ac_ch_is_ready(
		struct ac_dev_inst *const dev_inst,
		const uint8_t channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	AC_t *const ac_module = dev_inst->hw_dev;

	return (ac_module->STATUSB & (AC_READY0_bm << channel));
}

/** \brief Determines the output state of a comparator channel.
 *
 *  Retrieves the last comparison value (after filtering) of a given comparator.
 *  If the comparator was not ready at the time of the check, the comparison
 *  result will be indicated as being unknown.
 *
 *  \param[in] dev_inst   Software instance for the Analog Comparator peripheral
 *  \param[in] channel    Comparator channel channel to test
 *
 *  \return Comparator channel state.
 */
static inline enum ac_ch_state ac_ch_get_state(
		struct ac_dev_inst *const dev_inst,
		const uint8_t channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	AC_t *const ac_module = dev_inst->hw_dev;

	if (ac_ch_is_ready(dev_inst, channel) == false) {
		return AC_CH_STATE_UNKNOWN;
	}

	if (ac_module->STATUSA & (AC_STATE0_bm << channel)) {
		return AC_CH_STATE_POS_ABOVE_NEG;
	} else {
		return AC_CH_STATE_NEG_ABOVE_POS;
	}
}

/** @} */


/** \name Window Mode Configuration and Initialization
 * @{
 */

/** \brief Initializes an Analog Comparator window channel configuration structure to defaults.
 *
 *  Initializes a given Analog Comparator window channel configuration structure
 *  to a set of known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Detect when the signal is inside the comparator pair window
 *
 *  \param[out] config  Window channel configuration structure to initialize to
 *                      default values
 */
static inline void ac_win_get_config_defaults(
		struct ac_win_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->window_detection = AC_WIN_DETECT_INSIDE;
}

void ac_win_set_config(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel,
		struct ac_win_conf *const config);

enum status_code ac_win_enable(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel);

void ac_win_disable(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel);

/** @} */


/** \name Window Mode Control
 * @{
 */

/** \brief Determines if a given Window Comparator is ready for comparisons.
 *
 *  Checks a Window Comparator to see if the both comparators used for window
 *  detection is currently ready to begin comparisons.
 *
 *  \param[in] dev_inst     Software instance for the Analog Comparator peripheral
 *  \param[in] win_channel  Window Comparator channel channel to test
 *
 *  \return Window Comparator channel readiness state.
 */
static inline bool ac_win_is_ready(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Check if the two comparators used in the window are ready */
	bool win_pair_comp0_ready = ac_ch_is_ready(dev_inst, (win_channel * 2));
	bool win_pair_comp1_ready = ac_ch_is_ready(dev_inst, (win_channel * 2) + 1);

	/* If one or both window comparators not ready, return failure */
	if ((win_pair_comp0_ready == false) || (win_pair_comp1_ready == false)) {
		return false;
	}

	return true;
}

enum ac_win_state ac_win_get_state(
		struct ac_dev_inst *const dev_inst,
		const uint8_t channel);

/** \brief Determines if a Window Comparator has detected the configured window criteria.
 *
 *  Tests if a Windows Comparator has detected that the input signal relative
 *  to the window bounds matches the detection criteria previously configured
 *  for the Window Comparator.
 *
 *  \param[in] dev_inst     Software instance for the Analog Comparator peripheral
 *  \param[in] win_channel  Comparator Window channel to test
 *
 *  \return State of the Window Comparator criteria detection flag.
 */
static inline bool ac_win_is_detected(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	AC_t *const ac_module = dev_inst->hw_dev;

	return (ac_module->INTFLAG & (AC_WIN0_bm << win_channel));
}

/** \brief Clears a Comparator Window condition criteria detection flag.
 *
 *  Clears the Analog Comparator window condition detection flag for a specified
 *  comparator channel.
 *
 *  \param[in] dev_inst     Software instance for the Analog Comparator peripheral
 *  \param[in] win_channel  Comparator Window channel to modify
 */
static inline void ac_win_clear_detected(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	AC_t *const ac_module = dev_inst->hw_dev;

	ac_module->INTFLAG = (AC_WIN0_bm << win_channel);
}

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page ac_extra_info Extra Information
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
 *		<td>AC</td>
 *		<td>Analog Comparator</td>
 *	</tr>
 *	<tr>
 *		<td>MUX</td>
 *		<td>Multiplexer</td>
 *	</tr>
 * </table>
 *
 * \section fixed_errata Erratas fixed by driver
 * No errata workarounds in driver.
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
 * \page ac_quickstart Quick Start Guides for the AC module
 *
 * This is the quick start guide list for the \ref sam0_ac_group module, with
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
 * \section ac_use_cases AC module use cases
 * - \subpage ac_basic_use_case
 */

#endif
