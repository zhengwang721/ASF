/*
 * \file
 *
 * \brief SAM D2x Direct Memory Controller Driver
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

#include <string.h>
#include "dma.h"
#include "conf_dma.h"
#include "clock.h"
#include "system_interrupt.h"

struct _dma_module {
	volatile bool _dma_init;
	volatile uint32_t allocated_channels;
	uint8_t free_channels;
};

struct _dma_module _dma_inst = {
	._dma_init = false,
	.allocated_channels = 0,
	.free_channels = CONF_MAX_USED_CHANNEL_NUM,
};

/** Maximum retry counter for resuming a job transfer */
#define MAX_JOB_RESUME_COUNT    10000

/** DMA channel mask*/
#define DMA_CHANNEL_MASK   (0x1f)

/** Initial description section */
COMPILER_ALIGNED(16)
static struct dma_transfer_descriptor descriptor_section[CONF_MAX_USED_CHANNEL_NUM];
/** Initial write back memory section */
COMPILER_ALIGNED(16)
static struct dma_transfer_descriptor write_back_section[CONF_MAX_USED_CHANNEL_NUM];

/** Internal DMA resource pool */
static struct dma_resource dma_active_resource[CONF_MAX_USED_CHANNEL_NUM];

/**
 * \brief Find a free channel for a DMA resource.
 *
 * Find and set the priority level for the requested DMA resource.
 *
 * \param[in]  priority         DMA resource priority level
 *
 * \return Status of channel allocation
 * \retval DMA_INVALID_CHANNEL  No channel available
 * \retval count          Allocated channel for the DMA resource
 */
static uint8_t _dma_find_first_free_channel_and_allocate(uint8_t priority)
{
	uint8_t count;
	uint32_t tmp;
	bool allocated = false;

	system_interrupt_enter_critical_section();

	tmp = _dma_inst.allocated_channels;

	for (count = 0; count < CONF_MAX_USED_CHANNEL_NUM; ++count) {
		if (!(tmp & 0x00000001)) {
			/* If free channel found, set as allocated and return
			 *number */

			_dma_inst.allocated_channels |= 1 << count;
			_dma_inst.free_channels--;
			allocated = true;

			break;
		}

		tmp = tmp >> 1;
	}

	system_interrupt_leave_critical_section();

	if (!allocated) {
		return DMA_INVALID_CHANNEL;
	} else {
		/* Set priority level for the allocated channel */
		DMAC->CHID.reg = DMAC_CHID_ID(count);
		DMAC->CHCTRLB.reg &= ~DMAC_CHCTRLB_LVL_Msk;
		DMAC->CHCTRLB.reg |=  DMAC_CHCTRLB_LVL(priority);
		return count;
	}
}

/**
 * \brief Release an allocated DMA channel.
 *
 * \param[in]  channel  Channel id to be released.
 *
 */
static void _dma_release_channel(uint8_t channel)
{
	_dma_inst.allocated_channels &= ~(1 << channel);
	_dma_inst.free_channels++;
}

/**
 * \brief Configure the DMA resource.
 *
 * \param[in]  dma_resource Pointer to a DMA resource instance
 * \param[out] transfer_config Configurations of the DMA transfer
 *
 */
static void _dma_set_config(struct dma_resource *resource,
		struct dma_transfer_config *transfer_config)
{
	Assert(resource);
	Assert(transfer_config);

	system_interrupt_enter_critical_section();

	/** Select the DMA channel and clear software trigger */
	DMAC->CHID.reg = DMAC_CHID_ID(resource->channel_id);
	DMAC->SWTRIGCTRL.reg &= (uint32_t)(~(1 << resource->channel_id));

	/* Select transfer trigger */
	switch (transfer_config->transfer_trigger) {
	case DMA_TRIGGER_SOFTWARE:
		DMAC->SWTRIGCTRL.reg |= (1 << resource->channel_id);
		break;

	case DMA_TRIGGER_PERIPHERAL:
		DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_TRIGSRC(
				transfer_config->peripheral_trigger);
		break;

	case DMA_TRIGGER_EVENT:
		DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_EVIE | DMAC_CHCTRLB_EVACT(
				transfer_config->event_config.input_action);
		break;

	default:
		break;
	}
	/* Configure Trigger action */
	DMAC->CHCTRLB.bit.TRIGACT = transfer_config->trigger_action;

