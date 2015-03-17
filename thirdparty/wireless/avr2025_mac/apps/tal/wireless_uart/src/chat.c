/**
 * \file chat.c
 *
 * \brief Chat feature handling
 *
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
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
 * Copyright(c) 2013-2015, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include "tal.h"
#include "app_config.h"
#include "app_common.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/**
 * Define maximum charcter input length
 */
#define MAX_INPUT_LENGTH    80

/* === EXTERNALS =========================================================== */

/* === GLOBALS ============================================================= */

frame_info_t *tx_frame;
uint8_t tx_buf[LARGE_BUFFER_SIZE];
uint8_t *tx_frm_pay_ptr;
static uint8_t *chat_pay_ptr;
static uint8_t input_len;
uint8_t tx_hdr_len;
#if (TAL_TYPE == AT86RF215)
uint8_t crc_len[2];
#endif

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Print chat welcome menu
 */
void print_chat_menu(void)
{
    CLEAR_SCREEN();

#if (TAL_TYPE == AT86RF215)
    for (uint8_t i = 0; i < 2; i++)
    {
        uint8_t fcs_type;
        tal_pib_get((trx_id_t)i, macFCSType, &fcs_type);
        if (fcs_type == FCS_TYPE_4_OCTETS)
        {
            crc_len[i] = 4;
        }
        else
        {
            crc_len[i] = 2;
        }
    }
#endif

    printf("Chat Application\n AT86RF215 SAM4LXplained Pro RF215-XPRO\n");
#ifdef MULTI_TRX_SUPPORT
    printf("Active transmitter: %s\n", get_trx_id_text(current_trx_id));
#endif

    printf("Press 'CTRL-s' for sub-1GHz or 'CTRL-d' for 2.4GHz.\n");
    printf("Enter text and press 'enter' to trigger transmission\n");
    printf("\n");
    printf("> ");
    fflush(stdout);
}


/**
 * @brief Initialize transmit frame header
 */
void init_tx_frame(void)
{
    uint8_t *ptr;

    tx_frame = (frame_info_t *)tx_buf;
#if (TAL_TYPE == AT86RF215)
    tx_frame->mpdu = (uint8_t *)tx_frame + LARGE_BUFFER_SIZE - aMaxPHYPacketSize_4g;
    ptr = &tx_frame->mpdu[0]; // start with fcf; length is set elsewhere
#else
    tx_frame->mpdu = (uint8_t *)tx_frame + LARGE_BUFFER_SIZE - aMaxPHYPacketSize - 1; // 1 = length field
    ptr = &tx_frame->mpdu[1]; // start with fcf; length is set elsewhere
#endif
    /* fcf0 */
    *ptr = FCF_FRAMETYPE_DATA;
#if (PEER_ACK_REQUEST == true)
    *ptr |= FCF_ACK_REQUEST;
#endif
    ptr++;
    /* fcf1: use short address mode */
    uint8_t dest_addr_mode = DEST_ADDR_MODE;
    uint8_t src_addr_mode = SRC_ADDR_MODE;
    *ptr++ = (dest_addr_mode << FCF_2_DEST_ADDR_OFFSET) | (src_addr_mode << FCF_2_SOURCE_ADDR_OFFSET);
    *ptr++ = 0;     // seq_no
    if (dest_addr_mode == FCF_SHORT_ADDR)
    {
        uint16_t dest_pan_id = PEER_PAN_ID;
        memcpy(ptr, &dest_pan_id, 2);
        ptr += 2;
        uint16_t dest_short_addr = PEER_SHORT_ADDR;
        memcpy(ptr, &dest_short_addr, 2);
        ptr += 2;
    }
    else if (dest_addr_mode == FCF_LONG_ADDR)
    {
        uint16_t dest_pan_id = PEER_PAN_ID;
        memcpy(ptr, &dest_pan_id, 2);
        ptr += 2;
        uint64_t peer_ieee_addr = PEER_IEEE_ADDR;
        memcpy(ptr, &peer_ieee_addr, 8);
        ptr += 8;
    }

    if (src_addr_mode == FCF_SHORT_ADDR)
    {
        uint16_t src_pan_id;
        uint16_t src_short_addr;
#ifdef MULTI_TRX_SUPPORT
        tal_pib_get(current_trx_id, macPANId, (uint8_t *)&src_pan_id);
        tal_pib_get(current_trx_id, macShortAddress, (uint8_t *)&src_short_addr);
#else
        tal_pib_get(macPANId, (uint8_t *)&src_pan_id);
        tal_pib_get(macShortAddress, (uint8_t *)&src_short_addr);
#endif
        memcpy(ptr, &src_pan_id, 2);
        ptr += 2;
        memcpy(ptr, &src_short_addr, 2);
        ptr += 2;
    }
    else if (src_addr_mode == FCF_LONG_ADDR)
    {
        uint16_t src_pan_id;
        uint8_t ieee_addr[8];
#ifdef MULTI_TRX_SUPPORT
        tal_pib_get(current_trx_id, macPANId, (uint8_t *)&src_pan_id);
        tal_pib_get(current_trx_id, macIeeeAddress, ieee_addr);
#else
        tal_pib_get(macPANId, (uint8_t *)&src_pan_id);
        tal_pib_get(macIeeeAddress, ieee_addr);
#endif
        memcpy(ptr, &src_pan_id, 2);
        ptr += 2;
        memcpy(ptr, &ieee_addr, 8);
        ptr += 8;
    }
    tx_hdr_len = ptr - tx_frame->mpdu;
    tx_frm_pay_ptr = ptr;
    chat_pay_ptr = tx_frm_pay_ptr;
}


