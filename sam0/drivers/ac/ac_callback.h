/**
 * \file
 *
 * \brief SAM D20 AC - Analog Comparator Callback Driver
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

#ifndef AC_CALLBACK_H_INCLUDED
#define AC_CALLBACK_H_INCLUDED

#include "ac.h"
#include "system_interrupt.h"
#include "status_codes.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \name Callback Management
 * {@
 */

/**
 * \brief Channel interrupt selection enum.
 *
 * This enum is used to select when a channel interrupt should occur.
 */
enum ac_channel_interrupt_selection {
	/** An interrupt will be generated when the comparator level is passed */
	AC_CHANNEL_INTERRUPT_SELECTION_TOGGLE         = AC_COMPCTRL_INTSEL_TOGGLE,
	/** An interrupt will be generated when the measurement goes above the compare level*/
	AC_CHANNEL_INTERRUPT_SELECTION_RISING         = AC_COMPCTRL_INTSEL_RISING,
	/** An interrupt will be generated when the measurement goes below the compare level*/
	AC_CHANNEL_INTERRUPT_SELECTION_FALLING        = AC_COMPCTRL_INTSEL_FALLING,
	/** 
	 * An interrupt will be generated when a new measurement is complete. 
	 * Interrupts will only be generated in single shot mode
	 */
	AC_CHANNEL_INTERRUPT_SELECTION_END_OF_COMPARE  = AC_COMPCTRL_INTSEL_EOC,
};

/**
 * \brief Window interrupt selection enum.
 *
 * This enum is used to select when a window interrupt should occur.
 */
enum ac_window_interrupt_selection {
	/** Interrupt is generated when the compare value goes above the window */
	AC_WINDOW_INTERRUPT_SELECTION_ABOVE    = AC_WINCTRL_WINTSEL0_ABOVE,
	/** Interrupt is generated when the compare value goes inside the window */
	AC_WINDOW_INTERRUPT_SELECTION_INSIDE   = AC_WINCTRL_WINTSEL0_INSIDE,
	/** Interrupt is generated when the compare value goes below the window */
	AC_WINDOW_INTERRUPT_SELECTION_BELOW    = AC_WINCTRL_WINTSEL0_BELOW,
	/** Interrupt is generated when the compare value goes outside the window */
	AC_WINDOW_INTERRUPT_SELECTION_OUTSIDE  = AC_WINCTRL_WINTSEL0_OUTSIDE,
};

/**
 * \brief Function used to setup interrupt selection of a channel
 *
 * This function is used to setup when an interrupt should occur
 * for a given channel.
 *
 * \note This must be done before enabling the channel. And after 
 * configure_ac_channel() as configure_ac_channel() will over write
 * these configurations.
 *
 *
 * \param[in]  module               Pointer to software instance struct
 * \param[in]  channel              Channel to setup
 * \param[in]  interrupt_selection  Interrupt selection for the given channel
 */
static inline void ac_callback_channel_interrupt_selection(
		struct ac_module *const module,
		const enum ac_chan_channel channel,
		const enum ac_channel_interrupt_selection interrupt_selection)
{
	Assert(module);
	Assert(module->hw);

	module->hw->COMPCTRL[channel].bit.INTSEL = interrupt_selection;
}

/**
 * \brief Function used to setup interrupt selection of a window
 *
 * This function is used to setup when an interrupt should occur
 * for a given window.
 *
 * \note This must be done before enabling the channel. And after 
 * configure_ac_channel() as configure_ac_channel() will over write
 * these configurations.
 *
 * \param[in]  module               Pointer to software instance struct
 * \param[in]  win_channel          Window channel to setup
 * \param[in]  interrupt_selection  Interrupt selection for the given channel
 *
 * \retval  STATUS_OK               Function exited successful
 * \retval  STATUS_ERR_INVALID_ARG  win_channel argument incorrect
 */
static inline enum status_code ac_callback_window_interrupt_selection(
		struct ac_module *const module,
		const enum ac_win_channel win_channel,
		const enum ac_window_interrupt_selection interrupt_selection)
{
	Assert(module);
	Assert(module->hw);

	uint8_t winctrl_mask = module->hw->WINCTRL.reg;

	if (win_channel == AC_WIN_CHANNEL_0) {
		winctrl_mask &= ~AC_WINCTRL_WINTSEL0_Msk;
		winctrl_mask |= interrupt_selection;
	}
#if defined(__DOXYGEN__) || (AC_PAIRS > 1)
	else if (win_channel == AC_WIN_CHANNEL_1) {
		winctrl_mask &= ~AC_WINCTRL_WINTSEL1_Msk;
		winctrl_mask = (interrupt_selection << (AC_WINCTRL_WINTSEL1_Pos -
		 AC_WINCTRL_WINTSEL0_Pos);
	} 
#endif /* #if defined(__DOXYGEN__) || (AC_PAIRS > 1) */
	else {
		return STATUS_ERR_INVALID_ARG ;
	}

	module->hw->WINCTRL.reg = winctrl_mask;

	return STATUS_OK;
}

enum status_code ac_register_callback(
		struct ac_module *const module,
		ac_callback_t callback_func,
		const enum ac_callback callback_type);

enum status_code ac_unregister_callback(
		struct ac_module *const module,
		const enum ac_callback callback_type);

/**
 * \brief Enables callback
 *
 * Enables the callback function registered by the \ref
 * ac_register_callback. The callback function will be called from the
 * interrupt handler when the conditions for the callback type are
 * met. This function will also enable the appropriate interrupts.
 *
 * \param[in]     module Pointer to AC software instance struct
 * \param[in]     callback_type Callback type given by an enum
 */
static inline void ac_enable_callback(
		struct ac_module *const module,
		const enum ac_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module);

	/* Set software flag for the callback */
	module->enable_callback_mask |= (1 << callback_type);
	/* Enable the interrupt for the callback */
	module->hw->INTENSET.reg = (1 << callback_type);
	/* Enable interrupts for AC module */
	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_AC);
}

/**
 * \brief Disables callback
 *
 * Disables the callback function registered by the \ref
 * ac_register_callback, and the callback will not be called from the
 * interrupt routine. The function will also disable the appropriate
 * interrupts.
 *
 * \param[in]     module Pointer to AC software instance struct
 * \param[in]     callback_type Callback type given by an enum
 */
static inline void ac_disable_callback(
		struct ac_module *const module,
		const enum ac_callback callback_type){
	/* Sanity check arguments */
	Assert(module);

	/* Set software flag for the callback */
	module->enable_callback_mask |= (1 << callback_type);
	/* Enable the interrupt for the callback */
	module->hw->INTENCLR.reg = (1 << callback_type);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* AC_CALLBACK_H_INCLUDED */
