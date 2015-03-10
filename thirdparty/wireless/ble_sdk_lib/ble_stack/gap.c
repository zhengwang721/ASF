
#include "at_ble_api.h"

#include "platform.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "dbg_task.h"

#include "device.h"
#include "event.h"

// Slave preferred Connection interval Min
#define APP_CON_INTV_MIN	8
// Slave preferred Connection interval Max
#define APP_CON_INTV_MAX	10
// Slave preferred Connection latency
#define APP_CON_LATENCY		0
// Slave preferred Link supervision timeout
#define APP_SUPERV_TO		200 // 2s (500*10ms)

#define APP_MAX_MTU			L2C_MIN_LE_MTUSIG
#define APP_FLAGS			0

#define LOCAL_ADDR_TYPE GAPM_PUBLIC_ADDR
#define GAP_device_name "ATMEL_BLE"
static uint8_t IRK_KEY[KEY_LEN] = {0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01};
static uint8_t rand_addr[BD_ADDR_LEN]= { 0x00,0x00,0x00,0x00,0x00,0x00};

struct device_info device;


at_ble_status_t at_ble_init(void* args)
{
	at_ble_status_t  status;

	status = platform_init(args);

	if(status == AT_BLE_SUCCESS)
	{
		
		event_init();
		if(gapm_reset_req_handler() != ATT_ERR_NO_ERROR)
		{
			status = AT_BLE_FAILURE;
		}
	}

	// init device info
	if(status == AT_BLE_SUCCESS)
	{
		device.srLen = 0;
		device.advLen = 0;
		device.role = ROLE_SLAVE;
		device.at_addr_set = false;
		at_ble_device_name_set((uint8_t *)GAP_device_name,sizeof(GAP_device_name));
		
	}
	return status;
}
at_ble_status_t at_ble_device_name_set(uint8_t* dev_name, uint8_t len)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	do
	{
		if(dev_name == NULL)
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}
		
		if(gapm_set_dev_name_handler (len,dev_name) != ATT_ERR_NO_ERROR)
		{
			status = AT_BLE_FAILURE;
		}
	}while(0);

	return status;
}

// To do support random address
at_ble_status_t at_ble_addr_set(at_ble_addr_t* address)
{
	uint8_t u8Status;
	at_ble_status_t status = AT_BLE_SUCCESS;

	do
	{
		if((address->addr == NULL) || 
			(address->type >AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE))
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}
		if(address->type == AT_BLE_ADDRESS_PUBLIC)
		{
			u8Status = dbg_le_set_bd_addr_req_handler(address->addr);

			if(u8Status!=AT_BLE_SUCCESS)
			{
				status = AT_BLE_FAILURE;
				break;
			}
			device.at_addr_set = true;
			device.at_dev_addr.type = address->type;
			memcpy(device.at_dev_addr.addr, address->addr ,AT_BLE_ADDR_LEN);
		}
	}while(0);
	return status;
}

at_ble_status_t at_ble_addr_get(at_ble_addr_t* address)
{
	#define AT_BT_DEFAULT_BDADDR "\x01\x23\x45\x67\x89\xAB"

	if(device.at_addr_set == true)
	{
		address->type = device.at_dev_addr.type;
		memcpy(address->addr, device.at_dev_addr.addr,AT_BLE_ADDR_LEN);
	}
	else
	{
		// To do check device default address
		address->type = AT_BLE_ADDRESS_PUBLIC;
		memcpy(address->addr,(uint8_t *)AT_BT_DEFAULT_BDADDR,AT_BLE_ADDR_LEN);
	}
	
	return AT_BLE_SUCCESS;
}