	/** Enable event output, the event output selection is configured in
	 * each transfer descriptor  */
	if (transfer_config->event_config.event_output_enable) {
		DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_EVOE;
	}

	system_interrupt_leave_critical_section();
}

/**
 * \brief DMA interrupt service routine.
 *
 */
void DMAC_Handler( void )
{
	uint8_t active_channel;
	struct dma_resource *resource;
	uint8_t isr;

	system_interrupt_enter_critical_section();

	/* Get Pending channel */
	active_channel =  DMAC->INTPEND.reg & DMAC_INTPEND_ID_Msk;

	/* Get active DMA resource based on channel */
	resource = &dma_active_resource[active_channel];

	/* Select the active channel */
	DMAC->CHID.reg = DMAC_CHID_ID(resource->channel_id);
	isr = DMAC->CHINTFLAG.reg;

	/* Calculate block transfer size of the DMA transfer */
	resource->transfered_size
		= descriptor_section[resource->channel_id].block_transfer_count
			- write_back_section[resource->channel_id].block_transfer_count;

	/* DMA channel interrupt handler */
	if (isr & DMAC_CHINTENCLR_TERR) {
		/* Clear transfer error flag */
		DMAC->CHINTFLAG.reg |= DMAC_CHINTENCLR_TERR;

		/* Set IO ERROR status */
		resource->job_status = STATUS_ERR_IO;

		/* Execute the callback function */
		if ((resource->callback_enable & (1<<DMA_CALLBACK_TRANSFER_ERROR)) &&
				(resource->callback[DMA_CALLBACK_TRANSFER_ERROR])) {
			resource->callback[DMA_CALLBACK_TRANSFER_ERROR](resource);
		}
	} else if (isr & DMAC_CHINTENCLR_TCMPL) {
		/* Clear the transfer complete flag */
		DMAC->CHINTFLAG.reg |= DMAC_CHINTENCLR_TCMPL;

		/* Set job status */
		resource->job_status = STATUS_OK;

		/* Execute the callback function */
		if ((resource->callback_enable & (1 << DMA_CALLBACK_TRANSFER_DONE)) &&
				(resource->callback[DMA_CALLBACK_TRANSFER_DONE])) {
			resource->callback[DMA_CALLBACK_TRANSFER_DONE](resource);
		}
	} else if (isr & DMAC_CHINTENCLR_SUSP) {
		/* Clear channel suspend flag */
		DMAC->CHINTFLAG.reg |= DMAC_CHINTENCLR_SUSP;

		/* Set job status */
		resource->job_status = STATUS_SUSPEND;

		/* Execute the callback function */
		if ((resource->callback_enable & (1 << DMA_CALLBACK_CHANNEL_SUSPEND)) &&
			(resource->callback[DMA_CALLBACK_CHANNEL_SUSPEND])){
			resource->callback[DMA_CALLBACK_CHANNEL_SUSPEND](resource);
		}
	}

	system_interrupt_leave_critical_section();
}

/**
 * \brief Initializes config with predefined default values.
 *
 * This function will initialize a given DMA configuration structure to
 * a set of known default values. This function should be called on
 * any new instance of the configuration structures before being
 * modified by the user application.
 *
 * The default configuration is as follows:
 *  \li software trigger is used as the transfer trigger
 *  \li priority level 0
 *  \li Only software/event trigger
 *  \li No event input /output
 * \param[out] config Pointer to the configuration
 *
 */
void dma_get_config_defaults(struct dma_transfer_config *config)
{
	Assert(config);

	/* Using software trigger */
	config->transfer_trigger = DMA_TRIGGER_SOFTWARE;
	/* Set as priority 0 */
	config->priority = DMA_PRIORITY_LEVEL_0;
	/* Only software/event trigger */
	config->peripheral_trigger = 0;

	/* Event configurations, no event input/output */
	config->event_config.input_action = DMA_EVENT_INPUT_NOACT;
	config->event_config.event_output_enable = false;
}

/**
 * \brief Allocate a DMA with configurations.
 *
 * This function will allocate a proper channel for a DMA transfer request.
 *
 * \param[in,out]  dma_resource Pointer to a DMA resource instance pointer
 * \param[in] transfer_config Configurations of the DMA transfer
 *
 * \return Status of the allocation procedure.
 *
 * \retval STATUS_OK The DMA resource was allocated successfully
 * \retval STATUS_ERR_NOT_FOUND DMA resource allocation failed
 */
