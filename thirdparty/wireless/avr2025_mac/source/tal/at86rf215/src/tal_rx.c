/**
 * @file tal_rx.c
 *
 * @brief This file implements functions to handle received frames.
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
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_config.h"
#include "tal_pib.h"
#include "tal_internal.h"


/* === EXTERNALS =========================================================== */

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

#define UPLOADED_MHR_LENGTH     7

/* === GLOBALS ============================================================= */

frame_info_t *rx_frm_info[2];
uint8_t *rx_frm_ptr[2];

/* === PROTOTYPES ========================================================== */

static bool prepare_upload(trx_id_t trx_id);
static void upload_mhr(trx_id_t trx_id);
static void handle_incoming_frame(trx_id_t trx_id);

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
    //debug_text_val(PSTR("handle_rx_end_irq() for trx_id "), trx_id);

    trx_state[trx_id] = RF_TXPREP;

    if (prepare_upload(trx_id))
    {
#ifdef PROMISCUOUS_MODE
        if (tal_pib[trx_id].PromiscuousMode)
        {
            upload_frame(trx_id);
            complete_rx_transaction(trx_id);
        }
        else
#endif
        {
            upload_mhr(trx_id);
            /* Check if an ACK needs to be checked */
            if ((tal_state[trx_id] == TAL_WAITING_FOR_ACK_RECEPTION) &&
                handle_ack_reception(trx_id))
            {
				
                /* nothing to do */
            }
            else
            {
                handle_incoming_frame(trx_id);
            }
        }
    }
}


/**
 * @brief Prepares the uploads of data from the transceiver
 *
 * @param trx_id Transceiver identifier
 *
 * @return true if buffer for upload is available, else false
 */
static bool prepare_upload(trx_id_t trx_id)
{
    //debug_text_val(PSTR("prepare_upload(), trx_id ="), trx_id);

    bool ret = true;

    if (tal_rx_buffer[trx_id] == NULL)
    {
        Assert("no tal_rx_buffer available" == 0);
        ret = false;
    }
    else
    {
        rx_frm_info[trx_id] = (frame_info_t *)BMM_BUFFER_POINTER(tal_rx_buffer[trx_id]);

        /* Get Rx frame length */
        uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
        uint16_t phy_frame_len;
        pal_trx_read(bb_reg_offset + RG_BBC0_RXFLL, (uint8_t *)&phy_frame_len, 2);
        //debug_text_val(PSTR("Frm len = "), phy_frame_len);
        rx_frm_info[trx_id]->length = phy_frame_len - tal_pib[trx_id].FCSLen;

        /* Update payload pointer to store received frame. */
        rx_frm_ptr[trx_id] = (uint8_t *)rx_frm_info[trx_id] + LARGE_BUFFER_SIZE -
                             phy_frame_len - ED_VAL_LEN - LQI_LEN;
        rx_frm_info[trx_id]->mpdu = rx_frm_ptr[trx_id];
    }

    return ret;
}


/**
 * @brief Handles incoming frame from transceiver
 *
 * @param trx_id Transceiver identifier
 */
static void handle_incoming_frame(trx_id_t trx_id)
{
    //debug_text_val(PSTR("handle_incoming_frame(), trx_id ="), trx_id);

    if (rx_frm_info[trx_id]->length > 3) // no ACK upload
    {
        /* Check if ACK transmission is required */
        if (is_ack_required(trx_id))
        {
#ifdef RX_WHILE_BACKOFF
            if (tal_state[trx_id] == TAL_BACKOFF)
            {
                /* Stop backoff timer */
                stop_tal_timer(trx_id);
                csma_backoff_onhold[trx_id] = true;
                tal_pib[trx_id].NumRxFramesDuringBackoff++;
            }
#endif
            schedule_ack_transmission(trx_id);
            /* Frame is uploaded during ACK transmission */
        }
        else
        {
            upload_frame(trx_id);
            complete_rx_transaction(trx_id);
        }
    }
    else // len <= 3
    {
        /* Do not handle unexpected ACK */
        switch_to_rx(trx_id);
    }
}


