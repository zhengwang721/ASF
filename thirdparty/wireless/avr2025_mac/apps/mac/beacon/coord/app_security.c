/**
 * @file app_security.c
 *
 * @brief MAC Example Beacon Security Application - Coordinator
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
 */

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the IEEE 802.15.4 MAC Beacon Security Application -
 *Coordinator
 * \section main_files Application Files
 *      - app_security.c                Application security file.
 * \section intro Application Introduction
 * The basic MAC Example Beacon Application deploys a beacon-enabled network
 *with star topology.
 * The coordinator starts a PAN at channel DEFAULT_CHANNEL with the PAN ID
 *DEFAULT_PAN_ID.
 *
 * The coordinator starts a beaconing network and transmits user data within
 *beacon payload of transmitted beacon frames.
 * The device scans for this network,sync up with the coordinator and associates
 *to the coordinator.The devices receive
 * these beacon frames, extract the received user data from the coordinator
 *,print the received data on the hyper
 * terminal and also sends the extracted beacon payload back to the coordinator.
 * The coordinator also transmits broadcast data frames periodically.The devices
 *receive these broadcast data frames
 * and increase a counter.
 * The coordinator also puts the data in the indirect queue periodically and
 *transmits data frames based on the
 * periodic poll request from the device.
 * The results of the proper data transmission/reception are printed to a
 *terminal program via Serial I/O (UART or USB).
 * For demonstration purposes the coordinator's configuration is limited to
 *associate maximum of 100 devices.
 * \note For Two Processor Approach the Application needs to be flashed in the
 *Host board(eg.SAM4L-Xplained Pro) and the Serial-if application(Beacon FFD)
 *(MAC Stack)
 * needs to be flashed in the NCP(Network CoProcessor) board.
 * \section api_modules Application Dependent Modules
 * - \subpage api
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR .
 * Other compilers may or may not work.
 *
 * \section references References
 * 1)  IEEE Std 802.15.4-2006 Part 15.4: Wireless Medium Access Control (MAC)
 *     and Physical Layer (PHY) Specifications for Low-Rate Wireless Personal
 *Area
 *     Networks (WPANs).\n\n
 * 2)  AVR Wireless Support <A href="http://avr@atmel.com">avr@atmel.com</A>.\n
 *
 * \section contactinfo Contact Information
 * For further information,visit
 * <A href="http://www.atmel.com/avr">www.atmel.com</A>.\n
 */

/* === INCLUDES ============================================================ */
#ifdef MAC_SECURITY_ZIP
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include "conf_board.h"
#include "avr2025_mac.h"
#include "delay.h"
#include "common_sw_timer.h"
#include "sio2host.h"
#include "mac.h"
#include "mac_api.h"
#include "tal.h"
#include "ieee_const.h"
#include "mac_internal.h"
#include "hw_timer.h"
#include "common_hw_timer.h"
#include "conf_hw_timer.h"
#include <asf.h>
#include "beacon_app.h"
#include "stb_generic.h"
#include "mac_security.h"


/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/** This array stores all device related information. */
extern associated_device_t device_list[MAX_NUMBER_OF_DEVICES];
/** Stores the number of associated devices. */
extern uint8_t no_of_assoc_devices;
/** Keeps the track of the no of device associated. */ 
extern uint8_t ndevice_associated;

/** Current Channel */
extern uint8_t current_channel;

/** This array stores the current beacon payload. */
extern uint8_t beacon_payload[BEACON_PAYLOAD_LEN];

/** This variable stores the current state of the node. */
extern coord_state_t coord_state;

extern uint8_t current_channel_page;
/*
 * This is implemented as an array of bytes, but actually this is a
 * 128-bit variable. This is the reason why the array needs to be filled
 * in in reverse order than expected.
 */
static uint8_t default_key[4][16] = {
{
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF
},	
{
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xD0
},
{
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xD1
},
{
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF
}
};

extern uint8_t recent_assoc_dev_no;

