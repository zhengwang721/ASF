/**
 *
 * \file
 *
 * \brief SAM Segment Liquid Crystal Display(SLCD) Controller.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#ifndef SLCD_H_INCLUDED
#define SLCD_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup asfdoc_sam0_drivers_slcd_group SAM Segment Liquid Crystal Display(SLCD) Controller
 *
 * This driver for Atmel&reg; | SMART SAM devices provides an interface for the configuration
 * and management of the device's SLCD functionality. The following
 * driver API modes are covered by this manual:
 *
 *  - Polled APIs
 *  - Callback APIs
 *
 *
 * The following peripherals are used by this module:
 *  - Segment Liquid Crystal Display(SLCD)
 *
 * The following devices can use this module:
 *  - Atmel | SMART SAM L22
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam0_drivers_slcd_prerequisites
 *  - \ref asfdoc_sam0_drivers_slcd_module_overview
 *  - \ref asfdoc_sam0_drivers_slcd_special_considerations
 *  - \ref asfdoc_sam0_drivers_slcd_extra_info
 *  - \ref asfdoc_sam0_drivers_slcd_examples
 *  - \ref asfdoc_sam0_drivers_slcd_api_overview
 *
 *
 * \section asfdoc_sam0_drivers_slcd_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam0_drivers_slcd_module_overview Module Overview
 *
 * \subsection asfdoc_sam0_drivers_slcd_module_overview_internal Display Overview
 *
 * An LCD display is made of several segments (pixels or complete symbols)
 * which can be visible or invisible. A segment has two electrodes with liquid
 * crystal between them. These electrodes are the common terminal (COM pin) and
 * the segment terminal (SEG pin). When a voltage above a threshold voltage is applied
 * across the liquid crystal, the segment becomes visible. The voltage must alternate,
 * to avoid an electrophoresis effect in the liquid crystal, which degrades the
 * display.
 *
 * The LCD controller is intended for monochrome passive liquid crystal display (LCD)
 * with up to 8 common terminals and up to 44 segment terminals. A charge pump provides
 * LCD display supply which can be higher than supply voltage of the device.
 * Each LCD pin, segment or common terminals, can be configured as general purpose I/O pins
 * if not driven by LCD controller.
 *
 * \section asfdoc_sam0_drivers_slcd_special_considerations Special Considerations
 *
 * \subsection asfdoc_sam0_drivers_slcd_special_considerations_io I/O Lines
 *
 * The SLCD pins (SEG and COM) are multiplexed with other peripherals. The
 * user application must first configure the I/O Controller, to give control of
 * the requisite pins to the SLCD.
 *
 * \subsection asfdoc_sam0_drivers_slcd_special_considerations_power Power Management
 *
 * The SLCD will continue to operate in any sleep mode where the selected source clock
 * is running. The SLCD interrupts can be used to wake up the device from sleep modes.
 * Events connected to the event system can trigger other operations in the system
 * without exiting sleep modes.
 *
 * The power consumption of SLCD itself can be minimized by:
 * - Using the lowest acceptable frame rate (refer to the LCD glass technical
 * characteristics)
 * - Using the low-power waveform (default mode)
 * - Using automated modes of operation
 * - Configuring the lowest possible contrast value
 *
 * \section asfdoc_sam0_drivers_slcd_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam0_drivers_slcd_extra. This includes:
 *  - \ref asfdoc_sam0_drivers_slcd_extra_acronyms
 *  - \ref asfdoc_sam0_drivers_slcd_extra_dependencies
 *  - \ref asfdoc_sam0_drivers_slcd_extra_errata
 *  - \ref asfdoc_sam0_drivers_slcd_extra_history
 *
 * \section asfdoc_sam0_drivers_slcd_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam0_drivers_slcd_exqsg.
 *
 *
 * \section asfdoc_sam0_drivers_slcd_api_overview API Overview
 * @{
 */

#include <compiler.h>
#include <system.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Deadtime Insertion Value.
 *
 * Enum deadtime at the end of each frame.
 */
enum slcd_deadtime {
	/** The dead time period is 0. */
	SLCD_DEADTIME_0 = 0,
	/** The dead time period is 1. */
	SLCD_DEADTIME_1,
	/** The dead time period is 2. */
	SLCD_DEADTIME_2,
	/** The dead time period is 3. */
	SLCD_DEADTIME_3,
	/** The dead time period is 4. */
	SLCD_DEADTIME_4,
	/** The dead time period is 5. */
	SLCD_DEADTIME_5,
	/** The dead time period is 6. */
	SLCD_DEADTIME_6,
	/** The dead time period is 7. */
	SLCD_DEADTIME_7,
};

