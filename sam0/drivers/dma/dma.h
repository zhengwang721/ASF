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

/** DMA invalid channel number */
#define DMA_INVALID_CHANNEL        0xff

/** DMA priority level */
enum dma_priority_level {
	/** Priority level 0 */
	DMA_PRIORITY_LEVEL_0 = 0,
	/** Priority level 1 */
	DMA_PRIORITY_LEVEL_1,
	/** Priority level 2 */
	DMA_PRIORITY_LEVEL_2,
	/** Priority level 3 */
	DMA_PRIORITY_LEVEL_3,
	/** Priority level 4 */
	DMA_PRIORITY_LEVEL_4,
	/** Priority level 5 */
	DMA_PRIORITY_LEVEL_5,
	/** Priority level 6 */
	DMA_PRIORITY_LEVEL_6,
	/** Priority level 7 */
	DMA_PRIORITY_LEVEL_7,
};

/** DMA input actions */
enum dma_event_input_action {
	/** No action */
	DMA_EVENT_INPUT_NOACT = 0,
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

/**
 * Callback types for DMA callback driver
 */
enum dma_callback_type {
	/** Callback for transfer complete */
	DMA_CALLBACK_TRANSFER_DONE,
	/** Callback for any of transfer error */
	DMA_CALLBACK_TRANSFER_ERROR,
	/** Callback for channel suspend */
	DMA_CALLBACK_CHANNEL_SUSPEND,
	/** Max callback num */
	DMA_CALLBACK_MAX,
};

/** Configurations for DMA events */
struct dma_events_config {
	/** Event input actions */
	enum dma_event_input_action input_action;
	/** Enable DMA event output */
	bool event_output_enable;
	/** Event output actions */
	enum dma_event_output_selection output_action;
};

/** DMA transfer descriptor */
struct dma_transfer_descriptor {
	/** Block transfer control */
	uint16_t block_transfer_control;
	/** Transfer transfer count. Count value is 
	decremented by one after each beat data transfer */
	uint16_t block_transfer_count;
	/** Transfer source address */
	uint32_t source_address;
	/** Transfer destination address */
	uint32_t destination_address;
	/** Next transfer descriptor address */
	uint32_t next_descriptor_address;
};

/** DMA configurations for transfer */
struct dma_transfer_config {
	/** DMA transfer priority */
	enum dma_priority_level priority;
	/** CRC enable flag */
	bool crc;
	/** DMA transfer trigger selection */
	enum dma_transfer_trigger transfer_trigger;
	/**DMA peripheral trigger index*/
	uint8_t peripheral_trigger;
	/** DMA events configurations */
	struct dma_events_config event_config;
};

/** Forward definition of the dma resource */
struct dma_resource;
/** Type of the callback function for DMA resource*/
typedef void (*dma_callback_t)(const struct dma_resource *const resource);

/** Structure for DMA transfer resource */
struct dma_resource {
	/** Allocated Channel ID*/
	uint8_t channel_id;
	/** Callback function for DMA transfer job */
	dma_callback_t callback[DMA_CALLBACK_MAX];
	/** Bit mask for enabled callbacks */
	uint8_t callback_enable;
	/** Status of the last job */
	volatile enum status_code job_status;
	/** Calculated CRC checksum value */
	uint32_t crc_checksum;
	/** Transfered data size */
	uint32_t transfered_size;
};

void dma_get_config_defaults(struct dma_transfer_config *config);
enum status_code dma_allocate(struct dma_resource *resource,
								struct dma_transfer_config *config);
enum status_code dma_transfer_job(struct dma_resource *resource, struct dma_transfer_descriptor *descriptor);
void dma_abort_job(struct dma_resource *resource);
void dma_suspend_job(struct dma_resource *resource);
void dma_resume_job(struct dma_resource *resource);
enum status_code dma_get_job_status(struct dma_resource *resource);
enum status_code dma_release(struct dma_resource *resource);
bool dma_is_busy(struct dma_resource *resource);
void dma_enable_callback(struct dma_resource *resource, enum dma_callback_type type);
void dma_disable_callback(struct dma_resource *resource, enum dma_callback_type type);
void dma_register_callback(struct dma_resource *resource, dma_callback_t callback, enum dma_callback_type type);
void dma_unregister_callback(struct dma_resource *resource, enum dma_callback_type type);

#endif /* DMA_H_INCLUDED */
