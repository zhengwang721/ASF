/**
 * \file
 *
 * \brief SAM Control Area Network (CAN) Driver
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "can.h"

/* Message ram definition. */
COMPILER_ALIGNED(4)
static struct can_rx_element can0_rx_buffer[CONF_CAN0_RX_BUFFER_NUM];
COMPILER_ALIGNED(4)
static struct can_rx_element can0_rx_fifo_0[CONF_CAN0_RX_FIFO_0_NUM];
COMPILER_ALIGNED(4)
static struct can_rx_element can0_rx_fifo_1[CONF_CAN0_RX_FIFO_1_NUM];
COMPILER_ALIGNED(4)
static struct can_tx_element can0_tx_buffer[CONF_CAN0_TX_BUFFER_NUM];
COMPILER_ALIGNED(4)
static struct can_tx_element can0_tx_fifo_queue[CONF_CAN0_TX_FIFO_QUEUE_NUM];
COMPILER_ALIGNED(4)
static struct can_tx_event_element can0_tx_event_fifo[CONF_CAN0_TX_EVENT_FIFO];
COMPILER_ALIGNED(4)
static struct can_sd_message_filter_element can0_rx_standard_filter[CONF_CAN0_RX_STANDARD_ID_FILTER_NUM];
COMPILER_ALIGNED(4)
static struct can_et_message_filter_element can0_rx_extended_filter[CONF_CAN0_RX_EXTENDED_ID_FILTER_NUM];

COMPILER_ALIGNED(4)
static struct can_rx_element can1_rx_buffer[CONF_CAN1_RX_BUFFER_NUM];
COMPILER_ALIGNED(4)
static struct can_rx_element can1_rx_fifo_0[CONF_CAN1_RX_FIFO_0_NUM];
COMPILER_ALIGNED(4)
static struct can_rx_element can1_rx_fifo_1[CONF_CAN1_RX_FIFO_1_NUM];
COMPILER_ALIGNED(4)
static struct can_tx_element can1_tx_buffer[CONF_CAN1_TX_BUFFER_NUM];
COMPILER_ALIGNED(4)
static struct can_tx_element can1_tx_fifo_queue[CONF_CAN1_TX_FIFO_QUEUE_NUM];
COMPILER_ALIGNED(4)
static struct can_tx_event_element can1_tx_event_fifo[CONF_CAN1_TX_EVENT_FIFO];
COMPILER_ALIGNED(4)
static struct can_sd_message_filter_element can1_rx_standard_filter[CONF_CAN1_RX_STANDARD_ID_FILTER_NUM];
COMPILER_ALIGNED(4)
static struct can_et_message_filter_element can1_rx_extended_filter[CONF_CAN1_RX_EXTENDED_ID_FILTER_NUM];

