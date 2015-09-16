#include "at_ble_api.h"
#include "platform_drv.h"
#include "interface.h"
#include "event.h"
#include "ke_msg.h"

uint8_t interface_send_msg[INTERFACE_SEND_BUF_MAX];


#define	NMI_BLE_WAITING					0
#define	NMI_BLE_COLLECTING_HDR			1
#define	NMI_BLE_COLLECTING_PAYLOAD		2
#define MAX_EVT_LEN 	128
uint8_t buff[MAX_EVT_LEN];
uint16_t buff_index;
void rx_callback(uint8_t* data, uint16_t len)
{
	struct ke_msghdr* rcv_msg = (struct ke_msghdr *)data;
	uint16_t msg_id = rcv_msg->id;
	uint16_t src_id = rcv_msg->src_id;
	uint8_t* param = (uint8_t *)(rcv_msg + 1);
	uint16_t param_len = rcv_msg->param_len;
		if(param_len==0)
		{
			//post the event to api event queue
			if(!special_events_handler(msg_id, src_id, NULL))
			{
				// post it into the event queue
				event_post(msg_id, src_id, NULL,0);
			} 		
		}
		else
		{
			// post it into the event queue
			if(buff_index + param_len > MAX_EVT_LEN) {
				buff_index = 0;
			}
			memcpy(&buff[buff_index],param,param_len);
			if(!special_events_handler(msg_id, src_id, &buff[buff_index]))
			{
				event_post(msg_id, src_id, &buff[buff_index],param_len);
				buff_index += param_len;
			}
		}
}

void interface_send(uint8_t* msg, uint16_t u16TxLen)
{
	platform_interface_send(msg, u16TxLen);
}

void interface_init()
{
	memset(interface_send_msg,0,sizeof(interface_send_msg));
	memset(buff,0,sizeof(buff));
	buff_index = 0;
}