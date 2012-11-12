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
#define EVSYS_SWRST_bp         0
#define EVSYS_SWRST_bm         (1UL << EVSYS_SWRST_bp)

/* EVSYS Register CHCTRL definitions */
#define EVSYS_CHSEL_gp         0
#define EVSYS_CHSEL_gm         (0xFFUL << EVSYS_CHSEL_gp)
#define EVSYS_STROBE_bp        8
#define EVSYS_STROBE_bm        (1UL << EVSYS_STROBE_bp)
#define EVSYS_EVGSEL_gp        16
#define EVSYS_EVGSEL_gm        (0xFFUL << EVSYS_EVGSEL_gp)
#define EVSYS_PATH_gp          24
#define EVSYS_PATH_gm          (0x3UL << EVSYS_PATH_gp)
#define EVSYS_EDGESEL_gp       26
#define EVSYS_EDGESEL_gm       (0x3UL << EVSYS_EDGESEL_gp)

/* EVSYS Register USERMUX definitions */
#define EVSYS_UMUXSEL_gp       0
#define EVSYS_UMUXSEL_gm       (0xFFUL << EVSYS_UMUXSEL_gp)
#define EVSYS_CHANNELEVENT_gp  0
#define EVSYS_CHANNELEVENT_gm  (0xFFUL << EVSYS_CHANNELEVENT_gp)

/* EVSYS Register CHSTATUS definitions */
#define EVSYS_USRREADY0_bp     0
#define EVSYS_USRREADY0_bm     (1UL << EVSYS_USRREADY0_bp)
#define EVSYS_USRREADY1_bp     1
#define EVSYS_USRREADY1_bm     (1UL << EVSYS_USRREADY1_bp)
#define EVSYS_USRREADY2_bp     2
#define EVSYS_USRREADY2_bm     (1UL << EVSYS_USRREADY2_bp)
#define EVSYS_USRREADY3_bp     3
#define EVSYS_USRREADY3_bm     (1UL << EVSYS_USRREADY3_bp)
#define EVSYS_USRREADY4_bp     4
#define EVSYS_USRREADY4_bm     (1UL << EVSYS_USRREADY4_bp)
#define EVSYS_USRREADY5_bp     5
#define EVSYS_USRREADY5_bm     (1UL << EVSYS_USRREADY5_bp)
#define EVSYS_USRREADY6_bp     6
#define EVSYS_USRREADY6_bm     (1UL << EVSYS_USRREADY6_bp)
#define EVSYS_USRREADY7_bp     7
#define EVSYS_USRREADY7_bm     (1UL << EVSYS_USRREADY7_bp)
#define EVSYS_CHBUSY0_bp       8
#define EVSYS_CHBUSY0_bm       (1UL << EVSYS_CHBUSY0_bp)
#define EVSYS_CHBUSY1_bp       9
#define EVSYS_CHBUSY1_bm       (1UL << EVSYS_CHBUSY1_bp)
#define EVSYS_CHBUSY2_bp       10
#define EVSYS_CHBUSY2_bm       (1UL << EVSYS_CHBUSY2_bp)
#define EVSYS_CHBUSY3_bp       11
#define EVSYS_CHBUSY3_bm       (1UL << EVSYS_CHBUSY3_bp)
#define EVSYS_CHBUSY4_bp       12
#define EVSYS_CHBUSY4_bm       (1UL << EVSYS_CHBUSY4_bp)
#define EVSYS_CHBUSY5_bp       13
#define EVSYS_CHBUSY5_bm       (1UL << EVSYS_CHBUSY5_bp)
#define EVSYS_CHBUSY6_bp       14
#define EVSYS_CHBUSY6_bm       (1UL << EVSYS_CHBUSY6_bp)
#define EVSYS_CHBUSY7_bp       15
#define EVSYS_CHBUSY7_bm       (1UL << EVSYS_CHBUSY7_bp)
#define EVSYS_USRREADY8_bp     16
#define EVSYS_USRREADY8_bm     (1UL << EVSYS_USRREADY8_bp)
#define EVSYS_USRREADY9_bp     17
#define EVSYS_USRREADY9_bm     (1UL << EVSYS_USRREADY9_bp)
#define EVSYS_USRREADY10_bp    18
#define EVSYS_USRREADY10_bm    (1UL << EVSYS_USRREADY10_bp)
#define EVSYS_USRREADY11_bp    19
#define EVSYS_USRREADY11_bm    (1UL << EVSYS_USRREADY11_bp)
#define EVSYS_USRREADY12_bp    20
#define EVSYS_USRREADY12_bm    (1UL << EVSYS_USRREADY12_bp)
#define EVSYS_USRREADY13_bp    21
#define EVSYS_USRREADY13_bm    (1UL << EVSYS_USRREADY13_bp)
#define EVSYS_USRREADY14_bp    22
#define EVSYS_USRREADY14_bm    (1UL << EVSYS_USRREADY14_bp)
#define EVSYS_USRREADY15_bp    23
#define EVSYS_USRREADY15_bm    (1UL << EVSYS_USRREADY15_bp)
#define EVSYS_CHBUSY8_bp       24
#define EVSYS_CHBUSY8_bm       (1UL << EVSYS_CHBUSY8_bp)
#define EVSYS_CHBUSY9_bp       25
#define EVSYS_CHBUSY9_bm       (1UL << EVSYS_CHBUSY9_bp)
#define EVSYS_CHBUSY10_bp      26
#define EVSYS_CHBUSY10_bm      (1UL << EVSYS_CHBUSY10_bp)
#define EVSYS_CHBUSY11_bp      27
#define EVSYS_CHBUSY11_bm      (1UL << EVSYS_CHBUSY11_bp)
#define EVSYS_CHBUSY12_bp      28
#define EVSYS_CHBUSY12_bm      (1UL << EVSYS_CHBUSY12_bp)
#define EVSYS_CHBUSY13_bp      29
#define EVSYS_CHBUSY13_bm      (1UL << EVSYS_CHBUSY13_bp)
#define EVSYS_CHBUSY14_bp      30
#define EVSYS_CHBUSY14_bm      (1UL << EVSYS_CHBUSY14_bp)
#define EVSYS_CHBUSY15_bp      31
#define EVSYS_CHBUSY15_bm      (1UL << EVSYS_CHBUSY15_bp)

#endif /* __SAM0XX_COMPONENT_EVSYS_ */
