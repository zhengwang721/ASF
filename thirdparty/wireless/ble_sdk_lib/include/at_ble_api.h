/**
 ****************************************************************************************
 *
 * @file at_ble_api.h
 *
 * @brief Atmel BLE Api for Applications
 *
 * This module contains the public API and the necessary enumerations and structures that are required for 
 * BLE Application Developers using Atmel BLE SDK
 *
 *
 *  Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. The name of Atmel may not be used to endorse or promote products derived from this software 
 *  without specific prior written permission.
 *
 *  4. This software may only be redistributed and used in connection with an Atmel microcontroller product.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 *  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 *  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************************
 */
#ifndef __AT_BLE_API_H__
#define __AT_BLE_API_H__

#include <stdint.h>
#include <stdbool.h>
#include "profiles.h"
/****************************************************************************************
*							        Macros	                                     							*
****************************************************************************************/

/** @brief BLE address length. */
#define AT_BLE_ADDR_LEN            6

/** @brief BLE security key maximum length. */
#define AT_BLE_MAX_KEY_LEN (16)

/**@brief GAP Security Key Length. */
#define AT_BLE_PASSKEY_LEN (6)

/**@brief Maximum amount of addresses in a whitelist. */
#define AT_BLE_GAP_WHITELIST_ADDR_MAX_COUNT (8)

/**@brief Maximum amount of IRKs in a whitelist.
 * @note  The number of IRKs is limited to 8, even if the hardware supports more.
 */
#define AT_BLE_GAP_WHITELIST_IRK_MAX_COUNT (8)

/** @brief Maximum size of advertising data in octets. */
#define  AT_BLE_ADV_MAX_SIZE       31

/** @brief UUID 16 size in bytes */
#define AT_BLE_UUID_16_LEN (2)

/** @brief UUID 32 size in bytes */
#define AT_BLE_UUID_32_LEN (4)

/** @brief UUID 128 size in bytes */
#define AT_BLE_UUID_128_LEN (16)

/// GAP Advertising interval max and min
#define AT_BLE_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
#define AT_BLE_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
#define AT_BLE_ADV_INTERVAL_MAX        0x4000 /**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */

/// Scanning interval max and min
#define AT_BLE_SCAN_INTERVAL_MIN       0x0004 /**<Minimum scanning interval in 625 us units , i.e. 2.5 ms. */
#define AT_BLE_SCAN_INTERVAL_MAX       0x4000 /**<Maximum scanning interval in 625 us units , i.e. 10.24 s. */

/// Scanning window max and min
#define AT_BLE_SCAN_WINDOW_MIN         0x0004 /**<Minimum scanning window in 625 us units , i.e. 2.5 ms. */
#define AT_BLE_SCAN_WINDOW_MAX         0x4000 /**<Maximum scanning window in 625 us units , i.e. 10.24 s. */

/// Connection interval min (N*1.250ms)
#define AT_CNX_INTERVAL_MIN            6       //(0x06)
/// Connection interval Max (N*1.250ms)
#define AT_CNX_INTERVAL_MAX            3200    //(0xC80)
/// Connection latency min (N*cnx evt)
#define AT_CNX_LATENCY_MIN             0       //(0x00)
/// Connection latency Max (N*cnx evt
#define AT_CNX_LATENCY_MAX             499     //(0x1F3)
/// Supervision TO min (N*10ms)
#define AT_CNX_SUP_TO_MIN              10      //(0x0A)
/// Supervision TO Max (N*10ms)
#define AT_CNX_SUP_TO_MAX              3200    //(0xC80)

/// ATT MAximum Attribute Length
#define AT_BLE_MAX_ATT_LEN 512

/****************************************************************************************
*							        Basic types                                         *
****************************************************************************************/
/**
* BLE connection handle
*/
typedef uint16_t at_ble_handle_t;

/****************************************************************************************
*							        Enumerations	                                       							*
****************************************************************************************/

/// Enumeration for BLE Status 
typedef enum
{
	/// Success
	AT_BLE_SUCCESS =0,  
	/// Failure
	AT_BLE_FAILURE,  
	/// Input Parameter invalid
	AT_BLE_INVALID_PARAM  
}at_ble_status_t;

 
/// Enumeration for GAP Parameters
typedef enum 
{
	// Timer related
	AT_BLE_GAP_GEN_DISC_ADV_MIN =0,  //!< Minimum time to remain advertising, when in Discoverable mode (mSec).  Setting this parameter to 0 turns off the timeout (default).
	AT_BLE_GAP_LIM_ADV_TIMEOUT,            //!< Maximum time to remain advertising, when in Limited Discoverable mode. In seconds (default 180 seconds)
	AT_BLE_GAP_GEN_DISC_SCAN,              //!< Minimum time to perform scanning, when performing General Discovery proc (mSec)
	AT_BLE_GAP_LIM_DISC_SCAN,              //!< Minimum time to perform scanning, when performing Limited Discovery proc (mSec)
	AT_BLE_GAP_CONN_EST_ADV_TIMEOUT,       //!< Advertising timeout, when performing Connection Establishment proc (mSec)
	AT_BLE_GAP_CONN_PARAM_TIMEOUT,         //!< Link Layer connection parameter update notification timer, connection parameter update proc (mSec)
		
		// Constants
	AT_BLE_GAP_LIM_DISC_ADV_INT_MIN,       //!< Minimum advertising interval, when in limited discoverable mode (n * 0.625 mSec)
	AT_BLE_GAP_LIM_DISC_ADV_INT_MAX,       //!< Maximum advertising interval, when in limited discoverable mode (n * 0.625 mSec)
	AT_BLE_GAP_GEN_DISC_ADV_INT_MIN,       //!< Minimum advertising interval, when in General discoverable mode (n * 0.625 mSec)
	AT_BLE_GAP_GEN_DISC_ADV_INT_MAX,       //!< Maximum advertising interval, when in General discoverable mode (n * 0.625 mSec)
	AT_BLE_GAP_CONN_ADV_INT_MIN,           //!< Minimum advertising interval, when in Connectable mode (n * 0.625 mSec)
	AT_BLE_GAP_CONN_ADV_INT_MAX,           //!< Maximum advertising interval, when in Connectable mode (n * 0.625 mSec)
	AT_BLE_GAP_CONN_SCAN_INT,              //!< Scan interval used during Link Layer Initiating state, when in Connectable mode (n * 0.625 mSec)
	AT_BLE_GAP_CONN_SCAN_WIND,             //!< Scan window used during Link Layer Initiating state, when in Connectable mode (n * 0.625 mSec)
	AT_BLE_GAP_CONN_HIGH_SCAN_INT,         //!< Scan interval used during Link Layer Initiating state, when in Connectable mode, high duty scan cycle scan paramaters (n * 0.625 mSec)
	AT_BLE_GAP_CONN_HIGH_SCAN_WIND,        //!< Scan window used during Link Layer Initiating state, when in Connectable mode, high duty scan cycle scan paramaters (n * 0.625 mSec)
	AT_BLE_GAP_GEN_DISC_SCAN_INT,          //!< Scan interval used during Link Layer Scanning state, when in General Discovery proc (n * 0.625 mSec)
	AT_BLE_GAP_GEN_DISC_SCAN_WIND,         //!< Scan window used during Link Layer Scanning state, when in General Discovery proc (n * 0.625 mSec)
	AT_BLE_GAP_LIM_DISC_SCAN_INT,          //!< Scan interval used during Link Layer Scanning state, when in Limited Discovery proc (n * 0.625 mSec)
	AT_BLE_GAP_LIM_DISC_SCAN_WIND,         //!< Scan window used during Link Layer Scanning state, when in Limited Discovery proc (n * 0.625 mSec)
	AT_BLE_GAP_CONN_EST_ADV,               //!< Advertising interval, when using Connection Establishment proc (n * 0.625 mSec). Obsolete - Do not use.
	AT_BLE_GAP_CONN_EST_INT_MIN,           //!< Minimum Link Layer connection interval, when using Connection Establishment proc (n * 1.25 mSec)
	AT_BLE_GAP_CONN_EST_INT_MAX,           //!< Maximum Link Layer connection interval, when using Connection Establishment proc (n * 1.25 mSec)
	AT_BLE_GAP_CONN_EST_SCAN_INT,          //!< Scan interval used during Link Layer Initiating state, when using Connection Establishment proc (n * 0.625 mSec)
	AT_BLE_GAP_CONN_EST_SCAN_WIND,         //!< Scan window used during Link Layer Initiating state, when using Connection Establishment proc (n * 0.625 mSec)
	AT_BLE_GAP_CONN_EST_SUPERV_TIMEOUT,    //!< Link Layer connection supervision timeout, when using Connection Establishment proc (n * 10 mSec)
	AT_BLE_GAP_CONN_EST_LATENCY,           //!< Link Layer connection slave latency, when using Connection Establishment proc (in number of connection events)
	AT_BLE_GAP_CONN_EST_MIN_CE_LEN,        //!< Local informational parameter about min len of connection needed, when using Connection Establishment proc (n * 0.625 mSec)
	AT_BLE_GAP_CONN_EST_MAX_CE_LEN,        //!< Local informational parameter about max len of connection needed, when using Connection Establishment proc (n * 0.625 mSec)
	AT_BLE_GAP_PRIVATE_ADDR_INT,           //!< Minimum Time Interval between private (resolvable) address changes. In minutes (default 15 minutes)
	AT_BLE_GAP_CONN_PAUSE_CENTRAL,         //!< Central idle timer. In seconds (default 1 second)
	AT_BLE_GAP_CONN_PAUSE_PERIPHERAL      //!< Minimum time upon connection establishment before the peripheral starts a connection update procedure. In seconds (default 5 seconds)
}at_ble_parameters_t;


