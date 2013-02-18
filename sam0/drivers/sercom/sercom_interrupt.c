/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
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
#include "sercom_interrupt.h"

void *_sercom_instances[SERCOM_INST_NUM];

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
		sercom_handler_t interrupt_handler)
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
 * \internal Returns the system interrupt vector.
 *
 * \param[in] sercom_instance Instance pointer
 *
 * \return Enum of system interrupt vector
 * \retval SYSTEM_INTERRUPT_MODULE_SERCOM0
 * \retval SYSTEM_INTERRUPT_MODULE_SERCOM1
 * \retval SYSTEM_INTERRUPT_MODULE_SERCOM2
 * \retval SYSTEM_INTERRUPT_MODULE_SERCOM3
 * \retval SYSTEM_INTERRUPT_MODULE_SERCOM4
 * \retval SYSTEM_INTERRUPT_MODULE_SERCOM5
 */
enum system_interrupt_vector _sercom_get_interrupt_vector(Sercom *sercom_instance)
{
	uint8_t instance_index = _sercom_get_sercom_inst_index(sercom_instance);

	switch (instance_index) {
#ifdef ID_SERCOM0
	case 0:
		return SYSTEM_INTERRUPT_MODULE_SERCOM0;
#endif
#ifdef ID_SERCOM1
	case 1:
		return SYSTEM_INTERRUPT_MODULE_SERCOM1;
#endif
#ifdef ID_SERCOM2
	case 2:
		return SYSTEM_INTERRUPT_MODULE_SERCOM2;
#endif
#ifdef ID_SERCOM3
	case 3:
		return SYSTEM_INTERRUPT_MODULE_SERCOM3;
#endif
#ifdef ID_SERCOM4
	case 4:
		return SYSTEM_INTERRUPT_MODULE_SERCOM4;
#endif
#ifdef ID_SERCOM5
	case 5:
		return SYSTEM_INTERRUPT_MODULE_SERCOM5;
#endif
	default:
		/* Invalid data given. */
		Assert(false);
		return 0;
	}
}

/**
 * \internal ISR handler for SERCOM
 */
#ifdef ID_SERCOM0
void SERCOM0_IRQn_Handler(void);
void SERCOM0_IRQn_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_sercom_interrupt_handlers[0] (0);
}
#endif
#ifdef ID_SERCOM1
void SERCOM1_IRQn_Handler(void);
void SERCOM1_IRQn_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_sercom_interrupt_handlers[1] (1);
}
#endif
#ifdef ID_SERCOM2
void SERCOM2_IRQn_Handler(void);
void SERCOM2_IRQn_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_sercom_interrupt_handlers[2] (2);
}
#endif
#ifdef ID_SERCOM3
void SERCOM3_IRQn_Handler(void);
void SERCOM3_IRQn_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_sercom_interrupt_handlers[3] (3);
}
#endif
#ifdef ID_SERCOM4
void SERCOM4_IRQn_Handler(void);
void SERCOM4_IRQn_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_sercom_interrupt_handlers[4] (4);
}
#endif
#ifdef ID_SERCOM5
void SERCOM5_IRQn_Handler(void);
void SERCOM5_IRQn_Handler(void)
{
	/* Call appropriate interrupt handler. */
	_sercom_interrupt_handlers[5] (5);
}
#endif
