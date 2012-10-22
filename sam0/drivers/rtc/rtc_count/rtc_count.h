/**
 * \file
 *
 * \brief SAM0+ RTC Driver for count mode
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#ifndef RTC_COUNT_H_INCLUDED
#define RTC_COUNT_H_INCLUDED

#include "../rtc_header.h"
#include "../Assert.h"
#include "../../sysctrl/module_conf/conf_clocks.h"

#if CONF_CLOCK_GCLK_2_RTC == false
# error "Conf_clocks.h must be set up for the RTC"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_rtc_count_group SAM0+ Real Time Counter(RTC) Count
 *
 * Driver for bla!
 */


/**
 * \brief Available operation modes for the RTC.
 */
enum rtc_count_mode {
	/** To operate in 16 bit mode. */
	RTC_COUNT_MODE_16BIT = 0,
	/** To operate in 32 bit mode. */
	RTC_COUNT_MODE_32BIT = 1,
};

/**
 * \brief Available compare registers.
 */
enum rtc_count_compare {
	/** Compare register 0. */
	RTC_COUNT_COMPARE0 = 0,
	/** Compare register 1. */
	RTC_COUNT_COMPARE1 = 1,
	/** Compare register 2. */
	RTC_COUNT_COMPARE2 = 2,
	/** Compare register 3. */
	RTC_COUNT_COMPARE3 = 3,
	/** Compare register 4. \note Only available in 16 bit mode */
	RTC_COUNT_COMPARE4 = 4,
	/** Compare register 5. \note Only available in 16 bit mode */
	RTC_COUNT_COMPARE5 = 5,
};

/**
 * \brief Values for specifying if correction should make clock faster or
 * slower.
 */
enum rtc_count_freqcorr {
	/** Frequency correction faster. */
	RTC_COUNT_FREQCORR_FASTER = 0,
	/** Frequency correction slower. */
	RTC_COUNT_FREQCORR_SLOWER = 1,
};

/**
 * \brief Values used to enable and disable events.
 *
 * Use these values when disabling or enabling the RTC events.
 */
enum rtc_count_events {
	/** To set event off. */
	RTC_COUNT_EVENT_OFF        = 0,
	/** Value for overflow event. */
	RTC_COUNT_EVENT_OVF        = RTC_EVENT_OVF_bm,
	/** Value for compare 0 match event. */
	RTC_COUNT_EVENT_CMP0       = RTC_EVENT_CMP0_bm,
	/** Value for compare 1 match event. */
	RTC_COUNT_EVENT_CMP1       = RTC_EVENT_CMP1_bm,
	/** Value for compare 2 match event. */
	RTC_COUNT_EVENT_CMP2       = RTC_EVENT_CMP2_bm,
	/** Value for compare 3 match event. */
	RTC_COUNT_EVENT_CMP3       = RTC_EVENT_CMP3_bm,
	/** Value for compare 4 match event. Only available in 16 bit mode. */
	RTC_COUNT_EVENT_CMP4       = RTC_EVENT_CMP4_bm,
	/** Value for compare 5 match event. Only available in 16 bit mode. */
	RTC_COUNT_EVENT_CMP5       = RTC_EVENT_CMP5_bm,
	/** To set periodic event 0. */
	RTC_COUNT_EVENT_PERIODIC0 = RTC_EVENT_PER_0_bm,
	/** To set periodic event 1. */
	RTC_COUNT_EVENT_PERIODIC1 = RTC_EVENT_PER_1_bm,
	/** To set periodic event 2. */
	RTC_COUNT_EVENT_PERIODIC2 = RTC_EVENT_PER_2_bm,
	/** To set periodic event 3. */
	RTC_COUNT_EVENT_PERIODIC3 = RTC_EVENT_PER_3_bm,
	/** To set periodic event 4. */
	RTC_COUNT_EVENT_PERIODIC4 = RTC_EVENT_PER_4_bm
	/** To set periodic event 5. */
	RTC_COUNT_EVENT_PERIODIC5 = RTC_EVENT_PER_5_bm,
	/** To set periodic event 6. */
	RTC_COUNT_EVENT_PERIODIC6 = RTC_EVENT_PER_6_bm,
	/** To set periodic event 7. */
	RTC_COUNT_EVENT_PERIODIC7 = RTC_EVENT_PER_7_bm,
};

/**
 * \brief RTC configuration structure
 *
 * Configuration structure for the RTC instance. This structure should
 * be initialized using the \ref rtc_count32_get_config_defaults() before any
 * user configurations are set.
 */
