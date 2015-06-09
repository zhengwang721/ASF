/**
 *
 * \file
 *
 * \brief SAM SLCD Quick Start.
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

#include <asf.h>

typedef struct _CHAR_MAP_TB
{
	uint8_t  c;
	uint32_t value;
}CHAR_MAP;

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

#define ATMEL_ICON  0,24

struct usart_module usart_instance;

/**
 *  Configure serial console.
 */
static void configure_console(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate    = 38400;
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &config_usart);
	usart_enable(&usart_instance);
}

int main(void)
{
	struct slcd_config config;

	system_init();
	configure_console();
	delay_init();

	printf("SLCD example starts\r\n");
	slcd_get_config_defaults(&config);
	slcd_init(&config);

	slcd_set_frame_counter(SLCD_FRAME_COUNTER_0,true,2);
	slcd_set_frame_counter(SLCD_FRAME_COUNTER_1,false,3);
	slcd_set_frame_counter(SLCD_FRAME_COUNTER_2,true,3);

	slcd_enable();

	slcd_set_display_memory();
	delay_s(1);
	slcd_clear_display_memory();
	slcd_set_pixel(ATMEL_ICON);

	delay_s(2);
	slcd_disable();
	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,1);
	for(uint32_t i = 0 ; i < 8 ; i++) {
			slcd_character_write_data(0,14+i*2,
	  							charactor_map[i].value,0xffff0000);
	}
	delay_s(2);
	slcd_disable();
	struct slcd_blink_config blink_config;
	slcd_blink_get_config_defaults(&blink_config);
	slcd_blink_set_config(&blink_config);

	for(uint32_t i=0;i<7;i++){
		slcd_set_blink_pixel(i,0);
		slcd_set_blink_pixel(i,1);
	}

	slcd_set_frame_counter(SLCD_FRAME_COUNTER_0,true,0xf10);
	slcd_enable_frame_counter(SLCD_FRAME_COUNTER_0);

	slcd_enable_blink();
	slcd_enable();
    while(1){

    }

}

