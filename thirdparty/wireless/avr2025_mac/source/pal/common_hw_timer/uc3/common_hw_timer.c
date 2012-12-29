/**
 * @file common_hw_timer.c
 *
 * @brief 
 *
 *  Copyright (c) 2009 Atmel Corporation. All rights reserved.
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
 */
 #include <compiler.h>
#include "sysclk.h"
#include "common_hw_timer.h"
#include "status_codes.h"

static uint8_t timer_multiplier;
static uint16_t compare_value;
static volatile uint8_t timer_mul_var;
static volatile uint32_t overflow_counter;

static tc_callback_t common_tc_ovf_callback;
static tc_callback_t common_tc_cca_callback;

// Options for enabling TC interrupts
static tc_interrupt_t tc_interrupt = {
	.etrgs = 0,
	.ldrbs = 0,
	.ldras = 0,
	.cpcs  = 0, // Enable interrupt on RC compare alone
	.cpbs  = 0,
	.cpas  = 0,
	.lovrs = 0,
	.covfs = 0
};

#define ENABLE_CC_INTERRUPT()   tc_read_sr(TIMER, TIMER_CHANNEL_ID);\
								tc_interrupt.cpcs = 1;\
	                            tc_configure_interrupts(TIMER, TIMER_CHANNEL_ID, &tc_interrupt)

#define DISABLE_CC_INTERRUPT()  tc_interrupt.cpcs = 0;\
	                            tc_configure_interrupts(TIMER, TIMER_CHANNEL_ID, &tc_interrupt)

#define ENABLE_OVF_INTERRUPT()  tc_read_sr(TIMER, TIMER_CHANNEL_ID);\
								tc_interrupt.covfs = 1;\
	                            tc_configure_interrupts(TIMER, TIMER_CHANNEL_ID, &tc_interrupt)

#define DISABLE_OVF_INTERRUPT() tc_interrupt.covfs = 0;\
	                            tc_configure_interrupts(TIMER, TIMER_CHANNEL_ID, &tc_interrupt)
/* === Prototypes =========================================================== */

uint16_t countvalue;
uint16_t cv;

#if defined (__GNUC__)
__attribute__((__interrupt__))
#elif defined (__ICCAVR32__)
#pragma handler = AVR32_TC0_IRQ_GROUP, 1
__interrupt
#endif
static void tc_irq(void)
{
	uint32_t status = tc_read_sr(TIMER, TIMER_CHANNEL_ID);
	status &= tc_get_interrupt_settings(TIMER, TIMER_CHANNEL_ID);
	
	if ((status & AVR32_TC_IDR2_COVFS_MASK) == AVR32_TC_IDR2_COVFS_MASK)
	{
		if((overflow_counter > 0) && ((--overflow_counter) == 0))
		{
			ENABLE_CC_INTERRUPT();
		}
		if((++timer_mul_var) >= timer_multiplier)
		{
			timer_mul_var = 0;
			
			if (common_tc_ovf_callback) {
				common_tc_ovf_callback();
			}
		}
	}
	if ((status & AVR32_TC_IDR2_CPCS_MASK) == AVR32_TC_IDR2_CPCS_MASK)
	{
		DISABLE_CC_INTERRUPT();
		
		if (common_tc_cca_callback) {
			common_tc_cca_callback();
		}
	}
}

uint16_t common_tc_read_count(void)
{
	cv = 0;
	countvalue = tc_read_tc(TIMER, TIMER_CHANNEL_ID);

	if(timer_mul_var)
	{
		cv = ((UINT16_MAX / timer_multiplier) * timer_mul_var) +
				(countvalue / timer_multiplier);
	}
	else
	{
		cv = countvalue / timer_multiplier;
	}

	return cv;
}


void common_tc_compare_stop(void)
{
	DISABLE_CC_INTERRUPT();

	uint8_t flags = cpu_irq_save();
	overflow_counter = 0;
	compare_value = 0;
	cpu_irq_restore(flags);
}


