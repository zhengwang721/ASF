/**
 * \file
 *
 * \brief Linkloss service
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

 /**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the Device Information Service
 */
/****************************************************************************************
*							        Includes	                                     	*
****************************************************************************************/

#include "link_loss.h"

/****************************************************************************************
*							        Globals	                                     		*
****************************************************************************************/

uint8_t linkloss_initial_value = 0;


/****************************************************************************************
*							        Implementations	                                    *
****************************************************************************************/



/** @brief Linkloss service initialization
  * 
  */
void init_linkloss_service(gatt_service_handler_t *linkloss_serv)
{
	 
	 linkloss_serv->serv_handle = 0;
	 linkloss_serv->serv_uuid.type = AT_BLE_UUID_16;
	
	 linkloss_serv->serv_uuid.uuid[0] = (uint8_t) LINK_LOSS_SERVICE_UUID	; 
	 linkloss_serv->serv_uuid.uuid[1] = (uint8_t) (LINK_LOSS_SERVICE_UUID >> 8);
	 
	 //Characteristic Info for Alert Value
	 
	 /* handle stored here */
	 linkloss_serv->serv_chars.char_val_handle = 0;
	           
	 /*16bit UUID : Alert Level */
	 linkloss_serv->serv_chars.uuid.type = AT_BLE_UUID_16;
	
	 linkloss_serv->serv_chars.uuid.uuid[0] = (uint8_t)  ALERT_LEVEL_CHAR_UUID;
	 linkloss_serv->serv_chars.uuid.uuid[1] = (uint8_t) (ALERT_LEVEL_CHAR_UUID >> 8);
	
	 /* Properties */
	 linkloss_serv->serv_chars.properties = AT_BLE_CHAR_READ | AT_BLE_CHAR_WRITE; 
	
	 /* value */
	 linkloss_serv->serv_chars.init_value = &linkloss_initial_value ; 
	 linkloss_serv->serv_chars.value_init_len = sizeof(int8_t);
	 linkloss_serv->serv_chars.value_max_len = sizeof(int8_t);
	
	 /* permissions */
	 linkloss_serv->serv_chars.value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);   
	
	 /* user defined name */
	 linkloss_serv->serv_chars.user_desc = NULL;           
	 linkloss_serv->serv_chars.user_desc_len = 0;
	 linkloss_serv->serv_chars.user_desc_max_len = 0;
	
	 /*user description permissions*/
	 linkloss_serv->serv_chars.user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             
	
	 /*client config permissions*/
	 linkloss_serv->serv_chars.client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         
	
	  /*server config permissions*/
	 linkloss_serv->serv_chars.server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;        
	
	  /*user desc handles*/
	 linkloss_serv->serv_chars.user_desc_handle = 0;            
	
	  /*client config handles*/
	 linkloss_serv->serv_chars.client_config_handle = 0;        
	
	 /*server config handles*/
	 linkloss_serv->serv_chars.server_config_handle = 0;         
	
	 /* presentation format */
	 linkloss_serv->serv_chars.presentation_format = NULL;       
	
}
 

/** @brief linkloss service definition
  * 
  */

at_ble_status_t lls_primary_service_define(gatt_service_handler_t *lls_service)
{
	return(at_ble_primary_service_define(&lls_service->serv_uuid,
											&lls_service->serv_handle,
											NULL, LLS_INCLUDED_SERVICE_COUNT,  
											&lls_service->serv_chars, LLS_CHARACTERISTIC_COUNT));
}


uint8_t lls_set_alert_value(at_ble_characteristic_changed_t *change_params, gatt_service_handler_t *lls_handler)
{
	if (change_params->char_handle == lls_handler->serv_chars.char_val_handle)
	{
		DBG_LOG("The current alert level for linkloss is %x",change_params->char_new_value[0]);
		return change_params->char_new_value[0];
	}
	return INVALID_LLS_PARAM;
}