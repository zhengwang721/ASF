/**
 * \file
 *
 * \brief SAM DIVAS Driver Quick Start
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
#include <asf.h>

//! [buffer]
const int32_t numerator_s[] = {
	2046, 415, 26, 1, -1, -255, -3798, -65535,
};

const uint32_t numerator_u[] = {
	0x00000001,
	0x0000005A,
	0x000007AB,
	0x00006ABC, 
	0x0004567D,
	0x0093846E, 
	0x20781945, 
	0x7FFFFFFF,
};
//! [buffer]

//! [setup]
//static void cdc_uart_init(void)
//{
	//struct usart_config usart_conf;
//
	///* Configure USART for unit test output */
	//usart_get_config_defaults(&usart_conf);
	//usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	//usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	//usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	//usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	//usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	//usart_conf.baudrate    = CONF_STDIO_BAUDRATE;
//
	//stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART, &usart_conf);
	//usart_enable(&cdc_uart_module);
//}
//! [setup]

//! [calculate]
static void signed_division(void)
{
	int32_t numerator, denominator;
	int32_t result[8];
	uint8_t i;
	
	for(i = 0; i < sizeof(numerator_s) / sizeof(int32_t); i++)
	{
		numerator = numerator_s[i];
		denominator = i+1;
		result[i] = numerator / denominator;
		//printf("%ld / %ld = %ld\n", numerator, denominator, result);
	}
}

static void unsigned_division(void)
{
	uint32_t numerator, denominator;
	uint32_t result[8];
	uint8_t i;
	
	for(i = 0; i < sizeof(numerator_s) / sizeof(int32_t); i++)
	{
		numerator = numerator_u[i];
		denominator = i+1;
		result[i] = numerator / denominator;
		//printf("%lu / %lu = %lu\n", numerator, denominator, result);
	}
}

static void signed_division_mod(void)
{
	int32_t numerator, denominator;
	int32_t result[8];
	uint8_t i;
	
	for(i = 0; i < sizeof(numerator_s) / sizeof(int32_t); i++)
	{
		numerator = numerator_s[i];
		denominator = i+1;
		result[i] = numerator % denominator;
		//printf("%ld % %ld = %ld\n", numerator, denominator, result);
	}
}

static void unsigned_division_mod(void)
{
	uint32_t numerator, denominator;
	uint32_t result[8];
	uint8_t i;
	
	for(i = 0; i < sizeof(numerator_s) / sizeof(int32_t); i++)
	{
		numerator = numerator_u[i];
		denominator = i+1;
		result[i] = numerator % denominator;
		//printf("%lu % %lu = %lu\n", numerator, denominator, result);
	}
}

static void squart_root(void)
{
	uint32_t operator;
	uint32_t result[8];
	uint8_t i;
	
	for(i = 0; i < sizeof(numerator_s) / sizeof(int32_t); i++)
	{
		operator = numerator_u[i];
		result[i] = divas_sqrt(operator);
		//printf("sqrt(%lu) = %lu\n", operator, result);
	}
}
//! [calculate]

int main(void)
{
	//! [setup_init]
	//! [setup_init_1]
	system_init();
	//! [setup_init_1]
	//! [setup_init_2]
	//cdc_uart_init();
	//! [setup_init_2]
	//! [setup_init_3]
	//cpu_irq_enable();
	//! [setup_init_3]
	//! [setup_init]
	
	//! [main]
	//! [main_1]
	signed_division();
	//! [main_1]
	//! [main_2]
	unsigned_division();
	//! [main_2]
	//! [main_3]
	signed_division_mod();
	//! [main_3]
	//! [main_4]
	unsigned_division_mod();
	//! [main_4]
	//! [main_5]
	squart_root();
	//! [main_5]
	
	//! [main_6]
	while (true) {
	}
	//! [main_6]
	//! [main]
}
