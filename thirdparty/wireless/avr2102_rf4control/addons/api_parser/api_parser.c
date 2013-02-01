#ifdef SIO_HUB

/* === INCLUDES ============================================================ */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "compiler.h"
#include "nwk_msg_code.h"
#include "app_config.h"
#include "conf_sio2ncp.h"
#include "sio2ncp.h"
#include "api_parser.h"
#include "rf4ce.h"
#ifdef ZRC_PROFILE
#include "pb_pairing.h"
#include "zrc.h"
#endif
#include "vendor_data.h"

/* === MACROS ============================================================== */

/* UART communication framing */

/** The start of transmission delimiter. */
#define SOT                             (1)

/** The end of transmission delimiter. */
#define EOT                             (4)

/**
 * A UART state that expects a \ref SOT to be received as the next character.
 */
#define UART_RX_STATE_SOT               (1)

/**
 * A UART state that expects the length to be received as the next character.
 */
#define UART_RX_STATE_LENGTH            (2)

/**
 * A UART state that expects the length to be received as the next character.
 */
#define UART_RX_PROTOCOL_ID             (3)

/**
 * A UART state that expects the next data character to be received.
 */
#define UART_RX_STATE_DATA              (4)

/**
 * A UART state that expects a \ref EOT to be received as the next character.
 */
#define UART_RX_STATE_EOT               (5)

#define SIO_BUF_COUNT                   (2)

#define SIO_RX2NCP_BUF_SIZE                 (200)
#define SIO_TX_BUF_SIZE                 SIO_RX2NCP_BUF_SIZE

#ifdef UART0
#define SIO_CHANNEL                     (SIO_0)
#endif
#ifdef UART1
#define SIO_CHANNEL                     (SIO_1)
#endif
#ifdef USB0
#define SIO_CHANNEL                     (SIO_2)
#endif

/* === Globals ============================================================= */

/**
 * This is the receive buffer of the UART.
 */
static uint8_t sio_rx_buf[SIO_RX2NCP_BUF_SIZE];

/**
 * This is the transmit buffer of the UART.
 */
static uint8_t sio_tx_buf[SIO_BUF_COUNT][SIO_TX_BUF_SIZE];

/**
 * This pointer points to the next free element inside the
 * receive buffer of the UART.
 */
static uint8_t *sio_rx_ptr;

/**
 * This is the receiver state of the UART. (\ref UART_RX_STATE_SOT,
 *   \ref UART_RX_STATE_LENGTH, \ref UART_RX_STATE_DATA, or
 *   \ref UART_RX_STATE_EOT)
 */
static volatile uint8_t sio_rx_state;

/**
 * This is the transmit state of the UART. (\ref UART_TX_STATE_SOT,
 *   \ref UART_TX_STATE_LENGTH, \ref UART_TX_STATE_DATA, or
 *   \ref UART_TX_STATE_EOT)
 */
//volatile uint8_t sio_tx_state;

/**
 * This is the length of the message should be received.
 */
static uint8_t sio_rx_length;

/**
 * This is the length of the message should be transmitted.
 */
static uint8_t data[20];
static uint8_t data_length = 0;
static uint8_t rx_index = 0;
static uint8_t head = 0, buf_count = 0;
static uint8_t protocol_id;
static zrc_indication_callback_t zrc_ind_cb;
static nwk_indication_callback_t nwk_ind_callback;
/* === Prototypes ========================================================== */

static inline void process_incoming_data(void);
nwk_enum_t nwk_init(void);
static uint8_t *get_next_tx_buffer(void);
static inline void handle_incoming_msg(void);
bool nlme_reset_request(bool SetDefaultNIB
#ifdef RF4CE_CALLBACK_PARAM
                        , FUNC_PTR confirm_cb
#endif
                       );
bool nlde_data_request(uint8_t PairingRef, profile_id_t ProfileId,
                           uint16_t VendorId, uint8_t nsduLength, uint8_t *nsdu,
                           uint8_t TxOptions
#if (defined NLDE_HANDLE) || (defined DOXYGEN)
                           , uint8_t Handle
#endif
#if (defined RF4CE_CALLBACK_PARAM) || (defined DOXYGEN)
                           , FUNC_PTR confirm_cb
#endif
                          );
bool nlme_auto_discovery_request(uint8_t RecAppCapabilities,
                                 dev_type_t RecDevTypeList[DEVICE_TYPE_LIST_SIZE],
                                 profile_id_t RecProfileIdList[PROFILE_ID_LIST_SIZE],
                                 uint32_t AutoDiscDuration
#ifdef RF4CE_CALLBACK_PARAM
                                 , FUNC_PTR confirm_cb
#endif
                                );
bool nlme_discovery_request(uint16_t DstPANId, uint16_t DstNwkAddr,
                                uint8_t OrgAppCapabilities,
                                dev_type_t OrgDevTypeList[3],
                                profile_id_t OrgProfileIdList[7],
                                dev_type_t SearchDevType,
                                uint8_t DiscProfileIdListSize,
                                profile_id_t DiscProfileIdList[7],
                                uint32_t DiscDuration
#if (defined RF4CE_CALLBACK_PARAM) || (defined DOXYGEN)
                                , FUNC_PTR confirm_cb
#endif
                               );

bool nlme_discovery_response(nwk_enum_t Status, uint64_t DstIEEEAddr,
                                 uint8_t RecAppCapabilities, dev_type_t RecDevTypeList[3],
                                 profile_id_t RecProfileIdList[7], uint8_t DiscReqLQI);
bool nlme_pair_request(uint8_t LogicalChannel, uint16_t DstPANId, uint64_t DstIEEEAddr,
                       uint8_t OrgAppCapabilities, dev_type_t OrgDevTypeList[DEVICE_TYPE_LIST_SIZE],
                       profile_id_t OrgProfileIdList[PROFILE_ID_LIST_SIZE], uint8_t KeyExTransferCount
#ifdef RF4CE_CALLBACK_PARAM
                       , FUNC_PTR confirm_cb
#endif
                      );
bool nlme_pair_response(nwk_enum_t Status, uint16_t DstPANId, uint64_t DstIEEEAddr,
                            uint8_t RecAppCapabilities, dev_type_t RecDevTypeList[3],
                            profile_id_t RecProfileIdList[7], uint8_t ProvPairingRef);
bool nlme_get_request(nib_attribute_t NIBAttribute, uint8_t NIBAttributeIndex
#ifdef RF4CE_CALLBACK_PARAM
                      , FUNC_PTR confirm_cb
#endif
                     );
bool nlme_rx_enable_request(uint32_t RxOnDuration
#ifdef RF4CE_CALLBACK_PARAM
                            , FUNC_PTR confirm_cb
#endif
                           );
bool nlme_set_request(nib_attribute_t NIBAttribute, uint8_t NIBAttributeIndex, uint8_t *NIBAttributeValue
#ifdef RF4CE_CALLBACK_PARAM
                      , FUNC_PTR confirm_cb
#endif
                     );
bool nlme_start_request(
#ifdef RF4CE_CALLBACK_PARAM
    FUNC_PTR confirm_cb);
#else
    void);
#endif
bool nlme_unpair_request(uint8_t PairingRef
#ifdef RF4CE_CALLBACK_PARAM
                         , FUNC_PTR confirm_cb
#endif
                        );
bool nlme_unpair_response(uint8_t PairingRef);

bool nlme_update_key_request(uint8_t PairingRef, uint8_t NewLinkKey[16]
#ifdef RF4CE_CALLBACK_PARAM
                             , FUNC_PTR confirm_cb
#endif
                            );
bool nwk_ch_agility_request(nwk_agility_mode_t AgilityMode
#ifdef RF4CE_CALLBACK_PARAM
                            , FUNC_PTR confirm_cb
#endif
                           );
