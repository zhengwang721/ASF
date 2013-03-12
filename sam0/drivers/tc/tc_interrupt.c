/**
 * \file
 *
 * \brief SAMD20 Timer/Counter Interrupt Driver
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

/**
 * \brief Registers a callback
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by \ref tc_enable_callback,
 * in order for the interrupt handler to call it when the conditions for the
 * callback type is met.
 *
 * \param[in]     module      Pointer to TC software instance struct
 * \param[in]     callback_func Pointer to callback function
 * \param[in]     callback_type Callback type given by an enum
 *
 */
void tc_register_callback(struct tc_module *const module,
		tc_callback_t callback_func,
		enum tc_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(callback_func);

	/* Register callback function */
	module->callback[callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module->callback_reg_mask |= (1 << callback_type);
}

/**
 * \brief Unregisters a callback
 *
 * Unregisters a callback function implemented by the user.
 *
 * \param[in]     module Pointer to TC software instance struct
 * \param[in]     callback_func Pointer to callback function
 * \param[in]     callback_type Callback type given by an enum
 *
 */
void tc_unregister_callback(struct tc_module *const module,
		enum tc_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(module);

	/* Unregister callback function */
	module->callback[callback_type] = NULL;
	_tc_instances[_tc_get_module_irq_index(module)] = 0;

	/* Clear the bit corresponding to the callback_type */
	module->callback_reg_mask &= ~(1 << callback_type);


#include "tc_interrupt.h"
