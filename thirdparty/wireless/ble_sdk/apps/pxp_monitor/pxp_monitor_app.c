/**
* \file
*
* \brief Health Thermometer Profile Application
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
*    Atmel micro controller product.
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
* This is the reference manual for the Health Thermometer Profile Application
*/
/*- Includes ---------------------------------------------------------------*/

#define DEBUG_LOG

#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "pxp_monitor.h"
#include "pxp_monitor_app.h"
#include "profiles.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_extint.h"

at_ble_LTK_t app_bond_info;
bool app_device_bond = false;
uint8_t auth_info = 0;
uint8_t count1, serv_avail_count, device_found_id;
at_ble_scan_info_t scan_info[10];
//pxp_peripheral pxp_reporter;
at_ble_primary_service_found_t service_avail[6];



void app_init(void)
{
	uint8_t port = 74;
	at_ble_addr_t addr;
	at_ble_status_t scan_status;
	
	// init device reset the link layer
	at_ble_init(&port);
	
	
	at_ble_addr_get(&addr);
	
	at_ble_addr_set(&addr);
	
	/* Initialize the scanning procedure */
	scan_status = pxp_scan_start();
	
	if(scan_status == AT_BLE_SUCCESS)
	{
		DBG_LOG("\r\nScanning process initiated ");
	}
	else if(scan_status == AT_BLE_INVALID_PARAM)
	{
		DBG_LOG("\r\nScan parameters are invalid");
	}
	else if(scan_status == AT_BLE_FAILURE)
	{
		DBG_LOG("\r\nScanning Failed Generic error");
	}

}

void pxp_parse(void)
{
	uint8_t flag1,i,j,k;
	for(i=0;i<serv_avail_count;i++)
	{
		
		for(j=0;j<scan_info[i].adv_data_len;j++)
		{
			flag1=scan_info[i].adv_data[j];
			
			if(scan_info[i].adv_data[j+1]==0x09)     // check for local name
			{
				DBG_LOG("Device name is :");
				for(k=1;k<flag1;k++)
				{
					DBG_LOG("%c",scan_info[i].adv_data[j+k+1]);
				}
			}
			
			else if(scan_info[i].adv_data[j+1]==0x03) // check for services
			{
				DBG_LOG("Device supported Services :");
				if((scan_info[i].adv_data[j+2]==0x03)&&(scan_info[i].adv_data[j+3]==0x18))
				{
					device_found_id=i;
				}
				for(k=1;k<flag1;k++)
				{
					DBG_LOG("%02X",scan_info[i].adv_data[j+k+1]);
				}
			}
			
			else
			{
				for(k=0;k<flag1;k++)
				{
					//DBG_LOG("%02X ",scan_info[i].adv_data[j+k+1]);
				}
			}
			
			j+=k;
			DBG_LOG("\r\n");
		}
		
		DBG_LOG("\r\nDevice Address 0x%02x%02x%02x%02x%02x%02x ",
		scan_info[i].dev_addr.addr[5],
		scan_info[i].dev_addr.addr[4],
		scan_info[i].dev_addr.addr[3],
		scan_info[i].dev_addr.addr[2],
		scan_info[i].dev_addr.addr[1],
		scan_info[i].dev_addr.addr[0]);
	}
}

