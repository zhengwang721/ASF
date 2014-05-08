/**
* \file
*
* \brief PRIME Header
*
* Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

#ifndef PRIME_H_INCLUDE
#define PRIME_H_INCLUDE

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup prime_group PRIME
 *
 * This module provides configuration and utils for PRIME.
 */

/**
 * \ingroup prime_group
 * \defgroup mac_prime_group PRIME MAC Layer
 *
 * This module provides configuration and utils for the
 * MAC layer in PRIME.
 *
 * @{
 */

enum MacLnidValues
{
	PRIME_BROADCAST_LNID = 0x3FFF,
	PRIME_MULTICAST_LNID = 0x3FFE,
};

//! \name Length in bytes of EUI48 identifier of a Node
//@{
#define PRIME_MAC_LENGTH                         6
//@}

//! \name Length in bytes of max pdu
//@{
#define MAX_PDU_SIZE                             364
//@}

//! \name Time interval [msec] to check unexpected PLC chip resets
//@{
#define PRIME_CHECK_PLC_CFG_TIMER                5000
//@}

//! \name PRIME main control interface
//@{
void prime_Init (void);
void prime_Start (void);
void prime_Process (void);
void prime_Upd1msInt (void);
//void prime_Upd618msInt (void);
//@}

//! \name PLME Result values
//@{
#define PRIME_PLME_RESULT_DONE                   0 ///< Inherited from MLME Result
#define PRIME_PLME_RESULT_SUCCESS                0 ///< Inherited from MLME Result
#define PRIME_PLME_RESULT_FAILED                 1 ///< Inherited from MLME Result
#define PRIME_PLME_RESULT_REJECTED               2 ///< Inherited from MLME Result
#define PRIME_PLME_RESULT_BADATTR                3 ///< Inherited from MLME Result
#define PRIME_PLME_RESULT_OUTOFRANGE             4 ///< Inherited from MLME Result
#define PRIME_PLME_RESULT_READONLY               5 ///< Inherited from MLME Result
//@}

//! \name PLME Commands for callback messages
//@{
#define PRIME_PLME_RESET_CONFIRM                 1 ///< Command obtained from Callback when PLME has used Reset.Confirm primitive
#define PRIME_PLME_SLEEP_CONFIRM                 2 ///< Command obtained from Callback when PLME has used Sleep.Confirm primitive
#define PRIME_PLME_RESUME_CONFIRM                3 ///< Command obtained from Callback when PLME has used Resume.Confirm primitive
#define PRIME_PLME_TESTMODE_CONFIRM              4 ///< Command obtained from Callback when PLME has used TestMode.Confirm primitive
#define PRIME_PLME_GET_CONFIRM                   5 ///< Command obtained from Callback when PLME has used Get.Confirm primitive
#define PRIME_PLME_SET_CONFIRM                   6 ///< Command obtained from Callback when PLME has used Set.Confirm primitive
//@}

//! \name PLME Commands for request messages 
//@{
#define PRIME_PLME_RESET_REQUEST                 20
#define PRIME_PLME_SLEEP_REQUEST                 21
#define PRIME_PLME_RESUME_REQUEST                22
#define PRIME_PLME_TESTMODE_REQUEST              23
#define PRIME_PLME_GET_REQUEST                   24
#define PRIME_PLME_SET_REQUEST                   25
//@}

// Types for PLME information access 

//! Extract PLME command
typedef struct
{
	uint8_t cmd;                 ///< Message command
} PlmeCommand;

//! Extract PLME result of operation
typedef struct
{
	uint8_t cmd;                 ///< Message command
	uint8_t result;              ///< Operation result
} PlmeResult;

//! Extract PLME numeric value
typedef struct
{
	uint8_t cmd;                 ///< Message command
	uint8_t result;              ///< Operation result
	uint16_t pibAttrib;          ///< Variable attribute
	uint32_t pibValue;           ///< Variable value
} PlmeGetValue;

