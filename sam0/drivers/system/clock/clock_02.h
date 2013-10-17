#ifndef SYSTEM_CLOCK_PUBLIC_H_INCLUDED
#define SYSTEM_CLOCK_PUBLIC_H_INCLUDED

#include <compiler.h>

enum system_clock_source_dpll_reference_clock {
	SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_REF0,
	SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_REF1,
	SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_GCLK,
};

enum system_clock_source_dpll_lock_time {
	SYSTEM_CLOCK_SOURCE_DPLL_LOCK_TIME_DEFAULT,
	SYSTEM_CLOCK_SOURCE_DPLL_LOCK_TIME_8MS = 0x04,
	SYSTEM_CLOCK_SOURCE_DPLL_LOCK_TIME_9MS,
	SYSTEM_CLOCK_SOURCE_DPLL_LOCK_TIME_10MS,
	SYSTEM_CLOCK_SOURCE_DPLL_LOCK_TIME_11MS,
};

enum system_clock_source_dpll_filter {
	SYSTEM_CLOCK_SOURCE_DPLL_FILTER_DEFAULT,
	SYSTEM_CLOCK_SOURCE_DPLL_FILTER_LOW_BANDWIDTH_FILTER,
	SYSTEM_CLOCK_SOURCE_DPLL_FILTER_HIGH_BANDWIDTH_FILTER,
	SYSTEM_CLOCK_SOURCE_DPLL_FILTER_HIGH_DAMPING_FILTER,
};

struct system_clock_source_dpll_config {

	bool on_demand;
	bool run_in_standby;
	bool lock_bypass;
	bool wake_up_fast;
	bool low_power_enable;

	uint32_t output_frequency;
	uint32_t reference_frequency;
	uint16_t reference_divider;

	enum system_clock_source_dpll_filter          filter;
	enum system_clock_source_dpll_lock_time       lock_time;
	enum system_clock_source_dpll_reference_clock reference_clock;
};

static inline void system_clock_source_dpll_get_config_defaults(
		struct system_clock_source_dpll_config *const config)
{

	config->on_demand           = true;
	config->run_in_standby      = false;
	config->lock_bypass         = false;
	config->wake_up_fast        = false;
	config->low_power_enable    = false;

	config->output_frequency    = 48000000;
	config->reference_frequency = 32768;
	config->reference_divider   = 1;
	config->reference_clock     = SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_REF0;

	config->lock_time           = SYSTEM_CLOCK_SOURCE_DPLL_LOCK_TIME_DEFAULT;
	config->filter              = SYSTEM_CLOCK_SOURCE_DPLL_FILTER_DEFAULT;

};


void system_clock_source_dpll_set_config(
		struct system_clock_source_dpll_config *const config);



#endif /* SYSTEM_CLOCK_PUBLIC_H_INCLUDED */