/// GAP Advertising types
typedef enum{
	AT_BLE_ADV_TYPE_UNDIRECTED    = 0x00,   /**< Connectable undirected. */
	AT_BLE_ADV_TYPE_DIRECTED,      		/**< Connectable directed. */
	AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED,        /**< Scannable undirected. */
	AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED,     /**< Non connectable undirected. */
	AT_BLE_ADV_TYPE_SCAN_RESPONSE                  /** only used in @ref AT_BLE_SCAN_INFO event to signify a scan response*/
}at_ble_adv_type_t;


 /**@brief Events delivered from BLE stack to the application layer
 */
typedef enum{
	/* GAP events */
	/** Scan info needs to be delivered : either adv data or scan response data */
	AT_BLE_SCAN_INFO, 
	/** connected to a peer device */
	AT_BLE_CONNECTED, 
	/** peer device connection terminated */
	AT_BLE_DISCONNECTED, 
	 /** connection parameters updated */
	AT_BLE_CONN_PARAM_UPDATE_DONE,
	 /** peer device asks for connection params update */
	AT_BLE_CONN_PARAM_UPDATE_REQUEST,
	 /** reported RX power value */
	AT_BLE_RX_POWER_VALUE,
	/** Pairing procedure is completed */
	AT_BLE_PAIR_DONE, 
	/** A central device asks for Pairing */
	AT_BLE_PAIR_REQUEST, 
	/** Slave security request **/
	AT_BLE_SLAVE_SEC_REQUEST,
	/** A passkey or OOB data is requested as part of pairing procedure */
	AT_BLE_PAIR_KEY_REQUEST, 
	/** Encryption is requested by a master device */
	AT_BLE_ENCRYPTION_REQUEST, 
	/** Encryption status changed */
	AT_BLE_ENCRYPTION_STATUS_CHANGED, 
	/* GATT Client events */
	/** A primary service is found */
	AT_BLE_PRIMARY_SERVICE_FOUND, 
	/** An included service is found */
	AT_BLE_INCLUDED_SERVICE_FOUND, 
	/** A Characteristic is found */
	AT_BLE_CHARACTERISTIC_FOUND, 
	 /** A descriptor is found */
	AT_BLE_DESCRIPTOR_FOUND,
	/** Characteristic read by UUID procedure is done */
	AT_BLE_CHARACTERISTIC_READ_BY_UUID_RESPONSE, 
	 /** Characteristic read procedure is done */
	AT_BLE_CHARACTERISTIC_READ_RESPONSE,
	/** Characteristic multible read procedure is done */
	AT_BLE_CHARACTERISTIC_READ_MULTIBLE_RESPONSE, 
	/** Charactristic write procedure is done */
	AT_BLE_CHARACTERISTIC_WRITE_RESPONSE, 
	/** A Notification is recieved */
	AT_BLE_NOTIFICATION_RECIEVED, 
	 /** An Indication is recieved */
	AT_BLE_INDICATION_RECIEVED,
	/* GATT Server events */
	 /** The peer confirmed that it has recieved an Indication */
	AT_BLE_INDICATION_CONFIRMED,
	/** The peer has changed a characteristic value */
	AT_BLE_CHARACTERISTIC_CHANGED, 
	/** The peer has confirmed that it has recieved the service chnaged notifiaction */
	AT_BLE_SERVICE_CHANGED_NOTIFICATION_CONFIRMED, 
	/** The peer asks for a write Authorization */
	AT_BLE_WRITE_AUTHORIZE_REQUEST, 
	 /** The peer asks for a read Authorization */
	AT_BLE_READ_AUTHORIZE_REQUEST,
	/* L2CAP events */
	/** An L2CAP packet recieved from a registered custom CID */
	AT_BLE_L2CAP_RX, 
	/* HTPT Health Thermometer Profile events */
	/** Inform APP of database creation status */
	AT_BLE_HTPT_CREATE_DB_CFM, 
	/** Error indication to APP*/
	AT_BLE_HTPT_ERROR_IND,
	/** Automatically sent to the APP after a disconnection with the peer device to confirm disabled profile*/
	AT_BLE_HTPT_DISABLE_IND,
	/** Temperature value confirm to APP*/
	AT_BLE_HTPT_TEMP_SEND_CFM,
	/** Inform APP of new measurement interval value */
	AT_BLE_HTPT_MEAS_INTV_CHG_IND,
	/** Inform APP of new configuration value*/
	AT_BLE_HTPT_CFG_INDNTF_IND,
	/* Custom user defined events */
	/** A user-defined event is delivered to the system */
	AT_BLE_CUSTOM_EVENT, 

}at_ble_events_t; 


 /**@brief BLE can accept to kinds of addresses, either public or random addresses 
 */
typedef enum{
	/** a public static address */
	AT_BLE_ADDRESS_PUBLIC, 
	/** a random static address */
	AT_BLE_ADDRESS_RANDOM_STATIC, 
	/** resolvable private random address */
	AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE,
	/** non-resolvable private random address */ 
	AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE ,
	
}at_ble_addr_type_t;


 /**@brief disconnection reasons 
 */
typedef enum{
	AT_BLE_TERMINATED_BY_USER,
	AT_BLE_UNACCEPTABLE_INTERVAL,
}at_ble_disconnect_reason_t;

 /**@brief a device IO cababilities
 */
typedef enum{
	/** Can only display */ 
    AT_BLE_IO_CAP_DISPLAY_ONLY, 
	/** Can Display and get a Yes/No input from user*/
    AT_BLE_IO_CAP_DISPLAY_YES_NO, 
	/** Has only a keyboard */
    AT_BLE_IO_CAP_KB_ONLY, 
	/** Has no input and no output */
    AT_BLE_IO_CAP_NO_INPUT_NO_OUTPUT,
	/** Has both a display and a keyboard */
    AT_BLE_IO_CAP_KB_DISPLAY, 

}at_ble_iocab_t;

 /**@brief Security keys distribution list
 */
typedef enum{
	/** No keys to distribute */
    AT_BLE_KEY_DIST_NONE = 0x00,
	/** Distribute Encryption Key */
    AT_BLE_KEY_DIST_ENC = (1 << 0), 
	/** Distribute ID Key */
    AT_BLE_KEY_DIST_ID  = (1 << 1),
	/** Distribute Signiture Key */
    AT_BLE_KEY_DIST_SIGN = (1 << 2), 
	/** Distribute All Keys */
	AT_BLE_KEY_DIS_ALL = AT_BLE_KEY_DIST_ENC  
				| AT_BLE_KEY_DIST_ID
				| AT_BLE_KEY_DIST_SIGN,

}at_ble_key_dis_t;

 /**@brief Security authentication level
 */
typedef enum{
	/** no security */
    AT_BLE_NO_SEC = 0x00, 
	/** Gap Mode 1 Level 1, Unauthenticated pairing with encryption*/
    AT_BLE_MODE1_L1_NOAUTH_PAIR_ENC, 
	/** Gap Mode 1 Level 2, Authenticated pairing with encryption */
    AT_BLE_MODE1_L2_AUTH_PAIR_ENC, 
	/**  Gap Mode 2 Level 1, Unauthenticated pairing with data signing */
    AT_BLE_MODE2_L1_NOAUTH_DATA_SGN, 
	/**  Gap Mode 2 Level 2, Authentication pairing with data signing */
    AT_BLE_MODE2_L2_AUTH_DATA_SGN, 

}at_ble_auth_levels_t;

 /**@brief UUID Type
 */
typedef enum{
	AT_BLE_UUID_16,
	AT_BLE_UUID_32,
	AT_BLE_UUID_128,
}at_ble_uuid_type_t;

 /**@brief Characterisitc properties
 */
