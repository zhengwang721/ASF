/**
 * @file tal_internal.h
 *
 * @brief This header file contains types and variable definition that are used
 * within the TAL only.
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 *
 *
 */

/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef TAL_INTERNAL_H
#define TAL_INTERNAL_H

/* === INCLUDES ============================================================ */

#include "tal_config.h"
#include "bmm.h"
#include "qmm.h"
#include "mac_build_config.h"
#include "tal.h"
#include "tal_rf215.h"

/* === TYPES =============================================================== */

/**
 * Enumeration for tal_state
 */
typedef enum tal_state_tag
{
    TAL_IDLE,
    TAL_SLEEP,
    TAL_RESET,
    TAL_WAKING_UP,
    /* Rx transaction */
    TAL_WAITING_FOR_ACK_TRANSMISION,
    TAL_ACK_TRANSMITTING,
    /* Tx transaction */
    TAL_BACKOFF,
    TAL_CCA,
    TAL_TX,
    TAL_WAITING_FOR_ACK_RECEPTION,
    /* Other transaction */
    TAL_ED_SCAN
#if (defined ENABLE_TFA) || (defined TFA_ED_SAMPLE) || (defined CW_SUPPORTED)
    ,
    TAL_TFA_CW_RX,
    TAL_TFA_CW
#endif
} SHORTENUM tal_state_t;

/* === EXTERNALS =========================================================== */

/* Global TAL variables */
extern tal_state_t tal_state[2];
extern const uint8_t timer_cb_parameter[2];
extern int8_t tal_current_ed_val[2];
extern frame_info_t *mac_frame_ptr[2];
extern queue_t tal_incoming_frame_queue[2];
extern uint8_t *tal_frame_to_tx[2];
extern buffer_t *tal_rx_buffer[2];
extern bool tal_buf_shortage[2];
extern rf_cmd_state_t trx_state[2];
extern rf_cmd_state_t trx_default_state[2];
extern uint32_t rxe_txe_tstamp[2];
#ifdef RX_WHILE_BACKOFF
extern bool csma_backoff_onhold[2];
#endif
#if (defined ENABLE_TSTAMP) || (defined MEASURE_ON_AIR_DURATION)
extern uint32_t fs_tstamp[2];
#endif
extern frame_info_t *rx_frm_info[2];
extern uint8_t *rx_frm_ptr[2];

/* === MACROS ============================================================== */

#ifdef ENABLE_QUEUE_CAPACITY
#define TAL_INCOMING_FRAME_QUEUE_CAPACITY   (255)
#endif  /* ENABLE_QUEUE_CAPACITY */

#ifdef ENABLE_TSTAMP
#define TAL_DEFAULT_BB_IRQ_MASK     (BB_IRQ_TXFE | BB_IRQ_RXFE | BB_IRQ_RXFS)
#else
#define TAL_DEFAULT_BB_IRQ_MASK     (BB_IRQ_TXFE | BB_IRQ_RXFE)
#endif
#define TAL_DEFAULT_RF_IRQ_MASK     RF_IRQ_ALL_IRQ

/* === PROTOTYPES ========================================================== */


/*
 * Prototypes from tal.c
 */
void switch_to_rx(trx_id_t trx_id);
void switch_to_txprep(trx_id_t trx_id);
void stop_tal_timer(trx_id_t trx_id);

rf_cmd_status_t set_trx_state(trx_id_t trx,rf_cmd_state_t trx_cmd);
/*
 * Prototypes from tal_ftn.c
 */
#ifdef ENABLE_FTN_PLL_CALIBRATION
void start_ftn_timer(trx_id_t trx_id);
void stop_ftn_timer(trx_id_t trx_id);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */

/*
 * Prototypes from tal_init.c
 */
void trx_config(trx_id_t trx_id);

/*
 * Prototypes from tal_pib.c
 */
void calculate_pib_values(trx_id_t trx_id);
retval_t config_phy(trx_id_t trx_id);
void init_tal_pib(trx_id_t trx_id);
void write_all_tal_pib_to_trx(trx_id_t trx_id);

/*
 * Prototypes from tal_ed.c
 */
void handle_ed_end_irq(trx_id_t trx_id);
void set_ed_sample_duration(trx_id_t trx_id, uint16_t sample_duration_us);
uint8_t scale_ed_value(int8_t ed);
void stop_ed_scan(trx_id_t trx_id);

/*
 * Prototypes from tal_4g_utils.c
 */
uint16_t get_symbol_duration_us(trx_id_t trx_id);
uint16_t get_cca_duration_us(trx_id_t trx_id);
bool is_freq_band_supported(trx_id_t trx_id, phy_t *phy);
int8_t get_cca_thres(trx_id_t trx_id);
uint16_t ceiling_sym(trx_id_t trx_id, uint16_t duration_us);
uint16_t get_AckWaitDuration_us(trx_id_t trx_id);
uint16_t get_AckTiming_us(trx_id_t trx_id);
void get_ch_freq0_spacing(trx_id_t trx_id, uint32_t *freq, uint32_t *spacing);
uint8_t phr_duration_sym(trx_id_t trx_id);
uint16_t oqpsk_get_chip_rate(trx_id_t trx_id);
uint16_t calculate_cca_duration_us(trx_id_t trx_id);
uint16_t get_data_rate(trx_id_t trx_id);
uint8_t shr_duration_sym(trx_id_t trx_id);

/*
 * Prototypes from tal_phy_cfg.c
 */
retval_t conf_trx_modulation(trx_id_t trx_id);

/*
 * Prototypes from tal_rx.c
 */
void complete_rx_transaction(trx_id_t trx_id);
void upload_frame(trx_id_t trx_id);
void handle_rx_end_irq(trx_id_t trx_id);
void process_incoming_frame(trx_id_t trx_id, buffer_t *buf);
void ack_transmission_done(trx_id_t trx_id);

/*
 * Prototypes from tal_tx.c
 */
void tal_transmit_frame(trx_id_t trx_id);
void handle_tx_end_irq(trx_id_t trx_id);
void tx_done_handling(trx_id_t trx_id, retval_t status);

/*
 * Prototypes from tal_csma.c
 */
void cca_done_handling(trx_id_t trx_id);
void csma_start(trx_id_t trx_id);
void csma_continue(trx_id_t trx_id);
void trigger_cca_meaurement(trx_id_t trx_id);

/*
 * Prototypes from tal_ack.c
 */
bool is_ack_required(trx_id_t trx_id);
void schedule_ack_transmission(trx_id_t trx_id);
bool handle_ack_reception(trx_id_t trx_id);
void ack_timout_cb(void *parameter);

/*
 * Prototypes from tal_irq_handler.c
 */
void trx_irq_handler_cb(void);
#if (defined ENABLE_TSTAMP) || (defined DOXYGEN)
void trx_irq_timestamp_handler_cb(void);
#endif  /* #if (defined ENABLE_TSTAMP) || (defined DOXYGEN) */

/*
 * Prototypes from tfa_batmon.c
 */
#if (defined ENABLE_TFA) || (defined TFA_BAT_MON) || (defined TFA_BAT_MON_IRQ)
void handle_batmon_irq(void);
#endif

#endif /* TAL_INTERNAL_H */
