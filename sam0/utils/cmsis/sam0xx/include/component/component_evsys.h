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
 
#ifndef __SAM0XX_COMPONENT_EVSYS__
#define __SAM0XX_COMPONENT_EVSYS__

#include <io.h>

/* EVSYS module */
typedef volatile struct {
	__REG8(CTRL, __IO);
	__REG8(reserved_0x01, __I);
	__REG8(reserved_0x02, __I);
	__REG8(reserved_0x03, __I);
	__REG32(CHCTRL, __IO);
	__REG32(USERMUX, __IO);
	__REG32(CHSTATUS, __I);
	__REG32(IENCLR, __IO);
	__REG32(IENSET, __IO);
	__REG32(INTFLAGS, __IO);
} EVSYS_t;

/* EVSYS Register CTRL definitions */
#define EVSYS_CTRL_SWRST_bp         0
#define EVSYS_CTRL_SWRST_bm         (1UL << EVSYS_CTRL_SWRST_bp)

/* EVSYS Register CHCTRL definitions */
#define EVSYS_CHCTRL_CHSEL_gp      0
#define EVSYS_CHCTRL_CHSEL_gm      (0xFFUL << EVSYS_CHCTRL_CHSEL_gp)
#define EVSYS_CHCTRL_STROBE_bp      8
#define EVSYS_CHCTRL_STROBE_bm      (1UL << EVSYS_CHCTRL_STROBE_bp)
#define EVSYS_CHCTRL_EVGSEL_gp     16
#define EVSYS_CHCTRL_EVGSEL_gm     (0xFFUL << EVSYS_CHCTRL_EVGSEL_gp)
#define EVSYS_CHCTRL_PATH_gp       24
#define EVSYS_CHCTRL_PATH_gm       (0x3UL << EVSYS_CHCTRL_PATH_gp)
#define EVSYS_CHCTRL_EDGESEL_gp    26
#define EVSYS_CHCTRL_EDGESEL_gm    (0x3UL << EVSYS_CHCTRL_EDGESEL_gp)

/* EVSYS Register USERMUX definitions */
#define EVSYS_USERMUX_UMUXSEL_gp    0
#define EVSYS_USERMUX_UMUXSEL_gm    (0xFFUL << EVSYS_USERMUX_UMUXSEL_gp)
#define EVSYS_USERMUX_CHANNELEVENT_gp  0
#define EVSYS_USERMUX_CHANNELEVENT_gm  (0xFFUL << EVSYS_USERMUX_CHANNELEVENT_gp)

#endif /* __SAM0XX_COMPONENT_EVSYS_ */
