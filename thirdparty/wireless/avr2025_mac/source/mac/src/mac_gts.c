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
#include "sio2host.h"//vk

#ifdef GTS_SUPPORT

#ifdef FFD
static bool mac_gts_allocate(gts_char_t GtsCharacteristics, uint16_t DevAddress);
static bool mac_gts_deallocate(gts_char_t GtsCharacteristics, uint16_t DevAddress, bool persist);
#endif /* FFD */
static void mac_update_dev_gts_table(bool gts_dir, uint8_t slot_len, uint8_t start_slot, bool panc_slot, bool send_ind);

#ifdef FFD
mac_pan_gts_mgmt_t mac_pan_gts_table[MAX_GTS_ON_PANC];
uint8_t mac_pan_gts_table_len = 0;
mac_gts_spec_t mac_gts_spec;
#endif /* FFD */

/**
 * Holds the mlme gts request buffer pointer, used to give the respective
 * confirmation.
 */
uint8_t *mac_gts_buf_ptr;

mac_dev_gts_mgmt_t mac_dev_gts_table[MAX_GTS_ON_DEV];
uint8_t mac_dev_gts_table_len = 0;
gts_char_t requested_gts_char;

#ifdef FFD
static queue_t gts_q[MAX_GTS_ON_PANC];
#endif /* FFD */
static queue_t dev_tx_gts_q;

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
	else if(true != mac_pib.mac_GTSPermit
	|| (MAC_ASSOCIATED == mac_state && mgr.DeviceShortAddr != tal_pib.ShortAddress)
	|| (0 == mgr.GtsChar.GtsLength))
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
			if(mac_gts_allocate(mgr.GtsChar, mgr.DeviceShortAddr))
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
			if(mac_gts_deallocate(mgr.GtsChar, mgr.DeviceShortAddr, true))
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
		if((MAC_NO_SHORT_ADDR_VALUE <= tal_pib.ShortAddress 
			|| MAC_NO_SHORT_ADDR_VALUE <= mac_pib.mac_CoordShortAddress)
		|| MAC_SYNC_TRACKING_BEACON != mac_sync_state
		|| true != mac_pib.mac_GTSPermit)
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
		if(mac_gts_deallocate(mgi->GtsChar, mgi->DeviceAddr, false))
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

			if(GTS_RX_SLOT & mac_pan_gts_table[table_index].GtsDesc.GtsDirection)
			{
				direction_mask |= 1 << mac_gts_spec.GtsDescCount;
			}
			if(mac_pan_gts_table[table_index].PersistenceCount > 0
			&& --mac_pan_gts_table[table_index].PersistenceCount == 0)
			{
				if (tal_pib.BeaconOrder >= 9)
				{
					mac_pan_gts_table[table_index].ExpiryCount = GTS_EXPIRY_BO_9_TO_14;
				}
				else
				{
					mac_pan_gts_table[table_index].ExpiryCount = GTS_EXPIRY_BO_0_TO_8;
				}
			}
			mac_gts_spec.GtsDescCount++;
		}
		if(mac_pan_gts_table[table_index].ExpiryCount > 0
		&& --mac_pan_gts_table[table_index].ExpiryCount == 0)
		{
			gts_char_t gts_char;
			uint16_t dev_addr = mac_pan_gts_table[table_index].DevShortAddr;
			gts_char.GtsDirection = mac_pan_gts_table[table_index].GtsDesc.GtsDirection;
			gts_char.GtsLength = mac_pan_gts_table[table_index].GtsDesc.GtsLength;
			gts_char.GtsCharType = GTS_DEALLOCATE;
			gts_char.Reserved = 0;
			if(mac_gts_deallocate(gts_char, mac_pan_gts_table[table_index].DevShortAddr, true))
			{
				buffer_t *buffer_header;
				mlme_gts_ind_t *mgi;
					
				buffer_header = bmm_buffer_alloc(LARGE_BUFFER_SIZE);

				if (NULL == buffer_header) {
					/* Buffer is not available */
					return false;
				}
				mgi = (mlme_gts_ind_t *)BMM_BUFFER_POINTER(buffer_header);

				mgi->DeviceAddr = dev_addr;
					
				mgi->GtsChar = gts_char;
				mgi->cmdcode = MLME_GTS_INDICATION;

				/* Append the MLME GTS indication to the MAC-NHLE queue. */
				qmm_queue_append(&mac_nhle_q, buffer_header);
			}
		}
	}
	if(mac_gts_spec.GtsDescCount > 0)
	{
		frame_ptr--;
		*frame_ptr = direction_mask; //GTS Direction Mask
		++update_octets_count;
	}

	frame_ptr--;
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
	
	ENTER_CRITICAL_REGION();

	mac_pan_gts_table[mac_pan_gts_table_len].DevShortAddr = DevAddress;
	
	mac_pan_gts_table[mac_pan_gts_table_len].GtsDesc.GtsLength = GtsCharacteristics.GtsLength;
	
	mac_pan_gts_table[mac_pan_gts_table_len].GtsDesc.GtsStartingSlot = (mac_final_cap_slot - GtsCharacteristics.GtsLength) + 1;
	
	mac_pan_gts_table[mac_pan_gts_table_len].GtsDesc.GtsDirection = GtsCharacteristics.GtsDirection;
	
	mac_pan_gts_table[mac_pan_gts_table_len].PersistenceCount = aGTSDescPersistenceTime;
	
	mac_final_cap_slot -= GtsCharacteristics.GtsLength;

	++mac_pan_gts_table_len;

	LEAVE_CRITICAL_REGION();
	return true;
}
#endif /* FFD */

