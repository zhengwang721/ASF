/**
 * \file
 *
 * \brief API driver for component ads7843.
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
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

/**
 * \defgroup sam_component_rtouch_ads7843_group LCD Touch- ADS7843 Controller
 *
 * Low-level driver for the ADS7843 LCD Touch controller. This driver provides access to the main
 * features of the ADS7843 controller.
 *
 * \{
 */

#include "board.h"
#include "pio.h"
#include "spi_master.h"
#include "conf_spi_master.h"
#include "rtouch.h"
#include "conf_rtouch.h"
#include "conf_ads7843.h"

#define ADS_CTRL_PD0              (1 << 0)  /* PD0 */
#define ADS_CTRL_PD1              (1 << 1)  /* PD1 */
#define ADS_CTRL_DFR              (1 << 2)  /* SER/DFR */
#define ADS_CTRL_EIGHT_BITS_MOD   (1 << 3)  /* Mode */
#define ADS_CTRL_START            (1 << 7)  /* Start Bit */
#define ADS_CTRL_SWITCH_SHIFT     4  /* Address setting */

/* Get X position command */
#define CMD_Y_POSITION ((1 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START | ADS_CTRL_PD0 | ADS_CTRL_PD1)

/* Get Y position command */
#define CMD_X_POSITION ((5 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START | ADS_CTRL_PD0 | ADS_CTRL_PD1)

/* Enable penIRQ */
#define CMD_ENABLE_PENIRQ  ((1 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START)

#define ADS7843_TIMEOUT        5000000

#define DELAY_BEFORE_SPCK          200  /* 2us min (tCSS) <=> 200/100 000 000 = 2us */
#define DELAY_BETWEEN_CONS_COM     0xf  /* 5us min (tCSH) <=> (32 * 15) / (100 000 000) = 5us */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#define ADS7843_BUFSIZE 3

/**
 * \brief Generic function to send a command to the touchscreen controller.
 *
 * \param uc_cmd command to send
 *
 * \return command result
 */
static uint32_t ads7843_send_cmd(uint8_t uc_cmd)
{
	uint32_t uResult = 0;
	volatile uint32_t i;
	uint8_t data;

	/* (volatile declaration needed for code optimization by compiler) */
	volatile uint8_t bufferRX[ADS7843_BUFSIZE];
	volatile uint8_t bufferTX[ADS7843_BUFSIZE];

	bufferRX[0] = 0;
	bufferRX[1] = 0;
	bufferRX[2] = 0;

	bufferTX[0] = uc_cmd;
	bufferTX[1] = 0;
	bufferTX[2] = 0;

	for(i = 0; i < ADS7843_BUFSIZE; i++){
		spi_write_single(ADS7843_SPI_INTERFACE, bufferTX[i]);
	}
	for(i = 0; i < ADS7843_BUFSIZE; i++){
		spi_read_single(ADS7843_SPI_INTERFACE, &data);
		bufferRX[i] = data;
	}

	uResult = (uint32_t)bufferRX[1] << 8;
	uResult |= (uint32_t)bufferRX[2];
	uResult = uResult >> 4;

	return uResult;
}

/**
 * \brief Get position of the pen from the ADS7843 controller through SPI.
 *
 * \param px_pos pointer to the horizontal position
 * \param py_pos pointer to the vertical position
 *
 */
static void ads7843_get_position(rtouch_point_t *p_point)
{
	/* Get X position */
	p_point->x = ads7843_send_cmd(CMD_X_POSITION);

	/* Get Y position */
	p_point->y = ads7843_send_cmd(CMD_Y_POSITION);

	/* Switch to full power mode */
	ads7843_send_cmd(CMD_ENABLE_PENIRQ);
}

/**
 * \brief Reads and store a touchscreen measurement in the provided array.
 *
 * \param p_point  Array where the measurements will be stored
 */
void rtouch_get_raw_point(rtouch_point_t *p_point)
{
	/* Get the current position of the pressed pen */
	pio_disable_interrupt(RTOUCH_PIN_TSC_IRQ_PIO, RTOUCH_PIN_TSC_IRQ_MASK);
	ads7843_get_position(p_point);
	pio_enable_interrupt(RTOUCH_PIN_TSC_IRQ_PIO, RTOUCH_PIN_TSC_IRQ_MASK);
}

/* frequency rate for sending one bit */
#define ADS7843_SPI_BAUDRATE 1000000

/**
 * \brief Initialization of the SPI for communication with ADS7843 component.
 */
uint32_t rtouch_init_device(void)
{
	volatile uint32_t uDummy;
	struct spi_device ADS7843_SPI_DEVICE = {
		// Board specific chip select configuration
#ifdef BOARD_TSC_NPCS
		.id = BOARD_TSC_NPCS
#else
#warning The board TouchScreen chip select definition is missing. Default configuration is used.
		.id = 0
#endif
	};

	spi_master_init(ADS7843_SPI_INTERFACE);
	spi_master_setup_device(ADS7843_SPI_INTERFACE, &ADS7843_SPI_DEVICE, SPI_MODE_0, ADS7843_SPI_BAUDRATE, 0);
	spi_enable(ADS7843_SPI_INTERFACE);

	for (uDummy = 0; uDummy < 100000; uDummy++) {
	}

	ads7843_send_cmd(CMD_ENABLE_PENIRQ);

	return 0;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \}
 */