/**
 * @brief Uploads MHR or part of it from transceiver
 *
 * @param trx_id Transceiver identifier
 */
static void upload_mhr(trx_id_t trx_id)
{
    //debug_text_val(PSTR("upload_mhr(), trx_id = "), trx_id);

    uint8_t len;
    /* Check if frame is longer than required MHR length */
    if (rx_frm_info[trx_id]->length > (UPLOADED_MHR_LENGTH - 1))
    {
        len = UPLOADED_MHR_LENGTH;
    }
    else
    {
        len = rx_frm_info[trx_id]->length;
    }

    /* Upload MHR to buffer */
    uint16_t rx_frm_buf_offset = BB_RX_FRM_BUF_OFFSET * trx_id;
    pal_trx_read(rx_frm_buf_offset + RG_BBC0_FBRXS, rx_frm_ptr[trx_id], len);

    /* Update frame ptr to continue remaining upload later */
    rx_frm_ptr[trx_id] += (uint16_t)len;
}


/**
 * @brief Parses received frame and create the frame_info_t structure
 *
 * This function parses the received frame and creates the frame_info_t
 * structure to be sent to the MAC as a parameter of tal_rx_frame_cb().
 *
 * @param trx_id Transceiver identifier
 */
void upload_frame(trx_id_t trx_id)
{
    //debug_text_val(PSTR("upload_frame(), trx_id ="), trx_id);

    /* Get energy of received frame */
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    uint8_t ed = pal_trx_reg_read(rf_reg_offset + RG_RF09_EDV);
    //debug_text_val(PSTR("Energy of received frame = "), ed);
    uint16_t ed_pos = rx_frm_info[trx_id]->length + 1 + tal_pib[trx_id].FCSLen;
    rx_frm_info[trx_id]->mpdu[ed_pos] = ed; // PSDU, LQI, ED

#ifdef ENABLE_TSTAMP
    /* Store the timestamp. */
    rx_frm_info[trx_id]->time_stamp = fs_tstamp[trx_id];
#endif

    /* Upload received frame to buffer */
    uint8_t uploaded_len = rx_frm_ptr[trx_id] - rx_frm_info[trx_id]->mpdu;
#ifdef UPLOAD_CRC
    uint16_t remaining_len = rx_frm_info[trx_id]->length - uploaded_len +
                             tal_pib[trx_id].FCSLen;
#else
    uint16_t remaining_len = rx_frm_info[trx_id]->length - uploaded_len;
#endif
    if (remaining_len > 0)
    {
        uint16_t rx_frm_buf_offset = BB_RX_FRM_BUF_OFFSET * trx_id;
        pal_trx_read(rx_frm_buf_offset + uploaded_len + RG_BBC0_FBRXS,
                     rx_frm_ptr[trx_id], remaining_len);
    }
}


/**
 * @brief Completes Rx transaction
 *
 * @param trx_id Transceiver identifier
 */
void complete_rx_transaction(trx_id_t trx_id)
{
    //debug_text_val(PSTR("complete_rx_transaction(), trx_id = "), trx_id);

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
    //debug_text_val(PSTR("process_incoming_frame(), trx_id = "), trx_id);

    frame_info_t *receive_frame = (frame_info_t *)BMM_BUFFER_POINTER(buf_ptr);
    receive_frame->buffer_header = buf_ptr;

    /* Scale ED value to a LQI value: 0x00 - 0xFF */
    uint16_t lqi_pos = rx_frm_info[trx_id]->length + tal_pib[trx_id].FCSLen;
    receive_frame->mpdu[lqi_pos] =
        scale_ed_value((int8_t)receive_frame->mpdu[lqi_pos + 1]);

    tal_rx_frame_cb(trx_id, receive_frame);

    return;
} /* process_incoming_frame() */


/*  EOF */
