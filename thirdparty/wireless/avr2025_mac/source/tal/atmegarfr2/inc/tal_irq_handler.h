/**
 * \file tal_irq_handler.h
 *
 * \brief This header file contains the interrupt handling definitions
 *
 * $Id: tal_irq_handler.h 32840 2012-08-27 08:35:55Z sriram.r $
 *
 * \author    Atmel Corporation: http://www.atmel.com
 * \author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2011, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef TAL_IRQ_HANDLER_H
#define TAL_IRQ_HANDLER_H

/* === INCLUDES ============================================================ */


/* === EXTERNALS =========================================================== */


/* === TYPES =============================================================== */


/* === MACROS ============================================================== */


/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \addtogroup group_tal_irq
     * @{
     */

    /**
     * \brief Transceiver interrupt handler
     *
     * This function handles the transceiver generated interrupts for RX end.
     */

    void trx_rx_end_handler_cb(void);
    /**
     * \brief Transceiver interrupt handler
     *
     * This function handles the transceiver generated interrupts for TX end.
     */

    void trx_tx_end_handler_cb(void);
    /*
     * \brief Transceiver interrupt handler for awake end IRQ
     *
     * This function handles the transceiver awake end interrupt.
     */
    void trx_awake_handler_cb(void);

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || (defined DOXYGEN)
    /**
     * \brief Timestamp interrupt handler
     *
     * This function handles the interrupts handling the timestamp.
     *
     * The timestamping is only required for
     * - beaconing networks or if timestamping is explicitly enabled,
     * - and if antenna diversity is NOT enabled,
     * - and if the time stamp interrupt is not explicitly disabled.
     */
    void trx_irq_timestamp_handler_cb(void);
#endif

    //! @}
#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* TAL_IRQ_HANDLER_H */

/* EOF */
