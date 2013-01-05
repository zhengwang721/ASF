/**
 * \file
 *
 * \brief Analog Comparator interface driver for SAM4L.
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

#include "acifc.h"
#include "sysclk.h"
#include "sleepmgr.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_acifc_group Analog Comparator (AC)
 *
 * Driver for the Analog Comparator. This driver provides access to the main
 * features of the AC controller.
 *
 * @{
 */

acifc_callback_t acifc_callback_pointer;

/**
 * \brief Configure a set of AC channels in normal mode
 *
 * \param p_acifc   Pointer to an ACIFC instance
 * \param ac_chan_cfg   AC channel configuration value
 * \param nb_chan   The number of AC channel
 *
 */
void acifc_channel_configure(Acifc *p_acifc, const acifc_channel_cfg_t *ac_chan_cfg,
		uint8_t nb_chan)
{
	p_acifc->ACIFC_CONF[nb_chan].ACIFC_CONF =
			ACIFC_CONF_HYS(ac_chan_cfg->hysteresis_value) |
			ACIFC_CONF_INSELN(ac_chan_cfg->negative_input) |
			ACIFC_CONF_MODE(ac_chan_cfg->mode) |
			ACIFC_CONF_IS(ac_chan_cfg->interrupt_settings);
	if(ac_chan_cfg->alwayson) {
		p_acifc->ACIFC_CONF[nb_chan].ACIFC_CONF |= ACIFC_CONF_ALWAYSON;
	}
	if(ac_chan_cfg->fast) {
		p_acifc->ACIFC_CONF[nb_chan].ACIFC_CONF |= ACIFC_CONF_FAST;
	}
	if(ac_chan_cfg->event_negative) {
		p_acifc->ACIFC_CONF[nb_chan].ACIFC_CONF |= ACIFC_CONF_EVENN;
	}
	if(ac_chan_cfg->event_positive) {
		p_acifc->ACIFC_CONF[nb_chan].ACIFC_CONF |= ACIFC_CONF_EVENP;
	}
}

/**
 * \brief Configure one AC channel in Window mode
 *
 * \param p_acifc   Pointer to an ACIFC instance
 * \param ac_window_cfg   AC Window configuration value
 * \param nb_window   The number of the AC window pair
 *
 */
void acifc_window_configure(Acifc *p_acifc, const acifc_window_cfg_t *ac_window_cfg,
		uint8_t nb_window)
{
	p_acifc->ACIFC_CONFW[nb_window].ACIFC_CONFW =
			ACIFC_CONFW_WEVSRC(ac_window_cfg->window_event) |
			ACIFC_CONFW_WIS(ac_window_cfg->interrupt_settings);
	if(ac_window_cfg->window_event_enable) {
		p_acifc->ACIFC_CONFW[nb_window].ACIFC_CONFW |= ACIFC_CONFW_WEVEN;
	}
	if(ac_window_cfg->window_mode) {
		p_acifc->ACIFC_CONFW[nb_window].ACIFC_CONFW |= ACIFC_CONFW_WFEN;
	}
}

/**
 * \brief Configure the ACIFC module
 *
 * \param p_acifc   Pointer to an ACIFC instance
 * \param ac   AC configuration value
 *
 */
void acifc_configure(Acifc *p_acifc, const acifc_cfg_t *ac_cfg)
{
	if(ac_cfg->actest) {
		p_acifc->ACIFC_CTRL |=  ACIFC_CTRL_ACTEST;
	}
	if(ac_cfg->eventen) {
		p_acifc->ACIFC_CTRL |=  ACIFC_CTRL_EVENTEN;
	}
}

/**
 * \brief Enable ACIFC Module.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 *
 */
void acifc_enable(Acifc *p_acifc)
{
	sysclk_enable_peripheral_clock(p_acifc);
	sleepmgr_lock_mode(SLEEPMGR_BACKUP);
	p_acifc->ACIFC_CTRL |= ACIFC_CTRL_EN;
}

/**
 * \brief Disable ACIFC Module.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 *
 */
void acifc_disable(Acifc *p_acifc)
{
	p_acifc->ACIFC_CTRL &= ~ACIFC_CTRL_EN;
	sysclk_disable_peripheral_clock(p_acifc);
	sleepmgr_unlock_mode(SLEEPMGR_BACKUP);
}

/**
 * \brief Set callback for ACIFC
 *
 * \param p_acifc Base address of the ACIFC module
 * \param callback callback function pointer.
 * \param irq_line  interrupt line.
 * \param irq_level interrupt level.
 * \param interrupt_flags interrupt mask.
 */
void acifc_set_callback(Acifc *p_acifc, acifc_callback_t callback,
		uint8_t irq_line, uint8_t irq_level, uint32_t interrupt_flags)
{
	acifc_callback_pointer = callback;
	irq_register_handler((IRQn_Type) irq_line, irq_level);
	acifc_enable_interrupt(p_acifc, interrupt_flags);
}

/**
 * \internal
 * \brief Common ACIFC interrupt handler
 *
 * The optional callback used by the interrupt handler is set by the
 * acifc_set_callback() function.
 */
static void acifc_interrupt(void)
{
	if (acifc_callback_pointer) {
		acifc_callback_pointer();
	}
}

/**
 * \brief Interrupt handler for ACIFC interrupt.
 */
void ACIFC_Handler(void)
{
	acifc_interrupt();
}

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
