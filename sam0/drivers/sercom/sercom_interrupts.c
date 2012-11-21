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
}

enum status_code _sercom_register_dev_inst_ptr(SERCOM_DEV_INST_t *const dev_inst)
{
	/* Check arugment sanity. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Save device instance structure pointer. */
	uint32_t sercom_module = (uint32_t)dev_inst->hw_dev;

	/* Variable for array index. */
	uint8_t instance_index;

	switch(sercom_module) {
		case ((uint32_t)&SERCOM0):
			instance_index = 0;
			break;

		case ((uint32_t)&SERCOM1):
			instance_index = 1;
			break;

		case ((uint32_t)&SERCOM2):
			instance_index = 2;
			break;

		case ((uint32_t)&SERCOM3):
			instance_index = 3;
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
	}

	/* Save device instance pointer. */
	_sercom_instances[instance_index] = dev_inst;

	return STATUS_OK;
}


/* Interrupt Service Routine */
void SERCOM_Handler(void)
{
	uint8_t instance = _sercom_get_current_irq_index();
	uint16_t callback_status;

	/* Sercom mode is contained in the first byte of dev_inst */
	SERCOM_DEV_INST_t sercom_dev_inst = *_sercom_instances[instance];

	switch(sercom_dev_inst.mode) {
		case SERCOM_MODE_SPI:
		{
			/* Device instance */
			//struct spi_async_dev_inst dev_inst =
			//(struct spi_async_dev_inst *)mode;
			/* TODO: Implementation for the SPI async driver */
			break;
		}
		case SERCOM_MODE_USART:
		{
			struct usart_dev_inst *dev_inst = sercom_dev_inst.usart;

			/* Read and mask interrupt flag register */
			callback_status =  dev_inst->hw_dev->USART.INTFLAGS
					&dev_inst->callback_reg_mask
					&dev_inst->callback_enable_mask;

			if (callback_status & SERCOM_USART_RXCIF_bm){
				dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_TRANSMITTED](dev_inst);
			}

			if (callback_status & SERCOM_USART_TXCIF_bm){
				dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_RECEIVED](dev_inst);
			}

			if (callback_status & SERCOM_USART_DREIF_bm){
				dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_EMPTY](dev_inst);
			}

//			if (dev_inst->hw_dev.STATUS &
//					SERCOM_USART)
			break;
		}
		case SERCOM_MODE_I2C:
		{
			//struct usart_async_dev_inst dev_inst =
			//(struct twi_async_dev_inst *)mode;
			/* TODO: Implementation for the TWI async driver */
			break;
		}
		default:
		{
			Assert(false);
		}
	}
/*
	if (_sercom_instances[index] != NULL){
		if (dev_inst->hw_dev.STATUS & USART_RECEIVE_COMPLETE_IF_bm)

	} else {
		Assert(false);
	}
	*/
}
