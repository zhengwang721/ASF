#include "debug.h"

void init_debug_pins(void)

{
	uint32_t leds[] =  {LED1, LED2, LED3, LED4, IOPIN1, IOPIN2, IOPIN3, IOPIN4};
	uint32_t buttons[] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4};
	uint8_t i;
	struct port_pin_conf pin_conf;

	port_pin_get_config_defaults(&pin_conf);

	/* Buttons */
	pin_conf.input.enabled  = true;
	pin_conf.output.enabled = false;
	for(i = 0; i < sizeof(buttons) / 4; i++) {
		port_pin_set_config(buttons[i], &pin_conf);
	}

	/* LEDS and IOPINS*/
	pin_conf.input.enabled  = false;
	pin_conf.output.enabled = true;
	for(i = 0; i < sizeof(leds) / 4; i++) {
		port_pin_set_config(leds[i], &pin_conf);
	}
}


void debug_set_val(uint8_t val)
{
	uint8_t i;
	uint32_t pins[] = {IOPIN1, IOPIN2, IOPIN3, IOPIN4};

	for (i = 0; i < sizeof(pins) / 4; i++) {
		port_pin_set_output_level(pins[i], (val & (1 << i)));
	}

}

void debug_set_leds(uint8_t val)
{
	uint8_t i;
	uint32_t leds[] = {LED1, LED2, LED3, LED4};

	for (i = 0; i < sizeof(leds) / 4; i++) {
		port_pin_set_output_level(leds[i], (val & (1 << i)));
	}

}

void debug_delay(uint16_t len)
{
	volatile uint32_t delay;

	for(delay = 0; delay < len; delay++);
}
