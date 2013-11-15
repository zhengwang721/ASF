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
//TODO: Select several DMA not conflict when usage to behaviour the concurrency 

//Aim: Transfer a wave file from memory to I2S, discard the conflict of I2C transfer.

//! [setup]
//TODO: Sample wave file
const uint8_t wave_file[4096] = { 0 };

#define EXAMPLE_RESOURCE1_LIST_NUM 4

const uint8_t wave_file1[256] = {0x5a};
const uint8_t wave_file2[128] = {0xa5};
const uint8_t wave_file3[384] = {0xaa};
const uint8_t wave_file4{512} = {0x55};

#define EXAMPLE_TRANSFER_SOURCE (uint32_t)wave_file
#define EXAMPLE_TRANSFER_DEST (uint32_t)(&I2S->DATA[0])
#define EXAMPLE_LAST_LINK_ADDRESS 0x0  //0x0 stands for no further link

// [transfer_done]
static volatile bool transfer_is_done = false;
static volatile bool transfer_is_done1 = false;
// [transfer_done]

// [_transfer_done]
static void _transfer_done( void )
{
	transfer_is_done = true;
}
// [_transfer_done]

// [_transfer_done1]
static void _transfer_done1( void )
{
	transfer_is_done1 = true;
}
// [_transfer_done1]

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
	dma_allocate(resource, &config);
//! [setup_4]
}

static void configure_dma_resource1(struct dma_resource *resource)
{
	struct dma_transfer_descriptor[EXAMPLE_RESOURCE1_LIST_NUM];

//! [setup_dma_descriptor]
	dma_transfer_descriptor[3].block_count = sizeof(wave_file4);
	dma_transfer_descriptor[3].source_address = (uint32_t)wave_file4;
	dma_transfer_descriptor[3].destination_address = (uint32_t)(&I2S->DATA[0]);
	dma_transfer_descriptor[3].next_descriptor_address = 0;
	
	dma_transfer_descriptor[2].block_count = sizeof(wave_file3);
	dma_transfer_descriptor[2].source_address = (uint32_t)wave_file3;
	dma_transfer_descriptor[2].destination_address = (uint32_t)(&I2S->DATA[0]);
	dma_transfer_descriptor[2].next_descriptor_address = &dma_transfer_descriptor[3];
	
	dma_transfer_descriptor[1].block_count = sizeof(wave_file2);
	dma_transfer_descriptor[1].source_address = (uint32_t)wave_file2;
	dma_transfer_descriptor[1].destination_address = (uint32_t)(&I2S->DATA[0]);
	dma_transfer_descriptor[1].next_descriptor_address = &dma_transfer_descriptor[2];
	
	dma_transfer_descriptor[0].block_count = sizeof(wave_file1);
	dma_transfer_descriptor[0].source_address = (uint32_t)wave_file1;
	dma_transfer_descriptor[0].destination_address = (uint32_t)(&I2S->DATA[0]);
	dma_transfer_descriptor[0].next_descriptor_address = &dma_transfer_descriptor[1];
//! [setup_dma_descriptor]

//! [setup_1]
	struct dma_transfer_config config;
//! [setup_1]

//! [setup_2]
	dma_get_config_defaults(&config);
//! [setup_2]

//! [setup_3]
	config.transfer_descriptor = dma_transfer_descriptor[0];
//! [setup_3]

//! [setup_4]
	events_allocate(desc, &config);
//! [setup_4]
}

//! [setup]

int main(void)
{
	struct dma_resource example_resource,example_resource1;

	system_init();

	//TODO: Initial I2S
	// I2S_init();
	
	//! [setup_init]
	configure_dma_resource(&example_resource);
	configure_dma_resource1(&example_resource1);
	//! [setup_init]

	//! [main_callback_register]
	dma_register_callback(&example_resource, _transfer_done);
	dma_enable_callback(&example_resource);
	
	dma_register_callback(&example_resource1, _transfer_done1);
	dma_enable_callback(&example_resource1);
	//! [main_callback_register]

	//! [main]
	dma_transfer_job(&example_resource);
	dma_transfer_job(&example_resource1);
	
	//! [main_1]
	while ((!transfer_is_done) || (!transfer_is_done)) {
		/* Wait for transfer done */
	};
	//! [main_1]

	while (true) {
		/* Nothing to do */
	}

	//! [main]
}
