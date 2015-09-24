/*
 * cmn_defs.h
 *
 *  Created on: Sep 15, 2013
 *      Author: delsissy
 */

#ifndef CMN_DEFS_H_
#define CMN_DEFS_H_
#include <stdio.h>
#include "dbg_logs.h"
/// Builds the task identifier from the type and the index of that task.
#define KE_BUILD_ID(type, index) ( (uint16_t)(((index) << 8)|(type)) )

/// Retrieves task index number from task id.
#define KE_IDX_GET(NMI_Uint16) ((((uint16_t)(NMI_Uint16)) >> 8) & 0x00FF)

#define KE_TASK_GET(NMI_Uint16) ((NMI_Uint16) & 0x00FF)
typedef union union_ptr_tag
{
    void     *ptr;
    uint8_t  *u8ptr;
    uint16_t *u16ptr;
    uint32_t *u32ptr;
    uint32_t  val;
} UnionPtr;
extern uint8_t unNull;
#define UN_NULL(ptr) (ptr != NULL ? ptr : &unNull)
//#define ALIGN_PTR_WORD_CEIL_BOUNDARY(x)              (x=((ptr_to_uint32)(x+3)&0xFFFFFFFC))
#define ALIGN_PTR_WORD_FLOOR_BOUNDARY(x)\
    do{\
        UnionPtr _x;\
        _x.ptr = x;\
        _x.val = ((_x.val + 3) & 0xFFFFFFFC) - 4;\
        x = _x.ptr;\
    } while (0)
#define TASK_EXTERN         TASK_GTL
#define TASK_INTERN					(0x05)
#define API_PKT_ID          0x05
#define HDR_LEN             9

#define NMI_CHAR_MAX_LEN        (2)

#define KEY_LEN             0x10

#define ERROR_REPORT    PRINT_ERR("\n");
#define NMI_TRUE (1)


enum
{
    DISABLE = 0,
    ENABLE
};

/// Attribute access types
enum
{
    /// Read Access
    PERM_ACCESS_RD           = 0,
    /// Write Access
    PERM_ACCESS_WR           = 3,
    /// Indication Access
    PERM_ACCESS_IND          = 6,
    /// Notification Access
    PERM_ACCESS_NTF          = 9,
    /// Check Encryption key size
    PERM_ACCESS_EKS          = 12,
    /// Hide/Show attribute
    PERM_ACCESS_HIDE         = 13,
    /// Write Signed Enabled attribute
    PERM_ACCESS_WRITE_SIGNED = 14
};

/// Attribute access types mask
enum
{
    /// Read Access Mask
    PERM_ACCESS_MASK_RD           = 0x0007,
    /// Write Access Mask
    PERM_ACCESS_MASK_WR           = 0x0038,
    /// Indication Access Mask
    PERM_ACCESS_MASK_IND          = 0x01C0,
    /// Notification Access Mask
    PERM_ACCESS_MASK_NTF          = 0x0E00,
    /// Check Encryption key size Mask
    PERM_ACCESS_MASK_EKS          = 0x1000,
    /// Hide/Show attribute Mask
    PERM_ACCESS_MASK_HIDE         = 0x2000,
    /// Write Signed Enabled attribute Mask
    PERM_ACCESS_MASK_WRITE_SIGNED = 0x4000
};

/// Service access types
enum
{
    /// Service Access
    PERM_ACCESS_SVC      = 0,
    /// Check Encryption key size for service Access
    PERM_ACCESS_SVC_EKS  = 3,
    /// Hide/Show service Access
    PERM_ACCESS_SVC_HIDE = 4
};

/// Service access types mask
enum
{
    /// Service Access Mask
    PERM_ACCESS_MASK_SVC       = 0x07,
    /// Check Encryption key size for service Access Mask
    PERM_ACCESS_MASK_SVC_EKS   = 0x08,
    /// Hide/Show service Access Mask
    PERM_ACCESS_MASK_SVC_HIDE  = 0x10
};

/// Attribute & Service access rights
enum
{
    /// Disable access
    PERM_RIGHT_DISABLE  = 0,
    /// Enable access
    PERM_RIGHT_ENABLE   = 1,
    /// Access Requires Unauthenticated link
    PERM_RIGHT_UNAUTH   = 2,
    /// Access Requires Authenticated link
    PERM_RIGHT_AUTH     = 3,
    /// Access Requires authorization
    PERM_RIGHT_AUTHZ    = 4
};

/// Attribute & Service access rights mask
enum
{
    /// Disable access mask
    PERM_RIGHT_MASK_DISABLE  = 0x3,
    /// Enable access mask
    PERM_RIGHT_MASK_ENABLE   = 0x3,
    /// Access Requires Unauthenticated lin kmask
    PERM_RIGHT_MASK_UNAUTH   = 0x3,
    /// Access Requires Authenticated link mask
    PERM_RIGHT_MASK_AUTH     = 0x3,
    /// Access Requires authorization mask
    PERM_RIGHT_MASK_AUTHZ    = 0x4
};

#define TRUE 1
#define FALSE 0

