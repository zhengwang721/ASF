#ifndef __PROFILES_H__
#define __PROFILES_H__

/// Message structure used to inform APP that an error has occurred in the profile server role task
typedef struct prf_server_error_ind
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


#endif /* __PROFILES_H__ */