//! Extract PLME array value
typedef struct
{
	uint8_t cmd;                 ///< Message command
	uint8_t result;              ///< Operation result
	uint16_t pibAttrib;          ///< Variable attribute
	uint8_t buf[1];              ///< Array data (context dependent)
} PlmeGetArray;

//! Extract PLME SET result
typedef struct
{
	uint8_t cmd;                 ///< Message command
	uint8_t result;              ///< Operation result
	uint16_t pibAttrib;          ///< Variable attribute
} PlmeSetResult;

//! \name Buffer size needed to receive PLME data
//@{
#define PRIME_PLME_DATA_SIZE                     364
//@}

//! \name PLME Functions interface
//@{
int prime_PLME_RESET_request (void);
int prime_PLME_SLEEP_request (void);
int prime_PLME_RESUME_request (void);
int prime_PLME_TESTMODE_request (uint8_t enable, uint8_t mode, uint8_t modulation, uint8_t pwr_level);
int prime_PLME_GET_request (uint16_t pibAttribute);
int prime_PLME_SET_request (uint16_t pibAttribute, uint32_t pibAttributeValue);
int prime_PLME_SET_BUF_request (uint16_t pibAttribute, uint8_t * pibBuf, uint16_t pibLen);
int prime_PLME_callback (uint8_t *buf);
//@}

//! \name MLME Result values
//@{
#define PRIME_MLME_RESULT_DONE                   0 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_SUCCESS                0 ///< Equivalent to PRIME_MLME_RESULT_DONE
#define PRIME_MLME_RESULT_FAILED                 1 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_REJECTED               1 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_TIMEOUT                2 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_NOSNA                  8 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_NOSWITCH               9 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_REDUNDANT              10 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_BADATTR                11 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_OUTOFRANGE             12 ///< As defined in PRIME Spec v1.3
#define PRIME_MLME_RESULT_READONLY               13 ///< As defined in PRIME Spec v1.3
//@}

//! \name MLME Commands for callback messages
//@{
#define PRIME_MLME_REGISTER_CONFIRM              1 ///< Command obtained from Callback when MLME has used Register.Confirm primitive
#define PRIME_MLME_REGISTER_INDICATION           2 ///< Command obtained from Callback when MLME has used Register.Indication primitive
#define PRIME_MLME_UNREGISTER_CONFIRM            3 ///< Command obtained from Callback when MLME has used Unregister.Confirm primitive
#define PRIME_MLME_UNREGISTER_INDICATION         4 ///< Command obtained from Callback when MLME has used Unregister.Indication primitive
#define PRIME_MLME_PROMOTE_CONFIRM               5 ///< Command obtained from Callback when MLME has used Promote.Confirm primitive
#define PRIME_MLME_PROMOTE_INDICATION            6 ///< Command obtained from Callback when MLME has used Promote.Indication primitive
#define PRIME_MLME_DEMOTE_CONFIRM                7 ///< Command obtained from Callback when MLME has used Demote.Confirm primitive
#define PRIME_MLME_DEMOTE_INDICATION             8 ///< Command obtained from Callback when MLME has used Demote.Indication primitive
#define PRIME_MLME_RESET_CONFIRM                 9 ///< Command obtained from Callback when MLME has used Reset.Confirm primitive
#define PRIME_MLME_GET_CONFIRM                   10 ///< Command obtained from Callback when MLME has used Get.Confirm primitive
#define PRIME_MLME_LIST_GET_CONFIRM              11 ///< Command obtained from Callback when MLME has used ListGet.Confirm primitive
#define PRIME_MLME_SET_CONFIRM                   12 ///< Command obtained from Callback when MLME has used Set.Confirm primitive
//@}

