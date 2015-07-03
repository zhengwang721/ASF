/**
 ****************************************************************************************
 *
 * @file i2c.h
 *
 * @brief SAMB11 I2C API for Applications
 *
 * This module contains the public API and the necessary enumerations and structures that are required for 
 * SAMB11 Application Developers using I2C module
 *
 *
 *  Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. The name of Atmel may not be used to endorse or promote products derived from this software 
 *  without specific prior written permission.
 *
 *  4. This software may only be redistributed and used in connection with an Atmel microcontroller product.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 *  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 *  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************************
 */

#ifndef I2C_MASTER_H_INCLUDED
#define I2C_MASTER_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_MASTER_CALLBACK_MODE	(0)

/** \brief I2C Core index
 *
 * Some version of chip has multiple I2C modules.
 */
enum i2c_core_idx {
	I2C_CORE1	= 1,
#ifdef CHIPVERSION_B0
	I2C_CORE2,
#endif	//CHIPVERSION_B0
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
struct i2c_master_module;

/** Prototype for I2C Callback function */
typedef void (*i2c_master_callback_t)(
		struct i2c_master_module *const module);
#  endif
#endif

/**
 * \brief I<SUP>2</SUP>C Status codes.
 *
 * Status codes of I2C drivers.
 */
enum i2c_status_code {
	I2C_STATUS_OK = 0,
	I2C_STATUS_BUSY,
	I2C_STATUS_ERR_IO,
	I2C_STATUS_ERR_TIMEOUT,
	I2C_STATUS_ERR_OVERFLOW,
	I2C_STATUS_ERR_NOT_INITIALIZED,
	I2C_STATUS_ERR_DENIED,
	I2C_STATUS_ERR_INVALID_ARG,
	I2C_STATUS_ERR_UNSUPPORTED_DEV,
	I2C_STATUS_ERR_BAUDRATE_UNAVAILABLE,
	I2C_STATUS_ERR_ALREADY_INITIALIZED,
};

/**
 * \brief  I<SUP>2</SUP>C Master driver software device instance structure.
 *
 *  I<SUP>2</SUP>C Master driver software instance structure, used to
 * retain software state information of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct i2c_master_module {
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
	volatile enum i2c_status_code status;
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
	uint16_t pinmux_pad[2];
};

/**
@defgroup i2c-drv I2C Driver API

@{
*/

/**
 * \name Lock/Unlock
 * @{
 */

/**
 * \brief Attempt to get lock on driver instance
 *
 * This function checks the instance's lock, which indicates whether or not it
 * is currently in use, and sets the lock if it was not already set.
 *
 * The purpose of this is to enable exclusive access to driver instances, so
 * that, e.g., transactions by different services will not interfere with each
 * other.
 *
 * \param[in,out] module Pointer to the driver instance to lock
 *
 * \retval I2C_STATUS_OK If the module was locked
 * \retval I2C_STATUS_BUSY If the module was already locked
 */
enum i2c_status_code i2c_master_lock(
		struct i2c_master_module *const module);

/**
 * \brief Unlock driver instance
 *
 * This function clears the instance lock, indicating that it is available for
 * use.
 *
 * \param[in,out] module Pointer to the driver instance to lock
 *
 */
void i2c_master_unlock(struct i2c_master_module *const module);

/** @} */


/**
 * \name Configuration and Initialization
 * @{
 */

/**
 * \brief Gets the I<SUP>2</SUP>C master default configurations
 *
 * Use to initialize the configuration structure to known default values.
 *
 * The default configuration is as follows:
 * - Baudrate 100KHz
 * - CLOCK INPUT   0
 * - PINMUX_DEFAULT for  pads
 *
 * \param[out] config  Pointer to configuration structure to be initiated
 */
void i2c_master_get_config_defaults(
		struct i2c_master_config *const config);


/**
 * \brief Initializes the requested I<SUP>2</SUP>C hardware module
 *
 * Initializes the  I<SUP>2</SUP>C master device requested and sets the provided
 * software module struct. Run this function before any further use of
 * the driver.
 *
 * \param[out] module  Pointer to software module struct
 * \param[in]  config  Pointer to the configuration struct
 *
 * \return Status of initialization.
 * \retval I2C_STATUS_OK                        Module initiated correctly
 * \retval I2C_STATUS_ERR_INVALID_ARG           Invalid argument in module or config structure.
 * \retval I2C_STATUS_ERR_ALREADY_INITIALIZED   If the pinmux is not a valid one for I2C signals.
 *
 */
 enum i2c_status_code i2c_master_init(
		struct i2c_master_module *const module,
		const struct i2c_master_config *const config);


/**
 * \brief Resets the hardware module
 *
 * Reset the module to hardware defaults.
 *
 * \param[in,out] module Pointer to software module structure
 */
void i2c_master_reset(struct i2c_master_module *const module);

/** @} */

/**
* \name Read and Write
* @{
*/

/**
 * \brief Write Address & command to slave
 *
 * \param[in,out]  module  	Pointer to software module struct
 * \param[in]      address  Address of slave
 * \param[in]			 command	Command 
 *													0 - Write, 1 - Read
 *
 * \return Status of writing byte.
 * \retval I2C_STATUS_OK   The Address and command was written successfully
 */
enum i2c_status_code i2c_master_write_address(
		struct i2c_master_module *const module,
		uint8_t address,
		uint8_t command);

/**
 * \brief Reads a packet from I2C device.
 *
 * Reads a I2C packet from the address device with specified amount of data.
 *
 * \param[in,out] module 	Pointer to software module structure
 * \param[in] packet			Pointer to i2c_master_packet structure
 *
 * \return Status of reading packet.
 * \retval I2C_STATUS_OK                    The packet was read successfully
 * \retval I2C_STATUS_ERR_INVALID_ARG       Invalid argument in module or config strucuture
 *                                      		specified timeout period
 * \retval I2C_STATUS_BUSY            			If module has a pending request.
 */
enum i2c_status_code i2c_master_read_packet_wait(
		struct i2c_master_module *const module,
		struct i2c_master_packet *const packet);


/**
 * \brief Reads a packet from I2C device and doest not send a stop at the end.
 *
 * Reads a I2C packet from the address device with specified amount of data and 
 * doesn't send the stop at the end. Used in case of multiple read transfers or
 * reading after sending a write command. 
 *
 * \note After completing the transfer needs to send stop using
 * 				\ref i2c_master_send_stop
 *
 * \param[in,out] module 	Pointer to software module structure
 * \param[in] packet			Pointer to i2c_master_packet structure
 *
 * \return Status of reading packet.
 * \retval I2C_STATUS_OK                    The packet was read successfully
 * \retval I2C_STATUS_ERR_INVALID_ARG       Invalid argument in module or config strucuture
 *                                      		specified timeout period
 * \retval I2C_STATUS_BUSY            			If module has a pending request.
 */
enum i2c_status_code i2c_master_read_packet_wait_no_stop(
		struct i2c_master_module *const module,
		struct i2c_master_packet *const packet);

		
/**
 * \brief Writes a packet from I2C device and does send a stop at the end.
 *
 * Writes a I2C packet from the address device with specified amount of data and 
 * sends the stop at the end.  
 *
 * \param[in,out] module 	Pointer to software module structure
 * \param[in] packet			Pointer to i2c_master_packet structure
 *
 * \return Status of reading packet.
 * \retval I2C_STATUS_OK                    The packet was read successfully
 * \retval I2C_STATUS_ERR_INVALID_ARG       Invalid argument in module or config strucuture
 *                                      		specified timeout period
 * \retval I2C_STATUS_BUSY            			If module has a pending request.
 */
enum i2c_status_code i2c_master_write_packet_wait(
		struct i2c_master_module *const module,
		struct i2c_master_packet *const packet);

/**
 * \brief Writes a packet from I2C device and doesn't send a stop at the end.
 *
 * Writes a I2C packet from the address device with specified amount of data and 
 * doesn't sends the stop at the end of write.
 *
 * \note After completing the transfer needs to send stop using
 * 				\ref i2c_master_send_stop
 *
 * \param[in,out] module 	Pointer to software module structure
 * \param[in] packet			Pointer to i2c_master_packet structure
 *
 * \return Status of reading packet.
 * \retval I2C_STATUS_OK                    The packet was read successfully
 * \retval I2C_STATUS_ERR_INVALID_ARG       Invalid argument in module or config strucuture
 *                                      		specified timeout period
 * \retval I2C_STATUS_BUSY            			If module has a pending request.
 */
enum i2c_status_code i2c_master_write_packet_wait_no_stop(
		struct i2c_master_module *const module,
		struct i2c_master_packet *const packet);

/**
 * \brief Sends the stop signaling on the I2C bus.
 *
 * Sends the stop signaling on the I2C bus after previous read or write 
 * transfer that was sent without stop.
 *
 * \param[in,out] module 	Pointer to software module structure
 */
void i2c_master_send_stop(struct i2c_master_module *const module);

/**
 * \brief Sends start condition on bus
 *
 * Sends a start condition on bus.
 *
 * \note This function can only be used after the
 * \ref i2c_master_write_packet_wait_no_stop function. If a stop condition
 * is to be sent after a read, the \ref i2c_master_read_packet_wait
 * function must be used.
 *
 * \param[in,out] module  Pointer to the software instance struct
 */
void i2c_master_send_start(struct i2c_master_module *const module);
/**
 * \brief Reads a byte from I2C device.
 *
 * Reads a byte of data from the device addressed by the previous transfer.
 * It doesn't send the stop signaling at the end of reading. It can be used
 * as part of a sequence of transfer.
 *
 * \note After completing the transfer needs to send stop using
 * 				\ref i2c_master_send_stop
 *
 * \param[in] module 		Pointer to software module structure
 * \param[out] byte			Pointer to store the data read from I2C device.
 *
 * \return Status of reading packet.
 * \retval I2C_STATUS_OK                    A byte was read successfully
 */
enum i2c_status_code i2c_master_read_byte(
		struct i2c_master_module *const module,
		uint8_t *byte);

/**
 * \brief Writes a byte to I2C device.
 *
 * Writes a byte of data to the device addressed by the previous transfer.
 * It doesn't send the stop signaling at the end of reading. It can be used
 * as part of a sequence of transfer.
 *
 * \note After completing the transfer needs to send stop using
 * 				\ref i2c_master_send_stop
 *
 * \param[in] module 	Pointer to software module structure
 * \param[in] byte		Data byte to be written to the I2C.
 *
 * \return Status of reading packet.
 * \retval I2C_STATUS_OK                    A byte was written successfully
 */
enum i2c_status_code i2c_master_write_byte(
		struct i2c_master_module *const module,
		uint8_t byte);

/** @} */
		
/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_H_INCLUDED */

