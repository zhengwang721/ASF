/**
 * \file
 *
 * \brief Main application of Starter Kit Demo..
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

/**
 * \mainpage Starter Kit Demo
 *
 * \section Purpose
 *
 * The Starter Kit Demo will help new users get familiar with Atmel's
 * SAM family of microcontrollers. This demo features the IO1 and OLED1
 * extension boards for the SAM4 Xplained Pro.
 *
 * \section Requirements
 *
 * This package can be used with SAM Xplained Pro evaluation kits.
 *
 * \section Description
 *
 * The demonstration program can operate in 3 different modes; temperature
 * information, light sensor information and SD card status.
 * The user can switch between the various mode by pressing Button1.
 * When running in mode 3 (SD card content), the user can browse the SD
 * content using Button2 (previous) and Button3 (next). Filenames are directly
 * printed on the OLED screen.
 * If there is other version language program in the SD card, the user can
 * switch to it by push the SW0 button on the Xplain Pro board.
 *
 * IO1 extension must be connected on EXT1.
 * OLED1 extension must be connected on EXT3.
 *
 */

#include <asf.h>
#include <string.h>
#include "conf_example.h"
#include "chinese_font.h"

enum language_mode{
	LANGUAGE_ENGLISH = 0,
	LANGUAGE_CHINESE,
	LANGUAGE_FRENCH,
	LANGUAGE_JAPANESE,
	LANGUAGE_SPANISH,
	LANGUAGE_NUMBER
};

#ifdef CONF_ENGLISH_LANGUAGE
volatile uint32_t app_language = LANGUAGE_ENGLISH;
#endif

#ifdef CONF_CHINESE_LANGUAGE
volatile uint32_t app_language = LANGUAGE_CHINESE;
#endif

#ifdef CONF_FRENCH_LANGUAGE
volatile uint32_t app_language = LANGUAGE_FRENCH;
#endif

#ifdef CONF_JAPANESE_LANGUAGE
volatile uint32_t app_language = LANGUAGE_JAPANESE;
#endif

#ifdef CONF_SPANISH_LANGUAGE
volatile uint32_t app_language = LANGUAGE_SPANISH;
#endif

/* Temperature and light sensor data buffer size. */
#define BUFFER_SIZE 128

/* IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

/* Flag to indicate if the application bin exist: 0 for no, 1 for yes */
volatile uint32_t application_bin_flag = 0;

/* Flag to indicate if need reset: 0 for no, 1 for yes */
volatile uint32_t reset_flag = 0;

/* These settings will force to set and refresh the temperature mode. */
volatile uint32_t app_mode = 2;
volatile uint32_t app_mode_switch = 1;

/* SD related variants. */
volatile uint32_t sd_update = 0;
volatile uint32_t sd_fs_found = 0;
volatile uint32_t sd_listing_pos = 0;
volatile uint32_t sd_num_files = 0;

/* Fatfs global variants. */
FATFS fs;
DIR dir;
FIL file_object;
/* Include the path at begin. */
TCHAR file_name_unicode[5][14] = {
	{0x0030, 0x003A, 0x0064, 0x0065, 0x006D, 0x006F, 0x005F,
	0x0065, 0x006E, 0x002E, 0x0062, 0x0069, 0x006E, 0x0000},
	{0x0030, 0x003A, 0x0064, 0x0065, 0x006D, 0x006F, 0x005F,
	0x0063, 0x006E, 0x002E, 0x0062, 0x0069, 0x006E, 0x0000},
	{0x0030, 0x003A, 0x0064, 0x0065, 0x006D, 0x006F, 0x005F,
	0x0067, 0x0072, 0x002E, 0x0062, 0x0069, 0x006E, 0x0000},
	{0x0030, 0x003A, 0x0064, 0x0065, 0x006D, 0x006F, 0x005F,
	0x006A, 0x0070, 0x002E, 0x0062, 0x0069, 0x006E, 0x0000},
	{0x0030, 0x003A, 0x0064, 0x0065, 0x006D, 0x006F, 0x005F,
	0x0073, 0x0070, 0x002E, 0x0062, 0x0069, 0x006E, 0x0000}
};
/* Only name. */
uint8_t file_name_ascii[5][12] = {
	{0x64, 0x65, 0x6D, 0x6F, 0x5F, 0x65, 0x6E, 0x2E, 0x62, 0x69, 0x6E, 0x00},
	{0x64, 0x65, 0x6D, 0x6F, 0x5F, 0x63, 0x6E, 0x2E, 0x62, 0x69, 0x6E, 0x00},
	{0x64, 0x65, 0x6D, 0x6F, 0x5F, 0x67, 0x72, 0x2E, 0x62, 0x69, 0x6E, 0x00},
	{0x64, 0x65, 0x6D, 0x6F, 0x5F, 0x6A, 0x70, 0x2E, 0x62, 0x69, 0x6E, 0x00},
	{0x64, 0x65, 0x6D, 0x6F, 0x5F, 0x73, 0x70, 0x2E, 0x62, 0x69, 0x6E, 0x00}
};

volatile uint32_t file_exist_flag = 0;

/** Size of the data to write/read. */
#define DATA_SIZE 512

/* Read/write buffer */
static uint8_t data_buffer[DATA_SIZE];

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
static void multi_language_show_switch_info(void)
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
 * \brief Check the font bin file.
 */
