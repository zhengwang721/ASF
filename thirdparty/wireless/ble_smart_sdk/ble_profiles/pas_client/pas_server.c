#include "at_ble_api.h"
#include "ble_manager.h"
#include "pas_server.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

pas_prf_info_t * pas_server_dataref;
control_point_ntf_callback_t control_point_ntf_cb;

at_ble_characteristic_changed_t change_params;

at_ble_addr_t connected_address;
bool connect_request_flag = false;

uint8_t 	pas_server_conf(pas_prf_info_t * ref)
{
	DBG_LOG("pas_server_conf" );
	
	if( ref )
	{
		pas_server_dataref = ref;
		return 1;
	}
	return 0;
}

void 	pas_server_var_init(void)
{
	DBG_LOG("pas_server_var_init" );
	pas_server_dataref->alert_status_value = 0;
	pas_server_dataref->ringer_control_point_value = 0;
	pas_server_dataref->ringer_status_value = 0;
	pas_server_dataref->serv_handle_info = 0;
	
	control_point_ntf_cb = NULL;
	pas_server_dataref = NULL;

	connect_request_flag = false;
	memset(&connected_address, 0x00, sizeof(at_ble_addr_t));
}

void 	pas_server_init(void *param)
{
	DBG_LOG("pas_server_init : initialize ble probile" );
	pas_serv_init(&pas_server_dataref->alert_status_value, &pas_server_dataref->ringer_status_value, &pas_server_dataref->ringer_control_point_value);

	pas_server_dataref->serv_handle_info = pas_serv_db_reg();
	if( pas_server_dataref->serv_handle_info == -1 )
	{
		DBG_LOG("pas_server_init : failed defined service");
	}
}

at_ble_status_t pas_server_connect_request(at_ble_scan_info_t *scan_buffer)
{
	DBG_LOG("pas_server_connect_request");
	
	memcpy((uint8_t *)&connected_address,(uint8_t *)&scan_buffer->dev_addr,sizeof(at_ble_addr_t));
	connected_address.type = scan_buffer->dev_addr.type;

	DBG_LOG("Info: connectiong address   0x%02X%02X%02X%02X%02X%02X ",
							connected_address.addr[5],
							connected_address.addr[4],
							connected_address.addr[3],
							connected_address.addr[2],
							connected_address.addr[1],
							connected_address.addr[0]);	
	
	if (gap_dev_connect(&connected_address) == AT_BLE_SUCCESS) 
	{
		DBG_LOG("pas_server_connect_request : connect request sent");
		connect_request_flag = true;
		//hw_timer_start(PXP_CONNECT_REQ_INTERVAL);
		return AT_BLE_SUCCESS;
	} 
	else 
	{
		DBG_LOG("pasps Connect request send failed");
	}
	return AT_BLE_FAILURE;
}


