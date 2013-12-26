/**
 * \file
 *
 * \brief SAM D2x Sercom Usart driver with DMA quick start
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

void configure_usart(void);
void transfer_done( const struct dma_resource* const resource );
void configure_dma_resource(struct dma_resource *resource);
void setup_transfer_descriptor(DmacDescriptor *descriptor);

//! [module_inst]
struct usart_module usart_instance;
//! [module_inst]

//! [dma_resource]
struct dma_resource example_resource;
//! [dma_resource]

// [transfer_done_flag]
static volatile bool transfer_is_done = false;
// [transfer_done_flag]

// [usart_string]
static uint16_t string[] = {'h','e','l','l','o'};
// [usart_string]

// [transfer_descriptor]
COMPILER_ALIGNED(16)
DmacDescriptor example_descriptor;
// [transfer_descriptor]

//! [setup]
// [_transfer_done]
void transfer_done( const struct dma_resource* const resource )
{
	transfer_is_done = true;
}
// [_transfer_done]

// [config_dma_resource]
void configure_dma_resource(struct dma_resource *resource)
{
//! [setup_1]
	struct dma_resource_config config;
//! [setup_1]

//! [setup_2]
	dma_get_config_defaults(&config);
//! [setup_2]

//! [setup_3]
	config.transfer_trigger = DMA_TRIGGER_PERIPHERAL;
	config.peripheral_trigger = SERCOM3_DMAC_ID_TX;
	config.trigger_action = DMA_TRIGGER_ACTON_BEAT;
//! [setup_3]

//! [setup_4]
	dma_allocate(resource, &config);
//! [setup_4]
}
// [config_dma_resource]

// [setup_dma_transfer_descriptor]
void setup_transfer_descriptor(DmacDescriptor *descriptor)
{
//! [setup_5]
	struct dma_descriptor_config descriptor_config;
//! [setup_5]

//! [setup_6]
	dma_descriptor_get_config_defaults(&descriptor_config);
//! [setup_6]

//! [setup_7]
	descriptor_config.beat_size = DMA_BEAT_SIZE_HWORD;
	descriptor_config.dst_increment_enable = false;
	descriptor_config.block_transfer_count = sizeof(string)/sizeof(uint16_t);
	descriptor_config.source_address = (uint32_t)string + sizeof(string);
	descriptor_config.destination_address = (uint32_t)(&usart_instance.hw->USART.DATA.reg);
//! [setup_7]

//! [setup_8]
	dma_descriptor_create(descriptor, &descriptor_config);
//! [setup_8]
}
// [setup_dma_transfer_descriptor]

void configure_usart(void)
{
//! [setup_config]
	struct usart_config config_usart;
//! [setup_config]
//! [setup_config_defaults]
	usart_get_config_defaults(&config_usart);
//! [setup_config_defaults]

//! [setup_change_config]
	config_usart.baudrate    = 9600;
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
//! [setup_change_config]

//! [setup_set_config]
	while (usart_init(&usart_instance,
			EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	}
//! [setup_set_config]

//! [setup_enable]
	usart_enable(&usart_instance);
//! [setup_enable]
}
//! [setup]

int main(void)
{
	system_init();

//! [setup_init]
//! [setup_usart]
	configure_usart();
//! [setup_usart]

//! [setup_dma_resource]
	configure_dma_resource(&example_resource);
//! [setup_dma_resource]

//! [setup_transfer_descriptor]
	setup_transfer_descriptor(&example_descriptor);
//! [setup_transfer_descriptor]

//! [Add descriptor to DMA resource]
	dma_add_descriptor(&example_resource, &example_descriptor);
//! [Add descriptor to DMA resource]

//! [setup_callback_register]
	dma_register_callback(&example_resource, transfer_done,
			DMA_CALLBACK_TRANSFER_DONE);
//! [setup_callback_register]

//! [setup_enable_callback]
	dma_enable_callback(&example_resource, DMA_CALLBACK_TRANSFER_DONE);
//! [setup_enable_callback]
//! [setup_init]

//! [main]
//! [main_1]
	dma_start_transfer_job(&example_resource);
//! [main_1]

//! [main_2]
	while (!transfer_is_done) {
		/* Wait for transfer done */
	}
//! [main_2]

//! [endless_loop]
	while (true) {
//! [endless_loop]
	}
//! [main_2]
//! [main]
}
