/**
 * \file hal.h
 *
 * \brief  The HAL Layer forms the wrapper layer that forms the interface between the Stack and the Low level drivers.
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
 * Copyright (c) 2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifndef _HAL_H_
#define _HAL_H_


#include "sysTypes.h"
#include "common_hw_timer.h"

 /**
  * \defgroup group_lwmesh Light Weight Mesh 
  *     Atmel® Lightweight Mesh software stack is an easy to use proprietary low power wireless mesh network protocol. 
  *     It was designed to address the needs of a wide range of wireless connectivity applications. Some of these applications include:\n
  *   	• Remote control\n
  *   	• Alarms and security\n
  *   	• Automatic Meter Reading (AMR)\n
  *   	• Home and commercial building automation\n
  *   	• Toys and educational equipment\n
  * 
  */
  
/**
 * \ingroup group_lwmesh
 * \defgroup group_phy PHY 
 * Radio physical layer (PHY) provides functions for radio transceiver access. 
 * Some of them are accessible only by the network layer (request to send data, data indication); 
 * some of them can be used from the application (channel selection, random number generation, energy detection, etc.)
 *
 *
 */  

/**
 * \ingroup group_lwmesh
 * \defgroup group_lwmesh_nwk Network layer(NWK)
 *  Network layer (NWK) provides core stack functionality and provides two routing algorithms,1.Native routing and 2.AODV routing.
 * 
 */
 
/**
 * \ingroup group_lwmesh
 * \defgroup group_lwmesh_hal Hardware Abstraction Layer (HAL)
 * provides basic hardware dependent functionality, like hardware timer, GPIO access for the radio interface through ASF Drivers
 *
 * @{
 */
/*****************************************************************************
*****************************************************************************/
#define HAL_TIMER_INTERVAL      10ul // ms
#define MS 1000

/*****************************************************************************
*****************************************************************************/
/**
 * @brief Initialize the HAL module
 * This function initialize HAL timer unit
 */
void HAL_Init(void);

/**
 * @brief Initialize the HAL module
 * This function initialize HAL timer unit
 * @param[in] us time value
 */
void HAL_Delay(uint32_t us);

/**
 * @brief Triggers the sleep functionality
 * This function is called to trigger the TRX sleep for the specified interval
 * @param[in] interval time value 
 */
void HAL_Sleep(uint32_t interval);

/**
 * @brief Hardware timer expiry callback
 * This function is called after hardware timer is expired.
 */
void hal_hw_expiry_cb(void);

/* Enables the global interrupt */
#define ENABLE_GLOBAL_IRQ()                  Enable_global_interrupt()

/* Disables the global interrupt */
#define DISABLE_GLOBAL_IRQ()                 Disable_global_interrupt()

/* This macro saves the global interrupt status */
#define ENTER_CRITICAL_REGION()              {uint8_t flags = cpu_irq_save();

/* This macro restores the global interrupt status */
#define LEAVE_CRITICAL_REGION()              cpu_irq_restore(flags);}

/** @} */
#endif // _HAL_H_

