/**
 * @file mac_gts.c
 *
 * @brief This file implements miscellaneous MAC sublayer components.
 *
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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
 *
 */

/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */
#include <compiler.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "return_val.h"
#include "pal.h"
#include "bmm.h"
#include "qmm.h"
#include "tal.h"
#include "ieee_const.h"
#include "mac_msg_const.h"
#include "mac_api.h"
#include "mac_msg_types.h"
#include "mac_data_structures.h"
#include "stack_config.h"
#include "mac_internal.h"
#include "mac.h"
#include "mac_config.h"
#include "mac_build_config.h"
#ifdef MAC_SECURITY_ZIP
#include "mac_security.h"
#endif  /* MAC_SECURITY_ZIP */

#ifdef GTS_SUPPORT

static bool mac_gts_allocate(gts_char_t GtsCharacteristics, uint16_t DevAddress);
static bool mac_gts_deallocate(gts_char_t GtsCharacteristics, uint16_t DevAddress);


#define GTS_REQ_PAYLOAD_LEN  (2)
#define MAX_GTS_ON_PANC      (7)

mac_pan_gts_mgmt_t mac_gts_mgmt_table[MAX_GTS_ON_PANC];

uint8_t mac_curr_gts_table_len = 0;

mac_gts_spec_t mac_gts_spec;

void mlme_gts_request(uint8_t *m)
{
	mlme_gts_req_t mgr;
	memcpy(&mgr, BMM_BUFFER_POINTER((buffer_t *)m),
			sizeof(mlme_gts_req_t));

	if(MAC_PAN_COORD_STARTED == mac_state)
	{
		if(GTS_ALLOCATE & mgr.GtsChar.GtsCharType)
		{
			if(mac_gts_allocate((gts_char_t)(mgr.GtsChar), tal_pib.ShortAddress))
			{
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_SUCCESS);
			}
			else
			{
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_NO_DATA);
			}
		}
		else
		{
			if(mac_gts_deallocate((gts_char_t)(mgr.GtsChar), tal_pib.ShortAddress))
			{
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_SUCCESS);
			}
			else
			{
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_NO_DATA);
			}
		}
	}
	else if(MAC_ASSOCIATED == mac_state)
	{
		if(MAC_NO_SHORT_ADDR_VALUE <= tal_pib.ShortAddress 
		|| MAC_NO_SHORT_ADDR_VALUE <= mac_pib.mac_CoordShortAddress)
		{
			return false;
		}
		frame_info_t *transmit_frame
			= (frame_info_t *)BMM_BUFFER_POINTER((buffer_t *)m);

		mac_trx_wakeup();

		/* Build the GTS Request command frame. */
		uint8_t tal_tx_status;
		uint8_t frame_len;
		uint8_t *frame_ptr;
		uint8_t *temp_frame_ptr;
		uint16_t fcf;
		uint16_t bc_addr = BROADCAST;

		/*
		 * Use the mlme association request buffer for transmitting an
		 * association request frame.
		 */
		frame_info_t *gts_req_frame
			= (frame_info_t *)(BMM_BUFFER_POINTER((buffer_t *)m));

		gts_req_frame->msg_type = GTSREQUEST;
		gts_req_frame->buffer_header = (buffer_t *)m;

		/* Get the payload pointer. */
		frame_ptr = temp_frame_ptr
					= (uint8_t *)gts_req_frame +
						LARGE_BUFFER_SIZE -
						GTS_REQ_PAYLOAD_LEN - 2; /* Add 2 octets for FCS. */

		/* Update the payload field. */
		*frame_ptr++ = GTSREQUEST;
		/* Build the capability info. */
		*frame_ptr = *((uint8_t*)&mgr.GtsChar);

		/* Get the payload pointer again to add the MHR. */
		frame_ptr = temp_frame_ptr;

		/* Update the length. */
		frame_len = GTS_REQ_PAYLOAD_LEN +
				2 + /* Add 2 octets for FCS */
				2 + /* 2 octets for Destination PAN-Id */
				2 + /* 2 octets for short Destination Address */
				2 + /* 2 octets for short Source Address */
				3; /* 3 octets DSN and FCF */

		/* Source address */
		frame_ptr -= 2;

		convert_16_bit_to_byte_array(tal_pib.ShortAddress, frame_ptr);

		frame_ptr -= 2;
		convert_16_bit_to_byte_array(mac_pib.mac_CoordShortAddress, frame_ptr);

		fcf = FCF_SET_FRAMETYPE(FCF_FRAMETYPE_MAC_CMD) |
				FCF_SET_DEST_ADDR_MODE(FCF_SHORT_ADDR) |
				FCF_SET_SOURCE_ADDR_MODE(FCF_SHORT_ADDR) |
				FCF_ACK_REQUEST | FCF_PAN_ID_COMPRESSION;

		/* Destination PAN-Id */
		frame_ptr -= 2;
		convert_16_bit_to_byte_array(tal_pib.PANId, frame_ptr);

		/* Set DSN. */
		frame_ptr--;
		*frame_ptr = mac_pib.mac_DSN++;

		/* Set the FCF. */
		frame_ptr -= 2;
		convert_spec_16_bit_to_byte_array(fcf, frame_ptr);

		/* First element shall be length of PHY frame. */
		frame_ptr--;
		*frame_ptr = frame_len;

		/* Finished building of frame. */
		gts_req_frame->mpdu = frame_ptr;


#ifdef BEACON_SUPPORT
		csma_mode_t cur_csma_mode;

		if (NON_BEACON_NWK == tal_pib.BeaconOrder) {
			/* In Nonbeacon network the frame is sent with unslotted
			 *CSMA-CA. */
			cur_csma_mode = CSMA_UNSLOTTED;
		} else {
			/* In Beacon network the frame is sent with slotted CSMA-CA. */
			cur_csma_mode = CSMA_SLOTTED;
		}

		tal_tx_status = tal_tx_frame(transmit_frame, cur_csma_mode, true);
#else   /* No BEACON_SUPPORT */
		/* In Nonbeacon build the frame is sent with unslotted CSMA-CA. */
		tal_tx_status = tal_tx_frame(transmit_frame, CSMA_UNSLOTTED, true);
#endif  /* BEACON_SUPPORT / No BEACON_SUPPORT */

		if (MAC_SUCCESS == tal_tx_status) {
			MAKE_MAC_BUSY();
		} else {
			return false;
		}

		return true;
	}
	
	/*
	 * Store the buffer which was received from the NHLE as it will be
	 *reused
	 * while sending MLME association confirmation to the NHLE.
	 */
	mac_conf_buf_ptr = m;
}


