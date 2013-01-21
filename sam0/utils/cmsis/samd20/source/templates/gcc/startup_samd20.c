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
void PM_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NVMCTRL_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_IRQn_Handler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_IRQn_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AC_IRQn_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PTC_IRQn_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors __vector_table = {

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
        { (void*) PM_IRQn_Handler      }, /*  0 Power Manager */
        { (void*) SYSCTRL_IRQn_Handler }, /*  1 System Control */
          (void*) WDT_IRQn_Handler      , /*  2 Watchdog Timer */
        { (void*) RTC_IRQn_Handler     }, /*  3 Real-Time Counter */
        { (void*) EIC_IRQn_Handler     }, /*  4 External Interrupt Controller */
        { (void*) NVMCTRL_IRQn_Handler }, /*  5 Non-Volatile Memory Controller */
        { (void*) EVSYS_IRQn_Handler   }, /*  6 Event System Interface */
        { (void*) SERCOM0_IRQn_Handler }, /*  7 Serial Communication Interface 0 */
        { (void*) SERCOM1_IRQn_Handler }, /*  8 Serial Communication Interface 1 */
        { (void*) SERCOM2_IRQn_Handler }, /*  9 Serial Communication Interface 2 */
        { (void*) SERCOM3_IRQn_Handler }, /* 10 Serial Communication Interface 3 */
        { (void*) SERCOM4_IRQn_Handler }, /* 11 Serial Communication Interface 4 */
        { (void*) SERCOM5_IRQn_Handler }, /* 12 Serial Communication Interface 5 */
        { (void*) TC0_IRQn_Handler     }, /* 13 Basic Timer Counter 0 */
        { (void*) TC1_IRQn_Handler     }, /* 14 Basic Timer Counter 1 */
        { (void*) TC2_IRQn_Handler     }, /* 15 Basic Timer Counter 2 */
        { (void*) TC3_IRQn_Handler     }, /* 16 Basic Timer Counter 3 */
        { (void*) TC4_IRQn_Handler     }, /* 17 Basic Timer Counter 4 */
        { (void*) TC5_IRQn_Handler     }, /* 18 Basic Timer Counter 5 */
        { (void*) TC6_IRQn_Handler     }, /* 19 Basic Timer Counter 6 */
        { (void*) TC7_IRQn_Handler     }, /* 20 Basic Timer Counter 7 */
        { (void*) ADC_IRQn_Handler     }, /* 21 Analog Digital Converter */
        { (void*) AC_IRQn_Handler      }, /* 22 Analog Comparators */
        { (void*) DAC_IRQn_Handler     }, /* 23 Digital Analog Converter */
        { (void*) PTC_IRQn_Handler     }  /* 24 Peripheral Touch Controller */
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
