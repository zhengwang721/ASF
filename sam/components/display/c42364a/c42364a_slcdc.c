/**
 * \file
 *
 * \brief Management of C42364A LCD Glass component.
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

#include "compiler.h"
#include "board.h"
#include "c42364a.h"
#include "c42364a_font.h"
#include "conf_c42364a_slcdc.h"
#include "slcdc.h"

/* Symbol pixel array definition */
const enum symbol_pixel Symbol_D0[7] = PATTERN_SYMBOL_D0;
const enum symbol_pixel Symbol_D1[7] = PATTERN_SYMBOL_D1;
const enum symbol_pixel Symbol_D2[7] = PATTERN_SYMBOL_D2;
const enum symbol_pixel Symbol_D3[7] = PATTERN_SYMBOL_D3;

const enum symbol_pixel Symbol_A2[14] = PATTERN_SYMBOL_A2;
const enum symbol_pixel Symbol_A3[14] = PATTERN_SYMBOL_A3;
const enum symbol_pixel Symbol_A4[14] = PATTERN_SYMBOL_A4;
const enum symbol_pixel Symbol_A5[14] = PATTERN_SYMBOL_A5;
const enum symbol_pixel Symbol_A6[14] = PATTERN_SYMBOL_A6;

/* Symbol plot value definition */
const uint8_t Plot_Number[10] = {PLOT_NUMBER_0, PLOT_NUMBER_1, PLOT_NUMBER_2,
		PLOT_NUMBER_3, PLOT_NUMBER_4, PLOT_NUMBER_5, PLOT_NUMBER_6, PLOT_NUMBER_7,
		PLOT_NUMBER_8, PLOT_NUMBER_9};
const uint32_t Plot_Letter[26] = {PLOT_LETTER_A, PLOT_LETTER_B, PLOT_LETTER_C,
		PLOT_LETTER_D, PLOT_LETTER_E, PLOT_LETTER_F, PLOT_LETTER_G, PLOT_LETTER_H,
		PLOT_LETTER_I, PLOT_LETTER_J, PLOT_LETTER_K, PLOT_LETTER_L, PLOT_LETTER_M,
		PLOT_LETTER_N, PLOT_LETTER_O, PLOT_LETTER_P, PLOT_LETTER_Q, PLOT_LETTER_R,
		PLOT_LETTER_S, PLOT_LETTER_T, PLOT_LETTER_U, PLOT_LETTER_V, PLOT_LETTER_W,
		PLOT_LETTER_X, PLOT_LETTER_Y, PLOT_LETTER_Z};
const uint32_t Plot_Letter_Num[10] = {PLOT_LETTER_0, PLOT_LETTER_1, PLOT_LETTER_2,
		PLOT_LETTER_3, PLOT_LETTER_4, PLOT_LETTER_5, PLOT_LETTER_6, PLOT_LETTER_7,
		PLOT_LETTER_8, PLOT_LETTER_9};

/*Each bit mask of uint32 */
static uint32_t mask[32];

static void init_mask(void)
{
	uint32_t tmp = 1;
	uint8_t index = 0;
	for (index =0; index <32; index ++) {
		mask[index] = tmp;
		tmp <<=1;
	}
}

/**
 * \brief Set pixel in LCD display memory.
 *
 * This function sets a pixel in LCD display memory. If a parameter
 * is out of range, then the function doesn't set any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel.
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel.
 */
static void c42364a_slcdc_set_pixel(Slcdc *p_slcdc,
		uint8_t pix_com, uint8_t pix_seg)
{
	if (pix_seg < 32)
		slcdc_set_lsb_memory(p_slcdc, pix_com, mask[pix_seg]);
	else
		slcdc_set_msb_memory(p_slcdc, pix_com, mask[pix_seg & 0x1F]);
}

/**
 * \brief Clear pixel in LCD display memory.
 *
 * This function clears a pixel in LCD display memory. If a parameter
 * is out of range, then the function doesn't clear any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel.
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel.
 */
static void c42364a_slcdc_clear_pixel(Slcdc *p_slcdc,
		uint8_t pix_com, uint8_t pix_seg)
{
	if (pix_seg < 32)
		slcdc_clear_lsb_memory(p_slcdc, pix_com, mask[pix_seg]);
	else
		slcdc_clear_msb_memory(p_slcdc, pix_com, mask[pix_seg & 0x1F]);
}

/**
 * \brief Turn on a symbol according to symbol plot definition.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param p_symbol  Pointer to an symbol_pixel array.
 * \param number Symbol pixel array number.
 * \param plot   Symbol plot definition value
 */
static void c42364a_slcdc_display_symbol(Slcdc *p_slcdc,
		const enum symbol_pixel *p_symbol, uint8_t number, uint32_t plot)
{
	while(number--) {
		if (plot & mask[number])
			c42364a_slcdc_set_pixel(p_slcdc, (*(p_symbol+number)) & 0x03, (*(p_symbol+number)) >> 2);
		else
			c42364a_slcdc_clear_pixel(p_slcdc, (*(p_symbol+number)) & 0x03, (*(p_symbol+number)) >> 2);
	}
}

