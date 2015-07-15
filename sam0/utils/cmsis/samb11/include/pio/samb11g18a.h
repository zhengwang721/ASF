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

#define MEGAMUX_SEL0_UART1_RXD            0L
#define MEGAMUX_SEL1_UART1_TXD            1L
#define MEGAMUX_SEL2_UART1_CTS            2L
#define MEGAMUX_SEL3_UART1 RTS            3L
#define MEGAMUX_SEL4_UART2_RXD            4L
#define MEGAMUX_SEL5_UART2_TXD            5L
#define MEGAMUX_SEL6_UART2_CTS            6L
#define MEGAMUX_SEL7_UART2_RTS            7L
#define MEGAMUX_SEL8_I2C0_SDA             8L
#define MEGAMUX_SEL9_I2C0_SCL             9L
#define MEGAMUX_SEL10_I2C1_SDA            10L
#define MEGAMUX_SEL11_I2C1_SCL            11L
#define MEGAMUX_SEL12_PWM_1               12L
#define MEGAMUX_SEL13_PWM_2               13L
#define MEGAMUX_SEL14_PWM_3               14L
#define MEGAMUX_SEL15_PWM_4               15L
#define MEGAMUX_SEL16_LP_CLOCK_OUT        16L
#define MEGAMUX_SEL17_WLAN_TX_ACTIVE      17L
#define MEGAMUX_SEL18_WLAN_RX_ACTIVE      18L
#define MEGAMUX_SEL19_BLE_TX_ACTIVE       19L
#define MEGAMUX_SEL20_BLE_RX_ACTIVE       20L
#define MEGAMUX_SEL21_BLE_IN_PROCESS      21L
#define MEGAMUX_SEL22_BLE_MBSY            22L
#define MEGAMUX_SEL23_BLE_SYNC            23L
#define MEGAMUX_SEL24_BLE_RXNTX           24L
#define MEGAMUX_SEL25_BLE_PTI 0           25L
#define MEGAMUX_SEL26_BLE_PTI 1           26L
#define MEGAMUX_SEL27_BLE_PTI 2           27L
#define MEGAMUX_SEL28_BLE_PTI 3           28L
#define MEGAMUX_SEL29_QUAD_DEC_X_IN_A     29L
#define MEGAMUX_SEL30_QUAD_DEC_X_IN_B     30L
#define MEGAMUX_SEL31_QUAD_DEC_Y_IN_A     31L
#define MEGAMUX_SEL32_QUAD_DEC_Y_IN_B     32L
#define MEGAMUX_SEL33_QUAD_DEC_Z_IN_A     33L
#define MEGAMUX_SEL34_QUAD_DEC_Z_IN_B     34L

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

#define PIN_LP_GPIO_5_MUX5_SPI0_MISO       5L              /**< \brief SPI0 signal: MISO on LP_GPIO_5 mux 5 */
#define MUX_LP_GPIO_5_MUX5_SPI0_MISO       5L
#define PINMUX_LP_GPIO_5_MUX5_SPI0_MISO    ((PIN_LP_GPIO_5_MUX5_SPI0_MISO << 16) | MUX_LP_GPIO_5_MUX5_SPI0_MISO)

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
#define PIN_LP_GPIO_2_MUX4_SPI1_SCK        2L              /**< \brief SPI1 signal: SCK on LP_GPIO_2 mux 4 */
#define MUX_LP_GPIO_2_MUX4_SPI1_SCK        4L
#define PINMUX_LP_GPIO_2_MUX4_SPI1_SCK     ((PIN_LP_GPIO_2_MUX4_SPI1_SCK << 16) | MUX_LP_GPIO_2_MUX4_SPI1_SCK)

#define PIN_LP_GPIO_3_MUX4_SPI1_MOSI       3L              /**< \brief SPI1 signal: MOSI on LP_GPIO_3 mux 4 */
#define MUX_LP_GPIO_3_MUX4_SPI1_MOSI       4L
#define PINMUX_LP_GPIO_3_MUX4_SPI1_MOSI    ((PIN_LP_GPIO_3_MUX4_SPI1_MOSI << 16) | MUX_LP_GPIO_3_MUX4_SPI1_MOSI)

#define PIN_LP_GPIO_4_MUX4_SPI1_SSN        4L              /**< \brief SPI1 signal: SSN on LP_GPIO_4 mux 4 */
#define MUX_LP_GPIO_4_MUX4_SPI1_SSN        4L
#define PINMUX_LP_GPIO_4_MUX4_SPI1_SSN     ((PIN_LP_GPIO_4_MUX4_SPI1_SSN << 16) | MUX_LP_GPIO_4_MUX4_SPI1_SSN)

