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

#ifdef I2C_MASTER_ASYNC
# include "i2c_master_async.h"
#endif

/**
 * \internal Set configurations to module.
 *
 * \param[out] dev_inst Pointer to device instance structure.
 * \param[in]  config Configuration structure with configurations to set.
 *
 * \return              Status of setting config.
 * \retval STATUS_OK If module was configured correctly.
 * \retval STATUS_ERR_ALREADY_INITIALIZED If setting other gclk generator than
 *                                        previously set.
 * \retval STAUS_ERR_BAUDRATE_UNAVAILABLE If given baud rate is not compatible
 *                                        with set gclk frequency.
 */
static enum status_code _i2c_master_set_config(
		struct i2c_master_dev_inst *const dev_inst,
		const struct i2c_master_conf *const config)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(config);

	/* Temporary variables. */
	uint32_t tmp_ctrla = 0;
	int32_t tmp_baud;
	enum status_code tmp_status_code = STATUS_OK;

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Save timeout on unknown bus state in device instance. */
	dev_inst->unkown_bus_state_timeout = config->unkown_bus_state_timeout;
	/* Save timeout on buffer write. */
	dev_inst->buffer_timeout = config->buffer_timeout;

	/* Check and set if module should run in standby. */
	if (config->run_in_standby) {
		tmp_ctrla = (1 << I2C_MASTER_RUNINSTDBY_Pos);
	}

	/* Check and set start data hold timeout. */
	if (config->start_hold_time != I2C_MASTER_START_HOLD_TIME_DISABLED) {
		tmp_ctrla |= config->start_hold_time;
	}

	/* Write config to register CTRLA. */
	i2c_module->CTRLA |= tmp_ctrla;

	/* Set configurations in CTRLB. */
	i2c_module->CTRLB = I2C_MASTER_SMEN_Msk;

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
	tmp_baud = (int32_t)(system_gclk_ch_get_hz(SERCOM_GCLK_ID)
			/ (2*config->baud_rate)-5);

	/* Check that baud rate is supported at current speed. */
	if (tmp_baud > 255 || tmp_baud < 0) {
		/* Baud rate not supported. */
		tmp_status_code = STATUS_ERR_BAUDRATE_UNAVAILABLE;

	} else {
		/* Baud rate acceptable. */
		i2c_module->BAUD = (uint8_t)tmp_baud;
	}

	return tmp_status_code;
}

/**
 * \brief Initializes the requested I2C Hardware module.
 *
 * Initializes the SERCOM I2C Master device requested and sets the provided
 * device instance struct. This will also reset the hardware module, all
 * current settings will be lost. Run this function before any further use of
 * the driver.
 *
 * \param[out] dev_inst Pointer to device instance struct.
 * \param[in]  module   Pointer to the hardware instance.
 * \param[in]  config   Pointer to the configuration struct.
 *
 * \return              Status of initialization.
 * \retval STATUS_OK Module initiated correctly.
 * \retval STATUS_ERR_DENIED If module is enabled.
 * \retval STATUS_ERR_BUSY If module is busy resetting.
 * \retval STATUS_ERR_ALREADY_INITIALIZED If setting other gclk generator than
 *                                        previously set.
 * \retval STAUS_ERR_BAUDRATE_UNAVAILABLE If given baud rate is not compatible
 *                                        with set gclk frequency.
 *
 */
enum status_code i2c_master_init(struct i2c_master_dev_inst *const dev_inst,
		SERCOM_t *const module,
		const struct i2c_master_conf *const config)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(module);
	Assert(config);

	/* Initialize device instance */
	dev_inst->hw_dev = module;

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Check if module is enabled. */
	if (i2c_module->CTRLA & (1 << I2C_MASTER_ENABLE_Pos)) {
		return STATUS_ERR_DENIED;
	}

	/* Check if reset is in progress. */
	if (i2c_module->CTRLA & ( 1 << I2C_MASTER_SWRST_Pos)){
		return STATUS_ERR_BUSY;
	}

#ifdef I2C_MASTER_ASYNC
	/* Get sercom instance index. */
	uint8_t sercom_instance = _sercom_get_sercom_inst_index(module);

	/* Save device instance in interrupt handler. */
	_sercom_set_handler(sercom_instance,
			(void*)(&_i2c_master_async_callback_handler));

	/* Save device instance. */
	_sercom_instances[sercom_instance] = (void*) dev_inst;

	/* Initialize values in dev_inst. */
	dev_inst->registered_callback = 0;
	dev_inst->enabled_callback = 0;
	dev_inst->buffer_length = 0;
	dev_inst->async_ongoing = 0;
