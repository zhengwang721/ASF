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
#include <ctype.h>
#include "user_interface.h"
#include "sio2host.h"
# include "sio2ncp.h"
#include "conf_board.h"

/* === PROTOTYPES ========================================================== */
static void app_task(void);
static void app_alert(void);
static retval_t trx_215_init(void);
bool app_debounce_switch(void);
bool switch_pressed(void);
void start_ctx();
void frame_tx_handler(void);
void ncp_serial_handler(void);
void stop_ctx();
void sio_set_txpwr(void);
void sio_set_channel(void);
void set_rf24_channel(uint8_t channel);
uint8_t read_value_in_hex(uint8_t *value);
void trx_215_irq_handler_cb(void);
retval_t transmit_frame(uint16_t dst_addr,uint8_t seq_num,uint8_t *payload,uint8_t payload_length,bool ack_req,csma_mode_t csma_mode);
static uint8_t storage_buffer[LARGE_BUFFER_SIZE];

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
	BBC1_RXFS     = (0x01),

	/** Constant TRX_IRQ_6_TRX_UR for sub-register @ref SR_IRQ_6_TRX_UR */
	BBC1_RXFE      = (0x02),

	/** Constant TRX_IRQ_5_AMI for sub-register @ref SR_IRQ_5_AMI */
	BBC1_RXAM         = (0x04),

	/** Constant TRX_IRQ_4_CCA_ED_DONE for sub-register @ref
	 *SR_IRQ_4_CCA_ED_DONE */
	BBC1_RXEM = (0x08),

	/** Constant TRX_IRQ_3_TRX_END for sub-register @ref SR_IRQ_3_TRX_END */
	BBC1_TXFE     = (0x10),

	/** Constant TRX_IRQ_2_RX_START for sub-register @ref SR_IRQ_2_RX_START
	 **/
	BBC1_AGCH    = (0x20),
	
	BBC1_AGCR    = (0x40),


	/** No interrupt is indicated by IRQ_STATUS register */
	BBC1_NOIRQ = (0x00)
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
	
	sio2ncp_init();
	
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
		cpu_irq_enable();
		pal_trx_irq_en();
		
		pal_trx_irq_init((FUNC_PTR)trx_215_irq_handler_cb);

		/*Set PHY Mode as Legacy OQPSK*/
		
		pal_trx_bit_write(SR_BBC1_PC_PT,0X03);  //OQPSK
		pal_trx_bit_write(SR_BBC1_OQPSKPHRTX_LEG,0X01);	
		pal_trx_bit_write(SR_BBC1_OQPSKC0_FCHIP,0X03); //2000
		pal_trx_bit_write(SR_BBC1_PC_FCST,0X01);
		
		//Front end config for 2000kchips/s
		pal_trx_bit_write(SR_RF24_TXCUTC_PARAMP,0X00);
		pal_trx_bit_write(SR_RF24_TXCUTC_LPFCUT,0X0B);
		pal_trx_bit_write(SR_RF24_TXDFE_SR,0X01);
		pal_trx_bit_write(SR_RF24_TXDFE_RCUT,0X04);
		
		pal_trx_reg_write(RG_RF24_CMD,0X03); //TRX_PREP*/		
	
		pal_trx_bit_write(SR_RF24_PAC_TXPWR,0X1A);	
		
		set_rf24_channel(11); //2405MHZ
		
		sio2ncp_getchar();
		sio2ncp_tx("Bsic Func Tests -> SAM4L-XPRO-RF215 - RF24 Legacy OQPSK Configuration\n\n\r",75);
		sio2ncp_tx("Press 's' to toggle between CTX Start/Stop\n\r",45);
		sio2ncp_tx("Press 'c' to Change the Channel \n\r",35);
		sio2ncp_tx("Press 't' to Change the Transmit Power \n\r",42);
		sio2ncp_tx("Press 'f' to Transmit a dummy payload \n\r",41);
		/* RX Settings */
		//pal_trx_bit_write(SR_BBC1_PC_FCSFE,0X00);//FCS filter disabled
		//pal_trx_bit_write(SR_BBC1_AFC0_PM,0X01);
		//pal_trx_bit_write(SR_BBC1_OQPSKC2_RXM,0X02);
		//pal_trx_reg_write(RG_RF24_CMD,0X05); //RX

		//Prepare frame for CTX
		//start_ctx();
		

while(1)
{
	app_task();	
	
}

}

/**
 * \brief Application task
 */
static void app_task(void)
{

ncp_serial_handler();
	
}

void ncp_serial_handler(void)
{
	    uint8_t i;
		char input_char[3] = {0, 0, 0};
	    uint8_t input;
	    uint8_t channel;
	static bool ctx = false;
		uint8_t chars = sio2ncp_getchar_nowait();
		if(chars == 's')
		{
			if(ctx==false)
			{
						
			start_ctx();
			ctx=true;
			}
			else
			{
				stop_ctx();
				ctx=false;
			}			

		}
		
		if(chars == 'c')
		{
		sio_set_channel();
		}
		
		if(chars == 't')
		{
			sio_set_txpwr();
		}
		
		if(chars == 'f')
		{
			frame_tx_handler();
		}

		}
		
