/**
* \file
*
* \brief The SPI, PWM, Temperature definitions of COG hardware driver
*
* Copyright (c) 2012-2013 Pervasive Displays Inc. All rights reserved.
*
*  Authors: Pervasive Displays Inc.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef 	DISPLAY_HARDWARE_DRIVCE_H_INCLUDED_
#define 	DISPLAY_HARDWARE_DRIVCE_H_INCLUDED_

#include "Pervasive_Displays_small_EPD.h"

//#include "conf_EPD.h"

///**PWM Define ****************************************************************/
//#define EPD_TC_TIMER_ID                      &TCCR3A						/**< Timer/Counter(TC) to waveform mode */
//#define EPD_TC_TIMER_IRQn                 TIMER3_OVF_vect_num			/**< Timer/Counter(TC) Interrupt Number */
//#define EPD_TC_TIMER_CHANNEL      TC_COMPA					/**< Timer/Counter(TC) Compare Trigger Channel*/
//#define EPD_TC_WAVEFORM_CHANNEL     TC_COMPA					/**< Timer/Counter(TC) Channel */
//#define EPD_TC_WAVEFORM_ID          &TCCR3A						/**< TC0 mode */
//#define EPD_TC_WAVEFORM_PIN         IOPORT_CREATE_PIN (PORTE,3)	/**< TC0 signal: A0 on PA08 mux B */ 
//#define EPD_TC_WAVEFORM_PIN_FLAGS   IOPORT_MODE_PULLUP \
								//| IOPORT_INIT_HIGH \
								//| IOPORT_DIR_OUTPUT         /**< TC0 signal: A0 on PA08 mux B */ 
//
//#define EPD_TC_ClockSignalSel		TC_CLKSEL_DIV1_gc			/**< (TC_CMR) TIMER_CLOCK2 */
//#define EPD_TC_WAVEFORM_PWM_FREQUENCY         100000						/**< PWM frequency = 100K Hz */
//#define EPD_TC_WAVEFORM_PWM_DUTY_CYCLE        50							/**< PWM duty cycle = 50% */
//
///**SPI Defines ****************************************************************/
//#define EPD_SPI_ID              &SPCR	                    /**< use USART2 */
//#define EPD_SPI_baudrate            COG_SPI_baudrate			/**< the baud rate of SPI */
//#define EPD_SPI_CLK_PIN             IOPORT_CREATE_PIN (PORTB,1)	/**< USART2 signal: CLK on PA18 mux A */
//#define EPD_SPI_MOSI_PIN            IOPORT_CREATE_PIN (PORTB,2)	/**< USART2 signal: TXD on PA20 mux A */
//#define EPD_SPI_MISO_PIN            IOPORT_CREATE_PIN (PORTB,3)	/**< USART2 signal: RXD on PA19 mux A */
//#define EPD_SPI_SS_PIN				IOPORT_CREATE_PIN (PORTB,0)	/**< USART2 signal: RXD on PA19 mux A */
//
//
//#define EPD_SPI_CLK_MUX             IOPORT_INIT_HIGH \
								//| IOPORT_DIR_OUTPUT			/**< USART2 signal: CLK on PA18 mux A */
//#define EPD_SPI_MOSI_MUX            IOPORT_INIT_HIGH \
								//| IOPORT_DIR_OUTPUT			/**< USART2 signal: TXD on PA20 mux A */
//#define EPD_SPI_MISO_MUX            IOPORT_DIR_INPUT			/**< USART2 signal: RXD on PA19 mux A */
//#define EPD_SPI_SS_MUX				IOPORT_INIT_HIGH \
								//| IOPORT_DIR_OUTPUT
								//
///**Temperature ADC Defines****************************************************/
//#define EPD_Temperature_Sensor_ADC  0 //ADC_MUXPOS_4 /**< MUX selection on Positive ADC input channel 4 */
//#define EPD_TempeScaled            2.5f  /**< the scale of temperature circuit */
//#define EPD_ADCRefVcc              1.65f /**< ADC ref voltage = VCC/2 = 3.3/2 */
//#define EPD_ADCres                 (float)pow(2,12) /**< 2 ^12 */
//#define EPD_DegCOffset             5.5f  /**< the offset of SAM4L MCU */
//#define EPD_ADCSampleCount         16    /**< ADC sampling counter */


/// Mega RF

