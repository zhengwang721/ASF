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

#ifndef _SAMD20_ADC_INSTANCE_
#define _SAMD20_ADC_INSTANCE_

/* ========== Register definition for ADC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_ADC_CTRLA0                        (0x42004000U) /**< \brief (ADC) Control Register A 0 */
#define REG_ADC_CTRLA1                        (0x42004001U) /**< \brief (ADC) Control Register A 1 */
#define REG_ADC_CTRLA2                        (0x42004002U) /**< \brief (ADC) Control Register A 2 */
#define REG_ADC_CTRLA3                        (0x42004003U) /**< \brief (ADC) Control Register A 3 */
#define REG_ADC_REFCTRL0                      (0x42004001U) /**< \brief (ADC) Reference Control Register 0 */
#define REG_ADC_REFCTRL1                      (0x42004002U) /**< \brief (ADC) Reference Control Register 1 */
#define REG_ADC_REFCTRL2                      (0x42004003U) /**< \brief (ADC) Reference Control Register 2 */
#define REG_ADC_REFCTRL3                      (0x42004004U) /**< \brief (ADC) Reference Control Register 3 */
#define REG_ADC_AVGCTRL0                      (0x42004002U) /**< \brief (ADC) Average Control Register 0 */
#define REG_ADC_AVGCTRL1                      (0x42004003U) /**< \brief (ADC) Average Control Register 1 */
#define REG_ADC_AVGCTRL2                      (0x42004004U) /**< \brief (ADC) Average Control Register 2 */
#define REG_ADC_AVGCTRL3                      (0x42004005U) /**< \brief (ADC) Average Control Register 3 */
#define REG_ADC_SAMPCTRL0                     (0x42004003U) /**< \brief (ADC) Sample Time Control Register 0 */
#define REG_ADC_SAMPCTRL1                     (0x42004004U) /**< \brief (ADC) Sample Time Control Register 1 */
#define REG_ADC_SAMPCTRL2                     (0x42004005U) /**< \brief (ADC) Sample Time Control Register 2 */
#define REG_ADC_SAMPCTRL3                     (0x42004006U) /**< \brief (ADC) Sample Time Control Register 3 */
#define REG_ADC_CTRLB0                        (0x42004004U) /**< \brief (ADC) Control Register B 0 */
#define REG_ADC_CTRLB1                        (0x42004006U) /**< \brief (ADC) Control Register B 1 */
#define REG_ADC_CTRLB2                        (0x42004008U) /**< \brief (ADC) Control Register B 2 */
#define REG_ADC_CTRLB3                        (0x4200400AU) /**< \brief (ADC) Control Register B 3 */
#define REG_ADC_WINCTRL0                      (0x42004008U) /**< \brief (ADC) Window Monitor Control Register 0 */
#define REG_ADC_WINCTRL1                      (0x42004009U) /**< \brief (ADC) Window Monitor Control Register 1 */
#define REG_ADC_WINCTRL2                      (0x4200400AU) /**< \brief (ADC) Window Monitor Control Register 2 */
#define REG_ADC_WINCTRL3                      (0x4200400BU) /**< \brief (ADC) Window Monitor Control Register 3 */
#define REG_ADC_SWTRIG0                       (0x4200400CU) /**< \brief (ADC) Control Register B 0 */
#define REG_ADC_SWTRIG1                       (0x4200400DU) /**< \brief (ADC) Control Register B 1 */
#define REG_ADC_SWTRIG2                       (0x4200400EU) /**< \brief (ADC) Control Register B 2 */
#define REG_ADC_SWTRIG3                       (0x4200400FU) /**< \brief (ADC) Control Register B 3 */
#define REG_ADC_INPUTCTRL0                    (0x42004010U) /**< \brief (ADC) Input Control Register 0 */
#define REG_ADC_INPUTCTRL1                    (0x42004014U) /**< \brief (ADC) Input Control Register 1 */
#define REG_ADC_INPUTCTRL2                    (0x42004018U) /**< \brief (ADC) Input Control Register 2 */
#define REG_ADC_INPUTCTRL3                    (0x4200401CU) /**< \brief (ADC) Input Control Register 3 */
#define REG_ADC_EVCTRL0                       (0x42004014U) /**< \brief (ADC) Event Control Register 0 */
#define REG_ADC_EVCTRL1                       (0x42004015U) /**< \brief (ADC) Event Control Register 1 */
#define REG_ADC_EVCTRL2                       (0x42004016U) /**< \brief (ADC) Event Control Register 2 */
#define REG_ADC_EVCTRL3                       (0x42004017U) /**< \brief (ADC) Event Control Register 3 */
#define REG_ADC_INTENCLR0                     (0x42004016U) /**< \brief (ADC) Interrupt Enable Clear Register 0 */
#define REG_ADC_INTENCLR1                     (0x42004017U) /**< \brief (ADC) Interrupt Enable Clear Register 1 */
#define REG_ADC_INTENCLR2                     (0x42004018U) /**< \brief (ADC) Interrupt Enable Clear Register 2 */
#define REG_ADC_INTENCLR3                     (0x42004019U) /**< \brief (ADC) Interrupt Enable Clear Register 3 */
#define REG_ADC_INTENSET0                     (0x42004017U) /**< \brief (ADC) Interrupt Enable Set Register 0 */
#define REG_ADC_INTENSET1                     (0x42004018U) /**< \brief (ADC) Interrupt Enable Set Register 1 */
#define REG_ADC_INTENSET2                     (0x42004019U) /**< \brief (ADC) Interrupt Enable Set Register 2 */
#define REG_ADC_INTENSET3                     (0x4200401AU) /**< \brief (ADC) Interrupt Enable Set Register 3 */
#define REG_ADC_INTFLAG0                      (0x42004018U) /**< \brief (ADC) Interrupt Flag Status and Clear Register 0 */
#define REG_ADC_INTFLAG1                      (0x42004019U) /**< \brief (ADC) Interrupt Flag Status and Clear Register 1 */
#define REG_ADC_INTFLAG2                      (0x4200401AU) /**< \brief (ADC) Interrupt Flag Status and Clear Register 2 */
#define REG_ADC_INTFLAG3                      (0x4200401BU) /**< \brief (ADC) Interrupt Flag Status and Clear Register 3 */
#define REG_ADC_STATUS0                       (0x42004019U) /**< \brief (ADC) Status Register 0 */
#define REG_ADC_STATUS1                       (0x4200401AU) /**< \brief (ADC) Status Register 1 */
#define REG_ADC_STATUS2                       (0x4200401BU) /**< \brief (ADC) Status Register 2 */
#define REG_ADC_STATUS3                       (0x4200401CU) /**< \brief (ADC) Status Register 3 */
#define REG_ADC_RESULT0                       (0x4200401AU) /**< \brief (ADC) Result Register 0 */
#define REG_ADC_RESULT1                       (0x4200401CU) /**< \brief (ADC) Result Register 1 */
#define REG_ADC_RESULT2                       (0x4200401EU) /**< \brief (ADC) Result Register 2 */
#define REG_ADC_RESULT3                       (0x42004020U) /**< \brief (ADC) Result Register 3 */
#define REG_ADC_WINLT0                        (0x4200401CU) /**< \brief (ADC) Window Monitor Lower Threshold Register 0 */
#define REG_ADC_WINLT1                        (0x4200401EU) /**< \brief (ADC) Window Monitor Lower Threshold Register 1 */
#define REG_ADC_WINLT2                        (0x42004020U) /**< \brief (ADC) Window Monitor Lower Threshold Register 2 */
#define REG_ADC_WINLT3                        (0x42004022U) /**< \brief (ADC) Window Monitor Lower Threshold Register 3 */
#define REG_ADC_WINUT0                        (0x42004020U) /**< \brief (ADC) Window Monitor Upper Threshold Register 0 */
#define REG_ADC_WINUT1                        (0x42004022U) /**< \brief (ADC) Window Monitor Upper Threshold Register 1 */
#define REG_ADC_WINUT2                        (0x42004024U) /**< \brief (ADC) Window Monitor Upper Threshold Register 2 */
#define REG_ADC_WINUT3                        (0x42004026U) /**< \brief (ADC) Window Monitor Upper Threshold Register 3 */
#define REG_ADC_GAINCORR0                     (0x42004024U) /**< \brief (ADC) Gain Correction Register 0 */
#define REG_ADC_GAINCORR1                     (0x42004026U) /**< \brief (ADC) Gain Correction Register 1 */
#define REG_ADC_GAINCORR2                     (0x42004028U) /**< \brief (ADC) Gain Correction Register 2 */
#define REG_ADC_GAINCORR3                     (0x4200402AU) /**< \brief (ADC) Gain Correction Register 3 */
#define REG_ADC_OFFSETCORR0                   (0x42004026U) /**< \brief (ADC) Offset Correction Register 0 */
#define REG_ADC_OFFSETCORR1                   (0x42004028U) /**< \brief (ADC) Offset Correction Register 1 */
#define REG_ADC_OFFSETCORR2                   (0x4200402AU) /**< \brief (ADC) Offset Correction Register 2 */
#define REG_ADC_OFFSETCORR3                   (0x4200402CU) /**< \brief (ADC) Offset Correction Register 3 */
#define REG_ADC_CALIB0                        (0x42004028U) /**< \brief (ADC) Calibration Register 0 */
#define REG_ADC_CALIB1                        (0x4200402AU) /**< \brief (ADC) Calibration Register 1 */
#define REG_ADC_CALIB2                        (0x4200402CU) /**< \brief (ADC) Calibration Register 2 */
#define REG_ADC_CALIB3                        (0x4200402EU) /**< \brief (ADC) Calibration Register 3 */
#define REG_ADC_DEBUG0                        (0x4200402AU) /**< \brief (ADC) Debug Register 0 */
#define REG_ADC_DEBUG1                        (0x4200402BU) /**< \brief (ADC) Debug Register 1 */
#define REG_ADC_DEBUG2                        (0x4200402CU) /**< \brief (ADC) Debug Register 2 */
#define REG_ADC_DEBUG3                        (0x4200402DU) /**< \brief (ADC) Debug Register 3 */
#define REG_ADC_TEST0                         (0x4200402BU) /**< \brief (ADC) Test Modes Register 0 */
#define REG_ADC_TEST1                         (0x4200402CU) /**< \brief (ADC) Test Modes Register 1 */
#define REG_ADC_TEST2                         (0x4200402DU) /**< \brief (ADC) Test Modes Register 2 */
#define REG_ADC_TEST3                         (0x4200402EU) /**< \brief (ADC) Test Modes Register 3 */
#define REG_ADC_TESTRESULT0                   (0x4200402CU) /**< \brief (ADC) Test Result Register 0 */
#define REG_ADC_TESTRESULT1                   (0x42004030U) /**< \brief (ADC) Test Result Register 1 */
#define REG_ADC_TESTRESULT2                   (0x42004034U) /**< \brief (ADC) Test Result Register 2 */
#define REG_ADC_TESTRESULT3                   (0x42004038U) /**< \brief (ADC) Test Result Register 3 */
#define REG_ADC_DCFG0                         (0x42004030U) /**< \brief (ADC) Device Configuration 0 */
#define REG_ADC_DCFG1                         (0x42004031U) /**< \brief (ADC) Device Configuration 1 */
#define REG_ADC_DCFG2                         (0x42004032U) /**< \brief (ADC) Device Configuration 2 */
#define REG_ADC_DCFG3                         (0x42004033U) /**< \brief (ADC) Device Configuration 3 */
#else
#define REG_ADC_CTRLA              (*(RwReg8 *)0x42004000U) /**< \brief (ADC) Control Register A */
#define REG_ADC_REFCTRL            (*(RwReg8 *)0x42004001U) /**< \brief (ADC) Reference Control Register */
#define REG_ADC_AVGCTRL            (*(RwReg8 *)0x42004002U) /**< \brief (ADC) Average Control Register */
#define REG_ADC_SAMPCTRL           (*(RwReg8 *)0x42004003U) /**< \brief (ADC) Sample Time Control Register */
#define REG_ADC_CTRLB              (*(RwReg16*)0x42004004U) /**< \brief (ADC) Control Register B */
#define REG_ADC_WINCTRL            (*(RwReg8 *)0x42004008U) /**< \brief (ADC) Window Monitor Control Register */
#define REG_ADC_SWTRIG             (*(RwReg8 *)0x4200400CU) /**< \brief (ADC) Control Register B */
#define REG_ADC_INPUTCTRL          (*(RwReg  *)0x42004010U) /**< \brief (ADC) Input Control Register */
#define REG_ADC_EVCTRL             (*(RwReg8 *)0x42004014U) /**< \brief (ADC) Event Control Register */
#define REG_ADC_INTENCLR           (*(RwReg8 *)0x42004016U) /**< \brief (ADC) Interrupt Enable Clear Register */
#define REG_ADC_INTENSET           (*(RwReg8 *)0x42004017U) /**< \brief (ADC) Interrupt Enable Set Register */
#define REG_ADC_INTFLAG            (*(RwReg8 *)0x42004018U) /**< \brief (ADC) Interrupt Flag Status and Clear Register */
#define REG_ADC_STATUS             (*(RoReg8 *)0x42004019U) /**< \brief (ADC) Status Register */
#define REG_ADC_RESULT             (*(RoReg16*)0x4200401AU) /**< \brief (ADC) Result Register */
#define REG_ADC_WINLT              (*(RwReg16*)0x4200401CU) /**< \brief (ADC) Window Monitor Lower Threshold Register */
#define REG_ADC_WINUT              (*(RwReg16*)0x42004020U) /**< \brief (ADC) Window Monitor Upper Threshold Register */
#define REG_ADC_GAINCORR           (*(RwReg16*)0x42004024U) /**< \brief (ADC) Gain Correction Register */
#define REG_ADC_OFFSETCORR         (*(RwReg16*)0x42004026U) /**< \brief (ADC) Offset Correction Register */
#define REG_ADC_CALIB              (*(RwReg16*)0x42004028U) /**< \brief (ADC) Calibration Register */
#define REG_ADC_DEBUG              (*(RwReg8 *)0x4200402AU) /**< \brief (ADC) Debug Register */
#define REG_ADC_TEST               (*(RwReg8 *)0x4200402BU) /**< \brief (ADC) Test Modes Register */
#define REG_ADC_TESTRESULT         (*(RwReg  *)0x4200402CU) /**< \brief (ADC) Test Result Register */
#define REG_ADC_DCFG               (*(RwReg8 *)0x42004030U) /**< \brief (ADC) Device Configuration */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for ADC peripheral ========== */
#define ADC_ADC_RESULT_MSB          15
#define ADC_GCLK_ID                 23

#endif /* _SAMD20_ADC_INSTANCE_ */
