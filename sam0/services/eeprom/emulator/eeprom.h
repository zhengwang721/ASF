/**
 * \file
 *
 * \brief SAM0+ Peripheral Digital to Analog Converter Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#include "asf.h"

#ifndef _EEPROM_EMULATOR_H_
#define _EEPROM_EMULATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EEPROM_EMUALTOR_START_ADDRESS  ????

/**
 * \brief Initializes the eeprom emualtor service driver
 *
 * This function will check the EEPROM emualtor space, if there is already 
 * initialized eeprom data in the EEPROM space it will
 *
 * \li Re-fresh all used row to initial state
 * \li Re-map data
 *
 * If the EEPROM space is unused it will:
 *
 * \li Re-fresh all rows to make sure the row state is known
 * \li Initialize the EEPROM space 
 *
 * \return This function return a status code
 *
 */
enum status_code eeprom_emulator_init(void);

enum status_code eeprom_emulator_enable();
enum status_code eeprom_emulator_disable();

/**
 * \brief Writes a data buffer to the flash page buffer
 *
 * This function will write data to the flash page buffer, the data will be keept 
 * there until another write is executed or the flush function is used.
 *
 * \param[in] page  The number of the page in eeprom emualtor space to be written
 * \param[in] *data Pointer to the data buffer containing data to be written
 *
 * \return This function return a status code
 *
 */
enum status_code eeprom_write_page(uint16_t page, uint8_t *data);

/**
 * \brief Reads a page sized data buffer from EEPROM emulator flash space
 *
 * This function will read a pagesized buffer from the EEPROM emualtor space and
 * place it in the SRAM buffer. 
 *
 * \param[in] page The number of the page in eeprom emualtor space to be written
 * \param[out] *data Data buffer of page size that will contain the data read from memory
 */
enum status_code eeprom_read_page(uint16_t page, uint8_t *data);

/**
 * \brief Flushes the page buffer to flash memory
 *
 * This function will flush the content of the page buffer to the correct location
 * in EEPROM emualtor memory space
 *
 * \note This should be the first function executed in a BOD33 interupt routine
 * \note If the nvm driver is used to write other location in flash memory this
 *       this function has to be executed first
 *
 * \return This function may return some status code of unknown value
 */
enum status_code eeprom_flush_page_buffer() 

/**
 * \brief This function will initalize the EEPROM memory bank i flash
 *
 * This function fill erase the whole EEPROM memory block in flash. This includes
 * erase all rows, write EEPROM memory header, and create new EEPROM pages.
 *
 * \note This function will destroy any content in the alotted EEPROM memory block.
 *
 * \return This function may return some status code of unknown value
 */
enum status_code eeprom_erase_memory():

#ifdef __cplusplus
}
#endif

#endif
