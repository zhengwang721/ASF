/**
 * \file
 *
 * \brief TX Service
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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
/*- Includes ---------------------------------------------------------------*/

//Advertiser Mode

#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "profiles.h"
#include "console_serial.h"
#include "tx_power_service.h"

uint8_t tx_power_initial_value = 0;




void init_tx_power_service(tx_power_serv_record *tx_power_serv )
{
	tx_power_serv->tx_power_serv_handle = 0;
	tx_power_serv->tx_power_serv_uuid.type = AT_BLE_UUID_16;
	tx_power_serv->tx_power_serv_uuid.uuid[0] = 0x04;
	tx_power_serv->tx_power_serv_uuid.uuid[1] = 0x18;
	
	//Characterisitc Info for Alert Value
	tx_power_serv->tx_power_serv_chars.char_val_handle = 0;          /* handle stored here */
	tx_power_serv->tx_power_serv_chars.uuid.type = AT_BLE_UUID_16;
	tx_power_serv->tx_power_serv_chars.uuid.uuid[0] = 0x07;          /* UUID : Alert Level */
	tx_power_serv->tx_power_serv_chars.uuid.uuid[1] = 0x2a;          /* UUID : Alert Level */
	tx_power_serv->tx_power_serv_chars.properties = AT_BLE_CHAR_READ ; /* Properties */
	tx_power_serv->tx_power_serv_chars.init_value = &tx_power_initial_value ; /* value */
	tx_power_serv->tx_power_serv_chars.value_init_len = sizeof(int8_t);
	tx_power_serv->tx_power_serv_chars.value_max_len = sizeof(int8_t);
	tx_power_serv->tx_power_serv_chars.value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);   /* permissions */
	tx_power_serv->tx_power_serv_chars.user_desc = NULL;           /* user defined name */
	tx_power_serv->tx_power_serv_chars.user_desc_len = 0;
	tx_power_serv->tx_power_serv_chars.user_desc_max_len = 0;
	tx_power_serv->tx_power_serv_chars.user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	tx_power_serv->tx_power_serv_chars.client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	tx_power_serv->tx_power_serv_chars.server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	tx_power_serv->tx_power_serv_chars.user_desc_handle = 0;             /*user desc handles*/
	tx_power_serv->tx_power_serv_chars.client_config_handle = 0;         /*client config handles*/
	tx_power_serv->tx_power_serv_chars.server_config_handle = 0;         /*server config handles*/
	tx_power_serv->tx_power_serv_chars.presentation_format = NULL;       /* presentation format */
	
}



