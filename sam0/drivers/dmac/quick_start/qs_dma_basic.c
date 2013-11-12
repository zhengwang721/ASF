/**
 * \file
 *
 * \brief SAM D2x DMA System Driver Quick Start
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

//Aim: Transfer a wave file from memory to I2S

//! [setup]
//TODO: Sample wave file
const uint8_t wave_file[4096] = { 0 };

#define EXAMPLE_TRANSFER_SOURCE (uint32_t)wave_file
#define EXAMPLE_TRANSFER_DEST (uint32_t)(&I2S->DATA[0])
#define EXAMPLE_LAST_LINK_ADDRESS 0x0  //0x0 stands for no further link

// [transfer_done]
static volatile bool transfer_is_done = false;
// [transfer_done]

// [_transfer_done]
static void _transfer_done( void )
{
	transfer_is_done = true;
}
// [_transfer_done]

static void configure_dma_resource(struct dma_resource *resource)
{
//! [setup_1]
	struct dma_transfer_config config;
//! [setup_1]

//! [setup_2]
	dma_get_config_defaults(&config);
//! [setup_2]

//! [setup_3]
	config.transfer_descriptor.block_count = sizeof(wave_file);
	config.transfer_descriptor.source_address = EXAMPLE_TRANSFER_SOURCE;
	config.transfer_descriptor.destination_address = EXAMPLE_TRANSFER_DEST;
	config.transfer_descriptor.next_descriptor_address = EXAMPLE_LAST_LINK_ADDRESS;
//! [setup_3]

//! [setup_4]
	events_allocate(desc, &config);
//! [setup_4]
}

//! [setup]

int main(void)
{
	struct dma_resource example_resource;

	system_init();

	//TODO: Initial I2S
	// I2S_init();
	
	//! [setup_init]
	configure_dma_channel(&example_resource);
	//! [setup_init]

	//! [main_callback_register]
	dma_register_callback(&example_resource, _transfer_done);
	dma_enable_callback(&example_resource);
	//! [main_callback_register]

	//! [main]
	dma_transfer_job(&example_resource);
	
	//! [main_1]
	while (!transfer_is_done) {
		/* Wait for transfer done */
	};
	//! [main_1]

	while (true) {
		/* Nothing to do */
	}

	//! [main]
}
