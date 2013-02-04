/**
 * \file
 *
 * \brief SAMD20 TC Driver
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

#include "asf.h"

/**
 * \defgroup sam0_TC_group BANANORAMA Timer Counter(TC) Driver
 *
 * This is the TC driver documentation for the BANANORAMA architecture
 * devices. This driver provides an interface for configuration and
 * management of the TC module. This driver encompasses the following
 * module within the BANANORAMA devices: \li \b TC \b (Timer Counter)
 * \n
 *
 * This driver is created and meant to be used as a polled driver, as
 * such this documentation will not go into the use of interrupts with
 * regards to the TC module. It should also be noted that this is not
 * complete documentation for the TC module, but for the functionality
 * this driver can deliver.
 *
 *
 * \section module_overview TC Overview
 *
 * A TC is basically a counter with capture compare registers that can
 * be used to compare the counter value, or capture the counter
 * value. If the counter counts the pulses from a stable frequency it
 * can be used for timer operations. The TC enables the user to do the
 * following:
 *
 * \li Generate \ref pwm
 * \li \ref waveform_generation
 * \li Generate timestamps for events
 * \li Count
 * \li Perform \ref capture_operations
 * \li Perform \ref pwc
 * \li Perform frequency capture
 * \n
 *
 * \image html overview.svg "Basic overview of the TC module"
 *
 * \image latex overview.eps "Basic overview of the TC module"
 * \n
 *
 * As seen in the figure above, the TC in the BANANORAMA has
 * its own prescaler that can be used to divide the clock frequency
 * used in the counter. It is possible to configure the counters to
 * use either 8-, 16-, or 32-bit counter size. The amount of capture
 * compare registers available is dependent on what BANANORAMA device
 * is being used and in some cases the counter size. The maximum amount of
 * capture compare registers available in any BANANORAMA device is two
 * when running in 32-bit mode and four in 8-, and 16-bit modes. For
 * device specific information, see \ref differences.
 *
 * \section functional_description Functional Description
 *
 * Independent of what counter size the timer uses, it can be set up
 * in two different modes, although to some extent, one TC module can
 * be configured in both modes. These modes are capture and compare.
 * \n
 *
 * In compare mode, the counter value is compared with one or more of
 * the compare values. When the counter value coincides with the
 * compare value, this can generate an action, such as generating an
 * event or toggling a pin when used for frequency or PWM
 * generation. For more information, see \ref operations.  \n
 *
 * In capture mode the counter value is stored upon some configurable
 * event. This can be used to generate timestamps used in event
 * capture, or it can be used for frequency capture or pulse width
 * capture. For more on this, see the \ref compare_match section.
 *
 *
 * \section timer_counter_size TC Size
 *
 * It is possible to use three different counter sizes. These are 8-,
 * 16-, and 32-bits. The size of the counter determines the maximal
 * value it can manage to count to. For easy reference the table below
 * gives the max values for the different counter sizes.
 *
 * <table>
 *  <tr>
 *    <th> - </th>
 *    <th colspan="2">Max </th>
 *  </tr>
 *  <tr>
 *    <th> Counter Size </th>
 *    <th> Hexadecimal </th>
 *    <th> Decimal </th>
 *  </tr>
 *  <tr>
 *    <th> 8-bit </th>
 *    <td> 0xFF </td>
 *    <td> 255 </td>
 *  </tr>
 *  <tr>
 *    <th> 16-bit </th>
 *    <td> 0xFFFF </td>
 *    <td> 65,535 </td>
 *  </tr>
 *  <tr>
 *    <th> 32-bit </th>
 *    <td> 0xFFFFFFFF </td>
 *    <td> 4,294,967,295 </td>
 *  </tr>
 * </table>
 *
 * It should be noted that when using the counter with 16-, and 32-bit
 * counter size, Compare Capture register 0 (CC0) is used to store the
 * period value when running in PWM generation match mode.  \n
 *
 * When using 32-bit counter size, two 16-bit counters are used
 * together in cascade to realize this feature. Even numbered TC
 * modules can be configured as 32-bit counters. The odd numbered
 * counters will act as slaves to the even numbered masters. The
 * pairing is as follows:
 *
 * <table>
 *   <tr>
 *     <th> Master </th>
 *     <th> Slave </th>
 *   </tr>
 *   <tr>
 *     <td> TC0 </td>
 *     <td> TC1 </td>
 *   </tr>
 *   <tr>
 *     <td> TC2 </td>
 *     <td> TC3 </td>
 *   </tr>
 *   <tr>
 *     <td> ... </td>
 *     <td> ... </td>
 *   </tr>
 *   <tr>
 *     <td> TCn-1 </td>
 *     <td> TCn</td>
 *   </tr>
 * </table>
 *
 * \section clock_and_prescaler Clock Settings
 *
 * \subsection clock_selection Clock Selection
 *
 * The TC peripheral is clocked asynchronously to the system clock
 * by a GCLK (Generic Clock) channel. The GCLK channel connects to any
 * of the GCLK generators. The GCLK generators are configured to use
 * one of the available clock sources on the system such as internal
 * oscillator, external crystals etc. The GCLK generator provides a
 * prescaler that can be used to divide the clock source if needed.
 * Configuring the GCLK generator is done in the conf_clocks.h
 * configuration file, where all GCLK generators are configured. This
 * configuration is applied when running system_init. \n
 *
 * To connect a clock channel to the module use the \ref tc_conf
 * struct. Here, the clock for each pair of the TC modules can be
 * set. The pairing is as above. As an example, it is not possible to
 * have differing clock frequencies on TC0 and TC1. However, it is
 * possible to have different frequencies on modules not in the same
 * pair, for instance TC0 and TC2 does not have to have the same GCLK
 * input. It is possible to use the internal TC prescaler to get
 * different counting frequencies between the same modules in a pair.
 * \n
 *
 * For more on how to set up the clocks, see the \ref sam0_gclk_group
 * "GCLK documentation".
 *
 * \subsection prescaler Prescaler
 *
 * The module has its own prescaler. This prescaler only works to
 * prescale the clock used to provide clock pulses for the counter to
 * count. This means that while the counter module is running on the
 * GCLK frequency the counter value only changes on the prescaled
 * frequency.  \n
 *
 * There are two things to consider when it comes to the
 * prescaler. One is that the TC module will have to synchronize when
 * updating certain registers. This synchronization can be time
 * consuming, especially if the GCLK frequency is much lower than the
 * system clock. For this reason it can be better to use the module's
 * prescaler to reduce the clock frequency to the counter. In this way
 * synchronization should be faster. Higher frequencies will however
 * make the module consume more power. The prescaler is configured
 * with the help of the \ref tc_conf struct and the enums in
 * the \ref tc_clock_prescaler enum.  \n
 *
 * The other thing to consider with the prescaler is which reload
 * action to use. The reload action is the action performed when a
 * retrigger event occurs. Examples of a retrigger event can be when
 * the counter reaches the max value when counting up, or when an
 * event from the event system tells the counter to retrigger. The
 * reload action determines if the prescaler should be restarted, and
 * when this should happen. The counter will always be reloaded with
 * the value it is set to start counting from. The user can choose
 * between three different reload actions.
 *
 * The prescaler consists of a basic counter circuitry itself. When
 * the prescaler is used, it counts the clock cycles of the TC
 * module's GCLK. When the counter in the prescaler reaches the chosen
 * division factor value, the output from the prescaler toggles.  \n
 *
 * <table>
 *   <tr>
 *     <th> Code reference</th>
 *     <th> Description </th>
 *   </tr>
 *   <tr>
 *     <td> \ref TC_RELOAD_ACTION_GCLK </td>
 *     <td> Reload TC counter value on next GCLK cycle. Leav prescaler
 *     as it is. </td>
 *   </tr>
 *   <tr>
 *     <td> \ref TC_RELOAD_ACTION_PRESC </td>
 *     <td> Reloads TC counter value on next prescaler clock. No
 *     special action performed to set prescaler to zero. </td>
 *   </tr>
 *  <tr>
 *    <td> \ref TC_RELOAD_ACTION_RESYNC </td>
 *    <td> Reload TC counter value on next GCLK cycle. Set prescaler
 *    to zero. </td>
 *  </tr>
 * </table>
 *
 *
 * In different scenarios and applications the correct reload option
 * will differ. One example is when an external trigger for a reload
 * occurs. If the TC uses the prescaler, the counter in the prescaler
 * should not have a value between zero and the division factor. The
 * TC counter and the counter in the prescaler should both start at
 * zero. When the counter is set to retrigger when it reaches the max
 * value on the other hand, this is not the right option to use. In
 * such a case it would be better if the prescaler is left unaltered
 * when the retrigger happens and then let the counter reset on the
 * next GCLK cycle.
 *
 *
 * \section compare_match Compare Match Operations
 *
 * In compare match operation, compare capture registers are used in
 * comparison with the counter value. Upon match some action can be
 * taken.
 *
 * \subsection timer Timer
 *
 * A Timer is a simple application where compare match operations is
 * used. In timer operations one or more values in the module's
 * compare capture registers are used to specify the time when an
 * action should be taken by the microcontroller. This can be an
 * interrupt service routine, or it can be set as an event generator
 * in the event system.
 *
 * \subsection waveform_generation Waveform Generation
 *
 * Waveform generation enables the TC module to make square waves, or if
 * combined with a passive low-pass filter, analog waveforms.
 *
 * \subsubsection pwm Pulse Width Modulation (PWM)
 *
 * Pulse width modulation is a form of waveform generation and a
 * signaling technique that can be useful in many situations. Often it
 * is used to communicate a certain value to some other circuit or
 * component. Differing impedances is also a smaller problem when
 * using PWM compared to using an analog voltage value. PWM is far
 * less prone to noise.  \n
 *
 * PWM signals are generated by configuring the \ref tc_conf struct
 * using the \ref tc_wave_generation enum in the \ref tc_conf
 * struct, and using either the \ref TC_WAVE_GENERATION_MATCH_PWM mode
 * or the \ref TC_WAVE_GENERATION_NORMAL_PWM mode.
 *
 * It is necessary to set a compare value in the \ref tc_conf struct
 * or by using the tc_set_compare_value() function. This value gives
 * the pulse width together with the top value. In addition it may be
 * necessary to configure the top value using the tc_set_top_value()
 * function.
 *
 * \note The tc_set_top_value() function is only meant to be used when
 * using wave generation in match mode, and not in normal mode. Except
 * in 8-bit mode where it will always be possible to change the top
 * value. \n
 *
 * The functions tc_set_compare_value() and tc_set_top_value() can be
 * used to set the pulse width and the period, respectively. They can
 * be used while the counter is running.  \n
 *
 *
 * The figure below illustrates operations and different states of
 * the counter and its output when running the counter in PWM normal
 * mode. As can be seen, the top value is unchanged and is set to
 * max. The compare match value is changed to illustrate different
 * outcomes of this. The PWM output is here set to be ordinary output
 * as it is not inverted here.
 *
 * \image html pwm_normal_ex.svg "Example of PWM in normal mode, and different counter operations"
 *
 * \image latex pwm_normal_ex.eps "Example of PWM in normal mode, and different counter operations"
 * \n
 *
 * In the figure below, the counter is set to generate PWM in match
 * mode. The PWM output is inverted. Inversion of the wave generation
 * output can be done by specifying which channels should be inverted
 * in the \ref tc_conf struct, see \ref
 * tc_waveform_invert_output. In this example, the counter value is
 * changed once, but the compare match value is kept
 * unchanged. As can be seen, it is possible to change the top value
 * when running in PWM match mode. \n
 *
 * \image html pwm_match_ex.svg "Example of PWM in match mode, and different counter operations"
 *
 * \image latex pwm_match_ex.eps "Example of PWM in match mode, and different counter operations"
 * \n
 *
 * \subsubsection frequency_generation Frequency Generation
 *
 * Frequency generation is in many ways the same as PWM
 * generation. However, in frequency generation a toggle only occurs
 * on the output when a match on a capture channels occurs. For
 * frequency generation, the options that are available are \ref
 * TC_WAVE_GENERATION_NORMAL_FREQ mode and \ref
 * TC_WAVE_GENERATION_MATCH_FREQ mode. These configuration options can
 * be found in the \ref tc_wave_generation enum.  \n
 *
 * As in PWM generation, it is necessary to set the period and pulse width
 * values to be used. Either by using the \ref tc_conf struct before
 * initialization or by using the functions tc_set_top_value() and
 * tc_set_compare_value(). As with PWM generation, the top value can not be
 * set in any other mode than match.
 *
 * \subsection capture_operations Capture Operations
 *
 * In capture operations, any event from the event system or a pin
 * change can trigger a capture of the counter value. This captured
 * counter value can be used as a timestamp for the event, or it can
 * be used in frequency and pulse width capture.
 *
 * \subsubsection event_capture Event Capture
 *
 * Event capture is a simple use of the capture functionality. This
 * makes it possible to create timestamps for specific events. The
 * user should be aware that when performing any capture operation
 * there is a risk that the counter reaches its top value when
 * counting up, or the bottom value (zero) when counting down, before
 * the capture event takes place. This eventuality must be checked for
 * and handled should it happen.
 *
 * Before checking for a new capture, \ref TC_INTERRUPT_FLAG_OVERFLOW
 * should be checked.  This should be done to make certain that any
 * new capture value is valid. If an overflow has occurred this error
 * has to be handled. If no error has occurred, continue checking if a
 * new capture has happened. If this is the case, get the value and
 * clear the interrupt flag. In the diagram below a proposed program
 * flow is described.
 *
 * \image html state_dia_capture.svg "Diagram of capture operation"
 *
 *
 * \image latex state_dia_capture.eps "Diagram of capture operation" width = \textwidth*0.8
 *
 *
 * How to handle the buffer overflow error is up to the user. But it
 * should be mentioned that it may be necessary to clear both the
 * capture overflow flag and the capture flag. This should be done as
 * an erroneous capture value  may have been captured at this point.
 *
 * \subsubsection pwc Pulse Width Capture
 *
 * Pulse width capture mode makes it possible to measure the pulse
 * width and period of PWM signals. This mode uses two capture
 * channels of the counter. This means that the counter module used
 * for pulse width capture can not be used for any other
 * purpose. There are two modes for pulse width capture; Pulse Width
 * Period (PWP) and Period Pulse Width(PPW). In PWP mode, capture
 * channel 0 is used for storing the pulse width and capture channel 1
 * stores the period. While in PPW mode, the period is stored in
 * capture channel 0 and the pulse width is stored in capture channel
 * 1. \n
 *
 * As in the above example it is necessary to poll on interrupt flags
 * to see if a new capture has happened and check that a capture
 * overflow error has not occurred.
 *
 *
 * \subsection sleep_modes Operation in Sleep Modes
 *
 * The TC module can operate in all sleep modes. Note that to be able
 * to run in standby, the \ref run_in_standby value in the \ref tc_conf
 * struct \ref tc_conf has to be true for the TC module to be able to run in standby.
 *
 *
 * \section oneshot Oneshot
 *
 * It is possible to perform a oneshot action with the counter by
 * configuring this option in the \ref tc_conf struct. Oneshot action
 * performs one cycle of counting and then stops the counter. The
 * counter will start counting when enabled and then stop. The counter
 * can be restarted by using the start function
 * tc_start_counter(). When restarted, it will again perform a oneshot
 * operation.
 *
 * \subsection output_inversion Wave Generation Output Inversion
 *
 * The output of the wave generation can be inverted by selecting this
 * option in the \ref tc_conf struct. The figure below
 * illustrates how this affects the output.
 *
 *
 * \section differences Differences Between BANANORAMA Devices
 *
 * \subsection SAMD20
 *
 * The SAMD20 has two capture compare registers independent of the
 * counter size.
 *
 * There are 8 counter modules in the SAMD20 device, making 4 pairs of
 * TC modules that can have different GCLK_TC frequencies. It supports
 * 4 simultaneous 32-bit TC modules.
 *
 * There are 8 TC modules in the device.
 *
 * \section dependencies Dependencies
 * The TC driver has the following dependencies:
 *
 * \li \b GCLK
 * \li \b CLOCK
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

#if !defined (__DOXYGEN__)
#define TC_NEXT_TC ((uintptr_t)TC1 - (uintptr_t)TC0)
#endif

/** TODO: remove once present in device header file */
#define TC_INST_GCKL_ID  { TC0_GCLK_ID, TC1_GCLK_ID, TC2_GCLK_ID, \
		TC3_GCLK_ID, TC4_GCLK_ID, TC5_GCLK_ID, TC6_GCLK_ID, TC7_GCLK_ID }


