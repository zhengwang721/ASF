/**
 * @file tal_init.c
 *
 * @brief This file implements functions for initializing TAL and reset.
 *
 * $Id: tal_init.c 36436 2014-09-01 13:49:57Z uwalter $
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
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_pib.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "pal.h"
#include "tal.h"
#include "tal_internal.h"
#include "tal_config.h"
#ifdef ENABLE_TFA
#include "tfa.h"
#endif
#include "mac_build_config.h"


/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */
/**
 * \brief Initializes all timers used by the TAL module by assigning id's to
 *each of them
 */
static retval_t tal_timer_init(void);

/**
 * \brief Stops all initialized TAL timers
 */
static void tal_timers_stop(void);
//static retval_t trx_reset(trx_id_t trx_id); //vk
static void cleanup_tal(trx_id_t trx_id);
static void trx_init(void);

uint8_t	TAL_T_0;
uint8_t	TAL_T_1;

#ifdef ENABLE_FTN_PLL_CALIBRATION

uint8_t	TAL_T_CALIBRATION_0 ;
uint8_t	TAL_T_CALIBRATION_1 ;

#endif
/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Initializes the TAL
 *
 * This function is called to initialize the TAL. The transceiver is
 * initialized, the TAL PIBs are set to their default values, and the TAL state
 * machine is set to TAL_IDLE state.
 *
 * @return MAC_SUCCESS  if the transceiver state is changed to TRX_OFF and the
 *                 current device part number and version number are correct;
 *         FAILURE otherwise
 */
retval_t tal_init(void)
{
    debug_text(PSTR("tal_init()"));

    /* Init the PAL and by this means also the transceiver interface */
    if (pal_init() != MAC_SUCCESS)
    {
        return FAILURE;
    }

    /* Reset trx */
    if (trx_reset(RFBOTH) != MAC_SUCCESS)
    {
        return FAILURE;
    }
	
    /* Check if RF215 is connected */
    if ((pal_trx_reg_read(RG_RF_PN) != 0x34) ||
        (pal_trx_reg_read(RG_RF_VN) != 0x01))
    {
        return FAILURE;
    }

    /* Initialize trx */
    trx_init();

	if (tal_timer_init() != MAC_SUCCESS) {
		return FAILURE;
	}

    /* Initialize the buffer management */
    bmm_buffer_init();

    /* Configure both trx and set default PIB values */
    for (uint8_t trx_id = 0; trx_id < NO_TRX; trx_id++)
    {
        /* Configure transceiver */
        trx_config((trx_id_t)trx_id);

        /* Calibrate LO */
        calibrate_LO((trx_id_t)trx_id);

        /* Set the default PIB values */
        init_tal_pib((trx_id_t)trx_id); /* see 'tal_pib.c' */
        calculate_pib_values((trx_id_t)trx_id);

        /*
         * Write all PIB values to the transceiver
         * that are needed by the transceiver itself.
         */
        write_all_tal_pib_to_trx((trx_id_t)trx_id); /* see 'tal_pib.c' */
        config_phy((trx_id_t)trx_id);

        tal_rx_buffer[trx_id] = bmm_buffer_alloc(LARGE_BUFFER_SIZE);
        if (tal_rx_buffer[trx_id] == NULL)
        {
            return FAILURE;
        }

        /* Init incoming frame queue */
        qmm_queue_init(&tal_incoming_frame_queue[trx_id]);

        tal_state[trx_id] = TAL_IDLE;
        tx_state[trx_id] = TX_IDLE;
    }

    /* Init seed of rand() */
    tal_generate_rand_seed();

#ifndef DISABLE_IEEE_ADDR_CHECK
	for (uint8_t trx_id = 0; trx_id < 2; trx_id++)
	{
		/* Check if a valid IEEE address is available. */
		/*
		 * This while loop is on purpose, since just in the
		 * rare case that such an address is randomly
		 * generated again, we must repeat this.
		 */
		while ((tal_pib[trx_id].IeeeAddress == 0x0000000000000000) ||
			   (tal_pib[trx_id].IeeeAddress == 0xFFFFFFFFFFFFFFFF))
		{
			/*
			 * In case no valid IEEE address is available, a random
			 * IEEE address will be generated to be able to run the
			 * applications for demonstration purposes.
			 * In production code this can be omitted.
			 */
			/*
			 * The proper seed for function rand() has already been generated
			 * in function tal_generate_rand_seed().
			 */
			uint8_t *ptr_pib = (uint8_t *)&tal_pib[trx_id].IeeeAddress;

			for (uint8_t i = 0; i < 8; i++)
			{
				*ptr_pib++ = (uint8_t)rand();
				/*
				 * Note:
				 * Even if casting the 16 bit rand value back to 8 bit,
				 * and running the loop 8 timers (instead of only 4 times)
				 * may look cumbersome, it turns out that the code gets
				 * smaller using 8-bit here.
				 * And timing is not an issue at this place...
				 */
			}
		}
	}
#endif

#ifdef IQ_RADIO
    /* Init BB IRQ handler */
    pal_trx_irq_flag_clr(RF215_BB);
    pal_trx_irq_init(RF215_BB, bb_irq_handler_cb);
    pal_trx_irq_en(RF215_BB);

    /* Init RF IRQ handler */
    pal_trx_irq_flag_clr(RF215_RF);
    pal_trx_irq_init(RF215_RF, rf_irq_handler_cb);
    pal_trx_irq_en(RF215_RF);
#else
    /*
     * Configure interrupt handling.
     * Install a handler for the radio and the baseband interrupt.
     */
    pal_trx_irq_flag_clr();
    pal_trx_irq_init(trx_irq_handler_cb);
    pal_trx_irq_en();   /* Enable transceiver main interrupt. */
#endif

#if ((defined SUPPORT_FSK) && (defined SUPPORT_MODE_SWITCH))
    init_mode_switch();
#endif

    return MAC_SUCCESS;
} /* tal_init() */