enum status_code dma_allocate(struct dma_resource **resource,
		struct dma_transfer_config *config)
{
	uint8_t new_channel;
	uint8_t count;

	Assert(resource);

	system_interrupt_enter_critical_section();

	if (!_dma_inst._dma_init) {
		/* Initialize clocks for DMA */
		system_ahb_clock_set_mask(PM_AHBMASK_DMAC);
		system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBB,
				PM_APBBMASK_DMAC);

		/* Perform a software reset before enable DMA controller */
		DMAC->CTRL.reg &= ~DMAC_CTRL_DMAENABLE;
		DMAC->CTRL.reg = DMAC_CTRL_SWRST;

		/* Setup descriptor base address and write back section base
		 * address */
		DMAC->BASEADDR.reg = (uint32_t)descriptor_section;
		DMAC->WRBADDR.reg = (uint32_t)write_back_section;

		/* Enable all priority level at the same time */
		DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(0xf);

		/* Set all channels in the resource pool as not used */
		for (count = 0; count < CONF_MAX_USED_CHANNEL_NUM; count++) {
			dma_active_resource[count].channel_id = DMA_INVALID_CHANNEL;
		}

		_dma_inst._dma_init = true;
	}

	/* Find the proper channel */
	new_channel = _dma_find_first_free_channel_and_allocate(config->priority);

	/* If no channel available, return not found */
	if (new_channel == DMA_INVALID_CHANNEL) {
		system_interrupt_leave_critical_section();
		
		return STATUS_ERR_NOT_FOUND;
	}

	*resource = &dma_active_resource[new_channel];

	/* Set the channel */
	(*resource)->channel_id = new_channel;

	/** Perform a reset for the allocated channel */
	DMAC->CHID.reg = DMAC_CHID_ID((*resource)->channel_id);
	DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;

	/** Config the DMA control,channel registers and descriptors here */
	_dma_set_config(*resource, config);

	/* Log the DMA resouce into the internal DMA resource pool */
	dma_active_resource[(*resource)->channel_id] = **resource;

	system_interrupt_leave_critical_section();

	return STATUS_OK;
}

/**
 * \brief Free an allocated DMA resource.
 *
 * This function will Free an allocated DMA resource.
 *
 * \param[in,out] resource Pointer to the DMA resource
 *
 * \return Status of the free procedure.
 *
 * \retval STATUS_OK The DMA resource was freed successfully
 * \retval STATUS_BUSY The DMA resource was busy and can't be freed
 * \retval STATUS_ERR_NOT_INITIALIZED DMA resource was not initialized
 */
enum status_code dma_free(struct dma_resource *resource)
{
	Assert(resource);
	Assert(resource->channel_id != DMA_INVALID_CHANNEL);

	system_interrupt_enter_critical_section();

	/* Check if channel is busy */
	if (dma_is_busy(resource)) {
		system_interrupt_leave_critical_section();
		return STATUS_BUSY;
	}

	/* Check if DMA resource was not allocated */
	if (!(_dma_inst.allocated_channels & (1 << resource->channel_id))) {
		system_interrupt_leave_critical_section();
		return STATUS_ERR_NOT_INITIALIZED;
	}

	/* Release the DMA resource */
	_dma_release_channel(resource->channel_id);

	/* Reset the item in the DMA resource pool */
	memset(&dma_active_resource[resource->channel_id], 0x0,
			sizeof(struct dma_resource));
	dma_active_resource[resource->channel_id].channel_id
		= DMA_INVALID_CHANNEL;

	system_interrupt_leave_critical_section();

	return STATUS_OK;
}

/**
 * \brief Start a DMA transfer.
 *
 * This function will start a DMA transfer through an allocated DMA resource.
 *
 * \param[in,out] resource Pointer to the DMA resource
 * \param[in] transfer_descriptor Pointer to the DMA transfer descriptor
 *
 * \return Status of the release procedure.
 *
 * \retval STATUS_OK The transfer was started successfully
 * \retval STATUS_BUSY The DMA resource was busy and the transfer was not started
 * \retval STATUS_ERR_INVALID_ARG Transfer size is 0 and transfer was not started
 */
