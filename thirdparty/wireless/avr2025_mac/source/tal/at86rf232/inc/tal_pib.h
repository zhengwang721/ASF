/**
 * \file tal_pib.h
 *
 * \brief This file contains the prototypes for TAL PIB functions.
 *
 * $Id: tal_pib.h 32840 2012-08-27 08:35:55Z sriram.r $
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
#ifndef TAL_PIB_H
#define TAL_PIB_H

/* === INCLUDES ============================================================ */


/* === EXTERNALS =========================================================== */


/* === TYPES =============================================================== */


/* === MACROS ============================================================== */


/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \addtogroup group_tal_pib
     * @{
     */

    /**
     * \brief Initialize the TAL PIB
     *
     * This function initializes the TAL information base attributes
     * to their default values.
     */
    void init_tal_pib(void);

    /**
     * \brief Write all shadow PIB variables to the transceiver
     *
     * This function writes all shadow PIB variables to the transceiver.
     * It is assumed that the radio does not sleep.
     */
    void write_all_tal_pib_to_trx(void);

    //! @}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* TAL_PIB_H */

/* EOF */
