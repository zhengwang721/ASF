//﻿/*
//* \file
//*
//* \brief The EPD configurations
//*
//* Copyright (c) 2012-2013 Pervasive Displays Inc. All rights reserved.
//*
//*  Authors: Pervasive Displays Inc.
//*
//*  Redistribution and use in source and binary forms, with or without
//*  modification, are permitted provided that the following conditions
//*  are met:
//*
//*  1. Redistributions of source code must retain the above copyright
//*     notice, this list of conditions and the following disclaimer.
//*  2. Redistributions in binary form must reproduce the above copyright
//*     notice, this list of conditions and the following disclaimer in
//*     the documentation and/or other materials provided with the
//*     distribution.
//*
//*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*/

#ifndef CONF_EPD_H_INCLUDED
#define CONF_EPD_H_INCLUDED

/** Define EPD_KIT_TOOL_FUNCTIONS if the code will work with EPD Kit Tool. 
 * \note Comment out the #define below if you won't work with EPD Kit tool and 
 *       just want to show two images change alternately by predefined image array.
 */
//#define EPD_KIT_TOOL_FUNCTIONS

/** \brief Supported COG version
 *
 * \note
 * - place the value here to assign which COG driving waveform will be used.
 * - now=COG_V110, future=COG_V110_G2, COG_V230, etc. */
#define COG_V110

/** The SPI frequency of this kit (12MHz) */
#define COG_SPI_baudrate	12000000

/** Define the number of ram buffer for system packet used interchangeably*/
#define BUFFER_SIZE	2

/** System Packet length=6~64, maximum=64
* ========================
* | Byte | Name          |
* |------|---------------|
* | 0    | Packet Header |
* | 1    | Packet Length |
* | 2-3  | Kit ID        |
* | 4    | Command Type  |
* | 5-62 | Data          |
* | 63   | CRC           |
*/
#define PAYLOAD_SIZE 64

/** Firmware version to display on EPD Kit Tool  */
#define EPD_KIT_TOOL_VERSION	"1.00 "

/** \brief Kit ID for recognizing by EPD Kit Tool
 *
 * \note 
 * - High byte is IC vendor, low byte is the name of supported development kit
 * - Atmel=05, Atmel SAM4L Xplained PRO=01 */
#define EPD_KIT_TOOL_KIT_ID	(uint16_t)0x0501


/******************************************************************************
* GPIO Defines
*****************************************************************************/
//#define EPD_Temper_PIN      IOPORT_CREATE_PIN (PORTF,0); /**< MEGA256RFR2 EXT1.pin3.  PF0, ADC0 */
#define EPD_BORDER_PIN      IOPORT_CREATE_PIN (PORTF,1) /**< MEGA256RFR2 EXT1.pin4.  PF1 ADC1*/
#define EPD_DISCHARGE_PIN   IOPORT_CREATE_PIN (PORTE,2) /**< MEGA256RFR2 EXT1.pin5.  PB7 */
#define EPD_RST_PIN         IOPORT_CREATE_PIN (PORTE,3) /**< MEGA256RFR2 EXT1.pin6.  dummy */
#define EPD_PWM_PIN         IOPORT_CREATE_PIN (PORTB,5) /**< MEGA256RFR2 EXT1.pin7.  PB5, OC1A */
#define EPD_PANELON_PIN     IOPORT_CREATE_PIN (PORTB,6) /**< MEGA256RFR2 EXT1.pin8.  PA09 */
#define EPD_BUSY_PIN        IOPORT_CREATE_PIN (PORTE,5) /**< MEGA256RFR2 EXT1.pin9.  PE5 */
//#define Flash_CS_PIN        IOPORT_CREATE_PIN (PORTD,5) /**< MEGA256RFR2 EXT1.pin10. PD5 */
#define EPD_SPI_CS_PIN      IOPORT_CREATE_PIN (PORTG,0) /**< MEGA256RFR2 EXT1.pin15. PG0 */
#define EPD_SPI_SS_PIN		IOPORT_CREATE_PIN (PORTG,0) /**< MEGA256RFR2 EXT1.pin15. PG0 */
#define EPD_SPI_MOSI_PIN    IOPORT_CREATE_PIN (PORTB,2) /**< MEGA256RFR2 EXT1.pin16. PB2 */
#define EPD_SPI_MISO_PIN    IOPORT_CREATE_PIN (PORTB,3) /**< MEGA256RFR2 EXT1.pin17. PB3 */
#define EPD_SPI_CLK_PIN     IOPORT_CREATE_PIN (PORTB,1) /**< MEGA256RFR2 EXT1.pin18. PB1, UART2_SPI */



