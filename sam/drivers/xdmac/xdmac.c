/**
 * \file
 *
 * \brief SAM XDMA Controller (XDMAC) driver.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include  "xdmac.h"

/**
 * \brief Initialize XDMA driver instance.
 *
 * \param[out] p_module_inst Pointer to XDMA driver instance
 */
void xdmac_init(xdmac_module_t *p_module_inst)
{
	uint32_t j;

	p_module_inst->p_xdmac = XDMAC;
 
	for (j = 0; j < p_module_inst->numChannels; j ++) {
		p_module_inst->xdmac_channels[j].state = XDMAC_STATE_FREE;
	}	
}

/**
 * \brief Configure DMA for a single transfer.
 *
 * \param[out] p_module_inst Pointer to XDMA driver instance
 * \param[in] ul_num The used channel number.
 * \param[in] p_cfg   The configuration for used channel
 * \param[in] ul_desc_cfg   The configuration for next descriptor
 * \param[in] ul_desc_addr The address for next descriptor
 */
enum status_code xdmac_configure_transfer(xdmac_module_t *p_module_inst,
		uint32_t ul_num, xdmac_channel_config_t *p_cfg,
		uint32_t ul_desc_cfg, uint32_t ul_desc_addr)
{
	Assert(p_module_inst != NULL);

	Xdmac *p_xdmac = p_module_inst->p_xdmac;
	xdmac_channel_get_interrupt_status( p_xdmac, ul_num);

	if (p_module_inst->xdmac_channels[ul_num].state == XDMAC_STATE_FREE) {
		p_module_inst->xdmac_channels[ul_num].state == XDMAC_STATE_ALLOCATED;
	} else {
		return STATUS_ERR_BUSY;
	}

	/* Linked List is enabled */
	if ((ul_desc_cfg & XDMAC_CNDC_NDE) == XDMAC_CNDC_NDE_DSCR_FETCH_EN) {
		if ((ul_desc_cfg & XDMAC_CNDC_NDVIEW_Msk) == XDMAC_CNDC_NDVIEW_NDV0) {
			xdmac_channel_set_config(p_xdmac, ul_num, p_cfg->mbr_cfg );
			xdmac_channel_set_source_addr(p_xdmac, ul_num, p_cfg->mbr_sa);
			xdmac_channel_set_destination_addr(p_xdmac, ul_num, p_cfg->mbr_da);
		}
		if ((ul_desc_cfg & XDMAC_CNDC_NDVIEW_Msk) == XDMAC_CNDC_NDVIEW_NDV1) {
			xdmac_channel_set_config(p_xdmac, ul_num, p_cfg->mbr_cfg );
		}
		xdmac_channel_set_descriptor_addr(p_xdmac, ul_num, ul_desc_addr, 0);
		xdmac_channel_set_descriptor_control(p_xdmac, ul_num, ul_desc_cfg);
	}
	/* LLI is disabled. */
	else
	{
		xdmac_channel_set_source_addr(p_xdmac, ul_num, p_cfg->mbr_sa);
		xdmac_channel_set_destination_addr(p_xdmac, ul_num, p_cfg->mbr_da);
		xdmac_channel_set_microblock_control(p_xdmac, ul_num, p_cfg->mbr_ubc);
		xdmac_channel_set_block_control(p_xdmac, ul_num, p_cfg->mbr_bc);
		xdmac_channel_set_datastride_mempattern(p_xdmac, ul_num, p_cfg->mbr_ds);
		xdmac_channel_set_source_microblock_stride(p_xdmac, ul_num, p_cfg->mbr_sus);
		xdmac_channel_set_destination_microblock_stride(p_xdmac, ul_num, p_cfg->mbr_dus);
		xdmac_channel_set_config(p_xdmac, ul_num, p_cfg->mbr_cfg );
		xdmac_channel_set_descriptor_addr(p_xdmac, ul_num, 0, 0);
		xdmac_channel_set_descriptor_control(p_xdmac, ul_num, 0);
	}
	return STATUS_OK;
}

/**
 * \brief Start XDMA transfer.
 *
 * \param[out] p_module_inst Pointer to XDMA driver instance.
 * \param[in] ul_num The channel number.
 */
void xdmac_start_transfer(xdmac_module_t *p_module_inst,
	uint32_t ul_num)
{
	Assert(p_module_inst != NULL);

	Xdmac *p_xdmac = p_module_inst->p_xdmac;

	/* Change state to transferring */
	xdmac_channel_enable(p_xdmac, ul_num);
	xdmac_enable_interrupt(p_xdmac, ul_num);

}

/**
 * \brief Stop XDMA transfer.
 *
 * \param[out] p_module_inst Pointer to XDMA driver instance.
 * \param[in] ul_num The channel number.
 */
void xdmac_stop_transfer(xdmac_module_t *p_module_inst,
		uint32_t ul_num)
{
 	Assert(p_module_inst != NULL);

	Xdmac *p_xdmac = p_module_inst->p_xdmac;

	/* Disable channel */
	xdmac_channel_disable(p_xdmac, ul_num);
	/* Disable interrupts */
	xdmac_channel_disable_interrupt(p_xdmac, ul_num, 0xFF);
	/* Clear pending status */
	xdmac_channel_get_interrupt_status(p_xdmac, ul_num);
	xdmac_get_interrupt_status(p_xdmac);
}
	