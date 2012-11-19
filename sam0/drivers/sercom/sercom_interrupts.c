/**
 * \file
 *
 * \brief SAM0+ Serial Peripheral Interface Driver
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
#include <sercom_interrupts.h>


uint8_t _sercom_get_current_irq_index(void)
{
	/* Find in some core register */
	//TODO!
}

/* Interrupt Service Routine */
void SERCOM_Handler(void)
{
	uint8_t instance = _sercom_get_current_irq_index();
	uint16_t callback_status;

	/* Sercom mode is contained in the first byte of dev_inst */
	uint8_t mode = *(uint8_t *)_sercom_instances[instance];

	switch(mode) {
		case SERCOM_MODE_SPI:
		{
			/* TODO: Implementation for the SPI async driver */
			break;
		}
		case SERCOM_MODE_USART:
		{
			/* Get device instance from the look-up table */
			struct usart_dev_inst *dev_inst = (struct usart_dev_inst *)_sercom_instances[instance];

			/* Sanity check content from the look-up table */
			Assert(dev_inst);
			Assert(dev_inst->hw_dev);

			/* Pointer to the hardware module instance */
			SERCOM_USART_t *const usart_module =
					&(dev_inst->hw_dev->USART);

			/* Read and mask interrupt flag register */
			callback_status =  usart_module->INTFLAGS
					&dev_inst->callback_reg_mask
					&dev_inst->callback_enable_mask;

			/* Check if a DATA READY interrupt has occurred,
			 * and if the callback is enabled */
			if (callback_status & SERCOM_USART_DREIF_bm){
				//TODO: && dev_inst->remaining_tx_buffer) {
				/* Check if the transmission buffer has data
				 * to transfer */
				//TODO:remove if-else.
				if (dev_inst->remaining_tx_buffer_length) {
					/* Write current packet from transmission buffer,
					 * increment buffer pointer and decrement buffer length */

					if (dev_inst->char_size == USART_CHARACTER_SIZE_9BIT) {
						usart_module->DATA |= *(dev_inst->tx_buffer_ptr)
								& SERCOM_USART_DATA_gm;
						(dev_inst->tx_buffer_ptr)+2;
					} else {
						usart_module->DATA |= *(dev_inst->tx_buffer_ptr)
								& SERCOM_USART_DATA_gm;
						(dev_inst->tx_buffer_ptr)++;
					}

					(dev_inst->remaining_tx_buffer_length)--;

				} else {
					/* If the transmission buffer is empty,
					 * run callback*/
					//dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_EMPTY](dev_inst);
					//TODO: Not
				}

				/* Clear the interrupt flag */
				usart_module->INTFLAGS &= ~SERCOM_USART_DREIF_bm;
			}

			if ((callback_status & SERCOM_USART_TXCIF_bm) &&
					!dev_inst->remaining_tx_buffer_length){
				dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_TRANSMITTED](dev_inst);

				/* Clear the interrupt flag */
				usart_module->INTFLAGS &= ~SERCOM_USART_TXCIF_bm;
			}

			if ((callback_status & SERCOM_USART_RXCIF_bm) &&
					!dev_inst->remaining_rx_buffer_length){

				/* Check if the reception buffer has data
				 * to receive */
				if (dev_inst->remaining_rx_buffer_length) {
					/* Read current packet from DATA register,
					 * increment buffer pointer and decrement buffer length */
					if(dev_inst->char_size == USART_CHARACTER_SIZE_9BIT) {
						/* Read out from DATA and increment 8bit ptr by two */
						*(dev_inst->rx_buffer_ptr) =
								(usart_module->DATA & SERCOM_USART_DATA_gm);
						dev_inst->tx_buffer_ptr+2;
					} else {
						/* Read out from DATA and increment 8bit ptr by one */
						*(dev_inst->rx_buffer_ptr) =
								(usart_module->DATA & SERCOM_USART_DATA_gm);
						dev_inst->tx_buffer_ptr++;
					}

					/* Decrement length of the remaining buffer */
					(dev_inst->remaining_rx_buffer_length)--;

				} else {
					/* If the transmission buffer is empty,
					 * run callback*/
					dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_RECEIVED](dev_inst);
				}

				/* Clear the interrupt flag */
				usart_module->INTFLAGS &= ~SERCOM_USART_RXCIF_bm;
			}
		break;

		}
		case SERCOM_MODE_I2C:
		{
			/* TODO: Implementation for the I2C async driver */
			break;
		}
		default:
		{
			Assert(false);
		}
	}
}
