/**
 * \file
 *
 * \brief Starter Kit Demo.
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
 *
 * IO1 extension must be connected on EXT2.
 * OLED1 extension must be connected on EXT3.
 *
 */

#include <asf.h>
#include <string.h>
#include "conf_example.h"

#define BUFFER_SIZE 128

struct section_info{
	uint16_t   first;		 /* first character */
	uint16_t   last; 		 /* last character */
	uint32_t   first_address;		/* Address of the SECTION's first char info */
};

struct char_info{
	uint32_t   offset_address : 26;	   //start address of the  font lib
	uint32_t   Width : 6;	   //pixel width
};

volatile uint32_t language_mode = 0;

/* These settings will force to set and refresh the temperature mode. */
volatile uint32_t app_mode = 2;
volatile uint32_t app_mode_switch = 1;

volatile uint32_t sd_update = 0;
volatile uint32_t sd_fs_found = 0;
volatile uint32_t sd_listing_pos = 0;
volatile uint32_t sd_num_files = 0;

FATFS fs;

uint8_t font_bitmap_origin[32];
uint8_t font_bitmap_show[32];

static void font_bitmap_transfer(void)
{
	font_bitmap_show[0] = ((font_bitmap_origin[0]&0x80)>>7) |
			((font_bitmap_origin[2]&0x80)>>6) |
			((font_bitmap_origin[4]&0x80)>>5) |
			((font_bitmap_origin[6]&0x80)>>4) |
			((font_bitmap_origin[8]&0x80)>>3) |
			((font_bitmap_origin[10]&0x80)>>2) |
			((font_bitmap_origin[12]&0x80)>>1) |
			((font_bitmap_origin[14]&0x80)>>0);
	font_bitmap_show[1] = ((font_bitmap_origin[0]&0x40)>>6) |
			((font_bitmap_origin[2]&0x40)>>5) |
			((font_bitmap_origin[4]&0x40)>>4) |
			((font_bitmap_origin[6]&0x40)>>3) |
			((font_bitmap_origin[8]&0x40)>>2) |
			((font_bitmap_origin[10]&0x40)>>1) |
			((font_bitmap_origin[12]&0x40)>>0) |
			((font_bitmap_origin[14]&0x40)<<1);
	font_bitmap_show[2] = ((font_bitmap_origin[0]&0x20)>>5) |
			((font_bitmap_origin[2]&0x20)>>4) |
			((font_bitmap_origin[4]&0x20)>>3) |
			((font_bitmap_origin[6]&0x20)>>2) |
			((font_bitmap_origin[8]&0x20)>>1) |
			((font_bitmap_origin[10]&0x20)>>0) |
			((font_bitmap_origin[12]&0x20)<<1) |
			((font_bitmap_origin[14]&0x20)<<2);
	font_bitmap_show[3] = ((font_bitmap_origin[0]&0x10)>>4) |
			((font_bitmap_origin[2]&0x10)>>3) |
			((font_bitmap_origin[4]&0x10)>>2) |
			((font_bitmap_origin[6]&0x10)>>1) |
			((font_bitmap_origin[8]&0x10)>>0) |
			((font_bitmap_origin[10]&0x10)<<1) |
			((font_bitmap_origin[12]&0x10)<<2) |
			((font_bitmap_origin[14]&0x10)<<3);
	font_bitmap_show[4] = ((font_bitmap_origin[0]&0x08)>>3) |
			((font_bitmap_origin[2]&0x08)>>2) |
			((font_bitmap_origin[4]&0x08)>>1) |
			((font_bitmap_origin[6]&0x08)>>0) |
			((font_bitmap_origin[8]&0x08)<<1) |
			((font_bitmap_origin[10]&0x08)<<2) |
			((font_bitmap_origin[12]&0x08)<<3) |
			((font_bitmap_origin[14]&0x08)<<4);
	font_bitmap_show[5] = ((font_bitmap_origin[0]&0x04)>>2) |
			((font_bitmap_origin[2]&0x04)>>1) |
			((font_bitmap_origin[4]&0x04)>>0) |
			((font_bitmap_origin[6]&0x04)<<1) |
			((font_bitmap_origin[8]&0x04)<<2) |
			((font_bitmap_origin[10]&0x04)<<3) |
			((font_bitmap_origin[12]&0x04)<<4) |
			((font_bitmap_origin[14]&0x04)<<5);
	font_bitmap_show[6] = ((font_bitmap_origin[0]&0x02)>>1) |
			((font_bitmap_origin[2]&0x02)>>0) |
			((font_bitmap_origin[4]&0x02)<<1) |
			((font_bitmap_origin[6]&0x02)<<2) |
			((font_bitmap_origin[8]&0x02)<<3) |
			((font_bitmap_origin[10]&0x02)<<4) |
			((font_bitmap_origin[12]&0x02)<<5) |
			((font_bitmap_origin[14]&0x02)<<6);
	font_bitmap_show[7] = ((font_bitmap_origin[0]&0x01)>>0) |
			((font_bitmap_origin[2]&0x01)<<1) |
			((font_bitmap_origin[4]&0x01)<<2) |
			((font_bitmap_origin[6]&0x01)<<3) |
			((font_bitmap_origin[8]&0x01)<<4) |
			((font_bitmap_origin[10]&0x01)<<5) |
			((font_bitmap_origin[12]&0x01)<<6) |
			((font_bitmap_origin[14]&0x01)<<7);
	font_bitmap_show[8] = ((font_bitmap_origin[1]&0x80)>>7) |
			((font_bitmap_origin[3]&0x80)>>6) |
			((font_bitmap_origin[5]&0x80)>>5) |
			((font_bitmap_origin[7]&0x80)>>4) |
			((font_bitmap_origin[9]&0x80)>>3) |
			((font_bitmap_origin[11]&0x80)>>2) |
			((font_bitmap_origin[13]&0x80)>>1) |
			((font_bitmap_origin[15]&0x80)>>0);
	font_bitmap_show[9] = ((font_bitmap_origin[1]&0x40)>>6) |
			((font_bitmap_origin[3]&0x40)>>5) |
			((font_bitmap_origin[5]&0x40)>>4) |
			((font_bitmap_origin[7]&0x40)>>3) |
			((font_bitmap_origin[9]&0x40)>>2) |
			((font_bitmap_origin[11]&0x40)>>1) |
			((font_bitmap_origin[13]&0x40)>>1) |
			((font_bitmap_origin[15]&0x40)<<1);
	font_bitmap_show[10] = ((font_bitmap_origin[1]&0x20)>>5) |
			((font_bitmap_origin[3]&0x20)>>4) |
			((font_bitmap_origin[5]&0x20)>>3) |
			((font_bitmap_origin[7]&0x20)>>2) |
			((font_bitmap_origin[9]&0x20)>>1) |
			((font_bitmap_origin[11]&0x20)>>0) |
			((font_bitmap_origin[13]&0x20)<<1) |
			((font_bitmap_origin[15]&0x20)<<2);
	font_bitmap_show[11] = ((font_bitmap_origin[1]&0x10)>>4) |
			((font_bitmap_origin[3]&0x10)>>3) |
			((font_bitmap_origin[5]&0x10)>>2) |
			((font_bitmap_origin[7]&0x10)>>1) |
			((font_bitmap_origin[9]&0x10)>>0) |
			((font_bitmap_origin[11]&0x10)<<1) |
			((font_bitmap_origin[13]&0x10)<<2) |
			((font_bitmap_origin[15]&0x10)<<3);
	font_bitmap_show[12] = ((font_bitmap_origin[1]&0x08)>>3) |
			((font_bitmap_origin[3]&0x08)>>2) |
			((font_bitmap_origin[5]&0x08)>>1) |
			((font_bitmap_origin[7]&0x08)>>0) |
			((font_bitmap_origin[9]&0x08)<<1) |
			((font_bitmap_origin[11]&0x08)<<2) |
			((font_bitmap_origin[13]&0x08)<<3) |
			((font_bitmap_origin[15]&0x08)<<4);
	font_bitmap_show[13] = ((font_bitmap_origin[1]&0x04)>>2) |
			((font_bitmap_origin[3]&0x04)>>1) |
			((font_bitmap_origin[5]&0x04)>>0) |
			((font_bitmap_origin[7]&0x04)<<1) |
			((font_bitmap_origin[9]&0x04)<<2) |
			((font_bitmap_origin[11]&0x04)<<3) |
			((font_bitmap_origin[13]&0x04)<<4) |
			((font_bitmap_origin[15]&0x04)<<5);
	font_bitmap_show[14] = ((font_bitmap_origin[1]&0x02)>>1) |
			((font_bitmap_origin[3]&0x02)>>0) |
			((font_bitmap_origin[5]&0x02)<<1) |
			((font_bitmap_origin[7]&0x02)<<2) |
			((font_bitmap_origin[9]&0x02)<<3) |
			((font_bitmap_origin[11]&0x02)<<4) |
			((font_bitmap_origin[13]&0x02)<<5) |
			((font_bitmap_origin[15]&0x02)<<6);
	font_bitmap_show[15] = ((font_bitmap_origin[1]&0x01)>>0) |
			((font_bitmap_origin[3]&0x01)<<1) |
			((font_bitmap_origin[5]&0x01)<<2) |
			((font_bitmap_origin[7]&0x01)<<3) |
			((font_bitmap_origin[9]&0x01)<<4) |
			((font_bitmap_origin[11]&0x01)<<5) |
			((font_bitmap_origin[13]&0x01)<<6) |
			((font_bitmap_origin[15]&0x01)<<7);
	font_bitmap_show[16] = ((font_bitmap_origin[16]&0x80)>>7) |
			((font_bitmap_origin[18]&0x80)>>6) |
			((font_bitmap_origin[20]&0x80)>>5) |
			((font_bitmap_origin[22]&0x80)>>4) |
			((font_bitmap_origin[24]&0x80)>>3) |
			((font_bitmap_origin[26]&0x80)>>2) |
			((font_bitmap_origin[28]&0x80)>>1) |
			((font_bitmap_origin[30]&0x80)>>0);
	font_bitmap_show[17] = ((font_bitmap_origin[16]&0x40)>>6) |
			((font_bitmap_origin[18]&0x40)>>5) |
			((font_bitmap_origin[20]&0x40)>>4) |
			((font_bitmap_origin[22]&0x40)>>3) |
			((font_bitmap_origin[24]&0x40)>>2) |
			((font_bitmap_origin[26]&0x40)>>1) |
			((font_bitmap_origin[28]&0x40)>>0) |
			((font_bitmap_origin[30]&0x40)<<1);
	font_bitmap_show[18] = ((font_bitmap_origin[16]&0x20)>>5) |
			((font_bitmap_origin[18]&0x20)>>4) |
			((font_bitmap_origin[20]&0x20)>>3) |
			((font_bitmap_origin[22]&0x20)>>2) |
			((font_bitmap_origin[24]&0x20)>>1) |
			((font_bitmap_origin[26]&0x20)>>0) |
			((font_bitmap_origin[28]&0x20)<<1) |
			((font_bitmap_origin[30]&0x20)<<2);
	font_bitmap_show[19] = ((font_bitmap_origin[16]&0x10)>>4) |
			((font_bitmap_origin[18]&0x10)>>3) |
			((font_bitmap_origin[20]&0x10)>>2) |
			((font_bitmap_origin[22]&0x10)>>1) |
			((font_bitmap_origin[24]&0x10)>>0) |
			((font_bitmap_origin[26]&0x10)<<1) |
			((font_bitmap_origin[28]&0x10)<<2) |
			((font_bitmap_origin[30]&0x10)<<3);
	font_bitmap_show[20] = ((font_bitmap_origin[16]&0x08)>>3) |
			((font_bitmap_origin[18]&0x08)>>2) |
			((font_bitmap_origin[20]&0x08)>>1) |
			((font_bitmap_origin[22]&0x08)>>0) |
			((font_bitmap_origin[24]&0x08)<<1) |
			((font_bitmap_origin[26]&0x08)<<2) |
			((font_bitmap_origin[28]&0x08)<<3) |
			((font_bitmap_origin[30]&0x08)<<4);
	font_bitmap_show[21] = ((font_bitmap_origin[16]&0x04)>>2) |
			((font_bitmap_origin[18]&0x04)>>1) |
			((font_bitmap_origin[20]&0x04)>>0) |
			((font_bitmap_origin[22]&0x04)<<1) |
			((font_bitmap_origin[24]&0x04)<<2) |
			((font_bitmap_origin[26]&0x04)<<3) |
			((font_bitmap_origin[28]&0x04)<<4) |
			((font_bitmap_origin[30]&0x04)<<5);
	font_bitmap_show[22] = ((font_bitmap_origin[16]&0x02)>>1) |
			((font_bitmap_origin[18]&0x02)>>0) |
			((font_bitmap_origin[20]&0x02)<<1) |
			((font_bitmap_origin[22]&0x02)<<2) |
			((font_bitmap_origin[24]&0x02)<<3) |
			((font_bitmap_origin[26]&0x02)<<4) |
			((font_bitmap_origin[28]&0x02)<<5) |
			((font_bitmap_origin[30]&0x02)<<6);
	font_bitmap_show[23] = ((font_bitmap_origin[16]&0x01)>>0) |
			((font_bitmap_origin[18]&0x01)<<1) |
			((font_bitmap_origin[20]&0x01)<<2) |
			((font_bitmap_origin[22]&0x01)<<3) |
			((font_bitmap_origin[24]&0x01)<<4) |
			((font_bitmap_origin[26]&0x01)<<5) |
			((font_bitmap_origin[28]&0x01)<<6) |
			((font_bitmap_origin[30]&0x01)<<7);
	font_bitmap_show[24] = ((font_bitmap_origin[17]&0x80)>>7) |
			((font_bitmap_origin[19]&0x80)>>6) |
			((font_bitmap_origin[21]&0x80)>>5) |
			((font_bitmap_origin[23]&0x80)>>4) |
			((font_bitmap_origin[25]&0x80)>>3) |
			((font_bitmap_origin[27]&0x80)>>2) |
			((font_bitmap_origin[29]&0x80)>>1) |
			((font_bitmap_origin[31]&0x80)>>0);
	font_bitmap_show[25] = ((font_bitmap_origin[17]&0x40)>>6) |
			((font_bitmap_origin[19]&0x40)>>5) |
			((font_bitmap_origin[21]&0x40)>>4) |
			((font_bitmap_origin[23]&0x40)>>3) |
			((font_bitmap_origin[25]&0x40)>>2) |
			((font_bitmap_origin[27]&0x40)>>1) |
			((font_bitmap_origin[29]&0x40)>>0) |
			((font_bitmap_origin[31]&0x40)<<1);
	font_bitmap_show[26] = ((font_bitmap_origin[17]&0x20)>>5) |
			((font_bitmap_origin[19]&0x20)>>4) |
			((font_bitmap_origin[21]&0x20)>>3) |
			((font_bitmap_origin[23]&0x20)>>2) |
			((font_bitmap_origin[25]&0x20)>>1) |
			((font_bitmap_origin[27]&0x20)>>0) |
			((font_bitmap_origin[29]&0x20)<<1) |
			((font_bitmap_origin[31]&0x20)<<2);
	font_bitmap_show[27] = ((font_bitmap_origin[17]&0x10)>>4) |
			((font_bitmap_origin[19]&0x10)>>3) |
			((font_bitmap_origin[21]&0x10)>>2) |
			((font_bitmap_origin[23]&0x10)>>1) |
			((font_bitmap_origin[25]&0x10)>>0) |
			((font_bitmap_origin[27]&0x10)<<1) |
			((font_bitmap_origin[29]&0x10)<<2) |
			((font_bitmap_origin[31]&0x10)<<3);
	font_bitmap_show[28] = ((font_bitmap_origin[17]&0x08)>>3) |
			((font_bitmap_origin[19]&0x08)>>2) |
			((font_bitmap_origin[21]&0x08)>>1) |
			((font_bitmap_origin[23]&0x08)>>0) |
			((font_bitmap_origin[25]&0x08)<<1) |
			((font_bitmap_origin[27]&0x08)<<2) |
			((font_bitmap_origin[29]&0x08)<<3) |
			((font_bitmap_origin[31]&0x08)<<4);
	font_bitmap_show[29] = ((font_bitmap_origin[17]&0x04)>>2) |
			((font_bitmap_origin[19]&0x04)>>1) |
			((font_bitmap_origin[21]&0x04)>>0) |
			((font_bitmap_origin[23]&0x04)<<1) |
			((font_bitmap_origin[25]&0x04)<<2) |
			((font_bitmap_origin[27]&0x04)<<3) |
			((font_bitmap_origin[29]&0x04)<<4) |
			((font_bitmap_origin[31]&0x04)<<5);
	font_bitmap_show[30] = ((font_bitmap_origin[17]&0x02)>>1) |
			((font_bitmap_origin[19]&0x02)>>0) |
			((font_bitmap_origin[21]&0x02)<<1) |
			((font_bitmap_origin[23]&0x02)<<2) |
			((font_bitmap_origin[25]&0x02)<<3) |
			((font_bitmap_origin[27]&0x02)<<4) |
			((font_bitmap_origin[29]&0x02)<<5) |
			((font_bitmap_origin[31]&0x02)<<6);
	font_bitmap_show[31] = ((font_bitmap_origin[17]&0x01)>>0) |
			((font_bitmap_origin[19]&0x01)<<1) |
			((font_bitmap_origin[21]&0x01)<<2) |
			((font_bitmap_origin[23]&0x01)<<3) |
			((font_bitmap_origin[25]&0x01)<<4) |
			((font_bitmap_origin[27]&0x01)<<5) |
			((font_bitmap_origin[29]&0x01)<<6) |
			((font_bitmap_origin[31]&0x01)<<7);
}

