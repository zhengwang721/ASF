/**
 * \file
 *
 * \brief OTA Flash management
 *
 * Copyright (c) 2013-2014 Atmel Corporation. All rights reserved.
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
 
#ifdef OTA_FLASH

#include "ota_flash.h"
#include "otau_config.h"

static   ota_boot_req_t   boot_req;

/* The entry point is correct only for atmega1281 and atmega128rfa1. */
#define BOOT_ENTRY_POINT       (FLASHEND - 1ul)


void flash_write(uint32_t start_addr, uint32_t length, uint8_t *data)
{
    uint32_t remaining_len = length;
    uint8_t *ptr = data;

    do
    {
        uint16_t current_len;

        /* Fill page buffer */
        if (remaining_len > SPM_PAGESIZE)
        {
            current_len = SPM_PAGESIZE;
        }
        else
        {
            current_len = (uint16_t)remaining_len;
        }
        flash_fill_page_buffer(0x0000, current_len, ptr);
        ptr += SPM_PAGESIZE;
        remaining_len -= current_len;
        /* Erase and program flash page */
        flash_program_page(start_addr);
        start_addr += SPM_PAGESIZE;
    } while (remaining_len > 0);
}



void flash_fill_page_buffer(uint16_t start_offset, uint16_t length, uint8_t *data)
{

#if defined(__ICCAVR__)
    boot_entry_point_t boot_entry_point;
    memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
    boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
    //rf4ce_boot_req_t   boot_req;
    uint8_t gpior0;
    uint8_t gpior1;

    boot_req.request_type = FILL_PAGE_BUFFER;
    boot_req.common_boot_req.fill_page_buffer_req.data = data;
    boot_req.common_boot_req.fill_page_buffer_req.start_offset = start_offset;
    boot_req.common_boot_req.fill_page_buffer_req.length = length;

    gpior0 = GPIOR0;
    gpior1 = GPIOR1;

    GPIOR1 = ((uint16_t)&boot_req >> 8);
    GPIOR0 = (uint16_t)&boot_req & 0xFF;
    boot_entry_point();

    GPIOR0 = gpior0;
    GPIOR1 = gpior1;
}


void flash_program_page(uint32_t page_start_addr)
{
#if defined(__ICCAVR__)
    boot_entry_point_t boot_entry_point;
    memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
    boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
    //rf4ce_boot_req_t   boot_req;
    uint8_t gpior0;
    uint8_t gpior1;

    boot_req.request_type = PROGRAM_PAGE;
    boot_req.common_boot_req.program_page_req.page_start_addr = page_start_addr;

    gpior0 = GPIOR0;
    gpior1 = GPIOR1;

    GPIOR1 = ((uint16_t)&boot_req >> 8);
    GPIOR0 = (uint16_t)&boot_req & 0xFF;
    boot_entry_point();

    GPIOR0 = gpior0;
    GPIOR1 = gpior1;
}


void flash_swap(uint32_t image_start_addr, uint32_t image_size)
{
#if defined(__ICCAVR__)
    boot_entry_point_t boot_entry_point;
    memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
    boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
    //rf4ce_boot_req_t   boot_req;

    boot_req.request_type = SWAP_IMAGE;
    boot_req.common_boot_req.swap_images_req.image_size = image_size;
    boot_req.common_boot_req.swap_images_req.image_start_addr = image_start_addr;

    GPIOR1 = ((uint16_t)&boot_req >> 8);
    GPIOR0 = (uint16_t)&boot_req & 0xFF;
    boot_entry_point();
}

void flash_erase_page(uint32_t page_start_addr)
{
#if defined(__ICCAVR__)
    boot_entry_point_t boot_entry_point;
    memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
    boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
    //rf4ce_boot_req_t   boot_req;
    uint8_t gpior0;
    uint8_t gpior1;

    boot_req.request_type = ERASE_PAGE;
    boot_req.common_boot_req.erase_page_req.page_start_addr = page_start_addr;

    gpior0 = GPIOR0;
    gpior1 = GPIOR1;

    GPIOR1 = ((uint16_t)&boot_req >> 8);
    GPIOR0 = (uint16_t)&boot_req & 0xFF;
    boot_entry_point();

    GPIOR0 = gpior0;
    GPIOR1 = gpior1;
}

void flash_write_page(uint32_t page_start_addr)
{
#if defined(__ICCAVR__)
    boot_entry_point_t boot_entry_point;
    memcpy_P((void *)&boot_entry_point, (__farflash void *)BOOT_ENTRY_POINT, sizeof(void *));
#elif defined(__GNUC__)
    boot_entry_point_t boot_entry_point = (void *)pgm_read_word_far(BOOT_ENTRY_POINT);
#endif
    //rf4ce_boot_req_t   boot_req;
    uint8_t gpior0;
    uint8_t gpior1;

    boot_req.request_type = WRITE_PAGE;
    boot_req.common_boot_req.write_page_req.page_start_addr = page_start_addr;

    gpior0 = GPIOR0;
    gpior1 = GPIOR1;

    GPIOR1 = ((uint16_t)&boot_req >> 8);
    GPIOR0 = (uint16_t)&boot_req & 0xFF;
    boot_entry_point();

    GPIOR0 = gpior0;
    GPIOR1 = gpior1;
}

#endif  /* #ifdef OTA_FLASH */
