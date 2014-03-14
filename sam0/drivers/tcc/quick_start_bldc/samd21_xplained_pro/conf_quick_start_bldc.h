/**
 * \file
 *
 * \brief TCC Quick Start configuration for SAM D21 Xplained Pro
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

#ifndef CONF_QUICK_START_H_INCLUDED
#define CONF_QUICK_START_H_INCLUDED

//[definition_pwm]
/** PWM module to use */
#define CONF_PWM_MODULE      TCC0

/** PWM channel 0 */
#define CONF_PWM_CHANNEL0    0
/** PWM channel 1 */
#define CONF_PWM_CHANNEL1    1
/** PWM channel 2 */
#define CONF_PWM_CHANNEL2    2
/** PWM channel 3 */
#define CONF_PWM_CHANNEL3    3

/** PWM output 0 */
#define CONF_PWM_OUTPUT0     0
/** PWM output pin */
#define CONF_PWM_OUT0_PIN    PIN_PA08E_TCC0_WO0
/** PWM output pin mux */
#define CONF_PWM_OUT0_MUX    MUX_PA08E_TCC0_WO0

/** PWM output 1 */
#define CONF_PWM_OUTPUT1     1
/** PWM output pin */
#define CONF_PWM_OUT1_PIN    PIN_PA09E_TCC0_WO1
/** PWM output pin mux */
#define CONF_PWM_OUT1_MUX    MUX_PA09E_TCC0_WO1

/** PWM output 2 */
#define CONF_PWM_OUTPUT2     2
/** PWM output pin */
#define CONF_PWM_OUT2_PIN    PIN_PA10F_TCC0_WO2
/** PWM output pin mux */
#define CONF_PWM_OUT2_MUX    MUX_PA10F_TCC0_WO2

/** PWM output 3 */
#define CONF_PWM_OUTPUT3     3
/** PWM output pin */
#define CONF_PWM_OUT3_PIN    PIN_PA11F_TCC0_WO3
/** PWM output pin mux */
#define CONF_PWM_OUT3_MUX    MUX_PA11F_TCC0_WO3

/** PWM output 4 (uses inversion of channel 0 wave generation) */
#define CONF_PWM_OUTPUT4     4
/** PWM output pin */
#define CONF_PWM_OUT4_PIN    PIN_PB10F_TCC0_WO4
/** PWM output pin mux */
#define CONF_PWM_OUT4_MUX    MUX_PB10F_TCC0_WO4

/** PWM output 5 (uses inversion of channel 1 wave generation) */
#define CONF_PWM_OUTPUT5     5
/** PWM output pin */
#define CONF_PWM_OUT5_PIN    PIN_PB11F_TCC0_WO5
/** PWM output pin mux */
#define CONF_PWM_OUT5_MUX    MUX_PB11F_TCC0_WO5
//[definition_pwm]

#endif /* CONF_QUICK_START_H_INCLUDED */
