/**
* \file
*
* \brief ALert Notification Service
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

/***********************************************************************************
 *									Includes		                               *
 **********************************************************************************/
#include "at_ble_api.h"
#include "platform.h"
#include "alert_notification.h"
#include "ble_manager.h"

const char *bitmask0[] = {"Simple Alert","Email","News","Call","Missed Call","Sms/Mms","Voice Mail","Schedule"};
const char *bitmask1[] = {"High Prioritized Alert","Instant Message","Reserved For feature"};

/***********************************************************************************
 *									Implementation	                               *
 **********************************************************************************/

at_ble_status_t anp_alert_write(at_ble_handle_t conn_handle,at_ble_handle_t desc_handle, bool noti)
{
	uint8_t desc_data[3] = {0x05, 0xFF, 0};
	
	if(desc_handle == ANP_INVALID_CHAR_HANDLE)
	{
		return (AT_BLE_ATT_INVALID_HANDLE);
	}
	else
	{
		if(noti == true)
		{
			return(at_ble_characteristic_write(conn_handle, desc_handle, 0, 2, &desc_data[0],false, true));
		}
		else if(noti == false)
		{
			return(at_ble_characteristic_write(conn_handle, desc_handle, 0, 2, &desc_data[1],false, true));
		}
	}
	return 0;
}

at_ble_status_t anp_alert_noti(at_ble_handle_t conn_handle,at_ble_handle_t desc_handle, bool noti)
{
	uint8_t desc_data[3] = {1, 0, 0};
	
	if(desc_handle == ANP_INVALID_CHAR_HANDLE)
	{
		return (AT_BLE_ATT_INVALID_HANDLE);
	}
	else
	{
		if(noti == true)
		{
			return(at_ble_characteristic_write(conn_handle, desc_handle, 0, 2, &desc_data[0],false, true));
		}
		else if(noti == false)
		{
			return(at_ble_characteristic_write(conn_handle, desc_handle, 0, 2, &desc_data[1],false, true));
		}
	}
	return 0;
}

at_ble_status_t anp_alert_read(at_ble_handle_t conn_handle,at_ble_handle_t char_handle)
{
	if (char_handle == ANP_INVALID_CHAR_HANDLE)
	{
		return (AT_BLE_ATT_INVALID_HANDLE);
	}
	else
	{
		return (at_ble_characteristic_read(conn_handle,char_handle,ANP_READ_OFFSET,ANP_READ_LENGTH));
	}
	return AT_BLE_ATT_INVALID_HANDLE;
}

