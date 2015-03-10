/*
 * gap_task.c
 *
 *  Created on: Sep 16, 2013
 *      Author: delsissy
 */

#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"

#include "gapm_task.h"
#include "dbg_task.h"
#include "gapc_task.h"

#include "device.h"

// original location : llm.h
enum
{
    LLM_ADV_CONN_UNDIR = 0x00,
    LLM_ADV_CONN_DIR,
    LLM_ADV_NONCONN_UNDIR,
    LLM_SCAN_REQ,
    LLM_SCAN_RSP,
    LLM_CONNECT_REQ,
    LLM_ADV_DISC_UNDIR,
    LLM_ADV_END
};

uint8_t gapm_reset_req_handler (void)
{
	uint8_t u8Operation, u8Status;

	INTERFACE_MSG_INIT(GAPM_RESET_CMD, TASK_GAPM);
	INTERFACE_PACK_ARG_UINT8(GAPM_RESET);
	INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
	INTERFACE_UNPACK_UINT8(&u8Operation);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();
	if(u8Operation!=GAPM_RESET)
		return AT_BLE_FAILURE;
	return u8Status;
}
uint8_t gapm_set_dev_name_handler(uint8_t len, uint8_t* name)
{

	uint8_t u8Operation, u8Status;

	INTERFACE_MSG_INIT(GAPM_SET_DEV_NAME_CMD, TASK_GAPM);
	INTERFACE_PACK_ARG_UINT8(GAPM_SET_DEV_NAME);
	INTERFACE_PACK_ARG_UINT8(len);
	INTERFACE_PACK_ARG_BLOCK(name, len);
	INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
	INTERFACE_UNPACK_UINT8(&u8Operation);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();
	if(u8Operation!=GAPM_SET_DEV_NAME)
		return AT_BLE_FAILURE;
	return u8Status;
}

void gapm_start_adv_cmd_handler (uint8_t u8OpCode, uint8_t u8AddrSrc, uint16_t u16RenewDur, 
		uint8_t *pu8BdAddr, uint16_t u16MinIntv, uint16_t u16MaxIntv, uint8_t u8ChnlMap, uint8_t u8Mode ,
	uint8_t u8AdvFiltPolicy, uint8_t u8AdvDataLen, uint8_t *pu8AdvData, uint8_t u8ScnRespLen, uint8_t *pu8ScnRespData)
{
	INTERFACE_MSG_INIT(GAPM_START_ADVERTISE_CMD, TASK_GAPM);
	INTERFACE_PACK_ARG_UINT8(u8OpCode);
	INTERFACE_PACK_ARG_UINT8(u8AddrSrc);
	// Dummy data use to retrieve internal operation state (should be set to 0).
	INTERFACE_PACK_ARG_UINT16(0);
	// To do check renew address duration
	INTERFACE_PACK_ARG_UINT16(u16RenewDur);
	if(pu8BdAddr != NULL)
	{
		INTERFACE_PACK_ARG_BLOCK(pu8BdAddr, AT_BLE_ADDR_LEN);
	}
	else
	{
		INTERFACE_PACK_ARG_DUMMY(AT_BLE_ADDR_LEN);
	}
	INTERFACE_PACK_ARG_UINT16(u16MinIntv);
	INTERFACE_PACK_ARG_UINT16(u16MaxIntv);
	INTERFACE_PACK_ARG_UINT8(u8ChnlMap);
	INTERFACE_PACK_ARG_UINT8(u8Mode);
	INTERFACE_PACK_ARG_UINT8(u8AdvFiltPolicy);
	// advertising data
	INTERFACE_PACK_ARG_UINT8(u8AdvDataLen);
	INTERFACE_PACK_ARG_BLOCK(pu8AdvData, u8AdvDataLen);
	INTERFACE_PACK_ARG_DUMMY((ADV_DATA_MAX_SIZE - u8AdvDataLen));
	// scan response
	INTERFACE_PACK_ARG_UINT8(u8ScnRespLen);
	INTERFACE_PACK_ARG_BLOCK(pu8ScnRespData, u8ScnRespLen);
	INTERFACE_PACK_ARG_DUMMY((SCAN_RESP_DATA_MAX_SIZE - u8ScnRespLen));
		
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();

}

