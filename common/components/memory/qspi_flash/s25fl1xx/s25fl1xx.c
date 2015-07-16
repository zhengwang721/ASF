/**
 * \file
 *
 * \brief QSPI flash memory driver for S25FL116K.
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

/**
 * \addtogroup at25d_module S25FL1 driver
 * \ingroup lib_spiflash
 * The S25FL1 serial dataflash driver is based on the corresponding S25FL1 SPI driver.
 * A S25FL1 instance has to be initialized using the Dataflash levle function
 * S25FL1D_Configure(). S25FL1 Dataflash can be automatically detected using
 * the S25FL1D_FindDevice() function. Then S25FL1 dataflash operations such as
 * read, write and erase DF can be launched using s25fl1xx_send_command function
 * with corresponding S25FL1 command set.
 *
 * \section Usage
 * <ul>
 * <li> Reads a serial flash device ID using S25FL1D_ReadJedecId().</li>
 * <li> Reads data from the S25fl1 at the specified address using S25FL1D_Read().</li>
 * <li> Writes data on the S25fl1 at the specified address using S25FL1D_Write().</li>
 * <li> Erases all chip using S25FL1D_EraseBlock().</li>
 * <li> Erases a specified block using S25FL1D_EraseBlock().</li>
 * <li> Poll until the S25fl1 has completed of corresponding operations using
 * s25fl1xx_is_busy(qspid).</li>
 * <li> Retrieves and returns the S25fl1 current using s25fl1xx_read_status(qspid).</li>
 * </ul>
 *
 */
/*@{*/
/*@}*/

/**
 * \file
 *
 * Implementation for the S25FL1 Serialflash driver.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include <board.h>
#include <assert.h>
#include "stdlib.h"
#include "string.h"
#include "s25fl1xx.h"
#include "delay.h"

/*----------------------------------------------------------------------------
 *        Variable
 *----------------------------------------------------------------------------*/

struct qspi_inst_frame_t *dev;
struct qspi_inst_frame_t *mem;

/*----------------------------------------------------------------------------
 *        Definition
 *----------------------------------------------------------------------------*/
#define READ_DEV        0
#define WRITE_DEV       1

#define PAGE_SIZE       256

/**
 * \brief Initialize QSPI serial memory mode.
 * \param qspi                      Pointer to an S25FL1 qspid_t struct.
 * \param mode_config               Configure settings to config qspid.
 * \param use_default_config Config QSPI use default configures.
 */
void s25fl1xx_initialize(Qspi *qspi, struct qspi_config_t *mode_config, uint32_t use_default_config)
{
	puts("-I- Initialize S25FL1xx.\r");

	dev = (struct qspi_inst_frame_t *)malloc (sizeof(struct qspi_inst_frame_t));
	memset(dev, 0, sizeof(struct qspi_inst_frame_t));
	dev->inst_frame.bm.b_width = QSPI_IFR_WIDTH_SINGLE_BIT_SPI;

	mem = (struct qspi_inst_frame_t *)malloc (sizeof(struct qspi_inst_frame_t));
	memset(mem, 0, sizeof(struct qspi_inst_frame_t));
	mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_SINGLE_BIT_SPI;

	if (use_default_config) {
		qspi_get_default_config(mode_config);
	}

	qspi_initialize(qspi, mode_config);
}

/**
 * \brief Send command to the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param instr  Instruction to be execute.
 * \param tx_data  Data buffer to send data.
 * \param rx_data  Data buffer to receive data.
 * \param read_write  Read/write access.
 * \param size  Data size to be read/write.
 */
static void s25fl1xx_exec_command(struct qspid_t *qspid, uint8_t instr, uint32_t *tx_data, uint32_t *rx_data, enum qspi_access read_write, uint32_t size)
{
	qspid->qspi_command.instruction = instr;
	dev->inst_frame.bm.b_inst_en = 1;
	qspid->qspi_frame = dev;
	qspid->qspi_buffer.data_tx = tx_data;
	qspid->qspi_buffer.data_rx = rx_data;

	/** to prevent unaligned access */
	if( (size % sizeof(uint32_t)) && size > 1) {
		size += (sizeof(uint32_t) - (size % sizeof(uint32_t)));
	}

	if(read_write == QSPI_CMD_ACCESS) {
		dev->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ >> QSPI_IFR_TFRTYP_Pos);
		dev->inst_frame.bm.b_data_en = 0;
	} else if (read_write == QSPI_WRITE_ACCESS) {
		dev->inst_frame.bm.b_data_en = 1;
		dev->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_WRITE >> QSPI_IFR_TFRTYP_Pos);
		qspid->qspi_buffer.tx_data_size = size;
	} else {
		dev->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ >> QSPI_IFR_TFRTYP_Pos);
		dev->inst_frame.bm.b_data_en = 1;
		qspid->qspi_buffer.rx_data_size = size;
	}
	qspi_flash_exec_command(qspid, read_write);
}

