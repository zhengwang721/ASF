/**
 * \file
 *
 * \brief SAM D20 SPI Quick Start
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

//! [setup]
//! [buf_length]
#define BUF_LENGTH 20
//! [buf_length]
//! [slave_select_pin]
#define SLAVE_SELECT_PIN EXT1_PIN_SPI_SS_0
//! [slave_select_pin]
//! [buffer]
static const uint8_t buffer[BUF_LENGTH] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13
};
//! [buffer]

//! [dev_inst]
struct spi_module master;
//! [dev_inst]
//! [slave_dev_inst]
struct spi_slave_inst slave;
//! [slave_dev_inst]
//! [setup]

void configure_spi(void);

//! [configure_spi]
void configure_spi(void)
{
//! [config]
	struct spi_config config;
//! [config]
//! [slave_config]
	struct spi_slave_inst_config slave_dev_config;
//! [slave_config]
	/* Configure and initialize software device instance of peripheral slave */
//! [slave_conf_defaults]
	spi_slave_inst_get_config_defaults(&slave_dev_config);
//! [slave_conf_defaults]
//! [ss_pin]
	slave_dev_config.ss_pin = SLAVE_SELECT_PIN;
//! [ss_pin]
//! [slave_init]
	spi_attach_slave(&slave, &slave_dev_config);
//! [slave_init]
	/* Configure, initialize and enable SERCOM SPI module */
//! [conf_defaults]
	spi_get_config_defaults(&config);
//! [conf_defaults]
//! [mux_setting]
	config.mux_setting = EXT1_SPI_SERCOM_MUX_SETTING;
//! [mux_setting]
	/* Configure pad 0 for data in */
//! [di]
	config.pinmux_pad0 = EXT1_SPI_SERCOM_PINMUX_PAD0;
//! [di]
	/* Configure pad 1 as unused */
//! [ss]
	config.pinmux_pad1 = PINMUX_UNUSED;
//! [ss]
	/* Configure pad 2 for data out */
//! [do]
	config.pinmux_pad2 = EXT1_SPI_SERCOM_PINMUX_PAD2;
//! [do]
	/* Configure pad 3 for SCK */
//! [sck]
	config.pinmux_pad3 = EXT1_SPI_SERCOM_PINMUX_PAD3;
//! [sck]
//! [init]
	spi_init(&master, EXT1_SPI_MODULE, &config);
//! [init]

//! [enable]
	spi_enable(&master);
//! [enable]

}
//! [configure_spi]

int main(void)
{
//! [main_setup]
//! [system_init]
	system_init();
//! [system_init]
//! [run_config]
	configure_spi();
//! [run_config]
//! [main_setup]

//! [main_use_case]
//! [select_slave]
	spi_select_slave(&master, &slave, true);
//! [select_slave]
//! [write]
	spi_write_buffer_wait(&master, buffer, BUF_LENGTH);
//! [write]
//! [deselect_slave]
	spi_select_slave(&master, &slave, false);
//! [deselect_slave]

//! [inf_loop]
	while (true) {
		/* Infinite loop */
	}
//! [inf_loop]
//! [main_use_case]
}
