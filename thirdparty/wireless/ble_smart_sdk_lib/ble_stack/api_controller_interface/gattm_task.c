/*
 * gattm_task.c
 *
 */

#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"

#include "gattm_task.h"
uint32_t u32usedBuffers = 0;

uint8_t gattm_add_svc_req_handler(struct gattm_svc_desc *svc_desc)
{
    uint8_t u8Status = AT_BLE_SUCCESS;
    char NAtts = svc_desc->nb_att;
    int idx = 0;
    INTERFACE_MSG_INIT(GATTM_ADD_SVC_REQ, TASK_GATTM);
    INTERFACE_PACK_ARG_UINT16(svc_desc->start_hdl);
    INTERFACE_PACK_ARG_UINT16(svc_desc->task_id);
    INTERFACE_PACK_ARG_UINT8(svc_desc->perm);
    INTERFACE_PACK_ARG_UINT8(NAtts);
    for (idx = 0; idx < 16; idx++)
    {
        INTERFACE_PACK_ARG_UINT8(svc_desc->uuid[idx]);
    }
    for (idx = 0; idx < NAtts; idx++)
    {
        INTERFACE_PACK_ARG_BLOCK(svc_desc->atts[idx].uuid, ATT_UUID_128_LEN);
        INTERFACE_PACK_ARG_UINT16(svc_desc->atts[idx].perm);
        INTERFACE_PACK_ARG_UINT16(svc_desc->atts[idx].max_len);
    }
    INTERFACE_SEND_WAIT(GATTM_ADD_SVC_RSP, TASK_GATTM);
    INTERFACE_UNPACK_UINT16(&svc_desc->start_hdl);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    return u8Status;
}

uint8_t gattm_add_attribute_req_handler(struct gattm_svc_desc *svc_desc, uint16_t max_data_size, uint16_t perm,
                                        uint8_t uuid_len, uint8_t *uuid, uint16_t *handle)
{
    if (svc_desc->nb_att >= ((INTERFACE_SEND_BUF_MAX - sizeof(struct gattm_svc_desc)) / sizeof(struct gattm_att_desc)))
    {
        ERROR_REPORT;
        return AT_BLE_FAILURE;
    }
    memset(&svc_desc->atts[svc_desc->nb_att].uuid[0], 0, ATT_UUID_128_LEN);
    memcpy(&svc_desc->atts[svc_desc->nb_att].uuid[0], uuid, uuid_len);
    svc_desc->atts[svc_desc->nb_att].max_len = max_data_size;
    svc_desc->atts[svc_desc->nb_att].perm = perm;
    *handle = svc_desc->nb_att;
    svc_desc->nb_att++;
    return AT_BLE_SUCCESS;
}



uint8_t gattm_att_set_value_req_handler(uint16_t handle, uint16_t length, uint8_t *value)
{
    uint8_t u8Status = AT_BLE_FAILURE;
    uint16_t u16RcvHandle;
    INTERFACE_MSG_INIT(GATTM_ATT_SET_VALUE_REQ, TASK_GATTM);
    INTERFACE_PACK_ARG_UINT16(handle);
    INTERFACE_PACK_ARG_UINT16(length);
    INTERFACE_PACK_ARG_BLOCK(value, length);
    INTERFACE_SEND_WAIT(GATTM_ATT_SET_VALUE_RSP, TASK_GATTM);
    INTERFACE_UNPACK_UINT16(&u16RcvHandle);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    return u8Status;
}


uint8_t gattm_att_get_value_req_handler(uint16_t handle, uint16_t *length, uint8_t *value)
{
    uint16_t u16RcvHandle;
    uint8_t u8Status = AT_BLE_FAILURE;
    INTERFACE_MSG_INIT(GATTM_ATT_GET_VALUE_REQ, TASK_GATTM);
    INTERFACE_PACK_ARG_UINT16(handle);
    INTERFACE_SEND_WAIT(GATTM_ATT_GET_VALUE_RSP, TASK_GATTM);
    INTERFACE_UNPACK_UINT16(&u16RcvHandle);
    INTERFACE_UNPACK_UINT16(length);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_UNPACK_BLOCK(value, *length);
    INTERFACE_DONE();
    if (handle != u16RcvHandle)
    {
        return AT_BLE_FAILURE;
    }
    return u8Status;
}

