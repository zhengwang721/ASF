/**
 * \file
 *
 * \brief ABDACB driver for SAM.
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
 * \brief ABDACB callback function pointer
 */
abdacb_callback_t abdacb_callback_pointer;

void abdacb_get_config_defaults(struct abdacb_config *const cfg)
{
	/* Sanity check arguments */
	Assert(cfg);

	/* Default configuration values */
	cfg->sample_rate_hz = ABDACB_SAMPLE_RATE_8000;
	cfg->data_word_format = ABDACB_DATE_16BIT;
	cfg->mono = false;
	cfg->cmoc = false;
}

bool abdacb_init(struct abdacb_dev_inst *const dev_inst, Abdacb *const abdacb,
		struct abdacb_config *const cfg)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(abdacb);
	Assert(cfg);

	dev_inst->hw_dev = abdacb;
	dev_inst->abdacb_cfg = cfg;

	/* Enable APB clock for AES */
	sysclk_enable_peripheral_clock(abdacb);

	/* Initialize the AES with new configurations */
	abdacb_set_config(dev_inst);

	/* Disable APB clock for AES */
	sysclk_disable_peripheral_clock(abdacb);

	return true;
}

void abdacb_enable(struct abdacb_dev_inst *const dev_inst)
{
	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
	sleepmgr_lock_mode(SLEEPMGR_ACTIVE);
}

void abdacb_disable(struct abdacb_dev_inst *const dev_inst)
{
	while(abdacb_is_busy(dev_inst)) {
	}

	dev_inst->hw_dev->ABDACB_CR &= ~ABDACB_CR_EN;
	sleepmgr_unlock_mode(SLEEPMGR_ACTIVE);
	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
}

