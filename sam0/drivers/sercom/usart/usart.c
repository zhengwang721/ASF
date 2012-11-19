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


enum status_code usart_init(struct usart_dev_inst *dev_inst, SERCOM_t *hw_dev, struct usart_config *config)
{
	
	
}



/**
 * \brief Transfer a buffer of \ref length length bytes via USART
 *
 * This function will transfer a block of \ref length length bytes via the USART
 *
 * \param inst pointer to the device struct
 * \param data pointer to the buffer to transmit
 * \param length number of bytes to transfer
 */
enum status_code usart_write_buffer(const struct usart_dev_inst *const dev_inst,
		const uint16_t *tx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	do {
		usart_write(dev_inst, *(tx_data++));
	} while (--length);

	return STATUS_OK;

}

/**
* \brief Read a block of data from the USART
*
* This function will read a block of \ref length bytes from the USART
* and block during the transfer.
*
* \param dev_inst pointer to the device struct
* \param rx_data pointer to the buffer where to store the received data
* \param length number of bytes to receive
*/
enum status_code usart_read_buffer(const struct usart_dev_inst *const dev_inst,
		const uint16_t *rx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	do {
		usart_read(dev_inst, rx_data++);
	} while (--length);
	
	return STATUS_OK;
}



