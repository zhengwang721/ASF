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

/* ========== GPIO definition for SPI0 peripheral ========== */
#define PIN_LP_GPIO_2_MUX5_SPI0_SCK       2L              /**< \brief SPI0 signal: SCK on LP_GPIO_2 mux 5 */
#define MUX_LP_GPIO_2_MUX5_SPI0_SCK       5L
#define PINMUX_LP_GPIO_2_MUX5_SPI0_SCK    ((PIN_LP_GPIO_2_MUX5_SPI0_SCK << 16) | MUX_LP_GPIO_2_MUX5_SPI0_SCK)

#define PIN_LP_GPIO_3_MUX5_SPI0_MOSI      3L              /**< \brief SPI0 signal: MOSI on LP_GPIO_3 mux 5 */
#define MUX_LP_GPIO_3_MUX5_SPI0_MOSI      5L
#define PINMUX_LP_GPIO_3_MUX5_SPI0_MOSI   ((PIN_LP_GPIO_3_MUX5_SPI0_MOSI << 16) | MUX_LP_GPIO_3_MUX5_SPI0_MOSI)

#define PIN_LP_GPIO_4_MUX5_SPI0_SSN       4L              /**< \brief SPI0 signal: SSN on LP_GPIO_4 mux 5 */
#define MUX_LP_GPIO_4_MUX5_SPI0_SSN       5L
#define PINMUX_LP_GPIO_4_MUX5_SPI0_SSN    ((PIN_LP_GPIO_4_MUX5_SPI0_SSN << 16) | MUX_LP_GPIO_4_MUX5_SPI0_SSN)

#define PIN_LP_GPIO_5_MUX5_SPI0_SCK       5L              /**< \brief SPI0 signal: SCK on LP_GPIO_5 mux 5 */
#define MUX_LP_GPIO_5_MUX5_SPI0_SCK       5L
#define PINMUX_LP_GPIO_5_MUX5_SPI0_SCK    ((PIN_LP_GPIO_5_MUX5_SPI0_SCK << 16) | MUX_LP_GPIO_5_MUX5_SPI0_SCK)

#define PIN_LP_GPIO_6_MUX5_SPI0_SCK       6L              /**< \brief SPI0 signal: SCK on LP_GPIO_6 mux 5 */
#define MUX_LP_GPIO_6_MUX5_SPI0_SCK       5L
#define PINMUX_LP_GPIO_6_MUX5_SPI0_SCK    ((PIN_LP_GPIO_6_MUX5_SPI0_SCK << 16) | MUX_LP_GPIO_6_MUX5_SPI0_SCK)

#define PIN_LP_GPIO_7_MUX5_SPI0_MOSI      7L              /**< \brief SPI0 signal: MOSI on LP_GPIO_7 mux 5 */
#define MUX_LP_GPIO_7_MUX5_SPI0_MOSI      5L
#define PINMUX_LP_GPIO_7_MUX5_SPI0_MOSI   ((PIN_LP_GPIO_7_MUX5_SPI0_MOSI << 16) | MUX_LP_GPIO_7_MUX5_SPI0_MOSI)

#define PIN_LP_GPIO_8_MUX5_SPI0_SSN       8L              /**< \brief SPI0 signal: SSN on LP_GPIO_8 mux 5 */
#define MUX_LP_GPIO_8_MUX5_SPI0_SSN       5L
#define PINMUX_LP_GPIO_8_MUX5_SPI0_SSN    ((PIN_LP_GPIO_8_MUX5_SPI0_SSN << 16) | MUX_LP_GPIO_8_MUX5_SPI0_SSN)

#define PIN_LP_GPIO_9_MUX5_SPI0_MISO      9L              /**< \brief SPI0 signal: MISO on LP_GPIO_9 mux 5 */
#define MUX_LP_GPIO_9_MUX5_SPI0_MISO      5L
#define PINMUX_LP_GPIO_9_MUX5_SPI0_MISO   ((PIN_LP_GPIO_9_MUX5_SPI0_MISO << 16) | MUX_LP_GPIO_9_MUX5_SPI0_MISO)

#define PIN_LP_GPIO_10_MUX2_SPI0_SCK      10L              /**< \brief SPI0 signal: SCK on LP_GPIO_10 mux 2 */
#define MUX_LP_GPIO_10_MUX2_SPI0_SCK      2L
#define PINMUX_LP_GPIO_10_MUX2_SPI0_SCK   ((PIN_LP_GPIO_10_MUX2_SPI0_SCK << 16) | MUX_LP_GPIO_10_MUX2_SPI0_SCK)

