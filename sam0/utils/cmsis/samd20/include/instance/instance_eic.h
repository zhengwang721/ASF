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

#ifndef _SAMD20_EIC_INSTANCE_
#define _SAMD20_EIC_INSTANCE_

/* ========== Register definition for EIC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_EIC_CTRL0                         (0x40001800U) /**< \brief (EIC) Control Register 0 */
#define REG_EIC_CTRL1                         (0x40001801U) /**< \brief (EIC) Control Register 1 */
#define REG_EIC_CTRL2                         (0x40001802U) /**< \brief (EIC) Control Register 2 */
#define REG_EIC_CTRL3                         (0x40001803U) /**< \brief (EIC) Control Register 3 */
#define REG_EIC_STATUS0                       (0x40001801U) /**< \brief (EIC) Status Register 0 */
#define REG_EIC_STATUS1                       (0x40001802U) /**< \brief (EIC) Status Register 1 */
#define REG_EIC_STATUS2                       (0x40001803U) /**< \brief (EIC) Status Register 2 */
#define REG_EIC_STATUS3                       (0x40001804U) /**< \brief (EIC) Status Register 3 */
#define REG_EIC_NMICTRL0                      (0x40001802U) /**< \brief (EIC) NMI Control Register 0 */
#define REG_EIC_NMICTRL1                      (0x40001803U) /**< \brief (EIC) NMI Control Register 1 */
#define REG_EIC_NMICTRL2                      (0x40001804U) /**< \brief (EIC) NMI Control Register 2 */
#define REG_EIC_NMICTRL3                      (0x40001805U) /**< \brief (EIC) NMI Control Register 3 */
#define REG_EIC_NMIFLAG0                      (0x40001803U) /**< \brief (EIC) NMI Interrupt Flag Register 0 */
#define REG_EIC_NMIFLAG1                      (0x40001804U) /**< \brief (EIC) NMI Interrupt Flag Register 1 */
#define REG_EIC_NMIFLAG2                      (0x40001805U) /**< \brief (EIC) NMI Interrupt Flag Register 2 */
#define REG_EIC_NMIFLAG3                      (0x40001806U) /**< \brief (EIC) NMI Interrupt Flag Register 3 */
#define REG_EIC_EVCTRL0                       (0x40001804U) /**< \brief (EIC) Event Control Register 0 */
#define REG_EIC_EVCTRL1                       (0x40001808U) /**< \brief (EIC) Event Control Register 1 */
#define REG_EIC_EVCTRL2                       (0x4000180CU) /**< \brief (EIC) Event Control Register 2 */
#define REG_EIC_EVCTRL3                       (0x40001810U) /**< \brief (EIC) Event Control Register 3 */
#define REG_EIC_INTENCLR0                     (0x40001808U) /**< \brief (EIC) Interrupt Enable Clear Register 0 */
#define REG_EIC_INTENCLR1                     (0x4000180CU) /**< \brief (EIC) Interrupt Enable Clear Register 1 */
#define REG_EIC_INTENCLR2                     (0x40001810U) /**< \brief (EIC) Interrupt Enable Clear Register 2 */
#define REG_EIC_INTENCLR3                     (0x40001814U) /**< \brief (EIC) Interrupt Enable Clear Register 3 */
#define REG_EIC_INTENSET0                     (0x4000180CU) /**< \brief (EIC) Interrupt Enable Set Register 0 */
#define REG_EIC_INTENSET1                     (0x40001810U) /**< \brief (EIC) Interrupt Enable Set Register 1 */
#define REG_EIC_INTENSET2                     (0x40001814U) /**< \brief (EIC) Interrupt Enable Set Register 2 */
#define REG_EIC_INTENSET3                     (0x40001818U) /**< \brief (EIC) Interrupt Enable Set Register 3 */
#define REG_EIC_INTFLAG0                      (0x40001810U) /**< \brief (EIC) Interrupt Flag Status and Clear Register 0 */
#define REG_EIC_INTFLAG1                      (0x40001814U) /**< \brief (EIC) Interrupt Flag Status and Clear Register 1 */
#define REG_EIC_INTFLAG2                      (0x40001818U) /**< \brief (EIC) Interrupt Flag Status and Clear Register 2 */
#define REG_EIC_INTFLAG3                      (0x4000181CU) /**< \brief (EIC) Interrupt Flag Status and Clear Register 3 */
#define REG_EIC_WAKEUP0                       (0x40001814U) /**< \brief (EIC) Wake-up Enable Register 0 */
#define REG_EIC_WAKEUP1                       (0x40001818U) /**< \brief (EIC) Wake-up Enable Register 1 */
#define REG_EIC_WAKEUP2                       (0x4000181CU) /**< \brief (EIC) Wake-up Enable Register 2 */
#define REG_EIC_WAKEUP3                       (0x40001820U) /**< \brief (EIC) Wake-up Enable Register 3 */
#define REG_EIC_CONFIG0                       (0x40001818U) /**< \brief (EIC) Config Register 0 */
#define REG_EIC_CONFIG1                       (0x4000181CU) /**< \brief (EIC) Config Register 1 */
#else
#define REG_EIC_CTRL               (*(RwReg8 *)0x40001800U) /**< \brief (EIC) Control Register */
#define REG_EIC_STATUS             (*(RoReg8 *)0x40001801U) /**< \brief (EIC) Status Register */
#define REG_EIC_NMICTRL            (*(RwReg8 *)0x40001802U) /**< \brief (EIC) NMI Control Register */
#define REG_EIC_NMIFLAG            (*(RwReg8 *)0x40001803U) /**< \brief (EIC) NMI Interrupt Flag Register */
#define REG_EIC_EVCTRL             (*(RwReg  *)0x40001804U) /**< \brief (EIC) Event Control Register */
#define REG_EIC_INTENCLR           (*(RwReg  *)0x40001808U) /**< \brief (EIC) Interrupt Enable Clear Register */
#define REG_EIC_INTENSET           (*(RwReg  *)0x4000180CU) /**< \brief (EIC) Interrupt Enable Set Register */
#define REG_EIC_INTFLAG            (*(RwReg  *)0x40001810U) /**< \brief (EIC) Interrupt Flag Status and Clear Register */
#define REG_EIC_WAKEUP             (*(RwReg  *)0x40001814U) /**< \brief (EIC) Wake-up Enable Register */
#define REG_EIC_CONFIG0            (*(RwReg  *)0x40001818U) /**< \brief (EIC) Config Register 0 */
#define REG_EIC_CONFIG1            (*(RwReg  *)0x4000181CU) /**< \brief (EIC) Config Register 1 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for EIC peripheral ========== */
#define EIC_GCLK_ID                 3
#define EIC_NUMBER_OF_CONFIG_REGS   2
#define EIC_NUMBER_OF_INTERRUPTS    16

#endif /* _SAMD20_EIC_INSTANCE_ */
