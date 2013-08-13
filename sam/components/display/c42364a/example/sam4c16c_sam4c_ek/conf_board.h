/**
 * \file
 *
 * \brief Board configuration.
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

/** Enable Com Port. */
#define CONF_BOARD_UART_CONSOLE

/** Enable LCD backlight */
#define CONF_BOARD_BL

/** SEG and COM Num of ICON */
#define C42364A_ICON_USB            1,48
#define C42364A_ICON_COLON          0,48
#define C42364A_ICON_WLESS          2,48
#define C42364A_ICON_AUDIO          3,48
#define C42364A_ICON_BAT            0,49
#define C42364A_ICON_ERROR          1,49
#define C42364A_ICON_CHINESE        2,49
#define C42364A_ICON_ARM            3,49
#define C42364A_ICON_MINUS          0,21
#define C42364A_ICON_MINUS_SEG1     3,29
#define C42364A_ICON_MINUS_SEG2     0,26
#define C42364A_ICON_DOT_1          0,30
#define C42364A_ICON_DOT_2          3,20
#define C42364A_ICON_DOT_3          3,22
#define C42364A_ICON_DOT_4          3,25
#define C42364A_ICON_BAT_LEVEL_1    0,34
#define C42364A_ICON_BAT_LEVEL_2    0,45
#define C42364A_ICON_BAT_LEVEL_3    0,38
#define C42364A_ICON_AUDIO_PAUSE    3,21
#define C42364A_ICON_AUDIO_PLAY     3,25
#define C42364A_ICON_AM             3,27
#define C42364A_ICON_PM             3,33
#define C42364A_ICON_DEGREE_C       3,37
#define C42364A_ICON_DEGREE_F       3,41
#define C42364A_ICON_VOLT           3,45
#define C42364A_ICON_MILLI_VOLT     0,46

#endif /* CONF_BOARD_H_INCLUDED */
