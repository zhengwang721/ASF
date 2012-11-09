/**
 * \file
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
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
 
#ifndef __SAM0XX_COMPONENT_WDT__
#define __SAM0XX_COMPONENT_WDT__

#include <io.h>

/* WDT module */
typedef volatile struct {
	__REG8(CTRL, __IO);
	__REG8(WINCTRL, __IO);
	__REG8(EWCTRL, __IO);
	__REG8(reserved_0x03, __I);
	__REG8(INTENCLR, __IO);
	__REG8(INTENSET, __IO);
	__REG8(INTFLAG, __IO);
	__REG8(STATUS, __IO);
	__REG8(CLEAR, __IO);
} WDT_t;

/* WDT Register CTRL definitions */
#define WDT_ENABLE_bp       1
#define WDT_ENABLE_bm       (1UL << WDT_ENABLE_bp)
#define WDT_PER_gp          2
#define WDT_PER_gm          (0xFUL << WDT_PER_gp)
#define WDT_ALWAYSON_bp     7
#define WDT_ALWAYSON_bm     (1UL << WDT_ALWAYSON_bp)

/* WDT Register WINCTRL definitions */
#define WDT_WEN_bp          1
#define WDT_WEN_bm          (1UL << WDT_WEN_bp)
#define WDT_WINDOW_gp       2
#define WDT_WINDOW_gm       (0xFUL << WDT_WINDOW_gp)

/* WDT Register EWCTRL definitions */
#define WDT_EWOFFSET_gp     2
#define WDT_EWOFFSET_gm     (0xFUL << WDT_EWOFFSET_gp)

/* WDT Register INT* definitions */
#define WDT_EW_bp           0
#define WDT_EW_bm           (1UL << WDT_EW_bp)

/* WDT Register STATUS definitions */
#define WDT_SYNCBUSY_bp     0
#define WDT_SYNCBUSY_bm     (1UL << WDT_SYNCBUSY_bp)

/* WDT Register CLEAR definitions */
#define WDT_CLEAR_gp        0
#define WDT_CLEAR_gm        (0xFFUL << WDT_CLEAR_gp)

#endif /* __SAM0XX_COMPONENT_WDT__ */
