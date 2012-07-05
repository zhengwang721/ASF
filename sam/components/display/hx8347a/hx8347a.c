/**
 * \file
 *
 * \brief API driver for HX8347A TFT display component.
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
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

/**
 * \defgroup hx8347a_display_group COMPONENT - Display - HX8347A Controller
 *
 * Low-level driver for the HX8347A LCD controller. This driver provides access to the main
 * features of the HX8347A controller.
 *
 * \{
 */

#include "hx8347a.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
 extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/* Pixel cache used to speed up communication */
#define LCD_DATA_CACHE_SIZE HX8347A_LCD_WIDTH
static hx8347a_color_t g_ul_pixel_cache[LCD_DATA_CACHE_SIZE];

/* Global variable describing the font size used by the driver */
const struct font gfont = {10, 14};
/**
 * Character set table for font 10x14 
 * Coding format: 
 * Char height is 14 bits, which is coded using 2 bytes per column (2 unused bits). 
 * Char width is 10 bits.
 */
const uint8_t p_uc_charset10x14[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCC,
	0xFF, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0xC0, 0x0C, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x0C, 0xC0,
	0x0C, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0x60, 0x1E, 0x70, 0x3F, 0x30, 0x33, 0x30, 0xFF, 0xFC,
	0xFF, 0xFC, 0x33, 0x30, 0x33, 0xF0, 0x39, 0xE0, 0x18, 0xC0,
	0x60, 0x00, 0xF0, 0x0C, 0xF0, 0x3C, 0x60, 0xF0, 0x03, 0xC0,
	0x0F, 0x00, 0x3C, 0x18, 0xF0, 0x3C, 0xC0, 0x3C, 0x00, 0x18,
	0x3C, 0xF0, 0x7F, 0xF8, 0xC3, 0x1C, 0xC7, 0x8C, 0xCF, 0xCC,
	0xDC, 0xEC, 0x78, 0x78, 0x30, 0x30, 0x00, 0xFC, 0x00, 0xCC,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xEC, 0x00,
	0xF8, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x3F, 0xF0, 0x78, 0x78,
	0x60, 0x18, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0x60, 0x18,
	0x78, 0x78, 0x3F, 0xF0, 0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0x60, 0x0E, 0xE0, 0x07, 0xC0, 0x03, 0x80, 0x3F, 0xF8,
	0x3F, 0xF8, 0x03, 0x80, 0x07, 0xC0, 0x0E, 0xE0, 0x0C, 0x60,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x3F, 0xF0,
	0x3F, 0xF0, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x00, 0x44, 0x00, 0xEC, 0x00, 0xF8, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x00, 0x18, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0C, 0x00, 0x3C, 0x00, 0xF0, 0x03, 0xC0,
	0x0F, 0x00, 0x3C, 0x00, 0xF0, 0x00, 0xC0, 0x00, 0x00, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0xFC, 0xC1, 0xCC, 0xC3, 0x8C,
	0xC7, 0x0C, 0xCE, 0x0C, 0xFC, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x70, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x0C, 0x70, 0x1C, 0xE0, 0x3C, 0xC0, 0x7C, 0xC0, 0xEC,
	0xC1, 0xCC, 0xC3, 0x8C, 0xE7, 0x0C, 0x7E, 0x0C, 0x3C, 0x0C,
	0x30, 0x30, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x1C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x03, 0xC0, 0x07, 0xC0, 0x0E, 0xC0, 0x1C, 0xC0, 0x38, 0xC0,
	0x70, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0xC0, 0x00, 0xC0,
	0xFC, 0x30, 0xFC, 0x38, 0xCC, 0x1C, 0xCC, 0x0C, 0xCC, 0x0C,
	0xCC, 0x0C, 0xCC, 0x0C, 0xCE, 0x1C, 0xC7, 0xF8, 0xC3, 0xF0,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE3, 0x1C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x9C, 0x71, 0xF8, 0x30, 0xF0,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC3, 0xFC,
	0xC7, 0xFC, 0xCE, 0x00, 0xDC, 0x00, 0xF8, 0x00, 0xF0, 0x00,
	0x3C, 0xF0, 0x7F, 0xF8, 0xE7, 0x9C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE7, 0x9C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x3C, 0x00, 0x7E, 0x00, 0xE7, 0x0C, 0xC3, 0x0C, 0xC3, 0x1C,
	0xC3, 0x38, 0xC3, 0x70, 0xE7, 0xE0, 0x7F, 0xC0, 0x3F, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x3C, 0xF0,
	0x3C, 0xF0, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x44, 0x3C, 0xEC,
	0x3C, 0xF8, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x00, 0x07, 0x80, 0x0F, 0xC0, 0x1C, 0xE0,
	0x38, 0x70, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C, 0x00, 0x00,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x00, 0x00, 0xC0, 0x0C, 0xE0, 0x1C, 0x70, 0x38, 0x38, 0x70,
	0x1C, 0xE0, 0x0F, 0xC0, 0x07, 0x80, 0x03, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x70, 0x00, 0xE0, 0x00, 0xC0, 0x00, 0xC1, 0xEC,
	0xC3, 0xEC, 0xC3, 0x00, 0xE6, 0x00, 0x7E, 0x00, 0x3C, 0x00,
	0x30, 0xF0, 0x71, 0xF8, 0xE3, 0x9C, 0xC3, 0x0C, 0xC3, 0xFC,
	0xC3, 0xFC, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0x3F, 0xFC, 0x7F, 0xFC, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xC0, 0x7F, 0xFC, 0x3F, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE7, 0x9C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x70, 0x38, 0x30, 0x30,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
	0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x1C, 0x73, 0xF8, 0x33, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFF, 0xFC, 0xFF, 0xFC,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x00, 0x38, 0xC0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xFC, 0xFF, 0xFC, 0x07, 0x80, 0x07, 0x80, 0x0F, 0xC0,
	0x1C, 0xE0, 0x38, 0x70, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x70, 0x00, 0x38, 0x00, 0x1F, 0x00,
	0x1F, 0x00, 0x38, 0x00, 0x70, 0x00, 0xFF, 0xFC, 0xFF, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0x1C, 0x00, 0x0E, 0x00, 0x07, 0x00,
	0x03, 0x80, 0x01, 0xC0, 0x00, 0xE0, 0xFF, 0xFC, 0xFF, 0xFC,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
	0xC3, 0x00, 0xC3, 0x00, 0xE7, 0x00, 0x7E, 0x00, 0x3C, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0xCC,
	0xC0, 0xEC, 0xC0, 0x7C, 0xE0, 0x38, 0x7F, 0xFC, 0x3F, 0xEC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x80, 0xC3, 0x80,
	0xC3, 0xC0, 0xC3, 0xC0, 0xE7, 0x70, 0x7E, 0x3C, 0x3C, 0x1C,
	0x3C, 0x18, 0x7E, 0x1C, 0xE7, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x9C, 0xE1, 0xF8, 0x60, 0xF0,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0xFC,
	0xFF, 0xFC, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xF0, 0xFF, 0xF8, 0x00, 0x1C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0,
	0xFF, 0xC0, 0xFF, 0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C,
	0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0xFF, 0xE0, 0xFF, 0xC0,
	0xFF, 0xF0, 0xFF, 0xF8, 0x00, 0x1C, 0x00, 0x3C, 0x00, 0xF8,
	0x00, 0xF8, 0x00, 0x3C, 0x00, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0,
	0xF0, 0x3C, 0xF8, 0x7C, 0x1C, 0xE0, 0x0F, 0xC0, 0x07, 0x80,
	0x07, 0x80, 0x0F, 0xC0, 0x1C, 0xE0, 0xF8, 0x7C, 0xF0, 0x3C,
	0xFC, 0x00, 0xFE, 0x00, 0x07, 0x00, 0x03, 0x80, 0x01, 0xFC,
	0x01, 0xFC, 0x03, 0x80, 0x07, 0x00, 0xFE, 0x00, 0xFC, 0x00,
	0xC0, 0x3C, 0xC0, 0x7C, 0xC0, 0xEC, 0xC1, 0xCC, 0xC3, 0x8C,
	0xC7, 0x0C, 0xCE, 0x0C, 0xDC, 0x0C, 0xF8, 0x0C, 0xF0, 0x0C,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFC, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x30, 0x00, 0x30,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0x00, 0xE0, 0x00,
	0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C, 0x00, 0x0C, 0x00,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xE0, 0x00, 0x70, 0x00,
	0x38, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x06, 0x78, 0x0E, 0xFC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0E, 0xCC, 0x07, 0xFC, 0x03, 0xF8,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x0C, 0x03, 0x0C, 0x03, 0x0C,
	0x03, 0x0C, 0x03, 0x0C, 0x03, 0x9C, 0x01, 0xF8, 0x00, 0xF0,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0x38, 0x03, 0x30,
	0x00, 0xF0, 0x01, 0xF8, 0x03, 0x9C, 0x03, 0x0C, 0x03, 0x0C,
	0x03, 0x0C, 0x03, 0x0C, 0x03, 0x0C, 0xFF, 0xFC, 0xFF, 0xFC,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0xDC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0E, 0xDC, 0x07, 0xD8, 0x03, 0x90,
	0x00, 0x00, 0x03, 0x00, 0x3F, 0xFC, 0x7F, 0xFC, 0xE3, 0x00,
	0xE3, 0x00, 0x70, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x18, 0x07, 0x9C, 0x0F, 0xCC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0C, 0xDC, 0x0F, 0xF8, 0x07, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x80, 0x01, 0xFC, 0x00, 0xFC, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0xFC,
	0x1B, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x00, 0x38, 0x00, 0x1C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x1C, 0xCF, 0xF8, 0xCF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0xE0, 0x01, 0xE0,
	0x03, 0xF0, 0x07, 0x38, 0x0E, 0x1C, 0x0C, 0x0C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x0F, 0xFC, 0x0F, 0xFC, 0x0E, 0x00, 0x07, 0x00, 0x03, 0xC0,
	0x03, 0xC0, 0x07, 0x00, 0x0E, 0x00, 0x0F, 0xFC, 0x0F, 0xFC,
	0x0F, 0xFC, 0x0F, 0xFC, 0x03, 0x00, 0x07, 0x00, 0x0E, 0x00,
	0x0C, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x07, 0xFC, 0x03, 0xFC,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0xF8, 0x03, 0xF0,
	0x0F, 0xFC, 0x0F, 0xFC, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0F, 0xC0, 0x07, 0x80, 0x03, 0x00,
	0x03, 0x00, 0x07, 0x80, 0x0F, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0F, 0xFC, 0x0F, 0xFC,
	0x0F, 0xFC, 0x0F, 0xFC, 0x03, 0x80, 0x07, 0x00, 0x0E, 0x00,
	0x0C, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x07, 0x00, 0x03, 0x00,
	0x03, 0x18, 0x07, 0x9C, 0x0F, 0xCC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0C, 0xFC, 0x0E, 0x78, 0x06, 0x30,
	0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0xFF, 0xF0, 0xFF, 0xF8,
	0x0C, 0x1C, 0x0C, 0x1C, 0x0C, 0x38, 0x0C, 0x30, 0x00, 0x00,
	0x0F, 0xF0, 0x0F, 0xF8, 0x00, 0x1C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1C, 0x0F, 0xF8, 0x0F, 0xF0,
	0x0F, 0xC0, 0x0F, 0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C,
	0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0x0F, 0xE0, 0x0F, 0xC0,
	0x0F, 0xF0, 0x0F, 0xF8, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0xF8,
	0x00, 0xF8, 0x00, 0x1C, 0x00, 0x1C, 0x0F, 0xF8, 0x0F, 0xF0,
	0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0x38, 0x03, 0xF0, 0x01, 0xE0,
	0x01, 0xE0, 0x03, 0xF0, 0x07, 0x38, 0x0E, 0x1C, 0x0C, 0x0C,
	0x0C, 0x00, 0x0E, 0x00, 0x07, 0x0C, 0x03, 0x9C, 0x01, 0xF8,
	0x01, 0xF0, 0x03, 0x80, 0x07, 0x00, 0x0E, 0x00, 0x0C, 0x00,
	0x0C, 0x0C, 0x0C, 0x1C, 0x0C, 0x3C, 0x0C, 0x7C, 0x0C, 0xEC,
	0x0D, 0xCC, 0x0F, 0x8C, 0x0F, 0x0C, 0x0E, 0x0C, 0x0C, 0x0C,
	0x00, 0x00, 0x03, 0x00, 0x07, 0x80, 0x3F, 0xF0, 0x7C, 0xF8,
	0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00,
	0x03, 0x0C, 0x03, 0x0C, 0x3F, 0xFC, 0x7F, 0xFC, 0xE3, 0x0C,
	0xC3, 0x0C, 0xC0, 0x0C, 0xE0, 0x0C, 0x70, 0x0C, 0x30, 0x0C,
	0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C,
	0x7C, 0xF8, 0x3F, 0xF0, 0x07, 0x80, 0x03, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC
};

