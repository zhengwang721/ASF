/**
* \file
*
* \brief BLE Manager
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


#include <asf.h>
#include <string.h>
#include "at_ble_api.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "platform.h"

extern volatile bool init_done;

#if defined ATT_DB_MEMORY
uint32_t att_db_data[BLE_ATT_DB_MEMORY_SIZE/4];
#endif

/** @brief information of the connected devices */
at_ble_connected_t ble_connected_dev_info[MAX_DEVICE_CONNECTED];

at_ble_LTK_t app_bond_info;
bool app_device_bond;
at_ble_auth_t auth_info;


#define	MAX_GAP_EVENT_SUBSCRIBERS	 5
#define MAX_GATT_CLIENT_SUBSCRIBERS  5
#define MAX_GATT_SERVER_SUBSCRIBERS  5
#define MAX_L2CAP_EVENT_SUBSCRIBERS  1
#define MAX_HTPT_EVENT_SUBSCRIBERS   1
#define MAX_DTM_EVENT_SUBSCRIBERS	 5
#define MAX_CUSTOM_EVENT_SUBSCRIBERS 1

const ble_event_callback_t *ble_mgr_gap_event_cb[MAX_GAP_EVENT_SUBSCRIBERS];
const ble_event_callback_t *ble_mgr_gatt_client_event_cb[MAX_GATT_CLIENT_SUBSCRIBERS];
const ble_event_callback_t *ble_mgr_gatt_server_event_cb[MAX_GATT_SERVER_SUBSCRIBERS];
const ble_event_callback_t *ble_mgr_l2cap_event_cb[MAX_L2CAP_EVENT_SUBSCRIBERS];
const ble_event_callback_t *ble_mgr_htpt_event_cb[MAX_HTPT_EVENT_SUBSCRIBERS];
const ble_event_callback_t *ble_mgr_dtm_event_cb[MAX_DTM_EVENT_SUBSCRIBERS];
const ble_event_callback_t *ble_mgr_custom_event_cb[MAX_CUSTOM_EVENT_SUBSCRIBERS];

static const ble_event_callback_t ble_mgr_gap_handle[] = {
	ble_undefined_event_handler,
	ble_scan_info_handler,
	ble_scan_report_handler,
	NULL,
	NULL,
	ble_connected_state_handler,
	ble_disconnected_state_handler,
	ble_conn_param_update,
	ble_conn_param_update_req,
	ble_pair_done_handler,
	ble_pair_request_handler,
	ble_slave_security_request_handler,
	ble_pair_key_request_handler,	
	ble_encryption_request_handler,
	ble_encryption_status_change_handler,
	NULL,
	NULL,
	NULL,
	NULL
};

static const ble_event_callback_t ble_mgr_gatt_server_handle[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	ble_mtu_changed_indication_handler,
	ble_mtu_changed_cmd_complete_handler,
	ble_characteristic_write_cmd_complete_handler,
	NULL
};

volatile uint8_t scan_response_count = 0;
at_ble_scan_info_t scan_info[MAX_SCAN_DEVICE];


at_ble_events_t event;
uint8_t ble_event_params[BLE_EVENT_PARAM_MAX_SIZE];

/** @brief initializes the platform */
static void ble_init(at_ble_init_config_t * args);

/** @brief Set BLE Address, If address is NULL then it will use BD public address */
static void ble_set_address(at_ble_addr_t *addr);

/** @brief function to get event from stack */
at_ble_status_t ble_event_task(void)
{
    if (at_ble_event_get(&event, ble_event_params, BLE_EVENT_TIMEOUT) == AT_BLE_SUCCESS) 
    {
            ble_event_manager(event, ble_event_params);
            return AT_BLE_SUCCESS;
    }
    
    return AT_BLE_FAILURE;
}

