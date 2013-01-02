/**
 * @file common_sw_timer.h
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
 
/* Prevent double inclusion */
#ifndef COMMON_SW_TIMER_H
#define COMMON_SW_TIMER_H

#include "compiler.h"
#include "status_codes.h"
#include "conf_common_sw_timer.h"


/*
 * Type definition for callbacks for timer functions
 */
typedef void (*timer_expiry_cb_t)(void *);

#define TOTAL_TIMERS    TOTAL_NUMBER_OF_TIMERS
#define MAX_NO_OF_TIMERS          (25)

/*
 * Value to indicate end of timer in the array or queue
 */
#define NO_TIMER                (0xFF)

/*
 * Shift mask to obtain the 16-bit system time out of a 32-bit timeout
 */
#define SYS_TIME_SHIFT_MASK     (16)

/*
 * Mask to obtain the 16-bit H/W time out of a 32-bit timeout
 */
#define HW_TIME_MASK            (0xFFFF)

/*
 * The smallest timeout in microseconds
 */
#define MIN_TIMEOUT                     (0x80)

/*
 * The largest timeout in microseconds
 */
#define MAX_TIMEOUT                     (0x7FFFFFFF)

/**
 * Adds two time values
 */
#define ADD_TIME(a, b)                  ((a) + (b))

/**
 * Subtracts two time values
 */
#define SUB_TIME(a, b)                  ((a) - (b))

/**
 * Timeout type
 */
typedef enum sw_timeout_type_tag
{
    /** The timeout is relative to the current time. */
    SW_TIMEOUT_RELATIVE,
    /** The timeout is an absolute value. */
    SW_TIMEOUT_ABSOLUTE
} sw_timeout_type_t;

/*
 * This defines the structure of the time type.
 */
typedef struct timer_info_tag
{
    /* Timeout in microseconds */
    uint32_t abs_exp_timer;

    /* Callback function to be executed on expiry of the timer */
    FUNC_PTR timer_cb;

    /* Parameter to be passed to the callback function of the expired timer */
    void *param_cb;

    /* Next timer which was started or has expired */
    uint8_t next_timer_in_queue;
	
    bool loaded;
	
} timer_info_t;


status_code_t sw_timer_get_id(uint8_t* timer_id);
status_code_t sw_timer_start(uint8_t timer_id, 
							uint32_t timer_count,  
							sw_timeout_type_t timeout_type, 
							FUNC_PTR timer_cb, 
							void* param_cb);
status_code_t sw_timer_stop (uint8_t timer_id);
uint32_t sw_timer_get_time(void);
bool sw_timer_is_running(uint8_t timer_id);

void sw_timer_service(void);
void sw_timer_init(void);

#endif /* COMMON_SW_TIMER_H */
/* EOF */
