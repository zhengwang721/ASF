/**
 * \file
 *
 * \brief SAMD20 Xplained Pro board definition
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

#ifndef SAMD20_XPLAINED_PRO_H_INCLUDED
#define SAMD20_XPLAINED_PRO_H_INCLUDED

#include <conf_board.h>
#include <compiler.h>

/**
 * \ingroup group_common_boards
 * \defgroup samd20_xplained_pro_group SAMD20 Xplained Pro board
 *
 * @{
 */

/**
 * \defgroup samd20_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** Name string macro */
#define BOARD_NAME                "SAMD20_XPLAINED_PRO"

/** \name Board oscillator definitions
 * @{
 */
// TODO
/** @} */

/** \name LED0 definitions
 *  @{ */
#define LED0_PIN                  PIN_PB08
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name SW0 definitions
 *  @{ */
#define SW0_PIN                   PIN_PB09
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PB09A_EIC_EXTINT5
#define SW0_EIC_PIN_MUX           MUX_PB09A_EIC_EXTINT5
#define SW0_EIC_LINE              5
/** @} */

/**
 * \name LED #0 definitions
 *
 * Wrapper macros for LED0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define LED_0_NAME                "LED0 (yellow)"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 1

/**
 * \name Button #0 definitions
 *
 * Wrapper macros for SW0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define BUTTON_0_NAME             "SW0"
#define BUTTON_0_PIN              SW0_PIN
#define BUTTON_0_ACTIVE           SW0_ACTIVE
#define BUTTON_0_INACTIVE         SW0_INACTIVE
#define BUTTON_0_EIC_PIN          SW0_EIC_PIN
#define BUTTON_0_EIC_PIN_MUX      SW0_EIC_PIN_MUX
#define BUTTON_0_EIC_LINE         SW0_EIC_LINE
/** @} */

/** Number of on-board buttons */
#define BUTTON_COUNT 1

/** \name Extension header #1 pin definitions
 *  @{
 */
#define EXT1_PIN_3                PIN_PA04
#define EXT1_PIN_4                PIN_PA05
#define EXT1_PIN_5                PIN_PB04
#define EXT1_PIN_6                PIN_PB05
#define EXT1_PIN_7                PIN_PB00
#define EXT1_PIN_8                PIN_PB01
#define EXT1_PIN_9                PIN_PB06
#define EXT1_PIN_10               PIN_PB07
#define EXT1_PIN_11               PIN_PA18
#define EXT1_PIN_12               PIN_PA19
#define EXT1_PIN_13               PIN_PA11
#define EXT1_PIN_14               PIN_PA10
#define EXT1_PIN_15               PIN_PA15
#define EXT1_PIN_16               PIN_PA12
#define EXT1_PIN_17               PIN_PA14
#define EXT1_PIN_18               PIN_PA13
/** @} */

/** \name Extension header #1 pin definitions by function
 *  @{
 */
#define EXT1_PIN_ADC_0            EXT1_PIN_3
#define EXT1_PIN_ADC_1            EXT1_PIN_4
#define EXT1_PIN_GPIO_0           EXT1_PIN_5
#define EXT1_PIN_GPIO_1           EXT1_PIN_6
#define EXT1_PIN_PWM_0            EXT1_PIN_7
#define EXT1_PIN_PWM_1            EXT1_PIN_8
#define EXT1_PIN_IRQ              EXT1_PIN_9
#define EXT1_PIN_TWI_SDA          EXT1_PIN_11
#define EXT1_PIN_TWI_SCL          EXT1_PIN_12
#define EXT1_PIN_UART_RX          EXT1_PIN_13
#define EXT1_PIN_UART_TX          EXT1_PIN_14
#define EXT1_PIN_SPI_SS_1         EXT1_PIN_10
#define EXT1_PIN_SPI_SS_0         EXT1_PIN_15
#define EXT1_PIN_SPI_MOSI         EXT1_PIN_16
#define EXT1_PIN_SPI_MISO         EXT1_PIN_17
#define EXT1_PIN_SPI_SCK          EXT1_PIN_18
/** @} */

/** \name Extension header #1 ADC definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #1 PWM definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #1 IRQ/External interrupt definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #1 TWI definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #1 UART definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #1 SPI definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #2 pin definitions
 *  @{
 */
#define EXT2_PIN_3                PIN_PA06
#define EXT2_PIN_4                PIN_PA07
#define EXT2_PIN_5                PIN_PB12
#define EXT2_PIN_6                PIN_PB13
#define EXT2_PIN_7                PIN_PA30
#define EXT2_PIN_8                PIN_PA31
#define EXT2_PIN_9                PIN_PB16
#define EXT2_PIN_10               PIN_PB17
#define EXT2_PIN_11               PIN_PA18
#define EXT2_PIN_12               PIN_PA19
#define EXT2_PIN_13               PIN_PA25
#define EXT2_PIN_14               PIN_PA24
#define EXT2_PIN_15               PIN_PA16
#define EXT2_PIN_16               PIN_PA12
#define EXT2_PIN_17               PIN_PA14
#define EXT2_PIN_18               PIN_PA13
/** @} */

/** \name Extension header #2 pin definitions by function
 *  @{
 */
#define EXT2_PIN_ADC_0            EXT2_PIN_3
#define EXT2_PIN_ADC_1            EXT2_PIN_4
#define EXT2_PIN_GPIO_0           EXT2_PIN_5
#define EXT2_PIN_GPIO_1           EXT2_PIN_6
#define EXT2_PIN_PWM_0            EXT2_PIN_7
#define EXT2_PIN_PWM_1            EXT2_PIN_8
#define EXT2_PIN_IRQ              EXT2_PIN_9
#define EXT2_PIN_TWI_SDA          EXT2_PIN_11
#define EXT2_PIN_TWI_SCL          EXT2_PIN_12
#define EXT2_PIN_UART_RX          EXT2_PIN_13
#define EXT2_PIN_UART_TX          EXT2_PIN_14
#define EXT2_PIN_SPI_SS_1         EXT2_PIN_10
#define EXT2_PIN_SPI_SS_0         EXT2_PIN_15
#define EXT2_PIN_SPI_MOSI         EXT2_PIN_16
#define EXT2_PIN_SPI_MISO         EXT2_PIN_17
#define EXT2_PIN_SPI_SCK          EXT2_PIN_18
/** @} */

/** \name Extension header #2 ADC definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #2 PWM definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #2 IRQ/External interrupt definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #2 TWI definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #2 UART definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #2 SPI definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #3 pin definitions
 *  @{
 */
#define EXT3_PIN_3                PIN_PB02
#define EXT3_PIN_4                PIN_PB03
#define EXT3_PIN_5                PIN_PB24
#define EXT3_PIN_6                PIN_PB25
#define EXT3_PIN_7                PIN_PB18
#define EXT3_PIN_8                PIN_PB19
#define EXT3_PIN_9                PIN_PB26
#define EXT3_PIN_10               PIN_PB27
#define EXT3_PIN_11               PIN_PA18
#define EXT3_PIN_12               PIN_PA19
#define EXT3_PIN_13               PIN_PA27
#define EXT3_PIN_14               PIN_PA26
#define EXT3_PIN_15               PIN_PA17
#define EXT3_PIN_16               PIN_PA12
#define EXT3_PIN_17               PIN_PA14
#define EXT3_PIN_18               PIN_PA13
/** @} */

/** \name Extension header #3 pin definitions by function
 *  @{
 */
#define EXT3_PIN_ADC_0            EXT3_PIN_3
#define EXT3_PIN_ADC_1            EXT3_PIN_4
#define EXT3_PIN_GPIO_0           EXT3_PIN_5
#define EXT3_PIN_GPIO_1           EXT3_PIN_6
#define EXT3_PIN_PWM_0            EXT3_PIN_7
#define EXT3_PIN_PWM_1            EXT3_PIN_8
#define EXT3_PIN_IRQ              EXT3_PIN_9
#define EXT3_PIN_TWI_SDA          EXT3_PIN_11
#define EXT3_PIN_TWI_SCL          EXT3_PIN_12
#define EXT3_PIN_UART_RX          EXT3_PIN_13
#define EXT3_PIN_UART_TX          EXT3_PIN_14
#define EXT3_PIN_SPI_SS_1         EXT3_PIN_10
#define EXT3_PIN_SPI_SS_0         EXT3_PIN_15
#define EXT3_PIN_SPI_MOSI         EXT3_PIN_16
#define EXT3_PIN_SPI_MISO         EXT3_PIN_17
#define EXT3_PIN_SPI_SCK          EXT3_PIN_18
/** @} */

/** \name Extension header #3 ADC definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #3 PWM definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #3 IRQ/External interrupt definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #3 TWI definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #3 UART definitions
 *  @{
 */
// TODO
/** @} */

/** \name Extension header #3 SPI definitions
 *  @{
 */
// TODO
/** @} */

/** \name Embedded debugger GPIO interface definitions
 * @{
 */
#define EDBG_GPIO0_PIN            PIN_PB24
#define EDBG_GPIO1_PIN            PIN_PB25
#define EDBG_GPIO2_PIN            PIN_PB26
#define EDBG_GPIO3_PIN            PIN_PB27
/** @} */

/** \name Embedded debugger USART interface definitions
 * @{
 */
#define EDBG_UART_MODULE          SERCOM5
#define EDBG_UART_RX_PIN          PIN_PB20B_SERCOM5_PAD0
#define EDBG_UART_RX_MUX          MUX_PB20B_SERCOM5_PAD0
#define EDBG_UART_RX_SERCOM_PAD   0
#define EDBG_UART_TX_PIN          PIN_PB22B_SERCOM5_PAD2
#define EDBG_UART_TX_MUX          MUX_PB22B_SERCOM5_PAD2
#define EDBG_UART_TX_SERCOM_PAD   2
#define EDBG_UART_CLK_PIN         PIN_PB23B_SERCOM5_PAD3
#define EDBG_UART_CLK_MUX         MUX_PB23B_SERCOM5_PAD3
#define EDBG_UART_CLK_SERCOM_PAD  3
/** @} */

/** \name Embedded debugger TWI interface definitions
 * @{
 */
#define EDBG_TWI_MODULE           SERCOM1
#define EDBG_TWI_SDA_PIN          PIN_PA18B_SERCOM1_PAD0
#define EDBG_TWI_SDA_MUX          MUX_PA18B_SERCOM1_PAD0
#define EDBG_TWI_SDA_SERCOM_PAD   0
#define EDBG_TWI_SCL_PIN          PIN_PA19B_SERCOM1_PAD1
#define EDBG_TWI_SCL_MUX          MUX_PA19B_SERCOM1_PAD1
#define EDBG_TWI_SCL_SERCOM_PAD   1
/** @} */

/** \name Embedded debugger SPI interface definitions
 * @{
 */
#define EDBG_SPI_MODULE           SERCOM5
#define EDBG_SPI_MOSI_PIN         PIN_PB22B_SERCOM5_PAD2
#define EDBG_SPI_MOSI_MUX         MUX_PB22B_SERCOM5_PAD2
#define EDBG_SPI_MOSI_SERCOM_PAD  2
#define EDBG_SPI_MISO_PIN         PIN_PB20B_SERCOM5_PAD0
#define EDBG_SPI_MISO_MUX         MUX_PB20B_SERCOM5_PAD0
#define EDBG_SPI_MISO_SERCOM_PAD  0
#define EDBG_SPI_SCK_PIN          PIN_PB23B_SERCOM5_PAD3
#define EDBG_SPI_SCK_MUX          MUX_PB23B_SERCOM5_PAD3
#define EDBG_SPI_SCK_SERCOM_PAD   3
#define EDBG_SPI_SS_PIN           PIN_PB21B_SERCOM5_PAD1
#define EDBG_SPI_SS_MUX           MUX_PB21B_SERCOM5_PAD1
#define EDBG_SPI_SS_SERCOM_PAD    1
/** @} */

/** \name Embedded debugger CDC Gateway USART interface definitions
 * @{
 */
#define EDBG_CDC_MODULE           SERCOM5
#define EDBG_CDC_RX_PIN           PIN_PB20B_SERCOM5_PAD0
#define EDBG_CDC_RX_MUX           MUX_PB20B_SERCOM5_PAD0
#define EDBG_CDC_RX_SERCOM_PAD    0
#define EDBG_CDC_TX_PIN           PIN_PB22B_SERCOM5_PAD2
#define EDBG_CDC_TX_MUX           MUX_PB22B_SERCOM5_PAD2
#define EDBG_CDC_TX_SERCOM_PAD    2
#define EDBG_CDC_CLK_PIN          PIN_PB23B_SERCOM5_PAD3
#define EDBG_CDC_CLK_MUX          MUX_PB23B_SERCOM5_PAD3
#define EDBG_CDC_CLK_SERCOM_PAD   3
/** @} */

/** @} */

/** @} */

#endif  /* SAMD20_XPLAINED_PRO_H_INCLUDED */
