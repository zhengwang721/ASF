#include "events.h"

typedef void (*events_interrupt_hook)(struct events_resource *resource);


enum status_code              events_add_hook(struct events_resource *resource);
enum status_code              events_del_hook(struct events_resource *resource);

enum events_interrput_source  events_get_interrupt_source(struct events_resource resource);
enum status_code              events_ack_interrupt_source(struct events_resource, enum events_interrput_source source);

