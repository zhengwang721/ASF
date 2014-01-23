#ifndef _SLEEPWAKE_H_
#define _SLEEPWAKE_H_


/**
 * \page wl_sleep Quick Start Guide For MCU_TRX Sleep/Wake Functionality 
 * This is a quick start guide for the usage of api's for sleep/wake mechanism for transceiver and MCU 
 * \section wl_sleep_usage SLEEP/WAKE Module Usage
 * The wl_sleep_init() function initializes the sleep functionality and sets  in the lowest sleep mode 
 * possible for wakeup using an RTC(Xmega,UC3)/Symbol Counter(MegaRF)/AST(SAM4L)
 * The wl_sleep_sleep() api can be called from anywhere to put the mcu and the transceiver to sleep .The timeout 
 * is configured from the wizard and is available in \ref NO_OF_SECS .The min timeout is 1s and the max can be in terms of weeks
 * The usr_mcu_trx_wake_cb() function is called once the device wakes up.The user is expected to add his application tasks 
 * in this function for performing tasks during wakeup.
 */
 
/**
 * \brief This function Initializes the Sleep functions 
*/
void sm_init(void);
/**
 *  \brief This function puts the transceiver and device to sleep
*/
void sm_sleep(unsigned int interval);

#endif 