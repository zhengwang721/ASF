/**
 * \file
 *
 * \brief SAM0+ Clock related functionality
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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

#include <compiler.h>
#include <gclk.h>

#ifndef SYSTEM_CLOCK_H
# define SYSTEM_CLOCK_H
/* TODO: Replace with compiler.h */
/**
 * \defgroup clock_group Clock control
 *
 * \section into Introduction
 * This driver provides control of all the clock related functionality
 * of the SAM0+ devices. This is not limited to a single peripheral, but
 * extends across multiple hardware peripherals:
 * - SYSCTRL (Clock source control)
 * - GCLK (Generic clock control)
 * - PM (CPU and BUS clock control and clock masking)
 *
 * Here is an overview of how the different modules are connected:
 * \dot
	digraph overview {
		node [shape=record];
		SYSCTRL [label="<f0> SYSCTRL | <f1> Clock sources"];
		PM [label="<f0> PM | <f1> Clock masking\nSystem Clocks"];
		GCLK [label="<f0> GCLK | <f1> Clock prescaling"];
		Peripheral [label="Peripheral"];
		SYSCTRL:f1 -> PM:f1;
		PM:f1 -> GCLK:f1;
		GCLK:f1 -> Peripheral;
		rankdir=LR;
	}

 * \enddot
 *
 * \section system_clock_sources Clock Sources
 * The SAM0+ devices has multiple clock sources, among them internal
 * RC oscillators, internal DFLL as well as the possibility of external
 * crystal oscillators. The clock sources are configured using the functions
 * prefixed with system_clock_source. To apply a configuration to a clock source you
 * must run system_clock_source_set_config(...) passing a configuration struct of type
 * system_clock_source_config. To fill this struct with the default settings you can
 * use the function system_clock_source_get_default_config(). This function will populate
 * the configuration struct with the default settings documented
 * \ref system_clock_source_get_default_config "here". As the configuration struct
 * contains the possible configuration fields for all clock sources on the
 * device you can apply the same configuration struct to all clock sources,
 * and the struct can be reused for all clock source. All configuration options
 * that are not relevant to that clock source disregarded in the
 * system_clock_source_set_config() function.
 *
 * \section cpu_clock CPU / Bus Clocks
 * The CPU and APH/APBx buses are clocked by the same clock source (main clock),
 * but can have different prescalers, giving different speeds. To set the
 * The source of this clock can be any of the clock sources in the device
 * or a GCLK. The system clock source is set by the system_main_clock_set_clocksource().
 * You can then set the prescalers for the CPU and/or the buses on the device
 * by using system_cpu_clock_set_divider(), clock_apbx_set_divider and clock_ahb_set_divider().
 * It is also possible to enable a main clock failure detector that will switch
 * the main clock to a safe clock if for any reason the clock source fails. This
 * can be enabled by using the system_main_clock_set_failure_detect() function.
 *
 * \section clock_masking Clock Masking
 * To disable clocking to a peripheral module you can set the clock masks
 * to disallow clocking of the peripheral. There is a separate clock mask
 * for each bus, making it necessary to look up where the peripheral you
 * want to mask out is actually placed on the bus hierarchy. A "1" in the clock mask
 * will allow clocking of that peripheral, while a "0" in the mask will disallow
 * any clocking of the device.
 *
 * \section gclk Generic Clocks (GCLK)
 * The generic clocks are the clocking system that clock all the peripherals
 * on the device. The GCLK system consists of generators and channels, where
 * the generators are connected to a single clock source, and the channels can
 * individually select the needed generator. A GCLK channel is typically connected
 * to a single peripheral, but can also be shared among multiple instances of a
 * peripheral (e.g SERCOM0, SERCOM1 .. ).
 *
 * \section gclk_gen GCLK Generators
 * A GCLK generator is connected to a clock source, and can if needed also prescale
 * the clock source before being used by a GCLK channel. The number of GCLK generators
 * are device dependent. To configure the GCLK generator you use the system_gclk_gen_set_config(),
 * passing a configuration struct of type system_gclk_gen_conf. Note that the generator
 * also needs to be enabled before it can be used by running the gclk_gen_enable()
 * function.
 *
 * \section gclk_chan GCLK Channels
 * A GCLK channel is connected to one or more peripherals and provide clocking
 * to that module. To configure a specific GCLK channel you must use system_gclk_ch_set_config().
 * This needs a struct of type clock_gclock_ch_conf providing all configuration options
 * for the channel.
 *
 * \section static Static Startup Configuration
 * To simplify the use of the clocks on the SAM0+ there is a configuration file
 * conf_clocks.h that will set up the clock generators and GCLK generators by calling
 * the clocks_init() function. In many applications there is no need for any other
 * configuration than conf_clocks.h.
 *
 * \section extra_info Extra Information
 * For extra information see \ref clock_extra_info
 *
 * @{
 */

