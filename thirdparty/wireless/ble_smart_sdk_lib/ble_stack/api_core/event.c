
#include "platform_drv.h"
#include "at_ble_api.h"

#include "gapm_task.h"
#include "at_ble_api.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "htpt_task.h"
#include "device.h"

#include "interface.h"
#include "event.h"

#define EVENT_POOL_DEPTH 10

struct event {
	struct event* next;
	uint16_t msg_id;
	uint16_t src_id;
	uint16_t data_len;
	void* data;
};

static struct event event_pool[EVENT_POOL_DEPTH];

static struct event *event_free_list = NULL;
static struct event* event_pending_list = NULL;
struct str_watched_event watched_event;
static uint8_t cmd_cmpl_flag;

void cmd_cmpl_wait(bool* timeout)
{
	while(cmd_cmpl_flag != 1) {
		platform_event_wait(0);
	}
	cmd_cmpl_flag = 0;
	
}

void cmd_cmpl_signal(void)
{
	cmd_cmpl_flag = 1;
}

static void event_free(struct event* event)
{
	event->next = event_free_list;
	event_free_list = event;
}

void event_post(uint16_t msg_id, uint16_t src_id, void * data, uint16_t data_len)
{
	// get a free event object
	struct event* evt = event_free_list;
	if(evt != NULL)
	{
		event_free_list = evt->next;
		
		evt->next = NULL;
		evt->data = data;
		evt->data_len = data_len;
		evt->msg_id = msg_id;
		evt->src_id = src_id;

		if(event_pending_list == NULL)
		{
			event_pending_list = evt;
		}
		else
		{
			struct event* cur = event_pending_list;
			while(cur->next != NULL)
			{
				cur = cur->next;
			}
			cur->next = evt;
		}
		
		//platform_event_signal();
	}
	
}

static at_ble_status_t event_get(uint16_t* msg_id, uint16_t* src_id, uint8_t** data, uint32_t timeout)
{
	struct event* event = NULL;
	at_ble_status_t status= AT_BLE_SUCCESS;
	plf_drv_status	plf_status = STATUS_FAILURE;
	if (timeout != 0xFFFFFFFF) 
	{
		//block till an event is posted or timeout
		while(event_pending_list == NULL && (status != AT_BLE_TIMEOUT)
													&& (plf_status != STATUS_RECEIVED_PLF_EVENT_MSG))
		{	
			plf_status = platform_event_wait(timeout);
		}
	}
	else // user wants no timeout
	{
		// block till an event is posted
		while(event_pending_list == NULL && (plf_status != STATUS_RECEIVED_PLF_EVENT_MSG))
		{	
			plf_status = platform_event_wait(timeout);
		}
	}
	if(plf_status == STATUS_RECEIVED_PLF_EVENT_MSG) {
		*msg_id = 0xFFFE;
	}
	else {
		event = event_pending_list;
		event_pending_list = event_pending_list->next;
		*src_id = event->src_id;
		*msg_id = event->msg_id;
		*data = event->data;
		event_free(event);
	}
	return status;
	
}

void event_init()
{
	uint32_t i;
	event_free_list = NULL;
	event_pending_list = NULL;
	cmd_cmpl_flag = 0;
	memset(&watched_event,0,sizeof(struct str_watched_event));
	for(i = 0; i < EVENT_POOL_DEPTH; i++)
	{
		event_free(&event_pool[i]);
	}
}

