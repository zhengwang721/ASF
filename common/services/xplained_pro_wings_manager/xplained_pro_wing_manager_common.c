#include "xplained_pro_wing_manager_common.h"

void wing_find_strings(uint8_t *buf, struct wing_ebdg_strings *strings)
{
	uint8_t index = 0;
	uint8_t last_length = 0;

	/* First string is located at the beginning of the buffer */
	strings->string[index++] = buf;

	for(int c = 0; c<WING_EBDG_INFO_PACKAGE_LENGTH + 6; ++c) {
		
		/* We only need 4 strings */
		if(index>3) {
			break;
		}

		if(buf[c] == 0x00) {			
			/* Include 0 termination character */ 
			strings->string[index] = (char*)&buf[++c];
			strings->string_length[index-1] = c - last_length;
			index++;

			last_length = c;
		}
		
		
	}
}

uint8_t wing_find_compability(uint32_t crc)
{
	uint8_t index=0;

	do {
		if(wing_support_table[index].crc == crc) {
			return index;
		} else if (wing_support_table[index].crc == 0x00000000) {
			/* We can only have 10 wings, 0xff is not possible */
			return WING_INVALID;
		}

		index++;

	} while(index > WING_MAX);

	/* This should not happend */
	return WING_INVALID;

}

uint8_t wing_find_by_type(enum wing_type type)
{
}

