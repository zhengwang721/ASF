/* ----------------------------------------------------------------------------
 *         SAMV71 Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2015, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/**
 * \addtogroup at25d_module S25FL1 driver
 * \ingroup lib_spiflash
 * The S25FL1 serial dataflash driver is based on the corresponding S25FL1 SPI driver.
 * A S25FL1 instance has to be initialized using the Dataflash levle function
 * S25FL1D_Configure(). S25FL1 Dataflash can be automatically detected using
 * the S25FL1D_FindDevice() function. Then S25FL1 dataflash operations such as
 * read, write and erase DF can be launched using s25fl1d_send_command function
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
 * s25fl1d_is_busy(p_qspid).</li>
 * <li> Retrieves and returns the S25fl1 current using s25fl1d_read_status(p_qspid).</li>
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
#include "s25fl1.h"


/*----------------------------------------------------------------------------
 *        Variable
 *----------------------------------------------------------------------------*/

qspi_inst_frame_t *p_dev;
qspi_inst_frame_t *p_mem;


/*----------------------------------------------------------------------------
 *        Definition
 *----------------------------------------------------------------------------*/
#define READ_DEV        0
#define WRITE_DEV       1 

#define PAGE_SIZE       256

/** Tick Counter united by ms */
volatile uint32_t systick_count = 0 ;
uint8_t systick_configured = 0 ;

/**
 * \brief Get Delayed number of tick
 * \param startTick Start tick point.
 * \param endTick   End tick point.
 */
uint32_t get_delay_in_systick(uint32_t time_start_us, uint32_t time_end_us)
{
    assert(systick_configured);
    
    if (time_end_us >= time_start_us) {
		return (time_end_us - time_start_us);
	}
    return (time_end_us + (0xFFFFFFFF - time_start_us) + 1);   
}

/**
 *  \brief Sync Wait for several ms
 *  \param ul_ms    Waiting time in ms.
 */
void delay_ms(volatile uint32_t ul_ms)
{
    uint32_t time_start_us;
    
    assert(systick_configured);
    
    time_start_us = systick_count ;
    while(get_delay_in_systick(time_start_us, systick_count) < ul_ms);   
}

/**
 * \brief Send command to the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param instr  Instruction to be execute.
 * \param p_tx_data  Data buffer to send data.
 * \param p_rx_data  Data buffer to receive data.
 * \param read_write  Read/write access.
 * \param size  Data size to be read/write.
 */
void s25fl1d_send_command(qspid_t *p_qspid, uint8_t instr, uint32_t *p_tx_data, uint32_t *p_rx_data, qspi_access_t read_write, uint32_t size)
{
	p_qspid->qspi_command.instruction = instr;
	p_dev->inst_frame.bm.b_inst_en = 1;	
	p_qspid->p_qspi_frame = p_dev;
	p_qspid->qspi_buffer.p_data_tx = p_tx_data;
	p_qspid->qspi_buffer.p_data_rx = p_rx_data;
	
	// to prevent unaligned access
	if( (size % sizeof(uint32_t)) && size > 1) {
		size += (sizeof(uint32_t) - (size % sizeof(uint32_t)));
	}
	
	if(read_write == cmd_access) {
		p_dev->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ >> QSPI_IFR_TFRTYP_Pos);
		p_dev->inst_frame.bm.b_data_en = 0;
		
		qspi_flash_send_command(p_qspid, 0);
	}
	else if (read_write == write_access) {
		p_dev->inst_frame.bm.b_data_en = 1;
		p_dev->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_WRITE >> QSPI_IFR_TFRTYP_Pos);
		p_qspid->qspi_buffer.tx_data_size = size;
		qspi_flash_send_command_with_data(p_qspid, 0);
	}
	else {
		p_dev->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ >> QSPI_IFR_TFRTYP_Pos);
		p_dev->inst_frame.bm.b_data_en = 1;
		p_qspid->qspi_buffer.rx_data_size = size;
		qspi_flash_read_command(p_qspid, 0);
	}
}

