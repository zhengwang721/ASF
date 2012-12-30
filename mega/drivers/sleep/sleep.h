/**
 * \file
 *
 * \brief Sleep controller driver
 *
 * Copyright (c) 2010 Atmel Corporation. All rights reserved.
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
#ifndef _SLEEP_H
#define _SLEEP_H

#include "compiler.h"
#include "parts.h"

/**
 * \defgroup sleep_group Sleep controller driver
 *
 * This is a low-level driver implementation for the MEGA sleep controller.
 *
 * \note To minimize the code overhead, these functions do not feature
 * interrupt-protected access since they are likely to be called inside
 * interrupt handlers or in applications where such protection is not
 * necessary. If such protection is needed, it must be ensured by the calling
 * code.
 *
 * @{
 */

#if defined(__ICCAVR__) || defined(__DOXYGEN__)
# include <intrinsics.h>
/* ! Macro for issuing the sleep instruction. */
# define sleep_enter() __sleep()
#elif defined(__GNUC__)
# include <avr/sleep.h>
# define sleep_enter() sleep_cpu()
#else
# error Unsupported compiler.
#endif

/*  \brief Sleep Mode Select */
typedef enum SLEEP_SMODE_enum {
	SLEEP_SMODE_IDLE_gc = (0x00 << 1), /* Idle mode */
	SLEEP_SMODE_ADC_NOISE_REDUCTION_gc      = (0x01 << 1), /* ADC Noise
	                                                        * Reduction */
	SLEEP_SMODE_PDOWN_gc        = (0x02 << 1), /* Power-down Mode */
	SLEEP_SMODE_PSAVE_gc = (0x03 << 1), /* Power-save Mode */
	SLEEP_SMODE_STDBY_gc = (0x06 << 1), /* Standby Mode */
	SLEEP_SMODE_ESTDBY_gc = (0x07 << 1), /* Extended Standby Mode */
} SLEEP_SMODE_t;


/**
 * \code
 * sleep_set_mode(SLEEP_SMODE_PDOWN_gc);
 * sleep_enable();
 * sleep_enter();
 * sleep_disable();
 * \endcode
 *
 * \subsection Workflow
 * -# Set what sleep mode to use, the different sleep modes can be found in the
 *  device header file under the enum definition SLEEP_SMODE_enum:
 *  - \code sleep_set_mode(SLEEP_SMODE_PDOWN_gc); \endcode
 * -# Enable that the device are allowed to go to sleep:
 *  - \code sleep_enable(); \endcode
 *  - \note This function has to be called in order for the device to go to
 *  sleep. This is a safety feature to stop the device to go to sleep
 *  unintentionally, even though it is possible to have this enabled at all
 *  times
 *  it is recommended to enable sleep mode only when you intend to go to sleep
 *  within a few clock cycles.
 * -# Enter sleep mode:
 *  - \code sleep_enter(); \endcode
 *  - \attention Make sure to enable global interrupt and the interrupt you
 *  plan to use as wake-up source for your device, do also pay special
 *  attention to what wake-up sources are available for the different sleep
 *  modes. Failing to enable interrupts may result in indefinite sleep until
 *  power is cycled!
 * -# When the device is woken from sleep it will execute the interrupt handler
 *  related to the wakeup-source (interrupt source) and continue on the next
 *  line of code after the \ref sleep_enter() call. Make sure to disable sleep
 *  when waking up.
 *  - \code sleep_disable(); \endcode
 *
 *  \subsection Sleep Modes
 *  Possible sleep modes depend on the device that is used. Please refer to the
 *  device datasheet and header file to find these definitions.
 */

#endif /* _SLEEP_H */
