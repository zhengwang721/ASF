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

#ifndef __SAM0XX_COMPONENT_AC__
#define __SAM0XX_COMPONENT_AC__

#include <io.h>

/* WDT module */
typedef volatile struct {
	__REG8(CTRLA, __IO);
	__REG8(CTRLB, __IO);
	__REG16(EVCTRL, __IO);
	__REG8(INTENCLR, __IO);
	__REG8(INTENSET, __IO);
	__REG8(INTFLAG, __IO);
	__REG8(reserved_0x01, __I);
	__REG8(STATUSA, __IO);
	__REG8(STATUSB, __IO);
	__REG8(STATUSC, __IO);
	__REG8(reserved_0x02, __I);
	__REG8(WINCTRL, __IO);
	__REG8(reserved_0x03, __I);
	__REG16(reserved_0x04, __I);
	__REG32_ARRAY(COMPCTRL, 4, __IO);
	__REG8_ARRAY(SCALER, 4, __IO);
} AC_t;

/* AC Register CTRLA definitions */
#define AC_SWRST_bp         0
#define AC_SWRST_bm         (1UL << AC_SWRST_bp)
#define AC_ENABLE_bp        1
#define AC_ENABLE_bm        (1UL << AC_SLEEPEN_gp)
#define AC_SLEEPEN_gp       2
#define AC_SLEEPEN_gm       (0x3 << AC_SLEEPEN_gp)
#define AC_LPMUX_bp         7
#define AC_LPMUX_bm         (1UL << AC_LPMUX_bp)

/* AC Register CTRLB definitions */
#define AC_START0_bp        0
#define AC_START0_bm        (1UL << AC_START0_bp)
#define AC_START1_bp        1
#define AC_START1_bm        (1UL << AC_START1_bp)
#define AC_START2_bp        2
#define AC_START2_bm        (1UL << AC_START2_bp)
#define AC_START3_bp        3
#define AC_START3_bm        (1UL << AC_START3_bp)

/* AC Register EVCTRL definitions */
#define AC_COMPEO0_bp       0
#define AC_COMPEO0_bm       (1UL << AC_COMPEO0_bp)
#define AC_COMPEO1_bp       1
#define AC_COMPEO1_bm       (1UL << AC_COMPEO1_bp)
#define AC_COMPEO2_bp       2
#define AC_COMPEO2_bm       (1UL << AC_COMPEO2_bp)
#define AC_COMPEO3_bp       3
#define AC_COMPEO3_bm       (1UL << AC_COMPEO3_bp)
#define AC_WINEO0_bp        4
#define AC_WINEO0_bm        (1UL << AC_WINEO0_bp)
#define AC_WINEO1_bp        5
#define AC_WINEO1_bm        (1UL << AC_WINEO1_bp)
#define AC_COMPEI0_bp       8
#define AC_COMPEI0_bm       (1UL << AC_COMPEI0_bp)
#define AC_COMPEI1_bp       9
#define AC_COMPEI1_bm       (1UL << AC_COMPEI1_bp)
#define AC_COMPEI2_bp       10
#define AC_COMPEI2_bm       (1UL << AC_COMPEI2_bp)
#define AC_COMPEI3_bp       11
#define AC_COMPEI3_bm       (1UL << AC_COMPEI3_bp)

/* AC Register INTx definitions */
#define AC_COMP0_bp         0
#define AC_COMP0_bm         (1UL << AC_COMP0_bp)
#define AC_COMP1_bp         1
#define AC_COMP1_bm         (1UL << AC_COMP1_bp)
#define AC_COMP2_bp         2
#define AC_COMP2_bm         (1UL << AC_COMP2_bp)
#define AC_COMP3_bp         3
#define AC_COMP3_bm         (1UL << AC_COMP3_bp)
#define AC_WIN0_bp          4
#define AC_WIN0_bm          (1UL << AC_WIN0_bp)
#define AC_WIN1_bp          5
#define AC_WIN1_bm          (1UL << AC_WIN1_bp)

/* AC Register STATUSA/STATUSC definitions */
#define AC_STATE0_bp        0
#define AC_STATE0_bm        (1UL << AC_STATE0_bp)
#define AC_STATE1_bp        1
#define AC_STATE1_bm        (1UL << AC_STATE1_bp)
#define AC_STATE2_bp        2
#define AC_STATE2_bm        (1UL << AC_STATE2_bp)
#define AC_STATE3_bp        3
#define AC_STATE3_bm        (1UL << AC_STATE3_bp)
#define AC_WSTATE0_gp       4
#define AC_WSTATE0_gm       (0x2 << AC_WSTATE0_gp)
#define AC_WSTATE1_gp       6
#define AC_WSTATE1_gm       (0x2 << AC_WSTATE1_gp)

