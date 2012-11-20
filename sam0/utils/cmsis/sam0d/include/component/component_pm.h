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
#ifndef PM_HEADER_H_INCLUDED
#define PM_HEADER_H_INCLUDED
/* SYSCTRL Register Map */
typedef struct {
	__REG8(CTRL, __IO);
	__REG8(SLEEP, __IO);
	__REG8(RESERVED_0x2, __I);
	__REG8(RESERVED_0x3, __I);
	__REG8(RESERVED_0x4, __I);
	__REG8(RESERVED_0x5, __I);
	__REG8(RESERVED_0x6, __I);
	__REG8(RESERVED_0x7, __I);
	__REG8(CPUSEL, __IO);
	__REG8(APBASEL, __IO);
	__REG8(APBBSEL, __IO);
	__REG8(APBCSEL, __IO); // reserved ?
	__REG8(APBDSEL, __IO); // reserved ?
	__REG8(RESERVED_0xD, __I);
	__REG8(RESERVED_0xE, __I);
	__REG8(RESERVED_0xF, __I);
	__REG32(CPUMASK, __IO);
	__REG32(AHBMASK, __IO);
	__REG32(APBAMASK, __IO);
	__REG32(APBBMASK, __IO);
	__REG32(APBCMASK, __IO); //reserved ?
	__REG32(APBDMASK, __IO);
	__REG8(RESERVED_0x28, __I);
	__REG8(RESERVED_0x29, __I);
	__REG8(RESERVED_0x2A, __I);
	__REG8(RESERVED_0x2B, __I);
	__REG8(RESERVED_0x2C, __I);
	__REG32(RESERVED_0x30, __I); // Reserved ? crossed out in datasheet (SLEEPWALKCTRL)
	__REG8(INTENCRL, __O);
	__REG8(INTENSET, __O);
	__REG8(INTFLAG, __I);
	__REG8(RESERVED_0X37, __I);
	__REG8(RCAUSE, __I);
} PM_t;

#define PM_CFDEN_bm 0
#define PM_MCSEL_gm 0
#define PM_MCSEL_gp 0

#endif /* PM_HEADER_H_INCLUDED */
