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

/** Name of the board */
#define BOARD_NAME "SAM4L_XPLAINED_PRO"

/**
 * \defgroup sam4l_xplained_pro_group SAM4L Xplained PRO Board
 * @{
 */

//! Osc frequency (Hz.) and startup time (RCOsc periods).
#define FOSC0                     (12000000)

//! Osc32 frequency (Hz.) and startup time (RCOsc periods).
#define FOSC32                    (32768)

/**
 * \name Board oscillator configuration
 *
 */
//@{
#define BOARD_OSC32_IS_XTAL       true
#define BOARD_OSC32_HZ            FOSC32
#define BOARD_OSC32_STARTUP_US    (71000)
#define BOARD_OSC32_SELCURR       BSCIF_OSCCTRL32_SELCURR(10)
#define BOARD_OSC0_IS_XTAL        true
#define BOARD_OSC0_HZ             FOSC0
#define BOARD_OSC0_STARTUP_US     (1100)
//@}

/**
 * \name LEDs
 *
 * LED 0 is a single yellow LED that is active low.
 */
//@{
#define LED_0_PIN                 PIN_PC07
#define LED_0_ACTIVE              IOPORT_PIN_LEVEL_LOW
#define LED_0_INACTIVE            IOPORT_PIN_LEVEL_HIGH
//! @}

//! Number if LEDs on board
#define LED_COUNT   1

/*! \name Button 0 definitions
 */
//! @{
#define BUTTON_0_PIN              PIN_PC24
#define BUTTON_0_DOWN             IOPORT_PIN_LEVEL_LOW
#define BUTTON_0_UP               IOPORT_PIN_LEVEL_HIGH
//! @}

/*! \name Push button connection of external interrupt line
 */
//! @{
#define BUTTON_0_EIC_PIN          PIN_PC24B_EIC_EXTINT1
#define BUTTON_0_EIC_PIN_MUX      MUX_PC24B_EIC_EXTINT1
#define BUTTON_0_EIC_LINE         1
//! @}


/*! \name GPIO Connections of touch sensor
 */
//! @{
#define QTOUCH_DISCHARGE_PIN      PIN_PC14
#define QTOUCH_DISCHARGE_MUX      MUX_PC14_CATB_DIS

#define QTOUCH_BUTTON_PIN         PIN_PC13
#define QTOUCH_BUTTON_MUX         MUX_PB04G_CATB_SENSE15
//! @}

/*! \name Touch sensors pin assignements
 */
//! @{
#define QTOUCH_PINSEL_BUTTON      15
//! @}

/*! \name GPIO Connections of VBUS monitoring
 */
//! @{
#define VBUS_INPUT_PIN            PIN_PC11
//! @}

//@}
#endif /* SAM4L_XPLAINED_PRO_INCLUDED */
