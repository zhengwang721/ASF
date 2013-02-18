/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#include <asf.h>
#include <system_interrupt.h>

//! [packet_data]
#define DATA_LENGTH 8

static uint8_t buffer[DATA_LENGTH] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
};
static uint8_t buffer_reversed[DATA_LENGTH] = {
		0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
};
struct i2c_packet packet;
#define SLAVE_ADDRESS 0x12
//! [packet_data]

/* Number of time to try and send packet if failed. */
#define TIMEOUT 1000

/* Init device instance. */
//! [dev_inst]
struct i2c_master_dev_inst dev_inst;
//! [dev_inst]


//! [callback_func]
static void write_callback(const struct i2c_master_dev_inst *const dev_inst_tass)
{
	/* Send every other packet with reversed data. */
	//! [revert_order]
	if (packet.data[0] == 0x00) {
		packet.data = &buffer_reversed[0];
	} else {
		packet.data = &buffer[0];
	}
	//! [revert_order]
	
	/* Initiate new packet write. */
	//! [write_next]
	i2c_master_async_write_packet(&dev_inst, &packet);
	//! [write_next]
}
//! [callback_func]

//! [initialize_i2c]
static void configure_i2c(void)
{
	/* Initialize config structure and device instance. */
	//! [init_conf]
	struct i2c_master_config conf;
	i2c_master_get_config_defaults(&conf);
	//! [init_conf]

	/* Change buffer timeout to something longer. */
	//! [conf_change]
	conf.buffer_timeout = 10000;
	//! [conf_change]

	/* Initialize and enable device with config. */
	//! [init_module]
	i2c_master_init(&dev_inst, SERCOM2, &conf);
	//! [init_module]
	
	//! [enable_module]
	i2c_master_enable(&dev_inst);
	//! [enable_module]
}
//! [initialize_i2c]

//! [setup_callback]
void configure_callbacks(void){
  	/* Register callback function. */
	i2c_master_register_callback(&dev_inst, write_callback, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
	i2c_master_enable_callback(&dev_inst, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
  
	/* Enable interrupts for SERCOM instance. */
	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_SERCOM2);
}
//! [setup_callback]

int main(void)
{
	//! [run_initialize_i2c]
	/* Init system. */
	system_init();	
	/* Configure device and enable. */
	configure_i2c();
	/* Configure callbacks and enable. */
	configure_callbacks();
	//! [run_initialize_i2c]

	/* Init i2c packet. */
	//! [packet]
	packet.address     = SLAVE_ADDRESS;
	packet.data_length = DATA_LENGTH;
	packet.data        = buffer;
	//! [packet]
		
	/* Initiate first packet to be sent to slave. */
	//! [write_packet]
	i2c_master_async_write_packet(&dev_inst, &packet);
	//! [write_packet]
		
	while (1) {
		/* Inf loop. */
	}
}
