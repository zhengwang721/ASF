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

typedef void (*intfunc) (void);
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

void __iar_program_start(void);
int __low_level_init(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M0+ core handlers */
#pragma weak Reset_Handler            = Dummy_Handler
#pragma weak NonMaskableInt_Handler   = Dummy_Handler
#pragma weak HardFault_Handler        = Dummy_Handler
#pragma weak SVCall_Handler           = Dummy_Handler
#pragma weak PendSV_Handler           = Dummy_Handler
#pragma weak SysTick_Handler          = Dummy_Handler

/* Peripherals handlers */
#pragma weak PM_CFD_IRQn_Handler              = Dummy_Handler
#pragma weak PM_CKRDY_IRQn_Handler            = Dummy_Handler
#pragma weak SYSCTRL_BOD12DET_IRQn_Handler    = Dummy_Handler
#pragma weak SYSCTRL_BOD12RDY_IRQn_Handler    = Dummy_Handler
#pragma weak SYSCTRL_BOD12SYNRDY_IRQn_Handler = Dummy_Handler
#pragma weak SYSCTRL_BOD33DET_IRQn_Handler    = Dummy_Handler
#pragma weak SYSCTRL_BOD33RDY_IRQn_Handler    = Dummy_Handler
#pragma weak SYSCTRL_BOD33SYNRDY_IRQn_Handler = Dummy_Handler
#pragma weak SYSCTRL_DFLLLOCKC_IRQn_Handler   = Dummy_Handler
#pragma weak SYSCTRL_DFLLLOCKF_IRQn_Handler   = Dummy_Handler
#pragma weak SYSCTRL_DFLLOOB_IRQn_Handler     = Dummy_Handler
#pragma weak SYSCTRL_DFLLRCS_IRQn_Handler     = Dummy_Handler
#pragma weak SYSCTRL_DFLLRDY_IRQn_Handler     = Dummy_Handler
#pragma weak SYSCTRL_OSC32KRDY_IRQn_Handler   = Dummy_Handler
#pragma weak SYSCTRL_XOSCRDY_IRQn_Handler     = Dummy_Handler
#pragma weak SYSCTRL_XOSC32RDY_IRQn_Handler   = Dummy_Handler
#pragma weak WDT_EW_IRQn_Handler              = Dummy_Handler
#pragma weak RTC_CMP_0_IRQn_Handler           = Dummy_Handler
#pragma weak RTC_CMP_1_IRQn_Handler           = Dummy_Handler
#pragma weak RTC_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak RTC_READY_IRQn_Handler           = Dummy_Handler
#pragma weak EIC_EXTINT_0_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_1_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_2_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_3_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_4_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_5_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_6_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_7_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_8_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_9_IRQn_Handler        = Dummy_Handler
#pragma weak EIC_EXTINT_10_IRQn_Handler       = Dummy_Handler
#pragma weak EIC_EXTINT_11_IRQn_Handler       = Dummy_Handler
#pragma weak EIC_EXTINT_12_IRQn_Handler       = Dummy_Handler
#pragma weak EIC_EXTINT_13_IRQn_Handler       = Dummy_Handler
#pragma weak EIC_EXTINT_14_IRQn_Handler       = Dummy_Handler
#pragma weak EIC_EXTINT_15_IRQn_Handler       = Dummy_Handler
#pragma weak NVMCTRL_0_IRQn_Handler           = Dummy_Handler
#pragma weak NVMCTRL_1_IRQn_Handler           = Dummy_Handler
#pragma weak EVSYS_EVD_0_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_EVD_1_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_EVD_2_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_EVD_3_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_EVD_4_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_EVD_5_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_EVD_6_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_EVD_7_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_OVR_0_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_OVR_1_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_OVR_2_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_OVR_3_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_OVR_4_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_OVR_5_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_OVR_6_IRQn_Handler         = Dummy_Handler
#pragma weak EVSYS_OVR_7_IRQn_Handler         = Dummy_Handler
#pragma weak SERCOM0_0_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM0_1_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM0_2_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM0_3_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM1_0_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM1_1_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM1_2_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM1_3_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM2_0_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM2_1_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM2_2_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM2_3_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM3_0_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM3_1_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM3_2_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM3_3_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM4_0_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM4_1_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM4_2_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM4_3_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM5_0_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM5_1_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM5_2_IRQn_Handler           = Dummy_Handler
#pragma weak SERCOM5_3_IRQn_Handler           = Dummy_Handler
#pragma weak TC0_ERR_IRQn_Handler             = Dummy_Handler
#pragma weak TC0_MC_0_IRQn_Handler            = Dummy_Handler
#pragma weak TC0_MC_1_IRQn_Handler            = Dummy_Handler
#pragma weak TC0_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak TC0_READY_IRQn_Handler           = Dummy_Handler
#pragma weak TC1_ERR_IRQn_Handler             = Dummy_Handler
#pragma weak TC1_MC_0_IRQn_Handler            = Dummy_Handler
#pragma weak TC1_MC_1_IRQn_Handler            = Dummy_Handler
#pragma weak TC1_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak TC1_READY_IRQn_Handler           = Dummy_Handler
#pragma weak TC2_ERR_IRQn_Handler             = Dummy_Handler
#pragma weak TC2_MC_0_IRQn_Handler            = Dummy_Handler
#pragma weak TC2_MC_1_IRQn_Handler            = Dummy_Handler
#pragma weak TC2_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak TC2_READY_IRQn_Handler           = Dummy_Handler
#pragma weak TC3_ERR_IRQn_Handler             = Dummy_Handler
#pragma weak TC3_MC_0_IRQn_Handler            = Dummy_Handler
#pragma weak TC3_MC_1_IRQn_Handler            = Dummy_Handler
#pragma weak TC3_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak TC3_READY_IRQn_Handler           = Dummy_Handler
#pragma weak TC4_ERR_IRQn_Handler             = Dummy_Handler
#pragma weak TC4_MC_0_IRQn_Handler            = Dummy_Handler
#pragma weak TC4_MC_1_IRQn_Handler            = Dummy_Handler
#pragma weak TC4_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak TC4_READY_IRQn_Handler           = Dummy_Handler
#pragma weak TC5_ERR_IRQn_Handler             = Dummy_Handler
#pragma weak TC5_MC_0_IRQn_Handler            = Dummy_Handler
#pragma weak TC5_MC_1_IRQn_Handler            = Dummy_Handler
#pragma weak TC5_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak TC5_READY_IRQn_Handler           = Dummy_Handler
#pragma weak TC6_ERR_IRQn_Handler             = Dummy_Handler
#pragma weak TC6_MC_0_IRQn_Handler            = Dummy_Handler
#pragma weak TC6_MC_1_IRQn_Handler            = Dummy_Handler
#pragma weak TC6_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak TC6_READY_IRQn_Handler           = Dummy_Handler
#pragma weak TC7_ERR_IRQn_Handler             = Dummy_Handler
#pragma weak TC7_MC_0_IRQn_Handler            = Dummy_Handler
#pragma weak TC7_MC_1_IRQn_Handler            = Dummy_Handler
#pragma weak TC7_OVF_IRQn_Handler             = Dummy_Handler
#pragma weak TC7_READY_IRQn_Handler           = Dummy_Handler
#pragma weak ADC_OVERRUN_IRQn_Handler         = Dummy_Handler
#pragma weak ADC_READY_IRQn_Handler           = Dummy_Handler
#pragma weak ADC_RESRDY_IRQn_Handler          = Dummy_Handler
#pragma weak ADC_WINMON_IRQn_Handler          = Dummy_Handler
#pragma weak AC_COMP_0_IRQn_Handler           = Dummy_Handler
#pragma weak AC_COMP_1_IRQn_Handler           = Dummy_Handler
#pragma weak AC_WIN_0_IRQn_Handler            = Dummy_Handler
#pragma weak DAC_EMPTY_IRQn_Handler           = Dummy_Handler
#pragma weak DAC_READY_IRQn_Handler           = Dummy_Handler
#pragma weak DAC_UNDERRUN_IRQn_Handler        = Dummy_Handler
#pragma weak PTC_EOC_IRQn_Handler             = Dummy_Handler
#pragma weak PTC_READY_IRQn_Handler           = Dummy_Handler
#pragma weak PTC_WCOMP_IRQn_Handler           = Dummy_Handler

/* Exception Table */

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section = ".intvec"
#pragma location = ".intvec"
const DeviceVectors __vector_table[] = {
        (void*) (&__cstack_end__),

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
        union {
          (void*) PM_CFD_Handler,              /*  0 Power Manager0 */
          (void*) PM_CKRDY_Handler             /*  0 Power Manager0 */
        },
        union {
          (void*) SYSCTRL_BOD12DET_Handler,    /*  1 System Control0 */
          (void*) SYSCTRL_BOD12RDY_Handler,    /*  1 System Control0 */
          (void*) SYSCTRL_BOD12SYNRDY_Handler, /*  1 System Control0 */
          (void*) SYSCTRL_BOD33DET_Handler,    /*  1 System Control0 */
          (void*) SYSCTRL_BOD33RDY_Handler,    /*  1 System Control0 */
          (void*) SYSCTRL_BOD33SYNRDY_Handler, /*  1 System Control0 */
          (void*) SYSCTRL_DFLLLOCKC_Handler,   /*  1 System Control0 */
          (void*) SYSCTRL_DFLLLOCKF_Handler,   /*  1 System Control0 */
          (void*) SYSCTRL_DFLLOOB_Handler,     /*  1 System Control0 */
          (void*) SYSCTRL_DFLLRCS_Handler,     /*  1 System Control0 */
          (void*) SYSCTRL_DFLLRDY_Handler,     /*  1 System Control0 */
          (void*) SYSCTRL_OSC32KRDY_Handler,   /*  1 System Control0 */
          (void*) SYSCTRL_XOSCRDY_Handler,     /*  1 System Control0 */
          (void*) SYSCTRL_XOSC32RDY_Handler    /*  1 System Control0 */
        },
        (void*) WDT_EW_Handler,                /*  2 Watchdog Timer0 */
        union {
          (void*) RTC_CMP_0_Handler,           /*  3 Real-Time Counter0 */
          (void*) RTC_CMP_1_Handler,           /*  3 Real-Time Counter0 */
          (void*) RTC_OVF_Handler,             /*  3 Real-Time Counter0 */
          (void*) RTC_READY_Handler            /*  3 Real-Time Counter0 */
        },
        union {
          (void*) EIC_EXTINT_0_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_1_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_2_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_3_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_4_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_5_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_6_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_7_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_8_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_9_Handler,        /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_10_Handler,       /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_11_Handler,       /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_12_Handler,       /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_13_Handler,       /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_14_Handler,       /*  4 External Interrupt Controller0 */
          (void*) EIC_EXTINT_15_Handler        /*  4 External Interrupt Controller0 */
        },
        union {
          (void*) NVMCTRL_0_Handler,           /*  5 Non-Volatile Memory Controller0 */
          (void*) NVMCTRL_1_Handler            /*  5 Non-Volatile Memory Controller0 */
        },
        union {
          (void*) EVSYS_EVD_0_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_EVD_1_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_EVD_2_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_EVD_3_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_EVD_4_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_EVD_5_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_EVD_6_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_EVD_7_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_OVR_0_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_OVR_1_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_OVR_2_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_OVR_3_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_OVR_4_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_OVR_5_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_OVR_6_Handler,         /*  6 Event System Interface0 */
          (void*) EVSYS_OVR_7_Handler          /*  6 Event System Interface0 */
        },
        union {
          (void*) SERCOM0_0_Handler,           /*  7 Serial Communication Interface0 */
          (void*) SERCOM0_1_Handler,           /*  7 Serial Communication Interface0 */
          (void*) SERCOM0_2_Handler,           /*  7 Serial Communication Interface0 */
          (void*) SERCOM0_3_Handler            /*  7 Serial Communication Interface0 */
        },
        union {
          (void*) SERCOM1_0_Handler,           /*  8 Serial Communication Interface0 */
          (void*) SERCOM1_1_Handler,           /*  8 Serial Communication Interface0 */
          (void*) SERCOM1_2_Handler,           /*  8 Serial Communication Interface0 */
          (void*) SERCOM1_3_Handler            /*  8 Serial Communication Interface0 */
        },
        union {
          (void*) SERCOM2_0_Handler,           /*  9 Serial Communication Interface0 */
          (void*) SERCOM2_1_Handler,           /*  9 Serial Communication Interface0 */
          (void*) SERCOM2_2_Handler,           /*  9 Serial Communication Interface0 */
          (void*) SERCOM2_3_Handler            /*  9 Serial Communication Interface0 */
        },
        union {
          (void*) SERCOM3_0_Handler,           /* 10 Serial Communication Interface0 */
          (void*) SERCOM3_1_Handler,           /* 10 Serial Communication Interface0 */
          (void*) SERCOM3_2_Handler,           /* 10 Serial Communication Interface0 */
          (void*) SERCOM3_3_Handler            /* 10 Serial Communication Interface0 */
        },
        union {
          (void*) SERCOM4_0_Handler,           /* 11 Serial Communication Interface0 */
          (void*) SERCOM4_1_Handler,           /* 11 Serial Communication Interface0 */
          (void*) SERCOM4_2_Handler,           /* 11 Serial Communication Interface0 */
          (void*) SERCOM4_3_Handler            /* 11 Serial Communication Interface0 */
        },
        union {
          (void*) SERCOM5_0_Handler,           /* 12 Serial Communication Interface0 */
          (void*) SERCOM5_1_Handler,           /* 12 Serial Communication Interface0 */
          (void*) SERCOM5_2_Handler,           /* 12 Serial Communication Interface0 */
          (void*) SERCOM5_3_Handler            /* 12 Serial Communication Interface0 */
        },
        union {
          (void*) TC0_ERR_Handler,             /* 13 Basic Timer Counter0 */
          (void*) TC0_MC_0_Handler,            /* 13 Basic Timer Counter0 */
          (void*) TC0_MC_1_Handler,            /* 13 Basic Timer Counter0 */
          (void*) TC0_OVF_Handler,             /* 13 Basic Timer Counter0 */
          (void*) TC0_READY_Handler            /* 13 Basic Timer Counter0 */
        },
        union {
          (void*) TC1_ERR_Handler,             /* 14 Basic Timer Counter0 */
          (void*) TC1_MC_0_Handler,            /* 14 Basic Timer Counter0 */
          (void*) TC1_MC_1_Handler,            /* 14 Basic Timer Counter0 */
          (void*) TC1_OVF_Handler,             /* 14 Basic Timer Counter0 */
          (void*) TC1_READY_Handler            /* 14 Basic Timer Counter0 */
        },
        union {
          (void*) TC2_ERR_Handler,             /* 15 Basic Timer Counter0 */
          (void*) TC2_MC_0_Handler,            /* 15 Basic Timer Counter0 */
          (void*) TC2_MC_1_Handler,            /* 15 Basic Timer Counter0 */
          (void*) TC2_OVF_Handler,             /* 15 Basic Timer Counter0 */
          (void*) TC2_READY_Handler            /* 15 Basic Timer Counter0 */
        },
        union {
          (void*) TC3_ERR_Handler,             /* 16 Basic Timer Counter0 */
          (void*) TC3_MC_0_Handler,            /* 16 Basic Timer Counter0 */
          (void*) TC3_MC_1_Handler,            /* 16 Basic Timer Counter0 */
          (void*) TC3_OVF_Handler,             /* 16 Basic Timer Counter0 */
          (void*) TC3_READY_Handler            /* 16 Basic Timer Counter0 */
        },
        union {
          (void*) TC4_ERR_Handler,             /* 17 Basic Timer Counter0 */
          (void*) TC4_MC_0_Handler,            /* 17 Basic Timer Counter0 */
          (void*) TC4_MC_1_Handler,            /* 17 Basic Timer Counter0 */
          (void*) TC4_OVF_Handler,             /* 17 Basic Timer Counter0 */
          (void*) TC4_READY_Handler            /* 17 Basic Timer Counter0 */
        },
        union {
          (void*) TC5_ERR_Handler,             /* 18 Basic Timer Counter0 */
          (void*) TC5_MC_0_Handler,            /* 18 Basic Timer Counter0 */
          (void*) TC5_MC_1_Handler,            /* 18 Basic Timer Counter0 */
          (void*) TC5_OVF_Handler,             /* 18 Basic Timer Counter0 */
          (void*) TC5_READY_Handler            /* 18 Basic Timer Counter0 */
        },
        union {
          (void*) TC6_ERR_Handler,             /* 19 Basic Timer Counter0 */
          (void*) TC6_MC_0_Handler,            /* 19 Basic Timer Counter0 */
          (void*) TC6_MC_1_Handler,            /* 19 Basic Timer Counter0 */
          (void*) TC6_OVF_Handler,             /* 19 Basic Timer Counter0 */
          (void*) TC6_READY_Handler            /* 19 Basic Timer Counter0 */
        },
        union {
          (void*) TC7_ERR_Handler,             /* 20 Basic Timer Counter0 */
          (void*) TC7_MC_0_Handler,            /* 20 Basic Timer Counter0 */
          (void*) TC7_MC_1_Handler,            /* 20 Basic Timer Counter0 */
          (void*) TC7_OVF_Handler,             /* 20 Basic Timer Counter0 */
          (void*) TC7_READY_Handler            /* 20 Basic Timer Counter0 */
        },
        union {
          (void*) ADC_OVERRUN_Handler,         /* 21 Analog Digital Converter0 */
          (void*) ADC_READY_Handler,           /* 21 Analog Digital Converter0 */
          (void*) ADC_RESRDY_Handler,          /* 21 Analog Digital Converter0 */
          (void*) ADC_WINMON_Handler           /* 21 Analog Digital Converter0 */
        },
        union {
          (void*) AC_COMP_0_Handler,           /* 22 Analog Comparators0 */
          (void*) AC_COMP_1_Handler,           /* 22 Analog Comparators0 */
          (void*) AC_WIN_0_Handler             /* 22 Analog Comparators0 */
        },
        union {
          (void*) DAC_EMPTY_Handler,           /* 23 Digital Analog Converter0 */
          (void*) DAC_READY_Handler,           /* 23 Digital Analog Converter0 */
          (void*) DAC_UNDERRUN_Handler         /* 23 Digital Analog Converter0 */
        },
        union {
          (void*) PTC_EOC_Handler,             /* 24 Peripheral Touch Controller0 */
          (void*) PTC_READY_Handler,           /* 24 Peripheral Touch Controller0 */
          (void*) PTC_WCOMP_Handler            /* 24 Peripheral Touch Controller0 */
        }
};

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
        uint32_t *pSrc = __section_begin(".intvec");

        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        if (((uint32_t) pSrc >= HRAMC0_ADDR) && ((uint32_t) pSrc < (uint32_t) HRAMC0_ADDR + (uint32_t) HRAMC0_SIZE)) {
                SCB->VTOR |= (1UL) << SCB_VTOR_TBLBASE_Pos;
        }

        return 1; /* if return 0, the data sections will not be initialized */
}

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
void Reset_Handler(void)
{
        __iar_program_start();
}
