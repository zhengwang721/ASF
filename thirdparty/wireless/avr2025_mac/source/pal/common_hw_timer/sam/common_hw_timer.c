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

//todo remove later
#include <asf.h>
#include "conf_example.h"
#include "conf_common_hw_timer.h"
#include "ioport.h"
#include "tc.h"

static uint8_t timer_multiplier;
static uint16_t compare_value;
static volatile uint8_t timer_mul_var;
static volatile uint32_t overflow_counter;

static tc_callback_t common_tc_ovf_callback;
static tc_callback_t common_tc_cca_callback;

#define ENABLE_CC_INTERRUPT()  { tc_get_status(TIMER, TIMER_CHANNEL_ID);\
                                    tc_enable_interrupt(TIMER,TIMER_CHANNEL_ID, TC_IER_CPCS);}
	                            

#define DISABLE_CC_INTERRUPT()  tc_get_status(TIMER, TIMER_CHANNEL_ID);\
                                    tc_disable_interrupt(TIMER,TIMER_CHANNEL_ID, TC_IDR_CPCS);

#define ENABLE_OVF_INTERRUPT()  tc_get_status(TIMER, TIMER_CHANNEL_ID);\
                                    tc_enable_interrupt(TIMER,TIMER_CHANNEL_ID, TC_IER_COVFS);

#define DISABLE_OVF_INTERRUPT() tc_get_status(TIMER, TIMER_CHANNEL_ID);\
                                     tc_disable_interrupt(TIMER,TIMER_CHANNEL_ID, TC_IDR_COVFS);
                                    
/* === Prototypes =========================================================== */
/**
 * \brief Interrupt handler for TC00. Record the number of bytes received,
 * and then restart a read transfer on the USART if the transfer was stopped.
 */
void TC00_Handler(void)
{
	uint32_t ul_status;
 
	/* Read TC0 Status. */
	ul_status = tc_get_status(TIMER,TIMER_CHANNEL_ID);
        ul_status &= tc_get_interrupt_mask(TIMER,TIMER_CHANNEL_ID);
        if (TC_SR_CPCS == (ul_status & TC_SR_CPCS))
	{
            DISABLE_CC_INTERRUPT();
            
            if (common_tc_cca_callback) {
                    common_tc_cca_callback();
            }        
        }
	/* Overflow */
        if (TC_SR_COVFS == (ul_status & TC_SR_COVFS))
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
}

//uint16_t common_tc_read_count(void)
//{
//    uint16_t countvalue;
//    uint16_t cv;
//
//	cv = 0;
//	countvalue = tc_read_tc(TIMER, TIMER_CHANNEL_ID);
//
//	if(timer_mul_var)
//	{
//		cv = ((UINT16_MAX / timer_multiplier) * timer_mul_var) +
//				(countvalue / timer_multiplier);
//	}
//	else
//	{
//		cv = countvalue / timer_multiplier;
//	}
//
//	return cv;
//}
//
//
//void common_tc_compare_stop(void)
//{
//	DISABLE_CC_INTERRUPT();
//
//	uint8_t flags = cpu_irq_save();
//	overflow_counter = 0;
//	compare_value = 0;
//	cpu_irq_restore(flags);
//}
//
//
//void common_tc_overflow_stop(void)
//{
//	DISABLE_OVF_INTERRUPT();
//	timer_mul_var = 0;
//}
//
//
//void common_tc_stop(void)
//{
//	common_tc_compare_stop();
//
//	common_tc_overflow_stop();
//
//	tc_stop(TIMER, TIMER_CHANNEL_ID);
//}
//
//
void common_tc_delay(uint16_t value)
{
	uint32_t temp;
	uint16_t countvalue = tc_read_cv(TIMER, TIMER_CHANNEL_ID);

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
	/* Configure clock service. */
	sysclk_enable_peripheral_clock(TIMER);

	/* Get system clock. */
	timer_multiplier = sysclk_get_peripheral_bus_hz(TIMER)/1000000;

	tc_init(TC0, 0, 1 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_NO_AUTO);	
	timer_multiplier = timer_multiplier >> 1;

	timer_mul_var = 0;

	/* Configure and enable interrupt on RC compare. */
	NVIC_EnableIRQ(TC00_IRQn);
	tc_enable_interrupt(TIMER, 0, TC_IER_COVFS);
        DISABLE_CC_INTERRUPT();
        tc_start(TIMER,TIMER_CHANNEL_ID);
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