#define PIN_LP_GPIO_11_MUX2_SPI0_MOSI     11L              /**< \brief SPI0 signal: MOSI on LP_GPIO_11 mux 2 */
#define MUX_LP_GPIO_11_MUX2_SPI0_MOSI     2L
#define PINMUX_LP_GPIO_11_MUX2_SPI0_MOSI  ((PIN_LP_GPIO_11_MUX2_SPI0_MOSI << 16) | MUX_LP_GPIO_11_MUX2_SPI0_MOSI)

#define PIN_LP_GPIO_12_MUX2_SPI0_SSN      12L              /**< \brief SPI0 signal: SSN on LP_GPIO_12 mux 2 */
#define MUX_LP_GPIO_12_MUX2_SPI0_SSN      2L
#define PINMUX_LP_GPIO_12_MUX2_SPI0_SSN   ((PIN_LP_GPIO_12_MUX2_SPI0_SSN << 16) | MUX_LP_GPIO_12_MUX2_SPI0_SSN)

#define PIN_LP_GPIO_13_MUX2_SPI0_MISO     13L              /**< \brief SPI0 signal: MISO on LP_GPIO_13 mux 2 */
#define MUX_LP_GPIO_13_MUX2_SPI0_MISO     2L
#define PINMUX_LP_GPIO_13_MUX2_SPI0_MISO  ((PIN_LP_GPIO_13_MUX2_SPI0_MISO << 16) | MUX_LP_GPIO_13_MUX2_SPI0_MISO)

#define PIN_LP_GPIO_16_MUX5_SPI0_SCK       16L              /**< \brief SPI0 signal: SCK on LP_GPIO_16 mux 5 */
#define MUX_LP_GPIO_16_MUX5_SPI0_SCK       5L
#define PINMUX_LP_GPIO_16_MUX5_SPI0_SCK    ((PIN_LP_GPIO_16_MUX5_SPI0_SCK << 16) | MUX_LP_GPIO_16_MUX5_SPI0_SCK)

#define PIN_LP_GPIO_17_MUX5_SPI0_MOSI      17L              /**< \brief SPI0 signal: MOSI on LP_GPIO_17 mux 5 */
#define MUX_LP_GPIO_17_MUX5_SPI0_MOSI      5L
#define PINMUX_LP_GPIO_17_MUX5_SPI0_MOSI   ((PIN_LP_GPIO_17_MUX5_SPI0_MOSI << 16) | MUX_LP_GPIO_17_MUX5_SPI0_MOSI)

#define PIN_LP_GPIO_18_MUX5_SPI0_SSN       18L              /**< \brief SPI0 signal: SSN on LP_GPIO_18 mux 5 */
#define MUX_LP_GPIO_18_MUX5_SPI0_SSN       5L
#define PINMUX_LP_GPIO_18_MUX5_SPI0_SSN    ((PIN_LP_GPIO_18_MUX5_SPI0_SSN << 16) | MUX_LP_GPIO_18_MUX5_SPI0_SSN)

#define PIN_LP_GPIO_19_MUX5_SPI0_MISO      19L              /**< \brief SPI0 signal: MISO on LP_GPIO_19 mux 5 */
#define MUX_LP_GPIO_19_MUX5_SPI0_MISO      5L
#define PINMUX_LP_GPIO_19_MUX5_SPI0_MISO   ((PIN_LP_GPIO_19_MUX5_SPI0_MISO << 16) | MUX_LP_GPIO_19_MUX5_SPI0_MISO)

/* ========== GPIO definition for SPI1 peripheral ========== */
#define PIN_LP_GPIO_2_MUX4_SPI0_SCK       2L              /**< \brief SPI0 signal: SCK on LP_GPIO_2 mux 4 */
#define MUX_LP_GPIO_2_MUX4_SPI0_SCK       4L
#define PINMUX_LP_GPIO_2_MUX4_SPI0_SCK    ((PIN_LP_GPIO_2_MUX4_SPI0_SCK << 16) | MUX_LP_GPIO_2_MUX4_SPI0_SCK)

#define PIN_LP_GPIO_3_MUX4_SPI0_MOSI      3L              /**< \brief SPI0 signal: MOSI on LP_GPIO_3 mux 4 */
#define MUX_LP_GPIO_3_MUX4_SPI0_MOSI      4L
#define PINMUX_LP_GPIO_3_MUX4_SPI0_MOSI   ((PIN_LP_GPIO_3_MUX4_SPI0_MOSI << 16) | MUX_LP_GPIO_3_MUX4_SPI0_MOSI)

