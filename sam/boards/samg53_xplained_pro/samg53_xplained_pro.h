/**
 * \file
 *
 * \brief SAMG53 Xplained Pro board definition
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

#ifndef SAMG53_XPLAINED_PRO_H_INCLUDED
#define SAMG53_XPLAINED_PRO_H_INCLUDED

#include <conf_board.h>
#include <compiler.h>

/**
 * \ingroup group_common_boards
 * \defgroup samg53_xplained_pro_group SAMG53 Xplained Pro board
 *
 * @{
 */

void system_board_init(void);

/**
 * \defgroup samg53_config_group Configuration
 *
 * Symbols to use for configuring the board and its initialization.
 *
 * @{
 */
#ifdef __DOXYGEN__

/* ! \name Initialization */
/* @{ */

/**
 * \def CONF_BOARD_KEEP_WATCHDOG_AT_INIT
 * \brief If defined, the watchdog will remain enabled
 *
 * If this symbol is defined, the watchdog is left running with its current
 * configuration. Otherwise, it is disabled during board initialization.
 */
# ifndef CONF_BOARD_KEEP_WATCHDOG_AT_INIT
#  define CONF_BOARD_KEEP_WATCHDOG_AT_INIT
# endif

/* @} */

#endif /* __DOXYGEN__ */
/**@} */

/**
 * \defgroup samg53_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** Name string macro */
#define BOARD_NAME                "SAMG53_XPLAINED_PRO"

/** \name Resonator definitions
 *  @{ */
#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_BYPASS  0 /* Not Mounted */
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
/*TBD startup time needs to be adjusted according to measurements */
#define BOARD_OSC_STARTUP_US      15625

/** @} */

/** \name LED0 definitions
 *  @{ */
#define LED0_GPIO     (PIO_PA16_IDX)
#define LED0_FLAGS    (PIO_OUTPUT_1 | PIO_DEFAULT)

