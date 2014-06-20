/**
 * @file tal_4g_utils.c
 *
 * @brief This file supports the TAL PIB attributes by providing 4g specific
 *        information.
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

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_config.h"
#include "tal_pib.h"
#include "tal_internal.h"
#include "ieee_15_4g.h"


/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

#define OQPSK_SYMBOL_DURATION_TABLE \
    /* rows: freq band ; column: symbol duration in us */ \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 320 }, \
    { CHINA_780, 64 }, \
    { EU_863, 320 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 64 }, \
    { KOREA_917, 64 }, \
    { JAPAN_920, 320 }, \
    /* US_928 */   \
    /* { JAPAN_950, 320 }, */\
    /* US_1427 */  \
    { WORLD_2450, 64 }

#define OQPSK_SYMBOL_DURATION_TABLE_ROW_SIZE 7
#define OQPSK_SYMBOL_DURATION_TABLE_COL_SIZE 2
#define OQPSK_SYMBOL_DURATION_TABLE_DATA_TYPE  uint16_t

#define OQPSK_SYMBOL_LENGTH_TABLE \
    /* rows: freq band ; column: symbol length in chips */ \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 32 }, \
    { CHINA_780, 64 }, \
    { EU_863, 32 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 64 }, \
    { KOREA_917, 64 }, \
    { JAPAN_920, 32 }, \
    /* US_928 */   \
    /* { JAPAN_950, 320 }, */\
    /* US_1427 */  \
    { WORLD_2450, 128 }

#define OQPSK_SYMBOL_LENGTH_TABLE_ROW_SIZE 7
#define OQPSK_SYMBOL_LENGTH_TABLE_COL_SIZE 2
#define OQPSK_SYMBOL_LENGTH_TABLE_DATA_TYPE  uint8_t

#define OQPSK_CHIP_RATE_TABLE \
    /* rows: freq band ; column: chip rate (kchip/s) */ \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 100 }, \
    { CHINA_780, 1000 }, \
    { EU_863, 100 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 1000 }, \
    { KOREA_917, 1000 }, \
    { JAPAN_920, 100 }, \
    /* US_928 */   \
    /* { JAPAN_950, 320 }, */\
    /* US_1427 */  \
    { WORLD_2450, 2000 }

#define OQPSK_CHIP_RATE_TABLE_ROW_SIZE 7
#define OQPSK_CHIP_RATE_TABLE_COL_SIZE 2
#define OQPSK_CHIP_RATE_TABLE_DATA_TYPE  uint16_t


#define FSK_CCA_THRES_50_KBIT   (-91 + CCA_OFFSET) /* dBm */
#define FSK_CCA_THRES_100_KBIT   (-88 + CCA_OFFSET) /* dBm */
#define FSK_CCA_THRES_150_KBIT   (-86 + CCA_OFFSET) /* dBm */
#define FSK_CCA_THRES_200_KBIT   (-85 + CCA_OFFSET) /* dBm */
#define FK_CCA_THRES_FEC_OFFSET  (-6)   /* dBm */

#define OQPSK_CCA_DURATION_TABLE \
    /* rows: freq band ; column: aCCATime in symbols */ \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 4 }, \
    { CHINA_780, 8 }, \
    { EU_863, 4 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 8 }, \
    { KOREA_917, 8 }, \
    { JAPAN_920, 0 }, \
    /* US_928 */   \
    /* { JAPAN_950, 0 }, */\
    /* US_1427 */  \
    { WORLD_2450, 8 }

#define OQPSK_CCA_DURATION_TABLE_ROW_SIZE 7
#define OQPSK_CCA_DURATION_TABLE_COL_SIZE 2
#define OQPSK_CCA_DURATION_TABLE_DATA_TYPE  uint8_t

#define SUPPORTED_RF09_BAND_TABLE \
    /* EU_169 */ \
    /* US_450 */ \
    CHINA_470 , \
    CHINA_780 , \
    EU_863, \
    /* US_896 */  \
    /* US_901 */  \
    US_915, \
    KOREA_917, \
    JAPAN_920 \
    /* US_928 */   \
    /* { JAPAN_950, 0 }, */\
    /* US_1427 */  \
    /* { WORLD_2450, 8 } */

#define SUPPORTED_RF09_BAND_TABLE_ROW_SIZE 6
#define SUPPORTED_RF09_BAND_TABLE_COL_SIZE 1
#define SUPPORTED_RF09_BAND_TABLE_DATA_TYPE uint8_t

#define OQPSK_CCA_THRES  (-90)   /* dBm */
#define CCA_OFFSET  10
/* CCA threshold table is based on table 159, pg. 92 */
#define OFDM_CCA_THRES_TABLE \
    /* rows: MCSn; column: option n */ \
    { (-103 + CCA_OFFSET), (-105 + CCA_OFFSET), 0, 0 }, \
    { (-100 + CCA_OFFSET), (-103 + CCA_OFFSET), (-105 + CCA_OFFSET), 0 }, \
    { (-97 + CCA_OFFSET), (-100 + CCA_OFFSET), (-103 + CCA_OFFSET), (-105 + CCA_OFFSET) }, \
    { (-94 + CCA_OFFSET), (-97 + CCA_OFFSET), (-100 + CCA_OFFSET), (-103 + CCA_OFFSET) }, \
    { 0, (-94 + CCA_OFFSET), (-97 + CCA_OFFSET), (-100 + CCA_OFFSET) }, \
    { 0, (-91 + CCA_OFFSET), (-94 + CCA_OFFSET), (-97 + CCA_OFFSET) }, \
    { 0, 0, (-91 + CCA_OFFSET), (-94 + CCA_OFFSET) }

