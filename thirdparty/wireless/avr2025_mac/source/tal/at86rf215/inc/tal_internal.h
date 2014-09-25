/**
 * @file tal_internal.h
 *
 * @brief This header file contains types and variable definition that are used
 * within the TAL only.
 *
 * $Id: tal_internal.h 36440 2014-09-01 14:20:37Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
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
#ifdef IQ_RADIO
#    include "at86rf215_internal.h"
#endif

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
    TAL_TX,
    TAL_ED_SCAN
#if (defined ENABLE_TFA) || (defined TFA_ED_SAMPLE) || (defined CW_SUPPORTED)
    ,
    TAL_TFA_CW_RX,
    TAL_TFA_CW,
    TAL_TFA_CCA
#endif
#ifdef SUPPORT_MODE_SWITCH
    ,
    TAL_NEW_MODE_RECEIVING
#endif
} SHORTENUM tal_state_t;

/**
 * Enumeration for tx_state
 */
typedef enum tx_state_tag
{
    TX_IDLE,
    TX_BACKOFF,
    TX_CCATX,
    TX_TX,
    TX_WAITING_FOR_ACK
#ifdef RX_WHILE_BACKOFF
    ,
    TX_DEFER
#endif
#if (defined BASIC_MODE) || (defined SUPPORT_MODE_SWITCH)
    ,
    TX_CCA
#endif
#ifdef SUPPORT_MODE_SWITCH
    ,
    TX_MS_PPDU,
    TX_WAIT_FOR_NEW_MODE_TRANSMITTING,
    TX_MS_NEW_MODE_TRANSMITTING
#endif
} SHORTENUM tx_state_t;

/**
 * Enumeration use as parameter for function transmit_frame()
 */
typedef enum cca_use_tag
{
    NO_CCA,
    WITH_CCA
} SHORTENUM cca_use_t;

/* === EXTERNALS =========================================================== */

/* Global TAL variables */
extern tal_state_t tal_state[2];
extern tx_state_t tx_state[2];
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
extern uint8_t txc[2][2];
extern bool frame_buf_filled[2];
#if (defined ENABLE_TSTAMP) || (defined MEASURE_ON_AIR_DURATION)
extern uint32_t fs_tstamp[2];
#endif
extern frame_info_t *rx_frm_info[2];
#ifdef BASIC_MODE
extern uint8_t *rx_frm_ptr[2];
extern uint16_t last_txframe_length[2];
#endif
#ifdef SUPPORT_MODE_SWITCH
extern bool csm_active[2];
#endif

/* === MACROS ============================================================== */

#ifdef ENABLE_QUEUE_CAPACITY
#define TAL_INCOMING_FRAME_QUEUE_CAPACITY   (255)
#endif  /* ENABLE_QUEUE_CAPACITY */

#ifdef ENABLE_TSTAMP
#define TAL_DEFAULT_BB_IRQ_MASK     (BB_IRQ_TXFE | BB_IRQ_RXFE | BB_IRQ_RXFS)
#else
#define TAL_DEFAULT_BB_IRQ_MASK     (BB_IRQ_TXFE | BB_IRQ_RXFE)
#endif
#ifdef BASIC_MODE
#define TAL_DEFAULT_RF_IRQ_MASK     RF_IRQ_ALL_IRQ
#else
#define TAL_DEFAULT_RF_IRQ_MASK     (RF_IRQ_IQIFSF | RF_IRQ_TRXERR | \
                                     RF_IRQ_BATLOW | RF_IRQ_WAKEUP)
#endif

/* === PROTOTYPES ========================================================== */


/*
 * Prototypes from tal.c
 */
void switch_to_rx(trx_id_t trx_id);
void switch_to_txprep(trx_id_t trx_id);
void wait_for_txprep(trx_id_t trx_id);
void stop_tal_timer(trx_id_t trx_id);

rf_cmd_status_t set_trx_state(trx_id_t trx,rf_cmd_state_t trx_cmd);

/*
 * Prototypes from tal_ftn.c
 */
#ifdef ENABLE_FTN_PLL_CALIBRATION
void start_ftn_timer(trx_id_t trx_id);
void stop_ftn_timer(trx_id_t trx_id);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */
void calibrate_LO(trx_id_t trx_id);

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
#if (defined SUPPORT_FSK) && ((defined SUPPORT_FSK_RAW_MODE) || (defined SUPPORT_MODE_SWITCH))
void configure_raw_mode(trx_id_t trx_id, bool enable);
#endif

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
uint16_t get_cca_duration_us(trx_id_t trx_id);
int8_t get_cca_thres(trx_id_t trx_id);
uint16_t ceiling_sym(trx_id_t trx_id, uint16_t duration_us);
uint16_t get_AckWaitDuration_us(trx_id_t trx_id);
uint16_t get_AckTiming_us(trx_id_t trx_id);
void get_ch_freq0_spacing(trx_id_t trx_id, uint32_t *freq, uint32_t *spacing);
uint8_t phr_duration_sym(trx_id_t trx_id);
uint16_t calculate_cca_duration_us(trx_id_t trx_id);
float get_data_rate(trx_id_t trx_id);
uint8_t shr_duration_sym(trx_id_t trx_id);
#if (!defined BASIC_MODE) && (defined MEASURE_ON_AIR_DURATION)
uint16_t get_ack_duration_sym(trx_id_t trx_id);
#endif
retval_t get_supported_channels_tuple(trx_id_t trx_id, uint32_t *value);

