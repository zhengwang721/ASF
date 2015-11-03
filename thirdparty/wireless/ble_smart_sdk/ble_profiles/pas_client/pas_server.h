/**
* \file
*
* \brief Phone Alert Status Profile Server Declarations
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
*	  this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
*	  this list of conditions and the following disclaimer in the documentation
*	  and/or other materials provided with the distribution.
*
* 3. The name of Atmel may not be used to endorse or promote products derived
*	  from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an
*	  Atmel microcontroller product.
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

#ifndef _PASS_PROFILE_H_INCLUDE_
#define _PASS_PROFILE_H_INCLUDE_

#include "at_ble_api.h"
#include "pas_service.h"

/****************************************************************************************
*							        Macros	                                        *
****************************************************************************************/

typedef enum
{
	PAS_ALERT_STATUS = 0,
	PAS_RINGER_STATUS,
	PAS_RINGER_CONTROL_POINT,
	PAS_TOTAL_CHARACTERISTIC_NUM,
}pas_char_type_t;

typedef struct
{
	int16_t serv_handle_info;
	uint8_t alert_status_value;
	uint8_t ringer_status_value;
	uint8_t ringer_control_point_value;
}pas_prf_info_t;

typedef enum 
{
	PAS_PHONE_CONTROL_SILENT_MODE = 1,			
	PAS_PHONE_CONTROL_MUTE_ONCE_MODE = 2,
	PAS_PHONE_CONTROL_CANCEL_SILENT_MODE = 3,
}pas_ring_control_point_t;

typedef enum
{
	PAS_RINGER_STATE,
	PAS_VIBRATOR_STATE,
	PAS_DISPLAY_ALERT_STATUS
}pas_alert_status;

typedef void (*control_point_ntf_callback_t)(pas_ring_control_point_t * );


uint8_t pas_server_conf(pas_prf_info_t * ref);

void 	pas_server_var_init(void);

void 	pas_server_init(void *param);

at_ble_status_t pas_server_connect_request(at_ble_scan_info_t *scan_buffer);

at_ble_status_t pas_server_scan_data_handler(at_ble_scan_info_t * scan_buffer, uint8_t scanned_dev_count);
	
at_ble_status_t pas_server_disconnect_event_handler(at_ble_disconnected_t * disconnect);

at_ble_status_t pas_server_char_changed_handler(at_ble_characteristic_changed_t *char_handle);

void 	notify_control_point_handler(control_point_ntf_callback_t con_ntf_cb_fn);

#endif

