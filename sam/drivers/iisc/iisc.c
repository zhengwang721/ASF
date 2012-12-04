/**
 *
 * \file
 *
 * \brief IISC driver for SAM.
 *
 * This file defines a useful set of functions for the IISC on SAM devices.
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

#include "iisc.h"
#include "sysclk.h"
#include "sleepmgr.h"

void iisc_enable(Iisc *iisc)
{
	sysclk_enable_peripheral_clock(iisc);
	sleepmgr_lock_mode(SLEEPMGR_ACTIVE);
}

void iisc_disable(Iisc *iisc)
{
	sleepmgr_unlock_mode(SLEEPMGR_ACTIVE);
	sysclk_enable_peripheral_clock(iisc);
}

status_code_t iisc_set_config(Iisc *iisc, struct iisc_config *iisc_conf)
{
	uint32_t slot_length;

	if (iisc_conf->data_word_format < 4) {
		slot_length = 32;
	} else if (iisc_conf->data_word_format < 6) {
		slot_length = 16;
	} else {
		slot_length = 8;
	}

	iisc_reset(iisc);

	struct genclk_config gencfg;
	struct pll_config pcfg;

	/* Set the master/slave mode */
	if (iisc_conf->mode) {
		/* Set the GCLK according to the sample rate */
		genclk_config_defaults(&gencfg, IISC_GCLK_NUM);

		switch (iisc_conf->sample_rate_hz) {
		case IISC_SAMPLE_RATE_32000:
			/* CPUCLK 32M */
			pll_config_init(&pcfg, PLL_SRC_OSC0, 3, 96000000 /
					BOARD_OSC0_HZ);
			pll_enable(&pcfg, 0);
			sysclk_set_prescalers(0, 0, 0, 0, 0);
			pll_wait_for_lock(0);
			sysclk_set_source(SYSCLK_SRC_PLL0);
			/* GCLK according fs ratio */
			genclk_enable_source(GENCLK_SRC_CLK_CPU);
			genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
			genclk_config_set_divider(&gencfg, 1024 / iisc_conf->fs_ratio);
			break;

		case IISC_SAMPLE_RATE_48000:
			/* CPUCLK 48M */
			pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
					BOARD_OSC0_HZ);
			pll_enable(&pcfg, 0);
			sysclk_set_prescalers(0, 0, 0, 0, 0);
			pll_wait_for_lock(0);
			sysclk_set_source(SYSCLK_SRC_PLL0);
			/* GCLK according fs ratio */
			genclk_enable_source(GENCLK_SRC_CLK_CPU);
			genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
			genclk_config_set_divider(&gencfg, 1024 / iisc_conf->fs_ratio);
			break;

		case IISC_SAMPLE_RATE_96000:
			/* CPUCLK 48M */
			pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
					BOARD_OSC0_HZ);
			pll_enable(&pcfg, 0);
			sysclk_set_prescalers(0, 0, 0, 0, 0);
			pll_wait_for_lock(0);
			sysclk_set_source(SYSCLK_SRC_PLL0);
			/* GCLK according fs ratio */
			genclk_enable_source(GENCLK_SRC_CLK_CPU);
			genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
			genclk_config_set_divider(&gencfg, 512 / iisc_conf->fs_ratio);
			break;

		case IISC_SAMPLE_RATE_192000:
			/* CPUCLK 48M */
			pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
					BOARD_OSC0_HZ);
			pll_enable(&pcfg, 0);
			sysclk_set_prescalers(0, 0, 0, 0, 0);
			pll_wait_for_lock(0);
			sysclk_set_source(SYSCLK_SRC_PLL0);
			/* GCLK according fs ratio */
			genclk_enable_source(GENCLK_SRC_CLK_CPU);
			genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
			genclk_config_set_divider(&gencfg, 256 / iisc_conf->fs_ratio);
			break;

		default:
			break;
		}

		genclk_enable(&gencfg, IISC_GCLK_NUM);

		iisc->IISC_MR |= IISC_MR_MODE;
	} else {
		iisc->IISC_MR &= ~IISC_MR_MODE;
	}

	/* Set the sampling rate related settings */
	iisc->IISC_MR |= IISC_MR_IMCKFS(iisc_conf->fs_ratio / 16 - 1);

	/* Set the data word format */
	iisc->IISC_MR |= IISC_MR_DATALENGTH(iisc_conf->data_word_format);

	/* Set the master clock mode */
	if ((uint32_t)iisc_conf->fs_ratio > (slot_length * 2)) {
		iisc->IISC_MR |= IISC_MR_IMCKMODE;
	} else {
		iisc->IISC_MR &= ~IISC_MR_IMCKMODE;
	}

	/* Set the mono mode */
	if (iisc_conf->tx_mono) {
		iisc->IISC_MR |= IISC_MR_TXMONO;
	} else {
		iisc->IISC_MR &= ~IISC_MR_TXMONO;
	}
	if (iisc_conf->rx_mono) {
		iisc->IISC_MR |= IISC_MR_RXMONO;
	} else {
		iisc->IISC_MR &= ~IISC_MR_RXMONO;
	}

	/* Set the loop mode */
	if (iisc_conf->loop) {
		iisc->IISC_MR |= IISC_MR_RXLOOP;
	} else {
		iisc->IISC_MR &= ~IISC_MR_RXLOOP;
	}

	return STATUS_OK;
}


status_code_t iisc_write(Iisc *iisc, uint32_t data)
{
	uint32_t timeout = IISC_RETRY_VALUE;

	while (!(iisc->IISC_SR & IISC_SR_TXRDY) && timeout) {
		--timeout;
	}

	if (timeout == 0) {
		return ERR_TIMEOUT;
	}

	iisc->IISC_THR = data;

	return STATUS_OK;
}

status_code_t iisc_read(Iisc *iisc, uint32_t *data)
{
	uint32_t timeout = IISC_RETRY_VALUE;

	while (!(iisc->IISC_SR & IISC_SR_RXRDY) && timeout) {
		--timeout;
	}

	if (timeout == 0) {
		return ERR_TIMEOUT;
	}

	*data = iisc->IISC_RHR;

	return STATUS_OK;
}



