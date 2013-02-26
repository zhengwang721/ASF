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
#ifndef USART_INTERRUPT_H_INCLUDED
#define USART_INTERRUPT_H_INCLUDED

#include "usart.h"

uint8_t _sercom_get_module_irq_index(struct usart_module *const module);

#ifndef __DOXYGEN__
void _usart_write_buffer(struct usart_module *const module,
		uint8_t *tx_data, uint16_t length);
void _usart_read_buffer(struct usart_module *const module,
		uint8_t *rx_data, uint16_t length);
#endif
/**
 * \defgroup asfdoc_samd20_sercom_usart_interrupt Interrupt USART API
 *
 * This is the overview of the interrupt driven API for the SERCOM USART driver.
 *
 * Configuration of the driver is done by using the base polled init function.
 * When the asynchronous part of the driver is included the polled API will
 * be extended with the interrupt API listed below. All functions that are run
 * from interrupt context (and are none-blocking) have the postfix _job appended
 * to the function name.
 *
 * Interrupt operations are based on callbacks that are registered with
 * the driver using the \ref usart_register_callback function. The
 * callback functions will be run from the SERCOM USART interrupt handler. All
 * interrupt flags will be handled and cleared by the internal interrupt
 * handler; making the interrupt flags transparent to the user defined
 * callbacks.
 *
 * \warning Never execute large portions of code in the callbacks. These
 * are run from the interrupt routine, and thus having long callbacks will
 * keep the processor in the interrupt handler for an equally long time.
 * An oft used way to handle this is to use global flags signalling the
 * main application that an interrupt event has happened, and only do the
 * minimal needed processing in the callback.
 *
 * @{
 */

/**
 * \name Callback Management
 * @{
 */
void usart_register_callback(struct usart_module *const module,
		usart_callback_t callback_func,
		enum usart_callback callback_type);

void usart_unregister_callback(struct usart_module *module,
		enum usart_callback callback_type);

/**
 * \brief Enables callback
 *
 * Enables the callback function registered by the \ref
 * usart_register_callback. The callback function will be called from the
 * interrupt handler when the conditions for the callback type are met.
 *
 * \param[in]     module Pointer to USART software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_INVALID     If operation was not completed,
 *                                    due to invalid callback_type
 *
 */
static inline void usart_enable_callback(
		struct usart_module *const module,
		enum usart_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module);

	/* Enable callback */
	module->callback_enable_mask |= (1 << callback_type);

}

/**
 * \brief Disable callback
 *
 * Disables the callback function registered by the \ref
 * usart_register_callback, and the callback will not be called
 * from the interrupt routine.
 *
 * \param[in]     module Pointer to USART software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_INVALID     If operation was not completed,
 *                                    due to invalid callback_type
 *
 */
static inline void usart_disable_callback(
		struct usart_module *const module,
		enum usart_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module);

	/* Disable callback */
	module->callback_enable_mask |= (0 << callback_type);
}

/**
 * @}
 */

/**
 * \name Writing and reading
 * @{
 */
enum status_code usart_write_job(struct usart_module *const module,
		const uint16_t tx_data);

enum status_code usart_read_job(struct usart_module *const module,
		uint16_t *const rx_data);

enum status_code usart_write_buffer_job(struct usart_module *const module,
		uint8_t *tx_data, uint16_t length);

enum status_code usart_read_buffer_job(struct usart_module *const module,
		uint8_t *rx_data, uint16_t length);

void usart_abort_job(struct usart_module *const module,
		enum usart_transceiver_type transceiver_type);

enum status_code usart_get_job_status(
		struct usart_module *const module,
		enum usart_transceiver_type transceiver_type);
/**
 * @}
 */

/**
 * \name Writing and reading
 * @{
 */

void _usart_interrupt_handler(uint8_t instance);

/**
 * @}
 */

/**
 * @}
 */
#endif /* USART_INTERRUPT_H_INCLUDED */