typedef enum{
	AT_BLE_CHAR_BROADCST = (1 << 0),
	AT_BLE_CHAR_READ = (1 << 1),
	AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE = (1 << 2),
	AT_BLE_CHAR_WRITE = (1 << 3),
	AT_BLE_CHAR_NOTIFY = (1 << 4),
	AT_BLE_CHAR_INDICATE = (1 << 5),
	AT_BLE_CHAR_SIGNED_WRITE = (1 << 6),
	AT_BLE_CHAR_RELIABLE_WRITE = (1 << 7),
	AT_BLE_CHAR_WRITEABLE_AUX = (1 << 8),
}at_ble_char_properties_t;

 /**@brief Advertising Filter Policy
 */
typedef enum{
	AT_BLE_ADV_FP_ANY,
	AT_BLE_ADV_FP_FILTER_SCANREQ,
	AT_BLE_ADV_FP_FILTER_CONNREQ,
	AT_BLE_ADV_FP_FILTER_BOTH
}at_ble_filter_type_t;

 /**@brief Advertising Mode
 */
typedef enum{
	 /// Mode in non-discoverable
    AT_BLE_ADV_NON_DISCOVERABLE,
    /// Mode in general discoverable
    AT_BLE_ADV_GEN_DISCOVERABLE,
    /// Mode in limited discoverable
    AT_BLE_ADV_LIM_DISCOVERABLE,
    /// Broadcaster mode which is a non discoverable and non connectable mode.
    AT_BLE_ADV_BROADCASTER_MODE
}at_ble_adv_mode_t; 

 /**@brief Scan modes used at @ref at_ble_scan_start
 */
typedef enum{
	/** General Discovery mode */
	AT_BLE_SCAN_GEN_DISCOVERY, 
	/** Limited Discovery mode */
	AT_BLE_SCAN_LIM_DISCOVERY, 
	/** Observer only */
	AT_BLE_SCAN_OBSERVER_MODE 
}at_ble_scan_mode_t; 

 /**@brief Scan types used at @ref at_ble_scan_start
 */
typedef enum{
	AT_BLE_SCAN_PASSIVE,
	AT_BLE_SCAN_ACTIVE
}at_ble_scan_type_t;

 /**@brief Attribute Permission
 */
typedef enum{
	AT_BLE_ATTR_NO_PERMISSIONS = 0x00,
	
	AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR = 0x01,
	AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR = 0x02,
	AT_BLE_ATTR_READABLE_NO_AUTHN_REQ_AUTHR = 0x03,
	AT_BLE_ATTR_READABLE_REQ_AUTHN_REG_AUTHR = 0x04,

	AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR = 0x10,
	AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR = 0x20,
	AT_BLE_ATTR_WRITABLE_NO_AUTHN_REQ_AUTHR = 0x30,
	AT_BLE_ATTR_WRITABLE_REQ_AUTHN_REG_AUTHR = 0x40,

}at_ble_attr_permissions_t;

typedef enum{
	AT_BLE_PRES_FORMAT_BOOLEAN = 0X01,
	AT_BLE_PRES_FORMAT_2BIT = 0X02,
	AT_BLE_PRES_FORMAT_NIBBLE = 0X03,
	AT_BLE_PRES_FORMAT_UINT8 = 0X04,
	AT_BLE_PRES_FORMAT_UINT12 = 0X05,
	AT_BLE_PRES_FORMAT_UINT16 = 0X06,
	AT_BLE_PRES_FORMAT_UINT24 = 0X07,
	AT_BLE_PRES_FORMAT_UINT32 = 0X08,
	AT_BLE_PRES_FORMAT_UINT48 = 0X09,
	AT_BLE_PRES_FORMAT_UINT64 = 0X0A,
	AT_BLE_PRES_FORMAT_UINT128 = 0X0B,
	AT_BLE_PRES_FORMAT_SINT8 = 0X0C,
	AT_BLE_PRES_FORMAT_SINT12 = 0X0D,
	AT_BLE_PRES_FORMAT_SINT16 = 0X0E,
	AT_BLE_PRES_FORMAT_SINT24 = 0X0F,
	AT_BLE_PRES_FORMAT_SINT32 = 0X10,
	AT_BLE_PRES_FORMAT_SINT48 = 0X11,
	AT_BLE_PRES_FORMAT_SINT64 = 0X12,
	AT_BLE_PRES_FORMAT_SINT128 = 0X13,
	AT_BLE_PRES_FORMAT_FLOAT32 = 0X14,
	AT_BLE_PRES_FORMAT_FLOAT64 = 0X15,
	AT_BLE_PRES_FORMAT_SFLOAT = 0X16,
	AT_BLE_PRES_FORMAT_FLOAT = 0X17,
	AT_BLE_PRES_FORMAT_DUINT16 = 0X18,
	AT_BLE_PRES_FORMAT_UTF8S = 0X19,
	AT_BLE_PRES_FORMAT_UTF16S = 0X1A,
	AT_BLE_PRES_FORMAT_STRUCT = 0X1B,
}at_ble_char_pres_format_t;

 /**@brief Pairing Key Type
 */
typedef enum
{	/** A passkey is required @ref at_ble_passkey_type_t for more details */
	AT_BLE_PAIR_PASSKEY,
	/** Out of band key is required */
	AT_BLE_PAIR_OOB
}at_ble_pair_key_type_t;
 /**@brief Passkey Type
 */
typedef enum
{
	/** A passkey entry required */
	AT_BLE_PAIR_PASSKEY_ENTRY,
	 /** A passkey needs to be generated and displayed as part of pairing procedure */
	AT_BLE_PAIR_PASSKEY_DISPLAY
}at_ble_passkey_type_t;



/**@brief HTPT Database Configuration Flags
*/
typedef enum 
{
    ///Indicate if Temperature Type Char. is supported
    HTPT_TEMP_TYPE_CHAR_SUP        = 0x01,
    ///Indicate if Intermediate Temperature Char. is supported
    HTPT_INTERM_TEMP_CHAR_SUP      = 0x02,
    ///Indicate if Measurement Interval Char. is supported
    HTPT_MEAS_INTV_CHAR_SUP        = 0x04,
    ///Indicate if Measurement Interval Char. supports indications
    HTPT_MEAS_INTV_IND_SUP         = 0x08,
    ///Indicate if Measurement Interval Char. is writable
    HTPT_MEAS_INTV_WR_SUP          = 0x10,

    /// All Features supported
    HTPT_ALL_FEAT_SUP              = 0x1F,
}at_ble_htpt_db_config_flag;

/**@brief Service Security Level
*/

typedef enum
{
	//Service can be browsed but attributes cannot be accessed
	HTPT_DISABLE = 0x00,
	//Service can be browsed and attributes can be accessed according to attribute permission
	HTPT_ENABLE = 0x01,
	//Service can be used with minimum Unauthenticated Link (Just Work Pairing)
	HTPT_UNAUTH = 0x02,
	//Service can be used with Authenticated Link (Pin code Pairing)
	HTPT_AUTH = 0x03
}at_ble_htpt_sec_level;

/**@brief Attribute Table Indexes
*/
typedef enum
{
    HTPT_TEMP_MEAS_CHAR,
    HTPT_TEMP_TYPE_CHAR,
    HTPT_INTERM_TEMP_CHAR,
    HTPT_MEAS_INTV_CHAR,

    HTPT_CHAR_MAX
}at_ble_htpt_attr_index;

/**@brief Temperature Measurement Flags field bit values
*/
typedef enum
{
    /// Temperature Units Flag - Celsius
    HTPT_FLAG_CELSIUS             = 0x00,
    /// Temperature Units Flag - Fahrenheit
    HTPT_FLAG_FAHRENHEIT,
    /// Time Stamp Flag
    HTPT_FLAG_TIME,
   /// Temperature Type Flag
    HTPT_FLAG_TYPE                 = 0x04
}at_ble_htpt_temp_flags;


/**@brief Temperature Type Description 
*/
typedef enum 
{
    /// Armpit
    HTP_TYPE_ARMPIT                   = 0x01,
    /// Body (general)
    HTP_TYPE_BODY                     = 0x02,
    /// Ear (usually ear lobe)
    HTP_TYPE_EAR                      = 0x03,
    /// Finger
    HTP_TYPE_FINGER                   = 0x04,
    /// Gastro-intestinal Tract
    HTP_TYPE_GASTRO_INTESTINAL_TRACT  = 0x05,
    /// Mouth
    HTP_TYPE_MOUTH                    = 0x06,
    /// Rectum
    HTP_TYPE_RECTUM                   = 0x07,
    /// Toe
    HTP_TYPE_TOE                      = 0x08,
    /// Tympanum (ear drum)
    HTP_TYPE_TYMPANUM                 = 0x09
}at_ble_htpt_temp_type;

