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

#ifndef _SAMD20_TC2_INSTANCE_
#define _SAMD20_TC2_INSTANCE_

/* ========== Register definition for TC2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TC2_CTRLA0                        (0x42002800U) /**< \brief (TC2) Control A Register 0 */
#define REG_TC2_CTRLA1                        (0x42002802U) /**< \brief (TC2) Control A Register 1 */
#define REG_TC2_CTRLA2                        (0x42002804U) /**< \brief (TC2) Control A Register 2 */
#define REG_TC2_CTRLA3                        (0x42002806U) /**< \brief (TC2) Control A Register 3 */
#define REG_TC2_READREQ0                      (0x42002802U) /**< \brief (TC2) Read Request Register 0 */
#define REG_TC2_READREQ1                      (0x42002804U) /**< \brief (TC2) Read Request Register 1 */
#define REG_TC2_READREQ2                      (0x42002806U) /**< \brief (TC2) Read Request Register 2 */
#define REG_TC2_READREQ3                      (0x42002808U) /**< \brief (TC2) Read Request Register 3 */
#define REG_TC2_CTRLBCLR0                     (0x42002804U) /**< \brief (TC2) Control B Clear Register 0 */
#define REG_TC2_CTRLBCLR1                     (0x42002805U) /**< \brief (TC2) Control B Clear Register 1 */
#define REG_TC2_CTRLBCLR2                     (0x42002806U) /**< \brief (TC2) Control B Clear Register 2 */
#define REG_TC2_CTRLBCLR3                     (0x42002807U) /**< \brief (TC2) Control B Clear Register 3 */
#define REG_TC2_CTRLBSET0                     (0x42002805U) /**< \brief (TC2) Control B Set Register 0 */
#define REG_TC2_CTRLBSET1                     (0x42002806U) /**< \brief (TC2) Control B Set Register 1 */
#define REG_TC2_CTRLBSET2                     (0x42002807U) /**< \brief (TC2) Control B Set Register 2 */
#define REG_TC2_CTRLBSET3                     (0x42002808U) /**< \brief (TC2) Control B Set Register 3 */
#define REG_TC2_CTRLC0                        (0x42002806U) /**< \brief (TC2) Control C Register 0 */
#define REG_TC2_CTRLC1                        (0x42002807U) /**< \brief (TC2) Control C Register 1 */
#define REG_TC2_CTRLC2                        (0x42002808U) /**< \brief (TC2) Control C Register 2 */
#define REG_TC2_CTRLC3                        (0x42002809U) /**< \brief (TC2) Control C Register 3 */
#define REG_TC2_DEBUG0                        (0x42002808U) /**< \brief (TC2) Debug Register 0 */
#define REG_TC2_DEBUG1                        (0x42002809U) /**< \brief (TC2) Debug Register 1 */
#define REG_TC2_DEBUG2                        (0x4200280AU) /**< \brief (TC2) Debug Register 2 */
#define REG_TC2_DEBUG3                        (0x4200280BU) /**< \brief (TC2) Debug Register 3 */
#define REG_TC2_EVCTRL0                       (0x4200280AU) /**< \brief (TC2) Event Control Register 0 */
#define REG_TC2_EVCTRL1                       (0x4200280CU) /**< \brief (TC2) Event Control Register 1 */
#define REG_TC2_EVCTRL2                       (0x4200280EU) /**< \brief (TC2) Event Control Register 2 */
#define REG_TC2_EVCTRL3                       (0x42002810U) /**< \brief (TC2) Event Control Register 3 */
#define REG_TC2_INTENCLR0                     (0x4200280CU) /**< \brief (TC2) Interrupt Enable Clear Register 0 */
#define REG_TC2_INTENCLR1                     (0x4200280DU) /**< \brief (TC2) Interrupt Enable Clear Register 1 */
#define REG_TC2_INTENCLR2                     (0x4200280EU) /**< \brief (TC2) Interrupt Enable Clear Register 2 */
#define REG_TC2_INTENCLR3                     (0x4200280FU) /**< \brief (TC2) Interrupt Enable Clear Register 3 */
#define REG_TC2_INTENSET0                     (0x4200280DU) /**< \brief (TC2) Interrupt Enable Set Register 0 */
#define REG_TC2_INTENSET1                     (0x4200280EU) /**< \brief (TC2) Interrupt Enable Set Register 1 */
#define REG_TC2_INTENSET2                     (0x4200280FU) /**< \brief (TC2) Interrupt Enable Set Register 2 */
#define REG_TC2_INTENSET3                     (0x42002810U) /**< \brief (TC2) Interrupt Enable Set Register 3 */
#define REG_TC2_INTFLAG0                      (0x4200280EU) /**< \brief (TC2) Interrupt Flag Status and Clear Register 0 */
#define REG_TC2_INTFLAG1                      (0x4200280FU) /**< \brief (TC2) Interrupt Flag Status and Clear Register 1 */
#define REG_TC2_INTFLAG2                      (0x42002810U) /**< \brief (TC2) Interrupt Flag Status and Clear Register 2 */
#define REG_TC2_INTFLAG3                      (0x42002811U) /**< \brief (TC2) Interrupt Flag Status and Clear Register 3 */
#define REG_TC2_STATUS0                       (0x4200280FU) /**< \brief (TC2) Status Register 0 */
#define REG_TC2_STATUS1                       (0x42002810U) /**< \brief (TC2) Status Register 1 */
#define REG_TC2_STATUS2                       (0x42002811U) /**< \brief (TC2) Status Register 2 */
#define REG_TC2_STATUS3                       (0x42002812U) /**< \brief (TC2) Status Register 3 */
#define REG_TC2_COUNT16_COUNT0                (0x42002810U) /**< \brief (TC2) COUNT16 Count Register 0 */
#define REG_TC2_COUNT16_COUNT1                (0x42002812U) /**< \brief (TC2) COUNT16 Count Register 1 */
#define REG_TC2_COUNT16_COUNT2                (0x42002814U) /**< \brief (TC2) COUNT16 Count Register 2 */
#define REG_TC2_COUNT16_COUNT3                (0x42002816U) /**< \brief (TC2) COUNT16 Count Register 3 */
#define REG_TC2_COUNT32_COUNT0                (0x42002810U) /**< \brief (TC2) COUNT32 Count Register 0 */
#define REG_TC2_COUNT32_COUNT1                (0x42002814U) /**< \brief (TC2) COUNT32 Count Register 1 */
#define REG_TC2_COUNT32_COUNT2                (0x42002818U) /**< \brief (TC2) COUNT32 Count Register 2 */
#define REG_TC2_COUNT32_COUNT3                (0x4200281CU) /**< \brief (TC2) COUNT32 Count Register 3 */
#define REG_TC2_COUNT8_COUNT0                 (0x42002810U) /**< \brief (TC2) COUNT8 Count Register 0 */
#define REG_TC2_COUNT8_COUNT1                 (0x42002811U) /**< \brief (TC2) COUNT8 Count Register 1 */
#define REG_TC2_COUNT8_COUNT2                 (0x42002812U) /**< \brief (TC2) COUNT8 Count Register 2 */
#define REG_TC2_COUNT8_COUNT3                 (0x42002813U) /**< \brief (TC2) COUNT8 Count Register 3 */
#define REG_TC2_COUNT8_PER0                   (0x42002814U) /**< \brief (TC2) COUNT8 Period Register 0 */
#define REG_TC2_COUNT8_PER1                   (0x42002815U) /**< \brief (TC2) COUNT8 Period Register 1 */
#define REG_TC2_COUNT8_PER2                   (0x42002816U) /**< \brief (TC2) COUNT8 Period Register 2 */
#define REG_TC2_COUNT8_PER3                   (0x42002817U) /**< \brief (TC2) COUNT8 Period Register 3 */
#define REG_TC2_COUNT16_CC0                   (0x42002818U) /**< \brief (TC2) COUNT16 Compare and Capture Register 0 */
#define REG_TC2_COUNT16_CC1                   (0x4200281AU) /**< \brief (TC2) COUNT16 Compare and Capture Register 1 */
#define REG_TC2_COUNT32_CC0                   (0x42002818U) /**< \brief (TC2) COUNT32 Compare and Capture Register 0 */
#define REG_TC2_COUNT32_CC1                   (0x4200281CU) /**< \brief (TC2) COUNT32 Compare and Capture Register 1 */
#define REG_TC2_COUNT8_CC0                    (0x42002818U) /**< \brief (TC2) COUNT8 Compare and Capture Register 0 */
#define REG_TC2_COUNT8_CC1                    (0x42002819U) /**< \brief (TC2) COUNT8 Compare and Capture Register 1 */
#else
#define REG_TC2_CTRLA              (*(RwReg16*)0x42002800U) /**< \brief (TC2) Control A Register */
#define REG_TC2_READREQ            (*(RwReg16*)0x42002802U) /**< \brief (TC2) Read Request Register */
#define REG_TC2_CTRLBCLR           (*(RwReg8 *)0x42002804U) /**< \brief (TC2) Control B Clear Register */
#define REG_TC2_CTRLBSET           (*(RwReg8 *)0x42002805U) /**< \brief (TC2) Control B Set Register */
#define REG_TC2_CTRLC              (*(RwReg8 *)0x42002806U) /**< \brief (TC2) Control C Register */
#define REG_TC2_DEBUG              (*(RwReg8 *)0x42002808U) /**< \brief (TC2) Debug Register */
#define REG_TC2_EVCTRL             (*(RwReg16*)0x4200280AU) /**< \brief (TC2) Event Control Register */
#define REG_TC2_INTENCLR           (*(RwReg8 *)0x4200280CU) /**< \brief (TC2) Interrupt Enable Clear Register */
#define REG_TC2_INTENSET           (*(RwReg8 *)0x4200280DU) /**< \brief (TC2) Interrupt Enable Set Register */
#define REG_TC2_INTFLAG            (*(RwReg8 *)0x4200280EU) /**< \brief (TC2) Interrupt Flag Status and Clear Register */
#define REG_TC2_STATUS             (*(RoReg8 *)0x4200280FU) /**< \brief (TC2) Status Register */
#define REG_TC2_COUNT16_COUNT      (*(RwReg16*)0x42002810U) /**< \brief (TC2) COUNT16 Count Register */
#define REG_TC2_COUNT32_COUNT      (*(RwReg  *)0x42002810U) /**< \brief (TC2) COUNT32 Count Register */
#define REG_TC2_COUNT8_COUNT       (*(RwReg8 *)0x42002810U) /**< \brief (TC2) COUNT8 Count Register */
#define REG_TC2_COUNT8_PER         (*(RwReg8 *)0x42002814U) /**< \brief (TC2) COUNT8 Period Register */
#define REG_TC2_COUNT16_CC0        (*(RwReg16*)0x42002818U) /**< \brief (TC2) COUNT16 Compare and Capture Register 0 */
#define REG_TC2_COUNT16_CC1        (*(RwReg16*)0x4200281AU) /**< \brief (TC2) COUNT16 Compare and Capture Register 1 */
#define REG_TC2_COUNT32_CC0        (*(RwReg  *)0x42002818U) /**< \brief (TC2) COUNT32 Compare and Capture Register 0 */
#define REG_TC2_COUNT32_CC1        (*(RwReg  *)0x4200281CU) /**< \brief (TC2) COUNT32 Compare and Capture Register 1 */
#define REG_TC2_COUNT8_CC0         (*(RwReg8 *)0x42002818U) /**< \brief (TC2) COUNT8 Compare and Capture Register 0 */
#define REG_TC2_COUNT8_CC1         (*(RwReg8 *)0x42002819U) /**< \brief (TC2) COUNT8 Compare and Capture Register 1 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TC2 peripheral ========== */
#define TC2_CC8_NUM                 2
#define TC2_CC16_NUM                2
#define TC2_CC32_NUM                2
#define TC2_GCLK_ID                 20
#define TC2_OW_NUM                  2

#endif /* _SAMD20_TC2_INSTANCE_ */
