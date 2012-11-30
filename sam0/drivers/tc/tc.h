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

#include <compiler.h>


/**
 * \defgroup sam0_TC_group SAM0+ Timer Counter Driver (TC)
 *
 * Driver for the SAM0+ architecture devices. This driver provides an
 * interface for configuration and management of the TC module. This
 * driver encompasses the following module within the SAM0+ devices:
 * \li \b TC \b (Timer Counter)
 *
 * \section module_introduction Introduction
 *
 * \subsection module_overview TC Overview
 * The Timer counter (TC) enables the user to create PWM signals, do
 * waveform generation, keep time, generate time labels for events, do
 * rudimentary counting, perform input capture, pulse width capture,
 * frequency capture, and more.
 * \n\n
 *
 * A TC is basically a counter however if it is supplied with an accurate
 * frequency to count, the module becomes capable of performing timer
 * operations.
 *
 * This driver is cheated and meant to be used as a polled driver, as
 * such this documentation will not go into the use of interrupts with
 * regards to the TC module. It should also be noted that this is not
 * complete documentation for the TC module but for the functionality
 * this driver can deliver.
 *
 *
 * \subsection functional_description Functional Description
 *
 * describe in some more detail the different possible configurations
 * the user can get from the module. The amount of counters capture/compare
 * registers oneshot, event system connection. Waveform generation and
 * pin outs. period register. count direction. count sources. capture.
 *
 * The TC module in SAM0+ can be configured with 4 timer/counters in 8
 * bit resolution, or 4 timers/counters in 16 bit resolution, or 2
 * timers/counters in 32 bit resolution.
 * \n\n
 *
 * Independent of what resolution the timer runs on it can basically be
 * set up in two different modes, although to some extent one TC instance
 * can be configured in both modes. These modes are capture and
 * compare.
 * \n\n
 *
 * In compare mode the counter value is compared with one or more of
 * the compare values. When the counter coincides with the compare
 * value, this can generate an action. The capture mode is often used
 * for generating output signals such as waveform generation,
 * frequency generation and PWM generation. In addition the user can
 * use compare mode for timer applications where the value used in
 * comparison with the counter value signifies a time for when a
 * subroutine or an even should start. In a similar fashion it can be
 * used to count certain events and when this reaches a certain value
 * an action is taken.  \n\n
 *
 * In capture mode the counter value is stored upon some configurable
 * event. This can be used to generate time stamps or it can be used
 * for frequency capture, event capture and pulse width capture.
 *
 *
 * \subsection counter_resolution Timer Counter Resolution
 *
 * When talking about resolution with respect to the timer counter
 * what is meant is the bit precision the user can get with the counter. The
 * timer counter in the SAM0+ can achieve 8, 16 and 32 bit precision or
 * resolution. This naturally effects how far the counter is capable of
 * counting. For easy reference the different modes and their Max value
 * can be seen in the table below
 *
 * <table>
 *  <tr>
 *    <th> <\th>
 *    <th> colspan="2" MAX <\th>
 *  <\tr>
 *  <tr>
 *    <th> Resolution <\th>
 *    <th> Hexadecimal <\th>
 *    <th> Decimal <\th>
 *  <\tr>
 *  <tr>
 *    <th> 8 bit <\th>
 *    <td> 0xFF <\td>
 *    <td> 256 <\td>
 *  <tr>
 *  <tr>
 *    <th> 16 bit <\th>
 *    <td> 0xFFFF <\td>
 *    <td> 65536 <\td>
 *  <tr>
 *  <tr>
 *    <th> 8 bit <\th>
 *    <td> 0xFFFFFFFF <\td>
 *    <td> 4294967296 <\td>
 *  <tr>
 * </table>
 *
 * It should be noted that when running the counter in 16 and 13 bit
 * resolution, compare capture register 0 is used to store the period
 * value, when this is in use (when running in pwm generation match
 * mode).  \n\n

//TODO: may have to update this when the pwm module is complete

 *
 * When using 32 bit resolution two 16 bit counters are used together
 * in cascade to realize this feature. This means that it is only
 * posible to utilize two 32 bit counters or one 32 bit counters and
 * ether one 16 and one 8 bit counter or two 8 or 16 bit counters.
 *
 * \subsection clock_and_prescaler Clock Selection, Prescaler and Reload Action
 *
 * To be able to use the counter the module has to have a clock
 * this has to be configured in the config struct... .  \n\n
 *
 * In addition to the prescaler that can be used on the g-clock
 * frequency supplied to the module, the module has its own prescaler
 * This prescaler only works to prescale the clock used to provide
 * clock pulses to the counter. This means that while the counter is
 * running on the g-clock frequency the counter value only changes on
 * the prescaled frequency.  \n\n
 *
 * One thing to consider here is that the TC module will have to
 * synchronize when updating certain registers with the system
 * clock. This synchronization can be time consuming espesialy if the
 * g-clock frequensy is much lower thatn the system clock. For this
 * reason it can be better to use the modules prescaler to reduse the
 * clock frequensy to the counter. In this way synchronizationshould
 * be faster. \n\n
 *
 * In addition to configuring what clock source the tc module should
 * use and if the prescaler is needed on the clock to the counter. The
 * user have to consider what reload action to use. The reload action
 * is performed when a retrigger event ocure. Examples of this can be
 * when the counter reches the MAX value when counting up, or when an
 * event from the event system tels the counter to retrigger. The user
 * can then chose between 3 different reload actions:
 *
 * <table>
 *  <tr>
 *   <th> Description <\th>
 *   <th> code reference<\th>
 *  <\tr>
 *  <tr>
 *   <th> Reload on next G-clock cyckle leave prescaler as it is <\th>
 *   <td>  <\td>
 *  <\tr>
 *  <tr>
 *   <th> Relad on next prescaler clock <\th>
 *   <td> <\td>
 *  <\tr>
 *  <tr>
 *   <th> Reload on next G-clock cyckle  reset prescaler <\th>
 *   <td> <\td>
 *  <\tr>
 * <table>
 *
 * To better understand thes options it helps to have a rudementary
 * understanding of what the presclaer does. Thankfully this is not so
 * dificult to understand. The presclaer is after all only a counter
 * it self. When the prescaler is used it counts the clock cycles of
 * the modules g-clock. When the counter in the prescaler ratches the
 * chosen division factor the output from the prescaler togles.  \n\n
 *
 * The question then arises what do we want to happen when a reload
 * action gets trigered. In difernt secarios and aplications the
 * corect reload option will differ. One exsample is when an external
 * triger for a reload ocures. If the TC uses the prescaler the
 * counter in the prescaler should not have a value between zero and
 * the division factor. The TC counter and the counter in the
 * prescaler should both start at zero. When the counter is set to
 * retrigger when it reatches the MAX value on the other hand this is
 * not nesesearily the right option to use. In such a case it would
 * probably be better if th eprescaler is left un althered when the
 * retriger happens and then let the counter reset on the next g-clock
 * cyckle.
 *

//TODO: may have to be updated after having done teh doc review

 *
 * \subsection compare_match Compare Match Operations
 *
 * In compare match operation Capture Compare Channel registers are
 * used in comparison with the counter value. Upn match some action
 * can be taken.
 *
 *
 * \subsubsection timer Timer
 *
 * In many ways the simplest aplication wher compare match operations
 * is used, is a timer. Here the TC module is configured as a clock,
 * wher one or more values in the modules compare channels are used to
 * spesify the time when an action shold be taken by the
 * microcontroler.
 *
 * \subsubsection waveform_generation Waveform Generation
 *
 * Wave form generation enable the TC module, in combination with a
 * pasive filter to produce analogue wave formes, in adition to square
 * waves.
 *
 * \subsubsection frequency_generation Frequency Generation
 *
 * 
 *
 * \subsubsection pwm Pulse Width modulation (PWM)
 *
 * 
 *
 *
 * \subsection capture_operations Capture Operations
 *
 * In capture operations some action causes the clock value to be
 * stored and some action can then be performed depending on how the
 * module is configured.
 *
 * \subsubsection event_capture Event Capture
 *
 * 
 *
 * \subsubsection frequency_capture Frequency Capture
 *
 * 
 *
 * \subsubsection pwc Pulse width capture
 *
 * 
 *
 *
 * \subsection events Events
 *
 * 
 *
 *
 * \subsection sleep_modes Operation in Sleep Modes
 *
 * 
 *
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
 * generation in. The user can chose to use frequency generation or
 * pulse width modulation (PWM) In these modes it will either run in
 * normal mode or in match mode.  In normal mode the TOP value is set
 * to the maximum allowable value, depending on what resolution is
 * used, MAX is either 0xFF, 0xFFFF or 0xFFFFFFFF. In normal mode no
 * registers are used to store these values.
 *
 * In match mode the user can configure what the top value should
 * be. The range will be between the minimum value which is 3, up to
 * the MAX value.  This mode does however limit the number of compare
 * capture channels available, as one is used to store the TOP value.
 */
