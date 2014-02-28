/**
 * \file
 *
 * \brief SAM4CMP-DB Board Definition.
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

#ifndef SAM4CMP_DB_H_INCLUDED
#define SAM4CMP_DB_H_INCLUDED

#include "compiler.h"

/**
 * \ingroup group_common_boards
 * \defgroup sam4cmp_db_group "SAM4CMP-DB"
 * Demo Board for SAM4CMP devices.
 *
 * @{
 */

/**
 * \defgroup sam4cmp_db_board_info_group "SAM4CMP-DB - Board informations"
 * Definitions related to the board description.
 *
 * @{
 */

/** Name of the board */
#define BOARD_NAME "SAM4CMP-DB"
/** Board definition */
#define sam4cmpdb
/** Family definition (already defined) */
#define sam4cmp
/** Core definition */
#define cortexm4
/** Board revision definition */
#define BOARD_REV_2

/* @} */

/**
 *  \defgroup sam4cmp_db_opfreq_group "SAM4CMP-DB - Operating frequencies"
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
#define BOARD_FREQ_MAINCK_XTAL      (8192000U)
#define BOARD_FREQ_MAINCK_BYPASS    (8192000U)
/* @} */

/** Master clock frequency */
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX

/** board main clock xtal startup time */
#define BOARD_OSC_STARTUP_US        15625U

/* @} */

/**
 * \defgroup sam4cmp_db_features_group "SAM4CMP-DB - Features"
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/**
 * \name LED #2 pin definition
 * @{
 */
#define LED2_GPIO            (PIO_PC0_IDX)
#define LED2_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_HIGH
#define LED2_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_LOW

/* Wrapper macros to ensure common naming across all boards */
#define LED_2_NAME      "LED2"
#define PIN_LED_2       {PIO_PC0, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_2_MASK   PIO_PC0
#define PIN_LED_2_PIO    PIOC
#define PIN_LED_2_ID     ID_PIOC
#define PIN_LED_2_TYPE   PIO_OUTPUT_1
#define PIN_LED_2_ATTR   PIO_DEFAULT
/* @} */

/**
 * \name LED #3 pin definition
 * @{
 */
#define LED3_GPIO            (PIO_PC6_IDX)
#define LED3_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_HIGH
#define LED3_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_LOW

/* Wrapper macros to ensure common naming across all boards */
#define LED_3_NAME      "LED3"
#define PIN_LED_3       {PIO_PC6, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_3_MASK   PIO_PC6
#define PIN_LED_3_PIO    PIOC
#define PIN_LED_3_ID     ID_PIOC
#define PIN_LED_3_TYPE   PIO_OUTPUT_1
#define PIN_LED_3_ATTR   PIO_DEFAULT
/* @} */

/**
 * \name LED #4 pin definition
 * @{
 */
#define LED4_GPIO            (PIO_PC7_IDX)
#define LED4_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_HIGH
#define LED4_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_LOW

/* Wrapper macros to ensure common naming across all boards */
#define LED_4_NAME      "LED4"
#define PIN_LED_4       {PIO_PC7, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_4_MASK   PIO_PC7
#define PIN_LED_4_PIO    PIOC
#define PIN_LED_4_ID     ID_PIOC
#define PIN_LED_4_TYPE   PIO_OUTPUT_1
#define PIN_LED_4_ATTR   PIO_DEFAULT
/* @} */

/**
 * \name Push button #1 definition
 * Attributes = pull-up + debounce + interrupt on rising edge.
 * @{
 */
#define PUSHBUTTON_1_NAME        "SCROLL_UP (PB1)"
#define GPIO_PUSH_BUTTON_1       (PIO_PA19_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define GPIO_PUSH_BUTTON_1_SENSE (IOPORT_SENSE_RISING)

