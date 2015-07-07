#include <asf.h>
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "pxp_monitor.h"
#include "pxp_monitor_app.h"
#include "profiles.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_extint.h"

#include "pxp_monitor.h"

at_ble_status_t pxp_write(at_ble_handle_t pxp_conn_handle,at_ble_handle_t pxp_char_handle,uint8_t pxp_value)
{
	at_ble_status_t write_status = AT_BLE_FAILURE;
	//
	if(pxp_char_handle == linkloss_handler.val_handle)
	{
		// for link loss property write with response
		write_status = at_ble_characteristic_write(pxp_conn_handle,pxp_char_handle,0,1,&pxp_value,false,true);
	}
	else if (pxp_char_handle == immed_alert_handler.val_handle)
	{
		// for immediate alert write without response
		write_status = at_ble_characteristic_write(pxp_conn_handle,pxp_char_handle,0,1,&pxp_value,false,false);
	}
	
	return (write_status);
}

at_ble_status_t pxp_scan_start(void)
{
	
	return(at_ble_scan_start(SCAN_INTERVAL, SCAN_WINDOW, SCAN_TIMEOUT, AT_BLE_SCAN_ACTIVE, AT_BLE_SCAN_GEN_DISCOVERY, false,true)) ;
}

at_ble_status_t pxp_dev_connect (at_ble_scan_info_t dev_connect)
{
	at_ble_addr_t pxp_peripheral_1[1];
	at_ble_connection_params_t pxp_parameter_1;
	
	memcpy((uint8_t *)&pxp_peripheral_1[0], &dev_connect.dev_addr,sizeof(at_ble_addr_t));
	
	pxp_parameter_1.con_intv_min = PXP_CONN_MIN;
	pxp_parameter_1.con_intv_max = PXP_CONN_MAX;
	pxp_parameter_1.con_latency = PXP_CONN_LATENCY;
	pxp_parameter_1.ce_len_min = PXP_CE_LEN_MIN;
	pxp_parameter_1.ce_len_max = PXP_CE_LEN_MAX;
	pxp_parameter_1.superv_to = PXP_SUPER_TIMOUT;//500
	
	return( at_ble_connect(pxp_peripheral_1, 1 , SCAN_INTERVAL, SCAN_WINDOW, &pxp_parameter_1));
}
