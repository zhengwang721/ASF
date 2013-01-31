/**
 * \file
 *
 * \brief Instance description for PAC2
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

#ifndef _SAMD20_PAC2_INSTANCE_
#define _SAMD20_PAC2_INSTANCE_

/* ========== Register definition for PAC2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PAC2_WPCLR0                       (0x42000000U) /**< \brief (PAC2) Write Protection Clear Register 0 */
#define REG_PAC2_WPCLR1                       (0x42000004U) /**< \brief (PAC2) Write Protection Clear Register 1 */
#define REG_PAC2_WPCLR2                       (0x42000008U) /**< \brief (PAC2) Write Protection Clear Register 2 */
#define REG_PAC2_WPCLR3                       (0x4200000CU) /**< \brief (PAC2) Write Protection Clear Register 3 */
#define REG_PAC2_WPCLR4                       (0x42000010U) /**< \brief (PAC2) Write Protection Clear Register 4 */
#define REG_PAC2_WPCLR5                       (0x42000014U) /**< \brief (PAC2) Write Protection Clear Register 5 */
#define REG_PAC2_WPCLR6                       (0x42000018U) /**< \brief (PAC2) Write Protection Clear Register 6 */
#define REG_PAC2_WPCLR7                       (0x4200001CU) /**< \brief (PAC2) Write Protection Clear Register 7 */
#define REG_PAC2_WPCLR8                       (0x42000020U) /**< \brief (PAC2) Write Protection Clear Register 8 */
#define REG_PAC2_WPCLR9                       (0x42000024U) /**< \brief (PAC2) Write Protection Clear Register 9 */
#define REG_PAC2_WPCLR10                      (0x42000028U) /**< \brief (PAC2) Write Protection Clear Register 10 */
#define REG_PAC2_WPCLR11                      (0x4200002CU) /**< \brief (PAC2) Write Protection Clear Register 11 */
#define REG_PAC2_WPCLR12                      (0x42000030U) /**< \brief (PAC2) Write Protection Clear Register 12 */
#define REG_PAC2_WPCLR13                      (0x42000034U) /**< \brief (PAC2) Write Protection Clear Register 13 */
#define REG_PAC2_WPCLR14                      (0x42000038U) /**< \brief (PAC2) Write Protection Clear Register 14 */
#define REG_PAC2_WPCLR15                      (0x4200003CU) /**< \brief (PAC2) Write Protection Clear Register 15 */
#define REG_PAC2_WPSET0                       (0x42000004U) /**< \brief (PAC2) Write Protection Set Register 0 */
#define REG_PAC2_WPSET1                       (0x42000008U) /**< \brief (PAC2) Write Protection Set Register 1 */
#define REG_PAC2_WPSET2                       (0x4200000CU) /**< \brief (PAC2) Write Protection Set Register 2 */
#define REG_PAC2_WPSET3                       (0x42000010U) /**< \brief (PAC2) Write Protection Set Register 3 */
#define REG_PAC2_WPSET4                       (0x42000014U) /**< \brief (PAC2) Write Protection Set Register 4 */
#define REG_PAC2_WPSET5                       (0x42000018U) /**< \brief (PAC2) Write Protection Set Register 5 */
#define REG_PAC2_WPSET6                       (0x4200001CU) /**< \brief (PAC2) Write Protection Set Register 6 */
#define REG_PAC2_WPSET7                       (0x42000020U) /**< \brief (PAC2) Write Protection Set Register 7 */
#define REG_PAC2_WPSET8                       (0x42000024U) /**< \brief (PAC2) Write Protection Set Register 8 */
#define REG_PAC2_WPSET9                       (0x42000028U) /**< \brief (PAC2) Write Protection Set Register 9 */
#define REG_PAC2_WPSET10                      (0x4200002CU) /**< \brief (PAC2) Write Protection Set Register 10 */
#define REG_PAC2_WPSET11                      (0x42000030U) /**< \brief (PAC2) Write Protection Set Register 11 */
#define REG_PAC2_WPSET12                      (0x42000034U) /**< \brief (PAC2) Write Protection Set Register 12 */
#define REG_PAC2_WPSET13                      (0x42000038U) /**< \brief (PAC2) Write Protection Set Register 13 */
#define REG_PAC2_WPSET14                      (0x4200003CU) /**< \brief (PAC2) Write Protection Set Register 14 */
#define REG_PAC2_WPSET15                      (0x42000040U) /**< \brief (PAC2) Write Protection Set Register 15 */
#else
#define REG_PAC2_WPCLR             (*(RwReg  *)0x42000000U) /**< \brief (PAC2) Write Protection Clear Register */
#define REG_PAC2_WPSET             (*(RwReg  *)0x42000004U) /**< \brief (PAC2) Write Protection Set Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PAC2 peripheral ========== */
#define PAC2_WPROT_DEFAULT_VAL      0x00100000

#endif /* _SAMD20_PAC2_INSTANCE_ */
