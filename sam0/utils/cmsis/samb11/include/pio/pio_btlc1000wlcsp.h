/**
 * \file
 *
 * \brief Peripheral I/O description for BTLC1000WLCSP
 *
 * Copyright (c) 2016 Atmel Corporation. All rights reserved.
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

#ifndef _BTLC1000WLCSP_PIO_
#define _BTLC1000WLCSP_PIO_

#define PIN_LP_GPIO_0                     0  /**< Pin Number for LP_GPIO_0 */
#define GPIO_LP_GPIO_0           (1ul <<  0) /**< GPIO Mask for LP_GPIO_0 */
#define PIN_LP_GPIO_1                     1  /**< Pin Number for LP_GPIO_1 */
#define GPIO_LP_GPIO_1           (1ul <<  1) /**< GPIO Mask for LP_GPIO_1 */
#define PIN_LP_GPIO_2                     2  /**< Pin Number for LP_GPIO_2 */
#define GPIO_LP_GPIO_2           (1ul <<  2) /**< GPIO Mask for LP_GPIO_2 */
#define PIN_LP_GPIO_3                     3  /**< Pin Number for LP_GPIO_3 */
#define GPIO_LP_GPIO_3           (1ul <<  3) /**< GPIO Mask for LP_GPIO_3 */
#define PIN_LP_GPIO_8                     8  /**< Pin Number for LP_GPIO_8 */
#define GPIO_LP_GPIO_8           (1ul <<  8) /**< GPIO Mask for LP_GPIO_8 */
#define PIN_LP_GPIO_9                     9  /**< Pin Number for LP_GPIO_9 */
#define GPIO_LP_GPIO_9           (1ul <<  9) /**< GPIO Mask for LP_GPIO_9 */
#define PIN_LP_GPIO_10                   10  /**< Pin Number for LP_GPIO_10 */
#define GPIO_LP_GPIO_10          (1ul << 10) /**< GPIO Mask for LP_GPIO_10 */
#define PIN_LP_GPIO_11                   11  /**< Pin Number for LP_GPIO_11 */
#define GPIO_LP_GPIO_11          (1ul << 11) /**< GPIO Mask for LP_GPIO_11 */
#define PIN_LP_GPIO_12                   12  /**< Pin Number for LP_GPIO_12 */
#define GPIO_LP_GPIO_12          (1ul << 12) /**< GPIO Mask for LP_GPIO_12 */
#define PIN_LP_GPIO_13                   13  /**< Pin Number for LP_GPIO_13 */
#define GPIO_LP_GPIO_13          (1ul << 13) /**< GPIO Mask for LP_GPIO_13 */
#define PIN_AO_GPIO_0                    31  /**< Pin Number for AO_GPIO_0 */
#define GPIO_AO_GPIO_0           (1ul << 15) /**< GPIO Mask for AO_GPIO_0 */
#define PIN_GPIO_MS1                     47  /**< Pin Number for GPIO_MS1 */
#define GPIO_GPIO_MS1            (1ul << 15) /**< GPIO Mask for GPIO_MS1 */

