/**
 * \file
 *
 * \brief SAM0+ Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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
#ifndef SYSCTRL_HEADER_H_INCLUDED
#define SYSCTRL_HEADER_H_INCLUDED

/* SYSCTRL Register Map */
typedef struct {
	__REG32(INTENCLR, __O);
	__REG32(INTENSET, __O);
	__REG32(INTFLAG, __I);
	__REG32(PCLKSR, __IO);
	__REG16(XOSC, __IO);
	__REG8(RESERVED_0X12, __I);
	__REG8(RESERVED_0X13, __I);
	__REG16(XOSC32K, __IO);
	__REG8(XOSC32KTEST, __IO); // Not external ?
	__REG8(RESERVED_0X17, __I);
	__REG16(DFLLCTRL, __IO);
	__REG8(RESERVED_0X1A, __I);
	__REG8(RESERVED_0X1B, __I);
	__REG16(DFLLVAL, __IO);
	__REG16(DFLLDIFF, __IO);
	__REG16(DFLLMUL, __IO);
	__REG16(DFLLSTEP, __IO);
	__REG8(DFLLSYNC, __I);
	__REG8(RESERVED_0X25, __I);
	__REG8(RESERVED_0X26, __I);
	__REG8(RESERVED_0X27, __I);
	__REG16(BOD33CTRL, __IO);
	__REG16(BOD33LEVEL, __IO);
	__REG16(BOD12CTRL, __IO);
	__REG16(BOD12LEVEL, __IO);
	__REG32(OSC32K, __IO);
	__REG32(OSCULP32K, __IO);
	__REG8(OSCULP32KTEST, __IO); // Probably internal
	__REG32(OSC8M, __IO);
	__REG16(V__REGCTRL, __IO);
	__REG8(RESERVED_0X3E, __I);
	__REG8(RESERVED_0X3F, __I);
	__REG32(V__REGTEST, __IO); // Internal
	__REG16(VREFCTRLA, __IO);
	__REG16(VREFCTRLB, __IO);
	__REG32(VREFTESTA, __IO); // INTERNAL
	__REG16(VREFTESTB, __IO); // INTERNAL
	__REG8(RESERVED_0X4E, __I);
	__REG8(RESERVED_0X4F, __I);
	__REG32(TESTMODE, __IO);
} SYSCTRL_t;

#define SYSCTRL_MODE_bm 0
#define SYSCTRL_LLAW_bm 0
#define SYSCTRL_FINE_bm 0
#define SYSCTRL_CCDIS_bm 0
#define SYSCTRL_PRESC_bm 0
#define SYSCTRL_DFLLRDY_bm 0
#define SYSCTRL_SYNC_bm 0
#define SYSCTRL_OSC32KRDY_bm 0
#define SYSCTRL_CALIB_gp 0
#define SYSCTRL_FRANGE_gp 0
#define SYSCTRL_PRESC_gp 0
#define SYSCTRL_XTALEN_bm 0
#define SYSCTRL_AMPGC_bm 0
#define SYSCTRL_AAMPEN_bm 0
#define SYSCTRL_EN1K_bm 0
#define SYSCTRL_EN32K_bm 0
#define SYSCTRL_XOSCCRDY_bm 0
#define SYSCTRL_DFLL_ENABLE_bm 0
#define SYSCTRL_XOSC32KRDY_bm 0
#define SYSCTRL_XOSC32K_ENABLE_bm 0
#define SYSCTRL_XOSC_ENABLE_bm 0
#define SYSCTRL_OSC32K_ENABLE_bm 0
#define SYSCTRL_XOSC32K_ENABLE_bm 0
#define SYSCTRL_FIN_gp 0
#define SYSCTRL_DFLL_CLOSED_LOOP 0
#define SYSCTRL_FSTEP_gp 0
#define SYSCTRL_RC8MHZ_ENABLE 0
#define SYSCTRL_RC8MHZ_ENABLE_bm 0
#define SYSCTRL_XOSCRDY_bm 0
#define SYSCTRL_BOD33CTRL_ACTION_gp 0
#define SYSCTRL_BOD33CTRL_MODE_bp 0
#define SYSCTRL_BOD33CTRL_CEN_bm 0
#define SYSCTRL_BOD33CTRL_HYST_bm 0
#define SYSCTRL_BOD33CTRL_ENABLE_bm 0
#define SYSCTRL_ACTION_RESET_gc 0
#define SYSCTRL_ACTION_INTERRUPT_gc 0
#define SYSCTRL_PRESC_gm 0
#define SCR_SLEEPDEEP_bm 0


#endif /* SYSCTRL_HEADER_H_INCLUDED */