/**
 * \brief Read/write data from serial flash memory.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct..
 * \param instr Instruction to be execute.
 * \param addr  Address to send frame.
 * \param tx_data  Data buffer to send data.
 * \param rx_data  Data buffer to receive data.
 * \param read_write  Read/write access.
 * \param size  Data size to be read/write.
 * \param secure  Enable or disable scramble on QSPI.
 */
static void s25fl1xx_memory_access(struct qspid_t *qspid, uint8_t instr, uint32_t addr, uint32_t *tx_data, uint32_t *rx_data, enum qspi_access read_write, uint32_t size, uint8_t secure)
{
	qspid->qspi_command.instruction = instr;
	qspid->qspi_buffer.data_tx = tx_data;
	qspid->qspi_buffer.data_rx = rx_data;
	mem->addr= addr;
	mem->inst_frame.bm.b_inst_en = 1;
	mem->inst_frame.bm.b_data_en = 1;
	mem->inst_frame.bm.b_addr_en = 1;
	qspid->qspi_frame = mem;

	if (read_write == QSPI_WRITE_ACCESS) {
		mem->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_WRITE_MEMORY >> QSPI_IFR_TFRTYP_Pos);
		qspid->qspi_buffer.tx_data_size = size;
	} else {
		mem->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ_MEMORY >> QSPI_IFR_TFRTYP_Pos);
		qspid->qspi_buffer.rx_data_size = size;
	}
	qspi_flash_memory_access(qspid, read_write, secure);
}

/**
 * \brief Reads and returns the status register of the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
static uint8_t s25fl1xx_read_status1(struct qspid_t *qspid)
{
	uint8_t status;
	s25fl1xx_exec_command(qspid, READ_STATUS_1, 0, (uint32_t *)(&status), QSPI_READ_ACCESS, 1);
	return status;
}

/**
 * \brief Reads and returns the status register of the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
static uint8_t s25fl1xx_read_status2(struct qspid_t *qspid)
{
	uint8_t status;
	s25fl1xx_exec_command(qspid, READ_STATUS_2, 0, (uint32_t *)&status, QSPI_READ_ACCESS, 1);
	return status;
}

/**
 * \brief Reads and returns the status register of the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
static uint8_t s25fl1xx_read_status3(struct qspid_t *qspid)
{
	uint8_t status;
	s25fl1xx_exec_command(qspid, READ_STATUS_3, 0, (uint32_t *)&status, QSPI_READ_ACCESS, 1);
	return status;
}

/**
 * \brief Reads and returns the status register of the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
static uint32_t s25fl1xx_read_status(struct qspid_t *qspid)
{
	uint32_t status;
	status = s25fl1xx_read_status1(qspid) | (s25fl1xx_read_status2(qspid) << 8) | (s25fl1xx_read_status3(qspid) << 16);
	return status;
}

/**
 * \brief Wait for transfer to finish calling the SPI driver ISR. (interrupts are disabled)
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
static void s25fl1xx_is_busy(struct qspid_t *qspid)
{
	while(s25fl1xx_read_status1(qspid) & STATUS_RDYBSY);
}

/**
 * \brief Enable write option to serial flash memory.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
static void s25fl1xx_enable_write(struct qspid_t *qspid)
{
	uint8_t status = 0;
	while(!(status & STATUS_WEL)) {
		s25fl1xx_exec_command(qspid, WRITE_ENABLE, 0, 0, QSPI_CMD_ACCESS, 0);
		status = s25fl1xx_read_status1(qspid);
	}
}

/**
 * \brief Disable write option to serial flash memory.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
static void s25fl1xx_disable_write(struct qspid_t *qspid)
{
	uint8_t status;
	status = s25fl1xx_read_status1(qspid);
	while( (status & STATUS_WEL) != 0) {
		s25fl1xx_exec_command(qspid, WRITE_DISABLE, 0, 0, QSPI_CMD_ACCESS, 0);
		status = s25fl1xx_read_status1(qspid);
	}
}

/**
 * \brief Writes the given value in the status register of the serial flash device.
 *
 * \param qspid   Pointer to an S25FL1 qspid_t struct.
 * \param status  Status to write.
 */