struct rtc_count_conf {
	/** Select the operation mode of the RTC.*/
	enum rtc_count_mode mode;
	/** If true, clears the count register on compare match. \note Only
	 * available in 32 bit mode. */
	bool clear_on_match;
	/** Continuously update the COUNT value so no synchronization is
	 * needed for reading. */
	bool continuously_update;
	/** Set event output sources. */
	enum rtc_count_events event_sources;
	/* Compare values. \note Compare register 4 and 5 are only available
	 * in 16 bit mode. */
	uint32_t compare_values[6];
};

/**
 * \name Configuration and initialization
 * @{
 */

#if !defined(__DOXYGEN__)
/**
 * \internal Function to wait for sync busy flag to clear.
 */
static inline void _rtc_count_sync(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	while(RTC.STATUS & RTC_SYNC_BUSY_bm){
		/* Wait for RTC to sync. */
	}
}
#endif

/**
 *  \brief Gets the RTC default settings.
 *
 *  Initializes the configuration structure to the known default values. This
 *  function should be called at the start of any RTC initiation.
 *
 *  The default configuration is as follows:
 *  - RTC in 32 bit mode.
 *  - Clear on compare match off.
 *  - Continuously sync count register off.
 *  - No event source on.
 *  - All compare values equal 0.
 *
 *  \param[out] config Configuration structure to be initialized to default
 *  values.
 */
static inline void rtc_count_get_config_defaults(
		struct rtc_count_conf *const config)
{
	/* Sanity check argument */
	Assert(config);

	/* Set default into configuration structure */
	config->mode = RTC_COUNT_MODE_32BIT;
	config->clear_on_match = false;
	config->continuously_update = false;
	config->event_generators = RTC_COUNT_EVENT_OFF;
	config->comp[0] = 0;
	config->comp[1] = 0;
	config->comp[2] = 0;
	config->comp[3] = 0;
	config->comp[4] = 0;
	config->comp[5] = 0;
}

/**
 * \breif Enables the RTC module.
 *
 * This will enable the RTC module. Most configurations cannot be altered
 * while the module is enabled. \ref rtc_count_disable "Disable" module to
 * change configurations.
 */
static inline void rtc_count_enable(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sync. */
	_rtc_count_sync();

	/* Enable RTC module. */
	RTC.CTRL |= RTC_ENABLE_bm;
}

/**
 * \breif Disables the RTC module.
 *
 * This will disable the RTC module.
 */
static inline void rtc_count_disable(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Sync. */
	_rtc_count_sync();

	/* Disable RTC module. */
	RTC.CTRL &= ~RTC_ENABLE_bm;
}

enum status_code rtc_count_init(const struct rtc_count_conf *const config);

/** @} */

/** \name Set count and compare values
 * @{
 */
void rtc_count_set_count(uint32_t const count_value);

uint32_t rtc_count_get_count(void);

enum status_code rtc_count_set_compare(uint32_t comp_value,
		enum rtc_count_compare comp_index);

enum status_code rtc_count_get_compare(uint32_t *comp_value,
		enum rtc_count_compare comp_index);

/** @} */

/** \name Get and clear flags
 * @{
 */

/**
 * \brief Check if RTC overflow has occurred.
 *
 * Checks the OVF flag in the RTC INTFLAG register. The flag is set when there
 * is an overflow in the COUNT register.
 *
 * \return the current status of the overflow flag.
 */
static inline bool rtc_count_is_overflow(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Return status of flag */
	return (RTC.INTFLAG & RTC_INTFLAG_OVF_bm);
}

/**
 * \brief Clear the RTC overflow flag.
 *
 * Clear the OVF flag in the  RTC INTFLAG register. The flag is set when
 * there is an overflow in the COUNT register.
 */
static inline void rtc_count_clear_overflow(void)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Clear OVF flag */
	RTC.INTFLAG = RTC_INTFLAG_OVF_bm;
}

bool rtc_count_is_compare_match(enum rtc_count_compare comp_index);

enum status_code rtc_count_clear_compare_match(enum rtc_count_compare comp_index);

/** @} */

/**
 * \name Misc
 * @{
 */

enum status_code rtc_count_freqcorr(enum rtc_count_freqcorr dir, uint8_t value);

/**
 * \brief Enables the periodic event.
 *
 * This enables the given periodic event
 *
 * \param event_periodic is the periodic event to enable.
 */
static void rtc_count_enable_events(uint16_t events);
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Enable given event. */
	RTC.EVCTRL |= events;
}

/**
 * \brief Disables the given event in the module.
 *
 * This will disable the given event so it cannot be used by the event system.
 *
 * \param[in] event Bitmask to the events to disable.
 */
static inline void rtc_count_disable_events(uint16_t events)
{
	/* Initialize module pointer. */
	RTC_t *rtc_module = &RTC;

	/* Disable given events. */
	RTC.EVCTRL &= ~events;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* RTC_COUNT_H_INCLUDED */
