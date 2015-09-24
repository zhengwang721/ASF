#include "at_ble_api.h"
#include "htpt_task.h"
#include "cmn_defs.h"
#include "dbg_logs.h"


at_ble_status_t at_ble_htpt_create_db(at_ble_htpt_db_config_flag features,
                                      at_ble_htpt_temp_type  temp_type,
                                      uint16_t valid_range_min,
                                      uint16_t valid_range_max,
                                      uint16_t meas_intv,
                                      at_ble_htpt_sec_level sec_lvl,
                                      at_ble_handle_t *start_handle
                                     )
{
    at_ble_status_t status = AT_BLE_FAILURE;
    FN_IN();
    status =  htpt_create_db_req_handler(
                  features,
                  temp_type,
                  valid_range_min,
                  valid_range_max,
                  meas_intv,
                  sec_lvl,
                  start_handle);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_htpt_enable(at_ble_handle_t conn_handle,
                                   at_ble_htpt_ntf_ind_cfg ntf_ind_cfg
                                  )
{
    at_ble_status_t status = AT_BLE_FAILURE;
    FN_IN();
    status =  htpt_enable_req_handler(conn_handle, ntf_ind_cfg);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_htpt_temp_send(uint32_t temp,
                                      at_ble_prf_date_time_t *time_stamp,
                                      at_ble_htpt_temp_flags  flags,
                                      at_ble_htpt_temp_type  type,
                                      bool flag_stable_meas)
{
    at_ble_status_t status = AT_BLE_FAILURE;
    FN_IN();
    if (!time_stamp)
    {
        status = AT_BLE_INVALID_PARAM;
        goto EXIT;
    }
    status =  htpt_temp_send_req_handler(temp,
                                         time_stamp,
                                         flags,
                                         type,
                                         flag_stable_meas);
EXIT:
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_htpt_meas_intv_update(uint16_t meas_intv)
{
    at_ble_status_t status = AT_BLE_FAILURE;
    FN_IN();
    status =  htpt_meas_intv_upd_req_handler(meas_intv);
    FN_OUT(status);
    return status;
}


at_ble_status_t at_ble_htpt_meas_intv_chg_cfm(at_ble_handle_t conn_handle, at_ble_status_t status)
{
    at_ble_status_t ret = AT_BLE_FAILURE;
    FN_IN();
    ret = htpt_meas_intv_chg_cfm_handler((uint8_t)conn_handle, status);
    FN_OUT(ret);
    return ret;
}