static void s25fl1xx_write_status(struct qspid_t *qspid, uint8_t *status)
{
	s25fl1xx_enable_write(qspid);
	s25fl1xx_exec_command(qspid, WRITE_STATUS, (uint32_t *)status, 0, QSPI_WRITE_ACCESS, 3);
	s25fl1xx_disable_write(qspid);
}

/**
 * \brief Writes the given value in the status register of the serial flash device.
 *
 * \param qspid   Pointer to an S25FL1 qspid_t struct.
 * \param status  Status to write.
 */
static void s25fl1xx_write_volatile_status(struct qspid_t *qspid, uint8_t *status)
{
	s25fl1xx_exec_command(qspid, 0x50, 0, 0 , QSPI_CMD_ACCESS, 0);
	s25fl1xx_exec_command(qspid, WRITE_STATUS, (uint32_t *)status, 0 , QSPI_WRITE_ACCESS, 3);
	s25fl1xx_disable_write(qspid);
}

/**
 * \brief Check weather the address of addr is protected.
 *
 * \param qspid Pointer to an S25FL1 qspid_t struct.
 * \param addr  Address to be checked.
 */
static uint8_t s25fl1xx_check_protected_addr(uint8_t status1, uint32_t addr)
{
  const uint32_t addr_ump = (status1 & SEC_PROTECT_Msk) ? 0x001000UL : 0x010000UL;
  static uint8_t is_protected = 0;

  const uint8_t block_bits = ((status1 & BLOCK_PROTECT_Msk) >> 2);

  switch(block_bits) {
	case 1:
	if (status1 & TOP_BTM_PROTECT_Msk) {
		if((addr > 0x000000) && (addr < (0x000000 + addr_ump - 1))) {
			is_protected = 1;
		}
	} else {
		if((addr > (0x1FFFFF - addr_ump + 1)) && (addr < 0x1FFFFF)) {
			is_protected = 1;
		}
	}
	break;

  case 2:
	if (status1 & TOP_BTM_PROTECT_Msk) {
		if((addr > 0x000000) && (addr < (0x000000 + (2* addr_ump)- 1))) {
			is_protected = 1;
		}
	} else {
		if((addr > (0x1FFFFF - (2*addr_ump ) + 1)) && (addr < 0x1FFFFF)) {
			is_protected = 1;
		}
	}
	break;

  case 3:
	if (status1 & TOP_BTM_PROTECT_Msk) {
		if((addr > 0x000000) && (addr < (0x000000 + (4 * addr_ump) - 1))) {
			is_protected = 1;
		}
	} else {
		if( (addr > (0x1FFFFF - (4*addr_ump) + 1)) && (addr < 0x1FFFFF)) {
			is_protected = 1;
		}
	}
	break;

  case 4:
	if (status1 & TOP_BTM_PROTECT_Msk) {
		if((addr > 0x000000) && (addr < (0x000000 + (8 * addr_ump) - 1))) {
			is_protected = 1;
		}
	} else {
		if((addr > (0x1FFFFF - (8*addr_ump ) + 1)) && (addr < 0x1FFFFF)) {
			is_protected = 1;
		}
	}
	break;

  case 5:
	if(!(status1 & SEC_PROTECT_Msk)) {
		if (status1 & TOP_BTM_PROTECT_Msk) {
			if((addr > 0x000000) && (addr < (0x000000 + (16 * addr_ump) - 1))) {
				is_protected = 1;
			}
		} else {
			if((addr > (0x1FFFFF - (16*addr_ump) + 1)) && (addr < 0x1FFFFF)) {
				is_protected = 1;
			}
		}
	}
	break;

  case 6:
	if(!(status1 & SEC_PROTECT_Msk)) {
		if (status1 & TOP_BTM_PROTECT_Msk) {
			if((addr > 0x000000) && (addr < (0x000000 + (32 * addr_ump) - 1))) {
				is_protected = 1;
			}
		}
	}
	break;
  }

  return is_protected;
}