/** TODO: remove once present in device header file */
#define TC_INST_PM_APBCMASK { PM_APBCMASK_TC0, PM_APBCMASK_TC1, \
		PM_APBCMASK_TC2, PM_APBCMASK_TC3, PM_APBCMASK_TC4, \
		PM_APBCMASK_TC5, PM_APBCMASK_TC6, PM_APBCMASK_TC7 }


/**
 * \brief Index of the compare capture channels
 *
 * This enum is used to specify which capture/compare channel to do
 * operations on.
 */
enum tc_compare_capture_channel_index {
	/** Index of compare capture channel 0 */
	TC_COMPARE_CAPTURE_CHANNEL_0,
	/** Index of compare capture channel 1 */
	TC_COMPARE_CAPTURE_CHANNEL_1,
};

/**
 * \brief TC wave generation mode enum
 *
 * This enum is used to select which mode to run the wave
 * generation in.
 *
 */
enum tc_wave_generation {
	/** Top is max, except in 8-bit counter size where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_NORMAL_FREQ         = TC_CTRLA_WAVEGEN_NFRQ,

	/** Top is CC0, except in 8-bit counter size where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_MATCH_FREQ          = TC_CTRLA_WAVEGEN_MFRQ,

	/** Top is max, except in 8-bit counter size where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_NORMAL_PWM          = TC_CTRLA_WAVEGEN_NPWM,

	/** Top is CC0, except in 8-bit counter size where it is the PER
	 * register
	 */
	TC_WAVE_GENERATION_MATCH_PWM           = TC_CTRLA_WAVEGEN_MPWM,
};

