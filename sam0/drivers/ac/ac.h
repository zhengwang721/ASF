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
 * TODO->TODO2;
 * }
 * \enddot
 *
 * \section module_introduction Introduction
 *
 * \par Overview of the Analog Comparator
 * TODO
 *
 * \par Comparator Pairs
 * TODO
 *
 * \par Positive and Negative Input MUXs
 * TODO
 *
 * \par Window Comparators
 * TODO
 *
 * \par Output Filtering
 * TODO
 *
 * \par Input Hysteresis
 * TODO
 *
 * \par One Shot and Continuous Sampling Modes
 * TODO
 *
 * \par Input and Output Events
 * TODO
 *
 * \section module_dependencies Dependencies
 * The Analog Comparator driver has the following dependencies.
 *
 * \li \ref gclk_group "GCLK" (Generic Clock Management)
 *
 * \section special_considerations Special Considerations
 *
 * The number of comparator pairs (and, thus, window comparators) within a
 * single hardware instance of the Analog Comparator module is device-specific;
 * some devices will contain a single comparator pair, while others may have two
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

enum ac_ch_sample_mode {
	AC_CH_MODE_CONTINUOUS    = 0,
	AC_CH_MODE_SINGLE_SHOT   = AC_SINGLE_bm,
};

enum ac_ch_pos_mux {
	AC_CH_POS_MUX_PIN0       = AC_MUXPOS_PIN0_gc,
	AC_CH_POS_MUX_PIN1       = AC_MUXPOS_PIN1_gc,
	AC_CH_POS_MUX_PIN2       = AC_MUXPOS_PIN2_gc,
	AC_CH_POS_MUX_PIN3       = AC_MUXPOS_PIN3_gc,
};

enum ac_ch_neg_mux {
	AC_CH_NEG_MUX_PIN0       = AC_MUXNEG_PIN0_gc,
	AC_CH_NEG_MUX_PIN1       = AC_MUXNEG_PIN1_gc,
	AC_CH_NEG_MUX_PIN2       = AC_MUXNEG_PIN2_gc,
	AC_CH_NEG_MUX_PIN3       = AC_MUXNEG_PIN3_gc,
	AC_CH_NEG_MUX_GND        = AC_MUXNEG_GND_gc,
	AC_CH_NEG_MUX_SCALED_VCC = AC_MUXNEG_VSCALE_gc,
	AC_CH_NEG_MUX_BANDGAP    = AC_MUXNEG_BANDGAP_gc,
	AC_CH_NEG_MUX_DAC        = AC_MUXNEG_DAC_gc,
};

enum ac_ch_filter {
	AC_CH_FILTER_NONE        = AC_FLEN_OFF_gc,
	AC_CH_FILTER_MAJORITY_3  = AC_FLEN_MAJ3_gc,
	AC_CH_FILTER_MAJORITY_5  = AC_FLEN_MAJ5_gc,
};

enum ac_ch_output {
	AC_CH_OUTPUT_INTERNAL    = AC_OUT_OFF_gc,
	AC_CH_OUTPUT_ASYNCRONOUS = AC_OUT_ASYNC_gc,
	AC_CH_OUTPUT_SYNCHRONOUS = AC_OUT_SYNC_gc,
};

enum ac_ch_state {
	AC_CH_STATE_UNKNOWN,
	AC_CH_STATE_NEG_ABOVE_POS,
	AC_CH_STATE_POS_ABOVE_NEG,
};

enum ac_win_detect {
	AC_WIN_DETECT_ABOVE,
	AC_WIN_DETECT_INSIDE,
	AC_WIN_DETECT_BELOW,
	AC_WIN_DETECT_OUTSIDE,
};

enum ac_win_state {
	AC_WIN_STATE_UNKNOWN,
	AC_WIN_STATE_ABOVE,
	AC_WIN_STATE_INSIDE,
	AC_WIN_STATE_BELOW,
};

enum ac_events {
	AC_EVENT_COMPARATOR0_INPUT   = AC_COMPEI0_bm,
	AC_EVENT_COMPARATOR1_INPUT   = AC_COMPEI1_bm,
	AC_EVENT_COMPARATOR2_INPUT   = AC_COMPEI2_bm,
	AC_EVENT_COMPARATOR3_INPUT   = AC_COMPEI3_bm,
	AC_EVENT_COMPARATOR0_OUTPUT  = AC_COMPEO0_bm,
	AC_EVENT_COMPARATOR1_OUTPUT  = AC_COMPEO1_bm,
	AC_EVENT_COMPARATOR2_OUTPUT  = AC_COMPEO2_bm,
	AC_EVENT_COMPARATOR3_OUTPUT  = AC_COMPEO3_bm,
	AC_EVENT_WINDOW0_OUTPUT      = AC_WINEO0_bm,
	AC_EVENT_WINDOW1_OUTPUT      = AC_WINEO1_bm,
};

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
 *   \li All comparator pairs enabled during sleep mode
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
	config->enable_pair_during_sleep[0] = true;
	config->enable_pair_during_sleep[1] = true;
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
	bool win_pair_comp0_ready = ac_ch_is_ready(dev_inst, (win_channel / 2));
	bool win_pair_comp1_ready = ac_ch_is_ready(dev_inst, (win_channel / 2) + 1);

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
