/**
 * \file
 *
 * \brief Board configuration.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

/* Indicates board has an AAT3155 external component to control LCD backlight */
#define CONF_BOARD_AAT3155

/* Indicates board has an ILI9325 external component to control LCD */
#define CONF_BOARD_ILI9325

/* Indicates board has an OV7740 Image sensor  external component */
#define CONF_BOARD_OV7740_IMAGE_SENSOR

/** Configure TWI0 pins (for OV7740  communications)  */
#define CONF_BOARD_TWI0

/** Configure PCK0 pins (for OV7740  communications)  */
#define CONF_BOARD_PCK0

/* Indicates board has an SRAM external component */
#define CONF_BOARD_SRAM

/* TWI Board define */
#define ID_BOARD_TWI		    ID_TWI0
#define BOARD_TWI		        TWI0
#define BOARD_TWI_IRQn		  TWI0_IRQn

/* Push button board define */
#define PUSH_BUTTON_ID		  PIN_PUSHBUTTON_1_ID
#define PUSH_BUTTON_PIO		  PIN_PUSHBUTTON_1_PIO
#define PUSH_BUTTON_PIN_MSK	PIN_PUSHBUTTON_1_MASK
#define PUSH_BUTTON_ATTR	  PIN_PUSHBUTTON_1_ATTR

/* SRAM board define */
#define SRAM_BASE			      (0x60000000UL) // SRAM adress
#define SRAM_CS				      (0UL)
#define CAP_DEST			      (SRAM_BASE)

/* LCD board define */
#define ILI9325_LCD_CS		  (2UL) // Chip select number
#define IMAGE_WIDTH			    (320UL)
#define IMAGE_HEIGHT		    (240UL)

/* Image sensor board define */
//image sensor Power pin
#define OV_POWER_PIO		    OV_SW_OVT_PIO
#define OV_POWER_MASK		    OV_SW_OVT_MASK
//image sensor VSYNC pin
#define OV7740_VSYNC_PIO	  OV_VSYNC_PIO
#define OV7740_VSYNC_ID		  OV_VSYNC_ID
#define OV7740_VSYNC_MASK	  OV_VSYNC_MASK
#define OV7740_VSYNC_TYPE	  OV_VSYNC_TYPE
//image sensor data pin
#define OV7740_DATA_BUS_PIO	OV_DATA_BUS_PIO
#define OV7740_DATA_BUS_ID	OV_DATA_BUS_ID
//#define pio capture handler
//#define pio_capture_handler     PIOA_Handler

/* pin test */
#define PIN_TEST_GPIO		    PIO_PA5_IDX
#define PIN_TEST_FLAGS		  (PIO_OUTPUT_1 | PIO_DEFAULT)
#define PIN_TEST_ID			    ID_PIOA

/* PIO input/ouput mapping */
#define PIOA_INPUT			    (0x200802UL)
#define PIOB_INPUT			    (0xd8UL)
#define PIOC_INPUT			    (0x13200UL)
#define PIOA_OUTPUT			    (0xffbff7fbUL)
#define PIOB_OUTPUT			    (0xffffff27UL)
#define PIOC_OUTPUT			    (0xfffecdffUL)

#endif /* CONF_BOARD_H_INCLUDED */
