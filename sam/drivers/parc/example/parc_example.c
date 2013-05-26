/**
 * \file
 *
 * \brief USART Serial example for SAM.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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
 * \mainpage USART Serial Example
 *
 * \par Purpose
 * This example demonstrates the normal (serial) mode provided by the USART
 * peripherals.
 *
 * \par Requirements
 *  This package can be used with SAM4L EK with USART and PDCA.
 *
 * \par Description
 *
 * On start up, the debug information is dumped to on-board USART port.
 * A terminal application, such as HyperTerminal, is used to monitor these
 * debug information. Then the program works in ECHO mode, so USART will
 * send back anything it receives from the HyperTerminal.  You can send a text
 * file from the HyperTerminal connected with USART port to the device (without
 * any protocol such as X-modem).
 *
 * \par Usage
 *
 * -# Build the program and download it into the evaluation boards.
 * -# Connect a serial cable to the USART port for the evaluation kit.
 * -# On the computer, open and configure a terminal application
 *    (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- USART Serial Example --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     -- Start to echo serial inputs with PDCA --
 *    \endcode
 * -# Send a file in text format from the HyperTerminal connected with USART
 *    port to the device. On HyperTerminal, this is done by selecting
 *    "Transfer -> Send Text File"(this does not prevent you from sending
 *    binary files). The transfer will start and then you could read the file
 *    in the HyperTerminal.
 *
 */

#include <string.h>
#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_example.h"
#include "ioport.h"
#include <sysclk.h>
//#include "parc.h"     
/** Size of the receive buffer used by the PDCA, in bytes. */
#define BUFFER_SIZE         100

/** Max buffer number. */
#define MAX_BUF_NUM         1

/** All interrupt mask. */
#define ALL_INTERRUPT_MASK  0xffffffff

/** Timer counter frequency in Hz. */
#define TC_FREQ             10

#define STRING_EOL    "\r"
#define STRING_HEADER "-- USART Serial Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

#define PDCA_PARC_CHANNEL     0
#define PDCA_TX_CHANNEL       1
#define PDCA_PID_PARC         16
#define PDCA_PID_USART2_TX    20

/** Receive buffer. */
static uint8_t gs_puc_buffer[2][BUFFER_SIZE];

/** Next Receive buffer. */
static uint8_t gs_puc_nextbuffer[2][BUFFER_SIZE];

/** Current bytes in buffer. */
static uint32_t gs_ul_size_buffer = BUFFER_SIZE;

/** Current bytes in next buffer. */
static uint32_t gs_ul_size_nextbuffer = BUFFER_SIZE;

/** Buffer number in use. */
static uint8_t gs_uc_buf_num = 0;

/** Flag of one transfer end. */
static uint8_t g_uc_transend_flag = 0;

/** PARC configure stuct */
struct parc_config parc_configex;

/** PDCA channel options. */
pdca_channel_config_t PDCA_PARC_OPTIONS = {
	.addr = (void *)gs_puc_buffer, /* memory address */
	.pid = PDCA_PID_PARC, /* select peripheral - USART0 RX line.*/
	.size = BUFFER_SIZE, /* transfer counter */
	.r_addr = (void *)gs_puc_nextbuffer, /* next memory address */
	.r_size = BUFFER_SIZE, /* next transfer counter */
	.transfer_size = PDCA_MR_SIZE_BYTE /* select size of the transfer */
};
pdca_channel_config_t PDCA_TX_OPTIONS = {
	.addr = (void *)gs_puc_buffer, /* memory address */
	.pid = PDCA_PID_USART2_TX, /* select peripheral - USART0 TX line.*/
	.size = 0, /* transfer counter */
	.r_addr = (void *)gs_puc_nextbuffer, /* next memory address */
	.r_size = 0, /* next transfer counter */
	.transfer_size = PDCA_MR_SIZE_BYTE /* select size of the transfer */
};

/**
 * \brief Interrupt handler for USART. Echo the bytes received and start the
 * next receive.
 */
