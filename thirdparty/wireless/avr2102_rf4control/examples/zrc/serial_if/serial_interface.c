/**
 * @file serial_interface.c
 *
 * @brief RF4CE Serial Interface Implementation
 *
 * $Id: serial_interface.c 32661 2012-07-16 12:54:00Z agasthian.s $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifdef SIO_HUB

/* === INCLUDES ============================================================ */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "pal.h"
#include "app_config.h"
#include "nwk_msg_code.h"
#include "serial_interface.h"
#include "sio_helper.h"
#include "rf4ce.h"
#ifdef ZRC_PROFILE
#include "pb_pairing.h"
#include "zrc.h"
#endif
#ifdef VENDOR_DATA
#include "vendor_data.h"
#endif

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
 * A UART state that expects the protocol id  to be received as the next character.
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

#define SIO_RX_BUF_SIZE                 (200)
#define SIO_TX_BUF_SIZE                 SIO_RX_BUF_SIZE

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
static uint8_t sio_rx_buf[SIO_RX_BUF_SIZE];

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
static uint8_t data[SIO_RX_BUF_SIZE];
static uint8_t data_length = 0;
static uint8_t rx_index = 0;
static uint8_t head = 0, buf_count = 0;

static uint8_t protocol_id;

#ifdef RF4CE_CALLBACK_PARAM
zrc_indication_callback_t zrc_ind;
#endif

/* === Prototypes ========================================================== */

static inline void process_incoming_sio_data(void);
static uint8_t *get_next_tx_buffer(void);
static inline void handle_incoming_msg(void);

#ifdef RF4CE_CALLBACK_PARAM
static void nlde_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t OrgProfile
#ifdef NLDE_HANDLE
                              , uint8_t Handle
#endif
                             );

static void nlme_reset_confirm(nwk_enum_t Status);
static void nlme_rx_enable_confirm(nwk_enum_t Status);
static void nlme_start_confirm(nwk_enum_t Status);
static void nlme_set_confirm(nwk_enum_t Status, nib_attribute_t NIBAttribute, uint8_t NIBAttributeIndex);
static void nlme_get_confirm(nwk_enum_t Status, nib_attribute_t NIBAttribute,
                             uint8_t NIBAttributeIndex, void *NIBAttributeValue);
#if (NWK_UNPAIR_REQ_CONF == 1) || (defined RF4CE_PLATFORM)
static void nlme_unpair_confirm(uint8_t Status, uint8_t PairingRef);
#endif
#if ((NWK_UPDATE_KEY == 1) && (defined RF4CE_SECURITY)) || (defined RF4CE_PLATFORM)
static void nlme_update_key_confirm(nwk_enum_t Status, uint8_t PairingRef);
#endif
#if (defined CHANNEL_AGILITY) || (defined RF4CE_PLATFORM)
static void nwk_ch_agility_confirm(nwk_enum_t Status, bool ChannelChanged, uint8_t LogicalChannel);
#endif
#ifdef PBP_ORG
static void pbp_org_pair_confirm(nwk_enum_t Status, uint8_t PairingRef);
#endif
#ifdef PBP_REC
static void pbp_rec_pair_confirm(nwk_enum_t Status, uint8_t PairingRef);
#endif
#if ((defined ZRC_PROFILE) && (!defined RF4CE_TARGET))
static void zrc_cmd_confirm(nwk_enum_t Status, uint8_t PairingRef, cec_code_t RcCmd);
#endif
#ifdef VENDOR_DATA
static void vendor_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t ProfileId
#ifdef NLDE_HANDLE
                                , uint8_t Handle
#endif
                               );
#endif
#ifdef ZRC_CMD_DISCOVERY
static void zrc_cmd_disc_confirm(nwk_enum_t Status, uint8_t PairingRef, uint8_t *SupportedCmd);
#endif
void zrc_indication_callback_init(void);


