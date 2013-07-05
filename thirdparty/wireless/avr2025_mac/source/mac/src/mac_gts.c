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

#ifdef FFD
static bool mac_gts_allocate(gts_char_t GtsCharacteristics, uint16_t DevAddress);
static bool mac_gts_deallocate(gts_char_t GtsCharacteristics, uint16_t DevAddress);

#define GTS_REQ_PAYLOAD_LEN  (2)
#define MAX_GTS_ON_PANC      (7)

mac_pan_gts_mgmt_t mac_pan_gts_table[MAX_GTS_ON_PANC];
uint8_t mac_pan_gts_table_len = 0;
mac_gts_spec_t mac_gts_spec;
#endif /* FFD */

/**
 * Holds the mlme gts request buffer pointer, used to give the respective
 * confirmation.
 */
uint8_t *mac_gts_buf_ptr;

#define DEV_RX_SLOT_INDEX    (0)
#define DEV_TX_SLOT_INDEX    (1)
#define PAN_RX_SLOT_INDEX    (2)
#define PAN_TX_SLOT_INDEX    (3)

#define MAX_GTS_ON_DEV       (4)

mac_dev_gts_mgmt_t mac_dev_gts_table[MAX_GTS_ON_DEV];
uint8_t mac_dev_gts_table_len = 0;
gts_char_t requested_gts_char;

void mlme_gts_request(uint8_t *m)
{
	mlme_gts_req_t mgr;
	memcpy(&mgr, BMM_BUFFER_POINTER((buffer_t *)m),
			sizeof(mlme_gts_req_t));

	/*
	 * Store the buffer which was received from the NHLE as it will be
	 *reused
	 * while sending MLME association confirmation to the NHLE.
	 */
	mac_gts_buf_ptr = m;
	requested_gts_char = mgr.GtsChar;

	if(MAC_NO_SHORT_ADDR_VALUE <= tal_pib.ShortAddress 
	|| (MAC_NO_SHORT_ADDR_VALUE <= mac_pib.mac_CoordShortAddress 
	&& MAC_PAN_COORD_STARTED != mac_state))
	{
		mac_gen_mlme_gts_conf((buffer_t *)m, MAC_NO_SHORT_ADDRESS,
		mgr.GtsChar);
		return;
	}
	else if(true != mac_pib.mac_GTSPermit)
	{
		mac_gen_mlme_gts_conf((buffer_t *)m, MAC_INVALID_PARAMETER,
		mgr.GtsChar);
		return;
	}
#ifdef FFD
	else if(MAC_PAN_COORD_STARTED == mac_state)
	{
		if(GTS_ALLOCATE & mgr.GtsChar.GtsCharType)
		{
			if(mac_gts_allocate(mgr.GtsChar, tal_pib.ShortAddress))
			{
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_SUCCESS,
				mgr.GtsChar);
				return;
			}
			else
			{
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_NO_DATA,
				mgr.GtsChar);
				return;
			}
		}
		else
		{
			if(mac_gts_deallocate(mgr.GtsChar, tal_pib.ShortAddress))
			{
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_SUCCESS,
				mgr.GtsChar);
				return;
			}
			else
			{
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_NO_DATA,
				mgr.GtsChar);
				return;
			}
		}
	}