/**
 * @brief Configures the transceiver
 *
 * This function is called to configure a certain transceiver (RF09 or RF24)
 * after trx sleep or reset or power on.
 *
 * @param trx_id Transceiver identifier
 */
void trx_config(trx_id_t trx_id)
{
    debug_text_val(PSTR("trx_config(), trx_id ="), trx_id);

    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

#ifdef IQ_RADIO
    /* LVDS interface */
    // RF part: RF enable, BB disabled, IQIF enabled
    pal_trx_bit_write(RF215_RF, SR_RF_IQIFC1_CHPM, 0x01);
    // BB part: RF disable, BB enabled, IQIF enabled
    pal_trx_bit_write(RF215_BB, SR_RF_IQIFC1_CHPM, 0x03);
    // Clock Phase I/Q IF Driver at BB
    pal_trx_bit_write(RF215_BB, SR_RF_IQIFC2_CPHADRV, 0);
    // Clock Phase I/Q IF Receiver at BB
    pal_trx_bit_write(RF215_BB, SR_RF_IQIFC2_CPHAREC, 1);
    // Enable embedded control at RF
    pal_trx_bit_write(RF215_RF, SR_RF_IQIFC0_EEC, 1);
#if (BOARD_TYPE == EVAL215_FPGA)
    pal_trx_bit_write(RF215_RF, SR_RF_IQIFC0_CMV1V2, 1);
    uint8_t temp = pal_trx_reg_read(RF215_BB, RG_RF_IQIFC1);
    temp = (temp & 0xF3) | (1 << 2); // SKEWREC = 0ns
    pal_trx_reg_write(RF215_BB, RG_RF_IQIFC1, temp);
#endif
    /* Configure BB */
    /* Setup IRQ mask: in chip mode, the baseband controls the RF's AGC */
    pal_trx_reg_write(RF215_BB, reg_offset + RG_BBC0_IRQM, BB_IRQ_ALL_IRQ);
    pal_trx_reg_write(RF215_BB, reg_offset + RG_RF09_IRQM, RF_IRQ_ALL_IRQ);
    /* Configure RF */
    pal_trx_reg_write(RF215_RF, reg_offset + RG_BBC0_IRQM, BB_IRQ_ALL_IRQ);
    pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_IRQM, RF_IRQ_ALL_IRQ);
