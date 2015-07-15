/**
 * \file
 *
 * \brief Quad Serial Peripheral Interface (QSPI) driver for SAM.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef QQSPI_H_INCLUDED
#define QQSPI_H_INCLUDED

#include "compiler.h"
#include "string.h"
#include "status_codes.h"
#include <assert.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#ifndef QSPI_WPMR_WPKEY_PASSWD
#define QSPI_WPMR_WPKEY_PASSWD QSPI_WPMR_WPKEY((uint32_t) 0x515350)
#endif

/**
 * \brief qspi enum types for QSPI modes.
 */
typedef enum{
	spi_mode = QSPI_MR_SMM_SPI,
	mem_mode = QSPI_MR_SMM_MEMORY
}qspi_mode_t;

/**
 * \brief qspi frame structure for QSPI mode
 */
typedef struct {
	union _qspiinst_frame {
		uint32_t val;
		struct _qspiinst_frame_bitmap {
			uint32_t b_width:3,           /** Width of QSPI Addr , inst data */
					reserved0:1,         /** Reserved*/
					b_inst_en:1,         /** Enable Inst */
					b_addr_en:1,         /** Enable Address */
					b_opt_en:1,          /** Enable Option */
					b_data_en:1,         /** Enable Data */
					b_opt_len:2,         /** Option Length*/
					b_addr_len:1,        /** Addrs Length*/
					reserved1:1,         /** Option Length*/
					b_tfr_type:2,        /** Transfer type*/
					b_continues_read:1,  /** Continoues read mode*/
					reserved2:1,         /** Reserved*/
					b_dummy_cycles:5,    /**< Unicast hash match */
					reserved3:11;        /** Reserved*/
		} bm;
	} inst_frame;
  uint32_t       addr;
}qspi_inst_frame_t;

/**
 * \brief qspi buffer structure
 */
typedef struct {
  uint32_t      tx_data_size;     /* Tx buffer size */
  uint32_t      rx_data_size;     /* Rx buffer size */
  uint32_t      *p_data_tx;       /* Tx buffer */
  uint32_t      *p_data_rx;       /* Rx buffer */
}qspi_buffer_t;

/**
 * \brief qspi command structure
 */
typedef struct {
  uint8_t       instruction;
  uint8_t       option;
}qspi_mem_cmd_t;

/**
 * \brief qspi driver structure
 */
typedef struct {
  Qspi               *p_qspi_hw;       /* QSPI Hw instance */
  qspi_mode_t        *qspi_mode;       /* Qspi mode: SPI or Serial Memory */
  qspi_mem_cmd_t     qspi_command;     /* Qspi command structure*/
  qspi_buffer_t      qspi_buffer;      /* Qspi buffer*/
  qspi_inst_frame_t  *p_qspi_frame;    /* Qspi QSPI mode Fram register informations*/
}qspid_t;

/**
 * \brief qspi config structure
 */
typedef struct {
	Qspi           *p_qspi_hw;
	uint32_t       ul_csmode;
	uint32_t       ul_clock_polarity;
	uint32_t       ul_clock_phase;
	uint32_t       ul_bits_per_transfer;
	uint32_t       ul_baudrate;
	uint32_t       ul_transfer_delay;
	uint32_t       ul_qspi_mode;
}qspi_config_t;

/**
 * \brief qspi access modes
 */
typedef enum{
	cmd_access  = 0,
	read_access,
	write_access
}qspi_access_t;

/**
 * \brief Issue a LASTXFER command.
 *  The next transfer is the last transfer and after that CS is de-asserted.
 *
 * \param p_qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_lastxfer(Qspi *p_qspi)
{
	p_qspi->QSPI_CR = QSPI_CR_LASTXFER;
}

/**
 * \brief Set QSPI to Memory mode.
 *
 * \param p_spi Pointer to an QSPI instance.
 */
static inline void qspi_set_memory_mode(Qspi *p_qspi)
{
	p_qspi->QSPI_MR |= QSPI_MR_SMM_MEMORY;
}

/**
 * \brief Set QSPI to SPI mode (Master mode only).
 *
 * \param p_qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_spi_mode(Qspi *p_qspi)
{
	p_qspi->QSPI_MR &= (~QSPI_MR_SMM_SPI);
}

/**
 * \brief Enable loopback mode.
 *
 * \param p_qspi Pointer to an QSPI instance.
 */
