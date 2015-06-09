/**
 * \file
 *
 * \brief Management of C8263A SLCD component.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef C8263A_H_INCLUDED
#define C8263A_H_INCLUDED

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup c8263a_display_group Atmel YMCC8263AAAYDCN LCD Glass component
 *
 * This is a driver for Atmel YMCC8263AAAYDCN LCD component.
 * This component is the custom LCD used for SAM L22.
 * This driver provides functions for initialization and control of the
 * LCD segments.
 *
 * \section dependencies Dependencies
 * This driver depends on the following modules:
 * - \ref group_sam_drivers_lcdca for Liquid Crystal Display (LCD) module.
 * @{
 */

/**
 * \name Circular Animation Shift Direction
 * @{
 */

/**
 * \name Value for battery Icon setting
 * @{
 */
enum c8263a_battery_value {
	C8263A_BATTERY_NONE = 0,
	C8263A_BATTERY_ONE,
	C8263A_BATTERY_TWO,
	C8263A_BATTERY_THREE,
};
/** @} */

/**
 * \name Value for wireless Icon setting
 * @{
 */
enum c8263a_wireless_value {
	C8263A_WIRELESS_NONE = 0,
	C8263A_WIRELESS_ONE,
	C8263A_WIRELESS_TWO,
	C8263A_WIRELESS_THREE,
};
/** @} */

/** Time struct*/
struct c8263a_time {

	/** Minute value. */
	uint8_t  minute;
	/** Hour value. */
	uint8_t  hour;
	/** PM/AM value, \c true for PM, or \c false for AM. */
	bool     pm;
};

/** Char map table struct*/
typedef struct _CHAR_MAP_TB
{
	uint8_t  c;
	uint32_t value;
}CHAR_MAP;

/**
 * \name Function Prototypes
 * @{
 */

/**
 * \brief Initialize the C8263A LCD Glass component.
 *
 * This function initializes the LCD driver to control the LCD glass.
 * It perform LCD module intialization according to the C8263A characteristics.
 *
 */
status_code_t c8263a_init(void);

/**
 * \brief Show all content of the LCD glass.
 *
 * This function sets all pixels and areas of the LCD glass C8263A.
 *
 */
void c8263a_show_all(void);

/**
 * \brief Clear all content of the LCD glass.
 *
 * This function clears all pixels and areas of the LCD glass C8263A.
 *
 */
void c8263a_clear_all(void);

/**
 * \brief Clear a specific icon on the LCD glass.
 *
 * This function will clear a specific icon.
 *
 * \param icon_com  Pixel coordinate - COM - of the icon.
 * \param icon_seg  Pixel coordinate - SEG - of the icon.
 *
 * \note Use the icon define in header file.
 */
void c8263a_clear_icon(uint8_t icon_com, uint8_t icon_seg);

/**
 * \brief Show a specific icon on the LCD glass.
 *
 * This function will show a specific icon.
 *
 * \param icon_com  Pixel coordinate - COM - of the icon.
 * \param icon_seg  Pixel coordinate - SEG - of the icon.
 *
 * \note Use the icon define in header file.
 */
void c8263a_show_icon(uint8_t icon_com, uint8_t icon_seg);

/**
 * \brief Blink a specific icon on the LCD glass.
 *
 * This function will blink a specific icon.
 *
 * \param icon_com  Pixel coordinate - COM - of the icon.
 * \param icon_seg  Pixel coordinate - SEG - of the icon.
 *
 * \note Use the icon define in header file(with selected segments
 * blinking feature).
 */
void c8263a_blink_icon_start(uint8_t icon_com, uint8_t icon_seg);

/**
 * \brief Stop blink a specific icon on the LCD glass.
 *
 * This function will stop blink a specific icon.
 *
 * \param icon_com  Pixel coordinate - COM - of the icon.
 * \param icon_seg  Pixel coordinate - SEG - of the icon.
 *
 * \note Use the icon define in header file(with selected segments
 * blinking feature).
 */
void c8263a_blink_icon_stop(uint8_t icon_com, uint8_t icon_seg);

/**
 * \brief Blink the current screen content.
 *
 * This function will make the current screen blink.
 *
 */
void c8263a_blink_screen(void);

/**
 * \brief Disable all Blink.
 *
 * This function will disable all Blink content.
 *
 */
void c8263a_blink_disable(void);

/**
 * \brief Set the C8263A LCD glass contrast.
 *
 *  This function allows to adjust the contrast of the C8263A LCD glass.
 *
 *  \param contrast 0 <= contrast value <= 15.
 */
void c8263a_set_contrast(uint8_t contrast);

/**
 * \brief Start circular animation.
 *
 * This function start the circular animation.
 *
 * \param csr_dir Direction.
 * \param size Defines the size of the circular animation icon.
 * \param data Circular Shift Register Value.
 */
void c8263a_circular_animation_start(uint8_t csr_dir,
		uint8_t size, uint8_t data);

/**
 * \brief Stop circular animation.
 *
 *  This function stop the circular animation.
 */
void c8263a_circular_animation_stop(void);

/**
 * \brief Show battery condition by the battery icons on the LCD glass.
 *
 * This function allows to Show battery condition by the battery icons
 * on the LCD glass..
 *
 * \param val The 0 to 3 value which show the battery condition.
 */
void c8263a_show_battery(enum c8263a_battery_value val);

/**
 * \brief Show time on the LCD glass.
 *
 * This function allows to show time.
 *
 * \param val Time value.
 */
void c8263a_show_time(const struct c8263a_time val);

/**
 * \brief Show temperature on the LCD glass.
 *
 * This function allows to show temperature.
 *
 * \param val Temperature value.
 * \param is_celsius if \c true it will degrees celsius, else fahrenheit degree.
 */
void c8263a_show_temperature(float value,bool is_celsius);

/**
 * \brief Show wirless condition by the wirless icons on the LCD glass.
 *
 * This function allows to Show wirless condition by the battery icons
 * on the LCD glass.
 *
 * \param val The 0 to 3 value which show the wirless condition.
 */
void c8263a_show_wirless(enum c8263a_wireless_value val);

/**
 * \brief Show a decimal numeric value to LCD glass.
 *
 * This function displays an int32 value to the LCD numeric field of the glass.
 *
 * \param value The int32_t value to be displayed
 *
 * \note The value range is [-199999,199999].
 */
void c8263a_show_numeric_dec(int32_t value);

/**
 * \brief Clear C8263A LCD glass numeric field.
 *
 * This function will clear the numeric field of the LCD glass.
 */
void c8263a_clear_numeric_dec(void);

/**
 * \brief Show a float numeric value to LCD glass.
 *
 * This function displays an float value to the LCD numeric field of the glass.
 *
 * \param value The float value to be displayed
 *
 * \note The value range is [-199999,199999].
 */
void c8263a_show_numeric_float(float value);

/**
 * \brief Show text on C8263A LCD glass alphanumeric field.
 *
 * This function will show text on the alphanumeric field of the LCD glass.
 *
 * \param data Pointer to the input string(max length is 7)
 */
void c8263a_show_text(const uint8_t *data);

/**
 * \brief Clear C8263A LCD glass alphanumeric field.
 *
 * This function will clear the alphanumeric field of the LCD glass.
 */
void c8263a_clear_text(void);

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* C8263A_H_INCLUDED */
