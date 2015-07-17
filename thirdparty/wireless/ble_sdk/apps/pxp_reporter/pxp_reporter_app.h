
#ifndef __PXP_REPORTER_APP_H__
#define __PXP_REPORTER_APP_H__

/****************************************************************************************
*							        Includes	                                     							*
****************************************************************************************/

#include "at_ble_api.h"
#include "platform.h"
#include "console_serial.h"
#include "pxp_reporter.h"
//#include "ble_manager.h"

/****************************************************************************************
*							        Macros	                                     							*
****************************************************************************************/

/** @brief Maximum length of event parameters */
#define EVENT_MAX_PARAM_LENGTH 512

/** @brief APP_PXP_FAST_ADV between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s). */
#define APP_PXP_FAST_ADV 100 //100 ms

/** @brief APP_PXP_ADV_TIMEOUT Advertising time-out between 0x0001 and 0x3FFF in seconds, 0x0000 disables time-out.*/
#define APP_PXP_ADV_TIMEOUT 1000 // 100 Secs

/** @brief scan_resp_len is the length of the scan response data */
#define SCAN_RESP_LEN 10

/** @brief ADV_DATA_LEN */
#define ADV_DATA_LEN 18

/** @brief ADV_TYPE_LEN */
#define ADV_TYPE_LEN (0x01)

/** @brief LL_ADV_DATA_UUID_LEN the size of LL service uuid */
#define LL_ADV_DATA_UUID_LEN	 (2)

/** @brief LL_ADV_DATA_UUID_TYPE the total sizeof LL service uuid*/
#define LL_ADV_DATA_UUID_TYPE	 (0x03)

/** @brief LL_ADV_DATA_UUID_DATA the LL service uuid*/
#define LL_ADV_DATA_UUID_DATA	 "\x03\x18"

/** @brief TXP_ADV_DATA_UUID_LEN the size of TXP service uuid */
#define TXP_ADV_DATA_UUID_LEN	 (2)

/** @brief TXP_ADV_DATA_UUID_TYPE the total sizeof TXP service uuid*/
#define TXP_ADV_DATA_UUID_TYPE	 (0x03)

/** @brief TXP_ADV_DATA_UUID_DATA the TXP service uuid*/
#define TXP_ADV_DATA_UUID_DATA	 "\x04\x18"

/** @brief IAL_ADV_DATA_UUID_LEN the size of IAL service uuid */
#define IAL_ADV_DATA_UUID_LEN	 (2)

/** @brief IAL_ADV_DATA_UUID_TYPE the total sizeof IAL service uuid*/
#define IAL_ADV_DATA_UUID_TYPE	 (0x03)

/** @brief IAL_ADV_DATA_UUID_DATA the IAL service uuid*/
#define IAL_ADV_DATA_UUID_DATA	 "\x02\x18"

/** @brief PXP_ADV_DATA_NAME_LEN the  length of the device name */
#define PXP_ADV_DATA_NAME_LEN	 (9)

/** @brief PXP_ADV_DATA_NAME_TYPE the gap ad data type */
#define PXP_ADV_DATA_NAME_TYPE	 (0x09)

/** @brief PXP_ADV_DATA_NAME_DATA the actual name of device*/
#define PXP_ADV_DATA_NAME_DATA	 "ATMEL-PXP"

/** @brief led_state  the state of the led*/
extern uint8_t led_state;

#endif /* __PXP_REPORTER_APP_H__ */