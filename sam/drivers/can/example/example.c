/**
 * \file
 *
 * \brief CAN example for SAM.
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
 *  \mainpage CAN example
 *
 *  \par Purpose
 *
 *  This example demonstrates the basic functions of CAN controller.
 *
 *  \par Requirements
 *
 *  This package can be used with two SAM4E-XPRO boards.
 *  The CAN1 in two board should be connected externally before running
 *  the example.
 *
 *  \par Description
 *
 *  In this example, one board sends messages over CAN bus to another board.
 *  The CAN message will display on the terminal window.
 *
 *  \par Usage
 *
 *  -# Build the program and download it into the evaluation board.
 *  -# On the computer, open and configure a terminal application
 *     (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *    - 115200 bauds
 *    - 8 bits of data
 *    - No parity
 *    - 1 stop bit
 *    - No flow control
 *  -# Connect CAN1 in two boards.
 *  -# Start the application.
 *  -# Upon startup, the application will output the following lines
 *     on the terminal window.
 *      \code
 *      -- CAN Example --
 *      -- SAM4E -XPRO --
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *      \endcode
 *  -# Press 'r' key in the terminal window to configure one board to
 *     receive CAN message.
 *  -# Press 't' key in the terminal window to confgure another board to
 *     send CAN message. The message will be displayed on the terminal window.
 */

#include "asf.h"

#define STRING_EOL    "\r"

#define STRING_HEADER "-- CAN Example --\r\n" \
                      "-- "BOARD_NAME" --\r\n" \
                      "-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

#define CAN_MSG_TOGGLE_LED_0        0x11223344
#define CAN_MSG_DUMMY_DATA          0x55AAAA55

/** CAN frame max data length */
#define MAX_CAN_FRAME_DATA_LEN      8

/** CAN1 Transfer mailbox structure */
can_mb_conf_t can1_mailbox;

/** Receive status */
volatile uint32_t g_ul_recv_status = 0;

/**
 * \brief Default interrupt handler for CAN 1.
 */
void CAN1_Handler(void)
{
	uint32_t ul_status;

	ul_status = can_get_status(CAN1);
	if (ul_status & GLOBAL_MAILBOX_MASK) {
		for (uint8_t i = 0; i < CANMB_NUMBER; i++) {
			ul_status = can_mailbox_get_status(CAN1, i);
			if ((ul_status & CAN_MSR_MRDY) == CAN_MSR_MRDY) {
				can1_mailbox.ul_mb_idx = i;
				can1_mailbox.ul_status = ul_status;
				can_mailbox_read(CAN1, &can1_mailbox);
				g_ul_recv_status = 1;
				break;
			}
		}
	}
}

/**
 * \brief Decode CAN messages.
 *
 *  \param p_mailbox Pointer to CAN Mailbox structure.
 */
static void decode_can_msg(can_mb_conf_t *p_mailbox)
{
	uint32_t ul_led_Ctrl = p_mailbox->ul_datal;

	puts("CAN message:" STRING_EOL);
	if (ul_led_Ctrl == CAN_MSG_TOGGLE_LED_0) {
		puts("  Toggle LED 0" STRING_EOL);
	}
}

/**
 * \brief Reset mailbox configure structure.
 *
 *  \param p_mailbox Pointer to mailbox configure structure.
 */
static void reset_mailbox_conf(can_mb_conf_t *p_mailbox)
{
	p_mailbox->ul_mb_idx = 0;
	p_mailbox->uc_obj_type = 0;
	p_mailbox->uc_id_ver = 0;
	p_mailbox->uc_length = 0;
	p_mailbox->uc_tx_prio = 0;
	p_mailbox->ul_status = 0;
	p_mailbox->ul_id_msk = 0;
	p_mailbox->ul_id = 0;
	p_mailbox->ul_fid = 0;
	p_mailbox->ul_datal = 0;
	p_mailbox->ul_datah = 0;
}

