

#include "at_ble_api.h"
#include "platform.h"
#include "console_serial.h"

#define DBG_LOG printf
#define DBG_LOG_1LVL printf

at_ble_uuid_t txpower_service_uuid;
at_ble_handle_t txpower_service_handle;
at_ble_handle_t txpower_characteristic_handle;

	
	uint8_t txpower_initial_value;
	at_ble_status_t register_txpower_service(void );

	at_ble_status_t register_txpower_service(void ) 
	{	
		static at_ble_characteristic_t tx_power_service_chars[] = {
			{
				0, /* handle stored here */
				{ AT_BLE_UUID_16, {0x07, 0x2a}}, /* UUID : Tx Power Level */
				AT_BLE_CHAR_READ, /* Properties */
				&txpower_initial_value, sizeof(int8_t), sizeof(int8_t), /* value */
				AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR, /* permissions */
				NULL, 0, 0, /* user defined name */
				AT_BLE_ATTR_NO_PERMISSIONS, /*user description permissions*/
				AT_BLE_ATTR_NO_PERMISSIONS, /*client config permissions*/
				AT_BLE_ATTR_NO_PERMISSIONS, /*server config permissions*/
				0,0,0,	/*user desc, client config, and server config handles*/
				NULL /* presentation format */
			}
		};
		
		txpower_service_uuid.type = AT_BLE_UUID_16;	
		txpower_service_uuid.uuid[0] = 0x04;
		txpower_service_uuid.uuid[1] = 0x18;
		
		if(at_ble_primary_service_define(&txpower_service_uuid, &txpower_service_handle, NULL, 0, tx_power_service_chars, 1) == AT_BLE_SUCCESS)
		{
			DBG_LOG("\r\nPass at_ble_primary_service_define");
			txpower_characteristic_handle = tx_power_service_chars[0].char_val_handle;
				return AT_BLE_SUCCESS;
		}
		else
		{
			DBG_LOG("\r\nFail at_ble_primary_service_define");
			return AT_BLE_FAILURE;
		}
		
	}