#if 0


uint8_t gatt_notify_req_handler(uint16_t conhdl, uint16_t charhdl)
{
    uint8_t u8Status, u8PktId;
    uint16_t u16MsgId, u16DestId, u16SrcId;
    INTERFACE_MSG_INIT(GATT_NOTIFY_REQ, TASK_GATTM, NMI_TRUE);
    INTERFACE_PACK_ARG_UINT16(conhdl);
    INTERFACE_PACK_ARG_UINT16(charhdl);
    INTERFACE_SEND_NO_WAIT();
    NMI_IF_UNPACK_HDR(&u8PktId, &u16MsgId, &u16DestId, &u16SrcId);
    INTERFACE_UNPACK_UINT16(&charhdl);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if ((u8PktId != INTERFACE_API_PKT_ID) || (u16MsgId != GATT_NOTIFY_CMP_EVT) || (u16DestId != TASK_EXTERN) || (u16SrcId != TASK_GATTM))
    {
        return NMI_ERR_HDR;
    }
    return u8Status;
}

uint8_t gatt_indicate_req_handler(uint16_t conhdl, uint16_t charhdl)
{
    uint8_t u8Status, u8PktId;
    uint16_t u16MsgId, u16DestId, u16SrcId;
    INTERFACE_MSG_INIT(GATT_INDICATE_REQ, TASK_GATTM, NMI_TRUE);
    INTERFACE_PACK_ARG_UINT16(conhdl);
    INTERFACE_PACK_ARG_UINT16(charhdl);
    INTERFACE_SEND_NO_WAIT();
    NMI_IF_UNPACK_HDR(&u8PktId, &u16MsgId, &u16DestId, &u16SrcId);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if ((u8PktId != INTERFACE_API_PKT_ID) || (u16MsgId != GATT_CMP_EVT) || (u16DestId != TASK_EXTERN) || (u16SrcId != TASK_GATTM))
    {
        return NMI_ERR_HDR;
    }
    return u8Status;
}

uint8_t gatt_write_attribute_req_handler(uint16_t length, uint16_t attr_hdl, uint16_t perm,
        uint8_t *val)
{
    uint8_t u8Status, u8PktId;
    uint16_t u16MsgId, u16DestId, u16SrcId;
    INTERFACE_MSG_INIT(GATT_WRITE_ATTRIBUTE_REQ, TASK_GATTM, NMI_TRUE);
    INTERFACE_PACK_ARG_UINT16(length);
    INTERFACE_PACK_ARG_UINT16(attr_hdl);
    INTERFACE_PACK_ARG_UINT16(perm);
    INTERFACE_PACK_ARG_BLOCK(val, length);
    INTERFACE_SEND_NO_WAIT();
    NMI_IF_UNPACK_HDR(&u8PktId, &u16MsgId, &u16DestId, &u16SrcId);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if ((u8PktId != INTERFACE_API_PKT_ID) || (u16MsgId != GATT_WRITE_ATTRIBUTE_CMP_EVT) || (u16DestId != TASK_EXTERN) || (u16SrcId != TASK_GATTM))
    {
        return NMI_ERR_HDR;
    }
    return u8Status;
}