/**
 * \brief Reference Refresh Frequency.
 *
 * Enum Reference refresh frequency.
 */
enum slcd_reference_refresh_freq {
	/** Bias Reference refresh frequency is 2KHz. */
	SLCD_REF_REFRESH_FREQ_2KHZ = 0,
	/** Bias Reference refresh frequency is 1KHz. */
	SLCD_REF_REFRESH_FREQ_1KHZ ,
	/** Bias Reference refresh frequency is 500Hz. */
	SLCD_REF_REFRESH_FREQ_500HZ ,
	/** Bias Reference refresh frequency is 250Hz. */
	SLCD_REF_REFRESH_FREQ_250HZ ,
	/** Bias Reference refresh frequency is 125Hz. */
	SLCD_REF_REFRESH_FREQ_125HZ ,
	/** Bias Reference refresh frequency is 62.5Hz. */
	SLCD_REF_REFRESH_FREQ_62HZ ,
};

/**
 * \brief Power Refresh Frequency.
 *
 * Enum Power refresh rrequency.
 */
enum slcd_power_refresh_freq {
	/** Charge pump refresh frequency is 2KHz. */
	SLCD_POWER_REFRESH_FREQ_2KHZ = 0,
	/** Charge pump refresh frequency is 1KHz. */
	SLCD_POWER_REFRESH_FREQ_1KHZ ,
	/** Charge pump refresh frequency is 500KHz. */
	SLCD_POWER_REFRESH_FREQ_500HZ,
	/** Charge pump refresh frequency is 250Hz. */
	SLCD_POWER_REFRESH_FREQ_250HZ ,
};

/**
 * \brief Display Memory Update Frame Counter Selection.
 *
 * Enum Display memory update frame counter selection.
 */
enum slcd_display_memory_update_sel_fc {
	/** Display memory update frame counter is  Frame Counter 0. */
	SLCD_DISPLAY_MEMORY_UPDATE_SEL_FC0 = 0,
	/** Display memory update frame counter is  Frame Counter 1. */
	SLCD_DISPLAY_MEMORY_UPDATE_SEL_FC1,
	/** Display memory update frame counter is  Frame Counter 2. */
	SLCD_DISPLAY_MEMORY_UPDATE_SEL_FC2,
	/** Frame Counter event to DMU is forced to 0. */
	SLCD_DISPLAY_MEMORY_UPDATE_SEL_NFC,
};

/**
 * \brief Internal/External VLCD selection.
 *
 * Enum how VLCD is generated.
 */
enum slcd_vlcd_generated_sel {
	/** Internal VLCD generation. */
	SLCD_VLCD_INTERNAL = 0,
	/** External VLCD generation. */
	SLCD_VLCD_EXTTERNAL,
};

/**
 * \brief SLCD Bias Setting.
 *
 * Enum SLCD Bias Setting.
 */
enum slcd_bias {
	/** Static bias. */
	SLCD_BIAS_STATIC = 0,
	/** 1/2 bias. */
	SLCD_BIAS_HALF,
	/** 1/3 bias. */
	SLCD_BIAS_THIRD,
	/** 1/4 bias. */
	SLCD_BIAS_FOURTH,

};

/**
 * \brief SLCD Duty Setting.
 *
 * Enum SLCD duty setting.
 */
enum slcd_duty {
	/** Static duty. */
	SLCD_DUTY_STATIC = 0,
	/** Half duty. */
	SLCD_DUTY_HALF,
	/** 1/3 duty. */
	SLCD_DUTY_THIRD,
	/** 1/4 duty. */
	SLCD_DUTY_FOURTH,
	/** 1/6 duty. */
	SLCD_DUTY_SIXTH,
	/** 1/8 duty. */
	SLCD_DUTY_EIGHT,
};

/**
 * \brief Clock Prescaler.
 *
 * Enum Clock prescaler.
 */
enum slcd_clock_presc {
	/** Clock prescaler is 16. */
	SLCD_CLOCK_PRESC_16 = 0,
	/** Clock divider is 32. */
	SLCD_CLOCK_PRESC_32,
	/** Clock divider is 64. */
	SLCD_CLOCK_PRESC_64,
	/** Clock divider is 128. */
	SLCD_CLOCK_PRESC_128,
};

/**
 * \brief Clock Divider.
 *
 * Enum Clock divider.
 */
