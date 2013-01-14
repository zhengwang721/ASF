/**
 * \file
 *
 * \brief SAMD20 SERCOM USART Asynchronous Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#include "usart_async.h"

/**
 * \internal      Asynchronous write of a buffer with a given length
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[in]     tx_data  Pointer to data to be transmitted
 * \param[in]     length   Length of data buffer
 *
 */
void _usart_async_write_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *tx_data, uint16_t length)
{
	/* Write parameters to the device instance */
	dev_inst->remaining_tx_buffer_length = length;
	dev_inst->tx_buffer_ptr = tx_data;
	dev_inst->tx_status = STATUS_IN_PROGRESS;

	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_module = &(dev_inst->hw_dev->USART);

	/* Enable the Data Register Empty  and  TX Complete Interrupt */
	usart_module->INTENSET.reg = (USART_INTERRUPT_FLAG_DATA_BUFFER_EMPTY |
			USART_INTERRUPT_FLAG_TX_COMPLETE);
}

/**
 * \internal      Asynchronous read of a buffer with a given length
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[in]     rx_data  Pointer to data to be received
 * \param[in]     length   Length of data buffer
 *
 */
void _usart_async_read_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *rx_data, uint16_t length)
{
	/* Set length for the buffer and the pointer, and let
	 * the interrupt handler do the rest */
	dev_inst->remaining_rx_buffer_length = length;
	dev_inst->rx_buffer_ptr = rx_data;
	dev_inst->rx_status = STATUS_IN_PROGRESS;

	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_module = &(dev_inst->hw_dev->USART);

	/* Enable the RX Complete Interrupt */
	usart_module->INTENSET.reg = USART_INTERRUPT_FLAG_RX_COMPLETE;
}

/**
 * \brief Registers a callback
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by \ref usart_async_register_callback,
 * in order for the interrupt handler to call it when the conditions for the
 * callback type is met.
 *
 * \param[in]     dev_inst      Pointer to USART software instance struct
 * \param[in]     callback_func Pointer to callback function
 * \param[in]     callback_type Callback type given by an enum
 *
 */
void usart_async_register_callback(struct usart_dev_inst *const dev_inst,
		usart_async_callback_t *callback_func,
		enum usart_callback callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(callback_func);

	/* Register callback function */
	dev_inst->callback[callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	dev_inst->callback_reg_mask |= (1 << callback_type);
}

/**
 * \brief Unregisters a callback
 *
 * Unregisters a callback function which is implemented by the user.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[in]     callback_func Pointer to callback function
 * \param[in]     callback_type Callback type given by an enum
 *
 */
void usart_async_unregister_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback callback_type)
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
 * \brief Asynchronous write a single char
 *
 * Sets up the driver to write the data given. If registered and enabled,
 * a callback function will be called when the transmit is completed.
 *
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[in]     tx_data  Data to transfer
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_BUSY        If operation was not completed,
 *                                    due to the USART module being busy.
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
	_usart_async_write_buffer(dev_inst, (uint8_t *)&tx_data, 1);

	return STATUS_OK;
}

/**
 * \brief Asynchronous read a single char
 *
 * Sets up the driver to read data from the USART module to the data
 * pointer given. If registered and enabled, a callback will be called
 * when the receiving is completed.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[out]    rx_data  Pointer to where received data should
 *                         be put
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_BUSY        If operation was not completed,
 *                                    due to the USART module being busy.
 */
enum status_code usart_async_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Check if the USART receiver is busy */
	if (dev_inst->remaining_rx_buffer_length > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Call internal read buffer function with length 1 */
	_usart_async_read_buffer(dev_inst, (uint8_t *)rx_data, 1);

	return STATUS_OK;
}

/**
 * \brief Asynchronous buffer write
 *
 * Sets up the driver to write a given buffer over the USART. If registered and
 * enabled, a callback function will be called.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[in]     tx_data  Pointer do data buffer to transmit
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed
 * \retval     STATUS_ERR_BUSY        If operation was not completed,
 *                                    due to the USART module being busy.
 */
enum status_code usart_async_write_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *tx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Check if the USART transmitter is busy */
	if (dev_inst->remaining_tx_buffer_length > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Issue internal asynchronous write */
	_usart_async_write_buffer(dev_inst, tx_data, length);

	return STATUS_OK;
}

/**
 * \brief Asynchronous buffer read
 *
 * Sets up the driver to read from the USART to a given buffer. If registered
 * and enabled, a callback function will be called.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[out]    rx_data  Pointer to data buffer to receive
 * \param[in]     length   Data buffer length
 *
 * \returns    Status of the operation
 * \retval     STATUS_OK              If operation was completed.
 * \retval     STATUS_ERR_BUSY        If operation was not completed,
 *                                    due to the USART module being busy.
 */