/** @brief BLE device initialization */
void ble_device_init(at_ble_addr_t *addr)
{
	uint8_t idx;
	at_ble_init_config_t pf_cfg;
	platform_config busConfig;
	char *dev_name = NULL;

	/* Initialize the BLE Event callbacks */
	for (idx = 0; idx < MAX_GAP_EVENT_SUBSCRIBERS; idx++)
	{
		ble_mgr_gap_event_cb[idx] = NULL;
	}
	
	for (idx = 0; idx < MAX_GATT_CLIENT_SUBSCRIBERS; idx++)
	{
		ble_mgr_gatt_client_event_cb[idx] = NULL;
	}
	
	for (idx = 0; idx < MAX_GATT_SERVER_SUBSCRIBERS; idx++)
	{
		ble_mgr_gatt_server_event_cb[idx] = NULL;
	}
	
	for (idx = 0; idx < MAX_L2CAP_EVENT_SUBSCRIBERS; idx++)
	{
		ble_mgr_l2cap_event_cb[idx] = NULL;
	}
	
	for (idx = 0; idx < MAX_HTPT_EVENT_SUBSCRIBERS; idx++)
	{
		ble_mgr_htpt_event_cb[idx] = NULL;
	}
	
	for (idx = 0; idx < MAX_DTM_EVENT_SUBSCRIBERS; idx++)
	{
		ble_mgr_dtm_event_cb[idx] = NULL;
	}
	
	for (idx = 0; idx < MAX_CUSTOM_EVENT_SUBSCRIBERS; idx++)
	{
		ble_mgr_custom_event_cb[idx] = NULL;
	}
	
	
#if defined ATT_DB_MEMORY
	pf_cfg.memPool.memSize = BLE_ATT_DB_MEMORY_SIZE;
	pf_cfg.memPool.memStartAdd = (uint8_t *)&att_db_data;
#else
	pf_cfg.memPool.memSize = 0;
	pf_cfg.memPool.memStartAdd = NULL;
#endif
	/*Bus configuration*/
	busConfig.bus_type = AT_BLE_UART;
	busConfig.bus_info = 0; /* Bus Info Not used */
	pf_cfg.plf_config = &busConfig;	
	
	ble_init(&pf_cfg);
	
	init_done = true;
	
	ble_mgr_events_callback_handler(REGISTER_CALL_BACK, 
									BLE_GAP_EVENT_TYPE, 
									ble_mgr_gap_handle);
	ble_mgr_events_callback_handler(REGISTER_CALL_BACK,
									BLE_GATT_SERVER_EVENT_TYPE,
									ble_mgr_gatt_server_handle);
									
	
	ble_set_address(addr);	
	
	dev_name = (char *)BLE_DEVICE_NAME;
	if (ble_set_device_name((uint8_t *)dev_name, strlen(dev_name)) != AT_BLE_SUCCESS)
	{
		DBG_LOG("Device name set failed");
	}
}

/** @brief set device name to BLE Device*/
at_ble_status_t ble_set_device_name(uint8_t *name, uint8_t name_len)
{
	if ((name == NULL) || (name_len < 1))
	{
		return AT_BLE_INVALID_PARAM;
	}
	return at_ble_device_name_set(name, name_len);
}

/* Initialize the BLE */
static void ble_init(at_ble_init_config_t * args)
{
	/* Initialize the platform */
	DBG_LOG("Initializing BTLC1000");
	
	/* Init BLE device */
	if(at_ble_init(args) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BTLC1000 Initialization failed");
		DBG_LOG("Please check the power and connection / hardware connector");	
		while(1);
	}
}


/* Set BLE Address, If address is NULL then it will use BD public address */
static void ble_set_address(at_ble_addr_t *addr)
{
	at_ble_dev_config_t stDevConfig;
	at_ble_addr_t address = {AT_BLE_ADDRESS_PUBLIC, {0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF}};
	
	if (addr == NULL)
	{		
		/* get BD address from BLE device */
		if(at_ble_addr_get(&address) != AT_BLE_SUCCESS)
		{
			DBG_LOG("BD address get failed");
		}
		
		/* Copy the BD address into address pointer */
		addr = &address;
	}
	
	DBG_LOG("BD Address:0x%02X%02X%02X%02X%02X%02X, Address Type:%d",
	addr->addr[5],
	addr->addr[4],
	addr->addr[3],
	addr->addr[2],
	addr->addr[1],
	addr->addr[0], addr->type);
	
	/* Set device configuration */
	/* Device role */
	stDevConfig.role = BLE_DEVICE_ROLE;
	/* device renew duration */
	stDevConfig.renew_dur = AT_RENEW_DUR_VAL_MIN;
	/* device address type */
	memcpy((uint8_t *)&stDevConfig.address, (uint8_t *)addr, sizeof(at_ble_addr_t));
	/* Attributes */
	stDevConfig.att_cfg.b2NamePerm = AT_BLE_WRITE_DISABLE;
	stDevConfig.att_cfg.b2AppearancePerm = AT_BLE_WRITE_DISABLE;
	stDevConfig.att_cfg.b1EnableSpcs = 0;
	stDevConfig.att_cfg.b1EnableServiceChanged = 0;
	stDevConfig.att_cfg.b2Rfu = AT_BLE_WRITE_DISABLE;
	/* Handles */
	stDevConfig.gap_start_hdl = AT_BLE_AUTO_ALLOC_HANDLE;
	stDevConfig.gatt_start_hdl = AT_BLE_AUTO_ALLOC_HANDLE;
	/* MTU */
	stDevConfig.max_mtu = AT_MTU_VAL_RECOMMENDED;
	
	if(at_ble_set_dev_config(&stDevConfig) != AT_BLE_SUCCESS)
	{
		DBG_LOG("Set BLE Device configuration failed");
	}
}

