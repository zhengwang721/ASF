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
#include <assert.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** \brief qspi modes SPI or QSPI
 */
typedef enum{
     SpiMode = QSPI_MR_SMM_SPI,
     QspiMemMode = QSPI_MR_SMM_MEMORY
}qspi_mode_t;

/** \brief qspi frame structure for QSPI mode
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

/** \brief qspi buffer structure
 */
typedef struct {
  uint32_t      tx_data_size;     /* Tx buffer size */
  uint32_t      rx_data_size;     /* Rx buffer size */
  uint32_t      *p_data_tx;       /* Tx buffer */
  uint32_t      *p_data_rx;       /* Rx buffer */
}qspi_buffer_t;
      
/** \brief qspi command structure
 */
typedef struct {
  uint8_t       instruction; 
  uint8_t       option;  
}qspi_mem_cmd_t;

/** \brief qspi driver structure
 */
typedef struct {
  Qspi               *p_qspi_hw;       /* QSPI Hw instance */
  qspi_mode_t        *qspi_mode;       /* Qspi mode: SPI or Serial Memory */
  qspi_mem_cmd_t     qspi_command;     /* Qspi command structure*/
  qspi_buffer_t      qspi_buffer;      /* Qspi buffer*/
  qspi_inst_frame_t  *p_qspi_frame;    /* Qspi QSPI mode Fram register informations*/
}qspid_t;

/** Status codes used by the QSPI driver. */
typedef enum
{
	QSPI_SUCCESS = 0,
	QSPI_BUSY,
	QSPI_BUSY_SENDING,
	QSPI_READ_ERROR,
	QSPI_WRITE_ERROR,
	QSPI_UNKNOWN_ERROR,
	QSPI_INIT_ERROR,
	QSPI_INPUT_ERROR,
	QSPI_TOTAL_ERROR
} qspi_status_t;

/** \brief qspi access modes 
 */
typedef enum{
     CmdAccess = 0,
     ReadAccess,
     WriteAccess
}access_t;

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

void qspi_reset(Qspi *p_qspi);
void qspi_enable(Qspi *p_qspi);
void qspi_disable(Qspi *p_qspi);
void qspi_set_run_mode(Qspi *p_qspi, uint32_t ul_mode);
void qspi_set_minimum_inactive_qcs_delay(Qspi *p_qspi, uint8_t uc_dlybct);
void qspi_set_delay_between_consecutive_transfers(Qspi *p_qspi, uint32_t uc_dlycs);
qspi_status_t qspi_read(Qspi *p_qspi, uint16_t *us_data, uint32_t num_of_bytes);
qspi_status_t qspi_write(Qspi *p_qspi, uint16_t *us_data, uint32_t num_of_bytes);
void qspi_end_transfer(Qspi *p_qspi);
void qspi_set_clock_polarity(Qspi *p_qspi, uint32_t ul_polarity);
void qspi_set_clock_phase(Qspi *p_qspi, uint32_t ul_phase);
void qspi_set_baudrate(Qspi *p_qspi, uint8_t uc_baudrate_div);
void qspi_set_transfer_delay(Qspi *p_qspi, uint8_t uc_dlybs);
void qspi_set_instruction_addr(Qspi *p_qspi, uint32_t ul_addr);
void qspi_set_instruction_code(Qspi *p_qspi, qspi_mem_cmd_t ul_instruction_code);
void qspi_set_instruction_frame(Qspi *p_qspi, qspi_inst_frame_t ul_instruction_frame);
void qspi_set_scrambling_mode(Qspi *p_qspi, uint32_t ul_scr_enable, uint32_t ul_random_mode);
uint32_t qspi_set_scrambing_key(Qspi *p_qspi, uint32_t ul_key);
void qspi_set_writeprotect(Qspi *p_qspi, uint32_t ul_enable);
uint32_t qspi_get_writeprotect_status(Qspi *p_qspi);

/** Functionality API -- Serial Memory Mode */
qspi_status_t qspi_send_command(qspid_t *p_qspid, uint8_t const keep_cfg);
qspi_status_t qspi_send_command_with_data(qspid_t *p_qspid, uint8_t const keep_cfg);
qspi_status_t qspi_read_command(qspid_t *p_qspid, uint8_t const keep_cfg);
qspi_status_t qspi_enable_mem_access(qspid_t *p_qspid, uint8_t const keep_cfg, uint8_t scramble_flag);
qspi_status_t qspi_read_write_mem(qspid_t *p_qspid, access_t const read_write);

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
	   void spi_master_init(Spi *p_spi)
	   {
	       spi_enable_clock(p_spi);
	       spi_reset(p_spi);
	       spi_set_master_mode(p_spi);
	       spi_disable_mode_fault_detect(p_spi);
	       spi_disable_loopback(p_spi);
	       spi_set_peripheral_chip_select_value(p_spi,
	                                            spi_get_pcs(DEFAULT_CHIP_ID));
	       spi_set_fixed_peripheral_select(p_spi);
	       spi_disable_peripheral_select_decode(p_spi);
	       spi_set_delay_between_chip_select(p_spi, CONFIG_QSPI_MASTER_DELAY_BCS);
	   }
	   void spi_master_setup_device(Spi *p_spi, struct spi_device *device,
	       spi_flags_t flags, uint32_t baud_rate, board_spi_select_id_t sel_id)
	   {
	       spi_set_transfer_delay(p_spi, device->id, CONFIG_QSPI_MASTER_DELAY_BS,
	                              CONFIG_QSPI_MASTER_DELAY_BCT);

	       spi_set_bits_per_transfer(p_spi, device->id, CONFIG_QSPI_MASTER_BITS_PER_TRANSFER);
	       spi_set_baudrate_div(p_spi, device->id,
	                            spi_calc_baudrate_div(baud_rate, sysclk_get_cpu_hz()));

	       spi_configure_cs_behavior(p_spi, device->id, QSPI_CS_KEEP_LOW);

	       spi_set_clock_polarity(p_spi, device->id, flags >> 1);
	       spi_set_clock_phase(p_spi, device->id, ((flags & 0x1) ^ 0x1));
	   }
\endcode
 */
#endif /* QSPI_H_INCLUDED */
