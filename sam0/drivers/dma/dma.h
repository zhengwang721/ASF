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
	DMA_PRIORITY_LEVEL_0,
	/** Priority level 1 */
	DMA_PRIORITY_LEVEL_1,
	/** Priority level 2 */
	DMA_PRIORITY_LEVEL_2,
	/** Priority level 3 */
	DMA_PRIORITY_LEVEL_3,
};

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

/**
 * Address increment step size. These bits select the address increment step size.
 * The setting apply to source or destination address, depending on STEPSEL setting.
 */
enum dma_address_increment_stepsize {
	/** Next ADDR <- ADDR + BEATSIZE * 1 */
	DMA_ADDRESS_INCREMENT_STEP_SIZE_1 = 0,
	/** Next ADDR <- ADDR + BEATSIZE * 2 */
	DMA_ADDRESS_INCREMENT_STEP_SIZE_2,
	/** Next ADDR <- ADDR + BEATSIZE * 4 */
	DMA_ADDRESS_INCREMENT_STEP_SIZE_4,
	/** Next ADDR <- ADDR + BEATSIZE * 8 */
	DMA_ADDRESS_INCREMENT_STEP_SIZE_8,
	/** Next ADDR <- ADDR + BEATSIZE * 16 */
	DMA_ADDRESS_INCREMENT_STEP_SIZE_16,
	/** Next ADDR <- ADDR + BEATSIZE * 32 */
	DMA_ADDRESS_INCREMENT_STEP_SIZE_32,
	/** Next ADDR <- ADDR + BEATSIZE * 64 */
	DMA_ADDRESS_INCREMENT_STEP_SIZE_64,
	/** Next ADDR <- ADDR + BEATSIZE * 128 */
	DMA_ADDRESS_INCREMENT_STEP_SIZE_128,
};

/**
 * DMA step selection. This bit selects if source or destination addresses are
 * using the step size settings.
 */
enum dma_step_selection {
	/** Step size settings apply to the destination address */
	DMA_STEPSEL_DST = 0,
	/** Step size settings apply to the source address */
	DMA_STEPSEL_SRC,
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

/**
 * Block action definitions.
 */
enum dma_block_action {
	/** No action */
	DMA_BLOCK_ACTION_NOACT = 0,
	/** Channel in normal operation and block interrupt */
	DMA_BLOCK_ACTION_INT,
	/** Channel suspend operation is completed */
	DMA_BLOCK_ACTION_SUSPEND,
	/** Both channel suspend operation and block interrupt */
	DMA_BLOCK_ACTION_BOTH,
};

/** Event output selection */
enum dma_event_output_selection {
	/** Event generation disable */
	DMA_EVENT_OUTPUT_DISABLE = 0,
	/** Event strobe when block transfer complete */
	DMA_EVENT_OUTPUT_BLOCK,
	/** Event output reserved */
	DMA_EVENT_OUTPUT_RESERVED,
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

/** DMA trigger action type */
enum dma_transfer_trigger_action{
	DMA_TRIGGER_ACTON_BLOCK = DMAC_CHCTRLB_TRIGACT_BLOCK_Val,
	DMA_TRIGGER_ACTON_BEAT = DMAC_CHCTRLB_TRIGACT_BEAT_Val,
	DMA_TRIGGER_ACTON_TRANSACTION = DMAC_CHCTRLB_TRIGACT_TRANSACTION_Val,
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
	/** Max callback number */
	DMA_CALLBACK_N,
};

/**
 * DMA tansfer descriptor configuration
 */
struct dma_descriptor_config {
	/** The descriptor valid */
	bool descriptor_valid;
	/** Event output selection */
	enum dma_event_output_selection event_output_selection;
	/** Channel block actions */
	enum dma_block_action block_action;
	/** Transfer beat size */
	enum dma_beat_size beat_size;
	/** Source Address Increment Enable */
	bool src_increment_enable;
	/** Destination Address Increment Enable */
	bool dst_increment_enable;
	/** Step selection */
	enum dma_step_selection step_selection;
	/** Address Increment Step Size */
	enum dma_address_increment_stepsize step_size;
	/** Transfer transfer count. Count value is
	 * decremented by one after each beat data transfer */
	uint16_t block_transfer_count;
	/** Transfer source address */
	uint32_t source_address;
	/** Transfer destination address */
	uint32_t destination_address;
};

/** Configurations for DMA events */
struct dma_events_config {
	/** Event input actions */
	enum dma_event_input_action input_action;
	/** Enable DMA event output */
	bool event_output_enable;
};

/** DMA configurations for transfer */
struct dma_resource_config {
	/** DMA transfer priority */
	enum dma_priority_level priority;
	/** DMA transfer trigger selection */
	enum dma_transfer_trigger transfer_trigger;
	/**DMA peripheral trigger index*/
	uint8_t peripheral_trigger;
	/** DMA trigger action */
	enum dma_transfer_trigger_action trigger_action;
	/** DMA events configurations */
	struct dma_events_config event_config;
};

/** Forward definition of the DMA resource */
struct dma_resource;
/** Type of the callback function for DMA resource*/
typedef void (*dma_callback_t)(const struct dma_resource *const resource);

/** Structure for DMA transfer resource */
struct dma_resource {
	/** Allocated Channel ID*/
	uint8_t channel_id;
	/** Callback function for DMA transfer job */
	dma_callback_t callback[DMA_CALLBACK_N];
	/** Bit mask for enabled callbacks */
	uint8_t callback_enable;
	/** Status of the last job */
	volatile enum status_code job_status;
	/** Transferred data size */
	uint32_t transfered_size;
	/** DMA transfer descriptor */
	DmacDescriptor* descriptor;
};

/**
 * \brief Get DMA resource status.
 *
 * \param[in] resource Pointer to the DMA resource
 *
 * \return Status of the DMA resource.
 */
static inline enum status_code dma_get_job_status(struct dma_resource *resource)
{
	Assert(resource);

	return resource->job_status;
}

/**
 * \brief Check if the DMA was busy of transfer.
 *
 * \param[in] resource Pointer to the DMA resource
 *
 * \return Busy status of the DMA resource.
 *
 * \retval true The DMA resource has an on-going transfer
 * \retval false The DMA resource is not busy
 */
static inline bool dma_is_busy(struct dma_resource *resource)
{
	Assert(resource);

	return (resource->job_status == STATUS_BUSY);
}

/**
 * \brief Enable a callback function for a dedicated DMA resource
 *
 * \param[in] resource Pointer to the DMA resource
 * \param[in] type Callback function type
 *
 */
static inline void dma_enable_callback(struct dma_resource *resource,
		enum dma_callback_type type)
{
	Assert(resource);

