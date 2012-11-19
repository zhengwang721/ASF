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

/**
 * \brief Initializes the TC
 *
 * Enables the clock and initializes the TC module,
 * based on the values of the config struct
 *
 * \param dev_inst    pointer to the device struct
 * \param config pointer to the config struct
 *
 * \return enum status_code STATUS_ERR_BUSY    : When module is configured in 32 bit slave mode.
 *                                                The module will be left un althered in such a case.
 *                          STATUS_INVALID_ARG : When there is invalid data in the config struct.
 *                          STATUS_OK          : When init has compleeted sucsesfuly.
 */
enum status_code tc_init(TC_t *const tc_module,
			 struct tc_dev_inst *const dev_inst,
			 struct tc_config *const config)
{
	/* Sanity check arguments */
	Assert(tc_module);
	Assert(dev_inst);
	Assert(config);

	/* Associate the given device instance with the hardware module */
	dev_inst->hw_dev = tc_module;

	if (tc_module->STATUS && TC_SLAVE_bm) {
		return STATUS_ERR_BUSY;
	}
	/* Disable TC module */
	tc_disable(dev_inst);

	/*Reset the TC module to ensure it is re-initialized correctly */
	tc_reset(dev_inst);

	/* Synchronize */
	tc_wait_for_sync(tc_module);

	/* Set configuration to registers common for all 3 modes */
	tc_module->CTRLA |= config->resolution | config->wave_generation
			| config->reload_action | config->clock_prescaler;

	uint8_t ctrlbset_bm = 0;
	if(config->oneshot)
		ctrlbset_bm = TC_ONESHOT_ENABLED_bm;

	if(config->count_direction)
		ctrlbset_bm = ctrlbset_bm | TC_COUNT_DIRECTION_DOWN;

	if(ctrlbset_bm) {//check if we actualy need to go into a wait state.
		tc_wait_for_sync(tc_module);
		tc_module->CTRLBSET = ctrlbset_bm;
	}

	tc_wait_for_sync(tc_module);
	tc_module->CTRLC = config->waveform_invert_channel_mask
			| config->capture_enable_ch_mask;

	uint8_t evctrl_bm = 0;
	if(config->enable_event_input)
		evctrl_bm |= (1 << 5);//found no #define for this in the tc_header.h file.

	if(config->invert_event_input)
		evctrl_bm |= (1 << 4);//found no #define for this in the tc_header.h file.

	tc_wait_for_sync(tc_module);
	tc_module-EVCTRL = evctrl_bm | config->event_action
		        | config->event_generation_enable;

	/* Switch for TC mode  */
	switch(dev_inst->resolution) {
		case TC_MODE_COUNT8:
			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT8.COUNT =
				config->8bit_config.count;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT8.PER   =
				config->8bit_config.period;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT8.CC0   =
				config->8bit_config.capture_compare_channel_0;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT8.CC1   =
				config->8bit_config.capture_compare_channel_1;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT8.CC2   =
				config->8bit_config.capture_compare_channel_2;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT8.CC3   =
				config->8bit_config.capture_compare_channel_3;

			break;

		case TC_MODE_COUNT16:
			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT16.COUNT =
				config->16bit_config.count;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT16.CC0   =
				config->16bit_config.capture_compare_channel_0;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT16.CC1   =
				config->16bit_config.capture_compare_channel_1;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT16.CC2   =
				config->16bit_config.capture_compare_channel_2;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT16.CC3   =
				config->16bit_config.capture_compare_channel_3;

			break;

		case TC_MODE_COUNT32:
			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT32.COUNT =
				config->32bit_config.count;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT32.CC1 =
				config->32bit_config.capture_compare_channel_0;

			tc_wait_for_sync(tc_module);
			tc_module->TC_COUNT32.CC1 =
				config->32bit_config.capture_compare_channel_1;

			break;

		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/**
 * \brief Set TC module count value
 *
 * Set count value of the TC module
 * This function can be used to update value after init.
 * It can be used while the counter is running, there is no need
 * to disable the counter module.
 *
 * \param dev_inst      pointer to the device struct
 * \param count         value to write to the count register
 * \return enum status_code: STATUS_OK if
 */
enum status_code tc_set_count(
		struct tc_dev_inst *const dev_inst,
		uint32_t count)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(count);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_wait_for_sync(tc_module);

	/* Write to based on the TC mode */
	switch(dev_inst->resoultion) {
		case TC_MODE_COUNT8:
			tc_module->TC_COUNT8.COUNT = (uint8_t) count;
			return STATUS_OK;

		case TC_MODE_COUNT16:
			tc_module->TC_COUNT16.COUNT = (uint16_t) count;
			return STATUS_OK;

		case TC_MODE_COUNT32:
			tc_module->TC_COUNT32.COUNT = count;
			return STATUS_OK;

		default:
			return STATUS_ERR_INVALID_ARG;

	} /* Switch TC mode  */
}

/**
 * \brief Get TC module count register
 *
 * Get the count value of the TC module
 *
 * \param dev_inst      pointer to the device struct
 * \param count         pointer to the where the value is put
 */
enum status_code tc_get_count(
		struct tc_dev_inst *const dev_inst,
		uint32_t *count)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(count);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	//TODO: set RREQ or RCONT and ADDR field before doing the sync test. It may be that this is not nesesary in the polled driver.

