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
#include "i2c_common.h"

#ifdef I2C_MASTER_ASYNC
# include <sercom_interrupts.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_i2c_master_group I2C Master Driver
 *
 * \section bab blablba
 * blalba
 *
 * blabla
 */

/**
 * \section i2c_master_api_overview API Overview
 * @{
 *
 */
#if !defined(__DOXYGEN__)
/**
 * \internal Setting direction bit in address.
 */
enum _i2c_transfer_direction {
	_I2C_TRANSFER_WRITE = 0,
	_I2C_TRANSFER_READ = 1,
};
#endif

/** \brief Interrupt flags.
 *
 * Flags used when reading or setting interrupt flags.
 */
enum i2c_master_interrupt_flag {
	/** Interrupt flag used for write. */
	I2C_MASTER_INTERRUPT_WRITE = 0,
	/** Interrupt flag used for read. */
	I2C_MASTER_INTERRUPT_READ  = 1,
};

/**
 * \brief Values for hold time after start bit.
 *
 * Values for the possible I2C master mode SDA internal hold times after start
 * bit has been sent.
 */
enum i2c_master_start_hold_time {
	/** Internal SDA hold time disabled. */
	I2C_MASTER_START_HOLD_TIME_DISABLED = SERCOM_I2CM_CTRLA_SDAHOLD(0),
	/** Internal SDA hold time 50ns-100ns. */
	I2C_MASTER_START_HOLD_TIME_50NS_100NS = SERCOM_I2CM_CTRLA_SDAHOLD(1),
	/** Internal SDA hold time 300ns-600ns. */
	I2C_MASTER_START_HOLD_TIME_300NS_600NS = SERCOM_I2CM_CTRLA_SDAHOLD(2),
	/** Internal SDA hold time 400ns-800ns. */
	I2C_MASTER_START_HOLD_TIME_400NS_800NS = SERCOM_I2CM_CTRLA_SDAHOLD(3),
};

/**
 * /brief I2C protocol frequencies.
 *
 * Values for standard I2C speeds supported by the module.
 *
 * \note Max speed is given by gclk-frequency divided by 10.
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
 *
 * The available callback types for the I2C master module.
 */
enum i2c_master_callback {
	/** Callback for packet write complete. */
	I2C_MASTER_CALLBACK_WRITE_COMPLETE = 0,
	/** Callback for packet read complete. */
	I2C_MASTER_CALLBACK_READ_COMPLETE  = 1,
	/** Callback for error. */
	I2C_MASTER_CALLBACK_ERROR          = 2,
#if !defined(__DOXYGEN__)
	/** Total number of callbacks. */
	_I2C_MASTER_CALLBACK_N             = 3,
#endif
};
#if !defined(__DOXYGEN__)
/* Prototype for device instance. */
struct i2c_master_dev_inst;

typedef void (*i2c_master_callback_t)(
		const struct i2c_master_dev_inst *const dev_inst);
#endif
#endif

/**
 * \brief SERCOM I2C Master driver hardware instance.
 *
 * Device instance structure for SERCOM I2C Master instance. This structure
 * is used throughout the driver, and should be initiated using the
 * \ref i2c_master_init() function to associate the struct with a particular
 * hardware instance and configurations.
 */
struct i2c_master_dev_inst {
	/** Hardware instance initialized for the struct. */
	Sercom *hw_dev;
	/** Unknown bus state timeout. */
	uint16_t unkown_bus_state_timeout;
	/* Buffer write timeout value. */
	uint16_t buffer_timeout;
#ifdef I2C_MASTER_ASYNC
	/** Pointers to callback functions. */
	i2c_master_callback_t callbacks[_I2C_MASTER_CALLBACK_N];
	/** Mask for registered callbacks. */
	uint8_t registered_callback;
	/** Mask for enabled callbacks. */
	uint8_t enabled_callback;
	/** The total number of bytes to transfer. */
	uint16_t buffer_length;
	/** Counter used for bytes left to send in write and to count number of
	 * obtained bytes in read. */
	uint16_t buffer_remaining;
	/** Data buffer for packet write and read. */
	uint8_t *buffer;
	/** Save direction of async request. 1 = read, 0 = write. */
	uint8_t transfer_direction;
	/** Status for status read back in error callback. */
	enum status_code status;
	/** Save if there is an ongoing async operation. */
	bool async_ongoing;
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

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	while(i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_SYNCBUSY) {
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
 * - Baudrate 100kHz
 * - GCLK generator 0
 * - Do not run in standby
 * - Start bit hold time 300ns-600ns
 * - Buffer timeout = 65535
 * - Unknown bus status timeout 65535
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
	config->start_hold_time = I2C_MASTER_START_HOLD_TIME_300NS_600NS;
	config->buffer_timeout = 65535;
	config->unkown_bus_state_timeout = 65535;
}

enum status_code i2c_master_init(struct i2c_master_dev_inst *const dev_inst,
		Sercom *const module,
		const struct i2c_master_conf *const config);

/**
 * \brief Enable the I2C module.
 *
 * This will enable the requested I2C module.
 *
 * \param[in]  dev_inst Pointer to the device instance struct.
 */
static inline void i2c_master_enable(
		const struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check of arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Timeout counter used to force bus state. */
	volatile uint16_t timeout_counter = 0;

	/* Wait for module to sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Enable module. */
	i2c_module->CTRLA.reg |= SERCOM_I2CM_CTRLA_ENABLE;

	/* Start timeout if bus state is unknown. */
	while (i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE(0)) {
		timeout_counter++;
		if(timeout_counter >= (dev_inst->unkown_bus_state_timeout)) {
			/* Timeout, force bus state to idle. */
			i2c_module->STATUS.reg = SERCOM_I2CM_STATUS_BUSSTATE(1);
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
 */
static inline void i2c_master_disable(
		const struct i2c_master_dev_inst *const dev_inst)
{
	/* Sanity check of arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	/* Wait for module to sync. */
	_i2c_master_wait_for_sync(dev_inst);

	/* Disable module. */
	i2c_module->CTRLA.reg &= ~SERCOM_I2CM_CTRLA_ENABLE;
}

void i2c_master_reset(struct i2c_master_dev_inst *const dev_inst);

/** @} */

/**
* \name Interrupts and Flags
* @{
*/

/**
 * \brief Check interrupt flag.
 *
 * Check status of the requested interrupt flag.
 *
 * \param[in]  dev_inst       Pointer to device instance struct.
 * \param[in]  interrupt_flag Value of interrupt flag to check.
 */
static inline bool i2c_master_is_interrupt_flag_set(
		const struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	return (i2c_module->INTFLAG.reg & (1 << interrupt_flag));
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

	SercomI2cm *const i2c_module = &(dev_inst->hw_dev->I2CM);

	i2c_module->INTFLAG.reg = (1 << interrupt_flag);
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
/** \subpage i2c_master_async */
/** @} */


#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_H_INCLUDED */