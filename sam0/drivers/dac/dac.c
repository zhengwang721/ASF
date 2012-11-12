/**
 * \file
 *
 * \brief SAM0+ Peripheral Digital to Analog Converter Driver
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
#include "dac.h"


/**
 * \internal Reset the DAC module
 *
 * \param[in] dev_inst Pointer to the DAC software instance struct
 */
static void _dac_reset(
		struct dac_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	DAC_t *const dac_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	while (dac_module->STATUS & DAC_SYNC_BUSY_bm);

	/* Software reset the module */
	dac_module->CTRLA |= DAC_SWRST_bm;

}

/**
 * \internal Writes a DAC configuration to the hardware module.
 *
 * Writes out a given configuration to the hardware module.
 * Used by \ref dac_init.
 *
 * \param[out] dev_inst Pointer to the DAC software instance struct
 * \param[in]  config   Pointer to the configuration struct
 *
 */
static void _dac_set_config(
		struct dac_dev_inst *const dev_inst,
		struct dac_conf *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(config);
	Assert(dev_inst->hw_dev);

	DAC_t *const dac_module = dev_inst->hw_dev;

	/* Set selected DAC output to be enabled when enabling the module */
	dev_inst->output = config->output;

	/* Set reference voltage */
	dac_module->CTRLB  |= config->reference;

	/* Left adjust data if configured */
	if (config->left_adjust) {
		dac_module->CTRLB |= DAC_LEFTADJ_bm;
	}

	/* Enable DAC in standby sleep mode if configured */
	if (config->standby_sleep_enable) {
		dac_module->CTRLB |= DAC_SLEEPEN_bm;
	}
}

/**
 * \brief Writes a DAC channel configuration to the hardware module.
 *
 * Writes out a given channel configuration to the hardware module.
 *
 * \note The \ref dac_dev_inst "DAC device instance structure" must be
 * initialized before calling this function.
 *
 * \param[in] dev_inst Pointer to the DAC software instance struct
 * \param[in] channel  Channel to configure
 * \param[in] config   Pointer to the configuration struct
 *
 */
void dac_ch_set_config(
		struct dac_dev_inst *const dev_inst,
		const enum dac_channel channel,
		struct dac_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(config);
	Assert(dev_inst->hw_dev);

	DAC_t *const dac_module = dev_inst->hw_dev;

	if (config->enable_start_on_event) {
		/* Enable start conversion event input */
		dac_module->EVCTRL |= DAC_STARTEI_bm;
	} else {
		/* Disable start conversion event input */
		dac_module->EVCTRL &= ~DAC_STARTEI_bm;
	}

	if (config->enable_empty_event) {
		/* Enable data buffer empty event out */
		dac_module->EVCTRL |= DAC_EMPTYEO_bm;
	} else {
		/* Disable data buffer empty event out */
		dac_module->EVCTRL &= ~DAC_EMPTYEO_bm;
	}
}

/**
 * \brief Initialize the DAC device struct
 *
 * Use this function to initialize the Digital to Analog Converter. Resets the
 * underlying hardware module and configures it.
 * \note The DAC channel must be configured separately.
 *
 * \param[out] dev_inst Pointer to the DAC software instance struct
 * \param[in] module    Pointer to the DAC module instance
 * \param[in] config    Pointer to the config struct, created by the user
 *                      application
 *
 */
void dac_init(
		struct dac_dev_inst *const dev_inst,
		DAC_t *const module,
		struct dac_conf *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(module);
	Assert(config);

	/* Initialize device instance */
	dev_inst->hw_dev = module;

	/* Reset the module */
	_dac_reset(dev_inst);

	/* Write configuration to module */
	_dac_set_config(dev_inst, config);
}

/**
 * \brief Enable the DAC module
 *
 * Enables the DAC interface and the selected output.
 *
 * \param[in] dev_inst Pointer to the DAC software instance struct
 *
 */
void dac_enable(
		struct dac_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	DAC_t *const dac_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	while (dac_module->STATUS & DAC_SYNC_BUSY_bm);

	/* Enable the module */
	dac_module->CTRLA |= DAC_ENABLE_bm;

	/* Enable selected output */
	dac_module->CTRLB |= dev_inst->output;
}

/**
 * \brief Enable a DAC channel
 *
 * Enables the selected DAC channel.
 *
 * \param[in] dev_inst Pointer to the DAC software instance struct
 * \param[in] channel  Channel to enable
 *
 */
void dac_ch_enable(
		struct dac_dev_inst *const dev_inst,
		enum dac_channel channel)
{
	/* No channel support yet*/
}

/**
 * \brief Disable the DAC module
 *
 * Disables the DAC interface and the output buffer.
 *
 * \param[in] dev_inst Pointer to the DAC software instance struct
 *
 */
void dac_disable(
		struct dac_dev_inst *const dev_inst)
{
	/* Sanity check arguments*/
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	DAC_t *const dac_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	while (dac_module->STATUS & DAC_SYNC_BUSY_bm);

	/* Disable DAC */
	dac_module->CTRLA &= ~DAC_ENABLE_bm;

	/* Disable output buffer */
	dac_disable_output_buffer(dev_inst);
}

/**
 * \brief Disable a DAC channel
 *
 * Disables the selected DAC channel.
 *
 * \param[in] dev_inst Pointer to the DAC software instance struct
 * \param[in] channel  Channel to disable
 *
 */
void dac_ch_disable(
		struct dac_dev_inst *const dev_inst,
		enum dac_channel channel)
{
	/* No channel support yet*/
}

/**
 * \brief Enable the output buffer
 *
 * Enables the output buffer and drives the DAC output to the VOUT pin.
 *
 * \param[in] dev_inst Pointer to the DAC software instance struct
 *
 */
void dac_enable_output_buffer(
		struct dac_dev_inst *const dev_inst)
{
	/*Sanity check arguments*/
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	DAC_t *const dac_module = dev_inst->hw_dev;

	/* Enable output buffer */
	dac_module->CTRLB |= DAC_OUTPUT_EXTERNAL;

}

/**
 * \brief Disable the output buffer
 *
 * Disables the output buffer.
 * \note Should be done when the output buffer is not needed, as it will draw
 * current even if the system is in sleep mode.
 *
 * \param[in] dev_inst Pointer to the DAC software instance struct
 *
 */
void dac_disable_output_buffer(
		struct dac_dev_inst *const dev_inst)
{
	/* Sanity check arguments*/
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	DAC_t *const dac_module = dev_inst->hw_dev;

	/* Disable output buffer */
	dac_module->CTRLB &= ~(DAC_OUTPUT_EXTERNAL);
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
 * \param[in] dev_inst Pointer to the DAC software device struct
 * \param[in] channel  DAC channel to write to
 * \param[in] data     Conversion data
 * \param[in] event_triggered Boolean value to determine whether the conversion
 *        should be triggered immediately or by an incoming event.
 */
void dac_write(
		struct dac_dev_inst *const dev_inst,
		enum dac_channel channel,
		const uint16_t data,
		bool event_triggered)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	DAC_t *const dac_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	while (dac_module->STATUS & DAC_SYNC_BUSY_bm);

	if (event_triggered) {
		/* Event triggered conversion, write DATABUF register */
		dac_module->DATABUF = data;
	} else {
		/* Manual triggered conversion, write DATA register */
		dac_module->DATA = data;
	}
}