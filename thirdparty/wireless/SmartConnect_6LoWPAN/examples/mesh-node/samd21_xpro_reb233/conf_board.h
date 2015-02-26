/**
 * \file
 *
 * \brief Board configuration
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

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED


#define CONF_BOARD_AT86RFX
#define EXT2_CONFIG
#define AT86RFX_SPI                  EXT2_SPI_MODULE
#define AT86RFX_RST_PIN              EXT2_PIN_7
#define AT86RFX_MISC_PIN             EXT2_PIN_12
#define AT86RFX_IRQ_PIN              EXT2_PIN_9
#define AT86RFX_SLP_PIN              EXT2_PIN_10
#define AT86RFX_SPI_CS               EXT2_PIN_15
#define AT86RFX_SPI_MOSI             EXT2_PIN_16
#define AT86RFX_SPI_MISO             EXT2_PIN_17
#define AT86RFX_SPI_SCK              EXT2_PIN_18
#define AT86RFX_CSD                  EXT2_PIN_5
#define AT86RFX_CPS                  EXT2_PIN_8



#define AT86RFX_SPI_CONFIG(config) \
config.mux_setting = EXT2_SPI_SERCOM_MUX_SETTING; \
config.mode_specific.master.baudrate = AT86RFX_SPI_BAUDRATE; \
config.pinmux_pad0 = EXT2_SPI_SERCOM_PINMUX_PAD0; \
config.pinmux_pad1 = PINMUX_UNUSED; \
config.pinmux_pad2 = EXT2_SPI_SERCOM_PINMUX_PAD2; \
config.pinmux_pad3 = EXT2_SPI_SERCOM_PINMUX_PAD3;

#define AT86RFX_IRQ_CHAN       EXT2_IRQ_INPUT
#define AT86RFX_INTC_INIT()    \
struct extint_chan_conf eint_chan_conf; \
extint_chan_get_config_defaults(&eint_chan_conf); \
eint_chan_conf.gpio_pin = AT86RFX_IRQ_PIN; \
eint_chan_conf.gpio_pin_mux = EXT2_IRQ_PINMUX; \
eint_chan_conf.gpio_pin_pull      = EXTINT_PULL_NONE; \
eint_chan_conf.wake_if_sleeping    = true; \
eint_chan_conf.filter_input_signal = false; \
eint_chan_conf.detection_criteria  = EXTINT_DETECT_RISING; \
extint_chan_set_config(AT86RFX_IRQ_CHAN, &eint_chan_conf); \
extint_register_callback(AT86RFX_ISR, AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT);\
extint_chan_enable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT);
#define IC_TYPE             (0x00)

#define MCU_SOC_NAME        "ATSAMR21G18A"

#define AT86RFX_SPI_BAUDRATE		 4000000UL



#endif /* CONF_BOARD_H_INCLUDED */
