#ifndef __HTPT_APP_H__
#define __HTPT_APP_H__

#include "at_ble_api.h"

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
}htpt_app_t;

/**@brief Temperature measurement stability type
*/
typedef enum
{
	UNSTABLE_TEMPERATURE_VAL= 0,
	STABLE_TEMPERATURE_VAL=1
}stable_temp_reading;

#endif /* __HTPT_APP_H__ */