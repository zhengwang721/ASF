/**
 * \file
 *
 * \brief SAM0+ TC Driver
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

#include "tc.h"

/** \brief Initializes the TC
 *
 * Enables the clock and initializes the TC module,
 * based on the values of the config struct
 *
 * \param dev_inst  Pointer to the device struct
 * \param tc_module Pointer to the tc module
 * \param config    Pointer to the config struct
 *
 * \return Status of the procedure.
 * \retval STATUS_OK           The function exited sucsesfuly.
 * \retval STATUS_ERR_BUSY     When a reset has been initiated.
 * \retval STATUS_INVALID_ARG  When there is invalid data in the config struct.
 * \retval STATUS_ERR_DENIED   When module is enabled, and when module is
 *                             configured in 32 bit slave mode. Module will be
 *                             left unaltered in these cases.
 */
enum status_code tc_init(
		Tc *const tc_module,
		struct tc_dev_inst *const dev_inst,
		const struct tc_conf *const config)
{
	/* Sanity check arguments */
	Assert(tc_module);
	Assert(dev_inst);
	Assert(config);

	uint16_t ctrla_tmp = 0;
	uint8_t ctrlbset_tmp = 0;
	uint8_t evctrl_tmp = 0;
	uint8_t ctrlc_tmp = 0;

	/* Associate the given device instance with the hardware module */
	dev_inst->hw_dev = tc_module;

	/* make the counter size variable in the dev_inst struct reflect
	 * the counter size in the module
	 */
	dev_inst->counter_size = config->counter_size;

	if (tc_module->COUNT8.CTRLA.reg & TC_CTRLA_SWRST) {
		/* We are in the middle of a reset. Abort. */
		return STATUS_ERR_BUSY;
	}

	if (tc_module->COUNT8.STATUS.reg & TC_STATUS_SLAVE) {
		/* module is used as a slave*/
		return STATUS_ERR_DENIED;
	}

	if (tc_module->COUNT8.CTRLA.reg & TC_CTRLA_ENABLE) {
		/* Module must be disabled before initialization. Abort. */
		return STATUS_ERR_DENIED;
	}

	/* Setup clock for module */
	struct system_gclk_ch_conf gclk_ch_conf;
	gclk_ch_conf.source_generator  = config->clock_source;

	#if defined (REVB)
	/* Set the GCLK channel to run in standby mode */
	gclk_ch_conf.run_in_standby = config->run_in_standby;
	#else
	/* Set the GCLK channel sleep enable mode */
	gclk_ch_conf.enable_during_sleep = config->run_in_standby;
	#endif
	/* Apply configuration and enable the GCLK channel */
	system_gclk_ch_set_config(TC0_GCLK_ID, &gclk_ch_conf);
	system_gclk_ch_enable(TC0_GCLK_ID);

	if (config->run_in_standby) {
		ctrla_tmp |= TC_CTRLA_RUNSTDBY;
	}

	ctrla_tmp = config->counter_size | config->wave_generation
			| config->reload_action | config->clock_prescaler;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Set configuration to registers common for all 3 modes */
	tc_module->COUNT8.CTRLA.reg = ctrla_tmp;

	/* Set ctrlb register */
	if (config->oneshot) {
		ctrlbset_tmp = TC_CTRLBSET_ONESHOT;
	}

	if (config->count_direction) {
		ctrlbset_tmp |= TC_CTRLBSET_DIR;
	}

	if (ctrlbset_tmp) { /* check if we actually need to go into a wait
		             * state. */
		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT8.CTRLBSET.reg = ctrlbset_tmp;
	}

	ctrlc_tmp = config->waveform_invert_output
			| config->capture_enable;

	_tc_wait_for_sync(dev_inst);
	tc_module->COUNT8.CTRLC.reg = ctrlc_tmp;

	/* Set event register */
	if (config->enable_event_input) {
		evctrl_tmp |= TC_EVCTRL_TCEI;
	}

	if (config->invert_event_input) {
		evctrl_tmp |= TC_EVCTRL_TCINV;
	}

	_tc_wait_for_sync(dev_inst);
	tc_module->COUNT8.EVCTRL.reg = evctrl_tmp | config->event_action
			| config->event_generation_enable;

	/* Switch for TC counter size  */
	switch (dev_inst->counter_size) {
	case TC_COUNTER_SIZE_8BIT:
		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT8.COUNT.reg = config->tc_counter_size_conf. \
				tc_8bit_conf.count;

		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT8.PER.reg   = config->tc_counter_size_conf. \
				tc_8bit_conf.period;

		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT8.CC[0].reg
			= config->tc_counter_size_conf.tc_8bit_conf. \
				compare_capture_channel_0;

		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT8.CC[1].reg
			= config->tc_counter_size_conf.tc_8bit_conf. \
				compare_capture_channel_1;

		return STATUS_OK;

	case TC_COUNTER_SIZE_16BIT:
		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT16.COUNT.reg
			= config->tc_counter_size_conf.tc_16bit_conf.count;

		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT16.CC[0].reg
			= config->tc_counter_size_conf.tc_16bit_conf. \
				compare_capture_channel_0;

		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT16.CC[1].reg
			= config->tc_counter_size_conf.tc_16bit_conf. \
				compare_capture_channel_1;

		return STATUS_OK;

	case TC_COUNTER_SIZE_32BIT:
		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT32.COUNT.reg
			= config->tc_counter_size_conf.tc_32bit_conf.count;

		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT32.CC[0].reg
			= config->tc_counter_size_conf.tc_32bit_conf. \
				compare_capture_channel_0;

		_tc_wait_for_sync(dev_inst);
		tc_module->COUNT32.CC[1].reg
			= config->tc_counter_size_conf.tc_32bit_conf. \
				compare_capture_channel_1;

		return STATUS_OK;

	default:
		Assert(false);
		return STATUS_ERR_INVALID_ARG;
	}
	Assert(false); /* geting here should not be posible */
	return STATUS_ERR_PROTOCOL;
}