static void application_bin_check(void)
{
	/* Check if the application bin in SD card */
	uint32_t i;
	uint8_t card_check;
	FRESULT res;
	TCHAR path[3];

	/* Init the path to 0: */
	path[0] = 0x0030;
	path[1] = 0x003A;
	path[2] = 0x0000;

	/* Is SD card present? */
	if (gpio_pin_is_low(SD_MMC_0_CD_GPIO) == false) {
		return;
	}

	sd_mmc_init();
	card_check = sd_mmc_check(0);
	while (card_check != SD_MMC_OK) {
		card_check = sd_mmc_check(0);
		delay_ms(1);
	}

	/* Try to mount file system. */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
	if (FR_INVALID_DRIVE == res) {
		return;
	}

	/* Test if the disk is formatted */
	res = f_opendir(&dir, path);
	if (res != FR_OK) {
		return;
	}

	for (i = 0; i < LANGUAGE_NUMBER; i++) {
		/* Open the application bin file. */
		res = f_open(&file_object, file_name_unicode[i],
				(FA_OPEN_EXISTING | FA_READ));
		if (res != FR_OK) {
			break;
		}

		/* Close the file*/
		res = f_close(&file_object);
		if (res != FR_OK) {
			break;
		}

		/* Set the exist flag */
		file_exist_flag |= (0x01 << i);
	}

	if ((file_exist_flag & 0x1F) == 0) {
		return;
	}

	/* Show switch info */
	multi_language_show_switch_info();

	/* Wait 6 seconds to show above info. */
	delay_s(6);

	/* Set the flag. */
	application_bin_flag = 1;

}

/**
 * \brief Show the end info on the OLED screen.
 */
static void multi_language_show_end_info(void)
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
 * \brief Handler for reset to bootloader.
 */
static void reset_handler(void)
{
	uint32_t ul_last_page_addr = LAST_PAGE_ADDRESS;
	uint32_t *pul_last_page = (uint32_t *) ul_last_page_addr;
	uint32_t ul_rc;
	uint32_t ul_idx;
	uint32_t temp_data;
	uint32_t i;

	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		temp_data = pul_last_page[ul_idx];
		data_buffer[ul_idx * 4] = (uint8_t)(temp_data & 0xFF);
		data_buffer[ul_idx * 4 + 1] = (uint8_t)((temp_data >> 8) & 0xFF);
		data_buffer[ul_idx * 4 + 2] = (uint8_t)((temp_data >> 16) & 0xFF);
		data_buffer[ul_idx * 4 + 3] = (uint8_t)((temp_data >> 24) & 0xFF);
	}

	/* Trigger */
	data_buffer[0x14] = 0x01;
	data_buffer[0x15] = 0x00;
	data_buffer[0x16] = 0x00;
	data_buffer[0x17] = 0x00;

	for (i = 0; i < LANGUAGE_NUMBER; i++) {
		app_language++;
		app_language = app_language % LANGUAGE_NUMBER;
		/* Check the exist flag */
		if (file_exist_flag & (0x01 << app_language)) {
			data_buffer[0x18] = file_name_ascii[app_language][0];
			data_buffer[0x19] = file_name_ascii[app_language][1];
			data_buffer[0x1A] = file_name_ascii[app_language][2];
			data_buffer[0x1B] = file_name_ascii[app_language][3];
			data_buffer[0x1C] = file_name_ascii[app_language][4];
			data_buffer[0x1D] = file_name_ascii[app_language][5];
			data_buffer[0x1E] = file_name_ascii[app_language][6];
			data_buffer[0x1F] = file_name_ascii[app_language][7];
			data_buffer[0x20] = file_name_ascii[app_language][8];
			data_buffer[0x21] = file_name_ascii[app_language][9];
			data_buffer[0x22] = file_name_ascii[app_language][10];
			data_buffer[0x23] = file_name_ascii[app_language][11];
			break;
		}
	}

	ul_rc = flash_unlock(ul_last_page_addr,
			ul_last_page_addr + IFLASH_PAGE_SIZE, NULL, NULL);

	ul_rc = flash_erase_page(ul_last_page_addr, IFLASH_ERASE_PAGES_8);
	if (ul_rc != FLASH_RC_OK) {
		return;
	}

	ul_rc = flash_write(ul_last_page_addr, data_buffer,
			IFLASH_PAGE_SIZE, 0);
	if (ul_rc != FLASH_RC_OK) {
		return;
	}

	/* Clear screen. */
	ssd1306_clear();

	/* End message on the screen. */
	multi_language_show_end_info();

	/* Perform the software reset. */
	rstc_start_software_reset(RSTC);

	/* Wait for reset. */
	delay_ms(100);
}

/**
 * \brief Handler for SW0 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void sw0_handler(uint32_t id, uint32_t mask)
{
	if ((PIN_SW0_ID == id) && (PIN_SW0_MASK == mask) &&
			(application_bin_flag == 1)) {
		reset_flag = 1;
	}
}

/**
 * \brief Process Buttons Events.
 *
 * \param uc_button The button number.
 */
