/**
 * \file
 *
 * \brief SAM L21 Xplained Pro board configuration.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

#define LUT0_IN0_MUX  MUX_PA04I_CCL_IN0
#define LUT0_IN1_MUX  MUX_PA05I_CCL_IN1
#define LUT0_IN2_MUX  MUX_PA06I_CCL_IN2
#define LUT0_OUT_MUX  MUX_PA07I_CCL_OUT0

#define LUT0_IN0_PIN  PIN_PA04I_CCL_IN0
#define LUT0_IN1_PIN  PIN_PA05I_CCL_IN1
#define LUT0_IN2_PIN  PIN_PA06I_CCL_IN2
#define LUT0_OUT_PIN  PIN_PA07I_CCL_OUT0

#define LUT1_IN0_MUX  MUX_PA08I_CCL_IN3
#define LUT1_IN1_MUX  MUX_PA09I_CCL_IN4
#define LUT1_IN2_MUX  MUX_PA10I_CCL_IN5
#define LUT1_OUT_MUX  MUX_PA11I_CCL_OUT1

#define LUT1_IN0_PIN  PIN_PA08I_CCL_IN3
#define LUT1_IN1_PIN  PIN_PA09I_CCL_IN4
#define LUT1_IN2_PIN  PIN_PA10I_CCL_IN5
#define LUT1_OUT_PIN  PIN_PA11I_CCL_OUT1

#endif /* CONF_BOARD_H_INCLUDED */
