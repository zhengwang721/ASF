/**
 * \file
 *
 * \brief Instance description for PAC1
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#ifndef _SAMD20_PAC1_INSTANCE_
#define _SAMD20_PAC1_INSTANCE_

/* ========== Register definition for PAC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PAC1_WPCLR0                       (0x41000000U) /**< \brief (PAC1) Write Protection Clear Register 0 */
#define REG_PAC1_WPCLR1                       (0x41000004U) /**< \brief (PAC1) Write Protection Clear Register 1 */
#define REG_PAC1_WPCLR2                       (0x41000008U) /**< \brief (PAC1) Write Protection Clear Register 2 */
#define REG_PAC1_WPCLR3                       (0x4100000CU) /**< \brief (PAC1) Write Protection Clear Register 3 */
#define REG_PAC1_WPSET0                       (0x41000004U) /**< \brief (PAC1) Write Protection Set Register 0 */
#define REG_PAC1_WPSET1                       (0x41000008U) /**< \brief (PAC1) Write Protection Set Register 1 */
#define REG_PAC1_WPSET2                       (0x4100000CU) /**< \brief (PAC1) Write Protection Set Register 2 */
#define REG_PAC1_WPSET3                       (0x41000010U) /**< \brief (PAC1) Write Protection Set Register 3 */
#else
#define REG_PAC1_WPCLR             (*(RwReg  *)0x41000000U) /**< \brief (PAC1) Write Protection Clear Register */
#define REG_PAC1_WPSET             (*(RwReg  *)0x41000004U) /**< \brief (PAC1) Write Protection Set Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PAC1 peripheral ========== */
#define PAC1_WPROT_DEFAULT_VAL      0x00000002

#endif /* _SAMD20_PAC1_INSTANCE_ */
