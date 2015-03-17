/*
 * gattc_task.c
 *
 *  Created on: Feb 22, 2015
 *      Author: aabdelfattah
 */
#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"

#include "htpt_task.h"
//#include "profiles.h"

/*
 * msgs from APP to HTPT
 *
 *
 */
at_ble_status_t htpt_create_db_req_handler(uint8_t  temp_type, 
											uint16_t valid_range_min,
											uint16_t valid_range_max,
											uint8_t features)
{
	//TODO: check on platform error
	at_ble_status_t status = AT_BLE_SUCCESS;
	INTERFACE_MSG_INIT(HTPT_CREATE_DB_REQ, TASK_HTPT);
	INTERFACE_PACK_ARG_UINT16(valid_range_min);
	INTERFACE_PACK_ARG_UINT16(valid_range_max);
	INTERFACE_PACK_ARG_UINT8(features);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
	return status;

}

at_ble_status_t htpt_enable_req_handler(  at_ble_handle_t conn_handle,
										uint8_t sec_lvl,
										uint8_t con_type,
										uint16_t temp_meas_ind_en,
										uint16_t interm_temp_ntf_en,
										uint16_t meas_intv_ind_en,
										uint16_t meas_intv)
{
	//TODO: check on platform error
	at_ble_status_t status = AT_BLE_SUCCESS;
	INTERFACE_MSG_INIT(HTPT_ENABLE_REQ, TASK_HTPT);
	INTERFACE_PACK_ARG_UINT16(conn_handle);
	INTERFACE_PACK_ARG_UINT8(sec_lvl);
	INTERFACE_PACK_ARG_UINT8(con_type);
	INTERFACE_PACK_ARG_UINT16(temp_meas_ind_en);
	INTERFACE_PACK_ARG_UINT16(interm_temp_ntf_en);
	INTERFACE_PACK_ARG_UINT16(meas_intv_ind_en);
	INTERFACE_PACK_ARG_UINT16(meas_intv);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
	return status;							   
}

//used to pack the input 
typedef struct 
{
    ///Temp
    uint32_t temp;
    ///Time stamp
    struct prf_date_time time_stamp;
    ///Flag
    uint8_t  flags;
    ///Type
    uint8_t  type;
}htp_temp_meas;
typedef struct
{
    ///Temperature Measurement
    htp_temp_meas temp_meas;
    ///Connection handle
    uint16_t conhdl;
    ///Stable or intermediary type of temperature
    uint8_t flag_stable_meas;
} htpt_temp_send_req;



at_ble_status_t htpt_temp_send_req_handler(		uint32_t temp,
												struct prf_date_time* time_stamp,
												uint8_t  flags,
												uint8_t  type,
												at_ble_handle_t conn_handle,
												uint8_t flag_stable_meas)
{
	//TODO: check on platform error
	at_ble_status_t status = AT_BLE_SUCCESS;
	htpt_temp_send_req temp_send_pack = {0};
	INTERFACE_MSG_INIT(HTPT_TEMP_SEND_REQ, TASK_HTPT);
	temp_send_pack.conhdl=conn_handle;
	temp_send_pack.flag_stable_meas = flag_stable_meas;
	temp_send_pack.temp_meas.flags = flags;
	temp_send_pack.temp_meas.temp = temp;
	temp_send_pack.temp_meas.type = type;
	
	INTERFACE_PACK_ARG_BLOCK(&temp_send_pack,sizeof(htpt_temp_send_req));
	/*INTERFACE_PACK_ARG_UINT32(temp);
	INTERFACE_PACK_ARG_BLOCK(time_stamp,7);
	INTERFACE_PACK_ARG_UINT8(flags);
	INTERFACE_PACK_ARG_UINT8(type);
	INTERFACE_PACK_ARG_UINT16(conn_handle);
	INTERFACE_PACK_ARG_UINT8(flag_stable_meas);*/
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
	return status;
}

