/**
 * \file
 *
 * \brief SAM4N Xplained Pro board definition and driver
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#ifndef SAM4N_XPLAINED_PRO_H
#define SAM4N_XPLAINED_PRO_H

#include <conf_board.h>

/**
 * \ingroup group_common_boards
 * \defgroup sam4n_xplained_pro_group SAM4N Xplained Pro board
 * @{
 */


/**
 * \defgroup sam4n_xplained_pro_config_group Configuration
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
 * \defgroup sam4n_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

//! Name string macro
#define BOARD_NAME                "SAM4N_XPLAINED_PRO"

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
#define LED0_GPIO                  IOPORT_CREATE_PIN(PIOB, 14)
#define LED0_ACTIVE_LEVEL               false
#define LED0_INACTIVE_LEVEL             !LED0_ACTIVE_LEVEL
//@}

//! \name SW0 definitions
//@{
#define SW0_PIN                   IOPORT_CREATE_PIN(PIOA, 30)
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
#define LED_0_PIN                 LED0_GPIO
#define LED_0_ACTIVE              LED0_ACTIVE_LEVEL
#define LED_0_INACTIVE            LED0_INACTIVE_LEVEL

#define PIN_LED_0      {PIO_PB14, PIOB, ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_0_MASK PIO_PB14
#define PIN_LED_0_PIO  PIOB
#define PIN_LED_0_ID   ID_PIOB
#define PIN_LED_0_TYPE PIO_OUTPUT_1
#define PIN_LED_0_ATTR PIO_DEFAULT
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

#define PUSHBUTTON_1_NAME    "SW0"
#define PIN_PUSHBUTTON_1 {PIO_PA30, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_1_MASK PIO_PA30
#define PIN_PUSHBUTTON_1_PIO  PIOA
#define PIN_PUSHBUTTON_1_ID   ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE
//@}

//! Number of on-board buttons
#define BUTTON_COUNT              1

//! \name Extension header #1 pin definitions
//@{
#define EXT1_PIN_3                IOPORT_CREATE_PIN(PIOA, 17)
#define EXT1_PIN_4                IOPORT_CREATE_PIN(PIOA, 18)
#define EXT1_PIN_5                IOPORT_CREATE_PIN(PIOA, 24)
#define EXT1_PIN_6                IOPORT_CREATE_PIN(PIOA, 25)
#define EXT1_PIN_7                IOPORT_CREATE_PIN(PIOC, 23)
#define EXT1_PIN_8                IOPORT_CREATE_PIN(PIOC, 24)
#define EXT1_PIN_9                IOPORT_CREATE_PIN(PIOA, 0)
#define EXT1_PIN_10               IOPORT_CREATE_PIN(PIOA, 23)
#define EXT1_PIN_11               IOPORT_CREATE_PIN(PIOA, 3)
#define EXT1_PIN_12               IOPORT_CREATE_PIN(PIOA, 4)
#define EXT1_PIN_13               IOPORT_CREATE_PIN(PIOA, 21)
#define EXT1_PIN_14               IOPORT_CREATE_PIN(PIOA, 22)
#define EXT1_PIN_15               IOPORT_CREATE_PIN(PIOA, 11)
#define EXT1_PIN_16               IOPORT_CREATE_PIN(PIOA, 13)
#define EXT1_PIN_17               IOPORT_CREATE_PIN(PIOA, 12)
#define EXT1_PIN_18               IOPORT_CREATE_PIN(PIOA, 14)
//@}

//! \name Extension header #2 pin definitions
//@{
#define EXT2_PIN_3                IOPORT_CREATE_PIN(PIOA, 19)
#define EXT2_PIN_4                IOPORT_CREATE_PIN(PIOA, 20)
#define EXT2_PIN_5                IOPORT_CREATE_PIN(PIOC, 0)
#define EXT2_PIN_6                IOPORT_CREATE_PIN(PIOC, 1)
#define EXT2_PIN_7                IOPORT_CREATE_PIN(PIOC, 26)
#define EXT2_PIN_8                IOPORT_CREATE_PIN(PIOC, 27)
#define EXT2_PIN_9                IOPORT_CREATE_PIN(PIOA, 1)
#define EXT2_PIN_10               IOPORT_CREATE_PIN(PIOC, 8)
#define EXT2_PIN_11               IOPORT_CREATE_PIN(PIOB, 0)
#define EXT2_PIN_12               IOPORT_CREATE_PIN(PIOB, 1)
#define EXT2_PIN_13               IOPORT_CREATE_PIN(PIOA, 5)
#define EXT2_PIN_14               IOPORT_CREATE_PIN(PIOA, 6)
#define EXT2_PIN_15               IOPORT_CREATE_PIN(PIOC, 4)
#define EXT2_PIN_16               IOPORT_CREATE_PIN(PIOA, 13)
#define EXT2_PIN_17               IOPORT_CREATE_PIN(PIOA, 12)
#define EXT2_PIN_18               IOPORT_CREATE_PIN(PIOA, 14)
//@}

//! \name Extension header #3 pin definitions
//@{
#define EXT3_PIN_3                IOPORT_CREATE_PIN(PIOC, 29)
#define EXT3_PIN_4                IOPORT_CREATE_PIN(PIOC, 30)
#define EXT3_PIN_5                IOPORT_CREATE_PIN(PIOC, 2)
#define EXT3_PIN_6                IOPORT_CREATE_PIN(PIOC, 3)
#define EXT3_PIN_7                IOPORT_CREATE_PIN(PIOA, 26)
#define EXT3_PIN_8                IOPORT_CREATE_PIN(PIOA, 27)
#define EXT3_PIN_9                IOPORT_CREATE_PIN(PIOB, 2)
#define EXT3_PIN_10               IOPORT_CREATE_PIN(PIOB, 3)
#define EXT3_PIN_11               IOPORT_CREATE_PIN(PIOA, 3)
#define EXT3_PIN_12               IOPORT_CREATE_PIN(PIOA, 4)
#define EXT3_PIN_13               IOPORT_CREATE_PIN(PIOA, 9)
#define EXT3_PIN_14               IOPORT_CREATE_PIN(PIOA, 10)
#define EXT3_PIN_15               IOPORT_CREATE_PIN(PIOC, 7)
#define EXT3_PIN_16               IOPORT_CREATE_PIN(PIOA, 13)
#define EXT3_PIN_17               IOPORT_CREATE_PIN(PIOA, 12)
#define EXT3_PIN_18               IOPORT_CREATE_PIN(PIOA, 14)
//@}

//! \name Embedded debugger GPIO interface definitions
//@{
#define EDBG_GPIO0_PIN            IOPORT_CREATE_PIN(PIOC, 21)
#define EDBG_GPIO1_PIN            IOPORT_CREATE_PIN(PIOC, 20)
#define EDBG_GPIO2_PIN            IOPORT_CREATE_PIN(PIOC, 19)
#define EDBG_GPIO3_PIN            IOPORT_CREATE_PIN(PIOC, 18)
//@}

//! \name Embedded debugger USART interface definitions
//@{
#define EDBG_UART_MODULE          USART2
//@}

//! \name Embedded debugger TWI interface definitions
//@{
#define EDBG_TWI_MODULE           TWI2
//@}

#define CONSOLE_UART              UART3
#define CONSOLE_UART_ID        ID_UART3

/** UART3 pins (UTXD3 and URXD3) definitions, PB10,11. */
#define PINS_UART3        (PIO_PB10B_URXD3 | PIO_PB11B_UTXD3)
#define PINS_UART3_FLAGS  (IOPORT_MODE_MUX_B)

