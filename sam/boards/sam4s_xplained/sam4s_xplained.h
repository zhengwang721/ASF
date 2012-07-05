/**
 * \file
 *
 * \brief SAM4S-XPLAINED Board Definition.
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

#ifndef SAM4S_XPLAINED_H_INCLUDED
#define SAM4S_XPLAINED_H_INCLUDED

#include "compiler.h"
#include "system_sam4s.h"
#include "exceptions.h"


#define BOARD_REV_A
/*
#define BOARD_REV_B
*/
/*----------------------------------------------------------------------------*/
/**
 *  \page sam4s_xplained_opfreq "SAM4S-XPLAINED - Operating frequencies"
 *  This page lists several definition related to the board operating frequency
 *
 *  \section Definitions
 *  - \ref BOARD_FREQ_*
 *  - \ref BOARD_MCK
 */

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL		(32768U)
#define BOARD_FREQ_SLCK_BYPASS		(32768U)
#define BOARD_FREQ_MAINCK_XTAL		(12000000U)
#define BOARD_FREQ_MAINCK_BYPASS	(12000000U)

/** Master clock frequency */
#define BOARD_MCK					CHIP_FREQ_CPU_MAX

/*----------------------------------------------------------------------------*/
/**
 * \page sam4s_xplained_board_info "SAM4S-XPLAINED - Board informations"
 * This page lists several definition related to the board description.
 *
 * \section Definitions
 * - \ref BOARD_NAME
 */

/** Name of the board */
#define BOARD_NAME "SAM4S-XPLAINED"
/** Board definition */
#define sam4sxplained
/** Family definition (already defined) */
#define sam4s
/** Core definition */
#define cortexm3

/*----------------------------------------------------------------------------*/
/**
 * \page sam4s_xplained_piodef "SAM4S-XPLAINED - PIO definitions"
 * This pages lists all the pio definitions. The constants
 * are named using the following convention: PIN_* for a constant which defines
 * a single Pin instance (but may include several PIOs sharing the same
 * controller), and PINS_* for a list of Pin instances.
 *
 * ADC
 * - \ref PIN_ADC0_AD0
 * - \ref PIN_ADC0_AD1
 * - \ref PIN_ADC0_AD2
 * - \ref PIN_ADC0_AD3
 * - \ref PIN_ADC0_AD4
 * - \ref PIN_ADC0_AD5
 * - \ref PIN_ADC0_AD6
 * - \ref PIN_ADC0_AD7
 * - \ref PINS_ADC
 *
 * UART
 * - \ref PINS_UART
 *
 * EBI
 * - \ref PIN_EBI_DATA_BUS
 * - \ref PIN_EBI_NRD
 * - \ref PIN_EBI_NWE
 * - \ref PIN_EBI_NCS0
 * - \ref PIN_EBI_PSRAM_ADDR_BUS
 * - \ref PIN_EBI_PSRAM_NBS
 * - \ref PIN_EBI_A1
 * - \ref PIN_EBI_NCS1
 *
 * LEDs
 * - \ref PIN_LED_0
 * - \ref PIN_LED_1
 * - \ref PINS_LEDS
 *
 * Push buttons
 * - \ref PIN_PUSHBUTTON_1
 * - \ref PINS_PUSHBUTTONS
 * - \ref PUSHBUTTON_BP1
 *
 * SPI
 * - \ref PIN_SPI_MISO
 * - \ref PIN_SPI_MOSI
 * - \ref PIN_SPI_SPCK
 * - \ref PINS_SPI
 * - \ref PIN_SPI_NPCS0_PA11
 *
 * SSC
 * - \ref PIN_SSC_TD
 * - \ref PIN_SSC_TK
 * - \ref PIN_SSC_TF
 * - \ref PINS_SSC_CODEC
 *
 * PCK0
 * - \ref PIN_PCK0
 *
 * PIO PARALLEL CAPTURE
 * - \ref PIN_PIODCEN1
 * - \ref PIN_PIODCEN2
 *
 * TWI
 * - \ref TWI_V3XX
 * - \ref PIN_TWI_TWD0
 * - \ref PIN_TWI_TWCK0
 * - \ref PINS_TWI0
 * - \ref PIN_TWI_TWD1
 * - \ref PIN_TWI_TWCK1
 * - \ref PINS_TWI1
 *
 * USART0
 * - \ref PIN_USART0_RXD
 * - \ref PIN_USART0_TXD
 * - \ref PIN_USART0_CTS
 * - \ref PIN_USART0_RTS
 * - \ref PIN_USART0_SCK
 *
 *
 */

