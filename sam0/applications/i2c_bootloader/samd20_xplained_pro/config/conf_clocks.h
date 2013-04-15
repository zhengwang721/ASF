/**
 * \file
 *
 * \brief SAMD20 Clock configuration
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
#include <clock.h>

#ifndef CONF_CLOCKS_H
#  define CONF_CLOCKS_H

#define CONF_CLOCK_CPU_CLOCK_SOURCE SYSTEM_MAIN_CLOCK_OSC8M
#define CONF_CLOCK_CPU_DIVIDER SYSTEM_MAIN_CLOCK_DIV_1
#define CONF_CLOCK_ENABLE_CPU_CLOCK_FAILURE_DETECT false

#define CONF_CLOCK_APBA_DIVIDER SYSTEM_MAIN_CLOCK_DIV_1
#define CONF_CLOCK_APBB_DIVIDER SYSTEM_MAIN_CLOCK_DIV_1

/*
 * Configure clock sources when running clocks_init. If this is false none
 * of the settings below will be applied, and the hardware defaults will be used
 */
#define CONF_CLOCK_CONFIGURE_CLOCK_SOURCES true


 /* SYSTEM_CLOCK_SOURCE_OSC8M configuration */
#define CONF_CLOCK_OSC8M_PRESCALER SYSTEM_OSC8M_DIV_0

/* SYSTEM_CLOCK_SOURCE_XOSC External oscillator/clock configuration */

#define CONF_CLOCK_XOSC_ENABLE false
#define CONF_CLOCK_XOSC_EXTERNAL_CRYSTAL SYSTEM_CLOCK_EXTERNAL_CRYSTAL
#define CONF_CLOCK_XOSC_EXTERNAL_FREQUENCY 12000000UL
#define CONF_CLOCK_XOSC_STARTUP_TIME SYSTEM_XOSC_STARTUP_32768
#define CONF_CLOCK_XOSC_AUTO_GAIN_CONTROL true

/* SYSTEM_CLOCK_SOURCE_XOSC32K Configuration - External 32KHz crystal/clock oscillator */

#define CONF_CLOCK_XOSC32K_ENABLE false
#define CONF_CLOCK_XOSC32K_EXTERNAL_CRYSTAL SYSTEM_CLOCK_EXTERNAL_CRYSTAL
#define CONF_CLOCK_XOSC32K_STARTUP_TIME    SYSTEM_XOSC32K_STARTUP_65536
#define CONF_CLOCK_XOSC32K_AUTO_AMPLITUDE_CONTROL true
#define CONF_CLOCK_XOSC32K_ENABLE_1KHZ_OUPUT false
#define CONF_CLOCK_XOSC32K_ENABLE_32KHZ_OUTPUT true

/* SYSTEM_CLOCK_SOURCE_OSC32K Configuration - Internal 32KHz Oscillator */
/* This is enabled for the Watchdog timer */
#define CONF_CLOCK_OSC32K_ENABLE true
#define CONF_CLOCK_OSC32K_STARTUP_TIME  SYSTEM_OSC32K_STARTUP_128
#define CONF_CLOCK_OSC32K_ENABLE_1KHZ_OUTPUT true
#define CONF_CLOCK_OSC32K_ENABLE_32KHZ_OUTPUT true

/* Set up GCLK generator 2 for RTC */

#define CONF_CLOCK_GCLK_2_RTC false
#define CONF_CLOCK_RTC_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_OSC32K
#define CONF_CLOCK_RTC_FREQ CONF_CLOCK_RTC_FREQ_1HZ

/* SYSTEM_CLOCK_SOURCE_DFLL Configuration - Digital Frequency locked loop */

#define CONF_CLOCK_DFLL_ENABLE false
#define CONF_CLOCK_DFLL_LOOP_MODE SYSTEM_CLOCK_DFLL_OPEN_LOOP

