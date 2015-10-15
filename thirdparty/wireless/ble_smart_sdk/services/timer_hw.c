#include <asf.h>
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_timer.h"

extern struct uart_module uart_instance;

void hw_timer_init(void)
{
	uint8_t string1[] = "HW TIMER INIT\r\n";
	uart_write_buffer_wait(&uart_instance, string1, sizeof(string1));
	
	//! [setup_timer_1]
	struct timer_config config_timer;
	//! [setup_timer_1]
	//! [setup_timer_2]
	timer_get_config_defaults(&config_timer);
	//! [setup_timer_2]
	//! [setup_timer_3]
	config_timer.reload_value = CONF_TIMER_RELOAD_VALUE;
	//! [setup_timer_3]
	//! [setup_timer_4]
	timer_init(&config_timer);
	//! [setup_timer_4]
	//! [setup_timer_5]

	//! [setup_timer_5
}



void hw_timer_register_callback(hw_timer_callback_t timer_callback_handler)
{
	//! [setup_register_callback]
	timer_register_callback(timer_callback_handler);
	//! [setup_register_callback]
	
	/* For A4, timer0 IRQ is 9 */
	//! [enable_IRQ]
	
	//NVIC_EnableIRQ(9);
	NVIC_EnableIRQ(TIMER0_IRQn);
	//chris.choi : for B0 board, how about TIMER0_IRQn = 26?
	
	//! [enable_IRQ
}

void hw_timer_start(int dealy)
{
	uint8_t string1[] = "HW TIMER START\r\n";
	uart_write_buffer_wait(&uart_instance, string1, sizeof(string1));

	//timer_set_value(dealy);
	
	timer_enable();
}

void hw_timer_stop()
{
	uint8_t string1[] = "HW TIMER STOP\r\n";
	uart_write_buffer_wait(&uart_instance, string1, sizeof(string1));
	
	timer_disable();
}