/**
 * \brief Prepare to write GRAM data.
 */
static void hx8347a_write_ram_prepare(void)
{
	LCD_IR(HX8347A_SRAM_WRITE_CTRL);
}

/**
 * \brief Write data to LCD GRAM.
 *
 * \param us_color 16-bits RGB color.
 */
static void hx8347a_write_ram(hx8347a_color_t us_color)
{
	LCD_WD(us_color);
}

/**
 * \brief Write multiple data in buffer to LCD controller.
 *
 * \param p_ul_buf data buffer.
 * \param ul_size size in pixels.
 */
static void hx8347a_write_ram_buffer(const hx8347a_color_t *p_ul_buf, uint32_t ul_size)
{
	uint32_t addr;

	for (addr = 0; addr < (ul_size - ul_size % 8); addr += 8) {
		hx8347a_write_ram(p_ul_buf[addr]);
		hx8347a_write_ram(p_ul_buf[addr + 1]);
		hx8347a_write_ram(p_ul_buf[addr + 2]);
		hx8347a_write_ram(p_ul_buf[addr + 3]);
		hx8347a_write_ram(p_ul_buf[addr + 4]);
		hx8347a_write_ram(p_ul_buf[addr + 5]);
		hx8347a_write_ram(p_ul_buf[addr + 6]);
		hx8347a_write_ram(p_ul_buf[addr + 7]);
	}
	for (; addr < ul_size; addr++) {
		hx8347a_write_ram(p_ul_buf[addr]);
	}
}

