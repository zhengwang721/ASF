
/**
 * \file
 *
 * \brief Proximity Reporter Profile Application
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



#include "asf.h"
#include "console_serial.h"
#include "pxp_reporter.h"


#define link_loss 1
#define path_loss 2

int8_t tx_power;


extern at_ble_handle_t linkloss_alert_characteristic_handle;
extern at_ble_handle_t immediate_alert_characteristic_handle;
extern at_ble_handle_t txpower_characteristic_handle;

volatile uint8_t linkloss_current_alert_level;
volatile uint8_t pathloss_alert_value;


uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xff, 0x00, 0x06, 0xd6, 0xb2, 0xf0, 0x05, 0xf0, 0xf8};

//void general_adv_start();
uint8_t proximity_reporter_init(void );
void app_init(void );

at_ble_LTK_t app_bond_info;
bool app_device_bond = false;
uint8_t auth_info = 0;


void app_init(void)
{
	uint8_t port = 74;
	at_ble_addr_t addr;
	uint16_t uuid = 0x1803;
	// init device
	at_ble_init(&port);
	
	at_ble_addr_get(&addr);
	/* Update the Address in scan response data*/
	//memcpy(&scan_rsp_data[4], &addr.addr, 6);
	memcpy(&addr.addr[0],&uuid,2);
	at_ble_addr_set(&addr);
}

