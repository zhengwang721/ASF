/**
 * \file
 *
 * \brief Serial Peripheral Interface (SPI) driver for SAM.
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
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
 
#include "spi.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_spi_group Serial Peripheral Interface (SPI)
 *
 * The SPI circuit is a synchronous serial data link that provides communication 
 * with external devices in Master or Slave mode. Connection to Peripheral DMA 
 * Controller channel capabilities optimizes data transfers.
 *
 * @{
 */

#define SPI_WPMR_WPKEY_VALUE SPI_WPMR_WPKEY((uint32_t) 0x535049)

/**
 * \brief Reset SPI and set it to Slave mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_reset(Spi *p_spi)
{
	p_spi->SPI_CR = SPI_CR_SWRST;
}

/**
 * \brief Enable SPI.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_enable(Spi *p_spi)
{
	p_spi->SPI_CR = SPI_CR_SPIEN;
}

/**
 * \brief Disable SPI.
 *
 * \note CS is de-asserted, which indicates that the last data is done, and user 
 * should check TX_EMPTY before disabling SPI.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_disable(Spi *p_spi)
{
	p_spi->SPI_CR = SPI_CR_SPIDIS;
}

/**
 * \brief Issue a LASTXFER command. 
 *  The next transfer is the last transfer and after that CS is de-asserted.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_set_lastxfer(Spi *p_spi)
{
	p_spi->SPI_CR = SPI_CR_LASTXFER;
}

/**
 * \brief Set SPI to Master mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_set_master_mode(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_MSTR;
}

/**
 * \brief Set SPI to Slave mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_set_slave_mode(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_MSTR);
}

/**
 * \brief Get SPI work mode.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for master mode, 0 for slave mode.
 */
uint32_t spi_get_mode(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_MSTR) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Set Variable Peripheral Select.
 * Peripheral Chip Select can be controlled by SPI_TDR.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_set_variable_peripheral_select(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_PS;
}

/**
 * \brief Set Fixed Peripheral Select.
 *  Peripheral Chip Select is controlled by SPI_MR.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_set_fixed_peripheral_select(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_PS);
}

/**
 * \brief Get Peripheral Select mode. 
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for Variable mode, 0 for fixed mode.
 */
uint32_t spi_get_peripheral_select_mode(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_PS) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable Peripheral Select Decode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_enable_peripheral_select_decode(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_PCSDEC;
}

/**
 * \brief Disable Peripheral Select Decode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_disable_peripheral_select_decode(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_PCSDEC);
}

/**
 * \brief Get Peripheral Select Decode mode.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for decode mode, 0 for direct mode.
 */
uint32_t spi_get_peripheral_select_decode_setting(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_PCSDEC) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable Mode Fault Detection.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_enable_mode_fault_detect(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_MODFDIS);
}

/**
 * \brief Disable Mode Fault Detection.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_disable_mode_fault_detect(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_MODFDIS;
}

/**
 * \brief Check if mode fault detection is enabled.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for disabled, 0 for enabled.
 */
uint32_t spi_get_mode_fault_detect_setting(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_MODFDIS) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable waiting RX_EMPTY before transfer starts.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_enable_tx_on_rx_empty(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_WDRBT;
}

/**
 * \brief Disable waiting RX_EMPTY before transfer starts.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_disable_tx_on_rx_empty(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_WDRBT);
}

/**
 * \brief Check if SPI waits RX_EMPTY before transfer starts.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for SPI waits, 0 for no wait.
 */
uint32_t spi_get_tx_on_rx_empty_setting(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_WDRBT) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable loopback mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_enable_loopback(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_LLB;
}

/**
 * \brief Disable loopback mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_disable_loopback(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_LLB);
}

/**
 * \brief Set Peripheral Chip Select (PCS) value.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_value Peripheral Chip Select value.
 */
void spi_set_peripheral_chip_select_value(Spi *p_spi, uint32_t ul_value)
{
	p_spi->SPI_MR &= (~SPI_MR_PCS_Msk);
	p_spi->SPI_MR |= SPI_MR_PCS(ul_value);
}

/**
 * \brief Set delay between chip selects (in number of MCK clocks).
 *  If DLYBCS <= 6, 6 MCK clocks will be inserted by default.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_delay Delay between chip selects (in number of MCK clocks).
 */
