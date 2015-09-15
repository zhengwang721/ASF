#include "platform_drv.h"
#include "at_ble_api.h"
#include "app.h"
#include "at30tse75x.h"
#include <string.h>
#include "uart.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"
#include "timer.h"
#include <math.h>
#include "conf_timer.h"


/**
temperature sensor example
npi-samb11-asf\common2\components\memory\eeprom\at30tse75x\example

at30tse75x.h check and use below value for sensor choice

// chris.choi : samb11 main board's sensor (j403)
//#define AT30TSE_TEMPERATURE_TWI_ADDR	0x48 //0b1001000
//#define AT30TSE758_EEPROM_TWI_ADDR		0x50 //0b1010000
//#define AT30TSE754_EEPROM_TWI_ADDR		0x50 //0b1010000
//#define AT30TSE752_EEPROM_TWI_ADDR		0x50 //0b1010000

// chris.choi : I01 sensor
#define AT30TSE_TEMPERATURE_TWI_ADDR	0x4F //0b1001000
#define AT30TSE758_EEPROM_TWI_ADDR		0x54 //0b1010000
#define AT30TSE754_EEPROM_TWI_ADDR		0x56 //0b1010000
#define AT30TSE752_EEPROM_TWI_ADDR		0x57 //0b1010000
**/


/* timer example
npi-samb11-asf\sam0\drivers\timer\quick_start
*/


//#define CHIPVERSION_CARRIER
#define CHIPVERSION_A5
//
//#if defined(CHIPVERSION_A5)
	//#define UART_HW_MODULE_TYPE UART_HW_MODULE_UART1
//#elif defined(CHIPVERSION_CARRIER)
	//#define UART_HW_MODULE_TYPE UART_HW_MODULE_UART2
//#endif

bool app_device_bond = false;
uint8_t auth_info = 0;
htpt_app_t htpt_data;

uint8_t isEnableTempMeasurement = 0;
uint8_t isEnableIntermediateTemp = 0;
uint8_t isEnableMeasInterval = 0;


#define ATMEL_PRINTF_BUFF_LEN 80
static char atmel_printf_buff[ATMEL_PRINTF_BUFF_LEN];
static void atmel_printf(char* format, ...)
{
	//int inputStringLength = strlen(format);
	//if(inputStringLength > ATMEL_PRINTF_BUFF_LEN)
	//{
		//uart_printf(UART_HW_MODULE_TYPE, "atmel_printf / string length(input-%d, max-%d) Error !!!!!\r\n",
			//inputStringLength, ATMEL_PRINTF_BUFF_LEN);
		//return;
	//}
//
	//__va_list args;
	//va_start(args, format);
//
	//vsprintf(atmel_printf_buff, format, args);	
	//atmel_printf_buff[ATMEL_PRINTF_BUFF_LEN - 1] = 0;
	//uart_printf(UART_HW_MODULE_TYPE, atmel_printf_buff);
//
	//va_end(args);
}

static void adv_init(void)
{
	uint8_t idx = 0;
	uint8_t ro_adv_data[HT_ADV_DATA_NAME_LEN + HT_ADV_DATA_APPEARANCE_LEN + HT_ADV_DATA_UUID_LEN + 2 * 3];
	uint8_t ro_scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xFF, 0x00, 0x06, 0x25, 0x75, 0x11, 0x6a, 0x7f, 0x7f};

	/* Prepare ADV Data */
	ro_adv_data[idx++] = HT_ADV_DATA_UUID_LEN + ADV_TYPE_LEN;
	ro_adv_data[idx++] = HT_ADV_DATA_UUID_TYPE;
	memcpy(&ro_adv_data[idx], HT_ADV_DATA_UUID_DATA, HT_ADV_DATA_UUID_LEN);
	idx += HT_ADV_DATA_UUID_LEN;

	ro_adv_data[idx++] = HT_ADV_DATA_APPEARANCE_LEN + ADV_TYPE_LEN;
	ro_adv_data[idx++] = HT_ADV_DATA_APPEARANCE_TYPE;
	memcpy(&ro_adv_data[idx], HT_ADV_DATA_APPEARANCE_DATA, HT_ADV_DATA_APPEARANCE_LEN);
	idx += HT_ADV_DATA_APPEARANCE_LEN;

	ro_adv_data[idx++] = HT_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
	ro_adv_data[idx++] = HT_ADV_DATA_NAME_TYPE;
	memcpy(&ro_adv_data[idx], HT_ADV_DATA_NAME_DATA, HT_ADV_DATA_NAME_LEN);
	idx += HT_ADV_DATA_NAME_LEN;

	at_ble_adv_data_set(ro_adv_data, idx, ro_scan_rsp_data, SCAN_RESP_LEN);
}

