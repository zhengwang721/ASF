/**
 * @file tal_pwr_mgmt.c
 *
 * @brief This file implements TAL power management functionality
 *        of the transceiver.
 *
 * $Id: tal_pwr_mgmt.c 36436 2014-09-01 13:49:57Z uwalter $
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

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "pal.h"
#include "return_val.h"
#include "ieee_const.h"
#include "tal.h"
#include "tal_config.h"
#include "tal_internal.h"
#include "tal_pib.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Sets the transceiver to SLEEP
 *
 * This function sets the transceiver to state SLEEP.
 *
 * @param trx_id Transceiver identifier
 *
 * @return
 *      - @ref TAL_BUSY - The transceiver is busy in TX or RX
 *      - @ref MAC_SUCCESS - The transceiver is put to sleep
 *      - @ref TAL_TRX_ASLEEP - The transceiver is already asleep
 *      - @ref MAC_INVALID_PARAMETER - The specified sleep mode is not supported
 * @ingroup apiTalApi
 */
retval_t tal_trx_sleep(trx_id_t trx_id)
{
    debug_text(PSTR("tal_trx_sleep()"));

    if (tal_state[trx_id] == TAL_SLEEP)
    {
        debug_text(PSTR("TAL_TRX_ASLEEP"));
        return TAL_TRX_ASLEEP;
    }

    /* Device can be put to sleep only when the TAL is in IDLE state. */
    if (tal_state[trx_id] != TAL_IDLE)
    {
        debug_text(PSTR("TAL_BUSY"));
        return TAL_BUSY;
    }

    tal_state[trx_id] = TAL_SLEEP;

    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    trx_reg_write(reg_offset + RG_RF09_CMD, RF_TRXOFF);
#ifdef IQ_RADIO
    trx_reg_write(RF215_RF, reg_offset + RG_RF09_CMD, RF_TRXOFF);
#endif
    trx_reg_write(reg_offset + RG_RF09_CMD, RF_SLEEP);
#ifdef IQ_RADIO
    trx_reg_write(RF215_RF, reg_offset + RG_RF09_CMD, RF_SLEEP);
#endif
    TAL_BB_IRQ_CLR_ALL(trx_id);
    TAL_RF_IRQ_CLR_ALL(trx_id);
    trx_state[trx_id] = RF_SLEEP;

    /*
     * Free TAL Rx buffer. During sleep no buffer is required.
     * With tal_trx_wakeup() a new buffer gets allocated.
     */
    bmm_buffer_free(tal_rx_buffer[trx_id]);
    tal_rx_buffer[trx_id] = NULL;
    tal_buf_shortage[trx_id] = false;

#ifdef ENABLE_FTN_PLL_CALIBRATION
    stop_ftn_timer(trx_id);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */

    return MAC_SUCCESS;
}


/**
 * @brief Wakes up the transceiver from SLEEP
 *
 * This function awakes the transceiver from state SLEEP.
 *
 * @param trx_id Transceiver identifier
 *
 * @return
 *      - @ref TAL_TRX_AWAKE - The transceiver is already awake
 *      - @ref MAC_SUCCESS - The transceiver is woken up from sleep
 *      - @ref FAILURE - The transceiver did not wake-up from sleep
 * @ingroup apiTalApi
 */
retval_t tal_trx_wakeup(trx_id_t trx_id)
{
    debug_text(PSTR("tal_trx_wakeup()"));

    if (tal_state[trx_id] != TAL_SLEEP)
    {
        debug_text(PSTR("trx already awake"));
        return TAL_TRX_AWAKE;
    }

    tal_state[trx_id] = TAL_WAKING_UP;

    /* Check if other trx is sleep too. */
    uint8_t other_trx_id;
    if (trx_id == RF09)
    {
        other_trx_id = RF24;
    }
    else
    {
        other_trx_id = RF09;
    }
    if (tal_state[other_trx_id] == TAL_SLEEP)
    {
        tal_state[other_trx_id] = TAL_WAKING_UP;
    }

    /* Write command to wake device up */
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    trx_reg_write(reg_offset + RG_RF09_CMD, RF_TRXOFF);
#ifdef IQ_RADIO
    trx_reg_write(RF215_RF, reg_offset + RG_RF09_CMD, RF_TRXOFF);
#endif
    trx_state[trx_id] = RF_TRXOFF;

    /* Wait for transceiver wakeup */
    uint32_t start_time;
    uint32_t current_time;
    pal_get_current_time(&start_time);
    pal_get_current_time(&current_time);
    while (1)
    {
        if (TAL_RF_IS_IRQ_SET(trx_id, RF_IRQ_WAKEUP))
        {
            debug_text_val(PSTR("Clear RF_IRQ_WAKEUP for trx_id = "), trx_id);
            TAL_RF_IRQ_CLR(trx_id, RF_IRQ_WAKEUP);

            if (tal_state[other_trx_id] == TAL_WAKING_UP)
            {
                /* Wait for the other trx to wake-up as well. */
                if (TAL_RF_IS_IRQ_SET(other_trx_id, RF_IRQ_WAKEUP))
                {
                    debug_text(PSTR("Wake up of other device"));
                    debug_text_val(PSTR("Clear RF_IRQ_WAKEUP for trx_id = "),
                                   trx_id);
                    TAL_RF_IRQ_CLR(other_trx_id, RF_IRQ_WAKEUP);
                    break;
                }
            }
            else
            {
                break;
            }
        }
        pal_get_current_time(&current_time);
        // @ToDo: Use no magic number for "1000"
        if (pal_sub_time_us(current_time, start_time) > 1000)
        {
            debug_text_val(PSTR("long start up duration = "),
                           (uint16_t)(current_time - start_time));
            tal_state[trx_id] = TAL_SLEEP;
            if (tal_state[other_trx_id] == TAL_WAKING_UP)
            {
                tal_state[other_trx_id] = TAL_SLEEP;
            }
            debug_text(PSTR("Error: Trx did not wake up"));
            return FAILURE;
        }
    }

    debug_text_val(PSTR("start up duration = "),
                   (uint16_t)(current_time - start_time));

    /*
     * If the other transceiver block was in SLEEP mode, enable SLEEP for that
     * block again.
     */
    if (tal_state[other_trx_id] == TAL_WAKING_UP)
    {
        uint16_t other_reg_offset = RF_BASE_ADDR_OFFSET * other_trx_id;
        debug_text_val(PSTR("Set other trx back to sleep, other_trx_id ="),
                       other_trx_id);
        trx_reg_write(other_reg_offset + RG_RF09_CMD, RF_SLEEP);
#ifdef IQ_RADIO
        trx_reg_write(RF215_RF, other_reg_offset + RG_RF09_CMD, RF_SLEEP);
#endif
        tal_state[other_trx_id] = TAL_SLEEP;
        TAL_RF_IRQ_CLR_ALL(other_trx_id);
        TAL_RF_IRQ_CLR_ALL(other_trx_id);
    }

    trx_config(trx_id); /* see tal_init.c */
    write_all_tal_pib_to_trx(trx_id); /* see 'tal_pib.c' */
    config_phy(trx_id);

    /*
     * Allocate a new buffer
     * The previous buffer got freed with entering sleep mode.
     */
    tal_rx_buffer[trx_id] = bmm_buffer_alloc(LARGE_BUFFER_SIZE);

    tal_state[trx_id] = TAL_IDLE;

    debug_text_val(PSTR("tal_trx_wakeup done"), trx_id);

    return MAC_SUCCESS;
}

/* EOF */

