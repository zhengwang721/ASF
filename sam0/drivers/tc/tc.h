/**
 * \file
 *
 * \brief SAM0+ TC Driver
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


#include "status_codes.h" //may need to add path info but the name is correct
#include "tc_header.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef TC_H_INCLUDED
#define TC_H_INCLUDED

//TODO look into ccn->channel_n or cc_channel_n
enum tc_compare_capture_channel_index {
	TC_COMPARE_CAPTURE_CHANNEL_0                    = 0,
	TC_COMPARE_CAPTURE_CHANNEL_1                    = 1,
	TC_COMPARE_CAPTURE_CHANNEL_2                    = 2,
	TC_COMPARE_CAPTURE_CHANNEL_3                    = 3,
};

//TODO: Name does not explain so much perhaps tc_counter_reload_options or restart_options
// could use tc_reload_action
//TODO: send mail to ic dev. may remove compleetly
enum tc_reload_action {
	/* The counter is reloaded/reset on the next GCLK and starts
	 * counting on the prescaler clock.
	 */
	TC_RELOAD_ACTION_GCLK      = TC_PRESCSYNC_GCLK_bm,

	/* The counter is reloaded/reset on the next prescaler clock */
	TC_RELOAD_ACTION_PRESC          = TC_PRESCSYNC_PRESC_bm,

	/* The counter is reloaded/reset on the next GCLK, and the prescaler is
	 * reset as well. This may be usefull when an event retriggers the clock
	 */
	TC_RELOAD_ACTION_RESYNC         = TC_PRESCSYNC_RESYNC_bm,
};

enum tc_clock_prescaler {
	TC_CLOCK_PRESCALER_DIV1         = TC_PRESCALER_DIV1_bm,
	TC_CLOCK_PRESCALER_DIV2         = TC_PRESCALER_DIV2_bm,
	TC_CLOCK_PRESCALER_DIV4         = TC_PRESCALER_DIV4_bm,
	TC_CLOCK_PRESCALER_DIV8         = TC_PRESCALER_DIV8_bm,
	TC_CLOCK_PRESCALER_DIV16        = TC_PRESCALER_DIV16_bm,
	TC_CLOCK_PRESCALER_DIV64        = TC_PRESCALER_DIV64_bm,
	TC_CLOCK_PRESCALER_DIV256       = TC_PRESCALER_DIV256_bm,
	TC_CLOCK_PRESCALER_DIV1024      = TC_PRESCALER_DIV1024_bm,
};

enum tc_wave_generation {
	/** TOP is MAX, except in 8 bit mode where it is the PER register*/
	TC_WAVE_GENERATION_NORMAL_FREQ         = TC_WAVEGEN_NFRQ_bm,
	/** TOP is CC0, expert in 8 bit mode where it is the PER register*/
	TC_WAVE_GENERATION_MATCH_FREQ          = TC_WAVEGEN_MFRQ_bm,
	/** TOP is MAX, except in 8 bit mode where it is the PER register*/
	TC_WAVE_GENERATION_NORMAL_PWM          = TC_WAVEGEN_NPWM_bm,
	/** TOP is CC0, except in 8 bit mode where it is the PER register*/
	TC_WAVE_GENERATION_MATCH_PWM           = TC_WAVEGEN_MPWM_bm,
};

/** Specifies which direction for the TC module to count */
enum tc_count_direction {
	TC_COUNT_UP        = UP,
	TC_COUNT_DOWN      = DOWN,
};

//TODO: Use #define?
enum tc_capture_enable {
	TC_CAPTURE_ENABLE_NONE                 = 0,
	TC_CAPTURE_ENABLE_CC_CHANNEL_0            = TC_CPTEN_CC0_bm,
	TC_CAPTURE_ENABLE_CC_CHANNEL_1            = TC_CPTEN_CC1_bm,
	TC_CAPTURE_ENABLE_CC_CHANNEL_2            = TC_CPTEN_CC2_bm,
	TC_CAPTURE_ENABLE_CC_CHANNEL_3            = TC_CPTEN_CC3_bm,
	TC_CAPTURE_ENABLE_ALL                  = (0xf << TC_CPTEN_CC0_bp),
};

enum tc_event_action {
	TC_EVENT_ACTION_OFF              = TC_EVACT_OFF_bm,
	TC_EVENT_ACTION_RETRIGGER        = TC_EVACT_RETRIGGER_bm,
	TC_EVENT_ACTION_COUNT            = TC_EVACT_COUNT_bm,
	TC_EVENT_ACTION_FREQ_PULSE_WIDTH = TC_EVACT_PPW_bm,
	TC_EVENT_ACTION_PULSE_WIDTH_FREQ = TC_EVACT_PWP_bm,
};