int main(void )
{
	at_ble_events_t event;
	uint8_t params[512];
	
	at_ble_handle_t handle = 0;
	at_ble_status_t status;

	system_init();
	serial_console_init();
	app_init();


	if ((proximity_reporter_init()) != SUCCESS) {
		DBG_LOG_1LVL("\r\n Proximity reporter initialization failed\n");
	} 
	else 
	{
		DBG_LOG_1LVL("\r\nProximity reporter initialized successfully and started advertisement");
	}
	
	
	while(at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
	{
		switch(event)
		{
			case AT_BLE_CONNECTED:
			{
				
				at_ble_connected_t conn_params;
				memcpy((uint8_t *)&conn_params, params, sizeof(at_ble_connected_t));
				
				DBG_LOG("\r\nDevice connected to 0x%02x%02x%02x%02x%02x%02x handle=0x%x",
				conn_params.peer_addr.addr[5],
				conn_params.peer_addr.addr[4],
				conn_params.peer_addr.addr[3],
				conn_params.peer_addr.addr[2],
				conn_params.peer_addr.addr[1],
				conn_params.peer_addr.addr[0],
				conn_params.handle);
				handle = conn_params.handle;
				
				
				
				//Reading the transmission power 
				if ((tx_power = at_ble_tx_power_get(handle)) == -1) {
					DBG_LOG("Failed to read the tx power value\r\n");	
				} else {
					DBG_LOG("Tx power read is successfull and value is %x\r\n",tx_power);
				}
			
				if ((status = at_ble_characteristic_value_set(txpower_characteristic_handle,(uint8_t *)&tx_power,0 ,sizeof(int8_t))) != AT_BLE_SUCCESS){
					DBG_LOG("Setting the tx power characteristic failed because %x \r\n ",status);
				} else {
					DBG_LOG("Setting the tx power value is succesful \n");
				}
			}
			break;

			case AT_BLE_DISCONNECTED :
			{	
				
				at_ble_disconnected_t disconnect;
				memcpy((uint8_t *)&disconnect, params, sizeof(at_ble_disconnected_t));
				
				DBG_LOG("\r\nDevice disconnected Reason:0x%02x Handle=0x%x", disconnect.reason, disconnect.handle);
				
				
				if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY,
				APP_PXP_FAST_ADV, APP_PXP_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
				{
					DBG_LOG_1LVL("\r\nBLE Adv start Failed");
				}
				else
				{
					DBG_LOG_1LVL("\r\nBLE Started Adv");
				}
				
				
				
				while (1) {	
					if (linkloss_current_alert_level == 0)
						DBG_LOG("Link loss : Low Alert  \r\n");
					else if (linkloss_current_alert_level == 1)
						DBG_LOG("Link loss : Mid Alert  \r\n");
					else if (linkloss_current_alert_level == 2)
						DBG_LOG("Link loss : High Alert \r\n");
				}
			}
			break;
			
			case AT_BLE_CHARACTERISTIC_CHANGED:
			{
				at_ble_characteristic_changed_t change_params;
				uint32_t i = 0;
				
				memcpy((uint8_t *)&change_params, params, sizeof(at_ble_characteristic_changed_t));

				DBG_LOG("Characteristic 0x%x changed, new_value = ",
				change_params.char_handle);
				for(i=0; i<change_params.char_len; i++)
				DBG_LOG("0x%02x ", change_params.char_new_value[i]);
				DBG_LOG("\n");
				
				if (change_params.char_handle == linkloss_alert_characteristic_handle)
				{
					linkloss_current_alert_level = change_params.char_new_value[0];
					DBG_LOG("The current alert level for linkloss is %x \r\n",linkloss_current_alert_level); 
				} else if (change_params.char_handle == immediate_alert_characteristic_handle) {
					pathloss_alert_value = change_params.char_new_value[0];
					DBG_LOG("There is pathloss current alert level is %x \r\n",pathloss_alert_value);
				}  
			}
			break;
			
			case AT_BLE_CONN_PARAM_UPDATE_DONE : {
			DBG_LOG("\r\n AT_BLE_CONN_PARAM_UPDATE \n\r");
			}
			break;
			
			
			case AT_BLE_PAIR_REQUEST:
			{
				at_ble_pair_features_t features;
				uint8_t i = 0;
				char bond;
	
				DBG_LOG("\r\nRemote device request pairing");
			
				/* Check if we are already bonded (Only one bonded connection is supported
				in this example)*/
				if(app_device_bond)
				{
					DBG_LOG("\r\nBound relation exists with previously peer device");
					DBG_LOG("\r\nTo remove existing bonding information and accept pairing request from peer device press y else press n : ");
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
							DBG_LOG("\r\nPairing failed \n");
							break;
						}
						else
						{
							DBG_LOG("\r\nWrong value entered please try again : \n");
						}
					}while(app_device_bond);
				}
			
				if(!app_device_bond)
				{
					/* Authentication requirement is bond and MITM*/
					features.desired_auth =  AT_BLE_MODE1_L1_NOAUTH_PAIR_ENC;
					features.bond = false;
					features.mitm_protection = false;
					features.oob_avaiable = false;
					/* Device capabilities is display only , key will be generated 
					and displayed */
					features.io_cababilities = AT_BLE_IO_CAP_NO_INPUT_NO_OUTPUT;
					/* Distribution of LTK is required */
					features.initiator_keys =   AT_BLE_KEY_DIST_ENC;
					features.responder_keys =   AT_BLE_KEY_DIST_ENC;
					features.max_key_size = 16;
					features.min_key_size = 16;
				
					/* Generate LTK */
					for(i=0 ; i<8 ;i++)
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
					DBG_LOG("\r\n Sending pairing response handle=0x%x",
							handle);
					if(at_ble_authenticate(handle, &features, &app_bond_info, NULL) != AT_BLE_SUCCESS)
					{
						features.bond = false;
						features.mitm_protection = false;
						at_ble_authenticate(handle, &features, NULL, NULL);
					}					
				}
			}	
			break;
		
			case AT_BLE_PAIR_KEY_REQUEST:
			{
				/* Passkey has fixed value in this example MSB */
				uint8_t passkey[6]={0,0,0,0,0,0};
				uint8_t passkey_ascii[6];
				uint8_t i = 0;
	
				at_ble_pair_key_request_t pair_key_request;
				memcpy((uint8_t *)&pair_key_request, params, sizeof(at_ble_pair_key_request_t));
				/* Display passkey */
				if(pair_key_request.passkey_type == AT_BLE_PAIR_PASSKEY_DISPLAY)
				{
					/* Convert passkey to ASCII format */
					for(i=0; i<AT_BLE_PASSKEY_LEN ; i++)
					{
						passkey_ascii[i] = (passkey[i] + 48); 
					}
					DBG_LOG("\r\nplease enter the following code on the other device : ");
					for(i=0; i<AT_BLE_PASSKEY_LEN ; i++)
					{
						DBG_LOG("%c",passkey_ascii[i]);
					}
					DBG_LOG("\n");	
					at_ble_pair_key_reply(pair_key_request.handle,pair_key_request.type,passkey_ascii);
				}
			
			}
			break;

			case AT_BLE_PAIR_DONE:
			{
				at_ble_pair_done_t pair_params;
				memcpy((uint8_t *)&pair_params, params, sizeof(at_ble_pair_done_t));				
				if(pair_params.status == AT_BLE_SUCCESS)
				{
					DBG_LOG("\r\nPairing procedure completed successfully \n");
					app_device_bond = true;
					auth_info = pair_params.auth;
					
					handle = pair_params.handle;
					
					/* Enable the HTPT Profile */
					if(at_ble_htpt_enable(handle, HTPT_CFG_STABLE_MEAS_IND) == AT_BLE_FAILURE)
					{
						DBG_LOG("\r\nFailure in HTPT Profile Enable");
					}
				}
				else
				{
					DBG_LOG("\r\nPairing failed \n");
				}
			}
			break;

			case AT_BLE_ENCRYPTION_REQUEST:
			{
				bool key_found = false;
	
				at_ble_encryption_request_t enc_req;
				memcpy((uint8_t *)&enc_req, params, sizeof(at_ble_encryption_request_t));
			
				/* Check if bond information is stored */
				if((enc_req.ediv == app_bond_info.ediv)
					&& !memcmp(&enc_req.nb[0],&app_bond_info.nb[0],8))
				{
					key_found = true;
				}
			
				at_ble_encryption_request_reply(handle,auth_info ,key_found,app_bond_info);
			}
			break;
		
			case AT_BLE_ENCRYPTION_STATUS_CHANGED:
			{
				at_ble_encryption_status_changed_t enc_status;
				memcpy((uint8_t *)&enc_status, params, sizeof(at_ble_encryption_status_changed_t));
				if(enc_status.status == AT_BLE_SUCCESS)
				{
					DBG_LOG("\r\nEncryption completed successfully \n");
					
					handle = enc_status.handle;
					
					/* Enable the HTPT Profile */
					if(at_ble_htpt_enable(handle, HTPT_CFG_STABLE_MEAS_IND) == AT_BLE_FAILURE)
					{
						DBG_LOG("\r\nFailure in HTPT Profile Enable");
					}
				}
				else
				{
					DBG_LOG("\r\nEncryption failed \n");
				}
			}
			break;
			
			
			case AT_BLE_UNDEFINED_EVENT : {
			DBG_LOG("\r\n undefined event \n\r");
			}
			break;
			
			default:
			DBG_LOG("\r\nUnknown event received: event=0x%x", event);
			break;
		}
	}
	return 0;
}