#define PINS_UART3_PORT   IOPORT_PIOB
#define PINS_UART3_MASK   (PIO_PB10B_URXD3 | PIO_PB11B_UTXD3)
#define PINS_UART3_PIO    PIOB
#define PINS_UART3_ID     ID_PIOB
#define PINS_UART3_TYPE   PIO_PERIPH_B
#define PINS_UART3_ATTR   PIO_DEFAULT

#define PIN_TC0_TIOA0         (PIO_PA0_IDX)
#define PIN_TC0_TIOA0_MUX     (IOPORT_MODE_MUX_B)
#define PIN_TC0_TIOA0_FLAGS   (PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_TC0_TIOA1         (PIO_PA15_IDX)
#define PIN_TC0_TIOA1_MUX     (IOPORT_MODE_MUX_B)
#define PIN_TC0_TIOA1_FLAGS   (PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_TC0_TIOA1_PIO     PIOA
#define PIN_TC0_TIOA1_MASK    PIO_PA15
#define PIN_TC0_TIOA1_ID      ID_PIOA
#define PIN_TC0_TIOA1_TYPE    PIO_PERIPH_B
#define PIN_TC0_TIOA1_ATTR    PIO_DEFAULT

#define PIN_TC0_TIOA2         (PIO_PA26_IDX)
#define PIN_TC0_TIOA2_MUX     (IOPORT_MODE_MUX_B)
#define PIN_TC0_TIOA2_FLAGS   (PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_TC0_TIOA2_PIO     PIOA
#define PIN_TC0_TIOA2_MASK    PIO_PA26
#define PIN_TC0_TIOA2_ID      ID_PIOA
#define PIN_TC0_TIOA2_TYPE    PIO_INPUT
#define PIN_TC0_TIOA2_ATTR    PIO_DEFAULT