#define OFDM_CCA_THRES_TABLE_ROW_SIZE 7
#define OFDM_CCA_THRES_TABLE_COL_SIZE 4
#define OFDM_CCA_THRES_TABLE_DATA_TYPE  int8_t

#define CH_SPAC_FSK_MOD_1 200000 /* Hz */
#define CH_SPAC_FSK_MOD_2 400000 /* Hz */
#define CH_SPAC_FSK_MOD_3 400000 /* Hz */
#define CH_SPAC_FSK_MOD_4 600000 /* Hz */
#define CH_SPAC_FSK_TABLE   CH_SPAC_FSK_MOD_1, CH_SPAC_FSK_MOD_2, CH_SPAC_FSK_MOD_3, CH_SPAC_FSK_MOD_4

#define CH_SPAC_OFDM_OPT_1 1200000 /* Hz */
#define CH_SPAC_OFDM_OPT_2 800000 /* Hz */
#define CH_SPAC_OFDM_OPT_3 400000 /* Hz */
#define CH_SPAC_OFDM_OPT_4 200000 /* Hz */
#define CH_SPAC_OFDM_TABLE  CH_SPAC_OFDM_OPT_1, CH_SPAC_OFDM_OPT_2, CH_SPAC_OFDM_OPT_3, CH_SPAC_OFDM_OPT_4

#define OQPSK_CH_SPAC_TABLE  \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 400000 }, \
    { CHINA_780, 2000000 }, \
    { EU_863, 0 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 2000000 }, \
    { KOREA_917, 2000000 }, \
    { JAPAN_920, 200000 }, \
    /* US_928 */   \
    /* { JAPAN_950, 951000000, 951100000, 951200000, 951200000 }, */\
    /* US_1427 */  \
    { WORLD_2450, 5000000 }

#define OQPSK_CH_SPAC_TABLE_ROW_SIZE 7
#define OQPSK_CH_SPAC_TABLE_COL_SIZE 2
#define OQPSK_CH_SPAC_TABLE_DATA_TYPE  uint32_t


#define FSK_CH_CENTER_FREQ0_MAP  \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 470020000, 470040000, 0, 0}, \
    { CHINA_780, 779200000, 779400000, 779400000, 0 }, \
    { EU_863, 863125000, 863225000, 863225000, 0 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 902200000, 902400000, 902400000, 0 }, \
    { KOREA_917, 917100000, 917300000, 917300000, 0 }, \
    { JAPAN_920, 920600000, 920900000, 920800000, 920800000 }, \
    /* US_928 */   \
    /* { JAPAN_950, 951000000, 951100000, 951200000, 951200000 }, */\
    /* US_1427 */  \
    { WORLD_2450, 2400200000, 2400400000, 2400400000, 0 }

#define FSK_CH_CENTER_FREQ0_MAP_ROW_SIZE    7
#define FSK_CH_CENTER_FREQ0_MAP_COL_SIZE    5

#define OFDM_CH_CENTER_FREQ0_MAP  \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 0, 0, 0, 470200000 }, \
    { CHINA_780, 780200000, 779800000, 779400000, 779200000 }, \
    { EU_863, 863625000, 863425000, 863225000, 863125000 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 903200000, 902800000, 902400000, 902200000 }, \
    { KOREA_917, 917900000, 917500000, 917300000, 917100000 }, \
    { JAPAN_920, 921200000, 920800000, 920400000, 920200000 }, \
    /* US_928 */   \
    /* { JAPAN_950, 0, 951300000, 951100000, 951000000 }, */\
    /* US_1427 */  \
    { WORLD_2450, 2401200000, 2400800000, 2400400000, 2400200000 }

#define OFDM_CH_CENTER_FREQ0_MAP_ROW_SIZE    7
#define OFDM_CH_CENTER_FREQ0_MAP_COL_SIZE    5

#define OFDM_DATA_RATE_TABLE \
    /* see table 148, pg. 80 */ \
    /* OFDM option 1, OFDM option 2, OFDM option 3, OFDM option 4 */ \
    /* row contains value for a MCS value */ \
    /* A value of 0 indicates that the value is not supported. */ \
    /* All values need to be multiplied with a factor of 10. */ \
    /* MCS0: */ {10, 5, 0, 0}, \
    /* MCS1: */ {20, 10, 5, 0}, \
    /* MCS2: */ {40, 20, 10, 5}, \
    /* MCS3: */ {80, 40, 20, 10}, \
    /* MCS4: */ {0, 60, 30, 15}, \
    /* MCS5: */ {0, 80, 40, 20}, \
    /* MCS6: */ {0, 0, 60, 30}

#define OFDM_DATA_RATE_TABLE_ROW_SIZE 7
#define OFDM_DATA_RATE_TABLE_COL_SIZE 4
#define OFDM_DATA_RATE_TABLE_DATA_TYPE  uint8_t