#if (defined ZRC_CMD_DISCOVERY)
static void zrc_cmd_disc_indication(uint8_t PairingRef);
#endif
#if (defined RF4CE_TARGET)
static void zrc_cmd_indication(uint8_t PairingRef, uint8_t nsduLength, uint8_t *nsdu,
                               uint8_t RxLinkQuality, uint8_t RxFlags);
#endif
#ifdef VENDOR_DATA
static void vendor_data_ind(uint8_t PairingRef, uint16_t VendorId,
                            uint8_t nsduLength, uint8_t *nsdu, uint8_t RxLinkQuality,
                            uint8_t RxFlags);
#endif
#endif /* RF4CE_CALLBACK_PARAM */
/* === Implementation ====================================================== */

#ifdef RF4CE_CALLBACK_PARAM
void zrc_indication_callback_init(void)
{
#if (defined RF4CE_TARGET)
    zrc_ind.zrc_cmd_indication_cb = zrc_cmd_indication;
#endif
#if (defined ZRC_CMD_DISCOVERY)
    zrc_ind.zrc_cmd_disc_indication_cb = zrc_cmd_disc_indication;
#endif
#ifdef VENDOR_DATA
    zrc_ind.vendor_data_ind_cb = vendor_data_ind;
#endif
    register_zrc_indication_callback(&zrc_ind);
}
#endif
/**
 * @brief This function does the initialization of the SIO or UART.
 */
void serial_interface_init(void)
{
    sio_rx_state = UART_RX_STATE_SOT;
	sio_init();
}


/**
 * @brief Function to handle the state machine serial data exchange.
 */
