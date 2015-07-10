

#include "at_ble_api.h"
#include "platform.h"
#include "console_serial.h"
#include "immediate_alert_service.h"

#define DBG_LOG printf
#define DBG_LOG printf




uint8_t immediate_alert_initial_value = 0;



void init_immediate_alert_service(immediate_alert_serv_record *immediate_alert_serv ) 
{
		
		immediate_alert_serv->immediate_alert_serv_handle = 0;
		immediate_alert_serv->immediate_alert_serv_uuid.type = AT_BLE_UUID_16;
		immediate_alert_serv->immediate_alert_serv_uuid.uuid[0] = 0x02;
		immediate_alert_serv->immediate_alert_serv_uuid.uuid[1] = 0x18;
		
		//Characterisitc Info for Alert Value
		immediate_alert_serv->immediate_alert_serv_chars.char_val_handle = 0;          /* handle stored here */
		immediate_alert_serv->immediate_alert_serv_chars.uuid.type = AT_BLE_UUID_16;
		immediate_alert_serv->immediate_alert_serv_chars.uuid.uuid[0] = 0x06;          /* UUID : Alert Level */
		immediate_alert_serv->immediate_alert_serv_chars.uuid.uuid[1] = 0x2a;          /* UUID : Alert Level */
		immediate_alert_serv->immediate_alert_serv_chars.properties = AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE; /* Properties */
		immediate_alert_serv->immediate_alert_serv_chars.init_value = &immediate_alert_initial_value; /* value */
		immediate_alert_serv->immediate_alert_serv_chars.value_init_len = sizeof(int8_t);
		immediate_alert_serv->immediate_alert_serv_chars.value_max_len = sizeof(int8_t);
		immediate_alert_serv->immediate_alert_serv_chars.value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);   /* permissions */
		immediate_alert_serv->immediate_alert_serv_chars.user_desc = NULL;           /* user defined name */
		immediate_alert_serv->immediate_alert_serv_chars.user_desc_len = 0;
		immediate_alert_serv->immediate_alert_serv_chars.user_desc_max_len = 0;
		immediate_alert_serv->immediate_alert_serv_chars.user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
		immediate_alert_serv->immediate_alert_serv_chars.client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
		immediate_alert_serv->immediate_alert_serv_chars.server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
		immediate_alert_serv->immediate_alert_serv_chars.user_desc_handle = 0;             /*user desc handles*/
		immediate_alert_serv->immediate_alert_serv_chars.client_config_handle = 0;         /*client config handles*/
		immediate_alert_serv->immediate_alert_serv_chars.server_config_handle = 0;         /*server config handles*/
		immediate_alert_serv->immediate_alert_serv_chars.presentation_format = NULL;       /* presentation format */
		
}

