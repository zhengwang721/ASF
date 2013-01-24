/**
 * \file
 *
 * \brief MEGA FLASH DRIVER
 *
 * Copyright (c) 2010-2012 Atmel Corporation. All rights reserved.
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

/* Prevent double inclusion */
#ifndef FLASH_H
#define FLASH_H

#if defined(__ICCAVR__)

#define PGM_READ_BYTE_FAR(x) __extended_load_program_memory((const unsigned char __farflash *)x)

#define SPM_BUSY_WAIT() do{}while((SPMCSR & (uint8_t)(1 << (SPMEN))))
#define ENABLE_RWW_SECTION() __DataToR0ByteToSPMCR_SPM(0x00, 0x11)
#define EEPROM_BUSY_WAIT() do{}while(EECR & (1 << EEPE))

/* Flash write abstraction */
#define FLASH_ERASE(addr)  _SPM_24_ERASE(addr)
#define FLASH_PAGE_FILL(addr, data)  _SPM_24_FILLTEMP(addr, data)
#define FLASH_PAGE_WRITE(addr)  _SPM_24_PAGEWRITE(addr)


#endif

#if defined(__GNUC__)

#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#define PGM_READ_BYTE_FAR(x) pgm_read_byte_far(x)

#define SPM_BUSY_WAIT() boot_spm_busy_wait()
#define ENABLE_RWW_SECTION() boot_rww_enable()
#define EEPROM_BUSY_WAIT() eeprom_busy_wait()

#define FLASH_ERASE(addr)  boot_page_erase(addr)
#define FLASH_PAGE_FILL(addr, data)  boot_page_fill(addr, data)
#define FLASH_PAGE_WRITE(addr)  boot_page_write(addr)

#endif

void flash_write(uint32_t , uint32_t , uint8_t*);

void flash_read(uint32_t , uint32_t  ,uint8_t*);


#ifdef __ICCAVR__
#pragma location = "bootloader"
void flash_erase_page(uint32_t);
#endif
#ifdef __GNUC__
void flash_erase_page(uint32_t )
    __attribute__((section(".bootloader"),noinline));
#endif


#ifdef __ICCAVR__
#pragma location = "bootloader"
void flash_write_page(uint32_t );
#endif
#ifdef __GNUC__
void flash_write_page(uint32_t )
    __attribute__((section(".bootloader"),noinline));
#endif


#ifdef __ICCAVR__
#pragma location = "bootloader"
void flash_fill_page_buffer(uint32_t , uint16_t , uint8_t* );
#endif
#ifdef __GNUC__
void flash_fill_page_buffer(uint32_t , uint16_t , uint8_t*)
    __attribute__((section(".bootloader"),noinline));
#endif


 
#ifdef __ICCAVR__
#pragma location = "bootloader"
void flash_program_page(uint32_t ) ;
#endif
#ifdef __GNUC__
void flash_program_page(uint32_t )
    __attribute__((section(".bootloader"),noinline));
#endif



#if 	AVR8_PART_IS_DEFINED(ATmega128RFA1) | \
		AVR8_PART_IS_DEFINED(ATmega256RFR2)

#define FLASH_PAGE_SIZE      (SPM_PAGESIZE)
#define FLASH_SIZE (FLASHEND+1)

#endif


#endif  /* #ifndef FLASH_H */