at_ble_status_t pas_server_scan_data_handler(at_ble_scan_info_t * scan_buffer, uint8_t scanned_dev_count)
{
	uint8_t supp_scan_index[MAX_SCAN_DEVICE];
	uint8_t scan_index = 0;
	uint8_t index = 0;	

	DBG_LOG("pas_server_scan_data_handler");
	
	if (scanned_dev_count) 
	{		
		at_ble_uuid_t service_uuid;
		
		/* Service type to be searched */
		service_uuid.type = AT_BLE_UUID_16;
		
		/* Service UUID */
		service_uuid.uuid[1] = (PAS_SERVICE_UUID >> 8);
		service_uuid.uuid[0] = (uint8_t)PAS_SERVICE_UUID;

		for (index = 0; index < scanned_dev_count; index++) 
		{			
			/* Display only the connectible devices*/
			if((scan_buffer[index].type == AT_BLE_ADV_TYPE_DIRECTED) || (scan_buffer[index].type == AT_BLE_ADV_TYPE_UNDIRECTED))
			{				
				DBG_LOG("Info: Device found address [%d]  0x%02X%02X%02X%02X%02X%02X ",
						index,
						scan_buffer[index].dev_addr.addr[5],
						scan_buffer[index].dev_addr.addr[4],
						scan_buffer[index].dev_addr.addr[3],
						scan_buffer[index].dev_addr.addr[2],
						scan_buffer[index].dev_addr.addr[1],
						scan_buffer[index].dev_addr.addr[0]);

				if (scan_info_parse(&scan_buffer[index], &service_uuid, 0x03 ) == AT_BLE_SUCCESS) /*condition : 0x03 <AD_TYPE> */
				{
					/* Device Service UUID  matched */
					supp_scan_index[scan_index++] = index;
					DBG_LOG_CONT("---PAS");
				}
				
				//scan_device[scan_device_count++] = index;
			}
		}

		/* Stop the current scan active */
		at_ble_scan_stop();
		
		if (!scan_index) 
		{
			DBG_LOG("pas Profile supported device not found ");
		}
		else/*Updating the index pointer to connect */		
		{  
			/* Successful device found event*/
			//uint8_t deci_index = scan_index;
			/*
			deci_index += '0';
			do
			{
				DBG_LOG("Select Index number to Connect or [s] to scan");
				index = getchar();
				DBG_LOG("%c", index);
			} while (!(((index < (deci_index)) && (index >='0')) || (index == 's')));	
			
			if(index == 's')
			{
				return gap_dev_scan();
			}
			else
			{
				index -= '0';
			*/
				return pas_server_connect_request(&scan_buffer[supp_scan_index[0]]);
			//}			
		}			
	}
	

	/* from no device found event*/
	do
	{
		DBG_LOG("Select [s] to scan again");
		index = getchar();
		DBG_LOG("%c", index);
	} while (!(index == 's')); 
	
	if(index == 's')
	{
		return gap_dev_scan();
	}
	

	return AT_BLE_FAILURE;
}

at_ble_status_t pas_server_disconnect_event_handler(at_ble_disconnected_t * disconnect)
{
	char index_value;

	do
	{
		DBG_LOG("Select [r] to Reconnect or [s] Scan");
		index_value = getchar();
		DBG_LOG("%c", index_value);
	}	while (!((index_value == 'r') || (index_value == 's')));
	
	if(index_value == 'r')
	{
		if (gap_dev_connect(&connected_address) == AT_BLE_SUCCESS)
		{
			DBG_LOG("pasps Re-Connect request sent");		
			connect_request_flag = true;
			//hw_timer_start(PXP_CONNECT_REQ_INTERVAL);
			return AT_BLE_SUCCESS;
		}
		else
		{
			DBG_LOG("pasps Re-Connect request send failed");
		}
	}
	else if(index_value == 's')
	{
		return gap_dev_scan();
	}
	
	return AT_BLE_FAILURE;

}

at_ble_status_t pas_server_char_changed_handler(at_ble_characteristic_changed_t *char_handle)
{
	uint8_t i = 0;
	uint8_t value[2] = {0,0};
		
	memcpy((uint8_t *)&change_params, char_handle, sizeof(at_ble_characteristic_changed_t));
	if( change_params.status != AT_BLE_SUCCESS )
	{
		DBG_LOG( "pas_server_char_changed_handler : failed" );
	}
	
	DBG_LOG("pas_server_char_changed_handler success handle is %d", change_params.char_handle);	
	DBG_LOG("pas_server_char_changed_handler : length is %d", change_params.char_len);
	DBG_LOG("pas_server_char_changed_handler : data");
	for( i=0 ; i< change_params.char_len ; i++)
	{
		//DBG_LOG("%c", change_params.char_new_value[i]);
		DBG_LOG("%d", change_params.char_new_value[i]);
	}
	
	at_ble_characteristic_value_get( change_params.char_handle, value , 0, 2, 2);
	
	DBG_LOG("pas_server_char_changed_handler : db data is 0x%02d 0x%02d", value[0],value[1]);
	if( control_point_ntf_cb )
	{
		
	}		
	
	return AT_BLE_SUCCESS;

}



void 	notify_control_point_handler(control_point_ntf_callback_t con_ntf_cb_fn)
{
	DBG_LOG("notify_control_point_handler");
	control_point_ntf_cb = con_ntf_cb_fn;
}

