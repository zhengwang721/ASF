/**
 * \file
 *
 * \brief SAMD20 EEPROM Emulator
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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
#ifndef EEPROM_EMULATOR_H_INCLUDED
#define EEPROM_EMULATOR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>

#define EEPROM_DATA_SIZE         60
#define EEPROM_MAX_PAGES         124


enum status_code eeprom_emulator_init(void);

enum status_code eeprom_emulator_write_page(
		uint8_t lpage,
		uint8_t *data);


enum status_code eeprom_emulator_read_page(
		uint8_t lpage,
		uint8_t *data);

enum status_code eeprom_emulator_flush_page_buffer(void);

void eeprom_emulator_erase_memory(void);

enum status_code eeprom_emulator_write_buffer(
		uint16_t offset,
		uint8_t *data,
		uint16_t lenght);

enum status_code eeprom_emulator_read_buffer(
		uint16_t offset,
		uint8_t *data,
		uint16_t length);

#ifdef __cplusplus
}
#endif

#endif
