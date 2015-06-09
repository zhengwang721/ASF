/**
 * \file
 *
 * \brief C8263A configuration.
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

/** Configuration of the C8263A LCD glass driver */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_C8263A_H_INCLUDED
#define CONF_C8263A_H_INCLUDED

/**
 * \name Circular Animation Shift Direction
 * @{
 */
/** Right Direction. */
#define C8263A_CSR_RIGHT      SLCD_CIRCULAR_SHIFT_RIGHT
/** Left Direction. */
#define C8263A_CSR_LEFT       SLCD_CIRCULAR_SHIFT_LEFT
/** @} */


/** Init contrast configuration, it's wthin [0-15]. */
#define CONF_C8263A_CONTRAST  8


/** Frame count 0 configuration.
	Prescaler is not bypassed,the overflow value is (CONF_C8263A_FC0+1).
 */
#define CONF_C8263A_FC0     2
/** Frame count 0 configuration.
	Prescaler is not bypassed,the overflow value is (CONF_C8263A_FC0+1).
 */
#define CONF_C8263A_FC1     2
/** Frame count 0 configuration.
	Prescaler is not bypassed,the overflow value is (CONF_C8263A_FC0+1).
 */
#define CONF_C8263A_FC2     1

/** Blink timer configuration. */
#define CONF_C8263A_BLINK_TIMER                 SLCD_FRAME_COUNTER_0

/** Circular animation configuration. */
#define CONF_C8263A_CIRCULAR_ANIMATION_TIMER    SLCD_FRAME_COUNTER_0


#define C8263A_NB_OF_COM 8
#define C8263A_NB_OF_SEG 24

#define C8263A_NUM_SEG_INDEX_S 14
#define C8263A_NUM_SEG_INDEX_E 16

#define C8263A_TXT_SEG_INDEX_S 20
#define C8263A_TXT_SEG_INDEX_E 24
      
#define C8263A_HOUR_SEG_S   16 
#define C8263A_MIN_SEG_S    18


#define C8263A_MAX_CHAR 9
#define C8263A_MAX_NUM  5


/* name Icon with selected segments blinking feature */
#define C8263A_ICON_USB            1, 0
#define C8263A_ICON_BAT            1,0
#define C8263A_ICON_WIRELESS       2,0

/* name Icon without selected segments blinking feature */
#define C8263A_ICON_MINUS          0, 37
#define C8263A_ICON_MINUS_SEG1     3, 30
#define C8263A_ICON_MINUS_SEG2     0, 33
#define C8263A_ICON_DOT_1          0, 29
#define C8263A_ICON_DOT_2          3, 10
#define C8263A_ICON_DOT_3          3, 8
#define C8263A_ICON_DOT_4          3, 6
#define C8263A_ICON_DOT_5          3, 6
#define C8263A_ICON_TIME_DOT       3, 6

#define C8263A_ICON_BAT_LEVEL_1    0, 21
#define C8263A_ICON_BAT_LEVEL_2    0, 17
#define C8263A_ICON_BAT_LEVEL_3    0, 25
#define C8263A_ICON_WIRELESS_LEVEL_1    0, 21
#define C8263A_ICON_WIRELESS_LEVEL_2    0, 17
#define C8263A_ICON_WIRELESS_LEVEL_3    0, 25
#define C8263A_ICON_AM             3, 4
#define C8263A_ICON_PM             3, 26
#define C8263A_ICON_DEGREE_C       3, 22
#define C8263A_ICON_DEGREE_F       3, 18
#define C8263A_ICON_VOLT           3, 14
#define C8263A_ICON_AMP           3, 14
#define C8263A_ICON_MILLI_VOLT     0, 13
#define C8263A_ICON_LEFT             3, 26
#define C8263A_ICON_RIGHT             3, 26
#define C8263A_ICON_UP             3, 26
#define C8263A_ICON_DOWN             3, 26
#define C8263A_ICON_BACK             3, 26
#define C8263A_ICON_ATMEL             3, 26

/* @} */

#endif /* CONF_C8263A_H_INCLUDED */
