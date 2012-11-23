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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TC_H_INCLUDED
#define TC_H_INCLUDED

#include "status_codes.h"
#include "tc_header.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * \defgroup sam0_TC_group SAM0+ Timer Counter Driver (TC)
 *
 * Driver for the SAM0+ architecture devices. This driver provides an
 * interface for configuration and management of the TC module. This
 * driver encompasses the following module within the SAM0+ devices:
 *
 * \li \b TC \b (Timer Counter)
 *
 * \subsection module_overview TC Overview
 *
 * \subsection sleep_modes Operation in Sleep Modes
 *
 * \subsection clock_generation Clock Generation
 *
 * \section dependencies Dependencies
 * The TC driver has the following dependencies:
 * \li \b PORT
 *
 * \section special_cons Special Considerations
 *
 * \section extra_info Extra Information
 * For extra information see \ref spi_extra_info.
 *
 * \section module_examples Examples
 * - \ref quickstart
 *
 * \section api_overview API Overview
 * @{
 */

//following 12 lines can be removed if defines are done in tc_header.h
#ifndef TC_INTFLAG_ERR_bm
#define TC_INTFLAG_ERR_bm (0x01 << 1)
#endif
#ifndef TC_EVCTRL_TCEI_bm
#define TC_EVCTRL_TCEI_bm (0x0001 << 5)
#endif
#ifndef TC_EVCTRL_TCINV_bm
#define TC_EVCTRL_TCINV_bm (0x0001 << 4)
#endif
#ifndef TC_CTRLA_SLEEPEN_bp
#define TC_CTRLA_SLEEPEN_bp 11
#endif



/**
 * \brief Index of the compare capture channels
 *
 * These values are used in certain functions to specify what
 * capture/compare channel to do operations on.
 */
enum tc_compare_capture_channel_index {
	/** Index of compare capture channel 0 */
	TC_COMPARE_CAPTURE_CHANNEL_0,
	/** Index of compare capture channel 1 */
	TC_COMPARE_CAPTURE_CHANNEL_1,
	/** Index of compare capture channel 2 */
	TC_COMPARE_CAPTURE_CHANNEL_2,
	/** Index of compare capture channel 3 */
	TC_COMPARE_CAPTURE_CHANNEL_3,
};


//TODO: document better may have to make this part of the whole
//documentation, and then refer to that here
/**
 * \brief TC Counter reload action enum
 *
 * These enums specify how the clock should reload.
 */
enum tc_reload_action {
	/**
	 * The counter is reloaded/reset on the next GCLK and starts
	 * counting on the prescaler clock.
	 */
	TC_RELOAD_ACTION_GCLK                 = TC_PRESCSYNC_GCLK_bm,
	/**
	 * The counter is reloaded/reset on the next prescaler clock
	 */
	TC_RELOAD_ACTION_PRESC                = TC_PRESCSYNC_PRESC_bm,
	/**
	 * The counter is reloaded/reset on the next GCLK, and the
	 * prescaler is reset as well. This may be useful when an
	 * event retrigger the clock
	 */
	TC_RELOAD_ACTION_RESYNC               = TC_PRESCSYNC_RESYNC_bm,
};


/**
 * \brief TC clock prescaler values
 *
 * These values are used to chose the clock prescaler
 * configuration. The prescaler divides the clock frequency of the tc
 * module to make the counter count slower.
 */
enum tc_clock_prescaler {
	/** Divide clock by 1 */
	TC_CLOCK_PRESCALER_DIV1               = TC_PRESCALER_DIV1_bm,
	/** Divide clock by 2 */
	TC_CLOCK_PRESCALER_DIV2               = TC_PRESCALER_DIV2_bm,
	/** Divide clock by 4 */
	TC_CLOCK_PRESCALER_DIV4               = TC_PRESCALER_DIV4_bm,
	/** Divide clock by 8 */
	TC_CLOCK_PRESCALER_DIV8               = TC_PRESCALER_DIV8_bm,
	/** Divide clock by 16 */
	TC_CLOCK_PRESCALER_DIV16              = TC_PRESCALER_DIV16_bm,
	/** Divide clock by 64 */
	TC_CLOCK_PRESCALER_DIV64              = TC_PRESCALER_DIV64_bm,
	/** Divide clock by 256 */
	TC_CLOCK_PRESCALER_DIV256             = TC_PRESCALER_DIV256_bm,
	/** Divide clock by 1024 */
	TC_CLOCK_PRESCALER_DIV1024            = TC_PRESCALER_DIV1024_bm,
};


/**
 * \brief TC wave generation mode enum
 *
 * These values are used to select what mode to run the wave
 * generation in. You can chose to use frequency generation or pulse
 * width modulation (PWM) In these modes you can either run them in
 * normal mode or in match mode.  In normal mode the TOP value is set
 * to the maximum allowable value, depending on what resolution is
 * used, MAX is either 0xFF, 0xFFFF or 0xFFFFFFFF.  In normal mode you
 * don't use any registers to store these values.
 *
 * In match mode you can configure what the top value should be and
 * thus you get a range between the minimum value which is between 3
 * up to the MAX value.  This mode does however limit the number of
 * compare capture channels available, as one is used to store the TOP
 * value.
 */
enum tc_wave_generation {
	/**
	 * TOP is MAX, except in 8 bit mode where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_NORMAL_FREQ         = TC_WAVEGEN_NFRQ_bm,
	/**
	 * TOP is CC0, expert in 8 bit mode where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_MATCH_FREQ          = TC_WAVEGEN_MFRQ_bm,
	/**
	 * TOP is MAX, except in 8 bit mode where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_NORMAL_PWM          = TC_WAVEGEN_NPWM_bm,
	/**
	 * TOP is CC0, except in 8 bit mode where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_MATCH_PWM           = TC_WAVEGEN_MPWM_bm,
};


/**
 * \brief Specifies which direction the TC module is to count
 *
 * The counter can be set to either count up or down these values are
 * used to control this.
 */
enum tc_count_direction {
	/** Makes the counter count up from zero */
	TC_COUNT_DIRECTION_UP,
	/** Makes the counter cunt down from MAX or a specified value */
	TC_COUNT_DIRECTION_DOWN,
};


/**
 * \brief TC channel capture enable enum
 */
enum tc_capture_enable {
	/** Enables no channels for capture */
	TC_CAPTURE_ENABLE_NONE                 = 0,
	/** Enables channel 0 for capture */
	TC_CAPTURE_ENABLE_CHANNEL_0            = TC_CPTEN_CC0_bm,
	/** Enables channel 1 for capture */
	TC_CAPTURE_ENABLE_CHANNEL_1            = TC_CPTEN_CC1_bm,
	/** Enables channel 2 for capture when channel is available */
	TC_CAPTURE_ENABLE_CHANNEL_2            = TC_CPTEN_CC2_bm,
	/** Enables channel 3 for capture when channel i available */
	TC_CAPTURE_ENABLE_CHANNEL_3            = TC_CPTEN_CC3_bm,
};


/**
 * \brief TC event action enum
 *
 * Specifies what event to take when the even channel is active
 */
enum tc_event_action {
	/** No event is performed */
	TC_EVENT_ACTION_OFF                    = TC_EVACT_OFF_bm,
	/** The counter starts at the initial value */
	TC_EVENT_ACTION_RETRIGGER              = TC_EVACT_RETRIGGER_bm,
	/** The counter is incremented */
	TC_EVENT_ACTION_COUNT                  = TC_EVACT_COUNT_bm,
	/**  */
	TC_EVENT_ACTION_FREQ_PULSE_WIDTH       = TC_EVACT_PPW_bm,  
//TODO: Need to find out if there are more differences between this and
//PWP
	/**  */
	TC_EVENT_ACTION_PULSE_WIDTH_FREQ       = TC_EVACT_PWP_bm,
};


/**
 * \brief TC waveform invert on channel enum
 */
enum tc_waveform_invert_output {
	/**  */
	TC_WAVEFORM_INVERT_OUTPUT_NONE         = 0,
	/**  */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_0    = TC_INVEN_CC0_bm,
	/**  */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_1    = TC_INVEN_CC1_bm,
	/**  */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_2    = TC_INVEN_CC2_bm,
	/**  */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_3    = TC_INVEN_CC3_bm,
};


/**
 * \brief Enable generation of events from the TC module
 *
 *  The content of this enum is to be used by the
 *  event_generation_mask in the tc_conf struct
 */
enum tc_event_generation_enable {
	/** No generation of events */
	TC_EVENT_GENERATION_ENABLE_NONE       = TC_EVGEN_NONE_gc,
	/** Generate event for compare/capture on channel 0 */
	TC_EVENT_GENERATION_ENABLE_CHANNEL_0  = TC_EVGEN_CC0_gc,
	/** Generate event for compare/capture on channel 1 */
	TC_EVENT_GENERATION_ENABLE_CHANNEL_1  = TC_EVGEN_CC1_gc,
	/** Generate event for compare/capture on channel 2 */
	TC_EVENT_GENERATION_ENABLE_CHANNEL_2  = TC_EVGEN_CC2_gc,
	/** Generate event for compare/capture on channel 3 */
	TC_EVENT_GENERATION_ENABLE_CHANNEL_3  = TC_EVGEN_CC3_gc,
	/** Generate event for overflow/underflow */
	TC_EVENT_GENERATION_ENABLE_OVF        = TC_EVGEN_OVF_gc,
};


/**
 * \brief Specifies if the counter is 8, 16 or 32 bits.
 *
 * This specifies the maximum value you can count to. What resolution
 * you use will impose limitations in other areas.  The 32 bit counter
 * uses two 16 bit counters in cascade to realize the 32 bit
 * counter. When using 16 and 32 bit resolution you no longer get a
 * dedicated period register. In cases when you van t to change the
 * top value you will have to use one of the registers used in the
 * capture compare channels. Thus you get fewer capture compare
 * channels.
 */
enum tc_resolution {
	/**
	 * The counters max value is 0xFF, the period register is
	 * available to be used as TOP value
	 */
	TC_RESOLUTION_8BIT                    = TC_MODE_COUNT8_bm,
	/**
	 * The counters MAX value is 0xFFFF. There is no separate
	 * period register, to modify top one of the capture compare
	 * registers has to be used. This limits the amount of
	 * available channels.
	 */
	TC_RESOLUTION_16BIT                   = TC_MODE_COUNT16_bm,
	/**
	 * The counters MAX value is 0xFFFFFFFF. There is no separate
	 * period register, to modify top one of the capture compare
	 * registers has to be used. This limits the amount of
	 * available channels.
	 */
	TC_RESOLUTION_32BIT                   = TC_MODE_COUNT32_bm,
};


/*
 * Have removed the period uint from the 16 and 32 bit struct. You
 * only have cc0 be the period when you are not in PWM normal mode.
 * It may be that you only get to use two compare channels fro pwm
 * too. That is dependent on IC dep. As this is now this reflects how
 * the register is in the prelim TC doc.
 */


/**
 * \brief 
 *
 * 
 */
struct tc_8bit_conf {
	/**  */
	uint8_t count;
	/**  */
	uint8_t period;
	/**  */
	uint8_t compare_capture_channel_0;
	/**  */
	uint8_t compare_capture_channel_1;
	/**  */
	uint8_t compare_capture_channel_2;
	/**  */
	uint8_t compare_capture_channel_3;
};


/**
 * \brief 
 *
 * 
 */
struct tc_16bit_conf {
	/**  */
	uint16_t count;
	/**  */
	uint16_t compare_capture_channel_0;
	/**  */
	uint16_t compare_capture_channel_1;
	/**  */
	uint16_t compare_capture_channel_2;
	/**  */
	uint16_t compare_capture_channel_3;
};


/**
 * \brief 
 *
 * 
 */
struct tc_32bit_conf {
	/**  */
	uint32_t count;
	/**  */
	uint32_t compare_capture_channel_0;
	/**  */
	uint32_t compare_capture_channel_1;
};

/**
 * \brief TC configuration structure
 *
 * Configuration structure for a TC instance. This structure should be
 * initialized by the \ref spi_get_config_defaults function before being
 * modified by the user application.
 */
struct tc_conf {
	/** Select clock sourse for the TC clock */
	//TODO set clock param
	/**
	 * run_in_standby: When true the module is enabled during
	 * standby
	 */
	bool run_in_standby;
	/** Specifies either 8, 16 or 32 bit counter resolution */
	enum tc_resolution resolution;
	/** Specifies the prescaler value for GCLK_TC */
	enum tc_clock_prescaler clock_prescaler;
	/** Specifies which waveform generation mode to use*/
	enum tc_wave_generation wave_generation;
	/**
	 *  Specifies the reload or reset time of the counter and
	 *  prescaler resynchronization on a retrigger event for
	 *  timer/counter
	 */
	enum tc_reload_action reload_action;

	/** Specifies which channel(s) to invert the waveform on */
	uint16_t waveform_invert_output;
	/**
	 *  Specifies which channel (s) to enable channel capture
	 *  operation on. Since all capture channels use the same
	 *  event line, only one capture channel must be enabled at a
	 *  time, when used for event capture. When pwm capture is
	 *  used this is not the case as it does not use the event
	 *  line.
	 */
	uint16_t capture_enable;

	/**
	 *  Oneshot enabled will stop the TC on next HW/SW retrigger
	 *  event or overflow/underflow
	 */
	bool oneshot;
	/** Specifies the direction for the TC to count */
	enum tc_count_direction count_direction;

	/**
	 *  Specifies if the input event source is inverted, when used
	 *  in PWP, PPW event action
	 */
	bool invert_event_input;
	/**Must be set to true to enable event actions */
	bool enable_event_input;

	/** Specifies which event to trigger if an event is triggered */
	enum tc_event_action event_action;
	/**
	 *  Specifies which channel(s) to generate event on when a
	 *  compare/capture occurs
	 */
	uint16_t event_generation_enable;

	union {
		struct tc_8bit_conf 8bit_conf;
		struct tc_16bit_conf 16bit_conf;
		struct tc_32bit_conf 32bit_conf;
	};
};


/**
 * \brief 
 *
 * 
 */
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
static inline void _tc_wait_for_sync(TC_t  *const hw_dev)
{
	/* Sanity check arguments  */
	Assert(hw_dev);

	/* Synchronize  */
	while(hw_dev->STATUS & TC_SYNCBUSY_bm) {
		/* Wait for sync */
	}
}


/**
 * \brief Initializes config with predefined default values
 *
 * This function will initialize a given TC configuration structure to
 * a set of known default values. This function should be called on
 * any new instance of the configuration structures before being
 * modified by the user application.
 *
 * The default configuration is as follows:
 *  \li 16 bit resolution on the counter
 *  \li ...
 *
 * \param[out] config pointer to the config struct
 */
static inline void tc_get_config_defaults(struct tc_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Write default config to config struct */
	config->resolution                   = TC_16BIT_RESOLUTION;
	config->prescaler                    = TC_PRESCALER_DIV1;
	config->wave_generation              = TC_WAVE_GENERATION_NORMAL_FREQ;
	config->reload_action                = TC_RELOAD_ACTION_GCLK;
	config->run_in_standby               = false;

	config->waveform_invert_output       = TC_WAVEFORM_INVERT_OUTPUT_NONE;
	config->capture_enable               = TC_CAPTURE_ENABLE_NONE;

	config->count_direction              = TC_COUNT_DIRECTION_UP;
	config->oneshot                      = false;

	config->enable_event_input           = false;
	config->invert_event_input           = false;
	config->event_action                 = TC_EVENT_ACTION_OFF;
	config->event_generation_enable      = TC_EVENT_GENERATION_ENABLE_NONE;

	config->16bit_conf.count                       = 0x0000;
	config->16bit_conf.compare_capture_channel_0   = 0xFFFF;
	config->16bit_conf.compare_capture_channel_1   = 0x0000;
	config->16bit_conf.compare_capture_channel_2   = 0x0000;
	config->16bit_conf.compare_capture_channel_3   = 0x0000;
}


enum status_code tc_init(
		TC_t *const tc_module,
		struct tc_dev_inst *const dev_inst,
		struct tc_conf *const config);


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
	_tc_wait_for_sync(tc_module);

	/* Enable TC module, based on the module tc mode  */
	tc_module->CTRLA = TC_RESET_bm;
}


/**
 * \brief Enable the TC module
 *
 * Enables the TC module.
 *
 * \param dev_inst    pointer to the device struct
 */
static inline void tc_enable(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(tc_module);

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
static inline void tc_disable(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(tc_module);

	/* Disable TC module */
	tc_module->CTRLA  &= ~TC_ENABLE_bm;
}


uint32_t tc_get_count(struct tc_dev_inst *const dev_inst);


/**
 * \brief Sets the count value
 *
 * Sets the count value of the TC module
 *
 * \param dev_inst    pointer to the device struct
 * \return enum status_code STATUS_OK, STATUS_ERR_INVALID_ARG
 */

enum status_code tc_set_count(
		struct tc_dev_inst *const dev_inst,
		uint32_t count);


/**
 * \brief 
 *
 * \param dev_inst      
 */
static inline void tc_start_counter(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(tc_module);

	/* Write command to execute */
	tc_module->CTRLBSET |= TC_COMMAND_START_bm;
}


/**
 * \brief Stops the counter.
 *
 * This function will stop the counter. And...
 *
 * \param dev_inst    pointer to the device struct
 */
static inline void tc_stop_counter(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(tc_module);

	/* Write command to execute */
	tc_module->CTRLBSET |= TC_COMMAND_STOP_bm;
}


enum status_code tc_get_capture_value(
		struct tc_dev_inst *dev_inst,
		uint32_t *period_value
		enum tc_compare_capture_channel_index ccc_index);


enum status_code tc_set_compare_value(
		struct tc_dev_inst *dev_inst,
		uint32_t compare_value,
		enum tc_compare_capture_channel_index ccc_index);


/**
 * \brief Sets the top/period value
 *
 * Writes the top value for counter associated with the dev_inst struct
 *
 * \param dev_inst    pointer to the device struct
 * \param top_value   value to be used as top in the counter
 *
 * \return
 * \retval STATUS_OK
 * \retval STATUS_ERR_INVALID_ARG
 */
static enum status_code tc_set_top_value(
		struct tc_dev_inst *dev_inst,
		uint32_t top_value)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(compare);

	TC_t * const tc_module = dev_inst->hw_dev;

	_tc_wait_for_sync(tc_module);

	switch(dev_inst->resolution) {

	case TC_RESOLUTION_8BIT:
		tc_module->TC_COUNT8.PER = (uint8_t) top_value;
		return STATUS_OK;

	case TC_RESOLUTION_16BIT:
		tc_module->TC_COUNT16.compare_capture_channel_0 =
			(uint16_t) top_value;
		return STATUS_OK;

	case TC_RESOLUTION_32BIT:
		tc_module->TC_COUNT32.compare_capture_channel_0 =
			top_value;
		return STATUS_OK;

	default:
		Assert(false);
		return STATUS_ERR_INVALID_ARG;
	}
}


/**
 * \brief Checks if there are new capture value on channel
 *
 * Checks for new data on the specified channel or if the counter has over run,
 * in which case it returns an error.
 *
 * \param *dev_inst pointer to the devise instance.
 * \param channel_index value used to select either channel 0, 1, 2 or 3.
 * 
 * \return
 * \retval STATUS_VALID_DATA
 * \retval STATUS_NO_CHANGE
 * \retval STATUS_ERR_BAD_DATA
 */
enum status_codes tc_is_new_capture_on_channel(
		struct tc_dev_inst *dev_inst,
		enum tc_compare_capture_channel_index channel_index)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(channel_index);

	TC_t * const tc_module = dev_inst->hw_dev;

	if(tc_module->INTFLAG & TC_INTFLAG_ERR_bm) {
			tc_module->INTFLAG |= TC_INTFLAG_ERR_bm;
			return STATUS_ERR_BAD_DATA;
		}
	if(tc_module->INTFLAG & (0x01 << (channel_index + 4))) {
		tc_module->INTFLAG |= (0x01 << (channel_index + 4));
		return STATUS_VALID_DATA;
	}
	else {
		return STATUS_NO_CHANGE;
	}
}


