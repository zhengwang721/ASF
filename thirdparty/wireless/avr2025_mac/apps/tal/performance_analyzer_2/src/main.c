/**
 * \file main.c
 *
 * \brief  Main of TAL Examples - Performance_Analyzer application
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
 * \page license License
 * Copyright(c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */
#include <stdlib.h>
#include "tal.h"
#include "tal_helper.h"
#include "ieee_const.h"
#include "at86rf215.h"
#include "asf.h"
#include "sio2host.h"
# include "sio2ncp.h"
#include "conf_board.h"

/* === PROTOTYPES ========================================================== */
static void app_task(void);
static void app_alert(void);
static retval_t trx_215_init(void);
void trx_215_irq_handler_cb(void);
/**
 * \brief Main function of the Performance Analyzer application
 * \ingroup group_app_init
 */

typedef enum trx_215_irq_reason_tag {
	/** Constant TRX_IRQ_7_BAT_LOW for sub-register @ref SR_IRQ_7_BAT_LOW */
	RF24_WAKEUP     = (0x01),

	/** Constant TRX_IRQ_6_TRX_UR for sub-register @ref SR_IRQ_6_TRX_UR */
	RF24_TRXRDY      = (0x02),

	/** Constant TRX_IRQ_5_AMI for sub-register @ref SR_IRQ_5_AMI */
	RF24_EDC         = (0x04),

	/** Constant TRX_IRQ_4_CCA_ED_DONE for sub-register @ref
	 *SR_IRQ_4_CCA_ED_DONE */
	RF24_BATLOW = (0x08),

	/** Constant TRX_IRQ_3_TRX_END for sub-register @ref SR_IRQ_3_TRX_END */
	RF24_TRXERR     = (0x10),

	/** Constant TRX_IRQ_2_RX_START for sub-register @ref SR_IRQ_2_RX_START
	 **/
	RF24_IQIFSF    = (0x20),


	/** No interrupt is indicated by IRQ_STATUS register */
	RF_NOIRQ = (0x00)
} SHORTENUM trx_rf24_irq_reason_t;

typedef enum trx_bbc1_irq_reason_tag {
	/** Constant TRX_IRQ_7_BAT_LOW for sub-register @ref SR_IRQ_7_BAT_LOW */
	BBC24_RXFS     = (0x01),

	/** Constant TRX_IRQ_6_TRX_UR for sub-register @ref SR_IRQ_6_TRX_UR */
	BBC24_RXFE      = (0x02),

	/** Constant TRX_IRQ_5_AMI for sub-register @ref SR_IRQ_5_AMI */
	BBC24_RXAM         = (0x04),

	/** Constant TRX_IRQ_4_CCA_ED_DONE for sub-register @ref
	 *SR_IRQ_4_CCA_ED_DONE */
	BBC24_RXEM = (0x08),

	/** Constant TRX_IRQ_3_TRX_END for sub-register @ref SR_IRQ_3_TRX_END */
	BBC24_TXFE     = (0x10),

	/** Constant TRX_IRQ_2_RX_START for sub-register @ref SR_IRQ_2_RX_START
	 **/
	BBC24_AGCH    = (0x20),
	
	BBC24_AGCR    = (0x40),


	/** No interrupt is indicated by IRQ_STATUS register */
	BBC24_NOIRQ = (0x00)
} SHORTENUM trx_bbc1_irq_reason_t;

int main(void)
{
	irq_initialize_vectors();
	sysclk_init();

	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	board_init();    

	cpu_irq_enable();
	pal_trx_irq_en();
	pal_trx_irq_init((FUNC_PTR)trx_215_irq_handler_cb);
	/*
	 * Power ON - so set the board to INIT state. All hardware, PAL, TAL and
	 * stack level initialization must be done using this function
	 */

	/* Init the PAL and by this means also the transceiver interface */
	if (pal_init() != MAC_SUCCESS) {
		app_alert();
	}

	if (trx_215_init() != MAC_SUCCESS) {
		app_alert();
	}
	
	while(1);


	sio2host_init();
    

	/* Endless while loop */
	while (1) {
		pal_task(); /* Handle platform specific tasks, like serial
		             * interface */
		tal_task(); /* Handle transceiver specific tasks */
		app_task(); /* Application task */
        
	}
}


/**
 * \brief Application task
 */
static void app_task(void)
{

}

/*
 * \brief Callback that is called if data has been received by trx.
 *
 * \param frame Pointer to received frame
 */
void tal_rx_frame_cb(frame_info_t *frame)
{

	/* free buffer that was used for frame reception */
	bmm_buffer_free((buffer_t *)(frame->buffer_header));
}

/*
 * \brief Callback that is called once tx is done.
 *
 * \param status    Status of the transmission procedure
 * \param frame     Pointer to the transmitted frame structure
 */
void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
{

}

/*
 * \brief User call back function for finished ED Scan
 *
 * \param energy_level Measured energy level during ED Scan
 */
void tal_ed_end_cb(uint8_t energy_level)
{

}


static retval_t trx_215_init(void)
{
	
	tal_trx_status_t trx_status;
	uint8_t poll_counter = 0;

	PAL_RST_HIGH();
	

	/* Wait typical time of timer TR1. */
	pal_timer_delay(P_ON_TO_CLKM_AVAILABLE_TYP_US);

	/* Apply reset pulse */
	PAL_RST_LOW();
	pal_timer_delay(RST_PULSE_WIDTH_US);
	PAL_RST_HIGH();

	
	do {
		/* Wait not more than max. value of TR1. */
		if (poll_counter == 10) {
			return FAILURE;
		}

		/* Wait a short time interval. */
		pal_timer_delay(100);
		poll_counter++;
		/* Check if AT86RF233 is connected; omit manufacturer id check
		 **/
	} while (pal_trx_reg_read(RG_RF_PN) != 0X32); //215 Part Num
	
	poll_counter = 0;
	pal_trx_reg_write(RG_RF24_CMD,0X07); //RESET CMD 
	
	do 
	{
		/* Wait not more than max. value of TR1. */
		if (poll_counter == 10) {
			return FAILURE;
		}

		/* Wait a short time interval. */
		pal_timer_delay(100);
		poll_counter++;
	} while (pal_trx_reg_read(RG_RF24_STATE) != 0X02);
	
	/*Test For Initial State Write*/
/*
	/ ** /
		poll_counter = 0;
		
		pal_trx_reg_write(RG_RF24_CMD,0X03); //TRX_PREP
		
		do
		{
			/ * Wait not more than max. value of TR1. * /
			if (poll_counter == 10) {
				return FAILURE;
			}

			/ * Wait a short time interval. * /
			pal_timer_delay(100);
			poll_counter++;
		} while (pal_trx_reg_read(RG_RF24_STATE) != 0X03);*/
	

	return MAC_SUCCESS;
	
}

void trx_215_irq_handler_cb(void)
{

trx_rf24_irq_reason_t irq_status ;
irq_status = (trx_rf24_irq_reason_t)pal_trx_reg_read(RG_RF24_IRQS);
if(RF24_WAKEUP & irq_status)
{
/*Write Code here for Transceiver Wakeup*/
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
