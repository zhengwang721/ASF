/**
 * \file
 *
 * \brief SAM0+ EEPROM Emulator Service Quick Start
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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
#include <asf.h>

int main(void)
{


//! [main]
//! [variable]
	uint8_t write_buffer[EEPROM_DATA_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
	uint8_t read_buffer[EEPROM_DATA_SIZE];
//! [variable]

	/* Setup EEPROM emulator service*/
//! [init_eeprom_emulator]
	if (eeprom_emulator_init() != STATUS_OK) {
		eeprom_emulator_erase_memory();
		eeprom_emulator_init();
	}
//! [init_eeprom_emulator]

	/* Write data to page 0 */
//! [write_data]
	eeprom_emulator_write_page(0, write_buffer);
//! [write_data]

//! [read_data]
	eeprom_emulator_read_page(0, read_buffer);
//! [read_data]

//! [write_back_to_page_1]
	eeprom_emulator_write_page(1, read_buffer);
//! [write_back_to_page_1]

//! [flush_cache]
	eeprom_emulator_flush_page_buffer();
//! [flush_cache]

//! [erase_memory]
	eeprom_emulator_erase_memory();
//! [erase_memory]

//! [inf_loop]
	while (1) {
	}
//! [inf_loop]
//! [main]
}