bool ble_mgr_events_callback_handler(ble_mgr_event_cb_t event_cb_type, 
									ble_mgr_event_t event_type,
									const ble_event_callback_t *ble_event_handler)
{
	uint8_t idx;
	uint8_t max_event_handler = 0;
	bool status = true;
	const ble_event_callback_t **ble_event_cb_ptr = NULL;
	if (ble_event_handler != NULL)
	{
		switch(event_type)
		{
			case BLE_GAP_EVENT_TYPE:
			{
				ble_event_cb_ptr = ble_mgr_gap_event_cb;
				max_event_handler = MAX_GAP_EVENT_SUBSCRIBERS;
			}
			break;
			
			case BLE_GATT_CLIENT_EVENT_TYPE:
			{
				ble_event_cb_ptr = ble_mgr_gatt_client_event_cb;
				max_event_handler = MAX_GATT_CLIENT_SUBSCRIBERS;
			}
			break;
			
			case BLE_GATT_SERVER_EVENT_TYPE:
			{
				ble_event_cb_ptr = ble_mgr_gatt_server_event_cb;
				max_event_handler = MAX_GATT_SERVER_SUBSCRIBERS;
			}
			break;
			
			case BLE_L2CAP_EVENT_TYPE:
			{
				ble_event_cb_ptr = ble_mgr_l2cap_event_cb;
				max_event_handler = MAX_L2CAP_EVENT_SUBSCRIBERS;
			}
			break;
			
			case BLE_GATT_HTPT_EVENT_TYPE:
			{
				ble_event_cb_ptr = ble_mgr_htpt_event_cb;
				max_event_handler = MAX_HTPT_EVENT_SUBSCRIBERS;
			}
			break;
			
			case BLE_DTM_EVENT_TYPE:
			{
				ble_event_cb_ptr = ble_mgr_dtm_event_cb;
				max_event_handler = MAX_DTM_EVENT_SUBSCRIBERS;
			}
			break;
			
			case BLE_CUSTOM_EVENT_TYPE:
			{
				ble_event_cb_ptr = ble_mgr_custom_event_cb;
				max_event_handler = MAX_CUSTOM_EVENT_SUBSCRIBERS;
			}
			break;
			
			default:
			{
				status = false; //Invalid Parameter
			}			
			break;
		}
		
		if (status)
		{
			status = false; //Defaults to Failure[No Space to register, Not Found, Invalid Type]scenario
			if (event_cb_type == REGISTER_CALL_BACK)
			{
				for (idx = 0; idx < max_event_handler; idx++)
				{
					if(ble_event_cb_ptr[idx] == NULL)
					{
						ble_event_cb_ptr[idx] = ble_event_handler;
						status = true; //Successfully registered to table
						break;
					}
				}
			}
			else if(event_cb_type == UNREGISTER_CALL_BACK)
			{
				for (idx = 0; idx < max_event_handler; idx++)
				{
					if(ble_event_cb_ptr[idx] == ble_event_handler)
					{
						ble_event_cb_ptr[idx] = NULL;
						status = true;//Successfully unregistered from table
						break;
					}
				}
			}			
		}
	}
	else
	{
		status = false; //Invalid parameter
	}
	return status;
}

at_ble_status_t ble_undefined_event_handler(void *params)
{
	DBG_LOG_DEV("Undefined Event Received");
	return AT_BLE_SUCCESS;
}

/** @brief request the peer device for connection */
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

/** @brief instructs device to start scanning */
at_ble_status_t gap_dev_scan(void)
{
	/* Device Scan discover started*/
	DBG_LOG("Scanning...Please wait...");
	/* make service discover counter to zero*/
	scan_response_count = 0;
	return(at_ble_scan_start(SCAN_INTERVAL, SCAN_WINDOW, SCAN_TIMEOUT, SCAN_TYPE, AT_BLE_SCAN_GEN_DISCOVERY, false,true)) ;
}

/** @brief function handling scaned information */
at_ble_status_t ble_scan_info_handler(void *params)
{
	at_ble_scan_info_t *scan_param;
	scan_param = (at_ble_scan_info_t *)params;
	if(scan_response_count < MAX_SCAN_DEVICE)
	{
		// store the advertising report data into scan_info[]
		memcpy((uint8_t *)&scan_info[scan_response_count], scan_param, sizeof(at_ble_scan_info_t));
		DBG_LOG_DEV("Info:Device found address [%d]  0x%02X%02X%02X%02X%02X%02X ",
		scan_response_count,
		scan_param->dev_addr.addr[5],
		scan_param->dev_addr.addr[4],
		scan_param->dev_addr.addr[3],
		scan_param->dev_addr.addr[2],
		scan_param->dev_addr.addr[1],
		scan_param->dev_addr.addr[0]);
		scan_response_count++;
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("Info:maximum no.of scan device reached...Stopping Scan");
		if(at_ble_scan_stop() != AT_BLE_SUCCESS)
		{
			DBG_LOG("Failed to stop scanning");
		}
		
		return AT_BLE_FAILURE;
	}
}

/** @brief function handles scan report */
at_ble_status_t ble_scan_report_handler(void *params)
{
	at_ble_scan_report_t *scan_report;
	scan_report = (at_ble_scan_report_t *)params;
	if (scan_report->status == AT_BLE_SUCCESS)
	{
		/* All scan data should be handled */
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("Scanning  failed");
	}
	return AT_BLE_FAILURE;
}

