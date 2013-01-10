/**
 * \file
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

#ifndef _SAMD20_PTC_INSTANCE_
#define _SAMD20_PTC_INSTANCE_

/* ========== Register definition for PTC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PTC_CTRLA                         (0x42004C00U) /**< \brief (PTC) Control Register A */
#define REG_PTC_STATUS                        (0x42004C01U) /**< \brief (PTC) Status Register */
#define REG_PTC_CTRLB                         (0x42004C04U) /**< \brief (PTC) Control Register B */
#define REG_PTC_EVCTRL                        (0x42004C05U) /**< \brief (PTC) Event Control Register */
#define REG_PTC_INTENCLR                      (0x42004C08U) /**< \brief (PTC) Interrupt Enable Clear Register */
#define REG_PTC_INTENSET                      (0x42004C09U) /**< \brief (PTC) Interrupt Enable Set Register */
#define REG_PTC_INTFLAG                       (0x42004C0AU) /**< \brief (PTC) Interrupt Flag Status and Clear Register */
#define REG_PTC_CTSCTRLA                      (0x42004C0CU) /**< \brief (PTC) CTS Control Register A */
#define REG_PTC_CTSCTRLC                      (0x42004C0DU) /**< \brief (PTC) CTS Control Register C */
#define REG_PTC_YSEL                          (0x42004C10U) /**< \brief (PTC) Y Line Select Register */
#define REG_PTC_XSEL                          (0x42004C12U) /**< \brief (PTC) X Line Select Register */
#define REG_PTC_YEN                           (0x42004C14U) /**< \brief (PTC) Y Line Enable Register */
#define REG_PTC_XEN                           (0x42004C16U) /**< \brief (PTC) X Line Enable Register */
#define REG_PTC_CCCAL                         (0x42004C18U) /**< \brief (PTC) CTS Compensation Capacitor Calibration Register */
#define REG_PTC_CICAL                         (0x42004C1AU) /**< \brief (PTC) CTS Integrator Capacitor Calibration Register */
#define REG_PTC_CTSRS                         (0x42004C1BU) /**< \brief (PTC) CTS Resistor Selection Register */
#define REG_PTC_RESULT                        (0x42004C1CU) /**< \brief (PTC) Conversion Result Register */
#define REG_PTC_CTSCTRLB                      (0x42004C20U) /**< \brief (PTC) CTS Control Register B */
#define REG_PTC_WINCCTRL                      (0x42004C21U) /**< \brief (PTC) Window Comparator Control Register */
#define REG_PTC_WCHT                          (0x42004C24U) /**< \brief (PTC) Window Comparator High Threshold Register */
#define REG_PTC_WCLT                          (0x42004C26U) /**< \brief (PTC) Window Comparator Low Threshold Register */
#else
#define REG_PTC_CTRLA              (*(RwReg8 *)0x42004C00U) /**< \brief (PTC) Control Register A */
#define REG_PTC_STATUS             (*(RoReg8 *)0x42004C01U) /**< \brief (PTC) Status Register */
#define REG_PTC_CTRLB              (*(RwReg8 *)0x42004C04U) /**< \brief (PTC) Control Register B */
#define REG_PTC_EVCTRL             (*(RwReg8 *)0x42004C05U) /**< \brief (PTC) Event Control Register */
#define REG_PTC_INTENCLR           (*(RwReg8 *)0x42004C08U) /**< \brief (PTC) Interrupt Enable Clear Register */
#define REG_PTC_INTENSET           (*(RwReg8 *)0x42004C09U) /**< \brief (PTC) Interrupt Enable Set Register */
#define REG_PTC_INTFLAG            (*(RwReg8 *)0x42004C0AU) /**< \brief (PTC) Interrupt Flag Status and Clear Register */
#define REG_PTC_CTSCTRLA           (*(RwReg8 *)0x42004C0CU) /**< \brief (PTC) CTS Control Register A */
#define REG_PTC_CTSCTRLC           (*(RwReg8 *)0x42004C0DU) /**< \brief (PTC) CTS Control Register C */
#define REG_PTC_YSEL               (*(RwReg16*)0x42004C10U) /**< \brief (PTC) Y Line Select Register */
#define REG_PTC_XSEL               (*(RwReg16*)0x42004C12U) /**< \brief (PTC) X Line Select Register */
#define REG_PTC_YEN                (*(RwReg16*)0x42004C14U) /**< \brief (PTC) Y Line Enable Register */
#define REG_PTC_XEN                (*(RwReg16*)0x42004C16U) /**< \brief (PTC) X Line Enable Register */
#define REG_PTC_CCCAL              (*(RwReg16*)0x42004C18U) /**< \brief (PTC) CTS Compensation Capacitor Calibration Register */
#define REG_PTC_CICAL              (*(RwReg8 *)0x42004C1AU) /**< \brief (PTC) CTS Integrator Capacitor Calibration Register */
#define REG_PTC_CTSRS              (*(RwReg8 *)0x42004C1BU) /**< \brief (PTC) CTS Resistor Selection Register */
#define REG_PTC_RESULT             (*(RwReg16*)0x42004C1CU) /**< \brief (PTC) Conversion Result Register */
#define REG_PTC_CTSCTRLB           (*(RwReg8 *)0x42004C20U) /**< \brief (PTC) CTS Control Register B */
#define REG_PTC_WINCCTRL           (*(RwReg8 *)0x42004C21U) /**< \brief (PTC) Window Comparator Control Register */
#define REG_PTC_WCHT               (*(RwReg16*)0x42004C24U) /**< \brief (PTC) Window Comparator High Threshold Register */
#define REG_PTC_WCLT               (*(RwReg16*)0x42004C26U) /**< \brief (PTC) Window Comparator Low Threshold Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PTC peripheral ========== */
#define PTC_GCLK_ID                 27

#endif /* _SAMD20_PTC_INSTANCE_ */
