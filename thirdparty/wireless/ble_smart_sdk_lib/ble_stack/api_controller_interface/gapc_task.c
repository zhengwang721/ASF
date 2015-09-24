/*
 * gapc_task.c
 *
 */
#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"
#include "dbg_logs.h"
#include "dbg_task.h"
#include "device.h"



typedef struct
{
    at_ble_handle_t handle;
    at_ble_LTK_t ltk;
    at_ble_CSRK_t csrk;
    at_ble_IRK_t irk;
} at_key_info_t;

static at_key_info_t peer_key_info;

/*Used extern with other tasks*/
uint8_t check_ConnData_idx_role(uint16_t con_idx, uint8_t *pRole)
{
    uint8_t counter = AT_BLE_MAX_CONCURRENT_CONNS;
    if (0xFFFF != con_idx)
    {
        for (counter = 0; counter < AT_BLE_MAX_CONCURRENT_CONNS; counter++)
        {
            if (con_idx == gstrConnData[counter].conHandle)
            {
                if (NULL != pRole)
                {
                    *pRole = gstrConnData[counter].role;
                }
                break;
            }
        }
    }
    return counter; //incase of failure this value will be AT_BLE_MAX_CONCURRENT_CONNS
}

/*Used extern with other tasks*/
uint8_t check_ConnData_emptyEntry(void)
{
    uint8_t counter = 0;
    for (counter = 0; counter < AT_BLE_MAX_CONCURRENT_CONNS; counter++)
    {
        if (0xFFFF == gstrConnData[counter].conHandle)
        {
            break;
        }
    }
    return counter; //incase of failure thie value will be AT_BLE_MAX_CONCURRENT_CONNS
}