/** \brief Set TC module count value
 *
 * This function can be used to update count value after init. It can
 * be used while the counter is running, there is no need to disable
 * the counter module.
 *
 * \param[in] dev_inst      pointer to the device struct
 * \param[in] count         value to write to the count register
 *
 * \return Status of the procedure
 * \retval STATUS_OK              The procedure has gone well and the count
 *                                value has been set.
 * \retval STATUS_ERR_INVALID_ARG The counter size argument in the dev_inst
 *struct
 *                                is out of bounds.
 */
enum status_code tc_set_count_value(
		const struct tc_dev_inst *const dev_inst,
		uint32_t count)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(count);

	/* Get a pointer to the module's hardware instance*/
	Tc *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Write to based on the TC counter_size */
	switch (dev_inst->counter_size) {
	case TC_COUNTER_SIZE_8BIT:
		tc_module->COUNT8.COUNT.reg = (uint8_t)count;
		return STATUS_OK;

	case TC_COUNTER_SIZE_16BIT:
		tc_module->COUNT16.COUNT.reg = (uint16_t)count;
		return STATUS_OK;

	case TC_COUNTER_SIZE_32BIT:
		tc_module->COUNT32.COUNT.reg = count;
		return STATUS_OK;

	default:
		return STATUS_ERR_INVALID_ARG;
	} /* Switch TC counter size end  */
	Assert(false);
	return STATUS_ERR_PROTOCOL;
}

/** \brief Get TC module count value
 *
 * Get the count value of the TC module. It can be used while the
 * counter is running, there is no need to disable the counter module.
 *
 * \param[in]  dev_inst      pointer to the device struct
 * \param[out] count         pointer to the where the value is put
 *
 * \return Count value
 */
uint32_t tc_get_count_value(const struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(count);

	/* Get a pointer to the module's hardware instance*/
	Tc *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Read from based on the TC counter size */
	switch (dev_inst->counter_size) {
	case TC_COUNTER_SIZE_8BIT:
		return (uint32_t)tc_module->COUNT8.COUNT.reg;

	case TC_COUNTER_SIZE_16BIT:
		return (uint32_t)tc_module->COUNT16.COUNT.reg;

	case TC_COUNTER_SIZE_32BIT:
		return tc_module->COUNT32.COUNT.reg;

	default:
		Assert(false);
		return 0;
	} /* Switch TC counter size end  */
}

/** \brief Gets the capture value
 *
 * This procedure stores the capture value in the supplied buffer
 * pointed to by capture. This method can be used in 32, 16 and 8 bit
 * counter size. The user should perform a cast of the buffer after
 * the method has returned.
 *
 * \param[in]  dev_inst       Pointer to the device struct
 * \param[out] capture        Pointer to a buffer
 * \param[in]  channel_index  Index of the compare register to read from
 *
 * \return Capture value
 */
uint32_t tc_get_capture_value(
		const struct tc_dev_inst *const dev_inst,
		enum tc_compare_capture_channel_index channel_index)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance*/
	Tc *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Read out based on the TC counter size */
	switch (dev_inst->counter_size) {
	case TC_COUNTER_SIZE_8BIT:
		return (uint32_t)tc_module->COUNT8.CC[channel_index].reg;

	case TC_COUNTER_SIZE_16BIT:
		return (uint32_t)tc_module->COUNT16.CC[channel_index].reg;

	case TC_COUNTER_SIZE_32BIT:
		return tc_module->COUNT32.CC[channel_index].reg;
	}
	Assert(false);
	return 0;
}

/** \brief Sets a compare value
 *
 * Writes a compare value to the given compare/capture channel register
 *
 * \param[in] dev_inst       Pointer to the device struct
 * \param[in] compare        Pointer to the compare buffer
 * \param[in] channel_index  Index of the compare register to write to
 *
 * \return  Status of the procedure
 * \retval  STATUS_OK               The function exited sucsesfuly.
 * \retval  STATUS_ERR_INVALID_ARG  The channel index is out of range.
 */
enum status_code tc_set_compare_value(
		const struct tc_dev_inst *const dev_inst,
		uint32_t compare,
		enum tc_compare_capture_channel_index channel_index)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(compare);

	/* Get a pointer to the module's hardware instance*/
	Tc *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Read out based on the TC counter size */
	switch (dev_inst->counter_size) {
	case TC_COUNTER_SIZE_8BIT:
		if (channel_index < 2) {
			tc_module->COUNT8.CC[channel_index].reg
				= (uint8_t)compare;
			return STATUS_OK;
		} else {
			return STATUS_ERR_INVALID_ARG;
		}

	case TC_COUNTER_SIZE_16BIT:
		if (channel_index < 2) {
			tc_module->COUNT16.CC[channel_index].reg
				= (uint16_t)compare;
			return STATUS_OK;
		} else {
			return STATUS_ERR_INVALID_ARG;
		}

	case TC_COUNTER_SIZE_32BIT:
		if (channel_index < 2) {
			tc_module->COUNT16.CC[channel_index].reg
				= (uint32_t)compare;
			return STATUS_OK;
		} else {
			return STATUS_ERR_INVALID_ARG;
		}

	default:
		return STATUS_ERR_INVALID_ARG;
	} /* Switch TC counter size */
	Assert(false);
	return STATUS_ERR_PROTOCOL;
}
