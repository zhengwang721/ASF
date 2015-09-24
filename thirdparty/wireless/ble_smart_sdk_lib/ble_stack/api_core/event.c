
#include "platform.h"
#include "at_ble_api.h"
#include "dbg_logs.h"
#include "gapm_task.h"
#include "at_ble_api.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "l2cc_task.h"
#include "htpt_task.h"
#include "device.h"

#include "interface.h"
#include "event.h"

#define EVENT_POOL_DEPTH 10

// Direct test mode DTM opcodes
#define HCI_RESET_CMD_OPCODE                        0x0C03
#define HCI_LE_RX_TEST_CMD_OPCODE                   0x201D
#define HCI_LE_TX_TEST_CMD_OPCODE                   0x201E
#define HCI_LE_TEST_END_CMD_OPCODE                  0x201F


struct event
{
    struct event *next;
    uint16_t msg_id;
    uint16_t src_id;
#ifdef NEW_EVT_HANDLER
    uint16_t data_len;
#endif  //NEW_EVT_HANDLER
    void *data;
};

static struct event event_pool[EVENT_POOL_DEPTH];

static struct event *event_free_list = NULL;
static struct event *event_pending_list = NULL;

struct str_watched_event watched_event;

static void event_free(struct event *event)
{
    event->next = event_free_list;
    event_free_list = event;
}

#ifndef NEW_EVT_HANDLER
void event_post(uint16_t msg_id, uint16_t src_id, void *data)
#else
void event_post(uint16_t msg_id, uint16_t src_id, void *data, uint16_t data_len)
#endif  //NEW_EVT_HANDLER
{
    // get a free event object
    struct event *evt = event_free_list;
    if (evt != NULL)
    {
        event_free_list = evt->next;
        evt->next = NULL;
        evt->data = data;
        evt->msg_id = msg_id;
        evt->src_id = src_id;
#ifdef NEW_EVT_HANDLER
        evt->data_len = data_len;
#endif  //NEW_EVT_HANDLER
        if (event_pending_list == NULL)
        {
            event_pending_list = evt;
        }
        else
        {
            struct event *cur = event_pending_list;
            while (cur->next != NULL)
            {
                cur = cur->next;
            }
            cur->next = evt;
        }
        //platform_event_signal();
    }
}

#ifndef NEW_EVT_HANDLER
static at_ble_status_t event_get(uint16_t *msg_id, uint16_t *src_id, uint8_t **data, uint32_t timeout)
#else
static at_ble_status_t event_get(uint16_t *msg_id, uint16_t *src_id, uint8_t **data, uint16_t *data_len, uint32_t timeout)
#endif  //NEW_EVT_HANDLER
{
	struct event *event = NULL;
	at_ble_status_t status = AT_BLE_SUCCESS;

	/* Check Timeout */
	if (timeout != 0xFFFFFFFF)
	{
#ifdef NEW_EVT_HANDLER
        start_timer(timeout);
#endif  //NEW_EVT_HANDLER
		//block till an event is posted or timeout
		while (event_pending_list == NULL && status != AT_BLE_TIMEOUT)
		{
			status = (at_ble_status_t)platform_event_wait(timeout);
		}
	}
	else // user wants no timeout
	{
		// block till an event is posted
		while (event_pending_list == NULL)
		{
			status = (at_ble_status_t)platform_event_wait(timeout);
		}
	}

	if (status != AT_BLE_TIMEOUT)
	{
		//some event available in the event pending list.
#ifdef NEW_EVT_HANDLER
		stop_timer();
#endif  //NEW_EVT_HANDLER

#ifndef SAMB11
		if (bus_type == AT_BLE_SPI)
		{
			//make sure there is no any pending platform events.
			while (check_pending_bus_events())
			{
				platform_event_wait(0);
			}
		}
#else
		if((plf_drv_status)status == STATUS_RECEIVED_PLF_EVENT_MSG) {
			*msg_id = 0xFFFE;
			status = AT_BLE_SUCCESS;
		}
		else if((plf_drv_status)status == STATUS_RECEIVED_BLE_MSG) {
			status = AT_BLE_SUCCESS;
		}
#endif

		event = event_pending_list;
		event_pending_list = event_pending_list->next;
		*src_id = event->src_id;
		*msg_id = event->msg_id;
#ifdef NEW_EVT_HANDLER
		*data_len = event->data_len;
#endif  //NEW_EVT_HANDLER
		*data = event->data;
		event_free(event);
	}
	return status;
}

