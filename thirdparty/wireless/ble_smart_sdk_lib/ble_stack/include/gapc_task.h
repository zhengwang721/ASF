/*
 * gapc_task.h
 *
 *  Created on: Jul 16, 2014
 *      Author: delsissy
 */
#ifndef GAPC_TASK_H_
#define GAPC_TASK_H_

#include "cmn_defs.h"
#include "at_ble_api.h"
typedef struct
{
    /// RSSI value
    uint8_t rssi;
} gapc_con_rssi_ind;

typedef struct
{
    at_ble_handle_t conn_handle;
    // request types
#define GAPC_DEV_NAME               (0x00)
#define GAPC_DEV_APPEARANCE         (0x01)
#define GAPC_DEV_SLV_PREF_PARAMS    (0x02)
    // refer to the above types
    uint8_t req;
} gapc_get_dev_info_req_ind_t; /*GAPC_GET_DEV_INFO_REQ_IND*/

at_ble_events_t gapc_cmp_evt(uint16_t src, uint8_t *data, void *params);

uint8_t gapc_get_info_cmd_handler(uint16_t conn_handle, uint8_t operation, uint8_t *param);

void gapc_con_req_ind(uint8_t *data);

uint8_t gapc_connection_cfm_handler(uint8_t *pu8LocalKey,  uint32_t u32LocalSignCntr,
                                    uint8_t *pu8RemoteKey, uint32_t u32RemoteSignCntr,
                                    uint8_t u8Auth, uint8_t service_enabled, uint16_t u16ConHdl);

uint8_t gapc_disconnect_cmd_handler(uint8_t reason, uint16_t handle);

void gapc_disconnect_ind(uint8_t *data, at_ble_disconnected_t *param);

void gapc_dev_info_req_ind_handler(uint16_t src, uint8_t *data, gapc_get_dev_info_req_ind_t *params);

at_ble_events_t gapc_bond_ind(uint16_t src, uint8_t *data, at_ble_pair_done_t *param);

uint8_t gapc_bond_req_ind(uint16_t src, uint8_t *data, void *param);

void gapc_bond_cmd_handler(uint16_t conn_handle, uint8_t io_cababilities,
                           uint8_t oob_avaiable, uint8_t auth_req, uint8_t max_key_size,
                           uint8_t initiator_keys, uint8_t responder_keys, uint8_t sec_req);

void gapc_bond_cfm_handler_pair_resp(uint8_t u8Accept, uint8_t u8IoCap, uint8_t u8OOB, uint8_t u8Auth,
                                     uint8_t u8KeySize, uint8_t u8IkeyDist, uint8_t u8RkeyDist, uint8_t u8SecReq,
                                     uint16_t u16ConHdl);

void gapc_bond_cfm_handler_key_exch(uint8_t u8Req,  uint8_t u8Accept, uint8_t *key,
                                    uint16_t u16ConHdl);

void gapc_security_cmd_handler(uint16_t conn_handle, uint8_t auth_req);

void gapc_encrypt_cmd_handler(uint16_t conn_handle, uint8_t *key,
                              uint16_t ediv, uint8_t *rand, uint8_t key_size , uint8_t auth);

void gapc_encrypt_req_ind(uint16_t src, uint8_t *data, at_ble_encryption_request_t *params);

void gapc_encrypt_cfm_handler(uint16_t conn_handle, uint8_t auth , uint8_t key_found,
                              uint8_t *key, uint8_t key_size);

void gapc_encrypt_ind(uint16_t src , uint8_t *data , at_ble_encryption_status_changed_t *params);

void gapc_sec_req_ind(uint16_t src, uint8_t *data, at_ble_slave_sec_request_t *params);

uint8_t gapc_sign_counter_ind_handler(uint16_t src, uint8_t *data, at_ble_sign_counter_t *params);
void gapc_peer_att_info_ind_handler(uint16_t src, uint8_t *data, at_ble_peer_att_info_ind_t *param);
void gapc_con_channel_map_ind_handler(uint16_t src, uint8_t *data, at_ble_channel_map_t *param);


at_ble_status_t gapc_lecb_create_handler(at_ble_handle_t conn_handle,
        uint16_t sec_lvl, uint16_t le_psm,
        uint16_t cid, uint16_t initial_credit);

void gapc_lecb_connect_cmd_handler(at_ble_handle_t conn_handle, uint16_t le_psm,
                                   uint16_t cid, uint16_t intial_credit);