/**
 * \brief Specifies if the counter is 8-, 16-, or 32-bits.
 *
 * This enum specifies the maximum value it is possible to count to.
 */
enum tc_counter_size {
	/** The counter's max value is 0xFF, the period register is
	 * available to be used as top value.
	 */
	TC_COUNTER_SIZE_8BIT                    = TC_CTRLA_MODE_COUNT8,

	/** The counter's max value is 0xFFFF. There is no separate
	 * period register, to modify top one of the capture compare
	 * registers has to be used. This limits the amount of
	 * available channels.
	 */
	TC_COUNTER_SIZE_16BIT                   = TC_CTRLA_MODE_COUNT16,

	/** The counter's max value is 0xFFFFFFFF. There is no separate
	 * period register, to modify top one of the capture compare
	 * registers has to be used. This limits the amount of
	 * available channels.
	 */
	TC_COUNTER_SIZE_32BIT                   = TC_CTRLA_MODE_COUNT32,
};

/**
 * \brief TC Counter reload action enum
 *
 * This enum specify how the counter and prescaler should reload.
 */
enum tc_reload_action {
	/** The counter is reloaded/reset on the next GCLK and starts
	 * counting on the prescaler clock.
	 */
	TC_RELOAD_ACTION_GCLK                 = TC_CTRLA_PRESCSYNC_GCLK,