/**@brief Connection Type 
*/
typedef enum 
{
	HTPT_CONFIG_CONN= 0,
	HTPT_NORMAL_CONN=1
}at_ble_htpt_conn_type;

/**@brief Enable/Disable Notification/Indication 
*/
typedef enum 
{
    /// Stop notification/indication
    HTPT_STOP_NTFIND = 0x0000,
    /// Start notification
    HTPT_START_NTF,
    /// Start indication
    HTPT_START_IND,
}at_ble_htpt_ntfind;


/**@brief Enable Notification/Indication for HTPT characteristics
*/
typedef enum 
{
    /// Stable measurement interval indication enabled
    HTPT_CFG_STABLE_MEAS_IND    = (1 << 0),
    /// Intermediate measurement notification enabled
    HTPT_CFG_INTERM_MEAS_NTF    = (1 << 1),
    /// Measurement interval indication
    HTPT_CFG_MEAS_INTV_IND      = (1 << 2),
}at_ble_htpt_ntf_ind_cfg;

/****************************************************************************************
*							        Structures                                     							*
****************************************************************************************/

/**@brief Bluetooth Low Energy address Type. */
typedef struct
{
  at_ble_addr_type_t type;					/**< See @ref AT_BLE_GAP_ADVERTISING_TYPE */
  uint8_t addr[AT_BLE_ADDR_LEN];		/**< 48-bit address, LSB format. */
} at_ble_addr_t;



/**@brief Identity Resolving Key. */
typedef struct
{
  uint8_t irk[AT_BLE_MAX_KEY_LEN];   /**< Array containing IRK. */
} at_ble_gap_irk_t;


/**@brief Whitelist structure. */
typedef struct
{
  at_ble_addr_t   ** ppAddrs;        /**< Pointer to array of device address pointers, pointing to addresses to be used in whitelist. NULL if none are given. */
  uint8_t             addrCount;      /**< Count of device addresses in array, up to @ref AT_BLE_GAP_WHITELIST_ADDR_MAX_COUNT. */
  at_ble_gap_irk_t    ** ppIrks;         /**< Pointer to array of Identity Resolving Key (IRK) pointers, each pointing to an IRK in the whitelist. NULL if none are given. */
  uint8_t             irkCount;       /**< Count of IRKs in array, up to @ref AT_BLE_GAP_WHITELIST_IRK_MAX_COUNT. */
} at_ble_gap_whitelist_t;


/** @brief Connection parameters
*/
typedef struct
{
    /// Minimum of connection interval
    uint16_t             con_intv_min;
    /// Maximum of connection interval
    uint16_t             con_intv_max;
    /// Connection latency
    uint16_t             con_latency;
    /// Link supervision timeout
    uint16_t             superv_to;
    /// Minimum CE length
    uint16_t             ce_len_min;
    /// Maximum CE length
    uint16_t             ce_len_max;

}at_ble_connection_params_t;

/** @brief UUID type
*/
typedef struct
{
	at_ble_uuid_type_t type; /**< either a 16 bit UUID or a 128 bit UUID */
	uint8_t uuid[AT_BLE_UUID_128_LEN]; /**< if 16 bit UUID only bytes [0] and [1] are used, if 128 bit UUID all bits are used */
}at_ble_uuid_t;

/** @brief Characteristic presentation format
*/
typedef struct
{
	at_ble_char_pres_format_t format; /**< Value format */
	int8_t exponent; /**< Value Exponent */
	uint16_t unit; /**<  as defined in GATT spec Part G, Section 3.3.3.5.4 */
	uint8_t name_space; /**<  as defined in GATT spec Part G, Section 3.3.3.5.5 */
	uint16_t description; /**<  as defined in GATT spec Part G, Section 3.3.3.5.6 */

}at_ble_char_presentation_t;

/** @brief Characteristic structure
*/
typedef struct
{
	at_ble_handle_t char_val_handle; /**< Here the stack will store the char. value handle for future use */
	at_ble_uuid_t uuid; /**< Characteristic UUID */
	at_ble_char_properties_t properties; /**< Characteristic properties, values for Client Characteristic Configuration Descriptor and Server Characteristic Configuration Descriptor will be decided from this value*/
	
	uint8_t* init_value; /**< initial value of this characteristic  */
	uint16_t value_init_len; /**< initial value length */
	uint16_t value_max_len; /**< maximum possible length of the char. value */
	at_ble_attr_permissions_t value_permissions; /**< Value permissions */ //TODO: can this value be deduced from properties field ?
	
	uint8_t* user_desc; /**< a user friendly description, this value will be stored in the relevant descriptor, if no user description is desired set to NULL */
	uint16_t user_desc_len; /**< the user friendly description length, this value will be stored in the relevant descriptor, if no user description is desired set to 0*/
	uint16_t user_desc_max_len; /**< Maximum possible length for the user friendly description, this value will be stored in the relevant descriptor, if no user description is desired set to 0 */

	at_ble_char_presentation_t* presentation_format; /**< Characteristic presentation format, this value will be stored in the relevant descriptor, if no presentation format is necessary set to NULL */

}at_ble_characteristic_t;

/** @brief Service Definition
*/
typedef struct
{
	at_ble_uuid_t uuid; /**< UUID of the included Service, only 16 bit UUID are allowed */
	at_ble_handle_t service_handle; /**< The included service handle */
	at_ble_handle_t end_group_handle; /**< End group handle */
	
}at_ble_included_service_t;

/** @brief Pairing Features
*/  
typedef struct
{
	at_ble_iocab_t io_cababilities; /**<  IO capabilities of this device */
	bool mitm_protection; /**<  if ture then Man In The Middle protection must be provided */
	bool bond; /**< if True then bonding is requested */
	bool oob_avaiable; /**< if true then out-of-band data is avaialble */
	uint8_t min_key_size; /**< minimum key size accepted */
	uint8_t max_key_size; /**< maximum key size accepted */
	at_ble_key_dis_t initiator_keys; /**< security keys offered to peer device */
	at_ble_key_dis_t responder_keys; /**< security keys needed from peer device */ 
	at_ble_auth_levels_t desired_auth; /**< The desired authentication level to be reached */
}at_ble_pair_features_t;

/** @brief Long Term Key
*/  
typedef struct
{
    /// Long Term Key
    uint8_t key[AT_BLE_MAX_KEY_LEN];
    /// Encryption Diversifier
    uint16_t ediv;
    /// Random Number (64 bit)
    uint8_t     nb[8];
    /// Encryption key size (7 to 16)
    uint8_t key_size;
	
}at_ble_LTK_t;

/** @brief Connection Signature Resolution Key
*/
typedef struct
{
    uint8_t key[AT_BLE_MAX_KEY_LEN];
}at_ble_CSRK_t;

typedef struct
{
    /// Identity Resolving Key
    uint8_t key[AT_BLE_MAX_KEY_LEN];
    /// Device BD Address
    at_ble_addr_t addr;

}at_ble_IRK_t;
		
typedef struct
{
	at_ble_adv_type_t type;
	at_ble_addr_t dev_addr;
	uint8_t adv_data[AT_BLE_ADV_MAX_SIZE];
	uint8_t adv_data_len;

}at_ble_scan_info_t;


typedef struct
{
	at_ble_status_t status;
	uint8_t devices_found;

}at_ble_scan_report_t;

 /**@brief AT_BLE_CONNECTED message format. This message is sent to the app when the link request is complete.
 */
typedef struct
{
	at_ble_addr_t peer_addr;
	at_ble_handle_t handle;

}at_ble_connected_t;

typedef struct
{
	at_ble_handle_t handle;
	uint8_t reason;

}at_ble_disconnected_t;

typedef struct
{
	at_ble_handle_t handle;
	at_ble_status_t status;
	uint16_t con_intv;
	uint16_t con_latency;
	uint16_t superv_to;

}at_ble_conn_param_update_done_t;

typedef struct
{
	at_ble_handle_t handle;
	at_ble_connection_params_t params;

}at_ble_conn_param_update_request_t;

typedef struct
{
	at_ble_handle_t handle;
	uint16_t rx_value;
	
}at_ble_rx_power_value_t;

/** @brief pairing done msg
*/
typedef struct
{
	uint8_t auth;
	at_ble_handle_t handle;
	at_ble_status_t status;
	at_ble_LTK_t peer_ltk;
	at_ble_CSRK_t peer_csrk;
	at_ble_IRK_t peer_irk;

}at_ble_pair_done_t;

typedef struct
{
	at_ble_handle_t handle;
	at_ble_pair_features_t peer_features;
	
}at_ble_pair_request_t;

typedef struct
{
	at_ble_handle_t handle;
	at_ble_pair_key_type_t type;
	at_ble_passkey_type_t passkey_type;

}at_ble_pair_key_request_t;