/**
 * \brief Write data to LCD Register.
 *
 * \param uc_reg register address.
 * \param us_data data to be written.
 */
static void hx8347a_write_register(uint8_t uc_reg, uint16_t us_data)
{
	LCD_IR(uc_reg);
	LCD_WD(us_data);
}

/**
 * \brief Prepare to read GRAM data.
 */
static void hx8347a_read_ram_prepare(void)
{
	LCD_IR(HX8347A_SRAM_WRITE_CTRL);
}

/**
 * \brief Read data to LCD GRAM.
 *
 * \note Because pixel data LCD GRAM is 18-bits, so conversion to RGB 24-bits
 * will cause lost low color bits.
 *
 * \return 16-bits RGB color.
 */
static hx8347a_color_t hx8347a_read_ram(void)
{
	return (hx8347a_color_t) LCD_RD();
}

/**
 * \brief Read data from LCD Register.
 *
 * \param uc_reg register address.
 *
 * \return register value.
 */
static uint16_t hx8347a_read_register(uint8_t uc_reg)
{
	LCD_IR(uc_reg);
	return LCD_RD();
}

/**
 * \brief Delay function.
 */
static void hx8347a_delay(uint32_t ul_ms)
{
	volatile uint32_t i;

	for(i = 0; i < ul_ms; i++) {
		for(i = 0; i < 100000; i++) {
		}
	}
}

/**
 * \brief Check box coordinates.
 *
 * \param p_ul_x1 X coordinate of upper-left corner on LCD.
 * \param p_ul_y1 Y coordinate of upper-left corner on LCD.
 * \param p_ul_x2 X coordinate of lower-right corner on LCD.
 * \param p_ul_y2 Y coordinate of lower-right corner on LCD.
 */
static void hx8347a_check_box_coordinates(uint32_t *p_ul_x1, uint32_t *p_ul_y1,
		uint32_t *p_ul_x2, uint32_t *p_ul_y2)
{
	uint32_t ul;

	if (*p_ul_x1 >= HX8347A_LCD_WIDTH) {
		*p_ul_x1 = HX8347A_LCD_WIDTH - 1;
	}

	if (*p_ul_x2 >= HX8347A_LCD_WIDTH) {
		*p_ul_x2 = HX8347A_LCD_WIDTH - 1;
	}

	if (*p_ul_y1 >= HX8347A_LCD_HEIGHT) {
		*p_ul_y1 = HX8347A_LCD_HEIGHT - 1;
	}

	if (*p_ul_y2 >= HX8347A_LCD_HEIGHT) {
		*p_ul_y2 = HX8347A_LCD_HEIGHT - 1;
	}

	if (*p_ul_x1 > *p_ul_x2) {
		ul = *p_ul_x1;
		*p_ul_x1 = *p_ul_x2;
		*p_ul_x2 = ul;
	}

	if (*p_ul_y1 > *p_ul_y2) {
		ul = *p_ul_y1;
		*p_ul_y1 = *p_ul_y2;
		*p_ul_y2 = ul;
	}
}

/**
 * \brief Initialize the HX8347A lcd driver.
 *
 * \note Make sure below works have been done before calling hx8347a_init()\n
 * 1. HX8347A related Pins have been initialized correctly. \n
 * 2. SMC has been configured correctly for access HX8347A (16-bit system interface for now). \n
 *
 * \param p_opt pointer to HX8347A option structure.
 *
 * \return 0 if initialization succeeds, otherwise fails.
 */
