#include "xplained_pro_wing_manager_common.h"

void wing_find_strings(uint8_t *buf, struct wing_ebdg_strings *strings)
{
	uint8_t index = 0;
	uint8_t last_length = 0;

	/* First string is located at the beginning of the buffer */
	strings->string[index++] = buf;

	for(int c = 0; c<WING_EBDG_INFO_PACKAGE_LENGTH + 6; ++c) {

		/* We only need 4 strings */
		if(index>4) {
			break;
		}

		if(buf[c] == 0x00) {

			if (index<4) {
				/* Include 0 termination character */ 
				strings->string[index] = (char*)&buf[++c];
			}

			/* Do not include 0 termination character in string length */
			strings->string_length[index-1] = c - last_length - 1;
			last_length = c

			index++;
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

#define POLYNOMIAL 0xEDB88320
#define WIDTH      (8 * sizeof(uint32_t))
#define TOPBIT     (1 << (WIDTH - 1))

uint32_t wing_generate_crc(uint8_t *buf, uint8_t length)
{
	uint32_t  remainder = 0;

	for (uint32_t byte = 0; byte < length; ++byte) {
		remainder ^= (buf[byte] << (WIDTH - 8));

		for (uint8_t bit = 8; bit > 0; --bit) {
			if (remainder & TOPBIT) {
				remainder = (remainder << 1) ^ POLYNOMIAL;
			} else {
				remainder = (remainder << 1);
			}
		}
	}

	return (remainder);
}