void mac_gen_mlme_gts_conf(buffer_t *buf_ptr, uint8_t status)
{
	mlme_gts_conf_t *gts_conf
	= (mlme_gts_conf_t *)BMM_BUFFER_POINTER(buf_ptr);

	gts_conf->cmdcode = MLME_GTS_CONFIRM;
	gts_conf->status = status;

	/* Append the associate confirm message to MAC-NHLE queue. */
	qmm_queue_append(&mac_nhle_q, buf_ptr);
}


void mac_process_gts_request(buffer_t *gts_req)
{
	/* Use the frame reception buffer for association indication. */
	mlme_gts_ind_t *mgi = (mlme_gts_ind_t *)BMM_BUFFER_POINTER(
			gts_req);

	///* Build the MLME association indication parameters. */
	//mgi->CapabilityInformation
		//= mac_parse_data.mac_payload_data.assoc_req_data.
			//capability_info;
	//mgi->cmdcode = MLME_GTS_INDICATION;

	/* Append the MLME associate indication to the MAC-NHLE queue. */
	qmm_queue_append(&mac_nhle_q, gts_req);
}



uint8_t mac_add_gts_info(uint8_t *frame_ptr)
{
	uint8_t table_index;
	uint8_t update_octets_count = 0;

	if(mac_curr_gts_table_len > 0)
	{
		uint8_t direction_mask = 0;
		for(table_index = 0; table_index < mac_curr_gts_table_len; table_index++)
		{
			frame_ptr--;
			*frame_ptr = (mac_gts_mgmt_table[table_index].GtsDesc.GtsLength << 4) 
							| mac_gts_mgmt_table[table_index].GtsDesc.GtsStartingSlot; 
			frame_ptr--;
			*frame_ptr = mac_gts_mgmt_table[table_index].DevShortAddr >> 8; //GTS List
			frame_ptr--;
			*frame_ptr = mac_gts_mgmt_table[table_index].DevShortAddr; //GTS List

			update_octets_count += 3;

			if(mac_gts_mgmt_table[table_index].GtsDesc.GtsDirection & GTS_RX_SLOT)
			{
				direction_mask |= (1 << table_index);
			}
		}
		frame_ptr--;
		*frame_ptr = direction_mask; //GTS Direction Mask
		++update_octets_count;
		
		mac_gts_spec.GtsDescCount = mac_curr_gts_table_len; 
		frame_ptr--;
		*frame_ptr = *((uint8_t*)&mac_gts_spec);
	}
	return update_octets_count;
}

