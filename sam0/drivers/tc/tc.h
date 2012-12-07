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
#include <gclk.h>
#include <clock.h>


/**
 * \defgroup sam0_TC_group SAM0+ Timer Counter Driver (TC)
 *
 * Driver for the SAM0+ architecture devices. This driver provides an
 * interface for configuration and management of the TC module. This
 * driver encompasses the following module within the SAM0+ devices:
 * \li \b TC \b (Timer Counter)
 * \n\n
 *
 * This driver is chreated and meant to be used as a polled driver, as
 * such this documentation will not go into the use of interrupts with
 * regards to the TC module. It should also be noted that this is not
 * complete documentation for the TC module but for the functionality
 * this driver can deliver.
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
 * \n\n
 *
 * The TC in the SAMD20 has got its own prescaler that can be used to
 * divide the clock frequensy used in the counter. It is posible to
 * configure the counters to use either 32, 16 or 8 bit
 * resolution. No mather the resolution you will get two capture
 * compare registers that can be used either for capture or compare
 * operations.
 *
 *
 * \subsection functional_description Functional Description
 *
 * The TC module in SAMD20 can be configured with 4 timer/counters in
 * 8 bit resolution, or 4 timers/counters in 16 bit resolution, or 2
 * timers/counters in 32 bit resolution.  \n\n
 *
 * Independent of what resolution the timer runs on it can be set up
 * in two different modes, although to some extent one TC instance can
 * be configured in both modes. These modes are capture and compare.
 * \n\n
 *
 * In compare mode the counter value is compared with one or more of
 * the compare values. When the counter value coincides with the
 * compare value, this can generate an action. The capture mode is
 * often used for generating output signals such as waveform
 * generation, frequency generation and PWM generation. In addition
 * the user can use compare mode for timer applications where the
 * value used in comparison with the counter value signifies a time
 * for when a subroutine or an even should start. In a similar fashion
 * it can be used to count certain events and when this reaches a
 * certain value an action is taken.  \n\n
 *
 * In capture mode the counter value is stored upon some configurable
 * event. This can be used to generate time stamps or it can be used
 * for frequency capture, event capture and pulse width capture.
 *
 *
 * \subsection counter_resolution Timer Counter Resolution
 *
 * When talking about resolution with respect to the timer counter
 * what is meant is the bit precision the user can get with the
 * counter. The timer counter in the SAM0+ can achieve 8, 16 and 32
 * bit precision or resolution. This naturally effects how far the
 * counter is capable of counting. For easy reference the different
 * modes and their Max value can be seen in the table below
 *
 * <table>
 *  <tr>
 *    <th> - </th>
 *    <th colspan="2">MAX </th>
 *  </tr>
 *  <tr>
 *    <th> Resolution </th>
 *    <th> Hexadecimal </th>
 *    <th> Decimal </th>
 *  </tr>
 *  <tr>
 *    <th> 8 bit </th>
 *    <td> 0xFF </td>
 *    <td> 256 </td>
 *  </tr>
 *  <tr>
 *    <th> 16 bit </th>
 *    <td> 0xFFFF </td>
 *    <td> 65536 </td>
 *  </tr>
 *  <tr>
 *    <th> 32 bit </th>
 *    <td> 0xFFFFFFFF </td>
 *    <td> 4294967296 </td>
 *  <tr>
 * </table>
 *
 * It should be noted that when running the counter in 16 and 13 bit
 * resolution, compare capture register 0 is used to store the period
 * value, when this is in use (when running in pwm generation match
 * mode).
 * \n\n
 *
 * When using 32 bit resolution two 16 bit counters are used together
 * in cascade to realize this feature. This means that it is only
 * possible to utilize two 32 bit counters or one 32 bit counters and
 * ether one 16 and one 8 bit counter or two 8 or 16 bit counters.
 *
 * \subsection clock_and_prescaler Clock Selection, Prescaler and Reload Action
 *
 * To be able to use the counter the module has to have a clock
 * this has to be configured in the config struct... .
 * \n\n
 *
 * In addition to the prescaler that can be used on the g-clock
 * frequency supplied to the module, the module has its own prescaler
 * This prescaler only works to prescale the clock used to provide
 * clock pulses to the counter. This means that while the counter is
 * running on the g-clock frequency the counter value only changes on
 * the prescaled frequency.
 * \n\n
 *
 * One thing to consider here is that the TC module will have to
 * synchronize when updating certain registers with the system
 * clock. This synchronization can be time consuming especially if the
 * g-clock frequency is much lower than the system clock. For this
 * reason it can be better to use the modules prescaler to reduce the
 * clock frequency to the counter. In this way synchronization should
 * be faster.
 * \n\n
 *
 * In addition to configuring what clock source the tc module should
 * use and if the prescaler is needed on the clock to the counter. The
 * user have to consider what reload action to use. The reload action
 * is performed when a retrigger event occur. Examples of this can be
 * when the counter reaches the MAX value when counting up, or when an
 * event from the event system tels the counter to retrigger. The user
 * can then chose between 3 different reload actions:
 *
 * <table>
 *   <tr>
 *     <th> Description </th>
 *     <th> code reference</th>
 *   </tr>
 *   <tr>
 *     <th> Reload on next G-clock cycle leave prescaler as it is </th>
 *     <td>  </td>
 *   </tr>
 *   <tr>
 *     <th> Reload on next prescaler clock </th>
 *     <td> - </td>
 *   </tr>
 *  <tr>
 *    <th> Reload on next G-clock cycle, reset prescaler </th>
 *    <td> - </td>
 *  </tr>
 * </table>
 *
 * To better understand these options it helps to have a rudimentary
 * understanding of what the prescaler does. Thankfully this is not so
 * difficult to understand. The prescaler is after all only a counter
 * it self. When the prescaler is used it counts the clock cycles of
 * the modules g-clock. When the counter in the prescaler reaches the
 * chosen division factor the output from the prescaler toggles.
 * \n\n
 *
 * The question then arises what do we want to happen when a reload
 * action gets triggered. In different scenarios and applications the
 * correct reload option will differ. One example is when an external
 * trigger for a reload occur. If the TC uses the prescaler the
 * counter in the prescaler should not have a value between zero and
 * the division factor. The TC counter and the counter in the
 * prescaler should both start at zero. When the counter is set to
 * retrigger when it reaches the MAX value on the other hand this is
 * not necessarily the right option to use. In such a case it would
 * probably be better if th prescaler is left unaltered when the
 * retrigger happens and then let the counter reset on the next g-clock
 * cycle.
 *

//TODO: may have to be updated after having done the doc review

 *
 * \subsection compare_match Compare Match Operations
 *
 * In compare match operation Capture Compare Channel registers are
 * used in comparison with the counter value. Upon match some action
 * can be taken.
 *
 * \subsubsection timer Timer
 *
 * In many ways the simplest application where compare match operations
 * is used, is a timer. Here the TC module is configured as a clock,
 * where one or more values in the modules compare channels are used to
 * specify the time when an action should be taken by the
 * microcontroller.
 *
 * \subsubsection waveform_generation Waveform Generation
 *
 * Wave form generation enable the TC module to make square waves or
 * if combined with a passive low pas filter, different analogue waves.
 *
 * \subsubsection pwm Pulse Width modulation (PWM)
 *
 * Pulse with modulation is a signaling technique that can be useful
 * in many situations. Often it is used to communicate a certain value
 * to some other circuit or component such as a servo. In servo
 * control you let the pulse with relative to the period signify the
 * angle the servo should take. Using PWM for this is far less prone to
 * noise and differing impedances, compared to using an analogue
 * voltage value.
 * \n\n
 *
 * PWM signals are generated by configuring the tc_wave_generation
 * enum in the tc_conf struct, and using either the
 * TC_WAVE_GENERATION_MATCH_PWM mode or the
 * TC_WAVE_GENERATION_NORMAL_PWM mode. You then need to set a compare
 * value using the tc_set_compare_value function. This value gives the
 * pulse with together with the top value. In addition you may need to
 * configure the top value using the set_top_value function, whether
 * this is necessary or not depends on if NORMAL or MATCH
 * configuration is in use.
 *
 *
 * \subsection capture_operations Capture Operations
 *
 * In capture operations some action causes the clock value to be
 * stored. After this has been done some action can be performed,
 * depending on how the module is configured.
 *
 * \subsubsection event_capture Event Capture
 *
 * Event capture is in some ways the simplest use of the capture
 * functionality. This lets you create time stamps for specific
 * events. Care must be taken though to ensure that the counter value
 * is correct.
 *
 * \subsubsection pwc Pulse width capture
 *
 * Pulse with capture mode lets you measure the pulse with and period
 * of PWM signals. This mode uses both capture channels of the
 * counter. This means that the counter module used for pulse with
 * capture can not be used for any other purpose. There are two modes
 * for pulse with capture, however they differ very little. In PWP
 * mode capture channel 0 is used for storing the pulse width, and
 * capture channel 1 stores the period. While in PPW mode the period
 * is stored in capture channel 0, and the pulse with is stored in
 * capture channel 1.
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
 * For extra information see \ref tc_extra_info.
 *
 * \section module_examples Examples
 * - \ref quickstart
 *
 * \section TC_overview API Overview
 * @{
 */


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
	TC_RELOAD_ACTION_GCLK                 = TC_CTRLA_PRESCSYNC_GCLK,
	/**
	 * The counter is reloaded/reset on the next prescaler clock
	 */
	TC_RELOAD_ACTION_PRESC                = TC_CTRLA_PRESCSYNC_PRESC,
	/**
	 * The counter is reloaded/reset on the next GCLK, and the
	 * prescaler is reset as well. This may be useful when an
	 * event retrigger the clock
	 */
	TC_RELOAD_ACTION_RESYNC               = TC_CTRLA_PRESCSYNC_RESYNC,
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
	TC_CLOCK_PRESCALER_DIV1               = TC_CTRLA_PRESCALER(0),
	/** Divide clock by 2 */
	TC_CLOCK_PRESCALER_DIV2               = TC_CTRLA_PRESCALER(1),
	/** Divide clock by 4 */
	TC_CLOCK_PRESCALER_DIV4               = TC_CTRLA_PRESCALER(2),
	/** Divide clock by 8 */
	TC_CLOCK_PRESCALER_DIV8               = TC_CTRLA_PRESCALER(3),
	/** Divide clock by 16 */
	TC_CLOCK_PRESCALER_DIV16              = TC_CTRLA_PRESCALER(4),
	/** Divide clock by 64 */
	TC_CLOCK_PRESCALER_DIV64              = TC_CTRLA_PRESCALER(5),
	/** Divide clock by 256 */
	TC_CLOCK_PRESCALER_DIV256             = TC_CTRLA_PRESCALER(6),
	/** Divide clock by 1024 */
	TC_CLOCK_PRESCALER_DIV1024            = TC_CTRLA_PRESCALER(7),
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
	TC_WAVE_GENERATION_NORMAL_FREQ         = TC_CTRLA_WAVEGEN_NFRQ,
	/**
	 * TOP is CC0, expert in 8 bit resolution where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_MATCH_FREQ          = TC_CTRLA_WAVEGEN_MFRQ,
	/**
	 * TOP is MAX, except in 8 bit resolution where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_NORMAL_PWM          = TC_CTRLA_WAVEGEN_NPWM,
	/**
	 * TOP is CC0, except in 8 bit resolution where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_MATCH_PWM           = TC_CTRLA_WAVEGEN_MPWM,
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
 * \brief Enum to be used to check interrupt flags
 *
 * These enums are used as input in the tc_is_interrupt_flag_set and
 * tc_clear_interrupt_flag function. The interrupt flags will still be
 * set even when interrupts are not enabled. Interrupt flags has to be
 * checked in certain cases. When checking if there are matches on a
 * channel the interrupt flags has to be used.
 */
