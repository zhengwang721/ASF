
#include "ble_manager.h"

#if (BLE_DEVICE_ROLE == BLE_CENTRAL)
uint8_t scan_response_count = 0;
at_ble_scan_info_t scan_info[MAX_SCAN_DEVICE];
#endif

at_ble_connected_t ble_connected_dev_info[MAX_DEVICE_CONNECTED];

static void ble_init(void);
static void ble_set_address(at_ble_addr_t *addr);

at_ble_events_t event;
uint8_t params[AT_BLE_MAX_ATT_LEN];

at_ble_status_t ble_event_task(void)
{
	if (platform_ble_event_data() == AT_BLE_SUCCESS)
	{
		if(at_ble_event_get(&event, params, AT_BLE_EVENT_TIMEOUT) == AT_BLE_SUCCESS)
		{
			ble_event_manager(event, params);
			return AT_BLE_SUCCESS;
		}
	}
	
	return AT_BLE_FAILURE;
}

void ble_device_init(at_ble_addr_t *addr)
{
	ble_init();
	ble_set_address(addr);
	BLE_PROFILE_INIT();
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
		if(BLE_SCAN_DATA_HANDLER(scan_info, scan_response_count) == AT_BLE_FAILURE)
		{
			DBG_LOG("No Device found");
		}

	}
	else
	{
		DBG_LOG("Scanning  failed");		
	}
	return AT_BLE_FAILURE;
}

/* Parse the received adverting data for service and local name*/
uint8_t scan_info_parse(at_ble_scan_info_t *scan_info_data, at_ble_uuid_t *ble_service_uuid, uint8_t adv_type)
{
	if (scan_info_data->adv_data_len)
	{
		uint8_t adv_data_size;
		uint8_t index = 0;
		volatile adv_element_t *adv_element_p;
		volatile adv_element_t adv_element_data;
		
		adv_data_size = scan_info_data->adv_data_len;
		adv_element_data.len = 0;
		while(adv_data_size)
		{	
			adv_element_data.len = scan_info_data->adv_data[index];		
			adv_element_data.type = scan_info_data->adv_data[index+1];
			adv_element_data.data = &scan_info_data->adv_data[index+2];			
			adv_element_p = &adv_element_data;
			
			if (adv_element_p->type == adv_type)
			{
				uint8_t adv_type_size = adv_element_p->len;		// passing the length of data type
				adv_type_size -= 1;								// actual size of the data
				while(adv_type_size)
				{
					volatile int cmp_status = -1;
					if (ble_service_uuid->type == AT_BLE_UUID_16)
					{
						cmp_status = memcmp(adv_element_p->data, ble_service_uuid->uuid, AT_BLE_UUID_16_LEN);
						adv_element_p->data += AT_BLE_UUID_16_LEN;
						adv_type_size -= AT_BLE_UUID_16_LEN;
					}
					else if (ble_service_uuid->type == AT_BLE_UUID_32)
					{
						cmp_status = memcmp(adv_element_p->data, ble_service_uuid->uuid, AT_BLE_UUID_32_LEN);
						adv_element_p->data += AT_BLE_UUID_32_LEN;
						adv_type_size -= AT_BLE_UUID_32_LEN;
					}
					else if (ble_service_uuid->type == AT_BLE_UUID_128)
					{
						cmp_status = memcmp(adv_element_p->data, ble_service_uuid->uuid, AT_BLE_UUID_128_LEN);
						adv_element_p->data += AT_BLE_UUID_128_LEN;
						adv_type_size -= AT_BLE_UUID_32_LEN;
					}
					
					if (cmp_status == 0)
					{
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
#endif

void ble_connected_state_handler(at_ble_connected_t *conn_params)
{
	memcpy(ble_connected_dev_info, (uint8_t *)conn_params, sizeof(at_ble_connected_t));
	DBG_LOG("Device connected to 0x%02x%02x%02x%02x%02x%02x handle=0x%x status=%02X",
			conn_params->peer_addr.addr[5],
			conn_params->peer_addr.addr[4],
			conn_params->peer_addr.addr[3],
			conn_params->peer_addr.addr[2],
			conn_params->peer_addr.addr[1],
			conn_params->peer_addr.addr[0],
			conn_params->handle, 
			conn_params->conn_status);
}

void ble_disconnected_state_handler(at_ble_disconnected_t *disconnect)
{
	DBG_LOG("Device disconnected Reason:0x%02x Handle=0x%x", disconnect->reason, disconnect->handle);
}

void ble_event_manager(at_ble_events_t events, void *event_params)
{
	switch(events)
	{
		case AT_BLE_SCAN_INFO:
		{
			BLE_SCAN_INFO_HANDLER((at_ble_scan_info_t *)event_params);
		}
		break;
		
		case AT_BLE_SCAN_REPORT:
		{
			BLE_SCAN_REPORT_HANDLER((at_ble_scan_report_t *)event_params);
		}
		break;
		
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
		
		case AT_BLE_DISCOVERY_COMPLETE:
		{
			BLE_DISCOVERY_COMPLETE_HANDLER((at_ble_discovery_complete_t *)event_params);
		}
		break;

		case AT_BLE_PRIMARY_SERVICE_FOUND:
		{
			BLE_PRIMARY_SERVICE_FOUND_HANDLER((at_ble_primary_service_found_t *)event_params);
		}
		break;
		
		case AT_BLE_CHARACTERISTIC_FOUND:
		{
			BLE_CHARACTERISTIC_FOUND_HANDLER((at_ble_characteristic_found_t *)event_params);
		}
		break;
		
		case AT_BLE_CHARACTERISTIC_READ_RESPONSE:
		{
			BLE_CHARACTERISTIC_READ_RESPONSE((at_ble_characteristic_read_response_t *)event_params);
		}
		break;
		
		case AT_BLE_CHARACTERISTIC_WRITE_RESPONSE :
		{
			BLE_CHARACTERISTIC_WRITE_RESPONSE((at_ble_characteristic_write_response_t *)event_params);
		}
		break;
		
		default:
		{
			DBG_LOG("Unknown Event: event=0x%X", events);
		}
		break;
	}
}