uint32_t hx8347a_init(struct hx8347a_opt_t *p_opt)
{
	/* Check HX8347A chipid */
	uint16_t chipid = hx8347a_read_register(HX8347A_HIMAX_ID_CODE); /* Driver Code Read (R67h) */
	if (chipid != HX8347A_DEVICE_CODE) {
		return 1;
	}

	/* Start internal OSC */
	/* OSCADJ=10 0000, OSD_EN=1 60Hz */
	hx8347a_write_register(HX8347A_OSC_CTRL1, HX8347A_OSC_CTRL1_CADJ(0x4) |
			HX8347A_OSC_CTRL1_CUADJ(0x4) | HX8347A_OSC_CTRL1_OSC_EN);
	/* RADJ=1100 */
	hx8347a_write_register(HX8347A_OSC_CTRL3, HX8347A_OSC_CTRL3_RADJ(0x0C));

	/* Power on flow */
	/* VCM=100 1101 */
	hx8347a_write_register(HX8347A_VCOM_CTRL2, HX8347A_VCOM_CTRL2_VCM(0x4D));
	/* VDV=1 0001 */
	hx8347a_write_register(HX8347A_VCOM_CTRL3, HX8347A_VCOM_CTRL3_VDV(0x11));
	/* BT=0100 */
	hx8347a_write_register(HX8347A_POWER_CTRL6, HX8347A_POWER_CTRL6_BT(0x4));
	/* VC1=111 */
	hx8347a_write_register(HX8347A_POWER_CTRL3, HX8347A_POWER_CTRL3_VC1(0x07));
	/* VC3=000 */
	hx8347a_write_register(HX8347A_POWER_CTRL4, HX8347A_POWER_CTRL4_VC3(0x00));
	/* VRH=0100 */
	hx8347a_write_register(HX8347A_POWER_CTRL5, HX8347A_POWER_CTRL5_VRH(0x04));

	/* AP=100 */
	hx8347a_write_register(HX8347A_POWER_CTRL2, HX8347A_POWER_CTRL2_AP(0x04));
	/* GASENB=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0 */
	hx8347a_write_register(HX8347A_POWER_CTRL1, HX8347A_POWER_CTRL1_PON);
	hx8347a_delay(50);

	/* Set VCOMG=1 */
	hx8347a_write_register(HX8347A_VCOM_CTRL1, HX8347A_VCOM_CTRL1_VCOMG);
	hx8347a_delay(50);

	/* Gamma for CMO 2.8 */
	hx8347a_write_register(HX8347A_R_CTRL1, HX8347A_R_CTRL1_GSEL |
			HX8347A_R_CTRL1_CP10 | HX8347A_R_CTRL1_CP02 | HX8347A_R_CTRL1_CP00);
	hx8347a_write_register(HX8347A_R_CTRL2, HX8347A_R_CTRL2_CN12 |
			HX8347A_R_CTRL2_CN10 | HX8347A_R_CTRL2_CN00);
	hx8347a_write_register(HX8347A_R_CTRL3, 0x00);
	hx8347a_write_register(HX8347A_R_CTRL4, HX8347A_R_CTRL4_NP31 |
			HX8347A_R_CTRL4_NP30 | HX8347A_R_CTRL4_NP22 | HX8347A_R_CTRL4_NP21);
	hx8347a_write_register(HX8347A_R_CTRL5, HX8347A_R_CTRL5_NP50 |
			HX8347A_R_CTRL5_NP40);
	hx8347a_write_register(HX8347A_R_CTRL6, HX8347A_R_CTRL6_NN12 |
			HX8347A_R_CTRL6_NN11 | HX8347A_R_CTRL6_NN02 | HX8347A_R_CTRL6_NN01);
	hx8347a_write_register(HX8347A_R_CTRL7, HX8347A_R_CTRL7_NN30 |
			HX8347A_R_CTRL7_NN22);
	hx8347a_write_register(HX8347A_R_CTRL8, HX8347A_R_CTRL8_NN52 |
			HX8347A_R_CTRL8_NN51 | HX8347A_R_CTRL8_NN50 |
			HX8347A_R_CTRL8_NN42 | HX8347A_R_CTRL8_NN41 | HX8347A_R_CTRL8_NN40);
	hx8347a_write_register(HX8347A_R_CTRL9, HX8347A_R_CTRL9_CGMP00 |
			HX8347A_R_CTRL9_OP01 | HX8347A_R_CTRL9_OP00);
	hx8347a_write_register(HX8347A_R_CTRL10, HX8347A_R_CTRL10_CGMP2 | 
			HX8347A_R_CTRL10_OP13 | HX8347A_R_CTRL10_OP12);
	hx8347a_write_register(HX8347A_R_CTRL11, HX8347A_R_CTRL11_CGMN10 |
			HX8347A_R_CTRL11_ON02 | HX8347A_R_CTRL11_ON01);
	hx8347a_write_register(HX8347A_R_CTRL12, HX8347A_R_CTRL12_CGMN2 |
			HX8347A_R_CTRL12_ON12 | HX8347A_R_CTRL12_ON11);

	/* 240x320 window setting */
	/* Column address start2 */
	hx8347a_write_register(HX8347A_COL_ADDR_START2,
			HX8347A_COL_ADDR_START2_SC(0x00));
	/* Column address start1 */
	hx8347a_write_register(HX8347A_COL_ADDR_START1,
			HX8347A_COL_ADDR_START1_SC(0x00));
	/* Column address end2 */
	hx8347a_write_register(HX8347A_COL_ADDR_END2,
			HX8347A_COL_ADDR_END2_EC(0x00));
	/* Column address end1 */
	hx8347a_write_register(HX8347A_COL_ADDR_END1,
			HX8347A_COL_ADDR_END1_EC(0xEF));
	/* Row address start2 */
	hx8347a_write_register(HX8347A_ROW_ADDR_START2,
			HX8347A_ROW_ADDR_START2_SP(0x00));
	/* Row address start1 */
	hx8347a_write_register(HX8347A_ROW_ADDR_START1,
			HX8347A_ROW_ADDR_START1_SP(0x00));
	/* Row address end2 */
	hx8347a_write_register(HX8347A_ROW_ADDR_END2,
			HX8347A_ROW_ADDR_END2_EP(0x01));
	/* Row address end1 */
	hx8347a_write_register(HX8347A_ROW_ADDR_END1,
			HX8347A_ROW_ADDR_END1_EP(0x3F));

	/*  Display Setting */
	/* IDMON=0, INVON=1, NORON=1, PTLON=0 */
	hx8347a_write_register(HX8347A_DISP_MODE_CTRL,
			HX8347A_DISP_MODE_CTRL_INVON |
			HX8347A_DISP_MODE_CTRL_NORON);
	/* MY=1, MX=1, MV=0, BGR=1 */
	hx8347a_write_register(HX8347A_MEMORY_ACCESS_CTRL,
			HX8347A_MEMORY_ACCESS_CTRL_MY |
			HX8347A_MEMORY_ACCESS_CTRL_MX|
			HX8347A_MEMORY_ACCESS_CTRL_BGR);
	/* N_DC=1001 0101 */
	hx8347a_write_register(HX8347A_CYCLE_CTRL_1,
			HX8347A_CYCLE_CTRL_1_N_DC(0x95));
	/* P_DC=1001 0101 */
	hx8347a_write_register(HX8347A_CYCLE_CTRL_2,
			HX8347A_CYCLE_CTRL_2_PI_DC(0x95));
	/* I_DC=1111 1111 */
	hx8347a_write_register(HX8347A_CYCLE_CTRL_3,
			HX8347A_CYCLE_CTRL_3_I_DC(0xFF));
	/* N_BP=0000 0110 */
	hx8347a_write_register(HX8347A_DISP_CTRL2,
			HX8347A_DISP_CTRL2_N_BP(0x06));
	/* N_FP=0000 0110 */
	hx8347a_write_register(HX8347A_DISP_CTRL3,
			HX8347A_DISP_CTRL3_N_FP(0x06));
	/* P_BP=0000 0110 */
	hx8347a_write_register(HX8347A_DISP_CTRL4,
			HX8347A_DISP_CTRL4_PI_BP(0x06));
	/* P_FP=0000 0110 */
	hx8347a_write_register(HX8347A_DISP_CTRL5,
			HX8347A_DISP_CTRL5_PI_FP(0x06));
	/* I_BP=0000 0110 */
	hx8347a_write_register(HX8347A_DISP_CTRL6,
			HX8347A_DISP_CTRL6_I_BP(0x06));
	/* I_FP=0000 0110 */
	hx8347a_write_register(HX8347A_DISP_CTRL7,
			HX8347A_DISP_CTRL7_I_FP(0x06));
	/* N_RTN=0000, N_NW=001 */
	hx8347a_write_register(HX8347A_CYCLE_CTRL1,
			HX8347A_CYCLE_CTRL1_N_NW(0x01));
	/* P_RTN=0000, P_NW=001 */
	hx8347a_write_register(HX8347A_CYCLE_CTRL2,
			HX8347A_CYCLE_CTRL2_PI_NW(0x01));
	/* I_RTN=1111, I_NW=000 */
	hx8347a_write_register(HX8347A_CYCLE_CTRL3,
			HX8347A_CYCLE_CTRL3_I_RTN(0xF));
	/* DIV=00 */
	hx8347a_write_register(HX8347A_CYCLE_CTRL4, 0x00);
	/* SON=38h */
	hx8347a_write_register(HX8347A_CYCLE_CTRL5,
			HX8347A_CYCLE_CTRL5_SON(0x38));
	/* GDON=0Fh */
	hx8347a_write_register(HX8347A_CYCLE_CTRL6,
			HX8347A_CYCLE_CTRL6_GDON(0x0F));
	/* GDOF=F0h */
	hx8347a_write_register(HX8347A_CYCLE_CTRL7,
			HX8347A_CYCLE_CTRL7_GDOF(0xF0));

	hx8347a_set_window(0, 0, p_opt->ul_width, p_opt->ul_height);
	hx8347a_set_foreground_color(p_opt->foreground_color);
	hx8347a_set_cursor_position(0, 0);
	return 0;
}

