/**
 * \file
 *
 * \brief SAMB11 Platform Driver
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
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
#include <stdint.h>
#include <string.h>

#include "samb11.h"

#include "platform.h"

#include "ke_msg.h"
#include "gpio.h"
#include "common.h"
#include "event_handler.h"

/*
#include "CMSDK_CM0.h"


#include "uart_hw.h"

*/


uint8_t (*platform_register_isr)(uint8_t isr_index,void *fp);
uint8_t (*platform_unregister_isr)(uint8_t isr_index);

//#ifdef CHIPVERSION_B0
void (*handle_ext_wakeup_isr)(void);
//#endif	//CHIPVERSION_B0

#ifdef CHIPVERSION_A4
uint8_t register_isr(uint8_t isr_index,void *fp);
uint8_t unregister_isr(uint8_t isr_index);
#endif	//CHIPVERSION_A4

//#define TASK_INTERNAL_APP  62
#define MAX_BLE_EVT_LEN						512
#define MAX_PLF_EVT_LEN						128

#define BLE_EVENT_BUFFER_START_INDEX		0
#define PLF_EVENT_BUFFER_START_INDEX		(BLE_EVENT_BUFFER_START_INDEX + MAX_BLE_EVT_LEN)
//#define PLF_EVENT_BUFFER_START_INDEX		128

#define MAX_EVT_BUFF_LEN 	(MAX_BLE_EVT_LEN + MAX_PLF_EVT_LEN) 
#define REG_PL_WR(addr, value)       (*(volatile uint32_t *)(addr)) = (value)
#define REG_PL_RD(addr)              (*(volatile uint32_t *)(addr))

static void* gstrFwSem;
static void* InternalAppMsgQHandle;
static void (*ke_msg_send)(void const *param_ptr);
static void* (*ke_msg_alloc)(ke_msg_id_t const id, ke_task_id_t const dest_id,
	ke_task_id_t const src_id, uint16_t const param_len);
static int (*os_sem_up)(void* pstrSem);
static int (*NMI_MsgQueueRecv)(void* pHandle,void ** pvRecvBuffer);
static void (*ke_free)(void* mem_ptr);
static ke_task_id_t (* gapm_get_task_from_id)(ke_msg_id_t id);
static ke_task_id_t (* gapm_get_id_from_task)(ke_msg_id_t id);
volatile uint8_t platform_initialized = 0;

static platform_interface_callback ble_stack_message_handler;
uint8_t rx_buffer[MAX_EVT_BUFF_LEN];
uint16_t plf_event_buff_index;
#define ISR_RAM_MAP_START_ADDRESS	(0x10000000)
#define VECTOR_TABLE_LAST_INDEX		47

port port_list[LPGPIO_MAX];



//************************ start ******************************************//
//chris.choi : should ask china's driver team that it can be used like this
// this code is come from keil driver version code
//************************ start ******************************************//
#ifdef CHIPVERSION_B0
	#define UART1_RX_VECTOR_TABLE_INDEX		16
	#define UART1_TX_VECTOR_TABLE_INDEX		17
	#define UART2_RX_VECTOR_TABLE_INDEX		18
	#define UART2_TX_VECTOR_TABLE_INDEX		19
#else
	#define UART1_RX_VECTOR_TABLE_INDEX		16
	#define UART1_TX_VECTOR_TABLE_INDEX		17
	#define UART2_RX_VECTOR_TABLE_INDEX		18
	#define UART2_TX_VECTOR_TABLE_INDEX		19
#endif	//CHIPVERSION_B0


#ifdef CHIPVERSION_B0
void PORT1_COMB_Handler(void)
{
	//if(CMSDK_GPIO1->INTSTATUS & ((1<<15) | (1<<14) | (1<<13))) {
	if(GPIO1->INTSTATUSCLEAR.reg & ((1<<15) | (1<<14) | (1<<13))) {		
		
		handle_ext_wakeup_isr();
		
		// clear specific int pin status that caused the Interrupt
		//CMSDK_GPIO1->INTCLEAR |= CMSDK_GPIO1->INTSTATUS & ((1<<15) | (1<<14) | (1<<13));
		GPIO1->INTSTATUSCLEAR.reg |= GPIO1->INTSTATUSCLEAR.reg & ((1<<15) | (1<<14) | (1<<13));
		//NVIC_ClearPendingIRQ(PORT1_COMB_IRQn);
		NVIC_ClearPendingIRQ(8);
	}
	else
	{
		gpio1_combined_isr_handler();
	}
}

#endif	//CHIPVERSION_B0