void htpt_init(htpt_app_t *htpt_temp)
{
	/* Initialize the default htp parameters*/
	htpt_temp->measurement_interval = 5;
	htpt_temp->temperature = 25;
	htpt_temp->temperature_type = HTP_TYPE_ARMPIT;
	htpt_temp->max_meaurement_intv = 30;
	htpt_temp->min_measurement_intv = 1;
	htpt_temp->security_lvl = HTPT_UNAUTH;
	htpt_temp->optional = HTPT_ALL_FEAT_SUP;
	htpt_temp->flags = HTPT_FLAG_CELSIUS | HTPT_FLAG_TYPE;
}

void diss_init(void)
{
	at_ble_status_t status;
	at_ble_handle_t service;
	at_ble_uuid_t service_uuid = { AT_BLE_UUID_16, {DISS_ADV_DATA_UUID_DATA}};

	at_ble_characteristic_t chars[] = {
		{
			0, /* handle stored here */
			{ AT_BLE_UUID_16, {DISS_CHAR_UUID_MANUFACTURER_NAME_STRING}}, /* UUID */
			AT_BLE_CHAR_READ, /* Properties */
			(uint8_t *)DISS_MANUFACTURER_NAME, strlen(DISS_MANUFACTURER_NAME), 100, /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR,
			NULL, 0, 0, /* user defined name */
			AT_BLE_ATTR_NO_PERMISSIONS, /*user description permissions*/
			AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR, /*client config permissions*/
			AT_BLE_ATTR_NO_PERMISSIONS, /*server config permissions*/
			0,0,0,	/*user desc, client config, and server config handles*/
			NULL /* presentation format */
		},
		{
			0, /* handle stored here */
			{ AT_BLE_UUID_16, {DISS_CHAR_UUID_MODEL_NUMBER_STRING}}, /* UUID */
			AT_BLE_CHAR_READ, /* Properties */
			(uint8_t *)"ATMEL", strlen("ATMEL"), 100, /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR,
			NULL, 0, 0, /* user defined name */
			AT_BLE_ATTR_NO_PERMISSIONS, /*user description permissions*/
			AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR, /*client config permissions*/
			AT_BLE_ATTR_NO_PERMISSIONS, /*server config permissions*/
			0,0,0,	/*user desc, client config, and server config handles*/
			NULL /* presentation format */
		},
		{
			0, /* handle stored here */
			{ AT_BLE_UUID_16, {DISS_CHAR_UUID_SERIAL_NUMBER_STRING}}, /* UUID */
			AT_BLE_CHAR_READ, /* Properties */
			(uint8_t *)"ATMEL1", strlen("ATMEL1"), 100, /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR,
			NULL, 0, 0, /* user defined name */
			AT_BLE_ATTR_NO_PERMISSIONS, /*user description permissions*/
			AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR, /*client config permissions*/
			AT_BLE_ATTR_NO_PERMISSIONS, /*server config permissions*/
			0,0,0,	/*user desc, client config, and server config handles*/
			NULL /* presentation format */
		},
		{
			0, /* handle stored here */
			{ AT_BLE_UUID_16, {DISS_CHAR_UUID_HARDWARE_REVISION_STRING}}, /* UUID */
			AT_BLE_CHAR_READ, /* Properties */
			(uint8_t *)"ATMEL3", strlen("ATMEL3"), 100, /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR,
			NULL, 0, 0, /* user defined name */
			AT_BLE_ATTR_NO_PERMISSIONS, /*user description permissions*/
			AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR, /*client config permissions*/
			AT_BLE_ATTR_NO_PERMISSIONS, /*server config permissions*/
			0,0,0,	/*user desc, client config, and server config handles*/
			NULL /* presentation format */
		},
		{
			0, /* handle stored here */
			{ AT_BLE_UUID_16, {DISS_CHAR_UUID_FIRMWARE_REVISION_STRING}}, /* UUID */
			AT_BLE_CHAR_READ, /* Properties */
			(uint8_t *)"ATMEL2", strlen("ATMEL2"), 100, /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR,
			NULL, 0, 0, /* user defined name */
			AT_BLE_ATTR_NO_PERMISSIONS, /*user description permissions*/
			AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR, /*client config permissions*/
			AT_BLE_ATTR_NO_PERMISSIONS, /*server config permissions*/
			0,0,0,	/*user desc, client config, and server config handles*/
			NULL /* presentation format */
		},
		{
			0, /* handle stored here */
			{ AT_BLE_UUID_16, {DISS_CHAR_UUID_SOFTWARE_REVISION_STRING}}, /* UUID */
			AT_BLE_CHAR_READ, /* Properties */
			(uint8_t *)"ATMEL4", strlen("ATMEL4"), 100, /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR,
			NULL, 0, 0, /* user defined name */
			AT_BLE_ATTR_NO_PERMISSIONS, /*user description permissions*/
			AT_BLE_ATTR_READABLE_REQ_AUTHN_REQ_AUTHR, /*client config permissions*/
			AT_BLE_ATTR_NO_PERMISSIONS, /*server config permissions*/
			0,0,0,	/*user desc, client config, and server config handles*/
			NULL /* presentation format */
		}
	};

	status = at_ble_primary_service_define(&service_uuid, &service, NULL, 0, chars, 6);	
	if(AT_BLE_SUCCESS != status)
	{
		atmel_printf("diss_init / at_ble_primary_service_define Error(%d) !!!!!\r\n", status);
		return;
	}
}

