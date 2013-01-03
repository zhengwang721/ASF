/**
 * @file vendor_data.c
 *
 * @brief Implementation of vendor-specific data handling
 *
 * $Id: vendor_data.c 32660 2012-07-16 12:07:02Z agasthian.s $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifdef VENDOR_DATA

/* === INCLUDES ============================================================ */

#include <stdio.h>

#include "rf4ce.h"
#include "zrc.h"
#include "vendor_data.h"
#include "pal.h"


/* === MACROS ============================================================== */


/* === EXTERNALS =========================================================== */

FLASH_EXTERN(uint16_t VendorIdentifier);
#ifdef RF4CE_CALLBACK_PARAM
#ifdef RF4CE_TARGET
extern void vendor_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t ProfileId
#ifdef NLDE_HANDLE
                                , uint8_t Handle
#endif
                               );
#else /* RF4CE_TARGET */
extern void nlme_rx_enable_confirm(nwk_enum_t Status);
static void vendor_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t ProfileId
#ifdef NLDE_HANDLE
                                , uint8_t Handle
#endif
                               );
void vendor_data_ind(uint8_t PairingRef, uint16_t VendorId,
                     uint8_t nsduLength, uint8_t *nsdu, uint8_t RxLinkQuality,
                     uint8_t RxFlags);
#endif /* RF4CE_TARGET */
#endif /* RF4CE_CALLBACK_PARAM */

/* === IMPLEMENTATION ====================================================== */

bool vendor_data_request(uint8_t PairingRef, profile_id_t ProfileId,
                         uint16_t VendorId, uint8_t nsduLength, uint8_t *nsdu,
                         uint8_t TxOptions)
{
    /* Keep compiler happy */
    ProfileId = ProfileId;


    return nlde_data_request(PairingRef, PROFILE_ID_ZRC, VendorId,
                             nsduLength, nsdu, TxOptions
#ifdef NLDE_HANDLE
                             , 1
#endif
#ifdef RF4CE_CALLBACK_PARAM
                             , (FUNC_PTR) vendor_data_confirm
#endif
                            );
}


#ifndef RF4CE_TARGET
void vendor_data_ind(uint8_t PairingRef, uint16_t VendorId,
                     uint8_t nsduLength, uint8_t *nsdu, uint8_t RxLinkQuality,
                     uint8_t RxFlags)
{
    /* Check if vendor id matches.
       Handle here only vendor data from same vendor */
    uint16_t v_id = PGM_READ_WORD(&VendorIdentifier);
    if ((VendorId == v_id) && (RxFlags & RX_FLAG_WITH_SEC))
    {
        switch (nsdu[0])    // vendor-specific command id
        {
#ifdef TFA_BAT_MON
            case BATTERY_STATUS_REQ:
                {
                    uint16_t voltage = tfa_get_batmon_voltage();
                    nsdu[0] = BATTERY_STATUS_RESP;
                    nsdu[1] = (uint8_t)voltage;    // LSB
                    nsdu[2] = (uint8_t)(voltage >> 8);    // MSB
                    nsduLength = 3;
                }
                break;
#endif
            case ALIVE_REQ:  /* Alive request */
                vendor_app_alive_req();
                /* Send alive response */
                nsdu[0] = ALIVE_RESP;
                nsduLength = 1;
                break;

            case FW_VERSION_REQ:
                {
                    /* Send alive response */
                    nsdu[0] = FW_VERSION_RESP;
                    nsdu[1] = FW_VERSION_MAJOR;    // major version number
                    nsdu[2] = FW_VERSION_MINOR;    // minor version number
                    nsdu[3] = FW_VERSION_REV;    // revision version number
                    nsduLength = 4;
                }
                break;

            case RX_ON_REQ:
                {
                    uint32_t duration = 0;

                    memcpy(&duration, &nsdu[1], 3);
                    if (!nlme_rx_enable_request(duration
#ifdef RF4CE_CALLBACK_PARAM
                                                , (FUNC_PTR) nlme_rx_enable_confirm
#endif
                                               ))
                    {
                        /*
                         * RX enable could not been added to the queue.
                         * Therefore do not send response message.
                         */
                        return;
                    }
                    /* Send response */
                    nsdu[0] = RX_ON_RESP;
                    nsduLength = 1;
                }
                break;


            default:
                {
                    /* Send response */
                    nsdu[0] = FW_DATA_RESP;
                    nsdu[1] = VD_NOT_SUPPORTED_ATTRIBUTE;
                    nsduLength = 2;
                }
                break;
        }

        /* Transmit response message */

        nlde_data_request(PairingRef, PROFILE_ID_ZRC, VendorId,
                          nsduLength, nsdu,
                          TXO_UNICAST | TXO_DST_ADDR_NET | TXO_ACK_REQ | TXO_SEC_REQ | TXO_MULTI_CH | TXO_CH_NOT_SPEC | TXO_VEND_SPEC
#ifdef NLDE_HANDLE
                          , 1
#endif
#ifdef RF4CE_CALLBACK_PARAM
                          , (FUNC_PTR) vendor_data_confirm
#endif
                         );
        /* Keep compiler happy */
        RxLinkQuality = RxLinkQuality;
        RxFlags = RxFlags;
    }

}
#endif  /* #ifndef RF4CE_TARGET */


#ifndef RF4CE_TARGET
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void vendor_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t ProfileId
#ifdef NLDE_HANDLE
                         , uint8_t Handle
#endif
                        )
{
    /* Keep compiler happy */
    Status = Status;
    PairingRef = PairingRef;
#ifdef NLDE_HANDLE
    Handle = Handle;
    ProfileId = ProfileId;
#endif
}
#endif


#endif  /* #ifdef VENDOR_DATA */
