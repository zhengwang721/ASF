///**
 //* \file
 //*
 //* \brief TX Service
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
//#include "asf.h"
#include "tx_power.h"
#include "platform.h"

/* === GLOBALS ============================================================ */
uint8_t tx_power_initial_value = DEFAULT_TX_PWR_VALUE;



/* === IMPLEMENTATION ====================================================== */

/**
* \Tx power service initialisation
*/

void init_tx_power_service(gatt_service_handler_t *tx_power_serv )
{
	uint16_t tx_power_service_uuid_t = TX_POWER_SERVICE_UUID;
	tx_power_serv->serv_handle = 0;
	tx_power_serv->serv_uuid.type = AT_BLE_UUID_16;
	
	/*tx_power_serv->serv_uuid.uuid[0] = 0x04;
	tx_power_serv->serv_uuid.uuid[1] = 0x18;*/
	memcpy(tx_power_serv->serv_uuid.uuid,&tx_power_service_uuid_t,AT_BLE_UUID_16_LEN);
	
	//Characterisitc Info for Alert Value
	 /* handle stored here */
	tx_power_serv->serv_chars.char_val_handle = 0;         
	/* UUID : Alert Level */
	tx_power_serv->serv_chars.uuid.type = AT_BLE_UUID_16;
	tx_power_serv->serv_chars.uuid.uuid[0] = 0x07;          
	tx_power_serv->serv_chars.uuid.uuid[1] = 0x2a;         
	 /* Properties */
	tx_power_serv->serv_chars.properties = AT_BLE_CHAR_READ ; 
	/* value */
	tx_power_serv->serv_chars.init_value = &tx_power_initial_value ; 
	tx_power_serv->serv_chars.value_init_len = sizeof(int8_t);
	tx_power_serv->serv_chars.value_max_len = sizeof(int8_t);
	/* permissions */
	tx_power_serv->serv_chars.value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);   
	/* user defined name */
	tx_power_serv->serv_chars.user_desc = NULL;           
	tx_power_serv->serv_chars.user_desc_len = 0;
	tx_power_serv->serv_chars.user_desc_max_len = 0;
	/*user description permissions*/
	tx_power_serv->serv_chars.user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             
	/*client config permissions*/
	tx_power_serv->serv_chars.client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         
	/*server config permissions*/
	tx_power_serv->serv_chars.server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         
	 /*user desc handles*/
	tx_power_serv->serv_chars.user_desc_handle = 0;            
	/*client config handles*/
	tx_power_serv->serv_chars.client_config_handle = 0;         
	/*server config handles*/
	tx_power_serv->serv_chars.server_config_handle = 0;         
	/* presentation format */
	tx_power_serv->serv_chars.presentation_format = NULL;       
	
}


/**
* \Tx power service initialisation
*/
at_ble_status_t txps_primary_service_define(gatt_service_handler_t *txps_primary_service)
{
	
	return(at_ble_primary_service_define(&txps_primary_service->serv_uuid,
											&txps_primary_service->serv_handle,
											NULL, 0,
											&txps_primary_service->serv_chars, 1));
}	