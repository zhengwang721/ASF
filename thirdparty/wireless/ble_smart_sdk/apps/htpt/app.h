
#ifndef __HTPT_APP_H__
#define __HTPT_APP_H__

#ifndef NULL
#define NULL ((void*)0)
#endif

#define DBG_LOG printf

#define APP_HT_FAST_ADV 100     //100 ms
#define APP_HT_ADV_TIMEOUT 1000 // 100 Secs

#define SCAN_RESP_LEN 10

#define ADV_TYPE_LEN (0x01)

#define HT_ADV_DATA_UUID_LEN	 (2)
#define HT_ADV_DATA_UUID_TYPE	 (0x03)
#define HT_ADV_DATA_UUID_DATA	 "\x09\x18"

#define HT_ADV_DATA_APPEARANCE_LEN	 (2)
#define HT_ADV_DATA_APPEARANCE_TYPE	 (0x19)
#define HT_ADV_DATA_APPEARANCE_DATA	 "\x00\x03"

#define HT_ADV_DATA_NAME_LEN	 (13)
#define HT_ADV_DATA_NAME_TYPE	 (0x09)
#define HT_ADV_DATA_NAME_DATA	 "ATMEL-B11-HTP"

#define HT_DEVICE_NAME_LEN       9
#define HT_DEVICE_NAME_DATA      "ATMEL-HTP"

#define DISS_ADV_DATA_UUID_LEN	 (2)
#define DISS_ADV_DATA_UUID_TYPE	 (0x03)
#define DISS_ADV_DATA_UUID_DATA	 "\x0A\x18"

// https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.device_information.xml
#define DISS_CHAR_UUID_SYSTEM_ID                "\x23\x2A"    // own data type
#define DISS_CHAR_UUID_MODEL_NUMBER_STRING      "\x24\x2A"
#define DISS_CHAR_UUID_SERIAL_NUMBER_STRING     "\x25\x2A"
#define DISS_CHAR_UUID_FIRMWARE_REVISION_STRING "\x26\x2A"
#define DISS_CHAR_UUID_HARDWARE_REVISION_STRING "\x27\x2A"
#define DISS_CHAR_UUID_SOFTWARE_REVISION_STRING "\x28\x2A"
#define DISS_CHAR_UUID_MANUFACTURER_NAME_STRING "\x29\x2A"
#define DISS_CHAR_UUID_PNP_ID                   "\x50\x2A"    // own data type
#define DISS_CHAR_UUID_CERTIFICATION_DATA_LIST  "\x2A\x2A"    // own data type : IEEE 11073-20601 Regulatory Certification Data List 
#define DISS_MANUFACTURER_NAME                  "ATMEL"
#define DISS_SYSTEM_ID                          "ATMEL_KEVIN"

/* Typedef for health thermometer profile -  application */
typedef struct htpt_app{	
	
	/* Measured temperature value. Value may be Cecilius /Fahrenheit */
	uint32_t temperature;	
	
	/* Temperature type string */
	at_ble_htpt_temp_type temperature_type;
	
	/* Measurement Interval */
	uint16_t measurement_interval;
	
	/* Minimum measurement interval */
	uint16_t min_measurement_intv;
	
	/* Maximum measurement interval */
	uint16_t max_meaurement_intv;
	
	/* Security Level */
	at_ble_htpt_sec_level security_lvl;
	
	/* Optional Features */
	at_ble_htpt_db_config_flag optional;
	
	at_ble_htpt_temp_flags flags;
}htpt_app_t;

/**@brief Temperature measurement stability type
*/
typedef enum
{
	UNSTABLE_TEMPERATURE_VAL= 0,
	STABLE_TEMPERATURE_VAL=1
}stable_temp_reading;

#endif /* __HTPT_APP_H__ */

