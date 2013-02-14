/**
 * \file
 *
 * \brief SAM4S Xplained Pro board definition and driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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
 */

#ifndef SAM4S_XPLAINED_PRO_H
#define SAM4S_XPLAINED_PRO_H

#include <conf_board.h>

/**
 * \ingroup group_common_boards
 * \defgroup sam4s_xplained_pro_group SAM4S Xplained Pro board
 * @{
 */


/**
 * \defgroup sam4s_xplained_pro_config_group Configuration
 *
 * Symbols to use for configuring the board and its initialization.
 *
 * @{
 */
#ifdef __DOXYGEN__

//! \name Initialization
//@{

/**
 * \def CONF_BOARD_KEEP_WATCHDOG_AT_INIT
 * \brief Let watchdog remain enabled
 *
 * If this symbol is defined, the watchdog is left running with its current
 * configuration. Otherwise, it gets disabled during board initialization.
 */
# ifndef CONF_BOARD_KEEP_WATCHDOG_AT_INIT
#  define CONF_BOARD_KEEP_WATCHDOG_AT_INIT
# endif

//@}

#endif // __DOXYGEN__
/** @} */


/**
 * \defgroup sam4s_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

//! Name string macro
#define BOARD_NAME                "SAM4S_XPLAINED_PRO"

//! \name Resonator definitions
//@{
#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS  (12000000U)
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625
//@}

//! \name LED0 definitions
//@{
#define LED0_PIN                  IOPORT_CREATE_PIN(PIOC, 23)
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE
//@}

//! \name SW0 definitions
//@{
#define SW0_PIN                   IOPORT_CREATE_PIN(PIOA, 2)
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_SUPC_INPUT            2
//@}

/**
 * \name LED #0 definitions
 *
 * Wrapper macros for LED0, to ensure common naming across all Xplained Pro
 * boards.
 */
//@{
#define LED_0_NAME                "LED0 (yellow)"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
//@}

//! Number of on-board LEDs
#define LED_COUNT 1

/**
 * \name Button #0 definitions
 *
 * Wrapper macros for SW0, to ensure common naming across all Xplained Pro
 * boards.
 */
//@{
#define BUTTON_0_NAME             "SW0"
#define BUTTON_0_PIN              SW0_PIN
#define BUTTON_0_ACTIVE           SW0_ACTIVE
#define BUTTON_0_INACTIVE         SW0_INACTIVE
#define BUTTON_0_SUPC_INPUT       SW0_SUPC_INPUT
//@}

//! Number of on-board buttons
#define BUTTON_COUNT              1

//! \name Extension header #1 pin definitions
//@{
#define EXT1_PIN_3                IOPORT_CREATE_PIN(PIOA, 17)
#define EXT1_PIN_4                IOPORT_CREATE_PIN(PIOA, 18)
#define EXT1_PIN_5                IOPORT_CREATE_PIN(PIOA, 24)
#define EXT1_PIN_6                IOPORT_CREATE_PIN(PIOA, 25)
#define EXT1_PIN_7                IOPORT_CREATE_PIN(PIOA, 23)
#define EXT1_PIN_8                IOPORT_CREATE_PIN(PIOA, 19)
#define EXT1_PIN_9                IOPORT_CREATE_PIN(PIOA, 1)
#define EXT1_PIN_10               IOPORT_CREATE_PIN(PIOA, 6)
#define EXT1_PIN_11               IOPORT_CREATE_PIN(PIOA, 3)
#define EXT1_PIN_12               IOPORT_CREATE_PIN(PIOA, 4)
#define EXT1_PIN_13               IOPORT_CREATE_PIN(PIOA, 21)
#define EXT1_PIN_14               IOPORT_CREATE_PIN(PIOA, 22)
#define EXT1_PIN_15               IOPORT_CREATE_PIN(PIOA, 11)
#define EXT1_PIN_16               IOPORT_CREATE_PIN(PIOA, 13)
#define EXT1_PIN_17               IOPORT_CREATE_PIN(PIOA, 12)
#define EXT1_PIN_18               IOPORT_CREATE_PIN(PIOA, 14)
//@}

