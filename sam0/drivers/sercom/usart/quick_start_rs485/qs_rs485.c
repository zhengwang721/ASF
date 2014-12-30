/**
 * \file
 *
 * \brief SAM USART RS485 Quick Start
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
#include <stdio.h>
//! [module_var]

//! [module_inst]
static struct usart_module cdc_instance,rs485_instance;
//! [module_inst]

//! [rs485_buffer]
#define RS485_SENDING_MSG_INTERVAL 2
const static uint8_t tx_buffer[] = "Sending from RS485\r\n";
static uint8_t rx_buffer[24]={0};
//! [rs485_buffer]

//! [module_var]

//! [setup]

//! [CDC_setup]
static void configure_usart_cdc(void)
{
	struct usart_config config_cdc;
	usart_get_config_defaults(&config_cdc);
	config_cdc.baudrate	 = 38400;
	config_cdc.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_cdc.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_cdc.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_cdc.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_cdc.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	stdio_serial_init(&cdc_instance, EDBG_CDC_MODULE, &config_cdc);
	usart_enable(&cdc_instance);
}
//! [CDC_setup]

//! [rs485_setup]
static void rs485_read_callback(const struct usart_module *const usart_module)
{
	if (usart_read_buffer_wait(&rs485_instance,
			(uint8_t *)rx_buffer, sizeof(tx_buffer)) == STATUS_OK) {
		printf("Received :%s\r\n",rx_buffer);
	}
}

static void configure_usart_rs485(void)
{

	struct usart_config config;
	usart_get_config_defaults(&config);
	
	config.baudrate = 115200;
	config.mux_setting = EXT1_RS485_SERCOM_MUX_SETTING;
	config.pinmux_pad0 = EXT1_RS485_SERCOM_PINMUX_PAD0;
	config.pinmux_pad1 = EXT1_RS485_SERCOM_PINMUX_PAD1;
	config.pinmux_pad2 = EXT1_RS485_SERCOM_PINMUX_PAD2;
	config.pinmux_pad3 = EXT1_RS485_SERCOM_PINMUX_PAD3;

	while (usart_init(&rs485_instance,
		EXT1_RS485_MODULE, &config) != STATUS_OK) {
	}
	usart_enable(&rs485_instance);

	usart_register_callback(&rs485_instance,
		rs485_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&rs485_instance, USART_CALLBACK_BUFFER_RECEIVED);
}
//! [rs485_setup]

//! [setup]

int main(void)
{
//! [setup_init]
	system_init();
	delay_init();
	configure_usart_cdc();
//! [setup_init]

//! [main_setup]

//! [configure_rs485]
	configure_usart_rs485();
//! [configure_rs485]

//! [rs485_msg]
	while(1) {
		usart_write_buffer_wait(&rs485_instance,tx_buffer,sizeof(tx_buffer));
		delay_s(RS485_SENDING_MSG_INTERVAL);
	}
//! [rs485_msg]

//! [main_setup]
}