at_ble_events_t gapc_cmp_evt(uint16_t src, uint8_t *data, void *params)
{
    uint8_t u8Operation, status;
    at_ble_events_t evt_num = AT_BLE_UNDEFINED_EVENT;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&status);
    INTERFACE_DONE();
    switch (u8Operation)
    {
    case GAPC_DISCONNECT:
        if (AT_BLE_SUCCESS != status)
        {
            at_ble_disconnected_t *p = (at_ble_disconnected_t *)params;
            p->handle = KE_IDX_GET(src);
            p->reason = status;
            evt_num = AT_BLE_DISCONNECTED;
        }
        else
        {
            evt_num = AT_BLE_EVENT_MAX;
            PRINT_DBG("GAPC_CMP_EVT:OP GAPC_DISCONNECT consumed\n");
        }
        break;
    case GAPC_BOND:
    {
        uint8_t role = ROLE_SLAVE;
        if (AT_BLE_MAX_CONCURRENT_CONNS > check_ConnData_idx_role(KE_IDX_GET(src), &role) && (role == ROLE_MASTER))
        {
            evt_num = AT_BLE_EVENT_MAX;
            PRINT_DBG("GAPC_CMP_EVT:OP GAPC_BOND consumed\n");
        }
        else
        {
            evt_num = AT_BLE_PAIR_DONE;
            ((at_ble_pair_done_t *)params)->handle = KE_IDX_GET(src);
            ((at_ble_pair_done_t *)params)->status = (at_ble_status_t)status;
            PRINT_INFO("\t>>0x%X\r\n"
                       "\t>>0x%X\r\n", ((at_ble_pair_done_t *)params)->handle, ((at_ble_pair_done_t *)params)->status);
        }
    }
    break;
    case GAPC_SECURITY_REQ:
        // TODO : to be checked after release
        evt_num = AT_BLE_EVENT_MAX;
        PRINT_DBG("GAPC_CMP_EVT:OP GAPC_SECURITY_REQ consumed\n");
        break;
    case GAPC_ENCRYPT:
        if (AT_BLE_SUCCESS != status)
        {
            evt_num = AT_BLE_ENCRYPTION_STATUS_CHANGED;
            ((at_ble_encryption_status_changed_t *)params)->handle = KE_IDX_GET(src);
            ((at_ble_encryption_status_changed_t *)params)->status = (at_ble_status_t)status;
        }
        else
        {
            evt_num = AT_BLE_EVENT_MAX;
            PRINT_DBG("GAPC_CMP_EVT:OP GAPC_ENCRYPT consumed\n");
        }
        break;
    case GAPC_UPDATE_PARAMS:
        if (AT_BLE_SUCCESS != status)
        {
            at_ble_conn_param_update_done_t *conn_param_update_event = (at_ble_conn_param_update_done_t *)params;
            conn_param_update_event->handle = KE_IDX_GET(src);
            conn_param_update_event->status = (at_ble_status_t)status;
            conn_param_update_event->con_intv = 0;
            conn_param_update_event->con_latency = 0;
            conn_param_update_event->superv_to = 0;
            evt_num = AT_BLE_CONN_PARAM_UPDATE_DONE;
        }
        else
        {
            evt_num = AT_BLE_EVENT_MAX;
            PRINT_DBG("GAPC_CMP_EVT:OP GAPC_UPDATE_PARAMS consumed\n");
        }
        break;
    case GAPC_LE_CB_CONNECTION:
    {
        if (status != AT_BLE_SUCCESS)
        {
            at_ble_lecb_connected_t *p = (at_ble_lecb_connected_t *)params;
            p->status = status;
            evt_num = AT_BLE_LECB_DISCONNECTED;
            PRINT_ERR("LECB connection creation failed \n");
        }
        else
        {
            evt_num = AT_BLE_EVENT_MAX;
            PRINT_DBG("GAPC_CMP_EVT:OP GAPC_LE_CB_CREATE consumed\n");
        }
    }
    break;
    case GAPC_LE_CB_DISCONNECTION:
    {
        if (status != AT_BLE_SUCCESS)
        {
            at_ble_lecb_disconnected_t *p = (at_ble_lecb_disconnected_t *)params;
            p->status = status;
            evt_num = AT_BLE_LECB_DISCONNECTED;
            PRINT_ERR("LECB link disconnection failure \n");
        }
        else
        {
            evt_num = AT_BLE_EVENT_MAX;
            PRINT_DBG("GAPC_CMP_EVT:OP GAPC_LE_CB_DISCONNECTION consumed\n");
        }
    }
    break;
    case GAPC_GET_PEER_NAME:
    /*
     * fall Through
     */
    case GAPC_GET_PEER_APPEARANCE:
    /*
     * fall Through
     */
    case GAPC_GET_PEER_SLV_PREF_PARAMS:
        if (AT_BLE_SUCCESS != status)
        {
            at_ble_gapc_cmp_evt_t *cmp_param = (at_ble_gapc_cmp_evt_t *)params;
            evt_num = AT_BLE_PEER_ATT_INFO_IND;
            PRINT_ERR("Get peer device info error, Status: %d", status);
            cmp_param->operation =  u8Operation;
            cmp_param->status = (at_ble_status_t)status;
        }
        else
        {
            evt_num = AT_BLE_EVENT_MAX;
            PRINT_DBG(" Discard event, GAPC_CMP_EVT:OP 0x%02x, Status 0x%02x\r\n", u8Operation, status);
        }
        break;
    case GAPC_GET_CON_CHANNEL_MAP:
        if (AT_BLE_SUCCESS != status)
        {
            at_ble_channel_map_t *p = (at_ble_channel_map_t *)params;
            evt_num = AT_BLE_CON_CHANNEL_MAP_IND;
            p->status = (at_ble_status_t)status;
            p->conn_handle = KE_IDX_GET(src);
            memset(p->map, 0, sizeof(p->map));
            PRINT_ERR("GAPC_GET_CON_CHANNEL_MAP, Status: 0x%02X", status);
        }
        else
        {
            evt_num = AT_BLE_EVENT_MAX;
            PRINT_DBG("GAPC_CMP_EVT:OP GAPC_GET_CON_CHANNEL_MAP consumed\n");
        }
        break;
    default:
    {
        PRINT_ERR("Unhandled OP <%02X>\r\n", u8Operation);
    }
    break;
    }
    if (AT_BLE_UNDEFINED_EVENT == evt_num)
    {
        PRINT_DBG("Undefined event, GAPC_CMP_EVT:OP 0x%02x, Status 0x%02x\r\n", u8Operation, status);
    }
    return evt_num;
}

uint8_t gapc_connection_cfm_handler(uint8_t *lcsrk, uint32_t local_signcntr,
                                    uint8_t *rcsrk, uint32_t remote_signcntr,
                                    uint8_t auth, uint8_t service_enabled, uint16_t handle)
{
    INTERFACE_MSG_INIT(GAPC_CONNECTION_CFM,  KE_BUILD_ID(TASK_GAPC, handle));
    INTERFACE_PACK_ARG_BLOCK(lcsrk, KEY_LEN);
    INTERFACE_PACK_ARG_UINT32(local_signcntr);
    INTERFACE_PACK_ARG_BLOCK(rcsrk, KEY_LEN);
    INTERFACE_PACK_ARG_UINT32(remote_signcntr);
    INTERFACE_PACK_ARG_UINT8(auth);
    INTERFACE_PACK_ARG_UINT8(0); // pading
    INTERFACE_PACK_ARG_UINT8(service_enabled);
    INTERFACE_PACK_ARG_UINT8(0); // pading
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return (uint8_t)AT_BLE_SUCCESS;
}

