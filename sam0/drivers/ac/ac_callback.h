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
	module->hw->INTENSET.reg |= (1 << callback_type);
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

	/* Clear software flag for the callback */
	module->enable_callback_mask &= ~(1 << callback_type);
	/* Disable the interrupt for the callback */
	module->hw->INTENCLR.reg &= ~(1 << callback_type);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* AC_CALLBACK_H_INCLUDED */