#endif

	//dev_inst->callback[0] = 0;

	/* Set sercom module to operate in I2C master mode. */
	i2c_module->CTRLA = SERCOM_I2C_MODE | SERCOM_I2C_MASTER;

	/* Set config and return status. */
	return _i2c_master_set_config(dev_inst, config);
}

/**
 * \breif Reset module
 *
 * This will reset the module to hardware defaults.
 *
 * \param[in,out] dev_inst Pointer to device instance structure.
 */
void i2c_master_reset(struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Wait for sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Disable module. */
	i2c_master_disable(dev_inst);

	/* Wait for sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Reset module. */
	i2c_module->CTRLA = ( 1 << I2C_MASTER_SWRST_Pos );
}

/**
 * \internal Address respond.
 *
 * Called when address is answered or timed out.
 *
 * \param[in,out] dev_inst Pointer to device instance structure.
 *
 * \return Status of address response.
 * \retval STATUS_OK No error has occurred.
 * \retval STATUS_ERR_DENIED If error on bus.
 * \retval STATUS_ERR_PACKET_COLLISION If arbitration is lost.
 * \retval STATUS_ERR_BAD_ADDRESS If slave is busy, or no slave acknowledged the
 *                                address.
 */
static enum status_code _i2c_master_address_response(
		struct i2c_master_dev_inst *const dev_inst)
{
	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Check for error. */
	if (i2c_module->INTFLAGS & (1 << I2C_MASTER_WIF_Pos)) {
		/* Clear write interrupt flag. */
		i2c_module->INTFLAGS = (1 << I2C_MASTER_WIF_Pos);

		/* Check for busserror. */
		if (i2c_module->STATUS & (1 << I2C_MASTER_BUSSERROR_Pos)) {
			/* Return denied. */
			return STATUS_ERR_DENIED;

		/* Check arbitration. */
		} else if (i2c_module->STATUS & (1 << I2C_MASTER_ARBLOST_Pos)) {
			/* Return packet collision. */
			return STATUS_ERR_PACKET_COLLISION;
		}
	/* Check that slave responded with ack. */
	} else if ( i2c_module->STATUS & (0 << I2C_MASTER_RXACK_Pos) ) {
		/* Slave busy. Issue ack and stop command. */
		i2c_module->CTRLB |= SERCOM_I2C_MASTER_NACK |
				SERCOM_I2C_MASTER_CMD(3);

		/* Return bad address value. */
		return STATUS_ERR_BAD_ADDRESS;
	}

	return STATUS_OK;
}

/**
 * \internal Wait for answer on bus.
 *
 * \param[in,out] dev_inst Pointer to device instance structure.
 *
 * \return Status of bus.
 * \retval STATUS_OK If given response from slave device.
 * \retval STATUS_ERR_TIMEOUT If no response was given within specified timeout
 *                            period.
 */
static enum status_code _i2c_master_wait_for_bus(
		struct i2c_master_dev_inst *const dev_inst)
{
	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Wait for reply. */
	uint16_t timeout_counter = 0;
	while (!(i2c_module->INTFLAGS & (1 << I2C_MASTER_WIF_Pos)) ||
			!(i2c_module->INTFLAGS & (1 << I2C_MASTER_RIF_Pos))) {
		/* Check timeout condition. */
		if (++timeout_counter >= dev_inst->buffer_timeout) {
			return STATUS_ERR_TIMEOUT;
		}
	}
	return STATUS_OK;
}

/**
 * \brief Read data packet from slave.
 *
 * Reads a data packet from the specified slave address on the I2C bus.
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out]     packet    Pointer to I2C packet to transfer.
 * \return          Status describing progress of reading packet.
 * \retval STATUS_OK If packet was read.
 * \retval STATUS_ERR_BUSY If master module is busy.
 * \retval STATUS_ERR_DENIED If error on bus.
 * \retval STATUS_ERR_PACKET_COLLISION If arbitration is lost.
 * \retval STATUS_ERR_BAD_ADDRESS If slave is busy, or no slave acknowledged the
 *                                address.
 * \retval STATUS_ERR_TIMEOUT If timeout occurred.
 */