#define PIN_LP_GPIO_4_MUX4_SPI0_SSN       4L              /**< \brief SPI0 signal: SSN on LP_GPIO_4 mux 4 */
#define MUX_LP_GPIO_4_MUX4_SPI0_SSN       4L
#define PINMUX_LP_GPIO_4_MUX4_SPI0_SSN    ((PIN_LP_GPIO_4_MUX4_SPI0_SSN << 16) | MUX_LP_GPIO_4_MUX4_SPI0_SSN)

#define PIN_LP_GPIO_5_MUX4_SPI0_MISO      5L              /**< \brief SPI0 signal: SCK on LP_GPIO_5 mux 4 */
#define MUX_LP_GPIO_5_MUX4_SPI0_MISO      4L
#define PINMUX_LP_GPIO_5_MUX4_SPI0_MISO   ((PIN_LP_GPIO_5_MUX4_SPI0_MISO << 16) | MUX_LP_GPIO_5_MUX4_SPI0_MISO)

#define PIN_LP_GPIO_16_MUX4_SPI0_SCK      16L              /**< \brief SPI0 signal: SCK on LP_GPIO_16 mux 4 */
#define MUX_LP_GPIO_16_MUX4_SPI0_SCK      4L
#define PINMUX_LP_GPIO_16_MUX4_SPI0_SCK   ((PIN_LP_GPIO_16_MUX4_SPI0_SCK << 16) | MUX_LP_GPIO_16_MUX4_SPI0_SCK)

#define PIN_LP_GPIO_17_MUX4_SPI0_SSN      17L              /**< \brief SPI0 signal: SSN on LP_GPIO_17 mux 4 */
#define MUX_LP_GPIO_17_MUX4_SPI0_SSN      4L
#define PINMUX_LP_GPIO_17_MUX4_SPI0_SSN   ((PIN_LP_GPIO_17_MUX4_SPI0_SSN << 16) | MUX_LP_GPIO_17_MUX4_SPI0_SSN)

#define PIN_LP_GPIO_18_MUX4_SPI0_MISO     18L              /**< \brief SPI0 signal: MISO on LP_GPIO_18 mux 4 */
#define MUX_LP_GPIO_18_MUX4_SPI0_MISO     4L
#define PINMUX_LP_GPIO_18_MUX4_SPI0_MISO  ((PIN_LP_GPIO_18_MUX4_SPI0_MISO << 16) | MUX_LP_GPIO_18_MUX4_SPI0_MISO)

#define PIN_LP_GPIO_19_MUX4_SPI0_MOSI     19L              /**< \brief SPI0 signal: MOSI on LP_GPIO_19 mux 4 */
#define MUX_LP_GPIO_19_MUX4_SPI0_MOSI     4L
#define PINMUX_LP_GPIO_19_MUX4_SPI0_MOSI  ((PIN_LP_GPIO_19_MUX4_SPI0_MOSI << 16) | MUX_LP_GPIO_19_MUX4_SPI0_MOSI)


/* ========== GPIO definition for I2C peripheral ========== */
#define PIN_LP_GPIO_8_MUX2_I2C0_SDA			8L              /**< \brief I2C0 signal: SDA on LP_GPIO_8 mux 2 */
#define MUX_LP_GPIO_8_MUX2_I2C0_SDA			2L
#define PINMUX_LP_GPIO_8_MUX2_I2C0_SDA		((PIN_LP_GPIO_8_MUX2_I2C0_SDA << 16) | MUX_LP_GPIO_8_MUX2_I2C0_SDA)

#define PIN_LP_GPIO_9_MUX2_I2C0_SCK			9L              /**< \brief I2C0 signal: SCK on LP_GPIO_9 mux 2 */
#define MUX_LP_GPIO_9_MUX2_I2C0_SCK			2L
#define PINMUX_LP_GPIO_9_MUX2_I2C0_SCK		((PIN_LP_GPIO_9_MUX2_I2C0_SCK << 16) | MUX_LP_GPIO_9_MUX2_I2C0_SCK)

#define PIN_LP_GPIO_14_MUX4_I2C1_SDA		14L				/**< \brief I2C1 signal: SDA on LP_GPIO_14 mux 4 */
#define MUX_LP_GPIO_14_MUX4_I2C1_SDA		4L
#define PINMUX_LP_GPIO_14_MUX4_I2C1_SDA		((PIN_LP_GPIO_14_MUX4_I2C1_SDA << 16) | MUX_LP_GPIO_14_MUX4_I2C1_SDA)