int main (void)
{
	at_ble_events_t event;
	uint8_t params[512];
	at_ble_addr_t pxp_peripheral[2];
	at_ble_connection_params_t pxp_parameter;
	//at_ble_addr_t rem_dev = {0x00,0x03,0x18,0xf0,0x05,0xf0,0xf8};
	//at_ble_addr_t rem_dev = {.type=0x00, .addr[6] = {0x03,0xa4,0x1a,0x38,0xdb,0xd5}};
		
	at_ble_handle_t handle = 0;
	at_ble_characteristic_found_t charcter_avail[20];
	
	pxp_monitor_discover_t discover_role;
	discover_role = DISCOVER_SERVICE;

	#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	#elif SAM0
	system_init();
	#endif
	
	hw_timer_init();
	hw_timer_register_callback(timer_callback_handler);
	
	//button_init();
	serial_console_init();
	
	DBG_LOG_1LVL("\r\nInitializing Proximity Application");

	app_init();
	serv_avail_count=0;
	count1=0;
	
	while(at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
	{
		switch(event)
		{
			case AT_BLE_SCAN_INFO:
			{
				memcpy((uint8_t *)&scan_info[serv_avail_count], params, sizeof(at_ble_scan_info_t));
				
				//uint8_t device_name_poin,service_name_poin,counter;
				//uint8_t temp_buf,device_name_len,service_name_len;
				//
				//DBG_LOG("\r\n ");
				//
				//for(counter=0;counter<scan_info[serv_avail_count].adv_data_len;counter++)
				//{
					//DBG_LOG("%02X ",scan_info[serv_avail_count].adv_data[counter]);
				//}
				//
				//DBG_LOG("\r\n ");
				//
				//for(counter=0;counter<scan_info[serv_avail_count].adv_data_len;counter++)
				//{
					//
					//temp_buf = scan_info[serv_avail_count].adv_data[counter];
					//DBG_LOG("%02X  ",temp_buf);
					//if(scan_info[serv_avail_count].adv_data[counter+1]==0x09)
					//{
						//device_name_poin = counter+2;
						//device_name_len = temp_buf-1;
						//DBG_LOG("%02X ",device_name_poin);
						//DBG_LOG("%02X ",device_name_len);
					//}
					//if(scan_info[serv_avail_count].adv_data[counter+1]==0x03)
					//{
						//service_name_poin = counter+2;
						//service_name_len = temp_buf-1;
					//}
					//counter=counter+temp_buf;
				//}
				//
				//for(counter=0;counter<device_name_len;counter++)
				//{
					//DBG_LOG("%c",scan_info[serv_avail_count].adv_data[device_name_poin]);	
				//}
				//DBG_LOG("\r\n ");
				//for(counter=0;counter<service_name_len;counter++)
				//{
					//DBG_LOG("%c",scan_info[serv_avail_count].adv_data[service_name_poin]);
				//}
				
				{
					DBG_LOG("\r\nDevice found Address 0x%02X%02X%02X%02X%02X%02X adv len %02d",
					scan_info[serv_avail_count].dev_addr.addr[5],
					scan_info[serv_avail_count].dev_addr.addr[4],
					scan_info[serv_avail_count].dev_addr.addr[3],
					scan_info[serv_avail_count].dev_addr.addr[2],
					scan_info[serv_avail_count].dev_addr.addr[1],
					scan_info[serv_avail_count].dev_addr.addr[0],
					scan_info[serv_avail_count].adv_data_len);
				}
				
				
				serv_avail_count++;
			}
			break;
			
			
			case AT_BLE_SCAN_REPORT:
			{
				at_ble_status_t connect_status;
				at_ble_scan_report_t scan_report;
				
				memcpy((uint8_t *)&scan_report, params, sizeof(at_ble_scan_report_t));
				
				pxp_parse();
				if(device_found_id)
				{
					DBG_LOG("\r\nSupported Address is 0x%02x%02x%02x%02x%02x%02x Type %d length %d\r\n",
					scan_info[device_found_id].dev_addr.addr[5],
					scan_info[device_found_id].dev_addr.addr[4],
					scan_info[device_found_id].dev_addr.addr[3],
					scan_info[device_found_id].dev_addr.addr[2],
					scan_info[device_found_id].dev_addr.addr[1],
					scan_info[device_found_id].dev_addr.addr[0],
					scan_info[device_found_id].type,
					scan_info[device_found_id].adv_data_len);
				}
				
				if(scan_report.status==AT_BLE_SUCCESS)
				{
					at_ble_scan_stop();   // Stop the current scan active
					
					DBG_LOG("\r\nScan Process Completed ");
					
					pxp_parameter.con_intv_min = PXP_CONN_MIN;
					pxp_parameter.con_intv_max = PXP_CONN_MAX;
					pxp_parameter.con_latency = PXP_CONN_LATENCY;
					pxp_parameter.ce_len_min = PXP_CE_LEN_MIN;
					pxp_parameter.ce_len_max = PXP_CE_LEN_MAX;
					pxp_parameter.superv_to = PXP_SUPER_TIMOUT;//500
					
					memcpy((uint8_t *)&pxp_peripheral[0], &scan_info[device_found_id].dev_addr,sizeof(at_ble_addr_t));
					
					connect_status=at_ble_connect(pxp_peripheral, 1 , SCAN_INTERVAL, SCAN_WINDOW, &pxp_parameter);

					//memcpy((uint8_t *)&pxp_peripheral[0], &rem_dev,sizeof(at_ble_addr_t));
					
					//connect_status = pxp_dev_connect(scan_info[device_found_id].dev_addr);
					
					
					
					if(connect_status == AT_BLE_SUCCESS)
					{
						DBG_LOG("\r\nConnection request Send ");
					}
					else if(connect_status == AT_BLE_INVALID_PARAM)
					{
						DBG_LOG("\r\nConnect request failed invalid parameter");
					}
					else if(connect_status == AT_BLE_FAILURE)
					{
						DBG_LOG("\r\nConnect request failed");
					}
					DBG_LOG("\r\n");
				}
				
				else
				{
					DBG_LOG("\r\n Scanning  has insufficient data, type is %02x",scan_report.status);
					while(1);
				}
			}
			break;
			
			case AT_BLE_CONNECTED:
			{
				at_ble_connected_t conn_params;
				at_ble_status_t connection_status;
				
				memcpy((uint8_t *)&conn_params, params, sizeof(at_ble_connected_t));
				
				DBG_LOG("\r\nDevice connected to 0x%02x%02x%02x%02x%02x%02x handle=0x%x status=%02X",
				conn_params.peer_addr.addr[5],
				conn_params.peer_addr.addr[4],
				conn_params.peer_addr.addr[3],
				conn_params.peer_addr.addr[2],
				conn_params.peer_addr.addr[1],
				conn_params.peer_addr.addr[0],
				conn_params.handle,
				conn_params.conn_status);
				handle = conn_params.handle;
				//pxp_reporter.handle;
				
				LED_On(LED0);
				if(conn_params.conn_status==AT_BLE_SUCCESS)
				{
					connection_status = at_ble_primary_service_discover_all( handle, PXP_STARTING_HANDLE, PXP_ENDING_HANDLE);
					DBG_LOG("\r\n");
					serv_avail_count=0; // make service discover counter to zero
					
					if(connection_status==AT_BLE_SUCCESS)
					{
						DBG_LOG("\r\n GATT Discovery request started ");
					}
					else
					{
						DBG_LOG("\r\n GATT Discovery request failed");
					}
				}
			}
			break;

			case AT_BLE_DISCONNECTED:
			{
				at_ble_disconnected_t disconnect;
				memcpy((uint8_t *)&disconnect, params, sizeof(at_ble_disconnected_t));
				LED_Off(LED0);
				DBG_LOG("\r\nDevice disconnected Reason:0x%02x Handle=0x%x", disconnect.reason, disconnect.handle);
			}
			break;
			
			case AT_BLE_DISCOVERY_COMPLETE:
			{
				at_ble_discovery_complete_t discover_status;
				memcpy((uint8_t *)&discover_status, params, sizeof(at_ble_discovery_complete_t));
				
				#ifdef DEBUG_LOG
				DBG_LOG("\r\nDiscovery Complete  DiscoveryStatus 0x%02x DiscoveryRole 0x%02x",discover_status.status,discover_role);
				#endif
				
				count1++;
				if(discover_status.status == AT_BLE_DISCOVERY_SUCCESS)
				{
					
					if(discover_role == DISCOVER_SERVICE)
					{
						switch(count1)
						{
							case 01:
							{
								if(at_ble_characteristic_discover_all( handle, tx_handler.srt_handle, tx_handler.end_handle) == AT_BLE_SUCCESS)
								{
									#ifdef DEBUG_LOG
									DBG_LOG("\r\nTx Characteristic Discovery Started");
									#endif
								}
								else
								{
									#ifdef DEBUG_LOG
									DBG_LOG("\r\nTx Characteristic Discovery Failed");
									#endif
								}
							}
							break;
							
							case 02:
							{
								if(at_ble_characteristic_discover_all( handle, immed_alert_handler.srt_handle, immed_alert_handler.end_handle) == AT_BLE_SUCCESS)
								{
									#ifdef DEBUG_LOG
									DBG_LOG("\r\n Immediate Characteristic Discovery Started");
									#endif
								}
								else
								{
									#ifdef DEBUG_LOG
									DBG_LOG("\r\n Immediate Characteristic Discovery Failed");
									#endif
								}
							}
							break;
							
							case 03:
							{
								if(at_ble_characteristic_discover_all( handle, linkloss_handler.srt_handle, linkloss_handler.end_handle) == AT_BLE_SUCCESS)
								{
									#ifdef DEBUG_LOG
									DBG_LOG("\r\n Link Loss Characteristic Discovery Started");
									#endif
								}
								else
								{
									#ifdef DEBUG_LOG
									DBG_LOG("\r\n Link Loss Characteristic Discovery Failed");
									#endif
								}
								discover_role = GATT_DISC_COMPLETE;
							}
							break;
						}
						
						
					}
					
					else if(discover_role == GATT_DISC_COMPLETE)
					{
						
						discover_role = DISCOVER_SERVICE;
						DBG_LOG("\r\nGATT Discovery Completed");
						
						// GATT discovery completed Read the TX Power from peripheral
						if(at_ble_characteristic_read(handle, tx_handler.val_handle, 0, 0x01) == AT_BLE_SUCCESS)
						{
							#ifdef DEBUG_LOG
							DBG_LOG("\r\nCharacteristic Read Request successfully");
							#endif
							serv_avail_count=0;
						}
						else
						{
							#ifdef DEBUG_LOG
							DBG_LOG("\r\nCharacteristic Read Request failed");
							#endif
						}
					}
				}
				
			}
			break;

			case AT_BLE_PRIMARY_SERVICE_FOUND:
			{
				
				memcpy((uint8_t *)&service_avail[serv_avail_count], params, sizeof(at_ble_primary_service_found_t));
				
				if((service_avail[serv_avail_count].service_uuid.uuid[0]==0x04)&&(service_avail[serv_avail_count].service_uuid.uuid[1]==0x18)) // for Tx power service
				{
					tx_handler.srt_handle = service_avail[serv_avail_count].start_handle;
					tx_handler.end_handle = service_avail[serv_avail_count].end_handle;
					DBG_LOG("\r\n Tx Power Service found");
					#ifdef DEBUG_LOG
					DBG_LOG("\r\n Tx power service handler  %04X %04X",tx_handler.srt_handle,tx_handler.end_handle);
					#endif
				}
				
				else if((service_avail[serv_avail_count].service_uuid.uuid[0]==0x03)&&(service_avail[serv_avail_count].service_uuid.uuid[1]==0x18)) // for link loss service
				{
					linkloss_handler.srt_handle = service_avail[serv_avail_count].start_handle;
					linkloss_handler.end_handle = service_avail[serv_avail_count].end_handle;
					DBG_LOG("\r\n Link Loss Service found");
					#ifdef DEBUG_LOG
					DBG_LOG("\r\n link loss service discovered  %04X %04X",linkloss_handler.srt_handle,linkloss_handler.end_handle);
					#endif
				}
				
				
				else if((service_avail[serv_avail_count].service_uuid.uuid[0]==0x02)&&(service_avail[serv_avail_count].service_uuid.uuid[1]==0x18)) // for Immediate alert service
				{
					immed_alert_handler.srt_handle = service_avail[serv_avail_count].start_handle;
					immed_alert_handler.end_handle = service_avail[serv_avail_count].end_handle;
					DBG_LOG("\r\n Immediate Alert Service found");
					#ifdef DEBUG_LOG
					DBG_LOG("\r\n Immediate Alert service discovered %04X %04X ",immed_alert_handler.srt_handle,immed_alert_handler.end_handle);
					#endif
				}
				
				else
				{
					#ifdef DEBUG_LOG
					DBG_LOG("\r\n handler %02x Service available handler  %04X %04X ",
					service_avail[serv_avail_count].conn_handle, service_avail[serv_avail_count].start_handle,service_avail[serv_avail_count].end_handle);
					for(uint8_t n=0;n<sizeof(service_avail[serv_avail_count].service_uuid.uuid);n++)
					{
						DBG_LOG("%02X",service_avail[serv_avail_count].service_uuid.uuid[n]);
					}
					#endif
				}
				
				serv_avail_count++;
			}
			break;
			
			case AT_BLE_CHARACTERISTIC_FOUND:
			{
				memcpy((uint8_t *)&charcter_avail[serv_avail_count], params, sizeof(at_ble_characteristic_found_t));
				
				if((charcter_avail[serv_avail_count].char_uuid.uuid[0]==0x07)&&(charcter_avail[serv_avail_count].char_uuid.uuid[1]==0x2a))
				{
					tx_handler.val_handle = charcter_avail[serv_avail_count].value_handle;
					#ifdef DEBUG_LOG
					DBG_LOG("\r\n Tx power characteristics %04X",tx_handler.val_handle);
					#endif
				}
				
				else if((charcter_avail[serv_avail_count].char_uuid.uuid[0]==0x06)&&(charcter_avail[serv_avail_count].char_uuid.uuid[1]==0x2a))
				{
					if((charcter_avail[serv_avail_count].char_handle>linkloss_handler.srt_handle)&&(charcter_avail[serv_avail_count].char_handle<linkloss_handler.end_handle))
					{
						linkloss_handler.val_handle = charcter_avail[serv_avail_count].value_handle;
						#ifdef DEBUG_LOG
						DBG_LOG("\r\n link loss characteristics %04X",linkloss_handler.val_handle);
						#endif
					}
					else
					{
						immed_alert_handler.val_handle = charcter_avail[serv_avail_count].value_handle;
						#ifdef DEBUG_LOG
						DBG_LOG("\r\n Immediate alert characteristics %04X",immed_alert_handler.val_handle);
						#endif
					}
				}
				
				else
				{
					
					#ifdef DEBUG_LOG
					DBG_LOG("\r\n handler %02x Characteristics available handler  %04X  value %04X ",
					charcter_avail[serv_avail_count].conn_handle, charcter_avail[serv_avail_count].char_handle,charcter_avail[serv_avail_count].value_handle);
					for(uint8_t n=0;n<sizeof(charcter_avail[serv_avail_count].char_uuid.uuid);n++)
					{
						DBG_LOG("%02X",charcter_avail[serv_avail_count].char_uuid.uuid[n]);
					}
					#endif
				}
				
				serv_avail_count++;
			}
			break;
			
			case AT_BLE_CHARACTERISTIC_READ_RESPONSE:
			{
				at_ble_characteristic_read_response_t read_resp;
				//at_ble_status_t temp_status;
				//int8_t rx_power;
				
				memcpy((uint8_t *)&read_resp, params, sizeof(at_ble_characteristic_read_response_t));
				
				#ifdef DEBUG_LOG
				DBG_LOG("\r\n Read response Conn_handle %02X handle %02X length %02X Offset %02X",
				read_resp.conn_handle,read_resp.char_handle,read_resp.char_len,read_resp.char_offset);
				for(uint8_t n=0;n<read_resp.char_len;n++)
				{
					DBG_LOG("%02X",read_resp.char_value[n]);
				}
				#endif
				
				if(read_resp.char_handle == tx_handler.val_handle)
				{
					DBG_LOG("\r\n Tx Power of device is %04d dBm",read_resp.char_value[0]);
				}
				
				// set link loss profile to high alert upon connection
				if(pxp_write(handle,linkloss_handler.val_handle,HIGH_ALERT)==AT_BLE_FAILURE)
				{
					DBG_LOG("\r\n Link Loss write characteristics failed");
				}
			}
			break;
			
			default:
			LED_Off(LED0);
			DBG_LOG("\r\nUnknown event received: event=0x%x", event);
			DBG_LOG("  %d",event);
			break;
		}
	}
}




void timer_callback_handler(void)
{
	int8_t rx_power;
	//at_ble_status_t status_1;
	hw_timer_stop();
	rx_power = at_ble_rx_power_get(0x00);
	DBG_LOG("\r\n Rx power is %04d",rx_power);
	hw_timer_start(0x02);
}
