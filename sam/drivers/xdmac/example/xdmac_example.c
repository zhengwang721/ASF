/**
 * \file
 *
 * \brief SAM Direct Memory Access Controller driver example.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <conf_board.h>
#include <conf_clock.h>

/** Micro-block length for single transfer  */
#define MICROBLOCK_LEN       16

//! [xdmac_define_channel]
/** XDMA channel used in this example. */
#define XDMA_CH 0
//! [xdmac_define_channel]

/** XDMA channel configuration. */
static xdmac_channel_config_t xdmac_channel_cfg;

/* DMA transfer done flag. */
volatile uint32_t g_xfer_done = 0;

//! [xdmac_define_buffer]
COMPILER_ALIGNED(8) static uint8_t src_buf[512];
COMPILER_ALIGNED(8) static uint8_t dst_buf[512];
//! [xdmac_define_buffer]

/** Linked list descriptor */
COMPILER_WORD_ALIGNED static lld_view0 lld[2];

/**
 * \brief Configure the console UART.
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
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief XDMAC interrupt handler.
 */
void XDMAC_Handler(void)
{
	uint32_t dma_status;

	dma_status = xdmac_channel_get_interrupt_status(XDMAC, XDMA_CH);

	if (dma_status & XDMAC_CIS_BIS) {
		g_xfer_done = 1;
	}
}

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint32_t i;

	/* Initialize the system */
	sysclk_init();
	board_init();

	/* Configure console UART */
	configure_console();

	/* Output example information */
	printf("-- XDMAC Example --\n\r");
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	/* Initialize and enable DMA controller */
	pmc_enable_periph_clk(ID_XDMAC);

	   /*Enable XDMA interrupt */
	NVIC_ClearPendingIRQ(XDMAC_IRQn);
	NVIC_SetPriority( XDMAC_IRQn ,1);
	NVIC_EnableIRQ(XDMAC_IRQn);

	/* Initialize source and destination buffer */
	for (i = 0; i < 512; i++) {
		src_buf[i] = 0x55;
		dst_buf[i] = 0;
	}

	/* Initialize channel config */
	xdmac_channel_cfg.mbr_ubc = MICROBLOCK_LEN;
	xdmac_channel_cfg.mbr_sa = (uint32_t)src_buf;
	xdmac_channel_cfg.mbr_da = (uint32_t)dst_buf;
	xdmac_channel_cfg.mbr_cfg = XDMAC_CC_TYPE_MEM_TRAN |
		XDMAC_CC_MEMSET_NORMAL_MODE |
		XDMAC_CC_MBSIZE_SINGLE |
		XDMAC_CC_DWIDTH_BYTE |
		XDMAC_CC_SIF_AHB_IF0 |
		XDMAC_CC_DIF_AHB_IF0 |
		XDMAC_CC_SAM_INCREMENTED_AM |
		XDMAC_CC_DAM_INCREMENTED_AM;
	xdmac_channel_cfg.mbr_bc = 15;
	xdmac_channel_cfg.mbr_ds =  0;
	xdmac_channel_cfg.mbr_sus = 0;
	xdmac_channel_cfg.mbr_dus = 0;

	xdmac_configure_transfer(XDMAC, XDMA_CH, &xdmac_channel_cfg);

	/* Initialize linked list descriptor */
	lld[0].mbr_nda = (uint32_t)(&lld[1]);
	lld[0].mbr_ubc = XDMA_UBC_NVIEW_NDV0 |
			XDMA_UBC_NDE_FETCH_EN |
			XDMA_UBC_NSEN_UPDATED |
			XDMA_UBC_NDEN_UPDATED |
			MICROBLOCK_LEN;
	lld[0].mbr_da = (uint32_t)dst_buf;

	lld[1].mbr_nda = 0;
	lld[1].mbr_ubc = 256;
	lld[1].mbr_da = (uint32_t)dst_buf + 256;

	xdmac_channel_set_descriptor_control(XDMAC, XDMA_CH, XDMAC_CNDC_NDVIEW_NDV0 |
			XDMAC_CNDC_NDE_DSCR_FETCH_EN |
			XDMAC_CNDC_NDSUP_SRC_PARAMS_UPDATED |
			XDMAC_CNDC_NDDUP_DST_PARAMS_UPDATED);
	xdmac_channel_set_descriptor_addr(XDMAC, XDMA_CH, (uint32_t)(&lld[0]), 0);

	xdmac_enable_interrupt(XDMAC, XDMA_CH);
	xdmac_channel_enable_interrupt(XDMAC, XDMA_CH, XDMAC_CIE_BIE);
	xdmac_channel_enable(XDMAC, XDMA_CH);

	/* Wait transfer finish */
	while (!g_xfer_done) {
	}

	/* Verify the transferred data */
	for (i = 0; i < 512; i++) {
		if (src_buf[i] != dst_buf[i]) {
			printf("> Test Fail.\n\r");
			while (1) {
			}
		}
	}
	printf("> Test OK.\n\r");

	while (1) {
	}
}