/* PinMux definitions                                               */
/* Mux values for LP_GPIO_0 */
#define MUX_LP_GPIO_0_MUX6                     0x6
#define MUX_LP_GPIO_0_MUX7                     0x7
#define MUX_LP_GPIO_0_MUX4                     0x4
#define MUX_LP_GPIO_0_MUX5                     0x5
#define MUX_LP_GPIO_0_MUX2                     0x2
#define MUX_LP_GPIO_0_MUX3                     0x3
#define MUX_LP_GPIO_0_MUX0                     0x0
#define MUX_LP_GPIO_0_MUX1                     0x1
/* Mux values for LP_GPIO_1 */
#define MUX_LP_GPIO_1_MUX6                     0x6
#define MUX_LP_GPIO_1_MUX7                     0x7
#define MUX_LP_GPIO_1_MUX4                     0x4
#define MUX_LP_GPIO_1_MUX5                     0x5
#define MUX_LP_GPIO_1_MUX2                     0x2
#define MUX_LP_GPIO_1_MUX3                     0x3
#define MUX_LP_GPIO_1_MUX0                     0x0
#define MUX_LP_GPIO_1_MUX1                     0x1
/* Mux values for LP_GPIO_2 */
#define MUX_LP_GPIO_2_MUX6                     0x6
#define MUX_LP_GPIO_2_MUX7                     0x7
#define MUX_LP_GPIO_2_MUX4                     0x4
#define MUX_LP_GPIO_2_MUX5                     0x5
#define MUX_LP_GPIO_2_MUX2                     0x2
#define MUX_LP_GPIO_2_MUX3                     0x3
#define MUX_LP_GPIO_2_MUX0                     0x0
#define MUX_LP_GPIO_2_MUX1                     0x1
/* Mux values for LP_GPIO_3 */
#define MUX_LP_GPIO_3_MUX6                     0x6
#define MUX_LP_GPIO_3_MUX7                     0x7
#define MUX_LP_GPIO_3_MUX4                     0x4
#define MUX_LP_GPIO_3_MUX5                     0x5
#define MUX_LP_GPIO_3_MUX2                     0x2
#define MUX_LP_GPIO_3_MUX3                     0x3
#define MUX_LP_GPIO_3_MUX0                     0x0
#define MUX_LP_GPIO_3_MUX1                     0x1
/* Mux values for LP_GPIO_8 */
#define MUX_LP_GPIO_8_MUX6                     0x6
#define MUX_LP_GPIO_8_MUX7                     0x7
#define MUX_LP_GPIO_8_MUX4                     0x4
#define MUX_LP_GPIO_8_MUX5                     0x5
#define MUX_LP_GPIO_8_MUX2                     0x2
#define MUX_LP_GPIO_8_MUX3                     0x3
#define MUX_LP_GPIO_8_MUX0                     0x0
#define MUX_LP_GPIO_8_MUX1                     0x1
/* Mux values for LP_GPIO_9 */
#define MUX_LP_GPIO_9_MUX6                     0x6
#define MUX_LP_GPIO_9_MUX7                     0x7
#define MUX_LP_GPIO_9_MUX4                     0x4
#define MUX_LP_GPIO_9_MUX5                     0x5
#define MUX_LP_GPIO_9_MUX2                     0x2
#define MUX_LP_GPIO_9_MUX3                     0x3
#define MUX_LP_GPIO_9_MUX0                     0x0
#define MUX_LP_GPIO_9_MUX1                     0x1
/* Mux values for LP_GPIO_10 */
#define MUX_LP_GPIO_10_MUX6                    0x6
#define MUX_LP_GPIO_10_MUX7                    0x7
#define MUX_LP_GPIO_10_MUX4                    0x4
#define MUX_LP_GPIO_10_MUX5                    0x5
#define MUX_LP_GPIO_10_MUX2                    0x2
#define MUX_LP_GPIO_10_MUX3                    0x3
#define MUX_LP_GPIO_10_MUX0                    0x0
#define MUX_LP_GPIO_10_MUX1                    0x1
/* Mux values for LP_GPIO_11 */
#define MUX_LP_GPIO_11_MUX6                    0x6
#define MUX_LP_GPIO_11_MUX7                    0x7
#define MUX_LP_GPIO_11_MUX4                    0x4
#define MUX_LP_GPIO_11_MUX5                    0x5
#define MUX_LP_GPIO_11_MUX2                    0x2
#define MUX_LP_GPIO_11_MUX3                    0x3
#define MUX_LP_GPIO_11_MUX0                    0x0
#define MUX_LP_GPIO_11_MUX1                    0x1
/* Mux values for LP_GPIO_12 */
#define MUX_LP_GPIO_12_MUX6                    0x6
#define MUX_LP_GPIO_12_MUX7                    0x7
#define MUX_LP_GPIO_12_MUX4                    0x4
#define MUX_LP_GPIO_12_MUX5                    0x5
#define MUX_LP_GPIO_12_MUX2                    0x2
#define MUX_LP_GPIO_12_MUX3                    0x3
#define MUX_LP_GPIO_12_MUX0                    0x0
#define MUX_LP_GPIO_12_MUX1                    0x1
/* Mux values for LP_GPIO_13 */
#define MUX_LP_GPIO_13_MUX6                    0x6
#define MUX_LP_GPIO_13_MUX7                    0x7
#define MUX_LP_GPIO_13_MUX4                    0x4
#define MUX_LP_GPIO_13_MUX5                    0x5
#define MUX_LP_GPIO_13_MUX2                    0x2
#define MUX_LP_GPIO_13_MUX3                    0x3
#define MUX_LP_GPIO_13_MUX0                    0x0
#define MUX_LP_GPIO_13_MUX1                    0x1

