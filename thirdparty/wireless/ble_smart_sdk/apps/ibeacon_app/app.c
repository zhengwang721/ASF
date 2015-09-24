#include "at_ble_api.h"
#include <asf.h>
#include <string.h>
#ifndef NULL
#define NULL ((void*)0)
#endif

#define BEACON_IDENTIFIER (0x47)
static uint8_t adv_data[31];
static uint8_t scan_rsp_data[31];	


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


at_ble_init_config_t pf_cfg;

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







void ble_init(void)
{
	 uint16_t status = AT_BLE_FAILURE; 
	 at_ble_addr_t addr = {AT_BLE_ADDRESS_PUBLIC,
		{0x45, 0x75, 0x11, 0x6a, 0x7f, 0x7f} };
	 	 
		const uint8_t ro_adv_data[] = {0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15, 0x21, 0x8A,
															 0xF6, 0x52, 0x73, 0xE3, 0x40, 0xB3, 0xB4, 0x1C,
															 0x19, 0x53, 0x24, 0x2C, 0x72, 0xf4, 0x00, 0xbb,
																0x00, 0x45, 0xc5};

    const uint8_t ro_scan_rsp_data[] = {0x11, 0x07, 0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00,
																		0x37, 0xaa, 0xe3, 0x11, 0x2a, 0xdc, 0x00, 0xcd,
																		0x30, 0x57};
		
		addr.addr[0] = BEACON_IDENTIFIER;
		
		memset(adv_data,0,sizeof(adv_data));
		memset(scan_rsp_data,0,sizeof(scan_rsp_data));
		memcpy(adv_data,ro_adv_data,sizeof(ro_adv_data));
		memcpy(scan_rsp_data,ro_scan_rsp_data,sizeof(ro_scan_rsp_data));
		memset(&pf_cfg,0,sizeof(pf_cfg));
		// init device
		if((status = at_ble_init(&pf_cfg)) == AT_BLE_SUCCESS)
		{
		    status = AT_BLE_FAILURE;
  			// Set the device address
		    status = at_ble_addr_set(&addr);
	       // start advertising
		    adv_data[25] = BEACON_IDENTIFIER;
		    at_ble_adv_data_set((uint8_t *)adv_data, sizeof(ro_adv_data), (uint8_t *)scan_rsp_data, sizeof(ro_scan_rsp_data));
		    at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, 0x00A0, 0, 0);
		}

 }
		
int main(void);
//main_task_id = 0x63;
//unsigned int osAppTask_var __attribute__((at(0x100000D0))) = (unsigned int)app_entry;
//unsigned int main_task_id __attribute__((at(0x100000C8))) = 0x3E;

static uint8_t params[100];
int main()
{
	at_ble_events_t event;
	uint16_t handle;

	memset(params,0,sizeof(params));	
	
	//! [module_inst]
//uart_module uart_instance;
//! [module_inst]

//! [variable_inst]
	memset(string_input,0,sizeof(string_input));
	read_complete_flag = false;
	write_complete_flag = false;

	// chris choi : case 1, not working
	//ble_init();
	
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);

	// chris choi : case 4, working
	//ble_init();

//! [setup_init]
	configure_uart();
//! [setup_init]

//! [main]
//! [main_send_string1]
	uint8_t string1[] = " Hello Worlda\r\n";
	uart_write_buffer_wait(&uart_instance, string1, sizeof(string1));
//! [main_send_string1]
	

	
	uint8_t string2[] = " Hello World2\r\n";
	uart_write_buffer_wait(&uart_instance, string2, sizeof(string2));
//! [main_send_string1]

	//chris.choi : case 2 working
	//ble_init();


	// chris.choi : case 3 not working
	{	//case 3
		//ble_init();

	//! [test_callback_functions]
	uint8_t string3[] = "input 8\r\n";
	uart_register_callback(&uart_instance, uart_write_complete_callback,
		UART_TX_COMPLETE);
	uart_enable_callback(&uart_instance, UART_TX_COMPLETE);
	uart_write_buffer_job(&uart_instance, string3, sizeof(string3));
	while (!write_complete_flag);

	uart_register_callback(&uart_instance, uart_read_complete_callback,
		UART_RX_COMPLETE);
	uart_enable_callback(&uart_instance, UART_RX_COMPLETE);
	uart_read_buffer_job(&uart_instance, string_input, sizeof(string_input));
	while (!read_complete_flag);

	uint8_t string4[] = "received: ";
	uart_write_buffer_wait(&uart_instance, string4, sizeof(string4));
	uart_write_buffer_wait(&uart_instance, string_input, sizeof(string_input));
	
	}	// case 3


	ble_init();

	uint8_t string5[] = " Hello World3\r\n";
	uart_write_buffer_wait(&uart_instance, string5, sizeof(string5));
	
	

	
	while(at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
	{
		switch(event)
		{
			case AT_BLE_CONNECTED:
			{
				at_ble_connected_t* conn_params = (at_ble_connected_t*)params;
				handle = conn_params->handle;
				handle = handle;
			}
			break;

			case AT_BLE_DISCONNECTED:
			{
				at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED,AT_BLE_ADV_GEN_DISCOVERABLE,NULL, AT_BLE_ADV_FP_ANY, 0x00A0, 0, 0);
			}
			break;

			default:
			break;

		}
	}
	return 0;
}	
