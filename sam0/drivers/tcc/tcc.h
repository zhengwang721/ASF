/**
 * \file
 *
 * \brief SAM D2x TCC - Enhanced Timer Counter Driver
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#ifndef TCC_H_INCLUDED
#define TCC_H_INCLUDED

/**
 * \defgroup asfdoc_sam0_tcc_group SAM D2x Enhanced Timer/Counter Driver (TCC)
 */

#include <compiler.h>
#include <clock.h>
#include <gclk.h>
#include <pinmux.h>

/** Max number of channels supported by the driver
 *  (Channel index from 0 to \c TCC_NUM_CHANNELS - 1).
 */
#define TCC_NUM_CHANNELS           4

/** Max number of wave outputs lines supported by the driver
 *  (Output line index from 0 to \c TCC_NUM_WAVE_OUTPUTS - 1).
 */
#define TCC_NUM_WAVE_OUTPUTS       8

/** Max number of faults supported by the driver. */
#define TCC_NUM_FAULTS             2

#if TCC_ASYNC == true
#  include <system_interrupt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Generates a interrupt vector table enum list entry for a given type
   and index (e.g. "TCC_CALLBACK_MC_CHANNEL0,"). */
#define _TCC_CHANNEL_ENUM(n, type) TCC_##type##_CHANNEL_##n,

/* Generates callback table enum list entries for all channels of a
   given type and channel number on TCC module. */
#define _TCC_CHANNEL_ENUM_LIST(type) \
		MREPEAT(TCC_NUM_CHANNELS, _TCC_CHANNEL_ENUM, type)

#if TCC_ASYNC == true
/** Enum for the possible callback types for the TCC module. */
enum tcc_callback {
	/** Callback for TCC overflow. */
	TCC_CALLBACK_OVERFLOW,
	/** Callback for TCC Retrigger. */
	TCC_CALLBACK_RETRIGGER,
	/** Callback for TCC counter event. */
	TCC_CALLBACK_COUNTER_EVENT,
	/** Callback for capture overflow error. */
	TCC_CALLBACK_ERROR,
	/** Callback for Recoverable Fault A. */
	TCC_CALLBACK_FAULTA,
	/** Callback for Recoverable Fault B. */
	TCC_CALLBACK_FAULTB,
	/** Callback for Non-Recoverable Fault. 0 */
	TCC_CALLBACK_FAULT0,
	/** Callback for Non-Recoverable Fault. 1 */
	TCC_CALLBACK_FAULT1,

	/** Callbacks for Match/Capture channels, e.g., TCC_CALLBACK_CHANNEL_0. */
	_TCC_CHANNEL_ENUM_LIST(CALLBACK)

#  if !defined(__DOXYGEN__)
	/** Number of available callbacks. */
	TCC_CALLBACK_N
#  endif
};
#endif /* #if TCC_ASYNC == true */

/**
 * \name Module status flags
 *
 * TCC status flags, returned by \ref tcc_get_status() and cleared by
 * \ref tcc_clear_status().
 *
 * @{
 */

/** Timer channel \c ch (0 ~ 7) has matched against its compare value,
 * or has captured a new value.
 */
#define TCC_STATUS_CHANNEL_MATCH_CAPTURE(ch)        (1UL << (ch))
/** Timer channel \c ch (0 ~ 7) match/compare output value. */
#define TCC_STATUS_CHANNEL_OUTPUT(ch)               (1UL << ((ch)+8))
/** A Non-Recoverable Fault \c x (0 ~ 1) has occured */
#define TCC_STATUS_NON_RECOVERABLE_FAULT_OCCUR(x)   (1UL << ((x)+16))
/** A Recoverable Fault \c n (0 ~ 1 representing A ~ B) has occured */
#define TCC_STATUS_RECOVERABLE_FAULT_OCCUR(n)       (1UL << ((n)+18))
/** The Non-Recoverable Fault \c x (0 ~ 1) input is present */
#define TCC_STATUS_NON_RECOVERABLE_FAULT_PRESENT(x) (1UL << ((x)+20))
/** A Recoverable Fault \c n (0 ~ 1 representing A ~ B) is present */
#define TCC_STATUS_RECOVERABLE_FAULT_PRESENT(n)     (1UL << ((n)+22))
/** Timer registers synchronization has completed, and the synchronized count
 *  value may be read.
 */
#define TCC_STATUS_SYNC_READY                       (1UL << 23)
/** A new value was captured before the previous value was read, resulting in
 *  lost data.
 */
