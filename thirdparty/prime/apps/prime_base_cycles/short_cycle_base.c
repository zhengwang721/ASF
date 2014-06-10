/**
 * \file
 *
 * \brief Cycles application for ATMEL PRIME v1.3 Base Node
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

#include "short_cycle_base.h"

#include "asf.h"

/* Internal Function declarations */
static void prv_update_cycles(xTimerHandle pxTimer);
static void prv_process_cycles_app(void * pvParameters);

/* Tasks handlers */
xTaskHandle xCyclesHnd;
/* Timers handlers */
xTimerHandle xUpdateCyclesTimer;

Bool _update_registered_nodes(void);
void _update_table_nodes(sscs432_NotBase_t* not);
void _print_result_test(uint16_t us_node, uint8_t uc_step, uint8_t uc_error);
void _log_full_cycles_time(int cycleId, int cycleTime);
void _log_cycles_step(int cycleId, Node432Type node, int status, int step);
void _generate_str(uint8_t uc_step);
void _execute_sort_cycles(void);
Bool _execute_cycle(uint16_t us_node, uint8_t uc_step, uint8_t* puc_error);
void _check_notifications(void);

#define TIMER_BETWEEN_CYCLES	10
#define TIME_OUT_MSG			40

#define PIB_NODES_432_CONNECTED			0x8250
#define MAX_NUM_NODES_CONNECTED			10

#define ERROR_NO_ERROR					0
#define ERROR_TIMEOUT_DATA_CONFIRM		1
#define ERROR_BAD_TX					2
#define ERROR_TIMEOUT_DATA_INDICATION	3
#define ERROR_NON_CONNECTED_NODE		4
#define ERROR_INTEGRITY					5

// Invalid 432 address
#define INVALID_432_ADDRESS		0xffff

enum
{
	SC_WAIT_INITIAL_LIST,
	SC_WAIT_START_CYCLES,
	SC_TIME_NEXT_CYCLE,
	SC_CYCLES
} st_sort_cycles;

enum
{
	RN_INITIAL_STATE,
	RN_WAIT_ANSWER_NODES_REGISTERED
} st_registered_nodes;

enum
{
	SEND,
	WAIT_DATA_CONFIRM,
	WAIT_DATA_INDICATION
} st_execute_cycle;

/* Configure Number of steps and lengths */
#define NUM_STEPS	6
static uint16_t pus_length_msg [NUM_STEPS] = {56, 13, 64, 7, 7, 2};
static uint16_t pus_srv_length_msg [NUM_STEPS] = {43, 18, 179, 34, 58, 5};


static Node432Type nodes_connected[MAX_NUM_NODES_CONNECTED];
static uint16_t us_num_nodes_connected;

static uint32_t ul_cycles_counter;

static uint16_t us_node_cycling;
static uint8_t uc_step_cycling;

static uint32_t ul_timer_next_cycle;
static uint32_t ul_total_time_cycle;
static uint32_t ul_start_time_cycle;
static uint32_t ul_end_time_cycle;
static uint32_t ul_time_out;

static uint8_t puc_mlme_cmd[MLME_DATA_SIZE];

static dl432Buffer_t cmd_tx_432;
static dl432Buffer_t cmd_rx_432;
static dl432_LlcCallback_t rcv_432;
static Bool new_cmd;

static uint8_t puc_generate_str_msg[64];
static uint8_t puc_checkReceivedData_msg[179];
static char c_log_full_cycles_time_query[50];
static char c_log_cycles_step_query[100];
static char c_print_result_test_query[60];

void _log_full_cycles_time(int cycleId, int cycleTime)
{
	sprintf(c_log_full_cycles_time_query, "cycleId: %d, cycleTime: %d \r\n",
			cycleId,
			cycleTime
			);

        printf("%s", c_log_full_cycles_time_query);
}


void _log_cycles_step(int cycleId, Node432Type node, int status, int step)
{
	sprintf(c_log_cycles_step_query, "cycleId: %d, mac:%02x.%02x.%02x.%02x.%02x.%02x, status: %d, step: %d \r\n",
			cycleId,
			node.mac[0],
			node.mac[1],
			node.mac[2],
			node.mac[3],
			node.mac[4],
			node.mac[5],
			status,
			step
			);

        printf("%s", c_log_cycles_step_query);
}