static void _can_message_memory_init(Can *hw)
{
	if (hw == CAN0) {
		hw->SIDFC.reg = CAN_SIDFC_FLSSA((uint32_t)can0_rx_standard_filter) |
				CAN_SIDFC_LSS(CONF_CAN0_RX_STANDARD_ID_FILTER_NUM);
		hw->XIDFC.reg = CAN_XIDFC_FLESA((uint32_t)can0_rx_extended_filter) |
				CAN_XIDFC_LSE(CONF_CAN0_RX_EXTENDED_ID_FILTER_NUM);
		hw->RXF0C.reg = CAN_RXF0C_F0SA((uint32_t)can0_rx_fifo_0) |
				CAN_RXF0C_F0S(CONF_CAN0_RX_FIFO_0_NUM);
		hw->RXF1C.reg = CAN_RXF1C_F1SA((uint32_t)can0_rx_fifo_1) |
				CAN_RXF1C_F1S(CONF_CAN0_RX_FIFO_1_NUM);
		hw->RXBC.reg = CAN_RXBC_RBSA((uint32_t)can0_rx_buffer) |
				CAN_RXF0C_F0S(CONF_CAN0_RX_BUFFER_NUM);
		hw->TXBC.reg = CAN_TXBC_TBSA((uint32_t)can0_tx_buffer) |
				CAN_TXBC_NDTB(CONF_CAN0_TX_BUFFER_NUM) |
				CAN_TXBC_TFQS(CONF_CAN0_TX_FIFO_QUEUE_NUM);
		hw->TXEFC.reg = CAN_TXEFC_EFSA((uint32_t)can0_tx_event_fifo) |
				CAN_TXEFC_EFS(CONF_CAN0_TX_EVENT_FIFO);
	}

	if (hw == CAN1) {
		hw->SIDFC.reg = CAN_SIDFC_FLSSA((uint32_t)can1_rx_standard_filter) |
				CAN_SIDFC_LSS(CONF_CAN1_RX_STANDARD_ID_FILTER_NUM);
		hw->XIDFC.reg = CAN_XIDFC_FLESA((uint32_t)can1_rx_extended_filter) |
				CAN_XIDFC_LSE(CONF_CAN1_RX_EXTENDED_ID_FILTER_NUM);
		hw->RXF0C.reg = CAN_RXF0C_F0SA((uint32_t)can1_rx_fifo_0) |
				CAN_RXF0C_F0S(CONF_CAN1_RX_FIFO_0_NUM);
		hw->RXF1C.reg = CAN_RXF1C_F1SA((uint32_t)can1_rx_fifo_1) |
				CAN_RXF1C_F1S(CONF_CAN1_RX_FIFO_1_NUM);
		hw->RXBC.reg = CAN_RXBC_RBSA((uint32_t)can1_rx_buffer) |
				CAN_RXF0C_F0S(CONF_CAN1_RX_BUFFER_NUM);
		hw->TXBC.reg = CAN_TXBC_TBSA((uint32_t)can1_tx_buffer) |
				CAN_TXBC_NDTB(CONF_CAN1_TX_BUFFER_NUM) |
				CAN_TXBC_TFQS(CONF_CAN1_TX_FIFO_QUEUE_NUM);
		hw->TXEFC.reg = CAN_TXEFC_EFSA((uint32_t)can1_tx_event_fifo) |
				CAN_TXEFC_EFS(CONF_CAN1_TX_EVENT_FIFO);
	}
}

static void _can_set_configuration(Can *hw, struct can_config *config)
{
	if (config->run_in_standby) {
		hw->MRCFG.reg |= 0x01<<6;
	}

	hw->RWD.reg |= CAN_RWD_WDC(config->watchdog_configuration);

	if (config->transmit_pause) {
		hw->CCCR.reg |= CAN_CCCR_TXP;
	}

	if (config->edge_filtering) {
		hw->CCCR.reg |= CAN_CCCR_EFBI;
	}

	if (config->protocol_exception_handling) {
		hw->CCCR.reg |= CAN_CCCR_PXHD;
	}

	if (!config->automatic_retransmission) {
		hw->CCCR.reg |= CAN_CCCR_DAR;
	}

	if (config->clock_stop_request) {
		hw->CCCR.reg |= CAN_CCCR_CSR;
	}

	if (config->clock_stop_acknowledge) {
		hw->CCCR.reg |= CAN_CCCR_CSA;
	}

}
void can_module_init(struct can_module *const module_inst, Can *hw,
		struct can_config *config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(hw);
	Assert(config);

	/* Associate the software module instance with the hardware module */
	module_inst->hw = hw;

	/* Configure GCLK channel */
	struct system_gclk_chan_config gclk_chan_conf;
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	gclk_chan_conf.source_generator = config->clock_source;

	if (hw == CAN0) {
		/* Turn on the digital interface clock. */
		system_ahb_clock_set_mask(MCLK_AHBMASK_CAN0);

		system_gclk_chan_set_config(CAN0_GCLK_ID, &gclk_chan_conf);
		system_gclk_chan_enable(CAN0_GCLK_ID);
	}

	if (hw == CAN1) {
		/* Turn on the digital interface clock. */
		system_ahb_clock_set_mask(MCLK_AHBMASK_CAN1);

		system_gclk_chan_set_config(CAN1_GCLK_ID, &gclk_chan_conf);
		system_gclk_chan_enable(CAN1_GCLK_ID);
	}


	/* Configuration Change Enable. */
	hw->CCCR.reg |= CAN_CCCR_CCE;

	/* Initialize the message memory address. */
	_can_message_memory_init(hw);

	/* Set the configuration. */
	_can_set_configuration(hw, config);

	/* Enable the interrupt setting which no need change. */
	hw->ILE.reg = CAN_ILE_EINT0 | CAN_ILE_EINT1;
	hw->TXBTIE.reg = CAN_TXBTIE_MASK;
	hw->TXBCIE.reg = CAN_TXBCIE_MASK;
}

