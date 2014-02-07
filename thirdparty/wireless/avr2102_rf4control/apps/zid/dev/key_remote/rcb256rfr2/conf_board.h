/**
 * \file *********************************************************************
 *
 * \brief Example specific board configuration file
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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
 /*
 * Copyright (c) 2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define CONF_BOARD_ENABLE_USARTA1

/* Taking the battery voltage for TFA */
#define GET_BATTERY_VOLTAGE()    tfa_get_batmon_voltage()  


/* Enumerations used to identify ADC Channels */
typedef enum adc_channel_tag
{
    X_AXIS,
    Y_AXIS,
    Z_AXIS,
    ADC_REF
} SHORTENUM adc_channel_t;

/* Enumerations states for enabling or disabling the Accelerometer */
typedef enum acc_status_tag
{
    ACC_OFF,
    ACC_ON
} SHORTENUM acc_status_t;


/**
 * @brief Button action
 */
typedef enum button_state_tag
{
    /** Button is pressed. */
    BUTTON_PRESSED,
    /** Button is released. */
    BUTTON_OFF
} SHORTENUM button_state_t;


/* === Externals ============================================================*/


/* === Macros ===============================================================*/

/**
 * The default CPU clock
 *
 * The board must be configured for 16 MHz clock, which can be fused
 * for either the 16 MHz internal RC oscillator, or for the 16 MHz
 * crystal oscillator that is required for the transceiver operation.
 * If F_CPU is set to 4 or 8 MHz, the prescaler reduces the CPU clock.
 */
#ifndef F_CPU
#define F_CPU                           (16000000UL)
#endif

/**
 * This board does not support antenna diversity.
 */
#define ANTENNA_DIVERSITY               (0)

#define NO_OF_LEDS                      (5)

/* Definitions to identify buttons, reference HDMI specification */
#define BUTTON_UNKNOWN            0x00000000
#define BUTTON_F2_RED             0x00000001
#define BUTTON_F3_GREEN           0x00000002
#define BUTTON_F4_YELLOW          0x00000004
#define BUTTON_F1_BLUE            0x00000008
#define BUTTON_SELECT             0x00000010
#define BUTTON_NUMBER_0           0x00000020
#define BUTTON_POWER_OFF_FUNCTION 0x00000040
#define BUTTON_NUMBER_1           0x00000200
#define BUTTON_NUMBER_2           0x00000400
#define BUTTON_NUMBER_3           0x00000800
#define BUTTON_NUMBER_4           0x00001000
#define BUTTON_NUMBER_5           0x00002000
#define BUTTON_NUMBER_6           0x00004000
#define BUTTON_NUMBER_7           0x00008000
#define BUTTON_NUMBER_8           0x00010000
#define BUTTON_NUMBER_9           0x00020000
#define BUTTON_UP                 0x00040000
#define BUTTON_LEFT               0x00080000
#define BUTTON_ENTER              0x00100000
#define BUTTON_RIGHT              0x00200000
#define BUTTON_DOWN               0x00400000
#define BUTTON_LEFT_UP            0x00800000
#define BUTTON_LEFT_DOWN          0x01000000
#define BUTTON_RIGHT_UP           0x02000000
#define BUTTON_RIGHT_DOWN         0x04000000


#define button_id_t             uint32_t
#define NO_OF_BUTTONS           (24)

#define LED0    LED_4

#endif /* CONF_BOARD_H */
