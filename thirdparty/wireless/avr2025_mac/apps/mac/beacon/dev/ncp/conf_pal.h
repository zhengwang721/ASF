/**
 * \file *********************************************************************
 *
 * \brief Platform Abstraction Layer configuration
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
 */

#ifndef CONF_PAL_H_INCLUDED
#define CONF_PAL_H_INCLUDED

#if SAMD20

#include "extint.h"
#include "port.h"
#define LED0 LED0_PIN
#define AT86RFX_SPI                  EXT2_SPI_MODULE
#define AT86RFX_RST_PIN              EXT2_PIN_7 //EXT2_PIN_11
#define AT86RFX_MISC_PIN             EXT2_PIN_12
#define AT86RFX_IRQ_PIN              EXT2_PIN_9 // EXT2_PIN_13
#define AT86RFX_SLP_PIN              EXT2_PIN_10 //EXT2_PIN_14
#define AT86RFX_SPI_CS               EXT2_PIN_15
#define AT86RFX_SPI_MOSI             EXT2_PIN_16
#define AT86RFX_SPI_MISO             EXT2_PIN_17
#define AT86RFX_SPI_SCK              EXT2_PIN_18

#define AT86RFX_SPI_BAUDRATE		 4000000UL

#define AT86RFX_IRQ_CHAN             EXT2_IRQ_INPUT //13
#define AT86RFX_INTC_INIT()          extint_enable(); \
                                                struct extint_chan_conf eint_chan_conf; \
                                                extint_chan_get_config_defaults(&eint_chan_conf); \
                                                eint_chan_conf.gpio_pin = AT86RFX_IRQ_PIN; \
                                                eint_chan_conf.gpio_pin_mux = EXT2_IRQ_PINMUX; \
												eint_chan_conf.gpio_pin_pull      = EXTINT_PULL_NONE; \
                                                eint_chan_conf.wake_if_sleeping    = true; \
                                                eint_chan_conf.filter_input_signal = false; \
                                                eint_chan_conf.detection_criteria  = EXTINT_DETECT_RISING; \
                                                extint_chan_set_config(AT86RFX_IRQ_CHAN, &eint_chan_conf); \
                                                extint_register_callback(AT86RFX_ISR, EXTINT_CALLBACK_TYPE_DETECT);
                                                

/** Enables the transceiver main interrupt. */
#define ENABLE_TRX_IRQ()                extint_chan_enable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/** Disables the transceiver main interrupt. */
#define DISABLE_TRX_IRQ()               extint_chan_disable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/** Clears the transceiver main interrupt. */
#define CLEAR_TRX_IRQ()                 extint_chan_clear_detected(AT86RFX_IRQ_CHAN);

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()   { extint_chan_disable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()   extint_chan_enable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT); }

/**
 * \brief Turns off the specified LEDs.
 *
 * \param led_gpio LED to turn off (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Off(led_gpio)     port_pin_set_output_level(led_gpio,true)

/**
 * \brief Turns on the specified LEDs.
 *
 * \param led_gpio LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_On(led_gpio)      port_pin_set_output_level(led_gpio,false)

/**
 * \brief Toggles the specified LEDs.
 *
 * \param led_gpio LED to toggle (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Toggle(led_gpio)  port_pin_toggle_output_level(led_gpio)
#endif

#endif /* CONF_PAL_H_INCLUDED */