#define TCC_STATUS_CAPTURE_OVERFLOW                 (1UL << 24)
/** A counter event occurs */
#define TCC_STATUS_COUNTER_EVENT                    (1UL << 25)
/** A counter retrigger occurs */
#define TCC_STATUS_COUNTER_RETRIGGERED              (1UL << 26)
/** The timer count value has overflowed from its maximum value to its minimum
 *  when counting upward, or from its minimum value to its maximum when
 *  counting downward.
 */
#define TCC_STATUS_COUNT_OVERFLOW                   (1UL << 27)
/** Ramp period cycle index.
 *  In ramp operation, each two period cycles are marked as cycle A and B,
 *  the index 0 represents cycle A and 1 represents cycle B. */
#define TCC_STATUS_RAMP_CYCLE_INDEX                 (1UL << 28)
/** The counter has been stopped (due to disable, stop command or one-shot) */
#define TCC_STATUS_STOPPED                          (1UL << 29)

/** @} */

/**
 * \brief Index of the match capture channels
 *
 * This enum is used to specify which capture/match channel to do
 * operations on.
 */
enum tcc_match_capture_channel {

	/** Indexes of match capture channels, e.g., TCC_MATCH_CAPTURE_CHANNEL_0. */
	_TCC_CHANNEL_ENUM_LIST(MATCH_CAPTURE)

#  if !defined(__DOXYGEN__)
	/** Number of supported channels */
	TCC_MATCH_CAPTURE_CHANNEL_N
#  endif
};

/**
 * \brief TCC wave generation mode enum
 *
 * This enum is used to select which mode to run the wave
 * generation in.
 *
 */
enum tcc_wave_generation {
	/** Normal Frequency: Top is the PER register, output toggled on each
	 *  compare match. */
	TCC_WAVE_GENERATION_NORMAL_FREQ = 0,
	/** Match Frequency: Top is CC0 register, output toggles on each update
	 *  condition. */
	TCC_WAVE_GENERATION_MATCH_FREQ = 1,
	/** Single-Slope PWM: Top is the PER register, CCx controls duty cycle (
	 *  output active when count is greater than CCx). */
	TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM = 2,
	/** Double-slope (count up and down), critical: Top is the PER register,
	 *  CC[x] for counting up and CC[x+N/2] for counting down. */
	TCC_WAVE_GENERATION_DOUBLE_SLOPE_CRITICAL = 4,
	/** Double-slope (count up and down), interrupt/event at Bottom (Top is the
	 *  PER register, output active when count is greater than CCx). */
	TCC_WAVE_GENERATION_DOUBLE_SLOPE_BOTTOM = 5,
	/** Double-slope (count up and down), interrupt/event at Bottom and Top: (Top is the
	 *  PER register, output active when count is lower than CCx). */
	TCC_WAVE_GENERATION_DOUBLE_SLOPE_BOTH = 6,
	/** Double-slope (count up and down), interrupt/event at Top (Top is the
	 *  PER register, output active when count is greater than CCx). */
	TCC_WAVE_GENERATION_DOUBLE_SLOPE_TOP = 7,
};

/**
 * \brief Polarity of TCC wave generation on channels
 */
enum tcc_wave_polarity {
	/** Wave output initialized to ~DIR */
	TCC_WAVE_POLARITY_0,
	/** Wave output initialized to DIR */
	TCC_WAVE_POLARITY_1
};

/**
 * \brief TCC pattern generator for outputs
 */
enum tcc_output_pattern {
	/** SWAP Output pattern is not used */
	TCC_OUTPUT_PATTERN_DISABLE,
	/** Pattern 0 is applied to SWAP output */
	TCC_OUTPUT_PATTERN_0,
	/** Pattern 1 is applied to SWAP output */
	TCC_OUTPUT_PATTERN_1
};

/**
 * \brief Ramp Operations of TCC wave generation
 *  In ramp operation, each two period cycles are marked as cycle A and B,
 *  the index 0 represents cycle A and 1 represents cycle B.
 */
enum tcc_ramp {
	/** Default timer/counter PWM operation. */
	TCC_RAMP_RAMP1 = 0,
	/** CC0 compare outputs are for channel 1 and 0.
	 *  Two consecutive cycles A and B are for channel 1 and 0. */
	TCC_RAMP_RAMP2A,
	/** CC0 and CC1 compare outputs are involved.
	 *  Two consecutive cycles A and B are for channel 1 and 0. */
	TCC_RAMP_RAMP2
};

/**
 * \brief Ramp Index of TCC wave generation
 */