at_ble_status_t at_ble_adv_data_set( uint8_t const *const adv_data,
	uint8_t adv_data_len,uint8_t const *const scan_resp_data,
	uint8_t scan_response_data_len)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	do
	{
		if((adv_data== NULL) && (scan_resp_data == NULL))
		{
			status = AT_BLE_FAILURE;  
			break;
		}
		if((adv_data_len > AT_BLE_ADV_MAX_SIZE) || 
			(scan_response_data_len > AT_BLE_ADV_MAX_SIZE))
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}

	    if((adv_data != NULL) && (adv_data_len == 0) )
	    {
	       	// Clear advertising data
			memset(device.ADVData,0, AT_BLE_ADV_MAX_SIZE);
			device.advLen =  0;
	    }
		else if((adv_data != NULL) && (adv_data_len > 0))
		{
			// To do add advertising data check
			memcpy(device.ADVData,adv_data, adv_data_len);
			device.advLen =  adv_data_len;
		}

		if(scan_resp_data != NULL && scan_response_data_len == 0 )
	    {
	       	// Clear scan response data
			memset(device.SrData,0, AT_BLE_ADV_MAX_SIZE);
			device.srLen = 0;
	    }
		else if((scan_resp_data != NULL) && (scan_response_data_len > 0))
		{
			// To do add scan response data check
		    memcpy(device.SrData, scan_resp_data,scan_response_data_len);
			device.srLen = scan_response_data_len;
		}
	}while(0);
	return status;
}