uint8_t default_key_source[8] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};



/* === PROTOTYPES ========================================================== */

/** Initialize the security PIB and setting the Security PIB for use by mac security */
static void init_secuity_pib(uint8_t PIBAttribute, uint8_t PIBAttributeIndex);
static void usr_mlme_set_conf_run_time(uint8_t status, uint8_t PIBAttribute, uint8_t PIBAttributeIndex);
/* === IMPLEMENTATION ====================================================== */


/*
 * @brief Callback function usr_mlme_set_conf
 *
 * @param status        Result of requested PIB attribute set operation
 * @param PIBAttribute  Updated PIB attribute
 */
static uint8_t mlme_set_conf_run_time;
void usr_mlme_set_conf(uint8_t status, uint8_t PIBAttribute, uint8_t PIBAttributeIndex)
{
    if (status != MAC_SUCCESS)
    {
        // something went wrong; restart
        wpan_mlme_reset_req(true);
    }
    else
    {
		if (mlme_set_conf_run_time)
		{
			usr_mlme_set_conf_run_time(status, PIBAttribute, PIBAttributeIndex);
			return;
		}
		else
		{
			init_secuity_pib(PIBAttribute, PIBAttributeIndex);
		}
    }

    /* Keep compiler happy. */
    PIBAttributeIndex = PIBAttributeIndex;
}




