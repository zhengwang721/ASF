/**
 * \file
 *
 * \brief SAM D20 XOSC32K Runtime Failure Detector Application
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
 *
 * \license
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

/**
 * \mainpage SAM D20 XOSC32K Runtime Failure Detector Application
 * See \ref appdoc_main "here" for project documentation.
 * \copydetails appdoc_preface
 *
 *
 * \page appdoc_preface Overview
 * This application demonstrates a hardware XOSC32K external 32KHz crystal
 * oscillator clock source failure detector, using general purpose hardware
 * peripherals.
 */

/**
 * \page appdoc_main SAM D20 XOSC32K Runtime Failure Detector
 *
 * Overview:
 * - \ref appdoc_samd20_xosc32k_fail_detect_intro
 * - \ref appdoc_samd20_xosc32k_fail_detect_compinfo
 * - \ref appdoc_samd20_xosc32k_fail_detect_contactinfo
 *
 * \section appdoc_samd20_xosc32k_fail_detect_intro Introduction
 * This application demonstrates a runtime XOSC32K (external 32KHz crystal
 * oscillator) failure detector, using hardware timer peripherals and the
 * SAM D20 event system.
 *
 * A pair of timers are linked together; one with a clock source from the
 * XOSC32K clock, and another from the Ultra Low Power 32KHz (ULP) internal RC
 * clock source. Each time the XOSC32K timer reaches a configurable count value,
 * a hardware event resets the second timer using the ULP clock source.
 *
 * If the ULP clocked timer reaches a configurable count value before it is
 * reset by the XOSC32K timer, the XOSC32K clock source is considered failed and
 * a callback function is executed.
 *
 * \section appdoc_samd20_xosc32k_fail_detect_compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for ARM.
 * Other compilers may or may not work.
 *
 * \section appdoc_samd20_xosc32k_fail_detect_contactinfo Contact Information
 * For further information, visit
 * <a href="http://www.atmel.com">http://www.atmel.com</a>.
 */

#include <asf.h>

/** Number of cycles on the XOSC32K before resetting the reference timer */
#define CRYSTAL_RESET_CYCLES   200
/** Number of cycles on the ULP32K before assumine a XOSC32K failure */
#define REFERENCE_FAIL_CYCLES  (CRYSTAL_RESET_CYCLES * 2)
/** Generator index configured to source from the XOSC32K crystal. */
#define GCLK_GEN_XOSC32K       GCLK_GENERATOR_2
/** Generator index configured to source from the interal ULP32K oscillator. */
#define GCLK_GEN_REFERENCE     GCLK_GENERATOR_3
/** Timer configured to source from the XOSC32K crystal generator */
#define TC_CRYSTAL             TC0
/** Timer configured to source from the ULP32K oscillator generator */
#define TC_REFERENCE           TC2
/** Event system channel to use to link the crystal and reference timers */
#define EVSYS_XTAL_CHAN        EVENT_CHANNEL_0
/** Event generator ID from the crystal timer's overflow event */
#define EVSYS_XTAL_TC_GEN      EVSYS_ID_GEN_TC0_OVF
/** Event user ID of the reference timer's configurable action */
#define EVSYS_XTAL_TC_USR      EVSYS_ID_USER_TC2_EVU


/** Software instance of the XOSC32K timer */
static struct tc_module tc_crystal;
/** Software instance of the ULP32K timer */
static struct tc_module tc_reference;


/** Callback run when a XOSC32K crystal failure is detected.
 *
 *  \param[in]  instance  Timer instance that triggered the failure (\ref TC_REFERENCE)
 */
static void xosc32k_fail_callback(
		struct tc_module *instance)
{
	for (;;)
	{
		/* Crystal failed - act accordingly */
		Assert(false);
	}
}

/** Configures and starts the XOSC32K external crystal. */
static void init_xosc32k(void)
{
	/* Configure and enable the XOSC32K clock source */
	struct system_clock_source_xosc32k_config xosc32k_conf;
	system_clock_source_xosc32k_get_config_defaults(&xosc32k_conf);
	xosc32k_conf.on_demand = false;
	xosc32k_conf.auto_gain_control = false;
	system_clock_source_xosc32k_set_config(&xosc32k_conf);
	system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC32K);
}

/** Initializes the XOSC32K crystal failure detector, and starts it.
 *
 *  \param[in]  failure_callback  Callback function to run upon XOSC32K failure
 */
