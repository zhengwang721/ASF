/*
 * gattc_task.c
 *
 */
#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"
#include "dbg_logs.h"

#include "gattc_task.h"
#include "gattm_task.h"

// operation sequence number
static  uint16_t g_seq_num_tx = 0;
static  uint16_t g_seq_num_rx = 0;
uint16_t Seq_NUM = 0;



void gattc_read_req_ind_parser(uint8_t *data, at_ble_characteristic_read_req_t *params)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->char_handle);
    INTERFACE_DONE();
}

void gattc_write_req_ind_parser(uint8_t *data, at_ble_characteristic_write_req_t *params)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->char_handle);
    INTERFACE_UNPACK_UINT16(&params->offset);
    INTERFACE_UNPACK_UINT16(&params->length);
    INTERFACE_UNPACK_BLOCK(&params->value, params->length);
    INTERFACE_DONE();
}


void gattc_att_info_req_parser(uint8_t *data, at_ble_att_info_req_t *params)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->char_handle);
    INTERFACE_DONE();
}

void gattc_read_cfm(uint16_t handle, uint16_t u16ConHdl, uint8_t status, uint16_t length, uint8_t *value)
{
    INTERFACE_MSG_INIT(GATTC_READ_CFM, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT16(handle);
    INTERFACE_PACK_ARG_UINT16(length);
    INTERFACE_PACK_ARG_UINT8(status);
    if (length)
    {
        INTERFACE_PACK_ARG_BLOCK(value, length);
    }
    else
    {
        INTERFACE_PACK_ARG_UINT8(0);
    }
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}
void gattc_write_cfm(uint16_t handle, uint16_t u16ConHdl, uint8_t status)
{
    INTERFACE_MSG_INIT(GATTC_WRITE_CFM, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT16(handle);
    INTERFACE_PACK_ARG_UINT8(status);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_att_info_cfm(uint16_t handle , uint16_t u16ConHdl, uint16_t length, uint8_t status)
{
    INTERFACE_MSG_INIT(GATTC_ATT_INFO_CFM, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT16(handle);
    INTERFACE_PACK_ARG_UINT16(length);
    INTERFACE_PACK_ARG_UINT8(status);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}



void gattc_send_evt_cmd_handler(uint8_t u8ReqType, uint16_t u16AttHdl, uint16_t u16ConHdl, uint8_t *pvalue, uint16_t u16length)
{
    INTERFACE_MSG_INIT(GATTC_SEND_EVT_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(u8ReqType);
    INTERFACE_PACK_ARG_UINT16(Seq_NUM + 1); //Seq Number
    INTERFACE_PACK_ARG_UINT8(0);//padding
    INTERFACE_PACK_ARG_UINT16(u16AttHdl);
    INTERFACE_PACK_ARG_UINT16(u16length);
    INTERFACE_PACK_ARG_BLOCK(pvalue, u16length);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}




void gattc_disc_cmd_handler(uint8_t u8ReqType, uint8_t u8UUIDLen, uint16_t u16StartHdl,
                            uint16_t u16EndHdl, uint8_t *pu8UUID, uint16_t u16ConHdl)
{
    /* Start packing the message*/
    // build the KE ID
    INTERFACE_MSG_INIT(GATTC_DISC_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    // packing the request type GATTC_DISC_BY_UUID_SVC or GATTC_DISC_ALL_SVC
    INTERFACE_PACK_ARG_UINT8(u8ReqType);
    if (pu8UUID == NULL)
    {
        // even if no UUID is sent this must be a valid value to pass some error check in FW
        INTERFACE_PACK_ARG_UINT8(2);
    }
    else
    {
        INTERFACE_PACK_ARG_UINT8(u8UUIDLen);
    }
    INTERFACE_PACK_ARG_UINT16(g_seq_num_tx);
    g_seq_num_tx ++;
    INTERFACE_PACK_ARG_UINT16(u16StartHdl);
    INTERFACE_PACK_ARG_UINT16(u16EndHdl);
    if (pu8UUID == NULL)
    {
        INTERFACE_PACK_ARG_UINT16(0); // padding in case of no value
    }
    else
    {
        INTERFACE_PACK_ARG_BLOCK(pu8UUID, u8UUIDLen);
    }
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_reg_to_peer_handler(uint8_t u8ReqType, uint16_t u16StartHdl, uint16_t u16EndHdl,
                               uint16_t u16ConHdl)
{
    INTERFACE_MSG_INIT(GATTC_REG_TO_PEER_EVT_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(u8ReqType);
    INTERFACE_PACK_ARG_UINT8(0); //zero padding
    INTERFACE_PACK_ARG_UINT16(u16StartHdl);
    INTERFACE_PACK_ARG_UINT16(u16EndHdl);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_write_execute_cmd_handler(uint8_t u8Execute, uint16_t u16ConHdl)
{
    INTERFACE_MSG_INIT(GATTC_EXECUTE_WRITE_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(GATTC_EXEC_WRITE);
    INTERFACE_PACK_ARG_UINT8(u8Execute);
    INTERFACE_PACK_ARG_UINT16(g_seq_num_tx);
    g_seq_num_tx ++;
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_write_cmd_handler(uint8_t u8ReqType, uint8_t u8AutoExec, uint16_t u16AttHdl, uint16_t u16Offset,
                             uint16_t u16Length, uint8_t *pu8Val, uint16_t u16ConHdl)
{
    // Internal write cursor shall be initialised to 0
#define CURSOR_INTIAL_VALUE ((uint16_t)(0))
    INTERFACE_MSG_INIT(GATTC_WRITE_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(u8ReqType);
    INTERFACE_PACK_ARG_UINT8(u8AutoExec);
    INTERFACE_PACK_ARG_UINT16(g_seq_num_tx);
    g_seq_num_tx ++;
    INTERFACE_PACK_ARG_UINT16(u16AttHdl);
    INTERFACE_PACK_ARG_UINT16(u16Offset);
    INTERFACE_PACK_ARG_UINT16(u16Length);
    INTERFACE_PACK_ARG_UINT16(CURSOR_INTIAL_VALUE); // Internal write cursor shall be initialised to 0
    INTERFACE_PACK_ARG_BLOCK(pu8Val, u16Length);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_read_cmd_handler_by_uuid(uint16_t u16StartHdl, uint16_t u16EndHdl,
                                    uint8_t u8UUIDLen, uint8_t *pu8UUID, uint16_t u16ConHdl)
{
    INTERFACE_MSG_INIT(GATTC_READ_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(GATTC_READ_BY_UUID);
    INTERFACE_PACK_ARG_UINT8(0);
    INTERFACE_PACK_ARG_UINT16(u16StartHdl);
    INTERFACE_PACK_ARG_UINT16(u16EndHdl);
    INTERFACE_PACK_ARG_UINT8(u8UUIDLen);
    INTERFACE_PACK_ARG_BLOCK(pu8UUID, u8UUIDLen);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_read_cmd_handler_simple_read(uint16_t u16Handle, uint16_t u16Offset,
                                        uint16_t u16Len, uint16_t u16ConHdl)
{
    INTERFACE_MSG_INIT(GATTC_READ_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(GATTC_READ);
    INTERFACE_PACK_ARG_UINT8(0);
    INTERFACE_PACK_ARG_UINT16(u16Handle);
    INTERFACE_PACK_ARG_UINT16(u16Offset);
    INTERFACE_PACK_ARG_UINT16(u16Len);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_read_cmd_handler_multible(uint16_t *u16Handle_list, uint8_t u8Count,
                                     uint16_t *u16Length_list, uint16_t u16ConHdl)
{
    uint8_t counter;
    INTERFACE_MSG_INIT(GATTC_READ_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(GATTC_READ_MULTIPLE);
    INTERFACE_PACK_ARG_UINT8(u8Count);
    for (counter = 0; counter < u8Count; counter++)
    {
        INTERFACE_PACK_ARG_UINT16(u16Handle_list[counter]);
        INTERFACE_PACK_ARG_UINT16(u16Length_list[counter]);
    }
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_mtu_cmd_handler(uint16_t u16ConHdl)
{
#define REQ_TYPE (GATTC_MTU_EXCH)
    INTERFACE_MSG_INIT(GATTC_EXC_MTU_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(REQ_TYPE);
    INTERFACE_PACK_ARG_UINT16(g_seq_num_tx);
    g_seq_num_tx ++;
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_event_cfm_handler(uint16_t u16ConHdl, uint16_t u16AttHdl)
{
    INTERFACE_MSG_INIT(GATTC_EVENT_CFM, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT16(u16AttHdl);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_read_cmd_handler(enum gattc_operation u8Op, uint16_t u16SeqNo,
                            uint16_t u16par1, uint16_t u16par2,  uint16_t u16par3, uint8_t *pu8UUID,
                            uint16_t u16ConHdl)
{
    INTERFACE_MSG_INIT(GATTC_READ_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(u8Op);
    INTERFACE_PACK_ARG_UINT8(0);
    INTERFACE_PACK_ARG_UINT16(u16SeqNo);
    switch (u8Op)
    {
    case GATTC_READ:
    case GATTC_READ_LONG:
        INTERFACE_PACK_ARG_UINT16(u16par1);
        INTERFACE_PACK_ARG_UINT16(u16par2);
        INTERFACE_PACK_ARG_UINT16(u16par3);
        break;
    case GATTC_READ_BY_UUID:
        INTERFACE_PACK_ARG_UINT16(u16par1);
        INTERFACE_PACK_ARG_UINT16(u16par2);
        INTERFACE_PACK_ARG_UINT8((uint8_t)u16par3);
        INTERFACE_PACK_ARG_BLOCK(pu8UUID, u16par3);
        break;
    default:
        break;
    }
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_read_multiple_handler(at_ble_char_list *char_handle_list, uint8_t char_handle_count, uint16_t seq_num, at_ble_handle_t conn_handle)
{
    int i = 0;
    INTERFACE_MSG_INIT(GATTC_READ_CMD, KE_BUILD_ID(TASK_GATTC, conn_handle));
    INTERFACE_PACK_ARG_UINT8(GATTC_READ_MULTIPLE);
    INTERFACE_PACK_ARG_UINT8(char_handle_count);
    INTERFACE_PACK_ARG_UINT16(seq_num);
    for (; i < char_handle_count; i++)
    {
        INTERFACE_PACK_ARG_UINT16(char_handle_list[i].att_handle);
        INTERFACE_PACK_ARG_UINT16(char_handle_list[i].att_len);
    }
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}


void gattc_disc_svc_ind_parser(uint16_t src, uint8_t *data, at_ble_primary_service_found_t *params)
{
    uint8_t uuid_len;
    params->conn_handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->start_handle);
    INTERFACE_UNPACK_UINT16(&params->end_handle);
    INTERFACE_UNPACK_UINT8(&uuid_len);
    params->service_uuid.type = at_ble_uuid_len2type(uuid_len);
    INTERFACE_UNPACK_BLOCK(params->service_uuid.uuid, uuid_len);
    INTERFACE_DONE();
}

void gattc_disc_svc_incl_ind_parser(uint16_t src, uint8_t *data, at_ble_included_service_found_t *params)
{
    uint8_t uuid_len;
    params->conn_handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->attr_handle);
    INTERFACE_UNPACK_UINT16(&params->start_handle);
    INTERFACE_UNPACK_UINT16(&params->end_handle);
    INTERFACE_UNPACK_UINT8(&uuid_len);
    params->service_uuid.type = at_ble_uuid_len2type(uuid_len);
    INTERFACE_UNPACK_BLOCK(params->service_uuid.uuid, uuid_len);
    INTERFACE_DONE();
}

void gattc_disc_char_ind_parser(uint16_t src, uint8_t *data, at_ble_characteristic_found_t *params)
{
    uint8_t uuid_len;
    params->conn_handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->char_handle);
    INTERFACE_UNPACK_UINT16(&params->value_handle);
    INTERFACE_UNPACK_UINT8(&params->properties);
    INTERFACE_UNPACK_UINT8(&uuid_len);
    params->char_uuid.type = at_ble_uuid_len2type(uuid_len);
    INTERFACE_UNPACK_BLOCK(params->char_uuid.uuid, uuid_len);
    INTERFACE_DONE();
}

void gattc_disc_char_desc_ind_parser(uint16_t src, uint8_t *data, at_ble_descriptor_found_t *params)
{
    uint8_t uuid_len;
    params->conn_handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->desc_handle);
    INTERFACE_UNPACK_UINT8(&uuid_len);
    params->desc_uuid.type = at_ble_uuid_len2type(uuid_len);
    INTERFACE_UNPACK_BLOCK(params->desc_uuid.uuid, uuid_len);
    INTERFACE_DONE();
}
at_ble_events_t gattc_event_ind_and_notification_parser(uint16_t src, uint8_t *data, void *params)
{
    uint8_t operation = 0;
    at_ble_events_t evt = AT_BLE_UNDEFINED_EVENT;
    uint16_t char_handle = 0;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&operation);
    INTERFACE_UNPACK_SKIP(1);   // irrelevant byte
    if (GATTC_NOTIFY == operation)
    {
        at_ble_notification_recieved_t *params_notify = (at_ble_notification_recieved_t *)params;
        evt = AT_BLE_NOTIFICATION_RECIEVED;
        params_notify->conn_handle = KE_IDX_GET(src);
        INTERFACE_UNPACK_UINT8(&(params_notify->char_len));
        INTERFACE_UNPACK_SKIP(1);   // irrelevant byte
        INTERFACE_UNPACK_UINT16(&char_handle);
        params_notify->char_handle = char_handle;
        INTERFACE_UNPACK_BLOCK(params_notify->char_value, (params_notify->char_len));
    }
    else if (GATTC_INDICATE == operation)
    {
        at_ble_indication_recieved_t *params_indicate = (at_ble_indication_recieved_t *)params;
        evt = AT_BLE_INDICATION_RECIEVED;
        params_indicate->conn_handle = KE_IDX_GET(src);
        INTERFACE_UNPACK_UINT8(&(params_indicate->char_len));
        INTERFACE_UNPACK_UINT16(&char_handle);
        params_indicate->char_handle = char_handle;
        INTERFACE_UNPACK_SKIP(1);   // irrelevant byte
        INTERFACE_UNPACK_BLOCK(params_indicate->char_value, (params_indicate->char_len));
    }
    INTERFACE_DONE();
    return evt;
}

at_ble_events_t gattc_complete_evt_handler(uint16_t src, uint8_t *data, void *params)
{
    at_ble_events_t evt_num = AT_BLE_UNDEFINED_EVENT;
    at_ble_cmd_complete_event_t *parameters = (at_ble_cmd_complete_event_t *) params;
    uint8_t status = 0;
    parameters->conn_handle = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&parameters->operation);
    INTERFACE_UNPACK_UINT8(&status);
    INTERFACE_UNPACK_UINT16(&g_seq_num_rx);
    INTERFACE_DONE();
    parameters->status = (at_ble_status_t)status;
    switch (parameters->operation)
    {
    case GATTC_DISC_ALL_SVC:
    /*
     * fall through
     */
    case GATTC_DISC_BY_UUID_SVC:
    /*
     * fall through
     */
    case GATTC_DISC_INCLUDED_SVC:
    /*
     * fall through
     */
    case GATTC_DISC_ALL_CHAR:
    /*
     * fall through
     */
    case GATTC_DISC_BY_UUID_CHAR:
    /*
     * fall through
     */
    case GATTC_DISC_DESC_CHAR:
        evt_num = AT_BLE_DISCOVERY_COMPLETE;
        break;
    case GATTC_MTU_EXCH:
        evt_num = AT_BLE_MTU_CHANGED_CMD_COMPLETE;
        if (AT_BLE_SUCCESS == status)
        {
            /* discard event in case of
             * status = AT_BLE_SUCCESS
             */
            evt_num = AT_BLE_EVENT_MAX;
        }
        break;
    case GATTC_WRITE:
    /*
     * fall through
     */
    case GATTC_WRITE_NO_RESPONSE:
    /*
     * fall through
     */
    case GATTC_WRITE_SIGNED:
    /*
     * fall through
     */
    case GATTC_EXEC_WRITE:
        evt_num = AT_BLE_CHARACTERISTIC_WRITE_CMD_CMP;
        break;
    case GATTC_NOTIFY:
        u32usedBuffers--;
        evt_num = AT_BLE_NOTIFICATION_CONFIRMED;
        break;
    case GATTC_INDICATE:
        u32usedBuffers--;
        evt_num = AT_BLE_INDICATION_CONFIRMED;
        break;
    case GATTC_READ_MULTIPLE :
        evt_num = AT_BLE_CHARACTERISTIC_READ_MULTIBLE_RESPONSE;
        break;
    case GATTC_READ :
    case GATTC_READ_BY_UUID :
    case GATTC_READ_LONG :
    {
        /* in case of failure only shall we pass this event to the user */
        if (status != AT_BLE_SUCCESS)
        {
            at_ble_characteristic_read_response_t *p = (at_ble_characteristic_read_response_t *)params;
            evt_num = AT_BLE_CHARACTERISTIC_READ_RESPONSE;
            p->status = (at_ble_status_t)status;
            PRINT_ERR("Failed reading characteristic \n");
        }
        else
        {
            /* discard event in case of
             * status = AT_BLE_SUCCESS
             */
            evt_num = AT_BLE_EVENT_MAX;
        }
    }
    break;
    case GATTC_SVC_CHANGED:
        evt_num = AT_BLE_SERVICE_CHANGED_INDICATION_SENT;
        break;
    default:
    {
        PRINT_ERR("Unhandled OP <%02X>\r\n", parameters->operation);
    }
    break;
    }
    if (AT_BLE_UNDEFINED_EVENT == evt_num)
    {
        PRINT_DBG("GATTC_CMP_EVT:OP 0x%02x Status 0x%02x\r\n", parameters->operation, parameters->status);
    }
    return evt_num;
}

void gattc_event_mtu_changed_ind_parser(uint16_t src, uint8_t *data, at_ble_mtu_changed_ind_t *params)
{
    params->conhdl = KE_IDX_GET(src);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->mtu_value);
    INTERFACE_DONE();
}

void gattc_read_ind_parser(uint16_t src, uint8_t *data, at_ble_characteristic_read_response_t *params)
{
    params->conn_handle = KE_IDX_GET(src);
    params->status = AT_BLE_SUCCESS;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->char_handle);
    INTERFACE_UNPACK_UINT16(&params->char_offset);
    INTERFACE_UNPACK_UINT16(&params->char_len);
    INTERFACE_UNPACK_BLOCK(&params->char_value[0], params->char_len);
    INTERFACE_DONE();
}

void gattc_svc_changed_ind_cmd_handler(uint16_t u16ConHdl,
                                       uint16_t u16StartHdl,
                                       uint16_t u16EndHdl)
{
    INTERFACE_MSG_INIT(GATTC_SVC_CHANGED_IND_CMD, KE_BUILD_ID(TASK_GATTC, u16ConHdl));
    INTERFACE_PACK_ARG_UINT8(GATTC_SVC_CHANGED); // operation
    INTERFACE_PACK_ARG_UINT8(0); // padding
    INTERFACE_PACK_ARG_UINT16(g_seq_num_tx);
    g_seq_num_tx++;
    INTERFACE_PACK_ARG_UINT16(u16StartHdl);
    INTERFACE_PACK_ARG_UINT16(u16EndHdl);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gattc_svc_changed_cfg_ind_parser(uint16_t  src_id,
                                      uint8_t  *data,
                                      at_ble_characteristic_configuration_changed_t *params)
{
    params->conn_handle = KE_IDX_GET(src_id);
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&params->cfg);
    INTERFACE_DONE();
}
