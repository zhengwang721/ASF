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

#ifndef I2C_MASTER_ASYNC_H_INCLUDED
#define I2C_MASTER_ASYNC_H_INCLUDED

#include <i2c_master.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name Callbacks
 * @{
 */

void _i2c_master_callback_handler(uint8_t instance);

/**
 * \brief Register callback for the specified callback type.
 *
 * When called, the given callback function will be associated with the
 * specified callback type.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]  callback      Pointer to the function desired for the specified
 *                       callback.
 * \param[in]  callback_type Specifies the callback type to register.
 * \return          [description]
 */
enum status_code i2c_master_register_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type);

/**
 * \brief Unregister callback for the specified callback type.
 *
 * When called, the currently registered callback for the given callback type
 * will be removed.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Specifies the callback type to unregister.
 * \return          [description]
 */
enum status_code i2c_master_unregister_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type);

/**
 * \brief Enable callback.
 * Enables the callback specified the callback_value.
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to enable.
 * \return               [description]
 */
enum status_code i2c_master_enable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type);

/**
 * \brief Disable callback.
 * Disables the callback specified by the callback_type.
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to disable.
 * \return               [description]
 */
enum status_code i2c_master_disable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type);

/** @} */

/**
* \name Read and Write, Asynchronously
* @{
*/

/**
 * \brief Read data packet from slave asynchronous.
 *
 * Reads a data packet from the specified slave address on the I2C bus. This
 * is the non-blocking equivalent of \ref i2c_master_read .
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out] packet    Pointer to I2C packet to transfer.
 * \return          [description]
 */
enum status_code i2c_master_read_packet_async(
		const struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

/**
 * \brief Write data packet to slave asynchronous.
 *
 * Writes a data packet to the specified slave address on the I2C bus. This
 * is the non-blocking equivalent of \ref i2c_master_write .
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out] packet    Pointer to I2C packet to transfer.
 * \return          [description]
 */
enum status_code i2c_master_write_packet_async(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

/**
 * \brief Cancel the currently running operation.
 * This will terminate the running write or read transfer.
 *
 * \param  dev_inst Pointer to device instance struct.
 * \return          [description]
 */
enum status_code i2c_master_cancel_operation(
		struct i2c_master_dev_inst *const dev_inst);

/**
 * \brief Check if a started transfer is done.
 * This will return the current status of the read/write transfer.
 *
 * \param  dev_inst Pointer to the device instance struct.
 * \return          [description]
 */
enum status_code i2c_master_is_transfer_done(
		struct i2c_master_dev_inst *const dev_inst);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_ASYNC_H_INCLUDED */