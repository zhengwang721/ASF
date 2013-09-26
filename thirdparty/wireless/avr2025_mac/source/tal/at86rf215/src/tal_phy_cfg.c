/**
 * @file tal_phy_cfg.c
 *
 * @brief This file handles the PHY configuration
 *
 * $Id: tal_phy_cfg.c 35159 2013-09-24 06:52:33Z uwalter $
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
#if (PAL_GENERIC_TYPE == MEGA_RF_SIM)
#include "verification.h"
#endif

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

#if (defined SUPPORT_LEGACY_OQPSK) || (defined SUPPORT_OQPSK)
FLASH_DECLARE(uint8_t oqpsk_tx_rfcfg_table[4][2]) =
{
    /* rows: MCSn; column: option n */ \
    {
        (RF_PARAMP32U << TXCUTC_PARAMP_SHIFT | RF_FLC400KHZ << TXCUTC_LPFCUT_SHIFT),
        (10 << TXDFE_SR_SHIFT | 3 << TXDFE_RCUT_SHIFT)
    },
    {
        (RF_PARAMP16U << TXCUTC_PARAMP_SHIFT | RF_FLC400KHZ << TXCUTC_LPFCUT_SHIFT),
        (5 << TXDFE_SR_SHIFT | 3 << TXDFE_RCUT_SHIFT)
    },
    {
        (RF_PARAMP4U << TXCUTC_PARAMP_SHIFT | RF_FLC1000KHZ << TXCUTC_LPFCUT_SHIFT),
        (1 << TXDFE_SR_SHIFT | 3 << TXDFE_RCUT_SHIFT)
    },
    {
        (RF_PARAMP4U << TXCUTC_PARAMP_SHIFT | RF_FLC1000KHZ << TXCUTC_LPFCUT_SHIFT),
        (1 << TXDFE_SR_SHIFT | 4 << TXDFE_RCUT_SHIFT)
    }
};

FLASH_DECLARE(uint8_t oqpsk_rx_rfcfg_table[4][4]) =
{
    {
        (RF_BW160KHZ_IF250KHZ << RXBWC_BW_SHIFT),
        (1 << RXDFE_RCUT_SHIFT | 10 << RXDFE_SR_SHIFT),
        (2 << AGCC_AVGS_SHIFT | 1 << AGCC_EN_SHIFT),
        (7 << AGCS_TGT_SHIFT | 23 << AGCS_GCW_SHIFT)
    },
    {
        (RF_BW250KHZ_IF250KHZ << RXBWC_BW_SHIFT),
        (1 << RXDFE_RCUT_SHIFT | 5 << RXDFE_SR_SHIFT),
        (2 << AGCC_AVGS_SHIFT | 1 << AGCC_EN_SHIFT),
        (7 << AGCS_TGT_SHIFT | 23 << AGCS_GCW_SHIFT)
    },
    {
        (RF_BW1000KHZ_IF1000KHZ << RXBWC_BW_SHIFT),
        (0 << RXDFE_RCUT_SHIFT | 1 << RXDFE_SR_SHIFT),
        (0 << AGCC_AVGS_SHIFT | 1 << AGCC_EN_SHIFT),
        (3 << AGCS_TGT_SHIFT | 23 << AGCS_GCW_SHIFT)
    },
    {
        (RF_BW2000KHZ_IF2000KHZ << RXBWC_BW_SHIFT),
        (2 << RXDFE_RCUT_SHIFT | 1 << RXDFE_SR_SHIFT),
        (0 << AGCC_AVGS_SHIFT | 1 << AGCC_EN_SHIFT),
        (3 << AGCS_TGT_SHIFT | 23 << AGCS_GCW_SHIFT)
    }
};
#endif /* #if (defined SUPPORT_LEGACY_OQPSK) || (defined SUPPORT_OQPSK) */

