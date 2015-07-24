/**
 *
 * \file
 *
 * \brief Main application file.
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

/** \mainpage
 * \section intro Introduction
 * This example demonstrates the use of the WILC1000 with the SAM4S Xplained Pro
 * board in concurrent WiFi mode AP/STA.
 * STA mode is used to retrieve weather information from the internet. 
 * AP mode is used to provide access to HTTP server for any connected STA (up to 
 * 7 at the same time).<br>
 *
 * To access the HTTP server connect with your laptop to WiFi network "WILC1K", then
 * open a browser at http://192.168.5.1<br>
 *
 * It uses the following hardware:
 * - the SAM4S Xplained Pro.
 * - the WILC1000 on EXT1 (WINC1500 can be used as well in bypass mode).
 *
 * \section files Main Files
 * - main.c : Initialize the FreeRTOS scheduler.
 * - sta.c : Enable STA and AP mode for WILC1000. Retrieve weather information via STA mode.
 * - ap.c : Enable HTTP server via AP mode.
 *
 * \section usage Usage
 * -# Configure below code in the conf_wifi.h for AP to be connected.
 * \code
 *    #define CONF_WLAN_SSID         "DEMO_AP"
 *    #define CONF_WLAN_AUTH         M2M_WIFI_SEC_WPA_PSK
 *    #define CONF_WLAN_PSK          "12345678"
 * \endcode
 * -# Build the program and download it into the board.
 * -# On the computer, open and configure a terminal application as the follows.
 * \code
 *    Baud Rate : 115200
 *    Data : 8bit
 *    Parity bit : none
 *    Stop bit : 1bit
 *    Flow control : none
 * \endcode
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 * \code
 *    -- WINC1500 AP scan example --
 *    -- SAMD21_XPLAINED_PRO --
 *    -- Compiled: xxx xx xxxx xx:xx:xx --
 *
 *    [1] SSID:DEMO_AP1
 *    [2] SSID:DEMO_AP2
 *    [3] SSID:DEMO_AP
 *    Found DEMO_AP
 *    Wi-Fi connected
 *    Wi-Fi IP is xxx.xxx.xxx.xxx
 * \endcode
 *
 * \section compinfo Compilation Information
 * This software was written for the GNU GCC compiler using Atmel Studio 6.2
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com">Atmel</A>.\n
 */ 

#include "asf.h"
#include "osprintf.h"
#include "driver/include/m2m_wifi.h"
#include "sta.h"
#include "ap.h"

#define TASK_STA_STACK_SIZE		(4096 / sizeof(portSTACK_TYPE))
#define TASK_STA_PRIORITY			(tskIDLE_PRIORITY + 1)
#define TASK_AP_STACK_SIZE		(1024 / sizeof(portSTACK_TYPE))
#define TASK_AP_PRIORITY			(tskIDLE_PRIORITY + 1)

#define STRING_EOL    "\r\n"
#define STRING_HEADER "-- Weather client example --"STRING_EOL \
		"-- "BOARD_NAME " --"STRING_EOL	\
		"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

/** SysTick counter to avoid busy wait delay. */
uint32_t ms_ticks = 0;

void fatal(int code, const char *msg);
void fatal(int code, const char *msg)
{
	for (;;) {
	}
}

#if configUSE_MALLOC_FAILED_HOOK
void vApplicationMallocFailedHook(void);
void vApplicationMallocFailedHook(void)
{
	for (;;) {
	}
}
#endif

#if configCHECK_FOR_STACK_OVERFLOW
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName);
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
	TRACE("ERROR: STACK OVERFLOW");
	TRACE(pcTaskName);
	for (;;) {
	}
}
#endif

static uint32_t fault_addr;
void hard_fault(uint32_t *args, uint32_t lr);
void hard_fault(uint32_t *args, uint32_t lr)
{
	fault_addr = args[6];
	for (;;) {
	}
}

void HardFault_Handler(void)
{
	__asm(
		"  mov r0, #4          \n"
		"  mov r1, lr          \n"
		"  tst r0, r1          \n"
		"  beq using_msp       \n"
		"  mrs r0, psp         \n"
		"  b call_c            \n"
		"using_msp:            \n"
		"  mrs r0, msp         \n"
		"call_c:               \n"
		"  ldr r2, =hard_fault \n"
		"  bx r2               \n"
	);
}

/**
 * \brief Main program function.
 */
int main(void)
{
	/* Initialize the board. */
	sysclk_init();
	board_init();

	/* Initialize the UART console. */
	osprintf_init();
	puts(STRING_HEADER);
	
	/* Create main task. */
	xTaskCreate(sta_task, (signed char *)"WiFiSTA", TASK_STA_STACK_SIZE, 0,
			TASK_STA_PRIORITY, 0);
			
	/* Create main task. */
	xTaskCreate(ap_task, (signed char *)"WiFiAP", TASK_AP_STACK_SIZE, 0,
			TASK_AP_PRIORITY, 0);

	vTaskStartScheduler();
	
	while (1) {
	}

	return 0;
}