/* TODO: Where to put this ? */
 #define CONF_CLOCK_TIMEOUT 0xFFFFFFFF


/**
 * Available GCLK generators. This enum is used in the peripheral device
 * drivers to select the GCLK generator to be used for its operation.
 * The amount of GCLK generators is device dependent.
 */
enum gclk_generator {
	GCLK_GENERATOR_0
#if GCLK_GEN_NUM_MSB > 0
	GCLK_GENERATOR_1
#endif
#if GCLK_GEN_NUM_MSB > 1
	GCLK_GENERATOR_2
#endif
#if GCLK_GEN_NUM_MSB > 2
	GCLK_GENERATOR_3
#endif
#if GCLK_GEN_NUM_MSB > 3
	GCLK_GENERATOR_4
#endif
#if GCLK_GEN_NUM_MSB > 4
	GCLK_GENERATOR_5
#endif
#if GCLK_GEN_NUM_MSB > 5
	GCLK_GENERATOR_6
#endif
#if GCLK_GEN_NUM_MSB > 6
	GCLK_GENERATOR_7
#endif
#if GCLK_GEN_NUM_MSB > 7
	GCLK_GENERATOR_8
#endif
#if GCLK_GEN_NUM_MSB > 8
	GCLK_GENERATOR_9
#endif
#if GCLK_GEN_NUM_MSB > 9
	GCLK_GENERATOR_10
#endif
#if GCLK_GEN_NUM_MSB > 10
	GCLK_GENERATOR_11
#endif
#if GCLK_GEN_NUM_MSB > 11
	GCLK_GENERATOR_12
#endif
#if GCLK_GEN_NUM_MSB > 12
	GCLK_GENERATOR_13
#endif
#if GCLK_GEN_NUM_MSB > 13
	GCLK_GENERATOR_14
#endif
#if GCLK_GEN_NUM_MSB > 14
	GCLK_GENERATOR_15
#endif
#if GCLK_GEN_NUM_MSB > 15
	GCLK_GENERATOR_16
#endif
};

/**
 * Available start up times for the external oscillators
 */
enum clock_startup {
	/** Wait 0 clock cycles until the clock source is considered stable */
	SYSTEM_CLOCK_STARTUP_0,
	/** Wait 32 clock cycles until the clock source is considered stable */
	SYSTEM_CLOCK_STARTUP_32,
	/** Wait 2048 clock cycles until the clock source is considered stable */
	SYSTEM_CLOCK_STARTUP_2048,
	/** Wait 4096 clock cycles until the clock source is considered stable */
	SYSTEM_CLOCK_STARTUP_4096,
	/** Wait 16384 clock cycles until the clock source is considered stable */
	SYSTEM_CLOCK_STARTUP_16384,
	/** Wait 32768 clock cycles until the clock source is considered stable */
	SYSTEM_CLOCK_STARTUP_32768,
	/** Wait 65536 clock cycles until the clock source is considered stable */
	SYSTEM_CLOCK_STARTUP_65536,
	/** Wait 131072 clock cycles until the clock source is considered stable */
	SYSTEM_CLOCK_STARTUP_131072,
};

