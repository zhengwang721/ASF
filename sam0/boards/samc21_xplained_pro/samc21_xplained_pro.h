/**
 * \file
 *
 * \brief SAM C21 Xplained Pro board definition
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

#ifndef SAMC21_XPLAINED_PRO_H_INCLUDED
#define SAMC21_XPLAINED_PRO_H_INCLUDED

#include <conf_board.h>
#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup samc21_xplained_pro_group SAM C21 Xplained Pro board
 *
 * @{
 */

void system_board_init(void);

/**
 * \defgroup samc21_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** Name string macro */
#define BOARD_NAME                "SAMC21_XPLAINED_PRO"

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
#define LED0_PIN                  PIN_PA15
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name SW0 definitions
 *  @{ */
#define SW0_PIN                   PIN_PA28
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA28A_EIC_EXTINT8
#define SW0_EIC_MUX               MUX_PA28A_EIC_EXTINT8
#define SW0_EIC_PINMUX            PINMUX_PA28A_EIC_EXTINT8
#define SW0_EIC_LINE              8
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
#define LED0_GPIO                 LED0_PIN
#define LED0                      LED0_PIN

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
#define BUTTON_0_EIC_MUX          SW0_EIC_MUX
#define BUTTON_0_EIC_PINMUX       SW0_EIC_PINMUX
#define BUTTON_0_EIC_LINE         SW0_EIC_LINE
/** @} */

/** Number of on-board buttons */
#define BUTTON_COUNT 1

/** \name Extension header #1 pin definitions
 *  @{
 */
#define EXT1_PIN_3                PIN_PB09
#define EXT1_PIN_4                PIN_PB08
#define EXT1_PIN_5                PIN_PA20
#define EXT1_PIN_6                PIN_PA21
#define EXT1_PIN_7                PIN_PB12
#define EXT1_PIN_8                PIN_PB13
#define EXT1_PIN_9                PIN_PB14
#define EXT1_PIN_10               PIN_PB15
#define EXT1_PIN_11               PIN_PA12
#define EXT1_PIN_12               PIN_PA13
#define EXT1_PIN_13               PIN_PA23
#define EXT1_PIN_14               PIN_PA22
#define EXT1_PIN_15               PIN_PA17
#define EXT1_PIN_16               PIN_PA18
#define EXT1_PIN_17               PIN_PA16
#define EXT1_PIN_18               PIN_PA19
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
#define EXT1_PIN_I2C_SDA          EXT1_PIN_11
#define EXT1_PIN_I2C_SCL          EXT1_PIN_12
#define EXT1_PIN_UART_RX          EXT1_PIN_13
#define EXT1_PIN_UART_TX          EXT1_PIN_14
#define EXT1_PIN_SPI_SS_1         EXT1_PIN_10
#define EXT1_PIN_SPI_SS_0         EXT1_PIN_15
#define EXT1_PIN_SPI_MOSI         EXT1_PIN_16
#define EXT1_PIN_SPI_MISO         EXT1_PIN_17
#define EXT1_PIN_SPI_SCK          EXT1_PIN_18
/** @} */

/** \name Extension header #1 I2C definitions
 *  @{
 */
#define EXT1_I2C_MODULE              SERCOM2
#define EXT1_I2C_SERCOM_PINMUX_PAD0  PINMUX_PA12C_SERCOM2_PAD0
#define EXT1_I2C_SERCOM_PINMUX_PAD1  PINMUX_PA13C_SERCOM2_PAD1
#define EXT1_I2C_SERCOM_DMAC_ID_TX   SERCOM2_DMAC_ID_TX
#define EXT1_I2C_SERCOM_DMAC_ID_RX   SERCOM2_DMAC_ID_RX
/** @} */

/** \name Extension header #1 UART definitions
 *  @{
 */
#define EXT1_UART_MODULE              SERCOM3
#define EXT1_UART_SERCOM_MUX_SETTING  USART_RX_1_TX_0_XCK_1
#define EXT1_UART_SERCOM_PINMUX_PAD0  PINMUX_PA22C_SERCOM3_PAD0
#define EXT1_UART_SERCOM_PINMUX_PAD1  PINMUX_PA23C_SERCOM3_PAD1
#define EXT1_UART_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define EXT1_UART_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
#define EXT1_UART_SERCOM_DMAC_ID_TX   SERCOM3_DMAC_ID_TX
#define EXT1_UART_SERCOM_DMAC_ID_RX   SERCOM3_DMAC_ID_RX
/** @} */

/** \name Extension header #1 SPI definitions
 *  @{
 */
#define EXT1_SPI_MODULE              SERCOM1
#define EXT1_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_E
#define EXT1_SPI_SERCOM_PINMUX_PAD0  PINMUX_PA16C_SERCOM1_PAD0
#define EXT1_SPI_SERCOM_PINMUX_PAD1  PINMUX_PA17C_SERCOM1_PAD1
#define EXT1_SPI_SERCOM_PINMUX_PAD2  PINMUX_PA18C_SERCOM1_PAD2
#define EXT1_SPI_SERCOM_PINMUX_PAD3  PINMUX_PA19C_SERCOM1_PAD3
#define EXT1_SPI_SERCOM_DMAC_ID_TX   SERCOM1_DMAC_ID_TX
#define EXT1_SPI_SERCOM_DMAC_ID_RX   SERCOM1_DMAC_ID_RX
/** @} */

/** \name Extension header #2 pin definitions
 *  @{
 */
#define EXT2_PIN_3                PIN_PA08
#define EXT2_PIN_4                PIN_PA09
#define EXT2_PIN_5                PIN_PA10
#define EXT2_PIN_6                PIN_PA11
#define EXT2_PIN_7                PIN_PB30
#define EXT2_PIN_8                PIN_PB31
#define EXT2_PIN_9                PIN_PB16
#define EXT2_PIN_10               PIN_PB17
#define EXT2_PIN_11               PIN_PA12
#define EXT2_PIN_12               PIN_PA13
#define EXT2_PIN_13               PIN_PA23
#define EXT2_PIN_14               PIN_PA22
#define EXT2_PIN_15               PIN_PB03
#define EXT2_PIN_16               PIN_PB00
#define EXT2_PIN_17               PIN_PB02
#define EXT2_PIN_18               PIN_PB01
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
#define EXT2_PIN_I2C_SDA          EXT2_PIN_11
#define EXT2_PIN_I2C_SCL          EXT2_PIN_12
#define EXT2_PIN_UART_RX          EXT2_PIN_13
#define EXT2_PIN_UART_TX          EXT2_PIN_14
#define EXT2_PIN_SPI_SS_1         EXT2_PIN_10
#define EXT2_PIN_SPI_SS_0         EXT2_PIN_15
#define EXT2_PIN_SPI_MOSI         EXT2_PIN_16
#define EXT2_PIN_SPI_MISO         EXT2_PIN_17
#define EXT2_PIN_SPI_SCK          EXT2_PIN_18
/** @} */

 /** \name Extension header #2 I2C definitions
 *  @{
 */
#define EXT2_I2C_MODULE              SERCOM2
#define EXT2_I2C_SERCOM_PINMUX_PAD0  PINMUX_PA12C_SERCOM2_PAD0
#define EXT2_I2C_SERCOM_PINMUX_PAD1  PINMUX_PA13C_SERCOM2_PAD1
#define EXT2_I2C_SERCOM_DMAC_ID_TX   SERCOM2_DMAC_ID_TX
#define EXT2_I2C_SERCOM_DMAC_ID_RX   SERCOM2_DMAC_ID_RX
/** @} */

/** \name Extension header #2 UART definitions
 *  @{
 */
#define EXT2_UART_MODULE              SERCOM3
#define EXT2_UART_SERCOM_MUX_SETTING  USART_RX_1_TX_0_XCK_1
#define EXT2_UART_SERCOM_PINMUX_PAD0  PINMUX_PA22C_SERCOM3_PAD0
#define EXT2_UART_SERCOM_PINMUX_PAD1  PINMUX_PA23C_SERCOM3_PAD1
#define EXT2_UART_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define EXT2_UART_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
#define EXT2_UART_SERCOM_DMAC_ID_TX   SERCOM3_DMAC_ID_TX
#define EXT2_UART_SERCOM_DMAC_ID_RX   SERCOM3_DMAC_ID_RX
/** @} */

/** \name Extension header #2 SPI definitions
 *  @{
 */
#define EXT2_SPI_MODULE              SERCOM5
#define EXT2_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_E
#define EXT2_SPI_SERCOM_PINMUX_PAD0  PINMUX_PB02D_SERCOM5_PAD0
#define EXT2_SPI_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define EXT2_SPI_SERCOM_PINMUX_PAD2  PINMUX_PB00D_SERCOM5_PAD2
#define EXT2_SPI_SERCOM_PINMUX_PAD3  PINMUX_PB01D_SERCOM5_PAD3
#define EXT2_SPI_SERCOM_DMAC_ID_TX   SERCOM5_DMAC_ID_TX
#define EXT2_SPI_SERCOM_DMAC_ID_RX   SERCOM5_DMAC_ID_RX
/** @} */

/** \name Extension header #3 pin definitions
 *  @{
 */
#define EXT3_PIN_3                PIN_PB07
#define EXT3_PIN_4                PIN_PB06
#define EXT3_PIN_5                PIN_PB04
#define EXT3_PIN_6                PIN_PB05
#define EXT3_PIN_7                PIN_PA14
#define EXT3_PIN_8                PIN_PA15
#define EXT3_PIN_9                PIN_PA28
#define EXT3_PIN_10               PIN_PA27
#define EXT3_PIN_11               PIN_PA12
#define EXT3_PIN_12               PIN_PA13
#define EXT3_PIN_13               PIN_PA23
#define EXT3_PIN_14               PIN_PA22
#define EXT3_PIN_15               PIN_PA02
#define EXT3_PIN_16               PIN_PB00
#define EXT3_PIN_17               PIN_PB02
#define EXT3_PIN_18               PIN_PB01
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
#define EXT3_PIN_I2C_SDA          EXT3_PIN_11
#define EXT3_PIN_I2C_SCL          EXT3_PIN_12
#define EXT3_PIN_UART_RX          EXT3_PIN_13
#define EXT3_PIN_UART_TX          EXT3_PIN_14
#define EXT3_PIN_SPI_SS_1         EXT3_PIN_10
#define EXT3_PIN_SPI_SS_0         EXT3_PIN_15
#define EXT3_PIN_SPI_MOSI         EXT3_PIN_16
#define EXT3_PIN_SPI_MISO         EXT3_PIN_17
#define EXT3_PIN_SPI_SCK          EXT3_PIN_18
/** @} */

/** \name Extension header #3 I2C definitions
 *  @{
 */
#define EXT3_I2C_MODULE              SERCOM2
#define EXT3_I2C_SERCOM_PINMUX_PAD0  PINMUX_PA12C_SERCOM2_PAD0
#define EXT3_I2C_SERCOM_PINMUX_PAD1  PINMUX_PA13C_SERCOM2_PAD1
#define EXT3_I2C_SERCOM_DMAC_ID_TX   SERCOM2_DMAC_ID_TX
#define EXT3_I2C_SERCOM_DMAC_ID_RX   SERCOM2_DMAC_ID_RX
/** @} */

/** \name Extension header #3 SPI definitions
 *  @{
 */
#define EXT3_SPI_MODULE              SERCOM5
#define EXT3_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_E
#define EXT3_SPI_SERCOM_PINMUX_PAD0  PINMUX_PB02D_SERCOM5_PAD0
#define EXT3_SPI_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define EXT3_SPI_SERCOM_PINMUX_PAD2  PINMUX_PB00D_SERCOM5_PAD2
#define EXT3_SPI_SERCOM_PINMUX_PAD3  PINMUX_PB01D_SERCOM5_PAD3
#define EXT3_SPI_SERCOM_DMAC_ID_TX   SERCOM5_DMAC_ID_TX
#define EXT3_SPI_SERCOM_DMAC_ID_RX   SERCOM5_DMAC_ID_RX
/** @} */

/** \name LIN definitions
 *  @{
 */
#define LIN_TX_PIN              PIN_PA06
#define LIN_RX_PIN              PIN_PA07
#define LIN_EN_PIN              PIN_PB22

#define LIN_USART_MODULE              SERCOM0
#define LIN_USART_SERCOM_MUX_SETTING  USART_RX_3_TX_2_XCK_3
#define LIN_USART_SERCOM_PINMUX_PAD0  PINMUX_UNUSED
#define LIN_USART_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define LIN_USART_SERCOM_PINMUX_PAD2  PINMUX_PA06D_SERCOM0_PAD2
#define LIN_USART_SERCOM_PINMUX_PAD3  PINMUX_PA07D_SERCOM0_PAD3

#define LIN_USART_CONFIG(config) \
		config.mux_setting = LIN_USART_SERCOM_MUX_SETTING; \
		config.pinmux_pad0 = EXT1_SPI_SERCOM_PINMUX_PAD0; \
		config.pinmux_pad1 = PINMUX_UNUSED; \
		config.pinmux_pad2 = EXT1_SPI_SERCOM_PINMUX_PAD2; \
		config.pinmux_pad3 = EXT1_SPI_SERCOM_PINMUX_PAD3;

/** @} */

/** \name Embedded debugger I2C interface definitions
 * @{
 */
#define EDBG_I2C_MODULE              SERCOM2
#define EDBG_I2C_SERCOM_PINMUX_PAD0  PINMUX_PA12C_SERCOM2_PAD0
#define EDBG_I2C_SERCOM_PINMUX_PAD1  PINMUX_PA13C_SERCOM2_PAD1
#define EDBG_I2C_SERCOM_DMAC_ID_TX   SERCOM2_DMAC_ID_TX
#define EDBG_I2C_SERCOM_DMAC_ID_RX   SERCOM2_DMAC_ID_RX
/** @} */

/** \name Embedded debugger SPI interface definitions
 * @{
 */
#define EDBG_SPI_MODULE              SERCOM5
#define EDBG_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_E
#define EDBG_SPI_SERCOM_PINMUX_PAD0  PINMUX_PB02D_SERCOM5_PAD0
#define EDBG_SPI_SERCOM_PINMUX_PAD1  PINMUX_PB03D_SERCOM5_PAD1
#define EDBG_SPI_SERCOM_PINMUX_PAD2  PINMUX_PB00D_SERCOM5_PAD2
#define EDBG_SPI_SERCOM_PINMUX_PAD3  PINMUX_PB01D_SERCOM5_PAD3
#define EDBG_SPI_SERCOM_DMAC_ID_TX   SERCOM5_DMAC_ID_TX
#define EDBG_SPI_SERCOM_DMAC_ID_RX   SERCOM5_DMAC_ID_RX
/** @} */

/** \name Embedded debugger CDC Gateway USART interface definitions
 * @{
 */
#define EDBG_CDC_MODULE              SERCOM4
#define EDBG_CDC_SERCOM_MUX_SETTING  USART_RX_3_TX_2_XCK_3
#define EDBG_CDC_SERCOM_PINMUX_PAD0  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_PINMUX_PAD2  PINMUX_PB10D_SERCOM4_PAD2
#define EDBG_CDC_SERCOM_PINMUX_PAD3  PINMUX_PB11D_SERCOM4_PAD3
#define EDBG_CDC_SERCOM_DMAC_ID_TX   SERCOM4_DMAC_ID_TX
#define EDBG_CDC_SERCOM_DMAC_ID_RX   SERCOM4_DMAC_ID_RX
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* SAMC21_XPLAINED_PRO_H_INCLUDED */
