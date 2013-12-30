/**
 * \file
 *
 * \brief SAM D2x I2C Master Quick Start Guide
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

//! [packet_data]
#define DATA_LENGTH 10
static uint8_t buffer[DATA_LENGTH] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
};

#define SLAVE_ADDRESS 0x12
//! [packet_data]

/* Number of times to try to send packet if failed. */
//! [timeout]
#define TIMEOUT 1000
//! [timeout]

/* Init software module. */
//! [dev_inst]
struct i2c_master_module i2c_master_instance;
//! [dev_inst]

void configure_i2c_master(void);
void setup_dma_descriptor(DmacDescriptor *descriptor);
//! [initialize_i2c]
void configure_i2c_master(void)
{
	/* Initialize config structure and software module. */
	//! [init_conf]
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	//! [init_conf]

	/* Change buffer timeout to something longer. */
	//! [conf_change]
	config_i2c_master.buffer_timeout = 10000;
	//! [conf_change]

	/* Initialize and enable device with config. */
	//! [init_module]
	i2c_master_init(&i2c_master_instance, SERCOM2, &config_i2c_master);
	//! [init_module]

	//! [enable_module]
	i2c_master_enable(&i2c_master_instance);
	//! [enable_module]
}
//! [initialize_i2c]

//! [dma_resource]
struct dma_resource example_resource;
//! [dma_resource]

// [transfer_done_flag]
static volatile bool transfer_is_done = false;
// [transfer_done_flag]

// [transfer_descriptor]
COMPILER_ALIGNED(16)
DmacDescriptor example_descriptor;
// [transfer_descriptor]

// [_transfer_done]
void transfer_done( const struct dma_resource* const resource )
{
	UNUSED(resource);

	transfer_is_done = true;
}
// [_transfer_done]

// [config_dma_resource]
void configure_dma_resource(struct dma_resource *resource)
{
//! [dma_setup_1]
	struct dma_resource_config config;
//! [dma_setup_1]

//! [dma_setup_2]
	dma_get_config_defaults(&config);
	config.transfer_trigger = DMA_TRIGGER_PERIPHERAL;
	config.peripheral_trigger = SERCOM2_DMAC_ID_TX;
//! [dma_setup_2]

//! [dma_setup_3]
	dma_allocate(resource, &config);
//! [dma_setup_3]
}
// [config_dma_resource]

// [setup_dma_transfer_descriptor]
void setup_dma_descriptor(DmacDescriptor *descriptor)
{
	//! [dma_setup_4]
	struct dma_descriptor_config descriptor_config;
	//! [dma_setup_4]

	//! [dma_setup_5]
	dma_descriptor_get_config_defaults(&descriptor_config);
	//! [dma_setup_5]

	//! [dma_setup_6]
	descriptor_config.beat_size = DMA_BEAT_SIZE_BYTE;
	descriptor_config.dst_increment_enable = false;
	descriptor_config.block_transfer_count = DATA_LENGTH;
        descriptor_config.source_address = (uint32_t)buffer + sizeof(buffer);
	descriptor_config.destination_address = (uint32_t)(&i2c_master_instance.hw->I2CM.DATA.reg);
	//! [dma_setup_6]

	//! [dma_setup_7]
	dma_descriptor_create(descriptor, &descriptor_config);
	//! [dma_setup_7]
}
// [setup_dma_transfer_descriptor]

int main(void)
{
	system_init();

	//! [init]
	/* Configure device and enable. */
	//! [config]
	configure_i2c_master();
	//! [config]

	configure_dma_resource(&example_resource);
	setup_dma_descriptor(&example_descriptor);
	dma_add_descriptor(&example_resource, &example_descriptor);
	dma_register_callback(&example_resource, transfer_done,
			DMA_CALLBACK_TRANSFER_DONE);
	dma_enable_callback(&example_resource, DMA_CALLBACK_TRANSFER_DONE);


	i2c_master_instance.hw->I2CM.ADDR.reg = 
          SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDRESS) | SERCOM_I2CM_ADDR_LENEN |
           SERCOM_I2CM_ADDR_LEN(DATA_LENGTH);
	
	dma_start_transfer_job(&example_resource);

	while (!transfer_is_done) {
		/* Wait for transfer done */
	}

	while(1);
	/* Timeout counter. */
	//! [timeout_counter]
	uint16_t timeout = 0;
	//! [timeout_counter]

	/* Init i2c packet. */
	//! [packet]
	struct i2c_packet packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = DATA_LENGTH,
		.data        = buffer,
	};
	//! [packet]
	//! [init]

	//! [main]
	/* Write buffer to slave until success. */
	//! [write_packet]
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
			STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
	//! [write_packet]
	//! [main]

	while (true) {
		/* Infinite loop */
	}

}
