/**
 * @file mac_security.c
 *
 * @brief Handles MAC security
 *
 * This file handles MAC security according to IEEE 802.15.4-2006.
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

#if (defined MAC_SECURITY_ZIP) || (defined MAC_SECURITY_2006)
#include <compiler.h>
#include <string.h>
#include "tal.h"
#include "ieee_const.h"
#include "stack_config.h"
#include "mac_internal.h"
#include "stb.h"
#include "mac_security.h"

/* === Macros =============================================================== */

/* Security Control Field: Security Level mask */
#define SEC_CTRL_SEC_LVL_MASK           (0x07)

/* Security Control Field: Key Identifier mask */
#define SEC_CTRL_KEY_ID_MASK            (0x03)

/* Security Control Field: Key Identifier Field position */
#define SEC_CTRL_KEY_ID_FIELD_POS       (3)

#define SEC_CTRL_SEC_EN_MASK            (0x08)

#define FCF_FRAME_TYPE_MASK             (0x03)

#define FRAME_COUNTER_MAX_VAL           (0xFFFFFFFF)

#define SEC_CTRL_FLD_LEN                (0x01)
#define FRAME_COUNTER_LEN               (0x04)
#define IEEE_ADDR_LEN                   (0x08)
#define CRC_LEN                         (0x02)
#define SUPER_FRAME_SPEC_LEN			(0x02)
#define GTS_ADDR_SPEC_LEN				(0x01)
#define PENDING_ADDR_SPEC_LEN			(0x01)

/* key id mode and key identifier macros */
#define KEY_ID_MODE_0                   (0x00)
#define KEY_ID_MODE_1                   (0x01)
#define KEY_ID_MODE_2                   (0x02)
#define KEY_ID_MODE_3                   (0x03)
#define KEY_SRC_LEN_4                   (0x04)
#define KEY_SRC_LEN_8                   (0x08)
#define KEY_ID_LEN_MODE_1               (0x01)
#define KEY_ID_LEN_MODE_2               (0x05)
#define KEY_ID_LEN_MODE_3               (0x09)

#define FIVE_OCTET_LOOK_UP              (0x00)
#define NINE_OCTET_LOOK_UP              (0x01)

#define SHORT_ADDR_MAX                  (0xFFFD)
#define SHORT_ADDR_MIN                  (0x0000)
#define NO_SHORT_ADDR                   (0xFFFE)

#define LOOK_UP_SIZE_4                    (0x04)
#define LOOK_UP_SIZE_8                    (0x08)

#define LOOK_UP_SIZE_5                    (0x05)
#define LOOK_UP_SIZE_9                    (0x09)

#define FCF_POS                          (0x01)

/* === Globals ============================================================= */


/* === Prototypes ========================================================== */

static uint8_t get_mic_length(uint8_t security_level);

static inline uint8_t sec_additional_len(mcps_data_req_t *pmdr);

static inline retval_t parse_aux_sec_header(parse_t *mac_parse_data,
		uint8_t *mac_payload);

static inline retval_t unsecure_frame(parse_t *mac_parse_data_buf, uint8_t *mpdu,
		uint8_t *mac_payload,
		uint8_t *payload_index);

static inline retval_t outgoing_key_retrieval(mcps_data_req_t *pmdr, mac_key_table_t **key_desc);

static inline retval_t incoming_sec_material_retrieval(parse_t *mac_parse_data, mac_key_table_t **key_desc,
                                                       mac_device_desc_t **device_desc,
                                                       mac_key_device_desc_t **key_device_desc);

static void create_nonce(uint8_t *ieee_addr, uint8_t *frame_cnt, uint8_t security_level, uint8_t *nonce);

static retval_t key_descriptor_lookup(uint8_t *lookup_data, uint8_t lookup_data_size, mac_key_table_t **key_desc);

static retval_t device_descriptor_lookup(mac_device_desc_t device_desc,
                                         uint8_t *device_lookup_data,
                                         uint8_t device_lookup_data_size);

static retval_t blacklist_checking_procedure(uint8_t *device_lookup_data, uint8_t device_lookup_data_size,
                                             mac_key_table_t *key_desc,
                                             mac_device_desc_t **device_desc,
                                             mac_key_device_desc_t **key_device_desc);
											 
static retval_t incoming_key_usage_policy(mac_key_table_t *key_desc,
										  uint8_t frame_type, 
										  uint8_t mac_cmd);											 

/* === Implementation ====================================================== */

/* --- Helper Functions ---------------------------------------------------- */
/*
 * @brief Gets the length of the Key Identifier field
 *
 * This function returns the length of the Auxiliary Security Header
 *
 * @param tx_frame Frame information structure of current frame
 *
 * @return Length of Auxiliary Security Header
 */

static inline uint8_t sec_additional_len(mcps_data_req_t *pmdr)
{
	uint8_t len;

    /* Auxiliary security header length */
    len  = SEC_CTRL_FLD_LEN     /* security ctrl field */
           + FRAME_COUNTER_LEN   /* frame counter length */
           + get_key_id_field_len(pmdr->KeyIdMode);

	/* MIC length */
	len += get_mic_length(pmdr->SecurityLevel);

	return len;
}

/*
 * @brief Gets the length of the Key Identifier field
 *
 * This function returns the length of the Key Identifier field
 * within the Auxiliary Security Header of a secured frame based
 * on the Key Identifier Mode.
 *
 * @param key_id_mode Key Identifier Mode
 *
 * @return Length of Key Identifier field in octets.
 * @ToDo Check if lookup table implementation needs less footprint
 */
uint8_t get_key_id_field_len(uint8_t key_id_mode)
{
    uint8_t len_field = 0;

    switch (key_id_mode)
    {
        case KEY_ID_MODE_1:
            len_field = KEY_ID_LEN_MODE_1;
            break;
        case KEY_ID_MODE_2:
            len_field = KEY_ID_LEN_MODE_2;
            break;
        case KEY_ID_MODE_3:
            len_field = KEY_ID_LEN_MODE_3;
            break;
        default:
            break;
    }

    return (len_field);
}

/**
 * @brief Calculates the length of the MIC
 *
 * This function returns the length of the MIC depending on the given security
 *level
 *
 * @param security_level Security Level of current frame
 *
 * @return Length of MIC in octets.
 * @ToDo Check if lookup table implementation needs less footprint
 */
static uint8_t get_mic_length(uint8_t security_level)
{
    uint8_t mic_len = 0;

    switch (security_level)
    {
        case 1:
        case 5:
            mic_len = LEN_MIC_32;
            break;

        case 2:
        case 6:
            mic_len = LEN_MIC_64;
            break;

        case 3:
        case 7:
            mic_len = LEN_MIC_128;
            break;

        default:
            break;
    }

    return mic_len;
}

