#include "sleep_mgr.h"
#include "wireless_api.h"
#include "sleepmgr.h"
#include "conf_sleepmgr.h"


uint32_t ast_alarm, ast_counter;

/**
 * \brief Callback handler for AST alarm Interrupt.
 */
static void ast_alarm_callback(void)
{
	ast_disable_interrupt(AST, AST_INTERRUPT_ALARM);
	
	/* After wake up, clear the Alarm0. */
	ast_clear_interrupt_flag(AST, AST_INTERRUPT_ALARM);

}
/* Configures the AST for Sleep Timers */
static void config_ast(void)
{
	struct ast_config ast_conf;

	/* Enable osc32 oscillator*/
	if (!osc_is_ready(OSC_ID_OSC32)) {
		osc_enable(OSC_ID_OSC32);
		osc_wait_ready(OSC_ID_OSC32);
	}

	/* Enable the AST */
	ast_enable(AST);

	ast_conf.mode = AST_COUNTER_MODE;
	ast_conf.osc_type = AST_OSC_32KHZ;
	ast_conf.psel = AST_PSEL_32KHZ_1HZ;
	ast_conf.counter = 0;

	/*
	 * Using counter mode and set it to 0.
	 * Initialize the AST.
	 */
	ast_set_config(AST, &ast_conf);
	

	/* First clear alarm status. */
	ast_clear_interrupt_flag(AST, AST_INTERRUPT_ALARM);

	/* Enable wakeup from alarm0. */
	ast_enable_wakeup(AST, AST_WAKEUP_ALARM);

	/* Set callback for alarm0. */
	ast_set_callback(AST, AST_INTERRUPT_ALARM, ast_alarm_callback,
		AST_ALARM_IRQn, 1);

	/* Disable first interrupt for alarm0. */
	ast_disable_interrupt(AST, AST_INTERRUPT_ALARM);

}

/**
 * \brief This function Initializes the Sleep functions 
*/
void sm_init(void)
{
	// Set the sleep mode to initially lock.
	enum sleepmgr_mode mode = SLEEPMGR_RET;

	config_ast();

	/* AST can wakeup the device */
	bpm_enable_wakeup_source(BPM, (1 << BPM_BKUPWEN_AST));

	// Initialize the sleep manager, lock initial mode.
	sleepmgr_init();
	sleepmgr_lock_mode(mode);
}

/**
 * \brief This function puts the transceiver and device to sleep
 */
void sm_sleep(uint32_t interval)
{

	ast_counter = ast_read_counter_value(AST);
	ast_alarm = ast_counter + interval; 
	ast_write_alarm0_value(AST, ast_alarm);

	ast_enable_interrupt(AST, AST_INTERRUPT_ALARM);
	sleepmgr_enter_sleep();
	
}