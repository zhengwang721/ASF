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
					/** Width of QSPI Addr , inst data */
			uint32_t b_width:3,
					/** Reserved*/
					reserved0:1,
					/** Enable Inst */
					b_inst_en:1,
					/** Enable Address */
					b_addr_en:1,
					/** Enable Option */
					b_opt_en:1,
					/** Enable Data */
					b_data_en:1,
					/** Option Length*/
					b_opt_len:2,
					/** Addrs Length*/
					b_addr_len:1,
					/** Option Length*/
					reserved1:1,
					/** Transfer type*/
					b_tfr_type:2,
					/** Continoues read mode*/
					b_continues_read:1,
					/** Reserved*/
					reserved2:1,
					/**< Unicast hash match */
					b_dummy_cycles:5,
					/** Reserved*/
					reserved3:11;
		} bm;
	} inst_frame;
  uint32_t       addr;
}qspi_inst_frame_t;

/**
 * \brief qspi buffer structure
 */
typedef struct {
	/** Tx buffer size */
	uint32_t      tx_data_size;
	/** Rx buffer size */
	uint32_t      rx_data_size;
	/** Tx buffer */
	uint32_t      *data_tx;
	/** Rx buffer */
	uint32_t      *data_rx;
}qspi_buffer_t;

/**
 * \brief qspi command structure
 */
typedef struct {
	/** Qspi instruction code*/
	uint8_t       instruction;
	/** Qspi option code*/
	uint8_t       option;
}qspi_mem_cmd_t;

/**
 * \brief qspi driver structure
 */
typedef struct {
	/** QSPI Hw instance */
	Qspi               *qspi_hw;
	/** Qspi mode: SPI or Serial Memory */
	qspi_mode_t        qspi_mode;
	/** Qspi command structure*/
	qspi_mem_cmd_t     qspi_command;
	/** Qspi buffer*/
	qspi_buffer_t      qspi_buffer;
	/** Qspi QSPI mode Frame register informations*/
	qspi_inst_frame_t  *qspi_frame;
}qspid_t;

/**
 * \brief qspi config structure
 */
typedef struct {
	/** Qspi serial memory mode*/
	uint32_t       serial_memory;
	/** local loopback enable*/
	uint32_t       loopback_en;
	/** wait data read for transfer*/
	uint32_t       wait_data_for_transfer;
	/** chip select mode*/
	uint32_t       csmode;
	/** numbers of bits per transfer*/
	uint32_t       bits_per_transfer;
	/** minimum inactive QCS delay*/
	uint32_t       min_delay_qcs;
	/** delay between consecutive transfers*/
	uint32_t       delay_between_ct;
	/** Qspi clock polarity*/
	uint32_t       clock_polarity;
	/** Qspi clock phase*/
	uint32_t       clock_phase;
	/** Qspi baudrate*/
	uint32_t       baudrate;
	/** Qspi transfer delay*/
	uint32_t       transfer_delay;
	/** Scrambling/Unscrambling Enable*/
	uint32_t      scrambling_en;
	/** Scrambling/Unscrambling Random Value Disable*/
	uint32_t      scrambling_random_value_dis;
	/** Scrambling User Key*/
	uint32_t      scrambling_user_key;
}qspi_config_t;

/**
 * \brief qspi access modes
 */
typedef enum{
	/** cmd access*/
	cmd_access  = 0,
	/** read access*/
	read_access,
	/** write access*/
	write_access
}qspi_access_t;

/**
 * \brief Issue a LASTXFER command.
 *  The next transfer is the last transfer and after that CS is de-asserted.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_lastxfer(Qspi *qspi)
{
	qspi->QSPI_CR = QSPI_CR_LASTXFER;
}

/**
 * \brief Set QSPI to Memory mode.
 *
 * \param spi Pointer to an QSPI instance.
 */
static inline void qspi_set_memory_mode(Qspi *qspi)
{
	qspi->QSPI_MR |= QSPI_MR_SMM_MEMORY;
}

/**
 * \brief Set QSPI to SPI mode (Master mode only).
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_spi_mode(Qspi *qspi)
{
	qspi->QSPI_MR &= (~QSPI_MR_SMM_SPI);
}

/**
 * \brief Enable loopback mode.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_enable_loopback(Qspi *qspi)
{
	qspi->QSPI_MR |= QSPI_MR_LLB;
}

/**
 * \brief Disable loopback mode.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_disable_loopback(Qspi *qspi)
{
	qspi->QSPI_MR &= (~QSPI_MR_LLB);
}

/**
 * \brief Enable waiting RX_EMPTY before transfer starts.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_wait_data_read_before_transfer(Qspi *qspi)
{
	qspi->QSPI_MR |= QSPI_MR_WDRBT;
}

/**
 * \brief Disable waiting RX_EMPTY before transfer starts.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_disable_tx_on_rx_empty(Qspi *qspi)
{
	qspi->QSPI_MR &= (~QSPI_MR_WDRBT);
}

/**
 * \brief Set Chip Select Mode.
 *
 * \param qspi Pointer to an QSPI instance.
 * \param csmode Chip select mode to be set.
 */
