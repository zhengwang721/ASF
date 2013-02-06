/**
 * \file
 *
 * \brief SAMD20 TC Driver
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
#ifndef TC_H_INCLUDED
#define TC_H_INCLUDED

/**
 * \defgroup asfdoc_samd20_tc_group SAMD20 Timer/Counter Driver (TC)
 *
 * This driver for SAMD20 devices provides an interface for the configuration
 * and management of the timer modules within the device, for waveform
 * generation and timing operations.
 *
 * The following peripherals are used by this module:
 *
 *  - TC (Timer/Counter)
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_samd20_tc_prerequisites
 *  - \ref asfdoc_samd20_tc_module_overview
 *  - \ref asfdoc_samd20_tc_special_considerations
 *  - \ref asfdoc_samd20_tc_extra_info
 *  - \ref asfdoc_samd20_tc_examples
 *  - \ref asfdoc_samd20_tc_api_overview
 *
 *
 * \section asfdoc_samd20_tc_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samd20_tc_module_overview Module Overview
 *
 * The Timer/Counter (TC) module provides a set of timing and counting related
 * functionality, such as the generation of periodic waveforms, the capturing
 * of a periodic waveform's frequency/duty cycle, and software timekeeping for
 * periodic operations. TC modules can be configured to use an 8-, 16-, or
 * 32-bit counter size.
 *
 * This TC module for the SAMD20 is capable of the following functions:
 *
 * - Generation of PWM signals
 * - Generation of timestamps for events
 * - General time counting
 * - Waveform period capture
 * - Waveform frequency capture
 *
 * The diagram below shows the overview of the TC module design.
 *
 * \image html overview.svg "Basic overview of the TC module"
 *
 *
 * \subsection asfdoc_samd20_tc_module_overview_func_desc Functional Description
 * Independent of the configured counter size, each TC module can be set set up
 * in one of two different modes; capture and compare.
 *
 * In capture mode, the counter value is stored when a configurable event
 * occurs. This mode can be used to generate timestamps used in event capture,
 * or it can be used for the measurement of a periodic input signal's
 * frequency/duty cycle.
 *
 * In compare mode, the counter value is compared against one or more of the
 * configured channel compare values. When the counter value coincides with a
 * compare value an action can be taken automatically by the module, such as
 * generating an output event or toggling a pin when used for frequency or PWM
 * signal generation.
 *
 * \subsection asfdoc_samd20_tc_module_overview_tc_size Timer/Counter Size
 * Each timer module can be configured in one of three different counter
 * sizes; 8-, 16-, and 32-bits. The size of the counter determines the maximum
 * value it can count to before an overflow occurs and the count is reset back
 * to zero. The table below shows the maximum values for each of the possible
 * counter sizes.
 *
 * <table>
 *  <tr>
 *    <th>Counter Size</th>
 *    <th>Max (Hexadecimal)</th>
 *    <th>Max (Decimal)</th>
 *  </tr>
 *  <tr>
 *    <th>8-bit</th>
 *    <td>0xFF</td>
 *    <td>255</td>
 *  </tr>
 *  <tr>
 *    <th>16-bit</th>
 *    <td>0xFFFF</td>
 *    <td>65,535</td>
 *  </tr>
 *  <tr>
 *    <th>32-bit</th>
 *    <td>0xFFFFFFFF</td>
 *    <td>4,294,967,295</td>
 *  </tr>
 * </table>
 *
 * When using the counter in 16- or 32-bit count mode, Compare Capture
 * register 0 (CC0) is used to store the period value when running in PWM
 * generation match mode.
 *
 * When using 32-bit counter size, two 16-bit counters are chained together
 * in a cascade formation. Even numbered TC modules (e,g, TC0, TC2) can be
 * configured as 32-bit counters. The odd numbered counters will act as slaves
 * to the even numbered masters, and will not be reconfigurable until the
 * master timer is disabled. The pairing of timer modules for 32-bit mode is
 * shown in the table below.
 *
 * <table>
 *   <tr>
 *     <th>Master TC Module</th>
 *     <th>Slave TC Module</th>
 *   </tr>
 *   <tr>
 *     <td>TC0</td>
 *     <td>TC1</td>
 *   </tr>
 *   <tr>
 *     <td>TC2</td>
 *     <td>TC3</td>
 *   </tr>
 *   <tr>
 *     <td>...</td>
 *     <td>...</td>
 *   </tr>
 *   <tr>
 *     <td>TCn-1</td>
 *     <td>TCn</td>
 *   </tr>
 * </table>
 *
 * \subsection asfdoc_samd20_tc_module_overview_clock Clock Settings
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_clock_selection Clock Selection
 * Each TC peripheral is clocked asynchronously to the system clock by a GCLK
 * (Generic Clock) channel. The GCLK channel connects to any of the GCLK
 * generators. The GCLK generators are configured to use one of the available
 * clock sources on the system such as internal oscillator, external crystals
 * etc. - see the \ref asfdoc_samd20_gclk_group "Generic Clock Driver" for
 * more information.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_clock_prescaler Prescaler
 * Each TC module in the SAMD20 has its own individual clock prescaler, which
 * can be used to divide the input clock frequency used in the counter. This
 * prescaler only scales the clock used to provide clock pulses for the counter
 * to count, and does not affect the digital register interface portion of
 * the module, thus the timer registers will synchronized to the raw GCLK
 * frequency input to the module.
 *
 * As a result of this, when selecting a GCLK frequency and timer prescaler
 * value the user application should consider both the timer resolution
 * required and the synchronization frequency, to avoid lengthy
 * synchronization times of the module if a very slow GCLK frequency is fed
 * into the TC module. It is preferable to use a higher module GCLK frequency
 * as the input to the timer and prescale this down as much as possible to
 * obtain a suitable counter frequency in latency-sensitive applications.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_clock_reloading Reloading
 * Timer modules also contain a configurable reload action, used when a
 * re-trigger event occurs. Examples of a re-trigger event are the counter
 * reaching the max value when counting up, or when an event from the event
 * system tells the counter to re-trigger. The reload action determines if the
 * prescaler should be reset, and when this should happen. The counter will
 * always be reloaded with the value it is set to start counting from. The user
 * can choose between three different reload actions, described in the
 * following table.
 *
 * <table>
 *   <tr>
 *     <th>Reload Action</th>
 *     <th>Description</th>
 *   </tr>
 *   <tr>
 *     <td>\ref TC_RELOAD_ACTION_GCLK </td>
 *     <td>Reload TC counter value on next GCLK cycle. Leave prescaler
 *         as-is.</td>
 *   </tr>
 *   <tr>
 *     <td>\ref TC_RELOAD_ACTION_PRESC </td>
 *     <td>Reloads TC counter value on next prescaler clock. Leave prescaler
 *         as-is.</td>
 *   </tr>
 *  <tr>
 *    <td> \ref TC_RELOAD_ACTION_RESYNC </td>
 *    <td>Reload TC counter value on next GCLK cycle. Clear prescaler to
 *        zero.</td>
 *  </tr>
 * </table>
 *
 * The reload action to use will depend on the specific application being
 * implemented. One example is when an external trigger for a reload occurs; if
 * the TC uses the prescaler, the counter in the prescaler should not have a
 * value between zero and the division factor. The TC counter and the counter
 * in the prescaler should both start at zero. When the counter is set to
 * re-trigger when it reaches the max value on the other hand, this is not the
 * right option to use. In such a case it would be better if the prescaler is
 * left unaltered when the re-trigger happens, letting the counter reset on the
 * next GCLK cycle.
 *
 * \subsection asfdoc_samd20_tc_module_overview_compare_match Compare Match Operations
 * In compare match operation, Compare/Capture registers are used in comparison
 * with the counter value. When the timer's count value matches the value of a
 * compare channel, a user defined action can be taken.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_compare_match_timer Basic Timer
 *
 * A Basic Timer is a simple application where compare match operations is used
 * to determine when a specific period has elapsed. In Basic Timer operations,
 * one or more values in the module's Compare/Capture registers are used to
 * specify the time (as a number of prescaled GCLK cycles) when an action should
 * be taken by the microcontroller. This can be an Interrupt Service Routine
 * (ISR), event generator via the event system, or a software flag that is polled
 * via the user application.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_compare_match_wg Waveform Generation
 *
 * Waveform generation enables the TC module to generate square waves, or if
 * combined with an external passive low-pass filter, analog waveforms.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_compare_match_wg_pwm Waveform Generation - PWM
 *
 * Pulse width modulation is a form of waveform generation and a signaling
 * technique that can be useful in many situations. When PWM mode is used,
 * a digital pulse train with a configurable frequency and duty cycle can be
 * generated by the TC module and output to a GPIO pin of the device.
 *
 * Often PWM is used to communicate a control or information parameter to an
 * external circuit or component. Differing impedances of the source generator
 * and sink receiver circuits is less of an issue when using PWM compared to
 * using an analog voltage value, as noise will not generally affect the
 * signal's integrity to a meaningful extent.
 *
 * The figure below illustrates operations and different states of the counter
 * and its output when running the counter in PWM normal mode. As can be seen,
 * the TOP value is unchanged and is set to MAX. The compare match value is
 * changed at several points to illustrate the resulting waveform output
 * changes. The PWM output is set to normal (i.e non-inverted) output mode.
 *
 * \image html pwm_normal_ex.svg "Example of PWM in normal mode, and different counter operations"
 *
 *
 * In the figure below, the counter is set to generate PWM in Match mode. The
 * PWM output is inverted via the appropriate configuration option in the TC
 * driver configuration structure. In this example, the counter value is
 * changed once, but the compare match value is kept unchanged. As can be seen,
 * it is possible to change the TOP value when running in PWM match mode.
 *
 * \image html pwm_match_ex.svg "Example of PWM in match mode, and different counter operations"
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_compare_match_wg_freq Waveform Generation - Frequency
 *
 * Frequency Generation mode is in many ways identical to PWM generation. However,
 * in Frequency Generation a toggle only occurs on the output when a match on
 * a capture channels occurs. When the match is made, the timer value is reset,
 * resulting in a variable frequency square wave with a fixed 50% duty cycle.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_compare_match_capt Capture Operations
 *
 * In capture operations, any event from the event system or a pin change can
 * trigger a capture of the counter value. This captured counter value can be
 * used as a timestamp for the event, or it can be used in frequency and pulse
 * width capture.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_compare_match_capt_event_capture Capture Operations - Event
 *
 * Event capture is a simple use of the capture functionality, designed to create
 * timestamps for specific events. When the TC module's input capture pin is
 * externally toggled, the current timer count value is copied into a buffered
 * register which can then be read out by the user application.
 *
 * Note that when performing any capture operation, there is a risk that the
 * counter reaches its top value (MAX) when counting up, or the bottom value
 * (zero) when counting down, before the capture event occurs. This can distort
 * the result, making event timestamps to appear shorter than reality; the
 * user application should check for timer overflow when reading a capture
 * result in order to detect this situation and perform an appropriate
 * adjustment.
 *
 * Before checking for a new capture, \ref TC_INTERRUPT_FLAG_OVERFLOW
 * should be checked. A suitable program flow for capture events is shown below.
 *
 * \image html state_dia_capture.svg "Diagram of capture operation"
 *
 * How to handle the buffer overflow error is up to the user, however it may be
 * necessary to clear both the capture overflow flag and the capture flag upon
 * each capture reading.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_compare_match_capt_pwc Capture Operations - Pulse Width
 *
 * Pulse Width Capture mode makes it possible to measure the pulse width and
 * period of PWM signals. This mode uses two capture channels of the counter.
 * This means that the counter module used for Pulse Width Capture can not be
 * used for any other purpose. There are two modes for pulse width capture;
 * Pulse Width Period (PWP) and Period Pulse Width (PPW). In PWP mode, capture
 * channel 0 is used for storing the pulse width and capture channel 1 stores
 * the observed period. While in PPW mode, the roles of the two capture channels
 * is reversed.
 *
 * As in the above example it is necessary to poll on interrupt flags to see
 * if a new capture has happened and check that a capture overflow error has
 * not occurred.
 *
 * \subsection asfdoc_samd20_tc_module_overview_oneshot One-shot Mode
 *
 * TC modules can be configured into a one-shot mode. When configured in this
 * manner, starting the timer will cause it to count until the next overflow
 * or underflow condition before automatically halting, waiting to be manually
 * triggered by the user application software or an event signal from the event
 * system.
 *
 * \subsubsection asfdoc_samd20_tc_module_overview_inversion Wave Generation Output Inversion
 *
 * The output of the wave generation can be inverted by hardware if desired,
 * resulting in the logically inverted value being output to the configured
 * device GPIO pin.
 *
 *
 * \section asfdoc_samd20_tc_special_considerations Special Considerations
 *
 * The number of capture compare registers in each TC module is dependent on
 * the specific SAMD20 device being used, and in some cases the counter size.
 *
 * The maximum amount of capture compare registers available in any SAMD20
 * device is two when running in 32-bit mode and four in 8-, and 16-bit modes.
 *
 *
 * \section asfdoc_samd20_tc_extra_info Extra Information for TC
 *
 * For extra information see \ref asfdoc_samd20_tc_extra. This includes:
 *  - \ref asfdoc_samd20_tc_extra_acronyms
 *  - \ref asfdoc_samd20_tc_extra_dependencies
 *  - \ref asfdoc_samd20_tc_extra_errata
 *  - \ref asfdoc_samd20_tc_extra_history
 *
 *
 * \section asfdoc_samd20_tc_examples Examples
 *
 * The following Quick Start guides and application examples are available for this driver:
 * - \ref asfdoc_samd20_tc_basic_use_case
 *
 *
 * \section asfdoc_samd20_tc_api_overview API Overview
 * @{
 */

