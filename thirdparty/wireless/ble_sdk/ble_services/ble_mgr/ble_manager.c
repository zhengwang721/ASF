/**
* \file
*
* \brief Ble Manager
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


#include <asf.h>
#include <string.h>
#include "at_ble_api.h"
#include "ble_manager.h"
#include "ble_utils.h"

#if (BLE_DEVICE_ROLE == BLE_PERIPHERAL)
#include "link_loss.h"
#include "immediate_alert.h"
#include "tx_power.h"
#include "pxp_reporter.h"
#endif

#if defined PROXIMITY_MONITOR
#include "pxp_monitor.h"
#include "tx_power.h"
#include "link_loss.h"
#include "immediate_alert.h"
#endif


#if (BLE_DEVICE_ROLE == BLE_CENTRAL)
uint8_t scan_response_count = 0;
at_ble_scan_info_t scan_info[MAX_SCAN_DEVICE];
#endif

at_ble_connected_t ble_connected_dev_info[MAX_DEVICE_CONNECTED];

#if (BLE_DEVICE_ROLE == BLE_PERIPHERAL)
at_ble_LTK_t app_bond_info;
bool app_device_bond;
uint8_t auth_info;
#endif

static void ble_init(void);
static void ble_set_address(at_ble_addr_t *addr);

void ble_device_init(at_ble_addr_t *addr)
{
	ble_init();
	ble_set_address(addr);
	BLE_PROFILE_INIT(NULL);
}

/* Initialize the BLE */
static void ble_init(void)
{
	uint8_t port = 74;
	uint32_t chip_id;
	
	/* Initialize the platform */
	DBG_LOG("Initializing BTLC1000");
	
	/* Init BLE device */
	if(at_ble_init(&port) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BTLC1000 Initialization failed");
	}
	
	/* read BLE device chip id */
	if(at_ble_chip_id_get(&chip_id) != AT_BLE_SUCCESS)
	{
		DBG_LOG("Failed to get the BTLC1000 chip id");
	}
	
	DBG_LOG("BLE-chip id:0x%02X%02X%02X%02X", (uint8_t)(chip_id >> 24),
	(uint8_t)(chip_id >> 16),
	(uint8_t)(chip_id >> 8),
	(uint8_t)(chip_id));
}


/* Set BLE Address, If address is NULL then it will use BD public address */
static void ble_set_address(at_ble_addr_t *addr)
{
	if (addr == NULL)
	{
		at_ble_addr_t address;
		
		/* get BD address from BLE device */
		if(at_ble_addr_get(&address) != AT_BLE_SUCCESS)
		{
			DBG_LOG("BD address get failed");
		}
		
		/* set the BD address */
		if(at_ble_addr_set(&address) != AT_BLE_SUCCESS)
		{
			DBG_LOG("BD address set failed");
		}
		
		DBG_LOG("BD Address:0x%02X%02X%02X%02X%02X%02X, Address Type:%d",
		address.addr[5],
		address.addr[4],
		address.addr[3],
		address.addr[2],
		address.addr[1],
		address.addr[0], address.type);
	}
	else
	{
		/* set the given BD address */
		if(at_ble_addr_set(addr) != AT_BLE_SUCCESS)
		{
			DBG_LOG("BD address set failed");
		}
		DBG_LOG("BD Address:0x%02X%02X%02X%02X%02X%02X, Address Type:%d",
		addr->addr[5],
		addr->addr[4],
		addr->addr[3],
		addr->addr[2],
		addr->addr[1],
		addr->addr[0], addr->type);
	}
}

