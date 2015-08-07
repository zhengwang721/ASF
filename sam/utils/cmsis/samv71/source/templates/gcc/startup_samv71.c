/**
 * \file
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "samv71.h"

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

#define MPU_DEFAULT_ITCM_REGION           ( 1 )
#define MPU_DEFAULT_IFLASH_REGION         ( 2 )
#define MPU_DEFAULT_DTCM_REGION           ( 3 )
#define MPU_DEFAULT_PRAM_REGION           ( 4 )
#define MPU_DEFAULT_UPRAM_REGION          ( 5 )
#define MPU_PERIPHERALS_REGION            ( 6 )
#define MPU_DEFAULT_SDRAM_REGION          ( 7 )
#define MPU_USBHSRAM_REGION               ( 8 )
#define MPU_QSPIMEM_REGION                ( 9 )

#define MPU_ENABLE                      ( 0x1 )
#define MPU_BGENABLE                    ( 0x1 << 2 )
/************** USBHS_RAM region macros ******************/

#define USBHSRAM_START_ADDRESS                  0xA0100000UL
#define USBHSRAM_END_ADDRESS                    0xA0200000UL

/******* Ext-SRAM memory macros ***************************/

#define SDRAM_START_ADDRESS                     0x70000000UL
#define SDRAM_END_ADDRESS                       0x7FFFFFFFUL
/**************** DTCM  *******************************/
#define DTCM_START_ADDRESS                  0x20000000UL
#define DTCM_END_ADDRESS                    0x20400000UL


/******* SRAM memory macros ***************************/

#define SRAM_START_ADDRESS                  0x20400000UL
#define SRAM_END_ADDRESS                    0x2045FFFFUL
#define SRAM_HALF                           ((SRAM_END_ADDRESS - SRAM_START_ADDRESS) >>1)


#define SRAM_PRIVILEGE_START_ADDRESS        (SRAM_START_ADDRESS)
#define SRAM_PRIVILEGE_END_ADDRESS          (SRAM_START_ADDRESS + 0x3FFFF)

#define SRAM_UNPRIVILEGE_START_ADDRESS      (SRAM_PRIVILEGE_END_ADDRESS + 1)
#define SRAM_UNPRIVILEGE_END_ADDRESS        (SRAM_END_ADDRESS)

/************** Peripherials memory region macros ********/

#define PERIPHERALS_START_ADDRESS               0x40000000UL
#define PERIPHERALS_END_ADDRESS                 0x400E2000UL

#define UART_REGION_START_ADDRESS               0x400E1C00UL
#define UART_REGION_END_ADDRESS                 0x400E2000UL

/************** Peripherials memory region macros ********/

#define PERIPHERALS_START_ADDRESS               0x40000000UL
#define PERIPHERALS_END_ADDRESS                 0x400E2000UL
/********* IFLASH memory macros *********************/
#define ITCM_START_ADDRESS                  0x00000000UL
#define ITCM_END_ADDRESS                    0x00400000UL
#define IFLASH_START_ADDRESS                0x00400000UL
#define IFLASH_END_ADDRESS                  0x00600000UL
#define IFLASH_HALF                         ((IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) >> 1)

#define IFLASH_PRIVILEGE_START_ADDRESS      (IFLASH_START_ADDRESS)
#define IFLASH_PRIVILEGE_END_ADDRESS        (IFLASH_START_ADDRESS + IFLASH_HALF)

#define SRAM_UNCACHEABLE_START_ADDRESS      (SRAM_PRIVILEGE_END_ADDRESS + 1)
#define SRAM_UNCACHEABLE_END_ADDRESS        (SRAM_END_ADDRESS)

#define IFLASH_UNPRIVILEGE_START_ADDRESS    (IFLASH_START_ADDRESS + IFLASH_HALF + 1)
#define IFLASH_UNPRIVILEGE_END_ADDRESS      (IFLASH_END_ADDRESS)
/************** QSPI region macros ******************/

#define QSPI_START_ADDRESS                      0x80000000UL
#define QSPI_END_ADDRESS                        0x9fffffffUL


