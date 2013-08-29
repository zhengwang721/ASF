/**
 * @file tal_pib.h
 *
 * @brief This file contains the prototypes for TAL PIB functions.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 */

/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
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

#define MAX_TX_PWR                  (11)
#define MIN_TX_PWR                  (-25)
#define MAX_TX_PWR_CHINA_CH_1_2     (MAX_TX_PWR)
#define MAX_TX_PWR_CHINA_CH_0_3     (4)


/* \todo use one max power for each band (EU, NA, China) */
/* max power of standard compliant modes */
#define MAX_TX_PWR_BPSK_40          MAX_TX_PWR  /* NA, band: 915MHz */
#define MAX_TX_PWR_OQPSK_SIN_250    MAX_TX_PWR  /* NA, band: 915MHz */
#define MAX_TX_PWR_BPSK_20          (MAX_TX_PWR)         /* EU, band: 868MHz */
#define DEFAULT_TX_PWR_BPSK_20          (6)         /* EU, band: 868MHz */
#define MAX_TX_PWR_OQPSK_SIN_RC_100 (MAX_TX_PWR)         /* EU, band: 868MHz */
#define MAX_TX_PWR_OQPSK_RC_250     (10)        /* China, band: 780MHz */


/* max power of proprietary modes */
#ifdef HIGH_DATA_RATE_SUPPORT
#define MAX_TX_PWR_OQPSK_SIN_500    MAX_TX_PWR  /* NA, band: 915MHz */
#define MAX_TX_PWR_OQPSK_SIN_1000   MAX_TX_PWR  /* NA, band: 915MHz */
#define MAX_TX_PWR_OQPSK_SIN_RC_200 (MAX_TX_PWR)         /* EU, band: 868MHz */
#define MAX_TX_PWR_OQPSK_SIN_RC_400 (MAX_TX_PWR)         /* EU, band: 868MHz */
#define DEFAULT_TX_PWR_OQPSK_SIN_RC_100_200_400 (3)         /* EU, band: 868MHz */
#define MAX_TX_PWR_OQPSK_RC_500     (10)        /* China, band: 780MHz */
#define MAX_TX_PWR_OQPSK_RC_1000    (MAX_TX_PWR)        /* China, band: 780MHz */


/* old stuff
#define MAX_TX_PWR_OQPSK_200        MAX_TX_PWR_OQPSK_100
#define MAX_TX_PWR_OQPSK_400        MAX_TX_PWR_OQPSK_100
#define MAX_TX_PWR_OQPSK_SIN_500    (7)
#define MAX_TX_PWR_OQPSK_SIN_1000   (4)
*/
#endif /* #ifdef HIGH_DATA_RATE_SUPPORT */
/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup group_tal_pib_212b
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

/* ! @} */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TAL_PIB_H */

/* EOF */
