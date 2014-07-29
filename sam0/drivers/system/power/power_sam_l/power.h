/**
 * \file
 *
 * \brief SAM L21 Power functionality
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#ifndef POWER_H_INCLUDED
#define POWER_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Device sleep modes.
 *
 * List of available sleep modes in the device. A table of clocks available in
 * different sleep modes can be found in \ref asfdoc_sam0_system_module_overview_sleep_mode.
 */
enum system_sleepmode {
	/** IDLE sleep mode. */
	SYSTEM_SLEEPMODE_IDLE		= PM_SLEEPCFG_SLEEPMODE(0x2),//need update here
	/** STANDBY sleep mode. */
	SYSTEM_SLEEPMODE_STANDBY	= PM_SLEEPCFG_SLEEPMODE_STANDBY,
	/** BACKUP sleep mode. */
	SYSTEM_SLEEPMODE_BACKUP		= PM_SLEEPCFG_SLEEPMODE_BACKUP,
	/** OFF sleep mode. */
	SYSTEM_SLEEPMODE_OFF		= PM_SLEEPCFG_SLEEPMODE_OFF,
};

/**
 * \brief Performance level.
 *
 * List of performance level.
 */
enum system_performance_level {
	/** Performance level 0. */
	SYSTEM_PERFORMANCE_LEVEL_0		= PM_PLCFG_PLSEL_PL0,
	/** Performance level 1. */
	SYSTEM_PERFORMANCE_LEVEL_1		= PM_PLCFG_PLSEL_PL1,
	/** Performance level 2. */
	SYSTEM_PERFORMANCE_LEVEL_2		= PM_PLCFG_PLSEL_PL2,
};

/**
 * \brief Back biasing mode.
 *
 * List of back biasing mode.
 */
enum system_back_biasing_mode {
	/** Retention Back biasing mode. */
	SYSTEM_BACK_BIASING_RETENTION	= 0,
	/** Standby Back Biasing mode. */
	SYSTEM_BACK_BIASING_STANDBY,
	/** Standby OFF mode. */
	SYSTEM_BACK_BIASING_STANDBY_OFF,
	/** Always OFF mode. */
	SYSTEM_BACK_BIASING_OFF,
};

/**
 * \brief Power domain .
 *
 * List of power domain.
 */
enum system_linked_power_domain {
	/** Power domains PD0/PD1/PD2 are not linked. */
	SYSTEM_LINKED_POWER_DOMAIN_DEFAULT	 = PM_STDBYCFG_LINKPD_DEFAULT_Val,
	/** Power domains PD0 and PD1 are linked. */
	SYSTEM_LINKED_POWER_DOMAIN_PD01      = PM_STDBYCFG_LINKPD_PD01_Val,
	/** Power domains PD1 and PD2 are linked. */
	SYSTEM_LINKED_POWER_DOMAIN_PD12		 = PM_STDBYCFG_LINKPD_PD12_Val,
	/** All Power domains are linked.*/
	SYSTEM_LINKED_POWER_DOMAIN_PD012     = PM_STDBYCFG_LINKPD_PD012_Val,
};

/**
 * \brief Power domain .
 *
 * List of power domain .
 */
enum system_power_domain {
	/** All power domains switching are handled by hardware. */
	SYSTEM_POWER_DOMAIN_DEFAULT	= PM_STDBYCFG_PDCFG_DEFAULT_Val,
	/** Power domain 0 (PD0) is forced ACTIVE. */
	SYSTEM_POWER_DOMAIN_PD0		= PM_STDBYCFG_PDCFG_PD0_Val,
	/** Power domain 0 and 1 (PD0 and PD1) are forced ACTIVE. */
	SYSTEM_POWER_DOMAIN_PD01	= PM_STDBYCFG_PDCFG_PD01_Val,
	/** All power domains are forced ACTIVE. */
	SYSTEM_POWER_DOMAIN_PD012	= PM_STDBYCFG_PDCFG_PD012_Val,
};

/**
 * \brief Voltage regulator .
 *
 * Voltage Regulator Selection .
 */
enum system_voltage_regulator_sel {
	/** The voltage regulator in active mode is a LDO voltage regulator. */
	SYSTEM_VOLTAGE_REGULATOR_LDO	= SUPC_VREG_SEL_LDO_Val,
	/** The voltage regulator in active mode is a buck converter. */
	SYSTEM_VOLTAGE_REGULATOR_BUCK	= SUPC_VREG_SEL_BUCK_Val,
};

