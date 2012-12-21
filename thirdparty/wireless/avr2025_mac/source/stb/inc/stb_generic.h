/**
 * @file stb_generic.h
 *
 * @brief Declarations for security tool box
 *
 * This file contains declarations for the high-level security tool box API.
 *
 * $Id: stb_generic.h 32877 2012-08-30 08:02:51Z agasthian.s $
 *
 */
/**
 *  @author
 *      Atmel Corporation: http://www.atmel.com
 *      Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef STB_GENERIC_H
#define STB_GENERIC_H

#if defined(STB_ON_SAL) || defined(STB_ARMCRYPTO) || defined(DOXYGEN)

/* === Includes =========================================================== */
#include "compiler.h"
#include "sal_generic.h"

/* === Macros ============================================================= */




/* === Types ============================================================== */

/**
 * CCM status values
 */
typedef enum
{
    /** CCM en/decryption OK */
    STB_CCM_OK = 0,
    /** Illegal parameter value in stb_ccm_secure() */
    STB_CCM_ILLPARM,
    /** No previous key init in stb_ccm_secure() */
    STB_CCM_KEYMISS,
    /** MIC error detected in stb_ccm_secure() */
    STB_CCM_MICERR
} SHORTENUM stb_ccm_t;

/* === Externals ========================================================== */


/* === Prototypes ========================================================= */

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief STB Initialization
     *
     * This function initializes the STB.
     *
     * @ingroup apiStbApi
     */
    void stb_init(void);

    /**
     * @brief Secure one block with CCM*
     *
     * This functions secures one block with CCM* according to 802.15.4.
     *
     * @param[in,out] buffer Input: plaintext header and payload concatenated;
     *                       for encryption: MUST HAVE 'AES_BLOCKSIZE'
     *                       BYTES SPACE AT THE END FOR THE MIC!
     *                       Output: frame secured (with MIC at end)/unsecured
     * @param[in]  nonce   The nonce: Initialization Vector (IV) as used in
     *                     cryptography; the ZigBee nonce (13 bytes long)
     *                     are the bytes 2...14 of this nonce
     * @param[in] key The key to be used; if NULL, use the current key
     * @param[in] hdr_len Length of plaintext header (will not be encrypted)
     * @param[in] pld_len Length of payload to be encrypted; if 0, then only MIC
     *                    authentication implies
     * @param[in] sec_level Security level according to IEEE 802.15.4,
     *                    7.6.2.2.1, Table 95:
     *                    - the value may be 0 ... 7;
     *                    - the two LSBs contain the MIC length in bytes
     *                      (0, 4, 8 or 16);
     *                    - bit 2 indicates whether encryption applies or not
     * @param[in] aes_dir AES_DIR_ENCRYPT if secure, AES_DIR_DECRYPT if unsecure
     *
     * @return STB CCM Status
     *
     * @ingroup apiStbApi
     */
    stb_ccm_t stb_ccm_secure(uint8_t *buffer,
                             uint8_t nonce[AES_BLOCKSIZE],
                             uint8_t *key,
                             uint8_t hdr_len,
                             uint8_t pld_len,
                             uint8_t sec_level,
                             uint8_t aes_dir);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* #if defined(STB_ON_SAL) || defined(STB_ARMCRYPTO) || defined(DOXYGEN) */

#endif /* STB_GENERIC_H */
/* EOF */