#else
    /* Configure BB */
    /* Setup IRQ mask */
    pal_trx_reg_write(reg_offset + RG_BBC0_IRQM, TAL_DEFAULT_BB_IRQ_MASK);
    /* Configure RF */
    pal_trx_reg_write(reg_offset + RG_RF09_IRQM, TAL_DEFAULT_RF_IRQ_MASK);
#endif

#if (defined IQ_RADIO) && (BOARD_TYPE == EVAL215_FPGA)
    /* Set clip detector OFF */
    uint8_t agcc = pal_trx_reg_read(RF215_RF, reg_offset + RG_RF09_AGCC);
    agcc |= 0x80;
    pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_AGCC, agcc);
#endif

    /* Enable frame filter */
    pal_trx_bit_write(reg_offset + SR_BBC0_AFC0_AFEN0, 1);

#ifndef BASIC_MODE
#if (defined MEASURE_TIME_OF_FLIGHT) && (!defined IQ_RADIO)
    /* Enable automatic time of flight measurement */
    /* bit 3 CAPRXS, bit 2 RSTTXS, bit 0 EN */
    uint8_t cnt_cfg = 1 << 0 | 1 << 2 | 1 << 3;
    pal_trx_reg_write(reg_offset + RG_BBC0_CNTC, cnt_cfg);
#endif /* #if (defined MEASURE_TIME_OF_FLIGHT) && (!defined IQ_RADIO) */
#else // BASIC_MODE
    /* Enable counter for ACK timing: EN | RSTRXS */
    uint8_t cntc = 0x01 | 0x02;
    pal_trx_reg_write(reg_offset + RG_BBC0_CNTC, cntc);
#endif

#ifndef USE_TXPREP_DURING_BACKOFF
    /* Keep analog voltage regulator on during TRXOFF */
#ifdef IQ_RADIO
    pal_trx_bit_write(RF215_RF, reg_offset + SR_RF09_AUXS_AVEN, 1);
#else
    pal_trx_bit_write(reg_offset + SR_RF09_AUXS_AVEN, 1);
#endif /* #ifdef IQ_RADIO */
#endif

#ifndef BASIC_MODE
    /* Enable AACK */
    pal_trx_reg_write(reg_offset + RG_BBC0_AMCS, AMCS_AACK_MASK);
    /* Set data pending for ACK frames to 1 for all address units */
    pal_trx_reg_write(reg_offset + RG_BBC0_AMAACKPD, 0x0F);
#endif

#ifdef SUPPORT_MODE_SWITCH
    /* Use raw mode for mode switch PPDU in the not-inverted manner */
    pal_trx_bit_write(reg_offset + SR_BBC0_FSKC4_RAWRBIT, 0);
#endif

    /* Workaround for errata reference #4623 */
    if (trx_id == RF09)
    {
        pal_trx_reg_write(0x129, 0x04);
    }
}


/**
 * @brief Initializes the transceiver
 *
 * This function is called to initialize the general transceiver functionality
 * after power or IC reset.
 */
static void trx_init(void)
{
    debug_text(PSTR("trx_init()"));

    /*
     * Configure generic trx functionality
     * Configure Trx registers that are reset during sleep/reset
     * I.e.: RF_CFG, RF_BMDVC, RF_XOC, RF_IQIFC0, RF_IQIFC1, RF_IQIFC2
     */
#ifdef TRX_IRQ_POLARITY
#if (TRX_IRQ_POLARITY == 0)
    pal_trx_bit_write(SR_RF_CFG_IRQP, 0);
#endif
#endif
#if (TRX_CLOCK_OUTPUT_SELECTION != 1)
#ifdef IQ_RADIO
    pal_trx_bit_write(RF215_RF, SR_RF_CLKO_OS, TRX_CLOCK_OUTPUT_SELECTION);
#else
    pal_trx_bit_write(SR_RF_CLKO_OS, TRX_CLOCK_OUTPUT_SELECTION);
#endif
#endif
}


/**
 * @brief Resets TAL state machine and sets the default PIB values if requested
 *
 * @param trx_id Transceiver identifier
 * @param set_default_pib Defines whether PIB values need to be set
 *                        to its default values
 *
 * @return
 *      - @ref MAC_SUCCESS if the transceiver state is changed to TRX_OFF
 *      - @ref FAILURE otherwise
 * @ingroup apiTalApi
 */