	/** The counter is reloaded/reset on the next prescaler clock
	 */
	TC_RELOAD_ACTION_PRESC                = TC_CTRLA_PRESCSYNC_PRESC,

	/** The counter is reloaded/reset on the next GCLK, and the
	 * prescaler is restarted as well.
	 */
	TC_RELOAD_ACTION_RESYNC               = TC_CTRLA_PRESCSYNC_RESYNC,
};

/**
 * \brief TC clock prescaler values
 *
 * This enum is used to choose the clock prescaler
 * configuration. The prescaler divides the clock frequency of the TC
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
 * \brief Count direction enum.
 *
 * This enum is used to set the direction to either count up or down.
 */
enum tc_count_direction {
	/** Makes the counter count up from zero */
	TC_COUNT_DIRECTION_UP,

	/** Makes the counter count down from max or a specific user
	 *  defined value between max and 3
	 */
	TC_COUNT_DIRECTION_DOWN,
};

/**
 * \brief TC channel capture enable enum
 *
 * This enum is used to enable capture on the specified channel.
 */
enum tc_capture_enable {
	/** No channels are enabled for capture */
	TC_CAPTURE_ENABLE_NONE                 = 0,
	/** Enable channel 0 for capture */
	TC_CAPTURE_ENABLE_CHANNEL_0            = TC_CTRLC_CPTEN(2),
	/** Enable channel 1 for capture */
	TC_CAPTURE_ENABLE_CHANNEL_1            = TC_CTRLC_CPTEN(1),
};