enum status_code usart_async_read_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *rx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Check if the USART receiver is busy */
	if (dev_inst->remaining_rx_buffer_length > 0) {
		return STATUS_ERR_BUSY;
	}

	/* Issue internal asynchronous read */
	_usart_async_read_buffer(dev_inst, rx_data, length);

	return STATUS_OK;
}

/**
 * \brief Cancels ongoing read/write operation
 *
 * Cancels the ongoing read/write operation modifying parameters in the
 * USART software struct.
 *
 * \param[in]     dev_inst          Pointer to USART software instance struct
 * \param[in]     transceiver_type  Transfer type to cancel
 */
void usart_async_cancel_transfer(struct usart_dev_inst *const dev_inst,
		enum usart_transceiver_type transceiver_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_module = &(dev_inst->hw_dev->USART);

	switch(transceiver_type) {
	case USART_TRANSCEIVER_RX:
		/* Clear the interrupt flag in order to prevent the receive
		 * complete callback to fire */
		usart_module->INTFLAG.reg |= USART_INTERRUPT_FLAG_RX_COMPLETE;

		/* Clear the software reception buffer */
		dev_inst->remaining_rx_buffer_length = 0;

	case USART_TRANSCEIVER_TX:
		/* Clear the interrupt flag in order to prevent the receive
		 * complete callback to fire */
		usart_module->INTFLAG.reg |= USART_INTERRUPT_FLAG_TX_COMPLETE;

		/* Clear the software reception buffer */
		dev_inst->remaining_tx_buffer_length = 0;
	}
}

/**
 * \brief Get status from the ongoing or last asynchronous transfer operation
 *
 * Returns the error from a given ongoing or last asynchronous transfer operation.
 * Either from a read or write transfer.
 *
 * \param[in] dev_inst    Pointer to USART software instance struct
 *
 * \return
 * \retval STATUS_OK                No error occurred during the last transfer
 * \retval STATUS_IN_PROGRESS       A transfer is ongoing
 * \retval STATUS_ERR_BAD_DATA      The last operation was aborted due to a
 *                                  parity error. The transfer could be affected
 *                                  by external noise.
 * \retval STATUS_ERR_BAD_FORMAT    The last operation was aborted due to a
 *                                  frame error. Please check the configuration.
 * \retval STATUS_ERR_OVERFLOW      The last operation was aborted due to an
 *                                  buffer overflow. Please check the configuration.
 */
enum status_code usart_async_get_operation_status(
		struct usart_dev_inst *const dev_inst,
		enum usart_transceiver_type transceiver_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Variable for status code */
	enum status_code status_code;

	switch(transceiver_type) {
	case USART_TRANSCEIVER_RX:
		/* Get status code */
		status_code = dev_inst->rx_status;

	case USART_TRANSCEIVER_TX:
		/* Get status code */
		status_code = dev_inst->tx_status;
	}

	return status_code;
}

/**
 * \brief Interrupt Handler for USART module
 *
 * Handles interrupts as they occur, and it will run callback functions
 * which are registered and enabled.
 *
 * \note This function will be called by the Sercom_Handler, and should
 * not be called directly from any application code.
 *
 * \param[in]  instance  ID of the SERCOM instance calling the interrupt
 *                       handler.
 */