/* Parse the received advertising data for service and local name */
uint8_t scan_info_parse(at_ble_scan_info_t *scan_info_data,
				at_ble_uuid_t *ble_service_uuid, uint8_t adv_type)
{
	if (scan_info_data->adv_data_len) {
		uint8_t adv_data_size;
		uint8_t index = 0;
		volatile adv_element_t *adv_element_p;
		volatile adv_element_t adv_element_data;

		adv_data_size = scan_info_data->adv_data_len;
		adv_element_data.len = 0;
		while (adv_data_size) {
			adv_element_data.len = scan_info_data->adv_data[index];
			adv_element_data.type
			= scan_info_data->adv_data[index + 1];
			adv_element_data.data
			= &scan_info_data->adv_data[index + 2];
			adv_element_p = &adv_element_data;

			if (adv_element_p->type == adv_type) {
				/* passing the length of data type */
				uint8_t adv_type_size = adv_element_p->len;
				/* actual size of the data	*/
				adv_type_size -= 1;
				while (adv_type_size) {
					volatile int cmp_status = -1;
					if (ble_service_uuid->type ==
					AT_BLE_UUID_16) {
						cmp_status = memcmp(
						adv_element_p->data, ble_service_uuid->uuid,
						AT_BLE_UUID_16_LEN);
						adv_element_p->data
						+= AT_BLE_UUID_16_LEN;
						adv_type_size
						-= AT_BLE_UUID_16_LEN;
					} else if (ble_service_uuid->type ==
					AT_BLE_UUID_32) {
						cmp_status = memcmp(
						adv_element_p->data, ble_service_uuid->uuid,
						AT_BLE_UUID_32_LEN);
						adv_element_p->data
						+= AT_BLE_UUID_32_LEN;
						adv_type_size
						-= AT_BLE_UUID_32_LEN;
					} else if (ble_service_uuid->type ==
					AT_BLE_UUID_128) {
						cmp_status = memcmp(
						adv_element_p->data, ble_service_uuid->uuid,
						AT_BLE_UUID_128_LEN);
						adv_element_p->data
						+= AT_BLE_UUID_128_LEN;
						adv_type_size
						-= AT_BLE_UUID_32_LEN;
					}

					if (cmp_status == 0) {
						return AT_BLE_SUCCESS;
					}
				}
			}

			index += (adv_element_data.len + 1);
			adv_element_data.len += 1;
			adv_data_size -= adv_element_data.len;
		}
	}

	return AT_BLE_FAILURE;
}

/** @brief function to send slave security request */
at_ble_status_t ble_send_slave_sec_request(at_ble_handle_t conn_handle)
{
	#if BLE_PAIR_ENABLE
		if (at_ble_send_slave_sec_request(conn_handle, BLE_MITM_REQ, BLE_BOND_REQ) == AT_BLE_SUCCESS)
		{
			DBG_LOG_DEV("Slave security request successful");
			return AT_BLE_SUCCESS;
		}
		else
		{
			DBG_LOG("Slave security request failed");
		}
	#endif
	return AT_BLE_FAILURE;
}

/** @brief function to handle connected event received from stack */
at_ble_status_t ble_connected_state_handler(void *params)
{
	at_ble_connected_t *conn_params;
	conn_params = (at_ble_connected_t *)params;
	memcpy(ble_connected_dev_info, (uint8_t *)conn_params, sizeof(at_ble_connected_t));
	if (conn_params->conn_status == AT_BLE_SUCCESS)
	{
		DBG_LOG("Connected to peer device with address 0x%02x%02x%02x%02x%02x%02x",
		conn_params->peer_addr.addr[5],
		conn_params->peer_addr.addr[4],
		conn_params->peer_addr.addr[3],
		conn_params->peer_addr.addr[2],
		conn_params->peer_addr.addr[1],
		conn_params->peer_addr.addr[0]);
		
		DBG_LOG("Connection Handle %d", conn_params->handle);
		
#if (BLE_DEVICE_ROLE == BLE_ROLE_PERIPHERAL)
	ble_send_slave_sec_request(conn_params->handle);
#endif
	} 
	else
	{
		DBG_LOG("Device Connection Failed");
	}	
	return AT_BLE_SUCCESS;
}

at_ble_status_t ble_mtu_changed_indication_handler(void *params)
{
	at_ble_mtu_changed_ind_t *mtu_changed_ind;
	mtu_changed_ind = (at_ble_mtu_changed_ind_t *)params;
	DBG_LOG("BLE-MTU Changed, Connection Handle: %d, New Value: %d", 
										mtu_changed_ind->conhdl, 
										mtu_changed_ind->mtu_value);
	return AT_BLE_SUCCESS;
}

