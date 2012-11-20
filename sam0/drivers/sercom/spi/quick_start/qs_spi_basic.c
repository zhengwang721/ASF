/**
 * \file
 *
 * \brief SAM0+ SPI Quick Start
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \license
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

//! [setup]
//! [buf_length]
#define BUF_LENGTH 20
//! [buf_length]
//! [slave_select_pin]
#define SLAVE_SELECT_PIN 11
//! [slave_select_pin]
//! [buffer]
static const uint8_t buffer[20] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13
};
//! [buffer]
//! [setup]

int main(void)
{
//! [main]
	/* Structures for config and software device instance */
//! [config]
	struct spi_conf config;
//! [config]
//! [dev_inst]
	struct spi_dev_inst dev_inst;
//! [dev_inst]
//! [slave_config]
	struct spi_slave_dev_conf slave_config;
//! [slave_config]
//! [slave_dev_inst]
	struct spi_slave_dev_inst slave_dev_inst;
//! [slave_dev_inst]

	/* Configure and initialize software device instance of peripheral slave */
//! [slave_conf_defaults]
	spi_slave_dev_get_config_defaults(&slave_config);
//! [slave_conf_defaults]
//! [ss_pin]
	slave_config.ss_pin = SLAVE_SELECT_PIN;
//! [ss_pin]
//! [slave_init]
	spi_slave_dev_init(&slave_dev_inst, &slave_config);
//! [slave_init]

	/* Configure, initialize and enable SERCOM SPI module */
//! [conf_defaults]
	spi_get_config_defaults(&config);
//! [conf_defaults]
//! [conf_rec]
	config.receiver_enable = false;
//! [conf_rec]
//! [init]
	spi_init(&dev_inst, &SERCOM0, &config);
//! [init]

	/* Send a buffer to slave */
//! [enable]
	spi_enable(&dev_inst);
//! [enable]
//! [slave_select]
	spi_select_slave(&dev_inst, &slave_dev_inst);
//! [slave_select]
//! [write_buffer]
	spi_write_buffer(&dev_inst, buffer, BUF_LENGTH);
//! [write_buffer]
//! [slave_unselect]
	spi_deselect_slave(&dev_inst, &slave_dev_inst);
//! [slave_unselect]

	/* Loop forever */
//! [inf_loop]
	while (1); //{
	//}
//! [inf_loop]
//! [main]
}