uint8_t gapm_set_dev_config_cmd_handler(uint8_t u8Role, uint8_t *pu8Key, 
				uint16_t u16Appearance, uint8_t u8AppWrPerm, uint8_t u8NameWrPerm, uint16_t u16MaxMTU,
				uint16_t u16ConIntMin, uint16_t u16ConIntMax, uint16_t u16ConLatency, 
				uint16_t u16SupervTo, uint8_t u8Flags)
{
	uint8_t u8Operation, u8Status;

	INTERFACE_MSG_INIT(GAPM_SET_DEV_CONFIG_CMD, TASK_GAPM);
	INTERFACE_PACK_ARG_UINT8(GAPM_SET_DEV_CONFIG);
	INTERFACE_PACK_ARG_UINT8(u8Role);
	INTERFACE_PACK_ARG_BLOCK(pu8Key, KEY_LEN);
	INTERFACE_PACK_ARG_UINT16(u16Appearance);
	INTERFACE_PACK_ARG_UINT8(u8AppWrPerm);
	INTERFACE_PACK_ARG_UINT8(u8NameWrPerm);
	INTERFACE_PACK_ARG_UINT16(u16MaxMTU);
	if(u8Role == GAP_PERIPHERAL_SLV)
	{
		INTERFACE_PACK_ARG_UINT16(u16ConIntMin);
		INTERFACE_PACK_ARG_UINT16(u16ConIntMax);
		INTERFACE_PACK_ARG_UINT16(u16ConLatency);
		INTERFACE_PACK_ARG_UINT16(u16SupervTo);
		INTERFACE_PACK_ARG_UINT8(u8Flags);
	}
	INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
	INTERFACE_UNPACK_UINT8(&u8Operation);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();
	if(u8Operation!=GAPM_SET_DEV_CONFIG)
		return AT_BLE_FAILURE;
	return u8Status;
}

void gapm_start_scan_cmd_handler (uint8_t u8OpCode, uint8_t u8AddrType, uint16_t u16RenewDur, 
		uint8_t *pu8BdAddr, uint16_t u16ScanInterval, uint16_t u16ScanWin, uint8_t u8ScanMode, uint8_t u8FiltPolicy,
		uint8_t u8FilterDuplic)
{
	INTERFACE_MSG_INIT(GAPM_START_SCAN_CMD, TASK_GAPM);
	INTERFACE_PACK_ARG_UINT8(u8OpCode);
	INTERFACE_PACK_ARG_UINT8(u8AddrType);
	INTERFACE_PACK_ARG_UINT16(0);
	INTERFACE_PACK_ARG_UINT16(u16RenewDur);
	INTERFACE_PACK_ARG_BLOCK(pu8BdAddr, BD_ADDR_LEN);
	INTERFACE_PACK_ARG_UINT16(u16ScanInterval);
	INTERFACE_PACK_ARG_UINT16(u16ScanWin);
	INTERFACE_PACK_ARG_UINT8(u8ScanMode);
	INTERFACE_PACK_ARG_UINT8(u8FiltPolicy);
	INTERFACE_PACK_ARG_UINT8(u8FilterDuplic);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
}

