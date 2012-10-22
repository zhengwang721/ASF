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
#include "spi.h"

/**
 * \internal Reset the SPI SERCOM module
 *
 * \param dev_inst Pointer to the software instance struct
 */
void _spi_reset(struct spi_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	_spi_sync(dev_inst);

	/* Software reset the module */
	spi_module->SPI->CTRLA |= SPI_SWRST_bm;
}

/**
 * \internal Writes an SPI SERCOM configuration to the hardware module.
 *
 * Writes out a given configuration to the hardware module. Can only be done
 * when the module is disabled.
 *
 * \param[in] dev_inst  Pointer to the software instance struct
 * \param[in] config    Pointer to the configuration struct
 *
 * \return
 * \retval STATUS_ERR_INVALID_ARG
 * \retval STATUS_OK
 */
enum status_code _spi_set_config(struct spi_dev_inst *const dev_inst,
		struct spi_conf *config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(config);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Value to write to BAUD register */
	uint16_t baud;
	/* Value to write to CTRLA register */
	uint32_t ctrla = 0;
	/* Status code */
	enum status_code err = STATUS_OK;

		/* Find baud and write it */
	if (config->master_mode) {
		/*err = sercom_get_sync_baud_val(
				config->master_config.baudrate,
				sysclk_get_gclk_hz(dev_inst), &baud);*/
		if (err != STATUS_OK) {
			/* Baud rate calculation error, return*/
			return STATUS_ERR_INVALID_ARG;
		}
		spi_module->SPI->BAUD = (uint8_t)baud;
	}

	/* Set clock polarity and clock phase */
			config->dataorder;


	/*if ((config->form & SPI_FORM_WITH_ADDR) &&
			(config->master_mode & SPI_MASTER)) {
		spi_module->SPI.ADDR = (config->addrmode << 16) |
				config->address;
	}
	*/
	/* Enable receiver */
	spi_enable_rx(dev_inst);
	return STATUS_OK;
}

/**
 * \brief Initializes the SERCOM SPI module
 *
 * Initializes the SERCOM SPI module, based on the values of the config struct.
 *
 * \param[out] dev_inst    Pointer to the software instance struct
 * \param[in] config      Pointer to the config struct
 *
 * \return
 * \retval STATUS_OK
 * \retval STATUS_ERR_INVALID_ARG
 */
enum status_code spi_init(struct spi_dev_inst *const dev_inst, SPI_t *module,
		struct spi_conf *config)
{

	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(module);
	Assert(config);

	/* Initialize device instance */
	dev_inst->hw_dev->SPI = module;

	/* Reset the module */
	_spi_reset(dev_inst);

	/* Write configuration to module and return status code*/
	return _spi_set_config(dev_inst, config);
}

/**
 * \brief Read block of data from SPI peripheral by sending dummy
 * byte if in master mode, or by waiting for data in slave mode.
 *
 * \param[in] dev_inst  Pointer to the software instance struct
 * \param[out] rx_data  Data buffer for received data
 * \param[in] length    Length of data to receive
 *
 * \return
 * \retval STATUS_OK              If the read is completed
 * \retval STATUS_ERR_INVALID_ARG If length is 0
 */
static inline spi_read_block(struct spi_dev_inst *const dev_inst,
		uint8_t *rx_data, uint8_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Dummy byte to shift out in master mode */
	uint8_t dummy = 0x00;

	/* Sanity check arguments */
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	if (dev_inst->master_mode) {
		/* Send dummy byte to read in master mode */
		do {
			spi_write(dev_inst, dummy);
			spi_read(dev_inst, rx_data++);
		} while (--length);
	} else {
		/* Wait for data in slave mode */
		do {
			spi_read(dev_inst, rx_data++);
		} while (--length);
	}
	return STATUS_OK;
}

/**
 * \brief Send and receive a block of \ref length length bytes via SPI
 *
 * This function will send and receive a block of data via the SPI.
 
 * In master mode the bytes will be sent immediately and the received byte will
 * be read as soon as the shifting of the byte is complete.
 *
 * In slave mode this function will place the data to be sent into the transmit
 * buffer. It will then block until an SPI master has shifted the complete
 * buffer and the received data is available.
 *
 * \param[in] dev_inst Pointer to the software instance struct
 * \param[in] tx_data  Pointer to the buffer to transmit
 * \param[out] rx_data Pointer to the buffer where received data will be stored
 * \param[in] length   Number of bytes to transfer
 */
enum status_code spi_tranceive_block(struct spi_dev_inst *const dev_inst,
		uint8_t *tx_data, uint8_t *rx_data, uint8_t length)
{
		/* Sanity check arguments */
	Assert(dev_inst);

	/* Sanity check arguments */
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Send and receive block */
	do {
		spi_write(dev_inst, *(tx_data++));
		spi_read(dev_inst, rx_data++);
	} while (--length);

	return STATUS_OK;
}
