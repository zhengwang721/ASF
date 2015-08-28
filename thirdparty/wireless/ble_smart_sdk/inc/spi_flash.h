/**
****************************************************************************************
*
* @file uart.h
*
* @brief SAMB11 UART API for Applications
*
* This module contains the public API and the necessary enumerations and structures that are required for 
* SAMB11 Application Developers using UART module
*
*
*  Copyright (c) 2015 Atmel Corporation. All rights reserved.
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  1. Redistributions of source code must retain the above copyright notice, this
*  list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright notice,
*  this list of conditions and the following disclaimer in the documentation
*  and/or other materials provided with the distribution.
*
*  3. The name of Atmel may not be used to endorse or promote products derived from this software 
*  without specific prior written permission.
*
*  4. This software may only be redistributed and used in connection with an Atmel microcontroller product.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
*  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
*  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
*  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************************
*/


#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__
#include <stdint.h>
#include <stdio.h>


///Flash memory size: 265 KBytes
#define FLASH_MEMORY_SIZE		0x40000
///Flash memory sector size 4 KBytes
#define FLASH_SECT_SIZE			0x1000
///Flash memory page size: 256 Bytes
#define FLASH_PAGE_SIZE			0x100

///Flash sector Mask
#define FLASH_SECT_MASK			0xFFF000
///Flash page mask
#define FLASH_PAGE_MASK			0xFFFF00

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
/**
@defgroup spi-flash-drv SPI FLASH Driver API

@{
*/



/**
* \brief Initializes the SPI Flash module
*
* Initializes the SPI Flash module
*/
void spi_flash_init(void) ;

/**
* \brief Read SPI Flash Chip ID
*
* Reads SPI Flash Chip ID
*/
uint32_t spi_flash_rdid(void);


/**
* \brief Read SPI Flash memory
*
* Reads SPI Flash memory with up to page size (256 bytes) length
* \param[in]  pvReadBuffer  pointer to buffer to read into
* \param[in]  u32FlashAddr  flash memory address to read from
* \param[in]  u32ReadSize  data length to be read, must be less than or equal to FLASH_PAGE_SIZE
*/
void spi_flash_read
(
void		*pvReadBuffer, 
uint32_t	u32FlashAddr, 
uint32_t	u32ReadSize
);

/**
* \brief Write SPI Flash memory
*
* Writes SPI Flash memory with up to page size (256 bytes) length
* \param[in]  pvWriteBuffer  pointer to buffer to write from
* \param[in]  u32FlashAddr  flash memory address to write to
* \param[in]  u32WriteSize  data length to be written, must be less than or equal to FLASH_PAGE_SIZE
*/
int8_t spi_flash_write
(
void		*pvWriteBuffer, 
uint32_t	u32FlashAddr, 
uint32_t	u32WriteSize
) ;

/**
* \brief Erase SPI Flash sector
*
* Erases SPI Flash Sector 
* \param[in]  u32FlashAdr  flash memory address within the sector to erase
*/
void spi_flash_sector_erase
(
uint32_t 	u32FlashAdr
);

/**
* \brief Erase SPI Flash sector
*
* Erases SPI Flash Sector 
* \param[in]  u32StartOffset  start address of the spi flash
* \param[in]  u32Size  				size of the spi flash
*/
uint8_t spi_flash_erase
(
uint32_t u32StartOffset,
uint32_t u32Size
);

/**
* \brief Enter SPI Flash low power mode
*
* Enter SPI Flash low power mode
*/
void spi_flash_enter_low_power_mode(void) ;


/**
* \brief Exit SPI Flash Low power mode
*
* Exit SPI Flash Low power mode
*/
void spi_flash_leave_low_power_mode(void);

/** @} */

#endif /* __SPI_FLASH_H__ */
