/**
 * \file
 *
 * \brief SAMD20 I2C Serial Peripheral Interface Driver
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#include "i2c_slave_async.h"

/**
 * \internal Set configurations to module.
 *
 * \param[out] dev_inst Pointer to device instance structure.
 * \param[in]  config Configuration structure with configurations to set.
 *
 * \return              Status of setting config.
 * \retval STATUS_OK                      Module was configured correctly
 * \retval STATUS_ERR_ALREADY_INITIALIZED If setting other gclk generator than
 *                                        previously set
 */
static enum status_code _i2c_slave_set_config(
		struct i2c_slave_dev_inst *const dev_inst,
		const struct i2c_slave_conf *const config)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(config);

	SercomI2cs *const i2c_module = &(dev_inst->hw_dev->I2CS);
	Sercom *const sercom_module = dev_inst->hw_dev;
	struct system_pinmux_conf pin_conf;
	uint32_t pad0 = config->pinmux_pad0;
	uint32_t pad1 = config->pinmux_pad1;
	
	system_pinmux_get_config_defaults(&pin_conf);
	/* SERCOM PAD0 - SDA */
	if (pad0 == PINMUX_DEFAULT) {
		pad0 = _sercom_get_default_pad(sercom_module, 0);
	}
	pin_conf.peripheral_index = pad0 & 0xFFFF;
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT_WITH_READBACK;
	system_pinmux_set_config(pad0 >> 16, &pin_conf);

	/* SERCOM PAD1 - SCL */
	if (pad1 == PINMUX_DEFAULT) {
		pad1 = _sercom_get_default_pad(sercom_module, 1);
	}
	pin_conf.peripheral_index = pad1 & 0xFFFF;
	system_pinmux_set_config(pad1 >> 16, &pin_conf);

	/* Write config to register CTRLA */
	i2c_module->CTRLA.reg |= config->sda_hold_time |
			(config->run_in_standby << SERCOM_I2CS_CTRLA_RUNSTDBY_Pos);

	/* Set CTRLB configuration */
	i2c_module->CTRLB.reg = SERCOM_I2CS_CTRLB_SMEN | config->address_mode;

	i2c_module->ADDR.reg = config->address << SERCOM_I2CS_ADDR_ADDR_Pos |
			config->address_mask << SERCOM_I2CS_ADDR_ADDRMASK_Pos |
			config->enable_general_call_address << SERCOM_I2CS_ADDR_GENCEN_Pos;

	return STATUS_OK;
}

/**
 * \brief Initializes the requested I2C Hardware module.
 *
 * Initializes the SERCOM I2C Slave device requested and sets the provided
 * device instance struct.  Run this function before any further use of
 * the driver.
 *
 * \param[out] dev_inst Pointer to device instance struct.
 * \param[in]  module   Pointer to the hardware instance.
 * \param[in]  config   Pointer to the configuration struct.
 *
 * \return              Status of initialization.
 * \retval STATUS_OK                       Module initiated correctly.
 * \retval STATUS_ERR_DENIED               If module is enabled.
 * \retval STATUS_ERR_BUSY                 If module is busy resetting.
 * \retval STATUS_ERR_ALREADY_INITIALIZED  If setting other gclk generator than
 *                                         previously set.
 *
 */
