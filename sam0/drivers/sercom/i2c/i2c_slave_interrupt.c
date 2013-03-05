/**
 * \file
 *
 * \brief SAMD20 I2C Serial Peripheral Interface Driver
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#include "i2c_slave_interrupt.h"

/**
 * \internal Sets configuration to module
 *
 * \param[out] module Pointer to software module structure
 * \param[in]  config Configuration structure with configurations to set
 *
 * \return                                Status of setting configuration
 * \retval STATUS_OK                      Module was configured correctly
 * \retval STATUS_ERR_ALREADY_INITIALIZED If setting other GCLK generator than
 *                                        previously set
 */
static enum status_code _i2c_slave_set_config(
		struct i2c_slave_module *const module,
		const struct i2c_slave_config *const config)
{
	/* Sanity check arguments. */
	Assert(module);
	Assert(module->hw);
	Assert(config);

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);
	Sercom *const sercom_hw = module->hw;

	struct system_pinmux_config pin_conf;
	uint32_t pad0 = config->pinmux_pad0;
	uint32_t pad1 = config->pinmux_pad1;

	system_pinmux_get_config_defaults(&pin_conf);
	/* SERCOM PAD0 - SDA */
	if (pad0 == PINMUX_DEFAULT) {
		pad0 = _sercom_get_default_pad(sercom_hw, 0);
	}

	pin_conf.mux_position = pad0 & 0xFFFF;
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT_WITH_READBACK;
	system_pinmux_pin_set_config(pad0 >> 16, &pin_conf);

	/* SERCOM PAD1 - SCL */
	if (pad1 == PINMUX_DEFAULT) {
		pad1 = _sercom_get_default_pad(sercom_hw, 1);
	}

	pin_conf.mux_position = pad1 & 0xFFFF;
	system_pinmux_pin_set_config(pad1 >> 16, &pin_conf);

	/* Write config to register CTRLA */
	i2c_hw->CTRLA.reg |= config->sda_hold_time |
			(config->run_in_standby << SERCOM_I2CS_CTRLA_RUNSTDBY_Pos);

	/* Set CTRLB configuration */
	i2c_hw->CTRLB.reg = SERCOM_I2CS_CTRLB_SMEN | config->address_mode;

	i2c_hw->ADDR.reg = config->address << SERCOM_I2CS_ADDR_ADDR_Pos |
			config->address_mask << SERCOM_I2CS_ADDR_ADDRMASK_Pos |
			config->enable_general_call_address << SERCOM_I2CS_ADDR_GENCEN_Pos;

	return STATUS_OK;
}

/**
 * \brief Initializes the requested I2C hardware module
 *
 * Initializes the SERCOM I2C Slave device requested and sets the provided
 * software module struct.  Run this function before any further use of
 * the driver.
 *
 * \param[out] module  Pointer to software module struct
 * \param[in]  module  Pointer to the hardware instance
 * \param[in]  config  Pointer to the configuration struct
 *
 * \return                                 Status of initialization
 * \retval STATUS_OK                       Module initiated correctly
 * \retval STATUS_ERR_DENIED               If module is enabled
 * \retval STATUS_BUSY                     If module is busy resetting
 * \retval STATUS_ERR_ALREADY_INITIALIZED  If setting other GCLK generator than
 *                                         previously set
 */
