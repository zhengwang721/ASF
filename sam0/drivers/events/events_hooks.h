#include "events.h"

#ifndef _EVENTS_HOOKS_H_INCLUDED_
#define _EVENTS_HOOKS_H_INCLUDED_

enum events_interrupt_source {
	EVENTS_INTERRUPT_OVERRUN,
	EVENTS_INTERRUPT_DETECT,
};


enum status_code events_create_hook(struct events_hook *hook, events_interrupt_hook hook_func);

enum status_code events_add_hook(struct events_resource *resource, struct events_hook *hook);
enum status_code events_del_hook(struct events_resource *resource, struct events_hook *hook);

enum status_code events_enable_interrupt_source(struct events_resource *resource, enum events_interrupt_source source);

bool             events_is_interrupt_set(struct events_resource *resource, enum events_interrupt_source source);
enum status_code events_ack_interrupt(struct events_resource *resource, enum events_interrupt_source source);

#endif
