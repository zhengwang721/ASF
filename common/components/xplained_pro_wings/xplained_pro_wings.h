#ifndef _XPLAINED_PRO_WINGS_H_INCLUDED_
#define _XPLAINED_PRO_WINGS_H_INCLUDED_

#include <stdlib.h>

#if defined(OLED1_WING_CRC)
#include "oled1.h"
#endif

#define WING_NUMBER_OF_LEDS     5
#define WING_NUMBER_OF_BUTTONS  5
#define WING_MAX                10

enum wing_type {
	WING_UNKNOWN,
	WING_OLED1,
	WING_IO1,
};

struct wing_object {

	uint8_t        position;
	enum wing_type type;

	struct {
		uint8_t led[WING_NUMBER_OF_LEDS];
		uint8_t button[WING_NUMBER_OF_BUTTONS];
	} pins;

};

typedef void(*wing_init_prototype)(struct wing_object *wing, uint8_t position);

struct wing_support_entry {
	enum wing_type      type;
	uint32_t            crc;
	wing_init_prototype init;

};

struct wing_support_entry wing_support_table[];

#endif // _XPLAINED_PRO_WINGS_H_INCLUDED_