void gapc_con_req_ind(uint8_t *data)
{
    static uint8_t dummy_key[KEY_LEN] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t index = 0;
    uint8_t devRole = 0;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&(device.conn_handle));
    INTERFACE_UNPACK_SKIP(7); //we are not interested in those params
    INTERFACE_UNPACK_UINT8(&(device.peer_addr.type));
    INTERFACE_UNPACK_BLOCK(device.peer_addr.addr, AT_BLE_ADDR_LEN);
    INTERFACE_UNPACK_UINT8(&devRole);
    INTERFACE_DONE();
    index = check_ConnData_emptyEntry();
    if (AT_BLE_MAX_CONCURRENT_CONNS > index)
    {
        gstrConnData[index].conHandle = device.conn_handle;
        gstrConnData[index].role = devRole;
        gstrConnData[index].peerAddr.type = device.peer_addr.type;
        memcpy(&gstrConnData[index].peerAddr.addr[0], &device.peer_addr.addr[0], AT_BLE_ADDR_LEN);
    }
    gapc_connection_cfm_handler(dummy_key, 0, dummy_key, 0, GAP_AUTH_REQ_NO_MITM_NO_BOND,
                                0, gstrConnData[index].conHandle);
}

uint8_t gapc_disconnect_cmd_handler(uint8_t reason, uint16_t handle)
{
    INTERFACE_MSG_INIT(GAPC_DISCONNECT_CMD,  KE_BUILD_ID(TASK_GAPC, handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_DISCONNECT);
    INTERFACE_PACK_ARG_UINT8(reason);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return (uint8_t)AT_BLE_SUCCESS;
}

void gapc_disconnect_ind(uint8_t *data, at_ble_disconnected_t *param)
{
    uint8_t index = AT_BLE_MAX_CONCURRENT_CONNS;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&(param->handle));
    INTERFACE_UNPACK_UINT8(&(param->reason));
    INTERFACE_DONE();
    index = check_ConnData_idx_role(param->handle, NULL);
    if (AT_BLE_MAX_CONCURRENT_CONNS > index)
    {
        gstrConnData[index].conHandle = 0xFFFF;
    }
}

/*In case of MASTER role*/
void gapc_bond_cmd_handler(uint16_t conn_handle, uint8_t io_cababilities,
                           uint8_t oob_avaiable, uint8_t auth_req, uint8_t max_key_size,
                           uint8_t initiator_keys, uint8_t responder_keys, uint8_t sec_req)
{
    PRINT_DBG(">>IO CAP. 0x: %X\r\n"
              ">>OOB     0x: %X\r\n"
              ">>AUTHE.  0x: %X\r\n"
              ">>KEY SZ  0x: %X\r\n"
              ">>IKEYS   0x: %X\r\n"
              ">>RKEYS   0x: %X\r\n"
              ">>Sec.Req 0x: %X\r\n", io_cababilities, oob_avaiable, auth_req, max_key_size,
              initiator_keys, responder_keys, sec_req);
    INTERFACE_MSG_INIT(GAPC_BOND_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_BOND);
    INTERFACE_PACK_ARG_UINT8(io_cababilities);
    INTERFACE_PACK_ARG_UINT8(oob_avaiable);
    INTERFACE_PACK_ARG_UINT8(auth_req);
    // To do check max and min key size (not supported)
    INTERFACE_PACK_ARG_UINT8(max_key_size);
    INTERFACE_PACK_ARG_UINT8(initiator_keys);
    INTERFACE_PACK_ARG_UINT8(responder_keys);
    INTERFACE_PACK_ARG_UINT8(sec_req);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

at_ble_events_t gapc_bond_ind(uint16_t src, uint8_t *data, at_ble_pair_done_t *param)
{
    at_ble_events_t evt_num = AT_BLE_UNDEFINED_EVENT;
    uint8_t u8Info;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&u8Info);
    INTERFACE_UNPACK_SKIP(1); //padding
    switch (u8Info)
    {
    case GAPC_PAIRING_FAILED:
        evt_num = AT_BLE_PAIR_DONE;
        if (NULL != param)  //event will be posted and returned to user
        {
            param->handle = KE_IDX_GET(src);
						INTERFACE_UNPACK_UINT8(&u8Info);
            param->status = (at_ble_status_t)u8Info;
            PRINT_DBG("GAPC_PAIRING_FAILED:\r\n");
            PRINT_INFO("\t>>0x%X\r\n"
                       "\t>>0x%X\r\n", param->handle, param->status);
            memset((uint8_t *)&peer_key_info, 0, sizeof(at_key_info_t));
        }
        break;
    case GAPC_PAIRING_SUCCEED:
        evt_num = AT_BLE_PAIR_DONE;
        if (NULL != param)  //event will be posted and returned to user
        {
            param->handle = KE_IDX_GET(src);
            INTERFACE_UNPACK_UINT8(&u8Info);
            param->auth = (at_ble_auth_t)u8Info;
            param->status = AT_BLE_SUCCESS;//INTERFACE_UNPACK_UINT8(&u8Info);
            PRINT_DBG("GAPC_PAIRING_SUCCEED:\r\n");
            PRINT_INFO("\t%02x, %02x ", param->handle, param->auth);
            //if(peer_key_info.irk.key != NULL)
            {
                memcpy(&(param->peer_irk), &(peer_key_info.irk), sizeof(at_ble_IRK_t));
            }
            //if(peer_key_info.csrk.key != NULL)
            {
                memcpy(&(param->peer_csrk), &(peer_key_info.csrk), sizeof(at_ble_CSRK_t));
            }
            //if(peer_key_info.ltk.key != NULL)
            {
                memcpy(&(param->peer_ltk), &(peer_key_info.ltk), sizeof(at_ble_LTK_t));
            }
            memset((uint8_t *)&peer_key_info, 0, sizeof(at_key_info_t));
        }
        break;
    case GAPC_IRK_EXCH:
    {
        PRINT_DBG("GAPC_IRK_EXCH:\r\n");
        INTERFACE_UNPACK_BLOCK(peer_key_info.irk.key , KEY_LEN);
        INTERFACE_UNPACK_BLOCK((peer_key_info.irk.addr.addr), AT_BLE_ADDR_LEN);
        INTERFACE_UNPACK_UINT8(&(peer_key_info.irk.addr.type));
    }
    break;
    case GAPC_CSRK_EXCH:
    {
        PRINT_DBG("GAPC_CSRK_EXCH:\r\n");
        INTERFACE_UNPACK_BLOCK(peer_key_info.csrk.key , KEY_LEN);
    }
    break;
    case GAPC_LTK_EXCH:
    {
        PRINT_DBG("GAPC_LTK_EXCH:\r\n");
        INTERFACE_UNPACK_BLOCK(peer_key_info.ltk.key , KEY_LEN);
        INTERFACE_UNPACK_UINT16(&(peer_key_info.ltk.ediv));
        INTERFACE_UNPACK_BLOCK(peer_key_info.ltk.nb, 8);
        INTERFACE_UNPACK_UINT8(&(peer_key_info.ltk.key_size));
    }
    break;
    default:
    {
        PRINT_DBG("UNHANDLED: GAPC_BOND_IND OP %d\r\n", u8Info);
    }
    }
    INTERFACE_DONE();
    return evt_num;
}

