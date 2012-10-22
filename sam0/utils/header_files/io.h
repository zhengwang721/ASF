/**
 * \file
 *
 * \brief Arch file for SAM0.
 *
 * This file defines common SAM0 series.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#ifndef _SAM_IO_
#define _SAM_IO_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define __I  volatile const
#define __O  volatile
#define __IO volatile

/* Default register macro for sub access of 32-bit registers */
#define __REG32(name, type) union { \
		type uint32_t name; \
		type uint16_t name ## _UINT16_T[2]; \
		type uint8_t name ## _UINT8_T[4]; \
}

/* Default register macro for sub access of arrays of 32-bit registers */
#define __REG32_ARRAY(name, elements, type) union { \
		type uint32_t name[elements]; \
		type uint16_t name ## _UINT16_T[elements][2]; \
		type uint8_t name ## _UINT8_T[elements][4]; \
}

/* Default register macro for sub access of 16-bit registers */
#define __REG16(name, type) union { \
		type uint16_t name; \
		type uint8_t name ## _UINT8_T[2]; \
}

/* Default register macro for sub access of arrays of 16-bit registers */
#define __REG16_ARRAY(name, elements, type) union { \
		type uint16_t name[elements]; \
		type uint8_t name ## _UINT8_T[elements][2]; \
}

/* Default register definition for 8-bit registers */
#define __REG8(name, type) type uint8_t name

/* Default register definition for arrays of 8-bit registers */
#define __REG8_ARRAY(name, elements, type) type uint8_t name[elements]


/* SAM0XX family */
#if (SAM0XX)
#  include "sam0xx.h"
#endif

#endif /* _SAM_IO_ */
