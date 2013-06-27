/**
 * \file
 *
 * \brief lwIP core & application threads configuration file.
 *
 * This file contains the possible external configuration of the Ethernet module.
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
 ***************************************************************************/

#ifndef CONF_LWIP_THREADS_H_INCLUDED
#define CONF_LWIP_THREADS_H_INCLUDED

#include "board.h"

/** The stack sizes allocated to the netif stack: (1048 * 4) = 4096 bytes. */
#define netifINTERFACE_TASK_STACK_SIZE    1024

/** The priority of the netif stack. */
#define netifINTERFACE_TASK_PRIORITY      (tskIDLE_PRIORITY + 5)

/** The stack sizes allocated to the TCPIP stack: (1048 * 4) = 4096 bytes. */
#define TCPIP_THREAD_STACKSIZE            1024

/** The priority of the TCPIP stack. */
#define TCPIP_THREAD_PRIO                 (tskIDLE_PRIORITY + 4)

/** Number of threads that can be started with sys_thread_new() */
#define SYS_THREAD_MAX                    8

/** LED used by the ethernet task, toggled on each activation */
#define webCONN_LED                       LED1_GPIO

/** FIXME: these are mandatory, else CPU will hang... WHY??? */
#define TCPIP_MBOX_SIZE                 16
#define DEFAULT_ACCEPTMBOX_SIZE         16
#define DEFAULT_RAW_RECVMBOX_SIZE       16
#define DEFAULT_TCP_RECVMBOX_SIZE       16

#endif /* #ifndef CONF_LWIP_THREADS_H_INCLUDED */