enum status_code i2c_slave_init(struct i2c_slave_module *const module,
		Sercom *const hw,
		const struct i2c_slave_config *const config)
{
	/* Sanity check arguments. */
	Assert(module);
	Assert(hw);
	Assert(config);

	/* Initialize software module */
	module->hw = hw;

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	/* Check if module is enabled. */
	if (i2c_hw->CTRLA.reg & SERCOM_I2CS_CTRLA_ENABLE) {
		return STATUS_ERR_DENIED;
	}

	/* Check if reset is in progress. */
	if (i2c_hw->CTRLA.reg & SERCOM_I2CS_CTRLA_SWRST) {
		return STATUS_BUSY;
	}

	/* Turn on module in PM */
	uint32_t pm_index = _sercom_get_sercom_inst_index(module->hw)
			+ PM_APBCMASK_SERCOM0_Pos;
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);

	/* Set up GCLK */
	struct system_gclk_chan_config gclk_chan_conf;
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	uint32_t gclk_index = _sercom_get_sercom_inst_index(module->hw) + 13;
	gclk_chan_conf.source_generator = config->generator_source;
	system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
	system_gclk_chan_set_config(SERCOM_GCLK_ID, &gclk_chan_conf);
	system_gclk_chan_enable(gclk_index);
	system_gclk_chan_enable(SERCOM_GCLK_ID);

	/* Get sercom instance index. */
	uint8_t instance_index = _sercom_get_sercom_inst_index(module->hw);

	/* Save software module in interrupt handler. */
	_sercom_set_handler(instance_index, _i2c_slave_interrupt_handler);

	/* Save software module. */
	_sercom_instances[instance_index] = module;

	/* Initialize values in module. */
	module->registered_callback = 0;
	module->enabled_callback = 0;
	module->buffer_length = 0;

	/* Set SERCOM module to operate in I2C slave mode. */
	i2c_hw->CTRLA.reg = SERCOM_I2CS_CTRLA_MODE(2)
			& ~SERCOM_I2CS_CTRLA_MASTER;

	/* Set config and return status. */
	return _i2c_slave_set_config(module, config);
}

/**
 * \brief Resets the hardware module
 *
 * This will reset the module to hardware defaults.
 *
 * \param[in,out] module Pointer to software module structure
 */
void i2c_slave_reset(struct i2c_slave_module *const module)
{
	/* Sanity check arguments. */
	Assert(module);
	Assert(module->hw);

	/* Reset module instance. */
	module->registered_callback = 0;
	module->enabled_callback = 0;
	module->buffer_length = 0;
	module->buffer_remaining = 0;
	module->buffer = NULL;

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	/* Disable module */
	i2c_slave_disable(module);

	/* Clear all pending interrupts. */
	system_interrupt_enter_critical_section();
	system_interrupt_clear_pending(_sercom_get_interrupt_vector(module->hw));
	system_interrupt_leave_critical_section();

	/* Wait for sync. */
	_i2c_slave_wait_for_sync(module);

	/* Reset module. */
	i2c_hw->CTRLA.reg = SERCOM_I2CS_CTRLA_SWRST;
}

/**
 * \brief Enables sending of NACK on address match
 *
 * This will enable sending of NACK on address match, thus discarding
 * any incoming transaction.
 *
 * \param[in,out] module Pointer to software module structure
 */
void i2c_slave_enable_nack_on_address(struct i2c_slave_module
		*const module)
{
	/* Sanity check arguments. */
	Assert(module);
	module->nack_on_address = true;
}

/**
 * \brief Disables sending NACK on address match
 *
 * This will disable sending of NACK on address match, thus
 * acknowledging incoming transactions.
 *
 * \param[in,out] module Pointer to software module structure
 */
void i2c_slave_disable_nack_on_address(struct i2c_slave_module
		*const module)
{
	/* Sanity check arguments. */
	Assert(module);
	module->nack_on_address = false;
}

/**
 * \internal Reads next data
 *
 * Used by interrupt handler to get next data byte from master.
 *
 * \param[in,out] module Pointer to software module structure
 */
static void _i2c_slave_read(struct i2c_slave_module *const module)
{
	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	/* Read byte from master and put in buffer. */
	*(module->buffer++) = i2c_hw->DATA.reg;

	/*Decrement remaining buffer length */
	module->buffer_remaining--;
}

/**
 * \internal Writes next data
 *
 * Used by interrupt handler to send next data byte to master.
 *
 * \param[in,out] module Pointer to software module structure
 */
static void _i2c_slave_write(struct i2c_slave_module *const module)
{
	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	/* Write byte from buffer to master */
	i2c_hw->DATA.reg = *(module->buffer++);

	/*Decrement remaining buffer length */
	module->buffer_remaining--;
}

/**
 * \brief Registers callback for the specified callback type
 *
 * This will associate the given callback function with the
 * specified callback type.
 * To enable the callback, the \ref i2c_slave_enable_callback function
 * must be used.
 *
 * \param[in,out] module    Pointer to the software module struct
 * \param[in] callback      Pointer to the function desired for the specified
 *                          callback
 * \param[in] callback_type Callback type to register
 */
void i2c_slave_register_callback(
		struct i2c_slave_module *const module,
		i2c_slave_callback_t callback,
		enum i2c_slave_callback callback_type)
{
	/* Sanity check. */
	Assert(module);
	Assert(module->hw);
	Assert(callback);