static at_ble_events_t handle_ble_event(uint16_t msg_id, uint16_t src_id, uint8_t* data, void* params)
{
	at_ble_events_t evt_num = AT_BLE_UNDEFINED_EVENT;

	//printf("RW evt : 0x%x\n", msg_id);

	switch(msg_id)
	{
		case GAPM_DEVICE_READY_IND:
		{
			evt_num = AT_BLE_DEVICE_READY;
		}
		break;
		case GAPM_CMP_EVT:
		{
			evt_num = gapm_cmp_evt(data, params);
		}
		break;

		case GAPM_ADV_REPORT_IND:
		{
			evt_num = AT_BLE_SCAN_INFO;
			gapm_adv_report_evt_handler(data, (at_ble_scan_info_t*)params);
		}
		break;

		case GAPC_PARAM_UPDATED_IND:
		{
			evt_num = AT_BLE_CONN_PARAM_UPDATE_DONE;
			gapc_param_updated_ind(src_id, data, (at_ble_conn_param_update_done_t *)params);
		}
		break;

		case GAPC_PARAM_UPDATE_REQ_IND:
		{
			evt_num = AT_BLE_CONN_PARAM_UPDATE_DONE;
			gapc_param_update_req_ind(src_id, data, (at_ble_conn_param_update_request_t *)params);
		}
		break;
	
		case GATTC_WRITE_CMD_IND:
		{
			evt_num = AT_BLE_CHARACTERISTIC_CHANGED;
			gattc_write_cmd_ind(src_id, data, (at_ble_characteristic_changed_t *) params);
		}
		break;
	
		case GAPC_DISCONNECT_IND:
		{
			evt_num = AT_BLE_DISCONNECTED;
			gapc_disconnect_ind(data, (at_ble_disconnected_t*)params);
		}
		break;
	
		case GAPC_BOND_IND:
		{
			evt_num = gapc_bond_ind(src_id, data, (at_ble_pair_done_t*)params);
		}
		break;
	
		case GAPC_BOND_REQ_IND:
		{
			evt_num = gapc_bond_req_ind(src_id, data, params);
		}
		break;

		case GAPC_CMP_EVT:
		{
			evt_num = gapc_cmp_evt(src_id, data, (at_ble_encryption_status_changed_t*)params);
		}
		break;

		case GAPC_ENCRYPT_IND:
		{
			if(device.role == ROLE_SLAVE)
			{
				evt_num = AT_BLE_ENCRYPTION_STATUS_CHANGED;
				gapc_encrypt_ind(src_id ,data,(at_ble_encryption_status_changed_t*)params);
			}
		}
		break;
		
		case GAPC_ENCRYPT_REQ_IND:
		{
			evt_num = AT_BLE_ENCRYPTION_REQUEST;
			gapc_encrypt_req_ind(src_id, data, (at_ble_encryption_request_t *)params);
		}
		break;

		case GAPC_CON_RSSI_IND:
		{
			evt_num = AT_BLE_RX_POWER_VALUE;
			gapc_con_rssi_ind_parser(src_id,data,(gapc_con_rssi_ind* )params);
		}
		break;

		case GATTC_CMP_EVT:
		{
			evt_num = gattc_complete_evt_handler(src_id, data, (at_ble_discovery_complete_t *)params);
		}
		break;
	
		case GATTC_DISC_SVC_IND:
		{
			evt_num = AT_BLE_PRIMARY_SERVICE_FOUND;
			gattc_disc_svc_ind_parser(src_id, data, (at_ble_primary_service_found_t *)params);
		}
		break;

		case GATTC_DISC_SVC_INCL_IND:
		{
			evt_num = AT_BLE_INCLUDED_SERVICE_FOUND;
			gattc_disc_svc_incl_ind_parser(src_id, data, (at_ble_included_service_found_t*)params);
		}
		break;
	
		case GATTC_DISC_CHAR_IND:
		{
			evt_num = AT_BLE_CHARACTERISTIC_FOUND;
			gattc_disc_char_ind_parser(src_id, data, (at_ble_characteristic_found_t *)params);
		}
		break;
	
		case GATTC_DISC_CHAR_DESC_IND:
		{
			evt_num = AT_BLE_DESCRIPTOR_FOUND;
			gattc_disc_char_desc_ind_parser(src_id, data, (at_ble_descriptor_found_t*)params);
		}
		break;

		case GATTC_READ_IND:
		{
			evt_num = AT_BLE_CHARACTERISTIC_READ_RESPONSE;
			gattc_read_ind_parser(src_id, data, (at_ble_characteristic_read_response_t*)params);
		}
		break;

		case GATTC_EVENT_IND:
		{
			evt_num = gattc_event_ind_parser(src_id, data, params);
		}
		break;
	
		case GAPC_SECURITY_IND:
		{
			evt_num = AT_BLE_SLAVE_SEC_REQUEST;
			gapc_sec_req_ind(src_id,data,(at_ble_slave_sec_request_t*)params);
		}
		break;
		
		case GAPM_DEV_BDADDR_IND:
		{
			evt_num = AT_BLE_RAND_ADDR_CHANGED;
			gapm_dev_bdaddr_ind_handler(data,(at_ble_rand_addr_changed_t*)params);
		}
		break;
		
		case GAPM_ADDR_SOLVED_IND:
		{
			evt_num = AT_BLE_RESOLV_RAND_ADDR_STATUS;
			gapm_addr_solved_ind_handler(data,(at_ble_resolv_rand_addr_status_t*)params);
		}
		break;
		
		case HTPT_CREATE_DB_CFM:
		{
			evt_num = AT_BLE_HTPT_CREATE_DB_CFM;
			htpt_create_db_cfm_handler(src_id,data, (at_ble_htpt_create_db_cfm_t*)params);
		}
		break;
		
		case HTPT_DISABLE_IND:
		{
			evt_num = AT_BLE_HTPT_DISABLE_IND;
			htpt_disable_ind_handler(src_id,data, (at_ble_htpt_disable_ind_t*)params);
		}
		break;
		
		case HTPT_ERROR_IND:
		{
			evt_num = AT_BLE_HTPT_ERROR_IND;
			htpt_error_ind_handler(src_id,data, (at_ble_prf_server_error_ind_t*)params);
		}
		break;
		
		case HTPT_TEMP_SEND_CFM:
		{
			evt_num = AT_BLE_HTPT_TEMP_SEND_CFM;
			htpt_temp_send_cfm_handler(src_id,data, (at_ble_htpt_temp_send_cfm_t*)params);
		}
		break;
		
		case HTPT_MEAS_INTV_CHG_IND:
		{
			evt_num = AT_BLE_HTPT_MEAS_INTV_CHG_IND;
			htpt_meas_intv_chg_ind_handler(src_id,data,(at_ble_htpt_meas_intv_chg_ind_t*)params);
		}
		break;
		
		case HTPT_CFG_INDNTF_IND:
		{
			evt_num = AT_BLE_HTPT_CFG_INDNTF_IND;
			htpt_cfg_indntf_ind_handler(src_id,data,(at_ble_htpt_cfg_indntf_ind_t*)params);
		}
		break;
		
		default:
		{
			//printf("unhandled:\n\tsrc_id:0x%04x\n\tmsg_id:0x%04x\n\tdata  :0x%02X,0x%02X\n", src_id, msg_id, data[0], data[1]);	
		}
		break;
	}

	return evt_num;
}

