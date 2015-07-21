/**
* \file
*
* \brief Alert Notification Profile
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
/*- Includes ---------------------------------------------------------------*/

#include <string.h>
#include "at_ble_api.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "anp_client.h"
#include "ancs.h"


/*Profile Information*/
app_anp_data_t app_anp_info;


/*ANCS profile data*/
ancs_prf_t ancs_data;

static uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xFF, 0x00, 0x06, 0x28, 0x75, 0x11, 0x6a, 0x7f, 0x7f};

void anp_info_init(void)
{
	memset((uint8_t *)&app_anp_info, 0, sizeof(app_anp_data_t));
	app_anp_info.devicedb = FALSE;
	app_anp_info.discover_role = DISCOVER_SERVICE;
	app_anp_info.dispinfo = FALSE;
}

void anp_client_adv(void)
{
	uint8_t idx = 0;
	uint8_t adv_data[ANP_ADV_DATA_NAME_LEN + ANP_SOLICITATION_LEN + 2*2];
	
	// Prepare ADV Data
	adv_data[idx++] = ANP_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = ANP_ADV_DATA_NAME_TYPE;
	memcpy(&adv_data[idx], ANP_ADV_DATA_NAME_DATA, ANP_ADV_DATA_NAME_LEN);
	idx += ANP_ADV_DATA_NAME_LEN;
	
	adv_data[idx++] = ANP_SOLICITATION_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = ANP_ADV_DATA_SERVSOLICITATION_128UUID_TYPE;
	memcpy(&adv_data[idx], ANP_ANCS_SERVICE_UUID, ANP_SOLICITATION_LEN);
	idx += ANP_SOLICITATION_LEN;
	
	if (at_ble_adv_data_set(adv_data, idx, scan_rsp_data, SCAN_RESP_LEN) != AT_BLE_SUCCESS) 
	{
		DBG_LOG("adv set data not successful");
	}
	else {
		DBG_LOG("adv set data successful");
	}
	
	#ifdef OLED_MSG
	gfx_mono_draw_string("                          ", 0, XPOS_UPDATE_POS_Y, &sysfont);
	gfx_mono_draw_string("Advertising Mode", 0, XPOS_UPDATE_POS_Y, &sysfont);
	#endif
	
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY,
	APP_ANP_FAST_ADV, APP_ANP_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BLE: Failed to start advertisement");
	} else {
		DBG_LOG("Started advertisement");
	}
}



void anp_client_connected_state_handler(at_ble_connected_t *params)
{
	at_ble_status_t status;
	#ifdef OLED_MSG
	gfx_mono_draw_string("                          ", 0, XPOS_UPDATE_POS_Y, &sysfont);
	gfx_mono_draw_string("Connected Mode", 0, XPOS_UPDATE_POS_Y, &sysfont);
	#endif
	
	memcpy((uint8_t *)&app_anp_info.conn_params, params, sizeof(at_ble_connected_t));
		
			
	DBG_LOG("BLE: Connected to peer device Address: 0x%02x%02x%02x%02x%02x%02x Connection handle: 0x%02x",
	app_anp_info.conn_params.peer_addr.addr[5],
	app_anp_info.conn_params.peer_addr.addr[4],
	app_anp_info.conn_params.peer_addr.addr[3],
	app_anp_info.conn_params.peer_addr.addr[2],
	app_anp_info.conn_params.peer_addr.addr[1],
	app_anp_info.conn_params.peer_addr.addr[0],
	app_anp_info.conn_params.handle);
	
	
	DBG_LOG("BLE: Started Security Procedure");
	
	ancs_enable(&ancs_data, app_anp_info.conn_params.handle);
	
	if(!app_anp_info.devicedb)
	{
		
		
			DBG_LOG("BLE: Database discovery");
		

		app_anp_info.discover_role = DISCOVER_SERVICE;
		
		
				/* Discover Remote Service by service UUID */
		status = at_ble_primary_service_discover_by_uuid(app_anp_info.conn_params.handle,START_HANDLE, END_HANDLE, &ancs_data.ancs_serv.service_uuid);
		
		if(status != AT_BLE_SUCCESS)
		{
			DBG_LOG("BLE: Failed to start service discovery. status = %d", status);
		}
		
	}

}


