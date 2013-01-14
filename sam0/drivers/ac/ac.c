/**
 * \file
 *
 * \brief SAMD20 Analog Comparator Driver
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
#include "ac.h"

static void _ac_set_config(
		struct ac_dev_inst *const dev_inst,
		struct ac_conf *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(config);

	Ac *const ac_module = dev_inst->hw_dev;

	/* Use a temporary register for computing the control bits */
	uint32_t ctrla_temp = 0;

	/* Check if the comparators should be enabled during sleep */
	if (config->run_in_standby == true) {
		ctrla_temp |= AC_CTRLA_RUNSTDBY;
	}

	/* Wait until the synchronization is complete */
	_ac_wait_for_sync(dev_inst);

	/* Write the new comparator module control configuration */
	ac_module->CTRLA.reg = ctrla_temp;

	/* Enable any requested user events */
	ac_enable_events(dev_inst, &config->events);
}

/** \brief Resets and disables the Analog Comparator driver.
 *
 *  Resets and disables the Analog Comparator driver, resetting the hardware
 *  module registers to their power-on defaults.
 *
 * \param[out] dev_inst  Pointer to the AC software instance struct
 */
void ac_reset(
		struct ac_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Ac *const ac_module = dev_inst->hw_dev;

	/* Disable the hardware module */
	ac_disable(dev_inst);

	/* Wait until the synchronization is complete */
	_ac_wait_for_sync(dev_inst);

	/* Software reset the module */
	ac_module->CTRLA.reg |= AC_CTRLA_SWRST;
}

/** \brief Initializes and configures the Analog Comparator driver.
 *
 *  Initializes the Analog Comparator driver, configuring it to the user
 *  supplied configuration parameters, ready for use. This function should be
 *  called before enabling the Analog Comparator.
 *
 *  \note Once called the Analog Comparator will not be running; to start the
 *        Analog Comparator call \ref ac_enable() after configuring the module.
 *
 * \param[out] dev_inst  Pointer to the AC software instance struct
 * \param[in]  module    Pointer to the AC module instance
 * \param[in]  config    Pointer to the config struct, created by the user
 *                       application
 */
void ac_init(
		struct ac_dev_inst *const dev_inst,
		Ac *const module,
		struct ac_conf *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(module);
	Assert(config);

	/* Initialize device instance */
	dev_inst->hw_dev = module;

	/* Write configuration to module */
	_ac_set_config(dev_inst, config);
}

/** \brief Writes an Analog Comparator channel configuration to the hardware module.
 *
 *  Writes a given Analog Comparator channel configuration to the hardware
 *  module.
 *
 *  \param[in] dev_inst  Software instance for the Analog Comparator peripheral
 *  \param[in] channel   Analog Comparator channel to configure
 *  \param[in] config    Pointer to the channel configuration struct
 */
void ac_ch_set_config(
		struct ac_dev_inst *const dev_inst,
		const uint8_t channel,
		struct ac_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(config);

	Ac *const ac_module = dev_inst->hw_dev;

	/* Use a temporary variable to compute the comparator configuration */
	uint32_t compctrl_temp = 0;

	/* Enable output filter mode */
	compctrl_temp |= config->filter;

	/* Enable output hysteresis if required */
	if (config->enable_hysteresis == true) {
		compctrl_temp |= AC_COMPCTRL_HYST;
	}

	/* Set output signal routing mode */
	compctrl_temp |= config->output_mode;

	/* Configure comparator positive and negative pin MUX configurations */
	compctrl_temp |= (config->positive_input | config->negative_input);

	/* Set sampling mode (one shot or continuous) */
	compctrl_temp |= config->sample_mode;

	/* Write the final configuration to the module's control register */
	_ac_wait_for_sync(dev_inst);
	ac_module->COMPCTRL[channel].reg = compctrl_temp;

	/* Configure VCC voltage scaling for the comparator */
	ac_module->SCALER[channel].reg = config->vcc_scale_factor;
}

/** \brief Writes an Analog Comparator Window channel configuration to the hardware module.
 *
 *  Writes a given Analog Comparator Window channel configuration to the hardware
 *  module.
 *
 *  \param[in] dev_inst     Software instance for the Analog Comparator peripheral
 *  \param[in] win_channel  Analog Comparator window channel to configure
 *  \param[in] config       Pointer to the window channel configuration struct
 */
void ac_win_set_config(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel,
		struct ac_win_conf *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(config);

	Ac *const ac_module = dev_inst->hw_dev;

	_ac_wait_for_sync(dev_inst);

	uint32_t win_ctrl_mask = 0;

	switch (config->window_detection)
	{
		case AC_WIN_DETECT_ABOVE:
			win_ctrl_mask =
					AC_WINCTRL_WINTSEL0_ABOVE >> AC_WINCTRL_WINTSEL0_Pos;
			break;
		case AC_WIN_DETECT_BELOW:
			win_ctrl_mask =
					AC_WINCTRL_WINTSEL0_BELOW >> AC_WINCTRL_WINTSEL0_Pos;
			break;
		case AC_WIN_DETECT_INSIDE:
			win_ctrl_mask =
					AC_WINCTRL_WINTSEL0_INSIDE >> AC_WINCTRL_WINTSEL0_Pos;
			break;
		case AC_WIN_DETECT_OUTSIDE:
			win_ctrl_mask =
					AC_WINCTRL_WINTSEL0_OUTSIDE >> AC_WINCTRL_WINTSEL0_Pos;
			break;
		default:
			break;
	}

	if (win_channel == 0) {
		ac_module->WINCTRL.reg =
				(ac_module->WINCTRL.reg & ~AC_WINCTRL_WINTSEL0_Msk) |
				(win_ctrl_mask << AC_WINCTRL_WINTSEL0_Pos);
	} else {
		ac_module->WINCTRL.reg =
				(ac_module->WINCTRL.reg & ~AC_WINCTRL_WINTSEL1_Msk) |
				(win_ctrl_mask << AC_WINCTRL_WINTSEL1_Pos);
	}
}


