/**
 * \file
 *
 * \brief Cycles application for ATMEL PRIME v1.3 Service Node
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

#include <string.h>
#include <stdio.h>

#include "compiler.h"

#include "short_cycle_service.h"

#include "asf.h"

/* Internal Function declarations */
static void prv_update_cycles(xTimerHandle pxTimer);
static void prv_process_cycles_app(void * pvParameters);

/* Tasks handlers */
xTaskHandle xCyclesHnd;
/* Timers handlers */
xTimerHandle xUpdateCyclesTimer;

void _generate_str(uint8_t uc_step);
uint8_t _check_received_data(uint8_t uc_step, uint8_t* puc_data);
Bool _check_nodestatus(uint32_t *pul_status);
void _check_received_messages(void);

#define MAX_TIME_BETWEEN_MSGS	10		// seconds

#define STR_SERIAL_NUMBER		"ATMEL00000000"
#define LENGTH_SERIAL_NUMBER	13
enum {
	CH_SEND_CHECK,
	CH_WAIT_ANSWER
} st_check_node;

/* Configure Number of steps and lengths */
#define NUM_STEPS	6
static uint16_t pus_lengthMsg [NUM_STEPS] = {43,18,179,34,58,5};
static uint16_t pus_baseLengthMsg [NUM_STEPS] = {56, 13, 64, 7,7,2};
static uint8_t puc_received_data_reference_msg[300];
static uint8_t puc_generate_str_msg[300];

static uint32_t ul_time_out;
static uint8_t puc_mlmeCmd[MLME_DATA_SIZE];
static uint16_t us_base_node_address;

static dl432Buffer_t cmd_tx_432;
static dl432Buffer_t cmd_rx_432;
static dl432_LlcCallback_t rcv_432;

//***********************************************************************************
/**
*************************************************************************************/
void _generate_str(uint8_t uc_step)
{
	uint16_t us_len, i;
	uint8_t uc_c;

	us_len = pus_lengthMsg[uc_step];
	puc_generate_str_msg[0] = '0' + uc_step;
	uc_c = 'z';

	for (i = 1; i <us_len; i++)
	{
		puc_generate_str_msg[i] = uc_c;
		uc_c--;
		if (uc_c <'a') uc_c = 'z';
	}
	return;
}

//***********************************************************************************
/**
*************************************************************************************/
uint8_t _check_received_data(uint8_t uc_step, uint8_t* puc_data)
{
	uint16_t us_len, i;
	uint8_t uc_c;

	us_len = pus_baseLengthMsg[uc_step];
	puc_received_data_reference_msg[0] = '0' + uc_step;
	uc_c = 'A';

	for (i = 1; i <us_len; i++)
	{
		puc_received_data_reference_msg[i] = uc_c;
		uc_c++;
		if (uc_c >'Z') uc_c = 'A';
	}

	if(memcmp(puc_data,puc_received_data_reference_msg, us_len) == 0)
		return true;
	else
		return false;

}

//***********************************************************************************
/**
*************************************************************************************/
Bool _check_nodestatus(uint32_t *pul_status)
{
	uint16_t pibAttrib;
	Bool end = false;

	switch (st_check_node)
	{
		case CH_SEND_CHECK:
			prime_MLME_GET_request(PIB_432_CONNECTION_STATE);
			st_check_node = CH_WAIT_ANSWER;
			break;

		case CH_WAIT_ANSWER:
			if (prime_MLME_callback(puc_mlmeCmd))
			{
				pibAttrib = (uint16_t)(puc_mlmeCmd[2] << 8) + (uint16_t)puc_mlmeCmd[3];
				if ((puc_mlmeCmd[0] == PRIME_MLME_GET_CONFIRM) &&
					(pibAttrib == PIB_432_CONNECTION_STATE))
				{
					if (puc_mlmeCmd[1] == PRIME_MLME_RESULT_SUCCESS)
					{
						// Extract pibValue
						*pul_status = (uint16_t)(puc_mlmeCmd[4] << 24) +
									  (uint16_t)(puc_mlmeCmd[5] << 16) +
									  (uint16_t)(puc_mlmeCmd[6] << 8) +
									  (uint16_t)puc_mlmeCmd[7];
					}
					else
					{
						*pul_status = 0; // node disconnected
					}
					st_check_node = CH_SEND_CHECK;
					end = true;
				}

			}
			break;
		default:
			st_check_node = CH_SEND_CHECK;
			break;
	}
	return end;
}

//***********************************************************************************
/**
*************************************************************************************/
void _check_received_messages(void)
{
	sscs432_NotService_t newNot;
	uint8_t uc_step;

	if (sscs432_poll_establish_release(&newNot))
	{
		if (newNot.cmd == CL_432_RELEASE_CONFIRM)
		{
			ul_time_out = 0; //Restart process connection immediately
		}
		else
		{
			us_base_node_address = newNot.baseAddress;
		}
	}

	dl432_dl_callback(&rcv_432, &cmd_rx_432, MAX_LENGTH_432_DATA);
	if (rcv_432.cmd == DL432_DATA_INDICATION)
	{
		uc_step = cmd_rx_432.lsdu[0] - '0';
		if(_check_received_data(uc_step, cmd_rx_432.lsdu))
		{
      _generate_str(uc_step);
			memcpy(&cmd_tx_432.lsdu, puc_generate_str_msg, pus_lengthMsg[uc_step]);
			dl432_dl_data_request(0, 0, us_base_node_address, &cmd_tx_432, pus_lengthMsg[uc_step], 0);
		}
	}
}


/**
* \brief Create main Cycles Application task and create timer to update internal counters.
*
*/
void vCyclesInitTask(void)
{
	/* Create new task to Cycles Application */
	xTaskCreate(prv_process_cycles_app, (const signed char * const)"CyclesTask",
	            TASK_CYCLES_LAYER_STACK, NULL, TASK_CYCLES_LAYER_PRIO, &xCyclesHnd);
}

/**
* \brief Periodic task to process Cycles App. Initialize and start Cycles Application and launch timer
* to update internal counters.
*
*/
static void prv_process_cycles_app(void * pvParameters)
{
	UNUSED(pvParameters);

  static portTickType xLastWakeTime;
	static portTickType xPeriod;
	uint32_t ul_node_connected;

	/* Init variables and status */
  st_check_node = CH_SEND_CHECK;
  ul_time_out = PRIME_CHECK_CONNECTION_RATE;

	xPeriod = PRIME_CYCLES_APP_TIMER_RATE;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

    taskENTER_CRITICAL();
    _check_received_messages();
    if (!ul_time_out--){
      ul_time_out = PRIME_CHECK_CONNECTION_RATE;
      if(_check_nodestatus(&ul_node_connected)){
        if (!ul_node_connected){	// launch connection
          sscs432_establish_request((uint8_t*) STR_SERIAL_NUMBER, LENGTH_SERIAL_NUMBER);
        }
      }
    }
    taskEXIT_CRITICAL();
	}
}