/**
 * @brief Handle incoming frame
 *
 * @param trx_id Transceiver identifier
 * @parm rx_frame Pointer to frame_info_t structure
 */
void chat_handle_incoming_frame(trx_id_t trx_id, frame_info_t *rx_frame)
{
    uint8_t hdr_len;
    uint8_t *pay_ptr;

#if (TAL_TYPE == AT86RF215)
    uint16_t pay_len;
    uint8_t src_addr_mode = (rx_frame->mpdu[1] >> 6) & 0x03;
    uint8_t dest_addr_mode = (rx_frame->mpdu[1] >> 2) & 0x03;
    bool pan_id_comp = (rx_frame->mpdu[0] >> 6) & 0x01; // only dest pan id
    pay_ptr = (uint8_t *)&rx_frame->mpdu[0] + 3;
    int8_t dbm = rx_frame->mpdu[rx_frame->length + 1 + crc_len[trx_id]];
    printf("Rx (%s, %"PRIi8"dBm, from 0x", get_trx_id_text(trx_id), dbm);
#else
    uint8_t pay_len;
    uint8_t src_addr_mode = (rx_frame->mpdu[2] >> 6) & 0x03;
    uint8_t dest_addr_mode = (rx_frame->mpdu[2] >> 2) & 0x03;
    bool pan_id_comp = (rx_frame->mpdu[1] >> 6) & 0x01; // only dest pan id
    pay_ptr = (uint8_t *)&rx_frame->mpdu[0] + 4;
    printf("Rx (from 0x");
#endif

    if (dest_addr_mode == FCF_SHORT_ADDR)
    {
        pay_ptr += 4; // short addr and pan id
    }
    else if (dest_addr_mode == FCF_LONG_ADDR)
    {
        pay_ptr += 10; // long addr and pan id
    }

    if (!pan_id_comp)
    {
        pay_ptr += 2; // src pan id
    }

    if (src_addr_mode == FCF_SHORT_ADDR)
    {
        uint16_t src_addr;
        memcpy(&src_addr, pay_ptr, 2);
        printf("%.4"PRIX16"", src_addr);
        pay_ptr += 2; // short addr
    }
    else if (src_addr_mode == FCF_LONG_ADDR)
    {
        uint8_t src_addr[8];
        memcpy(src_addr, pay_ptr, 8);
        for (uint8_t i = 0; i < 8; i++)
        {
            printf("%.2"PRIX8"", src_addr[7 - i]);
        }
        pay_ptr += 8; // long addr
    }
    printf("): ");

    hdr_len = pay_ptr - (uint8_t *)&rx_frame->mpdu[0];
#if (TAL_TYPE == AT86RF215)
    pay_len = rx_frame->length - hdr_len;
#else
    pay_len = rx_frame->mpdu[0] - 2 - hdr_len;
#endif

    for (uint16_t i = 0; i < pay_len; i++)
    {
        if (isprint(*pay_ptr))
        {
            printf("%c", *pay_ptr);
        }
        else
        {
            printf(" ");
        }
        pay_ptr++;
    }

    printf("\n");
    printf("> ");
    fflush(stdout);

    /* Keep compiler happy */
    trx_id = trx_id;
}


/**
 * @brief Get input charater and store it to buffer. If frame is completed,
 *        transmit it.
 *
 * @param input Input character
 */
void get_chat_input(char input)
{
    if (app_state[current_trx_id] == APP_IDLE)
    {
        *chat_pay_ptr++ = input;
        input_len++;
        if ((input == NL) || (input == CR) || (input_len == MAX_INPUT_LENGTH))
        {
            app_state[current_trx_id] = APP_TX;
#if (TAL_TYPE == AT86RF215)
            tx_frame->length = input_len + tx_hdr_len;
            tx_frame->mpdu[PL_POS_SEQ_NUM - 1]++;
#else
            tx_frame->mpdu[0] = input_len + tx_hdr_len + 1;
            tx_frame->mpdu[PL_POS_SEQ_NUM]++;
#endif
#ifdef MULTI_TRX_SUPPORT
            tal_tx_frame(current_trx_id, tx_frame, CSMA_MODE, RETRANSMISSION_ENABLED);
#else
            tal_tx_frame(tx_frame, CSMA_MODE, RETRANSMISSION_ENABLED);
#endif
            input_len = 0;
        }
    }
}


/**
 * @brief Handle transmit done callback
 *
 * @param trx_id Transceiver identifier
 * @param status Status of the transmission
 * @param frame Pointer to frame_info_t structure
 */
void chat_tx_done_cb(trx_id_t trx_id, retval_t status, frame_info_t *frame)
{
    if (status != MAC_SUCCESS)
    {
        printf(STATUS_TEXT);
    }

    printf("\n> ");
    fflush(stdout);

    chat_pay_ptr = tx_frm_pay_ptr;
    input_len = 0;
    app_state[trx_id] = APP_IDLE;

    /* Keep compiler happy */
    frame = frame;
}



/* EOF */