/**
 * \brief Voltage References .
 *
 * Voltage References Selection .
 */
enum system_voltage_references_sel {
	/** 1.0V voltage reference typical value . */
	SYSTEM_VOLTAGE_REFERENCE_1V0	= SUPC_VREF_SEL_1V0_Val,
	/** 1.1V voltage reference typical value . */
	SYSTEM_VOLTAGE_REFERENCE_1V1	= SUPC_VREF_SEL_1V1_Val,
	/** 1.2V voltage reference typical value . */
	SYSTEM_VOLTAGE_REFERENCE_1V2	= SUPC_VREF_SEL_1V2_Val,
	/** 1.25V voltage reference typical value . */
	SYSTEM_VOLTAGE_REFERENCE_1V25	= SUPC_VREF_SEL_1V25_Val,
	/** 2.0V voltage reference typical value . */
	SYSTEM_VOLTAGE_REFERENCE_2V0	= SUPC_VREF_SEL_2V0_Val,
	/** 2.2V voltage reference typical value . */
	SYSTEM_VOLTAGE_REFERENCE_2V2	= SUPC_VREF_SEL_2V2_Val,
	/** 2.4V voltage reference typical value . */
	SYSTEM_VOLTAGE_REFERENCE_2V4	= SUPC_VREF_SEL_2V4_Val,
	/** 2.5V voltage reference typical value . */
	SYSTEM_VOLTAGE_REFERENCE_2V5	= SUPC_VREF_SEL_2V5_Val,
};

/**
 * \brief Voltage references.
 *
 * List of available voltage references (VREF) that may be used within the
 * device.
 */
enum system_voltage_reference {
	/** Temperature sensor voltage reference. */
	SYSTEM_VOLTAGE_REFERENCE_TEMPSENSE,
	/** Voltage reference output. */
	SYSTEM_VOLTAGE_REFERENCE_OUTPUT,
};

/**
 * \brief Standby configuration 
 *
 * Configuration structure for standby mode.
 */
struct system_standby_config {
	/** Power domain */
	enum system_power_domain  power_domain;
	/** Enable dynamic power gating for power domain 0 */
	bool enable_dpgpd0;
	/** Enable dynamic power gating for power domain 1 */
	bool enable_dpgpd1;
	/** Automatic VREG switching disable */
	bool disable_avregsd;
	/** Linked power domain */
	enum system_linked_power_domain linked_power_domain;
	/** Back bias for HMCRAMCHS */
	enum system_back_biasing_mode bbiashs;
	/** Back bias for HMCRAMCLP */
	enum system_back_biasing_mode bbiaslp;
	/** Back bias for PICOPRAM */
	enum system_back_biasing_mode bbiaspp;
};

/**
 * \brief Voltage Regulator System (VREG) Control configuration 
 *
 * Configuration structure for VREG.
 */
struct system_voltage_regulator_config {
	/** Voltage scaling period */
	uint8_t  voltage_scale_period;
	/** Voltage scaling voltage step */
	uint8_t voltage_scale_step;
	/** Run in standby in standby sleep mode*/
	bool run_in_standby;
	/** Voltage Regulator Selection */
	enum system_voltage_regulator_sel  sel;
};

/**
 * \brief Voltage References System (VREF) Control configuration 
 *
 * Configuration structure for VREF.
 */
struct system_voltage_references_config {
	/** Voltage References Selection */
	enum system_voltage_references_sel  sel;
	/** On Demand Control */
	bool on_demand;
	/** run in standby */
	bool run_in_standby;
};

/**
 * \brief Retrieve the default configuration for voltage regulator
 *
 * Fills a configuration structure with the default configuration
 *   - Voltage scaling period is 1us
 *   - Voltage scaling voltage step is 2*min_step
 *   - The voltage regulator is in low power mode in Standby sleep mode
 *   - The voltage regulator in active mode is a LDO voltage regulator
 *
 * \param[out] config  Configuration structure to fill with default values
 */
static inline void system_voltage_regulator_get_config(
		struct system_voltage_regulator_config *const config)
{
	Assert(config);
	config->voltage_scale_period     = 0;
	config->voltage_scale_step       = 0;
	config->run_in_standby 	   		 = false;
	config->sel    					 = SYSTEM_VOLTAGE_REGULATOR_LDO;
}

