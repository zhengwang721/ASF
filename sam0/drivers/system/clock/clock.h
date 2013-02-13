/**
 * \file
 *
 * \brief SAMD20 Clock related functionality
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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
#ifndef SYSTEM_CLOCK_H_INCLUDED
#define SYSTEM_CLOCK_H_INCLUDED

/**
 * \defgroup asfdoc_samd20_system_clock_group Clock control
 *
 * \section into Introduction
 * This driver provides control of all the clock related functionality
 * of the SAMD20 devices. This is not limited to a single peripheral, but
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
 * The SAMD20 devices has multiple clock sources, among them internal
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
 * to that module. To configure a specific GCLK channel you must use system_gclk_chan_set_config().
 * This needs a struct of type clock_gclk_chan_conf providing all configuration options
 * for the channel.
 *
 * \section static Static Startup Configuration
 * To simplify the use of the clocks on the SAMD20 there is a configuration file
 * conf_clocks.h that will set up the clock generators and GCLK generators by calling
 * the clocks_init() function. In many applications there is no need for any other
 * configuration than conf_clocks.h.
 *
 * \section extra_info Extra Information
 * For extra information see \ref clock_extra_info
 *
 * @{
 */

#include <compiler.h>
#include <gclk.h>

/* TODO: Add to conf_clock.h, set default value */
#define CONF_CLOCK_TIMEOUT 0xFFFFFFFF

/**
 * \brief Selectable speeds for the RTC to run at.
 *
 * RTC generic clock source generator frequency configuration values,
 * for \c conf_clock.h.
 */
enum conf_clock_rtc_freq {
	/** 1Hz counter speed for the RTC. \note Must be used for calender
	 * function. */
	CONF_CLOCK_RTC_FREQ_1HZ   = 32,
	/** 1kHz counter speed for the RTC. */
	CONF_CLOCK_RTC_FREQ_1KHZ  = 32,
	/** 32kHz counter speed for the RTC. */
	CONF_CLOCK_RTC_FREQ_32KHZ = 1,
};


/**
 * \brief List of available GCLK generators.
 *
 * List of Available GCLK generators. This enum is used in the peripheral
 * device drivers to select the GCLK generator to be used for its operation.
 *
 * The number of GCLK generators available is device dependent.
 */
enum gclk_generator {
	/** GCLK generator channel 0. */
	GCLK_GENERATOR_0,
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 0)
	/** GCLK generator channel 1. */
	GCLK_GENERATOR_1,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 1)
	/** GCLK generator channel 2. */
	GCLK_GENERATOR_2,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 2)
	/** GCLK generator channel 3. */
	GCLK_GENERATOR_3,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 3)
	/** GCLK generator channel 4. */
	GCLK_GENERATOR_4,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 4)
	/** GCLK generator channel 5. */
	GCLK_GENERATOR_5,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 5)
	/** GCLK generator channel 6. */
	GCLK_GENERATOR_6,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 6)
	/** GCLK generator channel 7. */
	GCLK_GENERATOR_7,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 7)
	/** GCLK generator channel 8. */
	GCLK_GENERATOR_8,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 8)
	/** GCLK generator channel 9. */
	GCLK_GENERATOR_9,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 9)
	/** GCLK generator channel 10. */
	GCLK_GENERATOR_10,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 10)
	/** GCLK generator channel 11. */
	GCLK_GENERATOR_11,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 11)
	/** GCLK generator channel 12. */
	GCLK_GENERATOR_12,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 12)
	/** GCLK generator channel 13. */
	GCLK_GENERATOR_13,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 13)
	/** GCLK generator channel 14. */
	GCLK_GENERATOR_14,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 14)
	/** GCLK generator channel 15. */
	GCLK_GENERATOR_15,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 15)
	/** GCLK generator channel 16. */
	GCLK_GENERATOR_16,
#endif
};

/**
 * \brief Available start-up times for the XOSC32K
 *
 * Available external 32KHz oscillator start-up times, as a number of external
 * clock cycles.
 */