enum tcc_ramp_index {
	/** Default, cycle index toggles. */
	TCC_RAMP_INDEX_DEFAULT,
	/** Force next cycle to be cycle B (set to 1). */
	TCC_RAMP_INDEX_FORCE_B,
	/** Force next cycle to be cycle A (clear to 0). */
	TCC_RAMP_INDEX_FORCE_A,
	/** Force next cycle keeping the same as current. */
	TCC_RAMP_INDEX_FORCE_KEEP
};

/**
 * \brief TCC output invertion
 */
enum tcc_output_invertion {
	/** output of WO[x] is disabled */
	TCC_OUTPUT_INVERTION_DISABLE,
	/** output of WO[x] is enabled */
	TCC_OUTPUT_INVERTION_ENABLE
};

/**
 * \brief TCC Counter reload action enum
 *
 * This enum specify how the counter and prescaler should reload.
 */
enum tcc_reload_action {
	/** The counter is reloaded/reset on the next GCLK and starts
	 * counting on the prescaler clock.
	 */
	TCC_RELOAD_ACTION_GCLK,
	/** The counter is reloaded/reset on the next prescaler clock
	 */
	TCC_RELOAD_ACTION_PRESC,
	/** The counter is reloaded/reset on the next GCLK, and the
	 * prescaler is restarted as well.
	 */
	TCC_RELOAD_ACTION_RESYNC
};


/**
 * \brief TCC clock prescaler values
 *
 * This enum is used to choose the clock prescaler
 * configuration. The prescaler divides the clock frequency of the TCC
 * module to make the counter count slower.
 */
enum tcc_clock_prescaler {
	/** Divide clock by 1 */
	TCC_CLOCK_PRESCALER_DIV1,
	/** Divide clock by 2 */
	TCC_CLOCK_PRESCALER_DIV2,
	/** Divide clock by 4 */
	TCC_CLOCK_PRESCALER_DIV4,
	/** Divide clock by 8 */
	TCC_CLOCK_PRESCALER_DIV8,
	/** Divide clock by 16 */
	TCC_CLOCK_PRESCALER_DIV16,
	/** Divide clock by 64 */
	TCC_CLOCK_PRESCALER_DIV64,
	/** Divide clock by 256 */
	TCC_CLOCK_PRESCALER_DIV256,
	/** Divide clock by 1024 */
	TCC_CLOCK_PRESCALER_DIV1024
};

/**
 * \brief TCC module count direction.
 *
 * Timer/Counter count direction.
 */
enum tcc_count_direction {
	/** Timer should count upward from zero to MAX. */
	TCC_COUNT_DIRECTION_UP,
	/** Timer should count downward to zero from MAX. */
	TCC_COUNT_DIRECTION_DOWN,
};

/**
 * \brief Action to perform when the TCC module is triggered by events.
 *
 * Event action to perform when the module is triggered by events.
 */
enum tcc_event_action {
	/** No event action. */
	TCC_EVENT_ACTION_OFF,
	/** Stop counting, the counter will maintain its current value, waveforms
	 *  are set to a defined Non-Recoverable State output
	 *  (\ref tcc_non_recoverable_state_output). */
	TCC_EVENT_ACTION_STOP,
	/** Re-trigger counter on event, may generate an event if the re-trigger
	 *  event output is enabled.
	 *  \note When re-trigger event action is enabled, enabling the counter
	 *        will not start the counter but waiting incoming event. */
	TCC_EVENT_ACTION_RETRIGGER,
	/** Start counter when previously stopped. */
	TCC_EVENT_ACTION_START,
	/** Count events (increment or decrement, dependint on DIR). */
	TCC_EVENT_ACTION_COUNT_EVENT,
	/** The event source must be an asynchronous event, input value will
	 *  overrides the direction settings (input low: counting up, input high
	 *  counting down). */
	TCC_EVENT_ACTION_DIR_CONTROL,
	/** Increment or decrement counter on event, depending on direction. */
	TCC_EVENT_ACTION_INCREMENT,
	/** Decrement or increment counter on event, depending on direction. */
	TCC_EVENT_ACTION_DECREMENT,
	/** Count during active state of asynchronous event. */
	TCC_EVENT_ACTION_COUNT_DURING_ACTIVE,

	/** Store period in capture register 0, pulse width in capture
	 *  register 1.
	 */
	TCC_EVENT_ACTION_PULSE_PULSE_WIDTH_CAPTURE,
	/** Store pulse width in capture register 0, period in capture
	 *  register 1.
	 */
	TCC_EVENT_ACTION_PULSE_WIDTH_PULSE_CAPTURE,