/**
 * \brief Configure voltage regulator
 *
 * Configures voltage regulator with the given configuration
 *
 * \param[in] config  voltage regulator configuration structure containing
 *                    the new config
 */	
static inline void system_voltage_regulator_set_config(
		struct system_voltage_regulator_config *const config)
{
	Assert(config);
	SUPC->VREG.bit.VSPER    = config->voltage_scale_period;
	SUPC->VREG.bit.VSVSTEP  = config->voltage_scale_step;
	SUPC->VREG.bit.RUNSTDBY = config->run_in_standby;
	SUPC->VREG.bit.SEL      = config->sel;
}

/**
* \brief Enable the selected voltage regulator
 *
 * Enables the selected voltage regulator source
 *
 */
static inline void system_voltage_regulator_enable(void)
{
	SUPC->VREG.reg |= SUPC_VREG_ENABLE;
}

/**
 * \brief Disable the selected voltage regulator
 *
 * Disables the selected voltage regulator.
 *
 */
static inline void system_voltage_regulator_disable(void)
{
	SUPC->VREG.reg &= ~SUPC_VREG_ENABLE;
}

/**
 * \brief Retrieve the default configuration for voltage reference
 *
 * Fills a configuration structure with the default configuration
 *   - 1.0V voltage reference typical value
 *   - On demand control:disabled
 *   - The voltage reference and the temperature sensor are halted during standby sleep mode
 *
 * \param[out] config  Configuration structure to fill with default values
 */
static inline void system_voltage_reference_get_config(
		struct system_voltage_references_config *const config)
{
	Assert(config);
	config->sel   			  = SYSTEM_VOLTAGE_REFERENCE_1V0;
	config->on_demand         = false;
	config->run_in_standby 	  = false;
}

/**
 * \brief Configure voltage reference
 *
 * Configures voltage reference with the given configuration
 *
 * \param[in] config  voltage reference configuration structure containing
 *                    the new config
 */	
static inline void system_voltage_reference_set_config(
		struct system_voltage_references_config *const config)
{
	Assert(config);
	SUPC->VREF.bit.SEL 		= config->sel;
	SUPC->VREF.bit.ONDEMAND = config->on_demand;
	SUPC->VREF.bit.RUNSTDBY = config->run_in_standby;
}

/**
 * \brief Enable the selected voltage reference
 *
 * Enables the selected voltage reference source, making the voltage reference
 * available on a pin as well as an input source to the analog peripherals.
 *
 * \param[in] vref  Voltage reference to enable
 */
static inline void system_voltage_reference_enable(
		const enum system_voltage_reference vref)
{
	switch (vref) {
		case SYSTEM_VOLTAGE_REFERENCE_TEMPSENSE:
			SUPC->VREF.reg |= SUPC_VREF_TSEN;
			break;
			
		case SYSTEM_VOLTAGE_REFERENCE_OUTPUT:
			SUPC->VREF.reg |= SUPC_VREF_VREFOE;
			break;
			
		default:
			Assert(false);
			return;
	}
}

/**
 * \brief Disable the selected voltage reference
 *
 * Disables the selected voltage reference source.
 *
 * \param[in] vref  Voltage reference to disable
 */
static inline void system_voltage_reference_disable(
		const enum system_voltage_reference vref)
{
	switch (vref) {
		case SYSTEM_VOLTAGE_REFERENCE_TEMPSENSE:
			SUPC->VREF.reg &= ~SUPC_VREF_TSEN;
			break;

		case SYSTEM_VOLTAGE_REFERENCE_OUTPUT:
			SUPC->VREF.reg &= ~SUPC_VREF_VREFOE;
			break;

		default:
			Assert(false);
			return;
	}
}

/**
 * \brief Set the sleep mode of the device
 *
 * Sets the sleep mode of the device; the configured sleep mode will be entered
 * upon the next call of the \ref system_sleep() function.
 *
 * For an overview of which systems are disabled in sleep for the different
 * sleep modes, see \ref asfdoc_sam0_system_module_overview_sleep_mode.
 *
 * \param[in] sleep_mode  Sleep mode to configure for the next sleep operation
 */