//! \name MLME Commands for request messages 
//@{
#define PRIME_MLME_REGISTER_REQUEST              20
#define PRIME_MLME_UNREGISTER_REQUEST_SERVICE    21
#define PRIME_MLME_PROMOTE_REQUEST_SERVICE       22
#define PRIME_MLME_DEMOTE_REQUEST_SERVICE        23
#define PRIME_MLME_RESET_REQUEST                 24
#define PRIME_MLME_GET_REQUEST                   25
#define PRIME_MLME_LIST_GET_REQUEST              26
#define PRIME_MLME_SET_REQUEST                   27
#define PRIME_MLME_UNREGISTER_REQUEST_BASE       29
#define PRIME_MLME_PROMOTE_REQUEST_BASE          30
#define PRIME_MLME_DEMOTE_REQUEST_BASE           31
//@}

//! \name MLME definitions
//@{
//! Buffer size needed to receive MLME data
#define MLME_DATA_SIZE                           364

//! Number of registered devices per message 
#define MLME_LIST_REGDEVNUM                      15

//! Number of active connections per message 
#define MLME_LIST_ACTCONNUM                      22

//! Number of multicast entries per message 
#define MLME_LIST_MCASTNUM                       83

//! Number of Switch Nodes entries per message 
#define MLME_LIST_SWNODENUM                      64

//! Number of direct connections per message 
#define MLME_LIST_DIRCONNUM                      8

//! Number of direct connections per message 
#define MLME_LIST_DIRSWNODENUM                   15

//! Number of Available Switch Nodes entries per message
#define MLME_LIST_AVAILSWNODENUM                 22

//! Number of Switch Nodes entries per message
#define MLME_LIST_PHYCOMMNUM                     17

//! Number of active connections per message 
#define MLME_LIST_ACTCONNUM_EX                   20

//! Number of 432 Nodes per message 
#define MLME_LIST_432_NODES                      7

//! Maximum number of extended SNR elements per message 
#define MLME_LIST_EXSNR_DEVNUM                   80

//! Number of FU Nodes per message
#define MLME_LIST_FU_NODES                       20
//@}

//! PLC states
enum MacPlcStateValues
{
	MAC_PLC_STATE_DISCONNECTED           = 0,
	MAC_PLC_STATE_DETECTION              = 1,
	MAC_PLC_STATE_REGISTERING            = 2,
	MAC_PLC_STATE_OPERATIVE              = 3,
	MAC_PLC_STATE_BASE                   = 4,
};


// Types for MLME information access 

//! Extract MLME command
typedef struct
{
	uint8_t cmd;                                 ///< Message command
} MlmeCommand;

//! Extract MLME result of operation
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
} MlmeResult;

//! Extract MLME numeric value
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint32_t pibValue;                           ///< Variable value
} MlmeGetValue;

//! Extract MLME array value
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t buf[1];                              ///< Array data (context dependent)
} MlmeGetArray;

//! Extract MLME SET result
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
} MlmeSetResult;

//! Extract MLME results
typedef struct
{
   uint8_t headerType;                           ///< MAC Header Type
   uint8_t ctrlPktFlag;                          ///< Control Packet identifier
   uint16_t lcid;                                ///< Local connection identifier
   uint8_t type;                                 ///< Type of connection
} MlmeParam;

//! Value for MLME Register Confirm command
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint8_t sna[PRIME_MAC_LENGTH];               ///< SNA value
	uint8_t mac[PRIME_MAC_LENGTH];               ///< MAC in format EUI48
} MlmeRegisterConfirm;

//! Value for MLME Register Indication command
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t sna[PRIME_MAC_LENGTH];               ///< SNA value
	uint8_t mac[PRIME_MAC_LENGTH];               ///< MAC in format EUI48
} MlmeRegisterIndication;

//! Header for MLME ListGet commands
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
} MlmeListGetHeader;

//! Information of Registered Device
typedef struct
{
	uint8_t mac[PRIME_MAC_LENGTH];               ///< MAC in format EUI48
	uint8_t state;                               ///< Status of Node
	uint8_t level;                               ///< Hierarchy level
	uint8_t tCap;                                ///< Terminal Capabilities
	uint8_t swCap;                               ///< Switch Capabilities
	uint16_t lnid;                               ///< Local network Id
	uint16_t lsid;                               ///< Local switch Id
	uint16_t sid;                                ///< Switch of register
} MlmeRegDevice;