static void process_button_event(uint8_t uc_button)
{
	/* Switch between temperature, light and SD mode. */
	if (uc_button == 1) {
		app_mode_switch = 1;
		pio_disable_interrupt(OLED1_PIN_PUSHBUTTON_1_PIO,
			OLED1_PIN_PUSHBUTTON_1_MASK);
	} else if ((uc_button == 2) && (app_mode == 2) &&
			(sd_fs_found == 1) && (sd_update == 0)) {
		/* Page UP button in SD mode. */
		if (sd_listing_pos > 0) {
			sd_listing_pos -= 1;
			sd_update = 1;
			pio_disable_interrupt(OLED1_PIN_PUSHBUTTON_2_PIO,
			OLED1_PIN_PUSHBUTTON_2_MASK);
		}
	} else if ((uc_button == 3) && (app_mode == 2) &&
			(sd_fs_found == 1) && (sd_update == 0)) {
		/* Page DOWN button in SD mode. */
		/* Lock DOWN button when showing the last file. */
		if (sd_listing_pos < sd_num_files) {
			sd_listing_pos += 1;
			sd_update = 1;
			pio_disable_interrupt(OLED1_PIN_PUSHBUTTON_3_PIO,
			OLED1_PIN_PUSHBUTTON_3_MASK);
		}
	}
}

/**
 * \brief Handler for Button 1 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void button1_handler(uint32_t id, uint32_t mask)
{
	if ((OLED1_PIN_PUSHBUTTON_1_ID == id) &&
			(OLED1_PIN_PUSHBUTTON_1_MASK == mask)) {
		process_button_event(1);
	}
}

/**
 * \brief Handler for Button 2 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void button2_handler(uint32_t id, uint32_t mask)
{
	if ((OLED1_PIN_PUSHBUTTON_2_ID == id) &&
			(OLED1_PIN_PUSHBUTTON_2_MASK == mask)) {
		process_button_event(2);
	}
}

/**
 * \brief Handler for Button 3 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void button3_handler(uint32_t id, uint32_t mask)
{
	if ((OLED1_PIN_PUSHBUTTON_3_ID == id) &&
			(OLED1_PIN_PUSHBUTTON_3_MASK == mask)) {
		process_button_event(3);
	}
}

/**
 * \brief Handler for SD card detect rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void sd_detect_handler(uint32_t id, uint32_t mask)
{
	if ((SD_MMC_0_CD_ID == id) && (SD_MMC_0_CD_MASK == mask)) {
		sd_listing_pos = 0;
		sd_num_files = 0;
		sd_fs_found = 0;
		sd_update = 1;
	}
}

/**
 * \brief Configure the Pushbuttons.
 *
 * Configure the PIO as inputs and generate corresponding interrupt when
 * pressed or released.
 */
static void configure_buttons(void)
{
	/* Configure SW0. */
	pmc_enable_periph_clk(PIN_SW0_ID);
	pio_set_debounce_filter(PIN_SW0_PIO, PIN_SW0_MASK, 10);
	pio_handler_set(PIN_SW0_PIO, PIN_SW0_ID,
			PIN_SW0_MASK, PIN_SW0_ATTR, sw0_handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_SW0_ID);
	pio_handler_set_priority(PIN_SW0_PIO, (IRQn_Type)PIN_SW0_ID,
			IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_SW0_PIO, PIN_SW0_MASK);

	/* Configure Pushbutton 1. */
	pmc_enable_periph_clk(OLED1_PIN_PUSHBUTTON_1_ID);
	pio_set_debounce_filter(OLED1_PIN_PUSHBUTTON_1_PIO,
			OLED1_PIN_PUSHBUTTON_1_MASK, 10);
	pio_handler_set(OLED1_PIN_PUSHBUTTON_1_PIO, OLED1_PIN_PUSHBUTTON_1_ID,
			OLED1_PIN_PUSHBUTTON_1_MASK,
			OLED1_PIN_PUSHBUTTON_1_ATTR,
			button1_handler);
	NVIC_EnableIRQ((IRQn_Type)OLED1_PIN_PUSHBUTTON_1_ID);
	pio_handler_set_priority(OLED1_PIN_PUSHBUTTON_1_PIO,
			(IRQn_Type)OLED1_PIN_PUSHBUTTON_1_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(OLED1_PIN_PUSHBUTTON_1_PIO,
			OLED1_PIN_PUSHBUTTON_1_MASK);

	/* Configure Pushbutton 2. */
	pmc_enable_periph_clk(OLED1_PIN_PUSHBUTTON_2_ID);
	pio_set_debounce_filter(OLED1_PIN_PUSHBUTTON_2_PIO,
			OLED1_PIN_PUSHBUTTON_2_MASK, 10);
	pio_handler_set(OLED1_PIN_PUSHBUTTON_2_PIO, OLED1_PIN_PUSHBUTTON_2_ID,
			OLED1_PIN_PUSHBUTTON_2_MASK,
			OLED1_PIN_PUSHBUTTON_2_ATTR,
			button2_handler);
	NVIC_EnableIRQ((IRQn_Type)OLED1_PIN_PUSHBUTTON_2_ID);
	pio_handler_set_priority(OLED1_PIN_PUSHBUTTON_2_PIO,
			(IRQn_Type)OLED1_PIN_PUSHBUTTON_2_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(OLED1_PIN_PUSHBUTTON_2_PIO,
			OLED1_PIN_PUSHBUTTON_2_MASK);

	/* Configure Pushbutton 3. */
	pmc_enable_periph_clk(OLED1_PIN_PUSHBUTTON_3_ID);
	pio_set_debounce_filter(OLED1_PIN_PUSHBUTTON_3_PIO,
			OLED1_PIN_PUSHBUTTON_3_MASK, 10);
	pio_handler_set(OLED1_PIN_PUSHBUTTON_3_PIO, OLED1_PIN_PUSHBUTTON_3_ID,
			OLED1_PIN_PUSHBUTTON_3_MASK,
			OLED1_PIN_PUSHBUTTON_3_ATTR,
			button3_handler);
	NVIC_EnableIRQ((IRQn_Type)OLED1_PIN_PUSHBUTTON_3_ID);
	pio_handler_set_priority(OLED1_PIN_PUSHBUTTON_3_PIO,
			(IRQn_Type)OLED1_PIN_PUSHBUTTON_3_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(OLED1_PIN_PUSHBUTTON_3_PIO,
			OLED1_PIN_PUSHBUTTON_3_MASK);

	/* Configure SD card detection. */
	pmc_enable_periph_clk(SD_MMC_0_CD_ID);
	pio_set_debounce_filter(SD_MMC_0_CD_PIO, SD_MMC_0_CD_MASK, 10);
	pio_handler_set(SD_MMC_0_CD_PIO, SD_MMC_0_CD_ID, SD_MMC_0_CD_MASK,
			SD_MMC_0_CD_ATTR, sd_detect_handler);
	NVIC_EnableIRQ((IRQn_Type)SD_MMC_0_CD_ID);
	pio_handler_set_priority(SD_MMC_0_CD_PIO, (IRQn_Type)SD_MMC_0_CD_ID,
			IRQ_PRIOR_PIO);
	pio_enable_interrupt(SD_MMC_0_CD_PIO, SD_MMC_0_CD_MASK);
}

/**
 * \brief Configure the ADC for the light sensor.
 */
static void configure_adc(void)
{
	struct adc_config adc_cfg;

	/* Configure ADC pin for light sensor. */
	gpio_configure_pin(LIGHT_SENSOR_GPIO, LIGHT_SENSOR_FLAGS);

	/* Enable ADC clock. */
	pmc_enable_periph_clk(ID_ADC);

	/* Configure ADC. */
	adc_enable();
	adc_get_config_defaults(&adc_cfg);
	adc_init(ADC, &adc_cfg);
	adc_channel_enable(ADC, ADC_CHANNEL_0);
	adc_set_trigger(ADC, ADC_TRIG_SW);
}

/**
 * \brief Get the number of files at the root of the SD card.
 * Result is stored in global sd_num_files.
 */
static void get_num_files_on_sd(void)
{
	FRESULT res;
	FILINFO fno;
	char *pc_fn;
	TCHAR path[3];
#if _LFN_UNICODE
	path[0] = 0x0030;
	path[1] = 0x003A;
	path[2] = 0x0000;
#else
	path[0] = '0';
	path[1] = ':';
	path[2] = 0x00;
#endif

#if _USE_LFN
	TCHAR c_lfn[_MAX_LFN + 1];
	fno.lfname = c_lfn;
	fno.lfsize = sizeof(c_lfn);
#endif

	sd_num_files = 0;

	/* Open the directory */
	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) {
				break;
			}

#if _USE_LFN
			pc_fn = (char *)(*fno.lfname ? fno.lfname : fno.fname);
#else
			pc_fn = (char *)(fno.fname);
#endif
			if (*pc_fn == '.') {
				continue;
			}

			sd_num_files += 1;
		}
	}
}

