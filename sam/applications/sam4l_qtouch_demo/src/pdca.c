/*****************************************************************************
 *
 * \file
 *
 * \brief PDCA driver for SAM4L.
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


#include "compiler.h"
#include "pdca.h"


volatile PdcaChannel *pdca_get_handler(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = &(PDCA->PDCA_CHANNEL[pdca_ch_number]);

	if (pdca_ch_number >= PDCA_CHANNEL_LENGTH) {
		return (volatile PdcaChannel *)PDCA_INVALID_ARGUMENT;
	}

	return pdca_channel;
}


int pdca_init_channel(unsigned int pdca_ch_number, const pdca_channel_options_t *opt)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	pdca_disable_interrupt_transfer_complete(pdca_ch_number); // disable channel interrupt
	pdca_disable_interrupt_reload_counter_zero(pdca_ch_number); // disable channel interrupt

	bool global_interrupt_enabled = cpu_irq_is_enabled();

	if (global_interrupt_enabled) cpu_irq_disable();
	pdca_channel->PDCA_MAR = (unsigned long)opt->addr;
	pdca_channel->PDCA_TCR = opt->size;
	pdca_channel->PDCA_PSR = opt->pid;
	pdca_channel->PDCA_MARR = (unsigned long)opt->r_addr;
	pdca_channel->PDCA_TCRR = opt->r_size;
	pdca_channel->PDCA_MR = opt->etrig | opt->transfer_size;
	pdca_channel->PDCA_CR = PDCA_CR_ECLR;
	pdca_channel->PDCA_ISR;
	if (global_interrupt_enabled) cpu_irq_enable();

	return PDCA_SUCCESS;
}


unsigned int pdca_get_channel_status(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	return (pdca_channel->PDCA_SR & PDCA_SR_TEN) != 0;
}


void pdca_disable(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	// Disable transfer
	pdca_channel->PDCA_CR = PDCA_CR_TDIS;

}


void pdca_enable(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	// Enable transfer
	pdca_channel->PDCA_CR = PDCA_CR_TEN;
}


unsigned int pdca_get_load_size(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	return pdca_channel->PDCA_TCR;
}


void pdca_load_channel(unsigned int pdca_ch_number, volatile void *addr, unsigned int size)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	bool global_interrupt_enabled = cpu_irq_is_enabled();

	if (global_interrupt_enabled) cpu_irq_disable();
	pdca_channel->PDCA_MAR = (unsigned long)addr;
	pdca_channel->PDCA_TCR = size;
	pdca_channel->PDCA_CR = PDCA_CR_ECLR;
	pdca_channel->PDCA_ISR;
	if (global_interrupt_enabled) cpu_irq_enable();
}


unsigned int pdca_get_reload_size(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	return pdca_channel->PDCA_TCRR;
}


void pdca_reload_channel(unsigned int pdca_ch_number, volatile void *addr, unsigned int size)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	bool global_interrupt_enabled = cpu_irq_is_enabled();

	if (global_interrupt_enabled) cpu_irq_disable();
	// set up next memory address
	pdca_channel->PDCA_MARR = (unsigned long)addr;
	// set up next memory size
	pdca_channel->PDCA_TCRR = size;
	pdca_channel->PDCA_CR = PDCA_CR_ECLR;
	pdca_channel->PDCA_ISR;
	if (global_interrupt_enabled) cpu_irq_enable();
}


void pdca_set_peripheral_select(unsigned int pdca_ch_number, unsigned int pid)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	pdca_channel->PDCA_PSR = pid;
}


void pdca_set_transfer_size(unsigned int pdca_ch_number, unsigned int transfer_size)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	pdca_channel->PDCA_MR = (pdca_channel->PDCA_MR & ~PDCA_MR_SIZE_Msk) |
			PDCA_MR_SIZE(transfer_size);
}


void pdca_disable_event_trigger(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	pdca_channel->PDCA_MR &= ~PDCA_MR_ETRIG;
}


void pdca_enable_event_trigger(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	pdca_channel->PDCA_MR |= PDCA_MR_ETRIG;
}


void pdca_disable_interrupt_transfer_error(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	bool global_interrupt_enabled = cpu_irq_is_enabled();

	if (global_interrupt_enabled) cpu_irq_disable();
	pdca_channel->PDCA_IDR = PDCA_IDR_TERR;
	pdca_channel->PDCA_ISR;
	if (global_interrupt_enabled) cpu_irq_enable();
}


void pdca_enable_interrupt_transfer_error(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	pdca_channel->PDCA_IER = PDCA_IER_TERR;
}


void pdca_disable_interrupt_transfer_complete(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	bool global_interrupt_enabled = cpu_irq_is_enabled();

	if (global_interrupt_enabled) cpu_irq_disable();
	pdca_channel->PDCA_IDR = PDCA_IDR_TRC;
	pdca_channel->PDCA_ISR;
	if (global_interrupt_enabled) cpu_irq_enable();
}


void pdca_enable_interrupt_transfer_complete(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	pdca_channel->PDCA_IER = PDCA_IER_TRC;
}


void pdca_disable_interrupt_reload_counter_zero(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	bool global_interrupt_enabled = cpu_irq_is_enabled();

	if (global_interrupt_enabled) cpu_irq_disable();
	pdca_channel->PDCA_IDR = PDCA_IDR_RCZ;
	pdca_channel->PDCA_ISR;
	if (global_interrupt_enabled) cpu_irq_enable();
}


void pdca_enable_interrupt_reload_counter_zero(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	pdca_channel->PDCA_IER = PDCA_IER_RCZ;
}


unsigned long pdca_get_transfer_status(unsigned int pdca_ch_number)
{
	// get the correct channel pointer
	volatile PdcaChannel *pdca_channel = pdca_get_handler(pdca_ch_number);

	return pdca_channel->PDCA_ISR;
}