enum status_code i2c_slave_init(struct i2c_slave_dev_inst *const dev_inst,
		Sercom *const module,
		const struct i2c_slave_conf *const config)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(module);
	Assert(config);

	/* Initialize device instance */
	dev_inst->hw_dev = module;

	SercomI2cs *const i2c_module = &(dev_inst->hw_dev->I2CS);

	/* Check if module is enabled. */
	if (i2c_module->CTRLA.reg & SERCOM_I2CS_CTRLA_ENABLE) {
		return STATUS_ERR_DENIED;
	}

	/* Check if reset is in progress. */
	if (i2c_module->CTRLA.reg & SERCOM_I2CS_CTRLA_SWRST){
		return STATUS_ERR_BUSY;
	}
	
	/* Turn on module in PM */
	uint32_t pm_index = _sercom_get_sercom_inst_index(dev_inst->hw_dev)
			+ PM_APBCMASK_SERCOM0_Pos;
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);
	
	/* Set up GCLK */
	struct system_gclk_ch_conf gclk_ch_conf;
	system_gclk_ch_get_config_defaults(&gclk_ch_conf); 
	uint32_t gclk_index = _sercom_get_sercom_inst_index(dev_inst->hw_dev) + 13;
	gclk_ch_conf.source_generator = config->generator_source;
	system_gclk_ch_set_config(gclk_index, &gclk_ch_conf);
	system_gclk_ch_set_config(SERCOM_GCLK_ID, &gclk_ch_conf);
	system_gclk_ch_enable(gclk_index);
	system_gclk_ch_enable(SERCOM_GCLK_ID);

	/* Get sercom instance index. */
	uint8_t sercom_instance = _sercom_get_sercom_inst_index(module);

	/* Save device instance in interrupt handler. */
	_sercom_set_handler(sercom_instance,
			(void*)(&_i2c_slave_async_callback_handler));

	/* Save device instance. */
	_sercom_instances[sercom_instance] = (void*) dev_inst;

	/* Initialize values in dev_inst. */
	dev_inst->registered_callback = 0;
	dev_inst->enabled_callback = 0;
	dev_inst->buffer_length = 0;


	/* Set SERCOM module to operate in I2C slave mode. */
	i2c_module->CTRLA.reg = SERCOM_I2CS_CTRLA_MODE(2)
			& ~SERCOM_I2CS_CTRLA_MASTER;

	/* Set config and return status. */
	return _i2c_slave_set_config(dev_inst, config);
}

/**
 * \brief Resets the hardware module.
 *
 * This will reset the module to hardware defaults.
 *
 * \param[in,out] dev_inst Pointer to device instance structure.
 */
void i2c_slave_reset(struct i2c_slave_dev_inst *const dev_inst)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SercomI2cs *const i2c_module = &(dev_inst->hw_dev->I2CS);

	/* Wait for sync. */
	_i2c_slave_wait_for_sync(dev_inst);

	/* Disable module. */
	i2c_slave_disable(dev_inst);

	/* Wait for sync. */
	_i2c_slave_wait_for_sync(dev_inst);

	/* Reset module. */
	i2c_module->CTRLA.reg = SERCOM_I2CS_CTRLA_SWRST;
}

/**
 * \brief Enables sending NACK on address match
 *
 * This function will enable sending of NACK on address match, thus not
 * being ready for any transactions.
 *
 * \param[in,out] dev_inst Pointer to device instance structure
 */
void i2c_slave_async_enable_address_nack(struct i2c_slave_dev_inst
		*const dev_inst)
{
	dev_inst->nack_address = true;
}

/**
 * \brief Disables sending NACK on address match
 *
 * This function will disable sending of NACK on address match, thus
 * sending an ACK and initating a transaction.
 *
 * \param[in,out] dev_inst Pointer to device instance structure
 */
void i2c_slave_async_disable_address_nack(struct i2c_slave_dev_inst
		*const dev_inst)
{
	dev_inst->nack_address = false;
}

/**
 * \internal Read next data.
 *
 * Used by interrupt handler to get next data byte from master.
 *
 * \param dev_inst Pointer to device instance structure.
 */
static void _i2c_slave_async_read(struct i2c_slave_dev_inst *const dev_inst)
{
	SercomI2cs *const i2c_module = &(dev_inst->hw_dev->I2CS);

	/* Find index to save next value in buffer. */
	uint16_t buffer_index =
			dev_inst->buffer_length - dev_inst->buffer_remaining--;

	/* Read byte from master and put in buffer. */
	dev_inst->buffer[buffer_index] = i2c_module->DATA.reg;
}

/**
 * \internal Write next data.
 *
 * Used by interrupt handler to send next data byte to master.
 *
 * \param dev_inst Pointer to device instance structure.
 */
static void _i2c_slave_async_write(struct i2c_slave_dev_inst *const dev_inst)
{
	SercomI2cs *const i2c_module = &(dev_inst->hw_dev->I2CS);

	/* Check for NACK from master */
	if (i2c_module->STATUS.reg & SERCOM_I2CS_STATUS_RXNACK)
	{
		/* Not acknowledged, transmission stopped */
		/* Return bad data value. */
		dev_inst->status = STATUS_ERR_BAD_DATA;
		return;
	}

	/* Find index to get next byte in buffer. */
	uint16_t buffer_index = dev_inst->buffer_length -
			dev_inst->buffer_remaining--;

	/* Write byte from buffer to master */
	i2c_module->DATA.reg = dev_inst->buffer[buffer_index];
}