void spi_set_delay_between_chip_select(Spi *p_spi, uint32_t ul_delay)
{
	p_spi->SPI_MR &= (~SPI_MR_DLYBCS_Msk);
	p_spi->SPI_MR |= SPI_MR_DLYBCS(ul_delay);
}

/**
 * \brief Read the received data and it's peripheral chip select value.
 * While SPI works in fixed peripheral select mode, the peripheral chip select
 * value is meaningless.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param data Pointer to the location where to store the received data word.
 * \param p_pcs Pointer to fill Peripheral Chip Select Value.
 *
 * \retval SPI_OK on Success.
 * \retval SPI_ERROR_TIMEOUT on Time-out.
 */
spi_status_t spi_read(Spi *p_spi, uint16_t *us_data, uint8_t *p_pcs)
{
	uint32_t timeout = SPI_TIMEOUT;
	static uint32_t reg_value;

	while (!(p_spi->SPI_SR & SPI_SR_RDRF)) {
		if (!timeout--) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	reg_value = p_spi->SPI_RDR;
	if (spi_get_peripheral_select_mode(p_spi)) {
		*p_pcs = (uint8_t) ((reg_value & SPI_RDR_PCS_Msk) >> SPI_RDR_PCS_Pos);
	}
	*us_data = (uint16_t) (reg_value & SPI_RDR_RD_Msk);

	return SPI_OK;
}

/**
 * \brief Write the transmitted data with specified peripheral chip select value.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param us_data The data to transmit.
 * \param uc_pcs Peripheral Chip Select Value while SPI works in peripheral select
 * mode, otherwise it's meaningless.
 * \param uc_last Indicate whether this data is the last one while SPI is working 
 * in variable peripheral select mode.
 *
 * \retval SPI_OK on Success.
 * \retval SPI_ERROR_TIMEOUT on Time-out.
 */
spi_status_t spi_write(Spi *p_spi, uint16_t us_data,
		uint8_t uc_pcs, uint8_t uc_last)
{
	uint32_t timeout = SPI_TIMEOUT;
	uint32_t value;

	while (!(p_spi->SPI_SR & SPI_SR_TDRE)) {
		if (!timeout--) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	if (spi_get_peripheral_select_mode(p_spi)) {
		value = SPI_TDR_TD(us_data) | SPI_TDR_PCS(uc_pcs);
		if (uc_last) {
			value |= SPI_TDR_LASTXFER;
		}
	} else {
		value = SPI_TDR_TD(us_data);
	}

	p_spi->SPI_TDR = value;

	return SPI_OK;
}

/**
 * \brief Read status register.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return SPI status register value.
 */
uint32_t spi_read_status(Spi *p_spi)
{
	return p_spi->SPI_SR;
}

/**
 * \brief Test if the SPI is enabled.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 if the SPI is enabled, otherwise 0.
 */
uint32_t spi_is_enabled(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_SPIENS) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if all transmissions are complete.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \retval 1 if transmissions are complete.
 * \retval 0 if transmissions are not complete.
 */
uint32_t spi_is_tx_empty(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_TXEMPTY) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if all transmissions are ready.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \retval 1 if transmissions are complete.
 * \retval 0 if transmissions are not complete.
 */
uint32_t spi_is_tx_ready(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_TDRE) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if the SPI contains a received character.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 if the SPI Receive Holding Register is full, otherwise 0.
 */
uint32_t spi_is_rx_full(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_RDRF) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if all receptions are ready.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 if the SPI Receiver is ready, otherwise 0.
 */
uint32_t spi_is_rx_ready(Spi *p_spi)
{
	if ((p_spi->SPI_SR & (SPI_SR_RDRF | SPI_SR_TXEMPTY))
			== (SPI_SR_RDRF | SPI_SR_TXEMPTY)) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable SPI interrupts.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_sources Interrupts to be enabled.
 */
void spi_enable_interrupt(Spi *p_spi, uint32_t ul_sources)
{
	p_spi->SPI_IER = ul_sources;
}

/**
 * \brief Disable SPI interrupts.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_sources Interrupts to be disabled.
 */
void spi_disable_interrupt(Spi *p_spi, uint32_t ul_sources)
{
	p_spi->SPI_IDR = ul_sources;
}

/**
 * \brief Read SPI interrupt mask.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return The interrupt mask value.
 */
uint32_t spi_read_interrupt_mask(Spi *p_spi)
{
	return p_spi->SPI_IMR;
}

/**
 * \brief Set clock default state.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param ul_polarity Default clock state is logical one(high)/zero(low).
 */
void spi_set_clock_polarity(Spi *p_spi, uint32_t ul_pcs_ch,
		uint32_t ul_polarity)
{
	if (ul_polarity) {
		p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_CPOL;
	} else {
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_CPOL);
	}
}

/**
 * \brief Set Data Capture Phase.
 *
 * \param p_spi Pointer to an SPI instance.
 *  \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 *  \param ul_phase Data capture on the rising/falling edge of clock.
 */
void spi_set_clock_phase(Spi *p_spi, uint32_t ul_pcs_ch, uint32_t ul_phase)
{
	if (ul_phase) {
		p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_NCPHA;
	} else {
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_NCPHA);
	}
}

/**
 * \brief Configure CS behavior for SPI transfer (\ref spi_cs_behavior_t).
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param ul_cs_behavior Behavior of the Chip Select after transfer (\ref spi_cs_behavior_t).
 */
void spi_configure_cs_behavior(Spi *p_spi, uint32_t ul_pcs_ch,
		uint32_t ul_cs_behavior)
{
	if (ul_cs_behavior == SPI_CS_RISE_FORCED) {
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_CSAAT);
		p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_CSNAAT;
	} else if (ul_cs_behavior == SPI_CS_RISE_NO_TX) {
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_CSAAT);
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_CSNAAT);
	} else if (ul_cs_behavior == SPI_CS_KEEP_LOW) {
		p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_CSAAT;
	}
}