	/* Register callback. */
	module->callbacks[callback_type] = callback;

	/* Set corresponding bit to set callback as initiated. */
	module->registered_callback |= (1 << callback_type);
}

/**
 * \brief Unregisters callback for the specified callback type
 *
 * This will remove the currently registered callback for the given callback
 * type.
 *
 * \param[in,out]  module        Pointer to the software module struct
 * \param[in]      callback_type Callback type to unregister
 */
void i2c_slave_unregister_callback(
		struct i2c_slave_module *const module,
		enum i2c_slave_callback callback_type)
{
	/* Sanity check. */
	Assert(module);
	Assert(module->hw);

	/* Register callback. */
	module->callbacks[callback_type] = NULL;

	/* Set corresponding bit to set callback as initiated. */
	module->registered_callback &= ~(1 << callback_type);
}

/**
 * \brief Initiates a reads packet operation
 *
 * Reads a data packet from the master. A write request must be initiated by
 * the master before the packet can be read.
 * The I2C_SLAVE_CALLBACK_WRITE_REQUEST callback can be used to call this
 * function.
 *
 * \param[in,out] module    Pointer to software module struct
 * \param[in,out] packet    Pointer to I2C packet to transfer
 *
 * \return             Status of starting asynchronously reading I2C packet
 * \retval STATUS_OK   If reading was started successfully
 * \retval STATUS_BUSY If module is currently busy with another transfer
 */
enum status_code i2c_slave_read_packet_job(
		struct i2c_slave_module *const module,
		struct i2c_packet *const packet)
{
	/* Sanity check */
	Assert(module);
	Assert(module->hw);
	Assert(packet);

	/* Check if the I2C module is busy doing async operation. */
	if (module->buffer_remaining > 0) {
		return STATUS_BUSY;
	}

	/* Save packet to software module. */
	module->buffer = packet->data;
	module->buffer_remaining = packet->data_length;
	module->status = STATUS_BUSY;

	/* Read will begin when master initiates the transfer */
	return STATUS_OK;
}

/**
 * \brief Initiates a write packet operation
 *
 * Writes a data packet to the master. A read request must be initiated by
 * the master before the packet can be written.
 * The I2C_SLAVE_CALLBACK_READ_REQUEST callback can be used to call this
 * function.
 *
 * \param[in,out]     module  Pointer to software module struct
 * \param[in,out]     packet    Pointer to I2C packet to transfer
 *
 * \return             Status of starting writing I2C packet
 * \retval STATUS_OK   If writing was started successfully
 * \retval STATUS_BUSY If module is currently busy with another transfer
 */
enum status_code i2c_slave_write_packet_job(
		struct i2c_slave_module *const module,
		struct i2c_packet *const packet)
{
	/* Sanity check */
	Assert(module);
	Assert(module->hw);
	Assert(packet);

	/* Check if the I2C module is busy doing async operation. */
	if (module->buffer_remaining > 0) {
		return STATUS_BUSY;
	}

	/* Save packet to software module. */
	module->buffer = packet->data;
	module->buffer_remaining = packet->data_length;
	module->status = STATUS_BUSY;

	return STATUS_OK;
}

/**
 * \internal Interrupt handler for I2C slave
 *
 * \param[in] instance Sercom instance that triggered the interrupt
 */
