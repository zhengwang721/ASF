/**
 * \file
 *
 * \brief SAM TIMER Driver for SAMB11
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
#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <compiler.h>
#include <system.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*timer_callback_t)(void);

/**
 * \brief TIMER configuration structure.
 *
 * Configuration struct for a TIMER instance. This structure should be
 * initialized by the \ref timer_get_config_defaults function before being
 * modified by the user application.
 */
struct timer_config {
	uint32_t reload_value;
	/** Enable timer interrupt */
	bool interrupt_enable;
};

void timer_get_config_defaults(struct timer_config *config);
void timer_init(const struct timer_config *config);
uint32_t timer_get_value(void);
void timer_set_value(uint32_t value);
uint32_t timer_get_interrupt_status(void);
void timer_clear_interrupt_status(void);
void timer_enable(void);
void timer_disable(void);
void timer_register_callback(timer_callback_t fun);
void timer_unregister_callback(void);

#ifdef __cplusplus
}
#endif

#endif