uint8_t gatt_clear_attribute_req_handler(uint16_t attr_hdl)
{
    uint8_t u8Status, u8PktId;
    uint16_t u16MsgId, u16DestId, u16SrcId;
    INTERFACE_MSG_INIT(GATT_CLEAR_ATTRIBUTE_REQ, TASK_GATTM, NMI_TRUE);
    INTERFACE_PACK_ARG_UINT16(attr_hdl);
    INTERFACE_SEND_NO_WAIT();
    NMI_IF_UNPACK_HDR(&u8PktId, &u16MsgId, &u16DestId, &u16SrcId);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if ((u8PktId != INTERFACE_API_PKT_ID) || (u16MsgId != GATT_CLEAR_ATTRIBUTE_CMP_EVT) || (u16DestId != TASK_EXTERN) || (u16SrcId != TASK_GATTM))
    {
        return NMI_ERR_HDR;
    }
    return u8Status;
}

uint8_t gatt_read_attribute_req_handler(uint16_t attr_hdl, uint16_t *len, uint16_t *perm, uint8_t *uuid_len,
                                        uint8_t *uuid, uint8_t *value)
{
    uint8_t u8Status, u8PktId;
    uint16_t u16MsgId, u16DestId, u16SrcId;
    INTERFACE_MSG_INIT(GATT_READ_ATTRIBUTE_REQ, TASK_GATTM, NMI_TRUE);
    INTERFACE_PACK_ARG_UINT16(attr_hdl);
    INTERFACE_SEND_NO_WAIT();
    NMI_IF_UNPACK_HDR(&u8PktId, &u16MsgId, &u16DestId, &u16SrcId);
    INTERFACE_UNPACK_UINT16(&attr_hdl);
    INTERFACE_UNPACK_UINT16(len);
    INTERFACE_UNPACK_UINT16(perm);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_UNPACK_UINT8(uuid_len);
    NMI_IF_UNPACK_RES_BLOCK(uuid, *uuid_len);
    NMI_IF_UNPACK_RES_BLOCK(value, *len);
    INTERFACE_DONE();
    if ((u8PktId != INTERFACE_API_PKT_ID) || (u16MsgId != GATT_READ_ATTRIBUTE_CMP_EVT) || (u16DestId != TASK_EXTERN) || (u16SrcId != TASK_GATTM))
    {
        return NMI_ERR_HDR;
    }
    return u8Status;
}

void gatt_resource_access_resp_handler(uint16_t con_hdl)
{
    INTERFACE_MSG_INIT(GATT_RESOURCE_ACCESS_RSP, TASK_GATTM, NMI_FALSE);
    INTERFACE_PACK_ARG_UINT16(con_hdl);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}




