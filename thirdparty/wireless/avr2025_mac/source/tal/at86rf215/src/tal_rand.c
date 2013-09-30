/**
 * @file tal_rand.c
 *
 * @brief This file implements the random seed function.
 *
 * $Id: tal_rand.c 35124 2013-09-19 11:22:42Z uwalter $
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

    //debug_text(PSTR("tal_generate_rand_seed()"));

    /* Check for non sleeping device */
    if ((tal_state[RF09] == TAL_SLEEP) && (tal_state[RF24] == TAL_SLEEP))
    {
        //debug_text(PSTR("no seed generated - TAL_SLEEP"));
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
            //debug_text(PSTR("no seed generated - TAL_BUSY"));
            return TAL_BUSY;
        }
    }

    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    /* Set widest filter bandwidth */
    uint8_t previous_bw = pal_trx_bit_read(rf_reg_offset + SR_RF09_RXBWC_BW);
    pal_trx_bit_write(rf_reg_offset + SR_RF09_RXBWC_BW, 0x0B);

    /* Ensure that transceiver is not in off mode */
    if (trx_state[trx_id] == RF_TRXOFF)
    {
        previous_trx_state = RF_TRXOFF;
        uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;

        /* Disable BB */
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_BBEN, 0);

        /* Enter TXPREP state */
        ENTER_TRX_REGION();
        pal_trx_bit_write(rf_reg_offset + SR_RF09_CMD_CMD, RF_TXPREP);
        rf_cmd_state_t temp_state = RF_NOP;
        while (1)
        {
            if (IRQ_PINGET() == HIGH)
            {
				
                break;
            }
        }
        do
        {
            temp_state = (rf_cmd_state_t)pal_trx_bit_read(
                             rf_reg_offset + SR_RF09_STATE_STATE);
        }
        while (temp_state != RF_TXPREP);
        /* Clear trx irqs */
        trx_irq_handler_cb();
        TAL_RF_IRQ_CLR(trx_id, RF_IRQ_TRXRDY);

        LEAVE_TRX_REGION();

        /*
         * Enter Rx state
         * Use direct register access to change to Rx state, since no buffer is
         * required.
         */
        pal_trx_bit_write(rf_reg_offset + SR_RF09_CMD_CMD, RF_RX);
        trx_state[trx_id] = RF_RX;
    }
    else
    {
        //debug_text(PSTR("Trx is NOT off"));
    }

    uint16_t seed;
#ifdef AT86RF215LT
    seed = pal_trx_reg_read(rf_reg_offset + RG_RF09_RNDV); // dummy read
#endif
    seed = pal_trx_reg_read(rf_reg_offset + RG_RF09_RNDV);
    seed |= (uint16_t)pal_trx_reg_read(rf_reg_offset + RG_RF09_RNDV) << 8;

    /* Restore previous transceiver state */
    if (previous_trx_state == RF_TRXOFF)
    {
        pal_trx_bit_write(rf_reg_offset + SR_RF09_CMD_CMD, RF_TRXOFF);
        trx_state[trx_id] = RF_TRXOFF;
        /* Enable BB again */
        uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_BBEN, 1);
    }

    /* Restore previous filter bandwidth setting */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_RXBWC_BW, previous_bw);

    /* Set the seed for the random number generator. */
    srand(seed);

    return MAC_SUCCESS;
}


/* EOF */
