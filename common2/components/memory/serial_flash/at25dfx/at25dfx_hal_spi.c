/**
 * \file
 *
 * \brief Connection of the AT25DFx SerialFlash to SPI interface driver.
 *
 * This file manages the connection of the AT25DFx SerialFlash driver to an
 * SPI service (could be spi master service or usart in spi mode). The SPI
 * service selection depends on  AT25DFX_USES_SPI_MASTER_SERVICE or
 * AT25DFX_USES_USART_SPI_SERVICE in conf_at25dfx.h.
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

#include "at25dfx_hal_spi.h"

#ifdef __cplusplus
extern "C" {
#endif


struct spi_slave_inst_config slave_configs[AT25DFX_MEM_CNT];

uint8_t ss_pins[] = {
# define AT25DFX_CS_ID(slot, unused) \
		AT25DFX_CS##slot,
		MREPEAT(AT25DFX_MEM_CNT, AT25DFX_CS_ID, ~)
# undef AT25DFX_CS_ID
};


/** 
 * \brief Initialize SPI external resource for AT25DFx SerialFlash driver.
 *
 *  This function initializes the SerialFlash component as well as the SPI service that
 *  the memory is connected to.
 *  \pre The conf_at25dfx.h file should provide the following information:
 *  - Selection of the SPI service the SerialFlash is connected
 *  (AT25DFX_USES_SPI_MASTER_SERVICE or  AT25DFX_USES_USART_SPI_SERVICE).
 *  - AT25DFX_MEM_SIZE: The SerialFlash type that the driver is connected to.
 *  - AT25DFX_MEM_CNT: The number of SerialFlash components (number of chipselect).
 *  - AT25DFX_SPI_MODULE:  The name of the SPI module used by the SerialFlash driver.
 *  - AT25DFX_SPI_MASTER_SPEED: The SPI bus speed.
 */
void at25dfx_spi_init(void)
{
	struct spi_config config;

	spi_get_config_defaults(&config);

	config.mux_setting = AT25DFX_SPI_PINMUX_SETTING;
	config.pinmux_pad0 = AT25DFX_SPI_PINMUX_PAD0;
	config.pinmux_pad1 = AT25DFX_SPI_PINMUX_PAD1;
	config.pinmux_pad2 = AT25DFX_SPI_PINMUX_PAD2;
	config.pinmux_pad3 = AT25DFX_SPI_PINMUX_PAD3;
	config.mode_specific.master.baudrate = AT25DFX_CLOCK_SPEED;

	spi_init(&at25dfx_master, AT25DFX_SPI, &config);
	spi_enable(&at25dfx_master);
}


/**
 * \brief Select one external SerialFlash component.
 *
 * \param mem_id  The SerialFlash index number.
 */
void at25dfx_spi_select_device(uint8_t mem_id)
{
	switch(mem_id) {
#define AT25DFX_SELECT(slot, unused) \
	case slot:\
		spi_slave_inst_get_config_defaults(&slave_configs[slot]);\
		slave_configs[slot].ss_pin = ss_pins[slot];\
		spi_attach_slave(&at25dfx_slave, &slave_configs[slot]);\
		spi_select_slave(&at25dfx_master, &at25dfx_slave, true);\
		break;
		MREPEAT(AT25DFX_MEM_CNT, AT25DFX_SELECT, ~)
#undef AT25DFX_SELECT
	default:
		/* unhandled_case(id); */
		return;
	}
}


/** 
 * \brief Deselect one external SerialFlash component.
 *
 * \param mem_id  The SerialFlash index number.
 */
void at25dfx_spi_deselect_device(uint8_t mem_id)
{
	switch(mem_id) {
#define AT25DFX_DESELECT(slot, unused) \
	case slot:\
		spi_select_slave(&at25dfx_master, &at25dfx_slave, false);\
		break;
		MREPEAT(AT25DFX_MEM_CNT, AT25DFX_DESELECT, ~)
#undef AT25DFX_DESELECT
	default:
		/* unhandled_case(id); */
		return;
	}
}


/**
 * \brief Send one byte to the SerialFlash.
 *
 * \param data The data byte to send.
 * \pre The SerialFlash should be selected first using at25dfx_spi_select_device.
 */
enum status_code at25dfx_spi_write_byte(uint8_t data)
{
	uint16_t temp = 0;
	while (!spi_is_ready_to_write(&at25dfx_master));
	spi_write(&at25dfx_master,data);
	while (!spi_is_ready_to_read(&at25dfx_master));
	spi_read(&at25dfx_master,&temp);
	return STATUS_OK;
}

/**
 * \brief Get one byte (read) from the SerialFlash.
 *
 * \param data   Data buffer to read.
 *
 * \return The received byte.
 * \pre The SerialFlash should be selected first using at25dfx_spi_select_device.
 */
enum status_code at25dfx_spi_read_byte(uint8_t *data)
{
	uint16_t temp = 0;

	while (!spi_is_ready_to_write(&at25dfx_master));
	spi_write(&at25dfx_master, DUMMY);
	while (!spi_is_ready_to_read(&at25dfx_master));
	spi_read(&at25dfx_master,&temp);

	*data = (uint8_t)temp;
	return STATUS_OK;
}


/**
 * \brief Receive a sequence of bytes from a SerialFlash.
 *
 * \param data   Data buffer to read
 * \param len    Length of data
 * \pre The SerialFlash should be selected first using at25dfx_spi_select_device
 */
enum status_code at25dfx_spi_read_packet(void const *data, size_t len)
{
	return spi_read_buffer_wait(&at25dfx_master, (uint8_t *)data, len,
		(uint16_t)DUMMY);
}

/**
 * \brief Send a sequence of bytes to a SerialFlash.
 *
 *
 * \param data   Data buffer to write
 * \param len    Length of data
 * \pre The SerialFlash should be selected first using at25dfx_spi_select_device
 *
 */
enum status_code at25dfx_spi_write_packet(void const *data, size_t len)
{
	return spi_write_buffer_wait(&at25dfx_master,(const uint8_t *)data, len);
}

//! @}

#ifdef __cplusplus
}
#endif