	/** Generate Non-Recoverable Fault on event. */
	TCC_EVENT_ACTION_NON_RECOVERABLE_FAULT,
};


/**
 * \brief Action to perform when the TCC module is triggered by event0.
 *
 * Event action to perform when the module is triggered by event0.
 */
enum tcc_event0_action {
	/** No event action. */
	TCC_EVENT0_ACTION_OFF                   = TCC_EVENT_ACTION_OFF,
	/** Re-trigger Counter on event. */
	TCC_EVENT0_ACTION_RETRIGGER             = TCC_EVENT_ACTION_RETRIGGER,
	/** Count events (increment or decrement, dependint on DIR). */
	TCC_EVENT0_ACTION_COUNT_EVENT           = TCC_EVENT_ACTION_COUNT_EVENT,
	/** Start counter on event. */
	TCC_EVENT0_ACTION_START                 = TCC_EVENT_ACTION_START,
	/** Increment counter on event. */
	TCC_EVENT0_ACTION_INCREMENT             = TCC_EVENT_ACTION_INCREMENT,
	/** Count during active state of asynchronous event. */
	TCC_EVENT0_ACTION_COUNT_DURING_ACTIVE   = TCC_EVENT_ACTION_COUNT_DURING_ACTIVE,

	/** Generate Non-Recoverable Fault on event. */
	TCC_EVENT0_ACTION_NON_RECOVERABLE_FAULT = TCC_EVENT_ACTION_NON_RECOVERABLE_FAULT
};

/**
 * \brief Action to perform when the TCC module is triggered by event1.
 *
 * Event action to perform when the module is triggered by event1.
 */
enum tcc_event1_action {
	/** No event action. */
	TCC_EVENT1_ACTION_OFF                   = TCC_EVENT_ACTION_OFF,
	/** Re-trigger Counter on event. */
	TCC_EVENT1_ACTION_RETRIGGER             = TCC_EVENT_ACTION_RETRIGGER,
	/** The event source must be an asynchronous event, input value will
	 *  overrides the direction settings (input low: counting up, input high
	 *  counting down). */
	TCC_EVENT1_ACTION_DIR_CONTROL           = TCC_EVENT_ACTION_DIR_CONTROL,
	/** Stop counter on event. */
	TCC_EVENT1_ACTION_STOP                  = TCC_EVENT_ACTION_STOP,
	/** Decrement on event */
	TCC_EVENT1_ACTION_DECREMENT             = TCC_EVENT_ACTION_DECREMENT,

	/** Store period in capture register 0, pulse width in capture
	 *  register 1.
	 */
	TCC_EVENT1_ACTION_PULSE_PULSE_WIDTH_CAPTURE  = TCC_EVENT_ACTION_PULSE_PULSE_WIDTH_CAPTURE,
	/** Store pulse width in capture register 0, period in capture
	 *  register 1.
	 */
	TCC_EVENT1_ACTION_PULSE_WIDTH_PULSE_CAPTURE  = TCC_EVENT_ACTION_PULSE_WIDTH_PULSE_CAPTURE,

	/** Generate Non-Recoverable Fault on event. */
	TCC_EVENT1_ACTION_NON_RECOVERABLE_FAULT = TCC_EVENT_ACTION_NON_RECOVERABLE_FAULT
};

/**
 * \brief On which part of the counter cycle the counter event output is generated
 */
enum tcc_event_generation_selection {
	/** Counter Event is generated when a new counter cycle starts */
	TCC_EVENT_GENERATION_SELECTION_START,
	/** Counter Event is generated when a counter cycle ends */
	TCC_EVENT_GENERATION_SELECTION_END,
	/** Counter Event is generated when a counter cycle ends, except for the
	 *  first and last cycles */
	TCC_EVENT_GENERATION_SELECTION_BETWEEN,
	/** Counter Event is generated when a new counter cycle starts or ends */
	TCC_EVENT_GENERATION_SELECTION_BOUNDARY
};

/**
 * \brief TCC channel operation modes
 */
enum tcc_channel_function {
	/** TCC channel performs compare operation. */
	TCC_CHANNEL_FUNCTION_COMPARE,
	/** TCC channel performs capture operation. */
	TCC_CHANNEL_FUNCTION_CAPTURE
};

/**
 * \brief TCC input event enable/disable/configure structure.
 */