#ifdef FFD
bool mac_gts_deallocate(gts_char_t GtsCharacteristics, uint16_t DevAddress, bool persist)
{
	uint8_t table_index, table_index1;
	uint8_t temp_slot = 0x10;
	
	for(table_index = 0; table_index < mac_pan_gts_table_len; table_index++)
	{
		/* Check to identify the GTS to be deallocated from the table...*/
		if(mac_pan_gts_table[table_index].DevShortAddr == DevAddress
		&& mac_pan_gts_table[table_index].GtsDesc.GtsDirection == GtsCharacteristics.GtsDirection
		&& mac_pan_gts_table[table_index].GtsDesc.GtsLength == GtsCharacteristics.GtsLength)
		{
			ENTER_CRITICAL_REGION();
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
				
				mac_pan_gts_table[table_index1].GtsDesc.GtsStartingSlot = mac_pan_gts_table[table_index1 + 1].GtsDesc.GtsStartingSlot;
				
				mac_pan_gts_table[table_index1].ExpiryCount = mac_pan_gts_table[table_index1 + 1].ExpiryCount;
				
				mac_pan_gts_table[table_index1].PersistenceCount = mac_pan_gts_table[table_index1 + 1].PersistenceCount;
			}
			
			
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].DevShortAddr = DevAddress;
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsDirection = GtsCharacteristics.GtsDirection;
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsLength = GtsCharacteristics.GtsLength;
			mac_pan_gts_table[MAX_GTS_ON_PANC - 1].GtsDesc.GtsStartingSlot = 0;
			if(persist)
			{
				mac_pan_gts_table[MAX_GTS_ON_PANC - 1].PersistenceCount = aGTSDescPersistenceTime;
			}
			else
			{
				mac_pan_gts_table[MAX_GTS_ON_PANC - 1].PersistenceCount = 0;
			}
			LEAVE_CRITICAL_REGION();
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
				mac_update_dev_gts_table(curr_gts_dir,gts_list->length, gts_list->starting_slot, false, false);

				mac_gen_mlme_gts_conf((buffer_t *)mac_gts_buf_ptr, MAC_SUCCESS,
				requested_gts_char);
			}
			mac_gts_state = MAC_GTS_IDLE;
		}
		else if(gts_list->dev_addr == mac_pib.mac_CoordShortAddress)
		{
			mac_update_dev_gts_table(curr_gts_dir,gts_list->length, gts_list->starting_slot, true, true);
		}
		if (0 == gts_list->starting_slot
		&& tal_pib.ShortAddress == gts_list->dev_addr)
		{
			mac_update_dev_gts_table(curr_gts_dir, gts_list->length, gts_list->starting_slot, false, true);
		}
		gts_list++;
	}
}

