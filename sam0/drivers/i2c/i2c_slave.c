/**
 * \file
 *
 * \brief I2C Master Driver for SAMB11
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "i2c_slave.h"
#if I2C_SLAVE_CALLBACK_MODE == true
#include "i2c_slave_interrupt.h"
#endif

void i2c_slave_get_config_defaults(
		struct i2c_slave_config *const config)
{
	config->core_idx        = I2C_CORE1;
	config->clock_source    = I2C_CLK_INPUT_3;
	config->clock_divider   = 0x10;
	config->pinmux_pad0   = PINMUX_LP_GPIO_8_MUX2_I2C0_SDA;
	config->pinmux_pad1   = PINMUX_LP_GPIO_9_MUX2_I2C0_SCK;
}

/**
 * \internal Sets configurations to module
 *
 * \param[out] module  Pointer to software module structure
 * \param[in]  config  Configuration structure with configurations to set
 *
 * \return Status of setting configuration.
 * \retval STATUS_OK                        If module was configured correctly
 * \retval STATUS_ERR_ALREADY_INITIALIZED   If setting other GCLK generator than
 *                                          previously set
 * \retval STATUS_ERR_BAUDRATE_UNAVAILABLE  If given baud rate is not compatible
 *                                          with set GCLK frequency
 */
static enum status_code _i2c_slave_set_config(
		struct i2c_slave_module *const module,
		const struct i2c_slave_config *const config)
{
	enum status_code status = STATUS_OK;
	I2C *const i2c_module = (module->hw);

	/* Set the pinmux for this i2c module. */
	gpio_pinmux_cofiguration(config->pinmux_pad0>>16, (uint16_t)(config->pinmux_pad0 & 0xFFFF));
	gpio_pinmux_cofiguration(config->pinmux_pad1>>16, (uint16_t)(config->pinmux_pad1 & 0xFFFF));
									
	/* Find and set baudrate. */
	i2c_module->CLOCK_SOURCE_SELECT.reg = config->clock_source;
	i2c_module->I2C_CLK_DIVIDER.reg = I2C_I2C_CLK_DIVIDER_I2C_DIVIDE_RATIO(config->clock_divider);
	/* I2C slave address */
	i2c_module->I2C_SLAVE_ADDRESS.reg = I2C_I2C_SLAVE_ADDRESS_ADDRESS(config->address);
	/* I2C slave mode */
	i2c_module->I2C_MASTER_MODE.reg = I2C_I2C_MASTER_MODE_MASTER_ENABLE_0;
#if I2C_SLAVE_CALLBACK_MODE == true
	i2c_module->I2C_FLUSH.reg = 1;
	i2c_module->RX_INTERRUPT_MASK.reg = I2C_RX_INTERRUPT_MASK_RX_FIFO_NOT_EMPTY_MASK;
	i2c_module->TX_INTERRUPT_MASK.reg = I2C_TX_INTERRUPT_MASK_TX_FIFO_NOT_FULL_MASK;
#endif
	return status;
}

/**
 * \brief Initializes the requested I<SUP>2</SUP>C hardware module
 *
 * Initializes the I<SUP>2</SUP>C master device requested and sets the provided
 * software module struct. Run this function before any further use of
 * the driver.
 *
 * \param[out] module  Pointer to software module struct
 * \param[in]  config  Pointer to the configuration struct
 *
 * \return Status of initialization.
 * \retval STATUS_OK                        Module initiated correctly
 * \retval STATUS_ERR_INVALID_ARG           Invalid argument in module or config structure.
 * \retval STATUS_ERR_ALREADY_INITIALIZED   If the Pinmux is not a valid one for I2C signals.
 *
 */
enum status_code i2c_slave_init(
		struct i2c_slave_module *const module,
		const struct i2c_slave_config *const config)
{
	/* Sanity check arguments. */
	if ((module == NULL) || (config == NULL))
		return STATUS_ERR_INVALID_ARG;

	/* Initialize software module */
	if (config->core_idx == I2C_CORE1) {
		module->hw = (void *)I2C0;
	}
#ifdef CHIPVERSION_B0
	else if (config->core_idx == I2C_CORE2) {
		module->hw = (void *)I2C1;
	}
#endif  /* CHIPVERSION_B0 */
	else {
		return STATUS_ERR_INVALID_ARG;
	}
	I2C *const i2c_module = (module->hw);
	i2c_disable(i2c_module);
	if (config->core_idx == I2C_CORE1)
		system_peripheral_reset(PERIPHERAL_I2C0_CORE);
#ifdef CHIPVERSION_B0
	else if (config->core_idx == I2C_CORE2) {
		system_peripheral_reset(PERIPHERAL_I2C1_CORE);
	}
#endif  /* CHIPVERSION_B0 */
	else {
		return STATUS_ERR_INVALID_ARG;
	}

#if I2C_SLAVE_CALLBACK_MODE == true

	/* Initialize values in module. */
	module->registered_callback = 0;
	module->enabled_callback = 0;
	module->rx.buffer_length = 0;
	module->rx.buffer_remaining = 0;
	module->rx.buffer = NULL;
	module->tx.buffer_length = 0;
	module->tx.buffer_remaining = 0;
	module->tx.buffer = NULL;
	module->status = STATUS_OK;
	_i2c_instances = (void*)module;
	system_register_isr(RAM_ISR_TABLE_I2CRX0_INDEX, (uint32_t)_i2c_slave_rx_isr_handler);
	system_register_isr(RAM_ISR_TABLE_I2CTX0_INDEX, (uint32_t)_i2c_slave_tx_isr_handler);
#endif

	i2c_enable(i2c_module);
	/* Set config and return status. */
	if(_i2c_slave_set_config(module, config) != STATUS_OK)
		return STATUS_ERR_NOT_INITIALIZED;

	return STATUS_OK;
}

