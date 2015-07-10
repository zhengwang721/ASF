/**
 * \file
 *
 * \brief I2C Driver for SAMB11
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

#include <stdint.h>
#include <string.h>
#include <compiler.h>
#include <conf_i2c.h>
#include <gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_WRITE_TO_SLAVE      0
#define I2C_READ_FROM_SLAVE     1

#define I2C_MASTER_CALLBACK_MODE     (0)

/** \brief I2C Core index
 *
 * Some version of chip has multiple I2C modules.
 */
enum i2c_core_idx {
	I2C_CORE1 = 1,
#ifdef CHIPVERSION_B0
	I2C_CORE2,
#endif      //CHIPVERSION_B0
	I2C_CORE_MAX,
};

/**
 * \brief I<SUP>2</SUP>C master packet for read/write
 *
 * Structure to be used when transferring I<SUP>2</SUP>C master packets.
 */
struct i2c_master_packet {
	/** Address to slave device.  */
	uint16_t address;
	/** Length of data array. */
	uint16_t data_length;
	/** Data array containing all data to be transferred. */
	uint8_t *data;
};

/** \brief Interrupt flags
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
 * \brief I<SUP>2</SUP>C frequencies
 *
 * Values for I<SUP>2</SUP>C speeds supported by the module. 
 *
 */
enum i2c_master_baud_rate {
	/** Baud rate at 100KHz (Standard-mode). */
	I2C_MASTER_BAUD_RATE_100KHZ = 100,
	/** Baud rate at 400KHz (Fast-mode). */
	I2C_MASTER_BAUD_RATE_400KHZ = 400,
};


#if I2C_MASTER_CALLBACK_MODE == true
/**
 * \brief Callback types
 *
 * The available callback types for the I<SUP>2</SUP>C master module.
 */
enum i2c_master_callback {
	/** Callback for packet write complete. */
	I2C_MASTER_CALLBACK_WRITE_COMPLETE = 0,
	/** Callback for packet read complete. */
	I2C_MASTER_CALLBACK_READ_COMPLETE  = 1,
	/** Callback for error. */
	I2C_MASTER_CALLBACK_ERROR          = 2,
#  if !defined(__DOXYGEN__)
	/** Total number of callbacks. */
	_I2C_MASTER_CALLBACK_N             = 3,
#  endif
};

/** \brief Transfer direction
 *
 * For master: transfer direction or setting direction bit in address.
 * For slave: direction of request from master.
 */
enum i2c_transfer_direction {
	/** Master write operation is in progress. */
	I2C_TRANSFER_WRITE = 0,
	/** Master read operation is in progress. */
	I2C_TRANSFER_READ  = 1,
};

#  if !defined(__DOXYGEN__)
/** Prototype for software module. */
struct i2c_module;

/** Prototype for I2C Callback function */
typedef void (*i2c_master_callback_t)(
		struct i2c_module *const module);
#  endif
#endif

/**
 * \brief  I<SUP>2</SUP>C driver software device instance structure.
 *
 *  I<SUP>2</SUP>C driver software instance structure, used to
 * retain software state information of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct i2c_module {
#if !defined(__DOXYGEN__)
	/** Hardware instance initialized for the struct. */
	void *hw;
	/** Module lock. */
	volatile bool locked;
	/** If true, stop condition will be sent after a read/write. */
	bool no_stop;
#  if I2C_MASTER_CALLBACK_MODE == true
	/** Pointers to callback functions. */
	volatile i2c_master_callback_t callbacks[_I2C_MASTER_CALLBACK_N];
	/** Mask for registered callbacks. */
	volatile uint8_t registered_callback;
	/** Mask for enabled callbacks. */
	volatile uint8_t enabled_callback;
	/** The total number of bytes to transfer. */
	volatile uint16_t buffer_length;
	/**
	 * Counter used for bytes left to send in write and to count number of
	 * obtained bytes in read.
	 */
	volatile uint16_t buffer_remaining;
	/** Data buffer for packet write and read. */
	volatile uint8_t *buffer;
	/** Save direction of async request. 1 = read, 0 = write. */
	volatile enum i2c_transfer_direction transfer_direction;
	/** Status for status read back in error callback. */
	volatile enum status_code status;
#  endif
#endif
};


/**
 * \brief I2C module clock input 
 *
 * I2C module clock.
 *
 */
