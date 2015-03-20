/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "htpt_app.h"
#include "profiles.h"

#define DBG_LOG printf
#define DBG_LOG_1LVL printf

#define APP_HT_FAST_ADV 100 //100 ms

#define APP_HT_ADV_TIMEOUT 1000 // 100 Secs



#if 0
static uint8_t adv_data[] = {0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15, 0x21, 0x8A,
	                         0xF6, 0x52, 0x73, 0xE3, 0x40, 0xB3, 0xB4, 0x1C,
	                         0x19, 0x53, 0x24, 0x2C, 0x72, 0xf4, 0x00, 0xbb,
                             0x00, 0x44, 0xc5};

static uint8_t scan_rsp_data[] = {0x11, 0x07, 0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00,
	                              0x37, 0xaa, 0xe3, 0x11, 0x2a, 0xdc, 0x00, 0xcd,
                                  0x30, 0x57};

#else

#define SCAN_RESP_LEN 10
#define ADV_DATA_LEN 18
								  
static uint8_t adv_data[ADV_DATA_LEN] = {0x03, 0x03, 0x09, 0x18, 0x03, 0x19, 0x00, 0x03, 0x09, 0x09, 
							'A', 'T', 'M', 'E', 'L', 'B', 'L', 'E'};

//static uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xFF,0x00,0x60,0x52,0x57,0x2D,0x42,0x4C,0x45};
static uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xFF, 0x00, 0x06, 0x25, 0x75, 0x11, 0x6a, 0x7f, 0x7f};
	

								  
#endif


at_ble_LTK_t app_bond_info;
bool app_device_bond = false;
uint8_t auth_info = 0;

htpt_app_t htpt_data;
volatile uint16_t gtc_count = false;

void app_init(void);
void htpt_init(htpt_app_t *htpt_temp);
void htpt_temperature_send(htpt_app_t *htpt_temp);

void init_timer(void);
void tc_start_timer(void);
void tc_stop_timer(void);
static void configure_console(void);

void app_init(void)
{
	uint8_t port = 74;
	at_ble_addr_t addr = {AT_BLE_ADDRESS_PUBLIC,
		{0x25, 0x75, 0x11, 0x6a, 0x7f, 0x7f} };

	// init device
	at_ble_init(&port);

	at_ble_addr_set(&addr);

#if 0
	// establish prephiral database
	at_ble_primary_service_define(&service_uuid, &service,
		NULL, 0, chars, 2);
		
	at_ble_adv_data_set(adv_data, sizeof(adv_data), scan_rsp_data, sizeof(scan_rsp_data));
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, 100, 1000, 0) != AT_BLE_SUCCESS)
	{
		while(1);
	}
#endif
		
	/* Initialize the htpt to default value */
	htpt_init(&htpt_data);

#if 1	
	/* Register the Initialized value into htpt profile */
	if(at_ble_htpt_create_db(
				htpt_data.optional,
				htpt_data.temperature_type,
				htpt_data.min_measurement_intv,
				htpt_data.max_meaurement_intv,
				htpt_data.measurement_interval,
				htpt_data.security_lvl
				) == AT_BLE_FAILURE)
				{
					while(1);
				}
#endif
}

double temp_res;

