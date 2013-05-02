/**
 * \file
 *
 * \brief SAM D20 Peripheral Digital to Analog Converter Driver
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
		struct dac_config *const config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(config);
	Assert(module_inst->hw_dev);

	Dac *const dac_module = module_inst->hw_dev;

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
 * \return Status of initialization
 * \retval STATUS_OK          Module initiated correctly
 * \retval STATUS_ERR_DENIED  If module is enabled
 * \retval STATUS_BUSY        If module is busy resetting
 */
enum status_code dac_init(
		struct dac_module *const module_inst,
		Dac *const module,
		struct dac_config *const config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module);
	Assert(config);

	/* Initialize device instance */
	module_inst->hw_dev = module;

	Dac *const dac_module = module_inst->hw_dev;

	/* Turn on the digital interface clock */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, PM_APBCMASK_DAC);

	/* Configure GCLK channel and enable clock */
	struct system_gclk_chan_config gclk_chan_conf;
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	gclk_chan_conf.source_generator = config->clock_source;
	system_gclk_chan_set_config(DAC_GCLK_ID, &gclk_chan_conf);
	system_gclk_chan_enable(DAC_GCLK_ID);

	/* Check if module is enabled. */
	if (dac_module->CTRLA.reg & DAC_CTRLA_ENABLE) {
		return STATUS_ERR_DENIED;
	}
	/* Check if reset is in progress. */
	if (dac_module->CTRLA.reg & DAC_CTRLA_SWRST) {
		return STATUS_BUSY;
	}

	/* MUX the DAC VOUT pin */
	struct system_pinmux_config pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);

	/* Set up the DAC VOUT pin */
	pin_conf.mux_position = MUX_PA02B_DAC_VOUT;
	pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pin_conf.input_pull   = SYSTEM_PINMUX_PIN_PULL_NONE;
	system_pinmux_pin_set_config(PIN_PA02B_DAC_VOUT, &pin_conf);

	/* Write configuration to module */
	_dac_set_config(module_inst, config);

	return STATUS_OK;
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
		struct dac_chan_config *const config)
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
 * This function writes to the DATABUF register.
 * If the conversion is not event-triggered, the data will be transferred
 * to the DATA register and the conversion will start.
 * If the conversion is event-triggered, the data will be transferred to the
 * DATA register when a Start Conversion Event is issued.
 * Conversion data must be right or left adjusted according to configuration
 * settings.
 * \note To be event triggered, the enable_start_on_event must be
 * enabled in the configuration.
 *
 * \param[in] module_inst      Pointer to the DAC software device struct
 * \param[in] channel          DAC channel to write to
 * \param[in] data             Conversion data
 *
 * \return Status of the operation
 * \retval STATUS_OK           If the data was written to DATABUF
 * \retval STATUS_BUSY         If the contents of DATABUF is not yet transferred
 *                             to DATA
 */
enum status_code dac_chan_write(
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

	if (!(dac_get_status(module_inst) & DAC_STATUS_CHANNEL_0_EMPTY)) {
		return STATUS_BUSY;
	}

	/* Wait until the synchronization is complete */
	while (dac_module->STATUS.reg & DAC_STATUS_SYNCBUSY);

	/* Write the new value to the buffered DAC data register */
	dac_module->DATABUF.reg = data;

	return STATUS_OK;
}

/**
 * \brief Retrieves the current module status
 *
 * Checks the status of the module and returns it as a bitmask of status
 * flags
 *
 * \param[in] module_inst      Pointer to the DAC software device struct
 * \param[in] channel          DAC channel to write to
 *
 * \return Bitmask of \ref dac_status flags
 *
 */
uint32_t dac_get_status(
		struct dac_module *const module_inst)
{
	 /* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	Dac *const dac_module = module_inst->hw_dev;

	uint8_t intflags = dac_module->INTFLAG.reg;
	uint32_t status_flags = 0;

	/* Check Data Buffer Empty flag */
	if (intflags & DAC_INTFLAG_EMPTY) {
		status_flags |= DAC_STATUS_CHANNEL_0_EMPTY;
	}
	/* Check Underrun flag */
	if (intflags & DAC_INTFLAG_UNDERRUN) {
		status_flags |= DAC_STATUS_CHANNEL_0_UNDERRUN;
	}

	return status_flags;
}

/**
 * \brief Clears a module status flag
 *
 * Clears the given status flag of the module.
 *
 * \param[in] module_inst      Pointer to the DAC software device struct
 * \param[in] status           Status flag to clear
 *
 * \return Status of the operation
 * \retval STATUS_OK              The status was cleared successfully
 * \retval STATUS_ERR_INVALID_ARG Invalid argument was provided
 *
 */
enum status_code dac_clear_status(
		struct dac_module *const module_inst,
		enum dac_status status)
{
	 /* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	Dac *const dac_module = module_inst->hw_dev;

	/* Clear requested status */
	switch (status) {
	case DAC_STATUS_CHANNEL_0_EMPTY:
		dac_module->INTENCLR.reg = DAC_INTFLAG_EMPTY;
		break;
	case DAC_STATUS_CHANNEL_0_UNDERRUN:
		dac_module->INTENCLR.reg = DAC_INTFLAG_UNDERRUN;
		break;
	default:
		return STATUS_ERR_INVALID_ARG;
	}
	return STATUS_OK;
}
