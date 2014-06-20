/**
 * @file tal_ack.c
 *
 * @brief This file implements acknowledgment handling.
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 *
 */

/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "stack_config.h"
#include "tal_config.h"
#include "tal_pib.h"
#include "tal_internal.h"



/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
static bool data_rate_switched_for_ack[2];
#endif

/* === PROTOTYPES ========================================================== */

static void transmit_ack(void *parameter);
static bool get_pending_bit(trx_id_t trx_id);
#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
static void adapt_data_rate_for_ack(trx_id_t trx_id);
static void restore_data_rate_after_ack(trx_id_t trx_id);
#endif

/* === IMPLEMENTATION ====================================================== */



/* --- ACK transmission ---------------------------------------------------- */



/**
 * @brief Checks if ACK transmission is required
 *
 * @param trx_id Id of the corresponding trx
 *
 * @return true If ACK transmission is required; else "false"
 */
bool is_ack_required(trx_id_t trx_id)
{
    //debug_text_val(PSTR("is_ack_required(), trx_id ="), trx_id);
    
	bool ack_required;
	uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
	ack_required = pal_trx_bit_read(rf_reg_offset+SR_BBC0_PC_FCSOK);	//for prom mode->check
	if(!ack_required)
	{
	return false;	
	}
	bool ret = true;
    uint8_t fcf0 = rx_frm_info[trx_id]->mpdu[0];
    uint8_t fcf1 = rx_frm_info[trx_id]->mpdu[1];

    if ((fcf0 & FCF_ACK_REQUEST) &&
        /* frame type */
        (((fcf0 & FCF_FRAMETYPE_MASK) == FCF_FRAMETYPE_DATA) ||
         ((fcf0 & FCF_FRAMETYPE_MASK) == FCF_FRAMETYPE_MAC_CMD)) &&
        /* frame version */
        (((fcf1 & FCF1_FV_MASK) == FCF1_FV_2003) ||
         ((fcf1 & FCF1_FV_MASK) == FCF1_FV_2006)))
    {
        if (rx_frm_info[trx_id]->length > 3)
        {
            /* Check if broadcast is used */
            uint8_t dest_addr_mode = (fcf1 >> 2) & 0x03;
            if (dest_addr_mode != 0)
            {
                uint16_t dest_pan_id = rx_frm_info[trx_id]->mpdu[3] | (rx_frm_info[trx_id]->mpdu[4] << 8);
                if (dest_pan_id == BROADCAST)
                {
                    ret = false;
                }
                else
                {
                    if (dest_addr_mode == FCF_SHORT_ADDR)
                    {
                        uint16_t dest_short_addr = rx_frm_info[trx_id]->mpdu[5] | (rx_frm_info[trx_id]->mpdu[6] << 8);
                        if ((dest_short_addr == BROADCAST) ||
                            (dest_short_addr == MAC_NO_SHORT_ADDR_VALUE))
                        {
                            ret = false;
                        }
                    }
                }
            }
        }
    }
    else
    {
        ret = false;
    }

    if (ret)
    {
        //debug_text(PSTR("ACK transmission requested"));
    }
    else
    {
        //debug_text(PSTR("No ACK transmission requested"));
    }

    return ret;
}


/**
 * @brief Schedules ACK transmission
 *
 * This function determines the point in time to transmit the ACK
 * and starts the timer accordingly.
 *
 * @param trx_id Id of the corresponding trx
 */
void schedule_ack_transmission(trx_id_t trx_id)
{
    //debug_text_val(PSTR("schedule_ack_transmission(), trx_id ="), trx_id);

#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
    if (tal_pib[trx_id].AdaptDataRateForACK)
    {
        adapt_data_rate_for_ack(trx_id);
    }
#endif

    /* Start timer for ACK transmission */
    //debug_text(PSTR("Start timer for ACK transmission"));
    uint8_t timer_id;
    if (trx_id == RF09)
    {
        timer_id = TAL_T_0;
    }
    else
    {
        timer_id = TAL_T_1;
    }

    uint32_t acktime;
    pal_get_current_time(&acktime);
    acktime = pal_sub_time_us(acktime, rxe_txe_tstamp[trx_id]);
    acktime = pal_sub_time_us(tal_pib[trx_id].ACKTiming, acktime);

    retval_t timer =
        pal_timer_start(timer_id, acktime, TIMEOUT_RELATIVE,
                        (FUNC_PTR)transmit_ack, (void *)&timer_cb_parameter[trx_id]);
    if (timer == MAC_SUCCESS)
    {
        tal_state[trx_id] = TAL_WAITING_FOR_ACK_TRANSMISION;
    }
    else
    {
        //debug_text(PSTR("ACK timer could not be started"));
        /* In case the ack timer could not be started, send ack immediately. */
        transmit_ack((void *)&timer_cb_parameter[trx_id]);
    }
}


