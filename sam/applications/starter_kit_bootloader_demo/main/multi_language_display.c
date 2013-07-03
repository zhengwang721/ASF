/**
 * \file
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#include "conf_example.h"
#include "multi_language_display.h"
#include "unicode_font_table.h"
#include "ssd1306.h"
#include "font.h"
#include "delay.h"

#ifndef CONF_ENGLISH_LANGUAGE
/* Font bitmap buffer. */
uint8_t font_bitmap_origin[32];
uint8_t font_bitmap_show[32];
uint8_t font_bitmap_width;

/**
 * \brief Transfer the font bitmap data to fit the display.
 */
static void font_bitmap_transfer(void)
{
	if (font_bitmap_width == 2) {
		font_bitmap_show[0] = ((font_bitmap_origin[0] & 0x80) >> 7) |
				((font_bitmap_origin[2] & 0x80) >> 6) |
				((font_bitmap_origin[4] & 0x80) >> 5) |
				((font_bitmap_origin[6] & 0x80) >> 4) |
				((font_bitmap_origin[8] & 0x80) >> 3) |
				((font_bitmap_origin[10] & 0x80) >> 2) |
				((font_bitmap_origin[12] & 0x80) >> 1) |
				((font_bitmap_origin[14] & 0x80) >> 0);
		font_bitmap_show[1] = ((font_bitmap_origin[0] & 0x40) >> 6) |
				((font_bitmap_origin[2] & 0x40) >> 5) |
				((font_bitmap_origin[4] & 0x40) >> 4) |
				((font_bitmap_origin[6] & 0x40) >> 3) |
				((font_bitmap_origin[8] & 0x40) >> 2) |
				((font_bitmap_origin[10] & 0x40) >> 1) |
				((font_bitmap_origin[12] & 0x40) >> 0) |
				((font_bitmap_origin[14] & 0x40) << 1);
		font_bitmap_show[2] = ((font_bitmap_origin[0] & 0x20) >> 5) |
				((font_bitmap_origin[2] & 0x20) >> 4) |
				((font_bitmap_origin[4] & 0x20) >> 3) |
				((font_bitmap_origin[6] & 0x20) >> 2) |
				((font_bitmap_origin[8] & 0x20) >> 1) |
				((font_bitmap_origin[10] & 0x20) >> 0) |
				((font_bitmap_origin[12] & 0x20) << 1) |
				((font_bitmap_origin[14] & 0x20) << 2);
		font_bitmap_show[3] = ((font_bitmap_origin[0] & 0x10) >> 4) |
				((font_bitmap_origin[2] & 0x10) >> 3) |
				((font_bitmap_origin[4] & 0x10) >> 2) |
				((font_bitmap_origin[6] & 0x10) >> 1) |
				((font_bitmap_origin[8] & 0x10) >> 0) |
				((font_bitmap_origin[10] & 0x10) << 1) |
				((font_bitmap_origin[12] & 0x10) << 2) |
				((font_bitmap_origin[14] & 0x10) << 3);
		font_bitmap_show[4] = ((font_bitmap_origin[0] & 0x08) >> 3) |
				((font_bitmap_origin[2] & 0x08) >> 2) |
				((font_bitmap_origin[4] & 0x08) >> 1) |
				((font_bitmap_origin[6] & 0x08) >> 0) |
				((font_bitmap_origin[8] & 0x08) << 1) |
				((font_bitmap_origin[10] & 0x08) << 2) |
				((font_bitmap_origin[12] & 0x08) << 3) |
				((font_bitmap_origin[14] & 0x08) << 4);
		font_bitmap_show[5] = ((font_bitmap_origin[0] & 0x04) >> 2) |
				((font_bitmap_origin[2] & 0x04) >> 1) |
				((font_bitmap_origin[4] & 0x04) >> 0) |
				((font_bitmap_origin[6] & 0x04) << 1) |
				((font_bitmap_origin[8] & 0x04) << 2) |
				((font_bitmap_origin[10] & 0x04) << 3) |
				((font_bitmap_origin[12] & 0x04) << 4) |
				((font_bitmap_origin[14] & 0x04) << 5);
		font_bitmap_show[6] = ((font_bitmap_origin[0] & 0x02) >> 1) |
				((font_bitmap_origin[2] & 0x02) >> 0) |
				((font_bitmap_origin[4] & 0x02) << 1) |
				((font_bitmap_origin[6] & 0x02) << 2) |
				((font_bitmap_origin[8] & 0x02) << 3) |
				((font_bitmap_origin[10] & 0x02) << 4) |
				((font_bitmap_origin[12] & 0x02) << 5) |
				((font_bitmap_origin[14] & 0x02) << 6);
		font_bitmap_show[7] = ((font_bitmap_origin[0] & 0x01) >> 0) |
				((font_bitmap_origin[2] & 0x01) << 1) |
				((font_bitmap_origin[4] & 0x01) << 2) |
				((font_bitmap_origin[6] & 0x01) << 3) |
				((font_bitmap_origin[8] & 0x01) << 4) |
				((font_bitmap_origin[10] & 0x01) << 5) |
				((font_bitmap_origin[12] & 0x01) << 6) |
				((font_bitmap_origin[14] & 0x01) << 7);
		font_bitmap_show[8] = ((font_bitmap_origin[1] & 0x80) >> 7) |
				((font_bitmap_origin[3] & 0x80) >> 6) |
				((font_bitmap_origin[5] & 0x80) >> 5) |
				((font_bitmap_origin[7] & 0x80) >> 4) |
				((font_bitmap_origin[9] & 0x80) >> 3) |
				((font_bitmap_origin[11] & 0x80) >> 2) |
				((font_bitmap_origin[13] & 0x80) >> 1) |
				((font_bitmap_origin[15] & 0x80) >> 0);
		font_bitmap_show[9] = ((font_bitmap_origin[1] & 0x40) >> 6) |
				((font_bitmap_origin[3] & 0x40) >> 5) |
				((font_bitmap_origin[5] & 0x40) >> 4) |
				((font_bitmap_origin[7] & 0x40) >> 3) |
				((font_bitmap_origin[9] & 0x40) >> 2) |
				((font_bitmap_origin[11] & 0x40) >> 1) |
				((font_bitmap_origin[13] & 0x40) >> 1) |
				((font_bitmap_origin[15] & 0x40) << 1);
		font_bitmap_show[10] = ((font_bitmap_origin[1] & 0x20) >> 5) |
				((font_bitmap_origin[3] & 0x20) >> 4) |
				((font_bitmap_origin[5] & 0x20) >> 3) |
				((font_bitmap_origin[7] & 0x20) >> 2) |
				((font_bitmap_origin[9] & 0x20) >> 1) |
				((font_bitmap_origin[11] & 0x20) >> 0) |
				((font_bitmap_origin[13] & 0x20) << 1) |
				((font_bitmap_origin[15] & 0x20) << 2);
		font_bitmap_show[11] = ((font_bitmap_origin[1] & 0x10) >> 4) |
				((font_bitmap_origin[3] & 0x10) >> 3) |
				((font_bitmap_origin[5] & 0x10) >> 2) |
				((font_bitmap_origin[7] & 0x10) >> 1) |
				((font_bitmap_origin[9] & 0x10) >> 0) |
				((font_bitmap_origin[11] & 0x10) << 1) |
				((font_bitmap_origin[13] & 0x10) << 2) |
				((font_bitmap_origin[15] & 0x10) << 3);
		font_bitmap_show[12] = ((font_bitmap_origin[1] & 0x08) >> 3) |
				((font_bitmap_origin[3] & 0x08) >> 2) |
				((font_bitmap_origin[5] & 0x08) >> 1) |
				((font_bitmap_origin[7] & 0x08) >> 0) |
				((font_bitmap_origin[9] & 0x08) << 1) |
				((font_bitmap_origin[11] & 0x08) << 2) |
				((font_bitmap_origin[13] & 0x08) << 3) |
				((font_bitmap_origin[15] & 0x08) << 4);
		font_bitmap_show[13] = ((font_bitmap_origin[1] & 0x04) >> 2) |
				((font_bitmap_origin[3] & 0x04) >> 1) |
				((font_bitmap_origin[5] & 0x04) >> 0) |
				((font_bitmap_origin[7] & 0x04) << 1) |
				((font_bitmap_origin[9] & 0x04) << 2) |
				((font_bitmap_origin[11] & 0x04) << 3) |
				((font_bitmap_origin[13] & 0x04) << 4) |
				((font_bitmap_origin[15] & 0x04) << 5);
		font_bitmap_show[14] = ((font_bitmap_origin[1] & 0x02) >> 1) |
				((font_bitmap_origin[3] & 0x02) >> 0) |
				((font_bitmap_origin[5] & 0x02) << 1) |
				((font_bitmap_origin[7] & 0x02) << 2) |
				((font_bitmap_origin[9] & 0x02) << 3) |
				((font_bitmap_origin[11] & 0x02) << 4) |
				((font_bitmap_origin[13] & 0x02) << 5) |
				((font_bitmap_origin[15] & 0x02) << 6);
		font_bitmap_show[15] = ((font_bitmap_origin[1] & 0x01) >> 0) |
				((font_bitmap_origin[3] & 0x01) << 1) |
				((font_bitmap_origin[5] & 0x01) << 2) |
				((font_bitmap_origin[7] & 0x01) << 3) |
				((font_bitmap_origin[9] & 0x01) << 4) |
				((font_bitmap_origin[11] & 0x01) << 5) |
				((font_bitmap_origin[13] & 0x01) << 6) |
				((font_bitmap_origin[15] & 0x01) << 7);
		font_bitmap_show[16] = ((font_bitmap_origin[16] & 0x80) >> 7) |
				((font_bitmap_origin[18] & 0x80) >> 6) |
				((font_bitmap_origin[20] & 0x80) >> 5) |
				((font_bitmap_origin[22] & 0x80) >> 4) |
				((font_bitmap_origin[24] & 0x80) >> 3) |
				((font_bitmap_origin[26] & 0x80) >> 2) |
				((font_bitmap_origin[28] & 0x80) >> 1) |
				((font_bitmap_origin[30] & 0x80) >> 0);
		font_bitmap_show[17] = ((font_bitmap_origin[16] & 0x40) >> 6) |
				((font_bitmap_origin[18] & 0x40) >> 5) |
				((font_bitmap_origin[20] & 0x40) >> 4) |
				((font_bitmap_origin[22] & 0x40) >> 3) |
				((font_bitmap_origin[24] & 0x40) >> 2) |
				((font_bitmap_origin[26] & 0x40) >> 1) |
				((font_bitmap_origin[28] & 0x40) >> 0) |
				((font_bitmap_origin[30] & 0x40) << 1);
		font_bitmap_show[18] = ((font_bitmap_origin[16] & 0x20) >> 5) |
				((font_bitmap_origin[18] & 0x20) >> 4) |
				((font_bitmap_origin[20] & 0x20) >> 3) |
				((font_bitmap_origin[22] & 0x20) >> 2) |
				((font_bitmap_origin[24] & 0x20) >> 1) |
				((font_bitmap_origin[26] & 0x20) >> 0) |
				((font_bitmap_origin[28] & 0x20) << 1) |
				((font_bitmap_origin[30] & 0x20) << 2);
		font_bitmap_show[19] = ((font_bitmap_origin[16] & 0x10) >> 4) |
				((font_bitmap_origin[18] & 0x10) >> 3) |
				((font_bitmap_origin[20] & 0x10) >> 2) |
				((font_bitmap_origin[22] & 0x10) >> 1) |
				((font_bitmap_origin[24] & 0x10) >> 0) |
				((font_bitmap_origin[26] & 0x10) << 1) |
				((font_bitmap_origin[28] & 0x10) << 2) |
				((font_bitmap_origin[30] & 0x10) << 3);
		font_bitmap_show[20] = ((font_bitmap_origin[16] & 0x08) >> 3) |
				((font_bitmap_origin[18] & 0x08) >> 2) |
				((font_bitmap_origin[20] & 0x08) >> 1) |
				((font_bitmap_origin[22] & 0x08) >> 0) |
				((font_bitmap_origin[24] & 0x08) << 1) |
				((font_bitmap_origin[26] & 0x08) << 2) |
				((font_bitmap_origin[28] & 0x08) << 3) |
				((font_bitmap_origin[30] & 0x08) << 4);
		font_bitmap_show[21] = ((font_bitmap_origin[16] & 0x04) >> 2) |
				((font_bitmap_origin[18] & 0x04) >> 1) |
				((font_bitmap_origin[20] & 0x04) >> 0) |
				((font_bitmap_origin[22] & 0x04) << 1) |
				((font_bitmap_origin[24] & 0x04) << 2) |
				((font_bitmap_origin[26] & 0x04) << 3) |
				((font_bitmap_origin[28] & 0x04) << 4) |
				((font_bitmap_origin[30] & 0x04) << 5);
		font_bitmap_show[22] = ((font_bitmap_origin[16] & 0x02) >> 1) |
				((font_bitmap_origin[18] & 0x02) >> 0) |
				((font_bitmap_origin[20] & 0x02) << 1) |
				((font_bitmap_origin[22] & 0x02) << 2) |
				((font_bitmap_origin[24] & 0x02) << 3) |
				((font_bitmap_origin[26] & 0x02) << 4) |
				((font_bitmap_origin[28] & 0x02) << 5) |
				((font_bitmap_origin[30] & 0x02) << 6);
		font_bitmap_show[23] = ((font_bitmap_origin[16] & 0x01) >> 0) |
				((font_bitmap_origin[18] & 0x01) << 1) |
				((font_bitmap_origin[20] & 0x01) << 2) |
				((font_bitmap_origin[22] & 0x01) << 3) |
				((font_bitmap_origin[24] & 0x01) << 4) |
				((font_bitmap_origin[26] & 0x01) << 5) |
				((font_bitmap_origin[28] & 0x01) << 6) |
				((font_bitmap_origin[30] & 0x01) << 7);
		font_bitmap_show[24] = ((font_bitmap_origin[17] & 0x80) >> 7) |
				((font_bitmap_origin[19] & 0x80) >> 6) |
				((font_bitmap_origin[21] & 0x80) >> 5) |
				((font_bitmap_origin[23] & 0x80) >> 4) |
				((font_bitmap_origin[25] & 0x80) >> 3) |
				((font_bitmap_origin[27] & 0x80) >> 2) |
				((font_bitmap_origin[29] & 0x80) >> 1) |
				((font_bitmap_origin[31] & 0x80) >> 0);
		font_bitmap_show[25] = ((font_bitmap_origin[17] & 0x40) >> 6) |
				((font_bitmap_origin[19] & 0x40) >> 5) |
				((font_bitmap_origin[21] & 0x40) >> 4) |
				((font_bitmap_origin[23] & 0x40) >> 3) |
				((font_bitmap_origin[25] & 0x40) >> 2) |
				((font_bitmap_origin[27] & 0x40) >> 1) |
				((font_bitmap_origin[29] & 0x40) >> 0) |
				((font_bitmap_origin[31] & 0x40) << 1);
		font_bitmap_show[26] = ((font_bitmap_origin[17] & 0x20) >> 5) |
				((font_bitmap_origin[19] & 0x20) >> 4) |
				((font_bitmap_origin[21] & 0x20) >> 3) |
				((font_bitmap_origin[23] & 0x20) >> 2) |
				((font_bitmap_origin[25] & 0x20) >> 1) |
				((font_bitmap_origin[27] & 0x20) >> 0) |
				((font_bitmap_origin[29] & 0x20) << 1) |
				((font_bitmap_origin[31] & 0x20) << 2);
		font_bitmap_show[27] = ((font_bitmap_origin[17] & 0x10) >> 4) |
				((font_bitmap_origin[19] & 0x10) >> 3) |
				((font_bitmap_origin[21] & 0x10) >> 2) |
				((font_bitmap_origin[23] & 0x10) >> 1) |
				((font_bitmap_origin[25] & 0x10) >> 0) |
				((font_bitmap_origin[27] & 0x10) << 1) |
				((font_bitmap_origin[29] & 0x10) << 2) |
				((font_bitmap_origin[31] & 0x10) << 3);
		font_bitmap_show[28] = ((font_bitmap_origin[17] & 0x08) >> 3) |
				((font_bitmap_origin[19] & 0x08) >> 2) |
				((font_bitmap_origin[21] & 0x08) >> 1) |
				((font_bitmap_origin[23] & 0x08) >> 0) |
				((font_bitmap_origin[25] & 0x08) << 1) |
				((font_bitmap_origin[27] & 0x08) << 2) |
				((font_bitmap_origin[29] & 0x08) << 3) |
				((font_bitmap_origin[31] & 0x08) << 4);
		font_bitmap_show[29] = ((font_bitmap_origin[17] & 0x04) >> 2) |
				((font_bitmap_origin[19] & 0x04) >> 1) |
				((font_bitmap_origin[21] & 0x04) >> 0) |
				((font_bitmap_origin[23] & 0x04) << 1) |
				((font_bitmap_origin[25] & 0x04) << 2) |
				((font_bitmap_origin[27] & 0x04) << 3) |
				((font_bitmap_origin[29] & 0x04) << 4) |
				((font_bitmap_origin[31] & 0x04) << 5);
		font_bitmap_show[30] = ((font_bitmap_origin[17] & 0x02) >> 1) |
				((font_bitmap_origin[19] & 0x02) >> 0) |
				((font_bitmap_origin[21] & 0x02) << 1) |
				((font_bitmap_origin[23] & 0x02) << 2) |
				((font_bitmap_origin[25] & 0x02) << 3) |
				((font_bitmap_origin[27] & 0x02) << 4) |
				((font_bitmap_origin[29] & 0x02) << 5) |
				((font_bitmap_origin[31] & 0x02) << 6);
		font_bitmap_show[31] = ((font_bitmap_origin[17] & 0x01) >> 0) |
				((font_bitmap_origin[19] & 0x01) << 1) |
				((font_bitmap_origin[21] & 0x01) << 2) |
				((font_bitmap_origin[23] & 0x01) << 3) |
				((font_bitmap_origin[25] & 0x01) << 4) |
				((font_bitmap_origin[27] & 0x01) << 5) |
				((font_bitmap_origin[29] & 0x01) << 6) |
				((font_bitmap_origin[31] & 0x01) << 7);
	} else {
		font_bitmap_show[0] = ((font_bitmap_origin[0] & 0x80) >> 7) |
				((font_bitmap_origin[1] & 0x80) >> 6) |
				((font_bitmap_origin[2] & 0x80) >> 5) |
				((font_bitmap_origin[3] & 0x80) >> 4) |
				((font_bitmap_origin[4] & 0x80) >> 3) |
				((font_bitmap_origin[5] & 0x80) >> 2) |
				((font_bitmap_origin[6] & 0x80) >> 1) |
				((font_bitmap_origin[7] & 0x80) >> 0);
		font_bitmap_show[1] = ((font_bitmap_origin[0] & 0x40) >> 6) |
				((font_bitmap_origin[1] & 0x40) >> 5) |
				((font_bitmap_origin[2] & 0x40) >> 4) |
				((font_bitmap_origin[3] & 0x40) >> 3) |
				((font_bitmap_origin[4] & 0x40) >> 2) |
				((font_bitmap_origin[5] & 0x40) >> 1) |
				((font_bitmap_origin[6] & 0x40) >> 0) |
				((font_bitmap_origin[7] & 0x40) << 1);
		font_bitmap_show[2] = ((font_bitmap_origin[0] & 0x20) >> 5) |
				((font_bitmap_origin[1] & 0x20) >> 4) |
				((font_bitmap_origin[2] & 0x20) >> 3) |
				((font_bitmap_origin[3] & 0x20) >> 2) |
				((font_bitmap_origin[4] & 0x20) >> 1) |
				((font_bitmap_origin[5] & 0x20) >> 0) |
				((font_bitmap_origin[6] & 0x20) << 1) |
				((font_bitmap_origin[7] & 0x20) << 2);
		font_bitmap_show[3] = ((font_bitmap_origin[0] & 0x10) >> 4) |
				((font_bitmap_origin[1] & 0x10) >> 3) |
				((font_bitmap_origin[2] & 0x10) >> 2) |
				((font_bitmap_origin[3] & 0x10) >> 1) |
				((font_bitmap_origin[4] & 0x10) >> 0) |
				((font_bitmap_origin[5] & 0x10) << 1) |
				((font_bitmap_origin[6] & 0x10) << 2) |
				((font_bitmap_origin[7] & 0x10) << 3);
		font_bitmap_show[4] = ((font_bitmap_origin[0] & 0x08) >> 3) |
				((font_bitmap_origin[1] & 0x08) >> 2) |
				((font_bitmap_origin[2] & 0x08) >> 1) |
				((font_bitmap_origin[3] & 0x08) >> 0) |
				((font_bitmap_origin[4] & 0x08) << 1) |
				((font_bitmap_origin[5] & 0x08) << 2) |
				((font_bitmap_origin[6] & 0x08) << 3) |
				((font_bitmap_origin[7] & 0x08) << 4);
		font_bitmap_show[5] = ((font_bitmap_origin[0] & 0x04) >> 2) |
				((font_bitmap_origin[1] & 0x04) >> 1) |
				((font_bitmap_origin[2] & 0x04) >> 0) |
				((font_bitmap_origin[3] & 0x04) << 1) |
				((font_bitmap_origin[4] & 0x04) << 2) |
				((font_bitmap_origin[5] & 0x04) << 3) |
				((font_bitmap_origin[6] & 0x04) << 4) |
				((font_bitmap_origin[7] & 0x04) << 5);
		font_bitmap_show[6] = ((font_bitmap_origin[0] & 0x02) >> 1) |
				((font_bitmap_origin[1] & 0x02) >> 0) |
				((font_bitmap_origin[2] & 0x02) << 1) |
				((font_bitmap_origin[3] & 0x02) << 2) |
				((font_bitmap_origin[4] & 0x02) << 3) |
				((font_bitmap_origin[5] & 0x02) << 4) |
				((font_bitmap_origin[6] & 0x02) << 5) |
				((font_bitmap_origin[7] & 0x02) << 6);
		font_bitmap_show[7] = ((font_bitmap_origin[0] & 0x01) >> 0) |
				((font_bitmap_origin[1] & 0x01) << 1) |
				((font_bitmap_origin[2] & 0x01) << 2) |
				((font_bitmap_origin[3] & 0x01) << 3) |
				((font_bitmap_origin[4] & 0x01) << 4) |
				((font_bitmap_origin[5] & 0x01) << 5) |
				((font_bitmap_origin[6] & 0x01) << 6) |
				((font_bitmap_origin[7] & 0x01) << 7);

		font_bitmap_show[16] = ((font_bitmap_origin[8] & 0x80) >> 7) |
				((font_bitmap_origin[9] & 0x80) >> 6) |
				((font_bitmap_origin[10] & 0x80) >> 5) |
				((font_bitmap_origin[11] & 0x80) >> 4) |
				((font_bitmap_origin[12] & 0x80) >> 3) |
				((font_bitmap_origin[13] & 0x80) >> 2) |
				((font_bitmap_origin[14] & 0x80) >> 1) |
				((font_bitmap_origin[15] & 0x80) >> 0);
		font_bitmap_show[17] = ((font_bitmap_origin[8] & 0x40) >> 6) |
				((font_bitmap_origin[9] & 0x40) >> 5) |
				((font_bitmap_origin[10] & 0x40) >> 4) |
				((font_bitmap_origin[11] & 0x40) >> 3) |
				((font_bitmap_origin[12] & 0x40) >> 2) |
				((font_bitmap_origin[13] & 0x40) >> 1) |
				((font_bitmap_origin[14] & 0x40) >> 0) |
				((font_bitmap_origin[15] & 0x40) << 1);
		font_bitmap_show[18] = ((font_bitmap_origin[8] & 0x20) >> 5) |
				((font_bitmap_origin[9] & 0x20) >> 4) |
				((font_bitmap_origin[10] & 0x20) >> 3) |
				((font_bitmap_origin[11] & 0x20) >> 2) |
				((font_bitmap_origin[12] & 0x20) >> 1) |
				((font_bitmap_origin[13] & 0x20) >> 0) |
				((font_bitmap_origin[14] & 0x20) << 1) |
				((font_bitmap_origin[15] & 0x20) << 2);
		font_bitmap_show[19] = ((font_bitmap_origin[8] & 0x10) >> 4) |
				((font_bitmap_origin[9] & 0x10) >> 3) |
				((font_bitmap_origin[10] & 0x10) >> 2) |
				((font_bitmap_origin[11] & 0x10) >> 1) |
				((font_bitmap_origin[12] & 0x10) >> 0) |
				((font_bitmap_origin[13] & 0x10) << 1) |
				((font_bitmap_origin[14] & 0x10) << 2) |
				((font_bitmap_origin[15] & 0x10) << 3);
		font_bitmap_show[20] = ((font_bitmap_origin[8] & 0x08) >> 3) |
				((font_bitmap_origin[9] & 0x08) >> 2) |
				((font_bitmap_origin[10] & 0x08) >> 1) |
				((font_bitmap_origin[11] & 0x08) >> 0) |
				((font_bitmap_origin[12] & 0x08) << 1) |
				((font_bitmap_origin[13] & 0x08) << 2) |
				((font_bitmap_origin[14] & 0x08) << 3) |
				((font_bitmap_origin[15] & 0x08) << 4);
		font_bitmap_show[21] = ((font_bitmap_origin[8] & 0x04) >> 2) |
				((font_bitmap_origin[9] & 0x04) >> 1) |
				((font_bitmap_origin[10] & 0x04) >> 0) |
				((font_bitmap_origin[11] & 0x04) << 1) |
				((font_bitmap_origin[12] & 0x04) << 2) |
				((font_bitmap_origin[13] & 0x04) << 3) |
				((font_bitmap_origin[14] & 0x04) << 4) |
				((font_bitmap_origin[15] & 0x04) << 5);
		font_bitmap_show[22] = ((font_bitmap_origin[8] & 0x02) >> 1) |
				((font_bitmap_origin[9] & 0x02) >> 0) |
				((font_bitmap_origin[10] & 0x02) << 1) |
				((font_bitmap_origin[11] & 0x02) << 2) |
				((font_bitmap_origin[12] & 0x02) << 3) |
				((font_bitmap_origin[13] & 0x02) << 4) |
				((font_bitmap_origin[14] & 0x02) << 5) |
				((font_bitmap_origin[15] & 0x02) << 6);
		font_bitmap_show[23] = ((font_bitmap_origin[8] & 0x01) >> 0) |
				((font_bitmap_origin[9] & 0x01) << 1) |
				((font_bitmap_origin[10] & 0x01) << 2) |
				((font_bitmap_origin[11] & 0x01) << 3) |
				((font_bitmap_origin[12] & 0x01) << 4) |
				((font_bitmap_origin[13] & 0x01) << 5) |
				((font_bitmap_origin[14] & 0x01) << 6) |
				((font_bitmap_origin[15] & 0x01) << 7);
	}
}