/**
 * \brief Turn off a symbol.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param p_symbol  Pointer to an symbol_pixel array.
 * \param number Symbol pixel array number.
 */
static void c42364a_slcdc_efface_symbol(Slcdc *p_slcdc,
const enum symbol_pixel *p_symbol, uint8_t number)
{
	while(number --) {
		c42364a_slcdc_clear_pixel(p_slcdc, (*(p_symbol+number)) & 0x03, (*(p_symbol+number)) >> 2);
	}
}

/**
 * \brief Display a string in lcd glass alphanumeric field.
 *
 * This function will write the input string to the alphanumeric field of the
 * lcd glass.
 *
 * \param data Pointer to the input string(max length is 5)
 */
static void c42364a_slcdc_display_alphanum_string(const uint8_t *data)
{
	if(data[0] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_A2, sizeof(Symbol_A2));
	} else if((0x30 < data[0]) && (data[0] < 0x41)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A2, sizeof(Symbol_A2),
				Plot_Letter_Num[data[0] - 0x30]);
	} else if((0x40 < data[0]) && (data[0] < 0x61)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A2, sizeof(Symbol_A2),
				Plot_Letter[data[0] - 0x41]);
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A2, sizeof(Symbol_A2),
				Plot_Letter[data[0] - 0x61]);
	}
	if(data[1] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_A3, sizeof(Symbol_A3));
	} else if((0x30 < data[1]) && (data[1] < 0x41)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A3, sizeof(Symbol_A3),
				Plot_Letter_Num[data[1] - 0x30]);
	} else if((0x40 < data[1]) && (data[1] < 0x61)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A3, sizeof(Symbol_A3),
				Plot_Letter[data[1] - 0x41]);
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A3, sizeof(Symbol_A3),
				Plot_Letter[data[1] - 0x61]);
	}
	if(data[2] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_A4, sizeof(Symbol_A4));
	} else if((0x30 < data[2]) && (data[2] < 0x41)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A4, sizeof(Symbol_A4),
				Plot_Letter_Num[data[2] - 0x30]);
	} else if((0x40 < data[2]) && (data[2] < 0x61)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A4, sizeof(Symbol_A4),
				Plot_Letter[data[2] - 0x41]);
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A4, sizeof(Symbol_A4),
				Plot_Letter[data[2] - 0x61]);
	}
	if(data[3] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_A5, sizeof(Symbol_A5));
	} else if((0x30 < data[3]) && (data[3] < 0x41)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A5, sizeof(Symbol_A5),
				Plot_Letter_Num[data[3] - 0x30]);
	} else if((0x40 < data[3]) && (data[3] < 0x61)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A5, sizeof(Symbol_A5),
				Plot_Letter[data[3] - 0x41]);
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A5, sizeof(Symbol_A5),
				Plot_Letter[data[3] - 0x61]);
	}
	if(data[4] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_A6, sizeof(Symbol_A6));
	} else if((0x30 < data[4]) && (data[4] < 0x41)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A6, sizeof(Symbol_A6),
				Plot_Letter_Num[data[4] - 0x30]);
	} else if((0x40 < data[4]) && (data[4] < 0x61)) {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A6, sizeof(Symbol_A6),
				Plot_Letter[data[4] - 0x41]);
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_A6, sizeof(Symbol_A6),
				Plot_Letter[data[4] - 0x61]);
	}
}

/**
 * \brief Display a string in lcd glass numeric field.
 *
 * This function will write the input string to the numeric field of the
 * lcd glass.
 *
 * \param data Pointer to the input string(max length is 4)
 */
static void c42364a_slcdc_display_num_string(const uint8_t *data)
{
	if(data[0] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_D0, sizeof(Symbol_D0));
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_D0, sizeof(Symbol_D0),
				Plot_Number[data[0] - 0x30]);
	}
	if(data[1] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_D1, sizeof(Symbol_D1));
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_D1, sizeof(Symbol_D1),
				Plot_Number[data[1] - 0x30]);
	}
	if(data[2] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_D2, sizeof(Symbol_D2));
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_D2, sizeof(Symbol_D2),
				Plot_Number[data[2] - 0x30]);
	}
	if(data[3] == 0x20) {
		c42364a_slcdc_efface_symbol(SLCDC, Symbol_D3, sizeof(Symbol_D3));
	} else {
		c42364a_slcdc_display_symbol(SLCDC, Symbol_D3, sizeof(Symbol_D3),
				Plot_Number[data[3] - 0x30]);
	}
}