#define LED0_PIN                  IOPORT_CREATE_PIN(PIOA, 16)
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name SW0 definitions
 *  @{ */
#define SW0_PIN                   IOPORT_CREATE_PIN(PIOA, 2)
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_SUPC_INPUT            2
/** @} */

/**
 * \name LED #0 definitions
 *
 * Wrapper macros for LED0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define LED_0_NAME                "LED0 (yellow)"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 1

/**
 * \name Button #0 definitions
 *
 * Wrapper macros for SW0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define BUTTON_0_NAME             "SW0"
#define BUTTON_0_PIN              SW0_PIN
#define BUTTON_0_ACTIVE           SW0_ACTIVE
#define BUTTON_0_INACTIVE         SW0_INACTIVE
#define BUTTON_0_SUPC_INPUT       SW0_SUPC_INPUT
/** @} */

/** Number of on-board buttons */
#define BUTTON_COUNT 1

/** \name Extension header #1 pin definitions
 *  @{
 */
#define EXT1_PIN_3                IOPORT_CREATE_PIN(PIOA, 17)
#define EXT1_PIN_4                IOPORT_CREATE_PIN(PIOA, 18)
#define EXT1_PIN_5                IOPORT_CREATE_PIN(PIOB, 3)
#define EXT1_PIN_6                IOPORT_CREATE_PIN(PIOB, 9)
#define EXT1_PIN_7                IOPORT_CREATE_PIN(PIOA, 0)
#define EXT1_PIN_8                IOPORT_CREATE_PIN(PIOA, 1)
#define EXT1_PIN_9                IOPORT_CREATE_PIN(PIOA, 24)
#define EXT1_PIN_10               IOPORT_CREATE_PIN(PIOA, 20)
#define EXT1_PIN_11               IOPORT_CREATE_PIN(PIOB, 0)
#define EXT1_PIN_12               IOPORT_CREATE_PIN(PIOB, 1)
#define EXT1_PIN_13               IOPORT_CREATE_PIN(PIOA, 5)
#define EXT1_PIN_14               IOPORT_CREATE_PIN(PIOA, 6)
#define EXT1_PIN_15               IOPORT_CREATE_PIN(PIOA, 11)
#define EXT1_PIN_16               IOPORT_CREATE_PIN(PIOA, 13)
#define EXT1_PIN_17               IOPORT_CREATE_PIN(PIOA, 12)
#define EXT1_PIN_18               IOPORT_CREATE_PIN(PIOA, 14)
/** @} */

/** \name Extension header #1 pin definitions by function
 *  @{
 */
#define EXT1_PIN_ADC_0            EXT1_PIN_3
#define EXT1_PIN_ADC_1            EXT1_PIN_4
#define EXT1_PIN_GPIO_0           EXT1_PIN_5
#define EXT1_PIN_GPIO_1           EXT1_PIN_6
#define EXT1_PIN_PWM_0            EXT1_PIN_7
#define EXT1_PIN_PWM_1            EXT1_PIN_8
#define EXT1_PIN_IRQ              EXT1_PIN_9
#define EXT1_PIN_I2C_SDA          EXT1_PIN_11
#define EXT1_PIN_I2C_SCL          EXT1_PIN_12
#define EXT1_PIN_UART_RX          EXT1_PIN_13
#define EXT1_PIN_UART_TX          EXT1_PIN_14
#define EXT1_PIN_SPI_SS_1         EXT1_PIN_10
#define EXT1_PIN_SPI_SS_0         EXT1_PIN_15
#define EXT1_PIN_SPI_MOSI         EXT1_PIN_16
#define EXT1_PIN_SPI_MISO         EXT1_PIN_17
#define EXT1_PIN_SPI_SCK          EXT1_PIN_18
/** @} */

/** \name Extension header #1 ADC definitions
 *  @{
 */
#define EXT1_ADC_MODULE           ADC
#define EXT1_ADC_0_CHANNEL        0
#define EXT1_ADC_1_CHANNEL        1
/** @} */

/** \name Extension header #1 PWM definitions
 *  @{
 */
#define EXT1_PWM_MODULE           TC0
#define EXT1_PWM_0_CHANNEL        0
#define EXT1_PWM_0_MUX            IOPORT_MODE_MUX_B
#define EXT1_PWM_1_CHANNEL        1
#define EXT1_PWM_1_MUX            IOPORT_MODE_MUX_B
/** @} */

/** \name Extension header #1 IRQ/External interrupt definitions
 *  @{
 */
#define EXT1_IRQ_MODULE           SUPC
#define EXT1_IRQ_INPUT            11
/** @} */

/** \name Extension header #1 I2C definitions
 *  @{
 */
#define EXT1_TWI_MODULE           TWI2
#define EXT1_TWI_TWD_MUX          IOPORT_MODE_MUX_B
#define EXT1_TWI_TWCK_MUX         IOPORT_MODE_MUX_B
/** @} */

/** \name Extension header #1 UART definitions
 *  @{
 */
#define EXT1_UART_MODULE          USART
#define EXT1_UART_RXD_MUX         IOPORT_MODE_MUX_A
#define EXT1_UART_TXD_MUX         IOPORT_MODE_MUX_A
/** @} */

/** \name Extension header #1 SPI definitions
 *  @{
 */
#define EXT1_SPI_MODULE           SPI
#define EXT1_SPI_MISO_MUX         IOPORT_MODE_MUX_A
#define EXT1_SPI_MOSI_MUX         IOPORT_MODE_MUX_A
#define EXT1_SPI_SPCK_MUX         IOPORT_MODE_MUX_A
#define EXT1_SPI_NPCS0_MUX        IOPORT_MODE_MUX_A
/** @} */

/** \name Extension header #3 pin definitions
 *  @{
 */
#define EXT3_PIN_3                IOPORT_CREATE_PIN(PIOA, 19)
#define EXT3_PIN_4                IOPORT_CREATE_PIN(PIOA, 20)
#define EXT3_PIN_5                IOPORT_CREATE_PIN(PIOB, 4)
#define EXT3_PIN_7                IOPORT_CREATE_PIN(PIOA, 21)
#define EXT3_PIN_9                IOPORT_CREATE_PIN(PIOA, 22)
#define EXT3_PIN_10               IOPORT_CREATE_PIN(PIOA, 15)
#define EXT3_PIN_11               IOPORT_CREATE_PIN(PIOB, 10)
#define EXT3_PIN_12               IOPORT_CREATE_PIN(PIOB, 11)
#define EXT3_PIN_13               IOPORT_CREATE_PIN(PIOA, 9)
#define EXT3_PIN_14               IOPORT_CREATE_PIN(PIOA, 10)
#define EXT3_PIN_15               IOPORT_CREATE_PIN(PIOB, 2)
#define EXT3_PIN_16               IOPORT_CREATE_PIN(PIOA, 13)
#define EXT3_PIN_17               IOPORT_CREATE_PIN(PIOA, 12)
#define EXT3_PIN_18               IOPORT_CREATE_PIN(PIOA, 14)
/** @} */

/** \name Extension header #3 pin definitions by function
 *  @{
 */
#define EXT3_PIN_ADC_0            EXT2_PIN_3
#define EXT3_PIN_ADC_1            EXT2_PIN_4
#define EXT3_PIN_GPIO_0           EXT2_PIN_5
#define EXT3_PIN_PWM_0            EXT2_PIN_7
#define EXT3_PIN_IRQ              EXT2_PIN_9
#define EXT3_PIN_I2C_SDA          EXT2_PIN_11
#define EXT3_PIN_I2C_SCL          EXT2_PIN_12
#define EXT3_PIN_UART_RX          EXT2_PIN_13
#define EXT3_PIN_UART_TX          EXT2_PIN_14
#define EXT3_PIN_SPI_SS_1         EXT2_PIN_10
#define EXT3_PIN_SPI_SS_0         EXT2_PIN_15
#define EXT3_PIN_SPI_MOSI         EXT2_PIN_16
#define EXT3_PIN_SPI_MISO         EXT2_PIN_17
#define EXT3_PIN_SPI_SCK          EXT2_PIN_18
/** @} */

/** \name Extension header #3 ADC definitions
 *  @{
 */
#define EXT3_ADC_MODULE           ADC
#define EXT3_ADC_0_CHANNEL        2
#define EXT3_ADC_1_CHANNEL        3
/** @} */

/** \name Extension header #3 PWM definitions
 *  @{
 */
#define EXT3_PWM_MODULE           TC2
#define EXT3_PWM_0_CHANNEL        0
#define EXT3_PWM_0_MUX            IOPORT_MODE_MUX_A
/** @} */

/** \name Extension header #3 IRQ/External interrupt definitions
 *  @{
 */
#define EXT3_IRQ_MODULE           SUPC
#define EXT3_IRQ_INPUT            10
/** @} */

/** \name Extension header #3 I2C definitions
 *  @{
 */
#define EXT3_TWI_MODULE           TWI1
#define EXT3_TWI_TWD_MUX          IOPORT_MODE_MUX_A
#define EXT3_TWI_TWCK_MUX         IOPORT_MODE_MUX_A
/** @} */

/** \name Extension header #3 UART definitions
 *  @{
 */
#define EXT3_UART_MODULE          UART0
#define EXT3_UART_RXD_MUX         IOPORT_MODE_MUX_A
#define EXT3_UART_TXD_MUX         IOPORT_MODE_MUX_A
/** @} */

/** \name Extension header #3 SPI definitions
 *  @{
 */
#define EXT3_SPI_MODULE           SPI
#define EXT3_SPI_MISO_MUX         IOPORT_MODE_MUX_A
#define EXT3_SPI_MOSI_MUX         IOPORT_MODE_MUX_A
#define EXT3_SPI_SPCK_MUX         IOPORT_MODE_MUX_A
#define EXT3_SPI_NPCS1_MUX        IOPORT_MODE_MUX_B
/** @} */

/** \name Extension header #4 pin definitions
 *  @{
 */
#define EXT4_PIN_5                IOPORT_CREATE_PIN(PIOA, 15)
#define EXT4_PIN_9                IOPORT_CREATE_PIN(PIOB, 8)
#define EXT4_PIN_11               IOPORT_CREATE_PIN(PIOB, 10)
#define EXT4_PIN_12               IOPORT_CREATE_PIN(PIOB, 11)
/** @} */

/** \name Extension header #4 pin definitions by function
 *  @{
 */
#define EXT4_PIN_GPIO_0           EXT3_PIN_5
#define EXT4_PIN_IRQ              EXT3_PIN_9
#define EXT4_PIN_I2C_SDA          EXT3_PIN_11
#define EXT4_PIN_I2C_SCL          EXT3_PIN_12
/** @} */

/** \name Extension header #4 IRQ/External interrupt definitions
 *  @{
 */
#define EXT4_IRQ_MODULE           SUPC
#define EXT4_IRQ_INPUT            14
/** @} */

/** \name Extension header #4 TWI definitions
 *  @{
 */
#define EXT4_TWI_MODULE           TWI1
#define EXT4_TWI_TWD_MUX          IOPORT_MODE_MUX_A
#define EXT4_TWI_TWCK_MUX         IOPORT_MODE_MUX_A
/** @} */

/** \name Embedded debugger GPIO interface definitions
 * @{
 */
#define EDBG_GPIO0_PIN            IOPORT_CREATE_PIN(PIOA, 15)
#define EDBG_GPIO1_PIN            IOPORT_CREATE_PIN(PIOB, 3)
#define EDBG_GPIO2_PIN            IOPORT_CREATE_PIN(PIOB, 4)
#define EDBG_GPIO3_PIN            IOPROT_CREATE_PIN(PIOB, 9)
/** @} */

/** \name Embedded debugger USART interface definitions
 * @{
 */
#define EDBG_UART_MODULE          USART
#define EDBG_UART_RX_PIN          IOPORT_CREATE_PIN(PIOA, 5)
#define EDBG_UART_RX_MUX          IOPORT_MODE_MUX_A
#define EDBG_UART_TX_PIN          IOPORT_CREATE_PIN(PIOA, 6)
#define EDBG_UART_TX_MUX          IOPORT_MODE_MUX_A
/** @} */

/** \name Embedded debugger I2C interface definitions
 * @{
 */
#define EDBG_TWI_MODULE           TWI1
#define EDBG_TWI_TWD_MUX          IOPORT_MODE_MUX_A
#define EDBG_TWI_TWCK_MUX         IOPORT_MODE_MUX_A
/** @} */

/** \name Embedded debugger CDC Gateway USART interface definitions
 * @{
 */
#define EDBG_CDC_UART_MODULE      UART0
#define EDBG_CDC_UART_RX_PIN      IOPORT_CREATE_PIN(PIOA, 9)
#define EDBG_CDC_UART_RX_MUX      IOPORT_MODE_MUX_A
#define EDBG_CDC_UART_TX_PIN      IOPORT_CREATE_PIN(PIOA, 10)
#define EDBG_CDC_UART_TX_MUX      IOPORT_MODE_MUX_A
/** @} */

/** @} */

/** @} */

#endif  /* SAMG53_XPLAINED_PRO_H_INCLUDED */
