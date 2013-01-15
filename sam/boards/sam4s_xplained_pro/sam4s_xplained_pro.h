/**
 * \file
 *
 * \brief SAM4S Xplained PRO board definition and driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 *    AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 */

#ifndef SAM4S_XPLAINED_PRO_H
#define SAM4S_XPLAINED_PRO_H

#include <conf_board.h>

/**
 * \defgroup sam4s_xplained_pro_group SAM4S Xplained PRO board
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

#endif
/** @} */


/**
 * \defgroup sam4s_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

//! Name string macro
#define BOARD_NAME "SAM4S_XPLAINED_PRO"

//! \name Resonator definitions
//@{
#define BOARD_FREQ_SLCK_XTAL        (32768U)
#define BOARD_FREQ_SLCK_BYPASS      (32768U)
#define BOARD_FREQ_MAINCK_XTAL      (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS    (12000000U)
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US        15625
//@}

//! \name LED #0 definition
//@{
//! Name string macro
#define LED_0_NAME   "LED0 (yellow)"
//! LED pin identifier
#define LED_0_PIN    IOPORT_CREATE_PIN(PIOC, 23)
//! Active level of LED
#define LED_0_ACTIVE false
//@}

//! Number of on-board LEDs
#define LED_COUNT 1

//! \name Button #0 definition
//@{
//! Name string macro
#define BUTTON_0_NAME   "SW0"
//! Button pin identifier
#define BUTTON_0_PIN    IOPORT_CREATE_PIN(PIOA, 2)
//! Active level of button
#define BUTTON_0_ACTIVE false
//@}

//! Number of on-board buttons
#define BUTTON_COUNT 1

//! \name EXT1 pins
//@{
#define EXT1_PIN_3  IOPORT_CREATE_PIN(PIOA, 17)
#define EXT1_PIN_4  IOPORT_CREATE_PIN(PIOA, 18)
#define EXT1_PIN_5  IOPORT_CREATE_PIN(PIOA, 24)
#define EXT1_PIN_6  IOPORT_CREATE_PIN(PIOA, 25)
#define EXT1_PIN_7  IOPORT_CREATE_PIN(PIOA, 23)
#define EXT1_PIN_8  IOPORT_CREATE_PIN(PIOA, 19)
#define EXT1_PIN_9  IOPORT_CREATE_PIN(PIOA, 1)
#define EXT1_PIN_10 IOPORT_CREATE_PIN(PIOA, 6)
#define EXT1_PIN_11 IOPORT_CREATE_PIN(PIOA, 3)
#define EXT1_PIN_12 IOPORT_CREATE_PIN(PIOA, 4)
#define EXT1_PIN_13 IOPORT_CREATE_PIN(PIOA, 21)
#define EXT1_PIN_14 IOPORT_CREATE_PIN(PIOA, 22)
#define EXT1_PIN_15 IOPORT_CREATE_PIN(PIOA, 11)
#define EXT1_PIN_16 IOPORT_CREATE_PIN(PIOA, 13)
#define EXT1_PIN_17 IOPORT_CREATE_PIN(PIOA, 12)
#define EXT1_PIN_18 IOPORT_CREATE_PIN(PIOA, 14)
//@}

//! \name EXT2 pins
//@{
#define EXT2_PIN_3  IOPORT_CREATE_PIN(PIOB, 0)
#define EXT2_PIN_4  IOPORT_CREATE_PIN(PIOB, 1)
#define EXT2_PIN_5  IOPORT_CREATE_PIN(PIOC, 24)
#define EXT2_PIN_6  IOPORT_CREATE_PIN(PIOC, 25)
#define EXT2_PIN_7  IOPORT_CREATE_PIN(PIOC, 19)
#define EXT2_PIN_8  IOPORT_CREATE_PIN(PIOA, 20)
#define EXT2_PIN_9  IOPORT_CREATE_PIN(PIOC, 26)
#define EXT2_PIN_10 IOPORT_CREATE_PIN(PIOC, 27)
#define EXT2_PIN_11 IOPORT_CREATE_PIN(PIOA, 3)
#define EXT2_PIN_12 IOPORT_CREATE_PIN(PIOA, 4)
#define EXT2_PIN_13 IOPORT_CREATE_PIN(PIOA, 21)
#define EXT2_PIN_14 IOPORT_CREATE_PIN(PIOA, 22)
#define EXT2_PIN_15 IOPORT_CREATE_PIN(PIOA, 9)
#define EXT2_PIN_16 IOPORT_CREATE_PIN(PIOA, 13)
#define EXT2_PIN_17 IOPORT_CREATE_PIN(PIOA, 12)
#define EXT2_PIN_18 IOPORT_CREATE_PIN(PIOA, 14)
//@}

//! \name EXT3 pins
//@{
#define EXT3_PIN_3  IOPORT_CREATE_PIN(PIOC, 29)
#define EXT3_PIN_4  IOPORT_CREATE_PIN(PIOC, 30)
#define EXT3_PIN_5  IOPORT_CREATE_PIN(PIOC, 21)
#define EXT3_PIN_6  IOPORT_CREATE_PIN(PIOC, 22)
#define EXT3_PIN_7  IOPORT_CREATE_PIN(PIOC, 20)
#define EXT3_PIN_8  IOPORT_CREATE_PIN(PIOA, 16)
#define EXT3_PIN_9  IOPORT_CREATE_PIN(PIOA, 0)
#define EXT3_PIN_10 IOPORT_CREATE_PIN(PIOC, 31)
#define EXT3_PIN_11 IOPORT_CREATE_PIN(PIOB, 4)
#define EXT3_PIN_12 IOPORT_CREATE_PIN(PIOB, 5)
#define EXT3_PIN_13 IOPORT_CREATE_PIN(PIOB, 2)
#define EXT3_PIN_14 IOPORT_CREATE_PIN(PIOB, 3)
#define EXT3_PIN_15 IOPORT_CREATE_PIN(PIOA, 10)
#define EXT3_PIN_16 IOPORT_CREATE_PIN(PIOA, 13)
#define EXT3_PIN_17 IOPORT_CREATE_PIN(PIOA, 12)
#define EXT3_PIN_18 IOPORT_CREATE_PIN(PIOA, 14)
//@}

/** @} */

/** @} */

#endif /* SAM4S_XPLAINED_PRO_H */