#include <compiler.h>
#include <clock.h>
#include <gclk.h>
#include <pinmux.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__DOXYGEN__)
#  define _TC_GCLK_ID(n, unused)     TC##n##_GCLK_ID   ,
#  define _TC_PM_APBCMASK(n, unused) PM_APBCMASK_TC##n ,

/** TODO: remove once present in device header file */
#  define TC_INST_GCLK_ID     { MREPEAT(TC_INST_NUM, _TC_GCLK_ID, ~)     }

/** TODO: remove once present in device header file */
#  define TC_INST_PM_APBCMASK { MREPEAT(TC_INST_NUM, _TC_PM_APBCMASK, ~) }
#endif

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
	TC_CAPTURE_ENABLE_CHANNEL_0            = TC_CTRLC_CPTEN(1),
	/** Enable channel 1 for capture */
	TC_CAPTURE_ENABLE_CHANNEL_1            = TC_CTRLC_CPTEN(2),
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
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_0 = TC_CTRLC_INVEN(1),
	/** Invert output from compare channel 1 */
	TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_1 = TC_CTRLC_INVEN(2),
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
	TC_EVENT_GENERATION_ENABLE_CHANNEL_0 = TC_EVCTRL_MCEO(1),
	/** Event generation on channel 1 */
	TC_EVENT_GENERATION_ENABLE_CHANNEL_1 = TC_EVCTRL_MCEO(2),
};

