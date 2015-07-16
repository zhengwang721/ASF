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


/**
 * \brief Qspi read datas.
 *
 * \param qspi  Pointer to a Qspi instance.
 * \param us_data Pointer to read datas.
 * \param num_of_bytes Read datas num.
 * \return status Read option result.
 */
enum status_code qspi_read(Qspi *qspi, uint16_t *us_data, uint32_t num_of_bytes)
{
	assert(qspi);
	if (num_of_bytes == 0) {
		return STATUS_OK;
	}

	enum status_code status = OPERATION_IN_PROGRESS;
	uint32_t num_of_bytes_read = 0;
	uint32_t num_of_attempt = 0;
	uint8_t *pw_data = (uint8_t *)us_data;
	uint16_t dummy = 0xFF;

	if (num_of_bytes == 1) {
		for(; ;) {
			if (qspi->QSPI_SR & QSPI_SR_RDRF) {
				*us_data = qspi_read_spi(qspi);
				qspi_write_spi(qspi, dummy);
				*us_data = qspi_read_spi(qspi);
				num_of_attempt = 0;
				status = STATUS_OK;
				break;
			} else {
				if(num_of_attempt > 0xFFFF) {
					status = ERR_TIMEOUT;
					puts(" SPI Read Error \n\r");
					break;
				} else {
					status = STATUS_ERR_BUSY;
					num_of_attempt++;
				}
			}
		}
	} else {
		/* dummy read  and write to discard  first bytes recvd and start receiving new data*/
		dummy = qspi_read_spi(qspi);
		qspi_write_spi(qspi, dummy);
		for(; num_of_bytes_read < num_of_bytes;) {
			if (qspi->QSPI_SR & QSPI_SR_TDRE) {
				*pw_data= qspi_read_spi(qspi);
				if(qspi->QSPI_MR & QSPI_MR_NBBITS_Msk) {
					pw_data += sizeof(uint16_t);
				} else {
					pw_data += sizeof(uint8_t);
				}
				num_of_bytes_read++;
				num_of_attempt = 0;
				status = STATUS_OK;
				qspi_write_spi(qspi, dummy);
			} else {
				if(num_of_attempt > 0xFFFF) {
					status = ERR_TIMEOUT;
					puts(" SPI MultiRead Error \n\r");
					break;
				} else {
					status = STATUS_ERR_BUSY;
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
 * \param qspi  Pointer to a Qspi instance.
 * \param us_data Pointer to datas to be written.
 * \param num_of_bytes Write datas num.
 * \return status Write option result.
 */
enum status_code qspi_write(Qspi *qspi, uint16_t *us_data, uint32_t num_of_bytes)
{
	assert(qspi);
	if (num_of_bytes == 0) {
		return STATUS_OK;
	}

	enum status_code status = OPERATION_IN_PROGRESS;
	uint32_t num_of_bytes_write = 0;
	uint32_t num_of_attempt = 0;
	uint8_t *pw_data = (uint8_t *)us_data;
	uint8_t Addr_Inc = 0;

	if (num_of_bytes == 1) {
		for(;;) {
			if(qspi->QSPI_SR & QSPI_SR_TDRE) {
				qspi_write_spi(qspi, (uint16_t)(*us_data));
				num_of_attempt = 0;
				status = STATUS_OK;
				break;
			} else {
				status = STATUS_ERR_BUSY;
				num_of_attempt++;
				if(num_of_attempt > 0xFFFF) {
					status = ERR_TIMEOUT;
					puts(" SPI Write Error \n\r");
					break;
				}
			}
		}
	} else {
		if(qspi->QSPI_MR & QSPI_MR_NBBITS_Msk) {
			Addr_Inc = sizeof(uint16_t);
		} else {
			Addr_Inc = sizeof(uint8_t);
		}

		for(; num_of_bytes_write < num_of_bytes; num_of_bytes_write++) {
			if (qspi->QSPI_SR & QSPI_SR_TXEMPTY) {
				qspi_write_spi(qspi, (uint16_t )(*pw_data));
				pw_data += Addr_Inc;
				num_of_attempt = 0;
				status = STATUS_OK;
			} else {
				status = STATUS_ERR_BUSY;
				num_of_attempt++;
				if(num_of_attempt > 0xFFFF) {
					status = ERR_TIMEOUT;
					puts(" SPI Multi Write Error \n\r");
					break;
				}
			}
		}
	}

	return status;
}

/**
 * \brief Config qspi according the config struct
 *
 * \param qspi_config     Pointer to an qspi_config_t struct.
 *
 */
void qspi_set_config(Qspi *qspi, qspi_config_t *qspi_config)
{
	if(qspi_config->serial_memory) {
		qspi_set_memory_mode(qspi);
	} else {
		qspi_set_spi_mode(qspi);
	}
	if(qspi_config->loopback_en) {
		qspi_enable_loopback(qspi);
	}
	if(qspi_config->wait_data_for_transfer) {
		qspi_wait_data_read_before_transfer(qspi);
	}
	qspi_set_chip_select_mode(qspi, qspi_config->csmode);
	qspi_set_bits_per_transfer(qspi, qspi_config->bits_per_transfer);
	qspi_set_minimum_inactive_qcs_delay(qspi, qspi_config->min_delay_qcs);
	qspi_set_delay_between_consecutive_transfers(qspi, qspi_config->delay_between_ct);
	qspi_set_clock_polarity(qspi, qspi_config->clock_polarity);
	qspi_set_clock_phase(qspi, qspi_config->clock_phase);
	qspi_set_baudrate(qspi, qspi_config->baudrate);
	qspi_set_transfer_delay(qspi, qspi_config->transfer_delay);
	qspi_set_scrambling_mode(qspi, qspi_config->scrambling_en, qspi_config->scrambling_random_value_dis);
	qspi_set_scrambing_key(qspi, qspi_config->scrambling_user_key);
}

/**
 * \brief Config qspi according the config struct
 *
 * \param qspi_config     Pointer to an qspi_config_t struct.
 *
 */
void qspi_get_default_config(qspi_config_t * qspi_config)
{
	qspi_config->serial_memory = 1;
	qspi_config->csmode = 1;
	qspi_config->clock_polarity = 0;
	qspi_config->clock_phase = 0;
	qspi_config->bits_per_transfer = QSPI_MR_NBBITS_8_BIT;
	qspi_config->baudrate = sysclk_get_cpu_hz() / 1000000;
	qspi_config->transfer_delay = 0x40;
}

/**
 * \brief Send an instruction over QSPI (only a flash command no data)
 *
 * \param pQspi     Pointer to an Qspi instance.
 * \param keep_cfg   To keep Instruction frame value or reset to zero
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
enum status_code qspi_flash_send_command(qspid_t *qspid, uint8_t const keep_cfg)
{
	qspi_inst_frame_t* const frame = qspid->qspi_frame;
	qspi_mem_cmd_t command = qspid->qspi_command;
	enum status_code status = OPERATION_IN_PROGRESS;

	if(frame->inst_frame.bm.b_addr_en) {
		qspi_set_instruction_addr(qspid->qspi_hw, frame->addr);
	}
	qspi_set_instruction_code(qspid->qspi_hw, command);
	qspi_set_instruction_frame(qspid->qspi_hw, *frame);
	__DSB();
	__ISB();
	qspi_end_transfer(qspid->qspi_hw);
	/** poll CR reg to know status if instruction has end */
	while(!(qspid->qspi_hw->QSPI_SR & QSPI_SR_INSTRE));
	if(!keep_cfg) {
		frame->inst_frame.val = 0;
	}
	status = STATUS_OK;
	return status;
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
enum status_code qspi_flash_send_command_with_data(qspid_t *qspid, uint8_t const keep_cfg)
{
	qspi_inst_frame_t* const  frame = qspid->qspi_frame;
	qspi_mem_cmd_t command = qspid->qspi_command;
	qspi_buffer_t buffer = qspid->qspi_buffer;
	uint32_t *qspi_buffer = (uint32_t *)QSPIMEM_ADDR;
	enum status_code status = OPERATION_IN_PROGRESS;

	assert(buffer.data_tx);

	qspi_set_instruction_code(qspid->qspi_hw, command);
	qspi_set_instruction_frame(qspid->qspi_hw, *frame );
	/** to synchronize system bus accesses */
	qspi_get_inst_frame(qspid->qspi_hw);
	if(!keep_cfg) {
		frame->inst_frame.val = 0;
	}

	memcpy(qspi_buffer, buffer.data_tx, buffer.tx_data_size);
	__DSB();
	__ISB();
	/** End transmission after all data has been sent */
	qspi_end_transfer(qspid->qspi_hw) ;
	/** poll CR reg to know status if instruction has end */
	while(!(qspid->qspi_hw->QSPI_SR & QSPI_SR_INSTRE));

	status = STATUS_OK;
	return status;
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
enum status_code qspi_flash_read_command(qspid_t *qspid, uint8_t const keep_cfg)
{
	qspi_inst_frame_t* const frame = qspid->qspi_frame;
	qspi_mem_cmd_t  command = qspid->qspi_command;
	qspi_buffer_t *buffer = &qspid->qspi_buffer;
	uint32_t *qspi_buffer = (uint32_t *)QSPIMEM_ADDR;
	enum status_code status = OPERATION_IN_PROGRESS;

	assert(buffer->rx_data_size);

	qspi_set_instruction_code(qspid->qspi_hw, command);
	qspi_set_instruction_frame(qspid->qspi_hw, *frame);

	/** to synchronize system bus accesses */
	qspi_get_inst_frame(qspid->qspi_hw);
	if(!keep_cfg) {
		frame->inst_frame.val = 0;
	}
	memcpy(buffer->data_rx , qspi_buffer,  buffer->rx_data_size);
	__DSB();
	__ISB();
	/** End transmission after all data has been sent */
	qspi_end_transfer(qspid->qspi_hw);
	/** poll CR reg to know status if instruction has end */
	while(!(qspid->qspi_hw->QSPI_SR & QSPI_SR_INSTRE));

	status = STATUS_OK;
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
enum status_code qspi_flash_enable_mem_access(qspid_t *qspid, uint8_t const keep_cfg, uint8_t scramble_flag)
{
	qspi_inst_frame_t* const frame = qspid->qspi_frame;
	qspi_mem_cmd_t command = qspid->qspi_command;

	enum status_code status = OPERATION_IN_PROGRESS;

	qspi_set_instruction_code(qspid->qspi_hw, command);

	if(scramble_flag) {
		qspi_set_scrambling_mode(qspid->qspi_hw, scramble_flag, 1);
	}

	qspi_set_instruction_frame(qspid->qspi_hw, *frame );

	/** to synchronize system bus accesses */
	qspi_get_inst_frame(qspid->qspi_hw);
	if(!keep_cfg) {
		frame->inst_frame.val = 0;
	}
	status = STATUS_OK;
	return status;
}

/**
 * \brief Writes or reads the QSPI memory (0x80000000) to trasmit or receive data from Flash memory
 * \param pQspi         Pointer to an Qspi instance.
 * \param ReadWrite     Flag to indicate read/write QSPI memory access
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
enum status_code qspi_flash_read_write_mem(qspid_t *qspid, qspi_access_t const read_write)
{
	enum status_code status = OPERATION_IN_PROGRESS;
	qspi_inst_frame_t* const frame = qspid->qspi_frame;
	uint32_t *qspi_mem = (uint32_t *)(QSPIMEM_ADDR | frame->addr);
	qspi_buffer_t *buffer = &qspid->qspi_buffer;

	assert( ( (read_write > cmd_access) && (read_write <= write_access) ) ? true: false );
	if (read_write == write_access) {
		memcpy(qspi_mem, buffer->data_tx , buffer->tx_data_size );
	} else {
		memcpy(buffer->data_rx, qspi_mem, buffer->rx_data_size );
	}
	__DSB();
	__ISB();
	/** End transmission after all data has been sent */
	qspi_end_transfer(qspid->qspi_hw) ;
	/** poll CR reg to know status if instruction has end */
	while(!(qspid->qspi_hw->QSPI_SR & QSPI_SR_INSTRE));

	status = STATUS_OK;
	return status;
}

/**
 * @}
 */