//TODO use #define?
enum tc_waveform_invert_ch {
	TC_WAVEFORM_INVERT_NONE                            = 0,
	TC_WAVEFORM_INVERT_COMPARE_CAPTURE_CHANNEL_0       = TC_INVEN_CC0_bm,
	TC_WAVEFORM_INVERT_COMPARE_CAPTURE_CHANNEL_1       = TC_INVEN_CC1_bm,
	TC_WAVEFORM_INVERT_COMPARE_CAPTURE_CHANNEL_2       = TC_INVEN_CC2_bm,
	TC_WAVEFORM_INVERT_COMPARE_CAPTURE_CHANNEL_3       = TC_INVEN_CC3_bm,
};

/** Enable generation of events from the TC module
 *
 *  The content of this enum is to be used by the event_generation_mask in the
 *  tc_config struct
 */
//TODO: Use #define?
enum tc_event_generation_enable {
	/** No generation of events */
	TC_EVENT_GENERATION_NONE             = TC_EVGEN_NONE_gc,
	/** Generate event for compare/capture on channel 0 */
	TC_EVENT_GENERATION_CC0              = TC_EVGEN_CC0_gc,
	/** Generate event for compare/capture on channel 1 */
	TC_EVENT_GENERATION_CC1              = TC_EVGEN_CC1_gc,
	/** Generate event for compare/capture on channel 2 */
	TC_EVENT_GENERATION_CC2              = TC_EVGEN_CC2_gc,
	/** Generate event for compare/capture on channel 3 */
	TC_EVENT_GENERATION_CC3              = TC_EVGEN_CC3_gc,
	/** Generate event for overflow/underflow */
	TC_EVENT_GENERATION_OVF              = TC_EVGEN_OVF_gc,
};


enum tc_resolution {
	TC_RESOLUTION_8BIT        = TC_MODE_COUNT8_bm,
	TC_RESOLUTION_16BIT       = TC_MODE_COUNT16_bm,
	TC_RESOLUTION_32BIT       = TC_MODE_COUNT32_bm,
};

//TODO: Use only 32bits and do cast
struct tc_8bit_config {
	uint8_t count;
	uint8_t period;
	uint8_t capture_compare_channel_0;
	uint8_t capture_compare_channel_1;
	uint8_t capture_compare_channel_2;
	uint8_t capture_compare_channel_3;
};

struct tc_16_bit_config {
	uint16_t count;
	uint16_t period;
	uint16_t capture_compare_channel_1;
	uint16_t capture_compare_channel_2;
	uint16_t capture_compare_channel_3;
};

struct tc_32bit_config {
	uint32_t count;
	uint32_t period;
	uint32_t capture_compare_channel_1;
};

struct tc_config {
	//TODO set clock param
	/** Specifies either 8, 16 or 32 bit counter mode  */
	enum tc_resolution resolution;
	/** Specifies the prescaler value for GCLK_TC */
	enum tc_clock_prescaler prescaler;


	/** Specifies which waveform to be generated on the output */
	enum tc_wave_generation wave_generation;
	/** Specifies which channel(s) to invert the waveform on */
	uint16_t waveform_invert_ch_mask;

	/** Specifies the reload or reset time of the counter and prescaler
	 *  resynchronization on a retrigger event for timer/counter */
	enum tc_reload_action reload_action;
	/** Oneshot enabled will stop the TC on next HW/SW retrigger event
	 *  or overflow/underflow */
	bool oneshot;
	/** Specifies the direction for the TC to count */
	enum tc_count_direction count_direction;
	/** Specifies which channel (s) to enable channel capture operation
	 *  on. Since all capture channels use the same event line,
	 *  only one capture channel must be enabled at a time,
	 *  when used for event capture. When pwm capture is used this
	 *  is not the case as it does not use the event line.
	 */
	uint16_t capture_enable_ch_mask;
	/** Specifies which event to trigger if an event is triggered */
	enum tc_event_action event_action;
	/** Specifies if the input event source is inverted, when used in PWP,
	 *  PPW event action */
	bool invert_event_input;
	/**Must be set to true to enable event actions */
	bool enable_event_input;
	/** Specifies which channel(s) to generate event on when a
	 *  compare/capture occurs */
	uint16_t event_generation_mask;

	union {
		struct tc_8bit_config tc_8bit_config;
		struct tc_16bit_config tc_16bit_config;
		struct tc_32bit_config tc_32bit_config;
	};
};

struct tc_dev_inst {
	TC_t *hw_dev;
	enum tc_resolution resolution;
};