bool mac_gts_allocate(gts_char_t GtsCharacteristics, uint16_t DevAddress)
{
	uint8_t Index = 0;
	
	for (Index = 0; Index < mac_curr_gts_table_len; Index++)
	{
		if(mac_gts_mgmt_table[Index].DevShortAddr == DevAddress 
		&& mac_gts_mgmt_table[Index].GtsDesc.GtsDirection == GtsCharacteristics.GtsDirection)
		{
			return false;
		}
	}
	
	mac_gts_mgmt_table[mac_curr_gts_table_len].DevShortAddr = DevAddress;
	
	mac_gts_mgmt_table[mac_curr_gts_table_len].GtsDesc.GtsLength = GtsCharacteristics.GtsLength;
	
	mac_gts_mgmt_table[mac_curr_gts_table_len].GtsDesc.GtsStartingSlot = (mac_final_cap_slot - GtsCharacteristics.GtsLength) + 1;
	
	mac_gts_mgmt_table[mac_curr_gts_table_len].GtsDesc.GtsDirection = GtsCharacteristics.GtsDirection;
	
	mac_final_cap_slot -= GtsCharacteristics.GtsLength;

	++mac_curr_gts_table_len;

	mac_gts_spec.GtsDescCount = mac_curr_gts_table_len;
	mac_gts_spec.GtsPermit = 1;
	
	return true;
}


bool mac_gts_deallocate(gts_char_t GtsCharacteristics, uint16_t DevAddress)
{
	uint8_t table_index, table_index1;
	uint8_t temp_slot = 0x10;
	
	for(table_index = 0; table_index < mac_curr_gts_table_len; table_index++)
	{
		if(mac_gts_mgmt_table[table_index].DevShortAddr == DevAddress
		&& mac_gts_mgmt_table[table_index].GtsDesc.GtsDirection == GtsCharacteristics.GtsDirection
		&& mac_gts_mgmt_table[table_index].GtsDesc.GtsLength == GtsCharacteristics.GtsLength)
		{
			for(table_index1 = table_index; table_index1 < (mac_curr_gts_table_len - 1); table_index1++)
			{
				mac_gts_mgmt_table[table_index1].DevShortAddr = mac_gts_mgmt_table[table_index1 + 1].DevShortAddr;
				
				mac_gts_mgmt_table[table_index1].GtsDesc.GtsDirection = mac_gts_mgmt_table[table_index1 + 1].GtsDesc.GtsDirection;
				
				mac_gts_mgmt_table[table_index1].GtsDesc.GtsLength = mac_gts_mgmt_table[table_index1 + 1].GtsDesc.GtsLength;
				
				mac_gts_mgmt_table[table_index1].GtsDesc.GtsStartingSlot = 
					temp_slot - mac_gts_mgmt_table[table_index1 + 1].GtsDesc.GtsLength;
					
				mac_gts_mgmt_table[table_index1].ExpiryCount = mac_gts_mgmt_table[table_index1 + 1].ExpiryCount;
				
				mac_gts_mgmt_table[table_index1].PersistenceCount = mac_gts_mgmt_table[table_index1 + 1].PersistenceCount;
				
				temp_slot -= mac_gts_mgmt_table[table_index1].GtsDesc.GtsLength;
			}
			
			--mac_curr_gts_table_len;

			mac_final_cap_slot = temp_slot - 1;
			
			for(; table_index1 < MAX_GTS_ON_PANC; table_index1++)
			{
				mac_gts_mgmt_table[table_index1].DevShortAddr = mac_gts_mgmt_table[table_index1 + 1].DevShortAddr;
				
				mac_gts_mgmt_table[table_index1].GtsDesc.GtsDirection = mac_gts_mgmt_table[table_index1 + 1].GtsDesc.GtsDirection;
				
				mac_gts_mgmt_table[table_index1].GtsDesc.GtsLength = mac_gts_mgmt_table[table_index1 + 1].GtsDesc.GtsLength;
				
				mac_gts_mgmt_table[table_index1].GtsDesc.GtsStartingSlot = mac_gts_mgmt_table[table_index1 + 1].GtsDesc.GtsLength;
				
				mac_gts_mgmt_table[table_index1].ExpiryCount = mac_gts_mgmt_table[table_index1 + 1].ExpiryCount;
				
				mac_gts_mgmt_table[table_index1].PersistenceCount = mac_gts_mgmt_table[table_index1 + 1].PersistenceCount;
			}
			
			
			mac_gts_mgmt_table[MAX_GTS_ON_PANC - 1].DevShortAddr = DevAddress;
			mac_gts_mgmt_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsDirection = GtsCharacteristics.GtsDirection;
			mac_gts_mgmt_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsLength = GtsCharacteristics.GtsLength;
			mac_gts_mgmt_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsStartingSlot = 0;
			mac_gts_mgmt_table[MAX_GTS_ON_PANC - 1].PersistenceCount = aGTSDescPersistenceTime;
			
			return true;
		}
		else
		{
			temp_slot -= mac_gts_mgmt_table[table_index].GtsDesc.GtsLength;
		}
	}
	return false;
}












































































#endif /* GTS_SUPPORT */

