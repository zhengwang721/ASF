/**
 *
 * \file
 *
 * \brief
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

#include <os/include/os_hook.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <lwip/tcpip.h>

extern uint8_t m2m_wifi_handle_events(void *p);
static xTaskHandle tcpip_task;
static xSemaphoreHandle notify_sem;
static xSemaphoreHandle mutex;

void os_hook_isr(void)
{
	tcpip_callback_with_block((tcpip_callback_fn)m2m_wifi_handle_events, 0, 0);
}

void os_hook_init(void)
{
	mutex = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(notify_sem);
	xSemaphoreTake(notify_sem, portMAX_DELAY);
}

void os_hook_set_handle(void *task)
{
	tcpip_task = task;
}

uint8_t os_hook_dispatch_no_wait(tcpip_cb handler, void *p)
{
	if (tcpip_task == xTaskGetCurrentTaskHandle()) {
		handler(p);
	} 
	else {
		return tcpip_callback_with_block(handler, p, 0);
	}
	return 0;
}

void os_hook_dispatch_wait(tcpip_cb handler, struct params_dispatch *p)
{
	if (tcpip_task == xTaskGetCurrentTaskHandle()) {
		p->signal_semaphore = 0;
		handler(p);
	} 
	else {
		p->signal_semaphore = 1;
		xSemaphoreTake(mutex, portMAX_DELAY);
		tcpip_callback_with_block(handler, p, 1);
		xSemaphoreTake(notify_sem, portMAX_DELAY);
		xSemaphoreGive(mutex);
	}
}

void os_hook_notify(void)
{
	xSemaphoreGive(notify_sem);
}