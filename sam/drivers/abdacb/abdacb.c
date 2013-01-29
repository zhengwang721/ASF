/**
 * \file
 *
 * \brief ABDAC driver for SAM.
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

#include "abdacb.h"
#include "sysclk.h"
#include "sleepmgr.h"

/**
 * \internal
 * \brief ABDAC callback function pointer
 */
abdac_callback_t abdac_callback_pointer;

void abdac_get_config_defaults(struct abdac_config *const cfg)
{
	/* Sanity check arguments */
	Assert(cfg);

	/* Default configuration values */
	cfg->sample_rate_hz = ABDAC_SAMPLE_RATE_8000;
	cfg->data_word_format = ABDAC_DATE_16BIT;
	cfg->mono = false;
	cfg->cmoc = false;
}

bool abdac_init(struct abdac_dev_inst *const dev_inst, Abdacb *const abdac,
		struct abdac_config *const cfg)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(abdac);
	Assert(cfg);

	dev_inst->hw_dev = abdac;
	dev_inst->cfg = cfg;

	/* Enable APB clock for AES */
	sysclk_enable_peripheral_clock(abdac);

	/* Initialize the AES with new configurations */
	abdac_set_config(dev_inst);

	/* Disable APB clock for AES */
	sysclk_disable_peripheral_clock(abdac);

	return true;
}

void abdac_enable(struct abdac_dev_inst *const dev_inst)
{
	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
	sleepmgr_lock_mode(SLEEPMGR_ACTIVE);
}

void abdac_disable(struct abdac_dev_inst *const dev_inst)
{
	while(abdac_is_busy(dev_inst)) {
	}

	dev_inst->hw_dev->ABDACB_CR &= ~ABDACB_CR_EN;
	sleepmgr_unlock_mode(SLEEPMGR_ACTIVE);
	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
}

void abdac_set_config(struct abdac_dev_inst *const dev_inst)
{
	struct genclk_config gencfg;
	struct pll_config pcfg;

	while(abdac_is_busy(dev_inst)) {
	}

	/* Set the GCLK according to the sample rate */
	genclk_config_defaults(&gencfg, ABDACB_GCLK_NUM);

	switch (dev_inst->cfg->sample_rate_hz) {
	case ABDAC_SAMPLE_RATE_8000:
		/* CPUCLK 32M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 3, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 8M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 4);
		break;

	case ABDAC_SAMPLE_RATE_11025:
		/* CPUCLK 48M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 12M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 4);
		break;

	case ABDAC_SAMPLE_RATE_12000:
		/* CPUCLK 48M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 12M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 4);
		break;

	case ABDAC_SAMPLE_RATE_16000:
		/* CPUCLK 32M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 3, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 16M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 2);
		break;

	case ABDAC_SAMPLE_RATE_22050:
		/* CPUCLK 48M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 24M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 2);
		break;

	case ABDAC_SAMPLE_RATE_24000:
		/* CPUCLK 48M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 24M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 2);
		break;

	case ABDAC_SAMPLE_RATE_32000:
		/* CPUCLK 32M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 3, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 32M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 1);
		break;

	case ABDAC_SAMPLE_RATE_44100:
		/* CPUCLK 48M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 48M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 1);
		break;

	case ABDAC_SAMPLE_RATE_48000:
		/* CPUCLK 48M */
		pll_config_init(&pcfg, PLL_SRC_OSC0, 2, 96000000 /
				BOARD_OSC0_HZ);
		pll_enable(&pcfg, 0);
		sysclk_set_prescalers(0, 0, 0, 0, 0);
		pll_wait_for_lock(0);
		sysclk_set_source(SYSCLK_SRC_PLL0);
		/* GCLK 48M */
		genclk_enable_source(GENCLK_SRC_CLK_CPU);
		genclk_config_set_source(&gencfg, GENCLK_SRC_CLK_CPU);
		genclk_config_set_divider(&gencfg, 1);
		break;

	default:
		break;
	}

	genclk_enable(&gencfg, ABDACB_GCLK_NUM);

	/* Set the sampling rate related settings */
	dev_inst->hw_dev->ABDACB_CR |=
			ABDACB_CR_FS(dev_inst->cfg->sample_rate_hz) |
			ABDACB_CR_ALTUPR;

	/* Set the data word format */
	dev_inst->hw_dev->ABDACB_CR |=
			ABDACB_CR_DATAFORMAT(dev_inst->cfg->data_word_format);

	/* Set the mono mode */
	if (dev_inst->cfg->mono) {
		dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_MONO;
	} else {
		dev_inst->hw_dev->ABDACB_CR &= ~ABDACB_CR_MONO;
	}

	/* Set the common mode offset */
	if (dev_inst->cfg->cmoc) {
		dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_CMOC;
	} else {
		dev_inst->hw_dev->ABDACB_CR &= ~ABDACB_CR_CMOC;
	}

	while(abdac_is_busy(dev_inst)) {
	}

	/* Enable the module after GCLK ready. */
	dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_EN;

	while(abdac_is_busy(dev_inst)) {
	}
}

