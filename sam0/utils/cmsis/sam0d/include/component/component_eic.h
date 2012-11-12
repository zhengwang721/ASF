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

#ifndef __SAM0XX_COMPONENT_EIC__
#define __SAM0XX_COMPONENT_EIC__

#include <io.h>

/* EIC module */
typedef volatile struct {
	__REG8(CTRL, __IO);
	__REG8(reserved_0x01, __I);
	__REG8(reserved_0x02, __I);
	__REG8(reserved_0x03, __I);
	__REG32(EVCTRL, __IO);
	__REG32(INTENCLR, __IO);
	__REG32(INTENSET, __IO);
	__REG32(INTFLAG, __IO);
	__REG32(WAKEUP, __IO);
	__REG32_ARRAY(CONFIG, 4, __IO);
} EIC_t;

/* EIC Register CTRL definitions */
#define EIC_CTRL_SWRST_bp          0
#define EIC_CTRL_SWRST_bm          (1UL << EIC_CTRL_SWRST_bp)
#define EIC_CTRL_ENABLE_bp         1
#define EIC_CTRL_ENABLE_bm         (1UL << EIC_CTRL_ENABLE_bp)
#define EIC_CTRL_NMIDETECTMODE_gp  5
#define EIC_CTRL_NMIDETECTMODE_gm  (0x7UL << EIC_CTRL_NMIDETECTMODE_gp)

/* EIC Register CONFIG definitions */
#define EIC_CONFIG_DETECTMODE0_gp  0
#define EIC_CONFIG_DETECTMODE0_gm  (0x7UL << EIC_CONFIG_DETECTMODE0_gp)
#define EIC_CONFIG_FILTEN0_bp      3
#define EIC_CONFIG_FILTEN0_bm      (1UL << EIC_CONFIG_FILTEN0_bp)
#define EIC_CONFIG_DETECTMODE1_gp  4
#define EIC_CONFIG_DETECTMODE1_gm  (0x7UL << EIC_CONFIG_DETECTMODE1_gp)
#define EIC_CONFIG_FILTEN1_bp      7
#define EIC_CONFIG_FILTEN1_bm      (1UL << EIC_CONFIG_FILTEN1_bp)
#define EIC_CONFIG_DETECTMODE2_gp  8
#define EIC_CONFIG_DETECTMODE2_gm  (0x7UL << EIC_CONFIG_DETECTMODE2_gp)
#define EIC_CONFIG_FILTEN2_bp      11
#define EIC_CONFIG_FILTEN2_bm      (1UL << EIC_CONFIG_FILTEN2_bp)
#define EIC_CONFIG_DETECTMODE3_gp  12
#define EIC_CONFIG_DETECTMODE3_gm  (0x7UL << EIC_CONFIG_DETECTMODE3_gp)
#define EIC_CONFIG_FILTEN3_bp      15
#define EIC_CONFIG_FILTEN3_bm      (1UL << EIC_CONFIG_FILTEN3_bp)
#define EIC_CONFIG_DETECTMODE4_gp  16
#define EIC_CONFIG_DETECTMODE4_gm  (0x7UL << EIC_CONFIG_DETECTMODE4_gp)
#define EIC_CONFIG_FILTEN4_bp      19
#define EIC_CONFIG_FILTEN4_bm      (1UL << EIC_CONFIG_FILTEN4_bp)
#define EIC_CONFIG_DETECTMODE5_gp  20
#define EIC_CONFIG_DETECTMODE5_gm  (0x7UL << EIC_CONFIG_DETECTMODE5_gp)
#define EIC_CONFIG_FILTEN5_bp      23
#define EIC_CONFIG_FILTEN5_bm      (1UL << EIC_CONFIG_FILTEN5_bp)
#define EIC_CONFIG_DETECTMODE6_gp  24
#define EIC_CONFIG_DETECTMODE6_gm  (0x7UL << EIC_CONFIG_DETECTMODE6_gp)
#define EIC_CONFIG_FILTEN6_bp      27
#define EIC_CONFIG_FILTEN6_bm      (1UL << EIC_CONFIG_FILTEN6_bp)
#define EIC_CONFIG_DETECTMODE7_gp  30
#define EIC_CONFIG_DETECTMODE7_gm  (0x7UL << EIC_CONFIG_DETECTMODE7_gp)
#define EIC_CONFIG_FILTEN7_bp      31
#define EIC_CONFIG_FILTEN7_bm      (1UL << EIC_CONFIG_FILTEN7_bp)

#endif /* __SAM0XX_COMPONENT_EIC__ */
