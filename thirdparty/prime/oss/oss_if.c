/**
 * \file
 *
 * \brief Operative System Support Interface
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

/* asf includes */
#include "asf.h"

/* Configuration includes */
#include "conf_oss.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \weakgroup oss_prime_group
 * @{
 */

//! Tasks handlers
xTaskHandle xPrimeHnd;
//! Timers handlers
xTimerHandle xUpdateTimer;

/**
 * \internal
 * \brief Task to update internal PRIME systick.
 *
 * This function must be called every 1 ms.
 *
 */
static void _update_1ms_proc(xTimerHandle pxTimer)
{
	UNUSED(pxTimer);
	taskENTER_CRITICAL();

	prime_Upd1msInt();

	sscs432_Upd1msInt();

	usi_txrx_block_timer();

	taskEXIT_CRITICAL();
}

/**
 * \internal
 * \brief Periodic task to process PRIME. Initialize and start every layer.
 *
 */
static void _prime_stack_process(void * pvParameters)
{
	static portTickType xLastWakeTime;
	static portTickType xPeriod;

	UNUSED(pvParameters);

	vPhyInitTask(DISABLE_SERIAL);

	pal_Init();

	prime_Init();

	sscs432_Init();

	usi_init();

	prime_Start();

	sscs432_Start();

#ifdef OSS_PRIME_DISABLE_PRM
	prime_MLME_SET_request(PIB_MAC_PRM_ACTIVATION, 0);
#endif
#ifdef OSS_PRIME_ENABLE_EMB_SNIFFER
	prime_MLME_SET_request(PIB_MAC_SNIFFER_ACTIVATION, 1);
#endif

	/* Start timer to update counters in phy layer */
	xTimerStart(xUpdateTimer, PRIME_UPDATE_PROCESS_TIMER_RATE);

	xPeriod = PRIME_PROCESS_TIMER_RATE;
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		taskENTER_CRITICAL();

		pal_Process();

		prime_Process();

		sscs432_Process();

		usi_process();

		taskEXIT_CRITICAL();
	}
}

/**
 * \internal
 * \brief Create main PRIME task and create timer to update internal counters.
 *
 */
void vPrimeStackInitTask(void)
{
	/* Create new task to USI_process */
	xTaskCreate(_prime_stack_process, (const signed char * const)"PrimeProc",
				TASK_PRIME_STACK, NULL, TASK_PRIME_PRIO, &xPrimeHnd);

	/* Create timer to update counters in phy layer */
	xUpdateTimer = xTimerCreate((const signed char * const) "UPD timer",/* A text name, purely to help debugging. */
				PRIME_UPDATE_PROCESS_TIMER_RATE, /* The timer period. */
				pdTRUE,		/* This is an auto-reload timer, so xAutoReload is set to pdTRUE. */
				NULL,		/* The timer does not use its ID, so the ID is just set to NULL. */
				_update_1ms_proc	/* The function that is called each time the timer expires. */
				);
	configASSERT(xUpdateTimer);
}

//! @}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