enum i2c_clock_input {
	/** source from clock input 0 26MHz*/
	I2C_CLK_INPUT_0	= 0,
	/** source from clock input 1 13MHz */
	I2C_CLK_INPUT_1,
	/** source from clock input 2 6.5MHz*/
	I2C_CLK_INPUT_2,
	/** source from clock input 3 3MHz*/
	I2C_CLK_INPUT_3,
};

/**
 * \brief Configuration structure for the I<SUP>2</SUP>C Master device
 *
 * This is the configuration structure for the I<SUP>2</SUP>C Master device. It
 * is used as an argument for \ref i2c_master_init to provide the desired
 * configurations for the module. The structure should be initialized using the
 * \ref i2c_master_get_config_defaults .
 */
struct i2c_master_config {
	/** I2C core index **/
	enum i2c_core_idx core_idx;
	/** Baud rate (in KHz) for I<SUP>2</SUP>C operations in
	 * standard-mode, Fast-mode,
	 * \ref i2c_master_baud_rate. */
	uint32_t baud_rate;
	/** CLOCK INPUT   to use as clock source. */
	enum i2c_clock_input clock_source;
	/** PAD0 (SDA) pinmux. */
	/** PAD1 (SCL) pinmux. */
	uint32_t pinmux_pad[2];
};

/**
@defgroup i2c-drv I2C Driver API

@{
*/

void i2c_enable(const struct i2c_module *const module);

void i2c_disable(const struct i2c_module *const module);

void i2c_reset(struct i2c_module *const module);

enum status_code i2c_lock(struct i2c_module *const module);

void i2c_unlock(struct i2c_module *const module);

#  if CONF_I2C_MASTER_ENABLE == true
void i2c_master_get_config_defaults(
		struct i2c_master_config *const config);

 enum status_code i2c_master_init(
		struct i2c_module *const module,
		const struct i2c_master_config *const config);

void i2c_master_reset(struct i2c_module *const module);

enum status_code i2c_master_write_address(
		struct i2c_module *const module,
		uint8_t address,
		uint8_t command);

enum status_code i2c_master_read_packet_wait(
		struct i2c_module *const module,
		struct i2c_master_packet *const packet);

enum status_code i2c_master_read_packet_wait_no_stop(
		struct i2c_module *const module,
		struct i2c_master_packet *const packet);

enum status_code i2c_master_write_packet_wait(
		struct i2c_module *const module,
		struct i2c_master_packet *const packet);

enum status_code i2c_master_write_packet_wait_no_stop(
		struct i2c_module *const module,
		struct i2c_master_packet *const packet);

void i2c_master_send_stop(struct i2c_module *const module);

void i2c_master_send_start(struct i2c_module *const module);

enum status_code i2c_master_read_byte(
		struct i2c_module *const module,
		uint8_t *byte);

enum status_code i2c_master_write_byte(
		struct i2c_module *const module,
		uint8_t byte);

#  endif

/**
 * \brief Disable driver instance
 *
 * This function disable driver instance
 *
 * \param[in,out] module Pointer to the driver instance to disable
 *
 */
static inline void _i2c_disable(I2C *const i2c_module)
{
	if (i2c_module == NULL)
		return;
	i2c_module->I2C_MODULE_ENABLE.reg = 0;
}

/**
 * \brief Enable driver instance
 *
 * This function enable driver instance
 *
 * \param[in,out] module Pointer to the driver instance to enable
 *
 */
static inline void _i2c_enable(I2C *const i2c_module)
{
	if (i2c_module == NULL)
		return;
	i2c_module->I2C_MODULE_ENABLE.reg= (1 << I2C_I2C_MODULE_ENABLE_ENABLE_Pos);
}

/**
 * \brief Returns the activity status of the module
 *
 * Returns the activity status of the module.
 *
 * \param[in]  module  Pointer to software module structure
 *
 * \return Status of the synchronization.
 * \retval true   Module is active 
 * \retval false  Module is not active
 */
static inline bool i2c_is_active (
		const struct i2c_module *const module)
{
	/* Sanity check. */
	Assert(module);
	Assert(module->hw);

	I2C *const i2c_hw = (module->hw);
	return (i2c_hw->I2C_STATUS.bit.I2C_ACTIVE);
}

/** @} */

/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_H_INCLUDED */