static void font_bitmap_find(uint16_t uni_code)
{
	uint32_t i;

	volatile uint32_t uni_code_info_offset;
	volatile struct char_info *p_uni_code_info;
	volatile uint8_t *p_font_bitmap;

	struct section_info *p_section_info = FONT_BIN_ADDRESS + 0x10;
	uni_code_info_offset = p_section_info->first_address +
			(uni_code - p_section_info->first) * sizeof(struct char_info);
	p_uni_code_info = uni_code_info_offset;
	p_uni_code_info = FONT_BIN_ADDRESS + uni_code_info_offset;
	p_font_bitmap = (uint8_t *)(((uint32_t)(p_uni_code_info->offset_address)) & 0xffffff);
	p_font_bitmap += FONT_BIN_ADDRESS;

	for (i=0; i<32;i++) {
		font_bitmap_origin[i] = *p_font_bitmap;
		p_font_bitmap ++;
	}
}

static void font_bitmap_display(uint8_t page, uint8_t column)
{
	uint32_t i;

	ssd1306_set_page_address(page);
	ssd1306_set_column_address(column);
	for (i = 0; i < 16; i++) {
		ssd1306_write_data(font_bitmap_show[i]);
	}
	ssd1306_set_page_address(page + 1);
	ssd1306_set_column_address(column);
	for (i = 16; i < 32; i++) {
		ssd1306_write_data(font_bitmap_show[i]);
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
	} else if ((uc_button == 2) && (app_mode == 2) &&
			(sd_fs_found == 1) && (sd_update == 0)) {
		/* Page UP button in SD mode. */
		if (sd_listing_pos > 0) {
			sd_listing_pos -= 1;
			sd_update = 1;
		}
	} else if ((uc_button == 3) && (app_mode == 2) &&
			(sd_fs_found == 1) && (sd_update == 0))	{
		/* Page DOWN button in SD mode. */
		/* Lock DOWN button when showing the last file. */
		if (sd_listing_pos < sd_num_files) {
			sd_listing_pos += 1;
			sd_update = 1;
		}
	}
}