/**
 * \brief Turn on the LCD.
 */
void hx8347a_display_on(void)
{
	/* SAP=0111 1111 */
	hx8347a_write_register(HX8347A_DISP_CTRL8, HX8347A_DISP_CTRL8_SAP(0x7F));
	/* GON=0, DTE=0, D=01 */
	hx8347a_write_register(HX8347A_DISP_CTRL1, HX8347A_DISP_CTRL1_D(0x01));
	hx8347a_delay(100);
	
	/* GON=1, DTE=0, D=01 */
	hx8347a_write_register(HX8347A_DISP_CTRL1, 
			HX8347A_DISP_CTRL1_GON | HX8347A_DISP_CTRL1_D(0x01)); 
		
	/* GON=1, DTE=0, D=11 */
	hx8347a_write_register(HX8347A_DISP_CTRL1, 
			HX8347A_DISP_CTRL1_GON | HX8347A_DISP_CTRL1_D(0x03));
	hx8347a_delay(100);
	
	/* GON=1, DTE=1, D=11 */
	hx8347a_write_register(HX8347A_DISP_CTRL1, 
			HX8347A_DISP_CTRL1_GON |
			HX8347A_DISP_CTRL1_DTE | 
			HX8347A_DISP_CTRL1_D(0x03));
}

/**
 * \brief Turn off the LCD.
 */
void hx8347a_display_off(void)
{
	/* SAP=0000 0000 */
	hx8347a_write_register(HX8347A_DISP_CTRL8, 0x00);

	/* GON=0, DTE=0, D=00 */ 
	hx8347a_write_register(HX8347A_DISP_CTRL1, 0x00);
}

/**
 * \brief Set foreground color.
 *
 * \param us_color foreground color.
 */
void hx8347a_set_foreground_color(hx8347a_color_t us_color)
{
	uint32_t i;

	/* Fill the cache with selected color */
	for (i = 0; i < LCD_DATA_CACHE_SIZE; ++i) {
		g_ul_pixel_cache[i] = us_color;
	}
}