#ifdef CONF_ENGLISH_LANGUAGE
/**
 * \brief Show SD card status on the OLED screen in english.
 */
static void display_sd_info_en(void)
{
	FRESULT res;
	uint8_t card_check;
	uint8_t sd_card_type;
	uint32_t sd_card_size;
	char size[64];

	/* Is SD card present? */
	if (gpio_pin_is_low(SD_MMC_0_CD_GPIO) == false) {
		ssd1306_write_text("Please insert SD card...");
	} else {
		ssd1306_write_text("SD card information:");

		sd_mmc_init();
		card_check = sd_mmc_check(0);
		while (card_check != SD_MMC_OK) {
			card_check = sd_mmc_check(0);
			delay_ms(1);
		}

		if (card_check == SD_MMC_OK) {
			sd_card_type = sd_mmc_get_type(0);
			sd_card_size = sd_mmc_get_capacity(0);

			ssd1306_set_page_address(1);
			ssd1306_set_column_address(0);

			/* Card type */
			switch (sd_card_type) {
			case CARD_TYPE_SD:
				ssd1306_write_text("- Type: Normal SD card");
				break;

			case CARD_TYPE_SDIO:
				ssd1306_write_text("- Type: SDIO card");
				break;

			case CARD_TYPE_HC:
				ssd1306_write_text("- Type: High Capacity card");
				break;

			case CARD_TYPE_SD_COMBO:
				ssd1306_write_text("- Type: SDIO/Memory card");
				break;

			default:
				ssd1306_write_text("- Type: unknown");
			}

			ssd1306_set_page_address(2);
			ssd1306_set_column_address(0);

			sprintf(size, "- Total size: %lu KB", sd_card_size);
			ssd1306_write_text(size);

			ssd1306_set_page_address(3);
			ssd1306_set_column_address(0);

			/* Try to mount file system. */
			memset(&fs, 0, sizeof(FATFS));
			res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
			if (FR_INVALID_DRIVE == res) {
				ssd1306_write_text("   <No FAT FS found on SD>");
				sd_fs_found = 0;
			} else {
				get_num_files_on_sd();
				if (sd_num_files == 0) {
					ssd1306_write_text("         <no content>");
					sd_fs_found = 1;
				} else {
					ssd1306_write_text("  <Press B2-3 to browse SD>");
					sd_fs_found = 1;
				}
			}
		}
	}
}
#endif

#ifdef CONF_CHINESE_LANGUAGE
/**
 * \brief Show SD card status on the OLED screen in chinese.
 */