typedef struct
{
	at_ble_handle_t handle;
	bool bond;
	bool mitm_protection;

}at_ble_slave_sec_request_t;

typedef struct
{
	at_ble_handle_t handle;
	/// Encryption Diversifier
    uint16_t ediv;
    /// Random Number (64 bit)
    uint8_t     nb[8];

}at_ble_encryption_request_t;

typedef struct
{
	at_ble_handle_t handle;
	at_ble_status_t status;

}at_ble_encryption_status_changed_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t start_handle;
	at_ble_handle_t end_handle;
	at_ble_uuid_t service_uuid;
	
}at_ble_primary_service_found_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t attr_handle;
	at_ble_handle_t start_handle;
	at_ble_handle_t end_handle;
	at_ble_uuid_t service_uuid;

}at_ble_included_service_found_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	at_ble_handle_t value_handle;
	uint8_t properties;
	at_ble_uuid_t char_uuid;

}at_ble_characteristic_found_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t desc_handle;
	at_ble_uuid_t desc_uuid;

}at_ble_descriptor_found_t;


typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	uint16_t char_len;
	uint16_t char_offset;
	uint8_t char_value[AT_BLE_MAX_ATT_LEN];
}at_ble_characteristic_read_response_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	at_ble_status_t status;

}at_ble_characteristic_write_response_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	uint8_t char_value[AT_BLE_MAX_ATT_LEN];
	uint16_t char_len;
}at_ble_notification_recieved_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	uint8_t char_value[AT_BLE_MAX_ATT_LEN];
	uint16_t char_len;
}at_ble_indication_recieved_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	at_ble_status_t status;
	
}at_ble_indication_confirmed_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	uint8_t char_new_value[AT_BLE_MAX_ATT_LEN];
	uint16_t char_len;
	uint16_t char_offset;
	at_ble_status_t status;
	
}at_ble_characteristic_changed_t;

typedef struct
{
	at_ble_handle_t conn_handle;

}at_ble_service_changed_notification_confirmed_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	at_ble_status_t status;

}at_ble_write_authorize_request_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	at_ble_handle_t char_handle;
	at_ble_status_t status;

}at_ble_read_authorize_request_t;

typedef struct
{
	at_ble_handle_t conn_handle;
	uint16_t cid;
	uint16_t len;
	uint8_t* data;

}at_ble_l2cap_rx_t;


/** @brief Parameters of the @ref HTPT_CREATE_DB_CFM message
*/

typedef struct 
{
    ///Status
    uint8_t status;
}at_ble_htpt_create_db_cfm_t;

/** @brief Parameters of the @ref HTPT_DISABLE_IND message
*/
typedef struct 
{
    ///Connection handle
    uint16_t conhdl;

    ///Temperature measurement indication configuration
    uint16_t temp_meas_ind_en;
    ///Intermediate temperature notification configuration
    uint16_t interm_temp_ntf_en;
    ///Measurement interval indication configuration
    uint16_t meas_intv_ind_en;
    ///Measurement interval
    uint16_t meas_intv;
}at_ble_htpt_disable_ind_t;

/** @brief Parameters of the @ref HTPT_TEMP_UPD_CFM message
*/

typedef struct 
{
    ///Connection handle
    uint16_t conhdl;
    ///Status
    uint8_t status;
    ///Confirmation Type
    uint8_t cfm_type;
}at_ble_htpt_temp_send_cfm_t;

/** @brief Parameters of the @ref HTPT_MEAS_INTV_CHG_IND message
*/

typedef struct 
{
    uint16_t intv;
}at_ble_htpt_meas_intv_chg_ind_t;

/** @brief Parameters of the @ref HTPT_CFG_INDNTF_IND message
*/

typedef struct
{
    ///Connection handle
    uint16_t conhdl;
    ///Stop/notify/indicate value to configure into the peer characteristic
    uint16_t cfg_val;
    ///Own code for differentiating between Temperature Measurement, Intermediate Temperature and Measurement Interval chars
    at_ble_htpt_attr_index char_code;
}at_ble_htpt_cfg_indntf_ind_t;

/****************************************************************************************
*                                       Functions                                       *
****************************************************************************************/

/**
@defgroup gap GAP API

@{
*/

/**
 ****************************************************************************************
 * @brief  Reset the link layer and the host
 *
 ****************************************************************************************
 */
at_ble_status_t at_ble_init(void* args);

 /** @brief set device name
  *
  * @param[in] dev_name the new device name
  * @param[in] len name length
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_device_name_set(uint8_t* dev_name, uint8_t len);

/** @brief set device Identity Resolving Key 
  *
  * @param[in] irk device IRK used for resolvable random BD address generation
  * @param[in] interval duration before regenerate device address
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_set_privacy_key(at_ble_gap_irk_t* irk , uint16_t interval);

 /** @brief set the local device BT Address
  *
  * @param[in] address the new BT address
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_addr_set(at_ble_addr_t* address);

void at_fw_patch_download(uint32_t* patch, uint32_t patch_size);

 /**@brief get the local device BT Address
  *
  * @param[out] address the current BT address
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_addr_get(at_ble_addr_t* address);

 /** @brief extracts the next event form the event queue
  *
  * @param[out] event The received event
  * @param[out] params Received event data
  * @param[in] timeout Time in millisecond this function will block waiting of an event (0xFFFFFFFF to block indefinitely)
  
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_event_get(at_ble_events_t* event, void* params,
	uint32_t timeout);

 /** @brief Posts a user defined custome event into the event queue, allowing the user code 
  *        to process other events in the same event handling loop. Events willbe recieved with the
  *        as @ref AT_BLE_CUSTOM_EVENT
  *
  * @param[in] params Parameters sent to the custom event
  *  
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_event_user_defined_post(void* params);

 
/**
 * @brief Change the GAP related parameters.
 *
 * @param[in] param         Paramter to set
 * @param[in] value_buff    Buffer where value is stored
 * @param[in] value_len		Value length
 *
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t at_ble_gap_set_param(at_ble_parameters_t param,
                       uint8_t* value_buff, uint16_t value_len);


/**
* @brief	   Fetch a GAP Parameter value.
*
* @param[in] param Paramter to get
* @param[out] value_buff    Buffer to store the value in
* @param[out] value_len		Value length
*
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
*/
at_ble_status_t at_ble_gap_get_param(at_ble_parameters_t param,
						uint8_t* value_buff, uint16_t value_len);

/**
 * @brief Adds a peer device address to the whitelist
 *
 * @param[in] address       peer device address
 *
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t at_ble_whitelist_add(at_ble_addr_t* address);

/**
 * @brief Removes a peer device address from the whitelist
 *
 * @param[in] address       peer device address
 *
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t at_ble_whitelist_remove(at_ble_addr_t* address);

/**
 * @brief Clear the whitelist
 *
 *
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t at_ble_whitelist_clear(void);

/**@brief Set, clear or update advertisement and scan response data.
 *
 *
 * @note: To clear the advertisement data and set it to a 0-length packet, simply provide a valid pointer (pAdvData/pSrData) with its corresponding 
 *        length (dlen/srdlen) set to 0.
 *
 * @note: The call will fail if pAdvData and pSrData are both NULL since this would have no effect.
 *
 * @param[in] adv_data    Raw data to be placed in advertisement packet. If NULL, no changes are made to the current advertisement packet data.
 * @param[in] adv_data_len      Data length for adv_data.
 * @param[in] scan_resp_data Raw data to be placed in scan response packet. If NULL, no changes are made to the current scan response packet data.
 * @param[in] scan_response_data_len    Data length for scan_resp_data
 *
 * @return @ref AT_BLE_SUCCESS Advertisement data successfully updated or cleared.
 * @return @ref AT_BLE_FAILURE Invalid pointer supplied.
 *
 */
at_ble_status_t at_ble_adv_data_set( uint8_t const *const  adv_data,  
  uint8_t  adv_data_len,  
  uint8_t const *const  scan_resp_data,  
  uint8_t  scan_response_data_len  
 ); 


 /**@brief Start advertising (GAP Discoverable, Connectable modes, Broadcast Procedure).
  *
  * @param[in] type Advertising type, see @ref at_ble_adv_type_t for more info
  * @param[in] mode either non-discoverable,General, Limited or Broadcaster , @ref at_ble_adv_mode_t for more details
  * @param[in] peer_addr For @ref AT_BLE_ADV_TYPE_DIRECTED mode only, known peer address
  * @param[in] filtered policy @ref at_ble_filter_type_t for more details
  * @param[in] interval Advertising interval between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s). This parameter must be set to 0 if type equals @ref AT_BLE_ADV_TYPE_DIRECTED
  * @param[in] timeout Advertising timeout between 0x0001 and 0x3FFF in seconds, 0x0000 disables timeout. This parameter must be set to 0 if type equals @ref AT_BLE_ADV_TYPE_DIRECTED
  * @param[in] disable_randomness if True the device will advertise at the exact interval without adding the random 10msec
  *
  * @return @ref AT_BLE_SUCCESS The BLE stack has started advertising.
  * @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
  * @return @ref AT_BLE_FAILURE Failed to start advertising.
  ****************************************************************************************
  */