/**
 * \brief Handler for SW0 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void sw0_handler(uint32_t id, uint32_t mask)
{
	if ((PIN_SW0_ID == id) && (PIN_SW0_MASK == mask)) {
		language_mode = (language_mode + 1) % 2;
		app_mode_switch = 1;
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

/* IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

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
			OLED1_PIN_PUSHBUTTON_1_MASK, OLED1_PIN_PUSHBUTTON_1_ATTR,
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
			OLED1_PIN_PUSHBUTTON_2_MASK, OLED1_PIN_PUSHBUTTON_2_ATTR,
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
			OLED1_PIN_PUSHBUTTON_3_MASK, OLED1_PIN_PUSHBUTTON_3_ATTR,
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
	DIR dir;
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
	char c_lfn[_MAX_LFN + 1];
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
			pc_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			pc_fn = fno.fname;
#endif
			if (*pc_fn == '.') {
				continue;
			}

			sd_num_files += 1;
		}
	}
}

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

			/* Wait 5 seconds to show the above message. */
			delay_s(5);

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

/**
 * \brief Display text on OLED screen.
 * \param string String to display(unicode format).
 */
static void ssd1306_write_text_unicode(uint8_t page, uint8_t column,
		const char *string)
{
	uint8_t *char_ptr;
	uint8_t i;
	uint8_t unicode_8[2];
	uint16_t unicode_16;

	unicode_8[0] = *string;
	string++;
	unicode_8[1] = *string;
	string++;

	while ((unicode_8[0] != 0) || (unicode_8[1] != 0)){
		if ((unicode_8[0] < 0x7F) && (unicode_8[1] == 0)) {
			char_ptr = font_table[unicode_8[0] - 32];
			column += char_ptr[0];
			for (i = 1; i <= char_ptr[0]; i++) {
				ssd1306_write_data(char_ptr[i]);
			}
			ssd1306_write_data(0x00);
			column++;
		} else if ((unicode_8[1] > 0x20) && (language_mode == 1)) {
			unicode_16 = unicode_8[0] | (unicode_8[1] << 8);
			font_bitmap_find(unicode_16);
			font_bitmap_transfer();
			font_bitmap_display(page - 1, column);
			column += 16;
		}
		unicode_8[0] = *string;
		string++;
		unicode_8[1] = *string;
		string++;
	}
}