/**
 * @brief Transmits the ACK
 *
 * This function creates the ACK frame and transmits it.
 *
 * @param parameter Pointer to trx_id
 */
static void transmit_ack(void *parameter)
{
    trx_id_t trx_id = *(trx_id_t *)parameter;

    //debug_text_val(PSTR("transmit_ack() for trx_id  = "), trx_id);

    uint8_t ack[3]; // plus 2 or 4 bytes CRC
    ack[0] = FCF_FRAMETYPE_ACK;
    /* Check if frame pending bit needs to be set */
    //if (rx_frm_info[trx_id]->mpdu[0] & FCF_FRAMETYPE_MAC_CMD)
    {
        if (get_pending_bit(trx_id))
        {
            ack[0] |= FCF_FRAME_PENDING;
        }
    }
    ack[1] = 0;
    ack[2] = rx_frm_info[trx_id]->mpdu[2]; // sequence number

    uint16_t ack_len;
    if (tal_pib[trx_id].FCSType == FCS_TYPE_4_OCTETS)
    {
        ack_len = 7;
    }
    else
    {
        ack_len = 5;
    }

    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    pal_trx_write(bb_reg_offset + RG_BBC0_TXFLL, (uint8_t *)&ack_len, 2);

    // fill Frame Buffer; do not provide FCS values
    uint16_t tx_frm_buf_offset = BB_TX_FRM_BUF_OFFSET * trx_id;
    pal_trx_write(tx_frm_buf_offset + RG_BBC0_FBTXS, ack, 3);



    /* Trigger transmission */
    tal_state[trx_id] = TAL_ACK_TRANSMITTING;
    //debug_text(PSTR("switch to Tx"));
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TX);
    trx_state[trx_id] = RF_TX;

#if (defined ENABLE_TSTAMP) || (defined MEASURE_ON_AIR_DURATION)
    pal_get_current_time(&fs_tstamp[trx_id]);
#endif

    /* Upload frame during ACK transmission */
    upload_frame(trx_id);
}


/**
 * @brief Get pending bit value
 *
 * This function checks if the pending bit needs to be set.
 *
 * @param trx_id Transceiver identifier
 */
static bool get_pending_bit(trx_id_t trx_id)
{
    /* @ToDo */

    bool ret = false;

    /* Check if received frame is a data request command */
    if (rx_frm_info[trx_id]->mpdu[0] & FCF_FRAMETYPE_MAC_CMD)
    {
        /* @ToDo: Check if command id is equal to data request */

        //ret = true;
    }

    return ret;
}


/**
 * @brief Handles end of ACK transmission
 *
 * This function is called with the TXFE IRQ.
 * It handles further processing after an ACK has been transmitted.
 *
 * @param trx_id Id of the corresponding trx
 */
void ack_transmission_done(trx_id_t trx_id)
{
    //debug_text(PSTR("ack_transmission_done()"));

#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
    if (data_rate_switched_for_ack[trx_id] == true)
    {
        restore_data_rate_after_ack(trx_id);
    }
#endif

#ifdef RX_WHILE_BACKOFF
    if (csma_backoff_onhold[trx_id])
    {
        csma_backoff_onhold[trx_id] = false;
        csma_start(trx_id);
    }
    else
#endif
    {
        complete_rx_transaction(trx_id);
        tal_state[trx_id] = TAL_IDLE;
    }
}


/**
 * @brief Adapts the data rate for ACK transmission
 *
 * @param trx_id Id of the corresponding trx
 */
#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
static void adapt_data_rate_for_ack(trx_id_t trx_id)
{
    //debug_text(PSTR("adapt_data_rate_for_ack()"));

    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    if (tal_pib[trx_id].phy.modulation == OFDM)
    {
        /* Check if MCS differs from local setting */
        uint8_t rx_mcs = pal_trx_bit_read(bb_reg_offset + SR_BBC0_OFDMPHRRX_MCS);
        //debug_text_val(PSTR("OFDMPHRRX_MCS = "), rx_mcs);
        if (rx_mcs != tal_pib[trx_id].phy.phy_mode.ofdm.mcs_val)
        {
            //debug_text(PSTR("Update MCS for ACK"));
            pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMPHRTX_MCS, rx_mcs);
            data_rate_switched_for_ack[trx_id] = true;
        }
        else
        {
            //debug_text(PSTR("Same MCS is used for ACK"));
        }
    }
    else if (tal_pib[trx_id].phy.modulation == OQPSK)
    {
        /* Check if data rate differs from local setting */
        uint8_t mod = pal_trx_bit_read(bb_reg_offset + SR_BBC0_OQPSKPHRRX_MOD);
        //debug_text_val(PSTR("OQPSKPHRRX_MOD = "), mod);
        if (mod != tal_pib[trx_id].phy.phy_mode.oqpsk.rate_mode)
        {
            //debug_text(PSTR("Update data rate for ACK"));
            pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKPHRTX_MOD, mod);
            data_rate_switched_for_ack[trx_id] = true;
        }
        else
        {
            //debug_text(PSTR("Same data rate is used for ACK"));
        }
    }
    else
    {
        /* Do nothing */
        /* @ToDo: Disable AdaptDataRateForACK for FSK */
    }
}
#endif


