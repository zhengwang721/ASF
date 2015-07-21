/**
* \file
*
* \brief Time Information Profile
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
* This is the reference manual for the Time Information Profile
*/
/***********************************************************************************
 *									Includes		                               *
 **********************************************************************************/
#include <string.h>
#include "at_ble_api.h"
#include "current_time.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "time_info.h"


#if defined CURRENT_TIME_SERVICE
#include "current_time.h"
#endif


/***********************************************************************************
 *									Globals			                               *
 **********************************************************************************/

#if defined CURRENT_TIME_SERVICE
/**@brief CTS Characteristic Value array*/
uint8_t cts_char_data[CTS_READ_LENGTH];
/**@brief CTS Service Handle*/
gatt_cts_handler_t cts_handle = {0, 0, 0, AT_BLE_INVALID_PARAM, NULL};
#endif


/**@breif Scan Response packet*/
static uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xFF, 0x00, 0x06, 0x25, 0x75, 0x11, 0x6a, 0x7f, 0x7f};
/**@breif Peer Connected device info*/
extern at_ble_connected_t ble_connected_dev_info[MAX_DEVICE_CONNECTED];


/***********************************************************************************
 *									Implementations	                               *
 **********************************************************************************/
/**
 * @brief time info advertisement data and start of advertisement data
 */
void time_info_adv()
{
	uint8_t idx = 0;
	uint8_t adv_data[TP_ADV_DATA_NAME_LEN + TP_ADV_DATA_APPEARANCE_LEN + TP_ADV_DATA_UUID_LEN + 3*2];
	
	// Prepare ADV Data
	adv_data[idx++] = TP_ADV_DATA_APPEARANCE_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = TP_ADV_DATA_APPEARANCE_TYPE;
	memcpy(&adv_data[idx], TP_ADV_DATA_APPEARANCE_DATA, TP_ADV_DATA_APPEARANCE_LEN);
	idx += TP_ADV_DATA_APPEARANCE_LEN;
	
	adv_data[idx++] = TP_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = TP_ADV_DATA_NAME_TYPE;
	memcpy(&adv_data[idx], TP_ADV_DATA_NAME_DATA, TP_ADV_DATA_NAME_LEN);
	idx += TP_ADV_DATA_NAME_LEN;
	
	adv_data[idx++] = TP_ADV_DATA_UUID_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = TP_ADV_DATA_SERVSOLICITATION_16UUID_TYPE;
	memcpy(&adv_data[idx], TP_ADV_DATA_UUID_CTS_DATA, TP_ADV_DATA_UUID_LEN);
	idx += TP_ADV_DATA_16BIT_UUID_LEN;
	
	at_ble_adv_data_set(adv_data, idx, scan_rsp_data, SCAN_RESP_LEN);
	
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY,
	APP_TP_FAST_ADV, APP_TP_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BLE Adv start Failed");
	}
	else
	{
		DBG_LOG("Device is in Advertising Mode");
	}
}

/**
 * @brief Time Information profile initilalizating and setting of device in advertising mode
 */
void time_info_init(void *param)
{
	#if defined CURRENT_TIME_SERVICE
	cts_handle.char_data = cts_char_data;
	#endif
	time_info_adv();
}

/**
 * @brief Discovering the services of time serverr
 */
at_ble_status_t time_info_service_discover(at_ble_connected_t *conn_params)
{	
	if (conn_params->conn_status != AT_BLE_SUCCESS)
	{
		return conn_params->conn_status;
	}
	if(at_ble_primary_service_discover_all(conn_params->handle, GATT_DISCOVERY_STARTING_HANDLE, GATT_DISCOVERY_ENDING_HANDLE) == AT_BLE_SUCCESS)
	{
		DBG_LOG_DEV("GATT Discovery request started ");
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("GATT Discovery request failed");
	}	
	return AT_BLE_FAILURE;
}

/**
 * @brief Handler for connection event 
 */
at_ble_status_t time_info_connected_state_handler(at_ble_connected_t *conn_params)
{
	at_ble_status_t discovery_status = AT_BLE_FAILURE;
	at_ble_status_t status;
	if(conn_params->conn_status==AT_BLE_SUCCESS)
	{
		{
			DBG_LOG("BLE: Security Procedure");
			/*Start the security procedure*/
			status = ble_send_slave_sec_request(conn_params->handle);
			
			if(status != AT_BLE_SUCCESS)
			{
				DBG_LOG("BLE: Fail to start security procedure. status = %d", status);
			}
		}
	}
	return discovery_status;
}

/**
 * @brief Service found handler invoked by ble manager
 */
