/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

// From module: GPIO - General purpose Input/Output
#include <gpio.h>

// From module: Generic board support
#include <board.h>

// From module: IOPORT - General purpose I/O service
#include <ioport.h>

// From module: Interrupt management - megaAVR and tinyAVR implementation
#include <interrupt.h>

// From module: MEGA compiler driver
#include <compiler.h>
#include <status_codes.h>

// From module: Part identification macros
#include <parts.h>
int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	bool button_state;
	while(1){
		button_state = ioport_get_pin_level(GPIO_PUSH_BUTTON_0);
		if(button_state){
			LED_On(LED0);
		}else{
			LED_Off(LED0);
		}
	}
	/* Insert application code here, after the board has been initialized. */
}