/**
 * \brief Find the font bitmap data by given the unicode.
 */
static void font_bitmap_find(uint16_t unicode)
{
	uint32_t i;

	volatile uint32_t first_char_info_offset;
	volatile uint32_t unicode_char_info_offset;
	volatile uint32_t unicode_char_info;
	volatile uint32_t font_bitmap_offset;

	first_char_info_offset = unicode_font_table[0x14] |
			(unicode_font_table[0x15] << 8) |
			(unicode_font_table[0x16] << 16) |
			(unicode_font_table[0x17] << 24);

	unicode_char_info_offset = first_char_info_offset +
			(unicode - unicode_font_table[0x10]) * 4;
	unicode_char_info = unicode_font_table[unicode_char_info_offset] |
			(unicode_font_table[unicode_char_info_offset + 1] << 8) |
			(unicode_font_table[unicode_char_info_offset + 2] << 16) |
			(unicode_font_table[unicode_char_info_offset + 3] << 24);
	font_bitmap_offset = unicode_char_info & 0xffffff;
	font_bitmap_width = unicode_font_table[unicode_char_info_offset + 3];
	if (font_bitmap_width > 0x20) {
		font_bitmap_width = 2;
	} else {
		font_bitmap_width = 1;
	}

	for (i = 0; i < 32; i++) {
		font_bitmap_origin[i] = unicode_font_table[font_bitmap_offset];
		font_bitmap_offset++;
	}
}