void event_init()
{
    uint32_t i;
    for (i = 0; i < EVENT_POOL_DEPTH; i++)
    {
        event_free(&event_pool[i]);
    }
		event_pending_list = NULL;
}

#ifndef NEW_EVT_HANDLER
static at_ble_events_t handle_ble_event(uint16_t msg_id, uint16_t src_id, uint8_t *data, void *params)
#else
static at_ble_events_t handle_ble_event(uint16_t msg_id, uint16_t src_id, uint8_t *data, uint16_t data_len, void *params)
#endif  //NEW_EVT_HANDLER
{
    at_ble_events_t evt_num = AT_BLE_UNDEFINED_EVENT;
    PRINT_DBG("RW evt : 0x%04X\n", msg_id);
    switch (msg_id)
    {
    /****************/
    /**GAPM EVENTS **/
    /****************/
    case GAPM_DEVICE_READY_IND:
    {
        PRINT_INFO("Device Ready Indication.\r\n");
        evt_num = (at_ble_events_t)AT_BLE_DEVICE_READY;
    }
    break;
    case GAPM_CMP_EVT:
    {
        evt_num = (at_ble_events_t)gapm_cmp_evt(data, params);
    }
    break;
    case GAPM_DEV_BDADDR_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_RAND_ADDR_CHANGED;
        gapm_dev_bdaddr_ind_handler(data, (at_ble_addr_t *)params);
    }
    break;
    case GAPM_ADV_REPORT_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_SCAN_INFO;
        gapm_adv_report_evt_handler(data, (at_ble_scan_info_t *)params);
    }
    break;
    case GAPM_ADDR_SOLVED_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_RESOLV_RAND_ADDR_STATUS;
        gapm_addr_solved_ind_handler(data, (at_ble_resolv_rand_addr_status_t *)params);
    }
    break;
    case GAPM_PROFILE_ADDED_IND:
    {
        evt_num  = (at_ble_events_t)gapm_profile_added_ind_handler(data, params);
    }
    break;
    /****************/
    /**GAPC EVENTS **/
    /****************/
    case GAPC_CMP_EVT:
    {
        evt_num = (at_ble_events_t)gapc_cmp_evt(src_id, data, params);
    }
    break;
    case GAPC_PEER_ATT_INFO_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_PEER_ATT_INFO_IND;
        gapc_peer_att_info_ind_handler(src_id, data, (at_ble_peer_att_info_ind_t *) params);
    }
    break;
    case GAPC_CON_CHANNEL_MAP_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_CON_CHANNEL_MAP_IND;
        gapc_con_channel_map_ind_handler(src_id, data, (at_ble_channel_map_t *) params);
    }
    break;
    case GAPC_DISCONNECT_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_DISCONNECTED;
        gapc_disconnect_ind(data, (at_ble_disconnected_t *)params);
    }
    break;
    case GAPC_BOND_IND:
    {
        evt_num = (at_ble_events_t)gapc_bond_ind(src_id, data, (at_ble_pair_done_t *)params);
    }
    break;
    case GAPC_BOND_REQ_IND:
    {
        evt_num = (at_ble_events_t)gapc_bond_req_ind(src_id, data, params);
    }
    break;
    case GAPC_ENCRYPT_REQ_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_ENCRYPTION_REQUEST;
        gapc_encrypt_req_ind(src_id, data, (at_ble_encryption_request_t *)params);
    }
    break;
    case GAPC_ENCRYPT_IND: //This event in case of SLAVE
    {
        evt_num = (at_ble_events_t)AT_BLE_ENCRYPTION_STATUS_CHANGED;
        gapc_encrypt_ind(src_id , data, (at_ble_encryption_status_changed_t *)params);
    }
    break;
    /*
     * the Implementation of this event will include only the device name request  GAPC_DEV_NAME
     * for now, and it will be handled implicitly without notifying the user
     * TODO: handle the remaining info that could be requested  GAPC_DEV_APPEARANCE and  GAPC_DEV_SLV_PREF_PARAMS
     */
    case GAPC_GET_DEV_INFO_REQ_IND:
    {
        // event not handled
        evt_num = (at_ble_events_t)AT_BLE_EVENT_MAX;
        gapc_dev_info_req_ind_handler(src_id, data, params);
    }
    break;
    case GAPC_SECURITY_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_SLAVE_SEC_REQUEST;
        gapc_sec_req_ind(src_id, data, (at_ble_slave_sec_request_t *)params);
    }
    break;
    case GAPC_SIGN_COUNTER_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_SIGN_COUNTERS_IND;
        gapc_sign_counter_ind_handler(src_id, data, (at_ble_sign_counter_t *)params);
    }
    break;
    case GAPC_LECB_CONNECT_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_LECB_CONNECTED;
        gapc_lecb_connected(data, (at_ble_lecb_connected_t *)params);
    }
    break;
    case GAPC_LECB_DISCONNECT_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_LECB_DISCONNECTED;
        gapc_lecb_disconnected(data, (at_ble_lecb_disconnected_t *)params);
    }
    break;
    case GAPC_LECB_CONNECT_REQ_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_LECB_CONN_REQ;
        gapc_lecb_connect_req_ind(data, (at_ble_lecb_conn_req_t *)params);
    }
    break;
    case GAPC_LECB_ADD_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_LECB_ADD_CREDIT_IND;
        gapc_lecb_add_credit_resp_handler(data, (at_ble_lecb_add_credit_ind_t *)params);
    }
    break;
    case GAPC_PARAM_UPDATED_IND:
    {
        evt_num = (at_ble_events_t)gapc_param_updated_ind(src_id, data, (at_ble_conn_param_update_done_t *)params);
    }
    break;
    case GAPC_PARAM_UPDATE_REQ_IND:
    {
        evt_num = (at_ble_events_t)gapc_param_updated_req_ind(src_id, data, (at_ble_conn_param_update_request_t *)params);
    }
    break;
    /****************/
    /**L2CC EVENTS**/
    /****************/
    case L2CC_PDU_SEND_RSP:
    {
        evt_num = (at_ble_events_t)AT_BLE_LECB_SEND_RESP;
        l2cc_data_send_rsp_handler(data, (at_ble_lecb_send_rsp_t *)params);
    }
    break;
    case L2CC_LECNX_DATA_RECV_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_LECB_DATA_RECIEVED;
        l2cc_data_received_handler(data, (at_ble_lecb_data_recv_t *)params);
    }
    break;
    /****************/
    /**GATTC EVENTS**/
    /****************/
    case GATTC_DISC_SVC_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_PRIMARY_SERVICE_FOUND;
        gattc_disc_svc_ind_parser(src_id, data, (at_ble_primary_service_found_t *)params);
    }
    break;
    case GATTC_DISC_SVC_INCL_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_INCLUDED_SERVICE_FOUND;
        gattc_disc_svc_incl_ind_parser(src_id, data, (at_ble_included_service_found_t *)params);
    }
    break;
    case GATTC_DISC_CHAR_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_CHARACTERISTIC_FOUND;
        gattc_disc_char_ind_parser(src_id, data, (at_ble_characteristic_found_t *)params);
    }
    break;
    case GATTC_DISC_CHAR_DESC_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_DESCRIPTOR_FOUND;
        gattc_disc_char_desc_ind_parser(src_id, data, (at_ble_descriptor_found_t *)params);
    }
    break;
    case GATTC_EVENT_IND:
    {
        evt_num = (at_ble_events_t)gattc_event_ind_and_notification_parser(src_id, data, params);
    }
    break;
    case GATTC_EVENT_REQ_IND:
    {
        at_ble_indication_recieved_t *params_indicate = (at_ble_indication_recieved_t *)params;
        evt_num = (at_ble_events_t)gattc_event_ind_and_notification_parser(src_id, data, params);
        /* send indication Event confirm to peer*/
        gattc_event_cfm_handler(params_indicate->conn_handle, params_indicate->char_handle);
    }
    break;
    case GATTC_MTU_CHANGED_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_MTU_CHANGED_INDICATION;
        gattc_event_mtu_changed_ind_parser(src_id, data, (at_ble_mtu_changed_ind_t *)params);
    }
    break;
    case GATTC_CMP_EVT:
    {
        evt_num = (at_ble_events_t)gattc_complete_evt_handler(src_id, data, params);
    }
    break;
    case GATTC_READ_REQ_IND:
    {
        ((at_ble_characteristic_read_req_t *)params)->conn_handle = KE_IDX_GET(src_id);
        evt_num = (at_ble_events_t)gatts_handle_read_req(data, params);
        //at_ble_indication_send(0,9);
    }
    break;
    case GATTC_WRITE_REQ_IND:
    {
        ((at_ble_characteristic_write_req_t *)params)->conn_handle = KE_IDX_GET(src_id);
        evt_num = (at_ble_events_t)gatts_handle_write_req(data, params);
    }
    break;
    case GATTC_ATT_INFO_REQ_IND:
    {
        PRINT_INFO("GATTC_ATT_INFO_REQ_IND\n");
        gatts_handle_att_info_req(data, params, src_id);
        evt_num = (at_ble_events_t)AT_BLE_EVENT_MAX ;
    }
    break;
    case GATTC_READ_IND:
    {
        PRINT_INFO("GATTC_READ_IND Received \n");
        evt_num = (at_ble_events_t)AT_BLE_CHARACTERISTIC_READ_RESPONSE;
        gattc_read_ind_parser(src_id, data, (at_ble_characteristic_read_response_t *)params);
    }
    break;
    case GATTC_SVC_CHANGED_CFG_IND:
        PRINT_INFO("GATTC_SVC_CHANGED_CFG_IND\r\n");
        evt_num = AT_BLE_CHARACTERISTIC_CONFIGURATION_CHANGED;
        gattc_svc_changed_cfg_ind_parser(src_id, data, (at_ble_characteristic_configuration_changed_t *)params);
        break;
    /**************/
    /*HTPT Profile*/
    /**************/
    case HTPT_ENABLE_RSP:
    {
        evt_num = (at_ble_events_t)AT_BLE_HTPT_ENABLE_RSP;
        htpt_enable_rsp_handler(src_id, data, (at_ble_htpt_enable_rsp_t *)params);
    }
    break;
    case HTPT_TEMP_SEND_RSP:
    {
        evt_num = (at_ble_events_t)AT_BLE_HTPT_TEMP_SEND_CFM;
        htpt_temp_send_rsp_handler(src_id, data, (at_ble_htpt_temp_send_cfm_t *)params);
    }
    break;
    case HTPT_MEAS_INTV_UPD_RSP:
    {
        evt_num = (at_ble_events_t)AT_BLE_HTPT_MEAS_INTV_UPD_RSP;
        htpt_meas_intv_upd_rsp_handler(src_id, data, (at_ble_htpt_meas_intv_upd_rsp_t *)params);
    }
    break;
    case HTPT_MEAS_INTV_CHG_REQ_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_HTPT_MEAS_INTV_CHG_REQ;
        htpt_meas_intv_chg_req_ind_handler(src_id, data, (at_ble_htpt_meas_intv_chg_req_t *)params);
    }
    break;
    case HTPT_CFG_INDNTF_IND:
    {
        evt_num = (at_ble_events_t)AT_BLE_HTPT_CFG_INDNTF_IND;
        htpt_cfg_indntf_ind_handler(src_id, data, (at_ble_htpt_cfg_indntf_ind_t *)params);
    }
    break;
    case AT_BLE_CUSTOM_EVENT:
    {
        // deliver the event to main app
        evt_num = (at_ble_events_t)AT_BLE_CUSTOM_EVENT;
    }
    break;
    /****************/
    /*UnKnown EVENTS*/
    /****************/
    default:
    {
        PRINT_ERR("Unhandled:\n\tsrc_id:0x%04x\n\tmsg_id:0x%04x\n", src_id, msg_id);
    }
    break;
    }
    return evt_num;
}

