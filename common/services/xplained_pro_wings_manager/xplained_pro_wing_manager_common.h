#ifndef _XPLAINED_PRO_WING_MANAGER_COMMON_H_INCLUDED_
#define _XPLAINED_PRO_WING_MANAGER_COMMON_H_INCLUDED_

#include <stdint.h>
#include "xplained_pro_wings.h"
#include "xplained_pro_wing_manager.h"

#define WING_NUMBER_OF_EBDG_STRINGS    4
#define WING_EBDG_INFO_PACKAGE_LENGTH  64
#define WING_EBDG_MAP_LENGTH           2

#define WING_INVALID                   0xff

enum wing_ebdg_string_index {
	WING_EBDG_STRING_MANUFACTURER,
	WING_EBDG_STRING_PRODUCT_NAME,
	WING_EBDG_STRING_PRODUCT_REV,
	WING_EBDG_STRING_PRODUCT_SERIAL,
};

struct wing_ebdg_strings {

	char     *string[WING_NUMBER_OF_EBDG_STRINGS];
	uint8_t  string_length[WING_NUMBER_OF_EBDG_STRINGS];
};

struct wing_ebdg_data_container {
	uint8_t map[WING_EBDG_MAP_LENGTH];
	uint8_t buffer[WING_MAX][WING_EBDG_INFO_PACKAGE_LENGTH];
};


uint8_t  wing_find_by_type(enum wing_type type);
uint8_t  wing_find_compability(uint32_t crc);
void     wing_find_strings(uint8_t *buf, struct wing_ebdg_strings *strings);


#endif /* _XPLAINED_PRO_WING_MANAGER_COMMON_H_INCLUDED_ */

