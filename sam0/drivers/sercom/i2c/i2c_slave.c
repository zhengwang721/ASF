/**
 * \file
 *
 * \brief SAMD20 I2C Slave Driver
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
 
#include "i2c_slave.h"

#ifdef I2C_SLAVE_ASYNC
# include "i2c_slave_interrupt.h"
#endif
 
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

#ifdef I2C_SLAVE_ASYNC
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
#endif

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

#ifdef I2C_SLAVE_ASYNC
	/* Reset module instance. */
	module->registered_callback = 0;
	module->enabled_callback = 0;
	module->buffer_length = 0;
	module->buffer_remaining = 0;
	module->buffer = NULL;
#endif

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	/* Disable module */
	i2c_slave_disable(module);

#ifdef I2C_SLAVE_ASYNC
	/* Clear all pending interrupts. */
	system_interrupt_enter_critical_section();
	system_interrupt_clear_pending(_sercom_get_interrupt_vector(module->hw));
	system_interrupt_leave_critical_section();
#endif

	/* Wait for sync. */
	_i2c_slave_wait_for_sync(module);

	/* Reset module. */
	i2c_hw->CTRLA.reg = SERCOM_I2CS_CTRLA_SWRST;
}
