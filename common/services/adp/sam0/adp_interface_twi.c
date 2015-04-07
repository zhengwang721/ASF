/**
 * \file
 *
 * \brief ADP service implementation
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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

#include <compiler.h>
#include <system.h>

#include <asf.h>
#include <adp_interface.h>

#define EDBG_TWI EDBG_I2C_MODULE
#define TWI_EDBG_SLAVE_ADDR 0x28
#define TIMEOUT 1000

struct i2c_master_module i2c_master_instance;

/**
* \brief Initialize EDBG TWI interface for SAM0
*
*/
enum status_code adp_interface_init(void)
{
	enum status_code return_value;

	system_init();

	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	config_i2c_master.buffer_timeout = 10000;
	return_value = i2c_master_init(&i2c_master_instance, EDBG_TWI, &config_i2c_master);
	i2c_master_enable(&i2c_master_instance);
	return return_value;
}

/**
* \brief Send data on TWI
*
* \param[in] data   Pointer to data to send
* \param[in] length Number of bytes to send
*/
enum status_code adp_interface_send(uint8_t *data,	uint16_t length)
{
	struct i2c_master_packet packet = {
		.address = TWI_EDBG_SLAVE_ADDR,
		.data_length = length,
		.data = data,
	};
	return i2c_master_write_packet_wait(&i2c_master_instance, &packet);
}

/**
* \brief Read data on TWI
*
* \param[out] data   Pointer to place received data
* \param[in]  length Number of bytes to receive
*/
enum status_code adp_interface_read(uint8_t *data,	uint16_t length)
{
	struct i2c_master_packet packet = {
		.address = TWI_EDBG_SLAVE_ADDR,
		.data_length = length,
		.data = data,
	};
	return i2c_master_read_packet_wait(&i2c_master_instance, &packet);
}

/**
* \brief Send single data byte on TWI
*
* \param[in] data   Data byte to send
*/
void adp_interface_send_single(uint8_t data)
{
	adp_interface_send(&data, 1);
}

/**
* \brief Send TWI start condition
*
*/
void adp_interface_send_start(void)
{
	/* Intentionally left empty */
}

/**
* \brief Send TWI stop condition
*
*/
void adp_interface_send_stop(void)
{
	/* Intentionally left empty */
}
