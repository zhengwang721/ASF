
#include "at_ble_api.h"

#include "platform.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "dbg_task.h"
#include "l2cc_task.h"

#include "device.h"
#include "dbg_logs.h"


at_ble_status_t at_ble_l2cap_cid_register(at_ble_handle_t conn_handle, uint16_t cid);
at_ble_status_t at_ble_l2cap_cid_unregister(at_ble_handle_t conn_handle, uint16_t cid);
at_ble_status_t at_ble_l2cap_tx(at_ble_handle_t conn_handle, uint16_t cid, uint8_t *data, uint16_t len);

at_ble_status_t at_ble_l2cap_cid_register(at_ble_handle_t conn_handle, uint16_t cid)
{
    return AT_BLE_FAILURE;
}

at_ble_status_t at_ble_l2cap_cid_unregister(at_ble_handle_t conn_handle, uint16_t cid)
{
    return AT_BLE_FAILURE;
}

at_ble_status_t at_ble_l2cap_tx(at_ble_handle_t conn_handle, uint16_t cid, uint8_t *data, uint16_t len)
{
    return AT_BLE_FAILURE;
}

at_ble_status_t at_ble_lecb_create(at_ble_handle_t conn_handle, at_ble_lecb_sec_level_t sec_level, uint16_t le_psm, uint16_t cid,
                                   uint16_t initial_credit)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    status =  gapc_lecb_create_handler(conn_handle , sec_level, le_psm, cid, initial_credit);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_lecb_destroy(at_ble_handle_t conn_handle, uint16_t le_psm)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    status = gapc_lecb_destroy_handler(conn_handle, le_psm);
    FN_OUT(status);
    return status;
}


at_ble_status_t at_ble_lecb_connect(at_ble_handle_t conn_handle, uint16_t le_psm, uint16_t cid, uint16_t intial_credit)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    gapc_lecb_connect_cmd_handler(conn_handle, le_psm, cid, intial_credit);
    FN_OUT(status);
    return status;
}


at_ble_status_t at_ble_lecb_cfm(at_ble_handle_t conn_handle, uint16_t le_psm, at_ble_lecb_status status)
{
    at_ble_status_t ret_status = AT_BLE_SUCCESS;
    FN_IN();
    ret_status = gapc_lecb_connect_cfm_handler(conn_handle, le_psm, status);
    FN_OUT(ret_status);
    return ret_status;
}

at_ble_status_t at_ble_lecb_disconnect(at_ble_handle_t conn_handle, uint16_t le_psm)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    gapc_lecb_disconnect_cmd_handler(conn_handle, le_psm);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_lecb_add_credit(at_ble_handle_t conn_handle, uint16_t le_psm, uint16_t credit)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    status = gapc_lecb_add_cmd_handler(conn_handle, le_psm, credit);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_lecb_send(at_ble_handle_t conn_handle, uint16_t cid, uint16_t len, uint8_t *data)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    if (data == 0)
    {
        status = AT_BLE_INVALID_PARAM;
        PRINT_ERR("Null data ptr. \n");
        goto EXIT;
    }
    if (len > AT_BLE_LECB_MAX_PKT_SIZE)
    {
        status = AT_BLE_INVALID_PARAM;
        PRINT_ERR("Invalid LECB packet size. \n");
        goto EXIT;
    }
    //decrement available heap memory , return to user if no available buffers
    l2cap_avail_buffers--;
    if (l2cap_avail_buffers < 0)
    {
        status = AT_BLE_BUSY;
        PRINT_ERR("No available buffers in heap , chip busy. \n");
        goto EXIT;
    }
    l2cc_pdu_send_req_handler(conn_handle, 0, 0, cid, 0, len, data);
EXIT:
    FN_OUT(status);
    return status;
}
