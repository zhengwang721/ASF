

#ifndef WIDBG_H
#define WIDBG_H

#include "compiler.h"
#define MODE_MASK     0x03

#define PHY_MODE_SUPPORTED_BIT 0x02
#define APP_MODE_SUPPORTED_BIT 0x01

#define SUPPORTED_MODE (PHY_MODE_SUPPORTED_BIT | APP_MODE_SUPPORTED_BIT)

#define LOG_MASK					(0x0F)

#define PACK __attribute__ ((packed))

#define APP_MODE             ()

typedef enum{
	ENTRY = 0,
	EXIT
}error_code_t;

typedef enum{
	TRACE_ENTRY = 0,
	TRACE_EXIT
}trace_type_t;

typedef enum{
	LOG_INFO = 0x01,
	LOG_WARNING = 0x02,
	LOG_ERROR = 0x04,
	LOG_ASSERT = 0x08
}log_type_t;

typedef enum {
	LWMESH_NWK,
	LWMESH_PHY,
	LWMESH_SYS
}module_id_t;

typedef enum {
	COMMON = 0,
	UPGRADE,
	DEBUG
}module_code_t;

typedef enum {
	SUCCESS = 0x00,
	CCA_FAILURE,
	NO_ACK,
	ERROR,
	INVALID_MSG = 0x80,
	MSG_IN_PROGRESS
}error_status_t;

typedef enum {
	TIMER_MODE_SINGLE,
	TIMER_MODE_PERIODIC,
} widbg_timer_mode_t;

typedef enum {
	BROADCAST_MODE = 0,
	NATIVE_ADDR_MODE,
	EXTENDED_ADDR_MODE,
}addr_mode_t;

typedef enum {
	DOMAIN_WIDBG_COMMON = 0x80,
	DOMAIN_WIDBG_UPGRADE,
	DOMAIN_WIDBG_DEBUG
}widbg_domain_t;

typedef enum {
	WIDBG_LED_ON,
	WIDBG_LED_OFF,
	WIDBG_LED_TOGGLE
}widbg_led_t;

typedef enum {
	MEMORY_ABSOLUTE_ADDRESS,
	MEMORY_OFFSET_ADDRESS,
	MEMORY_NODE_INFO_ADDRESS
}address_type_t;

void widbg_init(void);
void widbg_task(void);
void handle_widbg_msg(uint16_t length, uint8_t *msg);
void reverse_memcpy(uint8_t *dst, uint8_t *src, uint8_t len);

void widbg_log(uint8_t log_type, module_id_t module_id, error_code_t error_code, uint8_t len, uint8_t* user_log);
void widbg_trace(trace_type_t trace_type);
#endif /* WIDBG_H */