/**
 * \brief Display the font bitmap data in the given position.
 *
 * \param page page address of the OLED.
 * \param column column address of the OLED.
 */
static void font_bitmap_display(uint8_t page, uint8_t column)
{
	uint32_t i;

	ssd1306_set_page_address(page);
	ssd1306_set_column_address(column);
	for (i = 0; i < (8 * font_bitmap_width); i++) {
		ssd1306_write_data(font_bitmap_show[i]);
	}
	ssd1306_set_page_address(page + 1);
	ssd1306_set_column_address(column);
	for (i = 0; i < (8 * font_bitmap_width); i++) {
		ssd1306_write_data(font_bitmap_show[i + 16]);
	}
}
#endif

/**
 * \brief Show the switch info on the OLED screen.
 */
void multi_language_show_switch_info(void)
{
	/* Clear screen. */
	ssd1306_clear();

	/* Start message on the screen. */
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_set_page_address(0);
	ssd1306_set_column_address(0);
	ssd1306_write_text("Other language demo is");
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(0);
	ssd1306_write_text("found on SD card.");
	ssd1306_set_page_address(2);
	ssd1306_set_column_address(0);
	ssd1306_write_text("Push SW0 to switch.");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x5361);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x4E2D);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x6709);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x5176);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x5B83);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x7248);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
	font_bitmap_find(0x7A0B);
	font_bitmap_transfer();
	font_bitmap_display(0, 96);
	font_bitmap_find(0x5E8F);
	font_bitmap_transfer();
	font_bitmap_display(0, 112);
	font_bitmap_find(0x6309);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	ssd1306_set_page_address(3);
	ssd1306_set_column_address(17);
	ssd1306_write_text("SW0");
	font_bitmap_find(0x5207);
	font_bitmap_transfer();
	font_bitmap_display(2, 34);
	font_bitmap_find(0x6362);
	font_bitmap_transfer();
	font_bitmap_display(2, 50);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x62BC);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x3059);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(33);
	ssd1306_write_text("SW0");
	font_bitmap_find(0x30B9);
	font_bitmap_transfer();
	font_bitmap_display(0, 50);
	font_bitmap_find(0x30A4);
	font_bitmap_transfer();
	font_bitmap_display(0, 66);
	font_bitmap_find(0x30C3);
	font_bitmap_transfer();
	font_bitmap_display(0, 82);
	font_bitmap_find(0x30C1);
	font_bitmap_transfer();
	font_bitmap_display(0, 98);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0050);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0073);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0073);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 4;
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);

	colume_index = 0;
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	ssd1306_set_page_address(3);
	ssd1306_set_column_address(colume_index);
	ssd1306_write_text("SW0");
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0050);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0073);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8) + 1;
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(colume_index);
	ssd1306_write_text("SW0");

	colume_index = 0;
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	colume_index += 4;
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0062);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
}