uint32_t special_events_handler(uint16_t msg_id, uint16_t src_id, uint8_t *data)
{
    uint32_t consumed = 0;
    if (msg_id == watched_event.msg_id
            && src_id == watched_event.src_id)
    {
        watched_event.msg_id = 0xFFFF;
        watched_event.params = data;
        platform_cmd_cmpl_signal();
        consumed = 1;
    }
    else if (msg_id == watched_event.err_msg_id
             && src_id == watched_event.src_id)
    {
        watched_event.err_msg_id = 0xFFFF;
        watched_event.params = data;
        platform_cmd_cmpl_signal();
        consumed = 1;
    }
    else
    {
        switch (msg_id)
        {
        //In case of GAPM_CONNECTION_NAME_REQUEST connection request
        case GAPC_CONNECTION_REQ_IND:
        {
            gapc_con_req_ind(data);
            consumed = 1;
        }
        break;
        //In case of GAPM_CONNECTION_DIRECT and GAPM_CONNECTION_AUTO connections
        case GAPM_PEER_NAME_IND:
        {
            /* TODO:
             * handle request and Wait for complete event
             * without user intervention
             * Incomming data :
             *      struct bd_addr addr;
             *      uint8_t addr_type;
             *      uint8_t name_len;
             *      uint8_t name[__ARRAY_EMPTY];
             *
             *  It will be consumed = 1;
             */
        }
        break;
        case GAPM_ADV_REPORT_IND:
        {
            /*TODO: complete this implemntaion in case of selective connection mode is used*/
            /*extern bool bSelectiveConnModeRunning;
            if(true == bSelectiveConnModeRunning)
            {
                gapm_adv_report_ind_handler(data);


                consumed = 1;
            }*/
        }
        break;
        case GAPC_BOND_IND:
        {
            if (AT_BLE_UNDEFINED_EVENT == gapc_bond_ind(src_id, data, NULL))
            {
                consumed = 1;
            }
        }
        break;
        case GAPC_BOND_REQ_IND:
        {
            if (AT_BLE_UNDEFINED_EVENT == gapc_bond_req_ind(src_id, data, NULL))
            {
                consumed = 1;
            }
        }
        break;
        case GAPC_ENCRYPT_IND:  //neglect this event in case of MASTER
        {
            uint8_t index = check_ConnData_idx_role(KE_IDX_GET(src_id), NULL);
            if ((AT_BLE_MAX_CONCURRENT_CONNS > index) && (ROLE_MASTER == gstrConnData[index].role))
            {
                consumed = 1;
            }
        }
        break;
        default:
        {
            consumed = 0;
        }
        break;
        }
        PRINT_DBG("RW evt* [%d]: 0x%04X\n", consumed, msg_id);
    }
    return consumed;
}


