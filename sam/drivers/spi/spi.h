/**
 * \file
 *
 * \brief Serial Peripheral Interface (SPI) driver for SAM.
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
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
 
#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

//! Time-out value (number of attempts).
#define SPI_TIMEOUT       15000

//! Status codes used by the SPI driver.
typedef enum
{
	SPI_ERROR = -1,
	SPI_OK = 0,
	SPI_ERROR_TIMEOUT = 1,
	SPI_ERROR_ARGUMENT,
	SPI_ERROR_OVERRUN,
	SPI_ERROR_MODE_FAULT,
	SPI_ERROR_OVERRUN_AND_MODE_FAULT
} spi_status_t;

//! SPI Chip Select behavior modes while transferring.
typedef enum spi_cs_behavior {
	SPI_CS_KEEP_LOW = SPI_CSR_CSAAT,    //!< CS does not rise until a new transfer is requested on different chip select.
	SPI_CS_RISE_NO_TX = 0,              //!< CS rises if there is no more data to transfer.
	SPI_CS_RISE_FORCED = SPI_CSR_CSNAAT //!< CS is de-asserted systematically during a time DLYBCS.
} spi_cs_behavior_t;

void spi_reset(Spi *p_spi);
void spi_enable(Spi *p_spi);
void spi_disable(Spi *p_spi);
void spi_set_lastxfer(Spi *p_spi);
void spi_set_master_mode(Spi *p_spi);
void spi_set_slave_mode(Spi *p_spi);
uint32_t spi_get_mode(Spi *p_spi);
void spi_set_variable_peripheral_select(Spi *p_spi);
void spi_set_fixed_peripheral_select(Spi *p_spi);
uint32_t spi_get_peripheral_select_mode(Spi *p_spi);
void spi_enable_peripheral_select_decode(Spi *p_spi);
void spi_disable_peripheral_select_decode(Spi *p_spi);
uint32_t spi_get_peripheral_select_decode_setting(Spi *p_spi);
void spi_enable_mode_fault_detect(Spi *p_spi);
void spi_disable_mode_fault_detect(Spi *p_spi);
uint32_t spi_get_mode_fault_detect_setting(Spi *p_spi);
void spi_enable_tx_on_rx_empty(Spi *p_spi);
void spi_disable_tx_on_rx_empty(Spi *p_spi);
uint32_t spi_get_tx_on_rx_empty_setting(Spi *p_spi);
void spi_enable_loopback(Spi *p_spi);
void spi_disable_loopback(Spi *p_spi);
void spi_set_peripheral_chip_select_value(Spi *p_spi, uint32_t ul_value);
void spi_set_delay_between_chip_select(Spi *p_spi, uint32_t ul_delay);
spi_status_t spi_read(Spi *p_spi, uint16_t *us_data, uint8_t *p_pcs);
spi_status_t spi_write(Spi *p_spi, uint16_t us_data, uint8_t uc_pcs, uint8_t uc_last);
uint32_t spi_read_status(Spi *p_spi);
uint32_t spi_is_enabled(Spi *p_spi);
uint32_t spi_is_tx_empty(Spi *p_spi);
uint32_t spi_is_tx_ready(Spi *p_spi);
uint32_t spi_is_rx_full(Spi *p_spi);
uint32_t spi_is_rx_ready(Spi *p_spi);
void spi_enable_interrupt(Spi *p_spi, uint32_t ul_sources);
void spi_disable_interrupt(Spi *p_spi, uint32_t ul_sources);
uint32_t spi_read_interrupt_mask(Spi *p_spi);
void spi_set_clock_polarity(Spi *p_spi, uint32_t ul_pcs_ch, uint32_t ul_polarity);
void spi_set_clock_phase(Spi *p_spi, uint32_t ul_pcs_ch, uint32_t ul_phase);
void spi_configure_cs_behavior(Spi *p_spi, uint32_t ul_pcs_ch, uint32_t ul_cs_behavior);
void spi_set_bits_per_transfer(Spi *p_spi, uint32_t ul_pcs_ch, uint32_t ul_bits);
void spi_set_baudrate_div(Spi *p_spi, uint32_t ul_pcs_ch, uint8_t uc_baudrate_divider);
void spi_set_transfer_delay(Spi *p_spi, uint32_t ul_pcs_ch, uint8_t uc_dlybs, uint8_t uc_dlybct);

#if (SAM3S || SAM3N || SAM4S)
Pdc* spi_get_pdc_base(Spi *p_spi);
#endif

#if (SAM3U || SAM3XA)
void* spi_get_tx_access(Spi *p_spi);
void* spi_get_rx_access(Spi *p_spi);
#endif

void spi_set_writeprotect(Spi *p_spi, uint32_t ul_enable);
uint32_t spi_get_writeprotect_status(Spi *p_spi);

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* SPI_H_INCLUDED */