void can_switch_mode(struct can_module *const module_inst, const enum can_mode mode)
{
	if (mode == CAN_MODE_NORMAL_OPERATION) {
		module_inst->hw->CCCR.reg &= ~CAN_CCCR_INIT;
		/* Wait for the sync. */
		while (module_inst->hw->CCCR.reg & CAN_CCCR_INIT);
	}
	if (mode == CAN_MODE_TEST) {
		module_inst->hw->CCCR.reg |= CAN_CCCR_TEST;
		module_inst->hw->TEST.reg |= CAN_TEST_LBCK;;
	}
}

void can_set_rx_standand_filter(struct can_module *const module_inst,
		struct can_sd_message_filter_element *sd_filter, uint32_t index)
{
	if (module_inst->hw == CAN0) {
		can0_rx_standard_filter[index].S0.reg = sd_filter->S0.reg;
	}
	if (module_inst->hw == CAN1) {
		can1_rx_standard_filter[index].S0.reg = sd_filter->S0.reg;
	}
}

void can_set_rx_extended_filter(struct can_module *const module_inst,
		struct can_et_message_filter_element *et_filter, uint32_t index)
{
	if (module_inst->hw == CAN0) {
		can0_rx_extended_filter[index].F0.reg = et_filter->F0.reg;
		can0_rx_extended_filter[index].F1.reg = et_filter->F1.reg;
	}
	if (module_inst->hw == CAN1) {
		can1_rx_extended_filter[index].F0.reg = et_filter->F0.reg;
		can1_rx_extended_filter[index].F1.reg = et_filter->F1.reg;
	}
}

void can_get_rx_buffer_element(struct can_module *const module_inst,
		struct can_rx_element *rx_element, uint32_t index)
{
	if (module_inst->hw == CAN0) {
		rx_element = &can0_rx_buffer[index];
	}
	if (module_inst->hw == CAN1) {
		rx_element = &can1_rx_buffer[index];
	}
}

void can_get_rx_fifo_0_element(struct can_module *const module_inst,
		struct can_rx_element *rx_element, uint32_t index)
{
	if (module_inst->hw == CAN0) {
		rx_element = &can0_rx_fifo_0[index];
	}
	if (module_inst->hw == CAN1) {
		rx_element = &can1_rx_fifo_0[index];
	}
}

void can_get_rx_fifo_1_element(struct can_module *const module_inst,
		struct can_rx_element *rx_element, uint32_t index)
{
	if (module_inst->hw == CAN0) {
		rx_element = &can0_rx_fifo_1[index];
	}
	if (module_inst->hw == CAN1) {
		rx_element = &can1_rx_fifo_1[index];
	}
}

void can_set_tx_buffer_element(struct can_module *const module_inst,
		struct can_tx_element *tx_element, uint32_t index)
{
	uint32_t i;
	if (module_inst->hw == CAN0) {
		can0_tx_buffer[index].T0.reg = tx_element->T0.reg;
		can0_tx_buffer[index].T1.reg = tx_element->T1.reg;
		for (i = 0; i < 8; i++) {
			can0_tx_buffer[index].data[i] = tx_element->data[i];
		}
	}
	if (module_inst->hw == CAN1) {
		can1_tx_buffer[index].T0.reg = tx_element->T0.reg;
		can1_tx_buffer[index].T1.reg = tx_element->T1.reg;
		for (i = 0; i < 8; i++) {
			can1_tx_buffer[index].data[i] = tx_element->data[i];
		}
	}

}

void can_get_tx_event_fifo_element(struct can_module *const module_inst,
		 struct can_tx_event_element *tx_event_element, uint32_t index)
{
	if (module_inst->hw == CAN0) {
		tx_event_element = &can0_tx_event_fifo[index];
	}
	if (module_inst->hw == CAN1) {
		tx_event_element = &can1_tx_event_fifo[index];
	}
}

