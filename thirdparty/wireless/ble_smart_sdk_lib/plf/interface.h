#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "event.h"
#include "platform_drv.h"

#define INTERFACE_HDR_LENGTH		8

#define INTERFACE_API_PKT_ID			0x05

#define INTERFACE_SEND_BUF_MAX 	100 // ?? will be in RAM
#define INTERFACE_RCV_BUFF_LEN 500 //will be in stack of internal app , 500 used in extrernal app is too much ??

extern uint8_t interface_send_msg[INTERFACE_SEND_BUF_MAX];

#define INTERFACE_MSG_INIT(msg_id, dest_id) \
do{\
	uint16_t __idx = INTERFACE_HDR_LENGTH;\
	uint8_t* __ptr = NULL;\
	bool timeout = false;\
	interface_send_msg[0] = ((msg_id) & 0x00FF );\
	interface_send_msg[1] = (((msg_id)>>8) & 0x00FF );\
	interface_send_msg[2] = ((dest_id) & 0x00FF );\
	interface_send_msg[3] = (((dest_id)>>8) & 0x00FF );\
	interface_send_msg[4] = ((TASK_INTERN) & 0x00FF );\
	interface_send_msg[5] = (((TASK_INTERN)>>8) & 0x00FF );\

#define INTERFACE_PACK_ARG_UINT8(arg)\
	interface_send_msg[__idx++] = (arg)

#define INTERFACE_PACK_ARG_UINT16(arg)\
	interface_send_msg[__idx++] = ((arg) & 0x00FF);\
	interface_send_msg[__idx++] = (((arg) >> 8) & 0x00FF)

#define INTERFACE_PACK_ARG_UINT32(arg) \
	interface_send_msg[__idx++] = (uint8_t)((arg) & 0x00FF );\
	interface_send_msg[__idx++] = (uint8_t)(( (arg) >> 8) & 0x00FF) ;\
	interface_send_msg[__idx++] = (uint8_t)(( (arg) >> 16) & 0x00FF);\
	interface_send_msg[__idx++] = (uint8_t)(( (arg) >> 24) & 0x00FF)

#define INTERFACE_PACK_ARG_BLOCK(ptr,len)\
	memcpy(interface_send_msg + __idx, ptr, len);\
	__idx += len
		
#define INTERFACE_PACK_ARG_DUMMY(len)\
	__idx += len

#define INTERFACE_SEND_NO_WAIT()\
	__ptr = __ptr;\
	timeout = timeout;\
	interface_send_msg[INTERFACE_HDR_LENGTH-2] = ((__idx - INTERFACE_HDR_LENGTH) & 0x00FF );\
	interface_send_msg[INTERFACE_HDR_LENGTH-1] = (((__idx - INTERFACE_HDR_LENGTH)>>8) & 0x00FF);\
	interface_send(interface_send_msg, __idx)

#define INTERFACE_SEND_WAIT(msg, src)\
	watched_event.msg_id = msg;\
	watched_event.src_id = src;\
	interface_send_msg[INTERFACE_HDR_LENGTH-2] = ((__idx - INTERFACE_HDR_LENGTH) & 0x00FF );\
	interface_send_msg[INTERFACE_HDR_LENGTH-1] = (((__idx - INTERFACE_HDR_LENGTH)>>8) & 0x00FF);\
	interface_send(interface_send_msg, __idx);\
	platform_event_wait(timeout);\
	cmd_cmpl_wait(&timeout);\
	if(timeout){return AT_BLE_FAILURE;}\
	__idx = 0;\
	__ptr = watched_event.params;

#define INTERFACE_UNPACK_INIT(ptr)\
do{\
	uint8_t* __ptr = (uint8_t*)(ptr);\
	uint32_t __idx = 0;

#define INTERFACE_UNPACK_UINT8(ptr)\
	*((uint8_t*)(ptr)) = __ptr[__idx++]

#define INTERFACE_UNPACK_UINT16(ptr)\
	*((uint16_t*)(ptr)) = __ptr[__idx]\
		| (__ptr[__idx + 1] << 8);\
		__idx += 2

#define INTERFACE_UNPACK_UINT32(ptr)\
	*((uint32_t*)(ptr)) = __ptr[__idx] \
		| (__ptr[__idx + 1] << 8) \
		| (__ptr[__idx + 2] << 16)\
		| (__ptr[__idx + 3] << 24);\
		__idx += 4

#define INTERFACE_UNPACK_BLOCK(ptr, len)\
	memcpy(ptr, __ptr + __idx, len);\
	__idx += len;

#define INTERFACE_UNPACK_SKIP(len)\
		__idx += (len)

#define INTERFACE_DONE()\
}while(0)

void interface_send(uint8_t* msg, uint16_t u16TxLen);

void interface_init(void);
void rx_callback(uint8_t* data, uint16_t len);
#endif /* HCI_H_ */