static void gapc_bond_req(uint16_t src, uint8_t auth, at_ble_pair_request_t *param)
{
    param->handle = KE_IDX_GET(src);
    param->peer_features = (at_ble_auth_t)auth;
}

static void gapc_key_exch(uint8_t u8Req, uint16_t ConHdl)
{
    INTERFACE_MSG_INIT(GAPC_BOND_CFM, KE_BUILD_ID(TASK_GAPC, ConHdl));
    INTERFACE_PACK_ARG_UINT8(u8Req);
    INTERFACE_PACK_ARG_UINT8(1);
    switch (u8Req)
    {
    case GAPC_LTK_EXCH:
    {
        INTERFACE_PACK_ARG_BLOCK(device.ltk.key, AT_BLE_MAX_KEY_LEN);
        INTERFACE_PACK_ARG_UINT16(device.ltk.ediv);
        INTERFACE_PACK_ARG_BLOCK(device.ltk.nb, 8);
        INTERFACE_PACK_ARG_UINT8(device.ltk.key_size);
    }
    break;
    case GAPC_CSRK_EXCH:
    {
        INTERFACE_PACK_ARG_BLOCK(device.csrk.key, KEY_LEN);
    }
    break;
    }
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

uint8_t gapc_bond_req_ind(uint16_t src, uint8_t *data, void *param)
{
    uint8_t u8Req , pair_data;
    uint8_t evt_num = AT_BLE_UNDEFINED_EVENT;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&u8Req);
    INTERFACE_UNPACK_UINT8(&pair_data);
    INTERFACE_DONE();
    switch (u8Req)
    {
    case GAPC_PAIRING_REQ:  //pair_data->auth_req
        evt_num = AT_BLE_PAIR_REQUEST;
        if (NULL != param)  //event will be posted and returned to user
        {
            gapc_bond_req(src, pair_data, (at_ble_pair_request_t *)param);
        }
        break;
    case GAPC_TK_EXCH:  //pair_data->tk_type
        evt_num = AT_BLE_PAIR_KEY_REQUEST;
        if (NULL != param)  //event will be posted and returned to user
        {
            at_ble_pair_key_request_t *pair_key_request = (at_ble_pair_key_request_t *)param;
            pair_key_request->handle = KE_IDX_GET(src);
            pair_key_request->type = AT_BLE_PAIR_OOB; //Default value
            //To be modified out of bond or TK
            if (pair_data != GAP_TK_OOB)
            {
                pair_key_request->type = AT_BLE_PAIR_PASSKEY;
                if (pair_data == GAP_TK_KEY_ENTRY)
                {
                    pair_key_request->passkey_type = AT_BLE_PAIR_PASSKEY_ENTRY;
                }
                else if (pair_data == GAP_TK_DISPLAY)
                {
                    pair_key_request->passkey_type = AT_BLE_PAIR_PASSKEY_DISPLAY;
                }
            }
        }
        break;
    case GAPC_LTK_EXCH:     //pair_data->key_size
    case GAPC_CSRK_EXCH:    //pair_data->key_size
    {
        gapc_key_exch(u8Req, KE_IDX_GET(src));
    }
    break;
    default:
    {
        PRINT_DBG("Unhandled gapc_bond_req_ind %d\r\n", u8Req);
    }
    break;
    }
    return evt_num;
}