/** Initialize the security PIB and set the security parameters */
static void init_secuity_pib(uint8_t PIBAttribute, uint8_t PIBAttributeIndex)
{
	switch (PIBAttribute)
	{		
		case macDefaultKeySource:		
		{
            uint8_t mac_sec_level_table_entries = 2;

            wpan_mlme_set_req(macSecurityLevelTableEntries,
                                NO_PIB_INDEX,
                                &mac_sec_level_table_entries);
							
        }
		break;
		
		case macSecurityLevelTableEntries:
		{
            uint8_t mac_sec_level_table[4] = {	FRAME_TYPE_BEACON,      // FrameType: Beacon
                                                CMD_FRAME_ID_NA,      // CommandFrameIdentifier: N/A
												SECURITY_05_LEVEL,
                                                DEV_OVERRIDE_SEC_MIN  // DeviceOverrideSecurityMinimum: True
                                                };
												
											

            wpan_mlme_set_req(macSecurityLevelTable,
                                INDEX_1,    // Index: 1
                                &mac_sec_level_table);									
         }
		 break;
		 
		 case macSecurityLevelTable:
         {
			if(INDEX_1 == PIBAttributeIndex)
			{
				uint8_t mac_sec_level_table[4] =
				{	FRAME_TYPE_DATA,      // FrameType: Beacon
					CMD_FRAME_ID_NA,      // CommandFrameIdentifier: N/A
					SECURITY_05_LEVEL,
					DEV_OVERRIDE_SEC_MIN  // DeviceOverrideSecurityMinimum: True
				};
				wpan_mlme_set_req(macSecurityLevelTable,
				INDEX_0,    // Index: 0
				&mac_sec_level_table);
			}
			else if(INDEX_0 == PIBAttributeIndex)
			{
				uint8_t mac_key_table_entries = 4;

				wpan_mlme_set_req(macKeyTableEntries,
								NO_PIB_INDEX,
								&mac_key_table_entries);				
			}
         }
		break;
		
		case macKeyTableEntries:
		{
		  uint8_t coord_key_index = 4;
		  wpan_mlme_set_req(macDeviceTableEntries,
                          NO_PIB_INDEX,
                          &coord_key_index);	
		}        
		break;						  						  
		
		case macDeviceTableEntries:
        {
            uint8_t mac_key_table[43] =
            {
	            default_key_source[0], // LookupData[0]
	            default_key_source[1], // LookupData[1]
	            default_key_source[2], // LookupData[2]
	            default_key_source[3], // LookupData[3]
	            default_key_source[4], // LookupData[4]
	            default_key_source[5], // LookupData[5]
	            default_key_source[6], // LookupData[6]
	            default_key_source[7], // LookupData[7]
	            KEY_INDEX_0,           // LookupData[8]
	            LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
	            MAC_ZIP_MAX_KEY_ID_LOOKUP_LIST_ENTRIES, // KeyIdLookupListEntries = 1
	            // KeyDeviceList[1]
	            EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
	            true,              // UniqueDevice
	            false,              // Blacklisted
	            EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
	            true,              // UniqueDevice
	            false,              // Blacklisted
	            EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
	            true,              // UniqueDevice
	            false,              // Blacklisted
				EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
	            true,              // UniqueDevice
	            false,              // Blacklisted
	            MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES, // KeyDeviceListEntries
	            //  KeyUsageList
	            FRAME_TYPE_BEACON,    // FrameType - Beacon frames
	            CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
	            MAC_ZIP_MAX_KEY_USAGE_LIST_ENTRIES, // KeyUsageListEntries
	            // Key
	            default_key[3][0],
	            default_key[3][1],
	            default_key[3][2],
	            default_key[3][3],
	            default_key[3][4],
	            default_key[3][5],
	            default_key[3][6],
	            default_key[3][7],
	            default_key[3][8],
	            default_key[3][9],
	            default_key[3][10],
	            default_key[3][11],
	            default_key[3][12],
	            default_key[3][13],
	            default_key[3][14],
	            default_key[3][15],
            };
            wpan_mlme_set_req(macKeyTable,
            INDEX_3,    // Index: 3
            &mac_key_table);
        }
		break;
		
		case macKeyTable:
		{
					switch(PIBAttributeIndex)
					{
					case INDEX_3:
					{			
						uint8_t mac_key_table1[43] = { 
							// KeyIdLookupList[1].LookupData : macDefaultKeySource || g_Sec_KeyIndex_1
							default_key_source[0], // LookupData[0]
							default_key_source[1], // LookupData[1]
							default_key_source[2], // LookupData[2]
							default_key_source[3], // LookupData[3]
							default_key_source[4], // LookupData[4]
							default_key_source[5], // LookupData[5]
							default_key_source[6], // LookupData[6]
							default_key_source[7], // LookupData[7]
							KEY_INDEX_1,           // LookupData[8]
							LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
							MAC_ZIP_MAX_KEY_ID_LOOKUP_LIST_ENTRIES, // KeyIdLookupListEntries = 1
							// KeyDeviceList[1]
							EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
							true,              // UniqueDevice
							false,              // Blacklisted
							EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
							true,              // UniqueDevice
							false,              // Blacklisted
							EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
							true,              // UniqueDevice
							false,              // Blacklisted
							EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
							true,              // UniqueDevice
							false,              // Blacklisted
							MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES, // KeyDeviceListEntries
							//  KeyUsageList
							FRAME_TYPE_DATA,    // FrameType - Data frames
							CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
							MAC_ZIP_MAX_KEY_USAGE_LIST_ENTRIES, // KeyUsageListEntries
							// Key
							default_key[0][0],
							default_key[0][1],
							default_key[0][2],
							default_key[0][3],
							default_key[0][4],
							default_key[0][5],
							default_key[0][6],
							default_key[0][7],
							default_key[0][8],
							default_key[0][9],
							default_key[0][10],
							default_key[0][11],
							default_key[0][12],
							default_key[0][13],
							default_key[0][14],
							default_key[0][15],
						};
						wpan_mlme_set_req(macKeyTable,
						INDEX_0,    // Index: 0
						&mac_key_table1);
					}
					break;	
			
				 case INDEX_0:
				 {
						uint8_t mac_key_table2[43] = {
							// KeyIdLookupList[1].LookupData : macDefaultKeySource || g_Sec_KeyIndex_1
							default_key_source[0], // LookupData[0]
							default_key_source[1], // LookupData[1]
							default_key_source[2], // LookupData[2]
							default_key_source[3], // LookupData[3]
							default_key_source[4], // LookupData[4]
							default_key_source[5], // LookupData[5]
							default_key_source[6], // LookupData[6]
							default_key_source[7], // LookupData[7]
							KEY_INDEX_2,           // LookupData[8]
							LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
							MAC_ZIP_MAX_KEY_ID_LOOKUP_LIST_ENTRIES, // KeyIdLookupListEntries = 1
							// KeyDeviceList[1]
							EMPTY_DEV_HANDLE, // DeviceDescriptorHandle
							true,              // UniqueDevice
							false,              // Blacklisted
							EMPTY_DEV_HANDLE, // DeviceDescriptorHandle
							true,              // UniqueDevice
							false,              // Blacklisted
							EMPTY_DEV_HANDLE, // DeviceDescriptorHandle
							true,              // UniqueDevice
							false,              // Blacklisted
							EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
							true,              // UniqueDevice
							false,              // Blacklisted
							MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES, // KeyDeviceListEntries
							//  KeyUsageList
							FRAME_TYPE_DATA,    // FrameType - Data frames
							CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
							MAC_ZIP_MAX_KEY_USAGE_LIST_ENTRIES, // KeyUsageListEntries
							// Key
							default_key[1][0],
							default_key[1][1],
							default_key[1][2],
							default_key[1][3],
							default_key[1][4],
							default_key[1][5],
							default_key[1][6],
							default_key[1][7],
							default_key[1][8],
							default_key[1][9],
							default_key[1][10],
							default_key[1][11],
							default_key[1][12],
							default_key[1][13],
							default_key[1][14],
							default_key[1][15],
						};
				
						wpan_mlme_set_req(macKeyTable,
						INDEX_1,    // Index: 1
						&mac_key_table2);
					  }
					  break;
			
					case INDEX_1:
					{  
					  uint8_t mac_key_table3[43] = {
						// KeyIdLookupList[1].LookupData : macDefaultKeySource || g_Sec_KeyIndex_1
						default_key_source[0], // LookupData[0]
						default_key_source[1], // LookupData[1]
						default_key_source[2], // LookupData[2]
						default_key_source[3], // LookupData[3]
						default_key_source[4], // LookupData[4]
						default_key_source[5], // LookupData[5]
						default_key_source[6], // LookupData[6]
						default_key_source[7], // LookupData[7]
						KEY_INDEX_3,           // LookupData[8]
						LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
						MAC_ZIP_MAX_KEY_ID_LOOKUP_LIST_ENTRIES, // KeyIdLookupListEntries = 1
						// KeyDeviceList[1]
						EMPTY_DEV_HANDLE, // DeviceDescriptorHandle
						true,              // UniqueDevice
						false,              // Blacklisted
						EMPTY_DEV_HANDLE, // DeviceDescriptorHandle
						true,              // UniqueDevice
						false,              // Blacklisted
						EMPTY_DEV_HANDLE, // DeviceDescriptorHandle
						true,              // UniqueDevice
						false,              // Blacklisted
						EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
						true,              // UniqueDevice
						false,              // Blacklisted
						MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES, // KeyDeviceListEntries
						//  KeyUsageList
						FRAME_TYPE_DATA,    // FrameType - Data frames
						CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
						MAC_ZIP_MAX_KEY_USAGE_LIST_ENTRIES, // KeyUsageListEntries
						// Key
						default_key[2][0],
						default_key[2][1],
						default_key[2][2],
						default_key[2][3],
						default_key[2][4],
						default_key[2][5],
						default_key[2][6],
						default_key[2][7],
						default_key[2][8],
						default_key[2][9],
						default_key[2][10],
						default_key[2][11],
						default_key[2][12],
						default_key[2][13],
						default_key[2][14],
						default_key[2][15],
					};		
				wpan_mlme_set_req(macKeyTable,
								INDEX_2,    // Index: 2
								&mac_key_table3);
				}
				break;
				
				case INDEX_2:
				{
					uint8_t coord_dev_index = 3;
					uint8_t mac_dev_table[17];
			
					mac_dev_table[0] = (uint8_t)DEFAULT_PAN_ID;
					mac_dev_table[1] = (uint8_t)(DEFAULT_PAN_ID >> 8);
					mac_dev_table[2] = (uint8_t)COORD_SHORT_ADDR;
					mac_dev_table[3] = (uint8_t)(COORD_SHORT_ADDR >> 8);
					mac_dev_table[4] = (uint8_t)tal_pib.IeeeAddress;
					mac_dev_table[5] = (uint8_t)(tal_pib.IeeeAddress >> 8);
					mac_dev_table[6] = (uint8_t)(tal_pib.IeeeAddress >> 16);
					mac_dev_table[7] = (uint8_t)(tal_pib.IeeeAddress >> 24);
					mac_dev_table[8] = (uint8_t)(tal_pib.IeeeAddress >> 32);
					mac_dev_table[9] = (uint8_t)(tal_pib.IeeeAddress >> 40);
					mac_dev_table[10] = (uint8_t)(tal_pib.IeeeAddress >> 48);
					mac_dev_table[11] = (uint8_t)(tal_pib.IeeeAddress >> 56);
					mac_dev_table[12] = 0;  // Frame counter
					mac_dev_table[13] = 0;
					mac_dev_table[14] = 0;
					mac_dev_table[15] = 0;
					mac_dev_table[16] = 0;  // Exempt
			
					wpan_mlme_set_req(macDeviceTable,
										coord_dev_index,    // Index
										&mac_dev_table);
				}
				break;
		
				default:
				break;
			  }
		}
		break;
		
		case macDeviceTable:
		{
			uint8_t pan_coord_add[8];
			pan_coord_add[0] = (uint8_t)tal_pib.IeeeAddress;
			pan_coord_add[1] = (uint8_t)(tal_pib.IeeeAddress >> 8);
			pan_coord_add[2] = (uint8_t)(tal_pib.IeeeAddress >> 16);
			pan_coord_add[3] = (uint8_t)(tal_pib.IeeeAddress >> 24);
			pan_coord_add[4] = (uint8_t)(tal_pib.IeeeAddress >> 32);
			pan_coord_add[5] = (uint8_t)(tal_pib.IeeeAddress >> 40);
			pan_coord_add[6] = (uint8_t)(tal_pib.IeeeAddress >> 48);
			pan_coord_add[7] = (uint8_t)(tal_pib.IeeeAddress >> 56);
			
			wpan_mlme_set_req(macPANCoordExtendedAddress,
										NO_PIB_INDEX,    // Index
										pan_coord_add);
		}
		break;
		
		case macPANCoordExtendedAddress:
		{
			uint16_t short_add = COORD_SHORT_ADDR;
			wpan_mlme_set_req(macPANCoordShortAddress,
										NO_PIB_INDEX,    // Index
										&short_add);
		}
		break;
		
		case macPANCoordShortAddress:		
		{
			uint8_t short_addr[2];

			short_addr[0] = (uint8_t)COORD_SHORT_ADDR;  /* low byte */
			short_addr[1] = (uint8_t)(COORD_SHORT_ADDR >> 8); /* high byte */

			wpan_mlme_set_req(macShortAddress, 
								NO_PIB_INDEX, 
								short_addr);
			mlme_set_conf_run_time = true;
		}
		break;
		
		default:
		break;
	 }

}

