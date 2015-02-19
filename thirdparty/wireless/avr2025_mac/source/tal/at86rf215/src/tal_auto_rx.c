/**
 * @file tal_rx.c
 *
 * @brief This file implements functions to handle received frames.
 *
 * $Id: tal_auto_rx.c 36369 2014-08-25 09:12:51Z uwalter $
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
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_pib.h"
#include "tal_internal.h"

/* === EXTERNALS =========================================================== */

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

frame_info_t *rx_frm_info[NO_TRX];

/* === PROTOTYPES ========================================================== */

static void handle_incoming_frame(trx_id_t trx_id);
static bool upload_frame(trx_id_t trx_id);

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Handle received frame interrupt
 *
 * This function handles transceiver interrupts for received frames.
 *
 * @param trx_id Transceiver identifier
 */
void handle_rx_end_irq(trx_id_t trx_id)
{
    debug_text_val(PSTR("handle_rx_end_irq() for trx_id "), trx_id);

    trx_state[trx_id] = RF_TXPREP;

    if (upload_frame(trx_id) == false)
    {
        debug_text(PSTR("Upload frame failed"));
        return;
    }

#ifdef RX_WHILE_BACKOFF
    if (tx_state[trx_id] == TX_BACKOFF)
    {
        /* Stop backoff timer */
        stop_tal_timer(trx_id);
        tx_state[trx_id] = TX_DEFER;
        tal_pib[trx_id].NumRxFramesDuringBackoff++;
    }
#endif

#ifdef SUPPORT_MODE_SWITCH
    if (tal_pib[trx_id].ModeSwitchEnabled)
    {
        debug_text_val(PSTR("Mode switch RXFE, tal_state "), tal_state[trx_id]);
        if (tal_pib[trx_id].phy.modulation == FSK)
        {
            debug_text(PSTR("Check for mode switch"));
            uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
            if (trx_bit_read(reg_offset + SR_BBC0_FSKPHRRX_MS) == 0x01)
            {
                handle_rx_ms_packet(trx_id);
                return;
            }
        }
        if (tal_state[trx_id] == TAL_NEW_MODE_RECEIVING)
        {
            /* Restore previous PHY, i.e. CSM */
            debug_text(PSTR("Frame in new mode has been received"));
            /* Stop timer waiting for incoming frame at new mode */
            stop_tal_timer(trx_id);
            set_csm(trx_id);
            tal_state[trx_id] = TAL_IDLE;
        }
    }
#endif

#ifdef PROMISCUOUS_MODE
    if (tal_pib[trx_id].PromiscuousMode)
    {
        complete_rx_transaction(trx_id);
        return;
    }
#endif /* #ifdef PROMISCUOUS_MODE */

    handle_incoming_frame(trx_id);
}


/**
 * @brief Handles incoming frame from transceiver
 *
 * @param trx_id Transceiver identifier
 */
static void handle_incoming_frame(trx_id_t trx_id)
{
    debug_text_val(PSTR("handle_incoming_frame(), trx_id ="), trx_id);

    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    if (is_frame_an_ack(trx_id))
    {
        if (tx_state[trx_id] == TX_WAITING_FOR_ACK)
        {
            if (is_ack_valid(trx_id))
            {
                /* Stop ACK timeout timer */
                debug_text(PSTR("Stop ACKWaitDuration timer"));
                stop_tal_timer(trx_id);
                /* Re-store frame filter to pass "normal" frames */
                /* Configure frame filter to receive all allowed frame types */
#ifdef SUPPORT_FRAME_FILTER_CONFIGURATION
                trx_reg_write(reg_offset + RG_BBC0_AFFTM, tal_pib[trx_id].frame_types);
#else
                trx_reg_write(reg_offset + RG_BBC0_AFFTM, DEFAULT_FRAME_TYPES);
#endif
                tx_done_handling(trx_id, MAC_SUCCESS);
            }
            else
            {
                /* Continue waiting for incoming ACK */
                switch_to_rx(trx_id);
            }
        }
        else
        {
            /* No interest in ACKs */
            switch_to_rx(trx_id);
        }

        return; /* no further processing of ACK frames */
    }

    /* Check if ACK transmission is done by transceiver */
    bool ack_transmitting = trx_bit_read(reg_offset + SR_BBC0_AMCS_AACKFT);
    if (ack_transmitting)
    {
	    debug_text(PSTR("ACK transmitting"));
    }
    else
    {
        debug_text(PSTR("No ACK transmitting"));
        complete_rx_transaction(trx_id);
#ifdef RX_WHILE_BACKOFF
        if (tx_state[trx_id] == TX_DEFER)
        {
            csma_start(trx_id);
        }
#endif
    }
}


