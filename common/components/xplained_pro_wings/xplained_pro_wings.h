#ifndef _XPLAINED_PRO_WINGS_H_INCLUDED_
#define _XPLAINED_PRO_WINGS_H_INCLUDED_

#include <stdlib.h>

#if defined(OLED1_WING_CRC)
#include "oled1.h"
#endif

enum wing_type {
	WING_UNKNOWN,
	WING_OLED1,
	WING_IO1,
};

typedef void(*wing_init_prototype)(uint8_t position);

struct wing_support_entry {
	enum wing_type      type;
	uint32_t            crc;
	wing_init_prototype init;

};

struct wing_support_entry wing_support_table[];

#endif // _XPLAINED_PRO_WINGS_H_INCLUDED_