#endif /* FFD */
	else if(MAC_ASSOCIATED == mac_state)
	{
		if(0x00 == mgr.GtsChar.GtsLength
		|| (MAC_NO_SHORT_ADDR_VALUE <= tal_pib.ShortAddress 
			|| MAC_NO_SHORT_ADDR_VALUE <= mac_pib.mac_CoordShortAddress)
		|| MAC_SYNC_TRACKING_BEACON != mac_sync_state)
		{
			mac_gen_mlme_gts_conf((buffer_t *)m, MAC_INVALID_PARAMETER,
			mgr.GtsChar);
			return;
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
			if(GTS_DEALLOCATE == mgr.GtsChar.GtsCharType)
			{
				uint8_t updating_index;
				if(GTS_RX_SLOT == mgr.GtsChar.GtsDirection)
				{
					updating_index = DEV_RX_SLOT_INDEX;
				}
				else
				{
					updating_index = DEV_TX_SLOT_INDEX;
				}

				mac_dev_gts_table[updating_index].GtsLength	= 0;
				mac_dev_gts_table[updating_index].GtsStartingSlot = 0;
				mac_gen_mlme_gts_conf((buffer_t *)m, MAC_SUCCESS, mgr.GtsChar);
				return;
			}
			else
			{
				mac_gts_state = MAC_GTS_ALLOC_REQ_SENT;
				MAKE_MAC_BUSY();
			}
		} else {
			mac_gen_mlme_gts_conf((buffer_t *)m, tal_tx_status,
			mgr.GtsChar);
		}
	}
	else
	{
		mac_gen_mlme_gts_conf((buffer_t *)m, MAC_INVALID_PARAMETER,
		mgr.GtsChar);
		return;
	}
}

void mac_gen_mlme_gts_conf(buffer_t *buf_ptr, uint8_t status, gts_char_t gts_char)
{
	mlme_gts_conf_t *gts_conf
	= (mlme_gts_conf_t *)BMM_BUFFER_POINTER(buf_ptr);

	gts_conf->cmdcode = MLME_GTS_CONFIRM;
	gts_conf->status = status;
	gts_conf->GtsChar = gts_char;

	/* Append the associate confirm message to MAC-NHLE queue. */
	qmm_queue_append(&mac_nhle_q, buf_ptr);
}

#ifdef FFD
void mac_process_gts_request(buffer_t *gts_req)
{
	/* Use the frame reception buffer for association indication. */
	mlme_gts_ind_t *mgi = (mlme_gts_ind_t *)BMM_BUFFER_POINTER(
			gts_req);

	mgi->DeviceAddr = mac_parse_data.src_addr.short_address;
	
	mgi->GtsChar = mac_parse_data.mac_payload_data.gts_req_data;
	mgi->cmdcode = MLME_GTS_INDICATION;

	if(GTS_ALLOCATE == (mgi->GtsChar).GtsCharType)
	{
		if(mac_gts_allocate(mgi->GtsChar, mgi->DeviceAddr))
		{
			/* Append the MLME GTS indication to the MAC-NHLE queue. */
			qmm_queue_append(&mac_nhle_q, gts_req);
		}
		else
		{
			bmm_buffer_free(gts_req);
		}
	}
	else
	{
		if(mac_gts_deallocate(mgi->GtsChar, mgi->DeviceAddr))
		{
			/* Append the MLME GTS indication to the MAC-NHLE queue. */
			qmm_queue_append(&mac_nhle_q, gts_req);
		}
		else
		{
			bmm_buffer_free(gts_req);
		}
	}
}
#endif /* FFD */

#ifdef FFD
uint8_t mac_add_gts_info(uint8_t *frame_ptr)
{
	uint8_t table_index;
	uint8_t update_octets_count = 0;
	uint8_t direction_mask = 0;

	mac_gts_spec.GtsDescCount = 0;

	for(table_index = 0; table_index < MAX_GTS_ON_PANC; table_index++)
	{
		if(mac_pan_gts_table[table_index].PersistenceCount > 0)
		{
			frame_ptr--;
			*frame_ptr = (mac_pan_gts_table[table_index].GtsDesc.GtsLength << 4)
			| mac_pan_gts_table[table_index].GtsDesc.GtsStartingSlot;
			frame_ptr--;
			*frame_ptr = mac_pan_gts_table[table_index].DevShortAddr >> 8; //GTS List
			frame_ptr--;
			*frame_ptr = mac_pan_gts_table[table_index].DevShortAddr; //GTS List

			update_octets_count += 3;

			if(mac_pan_gts_table[table_index].GtsDesc.GtsDirection & GTS_RX_SLOT)
			{
				direction_mask = (direction_mask << 1) | 1;
			}
			else
			{
				direction_mask = direction_mask << 1;
			}
			mac_pan_gts_table[table_index].PersistenceCount--;
			mac_gts_spec.GtsDescCount++;
		}
	}
	if(mac_gts_spec.GtsDescCount > 0)
	{
		frame_ptr--;
		*frame_ptr = direction_mask; //GTS Direction Mask
		++update_octets_count;
		frame_ptr--;
	}

//	mac_gts_spec.GtsDescCount = mac_pan_gts_table_len;
	mac_gts_spec.GtsPermit = mac_pib.mac_GTSPermit;
	*frame_ptr = *((uint8_t*)&mac_gts_spec);
	return update_octets_count;
}
#endif /* FFD */