/**
 * \brief Show the start info on the OLED screen.
 */
void multi_language_show_start_info(void)
{
	/* Clear screen. */
	ssd1306_clear();

	/* Start message on the screen. */
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(0);
	ssd1306_write_text("Start Kit Demo");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x5F00);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x53D1);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x677F);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x6F14);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x793A);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x7A0B);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
	font_bitmap_find(0x5E8F);
	font_bitmap_transfer();
	font_bitmap_display(0, 96);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x958B);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x767A);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x30DC);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x30FC);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x30C9);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x306E);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
	font_bitmap_find(0x30C7);
	font_bitmap_transfer();
	font_bitmap_display(0, 96);
	font_bitmap_find(0x30E2);
	font_bitmap_transfer();
	font_bitmap_display(0, 112);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0043);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x006C);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x007A);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);

	colume_index = 0;
	font_bitmap_find(0x0044);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 8;
	font_bitmap_find(0x004B);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0049);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 8;
	font_bitmap_find(0x0044);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);

	colume_index = 0;
	font_bitmap_find(0x004B);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
}

/**
 * \brief Show the temperature info on the OLED screen.
 */
void multi_language_show_temperature_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("Temperature sensor:");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x6E29);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x5EA6);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x4F20);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x611F);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x5668);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x503C);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x6E29);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x5EA6);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x30BB);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x30F3);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x30B5);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x5024);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0054);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x00E9);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0054);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
#endif
}