/**
 * \brief Fill the LCD buffer with the specified color.
 *
 * \param us_color fill color.
 */
void hx8347a_fill(hx8347a_color_t us_color)
{
	uint32_t i;

	hx8347a_set_cursor_position(0, 0);
	hx8347a_write_ram_prepare();

	for (i = HX8347A_LCD_WIDTH * HX8347A_LCD_HEIGHT; i > 0; i--) {
		hx8347a_write_ram(us_color);
	}
}

/**
 * \brief Set display window.
 *
 * \param ul_x Horizontal address start position
 * \param ul_y Vertical address start position
 * \param ul_width The width of the window.
 * \param ul_height The height of the window.
 */
void hx8347a_set_window(uint32_t ul_x, uint32_t ul_y, uint32_t ul_width,
		uint32_t ul_height)
{
	assert(ul_x <= 0xEF);
	assert(ul_y <= 0x13f);
	assert(ul_width <= (0xF0 - ul_x));
	assert(ul_height <= (0x140 - ul_y));

	uint8_t x1, x2, y1, y2;

	x1 = ul_x & 0xff;
	x2 = (ul_x & 0xff00) >> 8;
	y1 = ul_y & 0xff;
	y2 = (ul_y & 0xff00) >> 8;
	hx8347a_write_register(HX8347A_COL_ADDR_START2, x2); /* column high */
	hx8347a_write_register(HX8347A_COL_ADDR_START1, x1); /* column low */
	hx8347a_write_register(HX8347A_ROW_ADDR_START2, y2); /* row high */
	hx8347a_write_register(HX8347A_ROW_ADDR_START1, y1); /* row low */

	x1 = (ul_x + ul_width - 1) & 0xff;
	x2 = ((ul_x + ul_width - 1) & 0xff00) >> 8;
	y1 = (ul_y + ul_height - 1) & 0xff;
	y2 = ((ul_y + ul_height - 1) & 0xff00) >> 8;
	hx8347a_write_register(HX8347A_COL_ADDR_END2, x2); /* column high */
	hx8347a_write_register(HX8347A_COL_ADDR_END1, x1); /* column low */
	hx8347a_write_register(HX8347A_ROW_ADDR_END2, y2); /* row high */
	hx8347a_write_register(HX8347A_ROW_ADDR_END1, y1); /* row low */
}

/**
 * \brief Set cursor of LCD screen.
 *
 * \param us_x X coordinate of upper-left corner on LCD.
 * \param us_y Y coordinate of upper-left corner on LCD.
 */
void hx8347a_set_cursor_position(uint16_t us_x, uint16_t us_y)
{
	uint8_t x1, x2, y1, y2;

	x1 = us_x & 0xff;
	x2 = (us_x & 0xff00) >> 8;
	y1 = us_y & 0xff;
	y2 = (us_y & 0xff00) >> 8;
	hx8347a_write_register(HX8347A_COL_ADDR_START2, x2); /* column high */
	hx8347a_write_register(HX8347A_COL_ADDR_START1, x1); /* column low */
	hx8347a_write_register(HX8347A_ROW_ADDR_START2, y2); /* row high */
	hx8347a_write_register(HX8347A_ROW_ADDR_START1, y1); /* row low */
}

/**
 * \brief Scroll up/down for the number of specified lines.
 *
 * \param ul_lines number of lines to scroll.
 */
void hx8347a_scroll(int32_t ul_lines)
{
	hx8347a_write_register(HX8347A_VERTICAL_SCROLL_START_ADDR2,
			HX8347A_VERTICAL_SCROLL_START_ADDR2_VSP((ul_lines >> 8) & 0xF));
	hx8347a_write_register(HX8347A_VERTICAL_SCROLL_START_ADDR1,
			HX8347A_VERTICAL_SCROLL_START_ADDR1_VSP(ul_lines & 0xF));
}

/**
 * \brief Enables the scrolling feature.
 */
void hx8347a_enable_scroll(void)
{
	hx8347a_write_register(HX8347A_GATE_SCAN_CTRL,
			HX8347A_GATE_SCAN_CTRL_SCROLL_ON);
}

/**
 * \brief Disables the scrolling.
 */
void hx8347a_disable_scroll(void)
{
	hx8347a_write_register(HX8347A_GATE_SCAN_CTRL, 0);
}

/**
 * \brief Set display direction.
 *
 * \param dd 0: horizontal direction, 1: vertical direction
 */
void hx8347a_set_display_direction(display_direction_t dd)
{
	if (dd == LANDSCAPE) {
		hx8347a_write_register(HX8347A_MEMORY_ACCESS_CTRL,
				HX8347A_MEMORY_ACCESS_CTRL_MV | HX8347A_MEMORY_ACCESS_CTRL_BGR);
	} else {
		hx8347a_write_register(HX8347A_MEMORY_ACCESS_CTRL,
				HX8347A_MEMORY_ACCESS_CTRL_MY | 
				HX8347A_MEMORY_ACCESS_CTRL_MX |
				HX8347A_MEMORY_ACCESS_CTRL_BGR);
	}
}

/**
 * \brief Draw a pixel on LCD.
 *
 * \param ul_x X coordinate of pixel.
 * \param ul_y Y coordinate of pixel.
 *
 * \return 0 if succeeds, otherwise fails.
 */
uint32_t hx8347a_draw_pixel(uint32_t ul_x, uint32_t ul_y)
{
	if ((ul_x >= HX8347A_LCD_WIDTH) || (ul_y >= HX8347A_LCD_HEIGHT)) {
		return 1;
	}

	/* Set cursor */
	hx8347a_set_cursor_position(ul_x, ul_y);

	/* Prepare to write in GRAM */
	hx8347a_write_ram_prepare();
	hx8347a_write_ram(*g_ul_pixel_cache);
	return 0;
}

/**
 * \brief Get a pixel from LCD.
 *
 * \param ul_x X coordinate of pixel.
 * \param ul_y Y coordinate of pixel.
 *
 * \return the pixel color.
 */