struct tcc_input_event_config {
	/** Event action on incoming event. */
	enum tcc_event_action action;
	/** Modify event action */
	bool modify_action;
	/** Invert incoming event input line. */
	bool invert;
};

/**
 * \brief TCC output event enable/disable/configure structure.
 */
struct tcc_output_event_config {
	/** Event output action for counter event generation. */
	enum tcc_event_generation_selection generation_selection;
	/** Modify output action */
	bool modify_generation_selection;
};

/**
 * \brief TCC event enable/disable structure.
 *
 * Event flags for the \ref tcc_enable_events() and \ref tcc_disable_events().
 */
struct tcc_events {
	/** Input events configuration */
	struct tcc_input_event_config input_config[2];
	/** Output event configuration */
	struct tcc_output_event_config output_config;

	/** Perform the configured event action when an incoming event is
	 *  signalled. */
	bool on_input_event_perform_action[2];

	/** Perform the configured event action when an incoming channel event is
	 *  signalled */
	bool on_event_perform_channel_action[TCC_NUM_CHANNELS];

	/** Generate an output event on a channel capture/match. */
	bool generate_event_on_channel[TCC_NUM_CHANNELS];

	/** Generate an output event on counter overflow/underflow. */
	bool generate_event_on_counter_overflow;
	/** Generate an output event on counter retrigger */
	bool generate_event_on_counter_retrigger;
	/** Generate an output event on counter boundary.
	 *  See \ref tcc_event_output_action */
	bool generate_event_on_counter_event;
};

/**
 * \brief Configuration struct for TCC module base counter
 */
struct tcc_counter_config {
	/** Value to initialize the count register */
	uint32_t count;
	/** Period/top and period/top buffer values for counter */
	uint32_t period;

	/** When \c true, one-shot will stop the TC on next hardware or software
	 *  re-trigger event or overflow/underflow.
	 */
	bool oneshot;

	/** Specifies the direction for the TC to count. */
	enum tcc_count_direction direction;

	/** GCLK generator used to clock the peripheral. */
	enum gclk_generator clock_source;
	/** Specifies the prescaler value for GCLK_TCC. */
	enum tcc_clock_prescaler clock_prescaler;
	/** Specifies the reload or reset time of the counter and prescaler
	 *  resynchronization on a re-trigger event for the TCC.
	 */
	enum tcc_reload_action reload_action;
};

/**
 * \brief Configuration struct for TCC module capture
 */
struct tcc_capture_config {
	/** Channel functions selection (capture/match) */
	enum tcc_channel_function channel_function[TCC_NUM_CHANNELS];
};

/**
 * \brief Configuration struct for TCC module match/wave generation
 */
struct tcc_match_wave_config {
	/** Channel functions selection (capture/match) */
	enum tcc_channel_function channel_function[TCC_NUM_CHANNELS];

	/** Specifies polarity for match output waveform generation. */
	enum tcc_wave_polarity wave_polarity[TCC_NUM_CHANNELS];
	/** Specifies which waveform generation mode to use. */
	enum tcc_wave_generation wave_generation;
	/** Specifies Ramp mode for waveform generation. */
	enum tcc_ramp wave_ramp;

	/** Value to be used for compare match on each channel. */
	uint32_t match[TCC_NUM_CHANNELS];
};

/**
 * \brief Configuration struct for TCC module waveform extension processes
 */
struct tcc_wave_extension_config {
	/** Invert waveform final outputs lines. */
	bool invert[TCC_NUM_WAVE_OUTPUTS];
};

/**
 * \brief Configuration struct for TCC module input/output pins
 */
struct tcc_pins_config {
	/** Specifies pin output for each channel. */
	uint32_t wave_out_pin[TCC_NUM_WAVE_OUTPUTS];
	/** Specifies MUX setting for each output channel pin. */
	uint32_t wave_out_pin_mux[TCC_NUM_WAVE_OUTPUTS];
	/** When \c true, PWM output pin for the given channel is enabled. */
	bool enable_wave_out_pin[TCC_NUM_WAVE_OUTPUTS];
};

/**
 * \brief TCC configuration structure.
 *
 * Configuration struct for a TCC instance. This structure should be
 * initialized by the \ref tcc_get_config_defaults function before being
 * modified by the user application.
 */
struct tcc_config {

	/** TCC base timer/counter configurations. */
	struct tcc_counter_config counter;

	/** TCC match/capture configurations. */
	union {
		struct tcc_capture_config capture;
		struct tcc_match_wave_config compare;
		struct tcc_match_wave_config wave;
	};