retval_t tal_reset(trx_id_t trx_id, bool set_default_pib)
{
    rf_cmd_state_t previous_trx_state[NO_TRX];

    debug_text_val(PSTR("tal_reset(), trx_id ="), trx_id);

    previous_trx_state[RF09] = trx_state[RF09];
    previous_trx_state[RF24] = trx_state[RF24];

    /* Reset the actual device or part of the device */
    if (trx_reset(trx_id) != MAC_SUCCESS)
    {
        debug_text(PSTR("Reset FAILURE"));
        return FAILURE;
    }

    /* Init Trx if necessary, e.g. trx was in deep sleep */
    if (((previous_trx_state[RF09] == RF_SLEEP) &&
         (previous_trx_state[RF24] == RF_SLEEP)) || (trx_id == RFBOTH))
    {
        trx_init(); /* Initialize generic trx functionality */
    }

    if (trx_id == RFBOTH)
    {
        for (uint8_t i = 0; i < NO_TRX; i++)
        {
            /* Clean TAL and removed any pending tasks */
            cleanup_tal((trx_id_t)i);

            /* Configure the transceiver register values. */
            trx_config((trx_id_t)i);

            if (set_default_pib)
            {
                /* Set the default PIB values */
                init_tal_pib((trx_id_t)i); /* see 'tal_pib.c' */
                calculate_pib_values(trx_id);
            }
            else
            {
                /* nothing to do - the current TAL PIB attribute values are used */
            }
            write_all_tal_pib_to_trx((trx_id_t)i); /* see 'tal_pib.c' */
            config_phy((trx_id_t)i);

            /* Reset TAL variables. */
            tal_state[(trx_id_t)i] = TAL_IDLE;
            tx_state[(trx_id_t)i] = TX_IDLE;
#ifdef ENABLE_FTN_PLL_CALIBRATION
            /* Stop FTN timer */
            stop_ftn_timer((trx_id_t)i);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */
        }
    }
    else
    {
        /* Maintain other trx */
        trx_id_t other_trx_id;
        if (trx_id == RF09)
        {
            other_trx_id = RF24;
        }
        else
        {
            other_trx_id = RF09;
        }
        if (tal_state[other_trx_id] == TAL_SLEEP)
        {
            /* Switch other trx back to sleep again */
            uint16_t reg_offset = RF_BASE_ADDR_OFFSET * other_trx_id;
#ifdef IQ_RADIO
            pal_trx_reg_write(RF215_BB, reg_offset + RG_RF09_CMD, RF_SLEEP);
            pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_CMD, RF_SLEEP);
#else
            pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_SLEEP);
#endif
            TAL_RF_IRQ_CLR_ALL(trx_id);
        }

        /* Clean TAL and removed any pending tasks */
        cleanup_tal(trx_id);

        /* Configure the transceiver register values. */
        trx_config(trx_id);

        if (set_default_pib)
        {
            /* Set the default PIB values */
            init_tal_pib(trx_id); /* see 'tal_pib.c' */
            calculate_pib_values(trx_id);
        }
        else
        {
            /* nothing to do - the current TAL PIB attribute values are used */
        }
        write_all_tal_pib_to_trx(trx_id); /* see 'tal_pib.c' */
        config_phy(trx_id);

        /* Reset TAL variables. */
        tal_state[trx_id] = TAL_IDLE;
        tx_state[trx_id] = TX_IDLE;
#ifdef ENABLE_FTN_PLL_CALIBRATION
        /* Stop FTN timer */
        stop_ftn_timer(trx_id);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */
    }

    /*
     * Configure interrupt handling.
     * Install a handler for the transceiver interrupt.
     */
#ifdef IQ_RADIO
    pal_trx_irq_init(RF215_BB, bb_irq_handler_cb);
    pal_trx_irq_init(RF215_RF, rf_irq_handler_cb);
    pal_trx_irq_en(RF215_BB);   /* Enable transceiver main interrupt. */
    pal_trx_irq_en(RF215_RF);   /* Enable transceiver main interrupt. */
#else
    pal_trx_irq_init(trx_irq_handler_cb);
    pal_trx_irq_en();   /* Enable transceiver main interrupt. */
