/**
 * \file
 *
 * \brief SAMD20 I2C Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#include "i2c_master_async.h"

/**
 * \internal Read next data.
 *
 * Used by interrupt handler to get next data byte from slave.
 *
 * \param dev_inst Pointer to device instance structure.
 */
static void _i2c_master_async_read(struct i2c_master_dev_inst *const dev_inst)
{
	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Find index to save next value in buffer. */
	uint16_t buffer_index = dev_inst->buffer_length - dev_inst->buffer_remaining--;

	if (!dev_inst->buffer_remaining) {
		/* Send nack and stop command. */
		i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_CMD(3);	
	}

	/* Read byte from slave and put in buffer. */
	dev_inst->buffer[buffer_index] = i2c_module->DATA.reg;
}

/**
 * \internal Write next data.
 *
 * Used by interrupt handler to send next data byte to slave.
 *
 * \param dev_inst Pointer to device instance structure.
 */
static void _i2c_master_async_write(struct i2c_master_dev_inst *const dev_inst)
{
	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Check for ack from slave. */
	if (i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
	{
		/* Not acknowledged, send stop bit. */
		i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);

		/* Return bad data value. */
		if (dev_inst->buffer_remaining) {
			dev_inst->status = STATUS_ERR_OVERFLOW;
			return;
		}
	}

	/* Find index to get next byte in buffer. */
	uint16_t buffer_index = dev_inst->buffer_length -
			dev_inst->buffer_remaining--;

	/* Write byte from buffer to slave. */
	i2c_module->DATA.reg = dev_inst->buffer[buffer_index];
}

/**
 * \internal Act on slave address response.
 *
 * Check for errors concerning master->slave handshake.
 *
 * \param dev_inst Pointer to device instance structure.
 */
static void _i2c_master_async_address_response(
		struct i2c_master_dev_inst *const dev_inst)
{
	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Check for error. Ignore bus-error; workaround for busstate stuck in BUSY. */
	if (i2c_module->INTFLAG.reg & SERCOM_I2CM_INTFLAG_WIF)
	{
		/* Clear write interrupt flag. */
		i2c_module->INTENCLR.reg = SERCOM_I2CM_INTENCLR_WIEN;

		i2c_module->INTFLAG.reg = SERCOM_I2CM_INTENCLR_WIEN;

		/* Check arbitration. */
		if (i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_ARBLOST) {
			/* Return busy. */
			dev_inst->status = STATUS_ERR_PACKET_COLLISION;
		}
	} else if (i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_RXNACK) {
		/* Slave busy. Issue ack and stop command. */
		i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT |
				SERCOM_I2CM_CTRLB_CMD(3);

		/* Return bad address value. */
		dev_inst->status = STATUS_ERR_BAD_ADDRESS;
	}

	dev_inst->buffer_length = dev_inst->buffer_remaining;

	/* Check for status OK. */
	if (dev_inst->status == STATUS_IN_PROGRESS) {
		/* Call function based on transfer direction. */
		if (dev_inst->transfer_direction == 0) {
			_i2c_master_async_write(dev_inst);
		} else {
			_i2c_master_async_read(dev_inst);
		}
	}
}

/**
 * \brief Register callback for the specified callback type.
 *
 * When called, the given callback function will be associated with the
 * specified callback type.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]  callback      Pointer to the function desired for the specified
 *                       callback.
 * \param[in]  callback_type Specifies the callback type to register.
 */
void i2c_master_async_register_callback(
		struct i2c_master_dev_inst *const dev_inst,
		const i2c_master_callback_t callback,
		enum i2c_master_callback callback_type)
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

/**
 * \brief Unregister callback for the specified callback type.
 *
 * When called, the currently registered callback for the given callback type
 * will be removed.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Specifies the callback type to unregister.
 */
void i2c_master_async_unregister_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback callback_type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Register callback. */
	dev_inst->callbacks[callback_type] = 0;

	/* Set corresponding bit to set callback as initiated. */
	dev_inst->registered_callback &= ~(1 << callback_type);
}

/**
 * \brief Read data packet from slave asynchronous.
 *
 * Reads a data packet from the specified slave address on the I2C bus. This
 * is the non-blocking equivalent of \ref i2c_master_read_packet.
 *
 * \param[in,out] dev_inst  Pointer to device instance struct.
 * \param[in,out] packet    Pointer to I2C packet to transfer.
 *
 * \return          Status of starting asynchronously reading I2C packet.
 * \retval STATUS_OK If reading was started successfully.
 * \retval STATUS_ERR_BUSY If module is currently busy with transfer operation.
 */
