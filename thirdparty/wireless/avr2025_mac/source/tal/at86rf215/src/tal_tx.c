/**
 * @file tal_tx.c
 *
 * @brief This file handles the frame transmission within the TAL.
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
#include <stdlib.h>
#include <inttypes.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_pib.h"
#include "tal_config.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_internal.h"
#include "mac_build_config.h"



/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

#define DEFAULT_FRAME_TYPES         ((1 << FCF_FRAMETYPE_BEACON) | \
                                     (1 << FCF_FRAMETYPE_DATA) | (1 << FCF_FRAMETYPE_MAC_CMD))
#define ACK_FRAME_TYPE_ONLY         (1 << FCF_FRAMETYPE_ACK)
#define DEFAULT_FRAME_VERSIONS      ((1 << FCF1_FV_2003) | (1 << FCF1_FV_2006))

/* === GLOBALS ============================================================= */

static uint8_t number_of_tx_retries[2];
static csma_mode_t global_csma_mode[2];
/* Last frame length for IFS handling. */
static uint16_t last_txframe_length[2];
static bool frame_buf_filled[2];

/* === PROTOTYPES ========================================================== */

static void handle_ifs(trx_id_t trx_id);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Requests to TAL to transmit frame
 *
 * This function is called by the MAC to deliver a frame to the TAL
 * to be transmitted by the transceiver.
 *
 * @param trx_id Transceiver identifier
 * @param tx_frame Pointer to the frame_info_t structure updated by the MAC layer
 * @param csma_mode Indicates mode of csma-ca to be performed for this frame
 * @param perform_frame_retry Indicates whether to retries are to be performed for
 *                            this frame
 *
 * @return MAC_SUCCESS  if the TAL has accepted the data from the MAC for frame
 *                 transmission
 *         TAL_BUSY if the TAL is busy servicing the previous MAC request
 */
retval_t tal_tx_frame(trx_id_t trx_id, frame_info_t *tx_frame,
                      csma_mode_t csma_mode, bool perform_frame_retry)
{
	
    //debug_text(PSTR("tal_tx_frame()"));

    if (tal_state[trx_id] == TAL_SLEEP)
    {
        return TAL_TRX_ASLEEP;
    }

    if (tal_state[trx_id] != TAL_IDLE)
    {
        //debug_text_val(PSTR("TAL_BUSY, tal_state[trx_id] = "), tal_state[trx_id]);
        return TAL_BUSY;
    }

    if ((csma_mode == CSMA_SLOTTED) || (tx_frame == NULL))
    {
        return MAC_INVALID_PARAMETER;
    }

    /*
     * In case the frame is too large, return immediately indicating
     * invalid status.
     */
    if ((tx_frame->length + tal_pib[trx_id].FCSLen) > tal_pib[trx_id].MaxPHYPacketSize)
    {
        //debug_text_val(PSTR("Invalid frame length = "), tx_frame->length);
        return MAC_INVALID_PARAMETER;
    }

    /* Store TRX state before entering Tx transaction */
    if ((trx_state[trx_id] == RF_RX) || (trx_state[trx_id] == RF_TXPREP))
    {
        trx_default_state[trx_id] = RF_RX;
    }

    /*
     * Store the pointer to the provided frame structure.
     * This is needed for the callback function.
     */
    mac_frame_ptr[trx_id] = tx_frame;

    /* Set pointer to actual MPDU to be downloaded to the transceiver. */
    tal_frame_to_tx[trx_id] = tx_frame->mpdu;

    if (perform_frame_retry)
    {
        number_of_tx_retries[trx_id] = 0;
    }
    else
    {
        /* No tx retry -> set current retry value to max value .*/
        number_of_tx_retries[trx_id] = tal_pib[trx_id].MaxFrameRetries;
    }

    frame_buf_filled[trx_id] = false;
    global_csma_mode[trx_id] = csma_mode;

    if (csma_mode == CSMA_UNSLOTTED)
    {
#ifdef RX_WHILE_BACKOFF
        tal_pib[trx_id].NumRxFramesDuringBackoff = 0;
#endif
        csma_start(trx_id);
    }
    else // csma_mode == NO_CSMA_NO_IFS or NO_CSMA_WITH_IFS
    {
        if (csma_mode == NO_CSMA_WITH_IFS)
        {
            handle_ifs(trx_id);
        }
        tal_transmit_frame(trx_id);
    }

    return MAC_SUCCESS;
}