static void init_xosc32k_fail_detector(
		tc_callback_t failure_callback)
{
	/* TC pairs share the same clock, ensure reference and crystal timers use
	 * different clocks */
	Assert(Abs(_tc_get_inst_index(TC_REFERENCE) -
			_tc_get_inst_index(TC_CRYSTAL)) >= 2);

	/* The crystal detection cycle count must be less than the reference cycle
	 * count, so that the reference timer is periodically reset before expiry */
	Assert(CRYSTAL_RESET_CYCLES < REFERENCE_FAIL_CYCLES);

	/* Must use different clock generators for the crystal and reference, must
	 * not be CPU generator 0 */
	Assert(GCLK_GEN_XOSC32K   != GCLK_GEN_REFERENCE);
	Assert(GCLK_GEN_XOSC32K   != GCLK_GENERATOR_0);
	Assert(GCLK_GEN_REFERENCE != GCLK_GENERATOR_0);

	/* Configure and enable the XOSC32K GCLK generator */
	struct system_gclk_gen_config xosc32k_gen_conf;
	system_gclk_gen_get_config_defaults(&xosc32k_gen_conf);
	xosc32k_gen_conf.source_clock = SYSTEM_CLOCK_SOURCE_XOSC32K;
	system_gclk_gen_set_config(GCLK_GEN_XOSC32K, &xosc32k_gen_conf);
	system_gclk_gen_enable(GCLK_GEN_XOSC32K);

	/* Configure and enable the reference clock GCLK generator */
	struct system_gclk_gen_config ref_gen_conf;
	system_gclk_gen_get_config_defaults(&ref_gen_conf);
	ref_gen_conf.source_clock = SYSTEM_CLOCK_SOURCE_ULP32K;
	system_gclk_gen_set_config(GCLK_GEN_REFERENCE, &ref_gen_conf);
	system_gclk_gen_enable(GCLK_GEN_REFERENCE);

	/* Set up crystal counter - when target count elapses, trigger event */
	struct tc_config tc_crystal_conf;
	tc_get_config_defaults(&tc_crystal_conf);
	tc_crystal_conf.clock_source = GCLK_GEN_XOSC32K;
	tc_crystal_conf.size_specific.size_16_bit.compare_capture_channel[0] =
			CRYSTAL_RESET_CYCLES;
	tc_crystal_conf.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
	tc_init(&tc_crystal, TC_CRYSTAL, &tc_crystal_conf);

	/* Set up reference counter - when event received, restart */
	struct tc_config tc_ref_conf;
	tc_get_config_defaults(&tc_ref_conf);
	tc_ref_conf.clock_source = GCLK_GEN_REFERENCE;
	tc_ref_conf.size_specific.size_16_bit.compare_capture_channel[0] =
			REFERENCE_FAIL_CYCLES;
	tc_ref_conf.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
	tc_ref_conf.event_action = TC_EVENT_ACTION_RETRIGGER;
	tc_init(&tc_reference, TC_REFERENCE, &tc_ref_conf);

	/* Configure event channel and link it to the crystal counter */
	struct events_chan_config event_chan_xtal_conf;
	events_chan_get_config_defaults(&event_chan_xtal_conf);
	event_chan_xtal_conf.generator_id = EVSYS_XTAL_TC_GEN;
	event_chan_xtal_conf.path = EVENT_PATH_ASYNCHRONOUS;
	events_chan_set_config(EVSYS_XTAL_CHAN, &event_chan_xtal_conf);

	/* Configure event user and link it to the reference counter */
	struct events_user_config event_user_xtal_conf;
	events_user_get_config_defaults(&event_user_xtal_conf);
	event_user_xtal_conf.event_channel_id = EVSYS_XTAL_CHAN;
	events_user_set_config(EVSYS_XTAL_TC_USR, &event_user_xtal_conf);

	/* Enable event generation for crystal counter */
	struct tc_events tc_crystal_events = { .generate_event_on_overflow = true };
	tc_enable_events(&tc_crystal, &tc_crystal_events);

	/* Enable event reception for reference counter */
	struct tc_events tc_ref_events = { .on_event_perform_action = true };
	tc_enable_events(&tc_reference, &tc_ref_events);

	/* Enable compare callback for the reference counter - if reference count
	 * has been reached, crystal has failed */
	tc_register_callback(&tc_reference, failure_callback,
			TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback(&tc_reference,
			TC_CALLBACK_CC_CHANNEL0);

	/* Start both crystal and reference counters */
	tc_enable(&tc_crystal);
	tc_enable(&tc_reference);

	/* Software trigger reference timer restart event to begin detector */
	events_chan_software_trigger(EVSYS_XTAL_CHAN);
}

/** Main application entry point. */
int main(void)
{
	system_init();
	events_init();

	init_xosc32k();
	init_xosc32k_fail_detector(xosc32k_fail_callback);

	while (1) {

	}
}
