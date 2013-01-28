/**
 * \file
 *
 * \brief SAM4L Xplained PRO Board header file.
 *
 * This file contains definitions and services related to the features of the
 * SAM4L Xplained PRO Board.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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
#ifndef SAM4L_XPLAINED_PRO_INCLUDED
#define SAM4L_XPLAINED_PRO_INCLUDED

/**
 * \defgroup sam4l_xplained_pro_group SAM4L Xplained PRO Board
 * @{
 */

/**
 * \defgroup sam4l_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** Name string macro */
#define BOARD_NAME "SAM4L_XPLAINED_PRO"

/** \name Board oscillator configuration */
//@{

/** Osc frequency (Hz.) and startup time (RCOsc periods). */
#define FOSC0                     (12000000)

/** Osc32 frequency (Hz.) and startup time (RCOsc periods). */
#define FOSC32                    (32768)

#define BOARD_OSC32_IS_XTAL       true
#define BOARD_OSC32_HZ            FOSC32
#define BOARD_OSC32_STARTUP_US    (71000)
#define BOARD_OSC32_SELCURR       BSCIF_OSCCTRL32_SELCURR(10)
#define BOARD_OSC0_IS_XTAL        true
#define BOARD_OSC0_HZ             FOSC0
#define BOARD_OSC0_STARTUP_US     (1100)
//@}

/*! \name Number of LEDs.
 */
//! @{
#define LED_COUNT   1
//! @}

/**
 * \name LEDs
 *
 * LED0 is a single yellow LED that is active low.
 */
//@{
#define LED0                          PIN_PC07
//@}

/*! \name GPIO Connections of Push Buttons
 */
//! @{
#define GPIO_PUSH_BUTTON_0              PIN_PC24
#define GPIO_PUSH_BUTTON_0_MASK         GPIO_PC24
#define PUSH_BUTTON_0_DOWN_LEVEL        IOPORT_PIN_LEVEL_LOW
#define PUSH_BUTTON_0_UP_LEVEL          IOPORT_PIN_LEVEL_HIGH
//! @}

/** \name Push button connection of external interrupt line */
//@{
#define BUTTON_0_EIC_PIN          PIN_PC24B_EIC_EXTINT1
#define BUTTON_0_EIC_PIN_MUX      MUX_PC24B_EIC_EXTINT1
#define BUTTON_0_EIC_LINE         1
//@}

/** Number of on-board buttons */
#define BUTTON_COUNT 1

/** \name GPIO Connections of touch sensor */
//@{
#define QTOUCH_DISCHARGE_PIN      PIN_PC14
#define QTOUCH_DISCHARGE_MUX      MUX_PC14_CATB_DIS

#define QTOUCH_BUTTON_PIN         PIN_PC13
#define QTOUCH_BUTTON_MUX         MUX_PB04G_CATB_SENSE15
//@}

/** \name Touch sensors pin assignements */
//@{
#define QTOUCH_PINSEL_BUTTON      15
//@}

/** \name GPIO Connections of VBUS monitoring */
//@{
#define VBUS_INPUT_PIN            PIN_PC11
//@}

/** \name Extension header 1 definitions */
//@{
/** \name Pin definitions */
//@{
#define EXT1_PIN_3                PIN_PA04
#define EXT1_PIN_4                PIN_PA05
#define EXT1_PIN_5                PIN_PB12
#define EXT1_PIN_6                PIN_PC02
#define EXT1_PIN_7                PIN_PC00
#define EXT1_PIN_8                PIN_PC01
#define EXT1_PIN_9                PIN_PC25
#define EXT1_PIN_10               PIN_PB13
#define EXT1_PIN_11               PIN_PA23
#define EXT1_PIN_12               PIN_PA24
#define EXT1_PIN_13               PIN_PB00
#define EXT1_PIN_14               PIN_PB01
#define EXT1_PIN_15               PIN_PC03
#define EXT1_PIN_16               PIN_PA22
#define EXT1_PIN_17               PIN_PA21
#define EXT1_PIN_18               PIN_PC30
//@}

/** \name Pins according to function */
//@{
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
//@}

