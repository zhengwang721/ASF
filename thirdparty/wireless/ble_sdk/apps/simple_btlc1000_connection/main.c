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


static uint8_t adv_data[] = {0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15, 0x21, 0x8A,
	                         0xF6, 0x52, 0x73, 0xE3, 0x40, 0xB3, 0xB4, 0x1C,
	                         0x19, 0x53, 0x24, 0x2C, 0x72, 0xf4, 0x00, 0xbb,
                             0x00, 0x44, 0xc5};

static uint8_t scan_rsp_data[] = {0x11, 0x07, 0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00,
	                              0x37, 0xaa, 0xe3, 0x11, 0x2a, 0xdc, 0x00, 0xcd,
                                  0x30, 0x57};

static at_ble_uuid_t service_uuid = {AT_BLE_UUID_128 ,
	{0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00, 0x37, 0xaa,
	 0xe3, 0x11, 0x2a, 0xdc, 0x00, 0xcd, 0x30, 0x57}};

static at_ble_handle_t service, char1, char2;
static at_ble_characteristic_t chars[] = {
	{
		0, /* handle stored here */
		{ AT_BLE_UUID_128, {0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00, 0x3b, 0x8e,
								0xe3, 0x11, 0x2a, 0xdc, 0xa0, 0xd3, 0x20, 0x8e}}, /* UUID */
			AT_BLE_CHAR_READ | AT_BLE_CHAR_WRITE | AT_BLE_CHAR_NOTIFY, /* Properties */
			"char1", sizeof("char1"), 100, /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR, /* permissions */

			NULL, 0, 0, /* user defined name */
			NULL /* presentation format */
	},
			
	{
		0, /* handle stored here */
		{ AT_BLE_UUID_128, {0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00, 0xff, 0xbb,
			0xe3, 0x11, 0x2a, 0xdc, 0xe0, 0x40, 0xd2, 0x98}}, /* UUID */
			AT_BLE_CHAR_READ | AT_BLE_CHAR_WRITE | AT_BLE_CHAR_RELIABLE_WRITE, /* Properties */
			"char2", sizeof("char1"), 100, /* value */
			AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR, /* permissions */

			NULL, 0, 0, /* user defined name */
			NULL /* presentation format */
	}
};

void app_init(void);


void app_init(void)
{
	uint8_t port = 74;
	at_ble_addr_t addr = {AT_BLE_ADDRESS_PUBLIC,
		{0x25, 0x75, 0x11, 0x6a, 0x7f, 0x7f} };

	// init device
	at_ble_init(&port);

	at_ble_addr_set(&addr);

	// establish prephiral database
	at_ble_primary_service_define(&service_uuid, &service,
		NULL, 0, chars, 2);

	// start advertising
	at_ble_adv_data_set(adv_data, sizeof(adv_data), scan_rsp_data, sizeof(scan_rsp_data));
	at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, 100, 1000, 0);

}



int main (void)
{
	at_ble_events_t event;
	uint8_t params[512];

	uint16_t handle;

#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
#elif SAM0
	system_init();
#endif
	
	//platform_init(NULL);
	
	app_init();
	
	while(at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
	{
		switch(event)
		{
			case AT_BLE_CONNECTED:
			{
				at_ble_connected_t* conn_params = (at_ble_connected_t*)params;
				#if 0
				printf("Device connected to 0x%02x%02x%02x%02x%02x%02x handle=0x%x\n",
					conn_params->peer_addr.addr[5],
					conn_params->peer_addr.addr[4],
					conn_params->peer_addr.addr[3],
					conn_params->peer_addr.addr[2],
					conn_params->peer_addr.addr[1],
					conn_params->peer_addr.addr[0],
					conn_params->handle);
				#endif
				handle = conn_params->handle;
			}
			break;

			case AT_BLE_DISCONNECTED:
			{
				//printf("Device disconnected\n");
				at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED,
					AT_BLE_ADV_GEN_DISCOVERABLE,
					NULL, AT_BLE_ADV_FP_ANY, 100, 1000, 0);
					
			}
			break;
#if 0
			case AT_BLE_CHARACTERISTIC_CHANGED:
			{
				at_ble_characteristic_changed_t* change_params
				= (at_ble_characteristic_changed_t*) params;
				uint32_t i = 0;

				printf("Characteristic 0x%x changed, new_value = ",
				change_params->char_handle);
				for(i=0; i<change_params->char_len; i++)
				printf("0x%02x ", change_params->char_new_value[i]);
				printf("\n");
			}
			break;
#endif

		}
	}


#if 0
	// This skeleton code simply sets the LED to the state of the button.
	while (1) {
		// Is button pressed?
		if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			// Yes, so turn LED on.
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		} else {
			// No, so turn LED off.
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
#endif
}
