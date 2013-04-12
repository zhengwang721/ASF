/**
 * \file
 *
 * \brief ARM functions for busy-wait delay loops
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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
#ifndef _cycle_counter_h_
#define _cycle_counter_h_

#ifdef __cplusplus
extern "C" {
#endif


#include <compiler.h>
#include <clock.h>

/**
 * @name Convenience functions for busy-wait delay loops
 *
 * @def delay_cycles
 * @brief Delay program execution for at least the specified number of CPU cycles.
 * @param n number of CPU cycles to wait
 *
 * @def cpu_delay_ms
 * @brief Delay program execution for at least the specified number of milliseconds.
 * @param delay number of milliseconds to wait
 *
 * @def cpu_delay_s
 * @brief Delay program execution for at least the specified number of seconds.
 * @param delay number of seconds to wait
 * @{
 */

/**
 * \brief Delay loop to delay n number of cycles
 *
 * \param n Number of cycles
 */
static inline void delay_cycles(uint32_t n)
{
	SysTick->LOAD = n;
	SysTick->VAL = 0;
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}

void delay_cycles_us(uint32_t n);

void delay_cycles_ms(uint32_t n);

void delay_init(void);

#define cpu_delay_cycles(delay) delay_cycles(delay)
#define cpu_delay_us(delay) delay_cycles_us(delay)
#define cpu_delay_ms(delay) delay_cycles_ms(delay)
#define cpu_delay_s(delay) delay_cycles_ms(1000 * delay)
//! @}


#ifdef __cplusplus
}
#endif

#endif /* _cycle_counter_h_ */