/**
 * \brief Read/write data from serial flash memory.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct..
 * \param instr Instruction to be execute.
 * \param p_tx_data  Data buffer to send data.
 * \param p_rx_data  Data buffer to receive data.
 * \param read_write  Read/write access.
 * \param size  Data size to be read/write.
 * \param secure  Enable or disable scramble on QSPI.
 */
void s25fl1d_memory_access(qspid_t *p_qspid, uint8_t instr, uint32_t addr, uint32_t *p_tx_data, uint32_t *p_rx_data, qspi_access_t read_write, uint32_t size, uint8_t secure)
{
	uint8_t qspi_buffer[4];
	p_qspid->qspi_command.instruction = instr;
	p_qspid->qspi_mode = mem_mode;
	p_qspid->qspi_buffer.p_data_tx = p_tx_data;
	p_qspid->qspi_buffer.p_data_rx = p_rx_data;
	p_mem->addr= addr;
	p_mem->inst_frame.bm.b_inst_en = 1;
	p_mem->inst_frame.bm.b_data_en = 1;
	p_mem->inst_frame.bm.b_addr_en = 1;
	p_qspid->p_qspi_frame = p_mem;
	
	if (read_write == write_access) {
		p_mem->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_WRITE_MEMORY >> QSPI_IFR_TFRTYP_Pos);
		p_qspid->qspi_buffer.tx_data_size = size;
	} else {
		p_mem->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ_MEMORY >> QSPI_IFR_TFRTYP_Pos);
		p_qspid->qspi_buffer.rx_data_size = size;
	}
	qspi_flash_enable_mem_access(p_qspid, 0, secure);
	qspi_flash_read_write_mem(p_qspid, read_write);
}

/**
 * \brief Reads and returns the status register of the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct..
 */
static uint8_t s25fl1d_read_status1(qspid_t *p_qspid)
{
    uint8_t status;    
    s25fl1d_send_command(p_qspid, READ_STATUS_1, 0, (uint32_t *)(&status), read_access, 1);
    return status;
}

/**
 * \brief Reads and returns the status register of the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct..
 */
static uint8_t s25fl1d_read_status2(qspid_t *p_qspid)
{
    uint8_t status;    
    s25fl1d_send_command(p_qspid, READ_STATUS_2, 0, (uint32_t *)&status, read_access, 1);
    return status;
}

/**
 * \brief Reads and returns the status register of the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 */
static uint8_t s25fl1d_read_status3(qspid_t *p_qspid)
{
    uint8_t status;
    s25fl1d_send_command(p_qspid, READ_STATUS_3, 0, (uint32_t *)&status, read_access, 1);
    return status;
}

/**
 * \brief Reads and returns the status register of the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 */
uint32_t s25fl1d_read_status(qspid_t *p_qspid)
{
    uint32_t status;
    status = s25fl1d_read_status1(p_qspid) | (s25fl1d_read_status2(p_qspid) << 8) | (s25fl1d_read_status3(p_qspid) << 16);	
    return status;
}

/**
 * \brief Wait for transfer to finish calling the SPI driver ISR. (interrupts are disabled)
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 */
static void s25fl1d_is_busy(qspid_t *p_qspid)
{
    while(s25fl1d_read_status1(p_qspid) & STATUS_RDYBSY);
}

/**
 * \brief Enable write option to serical flash memory.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 */
static void s25fl1d_enable_write(qspid_t *p_qspid)
{
    uint8_t status = 0;
    while(!(status & STATUS_WEL)) {
      s25fl1d_send_command(p_qspid, WRITE_ENABLE, 0, 0, cmd_access, 0);
      status = s25fl1d_read_status1(p_qspid);
    }
}

/**
 * \brief Disable write option to serical flash memory.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 */
static void s25fl1d_disable_write(qspid_t *p_qspid)
{
    uint8_t status;
    status = s25fl1d_read_status1(p_qspid);
    while( (status & STATUS_WEL) != 0) {
        s25fl1d_send_command(p_qspid, WRITE_DISABLE, 0, 0, cmd_access, 0);
        status = s25fl1d_read_status1(p_qspid);
    }
}

/**
 * \brief Writes the given value in the status register of the serial flash device.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param p_status  Status to write.
 */
