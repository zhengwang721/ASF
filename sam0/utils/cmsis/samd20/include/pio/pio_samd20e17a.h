/**
 * \file
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#ifndef _SAMD20E17A_PIO_
#define _SAMD20E17A_PIO_

#define PIN_PA00                           0  /**< \brief Pin Number for PA00 */
#define PORT_PA00                  (1u <<  0) /**< \brief PORT Mask  for PA00 */
#define PIN_PA01                           1  /**< \brief Pin Number for PA01 */
#define PORT_PA01                  (1u <<  1) /**< \brief PORT Mask  for PA01 */
#define PIN_PA02                           2  /**< \brief Pin Number for PA02 */
#define PORT_PA02                  (1u <<  2) /**< \brief PORT Mask  for PA02 */
#define PIN_PA04                           4  /**< \brief Pin Number for PA04 */
#define PORT_PA04                  (1u <<  4) /**< \brief PORT Mask  for PA04 */
#define PIN_PA05                           5  /**< \brief Pin Number for PA05 */
#define PORT_PA05                  (1u <<  5) /**< \brief PORT Mask  for PA05 */
#define PIN_PA06                           6  /**< \brief Pin Number for PA06 */
#define PORT_PA06                  (1u <<  6) /**< \brief PORT Mask  for PA06 */
#define PIN_PA07                           7  /**< \brief Pin Number for PA07 */
#define PORT_PA07                  (1u <<  7) /**< \brief PORT Mask  for PA07 */
#define PIN_PA08                           8  /**< \brief Pin Number for PA08 */
#define PORT_PA08                  (1u <<  8) /**< \brief PORT Mask  for PA08 */
#define PIN_PA09                           9  /**< \brief Pin Number for PA09 */
#define PORT_PA09                  (1u <<  9) /**< \brief PORT Mask  for PA09 */
#define PIN_PA10                          10  /**< \brief Pin Number for PA10 */
#define PORT_PA10                  (1u << 10) /**< \brief PORT Mask  for PA10 */
#define PIN_PA11                          11  /**< \brief Pin Number for PA11 */
#define PORT_PA11                  (1u << 11) /**< \brief PORT Mask  for PA11 */
#define PIN_PA12                          12  /**< \brief Pin Number for PA12 */
#define PORT_PA12                  (1u << 12) /**< \brief PORT Mask  for PA12 */
#define PIN_PA13                          13  /**< \brief Pin Number for PA13 */
#define PORT_PA13                  (1u << 13) /**< \brief PORT Mask  for PA13 */
#define PIN_PA14                          14  /**< \brief Pin Number for PA14 */
#define PORT_PA14                  (1u << 14) /**< \brief PORT Mask  for PA14 */
#define PIN_PA15                          15  /**< \brief Pin Number for PA15 */
#define PORT_PA15                  (1u << 15) /**< \brief PORT Mask  for PA15 */
#define PIN_PA16                          16  /**< \brief Pin Number for PA16 */
#define PORT_PA16                  (1u << 16) /**< \brief PORT Mask  for PA16 */
#define PIN_PA17                          17  /**< \brief Pin Number for PA17 */
#define PORT_PA17                  (1u << 17) /**< \brief PORT Mask  for PA17 */
#define PIN_PA18                          18  /**< \brief Pin Number for PA18 */
#define PORT_PA18                  (1u << 18) /**< \brief PORT Mask  for PA18 */
#define PIN_PA19                          19  /**< \brief Pin Number for PA19 */
#define PORT_PA19                  (1u << 19) /**< \brief PORT Mask  for PA19 */
#define PIN_PA24                          24  /**< \brief Pin Number for PA24 */
#define PORT_PA24                  (1u << 24) /**< \brief PORT Mask  for PA24 */
#define PIN_PA25                          25  /**< \brief Pin Number for PA25 */
#define PORT_PA25                  (1u << 25) /**< \brief PORT Mask  for PA25 */
#define PIN_PA28                          28  /**< \brief Pin Number for PA28 */
#define PORT_PA28                  (1u << 28) /**< \brief PORT Mask  for PA28 */
#define PIN_PA29                          29  /**< \brief Pin Number for PA29 */
#define PORT_PA29                  (1u << 29) /**< \brief PORT Mask  for PA29 */
#define PIN_PA30                          30  /**< \brief Pin Number for PA30 */
#define PORT_PA30                  (1u << 30) /**< \brief PORT Mask  for PA30 */
#define PIN_PA31                          31  /**< \brief Pin Number for PA31 */
#define PORT_PA31                  (1u << 31) /**< \brief PORT Mask  for PA31 */
/* ========== PORT definition for CORE peripheral ========== */
#define PIN_PA28F_CORE_SWCLK              28  /**< \brief CORE signal: SWCLK on PA28 mux F */
#define MUX_PA28F_CORE_SWCLK               5
#define PINMUX_PA28F_CORE_SWCLK    ((PIN_PA28F_CORE_SWCLK << 16) | MUX_PA28F_CORE_SWCLK)
#define PORT_PA28F_CORE_SWCLK      (1u << 28)
/* ========== PORT definition for GCLK peripheral ========== */
#define PIN_PA16G_GCLK_IO0                16  /**< \brief GCLK signal: IO0 on PA16 mux G */
#define MUX_PA16G_GCLK_IO0                 6
#define PINMUX_PA16G_GCLK_IO0      ((PIN_PA16G_GCLK_IO0 << 16) | MUX_PA16G_GCLK_IO0)
#define PORT_PA16G_GCLK_IO0        (1u << 16)
#define PIN_PA28G_GCLK_IO0                28  /**< \brief GCLK signal: IO0 on PA28 mux G */
#define MUX_PA28G_GCLK_IO0                 6
#define PINMUX_PA28G_GCLK_IO0      ((PIN_PA28G_GCLK_IO0 << 16) | MUX_PA28G_GCLK_IO0)
#define PORT_PA28G_GCLK_IO0        (1u << 28)
#define PIN_PA31G_GCLK_IO0                31  /**< \brief GCLK signal: IO0 on PA31 mux G */
#define MUX_PA31G_GCLK_IO0                 6
#define PINMUX_PA31G_GCLK_IO0      ((PIN_PA31G_GCLK_IO0 << 16) | MUX_PA31G_GCLK_IO0)
#define PORT_PA31G_GCLK_IO0        (1u << 31)
#define PIN_PA17G_GCLK_IO1                17  /**< \brief GCLK signal: IO1 on PA17 mux G */
#define MUX_PA17G_GCLK_IO1                 6
#define PINMUX_PA17G_GCLK_IO1      ((PIN_PA17G_GCLK_IO1 << 16) | MUX_PA17G_GCLK_IO1)
#define PORT_PA17G_GCLK_IO1        (1u << 17)
#define PIN_PA18G_GCLK_IO2                18  /**< \brief GCLK signal: IO2 on PA18 mux G */
#define MUX_PA18G_GCLK_IO2                 6
#define PINMUX_PA18G_GCLK_IO2      ((PIN_PA18G_GCLK_IO2 << 16) | MUX_PA18G_GCLK_IO2)
#define PORT_PA18G_GCLK_IO2        (1u << 18)
#define PIN_PA19G_GCLK_IO3                19  /**< \brief GCLK signal: IO3 on PA19 mux G */
#define MUX_PA19G_GCLK_IO3                 6
#define PINMUX_PA19G_GCLK_IO3      ((PIN_PA19G_GCLK_IO3 << 16) | MUX_PA19G_GCLK_IO3)
#define PORT_PA19G_GCLK_IO3        (1u << 19)
#define PIN_PA24G_GCLK_IO4                24  /**< \brief GCLK signal: IO4 on PA24 mux G */
#define MUX_PA24G_GCLK_IO4                 6
#define PINMUX_PA24G_GCLK_IO4      ((PIN_PA24G_GCLK_IO4 << 16) | MUX_PA24G_GCLK_IO4)
#define PORT_PA24G_GCLK_IO4        (1u << 24)
#define PIN_PA25G_GCLK_IO5                25  /**< \brief GCLK signal: IO5 on PA25 mux G */
#define MUX_PA25G_GCLK_IO5                 6
#define PINMUX_PA25G_GCLK_IO5      ((PIN_PA25G_GCLK_IO5 << 16) | MUX_PA25G_GCLK_IO5)
#define PORT_PA25G_GCLK_IO5        (1u << 25)
/* ========== PORT definition for EIC peripheral ========== */
#define PIN_PA00A_EIC_EXTINT0              0  /**< \brief EIC signal: EXTINT0 on PA00 mux A */
#define MUX_PA00A_EIC_EXTINT0              0
#define PINMUX_PA00A_EIC_EXTINT0   ((PIN_PA00A_EIC_EXTINT0 << 16) | MUX_PA00A_EIC_EXTINT0)
#define PORT_PA00A_EIC_EXTINT0     (1u <<  0)
#define PIN_PA01A_EIC_EXTINT1              1  /**< \brief EIC signal: EXTINT1 on PA01 mux A */
#define MUX_PA01A_EIC_EXTINT1              0
#define PINMUX_PA01A_EIC_EXTINT1   ((PIN_PA01A_EIC_EXTINT1 << 16) | MUX_PA01A_EIC_EXTINT1)
#define PORT_PA01A_EIC_EXTINT1     (1u <<  1)
#define PIN_PA04A_EIC_EXTINT2              4  /**< \brief EIC signal: EXTINT2 on PA04 mux A */
#define MUX_PA04A_EIC_EXTINT2              0
#define PINMUX_PA04A_EIC_EXTINT2   ((PIN_PA04A_EIC_EXTINT2 << 16) | MUX_PA04A_EIC_EXTINT2)
#define PORT_PA04A_EIC_EXTINT2     (1u <<  4)
#define PIN_PA05A_EIC_EXTINT3              5  /**< \brief EIC signal: EXTINT3 on PA05 mux A */
#define MUX_PA05A_EIC_EXTINT3              0
#define PINMUX_PA05A_EIC_EXTINT3   ((PIN_PA05A_EIC_EXTINT3 << 16) | MUX_PA05A_EIC_EXTINT3)
#define PORT_PA05A_EIC_EXTINT3     (1u <<  5)
#define PIN_PA08A_EIC_EXTINT4              8  /**< \brief EIC signal: EXTINT4 on PA08 mux A */
#define MUX_PA08A_EIC_EXTINT4              0
#define PINMUX_PA08A_EIC_EXTINT4   ((PIN_PA08A_EIC_EXTINT4 << 16) | MUX_PA08A_EIC_EXTINT4)
#define PORT_PA08A_EIC_EXTINT4     (1u <<  8)
#define PIN_PA28A_EIC_EXTINT4             28  /**< \brief EIC signal: EXTINT4 on PA28 mux A */
#define MUX_PA28A_EIC_EXTINT4              0
#define PINMUX_PA28A_EIC_EXTINT4   ((PIN_PA28A_EIC_EXTINT4 << 16) | MUX_PA28A_EIC_EXTINT4)
#define PORT_PA28A_EIC_EXTINT4     (1u << 28)
#define PIN_PA09A_EIC_EXTINT5              9  /**< \brief EIC signal: EXTINT5 on PA09 mux A */
#define MUX_PA09A_EIC_EXTINT5              0
#define PINMUX_PA09A_EIC_EXTINT5   ((PIN_PA09A_EIC_EXTINT5 << 16) | MUX_PA09A_EIC_EXTINT5)
#define PORT_PA09A_EIC_EXTINT5     (1u <<  9)
#define PIN_PA29A_EIC_EXTINT5             29  /**< \brief EIC signal: EXTINT5 on PA29 mux A */
#define MUX_PA29A_EIC_EXTINT5              0
#define PINMUX_PA29A_EIC_EXTINT5   ((PIN_PA29A_EIC_EXTINT5 << 16) | MUX_PA29A_EIC_EXTINT5)
#define PORT_PA29A_EIC_EXTINT5     (1u << 29)
#define PIN_PA10A_EIC_EXTINT6             10  /**< \brief EIC signal: EXTINT6 on PA10 mux A */
#define MUX_PA10A_EIC_EXTINT6              0
#define PINMUX_PA10A_EIC_EXTINT6   ((PIN_PA10A_EIC_EXTINT6 << 16) | MUX_PA10A_EIC_EXTINT6)
#define PORT_PA10A_EIC_EXTINT6     (1u << 10)
#define PIN_PA11A_EIC_EXTINT7             11  /**< \brief EIC signal: EXTINT7 on PA11 mux A */
#define MUX_PA11A_EIC_EXTINT7              0
#define PINMUX_PA11A_EIC_EXTINT7   ((PIN_PA11A_EIC_EXTINT7 << 16) | MUX_PA11A_EIC_EXTINT7)
#define PORT_PA11A_EIC_EXTINT7     (1u << 11)
#define PIN_PA31A_EIC_EXTINT7             31  /**< \brief EIC signal: EXTINT7 on PA31 mux A */
#define MUX_PA31A_EIC_EXTINT7              0
#define PINMUX_PA31A_EIC_EXTINT7   ((PIN_PA31A_EIC_EXTINT7 << 16) | MUX_PA31A_EIC_EXTINT7)
#define PORT_PA31A_EIC_EXTINT7     (1u << 31)
#define PIN_PA02A_EIC_EXTINT8              2  /**< \brief EIC signal: EXTINT8 on PA02 mux A */
#define MUX_PA02A_EIC_EXTINT8              0
#define PINMUX_PA02A_EIC_EXTINT8   ((PIN_PA02A_EIC_EXTINT8 << 16) | MUX_PA02A_EIC_EXTINT8)
#define PORT_PA02A_EIC_EXTINT8     (1u <<  2)
#define PIN_PA16A_EIC_EXTINT8             16  /**< \brief EIC signal: EXTINT8 on PA16 mux A */
#define MUX_PA16A_EIC_EXTINT8              0
#define PINMUX_PA16A_EIC_EXTINT8   ((PIN_PA16A_EIC_EXTINT8 << 16) | MUX_PA16A_EIC_EXTINT8)
#define PORT_PA16A_EIC_EXTINT8     (1u << 16)
#define PIN_PA17A_EIC_EXTINT9             17  /**< \brief EIC signal: EXTINT9 on PA17 mux A */
#define MUX_PA17A_EIC_EXTINT9              0
#define PINMUX_PA17A_EIC_EXTINT9   ((PIN_PA17A_EIC_EXTINT9 << 16) | MUX_PA17A_EIC_EXTINT9)
#define PORT_PA17A_EIC_EXTINT9     (1u << 17)
#define PIN_PA06A_EIC_EXTINT10             6  /**< \brief EIC signal: EXTINT10 on PA06 mux A */
#define MUX_PA06A_EIC_EXTINT10             0
#define PINMUX_PA06A_EIC_EXTINT10  ((PIN_PA06A_EIC_EXTINT10 << 16) | MUX_PA06A_EIC_EXTINT10)
#define PORT_PA06A_EIC_EXTINT10    (1u <<  6)
#define PIN_PA18A_EIC_EXTINT10            18  /**< \brief EIC signal: EXTINT10 on PA18 mux A */
#define MUX_PA18A_EIC_EXTINT10             0
#define PINMUX_PA18A_EIC_EXTINT10  ((PIN_PA18A_EIC_EXTINT10 << 16) | MUX_PA18A_EIC_EXTINT10)
#define PORT_PA18A_EIC_EXTINT10    (1u << 18)
#define PIN_PA07A_EIC_EXTINT11             7  /**< \brief EIC signal: EXTINT11 on PA07 mux A */
#define MUX_PA07A_EIC_EXTINT11             0
#define PINMUX_PA07A_EIC_EXTINT11  ((PIN_PA07A_EIC_EXTINT11 << 16) | MUX_PA07A_EIC_EXTINT11)
#define PORT_PA07A_EIC_EXTINT11    (1u <<  7)
#define PIN_PA19A_EIC_EXTINT11            19  /**< \brief EIC signal: EXTINT11 on PA19 mux A */
#define MUX_PA19A_EIC_EXTINT11             0
#define PINMUX_PA19A_EIC_EXTINT11  ((PIN_PA19A_EIC_EXTINT11 << 16) | MUX_PA19A_EIC_EXTINT11)
#define PORT_PA19A_EIC_EXTINT11    (1u << 19)
#define PIN_PA12A_EIC_EXTINT12            12  /**< \brief EIC signal: EXTINT12 on PA12 mux A */
#define MUX_PA12A_EIC_EXTINT12             0
#define PINMUX_PA12A_EIC_EXTINT12  ((PIN_PA12A_EIC_EXTINT12 << 16) | MUX_PA12A_EIC_EXTINT12)
#define PORT_PA12A_EIC_EXTINT12    (1u << 12)
#define PIN_PA24A_EIC_EXTINT12            24  /**< \brief EIC signal: EXTINT12 on PA24 mux A */
#define MUX_PA24A_EIC_EXTINT12             0
#define PINMUX_PA24A_EIC_EXTINT12  ((PIN_PA24A_EIC_EXTINT12 << 16) | MUX_PA24A_EIC_EXTINT12)
#define PORT_PA24A_EIC_EXTINT12    (1u << 24)
#define PIN_PA13A_EIC_EXTINT13            13  /**< \brief EIC signal: EXTINT13 on PA13 mux A */
#define MUX_PA13A_EIC_EXTINT13             0
#define PINMUX_PA13A_EIC_EXTINT13  ((PIN_PA13A_EIC_EXTINT13 << 16) | MUX_PA13A_EIC_EXTINT13)
#define PORT_PA13A_EIC_EXTINT13    (1u << 13)
#define PIN_PA25A_EIC_EXTINT13            25  /**< \brief EIC signal: EXTINT13 on PA25 mux A */
#define MUX_PA25A_EIC_EXTINT13             0
#define PINMUX_PA25A_EIC_EXTINT13  ((PIN_PA25A_EIC_EXTINT13 << 16) | MUX_PA25A_EIC_EXTINT13)
#define PORT_PA25A_EIC_EXTINT13    (1u << 25)
#define PIN_PA14A_EIC_EXTINT14            14  /**< \brief EIC signal: EXTINT14 on PA14 mux A */
#define MUX_PA14A_EIC_EXTINT14             0
#define PINMUX_PA14A_EIC_EXTINT14  ((PIN_PA14A_EIC_EXTINT14 << 16) | MUX_PA14A_EIC_EXTINT14)
#define PORT_PA14A_EIC_EXTINT14    (1u << 14)
#define PIN_PA15A_EIC_EXTINT15            15  /**< \brief EIC signal: EXTINT15 on PA15 mux A */
#define MUX_PA15A_EIC_EXTINT15             0
#define PINMUX_PA15A_EIC_EXTINT15  ((PIN_PA15A_EIC_EXTINT15 << 16) | MUX_PA15A_EIC_EXTINT15)
#define PORT_PA15A_EIC_EXTINT15    (1u << 15)
#define PIN_PA30A_EIC_NMI                 30  /**< \brief EIC signal: NMI on PA30 mux A */
#define MUX_PA30A_EIC_NMI                  0
#define PINMUX_PA30A_EIC_NMI       ((PIN_PA30A_EIC_NMI << 16) | MUX_PA30A_EIC_NMI)
#define PORT_PA30A_EIC_NMI         (1u << 30)
/* ========== PORT definition for SERCOM0 peripheral ========== */
#define PIN_PA02C_SERCOM0_PAD0             2  /**< \brief SERCOM0 signal: PAD0 on PA02 mux C */
#define MUX_PA02C_SERCOM0_PAD0             2
#define PINMUX_PA02C_SERCOM0_PAD0  ((PIN_PA02C_SERCOM0_PAD0 << 16) | MUX_PA02C_SERCOM0_PAD0)
#define PORT_PA02C_SERCOM0_PAD0    (1u <<  2)
#define PIN_PA10B_SERCOM0_PAD0            10  /**< \brief SERCOM0 signal: PAD0 on PA10 mux B */
#define MUX_PA10B_SERCOM0_PAD0             1
#define PINMUX_PA10B_SERCOM0_PAD0  ((PIN_PA10B_SERCOM0_PAD0 << 16) | MUX_PA10B_SERCOM0_PAD0)
#define PORT_PA10B_SERCOM0_PAD0    (1u << 10)
#define PIN_PA11B_SERCOM0_PAD1            11  /**< \brief SERCOM0 signal: PAD1 on PA11 mux B */
#define MUX_PA11B_SERCOM0_PAD1             1
#define PINMUX_PA11B_SERCOM0_PAD1  ((PIN_PA11B_SERCOM0_PAD1 << 16) | MUX_PA11B_SERCOM0_PAD1)
#define PORT_PA11B_SERCOM0_PAD1    (1u << 11)
#define PIN_PA00C_SERCOM0_PAD2             0  /**< \brief SERCOM0 signal: PAD2 on PA00 mux C */
#define MUX_PA00C_SERCOM0_PAD2             2
#define PINMUX_PA00C_SERCOM0_PAD2  ((PIN_PA00C_SERCOM0_PAD2 << 16) | MUX_PA00C_SERCOM0_PAD2)
#define PORT_PA00C_SERCOM0_PAD2    (1u <<  0)
#define PIN_PA08B_SERCOM0_PAD2             8  /**< \brief SERCOM0 signal: PAD2 on PA08 mux B */
#define MUX_PA08B_SERCOM0_PAD2             1
#define PINMUX_PA08B_SERCOM0_PAD2  ((PIN_PA08B_SERCOM0_PAD2 << 16) | MUX_PA08B_SERCOM0_PAD2)
#define PORT_PA08B_SERCOM0_PAD2    (1u <<  8)
#define PIN_PA01C_SERCOM0_PAD3             1  /**< \brief SERCOM0 signal: PAD3 on PA01 mux C */
#define MUX_PA01C_SERCOM0_PAD3             2
#define PINMUX_PA01C_SERCOM0_PAD3  ((PIN_PA01C_SERCOM0_PAD3 << 16) | MUX_PA01C_SERCOM0_PAD3)
#define PORT_PA01C_SERCOM0_PAD3    (1u <<  1)
#define PIN_PA09B_SERCOM0_PAD3             9  /**< \brief SERCOM0 signal: PAD3 on PA09 mux B */
#define MUX_PA09B_SERCOM0_PAD3             1
#define PINMUX_PA09B_SERCOM0_PAD3  ((PIN_PA09B_SERCOM0_PAD3 << 16) | MUX_PA09B_SERCOM0_PAD3)
#define PORT_PA09B_SERCOM0_PAD3    (1u <<  9)
/* ========== PORT definition for SERCOM1 peripheral ========== */
#define PIN_PA30C_SERCOM1_PAD0            30  /**< \brief SERCOM1 signal: PAD0 on PA30 mux C */
#define MUX_PA30C_SERCOM1_PAD0             2
#define PINMUX_PA30C_SERCOM1_PAD0  ((PIN_PA30C_SERCOM1_PAD0 << 16) | MUX_PA30C_SERCOM1_PAD0)
#define PORT_PA30C_SERCOM1_PAD0    (1u << 30)
#define PIN_PA18B_SERCOM1_PAD0            18  /**< \brief SERCOM1 signal: PAD0 on PA18 mux B */
#define MUX_PA18B_SERCOM1_PAD0             1
#define PINMUX_PA18B_SERCOM1_PAD0  ((PIN_PA18B_SERCOM1_PAD0 << 16) | MUX_PA18B_SERCOM1_PAD0)
#define PORT_PA18B_SERCOM1_PAD0    (1u << 18)
#define PIN_PA31C_SERCOM1_PAD1            31  /**< \brief SERCOM1 signal: PAD1 on PA31 mux C */
#define MUX_PA31C_SERCOM1_PAD1             2
#define PINMUX_PA31C_SERCOM1_PAD1  ((PIN_PA31C_SERCOM1_PAD1 << 16) | MUX_PA31C_SERCOM1_PAD1)
#define PORT_PA31C_SERCOM1_PAD1    (1u << 31)
#define PIN_PA19B_SERCOM1_PAD1            19  /**< \brief SERCOM1 signal: PAD1 on PA19 mux B */
#define MUX_PA19B_SERCOM1_PAD1             1
#define PINMUX_PA19B_SERCOM1_PAD1  ((PIN_PA19B_SERCOM1_PAD1 << 16) | MUX_PA19B_SERCOM1_PAD1)
#define PORT_PA19B_SERCOM1_PAD1    (1u << 19)
#define PIN_PA04C_SERCOM1_PAD2             4  /**< \brief SERCOM1 signal: PAD2 on PA04 mux C */
#define MUX_PA04C_SERCOM1_PAD2             2
#define PINMUX_PA04C_SERCOM1_PAD2  ((PIN_PA04C_SERCOM1_PAD2 << 16) | MUX_PA04C_SERCOM1_PAD2)
#define PORT_PA04C_SERCOM1_PAD2    (1u <<  4)
#define PIN_PA16B_SERCOM1_PAD2            16  /**< \brief SERCOM1 signal: PAD2 on PA16 mux B */
#define MUX_PA16B_SERCOM1_PAD2             1
#define PINMUX_PA16B_SERCOM1_PAD2  ((PIN_PA16B_SERCOM1_PAD2 << 16) | MUX_PA16B_SERCOM1_PAD2)
#define PORT_PA16B_SERCOM1_PAD2    (1u << 16)
#define PIN_PA05C_SERCOM1_PAD3             5  /**< \brief SERCOM1 signal: PAD3 on PA05 mux C */
#define MUX_PA05C_SERCOM1_PAD3             2
#define PINMUX_PA05C_SERCOM1_PAD3  ((PIN_PA05C_SERCOM1_PAD3 << 16) | MUX_PA05C_SERCOM1_PAD3)
#define PORT_PA05C_SERCOM1_PAD3    (1u <<  5)
#define PIN_PA17B_SERCOM1_PAD3            17  /**< \brief SERCOM1 signal: PAD3 on PA17 mux B */
#define MUX_PA17B_SERCOM1_PAD3             1
#define PINMUX_PA17B_SERCOM1_PAD3  ((PIN_PA17B_SERCOM1_PAD3 << 16) | MUX_PA17B_SERCOM1_PAD3)
#define PORT_PA17B_SERCOM1_PAD3    (1u << 17)
/* ========== PORT definition for SERCOM2 peripheral ========== */
#define PIN_PA10C_SERCOM2_PAD0            10  /**< \brief SERCOM2 signal: PAD0 on PA10 mux C */
#define MUX_PA10C_SERCOM2_PAD0             2
#define PINMUX_PA10C_SERCOM2_PAD0  ((PIN_PA10C_SERCOM2_PAD0 << 16) | MUX_PA10C_SERCOM2_PAD0)
#define PORT_PA10C_SERCOM2_PAD0    (1u << 10)
#define PIN_PA14B_SERCOM2_PAD0            14  /**< \brief SERCOM2 signal: PAD0 on PA14 mux B */
#define MUX_PA14B_SERCOM2_PAD0             1
#define PINMUX_PA14B_SERCOM2_PAD0  ((PIN_PA14B_SERCOM2_PAD0 << 16) | MUX_PA14B_SERCOM2_PAD0)
#define PORT_PA14B_SERCOM2_PAD0    (1u << 14)
#define PIN_PA11C_SERCOM2_PAD1            11  /**< \brief SERCOM2 signal: PAD1 on PA11 mux C */
#define MUX_PA11C_SERCOM2_PAD1             2
#define PINMUX_PA11C_SERCOM2_PAD1  ((PIN_PA11C_SERCOM2_PAD1 << 16) | MUX_PA11C_SERCOM2_PAD1)
#define PORT_PA11C_SERCOM2_PAD1    (1u << 11)
#define PIN_PA15B_SERCOM2_PAD1            15  /**< \brief SERCOM2 signal: PAD1 on PA15 mux B */
#define MUX_PA15B_SERCOM2_PAD1             1
#define PINMUX_PA15B_SERCOM2_PAD1  ((PIN_PA15B_SERCOM2_PAD1 << 16) | MUX_PA15B_SERCOM2_PAD1)
#define PORT_PA15B_SERCOM2_PAD1    (1u << 15)
#define PIN_PA08C_SERCOM2_PAD2             8  /**< \brief SERCOM2 signal: PAD2 on PA08 mux C */
#define MUX_PA08C_SERCOM2_PAD2             2
#define PINMUX_PA08C_SERCOM2_PAD2  ((PIN_PA08C_SERCOM2_PAD2 << 16) | MUX_PA08C_SERCOM2_PAD2)
#define PORT_PA08C_SERCOM2_PAD2    (1u <<  8)
#define PIN_PA12B_SERCOM2_PAD2            12  /**< \brief SERCOM2 signal: PAD2 on PA12 mux B */
#define MUX_PA12B_SERCOM2_PAD2             1
#define PINMUX_PA12B_SERCOM2_PAD2  ((PIN_PA12B_SERCOM2_PAD2 << 16) | MUX_PA12B_SERCOM2_PAD2)
#define PORT_PA12B_SERCOM2_PAD2    (1u << 12)
#define PIN_PA09C_SERCOM2_PAD3             9  /**< \brief SERCOM2 signal: PAD3 on PA09 mux C */
#define MUX_PA09C_SERCOM2_PAD3             2
#define PINMUX_PA09C_SERCOM2_PAD3  ((PIN_PA09C_SERCOM2_PAD3 << 16) | MUX_PA09C_SERCOM2_PAD3)
#define PORT_PA09C_SERCOM2_PAD3    (1u <<  9)
#define PIN_PA13B_SERCOM2_PAD3            13  /**< \brief SERCOM2 signal: PAD3 on PA13 mux B */
#define MUX_PA13B_SERCOM2_PAD3             1
#define PINMUX_PA13B_SERCOM2_PAD3  ((PIN_PA13B_SERCOM2_PAD3 << 16) | MUX_PA13B_SERCOM2_PAD3)
#define PORT_PA13B_SERCOM2_PAD3    (1u << 13)
/* ========== PORT definition for SERCOM3 peripheral ========== */
#define PIN_PA18C_SERCOM3_PAD0            18  /**< \brief SERCOM3 signal: PAD0 on PA18 mux C */
#define MUX_PA18C_SERCOM3_PAD0             2
#define PINMUX_PA18C_SERCOM3_PAD0  ((PIN_PA18C_SERCOM3_PAD0 << 16) | MUX_PA18C_SERCOM3_PAD0)
#define PORT_PA18C_SERCOM3_PAD0    (1u << 18)
#define PIN_PA24B_SERCOM3_PAD0            24  /**< \brief SERCOM3 signal: PAD0 on PA24 mux B */
#define MUX_PA24B_SERCOM3_PAD0             1
#define PINMUX_PA24B_SERCOM3_PAD0  ((PIN_PA24B_SERCOM3_PAD0 << 16) | MUX_PA24B_SERCOM3_PAD0)
#define PORT_PA24B_SERCOM3_PAD0    (1u << 24)
#define PIN_PA19C_SERCOM3_PAD1            19  /**< \brief SERCOM3 signal: PAD1 on PA19 mux C */
#define MUX_PA19C_SERCOM3_PAD1             2
#define PINMUX_PA19C_SERCOM3_PAD1  ((PIN_PA19C_SERCOM3_PAD1 << 16) | MUX_PA19C_SERCOM3_PAD1)
#define PORT_PA19C_SERCOM3_PAD1    (1u << 19)
#define PIN_PA25B_SERCOM3_PAD1            25  /**< \brief SERCOM3 signal: PAD1 on PA25 mux B */
#define MUX_PA25B_SERCOM3_PAD1             1
#define PINMUX_PA25B_SERCOM3_PAD1  ((PIN_PA25B_SERCOM3_PAD1 << 16) | MUX_PA25B_SERCOM3_PAD1)
#define PORT_PA25B_SERCOM3_PAD1    (1u << 25)
#define PIN_PA16C_SERCOM3_PAD2            16  /**< \brief SERCOM3 signal: PAD2 on PA16 mux C */
#define MUX_PA16C_SERCOM3_PAD2             2
#define PINMUX_PA16C_SERCOM3_PAD2  ((PIN_PA16C_SERCOM3_PAD2 << 16) | MUX_PA16C_SERCOM3_PAD2)
#define PORT_PA16C_SERCOM3_PAD2    (1u << 16)
#define PIN_PA17C_SERCOM3_PAD3            17  /**< \brief SERCOM3 signal: PAD3 on PA17 mux C */
#define MUX_PA17C_SERCOM3_PAD3             2
#define PINMUX_PA17C_SERCOM3_PAD3  ((PIN_PA17C_SERCOM3_PAD3 << 16) | MUX_PA17C_SERCOM3_PAD3)
#define PORT_PA17C_SERCOM3_PAD3    (1u << 17)
/* ========== PORT definition for SERCOM4 peripheral ========== */
#define PIN_PA14C_SERCOM4_PAD0            14  /**< \brief SERCOM4 signal: PAD0 on PA14 mux C */
#define MUX_PA14C_SERCOM4_PAD0             2
#define PINMUX_PA14C_SERCOM4_PAD0  ((PIN_PA14C_SERCOM4_PAD0 << 16) | MUX_PA14C_SERCOM4_PAD0)
#define PORT_PA14C_SERCOM4_PAD0    (1u << 14)
#define PIN_PA15C_SERCOM4_PAD1            15  /**< \brief SERCOM4 signal: PAD1 on PA15 mux C */
#define MUX_PA15C_SERCOM4_PAD1             2
#define PINMUX_PA15C_SERCOM4_PAD1  ((PIN_PA15C_SERCOM4_PAD1 << 16) | MUX_PA15C_SERCOM4_PAD1)
#define PORT_PA15C_SERCOM4_PAD1    (1u << 15)
#define PIN_PA12C_SERCOM4_PAD2            12  /**< \brief SERCOM4 signal: PAD2 on PA12 mux C */
#define MUX_PA12C_SERCOM4_PAD2             2
#define PINMUX_PA12C_SERCOM4_PAD2  ((PIN_PA12C_SERCOM4_PAD2 << 16) | MUX_PA12C_SERCOM4_PAD2)
#define PORT_PA12C_SERCOM4_PAD2    (1u << 12)
#define PIN_PA13C_SERCOM4_PAD3            13  /**< \brief SERCOM4 signal: PAD3 on PA13 mux C */
#define MUX_PA13C_SERCOM4_PAD3             2
#define PINMUX_PA13C_SERCOM4_PAD3  ((PIN_PA13C_SERCOM4_PAD3 << 16) | MUX_PA13C_SERCOM4_PAD3)
#define PORT_PA13C_SERCOM4_PAD3    (1u << 13)
/* ========== PORT definition for SERCOM5 peripheral ========== */
#define PIN_PA24C_SERCOM5_PAD0            24  /**< \brief SERCOM5 signal: PAD0 on PA24 mux C */
#define MUX_PA24C_SERCOM5_PAD0             2
#define PINMUX_PA24C_SERCOM5_PAD0  ((PIN_PA24C_SERCOM5_PAD0 << 16) | MUX_PA24C_SERCOM5_PAD0)
#define PORT_PA24C_SERCOM5_PAD0    (1u << 24)
#define PIN_PA25C_SERCOM5_PAD1            25  /**< \brief SERCOM5 signal: PAD1 on PA25 mux C */
#define MUX_PA25C_SERCOM5_PAD1             2
#define PINMUX_PA25C_SERCOM5_PAD1  ((PIN_PA25C_SERCOM5_PAD1 << 16) | MUX_PA25C_SERCOM5_PAD1)
#define PORT_PA25C_SERCOM5_PAD1    (1u << 25)
/* ========== PORT definition for TC0 peripheral ========== */
#define PIN_PA00D_TC0_WO0                  0  /**< \brief TC0 signal: WO0 on PA00 mux D */
#define MUX_PA00D_TC0_WO0                  3
#define PINMUX_PA00D_TC0_WO0       ((PIN_PA00D_TC0_WO0 << 16) | MUX_PA00D_TC0_WO0)
#define PORT_PA00D_TC0_WO0         (1u <<  0)
#define PIN_PA08D_TC0_WO0                  8  /**< \brief TC0 signal: WO0 on PA08 mux D */
#define MUX_PA08D_TC0_WO0                  3
#define PINMUX_PA08D_TC0_WO0       ((PIN_PA08D_TC0_WO0 << 16) | MUX_PA08D_TC0_WO0)
#define PORT_PA08D_TC0_WO0         (1u <<  8)
#define PIN_PA01D_TC0_WO1                  1  /**< \brief TC0 signal: WO1 on PA01 mux D */
#define MUX_PA01D_TC0_WO1                  3
#define PINMUX_PA01D_TC0_WO1       ((PIN_PA01D_TC0_WO1 << 16) | MUX_PA01D_TC0_WO1)
#define PORT_PA01D_TC0_WO1         (1u <<  1)
#define PIN_PA09D_TC0_WO1                  9  /**< \brief TC0 signal: WO1 on PA09 mux D */
#define MUX_PA09D_TC0_WO1                  3
#define PINMUX_PA09D_TC0_WO1       ((PIN_PA09D_TC0_WO1 << 16) | MUX_PA09D_TC0_WO1)
#define PORT_PA09D_TC0_WO1         (1u <<  9)
/* ========== PORT definition for TC1 peripheral ========== */
#define PIN_PA04D_TC1_WO0                  4  /**< \brief TC1 signal: WO0 on PA04 mux D */
#define MUX_PA04D_TC1_WO0                  3
#define PINMUX_PA04D_TC1_WO0       ((PIN_PA04D_TC1_WO0 << 16) | MUX_PA04D_TC1_WO0)
#define PORT_PA04D_TC1_WO0         (1u <<  4)
#define PIN_PA10D_TC1_WO0                 10  /**< \brief TC1 signal: WO0 on PA10 mux D */
#define MUX_PA10D_TC1_WO0                  3
#define PINMUX_PA10D_TC1_WO0       ((PIN_PA10D_TC1_WO0 << 16) | MUX_PA10D_TC1_WO0)
#define PORT_PA10D_TC1_WO0         (1u << 10)
#define PIN_PA30D_TC1_WO0                 30  /**< \brief TC1 signal: WO0 on PA30 mux D */
#define MUX_PA30D_TC1_WO0                  3
#define PINMUX_PA30D_TC1_WO0       ((PIN_PA30D_TC1_WO0 << 16) | MUX_PA30D_TC1_WO0)
#define PORT_PA30D_TC1_WO0         (1u << 30)
#define PIN_PA05D_TC1_WO1                  5  /**< \brief TC1 signal: WO1 on PA05 mux D */
#define MUX_PA05D_TC1_WO1                  3
#define PINMUX_PA05D_TC1_WO1       ((PIN_PA05D_TC1_WO1 << 16) | MUX_PA05D_TC1_WO1)
#define PORT_PA05D_TC1_WO1         (1u <<  5)
#define PIN_PA11D_TC1_WO1                 11  /**< \brief TC1 signal: WO1 on PA11 mux D */
#define MUX_PA11D_TC1_WO1                  3
#define PINMUX_PA11D_TC1_WO1       ((PIN_PA11D_TC1_WO1 << 16) | MUX_PA11D_TC1_WO1)
#define PORT_PA11D_TC1_WO1         (1u << 11)
#define PIN_PA31D_TC1_WO1                 31  /**< \brief TC1 signal: WO1 on PA31 mux D */
#define MUX_PA31D_TC1_WO1                  3
#define PINMUX_PA31D_TC1_WO1       ((PIN_PA31D_TC1_WO1 << 16) | MUX_PA31D_TC1_WO1)
#define PORT_PA31D_TC1_WO1         (1u << 31)
/* ========== PORT definition for TC2 peripheral ========== */
#define PIN_PA16D_TC2_WO0                 16  /**< \brief TC2 signal: WO0 on PA16 mux D */
#define MUX_PA16D_TC2_WO0                  3
#define PINMUX_PA16D_TC2_WO0       ((PIN_PA16D_TC2_WO0 << 16) | MUX_PA16D_TC2_WO0)
#define PORT_PA16D_TC2_WO0         (1u << 16)
#define PIN_PA12D_TC2_WO0                 12  /**< \brief TC2 signal: WO0 on PA12 mux D */
#define MUX_PA12D_TC2_WO0                  3
#define PINMUX_PA12D_TC2_WO0       ((PIN_PA12D_TC2_WO0 << 16) | MUX_PA12D_TC2_WO0)
#define PORT_PA12D_TC2_WO0         (1u << 12)
#define PIN_PA17D_TC2_WO1                 17  /**< \brief TC2 signal: WO1 on PA17 mux D */
#define MUX_PA17D_TC2_WO1                  3
#define PINMUX_PA17D_TC2_WO1       ((PIN_PA17D_TC2_WO1 << 16) | MUX_PA17D_TC2_WO1)
#define PORT_PA17D_TC2_WO1         (1u << 17)
#define PIN_PA13D_TC2_WO1                 13  /**< \brief TC2 signal: WO1 on PA13 mux D */
#define MUX_PA13D_TC2_WO1                  3
#define PINMUX_PA13D_TC2_WO1       ((PIN_PA13D_TC2_WO1 << 16) | MUX_PA13D_TC2_WO1)
#define PORT_PA13D_TC2_WO1         (1u << 13)
/* ========== PORT definition for TC3 peripheral ========== */
#define PIN_PA14D_TC3_WO0                 14  /**< \brief TC3 signal: WO0 on PA14 mux D */
#define MUX_PA14D_TC3_WO0                  3
#define PINMUX_PA14D_TC3_WO0       ((PIN_PA14D_TC3_WO0 << 16) | MUX_PA14D_TC3_WO0)
#define PORT_PA14D_TC3_WO0         (1u << 14)
#define PIN_PA18D_TC3_WO0                 18  /**< \brief TC3 signal: WO0 on PA18 mux D */
#define MUX_PA18D_TC3_WO0                  3
#define PINMUX_PA18D_TC3_WO0       ((PIN_PA18D_TC3_WO0 << 16) | MUX_PA18D_TC3_WO0)
#define PORT_PA18D_TC3_WO0         (1u << 18)
#define PIN_PA15D_TC3_WO1                 15  /**< \brief TC3 signal: WO1 on PA15 mux D */
#define MUX_PA15D_TC3_WO1                  3
#define PINMUX_PA15D_TC3_WO1       ((PIN_PA15D_TC3_WO1 << 16) | MUX_PA15D_TC3_WO1)
#define PORT_PA15D_TC3_WO1         (1u << 15)
#define PIN_PA19D_TC3_WO1                 19  /**< \brief TC3 signal: WO1 on PA19 mux D */
#define MUX_PA19D_TC3_WO1                  3
#define PINMUX_PA19D_TC3_WO1       ((PIN_PA19D_TC3_WO1 << 16) | MUX_PA19D_TC3_WO1)
#define PORT_PA19D_TC3_WO1         (1u << 19)
/* ========== PORT definition for TC4 peripheral ========== */
#define PIN_PA24D_TC4_WO0                 24  /**< \brief TC4 signal: WO0 on PA24 mux D */
#define MUX_PA24D_TC4_WO0                  3
#define PINMUX_PA24D_TC4_WO0       ((PIN_PA24D_TC4_WO0 << 16) | MUX_PA24D_TC4_WO0)
#define PORT_PA24D_TC4_WO0         (1u << 24)
#define PIN_PA25D_TC4_WO1                 25  /**< \brief TC4 signal: WO1 on PA25 mux D */
#define MUX_PA25D_TC4_WO1                  3
#define PINMUX_PA25D_TC4_WO1       ((PIN_PA25D_TC4_WO1 << 16) | MUX_PA25D_TC4_WO1)
#define PORT_PA25D_TC4_WO1         (1u << 25)
/* ========== PORT definition for ADC peripheral ========== */
#define PIN_PA00H_ADC_AIN0                 0  /**< \brief ADC signal: AIN0 on PA00 mux H */
#define MUX_PA00H_ADC_AIN0                 7
#define PINMUX_PA00H_ADC_AIN0      ((PIN_PA00H_ADC_AIN0 << 16) | MUX_PA00H_ADC_AIN0)
#define PORT_PA00H_ADC_AIN0        (1u <<  0)
#define PIN_PA01H_ADC_AIN1                 1  /**< \brief ADC signal: AIN1 on PA01 mux H */
#define MUX_PA01H_ADC_AIN1                 7
#define PINMUX_PA01H_ADC_AIN1      ((PIN_PA01H_ADC_AIN1 << 16) | MUX_PA01H_ADC_AIN1)
#define PORT_PA01H_ADC_AIN1        (1u <<  1)
#define PIN_PA02H_ADC_AIN2                 2  /**< \brief ADC signal: AIN2 on PA02 mux H */
#define MUX_PA02H_ADC_AIN2                 7
#define PINMUX_PA02H_ADC_AIN2      ((PIN_PA02H_ADC_AIN2 << 16) | MUX_PA02H_ADC_AIN2)
#define PORT_PA02H_ADC_AIN2        (1u <<  2)
#define PIN_PA04H_ADC_AIN4                 4  /**< \brief ADC signal: AIN4 on PA04 mux H */
#define MUX_PA04H_ADC_AIN4                 7
#define PINMUX_PA04H_ADC_AIN4      ((PIN_PA04H_ADC_AIN4 << 16) | MUX_PA04H_ADC_AIN4)
#define PORT_PA04H_ADC_AIN4        (1u <<  4)
#define PIN_PA05H_ADC_AIN5                 5  /**< \brief ADC signal: AIN5 on PA05 mux H */
#define MUX_PA05H_ADC_AIN5                 7
#define PINMUX_PA05H_ADC_AIN5      ((PIN_PA05H_ADC_AIN5 << 16) | MUX_PA05H_ADC_AIN5)
#define PORT_PA05H_ADC_AIN5        (1u <<  5)
#define PIN_PA06H_ADC_AIN6                 6  /**< \brief ADC signal: AIN6 on PA06 mux H */
#define MUX_PA06H_ADC_AIN6                 7
#define PINMUX_PA06H_ADC_AIN6      ((PIN_PA06H_ADC_AIN6 << 16) | MUX_PA06H_ADC_AIN6)
#define PORT_PA06H_ADC_AIN6        (1u <<  6)
#define PIN_PA07H_ADC_AIN7                 7  /**< \brief ADC signal: AIN7 on PA07 mux H */
#define MUX_PA07H_ADC_AIN7                 7
#define PINMUX_PA07H_ADC_AIN7      ((PIN_PA07H_ADC_AIN7 << 16) | MUX_PA07H_ADC_AIN7)
#define PORT_PA07H_ADC_AIN7        (1u <<  7)
#define PIN_PA02H_ADC_VREFP                2  /**< \brief ADC signal: VREFP on PA02 mux H */
#define MUX_PA02H_ADC_VREFP                7
#define PINMUX_PA02H_ADC_VREFP     ((PIN_PA02H_ADC_VREFP << 16) | MUX_PA02H_ADC_VREFP)
#define PORT_PA02H_ADC_VREFP       (1u <<  2)
/* ========== PORT definition for AC peripheral ========== */
#define PIN_PA04H_AC_AIN0                  4  /**< \brief AC signal: AIN0 on PA04 mux H */
#define MUX_PA04H_AC_AIN0                  7
#define PINMUX_PA04H_AC_AIN0       ((PIN_PA04H_AC_AIN0 << 16) | MUX_PA04H_AC_AIN0)
#define PORT_PA04H_AC_AIN0         (1u <<  4)
#define PIN_PA05H_AC_AIN1                  5  /**< \brief AC signal: AIN1 on PA05 mux H */
#define MUX_PA05H_AC_AIN1                  7
#define PINMUX_PA05H_AC_AIN1       ((PIN_PA05H_AC_AIN1 << 16) | MUX_PA05H_AC_AIN1)
#define PORT_PA05H_AC_AIN1         (1u <<  5)
#define PIN_PA06H_AC_AIN2                  6  /**< \brief AC signal: AIN2 on PA06 mux H */
#define MUX_PA06H_AC_AIN2                  7
#define PINMUX_PA06H_AC_AIN2       ((PIN_PA06H_AC_AIN2 << 16) | MUX_PA06H_AC_AIN2)
#define PORT_PA06H_AC_AIN2         (1u <<  6)
#define PIN_PA07H_AC_AIN3                  7  /**< \brief AC signal: AIN3 on PA07 mux H */
#define MUX_PA07H_AC_AIN3                  7
#define PINMUX_PA07H_AC_AIN3       ((PIN_PA07H_AC_AIN3 << 16) | MUX_PA07H_AC_AIN3)
#define PORT_PA07H_AC_AIN3         (1u <<  7)
#define PIN_PA10G_AC_CMP0                 10  /**< \brief AC signal: CMP0 on PA10 mux G */
#define MUX_PA10G_AC_CMP0                  6
#define PINMUX_PA10G_AC_CMP0       ((PIN_PA10G_AC_CMP0 << 16) | MUX_PA10G_AC_CMP0)
#define PORT_PA10G_AC_CMP0         (1u << 10)
#define PIN_PA12G_AC_CMP0                 12  /**< \brief AC signal: CMP0 on PA12 mux G */
#define MUX_PA12G_AC_CMP0                  6
#define PINMUX_PA12G_AC_CMP0       ((PIN_PA12G_AC_CMP0 << 16) | MUX_PA12G_AC_CMP0)
#define PORT_PA12G_AC_CMP0         (1u << 12)
#define PIN_PA11G_AC_CMP1                 11  /**< \brief AC signal: CMP1 on PA11 mux G */
#define MUX_PA11G_AC_CMP1                  6
#define PINMUX_PA11G_AC_CMP1       ((PIN_PA11G_AC_CMP1 << 16) | MUX_PA11G_AC_CMP1)
#define PORT_PA11G_AC_CMP1         (1u << 11)
#define PIN_PA13G_AC_CMP1                 13  /**< \brief AC signal: CMP1 on PA13 mux G */
#define MUX_PA13G_AC_CMP1                  6
#define PINMUX_PA13G_AC_CMP1       ((PIN_PA13G_AC_CMP1 << 16) | MUX_PA13G_AC_CMP1)
#define PORT_PA13G_AC_CMP1         (1u << 13)
/* ========== PORT definition for DAC peripheral ========== */
#define PIN_PA00H_DAC_VOUT                 0  /**< \brief DAC signal: VOUT on PA00 mux H */
#define MUX_PA00H_DAC_VOUT                 7
#define PINMUX_PA00H_DAC_VOUT      ((PIN_PA00H_DAC_VOUT << 16) | MUX_PA00H_DAC_VOUT)
#define PORT_PA00H_DAC_VOUT        (1u <<  0)
#define PIN_PA01H_DAC_VREFP                1  /**< \brief DAC signal: VREFP on PA01 mux H */
#define MUX_PA01H_DAC_VREFP                7
#define PINMUX_PA01H_DAC_VREFP     ((PIN_PA01H_DAC_VREFP << 16) | MUX_PA01H_DAC_VREFP)
#define PORT_PA01H_DAC_VREFP       (1u <<  1)
/* ========== PORT definition for PTC peripheral ========== */
#define PIN_PA10H_PTC_X0                  10  /**< \brief PTC signal: X0 on PA10 mux H */
#define MUX_PA10H_PTC_X0                   7
#define PINMUX_PA10H_PTC_X0        ((PIN_PA10H_PTC_X0 << 16) | MUX_PA10H_PTC_X0)
#define PORT_PA10H_PTC_X0          (1u << 10)
#define PIN_PA11H_PTC_X1                  11  /**< \brief PTC signal: X1 on PA11 mux H */
#define MUX_PA11H_PTC_X1                   7
#define PINMUX_PA11H_PTC_X1        ((PIN_PA11H_PTC_X1 << 16) | MUX_PA11H_PTC_X1)
#define PORT_PA11H_PTC_X1          (1u << 11)
#define PIN_PA14H_PTC_X2                  14  /**< \brief PTC signal: X2 on PA14 mux H */
#define MUX_PA14H_PTC_X2                   7
#define PINMUX_PA14H_PTC_X2        ((PIN_PA14H_PTC_X2 << 16) | MUX_PA14H_PTC_X2)
#define PORT_PA14H_PTC_X2          (1u << 14)
#define PIN_PA15H_PTC_X3                  15  /**< \brief PTC signal: X3 on PA15 mux H */
#define MUX_PA15H_PTC_X3                   7
#define PINMUX_PA15H_PTC_X3        ((PIN_PA15H_PTC_X3 << 16) | MUX_PA15H_PTC_X3)
#define PORT_PA15H_PTC_X3          (1u << 15)
#define PIN_PA16H_PTC_X4                  16  /**< \brief PTC signal: X4 on PA16 mux H */
#define MUX_PA16H_PTC_X4                   7
#define PINMUX_PA16H_PTC_X4        ((PIN_PA16H_PTC_X4 << 16) | MUX_PA16H_PTC_X4)
#define PORT_PA16H_PTC_X4          (1u << 16)
#define PIN_PA17H_PTC_X5                  17  /**< \brief PTC signal: X5 on PA17 mux H */
#define MUX_PA17H_PTC_X5                   7
#define PINMUX_PA17H_PTC_X5        ((PIN_PA17H_PTC_X5 << 16) | MUX_PA17H_PTC_X5)
#define PORT_PA17H_PTC_X5          (1u << 17)
#define PIN_PA18H_PTC_X6                  18  /**< \brief PTC signal: X6 on PA18 mux H */
#define MUX_PA18H_PTC_X6                   7
#define PINMUX_PA18H_PTC_X6        ((PIN_PA18H_PTC_X6 << 16) | MUX_PA18H_PTC_X6)
#define PORT_PA18H_PTC_X6          (1u << 18)
#define PIN_PA19H_PTC_X7                  19  /**< \brief PTC signal: X7 on PA19 mux H */
#define MUX_PA19H_PTC_X7                   7
#define PINMUX_PA19H_PTC_X7        ((PIN_PA19H_PTC_X7 << 16) | MUX_PA19H_PTC_X7)
#define PORT_PA19H_PTC_X7          (1u << 19)
#define PIN_PA00H_PTC_Y0                   0  /**< \brief PTC signal: Y0 on PA00 mux H */
#define MUX_PA00H_PTC_Y0                   7
#define PINMUX_PA00H_PTC_Y0        ((PIN_PA00H_PTC_Y0 << 16) | MUX_PA00H_PTC_Y0)
#define PORT_PA00H_PTC_Y0          (1u <<  0)
#define PIN_PA01H_PTC_Y1                   1  /**< \brief PTC signal: Y1 on PA01 mux H */
#define MUX_PA01H_PTC_Y1                   7
#define PINMUX_PA01H_PTC_Y1        ((PIN_PA01H_PTC_Y1 << 16) | MUX_PA01H_PTC_Y1)
#define PORT_PA01H_PTC_Y1          (1u <<  1)
#define PIN_PA02H_PTC_Y2                   2  /**< \brief PTC signal: Y2 on PA02 mux H */
#define MUX_PA02H_PTC_Y2                   7
#define PINMUX_PA02H_PTC_Y2        ((PIN_PA02H_PTC_Y2 << 16) | MUX_PA02H_PTC_Y2)
#define PORT_PA02H_PTC_Y2          (1u <<  2)
#define PIN_PA04H_PTC_Y4                   4  /**< \brief PTC signal: Y4 on PA04 mux H */
#define MUX_PA04H_PTC_Y4                   7
#define PINMUX_PA04H_PTC_Y4        ((PIN_PA04H_PTC_Y4 << 16) | MUX_PA04H_PTC_Y4)
#define PORT_PA04H_PTC_Y4          (1u <<  4)
#define PIN_PA05H_PTC_Y5                   5  /**< \brief PTC signal: Y5 on PA05 mux H */
#define MUX_PA05H_PTC_Y5                   7
#define PINMUX_PA05H_PTC_Y5        ((PIN_PA05H_PTC_Y5 << 16) | MUX_PA05H_PTC_Y5)
#define PORT_PA05H_PTC_Y5          (1u <<  5)
#define PIN_PA06H_PTC_Y6                   6  /**< \brief PTC signal: Y6 on PA06 mux H */
#define MUX_PA06H_PTC_Y6                   7
#define PINMUX_PA06H_PTC_Y6        ((PIN_PA06H_PTC_Y6 << 16) | MUX_PA06H_PTC_Y6)
#define PORT_PA06H_PTC_Y6          (1u <<  6)
#define PIN_PA07H_PTC_Y7                   7  /**< \brief PTC signal: Y7 on PA07 mux H */
#define MUX_PA07H_PTC_Y7                   7
#define PINMUX_PA07H_PTC_Y7        ((PIN_PA07H_PTC_Y7 << 16) | MUX_PA07H_PTC_Y7)
#define PORT_PA07H_PTC_Y7          (1u <<  7)

#endif /* _SAMD20E17A_PIO_ */