// chris.choi : jeffy's code 150908, so it's better code i think so use this
void gpio1_combined_isr_handler(void)
{
	uint8_t index = 0;
	//portint_callback_t callback;
	uint8_t port = LPGPIO_16;
	uint32_t intstatus = GPIO1->INTSTATUSCLEAR.reg;//jeffy
	GPIO1->INTTYPESET.reg |= (1<<15); //jeffy
	
	do {
		#ifdef CHIPVERSION_B0
		intstatus &= 0x1FF;
		#else
		intstatus &= 0x1F;
		#endif
		index = 0;
		if(intstatus != 0) {
			port = (intstatus & ~(intstatus - 1));
			while(!(port == 1)) {
				port = port >> 1;
				index++;
			}
			
			//chris.choi : i think that port_int_callback is always null but if not, then it makes error because callback is not called
			//if(port_int_callback[index+LPGPIO_16] != NULL) {
			//callback = port_int_callback[index+LPGPIO_16];
			//callback();
			//}
			//CMSDK_GPIO1->INTCLEAR = (1 << index);
			///REG_GPIO1_INTSTATUSCLEAR = (1 << index);
			GPIO1->INTSTATUSCLEAR.reg = (1 << index);/// jeffy
		}
		else {
			//CMSDK_GPIO1->INTCLEAR = CMSDK_GPIO1->INTSTATUS;
			GPIO1->INTSTATUSCLEAR.reg |= (1<<15);//jeffy
			//REG_GPIO1_INTSTATUSCLEAR = REG_GPIO1_INTENSET;
		}
		intstatus = GPIO1->INTSTATUSCLEAR.reg;
		#ifdef CHIPVERSION_B0
		intstatus &= 0x1FF;
		#else
		intstatus &= 0x1F;
		#endif
	}while(intstatus != 0);
	
	NVIC_ClearPendingIRQ(8);
}


void gpio0_combined_isr_handler(void)
{
	uint8_t index = 0;
	//portint_callback_t callback;
	uint8_t port = LPGPIO_0;
	uint32_t intstatus = GPIO0->INTSTATUSCLEAR.reg;//jeffy
	GPIO0->INTTYPESET.reg |= (1<<15); //jeffy
	
	do {
		#ifdef CHIPVERSION_B0
		intstatus &= 0x1FF;
		#else
		intstatus &= 0x1F;
		#endif
		index = 0;
		if(intstatus != 0) {
			port = (intstatus & ~(intstatus - 1));
			while(!(port == 1)) {
				port = port >> 1;
				index++;
			}
			
			//chris.choi : i think that port_int_callback is always null but if not, then it makes error because callback is not called
			//if(port_int_callback[index+LPGPIO_16] != NULL) {
			//callback = port_int_callback[index+LPGPIO_16];
			//callback();
			//}
			//CMSDK_GPIO1->INTCLEAR = (1 << index);
			///REG_GPIO1_INTSTATUSCLEAR = (1 << index);
			GPIO0->INTSTATUSCLEAR.reg = (1 << index);/// jeffy
		}
		else {
			//CMSDK_GPIO1->INTCLEAR = CMSDK_GPIO1->INTSTATUS;
			GPIO0->INTSTATUSCLEAR.reg |= (1<<15);//jeffy
			//REG_GPIO1_INTSTATUSCLEAR = REG_GPIO1_INTENSET;
		}
		intstatus = GPIO0->INTSTATUSCLEAR.reg;
		#ifdef CHIPVERSION_B0
		intstatus &= 0x1FF;
		#else
		intstatus &= 0x1F;
		#endif
	}while(intstatus != 0);
	
	NVIC_ClearPendingIRQ(7);
}







// chris.choi : it's my code, so i don't want to use it.
//void gpio1_combined_isr_handler(void)
//{
	//uint8_t index = 0;
	////portint_callback_t callback;
	//uint8_t port = LPGPIO_16;
	//uint32_t intstatus = REG_GPIO1_INTENSET;
	//
	//do {
//#ifdef CHIPVERSION_B0
		//intstatus &= 0x1FF;
//#else
		//intstatus &= 0x1F;
//#endif
		//index = 0;
		//if(intstatus != 0) {
			//port = (intstatus & ~(intstatus - 1));
			//while(!(port == 1)) {
				//port = port >> 1;
				//index++;
			//}
			//
			////chris.choi : i think that port_int_callback is always null but if not, then it makes error because callback is not called
			////if(port_int_callback[index+LPGPIO_16] != NULL) {
				////callback = port_int_callback[index+LPGPIO_16];
				////callback();
			////}
			////CMSDK_GPIO1->INTCLEAR = (1 << index);
			//REG_GPIO1_INTSTATUSCLEAR = (1 << index);
			//
		//}
		//else {
			////CMSDK_GPIO1->INTCLEAR = CMSDK_GPIO1->INTSTATUS;
			//REG_GPIO1_INTSTATUSCLEAR = REG_GPIO1_INTENSET;
		//}
		//intstatus = REG_GPIO1_INTENSET;
	//}while(intstatus != 0);
	//