enum tc_interrupt_flag {
	/** Interrupt flag for channel 0 */
	TC_INTERRUPT_FLAG_CHANNEL_0 =  TC_INTFLAG_MC(0),
	/** Interrupt flag for channel 1 */
	TC_INTERRUPT_FLAG_CHANNEL_1 =  TC_INTFLAG_MC(1),
	/**  */ //TODO: Have to check for a use case on this with IC
	TC_INTERRUPT_FLAG_READY     =  TC_INTFLAG_READY,
	/**
	 * This flag is used to test for capture overflow in capture
	 * mode
	 */
	TC_INTERRUPT_FLAG_ERROR     =  TC_INTFLAG_ERR,
	/**
	 * This flag can be used to check for a counter overflow in
	 * compare mode
	 */
	TC_INTERRUPT_FLAG_OVERFLOW  =  TC_INTFLAG_OVF,
};


/**
 * \brief Enum for event action
 *
 * Enum to setup specific event actions.
 */
enum tc_event_action {
	/** No event action */
	TC_EVENT_ACTION_OFF               = TC_EVCTRL_EVACT_OFF,
	/** Retrigger on event */
	TC_EVENT_ACTION_RETRIGGER         = TC_EVCTRL_EVACT_RETRIGGER,
	/** Increment counter on event */
	TC_EVENT_ACTION_INCREMENT_COUNTER = TC_EVCTRL_EVACT_COUNT,
	/** Start counter on event */
	TC_EVENT_ACTION_START             = TC_EVCTRL_EVACT_START,
	/**
	 * Store period in capture register 0, pulse with in capture
	 * register 1
	 */
	TC_EVENT_ACTION_PPW               = TC_EVCTRL_EVACT_PPW,
	/**
	 * Store pulse with in capture register 0, period in capture
	 * register 1
	 */
	TC_EVENT_ACTION_PWP               = TC_EVCTRL_EVACT_PWP,
};