enum _i2c_state_machine_state {
	_I2C_STATE_MACHINE_INIT = 0,
	_I2C_STATE_MACHINE_WAIT = 1,
	_I2C_STATE_MACHINE_ADDRESS = 2,
	_I2C_STATE_MACHINE_CHECK_STATUS = 3,
	_I2C_STATE_MACHINE_READ = 4,
	_I2C_STATE_MACHINE_BUS_STATE = 5,
	_I2C_STATE_MACHINE_CLOSE_CONNECTION = 6,
	_I2C_STATE_MACHINE_END = 7,
};

enum status_code i2c_master_read_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);
#ifdef I2C_MASTER_ASYNC
	/* Check if the I2C module is busy doing async operation. */
	if (dev_inst->async_ongoing != false) {
		return STATUS_ERR_BUSY;
	}
#endif

	/* Return value. */
	enum status_code tmp_status = STATUS_OK;

	/* Written buffer counter. */
	uint16_t counter = 0;

	/* Variables for state machine. */
	enum _i2c_state_machine_state state = _I2C_STATE_MACHINE_INIT;
	enum _i2c_state_machine_state next_state;
	bool run_state_machine = true;

	/* Loop for state machine. */
	while (run_state_machine) {
		switch (state) {
			case _I2C_STATE_MACHINE_INIT:
				/* Set address and direction bit. Will send start command on bus. */
				i2c_module->ADDR = packet->address << 1 | (1 << I2C_MASTER_TRANSFER_READ_Pos);

				/* Set state to go to. */
				state = _I2C_STATE_MACHINE_WAIT;
				/* Set state after wait. */
				next_state = _I2C_STATE_MACHINE_ADDRESS;

				break;

			case _I2C_STATE_MACHINE_WAIT:
				/* Wait for response on bus. */
				tmp_status = _i2c_master_wait_for_bus(dev_inst);

				/* Check status. */
				state = _I2C_STATE_MACHINE_CHECK_STATUS;

				break;

			case _I2C_STATE_MACHINE_CHECK_STATUS:
				/* Check for error. */
				if (tmp_status != STATUS_OK) {
					state = _I2C_STATE_MACHINE_END;

					/* Save the number of written bytes. */
					packet->data_length = counter;

				} else {
					state = next_state;
				}

				break;

			case _I2C_STATE_MACHINE_ADDRESS:
				/* Respond to address response. */
				tmp_status = _i2c_master_address_response(dev_inst);

				/* Check status. */
				state = _I2C_STATE_MACHINE_CHECK_STATUS;
				/* State after status check. */
				next_state = _I2C_STATE_MACHINE_READ;

				break;

			case _I2C_STATE_MACHINE_READ:
				/* Save data to buffer, sends "ack" when reading DATA register. */
				packet->data[counter++] = i2c_module->DATA;

				/* Check if packet is sent. */
				if (packet->data_length-- != 0) {
					/* Wait for response. */
					state = _I2C_STATE_MACHINE_WAIT;
				} else {
					/* Packet is sent, close connection. */
					state = _I2C_STATE_MACHINE_CLOSE_CONNECTION;
				}

				/* Next command after response. */
				next_state = _I2C_STATE_MACHINE_BUS_STATE;

				break;

			case _I2C_STATE_MACHINE_BUS_STATE:
				/* Check bus state. */
				if (!(i2c_module->STATUS & I2C_MASTER_BUSSTATE_OWNER_Msk)) {
					/* Lost ownership of bus. */
					tmp_status = STATUS_ERR_PACKET_COLLISION;

					state = _I2C_STATE_MACHINE_CHECK_STATUS;

				} else {
					/* Owner of bus, read next byte. */
					state = _I2C_STATE_MACHINE_READ;
				}

				break;

			case _I2C_STATE_MACHINE_CLOSE_CONNECTION:
				/* Send nack and stop signal. */
				i2c_module->CTRLB |= SERCOM_I2C_MASTER_NACK | SERCOM_I2C_MASTER_CMD(3);

				state = _I2C_STATE_MACHINE_END;

				break;

			case _I2C_STATE_MACHINE_END:
				/* Stop state machine. */
				run_state_machine = false;

				break;

			default:
				Assert(false);
				tmp_status = STATUS_ERR_INVALID_ARG;
				state = _I2C_STATE_MACHINE_END;
		}
	}


	return tmp_status;

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module->ADDR = packet->address << 1 | (1 << I2C_MASTER_TRANSFER_READ_Pos);

	/* Wait for response on bus. */
	tmp_status = _i2c_master_wait_for_bus(dev_inst);

	/* Check for address response error unless previous error is
	 * detected. */
	if (tmp_status == STATUS_OK) {
		tmp_status = _i2c_master_address_response(dev_inst);
	}

	/* Check that no error has occurred. */
	if (tmp_status == STATUS_OK) {
		/* Init data length. */
		uint16_t buffer_counter = 0;

		/* Read data buffer. */
		while (packet->data_length--) {
			/* Check that bus ownership is not lost. */
			if (!(i2c_module->STATUS & I2C_MASTER_BUSSTATE_OWNER_Msk)) {
				return STATUS_ERR_PACKET_COLLISION;
			}

			/* Save data to buffer. */
			packet->data[buffer_counter++] = i2c_module->DATA;

			/* Wait for response. */
			tmp_status = _i2c_master_wait_for_bus(dev_inst);

			/* Check for error. */
			if (tmp_status != STATUS_OK) {
				break;
			}
		}
		/* Send nack and stop command unless arbitration is lost. */
		i2c_module->CTRLB |= SERCOM_I2C_MASTER_NACK |
				SERCOM_I2C_MASTER_CMD(3);

	}

	return tmp_status;

}