/**
 * \brief Show the light info on the OLED screen.
 */
void multi_language_show_light_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("Light sensor:");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x4EAE);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x5EA6);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x4F20);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x611F);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x5668);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x503C);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x660E);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x308B);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x3055);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x30BB);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x30F3);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x30B5);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x004C);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x00E8);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x004C);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x007A);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
#endif
}

/**
 * \brief Show the end info on the OLED screen.
 */
void multi_language_show_end_info(void)
{
	/* Clear screen. */
	ssd1306_clear();

	/* End message on the screen. */
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(0);
	ssd1306_write_text("Wait bootloader complete...");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x7B49);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x5F85);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x5F15);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x5BFC);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x7A0B);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x5E8F);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
	font_bitmap_find(0x5B8C);
	font_bitmap_transfer();
	font_bitmap_display(0, 96);
	font_bitmap_find(0x6210);
	font_bitmap_transfer();
	font_bitmap_display(0, 112);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x30D6);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x30FC);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x30C8);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x30ED);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x30FC);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x30C0);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);

	font_bitmap_find(0x304C);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x5B8C);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	font_bitmap_find(0x4E86);
	font_bitmap_transfer();
	font_bitmap_display(2, 32);
	font_bitmap_find(0x5F85);
	font_bitmap_transfer();
	font_bitmap_display(2, 48);
	font_bitmap_find(0x3061);
	font_bitmap_transfer();
	font_bitmap_display(2, 64);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0041);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0064);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x007A);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);

	colume_index = 0;
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006C);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0045);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0073);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);

	colume_index = 0;
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006C);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
}