enum tc_wave_generation {
	/**
	 * TOP is MAX, except in 8 bit resolution where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_NORMAL_FREQ         = TC_WAVEGEN_NFRQ_bm,
	/**
	 * TOP is CC0, expert in 8 bit resolution where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_MATCH_FREQ          = TC_WAVEGEN_MFRQ_bm,
	/**
	 * TOP is MAX, except in 8 bit resolution where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_NORMAL_PWM          = TC_WAVEGEN_NPWM_bm,
	/**
	 * TOP is CC0, except in 8 bit resolution where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_MATCH_PWM           = TC_WAVEGEN_MPWM_bm,
};


/**
 * \brief Count direction enum.
 *
 * The counter can be set to either count up or down
 */
enum tc_count_direction {
	/** Makes the counter count up from zero */
	TC_COUNT_DIRECTION_UP,
	/**
	 * Makes the counter cunt down from MAX or a specific user
	 * defined value between max and 3
	 */
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
	/** Dont invert output on anny channels */
	TC_WAVEFORM_INVERT_OUTPUT_NONE         = 0,
	/** Invert output on channel 0 */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_0    = TC_INVEN_CC0_bm,
	/** Invert output on channel 1 */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_1    = TC_INVEN_CC1_bm,
	/** Invert output on channel 2 */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_2    = TC_INVEN_CC2_bm,
	/** Invert output on channel 3 */
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
 * This specifies the maximum value it is posible to count to. What
 * resolution used, will impose limitations in other areas.  The 32
 * bit counter uses two 16 bit counters in cascade to realize the 32
 * bit counter. When using 16 and 32 bit resolution there is no
 * dedicated period register. In cases when it is nesesary to change
 * the top value, match mode must be used (see
 * tc_wave_generation). One of the compare registers are used for the
 * period in this mode.
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


/**
 * \brief Config struct for 8 bit resolution
 *
 * Config struct for when using 8 bit resolution on the counter
 */
struct tc_8bit_conf {
	/** Initial count value */
	uint8_t count;
	/**
	 * Where to count to or from depending on the direction on the
	 * counter
	 */
	uint8_t period;
	/** Value to be used for compare match on channel 0 */
	uint8_t compare_capture_channel_0;
	/** Value to be used for compare match on channel 1 */
	uint8_t compare_capture_channel_1;
	/** Value to be used for compare match on channel 2 */
	uint8_t compare_capture_channel_2;
	/** Value to be used for compare match on channel 3 */
	uint8_t compare_capture_channel_3;
};


/**
 * \brief Config struct for 16 bit resolution
 *
 * Config struct for when using 16 bit resolution on the counter
 */
struct tc_16bit_conf {
	/** Initial count value */
	uint16_t count;
	/** Value to be used for compare match on channel 0 */
	uint16_t compare_capture_channel_0;
	/** Value to be used for compare match on channel 1 */
	uint16_t compare_capture_channel_1;
	/** Value to be used for compare match on channel 2 */
	uint16_t compare_capture_channel_2;
	/** Value to be used for compare match on channel 3 */
	uint16_t compare_capture_channel_3;
};


/**
 * \brief Config struct for 16 bit resolution
 *
 * Config struct for when using 16 bit resolution on the counter
 */
struct tc_32bit_conf {
	/** Initial count value */
	uint32_t count;
	/** Value to be used for compare match on channel 0 */
	uint32_t compare_capture_channel_0;
	/** Value to be used for compare match on channel 1 */
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
	/** Select clock source for the TC clock */
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
	uint8_t waveform_invert_output;
	/**
	 *  Specifies which channel (s) to enable channel capture
	 *  operation on. Since all capture channels use the same
	 *  event line, only one capture channel must be enabled at a
	 *  time, when used for event capture. When pwm capture is
	 *  used this is not the case as it does not use the event
	 *  line.
	 */
	uint8_t capture_enable;

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
 * \brief TC software device instance structure
 *
 * TC software device instance structure
 */
struct tc_dev_inst {
	/** Pointer to the TC Hardware module */
	TC_t *hw_dev;
	/**
	 * Witch resolution the counter should use. This value is set
	 * when running tc_init. Use the config struct to set the
	 * resolution. Do not alter this, it is only for internal
	 * checks.
	 */
	enum tc_resolution resolution;
};


/**
 * \brief Synchronization between clock domains
 *
 * Makes sure GCLK_TC is synchronized with sysclock,
 * Must be called before assessing certain registers in the TC.
 * Blocks while waiting
 *
 * \param dev_inst pointer to device instace
 */
static inline void _tc_wait_for_sync(const struct tc_dev_inst  *const dev_inst)
{
	/* Sanity check arguments  */
	Assert(dev_inst);

	TC_t tc_module = dev_inst->hw_dev;

	/* Synchronize  */
	while(tc_module->STATUS & TC_SYNCBUSY_bm) {
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
 *  \li No prescaler
 *  \li Normal frequency wave generation
 *  \li GCLK reload action
 *  \li Don't run in standby
 *  \li No inversion of waveform output
 *  \li No capture enabled
 *  \li Count upward
 *  \li Don't perform oneshot operations
 *  \li No event input enabled
 *  \li no event Action
 *  \li No event generation enabled
 *  \li Counter starts on 0
 *  \li Capture compare channel 0 set to 0xFFFF
 *  \li Capture compare channel 1 set to 0
 *  \li Capture compare channel 2 set to 0
 *  \li Capture compare channel 3 set to 0
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
 * Resets the TC module. Will block while waiting for sync between
 * clock domains before performing the reset. The TC module will not
 * be accessible while the reset is being performed.
 *
 * \param dev_inst    pointer to the device struct
 */
static inline void tc_reset(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments  */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module hardware instance */
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Enable TC module, based on the module tc mode  */
	tc_module->CTRLA = TC_RESET_bm;
}


/**
 * \brief Enable the TC module
 *
 * Enables the TC module. Except when the counter is enabled for
 * retrigger on even the counter will start when the counter i enabled.
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
	_tc_wait_for_sync(dev_inst);

	/* Enable TC module */
	tc_module->CTRLA |= TC_ENABLE_bm;
}


/**
 * \brief Disables the TC module
 *
 * Disables the TC module. Stops the counter. This is reqired for
 * changeing certain settings. The settings effected by this are
 * listed below.
 *
 * \li Run in standby
 * \li Prescaler values
 * \li Wave generation
 * \li Resync action
 * \li counter resolution
 * \li Capture enable
 * \li Invert output waveform
 * \li Event action selection
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
	_tc_wait_for_sync(dev_inst);

	/* Disable TC module */
	tc_module->CTRLA  &= ~TC_ENABLE_bm;
}


uint32_t tc_get_count_value(struct tc_dev_inst *const dev_inst);


enum status_code tc_set_count_value(
		struct tc_dev_inst *const dev_inst,
		uint32_t count);


/**
 * \brief Stops the counter
 *
 * This function will stop the counter. When the counter is stopped the
 * value in the count register is set to 0, if the counter was
 * counting up or MAX, if the counter was counting down when stopped.
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
	_tc_wait_for_sync(dev_inst);

	/* Write command to execute */
	tc_module->CTRLBSET = TC_COMMAND_STOP_bm;
}


/**
 * \brief Starts the counter
 *
 * This function can be used to start the counter. It will also
 * restart the counter after a stop action has been performed.
 *
 * \param dev_inst     Pointer to the devise struct.
 */
static inline void tc_start_counter(struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TC_t *const tc_module = dev_inst->hw_dev;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Write command to execute */
	tc_module->CTRLBSET = TC_COMMAND_START_bm;
}


enum status_code tc_get_capture_value(
		struct tc_dev_inst *const dev_inst,
		uint32_t *period_value
		enum tc_compare_capture_channel_index channel_index);


enum status_code tc_set_compare_value(
		struct tc_dev_inst *const dev_inst,
		uint32_t compare_value,
		enum tc_compare_capture_channel_index channel_index);


/**
 * \brief Sets the top/period value
 *
 * Writes the top value or period for the counter associated with the
 * dev_inst struct. In the case of a counter running in 8 bit
 * resolution a dedicated register is used for the period. For 16 and
 * 32 bit resolution there is no dedicated register for the period/Top
 * value. In these cases capture compare register 0 is used, and can
 * not be used for anything else.
 *
 * \param dev_inst    Pointer to the device struct.
 * \param top_value   Value to be used as top in the counter.
 *
 * \return status of the procedure
 * \retval STATUS_OK              The operation has been successful.
 * \retval STATUS_ERR_INVALID_ARG The resolution in the dev_inst is
 *                                out of bounds.
 */
static enum status_code tc_set_top_value(
		struct tc_dev_inst *const dev_inst,
		uint32_t top_value)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(compare);

	TC_t * const tc_module = dev_inst->hw_dev;

	_tc_wait_for_sync(dev_inst);

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
		struct tc_dev_inst *const dev_inst,
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
 * Checks if a new match has occurred on the channel specified in
 * channel_index.
 *
 * \param *dev_inst pointer to the devise instance.
 * \param channel_index value used to select either channel 0, 1, 2 or 3.
 *
 * \return A status telling whether a match has occurred
 * \retval STATUS_VALID_DATA   If a match has occurred since last check
 * \retval STATUS_NO_CHANGE    If no new match ha occurred since last check
 */
enum status_codes tc_is_there_match_on_channel(
		struct tc_dev_inst *const dev_inst,
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
