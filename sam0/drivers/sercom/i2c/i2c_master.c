/**
 * \file
 *
 * \brief SAM0+ I2C Serial Peripheral Interface Driver
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

#include <i2c_master.h>

#define I2C_MASTER_SWRST_Pos 0
#define SERCOM_MODE_I2C 0
#define SERCOM_I2C_MASTER 0
#define SERCOM_I2C_MODE 0
#define I2C_MASTER_RUNINSTDBY_Pos 0
#define SERCOM_GCLK_ID 0

/**
 * \breif blabla
 * \param  dev_inst [description]
 * \param  config   [description]
 * \return          [description]
 */
enum status_code _i2c_master_set_config(
		struct i2c_master_dev_inst *const dev_inst,
		struct i2c_master_conf *const config)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(config);

	/* Temporary variables. */
	uint32_t tmp_config = 0;
	uint8_t tmp_baud;
	enum status_code tmp_status_code = STATUS_OK;

	SERCOM_I2C_MASTER_t i2c_module = dev_inst->hw_dev->I2C_MASTER;

	/* Save timeout on unknown bus state in device instance. */
	dev_inst->unkown_bus_state_timeout = config->unkown_bus_state_timeout;
	/* Save timeout on buffer write. */
	dev_inst->buffer_timeout = config->buffer_timeout;

	/* Check and set if module should run in standby. */
	if (config->run_in_standby) {
		tmp_config = (1 << I2C_MASTER_RUNINSTDBY_Pos);
	}

	/* Check and set start data hold timeout. */
	if (config->start_hold_time != I2C_MASTER_START_HOLD_TIME_DISABLED) {
		tmp_config |= config->start_hold_time;
	}

	/* Write config to register CTRLA. */
	i2c_module.CTRLA |= tmp_config;

	/* Set sercom gclk generator according to config. */
	tmp_status_code = sercom_set_gclk_generator(
		config->generator_source,
		config->run_in_standby,
		false);
	/* Return status code if not OK. */
	if (tmp_status_code != STATUS_OK) {
		return tmp_status_code;
	}

	/* Find and set baudrate. */
	tmp_baud = (uint8_t)(clock_gclk_ch_get_hz(SERCOM_GCLK_ID)
			 / (2*config->baud_rate)-5);
	i2c_module.BAUD = tmp_baud;

	return tmp_status_code;
}

/**
 * \brief blalbalba
 * \param  dev_inst [description]
 * \param  module   [description]
 * \param  config   [description]
 * \return          [description]
 */
enum status_code i2c_master_init(struct i2c_master_dev_inst *const dev_inst,
		SERCOM_t *const module,
		const struct i2c_master_conf *const config)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(module);
	Assert(config);

	uint8_t sercom_instance;

	/* Initialize device instance */
	dev_inst->hw_dev = module;

	SERCOM_I2C_MASTER_t i2c_module = module->I2C_MASTER;

	/* Check if module is enabled. */
	if(i2c_module.CTRLA & (1 << I2C_MASTER_ENABLE_Pos)) {
		return STATUS_ERR_DENIED;
	}

	/* Check if reset is in progress. */
	if (i2c_module.CTRLA & ( 1 << I2C_MASTER_SWRST_Pos)){
		return STATUS_ERR_BUSY;
	}

#ifdef I2C_MASTER_ASYNC
	/* Get sercom instance index. */
	sercom_instance = _sercom_get_sercom_instance(module);

	/* Save device instance in interrupt handler. */
	_sercom_set_handler(instance, (void*)(&_i2c_master_callback_handler));

	/* Save device instance. */
	_sercom_instances[sercom_instance] = (void) dev_inst;
#endif

	/* Set sercom module to operate in I2C master mode. */
	i2c_module.CTRLA = SERCOM_I2C_MODE |
			SERCOM_I2C_MASTER;

	/* Set config and return status. */
	return _i2c_master_set_config(dev_inst, config);
}

/**
 * \breif blablabla
 * \param dev_inst [description]
 */
void i2c_master_reset(struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t i2c_module = dev_inst->hw_dev->I2C_MASTER;

	/* Wait for sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Reset module. */
	i2c_module.CTRLA = ( 1 << I2C_MASTER_SWRST_Pos );
}

enum status_code i2c_master_read_packet(
		const struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	;
}

enum status_code i2c_master_write_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	;
}