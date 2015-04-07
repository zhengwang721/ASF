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

#define EDBG_SPI EDBG_SPI_MODULE
#define EDBG_SPI_SS_PIN PIN_PB31//PIN_PB17

struct spi_module edbg_spi;
struct spi_slave_inst slave;

/**
* \brief Initialize EDBG SPI interface for SAM0
*
*/
enum status_code adp_interface_init(void)
{
	enum status_code return_value;

	system_init();
	ioport_init();

	struct spi_slave_inst_config slave_dev_config;

	struct spi_config config;

	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = EDBG_SPI_SS_PIN;
	spi_attach_slave(&slave, &slave_dev_config);

	spi_get_config_defaults(&config);
	config.mode_specific.master.baudrate = 1000000;
	config.mux_setting = EDBG_SPI_SERCOM_MUX_SETTING;
	config.pinmux_pad0 = EDBG_SPI_SERCOM_PINMUX_PAD0;
	config.pinmux_pad1 = PINMUX_UNUSED;
	config.pinmux_pad2 = EDBG_SPI_SERCOM_PINMUX_PAD2;
	config.pinmux_pad3 = EDBG_SPI_SERCOM_PINMUX_PAD3;

	return_value = spi_init(&edbg_spi, EDBG_SPI, &config);

	spi_enable(&edbg_spi);

	return return_value;
}

/**
* \brief Send data on SPI
*
* \param[in] data   Pointer to data to send
* \param[in] length Number of bytes to send
*/
enum status_code adp_interface_send(uint8_t *data,	uint16_t length)
{
	return spi_write_buffer_wait(&edbg_spi, data, length);
}

/**
* \brief Read data on SPI
*
* \param[out] data   Pointer to place received data
* \param[in]  length Number of bytes to receive
*/
enum status_code adp_interface_read(uint8_t *data,	uint16_t length)
{
	return spi_read_buffer_wait(&edbg_spi, data, length, 0xFF);
}

/**
* \brief Send single data byte on SPI
*
* \param[in] data   Data byte to send
*/
void adp_interface_send_single(uint8_t data)
{
	//spi_write(&edbg_spi, data);
	spi_write_buffer_wait(&edbg_spi, &data, 1);
}

/**
* \brief Send SPI start condition
*
*/
void adp_interface_send_start(void)
{
	spi_select_slave(&edbg_spi, &slave, true);
}

/**
* \brief Send SPI stop condition
*
*/
void adp_interface_send_stop(void)
{
	spi_select_slave(&edbg_spi, &slave, false);
}
