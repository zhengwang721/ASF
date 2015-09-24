/*
 * gap_task.c
 *
 */

#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"
#include "dbg_logs.h"
#include "gapm_task.h"
#include "dbg_task.h"
#include "gapc_task.h"

#include "device.h"

// original location : llm.h
enum
{
    LLM_ADV_CONN_UNDIR = 0x00,
    LLM_ADV_CONN_DIR,
    LLM_ADV_NONCONN_UNDIR,
    LLM_SCAN_REQ,
    LLM_SCAN_RSP,
    LLM_CONNECT_REQ,
    LLM_ADV_DISC_UNDIR,
    LLM_ADV_END
};

/*TODO: Used internal to detect Selective connection mode*/
//bool bSelectiveConnModeRunning = false;

/*
 * gapm_get_address_type
 * The following function will parse addrerss type to at_ble_addr_type_t values according to
 * last byte of address and recieved type from FW.
 * Where received values from FW are PUBLIC or RANDOM only.
 */
static uint8_t gapm_get_address_type(uint8_t *pu8Addr, uint8_t u8AddrType)
{
    uint8_t u8RetValue = AT_BLE_ADDRESS_PUBLIC;
    if (0x00 != (u8AddrType & 0x000000FF))
    {
        if (0xC0 == (0xC0 & pu8Addr[AT_BLE_ADDR_LEN - 1]))
        {
            u8RetValue = AT_BLE_ADDRESS_RANDOM_STATIC;
        }
        else if (0x40 == (0x40 & pu8Addr[AT_BLE_ADDR_LEN - 1]))
        {
            u8RetValue = AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE;
        }
        else
        {
            u8RetValue = AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE;
        }
    }
    return u8RetValue;
}

/*
* address on air is sent either 0:public or 1:random
*/
#define gapm_set_address_type(type) ((type>0 && type <4)?1:0)