void gapm_start_connection_cmd_handler(uint8_t u8OpCode, uint8_t u8AddrType, uint16_t u16RenewDur,
			uint8_t *pu8BdAddr, uint16_t u16ScanInterval, uint16_t u16ScanWin, uint16_t u16ConIntvMin, 
			uint16_t u16ConIntvMax, uint16_t u16ConLatency, uint16_t u16SupervTO, uint16_t u16CeMin, 
			uint16_t u16CeMAx, uint8_t u8NbOfPeers, at_ble_addr_t *peers)
{

	uint8_t count = 0;
	INTERFACE_MSG_INIT(GAPM_START_CONNECTION_CMD, TASK_GAPM);
	INTERFACE_PACK_ARG_UINT8(u8OpCode);
	INTERFACE_PACK_ARG_UINT8(u8AddrType);
	INTERFACE_PACK_ARG_UINT16(0);
	INTERFACE_PACK_ARG_UINT16(u16RenewDur);
	INTERFACE_PACK_ARG_BLOCK(pu8BdAddr, BD_ADDR_LEN);
	INTERFACE_PACK_ARG_UINT16(u16ScanInterval);
	INTERFACE_PACK_ARG_UINT16(u16ScanWin);
	INTERFACE_PACK_ARG_UINT16(u16ConIntvMin);
	INTERFACE_PACK_ARG_UINT16(u16ConIntvMax);
	INTERFACE_PACK_ARG_UINT16(u16ConLatency);
	INTERFACE_PACK_ARG_UINT16(u16SupervTO);
	INTERFACE_PACK_ARG_UINT16(u16CeMin);
	INTERFACE_PACK_ARG_UINT16(u16CeMAx);
	INTERFACE_PACK_ARG_UINT8(u8NbOfPeers);
	for(count = 0; count < u8NbOfPeers; count++)
	{
		INTERFACE_PACK_ARG_BLOCK((peers->addr), BD_ADDR_LEN);
		INTERFACE_PACK_ARG_UINT8(peers->type);
		peers +=1;
	}
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
}

uint8_t gapm_cancel_cmd_handler (void)
{
	uint8_t u8Status = 0;

	INTERFACE_MSG_INIT(GAPM_CANCEL_CMD, TASK_GAPM);
	INTERFACE_PACK_ARG_UINT8(GAPM_CANCEL);
	INTERFACE_SEND_NO_WAIT();
	//INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
	//INTERFACE_UNPACK_UINT8(&u8Operation);
	//INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();
	//if(u8Operation!=GAPM_CANCEL_CMD)
		//return AT_BLE_FAILURE;
	return u8Status;
}

void gapm_connection_cfm_handler ( uint8_t *pu8PeerBdAddr, uint8_t u8PeerAddrType,uint16_t u16ConIntvMin, 
			uint16_t u16ConIntvMax, uint16_t u16ConLatency, uint16_t u16SupervTO, uint16_t u16CeMin, 
			uint16_t u16CeMAx)
{
	INTERFACE_MSG_INIT(GAPM_CONNECTION_CFM, TASK_GAPM);
	INTERFACE_PACK_ARG_BLOCK(pu8PeerBdAddr, BD_ADDR_LEN);
	INTERFACE_PACK_ARG_UINT8(u8PeerAddrType);
	INTERFACE_PACK_ARG_UINT8(0);//zero padding
	INTERFACE_PACK_ARG_UINT16(u16ConIntvMin);
	INTERFACE_PACK_ARG_UINT16(u16ConIntvMax);
	INTERFACE_PACK_ARG_UINT16(u16ConLatency);
	INTERFACE_PACK_ARG_UINT16(u16SupervTO);
	INTERFACE_PACK_ARG_UINT16(u16CeMin);
	INTERFACE_PACK_ARG_UINT16(u16CeMAx);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
		
}
#if 0

