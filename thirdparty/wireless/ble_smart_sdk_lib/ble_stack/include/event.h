#ifndef __EVENT_H__
#define __EVENT_H__

struct str_watched_event{
	uint16_t msg_id;
	uint16_t src_id;
	uint8_t* params;
};

extern struct str_watched_event watched_event;

void event_init(void);

void event_post(uint16_t msg_id, uint16_t src_id, void * data, uint16_t data_len);

uint32_t special_events_handler(uint16_t msg_id, uint16_t src_id, uint8_t* data);

void cmd_cmpl_wait(bool* timeout);

void cmd_cmpl_signal(void);


#endif
