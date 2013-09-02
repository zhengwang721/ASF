/**
 * \file
 *
 * \brief SERCOM SPI master with vectored I/O driver configuration
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

#ifndef CONF_SPI_MASTER_VEC_H
#define CONF_SPI_MASTER_VEC_H

#include <board.h>

#define CONF_SPI_MASTER_VEC_SERCOM       EXT1_SPI_MODULE
#define CONF_SPI_MASTER_VEC_BAUDRATE     1000000
#define CONF_SPI_MASTER_VEC_SS_PIN       EXT1_PIN_SPI_SS_0
#define CONF_SPI_MASTER_VEC_GCLK_SOURCE  (GCLK_GENERATOR_0)
#define CONF_SPI_MASTER_VEC_SIGNAL_MUX   (1 << SERCOM_SPI_CTRLA_DOPO_Pos) /* SPI_SIGNAL_MUX_SETTING_E */

#define CONF_SPI_MASTER_VEC_PINMUX_PAD0  EXT1_SPI_SERCOM_PINMUX_PAD0
#define CONF_SPI_MASTER_VEC_PINMUX_PAD1  EXT1_SPI_SERCOM_PINMUX_PAD1
#define CONF_SPI_MASTER_VEC_PINMUX_PAD2  EXT1_SPI_SERCOM_PINMUX_PAD2
#define CONF_SPI_MASTER_VEC_PINMUX_PAD3  EXT1_SPI_SERCOM_PINMUX_PAD3

#endif /* CONF_SPI_MASTER_VEC_H */