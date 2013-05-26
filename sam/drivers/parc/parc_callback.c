/**
 * \file
 *
 * \brief PARC driver for SAM.
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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

#include "parc.h"
#include "parc_callback.h"
#include "sysclk.h"
#include "conf_parc.h"

struct parc_module *parc_module_instance;
void _parc_interrupt_handler(void);

void PARC_Handler(void)
{
	_parc_interrupt_handler();
}


void _parc_interrupt_handler()
{
	struct parc_module *module = parc_module_instance;

	/* get interrupt flags and mask out enabled callbacks */
	uint32_t flags = module->hw->PARC_SR;

	if (flags & PARC_INTERRUPT_DRDY) {
				if(module->enabled_callback_mask & (1 << PARC_CALLBACK_DRDY)) {
					(*(module->callback[PARC_CALLBACK_DRDY]))(module);
				}
			}
		

	if (flags & PARC_INTERRUPT_DRDY) {
		//module->hw->INTFLAG.reg = ADC_INTFLAG_OVERRUN;
		if(module->enabled_callback_mask & (1 << PARC_CALLBACK_OVERRUN)) {
			(*(module->callback[PARC_CALLBACK_OVERRUN]))(module);
		}
	}
}