//}
//void gpio0_combined_isr_handler(void)
//{
	//uint8_t port = LPGPIO_0;
	//uint8_t index = 0;
	////portint_callback_t callback;
	//uint32_t intstatus = REG_GPIO0_INTENSET;
	//do {
		//intstatus &= 0xFFFF;
		//if(intstatus != 0) {
			//index = 0;
			//port = (intstatus & ~(intstatus - 1));
			//while(!(port == 1)) {
				//port = port >> 1;
				//index++;
			//}
			////chris.choi : i think that port_int_callback is always null but if not, then it makes error because callback is not called
			////if(port_int_callback[index] != NULL) {
				////callback = port_int_callback[index];
				////callback();
			////}
			//REG_GPIO0_INTSTATUSCLEAR = (1 << index);
		//}
		//else {
			//REG_GPIO0_INTSTATUSCLEAR = REG_GPIO0_INTENSET;
		//}
		//intstatus = REG_GPIO0_INTENSET;
	//}while(intstatus != 0);
//}
//************************ end ******************************************//
//chris.choi : should ask china's driver team that it can be used like this
// this code is come from keil driver version code
//************************ end ******************************************//






plf_drv_status platform_driver_init()
{
	uint8_t i;
	plf_drv_status status = STATUS_NOT_INITIALIZED;
	if((platform_initialized == 0) || (platform_initialized != 1)) {
		memset(port_list,0,sizeof(port_list));
		for(i=0;i<(sizeof(port_list)/sizeof(port_list[0]));i++) {
			port_list[i].bit.gpio_num = i;
			port_list[i].bit.available = 1;
			port_list[i].bit.configured = 0;
		}
		//Set the GPIO for SWD is not available
		port_list[0].bit.available = 0;
		port_list[1].bit.available = 0;
		//GPIO 14 is used for Coex and controlled by Firmware
		port_list[14].bit.available = 0;
		
		// Initialize the ble stack message handler to NULL
		ble_stack_message_handler = NULL;
		
#ifdef CHIPVERSION_B0
		NVIC_DisableIRQ(GPIO0_IRQn);
		NVIC_DisableIRQ(GPIO1_IRQn);
		platform_register_isr = (uint8_t (*)(uint8_t ,void *))0x000007d7;
		platform_unregister_isr = (uint8_t (*)(uint8_t ))0x000007bd;
		handle_ext_wakeup_isr = (void (*)(void))0x1bc59;
		gapm_get_task_from_id = (ke_task_id_t (*)(ke_msg_id_t))0x100059b9;
		gapm_get_id_from_task = (ke_task_id_t (*)(ke_msg_id_t))0x0000d303;
#else
		NVIC_DisableIRQ(PORT0_ALL_IRQn);
		NVIC_DisableIRQ(PORT1_ALL_IRQn);
		platform_register_isr = register_isr;
		platform_unregister_isr = unregister_isr;
		handle_ext_wakeup_isr = (void (*)(void))0x14085;
#endif
		platform_unregister_isr(GPIO1_COMBINED_VECTOR_TABLE_INDEX);
		platform_register_isr(GPIO1_COMBINED_VECTOR_TABLE_INDEX,PORT1_COMB_Handler);
		platform_register_isr(GPIO0_COMBINED_VECTOR_TABLE_INDEX,gpio0_combined_isr_handler);
		
		// Initializing the FW messaging functions.
#ifdef CHIPVERSION_A3
		ke_msg_send 	= (void (*)(void const *))0x00015cd9;
		ke_msg_alloc 	= (void* (*)(ke_msg_id_t const id, ke_task_id_t const dest_id,
										ke_task_id_t const src_id, uint16_t const param_len) ) 0x00015ca9;
		os_sem_up 		= (int (*)(void*)) 0x00017aed;
		gstrFwSem 		= (void*) 0x10000dec;
		NMI_MsgQueueRecv = (int(*)(void*, void ** )) 0x00017c7b;
		InternalAppMsgQHandle = (void*) 0x10002bd8;
		ke_free = (void(*)(void*)) 0x00015bc9;
#elif CHIPVERSION_A4
		ke_msg_send 	= (void (*)(void const *))0x00015f4d;
		ke_msg_alloc 	= (void* (*)(ke_msg_id_t const id, ke_task_id_t const dest_id,
										ke_task_id_t const src_id, uint16_t const param_len) ) 0x00015f1d;
		os_sem_up 		= (int (*)(void*)) 0x00017dd9;
		gstrFwSem 		= (void*) 0x100004e4;
		NMI_MsgQueueRecv = (int(*)(void*, void ** )) 0x00017f67;
		InternalAppMsgQHandle = (void*) 0x10001158;
		ke_free = (void(*)(void*)) 0x00015e3d;
#elif CHIPVERSION_B0
		ke_msg_send 	= (void (*)(void const *))0x0001a01b;
		ke_msg_alloc 	= (void* (*)(ke_msg_id_t const id, ke_task_id_t const dest_id,
										ke_task_id_t const src_id, uint16_t const param_len) ) 0x00019fe9;
		os_sem_up 		= (int (*)(void*)) 0x0001dbdd;
		gstrFwSem 		= (void*) 0x100405ec;
		NMI_MsgQueueRecv = (int(*)(void*, void ** )) 0x0001d5e3;
		InternalAppMsgQHandle = (void*) 0x10040c20;
		ke_free = (void(*)(void*)) 0x00019f09;
#endif
		memset(rx_buffer,0,sizeof(rx_buffer));
		plf_event_buff_index = PLF_EVENT_BUFFER_START_INDEX;
		platform_event_init();
		
#ifdef CHIPVERSION_B0
		//NVIC_EnableIRQ(PORT0_COMB_IRQn);
		//NVIC_EnableIRQ(PORT1_COMB_IRQn);
		NVIC_EnableIRQ(GPIO0_IRQn);
		NVIC_EnableIRQ(GPIO1_IRQn);
#else
		//chris.choi : check keil driver's CMSDK_CM0.h and asf's samb11g18a.h (it's different so i don't know what should be used.
		// it's already asked to sanghai china team.
		//NVIC_EnableIRQ(PORT0_ALL_IRQn);
		//NVIC_EnableIRQ(PORT1_ALL_IRQn);
		NVIC_EnableIRQ(7);	
		NVIC_EnableIRQ(8);
#endif	//CHIPVERSION_B0		
		
#ifndef CHIPVERSION_B0		
		// spi_flash clock fix.
		spi_flash_clock_init();
#endif		
		
		platform_initialized = 1;
		status = STATUS_SUCCESS;
	}
	else {
		status = STATUS_ALREADY_INITIALIZED;
	}
	return status;
}