/**
 * \brief Checks if a new match has occurred on the channel
 *
 * Checks if a new match has occurred returns STATUS_VALID_DATA if it
 * has and STATUS_NO_CHANGE if no new match has occurred.
 *
 * \param *dev_inst pointer to the devise instance.
 * \param channel_index value used to select either channel 0, 1, 2 or 3.
 *
 * \return A status telling whether a match has ocured
 * \retval STATUS_VALID_DATA   If a match has ocured since last check
 * \retval STATUS_NO_CHANGE    If no new match ha ocured since last check
 */
enum status_codes tc_is_there_match_on_channel(
		struct tc_dev_inst *dev_inst,
		enum tc_compare_capture_channel_index channel_index)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(channel_index);

	TC_t * const tc_module = dev_inst->hw_dev;

	if(tc_module->INTFLAG & (0x01 << (channel_index + 4))) {
		tc_module->INTFLAG |= (0x01 << (channel_index + 4));
		return STATUS_VALID_DATA;
	}
	else {
		return STATUS_NO_CHANGE;
	}
}


/** @} */

#ifdef __cplusplus
}
#endif

/** @} */



/**
 * \page spi_extra_info Extra Information
 *
 * \section acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>TC</td>
 *		<td>Timer Counter</td>
 *	</tr>
 * </table>
 *
 * \section workarounds Workarounds implemented by driver
 * No workarounds in driver.
 *
 * \section module_history Module History
 * Below is an overview of the module history, detailing enhancements and fixes
 * made to the module since its first release. The current version of this
 * corresponds to the newest version listed in the table below.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page quickstart Quick Start Guides for the TC module
 *
 * This is the quick start guide list for the \ref sam0_spi_group module, with
 * step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function of the
 * user application and run at system startup, while the steps for usage can be
 * copied into the normal user application program flow.
 *
 * \see General list of module \ref module_examples "examples".
 *
 * \section spi_use_cases SPI driver use cases
 * - \subpage spi_basic_use_case
 */
#endif /* TC_ALL_HEADER */
