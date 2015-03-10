
#include "platform.h"
#include "at_ble_api.h"

#include "gapm_task.h"
#include "at_ble_api.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "gattm_task.h"

#include "device.h"

#include "interface.h"
#include "event.h"

#define EVENT_POOL_DEPTH 10

struct event {
	struct event* next;
	uint16_t msg_id;
	uint16_t src_id;
	void* data;
};

static struct event event_pool[EVENT_POOL_DEPTH];

static struct event *event_free_list = NULL;
static struct event* event_pending_list = NULL;

struct str_watched_event watched_event;

void event_free(struct event* event)
{
	event->next = event_free_list;
	event_free_list = event;
}

void event_post(uint16_t msg_id, uint16_t src_id, void * data)
{
	// get a free event object
	struct event* evt = event_free_list;
	if(evt != NULL)
	{
		event_free_list = evt->next;
		
		evt->next = NULL;
		evt->data = data;
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
		
		platform_event_signal();
	}
	
}

at_ble_status_t event_get(uint16_t* msg_id, uint16_t* src_id, uint8_t** data)
{
	struct event* event = NULL;

	while(event_pending_list == NULL)
	{	
		platform_event_wait();
	}
	
	event = event_pending_list;
	event_pending_list = event_pending_list->next;
	*src_id = event->src_id;
	*msg_id = event->msg_id;
	*data = event->data;

	event_free(event);

	return AT_BLE_SUCCESS;
	
}

void event_init()
{
	uint32_t i;
	for(i = 0; i < EVENT_POOL_DEPTH; i++)
	{
		event_free(&event_pool[i]);
	}
}

static at_ble_events_t handle_ble_event(uint16_t msg_id, uint16_t src_id, uint8_t* data, void* params)
{
	at_ble_events_t evt_num = -1;

	//printf("RW evt : 0x%x\n", msg_id);

	switch(msg_id)
	{
	
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
	default:
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
		platform_cmd_cmpl_signal();

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
	event_get(&msg_id, &src_id, &data);
	
	if(msg_id == 0xFFFF && src_id == 0xFFFF)
	{
		*event = AT_BLE_CUSTOM_EVENT;
		params = data;
	}
	else
	{
		*event = handle_ble_event(msg_id, src_id, data, params);
	}

	return AT_BLE_SUCCESS;
}

at_ble_status_t at_ble_event_user_defined_post(void* params)
{
	event_post(0xFFFF, 0xFFFF, params);
	return AT_BLE_SUCCESS;
}