static void create_nonce(uint8_t *ieee_addr, uint8_t *frame_cnt,
		uint8_t security_level, uint8_t *nonce)
{
	/*
	 * Create Nonce - Attentation: byte order is inverse in comparison to
	 *RF4CE
	 * RF4CE: Little endian
	 */
	uint8_t *ptr;
	uint8_t *nonce_ptr = &nonce[1];

	ptr = ieee_addr;
    ptr += IEEE_ADDR_LEN - 1;
    for (uint8_t i = 0; i < IEEE_ADDR_LEN; i++) {
		*nonce_ptr++ = *ptr--;
	}
	ptr = frame_cnt;
    ptr += FRAME_COUNTER_LEN - 1;
    for (uint8_t i = 0; i < FRAME_COUNTER_LEN; i++) {
		*nonce_ptr++ = *ptr--;
	}
	*nonce_ptr = security_level; /* Only security level NOT security control
	                              * field */
}

/* --- Outgoing Security --------------------------------------------------- */

/*
 * @brief Generates Auxiliary Security Header fields
 *
 * This function generates the required fields of the
 * Auxiliary Security Header of a secured frame based
 * on the actual security parameter.
 *
 * @param tx_frame Frame information structure of current frame
 * @param security_level Security Level of current frame
 * @param key_id_mode Key Identifier Mode of current frame
 * @param key_source Key Source of current frame
 * @param key_index Key Index of current frame
 *
 * @return Status of extraction of Auxiliary Security Header fields
 */
retval_t mac_build_aux_sec_header(uint8_t **frame_ptr, mcps_data_req_t *pmdr,
		uint8_t *frame_len)
{
	uint8_t sec_info_len = sec_additional_len(pmdr);
	*frame_ptr = *frame_ptr - sec_info_len;
	/* Add security field length to original frame length */
	*frame_len += sec_info_len;

	/* Create Auxiliary security header. */
	uint8_t *sec_msdu_ptr = *frame_ptr;
	/* Fill in Security Control Field. */
	*sec_msdu_ptr = (pmdr->SecurityLevel & SEC_CTRL_SEC_LVL_MASK) |
			((pmdr->KeyIdMode &
			SEC_CTRL_KEY_ID_MASK) << SEC_CTRL_KEY_ID_FIELD_POS);
	sec_msdu_ptr++;

    /* Fill in Frame Counter. */
    if (mac_sec_pib.FrameCounter == FRAME_COUNTER_MAX_VAL)
    {
        return MAC_COUNTER_ERROR;
    }
    else
    {
        memcpy(sec_msdu_ptr, &mac_sec_pib.FrameCounter, FRAME_COUNTER_LEN);
        sec_msdu_ptr += FRAME_COUNTER_LEN;
    }

    /*
     * Key Source and Key Index subfields are only present
     * if KeyIdMode is not zero.
     */
    if (pmdr->KeyIdMode == KEY_ID_MODE_1)
    {
        *sec_msdu_ptr = pmdr->KeyIndex;
    }
    else if (pmdr->KeyIdMode == KEY_ID_MODE_2)
    {
        memcpy(sec_msdu_ptr, pmdr->KeySource, KEY_SRC_LEN_4);
        sec_msdu_ptr += KEY_SRC_LEN_4;
        *sec_msdu_ptr = pmdr->KeyIndex;
    }
    else if (pmdr->KeyIdMode == KEY_ID_MODE_3)
    {
        memcpy(sec_msdu_ptr, pmdr->KeySource, KEY_SRC_LEN_8);
        sec_msdu_ptr += KEY_SRC_LEN_8;
        *sec_msdu_ptr = pmdr->KeyIndex;
    }
    return MAC_SUCCESS;
}


/*
 * @brief Secures MAC frame
 *
 * This function secures the given MAC frame.
 *
 * @param tx_frame Frame information structure of current frame
 * @param security_level Security Level of current frame
 * @param key_id_mode Key Identifier Mode
 *
 * @return retval_t MAC_SUCCESS or MAC_UNSUPPORTED_SECURITY
 */


/** Test vectors for Testing the MAC Security as per IEEE 802.15.4 2011 Spec */
#ifdef MAC_SECURITY_TEST_VECTOR
/** Test Vector  for MAC Cmd Testing */
#if 0  
	uint8_t key_local[16] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF};
	uint8_t test_payload[] = {0x01, 0xCE};
	uint8_t text_header[] = {0x2B, 0xDC, 0x84, 0x21, 0x43, 0x02, 0x00, 0x00, 0x00, 0x00, 0x48, \
							 0xDE, 0xAC, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x48, 0xDE, \
							 0xAC, 0x06, 0x05, 0x00, 0x00, 0x00, 0x01, 0xCE};
	uint8_t nonce_test[AES_BLOCKSIZE] = {0};
									 
	uint8_t hdr_len_test = 29;
	uint64_t IeeeAddress = 0xacde480000000001;
	uint64_t IeeeAddress_dst  = 0xacde480000000002;
	uint32_t FrameCounter = 0x5;
	uint8_t SecurityLevel = 0x06;	
	uint8_t enc_data_test[aMaxPHYPacketSize - FCS_LEN + AES_BLOCKSIZE];
	uint8_t msdu_length = 1;
#endif	

/** Test Vector  for MAC Beacon Testing */
#if 0 
	uint8_t key_local[16] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF};
	uint8_t test_payload[] = {0x51, 0x52, 0x53, 0x54};
	uint8_t text_header[] = {0x08, 0xD0, 0x84, 0x21, 0x43, 0x01, 0x00, 0x00, 0x00, 0x00, 0x48, \
		                     0xDE, 0xAC, 0x02, 0x05, 0x00, 0x00, 0x00, 0x55, 0xCF, 0x00, 0x00, \
							 0x51, 0x52, 0x53, 0x54};
	uint8_t nonce_test[AES_BLOCKSIZE] = {0};
									 
	uint8_t hdr_len_test = 26;
	uint64_t IeeeAddress = 0xacde480000000001;
	uint32_t FrameCounter = 0x5;
	uint8_t SecurityLevel = 0x02;	
	uint8_t enc_data_test[aMaxPHYPacketSize - FCS_LEN + AES_BLOCKSIZE];
	uint8_t msdu_length = 0;
#endif	

/* Test Vector  for Data Encryption Testing */
#if 0  
	uint8_t key_local[16] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF};
	uint8_t test_payload[] = {0x61, 0x62, 0x63, 0x64};
	uint8_t text_header[] = {0x69, 0xDC, 0x84, 0x21, 0x43, 0x02, 0x00, 0x00, 0x00, 0x00, 0x48, 0xDE, 0xAC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x48, 0xDE, 0xAC, \
		                     0x04, 0x05, 0x00, 0x00, 0x00, 0x61, 0x62, 0x63, 0x64};
	uint8_t nonce_test[AES_BLOCKSIZE] = {0};
									 
	uint8_t hdr_len_test = 26;
	uint64_t IeeeAddress = 0xacde480000000001;
	uint64_t IeeeAddress_dst  = 0xacde480000000002;
	uint32_t FrameCounter = 0x5;
	uint8_t SecurityLevel = 0x04;	
	uint8_t enc_data_test[aMaxPHYPacketSize - FCS_LEN + AES_BLOCKSIZE] = {0};
	uint8_t msdu_length = 4;
