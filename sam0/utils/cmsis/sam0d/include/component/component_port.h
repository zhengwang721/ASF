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

#ifndef __SAM0XX_COMPONENT_PORT__
#define __SAM0XX_COMPONENT_PORT__

#include <io.h>

/* PORT module */
typedef volatile struct {
	__REG32(DIR, __IO);
	__REG32(DIRCLR, __IO);
	__REG32(DIRSET, __IO);
	__REG32(DIRTGL, __IO);
	__REG32(OUT, __IO);
	__REG32(OUTCLR, __IO);
	__REG32(OUTSET, __IO);
	__REG32(OUTTGL, __IO);
	__REG32(IN, __I);
	__REG32(CTRL, __IO);
	__REG32(MPCCTRL, __O);
	__REG32_ARRAY(PMUX, 16, __IO);
	__REG32_ARRAY(PINCFG, 16, __IO);
} PORT_t;

/* PORT Register CTRL definitions */
#define PORT_CTRL_SAMPLING_gp    0
#define PORT_CTRL_SAMPLING_bm    (0xFFFFUL << PORT_CTRL_SAMPLING_gp)

/* PORT Register MPCCTRL definitions */
#define PORT_MPCCTRL_PINMASK_gp  0
#define PORT_MPCCTRL_PINMASK_gm  (0xFFFFUL << PORT_MPCCTRL_PINMASK_gp)
#define PORT_MPCCTRL_CONFIG_gp   16
#define PORT_MPCCTRL_CONFIG_gm   (0xFFUL << PORT_MPCCTRL_CONFIG_gp)
#define PORT_MPCCTRL_PMUX_gp     24
#define PORT_MPCCTRL_PMUX_gm     (0xFUL << PORT_MPCCTRL_PMUX_gp)
#define PORT_MPCCTRL_WRPMUX_bp   28
#define PORT_MPCCTRL_WRPMUX_bm   (1UL << PORT_MPCCTRL_WRPMUX_bp)
#define PORT_MPCCTRL_WRPINCFG_bp 30
#define PORT_MPCCTRL_WRPINCFG_bm (1UL << PORT_MPCCTRL_WRPINCFG_bp)
#define PORT_MPCCTRL_HWSEL_bp    31
#define PORT_MPCCTRL_HWSEL_bm    (1UL << PORT_MPCCTRL_HWSEL_bp)

/* PORT Register PMUX definitions */
#define PORT_PMUX_PMUXE_gp       0
#define PORT_PMUX_PMUXE_gm       (0xFUL << PORT_PMUX_PMUXE_gp)
#define PORT_PMUX_PMUXO_gp       5
#define PORT_PMUX_PMUXO_gm       (0xFUL << PORT_PMUX_PMUXO_gp)

/* PORT Register PINCFG definitions */
#define PORT_PINCFG_PMUXEN_bp    0
#define PORT_PINCFG_PMUXEN_bm    (1UL << PORT_PINCFG_PMUXEN_bp)
#define PORT_PINCFG_INEN_bp      1
#define PORT_PINCFG_INEN_bm      (1UL << PORT_PINCFG_INEN_bp)
#define PORT_PINCFG_PULLEN_bp    2
#define PORT_PINCFG_PULLEN_bm    (1UL << PORT_PINCFG_PULLEN_bp)
#define PORT_PINCFG_ODRAIN_bp    3
#define PORT_PINCFG_ODRAIN_bm    (1UL << PORT_PINCFG_ODRAIN_bp)
#define PORT_PINCFG_SLEWLIM_bp   4
#define PORT_PINCFG_SLEWLIM_bm   (1UL << PORT_PINCFG_SLEWLIM_bp)
#define PORT_PINCFG_DRVSTR_bp    6
#define PORT_PINCFG_DRVSTR_bm    (1UL << PORT_PINCFG_DRVSTR_bp)

#endif /* __SAM0XX_COMPONENT_PORT__ */