void common_tc_overflow_stop(void)
{
	DISABLE_OVF_INTERRUPT();
	timer_mul_var = 0;
}


void common_tc_stop(void)
{
	common_tc_compare_stop();

	common_tc_overflow_stop();

	tc_stop(TIMER, TIMER_CHANNEL_ID);
}


void common_tc_delay(uint16_t value)
{
	uint32_t temp;
	uint16_t countvalue = tc_read_tc(TIMER, TIMER_CHANNEL_ID);

	temp = ((uint32_t)value * timer_multiplier);

	overflow_counter = (temp + countvalue);

	overflow_counter = overflow_counter >> 16;

	if(overflow_counter)
	{
		compare_value = (uint16_t)((temp - (UINT16_MAX - countvalue)) -
							((overflow_counter - 1) << 16));

		DISABLE_CC_INTERRUPT();
	}
	else
	{
		compare_value = temp + countvalue;

		ENABLE_CC_INTERRUPT();
	}
	if(compare_value < 100)
	{
		compare_value += 100;
	}
	tc_write_rc(TIMER, TIMER_CHANNEL_ID, compare_value);
}


void common_tc_init(void)
{
	// Options for waveform generation.
	static const tc_waveform_opt_t waveform_opt =
	{
		.channel  = TIMER_CHANNEL_ID,        // Channel selection.
		
		.bswtrg   = TC_EVT_EFFECT_NOOP,           // Software trigger effect on TIOB.
		.beevt    = TC_EVT_EFFECT_NOOP,           // External event effect on TIOB.
		.bcpc     = TC_EVT_EFFECT_NOOP,           // RC compare effect on TIOB.
		.bcpb     = TC_EVT_EFFECT_NOOP,           // RB compare effect on TIOB.
		
		.aswtrg   = TC_EVT_EFFECT_NOOP,           // Software trigger effect on TIOA.
		.aeevt    = TC_EVT_EFFECT_NOOP,           // External event effect on TIOA.
		.acpc     = TC_EVT_EFFECT_NOOP,           // RC compare effect on TIOA
		.acpa     = TC_EVT_EFFECT_NOOP,           // RA compare effect on TIOA
		
		.wavsel   = TC_WAVEFORM_SEL_UP_MODE,      // Waveform selection: Up mode without automatic trigger on RC compare.
		.enetrg   = false,                        // External event trigger enable.
		.eevt     = TC_EXT_EVENT_SEL_TIOB_INPUT,  // External event selection.
		.eevtedg  = TC_SEL_NO_EDGE,               // External event edge selection.
		.cpcdis   = false,                        // Counter disable when RC compare.
		.cpcstop  = false,                        // Counter clock stopped with RC compare.
		
		.burst    = TC_BURST_NOT_GATED,           // Burst signal selection.
		.clki     = TC_CLOCK_RISING_EDGE,         // Clock inversion.
		.tcclks   = TC_CLOCK_SOURCE_TC2           // Internal source clock 3, connected to fPBA / 8.
	};

	sysclk_enable_peripheral_clock(TIMER);
	// Initialize the timer/counter.
	tc_init_waveform(TIMER, &waveform_opt);

	timer_multiplier = sysclk_get_peripheral_bus_hz(TIMER) / DEF_1MHZ;
	
	timer_multiplier = timer_multiplier >> 1;

	timer_mul_var = 0;

	irq_register_handler(tc_irq, AVR32_TC0_IRQ0, 0);

	ENABLE_OVF_INTERRUPT();

	DISABLE_CC_INTERRUPT();
	tc_start(TIMER, TIMER_CHANNEL_ID);
}


void set_common_tc_overflow_callback(tc_callback_t callback)
{
	common_tc_ovf_callback = callback;
}


void set_common_tc_expiry_callback(tc_callback_t callback)
{
	common_tc_cca_callback = callback;
}


/* EOF */
