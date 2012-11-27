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
#include <asf.h>
#include <sercom_interrupts.h>

/** Save status of initialized handlers. */
static bool _handler_table_initialized = false;

/** Void pointers for saving device instance structures. */
static void (*_sercom_interrupt_handlers[SERCOM_INSTS_NUM])(uint8_t instance);

uint8_t _sercom_get_current_irq_index(void)
{
	/* Find in some core register */
	//TODO!
	return 0;
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
uint8_t _sercom_get_instance_index(SERCOM_t *hw_dev)
{
	/* Variable for array index */
	uint8_t instance_index;

	/* Save device instance structure pointer. */
	uint32_t sercom_module = *(uint32_t *)hw_dev;

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

	return instance_index;
}

/**
 * \internal Default interrupt handler
 * \param instance Sercom instance used.
 */
void _sercom_default_handler(uint8_t instance)
{
	Assert(false);
}

/* Interrupt Service Routine */
void SERCOM_Handler(void)
{
	/* Something something. */
	uint8_t instance = _sercom_get_current_irq_index();

	/* Call appropriate interrupt handler. */
	_sercom_interrupt_handlers[instance] (instance);
}

/**
 */
void _sercom_set_handler(uint8_t instance,
		void (*interrupt_handler) (uint8_t instance))
{
	uint8_t i;
	/* Initialize handlers with default handler. */
	if(_handler_table_initialized == false) {
		for(i = 0; i < SERCOM_INSTS_NUM; i++) {
			_sercom_interrupt_handlers[i] = &_sercom_default_handler;
		}
		_handler_table_initialized = true;
	}

	/* Save interrupt handler. */
	_sercom_interrupt_handlers[instance] = interrupt_handler;
}

/** 
 * \internal 
 *
 **/
void _sercom_register_instance(uint8_t instance_index, void *const dev_inst)
{
	/* Register software device struct in look-up table. */
	_sercom_instances[instance_index] = dev_inst;	
}


