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

/** \name Resonator definitions
 *  @{ */
#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_BYPASS  0 /* Not Mounted */
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625
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
#define EXT1_ADC_MODULE           ADC
#define EXT1_ADC_0_CHANNEL        4
#define EXT1_ADC_0_PIN            PIN_PA04H_ADC_AIN4
#define EXT1_ADC_0_PIN_MUX        MUX_PA04H_ADC_AIN4
#define EXT1_ADC_1_CHANNEL        5
#define EXT1_ADC_1_PIN            PIN_PA05H_ADC_AIN4
#define EXT1_ADC_1_PIN_MUX        MUX_PA05H_ADC_AIN4
/** @} */

/** \name Extension header #1 PWM definitions
 *  @{
 */
#define EXT1_PWM_MODULE           TC5
#define EXT1_PWM_0_CHANNEL        0
#define EXT1_PWM_0_PIN            PIN_PB00D_TC5_WO0
#define EXT1_PWM_0_PIN_MUX        MUX_PB00D_TC5_WO0
#define EXT1_PWM_1_CHANNEL        0
#define EXT1_PWM_1_PIN            PIN_PB01D_TC5_WO0
#define EXT1_PWM_1_PIN_MUX        MUX_PB01D_TC5_WO0
/** @} */

/** \name Extension header #1 IRQ/External interrupt definitions
 *  @{
 */
#define EXT1_IRQ_MODULE           EIC
#define EXT1_IRQ_INPUT            10
#define EXT1_IRQ_PIN              PIN_PB06A_EIC_EXTINT10
#define EXT1_IRQ_PIN_MUX          MUX_PB06A_EIC_EXTINT10
/** @} */

/** \name Extension header #1 TWI definitions
 *  @{
 */
#define EXT1_TWI_MODULE           SERCOM4
#define EXT1_TWI_SDA_PIN          PIN_PB00C_SERCOM4_PAD0
#define EXT1_TWI_SDA_PIN_MUX      MUX_PB00C_SERCOM4_PAD0
#define EXT1_TWI_SDA_SERCOM_PAD   0
#define EXT1_TWI_SCL_PIN          PIN_PB01C_SERCOM4_PAD1
#define EXT1_TWI_SCL_PIN_MUX      MUX_PB01C_SERCOM4_PAD1
#define EXT1_TWI_SCL_SERCOM_PAD   1
/** @} */

/** \name Extension header #1 UART definitions
 *  @{
 */
#define EXT1_UART_MODULE          SERCOM0
#define EXT1_UART_RX_PIN          PIN_PA11B_SERCOM0_PAD1
#define EXT1_UART_RX_PIN_MUX      MUX_PA11B_SERCOM0_PAD1
#define EXT1_UART_RX_SERCOM_PAD   1
#define EXT1_UART_TX_PIN          PIN_PA10B_SERCOM0_PAD0
#define EXT1_UART_TX_PIN_MUX      MUX_PA10B_SERCOM0_PAD0
#define EXT1_UART_TX_SERCOM_PAD   0
/** @} */

/** \name Extension header #1 SPI definitions
 *  @{
 */
#define EXT1_SPI_MODULE           SERCOM2
#define EXT1_SPI_SS_PIN           PIN_PA15B_SERCOM2_PAD1
#define EXT1_SPI_SS_PIN_MUX       MUX_PA15B_SERCOM2_PAD1
#define EXT1_SPI_SS_SERCOM_PAD    1
#define EXT1_SPI_MOSI_PIN         PIN_PA12B_SERCOM2_PAD2
#define EXT1_SPI_MOSI_PIN_MUX     PIN_PA12B_SERCOM2_PAD2
#define EXT1_SPI_MOSI_SERCOM_PAD  2
#define EXT1_SPI_MISO_PIN         PIN_PA14B_SERCOM2_PAD0
#define EXT1_SPI_MISO_PIN_MUX     PIN_PA14B_SERCOM2_PAD0
#define EXT1_SPI_MISO_SERCOM_PAD  0
#define EXT1_SPI_SCK_PIN          PIN_PA13B_SERCOM2_PAD3
#define EXT1_SPI_SCK_PIN_MUX      MUX_PA13B_SERCOM2_PAD3
#define EXT1_SPI_SCK_SERCOM_PAD   3
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
#define EXT2_ADC_MODULE           ADC
#define EXT2_ADC_0_CHANNEL        6
#define EXT2_ADC_0_PIN            PIN_PA06H_ADC_AIN6
#define EXT2_ADC_0_PIN_MUX        MUX_PA06H_ADC_AIN6
#define EXT2_ADC_1_CHANNEL        7
#define EXT2_ADC_1_PIN            PIN_PA07H_ADC_AIN7
#define EXT2_ADC_1_PIN_MUX        MUX_PA07H_ADC_AIN7
/** @} */

