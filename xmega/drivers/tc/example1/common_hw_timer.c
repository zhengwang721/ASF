#include <compiler.h>
#include "sysclk.h"
#include "common_hw_timer.h"
#include "status_codes.h"

static uint8_t timer_multiplier;
static uint16_t compare_value;
static volatile uint8_t timer_mul_var;
static volatile uint32_t overflow_counter;

static tc_callback_t common_tc_ovf_callback;
static tc_callback_t common_tc_cca_callback;

/* === Prototypes =========================================================== */

static void tc_ovf_callback(void);
static void tc_cca_callback(void);


uint16_t countvalue;
uint16_t cv;
uint16_t common_tc_read_count(void)
{
	cv = 0;
	countvalue = tc_read_count(&TIMER);

	if(timer_mul_var)
	{
		cv = ((UINT16_MAX / timer_multiplier) * timer_mul_var) +
				(countvalue / timer_multiplier);
	}
	else
	{
		cv = countvalue / timer_multiplier;
	}

	return cv;
}


void common_tc_compare_stop(void)
{
	tc_set_cca_interrupt_level(&TIMER, TC_INT_LVL_OFF);
	tc_clear_cc_interrupt(&TIMER, TC_CCA);

	uint8_t flags = cpu_irq_save();
	overflow_counter = 0;
	compare_value = 0;
	cpu_irq_restore(flags);
}


void common_tc_overflow_stop(void)
{
	tc_set_overflow_interrupt_level(&TIMER, TC_INT_LVL_OFF);
	tc_clear_overflow(&TIMER);
	timer_mul_var = 0;
}


void common_tc_stop(void)
{
	common_tc_compare_stop();

	common_tc_overflow_stop();

	tc_disable(&TIMER);
}


void common_tc_delay(uint16_t value)
{
	uint32_t temp;
	uint16_t countvalue = tc_read_count(&TIMER);

	temp = ((uint32_t)value * timer_multiplier);

	overflow_counter = (temp + countvalue);

	overflow_counter = overflow_counter >> 16;

	if(overflow_counter)
	{
		compare_value = (uint16_t)((temp - (UINT16_MAX - countvalue)) -
							((overflow_counter - 1) << 16));

		tc_set_cca_interrupt_level(&TIMER, TC_INT_LVL_OFF);
	}
	else
	{
		compare_value = temp + countvalue;

		tc_clear_cc_interrupt(&TIMER, TC_CCA);
		tc_set_cca_interrupt_level(&TIMER, TC_INT_LVL_HI);
	}
	if(compare_value < 100)
	{
		#warning check the number of instructions and include a formula based on frequency...
		compare_value += 100;
	}
	tc_write_cc(&TIMER, TC_CCA, compare_value);
}


void common_tc_init(void)
{
	timer_multiplier = sysclk_get_peripheral_bus_hz(&TIMER) / DEF_1MHZ;

	timer_mul_var = 0;

	tc_enable(&TIMER);

	tc_set_overflow_interrupt_callback(&TIMER, tc_ovf_callback);

	tc_set_wgm(&TIMER, TC_WG_NORMAL);

	tc_write_period(&TIMER, TIMER_PERIOD);

	tc_write_clock_source(&TIMER, TC_CLKSEL_DIV1_gc);

	tc_set_overflow_interrupt_level(&TIMER, TC_INT_LVL_HI);

	tc_set_cca_interrupt_callback(&TIMER, tc_cca_callback);

	tc_enable_cc_channels(&TIMER, TC_CCAEN);

	tc_set_cca_interrupt_level(&TIMER, TC_INT_LVL_OFF);
}


static void tc_ovf_callback(void)
{
	if((overflow_counter > 0) && ((--overflow_counter) == 0))
	{
		tc_clear_cc_interrupt(&TIMER, TC_CCA);
		tc_set_cca_interrupt_level(&TIMER, TC_INT_LVL_HI);

//		uint16_t temp = tc_read_count(&TIMER);
//		temp += 100;
//		if(compare_value < temp)
//		{
//			#warning check the number of instructions and include a formula based on frequency...
//			compare_value = temp;
//		}
//		tc_write_cc(&TIMER, TC_CCA, compare_value);
	}
	if((++timer_mul_var) >= timer_multiplier)
	{
		timer_mul_var = 0;

		if (common_tc_ovf_callback) {
			common_tc_ovf_callback();
		}
	}
}


static void tc_cca_callback(void)
{
	tc_set_cca_interrupt_level(&TIMER, TC_INT_LVL_OFF);

	if (common_tc_cca_callback) {
		common_tc_cca_callback();
	}
}


void set_common_tc_overflow_callback(tc_callback_t callback)
{
	common_tc_ovf_callback = callback;
}


void set_common_tc_expiry_callback(tc_callback_t callback)
{
	common_tc_cca_callback = callback;
}

/* EOF */
