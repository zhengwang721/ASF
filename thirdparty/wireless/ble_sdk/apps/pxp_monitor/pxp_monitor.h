#ifndef __PXP_MONITOR_H__
#define __PXP_MONITOR_H__

#include "at_ble_api.h"

#define SCAN_INTERVAL 48              //Scan interval 50ms in term of 625us
#define SCAN_WINDOW 48                //Scan window 30ms values in term of 625ms
#define SCAN_TIMEOUT 0x0000           //Timeout  Scan time-out, 0x0000 disables time-out

#define PXP_CONN_MIN 20
#define PXP_CONN_MAX 40
#define PXP_CONN_LATENCY 4
#define PXP_CE_LEN_MIN 0
#define PXP_CE_LEN_MAX 0
#define PXP_SUPER_TIMOUT 0x1f4

typedef enum
{
	NO_ALERT,
	LOW_ALERT,
	HIGH_ALERT
}pxp_alert_level;

typedef struct
{
	at_ble_handle_t srt_handle;
	at_ble_handle_t end_handle;
	at_ble_handle_t val_handle;
}pxp_handler;


pxp_handler tx_handler,linkloss_handler,immed_alert_handler;

at_ble_status_t pxp_write(at_ble_handle_t pxp_conn_handle,at_ble_handle_t pxp_char_handle,uint8_t pxp_value);

void pxp_parse(void);

at_ble_status_t pxp_scan_start(void);

at_ble_status_t pxp_dev_connect (at_ble_scan_info_t dev_connect);


#endif /*__PXP_MONITOR_H__*/