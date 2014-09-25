/**
 * @file tal_rand.c
 *
 * @brief This file implements the random seed function.
 *
 * $Id: tal_rand.c 36327 2014-08-14 07:15:23Z uwalter $
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
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "tal_config.h"
#include "tal_internal.h"

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Generates a 16-bit random number used as initial seed for srand()
 *
 * This function generates a 16-bit random number by means of using the
 * Random Number Generator from the transceiver.
 *
 * The generated random 16-bit number is feed into function srand()
 * as initial seed.
 */
retval_t tal_generate_rand_seed(void)
{
    trx_id_t trx_id;
    rf_cmd_state_t previous_trx_state = RF_NOP;

    debug_text(PSTR("tal_generate_rand_seed()"));

    /* Check for non sleeping device */
    if ((tal_state[RF09] == TAL_SLEEP) && (tal_state[RF24] == TAL_SLEEP))
    {
        debug_text(PSTR("no seed generated - TAL_SLEEP"));
        return TAL_TRX_ASLEEP;
    }

    /* Find a trx that is idle */
    if (tal_state[RF09] == TAL_IDLE)
    {
        trx_id = RF09;
    }
    else
    {
        if (tal_state[RF24] == TAL_IDLE)
        {
            trx_id = RF24;
        }
        else
        {
            debug_text(PSTR("no seed generated - TAL_BUSY"));
            return TAL_BUSY;
        }
    }

    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    /* Set widest filter bandwidth and set IF shift */
    uint8_t previous_bwc;
#ifdef IQ_RADIO
    previous_bwc = pal_trx_reg_read(RF215_RF, reg_offset + RG_RF09_RXBWC);
    pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_RXBWC, 0x1B);
#else
    previous_bwc = pal_trx_reg_read(reg_offset + RG_RF09_RXBWC);
    pal_trx_reg_write(reg_offset + RG_RF09_RXBWC, 0x1B);
#endif

    /* Ensure that transceiver is not in off mode */
    if (trx_state[trx_id] == RF_TRXOFF)
    {
        previous_trx_state = RF_TRXOFF;

        /* Disable BB to avoid receiving any frame */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_BBEN, 0);

        switch_to_txprep(trx_id);

        /*
         * Enter Rx state
         * Use direct register access to change to Rx state, since no buffer is
         * required.
         */
        pal_trx_bit_write(reg_offset + SR_RF09_CMD_CMD, RF_RX);
#ifdef IQ_RADIO
        pal_trx_bit_write(RF215_RF, reg_offset + SR_RF09_CMD_CMD, RF_RX);
#endif
        trx_state[trx_id] = RF_RX;

        pal_timer_delay(10); /* Allow frontend to settle */
    }
    else
    {
        debug_text(PSTR("Trx is NOT off"));
    }

    uint16_t seed;
    for (uint8_t i = 0; i < 2; i++)
    {
        seed = pal_trx_reg_read(reg_offset + RG_RF09_RNDV);
        seed |= (uint16_t)pal_trx_reg_read(reg_offset + RG_RF09_RNDV) << 8;
    }

    /* Restore previous transceiver state */
    if (previous_trx_state == RF_TRXOFF)
    {
#ifdef IQ_RADIO
        pal_trx_bit_write(RF215_BB, reg_offset + SR_RF09_CMD_CMD, RF_TRXOFF);
        pal_trx_bit_write(RF215_RF, reg_offset + SR_RF09_CMD_CMD, RF_TRXOFF);
        trx_state[trx_id] = RF_TRXOFF;
        /* Enable BB again */
        pal_trx_bit_write(RF215_BB, reg_offset + SR_BBC0_PC_BBEN, 1);
#else
        pal_trx_bit_write(reg_offset + SR_RF09_CMD_CMD, RF_TRXOFF);
        trx_state[trx_id] = RF_TRXOFF;
        /* Enable BB again */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_BBEN, 1);
#endif
    }

    /* Restore previous filter bandwidth setting */
#ifdef IQ_RADIO
    pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_RXBWC, previous_bwc);
#else
    pal_trx_reg_write(reg_offset + RG_RF09_RXBWC, previous_bwc);
#endif

    /* Set the seed for the random number generator. */
    srand(seed);

    return MAC_SUCCESS;
}


/* EOF */