void sio_set_txpwr(void)		
{
	uint8_t reg_val;
	char chars[3];
	sio2ncp_tx("\n\rEnter TX Pwr Reg - 0X",23);
	if (read_value_in_hex(&reg_val) == true)
	{

	if (reg_val < 0X00 || reg_val > 0X1F )

	{
		sio2ncp_tx("\r\n Out of Range register value.. Press any key....",40);
		sio2ncp_getchar();
		return;
	}
	
	pal_trx_bit_write(SR_RF24_PAC_TXPWR,reg_val);
	sio2ncp_tx("\n\rTX Pwr Reg set \n\r",17);
/*
	itoa(reg_val,chars,10);
	sio2ncp_tx(chars,3);
	sio2ncp_tx("\n\r",2);*/
}
	
}
void sio_set_channel()
{
	char input_char[3] = {0, 0, 0};
	uint8_t i;
	uint8_t input;
	uint8_t channel;
	sio2ncp_tx("\r\nEnter channel (11..26) and press 'Enter': ",45);

	for (i = 0; i < 3; i++)
	{
		input = sio2ncp_getchar();
		if (((input < '0') || (input > '9')) && (input != '\r'))
		{
			sio2ncp_tx("\r\n Wrong value...",20);
			return;
		}
		else
		{
			if ((i == 0) && (input == '\r'))
			{
				sio2ncp_tx("\r\n Wrong value...",20);
				return;
			}
			else if (input == '\r')
			{
				break;

			}
			else
			{
				input_char[i] = input;
			}
		}
	}
	
	channel = atol(input_char);
	if((channel > 26 )|| (channel <11))
	{

	sio2ncp_tx("\r\n Wrong value...",20);
	return;
	}
	set_rf24_channel(channel);
	sio2ncp_tx("\n\rChannel set to ",18);
	sio2ncp_tx(input_char,3);


}

void start_ctx()
{
	uint8_t data[120];
	for(uint8_t i =0;i<120;i++)
	{
		data[i] = (uint8_t)rand();
	}
	
	pal_trx_reg_write(RG_BBC1_TXFLL,120); //Set Frame Length
	pal_trx_frame_write(data,120);
	pal_trx_bit_write(SR_BBC1_PC_CTX,0X01);
	pal_trx_reg_write(RG_RF24_CMD,0X04); //TX
	sio2ncp_tx("\n\rStarting CTX",15);
	
}

void stop_ctx()
{
	
	pal_trx_bit_write(SR_BBC1_PC_CTX,0X00);
	sio2ncp_tx("\n\rStopping CTX",15);

}
bool app_debounce_switch(void)
{
	
	uint8_t ret = 0;
	static uint8_t key_cnt;
	/*Read the current state of the button*/
	
	if (switch_pressed()) // Button Pressed
	{
		if (key_cnt != 10)
		{
			key_cnt++;
		}
	}
	else if (!(switch_pressed()) && (key_cnt == 10)) //Button released
	{
		ret = 1;
		key_cnt = 0;
	}
	else
	{
		key_cnt = 0;
	}
	return ret;
}


void frame_tx_handler()
{
static uint8_t i=5;
	//uint8_t key_press;
	/* Check for any key press */
	//key_press = app_debounce_switch();
	//if(key_press != 0)
	{
		transmit_frame(0X0001,i++,"Hello World",11,true,NO_CSMA_NO_IFS);
		sio2ncp_tx("\n\rDummy packet transmitted in Current Channel\n\r",45);
		i++;
	}
}

bool switch_pressed(void)
{
	#if defined GPIO_PUSH_BUTTON_0
	/*Read the current state of the button*/
	if (ioport_get_pin_level(GPIO_PUSH_BUTTON_0))
	{
		return false;
	}
	else
	{
		return true;
	}
	#else
	return false;
	#endif
}