#ifdef FFD
bool mac_gts_allocate(gts_char_t GtsCharacteristics, uint16_t DevAddress)
{
	uint8_t Index = 0;
	
	if(MAX_GTS_ON_PANC < mac_pan_gts_table_len)
	{
		return false;
	}
	for (Index = 0; Index < mac_pan_gts_table_len; Index++)
	{
		if(mac_pan_gts_table[Index].DevShortAddr == DevAddress 
		&& mac_pan_gts_table[Index].GtsDesc.GtsDirection == GtsCharacteristics.GtsDirection)
		{
			return false;
		}
	}
	
	mac_pan_gts_table[mac_pan_gts_table_len].DevShortAddr = DevAddress;
	
	mac_pan_gts_table[mac_pan_gts_table_len].GtsDesc.GtsLength = GtsCharacteristics.GtsLength;
	
	mac_pan_gts_table[mac_pan_gts_table_len].GtsDesc.GtsStartingSlot = (mac_final_cap_slot - GtsCharacteristics.GtsLength) + 1;
	
	mac_pan_gts_table[mac_pan_gts_table_len].GtsDesc.GtsDirection = GtsCharacteristics.GtsDirection;
	
	mac_pan_gts_table[mac_pan_gts_table_len].PersistenceCount = aGTSDescPersistenceTime;
	
	mac_final_cap_slot -= GtsCharacteristics.GtsLength;

	++mac_pan_gts_table_len;

	return true;
}
#endif /* FFD */