#if (BLE_DEVICE_ROLE == BLE_CENTRAL)
at_ble_status_t gap_dev_connect(at_ble_addr_t *dev_addr)
{
	at_ble_connection_params_t gap_conn_parameter;

	gap_conn_parameter.con_intv_min = GAP_CONN_INTERVAL_MIN;
	gap_conn_parameter.con_intv_max = GAP_CONN_INTERVAL_MAX;
	gap_conn_parameter.con_latency = GAP_CONN_SLAVE_LATENCY;
	gap_conn_parameter.ce_len_min = GAP_CE_LEN_MIN;
	gap_conn_parameter.ce_len_max = GAP_CE_LEN_MAX;
	gap_conn_parameter.superv_to = GAP_SUPERVISION_TIMOUT;

	return (at_ble_connect(dev_addr, GAP_CONNECT_PEER_COUNT, SCAN_INTERVAL, SCAN_WINDOW, &gap_conn_parameter));
}

at_ble_status_t gap_dev_scan(void)
{
	// make service discover counter to zero
	scan_response_count = 0;
	return(at_ble_scan_start(SCAN_INTERVAL, SCAN_WINDOW, SCAN_TIMEOUT, AT_BLE_SCAN_ACTIVE, AT_BLE_SCAN_GEN_DISCOVERY, false,true)) ;
}

at_ble_status_t ble_scan_info_handler(at_ble_scan_info_t *scan_param)
{
	if(scan_response_count < MAX_SCAN_DEVICE)
	{
		// store the advertising report data into scan_info[]
		memcpy((uint8_t *)&scan_info[scan_response_count++], scan_param, sizeof(at_ble_scan_info_t));
		DBG_LOG("Info:Device found address 0x%02X%02X%02X%02X%02X%02X ",
		scan_param->dev_addr.addr[5],
		scan_param->dev_addr.addr[4],
		scan_param->dev_addr.addr[3],
		scan_param->dev_addr.addr[2],
		scan_param->dev_addr.addr[1],
		scan_param->dev_addr.addr[0]);
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("Info:maximum of scan device reached");
		//Todo Stop Scanning
		return AT_BLE_FAILURE;
	}
}

at_ble_status_t ble_scan_report_handler(at_ble_scan_report_t *scan_report)
{
	if (scan_report->status == AT_BLE_SUCCESS)
	{
		/* All scan data should be handled */
		return (BLE_SCAN_DATA_HANDLER(scan_info, scan_response_count));
	}
	else
	{
		DBG_LOG("Scanning  failed");
	}
	return AT_BLE_FAILURE;
}
#endif


#if ((BLE_DEVICE_ROLE == BLE_CENTRAL) || (BLE_DEVICE_ROLE == BLE_PERIPHERAL))
void ble_connected_state_handler(at_ble_connected_t *conn_params)
{
	memcpy(ble_connected_dev_info, (uint8_t *)conn_params, sizeof(at_ble_connected_t));
	DBG_LOG("Device connected to 0x%02x%02x%02x%02x%02x%02x",
	conn_params->peer_addr.addr[5],
	conn_params->peer_addr.addr[4],
	conn_params->peer_addr.addr[3],
	conn_params->peer_addr.addr[2],
	conn_params->peer_addr.addr[1],
	conn_params->peer_addr.addr[0]);
	
	DBG_LOG("handle=0x%x status=%02X", conn_params->handle, conn_params->conn_status);

	
	if (at_ble_send_slave_sec_request(conn_params->handle,true,true) == AT_BLE_SUCCESS)
	{
		DBG_LOG("Slave security request successfull");
	}
	else {
		DBG_LOG("Slave security request failed");
	}
	
	
}

void ble_disconnected_state_handler(at_ble_disconnected_t *disconnect)
{
	
	DBG_LOG("Device disconnected Reason:0x%02x Handle=0x%x", disconnect->reason, disconnect->handle);
}

void ble_conn_param_update(at_ble_conn_param_update_done_t * conn_param_update)
{
	DBG_LOG("AT_BLE_CONN_PARAM_UPDATE ");
}

