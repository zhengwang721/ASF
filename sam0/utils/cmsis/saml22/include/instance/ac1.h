/**
 * \file
 *
 * \brief Instance description for AC1
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

#ifndef _SAML22_AC1_INSTANCE_
#define _SAML22_AC1_INSTANCE_

/* ========== Register definition for AC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_AC1_CTRLA              (0x42004400U) /**< \brief (AC1) Control A */
#define REG_AC1_CTRLB              (0x42004401U) /**< \brief (AC1) Control B */
#define REG_AC1_EVCTRL             (0x42004402U) /**< \brief (AC1) Event Control */
#define REG_AC1_INTENCLR           (0x42004404U) /**< \brief (AC1) Interrupt Enable Clear */
#define REG_AC1_INTENSET           (0x42004405U) /**< \brief (AC1) Interrupt Enable Set */
#define REG_AC1_INTFLAG            (0x42004406U) /**< \brief (AC1) Interrupt Flag Status and Clear */
#define REG_AC1_STATUSA            (0x42004407U) /**< \brief (AC1) Status A */
#define REG_AC1_STATUSB            (0x42004408U) /**< \brief (AC1) Status B */
#define REG_AC1_DBGCTRL            (0x42004409U) /**< \brief (AC1) Debug Control */
#define REG_AC1_WINCTRL            (0x4200440AU) /**< \brief (AC1) Window Control */
#define REG_AC1_SCALER0            (0x4200440CU) /**< \brief (AC1) Scaler 0 */
#define REG_AC1_SCALER1            (0x4200440DU) /**< \brief (AC1) Scaler 1 */
#define REG_AC1_COMPCTRL0          (0x42004410U) /**< \brief (AC1) Comparator Control 0 */
#define REG_AC1_COMPCTRL1          (0x42004414U) /**< \brief (AC1) Comparator Control 1 */
#define REG_AC1_SYNCBUSY           (0x42004420U) /**< \brief (AC1) Synchronization Busy */
#else
#define REG_AC1_CTRLA              (*(RwReg8 *)0x42004400U) /**< \brief (AC1) Control A */
#define REG_AC1_CTRLB              (*(WoReg8 *)0x42004401U) /**< \brief (AC1) Control B */
#define REG_AC1_EVCTRL             (*(RwReg16*)0x42004402U) /**< \brief (AC1) Event Control */
#define REG_AC1_INTENCLR           (*(RwReg8 *)0x42004404U) /**< \brief (AC1) Interrupt Enable Clear */
#define REG_AC1_INTENSET           (*(RwReg8 *)0x42004405U) /**< \brief (AC1) Interrupt Enable Set */
#define REG_AC1_INTFLAG            (*(RwReg8 *)0x42004406U) /**< \brief (AC1) Interrupt Flag Status and Clear */
#define REG_AC1_STATUSA            (*(RoReg8 *)0x42004407U) /**< \brief (AC1) Status A */
#define REG_AC1_STATUSB            (*(RoReg8 *)0x42004408U) /**< \brief (AC1) Status B */
#define REG_AC1_DBGCTRL            (*(RwReg8 *)0x42004409U) /**< \brief (AC1) Debug Control */
#define REG_AC1_WINCTRL            (*(RwReg8 *)0x4200440AU) /**< \brief (AC1) Window Control */
#define REG_AC1_SCALER0            (*(RwReg8 *)0x4200440CU) /**< \brief (AC1) Scaler 0 */
#define REG_AC1_SCALER1            (*(RwReg8 *)0x4200440DU) /**< \brief (AC1) Scaler 1 */
#define REG_AC1_COMPCTRL0          (*(RwReg  *)0x42004410U) /**< \brief (AC1) Comparator Control 0 */
#define REG_AC1_COMPCTRL1          (*(RwReg  *)0x42004414U) /**< \brief (AC1) Comparator Control 1 */
#define REG_AC1_SYNCBUSY           (*(RoReg  *)0x42004420U) /**< \brief (AC1) Synchronization Busy */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for AC1 peripheral ========== */
#define AC1_COMPCTRL_MUXNEG_OPAMP   7        // OPAMP selection for MUXNEG
#define AC1_GCLK_ID                 28       // Index of Generic Clock
#define AC1_NUM_CMP                 2        // Number of comparators
#define AC1_PAIRS                   1        // Number of pairs of comparators

#endif /* _SAML22_AC1_INSTANCE_ */