static void display_sd_info_cn(void)
{
	FRESULT res;
	uint8_t card_check;
	uint8_t sd_card_type;
	uint32_t sd_card_size;
	char size[64];

	/* Is SD card present? */
	if (gpio_pin_is_low(SD_MMC_0_CD_GPIO) == false) {
		font_bitmap_find(0x8BF7);
		font_bitmap_transfer();
		font_bitmap_display(0, 0);
		font_bitmap_find(0x63D2);
		font_bitmap_transfer();
		font_bitmap_display(0, 16);
		font_bitmap_find(0x5361);
		font_bitmap_transfer();
		font_bitmap_display(0, 32);
	} else {
		font_bitmap_find(0x5361);
		font_bitmap_transfer();
		font_bitmap_display(0, 0);
		font_bitmap_find(0x4FE1);
		font_bitmap_transfer();
		font_bitmap_display(0, 16);
		font_bitmap_find(0x606F);
		font_bitmap_transfer();
		font_bitmap_display(0, 32);

		sd_mmc_init();
		card_check = sd_mmc_check(0);
		while (card_check != SD_MMC_OK) {
			card_check = sd_mmc_check(0);
			delay_ms(1);
		}

		if (card_check == SD_MMC_OK) {
			sd_card_type = sd_mmc_get_type(0);
			sd_card_size = sd_mmc_get_capacity(0);

			ssd1306_set_page_address(1);
			ssd1306_set_column_address(48);
			ssd1306_write_text(":");

			/* Card type */
			switch (sd_card_type) {
			case CARD_TYPE_SD:
				font_bitmap_find(0x666E);
				font_bitmap_transfer();
				font_bitmap_display(0, 56);
				font_bitmap_find(0x901A);
				font_bitmap_transfer();
				font_bitmap_display(0, 72);
				font_bitmap_find(0x5361);
				font_bitmap_transfer();
				font_bitmap_display(0, 88);
				break;

			case CARD_TYPE_SDIO:
				break;

			case CARD_TYPE_HC:
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
				break;

			case CARD_TYPE_SD_COMBO:
				break;

			default:
				font_bitmap_find(0x672A);
				font_bitmap_transfer();
				font_bitmap_display(0, 56);
				font_bitmap_find(0x77E5);
				font_bitmap_transfer();
				font_bitmap_display(0, 72);
			}

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
			sprintf(size, ": %lu KB", sd_card_size);
			ssd1306_write_text(size);

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
			sprintf(size, ": %lu KB", sd_card_size);
			ssd1306_write_text(size);

			/* Try to mount file system. */
			memset(&fs, 0, sizeof(FATFS));
			res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
			if (FR_INVALID_DRIVE == res) {
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

				sd_fs_found = 0;
			} else {
				get_num_files_on_sd();
				if (sd_num_files == 0) {
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

					sd_fs_found = 1;
				} else {
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

					sd_fs_found = 1;
				}
			}
		}
	}
}
#endif

#ifdef CONF_JAPANESE_LANGUAGE
/**
 * \brief Show SD card status on the OLED screen in chinese.
 */
static void display_sd_info_jp(void)
{
	FRESULT res;
	uint8_t card_check;
	uint8_t sd_card_type;
	uint32_t sd_card_size;
	char size[64];

	/* Is SD card present? */
	if (gpio_pin_is_low(SD_MMC_0_CD_GPIO) == false) {
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
	} else {
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

		sd_mmc_init();
		card_check = sd_mmc_check(0);
		while (card_check != SD_MMC_OK) {
			card_check = sd_mmc_check(0);
			delay_ms(1);
		}

		if (card_check == SD_MMC_OK) {
			sd_card_type = sd_mmc_get_type(0);
			sd_card_size = sd_mmc_get_capacity(0);

			ssd1306_set_page_address(1);
			ssd1306_set_column_address(80);
			ssd1306_write_text(":");

			/* Card type */
			switch (sd_card_type) {
			case CARD_TYPE_SD:
				font_bitmap_find(0x666E);
				font_bitmap_transfer();
				font_bitmap_display(0, 88);
				font_bitmap_find(0x901A);
				font_bitmap_transfer();
				font_bitmap_display(0, 104);
				break;

			case CARD_TYPE_SDIO:
				break;

			case CARD_TYPE_HC:
				font_bitmap_find(0x5927);
				font_bitmap_transfer();
				font_bitmap_display(0, 88);
				break;

			case CARD_TYPE_SD_COMBO:
				break;

			default:
				font_bitmap_find(0x4E0D);
				font_bitmap_transfer();
				font_bitmap_display(0, 88);
				font_bitmap_find(0x660E);
				font_bitmap_transfer();
				font_bitmap_display(0, 104);
			}

			font_bitmap_find(0x5BB9);
			font_bitmap_transfer();
			font_bitmap_display(2, 0);
			font_bitmap_find(0x91CF);
			font_bitmap_transfer();
			font_bitmap_display(2, 16);

			ssd1306_set_page_address(3);
			ssd1306_set_column_address(32);
			sprintf(size, ": %lu KB", sd_card_size);
			ssd1306_write_text(size);

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
			sprintf(size, ": %lu KB", sd_card_size);
			ssd1306_write_text(size);

			/* Try to mount file system. */
			memset(&fs, 0, sizeof(FATFS));
			res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
			if (FR_INVALID_DRIVE == res) {
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

				sd_fs_found = 0;
			} else {
				get_num_files_on_sd();
				if (sd_num_files == 0) {
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

					sd_fs_found = 1;
				} else {
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

					sd_fs_found = 1;
				}
			}
		}
	}
}
#endif

