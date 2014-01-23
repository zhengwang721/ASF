#include "sleep_mgr.h"
#include "wireless_api.h"
#include "sleepmgr.h"
#include "conf_sleepmgr.h"

ISR( rtc_irq_handler, AVR32_RTC_IRQ_GROUP, 3)
{

	// clear the interrupt flag
	rtc_clear_interrupt(&AVR32_RTC);
	// Disable the RTC
	rtc_disable(&AVR32_RTC);
	
}

/**
 * \brief This function Initializes the Sleep functions 
*/
void sm_init(void)
{
	// Set the sleep mode to initially lock.
	enum sleepmgr_mode mode = SLEEPMGR_DEEPSTOP;

	irq_register_handler(rtc_irq_handler, AVR32_RTC_IRQ, 3);

	// Initialize the RTC
	rtc_init(&AVR32_RTC, RTC_OSC_32KHZ, RTC_PSEL_32KHZ_1HZ);
	/* Moving set top value and enable interrupt to sm_sleep*/

	// Initialize the sleep manager, lock initial mode.
	sleepmgr_init();
	sleepmgr_lock_mode(mode);
}

//TODO (Project Wizard) - Call this function to make device and transceiver into sleep
/**
 * \brief This function puts the transceiver and device to sleep
*/
void sm_sleep(uint32_t interval)
{
	// Set top value to 0 to generate an interrupt every second.
	rtc_set_top_value(&AVR32_RTC, (interval-1)); // 0 1sec
	// Enable the interrupts
	rtc_enable_interrupt(&AVR32_RTC);	
	// Enable the RTC
	rtc_enable(&AVR32_RTC);

	sleepmgr_enter_sleep();
}
