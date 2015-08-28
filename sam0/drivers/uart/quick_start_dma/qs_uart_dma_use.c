/**
 * \file
 *
 * \brief SAM Uart driver with DMA quick start
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
#include <string.h>
//! [module_inst]
struct uart_module uart_instance;
//! [module_inst]


//! [dma_resource]
struct dma_resource uart_dma_resource_tx;
//! [dma_resource]

//! [usart_buffer]
#define BUFFER_LEN    15
static uint8_t string[BUFFER_LEN] = "Hello World!\r\n";
//! [usart_buffer]

//! [transf_flag]
volatile bool transfer_done_flag = false;
//! [transf_flag]

//! [transfer_descriptor]
struct dma_descriptor example_descriptor_tx;
//! [transfer_descriptor]

//! [setup]
//! [transfer_done_tx]
static void transfer_done_tx(struct dma_resource* const resource )
{
	transfer_done_flag = true;
}
//! [transfer_done_tx]

static void delay(void)
{
	for (int i = 0; i < 3000000; i++)
		asm("nop");
}

//! [config_dma_resource_tx]
static void configure_dma_resource_tx(struct dma_resource *resource)
{
//! [setup_tx_1]
	struct dma_resource_config config;
//! [setup_tx_1]

//! [setup_tx_2]
	dma_get_config_defaults(&config);
//! [setup_tx_2]

//! [setup_tx_3]
	config.des.periph = UART0TX_DMA_PERIPHERAL;
	config.des.enable_inc_addr = false;
	//config.src.periph = UART0TX_DMA_PERIPHERAL;
//! [setup_tx_3]

//! [setup_tx_4]
	dma_allocate(resource, &config);
//! [setup_tx_4]
}
//! [config_dma_resource_tx]

//! [setup_dma_transfer_tx_descriptor]
static void setup_transfer_descriptor_tx(struct dma_descriptor *descriptor)
{

//! [setup_tx_5]
	dma_descriptor_get_config_defaults(descriptor);
//! [setup_tx_5]

//! [setup_tx_6]
	descriptor->buffer_size = BUFFER_LEN;
	descriptor->read_start_addr = (uint32_t)string;
	descriptor->write_start_addr = 
			(uint32_t)(&uart_instance.hw->TRANSMIT_DATA.reg);
//! [setup_tx_6]
}
//! [setup_dma_transfer_tx_descriptor]

//! [setup_usart]
static void configure_usart(void)
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

//! [enable_interrupt]
	uart_enable_transmit_dma(&uart_instance);
//! [enable_interrupt]
}
//! [setup_usart]

//! [setup_callback]
static void configure_dma_callback(void)
{
//! [setup_callback_register]
	dma_register_callback(&uart_dma_resource_tx, transfer_done_tx, DMA_CALLBACK_TRANSFER_DONE);
//! [setup_callback_register]

//! [setup_enable_callback]
	dma_enable_callback(&uart_dma_resource_tx, DMA_CALLBACK_TRANSFER_DONE);
//! [setup_enable_callback]

//! [enable_inic]
	NVIC_EnableIRQ(6);
//! [enable_inic]
}
//! [setup_callback]

//! [setup]

int main(void)
{
//! [setup_init]
//! [init_system]
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
//! [init_system]

//! [setup_usart]
	configure_usart();
//! [setup_usart]

//! [dma_global_init]
	dma_global_init();
//! [dma_global_init]

//! [setup_dma_resource]
	configure_dma_resource_tx(&uart_dma_resource_tx);
//! [setup_dma_resource]

//! [setup_transfer_descriptor]
	setup_transfer_descriptor_tx(&example_descriptor_tx);
//! [setup_transfer_descriptor]

//! [add_descriptor_to_resource]
	dma_add_descriptor(&uart_dma_resource_tx, &example_descriptor_tx);
//! [add_descriptor_to_resource]

//! [configure_callback]
	configure_dma_callback();
//! [configure_callback]
	
	transfer_done_flag = false;

//! [setup_init]

//! [main]
//! [main_1]
	dma_start_transfer_job(&uart_dma_resource_tx);
//! [main_1]

//! [endless_loop]
	while (true) {
		if (transfer_done_flag) {
			transfer_done_flag = false;
			delay();
			dma_start_transfer_job(&uart_dma_resource_tx);
		}
	}
//! [endless_loop]
//! [main]
}