/**
 * @brief Parses received frame and create the frame_info_t structure
 *
 * This function parses the received frame and creates the frame_info_t
 * structure to be sent to the MAC as a parameter of tal_rx_frame_cb().
 *
 * @param trx_id Transceiver identifier
 */
static bool upload_frame(trx_id_t trx_id)
{
    debug_text_val(PSTR("upload_frame(), trx_id ="), trx_id);

    if (tal_rx_buffer[trx_id] == NULL)
    {
        Assert("no tal_rx_buffer available" == 0);
        return false;
    }

    rx_frm_info[trx_id] = (frame_info_t *)BMM_BUFFER_POINTER(tal_rx_buffer[trx_id]);

    /* Get Rx frame length */
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    uint16_t phy_frame_len;
    trx_read(reg_offset + RG_BBC0_RXFLL, (uint8_t *)&phy_frame_len, 2);
    debug_text_val(PSTR("Frm len = "), phy_frame_len);
    rx_frm_info[trx_id]->len_no_crc = phy_frame_len - tal_pib[trx_id].FCSLen;

    /* Update payload pointer to store received frame. */
    rx_frm_info[trx_id]->mpdu = (uint8_t *)rx_frm_info[trx_id] + LARGE_BUFFER_SIZE -
                                phy_frame_len - ED_VAL_LEN - LQI_LEN;

#ifdef ENABLE_TSTAMP
    /* Store the timestamp. */
    rx_frm_info[trx_id]->time_stamp = fs_tstamp[trx_id];
#endif

    /* Upload received frame to buffer */
#ifdef UPLOAD_CRC
    uint16_t len = rx_frm_info[trx_id]->len_no_crc + tal_pib[trx_id].FCSLen;
#else
    uint16_t len = rx_frm_info[trx_id]->len_no_crc;
#endif
    uint16_t rx_frm_buf_offset = BB_RX_FRM_BUF_OFFSET * trx_id;
    trx_read(rx_frm_buf_offset + RG_BBC0_FBRXS, rx_frm_info[trx_id]->mpdu, len);

    return true;
}


/**
 * @brief Completes Rx transaction
 *
 * @param trx_id Transceiver identifier
 */
void complete_rx_transaction(trx_id_t trx_id)
{
    debug_text_val(PSTR("complete_rx_transaction(), trx_id = "), trx_id);

    /* Get energy of received frame */
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    uint8_t ed = trx_reg_read(reg_offset + RG_RF09_EDV);
    debug_text_val(PSTR("Energy of received frame = "), ed);
    uint16_t ed_pos = rx_frm_info[trx_id]->len_no_crc + 1 + tal_pib[trx_id].FCSLen;
    rx_frm_info[trx_id]->mpdu[ed_pos] = ed; // PSDU, LQI, ED

    /* Append received frame to incoming_frame_queue and get new rx buffer. */
    qmm_queue_append(&tal_incoming_frame_queue[trx_id], tal_rx_buffer[trx_id]);
    /* The previous buffer is eaten up and a new buffer is not assigned yet. */
    tal_rx_buffer[trx_id] = bmm_buffer_alloc(LARGE_BUFFER_SIZE);
    /* Switch to rx again to handle buffer shortage */
    switch_to_rx(trx_id);
}


/**
 * @brief Parses received frame and create the frame_info_t structure
 *
 * This function parses the received frame and creates the frame_info_t
 * structure to be sent to the MAC as a parameter of tal_rx_frame_cb().
 *
 * @param trx_id Transceiver identifier
 * @param buf_ptr Pointer to the buffer containing the received frame
 */
void process_incoming_frame(trx_id_t trx_id, buffer_t *buf_ptr)
{
    debug_text_val(PSTR("process_incoming_frame(), trx_id = "), trx_id);

    frame_info_t *receive_frame = (frame_info_t *)BMM_BUFFER_POINTER(buf_ptr);
    receive_frame->buffer_header = buf_ptr;

    /* Scale ED value to a LQI value: 0x00 - 0xFF */
    uint16_t lqi_pos = rx_frm_info[trx_id]->len_no_crc + tal_pib[trx_id].FCSLen;
    receive_frame->mpdu[lqi_pos] =
        scale_ed_value((int8_t)receive_frame->mpdu[lqi_pos + 1]);

#ifdef ENABLE_RTB
    /* The callback function implemented by RTB is invoked. */
    rtb_rx_frame_cb(trx_id, receive_frame);
#else
    /* The callback function implemented by MAC is invoked. */
    tal_rx_frame_cb(trx_id, receive_frame);
#endif
} /* process_incoming_frame() */

#endif /* #ifndef BASIC_MODE */

/*  EOF */