void serial_data_handler(void)
{
    /* Rx processing */
    if (data_length == 0)
    {
        /* No data to process, read the stream IO */
        rx_index = 0;
        data_length = sio_rx(data, SIO_RX_BUF_SIZE);
    }
    else    /* Data has been received, process the data */
    {
        /* Process each single byte */
        process_incoming_sio_data();
        data_length--;
        rx_index++;
    }

    /* Tx processing */
    if (buf_count != 0)
    {
        if (sio_tx(sio_tx_buf[head], (sio_tx_buf[head][1] + 3)) != 0)
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
}


/*
 * @brief Process data received from SIO
 */
static inline void process_incoming_sio_data(void)
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
    bool ret_val;

    switch (sio_rx_buf[1])  /* message type */
    {
        case NLDE_DATA_REQUEST:
            /* Attention: The TxOption field is moved before the nsduLength field! */
            ret_val = nlde_data_request(sio_rx_buf[2], (profile_id_t)sio_rx_buf[3],
                                        ((uint16_t)sio_rx_buf[4] | ((uint16_t)sio_rx_buf[5] << 8)),    /* VendorId */
                                        sio_rx_buf[7],     /* nsduLength */
                                        &sio_rx_buf[8],
                                        sio_rx_buf[6]     /* TxOptions */
#ifdef NLDE_HANDLE
                                        , 1
#endif
#ifdef RF4CE_CALLBACK_PARAM
                                        ,  (FUNC_PTR)nlde_data_confirm
#endif
                                       );
            break;

        case NLME_RESET_REQUEST:
            ret_val = nlme_reset_request(sio_rx_buf[2]
#ifdef RF4CE_CALLBACK_PARAM
                                         ,  (FUNC_PTR)nlme_reset_confirm
#endif
                                        );
            break;

#if (NWK_GET == 1)
        case NLME_GET_REQUEST:
            ret_val = nlme_get_request((nib_attribute_t)sio_rx_buf[2], sio_rx_buf[3]
#ifdef RF4CE_CALLBACK_PARAM
                                       ,  (FUNC_PTR)nlme_get_confirm
#endif
                                      );
            break;
#endif
        case NLME_RX_ENABLE_REQUEST:
#if (UC3) || (SAM)
            {
                uint32_t rx_on_duration;
                MEMCPY_ENDIAN(&rx_on_duration, &sio_rx_buf[2], sizeof(uint32_t));
                ret_val = nlme_rx_enable_request(rx_on_duration
#ifdef RF4CE_CALLBACK_PARAM
                                                 ,  (FUNC_PTR)nlme_rx_enable_confirm
#endif
                                                );
            }
#else
            ret_val = nlme_rx_enable_request(*(uint32_t *)&sio_rx_buf[2]
#ifdef RF4CE_CALLBACK_PARAM
                                             ,  (FUNC_PTR)nlme_rx_enable_confirm
#endif
                                            );
#endif
            break;
#if (NWK_SET == 1)
        case NLME_SET_REQUEST:
            ret_val = nlme_set_request((nib_attribute_t)sio_rx_buf[2], sio_rx_buf[3], &sio_rx_buf[5]
#ifdef RF4CE_CALLBACK_PARAM
                                       ,  (FUNC_PTR)nlme_set_confirm
#endif
                                      );
            break;
#endif
        case NLME_START_REQUEST:
            ret_val = nlme_start_request(
#ifdef RF4CE_CALLBACK_PARAM
                          (FUNC_PTR)nlme_start_confirm
#endif
                      );
            break;
#if (NWK_UNPAIR_REQ_CONF == 1)
        case NLME_UNPAIR_REQUEST:
            ret_val = nlme_unpair_request(sio_rx_buf[2]
#ifdef RF4CE_CALLBACK_PARAM
                                          ,  (FUNC_PTR)nlme_unpair_confirm
#endif
                                         );
            break;
#endif
#if (NWK_UNPAIR_IND_RESP == 1)
        case NLME_UNPAIR_RESPONSE:
            ret_val = nlme_unpair_response(sio_rx_buf[2]);
            break;
#endif
#if ((NWK_UPDATE_KEY == 1) && (defined RF4CE_SECURITY)) || (defined RF4CE_PLATFORM)
        case NLME_UPDATE_KEY_REQUEST:
            ret_val = nlme_update_key_request(sio_rx_buf[2], &sio_rx_buf[3]
#ifdef RF4CE_CALLBACK_PARAM
                                              ,  (FUNC_PTR)nlme_update_key_confirm
#endif
                                             );
            break;
#endif
#if (defined CHANNEL_AGILITY) || (defined RF4CE_PLATFORM)
        case NWK_CH_AGILITY_REQUEST:
            ret_val = nwk_ch_agility_request((nwk_agility_mode_t)sio_rx_buf[2]
#ifdef RF4CE_CALLBACK_PARAM
                                             ,  (FUNC_PTR)nwk_ch_agility_confirm
#endif
                                            );
            break;
#endif

#ifdef PBP_ORG
        case PBP_ORG_PAIR_REQUEST:
#if (UC3) 
            {
                dev_type_t OrgDevTypeList[DEVICE_TYPE_LIST_SIZE];
                profile_id_t OrgProfileIdList[PROFILE_ID_LIST_SIZE];
                profile_id_t DiscProfileIdList[PROFILE_ID_LIST_SIZE];

                for (uint8_t i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
                {
                    OrgDevTypeList[i] = (dev_type_t)sio_rx_buf[3 + i];
                }
                for (uint8_t i = 0; i < PROFILE_ID_LIST_SIZE; i++)
                {
                    OrgProfileIdList[i] = (profile_id_t)sio_rx_buf[6 + i];
                }
                for (uint8_t i = 0; i < PROFILE_ID_LIST_SIZE; i++)
                {
                    DiscProfileIdList[i] = (profile_id_t)sio_rx_buf[13 + 2 + i];
                }

                ret_val = pbp_org_pair_request(sio_rx_buf[2], // OrgAppCapabilities
                                               OrgDevTypeList,     //OrgDevTypeList
                                               OrgProfileIdList,   //OrgProfileIdList
                                               (dev_type_t)sio_rx_buf[13],     // SearchDevType
                                               sio_rx_buf[14],     //DiscProfileIdListSize
                                               DiscProfileIdList//DiscProfileIdList,
#ifdef RF4CE_CALLBACK_PARAM
                                               ,  (FUNC_PTR)pbp_org_pair_confirm
#endif
                                              );
            }
#else

            ret_val = pbp_org_pair_request(sio_rx_buf[2],
                                           (dev_type_t *)&sio_rx_buf[3],
                                           (profile_id_t *)&sio_rx_buf[3 + DEVICE_TYPE_LIST_SIZE],
                                           (dev_type_t) sio_rx_buf[3 +  DEVICE_TYPE_LIST_SIZE + PROFILE_ID_LIST_SIZE],
                                           sio_rx_buf[4 + DEVICE_TYPE_LIST_SIZE + PROFILE_ID_LIST_SIZE],
                                           (profile_id_t *)&sio_rx_buf[5 + DEVICE_TYPE_LIST_SIZE + PROFILE_ID_LIST_SIZE]
#ifdef RF4CE_CALLBACK_PARAM
                                           ,  (FUNC_PTR)pbp_org_pair_confirm
#endif
                                          );
#endif
            break;
#endif

#ifdef PBP_REC
        case PBP_REC_PAIR_REQUEST:
#if (UC3) || (SAM)
            {
                dev_type_t RecDevTypeList[DEVICE_TYPE_LIST_SIZE];
                profile_id_t RecProfileIdList[PROFILE_ID_LIST_SIZE];

                for (uint8_t i = 0; i < DEVICE_TYPE_LIST_SIZE; i++)
                {
                    RecDevTypeList[i] = (dev_type_t)sio_rx_buf[3 + i];
                }
                for (uint8_t i = 0; i < PROFILE_ID_LIST_SIZE; i++)
                {
                    RecProfileIdList[i] = (profile_id_t)sio_rx_buf[6 + i];
                }
                ret_val = pbp_rec_pair_request(sio_rx_buf[2], // RecAppCapabilities
                                               RecDevTypeList,//(dev_type_t *)&sio_rx_buf[3],
                                               RecProfileIdList//(profile_id_t *)&sio_rx_buf[3 + 3],
#ifdef RF4CE_CALLBACK_PARAM
                                               ,  (FUNC_PTR)pbp_rec_pair_confirm
#endif
                                              );

            }
#else
            ret_val = pbp_rec_pair_request(sio_rx_buf[2] /*uint8_t RecAppCapabilities*/,
                                           (dev_type_t *)&sio_rx_buf[3]/*dev_type_t RecDevTypeList[DEVICE_TYPE_LIST_SIZE]*/,
                                           (profile_id_t *)&sio_rx_buf[3 + 3]/*profile_id_t RecProfileIdList[PROFILE_ID_LIST_SIZE]*/
#ifdef RF4CE_CALLBACK_PARAM
                                           ,  (FUNC_PTR)pbp_rec_pair_confirm
#endif
                                          );
#endif
            break;
#endif

#ifdef ZRC_PROFILE
#if ((!defined RF4CE_TARGET) || (defined RF4CE_PLATFORM))
        case ZRC_CMD_REQUEST:
            ret_val = zrc_cmd_request(sio_rx_buf[2], /* pair_ref */
                                      (uint16_t)sio_rx_buf[3] | ((uint16_t)sio_rx_buf[4] << 8), /*uint16_t VendorId,*/
                                      (zrc_cmd_code_t)sio_rx_buf[5],  /* zrc_cmd_code_t cerc_cmd_code, */
                                      sio_rx_buf[7], /* uint8_t cmd_length,*/
                                      &sio_rx_buf[8], /*uint8_t *cmd_payload, */
                                      sio_rx_buf[6] /* uint8_t tx_options */
#ifdef RF4CE_CALLBACK_PARAM
                                      ,  (FUNC_PTR)zrc_cmd_confirm
#endif
                                     );
            break;
#endif
#endif  /* #ifdef ZRC_PROFILE */

#ifdef ZRC_CMD_DISCOVERY
        case ZRC_CMD_DISCOVERY_REQUEST:
            ret_val = zrc_cmd_disc_request(sio_rx_buf[2]  //PairingRef
#ifdef RF4CE_CALLBACK_PARAM
                                           ,  (FUNC_PTR)zrc_cmd_disc_confirm
#endif
                                          );
            break;
#endif

#ifdef ZRC_CMD_DISCOVERY
        case ZRC_CMD_DISCOVERY_RESPONSE:
            ret_val = zrc_cmd_disc_response(sio_rx_buf[2], &sio_rx_buf[3]);
            break;
#endif

#ifdef VENDOR_DATA
        case VENDOR_DATA_REQUEST:
            ret_val = nlde_data_request(sio_rx_buf[2], /*uint8_t PairingRef,*/
                                        PROFILE_ID_ZRC,
                                        (uint16_t)sio_rx_buf[4] | ((uint16_t)sio_rx_buf[5] << 8), /*uint16_t VendorId,*/
                                        sio_rx_buf[7], /*uint8_t nsduLength,*/
                                        &sio_rx_buf[8], /*uint8_t *nsdu,*/
                                        sio_rx_buf[6] /*uint8_t TxOptions*/
#ifdef NLDE_HANDLE
                                        , 1
#endif
#ifdef RF4CE_CALLBACK_PARAM
                                        ,  (FUNC_PTR)vendor_data_confirm
#endif
                                       );
            break;
#endif
        default:
            {
                /* Sending the unsupported command response on receiving the invalid command - For ASF support */
                //uint8_t *msg_buf;
                //msg_buf = get_next_tx_buffer();
                //*msg_buf++ = UNSUPPORTED_CMD_LEN;
                //*msg_buf++ = UNSUPPORTED_CMD;
                //*msg_buf = EOT;
            }
            Assert("???" == 0);
            break;
    }

    if (ret_val == false)
    {
        Assert("Buffer is not available (Test harness)" == 0);
    }
}




/* Network primitives callbacks*/



#ifndef ZRC_PROFILE
void nlde_data_indication(uint8_t PairingRef, profile_id_t ProfileId,
                          uint16_t VendorId, uint8_t nsduLength, uint8_t *nsdu,
                          uint8_t RxLinkQuality, uint8_t RxFlags)
{
    uint8_t *msg_buf;
    uint8_t *nsdu_ptr;
    uint8_t i;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLDE_DATA_IND_LEN + nsduLength + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLDE_DATA_INDICATION;
    *msg_buf++ = PairingRef;
    *msg_buf++ = ProfileId;
    *msg_buf++ = (uint8_t)VendorId; // LSB
    *msg_buf++ = (uint8_t)(VendorId >> 8); // MSB
    *msg_buf++ = RxLinkQuality;
    *msg_buf++ = RxFlags;
    *msg_buf++ = nsduLength;
    nsdu_ptr = nsdu;
    // @ToDo use memcpy
    for (i = 0; i < nsduLength; i++)
    {
        *msg_buf++ = *nsdu_ptr++;
    }
    *msg_buf = EOT;
}
#endif

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlde_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t OrgProfile
#ifdef NLDE_HANDLE
                       , uint8_t Handle
#endif
                      )
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLDE_DATA_CONF_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLDE_DATA_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = PairingRef;
    *msg_buf++ = OrgProfile;
    *msg_buf = EOT;
    /* Keeps compiler happy */