///**PWM Define ****************************************************************/
//#define EPD_TC_TIMER_ID             &TCCR1A						/**< Timer/Counter(TC) to waveform mode */
//#define EPD_TC_TIMER_IRQn           TIMER1_OVF_vect_num			/**< Timer/Counter(TC) Interrupt Number */
//#define EPD_TC_TIMER_CHANNEL		TC_COMPA					/**< Timer/Counter(TC) Compare Trigger Channel*/
//#define EPD_TC_WAVEFORM_CHANNEL     TC_COMPA					/**< Timer/Counter(TC) Channel */
//#define EPD_TC_WAVEFORM_ID          &TCCR1A						/**< TC0 mode */
//#define EPD_TC_WAVEFORM_PIN         EPD_PWM_PIN						/**< TC0 signal: A0 on PA08 mux B */
//#define EPD_TC_WAVEFORM_PIN_FLAGS   IOPORT_MODE_PULLUP \
									//| IOPORT_INIT_HIGH \
									//| IOPORT_DIR_OUTPUT         /**< TC0 signal: A0 on PA08 mux B */
//
//#define EPD_TC_ClockSignalSel				TC_CLKSEL_DIV1_gc			/**< (TC_CMR) TIMER_CLOCK2 */
//#define EPD_TC_WAVEFORM_PWM_FREQUENCY		100000						/**< PWM frequency = 100K Hz */
//#define EPD_TC_WAVEFORM_PWM_DUTY_CYCLE		50							/**< PWM duty cycle = 50% */
//
///**SPI Defines ****************************************************************/
//#define EPD_SPI_ID              &SPCR	                    /**< use USART2 */
//#define EPD_SPI_baudrate            COG_SPI_baudrate			/**< the baud rate of SPI */
////#define EPD_SPI_CLK_PIN             EPD_SPI_CLK_PIN				/**< USART2 signal: CLK on PA18 mux A */
////#define EPD_SPI_MOSI_PIN            EPD_SPI_MOSI_PIN			/**< USART2 signal: TXD on PA20 mux A */
////#define EPD_SPI_MISO_PIN            EPD_SPI_MOSI_PIN			/**< USART2 signal: RXD on PA19 mux A */
////#define EPD_SPI_SS_PIN				EPD_SPI_SS_PIN				/**< USART2 signal: RXD on PA19 mux A */
////
//
//#define EPD_SPI_CLK_MUX             IOPORT_INIT_HIGH \
								//| IOPORT_DIR_OUTPUT			/**< USART2 signal: CLK on PA18 mux A */
//#define EPD_SPI_MOSI_MUX            IOPORT_INIT_HIGH \
								//| IOPORT_DIR_OUTPUT			/**< USART2 signal: TXD on PA20 mux A */
//#define EPD_SPI_MISO_MUX            IOPORT_DIR_INPUT			/**< USART2 signal: RXD on PA19 mux A */
//#define EPD_SPI_SS_MUX				IOPORT_INIT_HIGH \
								//| IOPORT_DIR_OUTPUT

///**Temperature ADC Defines****************************************************/
//#define EPD_Temperature_Sensor_ADC  0 //ADC_MUXPOS_4 /**< MUX selection on Positive ADC input channel 4 */
//#define EPD_TempeScaled            2.5f  /**< the scale of temperature circuit */
//#define EPD_ADCRefVcc              1.65f /**< ADC ref voltage = VCC/2 = 3.3/2 */
//#define EPD_ADCres                 (float)pow(2,12) /**< 2 ^12 */
//#define EPD_DegCOffset             5.5f  /**< the offset of SAM4L MCU */
//#define EPD_ADCSampleCount         16    /**< ADC sampling counter */


void spi_init (void);
void spi_attach (void);
void spi_detach (void);
void SPI_send (unsigned char Register, unsigned char *Data, unsigned Length);
void SPI_send_byte (uint8_t Register, uint8_t Data);
uint8_t SPI_read(unsigned char RDATA);
void SPI_write (unsigned char Data);
uint8_t SPI_write_ex (unsigned char Data);
void sys_delay_ms(unsigned int ms);
void start_EPD_timer(void);
void stop_EPD_timer(void);
uint32_t get_current_time_tick(void);
void PWM_start_toggle(void);
void PWM_stop_toggle(void);
void PWM_run(uint16_t time);
void initialize_temperature(void);
int16_t get_temperature(void);
void EPD_display_hardware_init (void);
#if (defined COG_V110_G2) || (defined COG_V230)
uint8_t SPI_R(uint8_t Register, uint8_t Data);
#endif


//// tests//
//void debug_tgl_pin();
//// test// 

#endif 	//DISPLAY_HARDWARE_DRIVCE_H_INCLUDED_
