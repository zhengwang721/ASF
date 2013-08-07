#ifndef _XPLAINED_PRO_WING_MANAGER_H_INCLUDED_
#define _XPLAINED_PRO_WING_MANAGER_H_INCLUDED_

#include "xplained_pro_wings.h"

struct wing_list_entry {
	uint8_t        position;
	uint8_t        index;
	enum wing_type type;
};

struct wing_list {
	uint8_t count;

	struct wing_list_entry wing[WING_MAX];
};

enum status_code wing_probe(struct wing_list *list);
enum status_code wing_spawn(struct wing_list *list, enum wing_type type, struct wing_object *wing);

#endif /* _XPLAINED_PRO_WING_MANAGER_H_INCLUDED_ */
