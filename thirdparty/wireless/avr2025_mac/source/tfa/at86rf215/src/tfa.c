/**
 * @file tfa.c
 *
 * @brief Implementation of Transceiver Feature Access (TFA) functionality.
 *
 * $Id: tfa.c 34576 2013-04-25 07:36:44Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#if (defined ENABLE_TFA) || (defined CW_SUPPORTED)

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "at86rf215lt.h"
#include "tal_internal.h"
#include "tfa.h"



/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */


#if (defined ENABLE_TFA) || (defined TFA_CCA)
/**
 * @brief Perform a CCA
 *
 * This blocking function performs a CCA request.
 *
 * @return phy_enum_t PHY_IDLE or PHY_BUSY
 */
phy_enum_t tfa_cca_perform(trx_id_t trx_id)
{
    phy_enum_t ret;

    //debug_text(PSTR("tfa_cca_perform()"));

    if (tal_state[trx_id] != TAL_IDLE)
    {
        //debug_text(PSTR("TAL busy"));
        ret = PHY_BUSY;
    }
    else
    {
        rf_cmd_state_t previous_state = trx_state[trx_id];

        trigger_cca_meaurement(trx_id);

        while (TAL_RF_IS_IRQ_SET(trx_id, RF_IRQ_EDC) == false);
        TAL_RF_IRQ_CLR(trx_id, RF_IRQ_EDC);

        /* Since it is a blocking function, restore TAL state */
        tal_state[trx_id] = TAL_IDLE;

        switch_to_txprep(trx_id); /* Leave Rx mode */

        /* Restore PHY type - switch BB on again */
        uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_BBEN, 1);

        /* Capture ED value for current frame / ED scan */
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        tal_current_ed_val[trx_id] = pal_trx_reg_read(rf_reg_offset + RG_RF09_EDV);
        //debug_text_val(PSTR("tal_current_ed_val = "), (uint8_t)tal_current_ed_val[trx_id]);
#if (DEBUG > 0) && (!defined IAR_SIMULATION)
        uint_fast8_t dbm = (uint8_t)(~(uint32_t)tal_current_ed_val[trx_id] + 1);
        //debug_text_val(PSTR("Energy (dBm) = -"), dbm);
#endif
        //debug_text_val(PSTR("tal_pib[trx_id].CCAThreshold = "), (uint8_t)tal_pib[trx_id].CCAThreshold);
        //debug_text_val(PSTR("CCAThreshold dBm = -"), (uint8_t)(~(uint8_t)tal_pib[trx_id].CCAThreshold) + 1);
        if (tal_current_ed_val[trx_id] < tal_pib[trx_id].CCAThreshold)
        {
            /* Idle */
            //debug_text(PSTR("channel idle"));
            ret = PHY_IDLE;
        }
        else
        {
            /* Busy */
            //debug_text(PSTR("channel busy"));
            ret = PHY_BUSY;
        }

        /* Restore previous trx state */
        if (previous_state == RF_RX)
        {
            switch_to_rx(trx_id);
        }
        else
        {
            //debug_text(PSTR("Switch back to TRXOFF"));
            /* Switch to TRXOFF */
            pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TRXOFF);
            trx_state[trx_id] = RF_TRXOFF;
        }
    }

    return ret;
}
#endif


#if (defined ENABLE_TFA) || (defined CW_SUPPORTED)
/**
 * @brief Starts continuous transmission on current channel
 *
 * @param trx_id Identifier of the transceiver
 * @param tx_mode Transmission mode
 */
void tfa_continuous_tx_start(trx_id_t trx_id, continuous_tx_mode_t tx_mode)
{
    uint16_t len;

    //debug_text(PSTR("tfa_continuous_tx_start()"));

    if (tal_state[trx_id] != TAL_IDLE)
    {
        //debug_text(PSTR("TAL not IDLE"));
        return;
    }

    if (trx_state[trx_id] == RF_RX)
    {
        tal_state[trx_id] = TAL_TFA_CW_RX;
    }
    else
    {
        tal_state[trx_id] = TAL_TFA_CW;
    }

    /* Ensure TxPREP state */
    switch_to_txprep(trx_id);

    /* Enable continuous transmission mode */
    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_CTX, 1);

    if (tx_mode == CW_MODE)
    {
        //debug_text(PSTR("Carrier mode"));

        /* Configure DAC to generate carrier signal */
        uint8_t dac_config[2] = {0xFF, 0xFF};
        uint16_t rft_reg_offset = RFT_TST_ADDR_OFFSET * trx_id;
        pal_trx_write(rft_reg_offset + RG_RF09_TXDACI, dac_config, 2);
    }

    /* Fill length field */
#ifdef SUPPORT_LEGACY_OQPSK
    if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
    {
        len = 127;
    }
    else
#endif
    {
        len = 2047;
    }
    pal_trx_write(bb_reg_offset + RG_BBC0_TXFLL, (uint8_t *)&len, 2);

    /* Trigger Tx start */
    //debug_text(PSTR("Start transmission"));
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TX);
    trx_state[trx_id] = RF_TX;

    if (tx_mode == PRBS_MODE)
    {
        //debug_text(PSTR("PRBS mode"));

        /* Fill frame buffer */
        uint16_t tx_frm_buf_offset = BB_TX_FRM_BUF_OFFSET * trx_id;
        uint8_t data[10];
        for (uint16_t k = 0; k < (len / 10); k++)
        {
            for (uint16_t i = 0; i < 10; i++)
            {
                data[i] = pal_trx_reg_read(rf_reg_offset + RG_RF09_RNDV);
            }
            pal_trx_write(tx_frm_buf_offset + RG_BBC0_FBTXS, data, 10);
            tx_frm_buf_offset += 10;
        }
        uint16_t remaining_bytes = len % 10;
        for (uint16_t i = 0; i < remaining_bytes; i++)
        {
            data[i] = pal_trx_reg_read(rf_reg_offset + RG_RF09_RNDV);
        }
        pal_trx_write(tx_frm_buf_offset + RG_BBC0_FBTXS, data, remaining_bytes);
    }
}
#endif


#if (defined ENABLE_TFA) || (defined CW_SUPPORTED)
/**
 * @brief Stops CW transmission
 *
 * @param trx_id Identifier of the transceiver
 */
void tfa_continuous_tx_stop(trx_id_t trx_id)
{
    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    // Stop continuous transmission
    pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_CTX, 0);
    switch_to_txprep(trx_id);

    /* Disable carrier transmission - even if it has not been used. */
    uint8_t dac_config[2] = {0x00, 0x00};
    uint16_t rft_reg_offset = RFT_TST_ADDR_OFFSET * trx_id;
    pal_trx_write(rft_reg_offset + RG_RF09_TXDACI, dac_config, 2);

    /* Restore previous settings */
    if (tal_state[trx_id] == TAL_TFA_CW_RX)
    {
        tal_state[trx_id] = TAL_IDLE;
        switch_to_rx(trx_id);
    }
    else
    {
        tal_state[trx_id] = TAL_IDLE;
        //debug_text(PSTR("Switch back to TRXOFF"));
        /* Switch to TRXOFF */
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TRXOFF);
        trx_state[trx_id] = RF_TRXOFF;
    }
}
#endif


#endif /* #if (defined ENABLE_TFA) || (defined TFA_BAT_MON) */


/* EOF */