uint8_t gapm_reset_req_handler(void)
{
    uint8_t u8Operation, u8Status;
    INTERFACE_MSG_INIT(GAPM_RESET_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(GAPM_RESET);
    INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if (u8Operation != GAPM_RESET)
    {
        PRINT_ERR("W.OP: %02x, R.OP:%02x\r\n", GAPM_RESET, u8Operation);
        return AT_BLE_FAILURE;
    }
    return u8Status;
}

uint8_t gapm_set_dev_config_cmd_handler(uint8_t u8Role, uint16_t u16RenewDur,
                                        uint8_t *pu8Address, uint8_t *pu8Irkey, uint8_t u8AddrType, uint8_t u8AttCfg,
                                        uint16_t u16GapHandle, uint16_t u16GattHandle, uint16_t u16MaxMTU)
{
    uint8_t u8Operation, u8Status;
    INTERFACE_MSG_INIT(GAPM_SET_DEV_CONFIG_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(GAPM_SET_DEV_CONFIG);
    INTERFACE_PACK_ARG_UINT8(u8Role);
    INTERFACE_PACK_ARG_UINT16(u16RenewDur);
    INTERFACE_PACK_ARG_BLOCK(pu8Address, AT_BLE_ADDR_LEN);
    INTERFACE_PACK_ARG_BLOCK(pu8Irkey, AT_BLE_MAX_KEY_LEN);
    INTERFACE_PACK_ARG_UINT8(u8AddrType);
    INTERFACE_PACK_ARG_UINT8(u8AttCfg);
    INTERFACE_PACK_ARG_UINT16(u16GapHandle);
    INTERFACE_PACK_ARG_UINT16(u16GattHandle);
    INTERFACE_PACK_ARG_UINT16(u16MaxMTU);
    INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if (u8Operation != GAPM_SET_DEV_CONFIG)
    {
        PRINT_ERR("W.OP: %02x, R.OP:%02x\r\n", GAPM_SET_DEV_CONFIG, u8Operation);
        return AT_BLE_FAILURE;
    }
    return u8Status;
}

uint8_t gapm_get_dev_config_cmd_handler(at_ble_get_dev_info_op_t op, void *arg)
{
    at_ble_status_t u8Status = AT_BLE_INVALID_PARAM;
    uint8_t u8Operation;
    do
    {
        if ((op > AT_BLE_DEVICE_ADDRESS) || (NULL == arg))
        {
            break;
        }
        INTERFACE_MSG_INIT(GAPM_GET_DEV_INFO_CMD, TASK_GAPM);
        switch (op)
        {
        case AT_BLE_DEVICE_ADDRESS:
        {
            INTERFACE_PACK_ARG_UINT8(GAPM_GET_DEV_BDADDR);
            INTERFACE_SEND_WAIT_ANY(GAPM_DEV_BDADDR_IND, GAPM_CMP_EVT, TASK_GAPM);
            if (!INTERFACE_EVENT_FAILURE())
            {
                INTERFACE_UNPACK_BLOCK(&((at_ble_addr_t *)arg)->addr[0], AT_BLE_ADDR_LEN);
                INTERFACE_UNPACK_UINT8(&((at_ble_addr_t *)arg)->type);
                INTERFACE_WAIT_FOR(GAPM_CMP_EVT, TASK_GAPM);
            }
            INTERFACE_UNPACK_UINT8(&u8Operation);
            INTERFACE_UNPACK_UINT8(&u8Status);
            if (u8Operation != GAPM_GET_DEV_BDADDR)
            {
                PRINT_ERR("W.OP: %02x, R.OP:%02x\r\n", GAPM_GET_DEV_BDADDR, u8Operation);
                u8Status = AT_BLE_FAILURE;
                break;
            }
        }
        break;
        default:
            u8Status = AT_BLE_INVALID_PARAM;
            break;
        }
        INTERFACE_DONE();
    }
    while (0);
    return u8Status;
}

at_ble_status_t gapm_cancel_cmd_handler(void)
{
    uint8_t u8Operation, u8Status = 0;
    INTERFACE_MSG_INIT(GAPM_CANCEL_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(GAPM_CANCEL);
    INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if (u8Status == GAP_ERR_CANCELED)
    {
        return AT_BLE_SUCCESS;
    }
    else
    {
        return (at_ble_status_t)u8Status;
    }
}

uint8_t gapm_set_channel_map_cmd_handler(uint8_t *pu8Map)
{
    uint8_t u8Operation, u8Status;
    INTERFACE_MSG_INIT(GAPM_SET_CHANNEL_MAP_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(GAPM_SET_CHANNEL_MAP);
    INTERFACE_PACK_ARG_BLOCK(pu8Map, AT_BLE_CH_MAP_LEN);
    INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if (u8Operation != GAPM_SET_CHANNEL_MAP)
    {
        PRINT_ERR("W.OP: %02x, R.OP:%02x\r\n", GAPM_SET_CHANNEL_MAP, u8Operation);
        return AT_BLE_FAILURE;
    }
    return u8Status;
}

uint8_t gapm_dev_bdaddr_ind_handler(uint8_t *data, at_ble_addr_t *param)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_BLOCK(&param->addr[0], AT_BLE_ADDR_LEN);
    INTERFACE_UNPACK_UINT8(&param->type);
    INTERFACE_DONE();
    return AT_BLE_SUCCESS;
}

at_ble_events_t gapm_cmp_evt(uint8_t *data, void *params)
{
    uint8_t u8Operation , u8status;
    at_ble_events_t evt_num = AT_BLE_UNDEFINED_EVENT;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8status);
    INTERFACE_DONE();
    switch (u8Operation)
    {
    case GAPM_SCAN_PASSIVE:
    case GAPM_SCAN_ACTIVE:
    {
        at_ble_scan_report_t *scan_report = (at_ble_scan_report_t *) params;
        evt_num = AT_BLE_SCAN_REPORT;
        scan_report->status = (at_ble_status_t)((u8status == GAP_ERR_TIMEOUT) ? AT_BLE_SUCCESS : (u8status));
    }
    break;
    case GAPM_RESOLV_ADDR:
        if (u8status != GAP_ERR_NO_ERROR)
        {
            at_ble_resolv_rand_addr_status_t *resolv_status =
                (at_ble_resolv_rand_addr_status_t *)params;
            evt_num = AT_BLE_RESOLV_RAND_ADDR_STATUS;
            resolv_status->status = (at_ble_status_t)u8status;
            memset(resolv_status->irk, 0, KEY_LEN);
        }
        else
        {
            evt_num = AT_BLE_EVENT_MAX; //Ignore it incase of success whereas GAPM_ADDR_SOLVED_IND was received.
        }
        break;
    // TODO add handle advertise error comple event
    case GAPM_ADV_UNDIRECT:
    case GAPM_ADV_DIRECT:
    case GAPM_ADV_DIRECT_LDC:
    {
        if (AT_BLE_SUCCESS == u8status)
        {
            uint8_t index = check_ConnData_emptyEntry();
            if ((1 <= index) && (AT_BLE_MAX_CONCURRENT_CONNS > index))
            {
                at_ble_connected_t *connected = (at_ble_connected_t *)params;
                uint8_t role;
                index = check_ConnData_idx_role(device.conn_handle, &role);
                connected->handle = gstrConnData[index].conHandle;
                connected->peer_addr.type = gstrConnData[index].peerAddr.type;
                connected->conn_status = (at_ble_status_t)u8status;
                memcpy(connected->peer_addr.addr, gstrConnData[index].peerAddr.addr, AT_BLE_ADDR_LEN);
                connected->peer_addr.type = (at_ble_addr_type_t)gapm_get_address_type((uint8_t *)(&(connected->peer_addr.addr)), connected->peer_addr.type);
                device.conn_handle = 0xFFFF;
                evt_num = AT_BLE_CONNECTED;
                break;
            }
        }
        else
        {
            at_ble_adv_report_t *adv_report = (at_ble_adv_report_t *) params;
            adv_report->status = (at_ble_status_t)u8status;
            evt_num = AT_BLE_ADV_REPORT;
        }
    }
    break;
    case GAPM_ADV_NON_CONN:
    {
        at_ble_adv_report_t *adv_report = (at_ble_adv_report_t *) params;
        adv_report->status = (at_ble_status_t)((u8status == GAP_ERR_TIMEOUT) ? (AT_BLE_SUCCESS) : (u8status));
        evt_num = AT_BLE_ADV_REPORT;
    }
    break;
    case GAPM_CONNECTION_DIRECT:
    case GAPM_CONNECTION_AUTO:
    {
        at_ble_connected_t *connected = (at_ble_connected_t *)params;
        uint8_t index = 0;
        index = check_ConnData_idx_role(device.conn_handle, NULL);
        if (AT_BLE_MAX_CONCURRENT_CONNS > index)
        {
            device.conn_handle = 0xFFFF;
            connected->handle = gstrConnData[index].conHandle;
            connected->peer_addr.type = gstrConnData[index].peerAddr.type;
            connected->conn_status = (at_ble_status_t)u8status;
            memcpy(connected->peer_addr.addr, gstrConnData[index].peerAddr.addr, AT_BLE_ADDR_LEN);
        }
        connected->peer_addr.type = (at_ble_addr_type_t)gapm_get_address_type((uint8_t *)(&(connected->peer_addr.addr)), connected->peer_addr.type);
        evt_num = AT_BLE_CONNECTED;
    }
    break;
    default:
    {
        PRINT_ERR("Unhandled OP 0x<%02X>\r\n", u8Operation);
    }
    break;
    }
    if (AT_BLE_UNDEFINED_EVENT == evt_num)
    {
        PRINT_DBG("GAMP_CMP_EVT:OP 0x%02x Status 0x%02x\r\n", u8Operation, u8status);
    }
    return evt_num;
}

uint8_t gapm_start_adv_cmd_handler(uint8_t u8OpCode, uint8_t u8AddrSrc, uint16_t u16RenewDur,
                                   uint8_t peer_addr_type, uint8_t *pu8BdAddr, uint16_t u16MinIntv, uint16_t u16MaxIntv, uint8_t u8ChnlMap, uint8_t u8Mode ,
                                   uint8_t u8AdvFiltPolicy, uint8_t u8AdvDataLen, uint8_t *pu8AdvData, uint8_t u8ScnRespLen, uint8_t *pu8ScnRespData ,
                                   uint16_t u16Timeout, bool disable_randomness)
{
    INTERFACE_MSG_INIT(GAPM_START_ADVERTISE_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(u8OpCode);
    INTERFACE_PACK_ARG_UINT8(u8AddrSrc);
    // Dummy data use to retrieve internal operation state (should be set to 0).
    INTERFACE_PACK_ARG_UINT16(0);
    INTERFACE_PACK_ARG_UINT16(u16MinIntv);
    INTERFACE_PACK_ARG_UINT16(u16MaxIntv);
    INTERFACE_PACK_ARG_UINT8(u8ChnlMap);
    if ((u8OpCode == GAPM_ADV_DIRECT) ||
            (GAPM_ADV_DIRECT_LDC == u8OpCode)
       )
    {
        uint8_t paddingLen = sizeof(u8Mode) +
                             sizeof(u8AdvFiltPolicy) +
                             sizeof(u8AdvDataLen) +
                             ADV_DATA_MAX_SIZE +
                             sizeof(u8ScnRespLen) +
                             SCAN_RESP_DATA_MAX_SIZE;
        if (pu8BdAddr != NULL)
        {
            INTERFACE_PACK_ARG_BLOCK(pu8BdAddr, AT_BLE_ADDR_LEN);
        }
        else
        {
            INTERFACE_PACK_ARG_DUMMY(AT_BLE_ADDR_LEN);
        }
        INTERFACE_PACK_ARG_UINT8(peer_addr_type);
        INTERFACE_PACK_ARG_DUMMY(paddingLen - (AT_BLE_ADDR_LEN + sizeof(peer_addr_type)));
    }
    else
    {
        INTERFACE_PACK_ARG_UINT8(u8Mode);
        INTERFACE_PACK_ARG_UINT8(u8AdvFiltPolicy);
        // advertising data
        INTERFACE_PACK_ARG_UINT8(u8AdvDataLen);
        INTERFACE_PACK_ARG_BLOCK(pu8AdvData, u8AdvDataLen);
        INTERFACE_PACK_ARG_DUMMY((ADV_DATA_MAX_SIZE - u8AdvDataLen));
        // scan responseat_ble_scan_start
        INTERFACE_PACK_ARG_UINT8(u8ScnRespLen);
        INTERFACE_PACK_ARG_BLOCK(pu8ScnRespData, u8ScnRespLen);
        INTERFACE_PACK_ARG_DUMMY((SCAN_RESP_DATA_MAX_SIZE - u8ScnRespLen));
    }
    INTERFACE_PACK_ARG_UINT16(0xA5A5);
    INTERFACE_PACK_ARG_UINT16(u16Timeout);
    INTERFACE_PACK_ARG_UINT8(disable_randomness);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return AT_BLE_SUCCESS;
}

uint8_t get_gap_local_addr_type(void)
{
    uint8_t gap_addr_type = 0;
    switch (device.config.address.type)
    {
    default:
    case AT_BLE_ADDRESS_PUBLIC:
    case AT_BLE_ADDRESS_RANDOM_STATIC:
        gap_addr_type = GAPM_STATIC_ADDR;
        break;
    case AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE:
        gap_addr_type = GAPM_GEN_RSLV_ADDR;
        break;
    case AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE:
        gap_addr_type = GAPM_GEN_NON_RSLV_ADDR;
        break;
    }
    return gap_addr_type;
}

void gapm_start_scan_cmd_handler(uint8_t u8OpCode, uint8_t u8AddrType, uint16_t u16RenewDur,
                                 uint8_t *pu8BdAddr, uint16_t u16ScanInterval, uint16_t u16ScanWin, uint8_t u8ScanMode, uint8_t u8FiltPolicy,
                                 uint8_t u8FilterDuplic , uint16_t u16Timeout)
{
    INTERFACE_MSG_INIT(GAPM_START_SCAN_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(u8OpCode);
    INTERFACE_PACK_ARG_UINT8(u8AddrType);
    INTERFACE_PACK_ARG_UINT16(0);
    INTERFACE_PACK_ARG_UINT16(u16ScanInterval);
    INTERFACE_PACK_ARG_UINT16(u16ScanWin);
    INTERFACE_PACK_ARG_UINT8(u8ScanMode);
    INTERFACE_PACK_ARG_UINT8(u8FiltPolicy);
    INTERFACE_PACK_ARG_UINT8(u8FilterDuplic);
    //padding
    INTERFACE_PACK_ARG_UINT8(0);
    INTERFACE_PACK_ARG_UINT16(0xA5A5);
    INTERFACE_PACK_ARG_UINT16(u16Timeout);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gapm_adv_report_evt_handler(uint8_t *data,
                                 at_ble_scan_info_t *param)
{
    uint8_t evt_type, u8Rssi;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&evt_type);
    INTERFACE_UNPACK_UINT8(&(param->dev_addr.type));
    INTERFACE_UNPACK_BLOCK(param->dev_addr.addr, AT_BLE_ADDR_LEN);
    INTERFACE_UNPACK_UINT8(&(param->adv_data_len));
    INTERFACE_UNPACK_BLOCK(param->adv_data , param->adv_data_len);
    INTERFACE_UNPACK_SKIP(AT_BLE_ADV_MAX_SIZE - param->adv_data_len);
    INTERFACE_UNPACK_UINT8(&u8Rssi);
    INTERFACE_DONE();
    if (u8Rssi > 127)
    {
        u8Rssi -= 255;
    }
    param->rssi = (int8_t)u8Rssi;
    param->dev_addr.type = (at_ble_addr_type_t)gapm_get_address_type((uint8_t *)(&(param->dev_addr.addr)), param->dev_addr.type);
    switch (evt_type)
    {
    case ADV_CONN_UNDIR:
        param->type = AT_BLE_ADV_TYPE_UNDIRECTED;
        break;
    case ADV_CONN_DIR:
        param->type = AT_BLE_ADV_TYPE_DIRECTED;
        break;
    case ADV_DISC_UNDIR:
        param->type = AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED;
        break;
    case ADV_NONCONN_UNDIR:
        param->type = AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED;
        break;
    case LLM_SCAN_RSP:
        param->type = AT_BLE_ADV_TYPE_SCAN_RESPONSE;
        break;
    }
}


void gapm_start_connection_cmd_handler(uint8_t u8OpCode, uint8_t u8AddrType, uint16_t u16RenewDur,
                                       uint8_t *pu8BdAddr, uint16_t u16ScanInterval, uint16_t u16ScanWin, uint16_t u16ConIntvMin,
                                       uint16_t u16ConIntvMax, uint16_t u16ConLatency, uint16_t u16SupervTO, uint16_t u16CeMin,
                                       uint16_t u16CeMAx, uint8_t u8NbOfPeers, at_ble_addr_t *peers)
{
    uint8_t count = 0;
    INTERFACE_MSG_INIT(GAPM_START_CONNECTION_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(u8OpCode);
    INTERFACE_PACK_ARG_UINT8(u8AddrType);
    INTERFACE_PACK_ARG_UINT16(0);
    INTERFACE_PACK_ARG_UINT16(u16ScanInterval);
    INTERFACE_PACK_ARG_UINT16(u16ScanWin);
    INTERFACE_PACK_ARG_UINT16(u16ConIntvMin);
    INTERFACE_PACK_ARG_UINT16(u16ConIntvMax);
    INTERFACE_PACK_ARG_UINT16(u16ConLatency);
    INTERFACE_PACK_ARG_UINT16(u16SupervTO);
    INTERFACE_PACK_ARG_UINT16(u16CeMin);
    INTERFACE_PACK_ARG_UINT16(u16CeMAx);
    INTERFACE_PACK_ARG_UINT8(u8NbOfPeers);
    for (count = 0; count < u8NbOfPeers; count++)
    {
        INTERFACE_PACK_ARG_BLOCK((peers->addr), BD_ADDR_LEN);
        INTERFACE_PACK_ARG_UINT8(gapm_set_address_type(peers->type));
        peers += 1;
    }
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    if (GAPM_CONNECTION_SELECTIVE == u8OpCode)
    {
        //bSelectiveConnModeRunning = true;
        /*TODO: Selective mode is not handled yet*/
    }
}

uint8_t gapm_white_list_mgm_cmd(uint8_t operation, uint8_t addr_type, uint8_t *param)
{
    uint8_t u8Operation = 0, u8Status;
    PRINT_DBG("input operation : %d\n", operation);
    INTERFACE_MSG_INIT(GAPM_WHITE_LIST_MGT_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(operation);
    switch (operation)
    {
    case GAPM_ADD_DEV_IN_WLIST:
    case GAPM_RMV_DEV_FRM_WLIST:
        INTERFACE_PACK_ARG_UINT8(1);
        INTERFACE_PACK_ARG_BLOCK(param, AT_BLE_ADDR_LEN);
        INTERFACE_PACK_ARG_UINT8(addr_type);
        break;
    default:
        INTERFACE_PACK_ARG_UINT8(0);
        break;
    }
    if (GAPM_GET_WLIST_SIZE == operation)
    {
        INTERFACE_SEND_WAIT_ANY(GAPM_WHITE_LIST_SIZE_IND, GAPM_CMP_EVT, TASK_GAPM);
        if (!INTERFACE_EVENT_FAILURE())
        {
            INTERFACE_UNPACK_UINT8(param);
            INTERFACE_WAIT_FOR(GAPM_CMP_EVT, TASK_GAPM);
        }
    }
    else
    {
        INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
    }
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    PRINT_DBG("output operation : %d\n", u8Operation);
    return u8Status;
}

void gapm_resolv_addr_cmd_handler(uint8_t nb_key , uint8_t *rand_addr , uint8_t *irk)
{
    INTERFACE_MSG_INIT(GAPM_RESOLV_ADDR_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(GAPM_RESOLV_ADDR);
    INTERFACE_PACK_ARG_UINT8(nb_key);
    INTERFACE_PACK_ARG_BLOCK(rand_addr, AT_BLE_ADDR_LEN);
    INTERFACE_PACK_ARG_BLOCK(irk, (KEY_LEN * nb_key));
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gapm_addr_solved_ind_handler(uint8_t *data , at_ble_resolv_rand_addr_status_t *params)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_BLOCK(params->resolved_addr, AT_BLE_ADDR_LEN);
    INTERFACE_UNPACK_BLOCK(params->irk, AT_BLE_MAX_KEY_LEN);
    INTERFACE_DONE();
    params->status = AT_BLE_SUCCESS;
}

at_ble_status_t gapm_profile_task_add_cmd_handler(uint8_t  sec_lvl,
        uint16_t prf_task_id,
        uint16_t app_task,
        uint16_t start_hdl,
        void *param,
        uint16_t len,
        at_ble_handle_t *ret_start_handle
                                                 )
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    uint8_t operation;
    uint16_t ret_prf_task_id, ret_prf_task_nb;
    INTERFACE_MSG_INIT(GAPM_PROFILE_TASK_ADD_CMD, TASK_GAPM);
    INTERFACE_PACK_ARG_UINT8(GAPM_PROFILE_TASK_ADD);
    INTERFACE_PACK_ARG_UINT8(sec_lvl);
    INTERFACE_PACK_ARG_UINT16(prf_task_id);
    INTERFACE_PACK_ARG_UINT16(app_task);
    INTERFACE_PACK_ARG_UINT16(start_hdl);
    INTERFACE_PACK_ARG_BLOCK(param, len);
    INTERFACE_SEND_WAIT_ANY(GAPM_PROFILE_ADDED_IND, GAPM_CMP_EVT, TASK_GAPM);
    if (!INTERFACE_EVENT_FAILURE()) // if succeeded , unpack profile added ind
    {
        INTERFACE_UNPACK_UINT16(&ret_prf_task_id);
        INTERFACE_UNPACK_UINT16(&ret_prf_task_nb);
        INTERFACE_UNPACK_UINT16(ret_start_handle);
        INTERFACE_WAIT_FOR(GAPM_CMP_EVT, TASK_GAPM);// and wait for the success command complete event
    }
    //in case of failure , return failure code to user
    INTERFACE_UNPACK_UINT8(&operation);
    INTERFACE_UNPACK_UINT8(&status);
    INTERFACE_DONE();
    return status;
}

at_ble_events_t gapm_profile_added_ind_handler(uint8_t *data, void *params)
{
    at_ble_events_t evt;
    uint16_t prf_task_id, prf_task_nb, start_hdl;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&prf_task_id);
    INTERFACE_UNPACK_UINT16(&prf_task_nb);
    INTERFACE_UNPACK_UINT16(&start_hdl);
    INTERFACE_DONE();
    switch (prf_task_id)
    {
    case TASK_HTPT :
    {
        at_ble_htpt_create_db_cfm_t *p = (at_ble_htpt_create_db_cfm_t *)params;
        p->status = AT_BLE_SUCCESS;
        p->start_handle = start_hdl;
        evt = AT_BLE_HTPT_CREATE_DB_CFM;
    }
    default :
        evt = AT_BLE_EVENT_MAX;
        break;
    }
    return evt;
}