#define MPU_REGION_VALID                    ( 0x10 )
#define MPU_REGION_ENABLE                   ( 0x01 )
#define MPU_REGION_DISABLE                  ( 0x0  )

#define MPU_REGION_BUFFERABLE               ( 0x01 << 16 )
#define MPU_REGION_CACHEABLE                ( 0x01 << 17 )
#define MPU_REGION_SHAREABLE                ( 0x01 << 18 )

#define MPU_REGION_EXECUTE_NEVER            ( 0x01 << MPU_RASR_XN_Pos )

#define MPU_AP_NO_ACCESS                    ( 0x00 << MPU_RASR_AP_Pos )
#define MPU_AP_PRIVILEGED_READ_WRITE        ( 0x01 << MPU_RASR_AP_Pos )
#define MPU_AP_UNPRIVILEGED_READONLY        ( 0x02 << MPU_RASR_AP_Pos )
#define MPU_AP_FULL_ACCESS                  ( 0x03 << MPU_RASR_AP_Pos )
#define MPU_AP_RES                          ( 0x04 << MPU_RASR_AP_Pos )
#define MPU_AP_PRIVILEGED_READONLY          ( 0x05 << MPU_RASR_AP_Pos )
#define MPU_AP_READONLY                     ( 0x06 << MPU_RASR_AP_Pos )
#define MPU_AP_READONLY2                    ( 0x07 << MPU_RASR_AP_Pos )

#define MPU_TEX_B001                        ( 0x01 << MPU_RASR_TEX_Pos )
#define MPU_TEX_NON_CACHE                   ( 0x04 << MPU_RASR_TEX_Pos )
#define MPU_TEX_WRITE_BACK_ALLOCATE         ( 0x05 << MPU_RASR_TEX_Pos )
#define MPU_TEX_WRITE_THROUGH               ( 0x06 << MPU_RASR_TEX_Pos )
#define MPU_TEX_WRITE_BACK_NOALLOCATE       ( 0x07 << MPU_RASR_TEX_Pos )