#ifdef NLDE_HANDLE
    Handle = Handle;
#endif
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_reset_confirm(nwk_enum_t Status)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLME_RESET_CONF_LEN + RF4CONTROL_PID_LEN;
    *msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLME_RESET_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf = EOT;
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_rx_enable_confirm(nwk_enum_t Status)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLME_RX_ENABLE_CONF_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLME_RX_ENABLE_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf = EOT;
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_set_confirm(nwk_enum_t Status, nib_attribute_t NIBAttribute, uint8_t NIBAttributeIndex)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLME_SET_CONF_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLME_SET_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = NIBAttribute;
    *msg_buf++ = NIBAttributeIndex;
    *msg_buf = EOT;
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_get_confirm(nwk_enum_t Status, nib_attribute_t NIBAttribute,
                      uint8_t NIBAttributeIndex, void *NIBAttributeValue)
{
    uint8_t *msg_buf;
    uint8_t i;
    uint8_t attr_len;

    uint8_t *ptr;
    ptr = NIBAttributeValue;


    msg_buf = get_next_tx_buffer();
    attr_len = nwk_get_nib_attribute_size(NIBAttribute);
    *msg_buf++ = NLME_GET_CONF_LEN + attr_len + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLME_GET_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = NIBAttribute;
    *msg_buf++ = NIBAttributeIndex;
    *msg_buf++ = attr_len;
    for (i = 0; i < attr_len; i++)
    {
        *msg_buf++ = *ptr++;
    }
    *msg_buf = EOT;
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_start_confirm(nwk_enum_t Status)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLME_START_CONF_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLME_START_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf = EOT;
}

