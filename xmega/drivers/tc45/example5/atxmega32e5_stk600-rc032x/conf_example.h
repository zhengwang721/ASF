/**
 * \file
 *
 * \brief Board- and chip-specific example configuration
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
 *
 */
#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#include <compiler.h>

// Refer to example documentation for details.


#define TIMER_EXAMPLE1        TCC4
#define TIMER_EXAMPLE2        TCC5

/*
*This example runs at default 2MHz system clock
*The prescaler for TC is 64
*Thus, 2000000/64=31250
*/
#define TIMER_EXAMPLE1_PERIOD 31250
#define TIMER_EXAMPLE2_PERIOD 15625

//#define TIMER_PORT_LED       TCD5

//#define LEDPORT                          PORTD
//#define SWITCHPORT                       PORTA
//#define EVSYS_CHMUX_EXAMPLEPORT_PIN0_gc  EVSYS_CHMUX_PORTA_PIN0_gc;

/** Using button SW0-PD0 on STK600 to trigger an interrupt */
#define EXAMPLE_BUTTON_INT    PIN_PD0

#define CONF_BOARD_TC45_OUT0_PC0
#define CONF_BOARD_TC45_OUT0_PC1
#define CONF_BOARD_TC45_OUT0_PC2
#define CONF_BOARD_TC45_OUT0_PC3
#define CONF_BOARD_TC45_OUT0_PC4
#define CONF_BOARD_TC45_OUT0_PC5
#define CONF_BOARD_TC45_OUT0_PC6
#define CONF_BOARD_TC45_OUT0_PC7

#endif /* CONF_BOARD_H */
