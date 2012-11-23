/**
 * \file
 *
 * \brief SAM0+ SERCOM USART Driver
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
#include <usart.h>


enum status_code _usart_set_config(struct usart_dev_inst *const dev_inst,
		const struct usart_conf const *config) {
	/* */
	uint8_t i;
	

#ifdef USART_ASYNC
	/* Initialize parameters */
	dev_inst->sercom_mode          = SERCOM_MODE_USART;
	for(i; i < USART_CALLBACK_N; i++) {
		dev_inst->callback[i]  = NULL;
	}
	dev_inst->tx_buffer_ptr        = NULL;
	dev_inst->rx_buffer_ptr        = NULL;
	dev_inst->callback_reg_mask    = 0x00;
	dev_inst->callback_enable_mask = 0x00;

	_sercom_set_handler();
#endif

}




/**
 * \brief Initializes the device
 *
 * Initializes the USART device based on the setting specified in the
 * configuration struct. This will leave the device in an enabled state
 * after initialization.
 *
 * \param dev_inst Pointer to USART device
 * \param config Pointer to configuration struct
 */
enum status_code usart_init(struct usart_dev_inst *dev_inst,
		SERCOM_t *hw_dev, struct usart_conf *config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(hw_dev);
	Assert(config);

	/* Assign module pointer to software instance struct */
	dev_inst->hw_dev = hw_dev;

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);



	//TODO:
	if(usart_module->CTRLA & SERCOM_USART_RESET_bm) {
		/* Reset is ongoing. Abort. */
	}

	_usart_wait_for_sync(dev_inst);
	if(usart_module->CTRLA & SERCOM_USART_ENABLE_bm) {
		/* Module have to be disabled before initialization. Abort. */
		return STATUS_ERR_DENIED;
	}

	/* Set up GCLK_SERCOM using sysclk driver */
	//TODO

	_usart_set_config(dev_inst, config);
}

/**
 * \brief Transmit a character via the USART
 *
 * This non-blocking function will receive a character via the
 * USART.
 *
 * \note Using this function in combination with the asynchronous functions is
 *       not recommended as it has no functionality to check if there is an
 *       ongoing asynchronous operation running or not.
 *
 * param[in]   dev_inst Pointer to the software instance struct 
 * param[out]  tx_data  Data to transfer 
 *
 * \return     Status of the operation
 * \retval     STATUS_OK           If the operation was completed
 * \retval     STATUS_ERR_BUSY     If the operation was not completed,
 *                                 due to the USART module being busy.
 */
enum status_code usart_write(struct usart_dev_inst *const dev_inst,
		const uint16_t tx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Check if USART is ready for new data */
	if(!usart_is_data_buffer_empty(dev_inst)) {
		/* Return error code */
		return STATUS_ERR_BUSY;
	}

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Write data to USART module */
	usart_module->DATA = tx_data;

	return STATUS_OK;
}

/**
 * \brief Receive a character via the USART
 *
 * This non-blocking function will receive a character via the
 * USART.
 *
 * \note Using this function in combination with the asynchronous functions is
 *       not recommended as it has no functionality to check if there is an
 *       ongoing asynchronous operation running or not.
 *
 * param[in]   dev_inst Pointer to the software instance struct 
 * param[out]  rx_data  Pointer to received data
 *
 * \return     Status of the operation
 * \retval     STATUS_OK           If the operation was completed
 * \retval     STATUS_ERR_BUSY     If the operation was not completed,
 *                                 due to the USART module being busy.
 */
enum status_code usart_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Check if USART has new data */
	if(!usart_is_data_received(dev_inst)) {
		/* Return error code */
		return STATUS_ERR_BUSY;
	}

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Read data from USART module */
	*rx_data = usart_module->DATA;
}

/**
 * \brief Transmit a buffer of \ref length length characters via USART
 *
 * This blocking function will transmit a block of \ref length length characters
 * via the USART
 *
 * \note Using this function in combination with the asynchronous functions is
 *       not recommended as it has no functionality to check if there is an
 *       ongoing asynchronous operation running or not.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[out]    tx_data  Pointer to data to transmit
 * \param[length] number   Number of characters to transmit
 *
 * \return        Status of the operation
 * \retval        STATUS_OK                If operation was completed
 * \retval        STATUS_ERR_INVALID_ARG   If operation was not completed,
 *                                         due to invalid arguments
 * \retval        STATUS_ERR_TIMEOUT       If operation was not completed,
 *                                         due to USART module timing out
 */
enum status_code usart_write_buffer(struct usart_dev_inst *const dev_inst,
		const uint8_t *tx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Timeout variables */
	uint16_t i = 0;
	uint16_t timeout;
#ifdef USART_CUSTOM_TIMEOUT
	timeout = USART_CUSTOM_TIMEOUT;
#else
	timeout = USART_DEFAULT_TIMEOUT;
#endif

	/* Check if the buffer length is valid */
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Blocks while buffer is being transferred */
	while(length--){

		/* Wait for the USART to be ready for new data and abort
		 * operation if it doesn't get ready within the timeout*/
		for(i=0;i < timeout; i++) {
			if(usart_is_data_received(dev_inst)) {
				break;
			} else if(i == timeout) {
				return STATUS_ERR_TIMEOUT;
			}
		}

		/* Check if the character size exceeds 8 bit */
		if (dev_inst->char_size == USART_CHAR_SIZE_9BIT) {
			/* Increment 8 bit pointer by two */
			usart_write(dev_inst, (uint16_t)*(tx_data+2));
		} else {
			/* Increment 8 bit pointer by one */
			usart_write(dev_inst, (uint16_t)*(tx_data++));
		}
	}

	return STATUS_OK;
}

/**
 * \brief Receive a buffer of \ref length length characters via USART
 *
 * This blocking function will receive a block of \ref length length characters
 * via the USART.
 *
 * \note Using this function in combination with the asynchronous functions is
 *       not recommended as it has no functionality to check if there is an
 *       ongoing asynchronous operation running or not.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[out]    tx_data  Pointer to data to transmit
 * \param[length] number   Number of characters to transmit
 *
 * \return        Status of the operation
 * \retval        STATUS_OK                If operation was completed
 * \retval        STATUS_ERR_INVALID_ARG   If operation was not completed, due
 *                                         to invalid arguments
 * \retval        STATUS_ERR_TIMEOUT       If operation was not completed, due
 *                                         to USART module timing out
 */
enum status_code usart_read_buffer(struct usart_dev_inst *const dev_inst,
		const uint8_t *rx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Timeout variables */
	uint16_t i = 0;
	uint16_t timeout;
#ifdef USART_CUSTOM_TIMEOUT
	timeout = USART_CUSTOM_TIMEOUT;
#else
	timeout = USART_DEFAULT_TIMEOUT;
#endif
	/* Check if the buffer length is valid */
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Blocks while buffer is being received */
	while(length--) {

		/* Wait for the USART to have new data and abort operation if it
		 * doesn't get ready within the timeout*/
		for(i=0;i < timeout; i++) {
			if(usart_is_data_received(dev_inst)) {
				break;
			} else if(i == timeout) {
				return STATUS_ERR_TIMEOUT;
			}
		}

		/* Check if the character size exceeds 8 bit */
		if (dev_inst->char_size == USART_CHAR_SIZE_9BIT) {
			/* Increment the 8 bit data pointer by two */
			usart_read(dev_inst, (uint16_t *)(rx_data+2));
		} else {
			/* Increment the 8 bit data pointer by one */
			usart_read(dev_inst, (uint16_t *)(rx_data++));
		}
	}

	return STATUS_OK;
}