/**
 * \brief Enum for for inverting waveform output
 *
 * This enum can be used to setup inversion of the waveform output
 */
enum tc_waveform_invert_output {
	/** No inversion of output */
	TC_WAVEFORM_INVERT_OUTPUT_NONE      = 0,
	/** Invert output from compare channel 0 */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_0 = TC_CTRLC_INVEN(0),
	/** Invert output from compare channel 1 */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_1 = TC_CTRLC_INVEN(1),
};


/**
 * \brief Enum for setting up event generation
 *
 * Use this enum to configure event generation on respective channels
 */
enum tc_event_generation_enable {
	/** No event generation */
	TC_EVENT_GENERATION_ENABLE_NONE      = 0,
	/** Event generation on channel 0 */
	TC_EVENT_GENERATION_ENABLE_CHANNEL_0 = TC_EVCTRL_MCEO(0),
	/** Event generation on channel 1 */
	TC_EVENT_GENERATION_ENABLE_CHANNEL_1 = TC_EVCTRL_MCEO(1),
};

/**
 * \brief TC channel capture enable enum
 */
enum tc_capture_enable {
	/** Enables no channels for capture */
	TC_CAPTURE_ENABLE_NONE                 = 0,
	/** Enables channel 0 for capture */
	TC_CAPTURE_ENABLE_CHANNEL_0            = TC_CTRLC_CPTEN(2),
	/** Enables channel 1 for capture */
	TC_CAPTURE_ENABLE_CHANNEL_1            = TC_CTRLC_CPTEN(1),
};