//! \name Extension header #1 pin definitions by function
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

//! \name Extension header #1 ADC definitions
//@{
#define EXT1_ADC_MODULE           ADC
#define EXT1_ADC_0_CHANNEL        0
#define EXT1_ADC_1_CHANNEL        1
//@}

//! \name Extension header #1 PWM definitions
//@{
#define EXT1_PWM_MODULE           PWM
#define EXT1_PWM_CHANNEL          0
//@}

//! \name Extension header #1 IRQ/External interrupt definitions
//@{
#define EXT1_IRQ_MODULE           SUPC
#define EXT1_IRQ_INPUT            1
//@}

//! \name Extension header #1 TWI definitions
//@{
#define EXT1_TWI_MODULE           TWI0
//@}

//! \name Extension header #1 UART definitions
//@{
#define EXT1_UART_MODULE          USART1
//@}

//! \name Extension header #1 SPI definitions
//@{
#define EXT1_SPI_MODULE           SPI
//@}

//! \name Extension header #2 pin definitions
//@{
#define EXT2_PIN_3                IOPORT_CREATE_PIN(PIOB, 0)
#define EXT2_PIN_4                IOPORT_CREATE_PIN(PIOB, 1)
#define EXT2_PIN_5                IOPORT_CREATE_PIN(PIOC, 24)
#define EXT2_PIN_6                IOPORT_CREATE_PIN(PIOC, 25)
#define EXT2_PIN_7                IOPORT_CREATE_PIN(PIOC, 19)
#define EXT2_PIN_8                IOPORT_CREATE_PIN(PIOA, 20)
#define EXT2_PIN_9                IOPORT_CREATE_PIN(PIOC, 26)
#define EXT2_PIN_10               IOPORT_CREATE_PIN(PIOC, 27)
#define EXT2_PIN_11               IOPORT_CREATE_PIN(PIOA, 3)
#define EXT2_PIN_12               IOPORT_CREATE_PIN(PIOA, 4)
#define EXT2_PIN_13               IOPORT_CREATE_PIN(PIOA, 21)
#define EXT2_PIN_14               IOPORT_CREATE_PIN(PIOA, 22)
#define EXT2_PIN_15               IOPORT_CREATE_PIN(PIOA, 9)
#define EXT2_PIN_16               IOPORT_CREATE_PIN(PIOA, 13)
#define EXT2_PIN_17               IOPORT_CREATE_PIN(PIOA, 12)
#define EXT2_PIN_18               IOPORT_CREATE_PIN(PIOA, 14)
//@}

//! \name Extension header #2 pin definitions by function
//@{
#define EXT2_PIN_ADC_0            EXT2_PIN_3
#define EXT2_PIN_ADC_1            EXT2_PIN_4
#define EXT2_PIN_GPIO_0           EXT2_PIN_5
#define EXT2_PIN_GPIO_1           EXT2_PIN_6
#define EXT2_PIN_PWM_0            EXT2_PIN_7
#define EXT2_PIN_PWM_1            EXT2_PIN_8
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

//! \name Extension header #2 ADC definitions
//@{
#define EXT2_ADC_MODULE           ADC
#define EXT2_ADC_0_CHANNEL        4
#define EXT2_ADC_1_CHANNEL        5
//@}

//! \name Extension header #2 PWM definitions
//@{
#define EXT2_PWM_MODULE           PWM
#define EXT2_PWM_CHANNEL          1
//@}

//! \name Extension header #2 TWI definitions
//@{
#define EXT2_TWI_MODULE           TWI0
//@}

//! \name Extension header #2 UART definitions
//@{
#define EXT2_UART_MODULE          USART1
//@}

//! \name Extension header #2 SPI definitions
//@{
#define EXT2_SPI_MODULE           SPI
//@}

