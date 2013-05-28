/**
 * \file
 *
 * \brief PARC example configuration.
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

#ifndef CONF_EXAMPLE_H_INCLUDED
#define CONF_EXAMPLE_H_INCLUDED

#include "conf_board.h"
#include "conf_clock.h"
#include "ioport.h"

#define BOARD_ID_USART             COM_PORT_USART

#define BOARD_USART                COM_PORT_USART

#define BOARD_USART_BAUDRATE       115200

#define USART_Handler              USART1_Handler

#define USART_IRQn                 USART1_IRQn

#define PIN_PCCK_INPUT             PIN_PA06
#define PIN_PCEN1_INPUT            PIN_PC17
#define PIN_PCEN2_INPUT            PIN_PC18
#define PIN_PCDATA_ESCAPE          PIN_PC05


/**
 * \brief configure the GPIO simulation output port connected to PARC port.
 *
 *  The connections on SAM4L Xplained Pro or SAM4L8 Xplained Pro should be:
 *  EXT3-P9 (PIN_PA06) -- EXT3-P15(PCCK)
 *  EXT1-P7 (PIN_PC00) -- EXT3-P8 (PCDATA0)
 *  EXT1-P8 (PIN_PC01) -- EXT3-P10(PCDATA1)
 *  EXT1-P6 (PIN_PC02) -- EXT4-P15(PCDATA2)
 *  EXT1-P15(PIN_PC03) -- EXT4-P7 (PCDATA3)
 *  EXT2-P7 (PIN_PC04) -- EXT4-P8 (PCDATA4)
 *  EXT2-P8 (PIN_PC05) -- EXT4-P10(PCDATA6)
 *  EXT2-P9 (PIN_PC06) -- EXT4-P9 (PCDATA7)
 *  EXT4-P5 (PIN_PC17) -- EXT4-P18(PCEN1)
 *  EXT4-P6 (PIN_PC18) -- EXT4-P17(PCEN2)
 *  Please note the PCDATA5 is only connected to LCD connector (EXT5) 
 *  which can not be connected easily by plugging wires. So in this example
 *  PCDATA5 is nor required to be connected.
 */
static inline void parc_port_source_simulation_config(void)
{
	ioport_set_pin_dir(PIN_PA06, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PA06,IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_dir(PIN_PC00, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PC00,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PIN_PC01, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PC01,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PIN_PC02, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PC02,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PIN_PC03, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PC03,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PIN_PC04, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PC04,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PIN_PC05, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PC05,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PIN_PC06, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PC06,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PIN_PCEN1_INPUT, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PCEN1_INPUT,IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_dir(PIN_PCEN2_INPUT, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_PCEN2_INPUT,IOPORT_PIN_LEVEL_LOW);
}

/**
 * \brief Place data (a byte) on GPIO port.
 *
 * * \param data   GPIO output simulation data for PARC capture. 
 */
static inline void place_data_to_port(uint32_t data)
{
    ioport_pin_t pin;
	
 	for(uint32_t i=0;i<8;i++){
		pin = PIN_PC00+i;
		if(pin == PIN_PC05){
			data = data >> 1;
		}
		if(data & 0x01){
			ioport_set_pin_level(pin,IOPORT_PIN_LEVEL_HIGH);
		}else{
			ioport_set_pin_level(pin,IOPORT_PIN_LEVEL_LOW);
		}
		data = data >> 1;	
	}
}
#endif /* CONF_EXAMPLE_H_INCLUDED */
