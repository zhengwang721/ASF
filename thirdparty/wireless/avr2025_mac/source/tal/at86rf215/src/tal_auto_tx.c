/**
 * @file tal_tx.c
 *
 * @brief This file handles the frame transmission within the TAL.
 *
 * $Id: tal_auto_tx.c 36436 2014-09-01 13:49:57Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "tal_config.h"

#ifndef BASIC_MODE

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_pib.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_internal.h"
#include "mac_build_config.h"
#ifdef ENABLE_RTB
#include "rtb.h"
#endif  /* ENABLE_RTB */

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

#define DEFAULT_FRAME_TYPES         ((1 << FCF_FRAMETYPE_BEACON) | \
                                     (1 << FCF_FRAMETYPE_DATA) | (1 << FCF_FRAMETYPE_MAC_CMD))
#define ACK_FRAME_TYPE_ONLY         (1 << FCF_FRAMETYPE_ACK)

/* === GLOBALS ============================================================= */

static uint8_t number_of_tx_retries[2];
static csma_mode_t global_csma_mode[2];
static bool ack_requested[2];
/* Last frame length for IFS handling. */
uint16_t last_txframe_length[2];
bool frame_buf_filled[2];

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
 * @return
 *      - @ref MAC_SUCCESS  if the TAL has accepted the data from NHLE for frame
 *                 transmission
 *      - @ref TAL_BUSY if the TAL is busy servicing the previous MAC request
 *      - @ref TAL_TRX_ASLEEP if the device is in sleep mode
 *      - @ref MAC_INVALID_PARAMETER is a provided parameter is invalid
 * @ingroup apiTalApi
 */
retval_t tal_tx_frame(trx_id_t trx_id, frame_info_t *tx_frame,
                      csma_mode_t csma_mode, bool perform_frame_retry)
{
    debug_text(PSTR("tal_tx_frame()"));

    if (tal_state[trx_id] == TAL_SLEEP)
    {
        return TAL_TRX_ASLEEP;
    }

    if (tal_state[trx_id] != TAL_IDLE)
    {
        debug_text_val(PSTR("TAL_BUSY, tal_state[trx_id] = "), tal_state[trx_id]);
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
    if ((tx_frame->len_no_crc + tal_pib[trx_id].FCSLen) > tal_pib[trx_id].MaxPHYPacketSize)
    {
        debug_text_val(PSTR("Invalid frame length = "), tx_frame->len_no_crc);
        return MAC_INVALID_PARAMETER;
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

    tal_state[trx_id] = TAL_TX;
    frame_buf_filled[trx_id] = false;
    global_csma_mode[trx_id] = csma_mode;

    /* Check if ACK is requested. */
    if (*mac_frame_ptr[trx_id]->mpdu & FCF_ACK_REQUEST)
    {
        ack_requested[trx_id] = true;
    }
    else
    {
        ack_requested[trx_id] = false;
    }

#ifdef SUPPORT_MODE_SWITCH
    if (tal_pib[trx_id].ModeSwitchEnabled)
    {
        save_current_phy(trx_id);
        set_csm(trx_id);
    }
#endif

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
#ifdef SUPPORT_MODE_SWITCH
        if (tal_pib[trx_id].ModeSwitchEnabled)
        {
            tx_ms_ppdu(trx_id);
        }
        else
#endif
        {
            transmit_frame(trx_id, NO_CCA);
        }
    }

    return MAC_SUCCESS;
}


/**
 * @brief Transmits frame
 *
 * @param trx_id Transceiver identifier
 * @param cca If WITH_CCA CCA is enforced, else no CCA is applied before transmission.
 */
void transmit_frame(trx_id_t trx_id, cca_use_t cca)
{
    debug_text(PSTR("transmit_frame()"));

    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    /* Configure auto modes */
    uint8_t amcs = 0;
    if (cca == WITH_CCA)
    {
        amcs |= AMCS_CCATX_MASK;
    }
    if (ack_requested[trx_id])
    {
        /* Enable Tx2RX */
        amcs |= AMCS_TX2RX_MASK;
        /* Configure frame filter to receive only ACK frames */
        pal_trx_reg_write(reg_offset + RG_BBC0_AFFTM, ACK_FRAME_TYPE_ONLY);
    }
    /* Other auto mode settings can be set to 0 */
    pal_trx_reg_write(reg_offset + RG_BBC0_AMCS, amcs);

    if (frame_buf_filled[trx_id] == false)
    {
        /* fill length field */
        uint16_t len = mac_frame_ptr[trx_id]->len_no_crc + tal_pib[trx_id].FCSLen;
        pal_trx_write(reg_offset + RG_BBC0_TXFLL, (uint8_t *)&len, 2);

        /* Store tx frame length to handle IFS next time */
        last_txframe_length[trx_id] = mac_frame_ptr[trx_id]->len_no_crc;

        /* Disable automatic FCS appending */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_TXAFCS, 0);
    }

    if (cca == WITH_CCA)
    {
        /* Trigger CCA measurement */
        debug_text(PSTR("transmit with CCA"));

        /* Disable BB; it will enabled for transmission automatically again */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_BBEN, 0);

        /* Ensure to reach the correct trx state */
        if (trx_state[trx_id] == RF_TRXOFF)
        {
            switch_to_txprep(trx_id);
        }
        if (trx_state[trx_id] != RF_RX)
        {
            debug_text(PSTR("Switch to Rx"));
            switch_to_rx(trx_id);
            pal_timer_delay(tal_pib[trx_id].agc_settle_dur); // allow filters to settle
        }

        /* Disable TRXRDY during CCA */
        pal_trx_bit_write(reg_offset + SR_RF09_IRQM_TRXRDY, 0);

        /* Start single ED measurement; use reg_write - it's the only sub-register */
        debug_text(PSTR("Start ED measurement"));
        pal_trx_reg_write(reg_offset + RG_RF09_EDC, RF_EDSINGLE);
        tx_state[trx_id] = TX_CCATX;
    }
    else // no CCA
    {
        if (trx_state[trx_id] != RF_TXPREP)
        {
            switch_to_txprep(trx_id);
        }

        debug_text(PSTR("switch to Tx"));
        pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_TX);
        trx_state[trx_id] = RF_TX;
        tx_state[trx_id] = TX_TX;
    }