#define DISABLE   0
#define ENABLE    1
#define DISABLED  0
#define ENABLED   1
#define OFF       0
#define ON        1
#define FALSE     0
#define TRUE      1
#define STRONGLY_ORDERED_SHAREABLE_TYPE     (( 0x00 << MPU_RASR_TEX_Pos ) | ( DISABLE << MPU_RASR_C_Pos ) | ( DISABLE << MPU_RASR_B_Pos ))     // DO not care //

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M7 core handlers */
void NMI_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MemManage_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMon_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void SUPC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RSTC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTT_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PMC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EFC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOA_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOB_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAMV71_PIOC_INSTANCE_
void PIOC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_PIOC_INSTANCE_ */
void USART0_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USART1_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USART2_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOD_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAMV71_PIOE_INSTANCE_
void PIOE_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_PIOE_INSTANCE_ */
void HSMCI_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TWIHS0_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TWIHS1_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SSC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAMV71_TC1_INSTANCE_
void TC3_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
void TC4_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
void TC5_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_TC1_INSTANCE_ */
void AFEC0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAMV71_DACC_INSTANCE_
void DACC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_DACC_INSTANCE_ */
void PWM0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ICM_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ACC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USBHS_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MCAN0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MCAN1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GMAC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AFEC1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAMV71_TWIHS2_INSTANCE_
void TWIHS2_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_TWIHS2_INSTANCE_ */
void SPI1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void QSPI_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART2_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART3_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART4_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAMV71_TC2_INSTANCE_
void TC6_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
void TC7_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
void TC8_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_TC2_INSTANCE_ */
void TC9_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC10_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC11_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MLB_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AES_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TRNG_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void XDMAC_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ISI_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PWM1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAMV71_SDRAMC_INSTANCE_
void SDRAMC_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAMV71_SDRAMC_INSTANCE_ */
void RSWDT_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

        /* Configure Initial Stack Pointer, using linker-generated symbols */
        .pvStack = (void*) (&_estack),

        .pfnReset_Handler      = (void*) Reset_Handler,
        .pfnNMI_Handler        = (void*) NMI_Handler,
        .pfnHardFault_Handler  = (void*) HardFault_Handler,
        .pfnMemManage_Handler  = (void*) MemManage_Handler,
        .pfnBusFault_Handler   = (void*) BusFault_Handler,
        .pfnUsageFault_Handler = (void*) UsageFault_Handler,
        .pfnReserved1_Handler  = (void*) (0UL),          /* Reserved */
        .pfnReserved2_Handler  = (void*) (0UL),          /* Reserved */
        .pfnReserved3_Handler  = (void*) (0UL),          /* Reserved */
        .pfnReserved4_Handler  = (void*) (0UL),          /* Reserved */
        .pfnSVC_Handler        = (void*) SVC_Handler,
        .pfnDebugMon_Handler   = (void*) DebugMon_Handler,
        .pfnReserved5_Handler  = (void*) (0UL),          /* Reserved */
        .pfnPendSV_Handler     = (void*) PendSV_Handler,
        .pfnSysTick_Handler    = (void*) SysTick_Handler,

        /* Configurable interrupts */
        .pfnSUPC_Handler   = (void*) SUPC_Handler,   /* 0  Supply Controller */
        .pfnRSTC_Handler   = (void*) RSTC_Handler,   /* 1  Reset Controller */
        .pfnRTC_Handler    = (void*) RTC_Handler,    /* 2  Real Time Clock */
        .pfnRTT_Handler    = (void*) RTT_Handler,    /* 3  Real Time Timer */
        .pfnWDT_Handler    = (void*) WDT_Handler,    /* 4  Watchdog Timer */
        .pfnPMC_Handler    = (void*) PMC_Handler,    /* 5  Power Management Controller */
        .pfnEFC_Handler    = (void*) EFC_Handler,    /* 6  Enhanced Embedded Flash Controller */
        .pfnUART0_Handler  = (void*) UART0_Handler,  /* 7  UART 0 */
        .pfnUART1_Handler  = (void*) UART1_Handler,  /* 8  UART 1 */
        .pvReserved9       = (void*) (0UL),          /* 9  Reserved */
        .pfnPIOA_Handler   = (void*) PIOA_Handler,   /* 10 Parallel I/O Controller A */
        .pfnPIOB_Handler   = (void*) PIOB_Handler,   /* 11 Parallel I/O Controller B */
#ifdef _SAMV71_PIOC_INSTANCE_
        .pfnPIOC_Handler   = (void*) PIOC_Handler,   /* 12 Parallel I/O Controller C */
#else
        .pvReserved12      = (void*) (0UL),          /* 12 Reserved */
#endif /* _SAMV71_PIOC_INSTANCE_ */
        .pfnUSART0_Handler = (void*) USART0_Handler, /* 13 USART 0 */
        .pfnUSART1_Handler = (void*) USART1_Handler, /* 14 USART 1 */
        .pfnUSART2_Handler = (void*) USART2_Handler, /* 15 USART 2 */
        .pfnPIOD_Handler   = (void*) PIOD_Handler,   /* 16 Parallel I/O Controller D */
#ifdef _SAMV71_PIOE_INSTANCE_
        .pfnPIOE_Handler   = (void*) PIOE_Handler,   /* 17 Parallel I/O Controller E */
#else
        .pvReserved17      = (void*) (0UL),          /* 17 Reserved */
#endif /* _SAMV71_PIOE_INSTANCE_ */
        .pfnHSMCI_Handler  = (void*) HSMCI_Handler,  /* 18 Multimedia Card Interface */
        .pfnTWIHS0_Handler = (void*) TWIHS0_Handler, /* 19 Two Wire Interface 0 HS */
        .pfnTWIHS1_Handler = (void*) TWIHS1_Handler, /* 20 Two Wire Interface 1 HS */
        .pfnSPI0_Handler   = (void*) SPI0_Handler,   /* 21 Serial Peripheral Interface 0 */
        .pfnSSC_Handler    = (void*) SSC_Handler,    /* 22 Synchronous Serial Controller */
        .pfnTC0_Handler    = (void*) TC0_Handler,    /* 23 Timer/Counter 0 */
        .pfnTC1_Handler    = (void*) TC1_Handler,    /* 24 Timer/Counter 1 */
        .pfnTC2_Handler    = (void*) TC2_Handler,    /* 25 Timer/Counter 2 */
