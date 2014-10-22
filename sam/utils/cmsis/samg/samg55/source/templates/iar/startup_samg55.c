/**
 * \file
 *
 * \brief Startup file for SAMG55.
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

#include "samg55.h"

typedef void (*intfunc) (void);
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

void __iar_program_start(void);
int __low_level_init(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M4 core handlers */
#pragma weak NMI_Handler=Dummy_Handler
#pragma weak HardFault_Handler=Dummy_Handler
#pragma weak MemManage_Handler=Dummy_Handler
#pragma weak BusFault_Handler=Dummy_Handler
#pragma weak UsageFault_Handler=Dummy_Handler
#pragma weak SVC_Handler=Dummy_Handler
#pragma weak DebugMon_Handler=Dummy_Handler
#pragma weak PendSV_Handler=Dummy_Handler
#pragma weak SysTick_Handler=Dummy_Handler

/* Peripherals handlers */
#pragma weak SUPC_Handler=Dummy_Handler
#pragma weak RSTC_Handler=Dummy_Handler
#pragma weak RTC_Handler=Dummy_Handler
#pragma weak RTT_Handler=Dummy_Handler
#pragma weak WDT_Handler=Dummy_Handler
#pragma weak PMC_Handler=Dummy_Handler
#pragma weak EFC_Handler=Dummy_Handler
#ifdef _SAMG55_USART7_INSTANCE_
#pragma weak USART7_SPI7_TWI7_Handler=Dummy_Handler
#endif /* _SAMG55_USART7_INSTANCE_ */
#pragma weak USART0_SPI0_TWI0_Handler=Dummy_Handler
#pragma weak USART1_SPI1_TWI1_Handler=Dummy_Handler
#pragma weak PIOA_Handler=Dummy_Handler
#pragma weak PIOB_Handler=Dummy_Handler
#pragma weak PDMIC0_Handler=Dummy_Handler
#pragma weak USART2_SPI2_TWI2_Handler=Dummy_Handler
#pragma weak MEM2MEM_Handler=Dummy_Handler
#pragma weak I2SC0_Handler=Dummy_Handler
#pragma weak I2SC1_Handler=Dummy_Handler
#pragma weak PDMIC1_Handler=Dummy_Handler
#pragma weak USART3_SPI3_TWI3_Handler=Dummy_Handler
#pragma weak USART4_SPI4_TWI4_Handler=Dummy_Handler
#pragma weak USART5_SPI5_TWI5_Handler=Dummy_Handler
#pragma weak USART6_SPI6_TWI6_Handler=Dummy_Handler
#pragma weak TC0_Handler=Dummy_Handler
#pragma weak TC1_Handler=Dummy_Handler
#pragma weak TC2_Handler=Dummy_Handler
#pragma weak TC3_Handler=Dummy_Handler
#pragma weak TC4_Handler=Dummy_Handler
#pragma weak TC5_Handler=Dummy_Handler
#pragma weak ADC_Handler=Dummy_Handler
#pragma weak ARM_Handler=Dummy_Handler
#pragma weak WKUP0_Handler=Dummy_Handler
#pragma weak WKUP1_Handler=Dummy_Handler
#pragma weak WKUP2_Handler=Dummy_Handler
#pragma weak WKUP3_Handler=Dummy_Handler
#pragma weak WKUP4_Handler=Dummy_Handler
#pragma weak WKUP5_Handler=Dummy_Handler
#pragma weak WKUP6_Handler=Dummy_Handler
#pragma weak WKUP7_Handler=Dummy_Handler
#pragma weak WKUP8_Handler=Dummy_Handler
#pragma weak WKUP9_Handler=Dummy_Handler
#pragma weak WKUP10_Handler=Dummy_Handler
#pragma weak WKUP11_Handler=Dummy_Handler
#pragma weak WKUP12_Handler=Dummy_Handler
#pragma weak WKUP13_Handler=Dummy_Handler
#pragma weak WKUP14_Handler=Dummy_Handler
#pragma weak WKUP15_Handler=Dummy_Handler
#pragma weak UHP_Handler=Dummy_Handler
#pragma weak UDP_Handler=Dummy_Handler
#pragma weak CRCCU_Handler=Dummy_Handler