//! ListGet for Registered	Devices
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmeRegDevice dev[MLME_LIST_REGDEVNUM];      ///< Registered device information
} MlmeListGetRegDevices;

//! Information of Active Connections
typedef struct
{
	uint8_t mac[PRIME_MAC_LENGTH];               ///< MAC in format EUI48 of service node
	uint16_t sid;                                ///< Switch of register
	uint16_t lnid;                               ///< Local network Id
	uint8_t lcid;                                ///< Local connection ID
} MlmeActiveConn;

//! ListGet for Active Connections
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmeActiveConn conn[MLME_LIST_ACTCONNUM];    ///< Active Connection information
} MlmeListGetActiveConn;

//! Information of Active Connections (Extended List)
typedef struct
{
	uint8_t mac[PRIME_MAC_LENGTH];               ///< MAC in format EUI48 of service node
	uint16_t sid;                                ///< Switch of register
	uint16_t lnid;                               ///< Local network Id
	uint16_t lcid;							///< Local connection ID
	uint8_t type;                                ///< Connection Type
} MlmeActiveConnEx;

//! ListGet for Active Connections  (Extended List)
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmeActiveConnEx conn[MLME_LIST_ACTCONNUM_EX]; ///< Active Connection information
} MlmeListGetActiveConnEx;

//! Information of Multicast Entries
typedef struct
{
	uint16_t member;                             ///< Number of child nodes
	uint8_t lcid;                                ///< Local connection ID
} MlmeMcastEntrie;

//! ListGet for Multicast Entries
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmeMcastEntrie ent[MLME_LIST_MCASTNUM];     ///< Multicast entries information
} MlmeListGetMcastEntries;

//! Information of Switch Table
typedef struct
{
	uint16_t sid;                                ///< SID of attached Switch Node
} MlmeSwitchNode;

//! ListGet for Switch Table
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmeSwitchNode nod[MLME_LIST_SWNODENUM];     ///< Switch table info
} MlmeListGetSwitchTable;

//! Information of Direct Connections
typedef struct
{
	uint8_t srcmac[PRIME_MAC_LENGTH];            ///< MAC in format EUI48 of Source service node
	uint16_t srcsid;                             ///< Switch through which Source Node is registered through
	uint16_t srclnid;                            ///< Local network Id of Source Node
	uint8_t srclcid;                             ///< Local connection ID allocated on Source Node
	uint8_t dstmac[PRIME_MAC_LENGTH];            ///< MAC in format EUI48 of Destination service node
	uint16_t dstsid;                             ///< Switch through which Destination Node is registered through
	uint16_t dstlnid;                            ///< Local network Id of Destination Node
	uint8_t dstlcid;                             ///< Local connection ID allocated on Destination Node
	uint8_t dmac[PRIME_MAC_LENGTH];              ///< MAC in format EUI48 of Direct Switch
	uint16_t dsid;                               ///< SID of Direct Switch
} MlmeDirectConn;

//! ListGet for Direct Connections
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmeDirectConn conn[MLME_LIST_DIRCONNUM];    ///< Direct Connection information
} MlmeListGetDirectConn;

//! Information of Direct Switch Table
typedef struct
{
	uint8_t dsmac[PRIME_MAC_LENGTH];             ///< MAC in format EUI48 of Direct Switch
	uint16_t srcsid;                             ///< Switch through which Source Node is registered through
	uint16_t srclnid;                            ///< Local network Id of Source Node
	uint8_t srclcid;                             ///< Local connection ID allocated on Source Node
	uint16_t dstsid;                             ///< Switch through which Destination Node is registered through
	uint16_t dstlnid;                            ///< Local network Id of Destination Node
	uint8_t dstlcid;                             ///< Local connection ID allocated on Destination Node
} MlmeDirectSwitchNode;