static void app_init(void)
{
	at_ble_status_t ble_status;
	
	// init device
	ble_status = at_ble_init(NULL);
	if(AT_BLE_SUCCESS == ble_status)
	{
		at_ble_addr_t addr = {AT_BLE_ADDRESS_PUBLIC, {0xa4, 0xb4, 0xc4, 0xd9, 0xe9, 0xf9} };

		ble_status = at_ble_addr_set(&addr);
		if(AT_BLE_SUCCESS != ble_status)
		{
			atmel_printf("app_init / at_ble_addr_set Error(%d) !!!!!\r\n", ble_status);
			return;
		}


		//Set the device name
		ble_status = at_ble_device_name_set((uint8_t*)HT_DEVICE_NAME_DATA, HT_DEVICE_NAME_LEN);
		if(AT_BLE_SUCCESS == ble_status)
		{
			
			diss_init();

			// Device Information Service
			/////////////////////////////////////////////////////////
			// Health Thermometer Service

			/* Initialize the htpt to default value */
			htpt_init(&htpt_data);

			/* Register the Initialized value into htpt profile */
			at_ble_htpt_create_db(htpt_data.optional,
								htpt_data.temperature_type,
								htpt_data.min_measurement_intv,
								htpt_data.max_meaurement_intv,
								htpt_data.measurement_interval,
								htpt_data.security_lvl);

			adv_init();
					
			// start advertising
			at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL,
							AT_BLE_ADV_FP_ANY, APP_HT_FAST_ADV, APP_HT_ADV_TIMEOUT, 0);
		}
		else
			atmel_printf("app_init / at_ble_device_name_set Error(%d) !!!!!\r\n", ble_status);
			
	}
	else
		atmel_printf("app_init / at_ble_init Error(%d) !!!!!\r\n", ble_status);
		
}

static void htpt_temperature_send(htpt_app_t *htpt_temp, stable_temp_reading isStable)
{
	at_ble_status_t ble_status;
	at_ble_prf_date_time_t timestamp;
	float temperature;

	// Read Temperature Value from IO1 Xplained Pro 
	temperature = at30tse_read_temperature();
	
	if (htpt_temp->flags & HTPT_FLAG_FAHRENHEIT)
	{
		temperature = (((temperature * 9.0)/5.0) + 32.0);
	}
	
	timestamp.year = 15;
	timestamp.month = 8;
	timestamp.day = 1;
	timestamp.hour = 9;
	timestamp.min = 2;
	timestamp.sec = 36;

	ble_status = at_ble_htpt_temp_send((uint32_t)temperature,
									&timestamp,
									htpt_temp->flags,
									htpt_temp->temperature_type,
									isStable);
	if(ble_status != AT_BLE_SUCCESS)
		atmel_printf("htpt_temperature_send / at_ble_htpt_temp_send Error(%d) !!!!!\r\n", ble_status);

	atmel_printf("htpt_temperature_send / at_ble_htpt_temp_send / %f\r\n", temperature);
}