#ifdef SUPPORT_OFDM
FLASH_DECLARE(uint8_t ofdm_tx_rfcfg_table[4][2]) =
{
    /* rows: MCSn; column: option n */ \
    {
        (RF_PARAMP4U << TXCUTC_PARAMP_SHIFT | RF_FLC1000KHZ << TXCUTC_LPFCUT_SHIFT),
        (3 << TXDFE_SR_SHIFT | 4 << TXDFE_RCUT_SHIFT)
    },
    {
        (RF_PARAMP4U << TXCUTC_PARAMP_SHIFT | RF_FLC400KHZ << TXCUTC_LPFCUT_SHIFT),
        (3 << TXDFE_SR_SHIFT | 3 << TXDFE_RCUT_SHIFT)
    },
    {
        (RF_PARAMP4U << TXCUTC_PARAMP_SHIFT | RF_FLC315KHZ << TXCUTC_LPFCUT_SHIFT),
        (6 << TXDFE_SR_SHIFT | 3 << TXDFE_RCUT_SHIFT)
    },
    {
        (RF_PARAMP4U << TXCUTC_PARAMP_SHIFT | RF_FLC160KHZ << TXCUTC_LPFCUT_SHIFT),
        (6 << TXDFE_SR_SHIFT | 2 << TXDFE_RCUT_SHIFT)
    }
};

/* Use 900MHz setting if different from 2.4GHz */
FLASH_DECLARE(uint8_t ofdm_rx_rfcfg_table[4][4]) =
{
    {
        (RF_BW1250KHZ_IF2000KHZ << RXBWC_BW_SHIFT | 0 << RXBWC_IFS_SHIFT),
#ifdef TCXO_AVAILABLE
        (3 << RXDFE_RCUT_SHIFT | 3 << RXDFE_SR_SHIFT),
#else
        (4 << RXDFE_RCUT_SHIFT | 3 << RXDFE_SR_SHIFT),
#endif
        (0 << AGCC_AVGS_SHIFT | 0 << AGCC_AGCI_SHIFT | 1 << AGCC_EN_SHIFT),
        (3 << AGCS_TGT_SHIFT | 23 << AGCS_GCW_SHIFT)
    },

    {
        (RF_BW630KHZ_IF1000KHZ << RXBWC_BW_SHIFT | 0 << RXBWC_IFS_SHIFT),
        (1 << RXDFE_RCUT_SHIFT | 3 << RXDFE_SR_SHIFT),
        (2 << AGCC_AVGS_SHIFT | 0 << AGCC_AGCI_SHIFT | 1 << AGCC_EN_SHIFT),
        (7 << AGCS_TGT_SHIFT | 23 << AGCS_GCW_SHIFT)
    },

    {
        (RF_BW320KHZ_IF500KHZ << RXBWC_BW_SHIFT | 1 << RXBWC_IFS_SHIFT),
        (2 << RXDFE_RCUT_SHIFT | 6 << RXDFE_SR_SHIFT),
        (0 << AGCC_AVGS_SHIFT | 1 << AGCC_AGCI_SHIFT | 1 << AGCC_EN_SHIFT),
        (3 << AGCS_TGT_SHIFT | 23 << AGCS_GCW_SHIFT)
    },

    {
#ifdef TCXO_AVAILABLE
        (RF_BW200KHZ_IF250KHZ << RXBWC_BW_SHIFT | 1 << RXBWC_IFS_SHIFT),
        (0 << RXDFE_RCUT_SHIFT | 6 << RXDFE_SR_SHIFT),
#else
        (RF_BW320KHZ_IF500KHZ << RXBWC_BW_SHIFT | 1 << RXBWC_IFS_SHIFT),
        (1 << RXDFE_RCUT_SHIFT | 6 << RXDFE_SR_SHIFT),
#endif
        (0 << AGCC_AVGS_SHIFT | 1 <<  AGCC_AGCI_SHIFT | 1 << AGCC_EN_SHIFT),
        (3 << AGCS_TGT_SHIFT | 23 << AGCS_GCW_SHIFT)
    }
};
#endif /* #ifdef SUPPORT_OFDM */

