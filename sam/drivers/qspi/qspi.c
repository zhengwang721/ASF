/**
 * \file
 *
 * \brief Quad Serial Peripheral Interface (QSPI) driver for SAMV71.
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

#include "qspi.h"
#include "sysclk.h"

/**
 * \defgroup sam_drivers_qspi_group Quad Serial Peripheral Interface (QSPI)
 *
 * See \ref sam_qspi_quickstart.
 *
 * The Quad SPI Interface (QSPI) is a synchronous serial data link that provides communication with external
 * devices in Master mode.
 * The QSPI can be used in SPI mode to interface to serial peripherals (such as ADCs, DACs, LCD controllers, CAN
 * controllers and sensors), or in Serial Memory mode to interface to serial Flash memories.
 * The QSPI allows the system to execute code directly from a serial Flash memory (XIP) without code shadowing to
 * RAM. The serial Flash memory mapping is seen in the system as other memories (ROM, SRAM, DRAM,
 * embedded Flash memory, etc.).
 * With the support of the Quad SPI protocol, the QSPI allows the system to use high-performance serial Flash
 * memories which are small and inexpensive, in place of larger and more expensive parallel Flash memories.
 *
 * @{
 */
#ifndef QSPI_WPMR_WPKEY_PASSWD
#define QSPI_WPMR_WPKEY_PASSWD QSPI_WPMR_WPKEY((uint32_t) 0x515350)
#endif

/**
 * \brief Read QSPI RDR register for SPI mode
 *
 * \param p_qspi   Pointer to an Qspi instance.
 */
__STATIC_INLINE uint16_t qspi_read_spi( Qspi *p_qspi )
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
__STATIC_INLINE void qspi_write_spi( Qspi *p_qspi, uint16_t w_data)
{
    assert(p_qspi);
    /* Send data */
	while(!(p_qspi->QSPI_SR & QSPI_SR_TXEMPTY));
    p_qspi->QSPI_TDR = w_data ;
	while(!(p_qspi->QSPI_SR & QSPI_SR_TDRE));
}

/**
 * \brief Reads the Instruction frame of QSPI
 *
 * \param pQspi   Pointer to an Qspi instance.
 */
__STATIC_INLINE uint32_t qspi_get_inst_frame( Qspi *p_qspi )
{
    assert(p_qspi);
    return p_qspi->QSPI_IFR;
}

/**
 * \brief Resets a QSPI peripheral.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 */
void qspi_reset( Qspi *p_qspi )
{
    assert(p_qspi);
    p_qspi->QSPI_CR = QSPI_CR_SWRST ;
}

/**
 * \brief Enables a QSPI peripheral.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 */
void qspi_enable( Qspi *p_qspi )
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
void qspi_disable( Qspi *p_qspi )
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
void qspi_set_run_mode(Qspi *p_qspi, uint32_t ul_mode)
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
void qspi_set_minimum_inactive_qcs_delay(Qspi *p_qspi, uint8_t uc_dlybct)
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
void qspi_set_delay_between_consecutive_transfers(Qspi *p_qspi, uint32_t uc_dlycs)
{
	assert(p_qspi);
	
	uint32_t ul_mask = p_qspi->QSPI_MR & (~QSPI_MR_DLYCS_Msk);
	p_qspi->QSPI_MR = ul_mask | QSPI_MR_DLYCS(uc_dlycs);
}

/**
 * \brief Qspi read datas.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param us_data Pointer to read datas.
 * \param num_of_bytes Read datas num.
 * \return status Read option result.
 */