/**
 * \brief Specifies if the counter is 8, 16 or 32 bits.
 *
 * This specifies the maximum value it is possible to count to. What
 * resolution used, will impose limitations in other areas.  The 32
 * bit counter uses two 16 bit counters in cascade to realize the 32
 * bit counter. When using 16 and 32 bit resolution there is no
 * dedicated period register. In cases when it is necessary to change
 * the top value, match mode must be used (see
 * tc_wave_generation). One of the compare registers are used for the
 * period in this mode.
 */
enum tc_resolution {
	/**
	 * The counters max value is 0xFF, the period register is
	 * available to be used as TOP value
	 */
	TC_RESOLUTION_8BIT                    = TC_CTRLA_MODE_COUNT8,
	/**
	 * The counters MAX value is 0xFFFF. There is no separate
	 * period register, to modify top one of the capture compare
	 * registers has to be used. This limits the amount of
	 * available channels.
	 */
	TC_RESOLUTION_16BIT                   = TC_CTRLA_MODE_COUNT16,
	/**
	 * The counters MAX value is 0xFFFFFFFF. There is no separate
	 * period register, to modify top one of the capture compare
	 * registers has to be used. This limits the amount of
	 * available channels.
	 */
	TC_RESOLUTION_32BIT                   = TC_CTRLA_MODE_COUNT32,
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
 * initialized by the \ref tc_get_config_defaults function before being
 * modified by the user application.
 */
struct tc_conf {
	/** GCLK generator used to clock the peripheral */
	enum gclk_generator clock_source;

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
		struct tc_8bit_conf tc_8bit_conf;
		struct tc_16bit_conf tc_16bit_conf;
		struct tc_32bit_conf tc_32bit_conf;
	}tc_resolution_conf;
};


/**
 * \brief TC software device instance structure
 *
 * TC software device instance structure
 */