void gatt_disc_svc_req_handler(uint8_t u8ReqType, uint16_t u16ConHdl, uint16_t u16StartHdl, uint16_t u16EndHdl,
                               uint16_t u16AttDesValSize, uint8_t *pu8AttDesVal)
{
    INTERFACE_MSG_INIT(GATT_DISC_SVC_REQ, TASK_GATTM, NMI_FALSE);
    INTERFACE_PACK_ARG_UINT8(u8ReqType);
    INTERFACE_PACK_ARG_UINT8(0); //padding
    INTERFACE_PACK_ARG_UINT16(u16ConHdl);
    INTERFACE_PACK_ARG_UINT16(u16StartHdl);
    INTERFACE_PACK_ARG_UINT16(u16EndHdl);
    INTERFACE_PACK_ARG_UINT16(u16AttDesValSize);
    //if(u16AttDesValSize != 0)
    //{
    //INTERFACE_PACK_ARG_BLOCK(pu8AttDesVal, u16AttDesValSize);
    //}
    //else
    //{
    INTERFACE_PACK_ARG_UINT16(0);
    //}
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void gatt_disc_char_req_handler(uint8_t u8ReqType, uint16_t u16ConHdl, uint16_t u16StartHdl, uint16_t u16EndHdl,
                                uint16_t u16AttDesValSize, uint8_t *pu8AttDesVal)
{
    INTERFACE_MSG_INIT(GATT_DISC_CHAR_REQ, TASK_GATTM, NMI_FALSE);
    INTERFACE_PACK_ARG_UINT8(u8ReqType);
    INTERFACE_PACK_ARG_UINT8(0); //padding
    INTERFACE_PACK_ARG_UINT16(u16ConHdl);
    INTERFACE_PACK_ARG_UINT16(u16StartHdl);
    INTERFACE_PACK_ARG_UINT16(u16EndHdl);
    INTERFACE_PACK_ARG_UINT16(u16AttDesValSize);
    //if(u16AttDesValSize != 0)
    //{
    //INTERFACE_PACK_ARG_BLOCK(pu8AttDesVal, u16AttDesValSize);
    //}
    //else
    //{
    INTERFACE_PACK_ARG_UINT16(0);
    //}
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}


void gatt_read_char_req_handler(uint8_t u8ReqType, uint16_t u16Offset, uint16_t u16ConHdl, uint16_t u16StartHdl,
                                uint16_t u16EndHdl, uint16_t u16NbOfUUID, uint8_t u8UUIDSize, uint8_t u8RespSize,
                                uint16_t u16UUID)
{
    INTERFACE_MSG_INIT(GATT_READ_CHAR_REQ, TASK_GATTM, NMI_FALSE);
    INTERFACE_PACK_ARG_UINT8(u8ReqType);
    INTERFACE_PACK_ARG_UINT8(0); //padding
    INTERFACE_PACK_ARG_UINT16(u16Offset);
    INTERFACE_PACK_ARG_UINT16(u16ConHdl);
    INTERFACE_PACK_ARG_UINT16(u16StartHdl);
    INTERFACE_PACK_ARG_UINT16(u16EndHdl);
    INTERFACE_PACK_ARG_UINT16(u16NbOfUUID);
    INTERFACE_PACK_ARG_UINT8(u8UUIDSize);
    INTERFACE_PACK_ARG_UINT8(u8RespSize);
    INTERFACE_PACK_ARG_UINT16(u16UUID);
    //if(u16AttDesValSize != 0)
    //{
    //INTERFACE_PACK_ARG_BLOCK(pu8AttDesVal, u16AttDesValSize);
    //}
    //else
    //{
    //}
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}


uint8_t gatt_write_char_req_handler(uint16_t u16ConHdl, uint16_t u16CharHdl, uint16_t u16WrOffset, uint16_t u16ValLen,
                                    uint8_t u8ReqType, uint8_t u8AutoExec, uint8_t *pu8Val)
{
    uint8_t u8Status, u8PktId;
    uint16_t u16MsgId, u16DestId, u16SrcId;
    INTERFACE_MSG_INIT(GATT_WRITE_CHAR_REQ, TASK_GATTM, NMI_TRUE);
    INTERFACE_PACK_ARG_UINT16(u16ConHdl);
    INTERFACE_PACK_ARG_UINT16(u16CharHdl);
    INTERFACE_PACK_ARG_UINT16(u16WrOffset);
    INTERFACE_PACK_ARG_UINT16(u16ValLen);
    INTERFACE_PACK_ARG_UINT8(u8ReqType);
    INTERFACE_PACK_ARG_UINT8(u8AutoExec);
    INTERFACE_PACK_ARG_BLOCK(pu8Val, u16ValLen);
    INTERFACE_SEND_NO_WAIT();
    NMI_IF_UNPACK_HDR(&u8PktId, &u16MsgId, &u16DestId, &u16SrcId);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    if ((u8PktId != INTERFACE_API_PKT_ID) || (u16MsgId != GATT_WRITE_CHAR_RESP) || (u16DestId != TASK_EXTERN) || (u16SrcId != TASK_GATTM))
    {
        return NMI_ERR_HDR;
    }
    return u8Status;
}

void gatt_resource_access_req_handler(uint16_t con_hdl)
{
    INTERFACE_MSG_INIT(GATT_RESOURCE_ACCESS_REQ, TASK_GATTM, NMI_FALSE);
    INTERFACE_PACK_ARG_UINT16(con_hdl);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}
#endif
