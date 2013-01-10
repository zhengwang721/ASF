/**
 *
 * \file
 *
 * \brief IISC driver for SAM.
 *
 * This file defines a useful set of functions for the IISC on SAM devices.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#include "iisc.h"
#include "sysclk.h"
#include "sleepmgr.h"

void iis_enable(struct iisc_device *dev_inst)
{
	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
	sleepmgr_lock_mode(SLEEPMGR_ACTIVE);
}

void iis_disable(struct iisc_device *dev_inst)
{
	sleepmgr_unlock_mode(SLEEPMGR_ACTIVE);
	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
}

status_code_t iis_init(struct iisc_device *const dev_inst,
		Iisc *iisc, struct iisc_config *const iisc_conf)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(iisc);
	Assert(iisc_conf);

	dev_inst->hw_dev = iisc;
	dev_inst->iisc_cfg = iisc_conf;

	/* Enable APB clock for AESA */
	sysclk_enable_peripheral_clock(iisc);

	iis_reset(dev_inst);

	/* Set the master/slave mode */
	if (iisc_conf->master) {
		iisc->IISC_MR |= IISC_MR_MODE;
	} else {
		iisc->IISC_MR &= ~IISC_MR_MODE;
	}

	/* Set the sampling rate related settings */
	iisc->IISC_MR |= IISC_MR_IMCKFS(iisc_conf->fs_ratio);

	/* Set the data word format */
	iisc->IISC_MR |= IISC_MR_DATALENGTH(iisc_conf->data_word_format);

	/* Set the master clock mode */
	if ((((uint32_t)iisc_conf->fs_ratio + 1) * 16)
			> (iisc_conf->slot_length * 2)) {
		iisc->IISC_MR |= IISC_MR_IMCKMODE;
	} else {
		iisc->IISC_MR &= ~IISC_MR_IMCKMODE;
	}

	/* Set the mono mode */
	if (iisc_conf->num_tx_channels < 2) {
		iisc->IISC_MR |= IISC_MR_TXMONO;
	} else {
		iisc->IISC_MR &= ~IISC_MR_TXMONO;
	}
	if (iisc_conf->num_rx_channels < 2) {
		iisc->IISC_MR |= IISC_MR_RXMONO;
	} else {
		iisc->IISC_MR &= ~IISC_MR_RXMONO;
	}

	/* Set the loop mode */
	if (iisc_conf->loopback) {
		iisc->IISC_MR |= IISC_MR_RXLOOP;
	} else {
		iisc->IISC_MR &= ~IISC_MR_RXLOOP;
	}

	/* Disable APB clock for AESA */
	sysclk_disable_peripheral_clock(iisc);

	return STATUS_OK;
}


status_code_t iis_write(struct iisc_device *dev_inst, uint32_t data)
{
	uint32_t timeout = IISC_RETRY_VALUE;

	while (!(dev_inst->hw_dev->IISC_SR & IISC_SR_TXRDY) && timeout) {
		--timeout;
	}

	if (timeout == 0) {
		return ERR_TIMEOUT;
	}

	dev_inst->hw_dev->IISC_THR = data;

	return STATUS_OK;
}

status_code_t iis_read(struct iisc_device *dev_inst, uint32_t *data)
{
	uint32_t timeout = IISC_RETRY_VALUE;

	while (!(dev_inst->hw_dev->IISC_SR & IISC_SR_RXRDY) && timeout) {
		--timeout;
	}

	if (timeout == 0) {
		return ERR_TIMEOUT;
	}

	*data = dev_inst->hw_dev->IISC_RHR;

	return STATUS_OK;
}



