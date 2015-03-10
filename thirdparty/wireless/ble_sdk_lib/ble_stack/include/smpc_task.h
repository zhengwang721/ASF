/*
 * smpc_task.h
 *
 *  Created on: Sep 19, 2013
 *      Author: delsissy
 */

#ifndef SMPC_TASK_H_
#define SMPC_TASK_H_

#include "nmi_types.h"
#include "gapm_task.h"
#include "cmn_defs.h"

#define SMPC_IDX_MAX           BLE_CONNECTION_MAX

///Maximum Encryption Key size
#define SMPC_MAX_ENC_SIZE_LEN                 0x10

/// SMPC Task Messages
enum
{
    ///Host -> SMPC request to start pairing
    SMPC_PAIRING_REQ = 0x1C00,
    ///SMPC -> Host request for pairing information
    SMPC_PAIRING_IND,
    ///Host -> SMPC response with pairing information
    SMPC_PAIRING_RSP,

    ///Host -> SMPC request to encrypt link
    SMPC_START_ENC_REQ,
    ///SMPC -> Host security procedure over
    SMPC_SEC_STARTED_IND,

    //SMPC-> Host requests & responses for keys
    SMPC_TK_REQ_IND,
    SMPC_TK_REQ_RSP,
    SMPC_LTK_REQ_IND,
    SMPC_LTK_REQ_RSP,
    SMPC_IRK_REQ_IND,
    SMPC_IRK_REQ_RSP,
    SMPC_CSRK_REQ_IND,
    SMPC_CSRK_REQ_RSP,

    ///SMPC->Host key that was received
    SMPC_KEY_IND,

    /// -> SMPC : Timer Expiration indication
    SMPC_RTX_IND,

    //SMPC -> SMPM (-> LLM)
    ///Encryption Request using AES-128 HW implemented
    SMPC_E_REQ,
    ///Random Number request from HW
    SMPC_RAND_REQ,

    //(LLM -> ) SMPM -> SMPC
    ///Encrypted data response
    SMPC_E_CFM,
    ///Random number value response
    SMPC_RAND_CFM,

    //Message signing (GAP or GATT may be the source)
    ///HL  -> SMPC signature calculation request (whether for TX or check RX)
    SMPC_SIGN_REQ,
    ///SMPC-> HL Response with sign counter + calculated signature
    SMPC_GEN_SIGN_RSP,
    ///SMPC-> HL Response with status of signature check
    SMPC_CHK_SIGN_RSP,

    //Random address generation
    ///GAP  -> SMPC random address generation request
    SMPC_GEN_RAND_ADDR_REQ,
    ///SMPC -> GAP response with generated random address
    SMPC_GEN_RAND_ADDR_RSP,

    //Random address resolution
    ///GAP-> SMPC random address resolution request
    SMPC_SOLVE_ADDR_REQ,
    ///SMPC-> GAP given random address resolution conclusion
    SMPC_SOLVE_ADDR_RSP,

    ///Ask APP to check if peer with BD address is Unauth/Authen/Authen+Authoriz
    SMPC_CHK_BD_ADDR_REQ_IND,
    SMPC_CHK_BD_ADDR_REQ_RSP,

    SMPC_DBG_TRC,
    /// protocol timeout event, sent to source of procedure start
    SMPC_TIMEOUT_EVT,
    ///Repeated attempts Timeout
    SMPC_ATTEMPTS_IND
};


/// IO Capability Values
enum
{
    ///Display Only
    SMP_IO_CAP_DISPLAY_ONLY = 0x00,
    ///Display Yes No
    SMP_IO_CAP_DISPLAY_YES_NO,
    ///Keyboard Only
    SMP_IO_CAP_KB_ONLY,
    ///No Input No Output
    SMP_IO_CAP_NO_INPUT_NO_OUTPUT,
    ///Keyboard Display
    SMP_IO_CAP_KB_DISPLAY,
    SMP_IO_CAP_LAST
};

/// OOB Data Present Flag Values
enum
{
    ///OOB Data not present
    SMP_OOB_AUTH_DATA_NOT_PRESENT = 0x00,
    ///OOB data from remote device present
    SMP_OOB_AUTH_DATA_FROM_REMOTE_DEV_PRESENT,
    SMP_OOB_AUTH_DATA_LAST
};

/// Authentication Requirements
enum
{
    ///No MITM No Bonding
    SMP_AUTH_REQ_NO_MITM_NO_BOND = 0x00,
    ///No MITM Bonding
    SMP_AUTH_REQ_NO_MITM_BOND,
    ///MITM No Bonding
    SMP_AUTH_REQ_MITM_NO_BOND = 0x04,
    ///MITM and Bonding
    SMP_AUTH_REQ_MITM_BOND,
    SMP_AUTH_REQ_LAST
};

