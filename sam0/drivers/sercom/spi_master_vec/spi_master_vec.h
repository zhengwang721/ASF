/**
 * \file
 *
 * \brief SERCOM SPI master with vectored I/O driver include
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

#ifndef SPI_MASTER_VEC_H
#define SPI_MASTER_VEC_H

#include <conf_spi_master_vec.h>
#include <gclk.h>
#include <port.h>
#include <status_codes.h>

/**
 * \defgroup sercom_spi_master_vec_group SERCOM SPI master driver with
 * vectored I/O
 *
 * This driver is a single-instance, compile-time configured SPI master driver.
 * It supports both simplex and duplex transfers with vectored I/O, also know
 * as scatter/gather.
 *
 * Scatter/gather is implemented by the use of buffer descriptor arrays, which
 * must be passed to the driver to start a transfer.
 * See \ref spi_master_vec_transceive_buffers_wait() for more information.
 *
 * @{
 */

/**
 * \name Configuration types and macros
 * @{
 */

/**
 * \def PINMUX_DEFAULT
 * \brief Specify that default pin MUX setting should be used for pad.
 *
 * \note See \ref spi_master_vec_config for details about the behavior of this
 * pin MUX value.
 */
#ifndef PINMUX_DEFAULT
#  define PINMUX_DEFAULT 0
#endif

/**
 * \def PINMUX_UNUSED
 * \brief Specify that pin MUX should be skipped.
 */
#ifndef PINMUX_UNUSED
#  define PINMUX_UNUSED 0xFFFFFFFF
#endif

/** SERCOM PAD multiplexing */
enum spi_master_vec_padmux_setting {
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_a
	 */
	SPI_MASTER_VEC_PADMUX_SETTING_A =
			(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x0 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_b
	 */
	SPI_MASTER_VEC_PADMUX_SETTING_B =
			(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x1 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_c
	 */
	SPI_MASTER_VEC_PADMUX_SETTING_C =
			(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x2 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_d
	 */
	SPI_MASTER_VEC_PADMUX_SETTING_D =
			(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x3 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_e
	 */
	SPI_MASTER_VEC_PADMUX_SETTING_E =
			(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x0 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_f
	 */
	SPI_MASTER_VEC_PADMUX_SETTING_F =
			(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x1 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_g
	 */
	SPI_MASTER_VEC_PADMUX_SETTING_G =
			(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x2 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_h
	 */
	SPI_MASTER_VEC_PADMUX_SETTING_H =
			(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x3 << SERCOM_SPI_CTRLA_DIPO_Pos),
};

/** SPI transfer mode */
enum spi_master_vec_transfer_mode {
	/** Mode 0. Leading edge: rising, sample. Trailing edge: falling, setup */
	SPI_MASTER_VEC_TRANSFER_MODE_0 = 0,
	/** Mode 1. Leading edge: rising, setup. Trailing edge: falling, sample */
	SPI_MASTER_VEC_TRANSFER_MODE_1 = SERCOM_SPI_CTRLA_CPHA,
	/** Mode 2. Leading edge: falling, sample. Trailing edge: rising, setup */
	SPI_MASTER_VEC_TRANSFER_MODE_2 = SERCOM_SPI_CTRLA_CPOL,
	/** Mode 3. Leading edge: falling, setup. Trailing edge: rising, sample */
	SPI_MASTER_VEC_TRANSFER_MODE_3 = SERCOM_SPI_CTRLA_CPHA | SERCOM_SPI_CTRLA_CPOL,
};

/** SPI data transfer order */
enum spi_master_vec_data_order {
	/** LSB of data is transmitted first */
	SPI_MASTER_VEC_DATA_ORDER_LSB  = SERCOM_SPI_CTRLA_DORD,
	/** MSB of data is transmitted first */
	SPI_MASTER_VEC_DATA_ORDER_MSB  = 0,
};

/**
 * \brief Driver configuration structure
 *
 * \note The order of the pin MUX values in the \e pinmux_N members only matters
 * when the \ref PINMUX_DEFAULT is used: In this case, N signifies which SERCOM
 * pad to multiplex to its default pin. For example, if \e pinmux_0 is set to
 * \ref PINMUX_DEFAULT, then SERCOM pad 0 will be multiplexed to its default pin
 * and it is left to the user to ensure that the other \e pinmux_N do not
 * configure the same pin.
 */
struct spi_master_vec_config {
	/** Baud rate in Hertz */
	uint32_t baudrate;
	/** GCLK generator to use for the SERCOM */
	enum gclk_generator gclk_source;
	/** SERCOM Pad MUX setting */
	enum spi_master_vec_padmux_setting padmux_setting;
	/** Transfer mode */
	enum spi_master_vec_transfer_mode transfer_mode;
	/** Data order */
	enum spi_master_vec_data_order data_order;
	/** Pin MUX setting #0 */
	uint32_t pinmux_0;
	/** Pin MUX setting #1 */
	uint32_t pinmux_1;
	/** Pin MUX setting #2 */
	uint32_t pinmux_2;
	/** Pin MUX setting #4*/
	uint32_t pinmux_3;
};

/** @} */

/**
 * \name Buffer description
 * @{
 */

/** Type to contain length of described buffers */
typedef uint16_t spi_master_vec_buflen_t;

/** Struct to describe a buffer for writing or reading */
struct spi_master_vec_bufdesc {
	uint8_t *data;
	spi_master_vec_buflen_t length;
};

/** @} */

/**
 * \brief Select or deselect slave
 *
 * Drive the configured Slave Select line low or high, depending on whether the
 * slave is being selected or deselected.
 *
 * \param[in] select
 * \arg \c true to select the slave.
 * \arg \c false to deselect the slave.
 */
static inline void spi_master_vec_select_slave(bool select)
{
	if (select) {
		port_pin_set_output_level(CONF_SPI_MASTER_VEC_SS_PIN, false);
		} else {
		port_pin_set_output_level(CONF_SPI_MASTER_VEC_SS_PIN, true);
	}
}

/**
 * \brief Initialize configuration with default values
 *
 * \param[out] config struct to initialize.
 */
static inline void spi_master_vec_get_config_defaults(
		struct spi_master_vec_config *const config)
{
	config->baudrate = 100000;
	config->gclk_source = GCLK_GENERATOR_0;
	config->padmux_setting = SPI_MASTER_VEC_PADMUX_SETTING_D;
	config->transfer_mode = SPI_MASTER_VEC_TRANSFER_MODE_0;
	config->data_order = SPI_MASTER_VEC_DATA_ORDER_MSB;
	config->pinmux_0 = PINMUX_DEFAULT;
	config->pinmux_1 = PINMUX_DEFAULT;
	config->pinmux_2 = PINMUX_DEFAULT;
	config->pinmux_3 = PINMUX_DEFAULT;
}

#ifdef __cplusplus
extern "C" {
#endif

enum status_code spi_master_vec_init(Sercom *const sercom);
void spi_master_vec_enable(void);
void spi_master_vec_disable(void);
enum status_code spi_master_vec_transceive_buffers_wait(
		struct spi_master_vec_bufdesc tx_bufdescs[],
		struct spi_master_vec_bufdesc rx_bufdescs[]);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* SPI_MASTER_VEC_H */