static at_ble_events_t handle_ble_hci_dtm_event(uint8_t *_data, at_ble_dtm_t *params)
{
    uint16_t dtm_opcode = 0;
    uint8_t dtm_status = AT_BLE_SUCCESS;
    uint16_t number_of_packets = 0;
    at_ble_events_t evt_num = AT_BLE_UNDEFINED_EVENT;
    params->number_of_packets = 0x00;           // intial value in case of not used
    /*unpack data to get opcode, status and number of packets*/
    INTERFACE_UNPACK_INIT(_data);
    INTERFACE_UNPACK_SKIP(1);           // no interest in this byte as it is alwas 0x01
    INTERFACE_UNPACK_UINT16(&dtm_opcode);
    INTERFACE_UNPACK_UINT8(&dtm_status);
    INTERFACE_UNPACK_UINT16(&number_of_packets);
    INTERFACE_DONE();
    params->status = dtm_status;
    params->op_code = dtm_opcode;
    /* set the event number according to the Opcode */
    switch (dtm_opcode)
    {
    case HCI_RESET_CMD_OPCODE :
    // Reset response status, no parameters
    /*
     * fall through
     */
    case HCI_LE_RX_TEST_CMD_OPCODE:
    // rx test response status, no parameters
    /*
     * fall through
     */
    case HCI_LE_TX_TEST_CMD_OPCODE:
        // tx test response status, no parameters
        evt_num = AT_BLE_LE_TEST_STATUS;
        break;
    case HCI_LE_TEST_END_CMD_OPCODE:
        // end test response status, parameters: number of received packets
        evt_num = AT_BLE_LE_PACKET_REPORT;
        /*number of packets copy*/
        params->number_of_packets = number_of_packets;
        break;
    default:
        // log error unidentified opcode
        break;
    }
    return evt_num;
}

