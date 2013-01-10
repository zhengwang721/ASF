/**
 * \file
 *
 * \brief XMEGA-A3U-XPLAINED board header file.
 *
 * This file contains definitions and services related to the features of the
 * XMEGA-A3BU Xplained board.
 *
 * To use this board define BOARD=XMEGA_A3BU_XPLAINED.
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
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
#ifndef _XMEGA_A3BU_XPLAINED_H_
#define _XMEGA_A3BU_XPLAINED_H_

#include <compiler.h>

/**
 * \defgroup xmega_a3bu_xplained_group XMEGA-A3BU Xplained board
 * @{
 */

/**
 * \defgroup xmega_a3bu_xplained_feature_group Feature definitions
 * @{
 */

//! \name Miscellaneous data
//@{
//! Validate board support for the common sensor service.
#define COMMON_SENSOR_PLATFORM
//@}

/**
 * \name LEDs
 *
 * LED0 and LED1 are single yellow LEDs that are active low..
 * LED2 and LED3 are inside one package (Led red and green close
 * to USB connector) but can be controlled individually.
 * LED2 has a red color and is active low. This LED can be
 * used for general purposes.
 * LED3 has a green color and is active high. By default this
 * LED is on since it shall indicate that power is applied to the
 * board. By pulling the gate of a N-FET low it is possible to
 * turn off the LED if needed.
 */
//@{
#define LED0_GPIO                       IOPORT_CREATE_PIN(PORTA, 4)
#define LED1_GPIO                       IOPORT_CREATE_PIN(PORTA, 5)

#define LED0                            LED0_GPIO
#define LED1                            LED1_GPIO

//! Number of LEDs.
#define LED_COUNT                       2
//@}

/**
 * \name External oscillator
 *
 * \todo Need to measure the actual startup time on the hardware.
 */
//@{
#define BOARD_XOSC_HZ                   32768
#define BOARD_XOSC_TYPE                 XOSC_TYPE_32KHZ
#define BOARD_XOSC_STARTUP_US           1000000
//@}

//! \name Communication interfaces on header J1
//@{
#define TWIC_SDA                        IOPORT_CREATE_PIN(PORTC, 0)
#define TWIC_SCL                        IOPORT_CREATE_PIN(PORTC, 1)
#define USARTC0_RXD                     IOPORT_CREATE_PIN(PORTC, 2)
#define USARTC0_TXD                     IOPORT_CREATE_PIN(PORTC, 3)
#define SPIC_SS                         IOPORT_CREATE_PIN(PORTC, 4)
#define SPIC_MOSI                       IOPORT_CREATE_PIN(PORTC, 5)
#define SPIC_MISO                       IOPORT_CREATE_PIN(PORTC, 6)
#define SPIC_SCK                        IOPORT_CREATE_PIN(PORTC, 7)
//@}


/*! \name Connections of the AT86RFX transceiver
 */
//! @{
#define AT86RFX_SPI                  &SPIC
#define AT86RFX_RST_PIN              IOPORT_CREATE_PIN(PORTC, 0)
#define AT86RFX_MISC_PIN             IOPORT_CREATE_PIN(PORTC, 1)
#define AT86RFX_IRQ_PIN              IOPORT_CREATE_PIN(PORTC, 2)
#define AT86RFX_SLP_PIN              IOPORT_CREATE_PIN(PORTC, 3)
#define AT86RFX_SPI_CS               IOPORT_CREATE_PIN(PORTC, 4)
#define AT86RFX_SPI_MOSI             IOPORT_CREATE_PIN(PORTC, 5)
#define AT86RFX_SPI_MISO             IOPORT_CREATE_PIN(PORTC, 6)
#define AT86RFX_SPI_SCK              IOPORT_CREATE_PIN(PORTC, 7)

#define AT86RFX_INTC_INIT()          ioport_configure_pin(AT86RFX_IRQ_PIN, IOPORT_DIR_INPUT); \
									 PORTC.PIN2CTRL = PORT_ISC0_bm; \
									 PORTC.INT0MASK = PIN2_bm; \
									 PORTC.INTFLAGS = PORT_INT0IF_bm;

#define AT86RFX_ISR()                ISR(PORTC_INT0_vect)

/** Enables the transceiver main interrupt. */
#define ENABLE_TRX_IRQ()                (PORTC.INTCTRL |= PORT_INT0LVL_gm)

/** Disables the transceiver main interrupt. */
#define DISABLE_TRX_IRQ()               (PORTC.INTCTRL &= ~PORT_INT0LVL_gm)

/** Clears the transceiver main interrupt. */
#define CLEAR_TRX_IRQ()                 (PORTC.INTFLAGS = PORT_INT0IF_bm)

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()   { uint8_t irq_mask = PORTC.INTCTRL; PORTC.INTCTRL &= ~PORT_INT0LVL_gm

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()   PORTC.INTCTRL = irq_mask; }

//! @}



/**
 * @}
 */

/**
 * \defgroup xmega_a3bu_xplained_config_group Configuration options
 * @{
 */

#if defined(__DOXYGEN__)

/**
 * \name Initialization
 * \note Define these symbols in \ref conf_board.h to enable the corresponding
 * features.
 */
//@{

/**
 * \def CONF_BOARD_ENABLE_USARTC0
 * \brief Initialize IO pins for USART 0 on port C
 */
#  if !defined(CONF_BOARD_ENABLE_USARTC0)
#    define CONF_BOARD_ENABLE_USARTC0
#  endif

/**
 * \def CONF_BOARD_ENABLE_USARTD0
 * \brief Initialize IO pins for USART 0 on port D
 */
#  if !defined(CONF_BOARD_ENABLE_USARTD0)
#    define CONF_BOARD_ENABLE_USARTD0
#  endif

/**
 * \def CONF_BOARD_ENABLE_USARTE0
 * \brief Initialize IO pins for USART 0 on port E
 */
#  if !defined(CONF_BOARD_ENABLE_USARTE0)
#    define CONF_BOARD_ENABLE_USARTE0
#  endif

//@}

#endif // __DOXYGEN__

/**
 * @}
 */

/**
 * @}
 */

#endif /* _XMEGA_A3BU_XPLAINED_H_ */