void anp_client_discovery_complete_handler(at_ble_discovery_complete_t *params)
{
		at_ble_discovery_complete_t discover_status;
		memcpy((uint8_t *)&discover_status, params, sizeof(at_ble_discovery_complete_t));

		
		DBG_LOG("BLE: Discovery Complete  DiscoveryStatus 0x%02x DiscoveryRole 0x%02x",
		discover_status.status,
		app_anp_info.discover_role);
		
		
		if(discover_status.status == AT_BLE_DISCOVERY_SUCCESS)
		{
			if(app_anp_info.discover_role == DISCOVER_SERVICE)
			{
				
				DBG_LOG("BLE: Discover Characteristic");
				
				app_anp_info.discover_role = DISCOVER_CHARACTERISTIC;
				
				if(at_ble_characteristic_discover_all(app_anp_info.conn_params.handle, ancs_data.ancs_serv.start_handle, ancs_data.ancs_serv.end_handle) != AT_BLE_SUCCESS)
				{
					DBG_LOG("BLE: Fail to start discover characteristic");
				}
			}
			else if(app_anp_info.discover_role == DISCOVER_CHARACTERISTIC)
			{
				app_anp_info.discover_role = DISCOVER_DESCRIPTOR;
				
				DBG_LOG("BLE: Discover Descriptor");
				
				if(at_ble_descriptor_discover_all(ancs_data.notification_source_char.conn_handle,(ancs_data.notification_source_char.value_handle+1), (ancs_data.data_source_char.char_handle-1)) != AT_BLE_SUCCESS)
				{
					DBG_LOG("BLE: Descriptor Discovery Failed");
				}
			}
			else if(app_anp_info.discover_role == DISCOVER_DESCRIPTOR)
			{
				app_anp_info.discover_role = DISCOVER_IDLE;
				
				DBG_LOG("BLE: Database Discovery Completed");
				
				app_anp_info.devicedb = TRUE;
				
				if(at_ble_send_slave_sec_request(app_anp_info.conn_params.handle,TRUE,TRUE) != AT_BLE_SUCCESS)
				{
					DBG_LOG("BLE: Fail to start security procedure");
				}
				
				
			}
		}
}

void anp_client_service_found_handler(at_ble_primary_service_found_t * params)
{
	memcpy((uint8_t *)&ancs_data.ancs_serv, params, sizeof(at_ble_primary_service_found_t));
	
	
	DBG_LOG("BLE: Discover service Info:  ConnHandle 0x%02x : Start handle 0x%02x : End handle : 0x%02x",
	ancs_data.ancs_serv.conn_handle,
	ancs_data.ancs_serv.start_handle,
	ancs_data.ancs_serv.end_handle);
	
	DBG_LOG("BLE: Service 128bit UUID Value 0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X",
	ancs_data.ancs_serv.service_uuid.uuid[15],
	ancs_data.ancs_serv.service_uuid.uuid[14],
	ancs_data.ancs_serv.service_uuid.uuid[13],
	ancs_data.ancs_serv.service_uuid.uuid[12],
	ancs_data.ancs_serv.service_uuid.uuid[11],
	ancs_data.ancs_serv.service_uuid.uuid[10],
	ancs_data.ancs_serv.service_uuid.uuid[9],
	ancs_data.ancs_serv.service_uuid.uuid[8],
	ancs_data.ancs_serv.service_uuid.uuid[7],
	ancs_data.ancs_serv.service_uuid.uuid[6],
	ancs_data.ancs_serv.service_uuid.uuid[5],
	ancs_data.ancs_serv.service_uuid.uuid[4],
	ancs_data.ancs_serv.service_uuid.uuid[3],
	ancs_data.ancs_serv.service_uuid.uuid[2],
	ancs_data.ancs_serv.service_uuid.uuid[1],
	ancs_data.ancs_serv.service_uuid.uuid[0]);
	
}