/* === PROTOTYPES ========================================================== */

#ifdef SUPPORT_OFDM
static retval_t tal_ofdm_rfcfg(trx_id_t trx_id, ofdm_option_t ofdm_opt);
#endif
#if (defined SUPPORT_LEGACY_OQPSK) || (defined SUPPORT_OQPSK)
static retval_t tal_oqpsk_rfcfg(trx_id_t trx_id, oqpsk_chip_rate_t chip_rate);
#endif
#ifdef SUPPORT_FSK
static retval_t tal_fsk_rfcfg(trx_id_t trx_id);
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
    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;

    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            /* Configure RF */
            status = tal_fsk_rfcfg(trx_id);
            if (status == MAC_SUCCESS)
            {
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_PT, BB_MRFSK);

                pal_trx_bit_write(bb_reg_offset + SR_BBC0_FSKC0_MORD, 0); // ?
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_FSKC0_MIDX, tal_pib[trx_id].phy.phy_mode.fsk.mod_idx); // ?
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_FSKC0_BT, 1); // ?
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_FSKC1_FSKPLH, 0); // ?
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_FSKC1_SRATE, 0); // ?
                //FSKC1
                //FSKPLL
                pal_trx_reg_write(bb_reg_offset + RG_BBC0_FSKPLL, 8);
                //FSKPHR
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_FSKPHR_DWTX, 1);
            }
            break;
#endif

#ifdef SUPPORT_OFDM
        case OFDM:
            /* Configure RF */
            status = tal_ofdm_rfcfg(trx_id, tal_pib[trx_id].phy.phy_mode.ofdm.option);
            if (status == MAC_SUCCESS)
            {
                /* Configure BB */
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_PT, BB_MROFDM);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_OPT, tal_pib[trx_id].phy.phy_mode.ofdm.option);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_POI, tal_pib[trx_id].phy.phy_mode.ofdm.interl);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMPHRTX_MCS, tal_pib[trx_id].phy.phy_mode.ofdm.mcs_val);
            }
            break;
#endif

#ifdef SUPPORT_OQPSK
        case OQPSK:
            /* Configure RF */
            status = tal_oqpsk_rfcfg(trx_id, tal_pib[trx_id].phy.phy_mode.oqpsk.chip_rate);
            if (status == MAC_SUCCESS)
            {
                /* Configure BB */
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_PT, BB_MROQPSK);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKC0_FCHIP, tal_pib[trx_id].phy.phy_mode.oqpsk.chip_rate);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKPHRTX_LEG, 0);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKPHRTX_MOD,
                                  tal_pib[trx_id].phy.phy_mode.oqpsk.rate_mode);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKC2_RXM, 0); // MR mode only
            }
            break;
#endif

#ifdef SUPPORT_LEGACY_OQPSK
        case LEG_OQPSK:
            /* Configure RF */
            status = tal_oqpsk_rfcfg(trx_id, tal_pib[trx_id].phy.phy_mode.leg_oqpsk.chip_rate);
            if (status == MAC_SUCCESS)
            {
                /* set bb_core0 baseband mode to OQPSK */
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_PT, BB_MROQPSK);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKC0_FCHIP,
                                  tal_pib[trx_id].phy.phy_mode.leg_oqpsk.chip_rate);
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKC2_RXM, 1); // legacy mode only
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OQPSKPHRTX_LEG, 1); // configure legacy transmit
            }
            break;
#endif

        default:
            status = MAC_INVALID_PARAMETER;
            break;
    }

    return status;
}


#ifdef SUPPORT_OFDM
/**
 * @brief Configures RF according MR-OFDM
 *
 * @param trx_id Transceiver identifier
 * @param ofdm_opt OFDM option
 */