/**
 * \brief Show SD card content on the OLED screen in english.
 * \note Does not browse sub folders.
 */
static void display_sd_files_en(void)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
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
	char c_lfn[_MAX_LFN + 1];
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
			pc_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			pc_fn = fno.fname;
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

/**
 * \brief Show SD card content on the OLED screen in chinese.
 * \note Does not browse sub folders.
 */
static void display_sd_files_cn(void)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
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
	char c_lfn[_MAX_LFN + 1];
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
			pc_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			pc_fn = fno.fname;
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

/**
 * \brief Draw graph on the OLED screen using the provided point array.
 * \param col X coordinate.
 * \param page Y coordinate (please refer to OLED datasheet for page description).
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
		page_data[1] = bit_reverse8((bit_data>>8) & 0xFF);
		page_data[2] = bit_reverse8((bit_data>>16) & 0xFF);
		j = height - 1;
		for (page_start = page; page_start < (page + height); ++page_start) {
			ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(page_start));
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

	/* Clear screen. */
	ssd1306_clear();

	ssd1306_set_page_address(1);
	ssd1306_set_column_address(0);
	ssd1306_write_text("Push SW0 to switch language");

	/* Wait 3 seconds to let the user to select language. */
	delay_s(3);

	/* Clear screen. */
	ssd1306_clear();

	/* Start message on the screen. */
	if (language_mode == 0) {
		/* English language */
		ssd1306_set_page_address(1);
		ssd1306_set_column_address(0);
		ssd1306_write_text("Start Kit Demo");
	} else if (language_mode == 1) {
		/* Chinese language */
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
	}

	/* Wait 3 seconds to show the above message. */
	delay_s(3);

	while (true) {
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
				if (language_mode == 0) {
					ssd1306_write_text("Temperature sensor:");
				} else if (language_mode == 1) {
					/* Chinese language */
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
				}
			} else if (app_mode == 1) {
				/* Light mode. */
				ioport_set_pin_level(OLED1_LED2_PIN, OLED1_LED2_ACTIVE);
				ioport_set_pin_level(OLED1_LED1_PIN, !OLED1_LED1_ACTIVE);
				ioport_set_pin_level(OLED1_LED3_PIN, !OLED1_LED3_ACTIVE);
				if (language_mode == 0) {
					ssd1306_write_text("Light sensor:");
				} else if (language_mode == 1) {
					/* Chinese language */
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
				}
			} else {
				/* SD mode. */
				ioport_set_pin_level(OLED1_LED3_PIN, OLED1_LED3_ACTIVE);
				ioport_set_pin_level(OLED1_LED1_PIN, !OLED1_LED1_ACTIVE);
				ioport_set_pin_level(OLED1_LED2_PIN, !OLED1_LED2_ACTIVE);

				sd_listing_pos = 0;
				/* Show SD card info. */
				if (language_mode == 0) {
					display_sd_info_en();
				} else if (language_mode == 1) {
					display_sd_info_cn();
				}

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
			ssd1306_set_column_address(95);
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
			if (light_value < 100)
				ssd1306_clear_char();

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
					if (language_mode == 0) {
						display_sd_info_en();
					} else if (language_mode == 1) {
						display_sd_info_cn();
					}
				} else {
					/* List SD card files. */
					if (language_mode == 0) {
						display_sd_files_en();
					} else if (language_mode == 1) {
						display_sd_files_cn();
					}
				}

				sd_update = 0;
			}

		}

		/* Wait and stop screen flickers. */
		delay_ms(50);
	}
}