qspi_status_t qspi_read(Qspi *p_qspi, uint16_t *us_data, uint32_t num_of_bytes)
{
	assert(p_qspi);
	if (num_of_bytes == 0) {
		return QSPI_SUCCESS;
	}
		
	qspi_status_t status = QSPI_UNKNOWN_ERROR;
	uint32_t num_of_bytes_read = 0;
	uint32_t num_of_attempt = 0;
	uint8_t *pw_data = (uint8_t *)us_data;
	uint16_t dummy = 0xFF;
	
	if (num_of_bytes == 1) {	
		for(; ;) {
			if (p_qspi->QSPI_SR & QSPI_SR_RDRF) {
				*us_data = qspi_read_spi(p_qspi);
				qspi_write_spi(p_qspi, dummy);
				*us_data = qspi_read_spi(p_qspi);
				num_of_attempt = 0;
				status = QSPI_SUCCESS;
				break;
			} else {
				if(num_of_attempt > 0xFFFF) {
					status = QSPI_READ_ERROR;
					puts(" SPI Read Error \n\r");
					break;
				} else {
					status = QSPI_READ_ERROR;
					num_of_attempt++;
				}
			}
		}
	} else {			
		/* dummy read  and write to discard  first bytes recvd and start receiving new data*/
		dummy = qspi_read_spi(p_qspi);
		qspi_write_spi(p_qspi, dummy);
		for(; num_of_bytes_read < num_of_bytes;) {
			if (p_qspi->QSPI_SR & QSPI_SR_TDRE) {
				*pw_data= qspi_read_spi(p_qspi);
				if(p_qspi->QSPI_MR & QSPI_MR_NBBITS_Msk) {
					pw_data += sizeof(uint16_t);
				} else {
					pw_data += sizeof(uint8_t);
				}
				num_of_bytes_read++;
				num_of_attempt = 0;
				status = QSPI_SUCCESS;
				qspi_write_spi(p_qspi, dummy);
			} else {
				if(num_of_attempt > 0xFFFF) {
					status = QSPI_READ_ERROR;
					puts(" SPI MultiRead Error \n\r");
					break;
				} else {
					status = QSPI_READ_ERROR;
					num_of_attempt++;
				}
			}
		}
	}

	return status;
}

/**
 * \brief Qspi write datas.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param us_data Pointer to datas to be written.
 * \param num_of_bytes Write datas num.
 * \return status Write option result.
 */
qspi_status_t qspi_write(Qspi *p_qspi, uint16_t *us_data, uint32_t num_of_bytes)
{
	assert(p_qspi);
	if (num_of_bytes == 0) {
		return QSPI_SUCCESS;
	}
	
	qspi_status_t status = QSPI_UNKNOWN_ERROR;
	uint32_t num_of_bytes_write = 0;
	uint32_t num_of_attempt = 0;
	uint8_t *pw_data = (uint8_t *)us_data;
	uint8_t Addr_Inc = 0;
	
	if (num_of_bytes == 1) {		
		for(;;) {
			if(p_qspi->QSPI_SR & QSPI_SR_TDRE) {
				qspi_write_spi(p_qspi, (uint16_t)(*us_data));
				num_of_attempt = 0;
				status = QSPI_SUCCESS;
				break;
			} else {
				status = QSPI_BUSY_SENDING;
				num_of_attempt++;
				if(num_of_attempt > 0xFFFF) {
					status = QSPI_WRITE_ERROR;
					puts(" SPI Write Error \n\r");
					break;
				}
			}
		}
	} else {		
		if(p_qspi->QSPI_MR & QSPI_MR_NBBITS_Msk) {
			Addr_Inc = sizeof(uint16_t);
		} else {
			Addr_Inc = sizeof(uint8_t);
		}
		
		for(; num_of_bytes_write < num_of_bytes; num_of_bytes_write++) {
			if (p_qspi->QSPI_SR & QSPI_SR_TXEMPTY) {
				qspi_write_spi(p_qspi, (uint16_t )(*pw_data));
				pw_data += Addr_Inc;
				num_of_attempt = 0;
				status = QSPI_SUCCESS;
			} else {
				status = QSPI_BUSY_SENDING;
				num_of_attempt++;
				if(num_of_attempt > 0xFFFF) {
					status = QSPI_WRITE_ERROR;
					puts(" SPI Multi Write Error \n\r");
					break;
				}
			}
		}
	}
	
	return status;
}

/**
 * \brief Ends ongoing transfer by releasing CS of QSPI peripheral.
 *
 * \param pQspi  Pointer to an Qspi instance.
 */
void qspi_end_transfer(Qspi *p_qspi)
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
void qspi_set_clock_polarity(Qspi *p_qspi, uint32_t ul_polarity)
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
void qspi_set_clock_phase(Qspi *p_qspi, uint32_t ul_phase)
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
void qspi_set_baudrate(Qspi *p_qspi, uint8_t uc_baudrate_div)
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
void qspi_set_transfer_delay(Qspi *p_qspi, uint8_t uc_dlybs)
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
void qspi_set_instruction_addr(Qspi *p_qspi, uint32_t ul_addr)
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
void qspi_set_instruction_code(Qspi *p_qspi, qspi_mem_cmd_t ul_instruction_code)
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
void qspi_set_instruction_frame(Qspi *p_qspi, qspi_inst_frame_t ul_instruction_frame)
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
 * \brief Set qspi scrambling mode.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \param ul_mode Mode to be set.
 */