#endif	
#endif


retval_t mac_secure(frame_info_t *frame, uint8_t *mac_payload_ptr,
		mcps_data_req_t *pmdr)
{
#ifdef MAC_SECURITY_TEST_VECTOR
{
	create_nonce((uint8_t *)&IeeeAddress, (uint8_t *)&FrameCounter, SecurityLevel, nonce_test);
    memcpy(enc_data_test, text_header, (hdr_len_test + msdu_length));
	stb_ccm_secure( enc_data_test, /* plain text header (string a) and payload concatenated */
					nonce_test,
					key_local, /* security_key */
					hdr_len_test, /* plain text header length */
					msdu_length, /* Length of payload to be encrypted */
					SecurityLevel, /* security level **/
					AES_DIR_ENCRYPT);   					
}	
#endif	
	
	uint8_t *key;
    mac_key_table_t *key_desc;
    bool security_enabled = frame->mpdu[FCF_POS] & SEC_CTRL_SEC_EN_MASK;
    /* 7.5.8.2.1 (a) */
    if (security_enabled == false)
    {
        pmdr->SecurityLevel = 0;
    }
    /* 7.5.8.2.1 (b) */
    if (security_enabled == true && pmdr->SecurityLevel == 0)
    {
        return MAC_UNSUPPORTED_SECURITY;
    }
    /* 7.5.8.2.1 (c) */
    if ((mac_pib.mac_SecurityEnabled == false) && (pmdr->SecurityLevel != 0))
    {
        return MAC_UNSUPPORTED_SECURITY;
    }
    /* 7.5.8.2.1 (g) */
    /* Retrieval of the key */
     retval_t key_status = outgoing_key_retrieval(pmdr, &key_desc);
	 if (key_status != MAC_SUCCESS)
     {
        return key_status;
     }
	 key = key_desc->Key;
    
    uint8_t m = get_mic_length(pmdr->SecurityLevel);

    /* Encrypt payload data */
    uint8_t nonce[AES_BLOCKSIZE] = {0};
	uint8_t enc_data[aMaxPHYPacketSize - FCS_LEN + AES_BLOCKSIZE];
	/* Extra AES Bloc size at end required for MIC generation. */
	create_nonce((uint8_t *)&tal_pib.IeeeAddress,
				(uint8_t *)&mac_sec_pib.FrameCounter,
				pmdr->SecurityLevel,
				nonce);
	/*
	 * Create string a
	 * Here: MHR | Aux sec header = MHR | SecLvl | FrameCounter
	 * = FCF (2) | SeqNo (1) | AddrFields
	 */
	uint8_t hdr_len = mac_payload_ptr - &frame->mpdu[1] - m;
    memcpy(enc_data, &frame->mpdu[FCF_POS], hdr_len);

    switch (frame->mpdu[FCF_POS] & FCF_FRAME_TYPE_MASK)   // FCF
	{
	case FCF_FRAMETYPE_BEACON:                  /* (0x00) */
	{		    
			/* Append payload (string m) */
			memcpy(&enc_data[hdr_len], mac_payload_ptr, pmdr->msduLength);
			{
				uint8_t *current_key;

				/* Shall the real key be used or rather a test key? */
				current_key = key;

				stb_ccm_secure(enc_data, /* plain text header (string a)
										  *and payload concatenated */
						nonce,
						current_key, /*security_key */
						hdr_len, /* plain text header length */
						pmdr->msduLength, /* Length of payload
												   *to be encrypted */
						pmdr->SecurityLevel, /* security level
													  **/
						AES_DIR_ENCRYPT);
			}	
			mac_sec_pib.FrameCounter++;
					
			/* Replace original payload by secured payload */
			memcpy(mac_payload_ptr - m, &enc_data[hdr_len],
					pmdr->msduLength + m);
	}
		break;		
	case FCF_FRAMETYPE_DATA:                  /* (0x01) */
	{
			/* Append payload (string m) */
			memcpy(&enc_data[hdr_len], mac_payload_ptr, pmdr->msduLength);
			{
				uint8_t *current_key;

				/* Shall the real key be used or rather a test key? */
				current_key = key;

				stb_ccm_secure(enc_data, /* plaintext header (string a)
										  *and payload concatenated */
						nonce,
						current_key, /*security_key */
						hdr_len, /* plaintext header length */
						pmdr->msduLength, /* Length of payload
												   *to be encrypted */
						pmdr->SecurityLevel, /* security level
													  **/
						AES_DIR_ENCRYPT);
			}

			mac_sec_pib.FrameCounter++;

			/* Replace original payload by secured payload */
			memcpy(mac_payload_ptr - m, &enc_data[hdr_len],
					pmdr->msduLength + m);
	}
		break;
	default:
		return MAC_UNSUPPORTED_SECURITY;
	}
	return MAC_SUCCESS;
}

/*
 * @brief This function retrieves the  key descriptor for the outgoing procedure
 *
 * @param tx_frame Frame information structure of current frame
 * @param Pointer to the key descriptor
 *
 * @return retval_t MAC_SUCCESS or MAC_UNSUPPORTED_SECURITY or MAC_UNAVAILABLE_KEY
 */