/**
 * @brief Restores the original data rate after ACK transmission
 *
 * @param trx_id Id of the corresponding trx
 */
#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
static void restore_data_rate_after_ack(trx_id_t trx_id)
{
    //debug_text(PSTR("restore_data_rate_after_ack()"));

    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    if (tal_pib[trx_id].phy.modulation == OFDM)
    {
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMPHRTX_MCS,
                          tal_pib[trx_id].phy.phy_mode.ofdm.mcs_val);
    }
    else if (tal_pib[trx_id].phy.modulation == OQPSK)
    {
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKPHRTX_MOD,
                          tal_pib[trx_id].phy.phy_mode.oqpsk.rate_mode);
    }
    else
    {
        /* Do nothing */
    }
    data_rate_switched_for_ack[trx_id] = false;
}
#endif



/* --- ACK reception ------------------------------------------------------- */



/**
 * @brief Callback function for ACK timeout
 *
 * This function is called if the ACK timeout timer fires.
 *
 * @param parameter Pointer to trx_id
 */
void ack_timout_cb(void *parameter)
{
    trx_id_t trx_id = *(trx_id_t *)parameter;

    stop_tal_timer(trx_id);

    //debug_text(PSTR("ack_timout_cb()"));

    tx_done_handling(trx_id, MAC_NO_ACK);
}


/**
 * @brief Handles ACK reception
 *
 * This function is called with the RXFE IRQ,
 * if the device is waiting for an incoming ACK.
 * It checks if the ACK is valid.
 *
 * @param trx_id Id of the corresponding trx
 *
 * @return true if ACK is valid, otherwise false
 */
bool handle_ack_reception(trx_id_t trx_id)
{
    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    bool ack_valid = false;

    //debug_text(PSTR("handle_ack_reception()"));

    /* Check if incoming frame is the valid ACK */

    /* Check frame length */
    uint16_t frame_len;
    pal_trx_read(bb_reg_offset + RG_BBC0_RXFLL, (uint8_t *)&frame_len, 2);
    //debug_text_val(PSTR("ACK len = "), frame_len);

    uint8_t expected_ack_len = ACK_PAYLOAD_LEN + FCS_LEN;
    if (tal_pib[trx_id]. FCSType == FCS_TYPE_4_OCTETS)
    {
        expected_ack_len += 2;
    }

    /* Validate ACK */
    if (expected_ack_len == frame_len)
    {
        /* Upload ACK frame payload */
        uint8_t ack[ACK_PAYLOAD_LEN]; // No interest in FCS
        uint16_t rx_frm_buf_offset = BB_RX_FRM_BUF_OFFSET * trx_id;
        pal_trx_read(rx_frm_buf_offset + RG_BBC0_FBRXS, ack, ACK_PAYLOAD_LEN);

        /* Check frame type, frequency number and frame version */
        if ((ack[0] & FCF_FRAMETYPE_ACK) &&
            (((ack[1] >> FCF1_FV_SHIFT) & 0x03) <= FCF_FRAME_VERSION_2006) &&
            (ack[2] == mac_frame_ptr[trx_id]->mpdu[2]))
        {
            //debug_text(PSTR("ACK OK"));

            /* Stop ACK timeout timer */
            //debug_text(PSTR("Stop ACKWaitDuration timer"));
            stop_tal_timer(trx_id);
            tx_done_handling(trx_id, MAC_SUCCESS);
            ack_valid = true;
        }
        else
        {
            //debug_text(PSTR("Invalid ACK frame"));
        }
    }
    else
    {
        //debug_text(PSTR("Invalid ACK frame, wrong length"));
    }

    if (!ack_valid)
    {
        /*
         * Switch to RX again and
         * wait for the next frame that might be the valid ACK.
         * Use of switch_to_rx() not necessary here,
         * since buffer is for ACK not required.
         */
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_RX);
        trx_state[trx_id] = RF_RX;
    }

    return ack_valid;
}


/*  EOF */
