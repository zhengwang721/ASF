
#include "pxp_reporter.h"

uint8_t tx_pwr_initial_value;
//at_ble_handle_t tx_serv_handle;
	
void init_proximity_reporter (proximity_serv_info *services , features_option *choice) 
{
	
	if (choice ->linkloss ) {
		// Initializing the linkloss service for linkloss
		init_linkloss_service(&services->linkloss_service);
	}
	
	if (choice->pathloss) {	
		//Initializing the immediate alert service
		init_immediate_alert_service(&services->immediate_alert_service);
		//Initializing the tx power service
		init_tx_power_service(&services->tx_power_service);
	}
			
}
		
uint8_t add_proximity_service_database (proximity_serv_info *services ,features_option *choice)
{
	
	
	//Adding services for link loss
	if (choice->linkloss) 
	{		//Linkloss service
			
			if(at_ble_primary_service_define(&services->linkloss_service.linkloss_serv_uuid,
								  &services->linkloss_service.linkloss_serv_handle, NULL, 0,
					   &services->linkloss_service.linkloss_serv_chars, 1) == AT_BLE_SUCCESS) 
			{
				#ifdef DBG_LOG
				//DBG_LOG("Linkloss successfull added and handle is %x\n",
							//				linkloss_service->linkloss_serv_handle);
				#endif
			}
			else
			{
				DBG_LOG("Fail link loss service definition");
				return AT_BLE_FAILURE;
			}
	}
		
	//Adding services for pathloss
	if (choice->pathloss) 
	{	//Immediate Alert Service
	
		if(at_ble_primary_service_define(&services->immediate_alert_service.immediate_alert_serv_uuid,
			 &services->immediate_alert_service.immediate_alert_serv_handle, NULL, 0,
			  &services->immediate_alert_service.immediate_alert_serv_chars, 1) == AT_BLE_SUCCESS)
		{
			#ifdef DBG_LOG
			//DBG_LOG("Immediate alert service successfully added and handle is %x",
							//	immediate_alert_service->immediate_alert_serv_handle);
			#endif
		}
		else
		{
			#ifdef DBG_LOG
			DBG_LOG("Fail immediate_alert__service_define");
			#endif
			return AT_BLE_FAILURE;
		}
		// Tx Power Service
		
		
		if(at_ble_primary_service_define(&services->tx_power_service.tx_power_serv_uuid,&services->tx_power_service.tx_power_serv_handle,
		 NULL, 0, &services->tx_power_service.tx_power_serv_chars, 1) == AT_BLE_SUCCESS) 
		{
			#ifdef DBG_LOG
			//DBG_LOG("tx power successful added and handle is %x",tx_serv_handle);
			#endif
		}
		else
		{
			#ifdef DBG_LOG
			DBG_LOG("Fail at_ble_primary_service_define");
			#endif
			return AT_BLE_FAILURE;
		}
 
	}
	DBG_LOG("The link loss handle is %x",services->linkloss_service.linkloss_serv_handle);
	DBG_LOG("The immediate loss handle is %x",services->immediate_alert_service.immediate_alert_serv_handle);
	DBG_LOG("The tx power handle is %x",services->tx_power_service.tx_power_serv_handle);
	return AT_BLE_SUCCESS;	
}




