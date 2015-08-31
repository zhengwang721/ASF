/**
* \file
*
* \brief Spi Flash Driver for SAMB11
*
* Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
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

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "spi_flash.h"


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define SPI_FLASH_MODULE_BASE_ADDR 		0x40012000
#define AHB_DATA_MEM_BASE				0x100000000
#define GET_AHB_ADDRESS(x)				((AHB_DATA_MEM_BASE + (unsigned long)x))


#define FLASH_TOTAL_SZ		(256*1024UL)
#define FLASH_PAGE_SZ 		(256)

#define MIN(x,y)   			(((x)>(y))?(y):(x))

typedef struct{
	unsigned 	spi_mode;
	unsigned 	spi_cmd_count;
	unsigned 	spi_dat_count;
	unsigned 	spi_buffer1;
	unsigned 	spi_buffer2;
	unsigned 	spi_buffer_dir;
	unsigned 	spi_tr_done;
	unsigned 	spi_dma_addr;
}tstrSpi;



/*********************************************************************
Function
		spi_flash_read_security_reg

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
static void spi_flash_write_enable(void)
{
	volatile tstrSpi *m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	uint8_t	 		cmd[8];

	cmd[0] = 0x06;

	m->spi_dat_count = 0;
	m->spi_buffer1 = cmd[0];
	m->spi_buffer_dir = 0x1;
	m->spi_dma_addr = 0;
	m->spi_cmd_count = 1 | (1 << 7);
	while (m->spi_tr_done != 1);
}

/*********************************************************************
Function
		spi_flash_read_security_reg

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
static uint8_t spi_flash_read_status_reg(void)
{
	volatile tstrSpi 		*m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	uint8_t 				cmd[8];
	volatile uint32_t		u32RegVal;
	cmd[0] = 0x05;

	m->spi_dat_count = 1;
	m->spi_buffer1 = cmd[0];
	m->spi_buffer_dir = 0x1;
	m->spi_dma_addr = GET_AHB_ADDRESS(&u32RegVal);
	m->spi_cmd_count = 1 | (1 << 7);
	while (m->spi_tr_done != 1);

	return (u32RegVal & 0xFF);
}
/*********************************************************************
Function
		spi_flash_read_security_reg

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
static void spi_flash_page_program(uint32_t u32MemAddr,uint32_t u32FlashAddr, uint32_t u32Sz)
{
	volatile tstrSpi *m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	unsigned char cmd[8];

	cmd[0] = 0x02;
	cmd[1] = (unsigned char) (u32FlashAddr >> 16);
	cmd[2] = (unsigned char) (u32FlashAddr >> 8);
	cmd[3] = (unsigned char) (u32FlashAddr);

	m->spi_dat_count = 0;
	m->spi_buffer1 = cmd[0] | (cmd[1] << 8) | (cmd[2] << 16) | (cmd[3] << 24);
	m->spi_buffer_dir = 0xf;
	m->spi_dma_addr = u32MemAddr;
	m->spi_cmd_count = 4 | (1<<7) | ((u32Sz & 0xfffff) << 8);
	while (m->spi_tr_done != 1)
		;
}
/*********************************************************************
Function
		spi_flash_read_security_reg

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
static void spi_flash_write_disable(void)
{
	volatile tstrSpi *m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	uint8_t 			cmd[8];

	cmd[0] = 0x04;

	m->spi_dat_count = 0;
	m->spi_buffer1 = cmd[0];
	m->spi_buffer_dir = 0x1;
	m->spi_dma_addr = 0;
	m->spi_cmd_count = 1 | (1 << 7);
	while (m->spi_tr_done != 1);
}
/*********************************************************************
Function
		spi_flash_read_security_reg

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
static void spi_flash_pp(uint32_t u32Offset, uint32_t u32Adr, uint16_t u16Sz)
{
	spi_flash_write_enable();
	
	/* use shared packet memory as temp mem 
	*/
	spi_flash_page_program(u32Adr, u32Offset, u16Sz);
	
	// add additional read_status_reg before the while
	// this gives the flash memory time to update the registers
	spi_flash_read_status_reg();
	while(spi_flash_read_status_reg() & 0x01);
	spi_flash_write_disable();
}
/*********************************************************************
Function
		spi_flash_read_security_reg

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
void spi_flash_init(void)
{
	volatile tstrSpi 		*m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	m->spi_mode = 0x00;
	spi_flash_leave_low_power_mode();
}
/*********************************************************************
Function
		spi_flash_read_security_reg

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
void spi_flash_sector_erase(uint32_t u32FlashAdr)
{
	spi_flash_write_enable();
	spi_flash_read_status_reg();
	{
		volatile tstrSpi 	*m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
		uint8_t 			cmd[8];

		cmd[0] = 0x20;
		cmd[1] = (char)(u32FlashAdr >> 16);
		cmd[2] = (char)(u32FlashAdr >> 8);
		cmd[3] = (char)(u32FlashAdr);

		m->spi_dat_count = 0;
		m->spi_buffer1 = cmd[0]|(cmd[1]<<8)|(cmd[2]<<16)|(cmd[3]<<24);
		m->spi_buffer_dir = 0xf;
		m->spi_dma_addr = 0;
		m->spi_cmd_count = 4 | (1 << 7);
		while (m->spi_tr_done != 1);
	}
	while(spi_flash_read_status_reg() & 0x01);
}
/*********************************************************************
Function
		spi_flash_read

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
void spi_flash_read
(
void		*pvReadBuffer, 
uint32_t	u32FlashAddr, 
uint32_t	u32ReadSize
)
{
	volatile tstrSpi 		*m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	uint8_t			cmd[8];
	uint32_t			u32MemAddr;

	/* Get the destination buffer Address.
	*/


	if((u32FlashAddr+u32ReadSize) > FLASH_MEMORY_SIZE)
	{
		int i=0;
		uint8_t * data=pvReadBuffer;
		for(i=0;i<u32ReadSize;i++)
		{
			data[i]=0;
		}
	}
	
	u32MemAddr = GET_AHB_ADDRESS((unsigned long)pvReadBuffer);

	/* Perform read operation.
	*/
	cmd[0] = 0x0b;
	cmd[1] = (unsigned char) (u32FlashAddr >> 16);
	cmd[2] = (unsigned char) (u32FlashAddr >> 8);
	cmd[3] = (unsigned char) (u32FlashAddr);
	cmd[4] = 0xA5;

	m->spi_dat_count = u32ReadSize;
	m->spi_buffer1 = cmd[0] | (cmd[1] << 8) | (cmd[2] << 16) | (cmd[3] << 24);
	m->spi_buffer2 = cmd[4];
	m->spi_buffer_dir = 0x1f;
	m->spi_dma_addr = u32MemAddr;
	m->spi_cmd_count = 5 | (1 << 7);
	while (m->spi_tr_done != 1);
}
/*********************************************************************
Function
		spi_flash_write

Description
		
	
Return

Author

Version
		1.0

Date

*********************************************************************/
int8_t spi_flash_write
(
void		*pvWriteBuffer, 
uint32_t	u32FlashAddr, 
uint32_t	u32WriteSize
)
{
	int8_t		s8Ret = -1;
	if((pvWriteBuffer != NULL) && (u32WriteSize != 0))
	{
		/* Ensure the write size does not exceed the flash limit.
		*/
		if((u32FlashAddr + u32WriteSize) <= FLASH_TOTAL_SZ)
		{
			uint32_t 	u32wsz;
			uint32_t 	u32off;
			uint32_t	u32MemAddr;

			/* Get the destination buffer Address.
			*/
			u32MemAddr = GET_AHB_ADDRESS((unsigned long)pvWriteBuffer);

			/* Perform read operation.
			*/
			u32off = u32FlashAddr % FLASH_PAGE_SZ;

			if (u32off)/*first part of data in the address page*/
			{
				u32wsz = FLASH_PAGE_SZ - u32off;
				spi_flash_pp(u32FlashAddr, u32MemAddr, MIN(u32WriteSize, u32wsz));
				if (u32WriteSize < u32wsz)
				{
					s8Ret = 0;
					goto EXIT;
				}


				u32MemAddr += u32wsz;
				u32FlashAddr += u32wsz;
				u32WriteSize -= u32wsz;
			}
			do
			{
				u32wsz = MIN(u32WriteSize, FLASH_PAGE_SZ);
				
				/* Write complete page or the remaining data.
				*/
				spi_flash_pp(u32FlashAddr, u32MemAddr, u32wsz);
				u32MemAddr += u32wsz;
				u32FlashAddr += u32wsz;
				u32WriteSize -= u32wsz;
			} while (u32WriteSize > 0);
			s8Ret = 0;
		}
	}
	EXIT:
	return s8Ret;
}
/*********************************************************************
Function
		spi_flash_rdid

Description


Return

Author

Version
		1.0

Date

*********************************************************************/
uint32_t spi_flash_rdid(void)
{
	volatile tstrSpi 		*m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	uint8_t 				cmd[8];
	volatile uint32_t		u32RegVal;
	cmd[0] = 0x9f;

	m->spi_dat_count = 4;
	m->spi_buffer1 = cmd[0];
	m->spi_buffer_dir = 0x1;
	m->spi_dma_addr = GET_AHB_ADDRESS(&u32RegVal);
	
	m->spi_cmd_count = 1 | (1 << 7);
	while (m->spi_tr_done != 1);
	
	return (u32RegVal);
}

