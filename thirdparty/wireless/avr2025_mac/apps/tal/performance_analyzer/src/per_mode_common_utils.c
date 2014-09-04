/**
 * \file per_mode_common_utils.c
 *
 * \brief  Common utilities for both Initiator and Receptor in PER Measurement
 * mode - Performance Analyzer application
 *
 * Copyright (c) 2013-2014 Atmel Corporation. All rights reserved.
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
 * Copyright(c) 2013-2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */
#include <stdio.h>
#include "tal.h"
#include "tal_internal.h"
#include "tal_helper.h"
#include "ieee_const.h"
#include "app_per_mode.h"
#include "app_frame_format.h"
#include "app_init.h"
/* === TYPES =============================================================== */

/* === GLOBALS============================================================== */
#ifdef EXT_RF_FRONT_END_CTRL
static uint8_t prev_non_26chn_tx_power;
#endif
extern bool rdy_to_tx;
frame_info_t *stream_pkt;
uint8_t pkt_buffer[LARGE_BUFFER_SIZE];
/* === IMPLEMENTATION======================================================= */

/*
 * \brief This function is called rest the application equivalent to soft reset
 *
 */
void app_reset(void)
{
	/* app reset - this is to make the node to restart applicationa as
	 * bootup */
	set_main_state(INIT, NULL);

	/* INIT was a success - so change to WAIT_FOR_EVENT state */
	set_main_state(WAIT_FOR_EVENT, NULL);
}

#ifdef EXT_RF_FRONT_END_CTRL

/*
 * \brief handle the tx power settings in case of External PA enabled,
 * and the channel changes from or to 26.This is to meet the FCC compliance
 *
 * \param Current channel and Previous channel
 */
void limit_tx_power_in_ch26(uint8_t curr_chnl, uint8_t prev_chnl)
{
	pib_value_t pib_value;

	/* If the cuurent channel set to 26*/
	if (curr_chnl == CHANNEL_26) {
		/* Get last previous non 26 channel tx power  */
		if (prev_chnl != CHANNEL_26) {
			tal_pib_get(phyTransmitPower, &prev_non_26chn_tx_power);
		}

		/* If the Tx power is more than 13dBm, i.e. TX_PWR < 0x0d */
		if (trx_bit_read(SR_TX_PWR) <= MAX_TX_PWR_REG_VAL_CH26) {
			pib_value.pib_value_8bit = DEFAULT_TX_POWER_CH26;
			tal_pib_set(phyTransmitPower, &pib_value);
			curr_trx_config_params.tx_power_reg = trx_bit_read(
					SR_TX_PWR);
			curr_trx_config_params.tx_power_dbm
				= CONV_phyTransmitPower_TO_DBM(
					pib_value.pib_value_8bit);
		}
	} else {
		/* if the channel changed from 26 to other  */
		if (prev_chnl == CHANNEL_26) {
			/* Set back the tx power to default value i.e. 20dBm,
			 *TX_PWR 0x09 */
			pib_value.pib_value_8bit = prev_non_26chn_tx_power;
			tal_pib_set(phyTransmitPower, &pib_value);
			curr_trx_config_params.tx_power_reg = trx_bit_read(
					SR_TX_PWR);
			curr_trx_config_params.tx_power_dbm
				= CONV_phyTransmitPower_TO_DBM(
					pib_value.pib_value_8bit);
		}
	}
}

#endif /* End of EXT_RF_FRONT_END_CTRL */

/**
 * \brief To Configure the frame sending
 */
void configure_pkt_stream_frames(void)
{
	uint8_t index;
	uint8_t app_frame_length;
	uint8_t *frame_ptr;
	uint8_t *temp_frame_ptr;
	uint16_t fcf = 0;
	uint16_t temp_value;
	app_payload_t *tmp;

	stream_pkt = (frame_info_t *)pkt_buffer;	
	/*
	 * Fill in PHY frame.
	 */

	/* Get length of current frame. */
	app_frame_length = 127 - 	FRAME_OVERHEAD;

	/* Set payload pointer. */
	frame_ptr = temp_frame_ptr
				= (uint8_t *)stream_pkt +
					LARGE_BUFFER_SIZE -
					app_frame_length - FCS_LEN; /* Add 2
	                                                             * octets
	                                                             *for
	                                                             * FCS. */

	tmp = (app_payload_t *)temp_frame_ptr;

	(tmp->cmd_id) = PER_TEST_PKT;

	temp_frame_ptr++;

	/*
	 * Assign dummy payload values.
	 * Payload is stored to the end of the buffer avoiding payload copying
	 * by TAL.
	 */
	for (index = 0; index < (app_frame_length - 1); index++) { /* 1=> cmd ID
		                                                   **/
		*temp_frame_ptr++ = index; /* dummy values */
	}

	/* Source Address */
	temp_value =  tal_pib.ShortAddress;
	frame_ptr -= SHORT_ADDR_LEN;
	convert_16_bit_to_byte_array(temp_value, frame_ptr);

	/* Source PAN-Id */
#if (DST_PAN_ID == SRC_PAN_ID)
	/* No source PAN-Id included, but FCF updated. */
	fcf |= FCF_PAN_ID_COMPRESSION;
#else
	frame_ptr -= PAN_ID_LEN;
	temp_value = CCPU_ENDIAN_TO_LE16(SRC_PAN_ID);
	convert_16_bit_to_byte_array(temp_value, frame_ptr);
#endif

	/* Destination Address */
	temp_value = 0XFFFF;
	frame_ptr -= SHORT_ADDR_LEN;
	convert_16_bit_to_byte_array(temp_value, frame_ptr);

	/* Destination PAN-Id */
	temp_value = CCPU_ENDIAN_TO_LE16(DST_PAN_ID);
	frame_ptr -= PAN_ID_LEN;
	convert_16_bit_to_byte_array(temp_value, frame_ptr);

	/* Set DSN. */
	frame_ptr--;
	*frame_ptr = (uint8_t)rand();

	/* Set the FCF. */
	fcf |= FCF_FRAMETYPE_DATA | FCF_SET_SOURCE_ADDR_MODE(FCF_SHORT_ADDR) |
			FCF_SET_DEST_ADDR_MODE(FCF_SHORT_ADDR);


	frame_ptr -= FCF_LEN;
	convert_16_bit_to_byte_array(CCPU_ENDIAN_TO_LE16(fcf), frame_ptr);

	/* First element shall be length of PHY frame. */
	frame_ptr--;
	*frame_ptr = 127; //sriram

	/* Finished building of frame. */
	stream_pkt->mpdu = frame_ptr;
}

/**
 * \brief The reverse_float is used for reversing a float variable for
 * supporting BIG ENDIAN systems
 * \param float_val Float variable to be reversed
 */
float reverse_float( const float float_val )
{
	float retuVal;
	char *floatToConvert = (char *)&float_val;
	char *returnFloat = (char *)&retuVal;
#if UC3
	/* swap the bytes into a temporary buffer */
	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];
#else
	returnFloat[0] = floatToConvert[0];
	returnFloat[1] = floatToConvert[1];
	returnFloat[2] = floatToConvert[2];
	returnFloat[3] = floatToConvert[3];
#endif
	return retuVal;
}

void pkt_stream_gap_timer(void *parameter)
{
	rdy_to_tx = true;
	parameter=parameter;
}

/* EOF */
