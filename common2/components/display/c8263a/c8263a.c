/**
 * \file
 *
 * \brief Management of C8263A LCD Glass component.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "slcd.h"
#include "c8263a.h"
#include "config_c8263a.h"
#include <string.h>

const CHAR_MAP charactor_map[] = {
	{'0',0x0},{'1',0x0},{'2',0x0},{'3',0x0},{'4',0x0},{'5',0x0},{'6',0x0},
	{'7',0x0},{'8',0x0},{'9',0x0},
	{'A',0x0},{'B',0x0},{'C',0x0},{'D',0x0},{'E',0x0},{'F',0x0},{'G',0x0},
	{'H',0x0},{'I',0x0},{'J',0x0},{'K',0x0},{'L',0x0},{'M',0x0},{'N',0x0},
	{'O',0x0},{'P',0x0},{'Q',0x0},{'R',0x0},{'S',0x0},{'T',0x0},
	{'U',0x0},{'B',0x0},{'W',0x0},{'X',0x0},{'Y',0x0},{'Z',0x0},
	{'a',0x0},{'b',0x0},{'c',0x0},{'d',0x0},{'e',0x0},{'f',0x0},{'g',0x0},
	{'h',0x0},{'i',0x0},{'j',0x0},{'k',0x0},{'l',0x0},{'m',0x0},{'n',0x0},
	{'o',0x0},{'p',0x0},{'q',0x0},{'r',0x0},{'s',0x0},{'t',0x0},
	{'u',0x0},{'v',0x0},{'w',0x0},{'x',0x0},{'y',0x0},{'z',0x0},{255,0}
};
		
const CHAR_MAP charactor_map_dec[]= {
		{'0',0x0},{'1',0x0},{'2',0x0},{'3',0x0},{'4',0x0},{'5',0x0},{'6',0x0},
		{'7',0x0},{'8',0x0},{'9',0x0},
		{'A',0x0},{'B',0x0},{'C',0x0},{'D',0x0},{'E',0x0},{'F',0x0},{'G',0x0},
		{'H',0x0},{'I',0x0},{'J',0x0},{'K',0x0},{'L',0x0},{'M',0x0},{'N',0x0},
		{'O',0x0},{'P',0x0},{'Q',0x0},{'R',0x0},{'S',0x0},{'T',0x0},
		{'U',0x0},{'B',0x0},{'W',0x0},{'X',0x0},{'Y',0x0},{'Z',0x0},
		{'a',0x0},{'b',0x0},{'c',0x0},{'d',0x0},{'e',0x0},{'f',0x0},{'g',0x0},
		{'h',0x0},{'i',0x0},{'j',0x0},{'k',0x0},{'l',0x0},{'m',0x0},{'n',0x0},
		{'o',0x0},{'p',0x0},{'q',0x0},{'r',0x0},{'s',0x0},{'t',0x0},
		{'u',0x0},{'v',0x0},{'w',0x0},{'x',0x0},{'y',0x0},{'z',0x0},{255,0}
};

#define CHAR_LEN (sizeof(charactor_map)/sizeof(CHAR_MAP))

const uint32_t num_map[10] = {0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234};
const uint32_t num_map_dec[10] = {0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234};		


status_code_t c8263a_init(void)
{
	struct slcd_config config;

	slcd_get_config_defaults(&config);
	slcd_init(&config);

	slcd_set_frame_counter(SLCD_FRAME_COUNTER_0,false,CONF_C8263A_FC0);
	slcd_set_frame_counter(SLCD_FRAME_COUNTER_1,false,CONF_C8263A_FC0);
	slcd_set_frame_counter(SLCD_FRAME_COUNTER_2,false,CONF_C8263A_FC0);
	slcd_set_contrast(CONF_C8263A_CONTRAST);

	return STATUS_OK;
}

void c8263a_show_all(void)
{
	slcd_disable();
	slcd_disable_blink();
	slcd_disable_circular_shift();
	slcd_set_display_memory();
	slcd_enable();
}

void c8263a_clear_all(void)
{
	slcd_disable_blink();
	slcd_disable_circular_shift();
	slcd_clear_display_memory();
}

void c8263a_show_icon(uint8_t icon_com, uint8_t icon_seg)
{
	slcd_set_pixel(icon_com, icon_seg);
}

void c8263a_clear_icon(uint8_t icon_com, uint8_t icon_seg)
{
	slcd_clear_pixel(icon_com, icon_seg);
}

void c8263a_blink_icon_start(uint8_t icon_com, uint8_t icon_seg)
{
	if (icon_seg < 2) {
		slcd_disable();
		struct slcd_blink_config blink_config;
		slcd_blink_get_config_defaults(&blink_config);
		blink_config.blink_all_seg = false;
		blink_config.fc = CONF_C8263A_BLINK_TIMER;
		slcd_blink_set_config(&blink_config);
		slcd_set_blink_pixel(icon_com, icon_seg);
		slcd_enable_frame_counter(CONF_C8263A_BLINK_TIMER);
		slcd_enable_blink();
		slcd_enable();
	}

}

void c8263a_blink_icon_stop(uint8_t icon_com, uint8_t icon_seg)
{
	if (icon_seg < 2) {
		slcd_disable_blink();
		slcd_disable();
		slcd_clear_blink_pixel(icon_com, icon_seg);
		slcd_enable_blink();
		slcd_enable();
	}
}

void c8263a_blink_screen(void)
{
	slcd_disable_blink();
	slcd_disable();
	struct slcd_blink_config blink_config;
	slcd_blink_get_config_defaults(&blink_config);
	blink_config.blink_all_seg = true;
	blink_config.fc = CONF_C8263A_BLINK_TIMER;
	slcd_blink_set_config(&blink_config);

	slcd_enable_frame_counter(CONF_C8263A_BLINK_TIMER);
	slcd_enable_blink();
	slcd_enable();
}

void c8263a_blink_disable(void)
{
	slcd_disable_frame_counter(CONF_C8263A_BLINK_TIMER);
	slcd_disable_blink();
}

void c8263a_set_contrast(uint8_t contrast)
{
	slcd_set_contrast(contrast);
}

void c8263a_circular_animation_start(uint8_t csr_dir,
		uint8_t size, uint8_t data)
{

	struct slcd_circular_shift_config cfg;
	slcd_disable();
	slcd_circular_shift_get_config_defaults(&cfg);
	cfg.data = data;
	cfg.size = size;
	cfg.dir = csr_dir;
	cfg.fc = CONF_C8263A_CIRCULAR_ANIMATION_TIMER;
	slcd_circular_shift_set_config(&cfg);
	slcd_enable_circular_shift();
	slcd_enable_frame_counter(SLCD_FRAME_COUNTER_1);
	slcd_enable();

}

void c8263a_circular_animation_stop(void)
{
	slcd_disable_frame_counter(CONF_C8263A_CIRCULAR_ANIMATION_TIMER);
	slcd_disable_circular_shift();
}

void c8263a_show_battery(enum c8263a_battery_value val)
{
	if (val <= C8263A_BATTERY_THREE )
	{
		slcd_clear_pixel(C8263A_ICON_BAT_LEVEL_1);
		slcd_clear_pixel(C8263A_ICON_BAT_LEVEL_2);
		slcd_clear_pixel(C8263A_ICON_BAT_LEVEL_3);
		slcd_set_pixel(C8263A_ICON_BAT);
		switch (val) {
			case C8263A_BATTERY_THREE:
				slcd_set_pixel(C8263A_ICON_BAT_LEVEL_3);
			case C8263A_BATTERY_TWO:
				slcd_set_pixel(C8263A_ICON_BAT_LEVEL_2);
			case C8263A_BATTERY_ONE:
				slcd_set_pixel(C8263A_ICON_BAT_LEVEL_1);
				break;
			case C8263A_BATTERY_NONE:
				break;
		}
	}
}

void c8263a_show_wirless(enum c8263a_wireless_value val)
{
	slcd_clear_pixel(C8263A_ICON_WIRELESS_LEVEL_1);
	slcd_clear_pixel(C8263A_ICON_WIRELESS_LEVEL_2);
	slcd_clear_pixel(C8263A_ICON_WIRELESS_LEVEL_3);
	slcd_set_pixel(C8263A_ICON_WIRELESS);
	switch (val) {
		case C8263A_WIRELESS_THREE:
			slcd_set_pixel(C8263A_ICON_WIRELESS_LEVEL_3);
		case C8263A_WIRELESS_TWO:
			slcd_set_pixel(C8263A_ICON_WIRELESS_LEVEL_2);
		case C8263A_WIRELESS_ONE:
			slcd_set_pixel(C8263A_ICON_WIRELESS_LEVEL_1);
			break;
		case C8263A_WIRELESS_NONE:
			break;
	}
}
static uint32_t c8263a_get_char(uint8_t c)
{
	
	for(uint32_t i=0; i < CHAR_LEN; i++){
		if (charactor_map[i].c == c) {
			return charactor_map[i].value;
		}
	}

	return 0;
}
static uint32_t c8263a_get_char_dec(uint8_t c)
{
	for(uint32_t i=0; i < CHAR_LEN; i++){
		if (charactor_map_dec[i].c == c) {
			return charactor_map_dec[i].value;
		}
	}
	return 0;
}

void c8263a_show_time(const struct c8263a_time val)
{
	slcd_clear_pixel(C8263A_ICON_AM);
	slcd_clear_pixel(C8263A_ICON_PM);
	slcd_set_pixel(C8263A_ICON_TIME_DOT);
	if (val.pm) {
		slcd_set_pixel(C8263A_ICON_PM);
	} else {
		slcd_set_pixel(C8263A_ICON_AM);
	}
	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,0);
	if(val.hour >=10) {
		slcd_character_write_data(0,C8263A_HOUR_SEG_S,
	  							num_map[val.hour/10],0xffff0000);
		slcd_character_write_data(0,C8263A_HOUR_SEG_S+2,
	  							num_map[val.hour%10],0xffff0000);
	} else {
		slcd_character_write_data(0,C8263A_HOUR_SEG_S+2,
	  							val.hour,0xffff0000);
	}

	slcd_character_write_data(0,C8263A_MIN_SEG_S+4,
	  							num_map[val.minute/10],0xffff0000);
	slcd_character_write_data(0,C8263A_MIN_SEG_S+6,
	  							num_map[val.minute%10],0xffff0000);
	
}

void c8263a_show_temperature(float tmp,bool is_celsius)
{

	is_celsius ? 
	slcd_set_pixel(C8263A_ICON_DEGREE_C):slcd_set_pixel(C8263A_ICON_DEGREE_F);

	c8263a_show_numeric_float(tmp);
}

void c8263a_clear_numeric_dec(void)
{
	slcd_clear_pixel(C8263A_ICON_MINUS);
	slcd_clear_pixel(C8263A_ICON_MINUS_SEG1);
	slcd_clear_pixel(C8263A_ICON_MINUS_SEG2);

	slcd_clear_pixel(C8263A_ICON_DOT_1);
	slcd_clear_pixel(C8263A_ICON_DOT_2);
	slcd_clear_pixel(C8263A_ICON_DOT_3);
	slcd_clear_pixel(C8263A_ICON_DOT_4);

	slcd_clear_pixel(C8263A_ICON_TIME_DOT);

	slcd_clear_pixel(C8263A_ICON_AM);
	slcd_clear_pixel(C8263A_ICON_PM);
	slcd_clear_pixel(C8263A_ICON_DEGREE_C);
	slcd_clear_pixel(C8263A_ICON_DEGREE_F);
	slcd_clear_pixel(C8263A_ICON_VOLT);
	slcd_clear_pixel(C8263A_ICON_AMP);
	slcd_clear_pixel(C8263A_ICON_MILLI_VOLT);
	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,0);
	for(uint32_t i = 0 ; i < C8263A_MAX_NUM ; i++) {
	  slcd_character_write_data(0,C8263A_NUM_SEG_INDEX_S+i*2,
	  							0,0xffff0000);
	}
}

void c8263a_show_numeric_dec(int32_t value)
{
	uint32_t tmp=0;
	uint8_t i=0;
	Assert(value > -200000);
	Assert(value < 200000);

	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,0);

	if(value < 0) {
		slcd_set_pixel(C8263A_ICON_MINUS);
	} else {
		slcd_clear_pixel(C8263A_ICON_MINUS);
	}

	tmp = Abs(value);

	if (tmp > 99999) {
		slcd_set_pixel(C8263A_ICON_MINUS_SEG2);
		tmp -= 100000;
	} else {
		slcd_clear_pixel(C8263A_ICON_MINUS_SEG2);
	}

	while(tmp != 0 && i <= C8263A_MAX_NUM){
		slcd_character_write_data(0,C8263A_NUM_SEG_INDEX_S+i*2,num_map[tmp%10],0xffff0000);
		tmp /=10;
		i++;
	}

}

void c8263a_show_numeric_float(float value)
{
	uint8_t lcd_num[C8263A_MAX_NUM];
	float d=0;
	uint32_t tmp=0,i=0,n=0;
	uint8_t flag = 0;

	Assert(value > -200000);
	Assert(value < 200000);

	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,0);

	if(value < 0) {
		slcd_set_pixel(C8263A_ICON_MINUS);
	} else {
		slcd_clear_pixel(C8263A_ICON_MINUS);
	}

	d = Abs(value);

	if (tmp > 99999) {
		slcd_set_pixel(C8263A_ICON_MINUS_SEG2);
		tmp -= 100000;
		flag = 1;
	} else {
		slcd_clear_pixel(C8263A_ICON_MINUS_SEG2);
	}

	tmp = d ;
	d = d - tmp;
	while(tmp != 0 && n <= C8263A_MAX_NUM){
		lcd_num[n] = tmp%10;
		tmp /=10;
		n++;
	}

	if(d > 0) {
		switch(n) {
			case 0:
				if(flag) {
					slcd_set_pixel(C8263A_ICON_DOT_1);
				}
				break;
			case 1:
				slcd_set_pixel(C8263A_ICON_DOT_2);
				break;
			case 2:
				slcd_set_pixel(C8263A_ICON_DOT_3);
				break;
			case 3:
				slcd_set_pixel(C8263A_ICON_DOT_4);
				break;
			case 4:
				slcd_set_pixel(C8263A_ICON_DOT_5);
				break;
		}

	}

	while(d > 0 && n <= C8263A_MAX_NUM)
	{
		d= d*10;
		lcd_num[n] = d;
		d= d - lcd_num[n];
		n++;
	}

	for(i=0; i<n; i++) {
		if(lcd_num[i] <=9) {
			slcd_character_write_data(0,C8263A_NUM_SEG_INDEX_E-2*i,
									num_map[lcd_num[i]],0xffff0000);
		}
	}
}

void c8263a_show_text(const uint8_t *data)
{
	Assert(data);
	uint32_t len = (uint32_t)strlen((char *) data);
	
	len = (len > C8263A_MAX_CHAR) ? C8263A_MAX_CHAR : len;
	
	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,1);
	for(uint32_t i = 0 ; i < len ; i++) {
			slcd_character_write_data(0,C8263A_TXT_SEG_INDEX_S+i*2,
	  							c8263a_get_char(data[i]),0xffff0000);  
	}

}
void c8263a_clear_text(void)
{
	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,1);
	for(uint32_t i = 0 ; i < C8263A_MAX_CHAR ; i++) {
	  slcd_character_write_data(0,C8263A_TXT_SEG_INDEX_S+i*2,
	  							0,0xffff0000);
	}
}