/** \name Module-specific functions */
//@{
/** \name ADC */
//@{
#define EXT1_ADC_MODULE           ADCIFE
#define EXT1_ADC_0_CHANNEL        0
#define EXT1_ADC_0_MUX            MUX_PA04A_ADCIFE_AD0
#define EXT1_ADC_1_CHANNEL        1
#define EXT1_ADC_1_MUX            MUX_PA05A_ADCIFE_AD1
//@}

/** \name PWM */
//@{
#define EXT1_PWM_MODULE           TC1
#define EXT1_PWM_0_MUX            MUX_PC00D_TC1_A0
#define EXT1_PWM_1_MUX            MUX_PC01D_TC1_B0
// Define TC used here (TC A0/B0)
//@}

/** \name IRQ/External interrupt */
//@{
#define EXT1_IRQ_MODULE           EIC
#define EXT1_IRQ_MUX              MUX_PC25B_EIC_EXTINT2
#define EXT1_IRQ_MASK             EIC_IMR_INT2
//@}

/** \name TWI */
//@{
#define EXT1_TWI_MODULE           TWIMS0
#define EXT1_TWI_SDA_MUX          MUX_PA23B_TWIMS0_TWD
#define EXT1_TWI_SCL_MUX          MUX_PA24B_TWIMS0_TWCK
//@}

/** \name UART */
//@{
#define EXT1_UART_MODULE          USART0
#define EXT1_UART_RX_MUX          MUX_PB00B_USART0_RXD
#define EXT1_UART_TX_MUX          MUX_PB01B_USART0_TXD
//@}

/** \name SPI */
//@{
#define EXT1_SPI_MODULE           SPI
#define EXT1_SPI_SS_1_MUX         MUX_PB13B_SPI_NPCS1
#define EXT1_SPI_SS_0_MUX         MUX_PC03A_SPI_NPCS0
#define EXT1_SPI_SS_MOSI          MUX_PA22A_SPI_MOSI
#define EXT1_SPI_SS_MISO          MUX_PA21A_SPI_MISO
#define EXT1_SPI_SS_SCK           MUX_PC30B_SPI_SCK
//@}
//@}
//@}

/** \name Extension header 2 definitions */
//@{
/** \name Pin definitions */
//@{
#define EXT2_PIN_3                PIN_PA07
#define EXT2_PIN_4                PIN_PB02
#define EXT2_PIN_5                PIN_PC08
#define EXT2_PIN_6                PIN_PB10
#define EXT2_PIN_7                PIN_PC04
#define EXT2_PIN_8                PIN_PC05
#define EXT2_PIN_9                PIN_PC06
#define EXT2_PIN_10               PIN_PC09
#define EXT2_PIN_11               PIN_PB14
#define EXT2_PIN_12               PIN_PB15
#define EXT2_PIN_13               PIN_PC26
#define EXT2_PIN_14               PIN_PC27
#define EXT2_PIN_15               PIN_PB11
#define EXT2_PIN_16               PIN_PA22
#define EXT2_PIN_17               PIN_PA21
#define EXT2_PIN_18               PIN_PC30
//@}

/** \name Pins according to function */
//@{
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
//@}

/** \name Module-specific functions */
//@{
/** \name ADC */
//@{
#define EXT2_ADC_MODULE           ADCIFE
#define EXT2_ADC_0_CHANNEL        2
#define EXT2_ADC_0_MUX            MUX_PA07A_ADCIFE_AD2
#define EXT2_ADC_1_CHANNEL        3
#define EXT2_ADC_1_MUX            MUX_PB02A_ADCIFE_AD3
//@}

/** \name PWM */
//@{
#define EXT2_PWM_MODULE           TC1
#define EXT2_PWM_0_MUX            MUX_PC04D_TC1_A2
#define EXT2_PWM_1_MUX            MUX_PC05D_TC1_B2
// Define TC used here (TC A2/B2)
//@}

/** \name IRQ/External interrupt */
//@{
#define EXT2_IRQ_MODULE           EIC
#define EXT2_IRQ_MUX              MUX_PC06B_EIC_EXTINT8
#define EXT2_IRQ_MASK             EIC_IMR_INT8
//@}

