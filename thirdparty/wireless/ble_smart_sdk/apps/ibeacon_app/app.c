#include "at_ble_api.h"
#include <string.h>
#ifndef NULL
#define NULL ((void*)0)
#endif

#define BEACON_IDENTIFIER (0x47)
static uint8_t adv_data[31];
static uint8_t scan_rsp_data[31];	


void app_init(void)
{
	 uint16_t status = AT_BLE_FAILURE; 
	 at_ble_addr_t addr = {AT_BLE_ADDRESS_PUBLIC,
		{0x45, 0x75, 0x11, 0x6a, 0x7f, 0x7f} };
	 	 
		const uint8_t ro_adv_data[] = {0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15, 0x21, 0x8A,
															 0xF6, 0x52, 0x73, 0xE3, 0x40, 0xB3, 0xB4, 0x1C,
															 0x19, 0x53, 0x24, 0x2C, 0x72, 0xf4, 0x00, 0xbb,
																0x00, 0x45, 0xc5};

    const uint8_t ro_scan_rsp_data[] = {0x11, 0x07, 0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00,
																		0x37, 0xaa, 0xe3, 0x11, 0x2a, 0xdc, 0x00, 0xcd,
																		0x30, 0x57};
		
		addr.addr[0] = BEACON_IDENTIFIER;
		
		memset(adv_data,0,sizeof(adv_data));
		memset(scan_rsp_data,0,sizeof(scan_rsp_data));
		memcpy(adv_data,ro_adv_data,sizeof(ro_adv_data));
		memcpy(scan_rsp_data,ro_scan_rsp_data,sizeof(ro_scan_rsp_data));

		// init device
		if((status = at_ble_init(NULL)) == AT_BLE_SUCCESS)
		{
		    status = AT_BLE_FAILURE;
  			// Set the device address
		    status = at_ble_addr_set(&addr);
	       // start advertising
		    adv_data[25] = BEACON_IDENTIFIER;
		    at_ble_adv_data_set((uint8_t *)adv_data, sizeof(ro_adv_data), (uint8_t *)scan_rsp_data, sizeof(ro_scan_rsp_data));
		    at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, 100, 1000, 0);
		}

 }
		
int app_entry(void);
//main_task_id = 0x63;
//unsigned int osAppTask_var __attribute__((at(0x100000D0))) = (unsigned int)app_entry;
//unsigned int main_task_id __attribute__((at(0x100000C8))) = 0x3E;

static uint8_t params[100];
int app_entry()
{
	at_ble_events_t event;
	uint16_t handle;
	memset(params,0,sizeof(params));
	app_init();
	
	while(at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
	{
		switch(event)
		{
			case AT_BLE_CONNECTED:
			{
				at_ble_connected_t* conn_params = (at_ble_connected_t*)params;
				handle = conn_params->handle;
				handle = handle;
			}
			break;

			case AT_BLE_DISCONNECTED:
			{
				at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED,AT_BLE_ADV_GEN_DISCOVERABLE,NULL, AT_BLE_ADV_FP_ANY, 100, 1000, 0);
			}
			break;

			default:
			break;

		}
	}
	return 0;
}	


