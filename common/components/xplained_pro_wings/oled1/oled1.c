#include "oled1.h"

#define OLED1_INIT_INSTANCE(name, ext_header) \
name->pins.led[0] = ext_header ## _PIN_7;       \
name->pins.led[1] = ext_header ## _PIN_8;       \
name->pins.led[2] = ext_header ## _PIN_6;       \
name->pins.button[0] = ext_header ## _PIN_9;    \
name->pins.button[1] = ext_header ## _PIN_3;    \
name->pins.button[2] = ext_header ## _PIN_4;    \

void wing_oled1_init(uint8_t position)
{
	struct port_config pin_conf;

	oled_wing->type = WING_TYPE_OLED1;
	oled_wing->position = position;

	if(oled_wing->position == 0) {

		OLED1_INIT_INSTANCE(oled_wing, EXT1);

	} else if (oled_wing->position == 1) {

		OLED1_INIT_INSTANCE(oled_wing, EXT2);

	} else if (oled_wing->position == 2) {

		OLED1_INIT_INSTANCE(oled_wing, EXT3);

	}

	port_get_config_defaults(&pin_conf);

	pin_conf.direction = PORT_PIN_DIR_OUTPUT_WTH_READBACK;
	pin_conf.input_pull = PORT_PIN_PULL_NONE;

	port_pin_set_config(oled_wing->pins.led[0], &pin_conf);
	port_pin_set_config(oled_wing->pins.led[1], &pin_conf);
	port_pin_set_config(oled_wing->pins.led[2], &pin_conf);

	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;

	port_pin_set_config(oled_wing->pins.button[0], &pin_conf);
	port_pin_set_config(oled_wing->pins.button[1], &pin_conf);
	port_pin_set_config(oled_wing->pins.button[2], &pin_conf);
}
