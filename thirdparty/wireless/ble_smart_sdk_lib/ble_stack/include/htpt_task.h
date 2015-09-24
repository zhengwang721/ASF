/*
 * htpt_task.h
 *
 *  Created on: Feb 23, 2015
 *      Author: aabdelfattah
 *
 */
#ifndef HTPT_TASK_H_
#define HTPT_TASK_H_

#include "cmn_defs.h"
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
        at_ble_handle_t *start_handle);

at_ble_status_t htpt_enable_req_handler(at_ble_handle_t conn_handle,
                                        at_ble_htpt_ntf_ind_cfg ntf_ind_cfg);


at_ble_status_t htpt_temp_send_req_handler(uint32_t temp,
        at_ble_prf_date_time_t *time_stamp,
        uint8_t  flags,
        uint8_t  type,
        uint8_t flag_stable_meas);



at_ble_status_t htpt_meas_intv_upd_req_handler(uint16_t meas_intv);

at_ble_status_t htpt_meas_intv_chg_cfm_handler(uint8_t handle, at_ble_status_t status);

at_ble_status_t htpt_temp_type_upd_req_handler(at_ble_handle_t conn_handle,
        uint8_t value);

/*
 * events from HTPT to APP
 *
 *
 */
void htpt_enable_rsp_handler(uint16_t src, uint8_t *data, at_ble_htpt_enable_rsp_t *params);
void htpt_temp_send_rsp_handler(uint16_t src, uint8_t *data, at_ble_htpt_temp_send_cfm_t *params);
void htpt_meas_intv_upd_rsp_handler(uint16_t src, uint8_t *data, at_ble_htpt_meas_intv_upd_rsp_t *params);
void htpt_meas_intv_chg_req_ind_handler(uint16_t src_id, uint8_t *data, at_ble_htpt_meas_intv_chg_req_t *params);

void htpt_cfg_indntf_ind_handler(uint16_t src, uint8_t *data, at_ble_htpt_cfg_indntf_ind_t *params);

/// Messages for Health Thermometer Profile Thermometer
enum htpt_msg_id
{
    /// Start the Health Thermometer Profile Thermometer profile - at connection
    HTPT_ENABLE_REQ = (uint16_t)((TASK_HTPT) << 8),
    /// Enable confirmation
    HTPT_ENABLE_RSP,

    /// Send temperature value from APP
    HTPT_TEMP_SEND_REQ,
    /// Send temperature response
    HTPT_TEMP_SEND_RSP,

    /// Indicate Measurement Interval
    HTPT_MEAS_INTV_UPD_REQ,
    /// Send Measurement Interval response
    HTPT_MEAS_INTV_UPD_RSP,

    /// Inform APP of new measurement interval value requested by a peer device
    HTPT_MEAS_INTV_CHG_REQ_IND,
    /// APP Confirm message of new measurement interval value requested by a peer device
    /// If accepted, it triggers indication on measurement interval attribute
    HTPT_MEAS_INTV_CHG_CFM,

    /// Inform APP that Indication Configuration has been changed - use to update bond data
    HTPT_CFG_INDNTF_IND,
};



#endif // HTPT_TASK_H_