#ifdef CHIPVERSION_A4
uint8_t unregister_isr(uint8_t isr_index)
{
	uint8_t result = INTC_OK;
	if((isr_index < UART1_RX_VECTOR_TABLE_INDEX) || (VECTOR_TABLE_LAST_INDEX > 47))
		return INTC_INVALID_IRQ;
	else 
	{
		REG_PL_WR(isr_index*4 + ISR_RAM_MAP_START_ADDRESS, 0);
	}
	return result;
}

uint8_t register_isr(uint8_t isr_index,void *fp)
{
	uint8_t result = INTC_OK;
	if((isr_index < UART1_RX_VECTOR_TABLE_INDEX) || (VECTOR_TABLE_LAST_INDEX > 47))
		result = INTC_INVALID_IRQ;
	else if(fp == NULL)
		result = INTC_INVALID_ISR;
	else if(REG_PL_RD(ISR_RAM_MAP_START_ADDRESS + (isr_index*4)) != NULL)
		result = INTC_ISR_ALREADY_REGISTERED;
	else 
	{
		REG_PL_WR(isr_index*4 + ISR_RAM_MAP_START_ADDRESS, (uint32_t) fp);
	}
	return result;
}
#endif	//CHIPVERSION_A4


plf_drv_status platform_register_ble_msg_handler(platform_interface_callback fp)
{
	plf_drv_status status;
	if(platform_initialized == 1) {
		ble_stack_message_handler = fp;
		status = STATUS_SUCCESS;
	}
	else {
		status = STATUS_NOT_INITIALIZED;
	}
	return status;
}

static void at_ke_msg_send(void const * param_ptr)
{
	uint8_t osc_en = REG_PL_RD(0x4000B1EC)&0x01;
	ke_msg_send(param_ptr);
	if(osc_en == 0)
	{
		/* BLE Core is off, issue a wakeup request*/
		REG_PL_WR(0x4000B020, 1);
		while(REG_PL_RD(0x4000B020));
	}
	else
	{
		/*  */
		 os_sem_up(gstrFwSem);
	}
}


