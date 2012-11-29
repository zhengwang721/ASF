/**
 * \file
 *
 * \brief SAM0+ Serial Peripheral Interface Driver
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

#include <asf.h>

#define DATA_LENGTH 10
static uint8_t buffer[DATA_LENGTH] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
};

#define SLAVE_ADDRESS 0x01

/* Init device instance. */
struct i2c_master_dev_inst dev_inst;

static void configure_i2c(void)
{
	/* Initialize config structure and device instance. */
	struct i2c_master_conf conf;
	i2c_master_get_config_defaults(&conf);

	/* Change buffer timeout to something longer. */
	conf.buffer_timeout = 10000;

	/* Initialize and enable device with config. */
	i2c_master_init(&dev_inst, &SERCOM0, &conf);

	i2c_master_enable(&dev_inst);
}

int main(void)
{
	/* Configure device and enable. */
	configure_i2c();

	/* Init i2c packet. */
	i2c_packet_t packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = DATA_LENGTH,
		.data        = &buffer[0]
	};

	/* Write buffer to slave. */
	i2c_master_write_packet(&dev_inst, &packet);

	while (1) {
		/* Inf loop. */
	}
}