enum slcd_clock_div {
	/** Clock divider is 1. */
	SLCD_CLOCK_DIV_1 = 0,
	/** Clock divider is 2. */
	SLCD_CLOCK_DIV_2,
	/** Clock divider is 3. */
	SLCD_CLOCK_DIV_3,
	/** Clock divider is 4. */
	SLCD_CLOCK_DIV_4,
	/** Clock divider is 5. */
	SLCD_CLOCK_DIV_5,
	/** Clock divider is 6. */
	SLCD_CLOCK_DIV_6,
	/** Clock divider is 7. */
	SLCD_CLOCK_DIV_7,
	/** Clock divider is 8. */
	SLCD_CLOCK_DIV_8,
};

/**
 * \brief LCD Working Power Mode.
 *
 * Enum LCD working power mode.
 */
enum slcd_power_supply_mode {
	/** LCD power automatically select regualation mode or pump mode. */
	SLCD_POWER_MODE_AUTO = 0,
	/** LCD power use step-up pump loop only. */
	SLCD_POWER_MODE_STEPUP,
	/** LCD power use step-down drop-out regulation loop only. */
	SLCD_POWER_MODE_STEPDOWN,
};

/**
 * \brief Waveform Mode.
 *
 * Enum Waveform mode.
 */
enum slcd_waveform_mode {
	/** Low power waveform mode. */
	SLCD_LOW_POWER_WAVEFORM_MODE = 0,
	/** Standard waveform mode. */
	SLCD_STANDARD_WAVEFORM_MODE,
};

/**
 * \brief Contrast Value.
 *
 * Enum Contrast value.
 */
enum slcd_contrast {
	/** SLCD contrast value is 1. */
	SLCD_CONTRAST_1 = 0,
	/** SLCD contrast value is 2. */
	SLCD_CONTRAST_2,
	/** SLCD contrast value is 3. */
	SLCD_CONTRAST_3,
	/** SLCD contrast value is 4. */
	SLCD_CONTRAST_4,
	/** SLCD contrast value is 5. */
	SLCD_CONTRAST_5,
	/** SLCD contrast value is 6. */
	SLCD_CONTRAST_6,
	/** SLCD contrast value is 7. */
	SLCD_CONTRAST_7,
	/** SLCD contrast value is 8. */
	SLCD_CONTRAST_8,
	/** SLCD contrast value is 9. */
	SLCD_CONTRAST_9,
	/** SLCD contrast value is 10. */
	SLCD_CONTRAST_10,
	/** SLCD contrast value is 11. */
	SLCD_CONTRAST_11,
	/** SLCD contrast value is 12. */
	SLCD_CONTRAST_12,
	/** SLCD contrast value is 13. */
	SLCD_CONTRAST_13,
	/** SLCD contrast value is 14. */
	SLCD_CONTRAST_14,
	/** SLCD contrast value is 15. */
	SLCD_CONTRAST_15,
	/** SLCD contrast value is 16. */
	SLCD_CONTRAST_16,
};

/**
 * \brief SLCD Frame Counter Definition.
 *
 * Enum SLCD frame counter definition.
 */
enum slcd_frame_counter {
	/** SLCD frame counter 0. */
	SLCD_FRAME_COUNTER_0,
	/** SLCD frame counter 1. */
	SLCD_FRAME_COUNTER_1,
	/** SLCD frame counter 2. */
	SLCD_FRAME_COUNTER_2,
};

/**
 * Basic configuration for SLCDC.
 */
struct slcd_config {

	/** Duty ratio setting.*/
	enum slcd_duty duty;
	/** Bias setting.*/
	enum slcd_bias bias;
	/** Clock prescaler. */
	enum slcd_clock_presc clock_presc;
	/** Clock divider. */
	enum slcd_clock_div clock_div;

	/** Internal/External VLCD selection.*/
	enum slcd_vlcd_generated_sel xvlcd;
    /** Keep SLCD enabled in standby sleep mode if true. */
	bool run_in_standby;
	/** waveform mode selection. */
	enum slcd_waveform_mode waveform_mode;

	/** Display memory update frame counter selection. */
	enum  slcd_display_memory_update_sel_fc  dmfcs;
	/** Reference fefresh frequency. */
	enum slcd_reference_refresh_freq ref_refresh_freq;
	/** Power fefresh frequency. */
	enum slcd_power_refresh_freq power_refresh_freq;
	/** Low resistance network duration. */
	uint8_t low_resistance_duration;
	/** Enable Low resistance if true. */
	bool enable_low_resistance;
	/** Bias buffer duration. */
	uint8_t bias_buffer_duration;
	/** Enable bias buffer if true. */
	bool enable_bias_buffer;
	/** Enable external bias capacitor if true.*/
	bool enable_ext_bias_capacitor;
	/** Working power mode for LCD power supply.*/
	enum slcd_power_supply_mode power_mode;
};