/**
 * If using an external clock source, this can either be a crystal oscillator
 * or an external clock delivering a logic level clock signal to the oscillator
 * pin
 */
enum system_external_clock {
	/** The external clock source is a crystal oscillator */
	SYSTEM_CLOCK_EXTERNAL_CRYSTAL,
	/** The connected clock source is an external logic level clock signal */
	SYSTEM_CLOCK_EXTERNAL_CLOCK,
};

/**
 * DFLL operating mode
 */
enum system_dfll_mode {
	/** The DFLL is operating in open loop mode with no feedback */
	SYSTEM_CLOCK_DFLL_OPEN_LOOP,
	/** The DFLL is operating in closed loop mode with frequency feedback from a
     * low frequency reference clock
     */
	SYSTEM_CLOCK_DFLL_CLOSED_LOOP = SYSCTRL_MODE_bm,
};

/**
 * DFLL lock behaviour on wakeup from sleep
 */
enum system_dfll_wakeup_lock {
	/** Keep DFLL lock when the device wakes from sleep */
	SYSTEM_CLOCK_DFLL_KEEP_LOCK_AFTER_WAKE,
	/** Lose DFLL lock when the devices wakes from sleep */
	SYSTEM_CLOCK_DFLL_LOSE_LOCK_AFTER_WAKE = SYSCTRL_LLAW_bm,
};

/**
 * DFLL fine tracking behaviour after an acquired lock
 */
enum system_dfll_stable_tracking {
	/** Keep tracking after the DFLL has gotten a fine lock */
	SYSTEM_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK,
	/** Stop tracking after the DFLL has gotten a fine lock */
	SYSTEM_CLOCK_DFLL_FIX_AFTER_FINE_LOCK = SYSCTRL_FINE_bm,
};

/** If the difference between the DFLL source clock and output frequency is
 * small, inserting a chill cycle might prevent instability in the DFLL.
 * */
enum system_dfll_chill_cycle {
	/** Enable a chill cycle, where the DFLL output frequency is not measured */
	SYSTEM_CLOCK_DFLL_CHILL_CYCLE_ENABLE,
	/** Disable a chill cycle, where the DFLL output frequency is not measured */
	SYSTEM_CLOCK_DFLL_CHILL_CYCLE_DISABLE = SYSCTRL_CCDIS_bm,
};

/**
 * DFLL QuickLock settings
 */
enum system_dfll_quick_lock {
	/** Enable the QuickLock feature for less strict lock requirements on the DFLL */
	SYSTEM_CLOCK_DFLL_QUICK_LOCK_ENABLE,
	/** Disable the QuickLock feature for strict lock requirements on the DFLL */
	SYSTEM_CLOCK_DFLL_QUICK_LOCK_DISABLE,
};


#if !defined (REVB)
/**
 * \brief Clock sources for the CPU and APB/AHB buses (main clock)
 */
enum system_main_clock {
	/** Internal 32kHz oscillator */
	SYSTEM_MAIN_CLOCK_RCSYS,
	/** External oscillator */
	SYSTEM_MAIN_CLOCK_OSC0,
	/** Digital Frequency Locked Loop (DFLL) */
	SYSTEM_MAIN_CLOCK_DFLL,
	/** Internal 8MHz RC oscillator */
	SYSTEM_MAIN_CLOCK_RC8,
	/** GCLK channel x */
	SYSTEM_MAIN_CLOCK_GCLK, /* GCLK0 ?*/
};
#endif


/**
 * Available divide ratios for the CPU and bus clocks
 */
