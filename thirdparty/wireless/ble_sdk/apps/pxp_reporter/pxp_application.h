
#ifndef __PXP_APP_H__
#define __PXP_APP_H__

#include "at_ble_api.h"
#include "platform.h"
#include "console_serial.h"
#include "pxp_reporter.h"

#define DBG_LOG printf
#define DBG_LOG printf

#define PATHLOSS 1
#define LINKLOSS 1
#define APP_PXP_FAST_ADV 100 //100 ms

#define APP_PXP_ADV_TIMEOUT 1000 // 100 Secs


#define APP_PXP_FAST_ADV 100 //100 ms

#define APP_PXP_ADV_TIMEOUT 1000 // 100 Secs

#define SCAN_RESP_LEN 10
#define ADV_DATA_LEN 18

#define ADV_TYPE_LEN (0x01)

#define LL_ADV_DATA_UUID_LEN	 (2)
#define LL_ADV_DATA_UUID_TYPE	 (0x03)
#define LL_ADV_DATA_UUID_DATA	 "\x03\x18"

#define TXP_ADV_DATA_UUID_LEN	 (2)
#define TXP_ADV_DATA_UUID_TYPE	 (0x03)
#define TXP_ADV_DATA_UUID_DATA	 "\x04\x18"

#define IAL_ADV_DATA_UUID_LEN	 (2)
#define IAL_ADV_DATA_UUID_TYPE	 (0x03)
#define IAL_ADV_DATA_UUID_DATA	 "\x02\x18"


#define PXP_ADV_DATA_NAME_LEN	 (9)
#define PXP_ADV_DATA_NAME_TYPE	 (0x09)
#define PXP_ADV_DATA_NAME_DATA	 "ATMEL-PXP"



void start_advertising(void);

//void init_proximity_reporter (proximity_serv_info *services , features_option *choice) ;

//uint8_t add_proximity_service_database (proximity_serv_info *services ,features_option *choice);




#endif