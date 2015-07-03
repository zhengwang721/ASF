

#include "at_ble_api.h"
#include "platform.h"
#include "console_serial.h"

#define DBG_LOG printf
#define DBG_LOG_1LVL printf

at_ble_uuid_t	immediate_alert_service_uuid;
at_ble_handle_t	immediate_alert_service_handle;
at_ble_handle_t immediate_alert_characteristic_handle;


uint8_t immediate_alert_initial_value = 0;

at_ble_status_t register_immediate_alert_service(void );

at_ble_status_t register_immediate_alert_service(void ) 
{
		
		static at_ble_characteristic_t immediate_alert_service_chars[] = {
		{
			0, /* handle stored here */
			{ AT_BLE_UUID_16, {0x06, 0x2a}}, /* UUID : Alert Level */
			AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE, /* Properties */
			&immediate_alert_initial_value, sizeof(int8_t), sizeof(int8_t), /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR, /* permissions */
			NULL, 0, 0, /* user defined name */
			AT_BLE_ATTR_NO_PERMISSIONS, /*user description permissions*/
			AT_BLE_ATTR_NO_PERMISSIONS, /*client config permissions*/
			AT_BLE_ATTR_NO_PERMISSIONS, /*server config permissions*/
			0,0,0,	/*user desc, client config, and server config handles*/
			NULL /* presentation format */
		}
	};

	
	immediate_alert_service_uuid.type = AT_BLE_UUID_16;
	immediate_alert_service_uuid.uuid[0] = 0x02;
	immediate_alert_service_uuid.uuid[1] = 0x18;
	
	if(at_ble_primary_service_define(&immediate_alert_service_uuid, &immediate_alert_service_handle, NULL, 0, immediate_alert_service_chars, 1) == AT_BLE_SUCCESS)
	{
		DBG_LOG("\r\nPass at_ble_primary_service_define");
		immediate_alert_characteristic_handle = immediate_alert_service_chars[0].char_val_handle;
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("\r\nFail at_ble_primary_service_define");
		return AT_BLE_FAILURE;
	}
	
}