// 7.5.8.2.2
static inline retval_t outgoing_key_retrieval(mcps_data_req_t *pmdr, mac_key_table_t **key_desc)
{
	uint8_t key_lookup_data_size;
    uint8_t lookup_data[9] = {0};
    mac_key_table_t *curr_key_desc;
    if (pmdr->KeyIdMode == KEY_ID_MODE_0)    // implicit key identification
    {
		switch (pmdr->DstAddrMode) {
		/* 1) is not applicable for ZIP */
		/* 2) is not applicable for ZIP */
#ifndef MAC_SECURITY_ZIP
                /* 7.5.8.2.2 - a.1 & a.2 */
            case FCF_NO_ADDR:
                if (mac_sec_pib.PANCoordShortAddress <= SHORT_ADDR_MAX && mac_sec_pib.PANCoordShortAddress >= SHORT_ADDR_MIN)
                {
                    lookup_data[1] = (uint8_t)tal_pib.PANId;
                    lookup_data[0] = (uint8_t)(tal_pib.PANId >> 8);
                    lookup_data[3] = (uint8_t)mac_sec_pib.PANCoordShortAddress;
                    lookup_data[2] = (uint8_t)(mac_sec_pib.PANCoordShortAddress >> 8);
                    lookup_data[4] = 0x00;
                    key_lookup_data_size = FIVE_OCTET_LOOK_UP; // key lookup size = 5 -> key lookup data size = 0
                }
                else if (mac_sec_pib.PANCoordShortAddress == NO_SHORT_ADDR)
                {
                    memcpy(&lookup_data[0], &(mac_sec_pib.PANCoordExtendedAddress), 8);
                    lookup_data[8] = 0x00;
                    key_lookup_data_size = NINE_OCTET_LOOK_UP;   // key lookup size = 9 -> key lookup data size = 1
                }
                break;
#endif
                /* 7.5.8.2.2 -  a.3 */
		case FCF_SHORT_ADDR:
			/* lookup_data: Dest PANId right-concatenated */
			lookup_data[1] = (uint8_t)pmdr->DstPANId; /* LSB */
			lookup_data[0] = (uint8_t)(pmdr->DstPANId >> 8); /* MSB */
			/* 2-octet dest addr right-concatenated */
			lookup_data[3] = (uint8_t)pmdr->DstAddr; /* LSB */
			lookup_data[2] = (uint8_t)(pmdr->DstAddr >> 8); /* MSB */
			lookup_data[4] = 0x00; /* single octet 0x00 */
			key_lookup_data_size = FIVE_OCTET_LOOK_UP; // key lookup size = 5 -> key lookup data size = 0			                           
			break;

		case FCF_LONG_ADDR:
			/* lookup_data: 8-octet dest addr right-concatenated */
			memcpy(&lookup_data[0], &pmdr->DstAddr, 8);
			lookup_data[8] = 0x00; /* single octet 0x00 */
			key_lookup_data_size = NINE_OCTET_LOOK_UP; /* key lookup size = 9 -> key
			                           * lookup data size = 1 */
			break;

		default:
			return MAC_UNSUPPORTED_SECURITY; /* applies for ZIP only */
		}
	} 
	else if (pmdr->KeyIdMode == KEY_ID_MODE_1) 
	{ /* explicit key identification */
		/*
		 * lookup_data:
		 * 8-octet of macDefaultKeySource right-concatenated with
		 * single octet: Key Index parameter = tx_frame->key_id[0]
		 */
		for (uint8_t i = 0; i < KEY_SRC_LEN_8; i++) 
		{
			lookup_data[i] = mac_sec_pib.DefaultKeySource[i];
		}
		lookup_data[KEY_SRC_LEN_8] = pmdr->KeyIndex; /* tx_frame->key_id[0]; */
		key_lookup_data_size = NINE_OCTET_LOOK_UP; /* key lookup size = 9 -> key lookup
		                           * data size = 1 */
	}

#ifdef MAC_SECURITY_2006
    else if (pmdr->KeyIdMode == KEY_ID_MODE_2)
    {
        /*
        * lookup_data: 4-octet of KeySource
        * right-concatenated with
        * single octet: Key Index parameter
        */
        uint8_t *FrameKeySource = pmdr->KeySource;
        for (uint8_t i = 0; i < KEY_SRC_LEN_4; i++)
        {
            lookup_data[i] = *FrameKeySource++;
        }
        lookup_data[KEY_SRC_LEN_4] =  pmdr->KeyIndex;
        key_lookup_data_size = FIVE_OCTET_LOOK_UP;   // key lookup size = 5 -> key lookup data size = 0

    }
    else if (pmdr->KeyIdMode == KEY_ID_MODE_3)
    {
        /*
         * lookup_data: 8-octet of KeySource
         * right-concatenated with
         * single octet: Key Index parameter
         */
        uint8_t *FrameKeySource = pmdr->KeySource;
        for (uint8_t i = 0; i < KEY_SRC_LEN_8; i++)
        {
            lookup_data[i] = *FrameKeySource++;
        }
        lookup_data[KEY_SRC_LEN_8] =  pmdr->KeyIndex;
        key_lookup_data_size = NINE_OCTET_LOOK_UP;   // key lookup size = 9 -> key lookup data size = 1

    }
#endif  /* MAC_SECURITY_2006 */
    else
    {
        // other than 0x00 and 0x01 are NOT used for ZIP
        return MAC_UNSUPPORTED_SECURITY;   // applies for ZIP only
    }

    // Get key from KeyDescriptor as 7.5.8.2.5
    retval_t status = key_descriptor_lookup(lookup_data, key_lookup_data_size, &curr_key_desc);
    *key_desc = curr_key_desc;
    if (status != MAC_SUCCESS)
    {
        return status;
    }
    return MAC_SUCCESS;
}


/* --- Incoming Security --------------------------------------------------- */


/**
 * @brief Handles unsecuring of MAC frame
 *
 * This function handles the complete unsecuring of a MAC frame.
 * This includes the extraction of the Auxiliary Security Header and
 * the actual frame decryption.
 *
 * @param[in] tx_frame Frame information structure of current frame
 * @param[in] security_level Security Level of current frame
 * @param[in] key_id_mode Key Identifier Mode of current frame
 * @param[in] key_source Key Source of current frame
 * @param[in] key_index Key Index of current frame
 * @param[out] cur_data_frame_len Current length of frame before applying
 *security
 *
 * @return Status of extraction of Auxiliary Security Header fields
 */
retval_t mac_unsecure(parse_t *mac_parse_data_buf, uint8_t *mpdu,
		uint8_t *mac_payload, uint8_t *payload_index)
{
    retval_t status;
    status = parse_aux_sec_header(mac_parse_data_buf, mac_payload);
    if (status != MAC_SUCCESS)
    {
        return status;
    }
    /* 7.5.8.2.3 (d) */
    if (mac_pib.mac_SecurityEnabled == false)
    {

        if (mac_parse_data_buf->sec_ctrl.sec_level == 0)
        {
            return MAC_SUCCESS;
        }
        else
        {
            return MAC_UNSUPPORTED_SECURITY;
        }
    }
    /* 7.5.8.2.3 (e) todo implementation of 7.5.8.2.8*/
    /* 7.5.8.2.3 (f) */
    if (mac_parse_data_buf->sec_ctrl.sec_level == 0)
    {
        return MAC_SUCCESS;
    }

    status = unsecure_frame(mac_parse_data_buf, mpdu, mac_payload, payload_index);
    return status;
}


/*
 * @brief Generates Auxiliary Security Header fields
 *
 * This function extracts the actual security parameters
 * from the Auxiliary Security Header of a received secured frame.
 *
 * @param[in] mac_parse_data Parsed information of the received frame
 * @param[out] security_level Extracted Security Level of received frame
 * @param[out] key_id_mode Extracted Key Identifier Mode of received frame
 * @param[out] key_source Extracted Key Source of received frame
 * @param[out] key_index Extracted Key Index of received frame
 *
 * @return Status of generation of Auxiliary Security Header fields
 */
