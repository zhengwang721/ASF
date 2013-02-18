/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#include <asf.h>

//! [packet]
static struct i2c_packet packet;
//! [packet]


//! [packet_data]
#define DATA_LENGTH 10
static uint8_t write_buffer[DATA_LENGTH] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
};
static uint8_t read_buffer [DATA_LENGTH];
//! [packet_data]

/* Adress of the slave */
#define SLAVE_ADDRESS 0x12

/* Init device instance. */
//! [module]
struct i2c_slave_module sw_module;
//! [module]

//! [read_request]
static void read_request_callback(const struct i2c_slave_module *const module)
{
	/* Init i2c packet. */
	//! [packet_write]
	packet.data_length = DATA_LENGTH;
	packet.data        = write_buffer;
	//! [packet_write]

	/* Write buffer to master */
	//! [write_packet]
	i2c_slave_write_packet_job(&sw_module, &packet);
	//! [write_packet]
}
//! [read_request]

//! [write_request]
static void write_request_callback(const struct i2c_slave_module *const module)
{
	/* Init i2c packet. */
	//! [packet_read]
	packet.data_length = DATA_LENGTH;
	packet.data        = read_buffer;
	//! [packet_read]

	/* Read buffer to master */
	//! [read_packet]
	i2c_slave_read_packet_job(&sw_module, &packet);
	//! [read_packet]
}
//! [write_request]

//! [initialize_i2c]
static void configure_i2c(void)
{
	/* Initialize config structure and module instance. */
	//! [init_conf]
	struct i2c_slave_conf conf;
	i2c_slave_get_config_defaults(&conf);
	//! [init_conf]
	/* Change address and address_mode. */
	//! [conf_changes]
	conf.address = SLAVE_ADDRESS;
	conf.address_mode = I2C_SLAVE_ADDRESS_MODE_MASK;
	//! [conf_changes]
	/* Initialize and enable device with config. */
	//! [init_module]
	i2c_slave_init(&sw_module, SERCOM2, &conf);
	//! [init_module]

	//! [enable_module]
	i2c_slave_enable(&sw_module);
	//! [enable_module]
}
//! [initialize_i2c]

//! [setup_i2c_callback]
static void configure_callbacks(void)
{
	/* Register and enable callback functions. */
	//![reg_en_i2c_callback]
	i2c_slave_register_callback(&sw_module, read_request_callback, I2C_SLAVE_CALLBACK_READ_REQUEST);
	i2c_slave_enable_callback(&sw_module, I2C_SLAVE_CALLBACK_READ_REQUEST);

	i2c_slave_register_callback(&sw_module, write_request_callback, I2C_SLAVE_CALLBACK_WRITE_REQUEST);
	i2c_slave_enable_callback(&sw_module, I2C_SLAVE_CALLBACK_WRITE_REQUEST);

	//![reg_en_i2c_callback]

}
//! [setup_i2c_callback]

int main(void)
{
	//! [run_initialize_i2c]
	/* Init system. */
	system_init();

	/* Configure device and enable. */
	configure_i2c();
	configure_callbacks();
	//! [run_initialize_i2c]

	while (1) {
		/* Inf loop while waiting for I2C slave interaction.*/
	}
}