#define OQPSK_CH_CENTER_FREQ0_MAP  \
    /* frequency band, channel center freq0, channel spacing */ \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 470400000, 400000 }, \
    { CHINA_780, 780000000, 2000000 }, \
    { EU_863, 868300000, 0 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 904000000, 2000000 }, \
    { KOREA_917, 918100000, 2000000 }, \
    { JAPAN_920, 920600000, 200000 }, \
    /* US_928 */   \
    /* { JAPAN_950, 951100000, 400000 }, */\
    /* US_1427 */  \
    { WORLD_2450, 2405000000, 5000000 }

#define OQPSK_CH_CENTER_FREQ0_MAP_ROW_SIZE    7
#define OQPSK_CH_CENTER_FREQ0_MAP_COL_SIZE    3

#define OQPSK_EU_863_FREQ_TABLE   868300000, 868950000, 869525000
#define OQPSK_EU_863_FREQ_TABLE_SIZE  3
#define OQPSK_EU_863_FREQ_TABLE_DATA_TYPE  uint32_t

#define OQPSK_SHR_DURATION_TABLE \
    /* frequency band, shrDuration */ \
    /* EU_169 */ \
    /* US_450 */ \
    { CHINA_470, 48 }, \
    { CHINA_780, 72 }, \
    { EU_863, 48 }, \
    /* US_896 */  \
    /* US_901 */  \
    { US_915, 72 }, \
    { KOREA_917, 72 }, \
    { JAPAN_920, 48 }, \
    /* US_928 */   \
    /* { JAPAN_950,  }, */\
    /* US_1427 */  \
    { WORLD_2450, 72 }

#define OQPSK_SHR_DURATION_TABLE_ROW_SIZE  7
#define OQPSK_SHR_DURATION_TABLE_COL_SIZE  2
#define OQPSK_SHR_DURATION_TABLE_DATA_TYPE  uint8_t

#define OQPSK_TOTAL_CHANNELS_MAP  \
/* frequency band, total channels */ \
/* EU_169 */ \
/* US_450 */ \
{ CHINA_470, 99 }, \
{ CHINA_780, 4 }, \
{ EU_863, 3 }, \
/* US_896 */  \
/* US_901 */  \
{ US_915, 12 }, \
{ KOREA_917, 3 }, \
{ JAPAN_920, 38 }, \
/* US_928 */   \
/* { JAPAN_950, 951100000, 400000 }, */\
/* US_1427 */  \
{ WORLD_2450, 16 }

#define OQPSK_TOTAL_CHANNELS_MAP_ROW_SIZE    7
#define OQPSK_TOTAL_CHANNELS_MAP_COL_SIZE    2

#define OFDM_TOTAL_CHANNELS_MAP  \
/* frequency band, op1 ,opt2 ,opt3 ,opt4total channels */ \
/* EU_169 */ \
/* US_450 */ \
{ CHINA_470, 0,0,0,199 }, \
{ CHINA_780, 6,9,19,39 }, \
{ EU_863, 5,8,17,34 }, \
/* US_896 */  \
/* US_901 */  \
{ US_915, 20,31,64,129 }, \
{ KOREA_917, 5,8,16,32 }, \
{ JAPAN_920, 6,9,19,39 }, \
/* US_928 */   \
/* { JAPAN_950, 951100000, 400000 }, */\
/* US_1427 */  \
{ WORLD_2450, 64,97,207,416 }

#define OFDM_TOTAL_CHANNELS_MAP_ROW_SIZE    7
#define OFDM_TOTAL_CHANNELS_MAP_COL_SIZE    5
/* === GLOBALS ============================================================= */

FLASH_DECLARE(OQPSK_SYMBOL_DURATION_TABLE_DATA_TYPE
              oqpsk_sym_duration_table[OQPSK_SYMBOL_DURATION_TABLE_ROW_SIZE][OQPSK_SYMBOL_DURATION_TABLE_COL_SIZE]) =
{ OQPSK_SYMBOL_DURATION_TABLE };

FLASH_DECLARE(OQPSK_CCA_DURATION_TABLE_DATA_TYPE
              oqpsk_cca_dur_table[OQPSK_CCA_DURATION_TABLE_ROW_SIZE][OQPSK_CCA_DURATION_TABLE_COL_SIZE]) =
{OQPSK_CCA_DURATION_TABLE};

FLASH_DECLARE(SUPPORTED_RF09_BAND_TABLE_DATA_TYPE
              supported_rf09_band_table[SUPPORTED_RF09_BAND_TABLE_ROW_SIZE]) =
{SUPPORTED_RF09_BAND_TABLE};

FLASH_DECLARE(OFDM_CCA_THRES_TABLE_DATA_TYPE
              ofdm_cca_thres[OFDM_CCA_THRES_TABLE_ROW_SIZE][OFDM_CCA_THRES_TABLE_COL_SIZE]) =
{OFDM_CCA_THRES_TABLE};

FLASH_DECLARE(OFDM_DATA_RATE_TABLE_DATA_TYPE
              ofdm_data_rate_table[OFDM_DATA_RATE_TABLE_ROW_SIZE][OFDM_DATA_RATE_TABLE_COL_SIZE]) =
{OFDM_DATA_RATE_TABLE};