at_ble_status_t at_ble_adv_start(at_ble_adv_type_t type,at_ble_adv_mode_t mode ,
		  at_ble_addr_t* peer_addr, at_ble_filter_type_t filtered, 
		  uint16_t interval, uint16_t timeout, bool disable_randomness);


 
 /**@brief Stop advertising (GAP Discoverable, Connectable modes, Broadcast Procedure).
  *
  * @return @ref AT_BLE_SUCCESS The BLE stack has stopped advertising.
  * @return @ref AT_BLE_FAILURE Error to stop advertising (most probably not in advertising state).
  */
at_ble_status_t at_ble_adv_stop(void);

 /**@brief Set the Advertising transmission power
  *
  * @param[in] power    new tx power in dbm
  *
  * @return @ref AT_BLE_SUCCESS TX value changed successfully.
  * @return @ref AT_BLE_INVALID_PARAM TX value is out of range.
  * @return @ref AT_BLE_FAILURE Other error.
  */
at_ble_status_t at_ble_adv_set_tx_power(int8_t power);

 /**@brief Start scan operation
  *
  * while the scan operation is ongoing, the application will recieve @ref AT_BLE_SCAN_DEV_FOUND
  * event per found device, and @ref AT_BLE_SCAN_REPORT event at the end of the scan
  *
  * @param[in] interval scan interval in 625us units
  * @param[in] window   scan window in 625us units
  * @param[in] timeout  scan timeout, 0x0000 disables timeout.
  * @param[in] type  controls the type of scan to perform either Passive or Active @ref at_ble_scan_type_t for more details.
  * @param[in] mode     either General, Limited or Observer only, @ref at_ble_scan_mode_t for more details
  * @param[in] filter_whitelist    if true, scan only from whitelisted devices. if false scan from all devices
  * @param[in] filter_dublicates   if true, scan event will be generated only once per device, if false multible events will be issued
  *
  * @return @ref AT_BLE_SUCCESS scan operation programmed successfully .
  * @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_scan_start(uint16_t interval, uint16_t window, 
	uint16_t timeout, at_ble_scan_type_t type ,at_ble_scan_mode_t mode,
	bool filter_whitelist, bool filter_dublicates);



 /**@brief Stops an ongoing scan operation
  *
  * @return @ref AT_BLE_SUCCESS Scan operation stopped successfully .
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_scan_stop(void);


 /**@brief Connect to a peer device
  *
  * Connecting to a peer device, implicitly starting the necessary scan operation then
  * connecting if a device in the peers list is found. This API returns immediately after
  * the operation is programmed, actual connection establishment or failure is marked with
  * @ref AT_BLE_CONNECTED event
  *
  * @param[in] peers a list of peers that the device will connect to one of them
  * @param[in] peer_count number of elemnts in peers, 0 to connect to any peer device
  * @param[in] scan_interval  scan interval in 625us units
  * @param[in] scan_window   scan window in 625us units
  * @param[in] connection_params parameters of teh established connection
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully
  * @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_connect(at_ble_addr_t peers[], uint8_t peer_count, 
	uint16_t scan_interval, uint16_t scan_window, at_ble_connection_params_t* connection_params);

 /**@brief Cancels an ongoing connection attempt
  *
  * @return @ref AT_BLE_SUCCESS operation cancalled successfully .
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_connect_cancel(void);

 /**@brief Disconnect a connected peer device
  *
  *
  * @param[in] handle handle of the connection to be terminated
  * @param[in] reason disconnection reason, more info at @ref at_ble_disconnect_reason_t
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully
  * @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
  * @return @ref AT_BLE_FAILURE Generic error.
  */ 
at_ble_status_t at_ble_disconnect(at_ble_handle_t handle, at_ble_disconnect_reason_t reason);


 /**@brief Update the connection parameters of an ongoing connection
  *
  * This API returns after programming the new values but before they take effect,
  * actual effect of the parameters is marked by the event @ref AT_BLE_CONN_PARAM_UPDATE_DONE
  *
  * @param[in] handle handle of the connection to be updated
  * @param[in] connection_params new parameters to be used
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully
  * @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
  * @return @ref AT_BLE_FAILURE Generic error.
  */ 
at_ble_status_t at_ble_connection_param_update(at_ble_handle_t handle, 
	at_ble_connection_params_t* connection_params);

/**@brief Reply to connection parameters update request @ref AT_BLE_CONN_PARAM_UPDATE_REQUEST
  *
  * @param[in] handle handle of the connection to be updated
  * @param[in] connection_params new parameters to be used
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully
  * @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
  * @return @ref AT_BLE_FAILURE Generic error.
  */ 
void at_ble_conn_update_reply(at_ble_handle_t conn_handle , 
				at_ble_connection_params_t* connection_params);

 /**@brief Starts Pairing procedure for a given connection
  *
  * This API will initiate the authentication procedure, given local device requirements and
  * IO capabilities the authentication will proceed in a number of ways, progress is monitored and
  * controlled via the events : @ref AT_BLE_PAIR_KEY_REQUEST ,@ref AT_BLE_PAIR_DONE 
  *
  * In the central role, this function will send an SMP Pairing Request, 
  * otherwise in the peripheral role, this function will reply to 
  * @ref AT_BLE_PAIR_REQUEST.
  *
  * @param[in] conn_handle handle of the ongoing connection
  * @param[in] features local device requirements and IO capabilities
  * @param[in] ltk Local device Long Term Key, null if not available
  * @param[in] csrk Local device Connection Signature Resolution Key , null if not available
  * @param[in] irk Local device Identity Resolution Key, null if not available
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */ 
at_ble_status_t at_ble_authenticate(at_ble_handle_t conn_handle, at_ble_pair_features_t* features,
	at_ble_LTK_t* ltk, at_ble_CSRK_t* csrk, at_ble_IRK_t* irk);

/**@brief Send slave security request to master
  *
  * @param[in] conn_handle handle of the ongoing connection
  * @param[in] mitm_protection
  * @param[in] bond
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */ 
at_ble_status_t at_ble_send_slave_sec_request(at_ble_handle_t conn_handle,bool mitm_protection,bool bond);

 /**@brief Provides a passkey or OOB data that was requested via @ref AT_BLE_PAIR_KEY_REQUEST event
  * If key type is @ref AT_BLE_PAIR_PASSKEY, then a 6-byte ASCII string (digit 0..9 only).
  * If key type is @ref AT_BLE_PAIR_OOB , then a 16-byte OOB key value in Little Endian format
  *
  * @param[in] conn_handle handle of the ongoing connection
  * @param[in] type type of requested key, must match the one asked for in @ref AT_BLE_PAIR_KEY_REQUEST
  * @param[in] key the key requested
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */ 
at_ble_status_t at_ble_pair_key_reply(at_ble_handle_t conn_handle, at_ble_pair_key_type_t type, uint8_t* key);

 /**@brief Cancels an ongoing pairing attempt
  *
  * @param[in] conn_handle handle of the connection to be updated
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully .
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_pair_cancel(at_ble_handle_t conn_handle);

 /**@brief Starts encryption, once encryption starts @ref AT_BLE_ENCRYPTION_STATUS_CHANGED event is delivered
  *
  * @param[in] conn_handle handle of the connection to be updated
  * @param[in] key LTK key used for encryption
  * @param[in] auth authentication level , this information must be stored in device database
  * after each pairing process @ref at_ble_pair_done_t
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully .
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_encryption_start(at_ble_handle_t conn_handle, at_ble_LTK_t key ,uint8_t auth);

 /**@brief Stops encryption, once encryption stops @ref AT_BLE_ENCRYPTION_STATUS_CHANGED event is delivered
  *
  * @param[in] conn_handle handle of the connection to be updated
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully .
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_encryption_stop(at_ble_handle_t conn_handle);

 /**@brief Responds to encryption start request from master device, once encryption starts @ref AT_BLE_ENCRYPTION_STATUS_CHANGED event is delivered
  *
  * @param[in] conn_handle handle of the connection to be updated
  * @param[in] key_found If true then a valid key is found in device database and will be used
  * @param[in] auth authentication level , this information must be stored in device database
  * after each pairing process @ref at_ble_pair_done_t
  * @param[in] key LTK key used for encryption
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully.
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_encryption_request_reply(at_ble_handle_t conn_handle,uint8_t auth, bool key_found, at_ble_LTK_t key);


 /**@brief Sets TX power of a given connection
  *
  * @param[in] conn_handle handle of the connection 
  * @param[in] power  TX power value in dbm
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully .
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_tx_power_set(at_ble_handle_t conn_handle, int8_t power);

 /**@brief Gets TX power of a given connection
  *
  * @param[in] conn_handle handle of the connection 
  *
  * @return TX power in dbm or -1 if failed.
  */
