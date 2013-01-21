/**
 * \file
 *
 * \brief SAMD20 Xplained PRO Board Definition.
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

#ifndef SAMD20_XPLAINED_PRO_H_INCLUDED
#define SAMD20_XPLAINED_PRO_H_INCLUDED

#include <compiler.h>

/** \name Board LED definitions
 *  @{
 */
#define LED0_GPIO               PIN_PB08
#define LED0_ACTIVE_LEVEL       0
/** @} */

/** \name Board Button definitions
 *  @{
 */
#define PUSH_BUTTON_0_GPIO      PIN_PB09
/** @} */

/** \name Embedded Debugger GPIO link definitions
 *  @{
 */
#define EMB_GPIO0               PIN_PB24
#define EMB_GPIO1               PIN_PB25
#define EMB_GPIO2               PIN_PB26
#define EMB_GPIO3               PIN_PB27
/** @} */

/** \name Embedded Debugger TWI link definitions
 *  @{
 */
#define EMB_TWI                 SERCOM1
#define EMB_TWI_SERCOM_INDEX    1

#define EMB_TWI_SDA             PINMUX_PA18B_SERCOM1_PAD0
#define EMB_TWI_SDA_SERCOM_PAD  0
#define EMB_TWI_SCL             PINMUX_PA19B_SERCOM1_PAD1
#define EMB_TWI_SCL_SERCOM_PAD  1
/** @} */

/** \name Embedded Debugger UART link definitions
 *  @{
 */
#define EMB_UART                SERCOM5
#define EMB_UART_SERCOM_INDEX   5

#define EMB_UART_RX             PINMUX_PB20B_SERCOM5_PAD0
#define EMB_UART_RX_SERCOM_PAD  0
#define EMB_UART_TX             PINMUX_PB22B_SERCOM5_PAD2
#define EMB_UART_TX_SERCOM_PAD  2
#define EMB_UART_SCK            PINMUX_PB23B_SERCOM5_PAD3
#define EMB_UART_SCK_SERCOM_PAD 3
/** @} */

/** \name Embedded Debugger SPI link definitions
 *  @{
 */
#define EMB_SPI                 SERCOM5
#define EMB_SPI_SERCOM_INDEX    5

#define EMB_SPI_MISO            PINMUX_PB20B_SERCOM5_PAD0
#define EMB_SPI_MISO_SERCOM_PAD 0
#define EMB_SPI_SS              PINMUX_PB21B_SERCOM5_PAD1
#define EMB_SPI_SS_SERCOM_PAD   1
#define EMB_SPI_MOSI            PINMUX_PB22B_SERCOM5_PAD2
#define EMB_SPI_MOSI_SERCOM_PAD 2
#define EMB_SPI_SCK             PINMUX_PB23B_SERCOM5_PAD3
#define EMB_SPI_SCK_SERCOM_PAD  3
/** @} */

/** \name Embedded Debugger CDC UART link definitions
 *  @{
 */
#define CDC_UART                SERCOM4
#define CDC_UART_SERCOM_INDEX   4

#define CDC_UART_RX             PINMUX_PB10B_SERCOM4_PAD2
#define CDC_UART_RX_SERCOM_PAD  2
#define CDC_UART_TX             PINMUX_PB11B_SERCOM4_PAD3
#define CDC_UART_TX_SERCOM_PAD  3
/** @} */

/** \name Embedded Debugger EXT1 header definitions
 *  @{
 */
#define EXT1_PIN3              PIN_PA04
#define EXT1_PIN4              PIN_PA05
#define EXT1_PIN5              PIN_PB04
#define EXT1_PIN6              PIN_PB05
#define EXT1_PIN7              PIN_PB00
#define EXT1_PIN8              PIN_PB01
#define EXT1_PIN9              PIN_PB06
#define EXT1_PIN10             PIN_PB07
#define EXT1_PIN11             PIN_PA18
#define EXT1_PIN12             PIN_PA19
#define EXT1_PIN13             PIN_PA11
#define EXT1_PIN14             PIN_PA10
#define EXT1_PIN15             PIN_PA15
#define EXT1_PIN16             PIN_PA12
#define EXT1_PIN17             PIN_PA14
#define EXT1_PIN18             PIN_PA13
/** @} */

/** \name Embedded Debugger EXT2 header definitions
 *  @{
 */
#define EXT2_PIN3              PIN_PA06
#define EXT2_PIN4              PIN_PA07
#define EXT2_PIN5              PIN_PB12
#define EXT2_PIN6              PIN_PB13
#define EXT2_PIN7              PIN_PA30
#define EXT2_PIN8              PIN_PA31
#define EXT2_PIN9              PIN_PB16
#define EXT2_PIN10             PIN_PB17
#define EXT2_PIN11             PIN_PA18
#define EXT2_PIN12             PIN_PA19
#define EXT2_PIN13             PIN_PA25
#define EXT2_PIN14             PIN_PA24
#define EXT2_PIN15             PIN_PA16
#define EXT2_PIN16             PIN_PA12
#define EXT2_PIN17             PIN_PA14
#define EXT2_PIN18             PIN_PA13
/** @} */

/** \name Embedded Debugger EXT3 header definitions
 *  @{
 */
#define EXT3_PIN3              PIN_PB02
#define EXT3_PIN4              PIN_PB03
#define EXT3_PIN5              PIN_PB24
#define EXT3_PIN6              PIN_PB25
#define EXT3_PIN7              PIN_PB18
#define EXT3_PIN8              PIN_PB19
#define EXT3_PIN9              PIN_PB26
#define EXT3_PIN10             PIN_PB27
#define EXT3_PIN11             PIN_PA18
#define EXT3_PIN12             PIN_PA19
#define EXT3_PIN13             PIN_PA27
#define EXT3_PIN14             PIN_PA26
#define EXT3_PIN15             PIN_PA17
#define EXT3_PIN16             PIN_PA12
#define EXT3_PIN17             PIN_PA14
#define EXT3_PIN18             PIN_PA13
/** @} */

#endif  /* SAMD20_XPLAINED_PRO_H_INCLUDED */