/**
 * \brief Enum to be used to check interrupt flags
 *
 * This enum defines the different interrupt flags for the TC module.
 */
enum tc_interrupt_flag {
	/** Interrupt flag for channel 0 */
	TC_INTERRUPT_FLAG_CHANNEL_0 =  TC_INTFLAG_MC(1),
	/** Interrupt flag for channel 1 */
	TC_INTERRUPT_FLAG_CHANNEL_1 =  TC_INTFLAG_MC(2),
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
	 *  prescaler resynchronization on a re-trigger event for the
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

	/** When true, one-shot will stop the TC on next HW/SW re-trigger
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
static inline bool tc_is_syncing(
		const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
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
static inline void _tc_wait_for_sync(
		const struct tc_module *const dev_inst)
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
static inline void tc_get_config_defaults(
		struct tc_conf *const config)
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

enum status_code tc_reset(
		const struct tc_module *const dev_inst);

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
static inline void tc_enable(
		const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance */
	TcCount8 *const tc_instance = &(dev_inst->hw_dev->COUNT8);

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
static inline void tc_disable(
		const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance */
	TcCount8 *const tc_instance = &(dev_inst->hw_dev->COUNT8);

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

uint32_t tc_get_count_value(
		const struct tc_module *const dev_inst);

enum status_code tc_set_count_value(
		const struct tc_module *const dev_inst,
		const uint32_t count);

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
static inline void tc_stop_counter(
		const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance */
	TcCount8 *const tc_instance = &(dev_inst->hw_dev->COUNT8);

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
static inline void tc_start_counter(
		const struct tc_module *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the module's hardware instance */
	TcCount8 *const tc_instance = &(dev_inst->hw_dev->COUNT8);

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
		const enum tc_compare_capture_channel_index channel_index);

enum status_code tc_set_compare_value(
		const struct tc_module *const dev_inst,
		const uint32_t compare_value,
		const enum tc_compare_capture_channel_index channel_index);

/** @} */

/**
 * \name Set Top Value
 * @{
 */

enum status_code tc_set_top_value (
		const struct tc_module *const dev_inst,
		const uint32_t top_value);

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
		const enum tc_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments */
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
		const enum tc_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(interrupt_flag);

	TcCount8 *const tc_instance = &(dev_inst->hw_dev->COUNT8);

	tc_instance->INTFLAG.reg |= interrupt_flag;
}

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

/**
 * \page asfdoc_samd20_tc_extra Extra Information for TC Driver
 *
 * \section asfdoc_samd20_tc_extra_acronyms Acronyms
 * The table below presents the acronyms used in this module:
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
 *
 * \section asfdoc_samd20_tc_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - \ref asfdoc_samd20_pinmux_group "System Pin Multiplexer Driver"
 *
 *
 * \section asfdoc_samd20_tc_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samd20_tc_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
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
 * \page asfdoc_samd20_tc_exqsg Examples for TC Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samd20_tc_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_samd20_tc_basic_use_case
 */

#endif /* TC_H_INCLUDED */
