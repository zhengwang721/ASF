/**
*  @file		nm_spi_flash.h				
*  @brief		This module contains spi flash driver API
*  @author		M.S.M
*  @date		18 JULY 2012
*  @version		1.0	
*/

#ifndef __SPI_FLASH_h__
#define __SPI_FLASH_h__


#include "common\include\nm_common.h"

/* The SPI Serial EEPROM parameters */
#define SPI_EEPROM_ADDRESS_BYTES 3        /* number of bytes used for addressing within EEPROM */
#define SPI_EEPROM_PAGE_SIZE     0x0100   /* page size - maximum bytes per write */

/* The SPI FLASH instructions */
/*		Command							  Value		Description					     Addr Dummy Data	 */
#define SPI_EEPROM_WRITE_STATUS           0x01		 /* Write Status Register		 0   	0    1	     */
#define SPI_EEPROM_WRITE_DATA             0x02		 /* page program                 3      0    <=255   */
#define SPI_EEPROM_READ_DATA              0x0b		 /* High speed read				 3   	1    >=1     */
#define SPI_EEPROM_WRITE_LATCH_DISABLE    0x04		 /* Write Disable				 0   	0    0	     */
#define SPI_EEPROM_READ_STATUS            0x05		 /* Read Status Register		 0   	0    >=1     */
#define SPI_EEPROM_WRITE_LATCH_ENABLE     0x06	     /* Write Enable			     0		0    0       */
#define SPI_FLASH_CHIP_ERASE              0xC7		 /* Chip Erase				     0   	0    0	     */
#define	SPI_FLASH_CHIP_ID				  0x9f		 /* Read Identification			 0   	0    1-3     */

/* Funtion prototypes */
/*
*  @fn		void  nm_spi_flash_chip_erase(void)
*  @brief	Erase whole chip.
*/
void    nm_spi_flash_chip_erase(void);
/*
*  @fn		void nm_spi_flash_set_write_latch(boolean enable)
*  @brief	Write enable OR disable
*  @param [IN] enable
* 			1 enable or 0 disable  
*/
void    nm_spi_flash_set_write_latch(boolean enable);
/*
*  @fn		nm_spi_flash_set_protection(boolean protect)
*  @brief	enable protection or disable
*  @param [IN] enable
* 				1 enable or 0 disable 
*/
void    nm_spi_flash_set_protection(boolean protect);
/*
*  @fn		uint_8  nm_spi_flash_read_status(void)
*  @brief	Read status register 
*  @return  status register value  
*/
uint_8  nm_spi_flash_read_status(void);
/*
*  @fn		void    nm_spi_flash_write_byte(uint_32 addr, uchar data);
*  @brief	Write data to spi-flash using page programming mode
*  @param [IN] addr
* 				 memory address
*  @param [OUT] data
* 				Poitner to the read data.
*/
void    nm_spi_flash_write_byte(uint_32 addr, uchar data);
/*
*  @fn		uint_8  nm_spi_flash_read_byte(uint_32 addr)
*  @brief	Fast read data from M25P
*  @param [IN] addr
* 				 memory address
*  @return  value readed 
*/
uint_8  nm_spi_flash_read_byte(uint_32 addr);
/*
*  @fn		uint_32 nm_spi_flash_write_data(uint_32 addr, uint_32 size, uchar_ptr data);
*  @brief	Write data to spi-flash using page programming mode
*  @param [IN] addr
* 				 memory address
*  @param [OUT] data
* 				Poitner to the read data  buffer
*  @param [IN] size
* 				Data buffer size,max size 256 byte
    @return		
	The function shall return ZERO for successful operation and a negative value otherwise. 

*/
sint8 nm_spi_flash_write_data(uint_32 addr, uint_32 size, uchar_ptr data);
/*
*  @fn		uint_32 nm_spi_flash_read_data(uint_32 addr, uint_32 size, uchar_ptr data);
*  @brief	read data 
*  @param [IN] addr
* 				 memory address
*  @param [OUT] data
* 				Poitner to the read data  buffer
*  @param [IN] size
* 				Data buffer size,max size 256 byte
    @return 	   
   The function shall return ZERO for successful operation and a negative value otherwise. 
*/
sint8 nm_spi_flash_read_data(uint_32 addr, uint_32 size, uchar_ptr data);
/*
*  @fn		uint_8  nm_spi_flash_read_Chipid(void)
*  @brief	Read identification 
*/
void  nm_spi_flash_read_Chipid(void);
/*
*  @fn		sint8	nm_spi_flash_init(void)
*  @brief	Initialize SPI Flash
*/
sint8	nm_spi_flash_init(void);
/*
*  @fn		void	nm_spi_flash_deinit(void)
*  @brief	Initialize SPI Flash
    @return	The function shall return ZERO for successful operation and a negative value otherwise. 
*/
void	nm_spi_flash_deinit(void);

#endif
/* EOF */