void usart_async_handler(uint8_t instance)
{
	/* Sanity check content from the look-up table */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Temporary variables */
	uint16_t interrupt_status;
	uint16_t callback_status;
	uint8_t error_code;

	/* Get device instance from the look-up table */
	struct usart_dev_inst *dev_inst
		= (struct usart_dev_inst *)_sercom_instances[instance];

	/* Pointer to the hardware module instance */
	SercomUsart *const usart_module
		= &(dev_inst->hw_dev->USART);

	/* Wait for the synchronization to complete */
	_usart_wait_for_sync(dev_inst);

	/* Read and mask interrupt flag register */
	interrupt_status = usart_module->INTFLAG.reg;
	callback_status = dev_inst->callback_reg_mask
			&dev_inst->callback_enable_mask;

	/* Check if a DATA READY interrupt has occurred,
	 * and if there if there is more to transfer */
	if ((interrupt_status & USART_INTERRUPT_FLAG_DATA_BUFFER_EMPTY) &&
		dev_inst->remaining_tx_buffer_length) {
		/* Write current packet from transmission buffer
		 * and increment buffer pointer */
		if (dev_inst->char_size == USART_CHAR_SIZE_9BIT) {
			usart_module->DATA.reg |= *(dev_inst->tx_buffer_ptr)
					& SERCOM_USART_DATA_MASK;
			dev_inst->tx_buffer_ptr += 2;

		} else {
			usart_module->DATA.reg |= (*(dev_inst->tx_buffer_ptr)
					& SERCOM_USART_DATA_MASK);
			(dev_inst->tx_buffer_ptr)++;

		}

		/* Decrement buffer length */
		(dev_inst->remaining_tx_buffer_length)--;

		/* Check if it was the last transmission */
		if (dev_inst->remaining_tx_buffer_length == 0) {
			/* Disable the Data Register Empty Interrupt */
			usart_module->INTENCLR.reg
				= USART_INTERRUPT_FLAG_DATA_BUFFER_EMPTY;
		}

	/* Check if the Transmission Complete interrupt has occurred and
	 * that the transmit buffer is empty */
	} else if ((interrupt_status & USART_INTERRUPT_FLAG_TX_COMPLETE) &&
			!dev_inst->remaining_tx_buffer_length){

		/* Disable TX Complete Interrupt, and set STATUS_OK */
		usart_module->INTENCLR.reg = USART_INTERRUPT_FLAG_TX_COMPLETE;
		dev_inst->tx_status = STATUS_OK;

		/* Run callback if registered and enabled */
		if (callback_status & USART_CALLBACK_BUFFER_TRANSMITTED) {
			(*(dev_inst->callback[USART_CALLBACK_BUFFER_TRANSMITTED]))(dev_inst);
		}

	/* Check if the Receive Complete interrupt has occurred, and that
	 * there's more data to receive */
	} else if ((interrupt_status & USART_INTERRUPT_FLAG_RX_COMPLETE) &&
			dev_inst->remaining_rx_buffer_length) {
		/* Read out the status code and mask away all but the 4 LSBs*/
		error_code = (uint8_t)(usart_module->STATUS.reg & SERCOM_USART_STATUS_MASK);

		/* Check if an error has occurred during the receiving */
		if (error_code) {
			/* Check which error occurred */
			if (error_code & SERCOM_USART_STATUS_FERR) {
				/* Store the error code and clearing
				 * flag by writing 1 to it */
				dev_inst->rx_status = STATUS_ERR_BAD_FORMAT;
				usart_module->STATUS.reg &= ~SERCOM_USART_STATUS_FERR;

			} else if (error_code & SERCOM_USART_STATUS_BUFOVF) {
				/* Store the error code and clearing
				 * flag by writing 1 to it */
				dev_inst->rx_status = STATUS_ERR_OVERFLOW;
				usart_module->STATUS.reg &= ~SERCOM_USART_STATUS_BUFOVF;

			} else if (error_code & SERCOM_USART_STATUS_PERR) {
				/* Store the error code and clearing
				 * flag by writing 1 to it */
				dev_inst->rx_status = STATUS_ERR_BAD_DATA;
				usart_module->STATUS.reg &= ~SERCOM_USART_STATUS_PERR;
			}

			/* Run callback if registered and enabled */
			if (callback_status
					& USART_CALLBACK_ERROR) {
				(*(dev_inst->callback[USART_CALLBACK_ERROR]))(dev_inst);
			}

		} else {

			/* Read current packet from DATA register,
			 * increment buffer pointer and decrement buffer length */
			if(dev_inst->char_size == USART_CHAR_SIZE_9BIT) {
				/* Read out from DATA and increment 8bit ptr by two */
				*(dev_inst->rx_buffer_ptr) = (usart_module->DATA.reg & SERCOM_USART_DATA_MASK);
				dev_inst->tx_buffer_ptr += 2;
			} else {
				/* Read out from DATA and increment 8bit ptr by one */
				*(dev_inst->rx_buffer_ptr) = (usart_module->DATA.reg & SERCOM_USART_DATA_MASK);
				dev_inst->tx_buffer_ptr++;
			}

			/* Decrement length of the remaining buffer */
			(dev_inst->remaining_rx_buffer_length)--;

			/* Check if the last character have been received */
			if(dev_inst->remaining_rx_buffer_length == 0) {
				/* Disable RX Complete Interrupt,
				 * and set STATUS_OK */
				usart_module->INTENCLR.reg = USART_INTERRUPT_FLAG_RX_COMPLETE;
				dev_inst->rx_status = STATUS_OK;

				/* Run callback if registered and enabled */
				if (callback_status
						& USART_CALLBACK_BUFFER_RECEIVED) {
					(*(dev_inst->callback[USART_CALLBACK_BUFFER_RECEIVED]))(dev_inst);
				}
			}
		}
	}
}
