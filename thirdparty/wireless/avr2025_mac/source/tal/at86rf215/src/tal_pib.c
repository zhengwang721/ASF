/**
 * @file tal_pib.c
 *
 * @brief This file handles the TAL PIB attributes, set/get and initialization
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

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

static retval_t apply_channel_settings(trx_id_t trx_id);
static bool apply_channel_page_configuration(trx_id_t trx ,uint8_t ch_page);
static retval_t check_valid_freq_range(trx_id_t trx_id);
static int8_t limit_tx_pwr(trx_id_t trx_id, int8_t tx_pwr);
static void set_tx_pwr(trx_id_t trx_id);
/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Initialize the TAL PIB
 *
 * This function initializes the TAL information base attributes
 * to their default values.
 *
 * @param trx_id Transceiver identifier
 */
void init_tal_pib(trx_id_t trx_id)
{
    //debug_text(PSTR("init_tal_pib()"));

    tal_pib[trx_id].PANId = TAL_PANID_BC_DEF;
    tal_pib[trx_id].ShortAddress = TAL_SHORT_ADDRESS_DEF;

    if (trx_id == RF09)
    {
        tal_pib[RF09].MaxCSMABackoffs = TAL_RF09_MAX_CSMA_BACKOFFS_DEF;
        tal_pib[RF09].MinBE = TAL_RF09_MINBE_DEF;
        tal_pib[RF09].MaxBE = TAL_RF09_MAXBE_DEF;
        tal_pib[RF09].MaxFrameRetries = TAL_RF09_MAXFRAMERETRIES_DEF;
#ifdef RX_WHILE_BACKOFF
        tal_pib[RF09].MaxNumRxFramesDuringBackoff = TAL_RF09_MAX_FRAMES_DURING_BACKOFF_DEF;
#endif
        tal_pib[RF09].PrivatePanCoordinator = TAL_RF09_PAN_COORDINATOR_DEF;
#ifdef SUPPORT_LEGACY_OQPSK
		tal_pib[RF09].CurrentChannel = TAL_RF09_CURRENT_CHANNEL_LEG_DEF;
#else
		tal_pib[RF09].CurrentChannel = TAL_RF09_CURRENT_CHANNEL_DEF;
#endif	
        tal_pib[RF09].FCSType = TAL_RF09_FCS_TYPE_DEFAULT;
        tal_pib[RF09].phy.modulation = TAL_RF09_MODULATION_DEF;
        tal_pib[RF09].phy.freq_band = TAL_RF09_FRQ_BAND_DEF;
        tal_pib[RF09].phy.freq_f0 = TAL_RF09_FRQ_F0;
        tal_pib[RF09].phy.ch_spacing = TAL_RF09_CH_SPACING;
        tal_pib[RF09].phy.phy_mode.ofdm.option = TAL_RF09_OFDM_OPT_DEF;
        tal_pib[RF09].phy.phy_mode.ofdm.mcs_val = TAL_RF09_OFDM_MCS_VAL_DEF;
        tal_pib[RF09].phy.phy_mode.ofdm.interl = TAL_RF09_OFDM_INTERL_DEF;
        tal_pib[RF09].TransmitPower = TAL_RF09_OFDM_MAX_TX_PWR;
#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
        tal_pib[RF09].AdaptDataRateForACK = TAL_RF09_ADAPT_DATA_RATE_FOR_ACK;
#endif
#ifdef FRAME_FILTER_CONFIGURATION
        tal_pib[RF09].frame_types = TAL_RF09_FRAMES_TYPES;
        tal_pib[RF09].frame_versions = TAL_RF09_FRAME_VERSIONS;
#endif /* #ifdef FRAME_FILTER_CONFIGURATION */
 
    }
    else    // RF24
    {
        tal_pib[RF24].MaxCSMABackoffs = TAL_RF24_MAX_CSMA_BACKOFFS_DEF;
        tal_pib[RF24].MinBE = TAL_RF24_MINBE_DEF;
        tal_pib[RF24].MaxBE = TAL_RF24_MAXBE_DEF;
        tal_pib[RF24].MaxFrameRetries = TAL_RF24_MAXFRAMERETRIES_DEF;
#ifdef RX_WHILE_BACKOFF
        tal_pib[RF24].MaxNumRxFramesDuringBackoff = TAL_RF24_MAX_FRAMES_DURING_BACKOFF_DEF;
#endif
        tal_pib[RF24].PrivatePanCoordinator = TAL_RF24_PAN_COORDINATOR_DEF;
#ifdef SUPPORT_LEGACY_OQPSK		
        tal_pib[RF24].CurrentChannel = TAL_RF24_CURRENT_CHANNEL_LEG_DEF;
#else	
		tal_pib[RF24].CurrentChannel = TAL_RF24_CURRENT_CHANNEL_DEF;
#endif			
        tal_pib[RF24].FCSType = TAL_RF24_FCS_TYPE_DEFAULT;
        tal_pib[RF24].phy.modulation = TAL_RF24_MODULATION_DEF;
        tal_pib[RF24].phy.freq_band = TAL_RF24_FRQ_BAND_DEF;
        tal_pib[RF24].phy.freq_f0 = TAL_RF24_FRQ_F0;
        tal_pib[RF24].phy.ch_spacing = TAL_RF24_CH_SPACING;
        tal_pib[RF24].phy.phy_mode.ofdm.option = TAL_RF24_OFDM_OPT_DEF;
        tal_pib[RF24].phy.phy_mode.ofdm.mcs_val = TAL_RF24_OFDM_MCS_VAL_DEF;
        tal_pib[RF24].phy.phy_mode.ofdm.interl = TAL_RF24_OFDM_INTERL_DEF;
        tal_pib[RF24].TransmitPower = TAL_RF24_OFDM_MAX_TX_PWR;
#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
        tal_pib[RF24].AdaptDataRateForACK = TAL_RF24_ADAPT_DATA_RATE_FOR_ACK;
#endif
#ifdef FRAME_FILTER_CONFIGURATION
        tal_pib[RF24].frame_types = TAL_RF24_FRAMES_TYPES;
        tal_pib[RF24].frame_versions = TAL_RF24_FRAME_VERSIONS;
#endif /* #ifdef FRAME_FILTER_CONFIGURATION */
    }

#ifdef PROMISCUOUS_MODE
    tal_pib[trx_id].PromiscuousMode = false;
#endif
#ifdef MEASURE_ON_AIR_DURATION
    tal_pib[trx_id].OnAirDuration = 0;
#endif
}