at_ble_status_t at_ble_event_get(at_ble_events_t *event, void *params,
                                 uint32_t timeout)
{
    uint16_t msg_id, src_id;
    uint8_t *data;
    at_ble_status_t status = AT_BLE_SUCCESS;
    do
    {
#ifndef NEW_EVT_HANDLER
        status = event_get(&msg_id, &src_id, &data, timeout);
#else
        uint16_t data_len;
        status = event_get(&msg_id, &src_id, &data, &data_len, timeout);
#endif  //NEW_EVT_HANDLER
        PRINT_DBG("RW_EVENT : status = 0x%02X, msg_id = 0x%04X, src_id = 0x%04X, data_len = 0x%04X, data = ", status, msg_id, src_id, data_len);
        PRINT_BUS("", data, data_len);
        PRINT_BUS("\n", 0, 0);
        if (status == AT_BLE_SUCCESS)
        {
            if (msg_id == 0xFFFF && src_id == 0xFFFF)
            {
                *event = AT_BLE_CUSTOM_EVENT;
                params = data;
            }
						else if(msg_id == 0xFFFE) {
							//platform event.
							*event = AT_PLATFORM_EVENT;
							params = NULL;
						}
            else if ((src_id == AT_BLE_HCI_DTM_EVENT) && (msg_id == AT_BLE_HCI_DTM_EVENT))
            {
                *event = handle_ble_hci_dtm_event(data, (at_ble_dtm_t *)params);
            }
            else
            {
#ifndef NEW_EVT_HANDLER
                *event = handle_ble_event(msg_id, src_id, data, params);
#else
                *event = handle_ble_event(msg_id, src_id, data, data_len, params);
#endif  //NEW_EVT_HANDLER
            }
        }
    }
    while (*event == AT_BLE_EVENT_MAX);
    return status;
}

