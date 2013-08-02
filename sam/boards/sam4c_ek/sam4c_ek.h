/**
 * \file
 *
 * \brief SAM4C-EK Board Definition.
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

#ifndef SAM4C_EK_H_INCLUDED
#define SAM4C_EK_H_INCLUDED

#include "compiler.h"

/**
 * \ingroup group_common_boards
 * \defgroup sam4c_ek_group "SAM4C-EK"
 * Evaluation Kit for SAM4C devices.
 *
 * @{
 */

/**
 * \defgroup sam4c_ek_board_info_group "SAM4C-EK - Board informations"
 * Definitions related to the board description.
 *
 * @{
 */

/** Name of the board */
#define BOARD_NAME "SAM4C-EK"
/** Board definition */
#define sam4cek
/** Family definition (already defined) */
#define sam4c
/** Core definition */
#define cortexm4
/** Board revision definition */
#define BOARD_REV_C

/* @} */

/**
 *  \defgroup sam4c_ek_opfreq_group "SAM4C-EK - Operating frequencies"
 *  Definitions related to the board operating frequency.
 *
 *  @{
 */

/**
 * \name Board oscillator settings
 * @{
 */
#define BOARD_FREQ_SLCK_XTAL        (32768U)
#define BOARD_FREQ_SLCK_BYPASS      (32768U)
#define BOARD_FREQ_MAINCK_XTAL      (8000000U)
#define BOARD_FREQ_MAINCK_BYPASS    (8000000U)
/* @} */

/** Master clock frequency */
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX

/** board main clock xtal statup time */
#define BOARD_OSC_STARTUP_US        15625U

/* @} */

/**
 * \defgroup sam4c_ek_features_group "SAM4C-EK - Features"
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/**
 * \name LED #0 pin definition
 * @{
 */
#define LED0_GPIO            (PIO_PC6_IDX)
#define LED0_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define LED0_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/* Wrapper macros to ensure common naming across all boards */
#define LED_0_NAME      "blue LED D8"
#define PIN_LED_0       {PIO_PC6, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_0_MASK   PIO_PC6
#define PIN_LED_0_PIO    PIOC
#define PIN_LED_0_ID     ID_PIOC
#define PIN_LED_0_TYPE   PIO_OUTPUT_1
#define PIN_LED_0_ATTR   PIO_DEFAULT
/* @} */

/**
 * \name LED #1 pin definition
 * @{
 */
#define LED1_GPIO            (PIO_PC7_IDX)
#define LED1_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define LED1_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/* Wrapper macros to ensure common naming across all boards */
#define LED_1_NAME      "amber LED D9"
#define PIN_LED_1       {PIO_PC7, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_1_MASK   PIO_PC7
#define PIN_LED_1_PIO    PIOC
#define PIN_LED_1_ID     ID_PIOC
#define PIN_LED_1_TYPE   PIO_OUTPUT_1
#define PIN_LED_1_ATTR   PIO_DEFAULT
/* @} */

/**
 * \name LED #2 pin definition
 * @{
 */
#define LED2_GPIO            (PIO_PC8_IDX)
#define LED2_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define LED2_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/* Wrapper macros to ensure common naming across all boards */
#define LED_2_NAME      "blue LED D8"
#define PIN_LED_2       {PIO_PC8, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_2_MASK   PIO_PC8
#define PIN_LED_2_PIO    PIOC
#define PIN_LED_2_ID     ID_PIOC
#define PIN_LED_2_TYPE   PIO_OUTPUT_1
#define PIN_LED_2_ATTR   PIO_DEFAULT
/* @} */

/**
 * \name Push button #1 definition
 * Attributes = pull-up + debounce + interrupt on rising edge.
 * @{
 */
#define PUSHBUTTON_1_NAME        "Scroll down"
#define GPIO_PUSH_BUTTON_1       (PIO_PA20_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define GPIO_PUSH_BUTTON_1_SENSE (IOPORT_SENSE_RISING)

#define PIN_PUSHBUTTON_1       {PIO_PA20, PIOA, ID_PIOA, PIO_INPUT, \
		PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_1_MASK  PIO_PA20
#define PIN_PUSHBUTTON_1_PIO   PIOA
#define PIN_PUSHBUTTON_1_ID    ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE  PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR  (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)
#define PIN_PUSHBUTTON_1_IRQn  PIOA_IRQn
/* @} */

/**
 * \name Push button #2 definition
 * Attributes = pull-up + debounce + interrupt on rising edge.
 * @{
 */
#define PUSHBUTTON_2_NAME        "Scroll up"
#define PUSHBUTTON_2_WKUP_LINE   (4)
#define PUSHBUTTON_2_WKUP_FSTT   (PMC_FSMR_FSTT4)
#define GPIO_PUSH_BUTTON_2       (PIO_PA19_IDX)
#define GPIO_PUSH_BUTTON_2_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define GPIO_PUSH_BUTTON_2_SENSE (IOPORT_SENSE_RISING)

#define PIN_PUSHBUTTON_2       {PIO_PA19, PIOA, ID_PIOA, PIO_INPUT, \
		PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_2_MASK  PIO_PA19
#define PIN_PUSHBUTTON_2_PIO   PIOA
#define PIN_PUSHBUTTON_2_ID    ID_PIOA
#define PIN_PUSHBUTTON_2_TYPE  PIO_INPUT
#define PIN_PUSHBUTTON_2_ATTR  (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)
#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn
/* @} */

/** List of all push button definitions. */
#define PINS_PUSHBUTTONS    {PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2}

