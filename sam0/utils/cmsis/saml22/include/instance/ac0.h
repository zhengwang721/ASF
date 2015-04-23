/**
 * \file
 *
 * \brief Instance description for AC0
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

#ifndef _SAML22_AC0_INSTANCE_
#define _SAML22_AC0_INSTANCE_

/* ========== Register definition for AC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_AC0_CTRLA              (0x42004000U) /**< \brief (AC0) Control A */
#define REG_AC0_CTRLB              (0x42004001U) /**< \brief (AC0) Control B */
#define REG_AC0_EVCTRL             (0x42004002U) /**< \brief (AC0) Event Control */
#define REG_AC0_INTENCLR           (0x42004004U) /**< \brief (AC0) Interrupt Enable Clear */
#define REG_AC0_INTENSET           (0x42004005U) /**< \brief (AC0) Interrupt Enable Set */
#define REG_AC0_INTFLAG            (0x42004006U) /**< \brief (AC0) Interrupt Flag Status and Clear */
#define REG_AC0_STATUSA            (0x42004007U) /**< \brief (AC0) Status A */
#define REG_AC0_STATUSB            (0x42004008U) /**< \brief (AC0) Status B */
#define REG_AC0_DBGCTRL            (0x42004009U) /**< \brief (AC0) Debug Control */
#define REG_AC0_WINCTRL            (0x4200400AU) /**< \brief (AC0) Window Control */
#define REG_AC0_SCALER0            (0x4200400CU) /**< \brief (AC0) Scaler 0 */
#define REG_AC0_SCALER1            (0x4200400DU) /**< \brief (AC0) Scaler 1 */
#define REG_AC0_COMPCTRL0          (0x42004010U) /**< \brief (AC0) Comparator Control 0 */
#define REG_AC0_COMPCTRL1          (0x42004014U) /**< \brief (AC0) Comparator Control 1 */
#define REG_AC0_SYNCBUSY           (0x42004020U) /**< \brief (AC0) Synchronization Busy */
#else
#define REG_AC0_CTRLA              (*(RwReg8 *)0x42004000U) /**< \brief (AC0) Control A */
#define REG_AC0_CTRLB              (*(WoReg8 *)0x42004001U) /**< \brief (AC0) Control B */
#define REG_AC0_EVCTRL             (*(RwReg16*)0x42004002U) /**< \brief (AC0) Event Control */
#define REG_AC0_INTENCLR           (*(RwReg8 *)0x42004004U) /**< \brief (AC0) Interrupt Enable Clear */
#define REG_AC0_INTENSET           (*(RwReg8 *)0x42004005U) /**< \brief (AC0) Interrupt Enable Set */
#define REG_AC0_INTFLAG            (*(RwReg8 *)0x42004006U) /**< \brief (AC0) Interrupt Flag Status and Clear */
#define REG_AC0_STATUSA            (*(RoReg8 *)0x42004007U) /**< \brief (AC0) Status A */
#define REG_AC0_STATUSB            (*(RoReg8 *)0x42004008U) /**< \brief (AC0) Status B */
#define REG_AC0_DBGCTRL            (*(RwReg8 *)0x42004009U) /**< \brief (AC0) Debug Control */
#define REG_AC0_WINCTRL            (*(RwReg8 *)0x4200400AU) /**< \brief (AC0) Window Control */
#define REG_AC0_SCALER0            (*(RwReg8 *)0x4200400CU) /**< \brief (AC0) Scaler 0 */
#define REG_AC0_SCALER1            (*(RwReg8 *)0x4200400DU) /**< \brief (AC0) Scaler 1 */
#define REG_AC0_COMPCTRL0          (*(RwReg  *)0x42004010U) /**< \brief (AC0) Comparator Control 0 */
#define REG_AC0_COMPCTRL1          (*(RwReg  *)0x42004014U) /**< \brief (AC0) Comparator Control 1 */
#define REG_AC0_SYNCBUSY           (*(RoReg  *)0x42004020U) /**< \brief (AC0) Synchronization Busy */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for AC0 peripheral ========== */
#define AC0_COMPCTRL_MUXNEG_OPAMP   7        // OPAMP selection for MUXNEG
#define AC0_GCLK_ID                 27       // Index of Generic Clock
#define AC0_NUM_CMP                 2        // Number of comparators
#define AC0_PAIRS                   1        // Number of pairs of comparators

#endif /* _SAML22_AC0_INSTANCE_ */
