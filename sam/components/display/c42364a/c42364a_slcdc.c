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
#include "c42364a.h"
#include "conf_c42364a_slcdc.h"
#include "slcdc.h"

void c42364a_init(void)
{
	/* SLCDC configuration */
	struct slcdc_config slcdc_cfg;
	
	slcdc_cfg.com_num = SLCDC_COM_NUM_3;
	slcdc_cfg.seg_num = 39;
	slcdc_cfg.buf_time = SLCDC_BUF_TIME_6;
	slcdc_cfg.bias = SLCDC_BIAS_2;
	slcdc_cfg.is_low_power = false;
	slcdc_cfg.pres = SLCDC_CLK_PRES_2;
	slcdc_cfg.div = SLCDC_CLK_DIV_2;
	slcdc_cfg.disp_mode = SLCDC_DISP_MODE_0;
	slcdc_cfg.blink_freq = 0;
	slcdc_cfg.power_mode = SLCDC_POWER_MODE_2;
	slcdc_cfg.vlotage = SLCDC_VOL_300;
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
	slcdc_display_alphanum_string(data);
}

void c42364a_write_num_packet(const uint8_t *data)
{
	slcdc_display_num_string(data);
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
	slcdc_clear_pixel(icon_com, icon_seg);
}

void c42364a_show_icon(uint8_t icon_com, uint8_t icon_seg)
{
	slcdc_set_pixel(icon_com, icon_seg);
}

void c42364a_blink_icon_start(uint8_t icon_com, uint8_t icon_seg)
{

}

void c42364a_blink_icon_stop(uint8_t icon_com, uint8_t icon_seg)
{

}

void c42364a_blink_screen(void)
{
	slcdc_set_display_mode(SLCDC, SLCDC_DISP_MODE_3);
	slcdc_set_blink_freq(SLCDC, 30);
}

void c42364a_blink_disable(void)
{
	slcdc_set_display_mode(SLCDC, SLCDC_DISP_MODE_0);
}

void c42364a_set_contrast(int8_t contrast)
{

}

void c42364a_circular_animation_start(uint8_t csr_dir,
		uint8_t size, uint8_t data)
{

}

void c42364a_circular_animation_stop(void)
{

}

void c42364a_text_scrolling_start(const uint8_t *data, uint32_t length)
{

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
		slcdc_set_pixel(C42364A_ICON_MINUS);
	} else {
		slcdc_clear_pixel(C42364A_ICON_MINUS);
	}

	value = Abs(value);

	if(value > 9999) {
		value -= 10000;
		slcdc_set_pixel(C42364A_ICON_MINUS_SEG1);
		slcdc_set_pixel(C42364A_ICON_MINUS_SEG2);
	} else {
		slcdc_clear_pixel(C42364A_ICON_MINUS_SEG1);
		slcdc_clear_pixel(C42364A_ICON_MINUS_SEG2);
	}

	sprintf((char*)lcd_num, "%4d", (int)value);

	c42364a_write_num_packet((uint8_t const*)&lcd_num);
}

void c42364a_clear_numeric_dec(void)
{
	slcdc_clear_pixel(C42364A_ICON_MINUS);
	slcdc_clear_pixel(C42364A_ICON_MINUS_SEG1);
	slcdc_clear_pixel(C42364A_ICON_MINUS_SEG2);
	c42364a_write_num_packet("    ");
}

void c42364a_show_battery(enum c42364a_battery_value val)
{
	slcdc_clear_pixel(C42364A_ICON_BAT_LEVEL_1);
	slcdc_clear_pixel(C42364A_ICON_BAT_LEVEL_2);
	slcdc_clear_pixel(C42364A_ICON_BAT_LEVEL_3);
	slcdc_set_pixel(C42364A_ICON_BAT);
	if (val > 2) {
		slcdc_set_pixel(C42364A_ICON_BAT_LEVEL_3);
	}
	if (val > 1) {
		slcdc_set_pixel(C42364A_ICON_BAT_LEVEL_2);
	}
	if (val > 0) {
		slcdc_set_pixel(C42364A_ICON_BAT_LEVEL_1);
	}
}