/* AC Register STATUSB definitions */
#define AC_READY0_bp        0
#define AC_READY0_bm        (1UL << AC_READY0_bp)
#define AC_READY1_bp        1
#define AC_READY1_bm        (1UL << AC_READY1_bp)
#define AC_READY2_bp        2
#define AC_READY2_bm        (1UL << AC_READY2_bp)
#define AC_READY3_bp        3
#define AC_READY3_bm        (1UL << AC_READY3_bp)
#define AC_SYNCBUSY_bp      7
#define AC_SYNCBUSY_bm      (1UL << AC_SYNCBUSY_bp)

/* AC Register WINCTRL definitions */
#define AC_WEN0_bp          0
#define AC_WEN0_bm          (1UL << AC_WEN0_bp)
#define AC_WINTSEL0_gp      1
#define AC_WINTSEL0_gm      (0x3 << AC_WINTSEL0_gp)
#define AC_WEN1_bp          5
#define AC_WEN1_bm          (1UL << AC_WEN1_bp)
#define AC_WINTSEL1_gp      6
#define AC_WINTSEL1_gm      (0x3 << AC_WINTSEL1_gp)

/* AC Register COMPCTRLx definitions */
#define AC_CH_ENABLE_bp     0
#define AC_CH_ENABLE_bm     (1UL << AC_CH_ENABLE_bp)
#define AC_SINGLE_bp        1
#define AC_SINGLE_bm        (1UL << AC_SINGLE_bp)
#define AC_SPEED_gp         2
#define AC_SPEED_gm         (0x3 << AC_SPEED_gp)
#define AC_INTSEL_gp        5
#define AC_INTSEL_gm        (0x3 << AC_INTSEL_gp)
#define AC_MUXNEG_gp        8
#define AC_MUXNEG_gm        (0x7 << AC_MUXNEG_gp)
#define AC_MUXPOS_gp        12
#define AC_MUXPOS_gm        (0x7 << AC_MUXPOS_gp)
#define AC_SWAP_bp          15
#define AC_SWAP_bm          (1UL << AC_SWAP_bp)
#define AC_OUT_gp           16
#define AC_OUT_gm           (0x3 << AC_OUT_gp)
#define AC_HYST_bp          19
#define AC_HYST_bm          (1UL << AC_HYST_bp)
#define AC_FLEN_gp          24
#define AC_FLEN_gm          (0x7 << AC_FLEN_gp)

/* AC Register SCALARx definitions */
#define AC_VALUE_gp         0
#define AC_VALUE_gm         (0x3 << AC_VALUE_gp)

#define AC_MUXPOS_PIN0_gc     0
#define AC_MUXPOS_PIN1_gc     1
#define AC_MUXPOS_PIN2_gc     2
#define AC_MUXPOS_PIN3_gc     3
#define AC_MUXNEG_PIN0_gc     0
#define AC_MUXNEG_PIN1_gc     1
#define AC_MUXNEG_PIN2_gc     2
#define AC_MUXNEG_PIN3_gc     3
#define AC_MUXNEG_GND_gc      4
#define AC_MUXNEG_VSCALE_gc   5
#define AC_MUXNEG_BANDGAP_gc  6
#define AC_MUXNEG_DAC_gc      7

#define AC_FLEN_OFF_gc        0
#define AC_FLEN_MAJ3_gc       1
#define AC_FLEN_MAJ5_gc       2

#define AC_OUT_OFF_gc         0
#define AC_OUT_ASYNC_gc       1
#define AC_OUT_SYNC_gc        2

#define AC_WINTSEL_ABOVE_gc   0
#define AC_WINTSEL_INSIDE_gc  1
#define AC_WINTSEL_BELOW_gc   2
#define AC_WINTSEL_OUTSIDE_gc 3

#define AC_WSTATE_ABOVE_gc    0
#define AC_WSTATE_INSIDE_gc   1
#define AC_WSTATE_BELOW_gc    2

#endif /* __SAM0XX_COMPONENT_AC__ */