/**
 * \brief Enum for for inverting waveform output
 *
 * This enum can be used to configure inversion of the waveform output.
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
 * \brief Enum for event action
 *
 * This enum is used to setup specific event actions.
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

	/** Store period in capture register 0, pulse width in capture
	 * register 1
	 */
	TC_EVENT_ACTION_PPW               = TC_EVCTRL_EVACT_PPW,

	/** Store pulse width in capture register 0, period in capture
	 * register 1
	 */
	TC_EVENT_ACTION_PWP               = TC_EVCTRL_EVACT_PWP,
};

/**
 * \brief Enum for setting up event generation
 *
 * This enum configures event generation on respective channels
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
 * \brief Enum to be used to check interrupt flags
 *
 * This enum defines the different interrupt flags for the TC module.
 */
enum tc_interrupt_flag {
	/** Interrupt flag for channel 0 */
	TC_INTERRUPT_FLAG_CHANNEL_0 =  TC_INTFLAG_MC(0),
	/** Interrupt flag for channel 1 */
	TC_INTERRUPT_FLAG_CHANNEL_1 =  TC_INTFLAG_MC(1),
	/** Interrupt flag for generating interrupts when
	 *  synchronization is done. This is flag is meant for the
	 *  async driver. */
	TC_INTERRUPT_FLAG_READY     =  TC_INTFLAG_READY,