static inline void qspi_enable_loopback(Qspi *p_qspi)
{
	p_qspi->QSPI_MR |= QSPI_MR_LLB;
}

/**
 * \brief Disable loopback mode.
 *
 * \param p_qspi Pointer to an QSPI instance.
 */
static inline void qspi_disable_loopback(Qspi *p_qspi)
{
	p_qspi->QSPI_MR &= (~QSPI_MR_LLB);
}

/**
 * \brief Enable waiting RX_EMPTY before transfer starts.
 *
 * \param p_qspi Pointer to an QSPI instance.
 */
static inline void qspi_enable_tx_on_rx_empty(Qspi *p_qspi)
{
	p_qspi->QSPI_MR |= QSPI_MR_WDRBT;
}

/**
 * \brief Disable waiting RX_EMPTY before transfer starts.
 *
 * \param p_qspi Pointer to an QSPI instance.
 */
static inline void qspi_disable_tx_on_rx_empty(Qspi *p_qspi)
{
	p_qspi->QSPI_MR &= (~QSPI_MR_WDRBT);
}

/**
 * \brief Set Chip Select Mode.
 *
 * \param p_qspi Pointer to an QSPI instance.
 * \param ul_csmode Chip select mode to be set.
 */
static inline void qspi_set_chip_select_mode(Qspi *p_qspi, uint32_t ul_csmode)
{
	uint32_t ul_mask = p_qspi->QSPI_MR & (~QSPI_MR_CSMODE_Msk);
	p_qspi->QSPI_MR = ul_mask | QSPI_MR_CSMODE(ul_csmode);
}

/**
 * \brief Set the number of data bits transferred.
 *
 * \param p_qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_bits_per_transfer(Qspi *p_qspi, uint32_t ul_bits)
{
	uint32_t ul_mask = p_qspi->QSPI_MR & (~QSPI_MR_NBBITS_Msk);
	p_qspi->QSPI_MR = ul_mask | ul_bits;
}

/**
 * \brief Enable QSPI interrupts.
 *
 * \param p_qspi Pointer to an QSPI instance.
 * \param ul_sources Interrupts to be enabled.
 */
static inline void qspi_enable_interrupt(Qspi *p_qspi, uint32_t ul_sources)
{
	p_qspi->QSPI_IER = ul_sources;
}

/**
 * \brief Disable QSPI interrupts.
 *
 * \param p_qspi Pointer to an QSPI instance.
 * \param ul_sources Interrupts to be disabled.
 */
static inline void qspi_disable_interrupt(Qspi *p_qspi, uint32_t ul_sources)
{
	p_qspi->QSPI_IDR = ul_sources;
}

/**
 * \brief Resets a QSPI peripheral.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 */
static inline void qspi_reset( Qspi *p_qspi )
{
	assert(p_qspi);
	p_qspi->QSPI_CR = QSPI_CR_SWRST ;
}

/**
 * \brief Enables a QSPI peripheral.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 */
static inline void qspi_enable( Qspi *p_qspi )
{
	assert(p_qspi);
	p_qspi->QSPI_CR = QSPI_CR_QSPIEN;
	while(!(p_qspi->QSPI_SR & QSPI_SR_QSPIENS));
}

/**
 * \brief Disables a QSPI peripheral.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 */
static inline void qspi_disable( Qspi *p_qspi )
{
	assert(p_qspi);
	p_qspi->QSPI_CR = QSPI_CR_QSPIDIS ;
	while(p_qspi->QSPI_SR & QSPI_SR_QSPIENS);
}

/**
 * \brief Set a QSPI peripheral run mode.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_mode p_qspi run mode to be set.
 */
static inline void qspi_set_run_mode(Qspi *p_qspi, uint32_t ul_mode)
{
	assert(p_qspi);
	if (ul_mode == 0) {
		p_qspi->QSPI_MR &= (~QSPI_MR_SMM_MEMORY);
	} else {
		p_qspi->QSPI_MR |= QSPI_MR_SMM_MEMORY;
	}
}