/** ADC_AD0 pin definition. */
#define PIN_ADC0_AD0 {1 << 21, PIOA, ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/** ADC_AD1 pin definition. */
#define PIN_ADC0_AD1 {1 << 30, PIOA, ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/** ADC_AD2 pin definition. */
#define PIN_ADC0_AD2 {1 << 3, PIOB, ID_PIOB, PIO_INPUT, PIO_DEFAULT}
/** ADC_AD3 pin definition. */
#define PIN_ADC0_AD3 {1 << 4, PIOB, ID_PIOB, PIO_INPUT, PIO_DEFAULT}
/** ADC_AD4 pin definition. */
#define PIN_ADC0_AD4 {1 << 15, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}
/** ADC_AD5 pin definition. */
#define PIN_ADC0_AD5 {1 << 16, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}
/** ADC_AD6 pin definition. */
#define PIN_ADC0_AD6 {1 << 17, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}
/** ADC_AD7 pin definition. */
#define PIN_ADC0_AD7 {1 << 18, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}

/** Pins ADC */
#define PINS_ADC PIN_ADC0_AD0, PIN_ADC0_AD1, PIN_ADC0_AD2, PIN_ADC0_AD3, PIN_ADC0_AD4, PIN_ADC0_AD5, PIN_ADC0_AD6, PIN_ADC0_AD7
#define PINS_ADC_TRIG  PIO_PA8_IDX
#define PINS_ADC_TRIG_FLAG  (PIO_PERIPH_B | PIO_DEFAULT)

/** Startup time max, return from Idle mode (in µs) */
#define ADC_STARTUP_TIME_MAX       15
/** Track and hold Acquisition Time min (in ns) */
#define ADC_TRACK_HOLD_TIME_MIN  1200
/** ADC clock frequency */
#define BOARD_ADC_FREQ     (6000000)

/** UART pins (UTXD0 and URXD0) definitions, PA9,10. */
#define PINS_UART		(PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART_FLAGS	(PIO_PERIPH_A | PIO_DEFAULT)

#define PINS_UART_MASK PIO_PA9A_URXD0|PIO_PA10A_UTXD0
#define PINS_UART_PIO PIOA
#define PINS_UART_ID ID_PIOA
#define PINS_UART_TYPE PIO_PERIPH_A
#define PINS_UART_ATTR PIO_DEFAULT

/** EBI Data Bus pins */
#define PIN_EBI_DATA_BUS_D0        PIO_PC0_IDX
#define PIN_EBI_DATA_BUS_D1        PIO_PC1_IDX
#define PIN_EBI_DATA_BUS_D2        PIO_PC2_IDX
#define PIN_EBI_DATA_BUS_D3        PIO_PC3_IDX
#define PIN_EBI_DATA_BUS_D4        PIO_PC4_IDX
#define PIN_EBI_DATA_BUS_D5        PIO_PC5_IDX
#define PIN_EBI_DATA_BUS_D6        PIO_PC6_IDX
#define PIN_EBI_DATA_BUS_D7        PIO_PC7_IDX
#define PIN_EBI_DATA_BUS_FLAGS           PIO_PERIPH_A | PIO_PULLUP
#define PIN_EBI_DATA_BUS_MASK  0xFF
#define PIN_EBI_DATA_BUS_PIO  PIOC
#define PIN_EBI_DATA_BUS_ID  ID_PIOC
#define PIN_EBI_DATA_BUS_TYPE PIO_PERIPH_A
#define PIN_EBI_DATA_BUS_ATTR PIO_PULLUP
/** EBI NRD pin */
#define PIN_EBI_NRD                 PIO_PC11_IDX
#define PIN_EBI_NRD_FLAGS       PIO_PERIPH_A | PIO_PULLUP
#define PIN_EBI_NRD_MASK  1 << 11
#define PIN_EBI_NRD_PIO  PIOC
#define PIN_EBI_NRD_ID  ID_PIOC
#define PIN_EBI_NRD_TYPE PIO_PERIPH_A
#define PIN_EBI_NRD_ATTR PIO_PULLUP
/** EBI NWE pin */
#define PIN_EBI_NWE                  PIO_PC8_IDX
#define PIN_EBI_NWE_FLAGS       PIO_PERIPH_A | PIO_PULLUP
#define PIN_EBI_NWE_MASK  1 << 8
#define PIN_EBI_NWE_PIO  PIOC
#define PIN_EBI_NWE_ID  ID_PIOC
#define PIN_EBI_NWE_TYPE PIO_PERIPH_A
#define PIN_EBI_NWE_ATTR PIO_PULLUP
/** EBI NCS0 pin */
#define PIN_EBI_NCS0                PIO_PC14_IDX
#define PIN_EBI_NCS0_FLAGS     PIO_PERIPH_A | PIO_PULLUP
#define PIN_EBI_NCS0_MASK  1 << 14
#define PIN_EBI_NCS0_PIO  PIOC
#define PIN_EBI_NCS0_ID  ID_PIOC
#define PIN_EBI_NCS0_TYPE PIO_PERIPH_A
#define PIN_EBI_NCS0_ATTR PIO_PULLUP

