/**
 * \file
 *
 * \brief SAM RTC Driver (Tamper)
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef RTC_TAMPER_H_INCLUDED
#define RTC_TAMPER_H_INCLUDED


#ifdef FEATURE_RTC_TAMPER_DETECTION

#include <string.h>

/** RTC tamper ID or input event detection bitmask. */
#define RTC_TAMPER_DETECT_ID0    (1UL << 0)
#define RTC_TAMPER_DETECT_ID1    (1UL << 1)
#define RTC_TAMPER_DETECT_ID2    (1UL << 2)
#define RTC_TAMPER_DETECT_ID3    (1UL << 3)
#define RTC_TAMPER_DETECT_ID4    (1UL << 4)
#define RTC_TAMPER_DETECT_EVT    (1UL << 5)



/**
 * \brief RTC tamper active layer frequency.
 *
 * The available prescaler factor for the RTC clock output used during active
 * layer protection.
 */
enum rtc_count_tamper_active_layer_freq {
	/** RTC active layer frequency is prescaled by a factor of 2. */
	RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_2    = RTC_MODE0_CTRLB_ACTF_DIV2,
	/** RTC active layer frequency is prescaled by a factor of 4. */
	RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_4    = RTC_MODE0_CTRLB_ACTF_DIV4,
	/** RTC active layer frequency is prescaled by a factor of 8. */
	RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_8    = RTC_MODE0_CTRLB_ACTF_DIV8,
	/** RTC active layer frequency is prescaled by a factor of 16. */
	RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_16   = RTC_MODE0_CTRLB_ACTF_DIV16,
	/** RTC active layer frequency is prescaled by a factor of 32. */
	RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_32   = RTC_MODE0_CTRLB_ACTF_DIV32,
	/** RTC active layer frequency is prescaled by a factor of 64. */
	RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_64   = RTC_MODE0_CTRLB_ACTF_DIV64,
	/** RTC active layer frequency is prescaled by a factor of 128. */
	RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_128  = RTC_MODE0_CTRLB_ACTF_DIV128,
	/** RTC active layer frequency is prescaled by a factor of 256. */
	RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_256  = RTC_MODE0_CTRLB_ACTF_DIV256,
};

/**
 * \brief RTC tamper debounce frequency.
 *
 * The available prescaler factor for the input debouncers.
 */
enum rtc_count_tamper_debounce_freq {
	/** RTC debounce frequency is prescaled by a factor of 2. */
	RTC_TAMPER_DEBOUNCE_FREQ_DIV_2    = RTC_MODE0_CTRLB_DEBF_DIV2,
	/** RTC debounce frequency is prescaled by a factor of 4. */
	RTC_TAMPER_DEBOUNCE_FREQ_DIV_4    = RTC_MODE0_CTRLB_DEBF_DIV4,
	/** RTC debounce frequency is prescaled by a factor of 8. */
	RTC_TAMPER_DEBOUNCE_FREQ_DIV_8    = RTC_MODE0_CTRLB_DEBF_DIV8,
	/** RTC debounce frequency is prescaled by a factor of 16. */
	RTC_TAMPER_DEBOUNCE_FREQ_DIV_16   = RTC_MODE0_CTRLB_DEBF_DIV16,
	/** RTC debounce frequency is prescaled by a factor of 32. */
	RTC_TAMPER_DEBOUNCE_FREQ_DIV_32   = RTC_MODE0_CTRLB_DEBF_DIV32,
	/** RTC debounce frequency is prescaled by a factor of 64. */
	RTC_TAMPER_DEBOUNCE_FREQ_DIV_64   = RTC_MODE0_CTRLB_DEBF_DIV64,
	/** RTC debounce frequency is prescaled by a factor of 128. */
	RTC_TAMPER_DEBOUNCE_FREQ_DIV_128  = RTC_MODE0_CTRLB_DEBF_DIV128,
	/** RTC debounce frequency is prescaled by a factor of 256. */
	RTC_TAMPER_DEBOUNCE_FREQ_DIV_256  = RTC_MODE0_CTRLB_DEBF_DIV256,
};

/**
 * \brief RTC tamper input action.
 *
 * The available action taken by the tamper input.
 */