void mac_update_dev_gts_table(bool gts_dir, uint8_t slot_len, uint8_t start_slot, bool panc_slot, bool send_ind)
{
	uint8_t updating_index = (panc_slot << 1) | gts_dir;
	if((mac_dev_gts_table[updating_index].GtsLength != slot_len
			|| mac_dev_gts_table[updating_index].GtsStartingSlot != start_slot)
		&& true == send_ind)
	{
		gts_char_t gts_char;
		buffer_t *buffer_header;
		mlme_gts_ind_t *mgi;

		buffer_header = bmm_buffer_alloc(LARGE_BUFFER_SIZE);

		if (NULL == buffer_header) {
			/* Buffer is not available */
			Assert("Buffer not allocated..." == 0);
		}

		mgi = (mlme_gts_ind_t *)BMM_BUFFER_POINTER(buffer_header);

		if(0 == start_slot)
		{
			gts_char.GtsCharType = GTS_DEALLOCATE;
		}
		else
		{
			gts_char.GtsCharType = GTS_ALLOCATE;
		}
		gts_char.GtsDirection = gts_dir;
		gts_char.GtsLength = slot_len;
		gts_char.Reserved = 0;
		if(panc_slot)
		{
			mgi->DeviceAddr = mac_pib.mac_CoordShortAddress;
		}
		else
		{
			mgi->DeviceAddr = tal_pib.ShortAddress;
		}
		mgi->GtsChar = gts_char;
		mgi->cmdcode = MLME_GTS_INDICATION;

		/* Append the MLME GTS indication to the MAC-NHLE queue. */
		qmm_queue_append(&mac_nhle_q, buffer_header);
	}
	mac_dev_gts_table[updating_index].GtsLength
	= slot_len;
	mac_dev_gts_table[updating_index].GtsStartingSlot
	= start_slot;
}

uint8_t handle_gts_data_req(mcps_data_req_t *data_req, uint8_t *msg)
{
	uint16_t dst_addr;
	bool data_queued = false;
	ADDR_COPY_DST_SRC_16(dst_addr, data_req->DstAddr);
#ifdef FFD
	if(MAC_PAN_COORD_STARTED == mac_state)
	{
		uint8_t loop_index;
		for(loop_index = 0; loop_index < mac_pan_gts_table_len; loop_index++)
		{
			if(dst_addr == mac_pan_gts_table[loop_index].DevShortAddr
			&& GTS_RX_SLOT == mac_pan_gts_table[loop_index].GtsDesc.GtsDirection)
			{
				/* Append the MCPS data request into the GTS queue */
				#ifdef ENABLE_QUEUE_CAPACITY
				if (QUEUE_FULL == qmm_queue_append(mac_pan_gts_table[loop_index].gts_data_q,
													(buffer_t *)msg)) {
						mac_gen_mcps_data_conf((buffer_t *)msg,
						(uint8_t)MAC_CHANNEL_ACCESS_FAILURE,
						#ifdef ENABLE_TSTAMP
						data_req->msduHandle,
						0);
						#else
						data_req->msduHandle);
						#endif  /* ENABLE_TSTAMP */
				}
				#else
				qmm_queue_append(mac_pan_gts_table[loop_index].gts_data_q, (buffer_t *)msg);
				#endif   /* ENABLE_QUEUE_CAPACITY */
				data_queued = true;
				break;
			}
		}
	}
	else