//! ListGet for Direct Switch Table
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmeDirectSwitchNode nod[MLME_LIST_DIRSWNODENUM]; ///< Switch table info
} MlmeListGetDirectSwitchTable;

//! Information of Available Switch Table
typedef struct
{
	uint8_t sna[PRIME_MAC_LENGTH];               ///< MAC in format EUI48
	uint16_t sid;                                ///< SID of available Switch Node
	uint8_t level;                               ///< Hierarchy level of available Switch Node
	uint8_t rxLevel;                             ///< Rx level of available Switch Node
	uint8_t rxSNR;                               ///< Rx SNR of available Switch Node
} MlmeAvailableSwitchNode;

//! ListGet for Available Switch Table
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmeAvailableSwitchNode nod[MLME_LIST_AVAILSWNODENUM]; ///< Switch table info
} MlmeListGetAvailableSwitchTable;

//! Information of Phy Communication Parameters Table
typedef struct
{
	uint8_t eui48[PRIME_MAC_LENGTH];             ///< EUI48 of the other device
	uint8_t txPower;                             ///< Tx power of GPDU packets send to device
	uint8_t txCoding;                            ///< Tx coding of GPDU packets send to device
	uint8_t rxCoding;                            ///< Rx coding of GPDU packets received from device
	uint8_t rxLevel;                             ///< Rx power level of GPDU packets received from device
	uint8_t snr;                                 ///< SNR of GPDU packets received from device
	uint8_t txPowerMod;                          ///< Number of times the Tx power was modified
	uint8_t txCodingMod;                         ///< Number of times the Tx coding was modified
	uint8_t rxCodingMod;                         ///< Number of times the Rx coding was modified
} MlmePhyCommDevice;

//! ListGet for Phy Communication Parameters Table
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	MlmePhyCommDevice dev[MLME_LIST_PHYCOMMNUM]; ///< Phy Comm table info
} MlmeListGetPhyCommTable;

//! List of extended SNR (PIB 0x8150)
typedef struct {
	uint8_t sid;
	uint16_t lnid;
	uint8_t exSnr;
} ExSNRElementType;

//! ListGet for Phy Communication Parameters Table
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	ExSNRElementType exSnrDev[MLME_LIST_EXSNR_DEVNUM]; ///< extended SNR table info
} MlmeListGetExSnrTable;

//! Equal to LEN_432_SERIAL_NUMBER defined in 432 layer 
#define SERIAL_NUMBER_432_MAC                    16

//! List of 432 Information Nodes if exists 432 (PIB 0x8250)
typedef struct {
	uint16_t address;
	int8_t serialNumber[SERIAL_NUMBER_432_MAC];
	uint8_t lenSerial;
	uint8_t mac[PRIME_MAC_LENGTH];
	uint8_t arq;
} Node432Type;

//! ListGet for Phy Communication Parameters Table
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	Node432Type dev[MLME_LIST_432_NODES];        ///< Phy Comm table info
} MlmeListGetNode432Table;

//! List of Nodes in the FU target list (PIB 0x8350)
typedef struct {
	uint8_t fuState;
	uint32_t pagesReceived;
	uint8_t mac[PRIME_MAC_LENGTH];
} FuNodeTargetType;

//! ListGet for FU target list
typedef struct
{
	uint8_t cmd;                                 ///< Message command
	uint8_t result;                              ///< Operation result
	uint16_t pibAttrib;                          ///< Variable attribute
	uint8_t isLast;                              ///< true in last block
	uint8_t numItems;                            ///< Number of items in block
	FuNodeTargetType dev[MLME_LIST_FU_NODES];    ///< Fu node info
} MlmeListFUTable;

