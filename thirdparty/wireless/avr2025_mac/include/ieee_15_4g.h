/**
 * @file ieee_15_4g.h
 *
 * @brief This header holds all IEEE 802.15.4g-2012 constants and attribute identifiers
 *
 * $Id: ieee_15_4g.h 35133 2013-09-20 10:33:42Z uwalter $
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
#ifndef ieee_15_4g_H
#define ieee_15_4g_H

/**
 * Attribute Id addresses the PHY settings; id is not standard-compliant.
 */
#define phySetting                      0x08

/**
 * The duration for CCA, specified in symbols.
 */
#define phyCCADuration                  (0x09)

/**
 * This parameter determines how to calculate
 * the time required to perform CCA detection.
 */
#define phyCCATimeMethod                (0x0A)

/**
 * A value of TRUE indicates that FEC is turned on. A value of FALSE indicates
 * that FEC is turned off. This attribute is only valid for the MR-FSK PHY.
 */
#define phyFSKFECEnabled                (0x0B)

/**
 * A value of TRUE indicates that interleaving is enabled for RSC. A value of
 * FALSE indicates that interleaving is disabled for RSC. This attribute is
 * only valid for the MR-FSK PHY.
 */
#define phyFSKFECInterleavingRSC        (0x0C)

/**
 * A value of zero indicates that a nonrecursive and nonsystematic code (NRNSC)
 * is employed. A value of one indicates that a recursive and systematic code
 * (RSC) is employed. This attribute is only valid for the MR-FSK PHY.
 */
#define phyFSKFECScheme                 (0x0D)

/**
 * The list of channel numbers supported when phyCurrentPage = 7 or 8.
 */
#define phySUNChannelsSupported         (0x0E)

/**
 * The maximum channel number supported by the device.
 * This attribute is only valid if phyCurrentPage equals 7 or 8.
 */
#define phyMaxSUNChannelSupported       (0x0F)

/**
 * The number of SUN channel page entries supported by the device.
 */
#define phyNumSUNPageEntriesSupported   (0x10)

/**
 * Each entry in the list contains the description of a frequency band,
 * modulation scheme, and particular PHY mode implemented by the device.
 */
#define phySUNPageEntriesSupported      (0x11)

/**
 * Defines the current frequency band, modulation scheme, and particular PHY
 * mode when phyCurrentPage = 7 or 8.
 */
#define phyCurrentSUNPageEntry          (0x12)

/**
 * The number of GenericPHYDescriptor entries supported by the device
 */
#define phySUNNumGenericPHYDescriptors  (0x13)

/**
 * A table of GenericPHYDescriptor entries, where each entry is used to define
 * a channel page 10 PHY mode.
 */
#define phySUNGenericPHYDescriptors     (0x14)

/**
 * The number of current entries in phyModeSwitchParameterEntries.
 */
#define phyNumModeSwitchParameterEntries    (0x15)

/**
 * An array of up to four rows, where each row consists of a set of
 * ModeSwitchDescriptor entries. This attribute is only valid for the MR-FSK
 * PHY.
 */
#define phyModeSwitchParameterEntries   (0x16)

/**
 * The number of 1-octet patterns in the preamble.
 * This attribute is only valid for the MR-FSK PHY.
 */
#define phyFSKPreambleLength            (0x17)

/**
 * Determines which group of SFDs is used.
 * This attribute is only valid for the MR-FSK PHY.
 */
#define phyMRFSKSFD                     (0x18)

/**
 * A value of FALSE indicates that data whitening of the PSDU is disabled.
 * A value of TRUE indicates that data whitening of the PSDU is enabled.
 * This attribute is only valid for the MR-FSK PHY.
 */
#define phyFSKScramblePSDU              (0x19)

/**
 * A value of zero indicates an interleaving depth of one symbol.
 * A value of one indicates an interleaving depth of the number of symbols
 * equal to the frequency domain spreading factor (SF).
 * This attribute is only valid for the MR-OFDM PHY.
 */
#define phyOFDMInterleaving             (0x1A)

/**
 * The duration of the PHR, in symbols, for the current PHY.
 * This attribute is only valid for the MR-OFDM PHY and MR-O-QPSK PHY.
 */
#define phyPHRDuration                  (0x1B)

/**
 * The maximum duration for waiting for an incoming ACK.
 */
#define macACKWaitDuration              (0x1C)

