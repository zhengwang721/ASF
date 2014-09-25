/**
 * @file tal.c
 *
 * @brief This file implements the TAL state machine and provides general
 * functionality used by the TAL.
 *
 * $Id: tal.c 36382 2014-08-26 11:56:01Z uwalter $
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
#include "ieee_const.h"
#include "tal_pib.h"
#include "tal_config.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_internal.h"
#include "mac_build_config.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/*
 * Time gap between each poll access in microseconds.
 * If the value is equal to zero, no time gap is applied.
 */
#define POLL_TIME_GAP               10

/* Maximum PLL lock duration in us */
#define MAX_PLL_LOCK_DURATION       200

/* Maximum settling duration after PLL has been freezed */
#define PLL_FRZ_SETTLING_DURATION   20

/* === GLOBALS ============================================================= */

/*
 * Global TAL variables
 * These variables are only to be used by the TAL internally.
 */

/**
 * TAL PIBs
 */
tal_pib_t tal_pib[2];

/**
 * Current state of the TAL state machine.
 */
tal_state_t tal_state[2];

/**
 * Current state of the TX state machine.
 */
tx_state_t tx_state[2];

/**
 * Indicates if a buffer shortage issue needs to handled by tal_task().
 */
bool tal_buf_shortage[2];

/**
 * Pointer to the 15.4 frame created by the TAL to be handed over
 * to the transceiver.
 */
uint8_t *tal_frame_to_tx[2];

/**
 * Pointer to receive buffer that can be used to upload a frame from the trx.
 */
buffer_t *tal_rx_buffer[2] = {NULL, NULL};

/**
 * Queue that contains all frames that are uploaded from the trx, but have not
 * be processed by the MCL yet.
 */
queue_t tal_incoming_frame_queue[2];

/**
 * Frame pointer for the frame structure provided by the MCL.
 */
frame_info_t *mac_frame_ptr[2];

/**
 * Shadow variable for BB IRQS; variable is filled during TRX ISR,
 * see trx_irq_handler_cb() in tal_irq_handler.c
 */
volatile bb_irq_t tal_bb_irqs[2] = {BB_IRQ_NO_IRQ, BB_IRQ_NO_IRQ};

/**
 * Shadow variable for RF IRQS; variable is filled during TRX ISR,
 * see trx_irq_handler_cb() in tal_irq_handler.c
 */
volatile rf_irq_t tal_rf_irqs[2] = {RF_IRQ_NO_IRQ, RF_IRQ_NO_IRQ};

/**
 * Last retrieved energy value;
 * variable is filled by handle_ed_end_irq() in tal_ed.c
 */
int8_t tal_current_ed_val[2];

/** Parameter to handle timer callback functions */
const uint8_t timer_cb_parameter[2] = {RF09, RF24};

/** Current trx state */
rf_cmd_state_t trx_state[2];

/** Default/Previous trx state while entering a transaction */
rf_cmd_state_t trx_default_state[2];


#if (defined ENABLE_TSTAMP) || (defined MEASURE_ON_AIR_DURATION)
/**
 * Frame start time stamp
 * During Tx transaction it stores the time when the frame is actually send,
 * i.e. frame start.
 * During Rx transaction it stores the time when the frame is actually received,
 * i.e. frame end - RXFE.
 */
uint32_t fs_tstamp[2];
#endif  /* #ifdef ENABLE_TSTAMP */

/**
 * Timestamp for end of reception, i.e. RXFE IRQ;
 * used during Rx transaction to calculate ACK transmission time.
 * Timestamp for end of transmission;
 * used during Tx transaction to calculate ACK timeout.
 * Both scenarios use the same variable
 */
uint32_t rxe_txe_tstamp[2];

/**
 * TX calibration values
 */
uint8_t txc[2][2];

/* === PROTOTYPES ========================================================== */

static void handle_trxerr(trx_id_t trx_id);

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief TAL task handling
 *
 * This function
 * - handles any pending IRQ flags
 * - Processes the TAL incoming frame queue.
 * The function needs to be called on a regular basis.
 */
