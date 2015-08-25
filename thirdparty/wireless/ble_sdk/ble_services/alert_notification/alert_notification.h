/**
* \file
*
* \brief ANCS Service Declarations
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
*    Atmel micro controller product.
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
#ifndef __ALERT_NOTIFICATION_H__
#define __ALERT_NOTIFICATION_H__


#define ANP_INVALID_CHAR_HANDLE				(0)

#define ANP_READ_LENGTH                     (20)

/* Link Loss read offset length*/
#define ANP_READ_OFFSET                     (0)

#define ANS_SUP_NEW_READ_LENGTH				(20)
#define ANS_NEW_ALERT_READ_LENGTH			(20)
#define ANS_SUP_UNREA_READ_LENGTH			(20)
#define ANS_UNREA_ALERT_READ_LENGTH			(20)
#define ANS_ALERT_NP_READ_LENGTH			(20)


typedef struct gatt_anp_handler
{
	at_ble_handle_t start_handle;
	at_ble_handle_t end_handle;
	at_ble_status_t char_discovery;
	at_ble_status_t desc_discovery;
	/*Characteristics for supported new alert category*/
	at_ble_handle_t supp_new_char_handle;
	uint8_t *supp_new_char_data;
	/*Characteristics for new alert */
	at_ble_handle_t new_alert_char_handle;
	uint8_t *new_alert_char_data;
	at_ble_handle_t new_alert_desc_handle;
	/*Characteristics for supported unread alert category*/
	at_ble_handle_t supp_unread_char_handle;
	uint8_t *supp_unread_char_data;
	/*Characteristics for unread alert status */
	at_ble_handle_t unread_alert_char_handle;
	uint8_t *unread_alert_char_data;
	at_ble_handle_t unread_alert_desc_handle;
	/*Characteristics for alert notification control point */
	at_ble_handle_t alert_np_char_handle;
	uint8_t *alert_np_data;
	
}gatt_anp_handler_t;


/**
 * @brief Enabling ANCS service 
 * @param[in] Instance of ancs data
 * \note Called by the profile
 */
at_ble_status_t anp_alert_noti(at_ble_handle_t conn_handle,at_ble_handle_t desc_handle, bool noti);

int8_t anp_alert_read_response (at_ble_characteristic_read_response_t *read_resp, gatt_anp_handler_t *anp_handler);

at_ble_status_t anp_alert_read(at_ble_handle_t conn_handle, at_ble_handle_t char_handle);

at_ble_status_t anp_alert_write(at_ble_handle_t conn_handle,at_ble_handle_t desc_handle, bool noti);

#endif /* __ALERT_NOTIFICATION_H__ */