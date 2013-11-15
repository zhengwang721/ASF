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

#include "dmac.h"
#include "conf_dmac.h"
 
#define DMA_INVALID_CHANNEL			0xff

//Hidden dma system level init in dma_allocate
static bool _dma_init = false;

//setup initial description section
static struct dma_transfer_descriptor descriptor_section[CONF_MAX_USED_CHANNEL_NUM];
//Setup initial write back memory section
static struct dma_transfer_descriptor write_back_section[CONF_MAX_USED_CHANNEL_NUM];

//DMA resource pools
static struct dma_resource dma_active_resource[CONF_MAX_USED_CHANNEL_NUM];

struct _dma_module {
	volatile uint32_t allocated_channels;
	uint8_t free_channels;
};

struct _dma_module _dma_inst = {
	.allocated_channels = 0,
	.free_channels = CONF_MAX_USED_CHANNEL_NUM;  //The definition in the current instance_dmac.h is 12 which should be 32 if according to datasheet
};

static uint8_t _dma_find_first_free_channel_and_allocate(uint8_t priority)
{
	uint8_t count;
	uint32_t tmp;
	bool allocated = false;

	if (priority >= CONF_MAX_USED_CHANNEL_NUM) {
		priority = CONF_MAX_USED_CHANNEL_NUM - 1;
	}

	system_interrupt_enter_critical_section();

	tmp = _dma_inst.allocated_channels;

	for(count = priority; count < CONF_MAX_USED_CHANNEL_NUM; ++count) {
		if(!(tmp & (1<<priority))) {
			/* If free channel found, set as allocated and return number */
			_dma_inst.allocated_channels |= 1 << count;
			_dma_inst.free_channels--;
			allocated = true;

			break;
		}

		tmp = tmp >> 1;
	}

	if (!allocated) {
		tmp = 1<<priority;
		
		for(count = priority-1; count >=0 ; count--) {
			if(!(tmp & (1<<(priority-1)))) {
				/* If free channel found, set as allocated and return number */
				_dma_inst.allocated_channels |= 1 << count;
				_dma_inst.free_channels--;
				allocated = true;

				break;
			}

			tmp = tmp << 1;
		}
	}

	system_interrupt_leave_critical_section();

	if(!allocated) {
		return DMA_INVALID_CHANNEL;
	} else {
		return count;
	}
}

static void _dma_release_channel(uint8_t channel)
{
	system_interrupt_enter_critical_section();

	_dma_inst.allocated_channels &= ~(1 << channel);
	_dma_inst.free_channels++;

	system_interrupt_leave_critical_section();
}

static void _dma_set_config(struct dma_resource *dma_resource,
								struct dma_transfer_config *transfer_config)
{
	Assert(dma_resource);
	Assert(transfer_config);
	
	//TODO: Set actual configurations for a dma resource
	
	// Setup Control register, only CRC is under consideration.
	// Priority level is ignored as we using 32 level fixed priorities
	// The DMA controller is enabled during init phase
	//TODO: a global config for CRC?
	if (transfer_config->crc) {
		//Some additional logic control for the CRC should be taken
		//Or just simply applied to all channels with defect of a might be slower performance
		//for all channels as CRC calculation
		DMAC->CTRL |= DMAC_CTRL_CRCENABLE;
		DMAC->CRCCTRL |= DMAC_CRCCTRL_CRCSRC(dma_resource);
		//CRC using default configurations: 
	}
	
	//Using default settings for DMA channel configurations.
	DMAC->CHID.reg = DMAC_CHID_ID(dma_resource->channel_id);
	DMAC->SWTRIGCTRL.reg &= (uint32_t)(~(1 << dma_resource->channel_id));

	/* One trigger required for each transaction */
	DMAC->CHCTRLB.reg |=  DMAC_CHCTRLB_TRIGACT_TRANSACTION;