/**
 * \brief Synchronization between clock domains
 *
 * Makes sure GCLK_TC is synchronized with sysclock,
 * Must be called before assessing certain registers in the TC.
 * Blocks while waiting
 *
 * \param hw_dev pointer to module
 */
static inline void _tc_sync(TC_t  *const hw_dev)
{
	/* Sanity check arguments  */
	Assert(hw_dev);

	/* Synchronize  */
	while (hw_dev->STATUS & TC_SYNCBUSY_bm){
		/* Wait for sync */
	}
}

/**
 * \brief Initializes config with predefined default values
 *
 * The config struct is loaded with predefined values to allow quick startup.
 * The predefined values sets up all four channels separately, with no filtering
 * and low power operation.
 *
 * \param config pointer to the config struct
 */
static inline enum status_code tc_get_config_defaults(
		struct tc_config *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Write default config to config struct */
	config->resolution                                  = TC_16BIT_RESOLUTION;
	config->prescaler                                   = TC_PRESCALER_DIV1;
	config->reload_action                               = TC_RELOAD_ACTION_GCLK;
	config->wave_generation                             = TC_WAVE_GENERATION_NORMAL_FREQ;
	config->waveform_invert_ch_mask                     = TC_WAVEFORM_INVERT_NONE;
	config->count_direction                             = TC_COUNT_UP;
	config->oneshot                                     = false;
	config->capture_enable_ch_mask                      = TC_CAPTURE_ENABLE_NONE;
	config->event_action                                = TC_EVENT_ACTION_OFF;
	config->invert_event_input                          = false;
	config->event_generation_mask                       = TC_EVENT_GENERATION_NONE;
	config->tc_16bit_config.count                       = 0x00;
	config->tc_16bit_config.period                      = 0xFF;
	config->tc_16bit_config.capture_compare_channel_0   = 0x00;
	config->tc_16bit_config.capture_compare_channel_1   = 0x00;
	config->tc_16bit_config.capture_compare_channel_2   = 0x00;
	config->tc_16bit_config.capture_compare_channel_3   = 0x00;

	return STATUS_OK;
}

/**
 * \brief Initializes the TC
 *
 * Enables the clock and initializes the TC module,
 * based on the values of the config struct
 *
 * \param dev_inst    pointer to the device struct
 * \param config pointer to the config struct
 * \return enum status_code STATUS_OK,
 */
enum status_code tc_init(TC_t *const module,
		struct tc_dev_inst *const dev_inst,
		struct tc_config *const config);

/**
 * \brief Reset the TC module
 *
 * Resets the TC module
 *
 * \param dev_inst    pointer to the device struct
 */
static inline void tc_reset(struct tc_dev_inst *dev_inst)
{

	/* Sanity check arguments  */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module hardware instance */
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_sync(tc_module);

	/* Enable TC module, based on the module tc mode  */
	tc_module->CTRLA = TC_RESET_bm;
}

//removed tc_set_config();

/**
 * \brief Enable the TC module
 *
 * Enables the TC module.
 *
 * \param dev_inst    pointer to the device struct
 */
//TODO: may be necessary to wait until the enable bit reads as one
//before exiting the function. (see section 1.7.3 in the prelim tc doc).
static inline void tc_enable(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_sync(tc_module);

	/* Enable TC module */
	tc_module->CTRLA |= TC_ENABLE_bm;
}

/**
 * \brief Disables the TC module
 *
 * Disables the TC module.
 *
 * \param dev_inst    pointer to the device struct
 */
//TODO: may be necessary to wait here until the enable bit reads as zero
//before exiting the function. (see section 1.7.4 in the prelim tc doc).
static inline void tc_disable(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_sync(tc_module);

	/* Disable TC module */
	tc_module->CTRLA  &= ~TC_ENABLE_bm;
}


/**
 * \brief Gets the count value
 *
 * Gets the count value of the TC module
 *
 * \param dev_inst    pointer to the device struct
 * \return enum status_code STATUS_OK, STATUS_ERR_INVALID_ARG
 */
enum status_code tc_get_count(struct tc_dev_inst *const dev_inst,
		uint32_t *count);

/**
 * \brief Sets the count value
 *
 * Sets the count value of the TC module
 *
 * \param dev_inst    pointer to the device struct
 * \return enum status_code STATUS_OK, STATUS_ERR_INVALID_ARG
 */
enum status_code tc_set_count(struct tc_dev_inst *const dev_inst,
		uint32_t count);

/**
 * \brief Checks if the TC module is enabled
 *
 * Checks if the TC module is enabled and returns a true/false value.
 * Blocks if it needs to wait for SYNCBUSY.
 * \param dev_inst    pointer to the device struct
 * \return bool       returns true is the module is enabled
 */
