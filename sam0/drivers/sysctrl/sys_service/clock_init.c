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

#include <sysctrl.h>
#include <conf_clocks.h>
#include <gclk.h>


/**
 * \brief Initialize clock system based on the configuration in conf_clocks.h
 *
 * This function will apply the settings in conf_clock.h when run from the user
 * application. All clock sources and GCLKs are up and running when this function
 * returns.
 */
// TODO: Naming
status_t clocks_init(void)
{
	struct clock_source_config conf;
	struct clock_gclk_gen_conf gclk_generator_conf;

	/* XOSC */
	#if CONF_CLOCK_XOSC_ENABLE == true
	conf.external_crystal = CONF_CLOCK_XOSC_EXTERNAL_CRYSTAL;
	conf.startup_time = CONF_CLOCK_XOSC_STARTUP_TIME;
	conf.auto_gain_control = CONF_CLOCK_XOSC_AUTO_GAIN_CONTROL;

	clock_source_set_config(&conf,CLKSOURCE_XOSC);
	#endif /* CONF_CLOCK_XOSC_ENABLE */

	/* XOSC32K */
	#if CONF_CLOCK_XOSC32K_ENABLE == true
	conf.external_crystal = CONF_CLOCK_XOSC32K_EXTERNAL_CRYSTAL;
	conf.startup_time = CONF_CLOCK_XOSC32K_STARTUP_TIME;
	conf.auto_gain_control = CONF_CLOCK_XOSC32K_AUTO_AMPLITUDE_CONTROL;
	conf.enable_1khz_output = CONF_CLOCK_XOSC32K_ENABLE_1KHZ_OUTPUT;
	conf.enable_32khz_output = CONF_CLOCK_XOSC32K_ENABLE_32KHZ_OUTPUT;

	clock_source_set_config(&conf, CLKSOURCE_XOSC32K);
	#endif /* CONF_CLOCK_XOSC32K_ENABLE */

	/* OSC32K */
	#if CONF_CLOCK_OSC32K_ENABLE == true
	conf.enable_1khz_output = CONF_CLOCK_OSC32K_ENABLE_1KHZ_OUTPUT;
	conf.enable_32khz_output = CONF_CLOCK_OSC32K_ENABLE_32KHZ_OUTPUT;

	clock_source_set_config(&conf, CLKSOURCE_OSC32K);
	#endif /* CONF_CLOCK_OSC32K_ENABLE */

	/* DFLL */
	#if CONF_CLOCK_DFLL_ENABLE == true
	conf.loop = CONF_CLOCK_DFLL_MODE;

	#if (CONF_CLOCK_DFLL_MODE == SYSCTRL_DFLL_OPEN_LOOP)
		conf.dfll_target_frequency = CONF_CLOCK_DFLL_FREQUENCY;
	#endif

		#if CONF_DFLL_QUICK_LOCK == true
		conf.qlock = SYSCTRL_DFLL_QLOCK_ENABLE;
		#else
		conf.qlock = SYSCTRL_DFLL_QLOCK_DISABLE;
		#endif

		#if CONF_DFLL_TRACK_AFTER_FINE_LOCK == true
		conf.fine = SYSCTRL_DFLL_TRACK_AFTER_FINE_LOCK;
		#else
		conf.fine = SYSCTRL_DFLL_FIX_AFTER_FINE_LOCK;
		#endif

		#if CONF_DFLL_KEEP_LOCK_ON_WAKEUP == true
		conf.law = SYSCTRL_DFLL_KEEP_LOCK_AFTER_WAKE;
		#else
		conf.law = SYSCTRL_DFLL_LOSE_LOCK_AFTER_WAKE;
		#endif

		#if CONF_DFLL_ENABLE_CHILL_CYCLE == true
		conf.chill_cycle = SYSCTRL_DFLL_CHILL_CYCLE_ENABLE;
		#else
		conf.chill_cycle = SYSCTRL_DFLL_CHILL_CYCLE_DISABLE;
		#endif

	conf.dfll_coarse_max_step = CONF_CLOCK_DFLL_MAX_COARSE_STEP_SIZE;
	conf.dfll_fine_max_step = CONF_CLOCK_DFLL_MAX_FINE_STEP_SIZE;

	clock_source_set_config(&conf, CLKSOURCE_DFLL);
	#endif /* CONF_CLOCK_DFLL_ENABLE */

	conf.rc8mhz_prescaler = CONF_CLOCK_RC8MHZ_PRESCALER;
	clock_source_set_config(&conf, CLKSOURCE_RC8MHZ);



	#if CONF_CLOCK_GCLK_0_ENABLE == true
	g_conf.source_channel = clock_gclk_get_generator_channel(
			CONF_CLOCK_GCLK_0_CLKSOURCE);
	g_conf.division_factor = CONF_CLOCK_GCLK_0_PRESCALER;
	clock_gclk_gen_set_config(&gclk_dev, 0, &g_conf);
	clock_gclk_gen_enable(&gclk_dev, 0);
	#endif

	#if CONF_CLOCK_GCLK_1_ENABLE == true
	g_conf.source_channel = clock_gclk_get_generator_channel(
			CONF_CLOCK_GCLK_1_CLKSOURCE);
	g_conf.division_factor = CONF_CLOCK_GCLK_1_PRESCALER;
	clock_gclk_gen_set_config(&gclk_dev, 1, &g_conf);
	clock_gclk_gen_enable(&gclk_dev, 1);
	#endif

	#if CONF_CLOCK_GCLK_2_ENABLE == true
	g_conf.source_channel = clock_gclk_get_generator_channel(
			CONF_CLOCK_GCLK_2_CLKSOURCE);
	g_conf.division_factor = CONF_CLOCK_GCLK_2_PRESCALER;
	clock_gclk_gen_set_config(&gclk_dev, 2, &g_conf);
	clock_gclk_gen_enable(&gclk_dev, 2);
	#endif

	#if CONF_CLOCK_GCLK_3_ENABLE == true
	g_conf.source_channel = clock_gclk_get_generator_channel(
			CONF_CLOCK_GCLK_3_CLKSOURCE);
	g_conf.division_factor = CONF_CLOCK_GCLK_3_PRESCALER;
	clock_gclk_gen_set_config(&gclk_dev, 3, &g_conf);
	clock_gclk_gen_enable(&gclk_dev, 3);
	#endif

	#if CONF_CLOCK_GCLK_4_ENABLE == true
	g_conf.source_channel = clock_gclk_get_generator_channel(
			CONF_CLOCK_GCLK_4_CLKSOURCE);
	g_conf.division_factor = CONF_CLOCK_GCLK_4_PRESCALER;
	clock_gclk_gen_set_config(&gclk_dev, 4, &g_conf);
	clock_gclk_gen_enable(&gclk_dev, 4);
	#endif

	#if CONF_CLOCK_GCLK_5_ENABLE == true
	g_conf.source_channel = clock_gclk_get_generator_channel(
			CONF_CLOCK_GCLK_5_CLKSOURCE);
	g_conf.division_factor = CONF_CLOCK_GCLK_5_PRESCALER;
	clock_gclk_gen_set_config(&gclk_dev, 5, &g_conf);
	clock_gclk_gen_enable(&gclk_dev, 5);
	#endif

	#if CONF_CLOCK_GCLK_6_ENABLE == true
	g_conf.source_channel = clock_gclk_get_generator_channel(
			CONF_CLOCK_GCLK_6_CLKSOURCE);
	g_conf.division_factor = CONF_CLOCK_GCLK_6_PRESCALER;
	clock_gclk_gen_set_config(&gclk_dev, 6, &g_conf);
	clock_gclk_gen_enable(&gclk_dev, 6);
	#endif

	#if CONF_CLOCK_GCLK_7_ENABLE == true
	g_conf.source_channel = clock_gclk_get_generator_channel(
			CONF_CLOCK_GCLK_7_CLKSOURCE);
	g_conf.division_factor = CONF_CLOCK_GCLK_7_PRESCALER;
	clock_gclk_gen_set_config(&gclk_dev, 7, &g_conf);
	clock_gclk_gen_enable(&gclk_dev, 7);
	#endif

	/* CPU and BUS clocks */
	clock_cpu_set_clocksource(CONF_CLOCK_CPU_CLOCK_SOURCE);
	clock_cpu_set_divider(CONF_CLOCK_CPU_DIVIDER);
	#if CONF_CLOCK_ENABLE_CPU_CLOCK_FAILURE_DETECT == true
	clock_main_clock_set_failure_detect(true);
	#else
	clock_main_clock_set_failure_detect(false);
	#endif
	
	clock_apba_set_divider(CONF_CLOCK_APBA_DIVIDER);
	clock_apbb_set_divider(CONF_CLOCK_APBB_DIVIDER);
	

}