	/** TCC waveform extension configurations. */
	struct tcc_wave_extension_config wave_ext;

	/** TCC output pins configuration. */
	struct tcc_pins_config pins;

	/** When \c true the module is enabled during standby. */
	bool run_in_standby;
};

#if TCC_ASYNC == true
/* Forward Declaration for the device instance */
struct tcc_module;

/* Type of the callback functions */
typedef void (*tcc_callback_t)(struct tcc_module *const module);
#endif

/**
 * \brief TCC software device instance structure.
 *
 * TCC software instance structure, used to retain software state information
 * of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct tcc_module {
#if !defined(__DOXYGEN__)
	/** Hardware module pointer of the associated Timer/Counter peripheral. */
	Tcc *hw;

#  if TCC_ASYNC == true
	/** Array of callbacks */
	tcc_callback_t callback[TCC_CALLBACK_N];
	/** Bit mask for callbacks registered */
	uint32_t register_callback_mask;
	/** Bit mask for callbacks enabled */
	uint32_t enable_callback_mask;
#  endif

#endif /* !defined(__DOXYGEN__) */
};

#if !defined(__DOXYGEN__)
uint8_t _tcc_get_inst_index(
		Tcc *const hw);
#endif

/**
 * \name Driver Initialization and Configuration
 * @{
 */

/**
 * \brief Determines if the hardware module(s) are currently synchronizing to the bus.
 *
 * Checks to see if the underlying hardware peripheral module(s) are currently
 * synchronizing across multiple clock domains to the hardware bus, This
 * function can be used to delay further operations on a module until such time
 * that it is ready, to prevent blocking delays for synchronization in the
 * user application.
 *
 * \param[in]  module_inst   Pointer to the software module instance struct
 *
 * \return Synchronization status of the underlying hardware module(s).
 *
 * \retval true if the module has completed synchronization
 * \retval false if the module synchronization is ongoing
 */
static inline bool tcc_is_syncing(
		const struct tcc_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	return (module_inst->hw->SYNCBUSY.reg > 0);
}


void tcc_get_config_defaults(
		struct tcc_config *const config,
		Tcc *const hw);

enum status_code tcc_init(
		struct tcc_module *const module_inst,
		Tcc *const hw,
		const struct tcc_config *const config);

/** @} */

/**
 * \name Event Management
 * @{
 */

enum status_code tcc_enable_events(
		struct tcc_module *const module_inst,
		struct tcc_events *const events);

void tcc_disable_events(
		struct tcc_module *const module_inst,
		struct tcc_events *const events);

/** @} */

/**
 * \name Enable/Disable/Reset
 * @{
 */

/**
 * \brief Enable the TCC module.
 *
 * Enables a TCC module that has been previously initialized. The counter will
 * start when the counter is enabled.
 *
 * \note When the counter is configured to re-trigger on an event, the counter
 *       will not start until the start function is used.
 *
 * \param[in]  module_inst   Pointer to the software module instance struct
 */
static inline void tcc_enable(
		const struct tcc_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Get a pointer to the module's hardware instance */
	Tcc *const tcc_module = module_inst->hw;

	while (tcc_module->SYNCBUSY.reg & TCC_SYNCBUSY_ENABLE) {
		/* Wait for sync */
	}

	/* Enable TCC module */
	tcc_module->CTRLA.reg |= TCC_CTRLA_ENABLE;
}

/**
 * \brief Disables the TCC module.
 *
 * Disables a TCC module and stops the counter.
 *
 * \param[in]  module_inst   Pointer to the software module instance struct
 */
static inline void tcc_disable(
		const struct tcc_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Get a pointer to the module's hardware instance */
	Tcc *const tcc_module = module_inst->hw;

	while (tcc_module->SYNCBUSY.reg & TCC_SYNCBUSY_ENABLE) {
		/* Wait for sync */
	}

	/* Disable TCC module */
	tcc_module->CTRLA.reg  &= ~TC_CTRLA_ENABLE;
}

/**
 * \brief Resets the TCC module.
 *
 * Resets the TCC module, restoring all hardware module registers to their
 * default values and disabling the module. The TCC module will not be
 * accessible while the reset is being performed.
 *
 * \note When resetting a 32-bit counter only the master TCC module's instance
 *       structure should be passed to the function.
 *
 * \param[in]  module_inst    Pointer to the software module instance struct
 *
 */