/** EBI NLB pin */
#define PIN_EBI_NLB           PIO_PC16_IDX
#define PIN_EBI_NLB_FLAGS     PIO_OUTPUT_0

/** EBI address bus pins  */
#define PIN_EBI_ADDR_BUS_A0     PIO_PC18_IDX
#define PIN_EBI_ADDR_BUS_A1     PIO_PC19_IDX
#define PIN_EBI_ADDR_BUS_A2     PIO_PC20_IDX
#define PIN_EBI_ADDR_BUS_A3     PIO_PC21_IDX
#define PIN_EBI_ADDR_BUS_A4     PIO_PC22_IDX
#define PIN_EBI_ADDR_BUS_A5     PIO_PC23_IDX
#define PIN_EBI_ADDR_BUS_A6     PIO_PC24_IDX
#define PIN_EBI_ADDR_BUS_A7     PIO_PC25_IDX
#define PIN_EBI_ADDR_BUS_A8     PIO_PC26_IDX
#define PIN_EBI_ADDR_BUS_A9     PIO_PC27_IDX
#define PIN_EBI_ADDR_BUS_A10   PIO_PC28_IDX
#define PIN_EBI_ADDR_BUS_A11   PIO_PC29_IDX
#define PIN_EBI_ADDR_BUS_A12   PIO_PC30_IDX
#define PIN_EBI_ADDR_BUS_A13   PIO_PC31_IDX
#define PIN_EBI_ADDR_BUS_FLAG1  PIO_PERIPH_A | PIO_PULLUP
#define PIN_EBI_ADDR_BUS_A14   PIO_PA18_IDX
#define PIN_EBI_ADDR_BUS_A15   PIO_PA19_IDX
#define PIN_EBI_ADDR_BUS_A16   PIO_PA20_IDX
#define PIN_EBI_ADDR_BUS_A17   PIO_PA0_IDX
#define PIN_EBI_ADDR_BUS_A18   PIO_PA1_IDX
#define PIN_EBI_ADDR_BUS_FLAG2  PIO_PERIPH_C | PIO_PULLUP

/** LED pin ID. */
#define LED_BLUE      0
#define LED_BLUE_2    1

/** LED #0 pin definition (BLUE#1). */
#define LED_0_NAME           "LED D9"
#define PIN_LED_0   {PIO_PC10, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_0_MASK PIO_PC10
#define PIN_LED_0_PIO PIOC
#define PIN_LED_0_ID ID_PIOC
#define PIN_LED_0_TYPE PIO_OUTPUT_1
#define PIN_LED_0_ATTR PIO_DEFAULT

#define LED0_GPIO    (PIO_PC10_IDX)
#define LED0_FLAGS (PIO_OUTPUT_1 | PIO_DEFAULT)

/** LED #1 pin definition (Blue#2). */
#define LED_1_NAME           "LED D10"
#define PIN_LED_1   {PIO_PC17, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_1_MASK PIO_PC17
#define PIN_LED_1_PIO PIOC
#define PIN_LED_1_ID ID_PIOC
#define PIN_LED_1_TYPE PIO_OUTPUT_1
#define PIN_LED_1_ATTR PIO_DEFAULT

#define LED1_GPIO         (PIO_PC17_IDX)
#define LED1_FLAGS (PIO_OUTPUT_1 | PIO_DEFAULT)

/** Push button #0 definition. Attributes = pull-up + debounce + interrupt on rising edge. */
#ifdef BOARD_REV_A
#define PUSHBUTTON_1_NAME    "SW1"
#endif