/**
 * \brief Reads and returns the serial flash device ID.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
uint32_t s25fl1xx_read_jedec_id(struct qspid_t *qspid)
{
	static uint32_t id;
	s25fl1xx_exec_command(qspid, READ_JEDEC_ID, 0, &id, QSPI_READ_ACCESS, 3);
	return id;
}

/**
 * \brief Enables critical writes operation on a serial flash device, such as sector
 * protection, status register, etc.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param mode  Mode to be set.
 */
void s25fl1xx_set_quad_mode(struct qspid_t *qspid, uint8_t mode)
{
	uint8_t status[3];

	status[0] = s25fl1xx_read_status1(qspid);
	status[1] = s25fl1xx_read_status2(qspid);
	status[2] = s25fl1xx_read_status3(qspid);

	if(mode) {
		while(!(status[1] & STATUS_QUAD_ENABLE)) {
			status[1] |= STATUS_QUAD_ENABLE;
			s25fl1xx_write_status(qspid, status);
			status[1] = s25fl1xx_read_status2(qspid);
			delay_ms(50);
		}
	} else {
		while((status[1] & STATUS_QUAD_ENABLE)) {
			status[1] &= (~STATUS_QUAD_ENABLE)  ;
			s25fl1xx_write_status(qspid, status);
			status[1] = s25fl1xx_read_status2(qspid);
			delay_ms(50);
		}
	}
}

/**
 * \brief Enables critical writes operation on a serial flash device, such as sector
 * protection, status register, etc.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param byte_align  Burst wrap length to be set.
 */
void s25fl1xx_enable_wrap(struct qspid_t *qspid, uint8_t byte_align)
{
	uint8_t status[3];

	status[0] = s25fl1xx_read_status1(qspid);
	status[1] = s25fl1xx_read_status2(qspid);
	status[2] = s25fl1xx_read_status3(qspid);

	status[2] |= (byte_align << 5);

	dev->inst_frame.bm.b_dummy_cycles = 24;
	s25fl1xx_exec_command(qspid, WRAP_ENABLE,(uint32_t *)&status[2], 0,  QSPI_WRITE_ACCESS, 1);

	s25fl1xx_write_volatile_status(qspid, status);
	status[2] = s25fl1xx_read_status3(qspid);
	delay_ms(50);
}

/**
 * \brief Enables critical writes operation on a serial flash device, such as sector
 * protection, status register, etc.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param latency Latency control.
 */
void s25fl1xx_set_read_latency_control(struct qspid_t *qspid, uint8_t latency)
{
	uint8_t status[3];

	status[0] = s25fl1xx_read_status1(qspid);
	status[1] = s25fl1xx_read_status2(qspid);
	status[2] = s25fl1xx_read_status3(qspid);

	status[2] |= latency;

	qspid->qspi_buffer.data_tx = (uint32_t *)&status[2];
	while((status[2] & STATUS_LATENCY_CTRL) != latency) {
		s25fl1xx_write_volatile_status(qspid, status);
		status[2] = s25fl1xx_read_status3(qspid);
		delay_ms(50);
	}
}

/**
 * \brief s25fl1d software reset.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 */
void s25fl1xx_soft_reset(struct qspid_t *qspid)
{
	s25fl1xx_exec_command(qspid, SOFT_RESET_ENABLE,0, 0,  QSPI_CMD_ACCESS, 0);
	s25fl1xx_exec_command(qspid, SOFT_RESET, 0, 0, QSPI_CMD_ACCESS, 0);
}

/**
 * \brief Unprotected the control bits of the serial flash device.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 *
 * \return 0 if the device has been unprotected; otherwise returns
 * S25FL1D_ERROR_PROTECTED.
 */
