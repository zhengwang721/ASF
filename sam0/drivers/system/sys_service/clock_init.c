/**
 * \file
 *
 * \brief SAM0+ GCLK configuration
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

#include <compiler.h>
#include <clock.h>
#include <conf_clocks.h>
#include <gclk.h>
#include <clock_init.h>

/**
 * \brief Initialize clock system based on the configuration in conf_clocks.h
 *
 * This function will apply the settings in conf_clock.h when run from the user
 * application. All clock sources and GCLKs are up and running when this function
 * returns.
 */
void clocks_init(void)
{
	struct system_clock_source_config conf;
	struct system_gclk_gen_conf gclk_generator_conf;

	/* Initialize config struct to defaults */
	system_clock_source_get_default_config(&conf);

	/* XOSC */
	#if CONF_CLOCK_XOSC_ENABLE == true
	conf.ext.external_clock = CONF_CLOCK_XOSC_EXTERNAL_CRYSTAL;
	conf.ext.startup_time = CONF_CLOCK_XOSC_STARTUP_TIME;
	conf.ext.auto_gain_control = CONF_CLOCK_XOSC_AUTO_GAIN_CONTROL;

	system_clock_source_set_config(&conf,CLOCK_SOURCE_XOSC);
	#endif /* CONF_CLOCK_XOSC_ENABLE */

	/* XOSC32K */
	#if CONF_CLOCK_XOSC32K_ENABLE == true
	conf.ext.external_clock = CONF_CLOCK_XOSC32K_EXTERNAL_CRYSTAL;
	conf.ext.startup_time = CONF_CLOCK_XOSC32K_STARTUP_TIME;
	conf.ext.auto_gain_control = CONF_CLOCK_XOSC32K_AUTO_AMPLITUDE_CONTROL;
	conf.osc32k.enable_1khz_output = CONF_CLOCK_XOSC32K_ENABLE_1KHZ_OUPUT;
	conf.osc32k.enable_32khz_output = CONF_CLOCK_XOSC32K_ENABLE_32KHZ_OUTPUT;

	system_clock_source_set_config(&conf, CLOCK_SOURCE_XOSC32K);
	#endif /* CONF_CLOCK_XOSC32K_ENABLE */

	/* OSC32K */
	#if CONF_CLOCK_OSC32K_ENABLE == true
	conf.osc32k.enable_1khz_output = CONF_CLOCK_OSC32K_ENABLE_1KHZ_OUTPUT;
	conf.osc32k.enable_32khz_output = CONF_CLOCK_OSC32K_ENABLE_32KHZ_OUTPUT;

	system_clock_source_set_config(&conf, CLOCK_SOURCE_OSC32K);
	#endif /* CONF_CLOCK_OSC32K_ENABLE */

	/* DFLL */
	#if CONF_CLOCK_DFLL_ENABLE == true
	conf.dfll.loop = CONF_CLOCK_DFLL_LOOP_MODE;

	#if (CONF_CLOCK_DFLL_MODE == CLOCK_DFLL_OPEN_LOOP)
		// TODO: Add 48MHz calibration value here
	#endif

		#if CONF_CLOCK_DFLL_QUICK_LOCK == true
		conf.dfll.quick_lock = CLOCK_DFLL_QUICK_LOCK_ENABLE;
		#else
		conf.dfll.quick_lock = CLOCK_DFLL_QUICK_LOCK_DISABLE;
		#endif

		#if CONF_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK == true
		conf.dfll.stable_tracking = CLOCK_DFLL_TRACK_AFTER_FINE_LOCK;
		#else
		conf.dfll.stable_tracking = CLOCK_DFLL_FIX_AFTER_FINE_LOCK;
		#endif

		#if CONF_CLOCK_DFLL_KEEP_LOCK_ON_WAKEUP == true
		conf.dfll.wakeup_lock = CLOCK_DFLL_KEEP_LOCK_AFTER_WAKE;
		#else
		conf.dfll_wakeup_lock = CLOCK_DFLL_LOSE_LOCK_AFTER_WAKE;
		#endif

		#if CONF_CLOCK_DFLL_ENABLE_CHILL_CYCLE == true
		conf.dfll.chill_cycle = CLOCK_DFLL_CHILL_CYCLE_ENABLE;
		#else
		conf.dfll.chill_cycle = CLOCK_DFLL_CHILL_CYCLE_DISABLE;
		#endif

	conf.dfll.coarse_max_step = CONF_CLOCK_DFLL_MAX_COARSE_STEP_SIZE;
	conf.dfll.fine_max_step = CONF_CLOCK_DFLL_MAX_FINE_STEP_SIZE;

	system_clock_source_set_config(&conf, CLOCK_SOURCE_DFLL);
	#endif /* CONF_CLOCK_DFLL_ENABLE */

	conf.rc8mhz.prescaler = CONF_CLOCK_RC8MHZ_PRESCALER;
	system_clock_source_set_config(&conf, CLOCK_SOURCE_RC8MHZ);


	#if CONF_CLOCK_CONFIGURE_GCLK == true


	#if CONF_CLOCK_GCLK_0_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_0_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_0_PRESCALER;
	system_gclk_gen_set_config(0, &gclk_generator_conf);
	system_gclk_gen_enable(0);
	#endif

	#if CONF_CLOCK_GCLK_1_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_1_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_1_PRESCALER;
	system_gclk_gen_set_config(1, &gclk_generator_conf);
	system_gclk_gen_enable(1);
	#endif

	#if CONF_CLOCK_GCLK_2_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_2_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_2_PRESCALER;
	system_gclk_gen_set_config(2, &gclk_generator_conf);
	system_gclk_gen_enable(2);
	#endif

	#if CONF_CLOCK_GCLK_3_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_3_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_3_PRESCALER;
	system_gclk_gen_set_config(3, &gclk_generator_conf);
	system_gclk_gen_enable(3);
	#endif

	#if CONF_CLOCK_GCLK_4_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_4_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_4_PRESCALER;
	system_gclk_gen_set_config(4, &gclk_generator_conf);
	system_gclk_gen_enable(4);
	#endif

	#if CONF_CLOCK_GCLK_5_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_5_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_5_PRESCALER;
	system_gclk_gen_set_config(5, &gclk_generator_conf);
	system_gclk_gen_enable(5);
	#endif

	#if CONF_CLOCK_GCLK_6_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_6_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_6_PRESCALER;
	system_gclk_gen_set_config(6, &gclk_generator_conf);
	system_gclk_gen_enable(6);
	#endif

	#if CONF_CLOCK_GCLK_7_ENABLE == true
	gclk_generator_conf.source_clock = CONF_CLOCK_GCLK_7_CLOCK_SOURCE;
	gclk_generator_conf.division_factor = CONF_CLOCK_GCLK_7_PRESCALER;
	system_gclk_gen_set_config(7, &gclk_generator_conf);
	system_gclk_gen_enable(7);
	#endif

	#endif /* Configure GCLK */


	/* CPU and BUS clocks */
	clock_main_set_source(CONF_CLOCK_CPU_CLOCK_SOURCE);
	clock_cpu_set_divider(CONF_CLOCK_CPU_DIVIDER);
	#if CONF_CLOCK_ENABLE_CPU_CLOCK_FAILURE_DETECT == true
	clock_main_set_failure_detect(true);
	#else
	clock_main_set_failure_detect(false);
	#endif

	clock_apb_set_divider(CLOCK_APB_APBA, CONF_CLOCK_APBA_DIVIDER);
	clock_apb_set_divider(CLOCK_APB_APBB, CONF_CLOCK_APBB_DIVIDER);


}
