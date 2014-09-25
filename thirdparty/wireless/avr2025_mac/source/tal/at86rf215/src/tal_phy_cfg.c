/**
 * @file tal_phy_cfg.c
 *
 * @brief This file handles the PHY configuration
 *
 * $Id: tal_phy_cfg.c 36384 2014-08-27 07:19:39Z uwalter $
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
#include <string.h>
#include <stdbool.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "tal_config.h"
#include "ieee_const.h"
#include "tal_pib.h"
#include "tal_internal.h"
#include "ieee_154g.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

#ifdef SUPPORT_OFDM
static retval_t conf_ofdm(trx_id_t trx_id);
#endif
#ifdef SUPPORT_OQPSK
static retval_t conf_oqpsk(trx_id_t trx_id);
#endif
#ifdef SUPPORT_LEGACY_OQPSK
static retval_t conf_leg_oqpsk(trx_id_t trx_id);
#endif
#if (defined SUPPORT_LEGACY_OQPSK) || (defined SUPPORT_OQPSK)
#endif
#ifdef SUPPORT_FSK
#ifndef SUPPORT_MODE_SWITCH
static retval_t conf_fsk(trx_id_t trx_id);
#endif
#endif

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Configure the transceiver w.r.t. modulation
 *
 * @param trx_id Transceiver identifier
 *
 * @return MAC_SUCCESS if setting could be executed, else MAC_INVALID_PARAMETER
 */
retval_t conf_trx_modulation(trx_id_t trx_id)
{
    debug_text(PSTR("conf_trx_modulation()"));

    retval_t status;

    /* Change PHY only in TRXOFF or TXPREP. Since TXPREP is not possible here, check for TRXOFF */
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    rf_cmd_state_t previous_trx_state = (rf_cmd_state_t)pal_trx_reg_read(reg_offset + RG_RF09_STATE);
    if (previous_trx_state != RF_TRXOFF)
    {
        pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_TRXOFF);
#ifdef IQ_RADIO
        pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_CMD, RF_TRXOFF);
#endif
        trx_state[trx_id] = RF_TRXOFF;
    }

    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            status = conf_fsk(trx_id);
            break;
#endif

#ifdef SUPPORT_OFDM
        case OFDM:
            status = conf_ofdm(trx_id);
            break;
#endif

#ifdef SUPPORT_OQPSK
        case OQPSK:
            status = conf_oqpsk(trx_id);
            break;
#endif

#ifdef SUPPORT_LEGACY_OQPSK
        case LEG_OQPSK:
            status = conf_leg_oqpsk(trx_id);
            break;
#endif

        default:
            status = MAC_INVALID_PARAMETER;
            break;
    }

    /* Restore previous state */
    switch (previous_trx_state)
    {
        case RF_RX:
            switch_to_rx(trx_id);
            break;

        case RF_TXPREP:
            switch_to_txprep(trx_id);
            break;

        default:
            /* stay in TRXOFF */
            break;
    }

    return status;
}


#ifdef SUPPORT_OFDM
/**
 * @brief Configures MR-OFDM
 *
 * @param trx_id Transceiver identifier
 */
static retval_t conf_ofdm(trx_id_t trx_id)
{
    retval_t status;

    /* Configure RF */
    status = ofdm_rfcfg(tal_pib[trx_id].phy.phy_mode.ofdm.option, trx_id);
    if (status == MAC_SUCCESS)
    {
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

        /* Configure BB */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_PT, BB_MROFDM);
        pal_trx_bit_write(reg_offset + SR_BBC0_OFDMC_OPT, tal_pib[trx_id].phy.phy_mode.ofdm.option - 1);
    }

    return status;
}
#endif /* #ifdef SUPPORT_OFDM */


#ifdef SUPPORT_OQPSK
/**
 * @brief Configures MR-OQPSK
 *
 * @param trx_id Transceiver identifier
 */
static retval_t conf_oqpsk(trx_id_t trx_id)
{
    retval_t status;

    /* Configure RF */
    status = oqpsk_rfcfg(tal_pib[trx_id].phy.phy_mode.oqpsk.chip_rate, trx_id);
    if (status == MAC_SUCCESS)
    {
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

        /* Configure BB */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_PT, BB_MROQPSK);
        pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKC0_FCHIP,
                          tal_pib[trx_id].phy.phy_mode.oqpsk.chip_rate);
        pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKPHRTX_LEG, 0);
        pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKC2_RXM, 0); // MR mode only
    }

    return status;
}
#endif /* #ifdef SUPPORT_OQPSK */


