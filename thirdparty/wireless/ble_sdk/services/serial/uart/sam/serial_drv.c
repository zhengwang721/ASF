/**
 * \file serial_drv.c
 *
 * \brief Handles Serial driver functionalities
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
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

/* === INCLUDES ============================================================ */

#include <asf.h>
#include <string.h>
#include "serial_drv.h"
#include "conf_serialdrv.h"
#include "serial_fifo.h"
#include "platform.h"

/* === GLOBALS ========================================================== */
volatile bool ble_usart_tx_cmpl = true;

ser_fifo_desc_t ble_usart_rx_fifo;
uint8_t ble_usart_rx_buf[BLE_MAX_RX_PAYLOAD_SIZE];

extern void platform_pdc_process_rxdata(uint8_t *buf, uint16_t len);
extern void platform_process_rxdata(uint32_t t_rx_data);

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/**
 * \brief Set peripheral mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_pin_peripheral_mode(pin, mode) \
	do {\
		ioport_set_pin_mode(pin, mode);\
		ioport_disable_pin(pin);\
	} while (0)
	
/* === PROTOTYPES ========================================================== */

/* === GLOBALS ========================================================== */

/* === IMPLEMENTATION ====================================================== */

Pdc *ble_usart_pdc;
pdc_packet_t ble_usart_tx_pkt;
pdc_packet_t ble_usart_rx_pkt;
uint8_t pdc_rx_buffer[BLE_MAX_RX_PAYLOAD_SIZE];
//uint8_t pdc_rx_next_buffer[BLE_MAX_RX_PAYLOAD_SIZE]; //Improved with Next Buffer chain

void ble_pdc_send_data(uint8_t *buf, uint16_t len);
static uint8_t configure_primary_uart(void);

extern volatile enum tenuTransportState slave_state;

void ble_pdc_send_data(uint8_t *buf, uint16_t len)
{	
	/* Initialize the Tx buffers for data transfer */
	ble_usart_tx_pkt.ul_addr = (uint32_t)buf;
	ble_usart_tx_pkt.ul_size = len;
	/* Configure the PDC for data transmit */
	pdc_tx_init(ble_usart_pdc, &ble_usart_tx_pkt, NULL);
	
	/* Enable Buffer Empty Interrupt */
	usart_enable_interrupt(BLE_UART, US_IER_TXBUFE);
}

static uint8_t configure_primary_uart(void)
{ 
  	/* Usart async mode 8 bits transfer test */
  	sam_usart_opt_t usart_settings = {
	  	.baudrate     = CONF_UART_BAUDRATE,
	  	.char_length  = US_MR_CHRL_8_BIT,
	  	.parity_type  = US_MR_PAR_NO,
	  	.stop_bits    = US_MR_NBSTOP_1_BIT,
	  	.channel_mode = US_MR_CHMODE_NORMAL,
	  	/* This field is only used in IrDA mode. */
	  	.irda_filter  = 0
  	};
  	
  	ioport_set_pin_peripheral_mode(USART0_RXD_GPIO, USART0_RXD_FLAGS);
  	ioport_set_pin_peripheral_mode(USART0_TXD_GPIO, USART0_TXD_FLAGS);
  	ioport_set_pin_peripheral_mode(USART0_CTS_GPIO, USART0_CTS_FLAGS);
  	ioport_set_pin_peripheral_mode(USART0_RTS_GPIO, USART0_RTS_FLAGS);
  	
  	/* Clock Configuration for UART */
  	sysclk_enable_peripheral_clock(BLE_UART_ID);
  	
  	/* Enable the peripheral and set USART mode. */
  	flexcom_enable(BLE_USART_FLEXCOM);
  	flexcom_set_opmode(BLE_USART_FLEXCOM, FLEXCOM_USART);

  	/* Configure USART */
  	usart_init_rs232(BLE_UART, &usart_settings,
  	sysclk_get_peripheral_hz());
  	
  	/* Enable the receiver and transmitter. */
  	usart_enable_tx(BLE_UART);
  	usart_enable_rx(BLE_UART);
  	
  	ble_usart_pdc = usart_get_pdc_base(BLE_UART);
  	
  	/* Initialize the Rx buffers for data receive */
  	ble_usart_rx_pkt.ul_addr = (uint32_t)pdc_rx_buffer;
  	ble_usart_rx_pkt.ul_size = 1; //Initial BLE Frame size- Sync Packet size
  	
  	/* Configure the PDC for data receive */
  	pdc_rx_init(ble_usart_pdc, &ble_usart_rx_pkt, NULL);
  	
  	pdc_enable_transfer(ble_usart_pdc, PERIPH_PTCR_RXTEN | PERIPH_PTSR_TXTEN);
  	
  	/* Enable UART IRQ */
  	usart_enable_interrupt(BLE_UART, US_IER_RXBUFF | US_IER_OVRE);
  	
  	ser_fifo_init(&ble_usart_rx_fifo, ble_usart_rx_buf, BLE_MAX_RX_PAYLOAD_SIZE);

  	/* Enable UART interrupt */
  	NVIC_EnableIRQ(BLE_UART_IRQn);
	  
	return STATUS_OK;
}

