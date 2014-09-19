/**
 * \file
 *
 * \brief SAM0 Operational Amplifier Controller (OPAMP) Driver
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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

#include "opamp.h"

void opamp_module_init(void)
{
	/* Turn on the digital interface clock. */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBD, MCLK_APBDMASK_OPAMP);

	/* Reset module. */
	opamp_module_reset();

	/* Enable module. */
	opamp_module_enable();
}

static inline void _opamp_get_config_common_defaults(
		struct opamp_config_common *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->potentiometer_selection = OPAMP_POT_MUX_8R_8R;
	config->r1_enable = false;
	config->r2_vcc = false;
	config->r2_out = false;
	config->on_demand = false;
	config->run_in_standby = false;
	config->bias_value = OPAMP_BIAS_MODE_0;
	config->analog_out = false;
}

void opamp0_get_config_defaults(struct opamp0_config *const config)
{
	/* Sanity check arguments */
	Assert(config);

	_opamp_get_config_common_defaults(&(config->config_common));

	/* Default configuration values */
	config->negative_input = OPAMP0_NEG_MUX_OUT0;
	config->positive_input = OPAMP0_POS_MUX_PIN0;
	config->r1_connection = OPAMP0_RES1_MUX_GND;
}

void opamp1_get_config_defaults(struct opamp1_config *const config)
{
	/* Sanity check arguments */
	Assert(config);

	_opamp_get_config_common_defaults(&(config->config_common));

	/* Default configuration values */
	config->negative_input = OPAMP1_NEG_MUX_OUT1;
	config->positive_input = OPAMP1_POS_MUX_PIN1;
	config->r1_connection = OPAMP1_RES1_MUX_GND;
}

void opamp2_get_config_defaults(struct opamp2_config *const config)
{
	/* Sanity check arguments */
	Assert(config);

	_opamp_get_config_common_defaults(&(config->config_common));

	/* Default configuration values */
	config->negative_input = OPAMP2_NEG_MUX_OUT2;
	config->positive_input = OPAMP2_POS_MUX_PIN2;
	config->r1_connection = OPAMP2_RES1_MUX_GND;
}

void opamp0_set_config(struct opamp0_config *const config)
{
	uint32_t temp = 0;

	if (config->config_common.r1_enable) {
		temp |= OPAMP_OPAMPCTRL0_RES1EN;
	}

	if (config->config_common.r2_vcc) {
		temp |= OPAMP_OPAMPCTRL0_RES2VCC;
	}

	if (config->config_common.r2_out) {
		temp |= OPAMP_OPAMPCTRL0_RES2OUT;
	}

	if (config->config_common.on_demand) {
		temp |= OPAMP_OPAMPCTRL0_ONDEMAND;
	}

	if (config->config_common.run_in_standby) {
		temp |= OPAMP_OPAMPCTRL0_RUNSTDBY;
	}

	if (config->config_common.analog_out) {
		temp |= OPAMP_OPAMPCTRL0_ANAOUT;
	}

	OPAMP->OPAMPCTRL0.reg = temp |
		config->config_common.potentiometer_selection |
		config->config_common.bias_value |
		config->negative_input |
		config->positive_input|
		config->r1_connection;
}

void opamp1_set_config(struct opamp1_config *const config)
{
	uint32_t temp = 0;

	if (config->config_common.r1_enable) {
		temp |= OPAMP_OPAMPCTRL1_RES1EN;
	}

	if (config->config_common.r2_vcc) {
		temp |= OPAMP_OPAMPCTRL1_RES2VCC;
	}

	if (config->config_common.r2_out) {
		temp |= OPAMP_OPAMPCTRL1_RES2OUT;
	}

	if (config->config_common.on_demand) {
		temp |= OPAMP_OPAMPCTRL1_ONDEMAND;
	}

	if (config->config_common.run_in_standby) {
		temp |= OPAMP_OPAMPCTRL1_RUNSTDBY;
	}

	if (config->config_common.analog_out) {
		temp |= OPAMP_OPAMPCTRL1_ANAOUT;
	}

	OPAMP->OPAMPCTRL1.reg = temp |
		config->config_common.potentiometer_selection |
		config->config_common.bias_value |
		config->negative_input |
		config->positive_input|
		config->r1_connection;
}

void opamp2_set_config(struct opamp2_config *const config)
{
	uint32_t temp = 0;

	if (config->config_common.r1_enable) {
		temp |= OPAMP_OPAMPCTRL2_RES1EN;
	}

	if (config->config_common.r2_vcc) {
		temp |= OPAMP_OPAMPCTRL2_RES2VCC;
	}

	if (config->config_common.r2_out) {
		temp |= OPAMP_OPAMPCTRL2_RES2OUT;
	}

	if (config->config_common.on_demand) {
		temp |= OPAMP_OPAMPCTRL2_ONDEMAND;
	}

	if (config->config_common.run_in_standby) {
		temp |= OPAMP_OPAMPCTRL2_RUNSTDBY;
	}

	if (config->config_common.analog_out) {
		temp |= OPAMP_OPAMPCTRL2_ANAOUT;
	}

	OPAMP->OPAMPCTRL2.reg = temp |
		config->config_common.potentiometer_selection |
		config->config_common.bias_value |
		config->negative_input |
		config->positive_input|
		config->r1_connection;
}

void opamp_enable(const enum opamp_number number)
{
	/* Sanity check arguments */
	Assert(number);

	/* Enable the OPAMP */
	if (number == OPAMP_0) {
		OPAMP->OPAMPCTRL0.reg |= OPAMP_OPAMPCTRL0_ENABLE;
	} else if (number == OPAMP_1) {
		OPAMP->OPAMPCTRL1.reg |= OPAMP_OPAMPCTRL1_ENABLE;
	} else if (number == OPAMP_2) {
		OPAMP->OPAMPCTRL2.reg |= OPAMP_OPAMPCTRL2_ENABLE;
	}
}

void opamp_disable(const enum opamp_number number)
{
	/* Sanity check arguments */
	Assert(number);

	/* Disable the OPAMP */
	if (number == OPAMP_0) {
		OPAMP->OPAMPCTRL0.reg &= ~OPAMP_OPAMPCTRL0_ENABLE;
	} else if (number == OPAMP_1) {
		OPAMP->OPAMPCTRL1.reg &= ~OPAMP_OPAMPCTRL1_ENABLE;
	} else if (number == OPAMP_2) {
		OPAMP->OPAMPCTRL2.reg &= ~OPAMP_OPAMPCTRL2_ENABLE;
	}
}

bool opamp_is_ready(const enum opamp_number number)
{
	/* Sanity check arguments */
	Assert(number);

	/* Get the OPAMP output ready status*/
	if (number == OPAMP_0) {
		return OPAMP->STATUS.bit.READY0;
	} else if (number == OPAMP_1) {
		return OPAMP->STATUS.bit.READY1;
	} else if (number == OPAMP_2) {
		return OPAMP->STATUS.bit.READY2;
	}

	return false;
}