uint16_t at_ble_tx_power_get(at_ble_handle_t conn_handle);

 /**@brief Gets RX power of a given connection
  *
  * @param[in] conn_handle handle of the connection 
  *
  * @return RX power in dbm or -1 if failed.
  */
uint16_t at_ble_rx_power_get(at_ble_handle_t conn_handle);

 /**@brief Subscribes for RX power level notifications
  *
  * if RX power falls between range_min and range_max the event @ref AT_BLE_RX_POWER_VALUE will be recived
  * @param[in] conn_handle handle of the connection 
  * @param[in] range_max notifications will be received if RX power falls under this value but over range_min 
  * @param[in] range_min noticications will be received if RX power falls over this value but under range_max 
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully .
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_rx_power_subscribe(at_ble_handle_t conn_handle, uint16_t range_max, uint16_t range_min);


 /**@brief Unsubscribe form RX power level notifications
  *
  * no more @ref AT_BLE_RX_POWER_VALUE events will be received
  *
  * @param[in] conn_handle handle of the connection 
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully .
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_rx_power_unsubscribe(at_ble_handle_t conn_handle);

/** @}*/

/**
@defgroup gatt-client GATT client API

@{
*/

 /**@brief Discover all Primary services declared in a connected device
  *
  * search will go from start_handle to end_handle, whenever a service is found
  * @ref AT_BLE_PRIMARY_SERVICE_FOUND event is sent
  *
  * @param[in] conn_handle  handle of the connection 
  * @param[in] start_handle start of the searched range 
  * @param[in] end_handle   end of the searched range
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_primary_service_discover_all(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle);

 /**@brief Discover all Primary services of a given UUID declared in a connected device
  *
  * search will go from start_handle to end_handle, whenever a service of matching UUID 
  * is found @ref AT_BLE_PRIMARY_SERVICE_FOUND event is sent
  *
  * @param[in] conn_handle  handle of the connection 
  * @param[in] start_handle start of the searched range 
  * @param[in] end_handle   end of the searched range
  * @param[in] uuid         UUID of the service to be found
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_primary_service_discover_by_uuid(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t* uuid);

 /**@brief Discover all included services declared in a connected device
  *
  * search will go from start_handle to end_handle, whenever a service is found
  * @ref AT_BLE_INCLUDED_SERVICE_FOUND event is sent
  *
  * @param[in] conn_handle  handle of the connection 
  * @param[in] start_handle start of the searched range 
  * @param[in] end_handle   end of the searched range
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_included_service_discover_all(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle);

 /**@brief Discover all Characteristics declared in a connected device
  *
  * search will go from start_handle to end_handle, whenever a characteristic is found
  * @ref AT_BLE_CHARACTERISTIC_FOUND event is sent
  *
  * @param[in] conn_handle  handle of the connection 
  * @param[in] start_handle start of the searched range 
  * @param[in] end_handle   end of the searched range
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_discover_all(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle);

 /**@brief Discover all Characteristics of a given UUID declared in a connected device
  *
  * search will go from start_handle to end_handle, whenever a characteristic of matching UUID 
  * is found @ref AT_BLE_CHARACTERISTIC_FOUND event is sent
  *
  * @param[in] conn_handle  handle of the connetion 
  * @param[in] start_handle start of the searched range 
  * @param[in] end_handle   end of the searched range
  * @param[in] uuid         UUID of the characterisitc to be found
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_discover_by_uuid(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t* uuid);

 /**@brief Discover all Descriptors declared in a connected device
  *
  * search will go from start_handle to end_handle, whenever a descriptor is found
  * @ref AT_BLE_DESCRIPTOR_FOUND event is sent
  *
  * @param[in] conn_handle  handle of the connection 
  * @param[in] start_handle start of the searched range 
  * @param[in] end_handle   end of the searched range
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_descriptor_discover_all(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle);

 /**@brief Read a characteristic given its UUID
  *
  * search will go from start_handle to end_handle, whenever a descriptor with given UUID
  * is found its value will be read and reported via @ref AT_BLE_CHARACTERISTIC_READ_BY_UUID_RESPONSE event
  *
  * @param[in] conn_handle  handle of the connection 
  * @param[in] start_handle start of the searched range 
  * @param[in] end_handle   end of the searched range
  * @param[in] uuid UUID of the read characteristic
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_read_by_uuid(at_ble_handle_t conn_handle, 
    at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t* uuid);

 /**@brief Read a characteristic given its handle
  *
  * read value will be reported via @ref AT_BLE_CHARACTERISTIC_READ_RESPONSE event
  *
  * @param[in] conn_handle handle of the connection 
  * @param[in] char_handle handle of the characteristic
  * @param[in] offset offset at where to start reading
  * @param[in] length maximum length of data to read
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_read(at_ble_handle_t conn_handle, at_ble_handle_t char_handle, uint16_t offset, uint16_t length);

/**@brief Read a list of characteristics given thier handles
  *
  * read values will be reported via @ref AT_BLE_CHARACTERISTIC_READ_MULTIBLE_RESPONSE event
  *
  * @param[in] conn_handle handle of the connection 
  * @param[in] char_handle_list list of characteristic handles
  * @param[in] char_handle_count number of characteristic handles
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_read_multible(at_ble_handle_t conn_handle,  at_ble_handle_t* char_handle_list, uint16_t* char_val_lenght, uint8_t char_handle_count);


 /**@brief write a characteristic
  *
  * if with_response is True, write completion will be reported via @ref AT_BLE_CHARACTERISTIC_WRITE_RESPONSE event
  *
  * @param[in] conn_handle handle of the connection 
  * @param[in] char_handle handle of the characteristic
  * @param[in] offset offset at where to start write
  * @param[in] length length of data to write
  * @param[in] data a buffer where write data is stored
  * @param[in] signed_write if true, perform a signed write
  * @param[in] with_response if true, perform a write with response procedure, otherwise perform w write with no response procedure
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_write(at_ble_handle_t conn_handle, at_ble_handle_t char_handle, uint16_t offset, uint16_t length, uint8_t* data,
	bool signed_write, bool with_response );
 

 /**@brief add a new prepared write request
  *
  * Adds a new request to a pending prepared write operation, or starts a new prepared write
  * operation if none was started before
  *
  * @param[in] conn_handle handle of the connection 
  * @param[in] char_handle handle of the characteristic
  * @param[in] offset offset at where to start write
  * @param[in] length length of data to write
  * @param[in] data a buffer where write data is stored
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_reliable_write_prepare(at_ble_handle_t conn_handle, at_ble_handle_t char_handle, uint16_t offset, uint16_t length, uint8_t* data);


 /**@brief Executes a pending prepared write operation
  *
  * Executes a pending prepared write operation, write completion will be reported 
  * via @ref AT_BLE_CHARACTERISTIC_WRITE_RESPONSE event.
  *
  * @param[in] conn_handle handle of the connection 
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_reliable_write_execute(at_ble_handle_t conn_handle);



 /**@brief Cancels a pending prepared write operation
  *
  * Cancels a pending prepared write operation
  *
  * @param[in] conn_handle handle of the connection 
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_reliable_write_cancel(at_ble_handle_t conn_handle);

 /**@brief Confirms that a notification is received
  *
  * @param[in] conn_handle handle of the connection 
  * @param[in] char_handle handle of the characteristic that originated the notifiction 
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_indication_confirm(at_ble_handle_t conn_handle, at_ble_handle_t char_handle);

/** @}*/

