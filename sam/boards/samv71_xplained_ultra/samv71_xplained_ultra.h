/**
 * \file
 *
 * \brief SAMV71-XULTRA Board Definition.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _SAMV71_XULTRA_H_
#define _SAMV71_XULTRA_H_

#include "compiler.h"
#include "system_samv71.h"

/*----------------------------------------------------------------------------*/
/**
 *  \page samv71_xplained_ultra_opfreq "SAMV71-XULTRA - Operating frequencies"
 *  This page lists several definition related to the board operating frequency
 *
 *  \section Definitions
 *  - \ref BOARD_FREQ_*
 *  - \ref BOARD_MCK
 */

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL            (32768U)
#define BOARD_FREQ_SLCK_BYPASS          (32768U)
#define BOARD_FREQ_MAINCK_XTAL          (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS        (12000000U)

/** Master clock frequency */
#define BOARD_MCK                       CHIP_FREQ_CPU_MAX

/** board main clock xtal statup time */
#define BOARD_OSC_STARTUP_US            15625

/*----------------------------------------------------------------------------*/
/**
 * \page samv71_xplained_ultra_info "SAMV71-XULTRA - Board informations"
 * This page lists several definition related to the board description.
 *
 * \section Definitions
 * - \ref BOARD_NAME
 */

/** Name of the board */
#define BOARD_NAME "SAMV71-XULTRA"
/** Board definition */
#define samv71xultra
/** Family definition (already defined) */
#define samv71
/** Core definition */
#define cortexm7

/*----------------------------------------------------------------------------*/

#define CONSOLE_UART               USART1
#define CONSOLE_UART_ID            ID_USART1
/** USART1 pins definitions, PA21,PB4. */
#define USART1_RXD_GPIO   PIO_PA21_IDX
#define USART1_RXD_FLAGS  IOPORT_MODE_MUX_A
#define USART1_TXD_GPIO   PIO_PB4_IDX
#define USART1_TXD_FLAGS  IOPORT_MODE_MUX_D


//! \name LED definitions
//@{
#define LED0_GPIO            (PIO_PA23_IDX)
#define LED0_FLAGS           (0)
#define LED0_ACTIVE_LEVEL    (IOPORT_PIN_LEVEL_LOW)
#define LED0_INACTIVE_LEVEL  (IOPORT_PIN_LEVEL_HIGH)

#define LED1_GPIO            (PIO_PC9_IDX)
#define LED1_FLAGS           (0)
#define LED1_ACTIVE_LEVEL    (IOPORT_PIN_LEVEL_LOW)
#define LED1_INACTIVE_LEVEL  (IOPORT_PIN_LEVEL_HIGH)

#define	LED_COUNT			 (2)
//@}

//! \name SW0 definitions
//@{
#define SW0_PIN                   (PIO_PA9_IDX)
#define SW0_ACTIVE                (IOPORT_PIN_LEVEL_LOW)
#define SW0_INACTIVE              (!SW0_ACTIVE)
#define SW0_SUPC_INPUT            2

/**
 * Wrapper macros for SW0, to ensure common naming across all Xplained Ultra
 * boards.
 */
#define PIN_SW0      {PIO_PA9, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_SW0_MASK PIO_PA9
#define PIN_SW0_PIO  PIOA
#define PIN_SW0_ID   ID_PIOA
#define PIN_SW0_TYPE PIO_INPUT
#define PIN_SW0_ATTR (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)
//@}

//! \name SW1 definitions
//@{
#define SW1_PIN                   (PIO_PB12_IDX)
#define SW1_ACTIVE                (IOPORT_PIN_LEVEL_LOW)
#define SW1_INACTIVE              (!SW1_ACTIVE)
#define SW1_SUPC_INPUT            2

/**
 * Wrapper macros for SW1, to ensure common naming across all Xplained Ultra
 * boards.
 */
#define PIN_SW1      {PIO_PB12, PIOB, ID_PIOB, PIO_INPUT, PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_SW1_MASK PIO_PB12
#define PIN_SW1_PIO  PIOB
#define PIN_SW1_ID   ID_PIOB
#define PIN_SW1_TYPE PIO_INPUT
#define PIN_SW1_ATTR (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)
//@}

/**
 * \name LED #0 definitions
 *
 * Wrapper macros for LED0, to ensure common naming across all Xplained Ultra
 * boards.
 */
//@{
#define LED_0_NAME                "LED0 (yellow)"
#define LED_0_PIN                 LED0_GPIO
#define LED_0_ACTIVE              LED0_ACTIVE_LEVEL
#define LED_0_INACTIVE            LED0_INACTIVE_LEVEL

#define PIN_LED_0       {PIO_PA23, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_0_MASK  PIO_PA23
#define PIN_LED_0_PIO   PIOA
#define PIN_LED_0_ID    ID_PIOA
#define PIN_LED_0_TYPE  PIO_OUTPUT_1
#define PIN_LED_0_ATTR  PIO_DEFAULT
//@}

/**
 * \name LED #1 definitions
 *
 * Wrapper macros for LED1, to ensure common naming across all Xplained Ultra
 * boards.
 */
//@{
#define LED_1_NAME                "LED1 (yellow)"
#define LED_1_PIN                 LED1_GPIO
#define LED_1_ACTIVE              LED1_ACTIVE_LEVEL
#define LED_1_INACTIVE            LED1_INACTIVE_LEVEL

#define PIN_LED_1       {PIO_PC9, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_1_MASK  PIO_PC9
#define PIN_LED_1_PIO   PIOC
#define PIN_LED_1_ID    ID_PIOC
#define PIN_LED_1_TYPE  PIO_OUTPUT_1
#define PIN_LED_1_ATTR  PIO_DEFAULT
//@}

//! Number of on-board LEDs
#define BOARD_NUM_OF_LED 2

/**
 * Push button #0 definition. Attributes = pull-up + debounce + interrupt on
 * rising edge.
 */
#define BUTTON_0_NAME             "SW0"
#define BUTTON_0_PIN              SW0_PIN
#define BUTTON_0_ACTIVE           SW0_ACTIVE
#define BUTTON_0_INACTIVE         SW0_INACTIVE
#define BUTTON_0_SUPC_INPUT       SW0_SUPC_INPUT
#define GPIO_PUSH_BUTTON_0        BUTTON_0_PIN

#define PUSHBUTTON_1_NAME        "SW0"
#define PUSHBUTTON_1_WKUP_LINE   (2)
#define PUSHBUTTON_1_WKUP_FSTT   (PMC_FSMR_FSTT2)
#define GPIO_PUSH_BUTTON_1       (PIO_PA9_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define GPIO_PUSH_BUTTON_1_SENSE (IOPORT_SENSE_RISING)

#define PIN_PUSHBUTTON_1       {PIO_PA9, PIOA, ID_PIOA, PIO_INPUT, \
		PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_1_MASK  PIO_PA9
#define PIN_PUSHBUTTON_1_PIO   PIOA
#define PIN_PUSHBUTTON_1_ID    ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE  PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR  (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)
#define PIN_PUSHBUTTON_1_IRQn  PIOA_IRQn

/** List of all push button definitions. */
#define PINS_PUSHBUTTONS    {PIN_PUSHBUTTON_1}

/*----------------------------------------------------------------------------*/
#endif  /* _SAMV71_XULTRA_H_ */