void time_info_service_found_handler(at_ble_primary_service_found_t * primary_service_params)
{
	at_ble_uuid_t *ctx_service_uuid;
	ctx_service_uuid = &primary_service_params->service_uuid;
	if (ctx_service_uuid->type == AT_BLE_UUID_16)
	{
		uint16_t service_uuid;
		service_uuid = ((ctx_service_uuid->uuid[1] << 8) | ctx_service_uuid->uuid[0]);
		switch(service_uuid)
		{
			/* for cts service*/
			case CURRENT_TIME_SERVICE_UUID:
			{
				cts_handle.start_handle = primary_service_params->start_handle;
				cts_handle.end_handle = primary_service_params->end_handle;
				DBG_LOG_DEV("current time service discovered  %04X %04X", cts_handle.start_handle, cts_handle.end_handle);
				cts_handle.char_discovery = AT_BLE_SUCCESS;
			}
			break;
			
			default:
			break;
		}
	}
}

/**
 * @brief Discovery Complete handler invoked by ble manager
 */
void time_info_discovery_complete_handler(at_ble_discovery_complete_t *discover_status)
{
	bool discover_char_flag = true;
	if (discover_status->status == AT_BLE_DISCOVERY_SUCCESS)
	{
		//#if defined TX_POWER_SERVICE
		if ((cts_handle.char_discovery == AT_BLE_SUCCESS) && (discover_char_flag))
		{
			if (at_ble_characteristic_discover_all(
			ble_connected_dev_info[0].handle,
			cts_handle.start_handle,
			cts_handle.end_handle) == AT_BLE_SUCCESS)
			{
				DBG_LOG_DEV("CTS Characteristic Discovery Started");
			}
			else
			{
				DBG_LOG_DEV("CTS Characteristic Discovery Failed");
			}
			cts_handle.char_discovery = AT_BLE_FAILURE;
			discover_char_flag = false;
		}
		else if (cts_handle.char_discovery == AT_BLE_INVALID_PARAM)
		{
			DBG_LOG("Current time Service not Found");
			cts_handle.char_discovery = AT_BLE_INSUFF_RESOURCE;
			discover_char_flag = false;
		}
		
		if((cts_handle.char_discovery == AT_BLE_INSUFF_RESOURCE) && (discover_char_flag))
		{
			DBG_LOG("TIME INFOMATION PROFILE NOT SUPPORTED");
			discover_char_flag = false;
			at_ble_disconnect(ble_connected_dev_info[0].handle, AT_BLE_TERMINATED_BY_USER);
		}
		
		if (discover_char_flag)
		{
			DBG_LOG_DEV("GATT characteristic discovery completed");			
			if(ble_send_slave_sec_request(ble_connected_dev_info[0].handle) == AT_BLE_SUCCESS)
			{
				DBG_LOG_DEV("Successfully send Slave Security Request");
			}
			else
			{
				DBG_LOG("Fail to send Slave Security Request");
			}
		}
	}
}

/**
 * @brief time read response handler invoked by ble manager
 */
void time_info_characteristic_read_response(at_ble_characteristic_read_response_t *char_read_resp)
{
	
	#if defined CURRENT_TIME_CHAR_UUID
		tis_current_time_read_response(char_read_resp, &cts_handle);
	#endif
}

/**
 * @brief characteristic found handler invoked by ble manager
 */
void time_info_characteristic_found_handler(at_ble_characteristic_found_t *characteristic_found)
{
	uint16_t charac_16_uuid ;
	
	charac_16_uuid = (uint16_t)((characteristic_found->char_uuid.uuid[0]) | \
	(characteristic_found->char_uuid.uuid[1] << 8));
	
	if(charac_16_uuid == CURRENT_TIME_CHAR_UUID)
	{
		cts_handle.char_handle = characteristic_found->value_handle;
		DBG_LOG_DEV("current time characteristics %04X",cts_handle.char_handle);
		DBG_LOG_DEV("Characteristic Info ConnHandle 0x%02x : Char handle 0x%02x : Value handle : 0x%02x : Properties : 0x%02x",
		characteristic_found->conn_handle,
		characteristic_found->char_handle,
		characteristic_found->value_handle,
		characteristic_found->properties);
	}

}

/**
 * @brief disconnected event handler invoked by ble manager
 */
void time_info_disconnected_event_handler(at_ble_disconnected_t *disconnect)
{
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY,
	APP_TP_FAST_ADV, APP_TP_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BLE Adv start Failed");
	}
	else
	{
		DBG_LOG("BLE Started Adv");
	}	
}
/**
 * @brief char changed handler invoked by ble manager
 */
void time_info_char_changed_handler(at_ble_characteristic_changed_t *characteristic_changed)
{
	uint32_t i = 0;
	DBG_LOG("Characteristic 0x%x changed, new_value = ",
	characteristic_changed->char_handle);
	for(i=0; i<characteristic_changed->char_len; i++)
	DBG_LOG("0x%02x ", characteristic_changed->char_new_value[i]);
	DBG_LOG("\n");
}