static inline void qspi_set_chip_select_mode(Qspi *qspi, uint32_t csmode)
{
	uint32_t mask = qspi->QSPI_MR & (~QSPI_MR_CSMODE_Msk);
	qspi->QSPI_MR = mask | QSPI_MR_CSMODE(csmode);
}

/**
 * \brief Set the number of data bits transferred.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_bits_per_transfer(Qspi *qspi, uint32_t bits)
{
	uint32_t mask = qspi->QSPI_MR & (~QSPI_MR_NBBITS_Msk);
	qspi->QSPI_MR = mask | bits;
}

/**
 * \brief Enable QSPI interrupts.
 *
 * \param qspi Pointer to an QSPI instance.
 * \param sources Interrupts to be enabled.
 */
static inline void qspi_enable_interrupt(Qspi *qspi, uint32_t sources)
{
	qspi->QSPI_IER = sources;
}

/**
 * \brief Disable QSPI interrupts.
 *
 * \param qspi Pointer to an QSPI instance.
 * \param sources Interrupts to be disabled.
 */
static inline void qspi_disable_interrupt(Qspi *qspi, uint32_t sources)
{
	qspi->QSPI_IDR = sources;
}

/**
 * \brief Resets a QSPI peripheral.
 *
 * \param qspi  Pointer to a Qspi instance.
 */
static inline void qspi_reset( Qspi *qspi )
{
	assert(qspi);
	qspi->QSPI_CR = QSPI_CR_SWRST ;
}

/**
 * \brief Enables a QSPI peripheral.
 *
 * \param qspi  Pointer to a Qspi instance.
 */
static inline void qspi_enable( Qspi *qspi )
{
	assert(qspi);
	qspi->QSPI_CR = QSPI_CR_QSPIEN;
	while(!(qspi->QSPI_SR & QSPI_SR_QSPIENS));
}

/**
 * \brief Disables a QSPI peripheral.
 *
 * \param qspi  Pointer to a Qspi instance.
 */
static inline void qspi_disable( Qspi *qspi )
{
	assert(qspi);
	qspi->QSPI_CR = QSPI_CR_QSPIDIS ;
	while(qspi->QSPI_SR & QSPI_SR_QSPIENS);
}

/**
 * \brief Set a QSPI peripheral run mode.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param mode qspi run mode to be set.
 */
static inline void qspi_set_run_mode(Qspi *qspi, uint32_t mode)
{
	assert(qspi);
	if (mode == 0) {
		qspi->QSPI_MR &= (~QSPI_MR_SMM_MEMORY);
	} else {
		qspi->QSPI_MR |= QSPI_MR_SMM_MEMORY;
	}
}

/**
 * \brief Set qspi minimum inactive qcs delay.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param uc_dlybct Time to be delay.
 */
static inline void qspi_set_minimum_inactive_qcs_delay(Qspi *qspi, uint8_t uc_dlybct)
{
	assert(qspi);

	uint32_t mask = qspi->QSPI_MR & (~QSPI_MR_DLYBCT_Msk);
	qspi->QSPI_MR = mask | QSPI_MR_DLYBCT(uc_dlybct);
}

/**
 * \brief Set qspi delay between consecutive transfers.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param uc_dlycs Time to be delay.
 */
static inline void qspi_set_delay_between_consecutive_transfers(Qspi *qspi, uint32_t uc_dlycs)
{
	assert(qspi);

	uint32_t mask = qspi->QSPI_MR & (~QSPI_MR_DLYCS_Msk);
	qspi->QSPI_MR = mask | QSPI_MR_DLYCS(uc_dlycs);
}

/**
 * \brief Ends ongoing transfer by releasing CS of QSPI peripheral.
 *
 * \param pQspi  Pointer to an Qspi instance.
 */
static inline void qspi_end_transfer(Qspi *qspi)
{
	assert(qspi);
	while(!(qspi->QSPI_SR & QSPI_SR_TXEMPTY));
	qspi->QSPI_CR = QSPI_CR_LASTXFER;
}

/**
 * \brief Set qspi clock polarity.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param polarity Polarity to be set.
 */