/**
 * @brief Configures the PHY
 *
 * This function configures the PHY related settings.
 *
 * @param trx_id Transceiver identifier
 *
 * @return MAC_SUCCESS if the configuration was successful
 */
retval_t config_phy(trx_id_t trx_id)
{
    retval_t status;

    status = conf_trx_modulation(trx_id);
    if (status == MAC_SUCCESS)
    {
#ifdef SUPPORT_LEGACY_OQPSK
        uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_FCST, tal_pib[trx_id].FCSType);
#endif
        status = apply_channel_settings(trx_id);
        /* Set CCA measurement average duration */
        if (status == MAC_SUCCESS)
        {
            set_ed_sample_duration(trx_id, tal_pib[trx_id].CCADuration_us);
        }
    }

    return status;
}

/*
 * \brief Sets transceiver state
 *
 * \param trx_cmd needs to be one of the trx commands
 *
 * \return current trx state
 */
rf_cmd_status_t set_trx_state(trx_id_t trx,rf_cmd_state_t trx_cmd)
{

	//check
	return STATUS_RF_TRXOFF;
}
/**
 * @brief Calculates PIB values that depend on the current PHY configuration
 *
 * @param trx_id Transceiver identifier
 */
void calculate_pib_values(trx_id_t trx_id)
{
    //debug_text(PSTR("calculate_pib_values()"));

    /* Do not change the following order; some values are used to calculate others. */
    tal_pib[trx_id].SymbolDuration_us = get_symbol_duration_us(trx_id);

#ifdef SUPPORT_LEGACY_OQPSK
    if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
    {
        tal_pib[trx_id].OctetDuration_us = 32;
        tal_pib[trx_id].FCSType = FCS_TYPE_2_OCTETS;
        tal_pib[trx_id].FCSLen = 2;
        tal_pib[trx_id].MaxPHYPacketSize = aMaxPHYPacketSize;
#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
        tal_pib[trx_id].AdaptDataRateForACK = false;
#endif
    }
    else // SUN PHYs
#endif
    {
        tal_pib[trx_id].OctetDuration_us = (1000 * (uint16_t)8) / get_data_rate(trx_id);
        tal_pib[trx_id].FCSType = FCS_TYPE_4_OCTETS;
        tal_pib[trx_id].FCSLen = 4;
        tal_pib[trx_id].MaxPHYPacketSize = 2047;
    }
    tal_pib[trx_id].CCADuration_us = get_cca_duration_us(trx_id);
    tal_pib[trx_id].CCADuration_sym = tal_pib[trx_id].CCADuration_us / tal_pib[trx_id].SymbolDuration_us;
    tal_pib[trx_id].CCAThreshold = get_cca_thres(trx_id);
    tal_pib[trx_id].ACKWaitDuration = get_AckWaitDuration_us(trx_id);
    tal_pib[trx_id].ACKTiming = get_AckTiming_us(trx_id);
}


/**
 * @brief Write all shadow PIB variables to the transceiver
 *
 * This function writes all shadow PIB variables to the transceiver.
 * It is assumed that the radio does not sleep and has gone through a reset.
 *
 * @param trx_id Transceiver identifier
 */
void write_all_tal_pib_to_trx(trx_id_t trx_id)
{
    //debug_text_val(PSTR("write_all_tal_pib_to_trx trx_id = "), trx_id);
    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;

    if (tal_pib[trx_id].FCSType != FCS_TYPE_4_OCTETS) // Compared against reset value
    {
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_FCST, tal_pib[trx_id].FCSType);
    }

    /* Setup frame filter registers */
    pal_trx_write(bb_reg_offset + RG_BBC0_MACPID0F0,
                  (uint8_t *)&tal_pib[trx_id].PANId, 2);
    pal_trx_write(bb_reg_offset + RG_BBC0_MACSHA0F0,
                  (uint8_t *)&tal_pib[trx_id].ShortAddress, 2);
    pal_trx_write(bb_reg_offset + RG_BBC0_MACEA0,
                  (uint8_t *)&tal_pib[trx_id].IeeeAddress, 8);
    if (tal_pib[trx_id].PrivatePanCoordinator)
    {
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_AFC1_PANC0,
                          tal_pib[trx_id].PrivatePanCoordinator);
    }
#ifdef FRAME_FILTER_CONFIGURATION
    uint8_t temp[2];
    temp[0] = tal_pib[trx_id].frame_types;
    temp[1] = tal_pib[trx_id].frame_versions;
    pal_trx_write(bb_reg_offset + RG_BBC0_AFFTM, temp, 2);