/**
 * \brief Set qspi minimum inactive qcs delay.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param uc_dlybct Time to be delay.
 */
static inline void qspi_set_minimum_inactive_qcs_delay(Qspi *p_qspi, uint8_t uc_dlybct)
{
	assert(p_qspi);

	uint32_t ul_mask = p_qspi->QSPI_MR & (~QSPI_MR_DLYBCT_Msk);
	p_qspi->QSPI_MR = ul_mask | QSPI_MR_DLYBCT(uc_dlybct);
}

/**
 * \brief Set qspi delay between consecutive transfers.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param uc_dlycs Time to be delay.
 */
static inline void qspi_set_delay_between_consecutive_transfers(Qspi *p_qspi, uint32_t uc_dlycs)
{
	assert(p_qspi);

	uint32_t ul_mask = p_qspi->QSPI_MR & (~QSPI_MR_DLYCS_Msk);
	p_qspi->QSPI_MR = ul_mask | QSPI_MR_DLYCS(uc_dlycs);
}

/**
 * \brief Ends ongoing transfer by releasing CS of QSPI peripheral.
 *
 * \param pQspi  Pointer to an Qspi instance.
 */
static inline void qspi_end_transfer(Qspi *p_qspi)
{
	assert(p_qspi);
	while(!(p_qspi->QSPI_SR & QSPI_SR_TXEMPTY));
	p_qspi->QSPI_CR = QSPI_CR_LASTXFER;
}

/**
 * \brief Set qspi clock polarity.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_polarity Polarity to be set.
 */
static inline void qspi_set_clock_polarity(Qspi *p_qspi, uint32_t ul_polarity)
{
	assert(p_qspi);
	if (ul_polarity) {
		p_qspi->QSPI_SCR |= QSPI_SCR_CPOL;
	} else {
		p_qspi->QSPI_SCR &= (~QSPI_SCR_CPOL);
	}
}

/**
 * \brief Set qspi clock phase.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_phase Phase to be set.
 */
static inline void qspi_set_clock_phase(Qspi *p_qspi, uint32_t ul_phase)
{
	assert(p_qspi);
	if (ul_phase) {
		p_qspi->QSPI_SCR |= QSPI_SCR_CPHA;
	} else {
		p_qspi->QSPI_SCR &= (~QSPI_SCR_CPHA);
	}
}

/**
 * \brief Set qspi clock baudrate.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param uc_baudrate_div Baudrate_div to be set.
 */
static inline void qspi_set_baudrate(Qspi *p_qspi, uint8_t uc_baudrate_div)
{
	assert(p_qspi);
	uint32_t ul_mask = p_qspi->QSPI_SCR & (~QSPI_SCR_SCBR_Msk);
	p_qspi->QSPI_SCR = ul_mask | QSPI_SCR_SCBR(uc_baudrate_div);
}

/**
 * \brief Set qspi clock transfer delay.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param uc_dlybs Delay before QSCK.
 */
static inline void qspi_set_transfer_delay(Qspi *p_qspi, uint8_t uc_dlybs)
{
	assert(p_qspi);
	uint32_t ul_mask = p_qspi->QSPI_SCR & (~QSPI_SCR_DLYBS_Msk);
	p_qspi->QSPI_SCR = ul_mask | QSPI_SCR_DLYBS(uc_dlybs);
}

/**
 * \brief Set qspi instruction addr.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_addr Address to be set.
 */
static inline void qspi_set_instruction_addr(Qspi *p_qspi, uint32_t ul_addr)
{
	assert(p_qspi);
	p_qspi->QSPI_IAR = QSPI_IAR_ADDR(ul_addr);
}

/**
 * \brief Set qspi instruction code.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_instruction_code Code to be set.
 */
static inline void qspi_set_instruction_code(Qspi *p_qspi, qspi_mem_cmd_t ul_instruction_code)
{
	assert(p_qspi);
	p_qspi->QSPI_ICR = QSPI_ICR_INST(ul_instruction_code.instruction) | QSPI_ICR_OPT(ul_instruction_code.option);
}

/**
 * \brief Set qspi instruction frame.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_instruction_frame Frame to be set.
 */