uint8_t gap_adv_req_handler (NMI_Bool adv_en)
{
	uint8_t u8Status, u8PktId;
	uint16_t u16MsgId, u16DestId, u16SrcId;


	INTERFACE_MSG_INIT(GAP_ADV_REQ, TASK_GAP, NMI_TRUE);
	INTERFACE_PACK_ARG_UINT8(adv_en);
	INTERFACE_SEND_NO_WAIT();
	NMI_IF_UNPACK_HDR(&u8PktId, &u16MsgId, &u16DestId, &u16SrcId);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();
	if((u8PktId!=INTERFACE_API_PKT_ID)||(u16MsgId!=GAP_ADV_REQ_CMP_EVT)||(u16DestId!=TASK_EXTERN)||(u16SrcId!=TASK_GAP))
			return NMI_ERR_HDR;
	return u8Status;
}
#endif
//#define GAP_INQ_SCAN_INTV                                   0x0012
//#define GAP_INQ_SCAN_WIND                                   0x0012
#if 0
uint8_t gap_set_mode_req_handler (uint16_t mode, uint16_t adv_intv_min, uint16_t adv_intv_max, uint8_t adv_type,
		uint8_t own_addr_type, uint8_t direct_addr_type, uint8_t *direct_addr, uint8_t adv_chnl_map, uint8_t adv_filt_policy,
		uint8_t adv_data_len, uint8_t *adv_data, uint8_t scan_rsp_data_len, uint8_t	*scan_rsp_data, uint8_t u8ScanType, 
		uint16_t u16ScanIntv, uint16_t u16ScanWind, uint8_t u8OwnAddrType, uint8_t u8ScanFiltPolicy)
{

	uint8_t u8Status, u8PktId;
	uint16_t u16MsgId, u16DestId, u16SrcId, total_adv_data_len;

	INTERFACE_MSG_INIT(GAP_SET_MODE_REQ, TASK_GAP, NMI_TRUE);
	INTERFACE_PACK_ARG_UINT16(mode);
	if(mode!= GAP_OBSERVER)
	{	
	INTERFACE_PACK_ARG_UINT16(adv_intv_min);
	INTERFACE_PACK_ARG_UINT16(adv_intv_max);
	INTERFACE_PACK_ARG_UINT8(adv_type);
	INTERFACE_PACK_ARG_UINT8(own_addr_type);
	INTERFACE_PACK_ARG_UINT8(direct_addr_type);
	INTERFACE_PACK_ARG_BLOCK(direct_addr, BD_ADDR_LEN);
	INTERFACE_PACK_ARG_UINT8(adv_chnl_map);
	INTERFACE_PACK_ARG_UINT8(adv_filt_policy);
	INTERFACE_PACK_ARG_UINT8(0);

	if ((ADV_DATA_MAX_SIZE - adv_data_len) > sizeof(APP_ADV_DEV_NAME))
	{
		total_adv_data_len = adv_data_len + APP_ADV_DEV_NAME_LEN + 2;
		INTERFACE_PACK_ARG_UINT8(total_adv_data_len);
		INTERFACE_PACK_ARG_BLOCK(adv_data, adv_data_len);
		INTERFACE_PACK_ARG_UINT8(APP_ADV_DEV_NAME_LEN + 1);
		INTERFACE_PACK_ARG_UINT8(AD_TYPE_NAME);
		INTERFACE_PACK_ARG_BLOCK(APP_ADV_DEV_NAME, APP_ADV_DEV_NAME_LEN);
	}
	else
	{
		total_adv_data_len = adv_data_len;
		INTERFACE_PACK_ARG_UINT8(total_adv_data_len);
		INTERFACE_PACK_ARG_BLOCK(adv_data, adv_data_len);
	}

	if(total_adv_data_len != ADV_DATA_MAX_SIZE)
		INTERFACE_PACK_ARG_BLOCK(APP_ZERO_PAD_DATA, (ADV_DATA_MAX_SIZE - total_adv_data_len));

	INTERFACE_PACK_ARG_UINT8(scan_rsp_data_len);
	INTERFACE_PACK_ARG_BLOCK(scan_rsp_data, scan_rsp_data_len);
	if(scan_rsp_data_len != SCAN_RESP_DATA_MAX_SIZE)
		INTERFACE_PACK_ARG_BLOCK(APP_ZERO_PAD_DATA, (SCAN_RESP_DATA_MAX_SIZE - scan_rsp_data_len));
	INTERFACE_SEND_NO_WAIT();
	}
	else{
		INTERFACE_PACK_ARG_BLOCK(APP_ZERO_PAD_DATA, 16);
		INTERFACE_PACK_ARG_BLOCK(APP_ZERO_PAD_DATA, ADV_DATA_MAX_SIZE + 1);
		INTERFACE_PACK_ARG_BLOCK(APP_ZERO_PAD_DATA, SCAN_RESP_DATA_MAX_SIZE + 1);
		INTERFACE_PACK_ARG_UINT8(u8ScanType);
		INTERFACE_PACK_ARG_UINT8(0);//padding
		INTERFACE_PACK_ARG_UINT16(u16ScanIntv);
		INTERFACE_PACK_ARG_UINT16(u16ScanWind);
		INTERFACE_PACK_ARG_UINT8(u8OwnAddrType);
		INTERFACE_PACK_ARG_UINT8(u8ScanFiltPolicy);
		INTERFACE_SEND_NO_WAIT();
	}
	NMI_IF_UNPACK_HDR(&u8PktId, &u16MsgId, &u16DestId, &u16SrcId);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();
	if((u8PktId!=INTERFACE_API_PKT_ID)||(u16MsgId!=GAP_SET_MODE_REQ_CMP_EVT)||(u16DestId!=TASK_EXTERN)||(u16SrcId!=TASK_GAP))
		return NMI_ERR_HDR;
	return u8Status;
}