#ifdef FFD
bool mac_gts_deallocate(gts_char_t GtsCharacteristics, uint16_t DevAddress)
{
	uint8_t table_index, table_index1;
	uint8_t temp_slot = 0x10;
	
	for(table_index = 0; table_index < mac_pan_gts_table_len; table_index++)
	{
		if(mac_pan_gts_table[table_index].DevShortAddr == DevAddress
		&& mac_pan_gts_table[table_index].GtsDesc.GtsDirection == GtsCharacteristics.GtsDirection
		&& mac_pan_gts_table[table_index].GtsDesc.GtsLength == GtsCharacteristics.GtsLength)
		{
			for(table_index1 = table_index; table_index1 < (mac_pan_gts_table_len - 1); table_index1++)
			{
				mac_pan_gts_table[table_index1].DevShortAddr = mac_pan_gts_table[table_index1 + 1].DevShortAddr;
				
				mac_pan_gts_table[table_index1].GtsDesc.GtsDirection = mac_pan_gts_table[table_index1 + 1].GtsDesc.GtsDirection;
				
				mac_pan_gts_table[table_index1].GtsDesc.GtsLength = mac_pan_gts_table[table_index1 + 1].GtsDesc.GtsLength;
				
				mac_pan_gts_table[table_index1].GtsDesc.GtsStartingSlot = 
					temp_slot - mac_pan_gts_table[table_index1 + 1].GtsDesc.GtsLength;
					
				mac_pan_gts_table[table_index1].ExpiryCount = mac_pan_gts_table[table_index1 + 1].ExpiryCount;
				
				mac_pan_gts_table[table_index1].PersistenceCount = mac_pan_gts_table[table_index1 + 1].PersistenceCount;
				
				temp_slot -= mac_pan_gts_table[table_index1].GtsDesc.GtsLength;
			}
			
			--mac_pan_gts_table_len;

			mac_final_cap_slot = temp_slot - 1;
			
			for(; table_index1 < MAX_GTS_ON_PANC; table_index1++)
			{
				mac_pan_gts_table[table_index1].DevShortAddr = mac_pan_gts_table[table_index1 + 1].DevShortAddr;
				
				mac_pan_gts_table[table_index1].GtsDesc.GtsDirection = mac_pan_gts_table[table_index1 + 1].GtsDesc.GtsDirection;
				
				mac_pan_gts_table[table_index1].GtsDesc.GtsLength = mac_pan_gts_table[table_index1 + 1].GtsDesc.GtsLength;
				
				mac_pan_gts_table[table_index1].GtsDesc.GtsStartingSlot = mac_pan_gts_table[table_index1 + 1].GtsDesc.GtsLength;
				
				mac_pan_gts_table[table_index1].ExpiryCount = mac_pan_gts_table[table_index1 + 1].ExpiryCount;
				
				mac_pan_gts_table[table_index1].PersistenceCount = mac_pan_gts_table[table_index1 + 1].PersistenceCount;
			}
			
			
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].DevShortAddr = DevAddress;
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsDirection = GtsCharacteristics.GtsDirection;
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsLength = GtsCharacteristics.GtsLength;
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsStartingSlot = 0;
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].PersistenceCount = aGTSDescPersistenceTime;
			
			return true;
		}
		else
		{
			temp_slot -= mac_pan_gts_table[table_index].GtsDesc.GtsLength;
		}
	}
	return false;
}
#endif /* FFD */

void mac_parse_bcn_gts_info(uint8_t gts_count, uint8_t gts_dir, uint8_t *gts_list_ptr)
{
	uint8_t loop_index;
	mac_gts_list_t *gts_list = (mac_gts_list_t *) gts_list_ptr;
	uint8_t updating_index;
	uint8_t curr_gts_dir;

	for(loop_index = 0; loop_index < gts_count; loop_index++)
	{
		curr_gts_dir = ((gts_dir >> loop_index) && 0x01);

		if(MAC_GTS_ALLOC_REQ_SENT == mac_gts_state
		&& gts_list->dev_addr == tal_pib.ShortAddress)
		{
			if(0 == gts_list->starting_slot)
			{
				mac_gen_mlme_gts_conf((buffer_t *)mac_gts_buf_ptr, MAC_DENIED,
				requested_gts_char);
			}
			else if(gts_list->length == requested_gts_char.GtsLength)
			{
				if(GTS_RX_SLOT == curr_gts_dir)
				{
					updating_index = DEV_RX_SLOT_INDEX;
				}
				else
				{
					updating_index = DEV_TX_SLOT_INDEX;
				}
				mac_dev_gts_table[updating_index].GtsLength
				= gts_list->length;
				mac_dev_gts_table[updating_index].GtsStartingSlot
				= gts_list->starting_slot;

				mac_gen_mlme_gts_conf((buffer_t *)mac_gts_buf_ptr, MAC_SUCCESS,
				requested_gts_char);
			}
			mac_gts_state = MAC_GTS_IDLE;
		}
		else if(gts_list->dev_addr == mac_pib.mac_CoordShortAddress)
		{
			if(GTS_RX_SLOT == curr_gts_dir)
			{
				updating_index = PAN_RX_SLOT_INDEX;
			}
			else
			{
				updating_index = PAN_TX_SLOT_INDEX;
			}
			mac_dev_gts_table[updating_index].GtsLength
			= gts_list->length;
			mac_dev_gts_table[updating_index].GtsStartingSlot
			= gts_list->starting_slot;
		}
		gts_list++;
	}
}

#endif /* GTS_SUPPORT */
