/**
* \file
*
* \brief Link Loss Service declarations
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

#ifndef __LINK_LOSS_H__
#define __LINK_LOSS_H__

#include "ble_manager.h"

#define MAX_LLS_CHAR_SIZE			(1)       

/* Link Loss write offset value */
#define LLS_WRITE_OFFSET			(0)

/* Link loss Data write length */
#define LLS_WRITE_LENGTH			(1)

/* Link Loss write with response */
#define LLS_WRITE_WITH_RESPONSE		(true)

/* Link Loss no signed write */
#define LLS_NO_SIGNED_WRITE			(false)

/* Link Loss Alert length of data to read */
#define LLS_READ_LENGTH				(1)

/* Link Loss read offset length*/
#define LLS_READ_OFFSET				(0)

/* Link Loss invalid character handler*/
#define LLS_INVALID_CHAR_HANDLE		(0)

/* Link Loss invalid read response value */
#define LLS_READ_RESP_INVALID		(0xFF)

#define LLS_ALERT_LEVEL				LLS_HIGH_ALERT


typedef enum
{
	LLS_NO_ALERT ,
	LLS_LOW_ALERT,
	LLS_HIGH_ALERT
}link_loss_alert_level_t;

/**@brief write a characteristic  to Link Loss Characteristics
  *
  * if with_response is True, write completion will be reported via @ref AT_BLE_CHARACTERISTIC_WRITE_RESPONSE event
  *
  * @param[in] conn_handle handle of the connection 
  * @param[in] char_handle handle of the characteristic
  * @param[in] alert level need to write
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t lls_alert_level_write(at_ble_handle_t conn_handle, at_ble_handle_t char_handle, link_loss_alert_level_t alert_level);


/**@brief Send the Read request to link loss handler
  * Read value will be reported via @ref AT_BLE_CHARACTERISTIC_READ_RESPONSE event
  *
  * @param[in] conn_handle handle of the connection
  * @param[in] char_handle handle of the characteristic
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_INVALID_PARAM Invalid arguments.
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t lls_alert_level_read(at_ble_handle_t conn_handle, at_ble_handle_t char_handle);


/**@brief Read Link Loss Alert Value
*
* @param[in] read_value read response data available form at_ble_characteristic_read_response_t
* @return Link Loss Alert level .
* @return LLS_READ_RESP_INVALID if value are other than alert levels
*/
int8_t lls_alert_read_response(at_ble_characteristic_read_response_t *read_resp, gatt_char_handler_t *lls_handler);


#endif /*__LINK_LOSS_H__*/