//Sends a message through RW kernel messaging API
//struct ke_msg * p_msg;
void platform_interface_send(uint8_t* data, uint32_t len)
{
	struct ke_msghdr * p_msg_hdr = (struct ke_msghdr *) (data);
	void* params;
	
	#if (CHIPVERSION_B0)
	ke_task_id_t dest_id;
	dest_id = p_msg_hdr->dest_id;
	if(gapm_get_task_from_id != NULL)
		p_msg_hdr->dest_id = gapm_get_task_from_id(dest_id);
	#endif	//CHIPVERSION_B0
	// Allocate the kernel message
	params = ke_msg_alloc(p_msg_hdr->id, p_msg_hdr->dest_id, p_msg_hdr->src_id, p_msg_hdr->param_len);
											
	//no params
	if (p_msg_hdr->param_len == 0)
	{
			// Send message directly
			at_ke_msg_send(params);
	}
	else
	{
		//copy params
		memcpy(params,&(data[KE_MSG_HDR_LENGTH]),p_msg_hdr->param_len);
		// Send the kernel message
		at_ke_msg_send(params);
	}
}

void send_plf_int_msg_ind(uint8_t intr_index, uint8_t callback_id, void *data, uint16_t data_len)
{
	void* params;
	
	os_sem_up(gstrFwSem);
	// Allocate the kernel message
	params = ke_msg_alloc(PERIPHERAL_INTERRUPT_EVENT, TASK_INTERNAL_APP, BUILD_INTR_SRCID(callback_id,intr_index), data_len);
											
	//no params
	if(params != NULL) {
		if (data_len == 0)
		{
				// Send message directly
				at_ke_msg_send(params);
		}
		else
		{
			//copy params
			memcpy(params,data,data_len);
			// Send the kernel message
			at_ke_msg_send(params);
		}
	}
}

//Waits on InternalAppMsgQHandle Queue
// This function won't busy wait if nothing on queue but will down the semaphore and go to a blocking state
// The OS then will move control to other higher priority tasks and will only return if these tasks finish processing AND the queue has received 
//a msg
plf_drv_status platform_event_wait(uint32_t timeout)
{
	static struct ke_msg* rcv_msg;
	static struct ke_msghdr	*ke_msg_hdr;
	plf_drv_status status = STATUS_SUCCESS;

	do {
		if(NMI_MsgQueueRecv(InternalAppMsgQHandle, (void**)&rcv_msg) == STATUS_SUCCESS)
		{
			uint16_t msg_id = rcv_msg->id;
			uint16_t src_id = rcv_msg->src_id;
			uint8_t* data = (uint8_t*)rcv_msg->param;
			uint16_t len = rcv_msg->param_len;

			if(msg_id == PERIPHERAL_INTERRUPT_EVENT)
			{
				if(plf_event_buff_index+len > MAX_EVT_BUFF_LEN)
					plf_event_buff_index = PLF_EVENT_BUFFER_START_INDEX;
				memcpy(&rx_buffer[plf_event_buff_index],data,len);
				platform_event_post(src_id,&rx_buffer[plf_event_buff_index],len);
				plf_event_buff_index += len;
				status = STATUS_RECEIVED_PLF_EVENT_MSG;
			}
			else
			{
				// BLE stack messages
				if(ble_stack_message_handler) {
					ke_msg_hdr = (struct ke_msghdr *)&rx_buffer[BLE_EVENT_BUFFER_START_INDEX];
					ke_msg_hdr->id = rcv_msg->id;
#if (CHIPVERSION_A3 || CHIPVERSION_A4)
					ke_msg_hdr->src_id = rcv_msg->src_id;
#else
					ke_msg_hdr->src_id = gapm_get_id_from_task(rcv_msg->src_id);
#endif	//(CHIPVERSION_A3 || CHIPVERSION_A4)
					ke_msg_hdr->dest_id = rcv_msg->dest_id;
					ke_msg_hdr->param_len = rcv_msg->param_len;
					ke_msg_hdr++;
					if(rcv_msg->param_len > 0) {
						memcpy((void *)ke_msg_hdr,rcv_msg->param,rcv_msg->param_len);
					}
					ble_stack_message_handler(rx_buffer,(rcv_msg->param_len + sizeof(struct ke_msghdr)));
					status = STATUS_RECEIVED_BLE_MSG;
				}

			}
			ke_free(rcv_msg);
		}
		else

		{
			status = STATUS_FAILURE;
			break;
		}
	}while(0);

	return status;
}
