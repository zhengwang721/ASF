/**
 * @file tal_ftn.c
 *
 * @brief This file implements the filter periodic tuning.
 *
 * $Id: tal_ftn.c 35124 2013-09-19 11:22:42Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================= */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_config.h"
#include "tal_pib.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_internal.h"
#include "mac_build_config.h"


#if (defined ENABLE_FTN_PLL_CALIBRATION) || (defined DOXYGEN)

/* === TYPES ================================================================ */

/* === MACROS =============================================================== */

/*
 * PLL calibration and filter tuning timer timeout in minutes
 */
#define TAL_CALIBRATION_TIMEOUT_MIN         (30UL)

/*
 * PLL calibration and filter tuning timer timeout in us,
 */
#define TAL_CALIBRATION_TIMEOUT_US          ((TAL_CALIBRATION_TIMEOUT_MIN) * (60UL) * (1000UL) * (1000UL))

/* === GLOBALS ============================================================== */

/* === PROTOTYPES =========================================================== */

static void ftn_timer_cb(void *parameter);

/* === Implementation ======================================================= */

/**
 * @brief Starts the filter tuning timer
 *
 * @param trx_id Transceiver identifier
 */
void start_ftn_timer(trx_id_t trx_id)
{
    uint8_t timer_id;

    /* Handle PLL calibration and filter tuning. */
    retval_t timer_status;

    /* Calibration timer has already been stopped within this function. */

    if (trx_id == RF09)
    {
        timer_id = TAL_T_CALIBRATION_0;
    }
    else
    {
        timer_id = TAL_T_CALIBRATION_1;
    }

    /* Start periodic calibration timer. */
    timer_status = pal_timer_start(timer_id, TAL_CALIBRATION_TIMEOUT_US,
                                   TIMEOUT_RELATIVE, (FUNC_PTR)ftn_timer_cb,
                                   NULL);

    if (timer_status != MAC_SUCCESS)
    {
        Assert("PLL calibration timer start problem" == 0);
    }
}


/**
 * @brief Stops the filter tuning timer
 *
 * @param trx_id Transceiver identifier
 */
void stop_ftn_timer(trx_id_t trx_id)
{
    uint8_t timer_id;

    if (trx_id == RF09)
    {
        timer_id = TAL_T_CALIBRATION_0;
    }
    else
    {
        timer_id = TAL_T_CALIBRATION_1;
    }
    pal_timer_stop(timer_id);
}


/**
 * @brief Timer callback function for the filter tuning
 *
 * This function executes the filter tuning and restarts its timer again.
 * @param parameter Pointer to trx_id
 */
static void ftn_timer_cb(void *parameter)
{
    uint8_t timer_id;
    trx_id_t trx_id = *(trx_id_t *)parameter;

    //debug_text(PSTR("ftn_timer_cb()"));

    if (tal_state == TAL_IDLE)
    {
        if (trx_state[trx_id] == RF_RX)
        {
            uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
            /* Keep analog voltage on while toggling TRXOFF */
            pal_trx_bit_write(rf_reg_offset + SR_RF09_AUXS_AVEN, 1);
            pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TRXOFF);
            switch_to_txprep(trx_id);
            pal_trx_bit_write(rf_reg_offset + SR_RF09_AUXS_AVEN, 0);
            switch_to_rx(trx_id);
        }
        else
        {
            //debug_text(PSTR("unexpected Trx state"));
        }

        start_ftn_timer(trx_id); // Restart timer again
    }
    else
    {
        if (trx_id == RF09)
        {
            timer_id = TAL_T_CALIBRATION_0;
        }
        else
        {
            timer_id = TAL_T_CALIBRATION_1;
        }
        /* Postpone filter tuning, since TAL is busy */
        pal_timer_start(timer_id, 1000000, TIMEOUT_RELATIVE,
                        (FUNC_PTR)ftn_timer_cb, NULL);
    }
}



#endif  /* ENABLE_FTN_PLL_CALIBRATION */


/* EOF */