void ble_pair_request_handler(at_ble_pair_request_t *at_ble_pair_req)
{
	at_ble_pair_features_t features;
	uint8_t i = 0;
	char bond;
	
	DBG_LOG("Remote device request pairing");
	
	/* Check if we are already bonded (Only one bonded connection is supported
	in this example)*/
	if(app_device_bond)
	{
		DBG_LOG("Bound relation exists with previously peer device");
		DBG_LOG("To remove existing bonding information and accept pairing request from peer device press y else press n : ");
		do
		{
			bond = 'y';//getchar();
			if((bond == 'Y') || (bond == 'y'))
			{
				app_device_bond = false;
				break;
			}
			else if ((bond == 'N') || (bond == 'n'))
			{
				DBG_LOG("Pairing failed \n");
				break;
			}
			else
			{
				DBG_LOG("Wrong value entered please try again : \n");
			}
		}while(app_device_bond);
	}
	
	if(!app_device_bond)
	{
		/* Authentication requirement is bond and MITM*/
		//features.desired_auth =  AT_BLE_MODE1_L1_NOAUTH_PAIR_ENC;
		features.desired_auth =  AT_BLE_MODE1_L2_AUTH_PAIR_ENC;
		features.bond = true;
		features.mitm_protection = true;
		features.oob_avaiable = false;
		/* Device capabilities is display only , key will be generated
		and displayed */
		//features.io_cababilities = AT_BLE_IO_CAP_NO_INPUT_NO_OUTPUT;
		
		features.io_cababilities = AT_BLE_IO_CAP_DISPLAY_ONLY;
		/* Distribution of LTK is required */
		features.initiator_keys =   AT_BLE_KEY_DIST_ENC;
		features.responder_keys =   AT_BLE_KEY_DIST_ENC;
		features.max_key_size = 16;
		features.min_key_size = 16;
		
		/* Generate LTK */
		for(i=0; i<8; i++)
		{
			app_bond_info.key[i] = rand()&0x0f;
			app_bond_info.nb[i] = rand()&0x0f;
		}
		
		for(i=8 ; i<16 ;i++)
		{
			app_bond_info.key[i] = rand()&0x0f;
		}
		
		app_bond_info.ediv = rand()&0xffff;
		app_bond_info.key_size = 16;
		/* Send pairing response */
		DBG_LOG("Sending pairing response handle=0x%x",	ble_connected_dev_info->handle);
		if(at_ble_authenticate(ble_connected_dev_info->handle, &features, &app_bond_info, NULL) != AT_BLE_SUCCESS)
		{
			features.bond = false;
			features.mitm_protection = false;
			DBG_LOG(" != AT_BLE_SUCCESS ");
			at_ble_authenticate(ble_connected_dev_info->handle, &features, NULL, NULL);
			
		}
		DBG_LOG("authenticate done");
	}
}

void ble_pair_key_request_handler (at_ble_pair_key_request_t *pair_key)
{
	/* Passkey has fixed value in this example MSB */
	uint8_t passkey[6]={0,0,0,0,0,0};
	uint8_t passkey_ascii[6];
	uint8_t i = 0;
	
	at_ble_pair_key_request_t pair_key_request;
	memcpy((uint8_t *)&pair_key_request, pair_key, sizeof(at_ble_pair_key_request_t));
	DBG_LOG("handle 0x%02X ",pair_key_request.handle);
	DBG_LOG("passkey_type 0x%02X ",pair_key_request.passkey_type);	
	DBG_LOG("type 0x%02X ",pair_key_request.type);
	
	/* Display passkey */
	if(pair_key_request.passkey_type == AT_BLE_PAIR_PASSKEY_DISPLAY)
	{
		/* Convert passkey to ASCII format */
		for(i=0; i<AT_BLE_PASSKEY_LEN ; i++)
		{
			//DBG_LOG("please enter the 0x%02X ",passkey[i]);
			passkey_ascii[i] = (passkey[i] + 48);
		}
		DBG_LOG("please enter the following code on the other device : ");
		for(i=0; i<AT_BLE_PASSKEY_LEN ; i++)
		{
			DBG_LOG("%c",passkey_ascii[i]);
		}
		DBG_LOG("\n");
		at_ble_pair_key_reply(pair_key_request.handle,pair_key_request.type,passkey_ascii);
	}
	
}

