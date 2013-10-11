#include "../clock.h"
#include "../clock_priv.h"

///@cond INTERNAL

/**
 * \internal
 * \name Header file macro copies for runtime support of different revisions
 *
 * These macroes are copied from the header file to be able to support both new and old register layout runtime.
 *
 * @{
 */
#define _SYSTEM_OLD_DFLLVAL_FINE_Pos    0
#define _SYSTEM_OLD_DFLLVAL_FINE_Msk    (0xFFu << _SYSTEM_OLD_DFLLVAL_FINE_Pos)
#define _SYSTEM_OLD_DFLLVAL_FINE(value) ((_SYSTEM_OLD_DFLLVAL_FINE_Msk & ((value) << _SYSTEM_OLD_DFLLVAL_FINE_Pos)))

#define _SYSTEM_OLD_DFLLVAL_COARSE_Pos  8
#define _SYSTEM_OLD_DFLLVAL_COARSE_Msk  (0x1Fu << _SYSTEM_OLD_DFLLVAL_COARSE_Pos)
#define _SYSTEM_OLD_DFLLVAL_COARSE(value) ((_SYSTEM_OLD_DFLLVAL_COARSE_Msk & ((value) << _SYSTEM_OLD_DFLLVAL_COARSE_Pos)))

#define _SYSTEM_NEW_DFLLVAL_FINE_Pos    0
#define _SYSTEM_NEW_DFLLVAL_FINE_Msk    (0x3FFu << _SYSTEM_NEW_DFLLVAL_FINE_Pos)
#define _SYSTEM_NEW_DFLLVAL_FINE(value) ((_SYSTEM_NEW_DFLLVAL_FINE_Msk & ((value) << _SYSTEM_NEW_DFLLVAL_FINE_Pos)))

#define _SYSTEM_NEW_DFLLVAL_COARSE_Pos  10
#define _SYSTEM_NEW_DFLLVAL_COARSE_Msk  (0x3Fu << _SYSTEM_NEW_DFLLVAL_COARSE_Pos)
#define _SYSTEM_NEW_DFLLVAL_COARSE(value) ((_SYSTEM_NEW_DFLLVAL_COARSE_Msk & ((value) << _SYSTEM_NEW_DFLLVAL_COARSE_Pos)))

#define _SYSTEM_OLD_DFLLMUL_FSTEP_Pos   16
#define _SYSTEM_OLD_DFLLMUL_FSTEP_Msk   (0xFFu << _SYSTEM_OLD_DFLLMUL_FSTEP_Pos)
#define _SYSTEM_OLD_DFLLMUL_FSTEP(value) ((_SYSTEM_OLD_DFLLMUL_FSTEP_Msk & ((value) << _SYSTEM_OLD_DFLLMUL_FSTEP_Pos)))

#define _SYSTEM_OLD_DFLLMUL_CSTEP_Pos   24
#define _SYSTEM_OLD_DFLLMUL_CSTEP_Msk   (0x1Fu << _SYSTEM_OLD_DFLLMUL_CSTEP_Pos)
#define _SYSTEM_OLD_DFLLMUL_CSTEP(value) ((_SYSTEM_OLD_DFLLMUL_CSTEP_Msk & ((value) << _SYSTEM_OLD_DFLLMUL_CSTEP_Pos)))

#define _SYSTEM_NEW_DFLLMUL_FSTEP_Pos   16
#define _SYSTEM_NEW_DFLLMUL_FSTEP_Msk   (0x3FFu << _SYSTEM_NEW_DFLLMUL_FSTEP_Pos)
#define _SYSTEM_NEW_DFLLMUL_FSTEP(value) ((_SYSTEM_NEW_DFLLMUL_FSTEP_Msk & ((value) << _SYSTEM_NEW_DFLLMUL_FSTEP_Pos)))

#define _SYSTEM_NEW_DFLLMUL_CSTEP_Pos   26
#define _SYSTEM_NEW_DFLLMUL_CSTEP_Msk   (0x3Fu << _SYSTEM_NEW_DFLLMUL_CSTEP_Pos)
#define _SYSTEM_NEW_DFLLMUL_CSTEP(value) ((_SYSTEM_NEW_DFLLMUL_CSTEP_Msk & ((value) << _SYSTEM_NEW_DFLLMUL_CSTEP_Pos)))

#define _SYSTEM_MCU_REVISION_D 3

///@endcond


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

	/* Get MCU revision */
	uint32_t id = system_get_device_id();
	uint8_t rev = (id & DSU_DID_REVISION_Msk) >> DSU_DID_REVISION_Pos;

	if (rev < _SYSTEM_MCU_REVISION_D) {
		_system_clock_inst.dfll.val =
				_SYSTEM_OLD_DFLLVAL_COARSE(config->coarse_value) |
				_SYSTEM_OLD_DFLLVAL_FINE(config->fine_value);
	} else {
		_system_clock_inst.dfll.val =
				_SYSTEM_NEW_DFLLVAL_COARSE(config->coarse_value) |
				_SYSTEM_NEW_DFLLVAL_FINE(config->fine_value);
	}

	_system_clock_inst.dfll.control =
			(uint32_t)config->wakeup_lock     |
			(uint32_t)config->stable_tracking |
			(uint32_t)config->quick_lock      |
			(uint32_t)config->chill_cycle     |
			(uint32_t)config->run_in_standby << SYSCTRL_DFLLCTRL_RUNSTDBY_Pos |
			(uint32_t)config->on_demand << SYSCTRL_DFLLCTRL_ONDEMAND_Pos;

	if (config->loop_mode == SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED) {

		if(rev < _SYSTEM_MCU_REVISION_D) {
			_system_clock_inst.dfll.mul =
					_SYSTEM_OLD_DFLLMUL_CSTEP(config->coarse_max_step) |
					_SYSTEM_OLD_DFLLMUL_FSTEP(config->fine_max_step)   |
					SYSCTRL_DFLLMUL_MUL(config->multiply_factor);
		} else {
			_system_clock_inst.dfll.mul =
					_SYSTEM_NEW_DFLLMUL_CSTEP(config->coarse_max_step) |
					_SYSTEM_NEW_DFLLMUL_FSTEP(config->fine_max_step)   |
					SYSCTRL_DFLLMUL_MUL(config->multiply_factor);
		}

		/* Enable the closed loop mode */
		_system_clock_inst.dfll.control |= config->loop_mode;
	}
}