FLASH_DECLARE(uint32_t ofdm_freq0_map[OFDM_CH_CENTER_FREQ0_MAP_ROW_SIZE][OFDM_CH_CENTER_FREQ0_MAP_COL_SIZE]) = { OFDM_CH_CENTER_FREQ0_MAP };
FLASH_DECLARE(uint32_t oqpsk_freq0_map[OQPSK_CH_CENTER_FREQ0_MAP_ROW_SIZE][OQPSK_CH_CENTER_FREQ0_MAP_COL_SIZE]) = { OQPSK_CH_CENTER_FREQ0_MAP };
FLASH_DECLARE(uint32_t ofdm_ch_spacing_table[]) = { CH_SPAC_OFDM_TABLE };
FLASH_DECLARE(uint32_t ofdm_max_ch_map[OFDM_TOTAL_CHANNELS_MAP_ROW_SIZE][OFDM_TOTAL_CHANNELS_MAP_COL_SIZE]) = { OFDM_TOTAL_CHANNELS_MAP };
FLASH_DECLARE(uint32_t oqpsk_max_ch_map[OQPSK_TOTAL_CHANNELS_MAP_ROW_SIZE][OQPSK_TOTAL_CHANNELS_MAP_COL_SIZE]) = { OQPSK_TOTAL_CHANNELS_MAP };	
FLASH_DECLARE(OQPSK_CH_SPAC_TABLE_DATA_TYPE oqpsk_ch_spac_table[OQPSK_CH_SPAC_TABLE_ROW_SIZE][OQPSK_CH_SPAC_TABLE_COL_SIZE]) = { OQPSK_CH_SPAC_TABLE };
FLASH_DECLARE(OQPSK_SHR_DURATION_TABLE_DATA_TYPE oqpsk_shr_duration_table[OQPSK_SHR_DURATION_TABLE_ROW_SIZE][OQPSK_SHR_DURATION_TABLE_COL_SIZE]) = { OQPSK_SHR_DURATION_TABLE };
FLASH_DECLARE(OQPSK_SYMBOL_LENGTH_TABLE_DATA_TYPE oqpsk_sym_len_table[OQPSK_SYMBOL_LENGTH_TABLE_ROW_SIZE][OQPSK_SYMBOL_LENGTH_TABLE_COL_SIZE]) = {OQPSK_SYMBOL_LENGTH_TABLE};
FLASH_DECLARE(OQPSK_CHIP_RATE_TABLE_DATA_TYPE oqpsk_chip_rate_table[OQPSK_CHIP_RATE_TABLE_ROW_SIZE][OQPSK_CHIP_RATE_TABLE_COL_SIZE]) = {OQPSK_CHIP_RATE_TABLE};
#ifdef SUPPORT_FSK
FLASH_DECLARE(uint32_t fsk_freq0_map[FSK_CH_CENTER_FREQ0_MAP_ROW_SIZE][FSK_CH_CENTER_FREQ0_MAP_COL_SIZE]) = { FSK_CH_CENTER_FREQ0_MAP };
FLASH_DECLARE(uint32_t fsk_ch_spacing_table[]) = { CH_SPAC_FSK_TABLE };
#endif

/* === PROTOTYPES ========================================================== */

#ifdef SUPPORT_LEGACY_OQPSK
static inline void get_legacy_freq_f0(trx_id_t trx_id, uint32_t *freq, uint32_t *spacing);
#endif
static inline void get_sun_freq_f0(trx_id_t trx_id, uint32_t *freq, uint32_t *spacing);
static uint16_t oqpsk_psdu_duration_sym(trx_id_t trx_id, uint16_t len);
static uint8_t oqpsk_get_spread_rate(trx_id_t trx_id);


/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Gets the symbol duration in us
 *
 * @param trx_id Transceiver identifier
 *
 * @return Duration of a symbol in us
 */
uint16_t get_symbol_duration_us(trx_id_t trx_id)
{
    uint16_t ret_val = 0; // 0: indicator for wrong parameter

    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            ret_val = 20; // table 0, pg. 7
            break;
#endif
        case OFDM:
            ret_val = 120;
            break;

        case OQPSK:
            for (uint8_t i = 0; i < OQPSK_SYMBOL_DURATION_TABLE_ROW_SIZE; i++)
            {
                if (tal_pib[trx_id].phy.freq_band == (uint16_t)PGM_READ_WORD(&oqpsk_sym_duration_table[i][0]))
                {
                    ret_val = (uint16_t)PGM_READ_WORD(&oqpsk_sym_duration_table[i][1]);
                    break;
                }
            }
            break;

#ifdef SUPPORT_LEGACY_OQPSK
        case LEG_OQPSK:
            ret_val = 16;
            break;
#endif
        default:
            break;
    }

    return ret_val;
}


uint16_t get_cca_duration_us(trx_id_t trx_id)
{
    uint16_t ret_val = 0;

    if (tal_pib[trx_id].phy.freq_band == JAPAN_920)
    {
        ret_val = calculate_cca_duration_us(trx_id);
    }
    else
    {
        switch (tal_pib[trx_id].phy.modulation)
        {
#ifdef SUPPORT_FSK
            case FSK:
                /* Fall through */
#endif
            case OFDM:
                ret_val = 8; /* symbols */
                break;

            case OQPSK:
                /* Check CCA table for entry */
                for (uint8_t i = 0; i < OQPSK_CCA_DURATION_TABLE_ROW_SIZE; i++)
                {
                    if (tal_pib[trx_id].phy.freq_band == (uint8_t)PGM_READ_BYTE(&oqpsk_cca_dur_table[i][0]))
                    {
                        ret_val = (uint8_t)PGM_READ_BYTE(&oqpsk_cca_dur_table[i][1]); /* symbols */
                        break;
                    }
                }
                break;

#ifdef SUPPORT_LEGACY_OQPSK
            case LEG_OQPSK:
                ret_val = 8;
                break;
#endif

            default:
                break;
        }

        /* Change value from symbols to us */
        ret_val = ret_val * tal_pib[trx_id].SymbolDuration_us;
    }

    return ret_val;
}


