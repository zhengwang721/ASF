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

#ifndef _OTAFLASH_H
#define _OTAFLASH_H

/******************************************************************************
                   Types section
******************************************************************************/
/* The necessary list of functions ID for rf4ce */
typedef enum
{
  FILL_PAGE_BUFFER,
  PROGRAM_PAGE,
  SWAP_IMAGE,
  MAX_FUNCTION_NUMBER
} ota_boot_func_identity_t;

/* Parameters for filling page buffer */
typedef struct
{
  uint16_t start_offset;
  uint16_t length;
  uint8_t *data;
} fill_page_buffer_req_t;

/* Parameters for programming page */
typedef struct
{
  uint32_t page_start_addr;
} program_page_req_t;

/* Parameters for swapping image */
typedef struct
{
  uint32_t image_start_addr;
  uint32_t image_size;
} swap_images_req_t;

/* Common memory for all requests */
typedef union
{
  fill_page_buffer_req_t fill_page_buffer_req;
  program_page_req_t     program_page_req;
  swap_images_req_t      swap_images_req;
} common_boot_req_t;

/* Request structure for boot functionality */
typedef struct
{
  ota_boot_func_identity_t request_type;
  common_boot_req_t     common_boot_req;
} ota_boot_req_t;

/* Prototype of the entry point to bootloader. */
typedef void (* boot_entry_point_t)(void);

void pal_flash_write(uint32_t start_addr, uint32_t length, uint8_t *data);
void flash_fill_page_buffer(uint16_t start_offset, uint16_t length, uint8_t *data);
void flash_program_page(uint32_t page_start_addr);
void flash_swap(uint32_t image_start_addr, uint32_t image_size);
void flash_erase_page(uint32_t page_start_addr);
void flash_write_page(uint32_t page_start_addr);


#endif /* _OTAFLASH_H */