#define PIN_LP_GPIO_5_MUX4_SPI1_MISO       5L              /**< \brief SPI1 signal: SCK on LP_GPIO_5 mux 4 */
#define MUX_LP_GPIO_5_MUX4_SPI1_MISO       4L
#define PINMUX_LP_GPIO_5_MUX4_SPI1_MISO    ((PIN_LP_GPIO_5_MUX4_SPI1_MISO << 16) | MUX_LP_GPIO_5_MUX4_SPI1_MISO)

#define PIN_LP_GPIO_16_MUX4_SPI1_SSN       16L              /**< \brief SPI1 signal: SSN on LP_GPIO_16 mux 4 */
#define MUX_LP_GPIO_16_MUX4_SPI1_SSN       4L
#define PINMUX_LP_GPIO_16_MUX4_SPI1_SSN    ((PIN_LP_GPIO_16_MUX4_SPI1_SSN << 16) | MUX_LP_GPIO_16_MUX4_SPI1_SSN)

#define PIN_LP_GPIO_17_MUX4_SPI1_SCK       17L              /**< \brief SPI1 signal: SCK on LP_GPIO_17 mux 4 */
#define MUX_LP_GPIO_17_MUX4_SPI1_SCK       4L
#define PINMUX_LP_GPIO_17_MUX4_SPI1_SCK    ((PIN_LP_GPIO_17_MUX4_SPI1_SCK << 16) | MUX_LP_GPIO_17_MUX4_SPI1_SCK)

#define PIN_LP_GPIO_18_MUX4_SPI1_MISO      18L              /**< \brief SPI1 signal: MISO on LP_GPIO_18 mux 4 */
#define MUX_LP_GPIO_18_MUX4_SPI1_MISO      4L
#define PINMUX_LP_GPIO_18_MUX4_SPI1_MISO   ((PIN_LP_GPIO_18_MUX4_SPI1_MISO << 16) | MUX_LP_GPIO_18_MUX4_SPI1_MISO)

#define PIN_LP_GPIO_19_MUX4_SPI1_MOSI      19L              /**< \brief SPI1 signal: MOSI on LP_GPIO_19 mux 4 */
#define MUX_LP_GPIO_19_MUX4_SPI1_MOSI      4L
#define PINMUX_LP_GPIO_19_MUX4_SPI1_MOSI   ((PIN_LP_GPIO_19_MUX4_SPI1_MOSI << 16) | MUX_LP_GPIO_19_MUX4_SPI1_MOSI)


/* ========== GPIO definition for I2C peripheral ========== */
#define PIN_LP_GPIO_8_MUX2_I2C0_SDA        8L              /**< \brief I2C0 signal: SDA on LP_GPIO_8 mux 2 */
#define MUX_LP_GPIO_8_MUX2_I2C0_SDA        2L
#define PINMUX_LP_GPIO_8_MUX2_I2C0_SDA     ((PIN_LP_GPIO_8_MUX2_I2C0_SDA << 16) | MUX_LP_GPIO_8_MUX2_I2C0_SDA)

#define PIN_LP_GPIO_9_MUX2_I2C0_SCK        9L              /**< \brief I2C0 signal: SCK on LP_GPIO_9 mux 2 */
#define MUX_LP_GPIO_9_MUX2_I2C0_SCK        2L
#define PINMUX_LP_GPIO_9_MUX2_I2C0_SCK     ((PIN_LP_GPIO_9_MUX2_I2C0_SCK << 16) | MUX_LP_GPIO_9_MUX2_I2C0_SCK)

#define PIN_LP_GPIO_14_MUX4_I2C1_SDA       14L             /**< \brief I2C1 signal: SDA on LP_GPIO_14 mux 4 */
#define MUX_LP_GPIO_14_MUX4_I2C1_SDA       4L
#define PINMUX_LP_GPIO_14_MUX4_I2C1_SDA    ((PIN_LP_GPIO_14_MUX4_I2C1_SDA << 16) | MUX_LP_GPIO_14_MUX4_I2C1_SDA)

#define PIN_LP_GPIO_15_MUX4_I2C1_SCK       15L             /**< \brief I2C1 signal: SCK on LP_GPIO_15 mux 4 */
#define MUX_LP_GPIO_15_MUX4_I2C1_SCK       4L
#define PINMUX_LP_GPIO_15_MUX4_I2C1_SCK    ((PIN_LP_GPIO_15_MUX4_I2C1_SCK << 16) | MUX_LP_GPIO_15_MUX4_I2C1_SCK)

