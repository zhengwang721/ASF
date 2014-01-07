/**
 * \file hal.h
 *
 * \brief ATmega256rfr2 HAL interface
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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
 *
 */

/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifndef _HAL_H_
#define _HAL_H_


#include "sysTypes.h"
#include "common_hw_timer.h"
//#include "sysclk.h"





/*****************************************************************************
*****************************************************************************/
#define HAL_TIMER_INTERVAL      10ul // ms
#define MS 1000

/*****************************************************************************
*****************************************************************************/
void HAL_Init(void);
void HAL_Delay(uint32_t us);
void HAL_Sleep(uint32_t interval);
void hw_expiry_cb(void);
void HAL_PhyReset(void);

/* Enables the global interrupt */
#define ENABLE_GLOBAL_IRQ()                  Enable_global_interrupt()

/* Disables the global interrupt */
#define DISABLE_GLOBAL_IRQ()                 Disable_global_interrupt()

/* This macro saves the global interrupt status */
#define ENTER_CRITICAL_REGION()              {uint8_t flags = cpu_irq_save();

/* This macro restores the global interrupt status */
#define LEAVE_CRITICAL_REGION()              cpu_irq_restore(flags);}

#endif // _HAL_H_