/**
 * \brief Reads a packet from the master
 *
 * Reads a packet from the master. This will wait for the master to issue a
 * request.
 *
 * \param[in]  module  Pointer to software module structure
 * \param[out] packet  Packet to read from master
 *
 * \return Status of packet read.
 * \retval STATUS_OK                Packet was read successfully
 * \retval STATUS_ERR_INVALID_ARG   Invalid argument(s) was provided
 */
enum status_code i2c_slave_read_packet_wait(
		struct i2c_slave_module *const module,
		struct i2c_slave_packet *const packet)
{
	I2C *const i2c_module = (module->hw);
	uint16_t i = 0;
	uint32_t status = 0;
	uint16_t length = packet->data_length;
	
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}
	
	do {
		status = i2c_module->RECEIVE_STATUS.reg;
		if (status & I2C_RECEIVE_STATUS_RX_FIFO_NOT_EMPTY)
			packet->data[i++] = i2c_module->RECEIVE_DATA.reg;
	} while (i < length);

	/* Now check whether the core has sent the data out and its good to free the bus. */
	while (!(status & I2C_TRANSMIT_STATUS_TX_FIFO_EMPTY)) {
		status = i2c_module->TRANSMIT_STATUS.reg;
	}

	return STATUS_OK;
}

/**
 * \brief Writes a packet to the master
 *
 * Writes a packet to the master. This will wait for the master to issue
 * a request.
 *
 * \param[in]  module  Pointer to software module structure
 * \param[in]  packet  Packet to write to master
 *
 * \return Status of packet write.
 * \retval STATUS_OK                Packet was written successfully
 * \retval STATUS_ERR_INVALID_ARG   Invalid argument(s) was provided
 */
enum status_code i2c_slave_write_packet_wait(
		struct i2c_slave_module *const module,
		struct i2c_slave_packet *const packet)
{
	I2C *const i2c_module = (module->hw);
	uint16_t i = 0;
	uint32_t status = 0;
	uint16_t length = packet->data_length;
	
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}
	
	i2c_wait_for_idle(i2c_module);
	
	/* Flush the FIFO */
	i2c_module->I2C_FLUSH.reg = 1;
	
	do {
		status = i2c_module->TRANSMIT_STATUS.reg;
		if (status & I2C_TRANSMIT_STATUS_TX_FIFO_NOT_FULL_Msk) {
			i2c_module->TRANSMIT_DATA.reg = packet->data[i++];
		}
	} while (i < length);

	/* Now check whether the core has sent the data out and its good to free the bus */
	while (!(status & I2C_TRANSMIT_STATUS_TX_FIFO_EMPTY)) {
		status = i2c_module->TRANSMIT_STATUS.reg;
	}

	return STATUS_OK;
}

/**
 * \brief Waits for a start condition on the bus
 *
 * Waits for the master to issue a start condition on the bus.
 * Note that this function does not check for errors in the last transfer,
 * this will be discovered when reading or writing.
 *
 * \param[in]  module  Pointer to software module structure
 *
 * \return Direction of the current transfer, when in slave mode.
 * \retval I2C_SLAVE_DIRECTION_NONE   No request from master within timeout
 *                                    period
 * \retval I2C_SLAVE_DIRECTION_READ   Write request from master
 * \retval I2C_SLAVE_DIRECTION_WRITE  Read request from master
 */
enum i2c_slave_direction i2c_slave_get_direction_wait(
		struct i2c_slave_module *const module)
{
	I2C *const i2c_module = (module->hw);
	uint32_t status = 0;
	uint8_t dir = 0;
	
	while (!(status & I2C_RECEIVE_STATUS_RX_FIFO_NOT_EMPTY)) {
		
	}
	
	dir = i2c_module->RECEIVE_DATA.reg;
	
	if (dir == I2C_SLAVE_DIRECTION_READ) {
		return I2C_SLAVE_DIRECTION_READ;
	} else if (dir == I2C_SLAVE_DIRECTION_WRITE) {
		return I2C_SLAVE_DIRECTION_WRITE;
	} else {
		return I2C_SLAVE_DIRECTION_NONE;
	}
}