bool pbp_org_pair_request(uint8_t OrgAppCapabilities,
                          dev_type_t OrgDevTypeList[DEVICE_TYPE_LIST_SIZE],
                          profile_id_t OrgProfileIdList[PROFILE_ID_LIST_SIZE],
                          dev_type_t SearchDevType, uint8_t DiscProfileIdListSize,
                          profile_id_t DiscProfileIdList[PROFILE_ID_LIST_SIZE]
#ifdef RF4CE_CALLBACK_PARAM
                          , FUNC_PTR confirm_cb
#endif
                         );
bool pbp_rec_pair_request(uint8_t RecAppCapabilities,
                          dev_type_t RecDevTypeList[3],
                          profile_id_t RecProfileIdList[7]
#if (defined RF4CE_CALLBACK_PARAM) || (defined DOXYGEN)
                          , FUNC_PTR confirm_cb
#endif
                         );
bool zrc_cmd_request(uint8_t PairingRef, uint16_t VendorId, zrc_cmd_code_t CmdCode,
                     uint8_t CmdLength, uint8_t *Cmd, uint8_t TxOptions
#ifdef RF4CE_CALLBACK_PARAM
                     , FUNC_PTR confirm_cb
#endif
                    );
bool zrc_cmd_disc_request(uint8_t PairingRef
#ifdef RF4CE_CALLBACK_PARAM
                          , FUNC_PTR confirm_cb
#endif
                         );
bool zrc_cmd_disc_response(uint8_t PairingRef, uint8_t *SupportedCmd);
bool vendor_data_request(uint8_t PairingRef, profile_id_t ProfileId,
                         uint16_t VendorId, uint8_t nsduLength, uint8_t *nsdu,
                         uint8_t TxOptions);
void register_nwk_indication_callback(nwk_indication_callback_t *nwk_ind_cb);
void register_zrc_indication_callback(zrc_indication_callback_t *zrc_ind_callback);
#ifdef VENDOR_DATA
extern void vendor_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t ProfileId
#ifdef NLDE_HANDLE
                                , uint8_t Handle
#endif
                               );
#endif
/* === Externals ==========================================================  */
#ifdef RF4CE_CALLBACK_PARAM
/* Network confirmation callback pointers */

nlme_start_confirm_cb_t nlme_start_confirm_cb;
nlme_reset_confirm_cb_t nlme_reset_confirm_cb;
nlme_rx_enable_confirm_cb_t nlme_rx_enable_confirm_cb;
nlde_data_confirm_cb_t nlde_data_confirm_cb;

#if (defined RF4CE_TARGET) || (defined RF4CE_PLATFORM)
 nlme_auto_discovery_confirm_cb_t nlme_auto_discovery_confirm_cb;
#endif
#if (NWK_DISCOVERY_REQ_CONF == 1)
 nlme_discovery_confirm_cb_t nlme_discovery_confirm_cb;
#endif
#if (NWK_GET == 1)
 nlme_get_confirm_cb_t nlme_get_confirm_cb;
#endif
#if (NWK_PAIR_REQ_CONF == 1)
 nlme_pair_confirm_cb_t nlme_pair_confirm_cb;
#endif
#if (NWK_SET == 1)
 nlme_set_confirm_cb_t nlme_set_confirm_cb;
#endif
#if (NWK_UNPAIR_REQ_CONF == 1)
 nlme_unpair_confirm_cb_t nlme_unpair_confirm_cb;
#endif
#if (NWK_UPDATE_KEY == 1) || (defined RF4CE_PLATFORM)
 nlme_update_key_confirm_cb_t nlme_update_key_confirm_cb;
#endif
#if (defined CHANNEL_AGILITY) || (defined RF4CE_PLATFORM)
 nwk_ch_agility_confirm_cb_t nwk_ch_agility_confirm_cb;
#endif
pbp_org_pair_confirm_cb_t  pbp_org_pair_confirm_cb;
pbp_rec_pair_confirm_cb_t  pbp_rec_pair_confirm_cb;
#ifndef RF4CE_TARGET
zrc_cmd_confirm_cb_t zrc_cmd_confirm_cb;
#endif
zrc_cmd_disc_confirm_cb_t zrc_cmd_disc_confirm_cb;
#endif /* RF4CE_CALLBACK_PARAM */

///////////////
/** Gets attribute size by reading from a table stored in flash */
#define GET_NIB_ATTR_SIZE(x)    PGM_READ_BYTE(&nib_attr_size[x - nwkActivePeriod])


/* === Globals ============================================================= */

#if (NWK_SET == 1) || (NWK_GET == 1)
/** Size constants for NIB attributes */
static FLASH_DECLARE(uint8_t nib_attr_size[]) =
{
    sizeof(uint32_t),               // 0x60: nwkActivePeriod
    sizeof(uint8_t),                // 0x61: nwkBaseChannel
    sizeof(uint8_t),                // 0x62: nwkDiscoveryLQIThreshold
    sizeof(uint32_t),               // 0x63: nwkDiscoveryRepetitionInterval
    sizeof(uint32_t),               // 0x64: nwkDutyCycle
    sizeof(uint32_t),               // 0x65: nwkFrameCounter
    sizeof(bool),                   // 0x66: nwkIndicateDiscoveryRequests
    sizeof(bool),                   // 0x67: nwkInPowerSave
    sizeof(pairing_table_t),        // 0x68: nwkPairingTable
    sizeof(uint8_t),                // 0x69: nwkMaxDiscoveryRepetitions
    sizeof(uint8_t),                // 0x6A: nwkMaxFirstAttemptCSMABackoffs
    sizeof(uint8_t),                // 0x6B: nwkMaxFirstAttemptFrameRetries
    sizeof(uint8_t),                // 0x6C: nwkMaxReportedNodeDescriptors
    sizeof(uint32_t),               // 0x6D: nwkResponseWaitTime
    sizeof(uint8_t),                // 0x6E: nwkScanDuration
    (15 * sizeof(uint8_t)),         // 0x6F: nwkUserString
    sizeof(uint64_t),               // 0x70: nwkPrivateIeeeAddr
    sizeof(uint16_t),               // 0x71: nwkPrivateVendorIdentifier
    (7 * sizeof(uint8_t)),          // 0x72: nwkPrivateVendorString
    sizeof(uint8_t),                // 0x73: nwkPrivateNodeCapabilities
    sizeof(uint16_t),               // 0x74: nwkPrivatePanIdentifier
    sizeof(uint16_t),               // 0x75: nwkPrivateShortAddress
    sizeof(uint8_t),                // 0x76: nwkPrivateMaxPairingTableEntries
    sizeof(bool),                   // 0x77: nwkChAgEnabled
    sizeof(uint32_t),               // 0x78: nwkPrivateChAgScanInterval
    sizeof(uint8_t)                 // 0x79: nwkPrivateChAgEdThreshold
#ifdef ZRC_PROFILE
    ,
    sizeof(uint8_t),                // 0x80: aplKeyRepeatInterval
    sizeof(uint8_t),                // 0x81: aplKeyRepeatWaitTime
    sizeof(uint8_t)                 // 0x82: aplKeyExchangeTransferCount
#endif
};
#endif
/**
 * @brief This function does the initialization of the SIO or UART.
 */
void serial_api_init(void)
{
    sio_rx_state = UART_RX_STATE_SOT;
    sio2ncp_init();
}
nwk_enum_t nwk_init(void)
{
  serial_api_init();
  return NWK_SUCCESS;
}

/**
 * @brief Function to handle the state machine serial data exchange.
 */