uint32_t special_events_handler(uint16_t msg_id, uint16_t src_id, uint8_t* data)
{
	uint32_t consumed = 0;

	if(msg_id == watched_event.msg_id 
		&& src_id == watched_event.src_id)
	{
		watched_event.msg_id = 0xFFFF;
		watched_event.params = data;
		cmd_cmpl_signal();

		consumed = 1;
	}
	else if(msg_id == GAPC_CONNECTION_REQ_IND)
	{
		gapc_con_req_ind(data);
		consumed = 1;
	}

	return consumed;
}

at_ble_status_t at_ble_event_get(at_ble_events_t* event, void* params,
	uint32_t timeout)
{
	uint16_t msg_id, src_id;
	uint8_t* data;
	at_ble_status_t status = AT_BLE_SUCCESS;
	status = event_get(&msg_id, &src_id, &data, timeout);
	
	if (status == AT_BLE_SUCCESS)
	{
		if(msg_id == 0xFFFF && src_id == 0xFFFF)
		{
			*event = AT_BLE_CUSTOM_EVENT;
			params = data;
		}
		else if(msg_id == 0xFFFE) {
			//platform event.
			*event = AT_PLATFORM_EVENT;
			params = NULL;
		}
		else
		{
			*event = handle_ble_event(msg_id, src_id, data, params);
		}
	}
	return status;
}

at_ble_status_t at_ble_event_user_defined_post(void* params)
{
	event_post(0xFFFF, 0xFFFF, params, 0);
	return AT_BLE_SUCCESS;
}


