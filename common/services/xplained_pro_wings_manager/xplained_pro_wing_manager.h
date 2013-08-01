#ifndef _XPLAINED_PRO_WING_MANAGER_H_INCLUDED_
#define _XPLAINED_PRO_WING_MANAGER_H_INCLUDED_

#include "xplained_pro_wings.h"

#define WING_NUMBER_OF_LEDS     5
#define WING_NUMBER_OF_BUTTONS  5
#define WING_MAX                10

struct wing_object {

	uint8_t        position;
	enum wing_type type;

	struct {
		uint8_t led[WING_NUMBER_OF_LEDS];
		uint8_t button[WING_NUMBER_OF_BUTTONS];
	} pins;

};

struct wing_list {
	uint8_t count;

	struct wing_object wing[WING_MAX];
};

void wing_probe(struct wing_list *list);
void wing_spawn(enum wing_type type, struct wing_object *object);


#endif /* _XPLAINED_PRO_WING_MANAGER_H_INCLUDED_ */