#define PIN_LP_GPIO_15_MUX4_I2C1_SCK		15L				/**< \brief I2C1 signal: SCK on LP_GPIO_15 mux 4 */
#define MUX_LP_GPIO_15_MUX4_I2C1_SCK		4L
#define PINMUX_LP_GPIO_15_MUX4_I2C1_SCK		((PIN_LP_GPIO_15_MUX4_I2C1_SCK << 16) | MUX_LP_GPIO_15_MUX4_I2C1_SCK)

/* ========== GPIO definition for UART peripheral ========== */
#define PIN_LP_GPIO_2_MUX2_UART0_RXD		2L              /**< \brief UART0 signal: RXD on LP_GPIO_2 mux 2 */
#define MUX_LP_GPIO_2_MUX2_UART0_RXD		2L
#define PINMUX_LP_GPIO_2_MUX2_UART0_RXD		((PIN_LP_GPIO_2_MUX2_UART0_RXD << 16) | MUX_LP_GPIO_2_MUX2_UART0_RXD)

#define PIN_LP_GPIO_3_MUX2_UART0_TXD		3L              /**< \brief UART0 signal: TXD on LP_GPIO_3 mux 2 */
#define MUX_LP_GPIO_3_MUX2_UART0_TXD		2L
#define PINMUX_LP_GPIO_3_MUX2_UART0_TXD		((PIN_LP_GPIO_3_MUX2_UART0_TXD << 16) | MUX_LP_GPIO_3_MUX2_UART0_TXD)

#define PIN_LP_GPIO_4_MUX2_UART0_CTS		4L              /**< \brief UART0 signal: CTS on LP_GPIO_4 mux 2 */
#define MUX_LP_GPIO_4_MUX2_UART0_CTS		2L
#define PINMUX_LP_GPIO_4_MUX2_UART0_CTS		((PIN_LP_GPIO_4_MUX2_UART0_CTS << 16) | MUX_LP_GPIO_4_MUX2_UART0_CTS)

#define PIN_LP_GPIO_5_MUX2_UART0_RTS		5L              /**< \brief UART0 signal: RTS on LP_GPIO_5 mux 2 */
#define MUX_LP_GPIO_5_MUX2_UART0_RTS		2L
#define PINMUX_LP_GPIO_5_MUX2_UART0_RXD		((PIN_LP_GPIO_5_MUX2_UART0_RTS << 16) | MUX_LP_GPIO_5_MUX2_UART0_RTS)

#define PIN_LP_GPIO_6_MUX2_UART1_RXD		6L              /**< \brief UART1 signal: RXD on LP_GPIO_6 mux 2 */
#define MUX_LP_GPIO_6_MUX2_UART1_RXD		2L
#define PINMUX_LP_GPIO_6_MUX2_UART1_RXD		((PIN_LP_GPIO_6_MUX2_UART1_RXD << 16) | MUX_LP_GPIO_6_MUX2_UART1_RXD)

#define PIN_LP_GPIO_7_MUX2_UART1_TXD		7L              /**< \brief UART1 signal: TXD on LP_GPIO_7 mux 2 */
#define MUX_LP_GPIO_7_MUX2_UART1_TXD		2L
#define PINMUX_LP_GPIO_7_MUX2_UART1_TXD		((PIN_LP_GPIO_7_MUX2_UART1_TXD << 16) | MUX_LP_GPIO_7_MUX2_UART1_TXD)

#define PIN_LP_GPIO_14_MUX2_UART1_CTS		14L             /**< \brief UART1 signal: CTS on LP_GPIO_14 mux 2 */
#define MUX_LP_GPIO_14_MUX2_UART1_CTS		2L
#define PINMUX_LP_GPIO_14_MUX2_UART1_CTS	((PIN_LP_GPIO_14_MUX2_UART1_CTS << 16) | MUX_LP_GPIO_14_MUX2_UART1_CTS)

#define PIN_LP_GPIO_15_MUX2_UART1_RTS		15L             /**< \brief UART1 signal: RTS on LP_GPIO_15 mux 2 */
#define MUX_LP_GPIO_15_MUX2_UART1_RTS		2L
#define PINMUX_LP_GPIO_15_MUX2_UART1_RXD	((PIN_LP_GPIO_15_MUX2_UART1_RTS << 16) | MUX_LP_GPIO_15_MUX2_UART1_RTS)

#endif /* _SAMB11G18A_PIO_ */
