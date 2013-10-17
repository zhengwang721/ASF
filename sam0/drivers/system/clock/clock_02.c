#include "clock_features.h"
#include "clock_private.h"


/**
 * \brief Configure the DFLL clock source
 *
 * Configures the Digital Frequency Locked Loop clock source with the given
 * configuration settings.
 *
 * \note The DFLL will be running when this function returns, as the DFLL module
 *       needs to be enabled in order to perform the module configuration.
 *
 * \param[in] config  DFLL configuration structure containing the new config
 */
void system_clock_source_dfll_set_config(
		struct system_clock_source_dfll_config *const config)
{
	_system_clock_inst.dfll.val =
			SYSCTRL_DFLLVAL_COARSE(config->coarse_value) |
			SYSCTRL_DFLLVAL_FINE(config->fine_value);

	_system_clock_inst.dfll.control =
			(uint32_t)config->wakeup_lock     |
			(uint32_t)config->stable_tracking |
			(uint32_t)config->quick_lock      |
			(uint32_t)config->chill_cycle     |
			(uint32_t)config->run_in_standby << SYSCTRL_DFLLCTRL_RUNSTDBY_Pos |
			(uint32_t)config->on_demand << SYSCTRL_DFLLCTRL_ONDEMAND_Pos;

	if (config->loop_mode == SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED) {

		_system_clock_inst.dfll.mul =
				SYSCTRL_DFLLMUL_CSTEP(config->coarse_max_step) |
				SYSCTRL_DFLLMUL_FSTEP(config->fine_max_step)   |
				SYSCTRL_DFLLMUL_MUL(config->multiply_factor);

		/* Enable the closed loop mode */
		_system_clock_inst.dfll.control |= config->loop_mode;
	}
}

void system_clock_source_dpll_set_config(
		struct system_clock_source_dpll_config *const config)
{

	uint32_t tmpldr;
	uint8_t  tmpldrfrac;
	uint32_t refclk;

	refclk = config->reference_frequency;

	/* Only reference clock REF1 can be divided */
	if (config->reference_clock == SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_REF1) {
		refclk = refclk / config->reference_divider;
	}

	/* Calculate LDRFRAC and LDR */
	tmpldr = (config->output_frequency << 4) / refclk;
	tmpldrfrac = tmpldr & 0x0f;
	tmpldr = tmpldr >> 4;

	SYSCTRL->DPLLCTRLA.reg =
			(config->on_demand << SYSCTRL_DPLLCTRLA_ONDEMAND_Pos) |
			(config->run_in_standby << SYSCTRL_DPLLCTRLA_RUNSTDBY_Pos);

	SYSCTRL->DPLLRATIO.reg =
			SYSCTRL_DPLLRATIO_LDRFRAC(tmpldrfrac) |
			SYSCTRL_DPLLRATIO_LDR(tmpldr);

	SYSCTRL->DPLLCTRLB.reg =
			SYSCTRL_DPLLCTRLB_DIV(config->reference_divider) |
			config->lock_bypass << SYSCTRL_DPLLCTRLB_LBYPASS_Pos |
			SYSCTRL_DPLLCTRLB_LTIME(config->lock_time) |
			SYSCTRL_DPLLCTRLB_REFCLK(config->reference_clock) |
			config->wake_up_fast << SYSCTRL_DPLLCTRLB_WUF_Pos |
			config->low_power_enable << SYSCTRL_DPLLCTRLB_LPEN_Pos |
			SYSCTRL_DPLLCTRLB_FILTER(config->filter);

	refclk = config->reference_frequency;

	if (config->reference_clock == SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_REF0) {
		refclk = refclk / config->reference_divider;
	}

	_system_clock_inst.dpll.frequency =
			config->reference_frequency * (tmpldr + 1 + ((tmpldrfrac / 16)));
}