void USART_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status. */
	ul_status = usart_get_status(BOARD_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		/* Disable timer. */
		tc_stop(TC0, 0);

		/* Echo back buffer. */
		pdca_channel_write_load(PDCA_TX_CHANNEL,
				(void *)gs_puc_buffer[gs_uc_buf_num],
				gs_ul_size_buffer);
		pdca_channel_write_reload(PDCA_TX_CHANNEL,
				(void *)gs_puc_nextbuffer[gs_uc_buf_num],
				gs_ul_size_nextbuffer);

		if (g_uc_transend_flag) {
			gs_ul_size_buffer = BUFFER_SIZE;
			gs_ul_size_nextbuffer = BUFFER_SIZE;
			g_uc_transend_flag = 0;
		}

		gs_uc_buf_num = MAX_BUF_NUM - gs_uc_buf_num;

		/* Restart read on buffer. */
		pdca_channel_write_load(PDCA_PARC_CHANNEL,
				(void *)gs_puc_buffer[gs_uc_buf_num], BUFFER_SIZE);
		pdca_channel_write_reload(PDCA_PARC_CHANNEL,
				(void *)gs_puc_nextbuffer[gs_uc_buf_num], BUFFER_SIZE);

		/* Restart timer. */
		tc_start(TC0, 0);
	}
}
/**
 * \brief PARC input signal generating. Use IO pins to simulate PCD data.
 */
static volatile void PARC_input(uint8_t data)
{
    ioport_pin_t pin;
    
    
    int8_t i = 0;
	for(;i<8;i++)
	{
		pin = PIN_PC00+i;
		if(pin == PIN_PC05)
		{
			data = data >> 1;

		}
		        if(data & 0x01)
		        {
			        ioport_set_pin_level(pin,IOPORT_PIN_LEVEL_HIGH);
			        }else{
			        ioport_set_pin_level(pin,IOPORT_PIN_LEVEL_LOW);
		        }
		        data = data >> 1;
		
	}
    //for(;i<4;i++)
    //{
        //pin = PIN_PB08 + i;
        //if(data & 0x01)
        //{
            //ioport_set_pin_level(pin,IOPORT_PIN_LEVEL_HIGH);
        //}else{
            //ioport_set_pin_level(pin,IOPORT_PIN_LEVEL_LOW);
        //}
        //data = data >> 1;
    //}
    //
    //for(;i<8;i++)
    //{
           //pin = PIN_PC15 + i - 4;
        //if(data & 0x01)
        //{
            //ioport_set_pin_level(pin,IOPORT_PIN_LEVEL_HIGH);
        //}else{
            //ioport_set_pin_level(pin,IOPORT_PIN_LEVEL_LOW);
        //}
        //data = data >> 1;
    //}
}
/**
 * \brief Interrupt handler for TC00. Record the number of bytes received,
 * and then restart a read transfer on the USART if the transfer was stopped.
 */
void TC00_Handler(void)
{
	uint32_t ul_status;
	uint32_t ul_byte_total = 0;

	/* Read TC0 Status. */
	ul_status = tc_get_status(TC0, 0);

        /* Toggel IO pin to simulate the PCCK */
        ioport_toggle_pin_level(PIN_PA06);
        static int8_t ii = '0';
        static int8_t cnt = 0;
        /* Simulate PCD data */
        PARC_input(ii++);
        if(ii > '9')
        {
            ii = '0';
        }
}

/**
 * \brief Configure Timer Counter 0 (TC0) to generate an interrupt every 200ms.
 * This interrupt will be used to flush USART input and echo back.
 */
static void configure_tc(void)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	static uint32_t ul_pbaclk;

	/* Configure clock service. */
	sysclk_enable_peripheral_clock(TC0);

	/* Get system clock. */
	ul_pbaclk = sysclk_get_peripheral_bus_hz(TC0);

	/* Configure TC for a 1Hz frequency and trigger on RC compare. */
	tc_find_mck_divisor(TC_FREQ, ul_pbaclk, &ul_div, &ul_tcclks, ul_pbaclk);
	tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC0, 0, (ul_pbaclk / ul_div) / TC_FREQ/10);

	/* Configure and enable interrupt on RC compare. */
	NVIC_EnableIRQ(TC00_IRQn);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
}

