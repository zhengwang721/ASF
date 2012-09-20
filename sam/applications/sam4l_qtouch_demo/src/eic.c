/**
 * \file
 *
 * \brief EIC driver
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#include "eic.h"
#include "sysclk.h"


void eic_init(volatile Eic* eic, const eic_options_t *opt, uint32_t nb_lines)
{
	uint32_t i;
	sysclk_enable_peripheral_clock(EIC);
	for (i = 0; i < nb_lines; i++) {
		// Set up mode level
		eic->EIC_MODE = (opt[i].eic_mode == EIC_MODE_LEVEL_TRIGGERED)
			? (eic->EIC_MODE | (1 << opt[i].eic_line))
			: (eic->EIC_MODE & ~(1 << opt[i].eic_line));
		// Set up edge type
		eic->EIC_EDGE = (opt[i].eic_edge == EIC_EDGE_RISING_EDGE)
			? (eic->EIC_EDGE | (1 << opt[i].eic_line))
			: (eic->EIC_EDGE & ~(1 << opt[i].eic_line));
		// Set up level
		eic->EIC_LEVEL = (opt[i].eic_level == EIC_LEVEL_HIGH_LEVEL)
			? (eic->EIC_LEVEL | (1 << opt[i].eic_line))
			: (eic->EIC_LEVEL & ~(1 << opt[i].eic_line));
		// Set up if filter is used
		eic->EIC_FILTER = (opt[i].eic_filter == EIC_FILTER_ENABLED)
			? (eic->EIC_FILTER | (1 << opt[i].eic_line))
			: (eic->EIC_FILTER & ~(1 << opt[i].eic_line));
		// Set up which mode is used : asynchronous mode/ synchronous mode
		eic->EIC_ASYNC = (opt[i].eic_async == EIC_ASYNCH_MODE)
			? (eic->EIC_ASYNC | (1 << opt[i].eic_line))
			: (eic->EIC_ASYNC & ~(1 << opt[i].eic_line));
	}
}

void eic_enable_lines(volatile Eic* eic, uint32_t mask_lines)
{
	eic->EIC_EN = mask_lines;
}

void eic_enable_line(volatile Eic* eic, uint32_t line_number)
{
	// Enable line line_number
	eic->EIC_EN = 1 << line_number;
}

void eic_disable_lines(volatile Eic* eic, uint32_t mask_lines)
{
	eic->EIC_DIS = mask_lines;
}

void eic_disable_line(volatile Eic* eic, uint32_t line_number)
{
	// Disable line line_number
	eic->EIC_DIS = 1 << line_number;
}

bool eic_is_line_enabled(volatile Eic* eic, uint32_t line_number)
{
	return (eic->EIC_CTRL & (1 << line_number)) != 0;
}

void eic_enable_interrupt_lines(volatile Eic* eic, uint32_t mask_lines)
{
	eic->EIC_IER = mask_lines;
}

void eic_enable_interrupt_line(volatile Eic* eic, uint32_t line_number)
{
	// Enable line line_number
	eic->EIC_IER = 1 << line_number;
}

void eic_disable_interrupt_lines(volatile Eic* eic, uint32_t mask_lines)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	eic->EIC_IDR = mask_lines;
	eic->EIC_IMR;
	cpu_irq_restore(flags);
}

void eic_disable_interrupt_line(volatile Eic* eic, uint32_t line_number)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	eic->EIC_IDR = 1 << line_number;
	eic->EIC_IMR;
	cpu_irq_restore(flags);
}

bool eic_is_interrupt_line_enabled(volatile Eic* eic, uint32_t line_number)
{
	return (eic->EIC_IMR & (1 << line_number)) != 0;
}

void eic_clear_interrupt_lines(volatile Eic* eic, uint32_t mask_lines)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	eic->EIC_ICR = mask_lines;
	eic->EIC_ISR;
	cpu_irq_restore(flags);
}

void eic_clear_interrupt_line(volatile Eic* eic, uint32_t line_number)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	eic->EIC_ICR = 1 << line_number;
	eic->EIC_ISR;
	cpu_irq_restore(flags);
}

bool eic_is_interrupt_line_pending(volatile Eic* eic, uint32_t line_number)
{
	return (eic->EIC_ISR & (1 << line_number)) != 0;
}