#ifdef BOARD_REV_B
#define PUSHBUTTON_1_NAME    "BP2"
#endif
#define GPIO_PUSH_BUTTON_1   (PIO_PA5_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS    (PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)

#define PIN_PUSHBUTTON_1    {PIO_PA5, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_1_MASK PIO_PA5
#define PIN_PUSHBUTTON_1_PIO PIOA
#define PIN_PUSHBUTTON_1_ID ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

/** List of all push button definitions. */
#define PINS_PUSHBUTTONS    PIN_PUSHBUTTON_1
/** Push button #1 index. */
#define PUSHBUTTON_BP1   0


#define PIN_TC0_TIOA0		(PIO_PA0_IDX)
#define PIN_TC0_TIOA0_FLAGS	(PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_TC0_TIOA1		(PIO_PA15_IDX)
#define PIN_TC0_TIOA1_FLAGS	(PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_TC0_TIOA1_PIO     PIOA
#define PIN_TC0_TIOA1_MASK  PIO_PA15
#define PIN_TC0_TIOA1_ID      ID_PIOA
#define PIN_TC0_TIOA1_TYPE   PIO_PERIPH_B
#define PIN_TC0_TIOA1_ATTR   PIO_DEFAULT

#define PIN_TC0_TIOA2		(PIO_PA26_IDX)
#define PIN_TC0_TIOA2_FLAGS	(PIO_INPUT | PIO_DEFAULT)

#define PIN_TC0_TIOA2_PIO     PIOA
#define PIN_TC0_TIOA2_MASK  PIO_PA26
#define PIN_TC0_TIOA2_ID      ID_PIOA
#define PIN_TC0_TIOA2_TYPE   PIO_INPUT
#define PIN_TC0_TIOA2_ATTR   PIO_DEFAULT

/** SPI MISO pin definition. */
#define PIN_SPI_MISO    {PIO_PA12A_MISO, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI MOSI pin definition. */
#define PIN_SPI_MOSI    {PIO_PA13A_MOSI, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI SPCK pin definition. */
#define PIN_SPI_SPCK    {PIO_PA14A_SPCK, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI chip select pin definition. */
#define PIN_SPI_NPCS0_PA11  {PIO_PA11A_NPCS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** List of SPI pin definitions (MISO, MOSI & SPCK). */
#define PINS_SPI        PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SPCK
/** SPI MISO pin definition. */
#define SPI_MISO_GPIO 		      (PIO_PA12_IDX)
#define SPI_MISO_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO 		      (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO 		      (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)

/** SPI chip select 0 pin definition. (Only one configuration is possible) */
#define SPI_NPCS0_GPIO 		      (PIO_PA11_IDX)
#define SPI_NPCS0_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI chip select 1 pin definition. (multiple configurations are possible) */
#define SPI_NPCS1_PA9_GPIO 		  (PIO_PA9_IDX)
#define SPI_NPCS1_PA9_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS1_PA31_GPIO 	  (PIO_PA31_IDX)
#define SPI_NPCS1_PA31_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS1_PB14_GPIO 	  (PIO_PB14_IDX)
#define SPI_NPCS1_PB14_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS1_PC4_GPIO 		  (PIO_PC4_IDX)
#define SPI_NPCS1_PC4_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
/** SPI chip select 2 pin definition. (multiple configurations are possible) */
#define SPI_NPCS2_PA10_GPIO 	  (PIO_PA10_IDX)
#define SPI_NPCS2_PA10_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS2_PA30_GPIO 	  (PIO_PA30_IDX)
#define SPI_NPCS2_PA30_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS2_PB2_GPIO 		  (PIO_PB2_IDX)
#define SPI_NPCS2_PB2_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
/** SPI chip select 3 pin definition. (multiple configurations are possible) */
#define SPI_NPCS3_PA3_GPIO 		  (PIO_PA3_IDX)
#define SPI_NPCS3_PA3_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS3_PA5_GPIO 		  (PIO_PA5_IDX)
#define SPI_NPCS3_PA5_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS3_PA22_GPIO 	  (PIO_PA22_IDX)
#define SPI_NPCS3_PA22_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)

