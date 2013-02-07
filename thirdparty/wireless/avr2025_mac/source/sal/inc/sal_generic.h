/**
 * @file sal_generic.h
 *
 * @brief Declarations for low-level security API
 *
 * This file contains declarations for the low-level security
 * API.
 *
 * $Id: sal_generic.h 32877 2012-08-30 08:02:51Z agasthian.s $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef SAL_GENERIC_H
#define SAL_GENERIC_H

/* === Includes =========================================================== */


/* === Macros ============================================================= */

/**
 * Size of AES blocks
 */
#define AES_BLOCKSIZE               (16)

/**
 * Size of AES key
 */
#define AES_KEYSIZE                 (16)



/* === Types ============================================================== */


/* === Externals ========================================================== */


/* === Prototypes ========================================================= */

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SAL_GENERIC_H */
/* EOF */