#endif

    return MAC_SUCCESS;
}


/**
 * @brief Resets transceiver(s)
 *
 * @param trx_id Transceiver identifier
 *
 * @return MAC_SUCCESS  if the transceiver returns TRX_OFF
 *         FAILURE otherwise
 */
retval_t trx_reset(trx_id_t trx_id)
{
    debug_text_val(PSTR("trx_reset(), trx_id ="), trx_id);

    ENTER_TRX_REGION();

    uint32_t start_time;
    uint32_t current_time;
    pal_get_current_time(&start_time);

    if (trx_id == RFBOTH)
    {
        TAL_RF_IRQ_CLR_ALL(RF09);
        TAL_RF_IRQ_CLR_ALL(RF24);
        tal_state[RF09] = TAL_RESET;
        tal_state[RF24] = TAL_RESET;

        /* Apply reset pulse; low active */
#ifdef IQ_RADIO
        RST_LOW();
        PAL_WAIT_1_US();
        PAL_WAIT_1_US();
        RST_HIGH();
#if (BOARD_TYPE == EVAL215_FPGA)
        pal_timer_delay(10000);
#endif
        RST_LOW();
        PAL_WAIT_1_US();
        RST_HIGH();
#else
        RST_LOW();
        PAL_WAIT_1_US();
        RST_HIGH();
#endif

        /* Wait for IRQ line */
        while (1)
        {
            /*
             * @ToDo: Use a different macro for IRQ line; the polarity might be
             * different after reset
             */
#ifdef IQ_RADIO
            if ((PAL_DEV_IRQ_GET(RF215_BB) == HIGH) &&
                (PAL_DEV_IRQ_GET(RF215_RF) == HIGH))
            {
                debug_text(PSTR("BB and RF interrupts detected"));
                break;
            }
#else
            if (PAL_TRX_IRQ_GET() == HIGH)
            {
                break;
            }
#endif

            /* Handle timeout */
            pal_get_current_time(&current_time);
            // @ToDo: Remove magic number
            if (pal_sub_time_us(current_time, start_time) > 1000)
            {
                debug_text_val(PSTR("long start up duration = "),
                               (uint16_t)(current_time - start_time));
#if (BOARD_TYPE == EVAL215_FPGA)
                printf("\nReset failed - long start up duration\n");
#endif
                return FAILURE;
            }
        }
#ifdef IQ_RADIO
        trx_state[RF09] = (rf_cmd_state_t)pal_trx_reg_read(RF215_RF, RG_RF09_STATE);
        trx_state[RF24] = (rf_cmd_state_t)pal_trx_reg_read(RF215_RF, RG_RF24_STATE);
        rf_cmd_state_t bb_trx_state[2];
        bb_trx_state[RF09] = (rf_cmd_state_t)pal_trx_reg_read(RF215_BB, RG_RF09_STATE);
        bb_trx_state[RF24] = (rf_cmd_state_t)pal_trx_reg_read(RF215_BB, RG_RF24_STATE);
        if ((bb_trx_state[RF09] != RF_TRXOFF) || (bb_trx_state[RF24] != RF_TRXOFF))
        {
            return FAILURE;
        }
#else
        trx_state[RF09] = (rf_cmd_state_t)pal_trx_reg_read(RG_RF09_STATE);
        trx_state[RF24] = (rf_cmd_state_t)pal_trx_reg_read(RG_RF24_STATE);
#endif
        if ((trx_state[RF09] != RF_TRXOFF) || (trx_state[RF24] != RF_TRXOFF))
        {
            return FAILURE;
        }
        /* Get all IRQ status information */
#ifdef IQ_RADIO
        bb_irq_handler_cb();
        rf_irq_handler_cb();
#else
        trx_irq_handler_cb();
#endif
        TAL_RF_IRQ_CLR(RF09, RF_IRQ_WAKEUP);
        TAL_RF_IRQ_CLR(RF24, RF_IRQ_WAKEUP);
    }
    else
    {
        TAL_RF_IRQ_CLR_ALL(trx_id);
        tal_state[trx_id] = TAL_RESET;

        /* Trigger reset of device */
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        debug_text(PSTR("Trigger trx reset"));
#ifdef IQ_RADIO
        pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_CMD, RF_RESET);
        pal_trx_reg_write(RF215_BB, reg_offset + RG_RF09_CMD, RF_RESET);
#else
        pal_trx_reg_write(reg_offset + RG_RF09_CMD, RF_RESET);
#endif

        /* Wait for IRQ line */
        while (1)
        {
#ifdef IQ_RADIO
            if ((PAL_DEV_IRQ_GET(RF215_BB) == HIGH) &&
                (PAL_DEV_IRQ_GET(RF215_RF) == HIGH))
            {
                debug_text(PSTR("BB and RF interrupts detected"));
                break;
            }
#else
            if (PAL_TRX_IRQ_GET() == HIGH)
            {
                break;
            }
#endif

            /* Handle timeout */
            pal_get_current_time(&current_time);
            // @ToDo: Remove magic number
            if (pal_sub_time_us(current_time, start_time) > 1000)
            {
                debug_text_val(PSTR("long start up duration = "),
                               (uint16_t)(current_time - start_time));
                return FAILURE;
            }
        }
        trx_state[trx_id] = RF_TRXOFF;
        /* Get all IRQ status information */
#ifdef IQ_RADIO
        bb_irq_handler_cb();
        rf_irq_handler_cb();
#else
        trx_irq_handler_cb();
#endif
        TAL_RF_IRQ_CLR(trx_id, RF_IRQ_WAKEUP);
    }