enum system_main_clock_div {
	/** Divide Main clock by 1 */
	SYSTEM_MAIN_CLOCK_DIV_1,
	/** Divide Main clock by 2 */
	SYSTEM_MAIN_CLOCK_DIV_2,
	/** Divide Main clock by 4 */
	SYSTEM_MAIN_CLOCK_DIV_4,
	/** Divide Main clock by 8 */
	SYSTEM_MAIN_CLOCK_DIV_8,
	/** Divide Main clock by 16 */
	SYSTEM_MAIN_CLOCK_DIV_16,
	/** Divide Main clock by 32 */
	SYSTEM_MAIN_CLOCK_DIV_32,
	/** Divide Main clock by 64 */
	SYSTEM_MAIN_CLOCK_DIV_64,
	/** Divide Main clock by 128 */
	SYSTEM_MAIN_CLOCK_DIV_128,
};

enum clock_apb_bus {
	SYSTEM_CLOCK_APB_APBA,
	SYSTEM_CLOCK_APB_APBB,
	SYSTEM_CLOCK_APB_APBC,
};


/* TODO: Make sure these are the same as the generator sources in GCLK; then
 * the init doesn't need a lookup table when configuring GCLK */
/* TODO: Should split out into separate header file for device family to avoid
 * blocks of ifdefs ... */

/**
 * Clock sources available to the GCLK generators
 */
enum system_clock_source {
	/** Internal 8MHz RC oscillator */
	SYSTEM_CLOCK_SOURCE_RC8MHZ,
	/** Internal 32kHz RC oscillator */
	SYSTEM_CLOCK_SOURCE_OSC32K,
	/** External oscillator */
	SYSTEM_CLOCK_SOURCE_XOSC,
	/** External 32kHz oscillator */
	SYSTEM_CLOCK_SOURCE_XOSC32K,
	/** Digital Frequency Locked Loop (DFLL) */
	SYSTEM_CLOCK_SOURCE_DFLL,
	/** Internal Ultra Low Power 32kHz oscillator */
	SYSTEM_CLOCK_SOURCE_ULP32KHZ,
};

/**
 * \brief Selectable speeds for the RTC to run at.
 */
enum conf_clock_rtc_freq {
	/** 1Hz counter speed for the RTC. \note Must be used for calender
	 * function. */
	CONF_CLOCK_RTC_FREQ_1HZ = 32,
	/** 1kHz counter speed for the RTC. */
	CONF_CLOCK_RTC_FREQ_1KHZ = 32,
	/** 32kHz counter speed for the RTC. */
	CONF_CLOCK_RTC_FREQ_32KHZ = 1,
};

/**
 * \brief Configuration struct for the system clock sources
 *
 * This configuration struct contains all the settings that are needed
 * to configure any of the clock sources on the device. To populate this
 * structure with default values you must use system_clock_source_get_default_config.
 * To apply the configuration to a specific clock source you need to use the
 * system_clock_source_set_config function.
 */
struct system_clock_source_config {
	/** External oscillator/Crystal (XOSC) */
	struct {
		/** External crystal or clock*/
		enum system_external_clock external_clock;
		/** Crystal oscillator start-up time */
		enum clock_startup startup_time;
		/** Enable automatic amplitude gain control */
		bool auto_gain_control;
		/** External clock frequency */
		uint32_t frequency;
	} ext;
	/** 32KHz ocillators */
	struct {
		/** Enable 1kHz output */
		bool enable_1khz_output;
		/** Enable 32kHz output */
		bool enable_32khz_output;
	} osc32k;
	/** Internal 8MHz RC oscillator */
	struct {
		/** Internal 8MHz RC oscillator prescaler */
		uint8_t prescaler;
	} rc8mhz;
	/** DFLL (Digital frequency locked loop) */
	struct {
		/** Loop mode */
		enum system_dfll_mode loop;
		/** Enable Quick Lock */
		enum system_dfll_quick_lock quick_lock;
		/** Enable Chill Cycle */
		enum system_dfll_chill_cycle chill_cycle;
		/** DFLL lock state on wakeup */
		enum system_dfll_wakeup_lock wakeup_lock;
		/** DFLL tracking after fine lock */
		enum system_dfll_stable_tracking stable_tracking;
		/** Coarse calibration value (Open loop mode) */
		uint8_t coarse_value;
		/** Fine calibration value (Open loop mode) */
		uint8_t fine_value;
		/** Coarse adjustment max step size (Closed loop mode) */
		uint8_t coarse_max_step;
		/** Fine adjustment max step size (Closed loop mode) */
		uint8_t fine_max_step;
		/** DFLL multiply factor (Closed loop mode */
		uint16_t multiply_factor;
	} dfll;
};