hx8347a_color_t hx8347a_get_pixel(uint32_t ul_x, uint32_t ul_y)
{
	assert(ul_x <= HX8347A_LCD_WIDTH);
	assert(ul_y <= HX8347A_LCD_HEIGHT);

	/* Set cursor */
	hx8347a_set_cursor_position(ul_x, ul_y);

	/* Prepare to write in GRAM */
	hx8347a_read_ram_prepare();
	return hx8347a_read_ram();
}

/**
 * \brief Draw a line on LCD, which is not horizontal or vertical.
 *
 * \param ul_x1 X coordinate of line start.
 * \param ul_y1 Y coordinate of line start.
 * \param ul_x2 X coordinate of line end.
 * \param ul_y2 Y coordinate of line endl.
 */
static void hx8347a_draw_line_bresenham(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2)
{
	int dx, dy;
	int i;
	int xinc, yinc, cumul;
	int x, y;

	x = ul_x1;
	y = ul_y1;
	dx = ul_x2 - ul_x1;
	dy = ul_y2 - ul_y1;
	xinc = (dx > 0) ? 1 : -1;
	yinc = (dy > 0) ? 1 : -1;
	dx = abs(ul_x2 - ul_x1);
	dy = abs(ul_y2 - ul_y1);

	hx8347a_draw_pixel(x, y);

	if (dx > dy) {
		cumul = dx >> 1;

		for (i = 1; i <= dx; i++) {
			x += xinc;
			cumul += dy;

			if (cumul >= dx) {
				cumul -= dx;
				y += yinc;
			}

			hx8347a_draw_pixel(x, y);
		}
	} else {
		cumul = dy >> 1;

		for (i = 1; i <= dy; i++) {
			y += yinc;
			cumul += dx;

			if (cumul >= dy) {
				cumul -= dy;
				x += xinc;
			}

			hx8347a_draw_pixel(x, y);
		}
	}
}

/**
 * \brief Draw a line on LCD.
 *
 * \param ul_x1 X coordinate of line start.
 * \param ul_y1 Y coordinate of line start.
 * \param ul_x2 X coordinate of line end.
 * \param ul_y2 Y coordinate of line end.
 */
void hx8347a_draw_line(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2)
{
	if ((ul_y1 == ul_y2) || (ul_x1 == ul_x2)) {
		hx8347a_draw_filled_rectangle(ul_x1, ul_y1, ul_x2, ul_y2);
	} else {
		hx8347a_draw_line_bresenham(ul_x1, ul_y1, ul_x2, ul_y2);
	}
}

/**
 * \brief Draw a rectangle on LCD.
 *
 * \param ul_x1 X coordinate of upper-left corner on LCD.
 * \param ul_y1 Y coordinate of upper-left corner on LCD.
 * \param ul_x2 X coordinate of lower-right corner on LCD.
 * \param ul_y2 Y coordinate of lower-right corner on LCD.
 */
void hx8347a_draw_rectangle(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2)
{
	hx8347a_check_box_coordinates(&ul_x1, &ul_y1, &ul_x2, &ul_y2);

	hx8347a_draw_filled_rectangle(ul_x1, ul_y1, ul_x2, ul_y1);
	hx8347a_draw_filled_rectangle(ul_x1, ul_y2, ul_x2, ul_y2);

	hx8347a_draw_filled_rectangle(ul_x1, ul_y1, ul_x1, ul_y2);
	hx8347a_draw_filled_rectangle(ul_x2, ul_y1, ul_x2, ul_y2);
}

/**
 * \brief Draw a filled rectangle on LCD.
 *
 * \param ul_x1 X coordinate of upper-left corner on LCD.
 * \param ul_y1 Y coordinate of upper-left corner on LCD.
 * \param ul_x2 X coordinate of lower-right corner on LCD.
 * \param ul_y2 Y coordinate of lower-right corner on LCD.
 */
void hx8347a_draw_filled_rectangle(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2)
{
	uint32_t size, blocks;

	/* Swap coordinates if necessary */
	hx8347a_check_box_coordinates(&ul_x1, &ul_y1, &ul_x2, &ul_y2);

	/* Determine the refresh window area */
	hx8347a_set_window(ul_x1, ul_y1, (ul_x2 - ul_x1 + 1), (ul_y2 - ul_y1 + 1));

	/* Set cursor */
	hx8347a_set_cursor_position(ul_x1, ul_y1);

	/* Prepare to write in GRAM */
	hx8347a_write_ram_prepare();

	size = (ul_x2 - ul_x1 + 1) * (ul_y2 - ul_y1 + 1);
	/* Send pixels blocks => one SPI IT / block */
	blocks = size / LCD_DATA_CACHE_SIZE;
	while (blocks--) {
		hx8347a_write_ram_buffer(g_ul_pixel_cache, LCD_DATA_CACHE_SIZE);
	}
	/* Send remaining pixels */
	hx8347a_write_ram_buffer(g_ul_pixel_cache, size % LCD_DATA_CACHE_SIZE);

	/* Reset the refresh window area */
	hx8347a_set_window(0, 0, HX8347A_LCD_WIDTH, HX8347A_LCD_HEIGHT);
}

/**
 * \brief Draw a circle on LCD.
 *
 * \param ul_x X coordinate of circle center.
 * \param ul_y Y coordinate of circle center.
 * \param ul_r circle radius.
 *
 * \return 0 if succeeds, otherwise fails.
 */
uint32_t hx8347a_draw_circle(uint32_t ul_x, uint32_t ul_y, uint32_t ul_r)
{
	int32_t   d;    /* Decision Variable */
	uint32_t  curX; /* Current X Value */
	uint32_t  curY; /* Current Y Value */

	if (ul_r == 0) {
		return 1;
	}

	d = 3 - (ul_r << 1);
	curX = 0;
	curY = ul_r;

	while (curX <= curY) {
		hx8347a_draw_pixel(ul_x + curX, ul_y + curY);
		hx8347a_draw_pixel(ul_x + curX, ul_y - curY);
		hx8347a_draw_pixel(ul_x - curX, ul_y + curY);
		hx8347a_draw_pixel(ul_x - curX, ul_y - curY);
		hx8347a_draw_pixel(ul_x + curY, ul_y + curX);
		hx8347a_draw_pixel(ul_x + curY, ul_y - curX);
		hx8347a_draw_pixel(ul_x - curY, ul_y + curX);
		hx8347a_draw_pixel(ul_x - curY, ul_y - curX);

		if (d < 0) {
			d += (curX << 2) + 6;
		} else {
			d += ((curX - curY) << 2) + 10;
			curY--;
		}
		curX++;
	}

	return 0;
}

