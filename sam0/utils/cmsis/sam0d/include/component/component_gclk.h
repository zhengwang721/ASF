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
 
#ifndef __SAM0XX_COMPONENT_GCLK__
#define __SAM0XX_COMPONENT_GCLK__

#include <io.h>

/* GENCLK module */
typedef volatile struct {
	__REG8(CTRL, __IO);
	__REG8(reserved_0x01, __I);
	__REG16(CLKCTRL, __IO);
	__REG32(GENCTRL, __IO);
	__REG32(GENDIV, __IO);
	__REG32(reserved_0x0c, __I);
} GCLK_t;

/* GCLK Register CTRL definitions */
#define GCLK_CTRL_SWRST_bp         0
#define GCLK_CTRL_SWRST_bm         (1UL << GCLK_CTRL_SWRST_bp)

/* GCLK Register CLKCTRL definitions */
#define GCLK_CLKCTRL_CLKID_gp       0
#define GCLK_CLKCTRL_CLKID_gm       (0x3FUL << GCLK_CLKCTRL_CLKID_gp)
#define GCLK_CLKCTRL_GENID_gp       8
#define GCLK_CLKCTRL_GENID_gm       (0x1FUL << GCLK_CLKCTRL_GENID_gp)
#define GCLK_CLKCTRL_CEN_bp         14
#define GCLK_CLKCTRL_CEN_bm         (1UL << GCLK_CLKCTRL_CEN_bp)
#define GCLK_CLKCTRL_SLPMASK_bp     15
#define GCLK_CLKCTRL_SLPMASK_bm     (1UL << GCLK_CLKCTRL_SLPMASK_bp)

/* GCLK Register GENCTRL definitions */
#define GCLK_GENCTRL_GENID_gp       0
#define GCLK_GENCTRL_GENID_gm       (0x1FUL << GCLK_GENCTRL_GENID_gp)
#define GCLK_GENCTRL_SRCSEL_gp      8
#define GCLK_GENCTRL_SRCSEL_gm      (0x1FUL << GCLK_GENCTRL_SRCSEL_gp)
#define GCLK_GENCTRL_CEN_bp         16
#define GCLK_GENCTRL_CEN_bm         (1UL << GCLK_GENCTRL_CEN_bp)
#define GCLK_GENCTRL_IDC_bp         17
#define GCLK_GENCTRL_IDC_bm         (1UL << GCLK_GENCTRL_IDC_bp)
#define GCLK_GENCTRL_OOV_bp         18
#define GCLK_GENCTRL_OOV_bm         (1UL << GCLK_GENCTRL_OOV_bp)
#define GCLK_GENCTRL_DIVEN_bp       20
#define GCLK_GENCTRL_DIVEN_bm       (1UL << GCLK_GENCTRL_DIVEN_bp)
#define GCLK_GENCTRL_DIVFN_bp       21
#define GCLK_GENCTRL_DIVFN_bm       (1UL << GCLK_GENCTRL_DIVFN_bp)

/* GCLK Register GENDIV definitions */
#define GCLK_GENDIV_GENID_gp        0
#define GCLK_GENDIV_GENID_gm        (0x1FUL << GCLK_GENDIV_GENID_gp)
#define GCLK_GENDIV_DIV_gp          8
#define GCLK_GENDIV_DIV_gm          (0xFFFFFFUL << GCLK_GENDIV_DIV_gp)

#endif /* __SAM0XX_COMPONENT_GCLK__ */
