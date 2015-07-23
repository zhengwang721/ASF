/**
* \file
*
* \brief Current Time Service
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

/**
* \mainpage
* \section preface Preface
* This is the reference manual for the Current Time Service
*/
/***********************************************************************************
 *									Includes		                               *
 **********************************************************************************/

#include <string.h>
#include "at_ble_api.h"
#include "current_time.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "current_time.h"

/***********************************************************************************
 *									Implementations	                               *
 **********************************************************************************/
/**@brief Send the Read request to the current time characteristic
 * Read value will be reported via @ref AT_BLE_CHARACTERISTIC_READ_RESPONSE
 *event
 */

at_ble_status_t tis_current_time_read(at_ble_handle_t conn_handle,
		at_ble_handle_t char_handle)
{
	if (char_handle == CTS_INVALID_CHAR_HANDLE) 
	{
		return (AT_BLE_INVALID_HANDLE);
	} 
	else 
	{
		return (at_ble_characteristic_read(conn_handle,char_handle,CTS_READ_OFFSET,CTS_READ_LENGTH));
	}
}

/**@brief Read response handler for read response for time characteristic
 */
int8_t tis_current_time_read_response(at_ble_characteristic_read_response_t *read_resp,
		gatt_cts_handler_t *cts_handler)
{
	if (read_resp->char_handle == cts_handler->char_handle) 
	{

		const char *ptr[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
		memcpy(cts_handler->char_data,&read_resp->char_value[CTS_READ_OFFSET],CTS_READ_LENGTH);
		
		DBG_LOG("Current Time:");		
		
		DBG_LOG_CONT("[DD:MM:YYYY]: %02d-%02d-%02d [HH:MM:SS]: %02d:%02d:%02d  Day:%s",
		read_resp->char_value[3],
		read_resp->char_value[2],
		((uint16_t)read_resp->char_value[0] | (read_resp->char_value[1] <<8)),
		read_resp->char_value[4],
		read_resp->char_value[5],
		read_resp->char_value[6],
		ptr[read_resp->char_value[7]]
		);
				
	}
	return 0;
}


