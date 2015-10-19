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

xQueueHandle xQueue = NULL;		/* ykk */
xTaskHandle hif_task_id;	/* ykk */
extern void winc_tx_from_queue(hif_msg_t *msg);		/* ykk */

extern uint8_t m2m_wifi_handle_events(void *p);
extern void pending_imp(void * pv);
//static xTaskHandle tcpip_task;
 xTaskHandle wifi_task;
static xSemaphoreHandle notify_sem;
static xSemaphoreHandle mutex;
static uint8_t has_init = 0;
/* ykk */
static void os_hif_task(void *pv)
{
	int deinit_pending = 0;
	while(1) {
		hif_msg_t msg;
		
		if (xQueueReceive(xQueue, (void *)&msg, portMAX_DELAY) != pdFALSE) {
			if (msg.id == MSG_START) {
				deinit_pending = 0;
				if (msg.handler) 
					msg.handler(msg.priv);				
			} else if (msg.id == MSG_QUIT) {
				deinit_pending = 1;
				
				if (msg.handler) 
					msg.handler(msg.priv);					
			} else if (msg.id == MSG_RX) {
				if (!deinit_pending)
					m2m_wifi_handle_events(NULL);
				else
					osprintf("Warning: disable INT before deinit \n");
			} else if ((msg.id == MSG_TX_STA) || (msg.id == MSG_TX_AP)) {
				if (!deinit_pending) {
					winc_tx_from_queue(&msg);
				} else {	
					/* free tx buf*/
					struct pbuf *p = msg.pbuf;
					osprintf("Warning: call net_interface_down before deinit \n");
					if (p)
						pbuf_free(p);
				}
			} else if (msg.id == MSG_CMD) {
				if (!deinit_pending) {
					if (msg.handler) 
						msg.handler(msg.priv);
				} else {
					osprintf("Warning: do not call wifi API after deinit \n");
					pending_imp(msg.priv);
				}
			} else {
				osprintf("Warning: Wrong id  msg id %d \r\n", msg.id);
			}
			
		}
	}
	osprintf("os_hif_task exit \r\n");
	vTaskDelete(NULL);
	return;
}

void os_hook_isr(void)
{
	/* ykk */
#if 0
	tcpip_callback_with_block((tcpip_callback_fn)m2m_wifi_handle_events, 0, 0);
#else
	signed portBASE_TYPE woken = pdFALSE;
	hif_msg_t msg;

	msg.id = MSG_RX;
	msg.pbuf = NULL;
	msg.payload_size = 0;
	msg.payload = NULL;
	if (xQueueIsQueueFullFromISR(xQueue))
		osprintf("queue full \n");
	xQueueSendFromISR(xQueue, &msg, &woken);
	if (xQueueIsQueueFullFromISR(xQueue))
		osprintf("queue full \n");
	portEND_SWITCHING_ISR(woken);
	
#endif
}

void os_hook_init(void)
{
	if (!has_init) {
		mutex = xSemaphoreCreateMutex();
		vSemaphoreCreateBinary(notify_sem);
		xSemaphoreTake(notify_sem, portMAX_DELAY);
		xQueue = xQueueCreate(32, sizeof(hif_msg_t));
		
		xTaskCreate(os_hif_task, (const signed char *)"wifi hif", 1024, NULL, (configMAX_PRIORITIES - 1), &hif_task_id);			
		has_init = 1;
	}
}

void os_hook_deinit(void)
{
	if (has_init) {
		hif_msg_t msg;
		msg.id = MSG_QUIT;
	
		xQueueSend(xQueue, (void *)&msg, portMAX_DELAY);
		vSemaphoreDelete(notify_sem);
		vSemaphoreDelete(mutex);	
		has_init = 0;
	}
}

void os_hook_set_handle(void *task)
{
	//tcpip_task = task;
	wifi_task = task;
}
#if 0
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
#endif
uint8_t os_hook_dispatch_no_wait(wifi_task_cb handler, void *p)
{
	hif_msg_t msg;
	if (wifi_task == xTaskGetCurrentTaskHandle()) {
		handler(p);
	} 
	else {
		msg.id = MSG_CMD;
		msg.handler = handler;
		msg.priv = p;
		return xQueueSend(xQueue, (void *)&msg, portMAX_DELAY);
	}
	return 0;
}
void os_hook_dispatch_wait(wifi_task_cb handler, struct params_dispatch *p, void* pv)
{
	hif_msg_t msg;
	if (wifi_task == xTaskGetCurrentTaskHandle()) {
		p->signal_semaphore = 0;
		handler(pv);
	}
	else {
		p->signal_semaphore = 1;
		xSemaphoreTake(mutex, portMAX_DELAY);
		//tcpip_callback_with_block(handler, p, 1);
		msg.id = MSG_CMD;
		msg.handler = handler;
		msg.priv = pv;
		xQueueSend(xQueue, (void *)&msg, portMAX_DELAY);
		
		xSemaphoreTake(notify_sem, portMAX_DELAY);
		xSemaphoreGive(mutex);
	}
}
void os_hook_send_start(wifi_task_cb handler, struct params_dispatch *p, void* pv)
{
	hif_msg_t msg;
 
	p->signal_semaphore = 1;
	xSemaphoreTake(mutex, portMAX_DELAY);
	//tcpip_callback_with_block(handler, p, 1);
	msg.id = MSG_START;
	msg.handler = handler;
	msg.priv = pv;
	xQueueSend(xQueue, (void *)&msg, portMAX_DELAY);
		
	xSemaphoreTake(notify_sem, portMAX_DELAY);
	xSemaphoreGive(mutex);
	
}
void os_hook_send_stop(wifi_task_cb handler, struct params_dispatch *p, void* pv)
{
	hif_msg_t msg;
	
	p->signal_semaphore = 1;
	xSemaphoreTake(mutex, portMAX_DELAY);
	//tcpip_callback_with_block(handler, p, 1);
	msg.id = MSG_QUIT;
	msg.handler = handler;
	msg.priv = pv;
	xQueueSend(xQueue, (void *)&msg, portMAX_DELAY);
	
	xSemaphoreTake(notify_sem, portMAX_DELAY);
	xSemaphoreGive(mutex);
	
}
void os_hook_notify(void)
{
	xSemaphoreGive(notify_sem);
}