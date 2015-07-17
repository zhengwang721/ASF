///**
 //* \file
 //*
 //* \brief Immediate Alert service
 //*
 //* Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 //*
 //* \asf_license_start
 //*
 //* \page License
 //*
 //* Redistribution and use in source and binary forms, with or without
 //* modification, are permitted provided that the following conditions are met:
 //*
 //* 1. Redistributions of source code must retain the above copyright notice,
 //*    this list of conditions and the following disclaimer.
 //*
 //* 2. Redistributions in binary form must reproduce the above copyright notice,
 //*    this list of conditions and the following disclaimer in the documentation
 //*    and/or other materials provided with the distribution.
 //*
 //* 3. The name of Atmel may not be used to endorse or promote products derived
 //*    from this software without specific prior written permission.
 //*
 //* 4. This software may only be redistributed and used in connection with an
 //*    Atmel microcontroller product.
 //*
 //* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 //* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 //* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 //* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 //* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 //* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 //* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 //* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 //* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 //* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 //* POSSIBILITY OF SUCH DAMAGE.
 //*
 //* \asf_license_stop
 //*
 //*/
///*
 //* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 //*/
//
 ///**
 //* \mainpage
 //* \section preface Preface
 //* This is the reference manual for the Device Information Service
 //*/
///*- Includes ---------------------------------------------------------------*/
#include "immediate_alert.h"
#include "platform.h"
///*- Globals ---------------------------------------------------------------*/
uint8_t immediate_alert_initial_value = 0;

///*- Implementation ---------------------------------------------------------------*/

/**
* \Immediate alert service initialisation
*/

void init_immediate_alert_service(gatt_service_handler_t *immediate_alert_serv ) 
{
		uint16_t immediate_alert_service_uuid_t = IMMEDIATE_ALERT_SERVICE_UUID;
		immediate_alert_serv->serv_handle = 0;
		immediate_alert_serv->serv_uuid.type = AT_BLE_UUID_16;
		
		/*immediate_alert_serv->serv_uuid.uuid[0] = 0x02;
		immediate_alert_serv->serv_uuid.uuid[1] = 0x18;*/
		memcpy(immediate_alert_serv->serv_uuid.uuid,&immediate_alert_service_uuid_t,AT_BLE_UUID_16_LEN);
		
		//Characterisitc Info for Alert Value
		/* handle stored here */
		immediate_alert_serv->serv_chars.char_val_handle = 0;       
		/* UUID : Alert Level */ 
		immediate_alert_serv->serv_chars.uuid.type = AT_BLE_UUID_16;
		immediate_alert_serv->serv_chars.uuid.uuid[0] = 0x06;          
		immediate_alert_serv->serv_chars.uuid.uuid[1] = 0x2a;          
		 /* Properties */
		immediate_alert_serv->serv_chars.properties = AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE;
		/* value */
		immediate_alert_serv->serv_chars.init_value = &immediate_alert_initial_value; 
		immediate_alert_serv->serv_chars.value_init_len = sizeof(int8_t);
		immediate_alert_serv->serv_chars.value_max_len = sizeof(int8_t);
		immediate_alert_serv->serv_chars.value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);   /* permissions */
		/* user defined name */
		immediate_alert_serv->serv_chars.user_desc = NULL;           
		immediate_alert_serv->serv_chars.user_desc_len = 0;
		immediate_alert_serv->serv_chars.user_desc_max_len = 0;
		/*user description permissions*/
		immediate_alert_serv->serv_chars.user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             
		/*client config permissions*/
		immediate_alert_serv->serv_chars.client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         
		/*server config permissions*/
		immediate_alert_serv->serv_chars.server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         
		/*user desc handles*/
		immediate_alert_serv->serv_chars.user_desc_handle = 0;             
		/*client config handles*/
		immediate_alert_serv->serv_chars.client_config_handle = 0;         
		 /*server config handles*/
		immediate_alert_serv->serv_chars.server_config_handle = 0;        
		/* presentation format */
		immediate_alert_serv->serv_chars.presentation_format = NULL;       
		
}

/**
* \Immediate alert service definition
*/

at_ble_status_t ias_primary_service_define(gatt_service_handler_t *ias_primary_service)
{
	
	return(at_ble_primary_service_define(&ias_primary_service->serv_uuid,
											&ias_primary_service->serv_handle,
											NULL, 0,
											&ias_primary_service->serv_chars, 1));
}