/**
 *  Configure UART for debug message output.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}


static void PDCA_PARC_handler(enum pdca_channel_status status)
{
     puts("\r\ncomplete 1 frame\r\n");
     
     static bool switching = false;
     if(!switching)
     {
        pdca_channel_write_load(PDCA_TX_CHANNEL,
                                 (void *)gs_puc_buffer[gs_uc_buf_num],
                                 BUFFER_SIZE); 
             /* Restart read on buffer. */
        pdca_channel_write_reload(PDCA_PARC_CHANNEL,
				(void *)gs_puc_buffer[gs_uc_buf_num], BUFFER_SIZE);
     }else{
                  pdca_channel_write_load(PDCA_TX_CHANNEL,
                                 (void *)gs_puc_nextbuffer[gs_uc_buf_num],
                                 gs_ul_size_buffer); 
        pdca_channel_write_reload(PDCA_PARC_CHANNEL,
                                  (void *)gs_puc_nextbuffer[gs_uc_buf_num], BUFFER_SIZE);
     }
     switching = !switching;
}

void parallel_port_source_simulation_config()
{
ioport_set_pin_dir(PIN_PA06, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PA06,IOPORT_PIN_LEVEL_HIGH);
ioport_set_pin_dir(PIN_PC01, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PC01,IOPORT_PIN_LEVEL_HIGH);
ioport_set_pin_dir(PIN_PC02, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PC02,IOPORT_PIN_LEVEL_HIGH);
ioport_set_pin_dir(PIN_PC03, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PC03,IOPORT_PIN_LEVEL_HIGH);
ioport_set_pin_dir(PIN_PC04, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PC04,IOPORT_PIN_LEVEL_HIGH);
ioport_set_pin_dir(PIN_PC05, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PC05,IOPORT_PIN_LEVEL_HIGH);
ioport_set_pin_dir(PIN_PC06, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PC06,IOPORT_PIN_LEVEL_HIGH);
ioport_set_pin_dir(PIN_PC17, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PC17,IOPORT_PIN_LEVEL_HIGH);
ioport_set_pin_dir(PIN_PC18, IOPORT_DIR_OUTPUT);
ioport_set_pin_level(PIN_PC18,IOPORT_PIN_LEVEL_HIGH);
}

/**
 * \brief Application entry point for usart_serial example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
    static char chr[4];
    static uint32_t temp;
    
    /* Initialize the SAM system. */
    sysclk_init();
    board_init();
	
	/* Configure UART for debug message output. */
	configure_console();

//pmc_enable_periph_clk(ID_PARC);
sysclk_enable_peripheral_clock(PARC);
	struct parc_module module_inst;
	struct parc_config config;
	/* PARC config */
	parc_get_config_defaults(&config);
	parc_init(&module_inst, PARC, &config);
	parc_enable(&module_inst);

	parc_start_capture(&module_inst);
	
/* Output example information. */
puts(STRING_HEADER);
puts("-- PARC capture data -- \r\n\r");

    if(parc_get_status(&module_inst) & PARC_STATUS_EN)
    {
        puts("--PARC Enabled \r\n\r");
    } else{
        puts("--PARC not Enabled \r\n\r");
    }
    if(parc_get_status(&module_inst) & PARC_STATUS_CS)
    {
        puts("--PARC in Capture \r\n\r");
    }else{
        puts("--PARC not in Capture \r\n\r");
    }
    /* Configure TC. */
    configure_tc();
    /* Start timer. */
    tc_start(TC0, 0);
    
    /* Enable PDCA module clock */
    pdca_enable(PDCA);
    /* Init PDCA channel with the pdca_options.*/
    pdca_channel_set_config(PDCA_PARC_CHANNEL, &PDCA_PARC_OPTIONS);
    pdca_channel_set_config(PDCA_TX_CHANNEL, &PDCA_TX_OPTIONS);
    /* Set callback for PDCA interrupt. */
    pdca_channel_set_callback(PDCA_PARC_CHANNEL,PDCA_PARC_handler,PDCA_0_IRQn,1,PDCA_IER_RCZ);
    /* Enable PDCA channel, start receiving data. */
    pdca_channel_enable(PDCA_PARC_CHANNEL);
    pdca_channel_enable(PDCA_TX_CHANNEL);
    /* Start read PARC data capture via PDCA. */
    pdca_channel_write_load(PDCA_PARC_CHANNEL,
                            (void *)gs_puc_buffer[gs_uc_buf_num], BUFFER_SIZE);
    pdca_channel_write_reload(PDCA_PARC_CHANNEL,
                              (void *)gs_puc_nextbuffer[gs_uc_buf_num], BUFFER_SIZE);
    /* Empty main loop. */    
    while(1)
    {
    }
}
