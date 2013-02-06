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

#include "i2c_master.h"

#ifdef I2C_MASTER_ASYNC
# include "i2c_master_async.h"
#endif

#if !defined(__DOXYGEN__)
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

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);
	Sercom *const sercom_hw = dev_inst->hw_dev;

	/* Pin configuration */
	struct system_pinmux_conf pin_conf;
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

	/* Save timeout on unknown bus state in device instance. */
	dev_inst->unkown_bus_state_timeout = config->unkown_bus_state_timeout;

	/* Save timeout on buffer write. */
	dev_inst->buffer_timeout = config->buffer_timeout;

	/* Check and set if module should run in standby. */
	if (config->run_in_standby) {
		tmp_ctrla = SERCOM_I2CM_CTRLA_RUNSTDBY;
	}

	/* Check and set start data hold timeout. */
	if (config->start_hold_time != I2C_MASTER_START_HOLD_TIME_DISABLED) {
		tmp_ctrla |= config->start_hold_time;
	}

        /* Workaround for BUSSTATE stuck at BUSY */
        tmp_ctrla |= SERCOM_I2CM_CTRLA_INACTOUT(3);

	/* Write config to register CTRLA. */
	i2c_module->CTRLA.reg |= tmp_ctrla;

	/* Set configurations in CTRLB. */
	i2c_module->CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN;

	/* Set sercom gclk generator according to config. */
        uint32_t gclk_index = _sercom_get_sercom_inst_index(dev_inst->hw_dev) + 13;

	/* Find and set baudrate. */
	tmp_baud = (int32_t)((system_gclk_ch_get_hz(gclk_index)
			/ (2*1000*config->baud_rate))-5);
	/* Check that baud rate is supported at current speed. */
	if (tmp_baud > 255 || tmp_baud < 0) {
		/* Baud rate not supported. */
		tmp_status_code = STATUS_ERR_BAUDRATE_UNAVAILABLE;

	} else {
		/* Baud rate acceptable. */
		i2c_module->BAUD.reg = (uint8_t)tmp_baud;
	}

	return tmp_status_code;
}
#endif /* __DOXYGEN__ */

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
		Sercom *const module,
		const struct i2c_master_conf *const config)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(module);
	Assert(config);

	/* Initialize device instance */
	dev_inst->hw_dev = module;

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

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
	gclk_ch_conf.source_generator = GCLK_GENERATOR_1;
	system_gclk_ch_set_config(SERCOM_GCLK_ID, &gclk_ch_conf);
	system_gclk_ch_enable(gclk_index);
	system_gclk_ch_enable(SERCOM_GCLK_ID);


	/* Check if module is enabled. */
	if (i2c_module->CTRLA.reg & SERCOM_I2CM_CTRLA_ENABLE) {
		return STATUS_ERR_DENIED;
	}

	/* Check if reset is in progress. */
	if (i2c_module->CTRLA.reg & SERCOM_I2CM_CTRLA_SWRST){
		return STATUS_ERR_BUSY;
	}

	/* Check if reset is in progress. */
	if (i2c_module->CTRLA.reg & SERCOM_I2CS_CTRLA_SWRST){
		return STATUS_ERR_BUSY;
	}

#ifdef I2C_MASTER_ASYNC
	/* Get sercom instance index and register callback. */
	uint8_t instance_index = _sercom_get_sercom_inst_index(module);
	_sercom_instances[instance_index] = dev_inst;
	_sercom_set_handler(instance_index, _i2c_master_async_callback_handler);

	/* Initialize values in dev_inst. */
	dev_inst->registered_callback = 0;
	dev_inst->enabled_callback = 0;
	dev_inst->buffer_length = 0;
#endif

	//dev_inst->callback[0] = 0;
	/* Set sercom module to operate in I2C master mode. */
	i2c_module->CTRLA.reg = SERCOM_I2CS_CTRLA_MODE(2)
			| SERCOM_I2CS_CTRLA_MASTER;

	/* Set config and return status. */
	return _i2c_master_set_config(dev_inst, config);
}

/**
 * \brief Reset the hardware module.
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

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Wait for sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Disable module. */
	i2c_master_disable(dev_inst);

	/* Wait for sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Reset module. */
	i2c_module->CTRLA.reg = SERCOM_I2CM_CTRLA_SWRST;
}