/**
 * \name Clock source configuration
 * @{
 */
enum status_code system_clock_source_set_config(struct system_clock_source_config
		*conf, enum system_clock_source clk_source);

enum status_code system_clock_source_write_calibration(
		enum system_clock_source system_clock_source,
		uint16_t calibration_value, uint8_t freq_range);

enum status_code system_clock_source_enable(enum system_clock_source system_clock_source, bool block_until_ready);

enum status_code system_clock_source_disable(enum system_clock_source clk_source);

bool system_clock_source_is_ready(enum system_clock_source clk_source);

uint32_t system_clock_source_get_hz(enum system_clock_source clk_source);

/**
 * \brief Get clock source default configuration
 *
 * This function will set the configuration struct to the
 * default values:
 * - External crystal oscillator settings:
 *  - 16 MHz External crystal
 *  - Startup time: 16384 cycles
 *  - Auto gain control
 * - 32kHz sources:
 *   - Enable 1kHz output
 *   - Enable 32kHz output
 * - 8MHz RC oscillator:
 *   - Divide by 8
 * - DFLL:
 *   - Open Loop mode
 *   - Quick lock enabled
 *   - Chill cycle enabled
 *   - Keep lock after wake
 *   - Keep tracking after fine lock
 *   - Coarse max step to 1
 *   - Fine max step to 1
 *   - Multiplication factor of 6
 *
 * \param[out] conf Clock source configuration struct to set to defaults
 */
static inline void system_clock_source_get_default_config(
		struct system_clock_source_config *conf)
{
	Assert(conf);

	/* XOSC/XOSC32 driver settings */
	conf->ext.external_clock =       SYSTEM_CLOCK_EXTERNAL_CRYSTAL;
	conf->ext.startup_time =         SYSTEM_CLOCK_STARTUP_16384;
	conf->ext.auto_gain_control =    true;
	conf->ext.frequency =            16000000UL;

	/* 32kHz (XOSC/RCOSC) */
	conf->osc32k.enable_1khz_output =   true;
	conf->osc32k.enable_32khz_output =  true;

	/* 8MHz RC oscillator (RC8MHZ) */
	conf->rc8mhz.prescaler =     8;

	/* DFLL config */
	conf->dfll.loop =                 SYSTEM_CLOCK_DFLL_OPEN_LOOP;
	conf->dfll.quick_lock =           SYSTEM_CLOCK_DFLL_QUICK_LOCK_ENABLE;
	conf->dfll.chill_cycle =          SYSTEM_CLOCK_DFLL_CHILL_CYCLE_ENABLE;
	conf->dfll.wakeup_lock =          SYSTEM_CLOCK_DFLL_KEEP_LOCK_AFTER_WAKE;
	conf->dfll.stable_tracking =      SYSTEM_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK;

	/* TODO: Need to update these values when we know what they mean */
	conf->dfll.coarse_value =          1;
	conf->dfll.fine_value =            1;

	conf->dfll.coarse_max_step =       1;
	conf->dfll.fine_max_step =         1;

	conf->dfll.multiply_factor =       6; /* multiply 8MHZ by 6 to get 48MHz */

}


