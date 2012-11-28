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

#ifndef I2C_MASTER_H_INCLUDED
#define I2C_MASTER_H_INCLUDED

#include <sercom.h>
#include <i2c_common.h>

#ifdef I2C_MASTER_ASYNC
# include <sercom_interrupts.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_i2c_master_group SAM0+ I2C Master
 *
 * This is that!
 *
 * \section i2c_master_intro Introduction
 * This is an intro!
 *
 * \section i2c_master_api_overview API Overview
 * @{
 */

enum i2c_master_interrupt_flag {
	I2C_MASTER_INTERRUPT_WRITE = 0,
	I2C_MASTER_INTERRUPT_READ  = 1,
};

enum i2c_master_status_flag {
	I2C_MASTER_STATUS_ = 0,
};

/** Enum for the possible I2C master mode SDA internal hold times after start
 * bit has been sent. */
enum i2c_master_start_hold_time {
	/** Internal SDA hold time disabled. */
	I2C_MASTER_START_HOLD_TIME_DISABLED = 0,
	/** Internal SDA hold time 50ns-100ns. */
	I2C_MASTER_START_HOLD_TIME_50NS_100NS = 1,
	/** Internal SDA hold time 300ns-600ns. */
	I2C_MASTER_START_HOLD_TIME_300NS_600NS = 2,
	/** Internal SDA hold time 400ns-800ns. */
	I2C_MASTER_START_HOLD_TIME_400NS_800NS = 3,
};

/**
 * Values for standard I2C speeds.
 */
enum i2c_master_baud_rate {
	/** Baud rate at 100kHz. */
	I2C_MASTER_BAUD_RATE_100KHZ = 100,
	/** Baud rate at 400kHz. */
	I2C_MASTER_BAUD_RATE_400KHZ = 400,
};

#ifdef I2C_MASTER_ASYNC
/**
 * \brief Callback types.
 * The available callback types for the I2C master module.
 */
enum i2c_master_callback_type {
	I2C_MASTER_CALLBACK_WRITE_COMPLETE = 0,
	I2C_MASTER_CALLBACK_READ_COMPLETE  = 1,
	I2C_MASTER_CALLBACK_ERROR          = 2,
	_I2C_MASTER_CALLBACK_N             = 3,
};


struct i2c_master_dev_inst;

typedef void (*i2c_master_callback_t)(
		const struct i2c_master_dev_inst *const dev_inst);
#endif

/**
 * \brief SERCOM I2C Master driver hardware instance.
 *
 * Device instance structure for SERCOM I2C Master instance. This structure
 * is used throughout the driver, and should be initiated using the
 * /ref i2c_master_init() function to associate the struct with a particular
 * hardware instance and configurations.
 */
struct i2c_master_dev_inst {
	/** Hardware instance initialized for the struct. */
	SERCOM_t *hw_dev;
	/** Data buffer for packet write and read. */
	uint8_t *buffer;
	/** Counter used for bytes left to send in write and to count number of
	  * obtained bytes in read. */
	uint16_t buffer_length;
	/** Unknown bus state timeout. */
	uint16_t unkown_bus_state_timeout;
	/* Buffer write timeout value. */
	uint16_t buffer_timeout;
#ifdef I2C_MASTER_ASYNC
	/** Holder for callback functions. */
	i2c_master_callback_t callbacks[_I2C_MASTER_CALLBACK_N];
	/** Holder for registered callbacks. */
	uint8_t registered_callback;
	/** Holder for enabled callbacks. */
	uint8_t enabled_callback;
	/** Save direction of async request. 1 = read, 0 = write. */
	bool transfer_direction;
	/** Status for status read back in error callback. */
	enum status_code status;
#endif
};


/**
 * \brief Configuration structure for the I2C device.
 *
 * This is the configuration structure for the I2C Master device. It is used
 * as an argument for \ref i2c_master_init to provide the desired
 * configurations for the module. The structure should be initiated using the
 * \ref i2c_master_get_config_defaults .
 */
struct i2c_master_conf {
	/** Baud rate for I2C operations. */
	enum i2c_master_baud_rate baud_rate;
	/** GCLK generator to use as clock source. */
	enum gclk_generator generator_source;
	/** Bus hold time after start signal on data line. */
	enum i2c_master_start_hold_time start_hold_time;
	/** Unknown bus state timeout. */
	uint16_t unkown_bus_state_timeout;
	/** Timeout for packet write to wait for slave. */
	uint16_t buffer_timeout;
	/** Set to keep module active in sleep modes. */
	bool run_in_standby;
};

/**
 * \name Configuration and Initialization
 * @{
 */

#if !defined(__DOXYGEN__)
/**
 * \internal Wait for hardware module to sync.
 * \param[in]  dev_inst Pointer to device instance structure.
 */

static void _i2c_master_wait_for_sync(
		const struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	while(i2c_module->STATUS & I2C_SYNC_BUSY_Msk) {
		/* Wait for I2C module to sync. */
	}
}
#endif

/**
 * \brief Get the I2C master default configurations.
 *
 * Use to initialize the configuration structure to known default values. This
 * function should be called at the start of any I2C initiation.
 *
 * The default configuration is as follows:
 * - Blabla!
 *
 * \param[out] config Pointer to configuration structure to be initiated.
 */
static inline void i2c_master_get_config_defaults(
		struct i2c_master_conf *const config)
{
	/*Sanity check argument. */
	Assert(config);
	config->baud_rate = I2C_MASTER_BAUD_RATE_100KHZ;
	config->generator_source = GCLK_GENERATOR_0;
	config->run_in_standby = false;
	config->start_hold_time = I2C_MASTER_START_HOLD_TIME_50NS_100NS;
	config->buffer_timeout = 1000;
	config->unkown_bus_state_timeout = 1000;
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
 * \return                 [description]
 */
enum status_code i2c_master_init(struct i2c_master_dev_inst *const dev_inst,
		SERCOM_t *const module,
		const struct i2c_master_conf *const config);

/**
 * \brief Enable the I2C module.
 *
 * This will enable the requested I2C module.
 *
 * \param[in]  dev_inst Pointer to the device instance struct.
 * \return          [description]
 */
static inline void i2c_master_enable(
		const struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check of arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Timeout counter used to force bus state. */
	uint16_t timeout_counter = 0;

	/* Wait for module to sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Enable module. */
	i2c_module->CTRLA |= ( 1 << I2C_MASTER_ENABLE_Pos );

	/* Start timeout if bus state is unknown. */
	while (i2c_module->STATUS & I2C_MASTER_BUSSTATE_UNKNOWN) {
		if(++timeout_counter >= (dev_inst->unkown_bus_state_timeout)) {
			/* Timeout, force bus state to idle. */
			i2c_module->STATUS = I2C_MASTER_BUSSTATE_IDLE;
		}
	}
}

/**
 * \brief Disable the I2C module.
 *
 * This will disable the I2C module specified in the provided device instance
 * structure.
 *
 * \param[in]  dev_inst Pointer to the device instance struct.
 * \return          [description]
 */
static inline void i2c_master_disable(
		const struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check of arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Wait for module to sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Disable module. */
	i2c_module->CTRLA &= ~(1 << I2C_MASTER_ENABLE_Pos);
}

void i2c_master_reset(struct i2c_master_dev_inst *const dev_inst);

/** @} */

/**
* \name Interrupts and Flags
* @{
*/

/**
 * \brief Check given status flag.
 *
 * This will return the status of the requested status flag.
 * \param  dev_inst    Pointer to device instance structure.
 * \param  status_flag Status flag to check.
 */
static inline bool i2c_master_is_status_flag_set(
		const struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_status_flag status_flag)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Check requested status flag. */
	return (i2c_module->STATUS & (1 << status_flag));
}

/**
 * \brief Clear given status flag.
 *
 * This will clear the status flag specified.
 * \param dev_inst    Pointer to device instance structure.
 * \param status_flag Status flag to clear.
 */
static inline void i2c_master_clear_status_flag(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_status_flag status_flag)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	/* Clear requested status flag. */
	i2c_module->STATUS = (1 << status_flag);
}

/**
 * \brief Check interrupt flag.
 *
 * Check status of the requested interrupt flag.
 *
 * \param[in]  dev_inst       Pointer to device instance struct.
 * \param[in]  interrupt_flag Value of interrupt flag to check.
 *
 * \return Status of the requested interrupt flag.
 */
static inline bool i2c_master_is_interrupt_flag_set(
		const struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	return (i2c_module->INTFLAGS & (1 << interrupt_flag));
}

/**
 * \brief Clear interrupt flag.
 *
 * Clear the specified interrupt flag.
 *
 * \param[out] dev_inst       Pointer to device instance struct.
 * \param[in]  interrupt_flag Value of interrupt flag to clear.
 */
static inline void i2c_master_clear_interrupt_flag(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_I2C_MASTER_t *const i2c_module = &(dev_inst->hw_dev->I2C_MASTER);

	i2c_module->INTFLAGS = (1 << interrupt_flag);
}
/** @} */

/**
* \name Read and Write
* @{
*/

enum status_code i2c_master_read_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

enum status_code i2c_master_write_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);


/** @} */
/** @} */


#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_H_INCLUDED */