#define PIN_PUSHBUTTON_1       {PIO_PA19, PIOA, ID_PIOA, PIO_INPUT, \
		PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_1_MASK  PIO_PA19
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
#define PUSHBUTTON_2_NAME        "SCROLL_DOWN (PB2)"
#define PUSHBUTTON_2_WKUP_LINE   (4)
#define PUSHBUTTON_2_WKUP_FSTT   (PMC_FSMR_FSTT4)
#define GPIO_PUSH_BUTTON_2       (PIO_PA17_IDX)
#define GPIO_PUSH_BUTTON_2_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define GPIO_PUSH_BUTTON_2_SENSE (IOPORT_SENSE_RISING)

#define PIN_PUSHBUTTON_2       {PIO_PA17, PIOA, ID_PIOA, PIO_INPUT, \
		PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_2_MASK  PIO_PA17
#define PIN_PUSHBUTTON_2_PIO   PIOA
#define PIN_PUSHBUTTON_2_ID    ID_PIOA
#define PIN_PUSHBUTTON_2_TYPE  PIO_INPUT
#define PIN_PUSHBUTTON_2_ATTR  (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)
#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn
/* @} */

/** List of all push button definitions. */
#define PINS_PUSHBUTTONS    {PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2}

/**
 * \name Console UART definitions
 * @{
 */
#define CONSOLE_UART      UART0
#define CONSOLE_UART_ID   ID_UART0
/* @} */

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
#define PINS_UART1        (PIO_PC1A_URXD1 | PIO_PC0A_UTXD1)
#define PINS_UART1_FLAGS  (IOPORT_MODE_MUX_A)

#define PINS_UART1_PORT   IOPORT_PIOC
#define PINS_UART1_MASK   (PIO_PC1A_URXD1 | PIO_PC0A_UTXD1)
#define PINS_UART1_PIO    PIOC
#define PINS_UART1_ID     ID_PIOC
#define PINS_UART1_TYPE   PIO_PERIPH_A
#define PINS_UART1_ATTR   PIO_DEFAULT
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
#define SPI0_NPCS0_GPIO        (PIO_PA21_IDX)
#define SPI0_NPCS0_FLAGS       (IOPORT_MODE_MUX_A)
/* @} */

/* Select the SPI module that AT25DFx is connected to */
#define AT25DFX_SPI_MODULE     SPI0

/* Chip select used by AT25DFx components on the SPI module instance */
#define AT25DFX_CS             0

/**
 * \name TWIx pin definitions
 * @{
 */
/*! TWI ID for EEPROM application to use */
#define BOARD_ID_TWI_EEPROM       ID_TWI0

/*! TWI Base for TWI EEPROM application to use */
#define BOARD_BASE_TWI_EEPROM     TWI0
#define BOARD_AT24C_TWI_INSTANCE  TWI0
#define BOARD_AT24C_ADDRESS       0x50u

/*! TWI Base for TWI Temperature Sensor application to use */
#define BOARD_AT30TSE_TWI         TWI0
#define BOARD_AT30TSE_DEVICE_ADDR 0x48u
#define BOARD_AT30TSE_TWI_ID      ID_TWI0
#define BOARD_USING_AT30TSE       AT30TS75

/*! TWI Base for TWI Crypto Authentication application to use */
#define BOARD_ATSHA204_TWI         TWI0
#define BOARD_ATSHA204_DEVICE_ADDR 0xc9u
#define BOARD_ATSHA204_TWI_ID      ID_TWI0

/*! TWI0 Data pin for EEPROM */
#define TWIO_DATA_GPIO            PIO_PA24_IDX
#define TWIO_DATA_FLAG            IOPORT_MODE_MUX_A
/*! TWI0 Clock pin for EEPROM */
#define TWIO_CLK_GPIO             PIO_PA25_IDX
#define TWIO_CLK_FLAG             IOPORT_MODE_MUX_A
#define BOARD_CLK_TWI_EEPROM      TWIO_CLK_GPIO
#define BOARD_CLK_TWI_MUX_EEPROM  TWIO_CLK_FLAG
/* @} */

/* @} */ /* End of sam4cmp_db_features_group */

/* @} */ /* End of sam4cmp_db_group */

#endif  /* SAM4CMP_DB_H_INCLUDED */