static void timer_callback_handler()
{
	htpt_temperature_send(&htpt_data, STABLE_TEMPERATURE_VAL);	
	//send_plf_int_msg_ind(USER_TIMER_CALLBACK,TIMER_EXPIRED_CALLBACK_TYPE_DETECT,NULL,0);
}

static void configure_timer(void)
{
	//timer_drv_status timer_status;
	//
	//timer_status = timer_init();
	//if(TIMER_STATUS_OK != timer_status)
	//{
		//atmel_printf("configure_timer / timer_init Error(%d) !!!!!\r\n", timer_status);
		//while(1)
		//{
//
		//}
	//}
//
	//timer_status = timer_register_callback(timer_callback_handler);
	//if(TIMER_STATUS_OK != timer_status)
	//{
		//atmel_printf("configure_timer / timer_register_callback Error(%d) !!!!!\r\n", timer_status);
		//while(1)
		//{
//
		//}
	//}
	//
	

//! [setup_timer_1]
struct timer_config config_timer;
//! [setup_timer_1]
//! [setup_timer_2]
timer_get_config_defaults(&config_timer);
//! [setup_timer_2]
//! [setup_timer_3]
config_timer.reload_value = CONF_TIMER_RELOAD_VALUE;
//! [setup_timer_3]
//! [setup_timer_4]
timer_init(&config_timer);
//! [setup_timer_4]
//! [setup_timer_5]

//! [setup_timer_5


	//! [setup_register_callback]
	timer_register_callback(timer_callback_handler);
	//! [setup_register_callback]
	
	/* For A4, timer0 IRQ is 9 */
	//! [enable_IRQ]
	NVIC_EnableIRQ(9);
	//! [enable_IRQ




	
}













/*
for uart
*/
//! [module_inst]
struct uart_module uart_instance;
//! [module_inst]

//! [variable_inst]
static uint8_t string_input[8];
volatile static bool read_complete_flag = false;
volatile static bool write_complete_flag = false;
//! [variable_inst]

//! [callback_functions]
static void uart_read_complete_callback(struct uart_module *const module)
{
	read_complete_flag = true;
}

static void uart_write_complete_callback(struct uart_module *const module)
{
	write_complete_flag = true;
}
//! [callback_functions]



//! [setup]
static void configure_uart(void)
{
	//! [setup_config]
	struct uart_config config_uart;
	//! [setup_config]
	//! [setup_config_defaults]
	uart_get_config_defaults(&config_uart);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_uart.baud_rate = 38400;
	config_uart.pinmux_pad[0] = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_uart.pinmux_pad[1] = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_uart.pinmux_pad[2] = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_uart.pinmux_pad[3] = EDBG_CDC_SERCOM_PINMUX_PAD3;
	//! [setup_change_config]

	//! [setup_set_config]
	while (uart_init(&uart_instance,
	EDBG_CDC_MODULE, &config_uart) != STATUS_OK) {
	}
	//! [setup_set_config]
}
//! [setup]


/*
for uart
*/















// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
	int i=0, j=len-1, temp;
	while (i<j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x)
	{
		str[i++] = (x%10) + '0';
		x = x/10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}




// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0)
	{
		res[i] = '.';  // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter is needed
		// to handle cases like 233.007
		//fpart = fpart * pow(10, afterpoint);
		fpart = fpart * 0;
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}










