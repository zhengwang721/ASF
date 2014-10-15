/**
 * @file tal_ack.c
 *
 * @brief This file implements acknowledgment handling.
 *
 * $Id: tal_auto_ack.c 36441 2014-09-01 14:28:22Z uwalter $
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
#include <inttypes.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "stack_config.h"
#include "tal_pib.h"
#include "tal_internal.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

static void ack_timout_cb(void *parameter);

/* === IMPLEMENTATION ====================================================== */



/* --- ACK transmission ---------------------------------------------------- */


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
    debug_text(PSTR("ack_transmission_done()"));

#ifdef MEASURE_ON_AIR_DURATION
    tal_pib[trx_id].OnAirDuration += tal_pib[trx_id].ACKDuration_us;
#endif

#ifdef RX_WHILE_BACKOFF
    if (tx_state[trx_id] == TX_DEFER)
    {
        csma_start(trx_id);
    }
    else
#endif
    {
        complete_rx_transaction(trx_id);
    }
}


/* --- ACK reception ------------------------------------------------------- */


/**
 * @brief Checks if received frame is an ACK frame
 *
 * @param trx_id Transceiver identifier
 *
 * @return true if frame is an ACK frame, else false
 */
bool is_frame_an_ack(trx_id_t trx_id)
{
    debug_text(PSTR("is_frame_an_ack()"));

    bool ret;

    /* Check frame length */
    if (rx_frm_info[trx_id]->len_no_crc == 3)
    {
        /* Check frame type and frame version */
        if ((rx_frm_info[trx_id]->mpdu[0] & FCF_FRAMETYPE_ACK) &&
            (((rx_frm_info[trx_id]->mpdu[1] >> FCF1_FV_SHIFT) & 0x03) <= FCF_FRAME_VERSION_2006))
        {
            debug_text(PSTR("Frame is an ACK"));
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}


/**
 * @brief Checks if received ACK is an valid ACK frame
 *
 * @param trx_id Transceiver identifier
 *
 * @return true if ACK frame is valid, else false
 */
bool is_ack_valid(trx_id_t trx_id)
{
    debug_text(PSTR("is_ack_valid()"));

    bool ret;

    /* Check sequence number */
    if (rx_frm_info[trx_id]->mpdu[2] == mac_frame_ptr[trx_id]->mpdu[2])
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}


/**
 * @brief Starts the timer to wait for an ACK reception
 *
 * @param trx_id Id of the corresponding trx
 */
void start_ack_wait_timer(trx_id_t trx_id)
{
    debug_text_val(PSTR("ACK time out"), tal_pib[trx_id].ACKWaitDuration);
    debug_text(PSTR("waiting for ACK"));

    tx_state[trx_id] = TX_WAITING_FOR_ACK;

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
        pal_timer_start(timer_id, tal_pib[trx_id].ACKWaitDuration, TIMEOUT_RELATIVE,
                        (FUNC_PTR)ack_timout_cb, (void *)&timer_cb_parameter[trx_id]);
    if (status != MAC_SUCCESS)
    {
        debug_text_val(PSTR("ACK timer could not be started, "), status);
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_TRXOFF);
        trx_state[trx_id] = RF_TRXOFF;
        tx_done_handling(trx_id, status);
    }
}


/**
 * @brief Callback function for ACK timeout
 *
 * This function is called if the ACK timeout timer fires.
 *
 * @param parameter Pointer to trx_id
 */
void ack_timout_cb(void *parameter)
{
    debug_text(PSTR("ack_timout_cb()"));

    trx_id_t trx_id = *(trx_id_t *)parameter;

    /* Configure frame filter to receive all allowed frame types */
    /* Re-store frame filter to pass "normal" frames */
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
#ifdef SUPPORT_FRAME_FILTER_CONFIGURATION
    pal_trx_reg_write(reg_offset + RG_BBC0_AFFTM, tal_pib[trx_id].frame_types);
#else
    pal_trx_reg_write(reg_offset + RG_BBC0_AFFTM, DEFAULT_FRAME_TYPES);
#endif

    stop_tal_timer(trx_id);

    tx_done_handling(trx_id, MAC_NO_ACK);
}


#endif /* #ifndef BASIC_MODE */

/*  EOF */