bool is_freq_band_supported(trx_id_t trx_id, phy_t *phy)
{
    bool ret = false;

    if (trx_id == RF24)
    {
        if (phy->freq_band == WORLD_2450)
        {
            ret = true;
        }
    }
    else
    {
        for (uint8_t i = 0; i < SUPPORTED_RF09_BAND_TABLE_ROW_SIZE; i++)
        {
            if (phy->freq_band == (uint8_t)PGM_READ_BYTE(&supported_rf09_band_table[i]))
            {
                ret = true;
                break;
            }
        }
    }

    return ret;
}


int8_t get_cca_thres(trx_id_t trx_id)
{
    int8_t thres = 0; // 0: indicator for wrong parameter

    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            {
                uint16_t data_rate = get_data_rate(trx_id);

                switch (data_rate)
                {
                    case 50:
                        thres = FSK_CCA_THRES_50_KBIT;
                        break;

                    case 100:
                        thres = FSK_CCA_THRES_100_KBIT;
                        break;

                    case 150:
                        thres = FSK_CCA_THRES_150_KBIT;
                        break;

                    case 200:
                        thres = FSK_CCA_THRES_200_KBIT;
                        break;
                }
            }
            if (tal_pib[trx_id].FSKFECEnabled)
            {
                thres += FK_CCA_THRES_FEC_OFFSET;
            }
            break;
#endif
        case OFDM:
            {
                /* rows: MCSn; column: option n */ \
                thres = (int8_t)PGM_READ_BYTE(&ofdm_cca_thres[tal_pib[trx_id].phy.phy_mode.ofdm.mcs_val]\
                                              [tal_pib[trx_id].phy.phy_mode.ofdm.option]);
            }
            break;

        case OQPSK:
            thres = (int8_t)OQPSK_CCA_THRES;
            break;
#ifdef SUPPORT_LEGACY_OQPSK
        case LEG_OQPSK:
            thres = -85 + 10;
            break;
#endif

        default:
            break;
    }

    return thres;
}


// see section 6.4.3, pg. 30
uint16_t get_AckWaitDuration_us(trx_id_t trx_id)
{
    uint16_t AckWaitDuration = 6000; // ?

    uint8_t ack_len = 5;
    if (tal_pib[trx_id].FCSType == FCS_TYPE_4_OCTETS)
    {
        ack_len += 2;
    }

    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            break;
#endif

        case OFDM:
            /* aUnitBackoffPeriod */
            AckWaitDuration = ceiling_sym(trx_id, 1000) + tal_pib[trx_id].CCADuration_sym;
            /* aTurnaroundTime */
            AckWaitDuration += ceiling_sym(trx_id, 1000);
            /* phySHRDuration; see pg. 49 */
            AckWaitDuration += shr_duration_sym(trx_id);
            /* phyPHRDuration, see pg. 46 */
            AckWaitDuration += phr_duration_sym(trx_id);
            /* PSDU len = 3 + FCS = 5 or 7; add TAIL and PAD; */
            AckWaitDuration += ceiling_sym(trx_id, ack_len * tal_pib[trx_id].OctetDuration_us);
            break;

        case OQPSK:
            /* aUnitBackoffPeriod */
            AckWaitDuration = ceiling_sym(trx_id, 1000) + tal_pib[trx_id].CCADuration_sym;
            /* aTurnaroundTime */
            AckWaitDuration += ceiling_sym(trx_id, 1000);
            /* phySHRDuration */
            AckWaitDuration += shr_duration_sym(trx_id);
            /* phyPHRDuration */
            AckWaitDuration += phr_duration_sym(trx_id);
            /* PSDU duration */
            AckWaitDuration += oqpsk_psdu_duration_sym(trx_id, ack_len);
            break;

#ifdef SUPPORT_LEGACY_OQPSK
        case LEG_OQPSK:
            AckWaitDuration = 54; // symbols
            AckWaitDuration -= 2; // processing delay
            break;
#endif

        default:
            break;
    }

    /* Convert from symbols to us */
    AckWaitDuration *= tal_pib[trx_id].SymbolDuration_us;

    return AckWaitDuration;
}


uint16_t get_AckTiming_us(trx_id_t trx_id)
{
    uint16_t ack;

#ifdef SUPPORT_LEGACY_OQPSK
    if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
    {
        ack = aTurnaroundTime; // = 12
    }
    else

#endif
    {
        ack = ceiling_sym(trx_id, 1000);
    }

    ack *= tal_pib[trx_id].SymbolDuration_us;

    // ACK timing correction - processing delay?
    // RX to RXFE IRQ delay: 75 us // @ ToDo: verify values; PHY mode dependent
    // Txprep to Tx delay: 40 us
    // MCU processing delay: 50 us
    // PA ramping delay: 32 us; MCU dependent
    switch (tal_pib[trx_id].phy.modulation)
    {
        case OFDM:
            ack -= 200; // @ ToDo: verify values and replace magic numbers
            break;

        case OQPSK:
            ack -= 60; // @ ToDo: verify values and replace magic numbers
            break;

        case LEG_OQPSK:
            ack -= 35;  // @ ToDo: verify values and replace magic numbers
            break;

        default:
            break;
    }

    return ack;
}