//not certain that there are many use cases where this is needed.
static inline bool tc_is_enabled(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_sync(tc_module);

	/* Check if the TC module is enabled */
	return tc_module->CTRLA & TC_ENABLE_bm;
}

static inline enum status_code tc_start_counter(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_sync(tc_module);

	/* Write command to execute */
	tc_module->CTRLBSET |= TC_COMMAND_START_bm;
}


static inline enum status_code tc_stop_counter(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_sync(tc_module);

	/* Write command to execute */
	tc_module->CTRLBSET |= TC_COMMAND_STOP_bm;
}


/**
 * \brief Set the count direction of the TC module
 *
 * Set the count direction of the TC module, either incrementing
 * or decrementing
 *
 * \param dev_inst    pointer to the device struct
 * \param count_dir   direction for the TC to count
 * \returns  STATUS_OK
 */
static inline enum status_code tc_set_count_direction(
	struct tc_dev_inst *const dev_inst,
	enum tc_count_direction count_direction)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	tc_sync(tc_module);

	/* Set count direction */
	switch(count_direction) {
		case TC_COUNT_UP:
			tc_module->CTRLBSET = TC_COUNT_DIRECTION_bp;

		case TC_COUNT_DOWN:
			tc_module->CTRLBCLR = TC_COUNT_DIRECTION_bp;
		default:
			Assert(false);
	}
}
/**
 * \brief Gets the compare value
 *
 * Reads the capture value stored in a given compare/capture register into a
 * buffer.
 *
 * \param dev_inst    pointer to the device struct
 * \param buffer      pointer to a buffer
 * \comp_reg_index    index of the compare register to read from
 * \return enum status_code STATUS_OK, STATUS_ERR_INVALID_ARG
 */
//Have to check and clear interrupt flags to
//see if new information is available or check for overflow err.
//can either do this in this function and return nothing if no updates
//have occurred or can use one or more functions to check these flags before
//the user uses this function.
enum status_code tc_get_capture(
		struct tc_dev_inst *dev_inst,
		uint32_t *capture_value,
		enum tc_ccn_reg_index comp_reg_index);

/**
 * \brief Sets a compare value
 *
 * Writes a compare value from a buffer to a given compare/capture register
 *
 * \param dev_inst    pointer to the device struct
 * \param buffer      pointer to a buffer
 * \comp_reg_index    index of the compare register to write to
 * \return enum status_code STATUS_OK, STATUS_ERR_INVALID_ARG
 */
enum status_code tc_set_compare(
		struct tc_dev_inst *dev_inst,
		uint32_t compare_value,
		enum tc_ccn_reg_index comp_reg_index);


/**
 * \brief Sets the top value
 *
 * Writes the top value for counter associated with the dev_inst struct
 *
 * \param dev_inst    pointer to the device struct
 * \param top_value   value to be used as top in the counter
 * \return enum status_code STATUS_OK, STATUS_ERR_INVALID_ARG
 */

/*
 * This should make it easy to set top value as this is
 * different between 8 and the 16 and 32 bit modes.
 * This hides that difference away from the user.
 * He must be made aware that cc0 can not be used for pulse with though.
 * Not necessarily right to have all this in the .h file.
 */

static enum status_code tc_set_top(struct tc_dev_inst *dev_inst,
				uint32_t top_value)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(compare);

	TC_t * const tc_module = dev_inst->hw_dev;

	tc_sync(tc_module);

	switch(dev_inst->resolution) {
	case TC_RESOLUTION_8BIT:
		tc_module->TC_COUNT8.PER = (uint8_t) top_value;
		return STATUS_OK;
	case TC_RESOLUTION_16BIT:
		tc_module->TC_COUNT16.capture_compare_channel_0 = (uint16_t) top_value;
		return STATUS_OK;
	case TC_RESOLUTION_32BIT:
		tc_module->TC_COUNT32.capture_compare_channel_0 = top_value;
		return STATUS_OK;
	default:
		Assert(false);//not certain of this line. Will the return statement happen?
		return STATUS_ERR_INVALID_ARG;
	}
}

// The period will either be stored in cc0 or cc1 this is
//also the case for period. What is used depends on PWP
//and/or PPW settings. should be more economical in therms
//of cycles compared to two tc_get_captures.
//Have to setup interrupts and then check and clear interrupt flags to
//see if new information is available or check for overflow err.
//could also write a function to check these flags for you
enum status_code tc_get_pwm_capture(struct tc_dev_inst *dev_inst,
				 uint32_t *period,
				 uint32_t *pulse_width,
				 uint8_t PWP_or_PPW);

#endif /* TC_ALL_HEADER */