#if (defined ENABLE_TSTAMP) || (defined MEASURE_ON_AIR_DURATION)
    pal_get_current_time(&fs_tstamp[trx_id]);
#endif

    /* Download frame content during CCA or during preamble transmission */
    if (frame_buf_filled[trx_id] == false)
    {
        /* fill frame buffer; do not provide FCS values */
        uint16_t tx_frm_buf_offset = BB_TX_FRM_BUF_OFFSET * trx_id;
        pal_trx_write(tx_frm_buf_offset + RG_BBC0_FBTXS,
                      (uint8_t *)mac_frame_ptr[trx_id]->mpdu,
                      mac_frame_ptr[trx_id]->len_no_crc);

#if DEBUG > 0
        for (uint16_t i = 0; i < mac_frame_ptr[trx_id]->len_no_crc; i++)
        {
            debug_text_val(PSTR("tx val = "), mac_frame_ptr[trx_id]->mpdu[i]);
        }
#endif

        /* Check if under-run has occurred */
        bool underrun = pal_trx_bit_read(reg_offset + SR_BBC0_PS_TXUR);
        /* Enable automatic FCS appending for remaining frame or later ACK transmission */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_TXAFCS, 1);
        if (underrun)
        {
            debug_text(PSTR("Tx underrun occured"));
            /* Abort ongoing transmission */
            pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_TRXOFF);
            trx_state[trx_id] = RF_TRXOFF;
            TAL_BB_IRQ_CLR(trx_id, BB_IRQ_TXFE);
            TAL_RF_IRQ_CLR(trx_id, RF_IRQ_TRXERR | RF_IRQ_TRXRDY | RF_IRQ_EDC);
            tx_done_handling(trx_id, FAILURE);
        }
        else
        {
            frame_buf_filled[trx_id] = true;
        }
    }
}


/**
 * @brief Handles interrupts issued due to end of transmission
 *
 * @param trx_id Transceiver identifier
 */
