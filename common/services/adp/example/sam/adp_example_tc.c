/**
 * \file
 *
 * \brief ADP service example TC functions for SAM0
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <compiler.h>
#include <asf.h>
#include "adp_example_tc.h"
#include "conf_tc.h"

volatile bool time_out = true;

#define TC_CHANNEL_CAPTURE  2

//! [tc_capture_selection]
#define TC_CAPTURE_TIMER_SELECTION TC_CMR_TCCLKS_TIMER_CLOCK3
//! [tc_capture_selection]

static void adp_example_tc_configure(void)
{
	//struct tc_config config_tc;
	
	//tc_get_config_defaults(&config_tc);
	//config_tc.clock_source = GCLK_GENERATOR_1;
	//config_tc.counter_size = TC_COUNTER_SIZE_8BIT;
	//config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV16;
	/* Timer = 16*(2^8 - 55 - 1)/32000 = 100ms */
	//config_tc.counter_8_bit.value = 0;
	//config_tc.counter_8_bit.period = 55;
	
	//tc_init(&tc_instance, CONF_TC_MODULE, &config_tc);
	//tc_enable(&tc_instance);
		
	/* Init TC to capture mode. */
	tc_init(TC0, TC_CHANNEL_CAPTURE,
	TC_CAPTURE_TIMER_SELECTION /* Clock Selection */
	| TC_CMR_LDRA_RISING /* RA Loading: rising edge of TIOA */
	| TC_CMR_LDRB_FALLING /* RB Loading: falling edge of TIOA */
	| TC_CMR_ABETRG /* External Trigger: TIOA */
	| TC_CMR_ETRGEDG_FALLING /* External Trigger Edge: Falling edge */
	);
	
	tc_start(TC0, TC_CHANNEL_CAPTURE);
	
	//! [tc_capture_init_irq]
	/** Configure TC interrupts for TC TC_CHANNEL_CAPTURE only */
	NVIC_DisableIRQ(TC_IRQn);
	NVIC_ClearPendingIRQ(TC_IRQn);
	NVIC_SetPriority(TC_IRQn, 0);
	NVIC_EnableIRQ(TC_IRQn);
	//! [tc_capture_init_irq]
}

void adp_example_tc_init(void)
{
 	adp_example_tc_configure();
}

/**
 * \brief Interrupt handler for the TC TC_CHANNEL_CAPTURE
 */
//! [tc_capture_irq_handler_start]
void TC_Handler(void)
{
	time_out = true;
}
//! [tc_capture_irq_handler_end]