/**
 * \brief SLCD event enable/disable structure.
 *
 * Event flags for the SLCD module. This is used to enable and
 * disable events via \ref slcd_enable_events() and \ref slcd_disable_events().
 */
struct slcd_events {
	/** Enable event generation on frame counter 0 overflow. */
	bool generate_event_on_frame_counter_0_overflow;
	/** Enable event generation on frame counter 1 overflow. */
	bool generate_event_on_frame_counter_1_overflow;
	/** Enable event generation on frame counter 2 overflow. */
	bool generate_event_on_frame_counter_2_overflow;
};

/**
 * \brief SLCD Blink Mode Selection.
 *
 * Enum SLCD blinking mode.
 */
enum slcd_blink_mode {
	/** All segments are allowed to blink. */
	SLCD_BLINK_ALL_SEG = 0,
	/** Selected segments are allowed to blink. */
	SLCD_BLINK_SELECT_SEG,
};

struct slcd_blink_config {
	/** Frame counter selection for blinking. */
	enum slcd_frame_counter fc;
	/** Blinking Mode. */
	enum slcd_blink_mode blink_mode;
	/** Mask segment 0 (SEG0) to blink, each bit enables SEG0,
		connected to COM0 up to COM7.*/
	uint8_t blink_seg0_mask;
	/** Mask segment 1 (SEG1) to blink, each bit enables SEG0,
		connected to COM0 up to COM7.*/
	uint8_t blink_seg1_mask;
};

/**
 * \brief SLCD Circular Shift Direction.
 *
 * Enum SLCD circular shift direction.
 */
enum slcd_circular_shift_dir {
	/** Sircular shift direction is left. */
	SLCD_CIRCULAR_SHIFT_LEFT = 0,
	/** Sircular shift direction is right. */
	SLCD_CIRCULAR_SHIFT_RIGHT,
};

struct slcd_circular_shift_config {
	/** Frame counter selection for circular shift. */
	enum slcd_frame_counter fc;
	/** Shift direction. */
	enum slcd_circular_shift_dir dir;
	/** Size of the circular shift register, MAX. size is 16.*/
	uint8_t size;
	/** Circular shift register value. */
	uint16_t data;
};

/**
 * \brief Automated Char  Order.
 *
 * Enum Automated char order.
 */
enum slcd_automated_char_order {
	/** Segment is starting from bottom right . */
	SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_LEFT = 0,
	/** Segment is starting from bottom left . */
	SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,
};

/**
 * \brief Automated Char Display Mode.
 *
 * Enum Automated char display mode.
 */

enum slcd_automated_char_mode {
	/** Sequential Display Mode . */
	SLCD_AUTOMATED_CHAR_SEQ = 0,
	/** Scrolling Display Mode. */
	SLCD_AUTOMATED_CHAR_SCROLL,
};

struct slcd_automated_char_config {
	/** Mapping order in automated char mode. */
	enum slcd_automated_char_order order;
	/** Frame counter selection for automated character mapping. */
	enum slcd_frame_counter fc;
	/** Display mode. */
	enum slcd_automated_char_mode mode;
	/** Define the number of SEG line per digit,
		it equal to number of SEG line - 1.*/
	uint8_t seg_line_num;
	/** Define the index of the first segment terminal of the digit to display.*/
	uint8_t start_seg_line;
	/** Define the number of digit per row.*/
	uint8_t row_digit_num;
	/** Define the number of digit, it must be greater than 1.*/
	uint8_t digit_num;
	/** Define the number of steps in scrolling mode.
		scrolling_step = character string length - digit_num + 1*/
	uint8_t scrolling_step;
		/** Define the number of COM line per row,
		it equal to number of COM line - 1.*/
	uint8_t com_line_num;

};

/**
 * \brief Available clock source for SLCD.
 * SLCD clock source. Before enabling the SLCD module,
 * the SLCD clock must be enabled and selected .
 */
enum slcd_clock_sel {
	/** 32.768KHz from 32KHz internal ULP oscillator. */
	SLCD_CLOCK_SELECTION_ULP32K = 0,
	/** 32.768KHz from 32.768KHz external crystal oscillator. */
	SLCD_CLOCK_SELECTION_XOSC32K,
};