/**
 * \brief Write data packet to slave.
 *
 * Writes a data packet to the specified slave address on the I2C bus.
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out]     packet    Pointer to I2C packet to transfer.
 * \return          Status describing progress of reading packet.
 * \retval STATUS_OK If packet was read.
 * \retval STATUS_ERR_BUSY If master module is busy.
 * \retval STATUS_ERR_DENIED If error on bus.
 * \retval STATUS_ERR_PACKET_COLLISION If arbitration is lost.
 * \retval STATUS_ERR_BAD_ADDRESS If slave is busy, or no slave acknowledged the
 *                                address.
 * \retval STATUS_ERR_TIMEOUT If timeout occurred.
 * \retval STATUS_ERR_BAD_DATA If slave did not acknowledge last sent data,
 *                             indicating that slave do not want more data.
 */
enum status_code i2c_master_write_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

#ifdef I2C_MASTER_ASYNC
	/* Check if the I2C module is busy doing async operation. */
	if (dev_inst->async_ongoing != false) {
		return STATUS_ERR_BUSY;
	}
#endif

	/* Return value. */
	enum status_code tmp_status = STATUS_OK;

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module->ADDR = packet->address << 1 | (1 << I2C_MASTER_TRANSFER_WRITE_Pos);

	/* Wait for response on bus. */
	tmp_status = _i2c_master_wait_for_bus(dev_inst);

	/* Check for address response error unless previous error is
	 * detected. */
	if (tmp_status == STATUS_OK) {
		tmp_status = _i2c_master_address_response(dev_inst);
	}


	/* Check that no error has occurred. */
	if (tmp_status == STATUS_OK) {
		/* Buffer counter. */
		uint16_t buffer_counter = 0;

		/* Write data buffer. */
		while (packet->data_length--) {
			/* Check that bus ownership is not lost. */
			if (!(i2c_module->STATUS & I2C_MASTER_BUSSTATE_OWNER_Msk)) {
				return STATUS_ERR_PACKET_COLLISION;
			}

			/* Write byte to slave. */
			i2c_module->DATA = packet->data[buffer_counter++];

			/* Wait for response. */
			tmp_status = _i2c_master_wait_for_bus(dev_inst);

			/* Check for error. */
			if (tmp_status != STATUS_OK) {
				break;
			}

			/* Check for ack from slave. */
			if (!(i2c_module->STATUS & (1 << I2C_MASTER_RXACK_Pos)))
			{
				i2c_module->CTRLB |= SERCOM_I2C_MASTER_NACK |
						SERCOM_I2C_MASTER_CMD(3);
				/* Return bad data value. */
				tmp_status = STATUS_ERR_BAD_DATA;
				break;
			}
		}

		/* Send nack and stop command. */
		i2c_module->CTRLB |= SERCOM_I2C_MASTER_NACK |
				SERCOM_I2C_MASTER_CMD(3);
	}

	return tmp_status;
}