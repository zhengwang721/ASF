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

#ifndef _SAM0D4_REVA_
#define _SAM0D4_REVA_

/** \addtogroup sam0d4_reva_definitions SAM0D4_REVA definitions
  This file defines all structures and symbols for SAM0D4_REVA:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - PIO definitions
*/
/*@{*/

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef __ASSEMBLY__
#include <stdint.h>
#ifndef __cplusplus
typedef volatile const uint32_t RoReg;   /**< Read only 32-bit register (volatile const unsigned int) */
typedef volatile const uint16_t RoReg16; /**< Read only 16-bit register (volatile const unsigned int) */
typedef volatile const uint8_t  RoReg8;  /**< Read only  8-bit register (volatile const unsigned int) */
#else
typedef volatile       uint32_t RoReg;   /**< Read only 32-bit register (volatile const unsigned int) */
typedef volatile       uint16_t RoReg16; /**< Read only 16-bit register (volatile const unsigned int) */
typedef volatile       uint8_t  RoReg8;  /**< Read only  8-bit register (volatile const unsigned int) */
#endif
typedef volatile       uint32_t WoReg;   /**< Write only 32-bit register (volatile unsigned int) */
typedef volatile       uint16_t WoReg16; /**< Write only 16-bit register (volatile unsigned int) */
typedef volatile       uint32_t WoReg8;  /**< Write only  8-bit register (volatile unsigned int) */
typedef volatile       uint32_t RwReg;   /**< Read-Write 32-bit register (volatile unsigned int) */
typedef volatile       uint16_t RwReg16; /**< Read-Write 16-bit register (volatile unsigned int) */
typedef volatile       uint8_t  RwReg8;  /**< Read-Write  8-bit register (volatile unsigned int) */
#define CAST(type, value) ((type *)(value))
#define REG_ACCESS(type, address) (*(type*)(address)) /**< C code: Register value */
#else
#define CAST(type, value) (value)
#define REG_ACCESS(type, address) (address) /**< Assembly code: Register address */
#endif

/* ************************************************************************** */
/**  CMSIS DEFINITIONS FOR SAM0D4_REVA */
/* ************************************************************************** */
/** \addtogroup SAM0D4_REVA_cmsis CMSIS Definitions */
/*@{*/


/**< Interrupt Number Definition */
typedef enum IRQn
{
  /******  Cortex-M0+ Processor Exceptions Numbers ****************************/
  NonMaskableInt_IRQn   = -14, /**<  2 Non Maskable Interrupt                 */
  HardFault_IRQn        = -13, /**<  3 Cortex-M0+ Hard Fault Interrupt        */
  MemoryManagement_IRQn = -12, /**<  4 Cortex-M0+ Memory Management Interrupt */
  BusFault_IRQn         = -11, /**<  5 Cortex-M0+ Bus Fault Interrupt         */
  UsageFault_IRQn       = -10, /**<  6 Cortex-M0+ Usage Fault Interrupt       */
  SVCall_IRQn           = -5,  /**< 11 Cortex-M0+ SV Call Interrupt           */
  DebugMonitor_IRQn     = -4,  /**< 12 Cortex-M0+ Debug Monitor Interrupt     */
  PendSV_IRQn           = -2,  /**< 14 Cortex-M0+ Pend SV Interrupt           */
  SysTick_IRQn          = -1,  /**< 15 Cortex-M0+ System Tick Interrupt       */
  /******  SAM0D4_REVA-specific Interrupt Numbers ***********************/
  PM_CFD_IRQn           =  0, /**<  0 SAM0D4_REVA Power Manager (PM) */
  PM_CKRDY_IRQn         =  0, /**<  0 SAM0D4_REVA Power Manager (PM) */
  SYSCTRL_BOD12DET_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_BOD12RDY_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_BOD12SYNRDY_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_BOD33DET_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_BOD33RDY_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_BOD33SYNRDY_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_DFLLLOCKC_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_DFLLLOCKF_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_DFLLOOB_IRQn  =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_DFLLRCS_IRQn  =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_DFLLRDY_IRQn  =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_OSC32KRDY_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_XOSCRDY_IRQn  =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  SYSCTRL_XOSC32RDY_IRQn =  1, /**<  1 SAM0D4_REVA System Control (SYSCTRL) */
  WDT_EW_IRQn           =  2, /**<  2 SAM0D4_REVA Watchdog Timer (WDT) */
  RTC_CMP_0_IRQn        =  3, /**<  3 SAM0D4_REVA Real-Time Counter (RTC) */
  RTC_CMP_1_IRQn        =  3, /**<  3 SAM0D4_REVA Real-Time Counter (RTC) */
  RTC_OVF_IRQn          =  3, /**<  3 SAM0D4_REVA Real-Time Counter (RTC) */
  RTC_READY_IRQn        =  3, /**<  3 SAM0D4_REVA Real-Time Counter (RTC) */
  EIC_EXTINT_0_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_1_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_2_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_3_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_4_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_5_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_6_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_7_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_8_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_9_IRQn     =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_10_IRQn    =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_11_IRQn    =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_12_IRQn    =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_13_IRQn    =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_14_IRQn    =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  EIC_EXTINT_15_IRQn    =  4, /**<  4 SAM0D4_REVA External Interrupt Controller (EIC) */
  NVMCTRL_0_IRQn        =  5, /**<  5 SAM0D4_REVA NVM Controller (NVMCTRL) */
  NVMCTRL_1_IRQn        =  5, /**<  5 SAM0D4_REVA NVM Controller (NVMCTRL) */
  EVSYS_EVD_0_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_EVD_1_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_EVD_2_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_EVD_3_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_EVD_4_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_EVD_5_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_EVD_6_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_EVD_7_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_OVR_0_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_OVR_1_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_OVR_2_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_OVR_3_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_OVR_4_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_OVR_5_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_OVR_6_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  EVSYS_OVR_7_IRQn      =  6, /**<  6 SAM0D4_REVA Event System Interface (EVSYS) */
  SERCOM0_0_IRQn        =  7, /**<  7 SAM0D4_REVA Serial Communication Interface 0 (SERCOM0) */
  SERCOM0_1_IRQn        =  7, /**<  7 SAM0D4_REVA Serial Communication Interface 0 (SERCOM0) */
  SERCOM0_2_IRQn        =  7, /**<  7 SAM0D4_REVA Serial Communication Interface 0 (SERCOM0) */
  SERCOM0_3_IRQn        =  7, /**<  7 SAM0D4_REVA Serial Communication Interface 0 (SERCOM0) */
  SERCOM1_0_IRQn        =  8, /**<  8 SAM0D4_REVA Serial Communication Interface 1 (SERCOM1) */
  SERCOM1_1_IRQn        =  8, /**<  8 SAM0D4_REVA Serial Communication Interface 1 (SERCOM1) */
  SERCOM1_2_IRQn        =  8, /**<  8 SAM0D4_REVA Serial Communication Interface 1 (SERCOM1) */
  SERCOM1_3_IRQn        =  8, /**<  8 SAM0D4_REVA Serial Communication Interface 1 (SERCOM1) */
  SERCOM2_0_IRQn        =  9, /**<  9 SAM0D4_REVA Serial Communication Interface 2 (SERCOM2) */
  SERCOM2_1_IRQn        =  9, /**<  9 SAM0D4_REVA Serial Communication Interface 2 (SERCOM2) */
  SERCOM2_2_IRQn        =  9, /**<  9 SAM0D4_REVA Serial Communication Interface 2 (SERCOM2) */
  SERCOM2_3_IRQn        =  9, /**<  9 SAM0D4_REVA Serial Communication Interface 2 (SERCOM2) */
  SERCOM3_0_IRQn        = 10, /**< 10 SAM0D4_REVA Serial Communication Interface 3 (SERCOM3) */
  SERCOM3_1_IRQn        = 10, /**< 10 SAM0D4_REVA Serial Communication Interface 3 (SERCOM3) */
  SERCOM3_2_IRQn        = 10, /**< 10 SAM0D4_REVA Serial Communication Interface 3 (SERCOM3) */
  SERCOM3_3_IRQn        = 10, /**< 10 SAM0D4_REVA Serial Communication Interface 3 (SERCOM3) */
  SERCOM4_0_IRQn        = 11, /**< 11 SAM0D4_REVA Serial Communication Interface 4 (SERCOM4) */
  SERCOM4_1_IRQn        = 11, /**< 11 SAM0D4_REVA Serial Communication Interface 4 (SERCOM4) */
  SERCOM4_2_IRQn        = 11, /**< 11 SAM0D4_REVA Serial Communication Interface 4 (SERCOM4) */
  SERCOM4_3_IRQn        = 11, /**< 11 SAM0D4_REVA Serial Communication Interface 4 (SERCOM4) */
  SERCOM5_0_IRQn        = 12, /**< 12 SAM0D4_REVA Serial Communication Interface 5 (SERCOM5) */
  SERCOM5_1_IRQn        = 12, /**< 12 SAM0D4_REVA Serial Communication Interface 5 (SERCOM5) */
  SERCOM5_2_IRQn        = 12, /**< 12 SAM0D4_REVA Serial Communication Interface 5 (SERCOM5) */
  SERCOM5_3_IRQn        = 12, /**< 12 SAM0D4_REVA Serial Communication Interface 5 (SERCOM5) */
  TC0_ERR_IRQn          = 13, /**< 13 SAM0D4_REVA Basic Timer Counter 0 (TC0) */
  TC0_MC_0_IRQn         = 13, /**< 13 SAM0D4_REVA Basic Timer Counter 0 (TC0) */
  TC0_MC_1_IRQn         = 13, /**< 13 SAM0D4_REVA Basic Timer Counter 0 (TC0) */
  TC0_OVF_IRQn          = 13, /**< 13 SAM0D4_REVA Basic Timer Counter 0 (TC0) */
  TC0_READY_IRQn        = 13, /**< 13 SAM0D4_REVA Basic Timer Counter 0 (TC0) */
  TC1_ERR_IRQn          = 14, /**< 14 SAM0D4_REVA Basic Timer Counter 1 (TC1) */
  TC1_MC_0_IRQn         = 14, /**< 14 SAM0D4_REVA Basic Timer Counter 1 (TC1) */
  TC1_MC_1_IRQn         = 14, /**< 14 SAM0D4_REVA Basic Timer Counter 1 (TC1) */
  TC1_OVF_IRQn          = 14, /**< 14 SAM0D4_REVA Basic Timer Counter 1 (TC1) */
  TC1_READY_IRQn        = 14, /**< 14 SAM0D4_REVA Basic Timer Counter 1 (TC1) */
  TC2_ERR_IRQn          = 15, /**< 15 SAM0D4_REVA Basic Timer Counter 2 (TC2) */
  TC2_MC_0_IRQn         = 15, /**< 15 SAM0D4_REVA Basic Timer Counter 2 (TC2) */
  TC2_MC_1_IRQn         = 15, /**< 15 SAM0D4_REVA Basic Timer Counter 2 (TC2) */
  TC2_OVF_IRQn          = 15, /**< 15 SAM0D4_REVA Basic Timer Counter 2 (TC2) */
  TC2_READY_IRQn        = 15, /**< 15 SAM0D4_REVA Basic Timer Counter 2 (TC2) */
  TC3_ERR_IRQn          = 16, /**< 16 SAM0D4_REVA Basic Timer Counter 3 (TC3) */
  TC3_MC_0_IRQn         = 16, /**< 16 SAM0D4_REVA Basic Timer Counter 3 (TC3) */
  TC3_MC_1_IRQn         = 16, /**< 16 SAM0D4_REVA Basic Timer Counter 3 (TC3) */
  TC3_OVF_IRQn          = 16, /**< 16 SAM0D4_REVA Basic Timer Counter 3 (TC3) */
  TC3_READY_IRQn        = 16, /**< 16 SAM0D4_REVA Basic Timer Counter 3 (TC3) */
  TC4_ERR_IRQn          = 17, /**< 17 SAM0D4_REVA Basic Timer Counter 4 (TC4) */
  TC4_MC_0_IRQn         = 17, /**< 17 SAM0D4_REVA Basic Timer Counter 4 (TC4) */
  TC4_MC_1_IRQn         = 17, /**< 17 SAM0D4_REVA Basic Timer Counter 4 (TC4) */
  TC4_OVF_IRQn          = 17, /**< 17 SAM0D4_REVA Basic Timer Counter 4 (TC4) */
  TC4_READY_IRQn        = 17, /**< 17 SAM0D4_REVA Basic Timer Counter 4 (TC4) */
  TC5_ERR_IRQn          = 18, /**< 18 SAM0D4_REVA Basic Timer Counter 5 (TC5) */
  TC5_MC_0_IRQn         = 18, /**< 18 SAM0D4_REVA Basic Timer Counter 5 (TC5) */
  TC5_MC_1_IRQn         = 18, /**< 18 SAM0D4_REVA Basic Timer Counter 5 (TC5) */
  TC5_OVF_IRQn          = 18, /**< 18 SAM0D4_REVA Basic Timer Counter 5 (TC5) */
  TC5_READY_IRQn        = 18, /**< 18 SAM0D4_REVA Basic Timer Counter 5 (TC5) */
  TC6_ERR_IRQn          = 19, /**< 19 SAM0D4_REVA Basic Timer Counter 6 (TC6) */
  TC6_MC_0_IRQn         = 19, /**< 19 SAM0D4_REVA Basic Timer Counter 6 (TC6) */
  TC6_MC_1_IRQn         = 19, /**< 19 SAM0D4_REVA Basic Timer Counter 6 (TC6) */
  TC6_OVF_IRQn          = 19, /**< 19 SAM0D4_REVA Basic Timer Counter 6 (TC6) */
  TC6_READY_IRQn        = 19, /**< 19 SAM0D4_REVA Basic Timer Counter 6 (TC6) */
  TC7_ERR_IRQn          = 20, /**< 20 SAM0D4_REVA Basic Timer Counter 7 (TC7) */
  TC7_MC_0_IRQn         = 20, /**< 20 SAM0D4_REVA Basic Timer Counter 7 (TC7) */
  TC7_MC_1_IRQn         = 20, /**< 20 SAM0D4_REVA Basic Timer Counter 7 (TC7) */
  TC7_OVF_IRQn          = 20, /**< 20 SAM0D4_REVA Basic Timer Counter 7 (TC7) */
  TC7_READY_IRQn        = 20, /**< 20 SAM0D4_REVA Basic Timer Counter 7 (TC7) */
  ADC_OVERRUN_IRQn      = 21, /**< 21 SAM0D4_REVA Analog Digital Converter (ADC) */
  ADC_READY_IRQn        = 21, /**< 21 SAM0D4_REVA Analog Digital Converter (ADC) */
  ADC_RESRDY_IRQn       = 21, /**< 21 SAM0D4_REVA Analog Digital Converter (ADC) */
  ADC_WINMON_IRQn       = 21, /**< 21 SAM0D4_REVA Analog Digital Converter (ADC) */
  AC_COMP_0_IRQn        = 22, /**< 22 SAM0D4_REVA Analog Comparators (AC) */
  AC_COMP_1_IRQn        = 22, /**< 22 SAM0D4_REVA Analog Comparators (AC) */
  AC_WIN_0_IRQn         = 22, /**< 22 SAM0D4_REVA Analog Comparators (AC) */
  DAC_EMPTY_IRQn        = 23, /**< 23 SAM0D4_REVA Digital Analog Converter (DAC) */
  DAC_READY_IRQn        = 23, /**< 23 SAM0D4_REVA Digital Analog Converter (DAC) */
  DAC_UNDERRUN_IRQn     = 23, /**< 23 SAM0D4_REVA Digital Analog Converter (DAC) */
  PTC_EOC_IRQn          = 24, /**< 24 SAM0D4_REVA Peripheral Touch Controller (PTC) */
  PTC_READY_IRQn        = 24, /**< 24 SAM0D4_REVA Peripheral Touch Controller (PTC) */
  PTC_WCOMP_IRQn        = 24  /**< 24 SAM0D4_REVA Peripheral Touch Controller (PTC) */
} IRQn_Type;


/* Cortex-M4 core handlers */
void Reset_Handler      ( void );
void NMI_Handler        ( void );
void HardFault_Handler  ( void );
void MemManage_Handler  ( void );
void BusFault_Handler   ( void );
void UsageFault_Handler ( void );
void SVC_Handler        ( void );
void DebugMon_Handler   ( void );
void PendSV_Handler     ( void );
void SysTick_Handler    ( void );

/* Peripherals handlers */

void PM_CFD_Handler     ( void );
void PM_CKRDY_Handler   ( void );
void SYSCTRL_BOD12DET_Handler ( void );
void SYSCTRL_BOD12RDY_Handler ( void );
void SYSCTRL_BOD12SYNRDY_Handler ( void );
void SYSCTRL_BOD33DET_Handler ( void );
void SYSCTRL_BOD33RDY_Handler ( void );
void SYSCTRL_BOD33SYNRDY_Handler ( void );
void SYSCTRL_DFLLLOCKC_Handler ( void );
void SYSCTRL_DFLLLOCKF_Handler ( void );
void SYSCTRL_DFLLOOB_Handler ( void );
void SYSCTRL_DFLLRCS_Handler ( void );
void SYSCTRL_DFLLRDY_Handler ( void );
void SYSCTRL_OSC32KRDY_Handler ( void );
void SYSCTRL_XOSCRDY_Handler ( void );
void SYSCTRL_XOSC32RDY_Handler ( void );
void WDT_EW_Handler     ( void );
void RTC_CMP_0_Handler  ( void );
void RTC_CMP_1_Handler  ( void );
void RTC_OVF_Handler    ( void );
void RTC_READY_Handler  ( void );
void EIC_EXTINT_0_Handler ( void );
void EIC_EXTINT_1_Handler ( void );
void EIC_EXTINT_2_Handler ( void );
void EIC_EXTINT_3_Handler ( void );
void EIC_EXTINT_4_Handler ( void );
void EIC_EXTINT_5_Handler ( void );
void EIC_EXTINT_6_Handler ( void );
void EIC_EXTINT_7_Handler ( void );
void EIC_EXTINT_8_Handler ( void );
void EIC_EXTINT_9_Handler ( void );
void EIC_EXTINT_10_Handler ( void );
void EIC_EXTINT_11_Handler ( void );
void EIC_EXTINT_12_Handler ( void );
void EIC_EXTINT_13_Handler ( void );
void EIC_EXTINT_14_Handler ( void );
void EIC_EXTINT_15_Handler ( void );
void NVMCTRL_0_Handler  ( void );
void NVMCTRL_1_Handler  ( void );
void EVSYS_EVD_0_Handler ( void );
void EVSYS_EVD_1_Handler ( void );
void EVSYS_EVD_2_Handler ( void );
void EVSYS_EVD_3_Handler ( void );
void EVSYS_EVD_4_Handler ( void );
void EVSYS_EVD_5_Handler ( void );
void EVSYS_EVD_6_Handler ( void );
void EVSYS_EVD_7_Handler ( void );
void EVSYS_OVR_0_Handler ( void );
void EVSYS_OVR_1_Handler ( void );
void EVSYS_OVR_2_Handler ( void );
void EVSYS_OVR_3_Handler ( void );
void EVSYS_OVR_4_Handler ( void );
void EVSYS_OVR_5_Handler ( void );
void EVSYS_OVR_6_Handler ( void );
void EVSYS_OVR_7_Handler ( void );
void SERCOM0_0_Handler  ( void );
void SERCOM0_1_Handler  ( void );
void SERCOM0_2_Handler  ( void );
void SERCOM0_3_Handler  ( void );
void SERCOM1_0_Handler  ( void );
void SERCOM1_1_Handler  ( void );
void SERCOM1_2_Handler  ( void );
void SERCOM1_3_Handler  ( void );
void SERCOM2_0_Handler  ( void );
void SERCOM2_1_Handler  ( void );
void SERCOM2_2_Handler  ( void );
void SERCOM2_3_Handler  ( void );
void SERCOM3_0_Handler  ( void );
void SERCOM3_1_Handler  ( void );
void SERCOM3_2_Handler  ( void );
void SERCOM3_3_Handler  ( void );
void SERCOM4_0_Handler  ( void );
void SERCOM4_1_Handler  ( void );
void SERCOM4_2_Handler  ( void );
void SERCOM4_3_Handler  ( void );
void SERCOM5_0_Handler  ( void );
void SERCOM5_1_Handler  ( void );
void SERCOM5_2_Handler  ( void );
void SERCOM5_3_Handler  ( void );
void TC0_ERR_Handler    ( void );
void TC0_MC_0_Handler   ( void );
void TC0_MC_1_Handler   ( void );
void TC0_OVF_Handler    ( void );
void TC0_READY_Handler  ( void );
void TC1_ERR_Handler    ( void );
void TC1_MC_0_Handler   ( void );
void TC1_MC_1_Handler   ( void );
void TC1_OVF_Handler    ( void );
void TC1_READY_Handler  ( void );
void TC2_ERR_Handler    ( void );
void TC2_MC_0_Handler   ( void );
void TC2_MC_1_Handler   ( void );
void TC2_OVF_Handler    ( void );
void TC2_READY_Handler  ( void );
void TC3_ERR_Handler    ( void );
void TC3_MC_0_Handler   ( void );
void TC3_MC_1_Handler   ( void );
void TC3_OVF_Handler    ( void );
void TC3_READY_Handler  ( void );
void TC4_ERR_Handler    ( void );
void TC4_MC_0_Handler   ( void );
void TC4_MC_1_Handler   ( void );
void TC4_OVF_Handler    ( void );
void TC4_READY_Handler  ( void );
void TC5_ERR_Handler    ( void );
void TC5_MC_0_Handler   ( void );
void TC5_MC_1_Handler   ( void );
void TC5_OVF_Handler    ( void );
void TC5_READY_Handler  ( void );
void TC6_ERR_Handler    ( void );
void TC6_MC_0_Handler   ( void );
void TC6_MC_1_Handler   ( void );
void TC6_OVF_Handler    ( void );
void TC6_READY_Handler  ( void );
void TC7_ERR_Handler    ( void );
void TC7_MC_0_Handler   ( void );
void TC7_MC_1_Handler   ( void );
void TC7_OVF_Handler    ( void );
void TC7_READY_Handler  ( void );
void ADC_OVERRUN_Handler ( void );
void ADC_READY_Handler  ( void );
void ADC_RESRDY_Handler ( void );
void ADC_WINMON_Handler ( void );
void AC_COMP_0_Handler  ( void );
void AC_COMP_1_Handler  ( void );
void AC_WIN_0_Handler   ( void );
void DAC_EMPTY_Handler  ( void );
void DAC_READY_Handler  ( void );
void DAC_UNDERRUN_Handler ( void );
void PTC_EOC_Handler    ( void );
void PTC_READY_Handler  ( void );
void PTC_WCOMP_Handler  ( void );

/*
 * \brief Configuration of the Cortex-M0+ CPU and Core Peripherals
 */

#define LITTLE_ENDIAN          1
#define __CM0P_REV             0x0000    /*!< Core revision r0p0 */
#define __MPU_PRESENT          0         /*!< MPU present or not */
#define __NVIC_PRIO_BITS       2         /*!< Number of bits used for Priority Levels */
#define __VTOR_PRESENT         1         /*!< VTOR present or not */
#define __Vendor_SysTickConfig 0         /*!< Set to 1 if different SysTick Config is used */

/**
 * \brief CMSIS includes
 */

#include <core_cm0.h>

/*@}*/

/* ************************************************************************** */
/**  SOFTWARE PERIPHERAL API DEFINITION FOR SAM0D4_REVA */
/* ************************************************************************** */
/** \addtogroup SAM0D4_REVA_api Peripheral Software API */
/*@{*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR AC */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_AC Analog Comparators */
/*@{*/

#define REV_AC         0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_AC_FROM_CMSIS__

/* -------- AC_CTRLA : (AC Offset: 0x00) (R/W  8) Control A Register -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable                             */
    uint8_t  SLEEPEN:1;        /*!< bit:      2  Sleep Enable                       */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_CTRLA_Type;

#define AC_CTRLA_OFFSET             0x00         /**< \brief (AC offset) Control A Register */
#define AC_CTRLA_RESETVALUE         0x0          /**< \brief (AC reset_value) Control A Register */

#define AC_CTRLA_SWRST              (0x1u <<  0) /**< \brief (AC_CTRLA) Software Reset */
#define AC_CTRLA_ENABLE             (0x1u <<  1) /**< \brief (AC_CTRLA) Enable */
#define AC_CTRLA_SLEEPEN            (0x1u <<  2) /**< \brief (AC_CTRLA) Sleep Enable */
#define AC_CTRLA_MASK               0x7u /**< \brief (AC_CTRLA) MASK Register */

/* -------- AC_CTRLB : (AC Offset: 0x01) ( /W  8) Control B Register -------- */
typedef union {
  struct {
    uint8_t  START0:1;         /*!< bit:      0                                     */
    uint8_t  START1:1;         /*!< bit:      1                                     */
    uint8_t  START2:1;         /*!< bit:      2                                     */
    uint8_t  START3:1;         /*!< bit:      3                                     */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_CTRLB_Type;

#define AC_CTRLB_OFFSET             0x01         /**< \brief (AC offset) Control B Register */
#define AC_CTRLB_RESETVALUE         0x0          /**< \brief (AC reset_value) Control B Register */

#define AC_CTRLB_START0             (0x1u <<  0) /**< \brief (AC_CTRLB)  */
#define AC_CTRLB_START1             (0x1u <<  1) /**< \brief (AC_CTRLB)  */
#define AC_CTRLB_START2             (0x1u <<  2) /**< \brief (AC_CTRLB)  */
#define AC_CTRLB_START3             (0x1u <<  3) /**< \brief (AC_CTRLB)  */
#define AC_CTRLB_MASK               0xFu /**< \brief (AC_CTRLB) MASK Register */

/* -------- AC_EVCTRL : (AC Offset: 0x02) (R/W 16) Event Control Register -------- */
typedef union {
  struct {
    uint16_t COMPEO0:1;        /*!< bit:      0                                     */
    uint16_t COMPEO1:1;        /*!< bit:      1                                     */
    uint16_t COMPEO2:1;        /*!< bit:      2                                     */
    uint16_t COMPEO3:1;        /*!< bit:      3                                     */
    uint16_t WINEO0:1;         /*!< bit:      4                                     */
    uint16_t WINEO1:1;         /*!< bit:      5                                     */
    uint16_t _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
    uint16_t COMPEI0:1;        /*!< bit:      8                                     */
    uint16_t COMPEI1:1;        /*!< bit:      9                                     */
    uint16_t COMPEI2:1;        /*!< bit:     10                                     */
    uint16_t COMPEI3:1;        /*!< bit:     11                                     */
    uint16_t _reserved1:4;     /*!< bit: 12..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} AC_EVCTRL_Type;

#define AC_EVCTRL_OFFSET            0x02         /**< \brief (AC offset) Event Control Register */
#define AC_EVCTRL_RESETVALUE        0x0          /**< \brief (AC reset_value) Event Control Register */

#define AC_EVCTRL_COMPEO0           (0x1u <<  0) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_COMPEO1           (0x1u <<  1) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_COMPEO2           (0x1u <<  2) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_COMPEO3           (0x1u <<  3) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_WINEO0            (0x1u <<  4) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_WINEO1            (0x1u <<  5) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_COMPEI0           (0x1u <<  8) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_COMPEI1           (0x1u <<  9) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_COMPEI2           (0x1u << 10) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_COMPEI3           (0x1u << 11) /**< \brief (AC_EVCTRL)  */
#define AC_EVCTRL_MASK              0xF3Fu /**< \brief (AC_EVCTRL) MASK Register */

/* -------- AC_INTENCLR : (AC Offset: 0x04) (R/W  8) Interrupt Enable Clear Register -------- */
typedef union {
  struct {
    uint8_t  COMP0:1;          /*!< bit:      0                                     */
    uint8_t  COMP1:1;          /*!< bit:      1                                     */
    uint8_t  COMP2:1;          /*!< bit:      2                                     */
    uint8_t  COMP3:1;          /*!< bit:      3                                     */
    uint8_t  WIN0:1;           /*!< bit:      4                                     */
    uint8_t  WIN1:1;           /*!< bit:      5                                     */
    uint8_t  _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_INTENCLR_Type;

#define AC_INTENCLR_OFFSET          0x04         /**< \brief (AC offset) Interrupt Enable Clear Register */
#define AC_INTENCLR_RESETVALUE      0x0          /**< \brief (AC reset_value) Interrupt Enable Clear Register */

#define AC_INTENCLR_COMP0           (0x1u <<  0) /**< \brief (AC_INTENCLR)  */
#define AC_INTENCLR_COMP1           (0x1u <<  1) /**< \brief (AC_INTENCLR)  */
#define AC_INTENCLR_COMP2           (0x1u <<  2) /**< \brief (AC_INTENCLR)  */
#define AC_INTENCLR_COMP3           (0x1u <<  3) /**< \brief (AC_INTENCLR)  */
#define AC_INTENCLR_WIN0            (0x1u <<  4) /**< \brief (AC_INTENCLR)  */
#define AC_INTENCLR_WIN1            (0x1u <<  5) /**< \brief (AC_INTENCLR)  */
#define AC_INTENCLR_MASK            0x3Fu /**< \brief (AC_INTENCLR) MASK Register */

/* -------- AC_INTENSET : (AC Offset: 0x05) (R/W  8) Interupt Enable Set Register -------- */
typedef union {
  struct {
    uint8_t  COMP0:1;          /*!< bit:      0                                     */
    uint8_t  COMP1:1;          /*!< bit:      1                                     */
    uint8_t  COMP2:1;          /*!< bit:      2                                     */
    uint8_t  COMP3:1;          /*!< bit:      3                                     */
    uint8_t  WIN0:1;           /*!< bit:      4                                     */
    uint8_t  WIN1:1;           /*!< bit:      5                                     */
    uint8_t  _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_INTENSET_Type;

#define AC_INTENSET_OFFSET          0x05         /**< \brief (AC offset) Interupt Enable Set Register */
#define AC_INTENSET_RESETVALUE      0x0          /**< \brief (AC reset_value) Interupt Enable Set Register */

#define AC_INTENSET_COMP0           (0x1u <<  0) /**< \brief (AC_INTENSET)  */
#define AC_INTENSET_COMP1           (0x1u <<  1) /**< \brief (AC_INTENSET)  */
#define AC_INTENSET_COMP2           (0x1u <<  2) /**< \brief (AC_INTENSET)  */
#define AC_INTENSET_COMP3           (0x1u <<  3) /**< \brief (AC_INTENSET)  */
#define AC_INTENSET_WIN0            (0x1u <<  4) /**< \brief (AC_INTENSET)  */
#define AC_INTENSET_WIN1            (0x1u <<  5) /**< \brief (AC_INTENSET)  */
#define AC_INTENSET_MASK            0x3Fu /**< \brief (AC_INTENSET) MASK Register */

/* -------- AC_INTFLAG : (AC Offset: 0x06) (R/W  8) Interrupt Flag Register -------- */
typedef union {
  struct {
    uint8_t  COMP0:1;          /*!< bit:      0                                     */
    uint8_t  COMP1:1;          /*!< bit:      1                                     */
    uint8_t  COMP2:1;          /*!< bit:      2                                     */
    uint8_t  COMP3:1;          /*!< bit:      3                                     */
    uint8_t  WIN0:1;           /*!< bit:      4                                     */
    uint8_t  WIN1:1;           /*!< bit:      5                                     */
    uint8_t  _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_INTFLAG_Type;

#define AC_INTFLAG_OFFSET           0x06         /**< \brief (AC offset) Interrupt Flag Register */
#define AC_INTFLAG_RESETVALUE       0x0          /**< \brief (AC reset_value) Interrupt Flag Register */

#define AC_INTFLAG_COMP0            (0x1u <<  0) /**< \brief (AC_INTFLAG)  */
#define AC_INTFLAG_COMP1            (0x1u <<  1) /**< \brief (AC_INTFLAG)  */
#define AC_INTFLAG_COMP2            (0x1u <<  2) /**< \brief (AC_INTFLAG)  */
#define AC_INTFLAG_COMP3            (0x1u <<  3) /**< \brief (AC_INTFLAG)  */
#define AC_INTFLAG_WIN0             (0x1u <<  4) /**< \brief (AC_INTFLAG)  */
#define AC_INTFLAG_WIN1             (0x1u <<  5) /**< \brief (AC_INTFLAG)  */
#define AC_INTFLAG_MASK             0x3Fu /**< \brief (AC_INTFLAG) MASK Register */

/* -------- AC_STATUSA : (AC Offset: 0x08) (R/   8) Status A Register -------- */
typedef union {
  struct {
    uint8_t  STATE0:1;         /*!< bit:      0                                     */
    uint8_t  STATE1:1;         /*!< bit:      1                                     */
    uint8_t  STATE2:1;         /*!< bit:      2                                     */
    uint8_t  STATE3:1;         /*!< bit:      3                                     */
    uint8_t  WSTATE0:2;        /*!< bit:  4.. 5                                     */
    uint8_t  WSTATE1:2;        /*!< bit:  6.. 7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_STATUSA_Type;

#define AC_STATUSA_OFFSET           0x08         /**< \brief (AC offset) Status A Register */
#define AC_STATUSA_RESETVALUE       0x0          /**< \brief (AC reset_value) Status A Register */

#define AC_STATUSA_STATE0           (0x1u <<  0) /**< \brief (AC_STATUSA)  */
#define AC_STATUSA_STATE1           (0x1u <<  1) /**< \brief (AC_STATUSA)  */
#define AC_STATUSA_STATE2           (0x1u <<  2) /**< \brief (AC_STATUSA)  */
#define AC_STATUSA_STATE3           (0x1u <<  3) /**< \brief (AC_STATUSA)  */
#define AC_STATUSA_WSTATE0_Pos      4
#define AC_STATUSA_WSTATE0_Msk      (0x3u << AC_STATUSA_WSTATE0_Pos) /**< \brief (AC_STATUSA)  */
#define AC_STATUSA_WSTATE0(value)   ((AC_STATUSA_WSTATE0_Msk & ((value) << AC_STATUSA_WSTATE0_Pos)))
#define   AC_STATUSA_WSTATE0_ABOVE  (0x0u <<  4) /**< \brief (AC_STATUSA)  */
#define   AC_STATUSA_WSTATE0_INSIDE (0x1u <<  4) /**< \brief (AC_STATUSA)  */
#define   AC_STATUSA_WSTATE0_BELOW  (0x2u <<  4) /**< \brief (AC_STATUSA)  */
#define AC_STATUSA_WSTATE1_Pos      6
#define AC_STATUSA_WSTATE1_Msk      (0x3u << AC_STATUSA_WSTATE1_Pos) /**< \brief (AC_STATUSA)  */
#define AC_STATUSA_WSTATE1(value)   ((AC_STATUSA_WSTATE1_Msk & ((value) << AC_STATUSA_WSTATE1_Pos)))
#define   AC_STATUSA_WSTATE1_ABOVE  (0x0u <<  6) /**< \brief (AC_STATUSA)  */
#define   AC_STATUSA_WSTATE1_INSIDE (0x1u <<  6) /**< \brief (AC_STATUSA)  */
#define   AC_STATUSA_WSTATE1_BELOW  (0x2u <<  6) /**< \brief (AC_STATUSA)  */
#define AC_STATUSA_MASK             0xFFu /**< \brief (AC_STATUSA) MASK Register */

/* -------- AC_STATUSB : (AC Offset: 0x09) (R/   8) Status B Register -------- */
typedef union {
  struct {
    uint8_t  READY0:1;         /*!< bit:      0                                     */
    uint8_t  READY1:1;         /*!< bit:      1                                     */
    uint8_t  READY2:1;         /*!< bit:      2                                     */
    uint8_t  READY3:1;         /*!< bit:      3                                     */
    uint8_t  _reserved0:3;     /*!< bit:  4.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_STATUSB_Type;

#define AC_STATUSB_OFFSET           0x09         /**< \brief (AC offset) Status B Register */
#define AC_STATUSB_RESETVALUE       0x0          /**< \brief (AC reset_value) Status B Register */

#define AC_STATUSB_READY0           (0x1u <<  0) /**< \brief (AC_STATUSB)  */
#define AC_STATUSB_READY1           (0x1u <<  1) /**< \brief (AC_STATUSB)  */
#define AC_STATUSB_READY2           (0x1u <<  2) /**< \brief (AC_STATUSB)  */
#define AC_STATUSB_READY3           (0x1u <<  3) /**< \brief (AC_STATUSB)  */
#define AC_STATUSB_SYNCBUSY         (0x1u <<  7) /**< \brief (AC_STATUSB)  */
#define AC_STATUSB_MASK             0x8Fu /**< \brief (AC_STATUSB) MASK Register */

/* -------- AC_STATUSC : (AC Offset: 0x0A) (R/   8) Status C Register -------- */
typedef union {
  struct {
    uint8_t  STATE0:1;         /*!< bit:      0                                     */
    uint8_t  STATE1:1;         /*!< bit:      1                                     */
    uint8_t  STATE2:1;         /*!< bit:      2                                     */
    uint8_t  STATE3:1;         /*!< bit:      3                                     */
    uint8_t  WSTATE0:2;        /*!< bit:  4.. 5                                     */
    uint8_t  WSTATE1:2;        /*!< bit:  6.. 7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_STATUSC_Type;

#define AC_STATUSC_OFFSET           0x0A         /**< \brief (AC offset) Status C Register */
#define AC_STATUSC_RESETVALUE       0x0          /**< \brief (AC reset_value) Status C Register */

#define AC_STATUSC_STATE0           (0x1u <<  0) /**< \brief (AC_STATUSC)  */
#define AC_STATUSC_STATE1           (0x1u <<  1) /**< \brief (AC_STATUSC)  */
#define AC_STATUSC_STATE2           (0x1u <<  2) /**< \brief (AC_STATUSC)  */
#define AC_STATUSC_STATE3           (0x1u <<  3) /**< \brief (AC_STATUSC)  */
#define AC_STATUSC_WSTATE0_Pos      4
#define AC_STATUSC_WSTATE0_Msk      (0x3u << AC_STATUSC_WSTATE0_Pos) /**< \brief (AC_STATUSC)  */
#define AC_STATUSC_WSTATE0(value)   ((AC_STATUSC_WSTATE0_Msk & ((value) << AC_STATUSC_WSTATE0_Pos)))
#define   AC_STATUSC_WSTATE0_ABOVE  (0x0u <<  4) /**< \brief (AC_STATUSC)  */
#define   AC_STATUSC_WSTATE0_INSIDE (0x1u <<  4) /**< \brief (AC_STATUSC)  */
#define   AC_STATUSC_WSTATE0_BELOW  (0x2u <<  4) /**< \brief (AC_STATUSC)  */
#define AC_STATUSC_WSTATE1_Pos      6
#define AC_STATUSC_WSTATE1_Msk      (0x3u << AC_STATUSC_WSTATE1_Pos) /**< \brief (AC_STATUSC)  */
#define AC_STATUSC_WSTATE1(value)   ((AC_STATUSC_WSTATE1_Msk & ((value) << AC_STATUSC_WSTATE1_Pos)))
#define   AC_STATUSC_WSTATE1_ABOVE  (0x0u <<  6) /**< \brief (AC_STATUSC)  */
#define   AC_STATUSC_WSTATE1_INSIDE (0x1u <<  6) /**< \brief (AC_STATUSC)  */
#define   AC_STATUSC_WSTATE1_BELOW  (0x2u <<  6) /**< \brief (AC_STATUSC)  */
#define AC_STATUSC_MASK             0xFFu /**< \brief (AC_STATUSC) MASK Register */

/* -------- AC_WINCTRL : (AC Offset: 0x0C) (R/W  8) Window Control Register -------- */
typedef union {
  struct {
    uint8_t  WEN0:1;           /*!< bit:      0                                     */
    uint8_t  WINTSEL0:2;       /*!< bit:  1.. 2                                     */
    uint8_t  _reserved0:1;     /*!< bit:      3  Reserved                           */
    uint8_t  WEN1:1;           /*!< bit:      4                                     */
    uint8_t  WINTSEL1:2;       /*!< bit:  5.. 6                                     */
    uint8_t  _reserved1:1;     /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_WINCTRL_Type;

#define AC_WINCTRL_OFFSET           0x0C         /**< \brief (AC offset) Window Control Register */
#define AC_WINCTRL_RESETVALUE       0x0          /**< \brief (AC reset_value) Window Control Register */

#define AC_WINCTRL_WEN0             (0x1u <<  0) /**< \brief (AC_WINCTRL)  */
#define AC_WINCTRL_WINTSEL0_Pos     1
#define AC_WINCTRL_WINTSEL0_Msk     (0x3u << AC_WINCTRL_WINTSEL0_Pos) /**< \brief (AC_WINCTRL)  */
#define AC_WINCTRL_WINTSEL0(value)  ((AC_WINCTRL_WINTSEL0_Msk & ((value) << AC_WINCTRL_WINTSEL0_Pos)))
#define   AC_WINCTRL_WINTSEL0_ABOVE (0x0u <<  1) /**< \brief (AC_WINCTRL)  */
#define   AC_WINCTRL_WINTSEL0_INSIDE (0x1u <<  1) /**< \brief (AC_WINCTRL)  */
#define   AC_WINCTRL_WINTSEL0_BELOW (0x2u <<  1) /**< \brief (AC_WINCTRL)  */
#define   AC_WINCTRL_WINTSEL0_OUTSIDE (0x3u <<  1) /**< \brief (AC_WINCTRL)  */
#define AC_WINCTRL_WEN1             (0x1u <<  4) /**< \brief (AC_WINCTRL)  */
#define AC_WINCTRL_WINTSEL1_Pos     5
#define AC_WINCTRL_WINTSEL1_Msk     (0x3u << AC_WINCTRL_WINTSEL1_Pos) /**< \brief (AC_WINCTRL)  */
#define AC_WINCTRL_WINTSEL1(value)  ((AC_WINCTRL_WINTSEL1_Msk & ((value) << AC_WINCTRL_WINTSEL1_Pos)))
#define   AC_WINCTRL_WINTSEL1_ABOVE (0x0u <<  5) /**< \brief (AC_WINCTRL)  */
#define   AC_WINCTRL_WINTSEL1_INSIDE (0x1u <<  5) /**< \brief (AC_WINCTRL)  */
#define   AC_WINCTRL_WINTSEL1_BELOW (0x2u <<  5) /**< \brief (AC_WINCTRL)  */
#define   AC_WINCTRL_WINTSEL1_OUTSIDE (0x3u <<  5) /**< \brief (AC_WINCTRL)  */
#define AC_WINCTRL_MASK             0x77u /**< \brief (AC_WINCTRL) MASK Register */

/* -------- AC_COMPCTRL : (AC Offset: 0x10) (R/W 32) Comp Comparator Control Register -------- */
typedef union {
  struct {
    uint32_t ENABLE:1;         /*!< bit:      0                                     */
    uint32_t SINGLE:1;         /*!< bit:      1                                     */
    uint32_t SPEED:2;          /*!< bit:  2.. 3                                     */
    uint32_t _reserved0:1;     /*!< bit:      4  Reserved                           */
    uint32_t INTSEL:2;         /*!< bit:  5.. 6                                     */
    uint32_t _reserved1:1;     /*!< bit:      7  Reserved                           */
    uint32_t MUXNEG:3;         /*!< bit:  8..10                                     */
    uint32_t _reserved2:1;     /*!< bit:     11  Reserved                           */
    uint32_t MUXPOS:2;         /*!< bit: 12..13                                     */
    uint32_t _reserved3:1;     /*!< bit:     14  Reserved                           */
    uint32_t SWAP:1;           /*!< bit:     15                                     */
    uint32_t OUT:2;            /*!< bit: 16..17                                     */
    uint32_t _reserved4:1;     /*!< bit:     18  Reserved                           */
    uint32_t HYST:1;           /*!< bit:     19                                     */
    uint32_t _reserved5:4;     /*!< bit: 20..23  Reserved                           */
    uint32_t FLEN:3;           /*!< bit: 24..26                                     */
    uint32_t _reserved6:5;     /*!< bit: 27..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} AC_COMPCTRL_Type;

#define AC_COMPCTRL_OFFSET          0x10         /**< \brief (AC offset) Comparator Control Register */
#define AC_COMPCTRL_RESETVALUE      0x0          /**< \brief (AC reset_value) Comparator Control Register */

#define AC_COMPCTRL_ENABLE          (0x1u <<  0) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_SINGLE          (0x1u <<  1) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_SPEED_Pos       2
#define AC_COMPCTRL_SPEED_Msk       (0x3u << AC_COMPCTRL_SPEED_Pos) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_SPEED(value)    ((AC_COMPCTRL_SPEED_Msk & ((value) << AC_COMPCTRL_SPEED_Pos)))
#define   AC_COMPCTRL_SPEED_LOWPOWER (0x0u <<  2) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_SPEED_FAST    (0x1u <<  2) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_INTSEL_Pos      5
#define AC_COMPCTRL_INTSEL_Msk      (0x3u << AC_COMPCTRL_INTSEL_Pos) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_INTSEL(value)   ((AC_COMPCTRL_INTSEL_Msk & ((value) << AC_COMPCTRL_INTSEL_Pos)))
#define   AC_COMPCTRL_INTSEL_TOGGLE (0x0u <<  5) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_INTSEL_RISING (0x1u <<  5) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_INTSEL_FALLING (0x2u <<  5) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_INTSEL_EOC    (0x3u <<  5) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_MUXNEG_Pos      8
#define AC_COMPCTRL_MUXNEG_Msk      (0x7u << AC_COMPCTRL_MUXNEG_Pos) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_MUXNEG(value)   ((AC_COMPCTRL_MUXNEG_Msk & ((value) << AC_COMPCTRL_MUXNEG_Pos)))
#define   AC_COMPCTRL_MUXNEG_PIN0   (0x0u <<  8) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXNEG_PIN1   (0x1u <<  8) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXNEG_PIN2   (0x2u <<  8) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXNEG_PIN3   (0x3u <<  8) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXNEG_GND    (0x4u <<  8) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXNEG_VSCALE (0x5u <<  8) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXNEG_BANDGAP (0x6u <<  8) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXNEG_DAC    (0x7u <<  8) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_MUXPOS_Pos      12
#define AC_COMPCTRL_MUXPOS_Msk      (0x3u << AC_COMPCTRL_MUXPOS_Pos) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_MUXPOS(value)   ((AC_COMPCTRL_MUXPOS_Msk & ((value) << AC_COMPCTRL_MUXPOS_Pos)))
#define   AC_COMPCTRL_MUXPOS_PIN0   (0x0u << 12) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXPOS_PIN1   (0x1u << 12) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXPOS_PIN2   (0x2u << 12) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_MUXPOS_PIN3   (0x3u << 12) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_SWAP            (0x1u << 15) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_OUT_Pos         16
#define AC_COMPCTRL_OUT_Msk         (0x3u << AC_COMPCTRL_OUT_Pos) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_OUT(value)      ((AC_COMPCTRL_OUT_Msk & ((value) << AC_COMPCTRL_OUT_Pos)))
#define   AC_COMPCTRL_OUT_OFF       (0x0u << 16) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_OUT_ASYNC     (0x1u << 16) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_OUT_SYNC      (0x2u << 16) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_HYST            (0x1u << 19) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_FLEN_Pos        24
#define AC_COMPCTRL_FLEN_Msk        (0x7u << AC_COMPCTRL_FLEN_Pos) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_FLEN(value)     ((AC_COMPCTRL_FLEN_Msk & ((value) << AC_COMPCTRL_FLEN_Pos)))
#define   AC_COMPCTRL_FLEN_OFF      (0x0u << 24) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_FLEN_MAJ3     (0x1u << 24) /**< \brief (AC_COMPCTRL)  */
#define   AC_COMPCTRL_FLEN_MAJ5     (0x2u << 24) /**< \brief (AC_COMPCTRL)  */
#define AC_COMPCTRL_MASK            0x70BB76Fu /**< \brief (AC_COMPCTRL) MASK Register */

/* -------- AC_SCALER : (AC Offset: 0x20) (R/W  8) Scaler Scaler Register -------- */
typedef union {
  uint8_t reg;                 /*!< Type      used for register access              */
} AC_SCALER_Type;

#define AC_SCALER_OFFSET            0x20         /**< \brief (AC offset) Scaler Register */
#define AC_SCALER_RESETVALUE        0x0          /**< \brief (AC reset_value) Scaler Register */
#define AC_SCALER_MASK              0xFFu        /**< \brief (AC_SCALER) MASK Register */

#define AC_NUM_CMP                  2

/** \brief AC hardware registers */
typedef struct {
  __IO AC_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W  8) Control A Register */
  __O  AC_CTRLB_Type             CTRLB;       /**< \brief Offset: 0x01 ( /W  8) Control B Register */
  __IO AC_EVCTRL_Type            EVCTRL;      /**< \brief Offset: 0x02 (R/W 16) Event Control Register */
  __IO AC_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x04 (R/W  8) Interrupt Enable Clear Register */
  __IO AC_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x05 (R/W  8) Interupt Enable Set Register */
  __IO AC_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x06 (R/W  8) Interrupt Flag Register */
       RoReg8                    Reserved1[0x1];
  __I  AC_STATUSA_Type           STATUSA;     /**< \brief Offset: 0x08 (R/   8) Status A Register */
  __I  AC_STATUSB_Type           STATUSB;     /**< \brief Offset: 0x09 (R/   8) Status B Register */
  __I  AC_STATUSC_Type           STATUSC;     /**< \brief Offset: 0x0A (R/   8) Status C Register */
       RoReg8                    Reserved2[0x1];
  __IO AC_WINCTRL_Type           WINCTRL;     /**< \brief Offset: 0x0C (R/W  8) Window Control Register */
       RoReg8                    Reserved3[0x3];
  __IO AC_COMPCTRL_Type          COMPCTRL[AC_NUM_CMP]; /**< \brief Offset: 0x10 (R/W 32) Comparator Control Register */
       AC_COMPCTRL_Type          Reserved4[4 - AC_NUM_CMP];
  __IO AC_SCALER_Type            SCALER[AC_NUM_CMP]; /**< \brief Offset: 0x20 (R/W  8) Scaler Register */
       AC_SCALER_Type            Reserved5[4 - AC_NUM_CMP];
} Ac;

#endif /*  __EXCLUDE_AC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR ADC */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_ADC Analog Digital Converter */
/*@{*/

#define REV_ADC        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_ADC_FROM_CMSIS__

/* -------- ADC_CTRLA : (ADC Offset: 0x00) (R/W  8) Control register A -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software reset                     */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable module                      */
    uint8_t  SLEEPEN:1;        /*!< bit:      2  Sleep enable                       */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_CTRLA_Type;

#define ADC_CTRLA_OFFSET            0x00         /**< \brief (ADC offset) Control register A */
#define ADC_CTRLA_RESETVALUE        0x0          /**< \brief (ADC reset_value) Control register A */

#define ADC_CTRLA_SWRST             (0x1u <<  0) /**< \brief (ADC_CTRLA) Software reset */
#define ADC_CTRLA_ENABLE            (0x1u <<  1) /**< \brief (ADC_CTRLA) Enable module */
#define ADC_CTRLA_SLEEPEN           (0x1u <<  2) /**< \brief (ADC_CTRLA) Sleep enable */
#define ADC_CTRLA_MASK              0x7u /**< \brief (ADC_CTRLA) MASK Register */

/* -------- ADC_REFCTRL : (ADC Offset: 0x01) (R/W  8) Reference control register -------- */
typedef union {
  struct {
    uint8_t  REFSEL:4;         /*!< bit:  0.. 3  Reference Selection                */
    uint8_t  _reserved0:3;     /*!< bit:  4.. 6  Reserved                           */
    uint8_t  REFCOMP:1;        /*!< bit:      7  Reference Buffer Offset Compensation Enable */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_REFCTRL_Type;

#define ADC_REFCTRL_OFFSET          0x01         /**< \brief (ADC offset) Reference control register */
#define ADC_REFCTRL_RESETVALUE      0x0          /**< \brief (ADC reset_value) Reference control register */

#define ADC_REFCTRL_REFSEL_Pos      0
#define ADC_REFCTRL_REFSEL_Msk      (0xFu << ADC_REFCTRL_REFSEL_Pos) /**< \brief (ADC_REFCTRL) Reference Selection */
#define ADC_REFCTRL_REFSEL(value)   ((ADC_REFCTRL_REFSEL_Msk & ((value) << ADC_REFCTRL_REFSEL_Pos)))
#define ADC_REFCTRL_REFCOMP         (0x1u <<  7) /**< \brief (ADC_REFCTRL) Reference Buffer Offset Compensation Enable */
#define ADC_REFCTRL_MASK            0x8Fu /**< \brief (ADC_REFCTRL) MASK Register */

/* -------- ADC_AVGCTRL : (ADC Offset: 0x02) (R/W  8) Average Control register -------- */
typedef union {
  struct {
    uint8_t  SAMPLENUM:4;      /*!< bit:  0.. 3  Number of samples to be collected  */
    uint8_t  ADJRES:3;         /*!< bit:  4.. 6  Adjusting Result / Division Coefficient */
    uint8_t  _reserved0:1;     /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_AVGCTRL_Type;

#define ADC_AVGCTRL_OFFSET          0x02         /**< \brief (ADC offset) Average Control register */
#define ADC_AVGCTRL_RESETVALUE      0x0          /**< \brief (ADC reset_value) Average Control register */

#define ADC_AVGCTRL_SAMPLENUM_Pos    0
#define ADC_AVGCTRL_SAMPLENUM_Msk    (0xFu << ADC_AVGCTRL_SAMPLENUM_Pos) /**< \brief (ADC_AVGCTRL) Number of samples to be collected */
#define ADC_AVGCTRL_SAMPLENUM(value) ((ADC_AVGCTRL_SAMPLENUM_Msk & ((value) << ADC_AVGCTRL_SAMPLENUM_Pos)))
#define ADC_AVGCTRL_ADJRES_Pos      4
#define ADC_AVGCTRL_ADJRES_Msk      (0x7u << ADC_AVGCTRL_ADJRES_Pos) /**< \brief (ADC_AVGCTRL) Adjusting Result / Division Coefficient */
#define ADC_AVGCTRL_ADJRES(value)   ((ADC_AVGCTRL_ADJRES_Msk & ((value) << ADC_AVGCTRL_ADJRES_Pos)))
#define ADC_AVGCTRL_MASK            0x7Fu /**< \brief (ADC_AVGCTRL) MASK Register */

/* -------- ADC_SAMPCTRL : (ADC Offset: 0x03) (R/W  8) Sample Time Control register -------- */
typedef union {
  struct {
    uint8_t  SAMPLEN:6;        /*!< bit:  0.. 5  Sampling Time Length               */
    uint8_t  _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_SAMPCTRL_Type;

#define ADC_SAMPCTRL_OFFSET         0x03         /**< \brief (ADC offset) Sample Time Control register */
#define ADC_SAMPCTRL_RESETVALUE     0x0          /**< \brief (ADC reset_value) Sample Time Control register */

#define ADC_SAMPCTRL_SAMPLEN_Pos    0
#define ADC_SAMPCTRL_SAMPLEN_Msk    (0x3Fu << ADC_SAMPCTRL_SAMPLEN_Pos) /**< \brief (ADC_SAMPCTRL) Sampling Time Length */
#define ADC_SAMPCTRL_SAMPLEN(value) ((ADC_SAMPCTRL_SAMPLEN_Msk & ((value) << ADC_SAMPCTRL_SAMPLEN_Pos)))
#define ADC_SAMPCTRL_MASK           0x3Fu /**< \brief (ADC_SAMPCTRL) MASK Register */

/* -------- ADC_CTRLB : (ADC Offset: 0x04) (R/W 16) Control Register B -------- */
typedef union {
  struct {
    uint16_t DIFFMODE:1;       /*!< bit:      0  Differential mode                  */
    uint16_t LEFTADJ:1;        /*!< bit:      1  Lefts adjusted result              */
    uint16_t FREERUN:1;        /*!< bit:      2  Free Running Mode                  */
    uint16_t CORREN:1;         /*!< bit:      3  Digital Correction logic enable    */
    uint16_t RESSEL:2;         /*!< bit:  4.. 5  Conversion Result Resolution       */
    uint16_t _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
    uint16_t PRESCALER:3;      /*!< bit:  8..10  Prescaler configuration            */
    uint16_t _reserved1:5;     /*!< bit: 11..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} ADC_CTRLB_Type;

#define ADC_CTRLB_OFFSET            0x04         /**< \brief (ADC offset) Control Register B */
#define ADC_CTRLB_RESETVALUE        0x0          /**< \brief (ADC reset_value) Control Register B */

#define ADC_CTRLB_DIFFMODE          (0x1u <<  0) /**< \brief (ADC_CTRLB) Differential mode */
#define ADC_CTRLB_LEFTADJ           (0x1u <<  1) /**< \brief (ADC_CTRLB) Lefts adjusted result */
#define ADC_CTRLB_FREERUN           (0x1u <<  2) /**< \brief (ADC_CTRLB) Free Running Mode */
#define ADC_CTRLB_CORREN            (0x1u <<  3) /**< \brief (ADC_CTRLB) Digital Correction logic enable */
#define ADC_CTRLB_RESSEL_Pos        4
#define ADC_CTRLB_RESSEL_Msk        (0x3u << ADC_CTRLB_RESSEL_Pos) /**< \brief (ADC_CTRLB) Conversion Result Resolution */
#define ADC_CTRLB_RESSEL(value)     ((ADC_CTRLB_RESSEL_Msk & ((value) << ADC_CTRLB_RESSEL_Pos)))
#define ADC_CTRLB_PRESCALER_Pos     8
#define ADC_CTRLB_PRESCALER_Msk     (0x7u << ADC_CTRLB_PRESCALER_Pos) /**< \brief (ADC_CTRLB) Prescaler configuration */
#define ADC_CTRLB_PRESCALER(value)  ((ADC_CTRLB_PRESCALER_Msk & ((value) << ADC_CTRLB_PRESCALER_Pos)))
#define ADC_CTRLB_MASK              0x73Fu /**< \brief (ADC_CTRLB) MASK Register */

/* -------- ADC_WINCTRL : (ADC Offset: 0x08) (R/W  8) Window Monitor control register -------- */
typedef union {
  struct {
    uint8_t  WINMODE:3;        /*!< bit:  0.. 2  Window Monitor Mode                */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_WINCTRL_Type;

#define ADC_WINCTRL_OFFSET          0x08         /**< \brief (ADC offset) Window Monitor control register */
#define ADC_WINCTRL_RESETVALUE      0x0          /**< \brief (ADC reset_value) Window Monitor control register */

#define ADC_WINCTRL_WINMODE_Pos     0
#define ADC_WINCTRL_WINMODE_Msk     (0x7u << ADC_WINCTRL_WINMODE_Pos) /**< \brief (ADC_WINCTRL) Window Monitor Mode */
#define ADC_WINCTRL_WINMODE(value)  ((ADC_WINCTRL_WINMODE_Msk & ((value) << ADC_WINCTRL_WINMODE_Pos)))
#define ADC_WINCTRL_MASK            0x7u /**< \brief (ADC_WINCTRL) MASK Register */

/* -------- ADC_SWTRIG : (ADC Offset: 0x0C) (R/W  8) Control register B -------- */
typedef union {
  struct {
    uint8_t  FLUSH:1;          /*!< bit:      0  ADC Flush                          */
    uint8_t  START:1;          /*!< bit:      1  Start ADC Conversion               */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_SWTRIG_Type;

#define ADC_SWTRIG_OFFSET           0x0C         /**< \brief (ADC offset) Control register B */
#define ADC_SWTRIG_RESETVALUE       0x0          /**< \brief (ADC reset_value) Control register B */

#define ADC_SWTRIG_FLUSH            (0x1u <<  0) /**< \brief (ADC_SWTRIG) ADC Flush */
#define ADC_SWTRIG_START            (0x1u <<  1) /**< \brief (ADC_SWTRIG) Start ADC Conversion */
#define ADC_SWTRIG_MASK             0x3u /**< \brief (ADC_SWTRIG) MASK Register */

/* -------- ADC_INPUTCTRL : (ADC Offset: 0x10) (R/W 32) Input controlregister -------- */
typedef union {
  struct {
    uint32_t MUXPOS:5;         /*!< bit:  0.. 4  Positive Mux Input selection       */
    uint32_t _reserved0:3;     /*!< bit:  5.. 7  Reserved                           */
    uint32_t MUXNEG:5;         /*!< bit:  8..12  Negative Mux Input selection       */
    uint32_t _reserved1:3;     /*!< bit: 13..15  Reserved                           */
    uint32_t INPUTSCAN:4;      /*!< bit: 16..19  Number of input channels included in scan */
    uint32_t INPUTOFFSET:4;    /*!< bit: 20..23  Positive Mux Setting Offset        */
    uint32_t GAIN:4;           /*!< bit: 24..27  Gain control register              */
    uint32_t _reserved2:4;     /*!< bit: 28..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} ADC_INPUTCTRL_Type;

#define ADC_INPUTCTRL_OFFSET        0x10         /**< \brief (ADC offset) Input controlregister */
#define ADC_INPUTCTRL_RESETVALUE    0x0          /**< \brief (ADC reset_value) Input controlregister */

#define ADC_INPUTCTRL_MUXPOS_Pos    0
#define ADC_INPUTCTRL_MUXPOS_Msk    (0x1Fu << ADC_INPUTCTRL_MUXPOS_Pos) /**< \brief (ADC_INPUTCTRL) Positive Mux Input selection */
#define ADC_INPUTCTRL_MUXPOS(value) ((ADC_INPUTCTRL_MUXPOS_Msk & ((value) << ADC_INPUTCTRL_MUXPOS_Pos)))
#define ADC_INPUTCTRL_MUXNEG_Pos    8
#define ADC_INPUTCTRL_MUXNEG_Msk    (0x1Fu << ADC_INPUTCTRL_MUXNEG_Pos) /**< \brief (ADC_INPUTCTRL) Negative Mux Input selection */
#define ADC_INPUTCTRL_MUXNEG(value) ((ADC_INPUTCTRL_MUXNEG_Msk & ((value) << ADC_INPUTCTRL_MUXNEG_Pos)))
#define ADC_INPUTCTRL_INPUTSCAN_Pos    16
#define ADC_INPUTCTRL_INPUTSCAN_Msk    (0xFu << ADC_INPUTCTRL_INPUTSCAN_Pos) /**< \brief (ADC_INPUTCTRL) Number of input channels included in scan */
#define ADC_INPUTCTRL_INPUTSCAN(value) ((ADC_INPUTCTRL_INPUTSCAN_Msk & ((value) << ADC_INPUTCTRL_INPUTSCAN_Pos)))
#define ADC_INPUTCTRL_INPUTOFFSET_Pos    20
#define ADC_INPUTCTRL_INPUTOFFSET_Msk    (0xFu << ADC_INPUTCTRL_INPUTOFFSET_Pos) /**< \brief (ADC_INPUTCTRL) Positive Mux Setting Offset */
#define ADC_INPUTCTRL_INPUTOFFSET(value) ((ADC_INPUTCTRL_INPUTOFFSET_Msk & ((value) << ADC_INPUTCTRL_INPUTOFFSET_Pos)))
#define ADC_INPUTCTRL_GAIN_Pos      24
#define ADC_INPUTCTRL_GAIN_Msk      (0xFu << ADC_INPUTCTRL_GAIN_Pos) /**< \brief (ADC_INPUTCTRL) Gain control register */
#define ADC_INPUTCTRL_GAIN(value)   ((ADC_INPUTCTRL_GAIN_Msk & ((value) << ADC_INPUTCTRL_GAIN_Pos)))
#define ADC_INPUTCTRL_MASK          0xFFF1F1Fu /**< \brief (ADC_INPUTCTRL) MASK Register */

/* -------- ADC_EVCTRL : (ADC Offset: 0x14) (R/W  8) Event control register -------- */
typedef union {
  struct {
    uint8_t  STARTEI:1;        /*!< bit:      0  Start Conversion Event In          */
    uint8_t  SYNCEI:1;         /*!< bit:      1  Sync Event In                      */
    uint8_t  _reserved0:2;     /*!< bit:  2.. 3  Reserved                           */
    uint8_t  RESRDYEO:1;       /*!< bit:      4  Result Ready Event Out             */
    uint8_t  WINMONEO:1;       /*!< bit:      5  Window Monitor Event Out           */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_EVCTRL_Type;

#define ADC_EVCTRL_OFFSET           0x14         /**< \brief (ADC offset) Event control register */
#define ADC_EVCTRL_RESETVALUE       0x0          /**< \brief (ADC reset_value) Event control register */

#define ADC_EVCTRL_STARTEI          (0x1u <<  0) /**< \brief (ADC_EVCTRL) Start Conversion Event In */
#define ADC_EVCTRL_SYNCEI           (0x1u <<  1) /**< \brief (ADC_EVCTRL) Sync Event In */
#define ADC_EVCTRL_RESRDYEO         (0x1u <<  4) /**< \brief (ADC_EVCTRL) Result Ready Event Out */
#define ADC_EVCTRL_WINMONEO         (0x1u <<  5) /**< \brief (ADC_EVCTRL) Window Monitor Event Out */
#define ADC_EVCTRL_MASK             0x33u /**< \brief (ADC_EVCTRL) MASK Register */

/* -------- ADC_INTENCLR : (ADC Offset: 0x16) (R/W  8) Interrupt Enable Clear register -------- */
typedef union {
  struct {
    uint8_t  RESRDY:1;         /*!< bit:      0  Result Ready Interrupt Disable     */
    uint8_t  OVERRUN:1;        /*!< bit:      1  Overrun Interrupt Disable          */
    uint8_t  WINMON:1;         /*!< bit:      2  Window monitor Disable             */
    uint8_t  READY:1;          /*!< bit:      3  Ready Interrupt Disable            */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_INTENCLR_Type;

#define ADC_INTENCLR_OFFSET         0x16         /**< \brief (ADC offset) Interrupt Enable Clear register */
#define ADC_INTENCLR_RESETVALUE     0x0          /**< \brief (ADC reset_value) Interrupt Enable Clear register */

#define ADC_INTENCLR_RESRDY         (0x1u <<  0) /**< \brief (ADC_INTENCLR) Result Ready Interrupt Disable */
#define ADC_INTENCLR_OVERRUN        (0x1u <<  1) /**< \brief (ADC_INTENCLR) Overrun Interrupt Disable */
#define ADC_INTENCLR_WINMON         (0x1u <<  2) /**< \brief (ADC_INTENCLR) Window monitor Disable */
#define ADC_INTENCLR_READY          (0x1u <<  3) /**< \brief (ADC_INTENCLR) Ready Interrupt Disable */
#define ADC_INTENCLR_MASK           0xFu /**< \brief (ADC_INTENCLR) MASK Register */

/* -------- ADC_INTENSET : (ADC Offset: 0x17) (R/W  8) Interrupt Enable Set register -------- */
typedef union {
  struct {
    uint8_t  RESRDY:1;         /*!< bit:      0  Result Ready Interrupt Enable      */
    uint8_t  OVERRUN:1;        /*!< bit:      1  Overrun Interrupt Enable           */
    uint8_t  WINMON:1;         /*!< bit:      2  Window Monitor Enable              */
    uint8_t  READY:1;          /*!< bit:      3  Ready Interrupt Enable             */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_INTENSET_Type;

#define ADC_INTENSET_OFFSET         0x17         /**< \brief (ADC offset) Interrupt Enable Set register */
#define ADC_INTENSET_RESETVALUE     0x0          /**< \brief (ADC reset_value) Interrupt Enable Set register */

#define ADC_INTENSET_RESRDY         (0x1u <<  0) /**< \brief (ADC_INTENSET) Result Ready Interrupt Enable */
#define ADC_INTENSET_OVERRUN        (0x1u <<  1) /**< \brief (ADC_INTENSET) Overrun Interrupt Enable */
#define ADC_INTENSET_WINMON         (0x1u <<  2) /**< \brief (ADC_INTENSET) Window Monitor Enable */
#define ADC_INTENSET_READY          (0x1u <<  3) /**< \brief (ADC_INTENSET) Ready Interrupt Enable */
#define ADC_INTENSET_MASK           0xFu /**< \brief (ADC_INTENSET) MASK Register */

/* -------- ADC_INTFLAG : (ADC Offset: 0x18) (R/W  8) Interrupt Flag Status and Claer register -------- */
typedef union {
  struct {
    uint8_t  RESRDY:1;         /*!< bit:      0  Result Ready Interrupt             */
    uint8_t  OVERRUN:1;        /*!< bit:      1  Overrun Interrupt                  */
    uint8_t  WINMON:1;         /*!< bit:      2  Window Monitor Interrupt           */
    uint8_t  READY:1;          /*!< bit:      3  Ready Interrupt flag               */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_INTFLAG_Type;

#define ADC_INTFLAG_OFFSET          0x18         /**< \brief (ADC offset) Interrupt Flag Status and Claer register */
#define ADC_INTFLAG_RESETVALUE      0x0          /**< \brief (ADC reset_value) Interrupt Flag Status and Claer register */

#define ADC_INTFLAG_RESRDY          (0x1u <<  0) /**< \brief (ADC_INTFLAG) Result Ready Interrupt */
#define ADC_INTFLAG_OVERRUN         (0x1u <<  1) /**< \brief (ADC_INTFLAG) Overrun Interrupt */
#define ADC_INTFLAG_WINMON          (0x1u <<  2) /**< \brief (ADC_INTFLAG) Window Monitor Interrupt */
#define ADC_INTFLAG_READY           (0x1u <<  3) /**< \brief (ADC_INTFLAG) Ready Interrupt flag */
#define ADC_INTFLAG_MASK            0xFu /**< \brief (ADC_INTFLAG) MASK Register */

/* -------- ADC_STATUS : (ADC Offset: 0x19) (R/   8) Status register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:7;     /*!< bit:  0.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Synchronisation Busy Status        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_STATUS_Type;

#define ADC_STATUS_OFFSET           0x19         /**< \brief (ADC offset) Status register */
#define ADC_STATUS_RESETVALUE       0x0          /**< \brief (ADC reset_value) Status register */

#define ADC_STATUS_SYNCBUSY         (0x1u <<  7) /**< \brief (ADC_STATUS) Synchronisation Busy Status */
#define ADC_STATUS_MASK             0x80u /**< \brief (ADC_STATUS) MASK Register */

/* -------- ADC_RESULT : (ADC Offset: 0x1A) (R/  16) Result register -------- */
typedef union {
  struct {
    uint16_t RESULT:16;        /*!< bit:  0..15  Result value                       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} ADC_RESULT_Type;

#define ADC_RESULT_OFFSET           0x1A         /**< \brief (ADC offset) Result register */
#define ADC_RESULT_RESETVALUE       0x0          /**< \brief (ADC reset_value) Result register */

#define ADC_RESULT_RESULT_Pos       0
#define ADC_RESULT_RESULT_Msk       (0xFFFFu << ADC_RESULT_RESULT_Pos) /**< \brief (ADC_RESULT) Result value */
#define ADC_RESULT_RESULT(value)    ((ADC_RESULT_RESULT_Msk & ((value) << ADC_RESULT_RESULT_Pos)))
#define ADC_RESULT_MASK             0xFFFFu /**< \brief (ADC_RESULT) MASK Register */

/* -------- ADC_WINLT : (ADC Offset: 0x1C) (R/W 16) Window Monitor Lower Threshold register -------- */
typedef union {
  struct {
    uint16_t WINLT:16;         /*!< bit:  0..15  Window Lower Threshold             */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} ADC_WINLT_Type;

#define ADC_WINLT_OFFSET            0x1C         /**< \brief (ADC offset) Window Monitor Lower Threshold register */
#define ADC_WINLT_RESETVALUE        0x0          /**< \brief (ADC reset_value) Window Monitor Lower Threshold register */

#define ADC_WINLT_WINLT_Pos         0
#define ADC_WINLT_WINLT_Msk         (0xFFFFu << ADC_WINLT_WINLT_Pos) /**< \brief (ADC_WINLT) Window Lower Threshold */
#define ADC_WINLT_WINLT(value)      ((ADC_WINLT_WINLT_Msk & ((value) << ADC_WINLT_WINLT_Pos)))
#define ADC_WINLT_MASK              0xFFFFu /**< \brief (ADC_WINLT) MASK Register */

/* -------- ADC_WINUT : (ADC Offset: 0x20) (R/W 16) Window Monitor Upper threshold register -------- */
typedef union {
  struct {
    uint16_t WINUT:16;         /*!< bit:  0..15  Window Upper Threshold             */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} ADC_WINUT_Type;

#define ADC_WINUT_OFFSET            0x20         /**< \brief (ADC offset) Window Monitor Upper threshold register */
#define ADC_WINUT_RESETVALUE        0x0          /**< \brief (ADC reset_value) Window Monitor Upper threshold register */

#define ADC_WINUT_WINUT_Pos         0
#define ADC_WINUT_WINUT_Msk         (0xFFFFu << ADC_WINUT_WINUT_Pos) /**< \brief (ADC_WINUT) Window Upper Threshold */
#define ADC_WINUT_WINUT(value)      ((ADC_WINUT_WINUT_Msk & ((value) << ADC_WINUT_WINUT_Pos)))
#define ADC_WINUT_MASK              0xFFFFu /**< \brief (ADC_WINUT) MASK Register */

/* -------- ADC_GAINCORR : (ADC Offset: 0x24) (R/W 16) Gain Correction register -------- */
typedef union {
  struct {
    uint16_t GAINCORR:12;      /*!< bit:  0..11  Gain Correction value              */
    uint16_t _reserved0:4;     /*!< bit: 12..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} ADC_GAINCORR_Type;

#define ADC_GAINCORR_OFFSET         0x24         /**< \brief (ADC offset) Gain Correction register */
#define ADC_GAINCORR_RESETVALUE     0x0          /**< \brief (ADC reset_value) Gain Correction register */

#define ADC_GAINCORR_GAINCORR_Pos    0
#define ADC_GAINCORR_GAINCORR_Msk    (0xFFFu << ADC_GAINCORR_GAINCORR_Pos) /**< \brief (ADC_GAINCORR) Gain Correction value */
#define ADC_GAINCORR_GAINCORR(value) ((ADC_GAINCORR_GAINCORR_Msk & ((value) << ADC_GAINCORR_GAINCORR_Pos)))
#define ADC_GAINCORR_MASK           0xFFFu /**< \brief (ADC_GAINCORR) MASK Register */

/* -------- ADC_OFFSETCORR : (ADC Offset: 0x26) (R/W 16) Offset Correction register -------- */
typedef union {
  struct {
    uint16_t OFFSETCORR:12;    /*!< bit:  0..11  Offset Correction value            */
    uint16_t _reserved0:4;     /*!< bit: 12..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} ADC_OFFSETCORR_Type;

#define ADC_OFFSETCORR_OFFSET       0x26         /**< \brief (ADC offset) Offset Correction register */
#define ADC_OFFSETCORR_RESETVALUE   0x0          /**< \brief (ADC reset_value) Offset Correction register */

#define ADC_OFFSETCORR_OFFSETCORR_Pos    0
#define ADC_OFFSETCORR_OFFSETCORR_Msk    (0xFFFu << ADC_OFFSETCORR_OFFSETCORR_Pos) /**< \brief (ADC_OFFSETCORR) Offset Correction value */
#define ADC_OFFSETCORR_OFFSETCORR(value) ((ADC_OFFSETCORR_OFFSETCORR_Msk & ((value) << ADC_OFFSETCORR_OFFSETCORR_Pos)))
#define ADC_OFFSETCORR_MASK         0xFFFu /**< \brief (ADC_OFFSETCORR) MASK Register */

/* -------- ADC_CALIB : (ADC Offset: 0x28) (R/W 16) Calibration register -------- */
typedef union {
  struct {
    uint16_t LINEARITY_CAL:8;  /*!< bit:  0.. 7  Linearity calibration              */
    uint16_t BIAS_CAL:3;       /*!< bit:  8..10  Bias  Configuration                */
    uint16_t _reserved0:5;     /*!< bit: 11..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} ADC_CALIB_Type;

#define ADC_CALIB_OFFSET            0x28         /**< \brief (ADC offset) Calibration register */
#define ADC_CALIB_RESETVALUE        0x0          /**< \brief (ADC reset_value) Calibration register */

#define ADC_CALIB_LINEARITY_CAL_Pos    0
#define ADC_CALIB_LINEARITY_CAL_Msk    (0xFFu << ADC_CALIB_LINEARITY_CAL_Pos) /**< \brief (ADC_CALIB) Linearity calibration */
#define ADC_CALIB_LINEARITY_CAL(value) ((ADC_CALIB_LINEARITY_CAL_Msk & ((value) << ADC_CALIB_LINEARITY_CAL_Pos)))
#define ADC_CALIB_BIAS_CAL_Pos      8
#define ADC_CALIB_BIAS_CAL_Msk      (0x7u << ADC_CALIB_BIAS_CAL_Pos) /**< \brief (ADC_CALIB) Bias  Configuration */
#define ADC_CALIB_BIAS_CAL(value)   ((ADC_CALIB_BIAS_CAL_Msk & ((value) << ADC_CALIB_BIAS_CAL_Pos)))
#define ADC_CALIB_MASK              0x7FFu /**< \brief (ADC_CALIB) MASK Register */

/* -------- ADC_DEBUG : (ADC Offset: 0x2A) (R/W  8) Debug register -------- */
typedef union {
  struct {
    uint8_t  DBGRUN:1;         /*!< bit:      0  Debug Run                          */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_DEBUG_Type;

#define ADC_DEBUG_OFFSET            0x2A         /**< \brief (ADC offset) Debug register */
#define ADC_DEBUG_RESETVALUE        0x0          /**< \brief (ADC reset_value) Debug register */

#define ADC_DEBUG_DBGRUN            (0x1u <<  0) /**< \brief (ADC_DEBUG) Debug Run */
#define ADC_DEBUG_MASK              0x1u /**< \brief (ADC_DEBUG) MASK Register */

/* -------- ADC_TEST : (ADC Offset: 0x2B) (R/W  8) Test modes register -------- */
typedef union {
  struct {
    uint8_t  TEST_EN:1;        /*!< bit:      0  Enable Test mode                   */
    uint8_t  REFPAD_EN:1;      /*!< bit:      1  Connect Vrefp/n to aio33testp/n    */
    uint8_t  REFINT_DIS:1;     /*!< bit:      2  Disable internal Reference         */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_TEST_Type;

#define ADC_TEST_OFFSET             0x2B         /**< \brief (ADC offset) Test modes register */
#define ADC_TEST_RESETVALUE         0x0          /**< \brief (ADC reset_value) Test modes register */

#define ADC_TEST_TEST_EN            (0x1u <<  0) /**< \brief (ADC_TEST) Enable Test mode */
#define ADC_TEST_REFPAD_EN          (0x1u <<  1) /**< \brief (ADC_TEST) Connect Vrefp/n to aio33testp/n */
#define ADC_TEST_REFINT_DIS         (0x1u <<  2) /**< \brief (ADC_TEST) Disable internal Reference */
#define ADC_TEST_MASK               0x7u /**< \brief (ADC_TEST) MASK Register */

/* -------- ADC_TESTRESULT : (ADC Offset: 0x2C) (R/W 32) Test Result register -------- */
typedef union {
  struct {
    uint32_t TESTRESULT:24;    /*!< bit:  0..23  Result directly from ADC hard block */
    uint32_t _reserved0:8;     /*!< bit: 24..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} ADC_TESTRESULT_Type;

#define ADC_TESTRESULT_OFFSET       0x2C         /**< \brief (ADC offset) Test Result register */
#define ADC_TESTRESULT_RESETVALUE   0x0          /**< \brief (ADC reset_value) Test Result register */

#define ADC_TESTRESULT_TESTRESULT_Pos    0
#define ADC_TESTRESULT_TESTRESULT_Msk    (0xFFFFFFu << ADC_TESTRESULT_TESTRESULT_Pos) /**< \brief (ADC_TESTRESULT) Result directly from ADC hard block */
#define ADC_TESTRESULT_TESTRESULT(value) ((ADC_TESTRESULT_TESTRESULT_Msk & ((value) << ADC_TESTRESULT_TESTRESULT_Pos)))
#define ADC_TESTRESULT_MASK         0xFFFFFFu /**< \brief (ADC_TESTRESULT) MASK Register */

/* -------- ADC_DCFG : (ADC Offset: 0x30) (R/W  8) Device configuration -------- */
typedef union {
  struct {
    uint8_t  CMPDELAY:1;       /*!< bit:      0  Comparator Delay Control           */
    uint8_t  BOOSTEN:1;        /*!< bit:      1  Enable the SR bosster in the opamp */
    uint8_t  VCMPULSE:1;       /*!< bit:      2  Enable VCM pulse                   */
    uint8_t  BIAS_OPA:1;       /*!< bit:      3  Select PTAT biasing for OPA        */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} ADC_DCFG_Type;

#define ADC_DCFG_OFFSET             0x30         /**< \brief (ADC offset) Device configuration */
#define ADC_DCFG_RESETVALUE         0x0          /**< \brief (ADC reset_value) Device configuration */

#define ADC_DCFG_CMPDELAY           (0x1u <<  0) /**< \brief (ADC_DCFG) Comparator Delay Control */
#define ADC_DCFG_BOOSTEN            (0x1u <<  1) /**< \brief (ADC_DCFG) Enable the SR bosster in the opamp */
#define ADC_DCFG_VCMPULSE           (0x1u <<  2) /**< \brief (ADC_DCFG) Enable VCM pulse */
#define ADC_DCFG_BIAS_OPA           (0x1u <<  3) /**< \brief (ADC_DCFG) Select PTAT biasing for OPA */
#define ADC_DCFG_MASK               0xFu /**< \brief (ADC_DCFG) MASK Register */

/** \brief ADC hardware registers */
typedef struct {
  __IO ADC_CTRLA_Type            CTRLA;       /**< \brief Offset: 0x00 (R/W  8) Control register A */
  __IO ADC_REFCTRL_Type          REFCTRL;     /**< \brief Offset: 0x01 (R/W  8) Reference control register */
  __IO ADC_AVGCTRL_Type          AVGCTRL;     /**< \brief Offset: 0x02 (R/W  8) Average Control register */
  __IO ADC_SAMPCTRL_Type         SAMPCTRL;    /**< \brief Offset: 0x03 (R/W  8) Sample Time Control register */
  __IO ADC_CTRLB_Type            CTRLB;       /**< \brief Offset: 0x04 (R/W 16) Control Register B */
       RoReg8                    Reserved1[0x2];
  __IO ADC_WINCTRL_Type          WINCTRL;     /**< \brief Offset: 0x08 (R/W  8) Window Monitor control register */
       RoReg8                    Reserved2[0x3];
  __IO ADC_SWTRIG_Type           SWTRIG;      /**< \brief Offset: 0x0C (R/W  8) Control register B */
       RoReg8                    Reserved3[0x3];
  __IO ADC_INPUTCTRL_Type        INPUTCTRL;   /**< \brief Offset: 0x10 (R/W 32) Input controlregister */
  __IO ADC_EVCTRL_Type           EVCTRL;      /**< \brief Offset: 0x14 (R/W  8) Event control register */
       RoReg8                    Reserved4[0x1];
  __IO ADC_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x16 (R/W  8) Interrupt Enable Clear register */
  __IO ADC_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x17 (R/W  8) Interrupt Enable Set register */
  __IO ADC_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x18 (R/W  8) Interrupt Flag Status and Claer register */
  __I  ADC_STATUS_Type           STATUS;      /**< \brief Offset: 0x19 (R/   8) Status register */
  __I  ADC_RESULT_Type           RESULT;      /**< \brief Offset: 0x1A (R/  16) Result register */
  __IO ADC_WINLT_Type            WINLT;       /**< \brief Offset: 0x1C (R/W 16) Window Monitor Lower Threshold register */
       RoReg8                    Reserved5[0x2];
  __IO ADC_WINUT_Type            WINUT;       /**< \brief Offset: 0x20 (R/W 16) Window Monitor Upper threshold register */
       RoReg8                    Reserved6[0x2];
  __IO ADC_GAINCORR_Type         GAINCORR;    /**< \brief Offset: 0x24 (R/W 16) Gain Correction register */
  __IO ADC_OFFSETCORR_Type       OFFSETCORR;  /**< \brief Offset: 0x26 (R/W 16) Offset Correction register */
  __IO ADC_CALIB_Type            CALIB;       /**< \brief Offset: 0x28 (R/W 16) Calibration register */
  __IO ADC_DEBUG_Type            DEBUG;       /**< \brief Offset: 0x2A (R/W  8) Debug register */
  __IO ADC_TEST_Type             TEST;        /**< \brief Offset: 0x2B (R/W  8) Test modes register */
  __IO ADC_TESTRESULT_Type       TESTRESULT;  /**< \brief Offset: 0x2C (R/W 32) Test Result register */
  __IO ADC_DCFG_Type             DCFG;        /**< \brief Offset: 0x30 (R/W  8) Device configuration */
} Adc;

#endif /*  __EXCLUDE_ADC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR ATW */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_ATW Analog Test Wire Controller */
/*@{*/

#define REV_ATW        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_ATW_FROM_CMSIS__
// ### Null start_register AtwGroup, group_end=PWIREEN
// ### Null start_register AtwSpacer, group_end=PWIREEN

#define ATW_SPACERS                 2
#define ATW_GROUPS                  2

/** \brief AtwGroup hardware registers */
typedef struct {
} AtwGroup;

/** \brief ATW hardware registers */
typedef struct {
} Atw;

#endif /*  __EXCLUDE_ATW_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR DAC */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_DAC Digital Analog Converter */
/*@{*/

#define REV_DAC        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_DAC_FROM_CMSIS__

/* -------- DAC_CTRLA : (DAC Offset: 0x0) (R/W  8) Control Register A -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable                             */
    uint8_t  SLEEPEN:1;        /*!< bit:      2  Sleep Enable                       */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DAC_CTRLA_Type;

#define DAC_CTRLA_OFFSET            0x0          /**< \brief (DAC offset) Control Register A */
#define DAC_CTRLA_RESETVALUE        0x0          /**< \brief (DAC reset_value) Control Register A */

#define DAC_CTRLA_SWRST             (0x1u <<  0) /**< \brief (DAC_CTRLA) Software Reset */
#define DAC_CTRLA_ENABLE            (0x1u <<  1) /**< \brief (DAC_CTRLA) Enable */
#define DAC_CTRLA_SLEEPEN           (0x1u <<  2) /**< \brief (DAC_CTRLA) Sleep Enable */
#define DAC_CTRLA_MASK              0x7u /**< \brief (DAC_CTRLA) MASK Register */

/* -------- DAC_CTRLB : (DAC Offset: 0x1) (R/W  8) Control Register B -------- */
typedef union {
  struct {
    uint8_t  EOEN:1;           /*!< bit:      0  EOEN                               */
    uint8_t  IOEN:1;           /*!< bit:      1  IOEN                               */
    uint8_t  LEFTADJ:1;        /*!< bit:      2  LEFTADJ                            */
    uint8_t  VPD:1;            /*!< bit:      3  VPD                                */
    uint8_t  _reserved0:2;     /*!< bit:  4.. 5  Reserved                           */
    uint8_t  REFSEL:2;         /*!< bit:  6.. 7  REFSEL                             */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DAC_CTRLB_Type;

#define DAC_CTRLB_OFFSET            0x1          /**< \brief (DAC offset) Control Register B */
#define DAC_CTRLB_RESETVALUE        0x0          /**< \brief (DAC reset_value) Control Register B */

#define DAC_CTRLB_EOEN              (0x1u <<  0) /**< \brief (DAC_CTRLB) EOEN */
#define DAC_CTRLB_IOEN              (0x1u <<  1) /**< \brief (DAC_CTRLB) IOEN */
#define DAC_CTRLB_LEFTADJ           (0x1u <<  2) /**< \brief (DAC_CTRLB) LEFTADJ */
#define DAC_CTRLB_VPD               (0x1u <<  3) /**< \brief (DAC_CTRLB) VPD */
#define DAC_CTRLB_REFSEL_Pos        6
#define DAC_CTRLB_REFSEL_Msk        (0x3u << DAC_CTRLB_REFSEL_Pos) /**< \brief (DAC_CTRLB) REFSEL */
#define DAC_CTRLB_REFSEL(value)     ((DAC_CTRLB_REFSEL_Msk & ((value) << DAC_CTRLB_REFSEL_Pos)))
#define DAC_CTRLB_MASK              0xCFu /**< \brief (DAC_CTRLB) MASK Register */

/* -------- DAC_EVCTRL : (DAC Offset: 0x2) (R/W  8) Event Control Register -------- */
typedef union {
  struct {
    uint8_t  STARTEI:1;        /*!< bit:      0  Start conversion event input       */
    uint8_t  EMPTYEO:1;        /*!< bit:      1  Data buffer empty event output     */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DAC_EVCTRL_Type;

#define DAC_EVCTRL_OFFSET           0x2          /**< \brief (DAC offset) Event Control Register */
#define DAC_EVCTRL_RESETVALUE       0x0          /**< \brief (DAC reset_value) Event Control Register */

#define DAC_EVCTRL_STARTEI          (0x1u <<  0) /**< \brief (DAC_EVCTRL) Start conversion event input */
#define DAC_EVCTRL_EMPTYEO          (0x1u <<  1) /**< \brief (DAC_EVCTRL) Data buffer empty event output */
#define DAC_EVCTRL_MASK             0x3u /**< \brief (DAC_EVCTRL) MASK Register */

/* -------- DAC_TEST : (DAC Offset: 0x3) (R/W  8) Test Register -------- */
typedef union {
  struct {
    uint8_t  TESTEN:1;         /*!< bit:      0  TESTEN                             */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DAC_TEST_Type;

#define DAC_TEST_OFFSET             0x3          /**< \brief (DAC offset) Test Register */
#define DAC_TEST_RESETVALUE         0x0          /**< \brief (DAC reset_value) Test Register */

#define DAC_TEST_TESTEN             (0x1u <<  0) /**< \brief (DAC_TEST) TESTEN */
#define DAC_TEST_MASK               0x1u /**< \brief (DAC_TEST) MASK Register */

/* -------- DAC_INTENCLR : (DAC Offset: 0x4) (R/W  8) Interrupt Enable Clear Regiser -------- */
typedef union {
  struct {
    uint8_t  UNDERRUN:1;       /*!< bit:      0  Underrun interrupt disable         */
    uint8_t  EMPTY:1;          /*!< bit:      1  Empty Interrupt Disable            */
    uint8_t  READY:1;          /*!< bit:      2  Ready Interrupt Disable            */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DAC_INTENCLR_Type;

#define DAC_INTENCLR_OFFSET         0x4          /**< \brief (DAC offset) Interrupt Enable Clear Regiser */
#define DAC_INTENCLR_RESETVALUE     0x0          /**< \brief (DAC reset_value) Interrupt Enable Clear Regiser */

#define DAC_INTENCLR_UNDERRUN       (0x1u <<  0) /**< \brief (DAC_INTENCLR) Underrun interrupt disable */
#define DAC_INTENCLR_EMPTY          (0x1u <<  1) /**< \brief (DAC_INTENCLR) Empty Interrupt Disable */
#define DAC_INTENCLR_READY          (0x1u <<  2) /**< \brief (DAC_INTENCLR) Ready Interrupt Disable */
#define DAC_INTENCLR_MASK           0x7u /**< \brief (DAC_INTENCLR) MASK Register */

/* -------- DAC_INTENSET : (DAC Offset: 0x5) (R/W  8) Interrupt Enable Set Register -------- */
typedef union {
  struct {
    uint8_t  UNDERRUN:1;       /*!< bit:      0  Underrun Interrupt Enable          */
    uint8_t  EMPTY:1;          /*!< bit:      1  Empty Interrupt Enable             */
    uint8_t  READY:1;          /*!< bit:      2  Ready Interrupt Enable             */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DAC_INTENSET_Type;

#define DAC_INTENSET_OFFSET         0x5          /**< \brief (DAC offset) Interrupt Enable Set Register */
#define DAC_INTENSET_RESETVALUE     0x0          /**< \brief (DAC reset_value) Interrupt Enable Set Register */

#define DAC_INTENSET_UNDERRUN       (0x1u <<  0) /**< \brief (DAC_INTENSET) Underrun Interrupt Enable */
#define DAC_INTENSET_EMPTY          (0x1u <<  1) /**< \brief (DAC_INTENSET) Empty Interrupt Enable */
#define DAC_INTENSET_READY          (0x1u <<  2) /**< \brief (DAC_INTENSET) Ready Interrupt Enable */
#define DAC_INTENSET_MASK           0x7u /**< \brief (DAC_INTENSET) MASK Register */

/* -------- DAC_INTFLAG : (DAC Offset: 0x6) (R/W  8) Interrupt Flag Status and Clear Register -------- */
typedef union {
  struct {
    uint8_t  UNDERRUN:1;       /*!< bit:      0  Underrun flag status and clear     */
    uint8_t  EMPTY:1;          /*!< bit:      1  Empty flag status and clear        */
    uint8_t  READY:1;          /*!< bit:      2  Ready flag status and clear        */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DAC_INTFLAG_Type;

#define DAC_INTFLAG_OFFSET          0x6          /**< \brief (DAC offset) Interrupt Flag Status and Clear Register */
#define DAC_INTFLAG_RESETVALUE      0x0          /**< \brief (DAC reset_value) Interrupt Flag Status and Clear Register */

#define DAC_INTFLAG_UNDERRUN        (0x1u <<  0) /**< \brief (DAC_INTFLAG) Underrun flag status and clear */
#define DAC_INTFLAG_EMPTY           (0x1u <<  1) /**< \brief (DAC_INTFLAG) Empty flag status and clear */
#define DAC_INTFLAG_READY           (0x1u <<  2) /**< \brief (DAC_INTFLAG) Ready flag status and clear */
#define DAC_INTFLAG_MASK            0x7u /**< \brief (DAC_INTFLAG) MASK Register */

/* -------- DAC_STATUS : (DAC Offset: 0x7) (R/   8) Status Register -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset Status              */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable Status                      */
    uint8_t  _reserved0:5;     /*!< bit:  2.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Synchronization Busy               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DAC_STATUS_Type;

#define DAC_STATUS_OFFSET           0x7          /**< \brief (DAC offset) Status Register */
#define DAC_STATUS_RESETVALUE       0x0          /**< \brief (DAC reset_value) Status Register */

#define DAC_STATUS_SWRST            (0x1u <<  0) /**< \brief (DAC_STATUS) Software Reset Status */
#define DAC_STATUS_ENABLE           (0x1u <<  1) /**< \brief (DAC_STATUS) Enable Status */
#define DAC_STATUS_SYNCBUSY         (0x1u <<  7) /**< \brief (DAC_STATUS) Synchronization Busy */
#define DAC_STATUS_MASK             0x83u /**< \brief (DAC_STATUS) MASK Register */

/* -------- DAC_DATA : (DAC Offset: 0x8) (R/W 16) Data Register -------- */
typedef union {
  struct {
    uint16_t DATA:16;          /*!< bit:  0..15  DATA register                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} DAC_DATA_Type;

#define DAC_DATA_OFFSET             0x8          /**< \brief (DAC offset) Data Register */
#define DAC_DATA_RESETVALUE         0x0          /**< \brief (DAC reset_value) Data Register */

#define DAC_DATA_DATA_Pos           0
#define DAC_DATA_DATA_Msk           (0xFFFFu << DAC_DATA_DATA_Pos) /**< \brief (DAC_DATA) DATA register */
#define DAC_DATA_DATA(value)        ((DAC_DATA_DATA_Msk & ((value) << DAC_DATA_DATA_Pos)))
#define DAC_DATA_MASK               0xFFFFu /**< \brief (DAC_DATA) MASK Register */

/* -------- DAC_DATABUF : (DAC Offset: 0xC) (R/W 16) Data Buffer Register -------- */
typedef union {
  struct {
    uint16_t DATABUF:16;       /*!< bit:  0..15  Data Buffer                        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} DAC_DATABUF_Type;

#define DAC_DATABUF_OFFSET          0xC          /**< \brief (DAC offset) Data Buffer Register */
#define DAC_DATABUF_RESETVALUE      0x0          /**< \brief (DAC reset_value) Data Buffer Register */

#define DAC_DATABUF_DATABUF_Pos     0
#define DAC_DATABUF_DATABUF_Msk     (0xFFFFu << DAC_DATABUF_DATABUF_Pos) /**< \brief (DAC_DATABUF) Data Buffer */
#define DAC_DATABUF_DATABUF(value)  ((DAC_DATABUF_DATABUF_Msk & ((value) << DAC_DATABUF_DATABUF_Pos)))
#define DAC_DATABUF_MASK            0xFFFFu /**< \brief (DAC_DATABUF) MASK Register */

/** \brief DAC hardware registers */
typedef struct {
  __IO DAC_CTRLA_Type            CTRLA;       /**< \brief Offset: 0x0 (R/W  8) Control Register A */
  __IO DAC_CTRLB_Type            CTRLB;       /**< \brief Offset: 0x1 (R/W  8) Control Register B */
  __IO DAC_EVCTRL_Type           EVCTRL;      /**< \brief Offset: 0x2 (R/W  8) Event Control Register */
  __IO DAC_TEST_Type             TEST;        /**< \brief Offset: 0x3 (R/W  8) Test Register */
  __IO DAC_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x4 (R/W  8) Interrupt Enable Clear Regiser */
  __IO DAC_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x5 (R/W  8) Interrupt Enable Set Register */
  __IO DAC_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x6 (R/W  8) Interrupt Flag Status and Clear Register */
  __I  DAC_STATUS_Type           STATUS;      /**< \brief Offset: 0x7 (R/   8) Status Register */
  __IO DAC_DATA_Type             DATA;        /**< \brief Offset: 0x8 (R/W 16) Data Register */
       RoReg8                    Reserved1[0x2];
  __IO DAC_DATABUF_Type          DATABUF;     /**< \brief Offset: 0xC (R/W 16) Data Buffer Register */
} Dac;

#endif /*  __EXCLUDE_DAC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR DSU */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_DSU Device Service Unit */
/*@{*/

#define REV_DSU        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_DSU_FROM_CMSIS__

/* -------- DSU_CTRL : (DSU Offset: 0x0000) ( /W  8) Control Register -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0                                     */
    uint8_t  _reserved0:1;     /*!< bit:      1  Reserved                           */
    uint8_t  CRC:1;            /*!< bit:      2                                     */
    uint8_t  MBIST:1;          /*!< bit:      3                                     */
    uint8_t  CE:1;             /*!< bit:      4                                     */
    uint8_t  _reserved1:1;     /*!< bit:      5  Reserved                           */
    uint8_t  ARR:1;            /*!< bit:      6                                     */
    uint8_t  SMSA:1;           /*!< bit:      7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DSU_CTRL_Type;

#define DSU_CTRL_OFFSET             0x0000       /**< \brief (DSU offset) Control Register */

#define DSU_CTRL_SWRST              (0x1u <<  0) /**< \brief (DSU_CTRL)  */
#define DSU_CTRL_CRC                (0x1u <<  2) /**< \brief (DSU_CTRL)  */
#define DSU_CTRL_MBIST              (0x1u <<  3) /**< \brief (DSU_CTRL)  */
#define DSU_CTRL_CE                 (0x1u <<  4) /**< \brief (DSU_CTRL)  */
#define DSU_CTRL_ARR                (0x1u <<  6) /**< \brief (DSU_CTRL)  */
#define DSU_CTRL_SMSA               (0x1u <<  7) /**< \brief (DSU_CTRL)  */
#define DSU_CTRL_MASK               0xDDu /**< \brief (DSU_CTRL) MASK Register */

/* -------- DSU_STATUSA : (DSU Offset: 0x0001) (R/W  8) Status Register A -------- */
typedef union {
  struct {
    uint8_t  DONE:1;           /*!< bit:      0                                     */
    uint8_t  CRSTEXT:1;        /*!< bit:      1                                     */
    uint8_t  BERR:1;           /*!< bit:      2                                     */
    uint8_t  FAIL:1;           /*!< bit:      3                                     */
    uint8_t  PERR:1;           /*!< bit:      4                                     */
    uint8_t  _reserved0:3;     /*!< bit:  5.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DSU_STATUSA_Type;

#define DSU_STATUSA_OFFSET          0x0001       /**< \brief (DSU offset) Status Register A */

#define DSU_STATUSA_DONE            (0x1u <<  0) /**< \brief (DSU_STATUSA)  */
#define DSU_STATUSA_CRSTEXT         (0x1u <<  1) /**< \brief (DSU_STATUSA)  */
#define DSU_STATUSA_BERR            (0x1u <<  2) /**< \brief (DSU_STATUSA)  */
#define DSU_STATUSA_FAIL            (0x1u <<  3) /**< \brief (DSU_STATUSA)  */
#define DSU_STATUSA_PERR            (0x1u <<  4) /**< \brief (DSU_STATUSA)  */
#define DSU_STATUSA_MASK            0x1Fu /**< \brief (DSU_STATUSA) MASK Register */

/* -------- DSU_STATUSB : (DSU Offset: 0x0002) (R/   8) Status Register B -------- */
typedef union {
  struct {
    uint8_t  PROT:1;           /*!< bit:      0                                     */
    uint8_t  DBGPRES:1;        /*!< bit:      1                                     */
    uint8_t  DCC0D:1;          /*!< bit:      2                                     */
    uint8_t  DCC1D:1;          /*!< bit:      3                                     */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DSU_STATUSB_Type;

#define DSU_STATUSB_OFFSET          0x0002       /**< \brief (DSU offset) Status Register B */

#define DSU_STATUSB_PROT            (0x1u <<  0) /**< \brief (DSU_STATUSB)  */
#define DSU_STATUSB_DBGPRES         (0x1u <<  1) /**< \brief (DSU_STATUSB)  */
#define DSU_STATUSB_DCC0D           (0x1u <<  2) /**< \brief (DSU_STATUSB)  */
#define DSU_STATUSB_DCC1D           (0x1u <<  3) /**< \brief (DSU_STATUSB)  */
#define DSU_STATUSB_MASK            0xFu /**< \brief (DSU_STATUSB) MASK Register */

/* -------- DSU_STATUSC : (DSU Offset: 0x0003) (R/   8) Status Register C -------- */
typedef union {
  struct {
    uint8_t  STATE:3;          /*!< bit:  0.. 2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DSU_STATUSC_Type;

#define DSU_STATUSC_OFFSET          0x0003       /**< \brief (DSU offset) Status Register C */
#define DSU_STATUSC_RESETVALUE      0x0          /**< \brief (DSU reset_value) Status Register C */

#define DSU_STATUSC_STATE_Pos       0
#define DSU_STATUSC_STATE_Msk       (0x7u << DSU_STATUSC_STATE_Pos) /**< \brief (DSU_STATUSC)  */
#define DSU_STATUSC_STATE(value)    ((DSU_STATUSC_STATE_Msk & ((value) << DSU_STATUSC_STATE_Pos)))
#define DSU_STATUSC_MASK            0x7u /**< \brief (DSU_STATUSC) MASK Register */

/* -------- DSU_ADDR : (DSU Offset: 0x0004) (R/W 32) Address Register -------- */
typedef union {
  struct {
    uint32_t AMOD:2;           /*!< bit:  0.. 1                                     */
    uint32_t ADDR:30;          /*!< bit:  2..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_ADDR_Type;

#define DSU_ADDR_OFFSET             0x0004       /**< \brief (DSU offset) Address Register */
#define DSU_ADDR_RESETVALUE         0x0          /**< \brief (DSU reset_value) Address Register */

#define DSU_ADDR_AMOD_Pos           0
#define DSU_ADDR_AMOD_Msk           (0x3u << DSU_ADDR_AMOD_Pos) /**< \brief (DSU_ADDR)  */
#define DSU_ADDR_AMOD(value)        ((DSU_ADDR_AMOD_Msk & ((value) << DSU_ADDR_AMOD_Pos)))
#define DSU_ADDR_ADDR_Pos           2
#define DSU_ADDR_ADDR_Msk           (0x3FFFFFFFu << DSU_ADDR_ADDR_Pos) /**< \brief (DSU_ADDR)  */
#define DSU_ADDR_ADDR(value)        ((DSU_ADDR_ADDR_Msk & ((value) << DSU_ADDR_ADDR_Pos)))
#define DSU_ADDR_MASK               0xFFFFFFFFu /**< \brief (DSU_ADDR) MASK Register */

/* -------- DSU_LENGTH : (DSU Offset: 0x0008) (R/W 32) Length Register -------- */
typedef union {
  struct {
    uint32_t _reserved0:2;     /*!< bit:  0.. 1  Reserved                           */
    uint32_t LENGTH:30;        /*!< bit:  2..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_LENGTH_Type;

#define DSU_LENGTH_OFFSET           0x0008       /**< \brief (DSU offset) Length Register */
#define DSU_LENGTH_RESETVALUE       0x0          /**< \brief (DSU reset_value) Length Register */

#define DSU_LENGTH_LENGTH_Pos       2
#define DSU_LENGTH_LENGTH_Msk       (0x3FFFFFFFu << DSU_LENGTH_LENGTH_Pos) /**< \brief (DSU_LENGTH)  */
#define DSU_LENGTH_LENGTH(value)    ((DSU_LENGTH_LENGTH_Msk & ((value) << DSU_LENGTH_LENGTH_Pos)))
#define DSU_LENGTH_MASK             0xFFFFFFFCu /**< \brief (DSU_LENGTH) MASK Register */

/* -------- DSU_DATA : (DSU Offset: 0x000C) (R/W 32) Data Register -------- */
typedef union {
  struct {
    uint32_t DATA:32;          /*!< bit:  0..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DATA_Type;

#define DSU_DATA_OFFSET             0x000C       /**< \brief (DSU offset) Data Register */

#define DSU_DATA_DATA_Pos           0
#define DSU_DATA_DATA_Msk           (0xFFFFFFFFu << DSU_DATA_DATA_Pos) /**< \brief (DSU_DATA)  */
#define DSU_DATA_DATA(value)        ((DSU_DATA_DATA_Msk & ((value) << DSU_DATA_DATA_Pos)))
#define DSU_DATA_MASK               0xFFFFFFFFu /**< \brief (DSU_DATA) MASK Register */

/* -------- DSU_DCC0 : (DSU Offset: 0x0010) (R/W 32) Debug Communication Channel 0 Register -------- */
typedef union {
  struct {
    uint32_t DCC0:32;          /*!< bit:  0..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DCC0_Type;

#define DSU_DCC0_OFFSET             0x0010       /**< \brief (DSU offset) Debug Communication Channel 0 Register */
#define DSU_DCC0_RESETVALUE         0x0          /**< \brief (DSU reset_value) Debug Communication Channel 0 Register */

#define DSU_DCC0_DCC0_Pos           0
#define DSU_DCC0_DCC0_Msk           (0xFFFFFFFFu << DSU_DCC0_DCC0_Pos) /**< \brief (DSU_DCC0)  */
#define DSU_DCC0_DCC0(value)        ((DSU_DCC0_DCC0_Msk & ((value) << DSU_DCC0_DCC0_Pos)))
#define DSU_DCC0_MASK               0xFFFFFFFFu /**< \brief (DSU_DCC0) MASK Register */

/* -------- DSU_DCC1 : (DSU Offset: 0x0014) (R/W 32) Debug Communication Channel 1 Register -------- */
typedef union {
  struct {
    uint32_t DCC1:32;          /*!< bit:  0..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DCC1_Type;

#define DSU_DCC1_OFFSET             0x0014       /**< \brief (DSU offset) Debug Communication Channel 1 Register */
#define DSU_DCC1_RESETVALUE         0x0          /**< \brief (DSU reset_value) Debug Communication Channel 1 Register */

#define DSU_DCC1_DCC1_Pos           0
#define DSU_DCC1_DCC1_Msk           (0xFFFFFFFFu << DSU_DCC1_DCC1_Pos) /**< \brief (DSU_DCC1)  */
#define DSU_DCC1_DCC1(value)        ((DSU_DCC1_DCC1_Msk & ((value) << DSU_DCC1_DCC1_Pos)))
#define DSU_DCC1_MASK               0xFFFFFFFFu /**< \brief (DSU_DCC1) MASK Register */

/* -------- DSU_DID : (DSU Offset: 0x0018) (R/  32) Device Identification Register -------- */
typedef union {
  struct {
    uint32_t DEVSEL:8;         /*!< bit:  0.. 7                                     */
    uint32_t REVISION:4;       /*!< bit:  8..11                                     */
    uint32_t DIE:4;            /*!< bit: 12..15                                     */
    uint32_t SUBFAMILY:8;      /*!< bit: 16..23                                     */
    uint32_t FAMILY:4;         /*!< bit: 24..27                                     */
    uint32_t PROCESSOR:4;      /*!< bit: 28..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DID_Type;

#define DSU_DID_OFFSET              0x0018       /**< \brief (DSU offset) Device Identification Register */
#define DSU_DID_RESETVALUE          0x0          /**< \brief (DSU reset_value) Device Identification Register */

#define DSU_DID_DEVSEL_Pos          0
#define DSU_DID_DEVSEL_Msk          (0xFFu << DSU_DID_DEVSEL_Pos) /**< \brief (DSU_DID)  */
#define DSU_DID_DEVSEL(value)       ((DSU_DID_DEVSEL_Msk & ((value) << DSU_DID_DEVSEL_Pos)))
#define DSU_DID_REVISION_Pos        8
#define DSU_DID_REVISION_Msk        (0xFu << DSU_DID_REVISION_Pos) /**< \brief (DSU_DID)  */
#define DSU_DID_REVISION(value)     ((DSU_DID_REVISION_Msk & ((value) << DSU_DID_REVISION_Pos)))
#define DSU_DID_DIE_Pos             12
#define DSU_DID_DIE_Msk             (0xFu << DSU_DID_DIE_Pos) /**< \brief (DSU_DID)  */
#define DSU_DID_DIE(value)          ((DSU_DID_DIE_Msk & ((value) << DSU_DID_DIE_Pos)))
#define DSU_DID_SUBFAMILY_Pos       16
#define DSU_DID_SUBFAMILY_Msk       (0xFFu << DSU_DID_SUBFAMILY_Pos) /**< \brief (DSU_DID)  */
#define DSU_DID_SUBFAMILY(value)    ((DSU_DID_SUBFAMILY_Msk & ((value) << DSU_DID_SUBFAMILY_Pos)))
#define DSU_DID_FAMILY_Pos          24
#define DSU_DID_FAMILY_Msk          (0xFu << DSU_DID_FAMILY_Pos) /**< \brief (DSU_DID)  */
#define DSU_DID_FAMILY(value)       ((DSU_DID_FAMILY_Msk & ((value) << DSU_DID_FAMILY_Pos)))
#define DSU_DID_PROCESSOR_Pos       28
#define DSU_DID_PROCESSOR_Msk       (0xFu << DSU_DID_PROCESSOR_Pos) /**< \brief (DSU_DID)  */
#define DSU_DID_PROCESSOR(value)    ((DSU_DID_PROCESSOR_Msk & ((value) << DSU_DID_PROCESSOR_Pos)))
#define DSU_DID_MASK                0xFFFFFFFFu /**< \brief (DSU_DID) MASK Register */

/* -------- DSU_DCFG0 : (DSU Offset: 0x00F0) (R/W 32) Device Configuration Register 0 -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DCFG0_Type;

#define DSU_DCFG0_OFFSET            0x00F0       /**< \brief (DSU offset) Device Configuration Register 0 */
#define DSU_DCFG0_RESETVALUE        0x0          /**< \brief (DSU reset_value) Device Configuration Register 0 */
#define DSU_DCFG0_MASK              0xFFFFFFFFu  /**< \brief (DSU_DCFG0) MASK Register */

/* -------- DSU_DCFG1 : (DSU Offset: 0x00F4) (R/  32) Device Configuration Register 1 -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DCFG1_Type;

#define DSU_DCFG1_OFFSET            0x00F4       /**< \brief (DSU offset) Device Configuration Register 1 */
#define DSU_DCFG1_RESETVALUE        0x0          /**< \brief (DSU reset_value) Device Configuration Register 1 */
#define DSU_DCFG1_MASK              0xFFFFFFFFu  /**< \brief (DSU_DCFG1) MASK Register */

/* -------- DSU_UPTM : (DSU Offset: 0x00F8) (R/W 32) UnProtected Test Mode Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_UPTM_Type;

#define DSU_UPTM_OFFSET             0x00F8       /**< \brief (DSU offset) UnProtected Test Mode Register */
#define DSU_UPTM_RESETVALUE         0x0          /**< \brief (DSU reset_value) UnProtected Test Mode Register */
#define DSU_UPTM_MASK               0xFFFFFFFFu  /**< \brief (DSU_UPTM) MASK Register */

/* -------- DSU_TESTMODE : (DSU Offset: 0x00FC) (R/W 32) Test Mode Register -------- */
typedef union {
  struct {
    uint32_t TESTMODE:32;      /*!< bit:  0..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_TESTMODE_Type;

#define DSU_TESTMODE_OFFSET         0x00FC       /**< \brief (DSU offset) Test Mode Register */
#define DSU_TESTMODE_RESETVALUE     0x0          /**< \brief (DSU reset_value) Test Mode Register */

#define DSU_TESTMODE_TESTMODE_Pos    0
#define DSU_TESTMODE_TESTMODE_Msk    (0xFFFFFFFFu << DSU_TESTMODE_TESTMODE_Pos) /**< \brief (DSU_TESTMODE)  */
#define DSU_TESTMODE_TESTMODE(value) ((DSU_TESTMODE_TESTMODE_Msk & ((value) << DSU_TESTMODE_TESTMODE_Pos)))
#define DSU_TESTMODE_MASK           0xFFFFFFFFu /**< \brief (DSU_TESTMODE) MASK Register */

/* -------- DSU_ENTRY0 : (DSU Offset: 0x1000) (R/  32) Coresight ROM Table Entry 0 Register -------- */
typedef union {
  struct {
    uint32_t PRES:1;           /*!< bit:      0                                     */
    uint32_t FMT:1;            /*!< bit:      1                                     */
    uint32_t _reserved0:10;    /*!< bit:  2..11  Reserved                           */
    uint32_t ADDOFF:20;        /*!< bit: 12..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_ENTRY0_Type;

#define DSU_ENTRY0_OFFSET           0x1000       /**< \brief (DSU offset) Coresight ROM Table Entry 0 Register */

#define DSU_ENTRY0_PRES             (0x1u <<  0) /**< \brief (DSU_ENTRY0)  */
#define DSU_ENTRY0_FMT              (0x1u <<  1) /**< \brief (DSU_ENTRY0)  */
#define DSU_ENTRY0_ADDOFF_Pos       12
#define DSU_ENTRY0_ADDOFF_Msk       (0xFFFFFu << DSU_ENTRY0_ADDOFF_Pos) /**< \brief (DSU_ENTRY0)  */
#define DSU_ENTRY0_ADDOFF(value)    ((DSU_ENTRY0_ADDOFF_Msk & ((value) << DSU_ENTRY0_ADDOFF_Pos)))
#define DSU_ENTRY0_MASK             0xFFFFF003u /**< \brief (DSU_ENTRY0) MASK Register */

/* -------- DSU_ENTRY1 : (DSU Offset: 0x1004) (R/  32) Coresight ROM Table Entry 1 Register -------- */
typedef union {
  struct {
    uint32_t PRES:1;           /*!< bit:      0                                     */
    uint32_t FMT:1;            /*!< bit:      1                                     */
    uint32_t _reserved0:10;    /*!< bit:  2..11  Reserved                           */
    uint32_t ADDOFF:20;        /*!< bit: 12..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_ENTRY1_Type;

#define DSU_ENTRY1_OFFSET           0x1004       /**< \brief (DSU offset) Coresight ROM Table Entry 1 Register */

#define DSU_ENTRY1_PRES             (0x1u <<  0) /**< \brief (DSU_ENTRY1)  */
#define DSU_ENTRY1_FMT              (0x1u <<  1) /**< \brief (DSU_ENTRY1)  */
#define DSU_ENTRY1_ADDOFF_Pos       12
#define DSU_ENTRY1_ADDOFF_Msk       (0xFFFFFu << DSU_ENTRY1_ADDOFF_Pos) /**< \brief (DSU_ENTRY1)  */
#define DSU_ENTRY1_ADDOFF(value)    ((DSU_ENTRY1_ADDOFF_Msk & ((value) << DSU_ENTRY1_ADDOFF_Pos)))
#define DSU_ENTRY1_MASK             0xFFFFF003u /**< \brief (DSU_ENTRY1) MASK Register */

/* -------- DSU_END : (DSU Offset: 0x1008) (R/  32) Coresight ROM Table End Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_END_Type;

#define DSU_END_OFFSET              0x1008       /**< \brief (DSU offset) Coresight ROM Table End Register */
#define DSU_END_RESETVALUE          0x0          /**< \brief (DSU reset_value) Coresight ROM Table End Register */
#define DSU_END_MASK                0x0u /**< \brief (DSU_END) MASK Register */

/* -------- DSU_MEMTYPE : (DSU Offset: 0x1FCC) (R/  32) Coresight ROM Table Memory Type Register -------- */
typedef union {
  struct {
    uint32_t SMEMP:1;          /*!< bit:      0                                     */
    uint32_t _reserved0:31;    /*!< bit:  1..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_MEMTYPE_Type;

#define DSU_MEMTYPE_OFFSET          0x1FCC       /**< \brief (DSU offset) Coresight ROM Table Memory Type Register */

#define DSU_MEMTYPE_SMEMP           (0x1u <<  0) /**< \brief (DSU_MEMTYPE)  */
#define DSU_MEMTYPE_MASK            0x1u /**< \brief (DSU_MEMTYPE) MASK Register */

/* -------- DSU_PID4 : (DSU Offset: 0x1FD0) (R/  32) Peripheral Identification Register 4 -------- */
typedef union {
  struct {
    uint32_t JEPCC:4;          /*!< bit:  0.. 3                                     */
    uint32_t FKBC:4;           /*!< bit:  4.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID4_Type;

#define DSU_PID4_OFFSET             0x1FD0       /**< \brief (DSU offset) Peripheral Identification Register 4 */

#define DSU_PID4_JEPCC_Pos          0
#define DSU_PID4_JEPCC_Msk          (0xFu << DSU_PID4_JEPCC_Pos) /**< \brief (DSU_PID4)  */
#define DSU_PID4_JEPCC(value)       ((DSU_PID4_JEPCC_Msk & ((value) << DSU_PID4_JEPCC_Pos)))
#define DSU_PID4_FKBC_Pos           4
#define DSU_PID4_FKBC_Msk           (0xFu << DSU_PID4_FKBC_Pos) /**< \brief (DSU_PID4)  */
#define DSU_PID4_FKBC(value)        ((DSU_PID4_FKBC_Msk & ((value) << DSU_PID4_FKBC_Pos)))
#define DSU_PID4_MASK               0xFFu /**< \brief (DSU_PID4) MASK Register */

/* -------- DSU_PID5 : (DSU Offset: 0x1FD4) (R/  32) Peripheral Identification Register 5 -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID5_Type;

#define DSU_PID5_OFFSET             0x1FD4       /**< \brief (DSU offset) Peripheral Identification Register 5 */
#define DSU_PID5_MASK               0x0u /**< \brief (DSU_PID5) MASK Register */

/* -------- DSU_PID6 : (DSU Offset: 0x1FD8) (R/  32) Peripheral Identification Register 6 -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID6_Type;

#define DSU_PID6_OFFSET             0x1FD8       /**< \brief (DSU offset) Peripheral Identification Register 6 */
#define DSU_PID6_MASK               0x0u /**< \brief (DSU_PID6) MASK Register */

/* -------- DSU_PID7 : (DSU Offset: 0x1FDC) (R/  32) Peripheral Identification Register 7 -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID7_Type;

#define DSU_PID7_OFFSET             0x1FDC       /**< \brief (DSU offset) Peripheral Identification Register 7 */
#define DSU_PID7_MASK               0x0u /**< \brief (DSU_PID7) MASK Register */

/* -------- DSU_PID0 : (DSU Offset: 0x1FE0) (R/  32) Peripheral Identification Register 0 -------- */
typedef union {
  struct {
    uint32_t PARTNBL:8;        /*!< bit:  0.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID0_Type;

#define DSU_PID0_OFFSET             0x1FE0       /**< \brief (DSU offset) Peripheral Identification Register 0 */

#define DSU_PID0_PARTNBL_Pos        0
#define DSU_PID0_PARTNBL_Msk        (0xFFu << DSU_PID0_PARTNBL_Pos) /**< \brief (DSU_PID0)  */
#define DSU_PID0_PARTNBL(value)     ((DSU_PID0_PARTNBL_Msk & ((value) << DSU_PID0_PARTNBL_Pos)))
#define DSU_PID0_MASK               0xFFu /**< \brief (DSU_PID0) MASK Register */

/* -------- DSU_PID1 : (DSU Offset: 0x1FE4) (R/  32) Peripheral Identification Register 1 -------- */
typedef union {
  struct {
    uint32_t PARTNBH:4;        /*!< bit:  0.. 3                                     */
    uint32_t JEPIDCL:4;        /*!< bit:  4.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID1_Type;

#define DSU_PID1_OFFSET             0x1FE4       /**< \brief (DSU offset) Peripheral Identification Register 1 */

#define DSU_PID1_PARTNBH_Pos        0
#define DSU_PID1_PARTNBH_Msk        (0xFu << DSU_PID1_PARTNBH_Pos) /**< \brief (DSU_PID1)  */
#define DSU_PID1_PARTNBH(value)     ((DSU_PID1_PARTNBH_Msk & ((value) << DSU_PID1_PARTNBH_Pos)))
#define DSU_PID1_JEPIDCL_Pos        4
#define DSU_PID1_JEPIDCL_Msk        (0xFu << DSU_PID1_JEPIDCL_Pos) /**< \brief (DSU_PID1)  */
#define DSU_PID1_JEPIDCL(value)     ((DSU_PID1_JEPIDCL_Msk & ((value) << DSU_PID1_JEPIDCL_Pos)))
#define DSU_PID1_MASK               0xFFu /**< \brief (DSU_PID1) MASK Register */

/* -------- DSU_PID2 : (DSU Offset: 0x1FE8) (R/  32) Peripheral Identification Register 2 -------- */
typedef union {
  struct {
    uint32_t JEPIDCH:3;        /*!< bit:  0.. 2                                     */
    uint32_t JEPU:1;           /*!< bit:      3                                     */
    uint32_t REVISION:4;       /*!< bit:  4.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID2_Type;

#define DSU_PID2_OFFSET             0x1FE8       /**< \brief (DSU offset) Peripheral Identification Register 2 */

#define DSU_PID2_JEPIDCH_Pos        0
#define DSU_PID2_JEPIDCH_Msk        (0x7u << DSU_PID2_JEPIDCH_Pos) /**< \brief (DSU_PID2)  */
#define DSU_PID2_JEPIDCH(value)     ((DSU_PID2_JEPIDCH_Msk & ((value) << DSU_PID2_JEPIDCH_Pos)))
#define DSU_PID2_JEPU               (0x1u <<  3) /**< \brief (DSU_PID2)  */
#define DSU_PID2_REVISION_Pos       4
#define DSU_PID2_REVISION_Msk       (0xFu << DSU_PID2_REVISION_Pos) /**< \brief (DSU_PID2)  */
#define DSU_PID2_REVISION(value)    ((DSU_PID2_REVISION_Msk & ((value) << DSU_PID2_REVISION_Pos)))
#define DSU_PID2_MASK               0xFFu /**< \brief (DSU_PID2) MASK Register */

/* -------- DSU_PID3 : (DSU Offset: 0x1FEC) (R/  32) Peripheral Identification Register 3 -------- */
typedef union {
  struct {
    uint32_t CUSMOD:4;         /*!< bit:  0.. 3                                     */
    uint32_t REVAND:4;         /*!< bit:  4.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID3_Type;

#define DSU_PID3_OFFSET             0x1FEC       /**< \brief (DSU offset) Peripheral Identification Register 3 */

#define DSU_PID3_CUSMOD_Pos         0
#define DSU_PID3_CUSMOD_Msk         (0xFu << DSU_PID3_CUSMOD_Pos) /**< \brief (DSU_PID3)  */
#define DSU_PID3_CUSMOD(value)      ((DSU_PID3_CUSMOD_Msk & ((value) << DSU_PID3_CUSMOD_Pos)))
#define DSU_PID3_REVAND_Pos         4
#define DSU_PID3_REVAND_Msk         (0xFu << DSU_PID3_REVAND_Pos) /**< \brief (DSU_PID3)  */
#define DSU_PID3_REVAND(value)      ((DSU_PID3_REVAND_Msk & ((value) << DSU_PID3_REVAND_Pos)))
#define DSU_PID3_MASK               0xFFu /**< \brief (DSU_PID3) MASK Register */

/* -------- DSU_CID0 : (DSU Offset: 0x1FF0) (R/  32) Component Identification Register 0 -------- */
typedef union {
  struct {
    uint32_t PREAMBLEB0:8;     /*!< bit:  0.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_CID0_Type;

#define DSU_CID0_OFFSET             0x1FF0       /**< \brief (DSU offset) Component Identification Register 0 */
#define DSU_CID0_RESETVALUE         0x0          /**< \brief (DSU reset_value) Component Identification Register 0 */

#define DSU_CID0_PREAMBLEB0_Pos     0
#define DSU_CID0_PREAMBLEB0_Msk     (0xFFu << DSU_CID0_PREAMBLEB0_Pos) /**< \brief (DSU_CID0)  */
#define DSU_CID0_PREAMBLEB0(value)  ((DSU_CID0_PREAMBLEB0_Msk & ((value) << DSU_CID0_PREAMBLEB0_Pos)))
#define DSU_CID0_MASK               0xFFu /**< \brief (DSU_CID0) MASK Register */

/* -------- DSU_CID1 : (DSU Offset: 0x1FF4) (R/  32) Component Identification Register 1 -------- */
typedef union {
  struct {
    uint32_t PREAMBLE:4;       /*!< bit:  0.. 3                                     */
    uint32_t CCLASS:4;         /*!< bit:  4.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_CID1_Type;

#define DSU_CID1_OFFSET             0x1FF4       /**< \brief (DSU offset) Component Identification Register 1 */
#define DSU_CID1_RESETVALUE         0x0          /**< \brief (DSU reset_value) Component Identification Register 1 */

#define DSU_CID1_PREAMBLE_Pos       0
#define DSU_CID1_PREAMBLE_Msk       (0xFu << DSU_CID1_PREAMBLE_Pos) /**< \brief (DSU_CID1)  */
#define DSU_CID1_PREAMBLE(value)    ((DSU_CID1_PREAMBLE_Msk & ((value) << DSU_CID1_PREAMBLE_Pos)))
#define DSU_CID1_CCLASS_Pos         4
#define DSU_CID1_CCLASS_Msk         (0xFu << DSU_CID1_CCLASS_Pos) /**< \brief (DSU_CID1)  */
#define DSU_CID1_CCLASS(value)      ((DSU_CID1_CCLASS_Msk & ((value) << DSU_CID1_CCLASS_Pos)))
#define DSU_CID1_MASK               0xFFu /**< \brief (DSU_CID1) MASK Register */

/* -------- DSU_CID2 : (DSU Offset: 0x1FF8) (R/  32) Component Identification Register 2 -------- */
typedef union {
  struct {
    uint32_t PREAMBLEB2:8;     /*!< bit:  0.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_CID2_Type;

#define DSU_CID2_OFFSET             0x1FF8       /**< \brief (DSU offset) Component Identification Register 2 */
#define DSU_CID2_RESETVALUE         0x0          /**< \brief (DSU reset_value) Component Identification Register 2 */

#define DSU_CID2_PREAMBLEB2_Pos     0
#define DSU_CID2_PREAMBLEB2_Msk     (0xFFu << DSU_CID2_PREAMBLEB2_Pos) /**< \brief (DSU_CID2)  */
#define DSU_CID2_PREAMBLEB2(value)  ((DSU_CID2_PREAMBLEB2_Msk & ((value) << DSU_CID2_PREAMBLEB2_Pos)))
#define DSU_CID2_MASK               0xFFu /**< \brief (DSU_CID2) MASK Register */

/* -------- DSU_CID3 : (DSU Offset: 0x1FFC) (R/  32) Component Identification Register 3 -------- */
typedef union {
  struct {
    uint32_t PREAMBLEB3:8;     /*!< bit:  0.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_CID3_Type;

#define DSU_CID3_OFFSET             0x1FFC       /**< \brief (DSU offset) Component Identification Register 3 */
#define DSU_CID3_RESETVALUE         0x0          /**< \brief (DSU reset_value) Component Identification Register 3 */

#define DSU_CID3_PREAMBLEB3_Pos     0
#define DSU_CID3_PREAMBLEB3_Msk     (0xFFu << DSU_CID3_PREAMBLEB3_Pos) /**< \brief (DSU_CID3)  */
#define DSU_CID3_PREAMBLEB3(value)  ((DSU_CID3_PREAMBLEB3_Msk & ((value) << DSU_CID3_PREAMBLEB3_Pos)))
#define DSU_CID3_MASK               0xFFu /**< \brief (DSU_CID3) MASK Register */

/** \brief DSU hardware registers */
typedef struct {
  __O  DSU_CTRL_Type             CTRL;        /**< \brief Offset: 0x0000 ( /W  8) Control Register */
  __IO DSU_STATUSA_Type          STATUSA;     /**< \brief Offset: 0x0001 (R/W  8) Status Register A */
  __I  DSU_STATUSB_Type          STATUSB;     /**< \brief Offset: 0x0002 (R/   8) Status Register B */
  __I  DSU_STATUSC_Type          STATUSC;     /**< \brief Offset: 0x0003 (R/   8) Status Register C */
  __IO DSU_ADDR_Type             ADDR;        /**< \brief Offset: 0x0004 (R/W 32) Address Register */
  __IO DSU_LENGTH_Type           LENGTH;      /**< \brief Offset: 0x0008 (R/W 32) Length Register */
  __IO DSU_DATA_Type             DATA;        /**< \brief Offset: 0x000C (R/W 32) Data Register */
  __IO DSU_DCC0_Type             DCC0;        /**< \brief Offset: 0x0010 (R/W 32) Debug Communication Channel 0 Register */
  __IO DSU_DCC1_Type             DCC1;        /**< \brief Offset: 0x0014 (R/W 32) Debug Communication Channel 1 Register */
  __I  DSU_DID_Type              DID;         /**< \brief Offset: 0x0018 (R/  32) Device Identification Register */
       RoReg8                    Reserved1[0xD4];
  __IO DSU_DCFG0_Type            DCFG0;       /**< \brief Offset: 0x00F0 (R/W 32) Device Configuration Register 0 */
  __I  DSU_DCFG1_Type            DCFG1;       /**< \brief Offset: 0x00F4 (R/  32) Device Configuration Register 1 */
  __IO DSU_UPTM_Type             UPTM;        /**< \brief Offset: 0x00F8 (R/W 32) UnProtected Test Mode Register */
  __IO DSU_TESTMODE_Type         TESTMODE;    /**< \brief Offset: 0x00FC (R/W 32) Test Mode Register */
       RoReg8                    Reserved2[0xF00];
  __I  DSU_ENTRY0_Type           ENTRY0;      /**< \brief Offset: 0x1000 (R/  32) Coresight ROM Table Entry 0 Register */
  __I  DSU_ENTRY1_Type           ENTRY1;      /**< \brief Offset: 0x1004 (R/  32) Coresight ROM Table Entry 1 Register */
  __I  DSU_END_Type              END;         /**< \brief Offset: 0x1008 (R/  32) Coresight ROM Table End Register */
       RoReg8                    Reserved3[0xFC0];
  __I  DSU_MEMTYPE_Type          MEMTYPE;     /**< \brief Offset: 0x1FCC (R/  32) Coresight ROM Table Memory Type Register */
  __I  DSU_PID4_Type             PID4;        /**< \brief Offset: 0x1FD0 (R/  32) Peripheral Identification Register 4 */
  __I  DSU_PID5_Type             PID5;        /**< \brief Offset: 0x1FD4 (R/  32) Peripheral Identification Register 5 */
  __I  DSU_PID6_Type             PID6;        /**< \brief Offset: 0x1FD8 (R/  32) Peripheral Identification Register 6 */
  __I  DSU_PID7_Type             PID7;        /**< \brief Offset: 0x1FDC (R/  32) Peripheral Identification Register 7 */
  __I  DSU_PID0_Type             PID0;        /**< \brief Offset: 0x1FE0 (R/  32) Peripheral Identification Register 0 */
  __I  DSU_PID1_Type             PID1;        /**< \brief Offset: 0x1FE4 (R/  32) Peripheral Identification Register 1 */
  __I  DSU_PID2_Type             PID2;        /**< \brief Offset: 0x1FE8 (R/  32) Peripheral Identification Register 2 */
  __I  DSU_PID3_Type             PID3;        /**< \brief Offset: 0x1FEC (R/  32) Peripheral Identification Register 3 */
  __I  DSU_CID0_Type             CID0;        /**< \brief Offset: 0x1FF0 (R/  32) Component Identification Register 0 */
  __I  DSU_CID1_Type             CID1;        /**< \brief Offset: 0x1FF4 (R/  32) Component Identification Register 1 */
  __I  DSU_CID2_Type             CID2;        /**< \brief Offset: 0x1FF8 (R/  32) Component Identification Register 2 */
  __I  DSU_CID3_Type             CID3;        /**< \brief Offset: 0x1FFC (R/  32) Component Identification Register 3 */
} Dsu;

#endif /*  __EXCLUDE_DSU_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR EIC */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_EIC External Interrupt Controller */
/*@{*/

#define REV_EIC        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_EIC_FROM_CMSIS__

/* -------- EIC_CTRL : (EIC Offset: 0x00) (R/W  8) Control Register -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable                             */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EIC_CTRL_Type;

#define EIC_CTRL_OFFSET             0x00         /**< \brief (EIC offset) Control Register */
#define EIC_CTRL_RESETVALUE         0x0          /**< \brief (EIC reset_value) Control Register */

#define EIC_CTRL_SWRST              (0x1u <<  0) /**< \brief (EIC_CTRL) Software Reset */
#define EIC_CTRL_ENABLE             (0x1u <<  1) /**< \brief (EIC_CTRL) Enable */
#define EIC_CTRL_MASK               0x3u /**< \brief (EIC_CTRL) MASK Register */

/* -------- EIC_STATUS : (EIC Offset: 0x01) (R/   8) Status Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:7;     /*!< bit:  0.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  SYNCBUSY                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EIC_STATUS_Type;

#define EIC_STATUS_OFFSET           0x01         /**< \brief (EIC offset) Status Register */
#define EIC_STATUS_RESETVALUE       0x0          /**< \brief (EIC reset_value) Status Register */

#define EIC_STATUS_SYNCBUSY         (0x1u <<  7) /**< \brief (EIC_STATUS) SYNCBUSY */
#define EIC_STATUS_MASK             0x80u /**< \brief (EIC_STATUS) MASK Register */

/* -------- EIC_NMICTRL : (EIC Offset: 0x02) (R/W  8) NMI Control Register -------- */
typedef union {
  struct {
    uint8_t  NMISENSE:3;       /*!< bit:  0.. 2  NMISENSE                           */
    uint8_t  NMIFILTEN:1;      /*!< bit:      3  NMIFILTEN                          */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EIC_NMICTRL_Type;

#define EIC_NMICTRL_OFFSET          0x02         /**< \brief (EIC offset) NMI Control Register */
#define EIC_NMICTRL_RESETVALUE      0x0          /**< \brief (EIC reset_value) NMI Control Register */

#define EIC_NMICTRL_NMISENSE_Pos    0
#define EIC_NMICTRL_NMISENSE_Msk    (0x7u << EIC_NMICTRL_NMISENSE_Pos) /**< \brief (EIC_NMICTRL) NMISENSE */
#define EIC_NMICTRL_NMISENSE(value) ((EIC_NMICTRL_NMISENSE_Msk & ((value) << EIC_NMICTRL_NMISENSE_Pos)))
#define EIC_NMICTRL_NMIFILTEN       (0x1u <<  3) /**< \brief (EIC_NMICTRL) NMIFILTEN */
#define EIC_NMICTRL_MASK            0xFu /**< \brief (EIC_NMICTRL) MASK Register */

/* -------- EIC_NMIFLAG : (EIC Offset: 0x03) (R/W  8) NMI Interrupt Flag Register -------- */
typedef union {
  struct {
    uint8_t  NMI:1;            /*!< bit:      0  NMI Interrupt Flag                 */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EIC_NMIFLAG_Type;

#define EIC_NMIFLAG_OFFSET          0x03         /**< \brief (EIC offset) NMI Interrupt Flag Register */
#define EIC_NMIFLAG_RESETVALUE      0x0          /**< \brief (EIC reset_value) NMI Interrupt Flag Register */

#define EIC_NMIFLAG_NMI             (0x1u <<  0) /**< \brief (EIC_NMIFLAG) NMI Interrupt Flag */
#define EIC_NMIFLAG_MASK            0x1u /**< \brief (EIC_NMIFLAG) MASK Register */

/* -------- EIC_EVCTRL : (EIC Offset: 0x04) (R/W 32) Event Control Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_EVCTRL_Type;

#define EIC_EVCTRL_OFFSET           0x04         /**< \brief (EIC offset) Event Control Register */
#define EIC_EVCTRL_RESETVALUE       0x0          /**< \brief (EIC reset_value) Event Control Register */
#define EIC_EVCTRL_MASK             0xFFFFFFFFu  /**< \brief (EIC_EVCTRL) MASK Register */

/* -------- EIC_INTENCLR : (EIC Offset: 0x08) (R/W 32) Interrupt Enable Clear Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_INTENCLR_Type;

#define EIC_INTENCLR_OFFSET         0x08         /**< \brief (EIC offset) Interrupt Enable Clear Register */
#define EIC_INTENCLR_RESETVALUE     0x0          /**< \brief (EIC reset_value) Interrupt Enable Clear Register */
#define EIC_INTENCLR_MASK           0xFFFFFFFFu  /**< \brief (EIC_INTENCLR) MASK Register */

/* -------- EIC_INTENSET : (EIC Offset: 0x0C) (R/W 32) Interrupt Enable Set Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_INTENSET_Type;

#define EIC_INTENSET_OFFSET         0x0C         /**< \brief (EIC offset) Interrupt Enable Set Register */
#define EIC_INTENSET_RESETVALUE     0x0          /**< \brief (EIC reset_value) Interrupt Enable Set Register */
#define EIC_INTENSET_MASK           0xFFFFFFFFu  /**< \brief (EIC_INTENSET) MASK Register */

/* -------- EIC_INTFLAG : (EIC Offset: 0x10) (R/W 32) Interrupt Flag Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_INTFLAG_Type;

#define EIC_INTFLAG_OFFSET          0x10         /**< \brief (EIC offset) Interrupt Flag Register */
#define EIC_INTFLAG_RESETVALUE      0x0          /**< \brief (EIC reset_value) Interrupt Flag Register */
#define EIC_INTFLAG_MASK            0xFFFFFFFFu  /**< \brief (EIC_INTFLAG) MASK Register */

/* -------- EIC_WAKEUP : (EIC Offset: 0x14) (R/W 32) Wake-up Enable Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_WAKEUP_Type;

#define EIC_WAKEUP_OFFSET           0x14         /**< \brief (EIC offset) Wake-up Enable Register */
#define EIC_WAKEUP_RESETVALUE       0x0          /**< \brief (EIC reset_value) Wake-up Enable Register */
#define EIC_WAKEUP_MASK             0xFFFFFFFFu  /**< \brief (EIC_WAKEUP) MASK Register */

/* -------- EIC_CONFIG : (EIC Offset: 0x18) (R/W 32) Config Config Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_CONFIG_Type;

#define EIC_CONFIG_OFFSET           0x18         /**< \brief (EIC offset) Config Register */
#define EIC_CONFIG_RESETVALUE       0x0          /**< \brief (EIC reset_value) Config Register */
#define EIC_CONFIG_MASK             0xFFFFFFFFu  /**< \brief (EIC_CONFIG) MASK Register */

#define EIC_NUMBER_OF_CONFIG_REGS   2

/** \brief EIC hardware registers */
typedef struct {
  __IO EIC_CTRL_Type             CTRL;        /**< \brief Offset: 0x00 (R/W  8) Control Register */
  __I  EIC_STATUS_Type           STATUS;      /**< \brief Offset: 0x01 (R/   8) Status Register */
  __IO EIC_NMICTRL_Type          NMICTRL;     /**< \brief Offset: 0x02 (R/W  8) NMI Control Register */
  __IO EIC_NMIFLAG_Type          NMIFLAG;     /**< \brief Offset: 0x03 (R/W  8) NMI Interrupt Flag Register */
  __IO EIC_EVCTRL_Type           EVCTRL;      /**< \brief Offset: 0x04 (R/W 32) Event Control Register */
  __IO EIC_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x08 (R/W 32) Interrupt Enable Clear Register */
  __IO EIC_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x0C (R/W 32) Interrupt Enable Set Register */
  __IO EIC_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x10 (R/W 32) Interrupt Flag Register */
  __IO EIC_WAKEUP_Type           WAKEUP;      /**< \brief Offset: 0x14 (R/W 32) Wake-up Enable Register */
  __IO EIC_CONFIG_Type           CONFIG[EIC_NUMBER_OF_CONFIG_REGS]; /**< \brief Offset: 0x18 (R/W 32) Config Register */
       EIC_CONFIG_Type           Reserved1[4 - EIC_NUMBER_OF_CONFIG_REGS];
} Eic;

#endif /*  __EXCLUDE_EIC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR EVSYS */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_EVSYS Event System Interface */
/*@{*/

#define REV_EVSYS      0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_EVSYS_FROM_CMSIS__

/* -------- EVSYS_CTRL : (EVSYS Offset: 0x00) ( /W  8) Control Register -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0                                     */
    uint8_t  _reserved0:3;     /*!< bit:  1.. 3  Reserved                           */
    uint8_t  GCLKREQ:1;        /*!< bit:      4                                     */
    uint8_t  _reserved1:3;     /*!< bit:  5.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EVSYS_CTRL_Type;

#define EVSYS_CTRL_OFFSET           0x00         /**< \brief (EVSYS offset) Control Register */
#define EVSYS_CTRL_RESETVALUE       0x0          /**< \brief (EVSYS reset_value) Control Register */

#define EVSYS_CTRL_SWRST            (0x1u <<  0) /**< \brief (EVSYS_CTRL)  */
#define EVSYS_CTRL_GCLKREQ          (0x1u <<  4) /**< \brief (EVSYS_CTRL)  */
#define EVSYS_CTRL_MASK             0x11u /**< \brief (EVSYS_CTRL) MASK Register */

/* -------- EVSYS_CHANNEL : (EVSYS Offset: 0x04) (R/W 32) Channel Register -------- */
typedef union {
  struct {
    uint32_t CHANNEL:8;        /*!< bit:  0.. 7  Channel Selection                  */
    uint32_t SWEVT:1;          /*!< bit:      8  Software Event                     */
    uint32_t _reserved0:7;     /*!< bit:  9..15  Reserved                           */
    uint32_t EVGEN:8;          /*!< bit: 16..23  Event Generator Selection          */
    uint32_t PATH:2;           /*!< bit: 24..25  Path Selection                     */
    uint32_t EDGSEL:2;         /*!< bit: 26..27  Edge Selection                     */
    uint32_t _reserved1:4;     /*!< bit: 28..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_CHANNEL_Type;

#define EVSYS_CHANNEL_OFFSET        0x04         /**< \brief (EVSYS offset) Channel Register */
#define EVSYS_CHANNEL_RESETVALUE    0x0          /**< \brief (EVSYS reset_value) Channel Register */

#define EVSYS_CHANNEL_CHANNEL_Pos    0
#define EVSYS_CHANNEL_CHANNEL_Msk    (0xFFu << EVSYS_CHANNEL_CHANNEL_Pos) /**< \brief (EVSYS_CHANNEL) Channel Selection */
#define EVSYS_CHANNEL_CHANNEL(value) ((EVSYS_CHANNEL_CHANNEL_Msk & ((value) << EVSYS_CHANNEL_CHANNEL_Pos)))
#define EVSYS_CHANNEL_SWEVT         (0x1u <<  8) /**< \brief (EVSYS_CHANNEL) Software Event */
#define EVSYS_CHANNEL_EVGEN_Pos     16
#define EVSYS_CHANNEL_EVGEN_Msk     (0xFFu << EVSYS_CHANNEL_EVGEN_Pos) /**< \brief (EVSYS_CHANNEL) Event Generator Selection */
#define EVSYS_CHANNEL_EVGEN(value)  ((EVSYS_CHANNEL_EVGEN_Msk & ((value) << EVSYS_CHANNEL_EVGEN_Pos)))
#define EVSYS_CHANNEL_PATH_Pos      24
#define EVSYS_CHANNEL_PATH_Msk      (0x3u << EVSYS_CHANNEL_PATH_Pos) /**< \brief (EVSYS_CHANNEL) Path Selection */
#define EVSYS_CHANNEL_PATH(value)   ((EVSYS_CHANNEL_PATH_Msk & ((value) << EVSYS_CHANNEL_PATH_Pos)))
#define EVSYS_CHANNEL_EDGSEL_Pos    26
#define EVSYS_CHANNEL_EDGSEL_Msk    (0x3u << EVSYS_CHANNEL_EDGSEL_Pos) /**< \brief (EVSYS_CHANNEL) Edge Selection */
#define EVSYS_CHANNEL_EDGSEL(value) ((EVSYS_CHANNEL_EDGSEL_Msk & ((value) << EVSYS_CHANNEL_EDGSEL_Pos)))
#define EVSYS_CHANNEL_MASK          0xFFF01FFu /**< \brief (EVSYS_CHANNEL) MASK Register */

/* -------- EVSYS_USERMUX : (EVSYS Offset: 0x08) (R/W 16) User Mux Register -------- */
typedef union {
  struct {
    uint16_t UMUXSEL:8;        /*!< bit:  0.. 7  User Mux Selection                 */
    uint16_t CHANNELEVENT:8;   /*!< bit:  8..15  Channel Event Selection            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} EVSYS_USERMUX_Type;

#define EVSYS_USERMUX_OFFSET        0x08         /**< \brief (EVSYS offset) User Mux Register */
#define EVSYS_USERMUX_RESETVALUE    0x0          /**< \brief (EVSYS reset_value) User Mux Register */

#define EVSYS_USERMUX_UMUXSEL_Pos    0
#define EVSYS_USERMUX_UMUXSEL_Msk    (0xFFu << EVSYS_USERMUX_UMUXSEL_Pos) /**< \brief (EVSYS_USERMUX) User Mux Selection */
#define EVSYS_USERMUX_UMUXSEL(value) ((EVSYS_USERMUX_UMUXSEL_Msk & ((value) << EVSYS_USERMUX_UMUXSEL_Pos)))
#define EVSYS_USERMUX_CHANNELEVENT_Pos    8
#define EVSYS_USERMUX_CHANNELEVENT_Msk    (0xFFu << EVSYS_USERMUX_CHANNELEVENT_Pos) /**< \brief (EVSYS_USERMUX) Channel Event Selection */
#define EVSYS_USERMUX_CHANNELEVENT(value) ((EVSYS_USERMUX_CHANNELEVENT_Msk & ((value) << EVSYS_USERMUX_CHANNELEVENT_Pos)))
#define EVSYS_USERMUX_MASK          0xFFFFu /**< \brief (EVSYS_USERMUX) MASK Register */

/* -------- EVSYS_CHSTATUS : (EVSYS Offset: 0x0C) (R/  32) Channel Status Register -------- */
typedef union {
  struct {
    uint32_t USRREADY0:8;      /*!< bit:  0.. 7  User Ready for Channel 0 to 7(modulo 16) */
    uint32_t CHBUSY0:8;        /*!< bit:  8..15  Channel Busy 0 to 7(modulo 16)     */
    uint32_t USRREADY1:8;      /*!< bit: 16..23  User Ready for Channel 8 to 15(modulo 16) */
    uint32_t CHBUSY1:8;        /*!< bit: 24..31  Channel Busy 8 to 15(modulo 16)    */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_CHSTATUS_Type;

#define EVSYS_CHSTATUS_OFFSET       0x0C         /**< \brief (EVSYS offset) Channel Status Register */
#define EVSYS_CHSTATUS_RESETVALUE   0x0          /**< \brief (EVSYS reset_value) Channel Status Register */

#define EVSYS_CHSTATUS_USRREADY0_Pos    0
#define EVSYS_CHSTATUS_USRREADY0_Msk    (0xFFu << EVSYS_CHSTATUS_USRREADY0_Pos) /**< \brief (EVSYS_CHSTATUS) User Ready for Channel 0 to 7(modulo 16) */
#define EVSYS_CHSTATUS_USRREADY0(value) ((EVSYS_CHSTATUS_USRREADY0_Msk & ((value) << EVSYS_CHSTATUS_USRREADY0_Pos)))
#define EVSYS_CHSTATUS_CHBUSY0_Pos    8
#define EVSYS_CHSTATUS_CHBUSY0_Msk    (0xFFu << EVSYS_CHSTATUS_CHBUSY0_Pos) /**< \brief (EVSYS_CHSTATUS) Channel Busy 0 to 7(modulo 16) */
#define EVSYS_CHSTATUS_CHBUSY0(value) ((EVSYS_CHSTATUS_CHBUSY0_Msk & ((value) << EVSYS_CHSTATUS_CHBUSY0_Pos)))
#define EVSYS_CHSTATUS_USRREADY1_Pos    16
#define EVSYS_CHSTATUS_USRREADY1_Msk    (0xFFu << EVSYS_CHSTATUS_USRREADY1_Pos) /**< \brief (EVSYS_CHSTATUS) User Ready for Channel 8 to 15(modulo 16) */
#define EVSYS_CHSTATUS_USRREADY1(value) ((EVSYS_CHSTATUS_USRREADY1_Msk & ((value) << EVSYS_CHSTATUS_USRREADY1_Pos)))
#define EVSYS_CHSTATUS_CHBUSY1_Pos    24
#define EVSYS_CHSTATUS_CHBUSY1_Msk    (0xFFu << EVSYS_CHSTATUS_CHBUSY1_Pos) /**< \brief (EVSYS_CHSTATUS) Channel Busy 8 to 15(modulo 16) */
#define EVSYS_CHSTATUS_CHBUSY1(value) ((EVSYS_CHSTATUS_CHBUSY1_Msk & ((value) << EVSYS_CHSTATUS_CHBUSY1_Pos)))
#define EVSYS_CHSTATUS_MASK         0xFFFFFFFFu /**< \brief (EVSYS_CHSTATUS) MASK Register */

/* -------- EVSYS_IENCLR : (EVSYS Offset: 0x10) (R/W 32) Interrupt Enable Clear register -------- */
typedef union {
  struct {
    uint32_t OVR0:8;           /*!< bit:  0.. 7  Overrun for Channel 0 to 7(modulo 16) */
    uint32_t EVD0:8;           /*!< bit:  8..15  Event Detection for Channel 0 to 7(modulo 16) */
    uint32_t OVR1:8;           /*!< bit: 16..23  Overrun for Channel 8 to 15(modulo 16) */
    uint32_t EVD1:8;           /*!< bit: 24..31  Event Detection for Channel 8 to 15(modulo 16) */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_IENCLR_Type;

#define EVSYS_IENCLR_OFFSET         0x10         /**< \brief (EVSYS offset) Interrupt Enable Clear register */
#define EVSYS_IENCLR_RESETVALUE     0x0          /**< \brief (EVSYS reset_value) Interrupt Enable Clear register */

#define EVSYS_IENCLR_OVR0_Pos       0
#define EVSYS_IENCLR_OVR0_Msk       (0xFFu << EVSYS_IENCLR_OVR0_Pos) /**< \brief (EVSYS_IENCLR) Overrun for Channel 0 to 7(modulo 16) */
#define EVSYS_IENCLR_OVR0(value)    ((EVSYS_IENCLR_OVR0_Msk & ((value) << EVSYS_IENCLR_OVR0_Pos)))
#define EVSYS_IENCLR_EVD0_Pos       8
#define EVSYS_IENCLR_EVD0_Msk       (0xFFu << EVSYS_IENCLR_EVD0_Pos) /**< \brief (EVSYS_IENCLR) Event Detection for Channel 0 to 7(modulo 16) */
#define EVSYS_IENCLR_EVD0(value)    ((EVSYS_IENCLR_EVD0_Msk & ((value) << EVSYS_IENCLR_EVD0_Pos)))
#define EVSYS_IENCLR_OVR1_Pos       16
#define EVSYS_IENCLR_OVR1_Msk       (0xFFu << EVSYS_IENCLR_OVR1_Pos) /**< \brief (EVSYS_IENCLR) Overrun for Channel 8 to 15(modulo 16) */
#define EVSYS_IENCLR_OVR1(value)    ((EVSYS_IENCLR_OVR1_Msk & ((value) << EVSYS_IENCLR_OVR1_Pos)))
#define EVSYS_IENCLR_EVD1_Pos       24
#define EVSYS_IENCLR_EVD1_Msk       (0xFFu << EVSYS_IENCLR_EVD1_Pos) /**< \brief (EVSYS_IENCLR) Event Detection for Channel 8 to 15(modulo 16) */
#define EVSYS_IENCLR_EVD1(value)    ((EVSYS_IENCLR_EVD1_Msk & ((value) << EVSYS_IENCLR_EVD1_Pos)))
#define EVSYS_IENCLR_MASK           0xFFFFFFFFu /**< \brief (EVSYS_IENCLR) MASK Register */

/* -------- EVSYS_IENSET : (EVSYS Offset: 0x14) (R/W 32) Interrupt Enable Set Register -------- */
typedef union {
  struct {
    uint32_t OVR0:8;           /*!< bit:  0.. 7  Overrun for Channel 0 to 7(modulo 16) */
    uint32_t EVD0:8;           /*!< bit:  8..15  Event Detection for Channel 0 to 7(modulo 16) */
    uint32_t OVR1:8;           /*!< bit: 16..23  Overrun for Channel 8 to 15(modulo 16) */
    uint32_t EVD1:8;           /*!< bit: 24..31  Event Detection for Channel 8 to 15(modulo 16) */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_IENSET_Type;

#define EVSYS_IENSET_OFFSET         0x14         /**< \brief (EVSYS offset) Interrupt Enable Set Register */
#define EVSYS_IENSET_RESETVALUE     0x0          /**< \brief (EVSYS reset_value) Interrupt Enable Set Register */

#define EVSYS_IENSET_OVR0_Pos       0
#define EVSYS_IENSET_OVR0_Msk       (0xFFu << EVSYS_IENSET_OVR0_Pos) /**< \brief (EVSYS_IENSET) Overrun for Channel 0 to 7(modulo 16) */
#define EVSYS_IENSET_OVR0(value)    ((EVSYS_IENSET_OVR0_Msk & ((value) << EVSYS_IENSET_OVR0_Pos)))
#define EVSYS_IENSET_EVD0_Pos       8
#define EVSYS_IENSET_EVD0_Msk       (0xFFu << EVSYS_IENSET_EVD0_Pos) /**< \brief (EVSYS_IENSET) Event Detection for Channel 0 to 7(modulo 16) */
#define EVSYS_IENSET_EVD0(value)    ((EVSYS_IENSET_EVD0_Msk & ((value) << EVSYS_IENSET_EVD0_Pos)))
#define EVSYS_IENSET_OVR1_Pos       16
#define EVSYS_IENSET_OVR1_Msk       (0xFFu << EVSYS_IENSET_OVR1_Pos) /**< \brief (EVSYS_IENSET) Overrun for Channel 8 to 15(modulo 16) */
#define EVSYS_IENSET_OVR1(value)    ((EVSYS_IENSET_OVR1_Msk & ((value) << EVSYS_IENSET_OVR1_Pos)))
#define EVSYS_IENSET_EVD1_Pos       24
#define EVSYS_IENSET_EVD1_Msk       (0xFFu << EVSYS_IENSET_EVD1_Pos) /**< \brief (EVSYS_IENSET) Event Detection for Channel 8 to 15(modulo 16) */
#define EVSYS_IENSET_EVD1(value)    ((EVSYS_IENSET_EVD1_Msk & ((value) << EVSYS_IENSET_EVD1_Pos)))
#define EVSYS_IENSET_MASK           0xFFFFFFFFu /**< \brief (EVSYS_IENSET) MASK Register */

/* -------- EVSYS_INTFLAG : (EVSYS Offset: 0x18) (R/W 32) Interrupt Flag Register -------- */
typedef union {
  struct {
    uint32_t OVR0:8;           /*!< bit:  0.. 7  Overrun for Channel 0 to 7(modulo 16) */
    uint32_t EVD0:8;           /*!< bit:  8..15  Event Detection for Channel 0 to 7(modulo 16) */
    uint32_t OVR1:8;           /*!< bit: 16..23  Overrun for Channel 8 to 15(modulo 16) */
    uint32_t EVD1:8;           /*!< bit: 24..31  Event Detection for Channel 8 to 15(modulo 16) */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_INTFLAG_Type;

#define EVSYS_INTFLAG_OFFSET        0x18         /**< \brief (EVSYS offset) Interrupt Flag Register */
#define EVSYS_INTFLAG_RESETVALUE    0x0          /**< \brief (EVSYS reset_value) Interrupt Flag Register */

#define EVSYS_INTFLAG_OVR0_Pos      0
#define EVSYS_INTFLAG_OVR0_Msk      (0xFFu << EVSYS_INTFLAG_OVR0_Pos) /**< \brief (EVSYS_INTFLAG) Overrun for Channel 0 to 7(modulo 16) */
#define EVSYS_INTFLAG_OVR0(value)   ((EVSYS_INTFLAG_OVR0_Msk & ((value) << EVSYS_INTFLAG_OVR0_Pos)))
#define EVSYS_INTFLAG_EVD0_Pos      8
#define EVSYS_INTFLAG_EVD0_Msk      (0xFFu << EVSYS_INTFLAG_EVD0_Pos) /**< \brief (EVSYS_INTFLAG) Event Detection for Channel 0 to 7(modulo 16) */
#define EVSYS_INTFLAG_EVD0(value)   ((EVSYS_INTFLAG_EVD0_Msk & ((value) << EVSYS_INTFLAG_EVD0_Pos)))
#define EVSYS_INTFLAG_OVR1_Pos      16
#define EVSYS_INTFLAG_OVR1_Msk      (0xFFu << EVSYS_INTFLAG_OVR1_Pos) /**< \brief (EVSYS_INTFLAG) Overrun for Channel 8 to 15(modulo 16) */
#define EVSYS_INTFLAG_OVR1(value)   ((EVSYS_INTFLAG_OVR1_Msk & ((value) << EVSYS_INTFLAG_OVR1_Pos)))
#define EVSYS_INTFLAG_EVD1_Pos      24
#define EVSYS_INTFLAG_EVD1_Msk      (0xFFu << EVSYS_INTFLAG_EVD1_Pos) /**< \brief (EVSYS_INTFLAG) Event Detection for Channel 8 to 15(modulo 16) */
#define EVSYS_INTFLAG_EVD1(value)   ((EVSYS_INTFLAG_EVD1_Msk & ((value) << EVSYS_INTFLAG_EVD1_Pos)))
#define EVSYS_INTFLAG_MASK          0xFFFFFFFFu /**< \brief (EVSYS_INTFLAG) MASK Register */

/** \brief EVSYS hardware registers */
typedef struct {
  __O  EVSYS_CTRL_Type           CTRL;        /**< \brief Offset: 0x00 ( /W  8) Control Register */
       RoReg8                    Reserved1[0x3];
  __IO EVSYS_CHANNEL_Type        CHANNEL;     /**< \brief Offset: 0x04 (R/W 32) Channel Register */
  __IO EVSYS_USERMUX_Type        USERMUX;     /**< \brief Offset: 0x08 (R/W 16) User Mux Register */
       RoReg8                    Reserved2[0x2];
  __I  EVSYS_CHSTATUS_Type       CHSTATUS;    /**< \brief Offset: 0x0C (R/  32) Channel Status Register */
  __IO EVSYS_IENCLR_Type         IENCLR;      /**< \brief Offset: 0x10 (R/W 32) Interrupt Enable Clear register */
  __IO EVSYS_IENSET_Type         IENSET;      /**< \brief Offset: 0x14 (R/W 32) Interrupt Enable Set Register */
  __IO EVSYS_INTFLAG_Type        INTFLAG;     /**< \brief Offset: 0x18 (R/W 32) Interrupt Flag Register */
} Evsys;

#endif /*  __EXCLUDE_EVSYS_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR GCLK */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_GCLK Generic Clock Generator */
/*@{*/

#define REV_GCLK       0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_GCLK_FROM_CMSIS__

/* -------- GCLK_CTRL : (GCLK Offset: 0x0) (R/W  8) Control Register -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0                                     */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} GCLK_CTRL_Type;

#define GCLK_CTRL_OFFSET            0x0          /**< \brief (GCLK offset) Control Register */
#define GCLK_CTRL_RESETVALUE        0x0          /**< \brief (GCLK reset_value) Control Register */

#define GCLK_CTRL_SWRST             (0x1u <<  0) /**< \brief (GCLK_CTRL)  */
#define GCLK_CTRL_MASK              0x1u /**< \brief (GCLK_CTRL) MASK Register */

/* -------- GCLK_STATUS : (GCLK Offset: 0x1) (R/W  8) Status Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:7;     /*!< bit:  0.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} GCLK_STATUS_Type;

#define GCLK_STATUS_OFFSET          0x1          /**< \brief (GCLK offset) Status Register */

#define GCLK_STATUS_SYNCBUSY        (0x1u <<  7) /**< \brief (GCLK_STATUS)  */
#define GCLK_STATUS_MASK            0x80u /**< \brief (GCLK_STATUS) MASK Register */

/* -------- GCLK_CLKCTRL : (GCLK Offset: 0x2) (R/W 16) Generic Clock Control Register -------- */
typedef union {
  struct {
    uint16_t ID:6;             /*!< bit:  0.. 5                                     */
    uint16_t _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
    uint16_t GEN:4;            /*!< bit:  8..11                                     */
    uint16_t _reserved1:2;     /*!< bit: 12..13  Reserved                           */
    uint16_t CLKEN:1;          /*!< bit:     14                                     */
    uint16_t SLEEPEN:1;        /*!< bit:     15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} GCLK_CLKCTRL_Type;

#define GCLK_CLKCTRL_OFFSET         0x2          /**< \brief (GCLK offset) Generic Clock Control Register */
#define GCLK_CLKCTRL_RESETVALUE     0x0          /**< \brief (GCLK reset_value) Generic Clock Control Register */

#define GCLK_CLKCTRL_ID_Pos         0
#define GCLK_CLKCTRL_ID_Msk         (0x3Fu << GCLK_CLKCTRL_ID_Pos) /**< \brief (GCLK_CLKCTRL)  */
#define GCLK_CLKCTRL_ID(value)      ((GCLK_CLKCTRL_ID_Msk & ((value) << GCLK_CLKCTRL_ID_Pos)))
#define GCLK_CLKCTRL_GEN_Pos        8
#define GCLK_CLKCTRL_GEN_Msk        (0xFu << GCLK_CLKCTRL_GEN_Pos) /**< \brief (GCLK_CLKCTRL)  */
#define GCLK_CLKCTRL_GEN(value)     ((GCLK_CLKCTRL_GEN_Msk & ((value) << GCLK_CLKCTRL_GEN_Pos)))
#define GCLK_CLKCTRL_CLKEN          (0x1u << 14) /**< \brief (GCLK_CLKCTRL)  */
#define GCLK_CLKCTRL_SLEEPEN        (0x1u << 15) /**< \brief (GCLK_CLKCTRL)  */
#define GCLK_CLKCTRL_MASK           0xCF3Fu /**< \brief (GCLK_CLKCTRL) MASK Register */

/* -------- GCLK_GENCTRL : (GCLK Offset: 0x4) (R/W 32) Generic Clock Generator Control Register -------- */
typedef union {
  struct {
    uint32_t ID:4;             /*!< bit:  0.. 3                                     */
    uint32_t _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
    uint32_t SRC:5;            /*!< bit:  8..12                                     */
    uint32_t _reserved1:3;     /*!< bit: 13..15  Reserved                           */
    uint32_t GENEN:1;          /*!< bit:     16                                     */
    uint32_t IDC:1;            /*!< bit:     17                                     */
    uint32_t OOV:1;            /*!< bit:     18                                     */
    uint32_t OE:1;             /*!< bit:     19                                     */
    uint32_t DIVSEL:1;         /*!< bit:     20                                     */
    uint32_t _reserved2:11;    /*!< bit: 21..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} GCLK_GENCTRL_Type;

#define GCLK_GENCTRL_OFFSET         0x4          /**< \brief (GCLK offset) Generic Clock Generator Control Register */
#define GCLK_GENCTRL_RESETVALUE     0x0          /**< \brief (GCLK reset_value) Generic Clock Generator Control Register */

#define GCLK_GENCTRL_ID_Pos         0
#define GCLK_GENCTRL_ID_Msk         (0xFu << GCLK_GENCTRL_ID_Pos) /**< \brief (GCLK_GENCTRL)  */
#define GCLK_GENCTRL_ID(value)      ((GCLK_GENCTRL_ID_Msk & ((value) << GCLK_GENCTRL_ID_Pos)))
#define GCLK_GENCTRL_SRC_Pos        8
#define GCLK_GENCTRL_SRC_Msk        (0x1Fu << GCLK_GENCTRL_SRC_Pos) /**< \brief (GCLK_GENCTRL)  */
#define GCLK_GENCTRL_SRC(value)     ((GCLK_GENCTRL_SRC_Msk & ((value) << GCLK_GENCTRL_SRC_Pos)))
#define GCLK_GENCTRL_GENEN          (0x1u << 16) /**< \brief (GCLK_GENCTRL)  */
#define GCLK_GENCTRL_IDC            (0x1u << 17) /**< \brief (GCLK_GENCTRL)  */
#define GCLK_GENCTRL_OOV            (0x1u << 18) /**< \brief (GCLK_GENCTRL)  */
#define GCLK_GENCTRL_OE             (0x1u << 19) /**< \brief (GCLK_GENCTRL)  */
#define GCLK_GENCTRL_DIVSEL         (0x1u << 20) /**< \brief (GCLK_GENCTRL)  */
#define GCLK_GENCTRL_MASK           0x1F1F0Fu /**< \brief (GCLK_GENCTRL) MASK Register */

/* -------- GCLK_GENDIV : (GCLK Offset: 0x8) (R/W 32) Generic Clock Generator Division Register -------- */
typedef union {
  struct {
    uint32_t ID:4;             /*!< bit:  0.. 3                                     */
    uint32_t _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
    uint32_t DIV:24;           /*!< bit:  8..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} GCLK_GENDIV_Type;

#define GCLK_GENDIV_OFFSET          0x8          /**< \brief (GCLK offset) Generic Clock Generator Division Register */
#define GCLK_GENDIV_RESETVALUE      0x0          /**< \brief (GCLK reset_value) Generic Clock Generator Division Register */

#define GCLK_GENDIV_ID_Pos          0
#define GCLK_GENDIV_ID_Msk          (0xFu << GCLK_GENDIV_ID_Pos) /**< \brief (GCLK_GENDIV)  */
#define GCLK_GENDIV_ID(value)       ((GCLK_GENDIV_ID_Msk & ((value) << GCLK_GENDIV_ID_Pos)))
#define GCLK_GENDIV_DIV_Pos         8
#define GCLK_GENDIV_DIV_Msk         (0xFFFFFFu << GCLK_GENDIV_DIV_Pos) /**< \brief (GCLK_GENDIV)  */
#define GCLK_GENDIV_DIV(value)      ((GCLK_GENDIV_DIV_Msk & ((value) << GCLK_GENDIV_DIV_Pos)))
#define GCLK_GENDIV_MASK            0xFFFFFF0Fu /**< \brief (GCLK_GENDIV) MASK Register */

/** \brief GCLK hardware registers */
typedef struct {
  __IO GCLK_CTRL_Type            CTRL;        /**< \brief Offset: 0x0 (R/W  8) Control Register */
  __IO GCLK_STATUS_Type          STATUS;      /**< \brief Offset: 0x1 (R/W  8) Status Register */
  __IO GCLK_CLKCTRL_Type         CLKCTRL;     /**< \brief Offset: 0x2 (R/W 16) Generic Clock Control Register */
  __IO GCLK_GENCTRL_Type         GENCTRL;     /**< \brief Offset: 0x4 (R/W 32) Generic Clock Generator Control Register */
  __IO GCLK_GENDIV_Type          GENDIV;      /**< \brief Offset: 0x8 (R/W 32) Generic Clock Generator Division Register */
} Gclk;

#endif /*  __EXCLUDE_GCLK_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR HMATRIXB */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_HMATRIXB HSB Matrix */
/*@{*/

#define REV_HMATRIXB   0x130

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_HMATRIXB_FROM_CMSIS__

/* -------- HMATRIXB_MCFG : (HMATRIXB Offset: 0x000) (R/W 32) Mcfg Master Configuration Register 0 -------- */
typedef union {
  struct {
    uint32_t ULBT:3;           /*!< bit:  0.. 2  Undefined Length Burst Type        */
    uint32_t _reserved0:29;    /*!< bit:  3..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_MCFG_Type;

#define HMATRIXB_MCFG_OFFSET        0x000        /**< \brief (HMATRIXB offset) Master Configuration Register 0 */
#define HMATRIXB_MCFG_RESETVALUE    0x2          /**< \brief (HMATRIXB reset_value) Master Configuration Register 0 */

#define HMATRIXB_MCFG_ULBT_Pos      0
#define HMATRIXB_MCFG_ULBT_Msk      (0x7u << HMATRIXB_MCFG_ULBT_Pos) /**< \brief (HMATRIXB_MCFG) Undefined Length Burst Type */
#define HMATRIXB_MCFG_ULBT(value)   ((HMATRIXB_MCFG_ULBT_Msk & ((value) << HMATRIXB_MCFG_ULBT_Pos)))
#define   HMATRIXB_MCFG_ULBT_INFINITE (0x0u <<  0) /**< \brief (HMATRIXB_MCFG) Infinite Length */
#define   HMATRIXB_MCFG_ULBT_SINGLE (0x1u <<  0) /**< \brief (HMATRIXB_MCFG) Single Access */
#define   HMATRIXB_MCFG_ULBT_FOUR_BEAT (0x2u <<  0) /**< \brief (HMATRIXB_MCFG) Four Beat Burst */
#define   HMATRIXB_MCFG_ULBT_EIGHT_BEAT (0x3u <<  0) /**< \brief (HMATRIXB_MCFG) Eight Beat Burst */
#define   HMATRIXB_MCFG_ULBT_SIXTEEN_BEAT (0x4u <<  0) /**< \brief (HMATRIXB_MCFG) Sixteen Beat Burst */
#define HMATRIXB_MCFG_MASK          0x7u /**< \brief (HMATRIXB_MCFG) MASK Register */

/* -------- HMATRIXB_SCFG : (HMATRIXB Offset: 0x040) (R/W 32) Scfg Slave Configuration Register 0 -------- */
typedef union {
  struct {
    uint32_t SLOT_CYCLE:8;     /*!< bit:  0.. 7  Maximum Number of Allowed Cycles for a Burst */
    uint32_t _reserved0:8;     /*!< bit:  8..15  Reserved                           */
    uint32_t DEFMSTR_TYPE:2;   /*!< bit: 16..17  Default Master Type                */
    uint32_t FIXED_DEFMSTR:4;  /*!< bit: 18..21  Fixed Index of Default Master      */
    uint32_t _reserved1:2;     /*!< bit: 22..23  Reserved                           */
    uint32_t ARBT:1;           /*!< bit:     24  Arbitration Type                   */
    uint32_t _reserved2:7;     /*!< bit: 25..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_SCFG_Type;

#define HMATRIXB_SCFG_OFFSET        0x040        /**< \brief (HMATRIXB offset) Slave Configuration Register 0 */
#define HMATRIXB_SCFG_RESETVALUE    0x10         /**< \brief (HMATRIXB reset_value) Slave Configuration Register 0 */

#define HMATRIXB_SCFG_SLOT_CYCLE_Pos    0
#define HMATRIXB_SCFG_SLOT_CYCLE_Msk    (0xFFu << HMATRIXB_SCFG_SLOT_CYCLE_Pos) /**< \brief (HMATRIXB_SCFG) Maximum Number of Allowed Cycles for a Burst */
#define HMATRIXB_SCFG_SLOT_CYCLE(value) ((HMATRIXB_SCFG_SLOT_CYCLE_Msk & ((value) << HMATRIXB_SCFG_SLOT_CYCLE_Pos)))
#define HMATRIXB_SCFG_DEFMSTR_TYPE_Pos    16
#define HMATRIXB_SCFG_DEFMSTR_TYPE_Msk    (0x3u << HMATRIXB_SCFG_DEFMSTR_TYPE_Pos) /**< \brief (HMATRIXB_SCFG) Default Master Type */
#define HMATRIXB_SCFG_DEFMSTR_TYPE(value) ((HMATRIXB_SCFG_DEFMSTR_TYPE_Msk & ((value) << HMATRIXB_SCFG_DEFMSTR_TYPE_Pos)))
#define   HMATRIXB_SCFG_DEFMSTR_TYPE_NO_DEFAULT (0x0u << 16) /**< \brief (HMATRIXB_SCFG) No Default Master. At the end of current slave access, if no other master request is pending, the slave is deconnected from all masters. This resusts in having a one cycle latency for the first transfer of a burst. */
#define   HMATRIXB_SCFG_DEFMSTR_TYPE_LAST_DEFAULT (0x1u << 16) /**< \brief (HMATRIXB_SCFG) Last Default Master At the end of current slave access, if no other master request is pending, the slave stay connected with the last master havingaccessed it.This resusts in not having the one cycle latency when the last master re-trying access on the slave. */
#define   HMATRIXB_SCFG_DEFMSTR_TYPE_FIXED_DEFAULT (0x2u << 16) /**< \brief (HMATRIXB_SCFG) Fixed Default Master At the end of current slave access, if no other master request is pending, the slave connects with fixed master which numberis in FIXED_DEFMSTR register.This resusts in not having the one cycle latency when the fixed master re-trying access on the slave. */
#define HMATRIXB_SCFG_FIXED_DEFMSTR_Pos    18
#define HMATRIXB_SCFG_FIXED_DEFMSTR_Msk    (0xFu << HMATRIXB_SCFG_FIXED_DEFMSTR_Pos) /**< \brief (HMATRIXB_SCFG) Fixed Index of Default Master */
#define HMATRIXB_SCFG_FIXED_DEFMSTR(value) ((HMATRIXB_SCFG_FIXED_DEFMSTR_Msk & ((value) << HMATRIXB_SCFG_FIXED_DEFMSTR_Pos)))
#define HMATRIXB_SCFG_ARBT          (0x1u << 24) /**< \brief (HMATRIXB_SCFG) Arbitration Type */
#define   HMATRIXB_SCFG_ARBT_ROUND_ROBIN (0x0u << 24) /**< \brief (HMATRIXB_SCFG) Round-Robin Arbitration */
#define   HMATRIXB_SCFG_ARBT_FIXED_PRIORITY (0x1u << 24) /**< \brief (HMATRIXB_SCFG) Fixed Priority Arbitration */
#define HMATRIXB_SCFG_MASK          0x13F00FFu /**< \brief (HMATRIXB_SCFG) MASK Register */

/* -------- HMATRIXB_PRAS : (HMATRIXB Offset: 0x080) (R/W 32) Prs Priority Register A for Slave 0 -------- */
typedef union {
  struct {
    uint32_t M0PR:4;           /*!< bit:  0.. 3  Master 0 Priority                  */
    uint32_t M1PR:4;           /*!< bit:  4.. 7  Master 1 Priority                  */
    uint32_t M2PR:4;           /*!< bit:  8..11  Master 2 Priority                  */
    uint32_t M3PR:4;           /*!< bit: 12..15  Master 3 Priority                  */
    uint32_t M4PR:4;           /*!< bit: 16..19  Master 4 Priority                  */
    uint32_t M5PR:4;           /*!< bit: 20..23  Master 5 Priority                  */
    uint32_t M6PR:4;           /*!< bit: 24..27  Master 6 Priority                  */
    uint32_t M7PR:4;           /*!< bit: 28..31  Master 7 Priority                  */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_PRAS_Type;

#define HMATRIXB_PRAS_OFFSET        0x080        /**< \brief (HMATRIXB offset) Priority Register A for Slave 0 */
#define HMATRIXB_PRAS_RESETVALUE    0x0          /**< \brief (HMATRIXB reset_value) Priority Register A for Slave 0 */

#define HMATRIXB_PRAS_M0PR_Pos      0
#define HMATRIXB_PRAS_M0PR_Msk      (0xFu << HMATRIXB_PRAS_M0PR_Pos) /**< \brief (HMATRIXB_PRAS) Master 0 Priority */
#define HMATRIXB_PRAS_M0PR(value)   ((HMATRIXB_PRAS_M0PR_Msk & ((value) << HMATRIXB_PRAS_M0PR_Pos)))
#define HMATRIXB_PRAS_M1PR_Pos      4
#define HMATRIXB_PRAS_M1PR_Msk      (0xFu << HMATRIXB_PRAS_M1PR_Pos) /**< \brief (HMATRIXB_PRAS) Master 1 Priority */
#define HMATRIXB_PRAS_M1PR(value)   ((HMATRIXB_PRAS_M1PR_Msk & ((value) << HMATRIXB_PRAS_M1PR_Pos)))
#define HMATRIXB_PRAS_M2PR_Pos      8
#define HMATRIXB_PRAS_M2PR_Msk      (0xFu << HMATRIXB_PRAS_M2PR_Pos) /**< \brief (HMATRIXB_PRAS) Master 2 Priority */
#define HMATRIXB_PRAS_M2PR(value)   ((HMATRIXB_PRAS_M2PR_Msk & ((value) << HMATRIXB_PRAS_M2PR_Pos)))
#define HMATRIXB_PRAS_M3PR_Pos      12
#define HMATRIXB_PRAS_M3PR_Msk      (0xFu << HMATRIXB_PRAS_M3PR_Pos) /**< \brief (HMATRIXB_PRAS) Master 3 Priority */
#define HMATRIXB_PRAS_M3PR(value)   ((HMATRIXB_PRAS_M3PR_Msk & ((value) << HMATRIXB_PRAS_M3PR_Pos)))
#define HMATRIXB_PRAS_M4PR_Pos      16
#define HMATRIXB_PRAS_M4PR_Msk      (0xFu << HMATRIXB_PRAS_M4PR_Pos) /**< \brief (HMATRIXB_PRAS) Master 4 Priority */
#define HMATRIXB_PRAS_M4PR(value)   ((HMATRIXB_PRAS_M4PR_Msk & ((value) << HMATRIXB_PRAS_M4PR_Pos)))
#define HMATRIXB_PRAS_M5PR_Pos      20
#define HMATRIXB_PRAS_M5PR_Msk      (0xFu << HMATRIXB_PRAS_M5PR_Pos) /**< \brief (HMATRIXB_PRAS) Master 5 Priority */
#define HMATRIXB_PRAS_M5PR(value)   ((HMATRIXB_PRAS_M5PR_Msk & ((value) << HMATRIXB_PRAS_M5PR_Pos)))
#define HMATRIXB_PRAS_M6PR_Pos      24
#define HMATRIXB_PRAS_M6PR_Msk      (0xFu << HMATRIXB_PRAS_M6PR_Pos) /**< \brief (HMATRIXB_PRAS) Master 6 Priority */
#define HMATRIXB_PRAS_M6PR(value)   ((HMATRIXB_PRAS_M6PR_Msk & ((value) << HMATRIXB_PRAS_M6PR_Pos)))
#define HMATRIXB_PRAS_M7PR_Pos      28
#define HMATRIXB_PRAS_M7PR_Msk      (0xFu << HMATRIXB_PRAS_M7PR_Pos) /**< \brief (HMATRIXB_PRAS) Master 7 Priority */
#define HMATRIXB_PRAS_M7PR(value)   ((HMATRIXB_PRAS_M7PR_Msk & ((value) << HMATRIXB_PRAS_M7PR_Pos)))
#define HMATRIXB_PRAS_MASK          0xFFFFFFFFu /**< \brief (HMATRIXB_PRAS) MASK Register */

/* -------- HMATRIXB_PRBS : (HMATRIXB Offset: 0x084) (R/W 32) Prs Priority Register B for Slave 0 -------- */
typedef union {
  struct {
    uint32_t M8PR:4;           /*!< bit:  0.. 3  Master 8 Priority                  */
    uint32_t M9PR:4;           /*!< bit:  4.. 7  Master 9 Priority                  */
    uint32_t M10PR:4;          /*!< bit:  8..11  Master 10 Priority                 */
    uint32_t M11PR:4;          /*!< bit: 12..15  Master 11 Priority                 */
    uint32_t M12PR:4;          /*!< bit: 16..19  Master 12 Priority                 */
    uint32_t M13PR:4;          /*!< bit: 20..23  Master 13 Priority                 */
    uint32_t M14PR:4;          /*!< bit: 24..27  Master 14 Priority                 */
    uint32_t M15PR:4;          /*!< bit: 28..31  Master 15 Priority                 */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_PRBS_Type;

#define HMATRIXB_PRBS_OFFSET        0x084        /**< \brief (HMATRIXB offset) Priority Register B for Slave 0 */
#define HMATRIXB_PRBS_RESETVALUE    0x0          /**< \brief (HMATRIXB reset_value) Priority Register B for Slave 0 */

#define HMATRIXB_PRBS_M8PR_Pos      0
#define HMATRIXB_PRBS_M8PR_Msk      (0xFu << HMATRIXB_PRBS_M8PR_Pos) /**< \brief (HMATRIXB_PRBS) Master 8 Priority */
#define HMATRIXB_PRBS_M8PR(value)   ((HMATRIXB_PRBS_M8PR_Msk & ((value) << HMATRIXB_PRBS_M8PR_Pos)))
#define HMATRIXB_PRBS_M9PR_Pos      4
#define HMATRIXB_PRBS_M9PR_Msk      (0xFu << HMATRIXB_PRBS_M9PR_Pos) /**< \brief (HMATRIXB_PRBS) Master 9 Priority */
#define HMATRIXB_PRBS_M9PR(value)   ((HMATRIXB_PRBS_M9PR_Msk & ((value) << HMATRIXB_PRBS_M9PR_Pos)))
#define HMATRIXB_PRBS_M10PR_Pos     8
#define HMATRIXB_PRBS_M10PR_Msk     (0xFu << HMATRIXB_PRBS_M10PR_Pos) /**< \brief (HMATRIXB_PRBS) Master 10 Priority */
#define HMATRIXB_PRBS_M10PR(value)  ((HMATRIXB_PRBS_M10PR_Msk & ((value) << HMATRIXB_PRBS_M10PR_Pos)))
#define HMATRIXB_PRBS_M11PR_Pos     12
#define HMATRIXB_PRBS_M11PR_Msk     (0xFu << HMATRIXB_PRBS_M11PR_Pos) /**< \brief (HMATRIXB_PRBS) Master 11 Priority */
#define HMATRIXB_PRBS_M11PR(value)  ((HMATRIXB_PRBS_M11PR_Msk & ((value) << HMATRIXB_PRBS_M11PR_Pos)))
#define HMATRIXB_PRBS_M12PR_Pos     16
#define HMATRIXB_PRBS_M12PR_Msk     (0xFu << HMATRIXB_PRBS_M12PR_Pos) /**< \brief (HMATRIXB_PRBS) Master 12 Priority */
#define HMATRIXB_PRBS_M12PR(value)  ((HMATRIXB_PRBS_M12PR_Msk & ((value) << HMATRIXB_PRBS_M12PR_Pos)))
#define HMATRIXB_PRBS_M13PR_Pos     20
#define HMATRIXB_PRBS_M13PR_Msk     (0xFu << HMATRIXB_PRBS_M13PR_Pos) /**< \brief (HMATRIXB_PRBS) Master 13 Priority */
#define HMATRIXB_PRBS_M13PR(value)  ((HMATRIXB_PRBS_M13PR_Msk & ((value) << HMATRIXB_PRBS_M13PR_Pos)))
#define HMATRIXB_PRBS_M14PR_Pos     24
#define HMATRIXB_PRBS_M14PR_Msk     (0xFu << HMATRIXB_PRBS_M14PR_Pos) /**< \brief (HMATRIXB_PRBS) Master 14 Priority */
#define HMATRIXB_PRBS_M14PR(value)  ((HMATRIXB_PRBS_M14PR_Msk & ((value) << HMATRIXB_PRBS_M14PR_Pos)))
#define HMATRIXB_PRBS_M15PR_Pos     28
#define HMATRIXB_PRBS_M15PR_Msk     (0xFu << HMATRIXB_PRBS_M15PR_Pos) /**< \brief (HMATRIXB_PRBS) Master 15 Priority */
#define HMATRIXB_PRBS_M15PR(value)  ((HMATRIXB_PRBS_M15PR_Msk & ((value) << HMATRIXB_PRBS_M15PR_Pos)))
#define HMATRIXB_PRBS_MASK          0xFFFFFFFFu /**< \brief (HMATRIXB_PRBS) MASK Register */

/* -------- HMATRIXB_MRCR : (HMATRIXB Offset: 0x100) (R/W 32) Master Remap Control Register -------- */
typedef union {
  struct {
    uint32_t RCB0:1;           /*!< bit:      0  Remap Command bit for Master 0     */
    uint32_t RCB1:1;           /*!< bit:      1  Remap Command bit for Master 1     */
    uint32_t RCB2:1;           /*!< bit:      2  Remap Command bit for Master 2     */
    uint32_t RCB3:1;           /*!< bit:      3  Remap Command bit for Master 3     */
    uint32_t RCB4:1;           /*!< bit:      4  Remap Command bit for Master 4     */
    uint32_t RCB5:1;           /*!< bit:      5  Remap Command bit for Master 5     */
    uint32_t RCB6:1;           /*!< bit:      6  Remap Command bit for Master 6     */
    uint32_t RCB7:1;           /*!< bit:      7  Remap Command bit for Master 7     */
    uint32_t RCB8:1;           /*!< bit:      8  Remap Command bit for Master 8     */
    uint32_t RCB9:1;           /*!< bit:      9  Remap Command bit for Master 9     */
    uint32_t RCB10:1;          /*!< bit:     10  Remap Command bit for Master 10    */
    uint32_t RCB11:1;          /*!< bit:     11  Remap Command bit for Master 11    */
    uint32_t RCB12:1;          /*!< bit:     12  Remap Command bit for Master 12    */
    uint32_t RCB13:1;          /*!< bit:     13  Remap Command bit for Master 13    */
    uint32_t RCB14:1;          /*!< bit:     14  Remap Command bit for Master 14    */
    uint32_t RCB15:1;          /*!< bit:     15  Remap Command bit for Master 15    */
    uint32_t _reserved0:16;    /*!< bit: 16..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_MRCR_Type;

#define HMATRIXB_MRCR_OFFSET        0x100        /**< \brief (HMATRIXB offset) Master Remap Control Register */
#define HMATRIXB_MRCR_RESETVALUE    0x0          /**< \brief (HMATRIXB reset_value) Master Remap Control Register */

#define HMATRIXB_MRCR_RCB0          (0x1u <<  0) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 0 */
#define   HMATRIXB_MRCR_RCB0_0      (0x0u <<  0) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB0_1      (0x1u <<  0) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB1          (0x1u <<  1) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 1 */
#define   HMATRIXB_MRCR_RCB1_0      (0x0u <<  1) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB1_1      (0x1u <<  1) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB2          (0x1u <<  2) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 2 */
#define   HMATRIXB_MRCR_RCB2_0      (0x0u <<  2) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB2_1      (0x1u <<  2) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB3          (0x1u <<  3) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 3 */
#define   HMATRIXB_MRCR_RCB3_0      (0x0u <<  3) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB3_1      (0x1u <<  3) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB4          (0x1u <<  4) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 4 */
#define   HMATRIXB_MRCR_RCB4_0      (0x0u <<  4) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB4_1      (0x1u <<  4) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB5          (0x1u <<  5) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 5 */
#define   HMATRIXB_MRCR_RCB5_0      (0x0u <<  5) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB5_1      (0x1u <<  5) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB6          (0x1u <<  6) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 6 */
#define   HMATRIXB_MRCR_RCB6_0      (0x0u <<  6) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB6_1      (0x1u <<  6) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB7          (0x1u <<  7) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 7 */
#define   HMATRIXB_MRCR_RCB7_0      (0x0u <<  7) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB7_1      (0x1u <<  7) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB8          (0x1u <<  8) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 8 */
#define   HMATRIXB_MRCR_RCB8_0      (0x0u <<  8) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB8_1      (0x1u <<  8) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB9          (0x1u <<  9) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 9 */
#define   HMATRIXB_MRCR_RCB9_0      (0x0u <<  9) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB9_1      (0x1u <<  9) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB10         (0x1u << 10) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 10 */
#define   HMATRIXB_MRCR_RCB10_0     (0x0u << 10) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB10_1     (0x1u << 10) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB11         (0x1u << 11) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 11 */
#define   HMATRIXB_MRCR_RCB11_0     (0x0u << 11) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB11_1     (0x1u << 11) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB12         (0x1u << 12) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 12 */
#define   HMATRIXB_MRCR_RCB12_0     (0x0u << 12) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB12_1     (0x1u << 12) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB13         (0x1u << 13) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 13 */
#define   HMATRIXB_MRCR_RCB13_0     (0x0u << 13) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB13_1     (0x1u << 13) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB14         (0x1u << 14) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 14 */
#define   HMATRIXB_MRCR_RCB14_0     (0x0u << 14) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB14_1     (0x1u << 14) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_RCB15         (0x1u << 15) /**< \brief (HMATRIXB_MRCR) Remap Command bit for Master 15 */
#define   HMATRIXB_MRCR_RCB15_0     (0x0u << 15) /**< \brief (HMATRIXB_MRCR) Disable remapped address decoding for master */
#define   HMATRIXB_MRCR_RCB15_1     (0x1u << 15) /**< \brief (HMATRIXB_MRCR) Enable remapped address decoding for master */
#define HMATRIXB_MRCR_MASK          0xFFFFu /**< \brief (HMATRIXB_MRCR) MASK Register */

/* -------- HMATRIXB_SFR : (HMATRIXB Offset: 0x110) (R/W 32) Sfr Special Function Register 0 -------- */
typedef union {
  struct {
    uint32_t SFR:32;           /*!< bit:  0..31  Special Function Register          */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_SFR_Type;

#define HMATRIXB_SFR_OFFSET         0x110        /**< \brief (HMATRIXB offset) Special Function Register 0 */
#define HMATRIXB_SFR_RESETVALUE     0x0          /**< \brief (HMATRIXB reset_value) Special Function Register 0 */

#define HMATRIXB_SFR_SFR_Pos        0
#define HMATRIXB_SFR_SFR_Msk        (0xFFFFFFFFu << HMATRIXB_SFR_SFR_Pos) /**< \brief (HMATRIXB_SFR) Special Function Register */
#define HMATRIXB_SFR_SFR(value)     ((HMATRIXB_SFR_SFR_Msk & ((value) << HMATRIXB_SFR_SFR_Pos)))
#define HMATRIXB_SFR_MASK           0xFFFFFFFFu /**< \brief (HMATRIXB_SFR) MASK Register */

/** \brief HmatrixbPrs hardware registers */
typedef struct {
  __IO HMATRIXB_PRAS_Type        PRAS;        /**< \brief Offset: 0x000 (R/W 32) Priority Register A for Slave 0 */
  __IO HMATRIXB_PRBS_Type        PRBS;        /**< \brief Offset: 0x004 (R/W 32) Priority Register B for Slave 0 */
} HmatrixbPrs;

/** \brief HMATRIXB hardware registers */
typedef struct {
  __IO HMATRIXB_MCFG_Type        MCFG[16];    /**< \brief Offset: 0x000 (R/W 32) Master Configuration Register 0 */
  __IO HMATRIXB_SCFG_Type        SCFG[16];    /**< \brief Offset: 0x040 (R/W 32) Slave Configuration Register 0 */
       HmatrixbPrs               Prs[16];     /**< \brief Offset: 0x080 HmatrixbPrs groups */
  __IO HMATRIXB_MRCR_Type        MRCR;        /**< \brief Offset: 0x100 (R/W 32) Master Remap Control Register */
       RoReg8                    Reserved1[0xC];
  __IO HMATRIXB_SFR_Type         SFR[16];     /**< \brief Offset: 0x110 (R/W 32) Special Function Register 0 */
} Hmatrixb;

#endif /*  __EXCLUDE_HMATRIXB_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR NVMCTRL */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_NVMCTRL NVM Controller */
/*@{*/

#define REV_NVMCTRL    0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_NVMCTRL_FROM_CMSIS__

/* -------- NVMCTRL_CTRLA : (NVMCTRL Offset: 0x00) (R/W 32) NVM Control A Register -------- */
typedef union {
  struct {
    uint32_t CMD:7;            /*!< bit:  0.. 6                                     */
    uint32_t _reserved0:1;     /*!< bit:      7  Reserved                           */
    uint32_t CMDEX:8;          /*!< bit:  8..15                                     */
    uint32_t _reserved1:16;    /*!< bit: 16..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} NVMCTRL_CTRLA_Type;

#define NVMCTRL_CTRLA_OFFSET        0x00         /**< \brief (NVMCTRL offset) NVM Control A Register */
#define NVMCTRL_CTRLA_RESETVALUE    0x0          /**< \brief (NVMCTRL reset_value) NVM Control A Register */

#define NVMCTRL_CTRLA_CMD_Pos       0
#define NVMCTRL_CTRLA_CMD_Msk       (0x7Fu << NVMCTRL_CTRLA_CMD_Pos) /**< \brief (NVMCTRL_CTRLA)  */
#define NVMCTRL_CTRLA_CMD(value)    ((NVMCTRL_CTRLA_CMD_Msk & ((value) << NVMCTRL_CTRLA_CMD_Pos)))
#define   NVMCTRL_CTRLA_CMD_ER      (0x2u <<  0) /**< \brief (NVMCTRL_CTRLA) Erase Row */
#define   NVMCTRL_CTRLA_CMD_WP      (0x4u <<  0) /**< \brief (NVMCTRL_CTRLA) Write Page */
#define   NVMCTRL_CTRLA_CMD_EAR     (0x5u <<  0) /**< \brief (NVMCTRL_CTRLA) Erase Auxiliary Row */
#define   NVMCTRL_CTRLA_CMD_WAP     (0x6u <<  0) /**< \brief (NVMCTRL_CTRLA) Write  Auxiliary Row */
#define   NVMCTRL_CTRLA_CMD_SF      (0xAu <<  0) /**< \brief (NVMCTRL_CTRLA) Security Flow Command */
#define   NVMCTRL_CTRLA_CMD_WL      (0xFu <<  0) /**< \brief (NVMCTRL_CTRLA) Write lockbits */
#define   NVMCTRL_CTRLA_CMD_LR      (0x40u <<  0) /**< \brief (NVMCTRL_CTRLA) Lock Region */
#define   NVMCTRL_CTRLA_CMD_UR      (0x41u <<  0) /**< \brief (NVMCTRL_CTRLA) Unlock Region */
#define   NVMCTRL_CTRLA_CMD_SPRM    (0x42u <<  0) /**< \brief (NVMCTRL_CTRLA) Set Power Reduction Mode */
#define   NVMCTRL_CTRLA_CMD_CPRM    (0x43u <<  0) /**< \brief (NVMCTRL_CTRLA) Clear Power Reduction Mode */
#define   NVMCTRL_CTRLA_CMD_PBC     (0x44u <<  0) /**< \brief (NVMCTRL_CTRLA) Page Buffer Clear */
#define   NVMCTRL_CTRLA_CMD_SSB     (0x45u <<  0) /**< \brief (NVMCTRL_CTRLA) Set Security Bit */
#define   NVMCTRL_CTRLA_CMD_SMR     (0x54u <<  0) /**< \brief (NVMCTRL_CTRLA) State Machine Reset */
#define NVMCTRL_CTRLA_CMDEX_Pos     8
#define NVMCTRL_CTRLA_CMDEX_Msk     (0xFFu << NVMCTRL_CTRLA_CMDEX_Pos) /**< \brief (NVMCTRL_CTRLA)  */
#define NVMCTRL_CTRLA_CMDEX(value)  ((NVMCTRL_CTRLA_CMDEX_Msk & ((value) << NVMCTRL_CTRLA_CMDEX_Pos)))
#define NVMCTRL_CTRLA_MASK          0xFF7Fu /**< \brief (NVMCTRL_CTRLA) MASK Register */

/* -------- NVMCTRL_CTRLB : (NVMCTRL Offset: 0x04) (R/W 32) NVM Control B Register -------- */
typedef union {
  struct {
    uint32_t AUTOWS:1;         /*!< bit:      0                                     */
    uint32_t RWS:4;            /*!< bit:  1.. 4                                     */
    uint32_t _reserved0:2;     /*!< bit:  5.. 6  Reserved                           */
    uint32_t MANW:1;           /*!< bit:      7                                     */
    uint32_t SLEEPPRM:2;       /*!< bit:  8.. 9                                     */
    uint32_t _reserved1:6;     /*!< bit: 10..15  Reserved                           */
    uint32_t READMODE:2;       /*!< bit: 16..17                                     */
    uint32_t _reserved2:14;    /*!< bit: 18..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} NVMCTRL_CTRLB_Type;

#define NVMCTRL_CTRLB_OFFSET        0x04         /**< \brief (NVMCTRL offset) NVM Control B Register */
#define NVMCTRL_CTRLB_RESETVALUE    0x0          /**< \brief (NVMCTRL reset_value) NVM Control B Register */

#define NVMCTRL_CTRLB_AUTOWS        (0x1u <<  0) /**< \brief (NVMCTRL_CTRLB)  */
#define NVMCTRL_CTRLB_RWS_Pos       1
#define NVMCTRL_CTRLB_RWS_Msk       (0xFu << NVMCTRL_CTRLB_RWS_Pos) /**< \brief (NVMCTRL_CTRLB)  */
#define NVMCTRL_CTRLB_RWS(value)    ((NVMCTRL_CTRLB_RWS_Msk & ((value) << NVMCTRL_CTRLB_RWS_Pos)))
#define NVMCTRL_CTRLB_MANW          (0x1u <<  7) /**< \brief (NVMCTRL_CTRLB)  */
#define NVMCTRL_CTRLB_SLEEPPRM_Pos    8
#define NVMCTRL_CTRLB_SLEEPPRM_Msk    (0x3u << NVMCTRL_CTRLB_SLEEPPRM_Pos) /**< \brief (NVMCTRL_CTRLB)  */
#define NVMCTRL_CTRLB_SLEEPPRM(value) ((NVMCTRL_CTRLB_SLEEPPRM_Msk & ((value) << NVMCTRL_CTRLB_SLEEPPRM_Pos)))
#define   NVMCTRL_CTRLB_SLEEPPRM_WAKEONACCESS (0x0u <<  8) /**< \brief (NVMCTRL_CTRLB) Wake on first access. */
#define   NVMCTRL_CTRLB_SLEEPPRM_WAKEUPINSTANT (0x1u <<  8) /**< \brief (NVMCTRL_CTRLB) Wake on sleep exit. */
#define   NVMCTRL_CTRLB_SLEEPPRM_DISABLED (0x3u <<  8) /**< \brief (NVMCTRL_CTRLB) Auto power reduction disabled. */
#define NVMCTRL_CTRLB_READMODE_Pos    16
#define NVMCTRL_CTRLB_READMODE_Msk    (0x3u << NVMCTRL_CTRLB_READMODE_Pos) /**< \brief (NVMCTRL_CTRLB)  */
#define NVMCTRL_CTRLB_READMODE(value) ((NVMCTRL_CTRLB_READMODE_Msk & ((value) << NVMCTRL_CTRLB_READMODE_Pos)))
#define NVMCTRL_CTRLB_MASK          0x3039Fu /**< \brief (NVMCTRL_CTRLB) MASK Register */

/* -------- NVMCTRL_PARAM : (NVMCTRL Offset: 0x08) (R/W 32) Parameter Register -------- */
typedef union {
  struct {
    uint32_t NVMP:16;          /*!< bit:  0..15                                     */
    uint32_t PSZ:3;            /*!< bit: 16..18                                     */
    uint32_t _reserved0:13;    /*!< bit: 19..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} NVMCTRL_PARAM_Type;

#define NVMCTRL_PARAM_OFFSET        0x08         /**< \brief (NVMCTRL offset) Parameter Register */
#define NVMCTRL_PARAM_RESETVALUE    0x0          /**< \brief (NVMCTRL reset_value) Parameter Register */

#define NVMCTRL_PARAM_NVMP_Pos      0
#define NVMCTRL_PARAM_NVMP_Msk      (0xFFFFu << NVMCTRL_PARAM_NVMP_Pos) /**< \brief (NVMCTRL_PARAM)  */
#define NVMCTRL_PARAM_NVMP(value)   ((NVMCTRL_PARAM_NVMP_Msk & ((value) << NVMCTRL_PARAM_NVMP_Pos)))
#define NVMCTRL_PARAM_PSZ_Pos       16
#define NVMCTRL_PARAM_PSZ_Msk       (0x7u << NVMCTRL_PARAM_PSZ_Pos) /**< \brief (NVMCTRL_PARAM)  */
#define NVMCTRL_PARAM_PSZ(value)    ((NVMCTRL_PARAM_PSZ_Msk & ((value) << NVMCTRL_PARAM_PSZ_Pos)))
#define NVMCTRL_PARAM_MASK          0x7FFFFu /**< \brief (NVMCTRL_PARAM) MASK Register */

/* -------- NVMCTRL_INTENCLR : (NVMCTRL Offset: 0x0C) (R/W  8) Interrupt Enable Clear Register -------- */
typedef union {
  struct {
    uint8_t  READY:1;          /*!< bit:      0                                     */
    uint8_t  ERROR:1;          /*!< bit:      1                                     */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} NVMCTRL_INTENCLR_Type;

#define NVMCTRL_INTENCLR_OFFSET     0x0C         /**< \brief (NVMCTRL offset) Interrupt Enable Clear Register */
#define NVMCTRL_INTENCLR_RESETVALUE 0x0          /**< \brief (NVMCTRL reset_value) Interrupt Enable Clear Register */

#define NVMCTRL_INTENCLR_READY      (0x1u <<  0) /**< \brief (NVMCTRL_INTENCLR)  */
#define NVMCTRL_INTENCLR_ERROR      (0x1u <<  1) /**< \brief (NVMCTRL_INTENCLR)  */
#define NVMCTRL_INTENCLR_MASK       0x3u /**< \brief (NVMCTRL_INTENCLR) MASK Register */

/* -------- NVMCTRL_INTENSET : (NVMCTRL Offset: 0x10) (R/W  8) Interrupt Enable Set Register -------- */
typedef union {
  struct {
    uint8_t  READY:1;          /*!< bit:      0                                     */
    uint8_t  ERROR:1;          /*!< bit:      1                                     */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} NVMCTRL_INTENSET_Type;

#define NVMCTRL_INTENSET_OFFSET     0x10         /**< \brief (NVMCTRL offset) Interrupt Enable Set Register */
#define NVMCTRL_INTENSET_RESETVALUE 0x0          /**< \brief (NVMCTRL reset_value) Interrupt Enable Set Register */

#define NVMCTRL_INTENSET_READY      (0x1u <<  0) /**< \brief (NVMCTRL_INTENSET)  */
#define NVMCTRL_INTENSET_ERROR      (0x1u <<  1) /**< \brief (NVMCTRL_INTENSET)  */
#define NVMCTRL_INTENSET_MASK       0x3u /**< \brief (NVMCTRL_INTENSET) MASK Register */

/* -------- NVMCTRL_INTFLAG : (NVMCTRL Offset: 0x14) (R/W  8) Interrupt Register -------- */
typedef union {
  struct {
    uint8_t  READY:1;          /*!< bit:      0                                     */
    uint8_t  ERROR:1;          /*!< bit:      1                                     */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} NVMCTRL_INTFLAG_Type;

#define NVMCTRL_INTFLAG_OFFSET      0x14         /**< \brief (NVMCTRL offset) Interrupt Register */
#define NVMCTRL_INTFLAG_RESETVALUE  0x0          /**< \brief (NVMCTRL reset_value) Interrupt Register */

#define NVMCTRL_INTFLAG_READY       (0x1u <<  0) /**< \brief (NVMCTRL_INTFLAG)  */
#define NVMCTRL_INTFLAG_ERROR       (0x1u <<  1) /**< \brief (NVMCTRL_INTFLAG)  */
#define NVMCTRL_INTFLAG_MASK        0x3u /**< \brief (NVMCTRL_INTFLAG) MASK Register */

/* -------- NVMCTRL_STATUS : (NVMCTRL Offset: 0x18) (R/W 32) Status Register -------- */
typedef union {
  struct {
    uint32_t PRM:1;            /*!< bit:      0                                     */
    uint32_t LOAD:1;           /*!< bit:      1                                     */
    uint32_t PROGE:1;          /*!< bit:      2                                     */
    uint32_t LOCKE:1;          /*!< bit:      3                                     */
    uint32_t NVME:1;           /*!< bit:      4                                     */
    uint32_t _reserved0:3;     /*!< bit:  5.. 7  Reserved                           */
    uint32_t SB:1;             /*!< bit:      8                                     */
    uint32_t _reserved1:23;    /*!< bit:  9..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} NVMCTRL_STATUS_Type;

#define NVMCTRL_STATUS_OFFSET       0x18         /**< \brief (NVMCTRL offset) Status Register */
#define NVMCTRL_STATUS_RESETVALUE   0x0          /**< \brief (NVMCTRL reset_value) Status Register */

#define NVMCTRL_STATUS_PRM          (0x1u <<  0) /**< \brief (NVMCTRL_STATUS)  */
#define NVMCTRL_STATUS_LOAD         (0x1u <<  1) /**< \brief (NVMCTRL_STATUS)  */
#define NVMCTRL_STATUS_PROGE        (0x1u <<  2) /**< \brief (NVMCTRL_STATUS)  */
#define NVMCTRL_STATUS_LOCKE        (0x1u <<  3) /**< \brief (NVMCTRL_STATUS)  */
#define NVMCTRL_STATUS_NVME         (0x1u <<  4) /**< \brief (NVMCTRL_STATUS)  */
#define NVMCTRL_STATUS_SB           (0x1u <<  8) /**< \brief (NVMCTRL_STATUS)  */
#define NVMCTRL_STATUS_MASK         0x11Fu /**< \brief (NVMCTRL_STATUS) MASK Register */

/* -------- NVMCTRL_ADDR : (NVMCTRL Offset: 0x1C) (R/W 32) Address Register -------- */
typedef union {
  struct {
    uint32_t ADDR:22;          /*!< bit:  0..21                                     */
    uint32_t _reserved0:10;    /*!< bit: 22..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} NVMCTRL_ADDR_Type;

#define NVMCTRL_ADDR_OFFSET         0x1C         /**< \brief (NVMCTRL offset) Address Register */
#define NVMCTRL_ADDR_RESETVALUE     0x0          /**< \brief (NVMCTRL reset_value) Address Register */

#define NVMCTRL_ADDR_ADDR_Pos       0
#define NVMCTRL_ADDR_ADDR_Msk       (0x3FFFFFu << NVMCTRL_ADDR_ADDR_Pos) /**< \brief (NVMCTRL_ADDR)  */
#define NVMCTRL_ADDR_ADDR(value)    ((NVMCTRL_ADDR_ADDR_Msk & ((value) << NVMCTRL_ADDR_ADDR_Pos)))
#define NVMCTRL_ADDR_MASK           0x3FFFFFu /**< \brief (NVMCTRL_ADDR) MASK Register */

/* -------- NVMCTRL_LOCK : (NVMCTRL Offset: 0x20) (R/W 32) Lock Register -------- */
typedef union {
  struct {
    uint32_t LOCK:16;          /*!< bit:  0..15                                     */
    uint32_t _reserved0:16;    /*!< bit: 16..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} NVMCTRL_LOCK_Type;

#define NVMCTRL_LOCK_OFFSET         0x20         /**< \brief (NVMCTRL offset) Lock Register */
#define NVMCTRL_LOCK_RESETVALUE     0x0          /**< \brief (NVMCTRL reset_value) Lock Register */

#define NVMCTRL_LOCK_LOCK_Pos       0
#define NVMCTRL_LOCK_LOCK_Msk       (0xFFFFu << NVMCTRL_LOCK_LOCK_Pos) /**< \brief (NVMCTRL_LOCK)  */
#define NVMCTRL_LOCK_LOCK(value)    ((NVMCTRL_LOCK_LOCK_Msk & ((value) << NVMCTRL_LOCK_LOCK_Pos)))
#define NVMCTRL_LOCK_MASK           0xFFFFu /**< \brief (NVMCTRL_LOCK) MASK Register */

/** \brief NVMCTRL hardware registers */
typedef struct {
  __IO NVMCTRL_CTRLA_Type        CTRLA;       /**< \brief Offset: 0x00 (R/W 32) NVM Control A Register */
  __IO NVMCTRL_CTRLB_Type        CTRLB;       /**< \brief Offset: 0x04 (R/W 32) NVM Control B Register */
  __IO NVMCTRL_PARAM_Type        PARAM;       /**< \brief Offset: 0x08 (R/W 32) Parameter Register */
  __IO NVMCTRL_INTENCLR_Type     INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear Register */
       RoReg8                    Reserved1[0x3];
  __IO NVMCTRL_INTENSET_Type     INTENSET;    /**< \brief Offset: 0x10 (R/W  8) Interrupt Enable Set Register */
       RoReg8                    Reserved2[0x3];
  __IO NVMCTRL_INTFLAG_Type      INTFLAG;     /**< \brief Offset: 0x14 (R/W  8) Interrupt Register */
       RoReg8                    Reserved3[0x3];
  __IO NVMCTRL_STATUS_Type       STATUS;      /**< \brief Offset: 0x18 (R/W 32) Status Register */
  __IO NVMCTRL_ADDR_Type         ADDR;        /**< \brief Offset: 0x1C (R/W 32) Address Register */
  __IO NVMCTRL_LOCK_Type         LOCK;        /**< \brief Offset: 0x20 (R/W 32) Lock Register */
} Nvmctrl;

#endif /*  __EXCLUDE_NVMCTRL_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PAC */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_PAC Peripheral Access Controller */
/*@{*/

#define REV_PAC        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_PAC_FROM_CMSIS__

/* -------- PAC_WPCLR : (PAC Offset: 0x0) (R/W 32) Write Protection Clear -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PAC_WPCLR_Type;

#define PAC_WPCLR_OFFSET            0x0          /**< \brief (PAC offset) Write Protection Clear */
#define PAC_WPCLR_RESETVALUE        0x0          /**< \brief (PAC reset_value) Write Protection Clear */
#define PAC_WPCLR_MASK              0xFFFFFFFFu  /**< \brief (PAC_WPCLR) MASK Register */

/* -------- PAC_WPSET : (PAC Offset: 0x4) (R/W 32) Write Protection Set -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PAC_WPSET_Type;

#define PAC_WPSET_OFFSET            0x4          /**< \brief (PAC offset) Write Protection Set */
#define PAC_WPSET_RESETVALUE        0x0          /**< \brief (PAC reset_value) Write Protection Set */
#define PAC_WPSET_MASK              0xFFFFFFFFu  /**< \brief (PAC_WPSET) MASK Register */

/** \brief PAC hardware registers */
typedef struct {
  __IO PAC_WPCLR_Type            WPCLR;       /**< \brief Offset: 0x0 (R/W 32) Write Protection Clear */
  __IO PAC_WPSET_Type            WPSET;       /**< \brief Offset: 0x4 (R/W 32) Write Protection Set */
} Pac;

#endif /*  __EXCLUDE_PAC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PM */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_PM Power Manager */
/*@{*/

#define REV_PM         0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_PM_FROM_CMSIS__

/* -------- PM_CTRL : (PM Offset: 0x00) (R/W  8) Control Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:2;     /*!< bit:  0.. 1  Reserved                           */
    uint8_t  CFDEN:1;          /*!< bit:      2  Clock Failure Detector Enable      */
    uint8_t  _reserved1:1;     /*!< bit:      3  Reserved                           */
    uint8_t  MCSEL:2;          /*!< bit:  4.. 5  Main Clock Select                  */
    uint8_t  _reserved2:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_CTRL_Type;

#define PM_CTRL_OFFSET              0x00         /**< \brief (PM offset) Control Register */
#define PM_CTRL_RESETVALUE          0x0          /**< \brief (PM reset_value) Control Register */

#define PM_CTRL_CFDEN               (0x1u <<  2) /**< \brief (PM_CTRL) Clock Failure Detector Enable */
#define PM_CTRL_MCSEL_Pos           4
#define PM_CTRL_MCSEL_Msk           (0x3u << PM_CTRL_MCSEL_Pos) /**< \brief (PM_CTRL) Main Clock Select */
#define PM_CTRL_MCSEL(value)        ((PM_CTRL_MCSEL_Msk & ((value) << PM_CTRL_MCSEL_Pos)))
#define PM_CTRL_MASK                0x34u /**< \brief (PM_CTRL) MASK Register */

/* -------- PM_SLEEP : (PM Offset: 0x01) (R/W  8) Sleep Register -------- */
typedef union {
  struct {
    uint8_t  IDLE:2;           /*!< bit:  0.. 1  Idle level                         */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_SLEEP_Type;

#define PM_SLEEP_OFFSET             0x01         /**< \brief (PM offset) Sleep Register */
#define PM_SLEEP_RESETVALUE         0x0          /**< \brief (PM reset_value) Sleep Register */

#define PM_SLEEP_IDLE_Pos           0
#define PM_SLEEP_IDLE_Msk           (0x3u << PM_SLEEP_IDLE_Pos) /**< \brief (PM_SLEEP) Idle level */
#define PM_SLEEP_IDLE(value)        ((PM_SLEEP_IDLE_Msk & ((value) << PM_SLEEP_IDLE_Pos)))
#define PM_SLEEP_MASK               0x3u /**< \brief (PM_SLEEP) MASK Register */

/* -------- PM_CPUSEL : (PM Offset: 0x08) (R/W  8) CPU Clock Select -------- */
typedef union {
  struct {
    uint8_t  CPUDIV:3;         /*!< bit:  0.. 2  CPU Clock Select                   */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_CPUSEL_Type;

#define PM_CPUSEL_OFFSET            0x08         /**< \brief (PM offset) CPU Clock Select */
#define PM_CPUSEL_RESETVALUE        0x0          /**< \brief (PM reset_value) CPU Clock Select */

#define PM_CPUSEL_CPUDIV_Pos        0
#define PM_CPUSEL_CPUDIV_Msk        (0x7u << PM_CPUSEL_CPUDIV_Pos) /**< \brief (PM_CPUSEL) CPU Clock Select */
#define PM_CPUSEL_CPUDIV(value)     ((PM_CPUSEL_CPUDIV_Msk & ((value) << PM_CPUSEL_CPUDIV_Pos)))
#define PM_CPUSEL_MASK              0x7u /**< \brief (PM_CPUSEL) MASK Register */

/* -------- PM_APBASEL : (PM Offset: 0x09) (R/W  8) APBA Clock Select -------- */
typedef union {
  struct {
    uint8_t  APBADIV:3;        /*!< bit:  0.. 2  APBA Clock Select                  */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_APBASEL_Type;

#define PM_APBASEL_OFFSET           0x09         /**< \brief (PM offset) APBA Clock Select */
#define PM_APBASEL_RESETVALUE       0x0          /**< \brief (PM reset_value) APBA Clock Select */

#define PM_APBASEL_APBADIV_Pos      0
#define PM_APBASEL_APBADIV_Msk      (0x7u << PM_APBASEL_APBADIV_Pos) /**< \brief (PM_APBASEL) APBA Clock Select */
#define PM_APBASEL_APBADIV(value)   ((PM_APBASEL_APBADIV_Msk & ((value) << PM_APBASEL_APBADIV_Pos)))
#define PM_APBASEL_MASK             0x7u /**< \brief (PM_APBASEL) MASK Register */

/* -------- PM_APBBSEL : (PM Offset: 0x0A) (R/W  8) APBB Clock Select -------- */
typedef union {
  struct {
    uint8_t  APBBDIV:3;        /*!< bit:  0.. 2  PB Clock Select                    */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_APBBSEL_Type;

#define PM_APBBSEL_OFFSET           0x0A         /**< \brief (PM offset) APBB Clock Select */
#define PM_APBBSEL_RESETVALUE       0x0          /**< \brief (PM reset_value) APBB Clock Select */

#define PM_APBBSEL_APBBDIV_Pos      0
#define PM_APBBSEL_APBBDIV_Msk      (0x7u << PM_APBBSEL_APBBDIV_Pos) /**< \brief (PM_APBBSEL) PB Clock Select */
#define PM_APBBSEL_APBBDIV(value)   ((PM_APBBSEL_APBBDIV_Msk & ((value) << PM_APBBSEL_APBBDIV_Pos)))
#define PM_APBBSEL_MASK             0x7u /**< \brief (PM_APBBSEL) MASK Register */

/* -------- PM_APBCSEL : (PM Offset: 0x0B) (R/W  8) APBC Clock Select -------- */
typedef union {
  struct {
    uint8_t  APBCDIV:3;        /*!< bit:  0.. 2  APBC Clock Select                  */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_APBCSEL_Type;

#define PM_APBCSEL_OFFSET           0x0B         /**< \brief (PM offset) APBC Clock Select */
#define PM_APBCSEL_RESETVALUE       0x0          /**< \brief (PM reset_value) APBC Clock Select */

#define PM_APBCSEL_APBCDIV_Pos      0
#define PM_APBCSEL_APBCDIV_Msk      (0x7u << PM_APBCSEL_APBCDIV_Pos) /**< \brief (PM_APBCSEL) APBC Clock Select */
#define PM_APBCSEL_APBCDIV(value)   ((PM_APBCSEL_APBCDIV_Msk & ((value) << PM_APBCSEL_APBCDIV_Pos)))
#define PM_APBCSEL_MASK             0x7u /**< \brief (PM_APBCSEL) MASK Register */

/* -------- PM_AHBMASK : (PM Offset: 0x14) (R/W 32) AHB Mask -------- */
typedef union {
  struct {
    uint32_t HPB0:1;           /*!< bit:      0  HPB0 AHB Clock Mask                */
    uint32_t HPB1:1;           /*!< bit:      1  HPB1 AHB Clock Mask                */
    uint32_t HPB2:1;           /*!< bit:      2  HPB2 AHB Clock Mask                */
    uint32_t DSU:1;            /*!< bit:      3  DSU AHB Clock Mask                 */
    uint32_t NVMCTRL:1;        /*!< bit:      4  NVMCTRL AHB Clock Mask             */
    uint32_t _reserved0:27;    /*!< bit:  5..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PM_AHBMASK_Type;

#define PM_AHBMASK_OFFSET           0x14         /**< \brief (PM offset) AHB Mask */
#define PM_AHBMASK_RESETVALUE       0x1F         /**< \brief (PM reset_value) AHB Mask */

#define PM_AHBMASK_HPB0             (0x1u <<  0) /**< \brief (PM_AHBMASK) HPB0 AHB Clock Mask */
#define PM_AHBMASK_HPB1             (0x1u <<  1) /**< \brief (PM_AHBMASK) HPB1 AHB Clock Mask */
#define PM_AHBMASK_HPB2             (0x1u <<  2) /**< \brief (PM_AHBMASK) HPB2 AHB Clock Mask */
#define PM_AHBMASK_DSU              (0x1u <<  3) /**< \brief (PM_AHBMASK) DSU AHB Clock Mask */
#define PM_AHBMASK_NVMCTRL          (0x1u <<  4) /**< \brief (PM_AHBMASK) NVMCTRL AHB Clock Mask */
#define PM_AHBMASK_MASK             0x1Fu /**< \brief (PM_AHBMASK) MASK Register */

/* -------- PM_APBAMASK : (PM Offset: 0x18) (R/W 32) APBA Mask -------- */
typedef union {
  struct {
    uint32_t PAC0:1;           /*!< bit:      0  PAC0 APB Clock Mask                */
    uint32_t PM:1;             /*!< bit:      1  PM APB Clock Mask                  */
    uint32_t SYSCTRL:1;        /*!< bit:      2  SYSCTRL APB Clock Mask             */
    uint32_t GCLK:1;           /*!< bit:      3  GCLK APB Clock Mask                */
    uint32_t WDT:1;            /*!< bit:      4  WDT APB Clock Mask                 */
    uint32_t RTC:1;            /*!< bit:      5  RTC APB Clock Mask                 */
    uint32_t EIC:1;            /*!< bit:      6  EIC APB Clock Mask                 */
    uint32_t _reserved0:25;    /*!< bit:  7..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PM_APBAMASK_Type;

#define PM_APBAMASK_OFFSET          0x18         /**< \brief (PM offset) APBA Mask */
#define PM_APBAMASK_RESETVALUE      0x7F         /**< \brief (PM reset_value) APBA Mask */

#define PM_APBAMASK_PAC0            (0x1u <<  0) /**< \brief (PM_APBAMASK) PAC0 APB Clock Mask */
#define PM_APBAMASK_PM              (0x1u <<  1) /**< \brief (PM_APBAMASK) PM APB Clock Mask */
#define PM_APBAMASK_SYSCTRL         (0x1u <<  2) /**< \brief (PM_APBAMASK) SYSCTRL APB Clock Mask */
#define PM_APBAMASK_GCLK            (0x1u <<  3) /**< \brief (PM_APBAMASK) GCLK APB Clock Mask */
#define PM_APBAMASK_WDT             (0x1u <<  4) /**< \brief (PM_APBAMASK) WDT APB Clock Mask */
#define PM_APBAMASK_RTC             (0x1u <<  5) /**< \brief (PM_APBAMASK) RTC APB Clock Mask */
#define PM_APBAMASK_EIC             (0x1u <<  6) /**< \brief (PM_APBAMASK) EIC APB Clock Mask */
#define PM_APBAMASK_MASK            0x7Fu /**< \brief (PM_APBAMASK) MASK Register */

/* -------- PM_APBBMASK : (PM Offset: 0x1C) (R/W 32) APBB Mask -------- */
typedef union {
  struct {
    uint32_t PAC1:1;           /*!< bit:      0  PAC1 APB Clock Mask                */
    uint32_t DSU:1;            /*!< bit:      1  DSU APB Clock Mask                 */
    uint32_t NVMCTRL:1;        /*!< bit:      2  NVMCTRL APB Clock Mask             */
    uint32_t PORT:1;           /*!< bit:      3  PORT APB Clock Mask                */
    uint32_t HMATRIX:1;        /*!< bit:      4  HMATRIX APB Clock Mask             */
    uint32_t _reserved0:27;    /*!< bit:  5..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PM_APBBMASK_Type;

#define PM_APBBMASK_OFFSET          0x1C         /**< \brief (PM offset) APBB Mask */
#define PM_APBBMASK_RESETVALUE      0x1F         /**< \brief (PM reset_value) APBB Mask */

#define PM_APBBMASK_PAC1            (0x1u <<  0) /**< \brief (PM_APBBMASK) PAC1 APB Clock Mask */
#define PM_APBBMASK_DSU             (0x1u <<  1) /**< \brief (PM_APBBMASK) DSU APB Clock Mask */
#define PM_APBBMASK_NVMCTRL         (0x1u <<  2) /**< \brief (PM_APBBMASK) NVMCTRL APB Clock Mask */
#define PM_APBBMASK_PORT            (0x1u <<  3) /**< \brief (PM_APBBMASK) PORT APB Clock Mask */
#define PM_APBBMASK_HMATRIX         (0x1u <<  4) /**< \brief (PM_APBBMASK) HMATRIX APB Clock Mask */
#define PM_APBBMASK_MASK            0x1Fu /**< \brief (PM_APBBMASK) MASK Register */

/* -------- PM_APBCMASK : (PM Offset: 0x20) (R/W 32) APBC Mask -------- */
typedef union {
  struct {
    uint32_t PAC2:1;           /*!< bit:      0  PAC2 APB Clock Mask                */
    uint32_t EVSYS:1;          /*!< bit:      1  EVSYS APB Clock Mask               */
    uint32_t SERCOM0:1;        /*!< bit:      2  SERCOM0 APB Clock Mask             */
    uint32_t SERCOM1:1;        /*!< bit:      3  SERCOM1 APB Clock Mask             */
    uint32_t SERCOM2:1;        /*!< bit:      4  SERCOM2 APB Clock Mask             */
    uint32_t SERCOM3:1;        /*!< bit:      5  SERCOM3 APB Clock Mask             */
    uint32_t SERCOM4:1;        /*!< bit:      6  SERCOM4 APB Clock Mask             */
    uint32_t SERCOM5:1;        /*!< bit:      7  SERCOM5 APB Clock Mask             */
    uint32_t TC0:1;            /*!< bit:      8  TC0 APB Clock Mask                 */
    uint32_t TC1:1;            /*!< bit:      9  TC1 APB Clock Mask                 */
    uint32_t TC2:1;            /*!< bit:     10  TC2 APB Clock Mask                 */
    uint32_t TC3:1;            /*!< bit:     11  TC3 APB Clock Mask                 */
    uint32_t TC4:1;            /*!< bit:     12  TC4 APB Clock Mask                 */
    uint32_t TC5:1;            /*!< bit:     13  TC5 APB Clock Mask                 */
    uint32_t TC6:1;            /*!< bit:     14  TC6 APB Clock Mask                 */
    uint32_t TC7:1;            /*!< bit:     15  TC7 APB Clock Mask                 */
    uint32_t ADC:1;            /*!< bit:     16  ADC APB Clock Mask                 */
    uint32_t AC:1;             /*!< bit:     17  AC APB Clock Mask                  */
    uint32_t DAC:1;            /*!< bit:     18  DAC APB Clock Mask                 */
    uint32_t PTC:1;            /*!< bit:     19  PTC APB Clock Mask                 */
    uint32_t ATW:1;            /*!< bit:     20  ATW APB Clock Mask                 */
    uint32_t _reserved0:11;    /*!< bit: 21..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PM_APBCMASK_Type;

#define PM_APBCMASK_OFFSET          0x20         /**< \brief (PM offset) APBC Mask */
#define PM_APBCMASK_RESETVALUE      0x10000      /**< \brief (PM reset_value) APBC Mask */

#define PM_APBCMASK_PAC2            (0x1u <<  0) /**< \brief (PM_APBCMASK) PAC2 APB Clock Mask */
#define PM_APBCMASK_EVSYS           (0x1u <<  1) /**< \brief (PM_APBCMASK) EVSYS APB Clock Mask */
#define PM_APBCMASK_SERCOM0         (0x1u <<  2) /**< \brief (PM_APBCMASK) SERCOM0 APB Clock Mask */
#define PM_APBCMASK_SERCOM1         (0x1u <<  3) /**< \brief (PM_APBCMASK) SERCOM1 APB Clock Mask */
#define PM_APBCMASK_SERCOM2         (0x1u <<  4) /**< \brief (PM_APBCMASK) SERCOM2 APB Clock Mask */
#define PM_APBCMASK_SERCOM3         (0x1u <<  5) /**< \brief (PM_APBCMASK) SERCOM3 APB Clock Mask */
#define PM_APBCMASK_SERCOM4         (0x1u <<  6) /**< \brief (PM_APBCMASK) SERCOM4 APB Clock Mask */
#define PM_APBCMASK_SERCOM5         (0x1u <<  7) /**< \brief (PM_APBCMASK) SERCOM5 APB Clock Mask */
#define PM_APBCMASK_TC0             (0x1u <<  8) /**< \brief (PM_APBCMASK) TC0 APB Clock Mask */
#define PM_APBCMASK_TC1             (0x1u <<  9) /**< \brief (PM_APBCMASK) TC1 APB Clock Mask */
#define PM_APBCMASK_TC2             (0x1u << 10) /**< \brief (PM_APBCMASK) TC2 APB Clock Mask */
#define PM_APBCMASK_TC3             (0x1u << 11) /**< \brief (PM_APBCMASK) TC3 APB Clock Mask */
#define PM_APBCMASK_TC4             (0x1u << 12) /**< \brief (PM_APBCMASK) TC4 APB Clock Mask */
#define PM_APBCMASK_TC5             (0x1u << 13) /**< \brief (PM_APBCMASK) TC5 APB Clock Mask */
#define PM_APBCMASK_TC6             (0x1u << 14) /**< \brief (PM_APBCMASK) TC6 APB Clock Mask */
#define PM_APBCMASK_TC7             (0x1u << 15) /**< \brief (PM_APBCMASK) TC7 APB Clock Mask */
#define PM_APBCMASK_ADC             (0x1u << 16) /**< \brief (PM_APBCMASK) ADC APB Clock Mask */
#define PM_APBCMASK_AC              (0x1u << 17) /**< \brief (PM_APBCMASK) AC APB Clock Mask */
#define PM_APBCMASK_DAC             (0x1u << 18) /**< \brief (PM_APBCMASK) DAC APB Clock Mask */
#define PM_APBCMASK_PTC             (0x1u << 19) /**< \brief (PM_APBCMASK) PTC APB Clock Mask */
#define PM_APBCMASK_ATW             (0x1u << 20) /**< \brief (PM_APBCMASK) ATW APB Clock Mask */
#define PM_APBCMASK_MASK            0x1FFFFFu /**< \brief (PM_APBCMASK) MASK Register */

/* -------- PM_INTENCLR : (PM Offset: 0x34) (R/W  8) Interrupt Enable Clear Register -------- */
typedef union {
  struct {
    uint8_t  CKRDY:1;          /*!< bit:      0  Clock Ready Interrupt Enable Clear */
    uint8_t  CFD:1;            /*!< bit:      1  Clock Failure Detector Enable Clear */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_INTENCLR_Type;

#define PM_INTENCLR_OFFSET          0x34         /**< \brief (PM offset) Interrupt Enable Clear Register */
#define PM_INTENCLR_RESETVALUE      0x0          /**< \brief (PM reset_value) Interrupt Enable Clear Register */

#define PM_INTENCLR_CKRDY           (0x1u <<  0) /**< \brief (PM_INTENCLR) Clock Ready Interrupt Enable Clear */
#define PM_INTENCLR_CFD             (0x1u <<  1) /**< \brief (PM_INTENCLR) Clock Failure Detector Enable Clear */
#define PM_INTENCLR_MASK            0x3u /**< \brief (PM_INTENCLR) MASK Register */

/* -------- PM_INTENSET : (PM Offset: 0x35) (R/W  8) Interrupt Enable Set Register -------- */
typedef union {
  struct {
    uint8_t  CKRDY:1;          /*!< bit:      0  Clock Ready Interrupt Enable Set   */
    uint8_t  CFD:1;            /*!< bit:      1  Clock Failure Detector Enable Set  */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_INTENSET_Type;

#define PM_INTENSET_OFFSET          0x35         /**< \brief (PM offset) Interrupt Enable Set Register */
#define PM_INTENSET_RESETVALUE      0x0          /**< \brief (PM reset_value) Interrupt Enable Set Register */

#define PM_INTENSET_CKRDY           (0x1u <<  0) /**< \brief (PM_INTENSET) Clock Ready Interrupt Enable Set */
#define PM_INTENSET_CFD             (0x1u <<  1) /**< \brief (PM_INTENSET) Clock Failure Detector Enable Set */
#define PM_INTENSET_MASK            0x3u /**< \brief (PM_INTENSET) MASK Register */

/* -------- PM_INTFLAG : (PM Offset: 0x36) (R/W  8) Interrupt Flag Status and Clear Register -------- */
typedef union {
  struct {
    uint8_t  CKRDY:1;          /*!< bit:      0  Clock Ready Interrupt              */
    uint8_t  CFD:1;            /*!< bit:      1  Clock Failure Detectore Interrupt  */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_INTFLAG_Type;

#define PM_INTFLAG_OFFSET           0x36         /**< \brief (PM offset) Interrupt Flag Status and Clear Register */
#define PM_INTFLAG_RESETVALUE       0x0          /**< \brief (PM reset_value) Interrupt Flag Status and Clear Register */

#define PM_INTFLAG_CKRDY            (0x1u <<  0) /**< \brief (PM_INTFLAG) Clock Ready Interrupt */
#define PM_INTFLAG_CFD              (0x1u <<  1) /**< \brief (PM_INTFLAG) Clock Failure Detectore Interrupt */
#define PM_INTFLAG_MASK             0x3u /**< \brief (PM_INTFLAG) MASK Register */

/* -------- PM_RCAUSE : (PM Offset: 0x38) (R/   8) Reset Cause Register -------- */
typedef union {
  struct {
    uint8_t  POR:1;            /*!< bit:      0  Power-on Reset                     */
    uint8_t  BOD12:1;          /*!< bit:      1  Brown-out 1.2V Reset               */
    uint8_t  BOD33:1;          /*!< bit:      2  Brown-out 3.3V Reset               */
    uint8_t  _reserved0:1;     /*!< bit:      3  Reserved                           */
    uint8_t  EXT:1;            /*!< bit:      4  External Reset Pin                 */
    uint8_t  WDT:1;            /*!< bit:      5  Watchdog Reset                     */
    uint8_t  SYST:1;           /*!< bit:      6  System Reset Request               */
    uint8_t  _reserved1:1;     /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_RCAUSE_Type;

#define PM_RCAUSE_OFFSET            0x38         /**< \brief (PM offset) Reset Cause Register */
#define PM_RCAUSE_RESETVALUE        0x1          /**< \brief (PM reset_value) Reset Cause Register */

#define PM_RCAUSE_POR               (0x1u <<  0) /**< \brief (PM_RCAUSE) Power-on Reset */
#define PM_RCAUSE_BOD12             (0x1u <<  1) /**< \brief (PM_RCAUSE) Brown-out 1.2V Reset */
#define PM_RCAUSE_BOD33             (0x1u <<  2) /**< \brief (PM_RCAUSE) Brown-out 3.3V Reset */
#define PM_RCAUSE_EXT               (0x1u <<  4) /**< \brief (PM_RCAUSE) External Reset Pin */
#define PM_RCAUSE_WDT               (0x1u <<  5) /**< \brief (PM_RCAUSE) Watchdog Reset */
#define PM_RCAUSE_SYST              (0x1u <<  6) /**< \brief (PM_RCAUSE) System Reset Request */
#define PM_RCAUSE_MASK              0x77u /**< \brief (PM_RCAUSE) MASK Register */

/** \brief PM hardware registers */
typedef struct {
  __IO PM_CTRL_Type              CTRL;        /**< \brief Offset: 0x00 (R/W  8) Control Register */
  __IO PM_SLEEP_Type             SLEEP;       /**< \brief Offset: 0x01 (R/W  8) Sleep Register */
       RoReg8                    Reserved1[0x6];
  __IO PM_CPUSEL_Type            CPUSEL;      /**< \brief Offset: 0x08 (R/W  8) CPU Clock Select */
  __IO PM_APBASEL_Type           APBASEL;     /**< \brief Offset: 0x09 (R/W  8) APBA Clock Select */
  __IO PM_APBBSEL_Type           APBBSEL;     /**< \brief Offset: 0x0A (R/W  8) APBB Clock Select */
  __IO PM_APBCSEL_Type           APBCSEL;     /**< \brief Offset: 0x0B (R/W  8) APBC Clock Select */
       RoReg8                    Reserved2[0x8];
  __IO PM_AHBMASK_Type           AHBMASK;     /**< \brief Offset: 0x14 (R/W 32) AHB Mask */
  __IO PM_APBAMASK_Type          APBAMASK;    /**< \brief Offset: 0x18 (R/W 32) APBA Mask */
  __IO PM_APBBMASK_Type          APBBMASK;    /**< \brief Offset: 0x1C (R/W 32) APBB Mask */
  __IO PM_APBCMASK_Type          APBCMASK;    /**< \brief Offset: 0x20 (R/W 32) APBC Mask */
       RoReg8                    Reserved3[0x10];
  __IO PM_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x34 (R/W  8) Interrupt Enable Clear Register */
  __IO PM_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x35 (R/W  8) Interrupt Enable Set Register */
  __IO PM_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x36 (R/W  8) Interrupt Flag Status and Clear Register */
       RoReg8                    Reserved4[0x1];
  __I  PM_RCAUSE_Type            RCAUSE;      /**< \brief Offset: 0x38 (R/   8) Reset Cause Register */
} Pm;

#endif /*  __EXCLUDE_PM_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PORT */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_PORT Port Module */
/*@{*/

#define REV_PORT       0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_PORT_FROM_CMSIS__

/* -------- PORT_GROUP_DIR : (PORT Offset: 0x00) (R/W 32) Group Data Direction Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_DIR_Type;

#define PORT_GROUP_DIR_OFFSET       0x00         /**< \brief (PORT offset) Data Direction Register */
#define PORT_GROUP_DIR_RESETVALUE   0x0          /**< \brief (PORT reset_value) Data Direction Register */
#define PORT_GROUP_DIR_MASK         0xFFFFFFFFu  /**< \brief (PORT_GROUP_DIR) MASK Register */

/* -------- PORT_GROUP_DIRCLR : (PORT Offset: 0x04) (R/W 32) Group Data Direction Clear Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_DIRCLR_Type;

#define PORT_GROUP_DIRCLR_OFFSET    0x04         /**< \brief (PORT offset) Data Direction Clear Register */
#define PORT_GROUP_DIRCLR_RESETVALUE 0x0          /**< \brief (PORT reset_value) Data Direction Clear Register */
#define PORT_GROUP_DIRCLR_MASK      0xFFFFFFFFu  /**< \brief (PORT_GROUP_DIRCLR) MASK Register */

/* -------- PORT_GROUP_DIRSET : (PORT Offset: 0x08) (R/W 32) Group Data Direction Set Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_DIRSET_Type;

#define PORT_GROUP_DIRSET_OFFSET    0x08         /**< \brief (PORT offset) Data Direction Set Register */
#define PORT_GROUP_DIRSET_RESETVALUE 0x0          /**< \brief (PORT reset_value) Data Direction Set Register */
#define PORT_GROUP_DIRSET_MASK      0xFFFFFFFFu  /**< \brief (PORT_GROUP_DIRSET) MASK Register */

/* -------- PORT_GROUP_DIRTGL : (PORT Offset: 0x0C) (R/W 32) Group Data Direction Toggle Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_DIRTGL_Type;

#define PORT_GROUP_DIRTGL_OFFSET    0x0C         /**< \brief (PORT offset) Data Direction Toggle Register */
#define PORT_GROUP_DIRTGL_RESETVALUE 0x0          /**< \brief (PORT reset_value) Data Direction Toggle Register */
#define PORT_GROUP_DIRTGL_MASK      0xFFFFFFFFu  /**< \brief (PORT_GROUP_DIRTGL) MASK Register */

/* -------- PORT_GROUP_OUT : (PORT Offset: 0x10) (R/W 32) Group Data Output Value Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_OUT_Type;

#define PORT_GROUP_OUT_OFFSET       0x10         /**< \brief (PORT offset) Data Output Value Register */
#define PORT_GROUP_OUT_RESETVALUE   0x0          /**< \brief (PORT reset_value) Data Output Value Register */
#define PORT_GROUP_OUT_MASK         0xFFFFFFFFu  /**< \brief (PORT_GROUP_OUT) MASK Register */

/* -------- PORT_GROUP_OUTCLR : (PORT Offset: 0x14) (R/W 32) Group Data Output Value Clear Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_OUTCLR_Type;

#define PORT_GROUP_OUTCLR_OFFSET    0x14         /**< \brief (PORT offset) Data Output Value Clear Register */
#define PORT_GROUP_OUTCLR_RESETVALUE 0x0          /**< \brief (PORT reset_value) Data Output Value Clear Register */
#define PORT_GROUP_OUTCLR_MASK      0xFFFFFFFFu  /**< \brief (PORT_GROUP_OUTCLR) MASK Register */

/* -------- PORT_GROUP_OUTSET : (PORT Offset: 0x18) (R/W 32) Group Data Output Value Set Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_OUTSET_Type;

#define PORT_GROUP_OUTSET_OFFSET    0x18         /**< \brief (PORT offset) Data Output Value Set Register */
#define PORT_GROUP_OUTSET_RESETVALUE 0x0          /**< \brief (PORT reset_value) Data Output Value Set Register */
#define PORT_GROUP_OUTSET_MASK      0xFFFFFFFFu  /**< \brief (PORT_GROUP_OUTSET) MASK Register */

/* -------- PORT_GROUP_OUTTGL : (PORT Offset: 0x1C) (R/W 32) Group Data Output Value Toggle Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_OUTTGL_Type;

#define PORT_GROUP_OUTTGL_OFFSET    0x1C         /**< \brief (PORT offset) Data Output Value Toggle Register */
#define PORT_GROUP_OUTTGL_RESETVALUE 0x0          /**< \brief (PORT reset_value) Data Output Value Toggle Register */
#define PORT_GROUP_OUTTGL_MASK      0xFFFFFFFFu  /**< \brief (PORT_GROUP_OUTTGL) MASK Register */

/* -------- PORT_GROUP_IN : (PORT Offset: 0x20) (R/  32) Group Data Input Value Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_IN_Type;

#define PORT_GROUP_IN_OFFSET        0x20         /**< \brief (PORT offset) Data Input Value Register */
#define PORT_GROUP_IN_RESETVALUE    0x0          /**< \brief (PORT reset_value) Data Input Value Register */
#define PORT_GROUP_IN_MASK          0xFFFFFFFFu  /**< \brief (PORT_GROUP_IN) MASK Register */

/* -------- PORT_GROUP_CTRL : (PORT Offset: 0x24) (R/W 32) Group Control Register -------- */
typedef union {
  struct {
    uint32_t SAMPLING:32;      /*!< bit:  0..31  Input Sampling Mode                */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_CTRL_Type;

#define PORT_GROUP_CTRL_OFFSET      0x24         /**< \brief (PORT offset) Control Register */
#define PORT_GROUP_CTRL_RESETVALUE  0x0          /**< \brief (PORT reset_value) Control Register */

#define PORT_GROUP_CTRL_SAMPLING_Pos    0
#define PORT_GROUP_CTRL_SAMPLING_Msk    (0xFFFFFFFFu << PORT_GROUP_CTRL_SAMPLING_Pos) /**< \brief (PORT_GROUP_CTRL) Input Sampling Mode */
#define PORT_GROUP_CTRL_SAMPLING(value) ((PORT_GROUP_CTRL_SAMPLING_Msk & ((value) << PORT_GROUP_CTRL_SAMPLING_Pos)))
#define PORT_GROUP_CTRL_MASK        0xFFFFFFFFu /**< \brief (PORT_GROUP_CTRL) MASK Register */

/* -------- PORT_GROUP_WRCONFIG : (PORT Offset: 0x28) ( /W 32) Group Write Configuration Register -------- */
typedef union {
  struct {
    uint32_t PINMASK:16;       /*!< bit:  0..15  Pin Mask for Multiple Pin Configuration */
    uint32_t PMUXEN:1;         /*!< bit:     16  Select Peripheral Multiplexer      */
    uint32_t INEN:1;           /*!< bit:     17  Input Enable                       */
    uint32_t PULLEN:1;         /*!< bit:     18  Pull Enable                        */
    uint32_t ODRAIN:1;         /*!< bit:     19  Open Drain Output                  */
    uint32_t SLEWLIM:1;        /*!< bit:     20  Output Driver Slew Rate Limit Enable */
    uint32_t _reserved0:1;     /*!< bit:     21  Reserved                           */
    uint32_t DRVSTR:1;         /*!< bit:     22  Output Driver Strength Selection   */
    uint32_t _reserved1:1;     /*!< bit:     23  Reserved                           */
    uint32_t PMUX:4;           /*!< bit: 24..27  Peripheral Multiplexing Template   */
    uint32_t WRPMUX:1;         /*!< bit:     28  Write PMUX Registers               */
    uint32_t _reserved2:1;     /*!< bit:     29  Reserved                           */
    uint32_t WRPINCFG:1;       /*!< bit:     30  Write PINCFG Registers             */
    uint32_t HWSEL:1;          /*!< bit:     31  Half-Word Select                   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} PORT_GROUP_WRCONFIG_Type;

#define PORT_GROUP_WRCONFIG_OFFSET  0x28         /**< \brief (PORT offset) Write Configuration Register */
#define PORT_GROUP_WRCONFIG_RESETVALUE 0x0          /**< \brief (PORT reset_value) Write Configuration Register */

#define PORT_GROUP_WRCONFIG_PINMASK_Pos    0
#define PORT_GROUP_WRCONFIG_PINMASK_Msk    (0xFFFFu << PORT_GROUP_WRCONFIG_PINMASK_Pos) /**< \brief (PORT_GROUP_WRCONFIG) Pin Mask for Multiple Pin Configuration */
#define PORT_GROUP_WRCONFIG_PINMASK(value) ((PORT_GROUP_WRCONFIG_PINMASK_Msk & ((value) << PORT_GROUP_WRCONFIG_PINMASK_Pos)))
#define PORT_GROUP_WRCONFIG_PMUXEN  (0x1u << 16) /**< \brief (PORT_GROUP_WRCONFIG) Select Peripheral Multiplexer */
#define PORT_GROUP_WRCONFIG_INEN    (0x1u << 17) /**< \brief (PORT_GROUP_WRCONFIG) Input Enable */
#define PORT_GROUP_WRCONFIG_PULLEN  (0x1u << 18) /**< \brief (PORT_GROUP_WRCONFIG) Pull Enable */
#define PORT_GROUP_WRCONFIG_ODRAIN  (0x1u << 19) /**< \brief (PORT_GROUP_WRCONFIG) Open Drain Output */
#define PORT_GROUP_WRCONFIG_SLEWLIM (0x1u << 20) /**< \brief (PORT_GROUP_WRCONFIG) Output Driver Slew Rate Limit Enable */
#define PORT_GROUP_WRCONFIG_DRVSTR  (0x1u << 22) /**< \brief (PORT_GROUP_WRCONFIG) Output Driver Strength Selection */
#define PORT_GROUP_WRCONFIG_PMUX_Pos    24
#define PORT_GROUP_WRCONFIG_PMUX_Msk    (0xFu << PORT_GROUP_WRCONFIG_PMUX_Pos) /**< \brief (PORT_GROUP_WRCONFIG) Peripheral Multiplexing Template */
#define PORT_GROUP_WRCONFIG_PMUX(value) ((PORT_GROUP_WRCONFIG_PMUX_Msk & ((value) << PORT_GROUP_WRCONFIG_PMUX_Pos)))
#define PORT_GROUP_WRCONFIG_WRPMUX  (0x1u << 28) /**< \brief (PORT_GROUP_WRCONFIG) Write PMUX Registers */
#define PORT_GROUP_WRCONFIG_WRPINCFG (0x1u << 30) /**< \brief (PORT_GROUP_WRCONFIG) Write PINCFG Registers */
#define PORT_GROUP_WRCONFIG_HWSEL   (0x1u << 31) /**< \brief (PORT_GROUP_WRCONFIG) Half-Word Select */
#define PORT_GROUP_WRCONFIG_MASK    0xDF5FFFFFu /**< \brief (PORT_GROUP_WRCONFIG) MASK Register */

/* -------- PORT_GROUP_PMUX : (PORT Offset: 0x30) (R/W  8) Group_pmux Peripheral Multiplexing Register 0 -------- */
typedef union {
  struct {
    uint8_t  PMUXE:4;          /*!< bit:  0.. 3  Peripheral Multiplexing for even-numbered Pin */
    uint8_t  PMUXO:4;          /*!< bit:  4.. 7  Peripheral Multiplexing for odd-numbered Pin */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PORT_GROUP_PMUX_Type;

#define PORT_GROUP_PMUX_OFFSET      0x30         /**< \brief (PORT offset) Peripheral Multiplexing Register 0 */
#define PORT_GROUP_PMUX_RESETVALUE  0x0          /**< \brief (PORT reset_value) Peripheral Multiplexing Register 0 */

#define PORT_GROUP_PMUX_PMUXE_Pos    0
#define PORT_GROUP_PMUX_PMUXE_Msk    (0xFu << PORT_GROUP_PMUX_PMUXE_Pos) /**< \brief (PORT_GROUP_PMUX) Peripheral Multiplexing for even-numbered Pin */
#define PORT_GROUP_PMUX_PMUXE(value) ((PORT_GROUP_PMUX_PMUXE_Msk & ((value) << PORT_GROUP_PMUX_PMUXE_Pos)))
#define PORT_GROUP_PMUX_PMUXO_Pos    4
#define PORT_GROUP_PMUX_PMUXO_Msk    (0xFu << PORT_GROUP_PMUX_PMUXO_Pos) /**< \brief (PORT_GROUP_PMUX) Peripheral Multiplexing for odd-numbered Pin */
#define PORT_GROUP_PMUX_PMUXO(value) ((PORT_GROUP_PMUX_PMUXO_Msk & ((value) << PORT_GROUP_PMUX_PMUXO_Pos)))
#define PORT_GROUP_PMUX_MASK        0xFFu /**< \brief (PORT_GROUP_PMUX) MASK Register */

/* -------- PORT_GROUP_PINCFG : (PORT Offset: 0x40) (R/W  8) Group_pincfg Pin Configuration Register 0 -------- */
typedef union {
  struct {
    uint8_t  PMUXEN:1;         /*!< bit:      0  Select Peripheral Multiplexer      */
    uint8_t  INEN:1;           /*!< bit:      1  Input Enable                       */
    uint8_t  PULLEN:1;         /*!< bit:      2  Pull Enable                        */
    uint8_t  ODRAIN:1;         /*!< bit:      3  Open Drain Output                  */
    uint8_t  SLEWLIM:1;        /*!< bit:      4  Output Driver Slew Rate Limit Enable */
    uint8_t  _reserved0:1;     /*!< bit:      5  Reserved                           */
    uint8_t  DRVSTR:1;         /*!< bit:      6  Output Driver Strength Selection   */
    uint8_t  _reserved1:1;     /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PORT_GROUP_PINCFG_Type;

#define PORT_GROUP_PINCFG_OFFSET    0x40         /**< \brief (PORT offset) Pin Configuration Register 0 */
#define PORT_GROUP_PINCFG_RESETVALUE 0x0          /**< \brief (PORT reset_value) Pin Configuration Register 0 */

#define PORT_GROUP_PINCFG_PMUXEN    (0x1u <<  0) /**< \brief (PORT_GROUP_PINCFG) Select Peripheral Multiplexer */
#define PORT_GROUP_PINCFG_INEN      (0x1u <<  1) /**< \brief (PORT_GROUP_PINCFG) Input Enable */
#define PORT_GROUP_PINCFG_PULLEN    (0x1u <<  2) /**< \brief (PORT_GROUP_PINCFG) Pull Enable */
#define PORT_GROUP_PINCFG_ODRAIN    (0x1u <<  3) /**< \brief (PORT_GROUP_PINCFG) Open Drain Output */
#define PORT_GROUP_PINCFG_SLEWLIM   (0x1u <<  4) /**< \brief (PORT_GROUP_PINCFG) Output Driver Slew Rate Limit Enable */
#define PORT_GROUP_PINCFG_DRVSTR    (0x1u <<  6) /**< \brief (PORT_GROUP_PINCFG) Output Driver Strength Selection */
#define PORT_GROUP_PINCFG_MASK      0x5Fu /**< \brief (PORT_GROUP_PINCFG) MASK Register */

#define PORT_GROUPS                 2

/** \brief PortGroup hardware registers */
typedef struct {
  __IO PORT_GROUP_DIR_Type       DIR;         /**< \brief Offset: 0x00 (R/W 32) Data Direction Register */
  __IO PORT_GROUP_DIRCLR_Type    DIRCLR;      /**< \brief Offset: 0x04 (R/W 32) Data Direction Clear Register */
  __IO PORT_GROUP_DIRSET_Type    DIRSET;      /**< \brief Offset: 0x08 (R/W 32) Data Direction Set Register */
  __IO PORT_GROUP_DIRTGL_Type    DIRTGL;      /**< \brief Offset: 0x0C (R/W 32) Data Direction Toggle Register */
  __IO PORT_GROUP_OUT_Type       OUT;         /**< \brief Offset: 0x10 (R/W 32) Data Output Value Register */
  __IO PORT_GROUP_OUTCLR_Type    OUTCLR;      /**< \brief Offset: 0x14 (R/W 32) Data Output Value Clear Register */
  __IO PORT_GROUP_OUTSET_Type    OUTSET;      /**< \brief Offset: 0x18 (R/W 32) Data Output Value Set Register */
  __IO PORT_GROUP_OUTTGL_Type    OUTTGL;      /**< \brief Offset: 0x1C (R/W 32) Data Output Value Toggle Register */
  __I  PORT_GROUP_IN_Type        IN;          /**< \brief Offset: 0x20 (R/  32) Data Input Value Register */
  __IO PORT_GROUP_CTRL_Type      CTRL;        /**< \brief Offset: 0x24 (R/W 32) Control Register */
  __O  PORT_GROUP_WRCONFIG_Type  WRCONFIG;    /**< \brief Offset: 0x28 ( /W 32) Write Configuration Register */
       RoReg8                    Reserved1[0x4];
  __IO PORT_GROUP_PMUX_Type      PMUX[16];    /**< \brief Offset: 0x30 (R/W  8) Peripheral Multiplexing Register 0 */
  __IO PORT_GROUP_PINCFG_Type    PINCFG[32];  /**< \brief Offset: 0x40 (R/W  8) Pin Configuration Register 0 */
       RoReg8                    Reserved2[0x20];
} PortGroup;

/** \brief PORT hardware registers */
typedef struct {
       PortGroup                 Group[PORT_GROUPS]; /**< \brief Offset: 0x00 PortGroup groups */
       PortGroup                 Reserved1[4 - PORT_GROUPS];
} Port;

#endif /*  __EXCLUDE_PORT_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PTC */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_PTC Peripheral Touch Controller */
/*@{*/

#define REV_PTC        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_PTC_FROM_CMSIS__

/* -------- PTC_CTRLA : (PTC Offset: 0x00) (R/W  8) Control Register A -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable                             */
    uint8_t  SLEEPEN:1;        /*!< bit:      2  Sleep Enable                       */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTRLA_Type;

#define PTC_CTRLA_OFFSET            0x00         /**< \brief (PTC offset) Control Register A */
#define PTC_CTRLA_RESETVALUE        0x0          /**< \brief (PTC reset_value) Control Register A */

#define PTC_CTRLA_SWRST             (0x1u <<  0) /**< \brief (PTC_CTRLA) Software Reset */
#define PTC_CTRLA_ENABLE            (0x1u <<  1) /**< \brief (PTC_CTRLA) Enable */
#define PTC_CTRLA_SLEEPEN           (0x1u <<  2) /**< \brief (PTC_CTRLA) Sleep Enable */
#define PTC_CTRLA_MASK              0x7u /**< \brief (PTC_CTRLA) MASK Register */

/* -------- PTC_STATUS : (PTC Offset: 0x01) (R/   8) Status Register -------- */
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset Status              */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable Status                      */
    uint8_t  _reserved0:1;     /*!< bit:      2  Reserved                           */
    uint8_t  CTSBUSY:1;        /*!< bit:      3  CTS Busy                           */
    uint8_t  _reserved1:3;     /*!< bit:  4.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Synchronization Busy               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_STATUS_Type;

#define PTC_STATUS_OFFSET           0x01         /**< \brief (PTC offset) Status Register */
#define PTC_STATUS_RESETVALUE       0x0          /**< \brief (PTC reset_value) Status Register */

#define PTC_STATUS_SWRST            (0x1u <<  0) /**< \brief (PTC_STATUS) Software Reset Status */
#define PTC_STATUS_ENABLE           (0x1u <<  1) /**< \brief (PTC_STATUS) Enable Status */
#define PTC_STATUS_CTSBUSY          (0x1u <<  3) /**< \brief (PTC_STATUS) CTS Busy */
#define PTC_STATUS_SYNCBUSY         (0x1u <<  7) /**< \brief (PTC_STATUS) Synchronization Busy */
#define PTC_STATUS_MASK             0x8Bu /**< \brief (PTC_STATUS) MASK Register */

/* -------- PTC_CTRLB : (PTC Offset: 0x04) (R/W  8) Control Register B -------- */
typedef union {
  struct {
    uint8_t  PRSC:2;           /*!< bit:  0.. 1  Clock Prescaler                    */
    uint8_t  FREERUN:1;        /*!< bit:      2  Freerunning PTC                    */
    uint8_t  ADRS:1;           /*!< bit:      3  ADC Resolution Select              */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTRLB_Type;

#define PTC_CTRLB_OFFSET            0x04         /**< \brief (PTC offset) Control Register B */
#define PTC_CTRLB_RESETVALUE        0x0          /**< \brief (PTC reset_value) Control Register B */

#define PTC_CTRLB_PRSC_Pos          0
#define PTC_CTRLB_PRSC_Msk          (0x3u << PTC_CTRLB_PRSC_Pos) /**< \brief (PTC_CTRLB) Clock Prescaler */
#define PTC_CTRLB_PRSC(value)       ((PTC_CTRLB_PRSC_Msk & ((value) << PTC_CTRLB_PRSC_Pos)))
#define PTC_CTRLB_FREERUN           (0x1u <<  2) /**< \brief (PTC_CTRLB) Freerunning PTC */
#define PTC_CTRLB_ADRS              (0x1u <<  3) /**< \brief (PTC_CTRLB) ADC Resolution Select */
#define PTC_CTRLB_MASK              0xFu /**< \brief (PTC_CTRLB) MASK Register */

/* -------- PTC_EVCTRL : (PTC Offset: 0x05) (R/W  8) Event Control Register -------- */
typedef union {
  struct {
    uint8_t  STCONVEI:1;       /*!< bit:      0  Start Conversion Event Input       */
    uint8_t  EOCEO:1;          /*!< bit:      1  End of Conversion Event Output     */
    uint8_t  WCOMPEO:1;        /*!< bit:      2  Window Comparator Event Output     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_EVCTRL_Type;

#define PTC_EVCTRL_OFFSET           0x05         /**< \brief (PTC offset) Event Control Register */
#define PTC_EVCTRL_RESETVALUE       0x0          /**< \brief (PTC reset_value) Event Control Register */

#define PTC_EVCTRL_STCONVEI         (0x1u <<  0) /**< \brief (PTC_EVCTRL) Start Conversion Event Input */
#define PTC_EVCTRL_EOCEO            (0x1u <<  1) /**< \brief (PTC_EVCTRL) End of Conversion Event Output */
#define PTC_EVCTRL_WCOMPEO          (0x1u <<  2) /**< \brief (PTC_EVCTRL) Window Comparator Event Output */
#define PTC_EVCTRL_MASK             0x7u /**< \brief (PTC_EVCTRL) MASK Register */

/* -------- PTC_INTENCLR : (PTC Offset: 0x08) (R/W  8) Interrupt Enable Clear -------- */
typedef union {
  struct {
    uint8_t  EOC:1;            /*!< bit:      0  Clear EOC Interrupt Enable         */
    uint8_t  WCOMP:1;          /*!< bit:      1  Clear WCOMP Interrupt Enable       */
    uint8_t  READY:1;          /*!< bit:      2  Clear Ready Interrupt Enable       */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_INTENCLR_Type;

#define PTC_INTENCLR_OFFSET         0x08         /**< \brief (PTC offset) Interrupt Enable Clear */
#define PTC_INTENCLR_RESETVALUE     0x0          /**< \brief (PTC reset_value) Interrupt Enable Clear */

#define PTC_INTENCLR_EOC            (0x1u <<  0) /**< \brief (PTC_INTENCLR) Clear EOC Interrupt Enable */
#define PTC_INTENCLR_WCOMP          (0x1u <<  1) /**< \brief (PTC_INTENCLR) Clear WCOMP Interrupt Enable */
#define PTC_INTENCLR_READY          (0x1u <<  2) /**< \brief (PTC_INTENCLR) Clear Ready Interrupt Enable */
#define PTC_INTENCLR_MASK           0x7u /**< \brief (PTC_INTENCLR) MASK Register */

/* -------- PTC_INTENSET : (PTC Offset: 0x09) (R/W  8) Interrupt Enable Set -------- */
typedef union {
  struct {
    uint8_t  EOC:1;            /*!< bit:      0  Set EOC Interrupt Enable           */
    uint8_t  WCOMP:1;          /*!< bit:      1  Set WCOMP Interrupt Enable         */
    uint8_t  READY:1;          /*!< bit:      2  Set READY Interrupt Enable         */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_INTENSET_Type;

#define PTC_INTENSET_OFFSET         0x09         /**< \brief (PTC offset) Interrupt Enable Set */
#define PTC_INTENSET_RESETVALUE     0x0          /**< \brief (PTC reset_value) Interrupt Enable Set */

#define PTC_INTENSET_EOC            (0x1u <<  0) /**< \brief (PTC_INTENSET) Set EOC Interrupt Enable */
#define PTC_INTENSET_WCOMP          (0x1u <<  1) /**< \brief (PTC_INTENSET) Set WCOMP Interrupt Enable */
#define PTC_INTENSET_READY          (0x1u <<  2) /**< \brief (PTC_INTENSET) Set READY Interrupt Enable */
#define PTC_INTENSET_MASK           0x7u /**< \brief (PTC_INTENSET) MASK Register */

/* -------- PTC_INTFLAG : (PTC Offset: 0x0A) (R/W  8) Interrupt Flag -------- */
typedef union {
  struct {
    uint8_t  EOC:1;            /*!< bit:      0  EOC Interrupt Flag                 */
    uint8_t  WCOMP:1;          /*!< bit:      1  WCOMP Interrupt Flag               */
    uint8_t  READY:1;          /*!< bit:      2  READY Interrupt Flag               */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_INTFLAG_Type;

#define PTC_INTFLAG_OFFSET          0x0A         /**< \brief (PTC offset) Interrupt Flag */
#define PTC_INTFLAG_RESETVALUE      0x0          /**< \brief (PTC reset_value) Interrupt Flag */

#define PTC_INTFLAG_EOC             (0x1u <<  0) /**< \brief (PTC_INTFLAG) EOC Interrupt Flag */
#define PTC_INTFLAG_WCOMP           (0x1u <<  1) /**< \brief (PTC_INTFLAG) WCOMP Interrupt Flag */
#define PTC_INTFLAG_READY           (0x1u <<  2) /**< \brief (PTC_INTFLAG) READY Interrupt Flag */
#define PTC_INTFLAG_MASK            0x7u /**< \brief (PTC_INTFLAG) MASK Register */

/* -------- PTC_CTSCTRLA : (PTC Offset: 0x0C) (R/W  8) CTS Control Register A -------- */
typedef union {
  struct {
    uint8_t  SDS:4;            /*!< bit:  0.. 3  Sampling Delay Selection           */
    uint8_t  ASDV:1;           /*!< bit:      4  Automatic Sampling Delay Variation */
    uint8_t  CCDS:2;           /*!< bit:  5.. 6  Channel Change Delay Selection     */
    uint8_t  _reserved0:1;     /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTSCTRLA_Type;

#define PTC_CTSCTRLA_OFFSET         0x0C         /**< \brief (PTC offset) CTS Control Register A */
#define PTC_CTSCTRLA_RESETVALUE     0x0          /**< \brief (PTC reset_value) CTS Control Register A */

#define PTC_CTSCTRLA_SDS_Pos        0
#define PTC_CTSCTRLA_SDS_Msk        (0xFu << PTC_CTSCTRLA_SDS_Pos) /**< \brief (PTC_CTSCTRLA) Sampling Delay Selection */
#define PTC_CTSCTRLA_SDS(value)     ((PTC_CTSCTRLA_SDS_Msk & ((value) << PTC_CTSCTRLA_SDS_Pos)))
#define PTC_CTSCTRLA_ASDV           (0x1u <<  4) /**< \brief (PTC_CTSCTRLA) Automatic Sampling Delay Variation */
#define PTC_CTSCTRLA_CCDS_Pos       5
#define PTC_CTSCTRLA_CCDS_Msk       (0x3u << PTC_CTSCTRLA_CCDS_Pos) /**< \brief (PTC_CTSCTRLA) Channel Change Delay Selection */
#define PTC_CTSCTRLA_CCDS(value)    ((PTC_CTSCTRLA_CCDS_Msk & ((value) << PTC_CTSCTRLA_CCDS_Pos)))
#define PTC_CTSCTRLA_MASK           0x7Fu /**< \brief (PTC_CTSCTRLA) MASK Register */

/* -------- PTC_CTSCTRLC : (PTC Offset: 0x0D) (R/W  8) CTS Control Register C -------- */
typedef union {
  struct {
    uint8_t  ADAS:3;           /*!< bit:  0.. 2  ADC Accumulation Number Select     */
    uint8_t  _reserved0:4;     /*!< bit:  3.. 6  Reserved                           */
    uint8_t  STCONV:1;         /*!< bit:      7  Start Conversion                   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTSCTRLC_Type;

#define PTC_CTSCTRLC_OFFSET         0x0D         /**< \brief (PTC offset) CTS Control Register C */
#define PTC_CTSCTRLC_RESETVALUE     0x0          /**< \brief (PTC reset_value) CTS Control Register C */

#define PTC_CTSCTRLC_ADAS_Pos       0
#define PTC_CTSCTRLC_ADAS_Msk       (0x7u << PTC_CTSCTRLC_ADAS_Pos) /**< \brief (PTC_CTSCTRLC) ADC Accumulation Number Select */
#define PTC_CTSCTRLC_ADAS(value)    ((PTC_CTSCTRLC_ADAS_Msk & ((value) << PTC_CTSCTRLC_ADAS_Pos)))
#define PTC_CTSCTRLC_STCONV         (0x1u <<  7) /**< \brief (PTC_CTSCTRLC) Start Conversion */
#define PTC_CTSCTRLC_MASK           0x87u /**< \brief (PTC_CTSCTRLC) MASK Register */

/* -------- PTC_YSEL : (PTC Offset: 0x10) (R/W 16) Y Line Select -------- */
typedef union {
  struct {
    uint16_t YSEL:16;          /*!< bit:  0..15  Y Line Selection                   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_YSEL_Type;

#define PTC_YSEL_OFFSET             0x10         /**< \brief (PTC offset) Y Line Select */
#define PTC_YSEL_RESETVALUE         0x0          /**< \brief (PTC reset_value) Y Line Select */

#define PTC_YSEL_YSEL_Pos           0
#define PTC_YSEL_YSEL_Msk           (0xFFFFu << PTC_YSEL_YSEL_Pos) /**< \brief (PTC_YSEL) Y Line Selection */
#define PTC_YSEL_YSEL(value)        ((PTC_YSEL_YSEL_Msk & ((value) << PTC_YSEL_YSEL_Pos)))
#define PTC_YSEL_MASK               0xFFFFu /**< \brief (PTC_YSEL) MASK Register */

/* -------- PTC_XSEL : (PTC Offset: 0x12) (R/W 16) X Line Select -------- */
typedef union {
  struct {
    uint16_t XSEL:16;          /*!< bit:  0..15  X Line Selection                   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_XSEL_Type;

#define PTC_XSEL_OFFSET             0x12         /**< \brief (PTC offset) X Line Select */
#define PTC_XSEL_RESETVALUE         0x0          /**< \brief (PTC reset_value) X Line Select */

#define PTC_XSEL_XSEL_Pos           0
#define PTC_XSEL_XSEL_Msk           (0xFFFFu << PTC_XSEL_XSEL_Pos) /**< \brief (PTC_XSEL) X Line Selection */
#define PTC_XSEL_XSEL(value)        ((PTC_XSEL_XSEL_Msk & ((value) << PTC_XSEL_XSEL_Pos)))
#define PTC_XSEL_MASK               0xFFFFu /**< \brief (PTC_XSEL) MASK Register */

/* -------- PTC_YEN : (PTC Offset: 0x14) (R/W 16) Y Line Enable -------- */
typedef union {
  struct {
    uint16_t YEN:16;           /*!< bit:  0..15  Y Line Enable                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_YEN_Type;

#define PTC_YEN_OFFSET              0x14         /**< \brief (PTC offset) Y Line Enable */
#define PTC_YEN_RESETVALUE          0x0          /**< \brief (PTC reset_value) Y Line Enable */

#define PTC_YEN_YEN_Pos             0
#define PTC_YEN_YEN_Msk             (0xFFFFu << PTC_YEN_YEN_Pos) /**< \brief (PTC_YEN) Y Line Enable */
#define PTC_YEN_YEN(value)          ((PTC_YEN_YEN_Msk & ((value) << PTC_YEN_YEN_Pos)))
#define PTC_YEN_MASK                0xFFFFu /**< \brief (PTC_YEN) MASK Register */

/* -------- PTC_XEN : (PTC Offset: 0x16) (R/W 16) X Line Enable -------- */
typedef union {
  struct {
    uint16_t XEN:16;           /*!< bit:  0..15  X Line Enable                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_XEN_Type;

#define PTC_XEN_OFFSET              0x16         /**< \brief (PTC offset) X Line Enable */
#define PTC_XEN_RESETVALUE          0x0          /**< \brief (PTC reset_value) X Line Enable */

#define PTC_XEN_XEN_Pos             0
#define PTC_XEN_XEN_Msk             (0xFFFFu << PTC_XEN_XEN_Pos) /**< \brief (PTC_XEN) X Line Enable */
#define PTC_XEN_XEN(value)          ((PTC_XEN_XEN_Msk & ((value) << PTC_XEN_XEN_Pos)))
#define PTC_XEN_MASK                0xFFFFu /**< \brief (PTC_XEN) MASK Register */

/* -------- PTC_CCCAL : (PTC Offset: 0x18) (R/W 16) CTS Compensation Capacitor Calibration Register -------- */
typedef union {
  struct {
    uint16_t CCA:4;            /*!< bit:  0.. 3  Compensation Capacitor Accurate Value */
    uint16_t CCF:4;            /*!< bit:  4.. 7  Compensation Capacitor Fine Value  */
    uint16_t CCC:4;            /*!< bit:  8..11  Compensation Capacitor Coarse Value */
    uint16_t CCR:2;            /*!< bit: 12..13  Compensation Capacitor Rough Value */
    uint16_t _reserved0:2;     /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_CCCAL_Type;

#define PTC_CCCAL_OFFSET            0x18         /**< \brief (PTC offset) CTS Compensation Capacitor Calibration Register */
#define PTC_CCCAL_RESETVALUE        0x0          /**< \brief (PTC reset_value) CTS Compensation Capacitor Calibration Register */

#define PTC_CCCAL_CCA_Pos           0
#define PTC_CCCAL_CCA_Msk           (0xFu << PTC_CCCAL_CCA_Pos) /**< \brief (PTC_CCCAL) Compensation Capacitor Accurate Value */
#define PTC_CCCAL_CCA(value)        ((PTC_CCCAL_CCA_Msk & ((value) << PTC_CCCAL_CCA_Pos)))
#define PTC_CCCAL_CCF_Pos           4
#define PTC_CCCAL_CCF_Msk           (0xFu << PTC_CCCAL_CCF_Pos) /**< \brief (PTC_CCCAL) Compensation Capacitor Fine Value */
#define PTC_CCCAL_CCF(value)        ((PTC_CCCAL_CCF_Msk & ((value) << PTC_CCCAL_CCF_Pos)))
#define PTC_CCCAL_CCC_Pos           8
#define PTC_CCCAL_CCC_Msk           (0xFu << PTC_CCCAL_CCC_Pos) /**< \brief (PTC_CCCAL) Compensation Capacitor Coarse Value */
#define PTC_CCCAL_CCC(value)        ((PTC_CCCAL_CCC_Msk & ((value) << PTC_CCCAL_CCC_Pos)))
#define PTC_CCCAL_CCR_Pos           12
#define PTC_CCCAL_CCR_Msk           (0x3u << PTC_CCCAL_CCR_Pos) /**< \brief (PTC_CCCAL) Compensation Capacitor Rough Value */
#define PTC_CCCAL_CCR(value)        ((PTC_CCCAL_CCR_Msk & ((value) << PTC_CCCAL_CCR_Pos)))
#define PTC_CCCAL_MASK              0x3FFFu /**< \brief (PTC_CCCAL) MASK Register */

/* -------- PTC_CICAL : (PTC Offset: 0x1A) (R/W  8) CTS Integrator Capacitor Calibration Register -------- */
typedef union {
  struct {
    uint8_t  CIF:4;            /*!< bit:  0.. 3  Integration Capacitor Fine Value   */
    uint8_t  CIC:2;            /*!< bit:  4.. 5  Integration Capacitor Coarse Value */
    uint8_t  _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CICAL_Type;

#define PTC_CICAL_OFFSET            0x1A         /**< \brief (PTC offset) CTS Integrator Capacitor Calibration Register */
#define PTC_CICAL_RESETVALUE        0x0          /**< \brief (PTC reset_value) CTS Integrator Capacitor Calibration Register */

#define PTC_CICAL_CIF_Pos           0
#define PTC_CICAL_CIF_Msk           (0xFu << PTC_CICAL_CIF_Pos) /**< \brief (PTC_CICAL) Integration Capacitor Fine Value */
#define PTC_CICAL_CIF(value)        ((PTC_CICAL_CIF_Msk & ((value) << PTC_CICAL_CIF_Pos)))
#define PTC_CICAL_CIC_Pos           4
#define PTC_CICAL_CIC_Msk           (0x3u << PTC_CICAL_CIC_Pos) /**< \brief (PTC_CICAL) Integration Capacitor Coarse Value */
#define PTC_CICAL_CIC(value)        ((PTC_CICAL_CIC_Msk & ((value) << PTC_CICAL_CIC_Pos)))
#define PTC_CICAL_MASK              0x3Fu /**< \brief (PTC_CICAL) MASK Register */

/* -------- PTC_CTSRS : (PTC Offset: 0x1B) (R/W  8) CTS Resistor Selection Register -------- */
typedef union {
  struct {
    uint8_t  RSEL:2;           /*!< bit:  0.. 1  Resistor Select                    */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTSRS_Type;

#define PTC_CTSRS_OFFSET            0x1B         /**< \brief (PTC offset) CTS Resistor Selection Register */
#define PTC_CTSRS_RESETVALUE        0x0          /**< \brief (PTC reset_value) CTS Resistor Selection Register */

#define PTC_CTSRS_RSEL_Pos          0
#define PTC_CTSRS_RSEL_Msk          (0x3u << PTC_CTSRS_RSEL_Pos) /**< \brief (PTC_CTSRS) Resistor Select */
#define PTC_CTSRS_RSEL(value)       ((PTC_CTSRS_RSEL_Msk & ((value) << PTC_CTSRS_RSEL_Pos)))
#define PTC_CTSRS_MASK              0x3u /**< \brief (PTC_CTSRS) MASK Register */

/* -------- PTC_RESULT : (PTC Offset: 0x1C) (R/W 16) Conversion Result Register -------- */
typedef union {
  struct {
    uint16_t RES:16;           /*!< bit:  0..15  Result                             */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_RESULT_Type;

#define PTC_RESULT_OFFSET           0x1C         /**< \brief (PTC offset) Conversion Result Register */
#define PTC_RESULT_RESETVALUE       0x0          /**< \brief (PTC reset_value) Conversion Result Register */

#define PTC_RESULT_RES_Pos          0
#define PTC_RESULT_RES_Msk          (0xFFFFu << PTC_RESULT_RES_Pos) /**< \brief (PTC_RESULT) Result */
#define PTC_RESULT_RES(value)       ((PTC_RESULT_RES_Msk & ((value) << PTC_RESULT_RES_Pos)))
#define PTC_RESULT_MASK             0xFFFFu /**< \brief (PTC_RESULT) MASK Register */

/* -------- PTC_CTSCTRLB : (PTC Offset: 0x20) (R/W  8) CTS Control Register B -------- */
typedef union {
  struct {
    uint8_t  _reserved0:2;     /*!< bit:  0.. 1  Reserved                           */
    uint8_t  CTSLP:1;          /*!< bit:      2  CTS Low Power Mode                 */
    uint8_t  _reserved1:1;     /*!< bit:      3  Reserved                           */
    uint8_t  CTSM:4;           /*!< bit:  4.. 7  CTS Mode                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTSCTRLB_Type;

#define PTC_CTSCTRLB_OFFSET         0x20         /**< \brief (PTC offset) CTS Control Register B */
#define PTC_CTSCTRLB_RESETVALUE     0x0          /**< \brief (PTC reset_value) CTS Control Register B */

#define PTC_CTSCTRLB_CTSLP          (0x1u <<  2) /**< \brief (PTC_CTSCTRLB) CTS Low Power Mode */
#define PTC_CTSCTRLB_CTSM_Pos       4
#define PTC_CTSCTRLB_CTSM_Msk       (0xFu << PTC_CTSCTRLB_CTSM_Pos) /**< \brief (PTC_CTSCTRLB) CTS Mode */
#define PTC_CTSCTRLB_CTSM(value)    ((PTC_CTSCTRLB_CTSM_Msk & ((value) << PTC_CTSCTRLB_CTSM_Pos)))
#define PTC_CTSCTRLB_MASK           0xF4u /**< \brief (PTC_CTSCTRLB) MASK Register */

/* -------- PTC_WINCCTRL : (PTC Offset: 0x21) (R/W  8) Window Comparator Control Register -------- */
typedef union {
  struct {
    uint8_t  WINCM:3;          /*!< bit:  0.. 2  Window Comparator Mode             */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_WINCCTRL_Type;

#define PTC_WINCCTRL_OFFSET         0x21         /**< \brief (PTC offset) Window Comparator Control Register */
#define PTC_WINCCTRL_RESETVALUE     0x0          /**< \brief (PTC reset_value) Window Comparator Control Register */

#define PTC_WINCCTRL_WINCM_Pos      0
#define PTC_WINCCTRL_WINCM_Msk      (0x7u << PTC_WINCCTRL_WINCM_Pos) /**< \brief (PTC_WINCCTRL) Window Comparator Mode */
#define PTC_WINCCTRL_WINCM(value)   ((PTC_WINCCTRL_WINCM_Msk & ((value) << PTC_WINCCTRL_WINCM_Pos)))
#define PTC_WINCCTRL_MASK           0x7u /**< \brief (PTC_WINCCTRL) MASK Register */

/* -------- PTC_WCHT : (PTC Offset: 0x24) (R/W 16) Window Comparator High Threshold Register -------- */
typedef union {
  struct {
    uint16_t WCHT:16;          /*!< bit:  0..15  Window Comparator High Threshold   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_WCHT_Type;

#define PTC_WCHT_OFFSET             0x24         /**< \brief (PTC offset) Window Comparator High Threshold Register */
#define PTC_WCHT_RESETVALUE         0x0          /**< \brief (PTC reset_value) Window Comparator High Threshold Register */

#define PTC_WCHT_WCHT_Pos           0
#define PTC_WCHT_WCHT_Msk           (0xFFFFu << PTC_WCHT_WCHT_Pos) /**< \brief (PTC_WCHT) Window Comparator High Threshold */
#define PTC_WCHT_WCHT(value)        ((PTC_WCHT_WCHT_Msk & ((value) << PTC_WCHT_WCHT_Pos)))
#define PTC_WCHT_MASK               0xFFFFu /**< \brief (PTC_WCHT) MASK Register */

/* -------- PTC_WCLT : (PTC Offset: 0x26) (R/W 16) Window Comparator Low Threshold Register -------- */
typedef union {
  struct {
    uint16_t WCLT:16;          /*!< bit:  0..15  Window Comparator Low Threshold    */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_WCLT_Type;

#define PTC_WCLT_OFFSET             0x26         /**< \brief (PTC offset) Window Comparator Low Threshold Register */
#define PTC_WCLT_RESETVALUE         0x0          /**< \brief (PTC reset_value) Window Comparator Low Threshold Register */

#define PTC_WCLT_WCLT_Pos           0
#define PTC_WCLT_WCLT_Msk           (0xFFFFu << PTC_WCLT_WCLT_Pos) /**< \brief (PTC_WCLT) Window Comparator Low Threshold */
#define PTC_WCLT_WCLT(value)        ((PTC_WCLT_WCLT_Msk & ((value) << PTC_WCLT_WCLT_Pos)))
#define PTC_WCLT_MASK               0xFFFFu /**< \brief (PTC_WCLT) MASK Register */

/** \brief PTC hardware registers */
typedef struct {
  __IO PTC_CTRLA_Type            CTRLA;       /**< \brief Offset: 0x00 (R/W  8) Control Register A */
  __I  PTC_STATUS_Type           STATUS;      /**< \brief Offset: 0x01 (R/   8) Status Register */
       RoReg8                    Reserved1[0x2];
  __IO PTC_CTRLB_Type            CTRLB;       /**< \brief Offset: 0x04 (R/W  8) Control Register B */
  __IO PTC_EVCTRL_Type           EVCTRL;      /**< \brief Offset: 0x05 (R/W  8) Event Control Register */
       RoReg8                    Reserved2[0x2];
  __IO PTC_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x08 (R/W  8) Interrupt Enable Clear */
  __IO PTC_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x09 (R/W  8) Interrupt Enable Set */
  __IO PTC_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x0A (R/W  8) Interrupt Flag */
       RoReg8                    Reserved3[0x1];
  __IO PTC_CTSCTRLA_Type         CTSCTRLA;    /**< \brief Offset: 0x0C (R/W  8) CTS Control Register A */
  __IO PTC_CTSCTRLC_Type         CTSCTRLC;    /**< \brief Offset: 0x0D (R/W  8) CTS Control Register C */
       RoReg8                    Reserved4[0x2];
  __IO PTC_YSEL_Type             YSEL;        /**< \brief Offset: 0x10 (R/W 16) Y Line Select */
  __IO PTC_XSEL_Type             XSEL;        /**< \brief Offset: 0x12 (R/W 16) X Line Select */
  __IO PTC_YEN_Type              YEN;         /**< \brief Offset: 0x14 (R/W 16) Y Line Enable */
  __IO PTC_XEN_Type              XEN;         /**< \brief Offset: 0x16 (R/W 16) X Line Enable */
  __IO PTC_CCCAL_Type            CCCAL;       /**< \brief Offset: 0x18 (R/W 16) CTS Compensation Capacitor Calibration Register */
  __IO PTC_CICAL_Type            CICAL;       /**< \brief Offset: 0x1A (R/W  8) CTS Integrator Capacitor Calibration Register */
  __IO PTC_CTSRS_Type            CTSRS;       /**< \brief Offset: 0x1B (R/W  8) CTS Resistor Selection Register */
  __IO PTC_RESULT_Type           RESULT;      /**< \brief Offset: 0x1C (R/W 16) Conversion Result Register */
       RoReg8                    Reserved5[0x2];
  __IO PTC_CTSCTRLB_Type         CTSCTRLB;    /**< \brief Offset: 0x20 (R/W  8) CTS Control Register B */
  __IO PTC_WINCCTRL_Type         WINCCTRL;    /**< \brief Offset: 0x21 (R/W  8) Window Comparator Control Register */
       RoReg8                    Reserved6[0x2];
  __IO PTC_WCHT_Type             WCHT;        /**< \brief Offset: 0x24 (R/W 16) Window Comparator High Threshold Register */
  __IO PTC_WCLT_Type             WCLT;        /**< \brief Offset: 0x26 (R/W 16) Window Comparator Low Threshold Register */
} Ptc;

#endif /*  __EXCLUDE_PTC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR RTC */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_RTC Real-Time Counter */
/*@{*/

#define REV_RTC        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_RTC_FROM_CMSIS__

/* -------- RTC_CTRL : (RTC Offset: 0x00) (R/W 16) Control Register -------- */
typedef union {
  struct { // CLOCK mode
    uint16_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  Mode                               */
    uint16_t _reserved0:2;     /*!< bit:  4.. 5  Reserved                           */
    uint16_t CLKREP:1;         /*!< bit:      6  Clock Representation               */
    uint16_t MATCHCLR:1;       /*!< bit:      7  Match Clears Counter               */
    uint16_t PRESCALER:4;      /*!< bit:  8..11  Prescaler                          */
    uint16_t _reserved1:4;     /*!< bit: 12..15  Reserved                           */
  } CLOCK;                     /*!< Structure used for CLOCK                        */
  struct { // COUNT16 mode
    uint16_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  Mode                               */
    uint16_t _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
    uint16_t PRESCALER:4;      /*!< bit:  8..11  Prescaler                          */
    uint16_t _reserved1:4;     /*!< bit: 12..15  Reserved                           */
  } COUNT16;                   /*!< Structure used for COUNT16                      */
  struct { // COUNT32 mode
    uint16_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  Mode                               */
    uint16_t _reserved0:3;     /*!< bit:  4.. 6  Reserved                           */
    uint16_t MATCHCLR:1;       /*!< bit:      7  Match Clears Counter               */
    uint16_t PRESCALER:4;      /*!< bit:  8..11  Prescaler                          */
    uint16_t _reserved1:4;     /*!< bit: 12..15  Reserved                           */
  } COUNT32;                   /*!< Structure used for COUNT32                      */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_CTRL_Type;

#define RTC_CTRL_OFFSET             0x00         /**< \brief (RTC offset) Control Register */
#define RTC_CTRL_RESETVALUE         0x0          /**< \brief (RTC reset_value) Control Register */

// CLOCK mode
#define RTC_CTRL_CLOCK_SWRST        (0x1u <<  0) /**< \brief (RTC_CTRL_CLOCK) Software Reset */
#define RTC_CTRL_CLOCK_ENABLE       (0x1u <<  1) /**< \brief (RTC_CTRL_CLOCK) Enable */
#define RTC_CTRL_CLOCK_MODE_Pos     2
#define RTC_CTRL_CLOCK_MODE_Msk     (0x3u << RTC_CTRL_CLOCK_MODE_Pos) /**< \brief (RTC_CTRL_CLOCK) Mode */
#define RTC_CTRL_CLOCK_MODE(value)  ((RTC_CTRL_CLOCK_MODE_Msk & ((value) << RTC_CTRL_CLOCK_MODE_Pos)))
#define   RTC_CTRL_CLOCK_MODE_COUNT32 (0x0u <<  2) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_MODE_COUNT16 (0x1u <<  2) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_MODE_CLOCK (0x2u <<  2) /**< \brief (RTC_CTRL_CLOCK)  */
#define RTC_CTRL_CLOCK_CLKREP       (0x1u <<  6) /**< \brief (RTC_CTRL_CLOCK) Clock Representation */
#define RTC_CTRL_CLOCK_MATCHCLR     (0x1u <<  7) /**< \brief (RTC_CTRL_CLOCK) Match Clears Counter */
#define RTC_CTRL_CLOCK_PRESCALER_Pos    8
#define RTC_CTRL_CLOCK_PRESCALER_Msk    (0xFu << RTC_CTRL_CLOCK_PRESCALER_Pos) /**< \brief (RTC_CTRL_CLOCK) Prescaler */
#define RTC_CTRL_CLOCK_PRESCALER(value) ((RTC_CTRL_CLOCK_PRESCALER_Msk & ((value) << RTC_CTRL_CLOCK_PRESCALER_Pos)))
#define   RTC_CTRL_CLOCK_PRESCALER_DIV1 (0x0u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV2 (0x1u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV4 (0x2u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV8 (0x3u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV16 (0x4u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV32 (0x5u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV64 (0x6u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV128 (0x7u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV256 (0x8u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV512 (0x9u <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define   RTC_CTRL_CLOCK_PRESCALER_DIV1024 (0xAu <<  8) /**< \brief (RTC_CTRL_CLOCK)  */
#define RTC_CTRL_CLOCK_MASK         0xFCFu /**< \brief (RTC_CTRL_CLOCK) MASK Register */

// COUNT16 mode
#define RTC_CTRL_COUNT16_SWRST      (0x1u <<  0) /**< \brief (RTC_CTRL_COUNT16) Software Reset */
#define RTC_CTRL_COUNT16_ENABLE     (0x1u <<  1) /**< \brief (RTC_CTRL_COUNT16) Enable */
#define RTC_CTRL_COUNT16_MODE_Pos    2
#define RTC_CTRL_COUNT16_MODE_Msk    (0x3u << RTC_CTRL_COUNT16_MODE_Pos) /**< \brief (RTC_CTRL_COUNT16) Mode */
#define RTC_CTRL_COUNT16_MODE(value) ((RTC_CTRL_COUNT16_MODE_Msk & ((value) << RTC_CTRL_COUNT16_MODE_Pos)))
#define RTC_CTRL_COUNT16_PRESCALER_Pos    8
#define RTC_CTRL_COUNT16_PRESCALER_Msk    (0xFu << RTC_CTRL_COUNT16_PRESCALER_Pos) /**< \brief (RTC_CTRL_COUNT16) Prescaler */
#define RTC_CTRL_COUNT16_PRESCALER(value) ((RTC_CTRL_COUNT16_PRESCALER_Msk & ((value) << RTC_CTRL_COUNT16_PRESCALER_Pos)))
#define RTC_CTRL_COUNT16_MASK       0xF0Fu /**< \brief (RTC_CTRL_COUNT16) MASK Register */

// COUNT32 mode
#define RTC_CTRL_COUNT32_SWRST      (0x1u <<  0) /**< \brief (RTC_CTRL_COUNT32) Software Reset */
#define RTC_CTRL_COUNT32_ENABLE     (0x1u <<  1) /**< \brief (RTC_CTRL_COUNT32) Enable */
#define RTC_CTRL_COUNT32_MODE_Pos    2
#define RTC_CTRL_COUNT32_MODE_Msk    (0x3u << RTC_CTRL_COUNT32_MODE_Pos) /**< \brief (RTC_CTRL_COUNT32) Mode */
#define RTC_CTRL_COUNT32_MODE(value) ((RTC_CTRL_COUNT32_MODE_Msk & ((value) << RTC_CTRL_COUNT32_MODE_Pos)))
#define RTC_CTRL_COUNT32_MATCHCLR   (0x1u <<  7) /**< \brief (RTC_CTRL_COUNT32) Match Clears Counter */
#define RTC_CTRL_COUNT32_PRESCALER_Pos    8
#define RTC_CTRL_COUNT32_PRESCALER_Msk    (0xFu << RTC_CTRL_COUNT32_PRESCALER_Pos) /**< \brief (RTC_CTRL_COUNT32) Prescaler */
#define RTC_CTRL_COUNT32_PRESCALER(value) ((RTC_CTRL_COUNT32_PRESCALER_Msk & ((value) << RTC_CTRL_COUNT32_PRESCALER_Pos)))
#define RTC_CTRL_COUNT32_MASK       0xF8Fu /**< \brief (RTC_CTRL_COUNT32) MASK Register */

/* -------- RTC_READREQ : (RTC Offset: 0x02) (R/W 16) Read Request Register -------- */
typedef union {
  struct {
    uint16_t ADDR:6;           /*!< bit:  0.. 5  Read Address                       */
    uint16_t _reserved0:8;     /*!< bit:  6..13  Reserved                           */
    uint16_t RCONT:1;          /*!< bit:     14  Read Continuously                  */
    uint16_t RREQ:1;           /*!< bit:     15  Read Request                       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_READREQ_Type;

#define RTC_READREQ_OFFSET          0x02         /**< \brief (RTC offset) Read Request Register */
#define RTC_READREQ_RESETVALUE      0x10         /**< \brief (RTC reset_value) Read Request Register */

#define RTC_READREQ_ADDR_Pos        0
#define RTC_READREQ_ADDR_Msk        (0x3Fu << RTC_READREQ_ADDR_Pos) /**< \brief (RTC_READREQ) Read Address */
#define RTC_READREQ_ADDR(value)     ((RTC_READREQ_ADDR_Msk & ((value) << RTC_READREQ_ADDR_Pos)))
#define RTC_READREQ_RCONT           (0x1u << 14) /**< \brief (RTC_READREQ) Read Continuously */
#define RTC_READREQ_RREQ            (0x1u << 15) /**< \brief (RTC_READREQ) Read Request */
#define RTC_READREQ_MASK            0xC03Fu /**< \brief (RTC_READREQ) MASK Register */

/* -------- RTC_EVCTRL : (RTC Offset: 0x04) (R/W 16) Event Control Register -------- */
typedef union {
  struct {
    uint16_t PER0EO:1;         /*!< bit:      0                                     */
    uint16_t PER1EO:1;         /*!< bit:      1                                     */
    uint16_t PER2EO:1;         /*!< bit:      2                                     */
    uint16_t PER3EO:1;         /*!< bit:      3                                     */
    uint16_t PER4EO:1;         /*!< bit:      4                                     */
    uint16_t PER5EO:1;         /*!< bit:      5                                     */
    uint16_t PER6EO:1;         /*!< bit:      6                                     */
    uint16_t PER7EO:1;         /*!< bit:      7                                     */
    uint16_t CMP0EO:1;         /*!< bit:      8                                     */
    uint16_t CMP1EO:1;         /*!< bit:      9                                     */
    uint16_t CMP2EO:1;         /*!< bit:     10                                     */
    uint16_t CMP3EO:1;         /*!< bit:     11                                     */
    uint16_t CMP4EO:1;         /*!< bit:     12                                     */
    uint16_t CMP5EO:1;         /*!< bit:     13                                     */
    uint16_t _reserved0:1;     /*!< bit:     14  Reserved                           */
    uint16_t OVFEO:1;          /*!< bit:     15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_EVCTRL_Type;

#define RTC_EVCTRL_OFFSET           0x04         /**< \brief (RTC offset) Event Control Register */
#define RTC_EVCTRL_RESETVALUE       0x0          /**< \brief (RTC reset_value) Event Control Register */

#define RTC_EVCTRL_PER0EO           (0x1u <<  0) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_PER1EO           (0x1u <<  1) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_PER2EO           (0x1u <<  2) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_PER3EO           (0x1u <<  3) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_PER4EO           (0x1u <<  4) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_PER5EO           (0x1u <<  5) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_PER6EO           (0x1u <<  6) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_PER7EO           (0x1u <<  7) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_CMP0EO           (0x1u <<  8) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_CMP1EO           (0x1u <<  9) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_CMP2EO           (0x1u << 10) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_CMP3EO           (0x1u << 11) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_CMP4EO           (0x1u << 12) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_CMP5EO           (0x1u << 13) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_OVFEO            (0x1u << 15) /**< \brief (RTC_EVCTRL)  */
#define RTC_EVCTRL_MASK             0xBFFFu /**< \brief (RTC_EVCTRL) MASK Register */

/* -------- RTC_INTENCLR : (RTC Offset: 0x06) (R/W  8) Interrupt Enable Clear Register -------- */
typedef union {
  struct {
    uint8_t  CMP0:1;           /*!< bit:      0                                     */
    uint8_t  CMP1:1;           /*!< bit:      1                                     */
    uint8_t  CMP2:1;           /*!< bit:      2                                     */
    uint8_t  CMP3:1;           /*!< bit:      3                                     */
    uint8_t  CMP4:1;           /*!< bit:      4                                     */
    uint8_t  CMP5:1;           /*!< bit:      5                                     */
    uint8_t  READY:1;          /*!< bit:      6                                     */
    uint8_t  OVF:1;            /*!< bit:      7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_INTENCLR_Type;

#define RTC_INTENCLR_OFFSET         0x06         /**< \brief (RTC offset) Interrupt Enable Clear Register */
#define RTC_INTENCLR_RESETVALUE     0x0          /**< \brief (RTC reset_value) Interrupt Enable Clear Register */

#define RTC_INTENCLR_CMP0           (0x1u <<  0) /**< \brief (RTC_INTENCLR)  */
#define RTC_INTENCLR_CMP1           (0x1u <<  1) /**< \brief (RTC_INTENCLR)  */
#define RTC_INTENCLR_CMP2           (0x1u <<  2) /**< \brief (RTC_INTENCLR)  */
#define RTC_INTENCLR_CMP3           (0x1u <<  3) /**< \brief (RTC_INTENCLR)  */
#define RTC_INTENCLR_CMP4           (0x1u <<  4) /**< \brief (RTC_INTENCLR)  */
#define RTC_INTENCLR_CMP5           (0x1u <<  5) /**< \brief (RTC_INTENCLR)  */
#define RTC_INTENCLR_READY          (0x1u <<  6) /**< \brief (RTC_INTENCLR)  */
#define RTC_INTENCLR_OVF            (0x1u <<  7) /**< \brief (RTC_INTENCLR)  */
#define RTC_INTENCLR_MASK           0xFFu /**< \brief (RTC_INTENCLR) MASK Register */

/* -------- RTC_INTENSET : (RTC Offset: 0x07) (R/W  8) Interrupt Enable Set Register -------- */
typedef union {
  struct {
    uint8_t  CMP0:1;           /*!< bit:      0                                     */
    uint8_t  CMP1:1;           /*!< bit:      1                                     */
    uint8_t  CMP2:1;           /*!< bit:      2                                     */
    uint8_t  CMP3:1;           /*!< bit:      3                                     */
    uint8_t  CMP4:1;           /*!< bit:      4                                     */
    uint8_t  CMP5:1;           /*!< bit:      5                                     */
    uint8_t  READY:1;          /*!< bit:      6                                     */
    uint8_t  OVF:1;            /*!< bit:      7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_INTENSET_Type;

#define RTC_INTENSET_OFFSET         0x07         /**< \brief (RTC offset) Interrupt Enable Set Register */
#define RTC_INTENSET_RESETVALUE     0x0          /**< \brief (RTC reset_value) Interrupt Enable Set Register */

#define RTC_INTENSET_CMP0           (0x1u <<  0) /**< \brief (RTC_INTENSET)  */
#define RTC_INTENSET_CMP1           (0x1u <<  1) /**< \brief (RTC_INTENSET)  */
#define RTC_INTENSET_CMP2           (0x1u <<  2) /**< \brief (RTC_INTENSET)  */
#define RTC_INTENSET_CMP3           (0x1u <<  3) /**< \brief (RTC_INTENSET)  */
#define RTC_INTENSET_CMP4           (0x1u <<  4) /**< \brief (RTC_INTENSET)  */
#define RTC_INTENSET_CMP5           (0x1u <<  5) /**< \brief (RTC_INTENSET)  */
#define RTC_INTENSET_READY          (0x1u <<  6) /**< \brief (RTC_INTENSET)  */
#define RTC_INTENSET_OVF            (0x1u <<  7) /**< \brief (RTC_INTENSET)  */
#define RTC_INTENSET_MASK           0xFFu /**< \brief (RTC_INTENSET) MASK Register */

/* -------- RTC_INTFLAG : (RTC Offset: 0x08) (R/W  8) Interrupt Flag Register -------- */
typedef union {
  struct {
    uint8_t  CMP0:1;           /*!< bit:      0                                     */
    uint8_t  CMP1:1;           /*!< bit:      1                                     */
    uint8_t  CMP2:1;           /*!< bit:      2                                     */
    uint8_t  CMP3:1;           /*!< bit:      3                                     */
    uint8_t  CMP4:1;           /*!< bit:      4                                     */
    uint8_t  CMP5:1;           /*!< bit:      5                                     */
    uint8_t  READY:1;          /*!< bit:      6                                     */
    uint8_t  OVF:1;            /*!< bit:      7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_INTFLAG_Type;

#define RTC_INTFLAG_OFFSET          0x08         /**< \brief (RTC offset) Interrupt Flag Register */
#define RTC_INTFLAG_RESETVALUE      0x0          /**< \brief (RTC reset_value) Interrupt Flag Register */

#define RTC_INTFLAG_CMP0            (0x1u <<  0) /**< \brief (RTC_INTFLAG)  */
#define RTC_INTFLAG_CMP1            (0x1u <<  1) /**< \brief (RTC_INTFLAG)  */
#define RTC_INTFLAG_CMP2            (0x1u <<  2) /**< \brief (RTC_INTFLAG)  */
#define RTC_INTFLAG_CMP3            (0x1u <<  3) /**< \brief (RTC_INTFLAG)  */
#define RTC_INTFLAG_CMP4            (0x1u <<  4) /**< \brief (RTC_INTFLAG)  */
#define RTC_INTFLAG_CMP5            (0x1u <<  5) /**< \brief (RTC_INTFLAG)  */
#define RTC_INTFLAG_READY           (0x1u <<  6) /**< \brief (RTC_INTFLAG)  */
#define RTC_INTFLAG_OVF             (0x1u <<  7) /**< \brief (RTC_INTFLAG)  */
#define RTC_INTFLAG_MASK            0xFFu /**< \brief (RTC_INTFLAG) MASK Register */

/* -------- RTC_STATUS : (RTC Offset: 0x0A) (R/W  8) Status Register -------- */
typedef union {
  struct {
    uint8_t  RESET:1;          /*!< bit:      0                                     */
    uint8_t  _reserved0:6;     /*!< bit:  1.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_STATUS_Type;

#define RTC_STATUS_OFFSET           0x0A         /**< \brief (RTC offset) Status Register */
#define RTC_STATUS_RESETVALUE       0x0          /**< \brief (RTC reset_value) Status Register */

#define RTC_STATUS_RESET            (0x1u <<  0) /**< \brief (RTC_STATUS)  */
#define RTC_STATUS_SYNCBUSY         (0x1u <<  7) /**< \brief (RTC_STATUS)  */
#define RTC_STATUS_MASK             0x81u /**< \brief (RTC_STATUS) MASK Register */

/* -------- RTC_DEBUG : (RTC Offset: 0x0B) (R/W  8) Debug Register -------- */
typedef union {
  struct {
    uint8_t  DBGRUN:1;         /*!< bit:      0                                     */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_DEBUG_Type;

#define RTC_DEBUG_OFFSET            0x0B         /**< \brief (RTC offset) Debug Register */
#define RTC_DEBUG_RESETVALUE        0x0          /**< \brief (RTC reset_value) Debug Register */

#define RTC_DEBUG_DBGRUN            (0x1u <<  0) /**< \brief (RTC_DEBUG)  */
#define RTC_DEBUG_MASK              0x1u /**< \brief (RTC_DEBUG) MASK Register */

/* -------- RTC_FREQCORR : (RTC Offset: 0x0C) (R/W  8) Frequency Correction Register -------- */
typedef union {
  struct {
    uint8_t  VALUE:7;          /*!< bit:  0.. 6                                     */
    uint8_t  SIGN:1;           /*!< bit:      7                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_FREQCORR_Type;

#define RTC_FREQCORR_OFFSET         0x0C         /**< \brief (RTC offset) Frequency Correction Register */
#define RTC_FREQCORR_RESETVALUE     0x0          /**< \brief (RTC reset_value) Frequency Correction Register */

#define RTC_FREQCORR_VALUE_Pos      0
#define RTC_FREQCORR_VALUE_Msk      (0x7Fu << RTC_FREQCORR_VALUE_Pos) /**< \brief (RTC_FREQCORR)  */
#define RTC_FREQCORR_VALUE(value)   ((RTC_FREQCORR_VALUE_Msk & ((value) << RTC_FREQCORR_VALUE_Pos)))
#define RTC_FREQCORR_SIGN           (0x1u <<  7) /**< \brief (RTC_FREQCORR)  */
#define RTC_FREQCORR_MASK           0xFFu /**< \brief (RTC_FREQCORR) MASK Register */

/* -------- RTC_MODE0_COUNT : (RTC Offset: 0x10) (R/W 32) Mode0 Count Register -------- */
typedef union {
  struct {
    uint32_t COUNT:32;         /*!< bit:  0..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} RTC_MODE0_COUNT_Type;

#define RTC_MODE0_COUNT_OFFSET      0x10         /**< \brief (RTC offset) Count Register */
#define RTC_MODE0_COUNT_RESETVALUE  0x0          /**< \brief (RTC reset_value) Count Register */

#define RTC_MODE0_COUNT_COUNT_Pos    0
#define RTC_MODE0_COUNT_COUNT_Msk    (0xFFFFFFFFu << RTC_MODE0_COUNT_COUNT_Pos) /**< \brief (RTC_MODE0_COUNT)  */
#define RTC_MODE0_COUNT_COUNT(value) ((RTC_MODE0_COUNT_COUNT_Msk & ((value) << RTC_MODE0_COUNT_COUNT_Pos)))
#define RTC_MODE0_COUNT_MASK        0xFFFFFFFFu /**< \brief (RTC_MODE0_COUNT) MASK Register */

/* -------- RTC_MODE1_COUNT : (RTC Offset: 0x10) (R/W 16) Mode1 Count Register -------- */
typedef union {
  struct {
    uint16_t COUNT:16;         /*!< bit:  0..15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE1_COUNT_Type;

#define RTC_MODE1_COUNT_OFFSET      0x10         /**< \brief (RTC offset) Count Register */
#define RTC_MODE1_COUNT_RESETVALUE  0x0          /**< \brief (RTC reset_value) Count Register */

#define RTC_MODE1_COUNT_COUNT_Pos    0
#define RTC_MODE1_COUNT_COUNT_Msk    (0xFFFFu << RTC_MODE1_COUNT_COUNT_Pos) /**< \brief (RTC_MODE1_COUNT)  */
#define RTC_MODE1_COUNT_COUNT(value) ((RTC_MODE1_COUNT_COUNT_Msk & ((value) << RTC_MODE1_COUNT_COUNT_Pos)))
#define RTC_MODE1_COUNT_MASK        0xFFFFu /**< \brief (RTC_MODE1_COUNT) MASK Register */

/* -------- RTC_MODE2_CLOCK : (RTC Offset: 0x10) (R/W 32) Mode2 Clock Register -------- */
typedef union {
  struct {
    uint32_t SECOND:6;         /*!< bit:  0.. 5                                     */
    uint32_t MINUTE:6;         /*!< bit:  6..11                                     */
    uint32_t HOUR:5;           /*!< bit: 12..16                                     */
    uint32_t DAY:5;            /*!< bit: 17..21                                     */
    uint32_t MONTH:4;          /*!< bit: 22..25                                     */
    uint32_t YEAR:6;           /*!< bit: 26..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} RTC_MODE2_CLOCK_Type;

#define RTC_MODE2_CLOCK_OFFSET      0x10         /**< \brief (RTC offset) Clock Register */
#define RTC_MODE2_CLOCK_RESETVALUE  0x0          /**< \brief (RTC reset_value) Clock Register */

#define RTC_MODE2_CLOCK_SECOND_Pos    0
#define RTC_MODE2_CLOCK_SECOND_Msk    (0x3Fu << RTC_MODE2_CLOCK_SECOND_Pos) /**< \brief (RTC_MODE2_CLOCK)  */
#define RTC_MODE2_CLOCK_SECOND(value) ((RTC_MODE2_CLOCK_SECOND_Msk & ((value) << RTC_MODE2_CLOCK_SECOND_Pos)))
#define RTC_MODE2_CLOCK_MINUTE_Pos    6
#define RTC_MODE2_CLOCK_MINUTE_Msk    (0x3Fu << RTC_MODE2_CLOCK_MINUTE_Pos) /**< \brief (RTC_MODE2_CLOCK)  */
#define RTC_MODE2_CLOCK_MINUTE(value) ((RTC_MODE2_CLOCK_MINUTE_Msk & ((value) << RTC_MODE2_CLOCK_MINUTE_Pos)))
#define RTC_MODE2_CLOCK_HOUR_Pos    12
#define RTC_MODE2_CLOCK_HOUR_Msk    (0x1Fu << RTC_MODE2_CLOCK_HOUR_Pos) /**< \brief (RTC_MODE2_CLOCK)  */
#define RTC_MODE2_CLOCK_HOUR(value) ((RTC_MODE2_CLOCK_HOUR_Msk & ((value) << RTC_MODE2_CLOCK_HOUR_Pos)))
#define RTC_MODE2_CLOCK_DAY_Pos     17
#define RTC_MODE2_CLOCK_DAY_Msk     (0x1Fu << RTC_MODE2_CLOCK_DAY_Pos) /**< \brief (RTC_MODE2_CLOCK)  */
#define RTC_MODE2_CLOCK_DAY(value)  ((RTC_MODE2_CLOCK_DAY_Msk & ((value) << RTC_MODE2_CLOCK_DAY_Pos)))
#define RTC_MODE2_CLOCK_MONTH_Pos    22
#define RTC_MODE2_CLOCK_MONTH_Msk    (0xFu << RTC_MODE2_CLOCK_MONTH_Pos) /**< \brief (RTC_MODE2_CLOCK)  */
#define RTC_MODE2_CLOCK_MONTH(value) ((RTC_MODE2_CLOCK_MONTH_Msk & ((value) << RTC_MODE2_CLOCK_MONTH_Pos)))
#define RTC_MODE2_CLOCK_YEAR_Pos    26
#define RTC_MODE2_CLOCK_YEAR_Msk    (0x3Fu << RTC_MODE2_CLOCK_YEAR_Pos) /**< \brief (RTC_MODE2_CLOCK)  */
#define RTC_MODE2_CLOCK_YEAR(value) ((RTC_MODE2_CLOCK_YEAR_Msk & ((value) << RTC_MODE2_CLOCK_YEAR_Pos)))
#define RTC_MODE2_CLOCK_MASK        0xFFFFFFFFu /**< \brief (RTC_MODE2_CLOCK) MASK Register */

/* -------- RTC_MODE1_PER : (RTC Offset: 0x14) (R/W 16) Mode1 Period Register -------- */
typedef union {
  struct {
    uint16_t PER:16;           /*!< bit:  0..15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE1_PER_Type;

#define RTC_MODE1_PER_OFFSET        0x14         /**< \brief (RTC offset) Period Register */
#define RTC_MODE1_PER_RESETVALUE    0x0          /**< \brief (RTC reset_value) Period Register */

#define RTC_MODE1_PER_PER_Pos       0
#define RTC_MODE1_PER_PER_Msk       (0xFFFFu << RTC_MODE1_PER_PER_Pos) /**< \brief (RTC_MODE1_PER)  */
#define RTC_MODE1_PER_PER(value)    ((RTC_MODE1_PER_PER_Msk & ((value) << RTC_MODE1_PER_PER_Pos)))
#define RTC_MODE1_PER_MASK          0xFFFFu /**< \brief (RTC_MODE1_PER) MASK Register */

/* -------- RTC_MODE0_COMP : (RTC Offset: 0x18) (R/W 32) Mode0_comp Compare Register -------- */
typedef union {
  struct {
    uint32_t COMP:32;          /*!< bit:  0..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} RTC_MODE0_COMP_Type;

#define RTC_MODE0_COMP_OFFSET       0x18         /**< \brief (RTC offset) Compare Register */
#define RTC_MODE0_COMP_RESETVALUE   0x0          /**< \brief (RTC reset_value) Compare Register */

#define RTC_MODE0_COMP_COMP_Pos     0
#define RTC_MODE0_COMP_COMP_Msk     (0xFFFFFFFFu << RTC_MODE0_COMP_COMP_Pos) /**< \brief (RTC_MODE0_COMP)  */
#define RTC_MODE0_COMP_COMP(value)  ((RTC_MODE0_COMP_COMP_Msk & ((value) << RTC_MODE0_COMP_COMP_Pos)))
#define RTC_MODE0_COMP_MASK         0xFFFFFFFFu /**< \brief (RTC_MODE0_COMP) MASK Register */

/* -------- RTC_MODE1_COMP : (RTC Offset: 0x18) (R/W 16) Mode1_comp Compare Register -------- */
typedef union {
  struct {
    uint16_t COMP:16;          /*!< bit:  0..15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE1_COMP_Type;

#define RTC_MODE1_COMP_OFFSET       0x18         /**< \brief (RTC offset) Compare Register */
#define RTC_MODE1_COMP_RESETVALUE   0x0          /**< \brief (RTC reset_value) Compare Register */

#define RTC_MODE1_COMP_COMP_Pos     0
#define RTC_MODE1_COMP_COMP_Msk     (0xFFFFu << RTC_MODE1_COMP_COMP_Pos) /**< \brief (RTC_MODE1_COMP)  */
#define RTC_MODE1_COMP_COMP(value)  ((RTC_MODE1_COMP_COMP_Msk & ((value) << RTC_MODE1_COMP_COMP_Pos)))
#define RTC_MODE1_COMP_MASK         0xFFFFu /**< \brief (RTC_MODE1_COMP) MASK Register */

/* -------- RTC_MODE2_ALARM : (RTC Offset: 0x18) (R/W 32) Mode2_alarm Alarm Register -------- */
typedef union {
  struct {
    uint32_t SECOND:6;         /*!< bit:  0.. 5                                     */
    uint32_t MINUTE:6;         /*!< bit:  6..11                                     */
    uint32_t HOUR:5;           /*!< bit: 12..16                                     */
    uint32_t DAY:5;            /*!< bit: 17..21                                     */
    uint32_t MONTH:4;          /*!< bit: 22..25                                     */
    uint32_t YEAR:6;           /*!< bit: 26..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} RTC_MODE2_ALARM_Type;

#define RTC_MODE2_ALARM_OFFSET      0x18         /**< \brief (RTC offset) Alarm Register */
#define RTC_MODE2_ALARM_RESETVALUE  0x0          /**< \brief (RTC reset_value) Alarm Register */

#define RTC_MODE2_ALARM_SECOND_Pos    0
#define RTC_MODE2_ALARM_SECOND_Msk    (0x3Fu << RTC_MODE2_ALARM_SECOND_Pos) /**< \brief (RTC_MODE2_ALARM)  */
#define RTC_MODE2_ALARM_SECOND(value) ((RTC_MODE2_ALARM_SECOND_Msk & ((value) << RTC_MODE2_ALARM_SECOND_Pos)))
#define RTC_MODE2_ALARM_MINUTE_Pos    6
#define RTC_MODE2_ALARM_MINUTE_Msk    (0x3Fu << RTC_MODE2_ALARM_MINUTE_Pos) /**< \brief (RTC_MODE2_ALARM)  */
#define RTC_MODE2_ALARM_MINUTE(value) ((RTC_MODE2_ALARM_MINUTE_Msk & ((value) << RTC_MODE2_ALARM_MINUTE_Pos)))
#define RTC_MODE2_ALARM_HOUR_Pos    12
#define RTC_MODE2_ALARM_HOUR_Msk    (0x1Fu << RTC_MODE2_ALARM_HOUR_Pos) /**< \brief (RTC_MODE2_ALARM)  */
#define RTC_MODE2_ALARM_HOUR(value) ((RTC_MODE2_ALARM_HOUR_Msk & ((value) << RTC_MODE2_ALARM_HOUR_Pos)))
#define RTC_MODE2_ALARM_DAY_Pos     17
#define RTC_MODE2_ALARM_DAY_Msk     (0x1Fu << RTC_MODE2_ALARM_DAY_Pos) /**< \brief (RTC_MODE2_ALARM)  */
#define RTC_MODE2_ALARM_DAY(value)  ((RTC_MODE2_ALARM_DAY_Msk & ((value) << RTC_MODE2_ALARM_DAY_Pos)))
#define RTC_MODE2_ALARM_MONTH_Pos    22
#define RTC_MODE2_ALARM_MONTH_Msk    (0xFu << RTC_MODE2_ALARM_MONTH_Pos) /**< \brief (RTC_MODE2_ALARM)  */
#define RTC_MODE2_ALARM_MONTH(value) ((RTC_MODE2_ALARM_MONTH_Msk & ((value) << RTC_MODE2_ALARM_MONTH_Pos)))
#define RTC_MODE2_ALARM_YEAR_Pos    26
#define RTC_MODE2_ALARM_YEAR_Msk    (0x3Fu << RTC_MODE2_ALARM_YEAR_Pos) /**< \brief (RTC_MODE2_ALARM)  */
#define RTC_MODE2_ALARM_YEAR(value) ((RTC_MODE2_ALARM_YEAR_Msk & ((value) << RTC_MODE2_ALARM_YEAR_Pos)))
#define RTC_MODE2_ALARM_MASK        0xFFFFFFFFu /**< \brief (RTC_MODE2_ALARM) MASK Register */

/* -------- RTC_MODE2_ALARM_MASK : (RTC Offset: 0x1C) (R/W  8) Mode2_alarm Alarm Mask Register -------- */
typedef union {
  struct {
    uint8_t  SEL:3;            /*!< bit:  0.. 2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE2_ALARM_MASK_Type;

#define RTC_MODE2_ALARM_MASK_OFFSET 0x1C         /**< \brief (RTC offset) Alarm Mask Register */
#define RTC_MODE2_ALARM_MASK_RESETVALUE 0x0          /**< \brief (RTC reset_value) Alarm Mask Register */

#define RTC_MODE2_ALARM_MASK_SEL_Pos    0
#define RTC_MODE2_ALARM_MASK_SEL_Msk    (0x7u << RTC_MODE2_ALARM_MASK_SEL_Pos) /**< \brief (RTC_MODE2_ALARM_MASK)  */
#define RTC_MODE2_ALARM_MASK_SEL(value) ((RTC_MODE2_ALARM_MASK_SEL_Msk & ((value) << RTC_MODE2_ALARM_MASK_SEL_Pos)))
#define   RTC_MODE2_ALARM_MASK_SEL_OFF (0x0u <<  0) /**< \brief (RTC_MODE2_ALARM_MASK)  */
#define   RTC_MODE2_ALARM_MASK_SEL_SS (0x1u <<  0) /**< \brief (RTC_MODE2_ALARM_MASK)  */
#define   RTC_MODE2_ALARM_MASK_SEL_MMSS (0x2u <<  0) /**< \brief (RTC_MODE2_ALARM_MASK)  */
#define   RTC_MODE2_ALARM_MASK_SEL_HHMMSS (0x3u <<  0) /**< \brief (RTC_MODE2_ALARM_MASK)  */
#define   RTC_MODE2_ALARM_MASK_SEL_DDHHMMSS (0x4u <<  0) /**< \brief (RTC_MODE2_ALARM_MASK)  */
#define   RTC_MODE2_ALARM_MASK_SEL_MMDDHHMMSS (0x5u <<  0) /**< \brief (RTC_MODE2_ALARM_MASK)  */
#define   RTC_MODE2_ALARM_MASK_SEL_YYMMDDHHMMSS (0x6u <<  0) /**< \brief (RTC_MODE2_ALARM_MASK)  */
#define RTC_MODE2_ALARM_MASK_MASK   0x7u /**< \brief (RTC_MODE2_ALARM_MASK) MASK Register */

/** \brief RtcMode2Alarm hardware registers */
typedef struct {
  __IO RTC_MODE2_ALARM_Type      ALARM;       /**< \brief Offset: 0x00 (R/W 32) Alarm Register */
  __IO RTC_MODE2_ALARM_MASK_Type MASK;        /**< \brief Offset: 0x04 (R/W  8) Alarm Mask Register */
       RoReg8                    Reserved1[0x3];
} RtcMode2Alarm;

/** \brief RtcMode0 hardware registers */
typedef struct {
  __IO RTC_MODE0_COUNT_Type      COUNT;       /**< \brief Offset: 0x00 (R/W 32) Count Register */
       RoReg8                    Reserved1[0x4];
  __IO RTC_MODE0_COMP_Type       COMP[4];     /**< \brief Offset: 0x08 (R/W 32) Compare Register */
       RoReg8                    Reserved2[0x10];
} RtcMode0;

/** \brief RtcMode1 hardware registers */
typedef struct {
  __IO RTC_MODE1_COUNT_Type      COUNT;       /**< \brief Offset: 0x00 (R/W 16) Count Register */
       RoReg8                    Reserved1[0x2];
  __IO RTC_MODE1_PER_Type        PER;         /**< \brief Offset: 0x04 (R/W 16) Period Register */
       RoReg8                    Reserved2[0x2];
  __IO RTC_MODE1_COMP_Type       COMP[6];     /**< \brief Offset: 0x08 (R/W 16) Compare Register */
       RoReg8                    Reserved3[0x14];
} RtcMode1;

/** \brief RtcMode2 hardware registers */
typedef struct {
  __IO RTC_MODE2_CLOCK_Type      CLOCK;       /**< \brief Offset: 0x00 (R/W 32) Clock Register */
       RoReg8                    Reserved1[0x4];
       RtcMode2Alarm             Alarm[4];    /**< \brief Offset: 0x18 RtcMode2Alarm groups */
} RtcMode2;

/** \brief RTC hardware registers */
typedef struct {
  __IO RTC_CTRL_Type             CTRL;        /**< \brief Offset: 0x00 (R/W 16) Control Register */
  __IO RTC_READREQ_Type          READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request Register */
  __IO RTC_EVCTRL_Type           EVCTRL;      /**< \brief Offset: 0x04 (R/W 16) Event Control Register */
  __IO RTC_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x06 (R/W  8) Interrupt Enable Clear Register */
  __IO RTC_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x07 (R/W  8) Interrupt Enable Set Register */
  __IO RTC_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x08 (R/W  8) Interrupt Flag Register */
       RoReg8                    Reserved1[0x1];
  __IO RTC_STATUS_Type           STATUS;      /**< \brief Offset: 0x0A (R/W  8) Status Register */
  __IO RTC_DEBUG_Type            DEBUG;       /**< \brief Offset: 0x0B (R/W  8) Debug Register */
  __IO RTC_FREQCORR_Type         FREQCORR;    /**< \brief Offset: 0x0C (R/W  8) Frequency Correction Register */
       RoReg8                    Reserved2[0x3];
 union { // Overlapping sections
       RtcMode0                  Mode0;       /**< \brief Offset: 0x10 RtcMode0 groups */
       RtcMode1                  Mode1;       /**< \brief Offset: 0x10 RtcMode1 groups */
       RtcMode2                  Mode2;       /**< \brief Offset: 0x10 RtcMode2 groups */
 };
} Rtc;

#endif /*  __EXCLUDE_RTC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR SERCOM */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_SERCOM Serial Communication Interface */
/*@{*/

#define REV_SERCOM     0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_SERCOM_FROM_CMSIS__

/* -------- SERCOM_CTRLA : (SERCOM Offset: 0x00) (R/W 32) Control Register A -------- */
typedef union {
  struct { // SPI mode
    uint32_t SWRST:1;          /*!< bit:      0                                     */
    uint32_t ENABLE:1;         /*!< bit:      1                                     */
    uint32_t MODE:2;           /*!< bit:  2.. 3                                     */
    uint32_t _reserved0:3;     /*!< bit:  4.. 6  Reserved                           */
    uint32_t SLEEPEN:1;        /*!< bit:      7                                     */
    uint32_t _reserved1:8;     /*!< bit:  8..15  Reserved                           */
    uint32_t DOPO:1;           /*!< bit:     16                                     */
    uint32_t _reserved2:3;     /*!< bit: 17..19  Reserved                           */
    uint32_t DIPO:2;           /*!< bit: 20..21                                     */
    uint32_t _reserved3:2;     /*!< bit: 22..23  Reserved                           */
    uint32_t FORM:4;           /*!< bit: 24..27                                     */
    uint32_t CPHA:1;           /*!< bit:     28                                     */
    uint32_t CPOL:1;           /*!< bit:     29                                     */
    uint32_t DORD:1;           /*!< bit:     30                                     */
    uint32_t MASTER:1;         /*!< bit:     31                                     */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint32_t SWRST:1;          /*!< bit:      0                                     */
    uint32_t ENABLE:1;         /*!< bit:      1                                     */
    uint32_t MODE:2;           /*!< bit:  2.. 3                                     */
    uint32_t _reserved0:3;     /*!< bit:  4.. 6  Reserved                           */
    uint32_t SLEEPEN:1;        /*!< bit:      7                                     */
    uint32_t _reserved1:8;     /*!< bit:  8..15  Reserved                           */
    uint32_t PINOUT:1;         /*!< bit:     16                                     */
    uint32_t _reserved2:3;     /*!< bit: 17..19  Reserved                           */
    uint32_t SDAHOLD:2;        /*!< bit: 20..21                                     */
    uint32_t _reserved3:6;     /*!< bit: 22..27  Reserved                           */
    uint32_t INACTOUT:2;       /*!< bit: 28..29                                     */
    uint32_t LOWTOUTEN:1;      /*!< bit:     30                                     */
    uint32_t MASTER:1;         /*!< bit:     31                                     */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint32_t SWRST:1;          /*!< bit:      0                                     */
    uint32_t ENABLE:1;         /*!< bit:      1                                     */
    uint32_t MODE:2;           /*!< bit:  2.. 3                                     */
    uint32_t _reserved0:3;     /*!< bit:  4.. 6  Reserved                           */
    uint32_t SLEEPEN:1;        /*!< bit:      7                                     */
    uint32_t _reserved1:8;     /*!< bit:  8..15  Reserved                           */
    uint32_t PINOUT:1;         /*!< bit:     16                                     */
    uint32_t _reserved2:3;     /*!< bit: 17..19  Reserved                           */
    uint32_t SDAHOLD:2;        /*!< bit: 20..21                                     */
    uint32_t _reserved3:8;     /*!< bit: 22..29  Reserved                           */
    uint32_t LOWTOUT:1;        /*!< bit:     30                                     */
    uint32_t MASTER:1;         /*!< bit:     31                                     */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  struct { // USART mode
    uint32_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint32_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint32_t MODE:2;           /*!< bit:  2.. 3                                     */
    uint32_t _reserved0:3;     /*!< bit:  4.. 6  Reserved                           */
    uint32_t SLEEPEN:1;        /*!< bit:      7                                     */
    uint32_t _reserved1:8;     /*!< bit:  8..15  Reserved                           */
    uint32_t TXPO:1;           /*!< bit:     16                                     */
    uint32_t _reserved2:3;     /*!< bit: 17..19  Reserved                           */
    uint32_t RXPO:2;           /*!< bit: 20..21                                     */
    uint32_t _reserved3:2;     /*!< bit: 22..23  Reserved                           */
    uint32_t FORM:2;           /*!< bit: 24..25                                     */
    uint32_t _reserved4:2;     /*!< bit: 26..27  Reserved                           */
    uint32_t CMODE:1;          /*!< bit:     28                                     */
    uint32_t CPOL:1;           /*!< bit:     29                                     */
    uint32_t DORD:1;           /*!< bit:     30                                     */
    uint32_t CSRC:1;           /*!< bit:     31                                     */
  } USART;                     /*!< Structure used for USART                        */
  uint32_t reg;                /*!< Type      used for register access              */
} SERCOM_CTRLA_Type;

#define SERCOM_CTRLA_OFFSET         0x00         /**< \brief (SERCOM offset) Control Register A */
#define SERCOM_CTRLA_RESETVALUE     0x0          /**< \brief (SERCOM reset_value) Control Register A */

// SPI mode
#define SERCOM_CTRLA_SPI_SWRST      (0x1u <<  0) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_ENABLE     (0x1u <<  1) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_MODE_Pos    2
#define SERCOM_CTRLA_SPI_MODE_Msk    (0x3u << SERCOM_CTRLA_SPI_MODE_Pos) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_MODE(value) ((SERCOM_CTRLA_SPI_MODE_Msk & ((value) << SERCOM_CTRLA_SPI_MODE_Pos)))
#define SERCOM_CTRLA_SPI_SLEEPEN    (0x1u <<  7) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_DOPO       (0x1u << 16) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_DIPO_Pos    20
#define SERCOM_CTRLA_SPI_DIPO_Msk    (0x3u << SERCOM_CTRLA_SPI_DIPO_Pos) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_DIPO(value) ((SERCOM_CTRLA_SPI_DIPO_Msk & ((value) << SERCOM_CTRLA_SPI_DIPO_Pos)))
#define SERCOM_CTRLA_SPI_FORM_Pos    24
#define SERCOM_CTRLA_SPI_FORM_Msk    (0xFu << SERCOM_CTRLA_SPI_FORM_Pos) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_FORM(value) ((SERCOM_CTRLA_SPI_FORM_Msk & ((value) << SERCOM_CTRLA_SPI_FORM_Pos)))
#define SERCOM_CTRLA_SPI_CPHA       (0x1u << 28) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_CPOL       (0x1u << 29) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_DORD       (0x1u << 30) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_MASTER     (0x1u << 31) /**< \brief (SERCOM_CTRLA_SPI)  */
#define SERCOM_CTRLA_SPI_MASK       0xFF31008Fu /**< \brief (SERCOM_CTRLA_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_CTRLA_TWI_MASTER_SWRST (0x1u <<  0) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_ENABLE (0x1u <<  1) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_MODE_Pos    2
#define SERCOM_CTRLA_TWI_MASTER_MODE_Msk    (0x3u << SERCOM_CTRLA_TWI_MASTER_MODE_Pos) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_MODE(value) ((SERCOM_CTRLA_TWI_MASTER_MODE_Msk & ((value) << SERCOM_CTRLA_TWI_MASTER_MODE_Pos)))
#define SERCOM_CTRLA_TWI_MASTER_SLEEPEN (0x1u <<  7) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_PINOUT (0x1u << 16) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_SDAHOLD_Pos    20
#define SERCOM_CTRLA_TWI_MASTER_SDAHOLD_Msk    (0x3u << SERCOM_CTRLA_TWI_MASTER_SDAHOLD_Pos) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_SDAHOLD(value) ((SERCOM_CTRLA_TWI_MASTER_SDAHOLD_Msk & ((value) << SERCOM_CTRLA_TWI_MASTER_SDAHOLD_Pos)))
#define SERCOM_CTRLA_TWI_MASTER_INACTOUT_Pos    28
#define SERCOM_CTRLA_TWI_MASTER_INACTOUT_Msk    (0x3u << SERCOM_CTRLA_TWI_MASTER_INACTOUT_Pos) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_INACTOUT(value) ((SERCOM_CTRLA_TWI_MASTER_INACTOUT_Msk & ((value) << SERCOM_CTRLA_TWI_MASTER_INACTOUT_Pos)))
#define SERCOM_CTRLA_TWI_MASTER_LOWTOUTEN (0x1u << 30) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_MASTER (0x1u << 31) /**< \brief (SERCOM_CTRLA_TWI_MASTER)  */
#define SERCOM_CTRLA_TWI_MASTER_MASK 0xF031008Fu /**< \brief (SERCOM_CTRLA_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_CTRLA_TWI_SLAVE_SWRST (0x1u <<  0) /**< \brief (SERCOM_CTRLA_TWI_SLAVE)  */
#define SERCOM_CTRLA_TWI_SLAVE_ENABLE (0x1u <<  1) /**< \brief (SERCOM_CTRLA_TWI_SLAVE)  */
#define SERCOM_CTRLA_TWI_SLAVE_MODE_Pos    2
#define SERCOM_CTRLA_TWI_SLAVE_MODE_Msk    (0x3u << SERCOM_CTRLA_TWI_SLAVE_MODE_Pos) /**< \brief (SERCOM_CTRLA_TWI_SLAVE)  */
#define SERCOM_CTRLA_TWI_SLAVE_MODE(value) ((SERCOM_CTRLA_TWI_SLAVE_MODE_Msk & ((value) << SERCOM_CTRLA_TWI_SLAVE_MODE_Pos)))
#define SERCOM_CTRLA_TWI_SLAVE_SLEEPEN (0x1u <<  7) /**< \brief (SERCOM_CTRLA_TWI_SLAVE)  */
#define SERCOM_CTRLA_TWI_SLAVE_PINOUT (0x1u << 16) /**< \brief (SERCOM_CTRLA_TWI_SLAVE)  */
#define SERCOM_CTRLA_TWI_SLAVE_SDAHOLD_Pos    20
#define SERCOM_CTRLA_TWI_SLAVE_SDAHOLD_Msk    (0x3u << SERCOM_CTRLA_TWI_SLAVE_SDAHOLD_Pos) /**< \brief (SERCOM_CTRLA_TWI_SLAVE)  */
#define SERCOM_CTRLA_TWI_SLAVE_SDAHOLD(value) ((SERCOM_CTRLA_TWI_SLAVE_SDAHOLD_Msk & ((value) << SERCOM_CTRLA_TWI_SLAVE_SDAHOLD_Pos)))
#define SERCOM_CTRLA_TWI_SLAVE_LOWTOUT (0x1u << 30) /**< \brief (SERCOM_CTRLA_TWI_SLAVE)  */
#define SERCOM_CTRLA_TWI_SLAVE_MASTER (0x1u << 31) /**< \brief (SERCOM_CTRLA_TWI_SLAVE)  */
#define SERCOM_CTRLA_TWI_SLAVE_MASK 0xC031008Fu /**< \brief (SERCOM_CTRLA_TWI_SLAVE) MASK Register */

// USART mode
#define SERCOM_CTRLA_USART_SWRST    (0x1u <<  0) /**< \brief (SERCOM_CTRLA_USART) Software Reset */
#define SERCOM_CTRLA_USART_ENABLE   (0x1u <<  1) /**< \brief (SERCOM_CTRLA_USART) Enable */
#define SERCOM_CTRLA_USART_MODE_Pos    2
#define SERCOM_CTRLA_USART_MODE_Msk    (0x3u << SERCOM_CTRLA_USART_MODE_Pos) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_MODE(value) ((SERCOM_CTRLA_USART_MODE_Msk & ((value) << SERCOM_CTRLA_USART_MODE_Pos)))
#define SERCOM_CTRLA_USART_SLEEPEN  (0x1u <<  7) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_TXPO     (0x1u << 16) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_RXPO_Pos    20
#define SERCOM_CTRLA_USART_RXPO_Msk    (0x3u << SERCOM_CTRLA_USART_RXPO_Pos) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_RXPO(value) ((SERCOM_CTRLA_USART_RXPO_Msk & ((value) << SERCOM_CTRLA_USART_RXPO_Pos)))
#define SERCOM_CTRLA_USART_FORM_Pos    24
#define SERCOM_CTRLA_USART_FORM_Msk    (0x3u << SERCOM_CTRLA_USART_FORM_Pos) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_FORM(value) ((SERCOM_CTRLA_USART_FORM_Msk & ((value) << SERCOM_CTRLA_USART_FORM_Pos)))
#define SERCOM_CTRLA_USART_CMODE    (0x1u << 28) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_CPOL     (0x1u << 29) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_DORD     (0x1u << 30) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_CSRC     (0x1u << 31) /**< \brief (SERCOM_CTRLA_USART)  */
#define SERCOM_CTRLA_USART_MASK     0xF331008Fu /**< \brief (SERCOM_CTRLA_USART) MASK Register */

/* -------- SERCOM_CTRLB : (SERCOM Offset: 0x04) (R/W 32) Control Register B -------- */
typedef union {
  struct { // SPI mode
    uint32_t CHSIZE:1;         /*!< bit:      0                                     */
    uint32_t _reserved0:5;     /*!< bit:  1.. 5  Reserved                           */
    uint32_t PLOADEN:1;        /*!< bit:      6                                     */
    uint32_t _reserved1:7;     /*!< bit:  7..13  Reserved                           */
    uint32_t AMODE:2;          /*!< bit: 14..15                                     */
    uint32_t _reserved2:1;     /*!< bit:     16  Reserved                           */
    uint32_t RXEN:1;           /*!< bit:     17                                     */
    uint32_t _reserved3:14;    /*!< bit: 18..31  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint32_t _reserved0:8;     /*!< bit:  0.. 7  Reserved                           */
    uint32_t SMEN:1;           /*!< bit:      8                                     */
    uint32_t QCEN:1;           /*!< bit:      9                                     */
    uint32_t _reserved1:6;     /*!< bit: 10..15  Reserved                           */
    uint32_t CMD:2;            /*!< bit: 16..17                                     */
    uint32_t ACKACT:1;         /*!< bit:     18                                     */
    uint32_t _reserved2:13;    /*!< bit: 19..31  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint32_t _reserved0:8;     /*!< bit:  0.. 7  Reserved                           */
    uint32_t SMEN:1;           /*!< bit:      8                                     */
    uint32_t _reserved1:5;     /*!< bit:  9..13  Reserved                           */
    uint32_t AMODE:2;          /*!< bit: 14..15                                     */
    uint32_t CMD:2;            /*!< bit: 16..17                                     */
    uint32_t ACKACT:1;         /*!< bit:     18                                     */
    uint32_t _reserved2:13;    /*!< bit: 19..31  Reserved                           */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  struct { // USART mode
    uint32_t CHSIZE:3;         /*!< bit:  0.. 2                                     */
    uint32_t _reserved0:3;     /*!< bit:  3.. 5  Reserved                           */
    uint32_t SBMODE:1;         /*!< bit:      6                                     */
    uint32_t _reserved1:2;     /*!< bit:  7.. 8  Reserved                           */
    uint32_t SFDE:1;           /*!< bit:      9                                     */
    uint32_t _reserved2:3;     /*!< bit: 10..12  Reserved                           */
    uint32_t PMODE:1;          /*!< bit:     13                                     */
    uint32_t _reserved3:2;     /*!< bit: 14..15  Reserved                           */
    uint32_t TXEN:1;           /*!< bit:     16                                     */
    uint32_t RXEN:1;           /*!< bit:     17                                     */
    uint32_t _reserved4:14;    /*!< bit: 18..31  Reserved                           */
  } USART;                     /*!< Structure used for USART                        */
  uint32_t reg;                /*!< Type      used for register access              */
} SERCOM_CTRLB_Type;

#define SERCOM_CTRLB_OFFSET         0x04         /**< \brief (SERCOM offset) Control Register B */
#define SERCOM_CTRLB_RESETVALUE     0x0          /**< \brief (SERCOM reset_value) Control Register B */

// SPI mode
#define SERCOM_CTRLB_SPI_CHSIZE     (0x1u <<  0) /**< \brief (SERCOM_CTRLB_SPI)  */
#define SERCOM_CTRLB_SPI_PLOADEN    (0x1u <<  6) /**< \brief (SERCOM_CTRLB_SPI)  */
#define SERCOM_CTRLB_SPI_AMODE_Pos    14
#define SERCOM_CTRLB_SPI_AMODE_Msk    (0x3u << SERCOM_CTRLB_SPI_AMODE_Pos) /**< \brief (SERCOM_CTRLB_SPI)  */
#define SERCOM_CTRLB_SPI_AMODE(value) ((SERCOM_CTRLB_SPI_AMODE_Msk & ((value) << SERCOM_CTRLB_SPI_AMODE_Pos)))
#define SERCOM_CTRLB_SPI_RXEN       (0x1u << 17) /**< \brief (SERCOM_CTRLB_SPI)  */
#define SERCOM_CTRLB_SPI_MASK       0x2C041u /**< \brief (SERCOM_CTRLB_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_CTRLB_TWI_MASTER_SMEN (0x1u <<  8) /**< \brief (SERCOM_CTRLB_TWI_MASTER)  */
#define SERCOM_CTRLB_TWI_MASTER_QCEN (0x1u <<  9) /**< \brief (SERCOM_CTRLB_TWI_MASTER)  */
#define SERCOM_CTRLB_TWI_MASTER_CMD_Pos    16
#define SERCOM_CTRLB_TWI_MASTER_CMD_Msk    (0x3u << SERCOM_CTRLB_TWI_MASTER_CMD_Pos) /**< \brief (SERCOM_CTRLB_TWI_MASTER)  */
#define SERCOM_CTRLB_TWI_MASTER_CMD(value) ((SERCOM_CTRLB_TWI_MASTER_CMD_Msk & ((value) << SERCOM_CTRLB_TWI_MASTER_CMD_Pos)))
#define SERCOM_CTRLB_TWI_MASTER_ACKACT (0x1u << 18) /**< \brief (SERCOM_CTRLB_TWI_MASTER)  */
#define SERCOM_CTRLB_TWI_MASTER_MASK 0x70300u /**< \brief (SERCOM_CTRLB_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_CTRLB_TWI_SLAVE_SMEN (0x1u <<  8) /**< \brief (SERCOM_CTRLB_TWI_SLAVE)  */
#define SERCOM_CTRLB_TWI_SLAVE_AMODE_Pos    14
#define SERCOM_CTRLB_TWI_SLAVE_AMODE_Msk    (0x3u << SERCOM_CTRLB_TWI_SLAVE_AMODE_Pos) /**< \brief (SERCOM_CTRLB_TWI_SLAVE)  */
#define SERCOM_CTRLB_TWI_SLAVE_AMODE(value) ((SERCOM_CTRLB_TWI_SLAVE_AMODE_Msk & ((value) << SERCOM_CTRLB_TWI_SLAVE_AMODE_Pos)))
#define SERCOM_CTRLB_TWI_SLAVE_CMD_Pos    16
#define SERCOM_CTRLB_TWI_SLAVE_CMD_Msk    (0x3u << SERCOM_CTRLB_TWI_SLAVE_CMD_Pos) /**< \brief (SERCOM_CTRLB_TWI_SLAVE)  */
#define SERCOM_CTRLB_TWI_SLAVE_CMD(value) ((SERCOM_CTRLB_TWI_SLAVE_CMD_Msk & ((value) << SERCOM_CTRLB_TWI_SLAVE_CMD_Pos)))
#define SERCOM_CTRLB_TWI_SLAVE_ACKACT (0x1u << 18) /**< \brief (SERCOM_CTRLB_TWI_SLAVE)  */
#define SERCOM_CTRLB_TWI_SLAVE_MASK 0x7C100u /**< \brief (SERCOM_CTRLB_TWI_SLAVE) MASK Register */

// USART mode
#define SERCOM_CTRLB_USART_CHSIZE_Pos    0
#define SERCOM_CTRLB_USART_CHSIZE_Msk    (0x7u << SERCOM_CTRLB_USART_CHSIZE_Pos) /**< \brief (SERCOM_CTRLB_USART)  */
#define SERCOM_CTRLB_USART_CHSIZE(value) ((SERCOM_CTRLB_USART_CHSIZE_Msk & ((value) << SERCOM_CTRLB_USART_CHSIZE_Pos)))
#define SERCOM_CTRLB_USART_SBMODE   (0x1u <<  6) /**< \brief (SERCOM_CTRLB_USART)  */
#define SERCOM_CTRLB_USART_SFDE     (0x1u <<  9) /**< \brief (SERCOM_CTRLB_USART)  */
#define SERCOM_CTRLB_USART_PMODE    (0x1u << 13) /**< \brief (SERCOM_CTRLB_USART)  */
#define SERCOM_CTRLB_USART_TXEN     (0x1u << 16) /**< \brief (SERCOM_CTRLB_USART)  */
#define SERCOM_CTRLB_USART_RXEN     (0x1u << 17) /**< \brief (SERCOM_CTRLB_USART)  */
#define SERCOM_CTRLB_USART_MASK     0x32247u /**< \brief (SERCOM_CTRLB_USART) MASK Register */

/* -------- SERCOM_DEBUG : (SERCOM Offset: 0x08) (R/W  8) Debug Register -------- */
typedef union {
  struct { // SPI mode
    uint8_t  DBGSTOP:1;        /*!< bit:      0                                     */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint8_t  DBGSTOP:1;        /*!< bit:      0                                     */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint8_t  DBGSTOP:1;        /*!< bit:      0                                     */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  struct { // USART mode
    uint8_t  DBGSTOP:1;        /*!< bit:      0                                     */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } USART;                     /*!< Structure used for USART                        */
  uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_DEBUG_Type;

#define SERCOM_DEBUG_OFFSET         0x08         /**< \brief (SERCOM offset) Debug Register */
#define SERCOM_DEBUG_RESETVALUE     0x0          /**< \brief (SERCOM reset_value) Debug Register */

// SPI mode
#define SERCOM_DEBUG_SPI_DBGSTOP    (0x1u <<  0) /**< \brief (SERCOM_DEBUG_SPI)  */
#define SERCOM_DEBUG_SPI_MASK       0x1u /**< \brief (SERCOM_DEBUG_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_DEBUG_TWI_MASTER_DBGSTOP (0x1u <<  0) /**< \brief (SERCOM_DEBUG_TWI_MASTER)  */
#define SERCOM_DEBUG_TWI_MASTER_MASK 0x1u /**< \brief (SERCOM_DEBUG_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_DEBUG_TWI_SLAVE_DBGSTOP (0x1u <<  0) /**< \brief (SERCOM_DEBUG_TWI_SLAVE)  */
#define SERCOM_DEBUG_TWI_SLAVE_MASK 0x1u /**< \brief (SERCOM_DEBUG_TWI_SLAVE) MASK Register */

// USART mode
#define SERCOM_DEBUG_USART_DBGSTOP  (0x1u <<  0) /**< \brief (SERCOM_DEBUG_USART)  */
#define SERCOM_DEBUG_USART_MASK     0x1u /**< \brief (SERCOM_DEBUG_USART) MASK Register */

/* -------- SERCOM_BAUD : (SERCOM Offset: 0x0A) (R/W 16) Baud Rate Register -------- */
typedef union {
  struct { // SPI mode
    uint16_t BAUD:8;           /*!< bit:  0.. 7                                     */
    uint16_t _reserved0:8;     /*!< bit:  8..15  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint16_t BAUD:8;           /*!< bit:  0.. 7                                     */
    uint16_t _reserved0:8;     /*!< bit:  8..15  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // USART mode
    uint16_t BAUD:16;          /*!< bit:  0..15                                     */
  } USART;                     /*!< Structure used for USART                        */
  uint16_t reg;                /*!< Type      used for register access              */
} SERCOM_BAUD_Type;

#define SERCOM_BAUD_OFFSET          0x0A         /**< \brief (SERCOM offset) Baud Rate Register */
#define SERCOM_BAUD_RESETVALUE      0x0          /**< \brief (SERCOM reset_value) Baud Rate Register */

// SPI mode
#define SERCOM_BAUD_SPI_BAUD_Pos    0
#define SERCOM_BAUD_SPI_BAUD_Msk    (0xFFu << SERCOM_BAUD_SPI_BAUD_Pos) /**< \brief (SERCOM_BAUD_SPI)  */
#define SERCOM_BAUD_SPI_BAUD(value) ((SERCOM_BAUD_SPI_BAUD_Msk & ((value) << SERCOM_BAUD_SPI_BAUD_Pos)))
#define SERCOM_BAUD_SPI_MASK        0xFFu /**< \brief (SERCOM_BAUD_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_BAUD_TWI_MASTER_BAUD_Pos    0
#define SERCOM_BAUD_TWI_MASTER_BAUD_Msk    (0xFFu << SERCOM_BAUD_TWI_MASTER_BAUD_Pos) /**< \brief (SERCOM_BAUD_TWI_MASTER)  */
#define SERCOM_BAUD_TWI_MASTER_BAUD(value) ((SERCOM_BAUD_TWI_MASTER_BAUD_Msk & ((value) << SERCOM_BAUD_TWI_MASTER_BAUD_Pos)))
#define SERCOM_BAUD_TWI_MASTER_MASK 0xFFu /**< \brief (SERCOM_BAUD_TWI_MASTER) MASK Register */

// USART mode
#define SERCOM_BAUD_USART_BAUD_Pos    0
#define SERCOM_BAUD_USART_BAUD_Msk    (0xFFFFu << SERCOM_BAUD_USART_BAUD_Pos) /**< \brief (SERCOM_BAUD_USART)  */
#define SERCOM_BAUD_USART_BAUD(value) ((SERCOM_BAUD_USART_BAUD_Msk & ((value) << SERCOM_BAUD_USART_BAUD_Pos)))
#define SERCOM_BAUD_USART_MASK      0xFFFFu /**< \brief (SERCOM_BAUD_USART) MASK Register */

/* -------- SERCOM_INTENCLR : (SERCOM Offset: 0x0C) (R/W  8) Interrupt Enable Clear Register -------- */
typedef union {
  struct { // SPI mode
    uint8_t  DREIEN:1;         /*!< bit:      0                                     */
    uint8_t  TXCIEN:1;         /*!< bit:      1                                     */
    uint8_t  RXCIEN:1;         /*!< bit:      2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint8_t  WIEN:1;           /*!< bit:      0                                     */
    uint8_t  RIEN:1;           /*!< bit:      1                                     */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint8_t  PIEN:1;           /*!< bit:      0                                     */
    uint8_t  AIEN:1;           /*!< bit:      1                                     */
    uint8_t  DIEN:1;           /*!< bit:      2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  struct { // USART mode
    uint8_t  DREIEN:1;         /*!< bit:      0                                     */
    uint8_t  TXCIEN:1;         /*!< bit:      1                                     */
    uint8_t  RXCIEN:1;         /*!< bit:      2                                     */
    uint8_t  RXSIEN:1;         /*!< bit:      3                                     */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } USART;                     /*!< Structure used for USART                        */
  uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_INTENCLR_Type;

#define SERCOM_INTENCLR_OFFSET      0x0C         /**< \brief (SERCOM offset) Interrupt Enable Clear Register */
#define SERCOM_INTENCLR_RESETVALUE  0x0          /**< \brief (SERCOM reset_value) Interrupt Enable Clear Register */

// SPI mode
#define SERCOM_INTENCLR_SPI_DREIEN  (0x1u <<  0) /**< \brief (SERCOM_INTENCLR_SPI)  */
#define SERCOM_INTENCLR_SPI_TXCIEN  (0x1u <<  1) /**< \brief (SERCOM_INTENCLR_SPI)  */
#define SERCOM_INTENCLR_SPI_RXCIEN  (0x1u <<  2) /**< \brief (SERCOM_INTENCLR_SPI)  */
#define SERCOM_INTENCLR_SPI_MASK    0x7u /**< \brief (SERCOM_INTENCLR_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_INTENCLR_TWI_MASTER_WIEN (0x1u <<  0) /**< \brief (SERCOM_INTENCLR_TWI_MASTER)  */
#define SERCOM_INTENCLR_TWI_MASTER_RIEN (0x1u <<  1) /**< \brief (SERCOM_INTENCLR_TWI_MASTER)  */
#define SERCOM_INTENCLR_TWI_MASTER_MASK 0x3u /**< \brief (SERCOM_INTENCLR_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_INTENCLR_TWI_SLAVE_PIEN (0x1u <<  0) /**< \brief (SERCOM_INTENCLR_TWI_SLAVE)  */
#define SERCOM_INTENCLR_TWI_SLAVE_AIEN (0x1u <<  1) /**< \brief (SERCOM_INTENCLR_TWI_SLAVE)  */
#define SERCOM_INTENCLR_TWI_SLAVE_DIEN (0x1u <<  2) /**< \brief (SERCOM_INTENCLR_TWI_SLAVE)  */
#define SERCOM_INTENCLR_TWI_SLAVE_MASK 0x7u /**< \brief (SERCOM_INTENCLR_TWI_SLAVE) MASK Register */

// USART mode
#define SERCOM_INTENCLR_USART_DREIEN (0x1u <<  0) /**< \brief (SERCOM_INTENCLR_USART)  */
#define SERCOM_INTENCLR_USART_TXCIEN (0x1u <<  1) /**< \brief (SERCOM_INTENCLR_USART)  */
#define SERCOM_INTENCLR_USART_RXCIEN (0x1u <<  2) /**< \brief (SERCOM_INTENCLR_USART)  */
#define SERCOM_INTENCLR_USART_RXSIEN (0x1u <<  3) /**< \brief (SERCOM_INTENCLR_USART)  */
#define SERCOM_INTENCLR_USART_MASK  0xFu /**< \brief (SERCOM_INTENCLR_USART) MASK Register */

/* -------- SERCOM_INTENSET : (SERCOM Offset: 0x0D) (R/W  8) Interrupt Enable Set Register -------- */
typedef union {
  struct { // SPI mode
    uint8_t  DREIEN:1;         /*!< bit:      0                                     */
    uint8_t  TXCIEN:1;         /*!< bit:      1                                     */
    uint8_t  RXCIEN:1;         /*!< bit:      2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint8_t  WIEN:1;           /*!< bit:      0                                     */
    uint8_t  RIEN:1;           /*!< bit:      1                                     */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint8_t  PIEN:1;           /*!< bit:      0                                     */
    uint8_t  AIEN:1;           /*!< bit:      1                                     */
    uint8_t  DIEN:1;           /*!< bit:      2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  struct { // USART mode
    uint8_t  DREIEN:1;         /*!< bit:      0                                     */
    uint8_t  TXCIEN:1;         /*!< bit:      1                                     */
    uint8_t  RXCIEN:1;         /*!< bit:      2                                     */
    uint8_t  RXSIEN:1;         /*!< bit:      3                                     */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } USART;                     /*!< Structure used for USART                        */
  uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_INTENSET_Type;

#define SERCOM_INTENSET_OFFSET      0x0D         /**< \brief (SERCOM offset) Interrupt Enable Set Register */
#define SERCOM_INTENSET_RESETVALUE  0x0          /**< \brief (SERCOM reset_value) Interrupt Enable Set Register */

// SPI mode
#define SERCOM_INTENSET_SPI_DREIEN  (0x1u <<  0) /**< \brief (SERCOM_INTENSET_SPI)  */
#define SERCOM_INTENSET_SPI_TXCIEN  (0x1u <<  1) /**< \brief (SERCOM_INTENSET_SPI)  */
#define SERCOM_INTENSET_SPI_RXCIEN  (0x1u <<  2) /**< \brief (SERCOM_INTENSET_SPI)  */
#define SERCOM_INTENSET_SPI_MASK    0x7u /**< \brief (SERCOM_INTENSET_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_INTENSET_TWI_MASTER_WIEN (0x1u <<  0) /**< \brief (SERCOM_INTENSET_TWI_MASTER)  */
#define SERCOM_INTENSET_TWI_MASTER_RIEN (0x1u <<  1) /**< \brief (SERCOM_INTENSET_TWI_MASTER)  */
#define SERCOM_INTENSET_TWI_MASTER_MASK 0x3u /**< \brief (SERCOM_INTENSET_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_INTENSET_TWI_SLAVE_PIEN (0x1u <<  0) /**< \brief (SERCOM_INTENSET_TWI_SLAVE)  */
#define SERCOM_INTENSET_TWI_SLAVE_AIEN (0x1u <<  1) /**< \brief (SERCOM_INTENSET_TWI_SLAVE)  */
#define SERCOM_INTENSET_TWI_SLAVE_DIEN (0x1u <<  2) /**< \brief (SERCOM_INTENSET_TWI_SLAVE)  */
#define SERCOM_INTENSET_TWI_SLAVE_MASK 0x7u /**< \brief (SERCOM_INTENSET_TWI_SLAVE) MASK Register */

// USART mode
#define SERCOM_INTENSET_USART_DREIEN (0x1u <<  0) /**< \brief (SERCOM_INTENSET_USART)  */
#define SERCOM_INTENSET_USART_TXCIEN (0x1u <<  1) /**< \brief (SERCOM_INTENSET_USART)  */
#define SERCOM_INTENSET_USART_RXCIEN (0x1u <<  2) /**< \brief (SERCOM_INTENSET_USART)  */
#define SERCOM_INTENSET_USART_RXSIEN (0x1u <<  3) /**< \brief (SERCOM_INTENSET_USART)  */
#define SERCOM_INTENSET_USART_MASK  0xFu /**< \brief (SERCOM_INTENSET_USART) MASK Register */

/* -------- SERCOM_INTFLAG : (SERCOM Offset: 0x0E) (R/W  8) Interrupt Flag Register -------- */
typedef union {
  struct { // SPI mode
    uint8_t  DREIF:1;          /*!< bit:      0                                     */
    uint8_t  TXCIF:1;          /*!< bit:      1                                     */
    uint8_t  RXCIF:1;          /*!< bit:      2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint8_t  WIF:1;            /*!< bit:      0                                     */
    uint8_t  RIF:1;            /*!< bit:      1                                     */
    uint8_t  _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint8_t  PIF:1;            /*!< bit:      0                                     */
    uint8_t  AIF:1;            /*!< bit:      1                                     */
    uint8_t  DIF:1;            /*!< bit:      2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  struct { // USART mode
    uint8_t  DREIF:1;          /*!< bit:      0                                     */
    uint8_t  TXCIF:1;          /*!< bit:      1                                     */
    uint8_t  RXCIF:1;          /*!< bit:      2                                     */
    uint8_t  RXSIF:1;          /*!< bit:      3                                     */
    uint8_t  _reserved0:4;     /*!< bit:  4.. 7  Reserved                           */
  } USART;                     /*!< Structure used for USART                        */
  uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_INTFLAG_Type;

#define SERCOM_INTFLAG_OFFSET       0x0E         /**< \brief (SERCOM offset) Interrupt Flag Register */
#define SERCOM_INTFLAG_RESETVALUE   0x0          /**< \brief (SERCOM reset_value) Interrupt Flag Register */

// SPI mode
#define SERCOM_INTFLAG_SPI_DREIF    (0x1u <<  0) /**< \brief (SERCOM_INTFLAG_SPI)  */
#define SERCOM_INTFLAG_SPI_TXCIF    (0x1u <<  1) /**< \brief (SERCOM_INTFLAG_SPI)  */
#define SERCOM_INTFLAG_SPI_RXCIF    (0x1u <<  2) /**< \brief (SERCOM_INTFLAG_SPI)  */
#define SERCOM_INTFLAG_SPI_MASK     0x7u /**< \brief (SERCOM_INTFLAG_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_INTFLAG_TWI_MASTER_WIF (0x1u <<  0) /**< \brief (SERCOM_INTFLAG_TWI_MASTER)  */
#define SERCOM_INTFLAG_TWI_MASTER_RIF (0x1u <<  1) /**< \brief (SERCOM_INTFLAG_TWI_MASTER)  */
#define SERCOM_INTFLAG_TWI_MASTER_MASK 0x3u /**< \brief (SERCOM_INTFLAG_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_INTFLAG_TWI_SLAVE_PIF (0x1u <<  0) /**< \brief (SERCOM_INTFLAG_TWI_SLAVE)  */
#define SERCOM_INTFLAG_TWI_SLAVE_AIF (0x1u <<  1) /**< \brief (SERCOM_INTFLAG_TWI_SLAVE)  */
#define SERCOM_INTFLAG_TWI_SLAVE_DIF (0x1u <<  2) /**< \brief (SERCOM_INTFLAG_TWI_SLAVE)  */
#define SERCOM_INTFLAG_TWI_SLAVE_MASK 0x7u /**< \brief (SERCOM_INTFLAG_TWI_SLAVE) MASK Register */

// USART mode
#define SERCOM_INTFLAG_USART_DREIF  (0x1u <<  0) /**< \brief (SERCOM_INTFLAG_USART)  */
#define SERCOM_INTFLAG_USART_TXCIF  (0x1u <<  1) /**< \brief (SERCOM_INTFLAG_USART)  */
#define SERCOM_INTFLAG_USART_RXCIF  (0x1u <<  2) /**< \brief (SERCOM_INTFLAG_USART)  */
#define SERCOM_INTFLAG_USART_RXSIF  (0x1u <<  3) /**< \brief (SERCOM_INTFLAG_USART)  */
#define SERCOM_INTFLAG_USART_MASK   0xFu /**< \brief (SERCOM_INTFLAG_USART) MASK Register */

/* -------- SERCOM_STATUS : (SERCOM Offset: 0x10) (R/W 32) Status Register -------- */
typedef union {
  struct { // SPI mode
    uint32_t _reserved0:2;     /*!< bit:  0.. 1  Reserved                           */
    uint32_t BUFOVF:1;         /*!< bit:      2                                     */
    uint32_t _reserved1:12;    /*!< bit:  3..14  Reserved                           */
    uint32_t SYNCBUSY:1;       /*!< bit:     15                                     */
    uint32_t _reserved2:16;    /*!< bit: 16..31  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint32_t BUSERR:1;         /*!< bit:      0                                     */
    uint32_t ARBLOST:1;        /*!< bit:      1                                     */
    uint32_t RXACK:1;          /*!< bit:      2                                     */
    uint32_t _reserved0:1;     /*!< bit:      3  Reserved                           */
    uint32_t BUSSTATE:2;       /*!< bit:  4.. 5                                     */
    uint32_t LOWTOUT:1;        /*!< bit:      6                                     */
    uint32_t CLKHOLD:1;        /*!< bit:      7                                     */
    uint32_t _reserved1:7;     /*!< bit:  8..14  Reserved                           */
    uint32_t SYNCBUSY:1;       /*!< bit:     15                                     */
    uint32_t _reserved2:16;    /*!< bit: 16..31  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint32_t BUSERR:1;         /*!< bit:      0                                     */
    uint32_t COLL:1;           /*!< bit:      1                                     */
    uint32_t RXACK:1;          /*!< bit:      2                                     */
    uint32_t DIR:1;            /*!< bit:      3                                     */
    uint32_t SR:1;             /*!< bit:      4                                     */
    uint32_t _reserved0:1;     /*!< bit:      5  Reserved                           */
    uint32_t LOWTOUT:1;        /*!< bit:      6                                     */
    uint32_t CLKHOLD:1;        /*!< bit:      7                                     */
    uint32_t _reserved1:7;     /*!< bit:  8..14  Reserved                           */
    uint32_t SYNCBUSY:1;       /*!< bit:     15                                     */
    uint32_t _reserved2:16;    /*!< bit: 16..31  Reserved                           */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  struct { // USART mode
    uint32_t PERR:1;           /*!< bit:      0                                     */
    uint32_t FERR:1;           /*!< bit:      1                                     */
    uint32_t BUFOVF:1;         /*!< bit:      2                                     */
    uint32_t _reserved0:12;    /*!< bit:  3..14  Reserved                           */
    uint32_t SYNCBUSY:1;       /*!< bit:     15                                     */
    uint32_t _reserved1:16;    /*!< bit: 16..31  Reserved                           */
  } USART;                     /*!< Structure used for USART                        */
  uint32_t reg;                /*!< Type      used for register access              */
} SERCOM_STATUS_Type;

#define SERCOM_STATUS_OFFSET        0x10         /**< \brief (SERCOM offset) Status Register */
#define SERCOM_STATUS_RESETVALUE    0x0          /**< \brief (SERCOM reset_value) Status Register */

// SPI mode
#define SERCOM_STATUS_SPI_BUFOVF    (0x1u <<  2) /**< \brief (SERCOM_STATUS_SPI)  */
#define SERCOM_STATUS_SPI_SYNCBUSY  (0x1u << 15) /**< \brief (SERCOM_STATUS_SPI)  */
#define SERCOM_STATUS_SPI_MASK      0x8004u /**< \brief (SERCOM_STATUS_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_STATUS_TWI_MASTER_BUSERR (0x1u <<  0) /**< \brief (SERCOM_STATUS_TWI_MASTER)  */
#define SERCOM_STATUS_TWI_MASTER_ARBLOST (0x1u <<  1) /**< \brief (SERCOM_STATUS_TWI_MASTER)  */
#define SERCOM_STATUS_TWI_MASTER_RXACK (0x1u <<  2) /**< \brief (SERCOM_STATUS_TWI_MASTER)  */
#define SERCOM_STATUS_TWI_MASTER_BUSSTATE_Pos    4
#define SERCOM_STATUS_TWI_MASTER_BUSSTATE_Msk    (0x3u << SERCOM_STATUS_TWI_MASTER_BUSSTATE_Pos) /**< \brief (SERCOM_STATUS_TWI_MASTER)  */
#define SERCOM_STATUS_TWI_MASTER_BUSSTATE(value) ((SERCOM_STATUS_TWI_MASTER_BUSSTATE_Msk & ((value) << SERCOM_STATUS_TWI_MASTER_BUSSTATE_Pos)))
#define SERCOM_STATUS_TWI_MASTER_LOWTOUT (0x1u <<  6) /**< \brief (SERCOM_STATUS_TWI_MASTER)  */
#define SERCOM_STATUS_TWI_MASTER_CLKHOLD (0x1u <<  7) /**< \brief (SERCOM_STATUS_TWI_MASTER)  */
#define SERCOM_STATUS_TWI_MASTER_SYNCBUSY (0x1u << 15) /**< \brief (SERCOM_STATUS_TWI_MASTER)  */
#define SERCOM_STATUS_TWI_MASTER_MASK 0x80F7u /**< \brief (SERCOM_STATUS_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_STATUS_TWI_SLAVE_BUSERR (0x1u <<  0) /**< \brief (SERCOM_STATUS_TWI_SLAVE)  */
#define SERCOM_STATUS_TWI_SLAVE_COLL (0x1u <<  1) /**< \brief (SERCOM_STATUS_TWI_SLAVE)  */
#define SERCOM_STATUS_TWI_SLAVE_RXACK (0x1u <<  2) /**< \brief (SERCOM_STATUS_TWI_SLAVE)  */
#define SERCOM_STATUS_TWI_SLAVE_DIR (0x1u <<  3) /**< \brief (SERCOM_STATUS_TWI_SLAVE)  */
#define SERCOM_STATUS_TWI_SLAVE_SR  (0x1u <<  4) /**< \brief (SERCOM_STATUS_TWI_SLAVE)  */
#define SERCOM_STATUS_TWI_SLAVE_LOWTOUT (0x1u <<  6) /**< \brief (SERCOM_STATUS_TWI_SLAVE)  */
#define SERCOM_STATUS_TWI_SLAVE_CLKHOLD (0x1u <<  7) /**< \brief (SERCOM_STATUS_TWI_SLAVE)  */
#define SERCOM_STATUS_TWI_SLAVE_SYNCBUSY (0x1u << 15) /**< \brief (SERCOM_STATUS_TWI_SLAVE)  */
#define SERCOM_STATUS_TWI_SLAVE_MASK 0x80DFu /**< \brief (SERCOM_STATUS_TWI_SLAVE) MASK Register */

// USART mode
#define SERCOM_STATUS_USART_PERR    (0x1u <<  0) /**< \brief (SERCOM_STATUS_USART)  */
#define SERCOM_STATUS_USART_FERR    (0x1u <<  1) /**< \brief (SERCOM_STATUS_USART)  */
#define SERCOM_STATUS_USART_BUFOVF  (0x1u <<  2) /**< \brief (SERCOM_STATUS_USART)  */
#define SERCOM_STATUS_USART_SYNCBUSY (0x1u << 15) /**< \brief (SERCOM_STATUS_USART)  */
#define SERCOM_STATUS_USART_MASK    0x8007u /**< \brief (SERCOM_STATUS_USART) MASK Register */

/* -------- SERCOM_ADDR : (SERCOM Offset: 0x14) (R/W 32) Address Register -------- */
typedef union {
  struct { // SPI mode
    uint32_t ADDR:8;           /*!< bit:  0.. 7                                     */
    uint32_t _reserved0:8;     /*!< bit:  8..15  Reserved                           */
    uint32_t ADDRMASK:8;       /*!< bit: 16..23                                     */
    uint32_t _reserved1:8;     /*!< bit: 24..31  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint32_t ADDR:8;           /*!< bit:  0.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint32_t GENCEN:1;         /*!< bit:      0                                     */
    uint32_t ADDR:7;           /*!< bit:  1.. 7                                     */
    uint32_t _reserved0:9;     /*!< bit:  8..16  Reserved                           */
    uint32_t ADDRMASK:7;       /*!< bit: 17..23                                     */
    uint32_t _reserved1:8;     /*!< bit: 24..31  Reserved                           */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  uint32_t reg;                /*!< Type      used for register access              */
} SERCOM_ADDR_Type;

#define SERCOM_ADDR_OFFSET          0x14         /**< \brief (SERCOM offset) Address Register */
#define SERCOM_ADDR_RESETVALUE      0x0          /**< \brief (SERCOM reset_value) Address Register */

// SPI mode
#define SERCOM_ADDR_SPI_ADDR_Pos    0
#define SERCOM_ADDR_SPI_ADDR_Msk    (0xFFu << SERCOM_ADDR_SPI_ADDR_Pos) /**< \brief (SERCOM_ADDR_SPI)  */
#define SERCOM_ADDR_SPI_ADDR(value) ((SERCOM_ADDR_SPI_ADDR_Msk & ((value) << SERCOM_ADDR_SPI_ADDR_Pos)))
#define SERCOM_ADDR_SPI_ADDRMASK_Pos    16
#define SERCOM_ADDR_SPI_ADDRMASK_Msk    (0xFFu << SERCOM_ADDR_SPI_ADDRMASK_Pos) /**< \brief (SERCOM_ADDR_SPI)  */
#define SERCOM_ADDR_SPI_ADDRMASK(value) ((SERCOM_ADDR_SPI_ADDRMASK_Msk & ((value) << SERCOM_ADDR_SPI_ADDRMASK_Pos)))
#define SERCOM_ADDR_SPI_MASK        0xFF00FFu /**< \brief (SERCOM_ADDR_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_ADDR_TWI_MASTER_ADDR_Pos    0
#define SERCOM_ADDR_TWI_MASTER_ADDR_Msk    (0xFFu << SERCOM_ADDR_TWI_MASTER_ADDR_Pos) /**< \brief (SERCOM_ADDR_TWI_MASTER)  */
#define SERCOM_ADDR_TWI_MASTER_ADDR(value) ((SERCOM_ADDR_TWI_MASTER_ADDR_Msk & ((value) << SERCOM_ADDR_TWI_MASTER_ADDR_Pos)))
#define SERCOM_ADDR_TWI_MASTER_MASK 0xFFu /**< \brief (SERCOM_ADDR_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_ADDR_TWI_SLAVE_GENCEN (0x1u <<  0) /**< \brief (SERCOM_ADDR_TWI_SLAVE)  */
#define SERCOM_ADDR_TWI_SLAVE_ADDR_Pos    1
#define SERCOM_ADDR_TWI_SLAVE_ADDR_Msk    (0x7Fu << SERCOM_ADDR_TWI_SLAVE_ADDR_Pos) /**< \brief (SERCOM_ADDR_TWI_SLAVE)  */
#define SERCOM_ADDR_TWI_SLAVE_ADDR(value) ((SERCOM_ADDR_TWI_SLAVE_ADDR_Msk & ((value) << SERCOM_ADDR_TWI_SLAVE_ADDR_Pos)))
#define SERCOM_ADDR_TWI_SLAVE_ADDRMASK_Pos    17
#define SERCOM_ADDR_TWI_SLAVE_ADDRMASK_Msk    (0x7Fu << SERCOM_ADDR_TWI_SLAVE_ADDRMASK_Pos) /**< \brief (SERCOM_ADDR_TWI_SLAVE)  */
#define SERCOM_ADDR_TWI_SLAVE_ADDRMASK(value) ((SERCOM_ADDR_TWI_SLAVE_ADDRMASK_Msk & ((value) << SERCOM_ADDR_TWI_SLAVE_ADDRMASK_Pos)))
#define SERCOM_ADDR_TWI_SLAVE_MASK  0xFE00FFu /**< \brief (SERCOM_ADDR_TWI_SLAVE) MASK Register */

/* -------- SERCOM_DATA : (SERCOM Offset: 0x18) (R/W 32) Data Register -------- */
typedef union {
  struct { // SPI mode
    uint32_t DATA:9;           /*!< bit:  0.. 8                                     */
    uint32_t _reserved0:23;    /*!< bit:  9..31  Reserved                           */
  } SPI;                       /*!< Structure used for SPI                          */
  struct { // TWI_MASTER mode
    uint32_t DATA:8;           /*!< bit:  0.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } TWI_MASTER;                /*!< Structure used for TWI_MASTER                   */
  struct { // TWI_SLAVE mode
    uint32_t DATA:8;           /*!< bit:  0.. 7                                     */
    uint32_t _reserved0:24;    /*!< bit:  8..31  Reserved                           */
  } TWI_SLAVE;                 /*!< Structure used for TWI_SLAVE                    */
  struct { // USART mode
    uint32_t DATA:9;           /*!< bit:  0.. 8                                     */
    uint32_t _reserved0:23;    /*!< bit:  9..31  Reserved                           */
  } USART;                     /*!< Structure used for USART                        */
  uint32_t reg;                /*!< Type      used for register access              */
} SERCOM_DATA_Type;

#define SERCOM_DATA_OFFSET          0x18         /**< \brief (SERCOM offset) Data Register */
#define SERCOM_DATA_RESETVALUE      0x0          /**< \brief (SERCOM reset_value) Data Register */

// SPI mode
#define SERCOM_DATA_SPI_DATA_Pos    0
#define SERCOM_DATA_SPI_DATA_Msk    (0x1FFu << SERCOM_DATA_SPI_DATA_Pos) /**< \brief (SERCOM_DATA_SPI)  */
#define SERCOM_DATA_SPI_DATA(value) ((SERCOM_DATA_SPI_DATA_Msk & ((value) << SERCOM_DATA_SPI_DATA_Pos)))
#define SERCOM_DATA_SPI_MASK        0x1FFu /**< \brief (SERCOM_DATA_SPI) MASK Register */

// TWI_MASTER mode
#define SERCOM_DATA_TWI_MASTER_DATA_Pos    0
#define SERCOM_DATA_TWI_MASTER_DATA_Msk    (0xFFu << SERCOM_DATA_TWI_MASTER_DATA_Pos) /**< \brief (SERCOM_DATA_TWI_MASTER)  */
#define SERCOM_DATA_TWI_MASTER_DATA(value) ((SERCOM_DATA_TWI_MASTER_DATA_Msk & ((value) << SERCOM_DATA_TWI_MASTER_DATA_Pos)))
#define SERCOM_DATA_TWI_MASTER_MASK 0xFFu /**< \brief (SERCOM_DATA_TWI_MASTER) MASK Register */

// TWI_SLAVE mode
#define SERCOM_DATA_TWI_SLAVE_DATA_Pos    0
#define SERCOM_DATA_TWI_SLAVE_DATA_Msk    (0xFFu << SERCOM_DATA_TWI_SLAVE_DATA_Pos) /**< \brief (SERCOM_DATA_TWI_SLAVE)  */
#define SERCOM_DATA_TWI_SLAVE_DATA(value) ((SERCOM_DATA_TWI_SLAVE_DATA_Msk & ((value) << SERCOM_DATA_TWI_SLAVE_DATA_Pos)))
#define SERCOM_DATA_TWI_SLAVE_MASK  0xFFu /**< \brief (SERCOM_DATA_TWI_SLAVE) MASK Register */

// USART mode
#define SERCOM_DATA_USART_DATA_Pos    0
#define SERCOM_DATA_USART_DATA_Msk    (0x1FFu << SERCOM_DATA_USART_DATA_Pos) /**< \brief (SERCOM_DATA_USART)  */
#define SERCOM_DATA_USART_DATA(value) ((SERCOM_DATA_USART_DATA_Msk & ((value) << SERCOM_DATA_USART_DATA_Pos)))
#define SERCOM_DATA_USART_MASK      0x1FFu /**< \brief (SERCOM_DATA_USART) MASK Register */

/** \brief SERCOM hardware registers */
typedef struct {
  __IO SERCOM_CTRLA_Type         CTRLA;       /**< \brief Offset: 0x00 (R/W 32) Control Register A */
  __IO SERCOM_CTRLB_Type         CTRLB;       /**< \brief Offset: 0x04 (R/W 32) Control Register B */
  __IO SERCOM_DEBUG_Type         DEBUG;       /**< \brief Offset: 0x08 (R/W  8) Debug Register */
       RoReg8                    Reserved1[0x1];
  __IO SERCOM_BAUD_Type          BAUD;        /**< \brief Offset: 0x0A (R/W 16) Baud Rate Register */
  __IO SERCOM_INTENCLR_Type      INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear Register */
  __IO SERCOM_INTENSET_Type      INTENSET;    /**< \brief Offset: 0x0D (R/W  8) Interrupt Enable Set Register */
  __IO SERCOM_INTFLAG_Type       INTFLAG;     /**< \brief Offset: 0x0E (R/W  8) Interrupt Flag Register */
       RoReg8                    Reserved2[0x1];
  __IO SERCOM_STATUS_Type        STATUS;      /**< \brief Offset: 0x10 (R/W 32) Status Register */
  __IO SERCOM_ADDR_Type          ADDR;        /**< \brief Offset: 0x14 (R/W 32) Address Register */
  __IO SERCOM_DATA_Type          DATA;        /**< \brief Offset: 0x18 (R/W 32) Data Register */
} Sercom;

#endif /*  __EXCLUDE_SERCOM_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR SYSCTRL */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_SYSCTRL System Control */
/*@{*/

#define REV_SYSCTRL    0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_SYSCTRL_FROM_CMSIS__

/* -------- SYSCTRL_INTENCLR : (SYSCTRL Offset: 0x00) (R/W 32) Interrupt Enable Register -------- */
typedef union {
  struct {
    uint32_t XOSCRDY:1;        /*!< bit:      0  XOSC ready                         */
    uint32_t XOSC32KRDY:1;     /*!< bit:      1  XOSC32K ready                      */
    uint32_t BOD33RDY:1;       /*!< bit:      2  BOD33 ready                        */
    uint32_t BOD12RDY:1;       /*!< bit:      3  BOD12 ready                        */
    uint32_t BOD33DET:1;       /*!< bit:      4  BOD33 detection                    */
    uint32_t BOD12DET:1;       /*!< bit:      5  BOD12 detection                    */
    uint32_t BOD33BSY:1;       /*!< bit:      6  BOD33 synchronization busy         */
    uint32_t BOD12BSY:1;       /*!< bit:      7  BOD12 synchronization busy         */
    uint32_t DFLL48RDY:1;      /*!< bit:      8  DFLL ready                         */
    uint32_t DFLL48OOB:1;      /*!< bit:      9  DFLL out of boundaries             */
    uint32_t DFLL48LOCKF:1;    /*!< bit:     10  DFLL lock fine                     */
    uint32_t DFLL48LOCKC:1;    /*!< bit:     11  DFLL lock coarse                   */
    uint32_t DFLL48RCS:1;      /*!< bit:     12  DFLL reference clock stopped       */
    uint32_t OSC32KRDY:1;      /*!< bit:     13  OSC32K ready                       */
    uint32_t _reserved0:18;    /*!< bit: 14..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_INTENCLR_Type;

#define SYSCTRL_INTENCLR_OFFSET     0x00         /**< \brief (SYSCTRL offset) Interrupt Enable Register */
#define SYSCTRL_INTENCLR_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) Interrupt Enable Register */

#define SYSCTRL_INTENCLR_XOSCRDY    (0x1u <<  0) /**< \brief (SYSCTRL_INTENCLR) XOSC ready */
#define SYSCTRL_INTENCLR_XOSC32KRDY (0x1u <<  1) /**< \brief (SYSCTRL_INTENCLR) XOSC32K ready */
#define SYSCTRL_INTENCLR_BOD33RDY   (0x1u <<  2) /**< \brief (SYSCTRL_INTENCLR) BOD33 ready */
#define SYSCTRL_INTENCLR_BOD12RDY   (0x1u <<  3) /**< \brief (SYSCTRL_INTENCLR) BOD12 ready */
#define SYSCTRL_INTENCLR_BOD33DET   (0x1u <<  4) /**< \brief (SYSCTRL_INTENCLR) BOD33 detection */
#define SYSCTRL_INTENCLR_BOD12DET   (0x1u <<  5) /**< \brief (SYSCTRL_INTENCLR) BOD12 detection */
#define SYSCTRL_INTENCLR_BOD33BSY   (0x1u <<  6) /**< \brief (SYSCTRL_INTENCLR) BOD33 synchronization busy */
#define SYSCTRL_INTENCLR_BOD12BSY   (0x1u <<  7) /**< \brief (SYSCTRL_INTENCLR) BOD12 synchronization busy */
#define SYSCTRL_INTENCLR_DFLL48RDY  (0x1u <<  8) /**< \brief (SYSCTRL_INTENCLR) DFLL ready */
#define SYSCTRL_INTENCLR_DFLL48OOB  (0x1u <<  9) /**< \brief (SYSCTRL_INTENCLR) DFLL out of boundaries */
#define SYSCTRL_INTENCLR_DFLL48LOCKF (0x1u << 10) /**< \brief (SYSCTRL_INTENCLR) DFLL lock fine */
#define SYSCTRL_INTENCLR_DFLL48LOCKC (0x1u << 11) /**< \brief (SYSCTRL_INTENCLR) DFLL lock coarse */
#define SYSCTRL_INTENCLR_DFLL48RCS  (0x1u << 12) /**< \brief (SYSCTRL_INTENCLR) DFLL reference clock stopped */
#define SYSCTRL_INTENCLR_OSC32KRDY  (0x1u << 13) /**< \brief (SYSCTRL_INTENCLR) OSC32K ready */
#define SYSCTRL_INTENCLR_MASK       0x3FFFu /**< \brief (SYSCTRL_INTENCLR) MASK Register */

/* -------- SYSCTRL_INTENSET : (SYSCTRL Offset: 0x04) (R/W 32) Interrupt Clear Register -------- */
typedef union {
  struct {
    uint32_t XOSCRDY:1;        /*!< bit:      0  XOSC ready                         */
    uint32_t XOSC32KRDY:1;     /*!< bit:      1  XOSC32K ready                      */
    uint32_t BOD33RDY:1;       /*!< bit:      2  BOD33 ready                        */
    uint32_t BOD12RDY:1;       /*!< bit:      3  BOD12 ready                        */
    uint32_t BOD33DET:1;       /*!< bit:      4  BOD33 detection                    */
    uint32_t BOD12DET:1;       /*!< bit:      5  BOD12 detection                    */
    uint32_t BOD33BSY:1;       /*!< bit:      6  BOD33 synchronization busy         */
    uint32_t BOD12BSY:1;       /*!< bit:      7  BOD12 synchronization busy         */
    uint32_t DFLL48RDY:1;      /*!< bit:      8  DFLL ready                         */
    uint32_t DFLL48OOB:1;      /*!< bit:      9  DFLL out of boundaries             */
    uint32_t DFLL48LOCKF:1;    /*!< bit:     10  DFLL lock fine                     */
    uint32_t DFLL48LOCKC:1;    /*!< bit:     11  DFLL lock coarse                   */
    uint32_t DFLL48RCS:1;      /*!< bit:     12  DFLL reference clock stopped       */
    uint32_t OSC32KRDY:1;      /*!< bit:     13  OSC32K ready                       */
    uint32_t _reserved0:18;    /*!< bit: 14..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_INTENSET_Type;

#define SYSCTRL_INTENSET_OFFSET     0x04         /**< \brief (SYSCTRL offset) Interrupt Clear Register */
#define SYSCTRL_INTENSET_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) Interrupt Clear Register */

#define SYSCTRL_INTENSET_XOSCRDY    (0x1u <<  0) /**< \brief (SYSCTRL_INTENSET) XOSC ready */
#define SYSCTRL_INTENSET_XOSC32KRDY (0x1u <<  1) /**< \brief (SYSCTRL_INTENSET) XOSC32K ready */
#define SYSCTRL_INTENSET_BOD33RDY   (0x1u <<  2) /**< \brief (SYSCTRL_INTENSET) BOD33 ready */
#define SYSCTRL_INTENSET_BOD12RDY   (0x1u <<  3) /**< \brief (SYSCTRL_INTENSET) BOD12 ready */
#define SYSCTRL_INTENSET_BOD33DET   (0x1u <<  4) /**< \brief (SYSCTRL_INTENSET) BOD33 detection */
#define SYSCTRL_INTENSET_BOD12DET   (0x1u <<  5) /**< \brief (SYSCTRL_INTENSET) BOD12 detection */
#define SYSCTRL_INTENSET_BOD33BSY   (0x1u <<  6) /**< \brief (SYSCTRL_INTENSET) BOD33 synchronization busy */
#define SYSCTRL_INTENSET_BOD12BSY   (0x1u <<  7) /**< \brief (SYSCTRL_INTENSET) BOD12 synchronization busy */
#define SYSCTRL_INTENSET_DFLL48RDY  (0x1u <<  8) /**< \brief (SYSCTRL_INTENSET) DFLL ready */
#define SYSCTRL_INTENSET_DFLL48OOB  (0x1u <<  9) /**< \brief (SYSCTRL_INTENSET) DFLL out of boundaries */
#define SYSCTRL_INTENSET_DFLL48LOCKF (0x1u << 10) /**< \brief (SYSCTRL_INTENSET) DFLL lock fine */
#define SYSCTRL_INTENSET_DFLL48LOCKC (0x1u << 11) /**< \brief (SYSCTRL_INTENSET) DFLL lock coarse */
#define SYSCTRL_INTENSET_DFLL48RCS  (0x1u << 12) /**< \brief (SYSCTRL_INTENSET) DFLL reference clock stopped */
#define SYSCTRL_INTENSET_OSC32KRDY  (0x1u << 13) /**< \brief (SYSCTRL_INTENSET) OSC32K ready */
#define SYSCTRL_INTENSET_MASK       0x3FFFu /**< \brief (SYSCTRL_INTENSET) MASK Register */

/* -------- SYSCTRL_INTFLAG : (SYSCTRL Offset: 0x08) (R/W 32) Interrupt Flag Register -------- */
typedef union {
  struct {
    uint32_t XOSCRDY:1;        /*!< bit:      0  XOSC ready                         */
    uint32_t XOSC32KRDY:1;     /*!< bit:      1  XOSC32K ready                      */
    uint32_t BOD33RDY:1;       /*!< bit:      2  BOD33 ready                        */
    uint32_t BOD12RDY:1;       /*!< bit:      3  BOD12 ready                        */
    uint32_t BOD33DET:1;       /*!< bit:      4  BOD33 detection                    */
    uint32_t BOD12DET:1;       /*!< bit:      5  BOD12 detection                    */
    uint32_t BOD33BSY:1;       /*!< bit:      6  BOD33 synchronization busy         */
    uint32_t BOD12BSY:1;       /*!< bit:      7  BOD12 synchronization busy         */
    uint32_t DFLL48RDY:1;      /*!< bit:      8  DFLL ready                         */
    uint32_t DFLL48OOB:1;      /*!< bit:      9  DFLL out of boundaries             */
    uint32_t DFLL48LOCKF:1;    /*!< bit:     10  DFLL lock fine                     */
    uint32_t DFLL48LOCKC:1;    /*!< bit:     11  DFLL lock coarse                   */
    uint32_t DFLL48RCS:1;      /*!< bit:     12  DFLL reference clock stopped       */
    uint32_t OSC32KRDY:1;      /*!< bit:     13  OSC32K ready                       */
    uint32_t _reserved0:18;    /*!< bit: 14..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_INTFLAG_Type;

#define SYSCTRL_INTFLAG_OFFSET      0x08         /**< \brief (SYSCTRL offset) Interrupt Flag Register */
#define SYSCTRL_INTFLAG_RESETVALUE  0x0          /**< \brief (SYSCTRL reset_value) Interrupt Flag Register */

#define SYSCTRL_INTFLAG_XOSCRDY     (0x1u <<  0) /**< \brief (SYSCTRL_INTFLAG) XOSC ready */
#define SYSCTRL_INTFLAG_XOSC32KRDY  (0x1u <<  1) /**< \brief (SYSCTRL_INTFLAG) XOSC32K ready */
#define SYSCTRL_INTFLAG_BOD33RDY    (0x1u <<  2) /**< \brief (SYSCTRL_INTFLAG) BOD33 ready */
#define SYSCTRL_INTFLAG_BOD12RDY    (0x1u <<  3) /**< \brief (SYSCTRL_INTFLAG) BOD12 ready */
#define SYSCTRL_INTFLAG_BOD33DET    (0x1u <<  4) /**< \brief (SYSCTRL_INTFLAG) BOD33 detection */
#define SYSCTRL_INTFLAG_BOD12DET    (0x1u <<  5) /**< \brief (SYSCTRL_INTFLAG) BOD12 detection */
#define SYSCTRL_INTFLAG_BOD33BSY    (0x1u <<  6) /**< \brief (SYSCTRL_INTFLAG) BOD33 synchronization busy */
#define SYSCTRL_INTFLAG_BOD12BSY    (0x1u <<  7) /**< \brief (SYSCTRL_INTFLAG) BOD12 synchronization busy */
#define SYSCTRL_INTFLAG_DFLL48RDY   (0x1u <<  8) /**< \brief (SYSCTRL_INTFLAG) DFLL ready */
#define SYSCTRL_INTFLAG_DFLL48OOB   (0x1u <<  9) /**< \brief (SYSCTRL_INTFLAG) DFLL out of boundaries */
#define SYSCTRL_INTFLAG_DFLL48LOCKF (0x1u << 10) /**< \brief (SYSCTRL_INTFLAG) DFLL lock fine */
#define SYSCTRL_INTFLAG_DFLL48LOCKC (0x1u << 11) /**< \brief (SYSCTRL_INTFLAG) DFLL lock coarse */
#define SYSCTRL_INTFLAG_DFLL48RCS   (0x1u << 12) /**< \brief (SYSCTRL_INTFLAG) DFLL reference clock stopped */
#define SYSCTRL_INTFLAG_OSC32KRDY   (0x1u << 13) /**< \brief (SYSCTRL_INTFLAG) OSC32K ready */
#define SYSCTRL_INTFLAG_MASK        0x3FFFu /**< \brief (SYSCTRL_INTFLAG) MASK Register */

/* -------- SYSCTRL_PCLKSR : (SYSCTRL Offset: 0x0C) (R/  32) Power and Clocks Status Register -------- */
typedef union {
  struct {
    uint32_t XOSCRDY:1;        /*!< bit:      0  XOSC ready                         */
    uint32_t XOSC32KRDY:1;     /*!< bit:      1  XOSC32K ready                      */
    uint32_t BOD33RDY:1;       /*!< bit:      2  BOD33 ready                        */
    uint32_t BOD12RDY:1;       /*!< bit:      3  BOD12 ready                        */
    uint32_t BOD33DET:1;       /*!< bit:      4  BOD33 detection                    */
    uint32_t BOD12DET:1;       /*!< bit:      5  BOD12 detection                    */
    uint32_t BOD33BSY:1;       /*!< bit:      6  BOD33 synchronization busy         */
    uint32_t BOD12BSY:1;       /*!< bit:      7  BOD12 synchronization busy         */
    uint32_t DFLL48RDY:1;      /*!< bit:      8  DFLL ready                         */
    uint32_t DFLL48OOB:1;      /*!< bit:      9  DFLL out of boundaries             */
    uint32_t DFLL48LOCKF:1;    /*!< bit:     10  DFLL lock fine                     */
    uint32_t DFLL48LOCKC:1;    /*!< bit:     11  DFLL lock coarse                   */
    uint32_t DFLL48RCS:1;      /*!< bit:     12  DFLL reference clock stopped       */
    uint32_t OSC32KRDY:1;      /*!< bit:     13  OSC32KRDY                          */
    uint32_t _reserved0:18;    /*!< bit: 14..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_PCLKSR_Type;

#define SYSCTRL_PCLKSR_OFFSET       0x0C         /**< \brief (SYSCTRL offset) Power and Clocks Status Register */
#define SYSCTRL_PCLKSR_RESETVALUE   0x0          /**< \brief (SYSCTRL reset_value) Power and Clocks Status Register */

#define SYSCTRL_PCLKSR_XOSCRDY      (0x1u <<  0) /**< \brief (SYSCTRL_PCLKSR) XOSC ready */
#define SYSCTRL_PCLKSR_XOSC32KRDY   (0x1u <<  1) /**< \brief (SYSCTRL_PCLKSR) XOSC32K ready */
#define SYSCTRL_PCLKSR_BOD33RDY     (0x1u <<  2) /**< \brief (SYSCTRL_PCLKSR) BOD33 ready */
#define SYSCTRL_PCLKSR_BOD12RDY     (0x1u <<  3) /**< \brief (SYSCTRL_PCLKSR) BOD12 ready */
#define SYSCTRL_PCLKSR_BOD33DET     (0x1u <<  4) /**< \brief (SYSCTRL_PCLKSR) BOD33 detection */
#define SYSCTRL_PCLKSR_BOD12DET     (0x1u <<  5) /**< \brief (SYSCTRL_PCLKSR) BOD12 detection */
#define SYSCTRL_PCLKSR_BOD33BSY     (0x1u <<  6) /**< \brief (SYSCTRL_PCLKSR) BOD33 synchronization busy */
#define SYSCTRL_PCLKSR_BOD12BSY     (0x1u <<  7) /**< \brief (SYSCTRL_PCLKSR) BOD12 synchronization busy */
#define SYSCTRL_PCLKSR_DFLL48RDY    (0x1u <<  8) /**< \brief (SYSCTRL_PCLKSR) DFLL ready */
#define SYSCTRL_PCLKSR_DFLL48OOB    (0x1u <<  9) /**< \brief (SYSCTRL_PCLKSR) DFLL out of boundaries */
#define SYSCTRL_PCLKSR_DFLL48LOCKF  (0x1u << 10) /**< \brief (SYSCTRL_PCLKSR) DFLL lock fine */
#define SYSCTRL_PCLKSR_DFLL48LOCKC  (0x1u << 11) /**< \brief (SYSCTRL_PCLKSR) DFLL lock coarse */
#define SYSCTRL_PCLKSR_DFLL48RCS    (0x1u << 12) /**< \brief (SYSCTRL_PCLKSR) DFLL reference clock stopped */
#define SYSCTRL_PCLKSR_OSC32KRDY    (0x1u << 13) /**< \brief (SYSCTRL_PCLKSR) OSC32KRDY */
#define SYSCTRL_PCLKSR_MASK         0x3FFFu /**< \brief (SYSCTRL_PCLKSR) MASK Register */

/* -------- SYSCTRL_XOSC : (SYSCTRL Offset: 0x10) (R/W 16) XOSC Control Register -------- */
typedef union {
  struct {
    uint16_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1                                     */
    uint16_t XTALEN:1;         /*!< bit:      2                                     */
    uint16_t _reserved1:5;     /*!< bit:  3.. 7  Reserved                           */
    uint16_t GAIN:3;           /*!< bit:  8..10                                     */
    uint16_t AMPGC:1;          /*!< bit:     11                                     */
    uint16_t STARTUP:4;        /*!< bit: 12..15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_XOSC_Type;

#define SYSCTRL_XOSC_OFFSET         0x10         /**< \brief (SYSCTRL offset) XOSC Control Register */

#define SYSCTRL_XOSC_ENABLE         (0x1u <<  1) /**< \brief (SYSCTRL_XOSC)  */
#define SYSCTRL_XOSC_XTALEN         (0x1u <<  2) /**< \brief (SYSCTRL_XOSC)  */
#define SYSCTRL_XOSC_GAIN_Pos       8
#define SYSCTRL_XOSC_GAIN_Msk       (0x7u << SYSCTRL_XOSC_GAIN_Pos) /**< \brief (SYSCTRL_XOSC)  */
#define SYSCTRL_XOSC_GAIN(value)    ((SYSCTRL_XOSC_GAIN_Msk & ((value) << SYSCTRL_XOSC_GAIN_Pos)))
#define SYSCTRL_XOSC_AMPGC          (0x1u << 11) /**< \brief (SYSCTRL_XOSC)  */
#define SYSCTRL_XOSC_STARTUP_Pos    12
#define SYSCTRL_XOSC_STARTUP_Msk    (0xFu << SYSCTRL_XOSC_STARTUP_Pos) /**< \brief (SYSCTRL_XOSC)  */
#define SYSCTRL_XOSC_STARTUP(value) ((SYSCTRL_XOSC_STARTUP_Msk & ((value) << SYSCTRL_XOSC_STARTUP_Pos)))
#define SYSCTRL_XOSC_MASK           0xFF06u /**< \brief (SYSCTRL_XOSC) MASK Register */

/* -------- SYSCTRL_XOSC32K : (SYSCTRL Offset: 0x14) (R/W 16) XOSC32K Control Register -------- */
typedef union {
  struct {
    uint16_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1                                     */
    uint16_t XTALEN:1;         /*!< bit:      2                                     */
    uint16_t EN32K:1;          /*!< bit:      3                                     */
    uint16_t EN1K:1;           /*!< bit:      4                                     */
    uint16_t AAMPEN:1;         /*!< bit:      5                                     */
    uint16_t _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
    uint16_t STARTUP:3;        /*!< bit:  8..10                                     */
    uint16_t _reserved2:5;     /*!< bit: 11..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_XOSC32K_Type;

#define SYSCTRL_XOSC32K_OFFSET      0x14         /**< \brief (SYSCTRL offset) XOSC32K Control Register */
#define SYSCTRL_XOSC32K_RESETVALUE  0x0          /**< \brief (SYSCTRL reset_value) XOSC32K Control Register */

#define SYSCTRL_XOSC32K_ENABLE      (0x1u <<  1) /**< \brief (SYSCTRL_XOSC32K)  */
#define SYSCTRL_XOSC32K_XTALEN      (0x1u <<  2) /**< \brief (SYSCTRL_XOSC32K)  */
#define SYSCTRL_XOSC32K_EN32K       (0x1u <<  3) /**< \brief (SYSCTRL_XOSC32K)  */
#define SYSCTRL_XOSC32K_EN1K        (0x1u <<  4) /**< \brief (SYSCTRL_XOSC32K)  */
#define SYSCTRL_XOSC32K_AAMPEN      (0x1u <<  5) /**< \brief (SYSCTRL_XOSC32K)  */
#define SYSCTRL_XOSC32K_STARTUP_Pos    8
#define SYSCTRL_XOSC32K_STARTUP_Msk    (0x7u << SYSCTRL_XOSC32K_STARTUP_Pos) /**< \brief (SYSCTRL_XOSC32K)  */
#define SYSCTRL_XOSC32K_STARTUP(value) ((SYSCTRL_XOSC32K_STARTUP_Msk & ((value) << SYSCTRL_XOSC32K_STARTUP_Pos)))
#define SYSCTRL_XOSC32K_MASK        0x73Eu /**< \brief (SYSCTRL_XOSC32K) MASK Register */

/* -------- SYSCTRL_XOSC32KTEST : (SYSCTRL Offset: 0x16) (R/W  8) XOSC32K Test Register -------- */
typedef union {
  struct {
    uint8_t  MCALIBEN:1;       /*!< bit:      0                                     */
    uint8_t  MCALIB:6;         /*!< bit:  1.. 6                                     */
    uint8_t  _reserved0:1;     /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} SYSCTRL_XOSC32KTEST_Type;

#define SYSCTRL_XOSC32KTEST_OFFSET  0x16         /**< \brief (SYSCTRL offset) XOSC32K Test Register */
#define SYSCTRL_XOSC32KTEST_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) XOSC32K Test Register */

#define SYSCTRL_XOSC32KTEST_MCALIBEN (0x1u <<  0) /**< \brief (SYSCTRL_XOSC32KTEST)  */
#define SYSCTRL_XOSC32KTEST_MCALIB_Pos    1
#define SYSCTRL_XOSC32KTEST_MCALIB_Msk    (0x3Fu << SYSCTRL_XOSC32KTEST_MCALIB_Pos) /**< \brief (SYSCTRL_XOSC32KTEST)  */
#define SYSCTRL_XOSC32KTEST_MCALIB(value) ((SYSCTRL_XOSC32KTEST_MCALIB_Msk & ((value) << SYSCTRL_XOSC32KTEST_MCALIB_Pos)))
#define SYSCTRL_XOSC32KTEST_MASK    0x7Fu /**< \brief (SYSCTRL_XOSC32KTEST) MASK Register */

/* -------- SYSCTRL_DFLL48CTRL : (SYSCTRL Offset: 0x18) (R/W 32) DFLL  Config Register -------- */
typedef union {
  struct {
    uint32_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint32_t MODE:1;           /*!< bit:      2  Mode Selection                     */
    uint32_t STABLE:1;         /*!< bit:      3  Stable DFLL Frequency              */
    uint32_t LLAW:1;           /*!< bit:      4  Lose Lock After Wake               */
    uint32_t USBCRM:1;         /*!< bit:      5  USB Clock Recovery Mode            */
    uint32_t CCDIS:1;          /*!< bit:      6  Chill Cycle Disable                */
    uint32_t QLDIS:1;          /*!< bit:      7  Quick Lock Disable                 */
    uint32_t _reserved1:24;    /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_DFLL48CTRL_Type;

#define SYSCTRL_DFLL48CTRL_OFFSET   0x18         /**< \brief (SYSCTRL offset) DFLL  Config Register */
#define SYSCTRL_DFLL48CTRL_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) DFLL  Config Register */

#define SYSCTRL_DFLL48CTRL_ENABLE   (0x1u <<  1) /**< \brief (SYSCTRL_DFLL48CTRL) Enable */
#define SYSCTRL_DFLL48CTRL_MODE     (0x1u <<  2) /**< \brief (SYSCTRL_DFLL48CTRL) Mode Selection */
#define SYSCTRL_DFLL48CTRL_STABLE   (0x1u <<  3) /**< \brief (SYSCTRL_DFLL48CTRL) Stable DFLL Frequency */
#define SYSCTRL_DFLL48CTRL_LLAW     (0x1u <<  4) /**< \brief (SYSCTRL_DFLL48CTRL) Lose Lock After Wake */
#define SYSCTRL_DFLL48CTRL_USBCRM   (0x1u <<  5) /**< \brief (SYSCTRL_DFLL48CTRL) USB Clock Recovery Mode */
#define SYSCTRL_DFLL48CTRL_CCDIS    (0x1u <<  6) /**< \brief (SYSCTRL_DFLL48CTRL) Chill Cycle Disable */
#define SYSCTRL_DFLL48CTRL_QLDIS    (0x1u <<  7) /**< \brief (SYSCTRL_DFLL48CTRL) Quick Lock Disable */
#define SYSCTRL_DFLL48CTRL_MASK     0xFEu /**< \brief (SYSCTRL_DFLL48CTRL) MASK Register */

/* -------- SYSCTRL_DFLL48VALDIFF : (SYSCTRL Offset: 0x1C) (R/W 32) DFLL Calibration Value Register -------- */
typedef union {
  struct {
    uint32_t FINE:8;           /*!< bit:  0.. 7  DFLL Fine Calibration Value        */
    uint32_t COARSE:5;         /*!< bit:  8..12  DFLL Coarse Calibration Value      */
    uint32_t _reserved0:3;     /*!< bit: 13..15  Reserved                           */
    uint32_t DIFF:16;          /*!< bit: 16..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_DFLL48VALDIFF_Type;

#define SYSCTRL_DFLL48VALDIFF_OFFSET 0x1C         /**< \brief (SYSCTRL offset) DFLL Calibration Value Register */
#define SYSCTRL_DFLL48VALDIFF_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) DFLL Calibration Value Register */

#define SYSCTRL_DFLL48VALDIFF_FINE_Pos    0
#define SYSCTRL_DFLL48VALDIFF_FINE_Msk    (0xFFu << SYSCTRL_DFLL48VALDIFF_FINE_Pos) /**< \brief (SYSCTRL_DFLL48VALDIFF) DFLL Fine Calibration Value */
#define SYSCTRL_DFLL48VALDIFF_FINE(value) ((SYSCTRL_DFLL48VALDIFF_FINE_Msk & ((value) << SYSCTRL_DFLL48VALDIFF_FINE_Pos)))
#define SYSCTRL_DFLL48VALDIFF_COARSE_Pos    8
#define SYSCTRL_DFLL48VALDIFF_COARSE_Msk    (0x1Fu << SYSCTRL_DFLL48VALDIFF_COARSE_Pos) /**< \brief (SYSCTRL_DFLL48VALDIFF) DFLL Coarse Calibration Value */
#define SYSCTRL_DFLL48VALDIFF_COARSE(value) ((SYSCTRL_DFLL48VALDIFF_COARSE_Msk & ((value) << SYSCTRL_DFLL48VALDIFF_COARSE_Pos)))
#define SYSCTRL_DFLL48VALDIFF_DIFF_Pos    16
#define SYSCTRL_DFLL48VALDIFF_DIFF_Msk    (0xFFFFu << SYSCTRL_DFLL48VALDIFF_DIFF_Pos) /**< \brief (SYSCTRL_DFLL48VALDIFF)  */
#define SYSCTRL_DFLL48VALDIFF_DIFF(value) ((SYSCTRL_DFLL48VALDIFF_DIFF_Msk & ((value) << SYSCTRL_DFLL48VALDIFF_DIFF_Pos)))
#define SYSCTRL_DFLL48VALDIFF_MASK  0xFFFF1FFFu /**< \brief (SYSCTRL_DFLL48VALDIFF) MASK Register */

/* -------- SYSCTRL_DFLL48MULSTEP : (SYSCTRL Offset: 0x20) (R/W 32) DFLL Multiplier Register -------- */
typedef union {
  struct {
    uint32_t MUL:16;           /*!< bit:  0..15  DFLL Multiplitation Value          */
    uint32_t FSTEP:8;          /*!< bit: 16..23                                     */
    uint32_t CSTEP:5;          /*!< bit: 24..28                                     */
    uint32_t _reserved0:3;     /*!< bit: 29..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_DFLL48MULSTEP_Type;

#define SYSCTRL_DFLL48MULSTEP_OFFSET 0x20         /**< \brief (SYSCTRL offset) DFLL Multiplier Register */
#define SYSCTRL_DFLL48MULSTEP_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) DFLL Multiplier Register */

#define SYSCTRL_DFLL48MULSTEP_MUL_Pos    0
#define SYSCTRL_DFLL48MULSTEP_MUL_Msk    (0xFFFFu << SYSCTRL_DFLL48MULSTEP_MUL_Pos) /**< \brief (SYSCTRL_DFLL48MULSTEP) DFLL Multiplitation Value */
#define SYSCTRL_DFLL48MULSTEP_MUL(value) ((SYSCTRL_DFLL48MULSTEP_MUL_Msk & ((value) << SYSCTRL_DFLL48MULSTEP_MUL_Pos)))
#define SYSCTRL_DFLL48MULSTEP_FSTEP_Pos    16
#define SYSCTRL_DFLL48MULSTEP_FSTEP_Msk    (0xFFu << SYSCTRL_DFLL48MULSTEP_FSTEP_Pos) /**< \brief (SYSCTRL_DFLL48MULSTEP)  */
#define SYSCTRL_DFLL48MULSTEP_FSTEP(value) ((SYSCTRL_DFLL48MULSTEP_FSTEP_Msk & ((value) << SYSCTRL_DFLL48MULSTEP_FSTEP_Pos)))
#define SYSCTRL_DFLL48MULSTEP_CSTEP_Pos    24
#define SYSCTRL_DFLL48MULSTEP_CSTEP_Msk    (0x1Fu << SYSCTRL_DFLL48MULSTEP_CSTEP_Pos) /**< \brief (SYSCTRL_DFLL48MULSTEP)  */
#define SYSCTRL_DFLL48MULSTEP_CSTEP(value) ((SYSCTRL_DFLL48MULSTEP_CSTEP_Msk & ((value) << SYSCTRL_DFLL48MULSTEP_CSTEP_Pos)))
#define SYSCTRL_DFLL48MULSTEP_MASK  0x1FFFFFFFu /**< \brief (SYSCTRL_DFLL48MULSTEP) MASK Register */

/* -------- SYSCTRL_DFLL48SYNC : (SYSCTRL Offset: 0x24) (R/W  8) DFLL Synchronization Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:7;     /*!< bit:  0.. 6  Reserved                           */
    uint8_t  READREQ:1;        /*!< bit:      7  Readreq                            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} SYSCTRL_DFLL48SYNC_Type;

#define SYSCTRL_DFLL48SYNC_OFFSET   0x24         /**< \brief (SYSCTRL offset) DFLL Synchronization Register */
#define SYSCTRL_DFLL48SYNC_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) DFLL Synchronization Register */

#define SYSCTRL_DFLL48SYNC_READREQ  (0x1u <<  7) /**< \brief (SYSCTRL_DFLL48SYNC) Readreq */
#define SYSCTRL_DFLL48SYNC_MASK     0x80u /**< \brief (SYSCTRL_DFLL48SYNC) MASK Register */

/* -------- SYSCTRL_BOD33CTRL : (SYSCTRL Offset: 0x28) (R/W 16) BOD33 Control Register -------- */
typedef union {
  struct {
    uint16_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1                                     */
    uint16_t HYST:1;           /*!< bit:      2                                     */
    uint16_t ACTION:2;         /*!< bit:  3.. 4                                     */
    uint16_t _reserved1:3;     /*!< bit:  5.. 7  Reserved                           */
    uint16_t MODE:1;           /*!< bit:      8                                     */
    uint16_t CEN:1;            /*!< bit:      9                                     */
    uint16_t _reserved2:2;     /*!< bit: 10..11  Reserved                           */
    uint16_t PSEL:4;           /*!< bit: 12..15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_BOD33CTRL_Type;

#define SYSCTRL_BOD33CTRL_OFFSET    0x28         /**< \brief (SYSCTRL offset) BOD33 Control Register */

#define SYSCTRL_BOD33CTRL_ENABLE    (0x1u <<  1) /**< \brief (SYSCTRL_BOD33CTRL)  */
#define SYSCTRL_BOD33CTRL_HYST      (0x1u <<  2) /**< \brief (SYSCTRL_BOD33CTRL)  */
#define SYSCTRL_BOD33CTRL_ACTION_Pos    3
#define SYSCTRL_BOD33CTRL_ACTION_Msk    (0x3u << SYSCTRL_BOD33CTRL_ACTION_Pos) /**< \brief (SYSCTRL_BOD33CTRL)  */
#define SYSCTRL_BOD33CTRL_ACTION(value) ((SYSCTRL_BOD33CTRL_ACTION_Msk & ((value) << SYSCTRL_BOD33CTRL_ACTION_Pos)))
#define SYSCTRL_BOD33CTRL_MODE      (0x1u <<  8) /**< \brief (SYSCTRL_BOD33CTRL)  */
#define SYSCTRL_BOD33CTRL_CEN       (0x1u <<  9) /**< \brief (SYSCTRL_BOD33CTRL)  */
#define SYSCTRL_BOD33CTRL_PSEL_Pos    12
#define SYSCTRL_BOD33CTRL_PSEL_Msk    (0xFu << SYSCTRL_BOD33CTRL_PSEL_Pos) /**< \brief (SYSCTRL_BOD33CTRL)  */
#define SYSCTRL_BOD33CTRL_PSEL(value) ((SYSCTRL_BOD33CTRL_PSEL_Msk & ((value) << SYSCTRL_BOD33CTRL_PSEL_Pos)))
#define SYSCTRL_BOD33CTRL_MASK      0xF31Eu /**< \brief (SYSCTRL_BOD33CTRL) MASK Register */

/* -------- SYSCTRL_BOD33LEVEL : (SYSCTRL Offset: 0x2A) (R/W 16) BOD33 Level Register -------- */
typedef union {
  struct {
    uint16_t LEVEL:6;          /*!< bit:  0.. 5                                     */
    uint16_t _reserved0:10;    /*!< bit:  6..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_BOD33LEVEL_Type;

#define SYSCTRL_BOD33LEVEL_OFFSET   0x2A         /**< \brief (SYSCTRL offset) BOD33 Level Register */
#define SYSCTRL_BOD33LEVEL_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) BOD33 Level Register */

#define SYSCTRL_BOD33LEVEL_LEVEL_Pos    0
#define SYSCTRL_BOD33LEVEL_LEVEL_Msk    (0x3Fu << SYSCTRL_BOD33LEVEL_LEVEL_Pos) /**< \brief (SYSCTRL_BOD33LEVEL)  */
#define SYSCTRL_BOD33LEVEL_LEVEL(value) ((SYSCTRL_BOD33LEVEL_LEVEL_Msk & ((value) << SYSCTRL_BOD33LEVEL_LEVEL_Pos)))
#define SYSCTRL_BOD33LEVEL_MASK     0x3Fu /**< \brief (SYSCTRL_BOD33LEVEL) MASK Register */

/* -------- SYSCTRL_BOD12CTRL : (SYSCTRL Offset: 0x2C) (R/W 16) BOD12 Control Register -------- */
typedef union {
  struct {
    uint16_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1                                     */
    uint16_t HYST:1;           /*!< bit:      2                                     */
    uint16_t ACTION:2;         /*!< bit:  3.. 4                                     */
    uint16_t _reserved1:3;     /*!< bit:  5.. 7  Reserved                           */
    uint16_t MODE:1;           /*!< bit:      8                                     */
    uint16_t CEN:1;            /*!< bit:      9                                     */
    uint16_t _reserved2:2;     /*!< bit: 10..11  Reserved                           */
    uint16_t PSEL:4;           /*!< bit: 12..15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_BOD12CTRL_Type;

#define SYSCTRL_BOD12CTRL_OFFSET    0x2C         /**< \brief (SYSCTRL offset) BOD12 Control Register */

#define SYSCTRL_BOD12CTRL_ENABLE    (0x1u <<  1) /**< \brief (SYSCTRL_BOD12CTRL)  */
#define SYSCTRL_BOD12CTRL_HYST      (0x1u <<  2) /**< \brief (SYSCTRL_BOD12CTRL)  */
#define SYSCTRL_BOD12CTRL_ACTION_Pos    3
#define SYSCTRL_BOD12CTRL_ACTION_Msk    (0x3u << SYSCTRL_BOD12CTRL_ACTION_Pos) /**< \brief (SYSCTRL_BOD12CTRL)  */
#define SYSCTRL_BOD12CTRL_ACTION(value) ((SYSCTRL_BOD12CTRL_ACTION_Msk & ((value) << SYSCTRL_BOD12CTRL_ACTION_Pos)))
#define SYSCTRL_BOD12CTRL_MODE      (0x1u <<  8) /**< \brief (SYSCTRL_BOD12CTRL)  */
#define SYSCTRL_BOD12CTRL_CEN       (0x1u <<  9) /**< \brief (SYSCTRL_BOD12CTRL)  */
#define SYSCTRL_BOD12CTRL_PSEL_Pos    12
#define SYSCTRL_BOD12CTRL_PSEL_Msk    (0xFu << SYSCTRL_BOD12CTRL_PSEL_Pos) /**< \brief (SYSCTRL_BOD12CTRL)  */
#define SYSCTRL_BOD12CTRL_PSEL(value) ((SYSCTRL_BOD12CTRL_PSEL_Msk & ((value) << SYSCTRL_BOD12CTRL_PSEL_Pos)))
#define SYSCTRL_BOD12CTRL_MASK      0xF31Eu /**< \brief (SYSCTRL_BOD12CTRL) MASK Register */

/* -------- SYSCTRL_BOD12LEVEL : (SYSCTRL Offset: 0x2E) (R/W 16) BOD12 Control Register -------- */
typedef union {
  struct {
    uint16_t LEVEL:5;          /*!< bit:  0.. 4                                     */
    uint16_t _reserved0:11;    /*!< bit:  5..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_BOD12LEVEL_Type;

#define SYSCTRL_BOD12LEVEL_OFFSET   0x2E         /**< \brief (SYSCTRL offset) BOD12 Control Register */
#define SYSCTRL_BOD12LEVEL_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) BOD12 Control Register */

#define SYSCTRL_BOD12LEVEL_LEVEL_Pos    0
#define SYSCTRL_BOD12LEVEL_LEVEL_Msk    (0x1Fu << SYSCTRL_BOD12LEVEL_LEVEL_Pos) /**< \brief (SYSCTRL_BOD12LEVEL)  */
#define SYSCTRL_BOD12LEVEL_LEVEL(value) ((SYSCTRL_BOD12LEVEL_LEVEL_Msk & ((value) << SYSCTRL_BOD12LEVEL_LEVEL_Pos)))
#define SYSCTRL_BOD12LEVEL_MASK     0x1Fu /**< \brief (SYSCTRL_BOD12LEVEL) MASK Register */

/* -------- SYSCTRL_OSC32K : (SYSCTRL Offset: 0x30) (R/W 32) OSC32K Control Register -------- */
typedef union {
  struct {
    uint32_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  OSC32K Enable                      */
    uint32_t EN32K:1;          /*!< bit:      2  OSC32K 32K enable                  */
    uint32_t EN1K:1;           /*!< bit:      3  OSC32K 1K enable                   */
    uint32_t _reserved1:4;     /*!< bit:  4.. 7  Reserved                           */
    uint32_t STARTUP:3;        /*!< bit:  8..10  OSC32K startup                     */
    uint32_t _reserved2:5;     /*!< bit: 11..15  Reserved                           */
    uint32_t CALIB:7;          /*!< bit: 16..22                                     */
    uint32_t _reserved3:9;     /*!< bit: 23..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_OSC32K_Type;

#define SYSCTRL_OSC32K_OFFSET       0x30         /**< \brief (SYSCTRL offset) OSC32K Control Register */
#define SYSCTRL_OSC32K_RESETVALUE   0x0          /**< \brief (SYSCTRL reset_value) OSC32K Control Register */

#define SYSCTRL_OSC32K_ENABLE       (0x1u <<  1) /**< \brief (SYSCTRL_OSC32K) OSC32K Enable */
#define SYSCTRL_OSC32K_EN32K        (0x1u <<  2) /**< \brief (SYSCTRL_OSC32K) OSC32K 32K enable */
#define SYSCTRL_OSC32K_EN1K         (0x1u <<  3) /**< \brief (SYSCTRL_OSC32K) OSC32K 1K enable */
#define SYSCTRL_OSC32K_STARTUP_Pos    8
#define SYSCTRL_OSC32K_STARTUP_Msk    (0x7u << SYSCTRL_OSC32K_STARTUP_Pos) /**< \brief (SYSCTRL_OSC32K) OSC32K startup */
#define SYSCTRL_OSC32K_STARTUP(value) ((SYSCTRL_OSC32K_STARTUP_Msk & ((value) << SYSCTRL_OSC32K_STARTUP_Pos)))
#define SYSCTRL_OSC32K_CALIB_Pos    16
#define SYSCTRL_OSC32K_CALIB_Msk    (0x7Fu << SYSCTRL_OSC32K_CALIB_Pos) /**< \brief (SYSCTRL_OSC32K)  */
#define SYSCTRL_OSC32K_CALIB(value) ((SYSCTRL_OSC32K_CALIB_Msk & ((value) << SYSCTRL_OSC32K_CALIB_Pos)))
#define SYSCTRL_OSC32K_MASK         0x7F070Eu /**< \brief (SYSCTRL_OSC32K) MASK Register */

/* -------- SYSCTRL_OSCULP32K : (SYSCTRL Offset: 0x34) (R/W 32) OSCULP32K Control Register -------- */
typedef union {
  struct {
    uint32_t CALIB:5;          /*!< bit:  0.. 4                                     */
    uint32_t _reserved0:27;    /*!< bit:  5..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_OSCULP32K_Type;

#define SYSCTRL_OSCULP32K_OFFSET    0x34         /**< \brief (SYSCTRL offset) OSCULP32K Control Register */
#define SYSCTRL_OSCULP32K_RESETVALUE 0xF0002      /**< \brief (SYSCTRL reset_value) OSCULP32K Control Register */

#define SYSCTRL_OSCULP32K_CALIB_Pos    0
#define SYSCTRL_OSCULP32K_CALIB_Msk    (0x1Fu << SYSCTRL_OSCULP32K_CALIB_Pos) /**< \brief (SYSCTRL_OSCULP32K)  */
#define SYSCTRL_OSCULP32K_CALIB(value) ((SYSCTRL_OSCULP32K_CALIB_Msk & ((value) << SYSCTRL_OSCULP32K_CALIB_Pos)))
#define SYSCTRL_OSCULP32K_MASK      0x1Fu /**< \brief (SYSCTRL_OSCULP32K) MASK Register */

/* -------- SYSCTRL_OSCULP32KTEST : (SYSCTRL Offset: 0x38) (R/W  8) OSCULP32K Test Register -------- */
typedef union {
  struct {
    uint8_t  DISABLE:1;        /*!< bit:      0                                     */
    uint8_t  BIASOUTEN:1;      /*!< bit:      1                                     */
    uint8_t  CONTREF:1;        /*!< bit:      2                                     */
    uint8_t  _reserved0:5;     /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} SYSCTRL_OSCULP32KTEST_Type;

#define SYSCTRL_OSCULP32KTEST_OFFSET 0x38         /**< \brief (SYSCTRL offset) OSCULP32K Test Register */
#define SYSCTRL_OSCULP32KTEST_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) OSCULP32K Test Register */

#define SYSCTRL_OSCULP32KTEST_DISABLE (0x1u <<  0) /**< \brief (SYSCTRL_OSCULP32KTEST)  */
#define SYSCTRL_OSCULP32KTEST_BIASOUTEN (0x1u <<  1) /**< \brief (SYSCTRL_OSCULP32KTEST)  */
#define SYSCTRL_OSCULP32KTEST_CONTREF (0x1u <<  2) /**< \brief (SYSCTRL_OSCULP32KTEST)  */
#define SYSCTRL_OSCULP32KTEST_MASK  0x7u /**< \brief (SYSCTRL_OSCULP32KTEST) MASK Register */

/* -------- SYSCTRL_OSC8M : (SYSCTRL Offset: 0x3C) (R/W 32) OSC8M Control Register A -------- */
typedef union {
  struct {
    uint32_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1                                     */
    uint32_t _reserved1:6;     /*!< bit:  2.. 7  Reserved                           */
    uint32_t PRESC:2;          /*!< bit:  8.. 9                                     */
    uint32_t _reserved2:6;     /*!< bit: 10..15  Reserved                           */
    uint32_t CALIB:12;         /*!< bit: 16..27                                     */
    uint32_t _reserved3:2;     /*!< bit: 28..29  Reserved                           */
    uint32_t FRANGE:2;         /*!< bit: 30..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_OSC8M_Type;

#define SYSCTRL_OSC8M_OFFSET        0x3C         /**< \brief (SYSCTRL offset) OSC8M Control Register A */
#define SYSCTRL_OSC8M_RESETVALUE    0x80         /**< \brief (SYSCTRL reset_value) OSC8M Control Register A */

#define SYSCTRL_OSC8M_ENABLE        (0x1u <<  1) /**< \brief (SYSCTRL_OSC8M)  */
#define SYSCTRL_OSC8M_PRESC_Pos     8
#define SYSCTRL_OSC8M_PRESC_Msk     (0x3u << SYSCTRL_OSC8M_PRESC_Pos) /**< \brief (SYSCTRL_OSC8M)  */
#define SYSCTRL_OSC8M_PRESC(value)  ((SYSCTRL_OSC8M_PRESC_Msk & ((value) << SYSCTRL_OSC8M_PRESC_Pos)))
#define SYSCTRL_OSC8M_CALIB_Pos     16
#define SYSCTRL_OSC8M_CALIB_Msk     (0xFFFu << SYSCTRL_OSC8M_CALIB_Pos) /**< \brief (SYSCTRL_OSC8M)  */
#define SYSCTRL_OSC8M_CALIB(value)  ((SYSCTRL_OSC8M_CALIB_Msk & ((value) << SYSCTRL_OSC8M_CALIB_Pos)))
#define SYSCTRL_OSC8M_FRANGE_Pos    30
#define SYSCTRL_OSC8M_FRANGE_Msk    (0x3u << SYSCTRL_OSC8M_FRANGE_Pos) /**< \brief (SYSCTRL_OSC8M)  */
#define SYSCTRL_OSC8M_FRANGE(value) ((SYSCTRL_OSC8M_FRANGE_Msk & ((value) << SYSCTRL_OSC8M_FRANGE_Pos)))
#define SYSCTRL_OSC8M_MASK          0xCFFF0302u /**< \brief (SYSCTRL_OSC8M) MASK Register */

/* -------- SYSCTRL_VREGCTRL : (SYSCTRL Offset: 0x40) (R/W 16) VREG Control Register -------- */
typedef union {
  struct {
    uint16_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1                                     */
    uint16_t _reserved1:6;     /*!< bit:  2.. 7  Reserved                           */
    uint16_t LEVEL:3;          /*!< bit:  8..10                                     */
    uint16_t _reserved2:1;     /*!< bit:     11  Reserved                           */
    uint16_t CALIB:4;          /*!< bit: 12..15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREGCTRL_Type;

#define SYSCTRL_VREGCTRL_OFFSET     0x40         /**< \brief (SYSCTRL offset) VREG Control Register */

#define SYSCTRL_VREGCTRL_ENABLE     (0x1u <<  1) /**< \brief (SYSCTRL_VREGCTRL)  */
#define SYSCTRL_VREGCTRL_LEVEL_Pos    8
#define SYSCTRL_VREGCTRL_LEVEL_Msk    (0x7u << SYSCTRL_VREGCTRL_LEVEL_Pos) /**< \brief (SYSCTRL_VREGCTRL)  */
#define SYSCTRL_VREGCTRL_LEVEL(value) ((SYSCTRL_VREGCTRL_LEVEL_Msk & ((value) << SYSCTRL_VREGCTRL_LEVEL_Pos)))
#define SYSCTRL_VREGCTRL_CALIB_Pos    12
#define SYSCTRL_VREGCTRL_CALIB_Msk    (0xFu << SYSCTRL_VREGCTRL_CALIB_Pos) /**< \brief (SYSCTRL_VREGCTRL)  */
#define SYSCTRL_VREGCTRL_CALIB(value) ((SYSCTRL_VREGCTRL_CALIB_Msk & ((value) << SYSCTRL_VREGCTRL_CALIB_Pos)))
#define SYSCTRL_VREGCTRL_MASK       0xF702u /**< \brief (SYSCTRL_VREGCTRL) MASK Register */

/* -------- SYSCTRL_VREGTEST : (SYSCTRL Offset: 0x44) (R/W 32) VREG Test Register -------- */
typedef union {
  struct {
    uint32_t ULPENABLE:1;      /*!< bit:      0                                     */
    uint32_t BODIENABLE:1;     /*!< bit:      1                                     */
    uint32_t BODIIENABLE:1;    /*!< bit:      2                                     */
    uint32_t OSCENABLE:1;      /*!< bit:      3                                     */
    uint32_t LDOENABLE:1;      /*!< bit:      4                                     */
    uint32_t REFSEL:1;         /*!< bit:      5                                     */
    uint32_t _reserved0:2;     /*!< bit:  6.. 7  Reserved                           */
    uint32_t FORCEGAIN:2;      /*!< bit:  8.. 9                                     */
    uint32_t RAMREFSEL:3;      /*!< bit: 10..12                                     */
    uint32_t _reserved1:3;     /*!< bit: 13..15  Reserved                           */
    uint32_t REQBIAS:1;        /*!< bit:     16                                     */
    uint32_t REQHELPI:1;       /*!< bit:     17                                     */
    uint32_t REQHELPII:1;      /*!< bit:     18                                     */
    uint32_t _reserved2:12;    /*!< bit: 19..30  Reserved                           */
    uint32_t TESTEN:1;         /*!< bit:     31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREGTEST_Type;

#define SYSCTRL_VREGTEST_OFFSET     0x44         /**< \brief (SYSCTRL offset) VREG Test Register */
#define SYSCTRL_VREGTEST_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) VREG Test Register */

#define SYSCTRL_VREGTEST_ULPENABLE  (0x1u <<  0) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_BODIENABLE (0x1u <<  1) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_BODIIENABLE (0x1u <<  2) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_OSCENABLE  (0x1u <<  3) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_LDOENABLE  (0x1u <<  4) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_REFSEL     (0x1u <<  5) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_FORCEGAIN_Pos    8
#define SYSCTRL_VREGTEST_FORCEGAIN_Msk    (0x3u << SYSCTRL_VREGTEST_FORCEGAIN_Pos) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_FORCEGAIN(value) ((SYSCTRL_VREGTEST_FORCEGAIN_Msk & ((value) << SYSCTRL_VREGTEST_FORCEGAIN_Pos)))
#define SYSCTRL_VREGTEST_RAMREFSEL_Pos    10
#define SYSCTRL_VREGTEST_RAMREFSEL_Msk    (0x7u << SYSCTRL_VREGTEST_RAMREFSEL_Pos) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_RAMREFSEL(value) ((SYSCTRL_VREGTEST_RAMREFSEL_Msk & ((value) << SYSCTRL_VREGTEST_RAMREFSEL_Pos)))
#define SYSCTRL_VREGTEST_REQBIAS    (0x1u << 16) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_REQHELPI   (0x1u << 17) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_REQHELPII  (0x1u << 18) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_TESTEN     (0x1u << 31) /**< \brief (SYSCTRL_VREGTEST)  */
#define SYSCTRL_VREGTEST_MASK       0x80071F3Fu /**< \brief (SYSCTRL_VREGTEST) MASK Register */

/* -------- SYSCTRL_VREFCTRLA : (SYSCTRL Offset: 0x48) (R/W 16) VREF Control Register A -------- */
typedef union {
  struct {
    uint16_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint16_t TSEN:1;           /*!< bit:      1                                     */
    uint16_t BGOUTEN:1;        /*!< bit:      2                                     */
    uint16_t _reserved1:13;    /*!< bit:  3..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREFCTRLA_Type;

#define SYSCTRL_VREFCTRLA_OFFSET    0x48         /**< \brief (SYSCTRL offset) VREF Control Register A */
#define SYSCTRL_VREFCTRLA_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) VREF Control Register A */

#define SYSCTRL_VREFCTRLA_TSEN      (0x1u <<  1) /**< \brief (SYSCTRL_VREFCTRLA)  */
#define SYSCTRL_VREFCTRLA_BGOUTEN   (0x1u <<  2) /**< \brief (SYSCTRL_VREFCTRLA)  */
#define SYSCTRL_VREFCTRLA_MASK      0x6u /**< \brief (SYSCTRL_VREFCTRLA) MASK Register */

/* -------- SYSCTRL_VREFCTRLB : (SYSCTRL Offset: 0x4A) (R/W 16) VREF Control Register B -------- */
typedef union {
  struct {
    uint16_t CALIB:11;         /*!< bit:  0..10                                     */
    uint16_t _reserved0:5;     /*!< bit: 11..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREFCTRLB_Type;

#define SYSCTRL_VREFCTRLB_OFFSET    0x4A         /**< \brief (SYSCTRL offset) VREF Control Register B */
#define SYSCTRL_VREFCTRLB_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) VREF Control Register B */

#define SYSCTRL_VREFCTRLB_CALIB_Pos    0
#define SYSCTRL_VREFCTRLB_CALIB_Msk    (0x7FFu << SYSCTRL_VREFCTRLB_CALIB_Pos) /**< \brief (SYSCTRL_VREFCTRLB)  */
#define SYSCTRL_VREFCTRLB_CALIB(value) ((SYSCTRL_VREFCTRLB_CALIB_Msk & ((value) << SYSCTRL_VREFCTRLB_CALIB_Pos)))
#define SYSCTRL_VREFCTRLB_MASK      0x7FFu /**< \brief (SYSCTRL_VREFCTRLB) MASK Register */

/* -------- SYSCTRL_VREFTESTA : (SYSCTRL Offset: 0x4C) (R/W 32) VREF Test Register -------- */
typedef union {
  struct {
    uint32_t BIASEN:1;         /*!< bit:      0                                     */
    uint32_t BGEN:1;           /*!< bit:      1                                     */
    uint32_t _reserved0:6;     /*!< bit:  2.. 7  Reserved                           */
    uint32_t BGBUFEN:8;        /*!< bit:  8..15                                     */
    uint32_t BGRDY:1;          /*!< bit:     16                                     */
    uint32_t BIASRDY:1;        /*!< bit:     17                                     */
    uint32_t ACBUFRDY:1;       /*!< bit:     18                                     */
    uint32_t ADCBUFRDY:1;      /*!< bit:     19                                     */
    uint32_t DACBUFRDY:1;      /*!< bit:     20                                     */
    uint32_t VDOUBBUFRDY:1;    /*!< bit:     21                                     */
    uint32_t BUFRRRDY:1;       /*!< bit:     22                                     */
    uint32_t DIV2BUFRDY:1;     /*!< bit:     23                                     */
    uint32_t BUFRRSEL:2;       /*!< bit: 24..25                                     */
    uint32_t _reserved1:5;     /*!< bit: 26..30  Reserved                           */
    uint32_t TESTEN:1;         /*!< bit:     31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREFTESTA_Type;

#define SYSCTRL_VREFTESTA_OFFSET    0x4C         /**< \brief (SYSCTRL offset) VREF Test Register */
#define SYSCTRL_VREFTESTA_RESETVALUE 0x0          /**< \brief (SYSCTRL reset_value) VREF Test Register */

#define SYSCTRL_VREFTESTA_BIASEN    (0x1u <<  0) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_BGEN      (0x1u <<  1) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_BGBUFEN_Pos    8
#define SYSCTRL_VREFTESTA_BGBUFEN_Msk    (0xFFu << SYSCTRL_VREFTESTA_BGBUFEN_Pos) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_BGBUFEN(value) ((SYSCTRL_VREFTESTA_BGBUFEN_Msk & ((value) << SYSCTRL_VREFTESTA_BGBUFEN_Pos)))
#define SYSCTRL_VREFTESTA_BGRDY     (0x1u << 16) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_BIASRDY   (0x1u << 17) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_ACBUFRDY  (0x1u << 18) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_ADCBUFRDY (0x1u << 19) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_DACBUFRDY (0x1u << 20) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_VDOUBBUFRDY (0x1u << 21) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_BUFRRRDY  (0x1u << 22) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_DIV2BUFRDY (0x1u << 23) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_BUFRRSEL_Pos    24
#define SYSCTRL_VREFTESTA_BUFRRSEL_Msk    (0x3u << SYSCTRL_VREFTESTA_BUFRRSEL_Pos) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_BUFRRSEL(value) ((SYSCTRL_VREFTESTA_BUFRRSEL_Msk & ((value) << SYSCTRL_VREFTESTA_BUFRRSEL_Pos)))
#define SYSCTRL_VREFTESTA_TESTEN    (0x1u << 31) /**< \brief (SYSCTRL_VREFTESTA)  */
#define SYSCTRL_VREFTESTA_MASK      0x83FFFF03u /**< \brief (SYSCTRL_VREFTESTA) MASK Register */

/* -------- SYSCTRL_VREFTESTB : (SYSCTRL Offset: 0x50) (R/W 16)  -------- */
typedef union {
  struct {
    uint16_t SAMPLECLK:2;      /*!< bit:  0.. 1                                     */
    uint16_t DIVLEV0:3;        /*!< bit:  2.. 4                                     */
    uint16_t DIVLEV1:3;        /*!< bit:  5.. 7                                     */
    uint16_t DIVLEV2:3;        /*!< bit:  8..10                                     */
    uint16_t DIVLEV3:3;        /*!< bit: 11..13                                     */
    uint16_t _reserved0:1;     /*!< bit:     14  Reserved                           */
    uint16_t TESTEN:1;         /*!< bit:     15                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREFTESTB_Type;

#define SYSCTRL_VREFTESTB_OFFSET    0x50         /**< \brief (SYSCTRL offset)  */

#define SYSCTRL_VREFTESTB_SAMPLECLK_Pos    0
#define SYSCTRL_VREFTESTB_SAMPLECLK_Msk    (0x3u << SYSCTRL_VREFTESTB_SAMPLECLK_Pos) /**< \brief (SYSCTRL_VREFTESTB)  */
#define SYSCTRL_VREFTESTB_SAMPLECLK(value) ((SYSCTRL_VREFTESTB_SAMPLECLK_Msk & ((value) << SYSCTRL_VREFTESTB_SAMPLECLK_Pos)))
#define SYSCTRL_VREFTESTB_DIVLEV0_Pos    2
#define SYSCTRL_VREFTESTB_DIVLEV0_Msk    (0x7u << SYSCTRL_VREFTESTB_DIVLEV0_Pos) /**< \brief (SYSCTRL_VREFTESTB)  */
#define SYSCTRL_VREFTESTB_DIVLEV0(value) ((SYSCTRL_VREFTESTB_DIVLEV0_Msk & ((value) << SYSCTRL_VREFTESTB_DIVLEV0_Pos)))
#define SYSCTRL_VREFTESTB_DIVLEV1_Pos    5
#define SYSCTRL_VREFTESTB_DIVLEV1_Msk    (0x7u << SYSCTRL_VREFTESTB_DIVLEV1_Pos) /**< \brief (SYSCTRL_VREFTESTB)  */
#define SYSCTRL_VREFTESTB_DIVLEV1(value) ((SYSCTRL_VREFTESTB_DIVLEV1_Msk & ((value) << SYSCTRL_VREFTESTB_DIVLEV1_Pos)))
#define SYSCTRL_VREFTESTB_DIVLEV2_Pos    8
#define SYSCTRL_VREFTESTB_DIVLEV2_Msk    (0x7u << SYSCTRL_VREFTESTB_DIVLEV2_Pos) /**< \brief (SYSCTRL_VREFTESTB)  */
#define SYSCTRL_VREFTESTB_DIVLEV2(value) ((SYSCTRL_VREFTESTB_DIVLEV2_Msk & ((value) << SYSCTRL_VREFTESTB_DIVLEV2_Pos)))
#define SYSCTRL_VREFTESTB_DIVLEV3_Pos    11
#define SYSCTRL_VREFTESTB_DIVLEV3_Msk    (0x7u << SYSCTRL_VREFTESTB_DIVLEV3_Pos) /**< \brief (SYSCTRL_VREFTESTB)  */
#define SYSCTRL_VREFTESTB_DIVLEV3(value) ((SYSCTRL_VREFTESTB_DIVLEV3_Msk & ((value) << SYSCTRL_VREFTESTB_DIVLEV3_Pos)))
#define SYSCTRL_VREFTESTB_TESTEN    (0x1u << 15) /**< \brief (SYSCTRL_VREFTESTB)  */
#define SYSCTRL_VREFTESTB_MASK      0xBFFFu /**< \brief (SYSCTRL_VREFTESTB) MASK Register */

/* -------- SYSCTRL_FORCECLKON : (SYSCTRL Offset: 0x54) (R/W 32)  -------- */
typedef union {
  struct {
    uint32_t XOSCON:1;         /*!< bit:      0                                     */
    uint32_t XOSC32KON:1;      /*!< bit:      1                                     */
    uint32_t DFLLON:1;         /*!< bit:      2                                     */
    uint32_t OSC32KON:1;       /*!< bit:      3                                     */
    uint32_t OSC8MON:1;        /*!< bit:      4                                     */
    uint32_t _reserved0:27;    /*!< bit:  5..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_FORCECLKON_Type;

#define SYSCTRL_FORCECLKON_OFFSET   0x54         /**< \brief (SYSCTRL offset)  */

#define SYSCTRL_FORCECLKON_XOSCON   (0x1u <<  0) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_XOSC32KON (0x1u <<  1) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_DFLLON   (0x1u <<  2) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_OSC32KON (0x1u <<  3) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_OSC8MON  (0x1u <<  4) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_MASK     0x1Fu /**< \brief (SYSCTRL_FORCECLKON) MASK Register */

/* -------- SYSCTRL_TESTMODE : (SYSCTRL Offset: 0x58) (R/W 32)  -------- */
typedef union {
  struct {
    uint32_t XOSC_TESTMODE:1;  /*!< bit:      0                                     */
    uint32_t XOSC32_TESTMODE:1; /*!< bit:      1                                     */
    uint32_t DFLL_TESTMODE:1;  /*!< bit:      2                                     */
    uint32_t BOD33_TESTMODE:1; /*!< bit:      3                                     */
    uint32_t BOD12_TESTMODE:1; /*!< bit:      4                                     */
    uint32_t OSC32K_TESTMODE:1; /*!< bit:      5                                     */
    uint32_t OSCULP32K_TESTMODE:1; /*!< bit:      6                                     */
    uint32_t OSC8M_TESTMODE:1; /*!< bit:      7                                     */
    uint32_t VREG_TESTMODE:1;  /*!< bit:      8                                     */
    uint32_t VREF_TESTMODE:1;  /*!< bit:      9                                     */
    uint32_t ALLCLK_TESTMODE:1; /*!< bit:     10                                     */
    uint32_t POR33_TESTMODE:1; /*!< bit:     11                                     */
    uint32_t _reserved0:20;    /*!< bit: 12..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_TESTMODE_Type;

#define SYSCTRL_TESTMODE_OFFSET     0x58         /**< \brief (SYSCTRL offset)  */

#define SYSCTRL_TESTMODE_XOSC_TESTMODE (0x1u <<  0) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_XOSC32_TESTMODE (0x1u <<  1) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_DFLL_TESTMODE (0x1u <<  2) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_BOD33_TESTMODE (0x1u <<  3) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_BOD12_TESTMODE (0x1u <<  4) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_OSC32K_TESTMODE (0x1u <<  5) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_OSCULP32K_TESTMODE (0x1u <<  6) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_OSC8M_TESTMODE (0x1u <<  7) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_VREG_TESTMODE (0x1u <<  8) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_VREF_TESTMODE (0x1u <<  9) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_ALLCLK_TESTMODE (0x1u << 10) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_POR33_TESTMODE (0x1u << 11) /**< \brief (SYSCTRL_TESTMODE)  */
#define SYSCTRL_TESTMODE_MASK       0xFFFu /**< \brief (SYSCTRL_TESTMODE) MASK Register */

/** \brief SYSCTRL hardware registers */
typedef struct {
  __IO SYSCTRL_INTENCLR_Type     INTENCLR;    /**< \brief Offset: 0x00 (R/W 32) Interrupt Enable Register */
  __IO SYSCTRL_INTENSET_Type     INTENSET;    /**< \brief Offset: 0x04 (R/W 32) Interrupt Clear Register */
  __IO SYSCTRL_INTFLAG_Type      INTFLAG;     /**< \brief Offset: 0x08 (R/W 32) Interrupt Flag Register */
  __I  SYSCTRL_PCLKSR_Type       PCLKSR;      /**< \brief Offset: 0x0C (R/  32) Power and Clocks Status Register */
  __IO SYSCTRL_XOSC_Type         XOSC;        /**< \brief Offset: 0x10 (R/W 16) XOSC Control Register */
       RoReg8                    Reserved1[0x2];
  __IO SYSCTRL_XOSC32K_Type      XOSC32K;     /**< \brief Offset: 0x14 (R/W 16) XOSC32K Control Register */
  __IO SYSCTRL_XOSC32KTEST_Type  XOSC32KTEST; /**< \brief Offset: 0x16 (R/W  8) XOSC32K Test Register */
       RoReg8                    Reserved2[0x1];
  __IO SYSCTRL_DFLL48CTRL_Type   DFLL48CTRL;  /**< \brief Offset: 0x18 (R/W 32) DFLL  Config Register */
  __IO SYSCTRL_DFLL48VALDIFF_Type DFLL48VALDIFF; /**< \brief Offset: 0x1C (R/W 32) DFLL Calibration Value Register */
  __IO SYSCTRL_DFLL48MULSTEP_Type DFLL48MULSTEP; /**< \brief Offset: 0x20 (R/W 32) DFLL Multiplier Register */
  __IO SYSCTRL_DFLL48SYNC_Type   DFLL48SYNC;  /**< \brief Offset: 0x24 (R/W  8) DFLL Synchronization Register */
       RoReg8                    Reserved3[0x3];
  __IO SYSCTRL_BOD33CTRL_Type    BOD33CTRL;   /**< \brief Offset: 0x28 (R/W 16) BOD33 Control Register */
  __IO SYSCTRL_BOD33LEVEL_Type   BOD33LEVEL;  /**< \brief Offset: 0x2A (R/W 16) BOD33 Level Register */
  __IO SYSCTRL_BOD12CTRL_Type    BOD12CTRL;   /**< \brief Offset: 0x2C (R/W 16) BOD12 Control Register */
  __IO SYSCTRL_BOD12LEVEL_Type   BOD12LEVEL;  /**< \brief Offset: 0x2E (R/W 16) BOD12 Control Register */
  __IO SYSCTRL_OSC32K_Type       OSC32K;      /**< \brief Offset: 0x30 (R/W 32) OSC32K Control Register */
  __IO SYSCTRL_OSCULP32K_Type    OSCULP32K;   /**< \brief Offset: 0x34 (R/W 32) OSCULP32K Control Register */
  __IO SYSCTRL_OSCULP32KTEST_Type OSCULP32KTEST; /**< \brief Offset: 0x38 (R/W  8) OSCULP32K Test Register */
       RoReg8                    Reserved4[0x3];
  __IO SYSCTRL_OSC8M_Type        OSC8M;       /**< \brief Offset: 0x3C (R/W 32) OSC8M Control Register A */
  __IO SYSCTRL_VREGCTRL_Type     VREGCTRL;    /**< \brief Offset: 0x40 (R/W 16) VREG Control Register */
       RoReg8                    Reserved5[0x2];
  __IO SYSCTRL_VREGTEST_Type     VREGTEST;    /**< \brief Offset: 0x44 (R/W 32) VREG Test Register */
  __IO SYSCTRL_VREFCTRLA_Type    VREFCTRLA;   /**< \brief Offset: 0x48 (R/W 16) VREF Control Register A */
  __IO SYSCTRL_VREFCTRLB_Type    VREFCTRLB;   /**< \brief Offset: 0x4A (R/W 16) VREF Control Register B */
  __IO SYSCTRL_VREFTESTA_Type    VREFTESTA;   /**< \brief Offset: 0x4C (R/W 32) VREF Test Register */
  __IO SYSCTRL_VREFTESTB_Type    VREFTESTB;   /**< \brief Offset: 0x50 (R/W 16)  */
       RoReg8                    Reserved6[0x2];
  __IO SYSCTRL_FORCECLKON_Type   FORCECLKON;  /**< \brief Offset: 0x54 (R/W 32)  */
  __IO SYSCTRL_TESTMODE_Type     TESTMODE;    /**< \brief Offset: 0x58 (R/W 32)  */
} Sysctrl;

#endif /*  __EXCLUDE_SYSCTRL_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR TC */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_TC Basic Timer Counter */
/*@{*/

#define REV_TC         0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_TC_FROM_CMSIS__

/* -------- TC_CTRLA : (TC Offset: 0x00) (R/W 16) Control Register A -------- */
typedef union {
  struct {
    uint16_t _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  Timer Counter mode                 */
    uint16_t _reserved1:1;     /*!< bit:      4  Reserved                           */
    uint16_t WAVEGEN:2;        /*!< bit:  5.. 6  Waveform Generation operation      */
    uint16_t _reserved2:1;     /*!< bit:      7  Reserved                           */
    uint16_t PRESCALER:3;      /*!< bit:  8..10  Prescaler                          */
    uint16_t SLEEPEN:1;        /*!< bit:     11  Run during Sleep mode              */
    uint16_t PRESCSYNC:2;      /*!< bit: 12..13  Prescaler and counter Synchronization */
    uint16_t _reserved3:2;     /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_CTRLA_Type;

#define TC_CTRLA_OFFSET             0x00         /**< \brief (TC offset) Control Register A */
#define TC_CTRLA_RESETVALUE         0x0          /**< \brief (TC reset_value) Control Register A */

#define TC_CTRLA_ENABLE             (0x1u <<  1) /**< \brief (TC_CTRLA) Enable */
#define TC_CTRLA_MODE_Pos           2
#define TC_CTRLA_MODE_Msk           (0x3u << TC_CTRLA_MODE_Pos) /**< \brief (TC_CTRLA) Timer Counter mode */
#define TC_CTRLA_MODE(value)        ((TC_CTRLA_MODE_Msk & ((value) << TC_CTRLA_MODE_Pos)))
#define   TC_CTRLA_MODE_COUNT16     (0x0u <<  2) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_MODE_COUNT8      (0x1u <<  2) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_MODE_COUNT32     (0x2u <<  2) /**< \brief (TC_CTRLA)  */
#define TC_CTRLA_WAVEGEN_Pos        5
#define TC_CTRLA_WAVEGEN_Msk        (0x3u << TC_CTRLA_WAVEGEN_Pos) /**< \brief (TC_CTRLA) Waveform Generation operation */
#define TC_CTRLA_WAVEGEN(value)     ((TC_CTRLA_WAVEGEN_Msk & ((value) << TC_CTRLA_WAVEGEN_Pos)))
#define   TC_CTRLA_WAVEGEN_NFRQ     (0x0u <<  5) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_MFRQ     (0x1u <<  5) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_NPWM     (0x2u <<  5) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_MPWM     (0x3u <<  5) /**< \brief (TC_CTRLA)  */
#define TC_CTRLA_PRESCALER_Pos      8
#define TC_CTRLA_PRESCALER_Msk      (0x7u << TC_CTRLA_PRESCALER_Pos) /**< \brief (TC_CTRLA) Prescaler */
#define TC_CTRLA_PRESCALER(value)   ((TC_CTRLA_PRESCALER_Msk & ((value) << TC_CTRLA_PRESCALER_Pos)))
#define TC_CTRLA_SLEEPEN            (0x1u << 11) /**< \brief (TC_CTRLA) Run during Sleep mode */
#define TC_CTRLA_PRESCSYNC_Pos      12
#define TC_CTRLA_PRESCSYNC_Msk      (0x3u << TC_CTRLA_PRESCSYNC_Pos) /**< \brief (TC_CTRLA) Prescaler and counter Synchronization */
#define TC_CTRLA_PRESCSYNC(value)   ((TC_CTRLA_PRESCSYNC_Msk & ((value) << TC_CTRLA_PRESCSYNC_Pos)))
#define   TC_CTRLA_PRESCSYNC_GCLK   (0x0u << 12) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_PRESCSYNC_PRESC  (0x1u << 12) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_PRESCSYNC_RESYNC (0x2u << 12) /**< \brief (TC_CTRLA)  */
#define TC_CTRLA_MASK               0x3F6Eu /**< \brief (TC_CTRLA) MASK Register */

/* -------- TC_READREQ : (TC Offset: 0x02) (R/W 16) Read Request Register -------- */
typedef union {
  struct {
    uint16_t ADDR:5;           /*!< bit:  0.. 4  Address                            */
    uint16_t _reserved0:9;     /*!< bit:  5..13  Reserved                           */
    uint16_t RCONT:1;          /*!< bit:     14  Read Continuously                  */
    uint16_t RREQ:1;           /*!< bit:     15  Read Request                       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_READREQ_Type;

#define TC_READREQ_OFFSET           0x02         /**< \brief (TC offset) Read Request Register */
#define TC_READREQ_RESETVALUE       0x0          /**< \brief (TC reset_value) Read Request Register */

#define TC_READREQ_ADDR_Pos         0
#define TC_READREQ_ADDR_Msk         (0x1Fu << TC_READREQ_ADDR_Pos) /**< \brief (TC_READREQ) Address */
#define TC_READREQ_ADDR(value)      ((TC_READREQ_ADDR_Msk & ((value) << TC_READREQ_ADDR_Pos)))
#define TC_READREQ_RCONT            (0x1u << 14) /**< \brief (TC_READREQ) Read Continuously */
#define TC_READREQ_RREQ             (0x1u << 15) /**< \brief (TC_READREQ) Read Request */
#define TC_READREQ_MASK             0xC01Fu /**< \brief (TC_READREQ) MASK Register */

/* -------- TC_CTRLBCLR : (TC Offset: 0x04) (R/W  8) Control Clear Register B -------- */
typedef union {
  struct {
    uint8_t  DIR:1;            /*!< bit:      0  Counter Direction                  */
    uint8_t  _reserved0:1;     /*!< bit:      1  Reserved                           */
    uint8_t  ONESHOT:1;        /*!< bit:      2  Oneshot on counter                 */
    uint8_t  _reserved1:3;     /*!< bit:  3.. 5  Reserved                           */
    uint8_t  CMD:2;            /*!< bit:  6.. 7  Command                            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLBCLR_Type;

#define TC_CTRLBCLR_OFFSET          0x04         /**< \brief (TC offset) Control Clear Register B */
#define TC_CTRLBCLR_RESETVALUE      0x0          /**< \brief (TC reset_value) Control Clear Register B */

#define TC_CTRLBCLR_DIR             (0x1u <<  0) /**< \brief (TC_CTRLBCLR) Counter Direction */
#define TC_CTRLBCLR_ONESHOT         (0x1u <<  2) /**< \brief (TC_CTRLBCLR) Oneshot on counter */
#define TC_CTRLBCLR_CMD_Pos         6
#define TC_CTRLBCLR_CMD_Msk         (0x3u << TC_CTRLBCLR_CMD_Pos) /**< \brief (TC_CTRLBCLR) Command */
#define TC_CTRLBCLR_CMD(value)      ((TC_CTRLBCLR_CMD_Msk & ((value) << TC_CTRLBCLR_CMD_Pos)))
#define   TC_CTRLBCLR_CMD_NONE      (0x0u <<  6) /**< \brief (TC_CTRLBCLR)  */
#define   TC_CTRLBCLR_CMD_RETRIGGER (0x1u <<  6) /**< \brief (TC_CTRLBCLR)  */
#define   TC_CTRLBCLR_CMD_STOP      (0x2u <<  6) /**< \brief (TC_CTRLBCLR)  */
#define TC_CTRLBCLR_MASK            0xC5u /**< \brief (TC_CTRLBCLR) MASK Register */

/* -------- TC_CTRLBSET : (TC Offset: 0x05) (R/W  8) Control Set Register B -------- */
typedef union {
  struct {
    uint8_t  DIR:1;            /*!< bit:      0  Counter Direction                  */
    uint8_t  _reserved0:1;     /*!< bit:      1  Reserved                           */
    uint8_t  ONESHOT:1;        /*!< bit:      2  Oneshot on counter                 */
    uint8_t  _reserved1:3;     /*!< bit:  3.. 5  Reserved                           */
    uint8_t  CMD:2;            /*!< bit:  6.. 7  Command                            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLBSET_Type;

#define TC_CTRLBSET_OFFSET          0x05         /**< \brief (TC offset) Control Set Register B */
#define TC_CTRLBSET_RESETVALUE      0x0          /**< \brief (TC reset_value) Control Set Register B */

#define TC_CTRLBSET_DIR             (0x1u <<  0) /**< \brief (TC_CTRLBSET) Counter Direction */
#define TC_CTRLBSET_ONESHOT         (0x1u <<  2) /**< \brief (TC_CTRLBSET) Oneshot on counter */
#define TC_CTRLBSET_CMD_Pos         6
#define TC_CTRLBSET_CMD_Msk         (0x3u << TC_CTRLBSET_CMD_Pos) /**< \brief (TC_CTRLBSET) Command */
#define TC_CTRLBSET_CMD(value)      ((TC_CTRLBSET_CMD_Msk & ((value) << TC_CTRLBSET_CMD_Pos)))
#define TC_CTRLBSET_MASK            0xC5u /**< \brief (TC_CTRLBSET) MASK Register */

/* -------- TC_CTRLC : (TC Offset: 0x06) (R/W  8) Control Register C -------- */
typedef union {
  struct {
    uint8_t  INVEN:2;          /*!< bit:  0.. 1  Output Waveform Invert Enable      */
    uint8_t  _reserved0:2;     /*!< bit:  2.. 3  Reserved                           */
    uint8_t  CPTEN:2;          /*!< bit:  4.. 5  Capture Channel  Enable            */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLC_Type;

#define TC_CTRLC_OFFSET             0x06         /**< \brief (TC offset) Control Register C */
#define TC_CTRLC_RESETVALUE         0x0          /**< \brief (TC reset_value) Control Register C */

#define TC_CTRLC_INVEN_Pos          0
#define TC_CTRLC_INVEN_Msk          (0x3u << TC_CTRLC_INVEN_Pos) /**< \brief (TC_CTRLC) Output Waveform Invert Enable */
#define TC_CTRLC_INVEN(value)       ((TC_CTRLC_INVEN_Msk & ((value) << TC_CTRLC_INVEN_Pos)))
#define TC_CTRLC_CPTEN_Pos          4
#define TC_CTRLC_CPTEN_Msk          (0x3u << TC_CTRLC_CPTEN_Pos) /**< \brief (TC_CTRLC) Capture Channel  Enable */
#define TC_CTRLC_CPTEN(value)       ((TC_CTRLC_CPTEN_Msk & ((value) << TC_CTRLC_CPTEN_Pos)))
#define TC_CTRLC_MASK               0x33u /**< \brief (TC_CTRLC) MASK Register */

/* -------- TC_DEBUG : (TC Offset: 0x08) (R/W  8) Control Register C -------- */
typedef union {
  struct {
    uint8_t  DBGRUN:1;         /*!< bit:      0  Run During Debug                   */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_DEBUG_Type;

#define TC_DEBUG_OFFSET             0x08         /**< \brief (TC offset) Control Register C */
#define TC_DEBUG_RESETVALUE         0x0          /**< \brief (TC reset_value) Control Register C */

#define TC_DEBUG_DBGRUN             (0x1u <<  0) /**< \brief (TC_DEBUG) Run During Debug */
#define TC_DEBUG_MASK               0x1u /**< \brief (TC_DEBUG) MASK Register */

/* -------- TC_EVCTRL : (TC Offset: 0x0A) (R/W 16) Event Control Register -------- */
typedef union {
  struct {
    uint16_t EVACT:3;          /*!< bit:  0.. 2  Event Action                       */
    uint16_t _reserved0:1;     /*!< bit:      3  Reserved                           */
    uint16_t TCINV:1;          /*!< bit:      4  TC event input polarity            */
    uint16_t TCEI:1;           /*!< bit:      5  TC event enable                    */
    uint16_t _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
    uint16_t OVFEO:1;          /*!< bit:      8  Event output enable                */
    uint16_t _reserved2:3;     /*!< bit:  9..11  Reserved                           */
    uint16_t MCEO:2;           /*!< bit: 12..13  MC Event output enable             */
    uint16_t _reserved3:2;     /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_EVCTRL_Type;

#define TC_EVCTRL_OFFSET            0x0A         /**< \brief (TC offset) Event Control Register */
#define TC_EVCTRL_RESETVALUE        0x0          /**< \brief (TC reset_value) Event Control Register */

#define TC_EVCTRL_EVACT_Pos         0
#define TC_EVCTRL_EVACT_Msk         (0x7u << TC_EVCTRL_EVACT_Pos) /**< \brief (TC_EVCTRL) Event Action */
#define TC_EVCTRL_EVACT(value)      ((TC_EVCTRL_EVACT_Msk & ((value) << TC_EVCTRL_EVACT_Pos)))
#define   TC_EVCTRL_EVACT_OFF       (0x0u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_RETRIGGER (0x1u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_COUNT     (0x2u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_START     (0x3u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_PPW       (0x5u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_PWP       (0x6u <<  0) /**< \brief (TC_EVCTRL)  */
#define TC_EVCTRL_TCINV             (0x1u <<  4) /**< \brief (TC_EVCTRL) TC event input polarity */
#define TC_EVCTRL_TCEI              (0x1u <<  5) /**< \brief (TC_EVCTRL) TC event enable */
#define TC_EVCTRL_OVFEO             (0x1u <<  8) /**< \brief (TC_EVCTRL) Event output enable */
#define TC_EVCTRL_MCEO_Pos          12
#define TC_EVCTRL_MCEO_Msk          (0x3u << TC_EVCTRL_MCEO_Pos) /**< \brief (TC_EVCTRL) MC Event output enable */
#define TC_EVCTRL_MCEO(value)       ((TC_EVCTRL_MCEO_Msk & ((value) << TC_EVCTRL_MCEO_Pos)))
#define TC_EVCTRL_MASK              0x3137u /**< \brief (TC_EVCTRL) MASK Register */

/* -------- TC_INTENCLR : (TC Offset: 0x0C) (R/W  8) Interrupt Enable Clear Register -------- */
typedef union {
  struct {
    uint8_t  OVF:1;            /*!< bit:      0  OVF interrupt Falg Enable          */
    uint8_t  ERR:1;            /*!< bit:      1  ERR interrupt Falg Enable          */
    uint8_t  _reserved0:1;     /*!< bit:      2  Reserved                           */
    uint8_t  READY:1;          /*!< bit:      3  READY interrupt Falg Enable        */
    uint8_t  MC:2;             /*!< bit:  4.. 5  MC interrupt Falg Enable           */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTENCLR_Type;

#define TC_INTENCLR_OFFSET          0x0C         /**< \brief (TC offset) Interrupt Enable Clear Register */
#define TC_INTENCLR_RESETVALUE      0x0          /**< \brief (TC reset_value) Interrupt Enable Clear Register */

#define TC_INTENCLR_OVF             (0x1u <<  0) /**< \brief (TC_INTENCLR) OVF interrupt Falg Enable */
#define TC_INTENCLR_ERR             (0x1u <<  1) /**< \brief (TC_INTENCLR) ERR interrupt Falg Enable */
#define TC_INTENCLR_READY           (0x1u <<  3) /**< \brief (TC_INTENCLR) READY interrupt Falg Enable */
#define TC_INTENCLR_MC_Pos          4
#define TC_INTENCLR_MC_Msk          (0x3u << TC_INTENCLR_MC_Pos) /**< \brief (TC_INTENCLR) MC interrupt Falg Enable */
#define TC_INTENCLR_MC(value)       ((TC_INTENCLR_MC_Msk & ((value) << TC_INTENCLR_MC_Pos)))
#define TC_INTENCLR_MASK            0x3Bu /**< \brief (TC_INTENCLR) MASK Register */

/* -------- TC_INTENSET : (TC Offset: 0x0D) (R/W  8) Interrupt Enable Set Register -------- */
typedef union {
  struct {
    uint8_t  OVF:1;            /*!< bit:      0  OVF interrupt Falg Enable          */
    uint8_t  ERR:1;            /*!< bit:      1  ERR interrupt Falg Enable          */
    uint8_t  _reserved0:1;     /*!< bit:      2  Reserved                           */
    uint8_t  READY:1;          /*!< bit:      3  READY interrupt Falg Enable        */
    uint8_t  MC:2;             /*!< bit:  4.. 5  MC interrupt Falg Enable           */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTENSET_Type;

#define TC_INTENSET_OFFSET          0x0D         /**< \brief (TC offset) Interrupt Enable Set Register */
#define TC_INTENSET_RESETVALUE      0x0          /**< \brief (TC reset_value) Interrupt Enable Set Register */

#define TC_INTENSET_OVF             (0x1u <<  0) /**< \brief (TC_INTENSET) OVF interrupt Falg Enable */
#define TC_INTENSET_ERR             (0x1u <<  1) /**< \brief (TC_INTENSET) ERR interrupt Falg Enable */
#define TC_INTENSET_READY           (0x1u <<  3) /**< \brief (TC_INTENSET) READY interrupt Falg Enable */
#define TC_INTENSET_MC_Pos          4
#define TC_INTENSET_MC_Msk          (0x3u << TC_INTENSET_MC_Pos) /**< \brief (TC_INTENSET) MC interrupt Falg Enable */
#define TC_INTENSET_MC(value)       ((TC_INTENSET_MC_Msk & ((value) << TC_INTENSET_MC_Pos)))
#define TC_INTENSET_MASK            0x3Bu /**< \brief (TC_INTENSET) MASK Register */

/* -------- TC_INTFLAG : (TC Offset: 0x0E) (R/W  8) Interrupt Flag Register -------- */
typedef union {
  struct {
    uint8_t  OVF:1;            /*!< bit:      0                                     */
    uint8_t  ERR:1;            /*!< bit:      1                                     */
    uint8_t  _reserved0:1;     /*!< bit:      2  Reserved                           */
    uint8_t  READY:1;          /*!< bit:      3                                     */
    uint8_t  MC:2;             /*!< bit:  4.. 5                                     */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTFLAG_Type;

#define TC_INTFLAG_OFFSET           0x0E         /**< \brief (TC offset) Interrupt Flag Register */
#define TC_INTFLAG_RESETVALUE       0x0          /**< \brief (TC reset_value) Interrupt Flag Register */

#define TC_INTFLAG_OVF              (0x1u <<  0) /**< \brief (TC_INTFLAG)  */
#define TC_INTFLAG_ERR              (0x1u <<  1) /**< \brief (TC_INTFLAG)  */
#define TC_INTFLAG_READY            (0x1u <<  3) /**< \brief (TC_INTFLAG)  */
#define TC_INTFLAG_MC_Pos           4
#define TC_INTFLAG_MC_Msk           (0x3u << TC_INTFLAG_MC_Pos) /**< \brief (TC_INTFLAG)  */
#define TC_INTFLAG_MC(value)        ((TC_INTFLAG_MC_Msk & ((value) << TC_INTFLAG_MC_Pos)))
#define TC_INTFLAG_MASK             0x3Bu /**< \brief (TC_INTFLAG) MASK Register */

/* -------- TC_STATUS : (TC Offset: 0x0F) (R/W  8) Status Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:3;     /*!< bit:  0.. 2  Reserved                           */
    uint8_t  STOP:1;           /*!< bit:      3  Stop Status flag                   */
    uint8_t  SLAVE:1;          /*!< bit:      4  Slave Status flag                  */
    uint8_t  _reserved1:2;     /*!< bit:  5.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Synchronization Busy Status        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_STATUS_Type;

#define TC_STATUS_OFFSET            0x0F         /**< \brief (TC offset) Status Register */
#define TC_STATUS_RESETVALUE        0x8          /**< \brief (TC reset_value) Status Register */

#define TC_STATUS_STOP              (0x1u <<  3) /**< \brief (TC_STATUS) Stop Status flag */
#define TC_STATUS_SLAVE             (0x1u <<  4) /**< \brief (TC_STATUS) Slave Status flag */
#define TC_STATUS_SYNCBUSY          (0x1u <<  7) /**< \brief (TC_STATUS) Synchronization Busy Status */
#define TC_STATUS_MASK              0x98u /**< \brief (TC_STATUS) MASK Register */

/* -------- TC_COUNT16_COUNT : (TC Offset: 0x10) (R/W 16) Count16 Count Register 0 -------- */
typedef union {
  uint16_t reg;                /*!< Type      used for register access              */
} TC_COUNT16_COUNT_Type;

#define TC_COUNT16_COUNT_OFFSET     0x10         /**< \brief (TC offset) Count Register 0 */
#define TC_COUNT16_COUNT_RESETVALUE 0x0          /**< \brief (TC reset_value) Count Register 0 */
#define TC_COUNT16_COUNT_MASK       0xFFFFu      /**< \brief (TC_COUNT16_COUNT) MASK Register */

/* -------- TC_COUNT32_COUNT : (TC Offset: 0x10) (R/W 32) Count32 Count Register 0 -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} TC_COUNT32_COUNT_Type;

#define TC_COUNT32_COUNT_OFFSET     0x10         /**< \brief (TC offset) Count Register 0 */
#define TC_COUNT32_COUNT_RESETVALUE 0x0          /**< \brief (TC reset_value) Count Register 0 */
#define TC_COUNT32_COUNT_MASK       0xFFFFFFFFu  /**< \brief (TC_COUNT32_COUNT) MASK Register */

/* -------- TC_COUNT8_COUNT : (TC Offset: 0x10) (R/W  8) Count8 Count Register 0 -------- */
typedef union {
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_COUNT_Type;

#define TC_COUNT8_COUNT_OFFSET      0x10         /**< \brief (TC offset) Count Register 0 */
#define TC_COUNT8_COUNT_RESETVALUE  0x0          /**< \brief (TC reset_value) Count Register 0 */
#define TC_COUNT8_COUNT_MASK        0xFFu        /**< \brief (TC_COUNT8_COUNT) MASK Register */

/* -------- TC_COUNT8_PER : (TC Offset: 0x14) (R/W  8) Count8 Period Register -------- */
typedef union {
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_PER_Type;

#define TC_COUNT8_PER_OFFSET        0x14         /**< \brief (TC offset) Period Register */
#define TC_COUNT8_PER_RESETVALUE    0x0          /**< \brief (TC reset_value) Period Register */
#define TC_COUNT8_PER_MASK          0xFFu        /**< \brief (TC_COUNT8_PER) MASK Register */

/* -------- TC_COUNT16_CC : (TC Offset: 0x18) (R/W 16) Count16_cc Compare and Capture Register -------- */
typedef union {
  uint16_t reg;                /*!< Type      used for register access              */
} TC_COUNT16_CC_Type;

#define TC_COUNT16_CC_OFFSET        0x18         /**< \brief (TC offset) Compare and Capture Register */
#define TC_COUNT16_CC_RESETVALUE    0x0          /**< \brief (TC reset_value) Compare and Capture Register */
#define TC_COUNT16_CC_MASK          0xFFFFu      /**< \brief (TC_COUNT16_CC) MASK Register */

/* -------- TC_COUNT32_CC : (TC Offset: 0x18) (R/W 32) Count32_cc Compare and Capture Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} TC_COUNT32_CC_Type;

#define TC_COUNT32_CC_OFFSET        0x18         /**< \brief (TC offset) Compare and Capture Register */
#define TC_COUNT32_CC_RESETVALUE    0x0          /**< \brief (TC reset_value) Compare and Capture Register */
#define TC_COUNT32_CC_MASK          0xFFFFFFFFu  /**< \brief (TC_COUNT32_CC) MASK Register */

/* -------- TC_COUNT8_CC : (TC Offset: 0x18) (R/W  8) Count8_cc Compare and Capture Register -------- */
typedef union {
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_CC_Type;

#define TC_COUNT8_CC_OFFSET         0x18         /**< \brief (TC offset) Compare and Capture Register */
#define TC_COUNT8_CC_RESETVALUE     0x0          /**< \brief (TC reset_value) Compare and Capture Register */
#define TC_COUNT8_CC_MASK           0xFFu        /**< \brief (TC_COUNT8_CC) MASK Register */

/** \brief TcCount16 hardware registers */
typedef struct {
  __IO TC_COUNT16_COUNT_Type     COUNT;       /**< \brief Offset: 0x00 (R/W 16) Count Register 0 */
       RoReg8                    Reserved1[0x6];
  __IO TC_COUNT16_CC_Type        CC[2];       /**< \brief Offset: 0x08 (R/W 16) Compare and Capture Register */
       RoReg8                    Reserved2[0x4];
} TcCount16;

/** \brief TcCount32 hardware registers */
typedef struct {
  __IO TC_COUNT32_COUNT_Type     COUNT;       /**< \brief Offset: 0x00 (R/W 32) Count Register 0 */
       RoReg8                    Reserved1[0x4];
  __IO TC_COUNT32_CC_Type        CC[2];       /**< \brief Offset: 0x08 (R/W 32) Compare and Capture Register */
} TcCount32;

/** \brief TcCount8 hardware registers */
typedef struct {
  __IO TC_COUNT8_COUNT_Type      COUNT;       /**< \brief Offset: 0x00 (R/W  8) Count Register 0 */
       RoReg8                    Reserved1[0x3];
  __IO TC_COUNT8_PER_Type        PER;         /**< \brief Offset: 0x04 (R/W  8) Period Register */
       RoReg8                    Reserved2[0x3];
  __IO TC_COUNT8_CC_Type         CC[2];       /**< \brief Offset: 0x08 (R/W  8) Compare and Capture Register */
       RoReg8                    Reserved3[0x6];
} TcCount8;

/** \brief TC hardware registers */
typedef struct {
  __IO TC_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W 16) Control Register A */
  __IO TC_READREQ_Type           READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request Register */
  __IO TC_CTRLBCLR_Type          CTRLBCLR;    /**< \brief Offset: 0x04 (R/W  8) Control Clear Register B */
  __IO TC_CTRLBSET_Type          CTRLBSET;    /**< \brief Offset: 0x05 (R/W  8) Control Set Register B */
  __IO TC_CTRLC_Type             CTRLC;       /**< \brief Offset: 0x06 (R/W  8) Control Register C */
       RoReg8                    Reserved1[0x1];
  __IO TC_DEBUG_Type             DEBUG;       /**< \brief Offset: 0x08 (R/W  8) Control Register C */
       RoReg8                    Reserved2[0x1];
  __IO TC_EVCTRL_Type            EVCTRL;      /**< \brief Offset: 0x0A (R/W 16) Event Control Register */
  __IO TC_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear Register */
  __IO TC_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x0D (R/W  8) Interrupt Enable Set Register */
  __IO TC_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x0E (R/W  8) Interrupt Flag Register */
  __IO TC_STATUS_Type            STATUS;      /**< \brief Offset: 0x0F (R/W  8) Status Register */
 union { // Overlapping sections
       TcCount16                 Count16;     /**< \brief Offset: 0x10 TcCount16 groups */
       TcCount32                 Count32;     /**< \brief Offset: 0x10 TcCount32 groups */
       TcCount8                  Count8;      /**< \brief Offset: 0x10 TcCount8 groups */
 };
} Tc;

#endif /*  __EXCLUDE_TC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR WDT */
/* ========================================================================== */
/** \addtogroup SAM0D4_REVA_WDT Watchdog Timer */
/*@{*/

#define REV_WDT        0x100

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_WDT_FROM_CMSIS__

/* -------- WDT_CTRL : (WDT Offset: 0x0) (R/W  8) Control Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable                             */
    uint8_t  PER:4;            /*!< bit:  2.. 5  Per[3:0]                           */
    uint8_t  _reserved1:1;     /*!< bit:      6  Reserved                           */
    uint8_t  ALWAYSON:1;       /*!< bit:      7  Always On                          */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} WDT_CTRL_Type;

#define WDT_CTRL_OFFSET             0x0          /**< \brief (WDT offset) Control Register */
#define WDT_CTRL_RESETVALUE         0x2C         /**< \brief (WDT reset_value) Control Register */

#define WDT_CTRL_ENABLE             (0x1u <<  1) /**< \brief (WDT_CTRL) Enable */
#define WDT_CTRL_PER_Pos            2
#define WDT_CTRL_PER_Msk            (0xFu << WDT_CTRL_PER_Pos) /**< \brief (WDT_CTRL) Per[3:0] */
#define WDT_CTRL_PER(value)         ((WDT_CTRL_PER_Msk & ((value) << WDT_CTRL_PER_Pos)))
#define WDT_CTRL_ALWAYSON           (0x1u <<  7) /**< \brief (WDT_CTRL) Always On */
#define WDT_CTRL_MASK               0xBEu /**< \brief (WDT_CTRL) MASK Register */

/* -------- WDT_WINCTRL : (WDT Offset: 0x1) (R/W  8) Window Control Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:1;     /*!< bit:      0  Reserved                           */
    uint8_t  WEN:1;            /*!< bit:      1  WEN                                */
    uint8_t  WINDOW:4;         /*!< bit:  2.. 5  WINDOW[3:0]                        */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} WDT_WINCTRL_Type;

#define WDT_WINCTRL_OFFSET          0x1          /**< \brief (WDT offset) Window Control Register */
#define WDT_WINCTRL_RESETVALUE      0x2C         /**< \brief (WDT reset_value) Window Control Register */

#define WDT_WINCTRL_WEN             (0x1u <<  1) /**< \brief (WDT_WINCTRL) WEN */
#define WDT_WINCTRL_WINDOW_Pos      2
#define WDT_WINCTRL_WINDOW_Msk      (0xFu << WDT_WINCTRL_WINDOW_Pos) /**< \brief (WDT_WINCTRL) WINDOW[3:0] */
#define WDT_WINCTRL_WINDOW(value)   ((WDT_WINCTRL_WINDOW_Msk & ((value) << WDT_WINCTRL_WINDOW_Pos)))
#define WDT_WINCTRL_MASK            0x3Eu /**< \brief (WDT_WINCTRL) MASK Register */

/* -------- WDT_EWCTRL : (WDT Offset: 0x2) (R/W  8) Early Warning Control Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:2;     /*!< bit:  0.. 1  Reserved                           */
    uint8_t  EWOFFSET:4;       /*!< bit:  2.. 5  EWOFFSET[3:0]                      */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} WDT_EWCTRL_Type;

#define WDT_EWCTRL_OFFSET           0x2          /**< \brief (WDT offset) Early Warning Control Register */
#define WDT_EWCTRL_RESETVALUE       0x0          /**< \brief (WDT reset_value) Early Warning Control Register */

#define WDT_EWCTRL_EWOFFSET_Pos     2
#define WDT_EWCTRL_EWOFFSET_Msk     (0xFu << WDT_EWCTRL_EWOFFSET_Pos) /**< \brief (WDT_EWCTRL) EWOFFSET[3:0] */
#define WDT_EWCTRL_EWOFFSET(value)  ((WDT_EWCTRL_EWOFFSET_Msk & ((value) << WDT_EWCTRL_EWOFFSET_Pos)))
#define WDT_EWCTRL_MASK             0x3Cu /**< \brief (WDT_EWCTRL) MASK Register */

/* -------- WDT_INTENCLR : (WDT Offset: 0x4) (R/W  8) Interrupt Enable Clear Register -------- */
typedef union {
  struct {
    uint8_t  EW:1;             /*!< bit:      0  EW                                 */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} WDT_INTENCLR_Type;

#define WDT_INTENCLR_OFFSET         0x4          /**< \brief (WDT offset) Interrupt Enable Clear Register */
#define WDT_INTENCLR_RESETVALUE     0x0          /**< \brief (WDT reset_value) Interrupt Enable Clear Register */

#define WDT_INTENCLR_EW             (0x1u <<  0) /**< \brief (WDT_INTENCLR) EW */
#define WDT_INTENCLR_MASK           0x1u /**< \brief (WDT_INTENCLR) MASK Register */

/* -------- WDT_INTENSET : (WDT Offset: 0x5) (R/W  8) Interrupt Enable Set Register -------- */
typedef union {
  struct {
    uint8_t  EW:1;             /*!< bit:      0  EW                                 */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} WDT_INTENSET_Type;

#define WDT_INTENSET_OFFSET         0x5          /**< \brief (WDT offset) Interrupt Enable Set Register */
#define WDT_INTENSET_RESETVALUE     0x0          /**< \brief (WDT reset_value) Interrupt Enable Set Register */

#define WDT_INTENSET_EW             (0x1u <<  0) /**< \brief (WDT_INTENSET) EW */
#define WDT_INTENSET_MASK           0x1u /**< \brief (WDT_INTENSET) MASK Register */

/* -------- WDT_INTFLAG : (WDT Offset: 0x6) (R/W  8) Interrupt Flag Register -------- */
typedef union {
  struct {
    uint8_t  EW:1;             /*!< bit:      0  EW                                 */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} WDT_INTFLAG_Type;

#define WDT_INTFLAG_OFFSET          0x6          /**< \brief (WDT offset) Interrupt Flag Register */
#define WDT_INTFLAG_RESETVALUE      0x0          /**< \brief (WDT reset_value) Interrupt Flag Register */

#define WDT_INTFLAG_EW              (0x1u <<  0) /**< \brief (WDT_INTFLAG) EW */
#define WDT_INTFLAG_MASK            0x1u /**< \brief (WDT_INTFLAG) MASK Register */

/* -------- WDT_STATUS : (WDT Offset: 0x7) (R/   8) Status Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:7;     /*!< bit:  0.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  SYNCBUSY                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} WDT_STATUS_Type;

#define WDT_STATUS_OFFSET           0x7          /**< \brief (WDT offset) Status Register */
#define WDT_STATUS_RESETVALUE       0x0          /**< \brief (WDT reset_value) Status Register */

#define WDT_STATUS_SYNCBUSY         (0x1u <<  7) /**< \brief (WDT_STATUS) SYNCBUSY */
#define WDT_STATUS_MASK             0x80u /**< \brief (WDT_STATUS) MASK Register */

/* -------- WDT_CLEAR : (WDT Offset: 0x8) ( /W  8) Clear Register -------- */
typedef union {
  struct {
    uint8_t  CLEAR:8;          /*!< bit:  0.. 7  Clear                              */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} WDT_CLEAR_Type;

#define WDT_CLEAR_OFFSET            0x8          /**< \brief (WDT offset) Clear Register */
#define WDT_CLEAR_RESETVALUE        0x0          /**< \brief (WDT reset_value) Clear Register */

#define WDT_CLEAR_CLEAR_Pos         0
#define WDT_CLEAR_CLEAR_Msk         (0xFFu << WDT_CLEAR_CLEAR_Pos) /**< \brief (WDT_CLEAR) Clear */
#define WDT_CLEAR_CLEAR(value)      ((WDT_CLEAR_CLEAR_Msk & ((value) << WDT_CLEAR_CLEAR_Pos)))
#define WDT_CLEAR_MASK              0xFFu /**< \brief (WDT_CLEAR) MASK Register */

/** \brief WDT hardware registers */
typedef struct {
  __IO WDT_CTRL_Type             CTRL;        /**< \brief Offset: 0x0 (R/W  8) Control Register */
  __IO WDT_WINCTRL_Type          WINCTRL;     /**< \brief Offset: 0x1 (R/W  8) Window Control Register */
  __IO WDT_EWCTRL_Type           EWCTRL;      /**< \brief Offset: 0x2 (R/W  8) Early Warning Control Register */
       RoReg8                    Reserved1[0x1];
  __IO WDT_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x4 (R/W  8) Interrupt Enable Clear Register */
  __IO WDT_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x5 (R/W  8) Interrupt Enable Set Register */
  __IO WDT_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x6 (R/W  8) Interrupt Flag Register */
  __I  WDT_STATUS_Type           STATUS;      /**< \brief Offset: 0x7 (R/   8) Status Register */
  __O  WDT_CLEAR_Type            CLEAR;       /**< \brief Offset: 0x8 ( /W  8) Clear Register */
} Wdt;

#endif /*  __EXCLUDE_WDT_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

/*@}*/

/*@}*/

/* ************************************************************************** */
/**  REGISTER ACCESS DEFINITIONS FOR SAM0D4_REVA */
/* ************************************************************************** */
/** \addtogroup SAM0D4_REVA_reg Registers Access Definitions */
/*@{*/

/* ========== Register definition for PAC0 peripheral ========== */
#define REG_PAC0_WPCLR            REG_ACCESS(RwReg  , 0x40000000U) /**< \brief (PAC0) Write Protection Clear */
#define REG_PAC0_WPSET            REG_ACCESS(RwReg  , 0x40000004U) /**< \brief (PAC0) Write Protection Set */
/* ========== Register definition for PM peripheral ========== */
#define REG_PM_CTRL               REG_ACCESS(RwReg8 , 0x40000400U) /**< \brief (PM) Control Register */
#define REG_PM_SLEEP              REG_ACCESS(RwReg8 , 0x40000401U) /**< \brief (PM) Sleep Register */
#define REG_PM_CPUSEL             REG_ACCESS(RwReg8 , 0x40000408U) /**< \brief (PM) CPU Clock Select */
#define REG_PM_APBASEL            REG_ACCESS(RwReg8 , 0x40000409U) /**< \brief (PM) APBA Clock Select */
#define REG_PM_APBBSEL            REG_ACCESS(RwReg8 , 0x4000040AU) /**< \brief (PM) APBB Clock Select */
#define REG_PM_APBCSEL            REG_ACCESS(RwReg8 , 0x4000040BU) /**< \brief (PM) APBC Clock Select */
#define REG_PM_AHBMASK            REG_ACCESS(RwReg  , 0x40000414U) /**< \brief (PM) AHB Mask */
#define REG_PM_APBAMASK           REG_ACCESS(RwReg  , 0x40000418U) /**< \brief (PM) APBA Mask */
#define REG_PM_APBBMASK           REG_ACCESS(RwReg  , 0x4000041CU) /**< \brief (PM) APBB Mask */
#define REG_PM_APBCMASK           REG_ACCESS(RwReg  , 0x40000420U) /**< \brief (PM) APBC Mask */
#define REG_PM_INTENCLR           REG_ACCESS(RwReg8 , 0x40000434U) /**< \brief (PM) Interrupt Enable Clear Register */
#define REG_PM_INTENSET           REG_ACCESS(RwReg8 , 0x40000435U) /**< \brief (PM) Interrupt Enable Set Register */
#define REG_PM_INTFLAG            REG_ACCESS(RwReg8 , 0x40000436U) /**< \brief (PM) Interrupt Flag Status and Clear Register */
#define REG_PM_RCAUSE             REG_ACCESS(RoReg8 , 0x40000438U) /**< \brief (PM) Reset Cause Register */
/* ========== Register definition for SYSCTRL peripheral ========== */
#define REG_SYSCTRL_INTENCLR      REG_ACCESS(RwReg  , 0x40000800U) /**< \brief (SYSCTRL) Interrupt Enable Register */
#define REG_SYSCTRL_INTENSET      REG_ACCESS(RwReg  , 0x40000804U) /**< \brief (SYSCTRL) Interrupt Clear Register */
#define REG_SYSCTRL_INTFLAG       REG_ACCESS(RwReg  , 0x40000808U) /**< \brief (SYSCTRL) Interrupt Flag Register */
#define REG_SYSCTRL_PCLKSR        REG_ACCESS(RoReg  , 0x4000080CU) /**< \brief (SYSCTRL) Power and Clocks Status Register */
#define REG_SYSCTRL_XOSC          REG_ACCESS(RwReg16, 0x40000810U) /**< \brief (SYSCTRL) XOSC Control Register */
#define REG_SYSCTRL_XOSC32K       REG_ACCESS(RwReg16, 0x40000814U) /**< \brief (SYSCTRL) XOSC32K Control Register */
#define REG_SYSCTRL_XOSC32KTEST   REG_ACCESS(RwReg8 , 0x40000816U) /**< \brief (SYSCTRL) XOSC32K Test Register */
#define REG_SYSCTRL_DFLL48CTRL    REG_ACCESS(RwReg  , 0x40000818U) /**< \brief (SYSCTRL) DFLL  Config Register */
#define REG_SYSCTRL_DFLL48VALDIFF REG_ACCESS(RwReg  , 0x4000081CU) /**< \brief (SYSCTRL) DFLL Calibration Value Register */
#define REG_SYSCTRL_DFLL48MULSTEP REG_ACCESS(RwReg  , 0x40000820U) /**< \brief (SYSCTRL) DFLL Multiplier Register */
#define REG_SYSCTRL_DFLL48SYNC    REG_ACCESS(RwReg8 , 0x40000824U) /**< \brief (SYSCTRL) DFLL Synchronization Register */
#define REG_SYSCTRL_BOD33CTRL     REG_ACCESS(RwReg16, 0x40000828U) /**< \brief (SYSCTRL) BOD33 Control Register */
#define REG_SYSCTRL_BOD33LEVEL    REG_ACCESS(RwReg16, 0x4000082AU) /**< \brief (SYSCTRL) BOD33 Level Register */
#define REG_SYSCTRL_BOD12CTRL     REG_ACCESS(RwReg16, 0x4000082CU) /**< \brief (SYSCTRL) BOD12 Control Register */
#define REG_SYSCTRL_BOD12LEVEL    REG_ACCESS(RwReg16, 0x4000082EU) /**< \brief (SYSCTRL) BOD12 Control Register */
#define REG_SYSCTRL_OSC32K        REG_ACCESS(RwReg  , 0x40000830U) /**< \brief (SYSCTRL) OSC32K Control Register */
#define REG_SYSCTRL_OSCULP32K     REG_ACCESS(RwReg  , 0x40000834U) /**< \brief (SYSCTRL) OSCULP32K Control Register */
#define REG_SYSCTRL_OSCULP32KTEST REG_ACCESS(RwReg8 , 0x40000838U) /**< \brief (SYSCTRL) OSCULP32K Test Register */
#define REG_SYSCTRL_OSC8M         REG_ACCESS(RwReg  , 0x4000083CU) /**< \brief (SYSCTRL) OSC8M Control Register A */
#define REG_SYSCTRL_VREGCTRL      REG_ACCESS(RwReg16, 0x40000840U) /**< \brief (SYSCTRL) VREG Control Register */
#define REG_SYSCTRL_VREGTEST      REG_ACCESS(RwReg  , 0x40000844U) /**< \brief (SYSCTRL) VREG Test Register */
#define REG_SYSCTRL_VREFCTRLA     REG_ACCESS(RwReg16, 0x40000848U) /**< \brief (SYSCTRL) VREF Control Register A */
#define REG_SYSCTRL_VREFCTRLB     REG_ACCESS(RwReg16, 0x4000084AU) /**< \brief (SYSCTRL) VREF Control Register B */
#define REG_SYSCTRL_VREFTESTA     REG_ACCESS(RwReg  , 0x4000084CU) /**< \brief (SYSCTRL) VREF Test Register */
#define REG_SYSCTRL_VREFTESTB     REG_ACCESS(RwReg16, 0x40000850U) /**< \brief (SYSCTRL)  */
#define REG_SYSCTRL_FORCECLKON    REG_ACCESS(RwReg  , 0x40000854U) /**< \brief (SYSCTRL)  */
#define REG_SYSCTRL_TESTMODE      REG_ACCESS(RwReg  , 0x40000858U) /**< \brief (SYSCTRL)  */
/* ========== Register definition for GCLK peripheral ========== */
#define REG_GCLK_CTRL             REG_ACCESS(RwReg8 , 0x40000C00U) /**< \brief (GCLK) Control Register */
#define REG_GCLK_STATUS           REG_ACCESS(RwReg8 , 0x40000C01U) /**< \brief (GCLK) Status Register */
#define REG_GCLK_CLKCTRL          REG_ACCESS(RwReg16, 0x40000C02U) /**< \brief (GCLK) Generic Clock Control Register */
#define REG_GCLK_GENCTRL          REG_ACCESS(RwReg  , 0x40000C04U) /**< \brief (GCLK) Generic Clock Generator Control Register */
#define REG_GCLK_GENDIV           REG_ACCESS(RwReg  , 0x40000C08U) /**< \brief (GCLK) Generic Clock Generator Division Register */
/* ========== Register definition for WDT peripheral ========== */
#define REG_WDT_CTRL              REG_ACCESS(RwReg8 , 0x40001000U) /**< \brief (WDT) Control Register */
#define REG_WDT_WINCTRL           REG_ACCESS(RwReg8 , 0x40001001U) /**< \brief (WDT) Window Control Register */
#define REG_WDT_EWCTRL            REG_ACCESS(RwReg8 , 0x40001002U) /**< \brief (WDT) Early Warning Control Register */
#define REG_WDT_INTENCLR          REG_ACCESS(RwReg8 , 0x40001004U) /**< \brief (WDT) Interrupt Enable Clear Register */
#define REG_WDT_INTENSET          REG_ACCESS(RwReg8 , 0x40001005U) /**< \brief (WDT) Interrupt Enable Set Register */
#define REG_WDT_INTFLAG           REG_ACCESS(RwReg8 , 0x40001006U) /**< \brief (WDT) Interrupt Flag Register */
#define REG_WDT_STATUS            REG_ACCESS(RoReg8 , 0x40001007U) /**< \brief (WDT) Status Register */
#define REG_WDT_CLEAR             REG_ACCESS(WoReg8 , 0x40001008U) /**< \brief (WDT) Clear Register */
/* ========== Register definition for RTC peripheral ========== */
#define REG_RTC_CTRL              REG_ACCESS(RwReg16, 0x40001400U) /**< \brief (RTC) Control Register */
#define REG_RTC_READREQ           REG_ACCESS(RwReg16, 0x40001402U) /**< \brief (RTC) Read Request Register */
#define REG_RTC_EVCTRL            REG_ACCESS(RwReg16, 0x40001404U) /**< \brief (RTC) Event Control Register */
#define REG_RTC_INTENCLR          REG_ACCESS(RwReg8 , 0x40001406U) /**< \brief (RTC) Interrupt Enable Clear Register */
#define REG_RTC_INTENSET          REG_ACCESS(RwReg8 , 0x40001407U) /**< \brief (RTC) Interrupt Enable Set Register */
#define REG_RTC_INTFLAG           REG_ACCESS(RwReg8 , 0x40001408U) /**< \brief (RTC) Interrupt Flag Register */
#define REG_RTC_STATUS            REG_ACCESS(RwReg8 , 0x4000140AU) /**< \brief (RTC) Status Register */
#define REG_RTC_DEBUG             REG_ACCESS(RwReg8 , 0x4000140BU) /**< \brief (RTC) Debug Register */
#define REG_RTC_FREQCORR          REG_ACCESS(RwReg8 , 0x4000140CU) /**< \brief (RTC) Frequency Correction Register */
#define REG_RTC_MODE0_COUNT       REG_ACCESS(RwReg  , 0x40001410U) /**< \brief (RTC) Mode0 Count Register */
#define REG_RTC_MODE1_COUNT       REG_ACCESS(RwReg16, 0x40001410U) /**< \brief (RTC) Mode1 Count Register */
#define REG_RTC_MODE2_CLOCK       REG_ACCESS(RwReg  , 0x40001410U) /**< \brief (RTC) Mode2 Clock Register */
#define REG_RTC_MODE1_PER         REG_ACCESS(RwReg16, 0x40001414U) /**< \brief (RTC) Mode1 Period Register */
#define REG_RTC_MODE0_COMP_COMP   REG_ACCESS(RwReg  , 0x40001418U) /**< \brief (RTC) Mode0Comp Compare Register */
#define REG_RTC_MODE1_COMP_COMP   REG_ACCESS(RwReg16, 0x40001418U) /**< \brief (RTC) Mode1Comp Compare Register */
#define REG_RTC_MODE2_ALARM_ALARM REG_ACCESS(RwReg  , 0x40001418U) /**< \brief (RTC) Mode2Alarm Alarm Register */
#define REG_RTC_MODE2_ALARM_MASK  REG_ACCESS(RwReg8 , 0x4000141CU) /**< \brief (RTC) Mode2Alarm Alarm Mask Register */
/* ========== Register definition for EIC peripheral ========== */
#define REG_EIC_CTRL              REG_ACCESS(RwReg8 , 0x40001800U) /**< \brief (EIC) Control Register */
#define REG_EIC_STATUS            REG_ACCESS(RoReg8 , 0x40001801U) /**< \brief (EIC) Status Register */
#define REG_EIC_NMICTRL           REG_ACCESS(RwReg8 , 0x40001802U) /**< \brief (EIC) NMI Control Register */
#define REG_EIC_NMIFLAG           REG_ACCESS(RwReg8 , 0x40001803U) /**< \brief (EIC) NMI Interrupt Flag Register */
#define REG_EIC_EVCTRL            REG_ACCESS(RwReg  , 0x40001804U) /**< \brief (EIC) Event Control Register */
#define REG_EIC_INTENCLR          REG_ACCESS(RwReg  , 0x40001808U) /**< \brief (EIC) Interrupt Enable Clear Register */
#define REG_EIC_INTENSET          REG_ACCESS(RwReg  , 0x4000180CU) /**< \brief (EIC) Interrupt Enable Set Register */
#define REG_EIC_INTFLAG           REG_ACCESS(RwReg  , 0x40001810U) /**< \brief (EIC) Interrupt Flag Register */
#define REG_EIC_WAKEUP            REG_ACCESS(RwReg  , 0x40001814U) /**< \brief (EIC) Wake-up Enable Register */
#define REG_EIC_CONFIG0           REG_ACCESS(RwReg  , 0x40001818U) /**< \brief (EIC) Config Register 0 */
#define REG_EIC_CONFIG1           REG_ACCESS(RwReg  , 0x4000181CU) /**< \brief (EIC) Config Register 1 */
/* ========== Register definition for PAC1 peripheral ========== */
#define REG_PAC1_WPCLR            REG_ACCESS(RwReg  , 0x41000000U) /**< \brief (PAC1) Write Protection Clear */
#define REG_PAC1_WPSET            REG_ACCESS(RwReg  , 0x41000004U) /**< \brief (PAC1) Write Protection Set */
/* ========== Register definition for DSU peripheral ========== */
#define REG_DSU_CTRL              REG_ACCESS(WoReg8 , 0x41002000U) /**< \brief (DSU) Control Register */
#define REG_DSU_STATUSA           REG_ACCESS(RwReg8 , 0x41002001U) /**< \brief (DSU) Status Register A */
#define REG_DSU_STATUSB           REG_ACCESS(RoReg8 , 0x41002002U) /**< \brief (DSU) Status Register B */
#define REG_DSU_STATUSC           REG_ACCESS(RoReg8 , 0x41002003U) /**< \brief (DSU) Status Register C */
#define REG_DSU_ADDR              REG_ACCESS(RwReg  , 0x41002004U) /**< \brief (DSU) Address Register */
#define REG_DSU_LENGTH            REG_ACCESS(RwReg  , 0x41002008U) /**< \brief (DSU) Length Register */
#define REG_DSU_DATA              REG_ACCESS(RwReg  , 0x4100200CU) /**< \brief (DSU) Data Register */
#define REG_DSU_DCC0              REG_ACCESS(RwReg  , 0x41002010U) /**< \brief (DSU) Debug Communication Channel 0 Register */
#define REG_DSU_DCC1              REG_ACCESS(RwReg  , 0x41002014U) /**< \brief (DSU) Debug Communication Channel 1 Register */
#define REG_DSU_DID               REG_ACCESS(RoReg  , 0x41002018U) /**< \brief (DSU) Device Identification Register */
#define REG_DSU_DCFG0             REG_ACCESS(RwReg  , 0x410020F0U) /**< \brief (DSU) Device Configuration Register 0 */
#define REG_DSU_DCFG1             REG_ACCESS(RoReg  , 0x410020F4U) /**< \brief (DSU) Device Configuration Register 1 */
#define REG_DSU_UPTM              REG_ACCESS(RwReg  , 0x410020F8U) /**< \brief (DSU) UnProtected Test Mode Register */
#define REG_DSU_TESTMODE          REG_ACCESS(RwReg  , 0x410020FCU) /**< \brief (DSU) Test Mode Register */
#define REG_DSU_ENTRY0            REG_ACCESS(RoReg  , 0x41003000U) /**< \brief (DSU) Coresight ROM Table Entry 0 Register */
#define REG_DSU_ENTRY1            REG_ACCESS(RoReg  , 0x41003004U) /**< \brief (DSU) Coresight ROM Table Entry 1 Register */
#define REG_DSU_END               REG_ACCESS(RoReg  , 0x41003008U) /**< \brief (DSU) Coresight ROM Table End Register */
#define REG_DSU_MEMTYPE           REG_ACCESS(RoReg  , 0x41003FCCU) /**< \brief (DSU) Coresight ROM Table Memory Type Register */
#define REG_DSU_PID4              REG_ACCESS(RoReg  , 0x41003FD0U) /**< \brief (DSU) Peripheral Identification Register 4 */
#define REG_DSU_PID5              REG_ACCESS(RoReg  , 0x41003FD4U) /**< \brief (DSU) Peripheral Identification Register 5 */
#define REG_DSU_PID6              REG_ACCESS(RoReg  , 0x41003FD8U) /**< \brief (DSU) Peripheral Identification Register 6 */
#define REG_DSU_PID7              REG_ACCESS(RoReg  , 0x41003FDCU) /**< \brief (DSU) Peripheral Identification Register 7 */
#define REG_DSU_PID0              REG_ACCESS(RoReg  , 0x41003FE0U) /**< \brief (DSU) Peripheral Identification Register 0 */
#define REG_DSU_PID1              REG_ACCESS(RoReg  , 0x41003FE4U) /**< \brief (DSU) Peripheral Identification Register 1 */
#define REG_DSU_PID2              REG_ACCESS(RoReg  , 0x41003FE8U) /**< \brief (DSU) Peripheral Identification Register 2 */
#define REG_DSU_PID3              REG_ACCESS(RoReg  , 0x41003FECU) /**< \brief (DSU) Peripheral Identification Register 3 */
#define REG_DSU_CID0              REG_ACCESS(RoReg  , 0x41003FF0U) /**< \brief (DSU) Component Identification Register 0 */
#define REG_DSU_CID1              REG_ACCESS(RoReg  , 0x41003FF4U) /**< \brief (DSU) Component Identification Register 1 */
#define REG_DSU_CID2              REG_ACCESS(RoReg  , 0x41003FF8U) /**< \brief (DSU) Component Identification Register 2 */
#define REG_DSU_CID3              REG_ACCESS(RoReg  , 0x41003FFCU) /**< \brief (DSU) Component Identification Register 3 */
/* ========== Register definition for NVMCTRL peripheral ========== */
#define REG_NVMCTRL_CTRLA         REG_ACCESS(RwReg  , 0x41004000U) /**< \brief (NVMCTRL) NVM Control A Register */
#define REG_NVMCTRL_CTRLB         REG_ACCESS(RwReg  , 0x41004004U) /**< \brief (NVMCTRL) NVM Control B Register */
#define REG_NVMCTRL_PARAM         REG_ACCESS(RwReg  , 0x41004008U) /**< \brief (NVMCTRL) Parameter Register */
#define REG_NVMCTRL_INTENCLR      REG_ACCESS(RwReg8 , 0x4100400CU) /**< \brief (NVMCTRL) Interrupt Enable Clear Register */
#define REG_NVMCTRL_INTENSET      REG_ACCESS(RwReg8 , 0x41004010U) /**< \brief (NVMCTRL) Interrupt Enable Set Register */
#define REG_NVMCTRL_INTFLAG       REG_ACCESS(RwReg8 , 0x41004014U) /**< \brief (NVMCTRL) Interrupt Register */
#define REG_NVMCTRL_STATUS        REG_ACCESS(RwReg  , 0x41004018U) /**< \brief (NVMCTRL) Status Register */
#define REG_NVMCTRL_ADDR          REG_ACCESS(RwReg  , 0x4100401CU) /**< \brief (NVMCTRL) Address Register */
#define REG_NVMCTRL_LOCK          REG_ACCESS(RwReg  , 0x41004020U) /**< \brief (NVMCTRL) Lock Register */
/* ========== Register definition for PORT peripheral ========== */
#define REG_PORT_GROUP_DIR        REG_ACCESS(RwReg  , 0x41004400U) /**< \brief (PORT) Group Data Direction Register */
#define REG_PORT_GROUP_DIRCLR     REG_ACCESS(RwReg  , 0x41004404U) /**< \brief (PORT) Group Data Direction Clear Register */
#define REG_PORT_GROUP_DIRSET     REG_ACCESS(RwReg  , 0x41004408U) /**< \brief (PORT) Group Data Direction Set Register */
#define REG_PORT_GROUP_DIRTGL     REG_ACCESS(RwReg  , 0x4100440CU) /**< \brief (PORT) Group Data Direction Toggle Register */
#define REG_PORT_GROUP_OUT        REG_ACCESS(RwReg  , 0x41004410U) /**< \brief (PORT) Group Data Output Value Register */
#define REG_PORT_GROUP_OUTCLR     REG_ACCESS(RwReg  , 0x41004414U) /**< \brief (PORT) Group Data Output Value Clear Register */
#define REG_PORT_GROUP_OUTSET     REG_ACCESS(RwReg  , 0x41004418U) /**< \brief (PORT) Group Data Output Value Set Register */
#define REG_PORT_GROUP_OUTTGL     REG_ACCESS(RwReg  , 0x4100441CU) /**< \brief (PORT) Group Data Output Value Toggle Register */
#define REG_PORT_GROUP_IN         REG_ACCESS(RoReg  , 0x41004420U) /**< \brief (PORT) Group Data Input Value Register */
#define REG_PORT_GROUP_CTRL       REG_ACCESS(RwReg  , 0x41004424U) /**< \brief (PORT) Group Control Register */
#define REG_PORT_GROUP_WRCONFIG   REG_ACCESS(WoReg  , 0x41004428U) /**< \brief (PORT) Group Write Configuration Register */
#define REG_PORT_GROUP_PMUX_PMUX  REG_ACCESS(RwReg8 , 0x41004430U) /**< \brief (PORT) GroupPmux Peripheral Multiplexing Register 0 */
#define REG_PORT_GROUP_PINCFG_PINCFG REG_ACCESS(RwReg8 , 0x41004440U) /**< \brief (PORT) GroupPincfg Pin Configuration Register 0 */
/* ========== Register definition for HMATRIX peripheral ========== */
#define REG_HMATRIX_MCFG0         REG_ACCESS(RwReg  , 0x41004800U) /**< \brief (HMATRIX) Master Configuration Register 0 0 */
#define REG_HMATRIX_MCFG1         REG_ACCESS(RwReg  , 0x41004804U) /**< \brief (HMATRIX) Master Configuration Register 0 1 */
#define REG_HMATRIX_MCFG2         REG_ACCESS(RwReg  , 0x41004808U) /**< \brief (HMATRIX) Master Configuration Register 0 2 */
#define REG_HMATRIX_MCFG3         REG_ACCESS(RwReg  , 0x4100480CU) /**< \brief (HMATRIX) Master Configuration Register 0 3 */
#define REG_HMATRIX_MCFG4         REG_ACCESS(RwReg  , 0x41004810U) /**< \brief (HMATRIX) Master Configuration Register 0 4 */
#define REG_HMATRIX_MCFG5         REG_ACCESS(RwReg  , 0x41004814U) /**< \brief (HMATRIX) Master Configuration Register 0 5 */
#define REG_HMATRIX_MCFG6         REG_ACCESS(RwReg  , 0x41004818U) /**< \brief (HMATRIX) Master Configuration Register 0 6 */
#define REG_HMATRIX_MCFG7         REG_ACCESS(RwReg  , 0x4100481CU) /**< \brief (HMATRIX) Master Configuration Register 0 7 */
#define REG_HMATRIX_MCFG8         REG_ACCESS(RwReg  , 0x41004820U) /**< \brief (HMATRIX) Master Configuration Register 0 8 */
#define REG_HMATRIX_MCFG9         REG_ACCESS(RwReg  , 0x41004824U) /**< \brief (HMATRIX) Master Configuration Register 0 9 */
#define REG_HMATRIX_MCFG10        REG_ACCESS(RwReg  , 0x41004828U) /**< \brief (HMATRIX) Master Configuration Register 0 10 */
#define REG_HMATRIX_MCFG11        REG_ACCESS(RwReg  , 0x4100482CU) /**< \brief (HMATRIX) Master Configuration Register 0 11 */
#define REG_HMATRIX_MCFG12        REG_ACCESS(RwReg  , 0x41004830U) /**< \brief (HMATRIX) Master Configuration Register 0 12 */
#define REG_HMATRIX_MCFG13        REG_ACCESS(RwReg  , 0x41004834U) /**< \brief (HMATRIX) Master Configuration Register 0 13 */
#define REG_HMATRIX_MCFG14        REG_ACCESS(RwReg  , 0x41004838U) /**< \brief (HMATRIX) Master Configuration Register 0 14 */
#define REG_HMATRIX_MCFG15        REG_ACCESS(RwReg  , 0x4100483CU) /**< \brief (HMATRIX) Master Configuration Register 0 15 */
#define REG_HMATRIX_SCFG0         REG_ACCESS(RwReg  , 0x41004840U) /**< \brief (HMATRIX) Slave Configuration Register 0 0 */
#define REG_HMATRIX_SCFG1         REG_ACCESS(RwReg  , 0x41004844U) /**< \brief (HMATRIX) Slave Configuration Register 0 1 */
#define REG_HMATRIX_SCFG2         REG_ACCESS(RwReg  , 0x41004848U) /**< \brief (HMATRIX) Slave Configuration Register 0 2 */
#define REG_HMATRIX_SCFG3         REG_ACCESS(RwReg  , 0x4100484CU) /**< \brief (HMATRIX) Slave Configuration Register 0 3 */
#define REG_HMATRIX_SCFG4         REG_ACCESS(RwReg  , 0x41004850U) /**< \brief (HMATRIX) Slave Configuration Register 0 4 */
#define REG_HMATRIX_SCFG5         REG_ACCESS(RwReg  , 0x41004854U) /**< \brief (HMATRIX) Slave Configuration Register 0 5 */
#define REG_HMATRIX_SCFG6         REG_ACCESS(RwReg  , 0x41004858U) /**< \brief (HMATRIX) Slave Configuration Register 0 6 */
#define REG_HMATRIX_SCFG7         REG_ACCESS(RwReg  , 0x4100485CU) /**< \brief (HMATRIX) Slave Configuration Register 0 7 */
#define REG_HMATRIX_SCFG8         REG_ACCESS(RwReg  , 0x41004860U) /**< \brief (HMATRIX) Slave Configuration Register 0 8 */
#define REG_HMATRIX_SCFG9         REG_ACCESS(RwReg  , 0x41004864U) /**< \brief (HMATRIX) Slave Configuration Register 0 9 */
#define REG_HMATRIX_SCFG10        REG_ACCESS(RwReg  , 0x41004868U) /**< \brief (HMATRIX) Slave Configuration Register 0 10 */
#define REG_HMATRIX_SCFG11        REG_ACCESS(RwReg  , 0x4100486CU) /**< \brief (HMATRIX) Slave Configuration Register 0 11 */
#define REG_HMATRIX_SCFG12        REG_ACCESS(RwReg  , 0x41004870U) /**< \brief (HMATRIX) Slave Configuration Register 0 12 */
#define REG_HMATRIX_SCFG13        REG_ACCESS(RwReg  , 0x41004874U) /**< \brief (HMATRIX) Slave Configuration Register 0 13 */
#define REG_HMATRIX_SCFG14        REG_ACCESS(RwReg  , 0x41004878U) /**< \brief (HMATRIX) Slave Configuration Register 0 14 */
#define REG_HMATRIX_SCFG15        REG_ACCESS(RwReg  , 0x4100487CU) /**< \brief (HMATRIX) Slave Configuration Register 0 15 */
#define REG_HMATRIX_PRAS0         REG_ACCESS(RwReg  , 0x41004880U) /**< \brief (HMATRIX) Priority Register A for Slave 0 0 */
#define REG_HMATRIX_PRBS0         REG_ACCESS(RwReg  , 0x41004884U) /**< \brief (HMATRIX) Priority Register B for Slave 0 0 */
#define REG_HMATRIX_PRAS1         REG_ACCESS(RwReg  , 0x41004888U) /**< \brief (HMATRIX) Priority Register A for Slave 0 1 */
#define REG_HMATRIX_PRBS1         REG_ACCESS(RwReg  , 0x4100488CU) /**< \brief (HMATRIX) Priority Register B for Slave 0 1 */
#define REG_HMATRIX_PRAS2         REG_ACCESS(RwReg  , 0x41004890U) /**< \brief (HMATRIX) Priority Register A for Slave 0 2 */
#define REG_HMATRIX_PRBS2         REG_ACCESS(RwReg  , 0x41004894U) /**< \brief (HMATRIX) Priority Register B for Slave 0 2 */
#define REG_HMATRIX_PRAS3         REG_ACCESS(RwReg  , 0x41004898U) /**< \brief (HMATRIX) Priority Register A for Slave 0 3 */
#define REG_HMATRIX_PRBS3         REG_ACCESS(RwReg  , 0x4100489CU) /**< \brief (HMATRIX) Priority Register B for Slave 0 3 */
#define REG_HMATRIX_PRAS4         REG_ACCESS(RwReg  , 0x410048A0U) /**< \brief (HMATRIX) Priority Register A for Slave 0 4 */
#define REG_HMATRIX_PRBS4         REG_ACCESS(RwReg  , 0x410048A4U) /**< \brief (HMATRIX) Priority Register B for Slave 0 4 */
#define REG_HMATRIX_PRAS5         REG_ACCESS(RwReg  , 0x410048A8U) /**< \brief (HMATRIX) Priority Register A for Slave 0 5 */
#define REG_HMATRIX_PRBS5         REG_ACCESS(RwReg  , 0x410048ACU) /**< \brief (HMATRIX) Priority Register B for Slave 0 5 */
#define REG_HMATRIX_PRAS6         REG_ACCESS(RwReg  , 0x410048B0U) /**< \brief (HMATRIX) Priority Register A for Slave 0 6 */
#define REG_HMATRIX_PRBS6         REG_ACCESS(RwReg  , 0x410048B4U) /**< \brief (HMATRIX) Priority Register B for Slave 0 6 */
#define REG_HMATRIX_PRAS7         REG_ACCESS(RwReg  , 0x410048B8U) /**< \brief (HMATRIX) Priority Register A for Slave 0 7 */
#define REG_HMATRIX_PRBS7         REG_ACCESS(RwReg  , 0x410048BCU) /**< \brief (HMATRIX) Priority Register B for Slave 0 7 */
#define REG_HMATRIX_PRAS8         REG_ACCESS(RwReg  , 0x410048C0U) /**< \brief (HMATRIX) Priority Register A for Slave 0 8 */
#define REG_HMATRIX_PRBS8         REG_ACCESS(RwReg  , 0x410048C4U) /**< \brief (HMATRIX) Priority Register B for Slave 0 8 */
#define REG_HMATRIX_PRAS9         REG_ACCESS(RwReg  , 0x410048C8U) /**< \brief (HMATRIX) Priority Register A for Slave 0 9 */
#define REG_HMATRIX_PRBS9         REG_ACCESS(RwReg  , 0x410048CCU) /**< \brief (HMATRIX) Priority Register B for Slave 0 9 */
#define REG_HMATRIX_PRAS10        REG_ACCESS(RwReg  , 0x410048D0U) /**< \brief (HMATRIX) Priority Register A for Slave 0 10 */
#define REG_HMATRIX_PRBS10        REG_ACCESS(RwReg  , 0x410048D4U) /**< \brief (HMATRIX) Priority Register B for Slave 0 10 */
#define REG_HMATRIX_PRAS11        REG_ACCESS(RwReg  , 0x410048D8U) /**< \brief (HMATRIX) Priority Register A for Slave 0 11 */
#define REG_HMATRIX_PRBS11        REG_ACCESS(RwReg  , 0x410048DCU) /**< \brief (HMATRIX) Priority Register B for Slave 0 11 */
#define REG_HMATRIX_PRAS12        REG_ACCESS(RwReg  , 0x410048E0U) /**< \brief (HMATRIX) Priority Register A for Slave 0 12 */
#define REG_HMATRIX_PRBS12        REG_ACCESS(RwReg  , 0x410048E4U) /**< \brief (HMATRIX) Priority Register B for Slave 0 12 */
#define REG_HMATRIX_PRAS13        REG_ACCESS(RwReg  , 0x410048E8U) /**< \brief (HMATRIX) Priority Register A for Slave 0 13 */
#define REG_HMATRIX_PRBS13        REG_ACCESS(RwReg  , 0x410048ECU) /**< \brief (HMATRIX) Priority Register B for Slave 0 13 */
#define REG_HMATRIX_PRAS14        REG_ACCESS(RwReg  , 0x410048F0U) /**< \brief (HMATRIX) Priority Register A for Slave 0 14 */
#define REG_HMATRIX_PRBS14        REG_ACCESS(RwReg  , 0x410048F4U) /**< \brief (HMATRIX) Priority Register B for Slave 0 14 */
#define REG_HMATRIX_PRAS15        REG_ACCESS(RwReg  , 0x410048F8U) /**< \brief (HMATRIX) Priority Register A for Slave 0 15 */
#define REG_HMATRIX_PRBS15        REG_ACCESS(RwReg  , 0x410048FCU) /**< \brief (HMATRIX) Priority Register B for Slave 0 15 */
#define REG_HMATRIX_MRCR          REG_ACCESS(RwReg  , 0x41004900U) /**< \brief (HMATRIX) Master Remap Control Register */
#define REG_HMATRIX_SFR0          REG_ACCESS(RwReg  , 0x41004910U) /**< \brief (HMATRIX) Special Function Register 0 0 */
#define REG_HMATRIX_SFR1          REG_ACCESS(RwReg  , 0x41004914U) /**< \brief (HMATRIX) Special Function Register 0 1 */
#define REG_HMATRIX_SFR2          REG_ACCESS(RwReg  , 0x41004918U) /**< \brief (HMATRIX) Special Function Register 0 2 */
#define REG_HMATRIX_SFR3          REG_ACCESS(RwReg  , 0x4100491CU) /**< \brief (HMATRIX) Special Function Register 0 3 */
#define REG_HMATRIX_SFR4          REG_ACCESS(RwReg  , 0x41004920U) /**< \brief (HMATRIX) Special Function Register 0 4 */
#define REG_HMATRIX_SFR5          REG_ACCESS(RwReg  , 0x41004924U) /**< \brief (HMATRIX) Special Function Register 0 5 */
#define REG_HMATRIX_SFR6          REG_ACCESS(RwReg  , 0x41004928U) /**< \brief (HMATRIX) Special Function Register 0 6 */
#define REG_HMATRIX_SFR7          REG_ACCESS(RwReg  , 0x4100492CU) /**< \brief (HMATRIX) Special Function Register 0 7 */
#define REG_HMATRIX_SFR8          REG_ACCESS(RwReg  , 0x41004930U) /**< \brief (HMATRIX) Special Function Register 0 8 */
#define REG_HMATRIX_SFR9          REG_ACCESS(RwReg  , 0x41004934U) /**< \brief (HMATRIX) Special Function Register 0 9 */
#define REG_HMATRIX_SFR10         REG_ACCESS(RwReg  , 0x41004938U) /**< \brief (HMATRIX) Special Function Register 0 10 */
#define REG_HMATRIX_SFR11         REG_ACCESS(RwReg  , 0x4100493CU) /**< \brief (HMATRIX) Special Function Register 0 11 */
#define REG_HMATRIX_SFR12         REG_ACCESS(RwReg  , 0x41004940U) /**< \brief (HMATRIX) Special Function Register 0 12 */
#define REG_HMATRIX_SFR13         REG_ACCESS(RwReg  , 0x41004944U) /**< \brief (HMATRIX) Special Function Register 0 13 */
#define REG_HMATRIX_SFR14         REG_ACCESS(RwReg  , 0x41004948U) /**< \brief (HMATRIX) Special Function Register 0 14 */
#define REG_HMATRIX_SFR15         REG_ACCESS(RwReg  , 0x4100494CU) /**< \brief (HMATRIX) Special Function Register 0 15 */
/* ========== Register definition for PAC2 peripheral ========== */
#define REG_PAC2_WPCLR            REG_ACCESS(RwReg  , 0x42000000U) /**< \brief (PAC2) Write Protection Clear */
#define REG_PAC2_WPSET            REG_ACCESS(RwReg  , 0x42000004U) /**< \brief (PAC2) Write Protection Set */
/* ========== Register definition for EVSYS peripheral ========== */
#define REG_EVSYS_CTRL            REG_ACCESS(WoReg8 , 0x42000400U) /**< \brief (EVSYS) Control Register */
#define REG_EVSYS_CHANNEL         REG_ACCESS(RwReg  , 0x42000404U) /**< \brief (EVSYS) Channel Register */
#define REG_EVSYS_USERMUX         REG_ACCESS(RwReg16, 0x42000408U) /**< \brief (EVSYS) User Mux Register */
#define REG_EVSYS_CHSTATUS        REG_ACCESS(RoReg  , 0x4200040CU) /**< \brief (EVSYS) Channel Status Register */
#define REG_EVSYS_IENCLR          REG_ACCESS(RwReg  , 0x42000410U) /**< \brief (EVSYS) Interrupt Enable Clear register */
#define REG_EVSYS_IENSET          REG_ACCESS(RwReg  , 0x42000414U) /**< \brief (EVSYS) Interrupt Enable Set Register */
#define REG_EVSYS_INTFLAG         REG_ACCESS(RwReg  , 0x42000418U) /**< \brief (EVSYS) Interrupt Flag Register */
/* ========== Register definition for SERCOM0 peripheral ========== */
#define REG_SERCOM0_CTRLA         REG_ACCESS(RwReg  , 0x42000800U) /**< \brief (SERCOM0) Control Register A */
#define REG_SERCOM0_CTRLB         REG_ACCESS(RwReg  , 0x42000804U) /**< \brief (SERCOM0) Control Register B */
#define REG_SERCOM0_DEBUG         REG_ACCESS(RwReg8 , 0x42000808U) /**< \brief (SERCOM0) Debug Register */
#define REG_SERCOM0_BAUD          REG_ACCESS(RwReg16, 0x4200080AU) /**< \brief (SERCOM0) Baud Rate Register */
#define REG_SERCOM0_INTENCLR      REG_ACCESS(RwReg8 , 0x4200080CU) /**< \brief (SERCOM0) Interrupt Enable Clear Register */
#define REG_SERCOM0_INTENSET      REG_ACCESS(RwReg8 , 0x4200080DU) /**< \brief (SERCOM0) Interrupt Enable Set Register */
#define REG_SERCOM0_INTFLAG       REG_ACCESS(RwReg8 , 0x4200080EU) /**< \brief (SERCOM0) Interrupt Flag Register */
#define REG_SERCOM0_STATUS        REG_ACCESS(RwReg  , 0x42000810U) /**< \brief (SERCOM0) Status Register */
#define REG_SERCOM0_ADDR          REG_ACCESS(RwReg  , 0x42000814U) /**< \brief (SERCOM0) Address Register */
#define REG_SERCOM0_DATA          REG_ACCESS(RwReg  , 0x42000818U) /**< \brief (SERCOM0) Data Register */
/* ========== Register definition for SERCOM1 peripheral ========== */
#define REG_SERCOM1_CTRLA         REG_ACCESS(RwReg  , 0x42000C00U) /**< \brief (SERCOM1) Control Register A */
#define REG_SERCOM1_CTRLB         REG_ACCESS(RwReg  , 0x42000C04U) /**< \brief (SERCOM1) Control Register B */
#define REG_SERCOM1_DEBUG         REG_ACCESS(RwReg8 , 0x42000C08U) /**< \brief (SERCOM1) Debug Register */
#define REG_SERCOM1_BAUD          REG_ACCESS(RwReg16, 0x42000C0AU) /**< \brief (SERCOM1) Baud Rate Register */
#define REG_SERCOM1_INTENCLR      REG_ACCESS(RwReg8 , 0x42000C0CU) /**< \brief (SERCOM1) Interrupt Enable Clear Register */
#define REG_SERCOM1_INTENSET      REG_ACCESS(RwReg8 , 0x42000C0DU) /**< \brief (SERCOM1) Interrupt Enable Set Register */
#define REG_SERCOM1_INTFLAG       REG_ACCESS(RwReg8 , 0x42000C0EU) /**< \brief (SERCOM1) Interrupt Flag Register */
#define REG_SERCOM1_STATUS        REG_ACCESS(RwReg  , 0x42000C10U) /**< \brief (SERCOM1) Status Register */
#define REG_SERCOM1_ADDR          REG_ACCESS(RwReg  , 0x42000C14U) /**< \brief (SERCOM1) Address Register */
#define REG_SERCOM1_DATA          REG_ACCESS(RwReg  , 0x42000C18U) /**< \brief (SERCOM1) Data Register */
/* ========== Register definition for SERCOM2 peripheral ========== */
#define REG_SERCOM2_CTRLA         REG_ACCESS(RwReg  , 0x42001000U) /**< \brief (SERCOM2) Control Register A */
#define REG_SERCOM2_CTRLB         REG_ACCESS(RwReg  , 0x42001004U) /**< \brief (SERCOM2) Control Register B */
#define REG_SERCOM2_DEBUG         REG_ACCESS(RwReg8 , 0x42001008U) /**< \brief (SERCOM2) Debug Register */
#define REG_SERCOM2_BAUD          REG_ACCESS(RwReg16, 0x4200100AU) /**< \brief (SERCOM2) Baud Rate Register */
#define REG_SERCOM2_INTENCLR      REG_ACCESS(RwReg8 , 0x4200100CU) /**< \brief (SERCOM2) Interrupt Enable Clear Register */
#define REG_SERCOM2_INTENSET      REG_ACCESS(RwReg8 , 0x4200100DU) /**< \brief (SERCOM2) Interrupt Enable Set Register */
#define REG_SERCOM2_INTFLAG       REG_ACCESS(RwReg8 , 0x4200100EU) /**< \brief (SERCOM2) Interrupt Flag Register */
#define REG_SERCOM2_STATUS        REG_ACCESS(RwReg  , 0x42001010U) /**< \brief (SERCOM2) Status Register */
#define REG_SERCOM2_ADDR          REG_ACCESS(RwReg  , 0x42001014U) /**< \brief (SERCOM2) Address Register */
#define REG_SERCOM2_DATA          REG_ACCESS(RwReg  , 0x42001018U) /**< \brief (SERCOM2) Data Register */
/* ========== Register definition for SERCOM3 peripheral ========== */
#define REG_SERCOM3_CTRLA         REG_ACCESS(RwReg  , 0x42001400U) /**< \brief (SERCOM3) Control Register A */
#define REG_SERCOM3_CTRLB         REG_ACCESS(RwReg  , 0x42001404U) /**< \brief (SERCOM3) Control Register B */
#define REG_SERCOM3_DEBUG         REG_ACCESS(RwReg8 , 0x42001408U) /**< \brief (SERCOM3) Debug Register */
#define REG_SERCOM3_BAUD          REG_ACCESS(RwReg16, 0x4200140AU) /**< \brief (SERCOM3) Baud Rate Register */
#define REG_SERCOM3_INTENCLR      REG_ACCESS(RwReg8 , 0x4200140CU) /**< \brief (SERCOM3) Interrupt Enable Clear Register */
#define REG_SERCOM3_INTENSET      REG_ACCESS(RwReg8 , 0x4200140DU) /**< \brief (SERCOM3) Interrupt Enable Set Register */
#define REG_SERCOM3_INTFLAG       REG_ACCESS(RwReg8 , 0x4200140EU) /**< \brief (SERCOM3) Interrupt Flag Register */
#define REG_SERCOM3_STATUS        REG_ACCESS(RwReg  , 0x42001410U) /**< \brief (SERCOM3) Status Register */
#define REG_SERCOM3_ADDR          REG_ACCESS(RwReg  , 0x42001414U) /**< \brief (SERCOM3) Address Register */
#define REG_SERCOM3_DATA          REG_ACCESS(RwReg  , 0x42001418U) /**< \brief (SERCOM3) Data Register */
/* ========== Register definition for SERCOM4 peripheral ========== */
#define REG_SERCOM4_CTRLA         REG_ACCESS(RwReg  , 0x42001800U) /**< \brief (SERCOM4) Control Register A */
#define REG_SERCOM4_CTRLB         REG_ACCESS(RwReg  , 0x42001804U) /**< \brief (SERCOM4) Control Register B */
#define REG_SERCOM4_DEBUG         REG_ACCESS(RwReg8 , 0x42001808U) /**< \brief (SERCOM4) Debug Register */
#define REG_SERCOM4_BAUD          REG_ACCESS(RwReg16, 0x4200180AU) /**< \brief (SERCOM4) Baud Rate Register */
#define REG_SERCOM4_INTENCLR      REG_ACCESS(RwReg8 , 0x4200180CU) /**< \brief (SERCOM4) Interrupt Enable Clear Register */
#define REG_SERCOM4_INTENSET      REG_ACCESS(RwReg8 , 0x4200180DU) /**< \brief (SERCOM4) Interrupt Enable Set Register */
#define REG_SERCOM4_INTFLAG       REG_ACCESS(RwReg8 , 0x4200180EU) /**< \brief (SERCOM4) Interrupt Flag Register */
#define REG_SERCOM4_STATUS        REG_ACCESS(RwReg  , 0x42001810U) /**< \brief (SERCOM4) Status Register */
#define REG_SERCOM4_ADDR          REG_ACCESS(RwReg  , 0x42001814U) /**< \brief (SERCOM4) Address Register */
#define REG_SERCOM4_DATA          REG_ACCESS(RwReg  , 0x42001818U) /**< \brief (SERCOM4) Data Register */
/* ========== Register definition for SERCOM5 peripheral ========== */
#define REG_SERCOM5_CTRLA         REG_ACCESS(RwReg  , 0x42001C00U) /**< \brief (SERCOM5) Control Register A */
#define REG_SERCOM5_CTRLB         REG_ACCESS(RwReg  , 0x42001C04U) /**< \brief (SERCOM5) Control Register B */
#define REG_SERCOM5_DEBUG         REG_ACCESS(RwReg8 , 0x42001C08U) /**< \brief (SERCOM5) Debug Register */
#define REG_SERCOM5_BAUD          REG_ACCESS(RwReg16, 0x42001C0AU) /**< \brief (SERCOM5) Baud Rate Register */
#define REG_SERCOM5_INTENCLR      REG_ACCESS(RwReg8 , 0x42001C0CU) /**< \brief (SERCOM5) Interrupt Enable Clear Register */
#define REG_SERCOM5_INTENSET      REG_ACCESS(RwReg8 , 0x42001C0DU) /**< \brief (SERCOM5) Interrupt Enable Set Register */
#define REG_SERCOM5_INTFLAG       REG_ACCESS(RwReg8 , 0x42001C0EU) /**< \brief (SERCOM5) Interrupt Flag Register */
#define REG_SERCOM5_STATUS        REG_ACCESS(RwReg  , 0x42001C10U) /**< \brief (SERCOM5) Status Register */
#define REG_SERCOM5_ADDR          REG_ACCESS(RwReg  , 0x42001C14U) /**< \brief (SERCOM5) Address Register */
#define REG_SERCOM5_DATA          REG_ACCESS(RwReg  , 0x42001C18U) /**< \brief (SERCOM5) Data Register */
/* ========== Register definition for TC0 peripheral ========== */
#define REG_TC0_CTRLA             REG_ACCESS(RwReg16, 0x42002000U) /**< \brief (TC0) Control Register A */
#define REG_TC0_READREQ           REG_ACCESS(RwReg16, 0x42002002U) /**< \brief (TC0) Read Request Register */
#define REG_TC0_CTRLBCLR          REG_ACCESS(RwReg8 , 0x42002004U) /**< \brief (TC0) Control Clear Register B */
#define REG_TC0_CTRLBSET          REG_ACCESS(RwReg8 , 0x42002005U) /**< \brief (TC0) Control Set Register B */
#define REG_TC0_CTRLC             REG_ACCESS(RwReg8 , 0x42002006U) /**< \brief (TC0) Control Register C */
#define REG_TC0_DEBUG             REG_ACCESS(RwReg8 , 0x42002008U) /**< \brief (TC0) Control Register C */
#define REG_TC0_EVCTRL            REG_ACCESS(RwReg16, 0x4200200AU) /**< \brief (TC0) Event Control Register */
#define REG_TC0_INTENCLR          REG_ACCESS(RwReg8 , 0x4200200CU) /**< \brief (TC0) Interrupt Enable Clear Register */
#define REG_TC0_INTENSET          REG_ACCESS(RwReg8 , 0x4200200DU) /**< \brief (TC0) Interrupt Enable Set Register */
#define REG_TC0_INTFLAG           REG_ACCESS(RwReg8 , 0x4200200EU) /**< \brief (TC0) Interrupt Flag Register */
#define REG_TC0_STATUS            REG_ACCESS(RwReg8 , 0x4200200FU) /**< \brief (TC0) Status Register */
#define REG_TC0_COUNT16_COUNT     REG_ACCESS(RwReg16, 0x42002010U) /**< \brief (TC0) Count16 Count Register 0 */
#define REG_TC0_COUNT32_COUNT     REG_ACCESS(RwReg  , 0x42002010U) /**< \brief (TC0) Count32 Count Register 0 */
#define REG_TC0_COUNT8_COUNT      REG_ACCESS(RwReg8 , 0x42002010U) /**< \brief (TC0) Count8 Count Register 0 */
#define REG_TC0_COUNT8_PER        REG_ACCESS(RwReg8 , 0x42002014U) /**< \brief (TC0) Count8 Period Register */
#define REG_TC0_COUNT16_CC_CC     REG_ACCESS(RwReg16, 0x42002018U) /**< \brief (TC0) Count16Cc Compare and Capture Register */
#define REG_TC0_COUNT32_CC_CC     REG_ACCESS(RwReg  , 0x42002018U) /**< \brief (TC0) Count32Cc Compare and Capture Register */
#define REG_TC0_COUNT8_CC_CC      REG_ACCESS(RwReg8 , 0x42002018U) /**< \brief (TC0) Count8Cc Compare and Capture Register */
/* ========== Register definition for TC1 peripheral ========== */
#define REG_TC1_CTRLA             REG_ACCESS(RwReg16, 0x42002400U) /**< \brief (TC1) Control Register A */
#define REG_TC1_READREQ           REG_ACCESS(RwReg16, 0x42002402U) /**< \brief (TC1) Read Request Register */
#define REG_TC1_CTRLBCLR          REG_ACCESS(RwReg8 , 0x42002404U) /**< \brief (TC1) Control Clear Register B */
#define REG_TC1_CTRLBSET          REG_ACCESS(RwReg8 , 0x42002405U) /**< \brief (TC1) Control Set Register B */
#define REG_TC1_CTRLC             REG_ACCESS(RwReg8 , 0x42002406U) /**< \brief (TC1) Control Register C */
#define REG_TC1_DEBUG             REG_ACCESS(RwReg8 , 0x42002408U) /**< \brief (TC1) Control Register C */
#define REG_TC1_EVCTRL            REG_ACCESS(RwReg16, 0x4200240AU) /**< \brief (TC1) Event Control Register */
#define REG_TC1_INTENCLR          REG_ACCESS(RwReg8 , 0x4200240CU) /**< \brief (TC1) Interrupt Enable Clear Register */
#define REG_TC1_INTENSET          REG_ACCESS(RwReg8 , 0x4200240DU) /**< \brief (TC1) Interrupt Enable Set Register */
#define REG_TC1_INTFLAG           REG_ACCESS(RwReg8 , 0x4200240EU) /**< \brief (TC1) Interrupt Flag Register */
#define REG_TC1_STATUS            REG_ACCESS(RwReg8 , 0x4200240FU) /**< \brief (TC1) Status Register */
#define REG_TC1_COUNT16_COUNT     REG_ACCESS(RwReg16, 0x42002410U) /**< \brief (TC1) Count16 Count Register 0 */
#define REG_TC1_COUNT32_COUNT     REG_ACCESS(RwReg  , 0x42002410U) /**< \brief (TC1) Count32 Count Register 0 */
#define REG_TC1_COUNT8_COUNT      REG_ACCESS(RwReg8 , 0x42002410U) /**< \brief (TC1) Count8 Count Register 0 */
#define REG_TC1_COUNT8_PER        REG_ACCESS(RwReg8 , 0x42002414U) /**< \brief (TC1) Count8 Period Register */
#define REG_TC1_COUNT16_CC_CC     REG_ACCESS(RwReg16, 0x42002418U) /**< \brief (TC1) Count16Cc Compare and Capture Register */
#define REG_TC1_COUNT32_CC_CC     REG_ACCESS(RwReg  , 0x42002418U) /**< \brief (TC1) Count32Cc Compare and Capture Register */
#define REG_TC1_COUNT8_CC_CC      REG_ACCESS(RwReg8 , 0x42002418U) /**< \brief (TC1) Count8Cc Compare and Capture Register */
/* ========== Register definition for TC2 peripheral ========== */
#define REG_TC2_CTRLA             REG_ACCESS(RwReg16, 0x42002800U) /**< \brief (TC2) Control Register A */
#define REG_TC2_READREQ           REG_ACCESS(RwReg16, 0x42002802U) /**< \brief (TC2) Read Request Register */
#define REG_TC2_CTRLBCLR          REG_ACCESS(RwReg8 , 0x42002804U) /**< \brief (TC2) Control Clear Register B */
#define REG_TC2_CTRLBSET          REG_ACCESS(RwReg8 , 0x42002805U) /**< \brief (TC2) Control Set Register B */
#define REG_TC2_CTRLC             REG_ACCESS(RwReg8 , 0x42002806U) /**< \brief (TC2) Control Register C */
#define REG_TC2_DEBUG             REG_ACCESS(RwReg8 , 0x42002808U) /**< \brief (TC2) Control Register C */
#define REG_TC2_EVCTRL            REG_ACCESS(RwReg16, 0x4200280AU) /**< \brief (TC2) Event Control Register */
#define REG_TC2_INTENCLR          REG_ACCESS(RwReg8 , 0x4200280CU) /**< \brief (TC2) Interrupt Enable Clear Register */
#define REG_TC2_INTENSET          REG_ACCESS(RwReg8 , 0x4200280DU) /**< \brief (TC2) Interrupt Enable Set Register */
#define REG_TC2_INTFLAG           REG_ACCESS(RwReg8 , 0x4200280EU) /**< \brief (TC2) Interrupt Flag Register */
#define REG_TC2_STATUS            REG_ACCESS(RwReg8 , 0x4200280FU) /**< \brief (TC2) Status Register */
#define REG_TC2_COUNT16_COUNT     REG_ACCESS(RwReg16, 0x42002810U) /**< \brief (TC2) Count16 Count Register 0 */
#define REG_TC2_COUNT32_COUNT     REG_ACCESS(RwReg  , 0x42002810U) /**< \brief (TC2) Count32 Count Register 0 */
#define REG_TC2_COUNT8_COUNT      REG_ACCESS(RwReg8 , 0x42002810U) /**< \brief (TC2) Count8 Count Register 0 */
#define REG_TC2_COUNT8_PER        REG_ACCESS(RwReg8 , 0x42002814U) /**< \brief (TC2) Count8 Period Register */
#define REG_TC2_COUNT16_CC_CC     REG_ACCESS(RwReg16, 0x42002818U) /**< \brief (TC2) Count16Cc Compare and Capture Register */
#define REG_TC2_COUNT32_CC_CC     REG_ACCESS(RwReg  , 0x42002818U) /**< \brief (TC2) Count32Cc Compare and Capture Register */
#define REG_TC2_COUNT8_CC_CC      REG_ACCESS(RwReg8 , 0x42002818U) /**< \brief (TC2) Count8Cc Compare and Capture Register */
/* ========== Register definition for TC3 peripheral ========== */
#define REG_TC3_CTRLA             REG_ACCESS(RwReg16, 0x42002C00U) /**< \brief (TC3) Control Register A */
#define REG_TC3_READREQ           REG_ACCESS(RwReg16, 0x42002C02U) /**< \brief (TC3) Read Request Register */
#define REG_TC3_CTRLBCLR          REG_ACCESS(RwReg8 , 0x42002C04U) /**< \brief (TC3) Control Clear Register B */
#define REG_TC3_CTRLBSET          REG_ACCESS(RwReg8 , 0x42002C05U) /**< \brief (TC3) Control Set Register B */
#define REG_TC3_CTRLC             REG_ACCESS(RwReg8 , 0x42002C06U) /**< \brief (TC3) Control Register C */
#define REG_TC3_DEBUG             REG_ACCESS(RwReg8 , 0x42002C08U) /**< \brief (TC3) Control Register C */
#define REG_TC3_EVCTRL            REG_ACCESS(RwReg16, 0x42002C0AU) /**< \brief (TC3) Event Control Register */
#define REG_TC3_INTENCLR          REG_ACCESS(RwReg8 , 0x42002C0CU) /**< \brief (TC3) Interrupt Enable Clear Register */
#define REG_TC3_INTENSET          REG_ACCESS(RwReg8 , 0x42002C0DU) /**< \brief (TC3) Interrupt Enable Set Register */
#define REG_TC3_INTFLAG           REG_ACCESS(RwReg8 , 0x42002C0EU) /**< \brief (TC3) Interrupt Flag Register */
#define REG_TC3_STATUS            REG_ACCESS(RwReg8 , 0x42002C0FU) /**< \brief (TC3) Status Register */
#define REG_TC3_COUNT16_COUNT     REG_ACCESS(RwReg16, 0x42002C10U) /**< \brief (TC3) Count16 Count Register 0 */
#define REG_TC3_COUNT32_COUNT     REG_ACCESS(RwReg  , 0x42002C10U) /**< \brief (TC3) Count32 Count Register 0 */
#define REG_TC3_COUNT8_COUNT      REG_ACCESS(RwReg8 , 0x42002C10U) /**< \brief (TC3) Count8 Count Register 0 */
#define REG_TC3_COUNT8_PER        REG_ACCESS(RwReg8 , 0x42002C14U) /**< \brief (TC3) Count8 Period Register */
#define REG_TC3_COUNT16_CC_CC     REG_ACCESS(RwReg16, 0x42002C18U) /**< \brief (TC3) Count16Cc Compare and Capture Register */
#define REG_TC3_COUNT32_CC_CC     REG_ACCESS(RwReg  , 0x42002C18U) /**< \brief (TC3) Count32Cc Compare and Capture Register */
#define REG_TC3_COUNT8_CC_CC      REG_ACCESS(RwReg8 , 0x42002C18U) /**< \brief (TC3) Count8Cc Compare and Capture Register */
/* ========== Register definition for TC4 peripheral ========== */
#define REG_TC4_CTRLA             REG_ACCESS(RwReg16, 0x42003000U) /**< \brief (TC4) Control Register A */
#define REG_TC4_READREQ           REG_ACCESS(RwReg16, 0x42003002U) /**< \brief (TC4) Read Request Register */
#define REG_TC4_CTRLBCLR          REG_ACCESS(RwReg8 , 0x42003004U) /**< \brief (TC4) Control Clear Register B */
#define REG_TC4_CTRLBSET          REG_ACCESS(RwReg8 , 0x42003005U) /**< \brief (TC4) Control Set Register B */
#define REG_TC4_CTRLC             REG_ACCESS(RwReg8 , 0x42003006U) /**< \brief (TC4) Control Register C */
#define REG_TC4_DEBUG             REG_ACCESS(RwReg8 , 0x42003008U) /**< \brief (TC4) Control Register C */
#define REG_TC4_EVCTRL            REG_ACCESS(RwReg16, 0x4200300AU) /**< \brief (TC4) Event Control Register */
#define REG_TC4_INTENCLR          REG_ACCESS(RwReg8 , 0x4200300CU) /**< \brief (TC4) Interrupt Enable Clear Register */
#define REG_TC4_INTENSET          REG_ACCESS(RwReg8 , 0x4200300DU) /**< \brief (TC4) Interrupt Enable Set Register */
#define REG_TC4_INTFLAG           REG_ACCESS(RwReg8 , 0x4200300EU) /**< \brief (TC4) Interrupt Flag Register */
#define REG_TC4_STATUS            REG_ACCESS(RwReg8 , 0x4200300FU) /**< \brief (TC4) Status Register */
#define REG_TC4_COUNT16_COUNT     REG_ACCESS(RwReg16, 0x42003010U) /**< \brief (TC4) Count16 Count Register 0 */
#define REG_TC4_COUNT32_COUNT     REG_ACCESS(RwReg  , 0x42003010U) /**< \brief (TC4) Count32 Count Register 0 */
#define REG_TC4_COUNT8_COUNT      REG_ACCESS(RwReg8 , 0x42003010U) /**< \brief (TC4) Count8 Count Register 0 */
#define REG_TC4_COUNT8_PER        REG_ACCESS(RwReg8 , 0x42003014U) /**< \brief (TC4) Count8 Period Register */
#define REG_TC4_COUNT16_CC_CC     REG_ACCESS(RwReg16, 0x42003018U) /**< \brief (TC4) Count16Cc Compare and Capture Register */
#define REG_TC4_COUNT32_CC_CC     REG_ACCESS(RwReg  , 0x42003018U) /**< \brief (TC4) Count32Cc Compare and Capture Register */
#define REG_TC4_COUNT8_CC_CC      REG_ACCESS(RwReg8 , 0x42003018U) /**< \brief (TC4) Count8Cc Compare and Capture Register */
/* ========== Register definition for TC5 peripheral ========== */
#define REG_TC5_CTRLA             REG_ACCESS(RwReg16, 0x42003400U) /**< \brief (TC5) Control Register A */
#define REG_TC5_READREQ           REG_ACCESS(RwReg16, 0x42003402U) /**< \brief (TC5) Read Request Register */
#define REG_TC5_CTRLBCLR          REG_ACCESS(RwReg8 , 0x42003404U) /**< \brief (TC5) Control Clear Register B */
#define REG_TC5_CTRLBSET          REG_ACCESS(RwReg8 , 0x42003405U) /**< \brief (TC5) Control Set Register B */
#define REG_TC5_CTRLC             REG_ACCESS(RwReg8 , 0x42003406U) /**< \brief (TC5) Control Register C */
#define REG_TC5_DEBUG             REG_ACCESS(RwReg8 , 0x42003408U) /**< \brief (TC5) Control Register C */
#define REG_TC5_EVCTRL            REG_ACCESS(RwReg16, 0x4200340AU) /**< \brief (TC5) Event Control Register */
#define REG_TC5_INTENCLR          REG_ACCESS(RwReg8 , 0x4200340CU) /**< \brief (TC5) Interrupt Enable Clear Register */
#define REG_TC5_INTENSET          REG_ACCESS(RwReg8 , 0x4200340DU) /**< \brief (TC5) Interrupt Enable Set Register */
#define REG_TC5_INTFLAG           REG_ACCESS(RwReg8 , 0x4200340EU) /**< \brief (TC5) Interrupt Flag Register */
#define REG_TC5_STATUS            REG_ACCESS(RwReg8 , 0x4200340FU) /**< \brief (TC5) Status Register */
#define REG_TC5_COUNT16_COUNT     REG_ACCESS(RwReg16, 0x42003410U) /**< \brief (TC5) Count16 Count Register 0 */
#define REG_TC5_COUNT32_COUNT     REG_ACCESS(RwReg  , 0x42003410U) /**< \brief (TC5) Count32 Count Register 0 */
#define REG_TC5_COUNT8_COUNT      REG_ACCESS(RwReg8 , 0x42003410U) /**< \brief (TC5) Count8 Count Register 0 */
#define REG_TC5_COUNT8_PER        REG_ACCESS(RwReg8 , 0x42003414U) /**< \brief (TC5) Count8 Period Register */
#define REG_TC5_COUNT16_CC_CC     REG_ACCESS(RwReg16, 0x42003418U) /**< \brief (TC5) Count16Cc Compare and Capture Register */
#define REG_TC5_COUNT32_CC_CC     REG_ACCESS(RwReg  , 0x42003418U) /**< \brief (TC5) Count32Cc Compare and Capture Register */
#define REG_TC5_COUNT8_CC_CC      REG_ACCESS(RwReg8 , 0x42003418U) /**< \brief (TC5) Count8Cc Compare and Capture Register */
/* ========== Register definition for TC6 peripheral ========== */
#define REG_TC6_CTRLA             REG_ACCESS(RwReg16, 0x42003800U) /**< \brief (TC6) Control Register A */
#define REG_TC6_READREQ           REG_ACCESS(RwReg16, 0x42003802U) /**< \brief (TC6) Read Request Register */
#define REG_TC6_CTRLBCLR          REG_ACCESS(RwReg8 , 0x42003804U) /**< \brief (TC6) Control Clear Register B */
#define REG_TC6_CTRLBSET          REG_ACCESS(RwReg8 , 0x42003805U) /**< \brief (TC6) Control Set Register B */
#define REG_TC6_CTRLC             REG_ACCESS(RwReg8 , 0x42003806U) /**< \brief (TC6) Control Register C */
#define REG_TC6_DEBUG             REG_ACCESS(RwReg8 , 0x42003808U) /**< \brief (TC6) Control Register C */
#define REG_TC6_EVCTRL            REG_ACCESS(RwReg16, 0x4200380AU) /**< \brief (TC6) Event Control Register */
#define REG_TC6_INTENCLR          REG_ACCESS(RwReg8 , 0x4200380CU) /**< \brief (TC6) Interrupt Enable Clear Register */
#define REG_TC6_INTENSET          REG_ACCESS(RwReg8 , 0x4200380DU) /**< \brief (TC6) Interrupt Enable Set Register */
#define REG_TC6_INTFLAG           REG_ACCESS(RwReg8 , 0x4200380EU) /**< \brief (TC6) Interrupt Flag Register */
#define REG_TC6_STATUS            REG_ACCESS(RwReg8 , 0x4200380FU) /**< \brief (TC6) Status Register */
#define REG_TC6_COUNT16_COUNT     REG_ACCESS(RwReg16, 0x42003810U) /**< \brief (TC6) Count16 Count Register 0 */
#define REG_TC6_COUNT32_COUNT     REG_ACCESS(RwReg  , 0x42003810U) /**< \brief (TC6) Count32 Count Register 0 */
#define REG_TC6_COUNT8_COUNT      REG_ACCESS(RwReg8 , 0x42003810U) /**< \brief (TC6) Count8 Count Register 0 */
#define REG_TC6_COUNT8_PER        REG_ACCESS(RwReg8 , 0x42003814U) /**< \brief (TC6) Count8 Period Register */
#define REG_TC6_COUNT16_CC_CC     REG_ACCESS(RwReg16, 0x42003818U) /**< \brief (TC6) Count16Cc Compare and Capture Register */
#define REG_TC6_COUNT32_CC_CC     REG_ACCESS(RwReg  , 0x42003818U) /**< \brief (TC6) Count32Cc Compare and Capture Register */
#define REG_TC6_COUNT8_CC_CC      REG_ACCESS(RwReg8 , 0x42003818U) /**< \brief (TC6) Count8Cc Compare and Capture Register */
/* ========== Register definition for TC7 peripheral ========== */
#define REG_TC7_CTRLA             REG_ACCESS(RwReg16, 0x42003C00U) /**< \brief (TC7) Control Register A */
#define REG_TC7_READREQ           REG_ACCESS(RwReg16, 0x42003C02U) /**< \brief (TC7) Read Request Register */
#define REG_TC7_CTRLBCLR          REG_ACCESS(RwReg8 , 0x42003C04U) /**< \brief (TC7) Control Clear Register B */
#define REG_TC7_CTRLBSET          REG_ACCESS(RwReg8 , 0x42003C05U) /**< \brief (TC7) Control Set Register B */
#define REG_TC7_CTRLC             REG_ACCESS(RwReg8 , 0x42003C06U) /**< \brief (TC7) Control Register C */
#define REG_TC7_DEBUG             REG_ACCESS(RwReg8 , 0x42003C08U) /**< \brief (TC7) Control Register C */
#define REG_TC7_EVCTRL            REG_ACCESS(RwReg16, 0x42003C0AU) /**< \brief (TC7) Event Control Register */
#define REG_TC7_INTENCLR          REG_ACCESS(RwReg8 , 0x42003C0CU) /**< \brief (TC7) Interrupt Enable Clear Register */
#define REG_TC7_INTENSET          REG_ACCESS(RwReg8 , 0x42003C0DU) /**< \brief (TC7) Interrupt Enable Set Register */
#define REG_TC7_INTFLAG           REG_ACCESS(RwReg8 , 0x42003C0EU) /**< \brief (TC7) Interrupt Flag Register */
#define REG_TC7_STATUS            REG_ACCESS(RwReg8 , 0x42003C0FU) /**< \brief (TC7) Status Register */
#define REG_TC7_COUNT16_COUNT     REG_ACCESS(RwReg16, 0x42003C10U) /**< \brief (TC7) Count16 Count Register 0 */
#define REG_TC7_COUNT32_COUNT     REG_ACCESS(RwReg  , 0x42003C10U) /**< \brief (TC7) Count32 Count Register 0 */
#define REG_TC7_COUNT8_COUNT      REG_ACCESS(RwReg8 , 0x42003C10U) /**< \brief (TC7) Count8 Count Register 0 */
#define REG_TC7_COUNT8_PER        REG_ACCESS(RwReg8 , 0x42003C14U) /**< \brief (TC7) Count8 Period Register */
#define REG_TC7_COUNT16_CC_CC     REG_ACCESS(RwReg16, 0x42003C18U) /**< \brief (TC7) Count16Cc Compare and Capture Register */
#define REG_TC7_COUNT32_CC_CC     REG_ACCESS(RwReg  , 0x42003C18U) /**< \brief (TC7) Count32Cc Compare and Capture Register */
#define REG_TC7_COUNT8_CC_CC      REG_ACCESS(RwReg8 , 0x42003C18U) /**< \brief (TC7) Count8Cc Compare and Capture Register */
/* ========== Register definition for ADC peripheral ========== */
#define REG_ADC_CTRLA             REG_ACCESS(RwReg8 , 0x42004000U) /**< \brief (ADC) Control register A */
#define REG_ADC_REFCTRL           REG_ACCESS(RwReg8 , 0x42004001U) /**< \brief (ADC) Reference control register */
#define REG_ADC_AVGCTRL           REG_ACCESS(RwReg8 , 0x42004002U) /**< \brief (ADC) Average Control register */
#define REG_ADC_SAMPCTRL          REG_ACCESS(RwReg8 , 0x42004003U) /**< \brief (ADC) Sample Time Control register */
#define REG_ADC_CTRLB             REG_ACCESS(RwReg16, 0x42004004U) /**< \brief (ADC) Control Register B */
#define REG_ADC_WINCTRL           REG_ACCESS(RwReg8 , 0x42004008U) /**< \brief (ADC) Window Monitor control register */
#define REG_ADC_SWTRIG            REG_ACCESS(RwReg8 , 0x4200400CU) /**< \brief (ADC) Control register B */
#define REG_ADC_INPUTCTRL         REG_ACCESS(RwReg  , 0x42004010U) /**< \brief (ADC) Input controlregister */
#define REG_ADC_EVCTRL            REG_ACCESS(RwReg8 , 0x42004014U) /**< \brief (ADC) Event control register */
#define REG_ADC_INTENCLR          REG_ACCESS(RwReg8 , 0x42004016U) /**< \brief (ADC) Interrupt Enable Clear register */
#define REG_ADC_INTENSET          REG_ACCESS(RwReg8 , 0x42004017U) /**< \brief (ADC) Interrupt Enable Set register */
#define REG_ADC_INTFLAG           REG_ACCESS(RwReg8 , 0x42004018U) /**< \brief (ADC) Interrupt Flag Status and Claer register */
#define REG_ADC_STATUS            REG_ACCESS(RoReg8 , 0x42004019U) /**< \brief (ADC) Status register */
#define REG_ADC_RESULT            REG_ACCESS(RoReg16, 0x4200401AU) /**< \brief (ADC) Result register */
#define REG_ADC_WINLT             REG_ACCESS(RwReg16, 0x4200401CU) /**< \brief (ADC) Window Monitor Lower Threshold register */
#define REG_ADC_WINUT             REG_ACCESS(RwReg16, 0x42004020U) /**< \brief (ADC) Window Monitor Upper threshold register */
#define REG_ADC_GAINCORR          REG_ACCESS(RwReg16, 0x42004024U) /**< \brief (ADC) Gain Correction register */
#define REG_ADC_OFFSETCORR        REG_ACCESS(RwReg16, 0x42004026U) /**< \brief (ADC) Offset Correction register */
#define REG_ADC_CALIB             REG_ACCESS(RwReg16, 0x42004028U) /**< \brief (ADC) Calibration register */
#define REG_ADC_DEBUG             REG_ACCESS(RwReg8 , 0x4200402AU) /**< \brief (ADC) Debug register */
#define REG_ADC_TEST              REG_ACCESS(RwReg8 , 0x4200402BU) /**< \brief (ADC) Test modes register */
#define REG_ADC_TESTRESULT        REG_ACCESS(RwReg  , 0x4200402CU) /**< \brief (ADC) Test Result register */
#define REG_ADC_DCFG              REG_ACCESS(RwReg8 , 0x42004030U) /**< \brief (ADC) Device configuration */
/* ========== Register definition for AC peripheral ========== */
#define REG_AC_CTRLA              REG_ACCESS(RwReg8 , 0x42004400U) /**< \brief (AC) Control A Register */
#define REG_AC_CTRLB              REG_ACCESS(WoReg8 , 0x42004401U) /**< \brief (AC) Control B Register */
#define REG_AC_EVCTRL             REG_ACCESS(RwReg16, 0x42004402U) /**< \brief (AC) Event Control Register */
#define REG_AC_INTENCLR           REG_ACCESS(RwReg8 , 0x42004404U) /**< \brief (AC) Interrupt Enable Clear Register */
#define REG_AC_INTENSET           REG_ACCESS(RwReg8 , 0x42004405U) /**< \brief (AC) Interupt Enable Set Register */
#define REG_AC_INTFLAG            REG_ACCESS(RwReg8 , 0x42004406U) /**< \brief (AC) Interrupt Flag Register */
#define REG_AC_STATUSA            REG_ACCESS(RoReg8 , 0x42004408U) /**< \brief (AC) Status A Register */
#define REG_AC_STATUSB            REG_ACCESS(RoReg8 , 0x42004409U) /**< \brief (AC) Status B Register */
#define REG_AC_STATUSC            REG_ACCESS(RoReg8 , 0x4200440AU) /**< \brief (AC) Status C Register */
#define REG_AC_WINCTRL            REG_ACCESS(RwReg8 , 0x4200440CU) /**< \brief (AC) Window Control Register */
#define REG_AC_COMPCTRL0          REG_ACCESS(RwReg  , 0x42004410U) /**< \brief (AC) Comparator Control Register 0 */
#define REG_AC_COMPCTRL1          REG_ACCESS(RwReg  , 0x42004414U) /**< \brief (AC) Comparator Control Register 1 */
#define REG_AC_SCALER0            REG_ACCESS(RwReg8 , 0x42004420U) /**< \brief (AC) Scaler Register 0 */
#define REG_AC_SCALER1            REG_ACCESS(RwReg8 , 0x42004421U) /**< \brief (AC) Scaler Register 1 */
/* ========== Register definition for DAC peripheral ========== */
#define REG_DAC_CTRLA             REG_ACCESS(RwReg8 , 0x42004800U) /**< \brief (DAC) Control Register A */
#define REG_DAC_CTRLB             REG_ACCESS(RwReg8 , 0x42004801U) /**< \brief (DAC) Control Register B */
#define REG_DAC_EVCTRL            REG_ACCESS(RwReg8 , 0x42004802U) /**< \brief (DAC) Event Control Register */
#define REG_DAC_TEST              REG_ACCESS(RwReg8 , 0x42004803U) /**< \brief (DAC) Test Register */
#define REG_DAC_INTENCLR          REG_ACCESS(RwReg8 , 0x42004804U) /**< \brief (DAC) Interrupt Enable Clear Regiser */
#define REG_DAC_INTENSET          REG_ACCESS(RwReg8 , 0x42004805U) /**< \brief (DAC) Interrupt Enable Set Register */
#define REG_DAC_INTFLAG           REG_ACCESS(RwReg8 , 0x42004806U) /**< \brief (DAC) Interrupt Flag Status and Clear Register */
#define REG_DAC_STATUS            REG_ACCESS(RoReg8 , 0x42004807U) /**< \brief (DAC) Status Register */
#define REG_DAC_DATA              REG_ACCESS(RwReg16, 0x42004808U) /**< \brief (DAC) Data Register */
#define REG_DAC_DATABUF           REG_ACCESS(RwReg16, 0x4200480CU) /**< \brief (DAC) Data Buffer Register */
/* ========== Register definition for PTC peripheral ========== */
#define REG_PTC_CTRLA             REG_ACCESS(RwReg8 , 0x42004C00U) /**< \brief (PTC) Control Register A */
#define REG_PTC_STATUS            REG_ACCESS(RoReg8 , 0x42004C01U) /**< \brief (PTC) Status Register */
#define REG_PTC_CTRLB             REG_ACCESS(RwReg8 , 0x42004C04U) /**< \brief (PTC) Control Register B */
#define REG_PTC_EVCTRL            REG_ACCESS(RwReg8 , 0x42004C05U) /**< \brief (PTC) Event Control Register */
#define REG_PTC_INTENCLR          REG_ACCESS(RwReg8 , 0x42004C08U) /**< \brief (PTC) Interrupt Enable Clear */
#define REG_PTC_INTENSET          REG_ACCESS(RwReg8 , 0x42004C09U) /**< \brief (PTC) Interrupt Enable Set */
#define REG_PTC_INTFLAG           REG_ACCESS(RwReg8 , 0x42004C0AU) /**< \brief (PTC) Interrupt Flag */
#define REG_PTC_CTSCTRLA          REG_ACCESS(RwReg8 , 0x42004C0CU) /**< \brief (PTC) CTS Control Register A */
#define REG_PTC_CTSCTRLC          REG_ACCESS(RwReg8 , 0x42004C0DU) /**< \brief (PTC) CTS Control Register C */
#define REG_PTC_YSEL              REG_ACCESS(RwReg16, 0x42004C10U) /**< \brief (PTC) Y Line Select */
#define REG_PTC_XSEL              REG_ACCESS(RwReg16, 0x42004C12U) /**< \brief (PTC) X Line Select */
#define REG_PTC_YEN               REG_ACCESS(RwReg16, 0x42004C14U) /**< \brief (PTC) Y Line Enable */
#define REG_PTC_XEN               REG_ACCESS(RwReg16, 0x42004C16U) /**< \brief (PTC) X Line Enable */
#define REG_PTC_CCCAL             REG_ACCESS(RwReg16, 0x42004C18U) /**< \brief (PTC) CTS Compensation Capacitor Calibration Register */
#define REG_PTC_CICAL             REG_ACCESS(RwReg8 , 0x42004C1AU) /**< \brief (PTC) CTS Integrator Capacitor Calibration Register */
#define REG_PTC_CTSRS             REG_ACCESS(RwReg8 , 0x42004C1BU) /**< \brief (PTC) CTS Resistor Selection Register */
#define REG_PTC_RESULT            REG_ACCESS(RwReg16, 0x42004C1CU) /**< \brief (PTC) Conversion Result Register */
#define REG_PTC_CTSCTRLB          REG_ACCESS(RwReg8 , 0x42004C20U) /**< \brief (PTC) CTS Control Register B */
#define REG_PTC_WINCCTRL          REG_ACCESS(RwReg8 , 0x42004C21U) /**< \brief (PTC) Window Comparator Control Register */
#define REG_PTC_WCHT              REG_ACCESS(RwReg16, 0x42004C24U) /**< \brief (PTC) Window Comparator High Threshold Register */
#define REG_PTC_WCLT              REG_ACCESS(RwReg16, 0x42004C26U) /**< \brief (PTC) Window Comparator Low Threshold Register */
/* ========== Register definition for ATW peripheral ========== */

/*@}*/

/* ************************************************************************** */
/**  PERIPHERAL ID DEFINITIONS FOR SAM0D4_REVA */
/* ************************************************************************** */
/** \addtogroup SAM0D4_REVA_id Peripheral Ids Definitions */
/*@{*/

// Peripheral instances on HPB0 bridge
#define ID_PAC0         0 /**< \brief Peripheral Access Controller PAC (PAC0) */
#define ID_PM           1 /**< \brief Power Manager (PM) */
#define ID_SYSCTRL      2 /**< \brief System Control (SYSCTRL) */
#define ID_GCLK         3 /**< \brief Generic Clock Generator (GCLK) */
#define ID_WDT          4 /**< \brief Watchdog Timer (WDT) */
#define ID_RTC          5 /**< \brief Real-Time Counter (RTC) */
#define ID_EIC          6 /**< \brief External Interrupt Controller (EIC) */

// Peripheral instances on HPB1 bridge
#define ID_PAC1        32 /**< \brief Peripheral Access Controller PAC (PAC1) */
#define ID_DSU         33 /**< \brief Device Service Unit (DSU) */
#define ID_NVMCTRL     34 /**< \brief NVM Controller (NVMCTRL) */
#define ID_PORT        35 /**< \brief Port Module (PORT) */
#define ID_HMATRIX     36 /**< \brief HSB Matrix (HMATRIX) */

// Peripheral instances on HPB2 bridge
#define ID_PAC2        64 /**< \brief Peripheral Access Controller PAC (PAC2) */
#define ID_EVSYS       65 /**< \brief Event System Interface (EVSYS) */
#define ID_SERCOM0     66 /**< \brief Serial Communication Interface SERCOM (SERCOM0) */
#define ID_SERCOM1     67 /**< \brief Serial Communication Interface SERCOM (SERCOM1) */
#define ID_SERCOM2     68 /**< \brief Serial Communication Interface SERCOM (SERCOM2) */
#define ID_SERCOM3     69 /**< \brief Serial Communication Interface SERCOM (SERCOM3) */
#define ID_SERCOM4     70 /**< \brief Serial Communication Interface SERCOM (SERCOM4) */
#define ID_SERCOM5     71 /**< \brief Serial Communication Interface SERCOM (SERCOM5) */
#define ID_TC0         72 /**< \brief Basic Timer Counter TC (TC0) */
#define ID_TC1         73 /**< \brief Basic Timer Counter TC (TC1) */
#define ID_TC2         74 /**< \brief Basic Timer Counter TC (TC2) */
#define ID_TC3         75 /**< \brief Basic Timer Counter TC (TC3) */
#define ID_TC4         76 /**< \brief Basic Timer Counter TC (TC4) */
#define ID_TC5         77 /**< \brief Basic Timer Counter TC (TC5) */
#define ID_TC6         78 /**< \brief Basic Timer Counter TC (TC6) */
#define ID_TC7         79 /**< \brief Basic Timer Counter TC (TC7) */
#define ID_ADC         80 /**< \brief Analog Digital Converter (ADC) */
#define ID_AC          81 /**< \brief Analog Comparators (AC) */
#define ID_DAC         82 /**< \brief Digital Analog Converter (DAC) */
#define ID_PTC         83 /**< \brief Peripheral Touch Controller (PTC) */
#define ID_ATW         84 /**< \brief Analog Test Wire Controller (ATW) */

/*@}*/

/* ************************************************************************** */
/**  BASE ADDRESS DEFINITIONS FOR SAM0D4_REVA */
/* ************************************************************************** */
/** \addtogroup SAM0D4_REVA_base Peripheral Base Address Definitions */
/*@{*/

#define AC_ADDR           (0x42004400U)                  /**< \brief (AC) APB Base Address */
#define AC                CAST(Ac,        AC_ADDR)
#define AC_INST_NUM       1                              /**< \brief (AC) Number of instances */
#define AC_INSTS          { AC }                         /**< \brief (AC) Instances List */

#define ADC_ADDR          (0x42004000U)                  /**< \brief (ADC) APB Base Address */
#define ADC               CAST(Adc,       ADC_ADDR)
#define ADC_INST_NUM      1                              /**< \brief (ADC) Number of instances */
#define ADC_INSTS         { ADC }                        /**< \brief (ADC) Instances List */

#define ATW_ADDR          (0x42005000U)                  /**< \brief (ATW) APB Base Address */
#define ATW               CAST(Atw,       ATW_ADDR)
#define ATW_INST_NUM      1                              /**< \brief (ATW) Number of instances */
#define ATW_INSTS         { ATW }                        /**< \brief (ATW) Instances List */

#define DAC_ADDR          (0x42004800U)                  /**< \brief (DAC) APB Base Address */
#define DAC               CAST(Dac,       DAC_ADDR)
#define DAC_INST_NUM      1                              /**< \brief (DAC) Number of instances */
#define DAC_INSTS         { DAC }                        /**< \brief (DAC) Instances List */

#define DSU_ADDR          (0x41002000U)                  /**< \brief (DSU) APB Base Address */
#define DSU               CAST(Dsu,       DSU_ADDR)
#define DSU_INST_NUM      1                              /**< \brief (DSU) Number of instances */
#define DSU_INSTS         { DSU }                        /**< \brief (DSU) Instances List */

#define EIC_ADDR          (0x40001800U)                  /**< \brief (EIC) APB Base Address */
#define EIC               CAST(Eic,       EIC_ADDR)
#define EIC_INST_NUM      1                              /**< \brief (EIC) Number of instances */
#define EIC_INSTS         { EIC }                        /**< \brief (EIC) Instances List */

#define EVSYS_ADDR        (0x42000400U)                  /**< \brief (EVSYS) APB Base Address */
#define EVSYS             CAST(Evsys,     EVSYS_ADDR)
#define EVSYS_INST_NUM    1                              /**< \brief (EVSYS) Number of instances */
#define EVSYS_INSTS       { EVSYS }                      /**< \brief (EVSYS) Instances List */

#define GCLK_ADDR         (0x40000C00U)                  /**< \brief (GCLK) APB Base Address */
#define GCLK              CAST(Gclk,      GCLK_ADDR)
#define GCLK_INST_NUM     1                              /**< \brief (GCLK) Number of instances */
#define GCLK_INSTS        { GCLK }                       /**< \brief (GCLK) Instances List */

#define HMATRIX_ADDR      (0x41004800U)                  /**< \brief (HMATRIX) APB Base Address */
#define HMATRIX           CAST(Hmatrixb,  HMATRIX_ADDR)
#define HMATRIXB_INST_NUM 1                              /**< \brief (HMATRIXB) Number of instances */
#define HMATRIXB_INSTS    { HMATRIX }                    /**< \brief (HMATRIXB) Instances List */

#define NVMCTRL_ADDR      (0x41004000U)                  /**< \brief (NVMCTRL) APB Base Address */
#define NVMCTRL           CAST(Nvmctrl,   NVMCTRL_ADDR)
#define NVMCTRL_INST_NUM  1                              /**< \brief (NVMCTRL) Number of instances */
#define NVMCTRL_INSTS     { NVMCTRL }                    /**< \brief (NVMCTRL) Instances List */

#define PAC0_ADDR         (0x40000000U)                  /**< \brief (PAC0) APB Base Address */
#define PAC0              CAST(Pac,       PAC0_ADDR)
#define PAC1_ADDR         (0x41000000U)                  /**< \brief (PAC1) APB Base Address */
#define PAC1              CAST(Pac,       PAC1_ADDR)
#define PAC2_ADDR         (0x42000000U)                  /**< \brief (PAC2) APB Base Address */
#define PAC2              CAST(Pac,       PAC2_ADDR)
#define PAC_INST_NUM      3                              /**< \brief (PAC) Number of instances */
#define PAC_INSTS         { PAC0, PAC1, PAC2 }           /**< \brief (PAC) Instances List */

#define PM_ADDR           (0x40000400U)                  /**< \brief (PM) APB Base Address */
#define PM                CAST(Pm,        PM_ADDR)
#define PM_INST_NUM       1                              /**< \brief (PM) Number of instances */
#define PM_INSTS          { PM }                         /**< \brief (PM) Instances List */

#define PORT_ADDR         (0x41004400U)                  /**< \brief (PORT) APB Base Address */
#define PORT              CAST(Port,      PORT_ADDR)
#define PORT_IOBUS_ADDR   (0x60000000U)                  /**< \brief (PORT) IOBUS Base Address */
#define PORT_INST_NUM     1                              /**< \brief (PORT) Number of instances */
#define PORT_INSTS        { PORT }                       /**< \brief (PORT) Instances List */

#define PTC_ADDR          (0x42004C00U)                  /**< \brief (PTC) APB Base Address */
#define PTC               CAST(Ptc,       PTC_ADDR)
#define PTC_INST_NUM      1                              /**< \brief (PTC) Number of instances */
#define PTC_INSTS         { PTC }                        /**< \brief (PTC) Instances List */

#define RTC_ADDR          (0x40001400U)                  /**< \brief (RTC) APB Base Address */
#define RTC               CAST(Rtc,       RTC_ADDR)
#define RTC_INST_NUM      1                              /**< \brief (RTC) Number of instances */
#define RTC_INSTS         { RTC }                        /**< \brief (RTC) Instances List */

#define SERCOM0_ADDR      (0x42000800U)                  /**< \brief (SERCOM0) APB Base Address */
#define SERCOM0           CAST(Sercom,    SERCOM0_ADDR)
#define SERCOM1_ADDR      (0x42000C00U)                  /**< \brief (SERCOM1) APB Base Address */
#define SERCOM1           CAST(Sercom,    SERCOM1_ADDR)
#define SERCOM2_ADDR      (0x42001000U)                  /**< \brief (SERCOM2) APB Base Address */
#define SERCOM2           CAST(Sercom,    SERCOM2_ADDR)
#define SERCOM3_ADDR      (0x42001400U)                  /**< \brief (SERCOM3) APB Base Address */
#define SERCOM3           CAST(Sercom,    SERCOM3_ADDR)
#define SERCOM4_ADDR      (0x42001800U)                  /**< \brief (SERCOM4) APB Base Address */
#define SERCOM4           CAST(Sercom,    SERCOM4_ADDR)
#define SERCOM5_ADDR      (0x42001C00U)                  /**< \brief (SERCOM5) APB Base Address */
#define SERCOM5           CAST(Sercom,    SERCOM5_ADDR)
#define SERCOM_INST_NUM   6                              /**< \brief (SERCOM) Number of instances */
#define SERCOM_INSTS      { SERCOM0, SERCOM1, SERCOM2, SERCOM3, SERCOM4, SERCOM5 } /**< \brief (SERCOM) Instances List */

#define SYSCTRL_ADDR      (0x40000800U)                  /**< \brief (SYSCTRL) APB Base Address */
#define SYSCTRL           CAST(Sysctrl,   SYSCTRL_ADDR)
#define SYSCTRL_INST_NUM  1                              /**< \brief (SYSCTRL) Number of instances */
#define SYSCTRL_INSTS     { SYSCTRL }                    /**< \brief (SYSCTRL) Instances List */

#define TC0_ADDR          (0x42002000U)                  /**< \brief (TC0) APB Base Address */
#define TC0               CAST(Tc,        TC0_ADDR)
#define TC1_ADDR          (0x42002400U)                  /**< \brief (TC1) APB Base Address */
#define TC1               CAST(Tc,        TC1_ADDR)
#define TC2_ADDR          (0x42002800U)                  /**< \brief (TC2) APB Base Address */
#define TC2               CAST(Tc,        TC2_ADDR)
#define TC3_ADDR          (0x42002C00U)                  /**< \brief (TC3) APB Base Address */
#define TC3               CAST(Tc,        TC3_ADDR)
#define TC4_ADDR          (0x42003000U)                  /**< \brief (TC4) APB Base Address */
#define TC4               CAST(Tc,        TC4_ADDR)
#define TC5_ADDR          (0x42003400U)                  /**< \brief (TC5) APB Base Address */
#define TC5               CAST(Tc,        TC5_ADDR)
#define TC6_ADDR          (0x42003800U)                  /**< \brief (TC6) APB Base Address */
#define TC6               CAST(Tc,        TC6_ADDR)
#define TC7_ADDR          (0x42003C00U)                  /**< \brief (TC7) APB Base Address */
#define TC7               CAST(Tc,        TC7_ADDR)
#define TC_INST_NUM       8                              /**< \brief (TC) Number of instances */
#define TC_INSTS          { TC0, TC1, TC2, TC3, TC4, TC5, TC6, TC7 } /**< \brief (TC) Instances List */

#define WDT_ADDR          (0x40001000U)                  /**< \brief (WDT) APB Base Address */
#define WDT               CAST(Wdt,       WDT_ADDR)
#define WDT_INST_NUM      1                              /**< \brief (WDT) Number of instances */
#define WDT_INSTS         { WDT }                        /**< \brief (WDT) Instances List */

/*@}*/

/* ************************************************************************** */
/**  INSTANCE PARAMETERS DEFINITIONS FOR SAM0D4_REVA */
/* ************************************************************************** */
/** \addtogroup SAM0D4_REVA_parameters Instance Parameters Definitions */
/*@{*/

/* ========== Instance parameters for AC peripheral ========== */
#define AC_GCLK_ID_ANA              25
#define AC_GCLK_ID_DIG              24
#define AC_NUM_CMP                  2
#define AC_PAIRS                    1
/* ========== Instance parameters for ADC peripheral ========== */
#define ADC_ADC_RESULT_MSB          15
#define ADC_GCLK_ID                 23
/* ========== Instance parameters for ATW peripheral ========== */
#define ATW_GROUPS                  2
#define ATW_PINS                    57
#define ATW_SPACERS                 2
/* ========== Instance parameters for DAC peripheral ========== */
#define DAC_GCLK_ID                 26
/* ========== Instance parameters for DSU peripheral ========== */
#define DSU_CLK_HSB_ID              3
/* ========== Instance parameters for EIC peripheral ========== */
#define EIC_GCLK_ID                 3
#define EIC_NUMBER_OF_INTERRUPTS    16
/* ========== Instance parameters for EVSYS peripheral ========== */
#define EVSYS_CHANNELS              8
#define EVSYS_CHANNELS_MSB          7
#define EVSYS_EXT_EVT_MSB           0
#define EVSYS_GCLK_ID_0             4
#define EVSYS_GCLK_ID_1             5
#define EVSYS_GCLK_ID_2             6
#define EVSYS_GCLK_ID_3             7
#define EVSYS_GCLK_ID_4             8
#define EVSYS_GCLK_ID_5             9
#define EVSYS_GCLK_ID_6             10
#define EVSYS_GCLK_ID_7             11
#define EVSYS_GCLK_ID_LSB           4
#define EVSYS_GCLK_ID_MSB           11
#define EVSYS_GCLK_ID_SIZE          8
#define EVSYS_GENERATORS            49
#define EVSYS_USERS                 14

// GENERATORS
#define EVSYS_ID_GEN_RTC_CMP_0                 1
#define EVSYS_ID_GEN_RTC_CMP_1                 2
#define EVSYS_ID_GEN_RTC_OVF                   3
#define EVSYS_ID_GEN_RTC_PER_0                 4
#define EVSYS_ID_GEN_RTC_PER_1                 5
#define EVSYS_ID_GEN_RTC_PER_2                 6
#define EVSYS_ID_GEN_RTC_PER_3                 7
#define EVSYS_ID_GEN_RTC_PER_4                 8
#define EVSYS_ID_GEN_RTC_PER_5                 9
#define EVSYS_ID_GEN_RTC_PER_6                 10
#define EVSYS_ID_GEN_RTC_PER_7                 11
#define EVSYS_ID_GEN_EIC_EXTINT_0              12
#define EVSYS_ID_GEN_EIC_EXTINT_1              13
#define EVSYS_ID_GEN_EIC_EXTINT_2              14
#define EVSYS_ID_GEN_EIC_EXTINT_3              15
#define EVSYS_ID_GEN_EIC_EXTINT_4              16
#define EVSYS_ID_GEN_EIC_EXTINT_5              17
#define EVSYS_ID_GEN_EIC_EXTINT_6              18
#define EVSYS_ID_GEN_EIC_EXTINT_7              19
#define EVSYS_ID_GEN_EIC_EXTINT_8              20
#define EVSYS_ID_GEN_EIC_EXTINT_9              21
#define EVSYS_ID_GEN_EIC_EXTINT_10             22
#define EVSYS_ID_GEN_EIC_EXTINT_11             23
#define EVSYS_ID_GEN_EIC_EXTINT_12             24
#define EVSYS_ID_GEN_EIC_EXTINT_13             25
#define EVSYS_ID_GEN_EIC_EXTINT_14             26
#define EVSYS_ID_GEN_EIC_EXTINT_15             27
#define EVSYS_ID_GEN_TC0_OVF                   28
#define EVSYS_ID_GEN_TC0_MCX_0                 29
#define EVSYS_ID_GEN_TC0_MCX_1                 30
#define EVSYS_ID_GEN_TC1_OVF                   31
#define EVSYS_ID_GEN_TC1_MCX_0                 32
#define EVSYS_ID_GEN_TC1_MCX_1                 33
#define EVSYS_ID_GEN_TC2_OVF                   34
#define EVSYS_ID_GEN_TC2_MCX_0                 35
#define EVSYS_ID_GEN_TC2_MCX_1                 36
#define EVSYS_ID_GEN_TC3_OVF                   37
#define EVSYS_ID_GEN_TC3_MCX_0                 38
#define EVSYS_ID_GEN_TC3_MCX_1                 39
#define EVSYS_ID_GEN_TC4_OVF                   40
#define EVSYS_ID_GEN_TC4_MCX_0                 41
#define EVSYS_ID_GEN_TC4_MCX_1                 42
#define EVSYS_ID_GEN_TC5_OVF                   43
#define EVSYS_ID_GEN_TC5_MCX_0                 44
#define EVSYS_ID_GEN_TC5_MCX_1                 45
#define EVSYS_ID_GEN_TC6_OVF                   46
#define EVSYS_ID_GEN_TC6_MCX_0                 47
#define EVSYS_ID_GEN_TC6_MCX_1                 48
#define EVSYS_ID_GEN_TC7_OVF                   49
#define EVSYS_ID_GEN_TC7_MCX_0                 50
#define EVSYS_ID_GEN_TC7_MCX_1                 51
#define EVSYS_ID_GEN_ADC_RESRDY                52
#define EVSYS_ID_GEN_ADC_WINMON                53
#define EVSYS_ID_GEN_AC_COMP_0                 54
#define EVSYS_ID_GEN_AC_COMP_1                 55
#define EVSYS_ID_GEN_AC_WIN                    56
#define EVSYS_ID_GEN_DAC_EMPTY                 57
#define EVSYS_ID_GEN_PTC_EOC                   58
#define EVSYS_ID_GEN_PTC_WCOMP                 59

// USERS
#define EVSYS_ID_USER_TC0_EVU                  0
#define EVSYS_ID_USER_TC1_EVU                  1
#define EVSYS_ID_USER_TC2_EVU                  2
#define EVSYS_ID_USER_TC3_EVU                  3
#define EVSYS_ID_USER_TC4_EVU                  4
#define EVSYS_ID_USER_TC5_EVU                  5
#define EVSYS_ID_USER_TC6_EVU                  6
#define EVSYS_ID_USER_TC7_EVU                  7
#define EVSYS_ID_USER_ADC_START                8
#define EVSYS_ID_USER_ADC_SYNC                 9
#define EVSYS_ID_USER_AC_SOC_0                 10
#define EVSYS_ID_USER_AC_SOC_1                 11
#define EVSYS_ID_USER_DAC_START                12
#define EVSYS_ID_USER_PTC_STCONV               13
/* ========== Instance parameters for GCLK peripheral ========== */
#define GCLK_GEN_NUM_MSB            7
#define GCLK_GEN_SOURCE_NUM_MSB     7
#define GCLK_NUM                    28
#define GCLK_SOURCE_DFLL48M         7
#define GCLK_SOURCE_GCLKGEN1        2
#define GCLK_SOURCE_GCLKIN          1
#define GCLK_SOURCE_OSCULP32K       3
#define GCLK_SOURCE_OSC8M           6
#define GCLK_SOURCE_OSC32K          4
#define GCLK_SOURCE_XOSC            0
#define GCLK_SOURCE_XOSC32K         5
/* ========== Instance parameters for HMATRIX ========== */
#define HMATRIX_SLAVE_FLASH         0
#define HMATRIX_SLAVE_HPB0          1
#define HMATRIX_SLAVE_HPB1          2
#define HMATRIX_SLAVE_HPB2          3
#define HMATRIX_SLAVE_HRAMC0        4
#define HMATRIX_SLAVE_NUM           5

#define HMATRIX_MASTER_CM0P         0
#define HMATRIX_MASTER_DSU          1
#define HMATRIX_MASTER_NUM          2
/* ========== Instance parameters for NVMCTRL peripheral ========== */
#define NVMCTRL_AUX0_ADDRESS        NVMCTRL_USER_PAGE_ADDRESS + 0x00004000
#define NVMCTRL_AUX1_ADDRESS        NVMCTRL_USER_PAGE_ADDRESS + 0x00006000
#define NVMCTRL_AUX2_ADDRESS        NVMCTRL_USER_PAGE_ADDRESS + 0x00008000
#define NVMCTRL_AUX3_ADDRESS        NVMCTRL_USER_PAGE_ADDRESS + 0x0000A000
#define NVMCTRL_CALIB_OFFSET        0x00000000
#define NVMCTRL_CALIB_SHADOW_ADDRESS NVMCTRL_USER_PAGE_ADDRESS + 0x00020000
#define NVMCTRL_CLK_AHB_ID          4
#define NVMCTRL_FACTORY_WORD_IMPLEMENTED_MASK 0XC0000007FFFFFFFF
#define NVMCTRL_FLASH_SIZE          NVMCTRL_PAGES*NVMCTRL_PAGE_SIZE
#define NVMCTRL_GCLK_ID             12
#define NVMCTRL_LOCKBIT_ADDRESS     NVMCTRL_USER_PAGE_ADDRESS + 0x00002000
#define NVMCTRL_PAGES               4096
#define NVMCTRL_PAGE_HW             NVMCTRL_PAGE_SIZE/2
#define NVMCTRL_PAGE_SIZE           (1<<NVMCTRL_PSZ_BITS)
#define NVMCTRL_PAGE_W              NVMCTRL_PAGE_SIZE/4
#define NVMCTRL_PMSB                3
#define NVMCTRL_PSZ_BITS            6
#define NVMCTRL_ROM_ADDRESS         NVMCTRL_USER_PAGE_ADDRESS + 0x00030000
#define NVMCTRL_ROW_PAGES           NVMCTRL_ROW_SIZE/NVMCTRL_PAGE_SIZE
#define NVMCTRL_ROW_SIZE            NVMCTRL_PAGE_SIZE*4
#define NVMCTRL_TLATCH_ADDRESS      NVMCTRL_USER_PAGE_ADDRESS + 0x00010000
#define NVMCTRL_USER_PAGE_ADDRESS   FLASH_ADDR + NVMCTRL_USER_PAGE_OFFSET
#define NVMCTRL_USER_PAGE_OFFSET    0x00800000
#define NVMCTRL_USER_WORD_IMPLEMENTED_MASK 0XC01FFFFFFFFFFFFF
/* ========== Instance parameters for PAC0 peripheral ========== */
#define PAC0_WPROT_DEFAULT_VAL      0x00000000
/* ========== Instance parameters for PAC1 peripheral ========== */
#define PAC1_WPROT_DEFAULT_VAL      0x00000002
/* ========== Instance parameters for PAC2 peripheral ========== */
#define PAC2_WPROT_DEFAULT_VAL      0x00100000
/* ========== Instance parameters for PM peripheral ========== */
#define PM_CTRL_MCSEL_DFLL48M       2
#define PM_CTRL_MCSEL_GCLK          3
#define PM_CTRL_MCSEL_OSC8M         0
#define PM_CTRL_MCSEL_XOSC          1
#define PM_PM_CLK_AHB_PEVC          0
#define PM_PM_CLK_APB_NUM           2
/* ========== Instance parameters for PORT peripheral ========== */
#define PORT_BITS                   60
#define PORT_DIR_DEFAULT_VAL        { 0x00000000, 0x00000000 }
#define PORT_DIR_IMPLEMENTED        { 0xFF0FFFFF, 0x0FFF3FFF }
#define PORT_DRVSTR_DEFAULT_VAL     { 0x00000000, 0x00000000 }
#define PORT_DRVSTR_IMPLEMENTED     { 0x00000000, 0x00000000 }
#define PORT_GROUPS                 2
#define PORT_INEN_DEFAULT_VAL       { 0x00000000, 0x00000000 }
#define PORT_INEN_IMPLEMENTED       { 0xFF0FFFFF, 0x0FFF3FFF }
#define PORT_ODRAIN_DEFAULT_VAL     { 0x00000000, 0x00000000 }
#define PORT_ODRAIN_IMPLEMENTED     { 0x00000000, 0x00000000 }
#define PORT_OUT_DEFAULT_VAL        { 0x00000000, 0x00000000 }
#define PORT_OUT_IMPLEMENTED        { 0xFF0FFFFF, 0x0FFF3FFF }
#define PORT_PIN_IMPLEMENTED        { 0xFF0FFFFF, 0x0FFF3FFF }
#define PORT_PMUXBIT0_DEFAULT_VAL   { 0x10000000, 0x00000000 }
#define PORT_PMUXBIT0_IMPLEMENTED   { 0xDF0FFFFF, 0x0FFF0FFF }
#define PORT_PMUXBIT1_DEFAULT_VAL   { 0x00000000, 0x00000000 }
#define PORT_PMUXBIT1_IMPLEMENTED   { 0xDF0FFFFF, 0x0FFF3FFF }
#define PORT_PMUXBIT2_DEFAULT_VAL   { 0x10000000, 0x00000000 }
#define PORT_PMUXBIT2_IMPLEMENTED   { 0x9F0FFCFF, 0x0FF330FF }
#define PORT_PMUXBIT3_DEFAULT_VAL   { 0x00000000, 0x00000000 }
#define PORT_PMUXBIT3_IMPLEMENTED   { 0x00000000, 0x00000000 }
#define PORT_PMUXEN_DEFAULT_VAL     { 0x10F00000, 0x0000C000 }
#define PORT_PMUXEN_IMPLEMENTED     { 0xFF0FFFFF, 0x0FFF3FFF }
#define PORT_PULLDOWN_DEFAULT_VAL   { 0x00000000, 0x00000000 }
#define PORT_PULLDOWN_IMPLEMENTED   { 0xFFFFFFFF, 0x0FFFFFFF }
#define PORT_PULLEN_DEFAULT_VAL     { 0x00000000, 0x00000000 }
#define PORT_PULLEN_IMPLEMENTED     { 0xFF0FFFFF, 0x0FFF3FFF }
#define PORT_PULLUP_DEFAULT_VAL     { 0x00000000, 0x00000000 }
#define PORT_PULLUP_IMPLEMENTED     { 0x0FFFFFFF, 0x0FFFFFFF }
#define PORT_SLEWLIM_DEFAULT_VAL    { 0x00000000, 0x00000000 }
#define PORT_SLEWLIM_IMPLEMENTED    { 0x00000000, 0x00000000 }
/* ========== Instance parameters for PTC peripheral ========== */
#define PTC_GCLK_ID                 27
/* ========== Instance parameters for RTC peripheral ========== */
#define RTC_GCLK_ID                 2
#define RTC_NUM_OF_ALARMS           1
#define RTC_NUM_OF_COMP16           2
/* ========== Instance parameters for SERCOM0 peripheral ========== */
#define SERCOM0_GCLK_ID_CORE        13
#define SERCOM0_GCLK_ID_SLOW        12
#define SERCOM0_INT_MSB             3
#define SERCOM0_PMSB                3
/* ========== Instance parameters for SERCOM1 peripheral ========== */
#define SERCOM1_GCLK_ID_CORE        14
#define SERCOM1_GCLK_ID_SLOW        12
#define SERCOM1_INT_MSB             3
#define SERCOM1_PMSB                3
/* ========== Instance parameters for SERCOM2 peripheral ========== */
#define SERCOM2_GCLK_ID_CORE        15
#define SERCOM2_GCLK_ID_SLOW        12
#define SERCOM2_INT_MSB             3
#define SERCOM2_PMSB                3
/* ========== Instance parameters for SERCOM3 peripheral ========== */
#define SERCOM3_GCLK_ID_CORE        16
#define SERCOM3_GCLK_ID_SLOW        12
#define SERCOM3_INT_MSB             3
#define SERCOM3_PMSB                3
/* ========== Instance parameters for SERCOM4 peripheral ========== */
#define SERCOM4_GCLK_ID_CORE        17
#define SERCOM4_GCLK_ID_SLOW        12
#define SERCOM4_INT_MSB             3
#define SERCOM4_PMSB                3
/* ========== Instance parameters for SERCOM5 peripheral ========== */
#define SERCOM5_GCLK_ID_CORE        18
#define SERCOM5_GCLK_ID_SLOW        12
#define SERCOM5_INT_MSB             3
#define SERCOM5_PMSB                3
/* ========== Instance parameters for SYSCTRL peripheral ========== */
#define SYSCTRL_BGAP_CALIB_MSB      11
#define SYSCTRL_BOD12_CALIB_MSB     4
#define SYSCTRL_BOD33_CALIB_MSB     5
#define SYSCTRL_DFLL48M_COARSE_MSB  4
#define SYSCTRL_DFLL48M_FINE_MSB    7
#define SYSCTRL_DFLL48M_TESTEN_MSB  1
#define SYSCTRL_GCLK_ID_DFLL48      0
#define SYSCTRL_OSC32K_COARSE_CALIB_MSB 6
#define SYSCTRL_POR33_ENTEST_MSB    1
#define SYSCTRL_ULPVREF_DIVLEV_MSB  3
#define SYSCTRL_ULPVREG_FORCEGAIN_MSB 1
#define SYSCTRL_ULPVREG_RAMREFSEL_MSB 2
#define SYSCTRL_VREF_CONTROL_MSB    48
#define SYSCTRL_VREF_STATUS_MSB     7
#define SYSCTRL_VREG_LEVEL_MSB      2
#define SYSCTRL_BOD12_VERSION       0x100
#define SYSCTRL_BOD33_VERSION       0x100
#define SYSCTRL_DFLL48M_VERSION     0x100
#define SYSCTRL_GCLK_VERSION        0x100
#define SYSCTRL_OSCULP32K_VERSION   0x100
#define SYSCTRL_OSC8M_VERSION       0x100
#define SYSCTRL_OSC32K_VERSION      0x100
#define SYSCTRL_VREF_VERSION        0x100
#define SYSCTRL_VREG_VERSION        0x100
#define SYSCTRL_XOSC_VERSION        0x100
#define SYSCTRL_XOSC32K_VERSION     0x100
/* ========== Instance parameters for TC0 peripheral ========== */
#define TC0_GCLK_ID                 19
#define TC0_TC_CC8_NUM              2
#define TC0_TC_CC16_NUM             2
#define TC0_TC_CC32_NUM             2
#define TC0_TC_OW_NUM               2
/* ========== Instance parameters for TC1 peripheral ========== */
#define TC1_GCLK_ID                 19
#define TC1_TC_CC8_NUM              2
#define TC1_TC_CC16_NUM             2
#define TC1_TC_CC32_NUM             2
#define TC1_TC_OW_NUM               2
/* ========== Instance parameters for TC2 peripheral ========== */
#define TC2_GCLK_ID                 20
#define TC2_TC_CC8_NUM              2
#define TC2_TC_CC16_NUM             2
#define TC2_TC_CC32_NUM             2
#define TC2_TC_OW_NUM               2
/* ========== Instance parameters for TC3 peripheral ========== */
#define TC3_GCLK_ID                 20
#define TC3_TC_CC8_NUM              2
#define TC3_TC_CC16_NUM             2
#define TC3_TC_CC32_NUM             2
#define TC3_TC_OW_NUM               2
/* ========== Instance parameters for TC4 peripheral ========== */
#define TC4_GCLK_ID                 21
#define TC4_TC_CC8_NUM              2
#define TC4_TC_CC16_NUM             2
#define TC4_TC_CC32_NUM             2
#define TC4_TC_OW_NUM               2
/* ========== Instance parameters for TC5 peripheral ========== */
#define TC5_GCLK_ID                 21
#define TC5_TC_CC8_NUM              2
#define TC5_TC_CC16_NUM             2
#define TC5_TC_CC32_NUM             2
#define TC5_TC_OW_NUM               2
/* ========== Instance parameters for TC6 peripheral ========== */
#define TC6_GCLK_ID                 22
#define TC6_TC_CC8_NUM              2
#define TC6_TC_CC16_NUM             2
#define TC6_TC_CC32_NUM             2
#define TC6_TC_OW_NUM               2
/* ========== Instance parameters for TC7 peripheral ========== */
#define TC7_GCLK_ID                 22
#define TC7_TC_CC8_NUM              2
#define TC7_TC_CC16_NUM             2
#define TC7_TC_CC32_NUM             2
#define TC7_TC_OW_NUM               2
/* ========== Instance parameters for WDT peripheral ========== */
#define WDT_GCLK_ID                 1

/*@}*/

/* ************************************************************************** */
/**  PORT DEFINITIONS FOR SAM0D4_REVA */
/* ************************************************************************** */
/** \addtogroup SAM0D4_REVA_port PORT Definitions */
/*@{*/

#define PIN_PA00                           0  /**< \brief Pin Number for PA00 */
#define PORT_PA00                  (1u <<  0) /**< \brief PORT Mask  for PA00 */
#define PIN_PA01                           1  /**< \brief Pin Number for PA01 */
#define PORT_PA01                  (1u <<  1) /**< \brief PORT Mask  for PA01 */
#define PIN_PA02                           2  /**< \brief Pin Number for PA02 */
#define PORT_PA02                  (1u <<  2) /**< \brief PORT Mask  for PA02 */
#define PIN_PA03                           3  /**< \brief Pin Number for PA03 */
#define PORT_PA03                  (1u <<  3) /**< \brief PORT Mask  for PA03 */
#define PIN_PA04                           4  /**< \brief Pin Number for PA04 */
#define PORT_PA04                  (1u <<  4) /**< \brief PORT Mask  for PA04 */
#define PIN_PA05                           5  /**< \brief Pin Number for PA05 */
#define PORT_PA05                  (1u <<  5) /**< \brief PORT Mask  for PA05 */
#define PIN_PA06                           6  /**< \brief Pin Number for PA06 */
#define PORT_PA06                  (1u <<  6) /**< \brief PORT Mask  for PA06 */
#define PIN_PA07                           7  /**< \brief Pin Number for PA07 */
#define PORT_PA07                  (1u <<  7) /**< \brief PORT Mask  for PA07 */
#define PIN_PA08                           8  /**< \brief Pin Number for PA08 */
#define PORT_PA08                  (1u <<  8) /**< \brief PORT Mask  for PA08 */
#define PIN_PA09                           9  /**< \brief Pin Number for PA09 */
#define PORT_PA09                  (1u <<  9) /**< \brief PORT Mask  for PA09 */
#define PIN_PA10                          10  /**< \brief Pin Number for PA10 */
#define PORT_PA10                  (1u << 10) /**< \brief PORT Mask  for PA10 */
#define PIN_PA11                          11  /**< \brief Pin Number for PA11 */
#define PORT_PA11                  (1u << 11) /**< \brief PORT Mask  for PA11 */
#define PIN_PA12                          12  /**< \brief Pin Number for PA12 */
#define PORT_PA12                  (1u << 12) /**< \brief PORT Mask  for PA12 */
#define PIN_PA13                          13  /**< \brief Pin Number for PA13 */
#define PORT_PA13                  (1u << 13) /**< \brief PORT Mask  for PA13 */
#define PIN_PA14                          14  /**< \brief Pin Number for PA14 */
#define PORT_PA14                  (1u << 14) /**< \brief PORT Mask  for PA14 */
#define PIN_PA15                          15  /**< \brief Pin Number for PA15 */
#define PORT_PA15                  (1u << 15) /**< \brief PORT Mask  for PA15 */
#define PIN_PA16                          16  /**< \brief Pin Number for PA16 */
#define PORT_PA16                  (1u << 16) /**< \brief PORT Mask  for PA16 */
#define PIN_PA17                          17  /**< \brief Pin Number for PA17 */
#define PORT_PA17                  (1u << 17) /**< \brief PORT Mask  for PA17 */
#define PIN_PA18                          18  /**< \brief Pin Number for PA18 */
#define PORT_PA18                  (1u << 18) /**< \brief PORT Mask  for PA18 */
#define PIN_PA19                          19  /**< \brief Pin Number for PA19 */
#define PORT_PA19                  (1u << 19) /**< \brief PORT Mask  for PA19 */
#define PIN_PA24                          24  /**< \brief Pin Number for PA24 */
#define PORT_PA24                  (1u << 24) /**< \brief PORT Mask  for PA24 */
#define PIN_PA25                          25  /**< \brief Pin Number for PA25 */
#define PORT_PA25                  (1u << 25) /**< \brief PORT Mask  for PA25 */
#define PIN_PA26                          26  /**< \brief Pin Number for PA26 */
#define PORT_PA26                  (1u << 26) /**< \brief PORT Mask  for PA26 */
#define PIN_PA27                          27  /**< \brief Pin Number for PA27 */
#define PORT_PA27                  (1u << 27) /**< \brief PORT Mask  for PA27 */
#define PIN_PA28                          28  /**< \brief Pin Number for PA28 */
#define PORT_PA28                  (1u << 28) /**< \brief PORT Mask  for PA28 */
#define PIN_PA29                          29  /**< \brief Pin Number for PA29 */
#define PORT_PA29                  (1u << 29) /**< \brief PORT Mask  for PA29 */
#define PIN_PA30                          30  /**< \brief Pin Number for PA30 */
#define PORT_PA30                  (1u << 30) /**< \brief PORT Mask  for PA30 */
#define PIN_PA31                          31  /**< \brief Pin Number for PA31 */
#define PORT_PA31                  (1u << 31) /**< \brief PORT Mask  for PA31 */
#define PIN_PB00                          32  /**< \brief Pin Number for PB00 */
#define PORT_PB00                  (1u <<  0) /**< \brief PORT Mask  for PB00 */
#define PIN_PB01                          33  /**< \brief Pin Number for PB01 */
#define PORT_PB01                  (1u <<  1) /**< \brief PORT Mask  for PB01 */
#define PIN_PB02                          34  /**< \brief Pin Number for PB02 */
#define PORT_PB02                  (1u <<  2) /**< \brief PORT Mask  for PB02 */
#define PIN_PB03                          35  /**< \brief Pin Number for PB03 */
#define PORT_PB03                  (1u <<  3) /**< \brief PORT Mask  for PB03 */
#define PIN_PB04                          36  /**< \brief Pin Number for PB04 */
#define PORT_PB04                  (1u <<  4) /**< \brief PORT Mask  for PB04 */
#define PIN_PB05                          37  /**< \brief Pin Number for PB05 */
#define PORT_PB05                  (1u <<  5) /**< \brief PORT Mask  for PB05 */
#define PIN_PB06                          38  /**< \brief Pin Number for PB06 */
#define PORT_PB06                  (1u <<  6) /**< \brief PORT Mask  for PB06 */
#define PIN_PB07                          39  /**< \brief Pin Number for PB07 */
#define PORT_PB07                  (1u <<  7) /**< \brief PORT Mask  for PB07 */
#define PIN_PB08                          40  /**< \brief Pin Number for PB08 */
#define PORT_PB08                  (1u <<  8) /**< \brief PORT Mask  for PB08 */
#define PIN_PB09                          41  /**< \brief Pin Number for PB09 */
#define PORT_PB09                  (1u <<  9) /**< \brief PORT Mask  for PB09 */
#define PIN_PB10                          42  /**< \brief Pin Number for PB10 */
#define PORT_PB10                  (1u << 10) /**< \brief PORT Mask  for PB10 */
#define PIN_PB11                          43  /**< \brief Pin Number for PB11 */
#define PORT_PB11                  (1u << 11) /**< \brief PORT Mask  for PB11 */
#define PIN_PB12                          44  /**< \brief Pin Number for PB12 */
#define PORT_PB12                  (1u << 12) /**< \brief PORT Mask  for PB12 */
#define PIN_PB13                          45  /**< \brief Pin Number for PB13 */
#define PORT_PB13                  (1u << 13) /**< \brief PORT Mask  for PB13 */
#define PIN_PB16                          48  /**< \brief Pin Number for PB16 */
#define PORT_PB16                  (1u << 16) /**< \brief PORT Mask  for PB16 */
#define PIN_PB17                          49  /**< \brief Pin Number for PB17 */
#define PORT_PB17                  (1u << 17) /**< \brief PORT Mask  for PB17 */
#define PIN_PB18                          50  /**< \brief Pin Number for PB18 */
#define PORT_PB18                  (1u << 18) /**< \brief PORT Mask  for PB18 */
#define PIN_PB19                          51  /**< \brief Pin Number for PB19 */
#define PORT_PB19                  (1u << 19) /**< \brief PORT Mask  for PB19 */
#define PIN_PB20                          52  /**< \brief Pin Number for PB20 */
#define PORT_PB20                  (1u << 20) /**< \brief PORT Mask  for PB20 */
#define PIN_PB21                          53  /**< \brief Pin Number for PB21 */
#define PORT_PB21                  (1u << 21) /**< \brief PORT Mask  for PB21 */
#define PIN_PB22                          54  /**< \brief Pin Number for PB22 */
#define PORT_PB22                  (1u << 22) /**< \brief PORT Mask  for PB22 */
#define PIN_PB23                          55  /**< \brief Pin Number for PB23 */
#define PORT_PB23                  (1u << 23) /**< \brief PORT Mask  for PB23 */
#define PIN_PB24                          56  /**< \brief Pin Number for PB24 */
#define PORT_PB24                  (1u << 24) /**< \brief PORT Mask  for PB24 */
#define PIN_PB25                          57  /**< \brief Pin Number for PB25 */
#define PORT_PB25                  (1u << 25) /**< \brief PORT Mask  for PB25 */
#define PIN_PB26                          58  /**< \brief Pin Number for PB26 */
#define PORT_PB26                  (1u << 26) /**< \brief PORT Mask  for PB26 */
#define PIN_PB27                          59  /**< \brief Pin Number for PB27 */
#define PORT_PB27                  (1u << 27) /**< \brief PORT Mask  for PB27 */
/* ========== PORT definition for AC peripheral ========== */
#define PIN_PA04H_AC_AIN0                  4  /**< \brief AC signal: AIN0 on PA04 mux H */
#define MUX_PA04H_AC_AIN0                  7
#define PORT_PA04H_AC_AIN0         (1u <<  4)
#define PIN_PA05H_AC_AIN1                  5  /**< \brief AC signal: AIN1 on PA05 mux H */
#define MUX_PA05H_AC_AIN1                  7
#define PORT_PA05H_AC_AIN1         (1u <<  5)
#define PIN_PA06H_AC_AIN2                  6  /**< \brief AC signal: AIN2 on PA06 mux H */
#define MUX_PA06H_AC_AIN2                  7
#define PORT_PA06H_AC_AIN2         (1u <<  6)
#define PIN_PA07H_AC_AIN3                  7  /**< \brief AC signal: AIN3 on PA07 mux H */
#define MUX_PA07H_AC_AIN3                  7
#define PORT_PA07H_AC_AIN3         (1u <<  7)
#define PIN_PA10G_AC_CMP0                 10  /**< \brief AC signal: CMP0 on PA10 mux G */
#define MUX_PA10G_AC_CMP0                  6
#define PORT_PA10G_AC_CMP0         (1u << 10)
#define PIN_PA12G_AC_CMP0                 12  /**< \brief AC signal: CMP0 on PA12 mux G */
#define MUX_PA12G_AC_CMP0                  6
#define PORT_PA12G_AC_CMP0         (1u << 12)
#define PIN_PA11G_AC_CMP1                 11  /**< \brief AC signal: CMP1 on PA11 mux G */
#define MUX_PA11G_AC_CMP1                  6
#define PORT_PA11G_AC_CMP1         (1u << 11)
#define PIN_PA13G_AC_CMP1                 13  /**< \brief AC signal: CMP1 on PA13 mux G */
#define MUX_PA13G_AC_CMP1                  6
#define PORT_PA13G_AC_CMP1         (1u << 13)
/* ========== PORT definition for ADC peripheral ========== */
#define PIN_PA00H_ADC_AIN0                 0  /**< \brief ADC signal: AIN0 on PA00 mux H */
#define MUX_PA00H_ADC_AIN0                 7
#define PORT_PA00H_ADC_AIN0        (1u <<  0)
#define PIN_PA01H_ADC_AIN1                 1  /**< \brief ADC signal: AIN1 on PA01 mux H */
#define MUX_PA01H_ADC_AIN1                 7
#define PORT_PA01H_ADC_AIN1        (1u <<  1)
#define PIN_PA02H_ADC_AIN2                 2  /**< \brief ADC signal: AIN2 on PA02 mux H */
#define MUX_PA02H_ADC_AIN2                 7
#define PORT_PA02H_ADC_AIN2        (1u <<  2)
#define PIN_PA03H_ADC_AIN3                 3  /**< \brief ADC signal: AIN3 on PA03 mux H */
#define MUX_PA03H_ADC_AIN3                 7
#define PORT_PA03H_ADC_AIN3        (1u <<  3)
#define PIN_PA04H_ADC_AIN4                 4  /**< \brief ADC signal: AIN4 on PA04 mux H */
#define MUX_PA04H_ADC_AIN4                 7
#define PORT_PA04H_ADC_AIN4        (1u <<  4)
#define PIN_PA05H_ADC_AIN5                 5  /**< \brief ADC signal: AIN5 on PA05 mux H */
#define MUX_PA05H_ADC_AIN5                 7
#define PORT_PA05H_ADC_AIN5        (1u <<  5)
#define PIN_PA06H_ADC_AIN6                 6  /**< \brief ADC signal: AIN6 on PA06 mux H */
#define MUX_PA06H_ADC_AIN6                 7
#define PORT_PA06H_ADC_AIN6        (1u <<  6)
#define PIN_PA07H_ADC_AIN7                 7  /**< \brief ADC signal: AIN7 on PA07 mux H */
#define MUX_PA07H_ADC_AIN7                 7
#define PORT_PA07H_ADC_AIN7        (1u <<  7)
#define PIN_PB00H_ADC_AIN8                32  /**< \brief ADC signal: AIN8 on PB00 mux H */
#define MUX_PB00H_ADC_AIN8                 7
#define PORT_PB00H_ADC_AIN8        (1u <<  0)
#define PIN_PB01H_ADC_AIN9                33  /**< \brief ADC signal: AIN9 on PB01 mux H */
#define MUX_PB01H_ADC_AIN9                 7
#define PORT_PB01H_ADC_AIN9        (1u <<  1)
#define PIN_PB02H_ADC_AIN10               34  /**< \brief ADC signal: AIN10 on PB02 mux H */
#define MUX_PB02H_ADC_AIN10                7
#define PORT_PB02H_ADC_AIN10       (1u <<  2)
#define PIN_PB03H_ADC_AIN11               35  /**< \brief ADC signal: AIN11 on PB03 mux H */
#define MUX_PB03H_ADC_AIN11                7
#define PORT_PB03H_ADC_AIN11       (1u <<  3)
#define PIN_PB04H_ADC_AIN12               36  /**< \brief ADC signal: AIN12 on PB04 mux H */
#define MUX_PB04H_ADC_AIN12                7
#define PORT_PB04H_ADC_AIN12       (1u <<  4)
#define PIN_PB05H_ADC_AIN13               37  /**< \brief ADC signal: AIN13 on PB05 mux H */
#define MUX_PB05H_ADC_AIN13                7
#define PORT_PB05H_ADC_AIN13       (1u <<  5)
#define PIN_PB06H_ADC_AIN14               38  /**< \brief ADC signal: AIN14 on PB06 mux H */
#define MUX_PB06H_ADC_AIN14                7
#define PORT_PB06H_ADC_AIN14       (1u <<  6)
#define PIN_PB07H_ADC_AIN15               39  /**< \brief ADC signal: AIN15 on PB07 mux H */
#define MUX_PB07H_ADC_AIN15                7
#define PORT_PB07H_ADC_AIN15       (1u <<  7)
#define PIN_PA02H_ADC_VREFP                2  /**< \brief ADC signal: VREFP on PA02 mux H */
#define MUX_PA02H_ADC_VREFP                7
#define PORT_PA02H_ADC_VREFP       (1u <<  2)
/* ========== PORT definition for CORE peripheral ========== */
#define PIN_PA28F_CORE_SWCLK              28  /**< \brief CORE signal: SWCLK on PA28 mux F */
#define MUX_PA28F_CORE_SWCLK               5
#define PORT_PA28F_CORE_SWCLK      (1u << 28)
/* ========== PORT definition for DAC peripheral ========== */
#define PIN_PA00H_DAC_VOUT                 0  /**< \brief DAC signal: VOUT on PA00 mux H */
#define MUX_PA00H_DAC_VOUT                 7
#define PORT_PA00H_DAC_VOUT        (1u <<  0)
#define PIN_PA01H_DAC_VREFP                1  /**< \brief DAC signal: VREFP on PA01 mux H */
#define MUX_PA01H_DAC_VREFP                7
#define PORT_PA01H_DAC_VREFP       (1u <<  1)
/* ========== PORT definition for EIC peripheral ========== */
#define PIN_PA00A_EIC_EXTINT0              0  /**< \brief EIC signal: EXTINT0 on PA00 mux A */
#define MUX_PA00A_EIC_EXTINT0              0
#define PORT_PA00A_EIC_EXTINT0     (1u <<  0)
#define PIN_PB00A_EIC_EXTINT0             32  /**< \brief EIC signal: EXTINT0 on PB00 mux A */
#define MUX_PB00A_EIC_EXTINT0              0
#define PORT_PB00A_EIC_EXTINT0     (1u <<  0)
#define PIN_PB20A_EIC_EXTINT0             52  /**< \brief EIC signal: EXTINT0 on PB20 mux A */
#define MUX_PB20A_EIC_EXTINT0              0
#define PORT_PB20A_EIC_EXTINT0     (1u << 20)
#define PIN_PA01A_EIC_EXTINT1              1  /**< \brief EIC signal: EXTINT1 on PA01 mux A */
#define MUX_PA01A_EIC_EXTINT1              0
#define PORT_PA01A_EIC_EXTINT1     (1u <<  1)
#define PIN_PB01A_EIC_EXTINT1             33  /**< \brief EIC signal: EXTINT1 on PB01 mux A */
#define MUX_PB01A_EIC_EXTINT1              0
#define PORT_PB01A_EIC_EXTINT1     (1u <<  1)
#define PIN_PB21A_EIC_EXTINT1             53  /**< \brief EIC signal: EXTINT1 on PB21 mux A */
#define MUX_PB21A_EIC_EXTINT1              0
#define PORT_PB21A_EIC_EXTINT1     (1u << 21)
#define PIN_PA04A_EIC_EXTINT2              4  /**< \brief EIC signal: EXTINT2 on PA04 mux A */
#define MUX_PA04A_EIC_EXTINT2              0
#define PORT_PA04A_EIC_EXTINT2     (1u <<  4)
#define PIN_PB04A_EIC_EXTINT2             36  /**< \brief EIC signal: EXTINT2 on PB04 mux A */
#define MUX_PB04A_EIC_EXTINT2              0
#define PORT_PB04A_EIC_EXTINT2     (1u <<  4)
#define PIN_PB22A_EIC_EXTINT2             54  /**< \brief EIC signal: EXTINT2 on PB22 mux A */
#define MUX_PB22A_EIC_EXTINT2              0
#define PORT_PB22A_EIC_EXTINT2     (1u << 22)
#define PIN_PA05A_EIC_EXTINT3              5  /**< \brief EIC signal: EXTINT3 on PA05 mux A */
#define MUX_PA05A_EIC_EXTINT3              0
#define PORT_PA05A_EIC_EXTINT3     (1u <<  5)
#define PIN_PB05A_EIC_EXTINT3             37  /**< \brief EIC signal: EXTINT3 on PB05 mux A */
#define MUX_PB05A_EIC_EXTINT3              0
#define PORT_PB05A_EIC_EXTINT3     (1u <<  5)
#define PIN_PB23A_EIC_EXTINT3             55  /**< \brief EIC signal: EXTINT3 on PB23 mux A */
#define MUX_PB23A_EIC_EXTINT3              0
#define PORT_PB23A_EIC_EXTINT3     (1u << 23)
#define PIN_PA08A_EIC_EXTINT4              8  /**< \brief EIC signal: EXTINT4 on PA08 mux A */
#define MUX_PA08A_EIC_EXTINT4              0
#define PORT_PA08A_EIC_EXTINT4     (1u <<  8)
#define PIN_PA28A_EIC_EXTINT4             28  /**< \brief EIC signal: EXTINT4 on PA28 mux A */
#define MUX_PA28A_EIC_EXTINT4              0
#define PORT_PA28A_EIC_EXTINT4     (1u << 28)
#define PIN_PB08A_EIC_EXTINT4             40  /**< \brief EIC signal: EXTINT4 on PB08 mux A */
#define MUX_PB08A_EIC_EXTINT4              0
#define PORT_PB08A_EIC_EXTINT4     (1u <<  8)
#define PIN_PA09A_EIC_EXTINT5              9  /**< \brief EIC signal: EXTINT5 on PA09 mux A */
#define MUX_PA09A_EIC_EXTINT5              0
#define PORT_PA09A_EIC_EXTINT5     (1u <<  9)
#define PIN_PA29A_EIC_EXTINT5             29  /**< \brief EIC signal: EXTINT5 on PA29 mux A */
#define MUX_PA29A_EIC_EXTINT5              0
#define PORT_PA29A_EIC_EXTINT5     (1u << 29)
#define PIN_PB09A_EIC_EXTINT5             41  /**< \brief EIC signal: EXTINT5 on PB09 mux A */
#define MUX_PB09A_EIC_EXTINT5              0
#define PORT_PB09A_EIC_EXTINT5     (1u <<  9)
#define PIN_PA10A_EIC_EXTINT6             10  /**< \brief EIC signal: EXTINT6 on PA10 mux A */
#define MUX_PA10A_EIC_EXTINT6              0
#define PORT_PA10A_EIC_EXTINT6     (1u << 10)
#define PIN_PB10A_EIC_EXTINT6             42  /**< \brief EIC signal: EXTINT6 on PB10 mux A */
#define MUX_PB10A_EIC_EXTINT6              0
#define PORT_PB10A_EIC_EXTINT6     (1u << 10)
#define PIN_PA11A_EIC_EXTINT7             11  /**< \brief EIC signal: EXTINT7 on PA11 mux A */
#define MUX_PA11A_EIC_EXTINT7              0
#define PORT_PA11A_EIC_EXTINT7     (1u << 11)
#define PIN_PA31A_EIC_EXTINT7             31  /**< \brief EIC signal: EXTINT7 on PA31 mux A */
#define MUX_PA31A_EIC_EXTINT7              0
#define PORT_PA31A_EIC_EXTINT7     (1u << 31)
#define PIN_PB11A_EIC_EXTINT7             43  /**< \brief EIC signal: EXTINT7 on PB11 mux A */
#define MUX_PB11A_EIC_EXTINT7              0
#define PORT_PB11A_EIC_EXTINT7     (1u << 11)
#define PIN_PA02A_EIC_EXTINT8              2  /**< \brief EIC signal: EXTINT8 on PA02 mux A */
#define MUX_PA02A_EIC_EXTINT8              0
#define PORT_PA02A_EIC_EXTINT8     (1u <<  2)
#define PIN_PA16A_EIC_EXTINT8             16  /**< \brief EIC signal: EXTINT8 on PA16 mux A */
#define MUX_PA16A_EIC_EXTINT8              0
#define PORT_PA16A_EIC_EXTINT8     (1u << 16)
#define PIN_PB02A_EIC_EXTINT8             34  /**< \brief EIC signal: EXTINT8 on PB02 mux A */
#define MUX_PB02A_EIC_EXTINT8              0
#define PORT_PB02A_EIC_EXTINT8     (1u <<  2)
#define PIN_PB16A_EIC_EXTINT8             48  /**< \brief EIC signal: EXTINT8 on PB16 mux A */
#define MUX_PB16A_EIC_EXTINT8              0
#define PORT_PB16A_EIC_EXTINT8     (1u << 16)
#define PIN_PA03A_EIC_EXTINT9              3  /**< \brief EIC signal: EXTINT9 on PA03 mux A */
#define MUX_PA03A_EIC_EXTINT9              0
#define PORT_PA03A_EIC_EXTINT9     (1u <<  3)
#define PIN_PA17A_EIC_EXTINT9             17  /**< \brief EIC signal: EXTINT9 on PA17 mux A */
#define MUX_PA17A_EIC_EXTINT9              0
#define PORT_PA17A_EIC_EXTINT9     (1u << 17)
#define PIN_PB03A_EIC_EXTINT9             35  /**< \brief EIC signal: EXTINT9 on PB03 mux A */
#define MUX_PB03A_EIC_EXTINT9              0
#define PORT_PB03A_EIC_EXTINT9     (1u <<  3)
#define PIN_PB17A_EIC_EXTINT9             49  /**< \brief EIC signal: EXTINT9 on PB17 mux A */
#define MUX_PB17A_EIC_EXTINT9              0
#define PORT_PB17A_EIC_EXTINT9     (1u << 17)
#define PIN_PA06A_EIC_EXTINT10             6  /**< \brief EIC signal: EXTINT10 on PA06 mux A */
#define MUX_PA06A_EIC_EXTINT10             0
#define PORT_PA06A_EIC_EXTINT10    (1u <<  6)
#define PIN_PA18A_EIC_EXTINT10            18  /**< \brief EIC signal: EXTINT10 on PA18 mux A */
#define MUX_PA18A_EIC_EXTINT10             0
#define PORT_PA18A_EIC_EXTINT10    (1u << 18)
#define PIN_PB06A_EIC_EXTINT10            38  /**< \brief EIC signal: EXTINT10 on PB06 mux A */
#define MUX_PB06A_EIC_EXTINT10             0
#define PORT_PB06A_EIC_EXTINT10    (1u <<  6)
#define PIN_PB18A_EIC_EXTINT10            50  /**< \brief EIC signal: EXTINT10 on PB18 mux A */
#define MUX_PB18A_EIC_EXTINT10             0
#define PORT_PB18A_EIC_EXTINT10    (1u << 18)
#define PIN_PA07A_EIC_EXTINT11             7  /**< \brief EIC signal: EXTINT11 on PA07 mux A */
#define MUX_PA07A_EIC_EXTINT11             0
#define PORT_PA07A_EIC_EXTINT11    (1u <<  7)
#define PIN_PA19A_EIC_EXTINT11            19  /**< \brief EIC signal: EXTINT11 on PA19 mux A */
#define MUX_PA19A_EIC_EXTINT11             0
#define PORT_PA19A_EIC_EXTINT11    (1u << 19)
#define PIN_PB07A_EIC_EXTINT11            39  /**< \brief EIC signal: EXTINT11 on PB07 mux A */
#define MUX_PB07A_EIC_EXTINT11             0
#define PORT_PB07A_EIC_EXTINT11    (1u <<  7)
#define PIN_PB19A_EIC_EXTINT11            51  /**< \brief EIC signal: EXTINT11 on PB19 mux A */
#define MUX_PB19A_EIC_EXTINT11             0
#define PORT_PB19A_EIC_EXTINT11    (1u << 19)
#define PIN_PA12A_EIC_EXTINT12            12  /**< \brief EIC signal: EXTINT12 on PA12 mux A */
#define MUX_PA12A_EIC_EXTINT12             0
#define PORT_PA12A_EIC_EXTINT12    (1u << 12)
#define PIN_PA24A_EIC_EXTINT12            24  /**< \brief EIC signal: EXTINT12 on PA24 mux A */
#define MUX_PA24A_EIC_EXTINT12             0
#define PORT_PA24A_EIC_EXTINT12    (1u << 24)
#define PIN_PB12A_EIC_EXTINT12            44  /**< \brief EIC signal: EXTINT12 on PB12 mux A */
#define MUX_PB12A_EIC_EXTINT12             0
#define PORT_PB12A_EIC_EXTINT12    (1u << 12)
#define PIN_PB24A_EIC_EXTINT12            56  /**< \brief EIC signal: EXTINT12 on PB24 mux A */
#define MUX_PB24A_EIC_EXTINT12             0
#define PORT_PB24A_EIC_EXTINT12    (1u << 24)
#define PIN_PA13A_EIC_EXTINT13            13  /**< \brief EIC signal: EXTINT13 on PA13 mux A */
#define MUX_PA13A_EIC_EXTINT13             0
#define PORT_PA13A_EIC_EXTINT13    (1u << 13)
#define PIN_PA25A_EIC_EXTINT13            25  /**< \brief EIC signal: EXTINT13 on PA25 mux A */
#define MUX_PA25A_EIC_EXTINT13             0
#define PORT_PA25A_EIC_EXTINT13    (1u << 25)
#define PIN_PB13A_EIC_EXTINT13            45  /**< \brief EIC signal: EXTINT13 on PB13 mux A */
#define MUX_PB13A_EIC_EXTINT13             0
#define PORT_PB13A_EIC_EXTINT13    (1u << 13)
#define PIN_PB25A_EIC_EXTINT13            57  /**< \brief EIC signal: EXTINT13 on PB25 mux A */
#define MUX_PB25A_EIC_EXTINT13             0
#define PORT_PB25A_EIC_EXTINT13    (1u << 25)
#define PIN_PA14A_EIC_EXTINT14            14  /**< \brief EIC signal: EXTINT14 on PA14 mux A */
#define MUX_PA14A_EIC_EXTINT14             0
#define PORT_PA14A_EIC_EXTINT14    (1u << 14)
#define PIN_PA26A_EIC_EXTINT14            26  /**< \brief EIC signal: EXTINT14 on PA26 mux A */
#define MUX_PA26A_EIC_EXTINT14             0
#define PORT_PA26A_EIC_EXTINT14    (1u << 26)
#define PIN_PB26A_EIC_EXTINT14            58  /**< \brief EIC signal: EXTINT14 on PB26 mux A */
#define MUX_PB26A_EIC_EXTINT14             0
#define PORT_PB26A_EIC_EXTINT14    (1u << 26)
#define PIN_PA15A_EIC_EXTINT15            15  /**< \brief EIC signal: EXTINT15 on PA15 mux A */
#define MUX_PA15A_EIC_EXTINT15             0
#define PORT_PA15A_EIC_EXTINT15    (1u << 15)
#define PIN_PA27A_EIC_EXTINT15            27  /**< \brief EIC signal: EXTINT15 on PA27 mux A */
#define MUX_PA27A_EIC_EXTINT15             0
#define PORT_PA27A_EIC_EXTINT15    (1u << 27)
#define PIN_PB27A_EIC_EXTINT15            59  /**< \brief EIC signal: EXTINT15 on PB27 mux A */
#define MUX_PB27A_EIC_EXTINT15             0
#define PORT_PB27A_EIC_EXTINT15    (1u << 27)
#define PIN_PA30A_EIC_NMI                 30  /**< \brief EIC signal: NMI on PA30 mux A */
#define MUX_PA30A_EIC_NMI                  0
#define PORT_PA30A_EIC_NMI         (1u << 30)
/* ========== PORT definition for GCLK peripheral ========== */
#define PIN_PA16G_GCLK_IO0                16  /**< \brief GCLK signal: IO0 on PA16 mux G */
#define MUX_PA16G_GCLK_IO0                 6
#define PORT_PA16G_GCLK_IO0        (1u << 16)
#define PIN_PA28G_GCLK_IO0                28  /**< \brief GCLK signal: IO0 on PA28 mux G */
#define MUX_PA28G_GCLK_IO0                 6
#define PORT_PA28G_GCLK_IO0        (1u << 28)
#define PIN_PA31G_GCLK_IO0                31  /**< \brief GCLK signal: IO0 on PA31 mux G */
#define MUX_PA31G_GCLK_IO0                 6
#define PORT_PA31G_GCLK_IO0        (1u << 31)
#define PIN_PB20G_GCLK_IO0                52  /**< \brief GCLK signal: IO0 on PB20 mux G */
#define MUX_PB20G_GCLK_IO0                 6
#define PORT_PB20G_GCLK_IO0        (1u << 20)
#define PIN_PA17G_GCLK_IO1                17  /**< \brief GCLK signal: IO1 on PA17 mux G */
#define MUX_PA17G_GCLK_IO1                 6
#define PORT_PA17G_GCLK_IO1        (1u << 17)
#define PIN_PB21G_GCLK_IO1                53  /**< \brief GCLK signal: IO1 on PB21 mux G */
#define MUX_PB21G_GCLK_IO1                 6
#define PORT_PB21G_GCLK_IO1        (1u << 21)
#define PIN_PA18G_GCLK_IO2                18  /**< \brief GCLK signal: IO2 on PA18 mux G */
#define MUX_PA18G_GCLK_IO2                 6
#define PORT_PA18G_GCLK_IO2        (1u << 18)
#define PIN_PB22G_GCLK_IO2                54  /**< \brief GCLK signal: IO2 on PB22 mux G */
#define MUX_PB22G_GCLK_IO2                 6
#define PORT_PB22G_GCLK_IO2        (1u << 22)
#define PIN_PA19G_GCLK_IO3                19  /**< \brief GCLK signal: IO3 on PA19 mux G */
#define MUX_PA19G_GCLK_IO3                 6
#define PORT_PA19G_GCLK_IO3        (1u << 19)
#define PIN_PB23G_GCLK_IO3                55  /**< \brief GCLK signal: IO3 on PB23 mux G */
#define MUX_PB23G_GCLK_IO3                 6
#define PORT_PB23G_GCLK_IO3        (1u << 23)
#define PIN_PA24G_GCLK_IO4                24  /**< \brief GCLK signal: IO4 on PA24 mux G */
#define MUX_PA24G_GCLK_IO4                 6
#define PORT_PA24G_GCLK_IO4        (1u << 24)
#define PIN_PB12G_GCLK_IO4                44  /**< \brief GCLK signal: IO4 on PB12 mux G */
#define MUX_PB12G_GCLK_IO4                 6
#define PORT_PB12G_GCLK_IO4        (1u << 12)
#define PIN_PA25G_GCLK_IO5                25  /**< \brief GCLK signal: IO5 on PA25 mux G */
#define MUX_PA25G_GCLK_IO5                 6
#define PORT_PA25G_GCLK_IO5        (1u << 25)
#define PIN_PB13G_GCLK_IO5                45  /**< \brief GCLK signal: IO5 on PB13 mux G */
#define MUX_PB13G_GCLK_IO5                 6
#define PORT_PB13G_GCLK_IO5        (1u << 13)
#define PIN_PA26G_GCLK_IO6                26  /**< \brief GCLK signal: IO6 on PA26 mux G */
#define MUX_PA26G_GCLK_IO6                 6
#define PORT_PA26G_GCLK_IO6        (1u << 26)
#define PIN_PB16G_GCLK_IO6                48  /**< \brief GCLK signal: IO6 on PB16 mux G */
#define MUX_PB16G_GCLK_IO6                 6
#define PORT_PB16G_GCLK_IO6        (1u << 16)
#define PIN_PA27G_GCLK_IO7                27  /**< \brief GCLK signal: IO7 on PA27 mux G */
#define MUX_PA27G_GCLK_IO7                 6
#define PORT_PA27G_GCLK_IO7        (1u << 27)
#define PIN_PB17G_GCLK_IO7                49  /**< \brief GCLK signal: IO7 on PB17 mux G */
#define MUX_PB17G_GCLK_IO7                 6
#define PORT_PB17G_GCLK_IO7        (1u << 17)
/* ========== PORT definition for PTC peripheral ========== */
#define PIN_PA10H_PTC_X0                  10  /**< \brief PTC signal: X0 on PA10 mux H */
#define MUX_PA10H_PTC_X0                   7
#define PORT_PA10H_PTC_X0          (1u << 10)
#define PIN_PA11H_PTC_X1                  11  /**< \brief PTC signal: X1 on PA11 mux H */
#define MUX_PA11H_PTC_X1                   7
#define PORT_PA11H_PTC_X1          (1u << 11)
#define PIN_PA14H_PTC_X2                  14  /**< \brief PTC signal: X2 on PA14 mux H */
#define MUX_PA14H_PTC_X2                   7
#define PORT_PA14H_PTC_X2          (1u << 14)
#define PIN_PA15H_PTC_X3                  15  /**< \brief PTC signal: X3 on PA15 mux H */
#define MUX_PA15H_PTC_X3                   7
#define PORT_PA15H_PTC_X3          (1u << 15)
#define PIN_PA16H_PTC_X4                  16  /**< \brief PTC signal: X4 on PA16 mux H */
#define MUX_PA16H_PTC_X4                   7
#define PORT_PA16H_PTC_X4          (1u << 16)
#define PIN_PA17H_PTC_X5                  17  /**< \brief PTC signal: X5 on PA17 mux H */
#define MUX_PA17H_PTC_X5                   7
#define PORT_PA17H_PTC_X5          (1u << 17)
#define PIN_PA18H_PTC_X6                  18  /**< \brief PTC signal: X6 on PA18 mux H */
#define MUX_PA18H_PTC_X6                   7
#define PORT_PA18H_PTC_X6          (1u << 18)
#define PIN_PA19H_PTC_X7                  19  /**< \brief PTC signal: X7 on PA19 mux H */
#define MUX_PA19H_PTC_X7                   7
#define PORT_PA19H_PTC_X7          (1u << 19)
#define PIN_PB20H_PTC_X8                  52  /**< \brief PTC signal: X8 on PB20 mux H */
#define MUX_PB20H_PTC_X8                   7
#define PORT_PB20H_PTC_X8          (1u << 20)
#define PIN_PB21H_PTC_X9                  53  /**< \brief PTC signal: X9 on PB21 mux H */
#define MUX_PB21H_PTC_X9                   7
#define PORT_PB21H_PTC_X9          (1u << 21)
#define PIN_PB22H_PTC_X10                 54  /**< \brief PTC signal: X10 on PB22 mux H */
#define MUX_PB22H_PTC_X10                  7
#define PORT_PB22H_PTC_X10         (1u << 22)
#define PIN_PB23H_PTC_X11                 55  /**< \brief PTC signal: X11 on PB23 mux H */
#define MUX_PB23H_PTC_X11                  7
#define PORT_PB23H_PTC_X11         (1u << 23)
#define PIN_PB24H_PTC_X12                 56  /**< \brief PTC signal: X12 on PB24 mux H */
#define MUX_PB24H_PTC_X12                  7
#define PORT_PB24H_PTC_X12         (1u << 24)
#define PIN_PB25H_PTC_X13                 57  /**< \brief PTC signal: X13 on PB25 mux H */
#define MUX_PB25H_PTC_X13                  7
#define PORT_PB25H_PTC_X13         (1u << 25)
#define PIN_PB26H_PTC_X14                 58  /**< \brief PTC signal: X14 on PB26 mux H */
#define MUX_PB26H_PTC_X14                  7
#define PORT_PB26H_PTC_X14         (1u << 26)
#define PIN_PB27H_PTC_X15                 59  /**< \brief PTC signal: X15 on PB27 mux H */
#define MUX_PB27H_PTC_X15                  7
#define PORT_PB27H_PTC_X15         (1u << 27)
#define PIN_PA00H_PTC_Y0                   0  /**< \brief PTC signal: Y0 on PA00 mux H */
#define MUX_PA00H_PTC_Y0                   7
#define PORT_PA00H_PTC_Y0          (1u <<  0)
#define PIN_PA01H_PTC_Y1                   1  /**< \brief PTC signal: Y1 on PA01 mux H */
#define MUX_PA01H_PTC_Y1                   7
#define PORT_PA01H_PTC_Y1          (1u <<  1)
#define PIN_PA02H_PTC_Y2                   2  /**< \brief PTC signal: Y2 on PA02 mux H */
#define MUX_PA02H_PTC_Y2                   7
#define PORT_PA02H_PTC_Y2          (1u <<  2)
#define PIN_PA03H_PTC_Y3                   3  /**< \brief PTC signal: Y3 on PA03 mux H */
#define MUX_PA03H_PTC_Y3                   7
#define PORT_PA03H_PTC_Y3          (1u <<  3)
#define PIN_PA04H_PTC_Y4                   4  /**< \brief PTC signal: Y4 on PA04 mux H */
#define MUX_PA04H_PTC_Y4                   7
#define PORT_PA04H_PTC_Y4          (1u <<  4)
#define PIN_PA05H_PTC_Y5                   5  /**< \brief PTC signal: Y5 on PA05 mux H */
#define MUX_PA05H_PTC_Y5                   7
#define PORT_PA05H_PTC_Y5          (1u <<  5)
#define PIN_PA06H_PTC_Y6                   6  /**< \brief PTC signal: Y6 on PA06 mux H */
#define MUX_PA06H_PTC_Y6                   7
#define PORT_PA06H_PTC_Y6          (1u <<  6)
#define PIN_PA07H_PTC_Y7                   7  /**< \brief PTC signal: Y7 on PA07 mux H */
#define MUX_PA07H_PTC_Y7                   7
#define PORT_PA07H_PTC_Y7          (1u <<  7)
#define PIN_PB00H_PTC_Y8                  32  /**< \brief PTC signal: Y8 on PB00 mux H */
#define MUX_PB00H_PTC_Y8                   7
#define PORT_PB00H_PTC_Y8          (1u <<  0)
#define PIN_PB01H_PTC_Y9                  33  /**< \brief PTC signal: Y9 on PB01 mux H */
#define MUX_PB01H_PTC_Y9                   7
#define PORT_PB01H_PTC_Y9          (1u <<  1)
#define PIN_PB02H_PTC_Y10                 34  /**< \brief PTC signal: Y10 on PB02 mux H */
#define MUX_PB02H_PTC_Y10                  7
#define PORT_PB02H_PTC_Y10         (1u <<  2)
#define PIN_PB03H_PTC_Y11                 35  /**< \brief PTC signal: Y11 on PB03 mux H */
#define MUX_PB03H_PTC_Y11                  7
#define PORT_PB03H_PTC_Y11         (1u <<  3)
#define PIN_PB04H_PTC_Y12                 36  /**< \brief PTC signal: Y12 on PB04 mux H */
#define MUX_PB04H_PTC_Y12                  7
#define PORT_PB04H_PTC_Y12         (1u <<  4)
#define PIN_PB05H_PTC_Y13                 37  /**< \brief PTC signal: Y13 on PB05 mux H */
#define MUX_PB05H_PTC_Y13                  7
#define PORT_PB05H_PTC_Y13         (1u <<  5)
#define PIN_PB06H_PTC_Y14                 38  /**< \brief PTC signal: Y14 on PB06 mux H */
#define MUX_PB06H_PTC_Y14                  7
#define PORT_PB06H_PTC_Y14         (1u <<  6)
#define PIN_PB07H_PTC_Y15                 39  /**< \brief PTC signal: Y15 on PB07 mux H */
#define MUX_PB07H_PTC_Y15                  7
#define PORT_PB07H_PTC_Y15         (1u <<  7)
/* ========== PORT definition for SERCOM0 peripheral ========== */
#define PIN_PA02C_SERCOM0_PAD0             2  /**< \brief SERCOM0 signal: PAD0 on PA02 mux C */
#define MUX_PA02C_SERCOM0_PAD0             2
#define PORT_PA02C_SERCOM0_PAD0    (1u <<  2)
#define PIN_PA10B_SERCOM0_PAD0            10  /**< \brief SERCOM0 signal: PAD0 on PA10 mux B */
#define MUX_PA10B_SERCOM0_PAD0             1
#define PORT_PA10B_SERCOM0_PAD0    (1u << 10)
#define PIN_PA03C_SERCOM0_PAD1             3  /**< \brief SERCOM0 signal: PAD1 on PA03 mux C */
#define MUX_PA03C_SERCOM0_PAD1             2
#define PORT_PA03C_SERCOM0_PAD1    (1u <<  3)
#define PIN_PA11B_SERCOM0_PAD1            11  /**< \brief SERCOM0 signal: PAD1 on PA11 mux B */
#define MUX_PA11B_SERCOM0_PAD1             1
#define PORT_PA11B_SERCOM0_PAD1    (1u << 11)
#define PIN_PA00C_SERCOM0_PAD2             0  /**< \brief SERCOM0 signal: PAD2 on PA00 mux C */
#define MUX_PA00C_SERCOM0_PAD2             2
#define PORT_PA00C_SERCOM0_PAD2    (1u <<  0)
#define PIN_PA08B_SERCOM0_PAD2             8  /**< \brief SERCOM0 signal: PAD2 on PA08 mux B */
#define MUX_PA08B_SERCOM0_PAD2             1
#define PORT_PA08B_SERCOM0_PAD2    (1u <<  8)
#define PIN_PA01C_SERCOM0_PAD3             1  /**< \brief SERCOM0 signal: PAD3 on PA01 mux C */
#define MUX_PA01C_SERCOM0_PAD3             2
#define PORT_PA01C_SERCOM0_PAD3    (1u <<  1)
#define PIN_PA09B_SERCOM0_PAD3             9  /**< \brief SERCOM0 signal: PAD3 on PA09 mux B */
#define MUX_PA09B_SERCOM0_PAD3             1
#define PORT_PA09B_SERCOM0_PAD3    (1u <<  9)
/* ========== PORT definition for SERCOM1 peripheral ========== */
#define PIN_PA30C_SERCOM1_PAD0            30  /**< \brief SERCOM1 signal: PAD0 on PA30 mux C */
#define MUX_PA30C_SERCOM1_PAD0             2
#define PORT_PA30C_SERCOM1_PAD0    (1u << 30)
#define PIN_PA18B_SERCOM1_PAD0            18  /**< \brief SERCOM1 signal: PAD0 on PA18 mux B */
#define MUX_PA18B_SERCOM1_PAD0             1
#define PORT_PA18B_SERCOM1_PAD0    (1u << 18)
#define PIN_PA31C_SERCOM1_PAD1            31  /**< \brief SERCOM1 signal: PAD1 on PA31 mux C */
#define MUX_PA31C_SERCOM1_PAD1             2
#define PORT_PA31C_SERCOM1_PAD1    (1u << 31)
#define PIN_PA19B_SERCOM1_PAD1            19  /**< \brief SERCOM1 signal: PAD1 on PA19 mux B */
#define MUX_PA19B_SERCOM1_PAD1             1
#define PORT_PA19B_SERCOM1_PAD1    (1u << 19)
#define PIN_PA04C_SERCOM1_PAD2             4  /**< \brief SERCOM1 signal: PAD2 on PA04 mux C */
#define MUX_PA04C_SERCOM1_PAD2             2
#define PORT_PA04C_SERCOM1_PAD2    (1u <<  4)
#define PIN_PA16B_SERCOM1_PAD2            16  /**< \brief SERCOM1 signal: PAD2 on PA16 mux B */
#define MUX_PA16B_SERCOM1_PAD2             1
#define PORT_PA16B_SERCOM1_PAD2    (1u << 16)
#define PIN_PA05C_SERCOM1_PAD3             5  /**< \brief SERCOM1 signal: PAD3 on PA05 mux C */
#define MUX_PA05C_SERCOM1_PAD3             2
#define PORT_PA05C_SERCOM1_PAD3    (1u <<  5)
#define PIN_PA17B_SERCOM1_PAD3            17  /**< \brief SERCOM1 signal: PAD3 on PA17 mux B */
#define MUX_PA17B_SERCOM1_PAD3             1
#define PORT_PA17B_SERCOM1_PAD3    (1u << 17)
/* ========== PORT definition for SERCOM2 peripheral ========== */
#define PIN_PA10C_SERCOM2_PAD0            10  /**< \brief SERCOM2 signal: PAD0 on PA10 mux C */
#define MUX_PA10C_SERCOM2_PAD0             2
#define PORT_PA10C_SERCOM2_PAD0    (1u << 10)
#define PIN_PA14B_SERCOM2_PAD0            14  /**< \brief SERCOM2 signal: PAD0 on PA14 mux B */
#define MUX_PA14B_SERCOM2_PAD0             1
#define PORT_PA14B_SERCOM2_PAD0    (1u << 14)
#define PIN_PA11C_SERCOM2_PAD1            11  /**< \brief SERCOM2 signal: PAD1 on PA11 mux C */
#define MUX_PA11C_SERCOM2_PAD1             2
#define PORT_PA11C_SERCOM2_PAD1    (1u << 11)
#define PIN_PA15B_SERCOM2_PAD1            15  /**< \brief SERCOM2 signal: PAD1 on PA15 mux B */
#define MUX_PA15B_SERCOM2_PAD1             1
#define PORT_PA15B_SERCOM2_PAD1    (1u << 15)
#define PIN_PA08C_SERCOM2_PAD2             8  /**< \brief SERCOM2 signal: PAD2 on PA08 mux C */
#define MUX_PA08C_SERCOM2_PAD2             2
#define PORT_PA08C_SERCOM2_PAD2    (1u <<  8)
#define PIN_PA12B_SERCOM2_PAD2            12  /**< \brief SERCOM2 signal: PAD2 on PA12 mux B */
#define MUX_PA12B_SERCOM2_PAD2             1
#define PORT_PA12B_SERCOM2_PAD2    (1u << 12)
#define PIN_PA09C_SERCOM2_PAD3             9  /**< \brief SERCOM2 signal: PAD3 on PA09 mux C */
#define MUX_PA09C_SERCOM2_PAD3             2
#define PORT_PA09C_SERCOM2_PAD3    (1u <<  9)
#define PIN_PA13B_SERCOM2_PAD3            13  /**< \brief SERCOM2 signal: PAD3 on PA13 mux B */
#define MUX_PA13B_SERCOM2_PAD3             1
#define PORT_PA13B_SERCOM2_PAD3    (1u << 13)
/* ========== PORT definition for SERCOM3 peripheral ========== */
#define PIN_PA18C_SERCOM3_PAD0            18  /**< \brief SERCOM3 signal: PAD0 on PA18 mux C */
#define MUX_PA18C_SERCOM3_PAD0             2
#define PORT_PA18C_SERCOM3_PAD0    (1u << 18)
#define PIN_PA24B_SERCOM3_PAD0            24  /**< \brief SERCOM3 signal: PAD0 on PA24 mux B */
#define MUX_PA24B_SERCOM3_PAD0             1
#define PORT_PA24B_SERCOM3_PAD0    (1u << 24)
#define PIN_PA19C_SERCOM3_PAD1            19  /**< \brief SERCOM3 signal: PAD1 on PA19 mux C */
#define MUX_PA19C_SERCOM3_PAD1             2
#define PORT_PA19C_SERCOM3_PAD1    (1u << 19)
#define PIN_PA25B_SERCOM3_PAD1            25  /**< \brief SERCOM3 signal: PAD1 on PA25 mux B */
#define MUX_PA25B_SERCOM3_PAD1             1
#define PORT_PA25B_SERCOM3_PAD1    (1u << 25)
#define PIN_PA16C_SERCOM3_PAD2            16  /**< \brief SERCOM3 signal: PAD2 on PA16 mux C */
#define MUX_PA16C_SERCOM3_PAD2             2
#define PORT_PA16C_SERCOM3_PAD2    (1u << 16)
#define PIN_PA26B_SERCOM3_PAD2            26  /**< \brief SERCOM3 signal: PAD2 on PA26 mux B */
#define MUX_PA26B_SERCOM3_PAD2             1
#define PORT_PA26B_SERCOM3_PAD2    (1u << 26)
#define PIN_PA17C_SERCOM3_PAD3            17  /**< \brief SERCOM3 signal: PAD3 on PA17 mux C */
#define MUX_PA17C_SERCOM3_PAD3             2
#define PORT_PA17C_SERCOM3_PAD3    (1u << 17)
#define PIN_PA27B_SERCOM3_PAD3            27  /**< \brief SERCOM3 signal: PAD3 on PA27 mux B */
#define MUX_PA27B_SERCOM3_PAD3             1
#define PORT_PA27B_SERCOM3_PAD3    (1u << 27)
/* ========== PORT definition for SERCOM4 peripheral ========== */
#define PIN_PA14C_SERCOM4_PAD0            14  /**< \brief SERCOM4 signal: PAD0 on PA14 mux C */
#define MUX_PA14C_SERCOM4_PAD0             2
#define PORT_PA14C_SERCOM4_PAD0    (1u << 14)
#define PIN_PB00C_SERCOM4_PAD0            32  /**< \brief SERCOM4 signal: PAD0 on PB00 mux C */
#define MUX_PB00C_SERCOM4_PAD0             2
#define PORT_PB00C_SERCOM4_PAD0    (1u <<  0)
#define PIN_PB08B_SERCOM4_PAD0            40  /**< \brief SERCOM4 signal: PAD0 on PB08 mux B */
#define MUX_PB08B_SERCOM4_PAD0             1
#define PORT_PB08B_SERCOM4_PAD0    (1u <<  8)
#define PIN_PA15C_SERCOM4_PAD1            15  /**< \brief SERCOM4 signal: PAD1 on PA15 mux C */
#define MUX_PA15C_SERCOM4_PAD1             2
#define PORT_PA15C_SERCOM4_PAD1    (1u << 15)
#define PIN_PB01C_SERCOM4_PAD1            33  /**< \brief SERCOM4 signal: PAD1 on PB01 mux C */
#define MUX_PB01C_SERCOM4_PAD1             2
#define PORT_PB01C_SERCOM4_PAD1    (1u <<  1)
#define PIN_PB09B_SERCOM4_PAD1            41  /**< \brief SERCOM4 signal: PAD1 on PB09 mux B */
#define MUX_PB09B_SERCOM4_PAD1             1
#define PORT_PB09B_SERCOM4_PAD1    (1u <<  9)
#define PIN_PB02C_SERCOM4_PAD2            34  /**< \brief SERCOM4 signal: PAD2 on PB02 mux C */
#define MUX_PB02C_SERCOM4_PAD2             2
#define PORT_PB02C_SERCOM4_PAD2    (1u <<  2)
#define PIN_PB10B_SERCOM4_PAD2            42  /**< \brief SERCOM4 signal: PAD2 on PB10 mux B */
#define MUX_PB10B_SERCOM4_PAD2             1
#define PORT_PB10B_SERCOM4_PAD2    (1u << 10)
#define PIN_PA12C_SERCOM4_PAD2            12  /**< \brief SERCOM4 signal: PAD2 on PA12 mux C */
#define MUX_PA12C_SERCOM4_PAD2             2
#define PORT_PA12C_SERCOM4_PAD2    (1u << 12)
#define PIN_PA13C_SERCOM4_PAD3            13  /**< \brief SERCOM4 signal: PAD3 on PA13 mux C */
#define MUX_PA13C_SERCOM4_PAD3             2
#define PORT_PA13C_SERCOM4_PAD3    (1u << 13)
#define PIN_PB03C_SERCOM4_PAD3            35  /**< \brief SERCOM4 signal: PAD3 on PB03 mux C */
#define MUX_PB03C_SERCOM4_PAD3             2
#define PORT_PB03C_SERCOM4_PAD3    (1u <<  3)
#define PIN_PB11B_SERCOM4_PAD3            43  /**< \brief SERCOM4 signal: PAD3 on PB11 mux B */
#define MUX_PB11B_SERCOM4_PAD3             1
#define PORT_PB11B_SERCOM4_PAD3    (1u << 11)
/* ========== PORT definition for SERCOM5 peripheral ========== */
#define PIN_PA24C_SERCOM5_PAD0            24  /**< \brief SERCOM5 signal: PAD0 on PA24 mux C */
#define MUX_PA24C_SERCOM5_PAD0             2
#define PORT_PA24C_SERCOM5_PAD0    (1u << 24)
#define PIN_PB16C_SERCOM5_PAD0            48  /**< \brief SERCOM5 signal: PAD0 on PB16 mux C */
#define MUX_PB16C_SERCOM5_PAD0             2
#define PORT_PB16C_SERCOM5_PAD0    (1u << 16)
#define PIN_PB20B_SERCOM5_PAD0            52  /**< \brief SERCOM5 signal: PAD0 on PB20 mux B */
#define MUX_PB20B_SERCOM5_PAD0             1
#define PORT_PB20B_SERCOM5_PAD0    (1u << 20)
#define PIN_PA25C_SERCOM5_PAD1            25  /**< \brief SERCOM5 signal: PAD1 on PA25 mux C */
#define MUX_PA25C_SERCOM5_PAD1             2
#define PORT_PA25C_SERCOM5_PAD1    (1u << 25)
#define PIN_PB17C_SERCOM5_PAD1            49  /**< \brief SERCOM5 signal: PAD1 on PB17 mux C */
#define MUX_PB17C_SERCOM5_PAD1             2
#define PORT_PB17C_SERCOM5_PAD1    (1u << 17)
#define PIN_PB21B_SERCOM5_PAD1            53  /**< \brief SERCOM5 signal: PAD1 on PB21 mux B */
#define MUX_PB21B_SERCOM5_PAD1             1
#define PORT_PB21B_SERCOM5_PAD1    (1u << 21)
#define PIN_PA26C_SERCOM5_PAD2            26  /**< \brief SERCOM5 signal: PAD2 on PA26 mux C */
#define MUX_PA26C_SERCOM5_PAD2             2
#define PORT_PA26C_SERCOM5_PAD2    (1u << 26)
#define PIN_PB18C_SERCOM5_PAD2            50  /**< \brief SERCOM5 signal: PAD2 on PB18 mux C */
#define MUX_PB18C_SERCOM5_PAD2             2
#define PORT_PB18C_SERCOM5_PAD2    (1u << 18)
#define PIN_PB22B_SERCOM5_PAD2            54  /**< \brief SERCOM5 signal: PAD2 on PB22 mux B */
#define MUX_PB22B_SERCOM5_PAD2             1
#define PORT_PB22B_SERCOM5_PAD2    (1u << 22)
#define PIN_PA27C_SERCOM5_PAD3            27  /**< \brief SERCOM5 signal: PAD3 on PA27 mux C */
#define MUX_PA27C_SERCOM5_PAD3             2
#define PORT_PA27C_SERCOM5_PAD3    (1u << 27)
#define PIN_PB19C_SERCOM5_PAD3            51  /**< \brief SERCOM5 signal: PAD3 on PB19 mux C */
#define MUX_PB19C_SERCOM5_PAD3             2
#define PORT_PB19C_SERCOM5_PAD3    (1u << 19)
#define PIN_PB23B_SERCOM5_PAD3            55  /**< \brief SERCOM5 signal: PAD3 on PB23 mux B */
#define MUX_PB23B_SERCOM5_PAD3             1
#define PORT_PB23B_SERCOM5_PAD3    (1u << 23)
/* ========== PORT definition for TC0 peripheral ========== */
#define PIN_PA00D_TC0_WO0                  0  /**< \brief TC0 signal: WO0 on PA00 mux D */
#define MUX_PA00D_TC0_WO0                  3
#define PORT_PA00D_TC0_WO0         (1u <<  0)
#define PIN_PA08D_TC0_WO0                  8  /**< \brief TC0 signal: WO0 on PA08 mux D */
#define MUX_PA08D_TC0_WO0                  3
#define PORT_PA08D_TC0_WO0         (1u <<  8)
#define PIN_PA01D_TC0_WO1                  1  /**< \brief TC0 signal: WO1 on PA01 mux D */
#define MUX_PA01D_TC0_WO1                  3
#define PORT_PA01D_TC0_WO1         (1u <<  1)
#define PIN_PA09D_TC0_WO1                  9  /**< \brief TC0 signal: WO1 on PA09 mux D */
#define MUX_PA09D_TC0_WO1                  3
#define PORT_PA09D_TC0_WO1         (1u <<  9)
/* ========== PORT definition for TC1 peripheral ========== */
#define PIN_PA04D_TC1_WO0                  4  /**< \brief TC1 signal: WO0 on PA04 mux D */
#define MUX_PA04D_TC1_WO0                  3
#define PORT_PA04D_TC1_WO0         (1u <<  4)
#define PIN_PA10D_TC1_WO0                 10  /**< \brief TC1 signal: WO0 on PA10 mux D */
#define MUX_PA10D_TC1_WO0                  3
#define PORT_PA10D_TC1_WO0         (1u << 10)
#define PIN_PA30D_TC1_WO0                 30  /**< \brief TC1 signal: WO0 on PA30 mux D */
#define MUX_PA30D_TC1_WO0                  3
#define PORT_PA30D_TC1_WO0         (1u << 30)
#define PIN_PA05D_TC1_WO1                  5  /**< \brief TC1 signal: WO1 on PA05 mux D */
#define MUX_PA05D_TC1_WO1                  3
#define PORT_PA05D_TC1_WO1         (1u <<  5)
#define PIN_PA11D_TC1_WO1                 11  /**< \brief TC1 signal: WO1 on PA11 mux D */
#define MUX_PA11D_TC1_WO1                  3
#define PORT_PA11D_TC1_WO1         (1u << 11)
#define PIN_PA31D_TC1_WO1                 31  /**< \brief TC1 signal: WO1 on PA31 mux D */
#define MUX_PA31D_TC1_WO1                  3
#define PORT_PA31D_TC1_WO1         (1u << 31)
/* ========== PORT definition for TC2 peripheral ========== */
#define PIN_PA16D_TC2_WO0                 16  /**< \brief TC2 signal: WO0 on PA16 mux D */
#define MUX_PA16D_TC2_WO0                  3
#define PORT_PA16D_TC2_WO0         (1u << 16)
#define PIN_PA12D_TC2_WO0                 12  /**< \brief TC2 signal: WO0 on PA12 mux D */
#define MUX_PA12D_TC2_WO0                  3
#define PORT_PA12D_TC2_WO0         (1u << 12)
#define PIN_PA17D_TC2_WO1                 17  /**< \brief TC2 signal: WO1 on PA17 mux D */
#define MUX_PA17D_TC2_WO1                  3
#define PORT_PA17D_TC2_WO1         (1u << 17)
#define PIN_PA13D_TC2_WO1                 13  /**< \brief TC2 signal: WO1 on PA13 mux D */
#define MUX_PA13D_TC2_WO1                  3
#define PORT_PA13D_TC2_WO1         (1u << 13)
/* ========== PORT definition for TC3 peripheral ========== */
#define PIN_PA14D_TC3_WO0                 14  /**< \brief TC3 signal: WO0 on PA14 mux D */
#define MUX_PA14D_TC3_WO0                  3
#define PORT_PA14D_TC3_WO0         (1u << 14)
#define PIN_PA18D_TC3_WO0                 18  /**< \brief TC3 signal: WO0 on PA18 mux D */
#define MUX_PA18D_TC3_WO0                  3
#define PORT_PA18D_TC3_WO0         (1u << 18)
#define PIN_PA15D_TC3_WO1                 15  /**< \brief TC3 signal: WO1 on PA15 mux D */
#define MUX_PA15D_TC3_WO1                  3
#define PORT_PA15D_TC3_WO1         (1u << 15)
#define PIN_PA19D_TC3_WO1                 19  /**< \brief TC3 signal: WO1 on PA19 mux D */
#define MUX_PA19D_TC3_WO1                  3
#define PORT_PA19D_TC3_WO1         (1u << 19)
/* ========== PORT definition for TC4 peripheral ========== */
#define PIN_PB08D_TC4_WO0                 40  /**< \brief TC4 signal: WO0 on PB08 mux D */
#define MUX_PB08D_TC4_WO0                  3
#define PORT_PB08D_TC4_WO0         (1u <<  8)
#define PIN_PA24D_TC4_WO0                 24  /**< \brief TC4 signal: WO0 on PA24 mux D */
#define MUX_PA24D_TC4_WO0                  3
#define PORT_PA24D_TC4_WO0         (1u << 24)
#define PIN_PB09D_TC4_WO1                 41  /**< \brief TC4 signal: WO1 on PB09 mux D */
#define MUX_PB09D_TC4_WO1                  3
#define PORT_PB09D_TC4_WO1         (1u <<  9)
#define PIN_PA25D_TC4_WO1                 25  /**< \brief TC4 signal: WO1 on PA25 mux D */
#define MUX_PA25D_TC4_WO1                  3
#define PORT_PA25D_TC4_WO1         (1u << 25)
/* ========== PORT definition for TC5 peripheral ========== */
#define PIN_PA26D_TC5_WO0                 26  /**< \brief TC5 signal: WO0 on PA26 mux D */
#define MUX_PA26D_TC5_WO0                  3
#define PORT_PA26D_TC5_WO0         (1u << 26)
#define PIN_PB00D_TC5_WO0                 32  /**< \brief TC5 signal: WO0 on PB00 mux D */
#define MUX_PB00D_TC5_WO0                  3
#define PORT_PB00D_TC5_WO0         (1u <<  0)
#define PIN_PB10D_TC5_WO0                 42  /**< \brief TC5 signal: WO0 on PB10 mux D */
#define MUX_PB10D_TC5_WO0                  3
#define PORT_PB10D_TC5_WO0         (1u << 10)
#define PIN_PA27D_TC5_WO1                 27  /**< \brief TC5 signal: WO1 on PA27 mux D */
#define MUX_PA27D_TC5_WO1                  3
#define PORT_PA27D_TC5_WO1         (1u << 27)
#define PIN_PB01D_TC5_WO1                 33  /**< \brief TC5 signal: WO1 on PB01 mux D */
#define MUX_PB01D_TC5_WO1                  3
#define PORT_PB01D_TC5_WO1         (1u <<  1)
#define PIN_PB11D_TC5_WO1                 43  /**< \brief TC5 signal: WO1 on PB11 mux D */
#define MUX_PB11D_TC5_WO1                  3
#define PORT_PB11D_TC5_WO1         (1u << 11)
/* ========== PORT definition for TC6 peripheral ========== */
#define PIN_PB16D_TC6_WO0                 48  /**< \brief TC6 signal: WO0 on PB16 mux D */
#define MUX_PB16D_TC6_WO0                  3
#define PORT_PB16D_TC6_WO0         (1u << 16)
#define PIN_PB20D_TC6_WO0                 52  /**< \brief TC6 signal: WO0 on PB20 mux D */
#define MUX_PB20D_TC6_WO0                  3
#define PORT_PB20D_TC6_WO0         (1u << 20)
#define PIN_PB17D_TC6_WO1                 49  /**< \brief TC6 signal: WO1 on PB17 mux D */
#define MUX_PB17D_TC6_WO1                  3
#define PORT_PB17D_TC6_WO1         (1u << 17)
#define PIN_PB21D_TC6_WO1                 53  /**< \brief TC6 signal: WO1 on PB21 mux D */
#define MUX_PB21D_TC6_WO1                  3
#define PORT_PB21D_TC6_WO1         (1u << 21)
/* ========== PORT definition for TC7 peripheral ========== */
#define PIN_PB18D_TC7_WO0                 50  /**< \brief TC7 signal: WO0 on PB18 mux D */
#define MUX_PB18D_TC7_WO0                  3
#define PORT_PB18D_TC7_WO0         (1u << 18)
#define PIN_PB22D_TC7_WO0                 54  /**< \brief TC7 signal: WO0 on PB22 mux D */
#define MUX_PB22D_TC7_WO0                  3
#define PORT_PB22D_TC7_WO0         (1u << 22)
#define PIN_PB19D_TC7_WO1                 51  /**< \brief TC7 signal: WO1 on PB19 mux D */
#define MUX_PB19D_TC7_WO1                  3
#define PORT_PB19D_TC7_WO1         (1u << 19)
#define PIN_PB23D_TC7_WO1                 55  /**< \brief TC7 signal: WO1 on PB23 mux D */
#define MUX_PB23D_TC7_WO1                  3
#define PORT_PB23D_TC7_WO1         (1u << 23)

/*@}*/

/* ************************************************************************** */
/**  MEMORY MAPPING DEFINITIONS FOR SAM0D4_REVA */
/* ************************************************************************** */

#if defined __SAM0D4_REVA__
  #define FLASH_SIZE          0x40000 /* 256 kB */
  #define FLASH_PAGE_SIZE     512
  #define FLASH_NB_OF_PAGES   512
  #define FLASH_USER_PAGE_SIZE 512
  #define HRAMC0_SIZE         0x8000 /* 32 kB */
#else
  #error Library does not support the specified device.
#endif

#define FLASH_ADDR            (0x00000000u) /**< FLASH base address */
#define FLASH_USER_PAGE_ADDR  (0x00800000u) /**< FLASH_USER_PAGE base address */
#define HRAMC0_ADDR           (0x20000000u) /**< HRAMC0 base address */

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* SAM0D4_REVA_H */
