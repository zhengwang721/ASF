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
/*TODO: General stuff
 *       - Add asserts with comments to default-section of switch mode, keep the
 *       returns.
 *       - Use asserts with comments?
 *       - Move return to end of function for switch-case and add break
 *       - Doxygen - fix according to standard, \returns and such
 *       - Have enum status_code where applicable, void/bool elsewhere
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
 */
enum status_code tc_init(TC_t *const module,
		struct tc_dev_inst *const dev_inst,
		struct tc_config *const config)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(dev_inst);
	Assert(config);

	/* Associate the given device instance with the hardware module */
	dev_inst->hw_dev = module;

	/*Reset the TC module to ensure it is re-initialized correctly */
	tc_reset(dev_inst);

	/* Set the new configuration of the module based on the config struct
	 * and return with the status
	 */
	return tc_set_config(dev_inst, config);
}

//TODO: remove tc_set_config, do everything in init instead
enum status_code tc_set_config(struct tc_dev_inst *const dev_inst,
		struct tc_config *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(config);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_sync(tc_module);

	//TODO: move this to before resetting the module
	/* Check if the TC module is used by the previous TC module.
	 * If the previous TC module is in count32 mode, this TC module will
	 * be occupied
	 */
	if (tc_module->STATUS && TC_SLAVE_bm) {
		return STATUS_ERR_BUSY;
	}

	/* Disable TC module */
	tc_disable(dev_inst);

	/* Synchronize */
	tc_sync(tc_module);

	/* Set configuration to registers common for all 3 modes */
	tc_module->CTRLA |= config->mode | config->wavegen
			| config->sync_action | config->prescaler;
	tc_module->CTRLBSET = config->count_direction | config->oneshot;
	tc_module->CTRLC = config->waveform_invert_ch_mask
			| config->capture_enable_ch_mask;

	//TODO: add compare register cc2, cc3
	/* Switch for TC mode  */
	switch(dev_inst->mode) {
		case TC_MODE_COUNT8:
			tc_sync(tc_module);
			tc_module->TC_COUNT8.COUNT =
					config->count8_mode_values.count;

			tc_sync(tc_module);
			tc_module->TC_COUNT8.PER   =
					config->count8_mode_values.period;

			tc_sync(tc_module);
			tc_module->TC_COUNT8.CC0   =
					config->count8_mode_values.cc0;

			tc_sync(tc_module);
			tc_module->TC_COUNT8.CC1   =
					config->count8_mode_values.cc1;
			break;

		case TC_MODE_COUNT16:
			tc_sync(tc_module);
			tc_module->TC_COUNT16.COUNT =
					config->count16_mode_values.count;

			/* In normal and pwm mode for the timer, the cc1
			 * register is used to store the period
			 */
			tc_sync(tc_module);
			if(config->wavegen == TC_WAVEGEN_NORMAL_FREQ
					|| config->wavegen ==
					TC_WAVEGEN_NORMAL_PULSE_WIDTH_MODULATION) {
				tc_module->TC_COUNT16.CC1 =
						config->count16_mode_values.period;
			} else {
				tc_module->TC_COUNT16.CC1 =
						config->count16_mode_values.cc1;
			}

			tc_sync(tc_module);
			tc_module->TC_COUNT16.CC0 =
					config->count16_mode_values.cc0;
			break;

		//TODO: Check something about odd and even. Andreas knows what
		//something is.
		case TC_MODE_COUNT32:
			tc_sync(tc_module);
			tc_module->TC_COUNT32.COUNT =
					config->count32_mode_values.count;

			/* In normal and pwm mode for the timer, the cc1
			 * register is used as a period register
			 */
			tc_sync(tc_module);
			if(config->wavegen == TC_WAVEGEN_NORMAL_FREQ
					|| config->wavegen ==
					TC_WAVEGEN_NORMAL_PULSE_WIDTH_MODULATION) {
				tc_module->TC_COUNT32.CC1 =
						config->count32_mode_values.period;
			} else {
				tc_module->TC_COUNT32.CC1 =
						config->count32_mode_values.cc1;
			}

			tc_sync(tc_module);
			tc_module->TC_COUNT32.CC0 =
					config->count32_mode_values.cc0;
			break;

		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/**
 * \brief Set TC module count register
 *
 * Set count value of the TC module
 * This function can be used if to update value after init.
 *
 * \param dev_inst      pointer to the device struct
 * \param count         value to write to the count register
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
	tc_sync(tc_module);

	/* Write to based on the TC mode */
	switch(dev_inst->mode) {
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

	//TODO: set RREQ or RCONT and ADDR field before doing the sync test.

	/* Synchronize */
	tc_sync(tc_module);

	/* Read from based on the TC mode */
	switch(dev_inst->mode) {
		case TC_MODE_COUNT8:
			*count = tc_module->TC_COUNT8.COUNT;
			return STATUS_OK;

		case TC_MODE_COUNT16:
			*count = tc_module->TC_COUNT16.COUNT;
			return STATUS_OK;

		case TC_MODE_COUNT32:
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
//TODO: this should check for capture overflow error.
//TODO: must add cc2 and cc3 in 16 bit mode
//TODO: Require array for register groups like the cc-registers, this will make
//this function a lot smaller
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
	tc_sync(tc_module);//this is probably not enough may be necessary to set RREQ or RCONT and ADDR bits, before checking SYNCBUSY.

	/* Read out based on the TC mode */
	switch(dev_inst->mode) {
		case TC_MODE_COUNT8:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_CC0:
					*compare = tc_module->TC_COUNT8.CC0;
					return STATUS_OK;

				case TC_CC1:
					*compare = tc_module->TC_COUNT8.CC1;
					return STATUS_OK;

				case TC_CC2:
					*compare = tc_module->TC_COUNT8.CC2;
					return STATUS_OK;

				case TC_CC3:
					*compare = tc_module->TC_COUNT8.CC3;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

		case TC_MODE_COUNT16:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_CC0:
					*compare = tc_module->TC_COUNT16.CC0;
					return STATUS_OK;

				case TC_CC1:
					*compare = tc_module->TC_COUNT16.CC1;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

		case TC_MODE_COUNT32:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_CC0:
					*compare = tc_module->TC_COUNT32.CC0;
					return STATUS_OK;

				case TC_CC1:
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
//TODO: will have to be changed if arrays are used for ccn regs.
//TODO: must ad cc2 and cc3 in 16 bit mode.
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
	tc_sync(tc_module);

	/* Read out based on the TC mode */
	switch(dev_inst->mode) {
		case TC_MODE_COUNT8:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_CC0:
					tc_module->TC_COUNT8.CC0
						= (uint8_t) compare;
					return STATUS_OK;

				case TC_CC1:
					tc_module->TC_COUNT8.CC1
						= (uint8_t) compare;
					return STATUS_OK;

				case TC_CC2:
					tc_module->TC_COUNT8.CC2
						= (uint8_t) compare;
					return STATUS_OK;

				case TC_CC3:
					tc_module->TC_COUNT8.CC3
						= (uint8_t) compare;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

		case TC_MODE_COUNT16:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_CC0:
					tc_module->TC_COUNT16.CC0
						= (uint16_t) compare;
					return STATUS_OK;

				case TC_CC1:
					tc_module->TC_COUNT16.CC1
						= (uint16_t) compare;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

		case TC_MODE_COUNT32:
			/* Read out based on compare register */
			switch(comp_reg_index) {
				case TC_CC0:
					tc_module->TC_COUNT32.CC0 = compare;
					return STATUS_OK;

				case TC_CC1:
					tc_module->TC_COUNT32.CC1 = compare;
					return STATUS_OK;

				default:
					return STATUS_ERR_INVALID_ARG;
			}

	} /* Switch TC mode  */
}