#endif /* FFD */
	if(MAC_ASSOCIATED == mac_state && mac_pib.mac_CoordShortAddress == data_req->DstAddr
	&& 0 != mac_dev_gts_table[DEV_TX_SLOT_INDEX].GtsStartingSlot)
	{
		/* Append the MCPS data request into the GTS queue */
		#ifdef ENABLE_QUEUE_CAPACITY
		if (QUEUE_FULL == qmm_queue_append(&dev_tx_gts_q, (buffer_t *)msg)) {
			mac_gen_mcps_data_conf((buffer_t *)msg,
			(uint8_t)MAC_CHANNEL_ACCESS_FAILURE,
			#ifdef ENABLE_TSTAMP
			data_req->msduHandle,
			0);
			#else
			data_req->msduHandle);
			#endif  /* ENABLE_TSTAMP */
		}
		#else
		qmm_queue_append(&dev_tx_gts_q, (buffer_t *)msg);
		#endif   /* ENABLE_QUEUE_CAPACITY */
		data_queued = true;
	}
	if(!data_queued)
	{
		mac_gen_mcps_data_conf((buffer_t *)msg,
			(uint8_t)MAC_INVALID_GTS,
			#ifdef ENABLE_TSTAMP
			data_req->msduHandle, 0);
			#else
			data_req->msduHandle);
			#endif  /* ENABLE_TSTAMP */
	}
	return 0;
}

void reset_gts_globals(void)
{
#ifdef FFD
	mac_pan_gts_table_len = 0;
	memset(&mac_pan_gts_table, 0, sizeof(mac_pan_gts_mgmt_t) * MAX_GTS_ON_PANC);
#endif /* FFD */
	mac_dev_gts_table_len = 0;
	memset(&mac_dev_gts_table, 0, sizeof(mac_pan_gts_mgmt_t) * MAX_GTS_ON_DEV);
	init_gts_queues();
}

