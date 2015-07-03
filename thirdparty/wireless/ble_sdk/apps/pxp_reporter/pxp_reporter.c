
#include "pxp_reporter.h"


at_ble_status_t register_linkloss_service(void );
at_ble_status_t register_txpower_service(void );	
at_ble_status_t register_immediate_alert_service(void );
	

		
uint8_t proximity_reporter_init(void );



uint8_t proximity_reporter_init(void )
{
		
	uint8_t idx = 0;
	uint8_t adv_data[ PXP_ADV_DATA_NAME_LEN + LL_ADV_DATA_UUID_LEN   + (2*2)];
	//at_ble_status_t status_t;
	
		
		// registering service link loss service
	
		if (register_linkloss_service() == AT_BLE_SUCCESS) {
			DBG_LOG("Linkloss service successfully registered \r\n")	;
		} 
		else {
			DBG_LOG("Linkloss service registration failed \r\n");	
		}
		
		//registering optional services
		
		//Txpower Service
		
		if (register_txpower_service() == AT_BLE_SUCCESS) {
			DBG_LOG("txpower service successfully registered \r\n")	;
		}
		else {
			DBG_LOG("txpower service registration failed \r\n");
		}
		
		//Immediate alert service
		if (register_immediate_alert_service() == AT_BLE_SUCCESS) {
			DBG_LOG("Immediate alert service successfully registered \r\n")	;
		}
		else {
			DBG_LOG("Immediate service registration failed \r\n");
		}
		
		
	adv_data[idx++] = LL_ADV_DATA_UUID_LEN + ADV_TYPE_LEN +  TXP_ADV_DATA_UUID_LEN + IAL_ADV_DATA_UUID_LEN ;
	adv_data[idx++] = LL_ADV_DATA_UUID_TYPE;
	
	/* Appending the UUID */
	memcpy(&adv_data[idx], LL_ADV_DATA_UUID_DATA, LL_ADV_DATA_UUID_LEN);
	idx += LL_ADV_DATA_UUID_LEN;
	
	/* Prepare ADV Data for LL Service */
	memcpy(&adv_data[idx], TXP_ADV_DATA_UUID_DATA, TXP_ADV_DATA_UUID_LEN);
	idx += TXP_ADV_DATA_UUID_LEN;
	
	/* Prepare ADV Data for LL Service */
	memcpy(&adv_data[idx], IAL_ADV_DATA_UUID_DATA, IAL_ADV_DATA_UUID_LEN);
	idx += IAL_ADV_DATA_UUID_LEN;
	
	/*Appending the complete name to the Ad packet */
	adv_data[idx++] = PXP_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = PXP_ADV_DATA_NAME_TYPE;
	memcpy(&adv_data[idx], PXP_ADV_DATA_NAME_DATA, PXP_ADV_DATA_NAME_LEN );
	idx += PXP_ADV_DATA_NAME_LEN ;
	
	/*if ((status_t = at_ble_adv_set_tx_power((int8_t ) 3)) != AT_BLE_SUCCESS) {
		DBG_LOG("Setting the tx power is failed for %x\r\n",status_t);
	} else {
		DBG_LOG("Setting the tx power is successful \r\n");
	}*/
	
	if (at_ble_adv_data_set(adv_data, idx, scan_rsp_data, SCAN_RESP_LEN) != AT_BLE_SUCCESS) {
		DBG_LOG("\r\n failed adv data set");
	}
	else
	{
		DBG_LOG("Successfully adv data set");
	}
	
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY,
	APP_PXP_FAST_ADV, APP_PXP_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
	{
		DBG_LOG("\r\nBLE Adv start Failed");
	}
	else
	{
		DBG_LOG_1LVL("\r\nBLE Started Adv");
	}
	

	return 1;
	//Set the advertisement parameters and start advertising
	//at_ble_status_t at_ble_adv_data_set( );
}

