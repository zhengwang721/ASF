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
 * \brief Configure DMA for a single transfer.
 *
 * \param[out] p_xdmac Module hardware register base address pointer.
 * \param[in] channel_num The used channel number.
 * \param[in] p_cfg   The configuration for used channel
 */
void xdmac_configure_transfer(Xdmac *p_xdmac,
		uint32_t channel_num, xdmac_channel_config_t *p_cfg)
{
	xdmac_channel_get_interrupt_status( p_xdmac, channel_num);
	xdmac_channel_set_source_addr(p_xdmac, channel_num, p_cfg->mbr_sa);
	xdmac_channel_set_destination_addr(p_xdmac, channel_num, p_cfg->mbr_da);
	xdmac_channel_set_microblock_control(p_xdmac, channel_num, p_cfg->mbr_ubc);
	xdmac_channel_set_block_control(p_xdmac, channel_num, p_cfg->mbr_bc);
	xdmac_channel_set_datastride_mempattern(p_xdmac, channel_num, p_cfg->mbr_ds);
	xdmac_channel_set_source_microblock_stride(p_xdmac, channel_num, p_cfg->mbr_sus);
	xdmac_channel_set_destination_microblock_stride(p_xdmac, channel_num, p_cfg->mbr_dus);
	xdmac_channel_set_config(p_xdmac, channel_num, p_cfg->mbr_cfg );
}