/**
 * \file
 *
 * \brief This file contains the default exception handlers.
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

#include "exceptions.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

#ifdef __GNUC__
/* Cortex-M4 core handlers */
void Reset_Handler      (void  ) __attribute__ ((weak, alias("Dummy_Handler")));
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
void SMC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOA_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOB_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOD_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOE_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USART0_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USART1_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HSMCI_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TWI0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TWI1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC8_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AFEC0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AFEC1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DACC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ACC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ARM_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UDP_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PWM_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AES_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GMAC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* __GNUC__ */

#ifdef __ICCARM__
/* Cortex-M4 core handlers */
#pragma weak Reset_Handler=Reset_Handlerz
#pragma weak NMI_Handler=NMI_Handlerz
#pragma weak HardFault_Handler=HardFault_Handlerz
#pragma weak MemManage_Handler=MemManage_Handlerz
#pragma weak BusFault_Handler=BusFault_Handlerz
#pragma weak UsageFault_Handler=UsageFault_Handlerz
#pragma weak SVC_Handler=SVC_Handlerz
#pragma weak DebugMon_Handler=DebugMon_Handlerz
#pragma weak PendSV_Handler=PendSV_Handlerz
#pragma weak SysTick_Handler=SysTick_Handlerz

/* Peripherals handlers */
#pragma weak SUPC_Handler=SUPC_Handlerz
#pragma weak RSTC_Handler=RSTC_Handlerz
#pragma weak RTC_Handler=RTC_Handlerz
#pragma weak RTT_Handler=RTT_Handlerz
#pragma weak WDT_Handler=WDT_Handlerz
#pragma weak PMC_Handler=PMC_Handlerz
#pragma weak EFC_Handler=EFC_Handlerz
#pragma weak UART0_Handler=UART0_Handlerz
#pragma weak SMC_Handler=SMC_Handlerz
#pragma weak PIOA_Handler=PIOA_Handlerz
#pragma weak PIOB_Handler=PIOB_Handlerz
#pragma weak PIOC_Handler=PIOC_Handlerz
#pragma weak PIOD_Handler=PIOD_Handlerz
#pragma weak PIOE_Handler=PIOE_Handlerz
#pragma weak USART0_Handler=USART0_Handlerz
#pragma weak USART1_Handler=USART1_Handlerz
#pragma weak HSMCI_Handler=HSMCI_Handlerz
#pragma weak TWI0_Handler=TWI0_Handlerz
#pragma weak TWI1_Handler=TWI1_Handlerz
#pragma weak SPI_Handler=SPI_Handlerz
#pragma weak DMAC_Handler=DMAC_Handlerz
#pragma weak TC0_Handler=TC0_Handlerz
#pragma weak TC1_Handler=TC1_Handlerz
#pragma weak TC2_Handler=TC2_Handlerz
#pragma weak TC3_Handler=TC3_Handlerz
#pragma weak TC4_Handler=TC4_Handlerz
#pragma weak TC5_Handler=TC5_Handlerz
#pragma weak TC6_Handler=TC6_Handlerz
#pragma weak TC7_Handler=TC7_Handlerz
#pragma weak TC8_Handler=TC8_Handlerz
#pragma weak AFEC0_Handler=AFEC0_Handlerz
#pragma weak AFEC1_Handler=AFEC1_Handlerz
#pragma weak DACC_Handler=DACC_Handlerz
#pragma weak ACC_Handler=ACC_Handlerz
#pragma weak ARM_Handler=ARM_Handlerz
#pragma weak UDP_Handler=UDP_Handlerz
#pragma weak PWM_Handler=PWM_Handlerz
#pragma weak CAN0_Handler=CAN0_Handlerz
#pragma weak CAN1_Handler=CAN1_Handlerz
#pragma weak AES_Handler=AES_Handlerz
#pragma weak GMAC_Handler=GMAC_Handlerz
#pragma weak UART1_Handler=UART1_Handlerz
#endif /* __ICCARM__ */

void Reset_Handlerz(void) {while(1);}
void NMI_Handlerz(void) {while(1);}
void HardFault_Handlerz(void) {while(1);}
void MemManage_Handlerz(void) {while(1);}
void BusFault_Handlerz(void) {while(1);}
void UsageFault_Handlerz(void) {while(1);}
void SVC_Handlerz(void) {while(1);}
void DebugMon_Handlerz(void) {while(1);}
void PendSV_Handlerz(void) {while(1);}
void SysTick_Handlerz(void) {while(1);}

/* Peripherals handlers */
void SUPC_Handlerz(void) {while(1);}
void RSTC_Handlerz(void) {while(1);}
void RTC_Handlerz(void) {while(1);}
void RTT_Handlerz(void) {while(1);}
void WDT_Handlerz(void) {while(1);}
void PMC_Handlerz(void) {while(1);}
void EFC_Handlerz(void) {while(1);}
void UART0_Handlerz(void) {while(1);}
void SMC_Handlerz(void) {while(1);}
void PIOA_Handlerz(void) {while(1);}
void PIOB_Handlerz(void) {while(1);}
void PIOC_Handlerz(void) {while(1);}
void PIOD_Handlerz(void) {while(1);}
void PIOE_Handlerz(void) {while(1);}
void USART0_Handlerz(void) {while(1);}
void USART1_Handlerz(void) {while(1);}
void HSMCI_Handlerz(void) {while(1);}
void TWI0_Handlerz(void) {while(1);}
void TWI1_Handlerz(void) {while(1);}
void SPI_Handlerz(void) {while(1);}
void DMAC_Handlerz(void) {while(1);}
void TC0_Handlerz(void) {while(1);}
void TC1_Handlerz(void) {while(1);}
void TC2_Handlerz(void) {while(1);}
void TC3_Handlerz(void) {while(1);}
void TC4_Handlerz(void) {while(1);}
void TC5_Handlerz(void) {while(1);}
void TC6_Handlerz(void) {while(1);}
void TC7_Handlerz(void) {while(1);}
void TC8_Handlerz(void) {while(1);}
void AFEC0_Handlerz(void) {while(1);}
void AFEC1_Handlerz(void) {while(1);}
void DACC_Handlerz(void) {while(1);}
void ACC_Handlerz(void) {while(1);}
void ARM_Handlerz(void) {while(1);}
void UDP_Handlerz(void) {while(1);}
void PWM_Handlerz(void) {while(1);}
void CAN0_Handlerz(void) {while(1);}
void CAN1_Handlerz(void) {while(1);}
void AES_Handlerz(void) {while(1);}
void GMAC_Handlerz(void) {while(1);}
void UART1_Handlerz(void) {while(1);}

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
