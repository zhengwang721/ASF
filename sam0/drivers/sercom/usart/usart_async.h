/**
 * \file
 *
 * \brief SAM0+ SERCOM USART Asynchronous Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
#ifndef USART_ASYNC_H_INCLUDED
#define USART_ASYNC_H_INCLUDED

#include "usart.h"

uint8_t _sercom_get_module_irq_index(struct usart_dev_inst *const dev_inst);

/**
 * \name Callback Management
 * {@
 */
void usart_async_register_callback(struct usart_dev_inst *const dev_inst,
		usart_async_callback_t *callback_func,
		enum usart_callback callback_type);

void usart_async_unregister_callback(struct usart_dev_inst *dev_inst,
		enum usart_callback callback_type);

/**
 * \brief Enables callback
 *
 * Enables the callback function registered by the \ref
 * usart_async_register_callback. The callback function will be called from the
 * interrupt handler when the conditions for the callback type are met.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_INVALID     If operation was not completed,
 *                                    due to invalid callback_type
 *
 */
static inline void usart_async_enable_callback(
		struct usart_dev_inst *const dev_inst,
		enum usart_callback callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Enable callback */
	dev_inst->callback_enable_mask |= (1 << callback_type);

}

/**
 * \brief Disable callback
 *
 * Disables the callback function registered by the \ref
 * usart_async_register_callback, and the callback will not be called
 * from the interrupt routine.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_INVALID     If operation was not completed,
 *                                    due to invalid callback_type
 *
 */
static inline void usart_async_disable_callback(
		struct usart_dev_inst *const dev_inst,
		enum usart_callback callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Disable callback */
	dev_inst->callback_enable_mask |= (0 << callback_type);
}

/**
 * @}
 */

/**
 * \name Writing and reading
 * {@
 */
enum status_code usart_async_write(struct usart_dev_inst *const dev_inst,
		const uint16_t tx_data);

enum status_code usart_async_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data);

enum status_code usart_async_write_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *tx_data, uint16_t length);

enum status_code usart_async_read_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *rx_data, uint16_t length);

void usart_async_cancel_transfer(struct usart_dev_inst *const dev_inst,
		enum usart_transceiver_type transceiver_type);

enum status_code usart_async_get_operation_status(
		struct usart_dev_inst *const dev_inst,
		enum usart_transceiver_type transceiver_type);
/**
 * @}
 */

/**
 * \name Writing and reading
 * {@
 */

void usart_async_handler(uint8_t instance);

/**
 * @}
 */

#endif /* USART_ASYNC_H_INCLUDED */