/**
 * @brief Transmits frame
 *
 * @param trx_id Transceiver identifier
 */
void tal_transmit_frame(trx_id_t trx_id)
{
    //debug_text(PSTR("transmit_frame()"));

    if (trx_state[trx_id] != RF_TXPREP)
    {
        switch_to_txprep(trx_id);
    }

    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;

#ifdef RX_WHILE_BACKOFF
    if ((frame_buf_filled[trx_id] == false) ||
        (tal_pib[trx_id].NumRxFramesDuringBackoff > 0))
#else
    if (frame_buf_filled[trx_id] == false)
#endif
    {
        /* fill length field */
        uint16_t len = mac_frame_ptr[trx_id]->length + tal_pib[trx_id].FCSLen;
        pal_trx_write(bb_reg_offset + RG_BBC0_TXFLL, (uint8_t *)&len, 2);
    }


    //debug_text(PSTR("switch to Tx"));
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TX);
    trx_state[trx_id] = RF_TX;
    tal_state[trx_id] = TAL_TX;

#if (defined ENABLE_TSTAMP) || (defined MEASURE_ON_AIR_DURATION)
    pal_get_current_time(&fs_tstamp[trx_id]);
#endif

#ifdef RX_WHILE_BACKOFF
    if ((frame_buf_filled[trx_id] == false) ||
        (tal_pib[trx_id].NumRxFramesDuringBackoff > 0))
#else
    if (frame_buf_filled[trx_id] == false)
#endif
    {
        /* fill frame buffer; do not provide FCS values */
        uint16_t tx_frm_buf_offset = BB_TX_FRM_BUF_OFFSET * trx_id;
        pal_trx_write(tx_frm_buf_offset + RG_BBC0_FBTXS,
        (uint8_t *)mac_frame_ptr[trx_id]->mpdu,
        mac_frame_ptr[trx_id]->length);
        /* Check if under-run has occurred */
        bool underrun = pal_trx_bit_read(bb_reg_offset + SR_BBC0_PS_TXUR);
        if (underrun)
        {
            //debug_text(PSTR("Tx underrun occured"));
            /* Abort ongoing transmission */
            switch_to_txprep(trx_id);
            TAL_RF_IRQ_CLR(trx_id, BB_IRQ_TXFE);
            tx_done_handling(trx_id, FAILURE);
            return;
        }
        frame_buf_filled[trx_id] = true;
    }
    else
    {
        //debug_text(PSTR("Re-transmission"));
    }

#if DEBUG > 0
    for (uint16_t i = 0; i < mac_frame_ptr[trx_id]->length; i++)
    {
        //debug_text_val(PSTR("tx val = "), mac_frame_ptr[trx_id]->mpdu[i]);
    }
#endif
    /* Store tx frame length to handle IFS next time */
    last_txframe_length[trx_id] = mac_frame_ptr[trx_id]->length;
}


/**
 * @brief Handles interrupts issued due to end of transmission
 *
 * @param trx_id Transceiver identifier
 */
void handle_tx_end_irq(trx_id_t trx_id)
{
    //debug_text(PSTR("handle_tx_end_irq()"));

    trx_state[trx_id] = RF_TXPREP;

#ifdef MEASURE_ON_AIR_DURATION
    tal_pib[trx_id].OnAirDuration +=
        pal_sub_time_us(rxe_txe_tstamp[trx_id], fs_tstamp[trx_id]);
    /* @ToDo: Minus processing delay */
#endif



    if (tal_state[trx_id] == TAL_ACK_TRANSMITTING)
    {
        ack_transmission_done(trx_id);
        return;
    }


    /* Check if ACK is requested. */
    if (*mac_frame_ptr[trx_id]->mpdu & FCF_ACK_REQUEST)
    {
        /* Wait for ACK reception */
       // debug_text(PSTR("waiting for ACK"));
        tal_state[trx_id] = TAL_WAITING_FOR_ACK_RECEPTION;

        uint8_t timer_id;
        if (trx_id == RF09)
        {
            timer_id = TAL_T_0;
        }
        else
        {
            timer_id = TAL_T_1;
        }
		
        retval_t status =
            pal_timer_start(timer_id, tal_pib[trx_id].ACKWaitDuration + 2000, TIMEOUT_RELATIVE,
                            (FUNC_PTR)ack_timout_cb, (void *)&timer_cb_parameter[trx_id]);
        if (status != MAC_SUCCESS)
        {
            tx_done_handling(trx_id, status);
            return;
        }

        /* Configure frame filter to pass ACK only */
        uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(bb_reg_offset + RG_BBC0_AFFTM, ACK_FRAME_TYPE_ONLY);
        /*
         * Switch to Rx to receive the ACK;
         * switch_to_rx() is not used, since rx buffer is not required
         */
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_RX);
        trx_state[trx_id] = RF_RX;
    }

    else
    {
        //debug_text(PSTR("No ACK requested"));
        tx_done_handling(trx_id, MAC_SUCCESS);
    }
}