void anp_client_characteristic_found_handler(at_ble_characteristic_found_t *params)
{
	memcpy((uint8_t *)&app_anp_info.char_info, params, sizeof(at_ble_characteristic_found_t));
	
	if(!(memcmp(app_anp_info.char_info.char_uuid.uuid, ANCS_CHAR_NOTIFICATION_SOURCE_UUID, 16)))
	{
		memcpy((uint8_t *)&ancs_data.notification_source_char, &app_anp_info.char_info, sizeof(at_ble_characteristic_found_t));
	}
	else if(!(memcmp(app_anp_info.char_info.char_uuid.uuid, ANCS_CHAR_CONTROL_POINT, 16)))
	{
		memcpy((uint8_t *)&ancs_data.control_point_char, &app_anp_info.char_info, sizeof(at_ble_characteristic_found_t));
	}
	else if(!(memcmp(app_anp_info.char_info.char_uuid.uuid, ANCS_CHAR_DATA_SOURCE, 16)))
	{
		memcpy((uint8_t *)&ancs_data.data_source_char, &app_anp_info.char_info, sizeof(at_ble_characteristic_found_t));
	}
	
		
	DBG_LOG("BLE: Characteristic Info  ConnHandle 0x%02x : Char handle 0x%02x : Value handle : 0x%02x : Properties : 0x%02x",
	app_anp_info.char_info.conn_handle,
	app_anp_info.char_info.char_handle,
	app_anp_info.char_info.value_handle,
	app_anp_info.char_info.properties);
	
	printf("BLE: Characteristic 128bit UUID Value 0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X",
	app_anp_info.char_info.char_uuid.uuid[15],
	app_anp_info.char_info.char_uuid.uuid[14],
	app_anp_info.char_info.char_uuid.uuid[13],
	app_anp_info.char_info.char_uuid.uuid[12],
	app_anp_info.char_info.char_uuid.uuid[11],
	app_anp_info.char_info.char_uuid.uuid[10],
	app_anp_info.char_info.char_uuid.uuid[9],
	app_anp_info.char_info.char_uuid.uuid[8],
	app_anp_info.char_info.char_uuid.uuid[7],
	app_anp_info.char_info.char_uuid.uuid[6],
	app_anp_info.char_info.char_uuid.uuid[5],
	app_anp_info.char_info.char_uuid.uuid[4],
	app_anp_info.char_info.char_uuid.uuid[3],
	app_anp_info.char_info.char_uuid.uuid[2],
	app_anp_info.char_info.char_uuid.uuid[1],
	app_anp_info.char_info.char_uuid.uuid[0]);
	
}

void anp_client_disconnected_event_handler(at_ble_disconnected_t *params)
{
			at_ble_disconnected_t disconnect;
			memcpy((uint8_t *)&disconnect, params, sizeof(at_ble_disconnected_t));
			app_anp_info.devicedb = FALSE;
			
	
			DBG_LOG("BLE: Device disconnected Reason:0x%02x Handle=0x%x", disconnect.reason, disconnect.handle);
	
			
			#ifdef OLED_MSG
			gfx_mono_draw_string("                   ", 0, SCROLL_UPDATE_POS_Y, &sysfont);
			gfx_mono_draw_string("                      ", 0, (XPOS_UPDATE_POS_Y+1), &sysfont);
			gfx_mono_draw_string("                   ", 0, (XPOS_UPDATE_POS_Y+XPOS_UPDATE_POS_Y+2), &sysfont);
			gfx_mono_draw_string("                          ", 0, XPOS_UPDATE_POS_Y, &sysfont);
			gfx_mono_draw_string("Advertising Mode", 0, XPOS_UPDATE_POS_Y, &sysfont);
			#endif
			
			if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY,
			APP_ANP_FAST_ADV, APP_ANP_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
			{
				DBG_LOG("BLE Adv start Failed");
			}
			else
			{
				DBG_LOG("BLE Started Adv");
			}
			
}

