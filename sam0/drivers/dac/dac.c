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
#include "dac.h"
#include <system.h>
#include <pinmux.h>

/**
 * \internal Writes a DAC configuration to the hardware module.
 *
 * Writes out a given configuration to the hardware module.
 *
 * \param[out] module_inst  Pointer to the DAC software instance struct
 * \param[in]  config       Pointer to the configuration struct
 *
 */
static void _dac_set_config(
		struct dac_module *const module_inst,
		struct dac_conf *const config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(config);
	Assert(module_inst->hw_dev);

	Dac *const dac_module = module_inst->hw_dev;

	/* Configure GCLK channel and enable clock */
	struct system_gclk_chan_conf gclk_chan_conf;
	gclk_chan_conf.source_generator = config->clock_source;
	gclk_chan_conf.run_in_standby   = config->run_in_standby;
	system_gclk_chan_set_config(DAC_GCLK_ID, &gclk_chan_conf);
	system_gclk_chan_enable(DAC_GCLK_ID);

	/* Set selected DAC output to be enabled when enabling the module */
	module_inst->output = config->output;

	uint32_t new_config = 0;

	/* Set reference voltage */
	new_config     |= config->reference;

	/* Left adjust data if configured */
	if (config->left_adjust) {
		new_config |= DAC_CTRLB_LEFTADJ;
	}

	/* Enable DAC in standby sleep mode if configured */
	if (config->run_in_standby) {
		new_config |= DAC_CTRLA_RUNSTDBY;
	}

	/* Apply the new configuration to the hardware module */
	dac_module->CTRLA.reg = new_config;
}

/**
 * \brief Initialize the DAC device struct.
 *
 * Use this function to initialize the Digital to Analog Converter. Resets the
 * underlying hardware module and configures it.
 *
 * \note The DAC channel must be configured separately.
 *
 * \param[out] module_inst  Pointer to the DAC software instance struct
 * \param[in]  module       Pointer to the DAC module instance
 * \param[in]  config       Pointer to the config struct, created by the user
 *                          application
 *
 */
void dac_init(
		struct dac_module *const module_inst,
		Dac *const module,
		struct dac_conf *const config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module);
	Assert(config);

	/* Initialize device instance */
	module_inst->hw_dev = module;

	/* Turn on the digital interface clock */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, PM_APBCMASK_DAC);

	/* MUX the DAC VOUT pin */
	struct system_pinmux_conf pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);

	/* Set up the DAC VOUT pin */
	pin_conf.mux_position = MUX_PA00H_DAC_VOUT;
	pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pin_conf.input_pull   = SYSTEM_PINMUX_PIN_PULL_NONE;
	system_pinmux_pin_set_config(PIN_PA00H_DAC_VOUT, &pin_conf);

	/* Write configuration to module */
	_dac_set_config(module_inst, config);
}

/**
 * \brief Resets the DAC module.
 *
 * This function will reset the DAC module to its power on default values and
 * disable it.
 *
 * \param[in] module_inst  Pointer to the DAC software instance struct
 */
void dac_reset(
		struct dac_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw_dev);

	Dac *const dac_module = module_inst->hw_dev;

	while (dac_is_syncing(module_inst)) {
		/* Wait until the synchronization is complete */
	}

	/* Software reset the module */
	dac_module->CTRLA.reg |= DAC_CTRLA_SWRST;

}

/**
 * \brief Enable the DAC module.
 *
 * Enables the DAC interface and the selected output.
 *
 * \param[in] module_inst  Pointer to the DAC software instance struct
 *
 */
void dac_enable(
		struct dac_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw_dev);

	Dac *const dac_module = module_inst->hw_dev;

	while (dac_is_syncing(module_inst)) {
		/* Wait until the synchronization is complete */
	}

	/* Enable the module */
	dac_module->CTRLA.reg |= DAC_CTRLA_ENABLE;

	/* Enable selected output */
	dac_module->CTRLB.reg |= module_inst->output;
}

/**
 * \brief Disable the DAC module.
 *
 * Disables the DAC interface and the output buffer.
 *
 * \param[in] module_inst  Pointer to the DAC software instance struct
 *
 */
void dac_disable(
		struct dac_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw_dev);

	Dac *const dac_module = module_inst->hw_dev;

	/* Wait until the synchronization is complete */
	while (dac_module->STATUS.reg & DAC_STATUS_SYNCBUSY);

	/* Disable DAC */
	dac_module->CTRLA.reg &= ~DAC_CTRLA_ENABLE;
}