static inline retval_t parse_aux_sec_header(parse_t *mac_parse_data_instance,
		uint8_t *mac_payload)
{
    memcpy(&mac_parse_data_instance->sec_ctrl, &mac_payload[0], SEC_CTRL_FLD_LEN);
    memcpy(&mac_parse_data_instance->frame_cnt, &mac_payload[1], FRAME_COUNTER_LEN);
	mac_parse_data_instance->key_id_len = get_key_id_field_len(
			mac_parse_data_instance->sec_ctrl.key_id_mode);
    /* 7.5.8.2.3 (c)*/
    if (mac_parse_data_instance->sec_ctrl.sec_level == 0)
    {
        return MAC_UNSUPPORTED_SECURITY;
    }
	if (mac_parse_data_instance->sec_ctrl.key_id_mode != 0) {
         mac_payload += 5;
		if (mac_parse_data_instance->key_id_len > 1) {
            /* Key Id Len must not be larger than 1 for ZIP. */
#ifndef MAC_SECURITY_2006
            return MAC_UNSUPPORTED_SECURITY;
#else
            memcpy(mac_parse_data_instance->key_id, mac_payload, mac_parse_data_instance->key_id_len - 1);
#endif
        }
        mac_payload += mac_parse_data_instance->key_id_len - 1;
        mac_parse_data_instance->key_id[mac_parse_data_instance->key_id_len - 1] = *(mac_payload);
    }

    /* See 802.15.4-2006 section 7.5.8.2.3 b) */
    if ((mac_parse_data_instance->fcf & FCF_SECURITY_ENABLED) &&
        (!(mac_parse_data_instance->fcf & FCF_FRAME_VERSION_2006))
       )
    {
        return MAC_UNSUPPORTED_LEGACY;
    }
    return MAC_SUCCESS;
}

/**
 * @brief Unsecures MAC frame
 *
 * This function unsecures the given MAC frame.
 *
 * @param mac_parse_data Frame information structure of current frame
 * @param security_level Security Level of current frame
 *
 * @return retval_t MAC_SUCCESS, MAC_UNSUPPORTED_SECURITY or MAC_SECURITY_ERROR
 */
static inline retval_t unsecure_frame(parse_t *mac_parse_data_buf, uint8_t *mpdu, uint8_t *mac_payload, uint8_t *payload_index)
{
    /* Encrypt payload data */
    uint8_t nonce[AES_BLOCKSIZE] = {0};   /* AES_BLOCKSIZE 16 */

    /* Incoming key retrieval */
    uint8_t *key;
    mac_key_table_t *key_desc;
    mac_device_desc_t *device_desc;
    mac_key_device_desc_t *key_device_desc;

    /* Todo 7.5.8.2.3 (e) 7.5.8.2.8 security level checking procedure*/
		retval_t status = incoming_sec_material_retrieval(mac_parse_data_buf, &key_desc, &device_desc, &key_device_desc);
		if (status != MAC_SUCCESS)
		{
			return MAC_UNAVAILABLE_KEY;
		}
		key = key_desc->Key;

    /* 7.5.8.2.3 (j) & (k)*/
    if (((FCF_FRAMETYPE_DATA == mac_parse_data_buf->frame_type) ||\
	    (FCF_FRAMETYPE_BEACON == mac_parse_data_buf->frame_type))&& \
	((mac_parse_data_buf->frame_cnt == FRAME_COUNTER_MAX_VAL) || \
	    (mac_parse_data_buf->frame_cnt < device_desc->FrameCounter)))
    {
        return MAC_COUNTER_ERROR;
    }

    /*
     * Create Nonce - Attentation: byte order is inverse in comparison to RF4CE
     * RF4CE: Little endian
     */
    uint8_t *addr_ptr;
    uint8_t *current_key;   // Pointer to actually used key

    /* Standard way of extracting Src IEEE address. */
    addr_ptr = (uint8_t *)&device_desc->ExtAddress;
#ifdef MAC_SECURITY_BEACON
		uint64_t coord_ieee_address;
		if (FCF_FRAMETYPE_BEACON == mac_parse_data_buf->frame_type)
		{
			uint8_t *addr_index = (uint8_t *)&coord_ieee_address;
			
			memcpy(&addr_index[0], &mac_sec_pib.PANCoordExtendedAddress[0],
			sizeof(coord_ieee_address));
			
			addr_ptr = (uint8_t *)&coord_ieee_address;
		}		
#endif
    uint8_t m = get_mic_length(mac_parse_data_buf->sec_ctrl.sec_level);

    create_nonce(addr_ptr, (uint8_t *)&mac_parse_data_buf->frame_cnt, mac_parse_data_buf->sec_ctrl.sec_level, nonce);

    switch (mac_parse_data_buf->frame_type)
    {
        case FCF_FRAMETYPE_DATA:                  //(0x01)
            {
                /* Shall the real key be used or rather a test key? */
                current_key = key;

                uint8_t sec_hdr_len = SEC_CTRL_FLD_LEN + FRAME_COUNTER_LEN + mac_parse_data_buf->key_id_len;  // 5 = sec ctrl + frame counter
                uint8_t mhr_len = mac_payload - mpdu + sec_hdr_len;
                uint8_t encryp_payload_len = mac_parse_data_buf->mpdu_length - mhr_len - m - CRC_LEN;  // 2 = CRC

                if (stb_ccm_secure(mpdu, /* plaintext header (string a) and payload concatenated */
                                   nonce,
                                   current_key, /* security_key */
                                   mhr_len, /* plaintext header length */
                                   encryp_payload_len, /* Length of payload to be encrypted */
                                   mac_parse_data_buf->sec_ctrl.sec_level, /* security level */
                                   AES_DIR_DECRYPT)
                    == STB_CCM_OK)
                {
                    /* Adjust payload by secured payload */
                    *payload_index = sec_hdr_len;
                    /* Subtract the MIC length from the payload length. */
                    mac_parse_data_buf->mac_payload_length = encryp_payload_len;
                }
                else
                {
                    return MAC_SECURITY_ERROR;
                }
			}
                break;
				
		case FCF_FRAMETYPE_BEACON:                  //(0x00)
            {
                /* Shall the real key be used or rather a test key? */
                current_key = key;
				uint8_t beacon_add_len = 0;
				uint8_t gts_field_len = 0;
				uint8_t pending_addr_len = 0;

                uint8_t sec_hdr_len = SEC_CTRL_FLD_LEN + FRAME_COUNTER_LEN + \
									   mac_parse_data_buf->key_id_len;  // 5 = sec ctrl + frame counter
									  
                uint8_t mhr_len = mac_payload - mpdu + sec_hdr_len;
				uint8_t encryp_payload_len = mac_parse_data_buf->mpdu_length - mhr_len - m - CRC_LEN;  // 2 = CRC
				/* calculate the Fixed Field of the GTS Length and Pending Address Field Length and 
				  * Super Frame Specification
				  */
				beacon_add_len = SUPER_FRAME_SPEC_LEN + GTS_ADDR_SPEC_LEN + PENDING_ADDR_SPEC_LEN;
				
				/* Calculate the variable field GTS Field Length */      
				gts_field_len = ((mpdu[(mhr_len + SUPER_FRAME_SPEC_LEN)] & 0x07) * 3);
				if (gts_field_len)
				{
					/* GTS direction fields are available */
					gts_field_len++;
				}
				/* Calculate the variable field Pending Address Field Length */ 
				pending_addr_len = ((mpdu[(mhr_len + SUPER_FRAME_SPEC_LEN + GTS_ADDR_SPEC_LEN + \
									gts_field_len)] & 0x07) * 2) + \
								   (((mpdu[(mhr_len + SUPER_FRAME_SPEC_LEN + GTS_ADDR_SPEC_LEN + \
								    gts_field_len)] & 0x70) >> 4) * 8);
								   
				/* Total Additional Address Length Field */
				beacon_add_len += pending_addr_len + gts_field_len;							   

                if (stb_ccm_secure(mpdu, /* plaintext header (string a) and payload concatenated */
                                   nonce,
                                   current_key, /* security key */
                                   (mhr_len + beacon_add_len), /* plaintext header length */
                                   (encryp_payload_len - beacon_add_len), /* Length of payload to be encrypted */
                                   mac_parse_data_buf->sec_ctrl.sec_level, /* security level */
                                   AES_DIR_DECRYPT)
                    == STB_CCM_OK)
                {
                    /* Adjust payload by secured payload */
                    *payload_index = sec_hdr_len;
                    /* Subtract the MIC length from the payload length. */
                    mac_parse_data_buf->mac_payload_length = encryp_payload_len + sec_hdr_len;
                }
                else
                {
                    return MAC_SECURITY_ERROR;
                }
			}
              break;
            
        default:
            return MAC_UNSUPPORTED_SECURITY;
    }
    /* 7.5.8.2.3 (n) */
	if ((FCF_FRAMETYPE_DATA == mac_parse_data_buf->frame_type) || 
				(FCF_FRAMETYPE_BEACON == mac_parse_data_buf->frame_type))
	{
		device_desc->FrameCounter = (mac_parse_data_buf->frame_cnt) + 1;
		/* 7.5.8.2.3 (o) */
		if (device_desc->FrameCounter == FRAME_COUNTER_MAX_VAL)
		{
			key_device_desc->BlackListed = true;
		}
	}	

    return MAC_SUCCESS;
}