#define CAN_COMM_MB_IDX    0
#define CAN_TRANSFER_ID    0x07
#define CAN1_TX_PRIO       15

/**
 *  Configure UART for debug message output.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 *  \brief can_example application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint32_t ul_sysclk;
	uint8_t uc_key;

	/* Initialize the SAM system. */
	sysclk_init();
	board_init();

	/* Configure UART for debug message output. */
	configure_console();

	/* Output example information. */
	puts(STRING_HEADER);

	/* Enable CAN1 clock. */
	pmc_enable_periph_clk(ID_CAN1);

	ul_sysclk = sysclk_get_cpu_hz();
	if (can_init(CAN1, ul_sysclk, CAN_BPS_1000K)) {
		puts("CAN initialization is completed." STRING_EOL);

		/* Disable all CAN1 interrupts. */
		can_disable_interrupt(CAN1, CAN_DISABLE_ALL_INTERRUPT_MASK);

		/* Configure and enable interrupt of CAN1, as the tests will use receiver interrupt. */
		NVIC_EnableIRQ(CAN1_IRQn);

		can_reset_all_mailbox(CAN1);

		puts("  t: Perform CAN Transmisson \r"
				"  r: Perform CAN Reception \r");

		while (1) {
			scanf("%c", (char *)&uc_key);

			switch (uc_key) {
			case 'r':
				/* Init CAN1 Mailbox 0 to Reception Mailbox. */
				puts("Init CAN1 Mailbox 0 to Reception Mailbox!");
				reset_mailbox_conf(&can1_mailbox);
				can1_mailbox.ul_mb_idx = CAN_COMM_MB_IDX;
				can1_mailbox.uc_obj_type = CAN_MB_RX_MODE;
				can1_mailbox.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;
				can1_mailbox.ul_id = CAN_MID_MIDvA(CAN_TRANSFER_ID);
				can_mailbox_init(CAN1, &can1_mailbox);

				/* Enable CAN1 mailbox 0 interrupt. */
				can_enable_interrupt(CAN1, CAN_IER_MB0);

				while (!g_ul_recv_status) {
				}
				g_ul_recv_status = 0;
				if ((can1_mailbox.ul_datal == CAN_MSG_TOGGLE_LED_0) &&
					(can1_mailbox.uc_length == MAX_CAN_FRAME_DATA_LEN)) {
					puts("Test passed" STRING_EOL);
					decode_can_msg(&can1_mailbox);
				} else {
					puts("Test ERROR" STRING_EOL);
				}
				break;

			case 't':
				/* Init CAN1 Mailbox 0 to Transmit Mailbox. */
				puts("Init CAN1 Mailbox 0 to Transmission Mailbox!");
				reset_mailbox_conf(&can1_mailbox);
				can1_mailbox.ul_mb_idx = CAN_COMM_MB_IDX;
				can1_mailbox.uc_obj_type = CAN_MB_TX_MODE;
				can1_mailbox.uc_tx_prio = CAN1_TX_PRIO;
				can1_mailbox.uc_id_ver = 0;
				can1_mailbox.ul_id_msk = 0;
				can_mailbox_init(CAN1, &can1_mailbox);

				/* Write transmit information into mailbox. */
				can1_mailbox.ul_id = CAN_MID_MIDvA(CAN_TRANSFER_ID);
				can1_mailbox.ul_datal = CAN_MSG_TOGGLE_LED_0;
				can1_mailbox.ul_datah = CAN_MSG_DUMMY_DATA;
				can1_mailbox.uc_length = MAX_CAN_FRAME_DATA_LEN;
				can_mailbox_write(CAN1, &can1_mailbox);

				/* Send out the information in the mailbox. */
				can_global_send_transfer_cmd(CAN1, CAN_TCR_MB0);
				break;

			default:
				break;
			}
		}
	} else {
		puts("CAN initialization (sync) ERROR" STRING_EOL);
	}

	while (1) {
	}
}