#define CONSOLE_UART      UART0
#define CONSOLE_UART_ID   ID_UART0

#define PIN_TC0_TIOA0        (PIO_PA13_IDX)
#define PIN_TC0_TIOA0_MUX    (IOPORT_MODE_MUX_B)
#define PIN_TC0_TIOA0_FLAGS  (IOPORT_MODE_MUX_B)

#define PIN_TC0_TIOA1        (PIO_PB7_IDX)
#define PIN_TC0_TIOA1_MUX    (IOPORT_MODE_MUX_A)
#define PIN_TC0_TIOA1_FLAGS  (IOPORT_MODE_MUX_A)

#define PIN_TC0_TIOA1_PIO    PIOB
#define PIN_TC0_TIOA1_MASK   PIO_PB7
#define PIN_TC0_TIOA1_ID     ID_PIOB
#define PIN_TC0_TIOA1_TYPE   PIO_PERIPH_A
#define PIN_TC0_TIOA1_ATTR   PIO_DEFAULT

#define PIN_TC0_TIOA2        (PIO_PB10_IDX)
#define PIN_TC0_TIOA2_MUX    (IOPORT_MODE_MUX_A)
#define PIN_TC0_TIOA2_FLAGS  (IOPORT_MODE_MUX_A)

#define PIN_TC0_TIOA2_PIO    PIOB
#define PIN_TC0_TIOA2_MASK   PIO_PB10
#define PIN_TC0_TIOA2_ID     ID_PIOB
#define PIN_TC0_TIOA2_TYPE   PIO_PERIPH_A
#define PIN_TC0_TIOA2_ATTR   PIO_DEFAULT

/**
 * \name UART0 pis (UTXD0 and URXD0) definitions
 * @{
 */
#define PINS_UART0        (PIO_PB4A_URXD0 | PIO_PB5A_UTXD0)
#define PINS_UART0_FLAGS  (IOPORT_MODE_MUX_A)

#define PINS_UART0_PORT   IOPORT_PIOB
#define PINS_UART0_MASK   (PIO_PB4A_URXD0 | PIO_PB5A_UTXD0)
#define PINS_UART0_PIO    PIOB
#define PINS_UART0_ID     ID_PIOB
#define PINS_UART0_TYPE   PIO_PERIPH_A
#define PINS_UART0_ATTR   PIO_DEFAULT
/* @} */

/**
 * \name UART1 pis (UTXD1 and URXD1) definitions
 * @{
 */
#define PINS_UART1        (PIO_PC0A_URXD0 | PIO_PC1A_UTXD0)
#define PINS_UART1_FLAGS  (IOPORT_MODE_MUX_A)

#define PINS_UART1_PORT   IOPORT_PIOC
#define PINS_UART1_MASK   (PIO_PC0A_URXD0 | PIO_PC1A_UTXD0)
#define PINS_UART1_PIO    PIOC
#define PINS_UART1_ID     ID_PIOC
#define PINS_UART1_TYPE   PIO_PERIPH_A
#define PINS_UART1_ATTR   PIO_DEFAULT
/* @} */

/**
 * \name PWM LEDx pin definitions
 * @{
 */
// TBD
/* @} */

/**
 * \name SPI pin definitions
 * @{
 */
/** SPI0 MISO pin definition. */
#define SPI0_MISO_GPIO         (PIO_PA6_IDX)
#define SPI0_MISO_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI0 MOSI pin definition. */
#define SPI0_MOSI_GPIO         (PIO_PA7_IDX)
#define SPI0_MOSI_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI0 SPCK pin definition. */
#define SPI0_SPCK_GPIO         (PIO_PA8_IDX)
#define SPI0_SPCK_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI0 chip select 0 pin definition. */
#define SPI0_NPCS0_GPIO        (PIO_PA5_IDX)
#define SPI0_NPCS0_FLAGS       (IOPORT_MODE_MUX_A)

/** SPI1 MISO pin definition. */
#define SPI1_MISO_GPIO         (PIO_PC3_IDX)
#define SPI1_MISO_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI1 MOSI pin definition. */
#define SPI1_MOSI_GPIO         (PIO_PC4_IDX)
#define SPI1_MOSI_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI1 SPCK pin definition. */
#define SPI1_SPCK_GPIO         (PIO_PC5_IDX)
#define SPI1_SPCK_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI1 chip select 0 pin definition. */
#define SPI1_NPCS0_GPIO        (PIO_PC2_IDX)
#define SPI1_NPCS0_FLAGS       (IOPORT_MODE_MUX_A)
/* @} */

/* Select the SPI module that AT25DFx is connected to */
#define AT25DFX_SPI_MODULE     SPI0

/* Chip select used by AT25DFx components on the SPI module instance */
#define AT25DFX_CS             0

/**
 * \name TWIx pin definitions
 * @{
 */
// TBD
/* @} */

/**
 * \name PCK pin definitions
 * @{
 */
#define PIN_PCK2        (PIO_PA0_IDX)
#define PIN_PCK2_MUX    (IOPORT_MODE_MUX_B)
/* @} */

/**
 * \name USARTx pin definitions
 * @{
 */
// TBD
/* @} */

/**
 * \name ADC pin definitions
 * @{
 */
// TBD
/* @} */

/**
 * \name EBI pin definitions
 * @{
 */
// TBD
/* @} */


/* @} */ /* End of sam4c_ek_features_group */

/* @} */ /* End of sam4c_ek_group */

#endif  /* SAM4C_EK_H_INCLUDED */