int8_t anp_alert_read_response (at_ble_characteristic_read_response_t *read_resp, gatt_anp_handler_t *anp_handler)
{
	uint8_t idx;
	uint8_t unread_alerts;
	uint8_t new_alerts;
	/* Supported New Alert Category */
	if(read_resp->char_handle == anp_handler->supp_new_char_handle)
	{
		DBG_LOG_DEV("Support New Alert Characteristic read response %02x handler",read_resp->char_handle);
		DBG_LOG_DEV("The length is %d",read_resp->char_len);
		
		DBG_LOG("Peer device supports the following New Alert categories");
		
		if (read_resp->char_value[0] & 1)
		{
			DBG_LOG("%s",bitmask0[0]);		
		}
		
		if (read_resp->char_value[0] & (1 << 1))
		{
			DBG_LOG("%s",bitmask0[1]);
		}
		
		if (read_resp->char_value[0] & (1 << 2))
		{
			DBG_LOG("%s",bitmask0[2]);
		}
		
		if (read_resp->char_value[0] & (1 << 3))
		{
			DBG_LOG("%s",bitmask0[3]);
		}
		
		if (read_resp->char_value[0] & (1 << 4))
		{
			DBG_LOG("%s",bitmask0[4]);
		}
		
		if (read_resp->char_value[0] & (1 << 5))
		{
			DBG_LOG("%s",bitmask0[5]);
		}
		
		if (read_resp->char_value[0] & (1 << 6))
		{
			DBG_LOG("%s",bitmask0[6]);
		}
		
		if (read_resp->char_value[0] & (1 << 7))
		{
			DBG_LOG("%s",bitmask0[7]);
		}
		
		if (read_resp->char_value[1] & 1)
		{
			DBG_LOG("%s",bitmask1[0]);
		}
		
		if (read_resp->char_value[1] & (1 << 1))
		{
			DBG_LOG("%s",bitmask1[1]);
		}
		
		DBG_LOG_DEV("Category ID bit Mask 0 = %02x",read_resp->char_value[0]);
		DBG_LOG_DEV("Category ID bit Mask 1 = %02x",read_resp->char_value[1]);
	}
	/* New Alert Category */
	if(read_resp->char_handle == anp_handler->new_alert_char_handle)
	{
		DBG_LOG_DEV("New Alert Characteristic read response %02x handler",read_resp->char_handle);
		DBG_LOG_DEV("The length is %d",read_resp->char_len);
		
		if (read_resp->char_value[0] <= 7)
		{
			DBG_LOG("%s New Alert received",bitmask0[read_resp->char_value[0]]);
		} else if (read_resp->char_value[1] > 7 && read_resp->char_value[1] <= 9) {
			if (read_resp->char_value[1] == 8)
			{
				DBG_LOG("The New Alert is %s",bitmask1[read_resp->char_value[0]]);
			} 
			
			if (read_resp->char_value[1] == 9)
			{
				DBG_LOG("The New Alert is %s",bitmask1[read_resp->char_value[1]]);
			}
		}
		new_alerts = read_resp->char_value[1];
		DBG_LOG("The no of New alerts %d",new_alerts);
	
		for (idx = 0; idx < read_resp->char_len - 3; idx++)
		{
			DBG_LOG_CONT("%c",read_resp->char_value[3+idx]);
		}
		
		DBG_LOG_DEV("Category ID bit Mask 0 = %02x",read_resp->char_value[0]);
		DBG_LOG_DEV("Category ID bit Mask 1 = %02x",read_resp->char_value[1]);
	}
	/* Supported Unread Alert Category */
	if(read_resp->char_handle == anp_handler->supp_unread_char_handle)
	{
		DBG_LOG_DEV("Supported UnAlert Characteristic read response %02x handler",read_resp->char_handle);
		DBG_LOG_DEV("The length is %d",read_resp->char_len);
			DBG_LOG("Peer device supports the following Unread Alert categories");
			if (read_resp->char_value[0] & 1)
			{
				DBG_LOG("%s",bitmask0[0]);
			}
			
			if (read_resp->char_value[0] & (1 << 1))
			{
				DBG_LOG("%s",bitmask0[1]);
			}
			
			if (read_resp->char_value[0] & (1 << 2))
			{
				DBG_LOG("%s",bitmask0[2]);
			}
			
			if (read_resp->char_value[0] & (1 << 3))
			{
				DBG_LOG("%s",bitmask0[3]);
			}
			
			if (read_resp->char_value[0] & (1 << 4))
			{
				DBG_LOG("%s",bitmask0[4]);
			}
			
			if (read_resp->char_value[0] & (1 << 5))
			{
				DBG_LOG("%s",bitmask0[5]);
			}
			
			if (read_resp->char_value[0] & (1 << 6))
			{
				DBG_LOG("%s",bitmask0[6]);
			}
			
			if (read_resp->char_value[0] & (1 << 7))
			{
				DBG_LOG("%s",bitmask0[7]);
			}
			
			if (read_resp->char_value[1] & 1)
			{
				DBG_LOG("%s",bitmask1[0]);
			}
			
			if (read_resp->char_value[1] & (1 << 1))
			{
				DBG_LOG("%s",bitmask1[1]);
			}

		DBG_LOG_DEV("Category ID bit Mask 0 = %02x",read_resp->char_value[0]);
		DBG_LOG_DEV("Category ID bit Mask 1 = %02x",read_resp->char_value[1]);
	}
	/* Unread Alert Status*/
	if(read_resp->char_handle == anp_handler->unread_alert_char_handle)
	{
		DBG_LOG_DEV("Unread Alert Characteristic read response %02x handler",read_resp->char_handle);
		DBG_LOG_DEV("The length is %d",read_resp->char_len);
		if (read_resp->char_value[0] <= 7)
		{
			DBG_LOG("%s alert received",bitmask0[read_resp->char_value[0]]);
			} else if (read_resp->char_value[1] > 7 && read_resp->char_value[1] <= 9){
			if (read_resp->char_value[1] == 8)
			{
				DBG_LOG("The New Alert is %s",bitmask1[read_resp->char_value[0]]);
			}
			
			if (read_resp->char_value[1] == 9)
			{
				DBG_LOG("The New Alert is %s",bitmask1[read_resp->char_value[1]]);
			}
		}
		unread_alerts = read_resp->char_value[2];
		DBG_LOG("The no of new alerts %d",unread_alerts);
		
		DBG_LOG_DEV("Category ID bit Mask 0 = %02x",read_resp->char_value[0]);
		DBG_LOG_DEV("Category ID bit Mask 1 = %02x",read_resp->char_value[1]);
	}
	/* Alert Notification Control Point */
	if(read_resp->char_handle == anp_handler->alert_np_char_handle)
	{
		DBG_LOG("Alert Notification Characteristic read response %02x handler",read_resp->char_handle);
		DBG_LOG("Category ID bit Mask 0 = %02x",read_resp->char_value[0]);
		DBG_LOG("Category ID bit Mask 1 = %02x",read_resp->char_value[1]);
	}
	return 0;
}




