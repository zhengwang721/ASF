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

#ifndef _SAMD20J18A_PIO_
#define _SAMD20J18A_PIO_

#define PIN_PA00                           0  /**< \brief Pin Number for PA00 */
#define PORT_PA00                  (1u <<  0) /**< \brief PORT Mask  for PA00 */
#define PIN_PA01                           1  /**< \brief Pin Number for PA01 */
#define PORT_PA01                  (1u <<  1) /**< \brief PORT Mask  for PA01 */
#define PIN_PA02                           2  /**< \brief Pin Number for PA02 */
#define PORT_PA02                  (1u <<  2) /**< \brief PORT Mask  for PA02 */
#define PIN_PA03                           3  /**< \brief Pin Number for PA03 */
#define PORT_PA03                  (1u <<  3) /**< \brief PORT Mask  for PA03 */
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
#define PIN_PA26                          26  /**< \brief Pin Number for PA26 */
#define PORT_PA26                  (1u << 26) /**< \brief PORT Mask  for PA26 */
#define PIN_PA27                          27  /**< \brief Pin Number for PA27 */
#define PORT_PA27                  (1u << 27) /**< \brief PORT Mask  for PA27 */
#define PIN_PA28                          28  /**< \brief Pin Number for PA28 */
#define PORT_PA28                  (1u << 28) /**< \brief PORT Mask  for PA28 */
#define PIN_PA29                          29  /**< \brief Pin Number for PA29 */
#define PORT_PA29                  (1u << 29) /**< \brief PORT Mask  for PA29 */
#define PIN_PA30                          30  /**< \brief Pin Number for PA30 */
#define PORT_PA30                  (1u << 30) /**< \brief PORT Mask  for PA30 */
#define PIN_PA31                          31  /**< \brief Pin Number for PA31 */
#define PORT_PA31                  (1u << 31) /**< \brief PORT Mask  for PA31 */
#define PIN_PB00                          32  /**< \brief Pin Number for PB00 */
#define PORT_PB00                  (1u <<  0) /**< \brief PORT Mask  for PB00 */
#define PIN_PB01                          33  /**< \brief Pin Number for PB01 */
#define PORT_PB01                  (1u <<  1) /**< \brief PORT Mask  for PB01 */
#define PIN_PB02                          34  /**< \brief Pin Number for PB02 */
#define PORT_PB02                  (1u <<  2) /**< \brief PORT Mask  for PB02 */
#define PIN_PB03                          35  /**< \brief Pin Number for PB03 */
#define PORT_PB03                  (1u <<  3) /**< \brief PORT Mask  for PB03 */
#define PIN_PB04                          36  /**< \brief Pin Number for PB04 */
#define PORT_PB04                  (1u <<  4) /**< \brief PORT Mask  for PB04 */
#define PIN_PB05                          37  /**< \brief Pin Number for PB05 */
#define PORT_PB05                  (1u <<  5) /**< \brief PORT Mask  for PB05 */
#define PIN_PB06                          38  /**< \brief Pin Number for PB06 */
#define PORT_PB06                  (1u <<  6) /**< \brief PORT Mask  for PB06 */
#define PIN_PB07                          39  /**< \brief Pin Number for PB07 */
#define PORT_PB07                  (1u <<  7) /**< \brief PORT Mask  for PB07 */
#define PIN_PB08                          40  /**< \brief Pin Number for PB08 */
#define PORT_PB08                  (1u <<  8) /**< \brief PORT Mask  for PB08 */
#define PIN_PB09                          41  /**< \brief Pin Number for PB09 */
#define PORT_PB09                  (1u <<  9) /**< \brief PORT Mask  for PB09 */
#define PIN_PB10                          42  /**< \brief Pin Number for PB10 */
#define PORT_PB10                  (1u << 10) /**< \brief PORT Mask  for PB10 */
#define PIN_PB11                          43  /**< \brief Pin Number for PB11 */
#define PORT_PB11                  (1u << 11) /**< \brief PORT Mask  for PB11 */
#define PIN_PB12                          44  /**< \brief Pin Number for PB12 */
#define PORT_PB12                  (1u << 12) /**< \brief PORT Mask  for PB12 */
#define PIN_PB13                          45  /**< \brief Pin Number for PB13 */
#define PORT_PB13                  (1u << 13) /**< \brief PORT Mask  for PB13 */
#define PIN_PB16                          48  /**< \brief Pin Number for PB16 */
#define PORT_PB16                  (1u << 16) /**< \brief PORT Mask  for PB16 */
#define PIN_PB17                          49  /**< \brief Pin Number for PB17 */
#define PORT_PB17                  (1u << 17) /**< \brief PORT Mask  for PB17 */
#define PIN_PB18                          50  /**< \brief Pin Number for PB18 */
#define PORT_PB18                  (1u << 18) /**< \brief PORT Mask  for PB18 */
#define PIN_PB19                          51  /**< \brief Pin Number for PB19 */
#define PORT_PB19                  (1u << 19) /**< \brief PORT Mask  for PB19 */
#define PIN_PB20                          52  /**< \brief Pin Number for PB20 */
#define PORT_PB20                  (1u << 20) /**< \brief PORT Mask  for PB20 */
#define PIN_PB21                          53  /**< \brief Pin Number for PB21 */
#define PORT_PB21                  (1u << 21) /**< \brief PORT Mask  for PB21 */
#define PIN_PB22                          54  /**< \brief Pin Number for PB22 */
#define PORT_PB22                  (1u << 22) /**< \brief PORT Mask  for PB22 */
#define PIN_PB23                          55  /**< \brief Pin Number for PB23 */
#define PORT_PB23                  (1u << 23) /**< \brief PORT Mask  for PB23 */
#define PIN_PB24                          56  /**< \brief Pin Number for PB24 */
#define PORT_PB24                  (1u << 24) /**< \brief PORT Mask  for PB24 */
#define PIN_PB25                          57  /**< \brief Pin Number for PB25 */
#define PORT_PB25                  (1u << 25) /**< \brief PORT Mask  for PB25 */
#define PIN_PB26                          58  /**< \brief Pin Number for PB26 */
#define PORT_PB26                  (1u << 26) /**< \brief PORT Mask  for PB26 */
#define PIN_PB27                          59  /**< \brief Pin Number for PB27 */
#define PORT_PB27                  (1u << 27) /**< \brief PORT Mask  for PB27 */
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
#define PIN_PB20G_GCLK_IO0                52  /**< \brief GCLK signal: IO0 on PB20 mux G */
#define MUX_PB20G_GCLK_IO0                 6
#define PINMUX_PB20G_GCLK_IO0      ((PIN_PB20G_GCLK_IO0 << 16) | MUX_PB20G_GCLK_IO0)
#define PORT_PB20G_GCLK_IO0        (1u << 20)
#define PIN_PA17G_GCLK_IO1                17  /**< \brief GCLK signal: IO1 on PA17 mux G */
#define MUX_PA17G_GCLK_IO1                 6
#define PINMUX_PA17G_GCLK_IO1      ((PIN_PA17G_GCLK_IO1 << 16) | MUX_PA17G_GCLK_IO1)
#define PORT_PA17G_GCLK_IO1        (1u << 17)
#define PIN_PB21G_GCLK_IO1                53  /**< \brief GCLK signal: IO1 on PB21 mux G */
#define MUX_PB21G_GCLK_IO1                 6
#define PINMUX_PB21G_GCLK_IO1      ((PIN_PB21G_GCLK_IO1 << 16) | MUX_PB21G_GCLK_IO1)
#define PORT_PB21G_GCLK_IO1        (1u << 21)
#define PIN_PA18G_GCLK_IO2                18  /**< \brief GCLK signal: IO2 on PA18 mux G */
#define MUX_PA18G_GCLK_IO2                 6
#define PINMUX_PA18G_GCLK_IO2      ((PIN_PA18G_GCLK_IO2 << 16) | MUX_PA18G_GCLK_IO2)
#define PORT_PA18G_GCLK_IO2        (1u << 18)
#define PIN_PB22G_GCLK_IO2                54  /**< \brief GCLK signal: IO2 on PB22 mux G */
#define MUX_PB22G_GCLK_IO2                 6
#define PINMUX_PB22G_GCLK_IO2      ((PIN_PB22G_GCLK_IO2 << 16) | MUX_PB22G_GCLK_IO2)
#define PORT_PB22G_GCLK_IO2        (1u << 22)
#define PIN_PA19G_GCLK_IO3                19  /**< \brief GCLK signal: IO3 on PA19 mux G */
#define MUX_PA19G_GCLK_IO3                 6
#define PINMUX_PA19G_GCLK_IO3      ((PIN_PA19G_GCLK_IO3 << 16) | MUX_PA19G_GCLK_IO3)
#define PORT_PA19G_GCLK_IO3        (1u << 19)
#define PIN_PB23G_GCLK_IO3                55  /**< \brief GCLK signal: IO3 on PB23 mux G */
#define MUX_PB23G_GCLK_IO3                 6
#define PINMUX_PB23G_GCLK_IO3      ((PIN_PB23G_GCLK_IO3 << 16) | MUX_PB23G_GCLK_IO3)
#define PORT_PB23G_GCLK_IO3        (1u << 23)
#define PIN_PA24G_GCLK_IO4                24  /**< \brief GCLK signal: IO4 on PA24 mux G */
#define MUX_PA24G_GCLK_IO4                 6
#define PINMUX_PA24G_GCLK_IO4      ((PIN_PA24G_GCLK_IO4 << 16) | MUX_PA24G_GCLK_IO4)
#define PORT_PA24G_GCLK_IO4        (1u << 24)
#define PIN_PB12G_GCLK_IO4                44  /**< \brief GCLK signal: IO4 on PB12 mux G */
#define MUX_PB12G_GCLK_IO4                 6
#define PINMUX_PB12G_GCLK_IO4      ((PIN_PB12G_GCLK_IO4 << 16) | MUX_PB12G_GCLK_IO4)
#define PORT_PB12G_GCLK_IO4        (1u << 12)
#define PIN_PA25G_GCLK_IO5                25  /**< \brief GCLK signal: IO5 on PA25 mux G */
#define MUX_PA25G_GCLK_IO5                 6
#define PINMUX_PA25G_GCLK_IO5      ((PIN_PA25G_GCLK_IO5 << 16) | MUX_PA25G_GCLK_IO5)
#define PORT_PA25G_GCLK_IO5        (1u << 25)
#define PIN_PB13G_GCLK_IO5                45  /**< \brief GCLK signal: IO5 on PB13 mux G */
#define MUX_PB13G_GCLK_IO5                 6
#define PINMUX_PB13G_GCLK_IO5      ((PIN_PB13G_GCLK_IO5 << 16) | MUX_PB13G_GCLK_IO5)
#define PORT_PB13G_GCLK_IO5        (1u << 13)
#define PIN_PA26G_GCLK_IO6                26  /**< \brief GCLK signal: IO6 on PA26 mux G */
#define MUX_PA26G_GCLK_IO6                 6
#define PINMUX_PA26G_GCLK_IO6      ((PIN_PA26G_GCLK_IO6 << 16) | MUX_PA26G_GCLK_IO6)
#define PORT_PA26G_GCLK_IO6        (1u << 26)
#define PIN_PB16G_GCLK_IO6                48  /**< \brief GCLK signal: IO6 on PB16 mux G */
#define MUX_PB16G_GCLK_IO6                 6
#define PINMUX_PB16G_GCLK_IO6      ((PIN_PB16G_GCLK_IO6 << 16) | MUX_PB16G_GCLK_IO6)
#define PORT_PB16G_GCLK_IO6        (1u << 16)
#define PIN_PA27G_GCLK_IO7                27  /**< \brief GCLK signal: IO7 on PA27 mux G */
#define MUX_PA27G_GCLK_IO7                 6
#define PINMUX_PA27G_GCLK_IO7      ((PIN_PA27G_GCLK_IO7 << 16) | MUX_PA27G_GCLK_IO7)
#define PORT_PA27G_GCLK_IO7        (1u << 27)
#define PIN_PB17G_GCLK_IO7                49  /**< \brief GCLK signal: IO7 on PB17 mux G */
#define MUX_PB17G_GCLK_IO7                 6
#define PINMUX_PB17G_GCLK_IO7      ((PIN_PB17G_GCLK_IO7 << 16) | MUX_PB17G_GCLK_IO7)
#define PORT_PB17G_GCLK_IO7        (1u << 17)
/* ========== PORT definition for EIC peripheral ========== */
#define PIN_PA00A_EIC_EXTINT0              0  /**< \brief EIC signal: EXTINT0 on PA00 mux A */
#define MUX_PA00A_EIC_EXTINT0              0
#define PINMUX_PA00A_EIC_EXTINT0   ((PIN_PA00A_EIC_EXTINT0 << 16) | MUX_PA00A_EIC_EXTINT0)
#define PORT_PA00A_EIC_EXTINT0     (1u <<  0)
#define PIN_PB00A_EIC_EXTINT0             32  /**< \brief EIC signal: EXTINT0 on PB00 mux A */
#define MUX_PB00A_EIC_EXTINT0              0
#define PINMUX_PB00A_EIC_EXTINT0   ((PIN_PB00A_EIC_EXTINT0 << 16) | MUX_PB00A_EIC_EXTINT0)
#define PORT_PB00A_EIC_EXTINT0     (1u <<  0)
#define PIN_PB20A_EIC_EXTINT0             52  /**< \brief EIC signal: EXTINT0 on PB20 mux A */
#define MUX_PB20A_EIC_EXTINT0              0
#define PINMUX_PB20A_EIC_EXTINT0   ((PIN_PB20A_EIC_EXTINT0 << 16) | MUX_PB20A_EIC_EXTINT0)
#define PORT_PB20A_EIC_EXTINT0     (1u << 20)
#define PIN_PA01A_EIC_EXTINT1              1  /**< \brief EIC signal: EXTINT1 on PA01 mux A */
#define MUX_PA01A_EIC_EXTINT1              0
#define PINMUX_PA01A_EIC_EXTINT1   ((PIN_PA01A_EIC_EXTINT1 << 16) | MUX_PA01A_EIC_EXTINT1)
#define PORT_PA01A_EIC_EXTINT1     (1u <<  1)
#define PIN_PB01A_EIC_EXTINT1             33  /**< \brief EIC signal: EXTINT1 on PB01 mux A */
#define MUX_PB01A_EIC_EXTINT1              0
#define PINMUX_PB01A_EIC_EXTINT1   ((PIN_PB01A_EIC_EXTINT1 << 16) | MUX_PB01A_EIC_EXTINT1)
#define PORT_PB01A_EIC_EXTINT1     (1u <<  1)
#define PIN_PB21A_EIC_EXTINT1             53  /**< \brief EIC signal: EXTINT1 on PB21 mux A */
#define MUX_PB21A_EIC_EXTINT1              0
#define PINMUX_PB21A_EIC_EXTINT1   ((PIN_PB21A_EIC_EXTINT1 << 16) | MUX_PB21A_EIC_EXTINT1)
#define PORT_PB21A_EIC_EXTINT1     (1u << 21)
#define PIN_PA04A_EIC_EXTINT2              4  /**< \brief EIC signal: EXTINT2 on PA04 mux A */
#define MUX_PA04A_EIC_EXTINT2              0
#define PINMUX_PA04A_EIC_EXTINT2   ((PIN_PA04A_EIC_EXTINT2 << 16) | MUX_PA04A_EIC_EXTINT2)
#define PORT_PA04A_EIC_EXTINT2     (1u <<  4)
#define PIN_PB04A_EIC_EXTINT2             36  /**< \brief EIC signal: EXTINT2 on PB04 mux A */
#define MUX_PB04A_EIC_EXTINT2              0
#define PINMUX_PB04A_EIC_EXTINT2   ((PIN_PB04A_EIC_EXTINT2 << 16) | MUX_PB04A_EIC_EXTINT2)
#define PORT_PB04A_EIC_EXTINT2     (1u <<  4)
#define PIN_PB22A_EIC_EXTINT2             54  /**< \brief EIC signal: EXTINT2 on PB22 mux A */
#define MUX_PB22A_EIC_EXTINT2              0
#define PINMUX_PB22A_EIC_EXTINT2   ((PIN_PB22A_EIC_EXTINT2 << 16) | MUX_PB22A_EIC_EXTINT2)
#define PORT_PB22A_EIC_EXTINT2     (1u << 22)
#define PIN_PA05A_EIC_EXTINT3              5  /**< \brief EIC signal: EXTINT3 on PA05 mux A */
#define MUX_PA05A_EIC_EXTINT3              0
#define PINMUX_PA05A_EIC_EXTINT3   ((PIN_PA05A_EIC_EXTINT3 << 16) | MUX_PA05A_EIC_EXTINT3)
#define PORT_PA05A_EIC_EXTINT3     (1u <<  5)
#define PIN_PB05A_EIC_EXTINT3             37  /**< \brief EIC signal: EXTINT3 on PB05 mux A */
#define MUX_PB05A_EIC_EXTINT3              0
#define PINMUX_PB05A_EIC_EXTINT3   ((PIN_PB05A_EIC_EXTINT3 << 16) | MUX_PB05A_EIC_EXTINT3)
#define PORT_PB05A_EIC_EXTINT3     (1u <<  5)
#define PIN_PB23A_EIC_EXTINT3             55  /**< \brief EIC signal: EXTINT3 on PB23 mux A */
#define MUX_PB23A_EIC_EXTINT3              0
#define PINMUX_PB23A_EIC_EXTINT3   ((PIN_PB23A_EIC_EXTINT3 << 16) | MUX_PB23A_EIC_EXTINT3)
#define PORT_PB23A_EIC_EXTINT3     (1u << 23)
#define PIN_PA08A_EIC_EXTINT4              8  /**< \brief EIC signal: EXTINT4 on PA08 mux A */
#define MUX_PA08A_EIC_EXTINT4              0
#define PINMUX_PA08A_EIC_EXTINT4   ((PIN_PA08A_EIC_EXTINT4 << 16) | MUX_PA08A_EIC_EXTINT4)
#define PORT_PA08A_EIC_EXTINT4     (1u <<  8)
#define PIN_PA28A_EIC_EXTINT4             28  /**< \brief EIC signal: EXTINT4 on PA28 mux A */
#define MUX_PA28A_EIC_EXTINT4              0
#define PINMUX_PA28A_EIC_EXTINT4   ((PIN_PA28A_EIC_EXTINT4 << 16) | MUX_PA28A_EIC_EXTINT4)
#define PORT_PA28A_EIC_EXTINT4     (1u << 28)
#define PIN_PB08A_EIC_EXTINT4             40  /**< \brief EIC signal: EXTINT4 on PB08 mux A */
#define MUX_PB08A_EIC_EXTINT4              0
#define PINMUX_PB08A_EIC_EXTINT4   ((PIN_PB08A_EIC_EXTINT4 << 16) | MUX_PB08A_EIC_EXTINT4)
#define PORT_PB08A_EIC_EXTINT4     (1u <<  8)
#define PIN_PA09A_EIC_EXTINT5              9  /**< \brief EIC signal: EXTINT5 on PA09 mux A */
#define MUX_PA09A_EIC_EXTINT5              0
#define PINMUX_PA09A_EIC_EXTINT5   ((PIN_PA09A_EIC_EXTINT5 << 16) | MUX_PA09A_EIC_EXTINT5)
#define PORT_PA09A_EIC_EXTINT5     (1u <<  9)
#define PIN_PA29A_EIC_EXTINT5             29  /**< \brief EIC signal: EXTINT5 on PA29 mux A */
#define MUX_PA29A_EIC_EXTINT5              0
#define PINMUX_PA29A_EIC_EXTINT5   ((PIN_PA29A_EIC_EXTINT5 << 16) | MUX_PA29A_EIC_EXTINT5)
#define PORT_PA29A_EIC_EXTINT5     (1u << 29)
#define PIN_PB09A_EIC_EXTINT5             41  /**< \brief EIC signal: EXTINT5 on PB09 mux A */
#define MUX_PB09A_EIC_EXTINT5              0
#define PINMUX_PB09A_EIC_EXTINT5   ((PIN_PB09A_EIC_EXTINT5 << 16) | MUX_PB09A_EIC_EXTINT5)
#define PORT_PB09A_EIC_EXTINT5     (1u <<  9)
#define PIN_PA10A_EIC_EXTINT6             10  /**< \brief EIC signal: EXTINT6 on PA10 mux A */
#define MUX_PA10A_EIC_EXTINT6              0
#define PINMUX_PA10A_EIC_EXTINT6   ((PIN_PA10A_EIC_EXTINT6 << 16) | MUX_PA10A_EIC_EXTINT6)
#define PORT_PA10A_EIC_EXTINT6     (1u << 10)
#define PIN_PB10A_EIC_EXTINT6             42  /**< \brief EIC signal: EXTINT6 on PB10 mux A */
#define MUX_PB10A_EIC_EXTINT6              0
#define PINMUX_PB10A_EIC_EXTINT6   ((PIN_PB10A_EIC_EXTINT6 << 16) | MUX_PB10A_EIC_EXTINT6)
#define PORT_PB10A_EIC_EXTINT6     (1u << 10)
#define PIN_PA11A_EIC_EXTINT7             11  /**< \brief EIC signal: EXTINT7 on PA11 mux A */
#define MUX_PA11A_EIC_EXTINT7              0
#define PINMUX_PA11A_EIC_EXTINT7   ((PIN_PA11A_EIC_EXTINT7 << 16) | MUX_PA11A_EIC_EXTINT7)
#define PORT_PA11A_EIC_EXTINT7     (1u << 11)
#define PIN_PA31A_EIC_EXTINT7             31  /**< \brief EIC signal: EXTINT7 on PA31 mux A */
#define MUX_PA31A_EIC_EXTINT7              0
#define PINMUX_PA31A_EIC_EXTINT7   ((PIN_PA31A_EIC_EXTINT7 << 16) | MUX_PA31A_EIC_EXTINT7)
#define PORT_PA31A_EIC_EXTINT7     (1u << 31)
#define PIN_PB11A_EIC_EXTINT7             43  /**< \brief EIC signal: EXTINT7 on PB11 mux A */
#define MUX_PB11A_EIC_EXTINT7              0
#define PINMUX_PB11A_EIC_EXTINT7   ((PIN_PB11A_EIC_EXTINT7 << 16) | MUX_PB11A_EIC_EXTINT7)
#define PORT_PB11A_EIC_EXTINT7     (1u << 11)
#define PIN_PA02A_EIC_EXTINT8              2  /**< \brief EIC signal: EXTINT8 on PA02 mux A */
#define MUX_PA02A_EIC_EXTINT8              0
#define PINMUX_PA02A_EIC_EXTINT8   ((PIN_PA02A_EIC_EXTINT8 << 16) | MUX_PA02A_EIC_EXTINT8)
#define PORT_PA02A_EIC_EXTINT8     (1u <<  2)
#define PIN_PA16A_EIC_EXTINT8             16  /**< \brief EIC signal: EXTINT8 on PA16 mux A */
#define MUX_PA16A_EIC_EXTINT8              0
#define PINMUX_PA16A_EIC_EXTINT8   ((PIN_PA16A_EIC_EXTINT8 << 16) | MUX_PA16A_EIC_EXTINT8)
#define PORT_PA16A_EIC_EXTINT8     (1u << 16)
#define PIN_PB02A_EIC_EXTINT8             34  /**< \brief EIC signal: EXTINT8 on PB02 mux A */
#define MUX_PB02A_EIC_EXTINT8              0
#define PINMUX_PB02A_EIC_EXTINT8   ((PIN_PB02A_EIC_EXTINT8 << 16) | MUX_PB02A_EIC_EXTINT8)
#define PORT_PB02A_EIC_EXTINT8     (1u <<  2)
#define PIN_PB16A_EIC_EXTINT8             48  /**< \brief EIC signal: EXTINT8 on PB16 mux A */
#define MUX_PB16A_EIC_EXTINT8              0
#define PINMUX_PB16A_EIC_EXTINT8   ((PIN_PB16A_EIC_EXTINT8 << 16) | MUX_PB16A_EIC_EXTINT8)
#define PORT_PB16A_EIC_EXTINT8     (1u << 16)
#define PIN_PA03A_EIC_EXTINT9              3  /**< \brief EIC signal: EXTINT9 on PA03 mux A */
#define MUX_PA03A_EIC_EXTINT9              0
#define PINMUX_PA03A_EIC_EXTINT9   ((PIN_PA03A_EIC_EXTINT9 << 16) | MUX_PA03A_EIC_EXTINT9)
#define PORT_PA03A_EIC_EXTINT9     (1u <<  3)
#define PIN_PA17A_EIC_EXTINT9             17  /**< \brief EIC signal: EXTINT9 on PA17 mux A */
#define MUX_PA17A_EIC_EXTINT9              0
#define PINMUX_PA17A_EIC_EXTINT9   ((PIN_PA17A_EIC_EXTINT9 << 16) | MUX_PA17A_EIC_EXTINT9)
#define PORT_PA17A_EIC_EXTINT9     (1u << 17)
#define PIN_PB03A_EIC_EXTINT9             35  /**< \brief EIC signal: EXTINT9 on PB03 mux A */
#define MUX_PB03A_EIC_EXTINT9              0
#define PINMUX_PB03A_EIC_EXTINT9   ((PIN_PB03A_EIC_EXTINT9 << 16) | MUX_PB03A_EIC_EXTINT9)
#define PORT_PB03A_EIC_EXTINT9     (1u <<  3)
#define PIN_PB17A_EIC_EXTINT9             49  /**< \brief EIC signal: EXTINT9 on PB17 mux A */
#define MUX_PB17A_EIC_EXTINT9              0
#define PINMUX_PB17A_EIC_EXTINT9   ((PIN_PB17A_EIC_EXTINT9 << 16) | MUX_PB17A_EIC_EXTINT9)
#define PORT_PB17A_EIC_EXTINT9     (1u << 17)
#define PIN_PA06A_EIC_EXTINT10             6  /**< \brief EIC signal: EXTINT10 on PA06 mux A */
#define MUX_PA06A_EIC_EXTINT10             0
#define PINMUX_PA06A_EIC_EXTINT10  ((PIN_PA06A_EIC_EXTINT10 << 16) | MUX_PA06A_EIC_EXTINT10)
#define PORT_PA06A_EIC_EXTINT10    (1u <<  6)
#define PIN_PA18A_EIC_EXTINT10            18  /**< \brief EIC signal: EXTINT10 on PA18 mux A */
#define MUX_PA18A_EIC_EXTINT10             0
#define PINMUX_PA18A_EIC_EXTINT10  ((PIN_PA18A_EIC_EXTINT10 << 16) | MUX_PA18A_EIC_EXTINT10)
#define PORT_PA18A_EIC_EXTINT10    (1u << 18)
#define PIN_PB06A_EIC_EXTINT10            38  /**< \brief EIC signal: EXTINT10 on PB06 mux A */
#define MUX_PB06A_EIC_EXTINT10             0
#define PINMUX_PB06A_EIC_EXTINT10  ((PIN_PB06A_EIC_EXTINT10 << 16) | MUX_PB06A_EIC_EXTINT10)
#define PORT_PB06A_EIC_EXTINT10    (1u <<  6)
#define PIN_PB18A_EIC_EXTINT10            50  /**< \brief EIC signal: EXTINT10 on PB18 mux A */
#define MUX_PB18A_EIC_EXTINT10             0
#define PINMUX_PB18A_EIC_EXTINT10  ((PIN_PB18A_EIC_EXTINT10 << 16) | MUX_PB18A_EIC_EXTINT10)
#define PORT_PB18A_EIC_EXTINT10    (1u << 18)
#define PIN_PA07A_EIC_EXTINT11             7  /**< \brief EIC signal: EXTINT11 on PA07 mux A */
#define MUX_PA07A_EIC_EXTINT11             0
#define PINMUX_PA07A_EIC_EXTINT11  ((PIN_PA07A_EIC_EXTINT11 << 16) | MUX_PA07A_EIC_EXTINT11)
#define PORT_PA07A_EIC_EXTINT11    (1u <<  7)
#define PIN_PA19A_EIC_EXTINT11            19  /**< \brief EIC signal: EXTINT11 on PA19 mux A */
#define MUX_PA19A_EIC_EXTINT11             0
#define PINMUX_PA19A_EIC_EXTINT11  ((PIN_PA19A_EIC_EXTINT11 << 16) | MUX_PA19A_EIC_EXTINT11)
#define PORT_PA19A_EIC_EXTINT11    (1u << 19)
#define PIN_PB07A_EIC_EXTINT11            39  /**< \brief EIC signal: EXTINT11 on PB07 mux A */
#define MUX_PB07A_EIC_EXTINT11             0
#define PINMUX_PB07A_EIC_EXTINT11  ((PIN_PB07A_EIC_EXTINT11 << 16) | MUX_PB07A_EIC_EXTINT11)
#define PORT_PB07A_EIC_EXTINT11    (1u <<  7)
#define PIN_PB19A_EIC_EXTINT11            51  /**< \brief EIC signal: EXTINT11 on PB19 mux A */
#define MUX_PB19A_EIC_EXTINT11             0
#define PINMUX_PB19A_EIC_EXTINT11  ((PIN_PB19A_EIC_EXTINT11 << 16) | MUX_PB19A_EIC_EXTINT11)
#define PORT_PB19A_EIC_EXTINT11    (1u << 19)
#define PIN_PA12A_EIC_EXTINT12            12  /**< \brief EIC signal: EXTINT12 on PA12 mux A */
#define MUX_PA12A_EIC_EXTINT12             0
#define PINMUX_PA12A_EIC_EXTINT12  ((PIN_PA12A_EIC_EXTINT12 << 16) | MUX_PA12A_EIC_EXTINT12)
#define PORT_PA12A_EIC_EXTINT12    (1u << 12)
#define PIN_PA24A_EIC_EXTINT12            24  /**< \brief EIC signal: EXTINT12 on PA24 mux A */
#define MUX_PA24A_EIC_EXTINT12             0
#define PINMUX_PA24A_EIC_EXTINT12  ((PIN_PA24A_EIC_EXTINT12 << 16) | MUX_PA24A_EIC_EXTINT12)
#define PORT_PA24A_EIC_EXTINT12    (1u << 24)
#define PIN_PB12A_EIC_EXTINT12            44  /**< \brief EIC signal: EXTINT12 on PB12 mux A */
#define MUX_PB12A_EIC_EXTINT12             0
#define PINMUX_PB12A_EIC_EXTINT12  ((PIN_PB12A_EIC_EXTINT12 << 16) | MUX_PB12A_EIC_EXTINT12)
#define PORT_PB12A_EIC_EXTINT12    (1u << 12)
#define PIN_PB24A_EIC_EXTINT12            56  /**< \brief EIC signal: EXTINT12 on PB24 mux A */
#define MUX_PB24A_EIC_EXTINT12             0
#define PINMUX_PB24A_EIC_EXTINT12  ((PIN_PB24A_EIC_EXTINT12 << 16) | MUX_PB24A_EIC_EXTINT12)
#define PORT_PB24A_EIC_EXTINT12    (1u << 24)
#define PIN_PA13A_EIC_EXTINT13            13  /**< \brief EIC signal: EXTINT13 on PA13 mux A */
#define MUX_PA13A_EIC_EXTINT13             0
#define PINMUX_PA13A_EIC_EXTINT13  ((PIN_PA13A_EIC_EXTINT13 << 16) | MUX_PA13A_EIC_EXTINT13)
#define PORT_PA13A_EIC_EXTINT13    (1u << 13)
#define PIN_PA25A_EIC_EXTINT13            25  /**< \brief EIC signal: EXTINT13 on PA25 mux A */
#define MUX_PA25A_EIC_EXTINT13             0
#define PINMUX_PA25A_EIC_EXTINT13  ((PIN_PA25A_EIC_EXTINT13 << 16) | MUX_PA25A_EIC_EXTINT13)
#define PORT_PA25A_EIC_EXTINT13    (1u << 25)
#define PIN_PB13A_EIC_EXTINT13            45  /**< \brief EIC signal: EXTINT13 on PB13 mux A */
#define MUX_PB13A_EIC_EXTINT13             0
#define PINMUX_PB13A_EIC_EXTINT13  ((PIN_PB13A_EIC_EXTINT13 << 16) | MUX_PB13A_EIC_EXTINT13)
#define PORT_PB13A_EIC_EXTINT13    (1u << 13)
#define PIN_PB25A_EIC_EXTINT13            57  /**< \brief EIC signal: EXTINT13 on PB25 mux A */
#define MUX_PB25A_EIC_EXTINT13             0
#define PINMUX_PB25A_EIC_EXTINT13  ((PIN_PB25A_EIC_EXTINT13 << 16) | MUX_PB25A_EIC_EXTINT13)
#define PORT_PB25A_EIC_EXTINT13    (1u << 25)
#define PIN_PA14A_EIC_EXTINT14            14  /**< \brief EIC signal: EXTINT14 on PA14 mux A */
#define MUX_PA14A_EIC_EXTINT14             0
#define PINMUX_PA14A_EIC_EXTINT14  ((PIN_PA14A_EIC_EXTINT14 << 16) | MUX_PA14A_EIC_EXTINT14)
#define PORT_PA14A_EIC_EXTINT14    (1u << 14)
#define PIN_PA26A_EIC_EXTINT14            26  /**< \brief EIC signal: EXTINT14 on PA26 mux A */
#define MUX_PA26A_EIC_EXTINT14             0
#define PINMUX_PA26A_EIC_EXTINT14  ((PIN_PA26A_EIC_EXTINT14 << 16) | MUX_PA26A_EIC_EXTINT14)
#define PORT_PA26A_EIC_EXTINT14    (1u << 26)
#define PIN_PB26A_EIC_EXTINT14            58  /**< \brief EIC signal: EXTINT14 on PB26 mux A */
#define MUX_PB26A_EIC_EXTINT14             0
#define PINMUX_PB26A_EIC_EXTINT14  ((PIN_PB26A_EIC_EXTINT14 << 16) | MUX_PB26A_EIC_EXTINT14)
#define PORT_PB26A_EIC_EXTINT14    (1u << 26)
#define PIN_PA15A_EIC_EXTINT15            15  /**< \brief EIC signal: EXTINT15 on PA15 mux A */
#define MUX_PA15A_EIC_EXTINT15             0
#define PINMUX_PA15A_EIC_EXTINT15  ((PIN_PA15A_EIC_EXTINT15 << 16) | MUX_PA15A_EIC_EXTINT15)
#define PORT_PA15A_EIC_EXTINT15    (1u << 15)
#define PIN_PA27A_EIC_EXTINT15            27  /**< \brief EIC signal: EXTINT15 on PA27 mux A */
#define MUX_PA27A_EIC_EXTINT15             0
#define PINMUX_PA27A_EIC_EXTINT15  ((PIN_PA27A_EIC_EXTINT15 << 16) | MUX_PA27A_EIC_EXTINT15)
#define PORT_PA27A_EIC_EXTINT15    (1u << 27)
#define PIN_PB27A_EIC_EXTINT15            59  /**< \brief EIC signal: EXTINT15 on PB27 mux A */
#define MUX_PB27A_EIC_EXTINT15             0
#define PINMUX_PB27A_EIC_EXTINT15  ((PIN_PB27A_EIC_EXTINT15 << 16) | MUX_PB27A_EIC_EXTINT15)
#define PORT_PB27A_EIC_EXTINT15    (1u << 27)
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
#define PIN_PA03C_SERCOM0_PAD1             3  /**< \brief SERCOM0 signal: PAD1 on PA03 mux C */
#define MUX_PA03C_SERCOM0_PAD1             2
#define PINMUX_PA03C_SERCOM0_PAD1  ((PIN_PA03C_SERCOM0_PAD1 << 16) | MUX_PA03C_SERCOM0_PAD1)
#define PORT_PA03C_SERCOM0_PAD1    (1u <<  3)
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
#define PIN_PA26B_SERCOM3_PAD2            26  /**< \brief SERCOM3 signal: PAD2 on PA26 mux B */
#define MUX_PA26B_SERCOM3_PAD2             1
#define PINMUX_PA26B_SERCOM3_PAD2  ((PIN_PA26B_SERCOM3_PAD2 << 16) | MUX_PA26B_SERCOM3_PAD2)
#define PORT_PA26B_SERCOM3_PAD2    (1u << 26)
#define PIN_PA17C_SERCOM3_PAD3            17  /**< \brief SERCOM3 signal: PAD3 on PA17 mux C */
#define MUX_PA17C_SERCOM3_PAD3             2
#define PINMUX_PA17C_SERCOM3_PAD3  ((PIN_PA17C_SERCOM3_PAD3 << 16) | MUX_PA17C_SERCOM3_PAD3)
#define PORT_PA17C_SERCOM3_PAD3    (1u << 17)
#define PIN_PA27B_SERCOM3_PAD3            27  /**< \brief SERCOM3 signal: PAD3 on PA27 mux B */
#define MUX_PA27B_SERCOM3_PAD3             1
#define PINMUX_PA27B_SERCOM3_PAD3  ((PIN_PA27B_SERCOM3_PAD3 << 16) | MUX_PA27B_SERCOM3_PAD3)
#define PORT_PA27B_SERCOM3_PAD3    (1u << 27)
/* ========== PORT definition for SERCOM4 peripheral ========== */
#define PIN_PA14C_SERCOM4_PAD0            14  /**< \brief SERCOM4 signal: PAD0 on PA14 mux C */
#define MUX_PA14C_SERCOM4_PAD0             2
#define PINMUX_PA14C_SERCOM4_PAD0  ((PIN_PA14C_SERCOM4_PAD0 << 16) | MUX_PA14C_SERCOM4_PAD0)
#define PORT_PA14C_SERCOM4_PAD0    (1u << 14)
#define PIN_PB00C_SERCOM4_PAD0            32  /**< \brief SERCOM4 signal: PAD0 on PB00 mux C */
#define MUX_PB00C_SERCOM4_PAD0             2
#define PINMUX_PB00C_SERCOM4_PAD0  ((PIN_PB00C_SERCOM4_PAD0 << 16) | MUX_PB00C_SERCOM4_PAD0)
#define PORT_PB00C_SERCOM4_PAD0    (1u <<  0)
#define PIN_PB08B_SERCOM4_PAD0            40  /**< \brief SERCOM4 signal: PAD0 on PB08 mux B */
#define MUX_PB08B_SERCOM4_PAD0             1
#define PINMUX_PB08B_SERCOM4_PAD0  ((PIN_PB08B_SERCOM4_PAD0 << 16) | MUX_PB08B_SERCOM4_PAD0)
#define PORT_PB08B_SERCOM4_PAD0    (1u <<  8)
#define PIN_PA15C_SERCOM4_PAD1            15  /**< \brief SERCOM4 signal: PAD1 on PA15 mux C */
#define MUX_PA15C_SERCOM4_PAD1             2
#define PINMUX_PA15C_SERCOM4_PAD1  ((PIN_PA15C_SERCOM4_PAD1 << 16) | MUX_PA15C_SERCOM4_PAD1)
#define PORT_PA15C_SERCOM4_PAD1    (1u << 15)
#define PIN_PB01C_SERCOM4_PAD1            33  /**< \brief SERCOM4 signal: PAD1 on PB01 mux C */
#define MUX_PB01C_SERCOM4_PAD1             2
#define PINMUX_PB01C_SERCOM4_PAD1  ((PIN_PB01C_SERCOM4_PAD1 << 16) | MUX_PB01C_SERCOM4_PAD1)
#define PORT_PB01C_SERCOM4_PAD1    (1u <<  1)
#define PIN_PB09B_SERCOM4_PAD1            41  /**< \brief SERCOM4 signal: PAD1 on PB09 mux B */
#define MUX_PB09B_SERCOM4_PAD1             1
#define PINMUX_PB09B_SERCOM4_PAD1  ((PIN_PB09B_SERCOM4_PAD1 << 16) | MUX_PB09B_SERCOM4_PAD1)
#define PORT_PB09B_SERCOM4_PAD1    (1u <<  9)
#define PIN_PB02C_SERCOM4_PAD2            34  /**< \brief SERCOM4 signal: PAD2 on PB02 mux C */
#define MUX_PB02C_SERCOM4_PAD2             2
#define PINMUX_PB02C_SERCOM4_PAD2  ((PIN_PB02C_SERCOM4_PAD2 << 16) | MUX_PB02C_SERCOM4_PAD2)
#define PORT_PB02C_SERCOM4_PAD2    (1u <<  2)
#define PIN_PB10B_SERCOM4_PAD2            42  /**< \brief SERCOM4 signal: PAD2 on PB10 mux B */
#define MUX_PB10B_SERCOM4_PAD2             1
#define PINMUX_PB10B_SERCOM4_PAD2  ((PIN_PB10B_SERCOM4_PAD2 << 16) | MUX_PB10B_SERCOM4_PAD2)
#define PORT_PB10B_SERCOM4_PAD2    (1u << 10)
#define PIN_PA12C_SERCOM4_PAD2            12  /**< \brief SERCOM4 signal: PAD2 on PA12 mux C */
#define MUX_PA12C_SERCOM4_PAD2             2
#define PINMUX_PA12C_SERCOM4_PAD2  ((PIN_PA12C_SERCOM4_PAD2 << 16) | MUX_PA12C_SERCOM4_PAD2)
#define PORT_PA12C_SERCOM4_PAD2    (1u << 12)
#define PIN_PA13C_SERCOM4_PAD3            13  /**< \brief SERCOM4 signal: PAD3 on PA13 mux C */
#define MUX_PA13C_SERCOM4_PAD3             2
#define PINMUX_PA13C_SERCOM4_PAD3  ((PIN_PA13C_SERCOM4_PAD3 << 16) | MUX_PA13C_SERCOM4_PAD3)
#define PORT_PA13C_SERCOM4_PAD3    (1u << 13)
#define PIN_PB03C_SERCOM4_PAD3            35  /**< \brief SERCOM4 signal: PAD3 on PB03 mux C */
#define MUX_PB03C_SERCOM4_PAD3             2
#define PINMUX_PB03C_SERCOM4_PAD3  ((PIN_PB03C_SERCOM4_PAD3 << 16) | MUX_PB03C_SERCOM4_PAD3)
#define PORT_PB03C_SERCOM4_PAD3    (1u <<  3)
#define PIN_PB11B_SERCOM4_PAD3            43  /**< \brief SERCOM4 signal: PAD3 on PB11 mux B */
#define MUX_PB11B_SERCOM4_PAD3             1
#define PINMUX_PB11B_SERCOM4_PAD3  ((PIN_PB11B_SERCOM4_PAD3 << 16) | MUX_PB11B_SERCOM4_PAD3)
#define PORT_PB11B_SERCOM4_PAD3    (1u << 11)
/* ========== PORT definition for SERCOM5 peripheral ========== */
#define PIN_PA24C_SERCOM5_PAD0            24  /**< \brief SERCOM5 signal: PAD0 on PA24 mux C */
#define MUX_PA24C_SERCOM5_PAD0             2
#define PINMUX_PA24C_SERCOM5_PAD0  ((PIN_PA24C_SERCOM5_PAD0 << 16) | MUX_PA24C_SERCOM5_PAD0)
#define PORT_PA24C_SERCOM5_PAD0    (1u << 24)
#define PIN_PB16C_SERCOM5_PAD0            48  /**< \brief SERCOM5 signal: PAD0 on PB16 mux C */
#define MUX_PB16C_SERCOM5_PAD0             2
#define PINMUX_PB16C_SERCOM5_PAD0  ((PIN_PB16C_SERCOM5_PAD0 << 16) | MUX_PB16C_SERCOM5_PAD0)
#define PORT_PB16C_SERCOM5_PAD0    (1u << 16)
#define PIN_PB20B_SERCOM5_PAD0            52  /**< \brief SERCOM5 signal: PAD0 on PB20 mux B */
#define MUX_PB20B_SERCOM5_PAD0             1
#define PINMUX_PB20B_SERCOM5_PAD0  ((PIN_PB20B_SERCOM5_PAD0 << 16) | MUX_PB20B_SERCOM5_PAD0)
#define PORT_PB20B_SERCOM5_PAD0    (1u << 20)
#define PIN_PA25C_SERCOM5_PAD1            25  /**< \brief SERCOM5 signal: PAD1 on PA25 mux C */
#define MUX_PA25C_SERCOM5_PAD1             2
#define PINMUX_PA25C_SERCOM5_PAD1  ((PIN_PA25C_SERCOM5_PAD1 << 16) | MUX_PA25C_SERCOM5_PAD1)
#define PORT_PA25C_SERCOM5_PAD1    (1u << 25)
#define PIN_PB17C_SERCOM5_PAD1            49  /**< \brief SERCOM5 signal: PAD1 on PB17 mux C */
#define MUX_PB17C_SERCOM5_PAD1             2
#define PINMUX_PB17C_SERCOM5_PAD1  ((PIN_PB17C_SERCOM5_PAD1 << 16) | MUX_PB17C_SERCOM5_PAD1)
#define PORT_PB17C_SERCOM5_PAD1    (1u << 17)
#define PIN_PB21B_SERCOM5_PAD1            53  /**< \brief SERCOM5 signal: PAD1 on PB21 mux B */
#define MUX_PB21B_SERCOM5_PAD1             1
#define PINMUX_PB21B_SERCOM5_PAD1  ((PIN_PB21B_SERCOM5_PAD1 << 16) | MUX_PB21B_SERCOM5_PAD1)
#define PORT_PB21B_SERCOM5_PAD1    (1u << 21)
#define PIN_PA26C_SERCOM5_PAD2            26  /**< \brief SERCOM5 signal: PAD2 on PA26 mux C */
#define MUX_PA26C_SERCOM5_PAD2             2
#define PINMUX_PA26C_SERCOM5_PAD2  ((PIN_PA26C_SERCOM5_PAD2 << 16) | MUX_PA26C_SERCOM5_PAD2)
#define PORT_PA26C_SERCOM5_PAD2    (1u << 26)
#define PIN_PB18C_SERCOM5_PAD2            50  /**< \brief SERCOM5 signal: PAD2 on PB18 mux C */
#define MUX_PB18C_SERCOM5_PAD2             2
#define PINMUX_PB18C_SERCOM5_PAD2  ((PIN_PB18C_SERCOM5_PAD2 << 16) | MUX_PB18C_SERCOM5_PAD2)
#define PORT_PB18C_SERCOM5_PAD2    (1u << 18)
#define PIN_PB22B_SERCOM5_PAD2            54  /**< \brief SERCOM5 signal: PAD2 on PB22 mux B */
#define MUX_PB22B_SERCOM5_PAD2             1
#define PINMUX_PB22B_SERCOM5_PAD2  ((PIN_PB22B_SERCOM5_PAD2 << 16) | MUX_PB22B_SERCOM5_PAD2)
#define PORT_PB22B_SERCOM5_PAD2    (1u << 22)
#define PIN_PA27C_SERCOM5_PAD3            27  /**< \brief SERCOM5 signal: PAD3 on PA27 mux C */
#define MUX_PA27C_SERCOM5_PAD3             2
#define PINMUX_PA27C_SERCOM5_PAD3  ((PIN_PA27C_SERCOM5_PAD3 << 16) | MUX_PA27C_SERCOM5_PAD3)
#define PORT_PA27C_SERCOM5_PAD3    (1u << 27)
#define PIN_PB19C_SERCOM5_PAD3            51  /**< \brief SERCOM5 signal: PAD3 on PB19 mux C */
#define MUX_PB19C_SERCOM5_PAD3             2
#define PINMUX_PB19C_SERCOM5_PAD3  ((PIN_PB19C_SERCOM5_PAD3 << 16) | MUX_PB19C_SERCOM5_PAD3)
#define PORT_PB19C_SERCOM5_PAD3    (1u << 19)
#define PIN_PB23B_SERCOM5_PAD3            55  /**< \brief SERCOM5 signal: PAD3 on PB23 mux B */
#define MUX_PB23B_SERCOM5_PAD3             1
#define PINMUX_PB23B_SERCOM5_PAD3  ((PIN_PB23B_SERCOM5_PAD3 << 16) | MUX_PB23B_SERCOM5_PAD3)
#define PORT_PB23B_SERCOM5_PAD3    (1u << 23)
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
#define PIN_PB08D_TC4_WO0                 40  /**< \brief TC4 signal: WO0 on PB08 mux D */
#define MUX_PB08D_TC4_WO0                  3
#define PINMUX_PB08D_TC4_WO0       ((PIN_PB08D_TC4_WO0 << 16) | MUX_PB08D_TC4_WO0)
#define PORT_PB08D_TC4_WO0         (1u <<  8)
#define PIN_PA24D_TC4_WO0                 24  /**< \brief TC4 signal: WO0 on PA24 mux D */
#define MUX_PA24D_TC4_WO0                  3
#define PINMUX_PA24D_TC4_WO0       ((PIN_PA24D_TC4_WO0 << 16) | MUX_PA24D_TC4_WO0)
#define PORT_PA24D_TC4_WO0         (1u << 24)
#define PIN_PB09D_TC4_WO1                 41  /**< \brief TC4 signal: WO1 on PB09 mux D */
#define MUX_PB09D_TC4_WO1                  3
#define PINMUX_PB09D_TC4_WO1       ((PIN_PB09D_TC4_WO1 << 16) | MUX_PB09D_TC4_WO1)
#define PORT_PB09D_TC4_WO1         (1u <<  9)
#define PIN_PA25D_TC4_WO1                 25  /**< \brief TC4 signal: WO1 on PA25 mux D */
#define MUX_PA25D_TC4_WO1                  3
#define PINMUX_PA25D_TC4_WO1       ((PIN_PA25D_TC4_WO1 << 16) | MUX_PA25D_TC4_WO1)
#define PORT_PA25D_TC4_WO1         (1u << 25)
/* ========== PORT definition for TC5 peripheral ========== */
#define PIN_PA26D_TC5_WO0                 26  /**< \brief TC5 signal: WO0 on PA26 mux D */
#define MUX_PA26D_TC5_WO0                  3
#define PINMUX_PA26D_TC5_WO0       ((PIN_PA26D_TC5_WO0 << 16) | MUX_PA26D_TC5_WO0)
#define PORT_PA26D_TC5_WO0         (1u << 26)
#define PIN_PB00D_TC5_WO0                 32  /**< \brief TC5 signal: WO0 on PB00 mux D */
#define MUX_PB00D_TC5_WO0                  3
#define PINMUX_PB00D_TC5_WO0       ((PIN_PB00D_TC5_WO0 << 16) | MUX_PB00D_TC5_WO0)
#define PORT_PB00D_TC5_WO0         (1u <<  0)
#define PIN_PB10D_TC5_WO0                 42  /**< \brief TC5 signal: WO0 on PB10 mux D */
#define MUX_PB10D_TC5_WO0                  3
#define PINMUX_PB10D_TC5_WO0       ((PIN_PB10D_TC5_WO0 << 16) | MUX_PB10D_TC5_WO0)
#define PORT_PB10D_TC5_WO0         (1u << 10)
#define PIN_PA27D_TC5_WO1                 27  /**< \brief TC5 signal: WO1 on PA27 mux D */
#define MUX_PA27D_TC5_WO1                  3
#define PINMUX_PA27D_TC5_WO1       ((PIN_PA27D_TC5_WO1 << 16) | MUX_PA27D_TC5_WO1)
#define PORT_PA27D_TC5_WO1         (1u << 27)
#define PIN_PB01D_TC5_WO1                 33  /**< \brief TC5 signal: WO1 on PB01 mux D */
#define MUX_PB01D_TC5_WO1                  3
#define PINMUX_PB01D_TC5_WO1       ((PIN_PB01D_TC5_WO1 << 16) | MUX_PB01D_TC5_WO1)
#define PORT_PB01D_TC5_WO1         (1u <<  1)
#define PIN_PB11D_TC5_WO1                 43  /**< \brief TC5 signal: WO1 on PB11 mux D */
#define MUX_PB11D_TC5_WO1                  3
#define PINMUX_PB11D_TC5_WO1       ((PIN_PB11D_TC5_WO1 << 16) | MUX_PB11D_TC5_WO1)
#define PORT_PB11D_TC5_WO1         (1u << 11)
/* ========== PORT definition for TC6 peripheral ========== */
#define PIN_PB16D_TC6_WO0                 48  /**< \brief TC6 signal: WO0 on PB16 mux D */
#define MUX_PB16D_TC6_WO0                  3
#define PINMUX_PB16D_TC6_WO0       ((PIN_PB16D_TC6_WO0 << 16) | MUX_PB16D_TC6_WO0)
#define PORT_PB16D_TC6_WO0         (1u << 16)
#define PIN_PB20D_TC6_WO0                 52  /**< \brief TC6 signal: WO0 on PB20 mux D */
#define MUX_PB20D_TC6_WO0                  3
#define PINMUX_PB20D_TC6_WO0       ((PIN_PB20D_TC6_WO0 << 16) | MUX_PB20D_TC6_WO0)
#define PORT_PB20D_TC6_WO0         (1u << 20)
#define PIN_PB17D_TC6_WO1                 49  /**< \brief TC6 signal: WO1 on PB17 mux D */
#define MUX_PB17D_TC6_WO1                  3
#define PINMUX_PB17D_TC6_WO1       ((PIN_PB17D_TC6_WO1 << 16) | MUX_PB17D_TC6_WO1)
#define PORT_PB17D_TC6_WO1         (1u << 17)
#define PIN_PB21D_TC6_WO1                 53  /**< \brief TC6 signal: WO1 on PB21 mux D */
#define MUX_PB21D_TC6_WO1                  3
#define PINMUX_PB21D_TC6_WO1       ((PIN_PB21D_TC6_WO1 << 16) | MUX_PB21D_TC6_WO1)
#define PORT_PB21D_TC6_WO1         (1u << 21)
/* ========== PORT definition for TC7 peripheral ========== */
#define PIN_PB18D_TC7_WO0                 50  /**< \brief TC7 signal: WO0 on PB18 mux D */
#define MUX_PB18D_TC7_WO0                  3
#define PINMUX_PB18D_TC7_WO0       ((PIN_PB18D_TC7_WO0 << 16) | MUX_PB18D_TC7_WO0)
#define PORT_PB18D_TC7_WO0         (1u << 18)
#define PIN_PB22D_TC7_WO0                 54  /**< \brief TC7 signal: WO0 on PB22 mux D */
#define MUX_PB22D_TC7_WO0                  3
#define PINMUX_PB22D_TC7_WO0       ((PIN_PB22D_TC7_WO0 << 16) | MUX_PB22D_TC7_WO0)
#define PORT_PB22D_TC7_WO0         (1u << 22)
#define PIN_PB19D_TC7_WO1                 51  /**< \brief TC7 signal: WO1 on PB19 mux D */
#define MUX_PB19D_TC7_WO1                  3
#define PINMUX_PB19D_TC7_WO1       ((PIN_PB19D_TC7_WO1 << 16) | MUX_PB19D_TC7_WO1)
#define PORT_PB19D_TC7_WO1         (1u << 19)
#define PIN_PB23D_TC7_WO1                 55  /**< \brief TC7 signal: WO1 on PB23 mux D */
#define MUX_PB23D_TC7_WO1                  3
#define PINMUX_PB23D_TC7_WO1       ((PIN_PB23D_TC7_WO1 << 16) | MUX_PB23D_TC7_WO1)
#define PORT_PB23D_TC7_WO1         (1u << 23)
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
#define PIN_PA03H_ADC_AIN3                 3  /**< \brief ADC signal: AIN3 on PA03 mux H */
#define MUX_PA03H_ADC_AIN3                 7
#define PINMUX_PA03H_ADC_AIN3      ((PIN_PA03H_ADC_AIN3 << 16) | MUX_PA03H_ADC_AIN3)
#define PORT_PA03H_ADC_AIN3        (1u <<  3)
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
#define PIN_PB00H_ADC_AIN8                32  /**< \brief ADC signal: AIN8 on PB00 mux H */
#define MUX_PB00H_ADC_AIN8                 7
#define PINMUX_PB00H_ADC_AIN8      ((PIN_PB00H_ADC_AIN8 << 16) | MUX_PB00H_ADC_AIN8)
#define PORT_PB00H_ADC_AIN8        (1u <<  0)
#define PIN_PB01H_ADC_AIN9                33  /**< \brief ADC signal: AIN9 on PB01 mux H */
#define MUX_PB01H_ADC_AIN9                 7
#define PINMUX_PB01H_ADC_AIN9      ((PIN_PB01H_ADC_AIN9 << 16) | MUX_PB01H_ADC_AIN9)
#define PORT_PB01H_ADC_AIN9        (1u <<  1)
#define PIN_PB02H_ADC_AIN10               34  /**< \brief ADC signal: AIN10 on PB02 mux H */
#define MUX_PB02H_ADC_AIN10                7
#define PINMUX_PB02H_ADC_AIN10     ((PIN_PB02H_ADC_AIN10 << 16) | MUX_PB02H_ADC_AIN10)
#define PORT_PB02H_ADC_AIN10       (1u <<  2)
#define PIN_PB03H_ADC_AIN11               35  /**< \brief ADC signal: AIN11 on PB03 mux H */
#define MUX_PB03H_ADC_AIN11                7
#define PINMUX_PB03H_ADC_AIN11     ((PIN_PB03H_ADC_AIN11 << 16) | MUX_PB03H_ADC_AIN11)
#define PORT_PB03H_ADC_AIN11       (1u <<  3)
#define PIN_PB04H_ADC_AIN12               36  /**< \brief ADC signal: AIN12 on PB04 mux H */
#define MUX_PB04H_ADC_AIN12                7
#define PINMUX_PB04H_ADC_AIN12     ((PIN_PB04H_ADC_AIN12 << 16) | MUX_PB04H_ADC_AIN12)
#define PORT_PB04H_ADC_AIN12       (1u <<  4)
#define PIN_PB05H_ADC_AIN13               37  /**< \brief ADC signal: AIN13 on PB05 mux H */
#define MUX_PB05H_ADC_AIN13                7
#define PINMUX_PB05H_ADC_AIN13     ((PIN_PB05H_ADC_AIN13 << 16) | MUX_PB05H_ADC_AIN13)
#define PORT_PB05H_ADC_AIN13       (1u <<  5)
#define PIN_PB06H_ADC_AIN14               38  /**< \brief ADC signal: AIN14 on PB06 mux H */
#define MUX_PB06H_ADC_AIN14                7
#define PINMUX_PB06H_ADC_AIN14     ((PIN_PB06H_ADC_AIN14 << 16) | MUX_PB06H_ADC_AIN14)
#define PORT_PB06H_ADC_AIN14       (1u <<  6)
#define PIN_PB07H_ADC_AIN15               39  /**< \brief ADC signal: AIN15 on PB07 mux H */
#define MUX_PB07H_ADC_AIN15                7
#define PINMUX_PB07H_ADC_AIN15     ((PIN_PB07H_ADC_AIN15 << 16) | MUX_PB07H_ADC_AIN15)
#define PORT_PB07H_ADC_AIN15       (1u <<  7)
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
#define PIN_PB20H_PTC_X8                  52  /**< \brief PTC signal: X8 on PB20 mux H */
#define MUX_PB20H_PTC_X8                   7
#define PINMUX_PB20H_PTC_X8        ((PIN_PB20H_PTC_X8 << 16) | MUX_PB20H_PTC_X8)
#define PORT_PB20H_PTC_X8          (1u << 20)
#define PIN_PB21H_PTC_X9                  53  /**< \brief PTC signal: X9 on PB21 mux H */
#define MUX_PB21H_PTC_X9                   7
#define PINMUX_PB21H_PTC_X9        ((PIN_PB21H_PTC_X9 << 16) | MUX_PB21H_PTC_X9)
#define PORT_PB21H_PTC_X9          (1u << 21)
#define PIN_PB22H_PTC_X10                 54  /**< \brief PTC signal: X10 on PB22 mux H */
#define MUX_PB22H_PTC_X10                  7
#define PINMUX_PB22H_PTC_X10       ((PIN_PB22H_PTC_X10 << 16) | MUX_PB22H_PTC_X10)
#define PORT_PB22H_PTC_X10         (1u << 22)
#define PIN_PB23H_PTC_X11                 55  /**< \brief PTC signal: X11 on PB23 mux H */
#define MUX_PB23H_PTC_X11                  7
#define PINMUX_PB23H_PTC_X11       ((PIN_PB23H_PTC_X11 << 16) | MUX_PB23H_PTC_X11)
#define PORT_PB23H_PTC_X11         (1u << 23)
#define PIN_PB24H_PTC_X12                 56  /**< \brief PTC signal: X12 on PB24 mux H */
#define MUX_PB24H_PTC_X12                  7
#define PINMUX_PB24H_PTC_X12       ((PIN_PB24H_PTC_X12 << 16) | MUX_PB24H_PTC_X12)
#define PORT_PB24H_PTC_X12         (1u << 24)
#define PIN_PB25H_PTC_X13                 57  /**< \brief PTC signal: X13 on PB25 mux H */
#define MUX_PB25H_PTC_X13                  7
#define PINMUX_PB25H_PTC_X13       ((PIN_PB25H_PTC_X13 << 16) | MUX_PB25H_PTC_X13)
#define PORT_PB25H_PTC_X13         (1u << 25)
#define PIN_PB26H_PTC_X14                 58  /**< \brief PTC signal: X14 on PB26 mux H */
#define MUX_PB26H_PTC_X14                  7
#define PINMUX_PB26H_PTC_X14       ((PIN_PB26H_PTC_X14 << 16) | MUX_PB26H_PTC_X14)
#define PORT_PB26H_PTC_X14         (1u << 26)
#define PIN_PB27H_PTC_X15                 59  /**< \brief PTC signal: X15 on PB27 mux H */
#define MUX_PB27H_PTC_X15                  7
#define PINMUX_PB27H_PTC_X15       ((PIN_PB27H_PTC_X15 << 16) | MUX_PB27H_PTC_X15)
#define PORT_PB27H_PTC_X15         (1u << 27)
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
#define PIN_PA03H_PTC_Y3                   3  /**< \brief PTC signal: Y3 on PA03 mux H */
#define MUX_PA03H_PTC_Y3                   7
#define PINMUX_PA03H_PTC_Y3        ((PIN_PA03H_PTC_Y3 << 16) | MUX_PA03H_PTC_Y3)
#define PORT_PA03H_PTC_Y3          (1u <<  3)
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
#define PIN_PB00H_PTC_Y8                  32  /**< \brief PTC signal: Y8 on PB00 mux H */
#define MUX_PB00H_PTC_Y8                   7
#define PINMUX_PB00H_PTC_Y8        ((PIN_PB00H_PTC_Y8 << 16) | MUX_PB00H_PTC_Y8)
#define PORT_PB00H_PTC_Y8          (1u <<  0)
#define PIN_PB01H_PTC_Y9                  33  /**< \brief PTC signal: Y9 on PB01 mux H */
#define MUX_PB01H_PTC_Y9                   7
#define PINMUX_PB01H_PTC_Y9        ((PIN_PB01H_PTC_Y9 << 16) | MUX_PB01H_PTC_Y9)
#define PORT_PB01H_PTC_Y9          (1u <<  1)
#define PIN_PB02H_PTC_Y10                 34  /**< \brief PTC signal: Y10 on PB02 mux H */
#define MUX_PB02H_PTC_Y10                  7
#define PINMUX_PB02H_PTC_Y10       ((PIN_PB02H_PTC_Y10 << 16) | MUX_PB02H_PTC_Y10)
#define PORT_PB02H_PTC_Y10         (1u <<  2)
#define PIN_PB03H_PTC_Y11                 35  /**< \brief PTC signal: Y11 on PB03 mux H */
#define MUX_PB03H_PTC_Y11                  7
#define PINMUX_PB03H_PTC_Y11       ((PIN_PB03H_PTC_Y11 << 16) | MUX_PB03H_PTC_Y11)
#define PORT_PB03H_PTC_Y11         (1u <<  3)
#define PIN_PB04H_PTC_Y12                 36  /**< \brief PTC signal: Y12 on PB04 mux H */
#define MUX_PB04H_PTC_Y12                  7
#define PINMUX_PB04H_PTC_Y12       ((PIN_PB04H_PTC_Y12 << 16) | MUX_PB04H_PTC_Y12)
#define PORT_PB04H_PTC_Y12         (1u <<  4)
#define PIN_PB05H_PTC_Y13                 37  /**< \brief PTC signal: Y13 on PB05 mux H */
#define MUX_PB05H_PTC_Y13                  7
#define PINMUX_PB05H_PTC_Y13       ((PIN_PB05H_PTC_Y13 << 16) | MUX_PB05H_PTC_Y13)
#define PORT_PB05H_PTC_Y13         (1u <<  5)
#define PIN_PB06H_PTC_Y14                 38  /**< \brief PTC signal: Y14 on PB06 mux H */
#define MUX_PB06H_PTC_Y14                  7
#define PINMUX_PB06H_PTC_Y14       ((PIN_PB06H_PTC_Y14 << 16) | MUX_PB06H_PTC_Y14)
#define PORT_PB06H_PTC_Y14         (1u <<  6)
#define PIN_PB07H_PTC_Y15                 39  /**< \brief PTC signal: Y15 on PB07 mux H */
#define MUX_PB07H_PTC_Y15                  7
#define PINMUX_PB07H_PTC_Y15       ((PIN_PB07H_PTC_Y15 << 16) | MUX_PB07H_PTC_Y15)
#define PORT_PB07H_PTC_Y15         (1u <<  7)

#endif /* _SAMD20J18A_PIO_ */