uint8_t s25fl1xx_unprotect(struct qspid_t *qspid)
{
	uint8_t status[3];
	/* Get the status register value to check the current protection */
	status[0]= s25fl1xx_read_status1(qspid);
	status[1]= s25fl1xx_read_status2(qspid);
	status[2]= s25fl1xx_read_status3(qspid);
	if ((status[0] & STATUS_SWP) == STATUS_SWP_PROTNONE) {
		/* Protection already disabled */
		return 0;
	}

	status[0] &= (~STATUS_SWP);
	/* Check if sector protection registers are locked */
	if ((status[0] & STATUS_SPRL) == STATUS_SPRL_LOCKED) {
		status[0] &= (~STATUS_SPRL);
		/* Unprotected sector protection registers by writing the status reg. */
		s25fl1xx_write_status(qspid, status);
	}
	s25fl1xx_write_status(qspid, status);

	/* Check the new status */
	status[0] = s25fl1xx_read_status1(qspid);
	if (status[0] & (STATUS_SPRL | STATUS_SWP)) {
		puts("\r-E Unlock Failed!\n");
		return ERROR_PROTECTED;
	}
	else {
		return 0;
	}
}

/**
 * \brief Unprotected the contents of the serial flash device.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 *
 * \return 0 if the device has been unprotected; otherwise returns
 * S25FL1D_ERROR_PROTECTED.
 */
uint8_t s25fl1xx_data_unprotect(struct qspid_t *qspid)
{
	uint8_t status[3];
	/* Get the status register value to check the current protection */
	status[0]= s25fl1xx_read_status1(qspid);
	status[1]= s25fl1xx_read_status2(qspid);
	status[2]= s25fl1xx_read_status3(qspid);
	if (!(status[0] & CHIP_PROTECT_Msk)) {
		/* Protection already disabled */
		return 0;
	}

	status[0] &= (~CHIP_PROTECT_Msk);
	s25fl1xx_write_status(qspid, status);

	/* Check the new status */
	status[0] = s25fl1xx_read_status(qspid);
	if (status[0] & CHIP_PROTECT_Msk) {
		puts("\r-E Unlock Block Failed!\n");
		return ERROR_PROTECTED;
	}
	else {
		return 0;
	}
}

/**
 * \brief Unprotected the contents of the serial flash device.
 *
 * \param qspid  Pointer to an S25FL1 driver instance.
 *
 * \return 0 if the device has been protected; otherwise returns
 * S25FL1D_ERROR_PROTECTED.
 */
uint8_t s25fl1xx_protect(struct qspid_t *qspid)
{
	uint8_t status[3];
	/* Get the status register value to check the current protection */
	status[0]= s25fl1xx_read_status1(qspid);
	status[1]= s25fl1xx_read_status2(qspid);
	status[2]= s25fl1xx_read_status3(qspid);

	/* Check if sector protection registers are locked */
	if ((status[0] & STATUS_SPRL) == STATUS_SPRL_LOCKED) {
		return 0;
	}

	status[0] |= (STATUS_SWP | STATUS_SPRL);
	s25fl1xx_write_status(qspid, status);

	/* Check the new status */
	status[0] = s25fl1xx_read_status(qspid);
	if ((status[0] & (STATUS_SPRL | STATUS_SWP)) != (STATUS_SPRL | STATUS_SWP)) {
		puts("\r-E Lock protection failed!\n");
		return ERROR_PROTECTED;
	}
	else {
		return 0;
	}
}

/**
 * \brief Erases all the content of the memory chip.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 *
 * \return 0 if the device has been unprotected; otherwise returns
 * ERROR_PROTECTED.
 */
uint8_t s25fl1xx_erase_chip(struct qspid_t *qspid)
{
	char wait_ch[4] = {'\\','|','/','-' };
	uint8_t i=0;
	uint8_t status = STATUS_RDYBSY;
	uint8_t chip_status= s25fl1xx_read_status1(qspid);

	if(chip_status & CHIP_PROTECT_Msk) {
		puts(" -E  Chip is is_protected \n\r");
		printf(" -I  Flash Status Register 1 is %x\n\r", chip_status);
		return 1;
	} else {
		s25fl1xx_enable_write(qspid);
		s25fl1xx_exec_command(qspid, CHIP_ERASE_2, 0, 0, QSPI_CMD_ACCESS, 0);

		while(status & STATUS_RDYBSY) {
			delay_ms(200);
			printf("Erasing flash memory %c\r", wait_ch[i]);
			i++;
			status = s25fl1xx_read_status1(qspid);
			i = i % 4;
		}
		puts("\rErasing flash memory done..... 100%\n\r");
		return 0;
	}
}

/**
 *\brief  Erases the specified block of the serial firmware dataflash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param address  Address of the block to erase.
 *
 * \return 0 if successful; otherwise returns ERROR_PROTECTED if the
 * device is protected or ERROR_BUSY if it is busy executing a command.
 */
