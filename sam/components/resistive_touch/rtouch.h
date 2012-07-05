/**
 * \file
 *
 * \brief API driver for component ads7843.
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
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

#ifndef ADS7843_H_INCLUDED
#define ADS7843_H_INCLUDED

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#include "compiler.h"

//! Point definition.
typedef struct rtouch_point_struct
{
	uint16_t x;  //!< X coordinate.
	uint16_t y;  //!< Y coordinate.
} rtouch_point_t;

/**
 *  Info for one calibration point, raw samples and actual panel position.
 */
typedef struct rtouch_calibration_point_struct
{
	rtouch_point_t panel;  //!< Actual panel coordinate.
	rtouch_point_t raw;  //!< Raw sample value.
} rtouch_calibration_point_t;

/**
 * Collection of the five calibration points required to compute calibration.
 */
typedef struct rtouch_calibration_points_struct
{
	rtouch_calibration_point_t point1;
	rtouch_calibration_point_t point2;
	rtouch_calibration_point_t point3;
	rtouch_calibration_point_t point4;
	rtouch_calibration_point_t point5;
} rtouch_calibration_points_t;

/**
 * Event type used by rtouch_event_t.
 */
typedef enum rtouch_event_type_enum
{
	RTOUCH_PRESS,
	RTOUCH_MOVE,
	RTOUCH_RELEASE
} rtouch_event_type_t;

/**
 * User-friendly touch event structure.
 */
typedef struct rtouch_event_struct
{
	rtouch_event_type_t type;
	rtouch_point_t panel;  //!< Actual panel coordinate.
	rtouch_point_t raw;  //!< Raw sample value.
} rtouch_event_t;

typedef void (* rtouch_event_handler_t)(rtouch_event_t const * event);

uint32_t rtouch_init(const uint32_t ul_width, const uint32_t ul_height);
void rtouch_wait_pressed(void);
void rtouch_wait_released(void);
void rtouch_enable(void);
void rtouch_disable(void);
void rtouch_enable_detect_int(void);
void rtouch_disable_detect_int(void);
void rtouch_detect_int_handler(void);
uint32_t rtouch_compute_calibration(rtouch_calibration_point_t *points);
void rtouch_set_event_handler(rtouch_event_handler_t handler);
rtouch_event_handler_t rtouch_get_event_handler(void);
void rtouch_process(void);

/* EXPORTED FUNCTIONS */
extern void rtouch_get_raw_point(rtouch_point_t *p_point);
extern uint32_t rtouch_init_device(void);

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* ADS7843_H_INCLUDED */
