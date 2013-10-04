/**
 * @file tal_csma.c
 *
 * @brief This file handles CSMA / CA before frame transmission within the TAL.
 *
 * $Id: tal_csma.c 35153 2013-09-23 12:01:57Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_config.h"
#include "tal_internal.h"


/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

static uint8_t NB[2];
static uint8_t BE[2];

/* === PROTOTYPES ========================================================== */

static void start_backoff(trx_id_t trx_id);
static void cca_start(void *parameter);

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Starts software-controlled CSMA.
 *
 * @param trx_id Transceiver identifier
 */
void csma_start(trx_id_t trx_id)
{
    //debug_text(PSTR("csma_start()"));

    /* Initialize CSMA variables */
    NB[trx_id] = 0;
    BE[trx_id] = tal_pib[trx_id].MinBE;

    /* Start backoff timer to trigger CCA */
    start_backoff(trx_id);
}


/**
 * @brief Starts the timer for the backoff period and enables receiver.
 *
 * @param trx_id Transceiver identifier
 */
static void start_backoff(trx_id_t trx_id)
{
    //debug_text(PSTR("start_backoff()"));

    /* Start backoff timer to trigger CCA */
    uint8_t backoff_8;
    backoff_8  = (uint8_t)rand() & ((1 << BE[trx_id]) - 1);
    if (backoff_8 > 0)
    {
        uint8_t timer_id;
        uint16_t backoff_16;
        uint32_t backoff_duration_us;
        backoff_16 = backoff_8 * aUnitBackoffPeriod;
        backoff_duration_us = tal_pib[trx_id].SymbolDuration_us * backoff_16;
#ifdef REDUCED_BACKOFF_DURATION
        backoff_duration_us = REDUCED_BACKOFF_DURATION;
#endif

        if (trx_id == RF09)
        {
            timer_id = TAL_T_0;
        }
        else
        {
            timer_id = TAL_T_1;
        }
        //debug_text_val(PSTR("start backoff timer"), (uint16_t)backoff_duration_us);

        retval_t status =
            pal_timer_start(timer_id, backoff_duration_us, TIMEOUT_RELATIVE,
                            (FUNC_PTR)cca_start, (void *)&timer_cb_parameter[trx_id]);
        if (status != MAC_SUCCESS)
        {
            tx_done_handling(trx_id, status);

            return;
        }
        tal_state[trx_id] = TAL_BACKOFF;

#ifdef RX_WHILE_BACKOFF
        /* Keep receiver on during backoff */
        if ((trx_default_state[trx_id] == RF_RX) &&
            (tal_pib[trx_id].NumRxFramesDuringBackoff < tal_pib[trx_id].MaxNumRxFramesDuringBackoff))
        {
            if (trx_state[trx_id] != RF_RX)
            {
                if (trx_state[trx_id] == RF_TRXOFF)
                {
                    switch_to_txprep(trx_id);
                }
                switch_to_rx(trx_id);
            }
        }
        else
#endif
        {
#ifdef USE_TXPREP_DURING_BACKOFF
            /* Switch to TXPREP during backoff */
            //debug_text(PSTR("switch to TXPREP during backoff"));
            if (trx_state[trx_id] != RF_TXPREP)
            {
                switch_to_txprep(trx_id);
            }
#else
            /* Switch to TRXOFF during backoff */
            //debug_text(PSTR("switch to TRXOFF during backoff"));
            if (trx_state[trx_id] != RF_TRXOFF)
            {
                uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
                pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TRXOFF);
                trx_state[trx_id] = RF_TRXOFF;
            }
#endif
        }
    }
    else // no backoff required
    {
        /* Start CCA immediately - no backoff */
        cca_start((void *)&timer_cb_parameter[trx_id]);
    }
}


/**
 * @brief Start CCA.
 *
 * @param parameter Pointer to trx_id
 */
static void cca_start(void *parameter)
{
    trx_id_t trx_id = *(trx_id_t *)parameter;

    //debug_text_val(PSTR("cca_start(), trx_id = "), trx_id);

    /* Check if trx is currently detecting a frame ota */
    if (trx_state[trx_id] == RF_RX)
    {
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        uint8_t agc_freeze = pal_trx_bit_read(rf_reg_offset + SR_RF09_AGCC_FRZS);
        if (agc_freeze)
        {
            //debug_text(PSTR("AGC is freezed"));
            csma_continue(trx_id);
        }
        else
        {
            trigger_cca_meaurement(trx_id);
        }
    }
    else
    {
        trigger_cca_meaurement(trx_id);
    }
}


/**
 * @brief Triggers CCA measurement at transceiver
 *
 * @param trx_id Transceiver identifier
 */
void trigger_cca_meaurement(trx_id_t trx_id)
{
    /* Trigger CCA measurement */
    //debug_text(PSTR("trigger_cca_meaurement()"));

    /* Cancel any ongoing reception and ensure that TXPREP is reached. */
    if (trx_state[trx_id] != RF_TXPREP)
    {
        switch_to_txprep(trx_id);
    }

    /* Disable BB */
    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_BBEN, 0);

    /* CCA duration is already set by default; see apply_phy_settings() */
    /* Setup and start energy detection */
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    pal_trx_bit_write(rf_reg_offset + SR_RF09_AGCC_FRZC, 0);// Ensure AGC is not hold
    //debug_text(PSTR("Switch to Rx"));
    pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_RX);
    trx_state[trx_id] = RF_RX;
    tal_state[trx_id] = TAL_CCA;
    /* Start single ED measurement; use reg_write - it's the only subregister */
    pal_trx_reg_write(rf_reg_offset + RG_RF09_EDC, RF_EDSINGLE);

    /* Wait for EDC IRQ and handle it within cca_done_handling() */
}


/**
 * @brief Callback function for CCA completion.
 *
 * @param trx_id Transceiver identifier
 */
void cca_done_handling(trx_id_t trx_id)
{
    //debug_text(PSTR("cca_done_handling()"));

    switch_to_txprep(trx_id); /* Leave state Rx */

    /* Switch BB on again */
    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_BBEN, 1);

    /* Determine if channel is idle */
    if (tal_current_ed_val[trx_id] < tal_pib[trx_id].CCAThreshold)
    {
        /* Idle */
        //debug_text(PSTR("channel idle"));
        tal_transmit_frame(trx_id);
    }
    else
    {
        /* Busy */
        //debug_text(PSTR("channel busy"));
        csma_continue(trx_id);
    }
}


/**
 * @brief Continues CSMA; handles next CSMA retry.
 *
 * @param trx_id Transceiver identifier
 */
void csma_continue(trx_id_t trx_id)
{
    //debug_text(PSTR("csma_continue()"));

    NB[trx_id]++;
    //debug_text_val(PSTR("NB = "), NB[trx_id]);
    if (NB[trx_id] > tal_pib[trx_id].MaxCSMABackoffs)
    {
        tx_done_handling(trx_id, MAC_CHANNEL_ACCESS_FAILURE);
    }
    else
    {
        BE[trx_id]++;
        if (BE[trx_id] > tal_pib[trx_id].MaxBE)
        {
            BE[trx_id] = tal_pib[trx_id].MaxBE;
        }
        /* Start backoff timer to trigger CCA */
        start_backoff(trx_id);
    }
}


/* EOF */