/**
 * \brief Draw a filled circle on LCD.
 *
 * \param ul_x X coordinate of circle center.
 * \param ul_y Y coordinate of circle center.
 * \param ul_r circle radius.
 *
 * \return 0 if succeeds, otherwise fails.
 */
uint32_t hx8347a_draw_filled_circle(uint32_t ul_x, uint32_t ul_y, uint32_t ul_r)
{
	signed int d;       /* Decision Variable */
	uint32_t dwCurX;    /* Current X Value */
	uint32_t dwCurY;    /* Current Y Value */
	uint32_t dwXmin, dwYmin;

	if (ul_r == 0) {
		return 1;
	}

	d = 3 - (ul_r << 1);
	dwCurX = 0;
	dwCurY = ul_r;

	while (dwCurX <= dwCurY) {
		dwXmin = (dwCurX > ul_x) ? 0 : ul_x - dwCurX;
		dwYmin = (dwCurY > ul_y) ? 0 : ul_y - dwCurY;
		hx8347a_draw_filled_rectangle(dwXmin, dwYmin, ul_x + dwCurX, dwYmin);
		hx8347a_draw_filled_rectangle(dwXmin, ul_y + dwCurY, ul_x + dwCurX, ul_y + dwCurY);
		dwXmin = (dwCurY > ul_x) ? 0 : ul_x - dwCurY;
		dwYmin = (dwCurX > ul_y) ? 0 : ul_y - dwCurX;
		hx8347a_draw_filled_rectangle(dwXmin, dwYmin, ul_x + dwCurY, dwYmin);
		hx8347a_draw_filled_rectangle(dwXmin, ul_y + dwCurX, ul_x + dwCurY, ul_y + dwCurX);

		if (d < 0) {
			d += (dwCurX << 2) + 6;
		} else {
			d += ((dwCurX - dwCurY) << 2) + 10;
			dwCurY--;
		}

		dwCurX++;
	}

	return 0;
}

/**
 * \brief Draw an ASCII character on LCD.
 *
 * \param ul_x X coordinate of character upper-left corner.
 * \param ul_y Y coordinate of character upper-left corner.
 * \param uc_c character to print.
 */
static void hx8347a_draw_char(uint32_t ul_x, uint32_t ul_y, uint8_t uc_c)
{
	uint32_t row, col;
	uint32_t offset, offset0, offset1;

	/* Compute offset according of the specified ASCII character */
	/* Note: the first 32 characters of the ASCII table are not handled */
	offset = ((uint32_t)uc_c - 0x20) * 20;

	for (col = 0; col < 10; col++) {
		/* Compute the first and second byte offset of a column */
		offset0 = offset + col * 2;
		offset1 = offset0 + 1;

		/* Draw pixel on screen depending on the corresponding bit value from the charset */
		for (row = 0; row < 8; row++) {
			if ((p_uc_charset10x14[offset0] >> (7 - row)) & 0x1) {
				hx8347a_draw_pixel(ul_x + col, ul_y + row);
			}
		}

		for (row = 0; row < 6; row++) {
			if ((p_uc_charset10x14[offset1] >> (7 - row)) & 0x1) {
				hx8347a_draw_pixel(ul_x + col, ul_y + row + 8);
			}
		}
	}
}

/**
 * \brief Draw a string on LCD.
 *
 * \param ul_x X coordinate of string top-left corner.
 * \param ul_y Y coordinate of string top-left corner.
 * \param p_str String to display.
 */
void hx8347a_draw_string(uint32_t ul_x, uint32_t ul_y, const uint8_t *p_str)
{
	uint32_t xorg = ul_x;

	while (*p_str != 0) {
		/* If newline, jump to the next line (font height + 2) */
		if (*p_str == '\n') {
			ul_y += gfont.height + 2;
			ul_x = xorg;
		} else {
			/* Draw the character and place cursor right after (font width + 2) */
			hx8347a_draw_char(ul_x, ul_y, *p_str);
			ul_x += gfont.width + 2;
		}
		p_str++;
	}
}

/**
 * \brief Draw a pixmap on LCD.
 *
 * \param ul_x X coordinate of upper-left corner on LCD.
 * \param ul_y Y coordinate of upper-left corner on LCD.
 * \param ul_width width of the picture.
 * \param ul_height height of the picture.
 * \param p_ul_pixmap pixmap of the image.
 */
void hx8347a_draw_pixmap(uint32_t ul_x, uint32_t ul_y, uint32_t ul_width,
		uint32_t ul_height, const hx8347a_color_t *p_ul_pixmap)
{
	uint32_t size;
	uint32_t dwX1, dwY1, dwX2, dwY2;
	dwX1 = ul_x;
	dwY1 = ul_y;
	dwX2 = ul_x + ul_width;
	dwY2 = ul_y + ul_height;

	/* Swap coordinates if necessary */
	hx8347a_check_box_coordinates(&dwX1, &dwY1, &dwX2, &dwY2);

	/* Determine the refresh window area */
	hx8347a_set_window(dwX1, dwY1, (dwX2 - dwX1), (dwY2 - dwY1));

	/* Set cursor */
	hx8347a_set_cursor_position(dwX1, dwY1);

	/* Prepare to write in GRAM */
	hx8347a_write_ram_prepare();

	size = (dwX2 - dwX1) * (dwY2 - dwY1);

	hx8347a_write_ram_buffer(p_ul_pixmap, size);

	/* Reset the refresh window area */
	hx8347a_set_window(0, 0, HX8347A_LCD_WIDTH, HX8347A_LCD_HEIGHT);
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \}
 */