/**
 * \brief Register callback for the specified callback type.
 *
 * When called, the given callback function will be associated with the
 * specified callback type.
 *
 * \param[in,out] dev_inst  Pointer to the device instance struct.
 * \param[in] callback      Pointer to the function desired for the specified
 *                          callback.
 * \param[in] callback_type Specifies the callback type to register.
 */
void i2c_slave_async_register_callback(
		struct i2c_slave_dev_inst *const dev_inst,
		i2c_slave_callback_t callback,
		enum i2c_slave_callback callback_type)
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
void i2c_slave_async_unregister_callback(
		struct i2c_slave_dev_inst *const dev_inst,
		enum i2c_slave_callback callback_type)
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
 * \brief Read data packet from master asynchronous.
 *
 * Reads a data packet from the master. A write request must be initated by
 * the master before the packet can be read.
 * The I2C_SLAVE_CALLBACK_WRITE_REQUEST callback can be used to call this
 * function.
 *
 * \param[in,out] dev_inst  Pointer to device instance struct.
 * \param[in,out] packet    Pointer to I2C packet to transfer.
 *
 * \return          Status of starting asynchronously reading I2C packet.
 * \retval STATUS_OK If reading was started successfully.
 * \retval STATUS_ERR_BUSY If module is currently busy with transfer operation.
 */