void handle_tx_end_irq(trx_id_t trx_id)
{
    debug_text_val(PSTR("handle_tx_end_irq(), tal_state "), tal_state[trx_id]);

    switch (tx_state[trx_id])
    {
        case TX_CCATX:
            {
                /* Check channel */
                uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
                uint8_t ccaed = pal_trx_bit_read(reg_offset + SR_BBC0_AMCS_CCAED);
                if (ccaed == BB_CH_CLEAR)
                {
                    debug_text(PSTR("Channel idle and frame has been sent"));
#ifdef MEASURE_ON_AIR_DURATION
                    tal_pib[trx_id].OnAirDuration +=
                        pal_sub_time_us(rxe_txe_tstamp[trx_id], fs_tstamp[trx_id]);
                    tal_pib[trx_id].OnAirDuration -= tal_pib[trx_id].CCADuration_us;
                    /* @ToDo: Minus processing delay */
#endif
                    /* Start ACK wait timer - see below */
                }
                else
                {
                    trx_state[trx_id] = RF_RX;
                    debug_text(PSTR("Channel busy"));
                    csma_continue(trx_id);
                    return;
                }
            }
            break;

        case TX_TX:
#ifdef MEASURE_ON_AIR_DURATION
            tal_pib[trx_id].OnAirDuration +=
                pal_sub_time_us(rxe_txe_tstamp[trx_id], fs_tstamp[trx_id]);
            /* @ToDo: Minus processing delay */
#endif
            /* Start ACK wait timer - see below */
            break;

#ifdef RX_WHILE_BACKOFF
        case TX_DEFER: /* Fall through */
#endif
        case TX_IDLE: // receive transaction -> ACK transmission
            /* ACK transmission completed */
            ack_transmission_done(trx_id);
            return;
            break;

#ifdef SUPPORT_MODE_SWITCH
        case TX_MS_PPDU:
            switch_to_txprep(trx_id);
#ifdef MEASURE_ON_AIR_DURATION
            tal_pib[trx_id].OnAirDuration +=
                pal_sub_time_us(rxe_txe_tstamp[trx_id], fs_tstamp[trx_id]);
            /* @ToDo: Minus processing delay */
#endif
            prepare_actual_transmission(trx_id);
            return;

        case TX_MS_NEW_MODE_TRANSMITTING:
            switch_to_txprep(trx_id);
#ifdef MEASURE_ON_AIR_DURATION
            tal_pib[trx_id].OnAirDuration +=
                pal_sub_time_us(rxe_txe_tstamp[trx_id], fs_tstamp[trx_id]);
            /* @ToDo: Minus processing delay */
#endif
            set_csm(trx_id);
            break;
#endif

        default:
#if (DEBUG > 0) && (defined SIO_HUB)
            printf("Unexpected tx_state 0x%02X\n", tx_state[trx_id]);
#endif
            break;
    }

    if (ack_requested[trx_id])
    {
#ifdef SUPPORT_MODE_SWITCH
        if (tal_pib[trx_id].ModeSwitchEnabled)
        {
            switch_to_rx(trx_id);
        }
        else
#endif
        {
            /* Trx is switched to RX automatically due to TX2RX setting */
            trx_state[trx_id] = RF_RX;
        }
        start_ack_wait_timer(trx_id);
    }
    else // No ACK requested
    {
        trx_state[trx_id] = RF_TXPREP;
        debug_text(PSTR("No ACK requested"));
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
    debug_text(PSTR("tx_done_handling()"));

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
                if (global_csma_mode[trx_id] == NO_CSMA_WITH_IFS)
                {
                    handle_ifs(trx_id);
                }
#ifdef SUPPORT_MODE_SWITCH
                if (tal_pib[trx_id].ModeSwitchEnabled)
                {
                    set_csm(trx_id);
                    tx_ms_ppdu(trx_id);
                }
                else
#endif
                {
                    transmit_frame(trx_id, NO_CCA);
                }
            }
            return; // next tx attempt and no tx done cb
        }
    }
    else    // status == CHANNEL_ACCESS_FAILURE or FAILURE
    {
        /* do nothing */
    }

#ifdef SUPPORT_MODE_SWITCH
    if (tal_pib[trx_id].ModeSwitchEnabled)
    {
        restore_previous_phy(trx_id);
    }
#endif

#ifdef ENABLE_TSTAMP
    mac_frame_ptr[trx_id]->time_stamp = fs_tstamp[trx_id];
#endif

    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    /* Re-store frame filter to pass "normal" frames */
    if (ack_requested[trx_id])
    {
        /* Configure frame filter to receive all allowed frame types */
#ifdef SUPPORT_FRAME_FILTER_CONFIGURATION
        pal_trx_reg_write(reg_offset + RG_BBC0_AFFTM, tal_pib[trx_id].frame_types);
#else
        pal_trx_reg_write(reg_offset + RG_BBC0_AFFTM, DEFAULT_FRAME_TYPES);
#endif
#ifdef MEASURE_TIME_OF_FLIGHT
        if (status == MAC_SUCCESS)
        {
            pal_trx_read(reg_offset + RG_BBC0_CNT0, (uint8_t *)&tal_pib[trx_id].TimeOfFlight, 4);
        }
#endif
    }
    /* Enable AACK again and disable CCA / TX procedure */
    pal_trx_reg_write(reg_offset + RG_BBC0_AMCS, AMCS_AACK_MASK);

    /* Set trx state for leaving TX transaction */
    if (trx_default_state[trx_id] == RF_RX)
    {
        switch_to_rx(trx_id);
    }
    else
    {
        pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_TRXOFF);
        trx_state[trx_id] = RF_TRXOFF;
    }

    tx_state[trx_id] = TX_IDLE;
    tal_state[trx_id] = TAL_IDLE;

#ifdef ENABLE_RTB
    rtb_tx_frame_done_cb(trx_id, status, mac_frame_ptr[trx_id]);
#else
    /* Regular handling without RTB */
    tal_tx_frame_done_cb(trx_id, status, mac_frame_ptr[trx_id]);
#endif
} /* tx_done_handling() */


/**
 * @brief Handles interframe spacing (IFS)
 *
 * @param trx_id Transceiver identifier
 */
static void handle_ifs(trx_id_t trx_id)
{
    debug_text(PSTR("handle_ifs()"));

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
            debug_text_val(PSTR("delay LIFS = "), (uint16_t)delay);
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
            debug_text_val(PSTR("delay SIFS = "), (uint16_t)delay);
            pal_timer_delay(delay);
        }
    }
}

#endif /* #ifndef BASIC_MODE */

/* EOF */
