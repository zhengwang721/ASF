/**
 * \file
 *
 * \brief HTTP component task for the FreeRTOS Web/DSP Demo.
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
 */

#include "task.h"
#include "stdio_serial.h"
#include "ethernet.h"
#include "sysclk.h"
#include "lwip/tcpip.h"
#include "httpd.h"

#include "lwip/stats.h"

static void http_task(void *pvParameters);

/**
 * \brief Create the HTTP task.
 *
 * \param stack_depth_words Task stack size in 32 bits word.
 * \param task_priority Task priority.
 */
void create_http_task(uint16_t stack_depth_words,
		unsigned portBASE_TYPE task_priority)
{
	/** Create the task as described above. */
	xTaskCreate(http_task, (const signed char *const) "HTTP",
			stack_depth_words, NULL, task_priority,
			NULL);
}

/**
 * \brief HTTP task core function.
 *
 * \param pvParameters Junk parameter.
 */
static void http_task(void *pvParameters)
{
	struct netconn *conn, *newconn;
	err_t err;
	/* Just to avoid compiler warnings. */
	UNUSED(pvParameters);

	/** Wait for user to read instructions. */
	WAIT_FOR_TOUCH_EVENT;

	/* Create a new TCP connection handle */
	conn = netconn_new(NETCONN_TCP);
	if (conn == NULL)
	{
		printf("http_server: invalid conn");
		return;
	}

	/* Bind to port 80 (HTTP) with default IP address */
	netconn_bind(conn, NULL, 80);

	/* Put the connection into LISTEN state */
	netconn_listen(conn);

	do {
		err = netconn_accept(conn, &newconn);
		if (err == ERR_OK) {

			stats_display();

			/* Instanciate a new task to handle the HTTP request. */
			xTaskCreate(http_request, (const signed char *const) "HTTP",
					mainHTTP_TASK_STACK_SIZE, newconn, mainHTTP_TASK_PRIORITY,
					NULL);
		}
	} while (err == ERR_OK);

	printf("http_server_netconn_thread: netconn_accept received error %d, shutting down\r\n", err);

	netconn_close(conn);
	netconn_delete(conn);
}
