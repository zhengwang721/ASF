
#include "at_ble_api.h"
#include "platform.h"
#include "console_serial.h"
#include "linkloss_service.h"

#define DBG_LOG printf
#define DBG_LOG printf

/*
at_ble_uuid_t linkloss_service_uuid;
at_ble_handle_t linkloss_service_handle;
at_ble_handle_t linkloss_alert_characteristic_handle;
*/

uint8_t linkloss_initial_value = 0;



void init_linkloss_service(linkloss_serv_record *linkloss_serv )
 {
	 
	 linkloss_serv->linkloss_serv_handle = 0;
	 linkloss_serv->linkloss_serv_uuid.type = AT_BLE_UUID_16;
	 linkloss_serv->linkloss_serv_uuid.uuid[0] = 0x03;
	 linkloss_serv->linkloss_serv_uuid.uuid[1] = 0x18;
	 
	 //Characterisitc Info for Alert Value
	 linkloss_serv->linkloss_serv_chars.char_val_handle = 0;          /* handle stored here */
	 linkloss_serv->linkloss_serv_chars.uuid.type = AT_BLE_UUID_16;
	 linkloss_serv->linkloss_serv_chars.uuid.uuid[0] = 0x06;          /* UUID : Alert Level */
	 linkloss_serv->linkloss_serv_chars.uuid.uuid[1] = 0x2a;          /* UUID : Alert Level */
	 linkloss_serv->linkloss_serv_chars.properties = AT_BLE_CHAR_READ | AT_BLE_CHAR_WRITE; /* Properties */
	 linkloss_serv->linkloss_serv_chars.init_value = &linkloss_initial_value ; /* value */
	 linkloss_serv->linkloss_serv_chars.value_init_len = sizeof(int8_t);
	 linkloss_serv->linkloss_serv_chars.value_max_len = sizeof(int8_t);
	 linkloss_serv->linkloss_serv_chars.value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);   /* permissions */
	 linkloss_serv->linkloss_serv_chars.user_desc = NULL;           /* user defined name */
	 linkloss_serv->linkloss_serv_chars.user_desc_len = 0;
	 linkloss_serv->linkloss_serv_chars.user_desc_max_len = 0;
	 linkloss_serv->linkloss_serv_chars.user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 linkloss_serv->linkloss_serv_chars.client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 linkloss_serv->linkloss_serv_chars.server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 linkloss_serv->linkloss_serv_chars.user_desc_handle = 0;             /*user desc handles*/
	 linkloss_serv->linkloss_serv_chars.client_config_handle = 0;         /*client config handles*/
	 linkloss_serv->linkloss_serv_chars.server_config_handle = 0;         /*server config handles*/
	 linkloss_serv->linkloss_serv_chars.presentation_format = NULL;       /* presentation format */
	
 }