void _i2c_slave_interrupt_handler(uint8_t instance)
{
	/* Get software module for callback handling. */
	struct i2c_slave_module *module =
			(struct i2c_slave_module*)_sercom_instances[instance];

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	/* Combine callback registered and enabled masks. */
	uint8_t callback_mask =
			module->enabled_callback & module->registered_callback;


	if (i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_AIF) {
	/* Address match */
		/* Check if last read is done - repeated start */
		if (module->buffer_length != module->buffer_remaining &&
				module->transfer_direction == 0) {
			
			module->status = STATUS_OK;
			module->buffer_length = 0;
			module->buffer_remaining = 0;

			if ((callback_mask & (1 << I2C_SLAVE_CALLBACK_READ_COMPLETE))) {
				module->callbacks[I2C_SLAVE_CALLBACK_READ_COMPLETE](module);
			}
		}

		if (i2c_hw->STATUS.reg & (SERCOM_I2CS_STATUS_BUSERR ||
				SERCOM_I2CS_STATUS_COLL || SERCOM_I2CS_STATUS_LOWTOUT)) {
			/* An error occurred in last packet transfer */
			module->status = STATUS_ERR_IO;
			if ((callback_mask & (1 << I2C_SLAVE_CALLBACK_ERROR_LAST_TRANSFER))) {
				module->callbacks[I2C_SLAVE_CALLBACK_ERROR_LAST_TRANSFER](module);
			}
		}
		if (module->nack_on_address) {
			/* NACK address */
			i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
		} else if (i2c_hw->STATUS.reg & SERCOM_I2CS_STATUS_DIR) {
			/* Set transfer direction in dev inst */
			module->transfer_direction = 1;
			/* Read request from master */
			if (callback_mask & (1 << I2C_SLAVE_CALLBACK_READ_REQUEST)) {
				module->callbacks[I2C_SLAVE_CALLBACK_READ_REQUEST](module);
			}

			/* Setting total length of buffer. */
			module->buffer_length = module->buffer_remaining;
			if (module->buffer_length == 0) {
				/* Data buffer not set up, NACK address */
				i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
			} else {
				/* ACK address */
				i2c_hw->CTRLB.reg &= ~SERCOM_I2CS_CTRLB_ACKACT;
			}
		} else {
			/* Set transfer direction in dev inst */
			module->transfer_direction = 0;
			/* Write request from master */
			if (callback_mask & (1 << I2C_SLAVE_CALLBACK_WRITE_REQUEST)) {
				module->callbacks[I2C_SLAVE_CALLBACK_WRITE_REQUEST](module);
			}

			/* Setting total length of buffer. */
			module->buffer_length = module->buffer_remaining;
			if (module->buffer_length == 0) {
				/* Data buffer not set up, NACK address */
				i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
			} else {
				/* ACK address */
				i2c_hw->CTRLB.reg &= ~SERCOM_I2CS_CTRLB_ACKACT;
			}
		}

		/* ACK or NACK address */
		i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x3);
		/* ACK next incoming packet */
		i2c_hw->CTRLB.reg &= ~SERCOM_I2CS_CTRLB_ACKACT;
	} else if (i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_PIF) {
		/* Stop condition on bus - current transfer done */
		module->status = STATUS_OK;
		module->buffer_length = 0;
		module->buffer_remaining = 0;

		/* Clear Stop interrupt. */
		i2c_hw->INTFLAG.reg |= SERCOM_I2CS_INTFLAG_PIF;

		/* Call appropriate callback if enabled and registered. */
		if ((callback_mask & (1 << I2C_SLAVE_CALLBACK_READ_COMPLETE))
				&& (module->transfer_direction == 0)) {
			/* Read from master complete */
			module->callbacks[I2C_SLAVE_CALLBACK_READ_COMPLETE](module);
		}
	} else if (i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_DIF) {
		/* Check if buffer is full, or NACK from master. */
		if (module->buffer_remaining <= 0 ||
				(module->transfer_direction == 1 && (module->buffer_length > module->buffer_remaining)
				&& (i2c_hw->STATUS.reg & SERCOM_I2CS_STATUS_RXNACK))) {

			module->buffer_remaining = 0;
			module->buffer_length = 0;

			if (module->transfer_direction == 0) {
				/* Buffer is full, send NACK */
				i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
				i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x2);
				/* Set status, new character in DATA register will overflow
				buffer */
				module->status = STATUS_ERR_OVERFLOW;

				if (callback_mask & (1 << I2C_SLAVE_CALLBACK_ERROR)) {
					/* Read complete */
					module->callbacks[I2C_SLAVE_CALLBACK_ERROR](module);
				}
			} else {
				/* Release SCL and wait for new start condition */
				i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
				i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x2);

				/* Transfer successful. */
				module->status = STATUS_OK;

				// if pif - twice?
				if (callback_mask & (1 << I2C_SLAVE_CALLBACK_WRITE_COMPLETE)) {
					/* No more data to write, write complete */
					module->callbacks[I2C_SLAVE_CALLBACK_WRITE_COMPLETE](module);
				}
			}

		/* Continue buffer write/read. */
		} else if (module->buffer_length > 0 && module->buffer_remaining > 0) {
			/* Call function based on transfer direction. */
			if (module->transfer_direction == 0) {
				_i2c_slave_read(module);
			} else {
				_i2c_slave_write(module);
			}
		}
	}
}