static inline void tcc_reset(
		const struct tcc_module *const module_inst)
{
	/* Sanity check arguments  */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Get a pointer to the module hardware instance */
	Tcc *const tcc_module = module_inst->hw;

	/* Disable this module if it is running */
	if (tcc_module->CTRLA.reg & TCC_CTRLA_ENABLE) {
		tcc_disable(module_inst);
		while (tcc_is_syncing(module_inst)) {
			/* wait while module is disabling */
		}
	}

	/* Reset this TC module */
	tcc_module->CTRLA.reg  |= TCC_CTRLA_SWRST;
}

/** @} */


/**
 * \name Set/Toggle Count Direction
 * @{
 */

/**
 * \brief Sets TCC module count direction.
 *
 * Sets the current timer count direction of a initialized TCC module. The
 * specified TCC module may be started or stopped.
 *
 * \param[in] module_inst  Pointer to the software module instance struct
 * \param[in] dir          New timer count direction to set
 */
static inline void tcc_set_count_direction(
		const struct tcc_module *const module_inst,
		enum tcc_count_direction dir)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Get a pointer to the module's hardware instance */
	Tcc *const tcc_module = module_inst->hw;

	while (tcc_module->SYNCBUSY.bit.CTRLB) {
		/* Wait for sync */
	}

	/* Set count direction */
	if (TCC_COUNT_DIRECTION_DOWN == dir) {
		tcc_module->CTRLBSET.reg = TCC_CTRLBSET_DIR;
		return;
	}
	tcc_module->CTRLBCLR.reg = TCC_CTRLBCLR_DIR;
}

/**
 * \brief Toggles TCC module count direction.
 *
 * Toggles the current timer count direction of a initialized TCC module. The
 * specified TCC module may be started or stopped.
 *
 * \param[in] module_inst  Pointer to the software module instance struct
 */
static inline void tcc_toggle_count_direction(
		const struct tcc_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Get a pointer to the module's hardware instance */
	Tcc *const tcc_module = module_inst->hw;

	while (tcc_module->SYNCBUSY.bit.CTRLB) {
		/* Wait for sync */
	}
	bool dir_value_1 = tcc_module->CTRLBSET.bit.DIR;
	if (dir_value_1) {
		tcc_module->CTRLBCLR.reg = TCC_CTRLBCLR_DIR;
	} else {
		tcc_module->CTRLBSET.reg = TCC_CTRLBSET_DIR;
	}
}

/** @} */

/**
 * \name Get/Set Count Value
 * @{
 */

uint32_t tcc_get_count_value(
		const struct tcc_module *const module_inst);

enum status_code tcc_set_count_value(
		const struct tcc_module *const module_inst,
		const uint32_t count);

/** @} */

/**
 * \name Start/Stop Counter
 * @{
 */

/**
 * \brief Stops the counter.
 *
 * This function will stop the counter. When the counter is stopped
 * the value in the count value is set to 0 if the counter was
 * counting up, or max or the top value if the counter was counting
 * down when stopped.
 *
 * \param[in]  module_inst   Pointer to the software module instance struct
 */
static inline void tcc_stop_counter(
		const struct tcc_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Get a pointer to the module's hardware instance */
	Tcc *const tcc_module = module_inst->hw;
	uint32_t last_cmd;

	/* Wait last command done */
	do {
		while (tcc_module->SYNCBUSY.bit.CTRLB) {
			/* Wait for sync */
		}
		last_cmd = tcc_module->CTRLBSET.reg & TCC_CTRLBSET_CMD_Msk;
		if (TCC_CTRLBSET_CMD_NONE == last_cmd) {
			break;
		} else if (TCC_CTRLBSET_CMD_STOP == last_cmd) {
			/* Command have been issued */
			return;
		} else if (TCC_CTRLBSET_CMD_RETRIGGER == last_cmd) {
			/* Cancel RETRIGGER command and issue STOP */
			tcc_module->CTRLBCLR.reg = TCC_CTRLBCLR_CMD_Msk;
		}
	} while (1);

	/* Write command to execute */
	tcc_module->CTRLBSET.reg = TCC_CTRLBSET_CMD_STOP;
}

/**
 * \brief Starts the counter from beginning.
 *
 * Restarts an initialized TC module's counter.
 *
 * \param[in]  module_inst   Pointer to the software module instance struct
 */
