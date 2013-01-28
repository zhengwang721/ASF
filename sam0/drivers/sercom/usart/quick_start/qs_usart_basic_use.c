/**
 * \file
 *
 * \brief SAMD20 System related functionality
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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

/* SERCOM 4 is the Embedded debugger */
#define QUICKSTART_USART SERCOM4

void write_string(struct usart_dev_inst *const dev, uint8_t *string);

void write_string(struct usart_dev_inst *const dev, uint8_t *string)
{
	do {
		while (usart_write(dev, *string) != STATUS_OK) {
		}
	} while (*(++string) != 0);
}

int main(void)
{
	struct usart_dev_inst usart_edbg;
	struct usart_conf config_struct;

	uint8_t string[] = "Hello world!\n\r\0";

	/* Initialize system clocks */
	system_init();

	/* Get configuration defaults for the USART
	 * 9600 8N1
	 */
	usart_get_config_defaults(&config_struct);

	/* Apply configuration */
	while (usart_init(&usart_edbg, QUICKSTART_USART,
			&config_struct) != STATUS_OK) {
	}

	/* Enable USARTs */
	usart_enable(&usart_edbg);

	/* Enable transmitter */
	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_TX);

	while (1) {
		write_string(&usart_edbg, string);
	}
}