/**
 * @brief Implements the handling of the transmission end.
 *
 * This function handles the callback for the transmission end.
 *
 * @param trx_id Transceiver identifier
 * @param status Status of the transmission: MAC_SUCCESS, MAC_NO_ACK,
 *               CHANNEL_ACCESS_FAILURE, FAILURE
 */
void tx_done_handling(trx_id_t trx_id, retval_t status)
{
    //debug_text(PSTR("tx_done_handling()"));

    if (status == MAC_NO_ACK)
    {
        if (number_of_tx_retries[trx_id] < tal_pib[trx_id].MaxFrameRetries)
        {
            number_of_tx_retries[trx_id]++;
            if (global_csma_mode[trx_id] == CSMA_UNSLOTTED)
            {
                csma_start(trx_id);
            }
            else
            {
                tal_transmit_frame(trx_id);
            }
            return; // next tx attempt and no tx done cb
        }
    }
    else    // status == CHANNEL_ACCESS_FAILURE or FAILURE
    {
        /* do nothing */
    }

#ifdef ENABLE_TSTAMP
    mac_frame_ptr[trx_id]->time_stamp = fs_tstamp[trx_id];
#endif

    /* Re-store frame filter to pass "normal" frames */
    if (*mac_frame_ptr[trx_id]->mpdu & FCF_ACK_REQUEST)
    {
        uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(bb_reg_offset + RG_BBC0_AFFTM, DEFAULT_FRAME_TYPES);
    }

    /* Set trx state for leaving TX transaction */
    if (trx_default_state[trx_id] == RF_RX)
    {
        switch_to_rx(trx_id);
    }
    else
    {
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TRXOFF);
        trx_state[trx_id] = RF_TRXOFF;
    }

    tal_state[trx_id] = TAL_IDLE;

    tal_tx_frame_done_cb(trx_id, status, mac_frame_ptr[trx_id]);
} /* tx_done_handling() */


/**
 * @brief Handles interframe spacing (IFS)
 *
 * @param trx_id Transceiver identifier
 */
static void handle_ifs(trx_id_t trx_id)
{
    //debug_text(PSTR("handle_ifs()"));

    if ((global_csma_mode[trx_id] == CSMA_UNSLOTTED) ||
        (global_csma_mode[trx_id] == NO_CSMA_WITH_IFS))
    {
        uint32_t now;
        uint32_t time_diff;

        pal_get_current_time(&now);
        time_diff = now - rxe_txe_tstamp[trx_id];
        if (last_txframe_length[trx_id] > aMaxSIFSFrameSize)
        {
            /* Long IFS */
            uint32_t required_spacing = macMinLIFSPeriod_def * tal_pib[trx_id].SymbolDuration_us;
            if (time_diff < required_spacing)
            {
                uint32_t delay = required_spacing - time_diff;
                //debug_text_val(PSTR("delay LIFS = "), (uint16_t)delay);
                pal_timer_delay(delay);
            }
        }
        else
        {
            /* Short IFS */
            uint32_t required_spacing = macMinSIFSPeriod_def * tal_pib[trx_id].SymbolDuration_us;
            if (time_diff < required_spacing)
            {
                uint32_t delay = required_spacing - time_diff;
                //debug_text_val(PSTR("delay SIFS = "), (uint16_t)delay);
                pal_timer_delay(delay);
            }
        }
    }
}


/* EOF */
