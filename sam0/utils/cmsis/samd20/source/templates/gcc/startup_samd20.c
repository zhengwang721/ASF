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

#include "samd20.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M0+ core handlers */
void NonMaskableInt_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVCall_Handler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void PM_CFD_IRQn_Handler              ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PM_CKRDY_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_BOD12DET_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_BOD12RDY_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_BOD12SYNRDY_IRQn_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_BOD33DET_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_BOD33RDY_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_BOD33SYNRDY_IRQn_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_DFLLLOCKC_IRQn_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_DFLLLOCKF_IRQn_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_DFLLOOB_IRQn_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_DFLLRCS_IRQn_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_DFLLRDY_IRQn_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_OSC32KRDY_IRQn_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_XOSCRDY_IRQn_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_XOSC32RDY_IRQn_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_EW_IRQn_Handler              ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_CMP_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_CMP_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_0_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_1_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_2_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_3_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_4_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_5_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_6_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_7_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_8_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_9_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_10_IRQn_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_11_IRQn_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_12_IRQn_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_13_IRQn_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_14_IRQn_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_15_IRQn_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NVMCTRL_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NVMCTRL_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_EVD_0_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_EVD_1_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_EVD_2_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_EVD_3_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_EVD_4_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_EVD_5_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_EVD_6_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_EVD_7_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OVR_0_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OVR_1_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OVR_2_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OVR_3_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OVR_4_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OVR_5_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OVR_6_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OVR_7_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_2_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_3_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_2_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_3_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_2_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_3_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_2_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_3_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_2_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_3_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_2_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_3_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_ERR_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_MC_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_MC_1_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_ERR_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_MC_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_MC_1_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_ERR_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_MC_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_MC_1_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_ERR_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_MC_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_MC_1_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_ERR_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_MC_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_MC_1_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_ERR_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_MC_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_MC_1_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_ERR_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_MC_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_MC_1_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_ERR_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_MC_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_MC_1_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_OVF_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_OVERRUN_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_RESRDY_IRQn_Handler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_WINMON_IRQn_Handler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AC_COMP_0_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AC_COMP_1_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AC_WIN_0_IRQn_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_EMPTY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_UNDERRUN_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PTC_EOC_IRQn_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PTC_READY_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PTC_WCOMP_IRQn_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

        /* Configure Initial Stack Pointer, using linker-generated symbols */
        (void*) (&_estack),

        (void*) Reset_Handler,
        (void*) NonMaskableInt_Handler,
        (void*) HardFault_Handler,
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) SVCall_Handler,
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) PendSV_Handler,
        (void*) SysTick_Handler,

        /* Configurable interrupts */
        // TODO
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
        uint32_t *pSrc, *pDest;

        /* Initialize the relocate segment */
        pSrc = &_etext;
        pDest = &_srelocate;

        if (pSrc != pDest) {
                for (; pDest < &_erelocate;) {
                        *pDest++ = *pSrc++;
                }
        }

        /* Clear the zero segment */
        for (pDest = &_szero; pDest < &_ezero;) {
                *pDest++ = 0;
        }

        /* Set the vector table base address */
        pSrc = (uint32_t *) & _sfixed;
        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        /* Initialize the C library */
        __libc_init_array();

        /* Branch to main function */
        main();

        /* Infinite loop */
        while (1);
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
        while (1) {
        }
}