/**
 * \brief Writes a DAC channel configuration to the hardware module.
 *
 * Writes out a given channel configuration to the hardware module.
 *
 * \note The DAC device instance structure must be initialized before calling
 *       this function.
 *
 * \param[in] module_inst  Pointer to the DAC software instance struct
 * \param[in] channel      Channel to configure
 * \param[in] config       Pointer to the configuration struct
 *
 */
void dac_chan_set_config(
		struct dac_module *const module_inst,
		const enum dac_channel channel,
		struct dac_chan_conf *const config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(config);
	Assert(module_inst->hw_dev);

	/* No channel support yet */
	UNUSED(channel);

	Dac *const dac_module = module_inst->hw_dev;

	if (config->enable_start_on_event) {
		/* Enable start conversion event input */
		dac_module->EVCTRL.reg |=  DAC_EVCTRL_STARTEI;
	} else {
		/* Disable start conversion event input */
		dac_module->EVCTRL.reg &= ~DAC_EVCTRL_STARTEI;
	}

	if (config->enable_empty_event) {
		/* Enable data buffer empty event out */
		dac_module->EVCTRL.reg |=  DAC_EVCTRL_EMPTYEO;
	} else {
		/* Disable data buffer empty event out */
		dac_module->EVCTRL.reg &= ~DAC_EVCTRL_EMPTYEO;
	}
}

/**
 * \brief Enable a DAC channel.
 *
 * Enables the selected DAC channel.
 *
 * \param[in] module_inst  Pointer to the DAC software instance struct
 * \param[in] channel      Channel to enable
 *
 */
void dac_chan_enable(
		struct dac_module *const module_inst,
		enum dac_channel channel)
{
	/* No channel support yet */
	UNUSED(channel);
}

/**
 * \brief Disable a DAC channel.
 *
 * Disables the selected DAC channel.
 *
 * \param[in] module_inst  Pointer to the DAC software instance struct
 * \param[in] channel      Channel to disable
 *
 */
void dac_chan_disable(
		struct dac_module *const module_inst,
		enum dac_channel channel)
{
	/* No channel support yet */
	UNUSED(channel);
}

/**
 * \brief Enable the output buffer.
 *
 * Enables the output buffer and drives the DAC output to the VOUT pin.
 *
 * \param[in] module_inst  Pointer to the DAC software instance struct
 * \param[in] channel      DAC channel to alter
 */
void dac_chan_enable_output_buffer(
		struct dac_module *const module_inst,
		enum dac_channel channel)
{
	/*Sanity check arguments*/
	Assert(module_inst);
	Assert(module_inst->hw_dev);

	/* No channel support yet */
	UNUSED(channel);

	Dac *const dac_module = module_inst->hw_dev;

	/* Enable output buffer */
	dac_module->CTRLB.reg |= DAC_OUTPUT_EXTERNAL;
}

/**
 * \brief Disable the output buffer.
 *
 * Disables the output buffer.
 *
 * \note The output buffer(s) should be disabled when a channel's output is not
 *       currently needed, as it will draw current even if the system is in
 *       sleep mode.
 *
 * \param[in] module_inst  Pointer to the DAC software instance struct
 * \param[in] channel      DAC channel to alter
 */
void dac_chan_disable_output_buffer(
		struct dac_module *const module_inst,
		enum dac_channel channel)
{
	/* Sanity check arguments*/
	Assert(module_inst);
	Assert(module_inst->hw_dev);

	/* No channel support yet */
	UNUSED(channel);

	Dac *const dac_module = module_inst->hw_dev;

	/* Disable output buffer */
	dac_module->CTRLB.reg &= ~DAC_OUTPUT_EXTERNAL;
}

/**
 * \brief Write to the DAC.
 *
 * This function writes to the DATA or DATABUF register depending on whether
 * the conversion is event triggered or not.
 * If the conversion isn't event triggered, it will start right away.
 * If the conversion is event triggered, it will start when a Start Conversion
 * Event is issued.
 * Conversion data must be right or left adjusted according to configuration
 * settings.
 * \note To be event triggered, the enable_start_on_event must be
 * enabled in the configuration.
 *
 * \param[in] module_inst      Pointer to the DAC software device struct
 * \param[in] channel          DAC channel to write to
 * \param[in] data             Conversion data
 */
void dac_chan_write(
		struct dac_module *const module_inst,
		enum dac_channel channel,
		const uint16_t data)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw_dev);

	/* No channel support yet */
	UNUSED(channel);

	Dac *const dac_module = module_inst->hw_dev;

	/* Wait until the synchronization is complete */
	while (dac_module->STATUS.reg & DAC_STATUS_SYNCBUSY);

	/* Write the new value to the buffered DAC data register */
	dac_module->DATABUF.reg = data;
}