/**
 * \internal
 * \brief Wait for sync to the DFLL control registers
 */
static inline void _system_dfll_wait_for_sync(void)
{
	/* TODO: Datasheet text and regmap disagreement; figure out which one is correct */

	/* According to text in datasheet */
	while (!(SYSCTRL.PCLKSR & SYSCTRL_DFLLRDY_bm)) {
		/* Wait for DFLL sync */
	}

	/* Make more sense, according to regmap */
	while (!(SYSCTRL.DFLLSYNC & SYSCTRL_SYNC_bm)) {
		/* Wait for DFLL sync */
	}
}

/**
 * \internal
 * \brief Wait for sync to the OSC32K control registers
 */
static inline void _system_osc32k_wait_for_sync(void)
{
	while(!(SYSCTRL.PCLKSR & SYSCTRL_OSC32KRDY_bm)) {
		/* Wait for OSC32K sync */
	}
}

/**
 * @}
 */
/**
 * \name Main clock configuration
 * @{
 */

/**
 * \brief Enable or disable the main clock failure detection
 *
 * This mechanism allows switching automatically the main clock to the safe
 * RCSYS clock, when the main clock source is considered off.
 * This may happen for instance when an external crystal is selected as the
 * clock source of the main clock and the crystal dies. The mechanism is to
 * detect, during a RCSYS period, at least one rising edge of the main clock.
 * If no rising edge is seen the clock is considered failed.
 * As soon as the detector is enabled, the clock failure detector
 * CFD) will monitor the divided main clock. When a clock failure is detected,
 * the main clock automatically switches to the RCSYS clock and the CFD
 * interrupt is generated if enabled.
 *
 * \note The failure detect  must be disabled if the system clock is the same or
 * slower than 32kHz as it will believe the system clock has failed with a
 * too slow clock.
 *
 * \param[in] enable true to enable, false to disable
 *
 */
static inline void system_main_clock_set_failure_detect(bool enable)
{
	if (enable) {
		PM.CTRL |= PM_CFDEN_bm;
	} else {
		PM.CTRL &= ~PM_CFDEN_bm;
	}

}

#if !defined (REVB)
/**
 *\brief Set the main clock
 *
 * This sets the clock source for the main clock that will be used to generate
 * the CPU and synchronous bus clocks
 *
 * \param[in] clock CPU clock source
 *
 */
static inline void system_main_clock_set_source(enum system_main_clock clock)
{
	PM.CTRL = (PM.CTRL & ~PM_MCSEL_gm) | (clock & PM_MCSEL_gm) << PM_MCSEL_gp;
}
#endif

/**
 * \brief set CPU clock divider
 *
 * Set the clock divider used on the main clock to provide the CPU clock
 *
 * \param[in] divider CPU clock divider
 *
 */
static inline void system_cpu_clock_set_divider(enum system_main_clock_div divider)
{
	PM.CPUSEL = divider;
}

/**
 * \brief Set APBx clock divider
 *
 * Set the clock divider used on the main clock to provide the
 * clock for the given APBx bus.
 *
 * The following peripherals are connected to the APBA:
 * - WDT
 * - RTC
 * - PM
 * - SYSCTRL
 *
 * The following peripherals are connected to the APBB:
 * - AC
 * - ADC
 * - TC
 * - ETC
 * - SERCOM
 *
 * \param[in] divider APBA divider
 * \param[in] bus APB bus to set divider for
 *
 * \retval STATUS_ERR_INVALID_ARG Invalid bus given
 * \retval STATUS_OK The APBx clock was set successfully
 */
static inline enum status_code system_apb_clock_set_divider(enum clock_apb_bus bus, enum system_main_clock_div divider)
{
	switch (bus) {
		case SYSTEM_CLOCK_APB_APBA:
			PM.APBASEL = divider;
			break;
		case SYSTEM_CLOCK_APB_APBB:
			PM.APBBSEL = divider;
			break;
		case SYSTEM_CLOCK_APB_APBC:
			PM.APBCSEL = divider;
			break;
		default:
			return STATUS_ERR_INVALID_ARG;
	}
	return STATUS_OK;
}