/*
 * Prototypes from tal_phy_cfg.c
 */
retval_t conf_trx_modulation(trx_id_t trx_id);
#ifdef SUPPORT_FSK
void set_sfd(trx_id_t trx_id);
void set_fsk_pibs(trx_id_t trx_id);
retval_t conf_fsk_data_rate(trx_id_t trx_id, fsk_data_rate_t rate);
#ifdef SUPPORT_MODE_SWITCH
retval_t conf_fsk(trx_id_t trx_id);
#endif
#endif

/*
 * Prototypes from tal_fe.c
 */
#ifdef SUPPORT_FSK
retval_t fsk_rfcfg(fsk_data_rate_t srate, mod_idx_t mod_idx, trx_id_t trx_id);
#endif
#ifdef SUPPORT_OFDM
retval_t ofdm_rfcfg(ofdm_option_t ofdm_opt, trx_id_t trx_id);
int8_t get_max_ofdm_tx_pwr(trx_id_t trx_id, ofdm_mcs_t mcs);
#endif
#if (defined SUPPORT_OQPSK) || (defined SUPPORT_LEGACY_OQPSK)
retval_t oqpsk_rfcfg(oqpsk_chip_rate_t chip_rate, trx_id_t trx_id);
#endif

/*
 * Prototypes from tal_mode_switch.c
 */
#ifdef SUPPORT_MODE_SWITCH
void init_mode_switch(void);
void prepare_actual_transmission(trx_id_t trx_id);
void restore_previous_phy(trx_id_t trx_id);
void set_csm(trx_id_t trx_id);
void tx_ms_ppdu(trx_id_t trx_id);
void handle_rx_ms_packet(trx_id_t trx_id);
void save_current_phy(trx_id_t trx_id);
#endif /* #ifdef SUPPORT_MODE_SWITCH */

/*
 * Prototypes from tal_rx.c
 */
#ifdef BASIC_MODE
void complete_rx_transaction(trx_id_t trx_id);
void upload_frame(trx_id_t trx_id);
void handle_rx_end_irq(trx_id_t trx_id);
void process_incoming_frame(trx_id_t trx_id, buffer_t *buf);
void ack_transmission_done(trx_id_t trx_id);
#endif

/*
 * Prototypes from tal_auto_rx.c
 */
#ifndef BASIC_MODE
void complete_rx_transaction(trx_id_t trx_id);
void handle_rx_end_irq(trx_id_t trx_id);
void process_incoming_frame(trx_id_t trx_id, buffer_t *buf);
void ack_transmission_done(trx_id_t trx_id);
#endif

/*
 * Prototypes from tal_tx.c
 */
#ifdef BASIC_MODE
void transmit_frame(trx_id_t trx_id);
void handle_tx_end_irq(trx_id_t trx_id);
void tx_done_handling(trx_id_t trx_id, retval_t status);
#endif

/*
 * Prototypes from tal_auto_tx.c
 */
#ifndef BASIC_MODE
void transmit_frame(trx_id_t trx_id, cca_use_t cca);
void handle_tx_end_irq(trx_id_t trx_id);
void tx_done_handling(trx_id_t trx_id, retval_t status);
#endif

/*
 * Prototypes from tal_csma.c
 */
#ifdef BASIC_MODE
void cca_done_handling(trx_id_t trx_id);
void csma_start(trx_id_t trx_id);
void csma_continue(trx_id_t trx_id);
#endif

/*
 * Prototypes from tal_auto_csma.c
 */
#ifndef BASIC_MODE
void cca_done_handling(trx_id_t trx_id);
void csma_start(trx_id_t trx_id);
void csma_continue(trx_id_t trx_id);
#endif

/*
 * Prototypes from tal_ack.c
 */
#ifdef BASIC_MODE
bool is_ack_required(trx_id_t trx_id);
bool is_frame_an_ack(trx_id_t trx_id);
void schedule_ack_transmission(trx_id_t trx_id);
bool handle_ack_reception(trx_id_t trx_id);
void ack_timout_cb(void *parameter);
#endif

/*
 * Prototypes from tal_auto_ack.c
 */
#ifndef BASIC_MODE
bool is_frame_an_ack(trx_id_t trx_id);
void start_ack_wait_timer(trx_id_t trx_id);
bool is_ack_valid(trx_id_t trx_id);
#endif

/*
 * Prototypes from tal_irq_handler.c
 */
void trx_irq_handler_cb(void);
#ifdef IQ_RADIO
void rf_irq_handler_cb(void);
void bb_irq_handler_cb(void);
#endif
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