uint8_t s25fl1xx_erase_sector(struct qspid_t *qspid,uint32_t address)
{
	uint8_t status;
	/* Check that the flash is ready and unprotected */
	status = s25fl1xx_read_status1(qspid);
	if ((status & STATUS_RDYBSY) != STATUS_RDYBSY_READY) {
		printf(" -E  %s : Flash busy\n\r", __FUNCTION__);
		return ERROR_BUSY;
	}
	else if (status & BLOCK_PROTECT_Msk) {
		if(s25fl1xx_check_protected_addr(status, address)) {
			printf(" -E  %s : Flash addr is protected\n\r", __FUNCTION__);
			return ERROR_PROTECTED;
		}
	}

	/* Enable critical write operation */
	s25fl1xx_enable_write(qspid);

	dev->addr = address;
	dev->inst_frame.bm.b_addr_en = 1;
	/* Start the block erase command */
	s25fl1xx_exec_command(qspid, BLOCK_ERASE_4K, 0, 0, QSPI_CMD_ACCESS, 0);

	/* Wait for transfer to finish */
	s25fl1xx_is_busy(qspid);

	return 0;
}

/**
 *\brief  Erases the specified 64KB block of the serial firmware dataflash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param address  Address of the block to erase.
 *
 * \return 0 if successful; otherwise returns ERROR_PROTECTED if the
 * device is protected or ERROR_BUSY if it is busy executing a command.
 */
uint8_t s25fl1xx_erase_64k_block(struct qspid_t *qspid, uint32_t address)
{
	uint8_t status;

	/* Check that the flash is ready and unprotected */
	status = s25fl1xx_read_status(qspid);
	if ((status & STATUS_RDYBSY) != STATUS_RDYBSY_READY) {
		printf(" -E  S25FL1D_EraseBlock : Flash busy\n\r");
		return ERROR_BUSY;
	}
	else if ((status & STATUS_SWP) != STATUS_SWP_PROTNONE) {
		printf(" -E  EraseBlock : Flash protected\n\r");
		return ERROR_PROTECTED;
	}

	/* Enable critical write operation */
	s25fl1xx_enable_write(qspid);

	dev->addr = address;
	dev->inst_frame.bm.b_addr_en = 1;
	/* Start the block erase command */
	s25fl1xx_exec_command(qspid, BLOCK_ERASE_64K, 0, 0, QSPI_CMD_ACCESS, 0);

	/* Wait for transfer to finish */
	s25fl1xx_is_busy(qspid);

	return 0;
}

