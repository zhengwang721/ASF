#include <asf.h>
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_timer.h"
#include "conf_dualtimer.h"

extern struct uart_module uart_instance;

static void dualtimer_callback1(void)
{
	puts("Timer1 trigger\r\n");
}

/* ! [print_timer1] */

/* ! [print_timer2] */
static void dualtimer_callback2(void)
{
	puts("Timer2 trigger\r\n");
}

void hw_timer_init(void)
{
	struct dualtimer_config config_dualtimer;
	dualtimer_get_config_defaults(&config_dualtimer);

	config_dualtimer.timer1.load_value = CONF_DUALTIMER_TIMER1_LOAD;
	config_dualtimer.timer2.load_value = CONF_DUALTIMER_TIMER2_LOAD;

	dualtimer_init(&config_dualtimer);
}

void hw_timer_register_callback(hw_timer_callback_t timer_callback_handler)
{
	dualtimer_register_callback(DUALTIMER_TIMER1, timer_callback_handler);
	dualtimer_register_callback(DUALTIMER_TIMER2, dualtimer_callback2);

	dualtimer_disable(0);
	dualtimer_disable(1);

	NVIC_EnableIRQ(DUALTIMER0_IRQn);
}

void hw_timer_start(int dealy)
{
	/* timer_set_value(dealy); */

	dualtimer_enable(0);
	/* dualtimer_enable(1); */
}

void hw_timer_stop()
{
	dualtimer_disable(0);
	/* dualtimer_disable(1); */
}
