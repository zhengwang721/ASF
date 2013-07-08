/**
 * \file tal_tx.h
 *
 * \brief File contains the TAL sub-state macros and functions that
 * perform frame transmission.
 *
 * $Id: tal_tx.h 32840 2012-08-27 08:35:55Z sriram.r $
 *
 * \author    Atmel Corporation: http://www.atmel.com
 * \author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef TAL_TX_H
#define TAL_TX_H


/* === INCLUDES ============================================================ */

#include "tal.h"

/* === EXTERNALS =========================================================== */


/* === TYPES =============================================================== */


/* === MACROS ============================================================== */


/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \addtogroup group_tal_tx
     * @{
     */

    /**
     * \brief Handles interrupts issued due to end of transmission
     *
     * \param underrun_occured  true if under-run has occurred
     */

    void handle_tx_end_irq(bool underrun_occured);

    /**
     * \brief Sends frame using trx features to handle CSMA and re-transmissions
     *
     * \param csma_mode Indicates the CSMA mode used
     * \param tx_retries Flag indicating if transmission retries are requested
     *                   by the MAC layer
     */

    void send_frame(csma_mode_t csma_mode, bool tx_retries);

    /**
     * \brief Implements the handling of the transmission end.
     *
     * This function handles the callback for the transmission end.
     */
    void tx_done_handling(void);

    //! @}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TAL_TX_H */

/* EOF */

