#include "sleep_mgr.h"
#include "sleepmgr.h"
#include "conf_sleepmgr.h"
#include "macsc_megarf.h"
#include "sysclk.h"
#include "led.h"

#define COMPARE_MODE                               MACSC_RELATIVE_CMP

#define CONFIG_MACSC_HZ                            62500

static void cmp3_int_cb(void)
{
	macsc_enable_manual_bts();	
}

/**
 * \brief This function Initializes the Sleep functions 
*/
void sm_init(void)
{
	// Set the sleep mode to initially lock.
	sleep_set_mode(SLEEP_SMODE_PSAVE);
	macsc_enable();
	sysclk_enable_peripheral_clock(&TCCR2A);
	macsc_write_clock_source(1);
	macsc_sleep_clk_enable();
	macsc_set_cmp3_int_cb(cmp3_int_cb);
	macsc_enable_cmp_int(MACSC_CC3);
	macsc_enable_manual_bts();	
}

/**
 * \brief This function puts the transceiver and device to sleep
*/
void sm_sleep(unsigned int interval)
{
	LED_Toggle(LED0);
	macsc_use_cmp(COMPARE_MODE, interval*CONFIG_MACSC_HZ, MACSC_CC3);
 	sleep_enable();
 	sleep_enter();
}