at_ble_status_t htpt_meas_intv_upd_req_handler(at_ble_handle_t conn_handle,
										uint16_t meas_intv)
{
	//TODO: check on platform error
	at_ble_status_t status = AT_BLE_SUCCESS;
	INTERFACE_MSG_INIT(HTPT_MEAS_INTV_UPD_REQ, TASK_HTPT);
	INTERFACE_PACK_ARG_UINT16(conn_handle);
	INTERFACE_PACK_ARG_UINT16(meas_intv);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
	return status;
}

at_ble_status_t htpt_temp_type_upd_req_handler( at_ble_handle_t conn_handle,
											   uint8_t value)
{
	//TODO: check on platform error
	at_ble_status_t status = AT_BLE_SUCCESS;
	INTERFACE_MSG_INIT(HTPT_TEMP_TYPE_UPD_REQ, TASK_HTPT);
	INTERFACE_PACK_ARG_UINT8(value);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
	return status;
}



/*
 * events from HTPT to APP
 *
 *
 */
void htpt_disable_ind_handler(uint16_t src,uint8_t *data,at_ble_htpt_disable_ind_t *param)
{
	//param->conn_handle = KE_IDX_GET(src);

	INTERFACE_UNPACK_INIT(data);
	INTERFACE_UNPACK_UINT16(&param->conhdl);
	INTERFACE_UNPACK_UINT16(&param->temp_meas_ind_en);
	INTERFACE_UNPACK_UINT16(&param->interm_temp_ntf_en);
	INTERFACE_UNPACK_UINT16(&param->meas_intv_ind_en);
	INTERFACE_UNPACK_UINT16(&param->meas_intv);
	INTERFACE_DONE();

}

void htpt_error_ind_handler(uint16_t src,uint8_t *data,struct prf_server_error_ind  *param)
{
	//param->conn_handle = KE_IDX_GET(src);

	INTERFACE_UNPACK_INIT(data);
	INTERFACE_UNPACK_UINT16(&param->conhdl);
	INTERFACE_UNPACK_UINT16(&param->msg_id);
	INTERFACE_UNPACK_UINT8(&param->status);
	INTERFACE_DONE();

}

void htpt_temp_send_cfm_handler(uint16_t src,uint8_t *data, at_ble_htpt_temp_send_cfm_t *param)
{
	//param->conn_handle = KE_IDX_GET(src);

	INTERFACE_UNPACK_INIT(data);
	INTERFACE_UNPACK_UINT16(&param->conhdl);
	INTERFACE_UNPACK_UINT8(&param->status);
	INTERFACE_UNPACK_UINT8(&param->cfm_type);
	INTERFACE_DONE();

}

void htpt_create_db_cfm_handler(uint16_t src,uint8_t *data, at_ble_htpt_create_db_cfm_t *param)
{
	//param->conn_handle = KE_IDX_GET(src);

	INTERFACE_UNPACK_INIT(data);
	INTERFACE_UNPACK_UINT8(&param->status);
	INTERFACE_DONE();

}

void htpt_meas_intv_chg_ind_handler(uint16_t src,uint8_t *data,at_ble_htpt_meas_intv_chg_ind_t *param)
{
	//param->conn_handle = KE_IDX_GET(src);

	INTERFACE_UNPACK_INIT(data);
	INTERFACE_UNPACK_UINT16(&param->intv);
	INTERFACE_DONE();
}

void htpt_cfg_indntf_ind_handler(uint16_t src,uint8_t *data,at_ble_htpt_cfg_indntf_ind_t *param)
{
	//param->conn_handle = KE_IDX_GET(src);

	INTERFACE_UNPACK_INIT(data);
	INTERFACE_UNPACK_UINT16(&param->conhdl);
	INTERFACE_UNPACK_UINT16(&param->cfg_val);
	INTERFACE_UNPACK_UINT8(&param->char_code);
	INTERFACE_DONE();
}

