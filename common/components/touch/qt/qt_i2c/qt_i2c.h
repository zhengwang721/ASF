/**
 * \file
 *
 * \brief QTouch component driver with I2C interface.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#ifndef QT_I2C_H_INCLUDED
#define QT_I2C_H_INCLUDED

#include "compiler.h"
#include "status_codes.h"
#include "twi_master.h"
#include "conf_qt_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup qt_device_i2c_group QTouch device driver for I2C interface
 *
 * See \ref qt_device_i2c_quickstart.
 *
 * This component driver provides low-level read/write access to the
 * configuration and data registers on a QTouch device. It has only been
 * tested on AT42QT2160 parts, but it should be easy to add support for other
 * QTouch devices with I2C interface.
 *
 * @{
 */

/**
 *  \defgroup qt_device_i2c_support_list QT Device list support by the driver
 *  @{
 */
#define QT2160    1
/** @}  */

#if (CONF_QT_DEVICE == QT2160)
	#include "QT2160.h"
#else
	#error "Device not supported"
#endif

/** TWI host base address */
#define QT_TWI_BASE_ADDRESS  CONF_QT_TWI_BASE_ADDRESS
/** TWI slave memory address */
#define QT_I2C_ADDRESS       CONF_QT_I2C_ADDRESS
/** TWI data transfer rate for QT device */
#define QT_I2C_SPEED         CONF_QT_I2C_SPEED

/** Invalid pin index (used for RESET and CHANGE pin definition) */
#define QT_PIN_IDX_INVALID   0xFFFFu
/** RESET pin index for QT device */
#define QT_RESET_PIN_IDX     CONF_QT_RESET_PIN_IDX
/** CHANGE pin index for QT device */
#define QT_CHANGE_PIN_IDX    CONF_QT_CHANGE_PIN_IDX

/**
 * \brief Initialize I2C communication interface.
 */
void qt_init_interface(void);

/**
 * \brief Perform a hardware reset of the QT device.
 *
 * \note If RESET pin defined, it will perform a hardware reset,
 *       Otherwise it will do nothing.
 */
void qt_hardware_reset(void);

/**
 * \brief Check communication is ready and able to read Chip ID.
 *
 * \retval STATUS_OK  If communication is ready.
 * \retval ERR_IO_ERROR If communication failed with the device.
 * \retval ERR_UNSUPPORTED_DEV If the device is not supported.
 */
enum status_code qt_get_comm_ready(void);

/**
 * \brief Read entire setup block from QT device.
 *
 * \param setup_block Pointer to setup block buffer for read.
 *
 * \retval STATUS_OK  If read successful.
 * \retval ERR_IO_ERROR If communication failed with the device.
 */
enum status_code qt_read_setup_block(struct qt_setup_block *setup_block);

/**
 * \brief Write entire setup block to QT device.
 *
 * \param setup_block Pointer to setup block buffer for write.
 *
 * \retval STATUS_OK  If write successful.
 * \retval ERR_IO_ERROR If communication failed with the device.
 */
enum status_code qt_write_setup_block(struct qt_setup_block *setup_block);

/**
 * \brief Get all status from QT device.
 *
 * \param qt_status Pointer to QT status buffer for read.
 *
 * \retval STATUS_OK  If read successful.
 * \retval ERR_IO_ERROR If communication failed with the device.
 */
enum status_code qt_get_status(struct qt_status *qt_status);

/**
 * \brief Check if the CHANGE line is low level.
 *
 * \retval true  The CHANGE line is low level.
 * \retval false The CHANGE line is high level.
 */
bool qt_is_change_line_low(void);

/**
 * \brief Read registers content from QT device.
 *
 * \param reg_addr    Start register address.
 * \param read_buffer Pointer to byte array for read data.
 * \param length      Read data length.
 *
 * \retval STATUS_OK  If read successful.
 * \retval ERR_IO_ERROR If communication failed with the device.
 */
enum status_code qt_read_regs(uint8_t reg_addr, uint8_t *read_buffer,
		uint8_t length);

/**
 * \brief Write content to QT device registers.
 *
 * \param reg_addr     Start register address.
 * \param write_buffer Pointer to byte array for write data.
 * \param length       Write data length.
 *
 * \retval STATUS_OK  If write successful.
 * \retval ERR_IO_ERROR If communication failed with the device.
 */
enum status_code qt_write_regs(uint8_t reg_addr, uint8_t *write_buffer,
		uint8_t length);

/** @} */
#ifdef __cplusplus
}
#endif

/**
 * \page qt_device_i2c_quickstart Quick start guide for the QTouch component
 *
 * TBD
 */

#endif /* QT_I2C_H_INCLUDED */
