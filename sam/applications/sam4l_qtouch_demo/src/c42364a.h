/**
 * \file
 *
 * \brief Management of C42364A LCD Glass component.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#ifndef C42364A_H
#define C42364A_H


#include "compiler.h"
#include "lcdca.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup c42364a_group SODIMATEL YMCC42364AAANDCL LCD Glass component
 *
 * This is a driver for SODIMATEL YMCC42364AAANDCL LCD component.
 * This component is the custom LCD used for SAM4L-EK.
 * This driver provides functions for initialization and control of the
 * LCD segments.
 *
 * \section dependencies Dependencies
 * This driver depends on the following modules:
 * - \ref lcd_group for Liquid Crystal Display (LCD) module.
 * - \ref interrupt_group for ISR definition and disabling interrupts during
 * critical code sections.
 * @{
 */

/*! \name LCD Common
 * @{
 */
#define NB_OF_COM 4
#define NB_OF_SEG 40
//! @}

/*! \name LCD Connection Initialization
 * @{
 */
#define PORT_MASK NB_OF_SEG
#define X_BIAS    false
#define LCD_DUTY  (enum LCD_DUTY_enum)(NB_OF_COM % 4)
//! @}

/*! \name Digit Decoder
 * @{
 */
//! 7-segment with 3 commons terminals
#define FIRST_7SEG_3C  255 // 255 = unavailable
#define WIDTH_7SEG_3C  255 // 255 = unavailable
#define DIR_7SEG_3C    0   //0 = from left to right

//! 7-segment with 4 commons terminals
#define FIRST_7SEG_4C 10
#define WIDTH_7SEG_4C 5
#define DIR_7SEG_4C   1    // 1 = from right to left

//! 14-segment with 4 commons terminals
#define FIRST_14SEG_4C 36
#define WIDTH_14SEG_4C 7
#define DIR_14SEG_4C   1  // 1 = from right to left

//! 16-segment with 3 commons terminals
#define FIRST_16SEG_3C 255 // 255 = unavailable
#define WIDTH_16SEG_3C 255 // 255 = unavailable
#define DIR_16SEG_3C   0
//! @}


 /*! \name Pixel With Blinking feature
 * @{
 */

#define ICON_USB      1,0
#define ICON_COLON    0,0
#define ICON_WLESS    2,0
#define ICON_AUDIO    3,0
#define ICON_BAT      0,1
#define ICON_ERROR    1,1
#define ICON_CHINESE  2,1
#define ICON_ARM      3,1
//! @}

 /*! \name Pixel With No-Blinking feature
 * @{
 */
#define ICON_MINUS          0,37
#define ICON_MINUS_SEG1     3,30
#define ICON_MINUS_SEG2     0,33
#define ICON_DOT_1          0,29
#define ICON_DOT_2          3,10
#define ICON_DOT_3          3,8
#define ICON_DOT_4          3,6
#define ICON_BAT_LEVEL_1    0,21
#define ICON_BAT_LEVEL_2    0,17
#define ICON_BAT_LEVEL_3    0,25
#define ICON_AUDIO_PAUSE    3,38
#define ICON_AUDIO_PLAY     3,34
#define ICON_AM             3,4
#define ICON_PM             3,26
#define ICON_DEGREE_C       3,22
#define ICON_DEGREE_F       3,18
#define ICON_VOLT           3,14
#define ICON_MILLI_VOLT     0,13
//! @}


/*! \name Macro Functions
 * @{
 */

/**
 * \brief Write string to c42048a lcd glass alphanumeric field.
 *
 * This function will write the input string to the alphanumeric field of the
 * lcd glass.
 *
 * \param data Pointer to the data input string
 */
static inline void c42364a_write_alpha_packet(const uint8_t *data)
{
	lcdca_write_packet(TDG_14SEG4COM, FIRST_14SEG_4C, data, \
			WIDTH_14SEG_4C, DIR_14SEG_4C);
}

/**
 * \brief Write string to c42048a lcd glass numeric field.
 *
 * This function will write the input string to the numeric field of the
 * lcd glass.
 *
 * \param  data Pointer to the data input string
 */
static inline void c42364a_write_num_packet(const uint8_t *data)
{
	lcdca_write_packet(TDG_7SEG4COM, FIRST_7SEG_4C, data, \
			WIDTH_7SEG_4C, DIR_7SEG_4C);
}
/**
 * \brief Clear a specific LCD pixel on the LCD glass.
 *
 * This function will clear a specific LCD pixel.
 *
 *  \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 *  \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
static inline void c42364a_clear_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	lcdca_clear_pixel(pix_com,pix_seg);
}

/**
 * \brief Light on a specific LCD pixel on the LCD glass.
 *
 * This function will set a specific LCD pixel.
 *
 *  \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 *  \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
static inline void c42364a_set_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	if (pix_seg<2) {
		lcdca_clear_blink_pixel(pix_com,pix_seg);
	}
	lcdca_set_pixel(pix_com,pix_seg);
}

/**
 * \brief Blink on a specific LCD pixel on the LCD glass.
 *
 * This function will blink a specific LCD pixel.
 *
 *  \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 *  \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
static inline void c42364a_blink_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	// Be sure to blink only pixel that can do that by hardware
	if (pix_seg<2) {
		lcdca_set_blink_pixel(pix_com,pix_seg);
	}
	lcdca_blink_enable();
	lcdca_set_pixel(pix_com,pix_seg);
}

/**
 * \brief Blink the entire screen.
 *
 * This function will make the entire screen blink.
 *
 */
static inline void c42364a_set_blink_screen(void)
{
	lcdca_clear_blink_all_pixel();
	lcdca_blink_enable();
}

/**
 * \brief Blink the entire screen.
 *
 * This function will make the entire screen blink.
 *
 */
static inline void c42364a_clear_blink_screen(void)
{
	lcdca_blink_disable();
}


/*! \brief Set the LCD contrast.
 *
 *  This function allows to adjust the LCD contrast of the glass.
 *
 *  \param  fcont  -32 <= signed contrast value <= 31.
 */
static inline void c42364a_set_contrast(int8_t fcont)
{
	lcdca_set_contrast(fcont);
}

/*! \brief LCD Animation start.
 *
 *  This function start the shift register animation.
 *
 *
 *  \param  lcd_blink_rate  Hardware display blinking frequency.
 */
static inline void c42364a_start_anim(void)
{
	lcdca_csr_enable();
}
//! @}

/*! \name Function Prototypes
 * @{
 */

/**
 * \brief Initialize the LCD glass control.
 *
 * This function initializes the LCD driver to control the LCD glass.
 * It perform LCD module intialization according to the C42048 characteristics.
 *
 */
void c42364a_init(void);

/**
 * \brief Display a value on the LCD battery graph.
 *
 * This function allows to display a uint8_t value using the
 * the bargraph section of the LCD graph.
 *
 * \param val The uint8_t value to be displayed
 */
void c42364a_battery_graph(uint8_t val);

/**
 * \brief Write a decimal numeric value to LCD glass.
 *
 * This function displays an uint16 value to the LCD numeric field of the glass.
 *
 * \param val The uint16_t value to be displayed
 */
void c42364a_set_numeric_dec(uint16_t val);

/**
 * \brief Write an alphanumeric string to the alphanumeric field of the glass.
 *
 * This function displays an input text string to the alphanumeric field of LCD.
 *
 * \param data The uint16_t value to be displayed
 */
static inline void c42364a_set_text(const uint8_t *data) {
	c42364a_write_alpha_packet(data);
}

//! @}



/** @} */

#ifdef __cplusplus
}
#endif



#endif  // C42364A_H
