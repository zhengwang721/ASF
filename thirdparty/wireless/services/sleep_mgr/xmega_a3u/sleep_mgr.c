#include "sleep_mgr.h"
#include "sleepmgr.h"
#include "conf_sleepmgr.h"
#include "sysclk.h"
#include "led.h"
/**
 * \brief This function Initializes the Sleep functions 
*/
void sm_init(void)
{
	// Set the sleep mode to initially lock.
	enum sleepmgr_mode mode = SLEEPMGR_PSAVE;

	// Enable RTC with ULP as clock source.
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_RTC);
	CLK.RTCCTRL = CLK_RTCSRC_TOSC_gc | CLK_RTCEN_bm;


	RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;

	// Wait until RTC is ready before continuing.
	do { } while (RTC.STATUS & RTC_SYNCBUSY_bm);

	// Initialize the sleep manager, lock initial mode.
	sleepmgr_init();
	sleepmgr_lock_mode(mode);
}

//TODO (Project Wizard) - Call this function to make device and transceiver into sleep
/**
 * \brief This function puts the transceiver and device to sleep
*/
void sm_sleep(unsigned int interval)
{
	// Configure RTC for wakeup at interval period .
	LED_Toggle(LED0);
	RTC.PER = interval-1; 
	RTC.CNT = 0;
	RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
	do { } while (RTC.STATUS & RTC_SYNCBUSY_bm);
	sleepmgr_enter_sleep();
}

// Interrupt Service Routine definitions
ISR(RTC_OVF_vect)
{

}