int main (void)
{
	at_ble_events_t event;
	uint8_t params[512];

	at_ble_handle_t handle;

#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
#elif SAM0
	system_init();
#endif
	 
	configure_console();
	
	DBG_LOG_1LVL("\r\n Initializing HTPT Application");

	at30tse_init();
	
	at30tse_write_config_register(
			AT30TSE_CONFIG_RES(AT30TSE_CONFIG_RES_12_bit));

	
	//platform_init(NULL);
	init_timer();
	app_init();
	
	DBG_LOG_1LVL("\r\nHTPT Initialization completed. Waiting for Event");
	
	while(at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
	{
		switch(event)
		{
			case AT_BLE_CONNECTED:
			{
				at_ble_connected_t* conn_params = (at_ble_connected_t*)params;
				
				DBG_LOG("\r\nDevice connected to 0x%02x%02x%02x%02x%02x%02x handle=0x%x",
					conn_params->peer_addr.addr[5],
					conn_params->peer_addr.addr[4],
					conn_params->peer_addr.addr[3],
					conn_params->peer_addr.addr[2],
					conn_params->peer_addr.addr[1],
					conn_params->peer_addr.addr[0],
					conn_params->handle);
				handle = conn_params->handle;
				LED_On(LED0);
				
				/* Enable the HTPT Profile */
				if(at_ble_htpt_enable(handle, HTPT_CFG_STABLE_MEAS_IND) == AT_BLE_FAILURE)
				{
					DBG_LOG("\r\nFailure in HTPT Profile Enable");
				}
				else
				{
					//tc_start_timer();
				}							
			}
			break;

			case AT_BLE_DISCONNECTED:
			{
				at_ble_disconnected_t *disconnect = (at_ble_disconnected_t *)params;
				tc_stop_timer();
				LED_Off(LED0);	
				DBG_LOG("\r\nDevice disconnected Reason:0x%02, handle=0x%x", disconnect->reason, disconnect->handle);
				
				if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, 
				                   APP_HT_FAST_ADV, APP_HT_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
				{
					DBG_LOG_1LVL("\r\nBLE Adv start Failed");
				}
				else
				{
					DBG_LOG_1LVL("\r\nBLE Started Adv");
				}								
												
			}
			break;
			
			case AT_BLE_CHARACTERISTIC_CHANGED:
			{
				at_ble_characteristic_changed_t* change_params
					= (at_ble_characteristic_changed_t*) params;
				uint32_t i = 0;

				DBG_LOG("Characteristic 0x%x changed, new_value = ", 
					change_params->char_handle);
				for(i=0; i<change_params->char_len; i++)
					DBG_LOG("0x%02x ", change_params->char_new_value[i]);
				DBG_LOG("\n");
			}
			break;	
			
			/* HTPT Health Thermometer Profile events */
			
			/** Inform APP of database creation status */
			case AT_BLE_HTPT_CREATE_DB_CFM:	
			{	
				at_ble_htpt_create_db_cfm_t *create_db_params = (at_ble_htpt_create_db_cfm_t *)params;				
				// start advertising
				DBG_LOG("\r\nCreating HTPT DB: SUCCESS: Status=0x%x", create_db_params->status);
				at_ble_adv_data_set(adv_data, ADV_DATA_LEN, scan_rsp_data, SCAN_RESP_LEN);
				if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, 
				                   APP_HT_FAST_ADV, APP_HT_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
				{
					DBG_LOG("\r\nBLE Adv start Failed");
				}
				else
				{
					DBG_LOG("\r\nBLE Started Adv");
				}								
			}
			break;
			
			/** Error indication to APP*/
			case AT_BLE_HTPT_ERROR_IND:
			{
				prf_server_error_ind_t *prf_htpt_error_ind = (prf_server_error_ind_t *)params;
				
				DBG_LOG("\r\n HTPT Error Indication received, msg_id=0x%x, handle=0x%x, status=0x%x",
				prf_htpt_error_ind->msg_id, prf_htpt_error_ind->conhdl, prf_htpt_error_ind->status);
			}					
			break;
			
			/** Automatically sent to the APP after a disconnection with the peer device to confirm disabled profile*/
			case AT_BLE_HTPT_DISABLE_IND:
			{
				at_ble_htpt_disable_ind_t *htpt_disable_ind_params = (at_ble_htpt_disable_ind_t *) params;
				DBG_LOG("\r\nHTPT Disable Indication: conhdl=0x%x, interm_temp_ntf_en=0x%x, meas_intv=0x%x, meas_intv_ind_en=0x%x, handle=0x%x",
				htpt_disable_ind_params->conhdl,
				htpt_disable_ind_params->interm_temp_ntf_en,
				htpt_disable_ind_params->meas_intv,
				htpt_disable_ind_params->meas_intv_ind_en,
				htpt_disable_ind_params->temp_meas_ind_en);
			}
			break;
			
			/** Temperature value confirm to APP*/
			case AT_BLE_HTPT_TEMP_SEND_CFM:
			{
				at_ble_htpt_temp_send_cfm_t *htpt_send_temp_cfm_params = (at_ble_htpt_temp_send_cfm_t *)params;
				DBG_LOG("\r\n HTPT Temperature Send Confirm: cfm_type=0x%x, conhdl=0x%x, status=0x%x,",
					htpt_send_temp_cfm_params->cfm_type, htpt_send_temp_cfm_params->conhdl,
					htpt_send_temp_cfm_params->status);	
				gtc_count = false;	
				tc_start_timer();	
				while(gtc_count == false);
				htpt_temperature_send(&htpt_data);
				tc_stop_timer();
			}
			break;
			
			/** Inform APP of new measurement interval value */
			case AT_BLE_HTPT_MEAS_INTV_CHG_IND:
			{
				at_ble_htpt_meas_intv_chg_ind_t *htpt_meas_intv_chg_params = (at_ble_htpt_meas_intv_chg_ind_t *)params;	
				DBG_LOG("\r\nHTPT measure Interval change Indication: Interval=%d", htpt_meas_intv_chg_params->intv);
				htpt_data.measurement_interval = htpt_meas_intv_chg_params->intv;							
			}
			break;
			
			/** Inform APP of new configuration value*/
			case AT_BLE_HTPT_CFG_INDNTF_IND:
			{
				at_ble_htpt_cfg_indntf_ind_t * htpt_cfg_indntf_ind_params = (at_ble_htpt_cfg_indntf_ind_t *)params;
				DBG_LOG("\r\nHTPT Cfg indication notification indication cfg_val=0x%x, char code=0x%x, conhdl=0x%x", htpt_cfg_indntf_ind_params->cfg_val,
				htpt_cfg_indntf_ind_params->char_code, htpt_cfg_indntf_ind_params->conhdl);
				if (htpt_cfg_indntf_ind_params->char_code == HTPT_TEMP_MEAS_CHAR &&  htpt_cfg_indntf_ind_params->cfg_val == 2)
				{
					htpt_temperature_send(&htpt_data);
				}				
			}			
			break;
			
			case AT_BLE_PAIR_REQUEST:
			{
				at_ble_pair_features_t features;
				uint8_t i = 0;
				char bond;
	
				DBG_LOG("\r\nRemote device request pairing");
			
				/* Check if we are already bonded (Only one bonded connection is supported
				in this example)*/
				if(app_device_bond)
				{
					DBG_LOG("\r\nBound relation exists with previously peer device");
					DBG_LOG("\r\nTo remove exsisting bonding information and accept pairing request from peer device press y else press n : ");
					do
					{
						bond = getchar();						
						if((bond == 'Y') || (bond == 'y'))
						{
							app_device_bond = false;
							break;
						}
						else if ((bond == 'N') || (bond == 'n'))
						{
							DBG_LOG("\r\nPairing failed \n");
							break;
						}
						else
						{
							DBG_LOG("\r\nWrong value entered please try again : \n");
						}
					}while(app_device_bond);

				}
			
				if(!app_device_bond)
				{
					/* Authentication requirement is bond and MITM*/
					features.desired_auth =  AT_BLE_MODE1_L2_AUTH_PAIR_ENC;
					features.bond = true;
					features.mitm_protection = true;
					features.oob_avaiable = false;
					/* Device cababilities is display only , key will be generated 
					and displayed */
					features.io_cababilities = AT_BLE_IO_CAP_DISPLAY_ONLY;
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
					DBG_LOG("\r\n Sending pairing response handle=0x%x",
							handle);
					at_ble_authenticate(handle, &features,&app_bond_info,NULL,NULL);
					
				}
			}	
			break;
		
			case AT_BLE_PAIR_KEY_REQUEST:
			{
				/* Passkey has fixed value in this example MSB */
				uint8_t passkey[6]={0,0,0,0,0,0};
				uint8_t passkey_ascii[6];
				uint8_t i = 0;
	
				at_ble_pair_key_request_t* pair_key_request = (at_ble_pair_key_request_t*)params;
				/* Display passkey */
				if(pair_key_request->passkey_type == AT_BLE_PAIR_PASSKEY_DISPLAY)
				{
					/* Convert passkey to ASCII format */
					for(i=0; i<AT_BLE_PASSKEY_LEN ; i++)
					{
						passkey_ascii[i] = (passkey[i] + 48); 
					}
					DBG_LOG("\r\nplease enter the following code on the other device : ");
					for(i=0; i<AT_BLE_PASSKEY_LEN ; i++)
					{
						DBG_LOG("%c",passkey_ascii[i]);
					}
					DBG_LOG("\n");	
					at_ble_pair_key_reply(pair_key_request->handle,pair_key_request->type,passkey_ascii);
				}
			
			}
			break;

			case AT_BLE_PAIR_DONE:
			{
				at_ble_pair_done_t* pair_params 
					= (at_ble_pair_done_t*) params;
				if(pair_params->status == AT_BLE_SUCCESS)
				{
					DBG_LOG("\r\nPairing procedure completed successfully \n");
					app_device_bond = true;
					auth_info = pair_params->auth;
				}
				else
				{
					DBG_LOG("\r\nPairing failed \n");
				}
			}
			break;

			case AT_BLE_ENCRYPTION_REQUEST:
			{
				bool key_found = false;
	
				at_ble_encryption_request_t* enc_req 
					= (at_ble_encryption_request_t* )params;
			
				/* Check if bond information is stored */
				if((enc_req-> ediv == app_bond_info.ediv)
					&& !memcmp(&enc_req->nb[0],&app_bond_info.nb[0],8))
				{
					key_found = true;
				}
			
				at_ble_encryption_request_reply(handle,auth_info ,key_found,app_bond_info);
			}
			break;
		
			case AT_BLE_ENCRYPTION_STATUS_CHANGED:
			{
				at_ble_encryption_status_changed_t* enc_status 
					= (at_ble_encryption_status_changed_t*)params; 
				if(enc_status->status == AT_BLE_SUCCESS)
				{
					DBG_LOG("\r\nEncryption completed successfully \n");
				}
				else
				{
					DBG_LOG("\r\nEncryption failed \n");
				}
			}
			break;
			
			default:
			 LED_Off(LED0);
			 DBG_LOG("\r\nUnknown event received: event=0x%x", event);
			break;
		}
	}
}


void htpt_init(htpt_app_t *htpt_temp)
{
	/* Initialize to default temperature value  and htpt parameters*/
	htpt_temp->measurement_interval = 1; 
	htpt_temp->temperature = 3700;
	htpt_temp->temperature_type = HTP_TYPE_BODY;
	htpt_temp->max_meaurement_intv = 30;
	htpt_temp->min_measurement_intv = 1;
	htpt_temp->security_lvl = HTPT_ENABLE;
	htpt_temp->optional = HTPT_ALL_FEAT_SUP;	
}

void htpt_temperature_send(htpt_app_t *htpt_temp)
{
	static uint32_t temperature;
	struct prf_date_time timestamp; 
	/* Read Temperature Value from IO1 Xplained Pro */
	temp_res = at30tse_read_temperature();
	timestamp.day = 1;
	timestamp.hour = 9;
	timestamp.min = 2;
	timestamp.month = 8;
	timestamp.sec = 36;
	timestamp.year = 15;	
	temperature += 10;
	temperature |= 0xFE000000;
	at_ble_htpt_temp_send(temperature,
	                     &timestamp,
						 HTPT_FLAG_CELSIUS | HTPT_FLAG_TYPE,
						 htpt_temp->temperature_type,
						 STABLE_TEMPERATURE_VAL
						 );
}

#define CONF_TC_MODULE TC3
#define TC_COUNT_1SEC  (8000000ul/1024ul)
struct tc_module tc_instance;
void tc_cc0_cb(struct tc_module *const module_inst);

void init_timer(void)
{
	struct tc_config config_tc;

	tc_get_config_defaults(&config_tc);

	config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
	config_tc.clock_source = GCLK_GENERATOR_0;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1024;
	config_tc.counter_8_bit.period = 0;
	config_tc.counter_16_bit.compare_capture_channel[0] = TC_COUNT_1SEC;
	config_tc.counter_16_bit.compare_capture_channel[1] = 0xFFFF;

	tc_init(&tc_instance, CONF_TC_MODULE, &config_tc);

	tc_enable(&tc_instance);
	
	tc_register_callback(&tc_instance, tc_cc0_cb,
		TC_CALLBACK_CC_CHANNEL0);
}

void tc_cc0_cb(struct tc_module *const module_inst)
{
	static uint16_t tc_count;
	tc_set_count_value(&tc_instance, 0);
	tc_count += 1;
	if (tc_count >= htpt_data.measurement_interval)
	{		
		gtc_count = true;
		tc_count = 0;
	}	
	LED_Toggle(LED0);	
}

void tc_start_timer(void)
{
	tc_set_count_value(&tc_instance, 0);
	tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
}

void tc_stop_timer(void)
{	
	tc_disable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);	
}



#define CONF_STDIO_USART_MODULE  EDBG_CDC_MODULE
#define CONF_STDIO_MUX_SETTING   EDBG_CDC_SERCOM_MUX_SETTING
#define CONF_STDIO_PINMUX_PAD0   EDBG_CDC_SERCOM_PINMUX_PAD0
#define CONF_STDIO_PINMUX_PAD1   EDBG_CDC_SERCOM_PINMUX_PAD1
#define CONF_STDIO_PINMUX_PAD2   EDBG_CDC_SERCOM_PINMUX_PAD2
#define CONF_STDIO_PINMUX_PAD3   EDBG_CDC_SERCOM_PINMUX_PAD3
#define CONF_STDIO_BAUDRATE      115200
static struct usart_module cdc_uart_module;

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}