void anp_client_descriptor_found_handler(at_ble_descriptor_found_t *params)
{
		memcpy((uint8_t *)&ancs_data.notification_source_desc, params, sizeof(at_ble_descriptor_found_t));
					
	
		DBG_LOG("BLE: Descriptor Info ConnHandle 0x%02x : Descriptor handle : 0x%02x",
					ancs_data.notification_source_desc.conn_handle,
					ancs_data.notification_source_desc.desc_handle
					);
					
		DBG_LOG("BLE: Descriptor 16bit UUID Value 0x%02x%02x",
					ancs_data.notification_source_desc.desc_uuid.uuid[1],
					ancs_data.notification_source_desc.desc_uuid.uuid[0]);
}

void anp_client_char_changed_handler(at_ble_characteristic_changed_t *params)
{
	at_ble_characteristic_changed_t change_params;
	uint32_t i = 0;
	
	memcpy((uint8_t *)&change_params, params, sizeof(at_ble_characteristic_changed_t));

	DBG_LOG("Characteristic 0x%x changed, new_value = ",
	change_params.char_handle);
	for(i=0; i<change_params.char_len; i++)
	DBG_LOG("0x%02x ", change_params.char_new_value[i]);
	DBG_LOG("\n");
}

void anp_client_write_response_handler(at_ble_characteristic_write_response_t *params)
{
	at_ble_characteristic_write_response_t writersp;
	memcpy((uint8_t *)&writersp, params, sizeof(at_ble_characteristic_write_response_t));
	DBG_LOG("Write Response Conn_Handle:0x%02x char_handle:0x%02x status:0x%02x", writersp.conn_handle, writersp.char_handle, writersp.status);
	
}

void anp_client_notification_handler(at_ble_notification_recieved_t *params)
{
	 at_ble_notification_recieved_t notif;
	 memcpy((uint8_t *)&notif, params, sizeof(at_ble_notification_recieved_t));
	 
	 
	// DBG_LOG("Notification Received:0x%02x : 0x%02x", notif.char_value[0], notif.char_value[2]);
	 
	 
	 if(notif.char_value[0] == NOTIFICATION_ADDED)
	 {
		 if(notif.char_value[2] == CATEGORY_ID_INCOMINGCALL)
		 {
			 #ifdef OLED_MSG
			 gfx_mono_draw_string("                          ", 0, XPOS_UPDATE_POS_Y, &sysfont);
			 gfx_mono_draw_string("Incoming Call Alert", 0, XPOS_UPDATE_POS_Y, &sysfont);
			 #endif
			 DBG_LOG("Incoming Call Alert");
		 }
	 }
	 else if(notif.char_value[0] == NOTIFICATION_REMOVED)
	 {
		 if(notif.char_value[2] == CATEGORY_ID_INCOMINGCALL)
		 {
			 #ifdef OLED_MSG
			 gfx_mono_draw_string("                          ", 0, XPOS_UPDATE_POS_Y, &sysfont);
			 gfx_mono_draw_string("Alert Mode", 0, XPOS_UPDATE_POS_Y, &sysfont);
			 #endif
			 DBG_LOG("Alert Mode");
		 }
	 }
}

void anp_client_write_notification_handler(void)
{
	uint8_t data[2] = {1, 0};
	if(at_ble_characteristic_write(ancs_data.notification_source_desc.conn_handle, ancs_data.notification_source_desc.desc_handle, 0, 2, data,FALSE, TRUE) == AT_BLE_FAILURE)
	{
		DBG_LOG("\r\nFailed to send characteristic Write Request");
	}
}

void anp_client_init( void)
{
	DBG_LOG("In client init");
		
		anp_info_init();
		
		ancs_init(&ancs_data);
		
		anp_client_adv();
}