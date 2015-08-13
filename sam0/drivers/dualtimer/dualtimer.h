/**
 * \file
 *
 * \brief SAM DUALTIMER Driver for SAMB11
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
#ifndef DUALTIMER_H_INCLUDED
#define DUALTIMER_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*dualtimer_callback_t)(void);

/**
 * \brief Specifies the timer1 or timer2.
 *
 * This enum specifies duatimer timer1 or timer2.
 */
enum dualtimer_timer {
	/** Dualtimer timer1 */
	DUALTIMER_TIMER1 = 0,
	/** Dualtimer timer2 */
	DUALTIMER_TIMER2,
};

/**
 * \brief DUALTIMER module clock input.
 *
 * DUALTIMER module clock.
 */
enum dualtimer_clock_input {
	/** source from clock input 0 26MHz*/
	DUALTIMER_CLK_INPUT_0	= 0,
	/** source from clock input 1 13MHz */
	DUALTIMER_CLK_INPUT_1,
	/** source from clock input 2 6.5MHz*/
	DUALTIMER_CLK_INPUT_2,
	/** source from clock input 3 3MHz*/
	DUALTIMER_CLK_INPUT_3,
};

/**
 * \brief Specifies if the counter is 16-bit, or 32-bit.
 *
 * This enum specifies counter with one-shot, free running or periodic counter mode.
 */
enum dualtimer_counter_mode {
	/** Counter in one-shot mode */
	DUALTIMER_ONE_SHOT_MODE = 0,
	/** Counter is in free-running mode */
	DUALTIMER_FREE_RUNNING_MODE,
	/** Counter is in periodic mode */
	DUALTIMER_PERIODIC_MODE,
};

/**
 * \brief Dualtimer counter mode enum.
 *
 * This enum specify the maximum value it is possible to count to.
 */
enum dualtimer_counter_size {
	/** 16-bit counter */
	DUALTIMER_COUNTER_SIZE_16BIT = 0,
	/** 32-bit counter */
	DUALTIMER_COUNTER_SIZE_32BIT,
};

/**
 * \brief Dualtimer clock prescaler values.
 *
 * This enum is used to choose the clock prescaler
 * configuration. The prescaler divides the clock frequency of the
 * Dual Timer module to make the counter count slower.
 */
enum dualtimer_clock_prescaler {
	/** Divide clock by 1 */
	DUALTIMER_CLOCK_PRESCALER_DIV1 = 0,
	/** Divide clock by 16 */
	DUALTIMER_CLOCK_PRESCALER_DIV16,
	/** Divide clock by 256 */
	DUALTIMER_CLOCK_PRESCALER_DIV256,
};

/**
 * \brief Dualtimer set counter.
 *
 * This enum is used to choose set the load register or 
 * background load register. The difference to set load 
 * register is that writes to background register do not
 * cause the counter to immediately restart from the new value.
 */
enum dualtimer_set_register {
	/** Set current counter */
	DUALTIMER_SET_CURRUNT_REG = 0,
	/** Set background counter */
	DUALTIMER_SET_BG_REG,
};

/**
 * \brief Dualtimer private configuration structure.
 *
 * Private configuration struct for Dualtimer instance. 
 */
struct dualtimer_private_config {
	/** Enable timer */
	bool timer_enable;
	/** Selects one-shot or wrapping counter mode */
	enum dualtimer_counter_mode counter_mode;
	/** Selects 16-bit or 32- bit counter size */
	enum dualtimer_counter_size counter_size;
	/** Selects the prescaler value */
	enum dualtimer_clock_prescaler clock_prescaler;
	/** Enable the interrupt */
	bool interrup_enable;
	/** Counter load value */
	uint32_t load_value;
};

/**
 * \brief Dualtimer configuration structure.
 *
 * Configuration struct for Dualtimer instance. This structure should be
 * initialized by the \ref dualtimer_get_config_defaults function before being
 * modified by the user application.
 */
struct dualtimer_config {
	/** Timer1 private configuration */
	struct dualtimer_private_config timer1;
	/** Timer2 private configuration */
	struct dualtimer_private_config timer2;
	/** Selects Dualtimer clock frequency */
	enum dualtimer_clock_input clock_source;
};

void dualtimer_get_config_defaults(struct dualtimer_config *config);
void dualtimer_init(const struct dualtimer_config *config);
uint32_t dualtimer_get_value(enum dualtimer_timer timer);
void dualtimer_set_counter(enum dualtimer_timer timer,
		enum dualtimer_set_register cur_bg, uint32_t value);
uint8_t dualtimer_get_status(enum dualtimer_timer timer);
uint8_t dualtimer_get_interrupt_status(enum dualtimer_timer timer);
void dualtimer_clear_interrupt_status(enum dualtimer_timer timer);
void dualtimer_enable(enum dualtimer_timer timer);
void dualtimer_disable(enum dualtimer_timer timer);
void dualtimer_register_callback(enum dualtimer_timer timer, dualtimer_callback_t fun);
void dualtimer_unregister_callback(enum dualtimer_timer timer);
void dualtimer_isr_handler(void);
 
#ifdef __cplusplus
}
#endif

#endif