/**
 * \brief Show the no SD card info on the OLED screen.
 */
void multi_language_show_no_sd_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("Please insert SD card...");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x8BF7);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x63D2);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x5361);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x30AB);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x30FC);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x30C9);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x3092);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x304F);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	font_bitmap_find(0x3061);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
	font_bitmap_find(0x3055);
	font_bitmap_transfer();
	font_bitmap_display(0, 96);
	font_bitmap_find(0x3044);
	font_bitmap_transfer();
	font_bitmap_display(0, 112);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0073);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x00E9);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	
	colume_index = 0;
	font_bitmap_find(0x006C);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 4;
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0073);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	
	colume_index = 0;
	font_bitmap_find(0x006C);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 4;
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006A);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
}

/**
 * \brief Show the SD card info on the OLED screen.
 */
void multi_language_show_sd_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("SD card information:");
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(0);
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x5361);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x4FE1);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x606F);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(48);
	ssd1306_write_text(":");
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x30AB);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x30FC);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x30C9);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	font_bitmap_find(0x60C5);
	font_bitmap_transfer();
	font_bitmap_display(0, 48);
	font_bitmap_find(0x5831);
	font_bitmap_transfer();
	font_bitmap_display(0, 64);
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(80);
	ssd1306_write_text(":");
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0079);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	colume_index += 4;
	font_bitmap_find(0x0064);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	colume_index += 4;
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(colume_index);
	ssd1306_write_text(":");
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006A);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(colume_index);
	ssd1306_write_text(":");
#endif
}

/**
 * \brief Show normal SD card info on the OLED screen.
 */