static retval_t tal_ofdm_rfcfg(trx_id_t trx_id, ofdm_option_t ofdm_opt)
{
    retval_t status = MAC_SUCCESS;

#if (DEBUG > 0)
    if (ofdm_opt > 3)
    {
        debug_text_val_finish(PSTR("ERROR: OFDM option must be in [0..3], is:"), ofdm_opt, DEBUG_ERROR);
        status = MAC_INVALID_PARAMETER;
    }
    else
#endif
    {
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

        /* Limit transmit power */
        pal_trx_bit_write(rf_reg_offset + SR_RF09_PAC_TXPWR, 28);
        tal_pib[trx_id].TransmitPower = 11;

#if (defined __GNUC__) && (defined __AVR__)
        uint8_t tx_cfg[2] =
        {
            (uint8_t)PGM_READ_BYTE(&ofdm_tx_rfcfg_table[ofdm_opt][0]),
            (uint8_t)PGM_READ_BYTE(&ofdm_tx_rfcfg_table[ofdm_opt][1])
        };

        pal_trx_write(rf_reg_offset + RG_RF09_TXCUTC, tx_cfg, 2);

        uint8_t rx_cfg[4] =
        {
            (uint8_t)PGM_READ_BYTE(&ofdm_rx_rfcfg_table[ofdm_opt][0]),
            (uint8_t)PGM_READ_BYTE(&ofdm_rx_rfcfg_table[ofdm_opt][1]),
            (uint8_t)PGM_READ_BYTE(&ofdm_rx_rfcfg_table[ofdm_opt][2]),
            (uint8_t)PGM_READ_BYTE(&ofdm_rx_rfcfg_table[ofdm_opt][3])
        };

        pal_trx_write(rf_reg_offset + RG_RF09_RXBWC, rx_cfg, 4);
#else
        pal_trx_write(rf_reg_offset + RG_RF09_TXCUTC,
                      (uint8_t *)&ofdm_tx_rfcfg_table[ofdm_opt][0], 2);
        pal_trx_write(rf_reg_offset + RG_RF09_RXBWC,
                      (uint8_t *)&ofdm_rx_rfcfg_table[ofdm_opt][0], 4);
#endif

#ifndef TCXO_AVAILABLE
        /* Handle band specific settings */
        if ((ofdm_opt != 0) && (trx_id == RF24))
        {
            uint8_t tmp[2];

            switch (ofdm_opt)
            {
                case  OFDM_OPT_2:    /*OFDM Option 2: BW nom.: 552 kHz */
                    /* RXBWC */
                    tmp[0] = (RF_BW800KHZ_IF1000KHZ << RXBWC_BW_SHIFT) | (0 << RXBWC_IFS_SHIFT);
                    /* RXDFE */
                    tmp[1] = (3 << RXDFE_RCUT_SHIFT) | (3 << RXDFE_SR_SHIFT);
                    break;

                case  OFDM_OPT_3:    /*OFDM Option 3: BW nom.: 281 kHz */
                    /* RXBWC */
                    tmp[0] = (RF_BW400KHZ_IF500KHZ << RXBWC_BW_SHIFT) | (1 << RXBWC_IFS_SHIFT);
                    /* RXDFE */
                    tmp[1] = (3 << RXDFE_RCUT_SHIFT) | (6 << RXDFE_SR_SHIFT);
                    break;

                case  OFDM_OPT_4:    /*OFDM Option 4: BW nom.: 156 kHz */
                    /* RXBWC */
                    tmp[0] = (RF_BW320KHZ_IF500KHZ << RXBWC_BW_SHIFT) | (1 << RXBWC_IFS_SHIFT);
                    /* RXDFE */
                    tmp[1] = (2 << RXDFE_RCUT_SHIFT) | (6 << RXDFE_SR_SHIFT);;
                    break;

                default:
                    debug_text_val_finish(PSTR("ERROR: OFDM option must be in [0..3], is:"), ofdm_opt, DEBUG_ERROR);
                    status = MAC_INVALID_PARAMETER;
                    break;
            }
            pal_trx_write(RG_RF24_RXBWC, tmp, 2);
        }
#endif /* #ifndef TCXO_AVAILABLE */

    }

    return status;
}
#endif /* #ifdef SUPPORT_OFDM */