//! \name MXT143E Xplained top module
//@{
#define MXT143E_XPLAINED_TWI            TWI0
#define MXT143E_XPLAINED_SPI            SPI
#define MXT143E_XPLAINED_CS             (PIO_PA31_IDX)
#define MXT143E_XPLAINED_SCK            (PIO_PA14_IDX)
#define MXT143E_XPLAINED_MOSI           (PIO_PA13_IDX)
#define MXT143E_XPLAINED_MISO           (PIO_PA12_IDX)
#define MXT143E_XPLAINED_CHG            (PIO_PB2_IDX)
#define MXT143E_XPLAINED_DC             (PIO_PB3_IDX)
#define MXT143E_XPLAINED_BACKLIGHT      (PIO_PA17_IDX)
#define MXT143E_XPLAINED_LCD_RESET      (PIO_PC13_IDX)
#define MXT143E_XPLAINED_SDA            (PIO_PA3_IDX)
#define MXT143E_XPLAINED_SCL            (PIO_PA4_IDX)
//@}

/** SSC pin Transmitter Data (TD) */
#define PIN_SSC_TD      {PIO_PA17A_TD, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SSC pin Transmitter Clock (TK) */
#define PIN_SSC_TK      {PIO_PA16A_TK, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SSC pin Transmitter FrameSync (TF) */
#define PIN_SSC_TF      {PIO_PA15A_TF, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SSC pins definition for codec. */
#define PINS_SSC_CODEC  PIN_SSC_TD, PIN_SSC_TK, PIN_SSC_TF

/** PCK0 */
#define PIN_PCK0		(PIO_PA6_IDX)
#define PIN_PCK0_FLAGS	(PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_PCK_0_MASK PIO_PA6
#define PIN_PCK_0_PIO PIOA
#define PIN_PCK_0_ID ID_PIOA
#define PIN_PCK_0_TYPE PIO_PERIPH_B
#define PIN_PCK_0_ATTR PIO_DEFAULT
#define PIN_PCK1        {PIO_PA17B_PCK1,PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
#define PIN_PCK_1_MASK PIO_PA17
#define PIN_PCK_1_PIO PIOA
#define PIN_PCK_1_ID ID_PIOA
#define PIN_PCK_1_TYPE PIO_PERIPH_B
#define PIN_PCK_1_ATTR PIO_DEFAULT

/** PIO PARALLEL CAPTURE */
/** Parallel Capture Mode Data Enable1 */
#define PIN_PIODCEN1    PIO_PA15
/** Parallel Capture Mode Data Enable2 */
#define PIN_PIODCEN2    PIO_PA16

/** TWI ver 3.xx */
#define TWI_V3XX
/** TWI0 data pin */
#define PIN_TWI_TWD0   {PIO_PA3A_TWD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** TWI0 clock pin */
#define PIN_TWI_TWCK0  {PIO_PA4A_TWCK0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** TWI0 pins */
#define PINS_TWI0      PIN_TWI_TWD0, PIN_TWI_TWCK0
/** TWI1 data pin */
#define PIN_TWI_TWD1   {PIO_PB4A_TWD1, PIOB, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/** TWI1 clock pin */
#define PIN_TWI_TWCK1  {PIO_PB5A_TWCK1, PIOB, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/** TWI1 pins */
#define PINS_TWI1      PIN_TWI_TWD1, PIN_TWI_TWCK1

/** TWI0 pins definition */
#define TWI0_DATA_GPIO   PIO_PA3_IDX
#define TWI0_DATA_FLAGS  (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_CLK_GPIO    PIO_PA4_IDX
#define TWI0_CLK_FLAGS   (PIO_PERIPH_A | PIO_DEFAULT)

/** TWI1 pins definition */
#define TWI1_DATA_GPIO   PIO_PB4_IDX
#define TWI1_DATA_FLAGS  (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI1_CLK_GPIO    PIO_PB4_IDX
#define TWI1_CLK_FLAGS   (PIO_PERIPH_A | PIO_DEFAULT)

/** USART0 pin RX */
#define PIN_USART0_RXD    {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART0 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART0 pin RTS */
#define PIN_USART0_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART0 pin SCK */
#define PIN_USART0_SCK    {PIO_PA2B_SCK0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_SCK_IDX        (PIO_PA2_IDX)
#define PIN_USART0_SCK_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)

