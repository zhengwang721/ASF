/**
 * \file
 *
 * \brief SAMD20 I2C Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012-2013-2013 Atmel Corporation. All rights reserved.
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

#ifndef I2C_MASTER_ASYNC_H_INCLUDED
#define I2C_MASTER_ASYNC_H_INCLUDED

#include "i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_i2c_master_group_async I2C Master Asynchronous
 * @{
 *
 * This is an overview of the API for the asynchronous I2C master
 * driver.
 *
 * The user will still have to use the initialization from the basic use part
 * of the driver, which can be found \ref sam0_i2c_master_group "here". When the
 * asynchronous driver is included in the project, an asynchronous part in the basic driver
 * will be unlocked. This will allow the user to implement the driver in the same way as
 * for the basic polled driver, but will additionally get the asynchronous functions
 * documented below.
 *
 * \section i2c_master_async_api API Overview
 *
 */

/**
 * \name Callbacks
 * @{
 */
#if !defined(__DOXYGEN__)
void _i2c_master_async_callback_handler(uint8_t instance);
#endif

void i2c_master_async_register_callback(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_master_callback_t callback,
		enum i2c_master_callback callback_type);

void i2c_master_async_unregister_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback callback_type);

/**
 * \brief Enable callback.
 *
 * Enables the callback specified by the callback_value.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to enable.
 */
static inline void i2c_master_async_enable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback callback_type)
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
static inline void i2c_master_async_disable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback callback_type)
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

enum status_code i2c_master_async_read_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

enum status_code i2c_master_async_write_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

/**
 * \brief Cancel the currently running operation.
 *
 * This will terminate the running transfer operation.
 *
 * \param[in,out] dev_inst Pointer to device instance structure.
 */
static inline void i2c_master_async_cancel_transfer(
		struct i2c_master_dev_inst *const dev_inst)
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
 * \param[in,out] dev_inst Pointer to device instance structure.
 *
 * \return Last status code from transfer operation.
 * \retval STATUS_OK No error has occurred.
 * \retval STATUS_BUSY If transfer is in progress.
 * \retval STATUS_BUSY If master module is busy.
 * \retval STATUS_ERR_DENIED If error on bus.
 * \retval STATUS_ERR_PACKET_COLLISION If arbitration is lost.
 * \retval STATUS_ERR_BAD_ADDRESS If slave is busy, or no slave acknowledged the
 *                                address.
 * \retval STATUS_ERR_TIMEOUT If timeout occurred.
 * \retval STATUS_ERR_OVERFLOW If slave did not acknowledge last sent data,
 *                             indicating that slave do not want more data.
 */
static inline enum status_code i2c_master_async_get_operation_status(
		struct i2c_master_dev_inst *const dev_inst)
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

#endif /* I2C_MASTER_ASYNC_H_INCLUDED */
