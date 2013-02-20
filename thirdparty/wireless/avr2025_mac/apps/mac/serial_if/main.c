/**
 * \file main.c
 *
 * \brief MAC Serial Interface Application 
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
 */

/**
* \mainpage
* \section preface Preface
* This is the reference manual for the Serial Interface Application
* \section toc Table of Contents
*  - \subpage overview
*  -  \b Application \b Interface(API)
*    - \ref group_mac
*    - \ref group_pal
*    - \ref group_tal
*    - \ref group_resources
*  - \subpage main_files
*  - \subpage devsup
*  - \subpage compinfo
*  - \subpage references
*  - \subpage contactinfo
*/

/**
 * \page overview Overview
 * \section intro Introduction
 * To be Added
 */


/** \page main_files Application Files
 * - main.c\n                      Application main file.

 * \page devsup Device Support
 * - \b ATXMEGA256A3BU
 *                     - <A href="http://www.atmel.com/tools/xmega-a3buxplained.aspx"> \b   XMEGA-A3BU Xplained  </A>  <A href="http://store.atmel.com/PartDetail.aspx?q=p:10500293">\a Buy </A>\n
 * - \b ATXMEGA256A3U-Zigbit (USB and Carrier)
 * - \b Atmega256rfr2 Xplained Pro
 * - \b Atmega256rfr2 Zigbit (Carrier)
 * - \b UC3A3256S
 *                      - <A href="http://www.atmel.com/tools/rz600.aspx"> \b RZ600 </A> <A href="http://store.atmel.com/PartDetail.aspx?q=p:10500245;c:100118">\a Buy </A>\n
 * \page compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 * \page references References
 * 1)  IEEE Std 802.15.4-2006 Part 15.4: Wireless Medium Access Control (MAC)
 *     and Physical Layer (PHY) Specifications for Low-Rate Wireless Personal Area
 *     Networks (WPANs).\n\n
 * 2)  AVR Wireless Support <A href="http://avr@atmel.com">avr@atmel.com</A>.\n
 * \page contactinfo Contact Information
 * For further information,visit
 * <A href="http://www.atmel.com/avr">www.atmel.com</A>.\n
 */
 
#include "conf_board.h"
#include "avr2025_mac.h"
#include "serial_interface.h"
#include "common_sw_timer.h"
#include <asf.h>

/** Alert to indicate something has gone wrong in the application */
static void app_alert(void);

#define LED_POWER     (LED0)

/*! \brief Main function.
 */
int main(void)
{
	irq_initialize_vectors();

	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	board_init();
	sysclk_init();
	sw_timer_init();

	if(MAC_SUCCESS != wpan_init())
	{
		app_alert();
	}
	LED_On(LED_POWER);
	cpu_irq_enable();
	serial_interface_init();

	while (1)
	{
		wpan_task();
		serial_data_handler();
	}
}

/* Alert to indicate something has gone wrong in the application */
static void app_alert(void)
{
    while (1)
    {
     
		#if LED_COUNT > 0
		LED_Toggle(LED0);
		#endif

		#if LED_COUNT > 1
		LED_Toggle(LED1);
		#endif

		#if LED_COUNT > 2
		LED_Toggle(LED2);
		#endif

		#if LED_COUNT > 3
		LED_Toggle(LED3);
		#endif

		#if LED_COUNT > 4
		LED_Toggle(LED4);
		#endif

		#if LED_COUNT > 5
		LED_Toggle(LED5);
		#endif

		#if LED_COUNT > 6
		LED_Toggle(LED6);
		#endif

		#if LED_COUNT > 7
		LED_Toggle(LED7);
		#endif
		delay_us(0xFFFF);
	}
}