	/** Interrupt flag used to test for capture overflow in capture
	 *  mode
	 */
	TC_INTERRUPT_FLAG_ERROR     =  TC_INTFLAG_ERR,

	/** Interrupt flag used to check for a counter overflow in
	 *  compare mode
	 */
	TC_INTERRUPT_FLAG_OVERFLOW  =  TC_INTFLAG_OVF,
	/** Number of interrupts */
	TC_INTERRUPT_FLAG_N,
};

/**
 * \brief Config struct for 8-bit counter size */
struct tc_8bit_conf {
	/** Initial count value */
	uint8_t count;

	/** Where to count to or from depending on the direction on the
	 *  counter
	 */
	uint8_t period;
	/** Value to be used for compare match on channel 0 */
	uint8_t compare_capture_channel_0;
	/** Value to be used for compare match on channel 1 */
	uint8_t compare_capture_channel_1;
};

/**
 * \brief Config struct for 16-bit counter size */
struct tc_16bit_conf {
	/** Initial count value */
	uint16_t count;
	/** Value to be used for compare match on channel 0 */
	uint16_t compare_capture_channel_0;
	/** Value to be used for compare match on channel 1 */
	uint16_t compare_capture_channel_1;
};

/**
 * \brief Config struct for 32-bit counter size */
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
 * Configuration struct for a TC instance. This structure should be
 * initialized by the \ref tc_get_config_defaults function before being
 * modified by the user application.
 */
struct tc_conf {
	/** GCLK generator used to clock the peripheral */
	enum gclk_generator clock_source;

	/** When true the module is enabled during standby
	 */
	bool run_in_standby;
	/** Specifies either 8-, 16-, or 32-bit counter counter size */
	enum tc_counter_size counter_size;
	/** Specifies the prescaler value for GCLK_TC */
	enum tc_clock_prescaler clock_prescaler;
	/** Specifies which waveform generation mode to use */
	enum tc_wave_generation wave_generation;

	/** Specifies the reload or reset time of the counter and
	 *  prescaler resynchronization on a retrigger event for the
	 *  TC
	 */
	enum tc_reload_action reload_action;

	/** Specifies which channel(s) to invert the waveform on */
	uint8_t waveform_invert_output;

	/** Specifies which channel(s) to enable channel capture
	 *  operation on. Since all capture channels use the same
	 *  event line, only one capture channel must be enabled at a
	 *  time, when used for event capture. When PWM capture is
	 *  used this is not the case as it does not use the event
	 *  line.
	 */
	uint8_t capture_enable;

	/**  When true, oneshot will stop the TC on next HW/SW retrigger
	 *  event or overflow/underflow
	 */
	bool oneshot;
	/** Specifies the direction for the TC to count */
	enum tc_count_direction count_direction;

	/**  Specifies if the input event source is inverted, when used
	 *  in PWP, PPW event action
	 */
	bool invert_event_input;
	/** Must be set to true to enable event actions */
	bool enable_event_input;

	/** Specifies which event to trigger if an event is triggered */
	enum tc_event_action event_action;

	/** Specifies which channel(s) to generate event on when a
	 *  compare/capture occurs
	 */
	uint16_t event_generation_enable;

	/** When true, PWM output for channel 0 is enabled */
	bool channel_0_pwm_out_enabled;
	/** Specifies pin output for channel 0. See datasheet for available pins */
	uint32_t channel_0_pwm_out_pin;
	/** Specifies MUX setting for channel 0 output pin */
	uint32_t channel_0_pwm_out_mux;

	/** When true, PWM output for channel 1 is enabled */
	bool channel_1_pwm_out_enabled;
	/** Specifies pin output for channel 1. See datasheet for available pins */
	uint32_t channel_1_pwm_out_pin;
	/** Specifies MUX setting for channel 1 output pin */
	uint32_t channel_1_pwm_out_mux;

	/** This setting determines what size counter is used */
	union {
		/** Struct for 8-bit configuration */
		struct tc_8bit_conf tc_8bit_conf;
		/** Struct for 16-bit configuration */
		struct tc_16bit_conf tc_16bit_conf;
		/** Struct for 32-bit configuration */
		struct tc_32bit_conf tc_32bit_conf;
	} tc_counter_size_conf;
};