bool nwk_task(void)
{
    /* Rx processing */
    if (data_length == 0)
    {
        /* No data to process, read the stream IO */
        rx_index = 0;
        data_length = sio2ncp_rx(data, SIO_RX2NCP_BUF_SIZE);
    }
    else    /* Data has been received, process the data */
    {
        /* Process each single byte */
        process_incoming_data();
        data_length--;
        rx_index++;
    }

    /* Tx processing */
    if (buf_count != 0)
    {
        if (sio2ncp_tx(sio_tx_buf[head], (sio_tx_buf[head][1] + 3)) != 0)
        {
            head++;
            head %= SIO_BUF_COUNT;
            buf_count--;
        }
        else
        {
            // @ToDo what happens if none or only a part of the bytes could be transmitted?
        }
    }
 return true;
}

/*
 * @brief Process data received from SIO
 */
static inline void process_incoming_data(void)
{
    switch (sio_rx_state)
    {
        case UART_RX_STATE_SOT:
            sio_rx_ptr = sio_rx_buf;
            if (SOT == data[rx_index])
            {
                sio_rx_state = UART_RX_STATE_LENGTH;
            }
            break;

        case UART_RX_STATE_LENGTH:
            sio_rx_length = data[rx_index];
            if (sio_rx_length)
            {
                sio_rx_state = UART_RX_PROTOCOL_ID;
                *sio_rx_ptr = sio_rx_length;
                sio_rx_ptr++;
            }
            else
            {
                /* NULL message */
                sio_rx_ptr = sio_rx_buf;
                sio_rx_state = UART_RX_STATE_SOT;
            }
            break;

		case UART_RX_PROTOCOL_ID:
            
            protocol_id = data[rx_index];
			sio_rx_length--;
            sio_rx_state = UART_RX_STATE_DATA;
          
            break;
			
        case UART_RX_STATE_DATA:
            *sio_rx_ptr = data[rx_index];
            sio_rx_ptr++;
            sio_rx_length--;
            if (!sio_rx_length)
            {
                sio_rx_state = UART_RX_STATE_EOT;
            }
            break;

        case UART_RX_STATE_EOT:
            if (EOT == data[rx_index])
            {
			    if(RF4CONTROL_PID == protocol_id)
                {
                    /* Message received successfully */
                    handle_incoming_msg();
				}
            }
            /* Make rx buffer ready for next reception before handling received data. */
            sio_rx_ptr = sio_rx_buf;
            sio_rx_state = UART_RX_STATE_SOT;
            break;

        default:
            sio_rx_ptr = sio_rx_buf;
            sio_rx_state = UART_RX_STATE_SOT;
            break;
    }
}



static uint8_t *get_next_tx_buffer(void)
{
    if (buf_count != SIO_BUF_COUNT)
    {
        uint8_t *buf;
        uint8_t tail;

        tail = (head + buf_count) % SIO_BUF_COUNT;
        buf = (uint8_t *)(&sio_tx_buf[tail]);
        buf_count++;
        /* Add message start character */
        *buf++ = SOT;
        return buf;
    }

    return NULL;
}