void abdac_sw_reset(struct abdac_dev_inst *const dev_inst)
{
	while(abdac_is_busy(dev_inst)) {
	}

	dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_SWRST;
}

void abdac_swap_channels(struct abdac_dev_inst *const dev_inst)
{
	while(abdac_is_busy(dev_inst)) {
	}

	if (dev_inst->hw_dev->ABDACB_CR & ABDACB_CR_SWAP) {
		dev_inst->hw_dev->ABDACB_CR &= ~ABDACB_CR_SWAP;
	} else {
		dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_SWAP;
	}
}

void abdac_write_data0(struct abdac_dev_inst *const dev_inst,
		uint32_t data)
{
	while(abdac_is_busy(dev_inst)) {
	}

	while(!abdac_is_tx_ready(dev_inst)) {
	}

	dev_inst->hw_dev->ABDACB_SDR0 = data;
}

void abdac_write_data1(struct abdac_dev_inst *const dev_inst,
		uint32_t data)
{
	while(abdac_is_busy(dev_inst)) {
	}

	while(!abdac_is_tx_ready(dev_inst)) {
	}

	dev_inst->hw_dev->ABDACB_SDR1 = data;
}

void abdac_set_volume0(struct abdac_dev_inst *const dev_inst, bool mute,
		uint32_t volume)
{
	if (mute) {
		dev_inst->hw_dev->ABDACB_VCR0 = ABDACB_VCR0_MUTE
				| ABDACB_VCR0_VOLUME(volume);
	} else {
		dev_inst->hw_dev->ABDACB_VCR0 = ABDACB_VCR0_VOLUME(volume);
	}
}

void abdac_set_volume1(struct abdac_dev_inst *const dev_inst, bool mute,
		uint32_t volume)
{
	if (mute) {
		dev_inst->hw_dev->ABDACB_VCR1 = ABDACB_VCR1_MUTE
				| ABDACB_VCR1_VOLUME(volume);
	} else {
		dev_inst->hw_dev->ABDACB_VCR1 = ABDACB_VCR1_VOLUME(volume);
	}
}

void abdac_enable_interrupt(struct abdac_dev_inst *const dev_inst,
		abdac_interrupt_source_t source)
{
	switch (source) {
	case ABDAC_INTERRUPT_TXRDY:
		dev_inst->hw_dev->ABDACB_IER = ABDACB_IER_TXRDY;
		break;

	case ABDAC_INTERRUPT_TXUR:
		dev_inst->hw_dev->ABDACB_IER = ABDACB_IER_TXUR;
		break;

	default:
		break;
	}
}

void abdac_disable_interrupt(struct abdac_dev_inst *const dev_inst,
		abdac_interrupt_source_t source)
{
	switch (source) {
	case ABDAC_INTERRUPT_TXRDY:
		dev_inst->hw_dev->ABDACB_IDR = ABDACB_IDR_TXRDY;
		break;

	case ABDAC_INTERRUPT_TXUR:
		dev_inst->hw_dev->ABDACB_IDR = ABDACB_IDR_TXUR;
		break;

	default:
		break;
	}
}

void abdac_clear_interrupt_flag(struct abdac_dev_inst *const dev_inst,
		abdac_interrupt_source_t source)
{
	switch (source) {
	case ABDAC_INTERRUPT_TXRDY:
		dev_inst->hw_dev->ABDACB_SCR = ABDACB_SCR_TXRDY;
		break;

	case ABDAC_INTERRUPT_TXUR:
		dev_inst->hw_dev->ABDACB_SCR = ABDACB_SCR_TXUR;
		break;

	default:
		break;
	}
}

void abdac_set_callback(struct abdac_dev_inst *const dev_inst,
		abdac_interrupt_source_t source, abdac_callback_t callback,
		uint8_t irq_level)
{
	abdac_callback_pointer = callback;
	irq_register_handler((IRQn_Type)ABDACB_IRQn, irq_level);
	abdac_enable_interrupt(dev_inst, source);
}

/**
 * \brief The ABDACB interrupt handler.
 */
void ABDACB_Handler(void)
{
	if (abdac_callback_pointer) {
		abdac_callback_pointer();
	} else {
		Assert(false); /* Catch unexpected interrupt */
	}
}