/**
 * \brief TC software device instance structure
 *
 * TC software device instance structure.
 */
struct tc_module {
	/** Pointer to the TC Hardware module */
	Tc *hw_dev;

	/** Which counter size the counter should use. This value is set
	 *  when running tc_init. Use the \ref tc_conf struct to set the
	 *  counter size. Do not alter this, it is only for internal
	 *  checks
	 */
	enum tc_counter_size counter_size;
};

/** Check if module is busy synchronizing with system clock.
 *
 * \param dev_inst  Pointer to device instance
 *
 * \retval false            Module is not busy synchronizing
 * \retval true             Module is busy synchronizing
 */
static inline bool tc_is_syncing(const struct tc_module *const dev_inst)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	return (dev_inst->hw_dev->COUNT8.STATUS.reg & TC_STATUS_SYNCBUSY);
}

#if !defined (__DOXYGEN__)
/** \internal Synchronization between clock domains
 *
 * Makes sure GCLK_TC is synchronized with sysclock,
 * Must be called before assessing certain registers in the TC.
 * Blocks while waiting
 *
 * \param dev_inst  Pointer to device instance
 */
static inline void _tc_wait_for_sync(const struct tc_module *const dev_inst)
{
	/* Sanity check arguments  */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Synchronize */
	while (tc_is_syncing(dev_inst)) {
		/* Wait for sync */
	}
}
#endif

/**
 * \name Driver Initialization and Configuration
 * @{
 */

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
 *  \li 16-bit counter size on the counter
 *  \li No prescaler
 *  \li Normal frequency wave generation
 *  \li GCLK reload action
 *  \li Don't run in standby
 *  \li No inversion of waveform output
 *  \li No capture enabled
 *  \li Count upward
 *  \li Don't perform oneshot operations
 *  \li No event input enabled
 *  \li No event action
 *  \li No event generation enabled
 *  \li No channel 0 PWM output
 *  \li No channel 1 PWM output
 *  \li Counter starts on 0
 *  \li Capture compare channel 0 set to 0
 *  \li Capture compare channel 1 set to 0
 *
 * \param[out] config  Pointer to the \ref tc_conf struct
 */
static inline void tc_get_config_defaults(struct tc_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Write default config to config struct */
	config->clock_source                 = GCLK_GENERATOR_0;
	config->counter_size                 = TC_COUNTER_SIZE_16BIT;
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

	config->channel_0_pwm_out_enabled    = false;
	config->channel_0_pwm_out_pin        = 0;
	config->channel_0_pwm_out_mux        = 0;

	config->channel_1_pwm_out_enabled    = false;
	config->channel_1_pwm_out_pin        = 0;
	config->channel_1_pwm_out_mux        = 0;

	config->tc_counter_size_conf.tc_16bit_conf.count
		= 0x0000;
	config->tc_counter_size_conf.tc_16bit_conf.compare_capture_channel_0
		= 0x0000;
	config->tc_counter_size_conf.tc_16bit_conf.compare_capture_channel_1
		= 0x0000;
}

enum status_code tc_init(
		Tc *const tc_instance,
		struct tc_module *const dev_inst,
		const struct tc_conf *const config);

/** @} */

/**
 * \name Enable/Disable/Reset
 * @{
 */

enum status_code tc_reset(const struct tc_module *const dev_inst);

/**
 * \brief Enable the TC module
 *
 * This function enables the TC module. The counter will start when
 * the counter is enabled.
 *
 * \note When the counter is enabled for retrigger on an
 * event, the counter will not start until the start function is used.
 *
 * \param[in] dev_inst  Pointer to the device struct
 */
static inline void tc_enable(const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance */
	TcCount8 *tc_instance = &(dev_inst->hw_dev->COUNT8);

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Enable TC module */
	tc_instance->CTRLA.reg |= TC_CTRLA_ENABLE;
}

/**
 * \brief Disables the TC module
 *
 * This function disables the TC module and stops the counter.
 *
 * \param[in] dev_inst  Pointer to the device struct
 */
static inline void tc_disable(const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance */
	TcCount8 *tc_instance = &(dev_inst->hw_dev->COUNT8);

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Disable TC module */
	tc_instance->CTRLA.reg  &= ~TC_CTRLA_ENABLE;
}

/** @} */

/**
 * \name Get/Set Count Value
 * @{
 */