/**
 * \brief Set number of bits per transfer.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param ul_bits Number of bits (8~16), use the pattern defined in the device header file.
 */
void spi_set_bits_per_transfer(Spi *p_spi, uint32_t ul_pcs_ch,
		uint32_t ul_bits)
{
	p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_BITS_Msk);
	p_spi->SPI_CSR[ul_pcs_ch] |= ul_bits;
}

/**
 * \brief Set Serial Clock Baud Rate divider value (SCBR).
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param uc_baudrate_divider Baudrate divider from MCK.
 */
void spi_set_baudrate_div(Spi *p_spi, uint32_t ul_pcs_ch,
		uint8_t uc_baudrate_divider)
{
	p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_SCBR_Msk);
	p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_SCBR(uc_baudrate_divider);
}

/**
 * \brief Configure timing for SPI transfer.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param uc_dlybs Delay before SPCK (in number of MCK clocks).
 * \param uc_dlybct Delay between consecutive transfers (in number of MCK clocks).
 */
void spi_set_transfer_delay(Spi *p_spi, uint32_t ul_pcs_ch,
		uint8_t uc_dlybs, uint8_t uc_dlybct)
{
	p_spi->SPI_CSR[ul_pcs_ch] &= ~(SPI_CSR_DLYBS_Msk | SPI_CSR_DLYBCT_Msk);
	p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_DLYBS(uc_dlybs)
			| SPI_CSR_DLYBCT(uc_dlybct);
}


#if (SAM3S || SAM3N || SAM4S)
/**
 * \brief Get PDC registers base address.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return PDC registers base for PDC driver to access.
 */
Pdc *spi_get_pdc_base(Spi *p_spi)
{
	p_spi = p_spi;	/* Stop warning */

	return PDC_SPI;
}
#endif

#if (SAM3U  || SAM3XA)
/**
 * \brief Get transmit data register address for DMA operation.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return Transmit address for DMA access.
 */
void *spi_get_tx_access(Spi *p_spi)
{
	return (void *)&(p_spi->SPI_TDR);
}

/**
 * \brief Get receive data register address for DMA operation.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return Receive address for DMA access.
 */
void *spi_get_rx_access(Spi *p_spi)
{
	return (void *)&(p_spi->SPI_RDR);
}
#endif

/**
 * \brief Enable or disable write protection of SPI registers.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_enable 1 to enable, 0 to disable.
 */
void spi_set_writeprotect(Spi *p_spi, uint32_t ul_enable)
{
	if (ul_enable) {
		p_spi->SPI_WPMR = SPI_WPMR_WPKEY_VALUE | SPI_WPMR_WPEN;
	} else {
		p_spi->SPI_WPMR = SPI_WPMR_WPKEY_VALUE;
	}
}

/**
 * \brief Indicate write protect status.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return SPI_WPSR value.
 */
uint32_t spi_get_writeprotect_status(Spi *p_spi)
{
	return p_spi->SPI_WPSR;
}

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