/**
 * \brief Writes data at the specified address on the serial firmware dataflash. The
 * page(s) to program must have been erased prior to writing. This function
 * handles page boundary crossing automatically.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param pData  Data buffer.
 * \param size  Number of bytes in buffer.
 * \param address  Write address.
 * \param secure  Enable or disable scramble on QSPI.
 *
 * \return 0 if successful; otherwise, returns ERROR_PROGRAM is there has
 * been an error during the data programming.
 */
 uint8_t s25fl1xx_write(struct qspid_t *qspid, uint32_t *pData, uint32_t size, uint32_t address, uint8_t secure)
{
	uint32_t i = 0;

	 /** Size / page_zize */
	uint32_t  number_of_writes = (size >> 8);
	uint32_t addr = address;

	/** if less than page size */
	if(number_of_writes == 0) {
		s25fl1xx_enable_write(qspid);
		s25fl1xx_memory_access(qspid, BYTE_PAGE_PROGRAM , addr, pData, 0,  QSPI_WRITE_ACCESS, size, secure);
	} else {
		/** multiple page */
		for(i=0; i< number_of_writes; i++) {
			s25fl1xx_enable_write(qspid);
			s25fl1xx_memory_access(qspid, BYTE_PAGE_PROGRAM , addr, pData, 0, QSPI_WRITE_ACCESS, PAGE_SIZE, secure);
			s25fl1xx_is_busy(qspid);
			pData += (PAGE_SIZE >> 2);
			addr += PAGE_SIZE;
		}
		if(size % PAGE_SIZE ) {
			s25fl1xx_enable_write(qspid);
			s25fl1xx_memory_access(qspid, BYTE_PAGE_PROGRAM , addr, pData, 0, QSPI_WRITE_ACCESS, (size - (number_of_writes * PAGE_SIZE)), secure);
			s25fl1xx_is_busy(qspid);
		}
	}
	s25fl1xx_disable_write(qspid);
	return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 *
 * \return 0 if successful; otherwise, fail.
 */
 uint8_t s25fl1xx_read(struct qspid_t *qspid, uint32_t *data,  uint32_t size, uint32_t address)
{
	/** 1 DummyRead is 8 dummy cycles of SPI */
	const uint8_t dummy_read = 1;
	uint8_t *data_rx;
	uint8_t secure = 0;

	data_rx = malloc(size);
	s25fl1xx_memory_access(qspid, READ_ARRAY , address, 0, (uint32_t *)data_rx, QSPI_READ_ACCESS, (size + dummy_read), secure);
	memcpy(data, data_rx , size);

	data_rx = NULL;
	free(data_rx);
	return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 *
 * \return 0 if successful; otherwise, fail.
 */
 uint8_t s25fl1xx_read_dual(struct qspid_t *qspid, uint32_t *data, uint32_t size, uint32_t address)
{
	uint8_t secure = 0;

	mem->inst_frame.bm.b_dummy_cycles = 8;
	mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_DUAL_OUTPUT;

	s25fl1xx_memory_access(qspid, READ_ARRAY_DUAL , address, 0, data, QSPI_READ_ACCESS, size, secure);

	return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 *
 * \return 0 if successful; otherwise, fail.
 */
 uint8_t s25fl1xx_read_quad(struct qspid_t *qspid, uint32_t *data, uint32_t size, uint32_t address)
{
	uint8_t secure = 0;
	mem->inst_frame.bm.b_dummy_cycles  = 8;
	mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_QUAD_OUTPUT;
	s25fl1xx_memory_access(qspid, READ_ARRAY_QUAD,  address, 0, data, QSPI_READ_ACCESS, size, secure);

	return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 * \param cont_mode  Continue read mode.
 * \param secure  Enable or disable scramble on QSPI.
 *
 * \return 0 if successful; otherwise, fail.
 */
 uint8_t s25fl1xx_read_dual_io(struct qspid_t *qspid, uint32_t *data, uint32_t size, uint32_t address,
									uint8_t cont_mode, uint8_t secure)
{
	mem->inst_frame.bm.b_dummy_cycles = 4;
	if(cont_mode) {
		mem->inst_frame.bm.b_opt_len= (QSPI_IFR_OPTL_OPTION_4BIT >> QSPI_IFR_OPTL_Pos);
		qspid->qspi_command.option = 0x02;

		mem->inst_frame.bm.b_continues_read = cont_mode;
		mem->inst_frame.bm.b_dummy_cycles = 3;
	}

	mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_DUAL_IO;

	s25fl1xx_memory_access(qspid, READ_ARRAY_DUAL_IO , address, 0, data, QSPI_READ_ACCESS, size, secure);

	mem->inst_frame.bm.b_opt_en = 0;
	mem->inst_frame.bm.b_continues_read  = 0;

	return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param qspid  Pointer to an S25FL1 qspid_t struct.
 * \param data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 * \param cont_mode  Continue read mode.
 * \param secure  Enable or disable scramble on QSPI.
 *
 * \return 0 if successful; otherwise, fail.
 */
 uint8_t s25fl1xx_read_quad_io(struct qspid_t *qspid, uint32_t *data, uint32_t size, uint32_t address,
									uint8_t cont_mode, uint8_t secure)
{
	mem->inst_frame.bm.b_dummy_cycles = 6;
	if(cont_mode) {
		mem->inst_frame.bm.b_opt_len= (QSPI_IFR_OPTL_OPTION_4BIT >> QSPI_IFR_OPTL_Pos);
		qspid->qspi_command.option = 0x02;
		mem->inst_frame.bm.b_continues_read = cont_mode;
		mem->inst_frame.bm.b_dummy_cycles = 5;
		mem->inst_frame.bm.b_opt_en = 1;
	}

	mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_QUAD_IO;

	s25fl1xx_memory_access(qspid, READ_ARRAY_QUAD_IO , address, 0, data, QSPI_READ_ACCESS, size, secure);

	mem->inst_frame.bm.b_opt_en = 0;
	mem->inst_frame.bm.b_continues_read  = 0;

	return 0;
}

