/*
 */
#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"

#include "l2cc_task.h"
#include "dbg_task.h"

#include "device.h"

// flow control variable, assume a max packet size of 1K and available
// heap for l2cap tx of size 10K
uint8_t l2cap_avail_buffers = 10;

void l2cc_pdu_send_req_handler(uint16_t conn_handle, uint16_t offset, uint16_t payload_length, uint16_t cid, uint8_t code, uint16_t len, uint8_t *data)
{
    INTERFACE_MSG_INIT(L2CC_PDU_SEND_REQ,  KE_BUILD_ID(TASK_L2CC, conn_handle));
    INTERFACE_PACK_ARG_UINT16(offset); //Offset value used by L2CAP during fragmentation (why 0 ?[Testing])
    INTERFACE_PACK_ARG_UINT16(payload_length); //payload length (why 0 ?[Testing])
    INTERFACE_PACK_ARG_UINT16(cid);
    INTERFACE_PACK_ARG_UINT8(code); //code=0 RFU
    INTERFACE_PACK_ARG_DUMMY(1);//padding
    INTERFACE_PACK_ARG_UINT16(len);
    INTERFACE_PACK_ARG_BLOCK(&data[0], len);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
}

void l2cc_data_send_rsp_handler(uint8_t *data, at_ble_lecb_send_rsp_t *param)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&param->status);
    INTERFACE_UNPACK_UINT16(&param->dest_cid);
    INTERFACE_UNPACK_UINT16(&param->dest_credit);
    INTERFACE_DONE();
    //increment available heap memory
    if (param->status == AT_BLE_SUCCESS)
    {
        l2cap_avail_buffers++;
    }
}

void l2cc_data_received_handler(uint8_t *data, at_ble_lecb_data_recv_t *param)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&param->src_cid);
    INTERFACE_UNPACK_UINT16(&param->src_credit);
    INTERFACE_UNPACK_UINT16(&param->len);
    INTERFACE_UNPACK_BLOCK(&param->data[0], param->len);
    INTERFACE_DONE();
}