at_ble_status_t ble_pair_done_handler(at_ble_pair_done_t *pairing_params)
{
	at_ble_pair_done_t pair_params;
	memcpy((uint8_t *)&pair_params, pairing_params, sizeof(at_ble_pair_done_t));
	
	if(pair_params.status == AT_BLE_SUCCESS)
	{
		DBG_LOG("Pairing procedure completed successfully \n");
		app_device_bond = true;
		auth_info = pair_params.auth;
		ble_connected_dev_info->handle = pair_params.handle;
		
		
	}
	else
	{
		DBG_LOG("Pairing failed \n");
	}
	return(AT_BLE_SUCCESS);
}

void ble_encryption_status_change_handler(at_ble_encryption_status_changed_t *encry_status)
{
	at_ble_encryption_status_changed_t enc_status;
	memcpy((uint8_t *)&enc_status, encry_status, sizeof(at_ble_encryption_status_changed_t));
	
	if(enc_status.status == AT_BLE_SUCCESS)
	{
		DBG_LOG("Encryption completed successfully \n");
			
		ble_connected_dev_info->handle = enc_status.handle;
			
	}
	else
	{
		DBG_LOG("Encryption failed \n");
	}
	
	
}

void ble_encryption_request_handler (at_ble_encryption_request_t *encry_req)
{
	bool key_found = false;

	at_ble_encryption_request_t enc_req;
	memcpy((uint8_t *)&enc_req, encry_req, sizeof(at_ble_encryption_request_t));

	/* Check if bond information is stored */
	if((enc_req.ediv == app_bond_info.ediv)
	&& !memcmp(&enc_req.nb[0],&app_bond_info.nb[0],8))
	{
		key_found = true;
	}

	at_ble_encryption_request_reply(ble_connected_dev_info->handle,auth_info ,key_found,app_bond_info);
}
#endif


void ble_event_manager(at_ble_events_t events, void *event_params)
{
	switch(events)
	{
		case AT_BLE_CONNECTED:
		{
			BLE_CONNECTED_STATE_HANDLER((at_ble_connected_t *)event_params);
		}
		break;

		case AT_BLE_DISCONNECTED:
		{
			BLE_DISCONNECTED_STATE_HANDLER((at_ble_disconnected_t *)event_params);
		}
		break;
				
		case AT_BLE_CHARACTERISTIC_CHANGED :
		{
			BLE_CHARACTERISTIC_CHANGED ((at_ble_characteristic_changed_t *)event_params);
		}
		break;
		
		case AT_BLE_CONN_PARAM_UPDATE_DONE :
		{
			BLE_CONN_PARAM_UPDATE_DONE((at_ble_conn_param_update_done_t *)event_params);
		}
		break;
		
		case AT_BLE_PAIR_REQUEST :
		{
			BLE_PAIR_REQUEST((at_ble_pair_request_t *)event_params);
		}
		break;
		
		case AT_BLE_PAIR_KEY_REQUEST:
		{
			BLE_PAIR_KEY_REQUEST((at_ble_pair_key_request_t *)event_params);
		}
		break;
		
		case AT_BLE_PAIR_DONE:
		{
			BLE_PAIR_DONE((at_ble_pair_done_t *)event_params);
		}
		break;
		case AT_BLE_ENCRYPTION_REQUEST:
		{
			BLE_ENCRYPTION_REQUEST((at_ble_encryption_request_t *)event_params);
		}
		break;
		
		case AT_BLE_ENCRYPTION_STATUS_CHANGED:
		{
			BLE_ENCRYPTION_STATUS_CHANGED((at_ble_encryption_status_changed_t *)event_params);
		}
		break;
		
		default:
		{
			DBG_LOG("ble_event_manager-Unknown Event: event=0x%X", events);
		}
		break;
	}
}

