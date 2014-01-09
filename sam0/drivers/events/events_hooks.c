#include "events.h"
#include "events_hooks.h"
#include "system_interrupt.h"

#define _EVENTS_INTFLAGS_DETECT  0x0f00ff00
#define _EVENTS_INTFLAGS_OVERRUN 0x000f00ff

extern struct _events_module _events_inst;

enum status_code events_create_hook(struct events_hook *hook, events_interrupt_hook func)
{
	hook->next      = NULL;
	hook->resource  = NULL;
	hook->hook_func = func;

	return STATUS_OK;
}

enum status_code events_add_hook(struct events_resource *resource, struct events_hook *hook)
{
	struct events_hook *tmp_hook = NULL;

	hook->resource = resource;

	if (_events_inst.hook_list == NULL) {
		_events_inst.hook_list = hook;
	} else {

		tmp_hook = _events_inst.hook_list;

		while (tmp_hook->next != NULL) {
			tmp_hook = tmp_hook->next;
		}

		tmp_hook->next = hook;
	}

	if (!system_interrupt_is_enabled(SYSTEM_INTERRUPT_MODULE_EVSYS)) {
		system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_EVSYS);
	}


	return STATUS_OK;
}

enum status_code events_del_hook(struct events_resource *resource, struct events_hook *hook)
{
	return STATUS_OK;
}

enum status_code events_enable_interrupt_source(struct events_resource *resource, enum events_interrupt_source source)
{

	if (source == EVENTS_INTERRUPT_DETECT) {
		EVSYS->INTENSET.reg = _events_find_bit_position(resource->channel, _EVENTS_START_OFFSET_DETECTION_BIT);
	} else if (source == EVENTS_INTERRUPT_OVERRUN) {
		EVSYS->INTENSET.reg = _events_find_bit_position(resource->channel, _EVENTS_START_OFFSET_OVERRUN_BIT);
	} else {
		return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

bool events_is_interrupt_set(struct events_resource *resource, enum events_interrupt_source source)
{
	uint8_t bitpos;

	if (source == EVENTS_INTERRUPT_DETECT) {
		bitpos = _events_find_bit_position(resource->channel, _EVENTS_START_OFFSET_DETECTION_BIT);
	} else {
		bitpos = _events_find_bit_position(resource->channel, _EVENTS_START_OFFSET_OVERRUN_BIT);
	}

	return (bool)(_events_inst.interrupt_flag_buffer >> (bitpos - 1));
}

enum status_code events_ack_interrupt(struct events_resource *resource, enum events_interrupt_source source)
{
	uint8_t bitpos;

	if (source == EVENTS_INTERRUPT_DETECT) {
		bitpos = _events_find_bit_position(resource->channel, _EVENTS_START_OFFSET_DETECTION_BIT);
	} else {
		bitpos = _events_find_bit_position(resource->channel, _EVENTS_START_OFFSET_OVERRUN_BIT);
	}

	_events_inst.interrupt_flag_ack_buffer |= 1 << (bitpos - 1);

	return STATUS_OK;
}

void EVSYS_Handler(void)
{
	struct events_hook *current_hook = _events_inst.hook_list;

	_events_inst.interrupt_flag_buffer = EVSYS->INTFLAG.reg;

	while (current_hook != NULL) {
		current_hook->hook_func(current_hook->resource);
		current_hook = current_hook->next;
	}

	EVSYS->INTFLAG.reg = _events_inst.interrupt_flag_ack_buffer;
	_events_inst.interrupt_flag_buffer = 0;
}



