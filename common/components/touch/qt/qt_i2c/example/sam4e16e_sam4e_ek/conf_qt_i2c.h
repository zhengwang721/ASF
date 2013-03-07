/**
 * \file
 *
 * \brief QTouch component with I2C interface configuration.
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
#ifndef CONF_QT_I2C_H_INCLUDED
#define CONF_QT_I2C_H_INCLUDED

/**
 * Select one of the QT device from support list,
 * \ref qt_device_i2c_support_list (eg: QT2160)
 */
#define CONF_QT_DEVICE            QT2160

/** TWI host base address */
#define CONF_QT_TWI_BASE_ADDRESS  TWI0
/** TWI slave memory address (I2CA1 = I2CA0 = 0) */
#define CONF_QT_I2C_ADDRESS       0x0D
/** TWI data transfer rate for QT device */
#define CONF_QT_I2C_SPEED         50000

/** RESET pin index for QT device */
#define CONF_QT_RESET_PIN_IDX     QT_PIN_IDX_INVALID

/** CHANGE pin index for QT device */
#define CONF_QT_CHANGE_PIN_IDX    PIO_PE4_IDX

#endif /* CONF_QT_I2C_H_INCLUDED */
