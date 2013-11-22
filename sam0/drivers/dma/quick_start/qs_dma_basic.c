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

//! [setup]
/** Destination file */

/** 4 linked transfer descriptor s*/
#define EXAMPLE_RESOURCE1_LIST_NUM 4

#define SOURCE_FILE_SIZE (1024)
#define DEST_FILE_SIZE (SOURCE_FILE_SIZE * 4)

const uint8_t wave_file[DEST_FILE_SIZE] = { 0 };

/** Source file */
const uint8_t wave_file1[SOURCE_FILE_SIZE] = {0x5a};
const uint8_t wave_file2[SOURCE_FILE_SIZE] = {0xa5};
const uint8_t wave_file3[SOURCE_FILE_SIZE] = {0xaa};
const uint8_t wave_file4[SOURCE_FILE_SIZE] = {0x55};


struct dma_transfer_descriptor transfer_desc[EXAMPLE_RESOURCE1_LIST_NUM];

// [transfer_done]
static volatile bool transfer_is_done = false;
// [transfer_done]

// [_transfer_done]
static void transfer_done( const struct dma_resource* const resource )
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
	dma_allocate(resource, &config);
//! [setup_3]
}

static void setup_transfer_descriptor( void )
{
	/* ! [setup_dma_descriptor] */
	transfer_desc[3].block_transfer_control = DMAC_BTCTRL_VALID |
			DMAC_BTCTRL_BEATSIZE_BYTE;
	transfer_desc[3].block_transfer_count = sizeof(wave_file4);
	transfer_desc[3].source_address = (uint32_t)wave_file4;
	transfer_desc[3].destination_address
		= (uint32_t)(&wave_file[SOURCE_FILE_SIZE * 3]);
	transfer_desc[3].next_descriptor_address = 0;

	transfer_desc[2].block_transfer_control = DMAC_BTCTRL_VALID |
			DMAC_BTCTRL_BEATSIZE_BYTE;
	transfer_desc[2].block_transfer_count = sizeof(wave_file3);
	transfer_desc[2].source_address = (uint32_t)wave_file3;
	transfer_desc[2].destination_address
		= (uint32_t)(&wave_file[SOURCE_FILE_SIZE * 2]);
	transfer_desc[2].next_descriptor_address = (uint32_t)&transfer_desc[3];

	transfer_desc[1].block_transfer_control = DMAC_BTCTRL_VALID |
			DMAC_BTCTRL_BEATSIZE_BYTE;
	transfer_desc[1].block_transfer_count = sizeof(wave_file2);
	transfer_desc[1].source_address = (uint32_t)wave_file2;
	transfer_desc[1].destination_address
		= (uint32_t)(&wave_file[SOURCE_FILE_SIZE]);
	transfer_desc[1].next_descriptor_address = (uint32_t)&transfer_desc[2];

	transfer_desc[0].block_transfer_control = DMAC_BTCTRL_VALID |
			DMAC_BTCTRL_BEATSIZE_BYTE;
	transfer_desc[0].block_transfer_count = sizeof(wave_file1);
	transfer_desc[0].source_address = (uint32_t)wave_file1;
	transfer_desc[0].destination_address = (uint32_t)wave_file;
	transfer_desc[0].next_descriptor_address = (uint32_t)&transfer_desc[1];
//! [setup_dma_descriptor]
}

//! [setup]

int main(void)
{
	struct dma_resource example_resource;

	system_init();

	//! [setup_init]
	configure_dma_resource(&example_resource);

	setup_transfer_descriptor();
	//! [setup_init]

	//! [main_callback_register]
	dma_register_callback(&example_resource, transfer_done,
			DMA_CALLBACK_TRANSFER_DONE);
	dma_enable_callback(&example_resource, DMA_CALLBACK_TRANSFER_DONE);
	//! [main_callback_register]

	//! [main]
	dma_transfer_job(&example_resource, transfer_desc);
	
	//! [main_1]
	while (!transfer_is_done) {
		/* Wait for transfer done */
	}
	/* ! [main_1] */

	while (true) {
		/* Nothing to do */
	}

	//! [main]
}
