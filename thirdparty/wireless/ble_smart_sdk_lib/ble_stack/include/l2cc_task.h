#include "at_ble_api.h"
#include "cmn_defs.h"

extern uint8_t l2cap_avail_buffers;

void l2cc_pdu_send_req_handler(uint16_t conn_handle, uint16_t offset, uint16_t payload_length, uint16_t cid, uint8_t code, uint16_t len, uint8_t *data);
void l2cc_data_send_rsp_handler(uint8_t *data, at_ble_lecb_send_rsp_t *param);
void l2cc_data_received_handler(uint8_t *data, at_ble_lecb_data_recv_t *param);

/// Message API of the L2CC task
enum l2cc_msg_id
{
    /// Send a PDU packet
    L2CC_PDU_SEND_REQ = (TASK_L2CC << 8),
    /// Reception of a PDU packet
    L2CC_PDU_RECV_IND,
    /// Inform that packet been sent.
    L2CC_PDU_SEND_RSP,
    /// Inform that a lecb packet been received.
    L2CC_LECNX_DATA_RECV_IND,
    /// Indication of buffer status
    L2CC_PDU_BUFF_AVAIL,
};
