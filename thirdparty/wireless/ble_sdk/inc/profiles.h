/*
 * profiles.h
 *
 *  Created on: Mar 12, 2015
 *      Author: aabdelfattah
 */

#ifndef __PROFILES_H__
#define __PROFILES_H__

/// Message structure used to inform APP that an error has occurred in the profile server role task
typedef struct
{
    /// Connection Handle
	uint16_t conhdl;
    /// Message ID
	uint16_t msg_id;
    /// Status
	uint8_t status;
}prf_server_error_ind_t;

/// Time profile information
struct prf_date_time
{
    /// year time element
    uint16_t year;
    /// month time element
    uint8_t month;
    /// day time element
    uint8_t day;
    /// hour time element
    uint8_t hour;
    /// minute time element
    uint8_t min;
    /// second time element
    uint8_t sec;
};
/// Connection type
enum prf_con_type
{
    ///Discovery type connection
    PRF_CON_DISCOVERY = 0x00,
    /// Normal type connection
    PRF_CON_NORMAL    = 0x01
};

#endif /* __PROFILES_H__ */