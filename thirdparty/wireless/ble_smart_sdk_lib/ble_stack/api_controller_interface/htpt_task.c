/*
 * htpt_task.c
 *
 *  Created on: Feb 22, 2015
 *      Author: aabdelfattah
 */
#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"

#include "htpt_task.h"
#include "gapm_task.h"
#include "profiles.h"


/*
 * msgs from APP to HTPT
 *
 *
 */


at_ble_status_t htpt_create_db_req_handler(at_ble_htpt_db_config_flag features,
        at_ble_htpt_temp_type  temp_type,
        uint16_t valid_range_min,
        uint16_t valid_range_max,
        uint16_t meas_intv,
        at_ble_htpt_sec_level sec_lvl,
        at_ble_handle_t *start_handle)
{
    //TODO: check on platform error
    at_ble_status_t status = AT_BLE_SUCCESS;
    at_ble_htpt_db_cfg params;
    params.features = features;
    params.temp_type = temp_type;
    params.valid_range_max = valid_range_max;
    params.valid_range_min = valid_range_min;
    params.meas_intv = meas_intv;
    status = gapm_profile_task_add_cmd_handler(sec_lvl,
             TASK_HTPT,
             TASK_GTL,
             0, //need to keep track of start handle ?
             &params,
             sizeof(at_ble_htpt_db_cfg),
             start_handle
                                              );
    return status;
}


at_ble_status_t htpt_enable_req_handler(at_ble_handle_t conn_handle,
                                        at_ble_htpt_ntf_ind_cfg ntf_ind_cfg)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    INTERFACE_MSG_INIT(HTPT_ENABLE_REQ, TASK_HTPT);
    INTERFACE_PACK_ARG_UINT8((uint8_t)conn_handle);
    INTERFACE_PACK_ARG_UINT8(ntf_ind_cfg);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return status;
}


at_ble_status_t htpt_temp_send_req_handler(uint32_t temp,
        at_ble_prf_date_time_t *time_stamp,
        uint8_t  flags,
        uint8_t  type,
        uint8_t flag_stable_meas)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    INTERFACE_MSG_INIT(HTPT_TEMP_SEND_REQ, TASK_HTPT);
    INTERFACE_PACK_ARG_UINT32(temp);
    INTERFACE_PACK_ARG_UINT16(time_stamp->year);
    INTERFACE_PACK_ARG_UINT8(time_stamp->month);
    INTERFACE_PACK_ARG_UINT8(time_stamp->day);
    INTERFACE_PACK_ARG_UINT8(time_stamp->hour);
    INTERFACE_PACK_ARG_UINT8(time_stamp->min);
    INTERFACE_PACK_ARG_UINT8(time_stamp->sec);
    INTERFACE_PACK_ARG_DUMMY(1);
    INTERFACE_PACK_ARG_UINT8(flags);
    INTERFACE_PACK_ARG_UINT8(type);
    INTERFACE_PACK_ARG_DUMMY(2);
    INTERFACE_PACK_ARG_UINT8(flag_stable_meas);
    INTERFACE_PACK_ARG_DUMMY(3);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return status;
}

at_ble_status_t htpt_meas_intv_upd_req_handler(uint16_t meas_intv)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    INTERFACE_MSG_INIT(HTPT_MEAS_INTV_UPD_REQ, TASK_HTPT);
    INTERFACE_PACK_ARG_UINT16(meas_intv);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return status;
}

at_ble_status_t htpt_meas_intv_chg_cfm_handler(uint8_t handle, at_ble_status_t status)
{
    at_ble_status_t ret = AT_BLE_SUCCESS;
    INTERFACE_MSG_INIT(HTPT_MEAS_INTV_CHG_CFM, TASK_HTPT);
    INTERFACE_PACK_ARG_UINT8(handle);
    INTERFACE_PACK_ARG_UINT8((uint8_t)status & 0xFF);
    INTERFACE_SEND_NO_WAIT();
    INTERFACE_DONE();
    return ret;
}



/*
 * events from HTPT to APP
 *
 *
 */


void htpt_enable_rsp_handler(uint16_t src, uint8_t *data, at_ble_htpt_enable_rsp_t *params)
{
    uint8_t status = AT_BLE_FAILURE;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&params->conhdl);
    INTERFACE_UNPACK_UINT8(&status);
    INTERFACE_DONE();
    params->status = (at_ble_status_t)status;
}
void htpt_temp_send_rsp_handler(uint16_t src, uint8_t *data, at_ble_htpt_temp_send_cfm_t *params)
{
    uint8_t status = AT_BLE_FAILURE;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&status);
    INTERFACE_DONE();
    params->status = (at_ble_status_t)status;
}

void htpt_meas_intv_upd_rsp_handler(uint16_t src, uint8_t *data, at_ble_htpt_meas_intv_upd_rsp_t *params)
{
    uint8_t status = AT_BLE_FAILURE;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&status);
    INTERFACE_DONE();
    params->status = (at_ble_status_t)status;
}

void htpt_meas_intv_chg_req_ind_handler(uint16_t src_id, uint8_t *data, at_ble_htpt_meas_intv_chg_req_t *params)
{
    uint16_t dummy_b;
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT16(&dummy_b);
    INTERFACE_UNPACK_UINT16(&params->intv);
    INTERFACE_DONE();
}

void htpt_cfg_indntf_ind_handler(uint16_t src, uint8_t *data, at_ble_htpt_cfg_indntf_ind_t *params)
{
    INTERFACE_UNPACK_INIT(data);
    INTERFACE_UNPACK_UINT8(&params->conhdl);
    INTERFACE_UNPACK_UINT8(&params->ntf_ind_cfg);
    INTERFACE_DONE();
}