void abdacb_set_config(struct abdacb_dev_inst *const dev_inst)
{
	struct genclk_config gencfg;
	struct pll_config pcfg;

	while(abdacb_is_busy(dev_inst)) {
	}

	/* Set the GCLK according to the sample rate */
	genclk_config_defaults(&gencfg, ABDACB_GCLK_NUM);

	switch (dev_inst->abdacb_cfg->sample_rate_hz) {
	case ABDACB_SAMPLE_RATE_8000:
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

	case ABDACB_SAMPLE_RATE_11025:
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

	case ABDACB_SAMPLE_RATE_12000:
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

	case ABDACB_SAMPLE_RATE_16000:
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

	case ABDACB_SAMPLE_RATE_22050:
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

	case ABDACB_SAMPLE_RATE_24000:
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

	case ABDACB_SAMPLE_RATE_32000:
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

	case ABDACB_SAMPLE_RATE_44100:
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

	case ABDACB_SAMPLE_RATE_48000:
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
			ABDACB_CR_FS(dev_inst->abdacb_cfg->sample_rate_hz) |
			ABDACB_CR_ALTUPR;

	/* Set the data word format */
	dev_inst->hw_dev->ABDACB_CR |=
			ABDACB_CR_DATAFORMAT(dev_inst->abdacb_cfg->data_word_format);

	/* Set the mono mode */
	if (dev_inst->abdacb_cfg->mono) {
		dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_MONO;
	} else {
		dev_inst->hw_dev->ABDACB_CR &= ~ABDACB_CR_MONO;
	}

	/* Set the common mode offset */
	if (dev_inst->abdacb_cfg->cmoc) {
		dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_CMOC;
	} else {
		dev_inst->hw_dev->ABDACB_CR &= ~ABDACB_CR_CMOC;
	}

	while(abdacb_is_busy(dev_inst)) {
	}

	/* Enable the module after GCLK ready. */
	dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_EN;

	while(abdacb_is_busy(dev_inst)) {
	}
}

void abdacb_sw_reset(struct abdacb_dev_inst *const dev_inst)
{
	while(abdacb_is_busy(dev_inst)) {
	}

	dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_SWRST;
}

void abdacb_swap_channels(struct abdacb_dev_inst *const dev_inst)
{
	while(abdacb_is_busy(dev_inst)) {
	}

	if (dev_inst->hw_dev->ABDACB_CR & ABDACB_CR_SWAP) {
		dev_inst->hw_dev->ABDACB_CR &= ~ABDACB_CR_SWAP;
	} else {
		dev_inst->hw_dev->ABDACB_CR |= ABDACB_CR_SWAP;
	}
}

void abdacb_write_data0(struct abdacb_dev_inst *const dev_inst,
		uint32_t data)
{
	while(abdacb_is_busy(dev_inst)) {
	}

	while(!abdacb_is_tx_ready(dev_inst)) {
	}

	dev_inst->hw_dev->ABDACB_SDR0 = data;
}

void abdacb_write_data1(struct abdacb_dev_inst *const dev_inst,
		uint32_t data)
{
	while(abdacb_is_busy(dev_inst)) {
	}

	while(!abdacb_is_tx_ready(dev_inst)) {
	}

	dev_inst->hw_dev->ABDACB_SDR1 = data;
}

void abdacb_set_volume0(struct abdacb_dev_inst *const dev_inst, bool mute,
		uint32_t volume)
{
	if (mute) {
		dev_inst->hw_dev->ABDACB_VCR0 = ABDACB_VCR0_MUTE
				| ABDACB_VCR0_VOLUME(volume);
	} else {
		dev_inst->hw_dev->ABDACB_VCR0 = ABDACB_VCR0_VOLUME(volume);
	}
}

void abdacb_set_volume1(struct abdacb_dev_inst *const dev_inst, bool mute,
		uint32_t volume)
{
	if (mute) {
		dev_inst->hw_dev->ABDACB_VCR1 = ABDACB_VCR1_MUTE
				| ABDACB_VCR1_VOLUME(volume);
	} else {
		dev_inst->hw_dev->ABDACB_VCR1 = ABDACB_VCR1_VOLUME(volume);
	}
}

void abdacb_enable_interrupt(struct abdacb_dev_inst *const dev_inst,
		abdacb_interrupt_source_t source)
{
	switch (source) {
	case ABDACB_INTERRUPT_TXRDY:
		dev_inst->hw_dev->ABDACB_IER = ABDACB_IER_TXRDY;
		break;

	case ABDACB_INTERRUPT_TXUR:
		dev_inst->hw_dev->ABDACB_IER = ABDACB_IER_TXUR;
		break;

	default:
		break;
	}
}

/**
 * \brief This function disables the ABDACB interrupts
 *
 * \param dev_inst Device structure pointer..
 * \param source ABDACB Interrupts to be disabled
 */
void abdacb_disable_interrupt(struct abdacb_dev_inst *const dev_inst,
		abdacb_interrupt_source_t source)
{
	switch (source) {
	case ABDACB_INTERRUPT_TXRDY:
		dev_inst->hw_dev->ABDACB_IDR = ABDACB_IDR_TXRDY;
		break;

	case ABDACB_INTERRUPT_TXUR:
		dev_inst->hw_dev->ABDACB_IDR = ABDACB_IDR_TXUR;
		break;

	default:
		break;
	}
}

void abdacb_clear_interrupt_flag(struct abdacb_dev_inst *const dev_inst,
		abdacb_interrupt_source_t source)
{
	switch (source) {
	case ABDACB_INTERRUPT_TXRDY:
		dev_inst->hw_dev->ABDACB_SCR = ABDACB_SCR_TXRDY;
		break;

	case ABDACB_INTERRUPT_TXUR:
		dev_inst->hw_dev->ABDACB_SCR = ABDACB_SCR_TXUR;
		break;

	default:
		break;
	}
}

void abdacb_set_callback(struct abdacb_dev_inst *const dev_inst,
		abdacb_interrupt_source_t source, abdacb_callback_t callback,
		uint8_t irq_level)
{
	abdacb_callback_pointer = callback;
	irq_register_handler((IRQn_Type)ABDACB_IRQn, irq_level);
	abdacb_enable_interrupt(dev_inst, source);
}

/**
 * \brief The ABDACB interrupt handler.
 */
void ABDACB_Handler(void)
{
	if (abdacb_callback_pointer) {
		abdacb_callback_pointer();
	} else {
		Assert(false); /* Catch unexpected interrupt */
	}
}

