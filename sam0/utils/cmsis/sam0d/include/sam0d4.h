/**
 * \file
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#ifndef _SAM0D4_
#define _SAM0D4_

#include <stdint.h>

typedef volatile const uint32_t RoReg; /**< Read only 32-bit register (volatile const unsigned int) */
typedef volatile       uint32_t WoReg; /**< Write only 32-bit register (volatile unsigned int) */
typedef volatile       uint32_t RwReg; /**< Read-Write 32-bit register (volatile unsigned int) */

typedef enum IRQn
{
  NonMaskableInt_IRQn   = -14, /**<  2 Non Maskable Interrupt                */
  MemoryManagement_IRQn = -12, /**<  4 Cortex-M0 Memory Management Interrupt */
  BusFault_IRQn         = -11, /**<  5 Cortex-M0 Bus Fault Interrupt         */
  UsageFault_IRQn       = -10, /**<  6 Cortex-M0 Usage Fault Interrupt       */
  SVCall_IRQn           = -5,  /**< 11 Cortex-M0 SV Call Interrupt           */
  DebugMonitor_IRQn     = -4,  /**< 12 Cortex-M0 Debug Monitor Interrupt     */
  PendSV_IRQn           = -2,  /**< 14 Cortex-M0 Pend SV Interrupt           */
  SysTick_IRQn          = -1,  /**< 15 Cortex-M0 System Tick Interrupt       */
} IRQn_Type;

#define __CM0_REV              0x0100
#define __MPU_PRESENT          0
#define __NVIC_PRIO_BITS       4

#include <core_cm0.h>

#include <component/component_wdt.h>
#include <component/component_gclk.h>
#include <component/component_evsys.h>
#include <component/component_sysctrl.h>
#include <component/component_pm.h>
#include <component/component_sercom.h>
#include <component/component_port.h>
#include <component/component_eic.h>
#include <component/component_rtc.h>
#include <component/component_pac.h>
#include <component/component_nvm.h>
#include <component/component_ac.h>
#include <component/component_dac.h>
#include <component/component_tc.h>

#include <instance/instance_wdt.h>
#include <instance/instance_gclk.h>
#include <instance/instance_evsys.h>
#include <instance/instance_sysctrl.h>
#include <instance/instance_pm.h>
#include <instance/instance_sercom.h>
#include <instance/instance_port.h>
#include <instance/instance_eic.h>
#include <instance/instance_rtc.h>
#include <instance/instance_pac.h>
#include <instance/instance_nvm.h>
#include <instance/instance_ac.h>
#include <instance/instance_dac.h>
#include <instance/instance_tc.h>

#endif /* _SAM0D4_ */