//! \name MLME Functions interface
//@{
int prime_MLME_REGISTER_request (uint8_t *sna, uint8_t *mac);
int prime_MLME_UNREGISTER_request_base (uint8_t *mac);
int prime_MLME_UNREGISTER_request_service (void);
int prime_MLME_PROMOTE_request_base (uint8_t *mac);
int prime_MLME_PROMOTE_request_service (void);
int prime_MLME_DEMOTE_request_base (uint8_t *mac);
int prime_MLME_DEMOTE_request_service (void);
int prime_MLME_RESET_request (void);
int prime_MLME_GET_request (uint16_t pibAttribute);
int prime_MLME_LIST_GET_request (uint16_t pibListAttribute);
int prime_MLME_SET_request (uint16_t pibAttribute, uint32_t pibAttributeValue);
int prime_MLME_SET_BUF_request (uint16_t pibAttribute, uint8_t * pibBuf, uint8_t pibLen);
int prime_MLME_callback (uint8_t *buf);
//@}

//! \name MAC_SAP Result values
//@{
#define PRIME_MACSAP_RESULT_SUCCESS              0 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_ACCEPT               0 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_ERROR                1 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_REJECT               1 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_TIMEOUT              2 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_NO_BANDWITH          3 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_NO_SUCH_DEVICE       4 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_REDIRECT_FAILED      5 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_NOT_REGISTERED       6 ///< As defined in PRIME Spec v1.3
#define PRIME_MACSAP_RESULT_NO_MORE_LCIDS        7 ///< As defined in PRIME Spec v1.3
//@}

//! \name MAC_SAP Error Type values
//@{
#define PRIME_MACSAP_ERROR_NO_ERROR              0 ///< No Error found
#define PRIME_MACSAP_ERROR_REJECT                1 ///< Request was Rejected by the other end
#define PRIME_MACSAP_ERROR_TIMEOUT               2 ///< Message could not be sent or no response from other end
#define PRIME_MACSAP_ERROR_NO_SUCH_DEVICE        4 ///< No device with indicated EUI48 is present on the network
#define PRIME_MACSAP_ERROR_NOT_REGISTERED        6 ///< Node is not yet registered, or has been unregistered
#define PRIME_MACSAP_ERROR_INVALID_HANDLER       8 ///< Handler does not address a valid connection
#define PRIME_MACSAP_ERROR_NO_RESOURCES          9 ///< No room available for request (for example Table Full, or resources in use by previous request)
#define PRIME_MACSAP_ERROR_PROCESS_RUNNING       10 ///< Process already running for connection or connection in a transitory state
#define PRIME_MACSAP_ERROR_TOO_LONG              11 ///< Data packet too long to be sent using currently available resources (may be sent when more resources available)
#define PRIME_MACSAP_ERROR_CON_NOT_OPEN          12 ///< Connection through which to send data packet is not opened
#define PRIME_MACSAP_ERROR_NO_ACKED              13 ///< ACK has not been received or it was an unexpected id
#define PRIME_MACSAP_ERROR_TX_BUSY               14 ///< A previous Data Transmission Request is still on course
#define PRIME_MACSAP_ERROR_INVALID_BCAST         15 ///< Try to close unicast connection with leave request or leave group with release request
#define PRIME_MACSAP_ERROR_INVALID_DIRCON        16 ///< Wrong acknowledge received for a direct connection or direct connection information received for standard connection
#define PRIME_MACSAP_ERROR_INVALID_EUI48         17 ///< Wrong EUI48 destination address
//@}