#ifdef _SAMV71_TC1_INSTANCE_
        .pfnTC3_Handler    = (void*) TC3_Handler,    /* 26 Timer/Counter 3 */
#else
        .pvReserved26      = (void*) (0UL),          /* 26 Reserved */
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
        .pfnTC4_Handler    = (void*) TC4_Handler,    /* 27 Timer/Counter 4 */
#else
        .pvReserved27      = (void*) (0UL),          /* 27 Reserved */
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
        .pfnTC5_Handler    = (void*) TC5_Handler,    /* 28 Timer/Counter 5 */
#else
        .pvReserved28      = (void*) (0UL),          /* 28 Reserved */
#endif /* _SAMV71_TC1_INSTANCE_ */
        .pfnAFEC0_Handler  = (void*) AFEC0_Handler,  /* 29 Analog Front End 0 */
#ifdef _SAMV71_DACC_INSTANCE_
        .pfnDACC_Handler   = (void*) DACC_Handler,   /* 30 Digital To Analog Converter */
#else
        .pvReserved30      = (void*) (0UL),          /* 30 Reserved */
#endif /* _SAMV71_DACC_INSTANCE_ */
        .pfnPWM0_Handler   = (void*) PWM0_Handler,   /* 31 Pulse Width Modulation 0 */
        .pfnICM_Handler    = (void*) ICM_Handler,    /* 32 Integrity Check Monitor */
        .pfnACC_Handler    = (void*) ACC_Handler,    /* 33 Analog Comparator */
        .pfnUSBHS_Handler  = (void*) USBHS_Handler,  /* 34 USB Host / Device Controller */
        .pfnMCAN0_Handler  = (void*) MCAN0_Handler,  /* 35 MCAN Controller 0 */
        .pvReserved36      = (void*) (0UL),          /* 36 Reserved */
        .pfnMCAN1_Handler  = (void*) MCAN1_Handler,  /* 37 MCAN Controller 1 */
        .pvReserved38      = (void*) (0UL),          /* 38 Reserved */
        .pfnGMAC_Handler   = (void*) GMAC_Handler,   /* 39 Ethernet MAC */
        .pfnAFEC1_Handler  = (void*) AFEC1_Handler,  /* 40 Analog Front End 1 */
#ifdef _SAMV71_TWIHS2_INSTANCE_
        .pfnTWIHS2_Handler = (void*) TWIHS2_Handler, /* 41 Two Wire Interface 2 HS */
#else
        .pvReserved41      = (void*) (0UL),          /* 41 Reserved */
#endif /* _SAMV71_TWIHS2_INSTANCE_ */
        .pfnSPI1_Handler   = (void*) SPI1_Handler,   /* 42 Serial Peripheral Interface 1 */
        .pfnQSPI_Handler   = (void*) QSPI_Handler,   /* 43 Quad I/O Serial Peripheral Interface */
        .pfnUART2_Handler  = (void*) UART2_Handler,  /* 44 UART 2 */
        .pfnUART3_Handler  = (void*) UART3_Handler,  /* 45 UART 3 */
        .pfnUART4_Handler  = (void*) UART4_Handler,  /* 46 UART 4 */
#ifdef _SAMV71_TC2_INSTANCE_
        .pfnTC6_Handler    = (void*) TC6_Handler,    /* 47 Timer/Counter 6 */
#else
        .pvReserved47      = (void*) (0UL),          /* 47 Reserved */
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
        .pfnTC7_Handler    = (void*) TC7_Handler,    /* 48 Timer/Counter 7 */
