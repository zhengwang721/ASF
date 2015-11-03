#include "ble_manager.h"
#include "pas_service.h"


/** 
* @brief triggers the read of alert status characteristic * @param[in] at_ble_handle_t connection handle 
* @param[in] at_ble_handle-t characteristic handle 
* @return AT_BLE_SUCCESS for success AT_BLE_FAILURE for failure 
*/
at_ble_status_t pas_read_alert_status_char(at_ble_handle_t conn_handle,at_ble_handle_t char_handle)
{	
	return (at_ble_characteristic_read(conn_handle,char_handle,0,ALERT_STATUS_CHAR_LEN));
}

/** 
* @brief triggers the read of ringer setting characteristic 
* @param[in] at_ble_handle_t connection handle 
* @param[in] at_ble_handle-t characteristic handle 
* @return AT_BLE_SUCCESS for success AT_BLE_FAILURE for failure 
*/
at_ble_status_t pas_read_ringer_setting_char(at_ble_handle_t conn_handle,at_ble_handle_t char_handle)
{	
	return (at_ble_characteristic_read(conn_handle,char_handle,0,RINGER_SETTING_CHAR_LEN));
}

/** 
* @brief Enable/disable the notification 
* @param[in] at_ble_handle_t connection handle 
* @param[in] at_ble_handle-t characteristic handle 
* @param[in] set/clear the notification 
* @return AT_BLE_SUCCESS for success AT_BLE_FAILURE for failure 
*/
at_ble_status_t pas_char_write_notification(at_ble_handle_t conn_handle,at_ble_handle_t char_handle,bool enable)
{	
	uint16_t data = enable;	
	return (at_ble_characteristic_write(conn_handle,char_handle,0,CLIENT_CONF_CHAR_DESCRIPTOR_LEN	,(uint8_t *)&data,false,false));
}

/** 
* @brief sets ringer control point 
* @param[in] at_ble_handle_t connection handle 
* @param[in] at_ble_handle-t characteristic handle 
* @param[in] set/clear the notification 
* @return AT_BLE_SUCCESS for success AT_BLE_FAILURE for failure 
*/
at_ble_status_t pas_char_set_ringer_control_point(at_ble_handle_t conn_handle,at_ble_handle_t char_handle,uint8_t data)
{	
	uint8_t ringer_data = data;	
	return (at_ble_characteristic_write(conn_handle,char_handle,0,RINGER_SETTING_CHAR_LEN	,&ringer_data,false,false));
}