#endif
#ifdef PROMISCUOUS_MODE
    if (tal_pib[trx_id].PromiscuousMode)
    {
        pal_trx_bit_write(bb_reg_offset + SR_BBC0_AFC0_PM, 1);
        if (trx_state[trx_id] != RF_RX)
        {
            switch_to_rx(trx_id);
        }
    }
#endif
    /* Set tx power */
    set_tx_pwr(trx_id);
}


/**
 * @brief Limits the transmit power to its borders
 *
 * @param trx_id Transceiver identifier
 * @param tx_pwr Transmit power in dBm
 *
 * @return Limited transmit power in dBm
 */
static int8_t limit_tx_pwr(trx_id_t trx_id, int8_t tx_pwr)
{
    int8_t pwr;

/*
    if ((tal_pib[trx_id].phy.modulation == OFDM) && (tx_pwr > 11))
    {
        pwr = 11;
    }
    else*/
    {
        if (tx_pwr > 14)
        {
            pwr = 14;
        }
        else if (tx_pwr < -17)
        {
            pwr = -17;
        }
        else
        {
            pwr = tx_pwr;
        }
    }

    return pwr;
}


/**
 * @brief Sets the transmit power
 *
 * @param trx_id Transceiver identifier
 */
static void set_tx_pwr(trx_id_t trx_id)
{
    /* Scale dBm value to register value */
    uint8_t val = (uint8_t)(tal_pib[trx_id].TransmitPower + 17);
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    pal_trx_bit_write(rf_reg_offset + SR_RF09_PAC_TXPWR, val);
}


/**
 * @brief Check if freq spacing and frequency are in the supported range
 *
 * @param trx_id Transceiver identifier
 *
 * @return MAC_SUCCESS if frequency spacing and frequency are supported
           else FAILURE
 */
static retval_t check_valid_freq_range(trx_id_t trx_id)
{
    retval_t status = MAC_SUCCESS;

    uint32_t freq = tal_pib[trx_id].phy.freq_f0;
    uint32_t spacing = tal_pib[trx_id].phy.ch_spacing;

    if ((spacing % 25000) > 0)
    {
        status = FAILURE;
    }
    else
    {
        if (trx_id == RF09)
        {
            if (((freq < 389500000) || (freq > 1020000000)) ||
                ((freq > 510000000) && (freq < 779000000)))
            {
                status = FAILURE;
            }
        }
        else /* RF24 */
        {
            if ((freq < 2400000000) || (freq > 2483500000))
            {
                status = FAILURE;
            }
        }
    }

    return status;
}


/**
 * @brief Applies channel settings by writing them to the transceiver
 *
 * @param trx_id Transceiver identifier
 *
 * @return MAC_SUCCESS if channel setting is successful
                       else FAILURE
 */
static retval_t apply_channel_settings(trx_id_t trx_id)
{
    //debug_text_val(PSTR("apply_channel_settings(), trx_id ="), trx_id);

    /* Check if spacing and frequency are within the correct range. */
    retval_t status = check_valid_freq_range(trx_id);
    if (status == MAC_SUCCESS)
    {
        uint32_t freq = tal_pib[trx_id].phy.freq_f0;
        uint32_t spacing = tal_pib[trx_id].phy.ch_spacing;
        uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        uint16_t reg_val;
		uint8_t temp_val;

        /* Offset handling for 2.4GHz only */
        if (trx_id == RF24)
        {
            freq -= 1500000000;
        }
        reg_val = (uint16_t)(freq / 25000);
        pal_trx_write(rf_reg_offset + RG_RF09_CCF0L, (uint8_t *)&reg_val, 2);

        /* Set channel spacing */
        spacing /= 25000; // adjust to register scaling
        pal_trx_reg_write(rf_reg_offset + RG_RF09_CS, (uint8_t)spacing);

        /*
         * Set channel and channel mode.
         * Touching the CNM register forces the calculation of the actual frequency.
         */
//#ifdef	SUPPORT_LEGACY_OQPSK
if(tal_pib[trx_id].phy.modulation == LEG_OQPSK)
{

uint16_t value;
 if ((tal_pib[trx_id].phy.freq_band == CHINA_780))
 {
	 if(tal_pib[trx_id].CurrentChannel>3)
	 {	 
	 value = tal_pib[trx_id].CurrentChannel=3; //check
	 }
	 else
	 {
	
	 value = tal_pib[trx_id].CurrentChannel;
	 }
 }
	 else if ((tal_pib[trx_id].phy.freq_band == WORLD_2450))
	 {

if((tal_pib[trx_id].CurrentChannel >26) || (tal_pib[trx_id].CurrentChannel < 11))
{
	tal_pib[trx_id].CurrentChannel=11;
	value = tal_pib[trx_id].CurrentChannel-11;
}
else
{
	
	value = tal_pib[trx_id].CurrentChannel-11;
}
	 
	 }
	 
	else if ((tal_pib[trx_id].phy.freq_band == US_915))
	{
	if((tal_pib[trx_id].CurrentChannel >10) || (tal_pib[trx_id].CurrentChannel < 1))
	{
		tal_pib[trx_id].CurrentChannel=1;
		value = tal_pib[trx_id].CurrentChannel-1;
	}
	else
	{
		
		value = tal_pib[trx_id].CurrentChannel-1;
	}
	}
	
	 pal_trx_write(rf_reg_offset + RG_RF09_CNL,
	 (uint8_t *)&value, 2);
 
 
}
else
	 {
		uint16_t max_ch = get_sun_max_ch_no(trx_id);
		if(tal_pib[trx_id].CurrentChannel > (max_ch-1))
		{
		tal_pib[trx_id].CurrentChannel = (max_ch-1);		
		}
		if ((tal_pib[trx_id].phy.freq_band == EU_863)&&(tal_pib[trx_id].phy.modulation == OQPSK))
		{
			if(tal_pib[trx_id].CurrentChannel == 0)
			{
				freq = 868300000;


			}			
			else if(tal_pib[trx_id].CurrentChannel == 1)
			{
				 freq = 868950000;

			}
			else if(tal_pib[trx_id].CurrentChannel == 2)
			{
				freq = 869525000;

			}	
			temp_val = 0;
			reg_val = (uint16_t)(freq / 25000);					
			pal_trx_write(rf_reg_offset + RG_RF09_CCF0L, (uint8_t *)&reg_val, 2);
			pal_trx_write(rf_reg_offset + RG_RF09_CNL,
			(uint8_t *)&temp_val, 2); // write cnl as 0 to get the same freq as freq			
		}
		else
		{
        pal_trx_write(rf_reg_offset + RG_RF09_CNL,
                      (uint8_t *)&tal_pib[trx_id].CurrentChannel, 2);
		}
}					  
        /* Wait until channel set is completed */
        if (trx_state[trx_id] == RF_TXPREP)
        {
            while (TAL_RF_IS_IRQ_SET(trx_id, RF_IRQ_TRXRDY) == 0)
            {
                /* Wait until new channel is set */
            }
            TAL_RF_IRQ_CLR(trx_id, RF_IRQ_TRXRDY);
            //debug_text(PSTR("RF_IRQ_TRXRDY: channel change completed"));
        }
    }

    return status;
}


