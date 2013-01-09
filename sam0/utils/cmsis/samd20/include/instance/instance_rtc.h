/**
 * \file
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#ifndef _SAMD20_RTC_INSTANCE_
#define _SAMD20_RTC_INSTANCE_

/* ========== Register definition for RTC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_RTC_MODE0_CTRL         (0x40001400U) /**< \brief (RTC) Mode0 MODE0 Control Register */
#define REG_RTC_MODE1_CTRL         (0x40001400U) /**< \brief (RTC) Mode1 MODE1 Control Register */
#define REG_RTC_MODE2_CTRL         (0x40001400U) /**< \brief (RTC) Mode2 MODE2 Control Register */
#define REG_RTC_READREQ            (0x40001402U) /**< \brief (RTC) Read Request Register */
#define REG_RTC_MODE0_EVCTRL       (0x40001404U) /**< \brief (RTC) Mode0 MODE0 Event Control Register */
#define REG_RTC_MODE1_EVCTRL       (0x40001404U) /**< \brief (RTC) Mode1 MODE1 Event Control Register */
#define REG_RTC_MODE2_EVCTRL       (0x40001404U) /**< \brief (RTC) Mode2 MODE2 Event Control Register */
#define REG_RTC_MODE0_INTENCLR     (0x40001406U) /**< \brief (RTC) Mode0 MODE0 Interrupt Enable Clear Register */
#define REG_RTC_MODE1_INTENCLR     (0x40001406U) /**< \brief (RTC) Mode1 MODE1 Interrupt Enable Clear Register */
#define REG_RTC_MODE2_INTENCLR     (0x40001406U) /**< \brief (RTC) Mode2 MODE2 Interrupt Enable Clear Register */
#define REG_RTC_MODE0_INTENSET     (0x40001407U) /**< \brief (RTC) Mode0 MODE0 Interrupt Enable Set Register */
#define REG_RTC_MODE1_INTENSET     (0x40001407U) /**< \brief (RTC) Mode1 MODE1 Interrupt Enable Set Register */
#define REG_RTC_MODE2_INTENSET     (0x40001407U) /**< \brief (RTC) Mode2 MODE2 Interrupt Enable Set Register */
#define REG_RTC_MODE0_INTFLAG      (0x40001408U) /**< \brief (RTC) Mode0 MODE0 Interrupt Flag Status and Clear Register */
#define REG_RTC_MODE1_INTFLAG      (0x40001408U) /**< \brief (RTC) Mode1 MODE1 Interrupt Flag Status and Clear Register */
#define REG_RTC_MODE2_INTFLAG      (0x40001408U) /**< \brief (RTC) Mode2 MODE2 Interrupt Flag Status and Clear Register */
#define REG_RTC_STATUS             (0x4000140AU) /**< \brief (RTC) Status Register */
#define REG_RTC_DEBUG              (0x4000140BU) /**< \brief (RTC) Debug Register */
#define REG_RTC_FREQCORR           (0x4000140CU) /**< \brief (RTC) Frequency Correction Register */
#define REG_RTC_MODE0_COUNT        (0x40001410U) /**< \brief (RTC) Mode0 MODE0 Count Register */
#define REG_RTC_MODE1_COUNT        (0x40001410U) /**< \brief (RTC) Mode1 MODE1 Count Register */
#define REG_RTC_MODE2_CLOCK        (0x40001410U) /**< \brief (RTC) Mode2 MODE2 Clock Register */
#define REG_RTC_MODE1_PER          (0x40001414U) /**< \brief (RTC) Mode1 MODE1 Period Register */
#define REG_RTC_MODE0_COMP         (0x40001418U) /**< \brief (RTC) Mode0 MODE0 Compare Register */
#define REG_RTC_MODE1_COMP         (0x40001418U) /**< \brief (RTC) Mode1 MODE1 Compare Register */
#define REG_RTC_MODE2_ALARM_ALARM  (0x40001418U) /**< \brief (RTC) Mode2Alarm MODE2_ALARM Alarm Register */
#define REG_RTC_MODE2_ALARM_MASK   (0x4000141CU) /**< \brief (RTC) Mode2Alarm MODE2_ALARM Alarm Mask Register */
#else
#define REG_RTC_CTRL               (*(RwReg16*)0x40001400U) /**< \brief (RTC) MODE0 Control Register */
#define REG_RTC_CTRL               (*(RwReg16*)0x40001400U) /**< \brief (RTC) MODE1 Control Register */
#define REG_RTC_CTRL               (*(RwReg16*)0x40001400U) /**< \brief (RTC) MODE2 Control Register */
#define REG_RTC_READREQ            (*(RwReg16*)0x40001402U) /**< \brief (RTC) Read Request Register */
#define REG_RTC_EVCTRL             (*(RwReg16*)0x40001404U) /**< \brief (RTC) MODE0 Event Control Register */
#define REG_RTC_EVCTRL             (*(RwReg16*)0x40001404U) /**< \brief (RTC) MODE1 Event Control Register */
#define REG_RTC_EVCTRL             (*(RwReg16*)0x40001404U) /**< \brief (RTC) MODE2 Event Control Register */
#define REG_RTC_INTENCLR           (*(RwReg8 *)0x40001406U) /**< \brief (RTC) MODE0 Interrupt Enable Clear Register */
#define REG_RTC_INTENCLR           (*(RwReg8 *)0x40001406U) /**< \brief (RTC) MODE1 Interrupt Enable Clear Register */
#define REG_RTC_INTENCLR           (*(RwReg8 *)0x40001406U) /**< \brief (RTC) MODE2 Interrupt Enable Clear Register */
#define REG_RTC_INTENSET           (*(RwReg8 *)0x40001407U) /**< \brief (RTC) MODE0 Interrupt Enable Set Register */
#define REG_RTC_INTENSET           (*(RwReg8 *)0x40001407U) /**< \brief (RTC) MODE1 Interrupt Enable Set Register */
#define REG_RTC_INTENSET           (*(RwReg8 *)0x40001407U) /**< \brief (RTC) MODE2 Interrupt Enable Set Register */
#define REG_RTC_INTFLAG            (*(RwReg8 *)0x40001408U) /**< \brief (RTC) MODE0 Interrupt Flag Status and Clear Register */
#define REG_RTC_INTFLAG            (*(RwReg8 *)0x40001408U) /**< \brief (RTC) MODE1 Interrupt Flag Status and Clear Register */
#define REG_RTC_INTFLAG            (*(RwReg8 *)0x40001408U) /**< \brief (RTC) MODE2 Interrupt Flag Status and Clear Register */
#define REG_RTC_STATUS             (*(RwReg8 *)0x4000140AU) /**< \brief (RTC) Status Register */
#define REG_RTC_DEBUG              (*(RwReg8 *)0x4000140BU) /**< \brief (RTC) Debug Register */
#define REG_RTC_FREQCORR           (*(RwReg8 *)0x4000140CU) /**< \brief (RTC) Frequency Correction Register */
#define REG_RTC_COUNT              (*(RwReg  *)0x40001410U) /**< \brief (RTC) MODE0 Count Register */
#define REG_RTC_COUNT              (*(RwReg16*)0x40001410U) /**< \brief (RTC) MODE1 Count Register */
#define REG_RTC_CLOCK              (*(RwReg  *)0x40001410U) /**< \brief (RTC) MODE2 Clock Register */
#define REG_RTC_PER                (*(RwReg16*)0x40001414U) /**< \brief (RTC) MODE1 Period Register */
#define REG_RTC_COMP               (*(RwReg  *)0x40001418U) /**< \brief (RTC) MODE0 Compare Register */
#define REG_RTC_COMP               (*(RwReg16*)0x40001418U) /**< \brief (RTC) MODE1 Compare Register */
#define REG_RTC_ALARM              (*(RwReg  *)0x40001418U) /**< \brief (RTC) MODE2_ALARM Alarm Register */
#define REG_RTC_MASK               (*(RwReg8 *)0x4000141CU) /**< \brief (RTC) MODE2_ALARM Alarm Mask Register */
#endif /* (defined(__ASSEMBLY__) || defines(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for RTC peripheral ========== */
#define RTC_GCLK_ID                 2
#define RTC_NUM_OF_ALARMS           1
#define RTC_NUM_OF_COMP16           2

#endif /* _SAMD20_RTC_INSTANCE_ */