at_ble_status_t at_ble_adv_start(at_ble_adv_type_t type,at_ble_adv_mode_t mode,
				at_ble_addr_t* peer_addr,at_ble_filter_type_t filtered, 
				uint16_t interval, uint16_t timeout, bool disable_randomness)
{
	uint8_t u8Status,adv_type;
	at_ble_addr_t local_addr;
	uint8_t scan_rsp_len = device.srLen;

	at_ble_status_t status = AT_BLE_SUCCESS;

	// To do add timeout value for GAP
	do
	{
		//If the advertising event type is either a discoverable undirected
		//event type or a non-connectable undirected event type, the advInterval shall
		//not be less than 100 ms. If the advertising event type is a connectable undirected
		//event type, the advInterval can be 20 ms or greater.
		if(((type == AT_BLE_ADV_TYPE_DIRECTED)&&(timeout != 0))
		||(type > AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED)
		||(interval > AT_BLE_ADV_INTERVAL_MAX)
		||((type == AT_BLE_ADV_TYPE_UNDIRECTED)&&(interval < AT_BLE_ADV_INTERVAL_MIN)) 
		||((interval < AT_BLE_ADV_NONCON_INTERVAL_MIN) && 
		((type >= AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED)))
		||((type == AT_BLE_ADV_TYPE_DIRECTED)&&(peer_addr == NULL)) )
		{
			 status = AT_BLE_INVALID_PARAM;
			 break;
		};

		status = at_ble_addr_get(&local_addr);
		if(status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		// To do check privacy setting and IRK key
		u8Status = gapm_set_dev_config_cmd_handler(GAP_PERIPHERAL_SLV, IRK_KEY, 
				0, GAPM_WRITE_DISABLE, GAPM_WRITE_DISABLE, APP_MAX_MTU,
				APP_CON_INTV_MIN, APP_CON_INTV_MAX, APP_CON_LATENCY, 
				APP_SUPERV_TO, APP_FLAGS);
	
		if(u8Status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		switch(type)
		{
			default:
			case  AT_BLE_ADV_TYPE_UNDIRECTED :
				adv_type = GAPM_ADV_UNDIRECT;
				break;
			case  AT_BLE_ADV_TYPE_DIRECTED:     
				adv_type = GAPM_ADV_DIRECT;
				break;
			/* differentiate between them by using scan response length*/
			case AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED:
				scan_rsp_len = 0;
			case AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED:     
				adv_type = GAPM_ADV_NON_CONN;
				break;
		}

		gapm_start_adv_cmd_handler(adv_type, local_addr.type,
			GAP_TMR_PRIV_ADDR_INT, (peer_addr==NULL ? NULL : peer_addr->addr),
			interval, interval, 7, mode, filtered,
			device.advLen, device.ADVData, scan_rsp_len, device.SrData);
		
	}while(0);
	return status;
}

at_ble_status_t at_ble_adv_stop(void)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	if(gapm_cancel_cmd_handler())
	{
		status = AT_BLE_FAILURE; 
	}
	return status;
}

at_ble_status_t at_ble_adv_set_tx_power(int8_t power)
{
	// To do check ble max connection number
	at_ble_status_t status = at_ble_tx_power_set(8,power);
	return status;
}
at_ble_status_t at_ble_scan_start(uint16_t interval,uint16_t window,uint16_t timeout,
	at_ble_scan_type_t type ,at_ble_scan_mode_t mode, bool filter_whitelist, 
	bool filter_dublicates)
{
	uint8_t scan_type,u8Status;
	at_ble_addr_t local_addr;
	at_ble_status_t status = AT_BLE_SUCCESS;

	// To do support scan timeout
	do
	{
	    // if the scanning window is smaller or equal to the scanning interval
		if ((mode > AT_BLE_SCAN_OBSERVER_MODE)
		||(window > interval)
		||(type > AT_BLE_SCAN_ACTIVE)
		||(window > AT_BLE_SCAN_WINDOW_MAX)
		||(window < AT_BLE_SCAN_WINDOW_MIN)
		||(interval > AT_BLE_SCAN_INTERVAL_MAX)
		||(interval < AT_BLE_SCAN_INTERVAL_MIN))
		{
			 status = AT_BLE_INVALID_PARAM;
			 break;
		};
		
		status = at_ble_addr_get(&local_addr);
		if(status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		u8Status = gapm_set_dev_config_cmd_handler(GAP_CENTRAL_MST, IRK_KEY,768,
			GAPM_WRITE_DISABLE, GAPM_WRITE_DISABLE, APP_MAX_MTU, 0, 0, 0, 0, 0);

		if(u8Status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}
		
		if(type == AT_BLE_SCAN_PASSIVE)
		{
			scan_type = GAPM_SCAN_PASSIVE; 
		}
		else
		{
			scan_type = GAPM_SCAN_ACTIVE;
		}

		gapm_start_scan_cmd_handler(scan_type, local_addr.type, 
			GAP_TMR_PRIV_ADDR_INT,
			local_addr.addr, interval, window, mode, 
			filter_whitelist, filter_dublicates);

	}while(0);
	return status;
}

at_ble_status_t at_ble_scan_stop(void)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	if(gapm_cancel_cmd_handler())
	{
		status = AT_BLE_FAILURE; 
	}
	return status;
}

at_ble_status_t at_ble_connect(at_ble_addr_t peers[], uint8_t peer_count, 
	uint16_t scan_interval, uint16_t scan_window, at_ble_connection_params_t* connection_params)
{

	at_ble_status_t status = AT_BLE_SUCCESS;
	do
	{
		/* check for the range validity of the values */
		if ( (connection_params->con_intv_max < AT_CNX_INTERVAL_MIN || 
		connection_params->con_intv_max > AT_CNX_INTERVAL_MAX) ||
		(connection_params->con_intv_min < AT_CNX_INTERVAL_MIN ||
		connection_params->con_intv_min > AT_CNX_INTERVAL_MAX) ||
		(connection_params->superv_to  < AT_CNX_SUP_TO_MIN   || 
		connection_params->superv_to  > AT_CNX_SUP_TO_MAX)   ||
		(connection_params->con_latency  > AT_CNX_LATENCY_MAX) )
		{
			status = AT_BLE_INVALID_PARAM;
			break;
		}

		device.role = ROLE_MASTER;

		status = gapm_set_dev_config_cmd_handler(GAP_CENTRAL_MST, IRK_KEY,768,
		GAPM_WRITE_DISABLE, GAPM_WRITE_DISABLE, APP_MAX_MTU, 0, 0, 0, 0, 0);

		/*if(status!=AT_BLE_SUCCESS)
		{
			status = AT_BLE_FAILURE;
			break;
		}*/

		gapm_start_connection_cmd_handler(GAPM_CONNECTION_AUTO, LOCAL_ADDR_TYPE, 
			GAP_TMR_PRIV_ADDR_INT, rand_addr, scan_interval, scan_window,
			connection_params->con_intv_min, connection_params->con_intv_max,
			connection_params->con_latency,
			connection_params->superv_to,
			connection_params->ce_len_min,
			connection_params->ce_len_max,
			peer_count, peers);

	}while(0);
	return status;
}

at_ble_status_t at_ble_connect_cancel(void)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	if(gapm_cancel_cmd_handler())
	{
		status = AT_BLE_FAILURE; 
	}
	return status;
}

