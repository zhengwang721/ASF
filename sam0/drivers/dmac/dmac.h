/**
 * \file
 *
 * \brief SAM D2x DMA Driver
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
#ifndef DMA_H_INCLUDED
#define DMA_H_INCLUDED

#include <compiler.h>

/** DMA input actions */
enum dma_event_input_action {
	/** No action */
	DMA_EVENT_INPUT_NOACT,
	/** Transfer and periodic transfer trigger */
	DMA_EVENT_INPUT_TRIG,
	/** Conditional transfer trigger*/
	DMA_EVENT_INPUT_CTRIG,
	/** Conditional block transfer */
	DMA_EVENT_INPUT_CBLOCK,
	/** Channel suspend operation */
	DMA_EVENT_INPUT_SUSPEND,
	/** Channel resume operation */
	DMA_EVENT_INPUT_RESUME,
	/** Skip next block suspend action */
	DMA_EVENT_INPUT_SSKIP,
};

/** DMA AHB access size, it apply to both read and write */
enum dma_beat_size {
	/** 8-bit access */
	DMA_BEAT_SIZE_BYTE = 0,
	/** 16-bit access */
	DMA_BEAT_SIZE_HWORD,
	/** 32-bit access */
	DMA_BEAT_SIZE_WORD,
};

/** Event output selection */
enum dma_event_output_selection {
	/** Event generation disable */
	DMA_EVENT_OUTPUT_DISABLE,
	/** Event strobe when block transfer complete */
	DMA_EVENT_OUTPUT_BLOCK,
	/** Event strobe when beat transfer complete */
	DMA_EVENT_OUTPUT_BEAT,
};

/** DMA transfer trigger type */
enum dma_transfer_trigger {
	/** Use software as the DMA trigger */
	DMA_TRIGGER_SOFTWARE,
	/** Use peripheral as the DMA trigger */
	DMA_TRIGGER_PERIPHERAL,
	/** Use event as the DMA trigger */
	DMA_TRIGGER_EVENT,
};

/** Configurations for DMA events */
struct dma_events_config {
	/** Event input actions */
	enum dma_event_input_action input_action;
	/** Enable DMA event output */
	bool dma_event_output_enable;
	/** Event output actions */
	enum dma_event_output_selection output_action;
};

/** DMA generic configurations for all channels */
struct dma_control_config {
	/** Descriptor base address */
	uint32_t descriptor_base_addr;
	/** Descriptor total size */
	uint32_t descriptor_total_size;
	/** Write back memory base address */
	uint32_t write_back_memory_base_addr;
	/** Write back memory total size */
	uint32_t write_back_memory_total_size;	
};

/** DMA transfer descriptor */
struct dma_transfer_descriptor {
	/** Block transfer control */
	uint16_t block_transfer_control;
	/** Transfer transfer count. Count value is 
	decremented by one after each beat data transfer */
	uint16_t block_count;
	/** Transfer source address */
	uint32_t source_address;
	/** Transfer destination address */
	uint32_t destination_address;
	/** Next transfer descriptor address */
	uint32_t next_descriptor_address;
};

/** DMA configurations for transfer */
struct dma_transfer_config {
	/** DMA transfer trigger selection */
	enum dma_transfer_trigger transfer_trigger;
	/** DMA transfer priority */
	uint8_t priority;
	/** Is CRC enabled for the transfer */
	bool crc;
	/** Transfer beat size */
	enum dma_beat_size beat_size;
	/** DMA events configurations */
	struct dma_events_config event_config;
	/** DMA transfer descriptor */
	struct dma_transfer_descriptor transfer_descriptor;
};

/** Structure for DMA transfer resource */
struct dma_resource {
	/** Allocated Channel ID*/
	uint8_t channel_id;
	/** Callback function for DMA transfer job */
	dma_callback_t callback;
	/** Bit mask for enabled callbacks */
	uint8_t callback_enable;
	/** Status of the last job */
	volatile enum status_code job_status;
};

typedef void (*dma_callback_t)(void);

void dma_get_config_defaults(struct dma_transfer_config *transfer_config);
enum status_code dma_allocate(struct dma_resource *dma_resource,
								struct dma_transfer_config *transfer_config);
enum status_code dma_update_resource(struct dma_resource *dma_resource,
								struct dma_transfer_config *transfer_config)
enum status_code dma_transfer_job(struct dma_resource *dma_resource);
enum status_code dma_abort_job(struct dma_resource *dma_resource);
enum status_code dma_get_status(struct dma_resource *dma_resource);
enum status_code dma_deallocate(struct dma_resource *dma_resource);
void dma_enable_callback(struct dma_resource *dma_resource);
void dma_disable_callback(struct dma_resource *dma_resource);
void dma_register_callback(struct dma_resource *dma_resource, dma_callback_t callback);
void dma_unregister_callback(struct dma_resource *dma_resource, dma_callback_t callback);

#endif /* DMA_H_INCLUDED */