static inline void qspi_set_clock_polarity(Qspi *qspi, uint32_t polarity)
{
	assert(qspi);
	if (polarity) {
		qspi->QSPI_SCR |= QSPI_SCR_CPOL;
	} else {
		qspi->QSPI_SCR &= (~QSPI_SCR_CPOL);
	}
}

/**
 * \brief Set qspi clock phase.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param phase Phase to be set.
 */
static inline void qspi_set_clock_phase(Qspi *qspi, uint32_t phase)
{
	assert(qspi);
	if (phase) {
		qspi->QSPI_SCR |= QSPI_SCR_CPHA;
	} else {
		qspi->QSPI_SCR &= (~QSPI_SCR_CPHA);
	}
}

/**
 * \brief Set qspi clock baudrate.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param uc_baudrate_div Baudrate_div to be set.
 */
static inline void qspi_set_baudrate(Qspi *qspi, uint8_t uc_baudrate_div)
{
	assert(qspi);
	uint32_t mask = qspi->QSPI_SCR & (~QSPI_SCR_SCBR_Msk);
	qspi->QSPI_SCR = mask | QSPI_SCR_SCBR(uc_baudrate_div);
}

/**
 * \brief Set qspi clock transfer delay.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param uc_dlybs Delay before QSCK.
 */
static inline void qspi_set_transfer_delay(Qspi *qspi, uint8_t uc_dlybs)
{
	assert(qspi);
	uint32_t mask = qspi->QSPI_SCR & (~QSPI_SCR_DLYBS_Msk);
	qspi->QSPI_SCR = mask | QSPI_SCR_DLYBS(uc_dlybs);
}

/**
 * \brief Set qspi instruction addr.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param addr Address to be set.
 */
static inline void qspi_set_instruction_addr(Qspi *qspi, uint32_t addr)
{
	assert(qspi);
	qspi->QSPI_IAR = QSPI_IAR_ADDR(addr);
}

/**
 * \brief Set qspi instruction code.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param instruction_code Code to be set.
 */
static inline void qspi_set_instruction_code(Qspi *qspi, qspi_mem_cmd_t instruction_code)
{
	assert(qspi);
	qspi->QSPI_ICR = QSPI_ICR_INST(instruction_code.instruction) | QSPI_ICR_OPT(instruction_code.option);
}

/**
 * \brief Set qspi instruction frame.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param instruction_frame Frame to be set.
 */
static inline void qspi_set_instruction_frame(Qspi *qspi, qspi_inst_frame_t instruction_frame)
{
	assert(qspi);
	uint32_t mask = 0;
	mask |= QSPI_IFR_WIDTH(instruction_frame.inst_frame.bm.b_width);
	if (instruction_frame.inst_frame.bm.b_inst_en) {
		mask |= QSPI_IFR_INSTEN;
	}
	if (instruction_frame.inst_frame.bm.b_addr_en) {
		mask |= QSPI_IFR_ADDREN;
	}
	if (instruction_frame.inst_frame.bm.b_opt_en) {
		mask |= QSPI_IFR_OPTEN;
	}
	if (instruction_frame.inst_frame.bm.b_data_en) {
		mask |= QSPI_IFR_DATAEN;
	}
	mask |= QSPI_IFR_OPTL(instruction_frame.inst_frame.bm.b_opt_len);
	if (instruction_frame.inst_frame.bm.b_addr_len) {
		mask |= QSPI_IFR_ADDRL_32_BIT;
	}
	mask |= QSPI_IFR_TFRTYP(instruction_frame.inst_frame.bm.b_tfr_type);
	if (instruction_frame.inst_frame.bm.b_continues_read) {
		mask |= QSPI_IFR_CRM_ENABLED;
	}
	mask |= QSPI_IFR_NBDUM(instruction_frame.inst_frame.bm.b_dummy_cycles);

	qspi->QSPI_IFR = mask;
}

/**
 * \brief Reads the Instruction frame of QSPI
 *
 * \param pQspi   Pointer to an Qspi instance.
 */
static inline uint32_t qspi_get_inst_frame(Qspi *qspi)
{
	assert(qspi);
	return qspi->QSPI_IFR;
}

/**
 * \brief Set qspi scrambling mode.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param mode Mode to be set.
 */
static inline void qspi_set_scrambling_mode(Qspi *qspi, uint32_t scr_enable, uint32_t random_mode)
{
	assert(qspi);
	qspi->QSPI_SMR &= (~(QSPI_SMR_SCREN | QSPI_SMR_RVDIS));

	if (scr_enable) {
		qspi->QSPI_SCR |= QSPI_SMR_SCREN;
	} else {
		qspi->QSPI_SCR &= (~QSPI_SMR_SCREN);
	}

	if (random_mode) {
		qspi->QSPI_SCR |= QSPI_SMR_RVDIS;
	} else {
		qspi->QSPI_SCR &= (~QSPI_SMR_RVDIS);
	}
}