/*
 * @brief Callback function usr_mlme_set_conf
 *
 * @param status        Result of requested PIB attribute set operation
 * @param PIBAttribute  Updated PIB attribute
 */
static void usr_mlme_set_conf_run_time(uint8_t status, uint8_t PIBAttribute, uint8_t PIBAttributeIndex)
{
    if (status != MAC_SUCCESS)
    {
        // something went wrong; restart
        wpan_mlme_reset_req(true);
    }
    else
    {
        switch (PIBAttribute)
        {
            case macShortAddress:
                {
                    uint8_t association_permit = true;

                    wpan_mlme_set_req(macAssociationPermit, NO_PIB_INDEX, &association_permit);
                }
                break;

            case macAssociationPermit:
                {
                    bool rx_on_when_idle = true;

                    wpan_mlme_set_req(macRxOnWhenIdle, NO_PIB_INDEX, &rx_on_when_idle);
                }
                break;

            case macRxOnWhenIdle:
                {
					
					/* Set the beacon payload length. */
					uint8_t beacon_payload_len = BEACON_PAYLOAD_LEN;
					wpan_mlme_set_req(macBeaconPayloadLength,NO_PIB_INDEX, &beacon_payload_len);
                }
                break;

            case macBeaconPayloadLength:
				{
					/*
					 * Once the length of the beacon payload has been defined,
					 * set the actual beacon payload.
					 */
					wpan_mlme_set_req(macBeaconPayload,NO_PIB_INDEX, &beacon_payload);
				}
				break;
			case macBeaconPayload:
				{
					if (COORD_STARTING == coord_state) 
					{
					/*
					 * Initiate an active scan over all channels to
					 *determine
					 * which channel to use.
					 * Use: bool wpan_mlme_scan_req(uint8_t ScanType,
					 *                              uint32_t ScanChannels,
					 *                              uint8_t ScanDuration,
					 *                              uint8_t ChannelPage);
					 *
					 * This request leads to a scan confirm message ->
					 *usr_mlme_scan_conf
					 * Scan for about 50 ms on each channel -> ScanDuration
					 *= 1
					 * Scan for about 1/2 second on each channel ->
					 *ScanDuration = 5
					 * Scan for about 1 second on each channel ->
					 *ScanDuration = 6
					 */
					wpan_mlme_scan_req(MLME_SCAN_TYPE_ACTIVE,
							SCAN_CHANNEL(current_channel),
							SCAN_DURATION_COORDINATOR,
							current_channel_page);
					}
				}	
				break;
				
			case macDefaultKeySource:
                {
                    uint8_t mac_sec_level_table_entries = 2;

                    wpan_mlme_set_req(macSecurityLevelTableEntries,
                                      NO_PIB_INDEX,
                                      &mac_sec_level_table_entries);
                }
                break;

            case macSecurityLevelTableEntries:
                {
                    uint8_t mac_sec_level_table[4] = {FRAME_TYPE_DATA,      // FrameType: Data
                                                      CMD_FRAME_ID_NA,      // CommandFrameIdentifier: N/A
                                                     // ZIP_SEC_MIN,          // SecurityMinimum: 5
													  0X05,
                                                      DEV_OVERRIDE_SEC_MIN  // DeviceOverrideSecurityMinimum: True
                                                     };

                    wpan_mlme_set_req(macSecurityLevelTable,
                                      INDEX_0,    // Index: 0
                                      &mac_sec_level_table);
                }
                break;

            case macSecurityLevelTable:
                {
                    uint8_t mac_key_table_entries = 4;

                    wpan_mlme_set_req(macKeyTableEntries,
                                      NO_PIB_INDEX,
                                      &mac_key_table_entries);
                }
                break;

            case macKeyTableEntries:
                {
					uint8_t mac_key_table[43] =
					{
						default_key_source[0], // LookupData[0]
						default_key_source[1], // LookupData[1]
						default_key_source[2], // LookupData[2]
						default_key_source[3], // LookupData[3]
						default_key_source[4], // LookupData[4]
						default_key_source[5], // LookupData[5]
						default_key_source[6], // LookupData[6]
						default_key_source[7], // LookupData[7]
						KEY_INDEX_0,           // LookupData[8]
						LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
						MAC_ZIP_MAX_KEY_ID_LOOKUP_LIST_ENTRIES, // KeyIdLookupListEntries = 1
						// KeyDeviceList[1]
						EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
						true,              // UniqueDevice
						false,              // Blacklisted
						EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
						true,              // UniqueDevice
						false,              // Blacklisted
						EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
						true,              // UniqueDevice
						false,              // Blacklisted
						EMPTY_DEV_HANDLE,           // DeviceDescriptorHandle
						true,              // UniqueDevice
						false,              // Blacklisted
						MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES, // KeyDeviceListEntries
						//  KeyUsageList
						FRAME_TYPE_BEACON,    // FrameType - Beacon frames
						CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
						MAC_ZIP_MAX_KEY_USAGE_LIST_ENTRIES, // KeyUsageListEntries
						// Key
						default_key[3][0],
						default_key[3][1],
						default_key[3][2],
						default_key[3][3],
						default_key[3][4],
						default_key[3][5],
						default_key[3][6],
						default_key[3][7],
						default_key[3][8],
						default_key[3][9],
						default_key[3][10],
						default_key[3][11],
						default_key[3][12],
						default_key[3][13],
						default_key[3][14],
						default_key[3][15],
					};
					wpan_mlme_set_req(macKeyTable,
					INDEX_3,    // Index: 3
					&mac_key_table);
                }
                break;

            case macKeyTable:
                /*
                 * Setting of PIB attributes will continue once a device
                 * has associated successful.
                 */
                break;

            case macDeviceTableEntries:
                {
	                    static uint8_t Temp = 0;
	                    uint8_t mac_dev_table[17];
	                    for (uint8_t i = Temp; i < no_of_assoc_devices; i++) // Temp is used to not update the already device table again
	                    {
		                    mac_dev_table[0] = (uint8_t)tal_pib.PANId;
		                    mac_dev_table[1] = (uint8_t)(tal_pib.PANId >> 8);
		                    mac_dev_table[2] = (uint8_t)device_list[i].short_addr;
		                    mac_dev_table[3] = (uint8_t)(device_list[i].short_addr >> 8);
		                    mac_dev_table[4] = (uint8_t)device_list[i].ieee_addr;
		                    mac_dev_table[5] = (uint8_t)(device_list[i].ieee_addr >> 8);
		                    mac_dev_table[6] = (uint8_t)(device_list[i].ieee_addr >> 16);
		                    mac_dev_table[7] = (uint8_t)(device_list[i].ieee_addr >> 24);
		                    mac_dev_table[8] = (uint8_t)(device_list[i].ieee_addr >> 32);
		                    mac_dev_table[9] = (uint8_t)(device_list[i].ieee_addr >> 40);
		                    mac_dev_table[10] = (uint8_t)(device_list[i].ieee_addr >> 48);
		                    mac_dev_table[11] = (uint8_t)(device_list[i].ieee_addr >> 56);
		                    mac_dev_table[12] = 0;  // Frame counter
		                    mac_dev_table[13] = 0;
		                    mac_dev_table[14] = 0;
		                    mac_dev_table[15] = 0;
		                    mac_dev_table[16] = 0;  // Exempt

		                    wpan_mlme_set_req(macDeviceTable,
		                    i,    // Index
		                    &mac_dev_table);
	                    }
	                    Temp++;
                }
                break;

            case macDeviceTable:
                {
                    /* Use DSN start value as in ZIP test spec. */
                    uint8_t new_dsn = 0x0;

                    wpan_mlme_set_req(macDSN,
                                      NO_PIB_INDEX,
                                      &new_dsn);
                }
                break;

            case macDSN:
                {
                    /* Use DSN start value as in ZIP test spec. */
                    uint32_t frame_counter = 1;

                    wpan_mlme_set_req(macFrameCounter,
                                      NO_PIB_INDEX,
                                      &frame_counter);
                }
                break;

            case macFrameCounter:
                break;

            default:
                // undesired PIB attribute; restart
                wpan_mlme_reset_req(true);
                break;
		}
    }

    /* Keep compiler happy. */
    PIBAttributeIndex = PIBAttributeIndex;
}
#endif