static inline void qspi_set_instruction_frame(Qspi *p_qspi, qspi_inst_frame_t ul_instruction_frame)
{
	assert(p_qspi);
	uint32_t ul_mask = 0;
	ul_mask |= QSPI_IFR_WIDTH(ul_instruction_frame.inst_frame.bm.b_width);
	if (ul_instruction_frame.inst_frame.bm.b_inst_en) {
		ul_mask |= QSPI_IFR_INSTEN;
	}
	if (ul_instruction_frame.inst_frame.bm.b_addr_en) {
		ul_mask |= QSPI_IFR_ADDREN;
	}
	if (ul_instruction_frame.inst_frame.bm.b_opt_en) {
		ul_mask |= QSPI_IFR_OPTEN;
	}
	if (ul_instruction_frame.inst_frame.bm.b_data_en) {
		ul_mask |= QSPI_IFR_DATAEN;
	}
	ul_mask |= QSPI_IFR_OPTL(ul_instruction_frame.inst_frame.bm.b_opt_len);
	if (ul_instruction_frame.inst_frame.bm.b_addr_len) {
		ul_mask |= QSPI_IFR_ADDRL_32_BIT;
	}
	ul_mask |= QSPI_IFR_TFRTYP(ul_instruction_frame.inst_frame.bm.b_tfr_type);
	if (ul_instruction_frame.inst_frame.bm.b_continues_read) {
		ul_mask |= QSPI_IFR_CRM_ENABLED;
	}
	ul_mask |= QSPI_IFR_NBDUM(ul_instruction_frame.inst_frame.bm.b_dummy_cycles);

	p_qspi->QSPI_IFR = ul_mask;
}

/**
 * \brief Reads the Instruction frame of QSPI
 *
 * \param pQspi   Pointer to an Qspi instance.
 */
static inline uint32_t qspi_get_inst_frame(Qspi *p_qspi)
{
	assert(p_qspi);
	return p_qspi->QSPI_IFR;
}

/**
 * \brief Set qspi scrambling mode.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_mode Mode to be set.
 */
static inline void qspi_set_scrambling_mode(Qspi *p_qspi, uint32_t ul_scr_enable, uint32_t ul_random_mode)
{
	assert(p_qspi);
	p_qspi->QSPI_SMR &= (~(QSPI_SMR_SCREN | QSPI_SMR_RVDIS));

	if (ul_scr_enable) {
		p_qspi->QSPI_SCR |= QSPI_SMR_SCREN;
	} else {
		p_qspi->QSPI_SCR &= (~QSPI_SMR_SCREN);
	}

	if (ul_random_mode) {
		p_qspi->QSPI_SCR |= QSPI_SMR_RVDIS;
	} else {
		p_qspi->QSPI_SCR &= (~QSPI_SMR_RVDIS);
	}
}

/**
 * \brief Set qspi scrambling key.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_mode Mode to be set.
 * \return If write succeeded, return 0, else return 1.
 */
static inline uint32_t qspi_set_scrambing_key(Qspi *p_qspi, uint32_t ul_key)
{
	assert(p_qspi);
	if (p_qspi->QSPI_WPMR & QSPI_WPMR_WPEN) {
		return 1;
	}
	p_qspi->QSPI_SKR = QSPI_SKR_USRK(ul_key);

	return 0;
}

/**
 * \brief Enable write protection.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \ul_enable Protection status to be set.
 */
static inline void qspi_set_writeprotect(Qspi *p_qspi, uint32_t ul_enable)
{
	assert(p_qspi);
	if (ul_enable) {
		p_qspi->QSPI_WPMR |= (QSPI_WPMR_WPKEY_PASSWD | QSPI_WPMR_WPEN);
	} else {
		p_qspi->QSPI_WPMR &= (~(QSPI_WPMR_WPKEY_PASSWD | QSPI_WPMR_WPEN));
	}
}

/**
 * \brief Read QSPI RDR register for SPI mode
 *
 * \param p_qspi   Pointer to an Qspi instance.
 */
static inline uint16_t qspi_read_spi( Qspi *p_qspi )
{
	assert(p_qspi);
	while(!(p_qspi->QSPI_SR & QSPI_SR_RDRF));
	return  p_qspi->QSPI_RDR;
}

