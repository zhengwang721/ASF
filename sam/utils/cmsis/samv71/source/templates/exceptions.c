/**
 * \file
 *
 * \brief This file contains the default exception handlers.
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
 * \par Purpose
 *
 * This file provides basic support for Cortex-M processor based
 * microcontrollers.
 *
 * \note
 * The exception handler has weak aliases.
 * As they are weak aliases, any function with the same name will override
 * this definition.
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "exceptions.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

#ifdef __GNUC__
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
#endif /* __GNUC__ */

#ifdef __ICCARM__
/* Cortex-M7 core handlers */
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
#pragma weak UART0_Handler=Dummy_Handler
#pragma weak UART1_Handler=Dummy_Handler
#pragma weak PIOA_Handler=Dummy_Handler
#pragma weak PIOB_Handler=Dummy_Handler
#ifdef _SAMV71_PIOC_INSTANCE_
#pragma weak PIOC_Handler=Dummy_Handler
#endif /* _SAMV71_PIOC_INSTANCE_ */
#pragma weak USART0_Handler=Dummy_Handler
#pragma weak USART1_Handler=Dummy_Handler
#pragma weak USART2_Handler=Dummy_Handler
#pragma weak PIOD_Handler=Dummy_Handler
#ifdef _SAMV71_PIOE_INSTANCE_
#pragma weak PIOE_Handler=Dummy_Handler
#endif /* _SAMV71_PIOE_INSTANCE_ */
#pragma weak HSMCI_Handler=Dummy_Handler
#pragma weak TWIHS0_Handler=Dummy_Handler
#pragma weak TWIHS1_Handler=Dummy_Handler
#pragma weak SPI0_Handler=Dummy_Handler
#pragma weak SSC_Handler=Dummy_Handler
#pragma weak TC0_Handler=Dummy_Handler
#pragma weak TC1_Handler=Dummy_Handler
#pragma weak TC2_Handler=Dummy_Handler
#ifdef _SAMV71_TC1_INSTANCE_
#pragma weak TC3_Handler=Dummy_Handler
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
#pragma weak TC4_Handler=Dummy_Handler
#endif /* _SAMV71_TC1_INSTANCE_ */
#ifdef _SAMV71_TC1_INSTANCE_
#pragma weak TC5_Handler=Dummy_Handler
#endif /* _SAMV71_TC1_INSTANCE_ */
#pragma weak AFEC0_Handler=Dummy_Handler
#ifdef _SAMV71_DACC_INSTANCE_
#pragma weak DACC_Handler=Dummy_Handler
#endif /* _SAMV71_DACC_INSTANCE_ */
#pragma weak PWM0_Handler=Dummy_Handler
#pragma weak ICM_Handler=Dummy_Handler
#pragma weak ACC_Handler=Dummy_Handler
#pragma weak USBHS_Handler=Dummy_Handler
#pragma weak MCAN0_Handler=Dummy_Handler
#pragma weak MCAN1_Handler=Dummy_Handler
#pragma weak GMAC_Handler=Dummy_Handler
#pragma weak AFEC1_Handler=Dummy_Handler
#ifdef _SAMV71_TWIHS2_INSTANCE_
#pragma weak TWIHS2_Handler=Dummy_Handler
#endif /* _SAMV71_TWIHS2_INSTANCE_ */
#pragma weak SPI1_Handler=Dummy_Handler
#pragma weak QSPI_Handler=Dummy_Handler
#pragma weak UART2_Handler=Dummy_Handler
#pragma weak UART3_Handler=Dummy_Handler
#pragma weak UART4_Handler=Dummy_Handler
#ifdef _SAMV71_TC2_INSTANCE_
#pragma weak TC6_Handler=Dummy_Handler
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
#pragma weak TC7_Handler=Dummy_Handler
#endif /* _SAMV71_TC2_INSTANCE_ */
#ifdef _SAMV71_TC2_INSTANCE_
#pragma weak TC8_Handler=Dummy_Handler
#endif /* _SAMV71_TC2_INSTANCE_ */
#pragma weak TC9_Handler=Dummy_Handler
#pragma weak TC10_Handler=Dummy_Handler
#pragma weak TC11_Handler=Dummy_Handler
#pragma weak MLB_Handler=Dummy_Handler
#pragma weak AES_Handler=Dummy_Handler
#pragma weak TRNG_Handler=Dummy_Handler
#pragma weak XDMAC_Handler=Dummy_Handler
#pragma weak ISI_Handler=Dummy_Handler
#pragma weak PWM1_Handler=Dummy_Handler
#ifdef _SAMV71_SDRAMC_INSTANCE_
#pragma weak SDRAMC_Handler=Dummy_Handler
#endif /* _SAMV71_SDRAMC_INSTANCE_ */
#pragma weak RSWDT_Handler=Dummy_Handler
#endif /* __ICCARM__ */

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
	while (1) {
	}
}

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