static inline retval_t incoming_sec_material_retrieval(parse_t *mac_parse_data_buf, mac_key_table_t **key_desc,
                                                       mac_device_desc_t **device_desc,
                                                       mac_key_device_desc_t **key_device_desc)
{
    /* @ToDo: Check a holy bunch of other stuff, see 7.5.8.2.3*/

    uint8_t lookup_data_size = 0;
    uint8_t lookup_data[9];
    uint8_t device_lookup_data_size = 0;
    uint8_t device_lookup_data[8];
    mac_key_table_t *curr_key_desc;
    mac_device_desc_t *current_device_desc;
    mac_key_device_desc_t *curr_key_device_desc;
    /* 7.5.8.2.3 (a) & (b) */
    if (mac_parse_data_buf->sec_ctrl.key_id_mode == KEY_ID_MODE_0)    // implicit
    {
        switch (mac_parse_data_buf->src_addr_mode)
        {
                // 1) is not applicable for ZIP
                // 2) is not applicable for ZIP
            case FCF_NO_ADDR:
                {
                    if (mac_sec_pib.PANCoordShortAddress <= SHORT_ADDR_MAX && mac_sec_pib.PANCoordShortAddress >= SHORT_ADDR_MIN)
                    {
                        lookup_data[1] = (uint8_t)(mac_parse_data_buf->dest_panid);
                        lookup_data[0] = (uint8_t)(mac_parse_data_buf->dest_panid >> 8);
                        lookup_data[3] = (uint8_t)mac_sec_pib.PANCoordShortAddress;
                        lookup_data[2] = (uint8_t)(mac_sec_pib.PANCoordShortAddress >> 8);
                        lookup_data[4] = 0x00;
                        lookup_data_size = FIVE_OCTET_LOOK_UP; // key lookup size = 5 -> key lookup data size = 0
                    }
                    else if (mac_sec_pib.PANCoordShortAddress == NO_SHORT_ADDR)
                    {
                        memcpy(&lookup_data[0], &(mac_sec_pib.PANCoordExtendedAddress), 8);
                        lookup_data[8] = 0x00;
                        lookup_data_size = NINE_OCTET_LOOK_UP;   // key lookup size = 9 -> key lookup data size = 1
                    }
                }
                break;
            case FCF_SHORT_ADDR:
                {
                    //PAN Id: TAL handles PAN ID compression issue
                    lookup_data[1] = (uint8_t)mac_parse_data_buf->src_panid; // LSB
                    lookup_data[0] = (uint8_t)(mac_parse_data_buf->src_panid >> 8); // MSB
                    // 2-octet dest addr right-concatenated
                    lookup_data[3] = (uint8_t)mac_parse_data_buf->src_addr.short_address; // LSB
                    lookup_data[2] = (uint8_t)(mac_parse_data_buf->src_addr.short_address >> 8); // MSB
                    lookup_data[4] = 0x00;  // single octet 0x00
                    lookup_data_size = FIVE_OCTET_LOOK_UP; // key lookup size = 5 -> key lookup data size = 0
                }
                break;

            case FCF_LONG_ADDR:
                {
                    // lookup_data: 8-octet dest addr right-concatenated
                    memcpy(&lookup_data[0], &mac_parse_data_buf->src_addr.long_address, 8);
                    lookup_data[8] = 0x00;  // single octet 0x00
                    lookup_data_size = NINE_OCTET_LOOK_UP; // key lookup size = 9 -> key lookup data size = 1
                }
                break;
            default:
                return MAC_UNSUPPORTED_SECURITY;
        }
    }
    else if (mac_parse_data_buf->sec_ctrl.key_id_mode == KEY_ID_MODE_1)
    {
        // lookup_data: 8-octet of macDefaultKeySource right-concatenated
        // single octet: Key Index parameter
        for (uint8_t i = 0; i < 8; i++)
        {
            lookup_data[i] = mac_sec_pib.DefaultKeySource[i];
        }
        lookup_data[8] = mac_parse_data_buf->key_id[0];
        lookup_data_size = NINE_OCTET_LOOK_UP; // key lookup size = 9 -> key lookup data size = 1
    }
    else if (mac_parse_data_buf->sec_ctrl.key_id_mode == KEY_ID_MODE_2)
    {
        for (uint8_t i = 0; i < KEY_ID_LEN_MODE_2; i++)
        {
            lookup_data[i] = mac_parse_data_buf->key_id[i];
        }
        lookup_data_size = FIVE_OCTET_LOOK_UP;
    }
    else if (mac_parse_data_buf->sec_ctrl.key_id_mode == KEY_ID_MODE_3)
    {
        for (uint8_t i = 0; i < KEY_ID_LEN_MODE_3; i++)
        {
            lookup_data[i] = mac_parse_data_buf->key_id[i];
        }
        lookup_data_size = NINE_OCTET_LOOK_UP;
    }
    else
    {
        return MAC_UNSUPPORTED_SECURITY;    // not used by ZIP 0r 2006 security
    }

    // Get key from KeyDescriptor as 7.5.8.2.5
    retval_t status = key_descriptor_lookup(lookup_data, lookup_data_size, &curr_key_desc);
    if (status != MAC_SUCCESS)
    {
        return status;
    }
	
	/** Check the Key Usage Policy from the KeyDescriptor for the 
	  * Incoming Frame Type. The policy checking procedure  as per 
	  * 7.5.8.2.9
	  */
	status = incoming_key_usage_policy(curr_key_desc, 
									   mac_parse_data_buf->frame_type,				
									   mac_parse_data_buf->mac_command);
	if (status != MAC_SUCCESS)
    {
        return status;
    }											 
	
    *key_desc = curr_key_desc;
    /* 7.5.8.2.3 (d) */
    switch (mac_parse_data_buf->src_addr_mode)
    {
        case FCF_NO_ADDR:
            {
                if (mac_sec_pib.PANCoordShortAddress <= SHORT_ADDR_MAX && mac_sec_pib.PANCoordShortAddress >= SHORT_ADDR_MIN)
                {
                    device_lookup_data[1] = (uint8_t)mac_parse_data_buf->dest_panid;
                    device_lookup_data[0] = (uint8_t)(mac_parse_data_buf->dest_panid >> 8);
                    device_lookup_data[3] = (uint8_t)mac_sec_pib.PANCoordShortAddress;
                    device_lookup_data[2] = (uint8_t)(mac_sec_pib.PANCoordShortAddress >> 8);
                    device_lookup_data_size = LOOK_UP_SIZE_4;
                }
                else if (mac_sec_pib.PANCoordShortAddress == NO_SHORT_ADDR)
                {
                    memcpy(&device_lookup_data[0], &(mac_sec_pib.PANCoordExtendedAddress), 8);
                    device_lookup_data_size = LOOK_UP_SIZE_8;
                }
            }
            break;
        case FCF_SHORT_ADDR:
            {
                device_lookup_data[1] = (uint8_t)mac_parse_data_buf->src_panid; // LSB
                device_lookup_data[0] = (uint8_t)(mac_parse_data_buf->src_panid >> 8); // MSB
                // 2-octet dest addr right-concatenated
                device_lookup_data[3] = (uint8_t)mac_parse_data_buf->src_addr.short_address; // LSB
                device_lookup_data[2] = (uint8_t)(mac_parse_data_buf->src_addr.short_address >> 8); // MSB
                device_lookup_data_size = LOOK_UP_SIZE_4;
            }
            break;

        case FCF_LONG_ADDR:
            {
                memcpy(&device_lookup_data[0], &mac_parse_data_buf->src_addr.long_address, LOOK_UP_SIZE_8);
                device_lookup_data_size = LOOK_UP_SIZE_8;// key lookup size = 9 -> key lookup data size = 1
            }
            break;

        default:
            return MAC_UNSUPPORTED_SECURITY;
    }
    /* 7.5.8.2.3 (e) */
    status = blacklist_checking_procedure(device_lookup_data, device_lookup_data_size, curr_key_desc, &current_device_desc, &curr_key_device_desc);
    if (status != MAC_SUCCESS)
    {
        return FAILURE;
    }
    *device_desc = current_device_desc;
    *key_device_desc = curr_key_device_desc;
    return MAC_SUCCESS;
}