enum status_code i2c_slave_async_read_packet(
		struct i2c_slave_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	/* Check if the I2C module is busy doing async operation. */
	if (dev_inst->buffer_remaining > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Save packet to device instance. */
	dev_inst->buffer = packet->data;
	dev_inst->buffer_remaining = packet->data_length;
	dev_inst->status = STATUS_IN_PROGRESS;

	/* Read will begin when master initiates the transfer */

	return STATUS_OK;
}

/**
 * \brief Write data packet to master  asynchronous.
 *
 * Writes a data packet to the master. A read request must be initated by
 * the master before the packet can be written.
 * The I2C_SLAVE_CALLBACK_READ_REQUEST callback can be used to call this
 * function.
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out]     packet    Pointer to I2C packet to transfer.
 *
 * \return          Status of starting asynchronously writing I2C packet.
 * \retval STATUS_OK If writing was started successfully.
 * \retval STATUS_ERR_BUSY If module is currently busy with transfer operation.
 */
enum status_code i2c_slave_async_write_packet(
		struct i2c_slave_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	/* Check if the I2C module is busy doing async operation. */
	if (dev_inst->buffer_remaining > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Save packet to device instance. */
	dev_inst->buffer = packet->data;
	dev_inst->buffer_remaining = packet->data_length;
	dev_inst->transfer_direction = 0;
	dev_inst->status = STATUS_IN_PROGRESS;

	return STATUS_OK;
}

/**
 * \internal Interrupt handler for I2C slave
 *
 * \param[in] instance Sercom instance that triggered interrupt.
 */
void _i2c_slave_async_callback_handler(uint8_t instance)
{
	/* Get device instance for callback handling. */
	struct i2c_slave_dev_inst *dev_inst =
			(struct i2c_slave_dev_inst*)_sercom_instances[instance];

	SercomI2cs *const i2c_module = &(dev_inst->hw_dev->I2CS);

	/* Combine callback registered and enabled masks. */
	uint8_t callback_mask = 
			dev_inst->enabled_callback & dev_inst->registered_callback;


	if (i2c_module->INTFLAG.reg & SERCOM_I2CS_INTFLAG_AIF) {
	/* Address match */

		if (i2c_module->STATUS.reg & (SERCOM_I2CS_STATUS_BUSERR ||
				SERCOM_I2CS_STATUS_COLL || SERCOM_I2CS_STATUS_LOWTOUT)) {
			/* An error occured in last packet transfer */
			dev_inst->status = STATUS_ERR_IO;
			if ((callback_mask & I2C_SLAVE_CALLBACK_ERROR_LAST_TRANSFER)) {
				dev_inst->callbacks[I2C_SLAVE_CALLBACK_ERROR_LAST_TRANSFER](dev_inst);
			}
		}
		if (dev_inst->nack_address) {
			/* NACK address */
			i2c_module->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
		}
		/* Set transfer direction in dev inst */
		if (i2c_module->STATUS.reg & SERCOM_I2CS_STATUS_DIR){
			/* Read request from master */
			dev_inst->transfer_direction = 1;
			if ((callback_mask & I2C_SLAVE_CALLBACK_READ_REQUEST)) {
				dev_inst->callbacks[I2C_SLAVE_CALLBACK_READ_REQUEST](dev_inst);
			}
			i2c_module->CTRLB.reg &= ~SERCOM_I2CS_CTRLB_ACKACT;
		} else {
			/* Write request from master */
			dev_inst->transfer_direction = 0;
			if ((callback_mask & I2C_SLAVE_CALLBACK_WRITE_REQUEST)) {
				dev_inst->callbacks[I2C_SLAVE_CALLBACK_WRITE_REQUEST](dev_inst);
			}
			i2c_module->CTRLB.reg &= ~SERCOM_I2CS_CTRLB_ACKACT;
		}
		/* ACK or NACK address */
		i2c_module->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x3);
		/* ACK next incoming packet */
		i2c_module->CTRLB.reg &= ~SERCOM_I2CS_CTRLB_ACKACT;
		
	} else if (i2c_module->INTFLAG.reg & SERCOM_I2CS_INTFLAG_PIF) {
		/* Stop condition on bus - current transfer done */

		dev_inst->buffer_length = 0;
		dev_inst->status = STATUS_OK;

		/* Call appropriate callback if enabled and registered. */
		if ((callback_mask & I2C_SLAVE_CALLBACK_READ_COMPLETE)
				&& (dev_inst->transfer_direction == 0)) {
			/* Read from master complete */
			dev_inst->callbacks[I2C_SLAVE_CALLBACK_READ_COMPLETE](dev_inst);
		} else if ((callback_mask & I2C_SLAVE_CALLBACK_WRITE_COMPLETE)
				&& (dev_inst->transfer_direction == 1)) {
			/* Write to master complete */
			dev_inst->callbacks[I2C_SLAVE_CALLBACK_WRITE_COMPLETE](dev_inst);
		}
	} else if (i2c_module->INTFLAG.reg & SERCOM_I2CS_INTFLAG_DIF){
		/* Check if buffer is full, or no more data to write */
		if (dev_inst->buffer_length > 0 && dev_inst->buffer_remaining <= 0) {
	
			dev_inst->buffer_length = 0;
			dev_inst->status = STATUS_OK;
			if (dev_inst->transfer_direction == 0) {
				/* Buffer is full, send NACK */
				i2c_module->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
				i2c_module->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x2);
				/* Set status, new character in DATA register will overflow
				buffer */
				dev_inst->status = STATUS_ERR_OVERFLOW;
				// callback error or callback complete?
				if (callback_mask & I2C_SLAVE_CALLBACK_READ_COMPLETE) {
					/* Read complete */
					dev_inst->callbacks[I2C_SLAVE_CALLBACK_READ_COMPLETE](dev_inst);
				}
			} else {
				/* Wait for new start condition */
				i2c_module->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
				i2c_module->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x2);

				if (callback_mask & I2C_SLAVE_CALLBACK_WRITE_COMPLETE) {
					/* No more data to write, write complete */
					dev_inst->callbacks[I2C_SLAVE_CALLBACK_WRITE_COMPLETE](dev_inst);
				}
			}

		/* Continue buffer write/read. */
		} else if (dev_inst->buffer_length > 0 && dev_inst->buffer_remaining > 0){
			/* Call function based on transfer direction. */
			if (dev_inst->transfer_direction == 0) {
				_i2c_slave_async_read(dev_inst);
			} else {
				_i2c_slave_async_write(dev_inst);
			}
		}
	}

	/* Check for error. */
	if (dev_inst->status != STATUS_IN_PROGRESS &&
			dev_inst->status != STATUS_OK) {
		/* Stop packet operation. */
		dev_inst->buffer_length = 0;

		/* Call error callback if enabled and registered */
		if ((dev_inst->registered_callback & I2C_SLAVE_CALLBACK_ERROR)
				&& (dev_inst->enabled_callback & I2C_SLAVE_CALLBACK_ERROR)) {

			dev_inst->callbacks[I2C_SLAVE_CALLBACK_ERROR](dev_inst);

		}
	}
}