void gap_bond_resp_handler(uint16_t conhdl, uint8_t accept, uint8_t io_cap, uint8_t oob, uint8_t auth_req, uint8_t max_key_size,
		uint8_t ikeys, uint8_t rkeys)
{

	INTERFACE_MSG_INIT(GAP_BOND_RESP, TASK_GAP, NMI_FALSE);
	INTERFACE_PACK_ARG_UINT16(conhdl);
	INTERFACE_PACK_ARG_UINT8(accept);
	INTERFACE_PACK_ARG_UINT8(io_cap);
	INTERFACE_PACK_ARG_UINT8(oob);
	INTERFACE_PACK_ARG_UINT8(auth_req);
	INTERFACE_PACK_ARG_UINT8(max_key_size);
	INTERFACE_PACK_ARG_UINT8(ikeys);
	INTERFACE_PACK_ARG_UINT8(rkeys);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();

}
void gap_bond_req_handler ( uint8_t *peer_addr, uint8_t oob, uint8_t iocap, uint8_t auth, uint8_t key_size,
		uint8_t ikey_dist,  uint8_t rkey_dist)
{
	INTERFACE_MSG_INIT(GAP_BOND_REQ, TASK_GAP, NMI_FALSE);
	INTERFACE_PACK_ARG_BLOCK(peer_addr, BD_ADDR_LEN);
	INTERFACE_PACK_ARG_UINT8(oob);
	INTERFACE_PACK_ARG_UINT8(iocap);
	INTERFACE_PACK_ARG_UINT8(auth);
	INTERFACE_PACK_ARG_UINT8(key_size);
	INTERFACE_PACK_ARG_UINT8(ikey_dist);
	INTERFACE_PACK_ARG_UINT8(rkey_dist);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();

}
void gap_le_create_conn_req_handler(uint16_t u16ScanIntv, uint16_t u16ScanWind, uint8_t u8InitFilterPolicy, uint8_t u8PeerAddrType,
									uint8_t *pu8PeerAddr, uint8_t u8OwnAddrType, uint16_t u16ConIntvMin, uint16_t u16ConIntvMax,
									uint16_t u16ConLatency, uint16_t u16SupervTo, uint16_t u16CeLenMin, uint16_t u16CeLenMax)
{
	INTERFACE_MSG_INIT(GAP_LE_CREATE_CONN_REQ, TASK_GAP, NMI_FALSE);
	INTERFACE_PACK_ARG_UINT16(u16ScanIntv);
	INTERFACE_PACK_ARG_UINT16(u16ScanWind);
	INTERFACE_PACK_ARG_UINT8(u8InitFilterPolicy);
	INTERFACE_PACK_ARG_UINT8(u8PeerAddrType);
	INTERFACE_PACK_ARG_BLOCK(pu8PeerAddr, BD_ADDR_LEN);
	INTERFACE_PACK_ARG_UINT8(u8OwnAddrType);
	INTERFACE_PACK_ARG_UINT8(0); //padding
	INTERFACE_PACK_ARG_UINT16(u16ConIntvMin);
	INTERFACE_PACK_ARG_UINT16(u16ConIntvMax);
	INTERFACE_PACK_ARG_UINT16(u16ConLatency);
	INTERFACE_PACK_ARG_UINT16(u16SupervTo);
	INTERFACE_PACK_ARG_UINT16(u16CeLenMin);
	INTERFACE_PACK_ARG_UINT16(u16CeLenMax);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
}

