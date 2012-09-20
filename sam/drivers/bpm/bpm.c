/**
 * \file
 *
 * \brief BPM driver
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#include "compiler.h"
#include "bpm.h"

void bpm_configure_power_scaling(Bpm *bpm, uint32_t ps_value, uint32_t pscm)
{
	uint32_t pmcon = 0;

	/* Read last PM_CON value */
	pmcon = bpm->BPM_PMCON;
	/* Clear last PS Value */
	pmcon &= ~BPM_PMCON_PS_Msk;
	/* Write new PS Value */
	pmcon |= BPM_PMCON_PS(ps_value);
	/* Clear last PSM Value */
	pmcon &= ~BPM_PMCON_PSCM;
	/* Write new PS Value */
	pmcon |= (pscm ? BPM_PMCON_PSCM : 0);
	/* Power Scaling Change Request */
	pmcon |= BPM_PMCON_PSCREQ;
	/* Unlock PMCON register */
	BPM_UNLOCK(PMCON);
	/* Write back PM_CON value */
	bpm->BPM_PMCON = pmcon;
}

void bpm_enable_fast_wakeup(Bpm *bpm)
{
	uint32_t pmcon = bpm->BPM_PMCON | BPM_PMCON_FASTWKUP;
	BPM_UNLOCK(PMCON);
	bpm->BPM_PMCON = pmcon;
}

void bpm_disable_fast_wakeup(Bpm *bpm)
{
	uint32_t pmcon = bpm->BPM_PMCON & (~BPM_PMCON_FASTWKUP);
	BPM_UNLOCK(PMCON);
	bpm->BPM_PMCON = pmcon;
}

void bpm_set_clk32_source(Bpm *bpm, uint32_t source)
{
	uint32_t pmcon;

	/* Read PMCON first */
	pmcon = bpm->BPM_PMCON;
	if (source == BPM_CLK32_SOURCE_OSC32K) {
		/* Clear CK32S for OSC32K */
		pmcon &= ~BPM_PMCON_CK32S;
	} else {
		/* Set CK32S for RC32K */
		pmcon |= BPM_PMCON_CK32S;
	}

	/* Unlock PMCON register */
	BPM_UNLOCK(PMCON);
	bpm->BPM_PMCON = pmcon;
}

uint32_t bpm_get_backup_wakeup_cause(Bpm *bpm)
{
	return bpm->BPM_BKUPWCAUSE;
}

void bpm_enable_wakeup_source(Bpm *bpm, uint32_t sources)
{
	/* Write BKUPWEN value */
	bpm->BPM_BKUPWEN |= sources;
}

void bpm_disable_wakeup_source(Bpm *bpm, uint32_t sources)
{
	/* Write BKUPWEN value */
	bpm->BPM_BKUPWEN &= ~sources;
}

void bpm_enable_backup_pin(Bpm *bpm, uint32_t backup_pins)
{
	/* Write back BKUPPMUX value */
	bpm->BPM_BKUPPMUX |= backup_pins;
}

void bpm_disable_backup_pin(Bpm *bpm, uint32_t backup_pins)
{
	/* Write back BKUPPMUX value */
	bpm->BPM_BKUPPMUX &= ~backup_pins;
}

void bpm_enable_io_retention(Bpm *bpm)
{
	bpm->BPM_IORET |= BPM_IORET_RET;
}

void bpm_disable_io_retention(Bpm *bpm)
{
	bpm->BPM_IORET &= ~BPM_IORET_RET;
}

void bpm_enable_interrupt(Bpm *bpm, uint32_t sources)
{
	bpm->BPM_IER = sources;
}

void bpm_disable_interrupt(Bpm *bpm, uint32_t sources)
{
	bpm->BPM_IDR = sources;
}

uint32_t bpm_get_interrupt_mask(Bpm *bpm)
{
	return bpm->BPM_IMR;
}

uint32_t bpm_get_interrupt_status(Bpm *bpm)
{
	return bpm->BPM_ISR;
}

void bpm_clear_interrupt(Bpm *bpm, uint32_t sources)
{
	bpm->BPM_ICR = sources;
}

uint32_t bpm_get_status(Bpm *bpm)
{
	return bpm->BPM_SR;
}

uint32_t bpm_get_version(Bpm *bpm)
{
	return bpm->BPM_VERSION;
}