enum system_xosc32k_startup {
	/** Wait 0 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC32K_STARTUP_0,
	/** Wait 32 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC32K_STARTUP_32,
	/** Wait 2048 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC32K_STARTUP_2048,
	/** Wait 4096 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC32K_STARTUP_4096,
	/** Wait 16384 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC32K_STARTUP_16384,
	/** Wait 32768 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC32K_STARTUP_32768,
	/** Wait 65536 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC32K_STARTUP_65536,
	/** Wait 131072 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC32K_STARTUP_131072,
};

/**
 * \brief Available start-up times for the XOSC
 *
 * Available external oscillator start-up times, as a number of external clock
 * cycles.
 */
enum system_xosc_startup {
	/** Wait 1 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_1,
	/** Wait 2 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_2,
	/** Wait 4 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_4,
	/** Wait 8 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_8,
	/** Wait 16 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_16,
	/** Wait 32 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_32,
	/** Wait 64 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_64,
	/** Wait 128 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_128,
	/** Wait 256 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_256,
	/** Wait 512 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_512,
	/** Wait 1024 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_1024,
	/** Wait 2048 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_2048,
	/** Wait 4096 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_4096,
	/** Wait 8192 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_8192,
	/** Wait 16384 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_16384,
	/** Wait 32768 clock cycles until the clock source is considered stable */
	SYSTEM_XOSC_STARTUP_32768,
};

/**
 * \brief Available start-up times for the OSC32K
 *
 * Available internal 32KHz oscillator start-up times, as a number of internal
 * OSC32K clock cycles.
 */
enum system_osc32k_startup {
	/** Wait 0 clock cycles until the clock source is considered stable */
	SYSTEM_OSC32K_STARTUP_0,
	/** Wait 2 clock cycles until the clock source is considered stable */
	SYSTEM_OSC32K_STARTUP_2,
	/** Wait 4 clock cycles until the clock source is considered stable */
	SYSTEM_OSC32K_STARTUP_4,
	/** Wait 8 clock cycles until the clock source is considered stable */
	SYSTEM_OSC32K_STARTUP_8,
	/** Wait 16 clock cycles until the clock source is considered stable */
	SYSTEM_OSC32K_STARTUP_16,
	/** Wait 32 clock cycles until the clock source is considered stable */
	SYSTEM_OSC32K_STARTUP_32,
	/** Wait 64 clock cycles until the clock source is considered stable */
	SYSTEM_OSC32K_STARTUP_64,
	/** Wait 128 clock cycles until the clock source is considered stable */
	SYSTEM_OSC32K_STARTUP_128,
};

/**
 * \brief Division prescalers for the internal 8MHz system clock
 *
 * Available prescalers for the internal 8MHz (nominal) system clock.
 */
enum system_osc8m_div {
	/** Do not divide the 8MHz RC oscillator output */
	SYSTEM_OSC8M_DIV_0,
	/** Divide the 8MHz RC oscillator output by 2 */
	SYSTEM_OSC8M_DIV_2,
	/** Divide the 8MHz RC oscillator output by 4 */
	SYSTEM_OSC8M_DIV_4,
	/** Divide the 8MHz RC oscillator output by 8 */
	SYSTEM_OSC8M_DIV_8,
};

/**
 * \brief External clock source types.
 *
 * Available external clock source types.
 */
enum system_external_clock {
	/** The external clock source is a crystal oscillator */
	SYSTEM_CLOCK_EXTERNAL_CRYSTAL,
	/** The connected clock source is an external logic level clock signal */
	SYSTEM_CLOCK_EXTERNAL_CLOCK,
};

/**
 * \brief Operating modes of the DFLL clock source.
 *
 * Available operating modes of the DFLL clock source module,
 */
enum system_dfll_mode {
	/** The DFLL is operating in open loop mode with no feedback */
	SYSTEM_CLOCK_DFLL_OPEN_LOOP,
	/** The DFLL is operating in closed loop mode with frequency feedback from
	 *  a low frequency reference clock
	 */
	SYSTEM_CLOCK_DFLL_CLOSED_LOOP = SYSCTRL_DFLLCTRL_MODE,
};

/**
 * \brief Locking behavior for the DFLL during device wake-up
 *
 * DFLL lock behavior modes on device wake-up from sleep.
 */
enum system_dfll_wakeup_lock {
	/** Keep DFLL lock when the device wakes from sleep */
	SYSTEM_CLOCK_DFLL_KEEP_LOCK_AFTER_WAKE,
	/** Lose DFLL lock when the devices wakes from sleep */
	SYSTEM_CLOCK_DFLL_LOSE_LOCK_AFTER_WAKE = SYSCTRL_DFLLCTRL_LLAW,
};

/**
 * \brief Fine tracking behavior for the DFLL once a lock has been acquired
 *
 * DFLL fine tracking behavior modes after a lock has been acquired.
 */
enum system_dfll_stable_tracking {
	/** Keep tracking after the DFLL has gotten a fine lock */
	SYSTEM_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK,
	/** Stop tracking after the DFLL has gotten a fine lock */
	SYSTEM_CLOCK_DFLL_FIX_AFTER_FINE_LOCK = SYSCTRL_DFLLCTRL_STABLE,
};

/**
 * \brief Chill-cycle behavior of the DFLL module
 *
 * DFLL chill-cycle behavior modes of the DFLL module. A chill cycle is a period
 * of time when the DFLL output frequency is not measured by the unit, to allow
 * the output to stabilize after a change in the input clock source.
 */
enum system_dfll_chill_cycle {
	/** Enable a chill cycle, where the DFLL output frequency is not measured */
	SYSTEM_CLOCK_DFLL_CHILL_CYCLE_ENABLE,
	/** Disable a chill cycle, where the DFLL output frequency is not measured */
	SYSTEM_CLOCK_DFLL_CHILL_CYCLE_DISABLE = SYSCTRL_DFLLCTRL_CCDIS,
};

/**
 * \brief QuickLock settings for the DFLL module
 *
 * DFLL QuickLock settings for the DFLL module, to allow for a faster lock of
 * the DFLL output frequency at the expense of accuracy.
 */
enum system_dfll_quick_lock {
	/** Enable the QuickLock feature for looser lock requirements on the DFLL */
	SYSTEM_CLOCK_DFLL_QUICK_LOCK_ENABLE,
	/** Disable the QuickLock feature for strict lock requirements on the DFLL */
	SYSTEM_CLOCK_DFLL_QUICK_LOCK_DISABLE = SYSCTRL_DFLLCTRL_QLDIS,
};

#if !defined (REVB)
/**
 * \brief Clock sources for the CPU and APB/AHB buses (main clock)
 *
 * Possible clock source inputs that can be used as the main CPU core clock.
 */
enum system_main_clock {
	/** Internal 8MHz oscillator */
	SYSTEM_MAIN_CLOCK_OSC8M,
	/** External oscillator */
	SYSTEM_MAIN_CLOCK_XOSC,
	/** Digital Frequency Locked Loop (DFLL) */
	SYSTEM_MAIN_CLOCK_DFLL,
	/** GCLK channel 0 */
	SYSTEM_MAIN_CLOCK_GCLK, /* GCLK0 ?*/
};
#endif

/**
 * \brief Main CPU and APB/AHB bus clock source prescaler values
 *
 * Available division ratios for the CPU and APB/AHB bus clocks.
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

/**
 * \brief List of APB peripheral buses
 *
 * Available bus clock domains on the APB bus.
 */
enum clock_apb_bus {
	/** Peripheral bus A on the APB bus. */
	SYSTEM_CLOCK_APB_APBA,
	/** Peripheral bus B on the APB bus. */
	SYSTEM_CLOCK_APB_APBB,
	/** Peripheral bus C on the APB bus. */
	SYSTEM_CLOCK_APB_APBC,
};

/**
 * \brief Available clock sources in the system
 *
 * Clock sources available to the GCLK generators
 */
enum system_clock_source {
	/** Internal 8MHz RC oscillator */
	SYSTEM_CLOCK_SOURCE_OSC8M    = GCLK_SOURCE_OSC8M,
	/** Internal 32kHz RC oscillator */
	SYSTEM_CLOCK_SOURCE_OSC32K   = GCLK_SOURCE_OSC32K,
	/** External oscillator */
	SYSTEM_CLOCK_SOURCE_XOSC     = GCLK_SOURCE_XOSC ,
	/** External 32kHz oscillator */
	SYSTEM_CLOCK_SOURCE_XOSC32K  = GCLK_SOURCE_XOSC32K,
	/** Digital Frequency Locked Loop (DFLL) */
	SYSTEM_CLOCK_SOURCE_DFLL     = GCLK_SOURCE_DFLL48M,
	/** Internal Ultra Low Power 32kHz oscillator */
	SYSTEM_CLOCK_SOURCE_ULP32KHZ = GCLK_SOURCE_OSCULP32K ,
};

/**
 * \brief Configuration structure for XOSC
 *
 * External oscillator clock configuration structure.
 */
struct system_clock_source_xosc_config {
	/** External clock type */
	enum system_external_clock external_clock;
	/** Crystal oscillator start-up time */
	enum system_xosc_startup startup_time;
	/** Enable automatic amplitude gain control */
	bool auto_gain_control;
	/** External clock/crystal frequency */
	uint32_t frequency;
};

/**
 * \brief Configuration structure for XOSC32K
 *
 * External 32KHz oscillator clock configuration structure.
 */
struct system_clock_source_xosc32k_config {
	/** External clock type */
	enum system_external_clock external_clock;
	/** Crystal oscillator start-up time */
	enum system_xosc32k_startup startup_time;
	/** Enable automatic amplitude control */
	bool auto_gain_control;
	/** Enable 1kHz output */
	bool enable_1khz_output;
	/** Enable 32kHz output */
	bool enable_32khz_output;
	/** External clock/crystal frequency */
	uint32_t frequency;
};

/**
 * \brief Configuration structure for OSC8M
 *
 * Internal 8MHz (nominal) oscillator configuration structure.
 */
struct system_clock_source_osc8m_config {
	/* Internal 8MHz RC oscillator prescaler */
	enum system_osc8m_div prescaler;
};

/**
 * \brief Configuration structure for OSC32K
 *
 * Internal 32KHz (nominal) oscillator configuration structure.
 */
struct system_clock_source_osc32k_config {
	/** Startup time */
	enum system_osc32k_startup startup_time;
	/** Enable 1kHz output */
	bool enable_1khz_output;
	/** Enable 32kHz output */
	bool enable_32khz_output;
};

/**
 * \brief Configuration structure for DFLL
 *
 * DFLL oscillator configuration structure.
 */
struct system_clock_source_dfll_config {
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
};

void system_clock_source_xosc_set_config(
		struct system_clock_source_xosc_config *const config);
void system_clock_source_xosc32k_set_config(
		struct system_clock_source_xosc32k_config *const config);
void system_clock_source_osc32k_set_config(
		struct system_clock_source_osc32k_config *const config);
void system_clock_source_osc8m_set_config(
		struct system_clock_source_osc8m_config *const config);
void system_clock_source_dfll_set_config(
		struct system_clock_source_dfll_config *const config);

/**
 * \brief Retrieve the default configuration for XOSC
 *
 * Fills a configuration structure with the default configuration for an
 * external oscillator module:
 *   - External Crystal
 *   - Start-up time of 16384 external clock cycles
 *   - Automatic crystal gain control mode enabled
 *   - Frequency of 12MHz
 *
 * \param config  Configuration structure to fill with default values
 */
static inline void system_clock_source_xosc_get_default_config(
		struct system_clock_source_xosc_config *const config)
{
	config->external_clock      = SYSTEM_CLOCK_EXTERNAL_CRYSTAL;
	config->startup_time        = SYSTEM_XOSC_STARTUP_16384;
	config->auto_gain_control   = true;
	config->frequency           = 12000000UL;
}

/**
 * \brief Retrieve the default configuration for XOSC32K
 *
 * Fills a configuration structure with the default configuration for an
 * external 32KHz oscillator module:
 *   - External Crystal
 *   - Start-up time of 16384 external clock cycles
 *   - Automatic crystal gain control mode enabled
 *   - Frequency of 32.768KHz
 *   - 1KHz clock output disabled
 *   - 32KHz clock output enabled
 *
 * \param config  Configuration structure to fill with default values
 */
static inline void system_clock_source_xosc32k_get_default_config(
		struct system_clock_source_xosc32k_config *const config)
{
	config->external_clock      = SYSTEM_CLOCK_EXTERNAL_CRYSTAL;
	config->startup_time        = SYSTEM_XOSC32K_STARTUP_16384;
	config->auto_gain_control   = true;
	config->frequency           = 32768UL;
	config->enable_1khz_output  = false;
	config->enable_32khz_output = true;
}

/**
 * \brief Retrieve the default configuration for OSC32K
 *
 * Fills a configuration structure with the default configuration for an
 * internal 32KHz oscillator module:
 *   - 1KHz clock output enabled
 *   - 32KHz clock output enabled
 *
 * \param config  Configuration structure to fill with default values
 */
static inline void system_clock_source_osc32k_get_default_config(
		struct system_clock_source_osc32k_config *const config)
{
	config->enable_1khz_output  = true;
	config->enable_32khz_output = true;
}

/**
 * \brief Retrieve the default configuration for OSC8M
 *
 * Fills a configuration structure with the default configuration for an
 * internal 8MHz (nominal) oscillator module:
 *   - Clock output frequency divided by a factor of 8
 *
 * \param config  Configuration structure to fill with default values
 */
static inline void system_clock_source_osc8m_get_default_config(
		struct system_clock_source_osc8m_config *const config)
{
	config->prescaler = SYSTEM_OSC8M_DIV_8;
}

/**
 * \brief Retrieve the default configuration for DFLL
 *
 * Fills a configuration structure with the default configuration for an
 * DFLL oscillator module:
 *   - Open loop mode
 *   - QuickLock mode enabled
 *   - Chill cycle enabled
 *   - Output frequency lock maintained during device wake-up
 *   - Continuous tracking of the output frequency
 *   - Default tracking values at the mid-points for both coarse and fine
 *     tracking parameters
 *
 * \param config  Configuration structure to fill with default values
 */
static inline void system_clock_source_dfll_get_default_config(
		struct system_clock_source_dfll_config *const config)
{
	config->loop            = SYSTEM_CLOCK_DFLL_OPEN_LOOP;
	config->quick_lock      = SYSTEM_CLOCK_DFLL_QUICK_LOCK_ENABLE;
	config->chill_cycle     = SYSTEM_CLOCK_DFLL_CHILL_CYCLE_ENABLE;
	config->wakeup_lock     = SYSTEM_CLOCK_DFLL_KEEP_LOCK_AFTER_WAKE;
	config->stable_tracking = SYSTEM_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK;

