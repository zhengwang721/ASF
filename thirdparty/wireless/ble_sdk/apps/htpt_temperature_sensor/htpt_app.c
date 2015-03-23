/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "htpt_app.h"
#include "profiles.h"
#include "arm_math.h"
#include "console_serial.h"
#include "timer_hw.h"

static uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xFF, 0x00, 0x06, 0x25, 0x75, 0x11, 0x6a, 0x7f, 0x7f};

at_ble_LTK_t app_bond_info;
bool app_device_bond = false;
uint8_t auth_info = 0;

htpt_app_t htpt_data;
volatile uint16_t gtc_count = false;
double temp_res;

void app_init(void);
void htpt_init(htpt_app_t *htpt_temp);
void htpt_temperature_send(htpt_app_t *htpt_temp);
void timer_callback_handler(void);

static void button_cb(void);
static void button_init(void);

void app_init(void)
{
	uint8_t port = 74;
	at_ble_addr_t addr = {AT_BLE_ADDRESS_PUBLIC,
		{0x25, 0x75, 0x11, 0x6a, 0x7f, 0x7f} };

	// init device
	at_ble_init(&port);

	at_ble_addr_set(&addr);

	/* Initialize the htpt to default value */
	htpt_init(&htpt_data);

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
}


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
	 
	button_init();
	serial_console_init();
	
	DBG_LOG_1LVL("\r\n Initializing HTPT Application");

	at30tse_init();
	
	at30tse_write_config_register(
			AT30TSE_CONFIG_RES(AT30TSE_CONFIG_RES_12_bit));

	hw_timer_init();
	hw_timer_register_callback(timer_callback_handler);
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
			}
			break;

			case AT_BLE_DISCONNECTED:
			{
				at_ble_disconnected_t *disconnect = (at_ble_disconnected_t *)params;
				hw_timer_stop();
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
				uint8_t idx = 0;
				uint8_t adv_data[HT_ADV_DATA_NAME_LEN + HT_ADV_DATA_APPEARANCE_LEN + HT_ADV_DATA_UUID_LEN + 3*2];
				
				at_ble_htpt_create_db_cfm_t *create_db_params = (at_ble_htpt_create_db_cfm_t *)params;				
				// start advertising
				DBG_LOG("\r\nCreating HTPT DB: SUCCESS: Status=0x%x", create_db_params->status);
				
				/* Prepare ADV Data */
				adv_data[idx++] = HT_ADV_DATA_UUID_LEN + ADV_TYPE_LEN;
				adv_data[idx++] = HT_ADV_DATA_UUID_TYPE;
				memcpy(&adv_data[idx], HT_ADV_DATA_UUID_DATA, HT_ADV_DATA_UUID_LEN);				
				idx += HT_ADV_DATA_UUID_LEN;
				
				adv_data[idx++] = HT_ADV_DATA_APPEARANCE_LEN + ADV_TYPE_LEN;
				adv_data[idx++] = HT_ADV_DATA_APPEARANCE_TYPE;
				memcpy(&adv_data[idx], HT_ADV_DATA_APPEARANCE_DATA, HT_ADV_DATA_APPEARANCE_LEN);
				idx += HT_ADV_DATA_APPEARANCE_LEN;
				
				adv_data[idx++] = HT_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
				adv_data[idx++] = HT_ADV_DATA_NAME_TYPE;
				memcpy(&adv_data[idx], HT_ADV_DATA_NAME_DATA, HT_ADV_DATA_NAME_LEN);
				idx += HT_ADV_DATA_NAME_LEN;			
				
				
				at_ble_adv_data_set(adv_data, idx, scan_rsp_data, SCAN_RESP_LEN);
				
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
				hw_timer_start(htpt_data.measurement_interval);			
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
					/* Device capabilities is display only , key will be generated 
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
	htpt_temp->measurement_interval = 2; 
	htpt_temp->temperature = 3700;
	htpt_temp->temperature_type = HTP_TYPE_BODY;
	htpt_temp->max_meaurement_intv = 30;
	htpt_temp->min_measurement_intv = 1;
	htpt_temp->security_lvl = HTPT_ENABLE;
	htpt_temp->optional = HTPT_ALL_FEAT_SUP;	
}

void htpt_temperature_send(htpt_app_t *htpt_temp)
{
	//static uint32_t temperature;
	struct prf_date_time timestamp; 
	float32_t temperature;
	/* Read Temperature Value from IO1 Xplained Pro */
	temperature = (float32_t)at30tse_read_temperature();
	timestamp.day = 1;
	timestamp.hour = 9;
	timestamp.min = 2;
	timestamp.month = 8;
	timestamp.sec = 36;
	timestamp.year = 15;	
	at_ble_htpt_temp_send(temperature,
	                     &timestamp,
						 HTPT_FLAG_CELSIUS | HTPT_FLAG_TYPE,
						 htpt_temp->temperature_type,
						 STABLE_TEMPERATURE_VAL
						 );
}


static void button_init(void)
{
#if SAMD21
	struct extint_chan_conf eint_chan_conf;
	extint_chan_get_config_defaults(&eint_chan_conf);

	eint_chan_conf.gpio_pin           = BUTTON_0_EIC_PIN;
	eint_chan_conf.gpio_pin_mux       = BUTTON_0_EIC_MUX;
	eint_chan_conf.detection_criteria = EXTINT_DETECT_BOTH;
	eint_chan_conf.filter_input_signal = true;
	extint_chan_set_config(BUTTON_0_EIC_LINE, &eint_chan_conf);
	
	extint_register_callback(button_cb,
	BUTTON_0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
	
	extint_chan_enable_callback(BUTTON_0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
#endif
}

static void button_cb(void)
{
	htpt_data.temperature_type = ((htpt_data.temperature_type+1) % 9);
}

void timer_callback_handler(void)
{
	hw_timer_stop();
	htpt_temperature_send(&htpt_data);	
}