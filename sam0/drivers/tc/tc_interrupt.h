/**
 * \file
 *
 * \brief SAMD20 SERCOM USART Asynchronous Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#ifndef TC_INTERRUPT_H_INCLUDED
#define TC_INTERRUPT_H_INCLUDED

#include "tc.h"



/**
 * \name Callback Management
 * {@
 */

void tc_register_callback(
		struct tc_module *const module,
		tc_callback_t callback_func,
		const enum tc_callback callback_type);

void tc_unregister_callback(
		struct tc_module *const module,
		const enum tc_callback callback_type);

/**
 * \brief Enables callback
 *
 * Enables the callback function registered by the \ref
 * tc_register_callback. The callback function will be called from the
 * interrupt handler when the conditions for the callback type are met.
 *
 * \param[in]     module Pointer to TC software instance struct
 * \param[in]     callback_type Callback type given by an enum
 */
static inline void tc_enable_callback(struct tc_module *const module,
		enum tc_callback callback_type){

	/* Sanity check arguments */
	Assert(module);

	/* Enable callback */
	module->callback_enable_mask |= (1 << callback_type);
}

/**
 * \brief Disable callback
 *
 * Disables the callback function registered by the \ref
 * tc_register_callback, and the callback will not be called
 * from the interrupt routine.
 *
 * \param[in]     module Pointer to TC software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 */
static inline void tc_disable_callback(struct tc_module *const module,
		enum tc_callback callback_type){

	/* Sanity check arguments */
	Assert(module);

	/* Disable callback */
	module->callback_enable_mask &= ~(1 << callback_type);
}

/**
 * @}
 */

/**
 * \name Callbacks
 * {@
 */
/*
void tc_error_callback(
		struct tc_module *const module_inst,
		enum status_code error_code);

void tc_callback(
		struct tc_module *const module_inst);
*/

/**
 * @}
 */

#endif /* USART_INTERRUPT_H_INCLUDED */