at_ble_status_t ble_mtu_changed_cmd_complete_handler(void *params)
{
	at_ble_cmd_complete_event_t *cmd_complete_event;
	cmd_complete_event = (at_ble_cmd_complete_event_t *)params;
	
	if (cmd_complete_event->status == AT_BLE_SUCCESS)
	{
		DBG_LOG("MTU Changed, Connection Handle:%d, Operation:%d", 
		cmd_complete_event->conn_handle, 
		cmd_complete_event->operation);
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("MTU Change Failed, Connection Handle:%d, Operation:%d",
		cmd_complete_event->conn_handle,
		cmd_complete_event->operation);
	}
	return AT_BLE_FAILURE;
}

at_ble_status_t ble_characteristic_write_cmd_complete_handler(void *params)
{
	at_ble_cmd_complete_event_t *cmd_complete_event;
	cmd_complete_event = (at_ble_cmd_complete_event_t *)params;
	if (cmd_complete_event->status == AT_BLE_SUCCESS)
	{
		DBG_LOG("Char Write Cmd Complete, Connection Handle:%d, Operation:%d",
		cmd_complete_event->conn_handle,
		cmd_complete_event->operation);
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("Char Write Cmd Failed, Connection Handle:%d, Operation:%d",
		cmd_complete_event->conn_handle,
		cmd_complete_event->operation);
	}
	return AT_BLE_FAILURE;
}

/** @brief function handles disconnection event received from stack */
at_ble_status_t ble_disconnected_state_handler(void *params)
{
	at_ble_disconnected_t *disconnect;
	disconnect = (at_ble_disconnected_t *)params;
	DBG_LOG("Device disconnected Reason:0x%02x Handle=0x%x", disconnect->reason, disconnect->handle);
	return AT_BLE_SUCCESS;
}

/** @brief connection update parameter function */
at_ble_status_t ble_conn_param_update(void *params)
{
	at_ble_conn_param_update_done_t * conn_param_update;
	conn_param_update = (at_ble_conn_param_update_done_t *)params;
	DBG_LOG_DEV("AT_BLE_CONN_PARAM_UPDATE ");
	ALL_UNUSED(conn_param_update);  //To avoid compiler warning
	return AT_BLE_SUCCESS;
}

at_ble_status_t ble_conn_param_update_req(void *params)
{
	at_ble_conn_param_update_request_t * conn_param_req;
	conn_param_req = (at_ble_conn_param_update_request_t *)params;
	at_ble_conn_update_reply(conn_param_req->handle, true, 1, 120);
	return AT_BLE_SUCCESS;
}

at_ble_status_t ble_slave_security_request_handler(void* params)
{
	at_ble_pair_features_t features;
	uint8_t i = 0;
	at_ble_slave_sec_request_t* slave_sec_req;
	slave_sec_req = (at_ble_slave_sec_request_t*)params;
	
	if (app_device_bond)
	{
		app_device_bond = false;
	}
	
	if(!app_device_bond)
	{

		features.desired_auth =  BLE_AUTHENTICATION_LEVEL; 
		features.bond = slave_sec_req->bond;
		features.mitm_protection = slave_sec_req->mitm_protection;
		/* Device capabilities is display only , key will be generated
		and displayed */
		features.io_cababilities = AT_BLE_IO_CAP_KB_DISPLAY;

		features.oob_avaiable = false;
			
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
		DBG_LOG("Sending pairing response");
		if(at_ble_authenticate(slave_sec_req->handle, &features, &app_bond_info, NULL) != AT_BLE_SUCCESS)
		{
			features.bond = false;
			features.mitm_protection = false;
			DBG_LOG(" != AT_BLE_SUCCESS ");
			at_ble_authenticate(slave_sec_req->handle, &features, NULL, NULL);			
		}
	}
	return AT_BLE_SUCCESS;
}

/** @brief function handles pair request */
at_ble_status_t ble_pair_request_handler(void *params)
{
	at_ble_pair_features_t features;
	uint8_t idx = 0;
	at_ble_pair_request_t *at_ble_pair_req;
	at_ble_pair_req = (at_ble_pair_request_t *)params;
	
	DBG_LOG("Peer device request pairing");
	
	/* Check if we are already bonded (Only one bonded connection is supported
	in this example)*/
	if(app_device_bond)
	{
		DBG_LOG("Bonding information exists with peer device...Removing Bonding information");
		app_device_bond = false;
	}
	
	if(!app_device_bond)
	{
		/* Authentication requirement is bond and MITM*/
		features.desired_auth = BLE_AUTHENTICATION_LEVEL;
		features.bond = BLE_BOND_REQ;
		features.mitm_protection = BLE_MITM_REQ;
		features.io_cababilities = BLE_IO_CAPABALITIES;	
		features.oob_avaiable = BLE_OOB_REQ;
			
		/* Distribution of LTK is required */
		features.initiator_keys =   AT_BLE_KEY_DIST_ENC;
		features.responder_keys =   AT_BLE_KEY_DIST_ENC;
		features.max_key_size = 16;
		features.min_key_size = 16;
		
		/* Generate LTK */
		for(idx=0; idx<8; idx++)
		{
			app_bond_info.key[idx] = rand()&0x0f;
			app_bond_info.nb[idx] = rand()&0x0f;
		}
		
		for(idx=8 ; idx<16 ;idx++)
		{
			app_bond_info.key[idx] = rand()&0x0f;
		}
		
		app_bond_info.ediv = rand()&0xffff;
		app_bond_info.key_size = 16;
		/* Send pairing response */
		DBG_LOG("Sending pairing response");
		if(at_ble_authenticate(ble_connected_dev_info->handle, &features, &app_bond_info, NULL) != AT_BLE_SUCCESS)
		{
			features.bond = false;
			features.mitm_protection = false;
			DBG_LOG("BLE Authentication Failed..Retrying without mitm without bond");
			if(!(at_ble_authenticate(ble_connected_dev_info->handle, &features, NULL, NULL) == AT_BLE_SUCCESS))
			{
				DBG_LOG("BLE Authentication Retry Failed");
			}			
		}
	}
	ALL_UNUSED(at_ble_pair_req);  //To avoid compiler warning
	return AT_BLE_SUCCESS;
}