static retval_t key_descriptor_lookup(uint8_t *lookup_data, uint8_t lookup_data_size, mac_key_table_t **key_desc)
{
    // Get key from KeyDescriptor as 7.5.8.2.5
    for (uint8_t i = 0; i < mac_sec_pib.KeyTableEntries; i++)
    {
        for (uint8_t k = 0; k < mac_sec_pib.KeyTable[i].KeyIdLookupListEntries; k++)
        {
            if (mac_sec_pib.KeyTable[i].KeyIdLookupList[k].LookupDataSize == lookup_data_size)
            {
                int16_t compare;
                uint8_t len;

                switch (lookup_data_size)
                {
                    case FIVE_OCTET_LOOK_UP:
                        len = 5;
                        break;
                    case NINE_OCTET_LOOK_UP:
                        len = 9;
                        break;
                    default:
                        return MAC_UNSUPPORTED_SECURITY;
                }
                compare = memcmp(mac_sec_pib.KeyTable[i].KeyIdLookupList[k].LookupData,
                                 lookup_data, len);
                if (compare == 0x00)
                {
                    *key_desc = &(mac_sec_pib.KeyTable[i]);
                    return MAC_SUCCESS;
                }
            }
        }
    }
    // No key has been found. */
    return MAC_UNAVAILABLE_KEY;

}

static retval_t blacklist_checking_procedure(uint8_t *device_lookup_data, uint8_t device_lookup_data_size,
                                             mac_key_table_t *key_desc,
                                             mac_device_desc_t **device_desc,
                                             mac_key_device_desc_t **key_device_desc)
{
    for (uint8_t i = 0; i < key_desc->KeyDeviceListEntries; i++)
    {
        /* 7.5.8.2.6 (a.1) */
        uint8_t handle = key_desc->KeyDeviceList[i].DeviceDescriptorHandle;
        *key_device_desc = &(key_desc->KeyDeviceList[i]);
        *device_desc = &(mac_sec_pib.DeviceTable[handle].DeviceDescriptor[0]);
        /* 7.5.8.2.6 (a.2) */
        if (key_desc->KeyDeviceList[i].UniqueDevice == true)
        {
            if (key_desc->KeyDeviceList[i].BlackListed == false)
            {
                return MAC_SUCCESS;
            }
            else
            {
                return FAILURE;
            }
        }
        /* 7.5.8.2.6 (a.3) */
        else
        {
            retval_t status = device_descriptor_lookup(mac_sec_pib.DeviceTable[handle].DeviceDescriptor[0],
                                                       device_lookup_data,
                                                       device_lookup_data_size);
            if (status == MAC_SUCCESS && key_desc->KeyDeviceList[i].BlackListed == false)
            {
                return MAC_SUCCESS;
            }
        }
    }
    return FAILURE;
}

static retval_t device_descriptor_lookup(mac_device_desc_t device_desc, uint8_t *device_lookup_data, uint8_t device_lookup_data_size)
{
    uint8_t lookup_data[8];
    int16_t compare;

    if (device_lookup_data_size == LOOK_UP_SIZE_4)
    {
        lookup_data[1] = (uint8_t)device_desc.PANId; // LSB
        lookup_data[0] = (uint8_t)(device_desc.PANId >> 8); // MSB
        // 2-octet dest addr right-concatenated
        lookup_data[3] = (uint8_t)device_desc.ShortAddress; // LSB
        lookup_data[2] = (uint8_t)(device_desc.ShortAddress >> 8); // MSB
        compare = memcmp(device_lookup_data, lookup_data, LOOK_UP_SIZE_4);
        if (compare == 0x00)
        {
            return MAC_SUCCESS;
        }
    }
    else if (device_lookup_data_size == LOOK_UP_SIZE_8)
    {
        memcpy(&lookup_data[0], &device_desc.ExtAddress, LOOK_UP_SIZE_8);
        compare = memcmp(device_lookup_data, lookup_data, LOOK_UP_SIZE_8);
        if (compare == 0x00)
        {
            return MAC_SUCCESS;
        }
    }

    return FAILURE;
}

/** Build the Security MCPS Data Request frame from the mpdu data
  */
