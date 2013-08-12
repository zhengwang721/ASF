/**
 * \file
 *
 * \brief SERCOM SPI driver implementation for OZMO stack
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#include "ozmospi_master.h"
#include <sercom_interrupt.h>

/**
 * \ingroup ozmo_sercom_spi_master_group
 *
 * @{
 */

/**
 * \name Internal defines and types
 * @{
 */
/** Convenience macro for the SERCOM SPI */
#define OZMOSPI_SERCOM_SPI     (SercomSpi *)(CONF_OZMOSPI_SERCOM)
/** Transfer mode to use (CPOL = 1, CPHA = 1) */
#define OZMOSPI_TRANSFER_MODE  (SERCOM_SPI_CTRLA_CPOL | SERCOM_SPI_CTRLA_CPHA)
/** Transfer direction indicator */
enum _ozmospi_direction {
	OZMOSPI_DIRECTION_READ,
	OZMOSPI_DIRECTION_WRITE,
	OZMOSPI_DIRECTION_BOTH,
	OZMOSPI_DIRECTION_IDLE,
};
/** Struct to contain driver instance state */
struct ozmospi_module {
	enum _ozmospi_direction direction;
	enum status_code status;
	ozmospi_buflen_t rx_length;
	ozmospi_buflen_t tx_length;
	uint8_t *rx_head_ptr;
	uint8_t *tx_head_ptr;
	struct ozmospi_bufdesc *rx_bufdesc_ptr;
	struct ozmospi_bufdesc *tx_bufdesc_ptr;
};
/** @} */

/** Internal driver state */
volatile struct ozmospi_module _ozmospi_module;


static void _ozmospi_int_handler(uint8_t not_used);


/**
 * \brief Initialize hardware and driver state
 *
 * This function initializes the SERCOM SPI module and the internal driver state
 * structure. It assumes \ref system_init() has been called.
 *
 * The SERCOM SPI module is left disabled after initialization, and must be
 * enabled by \ref ozmospi_enable() before a transfer can be done.
 *
 * \retval STATUS_OK if initialization succeeded.
 * \retval STATUS_ERR_INVALID_ARG if driver has been misconfigured.
 */
enum status_code ozmospi_init(void)
{
	enum status_code status;
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;
	struct port_config portpin_conf;
	struct system_gclk_chan_config gclk_chan_conf;
	struct system_pinmux_config pin_conf;
	uint16_t tmp_baud;
	uint32_t sercom_index = _sercom_get_sercom_inst_index((Sercom *)spi_hw);
	uint32_t pm_index = sercom_index + PM_APBCMASK_SERCOM0_Pos;
	uint32_t gclk_index = sercom_index + SERCOM0_GCLK_ID_CORE;
	uint32_t gclk_hz;

	/* Enable clock for the module interface */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);

	/* Set up the GCLK for the module */
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	gclk_chan_conf.source_generator = CONF_OZMOSPI_GCLK_SOURCE;
	system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
	system_gclk_chan_enable(gclk_index);
	sercom_set_gclk_generator(CONF_OZMOSPI_GCLK_SOURCE, false);

	/* Set up the SERCOM SPI module as master */
	spi_hw->CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPI_MASTER;
	spi_hw->CTRLA.reg |= OZMOSPI_TRANSFER_MODE | CONF_OZMOSPI_SIGNAL_MUX;

	/* Get baud value from configured baudrate and internal clock rate */
	gclk_hz = system_gclk_chan_get_hz(gclk_index);
	status = _sercom_get_sync_baud_val(CONF_OZMOSPI_BAUDRATE, gclk_hz,
			&tmp_baud);

	if (status != STATUS_OK) {
		/* Baud rate calculation error! */
		return STATUS_ERR_INVALID_ARG;
	}

	spi_hw->BAUD.reg = (uint8_t)tmp_baud;

	/* Register our custom interrupt handler */
	_sercom_set_handler(sercom_index, _ozmospi_int_handler);

	/* Configure the slave select pin as output, driven high */
	port_get_config_defaults(&portpin_conf);
	portpin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(CONF_OZMOSPI_SS_PIN, &portpin_conf);
	port_pin_set_output_level(CONF_OZMOSPI_SS_PIN, true);

	/* Configure the pin multiplexers */
	system_pinmux_get_config_defaults(&pin_conf);

	pin_conf.mux_position = CONF_OZMOSPI_PINMUX_PAD0 & 0xFFFF;
	system_pinmux_pin_set_config(CONF_OZMOSPI_PINMUX_PAD0 >> 16, &pin_conf);

	pin_conf.mux_position = CONF_OZMOSPI_PINMUX_PAD1 & 0xFFFF;
	system_pinmux_pin_set_config(CONF_OZMOSPI_PINMUX_PAD1 >> 16, &pin_conf);

	pin_conf.mux_position = CONF_OZMOSPI_PINMUX_PAD2 & 0xFFFF;
	system_pinmux_pin_set_config(CONF_OZMOSPI_PINMUX_PAD2 >> 16, &pin_conf);

	pin_conf.mux_position = CONF_OZMOSPI_PINMUX_PAD3 & 0xFFFF;
	system_pinmux_pin_set_config(CONF_OZMOSPI_PINMUX_PAD3 >> 16, &pin_conf);

	/* Lastly, initialize our struct */
	_ozmospi_module.rx_bufdesc_ptr = NULL;
	_ozmospi_module.tx_bufdesc_ptr = NULL;
	_ozmospi_module.direction = OZMOSPI_DIRECTION_IDLE;
	_ozmospi_module.status = STATUS_OK;

	return STATUS_OK;
}