void mac_t_gts_cb(void *callback_parameter)
{
	uint32_t next_timer_dur = 0;
	queue_t *temp_ptr = NULL;
	uint8_t temp_index = 0;
	uint32_t slot_duration;
	ENTER_CRITICAL_REGION();
	slot_duration = (TAL_CONVERT_SYMBOLS_TO_US(TAL_GET_SUPERFRAME_DURATION_TIME(tal_pib.SuperFrameOrder)) >> 4);
#ifdef FFD
	if (MAC_PAN_COORD_STARTED == mac_state && mac_pan_gts_table_len > 0)
	{
		ioport_set_value(DEBUG_PIN3, 0); //vk
		if(MAC_ACTIVE_CAP == mac_superframe_state)
		{
			temp_index = mac_pan_gts_table_len - 1;
			next_timer_dur = slot_duration * (mac_pan_gts_table[temp_index].GtsDesc.GtsLength);
			mac_superframe_state = MAC_ACTIVE_CFP_GTS1;
			temp_ptr = mac_pan_gts_table[temp_index].gts_data_q;
			sio2host_tx("-GTS1-",sizeof("-GTS1-"));
			ioport_set_value(DEBUG_PIN4, 1);//vk
		}
		else if(MAC_ACTIVE_CFP_GTS1 == mac_superframe_state && (mac_pan_gts_table_len >= 2))
		{
			temp_index = mac_pan_gts_table_len - 2;
			next_timer_dur = slot_duration * (mac_pan_gts_table[mac_pan_gts_table_len - 2].GtsDesc.GtsLength);
			mac_superframe_state = MAC_ACTIVE_CFP_GTS2;
			temp_ptr = mac_pan_gts_table[temp_index].gts_data_q;
			sio2host_tx("-GTS2-",sizeof("-GTS1-"));
			ioport_set_value(DEBUG_PIN4, 0);//vk
			ioport_set_value(DEBUG_PIN5, 1);//vk
		}
		else if(MAC_ACTIVE_CFP_GTS2 == mac_superframe_state && (mac_pan_gts_table_len >= 3))
		{
			temp_index = mac_pan_gts_table_len - 3;
			next_timer_dur = slot_duration * (mac_pan_gts_table[mac_pan_gts_table_len - 3].GtsDesc.GtsLength);
			mac_superframe_state = MAC_ACTIVE_CFP_GTS3;
			temp_ptr = mac_pan_gts_table[temp_index].gts_data_q;
			sio2host_tx("-GTS3-",sizeof("-GTS1-"));
			ioport_set_value(DEBUG_PIN5, 0);//vk
			ioport_set_value(DEBUG_PIN6, 1);//vk
		}
		else if(MAC_ACTIVE_CFP_GTS3 == mac_superframe_state && (mac_pan_gts_table_len >= 4))
		{
			temp_index = mac_pan_gts_table_len - 4;
			next_timer_dur = slot_duration * (mac_pan_gts_table[mac_pan_gts_table_len - 4].GtsDesc.GtsLength);
			mac_superframe_state = MAC_ACTIVE_CFP_GTS4;
			temp_ptr = mac_pan_gts_table[temp_index].gts_data_q;
			sio2host_tx("-GTS4-",sizeof("-GTS1-"));
			ioport_set_value(DEBUG_PIN6, 0);//vk
			ioport_set_value(DEBUG_PIN7, 1);//vk
		}
		else if(MAC_ACTIVE_CFP_GTS4 == mac_superframe_state && (mac_pan_gts_table_len >= 5))
		{
			temp_index = mac_pan_gts_table_len - 5;
			next_timer_dur = slot_duration * (mac_pan_gts_table[mac_pan_gts_table_len - 5].GtsDesc.GtsLength);
			mac_superframe_state = MAC_ACTIVE_CFP_GTS5;
			temp_ptr = mac_pan_gts_table[temp_index].gts_data_q;
			sio2host_tx("-GTS5-",sizeof("-GTS1-"));
			ioport_set_value(DEBUG_PIN7, 0);//vk
			//ioport_set_value(DEBUG_PIN8, 1);//vk
		}
		else if(MAC_ACTIVE_CFP_GTS5 == mac_superframe_state && (mac_pan_gts_table_len >= 6))
		{
			temp_index = mac_pan_gts_table_len - 6;
			next_timer_dur = slot_duration * (mac_pan_gts_table[mac_pan_gts_table_len - 6].GtsDesc.GtsLength);
			mac_superframe_state = MAC_ACTIVE_CFP_GTS6;
			temp_ptr = mac_pan_gts_table[temp_index].gts_data_q;
			sio2host_tx("-GTS6-",sizeof("-GTS1-"));
			//ioport_set_value(DEBUG_PIN8, 0);//vk
		}
		else if(MAC_ACTIVE_CFP_GTS6 == mac_superframe_state && (mac_pan_gts_table_len == 7))
		{
			temp_index = mac_pan_gts_table_len - 7;
			next_timer_dur = slot_duration * (mac_pan_gts_table[mac_pan_gts_table_len - 7].GtsDesc.GtsLength);
			mac_superframe_state = MAC_ACTIVE_CFP_GTS7;
			temp_ptr = mac_pan_gts_table[temp_index].gts_data_q;
			sio2host_tx("-GTS7-",sizeof("-GTS1-"));
			//ioport_set_value(DEBUG_PIN4, 0);//vk
		}
		else
		{
			//if(tal_pib.SuperFrameOrder < tal_pib.BeaconOrder)
			//{
				//mac_superframe_state = MAC_INACTIVE;
			//}
			//else
			//{
				//mac_superframe_state = MAC_ACTIVE_CAP;
			//}
		}
		if(NULL != temp_ptr && (*temp_ptr).size > 0)
		{
			if (tal_pib.BeaconOrder >= 9)
			{
				mac_pan_gts_table[temp_index].ExpiryCount = GTS_EXPIRY_BO_9_TO_14;
			}
			else
			{
				mac_pan_gts_table[temp_index].ExpiryCount = GTS_EXPIRY_BO_0_TO_8;
			}
			mac_tx_gts_data(temp_ptr);
		}
	}
	else
#endif /* FFD */
	if (MAC_ASSOCIATED == mac_state)
	{
		ioport_set_value(DEBUG_PIN2, 0); //vk
		if(MAC_ACTIVE_CAP == mac_superframe_state)
		{
			next_timer_dur = slot_duration * (mac_dev_gts_table[DEV_TX_SLOT_INDEX].GtsLength);
			mac_superframe_state = MAC_ACTIVE_CFP_GTS1;
			sio2host_tx("-DGTS TX-",sizeof("-DGTS TX-"));
			ioport_set_value(DEBUG_PIN3, 1);//vk
			mac_tx_gts_data(&dev_tx_gts_q);
		}
		else if(MAC_ACTIVE_CFP_GTS1 == mac_superframe_state)
		{
			mac_superframe_state = MAC_ACTIVE_CAP;
			ioport_set_value(DEBUG_PIN3, 0); //vk
		}
	}
	if(0 != next_timer_dur)
	{
		pal_timer_start(T_CAP, next_timer_dur,
				TIMEOUT_RELATIVE,
				(FUNC_PTR)mac_t_gts_cb,
				NULL);
	}
	callback_parameter = callback_parameter;
	LEAVE_CRITICAL_REGION();
}