/* returns the smallest integer value in symbols greater or equal to its argument value */
// see section 9.2, pg. 43
uint16_t ceiling_sym(trx_id_t trx_id, uint16_t duration_us)
{
    uint16_t sym;
    sym = duration_us / tal_pib[trx_id].SymbolDuration_us;
    if ((duration_us % tal_pib[trx_id].SymbolDuration_us) > 0)
    {
        sym++;
    }
    return sym;
}


// see pg. 46
uint8_t phr_duration_sym(trx_id_t trx_id)
{
    uint8_t phr;

    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            phr = 0;
            break;
#endif
        case OFDM:
            if (tal_pib[trx_id].phy.phy_mode.ofdm.interl)
            {
                switch (tal_pib[trx_id].phy.phy_mode.ofdm.option)
                {
                    case OFDM_OPT_1:
                        phr = 4;
                        break;

                    case OFDM_OPT_2:
                        phr = 8;
                        break;

                    default:    /*  OFDM_OPT_3 and OFDM_OPT_4 */
                        phr = 6;
                        break;
                }
            }
            else // interleaving == 0
            {
                if (tal_pib[trx_id].phy.phy_mode.ofdm.option == OFDM_OPT_1)
                {
                    phr = 3;
                }
                else
                {
                    phr = 6;
                }
            }
            break;

        case OQPSK:
            phr = 15;
            break;
#ifdef SUPPORT_LEGACY_OQPSK
        case LEG_OQPSK:
            phr = 2;
            break;
#endif
        default:
            phr = 0;
            break;
    }

    return phr;
}


uint8_t shr_duration_sym(trx_id_t trx_id)
{
    uint8_t shr = 0;

    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            break;
#endif
        case OFDM:
            shr = 6;
            break;

        case OQPSK: /* see pg. 119 */
            for (uint8_t i = 0; i < OQPSK_SHR_DURATION_TABLE_ROW_SIZE; i++)
            {
                if (tal_pib[trx_id].phy.freq_band == (uint8_t)PGM_READ_BYTE(&oqpsk_shr_duration_table[i][0]))
                {
                    shr = (uint8_t)PGM_READ_BYTE(&oqpsk_shr_duration_table[i][1]);
                    break;
                }
            }
            break;
#ifdef SUPPORT_LEGACY_OQPSK
        case LEG_OQPSK:
            shr = 8 + 2; // 8 = SHR; 2 = SFD
            break;
#endif
        default:
            break;
    }

    return shr;
}


// pg. 119, section 18.3.2.14
static uint16_t oqpsk_psdu_duration_sym(trx_id_t trx_id, uint16_t len)
{
    uint8_t Ns = 0; // symbol length in chips
    for (uint8_t i = 0; i < OQPSK_SYMBOL_LENGTH_TABLE_ROW_SIZE; i++)
    {
        if (tal_pib[trx_id].phy.freq_band == (uint8_t)PGM_READ_BYTE(&oqpsk_sym_len_table[i][0]))
        {
            Ns = (uint8_t)PGM_READ_BYTE(&oqpsk_sym_len_table[i][1]); // Table 183
            break;
        }
    }
    uint8_t Rspread = oqpsk_get_spread_rate(trx_id);
    uint16_t temp = (8 * len) + 6;
    uint16_t Nd = temp / 63;
    if ((temp % 63) > 0)
    {
        Nd += 1;
    }
    Nd *= 63;
    uint16_t Npsdu = Rspread * 2 * Nd;
    uint16_t duration_sym = Npsdu / Ns;
    if (Npsdu % Ns)
    {
        duration_sym += 1;
    }
    temp = 16 * Ns;
    duration_sym += Npsdu / temp;
    if ((Npsdu % temp) > 0)
    {
        duration_sym += 1;
    }

    return duration_sym;
}


// table 166, pg. 101
static uint8_t oqpsk_get_spread_rate(trx_id_t trx_id)
{
    uint8_t rate = 1 << (3 - tal_pib[trx_id].phy.phy_mode.oqpsk.rate_mode);
    if (tal_pib[trx_id].phy.phy_mode.oqpsk.rate_mode == OQPSK_RATE_MOD_0)
    {
        /* Rate of rate mode 0 depends on freq band */
        switch (tal_pib[trx_id].phy.freq_band)
        {
            case CHINA_780:
            case US_915:
            case KOREA_917:
            case WORLD_2450:
                rate *= 2;
                break;

            default: /* no changes */
                break;
        }
    }

    if (tal_pib[trx_id].phy.freq_band == WORLD_2450)
    {
        rate *= 2;
    }

    return rate;
}