#ifdef SUPPORT_LEGACY_OQPSK
/**
 * @brief Configures legacy O-QPSK
 *
 * @param trx_id Transceiver identifier
 */
static retval_t conf_leg_oqpsk(trx_id_t trx_id)
{
    retval_t status;

    /* Configure RF */
    status = oqpsk_rfcfg(tal_pib[trx_id].phy.phy_mode.leg_oqpsk.chip_rate, trx_id);
    if (status == MAC_SUCCESS)
    {
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

        /* set bb_core0 baseband mode to OQPSK */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_PT, BB_MROQPSK);
        pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKC0_FCHIP,
                          tal_pib[trx_id].phy.phy_mode.leg_oqpsk.chip_rate);
        pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKC2_RXM, 1); // legacy mode only
        pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKPHRTX_LEG, 1); // configure legacy transmit
    }

    return status;
}
#endif /* #ifdef SUPPORT_LEGACY_OQPSK */


#ifdef SUPPORT_FSK
/**
 * @brief Configures FSK
 *
 * @param trx_id Transceiver identifier
 */
#ifdef SUPPORT_MODE_SWITCH
retval_t conf_fsk(trx_id_t trx_id)
#else
static retval_t conf_fsk(trx_id_t trx_id)
#endif
{
    retval_t status;

    /* Configure RF */
    status = fsk_rfcfg(tal_pib[trx_id].phy.phy_mode.fsk.data_rate, tal_pib[trx_id].phy.phy_mode.fsk.mod_idx, trx_id);
    if (status == MAC_SUCCESS)
    {
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

        pal_trx_bit_write(reg_offset + SR_BBC0_PC_PT, BB_MRFSK);
        // FSKC0
        pal_trx_bit_write(reg_offset + SR_BBC0_FSKC0_MORD,
                          tal_pib[trx_id].phy.phy_mode.fsk.mod_type);
        pal_trx_bit_write(reg_offset + SR_BBC0_FSKC0_MIDX,
                          tal_pib[trx_id].phy.phy_mode.fsk.mod_idx);
        // FSKC1
        pal_trx_bit_write(reg_offset + SR_BBC0_FSKC1_SRATE,
                          tal_pib[trx_id].phy.phy_mode.fsk.data_rate);
        /* Configure SFD */
        set_sfd(trx_id);
    }

    return status;
}
#endif /* #ifdef SUPPORT_FSK */


#ifdef SUPPORT_FSK
/**
 * @brief Configures SFDs
 *
 * @param trx_id Transceiver identifier
 */
void set_sfd(trx_id_t trx_id)
{
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    uint32_t sfd;

    uint32_t sfd_reset;
    pal_trx_read(reg_offset + RG_BBC0_FSKSFD0L, (uint8_t *)&sfd_reset, 4);
    sfd_reset++;

    if (tal_pib[trx_id].phy.phy_mode.fsk.mod_type == F2FSK)
    {
        if (tal_pib[trx_id].MRFSKSFD == 0)
        {
            sfd = (uint32_t)F2FSK_SFD_0_UNCODED | ((uint32_t)F2FSK_SFD_0_CODED << 16);
        }
        else // SFD 1
        {
            sfd = (uint32_t)F2FSK_SFD_1_UNCODED | ((uint32_t)F2FSK_SFD_1_CODED << 16);
        }
    }
    else // 4FSK
    {
        if (tal_pib[trx_id].MRFSKSFD == 0)
        {
            if (tal_pib[trx_id].FSKFECEnabled)
            {
                sfd = F4FSK_SFD_0_CODED;
            }
            else
            {
                sfd = F4FSK_SFD_0_UNCODED;
            }
        }
        else // SFD 1
        {
            if (tal_pib[trx_id].FSKFECEnabled)
            {
                sfd = F4FSK_SFD_1_CODED;
            }
            else
            {
                sfd = F4FSK_SFD_1_UNCODED;
            }
        }
    }
    pal_trx_write(reg_offset + RG_BBC0_FSKSFD0L, (uint8_t *)&sfd, 4);
}
#endif /* #ifdef SUPPORT_FSK */


/* EOF */