/**
 * \brief Enable the SERCOM SPI module
 *
 * This function must be called after \ref ozmospi_init() before a transfer can
 * be done.
 */
void ozmospi_enable(void)
{
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;

	spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE | SERCOM_SPI_INTFLAG_RXC
			| SERCOM_SPI_INTFLAG_TXC;

	while (spi_hw->STATUS.reg & SERCOM_SPI_STATUS_SYNCBUSY) {
		/* Intentionally left empty */
	}

	spi_hw->CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;

	system_interrupt_enable(_sercom_get_interrupt_vector((Sercom *)OZMOSPI_SERCOM_SPI));
}

/**
 * \brief Disable the SERCOM SPI module
 */
void ozmospi_disable(void)
{
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;

	system_interrupt_disable(_sercom_get_interrupt_vector((Sercom *)OZMOSPI_SERCOM_SPI));

	while (spi_hw->STATUS.reg & SERCOM_SPI_STATUS_SYNCBUSY) {
		/* Intentionally left empty */
	}

	spi_hw->CTRLB.reg = 0;
	spi_hw->CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
}

/**
 * \brief Start scatter/gather SPI transfer and wait for completion
 *
 * This function initiates a SPI transfer from/to an arbitrary number of data
 * buffers.
 *
 * An array of buffer descriptors must be specified for the transmission and/or
 * reception.
 * The lists \e must end with a descriptor that specifies zero buffer
 * length.
 * The first descriptor in the list can \e not specify zero length.
 *
 * To initiate a simplex transfer, pass \c NULL as the address of either buffer
 * descriptor list, like this:
 * \code
 *     // Transmit some buffers
 *     ozmospi_transceive_buffers_wait(tx_buffers, NULL);
 *
 *     // Receive some buffers
 *     ozmospi_transceive_buffers_wait(NULL, rx_buffers);
 * \endcode
 *
 * Note that \ref ozmospi_init() and \ref ozmospi_enable() must be called before
 * this function to ensure correct operation.
 *
 * \param[in] tx_bufdescs address of array with descriptors of buffers to
 * read bytes to transmit from.
 * \arg \c NULL if the transfer is a simplex read.
 * \param[out] rx_bufdescs address of array with descriptors of buffers to
 * write received bytes into.
 * \arg \c NULL if the transfer is a simplex write.
 *
 * \retval STATUS_OK if transfer succeeded. Other status codes upon failure.
 */
enum status_code ozmospi_transceive_buffers_wait(
	struct ozmospi_bufdesc tx_bufdescs[],
	struct ozmospi_bufdesc rx_bufdescs[])
{
	enum status_code status;
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;
	uint8_t tmp_intenset;

	Assert(tx_bufdescs || rx_bufdescs);

	if (_ozmospi_module.status == STATUS_BUSY) {
		return STATUS_BUSY;
	} else {
		_ozmospi_module.status = STATUS_BUSY;
	}

	_ozmospi_module.tx_bufdesc_ptr = tx_bufdescs;
	_ozmospi_module.rx_bufdesc_ptr = rx_bufdescs;

	if (tx_bufdescs && rx_bufdescs) {
		Assert(tx_bufdescs[0].length);
		Assert(rx_bufdescs[0].length);

		_ozmospi_module.direction = OZMOSPI_DIRECTION_BOTH;
		_ozmospi_module.tx_length = tx_bufdescs[0].length;
		_ozmospi_module.tx_head_ptr = tx_bufdescs[0].data;
		_ozmospi_module.rx_length = rx_bufdescs[0].length;
		_ozmospi_module.rx_head_ptr = rx_bufdescs[0].data;
		spi_hw->CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;

		tmp_intenset = SERCOM_SPI_INTFLAG_DRE | SERCOM_SPI_INTFLAG_RXC;
	} else {
		if (tx_bufdescs) {
			Assert(tx_bufdescs[0].length);

			_ozmospi_module.direction = OZMOSPI_DIRECTION_WRITE;
			_ozmospi_module.tx_length = tx_bufdescs[0].length;
			_ozmospi_module.tx_head_ptr = tx_bufdescs[0].data;

			tmp_intenset = SERCOM_SPI_INTFLAG_DRE;
		} else {
			Assert(rx_bufdescs[0].length);

			_ozmospi_module.direction = OZMOSPI_DIRECTION_READ;
			_ozmospi_module.rx_length = rx_bufdescs[0].length;
			_ozmospi_module.rx_head_ptr = rx_bufdescs[0].data;
			spi_hw->CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;

			tmp_intenset = SERCOM_SPI_INTFLAG_DRE | SERCOM_SPI_INTFLAG_RXC;
		}
	}

