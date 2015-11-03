#ifndef PASS_H_INCLUDE
#define PASS_H_INCLUDE

#include "at_ble_api.h"

#define ALERT_STATUS_CHAR_LEN			(1)
#define RINGER_SETTING_CHAR_LEN			(1)
#define CLIENT_CONF_CHAR_DESCRIPTOR_LEN	(2)

at_ble_status_t pas_read_alert_status_char(at_ble_handle_t conn_handle,at_ble_handle_t char_handle);
at_ble_status_t pas_read_ringer_setting_char(at_ble_handle_t conn_handle,at_ble_handle_t char_handle);
at_ble_status_t pas_char_write_notification(at_ble_handle_t conn_handle,at_ble_handle_t char_handle,bool enable);
at_ble_status_t pas_char_set_ringer_control_point(at_ble_handle_t conn_handle,at_ble_handle_t char_handle,uint8_t data);

#endif