/**
 * Duration between end of incoming frame and ACK transmission start.
 */
#define macACKTiming                    (0x1D)

/**
 * CCA threshold
 */
#define phyCCAThreshold                 (0x1E)

/**
 * On air duration or tx duration measured in us
 */
#define phyOnAirDuration                (0x1F)

/**
 * Adapt data rate of the incoming frame to use for ACK transmission
 */
#define macAdaptDataRateForACK          (0x20)

/**
 * MCS setting; used for OFDM only
 */
#define phyOFDMMCS                      (0x21)

/**
 * Frame filter configuration, frame type
 */
#define macFrameFilterFrameTypes        (0x22)

/**
 * Frame filter configuration, frame version
 */
#define macFrameFilterFrameVersions     (0x23)

/**
 * The type of the FCS. A value of zero indicates a 4-octet FCS. A value of
 * one indicates a 2-octet FCS. This attribute is only valid for SUN PHYs.
 */
#define macFCSType                      0xF2
#define FCS_TYPE_4_OCTETS               (0)
#define FCS_TYPE_2_OCTETS               (1)

#define aMaxPHYPacketSize_4g            2047


/**
 * Modulation schemes
 */
typedef enum modulation_tag
{
    FSK,
    OFDM,
    OQPSK,
    LEG_OQPSK
} SHORTENUM modulation_t;

/**
 * SUN PHY frequency bands
 */
typedef enum sun_freq_band_tag
{
    EU_169,
    US_450,
    CHINA_470,
    CHINA_780,
    EU_863,
    US_896,
    US_901,
    US_915,
    KOREA_917,
    JAPAN_920,
    US_928,
    JAPAN_950,
    US_1427,
    WORLD_2450
} SHORTENUM sun_freq_band_t;

/**
 * Channel page
 */
typedef enum ch_pg_tag
{
    CH_PG_2003 = 0,
    CH_PG_2006 = 2,
    CH_PG_CHINA = 5,
    CH_PG_JAPAN = 6,
    CH_PG_SUN = 9,
    CH_PG_GENERIC_PHY = 10
} SHORTENUM ch_pg_t;

typedef enum ofdm_option_tag
{
    OFDM_OPT_1,
    OFDM_OPT_2,
    OFDM_OPT_3,
    OFDM_OPT_4,
} SHORTENUM ofdm_option_t;

typedef enum ofdm_mcs_val_tag
{
    MCS0,
    MCS1,
    MCS2,
    MCS3,
    MCS4,
    MCS5,
    MCS6
} SHORTENUM ofdm_mcs_val_t;

typedef enum oqpsk_rate_mode_tag
{
    OQPSK_RATE_MOD_0,
    OQPSK_RATE_MOD_1,
    OQPSK_RATE_MOD_2,
    OQPSK_RATE_MOD_3,
    OQPSK_RATE_MOD_4 /* Proprietary mode */
} SHORTENUM oqpsk_rate_mode_t;

/** Enumeration for O-QPSK chip rate */
typedef enum oqpsk_chip_rate_tag
{
    CHIP_RATE_100,
    CHIP_RATE_200,
    CHIP_RATE_1000,
    CHIP_RATE_2000
} oqpsk_chip_rate_t;

/** Legacy O-QPSK constants */
#define LEG_780_F0                  780000000
#define LEG_868_F0                  868300000
#define LEG_915_F0                  906000000
#define LEG_915_CH_SPAC               2000000
#define LEG_2450_F0                2405000000
#define LEG_2450_CH_SPAC              5000000

