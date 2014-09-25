/**
 * @file tfa_batmon.c
 *
 * @brief Implementation of TFA battery monitor functionality.
 *
 * $Id: tfa_batmon.c 36327 2014-08-14 07:15:23Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#if (defined ENABLE_TFA) || (defined TFA_BAT_MON) || \
    (defined TFA_BAT_MON_READ) || (defined TFA_BAT_MON_IRQ)

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "at86rf215.h"
#include "tal_internal.h"
#include "tfa.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

#define BATMON_MON_VTH_MIN      1700
#define BATMON_MON_VTH_MAX      3675

/* === GLOBALS ============================================================= */

#if (defined ENABLE_TFA) || (defined TFA_BAT_MON) || (defined TFA_BAT_MON_IRQ)
static irq_handler_t irq_hdl_batmon;
#endif

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */


#if (defined ENABLE_TFA) || (defined TFA_BAT_MON) || (defined TFA_BAT_MON_READ)
/**
 * @brief Gets the transceiver's supply voltage
 *
 * @return mv Milli Volt; 0 if below range, 0xFFFF if above range
 */
uint16_t tfa_get_batmon_voltage(void)
{
    uint16_t mv;

    //debug_text(PSTR("tfa_get_batmon_voltage()"));

    if ((tal_state[RF09] == TAL_SLEEP) && (tal_state[RF24] == TAL_SLEEP))
    {
#if DEBUG > 0
        //debug_text(PSTR("Both trx are in sleep mode"));
#endif
        return 0;
    }

    /* Store previous settings: range, threshold, IRQ */
#ifdef IQ_RADIO
    bool previous_range = pal_trx_bit_read(RF215_RF, SR_RF_BMDVC_BMHR);
    uint8_t pre_vth = pal_trx_bit_read(RF215_RF, SR_RF_BMDVC_BMVTH);
#else
    bool previous_range = pal_trx_bit_read(SR_RF_BMDVC_BMHR);
    uint8_t pre_vth = pal_trx_bit_read(SR_RF_BMDVC_BMVTH);
#endif

    /* Disable both battery monitor interrupt during measurement */
    for (uint8_t i = 0; i < 2; i++)
    {
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * i;
#ifdef IQ_RADIO
        pal_trx_bit_write(RF215_RF, reg_offset + SR_RF09_IRQM_BATLOW, 0);
#else
        pal_trx_bit_write(reg_offset + SR_RF09_IRQM_BATLOW, 0);
#endif
    }

    /* Check if supply voltage is within lower range */
#ifdef IQ_RADIO
    pal_trx_bit_write(RF215_RF, SR_RF_BMDVC_BMHR, 0);
    pal_trx_bit_write(RF215_RF, SR_RF_BMDVC_BMVTH, 0x0F);
    pal_timer_delay(2); /* Wait until Batmon has been settled. */ // @ ToDo: Is it necessary?
    bool high_range = pal_trx_bit_read(RF215_RF, SR_RF_BMDVC_BMS);
#else
    pal_trx_bit_write(SR_RF_BMDVC_BMHR, 0);
    pal_trx_bit_write(SR_RF_BMDVC_BMVTH, 0x0F);
    pal_timer_delay(2); /* Wait until Batmon has been settled. */ // @ ToDo: Is it necessary?
    bool high_range = pal_trx_bit_read(SR_RF_BMDVC_BMS);
#endif
    if (high_range)
    {
        /* EVDD is above threshold */
        /* Set to high range */
        //debug_text(PSTR("high range"));
#ifdef IQ_RADIO
        pal_trx_bit_write(RF215_RF, SR_RF_BMDVC_BMHR, 1);
#else
        pal_trx_bit_write(SR_RF_BMDVC_BMHR, 1);
#endif
    }
    else
    {
        /* EVDD is below threshold */
        /* Keep current range; i.e. low range */
        //debug_text(PSTR("low range"));
    }

    /* Find the voltage factor */
    uint8_t vth = 0xFF; // 0xFF used as flag
    for (uint8_t i = 0; i < 16; i++)
    {
#ifdef IQ_RADIO
        pal_trx_bit_write(RF215_RF, SR_RF_BMDVC_BMVTH, i);
        pal_timer_delay(2); /* Wait until Batmon has been settled. */ // @ ToDo: Is it necessary?
        if (pal_trx_bit_read(RF215_RF, SR_RF_BMDVC_BMS) == 0)
#else
        pal_trx_bit_write(SR_RF_BMDVC_BMVTH, i);
        pal_timer_delay(2); /* Wait until Batmon has been settled. */ // @ ToDo: Is it necessary?
        if (pal_trx_bit_read(SR_RF_BMDVC_BMS) == 0)
#endif
        {
            vth = i;
            //debug_text_val(PSTR("vth = "), vth);
            break;
        }
    }

    /* Calculate EVDD */
    if (high_range)
    {
        /* High range */
        if (vth == 0xFF)
        {
            mv = BATMON_MON_VTH_MAX; /* Above range: return max value */
        }
        else
        {
            mv = 2550 + ((uint16_t)75 * (uint16_t)vth);
        }
    }
    else
    {
        /* Low range */
        if (vth == 0)
        {
            mv = BATMON_MON_VTH_MIN; /* Below range: return min value */
        }
        else
        {
            mv = 1700 + ((uint16_t)50 * (uint16_t)vth);
        }
    }

    /* Re-store previous settings: range, threshold, IRQ */
#ifdef IQ_RADIO
    pal_trx_bit_write(RF215_RF, SR_RF_BMDVC_BMHR, previous_range);
    pal_trx_bit_write(RF215_RF, SR_RF_BMDVC_BMVTH, pre_vth);
#else
    pal_trx_bit_write(SR_RF_BMDVC_BMHR, previous_range);
    pal_trx_bit_write(SR_RF_BMDVC_BMVTH, pre_vth);
#endif
    /* Clear pending BATLOW IRQ */
    uint8_t irqs[2];
    pal_trx_read(RG_RF09_IRQS, irqs, 2);
    for (uint8_t i = 0; i < 2; i++)
    {
        irqs[i] &=  (uint8_t)(~(uint32_t)RF_IRQ_BATLOW);
        TAL_RF_IRQ_ADD((trx_id_t)i, irqs[i]);
    }

    for (uint8_t i = 0; i < 2; i++)
    {
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * i;
#ifdef IQ_RADIO
        pal_trx_bit_write(RF215_RF, reg_offset + SR_RF09_IRQM_BATLOW, 1);
#else
        pal_trx_bit_write(reg_offset + SR_RF09_IRQM_BATLOW, 1);
#endif
    }

    return mv;
}
#endif /* #if (defined ENABLE_TFA) || (defined TFA_BAT_MON_READ) */