enum status_code dma_transfer_job(struct dma_resource *resource,
		struct dma_transfer_descriptor *transfer_descriptor)
{
	Assert(resource);
	Assert(resource->channel_id != DMA_INVALID_CHANNEL);

	system_interrupt_enter_critical_section();

	/* Check if resource was busy */
	if (resource->job_status == STATUS_BUSY) {
		system_interrupt_leave_critical_section();
		return STATUS_BUSY;
	}

	/* Check if transfer size is valid */
	if (transfer_descriptor->block_transfer_count == 0) {
		system_interrupt_leave_critical_section();
		return STATUS_ERR_INVALID_ARG;
	}

	/* Enable DMA interrupt */
	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_DMA);

	/* Set the interrupt flag */
	DMAC->CHID.reg = DMAC_CHID_ID(resource->channel_id);
	DMAC->CHINTENSET.reg = DMAC_CHINTENSET_TERR | DMAC_CHINTENSET_TCMPL |
			DMAC_CHINTENSET_SUSP;

	/* Set job status */
	resource->job_status = STATUS_BUSY;

	/* Set channel x descriptor 0 to the descriptor base address */
	descriptor_section[resource->channel_id] = *transfer_descriptor;


	/* Enable the transfer channel */
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_ENABLE;

	system_interrupt_leave_critical_section();

	return STATUS_OK;
}

/**
 * \brief Abort a DMA transfer.
 *
 * This function will abort a DMA transfer. The channel used for the DMA
 * resource will be disabled.
 * The block transfer count will be also calculated and output in the DMA
 * resource structure.
 *
 * \note The DMA resource will not be freed after calling this function.
 *
 * \param[in,out] resource Pointer to the DMA resource
 *
 */
void dma_abort_job(struct dma_resource *resource)
{
	Assert(resource);
	Assert(resource->channel_id != DMA_INVALID_CHANNEL);

	system_interrupt_enter_critical_section();

	DMAC->CHID.reg = DMAC_CHID_ID(resource->channel_id);
	DMAC->CHCTRLA.reg = 0;

	system_interrupt_leave_critical_section();

	/* Get transfered size */
	resource->transfered_size
		= descriptor_section[resource->channel_id].block_transfer_count
			- write_back_section[resource->channel_id].block_transfer_count;

	resource->job_status = STATUS_ABORTED;
}

/**
 * \brief Suspend a DMA transfer.
 *
 * This function will request to suspend the transfer of the DMA resource.
 *
 * \note This function will send the request only. A channel suspend interrupt
 * will be triggered and then the transfer is truely suspended.
 *
 * \param[in] resource Pointer to the DMA resource
 *
 */
void dma_suspend_job(struct dma_resource *resource)
{
	Assert(resource);
	Assert(resource->channel_id != DMA_INVALID_CHANNEL);

	system_interrupt_enter_critical_section();

	/* Select the channel */
	DMAC->CHID.reg = DMAC_CHID_ID(resource->channel_id);

	/* Send the suspend request */
	DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_CMD_SUSPEND;

	system_interrupt_leave_critical_section();
}

/**
 * \brief Resume a suspended DMA transfer.
 *
 * This function try to resume a suspended transfer of a DMA resource.
 *
 * \param[in] resource Pointer to the DMA resource
 *
 */
void dma_resume_job(struct dma_resource *resource)
{
	uint32_t bitmap_channel;
	uint32_t count = 0;

	Assert(resource);
	Assert(resource->channel_id != DMA_INVALID_CHANNEL);

	/* Get bitmap of the allocated DMA channel */
	bitmap_channel = (1 << resource->channel_id);

	/* Check if channel was suspended */
	if (resource->job_status != STATUS_SUSPEND) {
		return;
	}

	system_interrupt_enter_critical_section();

	/* Send resume request */
	DMAC->CHID.reg = DMAC_CHID_ID(resource->channel_id);
	DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_CMD_RESUME;

	system_interrupt_leave_critical_section();

	/* Check if transfer job resumed */
	for (count = 0; count < MAX_JOB_RESUME_COUNT; count++) {
		if ((DMAC->BUSYCH.reg & bitmap_channel) == bitmap_channel) {
			break;
		}
	}

	if (count < MAX_JOB_RESUME_COUNT) {
		/* Job resumed */
		resource->job_status = STATUS_BUSY;
	} else {
		/* Job resume timeout */
		resource->job_status = STATUS_ERR_TIMEOUT;
	}
}