uint32_t tc_get_count_value(const struct tc_module *const dev_inst);

enum status_code tc_set_count_value(
		const struct tc_module *const dev_inst,
		uint32_t count);

/** @} */

/**
 * \name Start/Stop Counter
 * @{
 */

/**
 * \brief Stops the counter
 *
 * This function will stop the counter. When the counter is stopped
 * the value in the count value is set to 0 if the counter was
 * counting up, or max or the top value if the counter was counting
 * down when stopped.
 *
 * \param[in] dev_inst  Pointer to the device struct
 */
static inline void tc_stop_counter(const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance */
	TcCount8 *tc_instance = &(dev_inst->hw_dev->COUNT8);

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);

	/* Write command to execute */
	tc_instance->CTRLBSET.reg = TC_CTRLBSET_CMD(2); //TC_CTRLBSET_CMD_STOP;
}

/**
 * \brief Starts the counter
 *
 * This function can be used to start the counter. It will also
 * restart the counter after a stop action has been performed.
 *
 * \param[in] dev_inst     Pointer to the device struct
 */
static inline void tc_start_counter(const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance */
	TcCount8 *tc_instance = &(dev_inst->hw_dev->COUNT8);

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);
	/* Make certain that there are no conflicting commands in the register
	 **/
	tc_instance->CTRLBCLR.reg = TC_CTRLBCLR_CMD_NONE;

	/* Synchronize */
	_tc_wait_for_sync(dev_inst);
	/* Write command to execute */
	tc_instance->CTRLBSET.reg = TC_CTRLBSET_CMD(1); //TC_CTRLBSET_CMD_RETRIGGER;
}

/** @} */

/**
 * \name Get Capture Set Compare
 * @{
 */
uint32_t tc_get_capture_value(
		const struct tc_module *const dev_inst,
		enum tc_compare_capture_channel_index channel_index);

enum status_code tc_set_compare_value(
		const struct tc_module *const dev_inst,
		uint32_t compare_value,
		enum tc_compare_capture_channel_index channel_index);

/** @} */

/**
 * \name Set Top Value
 * @{
 */

enum status_code tc_set_top_value (
		const struct tc_module *const dev_inst,
		uint32_t top_value);

/** @} */

/**
 * \name Check/Clear Interrupt Flags
 * @{
 */

/**
 * \brief Checks an interrupt flag is set
 *
 * This function checks if the interrupt flag indicated by the
 * interrupt flag parameter is set.
 *
 * \param[in] dev_inst       Pointer to the device instance
 * \param[in] interrupt_flag  Enum that tells what interrupt flag to check
 *
 * \return Bool value telling if the flag is set
 * \retval True   If the flag is set
 * \retval False  If the flag is not set
 */
static inline bool tc_is_interrupt_flag_set(
		const struct tc_module *const dev_inst,
		enum tc_interrupt_flag interrupt_flag)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(interrupt_flag);

	TcCount8 *tc_instance = &(dev_inst->hw_dev->COUNT8);

	if (tc_instance->INTFLAG.reg & interrupt_flag) {
		return true;
	} else {
		return false;
	}
}

/**
 * \brief Clears an interrupt flag
 *
 * This function can be used to clear the interrupt flag specified by
 * the interrupt_flag parameter. Using the function when the flag is not set
 * has no effect.
 *
 * \param[in] dev_inst       Pointer to the device instance
 * \param[in] interrupt_flag Enum telling what flag to check
 */
static inline void tc_clear_interrupt_flag(
		const struct tc_module *const dev_inst,
		enum tc_interrupt_flag interrupt_flag)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(interrupt_flag);

	TcCount8 *tc_instance = &(dev_inst->hw_dev->COUNT8);

	tc_instance->INTFLAG.reg |= interrupt_flag;
}

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

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
 *	<tr>
 *		<td>PWM</td>
 *		<td>Pulse Width Modulation</td>
 *	</tr>
 *	<tr>
 *		<td>PWP</td>
 *		<td>Pulse Width Period</td>
 *	</tr>
 *	<tr>
 *		<td>PPW</td>
 *		<td>Period Pulse Width</td>
 *	</tr>
 *	<tr>
 *		<td> - </td>
 *		<td> - </td>
 *	</tr>
 * </table>
 *
 * \section workarounds Workarounds implemented by driver
 *
 * Reset of 32-bit modules also reset the connected module. Both modules
 * will be reset.
 *
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
 * This is the quick start guide for the \ref sam0_tc_group module, with
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