uint8_t configure_serial_drv(void)
{
	configure_primary_uart();
	return STATUS_OK;
}

void configure_usart_after_patch(void)
{
	/* Usart async mode 8 bits transfer test */
	sam_usart_opt_t usart_settings = {
		.baudrate     = CONF_UART_BAUDRATE,
		.char_length  = US_MR_CHRL_8_BIT,
		.parity_type  = US_MR_PAR_NO,
		.stop_bits    = US_MR_NBSTOP_1_BIT,
		.channel_mode = US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		.irda_filter  = 0
	};
	
	/* Configure USART with Flow Control */
	usart_init_hw_handshaking(BLE_UART, &usart_settings,
	sysclk_get_peripheral_hz());
}

static void pdc_update_rx_transfer(void)
{
	if (slave_state == PLATFORM_TRANSPORT_SLAVE_CONNECTED)
	{
		static bool received_header = true; //Set it true for expecting first header
		if (received_header)
		{
			uint16_t len;
			uint8_t *buf_ptr;
			buf_ptr = (uint8_t *)ble_usart_rx_pkt.ul_addr;
			len = (buf_ptr[7] | (buf_ptr[8] << 8));
			if (len)
			{
				ble_usart_rx_pkt.ul_size = len;
				received_header = false;
			}
			else
			{
				/* No Payload. Ready for next header transfer */
				ble_usart_rx_pkt.ul_size = BLE_SERIAL_HEADER_LEN;
			}			 
		}
		else
		{
			/* Received Payload, Subsequent Frame will start with header */
			ble_usart_rx_pkt.ul_size = BLE_SERIAL_HEADER_LEN;
			received_header = true;			
		}
	}
	else
	{
		/* Non-Standard Header transfer */
		ble_usart_rx_pkt.ul_size = 1;
	}
	
	/* Configure the PDC for data receive */
	pdc_rx_init(ble_usart_pdc, &ble_usart_rx_pkt, NULL);
}

static inline void ble_pdc_uart_handler(void)
{
	if((BLE_UART->US_CSR & US_IER_OVRE) == 0)
	{
		if (usart_is_rx_buf_full(BLE_UART))
		{
			/* Disable the Receive Transfer before read */
			pdc_disable_transfer(ble_usart_pdc, PERIPH_PTCR_RXTDIS);
			platform_dma_process_rxdata((uint8_t *)ble_usart_rx_pkt.ul_addr, ble_usart_rx_pkt.ul_size);
			pdc_update_rx_transfer();
			pdc_enable_transfer(ble_usart_pdc, PERIPH_PTCR_RXTEN);
		}
		
		if ((usart_is_tx_buf_empty(BLE_UART))  && (!ble_usart_tx_cmpl))
		{
			usart_disable_interrupt(BLE_UART, US_IER_TXBUFE);
			ble_usart_tx_cmpl  = true;
		}
	}
	else
	{
	 /* Handle Buffer Over-Run Error */
	}	
}

void BLE_UART_Handler(void)
{
	ble_pdc_uart_handler();
}

static inline void ble_pdc_serial_drv_send(uint8_t *data, uint16_t len)
{
	while(ble_usart_tx_cmpl == false);
	
	//Set Tx Data write complete to false
	ble_usart_tx_cmpl = false;
	
	ble_pdc_send_data(data, len);
	
	//Wait for Tx Data write complete
	while(ble_usart_tx_cmpl == false);
	
	if(ble_usart_tx_cmpl)
	{
		#if SERIAL_DRV_TX_CB_ENABLE
		SERIAL_DRV_TX_CB();
		#endif
	}
}

uint16_t serial_drv_send(uint8_t* data, uint16_t len)
{
	ble_pdc_serial_drv_send(data, len);
	return STATUS_OK;
}

uint32_t platform_serial_drv_tx_status(void)
{
	return((usart_is_tx_empty(BLE_UART) == 1) ? 0 : 1);
}

/* EOF */