//! \name Extension header #3 pin definitions
//@{
#define EXT3_PIN_3                IOPORT_CREATE_PIN(PIOC, 29)
#define EXT3_PIN_4                IOPORT_CREATE_PIN(PIOC, 30)
#define EXT3_PIN_5                IOPORT_CREATE_PIN(PIOC, 21)
#define EXT3_PIN_6                IOPORT_CREATE_PIN(PIOC, 22)
#define EXT3_PIN_7                IOPORT_CREATE_PIN(PIOC, 20)
#define EXT3_PIN_8                IOPORT_CREATE_PIN(PIOA, 16)
#define EXT3_PIN_9                IOPORT_CREATE_PIN(PIOA, 0)
#define EXT3_PIN_10               IOPORT_CREATE_PIN(PIOC, 31)
#define EXT3_PIN_11               IOPORT_CREATE_PIN(PIOB, 4)
#define EXT3_PIN_12               IOPORT_CREATE_PIN(PIOB, 5)
#define EXT3_PIN_13               IOPORT_CREATE_PIN(PIOB, 2)
#define EXT3_PIN_14               IOPORT_CREATE_PIN(PIOB, 3)
#define EXT3_PIN_15               IOPORT_CREATE_PIN(PIOA, 10)
#define EXT3_PIN_16               IOPORT_CREATE_PIN(PIOA, 13)
#define EXT3_PIN_17               IOPORT_CREATE_PIN(PIOA, 12)
#define EXT3_PIN_18               IOPORT_CREATE_PIN(PIOA, 14)
//@}

//! \name Extension header #3 pin definitions by function
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

//! \name Extension header #3 ADC definitions
//@{
#define EXT3_ADC_MODULE           ADC
#define EXT3_ADC_0_CHANNEL        13
#define EXT3_ADC_1_CHANNEL        14
//@}

//! \name Extension header #3 PWM definitions
//@{
#define EXT3_PWM_MODULE           PWM
#define EXT3_PWM_CHANNEL          2
//@}

//! \name Extension header #3 IRQ/External interrupt definitions
//@{
#define EXT3_IRQ_MODULE           SUPC
#define EXT3_IRQ_INPUT            0
//@}

//! \name Extension header #3 TWI definitions
//@{
#define EXT3_TWI_MODULE           TWI1
//@}

//! \name Extension header #3 UART definitions
//@{
#define EXT3_UART_MODULE          UART1
//@}

//! \name Extension header #3 SPI definitions
//@{
#define EXT3_SPI_MODULE           SPI
//@}

//! \name Embedded debugger GPIO interface definitions
//@{
#define EDBG_GPIO0_PIN            IOPORT_CREATE_PIN(PIOA, 6)
#define EDBG_GPIO1_PIN            IOPORT_CREATE_PIN(PIOC, 22)
#define EDBG_GPIO2_PIN            IOPORT_CREATE_PIN(PIOC, 24)
#define EDBG_GPIO3_PIN            IOPORT_CREATE_PIN(PIOC, 25)
//@}

//! \name Embedded debugger USART interface definitions
//@{
#define EDBG_UART_MODULE          UART1
//@}

//! \name Embedded debugger TWI interface definitions
//@{
#define EDBG_TWI_MODULE           TWI0
//@}

//! \name Embedded debugger SPI interface definitions
//@{
#define EDBG_SPI_MODULE           SPI
//@}

#define CONSOLE_UART              UART1

/** UART1 pins (UTXD1 and URXD1) definitions, PB2,PB3. */
#define PINS_UART1                (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1)
#define PINS_UART1_FLAGS          (PIO_PERIPH_A | PIO_DEFAULT)
#define PINS_UART1_PIO            PIOB

#define PIN_PUSHBUTTON_1_MASK     PIO_PA2
#define PIN_PUSHBUTTON_1_PIO      PIOB
#define PIN_PUSHBUTTON_1_ID       ID_PIOB
#define PIN_PUSHBUTTON_1_TYPE     PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR     PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

/** @} */

/** @} */

#endif /* SAM4S_XPLAINED_PRO_H */
