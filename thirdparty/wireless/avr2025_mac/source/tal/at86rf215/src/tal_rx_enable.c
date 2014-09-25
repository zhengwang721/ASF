/**
 * @file tal_rx_enable.c
 *
 * @brief File provides functionality supporting RX-Enable feature.
 *
 * $Id: tal_rx_enable.c 36436 2014-09-01 13:49:57Z uwalter $
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
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_internal.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Switches receiver on or off
 *
 * This function switches the receiver on (PHY_RX_ON) or off (PHY_TRX_OFF).
 *
 * @param trx_id Transceiver identifier
 * @param state New state of receiver
 *
 * @return
 *      - @ref TAL_BUSY if the TAL state machine cannot switch receiver on or off,
 *      - @ref PHY_TRX_OFF if receiver has been switched off, or
 *      - @ref PHY_RX_ON otherwise.
 *
 * @ingroup apiTalApi
 */
uint8_t tal_rx_enable(trx_id_t trx_id, uint8_t state)
{
    uint8_t ret_val;

    debug_text_val(PSTR("tal_rx_enable, trx_id ="), trx_id);

    if (tal_state[trx_id] == TAL_SLEEP)
    {
        debug_text(PSTR("TAL_TRX_ASLEEP"));
        return TAL_TRX_ASLEEP;
    }

    /*
     * Trx can only be enabled if TAL is not busy;
     * i.e. if TAL is IDLE.
     */
    if (tal_state[trx_id] != TAL_IDLE)
    {
        debug_text(PSTR("TAL is busy"));
        return TAL_BUSY;
    }

    if (state == PHY_TRX_OFF)
    {
        debug_text(PSTR("Switch to PHY_TRX_OFF"));
        /*
            * If the rx needs to be switched off,
            * we are not interested in a frame that is currently being received.
            */
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_TRXOFF);
#ifdef IQ_RADIO
        pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_CMD, RF_TRXOFF);
#endif
        trx_state[trx_id] = RF_TRXOFF;
        tal_buf_shortage[trx_id] = false;
        ret_val = PHY_TRX_OFF;
#ifdef ENABLE_FTN_PLL_CALIBRATION
        stop_ftn_timer(trx_id);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */
        trx_default_state[trx_id] = RF_TRXOFF;
    }
    else
    {
        debug_text(PSTR("Switch to PHY_RX_ON"));
        switch_to_txprep(trx_id);
        switch_to_rx(trx_id);
        ret_val = PHY_RX_ON;
#ifdef ENABLE_FTN_PLL_CALIBRATION
        start_ftn_timer(trx_id);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */
        trx_default_state[trx_id] = RF_RX;
    }

    return ret_val;
}

/* EOF */
