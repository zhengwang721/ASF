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

	/* Set configurations in CTRLB. */
	i2c_module.CTRLB = I2C_MASTER_SMEN_Msk;

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
	i2c_module.CTRLA = SERCOM_I2C_MODE | SERCOM_I2C_MASTER;

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
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	SERCOM_I2C_MASTER_t i2c_module = dev_inst->hw_dev->I2C_MASTER;

	uint16_t timeout_counter = 0;

	/* Start timeout if bus state is unknown. */
	while (i2c_module.STATUS & I2C_MASTER_BUSSTATE_UNKNOWN) {
		if(++timeout_counter >= dev_inst->unkown_bus_state_timeout) {
			/* Timeout, force bus state to idle. */
			i2c_module.STATUS = I2C_MASTER_BUSSTATE_IDLE;
		}
	}

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module.ADDR = packet->address << 1 | 0x01;

	/* Wait for reply. */
	timeout_counter = 0;
	while (!(i2c_module.INTFLAGS & I2C_MASTER_WIF) ||
			!(i2c_module.INTFLAGS & I2C_MASTER_RIF)) {
		/* Check timeout condition. */
		if (++timeout_counter >= dev_inst->buffer_timeout) {
			return STATUS_ERR_TIMEOUT;
		}
	}

	/* Check for error. */
	if (i2c_module.INTFLAGS & I2C_MASTER_WIF) {
		/* Clear write interrupt flag. */
		i2c_module.INTENCLR = I2C_MASTER_WIF;
		/* Check for busserror. */
		if (i2c_module.STATUS & (1 << I2C_MASTER_BUSSERROR_Pos)) {
			/* Return denied. */
			return STATUS_ERR_DENIED;
		/* Check arbitration. */
		} else if (i2c_module.STATUS & (1 << I2C_MASTER_ARBLOST_Pos)) {
			/* Return busy. */
			return STATUS_ERR_BUSY;
		}
	}

	/* Check that slave sent ACK. */
	if (i2c_module.STATUS & (1 << I2C_MASTER_RXACK_Pos)) {
		/* Init data length. */
		dev_inst->buffer_length = 0;
		/* Read data buffer. */
		while (dev_inst->buffer_length < packet->data_length) {
			packet->data[dev_inst->buffer_length++] =
					i2c_module.DATA;

			/* Wait for more data. */
			timeout_counter = 0;
			while (!(i2c_module.INTFLAGS & I2C_MASTER_RIF)) {
				/* Check timeout condition. */
				if (++timeout_counter >=
						dev_inst->buffer_timeout) {
					return STATUS_ERR_TIMEOUT;
				}
			}
		}
		/* Send nack and stop command. */
		i2c_module.CTRLB |= SERCOM_I2C_MASTER_NACK |
				SERCOM_I2C_MASTER_CMD(3);

		return STATUS_OK;

	} else {
		/* Slave busy. Issue ack and stop command. */
		i2c_module.CTRLB |= SERCOM_I2C_MASTER_NACK |
				SERCOM_I2C_MASTER_CMD(3);
		/* Return bad address value. */
		return STATUS_ERR_BAD_ADDRESS;
	}

}

enum status_code i2c_master_write_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	SERCOM_I2C_MASTER_t i2c_module = dev_inst->hw_dev->I2C_MASTER;

	uint16_t timeout_counter = 0;

	/* Start timeout if bus state is unknown. */
	while (i2c_module.STATUS & I2C_MASTER_BUSSTATE_UNKNOWN) {
		if(++timeout_counter >= dev_inst->unkown_bus_state_timeout) {
			/* Timeout, force bus state to idle. */
			i2c_module.STATUS = I2C_MASTER_BUSSTATE_IDLE;
		}
	}

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module.ADDR = packet->address << 1 | 0x00;

	/* Wait for reply. */
	timeout_counter = 0;
	while (!(i2c_module.INTFLAGS & I2C_MASTER_WIF) ||
			!(i2c_module.INTFLAGS & I2C_MASTER_RIF)) {
		/* Check timeout condition. */
		if (++timeout_counter >= dev_inst->buffer_timeout) {
			return STATUS_ERR_TIMEOUT;
		}
	}

	/* Check for error. */
	if (i2c_module.INTFLAGS & I2C_MASTER_WIF) {
		/* Clear write interrupt flag. */
		i2c_module.INTENCLR = I2C_MASTER_WIF;
		/* Check for busserror. */
		if (i2c_module.STATUS & (1 << I2C_MASTER_BUSSERROR_Pos)) {
			/* Return denied. */
			return STATUS_ERR_PROTOCOL;
		/* Check arbitration. */
		} else if (i2c_module.STATUS & (1 << I2C_MASTER_ARBLOST_Pos)) {
			/* Return busy. */
			return STATUS_ERR_PACKET_COLLISION;
		}
	}

	/* Check that slave sent ACK. */
	if (i2c_module.STATUS & (1 << I2C_MASTER_RXACK_Pos)) {
		/* Init buffer counter. */
		dev_inst->buffer_length = 0;
		/* Write data buffer. */
		while (dev_inst->buffer_length < packet->data_length) {
			/* Write byte to slave. */
			i2c_module.DATA = packet->data[dev_inst->buffer_length++];

			/* Wait for ack. */
			timeout_counter = 0;
			while (!(i2c_module.INTFLAGS & I2C_MASTER_RIF)) {
				/* Check timeout condition. */
				if (++timeout_counter >=
						dev_inst->buffer_timeout) {
					return STATUS_ERR_TIMEOUT;
				}
			}

			/* Check for ack from slave. */
			if (!(i2c_module.STATUS & (1 << I2C_MASTER_RXACK_Pos)))
			{
				i2c_module.CTRLB |= SERCOM_I2C_MASTER_NACK |
						SERCOM_I2C_MASTER_CMD(3);
				/* Return bad address value. */
				return STATUS_ERR_BAD_DATA;
			}
		}
		return STATUS_OK;

	} else {
		/* Slave busy. Issue ack and stop command. */
		i2c_module.CTRLB |= SERCOM_I2C_MASTER_NACK |
				SERCOM_I2C_MASTER_CMD(3);
		/* Return bad address value. */
		return STATUS_ERR_BAD_ADDRESS;
	}
}