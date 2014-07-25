/**
 * \file
 *
 * \brief SAM L21 Xplained Pro board definition
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

#ifndef SAML21_XPLAINED_PRO_H_INCLUDED
#define SAML21_XPLAINED_PRO_H_INCLUDED

#include <conf_board.h>
#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup saml21_xplained_pro_group SAM L21 Xplained Pro board
 *
 * @{
 */

void system_board_init(void);

/**
 * \defgroup saml21_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** Name string macro */
#define BOARD_NAME                "SAML21_XPLAINED_PRO"

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
#define LED0_PIN                  PIN_PB10
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name SW0 definitions
 *  @{ */
#define SW0_PIN                   PIN_PA16
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA16A_EIC_EXTINT0
#define SW0_EIC_MUX               MUX_PA16A_EIC_EXTINT0
#define SW0_EIC_PINMUX            PINMUX_PA16A_EIC_EXTINT0
#define SW0_EIC_LINE              0
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
#define EXT1_PIN_3                PIN_PA02
#define EXT1_PIN_4                PIN_PA03
#define EXT1_PIN_5                PIN_PB06
#define EXT1_PIN_6                PIN_PB07
#define EXT1_PIN_7                PIN_PA12
#define EXT1_PIN_8                PIN_PA13
#define EXT1_PIN_9                PIN_PB04
#define EXT1_PIN_10               PIN_PB05
#define EXT1_PIN_11               PIN_PA08
#define EXT1_PIN_12               PIN_PA09
#define EXT1_PIN_13               PIN_PB09
#define EXT1_PIN_14               PIN_PB08
#define EXT1_PIN_15               PIN_PA05
#define EXT1_PIN_16               PIN_PA06
#define EXT1_PIN_17               PIN_PA04
#define EXT1_PIN_18               PIN_PA07
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

/** \name Extension header #1 ADC definitions
 *  @{
 */
#define EXT1_ADC_MODULE           ADC
#define EXT1_ADC_0_CHANNEL        0
#define EXT1_ADC_0_PIN            PIN_PA02B_ADC_AIN0
#define EXT1_ADC_0_MUX            MUX_PA02B_ADC_AIN0
#define EXT1_ADC_0_PINMUX         PINMUX_PA02B_ADC_AIN0
#define EXT1_ADC_1_CHANNEL        1
#define EXT1_ADC_1_PIN            PIN_PA03B_ADC_AIN1
#define EXT1_ADC_1_MUX            MUX_PA03B_ADC_AIN1
#define EXT1_ADC_1_PINMUX         PINMUX_PA03B_ADC_AIN1
/** @} */

/** \name Extension header #1 PWM definitions
 *  @{
 */
#define EXT1_PWM_MODULE           TCC2
#define EXT1_PWM_0_CHANNEL        0
#define EXT1_PWM_0_PIN            PIN_PA12E_TCC2_WO0
#define EXT1_PWM_0_MUX            MUX_PA12E_TCC2_WO0
#define EXT1_PWM_0_PINMUX         PINMUX_PA12E_TCC2_WO0
#define EXT1_PWM_1_CHANNEL        1
#define EXT1_PWM_1_PIN            PIN_PA13E_TCC2_WO1
#define EXT1_PWM_1_MUX            MUX_PA13E_TCC2_WO1
#define EXT1_PWM_1_PINMUX         PINMUX_PA13E_TCC2_WO1
/** @} */

/** \name Extension header #1 IRQ/External interrupt definitions
 *  @{
 */
#define EXT1_IRQ_MODULE           EIC
#define EXT1_IRQ_INPUT            4
#define EXT1_IRQ_PIN              PIN_PB04A_EIC_EXTINT4
#define EXT1_IRQ_MUX              MUX_PB04A_EIC_EXTINT4
#define EXT1_IRQ_PINMUX           PINMUX_PB04A_EIC_EXTINT4
/** @} */

/** \name Extension header #1 I2C definitions
 *  @{
 */
#define EXT1_I2C_MODULE              SERCOM2
#define EXT1_I2C_SERCOM_PINMUX_PAD0  PINMUX_PA08D_SERCOM2_PAD0
#define EXT1_I2C_SERCOM_PINMUX_PAD1  PINMUX_PA09D_SERCOM2_PAD1
#define EXT1_I2C_SERCOM_DMAC_ID_TX   SERCOM2_DMAC_ID_TX
#define EXT1_I2C_SERCOM_DMAC_ID_RX   SERCOM2_DMAC_ID_RX
/** @} */

/** \name Extension header #2 pin definitions
 *  @{
 */
#define EXT2_PIN_3                PIN_PA10
#define EXT2_PIN_4                PIN_PA11
#define EXT2_PIN_5                PIN_PA20
#define EXT2_PIN_6                PIN_PA21
#define EXT2_PIN_7                PIN_PB12
#define EXT2_PIN_8                PIN_PB13
#define EXT2_PIN_9                PIN_PB14
#define EXT2_PIN_10               PIN_PB15
#define EXT2_PIN_11               PIN_PA08
#define EXT2_PIN_12               PIN_PA09
#define EXT2_PIN_13               PIN_PA19
#define EXT2_PIN_14               PIN_PA18
#define EXT2_PIN_15               PIN_PA17
#define EXT2_PIN_16               PIN_PB22
#define EXT2_PIN_17               PIN_PB16
#define EXT2_PIN_18               PIN_PB23
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

/** \name Extension header #3 pin definitions
 *  @{
 */
#define EXT3_PIN_3                PIN_PB00
#define EXT3_PIN_4                PIN_PB01
#define EXT3_PIN_5                PIN_PB30
#define EXT3_PIN_6                PIN_PA15
#define EXT3_PIN_7                PIN_PB10
#define EXT3_PIN_8                PIN_PB11
#define EXT3_PIN_9                PIN_PA16
#define EXT3_PIN_10               PIN_PA27
#define EXT3_PIN_11               PIN_PA08
#define EXT3_PIN_12               PIN_PA09
#define EXT3_PIN_13               PIN_PA19
#define EXT3_PIN_14               PIN_PA18
#define EXT3_PIN_15               PIN_PB17
#define EXT3_PIN_16               PIN_PB22
#define EXT3_PIN_17               PIN_PB16
#define EXT3_PIN_18               PIN_PB23
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


/**
 * \brief Turns off the specified LEDs.
 *
 * \param led_gpio LED to turn off (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Off(led_gpio)     port_pin_set_output_level(led_gpio,true)

/**
 * \brief Turns on the specified LEDs.
 *
 * \param led_gpio LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_On(led_gpio)      port_pin_set_output_level(led_gpio,false)

/**
 * \brief Toggles the specified LEDs.
 *
 * \param led_gpio LED to toggle (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Toggle(led_gpio)  port_pin_toggle_output_level(led_gpio)

/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* SAML21_XPLAINED_PRO_H_INCLUDED */