/** USART1 pin RX */
#define PIN_USART1_RXD    {PIO_PA21A_RXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_RXD_IDX        (PIO_PA21_IDX)
#define PIN_USART1_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin TX */
#define PIN_USART1_TXD    {PIO_PA22A_TXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_TXD_IDX        (PIO_PA22_IDX)
#define PIN_USART1_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin CTS */
#define PIN_USART1_CTS    {PIO_PA25A_CTS1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_CTS_IDX        (PIO_PA25_IDX)
#define PIN_USART1_CTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin RTS */
#define PIN_USART1_RTS    {PIO_PA24A_RTS1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_RTS_IDX        (PIO_PA24_IDX)
#define PIN_USART1_RTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin ENABLE */
#define PIN_USART1_EN     {PIO_PA23A_SCK1, PIOA, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_USART1_EN_IDX         (PIO_PA23_IDX)
#define PIN_USART1_EN_FLAGS       (PIO_OUTPUT_0 | PIO_DEFAULT)
/** USART1 pin SCK */
#define PIN_USART1_SCK    {PIO_PA23A_SCK1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_SCK_IDX        (PIO_PA23_IDX)
#define PIN_USART1_SCK_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)

/** Definition of MMA7341L x,y,z axis channel number */
#define MMA7341L_ADC_CHANNEL_X  2
#define MMA7341L_ADC_CHANNEL_Y  6
#define MMA7341L_ADC_CHANNEL_Z  7

/** MMA7341L mode set pin definition. */
#define PIN_MMA7341L_MODE                PIO_PC13_IDX
#define PIN_MMA7341L_MODE_FLAG       PIO_OUTPUT_1 | PIO_DEFAULT

/** MMA7341L X,Y,Z axis pin definition. */
#define PIN_MMA7341L_X_AXIS                PIO_PB3_IDX
#define PIN_MMA7341L_X_AXIS_FLAG       PIO_INPUT | PIO_DEFAULT
#define PIN_MMA7341L_Y_AXIS                PIO_PC17_IDX
#define PIN_MMA7341L_Y_AXIS_FLAG       PIO_INPUT | PIO_DEFAULT
#define PIN_MMA7341L_Z_AXIS                PIO_PC18_IDX
#define PIN_MMA7341L_Z_AXIS_FLAG       PIO_INPUT | PIO_DEFAULT
/*----------------------------------------------------------------------------*/
/**
 * \page sam4s_xplained_chipdef "SAM4S-XPLAINED - Individual chip definition"
 * This page lists the definitions related to different chip's definition
 * located in the board.h file for the SAM4S-XPLAINED.
 *
 * \section USART
 * - \ref BOARD_PIN_USART_RXD
 * - \ref BOARD_PIN_USART_TXD
 * - \ref BOARD_PIN_USART_CTS
 * - \ref BOARD_PIN_USART_RTS
 * - \ref BOARD_PIN_USART_EN
 * - \ref BOARD_USART_BASE
 * - \ref BOARD_ID_USART
 */

/** Rtc */
#define BOARD_RTC_ID                ID_RTC

/** USART RX pin for application */
#define BOARD_PIN_USART_RXD        PIN_USART1_RXD
/** USART TX pin for application */
#define BOARD_PIN_USART_TXD        PIN_USART1_TXD
/** USART CTS pin for application */
#define BOARD_PIN_USART_CTS        PIN_USART1_CTS
/** USART RTS pin for application */
#define BOARD_PIN_USART_RTS        PIN_USART1_RTS
/** USART ENABLE pin for application */
#define BOARD_PIN_USART_EN         PIN_USART1_EN
/** USART Base for application */
#define BOARD_USART_BASE           USART1
/** USART ID for application */
#define BOARD_ID_USART             ID_USART1

#define CONSOLE_UART               UART0
#define CONSOLE_UART_ID            ID_UART0

/* RE pin. */
#define PIN_RE_IDX                 PIN_USART1_CTS_IDX
#define PIN_RE_FLAGS               (PIO_OUTPUT_0 | PIO_DEFAULT)

/* IRDA SD pin. */
#define PIN_IRDA_SD_IDX            PIN_USART1_CTS_IDX
#define PIN_IRDA_SD_FLAGS          (PIO_OUTPUT_1 | PIO_DEFAULT)

/* TXD pin configuration. */
#define PIN_USART_TXD_IDX          PIN_USART1_TXD_IDX
#define PIN_USART_TXD_FLAGS        (PIO_PERIPH_A | PIO_DEFAULT)
#define PIN_USART_TXD_IO_FLAGS     (PIO_OUTPUT_0 | PIO_DEFAULT)

#endif  /* SAM4S_XPLAINED_H_INCLUDED */