/* MegaMux definitions                                              */
/* MegaMux values for LP_GPIO_0 */
#define MEGAMUX_LP_GPIO_0_24                       0x18
#define MEGAMUX_LP_GPIO_0_25                       0x19
#define MEGAMUX_LP_GPIO_0_26                       0x1a
#define MEGAMUX_LP_GPIO_0_27                       0x1b
#define MEGAMUX_LP_GPIO_0_20                       0x14
#define MEGAMUX_LP_GPIO_0_21                       0x15
#define MEGAMUX_LP_GPIO_0_22                       0x16
#define MEGAMUX_LP_GPIO_0_23                       0x17
#define MEGAMUX_LP_GPIO_0_28                       0x1c
#define MEGAMUX_LP_GPIO_0_29                       0x1d
#define MEGAMUX_LP_GPIO_0_1                        0x1
#define MEGAMUX_LP_GPIO_0_0                        0x0
#define MEGAMUX_LP_GPIO_0_3                        0x3
#define MEGAMUX_LP_GPIO_0_2                        0x2
#define MEGAMUX_LP_GPIO_0_5                        0x5
#define MEGAMUX_LP_GPIO_0_4                        0x4
#define MEGAMUX_LP_GPIO_0_7                        0x7
#define MEGAMUX_LP_GPIO_0_6                        0x6
#define MEGAMUX_LP_GPIO_0_9                        0x9
#define MEGAMUX_LP_GPIO_0_8                        0x8
#define MEGAMUX_LP_GPIO_0_11                       0xb
#define MEGAMUX_LP_GPIO_0_10                       0xa
#define MEGAMUX_LP_GPIO_0_13                       0xd
#define MEGAMUX_LP_GPIO_0_12                       0xc
#define MEGAMUX_LP_GPIO_0_15                       0xf
#define MEGAMUX_LP_GPIO_0_14                       0xe
#define MEGAMUX_LP_GPIO_0_17                       0x11
#define MEGAMUX_LP_GPIO_0_16                       0x10
#define MEGAMUX_LP_GPIO_0_19                       0x13
#define MEGAMUX_LP_GPIO_0_18                       0x12
#define MEGAMUX_LP_GPIO_0_31                       0x1f
#define MEGAMUX_LP_GPIO_0_30                       0x1e
#define MEGAMUX_LP_GPIO_0_34                       0x22
#define MEGAMUX_LP_GPIO_0_33                       0x21
#define MEGAMUX_LP_GPIO_0_32                       0x20
/* MegaMux values for LP_GPIO_1 */
#define MEGAMUX_LP_GPIO_1_24                       0x18
#define MEGAMUX_LP_GPIO_1_25                       0x19
#define MEGAMUX_LP_GPIO_1_26                       0x1a
#define MEGAMUX_LP_GPIO_1_27                       0x1b
#define MEGAMUX_LP_GPIO_1_20                       0x14
#define MEGAMUX_LP_GPIO_1_21                       0x15
#define MEGAMUX_LP_GPIO_1_22                       0x16
#define MEGAMUX_LP_GPIO_1_23                       0x17
#define MEGAMUX_LP_GPIO_1_28                       0x1c
#define MEGAMUX_LP_GPIO_1_29                       0x1d
#define MEGAMUX_LP_GPIO_1_1                        0x1
#define MEGAMUX_LP_GPIO_1_0                        0x0
#define MEGAMUX_LP_GPIO_1_3                        0x3
#define MEGAMUX_LP_GPIO_1_2                        0x2
#define MEGAMUX_LP_GPIO_1_5                        0x5
#define MEGAMUX_LP_GPIO_1_4                        0x4
#define MEGAMUX_LP_GPIO_1_7                        0x7
#define MEGAMUX_LP_GPIO_1_6                        0x6
#define MEGAMUX_LP_GPIO_1_9                        0x9
#define MEGAMUX_LP_GPIO_1_8                        0x8
#define MEGAMUX_LP_GPIO_1_11                       0xb
#define MEGAMUX_LP_GPIO_1_10                       0xa
#define MEGAMUX_LP_GPIO_1_13                       0xd
#define MEGAMUX_LP_GPIO_1_12                       0xc
#define MEGAMUX_LP_GPIO_1_15                       0xf
#define MEGAMUX_LP_GPIO_1_14                       0xe
#define MEGAMUX_LP_GPIO_1_17                       0x11
#define MEGAMUX_LP_GPIO_1_16                       0x10
#define MEGAMUX_LP_GPIO_1_19                       0x13
#define MEGAMUX_LP_GPIO_1_18                       0x12
#define MEGAMUX_LP_GPIO_1_31                       0x1f
#define MEGAMUX_LP_GPIO_1_30                       0x1e
#define MEGAMUX_LP_GPIO_1_34                       0x22
#define MEGAMUX_LP_GPIO_1_33                       0x21
#define MEGAMUX_LP_GPIO_1_32                       0x20
/* MegaMux values for LP_GPIO_2 */
#define MEGAMUX_LP_GPIO_2_24                       0x18
#define MEGAMUX_LP_GPIO_2_25                       0x19
#define MEGAMUX_LP_GPIO_2_26                       0x1a
#define MEGAMUX_LP_GPIO_2_27                       0x1b
#define MEGAMUX_LP_GPIO_2_20                       0x14
#define MEGAMUX_LP_GPIO_2_21                       0x15
#define MEGAMUX_LP_GPIO_2_22                       0x16
#define MEGAMUX_LP_GPIO_2_23                       0x17
#define MEGAMUX_LP_GPIO_2_28                       0x1c
#define MEGAMUX_LP_GPIO_2_29                       0x1d
#define MEGAMUX_LP_GPIO_2_1                        0x1
#define MEGAMUX_LP_GPIO_2_0                        0x0
#define MEGAMUX_LP_GPIO_2_3                        0x3
#define MEGAMUX_LP_GPIO_2_2                        0x2
#define MEGAMUX_LP_GPIO_2_5                        0x5
#define MEGAMUX_LP_GPIO_2_4                        0x4
#define MEGAMUX_LP_GPIO_2_7                        0x7
#define MEGAMUX_LP_GPIO_2_6                        0x6
#define MEGAMUX_LP_GPIO_2_9                        0x9
#define MEGAMUX_LP_GPIO_2_8                        0x8
#define MEGAMUX_LP_GPIO_2_11                       0xb
#define MEGAMUX_LP_GPIO_2_10                       0xa
#define MEGAMUX_LP_GPIO_2_13                       0xd
#define MEGAMUX_LP_GPIO_2_12                       0xc
#define MEGAMUX_LP_GPIO_2_15                       0xf
#define MEGAMUX_LP_GPIO_2_14                       0xe
#define MEGAMUX_LP_GPIO_2_17                       0x11
#define MEGAMUX_LP_GPIO_2_16                       0x10
#define MEGAMUX_LP_GPIO_2_19                       0x13
#define MEGAMUX_LP_GPIO_2_18                       0x12
#define MEGAMUX_LP_GPIO_2_31                       0x1f
#define MEGAMUX_LP_GPIO_2_30                       0x1e
#define MEGAMUX_LP_GPIO_2_34                       0x22
#define MEGAMUX_LP_GPIO_2_33                       0x21
#define MEGAMUX_LP_GPIO_2_32                       0x20
/* MegaMux values for LP_GPIO_3 */
#define MEGAMUX_LP_GPIO_3_24                       0x18
#define MEGAMUX_LP_GPIO_3_25                       0x19
#define MEGAMUX_LP_GPIO_3_26                       0x1a
#define MEGAMUX_LP_GPIO_3_27                       0x1b
#define MEGAMUX_LP_GPIO_3_20                       0x14
#define MEGAMUX_LP_GPIO_3_21                       0x15
#define MEGAMUX_LP_GPIO_3_22                       0x16
#define MEGAMUX_LP_GPIO_3_23                       0x17
#define MEGAMUX_LP_GPIO_3_28                       0x1c
#define MEGAMUX_LP_GPIO_3_29                       0x1d
#define MEGAMUX_LP_GPIO_3_1                        0x1
#define MEGAMUX_LP_GPIO_3_0                        0x0
#define MEGAMUX_LP_GPIO_3_3                        0x3
#define MEGAMUX_LP_GPIO_3_2                        0x2
#define MEGAMUX_LP_GPIO_3_5                        0x5
#define MEGAMUX_LP_GPIO_3_4                        0x4
#define MEGAMUX_LP_GPIO_3_7                        0x7
#define MEGAMUX_LP_GPIO_3_6                        0x6
#define MEGAMUX_LP_GPIO_3_9                        0x9
#define MEGAMUX_LP_GPIO_3_8                        0x8
#define MEGAMUX_LP_GPIO_3_11                       0xb
#define MEGAMUX_LP_GPIO_3_10                       0xa
#define MEGAMUX_LP_GPIO_3_13                       0xd
#define MEGAMUX_LP_GPIO_3_12                       0xc
#define MEGAMUX_LP_GPIO_3_15                       0xf
#define MEGAMUX_LP_GPIO_3_14                       0xe
#define MEGAMUX_LP_GPIO_3_17                       0x11
#define MEGAMUX_LP_GPIO_3_16                       0x10
#define MEGAMUX_LP_GPIO_3_19                       0x13
#define MEGAMUX_LP_GPIO_3_18                       0x12
#define MEGAMUX_LP_GPIO_3_31                       0x1f
#define MEGAMUX_LP_GPIO_3_30                       0x1e
#define MEGAMUX_LP_GPIO_3_34                       0x22
#define MEGAMUX_LP_GPIO_3_33                       0x21
#define MEGAMUX_LP_GPIO_3_32                       0x20
/* MegaMux values for LP_GPIO_8 */
#define MEGAMUX_LP_GPIO_8_24                       0x18
#define MEGAMUX_LP_GPIO_8_25                       0x19
#define MEGAMUX_LP_GPIO_8_26                       0x1a
#define MEGAMUX_LP_GPIO_8_27                       0x1b
#define MEGAMUX_LP_GPIO_8_20                       0x14
#define MEGAMUX_LP_GPIO_8_21                       0x15
#define MEGAMUX_LP_GPIO_8_22                       0x16
#define MEGAMUX_LP_GPIO_8_23                       0x17
#define MEGAMUX_LP_GPIO_8_28                       0x1c
#define MEGAMUX_LP_GPIO_8_29                       0x1d
#define MEGAMUX_LP_GPIO_8_1                        0x1
#define MEGAMUX_LP_GPIO_8_0                        0x0
#define MEGAMUX_LP_GPIO_8_3                        0x3
#define MEGAMUX_LP_GPIO_8_2                        0x2
#define MEGAMUX_LP_GPIO_8_5                        0x5
#define MEGAMUX_LP_GPIO_8_4                        0x4
#define MEGAMUX_LP_GPIO_8_7                        0x7
#define MEGAMUX_LP_GPIO_8_6                        0x6
#define MEGAMUX_LP_GPIO_8_9                        0x9
#define MEGAMUX_LP_GPIO_8_8                        0x8
#define MEGAMUX_LP_GPIO_8_11                       0xb
#define MEGAMUX_LP_GPIO_8_10                       0xa
#define MEGAMUX_LP_GPIO_8_13                       0xd
#define MEGAMUX_LP_GPIO_8_12                       0xc
#define MEGAMUX_LP_GPIO_8_15                       0xf
#define MEGAMUX_LP_GPIO_8_14                       0xe
#define MEGAMUX_LP_GPIO_8_17                       0x11
#define MEGAMUX_LP_GPIO_8_16                       0x10
#define MEGAMUX_LP_GPIO_8_19                       0x13
#define MEGAMUX_LP_GPIO_8_18                       0x12
#define MEGAMUX_LP_GPIO_8_31                       0x1f
#define MEGAMUX_LP_GPIO_8_30                       0x1e
#define MEGAMUX_LP_GPIO_8_34                       0x22
#define MEGAMUX_LP_GPIO_8_33                       0x21
#define MEGAMUX_LP_GPIO_8_32                       0x20
/* MegaMux values for LP_GPIO_9 */
#define MEGAMUX_LP_GPIO_9_24                       0x18
#define MEGAMUX_LP_GPIO_9_25                       0x19
#define MEGAMUX_LP_GPIO_9_26                       0x1a
#define MEGAMUX_LP_GPIO_9_27                       0x1b
#define MEGAMUX_LP_GPIO_9_20                       0x14
#define MEGAMUX_LP_GPIO_9_21                       0x15
#define MEGAMUX_LP_GPIO_9_22                       0x16
#define MEGAMUX_LP_GPIO_9_23                       0x17
#define MEGAMUX_LP_GPIO_9_28                       0x1c
#define MEGAMUX_LP_GPIO_9_29                       0x1d
#define MEGAMUX_LP_GPIO_9_1                        0x1
#define MEGAMUX_LP_GPIO_9_0                        0x0
#define MEGAMUX_LP_GPIO_9_3                        0x3
#define MEGAMUX_LP_GPIO_9_2                        0x2
#define MEGAMUX_LP_GPIO_9_5                        0x5
#define MEGAMUX_LP_GPIO_9_4                        0x4
#define MEGAMUX_LP_GPIO_9_7                        0x7
#define MEGAMUX_LP_GPIO_9_6                        0x6
#define MEGAMUX_LP_GPIO_9_9                        0x9
#define MEGAMUX_LP_GPIO_9_8                        0x8
#define MEGAMUX_LP_GPIO_9_11                       0xb
#define MEGAMUX_LP_GPIO_9_10                       0xa
#define MEGAMUX_LP_GPIO_9_13                       0xd
#define MEGAMUX_LP_GPIO_9_12                       0xc
#define MEGAMUX_LP_GPIO_9_15                       0xf
#define MEGAMUX_LP_GPIO_9_14                       0xe
#define MEGAMUX_LP_GPIO_9_17                       0x11
#define MEGAMUX_LP_GPIO_9_16                       0x10
#define MEGAMUX_LP_GPIO_9_19                       0x13
#define MEGAMUX_LP_GPIO_9_18                       0x12
#define MEGAMUX_LP_GPIO_9_31                       0x1f
#define MEGAMUX_LP_GPIO_9_30                       0x1e
#define MEGAMUX_LP_GPIO_9_34                       0x22
#define MEGAMUX_LP_GPIO_9_33                       0x21
#define MEGAMUX_LP_GPIO_9_32                       0x20
/* MegaMux values for LP_GPIO_10 */
#define MEGAMUX_LP_GPIO_10_24                      0x18
#define MEGAMUX_LP_GPIO_10_25                      0x19
#define MEGAMUX_LP_GPIO_10_26                      0x1a
#define MEGAMUX_LP_GPIO_10_27                      0x1b
#define MEGAMUX_LP_GPIO_10_20                      0x14
#define MEGAMUX_LP_GPIO_10_21                      0x15
#define MEGAMUX_LP_GPIO_10_22                      0x16
#define MEGAMUX_LP_GPIO_10_23                      0x17
#define MEGAMUX_LP_GPIO_10_28                      0x1c
#define MEGAMUX_LP_GPIO_10_29                      0x1d
#define MEGAMUX_LP_GPIO_10_1                       0x1
#define MEGAMUX_LP_GPIO_10_0                       0x0
#define MEGAMUX_LP_GPIO_10_3                       0x3
#define MEGAMUX_LP_GPIO_10_2                       0x2
#define MEGAMUX_LP_GPIO_10_5                       0x5
#define MEGAMUX_LP_GPIO_10_4                       0x4
#define MEGAMUX_LP_GPIO_10_7                       0x7
#define MEGAMUX_LP_GPIO_10_6                       0x6
#define MEGAMUX_LP_GPIO_10_9                       0x9
#define MEGAMUX_LP_GPIO_10_8                       0x8
#define MEGAMUX_LP_GPIO_10_11                      0xb
#define MEGAMUX_LP_GPIO_10_10                      0xa
#define MEGAMUX_LP_GPIO_10_13                      0xd
#define MEGAMUX_LP_GPIO_10_12                      0xc
#define MEGAMUX_LP_GPIO_10_15                      0xf
#define MEGAMUX_LP_GPIO_10_14                      0xe
#define MEGAMUX_LP_GPIO_10_17                      0x11
#define MEGAMUX_LP_GPIO_10_16                      0x10
#define MEGAMUX_LP_GPIO_10_19                      0x13
#define MEGAMUX_LP_GPIO_10_18                      0x12
#define MEGAMUX_LP_GPIO_10_31                      0x1f
#define MEGAMUX_LP_GPIO_10_30                      0x1e
#define MEGAMUX_LP_GPIO_10_34                      0x22
#define MEGAMUX_LP_GPIO_10_33                      0x21
#define MEGAMUX_LP_GPIO_10_32                      0x20
/* MegaMux values for LP_GPIO_11 */
#define MEGAMUX_LP_GPIO_11_24                      0x18
#define MEGAMUX_LP_GPIO_11_25                      0x19
#define MEGAMUX_LP_GPIO_11_26                      0x1a
#define MEGAMUX_LP_GPIO_11_27                      0x1b
#define MEGAMUX_LP_GPIO_11_20                      0x14
#define MEGAMUX_LP_GPIO_11_21                      0x15
#define MEGAMUX_LP_GPIO_11_22                      0x16
#define MEGAMUX_LP_GPIO_11_23                      0x17
#define MEGAMUX_LP_GPIO_11_28                      0x1c
#define MEGAMUX_LP_GPIO_11_29                      0x1d
#define MEGAMUX_LP_GPIO_11_1                       0x1
#define MEGAMUX_LP_GPIO_11_0                       0x0
#define MEGAMUX_LP_GPIO_11_3                       0x3
#define MEGAMUX_LP_GPIO_11_2                       0x2
#define MEGAMUX_LP_GPIO_11_5                       0x5
#define MEGAMUX_LP_GPIO_11_4                       0x4
#define MEGAMUX_LP_GPIO_11_7                       0x7
#define MEGAMUX_LP_GPIO_11_6                       0x6
#define MEGAMUX_LP_GPIO_11_9                       0x9
#define MEGAMUX_LP_GPIO_11_8                       0x8
#define MEGAMUX_LP_GPIO_11_11                      0xb
#define MEGAMUX_LP_GPIO_11_10                      0xa
#define MEGAMUX_LP_GPIO_11_13                      0xd
#define MEGAMUX_LP_GPIO_11_12                      0xc
#define MEGAMUX_LP_GPIO_11_15                      0xf
#define MEGAMUX_LP_GPIO_11_14                      0xe
#define MEGAMUX_LP_GPIO_11_17                      0x11
#define MEGAMUX_LP_GPIO_11_16                      0x10
#define MEGAMUX_LP_GPIO_11_19                      0x13
#define MEGAMUX_LP_GPIO_11_18                      0x12
#define MEGAMUX_LP_GPIO_11_31                      0x1f
#define MEGAMUX_LP_GPIO_11_30                      0x1e
#define MEGAMUX_LP_GPIO_11_34                      0x22
#define MEGAMUX_LP_GPIO_11_33                      0x21
#define MEGAMUX_LP_GPIO_11_32                      0x20
/* MegaMux values for LP_GPIO_12 */
#define MEGAMUX_LP_GPIO_12_24                      0x18
#define MEGAMUX_LP_GPIO_12_25                      0x19
#define MEGAMUX_LP_GPIO_12_26                      0x1a
#define MEGAMUX_LP_GPIO_12_27                      0x1b
#define MEGAMUX_LP_GPIO_12_20                      0x14
#define MEGAMUX_LP_GPIO_12_21                      0x15
#define MEGAMUX_LP_GPIO_12_22                      0x16
#define MEGAMUX_LP_GPIO_12_23                      0x17
#define MEGAMUX_LP_GPIO_12_28                      0x1c
#define MEGAMUX_LP_GPIO_12_29                      0x1d
#define MEGAMUX_LP_GPIO_12_1                       0x1
#define MEGAMUX_LP_GPIO_12_0                       0x0
#define MEGAMUX_LP_GPIO_12_3                       0x3
#define MEGAMUX_LP_GPIO_12_2                       0x2
#define MEGAMUX_LP_GPIO_12_5                       0x5
#define MEGAMUX_LP_GPIO_12_4                       0x4
#define MEGAMUX_LP_GPIO_12_7                       0x7
#define MEGAMUX_LP_GPIO_12_6                       0x6
#define MEGAMUX_LP_GPIO_12_9                       0x9
#define MEGAMUX_LP_GPIO_12_8                       0x8
#define MEGAMUX_LP_GPIO_12_11                      0xb
#define MEGAMUX_LP_GPIO_12_10                      0xa
#define MEGAMUX_LP_GPIO_12_13                      0xd
#define MEGAMUX_LP_GPIO_12_12                      0xc
#define MEGAMUX_LP_GPIO_12_15                      0xf
#define MEGAMUX_LP_GPIO_12_14                      0xe
#define MEGAMUX_LP_GPIO_12_17                      0x11
#define MEGAMUX_LP_GPIO_12_16                      0x10
#define MEGAMUX_LP_GPIO_12_19                      0x13
#define MEGAMUX_LP_GPIO_12_18                      0x12
#define MEGAMUX_LP_GPIO_12_31                      0x1f
#define MEGAMUX_LP_GPIO_12_30                      0x1e
#define MEGAMUX_LP_GPIO_12_34                      0x22
#define MEGAMUX_LP_GPIO_12_33                      0x21
#define MEGAMUX_LP_GPIO_12_32                      0x20
/* MegaMux values for LP_GPIO_13 */
#define MEGAMUX_LP_GPIO_13_24                      0x18
#define MEGAMUX_LP_GPIO_13_25                      0x19
#define MEGAMUX_LP_GPIO_13_26                      0x1a
#define MEGAMUX_LP_GPIO_13_27                      0x1b
#define MEGAMUX_LP_GPIO_13_20                      0x14
#define MEGAMUX_LP_GPIO_13_21                      0x15
#define MEGAMUX_LP_GPIO_13_22                      0x16
#define MEGAMUX_LP_GPIO_13_23                      0x17
#define MEGAMUX_LP_GPIO_13_28                      0x1c
#define MEGAMUX_LP_GPIO_13_29                      0x1d
#define MEGAMUX_LP_GPIO_13_1                       0x1
#define MEGAMUX_LP_GPIO_13_0                       0x0
#define MEGAMUX_LP_GPIO_13_3                       0x3
#define MEGAMUX_LP_GPIO_13_2                       0x2
#define MEGAMUX_LP_GPIO_13_5                       0x5
#define MEGAMUX_LP_GPIO_13_4                       0x4
#define MEGAMUX_LP_GPIO_13_7                       0x7
#define MEGAMUX_LP_GPIO_13_6                       0x6
#define MEGAMUX_LP_GPIO_13_9                       0x9
#define MEGAMUX_LP_GPIO_13_8                       0x8
#define MEGAMUX_LP_GPIO_13_11                      0xb
#define MEGAMUX_LP_GPIO_13_10                      0xa
#define MEGAMUX_LP_GPIO_13_13                      0xd
#define MEGAMUX_LP_GPIO_13_12                      0xc
#define MEGAMUX_LP_GPIO_13_15                      0xf
#define MEGAMUX_LP_GPIO_13_14                      0xe
#define MEGAMUX_LP_GPIO_13_17                      0x11
#define MEGAMUX_LP_GPIO_13_16                      0x10
#define MEGAMUX_LP_GPIO_13_19                      0x13
#define MEGAMUX_LP_GPIO_13_18                      0x12
#define MEGAMUX_LP_GPIO_13_31                      0x1f
#define MEGAMUX_LP_GPIO_13_30                      0x1e
#define MEGAMUX_LP_GPIO_13_34                      0x22
#define MEGAMUX_LP_GPIO_13_33                      0x21
#define MEGAMUX_LP_GPIO_13_32                      0x20