/** @brief function handles pair key request */
at_ble_status_t ble_pair_key_request_handler (void *params)
{
	at_ble_pair_key_request_t *pair_key;
	pair_key = (at_ble_pair_key_request_t *)params;
	/* Passkey has fixed value in this example MSB */
	uint8_t passkey[6]={'1','2','3','4','5','6'};
	uint8_t idx = 0;
        uint8_t pin;
        
	at_ble_pair_key_request_t pair_key_request;
        
	memcpy((uint8_t *)&pair_key_request, pair_key, sizeof(at_ble_pair_key_request_t));
	
	if (pair_key_request.passkey_type == AT_BLE_PAIR_PASSKEY_ENTRY) {
	  DBG_LOG("Enter the Passkey(6-Digit) or q to quit in Terminal:");
	  for (idx = 0; idx < 6;) {          
		pin = getchar();
		if ((pin >= '0') && ( pin <= '9')) {
		  passkey[idx++] = pin;
		  DBG_LOG_CONT("%c", pin);
		} else if (pin == 'q') {
			DBG_LOG("Disconnecting ...");
			if (!(at_ble_disconnect(ble_connected_dev_info->handle, 
							AT_BLE_TERMINATED_BY_USER) == AT_BLE_SUCCESS)) {
				DBG_LOG("Disconnect Request Failed");
			}
			return AT_BLE_FAILURE;
		}
	  }
	}	
	
	/* Display passkey */
	if(((pair_key_request.passkey_type == AT_BLE_PAIR_PASSKEY_DISPLAY) &&
	   (pair_key_request.type == AT_BLE_PAIR_PASSKEY)) || (pair_key_request.passkey_type == AT_BLE_PAIR_PASSKEY_ENTRY))
	{
          if(pair_key_request.passkey_type == AT_BLE_PAIR_PASSKEY_ENTRY)
          {
            DBG_LOG("Entered Pass-code:");
          }
          else
          {
            DBG_LOG("Please Enter the following Pass-code(on other Device):");
          }
          
          /* Convert passkey to ASCII format */
          for(idx=0; idx<AT_BLE_PASSKEY_LEN; idx++)
          {
                  passkey[idx] = (passkey[idx]);
                  DBG_LOG_CONT("%c",passkey[idx]);
          }		
          
          if(!(at_ble_pair_key_reply(pair_key_request.handle, pair_key_request.type, passkey)) == AT_BLE_SUCCESS)
          {
                  DBG_LOG("Pair-key reply failed");
          }
	}
	else 
	{
		if(pair_key_request.type == AT_BLE_PAIR_OOB)
		{
			DBG_LOG("OOB Feature Not supported");
		}
	}
	return AT_BLE_SUCCESS;	
}

/** @brief function handles pair done event */
at_ble_status_t ble_pair_done_handler(void *params)
{
	at_ble_pair_done_t pair_params;
	at_ble_pair_done_t *pairing_params;
	pairing_params = (at_ble_pair_done_t *)params;
	memcpy((uint8_t *)&pair_params, pairing_params, sizeof(at_ble_pair_done_t));
	
	if(pair_params.status == AT_BLE_SUCCESS)
	{
		DBG_LOG("Pairing procedure completed successfully");
		app_device_bond = true;
		auth_info = pair_params.auth;
		ble_connected_dev_info->handle = pair_params.handle;
	}
	else
	{
		DBG_LOG("Pairing failed...Disconnecting");
		if(!(at_ble_disconnect(ble_connected_dev_info->handle, AT_BLE_TERMINATED_BY_USER) == AT_BLE_SUCCESS))
		{
			DBG_LOG("Disconnect Request Failed");
		}
	}
	return AT_BLE_SUCCESS;
}