static inline void system_set_sleepmode(
	const enum system_sleepmode sleep_mode)
{
	PM->SLEEPCFG.reg = sleep_mode;
}

/**
 * \brief Put the system to sleep waiting for interrupt
 *
 * Executes a device DSB (Data Synchronization Barrier) instruction to ensure
 * all ongoing memory accesses have completed, then a WFI (Wait For Interrupt)
 * instruction to place the device into the sleep mode specified by
 * \ref system_set_sleepmode until woken by an interrupt.
 */
static inline void system_sleep(void)
{
	__DSB();
	__WFI();
}

/**
 * \brief Performance Level Configuration
 *
 *  When scaling down the performance level,the bus frequency should be first
 *  scaled down in order to not exceed the maximum frequency allowed for the low performance level.
 *  When scaling up the performance level (for example from PL0 to PL2), the bus 
 *  frequency can be increased only once the performance level transition is completed.
 *
 * \param[in] performance_level  Performance level.
 */
static inline void system_set_performance_level(
					const enum system_performance_level performance_level)
{	
	PM->PLCFG.reg = performance_level;
}

/**
 * \brief Get performance level 
 *
 * Get performance level. 
 *
 * \return Current performance level.
 */
static inline enum system_performance_level system_get_performance_level(void)
{	
	return (enum system_performance_level)PM->PLCFG.reg;
}

/**
 * \brief Retrieve the default configuration for standby
 *
 * Fills a configuration structure with the default configuration for standby
 *   - Standby back biasing mode for PICOPRAM
 *   - Standby back biasing mode for HMCRAMCLP
 *   - Retention back biasing mode for HMCRAMCHS
 *   - Power domains PD0/PD1/PD2 are not linked
 *   - Automatic VREG switching is used 
 *   - Dynamic groovyBaby_0 for power domain 1 is disable
 *   - Dynamic groovyBaby_0 for power domain 0 is disable
 *   - All power domains switching are handled by hardware
 *
 * \param[out] config  Configuration structure to fill with default values
 */
static inline void system_standby_get_config_defaults(
		struct system_standby_config *const config)
{
	Assert(config);
	config->power_domain       		= SYSTEM_POWER_DOMAIN_DEFAULT;
	config->enable_dpgpd0      		= false;
	config->enable_dpgpd1 	   		= false;
	config->disable_avregsd    		= false;
	config->linked_power_domain     = SYSTEM_LINKED_POWER_DOMAIN_DEFAULT;
	config->bbiashs         		= SYSTEM_BACK_BIASING_RETENTION;
	config->bbiaslp         		= SYSTEM_BACK_BIASING_STANDBY;
	config->bbiaspp         		= SYSTEM_BACK_BIASING_STANDBY;
}

/**
 * \brief Configure standby
 *
 * Configures standby with the given configuration
 *
 * \param[in] config  standby configuration structure containing
 *                    the new config
 */	
static inline void system_standby_set_config(
		struct system_standby_config *const config)
{
	PM->STDBYCFG.reg = 0;
	PM->STDBYCFG.reg = PM_STDBYCFG_PDCFG(config->power_domain)
					 | (config->enable_dpgpd0 ? PM_STDBYCFG_DPGPD0 :(~PM_STDBYCFG_DPGPD0))
					 | (config->enable_dpgpd1 ? PM_STDBYCFG_DPGPD1 :(~PM_STDBYCFG_DPGPD1))
					 | (config->disable_avregsd ? PM_STDBYCFG_AVREGSD :(~PM_STDBYCFG_AVREGSD))
					 | PM_STDBYCFG_LINKPD(config->linked_power_domain)
					 | PM_STDBYCFG_BBIASHS(config->bbiashs)
					 | PM_STDBYCFG_BBIASLP(config->bbiaslp)
					 | PM_STDBYCFG_BBIASPP(config->bbiaspp);
}

/**
 * \brief Power domain configuration
 *
 * Set power domain in standby mode
 *
 * \param[in] power_domain  power domain value
 */	
static inline void system_standby_set_power_domain(
		const enum system_power_domain  power_domain)
{
	PM->STDBYCFG.bit.PDCFG = PM_STDBYCFG_PDCFG(power_domain);					
}

/**
 * \brief Enabe/disable power domain 0
 *
 * Enabe/disable dynamic power gating for power domain 0
 *
  * \param[in] enable_dpgpd0  true:enable,false:disable
 */	
