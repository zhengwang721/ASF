/**
 * \file
 *
 * \brief SAMB11 timer driver
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
#include "CMSDK_CM0.h"
#include "timer.h"
#include "platform_drv.h"

timer_callback	user_app_callback;

typedef struct _tstrOsTimer {
                char* pcName; /*!< pointer to timer name */
                unsigned char u8IsPeriodic; /*!< '0': single shot - '1': periodic */
                unsigned char u8Ps;        /*!< Power save mode 0-> Auto (may fire +/-Delta to save power) 1->EXACT TIME (Increase power)*/
                unsigned long u64Timeout; /*!< timer timeout in OS_TICK units */
                unsigned u32Expire; /*!< timer expiration in OS_TICK units */
                void (*pfTimerCb)(void*); /*!< timer callback function */
                void *pvData;    /*!< pointer to void for user data */
                struct _tstrOsTimer *pstrNext; /*!< Pointer to the next object at the list */
} tstrOsTimer;

void (*os_timer_init)(void);
int (*os_timer_start)(tstrOsTimer *pstrTimer, char *pcName,
                                void(*pfTimerCb)(void*), unsigned long u32Expire,
                                unsigned char u8IsPeriodic, void *pvData,unsigned char u8Ps);
int (*os_timer_stop)(tstrOsTimer *pstrTimer);
int (*os_timer_get_tickcount)(void);
tstrOsTimer	timer;
timer_state	app_timer_state;
uint32_t timer_dbg;
const char timer_name[10] = "app_timer";

timer_drv_status timer_init()
{
	timer_drv_status status = TIMER_STATUS_OK;
#ifdef CHIPVERSION_B0
	status = TIMER_STATUS_NOT_INITIALIZED;
#else
	os_timer_init							= (void (*)(void)) 0x00017933;
  os_timer_start						= (int (*)(tstrOsTimer *, char *,
                                void(*pfTimerCb)(void*), unsigned long ,
                                unsigned char , void *,unsigned char)) 0x0001793b;
  os_timer_stop							= (int (*)(tstrOsTimer *)) 0x0001798f;
  os_timer_get_tickcount    = (int (*)(void)) 0x000179dd;
	memset(&timer,0,sizeof(timer));
	user_app_callback = NULL;
	os_timer_init();
	app_timer_state = STATE_IDLE;
	timer_dbg = 0;
#endif
	return status;
}

void drv_callback()
{
	if(user_app_callback != NULL)
	{
		user_app_callback();
	}
	timer_dbg++;
	//send_plf_int_msg_ind(USER_TIMER_CALLBACK,TIMER_EXPIRED_CALLBACK_TYPE_DETECT,NULL,0);
}

timer_drv_status timer_start(uint32_t timeout,uint8_t periodic)
{
	if(app_timer_state == STATE_IDLE) {
		os_timer_start(&timer,(char *)timer_name,drv_callback,(timeout*2),periodic,NULL,1);
		#if 0
		if(timeout >= 2)
			os_timer_start(&timer,(char *)timer_name,drv_callback,(timeout*20),1,NULL,1);
		else {
			os_timer_start(&timer,(char *)timer_name,drv_callback,20,1,NULL,1);
		}
		app_timer_state = STATE_SCHEDULED;
		#endif	//0
	}
	else {
		return TIMER_STATUS_BUSY;
	}
	return TIMER_STATUS_OK;
}

void timer_stop()
{
	os_timer_stop(&timer);
	app_timer_state = STATE_IDLE;
}

timer_drv_status timer_register_callback(timer_callback cb)
{
	timer_drv_status status = TIMER_STATUS_OK;
	if(cb != NULL) {
		user_app_callback = cb;
	}
	else {
		status = TIMER_STATUS_INVALID_ARGUMENT;
	}
	return status;
}
