#include "xplained_pro_wings.h"

struct wing_support_entry wing_support_table[] = {

#if defined(OLED1_WING_CRC)
	{
		.type = WING_OLED1,
		.crc  = OLED1_WING_CRC,
		.init = wing_oled1_init,
	},
#endif

#if defined(IO1_WING_CRC)
	{
		.type = WING_IO1,
		.crc  = IO1_WING_CRC,
		.init = wing_io1.init,
	},
#endif

/* Add end entry to detect when we are at the end of the table.
   I assume that you can not have a 0x00000000 CRC code */

	{
		.type = WING_UNKNOWN,
		.crc  = 0x00000000,
		.init = NULL,
	},
};