#if 0
///SMPC Pairing request received from Host
struct smpc_pairing_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Pairing information
    struct gap_pair_par pair;
};

///SMPC Pairing Indication to Host, asking for Pairing information
struct smpc_pairing_ind
{
    ///Connection index for which pairing info is requested
    uint8_t idx;
    ///Authentication requirements from peer
    uint8_t auth_req;
    ///Pairing Request IO capabilities - otherwise use 0xFF
    uint8_t preq_io_cap;
    ///Pairing Request Out Of Band Data presence flag - otherwise use 0xFF
    uint8_t preq_oob_data_flg;
    ///Pairing Request Maximum Encryption Key Size - otherwise use 0xFF
    uint8_t preq_max_enc_size;
    ///Pairing Request Initiator Key Distribution - otherwise use 0xFF
    uint8_t preq_ikey_dist;
    ///Pairing Request Responder Key Distribution - otherwise use 0xFF
    uint8_t preq_rkey_dist;
};

///SMPC Pairing response from Host
struct smpc_pairing_rsp
{
    ///Connection index for which pairing info is requested
    uint8_t idx;
    ///status of the request - if failed, end pairing
    uint8_t status;
    ///security mode set in device before connection
    uint8_t sec_mode;
    ///IO capabilities
    uint8_t io_cap;
    ///OOB flag
    uint8_t oob;
    ///Authentication Requirements
    uint8_t auth_req;
    ///Max key size
    uint8_t max_key_size;
    ///Initiator key distribution
    uint8_t ikeys;
    ///Responder key distribution
    uint8_t rkeys;
};

///SMPC start encryption request
struct smpc_start_enc_req
{
    ///Connection index for which pairing info is requested
    uint8_t idx;
    ///Authentication Requirements - needed if slave
    uint8_t auth_req;
    ///EDIV
    uint16_t ediv;
    ///Random number
    struct rand_nb randnb;
    ///LTK
    struct smp_key ltk;
};

/// SMPC Security started indication to host, with status and keys
struct smpc_sec_started_ind
{
    /// Connection index
    uint8_t idx;
    /// Status (OK or failure status)
    uint8_t status;
    ///Key size for the LTK/STK agreed upon during pairing features exchange)
    uint8_t key_size;
    ///Security properties of the keys
    uint8_t sec_prop;
    ///Bonding status
    uint8_t bonded;
};

///SMPC TK request indication
struct smpc_tk_req_ind
{
    ///Connection index
    uint8_t idx;
    ///key type: OOB 16B or 6 digit
    uint8_t oob_en;
    ///action expected if 6 digit, depending on IOs
    uint8_t disp_en;
};

///SMPC TK request response: status, TK value and random value
struct smpc_tk_req_rsp
{
    ///Connection index
    uint8_t idx;
    ///Status of response
    uint8_t status;
    ///TK key, all 0 if status is error
    struct smp_key tk;
};

///SMPC request to Host for LTK (and associated EDIV and Rand values)
struct smpc_ltk_req_ind
{
    ///Connection index
    uint8_t idx;
    ///Authentication Requirements from request
    uint8_t auth_req;
};

///Host response to SMPC LTK request
struct smpc_ltk_req_rsp
{
    ///Connection index
    uint8_t idx;
    ///Status of response
    uint8_t status;
    ///Security properties kept in host with the key
    uint8_t sec_prop;
    ///EDIV corresponding to key, or 0 if error status
    uint16_t ediv;
    ///Random Number corresponding to key, or 0 if error status
    struct rand_nb nb;
    ///Found LTK key, or 0 if error status
    struct smp_key ltk;
};

///SMPC request to Host for IRK
struct smpc_irk_req_ind
{
    ///Connection index
    uint8_t idx;
};

///Host response to SMPC IRK request
struct smpc_irk_req_rsp
{
    ///Connection index
    uint8_t idx;
    ///Status of response
    uint8_t status;
    ///Original BD address associated to this IRK
    struct bd_addr orig_addr;
    ///Found IRK key, or 0 if error status
    struct smp_key irk;
};

///SMPC request to Host for CSRK
struct smpc_csrk_req_ind
{
    ///Connection index
    uint8_t idx;
    ///Bd address of device for which bonding info containing CSRK should exist
    struct bd_addr addr;
    ///Signing counter received - to check against last known in APP
    uint32_t signcnt;

};