/** \name Extension header #2 PWM definitions
 *  @{
 */
#define EXT2_PWM_MODULE           TC1
#define EXT2_PWM_0_CHANNEL        0
#define EXT2_PWM_0_PIN            PIN_PA30D_TC1_WO0
#define EXT2_PWM_0_PIN_MUX        MUX_PA30D_TC1_WO0
#define EXT2_PWM_1_CHANNEL        1
#define EXT2_PWM_1_PIN            PIN_PA31D_TC1_WO1
#define EXT2_PWM_1_PIN_MUX        MUX_PA31D_TC1_WO1
/** @} */

/** \name Extension header #2 IRQ/External interrupt definitions
 *  @{
 */
#define EXT1_IRQ_MODULE           EIC
#define EXT1_IRQ_INPUT            10
#define EXT1_IRQ_PIN              PIN_PB06A_EIC_EXTINT10
#define EXT1_IRQ_PIN_MUX          MUX_PB06A_EIC_EXTINT10
/** @} */

/** \name Extension header #2 TWI definitions
 *  @{
 */
#define EXT2_TWI_MODULE           SERCOM3
#define EXT2_TWI_SDA_PIN          PIN_PA18C_SERCOM3_PAD0
#define EXT2_TWI_SDA_PIN_MUX      MUX_PA18C_SERCOM3_PAD0
#define EXT2_TWI_SDA_SERCOM_PAD   0
#define EXT2_TWI_SCL_PIN          PIN_PA19C_SERCOM3_PAD1
#define EXT2_TWI_SCL_PIN_MUX      MUX_PA19C_SERCOM3_PAD1
#define EXT2_TWI_SCL_SERCOM_PAD   1
/** @} */

/** \name Extension header #2 UART definitions
 *  @{
 */
#define EXT2_UART_MODULE          SERCOM3
#define EXT2_UART_RX_PIN          PIN_PA25B_SERCOM3_PAD1
#define EXT2_UART_RX_PIN_MUX      MUX_PA25B_SERCOM3_PAD1
#define EXT2_UART_RX_SERCOM_PAD   1
#define EXT2_UART_TX_PIN          PIN_PA24B_SERCOM3_PAD0
#define EXT2_UART_TX_PIN_MUX      MUX_PA24B_SERCOM3_PAD0
#define EXT2_UART_TX_SERCOM_PAD   0
/** @} */

/** \name Extension header #2 SPI definitions
 *  @{
 */
#define EXT2_SPI_MODULE           SERCOM2
#define EXT2_SPI_SS_PIN           /* Not available on this SERCOM */
#define EXT2_SPI_SS_PIN_MUX       /* Not available on this SERCOM */
#define EXT2_SPI_SS_SERCOM_PAD    /* Not available on this SERCOM */
#define EXT2_SPI_MOSI_PIN         PIN_PA12B_SERCOM2_PAD2
#define EXT2_SPI_MOSI_PIN_MUX     MUX_PA12B_SERCOM2_PAD2
#define EXT2_SPI_MOSI_SERCOM_PAD  2
#define EXT2_SPI_MISO_PIN         PIN_PA14B_SERCOM2_PAD0
#define EXT2_SPI_MISO_PIN_MUX     MUX_PA14B_SERCOM2_PAD0
#define EXT2_SPI_MISO_SERCOM_PAD  0
#define EXT2_SPI_SCK_PIN          PIN_PA13B_SERCOM2_PAD3
#define EXT2_SPI_SCK_PIN_MUX      MUX_PA13B_SERCOM2_PAD3
#define EXT2_SPI_SCK_SERCOM_PAD   3
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
#define EXT3_ADC_MODULE           ADC
#define EXT3_ADC_0_CHANNEL        10
#define EXT3_ADC_0_PIN            PIN_PB02H_ADC_AIN10
#define EXT3_ADC_0_PIN_MUX        MUX_PB02H_ADC_AIN10
#define EXT3_ADC_1_CHANNEL        11
#define EXT3_ADC_1_PIN            PIN_PB03H_ADC_AIN11
#define EXT3_ADC_1_PIN_MUX        MUX_PB03H_ADC_AIN11
/** @} */