#ifdef CONF_FRENCH_LANGUAGE
/**
 * \brief Show SD card status on the OLED screen in chinese.
 */
static void display_sd_info_fr(void)
{
	FRESULT res;
	uint8_t card_check;
	uint8_t sd_card_type;
	uint32_t sd_card_size;
	char size[64];
	uint8_t colume_index;

	/* Is SD card present? */
	if (gpio_pin_is_low(SD_MMC_0_CD_GPIO) == false) {
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
	} else {
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

		sd_mmc_init();
		card_check = sd_mmc_check(0);
		while (card_check != SD_MMC_OK) {
			card_check = sd_mmc_check(0);
			delay_ms(1);
		}

		if (card_check == SD_MMC_OK) {
			sd_card_type = sd_mmc_get_type(0);
			sd_card_size = sd_mmc_get_capacity(0);

			ssd1306_set_page_address(1);
			ssd1306_set_column_address(colume_index);
			ssd1306_write_text(":");

			/* Card type */
			switch (sd_card_type) {
			case CARD_TYPE_SD:
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
				break;

			case CARD_TYPE_SDIO:
				break;

			case CARD_TYPE_HC:
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
				break;

			case CARD_TYPE_SD_COMBO:
				break;

			default:
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
			}

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
			sprintf(size, ": %lu KB", sd_card_size);
			ssd1306_write_text(size);

			/* Wait 1.5 seconds to show the above message. */
			delay_ms(1500);

			/* Clear screen. */
			ssd1306_clear();

			/* Try to mount file system. */
			memset(&fs, 0, sizeof(FATFS));
			res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
			if (FR_INVALID_DRIVE == res) {
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

				sd_fs_found = 0;
			} else {
				get_num_files_on_sd();
				if (sd_num_files == 0) {
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

					sd_fs_found = 1;
				} else {
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

					sd_fs_found = 1;
				}
			}
		}
	}
}
#endif

#ifdef CONF_SPANISH_LANGUAGE
/**
 * \brief Show SD card status on the OLED screen in chinese.
 */
static void display_sd_info_sp(void)
{
	FRESULT res;
	uint8_t card_check;
	uint8_t sd_card_type;
	uint32_t sd_card_size;
	char size[64];
	uint8_t colume_index;

	/* Is SD card present? */
	if (gpio_pin_is_low(SD_MMC_0_CD_GPIO) == false) {
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
	} else {
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

		sd_mmc_init();
		card_check = sd_mmc_check(0);
		while (card_check != SD_MMC_OK) {
			card_check = sd_mmc_check(0);
			delay_ms(1);
		}

		if (card_check == SD_MMC_OK) {
			sd_card_type = sd_mmc_get_type(0);
			sd_card_size = sd_mmc_get_capacity(0);

			ssd1306_set_page_address(1);
			ssd1306_set_column_address(colume_index);
			ssd1306_write_text(":");

			/* Card type */
			switch (sd_card_type) {
			case CARD_TYPE_SD:
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
				break;

			case CARD_TYPE_SDIO:
				break;

			case CARD_TYPE_HC:
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
				break;

			case CARD_TYPE_SD_COMBO:
				break;

			default:
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
			}

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
			sprintf(size, ": %lu KB", sd_card_size);
			ssd1306_write_text(size);

			/* Wait 1.5 seconds to show the above message. */
			delay_ms(1500);

			/* Clear screen. */
			ssd1306_clear();

			/* Try to mount file system. */
			memset(&fs, 0, sizeof(FATFS));
			res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
			if (FR_INVALID_DRIVE == res) {
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

				sd_fs_found = 0;
			} else {
				get_num_files_on_sd();
				if (sd_num_files == 0) {
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

					sd_fs_found = 1;
				} else {
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

					sd_fs_found = 1;
				}
			}
		}
	}
}
#endif

/**
 * \brief Display text on OLED screen.
 * \param string String to display(unicode format).
 */
static void ssd1306_write_text_unicode(uint8_t page, uint8_t column,
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

#ifdef CONF_ENGLISH_LANGUAGE
/**
 * \brief Show SD card content on the OLED screen in english.
 * \note Does not browse sub folders.
 */
static void display_sd_files_en(void)
{
	FRESULT res;
	FILINFO fno;
	uint32_t line;
	uint32_t pos;
	char *pc_fn;
	TCHAR path[3];
#if _LFN_UNICODE
	path[0] = 0x0030;
	path[1] = 0x003A;
	path[2] = 0x0000;
#else
	path[0] = '0';
	path[1] = ':';
	path[2] = 0x00;
#endif

#if _USE_LFN
	TCHAR c_lfn[_MAX_LFN + 1];
	fno.lfname = c_lfn;
	fno.lfsize = sizeof(c_lfn);
#endif

	line = 0;
	pos = 1;

	/* Open the directory */
	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) {
				break;
			}

#if _USE_LFN
			pc_fn = (char *)(*fno.lfname ? fno.lfname : fno.fname);
#else
			pc_fn = (char *)(fno.fname);
#endif
			if (*pc_fn == '.') {
				continue;
			}

#if _LFN_UNICODE
			if ((pos >= sd_listing_pos) && (line < 4)) {
				ssd1306_set_page_address(line);
				ssd1306_set_column_address(0);
				ssd1306_write_text("/");
				ssd1306_write_text_unicode(line, 3, pc_fn);
				line++;
			}

#else
			if ((pos >= sd_listing_pos) && (line < 4)) {
				ssd1306_set_page_address(line++);
				ssd1306_set_column_address(0);
				ssd1306_write_text("/");
				ssd1306_write_text(pc_fn);
			}

#endif
			pos += 1;
		}
	}
}
#else
/**
 * \brief Show SD card content on the OLED screen in chinese.
 * \note Does not browse sub folders.
 */