//***********************************************************************************
/**
*************************************************************************************/
void _print_result_test(uint16_t us_node, uint8_t uc_step, uint8_t uc_error)
{
	uint32_t ul_c_time;

	ul_end_time_cycle = (uint32_t)xTaskGetTickCount();
	ul_c_time = ul_end_time_cycle - ul_start_time_cycle;
	ul_total_time_cycle += ul_c_time;

	if ((uc_step == 6) && (uc_error == ERROR_NO_ERROR))
	{
#ifdef __GNUC__
		sprintf(c_print_result_test_query, "[Cycle] %02x.%02x.%02x.%02x.%02x.%02x Time %lu \tOK \r\n",
#endif
#ifdef __ICCARM__
		sprintf(c_print_result_test_query, "[Cycle] %02x.%02x.%02x.%02x.%02x.%02x Time %u \tOK \r\n",
#endif
			nodes_connected[us_node].mac[0],
			nodes_connected[us_node].mac[1],
			nodes_connected[us_node].mac[2],
			nodes_connected[us_node].mac[3],
			nodes_connected[us_node].mac[4],
			nodes_connected[us_node].mac[5],
			ul_c_time
			);
	}
	else
	{
#ifdef __GNUC__
		sprintf(c_print_result_test_query, "[Cycle] %02x.%02x.%02x.%02x.%02x.%02x Time %lu \tERROR \r\n",
#endif
#ifdef __ICCARM__
		sprintf(c_print_result_test_query, "[Cycle] %02x.%02x.%02x.%02x.%02x.%02x Time %u \tERROR \r\n",
#endif
			nodes_connected[us_node].mac[0],
			nodes_connected[us_node].mac[1],
			nodes_connected[us_node].mac[2],
			nodes_connected[us_node].mac[3],
			nodes_connected[us_node].mac[4],
			nodes_connected[us_node].mac[5],
			ul_c_time
			);
	}

        printf("%s", c_print_result_test_query);
}

//***********************************************************************************
/**
*************************************************************************************/
static uint8_t _checkReceivedData(uint8_t uc_step, uint8_t *data)
{
	uint16_t us_len, i;
	uint8_t uc_c;

	us_len = pus_srv_length_msg[uc_step];
	puc_checkReceivedData_msg[0] = '0' + uc_step;
	uc_c = 'z';

	for (i = 1; i <us_len; i++)
	{
		puc_checkReceivedData_msg[i] = uc_c;
		uc_c--;
		if (uc_c <'a') uc_c = 'z';
	}


	if(memcmp(data,puc_checkReceivedData_msg,us_len) == 0)
		return true;
	else
		return false;
}

//***********************************************************************************
/**
*************************************************************************************/
Bool _update_registered_nodes(void)
{
	//MlmeListGetNode432Table *ptrList432;
	uint16_t pibAttrib;
	uint16_t dev_idx;
	Bool end = false;
	uint8_t i;

	switch (st_registered_nodes)
	{
		case RN_INITIAL_STATE:
			// Beginning of the program, no problems with Queues, no need to retry
			prime_MLME_LIST_GET_request(PIB_NODES_432_CONNECTED);
			st_registered_nodes = RN_WAIT_ANSWER_NODES_REGISTERED;
			break;

		case RN_WAIT_ANSWER_NODES_REGISTERED:
			if (prime_MLME_callback(puc_mlme_cmd))
			{
				//ptrList432 = (MlmeListGetNode432Table *) puc_mlme_cmd;
				//if ((ptrList432->cmd == PRIME_MLME_LIST_GET_CONFIRM) &&
					//(ptrList432->pibAttrib == PIB_NODES_432_CONNECTED))
				//{
					//for (i = 0; i < ptrList432->numItems; i++)
					//{
						//memcpy(&nodes_connected[us_num_nodes_connected++], &(ptrList432->dev[i]), sizeof(Node432Type));
					//}
					//if (ptrList432->isLast) end = true;
				//}
				// Check cmd
				if (puc_mlme_cmd[0] == PRIME_MLME_LIST_GET_CONFIRM) {
					pibAttrib = (uint16_t)(puc_mlme_cmd[2] << 8) + (uint16_t)puc_mlme_cmd[3];
					if (pibAttrib == PIB_NODES_432_CONNECTED) {
						dev_idx = 6; // puc_mlme_cmd[6] corresponds to dev[0]
						for (i = 0; i < puc_mlme_cmd[5]; i++) {		// Limit is numItems
							memcpy(&nodes_connected[us_num_nodes_connected++], &puc_mlme_cmd[dev_idx], sizeof(Node432Type));
							dev_idx += sizeof(Node432Type);
						}
						// Check isLast
						if (puc_mlme_cmd[4]) {
							end = true;
						}
					}
				}
			}
			break;
	}
	return end;
}

//***********************************************************************************
/**
*************************************************************************************/
void _generate_str(uint8_t uc_step)
{
	uint16_t us_len, i;
	uint8_t uc_c;

	us_len = pus_length_msg[uc_step];
	puc_generate_str_msg[0] = '0' + uc_step;
	uc_c = 'A';

	for (i = 1; i <us_len; i++)
	{
		puc_generate_str_msg[i] = uc_c;
		uc_c++;
		if (uc_c >'Z') uc_c = 'A';
	}
	return;
}
//***********************************************************************************
/**
*************************************************************************************/
Bool _execute_cycle(uint16_t us_node, uint8_t uc_step, uint8_t* puc_error)
{
	Bool end = false;

	if (new_cmd &&
		(((rcv_432.dstAddress != nodes_connected[us_node].address) && (rcv_432.cmd ==  DL432_DATA_CONFIRM)) ||
		((rcv_432.srcAddress != nodes_connected[us_node].address)&& (rcv_432.cmd ==  DL432_DATA_INDICATION))))
	{
		// UNEXPECTED COMMAND
		new_cmd = false;
	}

	switch (st_execute_cycle)
	{
		case SEND:
			if (nodes_connected[us_node].address == INVALID_432_ADDRESS)
			{
				*puc_error = ERROR_NON_CONNECTED_NODE;
				end = true;
			}
			else
			{
				_generate_str(uc_step);
				memcpy(&cmd_tx_432.lsdu, puc_generate_str_msg, pus_length_msg[uc_step]);
				if (dl432_dl_data_request(0, 0, nodes_connected[us_node].address, &cmd_tx_432, pus_length_msg[uc_step], 0) == 0)
				{
					ul_time_out = TIME_OUT_MSG;
					st_execute_cycle = WAIT_DATA_CONFIRM;
				}

			}
			break;

		case WAIT_DATA_CONFIRM:
			if (!ul_time_out)
			{
				*puc_error = ERROR_TIMEOUT_DATA_CONFIRM;
				end = true;
			}

			if (new_cmd)
			{
				if (rcv_432.status == DL432_RESULT_SUCCESS)
				{
					ul_time_out = TIME_OUT_MSG;
					st_execute_cycle = WAIT_DATA_INDICATION;
				}
				else
				{
					*puc_error = ERROR_BAD_TX;
					end = true;
				}
			}
			break;

		case WAIT_DATA_INDICATION:
			if (!ul_time_out)
			{
				*puc_error = ERROR_TIMEOUT_DATA_INDICATION;
				end = true;
			}

			if (new_cmd)
			{
				if(_checkReceivedData(uc_step,cmd_rx_432.dl.buff) == true)
				{
					*puc_error = ERROR_NO_ERROR;
					end = true;
				}
				else
				{
					*puc_error = ERROR_INTEGRITY;
					end = true;
				}
			}
			break;
	}

	if (end)
		st_execute_cycle = SEND;

	return end;
}

//***********************************************************************************
/**
*************************************************************************************/
void _update_table_nodes(sscs432_NotBase_t* not)
{
	uint16_t i;
	for (i = 0; i < us_num_nodes_connected; i++)
	{
		if (memcmp(nodes_connected[i].mac, not->macAddress, PRIME_MAC_LENGTH) == 0) break;
	}

	nodes_connected[i].address = not->address;

	// New node
	if (i == us_num_nodes_connected)
	{
		memcpy(nodes_connected[i].mac, not->macAddress, PRIME_MAC_LENGTH);
		nodes_connected[i].arq = not->arq;
		nodes_connected[i].lenSerial = not->lenSerialNumber;
		memcpy(nodes_connected[i].serialNumber, not->serialNumber, SERIAL_NUMBER_432_MAC);
		us_num_nodes_connected++;
	}

}

//***********************************************************************************
/**
*************************************************************************************/
void _check_notifications(void)
{
	uint8_t i;
	sscs432_NotBase_t newNot;

	if (sscs432_poll_joins_leaves(&newNot) == 0)
		return;

	switch(newNot.cmd)
	{
		case CL_432_JOIN_INDICATE:
			_update_table_nodes(&newNot);
			break;

		case CL_432_LEAVE_INDICATE:
			for (i = 0; i < us_num_nodes_connected; i++)
			{
				if (nodes_connected[i].address==newNot.address)
				{
					nodes_connected[i].address = INVALID_432_ADDRESS;
					break;
				}
			}
			break;
	}
}

//***********************************************************************************
/**
*************************************************************************************/
void _execute_sort_cycles(void)
{
	uint8_t uc_error;

	new_cmd = dl432_dl_callback(&rcv_432, &cmd_rx_432, MAX_LENGTH_432_DATA);

	if (st_sort_cycles != SC_WAIT_INITIAL_LIST)
	{
		_check_notifications();
	}

	switch (st_sort_cycles)
	{
		case SC_WAIT_INITIAL_LIST:
			if (_update_registered_nodes())
				st_sort_cycles = SC_WAIT_START_CYCLES;
			break;

		case SC_WAIT_START_CYCLES:
			if (us_num_nodes_connected)
			{
				ul_timer_next_cycle = TIMER_BETWEEN_CYCLES;
				st_sort_cycles = SC_TIME_NEXT_CYCLE;
				ul_cycles_counter = 0;
				ul_total_time_cycle = 0;
			}
			break;

		case SC_TIME_NEXT_CYCLE:
			if (!ul_timer_next_cycle)
			{
				ul_timer_next_cycle = TIMER_BETWEEN_CYCLES;
				ul_start_time_cycle = (uint32_t)xTaskGetTickCount();
				us_node_cycling = 0;
				uc_step_cycling = 0;
				st_sort_cycles = SC_CYCLES;
			}
			break;

		case SC_CYCLES:
			if (us_node_cycling < us_num_nodes_connected)
			{
				if (_execute_cycle(us_node_cycling, uc_step_cycling, &uc_error))
				{
					if (uc_error != ERROR_NO_ERROR)
					{
						_log_cycles_step(ul_cycles_counter,nodes_connected[us_node_cycling],0,uc_step_cycling);
						_print_result_test(us_node_cycling, uc_step_cycling, uc_error);
						uc_step_cycling = 0;
						us_node_cycling++;
					}
					else
					{
						if(uc_step_cycling==NUM_STEPS-1){
							_log_cycles_step(ul_cycles_counter,nodes_connected[us_node_cycling],1,uc_step_cycling);
						}
						uc_step_cycling++;
						//if (uc_step_cycling == 4)
						if (uc_step_cycling == 6)
						{
							_print_result_test(us_node_cycling, uc_step_cycling, uc_error);
							uc_step_cycling = 0;
							us_node_cycling++;
						}
					}
				}
			}
			else
			{
				_log_full_cycles_time(ul_cycles_counter, ul_total_time_cycle);
				st_sort_cycles = SC_TIME_NEXT_CYCLE;
				ul_cycles_counter++;
			}
			break;
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

	/* Create timer to update counters in phy layer */
	xUpdateCyclesTimer = xTimerCreate((const signed char * const) "UpdCycles timer",/* A text name, purely to help debugging. */
		PRIME_UPDATE_CYCLES_TIMER_RATE,		/* The timer period. */
		pdTRUE,								/* This is an auto-reload timer, so xAutoReload is set to pdTRUE. */
		NULL,								/* The timer does not use its ID, so the ID is just set to NULL. */
		prv_update_cycles						/* The function that is called each time the timer expires. */
		);

	configASSERT(xUpdateCyclesTimer);
}

/**
* \brief Update internal counters.
*
*/
static void prv_update_cycles(xTimerHandle pxTimer)
{
	UNUSED(pxTimer);

        if (ul_timer_next_cycle)
		ul_timer_next_cycle--;
	if (ul_time_out)
		ul_time_out--;
}

/**
* \brief Periodic task to process Cycles App. Initialize and start Cycles Application and launch timer
* to update internal counters.
*
*/
static void prv_process_cycles_app(void * pvParameters)
{
	static portTickType xLastWakeTime;
	static portTickType xPeriod;

	UNUSED(pvParameters);

        /* Init variables and status */
	st_sort_cycles = SC_WAIT_INITIAL_LIST;
	st_registered_nodes = RN_INITIAL_STATE;
	ul_timer_next_cycle = TIMER_BETWEEN_CYCLES;
	st_execute_cycle = SEND;
	us_num_nodes_connected = 0;

	/* Start timer to update counters in Cycles App */
	xTimerStart(xUpdateCyclesTimer, 0);

	xPeriod = PRIME_CYCLES_APP_TIMER_RATE;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		taskENTER_CRITICAL();
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		_execute_sort_cycles();
		taskEXIT_CRITICAL();
	}
}
