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
 *    this list of conditions and the followingma disclaimer in the documentation
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

#ifndef I2C_SLAVE_ASYNC_H_INCLUDED
#define I2C_SLAVE_ASYNC_H_INCLUDED


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
#ifdef I2C_SLAVE_ASYNC
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
	/** Callback for error. */
	I2C_SLAVE_CALLBACK_ERROR,
#if !defined(__DOXYGEN__)
	/** Total number of callbacks. */
	_I2C_SLAVE_CALLBACK_N,
#endif
};
#if !defined(__DOXYGEN__)

typedef void (*i2c_slave_callback_t)(
		const struct i2c_slave_dev_inst *const dev_inst);
#endif
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
	I2C_SLAVE_SDA_HOLD_TIME_DISABLED = SERCOM_I2CS_CTRLA_SDAHOLD(0),
	/** SDA hold time 50ns-100ns */
	I2C_SLAVE_SDA_HOLD_TIME_50NS_100NS = SERCOM_I2CS_CTRLA_SDAHOLD(1),
	/** SDA hold time 300ns-600ns */
	I2C_SLAVE_SDA_HOLD_TIME_300NS_600NS = SERCOM_I2CS_CTRLA_SDAHOLD(2),
	/** SDA hold time 400ns-800ns */
	I2C_SLAVE_SDA_HOLD_TIME_400NS_800NS = SERCOM_I2CS_CTRLA_SDAHOLD(3),
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
 
 /** \brief Interrupt flags.
 *
 * Flags used when reading or setting interrupt flags.
 */
enum i2c_slave_interrupt_flag {
	/** Interrupt flag for stop condition */
	I2C_SLAVE_INTERRUPT_STOP = 0,
	/** Interrupt flag for address match */
	I2C_SLAVE_INTERRUPT_ADDRESS  = 1,
	/** Interrupt flag for data */
	I2C_SLAVE_INTERRUPT_DATA  = 1,
};
 
 /**
 * \brief SERCOM I2C Slave driver hardware instance
 *
 * Device instance structure for SERCOM I2C Slave instance. This structure
 * is used throughout the driver, and should be initiated using the
 * \ref i2c_slave_init() function to associate the struct with a particular
 * hardware instance and configurations.
 */
struct i2c_slave_dev_inst {
	/** Hardware instance initialized for the struct. */
	Sercom *hw_dev;
	/** Unknown bus state timeout. */
	uint16_t unkown_bus_state_timeout;
	/* Buffer write timeout value. */
	uint16_t buffer_timeout;
#ifdef I2C_SLAVE_ASYNC
	/** Pointers to callback functions. */
	volatile i2c_slave_callback_t callbacks[_I2C_MASTER_SLAVE_N];
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
#endif
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
	/** 
	 * Set to enable acknowledge action to be sent immediately after reading
	 * incoming data
	 */
	bool smart_mode_enable;
	/** Address or upper limit of address range */
	uint8_t address;
	/** Address mask, second address or lower limit of address range*/
	uint8_t address;
	/** GCLK generator to use as clock source. */
	enum gclk_generator generator_source;
	/** Bus hold time after start signal on data line. */
	enum i2c_slave_start_hold_time start_hold_time;
	/** Unknown bus state timeout. */
	uint16_t unkown_bus_state_timeout;
	/** Timeout for packet write to wait for slave. */
	uint16_t buffer_timeout;
	/** Set to keep module active in sleep modes. */
	bool run_in_standby;
};

/**
 * \brief Get the I2C slave default configurations.
 *
 * Use to initialize the configuration structure to known default values. This
 * function should be called at the start of any I2C initiation.
 *
 * The default configuration is as follows:
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
	config->generator_source = GCLK_GENERATOR_0;
	config->run_in_standby = false;
	config->sda_hold_time = I2C_SLAVE_SDA_HOLD_TIME_300NS_600NS;
}

enum status_code i2c_slave_init(struct i2c_slave_dev_inst *const dev_inst,
		Sercom *const module,
		const struct i2c_slave_conf *const config);

/**
 * \brief Enable the I2C module.
 *
 * This will enable the requested I2C module and set the bus state to IDLE after the specified
 * \ref timeout "timeout" period if no stop bit is detected.
 *
 * \param[in]  dev_inst Pointer to the device instance struct.
 */