struct tc_dev_inst {
	/** Pointer to the TC Hardware module */
	Tc *hw_dev;
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
 * \param dev_inst  Pointer to device instance
 */
static inline void _tc_wait_for_sync(const struct tc_dev_inst  *const dev_inst)
{
	/* Sanity check arguments  */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Synchronize  */
	while(dev_inst->hw_dev->COUNT8.STATUS.reg & TC_STATUS_SYNCBUSY) {
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
 *  \li GCLK generator 0 (GCLK main) clock source
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
 *  \li No event Action
 *  \li No event generation enabled
 *  \li Counter starts on 0
 *  \li Capture compare channel 0 set to 0xFFFF
 *  \li Capture compare channel 1 set to 0
 *  \li Capture compare channel 2 set to 0
 *  \li Capture compare channel 3 set to 0
 *
 * \param[out] config  Pointer to the config struct
 */
static inline void tc_get_config_defaults(struct tc_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Write default config to config struct */
	config->clock_source                 = GCLK_GENERATOR_0;
	config->resolution                   = TC_RESOLUTION_16BIT;
	config->clock_prescaler              = TC_CLOCK_PRESCALER_DIV1;
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

	config->tc_resolution_conf.tc_16bit_conf.count                     =
			0x0000;
	config->tc_resolution_conf.tc_16bit_conf.compare_capture_channel_0 =
			0xFFFF;
	config->tc_resolution_conf.tc_16bit_conf.compare_capture_channel_1 =
			0x0000;
}


enum status_code tc_init(
		Tc *const tc_module,
		struct tc_dev_inst *const dev_inst,
		const struct tc_conf *const config);


/**
 * \brief Reset the TC module
 *
 * Resets the TC module. Will block while waiting for sync between
 * clock domains before performing the reset. The TC module will not
 * be accessible while the reset is being performed.
 *
 * \param dev_inst  Pointer to the device struct
 */
static inline void tc_reset(const struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments  */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module hardware instance */
	TcCount8 tc_module = dev_inst->hw_dev->COUNT8;

	if (dev_inst->resolution == TC_RESOLUTION_32BIT) {
		if (tc_module->STATUS.reg & TC_STATUS_SLAVE) {
			/* Reset the 16 bit master counter module */
		}
		else {
			/* Reset the 16 bit slave counter module. */
		}
	}
	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Reset TC module */
	tc_module.CTRLA.reg |= TC_CTRLA_SWRST;
}


/**
 * \brief Enable the TC module
 *
 * Enables the TC module. Except when the counter is enabled for
 * retrigger on even the counter will start when the counter is enabled.
 *
 * \param dev_inst  Pointer to the device struct
 */
static inline void tc_enable(const struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance*/
	TcCount8 tc_module = dev_inst->hw_dev->COUNT8;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Enable TC module */
	tc_module.CTRLA.reg |= TC_CTRLA_ENABLE;
}


/**
 * \brief Disables the TC module
 *
 * Disables the TC module. Stops the counter. This is required for
 * changing certain settings. The settings effected by this are
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
 * \param dev_inst  Pointer to the device struct
 */
static inline void tc_disable(const struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TcCount8 tc_module = dev_inst->hw_dev->COUNT8;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Disable TC module */
	tc_module.CTRLA.reg  &= ~TC_CTRLA_ENABLE;
}


uint32_t tc_get_count_value(const struct tc_dev_inst *const dev_inst);


enum status_code tc_set_count_value(
		const struct tc_dev_inst *const dev_inst,
		uint32_t count);


/**
 * \brief Stops the counter
 *
 * This function will stop the counter. When the counter is stopped the
 * value in the count register is set to 0, if the counter was
 * counting up or MAX, if the counter was counting down when stopped.
 *
 * \param dev_inst  Pointer to the device struct
 */
static inline void tc_stop_counter(const struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TcCount8 tc_module = dev_inst->hw_dev->COUNT8;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Write command to execute */
	tc_module.CTRLBSET.reg = TC_CTRLBSET_CMD_STOP;
}


/**
 * \brief Starts the counter
 *
 * This function can be used to start the counter. It will also
 * restart the counter after a stop action has been performed.
 *
 * \param dev_inst     Pointer to the devise struct.
 */
static inline void tc_start_counter(const struct tc_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_ptr);

	/* Get a pointer to the module's hardware instance*/
	TcCount8 tc_module = dev_inst->hw_dev->COUNT8;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);
	/* Make certain that there are no conflicting commands in the register */
	tc_module.CTRLBCLR.reg = TC_CTRLBCLR_CMD_NONE;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);
	/* Write command to execute */
	tc_module.CTRLBSET.reg = TC_CTRLBSET_CMD_RETRIGGER;
}


uint32_t tc_get_capture_value(
		const struct tc_dev_inst *const dev_inst,
		enum tc_compare_capture_channel_index channel_index);