	spi_hw->INTENSET.reg = tmp_intenset;

	do {
		status = _ozmospi_module.status;
	} while (status == STATUS_BUSY);

	return status;
}

/**
 * \brief Interrupt handler
 *
 * \param not_used SERCOM instance number passed from the master SERCOM driver.
 */
static void _ozmospi_int_handler(uint8_t not_used)
{
	enum _ozmospi_direction dir = _ozmospi_module.direction;
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;
	uint8_t int_status;
	uint8_t *tx_head_ptr;
	uint8_t *rx_head_ptr;
	ozmospi_buflen_t tx_length;
	ozmospi_buflen_t rx_length;

	int_status = spi_hw->INTFLAG.reg & spi_hw->INTENSET.reg;

	if (int_status & SERCOM_SPI_INTFLAG_DRE) {
		/* If doing a READ, just send 0 to trigger the transfer */
		if (dir == OZMOSPI_DIRECTION_READ) {
			spi_hw->DATA.reg = 0;

			/* Check if this send will fetch the last byte to read */
check_for_read_end:
			tx_length = _ozmospi_module.rx_length - 1;

			if (!tx_length) {
				tx_length = (_ozmospi_module.rx_bufdesc_ptr + 1)->length;

				if (!tx_length) {
					/* Disable DRE to stop future transfers */
					spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
				}
			}

		/* For WRITE and BOTH, output current byte */
		} else {
			tx_head_ptr = _ozmospi_module.tx_head_ptr;
			spi_hw->DATA.reg = *(tx_head_ptr++);

			/* Check if this was the last byte to send */
			tx_length = _ozmospi_module.tx_length - 1;

			if (tx_length) {
				_ozmospi_module.tx_head_ptr = tx_head_ptr;
				_ozmospi_module.tx_length = tx_length;
			} else {
				/* Any more buffers left to send, perhaps? */
				tx_length = (++_ozmospi_module.tx_bufdesc_ptr)->length;

				if (tx_length) {
					_ozmospi_module.tx_head_ptr = _ozmospi_module.tx_bufdesc_ptr->data;
					_ozmospi_module.tx_length = tx_length;
				} else {
					if (dir == OZMOSPI_DIRECTION_WRITE) {
						/* Disable DRE and enable TXC to end WRITE */
						spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
						spi_hw->INTENSET.reg = SERCOM_SPI_INTFLAG_TXC;
					} else {
						/* For BOTH, check if we still have bytes left to read */
						dir = OZMOSPI_DIRECTION_READ;
						_ozmospi_module.direction = dir;
						goto check_for_read_end;
					}
				}
			}
		}
	}

	/* For READ and BOTH, store the received byte */
	if (int_status & SERCOM_SPI_INTFLAG_RXC) {
		rx_head_ptr = _ozmospi_module.rx_head_ptr;
		*(rx_head_ptr++) = spi_hw->DATA.reg;

		/* Check if this was the last byte to receive */
		rx_length = _ozmospi_module.rx_length - 1;

		if (rx_length) {
			_ozmospi_module.rx_head_ptr = rx_head_ptr;
			_ozmospi_module.rx_length = rx_length;
		} else {
			/* Any more buffers left to receive into? */
			rx_length = (++_ozmospi_module.rx_bufdesc_ptr)->length;

			if (rx_length) {
				_ozmospi_module.rx_head_ptr = _ozmospi_module.rx_bufdesc_ptr->data;
				_ozmospi_module.rx_length = rx_length;
			} else {
				/* Disable the SPI received (instant effect) */
				spi_hw->CTRLB.reg = 0;

				if (dir == OZMOSPI_DIRECTION_READ) {
					/* If doing READ, end the transaction here */
					spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_RXC;

					dir = OZMOSPI_DIRECTION_IDLE;
					_ozmospi_module.direction = dir;
					_ozmospi_module.status = STATUS_OK;
				} else {
					/* If doing BOTH, change direction to WRITE */
					dir = OZMOSPI_DIRECTION_WRITE;
					_ozmospi_module.direction = dir;
				}
			}
		}
	}

	/* For WRITE */
	if (int_status & SERCOM_SPI_INTFLAG_TXC) {
		/* End transaction here, since last byte has been sent */
		spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC;

		dir = OZMOSPI_DIRECTION_IDLE;
		_ozmospi_module.direction = dir;
		_ozmospi_module.status = STATUS_OK;
	}
}

/**
 * @}
 */