/** OFDM constants */
#define OFDM_470_OPT4_F0            470400000
#define OFDM_470_OPT4_CH_SPAC          400000
#define OFDM_780_OPT1_F0            780200000
#define OFDM_780_OPT1_CH_SPAC         1200000
#define OFDM_780_OPT2_F0            779800000
#define OFDM_780_OPT2_CH_SPAC          800000
#define OFDM_780_OPT3_F0            779400000
#define OFDM_780_OPT3_CH_SPAC          400000
#define OFDM_780_OPT4_F0            779200000
#define OFDM_780_OPT4_CH_SPAC          200000
#define OFDM_863_OPT1_F0            863625000
#define OFDM_863_OPT1_CH_SPAC         1200000
#define OFDM_863_OPT2_F0            863425000
#define OFDM_863_OPT2_CH_SPAC          800000
#define OFDM_863_OPT3_F0            863225000
#define OFDM_863_OPT3_CH_SPAC          400000
#define OFDM_863_OPT4_F0            863125000
#define OFDM_863_OPT4_CH_SPAC          200000
#define OFDM_915_OPT1_F0            903200000
#define OFDM_915_OPT1_CH_SPAC         1200000
#define OFDM_915_OPT2_F0            902800000
#define OFDM_915_OPT2_CH_SPAC          800000
#define OFDM_915_OPT3_F0            902400000
#define OFDM_915_OPT3_CH_SPAC          400000
#define OFDM_915_OPT4_F0            902200000
#define OFDM_915_OPT4_CH_SPAC          200000
#define OFDM_917_OPT1_F0            917900000
#define OFDM_917_OPT1_CH_SPAC         1200000
#define OFDM_917_OPT2_F0            917500000
#define OFDM_917_OPT2_CH_SPAC          800000
#define OFDM_917_OPT3_F0            917300000
#define OFDM_917_OPT3_CH_SPAC          400000
#define OFDM_917_OPT4_F0            917100000
#define OFDM_917_OPT4_CH_SPAC          200000
#define OFDM_920_OPT1_F0            921200000
#define OFDM_920_OPT1_CH_SPAC         1200000
#define OFDM_920_OPT2_F0            920800000
#define OFDM_920_OPT2_CH_SPAC          800000
#define OFDM_920_OPT3_F0            920400000
#define OFDM_920_OPT3_CH_SPAC          400000
#define OFDM_920_OPT4_F0            920200000
#define OFDM_920_OPT4_CH_SPAC          200000
#define OFDM_950_OPT2_F0            951300000
#define OFDM_950_OPT2_CH_SPAC          800000
#define OFDM_950_OPT3_F0            951100000
#define OFDM_950_OPT3_CH_SPAC          400000
#define OFDM_950_OPT4_F0            951000000
#define OFDM_950_OPT4_CH_SPAC          200000
#define OFDM_2450_OPT1_F0          2401200000
#define OFDM_2450_OPT1_CH_SPAC        1200000
#define OFDM_2450_OPT2_F0          2400800000
#define OFDM_2450_OPT2_CH_SPAC         800000
#define OFDM_2450_OPT3_F0          2400400000
#define OFDM_2450_OPT3_CH_SPAC         400000
#define OFDM_2450_OPT4_F0          2400200000
#define OFDM_2450_OPT4_CH_SPAC         200000

/** MR-O-QPSK constants */
#define OQPSK_915_F0                904000000
#define OQPSK_915_CH_SPAC             2000000

#define OQPSK_2450_F0              2405000000
#define OQPSK_2450_CH_SPAC            5000000

#define OQPSK_470_F0                470400000
#define OQPSK_470_CH_SPAC              400000
#define OQPSK_780_F0                780000000
#define OQPSK_780_CH_SPAC             2000000
#define OQPSK_863_F0                868300000
#define OQPSK_863_CH_SPAC              650000
#define OQPSK_915_F0                904000000
#define OQPSK_915_CH_SPAC             2000000
#define OQPSK_917_F0                918100000
#define OQPSK_917_CH_SPAC             2000000
#define OQPSK_920_F0                920600000
#define OQPSK_920_CH_SPAC              200000
#define OQPSK_2450_F0              2405000000
#define OQPSK_2450_CH_SPAC            5000000