retval_t transmit_frame(uint16_t dst_addr,
		uint8_t seq_num,
		uint8_t *payload,
		uint8_t payload_length,
		bool ack_req,csma_mode_t csma_mode)
{
	frame_info_t *tx_frame_info;
	uint8_t i;
	uint8_t frame_length;
	uint8_t *frame_ptr;
	uint8_t *temp_frame_ptr;
    
	/* Get length of current frame. */

	tx_frame_info = (frame_info_t *)storage_buffer;
	/* Get length of current frame. */
	frame_length = (11 + payload_length);

	/* Set payload pointer. */
	frame_ptr = temp_frame_ptr = (uint8_t *)tx_frame_info +
	LARGE_BUFFER_SIZE -
	payload_length - FCS_LEN;		


	/*
	* Payload is stored to the end of the buffer avoiding payload
	* copying by TAL.
	*/
	for (i = 0; i < payload_length; i++) {
	*temp_frame_ptr++ = *(payload + i);
	}


	frame_ptr -=2 ;
	*(uint16_t *)frame_ptr = 0X0005;
	
	frame_ptr -=2 ;
	*(uint16_t *)frame_ptr = dst_addr;
	
	frame_ptr -=2 ;
	*(uint16_t *)frame_ptr = SRC_PAN_ID;
	
	frame_ptr--;
	*frame_ptr = seq_num++;
	
	frame_ptr -= 2;
	*(uint16_t *)frame_ptr = 0X8841;
	
	frame_ptr--;
	*frame_ptr = frame_length;

	/* Finished building of frame. */
	tx_frame_info->mpdu = frame_ptr;
	
	tx_frame_info->mpdu = frame_ptr;
	
	pal_trx_reg_write(RG_BBC1_TXFLL,*frame_ptr); //Set Frame Length 

	//pal_trx_bit_write(SR_BBC1_PC_CTX,0X01);
	pal_trx_reg_write(RG_RF24_CMD,0X04);
	pal_trx_frame_write((frame_ptr+1),*frame_ptr);

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
	//pal_trx_reg_write(RG_RF24_CMD,0X07); //RESET CMD 
	
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
	
			pal_trx_irq_flag_clr();
			pal_trx_irq_en(); /* Enable transceiver main interrupt. */
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
	
		pal_trx_reg_write(RG_BBC1_IRQM,0XFF); //Enable fS and FE
		pal_trx_reg_write(RG_RF24_IRQM,0XFF);
	return MAC_SUCCESS;
	
}

void trx_215_irq_handler_cb(void)
{

trx_rf24_irq_reason_t irq_status ;
trx_bbc1_irq_reason_t irq_status1;
irq_status = (trx_rf24_irq_reason_t)pal_trx_reg_read(RG_RF24_IRQS);
irq_status=irq_status;
irq_status1 = (trx_bbc1_irq_reason_t)pal_trx_reg_read(RG_BBC1_IRQS);
pal_trx_reg_read(RG_RF09_IRQS);
pal_trx_reg_read(RG_BBC0_IRQS);

if(RF24_WAKEUP & irq_status)
{
/*Write Code here for Transceiver Wakeup*/
}

if(RF24_TRXRDY & irq_status)
{
	//LED_On(LED0);
	/*Write Code here for Transceiver Wakeup*/
}

if(RF24_TRXERR & irq_status)
app_alert();

if(BBC1_TXFE & irq_status1)
{
	pal_trx_reg_read(RG_BBC1_PC);
	if(pal_trx_reg_read(RG_BBC1_PS))
	return;
	else
	{
		uint8_t len = pal_trx_reg_read(RG_BBC1_TXFLL);
	for(int i =0;i<len;i++)
	{
	pal_trx_reg_read(RG_BBC1_FBTXS+i);
	}
	pal_trx_reg_read(RG_BBC1_FBLVL);
	LED_Toggle(LED0);
	}

}
}

void set_rf24_channel(uint8_t channel)
{
	pal_trx_reg_write(RG_RF24_CS,0XC8);
	pal_trx_reg_write(RG_RF24_CCF0L,0X68); //2405
	pal_trx_reg_write(RG_RF24_CCF0H,0X8D);
	pal_trx_reg_write(RG_RF24_CNL,channel-11);
	pal_trx_reg_write(RG_RF24_CNM,0X00);

	
}

uint8_t read_value_in_hex(uint8_t *value)
{
	
	char input_char[3] = {0, 0, 0};
	uint8_t i;
	uint8_t input;
	uint8_t temp;

	for (i = 0; i < 2; i++)
	{
		input = sio2ncp_getchar();
		input = toupper(input);
		if (((input < '0') || (input > '9')) &&
		((input < 'A') || (input > 'F')) && (input != '\r') )
		{
			sio2ncp_tx("\r\n Wrong value.. Press any key....",30);
			sio2ncp_getchar();
			return(false);
		}
		else
		{
			/* First key pressed is 'Enter' */
			if ((i == 0) && (input == '\r'))
			{
				sio2ncp_tx("\r\n Wrong value.. Press any key....",30);
				sio2ncp_getchar();
				return(false);
			}
			else if (input == '\r')/* Proess and don't wait for the next character */
			{
				*(value) = input_char[i - 1] ;
				return(true);
			}
			else /* Process and wait for next character until last char */
			{
				temp = input - 0x30;
				if (temp > 9)
				{
					temp = temp - 7;
				}
				input_char[i] = temp;
			}
		}
	}
	temp = input_char[0];
	temp = temp << 4;
	temp |= input_char[1];

	*(value) = temp;
	return(true);
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