	/* Synchronize */
	tc_wait_for_sync(tc_module);

	/* Read from based on the TC mode */
	switch(dev_inst->resolution) {
		case TC_RESOLUTION_8BIT:
			*count = tc_module->TC_COUNT8.COUNT;
			return STATUS_OK;

		case TC_RESOLUTION_16BIT:
			*count = tc_module->TC_COUNT16.COUNT;
			return STATUS_OK;

		case TC_RESOLUTION_32BIT:
			*count = tc_module->TC_COUNT32.COUNT;
			return STATUS_OK;

		default:
			return STATUS_ERR_INVALID_ARG;

	} /* Switch TC mode  */
}

/**
 * \brief Get value from TC compare/capture
 *
 * Get TC module compare/capture register with given index
 *
 * \param dev_inst      pointer to the device struct
 * \param value    pointer to store the value from the CC-register
 */
enum status_code tc_get_capture(
		struct tc_dev_inst *dev_inst,
		uint32_t *compare,
		enum tc_compare_reg_index comp_reg_index)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(compare);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_wait_for_sync(tc_module);//this is probably not enough may be necessary to set RREQ or RCONT and ADDR bits, before checking SYNCBUSY.

	/* Read out based on the TC mode */
	switch(dev_inst->mode) {
		case TC_MODE_COUNT8:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_COMPARE_CAPTURE_CHANNEL_0:
					*compare = tc_module->TC_COUNT8.CC0;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_1:
					*compare = tc_module->TC_COUNT8.CC1;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_2:
					*compare = tc_module->TC_COUNT8.CC2;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_3:
					*compare = tc_module->TC_COUNT8.CC3;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

		case TC_MODE_COUNT16:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_COMPARE_CAPTURE_CHANNEL_0:
					*compare = tc_module->TC_COUNT16.CC0;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_1:
					*compare = tc_module->TC_COUNT16.CC1;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_2:
					*compare = tc_module->TC_COUNT16.CC2;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_3:
					*compare = tc_module->TC_COUNT16.CC3;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

		case TC_MODE_COUNT32:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_COMPARE_CAPTURE_CHANNEL_0:
					*compare = tc_module->TC_COUNT32.CC0;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_1:
					*compare = tc_module->TC_COUNT32.CC1;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}
	} /* Switch TC mode  */
}

/**
 * \brief Set TC module compare/capture register 0
 *
 * Set TC module compare/capture register 0
 * This function can be used if to update value after init.
 *
 * \param dev_inst      pointer to the device struct
 * \param value    pointer to value to write to the CC0-register
 */
enum status_code tc_set_compare(
		struct tc_dev_inst *dev_inst,
		uint32_t compare,
		enum tc_compare_reg_index comp_reg_index)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(compare);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_wait_for_sync(tc_module);

	/* Read out based on the TC mode */
	switch(dev_inst->mode) {
		case TC_MODE_COUNT8:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_COMPARE_CAPTURE_CHANNEL_0:
					tc_module->TC_COUNT8.CC0
						= (uint8_t) compare;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_1:
					tc_module->TC_COUNT8.CC1
						= (uint8_t) compare;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_2:
					tc_module->TC_COUNT8.CC2
						= (uint8_t) compare;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_3:
					tc_module->TC_COUNT8.CC3
						= (uint8_t) compare;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

		case TC_MODE_COUNT16:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_COMPARE_CAPTURE_CHANNEL_0:
					tc_module->TC_COUNT16.CC0
						= (uint16_t) compare;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_1:
					tc_module->TC_COUNT16.CC1
						= (uint16_t) compare;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_2:
					tc_module->TC_COUNT16.CC2
						= (uint16_t) compare;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_3:
					tc_module->TC_COUNT16.CC3
						= (uint16_t) compare;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

		case TC_MODE_COUNT32:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_COMPARE_CAPTURE_CHANNEL_0:
					tc_module->TC_COUNT32.CC0 = compare;
					return STATUS_OK;

				case TC_COMPARE_CAPTURE_CHANNEL_1:
					tc_module->TC_COUNT32.CC1 = compare;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

	} /* Switch TC mode  */
}