#if (!defined RF4CE_CALLBACK_PARAM)
#if (NWK_UNPAIR_IND_RESP == 1) || (defined RF4CE_PLATFORM)
void nlme_unpair_indication(uint8_t PairingRef)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLME_UNPAIR_IND_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLME_UNPAIR_INDICATION;
    *msg_buf++ = PairingRef;
    *msg_buf = EOT;
}
#endif
#endif


#if (NWK_UNPAIR_REQ_CONF == 1) || (defined RF4CE_PLATFORM)
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_unpair_confirm(uint8_t Status, uint8_t PairingRef)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLME_UNPAIR_CONF_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLME_UNPAIR_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = PairingRef;
    *msg_buf = EOT;
}
#endif


#if ((NWK_UPDATE_KEY == 1) && (defined RF4CE_SECURITY)) || (defined RF4CE_PLATFORM)
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_update_key_confirm(nwk_enum_t Status, uint8_t PairingRef)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLME_UPDATE_KEY_CONF_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NLME_UPDATE_KEY_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = PairingRef;
    *msg_buf = EOT;
}
#endif

#if (!defined RF4CE_CALLBACK_PARAM)
#if (defined CHANNEL_AGILITY) || (defined RF4CE_PLATFORM)
void nwk_ch_agility_indication(uint8_t LogicalChannel)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NWK_CH_AGILITY_IND_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NWK_CH_AGILITY_INDICATION;
    *msg_buf++ = LogicalChannel;
    *msg_buf = EOT;
}
#endif
#endif