#define PERM(access, right) \
    (((PERM_RIGHT_ ## right) << (PERM_POS_ ## access)) & (PERM_MASK_ ## access))

enum KE_API_ID
{
    // Link Layer Tasks
    TASK_LLM          = 0,
    TASK_LLC          = 1,
    TASK_LLD          = 2,
    TASK_DBG          = 3,

    // BT Controller Tasks
    TASK_LM           = 4,
    TASK_LC           = 5,
    TASK_LB           = 6,
    TASK_LD           = 7,

    TASK_HCI          = 8,
    TASK_DISPLAY      = 9,

    TASK_L2CC         = 10,
    TASK_GATTM        = 11,   // Generic Attribute Profile Manager Task
    TASK_GATTC        = 12,   // Generic Attribute Profile Controller Task
    TASK_GAPM         = 13,   // Generic Access Profile Manager
    TASK_GAPC         = 14,   // Generic Access Profile Controller

    TASK_APP          = 15,
    TASK_GTL          = 16,
    TASK_INTERNAL_APP = 17,
    TASK_FLASH        = 18,
    // -----------------------------------------------------------------------------------
    // --------------------- BLE Profile TASK API Identifiers ----------------------------
    // -----------------------------------------------------------------------------------
    TASK_DISS         = 20,   // Device Information Service Server Task
    TASK_DISC         = 21,   // Device Information Service Client Task

    TASK_PROXM        = 22,   // Proximity Monitor Task
    TASK_PROXR        = 23,   // Proximity Reporter Task

    TASK_FINDL        = 24,   // Find Me Locator Task
    TASK_FINDT        = 25,   // Find Me Target Task

    TASK_HTPC         = 26,   // Health Thermometer Collector Task
    TASK_HTPT         = 27,   // Health Thermometer Sensor Task

    TASK_BLPS         = 28,   // Blood Pressure Sensor Task
    TASK_BLPC         = 29,   // Blood Pressure Collector Task

    TASK_HRPS         = 30,   // Heart Rate Sensor Task
    TASK_HRPC         = 31,   // Heart Rate Collector Task

    TASK_TIPS         = 32,   // Time Server Task
    TASK_TIPC         = 33,   // Time Client Task

    TASK_SCPPS        = 34,   // Scan Parameter Profile Server Task
    TASK_SCPPC        = 35,   // Scan Parameter Profile Client Task

    TASK_BASS         = 36,   // Battery Service Server Task
    TASK_BASC         = 37,   // Battery Service Client Task

    TASK_HOGPD        = 38,   // HID Device Task
    TASK_HOGPBH       = 39,   // HID Boot Host Task
    TASK_HOGPRH       = 40,   // HID Report Host Task

    TASK_GLPS         = 41,   // Glucose Profile Sensor Task
    TASK_GLPC         = 42,   // Glucose Profile Collector Task

    TASK_RSCPS        = 43,   // Running Speed and Cadence Profile Server Task
    TASK_RSCPC        = 44,   // Running Speed and Cadence Profile Collector Task

    TASK_CSCPS        = 45,   // Cycling Speed and Cadence Profile Server Task
    TASK_CSCPC        = 46,   // Cycling Speed and Cadence Profile Client Task

    TASK_ANPS         = 47,   // Alert Notification Profile Server Task
    TASK_ANPC         = 48,   // Alert Notification Profile Client Task

    TASK_PASPS        = 49,   // Phone Alert Status Profile Server Task
    TASK_PASPC        = 50,   // Phone Alert Status Profile Client Task

    TASK_CPPS         = 51,   // Cycling Power Profile Server Task
    TASK_CPPC         = 52,   // Cycling Power Profile Client Task

    TASK_LANS         = 53,   // Location and Navigation Profile Server Task
    TASK_LANC         = 54,   // Location and Navigation Profile Client Task


    TASK_INVALID      = 0xFF, // Invalid Task Identifier
};

///Advertising filter policy
enum adv_filter_policy
{
    ///Allow both scan and connection requests from anyone
    ADV_ALLOW_SCAN_ANY_CON_ANY    = 0x00,
    ///Allow both scan req from White List devices only and connection req from anyone
    ADV_ALLOW_SCAN_WLST_CON_ANY,
    ///Allow both scan req from anyone and connection req from White List devices only
    ADV_ALLOW_SCAN_ANY_CON_WLST,
    ///Allow scan and connection requests from White List devices only
    ADV_ALLOW_SCAN_WLST_CON_WLST,
    ///Enumeration end value for advertising filter policy value check
    ADV_ALLOW_SCAN_END
};



#define ATT_SIGNATURE_LEN                       0x0C

/// Length, number, offset defines
#define ATT_SVC_VALUE_MAX_LEN                   0x0030
#define ATT_CHAR_NAME_MAX_LEN                   0x0030
#define ATT_UUID_16_LEN                         0x0002
#define ATT_UUID_32_LEN                         0x0004
#define ATT_UUID_128_LEN                        0x0010
#define ATT_MAX_NUM_EXT_PROFILE                 0x000A
#define ATT_MAX_NUM_INCL_PROFILE                0x000A


/// Maximum Transmission Unit
#define ATT_DEFAULT_MTU                                 0x0017

/// Maximum value length
#define ATTM_MAX_VALUE                               (ATT_DEFAULT_MTU+1)


/// Maximum content of reliable write
#define ATTM_RELIABLE_WRITE                          0x10

/// Nb of handles for multiple handle operation
#define ATT_NB_MULT_HDLS                                0x0008

/// desired UUID
struct att_desired_type
{
    /// Size of the UUID
    uint16_t value_size;
    /// actual UUID
    uint8_t value[ATT_UUID_128_LEN];
};


/// UUID with different length Structure
struct att_uuid_type
{
    /// Size of the UUID
    uint8_t value_size;
    /// expected response size - read multiple
    uint8_t expect_resp_size;
    /// actual UUID
    uint8_t value[ATT_UUID_128_LEN];
};

/// Attribute data holder
struct att_info_data
{
    /// data length
    uint16_t len;
    /// each result length
    uint8_t each_len;
    /// data
    uint8_t data[1];
};

#endif /* CMN_DEFS_H_ */
