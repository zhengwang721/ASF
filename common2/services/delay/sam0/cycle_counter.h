/**
 * \file
 *
 * \brief ARM functions for busy-wait delay loops
 *
 * Copyright (c) 2013-2014 Atmel Corporation. All rights reserved.
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
#ifndef CYCLE_COUNTER_H_INCLUDED
#define CYCLE_COUNTER_H_INCLUDED

#include <compiler.h>
#include <clock.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def F_CPU
 * @brief MCU Clock Frequency (Hertz)
 *
 * @deprecated
 * The \ref F_CPU configuration constant is used for compatibility with the
 * \ref group_common_services_delay routines. The common loop-based delay
 * routines are designed to use the \ref clk_group modules while anticipating
 * support for legacy applications assuming a statically defined clock
 * frequency. Applications using a statically configured MCU clock frequency
 * can define \ref F_CPU (Hertz), in which case the common delay routines will
 * use this value rather than calling sysclk_get_cpu_hz() to get the current
 * MCU clock frequency.
 */
#ifndef F_CPU
#       define F_CPU system_gclk_gen_get_hz(0)
#endif

/**
 * @name Convenience functions for busy-wait delay loops
 *
 * @def delay_cycles
 * @brief Delay program execution for a specified number of CPU cycles.
 * @param n number of CPU cycles to wait
 *
 * @def cpu_delay_ms
 * @brief Delay program execution for a specified number of milliseconds.
 * @param delay number of milliseconds to wait
 * @param f_cpu CPU frequency in Hertz
 *
 * @def cpu_delay_us
 * @brief Delay program execution for a specified number of microseconds.
 * @param delay number of microseconds to wait
 * @param f_cpu CPU frequency in Hertz
 *
 * @def cpu_ms_2_cy
 * @brief Convert milli-seconds into CPU cycles.
 * @param ms number of milliseconds
 * @param f_cpu CPU frequency in Hertz
 * @return the converted number of CPU cycles
 *
 * @def cpu_us_2_cy
 * @brief Convert micro-seconds into CPU cycles.
 * @param ms number of microseconds
 * @param f_cpu CPU frequency in Hertz
 * @return the converted number of CPU cycles
 *
 * @{
 */

/**
 * \brief Delay loop to delay n number of cycles
 *
 * \note The function runs in internal RAM so that flash wait states
 *       will not affect the delay time.
 *
 * \param n Number of cycles
 */
void portable_delay_cycles(unsigned long n);

#define cpu_ms_2_cy(ms, f_cpu)  \
	(((uint64_t)(ms) * (f_cpu) + (uint64_t)(14e3-1ul)) / (uint64_t)14e3)
#define cpu_us_2_cy(us, f_cpu)  \
	(((uint64_t)(us) * (f_cpu) + (uint64_t)(14e6-1ul)) / (uint64_t)14e6)

#define delay_cycles               portable_delay_cycles

#define cpu_delay_s(delay) delay_cycles(cpu_ms_2_cy(1000 * delay, F_CPU))
#define cpu_delay_ms(delay) delay_cycles(cpu_ms_2_cy(delay, F_CPU))
#define cpu_delay_us(delay) delay_cycles(cpu_us_2_cy(delay, F_CPU))
//! @}


#ifdef __cplusplus
}
#endif

#endif /* CYCLE_COUNTER_H_INCLUDED */