/**
 * \brief Set qspi scrambling key.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param mode Mode to be set.
 * \return If write succeeded, return 0, else return 1.
 */
static inline uint32_t qspi_set_scrambing_key(Qspi *qspi, uint32_t key)
{
	assert(qspi);
	if (qspi->QSPI_WPMR & QSPI_WPMR_WPEN) {
		return 1;
	}
	qspi->QSPI_SKR = QSPI_SKR_USRK(key);

	return 0;
}

/**
 * \brief Enable write protection.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \enable Protection status to be set.
 */
static inline void qspi_set_writeprotect(Qspi *qspi, uint32_t enable)
{
	assert(qspi);
	if (enable) {
		qspi->QSPI_WPMR |= (QSPI_WPMR_WPKEY_PASSWD | QSPI_WPMR_WPEN);
	} else {
		qspi->QSPI_WPMR &= (~(QSPI_WPMR_WPKEY_PASSWD | QSPI_WPMR_WPEN));
	}
}

/**
 * \brief Read QSPI RDR register for SPI mode
 *
 * \param qspi   Pointer to an Qspi instance.
 */
static inline uint16_t qspi_read_spi( Qspi *qspi )
{
	assert(qspi);
	while(!(qspi->QSPI_SR & QSPI_SR_RDRF));
	return  qspi->QSPI_RDR;
}

/**
 * \brief Write to QSPI Tx register in SPI mode
 *
 * \param qspi   Pointer to an Qspi instance.
 * \param w_data   Data to transmit
 */
static inline void qspi_write_spi( Qspi *qspi, uint16_t w_data)
{
	assert(qspi);
	/* Send data */
	while(!(qspi->QSPI_SR & QSPI_SR_TXEMPTY));
	qspi->QSPI_TDR = w_data ;
	while(!(qspi->QSPI_SR & QSPI_SR_TDRE));
}

/**
 * \brief Get write protection status.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \return Write protection status.
 */
static inline uint32_t qspi_get_writeprotect_status(Qspi *qspi)
{
	assert(qspi);
	return qspi->QSPI_WPSR;
}

void qspi_set_config(Qspi *qspi, qspi_config_t *qspi_config);
void qspi_get_default_config(qspi_config_t * qspi_config);
enum status_code qspi_read(Qspi *qspi, uint16_t *us_data, uint32_t num_of_bytes);
enum status_code qspi_write(Qspi *qspi, uint16_t *us_data, uint32_t num_of_bytes);

/** Functionality API -- Serial Memory Mode */
enum status_code qspi_flash_send_command(qspid_t *qspid, uint8_t const keep_cfg);
enum status_code qspi_flash_send_command_with_data(qspid_t *qspid, uint8_t const keep_cfg);
enum status_code qspi_flash_read_command(qspid_t *qspid, uint8_t const keep_cfg);
enum status_code qspi_flash_enable_mem_access(qspid_t *qspid, uint8_t const keep_cfg, uint8_t scramble_flag);
enum status_code qspi_flash_read_write_mem(qspid_t *qspid, qspi_access_t const read_write);

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
		void qspi_set_config(qspi_config_t * qspi_config)
		{
			qspi_disable(qspi_config->qspi_hw);
			qspi_reset(qspi_config->qspi_hw);
			qspi_set_lastxfer(qspi_config->qspi_hw);
			qspi_set_chip_select_mode(qspi_config->qspi_hw, qspi_config->csmode);
			qspi_set_clock_polarity(qspi_config->qspi_hw, qspi_config->clock_polarity);
			qspi_set_clock_phase(qspi_config->qspi_hw, qspi_config->clock_phase);
			qspi_set_bits_per_transfer(qspi_config->qspi_hw, qspi_config->bits_per_transfer);
			qspi_set_baudrate(qspi_config->qspi_hw, qspi_config->baudrate);
			qspi_set_transfer_delay(qspi_config->qspi_hw, qspi_config->transfer_delay);
			qspi_set_memory_mode(qspi_config->qspi_hw);
			qspi_enable(qspi_config->qspi_hw);
		}

		void qspi_get_default_config(qspi_config_t * qspi_config)
		{
			qspi_config->qspi_hw = QSPI;
			qspi_config->csmode = 1;
			qspi_config->clock_polarity = 0;
			qspi_config->clock_phase = 0;
			qspi_config->bits_per_transfer = QSPI_MR_NBBITS_8_BIT;
			qspi_config->baudrate = sysclk_get_cpu_hz() / 1000000;
			qspi_config->transfer_delay = 0x40;
		}
\endcode
 */
#endif /* QSPI_H_INCLUDED */