/** @brief function handles encryption status change */
at_ble_status_t ble_encryption_status_change_handler(void *params)
{
	at_ble_encryption_status_changed_t enc_status;
	at_ble_encryption_status_changed_t *encry_status;
	encry_status = (at_ble_encryption_status_changed_t *)params;
	memcpy((uint8_t *)&enc_status, encry_status, sizeof(at_ble_encryption_status_changed_t));
	
	if(enc_status.status == AT_BLE_SUCCESS)
	{
		DBG_LOG("Encryption completed successfully");
			
		ble_connected_dev_info->handle = enc_status.handle;		
	}
	else
	{
		DBG_LOG("Encryption failed");
	}
	return AT_BLE_SUCCESS;
}

/** @brief function handles encryption requests */
at_ble_status_t ble_encryption_request_handler(void *params)
{
	at_ble_encryption_request_t *encry_req;
	bool key_found = false;
	encry_req = (at_ble_encryption_request_t *)params;

	at_ble_encryption_request_t enc_req;
	memcpy((uint8_t *)&enc_req, encry_req, sizeof(at_ble_encryption_request_t));

	/* Check if bond information is stored */
	if((enc_req.ediv == app_bond_info.ediv)
	&& !memcmp(&enc_req.nb[0],&app_bond_info.nb[0],8))
	{
		key_found = true;
	}
        else
        {
          DBG_LOG("Pairing information of peer device is not available."); 
          DBG_LOG("Please unpair the device from peer device(mobile) settings menu and start pairing again");
        }

	if(!(at_ble_encryption_request_reply(ble_connected_dev_info->handle,auth_info ,key_found, &app_bond_info) == AT_BLE_SUCCESS))
	{
		DBG_LOG("Encryption Request Reply Failed");
	}
	else
	{
		DBG_LOG_DEV("Encryption Request Reply");
	}
	return AT_BLE_SUCCESS;
}

