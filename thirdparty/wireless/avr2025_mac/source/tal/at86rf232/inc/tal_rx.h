/**
 * \file tal_rx.h
 *
 * \brief File contains macros and modules used while processing
 * a received frame.
 *
 * $Id: tal_rx.h 32840 2012-08-27 08:35:55Z sriram.r $
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
#ifndef TAL_RX_H
#define TAL_RX_H

/* === INCLUDES ============================================================ */


/* === EXTERNALS =========================================================== */


/* === TYPES =============================================================== */


/* === MACROS ============================================================== */


/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \addtogroup group_tal_rx
     * @{
     */

    /**
     * \brief Handle received frame interrupt
     *
     * This function handles transceiver interrupts for received frames and
     * uploads the frames from the trx.
     */

    void handle_received_frame_irq(void);

    /**
     * \brief Parses received frame and create the frame_info_t structure
     *
     * This function parses the received frame and creates the frame_info_t
     * structure to be sent to the MAC as a parameter of tal_rx_frame_cb().
     *
     * \param buf Pointer to the buffer containing the received frame
     */
    void process_incoming_frame(buffer_t *buf);

    //! @}
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TAL_RX_H */