enum status_code tc_set_compare_value(
		const struct tc_dev_inst *const dev_inst,
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
 * \return Status of the procedure
 * \retval STATUS_OK              The operation has been successful.
 * \retval STATUS_ERR_INVALID_ARG The resolution in the dev_inst is
 *                                out of bounds.
 */
static enum status_code tc_set_top_value(
		const struct tc_dev_inst *const dev_inst,
		uint32_t top_value)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(compare);

	Tc * const tc_module = dev_inst->hw_dev;

	_tc_wait_for_sync(dev_inst);

	switch(dev_inst->resolution) {

	case TC_RESOLUTION_8BIT:
		tc_module->COUNT8.PER.reg = (uint8_t) top_value;
		return STATUS_OK;

	case TC_RESOLUTION_16BIT:
		tc_module->COUNT16.CC[0].reg = (uint16_t) top_value;
		return STATUS_OK;

	case TC_RESOLUTION_32BIT:
		tc_module->COUNT32.CC[0].reg = top_value;
		return STATUS_OK;

	default:
		Assert(false);
		return STATUS_ERR_INVALID_ARG;
	}

	Assert(false);//This should never happen
	return STATUS_ERR_PROTOCOL;
}


/**
 * \brief Checks if there are new capture value on channel
 *
 * Checks for new data on the specified channel or if the counter has over run,
 * in which case it returns an error.
 *
 * \param *dev_inst      Pointer to the devise instance.
 * \param channel_index  Value used to select either channel 0, 1, 2 or 3.
 *
 * \return Status of the prosedure
 * \retval STATUS_VALID_DATA    New data is available to be read in the buffer.
 * \retval STATUS_NO_CHANGE     No new data in th ebuffer.
 * \retval STATUS_ERR_BAD_DATA  The counter has overflowed.
 */
static enum status_code tc_check_for_new_capture_on_channel(
		const struct tc_dev_inst *const dev_inst,
		enum tc_compare_capture_channel_index channel_index)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(channel_index);

	TcCount8 tc_module = dev_inst->hw_dev->COUNT8;

	if (tc_module.INTFLAG.reg & TC_INTFLAG_ERR) {
			tc_module.INTFLAG.reg |= TC_INTFLAG_ERR;
			return STATUS_ERR_BAD_DATA;
		}
	if(tc_module.INTFLAG.reg & TC_INTFLAG_MC(channel_index)) {
		tc_module.INTFLAG.reg |= TC_INTFLAG_MC(channel_index);
		return STATUS_VALID_DATA;
	}
	else {
		return STATUS_NO_CHANGE;
	}
}


/**
 * \brief Function checks if the splied interrupt flag is set
 *
 * Checks if the interupt flag indicated by the interupt flag variable
 * is set.
 *
 * \param *dev_inst       Pointer to the devise instance.
 * \param interrupt_flag  Enum that tels what interupt flag to check
 *
 * \return Bool value telling if the flag is set
 * \retval True   If the flag is set
 * \retval False  If the flage is not set
 */
static inline bool tc_is_interrupt_flag_set(
		const struct tc_dev_inst *const dev_inst,
		enum tc_interrupt_flag interrupt_flag)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(interrupt_flag);

	TcCount8 tc_module = dev_inst->hw_dev->COUNT8;

	if(tc_module.INTFLAG.reg & interrupt_flag) {
		return true;
	}
	else {
		return false;
	}
}


/**
 * \brief Clears interupt flag
 *
 * This function can be used to clear the interupt flag spesified by
 * the interupt_flag enum. Using the function when the flag is not set
 * has no effect.
 *
 * \param *dev_inst       Pointer to the devise instance.
 * \param interrupt_flag  Enum teling what flag to check.
 */
static inline void tc_clear_interrupt_flag(
		const struct tc_dev_inst *const dev_inst,
		enum tc_interrupt_flag interrupt_flag)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(interrupt_flag);

	TcCount8 tc_module = dev_inst->hw_dev->COUNT8;

	tc_module.INTFLAG.reg |= interrupt_flag;
}


/** @} */

#ifdef __cplusplus
}
#endif

/** @} */



/**
 * \page tc_extra_info Extra Information
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
 * This is the quick start guide list for the \ref sam0_tc_group module, with
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
 * \section tc_use_cases TC driver use cases
 * - \subpage tc_basic_use_case
 */
#endif /* TC_ALL_HEADER */