//! \name MAC_SAP Commands for callback messages 
//@{
#define PRIME_MACSAP_ESTABLISH_CONFIRM           1 ///< Command obtained from Callback when MAC has used Establish.Confirm primitive
#define PRIME_MACSAP_ESTABLISH_INDICATION        2 ///< Command obtained from Callback when MAC has used Establish.Indication primitive
#define PRIME_MACSAP_RELEASE_CONFIRM             3 ///< Command obtained from Callback when MAC has used Release.Confirm primitive
#define PRIME_MACSAP_RELEASE_INDICATION          4 ///< Command obtained from Callback when MAC has used Release.Indication primitive
#define PRIME_MACSAP_DATA_CONFIRM                5 ///< Command obtained from Callback when MAC has used Data.Confirm primitive
#define PRIME_MACSAP_DATA_INDICATION             6 ///< Command obtained from Callback when MAC has used Data.Indication primitive
#define PRIME_MACSAP_JOIN_CONFIRM                8 ///< Command obtained from Callback when MAC has used Join.Confirm primitive
#define PRIME_MACSAP_LEAVE_CONFIRM               9 ///< Command obtained from Callback when MAC has used Leave.Confirm primitive
#define PRIME_MACSAP_JOIN_INDICATION_BASE        10 ///< Command obtained from Callback on Base Node when MAC has used Join.Indication primitive
#define PRIME_MACSAP_LEAVE_INDICATION_BASE       11 ///< Command obtained from Callback on Base Node when MAC has used Leave.Indication primitive
#define PRIME_MACSAP_JOIN_INDICATION_SERVICE     12 ///< Command obtained from Callback on Service Node when MAC has used Join.Indication primitive
#define PRIME_MACSAP_LEAVE_INDICATION_SERVICE    13 ///< Command obtained from Callback on Service Node when MAC has used Leave.Indication primitive
//@}

//! \name MAC_SAP Commands for callback messages (Request & Response) 
//@{
#define PRIME_MACSAP_ESTABLISH_REQUEST           20
#define PRIME_MACSAP_ESTABLISH_RESPONSE          21
#define PRIME_MACSAP_REDIRECT_RESPONSE           22
#define PRIME_MACSAP_RELEASE_REQUEST             23
#define PRIME_MACSAP_RELEASE_RESPONSE            24
#define PRIME_MACSAP_SEND_REQUEST                25
#define PRIME_MACSAP_DATA_REQUEST                26
#define PRIME_MACSAP_JOIN_REQUEST_BASE           27
#define PRIME_MACSAP_LEAVE_REQUEST_BASE          28
#define PRIME_MACSAP_JOIN_RESPONSE_BASE          29
#define PRIME_MACSAP_JOIN_REQUEST_SERVICE        30
#define PRIME_MACSAP_LEAVE_REQUEST_SERVICE       31
#define PRIME_MACSAP_JOIN_RESPONSE_SERVICE       32
//@}

//! \name MAX Buffer size to transmit or receive MAC data 
//@{
#define PRIME_MACSAP_DATA_SIZE                   1024
//@}

//! \name Message Priorities 
//@{
#define MAX_PRIO                                 0
#define PRIO_1                                   1
#define PRIO_2                                   2
#define PRIO_3                                   3
//@}

//! \name Default Priority for Data Messages 
//@{
#define DATA_PRIO                                2
//@}

//! \name Type values defined for different CS defined in PRIME Spec v1.3 
//@{
#define TYPE_CL_IPv4_AR                          1
#define TYPE_CL_IPv4_UNICAST                     2
#define TYPE_CL_432                              3
#define TYPE_CL_MGMT                             4
#define TYPE_CL_APPEMU                           250
//@}

//! \name LCI values for broadcast connections defined in PRIME Spec v1.3 
//@{
#define LCI_CL_IPv4_BROADCAST                    1
#define LCI_CL_432_BROADCAST                     2
//@}

//! \name Generic Handler and Type values to use on MAC SAP Callback 
//@{
#define GENERIC_CALLBACK_TYPE                    0
#define GENERIC_CALLBACK_HANDLER                 0xFFFF
//@}

//! Type for callback information 
typedef struct
{
	uint8_t  command;                            ///< Message command
	uint8_t  type;                               ///< Type of convergence sublayer
	uint16_t handler;                            ///< Connection handler
	uint16_t answer;                             ///< Response value
	uint8_t  errorType;                          ///< Information about what caused a command to fail
	uint16_t bufLength;                          ///< Message buf length
	uint16_t cfpBytes;                           ///< CFP segment reserved bytes
	uint16_t lcid;                               ///< Local connection ID
	uint8_t  arq;                                ///< Use of ARQ mechanism in connection
	uint8_t  priority;                           ///< Message priority
	uint8_t  broadcast;                          ///< Broadcast or multicast flag
	uint8_t  macAddr[PRIME_MAC_LENGTH];          ///< EUI-48 address
	uint8_t  buf[PRIME_MACSAP_DATA_SIZE];        ///< Message data
} MacSapCallBack;