/**
@defgroup gatt-server GATT server API

@{
*/

 /**@brief Defines a new Primary service along with its included services and characteristics
  *
  * @param[in] uuid The primary service UUID
  * @param[out] service_handle the Service handle will be returned here 
  * @param[in] included_service_list an array of included service references, if NULL then included services will be defined later
  * @param[in] included_service_count number of elements in included_service_list
  * @param[in, out] charactristic_list an array of Characterisitcs included in the service, this array is update with respective characartistics handles. If NULL then characteristics will be defined later
  * @param[in] charactristic_count number of elements in charactristic_list
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_primary_service_define(at_ble_uuid_t* uuid, at_ble_handle_t* service_handle,
	at_ble_included_service_t* included_service_list, uint16_t included_service_count,
	at_ble_characteristic_t* charactristic_list, uint16_t charactristic_count);

 /**@brief Defines a new included service inside a primary service definition
  *
  * @param[in] service_handle The primary service handle that will have this definision added to, if 0 then the last defined service will be used
  * @param[in] included_service The included service entry definition
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_included_service_define(at_ble_handle_t service_handle, at_ble_included_service_t* included_service);

 /**@brief Defines a new characteristic inside a primary service definition
  *
  * @param[in] service_handle The primary service handle that will have this definition added to, if 0 then the last defined service will be used
  * @param[in, out] characteristic The Characteristic definition, this array is update with respective characteristic handles.
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_define(at_ble_handle_t service_handle, at_ble_characteristic_t* characteristic );

 /**@brief Defines an arbitrary attribute
  *
  * @param[out] attr_handle the attribute handle is returned here
  * @param[in] uuid Attribute UUID
  * @param[in] init_value Initial value of the attribute
  * @param[in] init_value_len Length of initial value, in bytes
  * @param[in] value_max_len Maximum possible length that this attribute will hold
  * @param[in] permissions Attribute permissions
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_attribute_define(at_ble_handle_t* attr_handle, 
   at_ble_uuid_t uuid, uint8_t* init_value, uint16_t init_value_len, uint16_t value_max_len, 
   at_ble_attr_permissions_t permissions);


 /**@brief Sets A characteristic value
  *
  * @param[in] handle Characteristic value handle
  * @param[in] value new value
  * @param[in] offset where the writing starts
  * @param[in] len Value length, in bytes
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_value_set(at_ble_handle_t handle, uint8_t* value, uint16_t offset, uint16_t len);

 /**@brief Reads A characteristic value
  *
  * @param[in] handle Characteristic value handle
  * @param[out] value read value will be returned here
  * @param[in] offset where the reading starts
  * @param[in] len desired read length, in bytes
  * @param[in] actual_read_len actual read length, in bytes
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_characteristic_value_get(at_ble_handle_t handle, uint8_t* value, uint16_t offset, uint16_t len, uint16_t actual_read_len);

 /**@brief Sends a Notification
  *
  * @param[in] conn_handle handle of the connection to be notified
  * @param[in] attr_handle handle of the attribute orginating the notification
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_notification_send(at_ble_handle_t conn_handle, at_ble_handle_t attr_handle);

 /**@brief Sends an Indication
  *
  * @param[in] conn_handle handle of the connection to be notified
  * @param[in] attr_handle handle of the attribute orginating the indication
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_indication_send(at_ble_handle_t conn_handle, at_ble_handle_t attr_handle);

 /**@brief Sends a Service changed Notification
  *
  * @param[in] conn_handle handle of the connection to which the notification will be sent
  * @param[in] start_handle start of affected handle range
  * @param[in] end_handle end of affected handle range
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_service_changed_notification_send(at_ble_handle_t conn_handle, at_ble_handle_t start_handle, at_ble_handle_t end_handle);

 /**@brief Replies to a read authorization request requested by by @ref AT_BLE_READ_AUTHORIZE_REQUEST event
  *
  * @param[in] conn_handle handle of the connection
  * @param[in] attr_handle handle of the attribute to read
  * @param[in] grant_authorization if True, Authorization is granted
  * @param[in] update_value if True, the value, offset, len params will be used to update this attribute value
  * @param[in] value new value
  * @param[in] offset where the writing starts
  * @param[in] len Value length, in bytes
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_read_authorize_reply(at_ble_handle_t conn_handle, 
   at_ble_handle_t attr_handle, bool grant_authorization,
   bool update_value ,uint8_t* value, uint16_t offset, uint16_t len);

 /**@brief Replies to a write authorization request requested by by @ref AT_BLE_WRITE_AUTHORIZE_REQUEST event
  *
  * @param[in] conn_handle handle of the connection
  * @param[in] attr_handle handle of the attribute to write
  * @param[in] grant_authorization if True, Authorization is granted
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_write_authorize_reply(at_ble_handle_t conn_handle, 
   at_ble_handle_t attr_handle, bool grant_authorization);

/** @}*/

/**
@defgroup l2cap L2CAP API

@{
*/

 /**@brief Registers a custom CID to received data from, when the L2CAP received data on the registered CID an @ref AT_BLE_L2CAP_RX event will be reported
  *
  * @param[in] conn_handle handle of the connection
  * @param[in] cid L2CAP CID to register
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_l2cap_cid_register(at_ble_handle_t conn_handle, uint16_t cid);

 /**@brief Unregisters a previously registered custom CID
  *
  * @param[in] conn_handle handle of the connection
  * @param[in] cid L2CAP CID to unregister
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_l2cap_cid_unregister(at_ble_handle_t conn_handle, uint16_t cid);

 /**@brief Sends a custom L2CAP message using the given CID
  *
  * @param[in] conn_handle handle of the connection
  * @param[in] cid L2CAP CID to use when sending the data
  * @param[in] data data to send
  * @param[in] len length of data, in bytes
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_l2cap_tx(at_ble_handle_t conn_handle, uint16_t cid, uint8_t* data, uint16_t len);

/** @}*/


/**
@defgroup htpt HTPT API

@{
*/

 /**@brief This API message shall be used to add an instance of the Health Thermometer service into the database.
  * This should be done during the initialization phase of the device , after the function is called you should wait for 
  * @ref AT_BLE_HTPT_CREATE_DB_CFM event
  *
  * @param[in] features Indicate if optional features are supported or not, see @ref at_ble_htpt_db_config_flag
  * @param[in] temp_type type of temprature as defined in org.bluetooth.characteristic.temperature_type
  * @param[in] valid_range_min Minimal measurement interval value
  * @param[in] valid_range_max Maximal measurement interval value
  * @param[in] meas_intv Latest known value for measurement interval  ()
  * @param[in] sec_lvl can be : Disable(0) , Enable(1), Unauth(2), Auth(3) see @ref at_ble_svc_sec_level
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_htpt_create_db(at_ble_htpt_db_config_flag features,
									  at_ble_htpt_temp_type  temp_type,
									  uint16_t valid_range_min,
									  uint16_t valid_range_max,
									  uint16_t meas_intv,
									  at_ble_htpt_sec_level sec_lvl
									  );


 /**@brief This API message is used for enabling the Thermometer role of the Health Thermometer profile.
  * This should be called after the database creation
  *
  * @param[in] conn_handle handle of the connection
  * @param[in] ntf_ind_cfg Enable notifications or indications for profile characteristics see @ref at_ble_htpt_ntf_ind_cfg
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_htpt_enable(     at_ble_handle_t conn_handle,
										at_ble_htpt_ntf_ind_cfg ntf_ind_cfg
										);


 /**@brief This message is used by the application (which handles the temperature device driver and
  * measurements) to send a temperature measurement through the Thermometer role.
  *
  * @param[in] temp temperature value
  * @param[in] time_stamp The time of reading according to the format defined in struct prf_date_time
  * @param[in] flags temp flag see @ref at_ble_htpt_temp_flags
  * @param[in] type temp type see @ref at_ble_htpt_temp_type
  * @param[in] flag_stable_meas Stable or intermediary type of temperature  (True stable meas, else false)
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_htpt_temp_send(	uint32_t temp,
										struct prf_date_time* time_stamp,
										at_ble_htpt_temp_flags  flags,
										at_ble_htpt_temp_type  type,
										bool flag_stable_meas);


 /**@brief This message is used by the application to order the HTPT profile to generate an indication (if enabled)
  * of the Measurement Interval Char. This can be done as the application desires, at each connection, or if the
  * measurement interval value has been modified locally (interface for this is not provided since a normal thermometer
  * would have very few configurable UI elements and configuration should be done through Collector)
  *
  * @param[in] meas_intv Measurement Interval value
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t at_ble_htpt_meas_intv_update(uint16_t meas_intv);


/** @}*/

/* utility functions, might be removed lated */
static uint8_t at_ble_uuid_type2len(at_ble_uuid_type_t type)
{
	switch(type)
	{
	case AT_BLE_UUID_16 : 
		return AT_BLE_UUID_16_LEN;
		
	case AT_BLE_UUID_32 : 
		return AT_BLE_UUID_32_LEN;
		
	default:
	case AT_BLE_UUID_128 : 
		return AT_BLE_UUID_128_LEN;
	}
}

static at_ble_uuid_type_t at_ble_uuid_len2type(uint8_t len)
{
	switch(len)
	{
	case AT_BLE_UUID_16_LEN: 
		return AT_BLE_UUID_16;
		
	case AT_BLE_UUID_32_LEN : 
		return AT_BLE_UUID_32;
		
	default:
	case AT_BLE_UUID_128_LEN : 
		return AT_BLE_UUID_128;
	}
}

#endif //__AT_BLE_API_H__