void tal_task(void)
{
    for (uint8_t trx_id = 0; trx_id < 2; trx_id++)
    {
        if (tal_state[trx_id] == TAL_SLEEP)
        {
            continue;
        }

        /*
         * Handle buffer shortage.
         * Check if the receiver needs to be switched on.
         */
        if (tal_buf_shortage[trx_id])
        {
            debug_text_val(PSTR("tal_task() - Handle tal_buf_shortage"),
                           trx_id);

            /* If necessary, try to allocate a new buffer. */
            if (tal_rx_buffer[trx_id] == NULL)
            {
                tal_rx_buffer[trx_id] = bmm_buffer_alloc(LARGE_BUFFER_SIZE);
            }

            /* Check if buffer could be allocated */
            if (tal_rx_buffer[trx_id] != NULL)
            {
                debug_text(PSTR("tal_task() - Buffer shortage resolved"));
                tal_buf_shortage[trx_id] = false;
                if (tal_state[trx_id] == TAL_IDLE)
                {
                    switch_to_rx((trx_id_t)trx_id);
                }
                else
                {
                    /*
                     * If TAL is currently not IDLE, it will recover once it
                     * gets IDLE again.
                     */
                }
            }
            else
            {
                debug_text(PSTR("tal_task() - Buffer shortage pending"));
            }
        }

        /*
         * If the transceiver has received a frame and it has been placed
         * into the queue of the TAL, the frame needs to be processed further.
         */
        if (tal_incoming_frame_queue[trx_id].size > 0)
        {
            buffer_t *rx_frame;

            /* Check if there are any pending data in the incoming_frame_queue. */
            rx_frame = qmm_queue_remove(&tal_incoming_frame_queue[trx_id], NULL);
            if (rx_frame != NULL)
            {
                process_incoming_frame((trx_id_t)trx_id, rx_frame);
            }
        }

        /*
         * Handle pending interrupt events
         * that have not been processed within the ISR
         */
        bb_irq_t bb_irqs;
        rf_irq_t rf_irqs;
        ENTER_TRX_REGION();
        bb_irqs = tal_bb_irqs[trx_id];
        rf_irqs = tal_rf_irqs[trx_id];
        LEAVE_TRX_REGION();

        if (bb_irqs != BB_IRQ_NO_IRQ)
        {
            if (bb_irqs & BB_IRQ_RXEM)
            {
                debug_text(PSTR("tal_task - BB_IRQ_RXEM"));
                TAL_BB_IRQ_CLR(trx_id, BB_IRQ_RXEM);
            }
            if (bb_irqs & BB_IRQ_RXAM)
            {
                debug_text(PSTR("tal_task - BB_IRQ_RXAM"));
                TAL_BB_IRQ_CLR(trx_id, BB_IRQ_RXAM);
            }
            if (bb_irqs & BB_IRQ_RXFS)
            {
                debug_text(PSTR("tal_task - BB_IRQ_RXFS"));
                TAL_BB_IRQ_CLR(trx_id, BB_IRQ_RXFS);
            }
            if (bb_irqs & BB_IRQ_TXFE)
            {
                debug_text(PSTR("tal_task - BB_IRQ_TXFE"));
                TAL_BB_IRQ_CLR(trx_id, BB_IRQ_TXFE);
#ifndef BASIC_MODE
                if (tx_state[trx_id] == TX_CCATX)
                {
                    /* Clear TRXRDY IRQ that has been issued while switching from Rx to TX via TXPREP */
                    /* Clear EDC IRQ that has been issued for CCA measurement */
                    TAL_RF_IRQ_CLR(trx_id, (RF_IRQ_EDC | RF_IRQ_TRXRDY));
                    rf_irqs &= (uint8_t)(~((uint32_t)(RF_IRQ_EDC | RF_IRQ_TRXRDY)));
                }
#endif
                handle_tx_end_irq((trx_id_t)trx_id);
            }
            if (bb_irqs & BB_IRQ_RXFE)
            {
                debug_text(PSTR("tal_task - BB_IRQ_RXFE"));
                TAL_BB_IRQ_CLR(trx_id, BB_IRQ_RXFE);
                handle_rx_end_irq((trx_id_t)trx_id);
            }
        }

        /* Handle RF interrupts */
        if (rf_irqs != RF_IRQ_NO_IRQ)
        {
            if (rf_irqs & RF_IRQ_TRXRDY)
            {
                debug_text(PSTR("tal_task - RF_IRQ_TRXRDY"));
                debug_text_val(PSTR("Error: unexpected IRQ, tal_state = "),
                               tal_state[trx_id]);
            }
            if (rf_irqs & RF_IRQ_TRXERR)
            {
                debug_text(PSTR("tal_task - RF_IRQ_TRXERR"));
                TAL_RF_IRQ_CLR(trx_id, RF_IRQ_TRXERR);
                handle_trxerr((trx_id_t)trx_id);
            }
            if (rf_irqs & RF_IRQ_BATLOW)
            {
                debug_text(PSTR("tal_task - RF_IRQ_BATLOW"));
                TAL_RF_IRQ_CLR(trx_id, RF_IRQ_BATLOW);
#if (defined ENABLE_TFA) || (defined TFA_BAT_MON_IRQ)
                handle_batmon_irq(); // see tfa_batmon.c
#endif
            }
            if (rf_irqs & RF_IRQ_WAKEUP)
            {
                debug_text(PSTR("tal_task - RF_IRQ_WAKEUP"));
                TAL_RF_IRQ_CLR(trx_id, RF_IRQ_WAKEUP);
                debug_text_val(PSTR("tal_state = "), tal_state[trx_id]);
            }
            if (rf_irqs & RF_IRQ_IQIFSF)
            {
                debug_text(PSTR("tal_task - RF_IRQ_IQIFSF"));
                TAL_RF_IRQ_CLR(trx_id, RF_IRQ_IQIFSF);
            }
            if (rf_irqs & RF_IRQ_EDC)
            {
                debug_text(PSTR("tal_task - RF_IRQ_EDC"));
                TAL_RF_IRQ_CLR(trx_id, RF_IRQ_EDC);
                handle_ed_end_irq((trx_id_t)trx_id);
            }
        }
    }
} /* tal_task() */