static inline void handle_incoming_msg(void)
{
    

    switch (sio_rx_buf[1])  /* message type */
    {
      case NLME_RESET_CONFIRM:
        {
#ifdef RF4CE_CALLBACK_PARAM
            nlme_reset_confirm_cb(
#else
            nlme_reset_confirm(
#endif
            (nwk_enum_t)sio_rx_buf[2]);
        }
         break;
      case NLDE_DATA_CONFIRM:
          {
#ifdef RF4CE_CALLBACK_PARAM
            nlde_data_confirm_cb(
#else
            nlde_data_confirm(
#endif
            (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3],(profile_id_t)sio_rx_buf[4],
#ifdef NLDE_HANDLE
             1
#endif
                             );
             
          }
          break;
      case NLME_AUTO_DISCOVERY_CONFIRM:
         {
           uint64_t SrcIEEEAddr;
           memcpy(&SrcIEEEAddr, &sio_rx_buf[3], 8);
#ifdef RF4CE_CALLBACK_PARAM
         nlme_auto_discovery_confirm_cb(
#else
         nlme_auto_discovery_confirm(
#endif
        (nwk_enum_t)sio_rx_buf[2],SrcIEEEAddr);
                                
         }
         break;
      case NLME_DISCOVERY_CONFIRM:
        {
          node_desc_t NodeDescList[3];
          uint8_t i;
          uint8_t k;
          uint8_t *addr_ptr;
          uint8_t numnodes=sio_rx_buf[3];
		  uint8_t index = 5;
          for (i = 0; i <numnodes; i++)
          {
	          NodeDescList[i].Status = (nwk_enum_t)sio_rx_buf[index++];
	          NodeDescList[i].LogicalChannel = sio_rx_buf[index++];
	          NodeDescList[i].PANId = sio_rx_buf[index++];
			  NodeDescList[i].PANId |=  (sio_rx_buf[index++] << 8);
	          addr_ptr = (uint8_t *)(&NodeDescList[i].IEEEAddr);
	          for (k = 0; k < 8; k++)
	          {
		          *addr_ptr++ = sio_rx_buf[index++];
	          }
	          NodeDescList[i].NodeCapabilities = sio_rx_buf[index++];//17
	          addr_ptr = (uint8_t *)&NodeDescList[i].VendorId;
	          *addr_ptr++ = sio_rx_buf[index++];//18
	          *addr_ptr= sio_rx_buf[index++];//19
	          for (k = 0; k < 7; k++)
	          {
		          NodeDescList[i].VendorString[k]= sio_rx_buf[index++];//20
	          }
	          NodeDescList[i].AppCapabilities = sio_rx_buf[index++];//27
	          for (k = 0; k < 15; k++)
	          {
		          NodeDescList[i].UserString[k]= sio_rx_buf[index++];//28
	          }
	          for (k = 0; k < DEVICE_TYPE_LIST_SIZE; k++)
	          {

		          NodeDescList[i].DevTypeList[k] = (dev_type_t)sio_rx_buf[index++];//43
	          }
	          for (k = 0; k < PROFILE_ID_LIST_SIZE; k++)
	          {

		          NodeDescList[i].ProfileIdList[k]= (profile_id_t)sio_rx_buf[index++];//46

	          }
	          NodeDescList[i].DiscReqLQI = sio_rx_buf[index++];//53
          }
    
    
#ifdef RF4CE_CALLBACK_PARAM
          nlme_discovery_confirm_cb(
#else
          nlme_discovery_confirm(
#endif
          (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3],NodeDescList);
    
        }
        break;
     case NLME_GET_CONFIRM:
        {
#ifdef RF4CE_CALLBACK_PARAM
           nlme_get_confirm_cb(
#else
           nlme_get_confirm(
#endif
           (nwk_enum_t)sio_rx_buf[2],(nib_attribute_t)sio_rx_buf[3],
           sio_rx_buf[4],(void *)&sio_rx_buf[6]);
        }
        break;
     case NLME_PAIR_CONFIRM:
      {
        uint8_t RecVendorString[7];
        for (uint8_t i = 0; i < 7; i++)
        {
          RecVendorString[i] = (uint8_t)sio_rx_buf[6 + i];
        }
        uint8_t RecUserString[15];
        for (uint8_t i = 0; i < 15; i++)
        {
          RecUserString[i] = (uint8_t)sio_rx_buf[14 + i];
        }
        dev_type_t RecDevTypeList[DEVICE_TYPE_LIST_SIZE];
        for (uint8_t i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
        {
          RecDevTypeList[i] = (dev_type_t)sio_rx_buf[29 + i];
        }
        profile_id_t RecProfileIdList[PROFILE_ID_LIST_SIZE];
        for (uint8_t i = 0; i < PROFILE_ID_LIST_SIZE; i++)
        {
          RecProfileIdList[i] = (profile_id_t)sio_rx_buf[32 + i];
        }
#ifdef RF4CE_CALLBACK_PARAM
      nlme_pair_confirm_cb(
#else
      nlme_pair_confirm(
#endif 
        (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3],
        ((uint16_t)sio_rx_buf[4] | ((uint16_t)sio_rx_buf[5] << 8)),//Rec Vendor Id//
        RecVendorString,//uint8_t RecVendorString[7],
        sio_rx_buf[6+7],//RecAppCapabilities
        RecUserString,
        RecDevTypeList,
        RecProfileIdList);
      }
      break;
      case NLME_RX_ENABLE_CONFIRM: 
      {
#ifdef RF4CE_CALLBACK_PARAM
      nlme_rx_enable_confirm_cb(
#else
      nlme_rx_enable_confirm(
#endif
     (nwk_enum_t)sio_rx_buf[2]);
                           
      }
      break;
     case NLME_SET_CONFIRM: 
      {
#ifdef RF4CE_CALLBACK_PARAM
        nlme_set_confirm_cb(
#else
        nlme_set_confirm(
#endif
       (nwk_enum_t)sio_rx_buf[2],(nib_attribute_t)sio_rx_buf[3],sio_rx_buf[4]);                
     }
     break;
     case NLME_START_CONFIRM: 
      {
#ifdef RF4CE_CALLBACK_PARAM
        nlme_start_confirm_cb(
#else
        nlme_start_confirm(
#endif
       (nwk_enum_t)sio_rx_buf[2]);
                           
      }
      break;
     case NLME_UNPAIR_CONFIRM:  
      {
#ifdef RF4CE_CALLBACK_PARAM
        nlme_unpair_confirm_cb(
#else
        nlme_unpair_confirm(
#endif
        (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3]);
      }
      break;
     case NLME_UPDATE_KEY_CONFIRM: 
       {
#ifdef RF4CE_CALLBACK_PARAM
         nlme_update_key_confirm_cb(
#else
         nlme_update_key_confirm(
#endif
        (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3]);
       }
       break;
     case NWK_CH_AGILITY_CONFIRM:  
       {
#ifdef RF4CE_CALLBACK_PARAM
         nwk_ch_agility_confirm_cb(
#else
         nwk_ch_agility_confirm(
#endif
         (nwk_enum_t)sio_rx_buf[2],(bool)sio_rx_buf[3],sio_rx_buf[4]);
       }
       break;
     case PBP_ORG_PAIR_CONFIRM:    
       {
#ifdef RF4CE_CALLBACK_PARAM
            pbp_org_pair_confirm_cb(
#else
            pbp_org_pair_confirm(
#endif
       (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3]);
       }
       break;
     case PBP_REC_PAIR_CONFIRM:
       {
#ifdef RF4CE_CALLBACK_PARAM
            pbp_rec_pair_confirm_cb(
#else
            pbp_rec_pair_confirm(
#endif
       (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3]);   
         
       }
       break;
     case ZRC_CMD_CONFIRM:
       {
#ifdef RF4CE_CALLBACK_PARAM
       zrc_cmd_confirm_cb(
#else
       zrc_cmd_confirm(
#endif
       (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3],(cec_code_t)sio_rx_buf[4]);            
       }
       break;
      case ZRC_CMD_DISCOVERY_CONFIRM:
        {
#ifdef RF4CE_CALLBACK_PARAM
         zrc_cmd_disc_confirm_cb(
#else
         zrc_cmd_disc_confirm(
#endif 
         (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3],&sio_rx_buf[4]);
            
        }
        break;
      case VENDOR_DATA_CONFIRM:
        {
#ifdef VENDOR_DATA
#ifdef RF4CE_CALLBACK_PARAM
        vendor_data_confirm(
#endif 
        (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3],(profile_id_t)sio_rx_buf[4],
#ifdef NLDE_HANDLE
         1
#endif
        );
#endif
        }
        break;
      case NLDE_DATA_INDICATION:
        {
#ifdef RF4CE_CALLBACK_PARAM
        if (nwk_ind_callback.nlde_data_indication_cb == NULL)
        {
          return;
        }
        nwk_ind_callback.nlde_data_indication_cb(
#else
    nlde_data_indication(
#endif       
        sio_rx_buf[2],(profile_id_t)sio_rx_buf[3],
        ((uint16_t)sio_rx_buf[4] | ((uint16_t)sio_rx_buf[5] << 8)),// Vendor Id//
        sio_rx_buf[8],//nsdu length
        &sio_rx_buf[9],
        sio_rx_buf[6],//rx link quality
        sio_rx_buf[7]//rx flags
        );
        
        }
        break;
      case NLME_COMM_STATUS_INDICATION:
        {
#ifdef RF4CE_CALLBACK_PARAM
      if (nwk_ind_callback.nlme_comm_status_indication_cb == NULL)
      {
        return;
      }
      nwk_ind_callback.nlme_comm_status_indication_cb(
#else
      nlme_comm_status_indication(
#endif
      (nwk_enum_t)sio_rx_buf[2],sio_rx_buf[3],
      ((uint16_t)sio_rx_buf[4] | ((uint16_t)sio_rx_buf[5] << 8)),//DstPANId//
       sio_rx_buf[6],
       *(uint64_t *)&sio_rx_buf[7]);
       }
       break;
       case NLME_DISCOVERY_INDICATION:
         {
//           uint8_t OrgVendorString[7];
//           for (uint8_t i = 0; i < 7; i++)
//           {
//              OrgVendorString[i] = (uint8_t)sio_rx_buf[14 + i];
//           }
//           uint8_t OrgUserString[15];
//           for (uint8_t i = 0; i < 15; i++)
//           {
//              OrgUserString[i] = (uint8_t)sio_rx_buf[22+ i];
//           }
//           dev_type_t OrgDevTypeList[3];
//           for (uint8_t i = 0; i < 3; i++)
//           {
//              OrgDevTypeList[i] = (dev_type_t)sio_rx_buf[45+ i];
//           }
//           profile_id_t OrgProfileIdList[7];
//           for (uint8_t i = 0; i < 7; i++)
//           {
//              OrgProfileIdList[i] = (profile_id_t)sio_rx_buf[40+ i];
//           }
           
#ifdef RF4CE_CALLBACK_PARAM
         if (nwk_ind_callback.nlme_discovery_indication_cb == NULL)
         {
           return;
         }
         nwk_ind_callback.nlme_discovery_indication_cb(
#else
         nlme_discovery_indication(
#endif
         (nwk_enum_t)sio_rx_buf[2],*(uint64_t *)&sio_rx_buf[3],//DstIEEEaddr
         sio_rx_buf[11], ((uint16_t)sio_rx_buf[12] | ((uint16_t)sio_rx_buf[13] << 8)),
         (uint8_t *)&sio_rx_buf[14],sio_rx_buf[21],
         (uint8_t *)&sio_rx_buf[22],(dev_type_t *)&sio_rx_buf[37],(profile_id_t *)&sio_rx_buf[40],(dev_type_t)sio_rx_buf[47],
         sio_rx_buf[48]);
         
         }
         break;
         case NLME_PAIR_INDICATION:
           {
             uint8_t OrgVendorString[7];
             for (uint8_t i = 0; i < 7; i++)
             {
               OrgVendorString[i] = (uint8_t)sio_rx_buf[16 + i];
             }
             uint8_t OrgUserString[15];
             for (uint8_t i = 0; i < 7; i++)
             {
               OrgUserString[i] = (uint8_t)sio_rx_buf[24 + i];
             }
             dev_type_t OrgDevTypeList[3];
             for (uint8_t i = 0; i < 3; i++)
             {
               OrgDevTypeList[i] = (dev_type_t)sio_rx_buf[39 + i];
             }
             profile_id_t OrgProfileIdList[7];
             for (uint8_t i = 0; i < 3; i++)
             {
               OrgProfileIdList[i] = (profile_id_t)sio_rx_buf[42 + i];
             }
#ifdef RF4CE_CALLBACK_PARAM
        if (nwk_ind_callback.nlme_pair_indication_cb == NULL)
        {
          return;
        }
       nwk_ind_callback.nlme_pair_indication_cb(
#else
       nlme_pair_indication(
#endif
       (nwk_enum_t)sio_rx_buf[2],((uint16_t)sio_rx_buf[3] | ((uint16_t)sio_rx_buf[4] << 8)),
        *(uint64_t *)&sio_rx_buf[5],
        sio_rx_buf[13],((uint16_t)sio_rx_buf[14] | ((uint16_t)sio_rx_buf[15] << 8)),
        OrgVendorString,sio_rx_buf[23],OrgUserString,
        OrgDevTypeList,OrgProfileIdList,sio_rx_buf[49],
        sio_rx_buf[50]);
        }
        break;
        case NLME_UNPAIR_INDICATION:
          {
#ifdef RF4CE_CALLBACK_PARAM
          if (nwk_ind_callback.nlme_unpair_indication_cb == NULL)
          {
            return;
          }
          nwk_ind_callback.nlme_unpair_indication_cb(
#else
          nlme_unpair_indication(
#endif
          sio_rx_buf[2]);//pairing ref
          }
          break;
          case NWK_CH_AGILITY_INDICATION:
            {
#ifdef RF4CE_CALLBACK_PARAM
            if (nwk_ind_callback.nwk_ch_agility_indication_cb == NULL)
            {
              return;
            }
            nwk_ind_callback.nwk_ch_agility_indication_cb(
#else
            nwk_ch_agility_indication(
#endif
             sio_rx_buf[2]);//logical channel
            
            }
            break;
          case ZRC_CMD_INDICATION:
            {
#ifdef RF4CE_CALLBACK_PARAM
            if (zrc_ind_cb.zrc_cmd_indication_cb == NULL)
            {
              return;
            }
            zrc_ind_cb.zrc_cmd_indication_cb(
#else
            zrc_cmd_indication(
#endif
            sio_rx_buf[2],//pairing ref
            sio_rx_buf[5],//nsdulength
            &sio_rx_buf[6],
            sio_rx_buf[3],//rxlink quality
            sio_rx_buf[4]//rxflags
              );
            
            }
            break;
        case ZRC_CMD_DISCOVERY_INDICATION:
          {
#ifdef RF4CE_CALLBACK_PARAM
            if (zrc_ind_cb.zrc_cmd_disc_indication_cb == NULL)
            {
              return;
            }
            zrc_ind_cb.zrc_cmd_disc_indication_cb(
#else
            zrc_cmd_disc_indication(
#endif
          
          sio_rx_buf[2]//pairing ref
         
          );
          }
          break;
       case VENDOR_DATA_INDICATION:
         {
#ifdef RF4CE_CALLBACK_PARAM
        zrc_ind_cb.vendor_data_ind_cb(
//#else
//        vendor_data_ind(
#endif
        sio_rx_buf[2],//pairing ref
        ((uint16_t)sio_rx_buf[4] | ((uint16_t)sio_rx_buf[5] << 8)),//vendor id
        sio_rx_buf[8],//nsdu length
        &sio_rx_buf[9],
        sio_rx_buf[6],//rx link quality
        sio_rx_buf[7]//rx flags
          );
        }
        break;
    }
}

bool nlme_reset_request(bool SetDefaultNIB
#ifdef RF4CE_CALLBACK_PARAM
                        , FUNC_PTR confirm_cb
#endif
                       )

{
  uint8_t *msg_buf;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
  {
        return false;
  }
#ifdef RF4CE_CALLBACK_PARAM
    if (confirm_cb == NULL)
    {
        return false;
    }
   nlme_reset_confirm_cb = (nlme_reset_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_RESET_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_RESET_REQUEST;
  *msg_buf++ = SetDefaultNIB;
  *msg_buf = EOT;
return true;
}


bool nlde_data_request(uint8_t PairingRef, profile_id_t ProfileId,
                           uint16_t VendorId, uint8_t nsduLength, uint8_t *nsdu,
                           uint8_t TxOptions
#if (defined NLDE_HANDLE) || (defined DOXYGEN)
                           , uint8_t Handle
#endif
#if (defined RF4CE_CALLBACK_PARAM) || (defined DOXYGEN)
                           , FUNC_PTR confirm_cb
#endif
                          )
{
   uint8_t *msg_buf;
   uint8_t *ptr;
   msg_buf = get_next_tx_buffer();
   if (NULL == msg_buf)
   {
        return false;
   }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlde_data_confirm_cb = (nlde_data_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLDE_DATA_REQ_LEN +RF4CONTROL_PID_LEN + nsduLength;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLDE_DATA_REQUEST;
  *msg_buf++ = PairingRef;
  *msg_buf++ = ProfileId;
#if (UC3) || (SAM)
    ptr = (uint8_t *)&VendorId;
    *msg_buf++ = *ptr++;
    *msg_buf++ = *ptr++;
#else
    *msg_buf++ = (uint8_t)VendorId; //LSB
    *msg_buf++ = (uint8_t)(VendorId >> 8); //MSB
#endif
    *msg_buf++ = TxOptions;
    *msg_buf++ = nsduLength;
     ptr = nsdu;
    // @ToDo use memcpy
    for (uint8_t i = 0; i < nsduLength; i++)
    {
        *msg_buf++ = *ptr++;
    }
    
    *msg_buf = EOT;
    Handle = Handle;
  return true;
  
}


/* The NLME-AUTO-DISCOVERY.request primitive allows the application to request
 * the NLME automatically handles the receipt of discovery request command frames. */

bool nlme_auto_discovery_request(uint8_t RecAppCapabilities,
                                 dev_type_t RecDevTypeList[DEVICE_TYPE_LIST_SIZE],
                                 profile_id_t RecProfileIdList[PROFILE_ID_LIST_SIZE],
                                 uint32_t AutoDiscDuration
#ifdef RF4CE_CALLBACK_PARAM
                                 , FUNC_PTR confirm_cb
#endif
                                )
{
  uint8_t *msg_buf;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_auto_discovery_confirm_cb = (nlme_auto_discovery_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_AUTO_DISCOVERY_REQ_LEN + RF4CONTROL_PID_LEN+1;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_AUTO_DISCOVERY_REQUEST;
  *msg_buf++ = RecAppCapabilities;
// #ifndef UC3
   // ptr = (uint8_t *)RecDevTypeList;
// #endif
   // for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = RecDevTypeList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }
// #ifndef UC3
   // ptr = (uint8_t *)RecProfileIdList;
// #endif
   // for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = RecProfileIdList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }
   for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
    {
        *msg_buf++ = RecDevTypeList[i];
    }
   for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = RecProfileIdList[i];
    }
  memcpy(msg_buf, &AutoDiscDuration, sizeof(uint32_t));
  msg_buf += sizeof(uint32_t);
  *msg_buf = EOT;
return true;
}


bool nlme_discovery_request(uint16_t DstPANId, uint16_t DstNwkAddr,
                                uint8_t OrgAppCapabilities,
                                dev_type_t OrgDevTypeList[3],
                                profile_id_t OrgProfileIdList[7],
                                dev_type_t SearchDevType,
                                uint8_t DiscProfileIdListSize,
                                profile_id_t DiscProfileIdList[7],
                                uint32_t DiscDuration
#if (defined RF4CE_CALLBACK_PARAM) || (defined DOXYGEN)
                                , FUNC_PTR confirm_cb
#endif
                               )

{
  uint8_t *msg_buf;
  uint8_t *ptr;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_discovery_confirm_cb = (nlme_discovery_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_DISCOVERY_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_DISCOVERY_REQUEST;
#if (UC3) || (SAM)
    ptr = (uint8_t *)&DstPANId;
    *msg_buf++ = *ptr++;
    *msg_buf++ = *ptr++;
#else
    *msg_buf++ = (uint8_t)DstPANId; //LSB
    *msg_buf++ = (uint8_t)(DstPANId >> 8); //MSB
#endif
#if (UC3) || (SAM)
    ptr = (uint8_t *)&DstNwkAddr;
    *msg_buf++ = *ptr++;
    *msg_buf++ = *ptr++;
#else
    *msg_buf++ = (uint8_t)DstNwkAddr; //LSB
    *msg_buf++ = (uint8_t)(DstNwkAddr >> 8); //MSB
#endif
    *msg_buf++ = OrgAppCapabilities;
// #ifndef UC3
   // ptr = (uint8_t *)OrgDevTypeList;
// #endif
   // for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = OrgDevTypeList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }  
// #ifndef UC3
   // ptr = (uint8_t *)OrgProfileIdList;
// #endif
   // for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = OrgProfileIdList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }
   for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
    {
        *msg_buf++ = OrgDevTypeList[i];
    }
   for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = OrgProfileIdList[i];
    }
   
   *msg_buf++ = SearchDevType;
   *msg_buf++ = DiscProfileIdListSize;
    for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = DiscProfileIdList[i];
    }
// #ifndef UC3
    // ptr = (uint8_t *)DiscProfileIdList;
// #endif
    // for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    // {
// #if (UC3)
        // *msg_buf++ = DiscProfileIdList[i];
// #else
        // *msg_buf++ = *ptr++;
// #endif
    // }
  memcpy(msg_buf, &DiscDuration, sizeof(uint32_t));
  msg_buf += sizeof(uint32_t);
  *msg_buf = EOT;
return true;
}


    bool nlme_discovery_response(nwk_enum_t Status, uint64_t DstIEEEAddr,
                                 uint8_t RecAppCapabilities, dev_type_t RecDevTypeList[3],
                                 profile_id_t RecProfileIdList[7], uint8_t DiscReqLQI)

{
  uint8_t *msg_buf;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
  *msg_buf++ = NLME_DISCOVERY_RESP_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_DISCOVERY_RESPONSE;
  *msg_buf++ = Status;
  memcpy(msg_buf, &DstIEEEAddr, sizeof(DstIEEEAddr));
  msg_buf += sizeof(uint64_t);
  *msg_buf++ = RecAppCapabilities;
// #ifndef UC3
   // ptr = (uint8_t *)RecDevTypeList;
// #endif
   // for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = RecDevTypeList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }  
// #ifndef UC3
   // ptr = (uint8_t *)RecProfileIdList;
// #endif
   // for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = RecProfileIdList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }
    for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
    {
        *msg_buf++ = RecDevTypeList[i];
    }
   for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = RecProfileIdList[i];
    }
   *msg_buf++ = DiscReqLQI;
   *msg_buf = EOT;
  return true;
  
}

/* The NLME-PAIR.request primitive allows the application to request the NLME pair
 * with another device. This primitive would normally be issued following a discovery
 * operation via the NLME-DISCOVERY.request primitive. */

bool nlme_pair_request(uint8_t LogicalChannel, uint16_t DstPANId, uint64_t DstIEEEAddr,
                       uint8_t OrgAppCapabilities, dev_type_t OrgDevTypeList[DEVICE_TYPE_LIST_SIZE],
                       profile_id_t OrgProfileIdList[PROFILE_ID_LIST_SIZE], uint8_t KeyExTransferCount
#ifdef RF4CE_CALLBACK_PARAM
                       , FUNC_PTR confirm_cb
#endif
                      )
{
  uint8_t *msg_buf;
  uint8_t *ptr;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_pair_confirm_cb = (nlme_pair_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_PAIR_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_PAIR_REQUEST;
  *msg_buf++ = LogicalChannel;
#if (UC3) || (SAM)
    ptr = (uint8_t *)&DstPANId;
    *msg_buf++ = *ptr++;
    *msg_buf++ = *ptr++;
#else
    *msg_buf++ = (uint8_t)DstPANId; //LSB
    *msg_buf++ = (uint8_t)(DstPANId >> 8); //MSB
#endif
  memcpy(msg_buf, &DstIEEEAddr, sizeof(DstIEEEAddr));
  msg_buf += sizeof(uint64_t);
  *msg_buf++ = OrgAppCapabilities;
// #ifndef UC3
   // ptr = (uint8_t *)OrgDevTypeList;
// #endif
   // for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = OrgDevTypeList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }  
// #ifndef UC3
   // ptr = (uint8_t *)OrgProfileIdList;
// #endif
   // for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = OrgProfileIdList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }
    for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
    {
        *msg_buf++ = OrgDevTypeList[i];
    }
   for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = OrgProfileIdList[i];
    }
   *msg_buf++ = KeyExTransferCount;
   *msg_buf = EOT;
return true;
  
}


    bool nlme_pair_response(nwk_enum_t Status, uint16_t DstPANId, uint64_t DstIEEEAddr,
                            uint8_t RecAppCapabilities, dev_type_t RecDevTypeList[3],
                            profile_id_t RecProfileIdList[7], uint8_t ProvPairingRef)

{
  uint8_t *msg_buf;
  uint8_t *ptr;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
  *msg_buf++ = NLME_PAIR_RESP_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_PAIR_RESPONSE;
  *msg_buf++ = Status;
#if (UC3) || (SAM)
    ptr = (uint8_t *)&DstPANId;
    *msg_buf++ = *ptr++;
    *msg_buf++ = *ptr++;
#else
    *msg_buf++ = (uint8_t)DstPANId; //LSB
    *msg_buf++ = (uint8_t)(DstPANId >> 8); //MSB
#endif
  memcpy(msg_buf, &DstIEEEAddr, sizeof(DstIEEEAddr));
  msg_buf += sizeof(uint64_t);
  *msg_buf++ = RecAppCapabilities;
// #ifndef UC3
   // ptr = (uint8_t *)RecDevTypeList;
// #endif
   // for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = RecDevTypeList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }  
// #ifndef UC3
   // ptr = (uint8_t *)RecProfileIdList;
// #endif
   // for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = RecProfileIdList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // } 
    for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
    {
        *msg_buf++ = RecDevTypeList[i];
    }
   for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = RecProfileIdList[i];
    }
   *msg_buf++ = ProvPairingRef;
   *msg_buf = EOT;
  return true;
  
}

/* The NLME-GET.request primitive allows the application to request the value of a
 * NIB attribute from the NLME. */

bool nlme_get_request(nib_attribute_t NIBAttribute, uint8_t NIBAttributeIndex
#ifdef RF4CE_CALLBACK_PARAM
                      , FUNC_PTR confirm_cb
#endif
                     )
{ 
  uint8_t *msg_buf;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_get_confirm_cb = (nlme_get_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_GET_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_GET_REQUEST;
  *msg_buf++ = NIBAttribute;
  *msg_buf++ = NIBAttributeIndex;
  *msg_buf = EOT;
return true;
  
}

/* The NLME-RX-ENABLE.request primitive allows the application to request that the
 * receiver is either enabled (for a finite period or until further notice) or disabled. */
bool nlme_rx_enable_request(uint32_t RxOnDuration
#ifdef RF4CE_CALLBACK_PARAM
                            , FUNC_PTR confirm_cb
#endif
                           )
{
  uint8_t *msg_buf;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_rx_enable_confirm_cb = (nlme_rx_enable_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_RX_ENABLE_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_RX_ENABLE_REQUEST;
  memcpy(msg_buf, &RxOnDuration, sizeof(RxOnDuration));
  msg_buf += sizeof(uint32_t);
  *msg_buf = EOT;
return true;
  
}
///////////check/////////

bool nlme_set_request(nib_attribute_t NIBAttribute, uint8_t NIBAttributeIndex, uint8_t *NIBAttributeValue
#ifdef RF4CE_CALLBACK_PARAM
                      , FUNC_PTR confirm_cb
#endif
                     )
{
  uint8_t *msg_buf;
  uint8_t i;
  uint8_t attr_len;
  uint8_t *ptr;
  ptr = NIBAttributeValue;
  msg_buf = get_next_tx_buffer();
  /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
  {
    return false;
  }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_set_confirm_cb = (nlme_set_confirm_cb_t)confirm_cb;
#endif
  attr_len = nwk_get_nib_attribute_size(NIBAttribute);
  *msg_buf++ = NLME_SET_REQ_LEN + RF4CONTROL_PID_LEN + attr_len;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_SET_REQUEST;
  *msg_buf++ = NIBAttribute;
  *msg_buf++ = NIBAttributeIndex;
  *msg_buf++ = attr_len;
  for (i = 0; i < attr_len; i++)
    {
        *msg_buf++ = *ptr++;
    }
  *msg_buf = EOT;
return true;
  
}
///////////////////////
/* The NLME-START.request primitive allows the application to request the NLME
 * start a network.*/
bool nlme_start_request(
#ifdef RF4CE_CALLBACK_PARAM
    FUNC_PTR confirm_cb)
#else
    void)
#endif
{
   uint8_t *msg_buf;
   msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_start_confirm_cb = (nlme_start_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_START_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_START_REQUEST;
  *msg_buf = EOT;
return true;
  
}
/* The NLME-UNPAIR.request primitive allows the application to request the NLME
 * removes a pairing link with another device both in the local and remote pairing tables. */

bool nlme_unpair_request(uint8_t PairingRef
#ifdef RF4CE_CALLBACK_PARAM
                         , FUNC_PTR confirm_cb
#endif
                        )
{
   uint8_t *msg_buf;
   msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_unpair_confirm_cb = (nlme_unpair_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_UNPAIR_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_UNPAIR_REQUEST;
  *msg_buf++ = PairingRef;
  *msg_buf = EOT;
return true;
}

/* The NLME-UNPAIR.response primitive allows the application to notify the NLME
 * that the pairing link indicated via the NLME-UNPAIR.indication primitive can be
 * removed from the pairing table. */
bool nlme_unpair_response(uint8_t PairingRef)
{
   uint8_t *msg_buf;
   msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
  *msg_buf++ = NLME_UNPAIR_RESP_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_UNPAIR_RESPONSE;
  *msg_buf++ = PairingRef;
  *msg_buf = EOT;
return true;
}

/* The NLME-UPDATE-KEY.request primitive allows the application to request the
 * NLME change the security link key of an entry in the pairing table. */

bool nlme_update_key_request(uint8_t PairingRef, uint8_t NewLinkKey[16]
#ifdef RF4CE_CALLBACK_PARAM
                             , FUNC_PTR confirm_cb
#endif
                            )
{
  uint8_t *msg_buf;
  uint8_t *ptr;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nlme_update_key_confirm_cb = (nlme_update_key_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NLME_UPDATE_KEY_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NLME_UPDATE_KEY_REQUEST;
  *msg_buf++ = PairingRef;
   ptr = NewLinkKey;
   for (i = 0; i < 16; i++)
    {
        *msg_buf++ = *ptr++;
    }
   *msg_buf = EOT;
return true;
  
  
}
/* The function configures the channel agility mode. */

bool nwk_ch_agility_request(nwk_agility_mode_t AgilityMode
#ifdef RF4CE_CALLBACK_PARAM
                            , FUNC_PTR confirm_cb
#endif
                           )
{
  uint8_t *msg_buf;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   nwk_ch_agility_confirm_cb = (nwk_ch_agility_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = NWK_CH_AGILITY_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = NWK_CH_AGILITY_REQUEST;
  *msg_buf++ = AgilityMode;
  *msg_buf = EOT;
return true;
  
}

bool pbp_org_pair_request(uint8_t OrgAppCapabilities,
                          dev_type_t OrgDevTypeList[DEVICE_TYPE_LIST_SIZE],
                          profile_id_t OrgProfileIdList[PROFILE_ID_LIST_SIZE],
                          dev_type_t SearchDevType, uint8_t DiscProfileIdListSize,
                          profile_id_t DiscProfileIdList[PROFILE_ID_LIST_SIZE]
#ifdef RF4CE_CALLBACK_PARAM
                          , FUNC_PTR confirm_cb
#endif
                         )
{
  uint8_t *msg_buf;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   pbp_org_pair_confirm_cb = (pbp_org_pair_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = PBP_ORG_PAIR_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = PBP_ORG_PAIR_REQUEST;
  *msg_buf++ = OrgAppCapabilities;
//#ifndef UC3
//   ptr = (uint8_t *)OrgDevTypeList;
//#endif
//   for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
//   {
//#if (UC3)
//    *msg_buf++ = OrgDevTypeList[i];
//#else
//    *msg_buf++ = *ptr++;
//#endif
//   } 
//#ifndef UC3
//   ptr = (uint8_t *)OrgProfileIdList;
//#endif
//   for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
//   {
//#if (UC3)
//    *msg_buf++ = OrgProfileIdList[i];
//#else
//    *msg_buf++ = *ptr++;
//#endif
//   }
   for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
    {
        *msg_buf++ = OrgDevTypeList[i];
    }
   for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = OrgProfileIdList[i];
    }
   
   *msg_buf++ = SearchDevType;
   *msg_buf++ = DiscProfileIdListSize;
// #ifndef UC3
   // ptr = (uint8_t *)DiscProfileIdList;
// #endif
   // for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = DiscProfileIdList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }
    for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = DiscProfileIdList[i];
    }
   *msg_buf =EOT;
return true;
   
}

bool pbp_rec_pair_request(uint8_t RecAppCapabilities,
                          dev_type_t RecDevTypeList[3],
                          profile_id_t RecProfileIdList[7]
#if (defined RF4CE_CALLBACK_PARAM) || (defined DOXYGEN)
                          , FUNC_PTR confirm_cb
#endif
                         )

{
  uint8_t *msg_buf;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   pbp_rec_pair_confirm_cb = (pbp_rec_pair_confirm_cb_t)confirm_cb;
#endif
 
  *msg_buf++ = PBP_REC_PAIR_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = PBP_REC_PAIR_REQUEST;
  *msg_buf++ = RecAppCapabilities;
// #ifndef UC3
   // ptr = (uint8_t *)RecDevTypeList;
// #endif
   // for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = RecDevTypeList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // } 
// #ifndef UC3
   // ptr = (uint8_t *)RecProfileIdList;
// #endif
   // for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
   // {
// #if (UC3)
    // *msg_buf++ = RecProfileIdList[i];
// #else
    // *msg_buf++ = *ptr++;
// #endif
   // }
   for (i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
    {
        *msg_buf++ = RecDevTypeList[i];
    }
   for (i = 0; i < PROFILE_ID_LIST_SIZE; i++)
    {
        *msg_buf++ = RecProfileIdList[i];
    }
   *msg_buf = EOT;
return true;
}





/**
 * @brief Handles the key press from the application.
 *
 * @param PairingRef        Pairing reference
 * @param VendorId         Vendor Id information
 * @param CmdCode          ZRC cmd code (Press/Release)
 * @param CmdLength        Length of the rc cmd (rc cmd code + Payload).
 * @param Cmd              Actual rc command code (rc cmd code + Payload)
 * @param TxOptions        Transmission options (ex- ack, security etc)
 *
 * @return true            returns the status of zrc_cmd_request.
 */
bool zrc_cmd_request(uint8_t PairingRef, uint16_t VendorId, zrc_cmd_code_t CmdCode,
                     uint8_t CmdLength, uint8_t *Cmd, uint8_t TxOptions
#ifdef RF4CE_CALLBACK_PARAM
                     , FUNC_PTR confirm_cb
#endif
                    )
{
  uint8_t *msg_buf;
  uint8_t *ptr;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   zrc_cmd_confirm_cb = (zrc_cmd_confirm_cb_t)confirm_cb;
#endif
 
  *msg_buf++ = ZRC_CMD_REQ_LEN + RF4CONTROL_PID_LEN + CmdLength+1;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = ZRC_CMD_REQUEST;
  *msg_buf++ = PairingRef;
#if (UC3) || (SAM)
    ptr = (uint8_t *)&VendorId;
    *msg_buf++ = *ptr++;
    *msg_buf++ = *ptr++;
#else
    *msg_buf++ = (uint8_t)VendorId; //LSB
    *msg_buf++ = (uint8_t)(VendorId >> 8); //MSB
#endif
    *msg_buf++ = CmdCode;
    *msg_buf++ = TxOptions;
    *msg_buf++ = CmdLength;
     ptr = Cmd;
    // @ToDo use memcpy
    for (i = 0; i < CmdLength; i++)
    {
        *msg_buf++ = *ptr++;
    }
    
    *msg_buf = EOT;
return true;
} 


/**
 * @brief Handles the cmd discovery request.
 *
 * @param PairingRef    pairing ref of the originator device.
 *
 * @return true         if cmd discovery is sent using nlde_data_req.
 */
bool zrc_cmd_disc_request(uint8_t PairingRef
#ifdef RF4CE_CALLBACK_PARAM
                          , FUNC_PTR confirm_cb
#endif
                         )
{
  uint8_t *msg_buf;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
#ifdef RF4CE_CALLBACK_PARAM
   if (confirm_cb == NULL)
   {
        return false;
   }
   zrc_cmd_disc_confirm_cb = (zrc_cmd_disc_confirm_cb_t)confirm_cb;
#endif
  *msg_buf++ = ZRC_CMD_DISCOVERY_REQ_LEN + RF4CONTROL_PID_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = ZRC_CMD_DISCOVERY_REQUEST;
  *msg_buf++ = PairingRef;
  *msg_buf = EOT;
return true;
}


/**
 * @brief Send out the cmd discovery response.
 *
 * @param PairingRef    pairing ref of the originator device.
 * @param SupportedCmd  Address of provided 32 byte array containing supported command codes
 *
 * @return true         if cmd discovery response is sent using nlde_data_req.
 */
bool zrc_cmd_disc_response(uint8_t PairingRef, uint8_t *SupportedCmd)
{
  uint8_t *msg_buf;
  uint8_t i;
   msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
  *msg_buf++ = ZRC_CMD_DISCOVERY_RESP_LEN;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = ZRC_CMD_DISCOVERY_RESPONSE;
  *msg_buf++ = PairingRef;
  for (i = 0; i < 32; i++)
    {
        *msg_buf++ = *SupportedCmd++;
    }
  *msg_buf = EOT;
return true;
}
bool vendor_data_request(uint8_t PairingRef, profile_id_t ProfileId,
                         uint16_t VendorId, uint8_t nsduLength, uint8_t *nsdu,
                         uint8_t TxOptions)
{
   uint8_t *msg_buf;
  uint8_t *ptr;
  uint8_t i;
  msg_buf = get_next_tx_buffer();

    /* Check if buffer could not be allocated */
  if (NULL == msg_buf)
    {
        return false;
    }
  *msg_buf++ = VENDOR_DATA_REQ_LEN + RF4CONTROL_PID_LEN +nsduLength ;
  *msg_buf++ = RF4CONTROL_PID;
  *msg_buf++ = VENDOR_DATA_REQUEST;
  *msg_buf++ = PairingRef;
  *msg_buf++ = ProfileId;
#if (UC3) || (SAM)
    ptr = (uint8_t *)&VendorId;
    *msg_buf++ = *ptr++;
    *msg_buf++ = *ptr++;
#else
    *msg_buf++ = (uint8_t)VendorId; //LSB
    *msg_buf++ = (uint8_t)(VendorId >> 8); //MSB
#endif
    *msg_buf++ = TxOptions;
    *msg_buf++ = nsduLength;
     ptr = nsdu;
    // @ToDo use memcpy
    for (i = 0; i < nsduLength; i++)
    {
        *msg_buf++ = *ptr++;
    }
    
    *msg_buf = EOT;
return true;
  
}
#ifdef RF4CE_CALLBACK_PARAM
void register_nwk_indication_callback(nwk_indication_callback_t *nwk_ind_cb)
{
#if (defined CHANNEL_AGILITY) || (defined RF4CE_PLATFORM)
    if (nwk_ind_cb->nwk_ch_agility_indication_cb != NULL)
    {
        nwk_ind_callback.nwk_ch_agility_indication_cb = nwk_ind_cb->nwk_ch_agility_indication_cb;
    }
#endif
#if (defined RF4CE_TARGET) || (defined RF4CE_PLATFORM)
    if (nwk_ind_cb->nlme_pair_indication_cb != NULL)
    {
        nwk_ind_callback.nlme_pair_indication_cb = nwk_ind_cb->nlme_pair_indication_cb;
    }
#endif
#if (NWK_UNPAIR_IND_RESP == 1) || (defined RF4CE_PLATFORM)
    if (nwk_ind_cb->nlme_unpair_indication_cb != NULL)
    {
        nwk_ind_callback.nlme_unpair_indication_cb = nwk_ind_cb->nlme_unpair_indication_cb;
    }
#endif
#if (((defined RF4CE_TARGET) && (NWK_DISCOVERY_IND_RESP == 1)) || (defined RF4CE_PLATFORM))
    if (nwk_ind_cb->nlme_discovery_indication_cb != NULL)
    {
        nwk_ind_callback.nlme_discovery_indication_cb = nwk_ind_cb->nlme_discovery_indication_cb;
    }
#endif
#if (defined RF4CE_TARGET) || (defined RF4CE_PLATFORM)
    if ( nwk_ind_cb->nlme_comm_status_indication_cb != NULL)
    {
        nwk_ind_callback.nlme_comm_status_indication_cb = nwk_ind_cb->nlme_comm_status_indication_cb;
    }
#endif
#ifdef ZRC_PROFILE
    if (nwk_ind_cb->zrc_data_indication_cb != NULL)
    {
        nwk_ind_callback.zrc_data_indication_cb = nwk_ind_cb->zrc_data_indication_cb;
    }

#endif
    if (nwk_ind_cb->nlde_data_indication_cb != NULL)
    {
        nwk_ind_callback.nlde_data_indication_cb = nwk_ind_cb->nlde_data_indication_cb;
    }
}
#endif /* RF4CE_CALLBACK_PARAM */
       
#ifdef RF4CE_CALLBACK_PARAM
void register_zrc_indication_callback(zrc_indication_callback_t *zrc_ind_callback)
{
#if (defined RF4CE_TARGET) || (defined ZRC_CMD_DISCOVERY)
    if (zrc_ind_callback->zrc_cmd_indication_cb != NULL)
    {
        zrc_ind_cb.zrc_cmd_indication_cb = zrc_ind_callback->zrc_cmd_indication_cb;
    }
#endif
#if (defined ZRC_CMD_DISCOVERY)
    if (zrc_ind_callback->zrc_cmd_disc_indication_cb != NULL)
    {
        zrc_ind_cb.zrc_cmd_disc_indication_cb = zrc_ind_callback->zrc_cmd_disc_indication_cb;
    }
#endif
#ifdef VENDOR_DATA
    if (zrc_ind_callback->vendor_data_ind_cb != NULL)
    {
        zrc_ind_cb.vendor_data_ind_cb = zrc_ind_callback->vendor_data_ind_cb;
    }
#endif

//#ifdef PBP_REC
//    pb_pairing_rec_ind_callback_init();
//#endif
    /* This will register zrc_data_ind call back to network layer */
 }
#endif /* RF4CE_CALLBACK_PARAM */      


            
uint8_t nwk_get_nib_attribute_size(uint8_t nib_attribute_id)
{
    return GET_NIB_ATTR_SIZE(nib_attribute_id);
}
            

#endif /*SIO_HUB*/