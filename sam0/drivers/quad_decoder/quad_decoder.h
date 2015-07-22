/**
 * \file
 *
 * \brief SAM QUAD DECODER Driver for SAMB11
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
#ifndef QUAD_DECODER_H_INCLUDED
#define QUAD_DECODER_H_INCLUDED

#include <compiler.h>
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief QUAD DECODER index number
 *
 * Values is the axis(x,y,z) of QUAD DECODER instance.
 */
enum quad_decoder_axis {
	QDEC_AXIS_X = 0,
	QDEC_AXIS_Y,
	QDEC_AXIS_Z,
};

/**
 * \brief QUAD DECODER module clock input 
 *
 * QUAD DECODER module clock.
 *
 */
enum quad_decoder_clock_input {
	/** Source from clock input 0 26MHz */
	QDEC_CLK_INPUT_0 = 0,
	/** Source from clock input 1 13MHz */
	QDEC_CLK_INPUT_1,
	/** Source from clock input 2 6.5MHz */
	QDEC_CLK_INPUT_2,
	/** Source from clock input 3 3MHz */
	QDEC_CLK_INPUT_3,
};

/**
 * \brief QUAD DECODER configuration structure.
 *
 * Configuration struct for a QUAD DECODER instance.
 * This structure should be initialized by the
 * \ref quad_decoder_get_config_defaults function
 * before being modified by the user application.
 */
struct quad_decoder_config {
	/** Quad Decoder Source Clock Frequency Select */
	enum quad_decoder_clock_input clock_sel; 
	/** Upper Threshold of counter for Quad Decoder */
	uint16_t threshold_upper;
	/** Lower Threshold of counter for Quad Decoder */
	uint16_t threshold_lower;
	/** Dec_in_a pinmux */
	/** Dec_in_b pinmux */
	uint32_t pinmux_pad[2];
	
};

void quad_decoder_get_config_defaults(
		struct quad_decoder_config *config);
void quad_decoder_init(enum quad_decoder_axis qdec,
		const struct quad_decoder_config *config);
void quad_decoder_enable(enum quad_decoder_axis qdec);
void quad_decoder_disable(enum quad_decoder_axis qdec);
uint8_t quad_decoder_get_irq_status(void);
void quad_decoder_clear_irq_status(enum quad_decoder_axis qdec);
uint16_t quad_decoder_get_counter(enum quad_decoder_axis qdec);
void quad_decoder_reset(enum quad_decoder_axis qdec);

#ifdef __cplusplus
}
#endif

#endif