/**
 * @brief Gets a TAL PIB attribute
 *
 * This function is called to retrieve the transceiver information base
 * attributes.
 *
 * @param[in] trx_id Transceiver identifier
 * @param[in] attribute TAL infobase attribute ID
 * @param[out] value TAL infobase attribute value
 *
 * @return MAC_UNSUPPORTED_ATTRIBUTE if the TAL infobase attribute is not found
 *         MAC_SUCCESS otherwise
 */
#if (HIGHEST_STACK_LAYER == TAL) || (defined DOXYGEN)
retval_t tal_pib_get(trx_id_t trx_id, uint8_t attribute, uint8_t *value)
{
    retval_t status = MAC_SUCCESS;

    switch (attribute)
    {
        case phySetting:
            memcpy(value, &tal_pib[trx_id].phy, sizeof(phy_t));
            break;
			
        case phyCurrentPage:
       *(uint8_t *)value = tal_pib[trx_id].CurrentPage;
        break;			

        case phyCurrentChannel:
#ifdef SUPPORT_LEGACY_OQPSK
            /* Adjust internal channel number to IEEE compliant numbering */
            if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
            {
				 *(uint16_t *)value = tal_pib[trx_id].CurrentChannel ;
/*
                if (trx_id == RF24)
                {
                    *(uint16_t *)value = tal_pib[trx_id].CurrentChannel + 11;
                }
                else // RF09
                {
                    if (tal_pib[trx_id].phy.freq_band == US_915)
                    {
                        *(uint16_t *)value = tal_pib[trx_id].CurrentChannel + 1;
                    }
					else
					{
						*(uint16_t *)value = tal_pib[trx_id].CurrentChannel ;
					}
					
                }*/
            }
            else

#endif
            {
                *(uint16_t *)value = tal_pib[trx_id].CurrentChannel;
            }
            break;

#ifdef SUPPORT_LEGACY_OQPSK
        case phyChannelsSupported:
            if ((tal_pib[trx_id].phy.modulation == LEG_OQPSK))
            {
                if (trx_id == RF09)
                {
                    if ((tal_pib[trx_id].phy.freq_band == CHINA_470)||(tal_pib[trx_id].phy.freq_band == CHINA_780))
                    {
                        *(uint32_t *)value = 0x0000000F;
                    }
                    else if ((tal_pib[trx_id].phy.freq_band == US_915))
                    {
                        *(uint32_t *)value = 0x000007FE;
                    }
                    else
                    {
                        status = MAC_UNSUPPORTED_ATTRIBUTE;
                    }
                }
                else // RF24
                {
                    *(uint32_t *)value = 0x07FFF800;
                }
            }
            else
            {
                status = MAC_UNSUPPORTED_ATTRIBUTE;
            }
            break;
#endif

        case phyTransmitPower:
            *(int8_t *)value = tal_pib[trx_id].TransmitPower;
            break;

        case phyCCADuration: /* CCA duration in symbols */
            *(uint8_t *)value = tal_pib[trx_id].CCADuration_sym;
            break;

        case phyCCATimeMethod:
            *(bool *)value = tal_pib[trx_id].CCATimeMethod;
            break;

        case phyCCAThreshold:
            *(uint8_t *)value = tal_pib[trx_id].CCAThreshold;
            break;

#if 0
#ifdef SUPPORT_FSK
        case phyFSKFECEnabled:
#if 0
            *(bool *)value = tal_pib[trx_id].phy.phy_mode.fsk.;
            if (tal_pib[trx_id].phy.modulation == FSK)
            {
                /*
                tal_pib[trx_id].phy.phy_mode.fsk.interl = value->pib_value_bool;
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_POI,
                                  tal_pib[trx_id].phy.phy_mode.ofdm.interl);
                calculate_pib_values(trx_id);
                */
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
#endif
            break;

        case phyFSKFECInterleavingRSC:
#if 0
            if (tal_pib[trx_id].phy.modulation == FSK)
            {
                /*
                tal_pib[trx_id].phy.phy_mode.fsk.interl = value->pib_value_bool;
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_POI,
                                  tal_pib[trx_id].phy.phy_mode.ofdm.interl);
                calculate_pib_values(trx_id);
                */
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
#endif
            break;

        case phyFSKFECScheme:
#if 0
            if (tal_pib[trx_id].phy.modulation == FSK)
            {
                /*
                tal_pib[trx_id].phy.phy_mode.fsk.interl = value->pib_value_bool;
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_POI,
                                  tal_pib[trx_id].phy.phy_mode.ofdm.interl);
                calculate_pib_values(trx_id);
                */
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
#endif
            break;
#endif /* #ifdef SUPPORT_FSK */
#endif

#ifdef SUPPORT_OFDM
        case phyOFDMMCS:
            *(ofdm_mcs_val_t *)value = tal_pib[trx_id].phy.phy_mode.ofdm.mcs_val;
            break;

        case phyOFDMInterleaving:
            *(bool *)value = tal_pib[trx_id].phy.phy_mode.ofdm.interl;
            break;
#endif /* #ifdef SUPPORT_OFDM */

#ifdef RX_WHILE_BACKOFF
        case macMaxNumRxFramesDuringBackoff:
            *value = tal_pib[trx_id].MaxNumRxFramesDuringBackoff;
            break;
#endif

        case macMaxCSMABackoffs:
            *value = tal_pib[trx_id].MaxCSMABackoffs;
            break;

        case macMinBE:
            *value = tal_pib[trx_id].MinBE;
            break;

        case macMaxBE:
            *value = tal_pib[trx_id].MaxBE;
            break;

        case phyCCAMode:
            *value = tal_pib[trx_id].CCAMode;
            break;

        case macFCSType:
            *(bool *)value = tal_pib[trx_id].FCSType;
            break;

        case macAckWaitDuration:
            *(uint16_t *)value = tal_pib[trx_id].ACKWaitDuration;
            break;

        case macACKTiming:
            *(uint16_t *)value = tal_pib[trx_id].ACKTiming;
            break;

        case macMaxFrameRetries:
            *value = tal_pib[trx_id].MaxFrameRetries;
            break;

        case phyMaxFrameDuration:
            *(uint16_t *)value = tal_pib[trx_id].MaxFrameDuration;
            break;

        case phySymbolsPerOctet:
            *value = tal_pib[trx_id].SymbolsPerOctet;
            break;

#ifdef PROMISCUOUS_MODE
        case macPromiscuousMode:
            *(bool *)value = tal_pib[trx_id].PromiscuousMode;
            break;
#endif

        case macPANId:
            *(uint16_t *)value = tal_pib[trx_id].PANId;
            break;

        case macShortAddress:
            *(uint16_t *)value = tal_pib[trx_id].ShortAddress;
            break;

        case macIeeeAddress:
            *(uint64_t *)value = tal_pib[trx_id].IeeeAddress;
            break;

        case mac_i_pan_coordinator:
            *(bool *)value = tal_pib[trx_id].PrivatePanCoordinator;
            break;

#ifdef MEASURE_ON_AIR_DURATION
        case phyOnAirDuration:
            *(uint32_t *)value = tal_pib[trx_id].OnAirDuration;
            break;
#endif

#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
        case macAdaptDataRateForACK:
            *(bool *)value = tal_pib[trx_id].AdaptDataRateForACK;
            break;
#endif

#ifdef FRAME_FILTER_CONFIGURATION
        case macFrameFilterFrameTypes:
            *(uint8_t *)value = tal_pib[trx_id].frame_types;
            break;

        case macFrameFilterFrameVersions:
            *(uint8_t *)value = tal_pib[trx_id].frame_versions;
            break;
#endif /* #ifdef FRAME_FILTER_CONFIGURATION */

        default:
            /* Invalid attribute id */
            status = MAC_UNSUPPORTED_ATTRIBUTE;
            break;
    }

    return status;
} /* tal_pib_get() */
#endif  /* (HIGHEST_STACK_LAYER == TAL) */


/**
 * @brief Sets a TAL PIB attribute
 *
 * This function is called to set the transceiver information base
 * attributes.
 *
 * @param trx_id Transceiver identifier
 * @param attribute TAL infobase attribute ID
 * @param value TAL infobase attribute value to be set
 *
 * @return MAC_UNSUPPORTED_ATTRIBUTE if the TAL info base attribute is not found
 *         TAL_BUSY if the TAL is not in TAL_IDLE state. An exception is
 *         macBeaconTxTime which can be accepted by TAL even if TAL is not
 *         in TAL_IDLE state.
 *         MAC_SUCCESS if the attempt to set the PIB attribute was successful
 *         TAL_TRX_ASLEEP if trx is in SLEEP mode
 */
retval_t tal_pib_set(trx_id_t trx_id, uint8_t attribute, pib_value_t *value)
{
    //debug_text(PSTR("tal_pib_set()"));

    retval_t status = MAC_SUCCESS;

    /*
     * Do not allow any changes while TAL is in SLEEP or busy.
     */
    if (tal_state[trx_id] == TAL_SLEEP)
    {
        //debug_text(PSTR("TAL_TRX_ASLEEP"));
        return TAL_TRX_ASLEEP;
    }

    if (tal_state[trx_id] != TAL_IDLE)
    {
        //debug_text_val(PSTR("TAL is busy or asleep, tal_state = "), tal_state[trx_id]);
        return TAL_BUSY;
    }

    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    switch (attribute)
    {
        case phySetting:
            //debug_text(PSTR("PIB attribute: phySetting"));
            {
                /* Store previous settings */
                phy_t previous_phy;
                memcpy(&previous_phy, &tal_pib[trx_id].phy, sizeof(phy_t));
                memcpy(&tal_pib[trx_id].phy, value, sizeof(phy_t));
                calculate_pib_values(trx_id);
                status = config_phy(trx_id);
                if (status != MAC_SUCCESS)
                {
                    /* Restore previous values */
                    memcpy(&tal_pib[trx_id].phy, &previous_phy, sizeof(phy_t));
                    calculate_pib_values(trx_id);
                }
            }
            break;

        case phyCurrentChannel:
{
			uint16_t channel,channel_to_set;
			channel = channel_to_set = value->pib_value_16bit;
			uint32_t freq ;
			uint16_t reg_val;

            /* Adjust internal channel number to IEEE compliant numbering */
            if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
            {

                if (trx_id == RF24)
                {
                    if ((channel < 11) ||
                        (channel > 26))
                    {
                        status = MAC_INVALID_PARAMETER;
                        /* no further processing of the channel value */
                        break;
                    }
                    else
                    {
                        channel -= 11;
                    }
                }
                else // RF09
                {
                    if ((tal_pib[trx_id].phy.freq_band == US_915))
                    {
                        if ((channel < 1)&& (channel > 10))
                        {
							
                            status = MAC_INVALID_PARAMETER;
                            /* no further processing of the channel value */
                            break;
                        }

						channel -= 1;

                    }
					else if (tal_pib[trx_id].phy.freq_band == CHINA_780)
					{
					 if (channel > 3)
					 {						 
						 status = MAC_INVALID_PARAMETER;
						 /* no further processing of the channel value */
						 break;
					 }
					
					}
					else{
						
						status = MAC_INVALID_PARAMETER;
						/* no further processing of the channel value */
						break;
						
					}

                }
            }
			else
			{
				uint16_t max_ch = get_sun_max_ch_no(trx_id);
				if(channel > (max_ch-1))
				{
                status = MAC_INVALID_PARAMETER;
                /* no further processing of the channel value */
                break;
				}
			}

            {
                rf_cmd_state_t previous_state = trx_state[trx_id];
                if (trx_state[trx_id] == RF_RX)
                {
                    /* Set TXPREP and wait until it is reached. */
                    switch_to_txprep(trx_id);
                }
				tal_pib[trx_id].CurrentChannel = channel_to_set;
				if ((tal_pib[trx_id].phy.freq_band == EU_863)&&(tal_pib[trx_id].phy.modulation == OQPSK))
				{
					if(tal_pib[trx_id].CurrentChannel == 0)
					{
						freq = 868300000;


					}
					else if(tal_pib[trx_id].CurrentChannel == 1)
					{
						freq = 868950000;

					}
					else if(tal_pib[trx_id].CurrentChannel == 2)
					{
						freq = 869525000;

					}
					channel = 0;
					reg_val = (uint16_t)(freq / 25000);
					pal_trx_write(rf_reg_offset + RG_RF09_CCF0L, (uint8_t *)&reg_val, 2);
					pal_trx_write(rf_reg_offset + RG_RF09_CNL,
					(uint8_t *)&channel, 2); // write cnl as 0 to get the same freq as freq
				}
				else
				{
                pal_trx_write(rf_reg_offset + RG_RF09_CNL,
                (uint8_t *)&channel, 2);
				}			


                if (trx_state[trx_id] == RF_TXPREP)
                {
                    while (TAL_RF_IS_IRQ_SET(trx_id, RF_IRQ_TRXRDY) == 0)
                    {
                        /* Wait until new channel is set */
                    }
                    TAL_RF_IRQ_CLR(trx_id, RF_IRQ_TRXRDY);
                    //debug_text(PSTR("RF_IRQ_TRXRDY: channel change completed"));
                }

                /* restore previous TRX state */
                if (previous_state == RF_RX)
                {
                    switch_to_rx(trx_id);
                }
            }
}
            break;
			
			
		 case phyCurrentPage:
		 {
			 uint8_t page;
			 bool ret_val;
			 page = value->pib_value_8bit;	
	


	ret_val = apply_channel_page_configuration(trx_id,page);
	if (ret_val) {
		
		tal_pib[trx_id].CurrentPage = page;
		}
		else
		{
		return MAC_INVALID_PARAMETER;
		}


		 }
		 break;
	 

        case phyTransmitPower:
            {
                int8_t pwr = (int8_t)value->pib_value_8bit;
                tal_pib[trx_id].TransmitPower = limit_tx_pwr(trx_id, pwr);
                set_tx_pwr(trx_id);
            }
            break;

        case phyCCADuration: /* CCA duration in symbols */
            tal_pib[trx_id].CCADuration_sym = value->pib_value_8bit;
            tal_pib[trx_id].CCADuration_us = calculate_cca_duration_us(trx_id);
            break;

        case phyCCATimeMethod:
            tal_pib[trx_id].CCATimeMethod = value->pib_value_bool;
            tal_pib[trx_id].CCADuration_us = calculate_cca_duration_us(trx_id);
            break;

        case phyCCAThreshold:
            tal_pib[trx_id].CCAThreshold = value->pib_value_8bit;
            break;
#if 0
#ifdef SUPPORT_FSK
        case phyFSKFECEnabled:
            if (tal_pib[trx_id].phy.modulation == FSK)
            {
                /*
                tal_pib[trx_id].phy.phy_mode.fsk.interl = value->pib_value_bool;
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_POI,
                                  tal_pib[trx_id].phy.phy_mode.ofdm.interl);
                calculate_pib_values(trx_id);
                */
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
            break;

        case phyFSKFECInterleavingRSC:
            if (tal_pib[trx_id].phy.modulation == FSK)
            {
                /*
                tal_pib[trx_id].phy.phy_mode.fsk.interl = value->pib_value_bool;
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_POI,
                                  tal_pib[trx_id].phy.phy_mode.ofdm.interl);
                calculate_pib_values(trx_id);
                */
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
            break;

        case phyFSKFECScheme:
            if (tal_pib[trx_id].phy.modulation == FSK)
            {
                /*
                tal_pib[trx_id].phy.phy_mode.fsk.interl = value->pib_value_bool;
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_POI,
                                  tal_pib[trx_id].phy.phy_mode.ofdm.interl);
                calculate_pib_values(trx_id);
                */
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
            break;
#endif /* #ifdef SUPPORT_FSK */
#endif

#ifdef SUPPORT_OFDM
        case phyOFDMMCS:
            if (tal_pib[trx_id].phy.modulation == OFDM)
            {
                tal_pib[trx_id].phy.phy_mode.ofdm.mcs_val =
                    (ofdm_mcs_val_t)value->pib_value_8bit;
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMPHRTX_MCS,
                                  tal_pib[trx_id].phy.phy_mode.ofdm.mcs_val);
                calculate_pib_values(trx_id);
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
            break;

        case phyOFDMInterleaving:
            if (tal_pib[trx_id].phy.modulation == OFDM)
            {
                tal_pib[trx_id].phy.phy_mode.ofdm.interl = value->pib_value_bool;
                pal_trx_bit_write(bb_reg_offset + SR_BBC0_OFDMC_POI,
                                  tal_pib[trx_id].phy.phy_mode.ofdm.interl);
                calculate_pib_values(trx_id);
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
            break;
#endif /* #ifdef SUPPORT_OFDM */

#ifdef RX_WHILE_BACKOFF
        case macMaxNumRxFramesDuringBackoff:
            tal_pib[trx_id].MaxNumRxFramesDuringBackoff = value->pib_value_8bit;
            break;
#endif

        case macMaxCSMABackoffs:
            tal_pib[trx_id].MaxCSMABackoffs = value->pib_value_8bit;
            break;

        case macMinBE:
            tal_pib[trx_id].MinBE = value->pib_value_8bit;
#ifndef REDUCED_PARAM_CHECK
            /*
             * macMinBE must not be larger than macMaxBE or calculation
             * of macMaxFrameWaitTotalTime will fail.
             */
            if (tal_pib[trx_id].MinBE > tal_pib[trx_id].MaxBE)
            {
                tal_pib[trx_id].MinBE = tal_pib[trx_id].MaxBE;
            }
#endif  /* REDUCED_PARAM_CHECK */
            break;

        case macMaxBE:
            tal_pib[trx_id].MaxBE = value->pib_value_8bit;
#ifndef REDUCED_PARAM_CHECK
            /*
             * macMinBE must not be larger than macMaxBE or calculation
             * of macMaxFrameWaitTotalTime will fail.
             */
            if (tal_pib[trx_id].MaxBE < tal_pib[trx_id].MinBE)
            {
                tal_pib[trx_id].MinBE = tal_pib[trx_id].MaxBE;
            }
#endif  /* REDUCED_PARAM_CHECK */
            break;

        case phyCCAMode:
            if (value->pib_value_8bit != CCA_MODE_1_ED)
            {
                status = MAC_INVALID_PARAMETER;
            }
            break;

        case macFCSType:
            tal_pib[trx_id].FCSType = value->pib_value_bool;
            pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_FCST, tal_pib[trx_id].FCSType);
            if (tal_pib[trx_id].FCSType == FCS_TYPE_4_OCTETS)
            {
                tal_pib[trx_id].FCSLen = 4;
            }
            else
            {
                tal_pib[trx_id].FCSLen = 2;
            }
            break;

        case macAckWaitDuration:
            tal_pib[trx_id].ACKWaitDuration = value->pib_value_16bit;
            break;

        case macACKTiming:
            tal_pib[trx_id].ACKTiming = value->pib_value_16bit;
            break;

        case macMaxFrameRetries:
            tal_pib[trx_id].MaxFrameRetries = value->pib_value_8bit;
            break;

#ifdef PROMISCUOUS_MODE
        case macPromiscuousMode:
            tal_pib[trx_id].PromiscuousMode = value->pib_value_bool;
            pal_trx_bit_write(bb_reg_offset + SR_BBC0_AFC0_PM,
                              tal_pib[trx_id].PromiscuousMode);
            if (tal_pib[trx_id].PromiscuousMode)
            {
                if (trx_state[trx_id] != RF_RX)
                {
                    switch_to_rx(trx_id);
                }
                //debug_text(PSTR("Prosmiscuous mode enabled"));
            }
            else
            {
                pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TRXOFF);
                trx_state[trx_id] = RF_TRXOFF;
                //debug_text(PSTR("Prosmiscuous mode disabled"));
            }
            break;
#endif

        case macPANId:
            tal_pib[trx_id].PANId = value->pib_value_16bit;
            pal_trx_write(bb_reg_offset + RG_BBC0_MACPID0F0,
                          (uint8_t *)&tal_pib[trx_id].PANId, 2);
            break;

        case macShortAddress:
            tal_pib[trx_id].ShortAddress = value->pib_value_16bit;
            pal_trx_write(bb_reg_offset + RG_BBC0_MACSHA0F0,
                          (uint8_t *)&tal_pib[trx_id].ShortAddress, 2);
            break;

        case macIeeeAddress:
            tal_pib[trx_id].IeeeAddress = value->pib_value_64bit;
            pal_trx_write(bb_reg_offset + RG_BBC0_MACEA0,
                          (uint8_t *)&tal_pib[trx_id].IeeeAddress, 8);
            break;

        case mac_i_pan_coordinator:
            tal_pib[trx_id].PrivatePanCoordinator = value->pib_value_bool;
            pal_trx_bit_write(bb_reg_offset + SR_BBC0_AFC1_PANC0,
                              (uint8_t)tal_pib[trx_id].PrivatePanCoordinator);
            break;

#ifdef MEASURE_ON_AIR_DURATION
        case phyOnAirDuration:
            tal_pib[trx_id].OnAirDuration = value->pib_value_32bit;
            break;
#endif

#ifdef ENABLE_ACK_RATE_MODE_ADAPTION
        case macAdaptDataRateForACK:
            if ((tal_pib[trx_id].phy.modulation == OFDM) ||
                (tal_pib[trx_id].phy.modulation == OQPSK))
            {
                tal_pib[trx_id].AdaptDataRateForACK = value->pib_value_bool;
            }
            else
            {
                status = MAC_INVALID_PARAMETER;
            }
            break;
#endif

#ifdef FRAME_FILTER_CONFIGURATION
        case macFrameFilterFrameTypes:
            tal_pib[trx_id].frame_types = value->pib_value_8bit;
            pal_trx_reg_write(bb_reg_offset + RG_BBC0_AFFTM, tal_pib[trx_id].frame_types);
            break;

        case macFrameFilterFrameVersions:
            tal_pib[trx_id].frame_versions = value->pib_value_8bit;
            pal_trx_bit_write(bb_reg_offset + SR_BBC0_AFFVM_AFFVM, tal_pib[trx_id].frame_versions);
            break;
#endif /* #ifdef FRAME_FILTER_CONFIGURATION */

        default:
            status = MAC_UNSUPPORTED_ATTRIBUTE;
            break;
    }

    return status;
} /* tal_pib_set() */

/**
 * \brief Apply channel page configuartion to transceiver
 *
 * \param ch_page Channel page
 *
 * \return true if changes could be applied else false
 */
extern void app_alert();

static bool apply_channel_page_configuration(trx_id_t trx ,uint8_t ch_page)
{

if (trx == RF24)
{
	if((ch_page != 0) && (ch_page != 9))
	
	{
		return false;
	}
}
else
{
	if((ch_page !=2) && (ch_page != 5)&& (ch_page != 9))
	{
return false;
	}
}
		phy_t phy;
		switch (ch_page) {
			case 0: /* compliant O-QPSK */
			/* Configure PHY for 2.4GHz */
			phy.modulation = LEG_OQPSK;
			phy.phy_mode.leg_oqpsk.chip_rate = CHIP_RATE_2000;
			phy.freq_band = WORLD_2450;
			phy.ch_spacing = LEG_2450_CH_SPAC;
			phy.freq_f0 = LEG_2450_F0;
			if (tal_pib_set(RF24, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
			{
				return false;
			}
			break;
			
			case 9: /* SunPhy Page */
			/* Configure PHY for 2.4GHz */
			if(trx==RF24)
			{
				
			
/*
			phy.modulation = OQPSK;
			phy.phy_mode.oqpsk.chip_rate = CHIP_RATE_2000;
			phy.phy_mode.oqpsk.rate_mode = OQPSK_RATE_MOD_4;
			phy.freq_band = WORLD_2450;
			phy.ch_spacing = OQPSK_2450_CH_SPAC;
			phy.freq_f0 = OQPSK_2450_F0;*/		
			
			phy.modulation = OFDM;
			phy.phy_mode.ofdm.interl = true;
			phy.phy_mode.ofdm.option = OFDM_OPT_1;
			phy.phy_mode.ofdm.mcs_val = MCS6;
			phy.freq_band = WORLD_2450;
			phy.ch_spacing = OFDM_2450_OPT1_CH_SPAC;
			phy.freq_f0 = OFDM_2450_OPT1_F0;
			if (tal_pib_set(RF24, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
			{
				return false;
			}
			}
			else
			{
				phy.modulation = OQPSK;
				phy.phy_mode.oqpsk.chip_rate = CHIP_RATE_1000;
				phy.phy_mode.oqpsk.rate_mode = OQPSK_RATE_MOD_3;
				phy.freq_band = US_915;
				phy.ch_spacing = OQPSK_915_CH_SPAC;
				phy.freq_f0 = OQPSK_915_F0;
				if (tal_pib_set(RF09, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
				{
					return false;
				}
			}
			break;			

			case 2: 

			/* Configure PHY for sub-1GHz */
			phy.modulation = LEG_OQPSK;
			phy.phy_mode.leg_oqpsk.chip_rate  = CHIP_RATE_1000;
			phy.freq_band = US_915;
			phy.ch_spacing = LEG_915_CH_SPAC; ;
			phy.freq_f0 = LEG_915_F0;
			if (tal_pib_set(RF09, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
			{
				return false;
			}
			break;
			
			case 5:
			/* Configure PHY for sub-1GHz */
			phy.modulation = LEG_OQPSK;
			phy.phy_mode.leg_oqpsk.chip_rate  = CHIP_RATE_1000;
			phy.freq_band = CHINA_780;
			phy.ch_spacing = LEG_915_CH_SPAC; ;
			phy.freq_f0 = LEG_780_F0;
			if (tal_pib_set(RF09, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
			{
				return false;
			}
			break;	

			default:
			return false;
		}

		return true;
}
/* EOF */
