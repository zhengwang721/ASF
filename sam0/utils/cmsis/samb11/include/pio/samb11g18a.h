/**
 * \file
 *
 * \brief Peripheral I/O description for SAMB11G18A
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#ifndef _SAMB11G18A_PIO_
#define _SAMB11G18A_PIO_


#define PIN_LP_GPIO_0        0
#define PIN_LP_GPIO_1        1
#define PIN_LP_GPIO_2        2
#define PIN_LP_GPIO_3        3
#define PIN_LP_GPIO_4        4
#define PIN_LP_GPIO_5        5
#define PIN_LP_GPIO_6        6
#define PIN_LP_GPIO_7        7
#define PIN_LP_GPIO_8        8
#define PIN_LP_GPIO_9        9
#define PIN_LP_GPIO_10       10
#define PIN_LP_GPIO_11       11
#define PIN_LP_GPIO_12       12
#define PIN_LP_GPIO_13       13
#define PIN_LP_GPIO_14       14
#define PIN_LP_GPIO_15       15
#define PIN_LP_GPIO_16       16
#define PIN_LP_GPIO_17       17
#define PIN_LP_GPIO_18       18
#define PIN_LP_GPIO_19       19
#define PIN_LP_GPIO_20       20
#ifdef CHIPVERSION_B0
	#define PIN_LP_GPIO_21   21
	#define PIN_LP_GPIO_22   22
	#define PIN_LP_GPIO_23   23
	#define PIN_LP_GPIO_24   24
#endif

/* ========== GPIO definition for SPI peripheral ========== */
//Candy Please fill it

/* ========== GPIO definition for I2C peripheral ========== */
#define PINMUX_LP_GPIO_8_MUX2_I2C0_SDA		((PIN_LP_GPIO_8 << 16) | 2)
#define PINMUX_LP_GPIO_9_MUX2_I2C0_SCK		((PIN_LP_GPIO_9 << 16) | 2)
#define PINMUX_LP_GPIO_14_MUX4_I2C1_SDA		((PIN_LP_GPIO_14 << 16) | 4)
#define PINMUX_LP_GPIO_15_MUX4_I2C1_SCK		((PIN_LP_GPIO_15 << 16) | 4)

#endif /* _SAMB11G18A_PIO_ */