// table 166, pg. 101
uint16_t oqpsk_get_chip_rate(trx_id_t trx_id)
{
    uint16_t rate = 0;

    for (uint8_t i = 0; i < OQPSK_CHIP_RATE_TABLE_ROW_SIZE; i++)
    {
        if (tal_pib[trx_id].phy.freq_band == (uint16_t)PGM_READ_WORD(&oqpsk_chip_rate_table[i][0]))
        {
            rate = (uint16_t)PGM_READ_WORD(&oqpsk_chip_rate_table[i][1]);
            break;
        }
    }

    return rate;
}
//check
oqpsk_chip_rate_t get_oqpsk_chip_rate(trx_id_t trx_id,sun_freq_band_t freq_band)
{
	uint16_t rate = 0;
	uint8_t rate_mode;

	for (uint8_t i = 0; i < OQPSK_CHIP_RATE_TABLE_ROW_SIZE; i++)
	{
		if (freq_band == (uint16_t)PGM_READ_WORD(&oqpsk_chip_rate_table[i][0]))
		{
			rate = (uint16_t)PGM_READ_WORD(&oqpsk_chip_rate_table[i][1]);
			break;
		}
	}
if(rate == 2000)
{
rate_mode = CHIP_RATE_2000;

}
else if(rate == 1000)
{
	rate_mode = CHIP_RATE_1000;

}
else if(rate == 200)
{
	rate_mode = CHIP_RATE_200;

}
else if(rate == 100)
{
	rate_mode = CHIP_RATE_100;

}
	return rate_mode;
}



void get_ch_freq0_spacing(trx_id_t trx_id, uint32_t *freq, uint32_t *spacing)
{
#ifdef SUPPORT_LEGACY_OQPSK
    if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
    {
        get_legacy_freq_f0(trx_id, freq, spacing);
    }
    else
#endif
    {
        get_sun_freq_f0(trx_id, freq, spacing);
    }
}


static inline void get_sun_freq_f0(trx_id_t trx_id, uint32_t *freq, uint32_t *spacing)
{
    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            for (uint8_t i = 0; i < FSK_CH_CENTER_FREQ0_MAP_ROW_SIZE; i++)
            {
                if (tal_pib[trx_id].phy.freq_band == (uint32_t)PGM_READ_DWORD(&fsk_freq0_map[i][0]))
                {
                    *freq = (uint32_t)PGM_READ_DWORD(&fsk_freq0_map[i][tal_pib[trx_id].phy.phy_mode.fsk.op_mode + 1]);
                    break;
                }
            }
            *spacing = (uint32_t)PGM_READ_DWORD(&fsk_ch_spacing_table[tal_pib[trx_id].phy.phy_mode.fsk.op_mode]);
            break;
#endif
        case OFDM:
            for (uint8_t i = 0; i < OFDM_CH_CENTER_FREQ0_MAP_ROW_SIZE; i++)
            {
                if (tal_pib[trx_id].phy.freq_band == (uint32_t)PGM_READ_DWORD(&ofdm_freq0_map[i][0]))
                {
                    *freq = (uint32_t)PGM_READ_DWORD(&ofdm_freq0_map[i][tal_pib[trx_id].phy.phy_mode.ofdm.option + 1]);
                    break;
                }
            }
            *spacing = (uint32_t)PGM_READ_DWORD(&ofdm_ch_spacing_table[tal_pib[trx_id].phy.phy_mode.ofdm.option]);
            break;

        case OQPSK:
            for (uint8_t i = 0; i < OQPSK_CH_CENTER_FREQ0_MAP_ROW_SIZE; i++)
            {
                if (tal_pib[trx_id].phy.freq_band == (uint32_t)PGM_READ_DWORD(&oqpsk_freq0_map[i][0]))
                {
                    *freq = (uint32_t)PGM_READ_DWORD(&oqpsk_freq0_map[i][1]);
                    if (tal_pib[trx_id].phy.freq_band == EU_863)
                    {
                        *spacing = 0; // ? //check
                    }
                    else
                    {
                        *spacing = (uint32_t)PGM_READ_DWORD(&oqpsk_ch_spac_table[i][1]);
                    }
                    break;
                }
            }
            break;

        default:
            break;
    }

    /* Some sanity check */
    if (trx_id == RF09)
    {
        if (*freq > 930000000)
        {
            *freq = 0;
        }
    }
    else // RF24
    {
        if (*freq < 2400000000)
        {
            *freq = 0;
        }
    }
}

//check
void get_ofdm_freq_f0(trx_id_t trx_id,sun_freq_band_t freq_band,ofdm_option_t option,uint32_t *freq, uint32_t *spacing)
{

		for (uint8_t i = 0; i < OFDM_CH_CENTER_FREQ0_MAP_ROW_SIZE; i++)
		{
			if (freq_band == (uint32_t)PGM_READ_DWORD(&ofdm_freq0_map[i][0]))
			{
				*freq = (uint32_t)PGM_READ_DWORD(&ofdm_freq0_map[i][option + 1]);
				break;	
			}
		}
		*spacing = (uint32_t)PGM_READ_DWORD(&ofdm_ch_spacing_table[option]);


}