void multi_language_show_normal_card_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("- Type: Normal SD card");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x666E);
	font_bitmap_transfer();
	font_bitmap_display(0, 56);
	font_bitmap_find(0x901A);
	font_bitmap_transfer();
	font_bitmap_display(0, 72);
	font_bitmap_find(0x5361);
	font_bitmap_transfer();
	font_bitmap_display(0, 88);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x666E);
	font_bitmap_transfer();
	font_bitmap_display(0, 88);
	font_bitmap_find(0x901A);
	font_bitmap_transfer();
	font_bitmap_display(0, 104);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x004E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006C);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x004E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006C);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
#endif
}

/**
 * \brief Show high capacity SD card info on the OLED screen.
 */
void multi_language_show_high_capacity_card_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("- Type: High Capacity card");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x5927);
	font_bitmap_transfer();
	font_bitmap_display(0, 56);
	font_bitmap_find(0x5BB9);
	font_bitmap_transfer();
	font_bitmap_display(0, 72);
	font_bitmap_find(0x91CF);
	font_bitmap_transfer();
	font_bitmap_display(0, 88);
	font_bitmap_find(0x5361);
	font_bitmap_transfer();
	font_bitmap_display(0, 104);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x5927);
	font_bitmap_transfer();
	font_bitmap_display(0, 88);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0047);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0064);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0047);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0064);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
#endif
}

/**
 * \brief Show unknow card info on the OLED screen.
 */
void multi_language_show_unknow_card_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("- Type: unknown");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x672A);
	font_bitmap_transfer();
	font_bitmap_display(0, 56);
	font_bitmap_find(0x77E5);
	font_bitmap_transfer();
	font_bitmap_display(0, 72);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x4E0D);
	font_bitmap_transfer();
	font_bitmap_display(0, 88);
	font_bitmap_find(0x660E);
	font_bitmap_transfer();
	font_bitmap_display(0, 104);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x00F3);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0067);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
}

/**
 * \brief Show the SD card size info on the OLED screen.
 */
void multi_language_show_card_size_info(char *p_string,
		uint32_t sd_card_size)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_set_page_address(2);
	ssd1306_set_column_address(0);
	
	sprintf(p_string, "- Total size: %lu KB", sd_card_size);
	ssd1306_write_text(p_string);
	
	ssd1306_set_page_address(3);
	ssd1306_set_column_address(0);
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x5361);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x5BB9);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	font_bitmap_find(0x91CF);
	font_bitmap_transfer();
	font_bitmap_display(2, 32);
	
	ssd1306_set_page_address(3);
	ssd1306_set_column_address(48);
	sprintf(p_string, ": %lu KB", sd_card_size);
	ssd1306_write_text(p_string);
	
	/* Wait 3 seconds to show the above message. */
	delay_s(3);
	
	/* Clear screen. */
	ssd1306_clear();
	
	/* scroll one line to show file info */
	font_bitmap_find(0x5361);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x5BB9);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	font_bitmap_find(0x91CF);
	font_bitmap_transfer();
	font_bitmap_display(0, 32);
	
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(48);
	sprintf(p_string, ": %lu KB", sd_card_size);
	ssd1306_write_text(p_string);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x5BB9);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x91CF);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	
	ssd1306_set_page_address(3);
	ssd1306_set_column_address(32);
	sprintf(p_string, ": %lu KB", sd_card_size);
	ssd1306_write_text(p_string);
	
	/* Wait 3 seconds to show the above message. */
	delay_s(3);
	
	/* Clear screen. */
	ssd1306_clear();
	
	/* scroll one line to show file info */
	font_bitmap_find(0x5BB9);
	font_bitmap_transfer();
	font_bitmap_display(0, 0);
	font_bitmap_find(0x91CF);
	font_bitmap_transfer();
	font_bitmap_display(0, 16);
	
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(32);
	sprintf(p_string, ": %lu KB", sd_card_size);
	ssd1306_write_text(p_string);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	/* Wait 1.5 seconds to show the above message. */
	delay_ms(1500);
	
	/* Clear screen. */
	ssd1306_clear();
	
	colume_index = 0;
	font_bitmap_find(0x0043);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x00E9);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	
	ssd1306_set_page_address(2);
	ssd1306_set_column_address(0);
	sprintf(p_string, ": %lu KB", sd_card_size);
	ssd1306_write_text(p_string);
	
	/* Wait 1.5 seconds to show the above message. */
	delay_ms(1500);
	
	/* Clear screen. */
	ssd1306_clear();
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	/* Wait 1.5 seconds to show the above message. */
	delay_ms(1500);
	
	/* Clear screen. */
	ssd1306_clear();
	
	colume_index = 0;
	font_bitmap_find(0x0043);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0064);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0064);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	
	ssd1306_set_page_address(2);
	ssd1306_set_column_address(0);
	sprintf(p_string, ": %lu KB", sd_card_size);
	ssd1306_write_text(p_string);
	
	/* Wait 1.5 seconds to show the above message. */
	delay_ms(1500);
	
	/* Clear screen. */
	ssd1306_clear();
#endif
}

/**
 * \brief Show no fatfs found info on the OLED screen.
 */
void multi_language_show_no_fatfs_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("   <No FAT FS found on SD>");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x65E0);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x6587);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	font_bitmap_find(0x4EF6);
	font_bitmap_transfer();
	font_bitmap_display(2, 32);
	font_bitmap_find(0x7CFB);
	font_bitmap_transfer();
	font_bitmap_display(2, 48);
	font_bitmap_find(0x7EDF);
	font_bitmap_transfer();
	font_bitmap_display(2, 64);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x30CE);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x30FC);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	font_bitmap_find(0x30D5);
	font_bitmap_transfer();
	font_bitmap_display(2, 32);
	font_bitmap_find(0x30A9);
	font_bitmap_transfer();
	font_bitmap_display(2, 48);
	font_bitmap_find(0x30FC);
	font_bitmap_transfer();
	font_bitmap_display(2, 64);
	font_bitmap_find(0x30DE);
	font_bitmap_transfer();
	font_bitmap_display(0, 80);
	font_bitmap_find(0x30C3);
	font_bitmap_transfer();
	font_bitmap_display(0, 96);
	font_bitmap_find(0x30C8);
	font_bitmap_transfer();
	font_bitmap_display(0, 112);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0050);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0073);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 4;
	font_bitmap_find(0x0066);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x00E9);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x004E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 4;
	font_bitmap_find(0x0066);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006D);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0064);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
