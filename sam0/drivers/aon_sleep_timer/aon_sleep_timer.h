/**
 * \file
 *
 * \brief SAM Always ON Sleep Timer Driver for SAMB11
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
#ifndef AON_SLEEP_TIMER_H_INCLUDED
#define AON_SLEEP_TIMER_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Specifies the counter mode is reload or single mode.
 *
 * This enum specifies counter with reload or single mode.
 */
enum aon_sleep_timer_mode {
	/** Counter in reload mode */
	AON_SLEEP_TIMER_RELOAD_MODE = 0,
	/** Counter in single mode */
	AON_SLEEP_TIMER_SINGLE_MODE,
};

/**
 * \brief Enable or disable ARM wakeup.
 *
 * This enum specifies ARM wakeup enable or disable.
 */
enum aon_sleep_timer_wakeup {
	/** Wakeup disable */
	AON_SLEEP_TIMER_WAKEUP_DIS = 0,
	/** ARM wakeup */
	AON_SLEEP_TIMER_WAKEUP_ARM,
	/** ARM and blue-tooth wakeup */
	AON_SLEEP_TIMER_WAKEUP_ARM_BLE,
};

/**
 *  \brief AON Sleep Timer configuration structure.
 *
 *  Configuration structure for a AON Sleep Timer instance. This structure should be
 *  initialized by the \ref aon_sleep_timer_get_config_defaults() function before being
 *  modified by the user application.
 */
struct aon_sleep_timer_config {
	/** Enable or disable ARM wakeup */
	enum aon_sleep_timer_wakeup wakeup;
	/** Counter is reload or single mode */
	enum aon_sleep_timer_mode mode;
	uint32_t counter;
};

void aon_sleep_timer_get_config_defaults(struct aon_sleep_timer_config *config);
void aon_sleep_timer_init(const struct aon_sleep_timer_config *config);
void aon_sleep_timer_disable(void);
uint32_t aon_sleep_timer_get_current_value(void);
void aon_sleep_timer_clear_interrup(void);
bool aon_sleep_timer_sleep_timer_active(void);

#ifdef __cplusplus
}
#endif

#endif