/**
 * \name SLCD Basic Operation Functions
 * @{
 */

void slcd_get_config_defaults(struct slcd_config *config);
enum status_code slcd_init(struct slcd_config *const config);
void slcd_enable(void);
void slcd_disable(void);
bool slcd_is_enabled(void);
void slcd_reset(void);
void slcd_set_contrast(enum slcd_contrast contrast);

/**
 * \brief Determines if SLCD module(s) are currently synchronizing to the bus.
 *
 * Checks to see if the underlying hardware peripheral module(s) are currently
 * synchronizing across multiple clock domains to the hardware bus, This
 * function can be used to delay further operations on a module until such time
 * that it is ready, to prevent blocking delays for synchronization in the
 * user application.
 *
 * \return Synchronization status of the underlying hardware module(s).
 *
 * \retval true  if the module synchronization is ongoing
 * \retval false if the module has completed synchronization
 */
static inline bool slcd_is_syncing(void)
{

	if (SLCD->SYNCBUSY.reg) {
		return true;
	}

	return false;
}

/**
 * \brief Lock Shadow Memory.
 *
 * It allows update of shadow display memory. If the display memory
 * is modified, the display remains unchanged when locked.
 */
static inline void slcd_lock_shadow_memory(void)
{
	SLCD->CTRLC.reg |= SLCD_CTRLC_LOCK;
}

/**
 * \brief Unlock Shadow Memory.
 *
 * Unlock the shadow display memory.
 */
static inline void slcd_unlock_shadow_memory(void)
{
	SLCD->CTRLC.reg &= (SLCD_CTRLC_MASK & ( ~SLCD_CTRLC_LOCK));
}

/**
 * \brief Clear Display Memory.
 *
 * Clears immediately the display memory.
 */
static inline void slcd_clear_display_memory(void)
{
	SLCD->CTRLC.reg |= SLCD_CTRLC_CLEAR;
}

/**
 * \brief Display enable.
 *
 * Enable COM/SEG signal output.
 */