/* DFLL open loop mode configuration */
#define CONF_CLOCK_DFLL_48MHZ_CALIBRATE false
#define CONF_CLOCK_DFLL_COARSE_VALUE (0x1f/4)
#define CONF_CLOCK_DFLL_FINE_VALUE (0xff/4)

/* DFLL Closed loop mode configuration */
#define CONF_CLOCK_DFLL_SOURCE_GCLK_GENERATOR GCLK_GENERATOR_6
#define CONF_CLOCK_DFLL_MULTIPLY_FACTOR 7
#define CONF_CLOCK_DFLL_QUICK_LOCK true
#define CONF_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK true
#define CONF_CLOCK_DFLL_KEEP_LOCK_ON_WAKEUP true
#define CONF_CLOCK_DFLL_ENABLE_CHILL_CYCLE true
#define CONF_CLOCK_DFLL_MAX_COARSE_STEP_SIZE (0x1f/4)
#define CONF_CLOCK_DFLL_MAX_FINE_STEP_SIZE (0xff/4)


/* Generic clocks (GCLK) */


/* Set this to true to configure the GCLK when running clocks_init. If set to
 * false none of the GCLK generators will be configured in clocks_init. */
#define CONF_CLOCK_CONFIGURE_GCLK true

/* Configure GCLK generator 0 (8MHZ div 8 at startup (CPU CLOCK)) */

#define CONF_CLOCK_GCLK_0_ENABLE true
#define CONF_CLOCK_GCLK_0_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_OSC8M
#define CONF_CLOCK_GCLK_0_PRESCALER 3

/* Configure GCLK generator 1 */

#define CONF_CLOCK_GCLK_1_ENABLE false
#define CONF_CLOCK_GCLK_1_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_OSC8M
#define CONF_CLOCK_GCLK_1_PRESCALER 1

/* Configure GCLK generator 2*/
#if CONF_CLOCK_GCLK_2_RTC == true
/* This section is configured in the RTC configuration section given above. */
# define CONF_CLOCK_GCLK_2_ENABLE true
# define CONF_CLOCK_GCLK_2_CLOCK_SOURCE CONF_CLOCK_RTC_CLOCK_SOURCE
# define CONF_CLOCK_GCLK_2_PRESCALER CONF_CLOCK_RTC_FREQ
#else
/* Configure these settings when GCLK 2 is not used for RTC or manual settings
 * are preferred. */
# define CONF_CLOCK_GCLK_2_ENABLE false
# define CONF_CLOCK_GCLK_2_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_OSC8M
# define CONF_CLOCK_GCLK_2_PRESCALER 1
#endif

/* Configure GCLK generator 3*/

#define CONF_CLOCK_GCLK_3_ENABLE false
#define CONF_CLOCK_GCLK_3_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_OSC8M
#define CONF_CLOCK_GCLK_3_PRESCALER 1

/* Configure GCLK generator 4 */

#define CONF_CLOCK_GCLK_4_ENABLE true
#define CONF_CLOCK_GCLK_4_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_OSC32K
#define CONF_CLOCK_GCLK_4_PRESCALER 1

/* Configure GCLK generator 5*/

#define CONF_CLOCK_GCLK_5_ENABLE false
#define CONF_CLOCK_GCLK_5_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_OSC8M
#define CONF_CLOCK_GCLK_5_PRESCALER 1

/* Configure GCLK generator 6*/

#define CONF_CLOCK_GCLK_6_ENABLE false
#define CONF_CLOCK_GCLK_6_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_XOSC32K
#define CONF_CLOCK_GCLK_6_PRESCALER 2

/* Configure GCLK generator 7*/

#define CONF_CLOCK_GCLK_7_ENABLE false
#define CONF_CLOCK_GCLK_7_CLOCK_SOURCE SYSTEM_CLOCK_SOURCE_OSC8M
#define CONF_CLOCK_GCLK_7_PRESCALER 1


#endif /* CONF_CLOCKS_H */
