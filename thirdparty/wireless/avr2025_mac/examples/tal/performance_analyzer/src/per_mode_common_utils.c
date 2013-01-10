/**
 * \file per_mode_common_utils.c
 *
 * \brief  Common utilities for both Initiator and Receptor in PER Measurement
 * mode - Performance Analyzer application
 *
 * Copyright (c) 2009 Atmel Corporation. All rights reserved.
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
 * Copyright(c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */
#include <stdio.h>
#include "tal.h"
#include "tal_internal.h"
#include "tal_helper.h"
#include "ieee_const.h"
#include "app_per_mode.h"
#include "app_init.h"
/* === TYPES =============================================================== */

/* === GLOBALS============================================================== */

/* === IMPLEMENTATION======================================================= */

/* === IMPLEMENTATION======================================================= */
/*
 * \brief This function is called to get the base RSSI value for repective radios
 *
 * \return value of the base RSSI value
 */
int8_t get_rssi_base_val(void)
{
#if (TAL_TYPE == AT86RF212B)
    switch (tal_pib.CurrentPage)
    {
        case 0: /* BPSK */
            if (tal_pib.CurrentChannel == 0)
            {
                return(RSSI_BASE_VAL_BPSK_300_DBM);
            }
            else
            {
                return(RSSI_BASE_VAL_BPSK_300_DBM);
            }

        case 2: /* O-QPSK */
            if (tal_pib.CurrentChannel == 0)
            {
                return(RSSI_BASE_VAL_OQPSK_400_SIN_RC_DBM);
            }
            else
            {
                return(RSSI_BASE_VAL_OQPSK_400_SIN_RC_DBM);
            }

        case 5: /* Chinese band */
        default:    /* High data rate modes */
            return(RSSI_BASE_VAL_OQPSK_400_RC_DBM);
    }
#elif (TAL_TYPE == AT86RF212)
    switch (tal_pib.CurrentPage)
    {
        case 0: /* BPSK */
            if (tal_pib.CurrentChannel == 0)
            {
                return(RSSI_BASE_VAL_BPSK_20_DBM);
            }
            else
            {
                return(RSSI_BASE_VAL_BPSK_40_DBM);
            }

        case 2: /* O-QPSK */
            if (tal_pib.CurrentChannel == 0)
            {
                return(RSSI_BASE_VAL_OQPSK_100_DBM);
            }
            else
            {
                return(RSSI_BASE_VAL_OQPSK_SIN_250_DBM);
            }

        case 5: /* Chinese band */
        default:    /* High data rate modes */
            return(RSSI_BASE_VAL_OQPSK_RC_250_DBM);
    }
#elif ((TAL_TYPE == AT86RF230A) || (TAL_TYPE == AT86RF230B) || (TAL_TYPE == AT86RF231) ||\
      (TAL_TYPE == ATMEGARFA1) || (TAL_TYPE == AT86RF232) || (TAL_TYPE == AT86RF233)||\
      (TAL_TYPE == ATMEGARFR2))
    return (RSSI_BASE_VAL_DBM);
#else
#error "Missing RSSI_BASE_VAL define"
#endif
}
/*
 * \brief This function is called rest the application equivalent to soft reset
 *
 */
void app_reset(void)
{
    /* app reset - this is to make the node to restart applicationa as bootup */
    set_main_state(INIT, NULL);

    /* INIT was a success - so change to WAIT_FOR_EVENT state */
    set_main_state(WAIT_FOR_EVENT, NULL);
}



/* EOF */
