/**
 * \file
 *
 * \brief HID Mouse Device Profile Application declarations
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 * Support</a>
 */

#ifndef __HID_DEVICE_APP_H__
#define __HID_DEVICE_APP_H__
#include "stdio.h"

/****************************************************************************************
*							        Macros	                                     							*
****************************************************************************************/
/** @brief Mouse movement pixel position */
#define MOUSE_MOVEMENT_POS 10

#define KEY_PAD_DEBOUNCE_TIME	(200)

#define MOUSE_CHANGE_DIRECTION	(5)

/****************************************************************************************
*							        Enumerations	                                   	*
****************************************************************************************/
/**@brief Mouse movement
*/
typedef enum
{
	/// Enable boot protocol mode
	MOUSE_RIGHT_MOVEMENT    = 1,
	MOUSE_LEFT_MOVEMENT,
	MOUSE_UP_MOVEMENT,
	MOUSE_DOWN_MOVEMENT,	
}mouse_move;

/****************************************************************************************
*							        Function                                     		*
****************************************************************************************/
/** @brief Initialize the hid profile based on user input */
void hid_mouse_app_init(void);

/** @brief Timer callback handler */
void timer_callback_handler(void);

/** @brief Control point notification callback */
void hid_prf_control_point_ntf_cb(hid_control_mode_ntf_t *hid_control_point_value);

/** @brief Protocol mode notification callback */
void hid_prf_protocol_mode_ntf_cb(hid_proto_mode_ntf_t *protocol_mode);

/** @brief Boot notification callback */
void hid_prf_boot_ntf_cb(hid_boot_ntf_t *boot_ntf_info);

/** @brief Report notification callback */
void hid_prf_report_ntf_cb(hid_report_ntf_t *report_info);

/** @brief Function used for mouse movement */
bool hid_mouse_move(int8_t pos, uint8_t index_report);

/** @brief Notification confirmed callback */
void hid_notification_confirmed_cb(at_ble_cmd_complete_event_t *notification_status);

#endif /*__HID_DEVICE_APP_H__*/