// this flag is used only with custom events to bypass the platform UART wait
volatile uint8_t gu8UserDefinedEvent = 0;

at_ble_status_t at_ble_event_user_defined_post(void *params)
{
#ifndef NEW_EVT_HANDLER
    event_post(0xFFFF, 0xFFFF, params);
#else
    event_post(0xFFFF, 0xFFFF, params, 0);
#endif  //NEW_EVT_HANDLER
    gu8UserDefinedEvent = 1;
    return AT_BLE_SUCCESS;
}
void at_ble_event_flush(uint16_t msg_id)
{
    struct event *prev_event = NULL;
    struct event *next_event = NULL;
    struct event *curr_event = event_pending_list;
    while (curr_event)
    {
        next_event = curr_event->next;
        if (curr_event->msg_id == msg_id)
        {
            // remove event from pending list
            if (prev_event == NULL)
            {
                event_pending_list = curr_event->next;
            }
            else
            {
                prev_event->next = curr_event->next;
            }
            // add event to free list
            curr_event->next = event_free_list;
            event_free_list = curr_event;
        }
        else
        {
            prev_event = curr_event;
        }
        curr_event = next_event;
    }
}
void at_ble_conn_flush(at_ble_handle_t conn_handle)
{
    struct event *prev_event = NULL;
    struct event *next_event = NULL;
    struct event *curr_event = event_pending_list;
    while (curr_event)
    {
        at_ble_handle_t handle = KE_IDX_GET(curr_event->src_id);
        uint16_t task_type = KE_TASK_GET(curr_event->src_id);
        next_event = curr_event->next;
        if (handle == conn_handle &&
                (task_type == TASK_GAPC || task_type == TASK_GATTC))
        {
            // remove event from pending list
            if (prev_event == NULL)
            {
                event_pending_list = curr_event->next;
            }
            else
            {
                prev_event->next = curr_event->next;
            }
            // add event to free list
            curr_event->next = event_free_list;
            event_free_list = curr_event;
        }
        else
        {
            prev_event = curr_event;
        }
        curr_event = next_event;
    }
}