/** \brief Enables an Analog Comparator window channel that was previously configured.
 *
 *  Enables and starts an Analog Comparator window channel that was previously
 *  configured via a call to \ref ac_win_set_config().
 *
 *  \note The comparator channels used by the window channel must be configured
 *        and enabled before calling this function. The two comparator channels
 *        forming each window comparator pair must have identical configurations
 *        other than the negative pin multiplexer setting.
 *
 *  \param[in] dev_inst     Software instance for the Analog Comparator peripheral
 *  \param[in] win_channel  Comparator window channel to enable
 *
 *  \return Status of the window enable procedure.
 *
 *  \retval STATUS_OK              The window comparator was enabled
 *  \retval STATUS_ERR_IO          One or both comparators in the window
 *                                 comparator pair is disabled
 *  \retval STATUS_ERR_BAD_FORMAT  The comparator channels in the window pair
 *                                 were not configured correctly
 */
enum status_code ac_win_enable(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Ac *const ac_module = dev_inst->hw_dev;

	/* Load the configurations of the two comparators used in the window */
	uint32_t win_pair_comp0_conf = ac_module->COMPCTRL[win_channel * 2].reg;
	uint32_t win_pair_comp1_conf = ac_module->COMPCTRL[win_channel * 2 + 1].reg;

	/* Make sure both comparators in the window comparator pair are enabled */
	if (!(win_pair_comp0_conf & AC_COMPCTRL_ENABLE) ||
			!(win_pair_comp1_conf & AC_COMPCTRL_ENABLE)) {
		return STATUS_ERR_IO;
	}

	/* Make sure the comparators are configured in the same way, other than the
	 * negative pin multiplexers */
	if ((win_pair_comp0_conf & ~AC_COMPCTRL_MUXNEG_Msk) !=
			(win_pair_comp1_conf & ~AC_COMPCTRL_MUXNEG_Msk)) {
		return STATUS_ERR_BAD_FORMAT;
	}

	_ac_wait_for_sync(dev_inst);

	/* Enable the requested window comparator */
	if (win_channel == 0) {
		ac_module->WINCTRL.reg |= AC_WINCTRL_WEN0;
	} else {
		ac_module->WINCTRL.reg |= AC_WINCTRL_WEN1;
	}

	return STATUS_OK;
}

/** \brief Disables an Analog Comparator window channel that was previously enabled.
 *
 *  Stops an Analog Comparator window channel that was previously started via a
 *  call to \ref ac_win_enable().
 *
 *  \param[in] dev_inst     Software instance for the Analog Comparator peripheral
 *  \param[in] win_channel  Comparator window channel to disable
 */
void ac_win_disable(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Ac *const ac_module = dev_inst->hw_dev;

	_ac_wait_for_sync(dev_inst);

	/* Disable the requested window comparator */
	if (win_channel == 0) {
		ac_module->WINCTRL.reg &= ~AC_WINCTRL_WEN0;
	} else {
		ac_module->WINCTRL.reg &= ~AC_WINCTRL_WEN1;
	}
}

/** \brief Determines the state of a specified Window Comparator.
 *
 *  Retrieves the current window detection state, indicating what the input
 *  signal is currently comparing to relative to the window boundaries.
 *
 *  \param[in] dev_inst     Software instance for the Analog Comparator peripheral
 *  \param[in] win_channel  Comparator Window channel to test
 *
 *  \return Current window comparison state.
 */
enum ac_win_state ac_win_get_state(
		struct ac_dev_inst *const dev_inst,
		const uint8_t win_channel)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	Ac *const ac_module = dev_inst->hw_dev;

	/* If one or both window comparators not ready, return unknown result */
	if (ac_win_is_ready(dev_inst, win_channel) == false) {
		return AC_WIN_STATE_UNKNOWN;
	}

	uint32_t win_state = 0;

	/* Extract window comparison state bits */
	if (win_channel == 0) {
		win_state = ac_module->STATUSA.bit.WSTATE0;
	} else {
		win_state = ac_module->STATUSA.bit.WSTATE1;
	}

	/* Map hardware comparison states to logical window states */
	switch (win_state)
	{
		case (AC_STATUSA_WSTATE0_ABOVE >> AC_STATUSA_WSTATE0_Pos):
			return AC_WIN_STATE_ABOVE;
		case (AC_STATUSA_WSTATE0_BELOW >> AC_STATUSA_WSTATE0_Pos):
			return AC_WIN_STATE_BELOW;
		case (AC_STATUSA_WSTATE0_INSIDE >> AC_STATUSA_WSTATE0_Pos):
			return AC_WIN_STATE_INSIDE;
		default:
			return AC_WIN_STATE_UNKNOWN;
	}
}