/** \name TWI */
//@{
#define EXT2_TWI_MODULE           TWIMS3
#define EXT2_TWI_SDA_MUX          MUX_PB14C_TWIMS3_TWD
#define EXT2_TWI_SCL_MUX          MUX_PB15C_TWIMS3_TWCK
//@}

/** \name UART */
//@{
#define EXT2_UART_MODULE          USART1
#define EXT2_UART_RX_MUX          MUX_PC26A_USART1_RXD
#define EXT2_UART_TX_MUX          MUX_PC27A_USART1_TXD
//@}

/** \name SPI */
//@{
#define EXT2_SPI_MODULE           SPI
#define EXT2_SPI_SS_0_MUX         MUX_PB11B_SPI_NPCS2
#define EXT2_SPI_SS_MOSI          MUX_PA22A_SPI_MOSI
#define EXT2_SPI_SS_MISO          MUX_PA21A_SPI_MISO
#define EXT2_SPI_SS_SCK           MUX_PC30B_SPI_SCK
//@}
//@}
//@}

/** \name Extension header 3 definitions */
//@{
/** \name Pin definitions */
//@{
#define EXT3_PIN_3                PIN_PB03
#define EXT3_PIN_4                PIN_PB04
#define EXT3_PIN_5                PIN_PC15
#define EXT3_PIN_6                PIN_PC16
#define EXT3_PIN_7                PIN_PA08
#define EXT3_PIN_8                PIN_PA09
#define EXT3_PIN_9                PIN_PA06
#define EXT3_PIN_10               PIN_PA10
#define EXT3_PIN_11               PIN_PB14
#define EXT3_PIN_12               PIN_PB15
#define EXT3_PIN_13               PIN_PC26
#define EXT3_PIN_14               PIN_PC27
#define EXT3_PIN_15               PIN_PA17
#define EXT3_PIN_16               PIN_PA20
#define EXT3_PIN_17               PIN_PA19
#define EXT3_PIN_18               PIN_PA18
//@}

/** \name Pins according to function */
//@{
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
//@}

/** \name Module-specific functions */
//@{
/** \name ADC */
//@{
#define EXT3_ADC_MODULE           ADCIFE
#define EXT3_ADC_0_CHANNEL        4
#define EXT3_ADC_0_MUX            MUX_PB03A_ADCIFE_AD4
#define EXT3_ADC_1_CHANNEL        5
#define EXT3_ADC_1_MUX            MUX_PB04A_ADCIFE_AD5
//@}

/** \name PWM */
//@{
#define EXT3_PWM_MODULE           TC0
#define EXT3_PWM_0_MUX            MUX_PA08B_TC0_A0
#define EXT3_PWM_1_MUX            MUX_PA09B_TC0_B0
// Define TC used here (TC A0/B0)
//@}

/** \name IRQ/External interrupt */
//@{
#define EXT3_IRQ_MODULE           EIC
#define EXT3_IRQ_MUX              MUX_PA06C_EIC_EXTINT1
#define EXT3_IRQ_MASK             EIC_IMR_INT1
//@}

/** \name TWI */
//@{
#define EXT3_TWI_MODULE           TWIMS3
#define EXT3_TWI_SDA_MUX          MUX_PB14C_TWIMS3_TWD
#define EXT3_TWI_SCL_MUX          MUX_PB15C_TWIMS3_TWCK
//@}

/** \name UART */
//@{
#define EXT3_UART_MODULE          USART1
#define EXT3_UART_RX_MUX          MUX_PC26A_USART1_RXD
#define EXT3_UART_TX_MUX          MUX_PC27A_USART1_TXD
//@}

/** \name SPI */
//@{
#define EXT3_SPI_MODULE           USART2
#define EXT3_SPI_SS_MOSI          MUX_PA20A_USART2_TXD
#define EXT3_SPI_SS_MISO          MUX_PA19A_USART2_RXD
#define EXT3_SPI_SS_SCK           MUX_PA18A_USART2_CLK
//@}
//@}
//@}