unsigned char spi_flash_erase(uint32_t u32StartOffset, uint32_t u32Size)
{
    unsigned long u32EndOffset = u32StartOffset + u32Size;


    //  Check address overflow
    if (u32EndOffset > FLASH_MEMORY_SIZE) 
    {
        return 1;
    }

    // Align to previous sector boundary
    u32StartOffset = u32StartOffset & FLASH_SECT_MASK;

    // Erase next sectors
    spi_flash_write_enable();
    while(u32StartOffset < u32EndOffset)
    {   	
			spi_flash_sector_erase(u32StartOffset);
			while(spi_flash_read_status_reg() & 0x01)
				;
			u32StartOffset += FLASH_SECT_SIZE;
    }
		
		spi_flash_write_disable();

	

    return 0;
}

/*********************************************************************
Function
		spi_flash_enter_low_power_mode

Description
		Enter SPI flash low power mode
Return

Author

Version
		1.0

Date

*********************************************************************/
void spi_flash_enter_low_power_mode(void) 
{
	volatile tstrSpi *m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	volatile unsigned long tmp;
	unsigned char* cmd = (unsigned char*) &tmp;
	
	cmd[0] = 0xb9;

	m->spi_dat_count = 0;
	m->spi_buffer1 = cmd[0];
	m->spi_buffer_dir = 0x1;
	m->spi_dma_addr = 0;
	m->spi_cmd_count = 1 | (1 << 7);
	while (m->spi_tr_done != 1);
}

/*********************************************************************
Function
		spi_flash_leave_low_power_mode

Description
		Leave SPI flash low power mode
Return

Author

Version
		1.0

Date

*********************************************************************/
void spi_flash_leave_low_power_mode(void)
{
	volatile tstrSpi *m = (volatile tstrSpi *) (SPI_FLASH_MODULE_BASE_ADDR);
	volatile unsigned long tmp;
	unsigned char* cmd = (unsigned char*) &tmp;
	volatile int x,d;


	cmd[0] = 0xab;

	m->spi_dat_count = 0;
	m->spi_buffer1 = cmd[0];
	m->spi_buffer_dir = 0x1;
	m->spi_dma_addr = 0;
	m->spi_cmd_count = 1 | (1 << 7);
	while (m->spi_tr_done != 1);
}
	