static void display_sd_files_unicode(void)
{
	FRESULT res;
	FILINFO fno;
	uint32_t line;
	uint32_t pos;
	char *pc_fn;
	TCHAR path[3];
#if _LFN_UNICODE
	path[0] = 0x0030;
	path[1] = 0x003A;
	path[2] = 0x0000;
#else
	path[0] = '0';
	path[1] = ':';
	path[2] = 0x00;
#endif

#if _USE_LFN
	TCHAR c_lfn[_MAX_LFN + 1];
	fno.lfname = c_lfn;
	fno.lfsize = sizeof(c_lfn);
#endif

	line = 1;
	pos = 1;

	/* Open the directory */
	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) {
				break;
			}

#if _USE_LFN
			pc_fn = (char *)(*fno.lfname ? fno.lfname : fno.fname);
#else
			pc_fn = (char *)(fno.fname);
#endif
			if (*pc_fn == '.') {
				continue;
			}

			if ((pos >= sd_listing_pos) && (line < 4)) {
				ssd1306_set_page_address(line);
				ssd1306_set_column_address(0);
				ssd1306_write_text("/");
				ssd1306_write_text_unicode(line, 3, pc_fn);
				line += 2;
			}

			pos += 1;
		}
	}
}
#endif

/**
 * \brief Draw graph on the OLED screen using the provided point array.
 * \param col X coordinate.
 * \param page Y coordinate (please refer to OLED datasheet for page
 * description).
 * \param width Graph width(columns).
 * \param height Graph height(pages, 1~3).
 * \param tab Data to draw. Must contain width elements.
 */
static void ssd1306_draw_graph(uint8_t col, uint8_t page, uint8_t width,
		uint8_t height, uint8_t *tab)
{
	uint8_t i, j;
	uint8_t page_start, scale, bit_length, page_data[3];
	uint32_t bit_data;

	for (i = col; i < width; ++i) {
		scale = 8 * height;
		bit_length = tab[i] * scale / 24;
		for (bit_data = 0; bit_length > 0; --bit_length) {
			bit_data = (bit_data << 1) + 1;
		}
		page_data[0] = bit_reverse8(bit_data & 0xFF);
		page_data[1] = bit_reverse8((bit_data >> 8) & 0xFF);
		page_data[2] = bit_reverse8((bit_data >> 16) & 0xFF);
		j = height - 1;
		for (page_start = page; page_start < (page + height); ++page_start) {
			ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(
					page_start));
			ssd1306_set_column_address(i);
			ssd1306_write_data(page_data[j]);
			--j;
		}
	}
}

/**
 * \brief Clear one character at the cursor current position on the OLED
 * screen.
 */
static void ssd1306_clear_char(void)
{
	ssd1306_write_data(0x00);
	ssd1306_write_data(0x00);
	ssd1306_write_data(0x00);
	ssd1306_write_data(0x00);
	ssd1306_write_data(0x00);
	ssd1306_write_data(0x00);
}

/**
 * \brief Show the start info on the OLED screen.
 */
static void multi_language_show_start_info(void)
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
static void multi_language_show_temperature_info(void)
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
static void multi_language_show_light_info(void)
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
 * \brief The main application.
 */
