#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"
#include "event.h"


uint8_t interface_send_msg[INTERFACE_SEND_BUF_MAX];


#define	NMI_BLE_WAITING					0
#define	NMI_BLE_COLLECTING_HDR			1
#define	NMI_BLE_COLLECTING_PAYLOAD		2

void platform_interface_callback(uint8_t* data, uint32_t len)
{
	uint32_t i;

	static uint16_t src_id, msg_id, param_len;
	static uint32_t state = NMI_BLE_WAITING;
	static uint8_t header[INTERFACE_HDR_LENGTH - 1]; // we already dropped the first byte (which is always 0x5)
	static uint32_t header_idx;
	static uint8_t buff[INTERFACE_RCV_BUFF_LEN];
	static uint32_t cur_msg_start = 0;
	static uint32_t write_ptr = 0;

	for(i=0; i<len; i++)
	{
		if(state == NMI_BLE_WAITING)
		{
			if(data[i] == INTERFACE_API_PKT_ID)
			{
				state = NMI_BLE_COLLECTING_HDR;
				header_idx = 0;
			}
			continue;
		}

		if(state == NMI_BLE_COLLECTING_HDR)
		{
			header[header_idx] = data[i];
			header_idx++;
			
			if(header_idx == INTERFACE_HDR_LENGTH - 1)
			{
				state = NMI_BLE_COLLECTING_PAYLOAD;

				INTERFACE_UNPACK_INIT(header);
				INTERFACE_UNPACK_UINT16(&msg_id);
				INTERFACE_UNPACK_SKIP(2); // not interested in destination (always EXT_APP)
				INTERFACE_UNPACK_UINT16(&src_id);
				INTERFACE_UNPACK_UINT16(&param_len);
				INTERFACE_DONE();

				if(param_len == 0)
				{
					if(!special_events_handler(msg_id, src_id, NULL))
					{
						// post it into the event queue
						if(msg_id != 0x3401)
						{
							event_post(msg_id, src_id, NULL);
						}
						else
						{
							state = NMI_BLE_WAITING;
						}
					}
				}

				// make sure that we have room for params
				if(sizeof(buff) - write_ptr < param_len)
				{
					write_ptr = 0;
				}
				cur_msg_start = write_ptr;
			}
			continue;
		}

		if(state == NMI_BLE_COLLECTING_PAYLOAD)
		{
			buff[write_ptr] = data[i];
			write_ptr++;

			if(write_ptr - cur_msg_start == param_len)
			{
				// message fully recieved
				state = NMI_BLE_WAITING;

				// handle message
				if(!special_events_handler(msg_id, src_id, &buff[cur_msg_start]))
				{
					// post it into the event queue
					event_post(msg_id, src_id, &buff[cur_msg_start]);
				}

			}

		}
	}


}

void interface_send(uint8_t* msg, uint16_t u16TxLen)
{
	platform_interface_send(msg, u16TxLen);
}


