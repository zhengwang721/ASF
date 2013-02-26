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

int main(void)
{
//! [main]
//! [module_inst]
	struct usart_module usart_edbg;
//! [module_inst]
//! [config]
	struct usart_config config_struct;
//! [config]
//! [temp_variable]
	uint16_t temp;
//! [temp_variable]

//! [system_init]
	/* Initialize system clocks */
	system_init();
//! [system_init]

//! [conf_defaults]
	/* Get configuration defaults for the USART
	 * 9600 8N1
	 */
	usart_get_config_defaults(&config_struct);
//! [conf_defaults]
//! [conf_modify]
	config_struct.mux_settings = USART_RX_3_TX_2_XCK_3;
	config_struct.pinout_pad3 = EDBG_CDC_RX_PINMUX;
	config_struct.pinout_pad2 = EDBG_CDC_TX_PINMUX;
//! [conf_modify]

//! [init]
	/* Apply configuration */
	while (usart_init(&usart_edbg, EDBG_CDC_MODULE,
			&config_struct) != STATUS_OK) {
	}
//! [init]

//! [enable]
	/* Enable USARTs */
	usart_enable(&usart_edbg);
//! [enable]

//! [enable_transceivers]
	/* Enable transmitter */
	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_RX);
//! [enable_transceivers]

//! [echo_characters]
	/* Echo back characters received */
	while (1) {
		if (usart_read_wait(&usart_edbg, &temp) == STATUS_OK) {
			while (usart_write_wait(&usart_edbg, temp) != STATUS_OK) {
			}
		}
	}
//! [echo_characters]
//! [main]
}