/**
 * @brief Switches the transceiver to Rx
 *
 * This function tries to switch the transceiver to Rx. It checks if a Rx buffer
 * is available. Only if a buffer is available, the receiver is actually
 * switched on. If no buffer is available, the tal_buf_shortage flag is set.
 * This buffer shortage issue is tried to solve within tal_task() again.
 *
 * @param trx_id Transceiver identifier
 */
void switch_to_rx(trx_id_t trx_id)
{
    debug_text_val(PSTR("switch_to_rx(), trx_id ="), trx_id);

    /* Check if buffer is available now. */
    if (tal_rx_buffer[trx_id] != NULL)
    {
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_RX);
        trx_state[trx_id] = RF_RX;
    }
    else
    {
        switch_to_txprep(trx_id);
        tal_buf_shortage[trx_id] = true;
        debug_text_val(PSTR("Warning: buffer shortage !!!!!!!!!"), trx_id);
    }
}


/**
 * @brief Switches the transceiver to TxPREP
 *
 * This function switches the transceiver to TxPREP and waits until it has
 * has reached this state. Do not call this function within an ISR.
 *
 * @param trx_id Transceiver identifier
 */
void switch_to_txprep(trx_id_t trx_id)
{
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    debug_text_val(PSTR("switch_to_txprep(), trx_id ="), trx_id);

    pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_TXPREP);

    wait_for_txprep(trx_id);

    /* Workaround for errata reference #4807 */
    pal_trx_write(reg_offset + 0x125, (uint8_t *)&txc[trx_id][0], 2);
}


/**
 * @brief Wait to reach TXPREP
 *
 * @param trx_id Transceiver identifier
 */
void wait_for_txprep(trx_id_t trx_id)
{
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    rf_cmd_state_t state;
    uint32_t start_time = 0;

    pal_get_current_time(&start_time);

    do
    {
#if (POLL_TIME_GAP > 0)
        pal_timer_delay(POLL_TIME_GAP); /* Delay to reduce SPI storm */
#endif
        state = (rf_cmd_state_t)pal_trx_reg_read(reg_offset + RG_RF09_STATE);

        if (state != RF_TXPREP)
        {
            /* Workaround for errata reference #4810 */
            uint32_t now;
            pal_get_current_time(&now);
            if (abs(now - start_time) > MAX_PLL_LOCK_DURATION)
            {
                pal_trx_reg_write(reg_offset + RG_RF09_PLL, 9);
                pal_timer_delay(PLL_FRZ_SETTLING_DURATION);
                pal_trx_reg_write(reg_offset + RG_RF09_PLL, 8);
                do
                {
                    state = (rf_cmd_state_t)pal_trx_reg_read(reg_offset + RG_RF09_STATE);
                }
                while (state != RF_TXPREP);
                break;
            }
        }
    }
    while (state != RF_TXPREP);

    trx_state[trx_id] = RF_TXPREP;
}


/**
 * @brief Handles a transceiver error
 *
 * This function handles a TRXERR interrupt.
 *
 * @param trx_id Transceiver identifier
 */
static void handle_trxerr(trx_id_t trx_id)
{
    debug_text(PSTR("handle_trxerr()"));
    debug_text_val(PSTR("tal_state = "), tal_state[trx_id]);

    /* Set device to TRXOFF */
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_TRXOFF);
    trx_state[trx_id] = RF_TRXOFF;
    tx_state[trx_id] = TX_IDLE;
    stop_tal_timer(trx_id);
#ifdef ENABLE_FTN_PLL_CALIBRATION
    stop_ftn_timer(trx_id);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */

    switch (tal_state[trx_id])
    {
        case TAL_TX:
            tx_done_handling(trx_id, FAILURE);
            break;

#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1)
        case TAL_ED_SCAN:
            stop_ed_scan(trx_id); // see tal_ed.c; covers state handling
            break;
#endif

        default:
            if (trx_default_state[trx_id] == RF_RX)
            {
                tal_rx_enable(trx_id, PHY_RX_ON);
            }
            tal_state[trx_id] = TAL_IDLE;
            break;
    }
}


/**
 * @brief Stops TAL timer
 *
 * @param trx_id Transceiver identifier
 */
void stop_tal_timer(trx_id_t trx_id)
{
#if (TAL_FIRST_TIMER_ID == 0)
    pal_timer_stop(trx_id);
#else
    uint8_t timer_id;
    if (trx_id == RF09)
    {
        timer_id = TAL_T_0;
    }
    else
    {
        timer_id = TAL_T_1;
    }
    pal_timer_stop(timer_id);
#endif
}

/* EOF */