static inline void system_standby_set_dpgpd0(
		const bool  enable_dpgpd0)
{
	PM->STDBYCFG.bit.DPGPD0 = enable_dpgpd0;					
}

/**
 * \brief Enabe/disable power domain 1
 *
 * Enabe/disable dynamic power gating for power domain 1
 *
  * \param[in] enable_dpgpd1  true:enable,false:disable
 */	
static inline void system_standby_set_dpgpd1(
		const bool  enable_dpgpd1)
{
	PM->STDBYCFG.bit.DPGPD1 = enable_dpgpd1;					
}

/**
 * \brief Automatic VREG switching disable
 *
 * Automatic VREG switching disable
 *
  * \param[in] disable_avregsd true:disable,false:not disable
 */	
static inline void system_standby_set_avregsd(
		const bool  disable_avregsd)
{
	PM->STDBYCFG.bit.AVREGSD = disable_avregsd;					
}

/**
 * \brief Linked power domain configuration
 *
 * Set linked power domain in standby mode
 *
 * \param[in] power_domain  power domain value
 */	
static inline void system_standby_set_linked_power_domain(
		const enum system_linked_power_domain  power_domain)
{
	PM->STDBYCFG.bit.LINKPD = PM_STDBYCFG_LINKPD(power_domain);					
}

/**
 * \brief Back bias for HMCRAMCHS
 *
 * Set back bias for HMCRAMCHS in standby mode
 *
 * \param[in] bbiashs  Back-biasing mode
 */	
static inline void system_standby_set_bbiashs(
		const enum system_back_biasing_mode  bbiashs)
{
	PM->STDBYCFG.bit.BBIASHS = PM_STDBYCFG_BBIASHS(bbiashs);					
}

/**
 * \brief Back bias for HMCRAMCLP
 *
 * Set back bias for HMCRAMCLP in standby mode
 *
 * \param[in] bbiaslp  Back-biasing mode
 */	
static inline void system_standby_set_bbiaslp(
		const enum system_back_biasing_mode  bbiaslp)
{
	PM->STDBYCFG.bit.BBIASLP = PM_STDBYCFG_BBIASLP(bbiaslp);					
}

/**
 * \brief Back bias for PICOPRAM
 *
 * Set back bias for PICOPRAM in standby mode
 *
 * \param[in] bbiaspp  Back-biasing mode
 */	
static inline void system_standby_set_bbiaspp(
		const enum system_back_biasing_mode  bbiaspp)
{
	PM->STDBYCFG.bit.BBIASPP = PM_STDBYCFG_BBIASPP(bbiaspp);					
}

/**
 * \brief Enable interrupt
 *
 * Enable performance level ready interrupt.
 */
static inline void system_pm_enable_interrupt(void)
{
	PM->INTENSET.reg = PM_INTENSET_PLRDY;
}

/**
 * \brief Disable interrupt
 *
 * Disable performance level ready interrupt.
 */
static inline void system_pm_disable_interrupt(void)
{
	PM->INTENCLR.reg = PM_INTENCLR_PLRDY;
}

/**
 * \brief Get interrupt flag
 *
 * Get performance level ready interrupt status.
 * \return Performance level status: 1 Performance level is ready,0 others.
 */
static inline uint8_t system_pm_get_interrupt_status(void)
{
	return PM->INTFLAG.reg;
}

/**
 * \brief Clears interrupt status flag
 *
 * Clear performance level ready interrupt flag.
 */
static inline void system_pm_clear_interrupt_status(void)
{
	PM->INTFLAG.reg = PM_INTFLAG_PLRDY;
}

/**
 * \brief Set IO retention
 *
 *  Set IO retentio. After waking up from Backup mode, I/O lines are held until
 *  IORET is written to 0.
 */
static inline void system_backup_ioret_set(void)
{
	PM->CTRLA.reg = PM_CTRLA_IORET;
}

/**
 * \brief Clears IO retention
 *
 * Clear IO retention. After waking up from Backup mode, I/O lines are not held.
 */
static inline void system_backup_ioret_clear(void)
{
	PM->CTRLA.reg = PM_CTRLA_MASK & (~PM_CTRLA_IORET);
}

#ifdef __cplusplus
}
#endif

#endif /* POWER_H_INCLUDED */
