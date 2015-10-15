#ifndef __EVENT_H__
#define __EVENT_H__

struct str_watched_event
{
    uint16_t msg_id;
    uint16_t err_msg_id;
    uint16_t src_id;
    uint8_t *params;
};

extern struct str_watched_event watched_event;

void internal_event_init(void);

#ifndef NEW_EVT_HANDLER
void internal_event_post(uint16_t msg_id, uint16_t src_id, void *data);
#else
void internal_event_post(uint16_t msg_id, uint16_t src_id, void *data, uint16_t data_len);
#endif  //NEW_EVT_HANDLER

uint32_t special_events_handler(uint16_t msg_id, uint16_t src_id, uint8_t *data);
void internal_event_flush(uint16_t msg_id);
void internal_conn_flush(at_ble_handle_t conn_handle);

#endif
