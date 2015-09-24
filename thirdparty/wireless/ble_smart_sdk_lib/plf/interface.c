#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"
#include "event.h"
#include "ke_msg.h"
#include "uart.h"

uint8_t interface_send_msg[INTERFACE_SEND_BUF_MAX];


#define	NMI_BLE_WAITING					0
#define	NMI_BLE_COLLECTING_HDR			1
#define	NMI_BLE_COLLECTING_PAYLOAD		2
#define MAX_EVT_LEN 	600
uint8_t event_param_buff[MAX_EVT_LEN];
uint16_t event_param_buff_index;
uint32_t dummy_buffer_buff[INTERFACE_SEND_BUF_MAX / 4];
uint8_t * dummy_buffer = (uint8_t *) dummy_buffer_buff;

void rx_callback(uint8_t* data, uint32_t len)
{
	struct ke_msghdr* rcv_msg = (struct ke_msghdr *)data;
	uint16_t msg_id = rcv_msg->id;
	uint16_t src_id = rcv_msg->src_id;
	uint8_t* param = (uint8_t *)(rcv_msg + 1);
	uint16_t param_len = rcv_msg->param_len;
	//uart_printf(UART_HW_MODULE_UART2,"%04x\n\r",msg_id);
	if(param_len==0)
	{
		//post the event to api event queue
		if(!special_events_handler(msg_id, src_id, NULL))
		{
#ifndef NEW_EVT_HANDLER
			// post it into the event queue
			event_post(msg_id, src_id, NULL);
#else
			// post it into the event queue
			event_post(msg_id, src_id, NULL,0);
#endif
		}
	}
	else
	{
		// post it into the event queue
		if(event_param_buff_index + param_len > MAX_EVT_LEN) {
			event_param_buff_index = 0;
		}
		memcpy(&event_param_buff[event_param_buff_index],param,param_len);
		if(!special_events_handler(msg_id, src_id, &event_param_buff[event_param_buff_index]))
		{
#ifndef NEW_EVT_HANDLER
			event_post(msg_id, src_id, &event_param_buff[event_param_buff_index]);
#else
			event_post(msg_id, src_id, &event_param_buff[event_param_buff_index],param_len);
#endif
			event_param_buff_index += param_len;
		}
	}
}

at_ble_status_t interface_send(uint8_t* msg, uint16_t u16TxLen)
{
	platform_interface_send(msg, u16TxLen);
	return AT_BLE_SUCCESS;
}

void interface_init(void)
{
	struct uart_config uart_cfg;
	memset(interface_send_msg,0,sizeof(interface_send_msg));
	memset(event_param_buff,0,sizeof(event_param_buff));
	event_param_buff_index = 0;
	/* Just for testing we can remove in the final release */
	memset(&uart_cfg,0,sizeof(uart_cfg));
	//uart_get_config_defaults(&uart_cfg);
	//uart_init(UART_HW_MODULE_UART2,&uart_cfg);
	//uart_printf(UART_HW_MODULE_UART2,"Interface Initialized\n\r");
}

static volatile uint32_t cmd_cmpl_flag = 0;
static volatile uint32_t event_flag = 0;

void platform_cmd_cmpl_signal()
{
    cmd_cmpl_flag = 1;
}

void platform_cmd_cmpl_wait(bool *timeout)
{
	// start_timer(to);

	while ((cmd_cmpl_flag != 1))
	{
		platform_event_wait(0);
	}

	if (cmd_cmpl_flag == 1)
	{
		cmd_cmpl_flag = 0;
		/*
#ifdef NEW_EVT_HANDLER
        stop_timer();
#endif  //NEW_EVT_HANDLER
		 */
	}
	else
	{
		*timeout = true;
	}
}

void start_timer(uint32_t timeout)
{
}

void stop_timer()
{
}

