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
#include <asf.h>
#include "adp_interface.h"

#define EDBG_SPI SPI
#define SPI_CHIP_SEL  SPI_NPCS3_PA5_GPIO

#define SPI_DEVICE_ID         3
#define SPI_BAUDRATE          1500000

struct spi_device SPI_DEVICE = {
	/** Board specific select id */
	.id = SPI_DEVICE_ID
};


/**
* \brief Initialize EDBG SPI interface for SAM 4S
*
*/
enum status_code adp_interface_init(void)
{
	sysclk_init();

	/* Configure the SPI interface */
	spi_master_init(EDBG_SPI);
	spi_master_setup_device(EDBG_SPI, &SPI_DEVICE, SPI_MODE_0,
			SPI_BAUDRATE, 0);
	spi_enable(EDBG_SPI);

	return STATUS_OK;
}

/**
* \brief Send data on SPI
*
* \param[in] data   Pointer to data to send
* \param[in] length Number of bytes to send
*/
enum status_code adp_interface_send(uint8_t *data,	uint16_t length)
{
	return spi_write_packet(EDBG_SPI, data, length);
}

/**
* \brief Read data on SPI
*
* \param[out] data   Pointer to place received data
* \param[in]  length Number of bytes to receive
*/
enum status_code adp_interface_read(uint8_t *data,	uint16_t length)
{
	return spi_read_packet(EDBG_SPI, data, length);
}

/**
* \brief Send single data byte on SPI
*
* \param[in] data   Data byte to send
*/
void adp_interface_send_single(uint8_t data)
{
	spi_write_single(EDBG_SPI, data);
}

/**
* \brief Send SPI start condition
*
*/
void adp_interface_send_start(void)
{
	spi_select_device(EDBG_SPI, &SPI_DEVICE);
}

/**
* \brief Send SPI stop condition
*
*/
void adp_interface_send_stop(void)
{
	spi_deselect_device(EDBG_SPI, &SPI_DEVICE);
}