/*In case of SLAVE role*/
void gapc_bond_cfm_handler_pair_resp(uint8_t u8Accept, uint8_t u8IoCap, uint8_t u8OOB, uint8_t u8Auth,
                                     uint8_t u8KeySize, uint8_t u8IkeyDist, uint8_t u8RkeyDist, uint8_t u8SecReq,
                                     uint16_t u16ConHdl)
{
    INTERFACE_MSG_INIT(GAPC_BOND_CFM, KE_BUILD_ID(TASK_GAPC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(GAPC_PAIRING_RSP);
    INTERFACE_PACK_ARG_UINT8(u8Accept);
    INTERFACE_PACK_ARG_UINT8(u8IoCap);
    INTERFACE_PACK_ARG_UINT8(u8OOB);
    INTERFACE_PACK_ARG_UINT8(u8Auth);
    INTERFACE_PACK_ARG_UINT8(u8KeySize);
    INTERFACE_PACK_ARG_UINT8(u8IkeyDist);
    INTERFACE_PACK_ARG_UINT8(u8RkeyDist);
    INTERFACE_PACK_ARG_UINT8(u8SecReq);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gapc_bond_cfm_handler_key_exch(uint8_t u8Req,  uint8_t u8Accept, uint8_t *key,
                                    uint16_t u16ConHdl)
{
    INTERFACE_MSG_INIT(GAPC_BOND_CFM, KE_BUILD_ID(TASK_GAPC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(u8Req);
    INTERFACE_PACK_ARG_UINT8(u8Accept);
    INTERFACE_PACK_ARG_BLOCK(key, KEY_LEN);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gapc_security_cmd_handler(uint16_t conn_handle, uint8_t auth_req)
{
    INTERFACE_MSG_INIT(GAPC_SECURITY_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_SECURITY_REQ);
    INTERFACE_PACK_ARG_UINT8(auth_req);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gapc_encrypt_cmd_handler(uint16_t conn_handle, uint8_t *key,
                              uint16_t ediv, uint8_t *rand, uint8_t key_size , uint8_t auth)
{
    static uint8_t dummy_key[KEY_LEN] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    //Set AUTHEN type first
    gapc_connection_cfm_handler(dummy_key, 0, dummy_key, 0, auth, 0, conn_handle);
    INTERFACE_MSG_INIT(GAPC_ENCRYPT_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_ENCRYPT);
    //Added for structure padding
    INTERFACE_PACK_ARG_UINT8(0);
    INTERFACE_PACK_ARG_BLOCK(key, KEY_LEN);
    INTERFACE_PACK_ARG_UINT16(ediv);
    INTERFACE_PACK_ARG_BLOCK(rand, 8);
    INTERFACE_PACK_ARG_UINT8(key_size);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gapc_encrypt_req_ind(uint16_t src, uint8_t *data, at_ble_encryption_request_t *params)
{
    params->handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&(params->ediv));
    INTERFACE_UNPACK_BLOCK((params->nb), 8);
    INTERFACE_DONE();
}

void gapc_encrypt_cfm_handler(uint16_t conn_handle, uint8_t auth , uint8_t key_found,
                              uint8_t *key, uint8_t key_size)
{
    static uint8_t dummy_key[KEY_LEN] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    //Set AUTHEN type first
    gapc_connection_cfm_handler(dummy_key, 0, dummy_key, 0, auth, 0, conn_handle);
    INTERFACE_MSG_INIT(GAPC_ENCRYPT_CFM, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(key_found);
    INTERFACE_PACK_ARG_UINT8(0); // padding
    INTERFACE_PACK_ARG_BLOCK(key, KEY_LEN);
    //Encrption key size
    INTERFACE_PACK_ARG_UINT8(key_size);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gapc_encrypt_ind(uint16_t src, uint8_t *data, at_ble_encryption_status_changed_t *params)
{
    uint8_t auth;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&auth);
    INTERFACE_DONE();
    params->handle = KE_IDX_GET(src);
    params->authen = (at_ble_auth_t)auth;
    params->status = AT_BLE_SUCCESS;
}

at_ble_status_t gapc_param_update_cmd_handler(at_ble_handle_t handle,
        uint16_t intv_min,
        uint16_t intv_max,
        uint16_t latency,
        uint16_t superv_to,
        uint16_t ce_len_min,
        uint16_t ce_len_max)
{
    INTERFACE_MSG_INIT(GAPC_PARAM_UPDATE_CMD, KE_BUILD_ID(TASK_GAPC, handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_UPDATE_PARAMS); // operation
    INTERFACE_PACK_ARG_UINT8(0); // padding
    INTERFACE_PACK_ARG_UINT16(intv_min);
    INTERFACE_PACK_ARG_UINT16(intv_max);
    INTERFACE_PACK_ARG_UINT16(latency);
    INTERFACE_PACK_ARG_UINT16(superv_to);
    INTERFACE_PACK_ARG_UINT16(ce_len_min);
    INTERFACE_PACK_ARG_UINT16(ce_len_max);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return AT_BLE_SUCCESS;
}
at_ble_status_t gapc_param_update_cfm_handler(at_ble_handle_t handle,
        bool accept,
        uint16_t ce_len_min,
        uint16_t ce_len_max)
{
    INTERFACE_MSG_INIT(GAPC_PARAM_UPDATE_CFM, KE_BUILD_ID(TASK_GAPC, handle));
    INTERFACE_PACK_ARG_UINT8((uint8_t)accept);
    INTERFACE_PACK_ARG_UINT16(ce_len_min);
    INTERFACE_PACK_ARG_UINT16(ce_len_max);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return AT_BLE_SUCCESS;
}
at_ble_events_t gapc_param_updated_ind(uint16_t src_id,
                                       uint8_t *data,
                                       at_ble_conn_param_update_done_t *params)
{
    params->handle = KE_IDX_GET(src_id);
    params->status = AT_BLE_SUCCESS;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&(params->con_intv));
    INTERFACE_UNPACK_UINT16(&(params->con_latency));
    INTERFACE_UNPACK_UINT16(&(params->superv_to));
    INTERFACE_DONE();
    return AT_BLE_CONN_PARAM_UPDATE_DONE;
}
at_ble_events_t gapc_param_updated_req_ind(uint16_t src_id,
        uint8_t *data,
        at_ble_conn_param_update_request_t *params)
{
    params->handle = KE_IDX_GET(src_id);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&(params->con_intv_min));
    INTERFACE_UNPACK_UINT16(&(params->con_intv_max));
    INTERFACE_UNPACK_UINT16(&(params->con_latency));
    INTERFACE_UNPACK_UINT16(&(params->superv_to));
    INTERFACE_DONE();
    return AT_BLE_CONN_PARAM_UPDATE_REQUEST;
}

static void gapc_get_dev_info_cfm(uint8_t req, struct device_info *dev_info, at_ble_handle_t handle)
{
    INTERFACE_MSG_INIT(GAPC_GET_DEV_INFO_CFM,  KE_BUILD_ID(TASK_GAPC, handle));
    INTERFACE_PACK_ARG_UINT8(req);
    INTERFACE_PACK_ARG_UINT8(0);
    switch (req)
    {
    case GAPC_DEV_NAME:
        INTERFACE_PACK_ARG_UINT16(dev_info->dev_name.length);
        INTERFACE_PACK_ARG_BLOCK(&dev_info->dev_name.value[0], dev_info->dev_name.length);
        break;
    case GAPC_DEV_APPEARANCE:
        INTERFACE_PACK_ARG_UINT16(dev_info->appearance);
        break;
    case GAPC_DEV_SLV_PREF_PARAMS:
        INTERFACE_PACK_ARG_UINT16(dev_info->spcp_param.con_intv_min);
        INTERFACE_PACK_ARG_UINT16(dev_info->spcp_param.con_intv_max);
        INTERFACE_PACK_ARG_UINT16(dev_info->spcp_param.con_latency);
        INTERFACE_PACK_ARG_UINT16(dev_info->spcp_param.superv_to);
        break;
    default:
        break;
    }
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gapc_dev_info_req_ind_handler(uint16_t src, uint8_t *data, gapc_get_dev_info_req_ind_t *params)
{
    params->conn_handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&(params->req));
    INTERFACE_DONE();
    switch (params->req)
    {
    case GAPC_DEV_NAME:
        if (device.dev_name.length > 0)
        {
            /* send device name to peer by GAPC_GET_DEV_INFO_CFM */
            gapc_get_dev_info_cfm(params->req, &device, params->conn_handle);
        }
        else
        {
            PRINT_ERR("Device Name not set");
        }
        break;
    default:
        gapc_get_dev_info_cfm(params->req, &device, params->conn_handle);
        break;
    }
}

void gapc_sec_req_ind(uint16_t src, uint8_t *data, at_ble_slave_sec_request_t *params)
{
    uint8_t auth;
    params->handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&auth);
    INTERFACE_DONE();
#define BOND_BIT_POS    (0)
#define MITM_BIT_POS    (2)
    params->bond            = (auth >> BOND_BIT_POS) & 0x01;
    params->mitm_protection = (auth >> MITM_BIT_POS) & 0x01;
}

uint8_t gapc_sign_counter_ind_handler(uint16_t src, uint8_t *data, at_ble_sign_counter_t *params)
{
    params->stConnHandle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT32(&(params->u32LocalCounter));
    INTERFACE_UNPACK_UINT32(&(params->u32PeerCounter));
    INTERFACE_DONE();
    return (uint8_t)AT_BLE_SUCCESS;
}

uint8_t gapc_get_info_cmd_handler(uint16_t conn_handle, uint8_t operation, uint8_t *param)
{
    uint8_t u8Operation = 0, u8Status = 0;

    /* Dummy Assignment */
    u8Operation = u8Operation;

    switch (operation)
    {
    case GAPC_GET_CON_RSSI:
    {
        INTERFACE_MSG_INIT(GAPC_GET_INFO_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
        INTERFACE_PACK_ARG_UINT8(operation);
        INTERFACE_SEND_WAIT_ANY(GAPC_CON_RSSI_IND, GAPC_CMP_EVT, TASK_GAPC);
        if (!INTERFACE_EVENT_FAILURE())
        {
            INTERFACE_UNPACK_UINT8(param);
            INTERFACE_WAIT_FOR(GAPC_CMP_EVT, TASK_GAPC);
        }
        INTERFACE_UNPACK_UINT8(&u8Operation);
        INTERFACE_UNPACK_UINT8(&u8Status);
        INTERFACE_DONE();
    }
    break;
    default:
        INTERFACE_MSG_INIT(GAPC_GET_INFO_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
        INTERFACE_PACK_ARG_UINT8(operation);
        INTERFACE_SEND_NO_WAIT();
        INTERFACE_DONE();
        break;
    }
    return u8Status;
}

void gapc_peer_att_info_ind_handler(uint16_t src, uint8_t *data, at_ble_peer_att_info_ind_t *param)
{
    param->conn_handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&param->req);
    INTERFACE_UNPACK_UINT16(&param->att_handle);
    switch (param->req)
    {
    case GAPC_DEV_NAME:
        INTERFACE_UNPACK_UINT16(&param->info.name.length);
        INTERFACE_UNPACK_BLOCK(&param->info.name.value[0], param->info.name.length);
        param->req = AT_BLE_GET_PEER_NAME;
        break;
    case GAPC_DEV_APPEARANCE:
        INTERFACE_UNPACK_UINT16(&param->info.appearance);
        param->req = AT_BLE_GET_PEER_APPEARANCE;
        break;
    case GAPC_DEV_SLV_PREF_PARAMS:
        INTERFACE_UNPACK_UINT16(&param->info.slv_params.con_intv_max);
        INTERFACE_UNPACK_UINT16(&param->info.slv_params.con_intv_min);
        INTERFACE_UNPACK_UINT16(&param->info.slv_params.superv_to);
        INTERFACE_UNPACK_UINT16(&param->info.slv_params.con_latency);
        param->req = AT_BLE_GET_PEER_SLV_PREF_PARAMS;
        break;
    default:
        PRINT_DBG(" unhandled request, operation: %d ", param->req);
        break;
    }
    INTERFACE_DONE();
}

void gapc_con_channel_map_ind_handler(uint16_t src, uint8_t *data, at_ble_channel_map_t *param)
{
    param->status = AT_BLE_SUCCESS;
    param->conn_handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_BLOCK(&param->map[0], sizeof(param->map));
    INTERFACE_DONE();
}



// LECB cmds handlers

at_ble_status_t gapc_lecb_create_handler(at_ble_handle_t conn_handle,
        uint16_t sec_lvl, uint16_t le_psm,
        uint16_t cid, uint16_t initial_credit)
{
    uint8_t u8Operation, u8Status;
    INTERFACE_MSG_INIT(GAPC_LECB_CREATE_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_LE_CB_CREATE);
    INTERFACE_PACK_ARG_DUMMY(1);
    INTERFACE_PACK_ARG_UINT16(sec_lvl);
    INTERFACE_PACK_ARG_UINT16(le_psm);
    INTERFACE_PACK_ARG_UINT16(cid);
    INTERFACE_PACK_ARG_UINT16(initial_credit);
    INTERFACE_SEND_WAIT(GAPC_CMP_EVT, TASK_GAPC);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    return (at_ble_status_t)u8Status;
}

at_ble_status_t gapc_lecb_destroy_handler(at_ble_handle_t conn_handle, uint16_t le_psm)
{
    uint8_t u8Operation, u8Status;
    INTERFACE_MSG_INIT(GAPC_LECB_DESTROY_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_LE_CB_DESTROY);
    INTERFACE_PACK_ARG_DUMMY(1);
    INTERFACE_PACK_ARG_UINT16(le_psm);
    INTERFACE_SEND_WAIT(GAPC_CMP_EVT, TASK_GAPC);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    return (at_ble_status_t)u8Status;
}

void gapc_lecb_connect_cmd_handler(at_ble_handle_t conn_handle, uint16_t le_psm, uint16_t cid, uint16_t intial_credit)
{
    INTERFACE_MSG_INIT(GAPC_LECB_CONNECT_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_LE_CB_CONNECTION);
    INTERFACE_PACK_ARG_DUMMY(1);//Instead of packet ID
    INTERFACE_PACK_ARG_UINT16(le_psm);
    INTERFACE_PACK_ARG_UINT16(cid);
    INTERFACE_PACK_ARG_UINT16(intial_credit);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

at_ble_status_t gapc_lecb_connect_cfm_handler(at_ble_handle_t conn_handle, uint16_t le_psm,
        at_ble_lecb_status status)
{
    uint8_t u8Status = AT_BLE_SUCCESS;
    INTERFACE_MSG_INIT(GAPC_LECB_CONNECT_CFM, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT16(le_psm);
    INTERFACE_PACK_ARG_UINT16(status);
    INTERFACE_SEND_NO_WAIT();
    //INTERFACE_SEND_WAIT(GAPC_CMP_EVT,TASK_GAPC); ?? Behaviour when testing doesn't match docs
    //INTERFACE_UNPACK_UINT8(&u8Operation);
    //INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    return (at_ble_status_t)u8Status;
}

void gapc_lecb_disconnect_cmd_handler(at_ble_handle_t conn_handle, uint16_t le_psm)
{
    INTERFACE_MSG_INIT(GAPC_LECB_DISCONNECT_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_LE_CB_DISCONNECTION);
    INTERFACE_PACK_ARG_DUMMY(1);//Instead of packet ID
    INTERFACE_PACK_ARG_UINT16(le_psm);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}



at_ble_status_t gapc_lecb_add_cmd_handler(at_ble_handle_t conn_handle, uint16_t le_psm, uint16_t credit)
{
    uint8_t u8Operation, u8Status;
    INTERFACE_MSG_INIT(GAPC_LECB_ADD_CMD, KE_BUILD_ID(TASK_GAPC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GAPC_LE_CB_ADDITION);
    INTERFACE_PACK_ARG_DUMMY(1);//Instead of packet ID
    INTERFACE_PACK_ARG_UINT16(le_psm);
    INTERFACE_PACK_ARG_UINT16(credit);
    INTERFACE_SEND_WAIT(GAPC_CMP_EVT, TASK_GAPC);
    INTERFACE_UNPACK_UINT8(&u8Operation);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    return (at_ble_status_t)u8Status;
}

//LECB events handler
void gapc_lecb_connected(uint8_t *data, at_ble_lecb_connected_t *param)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&param->le_psm);
    INTERFACE_UNPACK_UINT16(&param->dest_credit);
    INTERFACE_UNPACK_UINT16(&param->max_sdu);
    INTERFACE_UNPACK_UINT16(&param->dest_cid);
    INTERFACE_DONE();
    param->status = AT_BLE_SUCCESS;
}
void gapc_lecb_disconnected(uint8_t *data, at_ble_lecb_disconnected_t *param)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&param->le_psm);
    INTERFACE_UNPACK_UINT16(&param->reason);
    INTERFACE_DONE();
}
void gapc_lecb_connect_req_ind(uint8_t *data, at_ble_lecb_conn_req_t *param)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&param->le_psm);
    INTERFACE_UNPACK_UINT16(&param->dest_credit);
    INTERFACE_UNPACK_UINT16(&param->max_sdu);
    INTERFACE_UNPACK_UINT16(&param->dest_cid);
    INTERFACE_DONE();
}

void gapc_lecb_add_credit_resp_handler(uint8_t *data, at_ble_lecb_add_credit_ind_t *param)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&param->le_psm);
    INTERFACE_UNPACK_UINT16(&param->src_credit);
    INTERFACE_UNPACK_UINT16(&param->dest_credit);
    INTERFACE_DONE();
}