	resource->callback_enable |= 1 << type;
}

/**
 * \brief Disable a callback function for a dedicated DMA resource
 *
 * \param[in] resource Pointer to the DMA resource
 * \param[in] type Callback function type
 *
 */
static inline void dma_disable_callback(struct dma_resource *resource,
		enum dma_callback_type type)
{
	Assert(resource);

	resource->callback_enable &= ~(1 << type);
}

/**
 * \brief Register a callback function for a dedicated DMA resource
 *
 * \param[in] resource Pointer to the DMA resource
 * \param[in] callback Pointer to the callback function
 * \param[in] type Callback function type
 *
 */
static inline void dma_register_callback(struct dma_resource *resource,
		dma_callback_t callback, enum dma_callback_type type)
{
	Assert(resource);

	resource->callback[type] = callback;
}

/**
 * \brief Unregister a callback function for a dedicated DMA resource
 *
 * \param[in] resource Pointer to the DMA resource
 * \param[in] type Callback function type
 *
 */
static inline void dma_unregister_callback(struct dma_resource *resource,
		enum dma_callback_type type)
{
	Assert(resource);

	resource->callback[type] = NULL;
}

/**
 * \brief Initializes DMA transfer configuration with predefined default values.
 *
 * This function will initialize a given DMA descriptor configuration structure to
 * a set of known default values. This function should be called on
 * any new instance of the configuration structures before being
 * modified by the user application.
 *
 * The default configuration is as follows:
 *  \li Set the descriptor as valid
 *  \li Disable event output
 *  \li No block action
 *  \li Set beat size as byte
 *  \li Enable source increment
 *  \li Enable destination increment
 *  \li Step size is applied to the destination address
 *  \li Address increment is beat size mutipled by 1
 *  \li Default transfer size is set to 0
 *  \li Default source address is set to NULL
 *  \li Default destination address is set to NULL
 * \param[out] config Pointer to the configuration
 *
 */
static inline void dma_descriptor_get_config_defaults(struct dma_descriptor_config *config)
{
	Assert(config);

	/* Set descriptor as valid */
	config->descriptor_valid = true;
	/* Disable event output */
	config->event_output_selection = DMA_EVENT_OUTPUT_DISABLE;
	/* No block action */
	config->block_action = DMA_BLOCK_ACTION_NOACT;
	/* Set beat size as byte */
	config->beat_size = DMA_BEAT_SIZE_BYTE;
	/* Enable source increment */
	config->src_increment_enable = true;
	/* Enable destination increment */
	config->dst_increment_enable = true;
	/* Step size is applied to the destination address */
	config->step_selection = DMA_STEPSEL_DST;
	/* Address increment is beat size mutipled by 1*/
	config->step_size = DMA_ADDRESS_INCREMENT_STEP_SIZE_1;
	/* Default transfer size is set to 0 */
	config->block_transfer_count = 0;
	/* Default source address is set to NULL */
	config->source_address = (uint32_t)NULL;
	/* Default destination address is set to NULL */
	config->destination_address = (uint32_t)NULL;
}

/**
 * \brief Update DMA descriptor
 *
 * This function can update the descriptor of an allocated DMA resource.
 *
 */
static inline void dma_update_descriptor(struct dma_resource *resource,
		DmacDescriptor* descriptor)
{
	Assert(resource);

	resource->descriptor = descriptor;
}

/**
 * \brief Reset DMA descriptor
 *
 * This function will reset the descriptor of an allocated DMA resource.
 *
 */
static inline void dma_reset_descriptor(struct dma_resource *resource)
{
	Assert(resource);

	resource->descriptor = NULL;
}

void dma_get_config_defaults(struct dma_resource_config *config);
enum status_code dma_allocate(struct dma_resource *resource,
		struct dma_resource_config *config);
enum status_code dma_free(struct dma_resource *resource);
enum status_code dma_start_transfer_job(struct dma_resource *resource);
void dma_abort_job(struct dma_resource *resource);
void dma_suspend_job(struct dma_resource *resource);
void dma_resume_job(struct dma_resource *resource);
void dma_descriptor_create(DmacDescriptor* descriptor,
	struct dma_descriptor_config *config);
enum status_code dma_add_descriptor(struct dma_resource *resource,
		DmacDescriptor* descriptor);

#endif /* DMA_H_INCLUDED */