at_ble_status_t gapc_lecb_destroy_handler(at_ble_handle_t conn_handle, uint16_t le_psm);

at_ble_status_t gapc_lecb_connect_cfm_handler(at_ble_handle_t conn_handle, uint16_t le_psm,
        at_ble_lecb_status status);

void gapc_lecb_disconnect_cmd_handler(at_ble_handle_t conn_handle, uint16_t le_psm);

at_ble_status_t gapc_lecb_add_cmd_handler(at_ble_handle_t conn_handle, uint16_t le_psm, uint16_t credit);

void gapc_lecb_connected(uint8_t *data, at_ble_lecb_connected_t *param);
void gapc_lecb_disconnected(uint8_t *data, at_ble_lecb_disconnected_t *param);
void gapc_lecb_connect_req_ind(uint8_t *data, at_ble_lecb_conn_req_t *param);
void gapc_lecb_add_credit_resp_handler(uint8_t *data, at_ble_lecb_add_credit_ind_t *param);


enum gapc_msg_id
{
    /* Default event */
    /// Command Complete event
    GAPC_CMP_EVT = ((uint16_t)(TASK_GAPC << 8)),

    /* Connection state information */
    /// Indicate that a connection has been established
    GAPC_CONNECTION_REQ_IND,
    /// Set specific link data configuration.
    GAPC_CONNECTION_CFM,

    /// Indicate that a link has been disconnected
    GAPC_DISCONNECT_IND,

    /* Link management command */
    /// Request disconnection of current link command.
    GAPC_DISCONNECT_CMD,

    /* Peer device info */
    /// Retrieve information command
    GAPC_GET_INFO_CMD,
    /// Peer device attribute DB info such as Device Name, Appearance or Slave Preferred Parameters
    GAPC_PEER_ATT_INFO_IND,
    /// Indication of peer version info
    GAPC_PEER_VERSION_IND,
    /// Indication of peer features info
    GAPC_PEER_FEATURES_IND,
    /// Indication of ongoing connection RSSI
    GAPC_CON_RSSI_IND,

    /* Device Name Management */
    /// Peer device request local device info such as name, appearance or slave preferred parameters
    GAPC_GET_DEV_INFO_REQ_IND,
    /// Send requested info to peer device
    GAPC_GET_DEV_INFO_CFM,
    /// Peer device request to modify local device info such as name or appearance
    GAPC_SET_DEV_INFO_REQ_IND,
    /// Local device accept or reject device info modification
    GAPC_SET_DEV_INFO_CFM,

    /* Connection parameters update */
    /// Perform update of connection parameters command
    GAPC_PARAM_UPDATE_CMD,
    /// Request of updating connection parameters indication
    GAPC_PARAM_UPDATE_REQ_IND,
    /// Master confirm or not that parameters proposed by slave are accepted or not
    GAPC_PARAM_UPDATE_CFM,
    /// Connection parameters updated indication
    GAPC_PARAM_UPDATED_IND,

    /* Bonding procedure */
    /// Start Bonding command procedure
    GAPC_BOND_CMD,
    /// Bonding requested by peer device indication message.
    GAPC_BOND_REQ_IND,
    /// Confirm requested bond information.
    GAPC_BOND_CFM,
    /// Bonding information indication message
    GAPC_BOND_IND,

    /* Encryption procedure */
    /// Start Encryption command procedure
    GAPC_ENCRYPT_CMD,
    /// Encryption requested by peer device indication message.
    GAPC_ENCRYPT_REQ_IND,
    /// Confirm requested Encryption information.
    GAPC_ENCRYPT_CFM,
    /// Encryption information indication message
    GAPC_ENCRYPT_IND,

    /* Security request procedure */
    /// Start Security Request command procedure
    GAPC_SECURITY_CMD,
    /// Security requested by peer device indication message
    GAPC_SECURITY_IND,

    /* Signature procedure */
    /// Indicate the current sign counters to the application
    GAPC_SIGN_COUNTER_IND,

    /* Device information */
    /// Indication of ongoing connection Channel Map
    GAPC_CON_CHANNEL_MAP_IND,