/** \name Extension header 4 definitions */
//@{
/** \name Pin definitions */
//@{
#define EXT4_PIN_3                PIN_PB05
#define EXT4_PIN_4                PIN_PC10
#define EXT4_PIN_5                PIN_PC17
#define EXT4_PIN_6                PIN_PC18
#define EXT4_PIN_7                PIN_PA12
#define EXT4_PIN_8                PIN_PA13
#define EXT4_PIN_9                PIN_PA16
#define EXT4_PIN_10               PIN_PA15
#define EXT4_PIN_11               PIN_PB14
#define EXT4_PIN_12               PIN_PB15
#define EXT4_PIN_13               PIN_PB00
#define EXT4_PIN_14               PIN_PB01
#define EXT4_PIN_15               PIN_PA11
#define EXT4_PIN_16               PIN_PA20
#define EXT4_PIN_17               PIN_PA19
#define EXT4_PIN_18               PIN_PA18
//@}

/** \name Pins according to function */
//@{
#define EXT4_PIN_ADC_0            EXT4_PIN_3
#define EXT4_PIN_ADC_1            EXT4_PIN_4
#define EXT4_PIN_GPIO_0           EXT4_PIN_5
#define EXT4_PIN_GPIO_1           EXT4_PIN_6
#define EXT4_PIN_PWM_0            EXT4_PIN_7
#define EXT4_PIN_PWM_1            EXT4_PIN_8
#define EXT4_PIN_IRQ              EXT4_PIN_9
#define EXT4_PIN_TWI_SDA          EXT4_PIN_11
#define EXT4_PIN_TWI_SCL          EXT4_PIN_12
#define EXT4_PIN_UART_RX          EXT4_PIN_13
#define EXT4_PIN_UART_TX          EXT4_PIN_14
#define EXT4_PIN_SPI_SS_1         EXT4_PIN_10
#define EXT4_PIN_SPI_SS_0         EXT4_PIN_15
#define EXT4_PIN_SPI_MOSI         EXT4_PIN_16
#define EXT4_PIN_SPI_MISO         EXT4_PIN_17
#define EXT4_PIN_SPI_SCK          EXT4_PIN_18
//@}

/** \name Module-specific functions */
//@{
/** \name ADC */
//@{
#define EXT4_ADC_MODULE           ADCIFE
#define EXT4_ADC_0_CHANNEL        6
#define EXT4_ADC_0_MUX            MUX_PB05A_ADCIFE_AD6
#define EXT4_ADC_1_CHANNEL        10
#define EXT4_ADC_1_MUX            MUX_PC10A_ADCIFE_AD10
//@}

/** \name PWM */
//@{
#define EXT4_PWM_MODULE           TC0
#define EXT4_PWM_0_MUX            MUX_PA12B_TC0_A2
#define EXT4_PWM_1_MUX            MUX_PA13B_TC0_B2
// Define TC used here (TC A2/B2)
//@}

/** \name IRQ/External interrupt */
//@{
#define EXT4_IRQ_MODULE           EIC
#define EXT4_IRQ_MUX              MUX_PA16C_EIC_EXTINT1
#define EXT4_IRQ_MASK             EIC_IMR_INT1
//@}

/** \name TWI */
//@{
#define EXT4_TWI_MODULE           TWIMS3
#define EXT4_TWI_SDA_MUX          MUX_PB14C_TWIMS3_TWD
#define EXT4_TWI_SCL_MUX          MUX_PB15C_TWIMS3_TWCK
//@}

/** \name UART */
//@{
#define EXT4_UART_MODULE          USART0
#define EXT4_UART_RX_MUX          MUX_PB00B_USART0_RXD
#define EXT4_UART_TX_MUX          MUX_PB01B_USART0_TXD
//@}

/** \name SPI */
//@{
#define EXT4_SPI_MODULE           USART2
#define EXT4_SPI_SS_MOSI          MUX_PA20A_USART2_TXD
#define EXT4_SPI_SS_MISO          MUX_PA19A_USART2_RXD
#define EXT4_SPI_SS_SCK           MUX_PA18A_USART2_CLK
//@}
//@}
//@}

//@}
//@}
#endif /* SAM4L_XPLAINED_PRO_INCLUDED */
