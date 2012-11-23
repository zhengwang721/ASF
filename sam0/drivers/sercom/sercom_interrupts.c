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

#ifdef USART_ASYNC
#include <usart.h>
#else
struct usart_dev_inst;
#endif
#ifdef SPI_ASYNC
#include <spi.h>
#else
struct spi_dev_inst{};
#endif

static uint8_t handler_table_initialized = 0;

static void *_sercom_instances[SERCOM_INST];

static void (*_sercom_handlers[SERCOM_INST])(uint8_t instance);

/* */
union _sercom_dev_inst {
	/** Mode of the SERCOM hardware instance */
	uint8_t mode;
	/** Pointer to the SERCOM hardware instance*/
	SERCOM_t *hw_dev;
	/** Device struct for the USART mode */
	struct usart_dev_inst usart;
	/** Device struct for the SPI mode */
	struct spi_dev_inst spi;
};

uint8_t _sercom_get_current_irq_index(void)
{
	/* Find in some core register */
	//TODO!
}

/**
 * \internal 
 *
 *
 * param[]
 *
 * returns 
 * retval  
 */
//TODO: move to sercom

enum status_code _sercom_register_dev_inst_ptr(union _sercom_dev_inst *const dev_inst)
{
	/* Variable for array index */
	uint8_t instance_index;

	/* Save device instance structure pointer. */
	uint32_t sercom_module = (uint32_t)dev_inst->hw_dev;

	/**/

	switch(sercom_module){
		//TODO: fix casting

		case (uint32_t)&SERCOM0:
			instance_index = 0;
			break;

		case (uint32_t)&SERCOM1:
			instance_index = 1;
			break;

		case (uint32_t)&SERCOM2:
			instance_index = 2;
			break;

		case (uint32_t)&SERCOM3:
			instance_index = 3;
			break;

		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;
	}

	/* Save device instance pointer */
	_sercom_instances[instance_index] = dev_inst;

	return STATUS_OK;
}




/* Interrupt Service Routine */
void SERCOM_Handler(void)
{
	uint8_t instance = _sercom_get_current_irq_index();
	uint16_t interrupt_status;
	uint16_t callback_status;

	/* Sercom mode is contained in the first byte of *Æ*dev_inst */
	uint8_t mode = *(uint8_t *)_sercom_instances[instance];

	switch(mode) {
		case SERCOM_MODE_SPI:
		{
			/* TODO: Implementation for the SPI async driver */
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