#if (defined CHANNEL_AGILITY) || (defined RF4CE_PLATFORM)
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nwk_ch_agility_confirm(nwk_enum_t Status, bool ChannelChanged, uint8_t LogicalChannel)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NWK_CH_AGILITY_CONF_LEN + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = NWK_CH_AGILITY_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = (uint8_t)ChannelChanged;
    *msg_buf++ = LogicalChannel;
    *msg_buf = EOT;
}
#endif


#ifdef PBP_ORG
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void pbp_org_pair_confirm(nwk_enum_t Status, uint8_t PairingRef)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = 3 + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = PBP_ORG_PAIR_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = PairingRef;
    *msg_buf = EOT;
}
#endif


#ifdef PBP_REC
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void pbp_rec_pair_confirm(nwk_enum_t Status, uint8_t PairingRef)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = 3 + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = PBP_REC_PAIR_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = PairingRef;
    *msg_buf = EOT;
}
#endif


#if ((defined ZRC_PROFILE) && (!defined RF4CE_TARGET))
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zrc_cmd_confirm(nwk_enum_t Status, uint8_t PairingRef,
                     cec_code_t RcCmd)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = 4 + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = ZRC_CMD_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = PairingRef;
    *msg_buf++ = RcCmd;
    *msg_buf = EOT;
}
#endif