enum rtc_count_tamper_input_action {
	/** RTC tamper input action is disabled. */
	RTC_TAMPER_INPUT_ACTION_OFF     = RTC_TAMPCTRL_IN4ACT_OFF,
	/** RTC tamper input action is wake and set tamper flag. */
	RTC_TAMPER_INPUT_ACTION_WAKE    = RTC_TAMPCTRL_IN4ACT_WAKE,
	/** RTC tamper input action is capture timestamp and set tamper flag. */
	RTC_TAMPER_INPUT_ACTION_CAPTURE = RTC_TAMPCTRL_IN4ACT_CAPTURE,
	/** RTC tamper input action is compare IN to OUT, when a mismatch occurs,
	 *  capture timestamp and set tamper flag. */
	RTC_TAMPER_INPUT_ACTION_ACTL    = RTC_TAMPCTRL_IN4ACT_ACTL,
};

/**
 * \brief RTC tamper level select.
 *
 * The available edge condition for tamper level select.
 */
enum rtc_count_tamper_level_sel {
	/** A falling edge condition will be detected on Tamper input. */
	RTC_TAMPER_LEVEL_FALLING = RTC_TAMPCTRL_TAMLVL(0),
	/** A rising edge condition will be detected on Tamper input. */
	RTC_TAMPER_LEVEL_RISING  = RTC_TAMPCTRL_TAMLVL(1),
};

/**
 * \brief RTC tamper debounce sequential.
 *
 * The available sequential for tamper debounce.
 */
enum rtc_count_tamper_debounce_seq {
	/** Tamper input detect edge with synchronous stability debounce. */
	RTC_TAMPER_DEBOUNCE_SYNC,
	/** Tamper input detect edge with asynchronous stability debounce. */
	RTC_TAMPER_DEBOUNCE_ASYNC,
	/** Tamper input detect edge with majority debounce. */
	RTC_TAMPER_DEBOUNCE_MAJORITY,
};

/**
 * \brief RTC tamper input configuration structure.
 *
 * The configuration structure for tamper INn.
 */
struct rtc_count_tamper_input_config {
	/** Debounce enable. */
	bool debounce_enable;
	/** Tamper level select. */
	enum rtc_count_tamper_level_sel level;
	/** Tamper input action. */
	enum rtc_count_tamper_input_action action;
};

/**
 * \brief RTC Tamper configuration structure.
 *
 * The configuration structure for the RTC tamper. This structure should
 * be initialized using the \ref rtc_tamper_get_config_defaults() before any
 * user configurations are set.
 */
struct rtc_tamper_config {
	/** Backup register reset on tamper enable. */
	bool bkup_reset_on_tamper;
	/** GP register reset on tamper enable. */
	bool gp_reset_on_tamper;
	/** Active layer frequency. */
	enum rtc_count_tamper_active_layer_freq actl_freq;
	/** Debounce frequency. */
	enum rtc_count_tamper_debounce_freq deb_freq;
	/** Debounce sequential. */
	enum rtc_count_tamper_debounce_seq deb_seq;
	/** DMA on tamper enable. */
	bool dma_tamper_enable;
	/** General Purpose 0/1 Enable. */
	bool gp0_enable;
	/** Tamper IN configuration. */
	struct rtc_count_tamper_input_config in_cfg[5];
};

/**
 * \name RTC Tamper Detection
 * @{
 */

/**
 *  \brief Gets the RTC tamper default configurations.
 *
 *  Initializes the configuration structure to default values. 
 *
 *  The default configuration is as follows:
 *  - Disable backup register reset on tamper
 *  - Disable GP register reset on tamper
 *  - Active layer clock divided by a factor of 8
 *  - Debounce clock divided by a factor of 8
 *  - Detect edge on INn with synchronous stability debouncing
 *  - Disable DMA on  tamper
 *  - Enable GP register
 *  - Disable debouce, detect on falling edge and no action on INn
 *
 *  \param[out] config Configuration structure to be initialized to default values.
 */
static inline void rtc_tamper_get_config_defaults(
		struct rtc_tamper_config *const config)
{
	/* Sanity check argument */
	Assert(config);

	config->bkup_reset_on_tamper= false;
	config->gp_reset_on_tamper  = false;
	config->actl_freq           = RTC_TAMPER_ACTIVE_LAYER_FREQ_DIV_8;
	config->deb_freq            = RTC_TAMPER_DEBOUNCE_FREQ_DIV_8;
	config->deb_seq             = RTC_TAMPER_DEBOUNCE_SYNC;
	config->dma_tamper_enable   = false;
	config->gp0_enable          = true;