#endif
}

/**
 * \brief Show no file found info on the OLED screen.
 */
void multi_language_show_no_files_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("		 <no content>");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x6CA1);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x6709);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	font_bitmap_find(0x6587);
	font_bitmap_transfer();
	font_bitmap_display(2, 32);
	font_bitmap_find(0x4EF6);
	font_bitmap_transfer();
	font_bitmap_display(2, 48);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x30CE);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x30FC);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	font_bitmap_find(0x30D5);
	font_bitmap_transfer();
	font_bitmap_display(2, 32);
	font_bitmap_find(0x30A1);
	font_bitmap_transfer();
	font_bitmap_display(2, 48);
	font_bitmap_find(0x30A4);
	font_bitmap_transfer();
	font_bitmap_display(2, 64);
	font_bitmap_find(0x30EB);
	font_bitmap_transfer();
	font_bitmap_display(2, 80);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0040);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 4;
	font_bitmap_find(0x0066);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0068);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x004E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0067);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x00FA);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 4;
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0068);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0076);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
#endif
}

/**
 * \brief Show how to browse the SD card info on the OLED screen.
 */
void multi_language_show_browse_info(void)
{
#ifdef CONF_ENGLISH_LANGUAGE
	ssd1306_write_text("  <Press B2-3 to browse SD>");
#endif
#ifdef CONF_CHINESE_LANGUAGE
	font_bitmap_find(0x4F7F);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x7528);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	font_bitmap_find(0x6309);
	font_bitmap_transfer();
	font_bitmap_display(2, 32);
	font_bitmap_find(0x952E);
	font_bitmap_transfer();
	font_bitmap_display(2, 48);
	font_bitmap_find(0x4E8C);
	font_bitmap_transfer();
	font_bitmap_display(2, 64);
	font_bitmap_find(0x4E09);
	font_bitmap_transfer();
	font_bitmap_display(2, 80);
	font_bitmap_find(0x6D4F);
	font_bitmap_transfer();
	font_bitmap_display(2, 96);
	font_bitmap_find(0x89C8);
	font_bitmap_transfer();
	font_bitmap_display(2, 112);
#endif
#ifdef CONF_JAPANESE_LANGUAGE
	font_bitmap_find(0x4F7F);
	font_bitmap_transfer();
	font_bitmap_display(2, 0);
	font_bitmap_find(0x7528);
	font_bitmap_transfer();
	font_bitmap_display(2, 16);
	font_bitmap_find(0x30DC);
	font_bitmap_transfer();
	font_bitmap_display(2, 32);
	font_bitmap_find(0x30BF);
	font_bitmap_transfer();
	font_bitmap_display(2, 48);
	font_bitmap_find(0x30F3);
	font_bitmap_transfer();
	font_bitmap_display(2, 64);
	font_bitmap_find(0x4E8C);
	font_bitmap_transfer();
	font_bitmap_display(2, 80);
	font_bitmap_find(0x4E09);
	font_bitmap_transfer();
	font_bitmap_display(2, 96);
	font_bitmap_find(0x898B);
	font_bitmap_transfer();
	font_bitmap_display(2, 112);
#endif
#ifdef CONF_FRENCH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0042);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0032);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0033);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	
	colume_index = 0;
	font_bitmap_find(0x00E0);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	colume_index += 4;
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0063);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0075);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0069);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
#ifdef CONF_SPANISH_LANGUAGE
	uint8_t colume_index;
	colume_index = 0;
	font_bitmap_find(0x0042);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006F);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0074);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x00F3);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0032);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0033);
	font_bitmap_transfer();
	font_bitmap_display(0, colume_index);
	
	colume_index = 0;
	font_bitmap_find(0x0070);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	colume_index += 4;	
	font_bitmap_find(0x006E);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0076);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8);
	font_bitmap_find(0x0065);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0067);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0061);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
	colume_index += (font_bitmap_width * 8 - 4);
	font_bitmap_find(0x0072);
	font_bitmap_transfer();
	font_bitmap_display(2, colume_index);
#endif
}

/**
 * \brief Display SD card file name on OLED screen.
 *
 * \param page page address of the OLED.
 * \param string String to display(unicode format).
 */
void multi_language_show_file_name(uint8_t page,
		const char *string)
{
#ifdef CONF_ENGLISH_LANGUAGE
	uint8_t *char_ptr;
	uint32_t i;
#endif
#ifndef CONF_ENGLISH_LANGUAGE
	uint16_t unicode_16;
#endif
	uint8_t unicode_8[2];
	uint8_t column;

	ssd1306_set_page_address(page);
	ssd1306_set_column_address(0);
	ssd1306_write_text("/");

	column = 3;

	unicode_8[0] = *string;
	string++;
	unicode_8[1] = *string;
	string++;

	while (((unicode_8[0] != 0) || (unicode_8[1] != 0)) && (column < 128)) {
#ifdef CONF_ENGLISH_LANGUAGE
		if ((unicode_8[0] < 0x7F) && (unicode_8[1] == 0)) {
			char_ptr = font_table[unicode_8[0] - 32];
			if ((column + char_ptr[0]) < 128) {
				for (i = 1; i <= char_ptr[0]; i++) {
					ssd1306_write_data(char_ptr[i]);
				}
				ssd1306_write_data(0x00);
			}
			column += char_ptr[0];
			column++;
		}
#else
		unicode_16 = unicode_8[0] | (unicode_8[1] << 8);
		font_bitmap_find(unicode_16);
		font_bitmap_transfer();
		if ((column + 8 * font_bitmap_width) < 128) {
			font_bitmap_display(page - 1, column);
		}
		column += (8 * font_bitmap_width);
#endif
		unicode_8[0] = *string;
		string++;
		unicode_8[1] = *string;
		string++;
	}
}

