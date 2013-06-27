/**
 * \file
 *
 * \brief FreeRTOS Web/DSP Demo.
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

/**
 * \mainpage web_dsp_demo FreeRTOS Web/DSP Demo
 *
 * \section Purpose
 *
 * This demo demonstrates how to use the DACC, TC, GMAC, SPI, SMC in a real-time
 * context, using the FreeRTOS kernel. The demo also features the LwIP stack on
 * top of the GMAC driver.
 *
 * \section Requirements
 *
 * This example can be used on the following boards:<br>
 * - SAM4E-EK<br>
 *
 * \section Description
 *
 * The demo relies on several FreeRTOS task to work:
 * - DSP task : based on the selected source mode, the DSP task will process data
 *   from a wave file (loopback mode) or from a generated sinus wave (its frequency
 *   can be changed using the potentiometer). The input signal is then modulated
 *   based on the QTouch slider last known position.
 *   The DSP task then performs a Fast Fourrier Transform to compute the magnitude
 *   of the signal.
 * - GFX task : draws the IP configuration menu, then the input signal source and
 *   magnitude on screen.
 * - HTTP task : handles the LwIP stack and HTTPD callbacks to answer remote
 *   client request.
 *   The device IP can either be configured statically (192.168.0.100) or
 *   dynamically using DHCP.
 * - QTouch task : used to compute the slider's position.
 *
 * Once the IP address is configured from the touch screen interface, the board
 * ressources can be accessed from any web browser providing the IP address. For
 * instance, when the IP address is configured statically :
 * <a href="http://192.168.0.100/">
 *
 * The web page gives the following information about the board status:
 * - CPU core type.
 * - Chip name.
 * - SRAM size.
 * - Flash size.
 * - Audio out connector.
 * - Boot from memory.
 * - Board IP.
 * - Your IP.
 * - Total number of HTTP status requests.
 * - FreeRTOS uptime.
 * - Temperature sensor value.
 * - Led status.
 * - Potentiometer value.
 * - Graph of the FFT input signal magnitude (range from 0 to 10 khz).
 *
 * The web page is periodically refreshed to keep the board status up-to-date.
 * With Chrome browser (V25), sometimes two http webpage requests are needed to
 * get correct status periodical update on browser. This is a known issue.
 * And please refer to the datasheet for the temperature sensor accuracy.
 *
 * For IAR Embedded Workbench IDE project, the source supports both FPU on
 * and FPU off cases. FPU on or FPU off can be configured in
 * IAR project "General Options" by selecting "VFPv4" or "None" respectively.
 * With FPU on, hardware floating-point instruction is used, which can improve
 * the performance of floating-point arithmetic in the DSP task.
 *
 */

#include "task.h"
#include "ethernet.h"
#include "status_codes.h"
#include "conf_uart_serial.h"
#include "uart_serial.h"
#include "stdio_serial.h"

/*
 * FreeRTOS hook (or callback) functions that are defined in this file.
 */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle pxTask,
		signed char *pcTaskName);
void vApplicationTickHook(void);



static void prvSetupHardware(void)
{
	/* ASF function to setup clocking. */
	sysclk_init();

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_SetPriorityGrouping(0);

	/* Atmel library function to setup for the evaluation kit being used. */
	board_init();
}

/**
 * Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/** Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

int main(void)
{
	/* Prepare the hardware to run this demo. */
	prvSetupHardware();
#if SAM4E
	NVIC_DisableIRQ(ARM_IRQn);
#endif
	/** Initilize the SAM system */
	sysclk_init();
	board_init();

	/** Initialize the console uart */
	configure_console();

	printf("Program started...!\r\n");

	/** Create GFX task. */
	create_gfx_task(mainGFX_TASK_STACK_SIZE, mainGFX_TASK_PRIORITY);

	/** Create WebServer task. */
	create_http_task(mainHTTP_TASK_STACK_SIZE, mainHTTP_TASK_PRIORITY);

	/** Start the RTOS scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for (;;) {
	}

	return 0;
}

void vApplicationMallocFailedHook(void)
{
	/**
	 * vApplicationMallocFailedHook() will only be called if
	 * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.
	 * It is a hook function that will get called if a call to
	 * pvPortMalloc() fails. pvPortMalloc() is called internally by
	 * the kernel whenever a task, queue, timer or semaphore is created.
	 * It is also called by various parts of the demo application.
	 * If heap_1.c or heap_2.c are used, then the size of the heap
	 * available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	 * FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can
	 * be used to query the size of free heap space that remains
	 * (although it does not provide information on how the remaining heap
	 * might be fragmented).
	 */
	taskDISABLE_INTERRUPTS();
	for (;;) {
	}
}

void vApplicationIdleHook(void)
{
	/**
	 * vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK
	 * is set to 1 in FreeRTOSConfig.h.  It will be called on each iteration
	 * of the idle task.  It is essential that code added to this hook
	 * function never attempts to block in any way (for example, call
	 * xQueueReceive() with a block time specified, or call vTaskDelay()).
	 * If the application makes use of the vTaskDelete() API function
	 * (as this demo application does) then it is also important that
	 * vApplicationIdleHook() is permitted to return to its calling
	 * function, because it is the responsibility of the idle task to
	 * clean up memory allocated by the kernel to any task that has
	 * since been deleted.
	 */
}

void vApplicationStackOverflowHook(xTaskHandle pxTask,
		signed char *pcTaskName)
{
	(void) pcTaskName;
	(void) pxTask;

	/**
	 * Run time stack overflow checking is performed if
	 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.
	 * This hook function is called if a stack overflow is
	 * detected.
	 */
	taskDISABLE_INTERRUPTS();
	for (;;) {
	}
}

void vApplicationTickHook(void)
{
	/**
	 * This function will be called by each tick interrupt if
	 * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.
	 * User code can be added here, but the tick hook is called from
	 * an interrupt context, so code must not attempt to block,
	 * and only the interrupt safe FreeRTOS API
	 * functions can be used (those that end in FromISR()).
	 */
}