/* Exception Table */
#pragma language = extended
#pragma segment = "CSTACK"

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section = ".intvec"
#pragma location = ".intvec"
const DeviceVectors __vector_table = {
        (void*) __sfe("CSTACK"),
        
        .pfnReset_Handler      = (void*) Reset_Handler,
        .pfnNMI_Handler        = (void*) NMI_Handler,
        .pfnHardFault_Handler  = (void*) HardFault_Handler,
        .pfnMemManage_Handler  = (void*) MemManage_Handler,
        .pfnBusFault_Handler   = (void*) BusFault_Handler,
        .pfnUsageFault_Handler = (void*) UsageFault_Handler,
        .pfnReserved1_Handler  = (void*) (0UL),           /* Reserved */
        .pfnReserved2_Handler  = (void*) (0UL),           /* Reserved */
        .pfnReserved3_Handler  = (void*) (0UL),           /* Reserved */
        .pfnReserved4_Handler  = (void*) (0UL),           /* Reserved */
        .pfnSVC_Handler        = (void*) SVC_Handler,
        .pfnDebugMon_Handler   = (void*) DebugMon_Handler,
        .pfnReserved5_Handler  = (void*) (0UL),           /* Reserved */
        .pfnPendSV_Handler     = (void*) PendSV_Handler,
        .pfnSysTick_Handler    = (void*) SysTick_Handler,

        /* Configurable interrupts */
        .pfnSUPC_Handler    = (void*) SUPC_Handler,    /* 0  Supply Controller */
        .pfnRSTC_Handler    = (void*) RSTC_Handler,    /* 1  Reset Controller */
        .pfnRTC_Handler     = (void*) RTC_Handler,     /* 2  Real Time Clock */
        .pfnRTT_Handler     = (void*) RTT_Handler,     /* 3  Real Time Timer */
        .pfnWDT_Handler     = (void*) WDT_Handler,     /* 4  Watchdog Timer */
        .pfnPMC_Handler     = (void*) PMC_Handler,     /* 5  Power Management Controller */
        .pfnEFC_Handler     = (void*) EFC_Handler,     /* 6  Enhanced Flash Controller */
#ifdef _SAMG55_USART7_INSTANCE_
        .pfnUSART7_SPI7_TWI7_Handler  = (void*) USART7_SPI7_TWI7_Handler,  /* 7  FLEXCOM 7 */
#else
        .pvReserved7        = (void*) (0UL),           /* 7  Reserved */
#endif /* _SAMG55_USART7_INSTANCE_ */
        .pfnUSART0_SPI0_TWI0_Handler  = (void*) USART0_SPI0_TWI0_Handler,  /* 8  FLEXCOM 0 */
        .pfnUSART1_SPI1_TWI1_Handler  = (void*) USART1_SPI1_TWI1_Handler,  /* 9  FLEXCOM 1 */
        .pvReserved10       = (void*) (0UL),           /* 10 Reserved */
        .pfnPIOA_Handler    = (void*) PIOA_Handler,    /* 11 Parallel I/O Controller A */
        .pfnPIOB_Handler    = (void*) PIOB_Handler,    /* 12 Parallel I/O Controller B */
        .pfnPDMIC0_Handler  = (void*) PDMIC0_Handler,  /* 13 PDM 0 */
        .pfnUSART2_SPI2_TWI2_Handler  = (void*) USART2_SPI2_TWI2_Handler,  /* 14 FLEXCOM2 */
        .pfnMEM2MEM_Handler = (void*) MEM2MEM_Handler, /* 15 MEM2MEM */
        .pfnI2SC0_Handler   = (void*) I2SC0_Handler,   /* 16 I2SC0 */
        .pfnI2SC1_Handler   = (void*) I2SC1_Handler,   /* 17 I2SC1 */
        .pfnPDMIC1_Handler  = (void*) PDMIC1_Handler,  /* 18 PDM 1 */
        .pfnUSART3_SPI3_TWI3_Handler  = (void*) USART3_SPI3_TWI3_Handler,  /* 19 FLEXCOM3 */
        .pfnUSART4_SPI4_TWI4_Handler    = (void*) USART4_SPI4_TWI4_Handler,    /* 20 FLEXCOM4 */
        .pfnUSART5_SPI5_TWI5_Handler    = (void*) USART5_SPI5_TWI5_Handler,    /* 21 FLEXCOM5 */
        .pfnUSART6_SPI6_TWI6_Handler  = (void*) USART6_SPI6_TWI6_Handler,  /* 22 FLEXCOM6 */
        .pfnTC0_Handler     = (void*) TC0_Handler,     /* 23 Timer/Counter 0 */
        .pfnTC1_Handler     = (void*) TC1_Handler,     /* 24 Timer/Counter 1 */
        .pfnTC2_Handler     = (void*) TC2_Handler,     /* 25 Timer/Counter 2 */
        .pfnTC3_Handler     = (void*) TC3_Handler,     /* 26 Timer/Counter 3 */
        .pfnTC4_Handler     = (void*) TC4_Handler,     /* 27 Timer/Counter 4 */
        .pfnTC5_Handler     = (void*) TC5_Handler,     /* 28 Timer/Counter 5 */
        .pfnADC_Handler     = (void*) ADC_Handler,     /* 29 Analog To Digital Converter */
        .pfnARM_Handler     = (void*) ARM_Handler,     /* 30 FPU */
        .pfnWKUP0_Handler   = (void*) WKUP0_Handler,   /* 31 External interrupt 0 */
        .pfnWKUP1_Handler   = (void*) WKUP1_Handler,   /* 32 External interrupt 1 */
        .pfnWKUP2_Handler   = (void*) WKUP2_Handler,   /* 33 External interrupt 2 */
        .pfnWKUP3_Handler   = (void*) WKUP3_Handler,   /* 34 External interrupt 3 */
        .pfnWKUP4_Handler   = (void*) WKUP4_Handler,   /* 35 External interrupt 4 */
        .pfnWKUP5_Handler   = (void*) WKUP5_Handler,   /* 36 External interrupt 5 */
        .pfnWKUP6_Handler   = (void*) WKUP6_Handler,   /* 37 External interrupt 6 */
        .pfnWKUP7_Handler   = (void*) WKUP7_Handler,   /* 38 External interrupt 7 */
        .pfnWKUP8_Handler   = (void*) WKUP8_Handler,   /* 39 External interrupt 8 */
        .pfnWKUP9_Handler   = (void*) WKUP9_Handler,   /* 40 External interrupt 9 */
        .pfnWKUP10_Handler  = (void*) WKUP10_Handler,  /* 41 External interrupt 10 */
        .pfnWKUP11_Handler  = (void*) WKUP11_Handler,  /* 42 External interrupt 11 */
        .pfnWKUP12_Handler  = (void*) WKUP12_Handler,  /* 43 External interrupt 12 */
        .pfnWKUP13_Handler  = (void*) WKUP13_Handler,  /* 44 External interrupt 13 */
        .pfnWKUP14_Handler  = (void*) WKUP14_Handler,  /* 45 External interrupt 14 */
        .pfnWKUP15_Handler  = (void*) WKUP15_Handler,  /* 46 External interrupt 15 */
        .pfnUHP_Handler     = (void*) UHP_Handler,     /* 47 USB OHCI */
        .pfnUDP_Handler     = (void*) UDP_Handler,     /* 48 USB Device FS */
        .pfnCRCCU_Handler   = (void*) (CRCCU_Handler)  /* 49 CRCCU */
};

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
        uint32_t *pSrc = __section_begin(".intvec");

        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

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

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
        while (1) {
        }
}