void get_oqpsk_freq_f0(trx_id_t trx_id,sun_freq_band_t freq_band ,uint32_t *freq, uint32_t *spacing)
{

          for (uint8_t i = 0; i < OQPSK_CH_CENTER_FREQ0_MAP_ROW_SIZE; i++)
          {
	          if (freq_band == (uint32_t)PGM_READ_DWORD(&oqpsk_freq0_map[i][0]))
	          {
		          *freq = (uint32_t)PGM_READ_DWORD(&oqpsk_freq0_map[i][1]);
		          if (freq_band == EU_863)
		          {
			          *spacing = 0; // ?
		          }
		          else
		          {
			          *spacing = (uint32_t)PGM_READ_DWORD(&oqpsk_ch_spac_table[i][1]);
		          }
		          break;
	          }
          }
}

uint16_t get_sun_max_ch_no(trx_id_t trx_id)
{
	
uint16_t max_ch = 0;
    switch (tal_pib[trx_id].phy.modulation)
    {
	case OFDM:
	for (uint8_t i = 0; i < OFDM_CH_CENTER_FREQ0_MAP_ROW_SIZE; i++)
	{
		if (tal_pib[trx_id].phy.freq_band == (uint32_t)PGM_READ_DWORD(&ofdm_freq0_map[i][0]))
		{
			max_ch = (uint32_t)PGM_READ_DWORD(&ofdm_max_ch_map[i][tal_pib[trx_id].phy.phy_mode.ofdm.option +1]);
			break;
		}
	}
	break;

	case OQPSK:
	for (uint8_t i = 0; i < OQPSK_CH_CENTER_FREQ0_MAP_ROW_SIZE; i++)
	{
		if (tal_pib[trx_id].phy.freq_band == (uint32_t)PGM_READ_DWORD(&oqpsk_freq0_map[i][0]))
		{
			max_ch = (uint32_t)PGM_READ_DWORD(&oqpsk_max_ch_map[i][1]);
			break;
		}
	}
	break;

	default:
	break;
	}
	return max_ch;
}


#ifdef SUPPORT_LEGACY_OQPSK
static inline void get_legacy_freq_f0(trx_id_t trx_id, uint32_t *freq, uint32_t *spacing)
{
    //debug_text(PSTR("get_legacy_freq_f0()"));

    switch (tal_pib[trx_id].CurrentPage)
    {
        case CH_PG_2003: /* Fall through */
        case CH_PG_2006:
            //debug_text(PSTR("CH_PG_2003 or CH_PG_2006"));
            //debug_text_val(PSTR("tal_pib[trx_id].CurrentChannel ="), tal_pib[trx_id].CurrentChannel);
            if (trx_id == RF09)
            {
                if (tal_pib[trx_id].CurrentChannel == 0)
                {
                    *freq = 868300000;
                    *spacing = 2000000; // only single channel
                }
                else if (tal_pib[trx_id].CurrentChannel < 11)
                {
                    *freq = 906000000;// + (2 * (ch - 1));
                    *spacing = 2000000;
                }
            }
            else // RF24
            {
                if ((tal_pib[trx_id].CurrentChannel > 10) &&
                    (tal_pib[trx_id].CurrentChannel < 27)) /* 2.4 GHz O-QPSK */
                {
                    *freq = 2405000000;// + (5 * (ch - 11));
                    *spacing = 5000000;
                }
            }
            break;

        case CH_PG_CHINA:
            //debug_text(PSTR("CH_PG_CHINA"));
            if (tal_pib[trx_id].CurrentChannel < 4)
            {
                *freq = 780000000;// + (2 * ch);
                *spacing = 2000000;
            }
            break;

        default:
            break;
    }
}
#endif /* #ifdef SUPPORT_LEGACY_OQPSK */


uint16_t get_data_rate(trx_id_t trx_id)
{
    uint16_t rate = 0;

    switch (tal_pib[trx_id].phy.modulation)
    {
#ifdef SUPPORT_FSK
        case FSK:
            {
                switch (tal_pib[trx_id].phy.phy_mode.fsk.op_mode)
                {
                    case FSK_OP_MOD_1:
                        rate = 50;
                        break;

                    case FSK_OP_MOD_2:
                        switch (tal_pib[trx_id].phy.freq_band)
                        {
                            case US_915:
                            case KOREA_917:
                            case WORLD_2450:
                                rate = 150;
                                break;

                            default:
                                rate = 100;
                                break;
                        }
                        break;

                    case FSK_OP_MOD_3:
                        rate = 200;
                        break;

                    default:
                        break;
                }
            }
            break;
#endif /* #ifdef SUPPORT_FSK */

        case OFDM:
            rate = 10 * (uint8_t)PGM_READ_BYTE(&ofdm_data_rate_table[tal_pib[trx_id].phy.phy_mode.ofdm.mcs_val] \
                                               [tal_pib[trx_id].phy.phy_mode.ofdm.option]);
            break;

        case OQPSK:
            {
                uint16_t chip_rate = oqpsk_get_chip_rate(trx_id);
                uint8_t spread = oqpsk_get_spread_rate(trx_id);
                rate = chip_rate / 2 / spread;
            }
            break;

        default:
            break;
    }

    return rate;
}


uint16_t calculate_cca_duration_us(trx_id_t trx_id)
{
    uint16_t ret;

    if (tal_pib[trx_id].CCATimeMethod == 0)
    {
        ret = tal_pib[trx_id].SymbolDuration_us * tal_pib[trx_id].CCADuration_sym;
    }
    else    // == 1
    {
        ret = 2 << tal_pib[trx_id].CCADuration_us;
    }

    return ret;
}

/* EOF */
