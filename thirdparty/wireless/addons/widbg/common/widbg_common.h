#ifndef WIDBG_COMMON_H
#define WIDBG_COMMON_H

#include "widbg.h"
#include "widbg_mgr.h"

typedef enum {
	OTA_CLIENT_DISCOVERY = 0,
	OTA_CLIENT_NOTIFY,
	OTA_CLIENT_INFO_REQ,
	OTA_CLIENT_INFO,
	OTA_IDENTIFY_REQ,
	OTA_RESET_REQ,
	OTA_ERROR_MSG = 0xF0
}widbg_common_msg_code_t;

typedef enum {
	COORDINATOR,
	ROUTER,
	ENDDEVICE
}dev_type_t;

typedef struct PACK{
	uint8_t msg_id;
	dev_type_t dev_type;
	addr_mode_t ieee_addr_mode;
	uint64_t ieee_addr;
	uint16_t pan_id;
	addr_mode_t parent_addr_mode;
	uint64_t parent_short_addr;
	uint8_t lqi;
}client_notify_indication_t;

typedef struct PACK{
	uint8_t msg_id;
	uint8_t supported_mode;
	uint8_t firmware[10];
	uint8_t firmware_version[5];
	uint8_t board[10];
	uint8_t board_version[5];
}client_info_indication_t;


#define INVALID_ADDR 0xFFFF

void handle_common_widbg_msg(uint16_t len, uint8_t *msg);
void widbg_common_timer_handler(void);
void widbg_common_init(void);
void widbg_common_sent_frame(uint8_t addr_mode, uint8_t *addr, uint8_t status);
void widbg_common_rcvd_frame(uint8_t addr_mode, uint8_t *src_addr, uint8_t length, uint8_t *payload, uint8_t lqi);
#endif /* OTA_COMMON_H */