static inline void i2c_slave_enable(
		const struct i2c_slave_dev_inst *const dev_inst)
{
	/* Sanity check of arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SercomI2cs *const i2c_module = &(dev_inst->hw_dev->I2CS);

	/* Timeout counter used to force bus state. */
	volatile uint16_t timeout_counter = 0;

	/* Wait for module to sync. */
	_i2c_slave_wait_for_sync(dev_inst);

	/* Enable module. */
	i2c_module->CTRLA.reg |= SERCOM_I2CS_CTRLA_ENABLE;
}

/**
 * \brief Disable the I2C module.
 *
 * This will disable the I2C module specified in the provided device instance
 * structure.
 *
 * \param[in]  dev_inst Pointer to the device instance struct.
 */
static inline void i2c_slave_disable(
		const struct i2c_slave_dev_inst *const dev_inst)
{
	/* Sanity check of arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SercomI2cs *const i2c_module = &(dev_inst->hw_dev->I2CS);

	/* Wait for module to sync. */
	_i2c_slave_wait_for_sync(dev_inst);

	/* Disable module. */
	i2c_module->CTRLA.reg &= ~SERCOM_I2CS_CTRLA_ENABLE;
}

void i2c_slave_reset(struct i2c_slave_dev_inst *const dev_inst);


/**
 * \name Callbacks
 * @{
 */
#if !defined(__DOXYGEN__)
void _i2c_slave_async_callback_handler(uint8_t instance);
#endif

void i2c_slave_async_register_callback(
		struct i2c_slave_dev_inst *const dev_inst,
		i2c_slave_callback_t callback,
		enum i2c_slave_callback callback_type);

void i2c_slave_async_unregister_callback(
		struct i2c_slave_dev_inst *const dev_inst,
		enum i2c_slave_callback callback_type);

/**
 * \brief Enable callback.
 *
 * Enables the callback specified by the callback_value.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to enable.
 */
static inline void i2c_slave_async_enable_callback(
		struct i2c_slave_dev_inst *const dev_inst,
		enum i2c_slave_callback callback_type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Mark callback as enabled. */
	dev_inst->enabled_callback = (1 << callback_type);
}


/**
 * \brief Disable callback.
 *
 * Disables the callback specified by the callback_type.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to disable.
 */
static inline void i2c_slave_async_disable_callback(
		struct i2c_slave_dev_inst *const dev_inst,
		enum i2c_slave_callback callback_type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Mark callback as enabled. */
	dev_inst->enabled_callback &= ~(1 << callback_type);
}

/** @} */

/**
* \name Read and Write, Asynchronously
* @{
*/

enum status_code i2c_slave_async_read_packet(
		struct i2c_slave_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

enum status_code i2c_slave_async_write_packet(
		struct i2c_slave_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

/**
 * \brief Cancel the currently running operation.
 *
 * This will terminate the running transfer operation.
 *
 * \param  dev_inst Pointer to device instance structure.
 */
static inline void i2c_slave_async_cancel_transfer(
		struct i2c_slave_dev_inst *const dev_inst)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Set buffer to 0. */
	dev_inst->buffer_remaining = 0;
}

/**
 * \brief Get last error from asynchronous operation.
 *
 * Will return the last error that occurred in asynchronous transfer operation. The
 * status will be cleared on next operation.
 *
 * \param  dev_inst Pointer to device instance structure.
 *
 * \return          Last status code from transfer operation.
 * \retval STATUS_OK No error has occurred.
 * \retval STATUS_IN_PROGRESS If transfer is in progress.
 * \retval STATUS_ERR_BUSY If slave module is busy.
 * \retval STATUS_ERR_DENIED If error on bus.
 * \retval STATUS_ERR_PACKET_COLLISION If arbitration is lost.
 * \retval STATUS_ERR_BAD_ADDRESS If slave is busy, or no slave acknowledged the
 *                                address.
 * \retval STATUS_ERR_TIMEOUT If timeout occurred.
 * \retval STATUS_ERR_OVERFLOW If slave did not acknowledge last sent data,
 *                             indicating that slave do not want more data.
 */
static inline enum status_code i2c_slave_async_get_operation_status(
		struct i2c_slave_dev_inst *const dev_inst)
{
	/* Check sanity. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Return current status code. */
	return dev_inst->status;
}

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* I2C_SLAVE_ASYNC_H_INCLUDED */