#if (defined SUPPORT_LEGACY_OQPSK) || (defined SUPPORT_OQPSK)
/**
 * @brief Configures RF according MR-OQPSK
 *
 * @param trx_id Transceiver identifier
 * @param chip_rate OQPSK chip mode
 */
static retval_t tal_oqpsk_rfcfg(trx_id_t trx_id, oqpsk_chip_rate_t chip_rate)
{
    retval_t status = MAC_SUCCESS;

    if (chip_rate > 3)
    {
        debug_text_val_finish(PSTR("ERROR: MODE_FCHIP  must be in [0..3], is:"), chip_rate, DEBUG_ERROR);
        status = MAC_INVALID_PARAMETER;
    }
    else
    {
        /* set transmit power to 14 dBm */
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        pal_trx_bit_write(rf_reg_offset + SR_RF09_PAC_TXPWR, 0x1F);
        tal_pib[trx_id].TransmitPower = 14;

#if (defined __GNUC__) && (defined __AVR__)
        uint8_t tx_cfg[2] =
        {
            (uint8_t)PGM_READ_BYTE(&oqpsk_tx_rfcfg_table[chip_rate][0]),
            (uint8_t)PGM_READ_BYTE(&oqpsk_tx_rfcfg_table[chip_rate][1])
        };

        pal_trx_write(rf_reg_offset + RG_RF09_TXCUTC, tx_cfg, 2);

        uint8_t rx_cfg[4] =
        {
            (uint8_t)PGM_READ_BYTE(&oqpsk_rx_rfcfg_table[chip_rate][0]),
            (uint8_t)PGM_READ_BYTE(&oqpsk_rx_rfcfg_table[chip_rate][1]),
            (uint8_t)PGM_READ_BYTE(&oqpsk_rx_rfcfg_table[chip_rate][2]),
            (uint8_t)PGM_READ_BYTE(&oqpsk_rx_rfcfg_table[chip_rate][3])
        };

        pal_trx_write(rf_reg_offset + RG_RF09_RXBWC, rx_cfg, 4);
#else
        pal_trx_write(rf_reg_offset + RG_RF09_TXCUTC,
                      (uint8_t *)&oqpsk_tx_rfcfg_table[chip_rate][0], 2);
        pal_trx_write(rf_reg_offset + RG_RF09_RXBWC,
                      (uint8_t *)&oqpsk_rx_rfcfg_table[chip_rate][0], 4);
#endif
    }

    return status;
}
#endif /* #if (defined SUPPORT_LEGACY_OQPSK) || (defined SUPPORT_OQPSK) */


#ifdef SUPPORT_FSK
static retval_t tal_fsk_rfcfg(trx_id_t trx_id)
{
    retval_t status = MAC_SUCCESS;

    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    /* 50 kbps, fs= 400 kHz */

    /* -- TX ----------------------------------------------------------- */
    /* PA ramp time 32 us */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_TXCUTC_PARAMP, RF_PARAMP32U);
    /* I/F rate: 0.4 Ms/sec */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_TXDFE_SR, 10);
    /* DFE cut-off: 0.75 */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_TXDFE_RCUT, 3);
    /* analog tx-filter */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_TXCUTC_LPFCUT, RF_FLC400KHZ);
    /* -- RX ----------------------------------------------------------- */
    /* analog rx-filter */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_RXBWC_BW, RF_BW160KHZ_IF250KHZ);
    /* DFE cut-off: 0.375 */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_RXDFE_RCUT, 1);
    /* I/F rate: 0.4 Ms/sec */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_RXDFE_SR, 10);
    /* AGC setup: 32 samples */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_AGCC_AVGS, 2);
    /* AGC target: -42 dB */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_AGCS_TGT, 7);

    return status;
}
#endif /* #ifdef SUPPORT_FSK */

/* EOF */