void main()
{
	uint16_t plf_event_type;
	uint16_t plf_event_data_len;
	uint8_t	plf_event_data[16];
	at_ble_events_t event;
	at_ble_handle_t handle;
	uint8_t params[100];
	at_ble_LTK_t app_bond_info;
	
	memset(&app_bond_info, 0, sizeof(at_ble_LTK_t));
	memset(params, 0, sizeof(uint8_t) * 100);
	
	
	
	
		memset(string_input,0,sizeof(string_input));
		read_complete_flag = false;
		write_complete_flag = false;
	
	
		system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	
		configure_uart();

	uint8_t string1[] = " Hello World\r\n";
	uart_write_buffer_wait(&uart_instance, string1, sizeof(string1));
	
	
	app_device_bond = false;
	auth_info = 0;
	memset(&htpt_data, 0, sizeof(htpt_app_t));

	isEnableTempMeasurement = 0;
	isEnableIntermediateTemp = 0;
	isEnableMeasInterval = 0;

	//Init Platform_driver
	//platform_driver_init();

	at30tse_init();
	/* Set 12-bit resolution mode. */
	at30tse_write_config_register(AT30TSE_CONFIG_RES(AT30TSE_CONFIG_RES_12_bit));


	configure_timer();

	atmel_printf("\r\n\r\napp_entry ======================= 42\r\n");

	app_init();

	uint8_t string2[] = " Hello World2\r\n";
	uart_write_buffer_wait(&uart_instance, string2, sizeof(string2));

   char res[20];
   float n = at30tse_read_temperature();
   ftoa(n, res, 4);
   uart_write_buffer_wait(&uart_instance, res, sizeof(20));




	while(at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
	{
		
			uint8_t string3[] = " Hello World3\r\n";
		uart_write_buffer_wait(&uart_instance, string3, sizeof(string3));
		
		
   char res[20];
   float n = at30tse_read_temperature();
   ftoa(n, res, 4);
   uart_write_buffer_wait(&uart_instance, res, sizeof(20));
		
		switch(event)
		{
			//case AT_PLATFORM_EVENT:
			//{
				//platform_event_get(&plf_event_type,plf_event_data,&plf_event_data_len);
				//if(plf_event_type == ((TIMER_EXPIRED_CALLBACK_TYPE_DETECT << 8)| USER_TIMER_CALLBACK)) {
					//htpt_temperature_send(&htpt_data, STABLE_TEMPERATURE_VAL);
					////timer_start(htpt_data.measurement_interval,1);
				//}
			//}
			//break;
			case AT_BLE_CONNECTED:
			{
				at_ble_connected_t connected_params;
				memcpy((uint8_t *)&connected_params, params, sizeof(at_ble_connected_t));

				atmel_printf("AT_BLE_CONNECTED / %d, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, %d, %d\r\n",
					connected_params.peer_addr.type, connected_params.peer_addr.addr[0], connected_params.peer_addr.addr[1],
					connected_params.peer_addr.addr[2], connected_params.peer_addr.addr[3], connected_params.peer_addr.addr[4],
					connected_params.peer_addr.addr[5], connected_params.handle, connected_params.conn_status);

				isEnableTempMeasurement = 0;
				isEnableIntermediateTemp = 0;
				isEnableMeasInterval = 0;

 				if(connected_params.conn_status == AT_BLE_SUCCESS)
				{
					handle = connected_params.handle;
		 			 /* Enable the HTPT Profile */
					at_ble_htpt_enable(handle, HTPT_CFG_STABLE_MEAS_IND|HTPT_CFG_INTERM_MEAS_NTF|HTPT_CFG_MEAS_INTV_IND);
				}
			}
			break;

			case AT_BLE_DISCONNECTED:
			{
				at_ble_disconnected_t disconnected_params;
				at_ble_status_t status = AT_BLE_SUCCESS;
				memcpy((uint8_t *)&disconnected_params, params, sizeof(at_ble_disconnected_t));

				atmel_printf("AT_BLE_DISCONNECTED / %d, %d\r\n",
					disconnected_params.handle, disconnected_params.reason);

				if(isEnableTempMeasurement)
					timer_disable();
					//timer_stop();

				isEnableTempMeasurement = 0;
				isEnableIntermediateTemp = 0;
				isEnableMeasInterval = 0;

				adv_init();
				status = at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL,
								AT_BLE_ADV_FP_ANY, APP_HT_FAST_ADV, APP_HT_ADV_TIMEOUT, 0);
				
				atmel_printf("status = %d\r\n", status);
			}
			break;

			case AT_BLE_CHARACTERISTIC_CHANGED:
			{
				at_ble_characteristic_changed_t characteristic_changed_params;				 	
				memcpy((uint8_t *)&characteristic_changed_params, params, sizeof(at_ble_characteristic_changed_t));

				atmel_printf("AT_BLE_CHARACTERISTIC_CHANGED /\r\n");
			}
			break;

			/* HTPT Health Thermometer Profile events */
			/** Inform APP of database creation status */
			case AT_BLE_HTPT_CREATE_DB_CFM:	
			{
				at_ble_htpt_create_db_cfm_t htpt_create_db_cfm_params;
				memcpy((uint8_t *)&htpt_create_db_cfm_params, params, sizeof(at_ble_htpt_create_db_cfm_t));

				atmel_printf("AT_BLE_HTPT_CREATE_DB_CFM / %d\r\n",
					htpt_create_db_cfm_params.status);

				atmel_printf("Please find %s device\r\n",
					HT_ADV_DATA_NAME_DATA);				
			}
			break;

			/** Error indication to APP*/
			case AT_BLE_HTPT_ERROR_IND:
			{
				at_ble_prf_server_error_ind_t prf_htpt_error_ind_params;
				memcpy((uint8_t *)&prf_htpt_error_ind_params, params, sizeof(at_ble_prf_server_error_ind_t));

				atmel_printf("AT_BLE_HTPT_ERROR_IND / %d, %d, %d\r\n",
					prf_htpt_error_ind_params.conhdl,
					prf_htpt_error_ind_params.msg_id,
					prf_htpt_error_ind_params.status);				
			}					
			break;

			/** Automatically sent to the APP after a disconnection with the peer device to confirm disabled profile*/
			case AT_BLE_HTPT_DISABLE_IND:
			{
				at_ble_htpt_disable_ind_t htpt_disable_ind_params;
				memcpy((uint8_t *)&htpt_disable_ind_params, params, sizeof(at_ble_htpt_disable_ind_t));

				if(isEnableTempMeasurement)
					timer_disable();
					//timer_stop();

				isEnableTempMeasurement = 0;
				isEnableIntermediateTemp = 0;
				isEnableMeasInterval = 0;

				atmel_printf("AT_BLE_HTPT_DISABLE_IND / %d, %d, %d, %d, %d\r\n",
					htpt_disable_ind_params.conhdl,
					htpt_disable_ind_params.temp_meas_ind_en,
					htpt_disable_ind_params.interm_temp_ntf_en,
					htpt_disable_ind_params.meas_intv_ind_en,
					htpt_disable_ind_params.meas_intv);
			}
			break;
			
			/** Temperature value confirm to APP*/
			case AT_BLE_HTPT_TEMP_SEND_CFM:
			{
				at_ble_htpt_temp_send_cfm_t htpt_temp_send_cfm_params;
				memcpy((uint8_t *)&htpt_temp_send_cfm_params, params, sizeof(at_ble_htpt_temp_send_cfm_t));

				atmel_printf("HTPT_TEMP_SEND_CFM / %d, %d, cfm_type %d\r\n",
					htpt_temp_send_cfm_params.conhdl, htpt_temp_send_cfm_params.status, htpt_temp_send_cfm_params.cfm_type);

// 				timer_drv_status timer_status;
// 				timer_status = timer_start(htpt_data.measurement_interval, 1);
// 				if(timer_status != TIMER_STATUS_OK)
// 					atmel_printf("AT_BLE_HTPT_CFG_INDNTF_IND / timer_start Error(%d) !!!!!\r\n", timer_status);
				
				timer_enable();
				
				atmel_printf("AT_BLE_HTPT_CFG_INDNTF_IND / measurement_interval %d\r\n", htpt_data.measurement_interval);					
			}
			break;

			/** Inform APP of new measurement interval value */
			case AT_BLE_HTPT_MEAS_INTV_CHG_IND:
			{
				at_ble_htpt_meas_intv_chg_ind_t htpt_meas_intv_chg_ind_params;
				memcpy((uint8_t *)&htpt_meas_intv_chg_ind_params, params, sizeof(at_ble_htpt_meas_intv_chg_ind_t));

				atmel_printf("AT_BLE_HTPT_MEAS_INTV_CHG_IND / %d\r\n",
					htpt_meas_intv_chg_ind_params.intv);

				htpt_data.measurement_interval = htpt_meas_intv_chg_ind_params.intv;							
			}
			break;

			/** Inform APP of new configuration value*/
			case AT_BLE_HTPT_CFG_INDNTF_IND:
			{
				at_ble_htpt_cfg_indntf_ind_t htpt_cfg_indntf_ind_params;
				memcpy((uint8_t *)&htpt_cfg_indntf_ind_params, params, sizeof(at_ble_htpt_cfg_indntf_ind_t));

				atmel_printf("AT_BLE_HTPT_CFG_INDNTF_IND / %d, %d, %d\r\n",
					htpt_cfg_indntf_ind_params.conhdl, htpt_cfg_indntf_ind_params.cfg_val, htpt_cfg_indntf_ind_params.char_code);

				if(htpt_cfg_indntf_ind_params.char_code == HTPT_TEMP_MEAS_CHAR && htpt_cfg_indntf_ind_params.cfg_val == 2)
				{
					isEnableTempMeasurement = 1;
					
					//timer_drv_status timer_status;
					//timer_status = timer_start(htpt_data.measurement_interval, 1);
					//if(timer_status != TIMER_STATUS_OK)
						//atmel_printf("AT_BLE_HTPT_CFG_INDNTF_IND / timer_start Error(%d) !!!!!\r\n", timer_status);
						
					timer_enable();	

					atmel_printf("AT_BLE_HTPT_CFG_INDNTF_IND / measurement_interval %d\r\n", htpt_data.measurement_interval);					
				}
				else if(htpt_cfg_indntf_ind_params.char_code == HTPT_TEMP_MEAS_CHAR && htpt_cfg_indntf_ind_params.cfg_val == 0)
				{
					isEnableTempMeasurement = 0;

					timer_disable();
					
					atmel_printf("AT_BLE_HTPT_CFG_INDNTF_IND / timer_stop\r\n");					
				}
				else if(htpt_cfg_indntf_ind_params.char_code == HTPT_INTERM_TEMP_CHAR && htpt_cfg_indntf_ind_params.cfg_val == 1)
				{
					//Typical update intervals range from 0.25 seconds to 2 seconds.
					isEnableIntermediateTemp = 1;
				}
				else if(htpt_cfg_indntf_ind_params.char_code == HTPT_INTERM_TEMP_CHAR && htpt_cfg_indntf_ind_params.cfg_val == 0)
				{
					//Stop sending 'Intermediate Temperature'
					isEnableIntermediateTemp = 0;
				}
				else if(htpt_cfg_indntf_ind_params.char_code == HTPT_MEAS_INTV_CHAR && htpt_cfg_indntf_ind_params.cfg_val == 2)
				{
					//Start sending 'Measurement Interval'
					isEnableMeasInterval = 1;
				}
				else if(htpt_cfg_indntf_ind_params.char_code == HTPT_MEAS_INTV_CHAR && htpt_cfg_indntf_ind_params.cfg_val == 0)
				{
					//Stop sending 'Measurement Interval'
					isEnableMeasInterval = 0;
				}
			}
			break;
			
			case AT_BLE_PAIR_REQUEST:
			{
				at_ble_pair_request_t pair_request_params;
				memcpy((uint8_t *)&pair_request_params, params, sizeof(at_ble_pair_request_t));

				atmel_printf("AT_BLE_PAIR_REQUEST /\r\n");

				at_ble_pair_features_t features;
				uint8_t i = 0;
				char bond;

				//atmel_printf("Remote device request pairing\r\n");

				/* Check if we are already bonded (Only one bonded connection is supported
				in this example)*/
				if(app_device_bond)
				{
					//atmel_printf("Bound relation exists with previously peer device\r\n");
					//atmel_printf("To remove existing bonding information and
					//atmel_printf("accept pairing request from peer device press y else press n : \r\n");

					do
					{
						bond = 'y';//getchar();						
						if((bond == 'Y') || (bond == 'y'))
						{
							app_device_bond = false;
							break;
						}
						else if ((bond == 'N') || (bond == 'n'))
						{
							//atmel_printf("Pairing failed\r\n");
							break;
						}
						else
						{
							//atmel_printf("Wrong value entered please try again : \r\n");
						}
					}while(app_device_bond);
				}

				if(!app_device_bond)
				{
					/* Authentication requirement is bond and MITM*/
					features.desired_auth =  AT_BLE_MODE1_L1_NOAUTH_PAIR_ENC;
					features.bond = false;
					features.mitm_protection = false;
					features.oob_avaiable = false;
					/* Device capabilities is display only , key will be generated 
					and displayed */
					features.io_cababilities = AT_BLE_IO_CAP_NO_INPUT_NO_OUTPUT;
					/* Distribution of LTK is required */
					features.initiator_keys =   AT_BLE_KEY_DIST_ENC;
					features.responder_keys =   AT_BLE_KEY_DIST_ENC;
					features.max_key_size = 16;
					features.min_key_size = 16;

					/* Generate LTK */
					for(i=0 ; i<8 ;i++)
					{
						app_bond_info.key[i] = rand()&0x0f;
						app_bond_info.nb[i] = rand()&0x0f;
					}

					for(i=8 ; i<16 ;i++)
					{
						app_bond_info.key[i] = rand()&0x0f;
					}

					app_bond_info.ediv = rand()&0xffff; 
					app_bond_info.key_size = 16;

					/* Send pairing response */
					if(at_ble_authenticate(handle, &features, &app_bond_info, NULL) != AT_BLE_SUCCESS)
					{
						features.bond = false;
						features.mitm_protection = false;
						at_ble_authenticate(handle, &features, NULL, NULL);
					}
				}
			}	
			break;
		
			case AT_BLE_PAIR_KEY_REQUEST:
			{
				/* Passkey has fixed value in this example MSB */
				uint8_t passkey[6]={0,0,0,0,0,0};
				uint8_t passkey_ascii[6];
				uint8_t i = 0;
	
				at_ble_pair_key_request_t pair_key_request;
				memcpy((uint8_t *)&pair_key_request, params, sizeof(at_ble_pair_key_request_t));
				/* Display passkey */
				if(pair_key_request.passkey_type == AT_BLE_PAIR_PASSKEY_DISPLAY)
				{
					/* Convert passkey to ASCII format */
					for(i=0; i<AT_BLE_PASSKEY_LEN ; i++)
					{
						passkey_ascii[i] = (passkey[i] + 48); 
					}
					at_ble_pair_key_reply(pair_key_request.handle,pair_key_request.type,passkey_ascii);
				}
			
			}
			break;

			case AT_BLE_PAIR_DONE:
			{
				at_ble_pair_done_t pair_done_params;
				memcpy((uint8_t *)&pair_done_params, params, sizeof(at_ble_pair_done_t));				

				atmel_printf("AT_BLE_PAIR_DONE /\r\n");

				if(pair_done_params.status == AT_BLE_SUCCESS)
				{
					app_device_bond = true;
					auth_info = pair_done_params.auth;
					handle = pair_done_params.handle;

					/* Enable the HTPT Profile */
					at_ble_htpt_enable(handle, HTPT_CFG_STABLE_MEAS_IND|HTPT_CFG_INTERM_MEAS_NTF|HTPT_CFG_MEAS_INTV_IND);
				}
			}
			break;

			case AT_BLE_ENCRYPTION_REQUEST:
			{
				bool key_found = false;

				at_ble_encryption_request_t enc_req;
				memcpy((uint8_t *)&enc_req, params, sizeof(at_ble_encryption_request_t));

				/* Check if bond information is stored */
				if((enc_req.ediv == app_bond_info.ediv)
					&& !memcmp(&enc_req.nb[0],&app_bond_info.nb[0],8))
				{
					key_found = true;
				}

				at_ble_encryption_request_reply(handle, auth_info, key_found, app_bond_info);
			}
			break;

			case AT_BLE_ENCRYPTION_STATUS_CHANGED:
			{
				at_ble_encryption_status_changed_t enc_status;
				memcpy((uint8_t *)&enc_status, params, sizeof(at_ble_encryption_status_changed_t));

				if(enc_status.status == AT_BLE_SUCCESS)
				{
					handle = enc_status.handle;
					at_ble_htpt_enable(handle, HTPT_CFG_STABLE_MEAS_IND);
				}
			}
			break;


			default:
			break;
		}
	}
}	