///Host response to SMPC CSRK request
struct smpc_csrk_req_rsp
{
    ///Connection index
    uint8_t idx;
    ///Status of response
    uint8_t status;
    /// Security Status
    uint8_t sec_status;
    ///Found CSRK key, or 0 if error status
    struct smp_key csrk;
};

///SMPC Key indication to Host (with a received key during TKDP)
struct smpc_key_ind
{
    ///Connection index
    uint8_t idx;
    ///Key code - use one of defined values for Key distribution parameters
    uint8_t key_code;
    ///EDIV (=0 if not sending an LTK)
    uint16_t ediv;
    ///Random number (=0 if not sending an LTK)
    struct rand_nb nb;
    ///Key being sent to Host (LTK/IRK/CSRK)
    struct smp_key key;
};

///E function calculation result for SMPC
struct smpc_e_cfm
{
    ///Connection index
    uint8_t idx;
    /// Status of the command reception
    uint8_t status;
    ///Encrypted data to return to command source.
    uint8_t e_result[16];
};

///Random confirmation
struct smpc_rand_cfm
{
    ///Connection index
    uint8_t idx;
    /// Status of the command reception
    uint8_t status;
    ///Random number
    struct rand_nb nb;
};

///Signature calculation request
struct smpc_sign_req
{
    ///Connection index
    uint8_t idx;
    ///Flag indicating whether a signature is being generated(1) or checked(0) for the msg
    uint8_t flag;
    ///Byte Message length - consider useful bytes only, not bit length
    uint8_t bytelen;
    ///Message to sign(if check: msg+signcnt+signature; if gen: msg only) //todo decide max len
    uint8_t msg[128];
};

///SMPC Signature Generation Response for Host
struct smpc_gen_sign_rsp
{
    ///Connection index
    uint8_t idx;
    ///Status of generation
    uint8_t status;
    ///12 octet sign counter + signature array, LSB to MSB format for PDU
    uint8_t cnt_sign[12];
    ///Original Message
    uint8_t msg[128];
};

///SMPC Signature Check Response for Host
struct smpc_chk_sign_rsp
{
    ///Connection index
    uint8_t idx;
    ///Status of generation
    uint8_t status;
    ///Original Message
    uint8_t msg[128];
};

///Generate Random Address Request (for local use - will use SMPM kept IRK)
struct smpc_gen_rand_addr_req
{
    ///Connection index -may be a free task index
    uint8_t idx;
    /// Non-res = 0x00, Resolvable = 0x01
    uint8_t type;
    /// No Set to LL = 0x00, Set to LL = 0x01
    uint8_t set_to_ll;
    /// Prand value - for testing algorithm only, randomly generated in normal use by GAP
    uint32_t prand;
};

/// Random address generation response
struct smpc_gen_rand_addr_rsp
{
    ///Connection index
    uint8_t idx;
    ///Generated random address
    struct bd_addr rand_addr;
};

///Solve Address Request
struct smpc_solve_addr_req
{
    ///Connection index -may be a free task index
    uint8_t idx;
    ///Type of address to check
    uint8_t type;
    ///Random address to resolve or Public address to check in APP
    struct bd_addr addr;
};

/// Random address solving response
struct smpc_solve_addr_rsp
{
    ///Connection index
    uint8_t idx;
    //status
    uint8_t status;
    ///Original bd address associated to matched IRK
    struct bd_addr orig_addr;
    //Type of address
    uint8_t orig_type;
    /// security status
    uint8_t sec_status;
    ///Original bd address associated to matched IRK
    struct bd_addr res_addr;
    //Type of address
    uint8_t res_type;

};

///Check BD address Request
struct smpc_chk_bd_addr_req
{
    ///Connection index -may be a free task index
    uint8_t idx;
    ///Type of address to check
    uint8_t type;
    ///Random address to resolve or Public address to check in APP
    struct bd_addr addr;
};

///Check BD address Response
struct smpc_chk_bd_addr_rsp
{
    ///Connection index -may be a free task index
    uint8_t idx;
    ///Status - found or not
    uint8_t found_flag;
    ///Saved link security status from higher layer
    uint8_t lk_sec_status;
    ///Type of address to check
    uint8_t type;
    ///Random address to resolve or Public address to check in APP
    struct bd_addr addr;
};

///Debug trace
struct smpc_dbg_trc
{
    /// index
    uint8_t idx;
    /// debug code
    uint8_t dbg_code;
    /// SMP key
    struct smp_key dbg;
};

///Timeout Event
struct smpc_timeout_evt
{
    /// index
    uint8_t idx;
};

#endif
#endif /* SMPC_TASK_H_ */
