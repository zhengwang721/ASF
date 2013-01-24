/**
 * \file main.c
 *
 * \brief  Serial Bridge Application
 *
 * Copyright (c) 2009 Atmel Corporation. All rights reserved.
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
 * \page license License
 * Copyright(c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
 
 /**
  * \mainpage Serial Bridge Application
  */

/* === INCLUDES ============================================================ */

#include <stdlib.h>
#include "sio2host.h"
#include "sio2ncp.h"
#include "asf.h"

/* === PROTOTYPES =============================================================== */

void app_alert(void);
void serial_handler(void);

/* === MACROS ============================================================== */
#define SIO_RX_BUF_SIZE                 (32)

/* === GLOBALS ============================================================== */

uint8_t length_received_host , length_received_ncp;	
uint8_t temp[SIO_RX_BUF_SIZE];


/* === IMPLEMENTATION ====================================================== */


/**
 * \brief Main function of the Serial Bridge application
 */
int main(void)
{
    irq_initialize_vectors();

	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
    sysclk_init();
	board_init();

    cpu_irq_enable();

    if ( STATUS_OK != sio2host_init() )
    {
        /* something went wrong during initialization */
        app_alert();
    }
    
    if (STATUS_OK != sio2ncp_init())
    {
        /* something went wrong during initialization */
        app_alert();
    }
    while (1)
    {
        serial_handler();
    }
}


void serial_handler()
{

	length_received_host = sio2host_rx(temp,SIO_RX_BUF_SIZE);
	if(length_received_host !=0)
	{
		sio2ncp_tx(temp,length_received_host);
		length_received_host = 0;
	}

	length_received_ncp = sio2ncp_rx(temp,SIO_RX_BUF_SIZE);
	if(length_received_ncp !=0)
	{
		sio2host_tx(temp,length_received_ncp);
		length_received_ncp = 0;
	} 
}


void app_alert()
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
/* EOF */