#if !defined(__DOXYGEN__)
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
	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Check for error. Ignore bus-error; workaround for BUSSTATE stuck in BUSY. */
	if (i2c_module->INTFLAG.reg & SERCOM_I2CM_INTFLAG_RIF) {

		/* Clear write interrupt flag. */
		i2c_module->INTFLAG.reg = SERCOM_I2CM_INTFLAG_RIF;

		/* Check arbitration. */
		if (i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_ARBLOST) {
			/* Return packet collision. */
			return STATUS_ERR_PACKET_COLLISION;
		}
	/* Check that slave responded with ack. */
	} else if (i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_RXNACK) {
		/* Slave busy. Issue ack and stop command. */
		i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);

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
	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Wait for reply. */
	uint16_t timeout_counter = 0;
	while (!(i2c_module->INTFLAG.reg & SERCOM_I2CM_INTFLAG_WIF) &&
			!(i2c_module->INTFLAG.reg & SERCOM_I2CM_INTFLAG_RIF)) {

		/* Check timeout condition. */
		if (++timeout_counter >= dev_inst->buffer_timeout) {
			return STATUS_ERR_TIMEOUT;
		}
	}
	return STATUS_OK;
}
#endif /* __DOXYGEN__ */

/**
 * \brief Read data packet from slave.
 *
 * Reads a data packet from the specified slave address on the I2C bus.
 *
 * \note This will stall the device from any other operation. For asynchronous
 * operation, see \ref i2c_master_async_read_packet.
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
enum status_code i2c_master_read_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet)
{
	/* Sanity check */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(packet);

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

#ifdef I2C_MASTER_ASYNC
	/* Check if the I2C module is busy doing async operation. */
	if (dev_inst->buffer_remaining > 0) {
		return STATUS_ERR_BUSY;
	}
#endif

	/* Return value. */
	enum status_code tmp_status;
	uint8_t tmp_data_length = packet->data_length;

	/* Written buffer counter. */
	uint16_t counter = 0;

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module->ADDR.reg = (packet->address << 1) | _I2C_TRANSFER_READ;

	/* Wait for response on bus. */
	tmp_status = _i2c_master_wait_for_bus(dev_inst);

	/* Set action to ack. */
	i2c_module->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

	/* Check for address response error unless previous error is
	 * detected. */
	if (tmp_status == STATUS_OK) {
		tmp_status = _i2c_master_address_response(dev_inst);
	}

	/* Check that no error has occurred. */
	if (tmp_status == STATUS_OK) {
		/* Read data buffer. */
		while (tmp_data_length--) {
			/* Check that bus ownership is not lost. */
			if (!(i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE(2))) {
				return STATUS_ERR_PACKET_COLLISION;
			}

			/* Save data to buffer. */
			packet->data[counter++] = i2c_module->DATA.reg;

			/* Wait for response. */
			tmp_status = _i2c_master_wait_for_bus(dev_inst);

			/* Check for error. */
			if (tmp_status != STATUS_OK) {
				break;
			}
		}

		/* Send nack and stop command unless arbitration is lost. */
		i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_CMD(3);
	}

	return tmp_status;
}

/**
 * \brief Write data packet to slave.
 *
 * Writes a data packet to the specified slave address on the I2C bus.
 *
 * \note This will stall the device from any other operation. For asynchronous
 * operation, see \ref i2c_master_async_read_packet.
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
 * \retval STATUS_ERR_OVERFLOW If slave did not acknowledge last sent data,
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

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

#ifdef I2C_MASTER_ASYNC
	/* Check if the I2C module is busy doing async operation. */
	if (dev_inst->buffer_remaining > 0) {
		return STATUS_ERR_BUSY;
	}
#endif

	/* Return value. */
	enum status_code tmp_status;
	uint8_t tmp_data_length = packet->data_length;

        _i2c_master_wait_for_sync(dev_inst);

	/* Set address and direction bit. Will send start command on bus. */
	i2c_module->ADDR.reg = (packet->address << 1) | _I2C_TRANSFER_WRITE;

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
		while (tmp_data_length--) {
			/* Check that bus ownership is not lost. */
			if (!(i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE(2))) {
				return STATUS_ERR_PACKET_COLLISION;
			}

			/* Write byte to slave. */
			i2c_module->DATA.reg = packet->data[buffer_counter++];

			/* Wait for response. */
			tmp_status = _i2c_master_wait_for_bus(dev_inst);

			/* Check for error. */
			if (tmp_status != STATUS_OK) {
				break;
			}

			/* Check for NAK from slave. */
			if (i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
			{
				i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
				if(packet->data_length) {
					/* Return bad data value. */
					tmp_status = STATUS_ERR_OVERFLOW;
					break;
				}
			}
		}

		/* Stop command. */
		i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
	}

	return tmp_status;
}