void qspi_set_scrambling_mode(Qspi *p_qspi, uint32_t ul_scr_enable, uint32_t ul_random_mode)
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
uint32_t qspi_set_scrambing_key(Qspi *p_qspi, uint32_t ul_key)
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
void qspi_set_writeprotect(Qspi *p_qspi, uint32_t ul_enable)
{
	assert(p_qspi);
	if (ul_enable) {
		p_qspi->QSPI_WPMR |= (QSPI_WPMR_WPKEY_PASSWD | QSPI_WPMR_WPEN);
	} else {
		p_qspi->QSPI_WPMR &= (~(QSPI_WPMR_WPKEY_PASSWD | QSPI_WPMR_WPEN));
	}
}

/**
 * \brief Get write protection status.
 *
 * \param p_qspi  Pointer to a Qspi instance.
 * \return Write protection status.
 */
uint32_t qspi_get_writeprotect_status(Qspi *p_qspi)
{
	assert(p_qspi);
	return p_qspi->QSPI_WPSR;
}

/**
 * \brief Send an instruction over QSPI (oly a flash command no data)
 *
 * \param pQspi     Pointer to an Qspi instance.
 * \param keep_cfg   To keep Instruction fram value or restes to zero
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
qspi_status_t qspi_send_command(qspid_t *p_qspid, uint8_t const keep_cfg)
{  
    qspi_inst_frame_t* const p_frame = p_qspid->p_qspi_frame;
    qspi_mem_cmd_t p_command = p_qspid->qspi_command;
    qspi_status_t Status = QSPI_UNKNOWN_ERROR;
    
    if(p_frame->inst_frame.bm.b_addr_en) {
		qspi_set_instruction_addr(p_qspid->p_qspi_hw, p_frame->addr);
    }
    qspi_set_instruction_code(p_qspid->p_qspi_hw, p_command);
    qspi_set_instruction_frame(p_qspid->p_qspi_hw, *p_frame);
    __DSB();
	__ISB();
	qspi_end_transfer(p_qspid->p_qspi_hw);
    while(!(p_qspid->p_qspi_hw->QSPI_SR & QSPI_SR_INSTRE));             // poll CR reg to know status if instruction has end
    if(!keep_cfg) {
		p_frame->inst_frame.val = 0;
    }
    return Status;
}

/**
 * \brief Send instruction over QSPI with data
 *
 * \param pQspi     Pointer to an Qspi instance.
 * \param keep_cfg   To keep Instruction fram value or restes to zero
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
qspi_status_t qspi_send_command_with_data(qspid_t *p_qspid, uint8_t const keep_cfg)
{  
    qspi_inst_frame_t* const  p_frame = p_qspid->p_qspi_frame;
    qspi_mem_cmd_t p_command = p_qspid->qspi_command;
    qspi_buffer_t p_buffer = p_qspid->qspi_buffer;
    uint32_t *p_qspi_buffer = (uint32_t *)QSPIMEM_ADDR;
    qspi_status_t Status = QSPI_UNKNOWN_ERROR;
   
    assert(p_buffer.p_data_tx);
        
    qspi_set_instruction_code(p_qspid->p_qspi_hw, p_command);
    qspi_set_instruction_frame(p_qspid->p_qspi_hw, *p_frame );
    
    qspi_get_inst_frame(p_qspid->p_qspi_hw);                        // to synchronize system bus accesses
    if(!keep_cfg)
    {
      p_frame->inst_frame.val = 0;
    }
    
    memcpy(p_qspi_buffer, p_buffer.p_data_tx, p_buffer.tx_data_size); 
	__DSB();
	__ISB();
    qspi_end_transfer(p_qspid->p_qspi_hw) ;                         // End transmission after all data has been sent
    while(!(p_qspid->p_qspi_hw->QSPI_SR & QSPI_SR_INSTRE));         // poll CR reg to know status if instruction has end
    
    return Status;
}

/**
 * \brief Send instruction over QSPI to read data
 *
 * \param pQspi     Pointer to an Qspi instance.
 * \param keep_cfg   To keep Instruction fram value or restes to zero
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
qspi_status_t qspi_read_command(qspid_t *p_qspid, uint8_t const keep_cfg)
{  
    qspi_inst_frame_t* const p_frame = p_qspid->p_qspi_frame;
    qspi_mem_cmd_t  p_command = p_qspid->qspi_command;
    qspi_buffer_t *p_buffer = &p_qspid->qspi_buffer;
    uint32_t *p_qspi_buffer = (uint32_t *)QSPIMEM_ADDR;
    qspi_status_t status = QSPI_UNKNOWN_ERROR;
    
    assert(p_buffer->rx_data_size);
      
    qspi_set_instruction_code(p_qspid->p_qspi_hw, p_command);
    qspi_set_instruction_frame(p_qspid->p_qspi_hw, *p_frame);
    
    qspi_get_inst_frame(p_qspid->p_qspi_hw);                        // to synchronize system bus accesses   
    if(!keep_cfg) {
		p_frame->inst_frame.val = 0;
    }
    memcpy(p_buffer->p_data_rx , p_qspi_buffer,  p_buffer->rx_data_size);
	__DSB();
	__ISB();
    qspi_end_transfer(p_qspid->p_qspi_hw);                        // End transmission after all data has been sent
    while(!(p_qspid->p_qspi_hw->QSPI_SR & QSPI_SR_INSTRE));       // poll CR reg to know status if instruction has end
	    
    return status;
}

/**
 * \brief Sends an instruction over QSPI and configures other related address like Addr , Frame and synchronise bus access before data read or write
 *
 * \param pQspi         Pointer to an Qspi instance.
 * \param keep_cfg       To keep Instruction fram value or restes to zero
 * \param ScrambleFlag  Enable or disable scramble on QSPI
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
qspi_status_t qspi_enable_mem_access(qspid_t *p_qspid, uint8_t const keep_cfg, uint8_t scramble_flag)
{  
    qspi_inst_frame_t* const p_frame = p_qspid->p_qspi_frame;
    qspi_mem_cmd_t p_command = p_qspid->qspi_command;
        
    qspi_status_t Status = QSPI_UNKNOWN_ERROR;
     
    qspi_set_instruction_code(p_qspid->p_qspi_hw, p_command);
    
    if(scramble_flag) {
		qspi_set_scrambling_mode(p_qspid->p_qspi_hw, scramble_flag, 1);
    }
    
    qspi_set_instruction_frame(p_qspid->p_qspi_hw, *p_frame );
    
    qspi_get_inst_frame(p_qspid->p_qspi_hw);                        // to synchronize system bus accesses   
    if(!keep_cfg) {
		p_frame->inst_frame.val = 0;
    } 
    Status = QSPI_SUCCESS;
    return Status;
}

/**
 * \brief Writes or reads the QSPI memory (0x80000000) to trasmit or receive data from Flash memory
 * \param pQspi         Pointer to an Qspi instance.
 * \param ReadWrite     Flag to indicate read/write QSPI memory access
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
qspi_status_t qspi_read_write_mem(qspid_t *p_qspid, access_t const read_write)
{  
    qspi_status_t Status = QSPI_UNKNOWN_ERROR;
    qspi_inst_frame_t* const p_frame = p_qspid->p_qspi_frame;
    uint32_t *p_qspi_mem = (uint32_t *)(QSPIMEM_ADDR | p_frame->addr);
    qspi_buffer_t *p_buffer = &p_qspid->qspi_buffer;
    
    assert( ( (read_write > CmdAccess) && (read_write <= WriteAccess) ) ? true: false );
    if (read_write == WriteAccess) {
		memcpy(p_qspi_mem, p_buffer->p_data_tx , p_buffer->tx_data_size ); 
    } else {
		memcpy(p_buffer->p_data_rx, p_qspi_mem, p_buffer->rx_data_size ); 
    }
	__DSB();
	__ISB();
    qspi_end_transfer(p_qspid->p_qspi_hw) ;                   // End transmission after all data has been sent
    while(!(p_qspid->p_qspi_hw->QSPI_SR & QSPI_SR_INSTRE));             // poll CR reg to know status if instruction has end
    
    Status = QSPI_SUCCESS;
    return Status;
}

/**
 * @}
 */
