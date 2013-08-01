#include "compiler.h"
#include "xplained_pro_wing_manager.h"
#include "xplained_pro_wing_manager_common.h"
#include "i2c_common.h"
#include "i2c_master.h"
#include "status_codes.h"
#include "board.h"

#define EBDG_TOKEN                  0xe1
#define EBDG_I2C_ADDRESS            0x28

/* Hardware supported CRC generation */
static uint32_t _wing_generate_crc(uint8_t *buf, uint8_t length)
{
	/* Reset module */
	DSU->CTRL.reg = DSU_CTRL_SWRST;

	/* Set buffer address */
	DSU->ADDR.reg = (((uint32_t)&buf) & DSU_ADDR_ADDR_Msk) << DSU_ADDR_ADDR_Pos;

	/* Set buffer length */
	DSU->LENGTH.reg = (length & DSU_LENGTH_LENGTH_Msk) << DSU_LENGTH_LENGTH_Pos;

	/* Make sure status flags are cleared */
	DSU->STATUSA.reg = DSU_STATUSA_MASK;

	/* Start checksum generation */
	DSU->CTRL.reg = DSU_CTRL_CRC;

	while(!DSU->STATUSA.reg) {

		if(DSU->STATUSA.reg & DSU_STATUSA_DONE) {
			return DSU->DATA.reg;
		} else {
			/* Can I assume that this is not a valid CRC code? */
			return 0x00000000;
		}

	}
}

static enum status_code _wing_configure_i2c(struct i2c_master_module *ebdg_i2c)
{
	struct i2c_master_config config;

	/* Get default i2c configuration, it should do for our application */
	i2c_master_get_config_defaults(&config);

	/* Make sure the pin muxes are correctly set */
	config.pinmux_pad0 = EDBG_I2C_SERCOM_PINMUX_PAD0;
	config.pinmux_pad1 = EDBG_I2C_SERCOM_PINMUX_PAD1;

	/* Initialize the i2c module */
	return i2c_master_init(ebdg_i2c, EDBG_I2C_MODULE, &config);
}

void wing_probe(struct wing_list *list)
{

	struct i2c_packet packet;
	uint8_t ebdg_token = EBDG_TOKEN;
	enum status_code err;
	struct wing_ebdg_data_container wing_data;
	struct i2c_master_module ebdg_i2c;

	/* Setup I2C communication with the EBDG */
	err = _wing_configure_i2c(&ebdg_i2c);

	if(err == STATUS_OK) {
		i2c_master_enable(&ebdg_i2c);

		/* Send a token byte to the EBDG to tell it I want information */
		packet.address = EBDG_I2C_ADDRESS;
		packet.data = &ebdg_token;
		packet.data_length = 1;

		/* Write i2c package without stop condition */

		/* This call will return a STATUS_ERR_PACKET_COLLISION,
		   but I can ignore that error for some reason. This has to be investigate further
		   at a later stage */
		i2c_master_write_packet_wait_no_stop(&ebdg_i2c, &packet);
	}

	/* Do a repeated start condition */
	if(err == STATUS_OK) {
		/* Setup i2c driver for reading the data package from the EBDG */
		packet.data = (uint8_t*)&wing_data;
		packet.data_length = WING_EBDG_MAP_LENGTH + (WING_EBDG_INFO_PACKAGE_LENGTH * WING_MAX);

		err = i2c_master_read_packet_wait_no_stop(&ebdg_i2c, &packet);
	}

	if(err == STATUS_OK) {

		/* Temporary variable to hold ext map bit pattern while shifting it down,
			each of the 10 bits (support for 10 possible wings) identifies if there 
			is a wing connected to that extension */
		uint16_t tmp_ext_map;

		/* Reverse endianness */
		tmp_ext_map = wing_data.map[1] & 0x00ff;
		tmp_ext_map |= (wing_data.map[0] << 0x08) & 0xff00;

		/* Extension counter */
		uint8_t ext_count = 0;

		for(int c=0;c<WING_MAX;++c) {
			if(tmp_ext_map & 0x01) {

				struct wing_ebdg_strings ebdg_strings;
				uint8_t index=0;
				uint32_t crc;

				/* Split buffer into 4 strings */
				wing_find_strings(wing_data.buffer[ext_count], &ebdg_strings);

				crc = _wing_generate_crc((uint8_t*)ebdg_strings.string[WING_EBDG_STRING_PRODUCT_NAME],
						ebdg_strings.string_length[WING_EBDG_STRING_PRODUCT_NAME]);

				index = wing_find_compability(crc);

				if(index != WING_INVALID) {

					list->wing[ext_count].position = c;
					list->wing[ext_count].type     = wing_support_table[index].type;

					ext_count++;
				}

			}

			/* Shift ext map to check the next bit */
			tmp_ext_map = tmp_ext_map >> 1;

			/* If tmp_ext_map == 0 there is no more wings to check */
			if(tmp_ext_map == 0) {
				break;
			}

		}

		/* Store the number of discovered wings */
		list->count = ext_count;
	}

	/* Make sure the to always end communication with the ebdg */
	i2c_master_send_stop(&ebdg_i2c);
	i2c_master_disable(&ebdg_i2c);

	return err;

}

void wing_spawn(enum wing_type type, struct wing_object *object)
{
}