/**
 * \brief Write to QSPI Tx register in SPI mode
 *
 * \param p_qspi   Pointer to an Qspi instance.
 * \param w_data   Data to transmit
 */
static inline void qspi_write_spi( Qspi *p_qspi, uint16_t w_data)
{
	assert(p_qspi);
	/* Send data */
	while(!(p_qspi->QSPI_SR & QSPI_SR_TXEMPTY));
	p_qspi->QSPI_TDR = w_data ;
	while(!(p_qspi->QSPI_SR & QSPI_SR_TDRE));
}

/**
 * \brief Get write protection status.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \return Write protection status.
 */
static inline uint32_t qspi_get_writeprotect_status(Qspi *p_qspi)
{
	assert(p_qspi);
	return p_qspi->QSPI_WPSR;
}

void qspi_set_config(qspi_config_t * p_qspi_config);
void qspi_get_default_config(qspi_config_t * p_qspi_config);
enum status_code qspi_read(Qspi *p_qspi, uint16_t *us_data, uint32_t num_of_bytes);
enum status_code qspi_write(Qspi *p_qspi, uint16_t *us_data, uint32_t num_of_bytes);

/** Functionality API -- Serial Memory Mode */
enum status_code qspi_flash_send_command(qspid_t *p_qspid, uint8_t const keep_cfg);
enum status_code qspi_flash_send_command_with_data(qspid_t *p_qspid, uint8_t const keep_cfg);
enum status_code qspi_flash_read_command(qspid_t *p_qspid, uint8_t const keep_cfg);
enum status_code qspi_flash_enable_mem_access(qspid_t *p_qspid, uint8_t const keep_cfg, uint8_t scramble_flag);
enum status_code qspi_flash_read_write_mem(qspid_t *p_qspid, qspi_access_t const read_write);

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \page sam_spi_quickstart Quickstart guide for SAM QSPI driver
 *
 * This is the quickstart guide for the \ref spi_group "SAM QSPI driver",
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g.the main application function.
 *
 * \section spi_basic_use_case Basic use case
 * In this basic use case, the QSPI module are configured for:
 * - SPI mode
 * - Serial Memory Mode
 *
 * \subsection sam_spi_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 * \section spi_basic_use_case_setup Setup steps
 * \subsection spi_basic_use_case_setup_code Example code
 * Add to application C-file:
 * \code
		void qspi_set_config(qspi_config_t * p_qspi_config)
		{
			qspi_disable(p_qspi_config->p_qspi_hw);
			qspi_reset(p_qspi_config->p_qspi_hw);
			qspi_set_lastxfer(p_qspi_config->p_qspi_hw);
			qspi_set_chip_select_mode(p_qspi_config->p_qspi_hw, p_qspi_config->ul_csmode);
			qspi_set_clock_polarity(p_qspi_config->p_qspi_hw, p_qspi_config->ul_clock_polarity);
			qspi_set_clock_phase(p_qspi_config->p_qspi_hw, p_qspi_config->ul_clock_phase);
			qspi_set_bits_per_transfer(p_qspi_config->p_qspi_hw, p_qspi_config->ul_bits_per_transfer);
			qspi_set_baudrate(p_qspi_config->p_qspi_hw, p_qspi_config->ul_baudrate);
			qspi_set_transfer_delay(p_qspi_config->p_qspi_hw, p_qspi_config->ul_transfer_delay);
			qspi_set_memory_mode(p_qspi_config->p_qspi_hw);
			qspi_enable(p_qspi_config->p_qspi_hw);
		}

		void qspi_get_default_config(qspi_config_t * p_qspi_config)
		{
			p_qspi_config->p_qspi_hw = QSPI;
			p_qspi_config->ul_csmode = 1;
			p_qspi_config->ul_clock_polarity = 0;
			p_qspi_config->ul_clock_phase = 0;
			p_qspi_config->ul_bits_per_transfer = QSPI_MR_NBBITS_8_BIT;
			p_qspi_config->ul_baudrate = sysclk_get_cpu_hz() / 1000000;
			p_qspi_config->ul_transfer_delay = 0x40;
		}
\endcode
 */
#endif /* QSPI_H_INCLUDED */