	switch (transfer_config->transfer_trigger) {
		case DMA_TRIGGER_SOFTWARE :
			DMAC->SWTRIGCTRL.reg |= (1 << dma_resource->channel_id);
			break;

		case DMA_TRIGGER_PERIPHERAL:
			DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_TRIGSRC(transfer_config->dma_peripheral_trigger_index);
			break;

		case DMA_TRIGGER_EVENT:
			DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_EVIE | DMAC_CHCTRLB_EVACT(transfer_config->event_config.input_action);
			break;

		default :
			break;
	};

	/** Enable event output, the event output selection is configured in each transfer descriptor  */
	if (transfer_config->event_config.dma_event_output_enable) {
		DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_EVOE;
	}
	
	//DMA detail descriptors are handled by apps
	descriptor_section[dma_resource->channel_id] = transfer_config->transfer_descriptor;
	
	//Update beat size if different
	descriptor_section[dma_resource->channel_id].block_transfer_control.BEATSIZE = DMAC_BTCTRL_BEATSIZE(transfer_config->beat_size);
}

void DMAC_Handler( void )
{
	uint8_t active_channel;
	
	struct dma_resource dma_resource;
	
	uint8_t isr = DMAC->CHINTFLAG.reg;

	//Lock DMA interrupt?

	active_channel = (DMAC->ACTIVE.reg >> 8 & 0x1f);
	dma_resource = dma_active_resource[active_channel];
	DMAC->CHID.reg = DMAC_CHID_ID(dma_resource->channel_id);

	//TODO Error handler?
	//CALLBACKs for all
	if (isr & DMAC_CHINTENCLR_TERR) {
		DMAC->CHINTFLAG.reg |= DMAC_CHINTENCLR_TERR;
		dma_resource->job_status = ERR_IO_ERROR;
		if ((dma_resource->enabled_callbacks & 1 << DMA_CALLBACK_TRANSFER_ERROR)
			&& (dma_resource->callback[DMA_CALLBACK_TRANSFER_ERROR])){
			dma_resource->callback[DMA_CALLBACK_TRANSFER_ERROR](&dma_resource);
		}
	} else if (isr & DMAC_CHINTENCLR_TCMPL) {
		DMAC->CHINTFLAG.reg |= DMAC_CHINTENCLR_TCMPL;
		dma_resource->job_status = STATUS_OK;
		if ((dma_resource->enabled_callbacks & 1 << DMA_CALLBACK_TRANSFER_DONE)
			&& (dma_resource->callback[DMA_CALLBACK_TRANSFER_DONE])){
			dma_resource->callback[DMA_CALLBACK_TRANSFER_DONE](&dma_resource);
		}
	} else if (isr & DMAC_CHINTENCLR_SUSP) {
		DMAC->CHINTFLAG.reg |= DMAC_CHINTENCLR_TCMPL;
		dma_resource->job_status = ERR_INVALID_ARG;
		if ((dma_resource->enabled_callbacks & 1 << DMA_CALLBACK_CHANNEL_SUSPEND)
			&& (dma_resource->callback[DMA_CALLBACK_CHANNEL_SUSPEND])){
			dma_resource->callback[DMA_CALLBACK_CHANNEL_SUSPEND](&dma_resource);
		}
	}
}

void dma_get_config_defaults(struct dma_transfer_config *transfer_config)
{
	Assert(config);
	
	config->transfer_trigger = DMA_TRIGGER_SOFTWARE;
	config->priority = 0;
	config->crc = false;
	config->beat_size = DMA_BEAT_SIZE_BYTE;

	/** NULL setting for events as using software trigger by default */
	
	/** DMA transfer descriptor defines a real transfer and is always fulfilled by apps */
}

enum status_code dma_allocate(struct dma_resource *dma_resource,
								struct dma_transfer_config *transfer_config)
{
	uint8_t new_channel;

	Assert(dma_resource);

	if (!_dma_init) {
		//TODO: do DMA HW init phase here
		system_ahb_clock_set_mask(PM_AHBMASK_DMAC);
		system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBB, PM_APBBMASK_DMAC);
		