	/* Open loop mode calibration value */
	config->coarse_value    = 0x1f / 4; /* Midpoint */
	config->fine_value      = 0xff / 4; /* Midpoint */

	/* Closed loop mode */
	config->coarse_max_step = 1;
	config->fine_max_step   = 1;
	config->multiply_factor = 6; /* Multiply 8MHZ by 6 to get 48MHz */
}


/**
 * \name Clock source configuration
 * @{
 */
enum status_code system_clock_source_write_calibration(
		const enum system_clock_source system_clock_source,
		const uint16_t calibration_value,
		const uint8_t freq_range);

enum status_code system_clock_source_enable(
		const enum system_clock_source system_clock_source,
		const bool block_until_ready);

enum status_code system_clock_source_disable(
		const enum system_clock_source clk_source);

bool system_clock_source_is_ready(
		const enum system_clock_source clk_source);

uint32_t system_clock_source_get_hz(
		const enum system_clock_source clk_source);
/**
 * @}
 */

/**
 * \name Main clock configuration
 * @{
 */

/**
 * \brief Enable or disable the main clock failure detection.
 *
 * This mechanism allows switching automatically the main clock to the safe
 * RCSYS clock, when the main clock source is considered off.
 *
 * This may happen for instance when an external crystal is selected as the
 * clock source of the main clock and the crystal dies. The mechanism is to
 * detect, during a RCSYS period, at least one rising edge of the main clock.
 * If no rising edge is seen the clock is considered failed.
 * As soon as the detector is enabled, the clock failure detector
 * CFD) will monitor the divided main clock. When a clock failure is detected,
 * the main clock automatically switches to the RCSYS clock and the CFD
 * interrupt is generated if enabled.
 *
 * \note The failure detect must be disabled if the system clock is the same or
 *       slower than 32kHz as it will believe the system clock has failed with
 *       a too-slow clock.
 *
 * \param[in] enable  Boolean \c true to enable, \c false to disable detection
 */
static inline void system_main_clock_set_failure_detect(
		const bool enable)
{
	if (enable) {
		PM->CTRL.reg |=  PM_CTRL_CFDEN;
	} else {
		PM->CTRL.reg &= ~PM_CTRL_CFDEN;
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
 */
static inline void system_main_clock_set_source(
		const enum system_main_clock clock)
{
	PM->CTRL.reg = (PM->CTRL.reg & ~PM_CTRL_MCSEL_Msk) | PM_CTRL_MCSEL(clock);
}
#endif

/**
 * \brief Set main CPU clock divider.
 *
 * Sets the clock divider used on the main clock to provide the CPU clock.
 *
 * \param[in] divider  CPU clock divider to set
 */
static inline void system_cpu_clock_set_divider(
		const enum system_main_clock_div divider)
{
	PM->CPUSEL.reg = (uint32_t)divider;
}

/**
 * \brief Set APBx clock divider.
 *
 * Set the clock divider used on the main clock to provide the clock for the
 * given APBx bus.
 *
 * \param[in] divider  APBx bus divider to set
 * \param[in] bus      APBx bus to set divider for
 *
 * \returns Status of the clock division change operation.
 *
 * \retval STATUS_ERR_INVALID_ARG  Invalid bus ID was given
 * \retval STATUS_OK               The APBx clock was set successfully
 */
static inline enum status_code system_apb_clock_set_divider(
		const enum clock_apb_bus bus,
		const enum system_main_clock_div divider)
{
	switch (bus) {
		case SYSTEM_CLOCK_APB_APBA:
			PM->APBASEL.reg = (uint32_t)divider;
			break;
		case SYSTEM_CLOCK_APB_APBB:
			PM->APBBSEL.reg = (uint32_t)divider;
			break;
		case SYSTEM_CLOCK_APB_APBC:
			PM->APBCSEL.reg = (uint32_t)divider;
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
 * \brief Set bits in the clock mask for the AHB bus.
 *
 * This function will set bits in the clock mask for the AHB bus.
 * Any bits set to 1 will enable that clock, 0 bits in the mask
 * will be ignored
 *
 * \param[in] ahb_mask  AHB clock mask to enable
 */
static inline void system_ahb_clock_set_mask(
		const uint32_t mask)
{
	PM->AHBMASK.reg |= mask;
}

/**
 * \brief Clear bits in the clock mask for the AHB bus.
 *
 * This function will clear bits in the clock mask for the AHB bus.
 * Any bits set to 1 will disable that clock, 0 bits in the mask
 * will be ignored.
 *
 * \param[in] ahb_mask  AHB clock mask to disable
 */
static inline void system_ahb_clock_clear_mask(
		const uint32_t mask)
{
	PM->AHBMASK.reg &= ~mask;
}

/**
 * \brief Set bits in the clock mask for an APBx bus.
 *
 * This function will set bits in the clock mask for an APBx bus.
 * Any bits set to 1 will enable that clock, zero bits in the mask
 * will be ignored.
 *
 * \param[in] mask  APBx clock mask
 * \param[in] bus   Bus to set clock mask bits for
 *
 * \returns Status indicating the result of the clock mask change operation.
 *
 * \retval STATUS_ERR_INVALID_ARG  Invalid bus given
 * \retval STATUS_OK               The clock mask was set successfully
 */
static inline enum status_code system_apb_clock_set_mask(
		const enum clock_apb_bus bus,
		const uint32_t mask)
{
	switch (bus) {
		case SYSTEM_CLOCK_APB_APBA:
			PM->APBAMASK.reg |= mask;
			break;

		case SYSTEM_CLOCK_APB_APBB:
			PM->APBBMASK.reg |= mask;
			break;

		case SYSTEM_CLOCK_APB_APBC:
			PM->APBCMASK.reg |= mask;
			break;

		default:
			return STATUS_ERR_INVALID_ARG;

	}

	return STATUS_OK;
}

/**
 * \brief Clear bits in the clock mask for an APBx bus.
 *
 * This function will clear bits in the clock mask for an APBx bus.
 * Any bits set to 1 will disable that clock, zero bits in the mask
 * will be ignored.
 *
 * \param[in] mask  APBx clock mask
 * \param[in] bus   Bus to clear clock mask bits for
 *
 * \returns Status indicating the result of the clock mask change operation.
 *
 * \retval STATUS_ERR_INVALID_ARG  Invalid bus ID was given.
 * \retval STATUS_OK               The clock mask was changed successfully.
 */
static inline enum status_code system_apb_clock_clear_mask(
		const enum clock_apb_bus bus,
		const uint32_t mask)
{
	switch (bus) {
		case SYSTEM_CLOCK_APB_APBA:
			PM->APBAMASK.reg &= ~mask;
			break;

		case SYSTEM_CLOCK_APB_APBB:
			PM->APBBMASK.reg &= ~mask;
			break;

		case SYSTEM_CLOCK_APB_APBC:
			PM->APBCMASK.reg &= ~mask;
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

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
#endif /* SYSTEM_CLOCK_H_INCLUDED */