void init_gts_queues(void)
{
	uint8_t loop_index;
#ifdef FFD
	for(loop_index = 0; loop_index < MAX_GTS_ON_PANC; loop_index ++)
	{
#ifdef ENABLE_QUEUE_CAPACITY
		qmm_queue_init(&gts_q[loop_index], GTS_QUEUE_CAPACITY);
#else
		qmm_queue_init(&gts_q[loop_index]);
#endif  /* ENABLE_QUEUE_CAPACITY */
		mac_pan_gts_table[loop_index].gts_data_q = &gts_q[loop_index];
	}
#else
#ifdef ENABLE_QUEUE_CAPACITY
		qmm_queue_init(&dev_tx_gts_q, GTS_QUEUE_CAPACITY);
#else
		qmm_queue_init(&dev_tx_gts_q);
#endif  /* ENABLE_QUEUE_CAPACITY */
#endif
}

void mac_tx_gts_data(queue_t *gts_data)
{
	buffer_t *buf_ptr;
	frame_info_t *transmit_frame;
	retval_t tal_tx_status;
	ioport_toggle_pin(DEBUG_PIN8);
	buf_ptr = qmm_queue_remove(gts_data, NULL);

	Assert(buf_ptr != NULL);

	if (NULL == buf_ptr) {
		/* Nothing to be done. */
		return;
	}

	/* Broadcast data present and to be sent. */
	transmit_frame = (frame_info_t *)BMM_BUFFER_POINTER(buf_ptr);

	transmit_frame->buffer_header = buf_ptr;

	transmit_frame->gts_queue = gts_data;

	tal_tx_status = tal_tx_frame(transmit_frame, NO_CSMA_WITH_IFS, false);

	if (MAC_SUCCESS == tal_tx_status) {
		MAKE_MAC_BUSY();
		} else {
		mac_gen_mcps_data_conf(
		(buffer_t *)transmit_frame->buffer_header,
		(uint8_t)MAC_CHANNEL_ACCESS_FAILURE,
		#ifdef ENABLE_TSTAMP
		transmit_frame->msduHandle,
		0);
		#else
		transmit_frame->msduHandle);
		#endif  /* ENABLE_TSTAMP */
	}
}

uint8_t handle_gts_data_tx_end(void)
{
#ifdef FFD	
	if(MAC_PAN_COORD_STARTED == mac_state)
	{
		if(MAC_ACTIVE_CFP_GTS1 <= mac_superframe_state && MAC_ACTIVE_CFP_GTS7 >= mac_superframe_state)
		{
			mac_tx_gts_data(mac_pan_gts_table[mac_pan_gts_table_len - ((mac_superframe_state - MAC_ACTIVE_CFP_GTS1) + 1)].gts_data_q);
		}
	}
	else
#endif /* FFD */
	if (MAC_ASSOCIATED == mac_state && MAC_ACTIVE_CFP_GTS1 == mac_superframe_state)
	{
		mac_tx_gts_data(&dev_tx_gts_q);
	}
}

#endif /* GTS_SUPPORT */
