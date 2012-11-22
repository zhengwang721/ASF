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

#include <usart_async.h>



/**
 * \internal 
 *
 *
 * param[]
 *
 * returns 
 * retval  
 */
void _usart_async_write_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *tx_data, uint16_t length)
{

	/* Write parameters to the device instance */
	dev_inst->remaining_tx_buffer_length = length;
	dev_inst->tx_buffer_ptr = tx_data;

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Enable the Data Register Empty Interrupt Flag (DREIF) */
	usart_module->INTENSET = SERCOM_USART_DREIF_bm;
}

/**
 * \internal 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 */
void _usart_async_read_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *rx_data, uint16_t length)
{
	/* Set length for the buffer and the pointer, and let
	 * the interrupt handler do the rest */
	dev_inst->remaining_rx_buffer_length = length;
	dev_inst->rx_buffer_ptr;
}


/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 */
void usart_async_register_callback(struct usart_dev_inst *const dev_inst,
		const usart_async_callback_t *const callback_func,
		enum usart_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(callback_func);

	/* Register callback function */
	dev_inst->callback[callback_type] = callback_func;
	//TODO: fix casting
	_sercom_instances[_sercom_get_module_irq_index(dev_inst)] =
			*(uint32_t *)dev_inst;

	/* Set the bit corresponding to the callback_type */
	dev_inst->callback_reg_mask |= (1 << callback_type);
}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
void usart_async_unregister_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Unregister callback function */
	dev_inst->callback[callback_type] = NULL;
	_sercom_instances[_sercom_get_module_irq_index(dev_inst)] = 0;

	/* Clear the bit corresponding to the callback_type */
	dev_inst->callback_reg_mask |= (0 << callback_type);
}



/**
 * \brief 
 *
 *
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_enable_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Enable callback */
	dev_inst->callback_enable_mask |= (0 << callback_type);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Enable the interrupt flag */
	switch (callback_type){

		case USART_CALLBACK_TYPE_BUFFER_TRANSMITTED:
			usart_module->INTENSET = SERCOM_USART_TXCIF_bm;
			break;

		case USART_CALLBACK_TYPE_BUFFER_RECEIVED:
			usart_module->INTENSET = SERCOM_USART_RXCIF_bm;
			break;

		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;

	}

	return STATUS_OK;

}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_disable_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Disable callback */
	dev_inst->callback_enable_mask |= (1 << callback_type);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Disable the interrupt flag */
	switch (callback_type){

		case USART_CALLBACK_TYPE_BUFFER_TRANSMITTED:
			usart_module->INTENCLR = SERCOM_USART_TXCIF_bm;
			break;

		case USART_CALLBACK_TYPE_BUFFER_RECEIVED:
			usart_module->INTENCLR = SERCOM_USART_RXCIF_bm;
			break;

		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;

	}

	return STATUS_OK;
}



/**
 * \brief 
 *
 * \note Namespace
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_write(struct usart_dev_inst *const dev_inst,
		const uint16_t tx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	/* Check if the USART transmitter is busy */
	if (dev_inst->remaining_tx_buffer_length > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Call internal write buffer function with length 1 */
	_usart_async_write(dev_inst, (uint8_t *)&tx_data, 1);

	return STATUS_OK;
}


/**
 * \brief 
 *
 * \note Namespace
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
//TODO: differ on number of bits for char_size??
enum status_code usart_async_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Check if the USART receiver is busy */
	if (dev_inst->remaining_rx_buffer_length > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Call internal read buffer function with length 1 */
	_usart_async_read_buffer(dev_inst, (uint8_t *)rx_data, 1);

	return STATUS_OK;
}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_write_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *tx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Check if the USART transmitter is busy */
	if (dev_inst->remaining_tx_buffer_length) {
		return STATUS_ERR_BUSY;
	}

	/* Issue internal asynchronous write */
	_usart_async_write(dev_inst, tx_data, length);

	return STATUS_OK;
}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_read_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *rx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Check if the USART receiver is busy */
	if (dev_inst->remaining_rx_buffer_length) {
		return STATUS_ERR_BUSY;
	}

	/* Issue internal asynchronous read */
	_usart_async_read(dev_inst, rx_data, length);

	return STATUS_OK;

}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
void usart_async_cancel_transmission(struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Clear the interrupt flag in order to prevent the transmission
	 * complete callback to fire */
	usart_module->INTFLAGS |= SERCOM_USART_TXCIF_bm;

	/* Clear the software transmission buffer */
	dev_inst->remaining_tx_buffer_length = 0;
}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
void usart_async_cancel_reception(struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Clear the interrupt flag in order to prevent the receive
	 * complete callback to fire */
	usart_module->INTFLAGS |= SERCOM_USART_RXCIF_bm;

	/* Clear the software reception buffer */
	dev_inst->remaining_rx_buffer_length = 0;
}