/* PinMux macros for LP_GPIO_0 */
#define PINMUX_LP_GPIO_0_MUX6                     ( PIN_LP_GPIO_0<<16 | MUX_LP_GPIO_0_MUX6 )
#define PINMUX_LP_GPIO_0_MUX7                     ( PIN_LP_GPIO_0<<16 | MUX_LP_GPIO_0_MUX7 )
#define PINMUX_LP_GPIO_0_MUX4                     ( PIN_LP_GPIO_0<<16 | MUX_LP_GPIO_0_MUX4 )
#define PINMUX_LP_GPIO_0_MUX5                     ( PIN_LP_GPIO_0<<16 | MUX_LP_GPIO_0_MUX5 )
#define PINMUX_LP_GPIO_0_MUX2                     ( PIN_LP_GPIO_0<<16 | MUX_LP_GPIO_0_MUX2 )
#define PINMUX_LP_GPIO_0_MUX3                     ( PIN_LP_GPIO_0<<16 | MUX_LP_GPIO_0_MUX3 )
#define PINMUX_LP_GPIO_0_MUX0                     ( PIN_LP_GPIO_0<<16 | MUX_LP_GPIO_0_MUX0 )
#define PINMUX_LP_GPIO_0_MUX1                     ( PIN_LP_GPIO_0<<16 | MUX_LP_GPIO_0_MUX1 )
/* PinMux macros for LP_GPIO_1 */
#define PINMUX_LP_GPIO_1_MUX6                     ( PIN_LP_GPIO_1<<16 | MUX_LP_GPIO_1_MUX6 )
#define PINMUX_LP_GPIO_1_MUX7                     ( PIN_LP_GPIO_1<<16 | MUX_LP_GPIO_1_MUX7 )
#define PINMUX_LP_GPIO_1_MUX4                     ( PIN_LP_GPIO_1<<16 | MUX_LP_GPIO_1_MUX4 )
#define PINMUX_LP_GPIO_1_MUX5                     ( PIN_LP_GPIO_1<<16 | MUX_LP_GPIO_1_MUX5 )
#define PINMUX_LP_GPIO_1_MUX2                     ( PIN_LP_GPIO_1<<16 | MUX_LP_GPIO_1_MUX2 )
#define PINMUX_LP_GPIO_1_MUX3                     ( PIN_LP_GPIO_1<<16 | MUX_LP_GPIO_1_MUX3 )
#define PINMUX_LP_GPIO_1_MUX0                     ( PIN_LP_GPIO_1<<16 | MUX_LP_GPIO_1_MUX0 )
#define PINMUX_LP_GPIO_1_MUX1                     ( PIN_LP_GPIO_1<<16 | MUX_LP_GPIO_1_MUX1 )
/* PinMux macros for LP_GPIO_2 */
#define PINMUX_LP_GPIO_2_MUX6                     ( PIN_LP_GPIO_2<<16 | MUX_LP_GPIO_2_MUX6 )
#define PINMUX_LP_GPIO_2_MUX7                     ( PIN_LP_GPIO_2<<16 | MUX_LP_GPIO_2_MUX7 )
#define PINMUX_LP_GPIO_2_MUX4                     ( PIN_LP_GPIO_2<<16 | MUX_LP_GPIO_2_MUX4 )
#define PINMUX_LP_GPIO_2_MUX5                     ( PIN_LP_GPIO_2<<16 | MUX_LP_GPIO_2_MUX5 )
#define PINMUX_LP_GPIO_2_MUX2                     ( PIN_LP_GPIO_2<<16 | MUX_LP_GPIO_2_MUX2 )
#define PINMUX_LP_GPIO_2_MUX3                     ( PIN_LP_GPIO_2<<16 | MUX_LP_GPIO_2_MUX3 )
#define PINMUX_LP_GPIO_2_MUX0                     ( PIN_LP_GPIO_2<<16 | MUX_LP_GPIO_2_MUX0 )
#define PINMUX_LP_GPIO_2_MUX1                     ( PIN_LP_GPIO_2<<16 | MUX_LP_GPIO_2_MUX1 )
/* PinMux macros for LP_GPIO_3 */
#define PINMUX_LP_GPIO_3_MUX6                     ( PIN_LP_GPIO_3<<16 | MUX_LP_GPIO_3_MUX6 )
#define PINMUX_LP_GPIO_3_MUX7                     ( PIN_LP_GPIO_3<<16 | MUX_LP_GPIO_3_MUX7 )
#define PINMUX_LP_GPIO_3_MUX4                     ( PIN_LP_GPIO_3<<16 | MUX_LP_GPIO_3_MUX4 )
#define PINMUX_LP_GPIO_3_MUX5                     ( PIN_LP_GPIO_3<<16 | MUX_LP_GPIO_3_MUX5 )
#define PINMUX_LP_GPIO_3_MUX2                     ( PIN_LP_GPIO_3<<16 | MUX_LP_GPIO_3_MUX2 )
#define PINMUX_LP_GPIO_3_MUX3                     ( PIN_LP_GPIO_3<<16 | MUX_LP_GPIO_3_MUX3 )
#define PINMUX_LP_GPIO_3_MUX0                     ( PIN_LP_GPIO_3<<16 | MUX_LP_GPIO_3_MUX0 )
#define PINMUX_LP_GPIO_3_MUX1                     ( PIN_LP_GPIO_3<<16 | MUX_LP_GPIO_3_MUX1 )
/* PinMux macros for LP_GPIO_8 */
#define PINMUX_LP_GPIO_8_MUX6                     ( PIN_LP_GPIO_8<<16 | MUX_LP_GPIO_8_MUX6 )
#define PINMUX_LP_GPIO_8_MUX7                     ( PIN_LP_GPIO_8<<16 | MUX_LP_GPIO_8_MUX7 )
#define PINMUX_LP_GPIO_8_MUX4                     ( PIN_LP_GPIO_8<<16 | MUX_LP_GPIO_8_MUX4 )
#define PINMUX_LP_GPIO_8_MUX5                     ( PIN_LP_GPIO_8<<16 | MUX_LP_GPIO_8_MUX5 )
#define PINMUX_LP_GPIO_8_MUX2                     ( PIN_LP_GPIO_8<<16 | MUX_LP_GPIO_8_MUX2 )
#define PINMUX_LP_GPIO_8_MUX3                     ( PIN_LP_GPIO_8<<16 | MUX_LP_GPIO_8_MUX3 )
#define PINMUX_LP_GPIO_8_MUX0                     ( PIN_LP_GPIO_8<<16 | MUX_LP_GPIO_8_MUX0 )
#define PINMUX_LP_GPIO_8_MUX1                     ( PIN_LP_GPIO_8<<16 | MUX_LP_GPIO_8_MUX1 )
/* PinMux macros for LP_GPIO_9 */
#define PINMUX_LP_GPIO_9_MUX6                     ( PIN_LP_GPIO_9<<16 | MUX_LP_GPIO_9_MUX6 )
#define PINMUX_LP_GPIO_9_MUX7                     ( PIN_LP_GPIO_9<<16 | MUX_LP_GPIO_9_MUX7 )
#define PINMUX_LP_GPIO_9_MUX4                     ( PIN_LP_GPIO_9<<16 | MUX_LP_GPIO_9_MUX4 )
#define PINMUX_LP_GPIO_9_MUX5                     ( PIN_LP_GPIO_9<<16 | MUX_LP_GPIO_9_MUX5 )
#define PINMUX_LP_GPIO_9_MUX2                     ( PIN_LP_GPIO_9<<16 | MUX_LP_GPIO_9_MUX2 )
#define PINMUX_LP_GPIO_9_MUX3                     ( PIN_LP_GPIO_9<<16 | MUX_LP_GPIO_9_MUX3 )
#define PINMUX_LP_GPIO_9_MUX0                     ( PIN_LP_GPIO_9<<16 | MUX_LP_GPIO_9_MUX0 )
#define PINMUX_LP_GPIO_9_MUX1                     ( PIN_LP_GPIO_9<<16 | MUX_LP_GPIO_9_MUX1 )
/* PinMux macros for LP_GPIO_10 */
#define PINMUX_LP_GPIO_10_MUX6                    ( PIN_LP_GPIO_10<<16 | MUX_LP_GPIO_10_MUX6 )
#define PINMUX_LP_GPIO_10_MUX7                    ( PIN_LP_GPIO_10<<16 | MUX_LP_GPIO_10_MUX7 )
#define PINMUX_LP_GPIO_10_MUX4                    ( PIN_LP_GPIO_10<<16 | MUX_LP_GPIO_10_MUX4 )
#define PINMUX_LP_GPIO_10_MUX5                    ( PIN_LP_GPIO_10<<16 | MUX_LP_GPIO_10_MUX5 )
#define PINMUX_LP_GPIO_10_MUX2                    ( PIN_LP_GPIO_10<<16 | MUX_LP_GPIO_10_MUX2 )
#define PINMUX_LP_GPIO_10_MUX3                    ( PIN_LP_GPIO_10<<16 | MUX_LP_GPIO_10_MUX3 )
#define PINMUX_LP_GPIO_10_MUX0                    ( PIN_LP_GPIO_10<<16 | MUX_LP_GPIO_10_MUX0 )
#define PINMUX_LP_GPIO_10_MUX1                    ( PIN_LP_GPIO_10<<16 | MUX_LP_GPIO_10_MUX1 )
/* PinMux macros for LP_GPIO_11 */
#define PINMUX_LP_GPIO_11_MUX6                    ( PIN_LP_GPIO_11<<16 | MUX_LP_GPIO_11_MUX6 )
#define PINMUX_LP_GPIO_11_MUX7                    ( PIN_LP_GPIO_11<<16 | MUX_LP_GPIO_11_MUX7 )
#define PINMUX_LP_GPIO_11_MUX4                    ( PIN_LP_GPIO_11<<16 | MUX_LP_GPIO_11_MUX4 )
#define PINMUX_LP_GPIO_11_MUX5                    ( PIN_LP_GPIO_11<<16 | MUX_LP_GPIO_11_MUX5 )
#define PINMUX_LP_GPIO_11_MUX2                    ( PIN_LP_GPIO_11<<16 | MUX_LP_GPIO_11_MUX2 )
#define PINMUX_LP_GPIO_11_MUX3                    ( PIN_LP_GPIO_11<<16 | MUX_LP_GPIO_11_MUX3 )
#define PINMUX_LP_GPIO_11_MUX0                    ( PIN_LP_GPIO_11<<16 | MUX_LP_GPIO_11_MUX0 )
#define PINMUX_LP_GPIO_11_MUX1                    ( PIN_LP_GPIO_11<<16 | MUX_LP_GPIO_11_MUX1 )
/* PinMux macros for LP_GPIO_12 */
#define PINMUX_LP_GPIO_12_MUX6                    ( PIN_LP_GPIO_12<<16 | MUX_LP_GPIO_12_MUX6 )
#define PINMUX_LP_GPIO_12_MUX7                    ( PIN_LP_GPIO_12<<16 | MUX_LP_GPIO_12_MUX7 )
#define PINMUX_LP_GPIO_12_MUX4                    ( PIN_LP_GPIO_12<<16 | MUX_LP_GPIO_12_MUX4 )
#define PINMUX_LP_GPIO_12_MUX5                    ( PIN_LP_GPIO_12<<16 | MUX_LP_GPIO_12_MUX5 )
#define PINMUX_LP_GPIO_12_MUX2                    ( PIN_LP_GPIO_12<<16 | MUX_LP_GPIO_12_MUX2 )
#define PINMUX_LP_GPIO_12_MUX3                    ( PIN_LP_GPIO_12<<16 | MUX_LP_GPIO_12_MUX3 )
#define PINMUX_LP_GPIO_12_MUX0                    ( PIN_LP_GPIO_12<<16 | MUX_LP_GPIO_12_MUX0 )
#define PINMUX_LP_GPIO_12_MUX1                    ( PIN_LP_GPIO_12<<16 | MUX_LP_GPIO_12_MUX1 )
/* PinMux macros for LP_GPIO_13 */
#define PINMUX_LP_GPIO_13_MUX6                    ( PIN_LP_GPIO_13<<16 | MUX_LP_GPIO_13_MUX6 )
#define PINMUX_LP_GPIO_13_MUX7                    ( PIN_LP_GPIO_13<<16 | MUX_LP_GPIO_13_MUX7 )
#define PINMUX_LP_GPIO_13_MUX4                    ( PIN_LP_GPIO_13<<16 | MUX_LP_GPIO_13_MUX4 )
#define PINMUX_LP_GPIO_13_MUX5                    ( PIN_LP_GPIO_13<<16 | MUX_LP_GPIO_13_MUX5 )
#define PINMUX_LP_GPIO_13_MUX2                    ( PIN_LP_GPIO_13<<16 | MUX_LP_GPIO_13_MUX2 )
#define PINMUX_LP_GPIO_13_MUX3                    ( PIN_LP_GPIO_13<<16 | MUX_LP_GPIO_13_MUX3 )
#define PINMUX_LP_GPIO_13_MUX0                    ( PIN_LP_GPIO_13<<16 | MUX_LP_GPIO_13_MUX0 )
#define PINMUX_LP_GPIO_13_MUX1                    ( PIN_LP_GPIO_13<<16 | MUX_LP_GPIO_13_MUX1 )

#endif /* _BTLC1000WLCSP_PIO_ */