/* I2C0 signal: SDA on gpio_pin mux 1 and MEGAMUX 8 */
#define PINMUX_MEGAMUX_SEL8_I2C0_SDA_PIN(gpio_pin)    ((gpio_pin << 16) | (MEGAMUX_SEL8_I2C0_SDA << 8) | 1)

/* I2C0 signal: SCK on gpio_pin mux 1 and MEGAMUX 9 */
#define PINMUX_MEGAMUX_SEL9_I2C0_SCK_PIN(gpio_pin)    ((gpio_pin << 16) | (MEGAMUX_SEL9_I2C0_SCL << 8) | 1)

/* I2C1 signal: SDA on gpio_pin mux 1 and MEGAMUX 10 */
#define PINMUX_MEGAMUX_SEL10_I2C0_SDA_PIN(gpio_pin)   ((gpio_pin << 16) | (MEGAMUX_SEL10_I2C1_SDA << 8) | 1)

/* I2C1 signal: SCK on gpio_pin mux 1 and MEGAMUX 11 */
#define PINMUX_MEGAMUX_SEL11_I2C0_SCK_PIN(gpio_pin)   ((gpio_pin << 16) | (MEGAMUX_SEL11_I2C1_SCL << 8) | 1)

/* ========== GPIO definition for PWM peripheral ========== */
/* PWM1 signal: PWM1 on gpio_pin mux 1 and MEGAMUX 12 */
#define PINMUX_MEGAMUX_SEL12_PWM1_PIN(gpio_pin)       ((gpio_pin << 16) | (MEGAMUX_SEL12_PWM_1 << 8) | 1)

/* PWM2 signal: PWM2 on gpio_pin mux 1 and MEGAMUX 13 */
#define PINMUX_MEGAMUX_SEL13_PWM2_PIN(gpio_pin)       ((gpio_pin << 16) | (MEGAMUX_SEL13_PWM_2 << 8) | 1)

/* PWM3 signal: PWM3 on gpio_pin mux 1 and MEGAMUX 14 */
#define PINMUX_MEGAMUX_SEL14_PWM3_PIN(gpio_pin)       ((gpio_pin << 16) | (MEGAMUX_SEL14_PWM_3 << 8) | 1)

/* PWM4 signal: PWM4 on gpio_pin mux 1 and MEGAMUX 15 */
#define PINMUX_MEGAMUX_SEL15_PWM4_PIN(gpio_pin)       ((gpio_pin << 16) | (MEGAMUX_SEL15_PWM_4 << 8) | 1)

/* ========== GPIO definition for QUAD DECODER peripheral ========== */
/* QDEC signal: QDEC X IN A on gpio_pin mux 1 and MEGAMUX 29 */
#define PINMUX_MEGAMUX_SEL29_QDEC_X_A_PIN(gpio_pin)   ((gpio_pin << 16) | (MEGAMUX_SEL29_QUAD_DEC_X_IN_A << 8) | 1)

/* QDEC signal: QDEC X IN B on gpio_pin mux 1 and MEGAMUX 30 */
#define PINMUX_MEGAMUX_SEL30_QDEC_X_B_PIN(gpio_pin)   ((gpio_pin << 16) | (MEGAMUX_SEL30_QUAD_DEC_X_IN_B << 8) | 1)

/* QDEC signal: QDEC Y IN A on gpio_pin mux 1 and MEGAMUX 31 */
#define PINMUX_MEGAMUX_SEL31_QDEC_Y_A_PIN(gpio_pin)   ((gpio_pin << 16) | (MEGAMUX_SEL31_QUAD_DEC_Y_IN_A << 8) | 1)

/* QDEC signal: QDEC Y IN A on gpio_pin mux 1 and MEGAMUX 32 */
#define PINMUX_MEGAMUX_SEL32_QDEC_Y_B_PIN(gpio_pin)   ((gpio_pin << 16) | (MEGAMUX_SEL32_QUAD_DEC_Y_IN_B << 8) | 1)

/* QDEC signal: QDEC Z IN A on gpio_pin mux 1 and MEGAMUX 33 */
#define PINMUX_MEGAMUX_SEL33_QDEC_Z_A_PIN(gpio_pin)   ((gpio_pin << 16) | (MEGAMUX_SEL33_QUAD_DEC_Z_IN_A << 8) | 1)

/* QDEC signal: QDEC Z IN B on gpio_pin mux 1 and MEGAMUX 34 */
#define PINMUX_MEGAMUX_SEL34_QDEC_Z_B_PIN(gpio_pin)   ((gpio_pin << 16) | (MEGAMUX_SEL34_QUAD_DEC_Z_IN_B << 8) | 1)

#endif /* _SAMB11G18A_PIO_ */