/** \name Extension header #3 PWM definitions
 *  @{
 */
#define EXT3_PWM_MODULE           TC7
#define EXT3_PWM_0_CHANNEL        0
#define EXT3_PWM_0_PIN            PIN_PB18D_TC7_WO0
#define EXT3_PWM_0_PIN_MUX        MUX_PB18D_TC7_WO0
#define EXT3_PWM_1_CHANNEL        1
#define EXT3_PWM_1_PIN            PIN_PB19D_TC7_WO1
#define EXT3_PWM_1_PIN_MUX        MUX_PB19D_TC7_WO1
/** @} */

/** \name Extension header #3 IRQ/External interrupt definitions
 *  @{
 */
#define EXT3_IRQ_MODULE           EIC
#define EXT3_IRQ_INPUT            14
#define EXT3_IRQ_PIN              PIN_PB26A_EIC_EXTINT14
#define EXT3_IRQ_PIN_MUX          MUX_PB26A_EIC_EXTINT14
/** @} */

/** \name Extension header #3 TWI definitions
 *  @{
 */
#define EXT3_TWI_MODULE           SERCOM1
#define EXT3_TWI_SDA_PIN          PIN_PA18B_SERCOM1_PAD0
#define EXT3_TWI_SDA_PIN_MUX      MUX_PA18B_SERCOM1_PAD0
#define EXT3_TWI_SDA_SERCOM_PAD   0
#define EXT3_TWI_SCL_PIN          PIN_PA19B_SERCOM1_PAD1
#define EXT3_TWI_SCL_PIN_MUX      MUX_PA19B_SERCOM1_PAD1
#define EXT3_TWI_SCL_SERCOM_PAD   1
/** @} */

/** \name Extension header #3 UART definitions
 *  @{
 */
#define EXT3_UART_MODULE          SERCOM3
#define EXT3_UART_RX_PIN          PIN_PA27B_SERCOM3_PAD3
#define EXT3_UART_RX_PIN_MUX      MUX_PA27B_SERCOM3_PAD3
#define EXT3_UART_RX_SERCOM_PAD   3
#define EXT3_UART_TX_PIN          PIN_PA26B_SERCOM3_PAD2
#define EXT3_UART_TX_PIN_MUX      MUX_PA26B_SERCOM3_PAD2
#define EXT3_UART_TX_SERCOM_PAD   2
/** @} */

/** \name Extension header #3 SPI definitions
 *  @{
 */
#define EXT3_SPI_MODULE          SERCOM2
#define EXT3_SPI_SS_PIN          /* Not available on this SERCOM */
#define EXT3_SPI_SS_PIN_MUX      /* Not available on this SERCOM */
#define EXT3_SPI_SS_SERCOM_PAD   /* Not available on this SERCOM */
#define EXT3_SPI_MOSI_PIN        PIN_PA12B_SERCOM2_PAD2
#define EXT3_SPI_MOSI_PIN_MUX    MUX_PA12B_SERCOM2_PAD2
#define EXT3_SPI_MOSI_SERCOM_PAD 2
#define EXT3_SPI_MISO_PIN        PIN_PA14B_SERCOM2_PAD0
#define EXT3_SPI_MISO_PIN_MUX    MUX_PA14B_SERCOM2_PAD0
#define EXT3_SPI_MISO_SERCOM_PAD 0
#define EXT3_SPI_SCK_PIN         PIN_PA13B_SERCOM2_PAD3
#define EXT3_SPI_SCK_PIN_MUX     MUX_PA13B_SERCOM2_PAD3
#define EXT3_SPI_SCK_SERCOM_PAD  3
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