#else
        .pvReserved48      = (void*) (0UL),          /* 48 Reserved */
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
        .pfnTC8_Handler    = (void*) TC8_Handler,    /* 49 Timer/Counter 8 */
#else
        .pvReserved49      = (void*) (0UL),          /* 49 Reserved */
#endif /* _SAMV71_TC2_INSTANCE_ */
        .pfnTC9_Handler    = (void*) TC9_Handler,    /* 50 Timer/Counter 9 */
        .pfnTC10_Handler   = (void*) TC10_Handler,   /* 51 Timer/Counter 10 */
        .pfnTC11_Handler   = (void*) TC11_Handler,   /* 52 Timer/Counter 11 */
        .pfnMLB_Handler    = (void*) MLB_Handler,    /* 53 MediaLB */
        .pvReserved54      = (void*) (0UL),          /* 54 Reserved */
        .pvReserved55      = (void*) (0UL),          /* 55 Reserved */
        .pfnAES_Handler    = (void*) AES_Handler,    /* 56 AES */
        .pfnTRNG_Handler   = (void*) TRNG_Handler,   /* 57 True Random Generator */
        .pfnXDMAC_Handler  = (void*) XDMAC_Handler,  /* 58 DMA */
        .pfnISI_Handler    = (void*) ISI_Handler,    /* 59 Camera Interface */
        .pfnPWM1_Handler   = (void*) PWM1_Handler,   /* 60 Pulse Width Modulation 1 */
        .pvReserved61      = (void*) (0UL),          /* 61 Reserved */
#ifdef _SAMV71_SDRAMC_INSTANCE_
        .pfnSDRAMC_Handler = (void*) SDRAMC_Handler, /* 62 SDRAM Controller */
#else
        .pvReserved62      = (void*) (0UL),          /* 62 Reserved */
#endif /* _SAMV71_SDRAMC_INSTANCE_ */
        .pfnRSWDT_Handler  = (void*) RSWDT_Handler   /* 63 Reinforced Secure Watchdog Timer */
};

/**
 * \brief Calculate region size for the RASR.
 */
uint32_t MPU_CalMPURegionSize( uint32_t dwActualSizeInBytes )
{
    uint32_t dwRegionSize = 32;
    uint32_t dwReturnValue = 4;

    while( dwReturnValue < 31 )
    {
        if( dwActualSizeInBytes <= dwRegionSize )
        {
            break;
        }
        else
        {
            dwReturnValue++;
        }

        dwRegionSize <<= 1;
    }

    return ( dwReturnValue << 1 );
}

void MPU_SetRegion( uint32_t dwRegionBaseAddr, uint32_t dwRegionAttr )
{
    *(unsigned int *)0xe000ed9c = dwRegionBaseAddr;//MPU->RBAR
    *(unsigned int *)0xe000eda0 = dwRegionAttr;//MPU->RASR
}
void MPU_Enable( uint32_t dwMPUEnable )
{
    *(unsigned int *)0xe000ed94 = dwMPUEnable ;//MPU->CTRL
}

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
        uint32_t *pSrc, *pDest;
        uint32_t dwRegionBaseAddr;
        uint32_t dwRegionAttr;

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

#if 1
		__DMB();
		dwRegionBaseAddr =
        QSPI_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_QSPIMEM_REGION;              //8

		dwRegionAttr =
			MPU_AP_FULL_ACCESS |
			STRONGLY_ORDERED_SHAREABLE_TYPE |
			//MPU_REGION_CACHEABLE |
			MPU_REGION_SHAREABLE |
			MPU_CalMPURegionSize(QSPI_END_ADDRESS - QSPI_START_ADDRESS) |
			MPU_REGION_ENABLE;

		MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

		/* Enable the memory management fault , Bus Fault, Usage Fault exception */
		SCB->SHCSR |= (SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk);

		/* Enable the MPU region */
		MPU_Enable( MPU_ENABLE | MPU_BGENABLE );

		__DSB();
		__ISB();

#endif

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