static inline void slcd_enable_display(void)
{
	SLCD->CTRLD.reg |= SLCD_CTRLD_DISPEN;
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Display disable.
 *
 * Disable COM/SEG signal output.
 */
static inline void slcd_disable_display(void)
{
	SLCD->CTRLD.reg &= (SLCD_CTRLD_MASK & ( ~SLCD_CTRLD_DISPEN));
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Deadtime enable.
 *
 * Enable deadtime.
 */
static inline void slcd_enable_deadtime(void)
{
	//todo
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Deadtime disable.
 *
 * Disable deadtime.
 */
static inline void slcd_disable_deadtime(void)
{
	////todo
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Set deadtime.
 *
 * Set deadtime.
 */
static inline void slcd_set_deadtime(enum slcd_deadtime deadtime)
{
	//todo
}

/** @} */

/**
 * \name SLCD Blink Functions
 * @{
 */

/**
 * \brief Blink Mode Enable.
 *
 * Enable blink mode.
 */
static inline void slcd_enable_blink(void)
{
	SLCD->CTRLD.reg |= SLCD_CTRLD_BLINK;
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Blink Mode Disable.
 *
 * Disable blink mode.
 */
static inline void slcd_disable_blink(void)
{
	SLCD->CTRLD.reg &= ~SLCD_CTRLD_BLINK;
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

void slcd_blink_get_config_defaults(struct slcd_blink_config *blink_config);
enum status_code slcd_blink_set_config(struct slcd_blink_config *const blink_config);


/** @} */

/**
 * \name SLCD Blank Functions
 * @{
 */

/**
 * \brief Blank Mode Enable.
 *
 * Enable blank mode.
 */

static inline void slcd_enable_blank(void)
{
	SLCD->CTRLD.reg |= SLCD_CTRLD_BLANK;
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}
/**
 * \brief Blank Mode Disable.
 *
 * Disable blank mode.
 */
static inline void slcd_disable_blank(void)
{
	SLCD->CTRLD.reg &= ~SLCD_CTRLD_BLANK;
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}
/** @} */

/**
 * \name SLCD Event Functions
 * @{
 */

/**
 * \brief Enables a SLCD event output.
 *
 *  Enables one or more output events
 *
 *  \note Events cannot be altered while the module is enabled.
 *
 *  \param[in] events       Struct containing flags of events to enable
 */
static inline void slcd_enable_events(struct slcd_events *const events)
{

	Assert(hw);
	Assert(events);

	uint8_t event_mask = 0;

	if (events->generate_event_on_frame_counter_0_overflow) {
		event_mask |= SLCD_EVCTRL_FC0OEO;
	}

	if (events->generate_event_on_frame_counter_1_overflow) {
		event_mask |= SLCD_EVCTRL_FC1OEO;
	}

	if (events->generate_event_on_frame_counter_2_overflow) {
		event_mask |= SLCD_EVCTRL_FC2OEO;
	}

	SLCD->EVCTRL.reg |= event_mask;
}

/**
 * \brief Disables a SLCD event output.
 *
 *  Disables one or more SLCD events output.
 *
 *  \param[in] events Struct containing flags of events to disable
 */
static inline void slcd_disable_events(struct slcd_events *const events)
{

	Assert(events);

	uint8_t event_mask = 0;

	if (events->generate_event_on_frame_counter_0_overflow) {
		event_mask |= SLCD_EVCTRL_FC0OEO;
	}

	if (events->generate_event_on_frame_counter_1_overflow) {
		event_mask |= SLCD_EVCTRL_FC1OEO;
	}

	if (events->generate_event_on_frame_counter_2_overflow) {
		event_mask |= SLCD_EVCTRL_FC2OEO;
	}

	SLCD->EVCTRL.reg &= ~event_mask;
}

/** @} */

/**
 * \name SLCD Frame Counter Functions
 * @{
 */

/**
 * \brief Frame Counter Configuration.
 *
 *  Config frame counter.
 *
 *  \note Frame counter cannot be set while it is enabled.
 *
 *  \param[in] fc Frame counter index
 *  \param[in] presc_bypass_enable Bypass of the frame counter prescaler
 *  \param[in] overflow_value  Frame counter overflow value. The number of frame
 *		before overflow is ((overflow_value+1)*8) when presc_bypass_enable=0
 		else (overflow_value+1). The MAX. overflow value is 0x1ffff.
 */
static inline void slcd_set_frame_counter(enum slcd_frame_counter fc,
										   bool  presc_bypass_enable,
										   uint16_t overflow_value)
{
	*(&(SLCD->FC0.reg) + fc) =  (presc_bypass_enable << SLCD_FC0_PB_Pos)
							 | SLCD_FC0_OVF(overflow_value);
}

/**
 * \brief Enables a Frame Counter.
 *
 *  Enables one frame counter.
 *
 *	\param[in] fc Frame counter index
 */
static inline void slcd_enable_frame_counter(enum slcd_frame_counter fc)
{

	switch(fc) {
		case SLCD_FRAME_COUNTER_0:
			SLCD->CTRLD.reg |= SLCD_CTRLD_FC0EN;
			break;
		case SLCD_FRAME_COUNTER_1:
			SLCD->CTRLD.reg |= SLCD_CTRLD_FC1EN;
			break;
		case SLCD_FRAME_COUNTER_2:
			SLCD->CTRLD.reg |= SLCD_CTRLD_FC2EN;
			break;
		default :
			break;
	}
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Disable a Frame Counter.
 *
 *  Disable one frame counter.
 *
 *	\param[in] fc Frame counter index
 */
static inline void slcd_disable_frame_counter(enum slcd_frame_counter fc)
{
	switch(fc) {
		case SLCD_FRAME_COUNTER_0:
			SLCD->CTRLD.reg &= ~SLCD_CTRLD_FC0EN;
			break;
		case SLCD_FRAME_COUNTER_1:
			SLCD->CTRLD.reg &= ~SLCD_CTRLD_FC1EN;
			break;
		case SLCD_FRAME_COUNTER_2:
			SLCD->CTRLD.reg &= ~SLCD_CTRLD_FC2EN;
			break;
		default :
			break;
	}
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}
/** @} */

/**
 * \name COM/SEG PIN Selection Functions
 * There are 52 LCD pins, but SLCD can drive up to 48 LCD pins which can be
 * individually enabled or disabled according to the LCD glass.
 * Each LCD pin can be configured as a frontplane (SEG) or backplane (COM) offering
 * various configurations.
 * @{
 */

/**
 * \brief Set SLCD PIN.
 *
 *  Set SLCD PIN.
 *
 *  \note PIN cannot be set while the module is enabled.The number of LCD pins
 *	enabled should not be higher than the maximum of COM and SEG lines supported.
 *
 *	\param[in] pin_mask For LCD pins from 0 to 51.
 */
static inline void slcd_set_pin(uint64_t pin_mask)
{
	SLCD->LPENL.reg = pin_mask & SLCD_LPENL_MASK;
	SLCD->LPENH.reg = (pin_mask >> 32 ) & SLCD_LPENH_MASK;
}

/**
 * \brief Enable Single SLCD PIN.
 *
 *  Enable Single SLCD PIN.
 *
 *	\param[in] pin_index PIN index [0-51].
 */
static inline void slcd_enable_single_pin(uint8_t pin_index)
{
	volatile uint32_t tmp;

	tmp = SLCD->LPENL.reg;
	tmp &= ~(0x1 << pin_index);
	tmp |= (0x1 << pin_index);
	SLCD->LPENL.reg = tmp & SLCD_LPENL_MASK;

	tmp = SLCD->LPENH.reg;
	tmp &= ~((0x1 << pin_index) >> 32);
	tmp |= ((0x1 << pin_index) >> 32);
	SLCD->LPENH.reg = tmp;
}
/**
 * \brief Disable Single SLCD PIN.
 *
 *  Disable Single SLCD PIN.
 *
 *	\param[in] pin_index PIN index [0-51].
 */
static inline void slcd_disable_single_pin(uint8_t pin_index)
{
	volatile uint32_t tmp;

	tmp = SLCD->LPENL.reg;
	tmp &= ~(0x1 << pin_index);
	SLCD->LPENL.reg = tmp & SLCD_LPENL_MASK;

	tmp = SLCD->LPENH.reg;
	tmp &= ~((0x1 << pin_index) >> 32);
	SLCD->LPENH.reg = tmp & SLCD_LPENH_MASK;
}

/** @} */

/**
 * \name Display Memory Functions
 * CPU can access display memory in direct access or in indirect access.
 * @{
 */
void slcd_set_pixel(uint8_t pix_com, uint8_t pix_seg);
void slcd_clear_pixel(uint8_t pix_com, uint8_t pix_seg);

void slcd_set_seg_data(uint8_t seg_data,uint8_t byte_offset);
void slcd_set_seg_mask(uint8_t seg_mask);
/** @} */

/**
 * \name Automated Character Mapping Functions
 * @{
 */

/**
 * \brief Enables Automated Character Display.
 *
 *  Enables automated character display.
 */
static inline void slcd_enable_automated_character(void)
{
	SLCD->CTRLC.reg |= SLCD_CTRLC_ACMEN;
}

/**
 * \brief Disables Automated Character Display.
 *
 *  Disables automated character display.
 */
static inline void slcd_disable_automated_character(void)
{
	SLCD->CTRLC.reg &= ~SLCD_CTRLC_ACMEN;
}
void slcd_automated_char_get_config_default(
		struct slcd_automated_char_config *config);
enum status_code slcd_automated_char_set_config(
		struct slcd_automated_char_config *const config);
void slcd_automated_char_write_data(uint32_t seg_data);
void slcd_automated_char_set_mask(uint32_t data_mask);
void slcd_automated_char_set_index(uint8_t com_line_index,uint8_t seg_line_index);

/** @} */

/**
 * \name Automated Bit Mapping Functions
 * @{
 */

/**
 * \brief Enables Automated Bit Display.
 *
 *  Enables automated bit display.
 */
static inline void slcd_enable_automated_bit(void)
{
	SLCD->CTRLC.reg |= SLCD_CTRLC_ABMEN;
}

/**
 * \brief Disables Automated Bit Display.
 *
 *  Disables automated bit display.
 */
static inline void slcd_disable_automated_bit(void)
{
	SLCD->CTRLC.reg &= ~SLCD_CTRLC_ABMEN;
}

/**
 * \brief Sets Automated Bit Display.
 *
 *  Sets automated bit display.
 *
 *  \note Automated bit cannot be set while it is enabled or busy.
 */
static inline void slcd_set_automated_bit(uint8_t size,enum slcd_frame_counter fc)
{
	if(size > 0 && size < 0x3f) {
		SLCD->ABMCFG.reg = SLCD_ABMCFG_SIZE(size) | SLCD_ABMCFG_FCS(fc);
	}
}

/** @} */

/**
 * \name Autonomous Segment Animation
 * @{
 */

/**
 * \brief Enable SLCD circular shift mode.
 */
static inline void slcd_enable_circular_shift(void)
{
	SLCD->CTRLD.reg |= SLCD_CTRLD_CSREN;
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}
/**
 * \brief Disable SLCD circular shift mode.
 */

static inline void slcd_disable_circular_shift(void)
{
	SLCD->CTRLD.reg &= ~SLCD_CTRLD_CSREN;
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}
void slcd_circular_shift_get_config_defaults(
		struct slcd_circular_shift_config *const config);

enum status_code slcd_circular_shift_set_config(
		struct slcd_circular_shift_config *const config);

/** @} */

/**
 * \name SLCD Status
 * @{
 */

/**
 * \brief Checks if auto bit mapping state machine is busy.
 *
 * Checks if  auto bit mapping state machine is busy or not.
 *
 * \retval true   Auto bit mapping state machine is busy
 * \retval false  Auto bit mapping state machine is idle
 */

static inline bool slcd_get_auto_bit_status(void)
{
	return ((SLCD->STATUS.reg & SLCD_STATUS_ABMBUSY) == SLCD_STATUS_ABMBUSY);
}

/**
 * \brief Checks if auto character mapping state machine is busy.
 *
 * Checks if  auto character state machine is busy or not.
 *
 * \retval true   Auto character mapping state machine is busy
 * \retval false  Auto character mapping state machine is idle
 */

static inline bool slcd_get_auto_char_status(void)
{
	return ((SLCD->STATUS.reg & SLCD_STATUS_ACMBUSY) == SLCD_STATUS_ACMBUSY);
}

/**
 * \brief Checks if character writing function is busy.
 *
 * Checksif character writing function is busy or not.
 *
 * \retval true   Character writting function is busy
 * \retval false  Character writting function is ready for use
 */

static inline bool slcd_get_char_writing_status(void)
{
	return ((SLCD->STATUS.reg & SLCD_STATUS_CMWRBUSY) == SLCD_STATUS_CMWRBUSY);
}

/**
 * \brief Checks VLCD and VDD33 status.
 *
 * Checks VLCD and VDD33 status.
 *
 * \retval true   VDD33 is greater than target VLCD
 * \retval false  Target VLCD is greater than vdd33
 */

static inline bool slcd_get_vlcd_vdd33_status(void)
{
	return ((SLCD->STATUS.reg & SLCD_STATUS_VLCDS) == SLCD_STATUS_VLCDS);
}

/**
 * \brief Checks LCD Charge Pump Status.
 *
 * Checks LCD Charge Pump Status.
 *
 * \retval true   LCD power charge pump is running
 * \retval false  LCD power charge pump is stopped
 */

static inline bool slcd_get_charge_pump_status(void)
{
	return ((SLCD->STATUS.reg & SLCD_STATUS_PRUN) == SLCD_STATUS_PRUN);
}

/**
 * \brief Checks if VLCD  is Ready.
 *
 * Checks if VLCD is well regulated to the target value.
 *
 * \retval true   VLCD is well regulated to the target value
 * \retval false  VLCD is not well regulated to the target value
 */

static inline bool slcd_get_vlcd_ready_status(void)
{
	return ((SLCD->STATUS.reg & SLCD_STATUS_VLCDR) == SLCD_STATUS_VLCDR);
}

/** @} */


/** @} */

/**
 * \page asfdoc_sam0_drivers_slcd_extra Extra Information for SLCD
 *
 * \section asfdoc_sam0_drivers_slcd_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *      <td>SLCD</td>
 *      <td>Segment Liquid Crystal Display</td>
 * </tr>
 *  <tr>
 *      <td>COM</td>
 *      <td>Common,denotes how many segments are connected to a segment terminal</td>
 * </tr>
 *  <tr>
 *      <td>SEG</td>
 *      <td>Segment,the least viewing element (pixel) which can be on or off</td>
 * </tr>
 *  <tr>
 *      <td>Duty</td>
 *      <td> 1/(Number of common terminals on an actual LCD display)</td>
 * </tr>
 *  <tr>
 *      <td>Bias</td>
 *      <td>1/(Number of voltage levels used driving a LCD display -1)</td>
 * </tr>
 *  <tr>
 *      <td>Frame Rate</td>
 *      <td>Number of times the LCD segments are energized per second</td>
 * </tr>
 * </table>
 *
 *
 * \section asfdoc_sam0_drivers_slcd_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_sam0_drivers_slcd_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam0_drivers_slcd_extra_history Module History
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
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_sam0_drivers_slcd_exqsg Examples for SLCD
 *
 * This is a list of the available Quick Start Guides (QSGs) and example
 * applications for \ref asfdoc_sam0_drivers_slcd_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam0_slcd_basic_use_case
 *
 * \page asfdoc_sam0_drivers_slcd_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *      <td>A</td>
 *		<td>06/2015</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 *
 */

 #ifdef __cplusplus
}
#endif

#endif  /* SLCD_H_INCLUDED */