    /* LE Credit Based */
    /// LE credit based link creation
    GAPC_LECB_CREATE_CMD,
    /// LE credit based link destruction
    GAPC_LECB_DESTROY_CMD,
    /// LE credit based connection request
    GAPC_LECB_CONNECT_CMD,
    /// LE credit based connection request indication
    GAPC_LECB_CONNECT_REQ_IND,
    /// LE credit based connection indication
    GAPC_LECB_CONNECT_IND,
    /// LE credit based connection request confirmation
    GAPC_LECB_CONNECT_CFM,
    /// LE credit based credit addition
    GAPC_LECB_ADD_CMD,
    /// LE credit based credit addition indication
    GAPC_LECB_ADD_IND,
    /// disconnect request
    GAPC_LECB_DISCONNECT_CMD,
    /// disconnect indication
    GAPC_LECB_DISCONNECT_IND,

    /* LE Ping */
    /// Update LE Ping timeout value
    GAPC_SET_LE_PING_TO_CMD,
    /// LE Ping timeout indication
    GAPC_LE_PING_TO_VAL_IND,
    /// LE Ping timeout expires indication
    GAPC_LE_PING_TO_IND,


    // ---------------------- INTERNAL API ------------------------
    /* Internal messages for timer events, not part of API*/
    /// Signature procedure
    GAPC_SIGN_CMD,
    /// Signature result
    GAPC_SIGN_IND,

    /// Parameter update procedure timeout indication
    GAPC_PARAM_UPDATE_TO_IND,
    /// Pairing procedure timeout indication
    GAPC_SMP_TIMEOUT_TIMER_IND,
    /// Pairing repeated attempts procedure timeout indication
    GAPC_SMP_REP_ATTEMPTS_TIMER_IND,
    /// Connection procedure timeout indication
    GAPC_LECB_CONN_TO_IND,
    /// Disconnection procedure timeout indication
    GAPC_LECB_DISCONN_TO_IND,
};


/// request operation type - application interface
enum gapc_operation
{
    /*                 Operation Flags                  */
    /* No Operation (if nothing has been requested)     */
    /* ************************************************ */
    /// No operation
    GAPC_NO_OP                                    = 0x00,

    /* Connection management */
    /// Disconnect link
    GAPC_DISCONNECT,

    /* Connection information */
    /// Retrieve name of peer device.
    GAPC_GET_PEER_NAME,
    /// Retrieve peer device version info.
    GAPC_GET_PEER_VERSION,
    /// Retrieve peer device features.
    GAPC_GET_PEER_FEATURES,
    /// Get Peer device appearance
    GAPC_GET_PEER_APPEARANCE,
    /// Get Peer device Slaved Preferred Parameters
    GAPC_GET_PEER_SLV_PREF_PARAMS,
    /// Retrieve connection RSSI.
    GAPC_GET_CON_RSSI,
    /// Retrieve Connection Channel MAP.
    GAPC_GET_CON_CHANNEL_MAP,

    /* Connection parameters update */
    /// Perform update of connection parameters.
    GAPC_UPDATE_PARAMS,

    /* Security procedures */
    /// Start bonding procedure.
    GAPC_BOND,
    /// Start encryption procedure.
    GAPC_ENCRYPT,
    /// Start security request procedure
    GAPC_SECURITY_REQ,

    /* LE Credit Based*/
    /// LE credit based connection creation
    GAPC_LE_CB_CREATE,
    /// LE credit based connection destruction
    GAPC_LE_CB_DESTROY,
    /// LE credit based connection request
    GAPC_LE_CB_CONNECTION,
    /// LE credit based disconnection request
    GAPC_LE_CB_DISCONNECTION,
    /// LE credit addition request
    GAPC_LE_CB_ADDITION,

    /* LE Ping*/
    /// get timer timeout value
    GAPC_GET_LE_PING_TO,
    /// set timer timeout value
    GAPC_SET_LE_PING_TO,

    // ---------------------- INTERNAL API ------------------------
    /* Packet signature */
    /// sign an attribute packet
    GAPC_SIGN_PACKET,
    /// Verify signature or an attribute packet
    GAPC_SIGN_CHECK,

    /// Last GAPC operation flag
    GAPC_LAST
};

/// Authentication mask
enum gap_auth_mask
{
    /// No Flag set
    GAP_AUTH_NONE = 0,
    /// Bond authentication
    GAP_AUTH_BOND = (1 << 0),
    /// Man In the middle protection
    GAP_AUTH_MITM = (1 << 2)
};
/// Authentication Requirements
typedef enum gap_auth
{
    /// No MITM No Bonding
    GAP_AUTH_REQ_NO_MITM_NO_BOND = (GAP_AUTH_NONE),
    /// No MITM Bonding
    GAP_AUTH_REQ_NO_MITM_BOND    = (GAP_AUTH_BOND),
    /// MITM No Bonding
    GAP_AUTH_REQ_MITM_NO_BOND    = (GAP_AUTH_MITM),
    /// MITM and Bonding
    GAP_AUTH_REQ_MITM_BOND       = (GAP_AUTH_MITM | GAP_AUTH_BOND),
    GAP_AUTH_REQ_LAST
} gap_auth_t;