int main(void)
{
	uint8_t i;
	uint8_t temperature[BUFFER_SIZE];
	uint8_t light[BUFFER_SIZE];
	char value_disp[5];
	uint32_t adc_value;
	uint32_t light_value;
	double temp;

	/* Initialize clocks. */
	sysclk_init();

	/* Initialize GPIO states. */
	board_init();

	/* Configure ADC for light sensor. */
	configure_adc();

	/* Initialize at30tse. */
	at30tse_init();

	/* Configure IO1 buttons. */
	configure_buttons();

	/* Initialize SPI and SSD1306 controller. */
	ssd1306_init();
	ssd1306_clear();

	/* Clear internal buffers. */
	for (i = 0; i < BUFFER_SIZE; ++i) {
		temperature[i] = 0;
		light[i] = 0;
	}

	/* Show the start info. */
	multi_language_show_start_info();

	/* Wait 3 seconds to show the above message. */
	delay_s(3);

	/* Check application bin in SD card. */
	application_bin_check();

	while (true) {
		/* Refresh page title only if necessary. */
		if (reset_flag) {
			reset_handler();
		}

		/* Refresh page title only if necessary. */
		if (app_mode_switch > 0) {
			app_mode = (app_mode + 1) % 3;

			/* Clear screen. */
			ssd1306_clear();
			ssd1306_set_page_address(0);
			ssd1306_set_column_address(0);

			if (app_mode == 0) {
				/* Temperature mode. */
				ioport_set_pin_level(OLED1_LED1_PIN, OLED1_LED1_ACTIVE);
				ioport_set_pin_level(OLED1_LED2_PIN, !OLED1_LED2_ACTIVE);
				ioport_set_pin_level(OLED1_LED3_PIN, !OLED1_LED3_ACTIVE);
				multi_language_show_temperature_info();
			} else if (app_mode == 1) {
				/* Light mode. */
				ioport_set_pin_level(OLED1_LED2_PIN, OLED1_LED2_ACTIVE);
				ioport_set_pin_level(OLED1_LED1_PIN, !OLED1_LED1_ACTIVE);
				ioport_set_pin_level(OLED1_LED3_PIN, !OLED1_LED3_ACTIVE);
				multi_language_show_light_info();
			} else {
				/* SD mode. */
				ioport_set_pin_level(OLED1_LED3_PIN, OLED1_LED3_ACTIVE);
				ioport_set_pin_level(OLED1_LED1_PIN, !OLED1_LED1_ACTIVE);
				ioport_set_pin_level(OLED1_LED2_PIN, !OLED1_LED2_ACTIVE);

				sd_listing_pos = 0;
				/* Show SD card info. */
#ifdef CONF_ENGLISH_LANGUAGE
				display_sd_info_en();
#endif
#ifdef CONF_CHINESE_LANGUAGE
				display_sd_info_cn();
#endif
#ifdef CONF_JAPANESE_LANGUAGE
				display_sd_info_jp();
#endif
#ifdef CONF_FRENCH_LANGUAGE
				display_sd_info_fr();
#endif
#ifdef CONF_SPANISH_LANGUAGE
				display_sd_info_sp();
#endif
			}

			app_mode_switch = 0;
		}

		/* Shift graph buffers. */
		for (i = 0; i < (BUFFER_SIZE - 1); ++i) {
			temperature[i] = temperature[i + 1];
			light[i] = light[i + 1];
		}

		/* Get temperature. */
		if (at30tse_read_temperature(&temp) == TWI_SUCCESS) {
			/* Don't care about negative temperature. */
			if (temp < 0) {
				temp = 0;
			}

			/* Update temperature for display. */
			/* Note: rescale to 0~24 for better rendering. */
			if (temp > 40) {
				temperature[BUFFER_SIZE - 1] = 24;
			} else {
				temperature[BUFFER_SIZE - 1] = temp * 24 / 40;
			}
		} else {
			/* Error print zero values. */
			temperature[BUFFER_SIZE - 1] = 0;
		}

		/* Get light sensor information. */
		/* Rescale to 0~24 for better rendering. */
		adc_start_software_conversion(ADC);
		adc_value = adc_channel_get_value(ADC, ADC_CHANNEL_0);
		light[BUFFER_SIZE - 1] = 24 - adc_value * 24 / 1024;

		if (app_mode == 0) {
			/* Print temperature in text format. */
			sprintf(value_disp, "%d", (uint8_t)temp);
			ssd1306_set_column_address(98);
			ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(0));
			ssd1306_write_text(" ");
			/* Avoid character overlapping. */
			if (temp < 10) {
				ssd1306_clear_char();
			}

			ssd1306_write_text(value_disp);
			/* Display degree symbol. */
			ssd1306_write_data(0x06);
			ssd1306_write_data(0x06);
			ssd1306_write_text("c");

			/* Refresh graph. */
			ssd1306_draw_graph(0, 2, BUFFER_SIZE, 2, temperature);
		} else if (app_mode == 1) {
			light_value = 100 - (adc_value * 100 / 1024);
			sprintf(value_disp, "%lu", light_value);
			ssd1306_set_column_address(98);
			ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(0));
			ssd1306_write_text(" ");
			/* Avoid character overlapping. */
			if (light_value < 10) {
				ssd1306_clear_char();
			}

			ssd1306_write_text(value_disp);
			ssd1306_write_text("%");
			/* Avoid character overlapping. */
			if (light_value < 100) {
				ssd1306_clear_char();
			}

			/* Refresh graph. */
			ssd1306_draw_graph(0, 2, BUFFER_SIZE, 2, light);
		} else {
			/**
			 * Refresh screen if card was inserted/removed or
			 * browsing content.
			 */
			if (sd_update == 1) {
				/* Clear screen. */
				ssd1306_clear();
				ssd1306_set_page_address(0);
				ssd1306_set_column_address(0);

				if (sd_listing_pos == 0) {
					/* Show SD card info. */
#ifdef CONF_ENGLISH_LANGUAGE
					display_sd_info_en();
#endif
#ifdef CONF_CHINESE_LANGUAGE
					display_sd_info_cn();
#endif
#ifdef CONF_JAPANESE_LANGUAGE
					display_sd_info_jp();
#endif
#ifdef CONF_FRENCH_LANGUAGE
					display_sd_info_fr();
#endif
#ifdef CONF_SPANISH_LANGUAGE
					display_sd_info_sp();
#endif
				} else {
					/* List SD card files. */
#ifdef CONF_ENGLISH_LANGUAGE
					display_sd_files_en();
#else
					display_sd_files_unicode();
#endif
				}

				sd_update = 0;
			}
		}

		/* Wait and stop screen flickers. */
		delay_ms(150);

		if (app_mode_switch == 0) {
			pio_enable_interrupt(OLED1_PIN_PUSHBUTTON_1_PIO,
			OLED1_PIN_PUSHBUTTON_1_MASK);
		}
		if (sd_update == 0) {
			pio_enable_interrupt(OLED1_PIN_PUSHBUTTON_2_PIO,
			OLED1_PIN_PUSHBUTTON_2_MASK);
			pio_enable_interrupt(OLED1_PIN_PUSHBUTTON_3_PIO,
			OLED1_PIN_PUSHBUTTON_3_MASK);
		}

	}
}