		DMAC->CTRL.reg = DMAC_CTRL_SWRST;
		DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE;
		
		_dma_init = true;
	}
	
	new_channel = _dma_find_first_free_channel_and_allocate(transfer_config->priority);

	if(new_channel == DMA_INVALID_CHANNEL) {
		return STATUS_ERR_NOT_FOUND;
	}

	dma_resource->channel_id = new_channel;

	/** Perform a reset for the allocated channel */
	DMAC->CHID.reg = DMAC_CHID_ID(dma_resource->channel_id);
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;

	/** Config the DMA control,channel registers and descriptors here */
	_dma_set_config(dma_resource, transfer_config);

	return STATUS_OK;
}

enum status_code dma_update_resource(struct dma_resource *dma_resource,
								struct dma_transfer_config *transfer_config)
{
	Assert(dma_resource);

	/* Check if channel is busy */
	if(dma_is_busy(dma_resource)) {
		return STATUS_BUSY;
	}

	if (!(_dma_inst.allocated_channels & (1<<dma_resource->channel_id))) {
		return STATUS_ERR_NOT_INITIALIZED;
	}
	
	system_interrupt_enter_critical_section();
	
	/** Update configurations with channel */
	_dma_set_config(dma_resource, transfer_config);
	
	system_interrupt_leave_critical_section();
}

enum status_code dma_release(struct dma_resource *dma_resource)
{
	Assert(dma_resource);

	/* Check if channel is busy */
	if(dma_is_busy(dma_resource)) {
		return STATUS_BUSY;
	}

	if (!(_dma_inst.allocated_channels & (1<<dma_resource->channel_id))) {
		return STATUS_ERR_NOT_INITIALIZED;
	}

	dma_active_resource[dma_resource->channel_id] = NULL;

	_dma_release_channel(dma_resource->channel_id);

	return STATUS_OK;
}

enum status_code dma_transfer_job(struct dma_resource *dma_resource)
{
	Assert(dma_resource);
	
	if (dma_resource->job_status == ERR_BUSY) {
		return ERR_BUSY;  // waiting for channel ready
	}
	
	if (descriptor_section[dma_resource->channel_id].block_count == 0) {
		return ERR_INVALID_ARG;
	}
	
	DMAC->CHID.reg = DMAC_CHID_ID(dma_resource->channel_id);
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_ENABLE;

	
	//TODO: ENABLE Channel suspend / Transfer complete / Transfer error interrupt

	dma_resource->job_status = ERR_BUSY;

	dma_active_resource[dma_resource->channel_id] = *dma_resource;
	
	return STATUS_OK;
}

void dma_abort_job(struct dma_resource *dma_resource)
{
	Assert(dma_resource);

	//TODO: Disable Channel suspend / Transfer complete / Transfer error interrupt
	
	DMAC->CHID.reg = DMAC_CHID_ID(dma_resource->channel_id);
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_ENABLE;
	
	dma_resource->job_status = ERR_FLUSHED;
}

enum status_code dma_get_job_status(struct dma_resource *dma_resource)
{
	Assert(dma_resource);
	
	return dma_resource->job_status;
}

bool dma_is_busy(struct dma_resource *resource)
{
	Assert(resource);

	return (resource->job_status == ERR_BUSY) ;
}

void dma_enable_callback(struct dma_resource *dma_resource, enum dma_callback_type type)
{
	Assert(dma_resource);
	
	dma_resource->callback_enable |= 1<<type;
}

void dma_disable_callback(struct dma_resource *dma_resource, enum dma_callback_type type)
{
	Assert(dma_resource);
	
	dma_resource->callback_enable &= ~(1<<type);
}

void dma_register_callback(struct dma_resource *dma_resource, dma_callback_t callback, enum dma_callback_type type)
{
	Assert(dma_resource);
	
	dma_resource->callback[type] = callback;
}

void dma_unregister_callback(struct dma_resource *dma_resource, enum dma_callback_type type)
{
	Assert(dma_resource);

	dma_resource->callback[type] = NULL;
}