// Constant defining the role
enum
{
    //Master role
    ROLE_MASTER,
    //Slave role
    ROLE_SLAVE,
    //Enumeration end value for role value check
    ROLE_END
};

/// Bond event type.
enum gapc_bond
{
    /// Bond Pairing request
    GAPC_PAIRING_REQ,
    /// Respond to Pairing request
    GAPC_PAIRING_RSP,

    /// Pairing Finished information
    GAPC_PAIRING_SUCCEED,
    /// Pairing Failed information
    GAPC_PAIRING_FAILED,

    /// Used to retrieve pairing Temporary Key
    GAPC_TK_EXCH,
    /// Used for Identity Resolving Key exchange
    GAPC_IRK_EXCH,
    /// Used for Connection Signature Resolving Key exchange
    GAPC_CSRK_EXCH,
    /// Used for Long Term Key exchange
    GAPC_LTK_EXCH,

    /// Bond Pairing request issue, Repeated attempt
    GAPC_REPEATED_ATTEMPT
};


/// OOB Data Present Flag Values
enum gap_oob
{
    /// OOB Data not present
    GAP_OOB_AUTH_DATA_NOT_PRESENT = 0x00,
    /// OOB data present
    GAP_OOB_AUTH_DATA_PRESENT,
    GAP_OOB_AUTH_DATA_LAST
};

/// Key Distribution Flags
enum gap_kdist
{
    /// No Keys to distribute
    GAP_KDIST_NONE = 0x00,
    /// Encryption key in distribution
    GAP_KDIST_ENCKEY = (1 << 0),
    /// IRK (ID key)in distribution
    GAP_KDIST_IDKEY  = (1 << 1),
    /// CSRK(Signature key) in distribution
    GAP_KDIST_SIGNKEY = (1 << 2),

    GAP_KDIST_LAST = (1 << 3)
};

/// Security Defines
enum gap_sec_req
{
    /// No security (no authentication and encryption)
    GAP_NO_SEC = 0x00,
    /// Unauthenticated pairing with encryption
    GAP_SEC1_NOAUTH_PAIR_ENC,
    /// Authenticated pairing with encryption
    GAP_SEC1_AUTH_PAIR_ENC,
    /// Unauthenticated pairing with data signing
    GAP_SEC2_NOAUTH_DATA_SGN,
    /// Authentication pairing with data signing
    GAP_SEC2_AUTH_DATA_SGN,
    /// Unrecognized security
    GAP_SEC_UNDEFINED
};

/// IO Capability Values
enum gap_io_cap
{
    /// Display Only
    GAP_IO_CAP_DISPLAY_ONLY = 0x00,
    /// Display Yes No
    GAP_IO_CAP_DISPLAY_YES_NO,
    /// Keyboard Only
    GAP_IO_CAP_KB_ONLY,
    /// No Input No Output
    GAP_IO_CAP_NO_INPUT_NO_OUTPUT,
    /// Keyboard Display
    GAP_IO_CAP_KB_DISPLAY,
    GAP_IO_CAP_LAST
};

/// TK Type
enum gap_tk_type
{
    ///  TK get from out of band method
    GAP_TK_OOB         = 0x00,
    /// TK generated and shall be displayed by local device
    GAP_TK_DISPLAY,
    /// TK shall be entered by user using device keyboard
    GAP_TK_KEY_ENTRY
};

at_ble_status_t gapc_param_update_cmd_handler(at_ble_handle_t handle,
        uint16_t intv_min,
        uint16_t intv_max,
        uint16_t latency,
        uint16_t timeout,
        uint16_t ce_len_min,
        uint16_t ce_len_max);
at_ble_status_t gapc_param_update_cfm_handler(at_ble_handle_t handle,
        bool accept,
        uint16_t ce_len_min,
        uint16_t ce_len_max);
at_ble_events_t gapc_param_updated_ind(uint16_t src_id,
                                       uint8_t *data,
                                       at_ble_conn_param_update_done_t *params);
at_ble_events_t gapc_param_updated_req_ind(uint16_t src_id,
        uint8_t *data,
        at_ble_conn_param_update_request_t *params);
#endif