static void s25fl1d_write_status(qspid_t *p_qspid, uint8_t *p_status)
{
    s25fl1d_enable_write(p_qspid);
    s25fl1d_send_command(p_qspid, WRITE_STATUS, (uint32_t *)p_status, 0, write_access, 3);  
    s25fl1d_disable_write(p_qspid);
}

/**
 * \brief Writes the given value in the status register of the serial flash device.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param status  Status to write.
 */
static void s25fl1d_write_volatile_status(qspid_t *p_qspid, uint8_t *p_status)
{
    s25fl1d_send_command(p_qspid, 0x50, 0, 0 , cmd_access, 0); 
    s25fl1d_send_command(p_qspid, WRITE_STATUS, &p_status, 0 , write_access, 3);
    s25fl1d_disable_write(p_qspid);
}

/**
 * \brief Check weather the address of ul_addr is protected.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param ul_addr  Address to be checked.
 */
static uint8_t s25fl1d_check_protected_addr(uint8_t status1, uint32_t ul_addr)
{
  const uint32_t addr_ump = (status1 & SEC_PROTECT_Msk) ? 0x001000UL : 0x010000UL;
  static uint8_t is_protected = 0;
    
  const uint8_t block_bits = ((status1 & BLOCK_PROTECT_Msk) >> 2);
  
  switch(block_bits) {
	case 1:
	if (status1 & TOP_BTM_PROTECT_Msk)
	{
		if((ul_addr > 0x000000) && (ul_addr < (0x000000 + addr_ump - 1)))
		{
			is_protected = 1;
		}
	} else {
		if((ul_addr > (0x1FFFFF - addr_ump + 1)) && (ul_addr < 0x1FFFFF))
		{
			is_protected = 1;
		}
	}
	break;
    
  case 2:
    if (status1 & TOP_BTM_PROTECT_Msk) {
		if((ul_addr > 0x000000) && (ul_addr < (0x000000 + (2* addr_ump)- 1))) {
			is_protected = 1;
		}
    } else {
		if((ul_addr > (0x1FFFFF - (2*addr_ump ) + 1)) && (ul_addr < 0x1FFFFF)) {
			is_protected = 1;
		}
    }
    break;
	
  case 3:
    if (status1 & TOP_BTM_PROTECT_Msk) {
		if((ul_addr > 0x000000) && (ul_addr < (0x000000 + (4 * addr_ump) - 1))) {
			is_protected = 1;
		}
    } else {
		if( (ul_addr > (0x1FFFFF - (4*addr_ump) + 1)) && (ul_addr < 0x1FFFFF)) {
			is_protected = 1;
		}
    }
    break;
    
  case 4:
    if (status1 & TOP_BTM_PROTECT_Msk) {
		if((ul_addr > 0x000000) && (ul_addr < (0x000000 + (8 * addr_ump) - 1))) {
			is_protected = 1;
		}
    } else {
		if((ul_addr > (0x1FFFFF - (8*addr_ump ) + 1)) && (ul_addr < 0x1FFFFF)) {
			is_protected = 1;
		}
    }
    break;
	
  case 5:
    if(!(status1 & SEC_PROTECT_Msk)) {
		if (status1 & TOP_BTM_PROTECT_Msk) {
			if((ul_addr > 0x000000) && (ul_addr < (0x000000 + (16 * addr_ump) - 1))) {
				is_protected = 1;
			}
		} else {
			if((ul_addr > (0x1FFFFF - (16*addr_ump) + 1)) && (ul_addr < 0x1FFFFF)) {
				is_protected = 1;
			}
		}
    }
    break;
    
  case 6:    
    if(!(status1 & SEC_PROTECT_Msk)) {    
		if (status1 & TOP_BTM_PROTECT_Msk) {
			if((ul_addr > 0x000000) && (ul_addr < (0x000000 + (32 * addr_ump) - 1))) {
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
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 */
uint32_t s25fl1d_read_jedec_id(qspid_t *p_qspid)
{
    static uint32_t pId;
    s25fl1d_send_command(p_qspid, READ_JEDEC_ID, 0, &pId, read_access, 3);
    return pId;
}

/**
 * \brief Enables critical writes operation on a serial flash device, such as sector
 * protection, status register, etc.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param ul_mode  Mode to be set.
 */
void s25fl1d_set_quad_mode(qspid_t *p_qspid, uint8_t ul_mode)
{
    uint8_t status[3];
    
    status[0] = s25fl1d_read_status1(p_qspid);
    status[1] = s25fl1d_read_status2(p_qspid);
    status[2] = s25fl1d_read_status3(p_qspid);

    if(ul_mode) {
		while(!(status[1] & STATUS_QUAD_ENABLE)) {
			status[1] |= STATUS_QUAD_ENABLE;
			s25fl1d_write_status(p_qspid, status);
			status[1] = s25fl1d_read_status2(p_qspid);
			delay_ms(50);
		}
    } else {
		while((status[1] & STATUS_QUAD_ENABLE)) {
			status[1] &= (~STATUS_QUAD_ENABLE)  ;
			s25fl1d_write_status(p_qspid, status);
			status[1] = s25fl1d_read_status2(p_qspid);
			delay_ms(50);
		}
    }
}

/**
 * \brief Enables critical writes operation on a serial flash device, such as sector
 * protection, status register, etc.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param byte_align  Burst wrap length to be set.
 */
void s25fl1d_enable_wrap(qspid_t *p_qspid, uint8_t byte_align)
{
    uint8_t status[3];

    status[0] = s25fl1d_read_status1(p_qspid);
    status[1] = s25fl1d_read_status2(p_qspid);
    status[2] = s25fl1d_read_status3(p_qspid);

    status[2] |= (byte_align << 5);

    p_dev->inst_frame.bm.b_dummy_cycles = 24;
    s25fl1d_send_command(p_qspid, WRAP_ENABLE,(uint32_t *)&status[2], 0,  write_access, 1);

    s25fl1d_write_volatile_status(p_qspid, status);
    status[2] = s25fl1d_read_status3(p_qspid);
    delay_ms(50);
}

/**
 * \brief Enables critical writes operation on a serial flash device, such as sector
 * protection, status register, etc.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param latency Latency control.
 */
void s25fl1d_set_read_latency_control(qspid_t *p_qspid, uint8_t latency)
{
    uint8_t status[3];

    status[0] = s25fl1d_read_status1(p_qspid);
    status[1] = s25fl1d_read_status2(p_qspid);
    status[2] = s25fl1d_read_status3(p_qspid);

    status[2] |= latency;

	p_qspid->qspi_buffer.p_data_tx = (uint32_t *)&status[2];
    while((status[2] & STATUS_LATENCY_CTRL) != latency) {
		s25fl1d_write_volatile_status(p_qspid, status);
		status[2] = s25fl1d_read_status3(p_qspid);
		delay_ms(50);
    }
}

/**
 * \brief s25fl1d software reset.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 */
void s25fl1d_soft_reset(qspid_t *p_qspid)
{
    s25fl1d_send_command(p_qspid, SOFT_RESET_ENABLE,0, 0,  cmd_access, 0);
    s25fl1d_send_command(p_qspid, SOFT_RESET, 0, 0, cmd_access, 0);
}

/**
 * \brief Unprotected the control bits of the serial flash device.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 *
 * \return 0 if the device has been unprotected; otherwise returns
 * S25FL1D_ERROR_PROTECTED.
 */
unsigned char s25fl1d_unprotect(qspid_t *p_qspid)
{
    unsigned char status[3];
    /* Get the status register value to check the current protection */
    status[0]= s25fl1d_read_status1(p_qspid);
    status[1]= s25fl1d_read_status2(p_qspid);
    status[2]= s25fl1d_read_status3(p_qspid);
    if ((status[0] & STATUS_SWP) == STATUS_SWP_PROTNONE) {
        /* Protection already disabled */
        return 0;
    }

    status[0] &= (~STATUS_SWP);
    /* Check if sector protection registers are locked */
    if ((status[0] & STATUS_SPRL) == STATUS_SPRL_LOCKED) {
        status[0] &= (~STATUS_SPRL);
        /* Unprotected sector protection registers by writing the status reg. */
        s25fl1d_write_status(p_qspid, status);
    }
    s25fl1d_write_status(p_qspid, status);

    /* Check the new status */
    status[0] = s25fl1d_read_status1(p_qspid);
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
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 *
 * \return 0 if the device has been unprotected; otherwise returns
 * S25FL1D_ERROR_PROTECTED.
 */
unsigned char s25fl1d_data_unprotect(qspid_t *p_qspid)
{
    unsigned char status[3];
    /* Get the status register value to check the current protection */
    status[0]= s25fl1d_read_status1(p_qspid);
    status[1]= s25fl1d_read_status2(p_qspid);
    status[2]= s25fl1d_read_status3(p_qspid);
    if (!(status[0] & CHIP_PROTECT_Msk)) {
        /* Protection already disabled */
        return 0;
    }

    status[0] &= (~CHIP_PROTECT_Msk);
    s25fl1d_write_status(p_qspid, status);

    /* Check the new status */
    status[0] = s25fl1d_read_status(p_qspid);
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
 * \param pS25fl1  Pointer to an S25FL1 driver instance.
 *
 * \return 0 if the device has been protected; otherwise returns
 * S25FL1D_ERROR_PROTECTED.
 */
unsigned char s25fl1d_protect(qspid_t *p_qspid)
{
    unsigned char status[3];
    /* Get the status register value to check the current protection */
    status[0]= s25fl1d_read_status1(p_qspid);
    status[1]= s25fl1d_read_status2(p_qspid);
    status[2]= s25fl1d_read_status3(p_qspid);

    /* Check if sector protection registers are locked */
    if ((status[0] & STATUS_SPRL) == STATUS_SPRL_LOCKED) {
        return 0;
    }
	
    status[0] |= (STATUS_SWP | STATUS_SPRL);
    s25fl1d_write_status(p_qspid, status);

    /* Check the new status */
    status[0] = s25fl1d_read_status(p_qspid);
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
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 *
 * \return 0 if the device has been unprotected; otherwise returns
 * ERROR_PROTECTED.
 */
unsigned char s25fl1d_erase_chip(qspid_t *p_qspid)
{
    char wait_ch[4] = {'\\','|','/','-' };
    uint8_t i=0;
    uint8_t status = STATUS_RDYBSY;
    uint8_t chip_status= s25fl1d_read_status1(p_qspid);
    
    if(chip_status & CHIP_PROTECT_Msk) {
		printf(" -E  Chip is is_protected \n\r");
		printf(" -I  Flash Status Register 1 is %x\n\r", chip_status);
		return 1;
    } else {
		s25fl1d_enable_write(p_qspid);
		s25fl1d_send_command(p_qspid, CHIP_ERASE_2, 0, 0, cmd_access, 0);

		while(status & STATUS_RDYBSY) {
			delay_ms(200);
			printf("Erasing flash memory %c\r", wait_ch[i]);
			i++;
			status = s25fl1d_read_status1(p_qspid);   
			i = i % 4;
		}
		printf("\rErasing flash memory done..... 100%\n\r");
		return 0;
    }
}

/**
 *\brief  Erases the specified block of the serial firmware dataflash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param address  Address of the block to erase.
 *
 * \return 0 if successful; otherwise returns ERROR_PROTECTED if the
 * device is protected or ERROR_BUSY if it is busy executing a command.
 */
unsigned char s25fl1d_erase_sector(qspid_t *p_qspid,unsigned int address)
{
    uint8_t status;
    uint8_t secure = 0;
    /* Check that the flash is ready and unprotected */
    status = s25fl1d_read_status1(p_qspid);
    if ((status & STATUS_RDYBSY) != STATUS_RDYBSY_READY) {
        printf(" -E  %s : Flash busy\n\r", __FUNCTION__);
        return ERROR_BUSY;
    }
    else if (status & BLOCK_PROTECT_Msk) {
        if(s25fl1d_check_protected_addr(status, address)) {
			printf(" -E  %s : Flash addr is protected\n\r", __FUNCTION__);
			return ERROR_PROTECTED;
        }
    }

    /* Enable critical write operation */
    s25fl1d_enable_write(p_qspid);

    p_dev->addr = address;
    p_dev->inst_frame.bm.b_addr_en = 1;
    /* Start the block erase command */
    s25fl1d_send_command(p_qspid, BLOCK_ERASE_4K, 0, 0, cmd_access, 0);
    
    /* Wait for transfer to finish */
    s25fl1d_is_busy(p_qspid);

    return 0;
}

/**
 *\brief  Erases the specified 64KB block of the serial firmware dataflash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param address  Address of the block to erase.
 *
 * \return 0 if successful; otherwise returns ERROR_PROTECTED if the
 * device is protected or ERROR_BUSY if it is busy executing a command.
 */
unsigned char s25fl1d_erase_64k_block(qspid_t *p_qspid, unsigned int address)
{
    unsigned char status;

    /* Check that the flash is ready and unprotected */
    status = s25fl1d_read_status(p_qspid);
    if ((status & STATUS_RDYBSY) != STATUS_RDYBSY_READY) {
        printf(" -E  S25FL1D_EraseBlock : Flash busy\n\r");
        return ERROR_BUSY;
    }
    else if ((status & STATUS_SWP) != STATUS_SWP_PROTNONE) {
        printf(" -E  EraseBlock : Flash protected\n\r");
        return ERROR_PROTECTED;
    }

    /* Enable critical write operation */
    s25fl1d_enable_write(p_qspid);

    p_dev->addr = address;
    p_dev->inst_frame.bm.b_addr_en = 1;
    /* Start the block erase command */
    s25fl1d_send_command(p_qspid, BLOCK_ERASE_64K, 0, 0, cmd_access, 0);
    
    /* Wait for transfer to finish */
    s25fl1d_is_busy(p_qspid);

    return 0;
}

/**
 * \brief Writes data at the specified address on the serial firmware dataflash. The
 * page(s) to program must have been erased prior to writing. This function
 * handles page boundary crossing automatically.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param pData  Data buffer.
 * \param size  Number of bytes in buffer.
 * \param address  Write address.
 * \param secure  Enable or disable scramble on QSPI.
 *
 * \return 0 if successful; otherwise, returns ERROR_PROGRAM is there has
 * been an error during the data programming.
 */
 unsigned char s25fl1d_write(qspid_t *p_qspid, uint32_t *pData, uint32_t size, uint32_t address, uint8_t secure)
{
    unsigned int i = 0;
    
	 /* Size / pagezize */
    uint32_t  number_of_writes = (size >> 8);
    uint32_t addr = address;
  
	/* if less than page size */
    if(number_of_writes == 0) {
        s25fl1d_enable_write(p_qspid);
        s25fl1d_memory_access(p_qspid, BYTE_PAGE_PROGRAM , addr, pData, 0,  write_access, size, secure);
    } else {
		/* multiple page */
        for(i=0; i< number_of_writes; i++) {
            s25fl1d_enable_write(p_qspid);
            s25fl1d_memory_access(p_qspid, BYTE_PAGE_PROGRAM , addr, pData, 0, write_access, PAGE_SIZE, secure);            
            s25fl1d_is_busy(p_qspid);
            pData += (PAGE_SIZE >> 2);
            addr += PAGE_SIZE;
        }
        if(size % PAGE_SIZE ) {
            s25fl1d_enable_write(p_qspid);
            s25fl1d_memory_access(p_qspid, BYTE_PAGE_PROGRAM , addr, pData, 0, write_access, (size - (number_of_writes * PAGE_SIZE)), secure);         
            s25fl1d_is_busy(p_qspid);
        }
    }
    s25fl1d_disable_write(p_qspid);
    return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param p_data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 *
 * \return 0 if successful; otherwise, fail.
 */
 unsigned char s25fl1d_read(qspid_t *p_qspid, uint32_t *p_data,  uint32_t size, uint32_t address)
{
	/** 1 DummyRead is 8 dummy cycles of SPI */
    const uint8_t dummy_read = 1;    
    uint8_t *p_data_rx;
    uint8_t secure = 0;
    
    p_data_rx = malloc(size);
    s25fl1d_memory_access(p_qspid, READ_ARRAY , address, 0, (uint32_t *)p_data_rx, read_access, (size + dummy_read), secure);
    memcpy(p_data, p_data_rx , size);
    
    p_data_rx = NULL;
    free(p_data_rx);
    return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param p_data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 *
 * \return 0 if successful; otherwise, fail.
 */
 unsigned char s25fl1d_read_dual(qspid_t *p_qspid, uint32_t *p_data, uint32_t size, uint32_t address)
{
    uint8_t secure = 0;

    p_mem->inst_frame.bm.b_dummy_cycles = 8;
    p_mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_DUAL_OUTPUT;
    
    s25fl1d_memory_access(p_qspid, READ_ARRAY_DUAL , address, 0, p_data, read_access, size, secure);
    
    return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param p_data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 *
 * \return 0 if successful; otherwise, fail.
 */
 unsigned char s25fl1d_read_quad(qspid_t *p_qspid, uint32_t *p_data, uint32_t size, uint32_t address)
{
    uint8_t secure = 0;
    p_mem->inst_frame.bm.b_dummy_cycles  = 8;
    p_mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_QUAD_OUTPUT;
    s25fl1d_memory_access(p_qspid, READ_ARRAY_QUAD,  address, 0, p_data, read_access, size, secure);
    
    return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param p_data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 *
 * \return 0 if successful; otherwise, fail.
 */
 unsigned char s25fl1d_read_dual_io(qspid_t *p_qspid, uint32_t *p_data, uint32_t size, uint32_t address,
									uint8_t cont_mode, uint8_t secure)
{
      p_mem->inst_frame.bm.b_dummy_cycles = 4;
      if(cont_mode) {
          p_mem->inst_frame.bm.b_opt_len= (QSPI_IFR_OPTL_OPTION_4BIT >> QSPI_IFR_OPTL_Pos);
		  p_qspid->qspi_command.option = 0x02;
		  
          p_mem->inst_frame.bm.b_continues_read = cont_mode;
          p_mem->inst_frame.bm.b_dummy_cycles = 3;
      }

      p_mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_DUAL_IO;
      
      s25fl1d_memory_access(p_qspid, READ_ARRAY_DUAL_IO , address, 0, p_data, read_access, size, secure);
      
      p_mem->inst_frame.bm.b_opt_en = 0;
      p_mem->inst_frame.bm.b_continues_read  = 0;
	  
      return 0;
}

/**
 * \brief Reads data from the specified address on the serial flash.
 *
 * \param p_qspid  Pointer to an S25FL1 qspid_t struct.
 * \param p_data  Data buffer.
 * \param size  Number of bytes to read.
 * \param address  Read address.
 *
 * \return 0 if successful; otherwise, fail.
 */
 unsigned char s25fl1d_read_quad_io(qspid_t *p_qspid, uint32_t *p_data, uint32_t size, uint32_t address,
									uint8_t cont_mode, uint8_t secure)
{
      p_mem->inst_frame.bm.b_dummy_cycles = 6;
      if(cont_mode) {
          p_mem->inst_frame.bm.b_opt_len= (QSPI_IFR_OPTL_OPTION_4BIT >> QSPI_IFR_OPTL_Pos);
		  p_qspid->qspi_command.option = 0x02;
          p_mem->inst_frame.bm.b_continues_read = cont_mode;
          p_mem->inst_frame.bm.b_dummy_cycles = 5;
          p_mem->inst_frame.bm.b_opt_en = 1;
      }

      p_mem->inst_frame.bm.b_width = QSPI_IFR_WIDTH_QUAD_IO;
      
      s25fl1d_memory_access(p_qspid, READ_ARRAY_QUAD_IO , address, 0, p_data, read_access, size, secure);
      
      p_mem->inst_frame.bm.b_opt_en = 0;
      p_mem->inst_frame.bm.b_continues_read  = 0;

      return 0;  
}