//! \name MAC Functions Interface 
//@{
int prime_MAC_ESTABLISH_request (uint8_t* macAddr, uint8_t type, uint8_t* buf, uint16_t dataLength, uint8_t arq, uint16_t cfpBytes);
int prime_MAC_ESTABLISH_response (uint16_t conHandle, uint8_t answer, uint8_t* buf, uint16_t dataLength);
int prime_MAC_RELEASE_request (uint16_t conHandle);
int prime_MAC_RELEASE_response (uint16_t conHandle, uint8_t answer);
int prime_MAC_JOIN_request_base (uint8_t broadcast, uint16_t handler, uint8_t* macAddr, uint8_t type, uint8_t* buf, uint16_t dataLength);
int prime_MAC_JOIN_request_service (uint8_t broadcast, uint8_t type, uint8_t* buf, uint16_t dataLength);
int prime_MAC_JOIN_response_base (uint16_t handler, uint8_t* macAddr, uint8_t answer);
int prime_MAC_JOIN_response_service (uint16_t handler, uint8_t answer);
int prime_MAC_LEAVE_request_base (uint16_t conHandle, uint8_t* macAddr);
int prime_MAC_LEAVE_request_service (uint16_t conHandle);
int prime_MAC_REDIRECT_response (uint16_t conHandle, uint8_t* macAddr, uint8_t* buf, uint16_t dataLength);
int prime_MAC_DATA_request (uint16_t conHandle, uint8_t* buf, uint16_t dataLength, uint8_t priority);
int prime_MAC_callback (MacSapCallBack *info, uint16_t handler, uint8_t type);
//@}

//! \name Number of different types
//@{
#define PRIME_TYPE_NUM                           255
//@}

//! \name Prime reserved Interface 
//@{
int8_t macSapReserveType(uint8_t type);
int8_t macSapFreeType(uint8_t type);
int8_t macSapGetReserveType(uint8_t type);

int8_t macSapReserveHandler(uint16_t handler, uint16_t iNID, uint8_t type);
int8_t macSapFreeHandler(uint16_t connHandler);
int8_t macSapGetReserveHandler(uint16_t handler);

int prime_MAC_callback_reserved (MacSapCallBack *info, uint16_t handler, uint8_t type);
//@}

//! \name Types for 432 information access 
//@{
//! Extract 432 connections table
typedef struct _Node432
{
	uint16_t handler;
	int8_t serialNumber[SERIAL_NUMBER_432_MAC];
	uint8_t lenSerial;
	uint8_t mac[PRIME_MAC_LENGTH];
	uint8_t arq;
	uint16_t next;
	uint16_t prev;
} Node432;
//@}

//! \name Macros
//@{
#define GET_INDEX_NODE_432(X)                    (X-2)
#define GET_ADDRESS_NODE_432(X)                  (X+2)
//@}

//! \name Embedded Sniffer Interface 
//@{
#define MASK_SNIFFER_EMBEDDED                    0x80
//@}

//! \name Sniffer types
//@{
#define SNIFFER_ATPL210                          0x00
#define SNIFFER_ATPL240                          0x01
//@}

typedef enum
{
	SNIFFER_RX = 0,
	SNIFFER_TX = 1,
	SNIFFER_TX_SERIAL = 2,
	SNIFFER_RX_SERIAL = 3
} SnifferType;

typedef struct
{
	SnifferType type;
	uint8_t scheme;
	uint8_t level;
	uint8_t exSnr;
	uint32_t time;
} SnifferParam;


//! @}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
#endif /* PRIME_H_INCLUDE */