at_ble_status_t at_ble_whitelist_add(at_ble_addr_t* address)
{
	if(address->addr == NULL)
	{
		return AT_BLE_FAILURE;
	}

	gapm_white_list_mgm_cmd(GAPM_ADD_DEV_IN_WLIST, 
		address->type, address->addr);

	return AT_BLE_SUCCESS;
}

at_ble_status_t at_ble_whitelist_remove(at_ble_addr_t* address)
{
	if(address->addr == NULL)
	{
		return AT_BLE_FAILURE;
	}

	gapm_white_list_mgm_cmd(GAPM_RMV_DEV_FRM_WLIST, 
		address->type, address->addr);

	return AT_BLE_SUCCESS;
}

at_ble_status_t at_ble_whitelist_clear(void)
{
	gapm_white_list_mgm_cmd(GAPM_CLEAR_WLIST, 
		0, NULL);

	return AT_BLE_SUCCESS;
}

at_ble_status_t at_ble_disconnect(at_ble_handle_t handle, at_ble_disconnect_reason_t reason)
{
	uint8_t gapc_reason ;
	switch(reason)
	{
		case AT_BLE_TERMINATED_BY_USER:
			gapc_reason = 0x13;
			break;
		case AT_BLE_UNACCEPTABLE_INTERVAL:
			gapc_reason = 0x3b;
			break;
		default:
			gapc_reason = 0x1F;
	}
	gapc_disconnect_cmd_handler(gapc_reason, handle);
	
	return AT_BLE_SUCCESS;
}


at_ble_status_t at_ble_connection_param_update(at_ble_handle_t handle, 
	at_ble_connection_params_t* connection_params)
{
	/* check for the range validity of the values */
	if ( (connection_params->con_intv_max < AT_CNX_INTERVAL_MIN || 
	connection_params->con_intv_max > AT_CNX_INTERVAL_MAX) ||
	(connection_params->con_intv_min < AT_CNX_INTERVAL_MIN ||
	connection_params->con_intv_min > AT_CNX_INTERVAL_MAX) ||
	(connection_params->superv_to  < AT_CNX_SUP_TO_MIN   || 
	connection_params->superv_to  > AT_CNX_SUP_TO_MAX)   ||
	(connection_params->con_latency  > AT_CNX_LATENCY_MAX) )
	{
		return AT_BLE_INVALID_PARAM;
	}

	gapc_param_update_cmd_handler(handle,
		connection_params->con_intv_min, connection_params->con_intv_max,
		connection_params->con_latency, connection_params->superv_to, 
		connection_params->ce_len_min, connection_params->ce_len_max);

	return AT_BLE_SUCCESS;
}

// To do check parameters
void at_ble_conn_update_reply(at_ble_handle_t conn_handle , 
				at_ble_connection_params_t* connection_params)
{
	gapc_param_update_cfm_handler(conn_handle, 
		connection_params->ce_len_min, connection_params->ce_len_max);
}

at_ble_status_t at_ble_tx_power_set(at_ble_handle_t conn_handle, int8_t power)
{
	uint8_t level;
	/// Power table
	static const int8_t RF_RPL_TX_PW_CONV_TBL[RPL_PWR_TBL_SIZE] 
			= {-23,-20,-17,-14,-11,-8,-5,-2};


	for(level = RPL_POWER_MIN; level <= RPL_POWER_MAX; level++)
	{
		if(RF_RPL_TX_PW_CONV_TBL[level] >= power)
			break;
	}

	if(level > RPL_POWER_MAX)
	{
		level = RPL_POWER_MAX;
	}

	if(dbg_set_tx_pw_req_handler(conn_handle, level) != AT_BLE_SUCCESS)
	{
		return AT_BLE_FAILURE;
	}
	else
	{
		return AT_BLE_SUCCESS;
	}
}


