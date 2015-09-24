#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "gapc_task.h"

struct device_info
{
    /*
     =====================================
     *Configuration Data to be sent to FW*
     =====================================
     */
    at_ble_dev_config_t config;

    /*
     ==================
     *Operations Flags*
     ==================
     */
    bool bAddrWasSet;
    bool bIrkeyWasSet;
    bool bDevNameWasSet;
    bool bGapDevInfo;
    //bool bAddrAutoGen;
    bool bDataChanged;

    /*
     ========================
     *Local Data to be saved*
     ========================
     */
    uint16_t conn_handle;
    // local device adv and scan data
    uint8_t ADVData[AT_BLE_ADV_MAX_SIZE];
    uint8_t SrData[AT_BLE_ADV_MAX_SIZE];
    uint8_t advLen;
    uint8_t srLen;
    at_ble_adv_channel_map_t u8AdvChnlMap;

    //Connection mode operation
    at_ble_conn_mode_t conMode;

    uint8_t privacy_flags;
    // peer device address
    at_ble_addr_t peer_addr;

    // device local keys
    at_ble_LTK_t ltk;
    at_ble_CSRK_t csrk;

    /* Device Info*/
    struct device_name dev_name;
    uint16_t appearance;
    at_ble_spcp_t spcp_param;
    uint8_t dev_name_write_perm;
};

typedef struct
{
    uint8_t             role;
    at_ble_addr_t       peerAddr;
    at_ble_conn_mode_t  conMode;
    at_ble_handle_t     conHandle;
    gap_auth_t          auth;
    at_ble_LTK_t        ltk;
    at_ble_CSRK_t       csrk;
} tstrConnData;

extern uint8_t check_ConnData_idx_role(uint16_t con_idx, uint8_t *pRole);
extern uint8_t check_ConnData_emptyEntry(void);

extern struct device_info device;
extern tstrConnData gstrConnData[AT_BLE_MAX_CONCURRENT_CONNS];

#endif
