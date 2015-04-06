#ifndef WIDBG_DEBUG_H
#define WIDBG_DEBUG_H

#include "widbg.h"
#include "widbg_mgr.h"

typedef enum {
	OTA_READ_MEMORY_REQUEST = 0,
	OTA_READ_MEMORY_INDICATION,
	OTA_WRITE_MEMORY_REQUEST,
	OTA_WRITE_MEMORY_INDICATION,
	OTA_LOG_REQUEST,
	OTA_LOG_INDICATION,
	OTA_TRACE_REQUEST,
	OTA_TRACE_INDICATION
}widbg_debug_msg_code_t;

typedef struct PACK {
	uint8_t msg_id;
	uint8_t mem_type;
	uint16_t mem_len;
	uint32_t mem_addr;
}read_memory_req_t;

typedef struct PACK {
	uint8_t msg_id;
	uint8_t status;
	uint8_t mem_type;
	uint16_t mem_len;
	uint32_t mem_addr;
	uint8_t mem_content[100];
}read_mem_indication_t;

typedef struct PACK {
	uint8_t msg_id;
	uint8_t mem_type;
	uint16_t mem_len;
	uint32_t mem_addr;
	uint8_t mem_content[100];
}write_memory_req_t;

typedef struct PACK {
	uint8_t msg_id;
	uint8_t status;
	uint8_t mem_type;
	uint16_t mem_len;
	uint32_t mem_addr;
}write_mem_indication_t;

typedef struct PACK {
	uint8_t msg_id;
	uint8_t log_bitmap;
}log_request_t;

void handle_debug_widbg_msg(uint16_t len, uint8_t *msg);
void widbg_debug_timer_handler(void);
void widbg_debug_init(void);
void widbg_debug_sent_frame(uint8_t addr_mode, uint8_t *addr, uint8_t status);
void widbg_debug_rcvd_frame(uint8_t addr_mode, uint8_t *src_addr, uint8_t length, uint8_t *payload, uint8_t lqi);

#endif /* WIDBG_DEBUG_H */