/******************************************************************************
* EPD Hardware module Defines
*****************************************************************************/
/**PWM Define ****************************************************************/
#define EPD_TC_TIMER_ID             &TCCR1A						/**< Timer/Counter(TC) to waveform mode */
#define EPD_TC_TIMER_IRQn           TIMER1_OVF_vect_num			/**< Timer/Counter(TC) Interrupt Number */
#define EPD_TC_TIMER_CHANNEL		TC_COMPA					/**< Timer/Counter(TC) Compare Trigger Channel*/
#define EPD_TC_WAVEFORM_CHANNEL     TC_COMPA					/**< Timer/Counter(TC) Channel */
#define EPD_TC_WAVEFORM_ID          &TCCR1A						/**< TC0 mode */
#define EPD_TC_WAVEFORM_PIN         EPD_PWM_PIN						/**< TC0 signal: A0 on PA08 mux B */
#define EPD_TC_WAVEFORM_PIN_FLAGS   IOPORT_MODE_PULLUP \
									| IOPORT_INIT_HIGH \
									| IOPORT_DIR_OUTPUT         /**< TC0 signal: A0 on PA08 mux B */

#define EPD_TC_ClockSignalSel				TC_CLKSEL_DIV1_gc			/**< (TC_CMR) TIMER_CLOCK2 */
#define EPD_TC_WAVEFORM_PWM_FREQUENCY		100000						/**< PWM frequency = 100K Hz */
#define EPD_TC_WAVEFORM_PWM_DUTY_CYCLE		50							/**< PWM duty cycle = 50% */

/**SPI Defines ****************************************************************/
#define EPD_SPI_ID					&SPCR	                    /**< use USART2 */
#define EPD_SPI_baudrate            COG_SPI_baudrate			/**< the baud rate of SPI */
//#define EPD_SPI_CLK_PIN			EPD_SPI_CLK_PIN				/**< USART2 signal: CLK on PA18 mux A */
//#define EPD_SPI_MOSI_PIN			EPD_SPI_MOSI_PIN			/**< USART2 signal: TXD on PA20 mux A */
//#define EPD_SPI_MISO_PIN			EPD_SPI_MOSI_PIN			/**< USART2 signal: RXD on PA19 mux A */
//#define EPD_SPI_SS_PIN			EPD_SPI_SS_PIN				/**< USART2 signal: RXD on PA19 mux A */


#define EPD_SPI_CLK_MUX             IOPORT_INIT_HIGH \
									| IOPORT_DIR_OUTPUT			/**< USART2 signal: CLK on PA18 mux A */
#define EPD_SPI_MOSI_MUX            IOPORT_INIT_HIGH \
									| IOPORT_DIR_OUTPUT			/**< USART2 signal: TXD on PA20 mux A */
#define EPD_SPI_MISO_MUX            IOPORT_DIR_INPUT			/**< USART2 signal: RXD on PA19 mux A */
#define EPD_SPI_SS_MUX				IOPORT_INIT_HIGH \
									| IOPORT_DIR_OUTPUT
/**Temperature ADC Defines****************************************************/
#define EPD_Temperature_Sensor_ADC  0 //ADC_MUXPOS_4 /**< MUX selection on Positive ADC input channel 4 */
#define EPD_TempeScaled            2.5f  /**< the scale of temperature circuit */
#define EPD_ADCRefVcc              1.65f /**< ADC ref voltage = VCC/2 = 3.3/2 */
#define EPD_ADCres                 (float)pow(2,12) /**< 2 ^12 */
#define EPD_DegCOffset             5.5f  /**< the offset of SAM4L MCU */
#define EPD_ADCSampleCount         16    /**< ADC sampling counter */

 
#endif /* CONF_EPD_H_INCLUDED */