#ifdef IQ_RADIO
    pal_trx_irq_flag_clr(RF215_BB);
    pal_trx_irq_flag_clr(RF215_RF);
#else
    pal_trx_irq_flag_clr();
#endif
    LEAVE_TRX_REGION();

    return MAC_SUCCESS;
}


/**
 * @brief Cleanup TAL
 *
 * @param trx_id Transceiver identifier
 */
static void cleanup_tal(trx_id_t trx_id)
{
    debug_text(PSTR("cleanup_tal()"));

    /* Clear all running TAL timers. */
    ENTER_CRITICAL_REGION();
    stop_tal_timer(trx_id);
    LEAVE_CRITICAL_REGION();

    /* Clear TAL Incoming Frame queue and free used buffers. */
    while (tal_incoming_frame_queue[trx_id].size > 0)
    {
        buffer_t *frame = qmm_queue_remove(&tal_incoming_frame_queue[trx_id], NULL);
        if (NULL != frame)
        {
            bmm_buffer_free(frame);
        }
    }
    /* Get new TAL Rx buffer if necessary */
    if (tal_rx_buffer[trx_id] == NULL)
    {
        tal_rx_buffer[trx_id] = bmm_buffer_alloc(LARGE_BUFFER_SIZE);
    }
    /* Handle buffer shortage */
    if (tal_rx_buffer[trx_id] == NULL)
    {
        tal_buf_shortage[trx_id] = true;
    }
    else
    {
        tal_buf_shortage[trx_id] = false;
    }
}

static retval_t tal_timer_init(void)
{

	if (MAC_SUCCESS != pal_timer_get_id(&TAL_T_0)) {
		return FAILURE;
	}

	if (MAC_SUCCESS != pal_timer_get_id(&TAL_T_1)) {
		return FAILURE;
	}
	
	#ifdef ENABLE_FTN_PLL_CALIBRATION
	if (MAC_SUCCESS != pal_timer_get_id(&TAL_T_CALIBRATION_0)) {
		return FAILURE;
	}

	if (MAC_SUCCESS != pal_timer_get_id(&TAL_T_CALIBRATION_1)) {
		return FAILURE;
	}
	#endif  /* ENABLE_FTN_PLL_CALIBRATION */

	return MAC_SUCCESS;
}

static void tal_timers_stop(void)
{
	#if (NUMBER_OF_TAL_TIMERS > 0)
	pal_timer_stop(TAL_T_0);
	pal_timer_stop(TAL_T_1);
	#ifdef ENABLE_FTN_PLL_CALIBRATION
	pal_timer_stop(TAL_T_CALIBRATION_0);
	pal_timer_stop(TAL_T_CALIBRATION_1);
	#endif
	#endif /*  (NUMBER_OF_TAL_TIMERS > 0) */
}
/* EOF */