static inline void tcc_restart_counter(
		const struct tcc_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Get a pointer to the module's hardware instance */
	Tcc *const tcc_module = module_inst->hw;
	uint32_t last_cmd;

	/* Wait last command done */
	do {
		while (tcc_module->SYNCBUSY.bit.CTRLB) {
			/* Wait for sync */
		}
		last_cmd = tcc_module->CTRLBSET.reg & TCC_CTRLBSET_CMD_Msk;
		if (TCC_CTRLBSET_CMD_NONE == last_cmd) {
			break;
		} else if (TCC_CTRLBSET_CMD_RETRIGGER == last_cmd) {
			/* Command have been issued */
			return;
		} else if (TCC_CTRLBSET_CMD_STOP == last_cmd) {
			/* Cancel STOP command and issue RETRIGGER */
			tcc_module->CTRLBCLR.reg = TCC_CTRLBCLR_CMD_Msk;
		}
	} while (1);

	/* Write command to execute */
	tcc_module->CTRLBSET.reg = TCC_CTRLBSET_CMD_RETRIGGER;
}

/** @} */

/**
 * \name Get Capture Set Compare
 * @{
 */

uint32_t tcc_get_capture_value(
		const struct tcc_module *const module_inst,
		const enum tcc_match_capture_channel channel_index);

enum status_code tcc_set_compare_value(
		const struct tcc_module *const module_inst,
		const enum tcc_match_capture_channel channel_index,
		const uint32_t compare);

/** @} */

/**
 * \name Set Top Value
 * @{
 */

/**
 * \brief Set the timer TOP/PERIOD value.
 *
 * This function writes the top value.
 *
 * When using MFRQ, the top value is defined by the CC0 register value, for all
 * other waveforms operation the top value is defined by PER register value.
 *
 * \param[in]  module_inst   Pointer to the software module instance struct
 * \param[in]  top_value     New timer TOP value to set
 *
 * \return Status of the TOP set procedure.
 *
 * \retval STATUS_OK              The timer TOP value was updated successfully
 * \retval STATUS_ERR_INVALID_ARG The configured TC module counter size in the
 *                                module instance is invalid.
 */
enum status_code tcc_set_top_value(
		const struct tcc_module *const module_inst,
		const uint32_t top_value);

/** @} */


/**
 * \name Set Output Pattern
 * @{
 */

enum status_code tcc_set_pattern(
		const struct tcc_module *const module_inst,
		const uint32_t line_index,
		const enum tcc_output_pattern pattern);

/** @} */


/**
 * \name Set Ramp Index
 * @{
 */

/**
 * \brief Sets TCC module ramp index on next cycle
 *
 * Force cycle A and cycle B changes in RAMP2 and RAMP2A operation.
 * See \ref tcc_ramp.
 *
 * \param[in]  module_inst Pointer to the software module instance struct
 * \param[in]  ramp_index  Ramp index (\ref tcc_ramp_index) of the next cycle
 */
static inline void tcc_set_ramp_index(
		const struct tcc_module *const module_inst,
		const enum tcc_ramp_index ramp_index)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Get a pointer to the module's hardware instance */
	Tcc *const tcc_module = module_inst->hw;
	uint32_t last_cmd;

	/* Wait last command done */
	do {
		while (tcc_module->SYNCBUSY.bit.CTRLB) {
			/* Wait for sync */
		}
		if (TCC_RAMP_INDEX_DEFAULT == ramp_index) {
			/* Cancel pending command */
			tcc_module->CTRLBCLR.reg = TCC_CTRLBSET_IDXCMD_DISABLE;
			return;
		}
		last_cmd = tcc_module->CTRLBSET.reg & TCC_CTRLBSET_IDXCMD_Msk;
		if (TCC_CTRLBSET_IDXCMD_DISABLE == last_cmd) {
			break;
		} else if (TCC_CTRLBSET_CMD(ramp_index) == last_cmd) {
			/* Command have been issued */
			return;
		}
	} while (1);

	/* Write command to execute */
	tcc_module->CTRLBSET.reg = TCC_CTRLBSET_CMD(ramp_index);
}

/**
 * \name Status Management
 * @{
 */

/**
 * \brief Checks if the timer/counter is running.
 *
 * \param[in] module_inst  Pointer to the TCC software instance struct
 *
 * \return Module running status.
 *
 * \retval true The timer/counter is running.
 * \retval false The timer/counter is stopped.
 */
static inline bool tcc_is_running(
		struct tcc_module *const module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	return !module_inst->hw->STATUS.bit.STOP;
}

uint32_t tcc_get_status(
		struct tcc_module *const module_inst);

void tcc_clear_status(
		struct tcc_module *const module_inst,
		const uint32_t status_flags);

/** @} */


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* TCC_H_INCLUDED */