void gap_dev_inq_req_handler (uint8_t u8InqType, uint8_t u8OwnAddrType, uint16_t u16NbOfResp)
{
	INTERFACE_MSG_INIT(GAP_DEV_INQ_REQ, TASK_GAP, NMI_FALSE);
	INTERFACE_PACK_ARG_UINT8(u8InqType);
	INTERFACE_PACK_ARG_UINT8(u8OwnAddrType);
	INTERFACE_PACK_ARG_UINT16(u16NbOfResp);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
}

void gap_scan_req_handler (uint8_t u8ScanEn , uint8_t u8FilterDup)
{
	INTERFACE_MSG_INIT(GAP_SCAN_REQ, TASK_GAP, NMI_FALSE);
	INTERFACE_PACK_ARG_UINT8(u8ScanEn);
	INTERFACE_PACK_ARG_UINT8(u8FilterDup);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();
}
#endif

void gapm_white_list_mgm_cmd(uint8_t operation, uint8_t addr_type, uint8_t* address)
{
	// To do check complete effect operation or not
	INTERFACE_MSG_INIT(GAPM_WHITE_LIST_MGT_CMD, TASK_GAPM);
	INTERFACE_PACK_ARG_UINT8(GAPM_ADD_DEV_IN_WLIST);
	/// Number of device information present in command
	if(address != NULL)
	{
		INTERFACE_PACK_ARG_UINT8(1);
		INTERFACE_PACK_ARG_BLOCK(address, AT_BLE_ADDR_LEN);
		INTERFACE_PACK_ARG_UINT8(addr_type);
	}
	INTERFACE_SEND_WAIT(GAPM_CMP_EVT, TASK_GAPM);
	INTERFACE_DONE();
}

void gapm_adv_report_evt_handler(uint8_t* data,
	at_ble_scan_info_t* param)
{
	uint8_t evt_type;
	INTERFACE_UNPACK_INIT(data);
	INTERFACE_UNPACK_UINT8(&evt_type);
	INTERFACE_UNPACK_UINT8(&(param->dev_addr.type));
	INTERFACE_UNPACK_BLOCK(param->dev_addr.addr, AT_BLE_ADDR_LEN);
	INTERFACE_UNPACK_UINT8(&(param->adv_data_len));
	INTERFACE_UNPACK_BLOCK(param->adv_data ,param->adv_data_len);
	INTERFACE_DONE();
	
	switch(evt_type)
	{	
		case ADV_CONN_UNDIR:
			param->type = AT_BLE_ADV_TYPE_UNDIRECTED;
			break;

		case ADV_CONN_DIR:
			param->type = AT_BLE_ADV_TYPE_DIRECTED;
			break;

		case ADV_DISC_UNDIR:
			param->type = AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED;
			break;

		case ADV_NONCONN_UNDIR:
    		param->type = AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED;
			break;

		case LLM_SCAN_RSP:
			param->type = AT_BLE_ADV_TYPE_SCAN_RESPONSE;
			break;
			
	}


}



at_ble_events_t gapm_cmp_evt(uint8_t* data, void* params)
{
	uint8_t u8Operation;
	at_ble_events_t evt_num = -1;
	INTERFACE_UNPACK_INIT(data);
	INTERFACE_UNPACK_UINT8(&u8Operation);
	INTERFACE_DONE();

	if((u8Operation == GAPM_CONNECTION_AUTO) || (u8Operation == GAPM_ADV_UNDIRECT))
	{
		at_ble_connected_t *connected = (at_ble_connected_t*)params;
		connected->handle = device.conn_handle;
		connected->peer_addr.type = device.peer_addr.type;
		memcpy(connected->peer_addr.addr, device.peer_addr.addr, AT_BLE_ADDR_LEN);
		
		evt_num = AT_BLE_CONNECTED;
	}
	return evt_num;
}