#if (defined ENABLE_TFA) || (defined TFA_BAT_MON) || (defined TFA_BAT_MON_IRQ)
/*
 * @brief Setups the battery monitor interrupt
 *
 * @param   batmon_irq_cb Callback function for the batmon interrupt
 * @param   vth Threshold value in millivolt
 *
 * @return MAC_SUCCESS if provided voltage is within range, else
 *         MAC_INVALID_PARAMETER
 *
 * @ingroup apiTfaApi
 */
retval_t tfa_batmon_irq_init(FUNC_PTR(batmon_irq_cb), uint16_t vth)
{
    retval_t ret;

    //debug_text(PSTR("tfa_batmon_irq_init()"));

    if ((vth < BATMON_MON_VTH_MIN) || (vth > BATMON_MON_VTH_MAX))
    {
        //debug_text(PSTR("Invalid range"));
        ret = MAC_INVALID_PARAMETER;
    }
    else
    {
        /* Store callback function */
        irq_hdl_batmon = (irq_handler_t)batmon_irq_cb;

        /* Configure battery monitor threshold */
        bool high_range;
        uint8_t reg;
        if (vth >= 2550)
        {
            high_range = true;
            reg = (vth - 2550) / 75;
        }
        else
        {
            high_range = false;
            if (vth >= 2450)
            {
                reg = 0x0F;
            }
            else
            {
                reg = (vth - 1700) / 50;
            }
        }
#ifdef IQ_RADIO
        pal_trx_bit_write(RF215_RF, SR_RF_BMDVC_BMHR, high_range);
        pal_trx_bit_write(RF215_RF, SR_RF_BMDVC_BMVTH, reg);
#else
        pal_trx_bit_write(SR_RF_BMDVC_BMHR, high_range);
        pal_trx_bit_write(SR_RF_BMDVC_BMVTH, reg);
#endif

        ret = MAC_SUCCESS;
    }

    return ret;
}
#endif /* #if (defined ENABLE_TFA) || (defined TFA_BAT_MON) || (defined TFA_BAT_MON_IRQ) */


#if (defined ENABLE_TFA) || (defined TFA_BAT_MON) || (defined TFA_BAT_MON_IRQ)
void handle_batmon_irq(void)
{
    if (irq_hdl_batmon != NULL)
    {
        irq_hdl_batmon();
    }
}
#endif /* #if (defined ENABLE_TFA) || (defined TFA_BAT_MON) || (defined TFA_BAT_MON_IRQ) */




#endif /* #if (defined ENABLE_TFA)  || . . . */


/* EOF */
