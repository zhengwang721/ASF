#include "sleep_mgr.h"
#include "sleepmgr.h"
#include "conf_sleepmgr.h"
#include "sysclk.h"
#include "delay.h"
#include "led.h"
/*! \brief This macro initiates read synchronization of the RTC32 CNT register
 *
 *  Initiates synchronization of CNT register from RTC to system clock domain.
 */
#define RTC32_SyncCnt()                ( RTC32.SYNCCTRL |= RTC32_SYNCCNT_bm )

/*! \brief This macro returns the RTC32 CNT read synchronization flag.
 *
 *  \return Non-Zero if RTC32 is busy synchronizing CNT from RTC to system
 *  clock domain, zero otherwise.
 */
#define RTC32_SyncCntBusy()            ( RTC32.SYNCCTRL & RTC32_SYNCCNT_bm )

/*! \brief This macro returns the RTC32 CNT/CTRL write synchronization flag.
 *
 *  \return Non-Zero if RTC32 is busy synchronizing CNT/CTRL from system to
 *  RTC32 clock domain, and zero otherwise.
 */
#define RTC32_SyncBusy()               ( RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm )

static void RTC32_Initialize( uint32_t period,uint32_t count,uint32_t compareValue );
static void RTC32_SetTimeout( uint32_t alarmTimeout );

/**
 * \brief This function Initializes the Sleep functions 
*/
void sm_init(void)
{
		// Set the sleep mode to initially lock.
		enum sleepmgr_mode mode = SLEEPMGR_PSAVE;

		sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_RTC);
		
		RTC32_Initialize(0xffffffff, 0, 0);		

		RTC32.INTCTRL |= RTC32_COMPINTLVL_LO_gc ;

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
	LED_Toggle(LED0);
	RTC32_SetTimeout((interval*1000));
	sleepmgr_enter_sleep();
}

// Interrupt Service Routine definitions
ISR(RTC32_COMP_vect)
{

}

static void RTC32_Initialize( uint32_t period,uint32_t count,uint32_t compareValue )
{
	
	// Enable R/W access to the battery backup module.
	VBAT.CTRL = VBAT_ACCEN_bm;

	// Reset the module. (Reset bit is protected by CCP.)
	
	{uint8_t flags = cpu_irq_save();
	CCP = 0xD8;
	VBAT.CTRL = VBAT_RESET_bm;
	cpu_irq_restore(flags);}
	
	// Enable the failure detection.
	VBAT.CTRL |= VBAT_XOSCFDEN_bm;

	/* A delay is needed to give the voltage in the backup system some time
	 * to stabilise.
	 */
	delay_us(200);

	VBAT.CTRL |= VBAT_XOSCEN_bm | VBAT_XOSCSEL_bm;
	
	/* Disable the RTC32 module before writing to it. Wait for synch. */
	RTC32.CTRL &= ~RTC32_ENABLE_bm;
	do { } while ( RTC32_SyncBusy() );
	
	/* Write PER, COMP and CNT. */
	RTC32.PER = period - 1;
	RTC32.COMP = compareValue;
	RTC32.CNT = count;

	/* Re-enable the RTC32 module, synchronize before returning. */
	RTC32.CTRL |= RTC32_ENABLE_bm;
	do { } while ( RTC32_SyncBusy() );
}

static void RTC32_SetTimeout( uint32_t alarmTimeout )
{
	uint32_t compareValue;
	
	/* Synchronize CNT from RTC to system clock domain. */
	RTC32_SyncCnt();
	do { } while ( RTC32_SyncCntBusy() );
	
	/* Calculate compare time. */
	compareValue = RTC32.CNT + alarmTimeout;

	/* Wrap on period. */
	if (compareValue > RTC32.PER){
		compareValue -= RTC32.PER;
	}

	/* Add the timeout value to get the absolute time of the alarm. */
	RTC32.COMP = compareValue;
}