enum status_code i2c_master_async_read_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Check if the I2C module is busy doing async operation. */
	if (dev_inst->buffer_remaining > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Save packet to device instance. */
	dev_inst->buffer = packet->data;
	dev_inst->buffer_remaining = packet->data_length;
	dev_inst->transfer_direction = 1;
	dev_inst->status = STATUS_IN_PROGRESS;

	/* Enable interrupts. */
	i2c_module->INTENSET.reg = SERCOM_I2CM_INTENSET_WIEN | SERCOM_I2CM_INTENSET_RIEN;

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module->ADDR.reg = (packet->address << 1) | _I2C_TRANSFER_READ;

	/* Set action to ack. */
	i2c_module->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

	return STATUS_OK;
}

/**
 * \brief Write data packet to slave asynchronous.
 *
 * Writes a data packet to the specified slave address on the I2C bus. This
 * is the non-blocking equivalent of \ref i2c_master_write_packet.
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out]     packet    Pointer to I2C packet to transfer.
 *
 * \return          Status of starting asynchronously writing I2C packet.
 * \retval STATUS_OK If writing was started successfully.
 * \retval STATUS_ERR_BUSY If module is currently busy with transfer operation.
 */
enum status_code i2c_master_async_write_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Check if the I2C module is busy doing async operation. */
	if (dev_inst->buffer_remaining > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Save packet to device instance. */
	dev_inst->buffer = packet->data;
	dev_inst->buffer_remaining = packet->data_length;
	dev_inst->transfer_direction = 0;
	dev_inst->status = STATUS_IN_PROGRESS;

	/* Enable interrupts. */
	i2c_module->INTENSET.reg = SERCOM_I2CM_INTENSET_WIEN | SERCOM_I2CM_INTENSET_RIEN;

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module->ADDR.reg = (packet->address << 1) | _I2C_TRANSFER_WRITE;

	return STATUS_OK;
}

/**
 * \internal Interrupt handler for I2C master
 *
 * \param[in] instance Sercom instance that triggered interrupt.
 */
void _i2c_master_async_callback_handler(uint8_t instance)
{
	/* Get device instance for callback handling. */
	struct i2c_master_dev_inst *dev_inst =
			(struct i2c_master_dev_inst*)_sercom_instances[instance];

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Combine callback registered and enabled masks. */
	uint8_t callback_mask = dev_inst->enabled_callback & dev_inst->registered_callback;

	/* Check if the module should response to address ack. */
	if (dev_inst->buffer_length <= 0 && dev_inst->buffer_remaining > 0) {
		/* Call function for address response. */
		_i2c_master_async_address_response(dev_inst);

	/* Check if buffer transfer is complete. */
	} else if (dev_inst->buffer_length > 0 && dev_inst->buffer_remaining <= 0) {
		/* Stop packet operation. */
		i2c_module->INTENCLR.reg = SERCOM_I2CM_INTENCLR_WIEN | SERCOM_I2CM_INTENCLR_RIEN;
		dev_inst->buffer_length = 0;
		dev_inst->status = STATUS_OK;

		/* Call appropriate callback if enabled and registered. */
		if ((callback_mask & (1 << I2C_MASTER_CALLBACK_READ_COMPLETE))
				&& (dev_inst->transfer_direction == 1)) {

			dev_inst->callbacks[I2C_MASTER_CALLBACK_READ_COMPLETE](dev_inst);

		} else if ((callback_mask & (1 << I2C_MASTER_CALLBACK_WRITE_COMPLETE))
				&& (dev_inst->transfer_direction == 0)) {

			dev_inst->callbacks[I2C_MASTER_CALLBACK_WRITE_COMPLETE](dev_inst);
		}

	/* Continue buffer write/read. */
	} else if (dev_inst->buffer_length > 0 && dev_inst->buffer_remaining > 0){
		/* Check that bus ownership is not lost. */
		if (!(i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE(2))) {
			dev_inst->status = STATUS_ERR_PACKET_COLLISION;

		/* Call function based on transfer direction. */
		} else if (dev_inst->transfer_direction <= 0) {
			_i2c_master_async_write(dev_inst);
		} else {
			_i2c_master_async_read(dev_inst);
		}
	}

	/* Check for error. */
	if (dev_inst->status != STATUS_IN_PROGRESS) {
		/* Stop packet operation. */
		i2c_module->INTENCLR.reg = SERCOM_I2CM_INTENCLR_WIEN | SERCOM_I2CM_INTENCLR_RIEN;
		dev_inst->buffer_length = 0;

		/* Send nack and stop command unless arbitration is lost. */
		if (dev_inst->status != STATUS_ERR_PACKET_COLLISION) {
			i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_CMD(3);
		}

		/* Call error callback if enabled and registered. */
		if (callback_mask & (1 << I2C_MASTER_CALLBACK_ERROR)) {

			dev_inst->callbacks[I2C_MASTER_CALLBACK_ERROR](dev_inst);

		}
	}
}
