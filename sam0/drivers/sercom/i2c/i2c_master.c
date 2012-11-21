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

#include "i2c_master.h"

enum status_code i2c_master_init(struct i2c_master_dev_inst *const dev_inst,
		const SERCOM_t *const module,
		const struct i2c_master_conf *const config)
{

}

void i2c_master_enable(
		const struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check of arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *i2c_module = dev_inst->hw_dev;

	i2c_module->I2C_MASTER.CTRLA |= (1 << I2C_MASTER_ENABLE_bp);
}

enum status_code i2c_master_disable(
		const struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check of arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *i2c_module = dev_inst->hw_dev;

	i2c_module->I2C_MASTER.CTRLA &= ~(1 << I2C_MASTER_ENABLE_bp);
}

enum status_code i2c_master_register_callback(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_master_callback_t callback,
		enum i2c_master_callback_type)
{

}

enum status_code i2c_master_unregister_callback(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_master_callback_t callback,
		enum i2c_master_callback_type)
{

}

enum status_code i2c_master_enable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type)
{

}

enum status_code i2c_master_disable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type)
{

}

enum status_code i2c_master_read_buffer()
{

}

enum status_code i2c_master_write_buffer()
{

}