void c42364a_init(void)
{
	/* SLCDC configuration */
	struct slcdc_config slcdc_cfg;

	init_mask();

	/* SLCDC Reset */
	slcdc_reset(SLCDC);

	/* SLCDC initialization */
	slcdc_cfg.buf_time = SLCDC_BUFTIME_X64_SCLK;
	slcdc_cfg.frame_rate= CONF_C42364A_FRAME_RATE;
	slcdc_cfg.disp_mode = SLCDC_DISPMODE_NORMAL;
	slcdc_cfg.power_mode = SLCDC_POWER_MODE_LCDON_INVR;
	slcdc_cfg.contrast = CONF_C4236A_CONTRAST;
	slcdc_init(SLCDC, &slcdc_cfg);

	/*LCD seg 17, 20~22, and 24 ~49 mapped on SEGx I/O pin */
	slcdc_set_segmap0(SLCDC,0xff720000);
	slcdc_set_segmap1(SLCDC,0x3ffff);

	/* Enable SLCDC */
 	slcdc_enable(SLCDC);
	while(!slcdc_get_status(SLCDC));
}

void c42364a_write_alphanum_packet(const uint8_t *data)
{
	c42364a_slcdc_display_alphanum_string(data);
}

void c42364a_write_num_packet(const uint8_t *data)
{
	c42364a_slcdc_display_num_string(data);
}

void c42364a_show_text(const uint8_t *data)
{
	c42364a_write_alphanum_packet(data);
}

void c42364a_clear_text(void)
{
	c42364a_write_alphanum_packet("     ");
}

void c42364a_clear_icon(uint8_t icon_com, uint8_t icon_seg)
{
	c42364a_slcdc_clear_pixel(SLCDC, icon_com, icon_seg);
}

void c42364a_show_icon(uint8_t icon_com, uint8_t icon_seg)
{
	c42364a_slcdc_set_pixel(SLCDC, icon_com, icon_seg);
}

void c42364a_blink_icon_start(uint8_t icon_com, uint8_t icon_seg)
{
	UNUSED(icon_com);
	UNUSED(icon_seg);
}

void c42364a_blink_icon_stop(uint8_t icon_com, uint8_t icon_seg)
{
	UNUSED(icon_com);
	UNUSED(icon_seg);
}

void c42364a_blink_screen(void)
{
	slcdc_set_display_mode(SLCDC, SLCDC_DISPMODE_BLINKING);
	slcdc_set_blink_freq(SLCDC, CONF_C42364A_BLINK_FREQ, 1);
}

void c42364a_blink_disable(void)
{
	slcdc_set_display_mode(SLCDC, SLCDC_DISPMODE_NORMAL);
}

void c42364a_set_contrast(int8_t contrast)
{
	slcdc_set_contrast(contrast);
}

void c42364a_circular_animation_start(uint8_t csr_dir,
		uint8_t size, uint8_t data)
{
	UNUSED(csr_dir);
	UNUSED(size);
	UNUSED(data);
}

void c42364a_circular_animation_stop(void)
{

}

void c42364a_text_scrolling_start(const uint8_t *data, uint32_t length)
{
	UNUSED(data);
	UNUSED(length);
}

void c42364a_text_scrolling_stop(void)
{

}

void c42364a_show_all(void)
{
	slcdc_set_display_memory(SLCDC);
}

void c42364a_clear_all(void)
{
	slcdc_clear_display_memory(SLCDC);
}

void c42364a_show_numeric_dec(int32_t value)
{
	uint8_t lcd_num[5];

	Assert(value > -20000);
	Assert(value < 20000);

	if(value < 0) {
		c42364a_slcdc_set_pixel(SLCDC, C42364A_ICON_MINUS);
	} else {
		c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_MINUS);
	}

	value = Abs(value);

	if(value > 9999) {
		value -= 10000;
		c42364a_slcdc_set_pixel(SLCDC, C42364A_ICON_MINUS_SEG1);
		c42364a_slcdc_set_pixel(SLCDC, C42364A_ICON_MINUS_SEG2);
	} else {
		c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_MINUS_SEG1);
		c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_MINUS_SEG2);
	}

	sprintf((char*)lcd_num, "%4d", (int)value);

	c42364a_write_num_packet((uint8_t const*)&lcd_num);
}

void c42364a_clear_numeric_dec(void)
{
	c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_MINUS);
	c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_MINUS_SEG1);
	c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_MINUS_SEG2);
	c42364a_write_num_packet("    ");
}

void c42364a_show_battery(enum c42364a_battery_value val)
{
	c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_BAT_LEVEL_1);
	c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_BAT_LEVEL_2);
	c42364a_slcdc_clear_pixel(SLCDC, C42364A_ICON_BAT_LEVEL_3);
	c42364a_slcdc_set_pixel(SLCDC, C42364A_ICON_BAT);
	if (val > 2) {
		c42364a_slcdc_set_pixel(SLCDC, C42364A_ICON_BAT_LEVEL_3);
	}
	if (val > 1) {
		c42364a_slcdc_set_pixel(SLCDC, C42364A_ICON_BAT_LEVEL_2);
	}
	if (val > 0) {
		c42364a_slcdc_set_pixel(SLCDC, C42364A_ICON_BAT_LEVEL_1);
	}
}
