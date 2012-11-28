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
#include <i2c_master_async.h>

static void _i2c_master_async_read(struct i2c_master_dev_inst *const dev_inst)
{

}

static void _i2c_master_async_write(struct i2c_master_dev_inst *const dev_inst)
{

}

static void _i2c_master_async_address_response(
		struct i2c_master_dev_inst *const dev_inst)
{
	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Check for error. */
	if (i2c_module->INTFLAGS & I2C_MASTER_WIF) {
		/* Clear write interrupt flag. */
		i2c_module->INTENCLR = I2C_MASTER_WIF;
		/* Check for busserror. */
		if (i2c_module->STATUS & (1 << I2C_MASTER_BUSSERROR_Pos)) {
			/* Return denied. */
			dev_inst->status = STATUS_ERR_DENIED;
			return;
			dev_inst->callbacks[I2C_MASTER_CALLBACK_ERROR](dev_inst);
		/* Check arbitration. */
		} else if (i2c_module->STATUS & (1 << I2C_MASTER_ARBLOST_Pos)) {
			/* Return busy. */
			dev_inst->status = STATUS_ERR_PACKET_COLLISION;
			return;
		}
	}

	/* Call next operation. */
	if (dev_inst->transfer_direction) {
		_i2c_master_async_read(dev_inst);
	} else {
		_i2c_master_async_write(dev_inst);
	}
}

void _i2c_master_async_callback_handler(uint8_t instance)
{
	/* Get device instance for callback handling. */
	struct i2c_master_dev_inst *dev_inst =
			(struct i2c_master_dev_inst*)_sercom_instances[instance];

	dev_inst->hw_dev = 0;

	_i2c_master_async_address_response(dev_inst);
}

void i2c_master_async_register_callback(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_master_callback_t callback,
		enum i2c_master_callback_type callback_type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(callback);

	/* Register callback. */
	dev_inst->callbacks[callback_type] = callback;

	/* Set corresponding bit to set callback as initiated. */
	dev_inst->registered_callback |= (1 << callback_type);
}

void i2c_master_async_unregister_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Register callback. */
	dev_inst->callbacks[callback_type] = 0;

	/* Set corresponding bit to set callback as initiated. */
	dev_inst->registered_callback &= ~(1 << callback_type);
}

void i2c_master_async_enable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Check if interrupts has previously been enabled in module. */
	if (dev_inst->enabled_callback == 0) {
		SERCOM_I2C_MASTER_t *i2c_module =
				&(dev_inst->hw_dev->I2C_MASTER);

		/* Enable interrupt in module. */
		i2c_module->INTENSET = I2C_MASTER_WIF_Msk | I2C_MASTER_RIF_Msk;
	}

	/* Mark callback as enabled. */
	dev_inst->enabled_callback = (1 << callback_type);
}

void i2c_master_async_disable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Mark callback as enabled. */
	dev_inst->enabled_callback &= ~(1 << callback_type);

	/* Check if interrupts should be disabled in module. */
	if (dev_inst->enabled_callback == 0) {
		SERCOM_I2C_MASTER_t *const i2c_module =
				&(dev_inst->hw_dev->I2C_MASTER);


		/* Enable interrupt in module. */
		i2c_module->INTENCLR = I2C_MASTER_WIF_Msk | I2C_MASTER_RIF_Msk;
	}
}

enum status_code i2c_master_async_read_packet_async(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	/* Check if the USART transmitter is busy */
	if (dev_inst->buffer_length > 0) {
		return STATUS_ERR_BUSY;
	}

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Save packet to device instance. */
	dev_inst->buffer = packet->data;
	dev_inst->buffer_length = packet->data_length;
	dev_inst->transfer_direction = 1;

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module->ADDR = packet->address << 1 | 0x01;

	return STATUS_OK;
}

enum status_code i2c_master_async_write_packet_async(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	return STATUS_OK;
}

enum status_code i2c_master_async_cancel_operation(
		struct i2c_master_dev_inst *const dev_inst)
{
	return STATUS_OK;
}

enum status_code i2c_master_async_is_transfer_done(
		struct i2c_master_dev_inst *const dev_inst)
{
	return STATUS_OK;
}