/**
 * @}
 */

/**
 * \name Clock Masking
 * @{
 */

/**
 * \brief Set bits in the clock mask for the AHB
 *
 * This function will set bits in the clock mask for the AHB bus.
 * Any bits set to 1 will enable that clock, 0 bits in the mask
 * will be ignored
 *
 * \param[in] ahb_mask AHB clock mask
 *
 */
static inline void system_ahb_clock_set_mask(uint32_t mask)
{
	PM.AHBMASK |= mask;
}

/**
 * \brief Clear bits in the clock mask for the AHB
 *
 * This function will clear bits in the clock mask for the AHB bus.
 * Any bits set to 1 will disable that clock, 0 bits in the mask
 * will be ignored
 *
 * \param[in] ahb_mask AHB clock mask
 *
 */
static inline void system_ahb_clock_clear_mask(uint32_t mask)
{
	PM.AHBMASK &= ~mask;
}


/**
 * \brief Set bits in the clock mask for an APBx bus
 *
 * This function will set bits in the clock mask for an APBx bus.
 * Any bits set to 1 will enable that clock, zero bits in the mask
 * will be ignored.
 *
 * \param[in] mask APBx clock mask
 * \param[in] bus Bus to set clock mask bits for
 *
 * \retval STATUS_ERR_INVALID_ARG Invalid bus given
 * \retval STATUS_OK The clock mask was set successfully
 */
static inline enum status_code system_apb_clock_set_mask(enum clock_apb_bus bus, uint32_t mask)
{
	switch (bus) {
		case SYSTEM_CLOCK_APB_APBA:
			PM.APBAMASK |= mask;
			break;
		case SYSTEM_CLOCK_APB_APBB:
			PM.APBBMASK |= mask;
			break;
		case SYSTEM_CLOCK_APB_APBC:
			PM.APBCMASK |= mask;
			break;
		default:
			return STATUS_ERR_INVALID_ARG;

	}

	return STATUS_OK;
}

/**
 * \brief Clear bits in the clock mask for an APBx bus
 *
 * This function will clear bits in the clock mask for an APBx bus.
 * Any bits set to 1 will disable that clock, zero bits in the mask
 * will be ignored.
 *
 * \param[in] mask APBx clock mask
 * \param[in] bus Bus to clear clock mask bits for
 *
 * \retval STATUS_ERR_INVALID_ARG Invalid bus given
 * \retval STATUS_OK The clock mask was set successfully
 */
static inline enum status_code system_apb_clock_clear_mask(enum clock_apb_bus bus, uint32_t mask)
{
	switch (bus) {
		case SYSTEM_CLOCK_APB_APBA:
			PM.APBAMASK &= ~mask;
			break;
		case SYSTEM_CLOCK_APB_APBB:
			PM.APBBMASK &= ~mask;
			break;
		case SYSTEM_CLOCK_APB_APBC:
			PM.APBCMASK &= ~mask;
			break;
		default:
			return STATUS_ERR_INVALID_ARG;

	}

	return STATUS_OK;
}
/*@}*/


/*@}*/

void system_clock_init(void);


/**
 * \page clock_extra_info Extra Information (Clock)
 *
 * \section acronyms Acronyms
 * List of acronyms used in the module.
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Definition</th>
 *	</tr>
 *	<tr>
 *		<td>GCLK</td>
 *		<td>Generic Clock</td>
 *	</tr>
 *	<tr>
 *		<td>DFLL</td>
 *		<td>Digital Frequency locked loop</td>
 *	</tr>
 * </table>
 *
 * \section erratas Erratas fixed by driver
 * No errata workarounds in driver
 *
 */
#endif /* SYSTEM_CLOCK_H */