	struct rtc_count_tamper_input_config in_default_cfg[] = {
		{false, RTC_TAMPER_LEVEL_FALLING, RTC_TAMPER_INPUT_ACTION_OFF},
		{false, RTC_TAMPER_LEVEL_FALLING, RTC_TAMPER_INPUT_ACTION_OFF},
		{false, RTC_TAMPER_LEVEL_FALLING, RTC_TAMPER_INPUT_ACTION_OFF},
		{false, RTC_TAMPER_LEVEL_FALLING, RTC_TAMPER_INPUT_ACTION_OFF},
		{false, RTC_TAMPER_LEVEL_FALLING, RTC_TAMPER_INPUT_ACTION_OFF},
	};
	memcpy(config->in_cfg, in_default_cfg, RTC_TAMPER_NUM * sizeof(struct rtc_count_tamper_input_config));
}
enum status_code rtc_tamper_set_config (struct rtc_module *const module,
		struct rtc_tamper_config *const tamper_cfg);

/**
 * \brief Retrieves the RTC tamper detection status.
 *
 * Retrieves the detection status of each input pin and the input event.
 *
 * \param[in] module  Pointer to the RTC software instance struct
 *
 * \return Bitmask of detection flags.
 *
 * \retval RTC_TAMPER_DETECT_ID0   Tamper condition on IN0 has been detected
 * \retval RTC_TAMPER_DETECT_ID1   Tamper condition on IN1 has been detected
 * \retval RTC_TAMPER_DETECT_ID2   Tamper condition on IN2 has been detected
 * \retval RTC_TAMPER_DETECT_ID3   Tamper condition on IN3 has been detected
 * \retval RTC_TAMPER_DETECT_ID4   Tamper condition on IN4 has been detected
 * \retval RTC_TAMPER_DETECT_EVT   Tamper input event has been detected
 */
static inline uint32_t rtc_tamper_get_detect_flag (struct rtc_module *const module)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	uint32_t tamper_id = module->hw->MODE0.TAMPID.reg;
	uint32_t detect_flags = 0;

	if (tamper_id & RTC_TAMPID_TAMPID0) {
		detect_flags |= RTC_TAMPER_DETECT_ID0;
	}

	if (tamper_id & RTC_TAMPID_TAMPID1) {
		detect_flags |= RTC_TAMPER_DETECT_ID1;
	}

	if (tamper_id & RTC_TAMPID_TAMPID2) {
		detect_flags |= RTC_TAMPER_DETECT_ID2;
	}

	if (tamper_id & RTC_TAMPID_TAMPID3) {
		detect_flags |= RTC_TAMPER_DETECT_ID3;
	}

	if (tamper_id & RTC_TAMPID_TAMPID4) {
		detect_flags |= RTC_TAMPER_DETECT_ID4;
	}

	if (tamper_id & RTC_TAMPID_TAMPEVT) {
		detect_flags |= RTC_TAMPER_DETECT_EVT;
	}

	return detect_flags;
}

/**
 * \brief Clears RTC tamper detection flag.
 *
 * Clears the given detection flag of the module.
 *
 * \param[in] module   Pointer to the TC software instance struct
 * \param[in] detect_flags  Bitmask of detection flags
 */
static inline void rtc_tamper_clear_detect_flag(
		struct rtc_module *const module,
		const uint32_t detect_flags)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	uint32_t tamper_id = 0;

	if (detect_flags & RTC_TAMPER_DETECT_ID0) {
		tamper_id |= RTC_TAMPID_TAMPID0;
	}

	if (detect_flags & RTC_TAMPER_DETECT_ID1) {
		tamper_id |= RTC_TAMPID_TAMPID1;
	}

	if (detect_flags & RTC_TAMPER_DETECT_ID2) {
		tamper_id |= RTC_TAMPID_TAMPID2;
	}

	if (detect_flags & RTC_TAMPER_DETECT_ID3) {
		tamper_id |= RTC_TAMPID_TAMPID3;
	}

	if (detect_flags & RTC_TAMPER_DETECT_ID4) {
		tamper_id |= RTC_TAMPID_TAMPID4;
	}

	if (detect_flags & RTC_TAMPER_DETECT_EVT) {
		tamper_id |= RTC_TAMPID_TAMPEVT;
	}

	module->hw->MODE0.TAMPID.reg = tamper_id;

}

/** @} */

#endif

#endif /* RTC_TAMPER_H_INCLUDED */