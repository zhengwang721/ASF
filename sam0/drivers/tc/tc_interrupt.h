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
/** Enum for the possible callback types for the TC module. */
enum tc_callback
{
	/** Callback for TC overflow */
	TC_CALLBACK_OVERFLOW,
	/** Callback for error */
	TC_CALLBACK_ERROR,
	/** Callback for Match/Capture channel 0 */
	TC_CALLBACK_MC_CHANNEL0,
	/** Callback for Match/Capture channel 1 */
	TC_CALLBACK_MC_CHANNEL1,
#if !defined(__DOXYGEN__)
	/** Number of available callbacks. */
	TC_CALLBACK_N,
#endif
};

/**
 * \name Callback Management
 * {@
 */
void tc_register_callback(struct tc_module *const module,
		tc_callback_t callback_func,
		enum tc_callback_type callback_type);

void tc_unregister_callback(struct tc_module *const module,
		enum tc_callback_type callback_type);

/**
 * \brief Enables callback
 *
 * Enables the callback function registered by the \ref
 * tc_register_callback. The callback function will be called from the
 * interrupt handler when the conditions for the callback type are met.
 *
 * \param[in]     module Pointer to TC software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_INVALID     If operation was not completed,
 *                                    due to invalid callback_type
 */
static inline void tc_enable_callback(struct tc_module *const module,
		enum tc_callback_type callback_type){

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
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_INVALID     If operation was not completed,
 *                                    due to invalid callback_type
 *
 */
static inline void tc_disable_callback(struct tc_module *const module,
		enum tc_callback_type callback_type){

	/* Sanity check arguments */
	Assert(module);

	/* Disable callback */
	//module->callback_enable_mask &= ~(1 << callback_type);
}

/**
 * @}
 */


/**
 * \internal ISR handler for TC
 */
#ifdef ID_TC0
void TC0_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_tc_interrupt_handler(0);
}
#endif
#ifdef ID_TC1
void TC1_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_tc_interrupt_handler(1);
}
#endif
#ifdef ID_TC2
void TC2_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_tc_interrupt_handler(2);
}
#endif
#ifdef ID_TC3
void TC3_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_tc_interrupt_handler(3);
}
#endif
#ifdef ID_TC4
void TC4_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_tc_interrupt_handler(4);
}
#endif
#ifdef ID_TC5
void TC5_Handler(void)
{
	/* Call appropriate interrupt handler. */
#ifdef CUSTOM
	_tc_custom_handler();
#elif
	_tc_interrupt_handler(5);
#endif
}
#endif
#ifdef ID_TC6
void TC6_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_tc_interrupt_handler(6);
}
#endif
#ifdef ID_TC7
void TC7_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_tc_interrupt_handler(7);
}
#endif

/**
 * \internal Default interrupt handler
 *
 * \param[in] instance TC instance used.

static void _tc_default_handler(uint8_t instance)
{
	Assert(false);
}
 */

/**
 * \brief Interrupt Handler for TC module
 *
 * Handles interrupts as they occur, and it will run callback functions
 * which are registered and enabled.
 *
 * \param[in]  instance  ID of the SERCOM instance calling the interrupt
 *                       handler.
 */
void _tc_interrupt_handler(uint8_t instance)
{
	/* Temporary variables */
	uint16_t interrupt_status;
	uint16_t callback_status;

	/* Get device instance from the look-up table */
	struct tc_module *module
		= (struct tc_module *)_tc_instances[instance];

	/* Pointer to the hardware module instance */
	if(module->counter_size == TC_COUNTER_SIZE_8BIT){
		TcCount8 *const tc_hw = &(module->hw->COUNT8);
	}
	else if(module->counter_size == TC_COUNTER_SIZE_16BIT){
		TcCount16 *const tc_hw = &(module->hw->COUNT16);
	}
	else{
		TcCount32 *const tc_hw = &(module->hw->COUNT32);
	}

	/* Wait for the synchronization to complete */
	while (tc_is_syncing(module)) {
		/* Wait for sync */
	}

	/* Read and mask interrupt flag register */
	interrupt_status = tc_hw->INTFLAG.reg;
	callback_status = module->callback_reg_mask
			&module->callback_enable_mask;

	/* Check if an Overflow interrupt has occurred */
	if (interrupt_status & TC_INTFLAG_OVF){
		if (callback_status & (1 << TC_CALLBACK_OVERFLOW)) {
			/* Invoke registered and enabled calback function */
			(*(module->callback[TC_CALLBACK_OVERFLOW]))(module);
			/* Clear interrupt flag */
			module->hw->COUNT8->INTFLAG.reg = TC_INTFLAG_OVF;
		}
	}
	/* Check if an Error interrupt has occurred */
	if (interrupt_status & TC_INTFLAG_ERR){
		if (callback_status & (1 << TC_CALLBACK_ERROR)) {
			/* Invoke registered and enabled calback function */
			(*(module->callback[TC_CALLBACK_ERROR]))(module);
			/* Clear interrupt flag */
			module->hw->COUNT8->INTFLAG.reg = TC_INTFLAG_ERR;
		}
	}
	/* Check if an Match/Capture Channel 0 interrupt has occurred */
	if (interrupt_status & (1<<TC_INTFLAG_MC_Pos)){
		if (callback_status & (1 << TC_CALLBACK_MC_CHANNEL0)) {
			/* Invoke registered and enabled calback function */
			(*(module->callback[TC_CALLBACK_MC_CHANNEL0]))(module);
			/* Clear interrupt flag */
			module->hw->COUNT8->INTFLAG.reg = TC_INTFLAG_MC(1);
		}
	}
	/* Check if an Match/Capture Channel 1 interrupt has occurred */
	if (interrupt_status & (2<<TC_INTFLAG_MC_Pos)){
		if (callback_status & (1 << TC_CALLBACK_MC_CHANNEL1)) {
			/* Invoke registered and enabled calback function */
			(*(module->callback[TC_CALLBACK_MC_CHANNEL1]))(module);
			/* Clear interrupt flag */
			module->hw->COUNT8->INTFLAG.reg = TC_INTFLAG_MC(2);
		}
	}
}

/**
 * \name Callbacks
 * {@
 */
/*
void <namespace>_error_callback(
		struct <namespace>_module *const module_inst,
		enum status_code error_code);

void <namespace>_callback(
		struct <namespace_module *const module_inst,
		enum <namespace>_channel channel);

*/
/**
 * @}
 */

#endif /* USART_INTERRUPT_H_INCLUDED */