void ble_event_manager(at_ble_events_t events, void *event_params)
{
	DBG_LOG_DEV("BLE-Event:%d", events);
	switch(events)
	{		
	 /* GAP events */
	case AT_BLE_UNDEFINED_EVENT:
	case AT_BLE_SCAN_INFO:
	case AT_BLE_SCAN_REPORT:
	case AT_BLE_ADV_REPORT:
	case AT_BLE_RAND_ADDR_CHANGED:
	case AT_BLE_CONNECTED:
	case AT_BLE_DISCONNECTED:
	case AT_BLE_CONN_PARAM_UPDATE_DONE:
	case AT_BLE_CONN_PARAM_UPDATE_REQUEST:
	case AT_BLE_PAIR_DONE:
	case AT_BLE_PAIR_REQUEST:
	case AT_BLE_SLAVE_SEC_REQUEST:
	case AT_BLE_PAIR_KEY_REQUEST:
	case AT_BLE_ENCRYPTION_REQUEST:
	case AT_BLE_ENCRYPTION_STATUS_CHANGED:
	case AT_BLE_RESOLV_RAND_ADDR_STATUS:
	case AT_BLE_SIGN_COUNTERS_IND:
	case AT_BLE_PEER_ATT_INFO_IND:
	case AT_BLE_CON_CHANNEL_MAP_IND:
	{
		uint8_t idx;
		for (idx = 0; idx < MAX_GAP_EVENT_SUBSCRIBERS; idx++)
		{
			if (ble_mgr_gap_event_cb[idx] != NULL)
			{
				const ble_event_callback_t *event_cb_fn = ble_mgr_gap_event_cb[idx];
				if(event_cb_fn[events] != NULL)
				{
					event_cb_fn[events](event_params);
				}
			}
		}
	}
	break;
	
	case AT_BLE_PRIMARY_SERVICE_FOUND:
	case AT_BLE_INCLUDED_SERVICE_FOUND:
	case AT_BLE_CHARACTERISTIC_FOUND:
	case AT_BLE_DESCRIPTOR_FOUND:
	case AT_BLE_DISCOVERY_COMPLETE:
	case AT_BLE_CHARACTERISTIC_READ_BY_UUID_RESPONSE:
	case AT_BLE_CHARACTERISTIC_READ_MULTIBLE_RESPONSE:
	case AT_BLE_CHARACTERISTIC_WRITE_RESPONSE:
	case AT_BLE_NOTIFICATION_RECIEVED:
	case AT_BLE_INDICATION_RECIEVED:
	{
		uint8_t idx;
		events -= (AT_BLE_CON_CHANNEL_MAP_IND + 1);
		for (idx = 0; idx < MAX_GATT_CLIENT_SUBSCRIBERS; idx++)
		{
			if (ble_mgr_gatt_client_event_cb[idx] != NULL)
			{
				const ble_event_callback_t *event_cb_fn = ble_mgr_gatt_client_event_cb[idx];
				if(event_cb_fn[events] != NULL)
				{
					event_cb_fn[events](event_params);
				}
			}
		}
	}
	break;
	
	case AT_BLE_NOTIFICATION_CONFIRMED:
	case AT_BLE_INDICATION_CONFIRMED:
	case AT_BLE_CHARACTERISTIC_CHANGED:
	case AT_BLE_CHARACTERISTIC_CONFIGURATION_CHANGED:
	case AT_BLE_SERVICE_CHANGED_INDICATION_SENT:
	case AT_BLE_WRITE_AUTHORIZE_REQUEST:
	case AT_BLE_MTU_CHANGED_INDICATION:
	case AT_BLE_MTU_CHANGED_CMD_COMPLETE:
	case AT_BLE_CHARACTERISTIC_WRITE_CMD_CMP:
	case AT_BLE_READ_AUTHORIZE_REQUEST:
	{
		uint8_t idx;
		events -= (AT_BLE_INDICATION_RECIEVED + 1);
		DBG_LOG_DEV("GATT_Server Event:%d", events);
		for (idx = 0; idx < MAX_GATT_SERVER_SUBSCRIBERS; idx++)
		{
			if (ble_mgr_gatt_server_event_cb[idx] != NULL)
			{
				const ble_event_callback_t *event_cb_fn = ble_mgr_gatt_server_event_cb[idx];
				if(event_cb_fn[events] != NULL)
				{
					event_cb_fn[events](event_params);
				}
			}
		}
	}
	break;
	
	case AT_BLE_LECB_CONN_REQ:
	case AT_BLE_LECB_CONNECTED:
	case AT_BLE_LECB_DISCONNECTED:
	case AT_BLE_LECB_ADD_CREDIT_IND:
	case AT_BLE_LECB_SEND_RESP:
	case AT_BLE_LECB_DATA_RECIEVED:
	{
		uint8_t idx;
		events -= (AT_BLE_READ_AUTHORIZE_REQUEST + 1);
		for (idx = 0; idx < MAX_L2CAP_EVENT_SUBSCRIBERS; idx++)
		{
			if (ble_mgr_l2cap_event_cb[idx] != NULL)
			{
				const ble_event_callback_t *event_cb_fn = ble_mgr_l2cap_event_cb[idx];
				if(event_cb_fn[events] != NULL)
				{
					event_cb_fn[events](event_params);
				}
			}
		}
	}
	break;
	
	case AT_BLE_HTPT_CREATE_DB_CFM:
	case AT_BLE_HTPT_ERROR_IND:
	case AT_BLE_HTPT_DISABLE_IND:
	case AT_BLE_HTPT_TEMP_SEND_CFM:
	case AT_BLE_HTPT_MEAS_INTV_CHG_IND:
	case AT_BLE_HTPT_CFG_INDNTF_IND:
	case AT_BLE_HTPT_ENABLE_RSP:
	case AT_BLE_HTPT_MEAS_INTV_UPD_RSP:
	case AT_BLE_HTPT_MEAS_INTV_CHG_REQ:
	{
		uint8_t idx;
		events -= (AT_BLE_LECB_DATA_RECIEVED + 1);
		for (idx = 0; idx < MAX_HTPT_EVENT_SUBSCRIBERS; idx++)
		{
			if (ble_mgr_htpt_event_cb[idx] != NULL)
			{
				const ble_event_callback_t *event_cb_fn = ble_mgr_htpt_event_cb[idx];
				if(event_cb_fn[events] != NULL)
				{
					event_cb_fn[events](event_params);
				}
			}
		}
	}
	break;
	
	case AT_BLE_LE_TEST_STATUS:
	case AT_BLE_LE_PACKET_REPORT:
	{
		uint8_t idx;
		events -= (AT_BLE_HTPT_MEAS_INTV_CHG_REQ + 1);
		for (idx = 0; idx < MAX_DTM_EVENT_SUBSCRIBERS; idx++)
		{
			if (ble_mgr_dtm_event_cb[idx] != NULL)
			{
				const ble_event_callback_t *event_cb_fn = ble_mgr_dtm_event_cb[idx];
				if(event_cb_fn[events] != NULL)
				{
					event_cb_fn[events](event_params);
				}
			}
		}
	}
	break;
	
	case AT_BLE_CUSTOM_EVENT:
	case AT_BLE_DEVICE_READY:
	case AT_BLE_EVENT_MAX:
	{
		uint8_t idx;
		events -= (AT_BLE_LE_PACKET_REPORT + 1);
		for (idx = 0; idx < MAX_CUSTOM_EVENT_SUBSCRIBERS; idx++)
		{
			if (ble_mgr_custom_event_cb[idx] != NULL)
			{
				const ble_event_callback_t *event_cb_fn = ble_mgr_custom_event_cb[idx];
				if(event_cb_fn[events] != NULL)
				{
					event_cb_fn[events](event_params);
				}
			}
		}
	}
	break;
	
	default:
	{
		DBG_LOG_DEV("BLE-Manager:Unknown Event=0x%X", events);
		DBG_LOG("\r\n");
	}
	break;		
	}
}