//! \name SPI
//@{
/** SPI MISO pin definition. */
#define SPI_MISO_GPIO         (PIO_PA12_IDX)
#define SPI_MISO_FLAGS       (PIO_PERIPH_A | PIO_PULLUP)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO         (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS       (PIO_PERIPH_A | PIO_PULLUP)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO         (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS       (PIO_PERIPH_A | PIO_PULLUP)

/** SPI chip select 0 pin definition. (Only one configuration is possible) */
#define SPI_NPCS0_GPIO         (PIO_PA11_IDX)
#define SPI_NPCS0_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI chip select 1 pin definition. (multiple configurations are possible) */
#define SPI_NPCS1_PA9_GPIO     (PIO_PA9_IDX)
#define SPI_NPCS1_PA9_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS1_PA31_GPIO    (PIO_PA31_IDX)
#define SPI_NPCS1_PA31_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS1_PB14_GPIO    (PIO_PB14_IDX)
#define SPI_NPCS1_PB14_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS1_PC4_GPIO     (PIO_PC4_IDX)
#define SPI_NPCS1_PC4_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
/** SPI chip select 2 pin definition. (multiple configurations are possible) */
#define SPI_NPCS2_PA10_GPIO    (PIO_PA10_IDX)
#define SPI_NPCS2_PA10_FLAGS      (PIO_PERIPH_B | PIO_PULLUP)
#define SPI_NPCS2_PA30_GPIO    (PIO_PA30_IDX)
#define SPI_NPCS2_PA30_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS2_PB2_GPIO     (PIO_PB2_IDX)
#define SPI_NPCS2_PB2_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
/** SPI chip select 3 pin definition. (multiple configurations are possible) */
#define SPI_NPCS3_PA3_GPIO     (PIO_PA3_IDX)
#define SPI_NPCS3_PA3_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS3_PA5_GPIO     (PIO_PA5_IDX)
#define SPI_NPCS3_PA5_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS3_PA22_GPIO    (PIO_PA22_IDX)
#define SPI_NPCS3_PA22_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)
//@}

//! \name TWI
//@{
/** TWI0 pin definitions */
#define TWI0_DATA_GPIO   PIO_PA3_IDX
#define TWI0_DATA_FLAGS  (PIO_PERIPH_A | PIO_PULLUP)
#define TWI0_CLK_GPIO    PIO_PA4_IDX
#define TWI0_CLK_FLAGS   (PIO_PERIPH_A | PIO_PULLUP)
/** TWI1 pin definitions */
#define TWI1_DATA_GPIO   PIO_PB4_IDX
#define TWI1_DATA_FLAGS  (PIO_PERIPH_A | PIO_PULLUP)
#define TWI1_CLK_GPIO    PIO_PB5_IDX
#define TWI1_CLK_FLAGS   (PIO_PERIPH_A | PIO_PULLUP)
/** TWI2 pin definitions */
#define TWI2_DATA_GPIO   PIO_PB0_IDX
#define TWI2_DATA_FLAGS  (PIO_PERIPH_B | PIO_PULLUP)
#define TWI2_CLK_GPIO    PIO_PB1_IDX
#define TWI2_CLK_FLAGS   (PIO_PERIPH_B | PIO_PULLUP)
//@}

/** PCK0 pin definition (PA6) */
#define PIN_PCK0         (PIO_PA6_IDX)
#define PIN_PCK0_MUX     (IOPORT_MODE_MUX_B)
#define PIN_PCK0_FLAGS   (IOPORT_MODE_MUX_B)
#define PIN_PCK0_PORT    IOPORT_PIOA
#define PIN_PCK0_MASK    PIO_PA6B_PCK0
#define PIN_PCK0_PIO     PIOA
#define PIN_PCK0_ID      ID_PIOA
#define PIN_PCK0_TYPE    PIO_PERIPH_B
#define PIN_PCK0_ATTR    PIO_DEFAULT

//! \name IO1 button definitions */
//@{
/** Push button #1 definition. Attributes = pull-up + debounce + interrupt on rising edge. */
#define PUSHBUTTON_1_NAME    "SW0"
#define GPIO_PUSH_BUTTON_1   (PIO_PA30_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS    (PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)

#define PIN_PUSHBUTTON_1    {PIO_PA30, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_1_MASK PIO_PA30
#define PIN_PUSHBUTTON_1_PIO PIOA
#define PIN_PUSHBUTTON_1_ID ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)
//@}

/** @} */

/** @} */

#endif /* SAM4N_XPLAINED_PRO_H */
