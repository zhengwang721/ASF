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

#ifndef I2C_SLAVE_INTERRUPT_H_INCLUDED
#define I2C_SLAVE_INTERRUPT_H_INCLUDED

#include "i2c_common.h"
#include <sercom.h>
#include <sercom_interrupt.h>
#include <system_interrupt.h>
#include <pinmux.h>

#ifndef PINMUX_DEFAULT
#define PINMUX_DEFAULT 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_i2c_slave_group_async I2C Slave Asynchronous
 * @{
 *
 * This is an overview of the API for the asynchronous I2C slave
 * driver.
 *
 * The user will still have to use the initialization from the basic use part
 * of the driver, which can be found \ref sam0_i2c_slave_group "here". When the
 * asynchronous driver is included in the project, an asynchronous part in the basic driver
 * will be unlocked. This will allow the user to implement the driver in the same way as
 * for the basic polled driver, but will additionally get the asynchronous functions
 * documented below.
 *
 * \section i2c_slave_async_api API Overview
 *
 */

/**
 * \brief Callback types.
 *
 * The available callback types for the I2C master module.
 */
enum i2c_slave_callback {
	/** Callback for packet write complete. */
	I2C_SLAVE_CALLBACK_WRITE_COMPLETE,
	/** Callback for packet read complete. */
	I2C_SLAVE_CALLBACK_READ_COMPLETE,
	/** Callback for read request from master can be usde to call write_buff */
	I2C_SLAVE_CALLBACK_READ_REQUEST,
	/** Callback for write request from master - can be used to call read_buff */
	I2C_SLAVE_CALLBACK_WRITE_REQUEST,
	/** Callback for error. */
	I2C_SLAVE_CALLBACK_ERROR,
	/** Callback for error in last transfer. Discovered on a new address
	 * interrupt */
	I2C_SLAVE_CALLBACK_ERROR_LAST_TRANSFER,
#if !defined(__DOXYGEN__)
	/** Total number of callbacks. */
	_I2C_SLAVE_CALLBACK_N,
#endif
};
#if !defined(__DOXYGEN__)

/* Device instance prototype */
struct i2c_slave_module;

typedef void (*i2c_slave_callback_t)(
		const struct i2c_slave_module *const module);
#endif

/**
 * \brief Enum for the possible SDA hold times with respect to the negative edge
 * of SCL
 *
 * Enum for the possible SDA hold times with respect to the negative edge
 * of SCL
 */
enum i2c_slave_sda_hold_time {
	/** SDA hold time disabled */
	I2C_SLAVE_SDA_HOLD_TIME_DISABLED = ((SERCOM_I2CS_CTRLA_SDAHOLD_Msk & ((0)
			<< SERCOM_I2CS_CTRLA_SDAHOLD_Pos))),
	/** SDA hold time 50ns-100ns */
	I2C_SLAVE_SDA_HOLD_TIME_50NS_100NS = ((SERCOM_I2CS_CTRLA_SDAHOLD_Msk & ((1)
			<< SERCOM_I2CS_CTRLA_SDAHOLD_Pos))),
	/** SDA hold time 300ns-600ns */
	I2C_SLAVE_SDA_HOLD_TIME_300NS_600NS = ((SERCOM_I2CS_CTRLA_SDAHOLD_Msk & ((2)
			<< SERCOM_I2CS_CTRLA_SDAHOLD_Pos))),
	/** SDA hold time 400ns-800ns */
	I2C_SLAVE_SDA_HOLD_TIME_400NS_800NS = ((SERCOM_I2CS_CTRLA_SDAHOLD_Msk & ((3)
			<< SERCOM_I2CS_CTRLA_SDAHOLD_Pos))),
};

/**
 * \brief Enum for the possible address modes
 *
 * Enum for the possible address modes
 */
 enum i2c_slave_address_mode {
	/** Address match on address_mask used as a mask to address */
	I2C_SLAVE_ADDRESS_MODE_MASK = SERCOM_I2CS_CTRLB_AMODE(0),
	/** Address math on both address and address_mask */
	I2C_SLAVE_ADDRESS_MODE_TWO_ADDRESSES = SERCOM_I2CS_CTRLB_AMODE(1),
	/** Address match on range of addresses between and including address and
	* address_mask */
	I2C_SLAVE_ADDRESS_MODE_RANGE = SERCOM_I2CS_CTRLB_AMODE(2),
 };

 /**
 * \brief SERCOM I2C Slave driver hardware instance
 *
 * Device instance structure for SERCOM I2C Slave instance. This structure
 * is used throughout the driver, and should be initiated using the
 * \ref i2c_slave_init() function to associate the struct with a particular
 * hardware instance and configurations.
 */
struct i2c_slave_module {
	/** Hardware instance initialized for the struct. */
	Sercom *hw;
	/** Nack on address match */
	bool nack_on_address;
	/** Pointers to callback functions. */
	volatile i2c_slave_callback_t callbacks[_I2C_SLAVE_CALLBACK_N];
	/** Mask for registered callbacks. */
	volatile uint8_t registered_callback;
	/** Mask for enabled callbacks. */
	volatile uint8_t enabled_callback;
	/** The total number of bytes to transfer. */
	volatile uint16_t buffer_length;
	/** Counter used for bytes left to send in write and to count number of
	 * obtained bytes in read. */
	volatile uint16_t buffer_remaining;
	/** Data buffer for packet write and read. */
	volatile uint8_t *buffer;
	/** Save direction of async request. 1 = read, 0 = write. */
	volatile uint8_t transfer_direction;
	/** Status for status read back in error callback. */
	volatile enum status_code status;
};

/**
 * \brief Configuration structure for the I2C device.
 *
 * This is the configuration structure for the I2C Slave device. It is used
 * as an argument for \ref i2c_slave_init to provide the desired
 * configurations for the module. The structure should be initiated using the
 * \ref i2c_slave_get_config_defaults .
 */
struct i2c_slave_conf {
	/** Set to enable the SCL low timeout */
	bool enable_scl_low_timeout;
	/** SDA hold time with respect to the negative edge of SCL */
	enum i2c_slave_sda_hold_time sda_hold_time;
	/** Addressing mode */
	enum i2c_slave_address_mode address_mode;
	/** Address or upper limit of address range */
	uint8_t address;
	/** Address mask, second address or lower limit of address range*/
	uint8_t address_mask;
	/**
	 * Enable general call address recognition. General call address
	 * is defined as 0000000 with dir bit 0
	 */
	bool enable_general_call_address;
	/**
	 * Enable nack on address match. Can be changed with \ref
	 * enable_address_nack and \ref disable_address_nack functions.
	 */
	bool enable_nack_on_address;
	/** GCLK generator to use as clock source. */
	enum gclk_generator generator_source;
	/** Set to keep module active in sleep modes. */
	bool run_in_standby;
	/** PAD0 (SDA) pinmux */
	uint32_t pinmux_pad0;
	/** PAD1 (SCL) pinmux */
	uint32_t pinmux_pad1;
};

#if !defined(__DOXYGEN__)
/**
 * \internal Wait for hardware module to sync.
 * \param[in]  module Pointer to device instance structure.
 */
static void _i2c_slave_wait_for_sync(
		const struct i2c_slave_module *const module)
{
	/* Sanity check. */
	Assert(module);
	Assert(module->hw);

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	while(i2c_hw->STATUS.reg & SERCOM_I2CS_STATUS_SYNCBUSY) {
		/* Wait for I2C module to sync. */
	}
}
#endif


/**
 * \brief Returns the synchronization status of the module.
 *
 * Returns the synchronization status of the module.
 *
 * \param[out] module Pointer to device instance structure.
 *
 * \return       Status of the synchronization
 * \retval true  Module is busy synchronizing
 * \retval false Module is not synchronizing
 */
static inline bool i2c_slave_is_syncing (const struct i2c_slave_module *const module)
{
	/* Sanity check. */
	Assert(module);
	Assert(module->hw);

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	if (i2c_hw->STATUS.reg & SERCOM_I2CS_STATUS_SYNCBUSY) {
		return true;
	} else {
		return false;
	}
}

/**
 * \brief Get the I2C slave default configurations.
 *
 * Use to initialize the configuration structure to known default values. This
 * function should be called at the start of any I2C initiation.
 *
 * The default configuration is as follows:
 * - Disable SCL low timeout
 * - 300ns - 600ns SDA hold time
 * - Address with mask
 * - Address = 0
 * - Address mask = 0 (one single address)
 * - General call address disabled
 * - Address nack disabled
 * - GCLK generator 0
 * - Do not run in standby
 *
 * \param[out] config Pointer to configuration structure to be initiated.
 */
static inline void i2c_slave_get_config_defaults(
		struct i2c_slave_conf *const config)
{
	/*Sanity check argument. */
	Assert(config);
	config->enable_scl_low_timeout = false;
	config->sda_hold_time = I2C_SLAVE_SDA_HOLD_TIME_300NS_600NS;
	config->address_mode = I2C_SLAVE_ADDRESS_MODE_MASK;
	config->address = 0;
	config->address_mask = 0;
	config->enable_general_call_address = false;
	config->enable_nack_on_address = false;
	config->generator_source = GCLK_GENERATOR_0;
	config->run_in_standby = false;
	config->pinmux_pad0 = PINMUX_DEFAULT;
	config->pinmux_pad1 = PINMUX_DEFAULT;
}

enum status_code i2c_slave_init(struct i2c_slave_module *const module,
		Sercom *const hw,
		const struct i2c_slave_conf *const config);

/**
 * \brief Enable the I2C module.
 *
 * This will enable the requested I2C module.
 *
 * \param[in]  module Pointer to the device instance struct.
 */
static inline void i2c_slave_enable(
		const struct i2c_slave_module *const module)
{
	/* Sanity check of arguments. */
	Assert(module);
	Assert(module->hw);

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	/* Enable interrupts */
	i2c_hw->INTENSET.reg = SERCOM_I2CS_INTENSET_PIEN |
			SERCOM_I2CS_INTENSET_AIEN | SERCOM_I2CS_INTENSET_DIEN;

	/* Wait for module to sync. */
	_i2c_slave_wait_for_sync(module);

	/* Enable module. */
	i2c_hw->CTRLA.reg |= SERCOM_I2CS_CTRLA_ENABLE;
}

/**
 * \brief Disable the I2C module.
 *
 * This will disable the I2C module specified in the provided device instance
 * structure.
 *
 * \param[in]  module Pointer to the device instance struct.
 */
static inline void i2c_slave_disable(
		const struct i2c_slave_module *const module)
{
	/* Sanity check of arguments. */
	Assert(module);
	Assert(module->hw);

	SercomI2cs *const i2c_hw = &(module->hw->I2CS);

	/* Disable interrupts */
	i2c_hw->INTENCLR.reg = SERCOM_I2CS_INTENSET_PIEN |
			SERCOM_I2CS_INTENSET_AIEN | SERCOM_I2CS_INTENSET_DIEN;

	/* Clear interrupt flags */
	i2c_hw->INTFLAG.reg = SERCOM_I2CS_INTFLAG_PIF | SERCOM_I2CS_INTFLAG_AIF |
			SERCOM_I2CS_INTFLAG_DIF;

	/* Wait for module to sync. */
	_i2c_slave_wait_for_sync(module);

	/* Disable module. */
	i2c_hw->CTRLA.reg &= ~SERCOM_I2CS_CTRLA_ENABLE;
}

void i2c_slave_reset(struct i2c_slave_module *const module);
void i2c_slave_enable_nack_on_address(struct i2c_slave_module
		*const module);
void i2c_slave_disable_nack_on_address(struct i2c_slave_module
		*const module);

/**
 * \name Callbacks
 * @{
 */
#if !defined(__DOXYGEN__)
void _i2c_slave_interrupt_handler(uint8_t instance);
#endif

void i2c_slave_register_callback(
		struct i2c_slave_module *const module,
		i2c_slave_callback_t callback,
		enum i2c_slave_callback callback_type);

void i2c_slave_unregister_callback(
		struct i2c_slave_module *const module,
		enum i2c_slave_callback callback_type);

/**
 * \brief Enable callback.
 *
 * Enables the callback specified by the callback_value.
 *
 * \param[in,out]  module      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to enable.
 */
static inline void i2c_slave_enable_callback(
		struct i2c_slave_module *const module,
		enum i2c_slave_callback callback_type)
{
	/* Sanity check. */
	Assert(module);
	Assert(module->hw);

	/* Mark callback as enabled. */
	module->enabled_callback |= (1 << callback_type);
}


/**
 * \brief Disable callback.
 *
 * Disables the callback specified by the callback_type.
 *
 * \param[in,out]  module      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to disable.
 */
static inline void i2c_slave_disable_callback(
		struct i2c_slave_module *const module,
		enum i2c_slave_callback callback_type)
{
	/* Sanity check. */
	Assert(module);
	Assert(module->hw);

	/* Mark callback as enabled. */
	module->enabled_callback &= ~(1 << callback_type);
}

/** @} */

/**
* \name Read and Write, Asynchronously
* @{
*/

enum status_code i2c_slave_read_packet_job(
		struct i2c_slave_module *const module,
		struct i2c_packet *const packet);

enum status_code i2c_slave_write_packet_job(
		struct i2c_slave_module *const module,
		struct i2c_packet *const packet);

/**
 * \brief Cancel the currently running operation.
 *
 * This will terminate the running transfer operation.
 *
 * \param  module Pointer to device instance structure.
 */
static inline void i2c_slave_abort_job(
		struct i2c_slave_module *const module)
{
	/* Sanity check. */
	Assert(module);
	Assert(module->hw);

	/* Set buffer to 0. */
	module->buffer_remaining = 0;
	module->buffer_length = 0;
}

/**
 * \brief Get last error from asynchronous operation.
 *
 * Will return the last error that occurred in the last asynchronous transfer
 * operation. Most errors will not be set until a new start condition has been
 * detected on the bus.
 * The status will be cleared on next operation.
 *
 * \param  module Pointer to device instance structure
 *
 * \return                     Last status code from transfer operation
 * \retval STATUS_OK           No error has occurred
 * \retval STATUS_BUSY         Transfer is in progress
 * \retval STATUS_ERR_BAD_DATA Master sent a NACK as response to last sent data
 * \retval STATUS_ERR_IO       A collision, timeout or buserror happened in the
 *                             last transfer
 * \retval STATUS_ERR_TIMEOUT  If timeout occurred.
 * \retval STATUS_ERR_OVERFLOW Data from master overflows receive buffer
 */
static inline enum status_code i2c_slave_get_job_status(
		struct i2c_slave_module *const module)
{
	/* Check sanity. */
	Assert(module);
	Assert(module->hw);

	/* Return current status code. */
	return module->status;
}

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* I2C_SLAVE_INTERRUPT_H_INCLUDED */
