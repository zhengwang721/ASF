/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
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
#include "sercom_interrupts.h"

/** Save status of initialized handlers. */
static bool _handler_table_initialized = false;

/** Void pointers for saving device instance structures. */
static void (*_sercom_interrupt_handlers[SERCOM_INST_NUM])(uint8_t instance);

/**
 * \internal Default interrupt handler
 *
 * \param[in] instance SERCOM instance used.
 */
static void _sercom_default_handler(uint8_t instance)
{
	Assert(false);
}

/**
 * \internal Find index of given instance.
 *
 * \param[in] Instance pointer.
 *
 * \return Index of given instance.
 */
uint8_t _sercom_get_sercom_inst_index(Sercom *sercom_instance)
{
	/* Variable used for iteration. */
	uint8_t i;
	/* Save address of sercom instance. */
	uint32_t hw_dev = (uint32_t)sercom_instance;
	/* Save all available SERCOM instances for compare. */
	Sercom *sercom_instances[SERCOM_INST_NUM] = SERCOM_INSTS;

	/* Find index for sercom instance. */
	for (i = 0; i < SERCOM_INST_NUM; i++) {
		if (hw_dev == (uint32_t)sercom_instances[i]) {
			return i;
		}
	}

	/* Invalid data given. */
	Assert(false);
	return 0;
}

/**
 * \internal Saves the given callback handler.
 *
 * \param[in] instance Instance index.
 * \param[in] interrupt_handler Pointer to instance callback handler.
 *
 */
void _sercom_set_handler(uint8_t instance,
		void (*interrupt_handler) (uint8_t instance))
{
	uint8_t i;
	/* Initialize handlers with default handler and device instances with 0.
	 */
	if(_handler_table_initialized == false) {
		for(i = 0; i < SERCOM_INST_NUM; i++) {
			_sercom_interrupt_handlers[i] = &_sercom_default_handler;
			_sercom_instances[i] = NULL;
		}
		_handler_table_initialized = true;
	}

	/* Save interrupt handler. */
	_sercom_interrupt_handlers[instance] = interrupt_handler;
}

/**
 * \internal ISR handler for SERCOM
 */
void SERCOM_Handler(void)
{
	/* Something something. */
	uint8_t instance = 1; // (uint8_t)system_interrupt_get_active()-7;

	/* Call appropriate interrupt handler. */
	_sercom_interrupt_handlers[instance] (instance);
}