/** FSK constants */
#define FSK_470_MOD1_F0             470200000
#define FSK_470_MOD1_CH_SPAC           200000
#define FSK_470_MOD2_F0             470400000
#define FSK_470_MOD2_CH_SPAC           400000
#define FSK_470_MOD3_F0             470400000
#define FSK_470_MOD3_CH_SPAC           400000
#define FSK_780_MOD1_F0             779200000
#define FSK_780_MOD1_CH_SPAC           200000
#define FSK_780_MOD2_F0             779400000
#define FSK_780_MOD2_CH_SPAC           400000
#define FSK_780_MOD3_F0             779400000
#define FSK_780_MOD3_CH_SPAC           400000
#define FSK_863_MOD1_F0             863125000
#define FSK_863_MOD1_CH_SPAC           200000
#define FSK_863_MOD2_F0             863225000
#define FSK_863_MOD2_CH_SPAC           400000
#define FSK_863_MOD3_F0             863225000
#define FSK_863_MOD3_CH_SPAC           400000
#define FSK_915_MOD1_F0             902200000
#define FSK_915_MOD1_CH_SPAC           200000
#define FSK_915_MOD2_F0             902400000
#define FSK_915_MOD2_CH_SPAC           400000
#define FSK_915_MOD3_F0             902400000
#define FSK_915_MOD3_CH_SPAC           400000
#define FSK_917_MOD1_F0             917100000
#define FSK_917_MOD1_CH_SPAC           200000
#define FSK_917_MOD2_F0             917300000
#define FSK_917_MOD2_CH_SPAC           400000
#define FSK_917_MOD3_F0             917300000
#define FSK_917_MOD3_CH_SPAC           400000
#define FSK_920_MOD1_F0             920600000
#define FSK_920_MOD1_CH_SPAC           200000
#define FSK_920_MOD2_F0             920900000
#define FSK_920_MOD2_CH_SPAC           400000
#define FSK_920_MOD3_F0             920800000
#define FSK_920_MOD3_CH_SPAC           600000
#define FSK_920_MOD4_F0             920800000
#define FSK_920_MOD4_CH_SPAC           600000
#define FSK_950_MOD1_F0             951000000
#define FSK_950_MOD1_CH_SPAC           200000
#define FSK_950_MOD2_F0             951100000
#define FSK_950_MOD2_CH_SPAC           400000
#define FSK_950_MOD3_F0             951200000
#define FSK_950_MOD3_CH_SPAC           600000
#define FSK_950_MOD4_F0             951200000
#define FSK_950_MOD4_CH_SPAC           600000
#define FSK_2450_MOD1_F0           2400200000
#define FSK_2450_MOD1_CH_SPAC          200000
#define FSK_2450_MOD2_F0           2400400000
#define FSK_2450_MOD2_CH_SPAC          400000
#define FSK_2450_MOD3_F0           2400400000
#define FSK_2450_MOD3_CH_SPAC          400000

typedef struct oqpsk_tag
{
    oqpsk_chip_rate_t chip_rate;
    oqpsk_rate_mode_t rate_mode;
} oqpsk_t;

typedef struct ofdm_tag
{
    ofdm_option_t option;
    ofdm_mcs_val_t mcs_val;
    bool interl;
} ofdm_t;

typedef struct leg_oqpsk_tag
{
    oqpsk_chip_rate_t chip_rate;
} leg_oqpsk_t;

typedef enum op_mode_tag
{
    FSK_OP_MOD_1,
    FSK_OP_MOD_2,
    FSK_OP_MOD_3,
    FSK_OP_MOD_4
} SHORTENUM op_mode_t;

typedef enum mod_idx_tag
{
    MOD_IDX_0_33,
    MOD_IDX_0_5,
    MOD_IDX_1_0
} SHORTENUM mod_idx_t;

typedef enum data_rate_tag
{
    DATA_RATE_50,
    DATA_RATE_100,
    DATA_RATE_150,
    DATA_RATE_200,
    DATA_RATE_400
} SHORTENUM data_rate_t;

typedef struct fsk_tag
{
    op_mode_t op_mode;
    mod_idx_t mod_idx;
    data_rate_t data_rate;
} fsk_t;

/**
 * PHY mode structure
 */
typedef union phy_mode_tag
{
    fsk_t fsk;
    ofdm_t ofdm;
    oqpsk_t oqpsk;
    leg_oqpsk_t leg_oqpsk;
} phy_mode_t;

typedef struct phy_tag
{
    modulation_t modulation;
    sun_freq_band_t freq_band;
    uint32_t freq_f0;
    uint32_t ch_spacing;
    phy_mode_t phy_mode;
} phy_t;


/*** TO BE VERIFIED ******/ //todo


typedef struct mr_oqpsk_tag
{
	oqpsk_chip_rate_t chip_rate;
	oqpsk_rate_mode_t rate_mode;
} mr_oqpsk_t;

typedef union sun_phy_mode_tag
{
	fsk_t mr_fsk;
	ofdm_t mr_ofdm;
	oqpsk_rate_mode_t mr_oqpsk_rate_mode;

} sun_phy_mode_t;

typedef struct sun_phy_tag
{
	uint8_t page_no ;
	sun_freq_band_t freq_band;	
	modulation_t modulation;
	sun_phy_mode_t sun_phy_mode;
} sun_phy_t;

/* === Externals ============================================================ */

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ieee_15_4g_H */
/* EOF */