bool build_sec_mcps_data_frame(mcps_data_req_t *mpdr, frame_info_t *mframe)
{
	uint16_t fcf;
	bool intra_pan;
	uint8_t addr_field_len = 0;
	
	uint8_t *mpdu_ptr = &(mframe->mpdu[1]);
	
	/* Extract the FCF */
	fcf = convert_byte_array_to_16_bit(mpdu_ptr);
	fcf = CLE16_TO_CPU_ENDIAN(fcf);
	
	mpdu_ptr += (2/*FCF*/ + 1/*Seq.No*/);
	
	
	mpdr->SrcAddrMode = (fcf >> FCF_SOURCE_ADDR_OFFSET) & FCF_ADDR_MASK;
	mpdr->DstAddrMode = (fcf >> FCF_DEST_ADDR_OFFSET) & FCF_ADDR_MASK;
	intra_pan = fcf & FCF_PAN_ID_COMPRESSION;
	

	if (mpdr->DstAddrMode != 0) 
	{
		mpdr->DstPANId = convert_byte_array_to_16_bit(
				mpdu_ptr);
		mpdu_ptr += PAN_ID_LEN;
		addr_field_len += PAN_ID_LEN;
		
		mpdr->DstAddr = 0;
		if (FCF_SHORT_ADDR == mpdr->DstAddrMode) 
		{
			/*
			 * First initialize the complete long address with zero,
			 *since
			 * later only 16 bit are actually written.
			 */
			mpdr->DstAddr
				= convert_byte_array_to_16_bit(mpdu_ptr);
			mpdu_ptr += SHORT_ADDR_LEN;
			addr_field_len += SHORT_ADDR_LEN;
		} 
		else if (FCF_LONG_ADDR == mpdr->DstAddrMode) 
		{
			mpdr->DstAddr
				= convert_byte_array_to_64_bit(mpdu_ptr);
			mpdu_ptr += EXT_ADDR_LEN;
			addr_field_len += EXT_ADDR_LEN;
		}
	}
	
	if (mpdr->SrcAddrMode != 0) 
	{
		if (!intra_pan) 
		{
			/*
			 * Source PAN ID is present in the frame only if the
			 *intra-PAN bit
			 * is zero and src_addr_mode is non zero.
			 */
			//= convert_byte_array_to_16_bit(
					//frame_ptr);
			mpdu_ptr += PAN_ID_LEN;
			addr_field_len += PAN_ID_LEN;
		} 
		else 
		{
			/*
			 * The received frame does not contain a source PAN ID,
			 *hence
			 * source PAN ID is updated with the destination PAN ID.
			 */
			//src_panid = mpdr->DstPANId;
		}

		/* The source address is updated. */
		if (FCF_SHORT_ADDR == mpdr->SrcAddrMode) 
		{
			/*
			 * First initialize the complete long address with zero,
			 *since
			 * later only 16 bit are actually written.
			 */
			
				//= convert_byte_array_to_16_bit(mpdu_ptr);
			mpdu_ptr += SHORT_ADDR_LEN;

			addr_field_len += SHORT_ADDR_LEN;
		} 
		else if (FCF_LONG_ADDR == mpdr->SrcAddrMode) 
		{
			
				//= convert_byte_array_to_64_bit(mpdu_ptr);
			mpdu_ptr += EXT_ADDR_LEN;
			addr_field_len += EXT_ADDR_LEN;
		}
	}

	if (fcf & FCF_SECURITY_ENABLED)
	{
		uint8_t keyid_len;
		/* Security Fields are available so extract the aux security header */
		mpdr->SecurityLevel = (*mpdu_ptr & SEC_CTRL_SEC_LVL_MASK);	
		mpdr->KeyIdMode = ((*mpdu_ptr++ >> SEC_CTRL_KEY_ID_FIELD_POS) & SEC_CTRL_KEY_ID_MASK);
	
		memcpy(mpdu_ptr, (uint8_t *)&mac_sec_pib.FrameCounter,\
				FRAME_COUNTER_LEN);		
		mpdu_ptr += FRAME_COUNTER_LEN;
	
		keyid_len = get_key_id_field_len(mpdr->KeyIdMode);
	
		switch(keyid_len)
		{
			case KEY_ID_MODE_0:
			/** No Key source & Key Index will be available
				* Key is determined implicitly from the originator and
				* recipient(s) of the frame 
				*/
				mpdr->KeyIndex = 0;
				mpdr->KeySource = NULL;
			break;
			
			case KEY_ID_MODE_1:
			/** Key is determined from the Key Index field in
				* conjunction with macDefault-KeySource
				*/
			mpdr->KeySource = NULL;	
			mpdr->KeyIndex = *mpdu_ptr;
			mpdu_ptr += keyid_len;					
			break;
			
			case KEY_ID_MODE_2:			
			/** Key is determined explicitly from the 4-octet Key
				* Source field and the Key Index field.
				*/
			mpdr->KeySource = mpdu_ptr;
			mpdu_ptr +=(keyid_len - 1);
			mpdr->KeyIndex = *mpdu_ptr++;
			break;
			
			case KEY_ID_MODE_3:
			/** Key is determined explicitly from the 8-octet Key
				* Source field and the Key Index field.
				*/
			mpdr->KeySource = mpdu_ptr;
			mpdu_ptr +=(keyid_len - 1);
			mpdr->KeyIndex = *mpdu_ptr++;
			break;
			
			default:
			mpdr->KeyIndex = 0;
			mpdr->KeySource = NULL;
			break;
		}	
	}
   mpdr->msduHandle = mframe->msduHandle;
   
   mpdr->msduLength = mframe->mpdu[0] - (mframe->mac_payload - \
						mframe->mpdu + 1);
						
   mpdr->msdu = mframe->mac_payload;
  return MAC_SUCCESS;						
}

/** The inputs to this procedure are the KeyDescriptor, 
  * the frame type, and the command frame identifier. The
  * output from this procedure is a passed or failed status
  */
static retval_t incoming_key_usage_policy(mac_key_table_t *key_desc,
										  uint8_t frame_type, 
										  uint8_t mac_cmd)
{
 /* as per spec 7.5.8.2.9 (a.1) */
 if (FCF_FRAMETYPE_MAC_CMD != frame_type)
 {
	 for(uint8_t index = 0; index <key_desc->KeyUsageListEntries; index++)
	 {
		 if (key_desc->KeyUsageList[index].Frametype == frame_type)
		 {
		   return MAC_SUCCESS;
		 }		 
	 }	 	
 }
 else if(FCF_FRAMETYPE_MAC_CMD == frame_type)
 {
	 for(uint8_t index = 0; index <key_desc->KeyUsageListEntries; index++)
	 {
		 if ((key_desc->KeyUsageList[index].Frametype == frame_type) &&
		     (key_desc->KeyUsageList[index].CommandFrameIdentifier == mac_cmd))
		 {
		   return MAC_SUCCESS;
		 }		 
	 }		 
 }
 return MAC_IMPROPER_KEY_TYPE; 	
}


#endif  /* (defined MAC_SECURITY_ZIP) || (defined MAC_SECURITY_2006) */

/* EOF */
