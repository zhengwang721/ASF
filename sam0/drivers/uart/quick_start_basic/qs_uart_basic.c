/**
 * \file
 *
 * \brief SAM UART Quick Start
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

void configure_uart(void);

//! [module_inst]
struct uart_module uart_instance;
//! [module_inst]

//! [setup]
void configure_uart(void)
{
//! [setup_config]
	struct uart_config config_uart;
//! [setup_config]
//! [setup_config_defaults]
	uart_get_config_defaults(&config_uart);
//! [setup_config_defaults]

//! [setup_change_config]
	config_uart.baud_rate = 38400;
	config_uart.pinmux_pad[0] = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_uart.pinmux_pad[1] = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_uart.pinmux_pad[2] = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_uart.pinmux_pad[3] = EDBG_CDC_SERCOM_PINMUX_PAD3;
//! [setup_change_config]

//! [setup_set_config]
	while (uart_init(&uart_instance,
			EDBG_CDC_MODULE, &config_uart) != STATUS_OK) {
	}
//! [setup_set_config]
}
//! [setup]

int main(void)
{
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);

//! [setup_init]
	configure_uart();
//! [setup_init]

//! [main]
//! [main_send_string]
	uint8_t string[] = "Hello World!\r\n";
	uart_write_buffer_wait(&uart_instance, string, sizeof(string));
//! [main_send_string]

//! [main_rec_var]
	uint8_t temp;
//! [main_rec_var]

//! [main_loop]
	while (true) {
//! [main_read]
		if (uart_read_wait(&uart_instance, &temp) == STATUS_OK) {
//! [main_read]
//! [main_write]
			while (uart_write_wait(&uart_instance, temp) != STATUS_OK) {
			}
//! [main_write]
		}
	}
//! [main_loop]
//! [main]
}