#if ((defined ZRC_PROFILE) && (defined RF4CE_TARGET))
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zrc_cmd_indication(uint8_t PairingRef, uint8_t nsduLength, uint8_t *nsdu,
                        uint8_t RxLinkQuality, uint8_t RxFlags)
{
    uint8_t *msg_buf;
    uint8_t *nsdu_ptr;
    uint8_t i;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = NLDE_DATA_IND_LEN + nsduLength - 3 + RF4CONTROL_PID_LEN;
    *msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = ZRC_CMD_INDICATION;
    *msg_buf++ = PairingRef;
    *msg_buf++ = RxLinkQuality;
    *msg_buf++ = RxFlags;
    *msg_buf++ = nsduLength;
    nsdu_ptr = nsdu;
    // @ToDo use memcpy
    for (i = 0; i < nsduLength; i++)
    {
        *msg_buf++ = *nsdu_ptr++;
    }
    *msg_buf = EOT;
}
#endif


#ifdef VENDOR_DATA
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void vendor_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t ProfileId
#ifdef NLDE_HANDLE
                         , uint8_t Handle
#endif
                        )
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = 3 + RF4CONTROL_PID_LEN;
	*msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = VENDOR_DATA_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = PairingRef;
    *msg_buf = EOT;
    /* Keeps compiler Happy */
    ProfileId = ProfileId;
#ifdef NLDE_HANDLE
    Handle = Handle;
#endif
}
#endif


#ifdef VENDOR_DATA
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void vendor_data_ind(uint8_t PairingRef, uint16_t VendorId,
                     uint8_t nsduLength, uint8_t *nsdu, uint8_t RxLinkQuality,
                     uint8_t RxFlags)
{
    uint8_t *msg_buf;
    uint8_t *nsdu_ptr;
    uint8_t i;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = 8 + nsduLength + RF4CONTROL_PID_LEN;
    *msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = VENDOR_DATA_INDICATION;
    *msg_buf++ = PairingRef;
    *msg_buf++ = PROFILE_ID_ZRC;
    *msg_buf++ = (uint8_t)(VendorId & 0xFF);
    *msg_buf++ = (uint8_t)(VendorId >> 8);
    *msg_buf++ = RxLinkQuality;
    *msg_buf++ = RxFlags;
    *msg_buf++ = nsduLength;
    nsdu_ptr = nsdu;
    // @ToDo use memcpy
    for (i = 0; i < nsduLength; i++)
    {
        *msg_buf++ = *nsdu_ptr++;
    }
    *msg_buf = EOT;
}
#endif


#ifdef ZRC_CMD_DISCOVERY
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zrc_cmd_disc_confirm(nwk_enum_t Status, uint8_t PairingRef, uint8_t *SupportedCmd)
{
    uint8_t *msg_buf;
    uint8_t i;

    msg_buf = get_next_tx_buffer();
    *msg_buf++ = 35 + RF4CONTROL_PID_LEN;
    *msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = ZRC_CMD_DISCOVERY_CONFIRM;
    *msg_buf++ = Status;
    *msg_buf++ = PairingRef;
    for (i = 0; i < 32; i++)
    {
        *msg_buf++ = *SupportedCmd++;
    }
    *msg_buf = EOT;
}
#endif

#ifdef ZRC_CMD_DISCOVERY
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zrc_cmd_disc_indication(uint8_t PairingRef)
{
    uint8_t *msg_buf;

    msg_buf = get_next_tx_buffer();
	*msg_buf++ = 2 + RF4CONTROL_PID_LEN;
    *msg_buf++ = RF4CONTROL_PID;
    *msg_buf++ = ZRC_CMD_DISCOVERY_INDICATION;
    *msg_buf++ = PairingRef;
    *msg_buf = EOT;
}
#endif


#endif /* SIO_HUB */
/* EOF */

